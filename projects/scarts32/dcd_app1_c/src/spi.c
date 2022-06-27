/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          spi.c                                   */
/*   Description:   SPI-Interface driver                    */
/*   Autor:         Johannes Natter                         */
/*   Date:          09.09.2007                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "cstdlib.h"
#include "modules.h"
#include "spi.h"

#define DUMMY_DATA                  0x00

/* SPI Control- and Statusbit defines */
#define SSPSTAT_RESET_STATE         0x3F
#define SSPSTAT_BUFFER_FULL         0x01
#define SSPCON1_RESET_STATE         0x00
#define SERIAL_PORT_ENABLE          0x20
#define SERIAL_MODE_MASTER_FOSC4    0x00

/* ******************** local variables ******************* */
static UINT1 lsb_first;

/* ******************** LOCAL FUNCTIONS ******************* */
/* ******************************************************** */


/* **************************************************** */
/* Initialize spi interface                             */
/* **************************************************** */
UINT1 WAPI spi_init(void)
{
    p_scarts->gpio.gpo = SPI_CS_IDLE_HIGH;
    lsb_first = 0;

    return 0;
}

/* **************************************************** */
/* Start a read-sequence                                */
/* **************************************************** */
UINT1 WAPI spi_read(UINT1 slave_select)
{
    p_scarts->gpio.gpo &= ~slave_select;
    p_scarts->spi.wr_data = DUMMY_DATA;
    while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
    p_scarts->gpio.gpo |= slave_select;
    return p_scarts->spi.rd_data;
}

/* **************************************************** */
/* Start a write-sequence                               */
/* **************************************************** */
void WAPI spi_write(UINT1 data, UINT1 slave_select)
{
    p_scarts->gpio.gpo &= ~slave_select;
    p_scarts->spi.wr_data = data;
    while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
    p_scarts->gpio.gpo |= slave_select;
}

/* **************************************************** */
/* Start a write n-sequence                             */
/* **************************************************** */
void spi_wr_n(UINT1 *p_data, ULONG4 len, UINT1 slave_select)
{
    p_scarts->gpio.gpo &= ~slave_select;
    while(len--)
    {
        p_scarts->spi.wr_data = *p_data++;
        while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
    }
    p_scarts->gpio.gpo |= slave_select;
}

/* **************************************************** */
/* Start a write-read-sequence                          */
/* **************************************************** */
UINT1 WAPI spi_wr_rd(UINT1 data, UINT1 slave_select)
{
    p_scarts->gpio.gpo &= ~slave_select;
    p_scarts->spi.wr_data = data;
    while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
    p_scarts->spi.wr_data = DUMMY_DATA;
    while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
    p_scarts->gpio.gpo |= slave_select;
    return p_scarts->spi.rd_data;
}

/* **************************************************** */
/* Start a write-write-sequence                         */
/* **************************************************** */
void WAPI spi_wr_wr(UINT1 data1, UINT1 data2, UINT1 slave_select)
{
    p_scarts->gpio.gpo &= ~slave_select;
    p_scarts->spi.wr_data = data1;
    while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
    p_scarts->spi.wr_data = data2;
    while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
    p_scarts->gpio.gpo |= slave_select;
}

/* **************************************************** */
/* Start a write-read n-times-sequence                  */
/* **************************************************** */
void WAPI spi_wr_rd_n(UINT1 data, UINT1 slave_select, MCP_LEN_TYPE size, UINT1 *p_data)
{
    MCP_LEN_TYPE i;

    p_scarts->gpio.gpo &= ~slave_select;
    p_scarts->spi.wr_data = data;
    while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);

    for(i = 0; i < size; i++)
    {
        p_scarts->spi.wr_data = DUMMY_DATA;
        while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
        *p_data++ = p_scarts->spi.rd_data;
    }

    p_scarts->gpio.gpo |= slave_select;
}

/* **************************************************** */
/* Start a write-write n-times-sequence                 */
/* **************************************************** */
void WAPI spi_wr_wr_n(UINT1 data, UINT1 slave_select, MCP_LEN_TYPE size, UINT1 *p_data)
{
    MCP_LEN_TYPE i;

    p_scarts->gpio.gpo &= ~slave_select;
    p_scarts->spi.wr_data = data;
    while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);

    for(i = 0; i < size; i++)
    {
        p_scarts->spi.wr_data = *p_data++;
        while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
    }

    p_scarts->gpio.gpo |= slave_select;
}

/* **************************************************** */
/* Start a write n-read n-sequence                      */
/* **************************************************** */
void spi_wr_n_rd_n(UINT1 *p_wr_data, ULONG4 wr_len, UINT1 *p_rd_data, ULONG4 rd_len, UINT1 slave_select)
{
    p_scarts->gpio.gpo &= ~slave_select;
    while(wr_len--)
    {
        p_scarts->spi.wr_data = *p_wr_data++;
        while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
    }

    if(lsb_first)
        p_scarts->spi.gen_if.usr_conf |= SPI_REG_CONF_LSB;
    else
        p_scarts->spi.gen_if.usr_conf &= ~SPI_REG_CONF_LSB;

    while(rd_len--)
    {
        p_scarts->spi.wr_data = DUMMY_DATA;
        while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
        *p_rd_data++ = p_scarts->spi.rd_data;
    }

    p_scarts->spi.gen_if.usr_conf &= ~SPI_REG_CONF_LSB;
    p_scarts->gpio.gpo |= slave_select;
}

/* **************************************************** */
/* Start a write n-read n-sequence                      */
/* **************************************************** */
void spi_wr_n_wr_n(UINT1 *p_data0, ULONG4 len0, UINT1 *p_data1, ULONG4 len1, UINT1 slave_select)
{
    p_scarts->gpio.gpo &= ~slave_select;
    while(len0--)
    {
        p_scarts->spi.wr_data = *p_data0++;
        while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
    }

    if(lsb_first)
        p_scarts->spi.gen_if.usr_conf |= SPI_REG_CONF_LSB;
    else
        p_scarts->spi.gen_if.usr_conf &= ~SPI_REG_CONF_LSB;

    while(len1--)
    {
        p_scarts->spi.wr_data = *p_data1++;
        while(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY);
    }

    p_scarts->spi.gen_if.usr_conf &= ~SPI_REG_CONF_LSB;
    p_scarts->gpio.gpo |= slave_select;
}

void spi_set_lsb_first(UINT1 lsb_first_true)
{
    lsb_first = lsb_first_true;
}

void spi_set_clock_2x(UINT1 clock_2x_true)
{
    if(clock_2x_true)
        p_scarts->spi.gen_if.usr_conf |= SPI_REG_CONF_CLOCK_2X;
    else
        p_scarts->spi.gen_if.usr_conf &= ~SPI_REG_CONF_CLOCK_2X;
}
