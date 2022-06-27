/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          nodesw.c                                */
/*   Description:   Node switch reader                      */
/*   Autor:         Johannes Natter                         */
/*   Date:          26.12.2009                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "cstdlib.h"
#include "spi.h"
#include "nodesw.h"

UINT1 get_node_number(void)
{
    //return spi_read(SPI_CS_NODE); DEBUG: Not implemented!
    return 0; /* TODO Read Nodenumber */
}
