/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          flash.c                                 */
/*   Description:   Flash controller                        */
/*   Tested on:     A25LQ032                                */
/*   Autor:         Johannes Natter                         */
/*   Date:          25.06.2012                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "cstdlib.h"
#include "flash.h"
#include "spi.h"

typedef struct
{
    FLASH_MEMORY_OBJ_TYPE   obj;

    /* Memory structure */
    ULONG4  addr_mask_byte;
    ULONG4  addr_mask_page;
    ULONG4  addr_mask_sector;
    ULONG4  addr_mask_block;
    UINT2   page_size;

    /* SPI commands */
    UINT1   write_enable;
    UINT1   write_bytes;
    UINT1   read_bytes;
    UINT1   sector_erase;
} FLASH_MEMORY_INT_OBJ_TYPE;

/* FLASH_MEMORY_INT_OBJ_TYPE flash_memory */

#define SPI_CMD_PP              0x02    /* Page Program (Write)     - 1Byte PP, 3Byte ADDR, nByte Data */
#define SPI_CMD_READ            0x03    /* READ                     - 1Byte READ, 3Byte ADDR, nByte Data */
#define SPI_CMD_RDSR1           0x05    /* Read Status Register 1   - 1Byte RDSR1, nByte Data */
#define SPI_CMD_WREN            0x06    /* WRite Enable             - 1Byte WREN */
#define SPI_CMD_SE              0x20    /* Sector Erase             - 1Byte SE, 3Byte ADDR */

#define ADDR_BYTE_MASK          ((ULONG4)0x000000FF) /* 256 Bytes in 1 Page */
#define ADDR_PAGE_MASK          ((ULONG4)0x00000F00) /* 16 Pages in 1 Sector */
#define ADDR_SECTOR_MASK        ((ULONG4)0x0000F000) /* 16 Sectors in 1 Block */
#define ADDR_BLOCK_MASK         ((ULONG4)0x003F0000) /* 64 Blocks in 1 Memory */

#define ADDR_MASK               (ADDR_BLOCK_MASK | ADDR_SECTOR_MASK | ADDR_PAGE_MASK | ADDR_BYTE_MASK)

#define STATUS_REG1_WIP         0x01    /* Write In Progress Bit */

#define PAGE_SIZE               ((UINT2)256)

/* ******************** LOCAL FUNCTIONS ******************* */
static void swap_addr(ULONG4 *addr);
/* ******************************************************** */

/* *************************************************** */
/* flash: create flash memory controller object        */
/* *************************************************** */
FLASH_MEMORY_OBJ_TYPE *flash_memory_create_obj(FLASH_MEMORY_CTRL_TYPE *ctrl)
{
    /* TODO: Flash memory controller object */
    return 0;
}

/* *************************************************** */
/* flash: write memory                                 */
/* *************************************************** */
void flash_write_mem(UINT1 *p_data, ULONG4 start_addr, ULONG4 len)
{
    ULONG4 wr_len, spi_cmd;

    if(0 == (start_addr & (ADDR_PAGE_MASK | ADDR_BYTE_MASK)))
    {
        spi_write(SPI_CMD_WREN, SPI_CS_FLASH);
        spi_cmd = (((ULONG4)SPI_CMD_SE) << 24) | (start_addr & (ADDR_BLOCK_MASK | ADDR_SECTOR_MASK));
        swap_addr(&spi_cmd);
        spi_wr_n((UINT1 *)&spi_cmd, sizeof(spi_cmd), SPI_CS_FLASH);
        while(spi_wr_rd(SPI_CMD_RDSR1, SPI_CS_FLASH) & STATUS_REG1_WIP);
    }

    while(len)
    {
        wr_len = MIN(len, PAGE_SIZE);

        spi_write(SPI_CMD_WREN, SPI_CS_FLASH);
        spi_cmd = (((ULONG4)SPI_CMD_PP) << 24) | (start_addr & ADDR_MASK);
        swap_addr(&spi_cmd);
        spi_wr_n_wr_n((UINT1 *)&spi_cmd, sizeof(spi_cmd), p_data, wr_len, SPI_CS_FLASH);
        while(spi_wr_rd(SPI_CMD_RDSR1, SPI_CS_FLASH) & STATUS_REG1_WIP);

        p_data += wr_len;
        start_addr += wr_len;
        len -= wr_len;
    }
}

/* *************************************************** */
/* flash: read memory                                  */
/* *************************************************** */
void flash_read_mem(UINT1 *p_data, ULONG4 start_addr, ULONG4 len)
{
    start_addr = (((ULONG4)SPI_CMD_READ) << 24) | (start_addr & ADDR_MASK);
    swap_addr(&start_addr);
    spi_wr_n_rd_n((UINT1 *)&start_addr, sizeof(start_addr), p_data, len, SPI_CS_FLASH);
}

/* *************************************************** */
/* flash: swap address bytes helper function           */
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
