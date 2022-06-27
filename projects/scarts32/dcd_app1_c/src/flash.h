/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          flash.h                                 */
/*   Description:   Flash controller                        */
/*   Tested on:     A25LQ032                                */
/*   Autor:         Johannes Natter                         */
/*   Date:          25.06.2012                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#ifndef FLASH_CTRL_H
#define FLASH_CTRL_H

#include "cstdlib.h"

typedef struct
{
    /* Memory structure */
    UINT2   bytes_per_page;
    UINT2   pages_per_sector;
    UINT2   sectors_per_block;
    UINT2   blocks_per_memory;

    /* SPI commands */
    UINT1   write_enable;
    UINT1   write_bytes;
    UINT1   read_bytes;
    UINT1   sector_erase;
} FLASH_MEMORY_CTRL_TYPE;

typedef struct
{
    void *p_int;
} FLASH_MEMORY_OBJ_TYPE;

FLASH_MEMORY_OBJ_TYPE  *flash_memory_create_obj     (FLASH_MEMORY_CTRL_TYPE *ctrl);
void                    flash_write_mem             (UINT1 *p_data, ULONG4 start_addr, ULONG4 len);
void                    flash_read_mem              (UINT1 *p_data, ULONG4 start_addr, ULONG4 len);

#endif
