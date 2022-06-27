/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          spi.h                                   */
/*   Description:   SPI-Interface driver                    */
/*   Autor:         Johannes Natter                         */
/*   Date:          09.09.2007                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#ifndef SPI_H
#define SPI_H

#include "cstdlib.h"

/* SPI useroptions */
#define SPI_CLK_IDLE_LOW    0x00
#define SPI_CLK_SELECT      0x40

/* SPI chipselect-pins */
#define SPI_CS_MAIN_ENC     0x01
#define SPI_CS_FLASH        0x02
#define SPI_CS_EPCS16       0x04
#define SPI_CS_GDB_ENC      0x08 /* Should not be visible for user */

/* SPI chipselect-pins */
#define SPI_CS_IDLE_HIGH    (SPI_CS_MAIN_ENC  | \
                             SPI_CS_FLASH  | \
                             SPI_CS_EPCS16 | \
                             SPI_CS_GDB_ENC)

#define SPI_CS_IDLE_LOW     (0)

/* SPI functionprototypes */
UINT1   WAPI    spi_init            (void);
UINT1   WAPI    spi_read            (UINT1 slave_select);
void    WAPI    spi_write           (UINT1 data, UINT1 slave_select);
void            spi_wr_n            (UINT1 *p_data, ULONG4 len, UINT1 slave_select);
UINT1   WAPI    spi_wr_rd           (UINT1 data, UINT1 slave_select);
void    WAPI    spi_wr_wr           (UINT1 data1, UINT1 data2, UINT1 slave_select);
void    WAPI    spi_wr_rd_n         (UINT1 data, UINT1 slave_select, MCP_LEN_TYPE size, UINT1 *p_data);
void    WAPI    spi_wr_wr_n         (UINT1 data, UINT1 slave_select, MCP_LEN_TYPE size, UINT1 *p_data);
void            spi_wr_n_rd_n       (UINT1 *p_wr_data, ULONG4 wr_len, UINT1 *p_rd_data, ULONG4 rd_len, UINT1 slave_select);
void            spi_wr_n_wr_n       (UINT1 *p_wr_data, ULONG4 wr_len, UINT1 *p_rd_data, ULONG4 rd_len, UINT1 slave_select);
void            spi_set_lsb_first   (UINT1 lsb_first_true);
void            spi_set_clock_2x    (UINT1 clock_2x_true);

#endif
