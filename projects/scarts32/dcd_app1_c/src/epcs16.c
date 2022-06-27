/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          epcs16.c                                */
/*   Description:   EPCS16 controller                       */
/*   Autor:         Johannes Natter                         */
/*   Date:          03.07.2012                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "cstdlib.h"
#include "epcs16.h"
#include "spi.h"

#define SPI_CMD_WRITE           0x02    /* Write Bytes              - 1Byte WRITE, 3Byte ADDR, nByte Data */
#define SPI_CMD_READ            0x03    /* Read Bytes               - 1Byte READ, 3Byte ADDR, nByte Data */
#define SPI_CMD_RS              0x05    /* Read Status              - 1Byte RS, nByte Data */
#define SPI_CMD_WREN            0x06    /* WRite Enable             - 1Byte WREN */
#define SPI_CMD_ES              0xD8    /* Erase Sector             - 1Byte ES, 3Byte ADDR */

#define ADDR_BYTE_MASK          ((ULONG4)0x000000FF) /* 256 Bytes in 1 Page */
#define ADDR_PAGE_MASK          ((ULONG4)0x0000FF00) /* 256 Pages in 1 Sector */
#define ADDR_SECTOR_MASK        ((ULONG4)0x001F0000) /* 32 Sectors in 1 Memory */

#define ADDR_MASK               (ADDR_SECTOR_MASK | ADDR_PAGE_MASK | ADDR_BYTE_MASK)

#define STATUS_REG_WIP          0x01    /* Write In Progress Bit */

#define PAGE_SIZE               ((UINT2)256)

/* ******************** LOCAL FUNCTIONS ******************* */
static void swap_addr(ULONG4 *addr);
/* ******************************************************** */

/* *************************************************** */
/* epcs16: write memory                                */
/* *************************************************** */
void epcs16_write_mem(UINT1 *p_data, ULONG4 start_addr, ULONG4 len)
{
    ULONG4 wr_len, spi_cmd;

    if(0 == (start_addr & (ADDR_PAGE_MASK | ADDR_BYTE_MASK)))
    {
        spi_write(SPI_CMD_WREN, SPI_CS_EPCS16);
        spi_cmd = (((ULONG4)SPI_CMD_ES) << 24) | (start_addr &  ADDR_SECTOR_MASK);
        swap_addr(&spi_cmd);
        spi_wr_n((UINT1 *)&spi_cmd, sizeof(spi_cmd), SPI_CS_EPCS16);
        while(spi_wr_rd(SPI_CMD_RS, SPI_CS_EPCS16) & STATUS_REG_WIP);
    }

    while(len)
    {
        wr_len = MIN(len, PAGE_SIZE);

        spi_write(SPI_CMD_WREN, SPI_CS_EPCS16);
        spi_cmd = (((ULONG4)SPI_CMD_WRITE) << 24) | (start_addr & ADDR_MASK);
        swap_addr(&spi_cmd);
        spi_set_lsb_first(1);
        spi_wr_n_wr_n((UINT1 *)&spi_cmd, sizeof(spi_cmd), p_data, wr_len, SPI_CS_EPCS16);
        spi_set_lsb_first(0);
        while(spi_wr_rd(SPI_CMD_RS, SPI_CS_EPCS16) & STATUS_REG_WIP);

        p_data += wr_len;
        start_addr += wr_len;
        len -= wr_len;
    }
}

/* *************************************************** */
/* epcs16: read memory                                 */
/* *************************************************** */
void epcs16_read_mem(UINT1 *p_data, ULONG4 start_addr, ULONG4 len)
{
    start_addr = (((ULONG4)SPI_CMD_READ) << 24) | (start_addr & ADDR_MASK);
    swap_addr(&start_addr);
    spi_set_lsb_first(1);
    spi_wr_n_rd_n((UINT1 *)&start_addr, sizeof(start_addr), p_data, len, SPI_CS_EPCS16);
    spi_set_lsb_first(0);
}

/* *************************************************** */
/* epcs16: swap address bytes helper function          */
/* *************************************************** */
static void swap_addr(ULONG4 *addr)
{
    UINT1 *p_1 = (UINT1 *)addr;
    UINT1 *p_2 = (UINT1 *)addr;

    p_2 += 3;

    *p_1 ^= *p_2;
    *p_2 ^= *p_1;
    *p_1 ^= *p_2;

    ++p_1;
    --p_2;

    *p_1 ^= *p_2;
    *p_2 ^= *p_1;
    *p_1 ^= *p_2;
}
