/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          timefcts.h                              */
/*   Description:   Time/Tick functions for Microchip's     */
/*                  TCP/IP-Stack                            */
/*   Autor:         Johannes Natter                         */
/*   Date:          23.12.2009                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#ifndef TIMEFCTS_H
#define TIMEFCTS_H

#include "cstdlib.h"

typedef UINT2 TICK;

#ifdef C18C
#define TICKS_PER_125USEC           1
#define TICKS_PER_MSEC              8
#elif defined(__SCARTS_32__) /* TU Wien scarts32 */
#define TICKS_PER_250USEC           1
#define TICKS_PER_MSEC              4
#else
#define TICKS_PER_125USEC           1 /* min 1msec on win */
#define TICKS_PER_MSEC              1
#endif

#define TICK_MSEC                   ((ULONG4)TICKS_PER_MSEC)
#define TICK_SECOND                 ((ULONG4)TICKS_PER_MSEC*1000)
#define TICK_MINUTE                 ((ULONG4)TICKS_PER_SECOND)*60ull)
#define TICK_HOUR                   ((ULONG4)TICKS_PER_SECOND*3600ull)

UINT2   TickGet                     (void);

#endif
