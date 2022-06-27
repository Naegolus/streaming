/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          hardware.h                              */
/*   Description:   Hardware funktions                      */
/*   Autor:         Johannes Natter                         */
/*   Date:          07.09.2007                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#ifndef HARDWARE_H
#define HARDWARE_H

#include "cstdlib.h"

void    hw_init                     (void);
void    enable_irq                  (void);
void    start_firmware              (void);
UINT1   write_mem                   (UINT1 *p_data, UINT2 mem_id, UINT2 mem_idx, ULONG4 start_addr, ULONG4 len);
UINT1   read_mem                    (UINT1 *p_data, UINT2 mem_id, UINT2 mem_idx, ULONG4 start_addr, ULONG4 len);
void    signal_app                  (void);
UINT2   get_tick_cnt                (void);
void    trigger_pin_pulse           (void);

#endif
