/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          cstdlib.c                               */
/*   Description:   Standard library                        */
/*   Autor:         Johannes Natter                         */
/*   Date:          09.09.2007                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "cstdlib.h"

#define PORTABLE

#ifdef PORTABLE
void mcpy(UINT1 *s1, const UINT1 *s2, MCP_LEN_TYPE n)
{
    while(n--)
        *s1++ = *s2++;
}

INT1 mcmp(UINT1 *s1, const UINT1 *s2, MCP_LEN_TYPE len)
{
    MCP_LEN_TYPE i;

    for(i = 0; i < len; i++)
    {
        if(*s1++ != *s2++)
            return i + 1;
    }
    return 0;
}
#else /* scarts32 Hardware */
void mcpy(UINT1 *s1, const UINT1 *s2, MCP_LEN_TYPE n)
{
}

/* !!!ASM!!! */
INT1 mcmp(UINT1 *s1, const UINT1 *s2, MCP_LEN_TYPE len)
{
    return 0;
}

#endif
