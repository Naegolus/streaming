/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          bsl_cmd.c                               */
/*   Description:   BSL_SLAVE Commando Interface            */
/*   Autor:         Johannes Natter                         */
/*   Date:          25.07.2012                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "cstdlib.h"
#include "bsl_cmd.h"
#include "hardware.h"

#include "TCPIP.h"

/* BSL State Machine */
typedef enum
{
    BSL_SM_GET_SOCKET = 0,
    BSL_SM_LISTENING
} BSL_SM_ENUM;

/* bootloader commands */
typedef enum
{
    BSL_CMD_RD_VERSION = 0,
    BSL_CMD_START_FW,
    BSL_CMD_RD_MEM,
    BSL_CMD_WR_MEM,
    BSL_CMD_GET_MEM_LEN
} BSL_CMD_ENUM;

/* BSL_Protocol */
#define BSL_PROTOCOL_KEY_LEN        12
const UINT1 bsl_key[] = "BSL_Protocol";

typedef struct
{
    UINT1   bsl_key[BSL_PROTOCOL_KEY_LEN];
    UINT2   mem_id;
    UINT2   mem_idx;
    ULONG4  data_len;
    ULONG4  data_addr;
    UINT2   cmd_id;
    UINT1   command;
    UINT1   chk_sum;
} BSL_CMD_TYPE;

typedef struct
{
    BSL_SM_ENUM sm_state;
    UDP_SOCKET h_socket;
} BSL_CMD_DRV_TYPE;

/* bootloader constants */
#define BSL_CHK_SUM_LEN             (sizeof(BSL_CMD_TYPE) - 1)
#define BSL_PROTOCOL_CHK_SUM_START  0x00
#define CMD_BUFF_CPY_SIZE           256

/* network constants */
#define BSL_UDP_PORT                22150

/* int. var. */
UINT1 data_buff[CMD_BUFF_CPY_SIZE];
BSL_CMD_DRV_TYPE bsl;

/* ******************** LOCAL FUNCTIONS ******************* */
static  void    command_int         (BSL_CMD_TYPE *cmd);
static  void    set_chk_sum         (BSL_CMD_TYPE *cmd);
static  UINT1   ver_chk_sum         (BSL_CMD_TYPE *cmd);
static  UINT1   calc_chk_sum        (UINT1 *p_data);
/* ******************************************************** */

/* *************************************************** */
/* BSL SLAVE: Cyclic task                              */
/* *************************************************** */
void BSLInit(void)
{
    bsl.sm_state = BSL_SM_GET_SOCKET;
}

/* *************************************************** */
/* BSL SLAVE: Cyclic task                              */
/* *************************************************** */
void BSLTask(void)
{
    switch(bsl.sm_state)
    {
        case BSL_SM_GET_SOCKET:
            // Open a socket to send and receive broadcast messages on
            bsl.h_socket = UDPOpen(BSL_UDP_PORT, NULL, BSL_UDP_PORT);
            if(INVALID_UDP_SOCKET == bsl.h_socket)
                break;

            bsl.sm_state = BSL_SM_LISTENING;

        case BSL_SM_LISTENING:
            if(sizeof(BSL_CMD_TYPE) > UDPIsGetReady(bsl.h_socket))
                break;

            /* data received */
            {
                BSL_CMD_TYPE cmd;
                UDPGetArray((BYTE *)&cmd, sizeof(BSL_CMD_TYPE));
                command_int(&cmd);
            }

            break;
    }
}

/* *************************************************** */
/* BSL SLAVE: Commando interpreter                     */
/* *************************************************** */
static void command_int(BSL_CMD_TYPE *cmd)
{
    UINT2 pkt_len, sw_ver = SW_VERSION;

    if(mcmp(cmd->bsl_key, bsl_key, BSL_PROTOCOL_KEY_LEN))
        return;

    if(ver_chk_sum(cmd))
        return;

    cmd->cmd_id = ~cmd->cmd_id;

    switch(cmd->command)
    {
        case BSL_CMD_RD_VERSION:
            cmd->data_len          = 0x0002;
            set_chk_sum(cmd);
            pkt_len = sizeof(BSL_CMD_TYPE) + cmd->data_len;

            if(UDPIsPutReady(bsl.h_socket) < pkt_len)
                return;

            UDPPutArray((UINT1 *)cmd, sizeof(BSL_CMD_TYPE));
            UDPPutArray((UINT1 *)&sw_ver, sizeof(UINT2));
            break;

        case BSL_CMD_START_FW:
#if 0
            cmd->data_len          = 0x0000;
            set_chk_sum(cmd);

            MACPutHeader(&mac_address_bc, MAC_UNKNOWN, sizeof(BSL_CMD_TYPE));
            MACPutArray((UINT1 *)cmd, sizeof(BSL_CMD_TYPE));
            MACFlush();
#endif

            start_firmware();
            return;

        case BSL_CMD_RD_MEM:
        case BSL_CMD_WR_MEM:
            {
                UINT2 dat_len = cmd->data_len;
                MCP_LEN_TYPE copy_len;

                if(BSL_CMD_WR_MEM == cmd->command)
                {
                    cmd->data_len = 0x0000;

                    if(UDPIsGetReady(bsl.h_socket) != dat_len)
                        return;
                }

                set_chk_sum(cmd);

                pkt_len = sizeof(BSL_CMD_TYPE) + cmd->data_len;

                if(UDPIsPutReady(bsl.h_socket) < pkt_len)
                    return;
                UDPPutArray((UINT1 *)cmd, sizeof(BSL_CMD_TYPE));

                while(dat_len)
                {
                    copy_len = MIN(dat_len, CMD_BUFF_CPY_SIZE);

                    if(BSL_CMD_WR_MEM == cmd->command)
                    {
                        UDPGetArray(data_buff, copy_len);
                        if(write_mem(data_buff, cmd->mem_id, cmd->mem_idx, cmd->data_addr, copy_len))
                            return;
                    }else{
                        if(read_mem(data_buff, cmd->mem_id, cmd->mem_idx, cmd->data_addr, copy_len))
                            return;
                        UDPPutArray(data_buff, copy_len);
                    }

                    cmd->data_addr += copy_len;
                    dat_len -= copy_len;
                }
            }
            break;

        default:
            return;
    }
    UDPFlush();
}

/* *************************************************** */
/* BSL SLAVE: update checksum                          */
/* *************************************************** */
static void set_chk_sum(BSL_CMD_TYPE *cmd)
{
    cmd->chk_sum = calc_chk_sum((UINT1 *)cmd);
}

/* *************************************************** */
/* BSL SLAVE: verify checksum                          */
/* *************************************************** */
static UINT1 ver_chk_sum(BSL_CMD_TYPE *cmd)
{
    if(cmd->chk_sum == calc_chk_sum((UINT1 *)cmd))
        return 0;
    else
        return 1;
}

/* *************************************************** */
/* BSL SLAVE: calculate checksum                       */
/* *************************************************** */
static UINT1 calc_chk_sum(UINT1 *p_data)
{
    UINT1 i, tmp_data = BSL_PROTOCOL_CHK_SUM_START;

    if(0 == p_data)
        return 0;

    for(i = 0; i < BSL_CHK_SUM_LEN; ++i)
        tmp_data += *p_data++;

    return ~tmp_data;
}
