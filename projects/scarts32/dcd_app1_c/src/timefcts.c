/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          timefcts.c                              */
/*   Description:   Time/Tick functions for Microchip's     */
/*                  TCP/IP-Stack                            */
/*   Autor:         Johannes Natter                         */
/*   Date:          23.12.2009                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "cstdlib.h"
#include "hardware.h"
#include "timefcts.h"

UINT2 TickGet(void)
{
    return get_tick_cnt();
}
