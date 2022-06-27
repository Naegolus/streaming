/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          cstdlib.h                               */
/*   Description:   Standard library                        */
/*   Autor:         Johannes Natter                         */
/*   Date:          09.09.2007                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#ifndef CSTDLIB_H
#define CSTDLIB_H

#if defined(C18C)           /* C18-Compiler for PIC */
    typedef unsigned    char        UINT1;
    typedef             char        INT1;
    typedef unsigned    int         UINT2;
    typedef             int         INT2;
    typedef unsigned    long        ULONG4;
    typedef             long        LONG4;
    typedef             UINT1       MCP_LEN_TYPE;   /* PIC Adressbereich */
    typedef             UINT2       BLOCK_LEN_TYPE; /* ENC Adressbereich */

    #define WAPI
    #define GET_TICK_CNT()          0

    #define casm        _asm
    #define cendasm     _endasm
#elif defined(__SCARTS_32__) /* TU Wien scarts32 */
    typedef unsigned    char        UINT1;
    typedef             char        INT1;
    typedef unsigned    short       UINT2;
    typedef             short       INT2;
    typedef unsigned    int         ULONG4;
    typedef             int         LONG4;
    typedef             ULONG4      MCP_LEN_TYPE;
    typedef             UINT2       BLOCK_LEN_TYPE;

    #define WAPI
#elif defined(SYS_WIN)      /* Microsoft-Compiler */
    typedef unsigned    char        UINT1;
    typedef             char        INT1;
    typedef unsigned    short       UINT2;
    typedef             short       INT2;
    typedef unsigned    int         ULONG4;
    typedef             int         LONG4;
    typedef             ULONG4      MCP_LEN_TYPE;
    typedef             UINT2       BLOCK_LEN_TYPE;

    #define WAPI                    _stdcall
    #define GET_TICK_CNT()          0
#elif defined(SYS_AVR)      /* AVR-GCC */
    typedef unsigned    char        UINT1;
    //typedef             char        INT1;
    typedef unsigned    int         UINT2;
    //typedef             int         INT2;
    typedef unsigned    long        ULONG4;
    typedef             long        LONG4;
    typedef             UINT2       MCP_LEN_TYPE;   /* AVR Adressbereich */
#else
#error "No System spezified"
#endif /* SYS_xyz */

#define MIN(a, b)       ((a)<(b)?(a):(b))

/* TODO Software exception
__DATE__
__TIME__
__FILE__
__LINE__
*/

#ifdef USE_EXCEPTIONS
#define THROW_EXCEPTION(cond, info) \
{ \
    if(cond) \
    { \
        /* Display Line 1: __FILE__, __LINE__ */
        /* Display Line 2: Info: [info]*/
        /* error-log. first: mac, second: mmc */
        /* wait? */
    } \
}
#else
#define THROW_EXCEPTION(cond, info);
#endif

extern void     mcpy                (UINT1 *s1, const UINT1 *s2, MCP_LEN_TYPE n);
extern INT1     mcmp                (UINT1 *s1, const UINT1 *s2, MCP_LEN_TYPE len);

#endif
