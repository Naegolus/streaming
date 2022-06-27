/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          eth.h                                   */
/*   Description:   Ethernethandling                        */
/*   Autor:         Johannes Natter                         */
/*   Date:          29.10.2007                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#ifndef ETH_H
#define ETH_H

#include "cstdlib.h"
#include "GenericTypeDefs.h"

typedef struct
{
    MAC_ADDR        DestMACAddr;
    MAC_ADDR        SourceMACAddr;
    WORD_VAL        Type;
} ETHER_HEADER;

#define MAC_IP                      (0x00u)
#define MAC_ARP                     (0x06u)
#define MAC_UNKNOWN                 (0xFFu)

#define RAMSIZE                     (8*1024ul)
#define TMIT_PKT_CTRL_BYTE          1ul
#define TMIT_PKT_MAX_SIZE           1518ul
#define TMIT_PKT_STATUS_BYTES       7ul
#define RESERVED_HTTP_MEMORY        0ul
#define RESERVED_SSL_MEMORY         0ul
                                                                                                               /* |                       USE FRAM!!                          | */
#define TXSTART                     (RAMSIZE - (TMIT_PKT_CTRL_BYTE + TMIT_PKT_MAX_SIZE + TMIT_PKT_STATUS_BYTES) - TCP_ETH_RAM_SIZE - RESERVED_HTTP_MEMORY - RESERVED_SSL_MEMORY)
#define RXSTART                     (0ul)                       // Should be an even memory address; must be 0 for errata
#define RXSTOP                      ((TXSTART-2ul) | 0x0001ul)  // Odd for errata workaround
#define RXSIZE                      (RXSTOP-RXSTART+1ul)
#define BASE_TX_ADDR                (TXSTART + TMIT_PKT_CTRL_BYTE)
#define BASE_TCB_ADDR               (BASE_TX_ADDR + (1514ul+7ul))
#define BASE_HTTPB_ADDR             (BASE_TCB_ADDR + TCP_ETH_RAM_SIZE)
#define BASE_SSLB_ADDR              (BASE_HTTPB_ADDR + RESERVED_HTTP_MEMORY)

#define MAC_TX_BUFFER_SIZE          ((UINT2)1500)

/* Public functions */
/* -------------------------------------------------------------------------------------------------------------- */
void            eth_init                (void);
void            MACSendDiag             (UINT1 *p_data, BLOCK_LEN_TYPE size);
void            MACPutHeader            (MAC_ADDR *remote, BYTE type, WORD dataLen);
BOOL            MACGetHeader            (MAC_ADDR *remote, BYTE* type);
void            MACPutArray             (UINT1 *p_buffer, MCP_LEN_TYPE size);
MCP_LEN_TYPE    MACGetArray             (UINT1 *p_buffer, MCP_LEN_TYPE size);
void            MACPut                  (BYTE val);
BYTE            MACGet                  (void);
void            MACFlush                (void);
UINT2           MACSetWritePtr          (UINT2 addr);
BOOL            MACIsTxReady            (void);
void            MACMemCopyAsync         (WORD destAddr, WORD sourceAddr, WORD len);
BOOL            MACIsMemCopyDone        (void);
WORD            MACCalcRxChecksum       (WORD offset, WORD len);
WORD            CalcIPBufferChecksum    (WORD len);
BOOL            MACIsLinked             (void);
MAC_ADDR        *get_bc_addr            (void);
void            MACSetReadPtrInRx       (WORD offset);
WORD            MACSetReadPtr           (WORD address);
void            MACDiscardRx            (void);
void            eth_close               (void);
/* -------------------------------------------------------------------------------------------------------------- */

/* Microchip functions
               -WORD MACGetFreeRxSize(void);
               -void MACPutROMArray(ROM BYTE *val, WORD len);
               -void MACPowerDown(void);
               -void MACPowerUp(void);
               -void SetRXHashTableEntry(MAC_ADDR DestMACAddr);
*/
#endif
