/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          epcs16.h                                */
/*   Description:   EPCS16 controller                       */
/*   Autor:         Johannes Natter                         */
/*   Date:          03.07.2012                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#ifndef EPCS16_H
#define EPCS16_H

#include "cstdlib.h"

void    epcs16_write_mem            (UINT1 *p_data, ULONG4 start_addr, ULONG4 len);
void    epcs16_read_mem             (UINT1 *p_data, ULONG4 start_addr, ULONG4 len);

#endif
