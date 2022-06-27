/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          net.c                                   */
/*   Description:   Networkmanager                          */
/*   Autor:         Johannes Natter                         */
/*   Date:          03.02.2008                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "cstdlib.h"
#include "net.h"
#define THIS_IS_STACK_APPLICATION
#include "TCPIP.h"
#include "bsl_cmd.h"

typedef struct
{
    NODE_INFO           remoteNode;
} NET_TYPE;

APP_CONFIG AppConfig;

NET_TYPE net;

/* ******************** LOCAL FUNCTIONS ******************* */
static  void    mc_stack_init           (void);
static  void    stack_tsk               (void);
static  void    stack_app               (void);
/* ******************************************************** */


/* **************************************************** */
/* Initialize Network                                   */
/* **************************************************** */
void net_init(void)
{
    AppConfig.Flags.bIsDHCPEnabled = TRUE;

    eth_init();

    ip_init();

    mc_stack_init();
}

/* **************************************************** */
/* Initialize Microchip's TCP/IP-Stack                  */
/* **************************************************** */
static void mc_stack_init(void)
{
#if defined(STACK_USE_IP_GLEANING) || defined(STACK_USE_DHCP_CLIENT)
    AppConfig.Flags.bInConfigMode = TRUE;
#endif

    ARPInit();

#if defined(STACK_USE_UDP)
    UDPInit();
#endif

#if defined(STACK_USE_TCP)
    TCPInit();
#endif

#if defined(STACK_USE_DHCP_CLIENT)
    DHCPInit(0);
    if(!AppConfig.Flags.bIsDHCPEnabled)
    {
        DHCPDisable(0);
    }
#endif

    BSLInit();
}

/* **************************************************** */
/* Cyclic network task                                  */
/* **************************************************** */
void net_cycl(void)
{
    stack_tsk();
    stack_app();
}

static BOOL bLastLinkState = FALSE;

static void stack_tsk(void)
{
    WORD dataCount;
    IP_ADDR tempLocalIP;
    BYTE cFrameType;
    BYTE cIPFrameType;

    #if defined(STACK_USE_DHCP_CLIENT)
    /* Normally, an application would not include  DHCP module
       if it is not enabled. But in case some one wants to disable
       DHCP module at run-time, remember to not clear our IP
       address if link is removed. */
    if(AppConfig.Flags.bIsDHCPEnabled)
    {
        BOOL bCurrentLinkState;

        bCurrentLinkState = MACIsLinked();
        if(bCurrentLinkState != bLastLinkState)
        {
            bLastLinkState = bCurrentLinkState;
            if(!bCurrentLinkState)
            {
                AppConfig.MyIPAddr.w[0] = AppConfig.DefaultIPAddr.w[0];
                AppConfig.MyIPAddr.w[1] = AppConfig.DefaultIPAddr.w[1];
                AppConfig.MyMask.w[0] = AppConfig.DefaultMask.w[0];
                AppConfig.MyMask.w[1] = AppConfig.DefaultMask.w[1];
                AppConfig.Flags.bInConfigMode = TRUE;
                DHCPInit(0);
            }
        }

        /* DHCP must be called all the time even after IP configuration is
           discovered.
           DHCP has to account lease expiration time and renew the configuration
           time. */
        DHCPTask();

        if(DHCPIsBound(0))
            AppConfig.Flags.bInConfigMode = FALSE;
    }
    #endif

    #if defined(STACK_USE_UDP)
    UDPTask();
    #endif

    /* Process as many incomming packets as we can */
    while(1)
    {
        /* We are about to fetch a new packet, make sure that the
           UDP module knows that any old RX data it has laying
           around will now be gone. */
        #if defined(STACK_USE_UDP)
            UDPDiscard();
        #endif

        /* Fetch a packet (throws old one away, if not thrown away yet) */
        if(!MACGetHeader(&net.remoteNode.MACAddr, &cFrameType))
            break;

        switch(cFrameType)
        {
            case MAC_ARP:
                ARPProcess();
                break;
            case MAC_IP:
                if(!IPGetHeader(&tempLocalIP, &net.remoteNode, &cIPFrameType, &dataCount))
                    break;

                #if defined(STACK_USE_ICMP_SERVER) || defined(STACK_USE_ICMP_CLIENT)
                if(cIPFrameType == IP_PROT_ICMP)
                {
                    #if defined(STACK_USE_IP_GLEANING)
                    if(AppConfig.Flags.bInConfigMode && AppConfig.Flags.bIsDHCPEnabled)
                    {
                        // Accoriding to "IP Gleaning" procedure,
                        // when we receive an ICMP packet with a valid
                        // IP address while we are still in configuration
                        // mode, accept that address as ours and conclude
                        // configuration mode.
                        if(tempLocalIP.Val != 0xffffffff)
                        {
                            AppConfig.Flags.bInConfigMode = FALSE;
                            AppConfig.MyIPAddr = tempLocalIP;
                        }
                    }
                    #endif

                    /* Process this ICMP packet if it the destination IP address matches our address or one of the broadcast IP addressees */
                    if( (GET_DWORD_VAL(tempLocalIP) == GET_DWORD_VAL(AppConfig.MyIPAddr)) ||
                        (GET_DWORD_VAL(tempLocalIP) == 0xFFFFFFFF) ||
                        (GET_DWORD_VAL(tempLocalIP) == ((GET_DWORD_VAL(AppConfig.MyIPAddr) & GET_DWORD_VAL(AppConfig.MyMask)) | ~GET_DWORD_VAL(AppConfig.MyMask))))
                    {
                        ICMPProcess(&net.remoteNode, dataCount);
                    }

                    break;
                }
                #endif

                #if defined(STACK_USE_TCP)
                if(cIPFrameType == IP_PROT_TCP)
                {
                    TCPProcess(&net.remoteNode, &tempLocalIP, dataCount);
                    break;
                }
                #endif

                #if defined(STACK_USE_UDP)
                if(cIPFrameType == IP_PROT_UDP)
                {
                    /* Stop processing packets if we came upon a UDP frame with application data in it */
                    if(UDPProcess(&net.remoteNode, &tempLocalIP, dataCount))
                        return;
                }
                #endif

                break;

            default:
                break;
        }
    }
}

static void stack_app(void)
{
    #if defined(STACK_USE_HTTP_SERVER) || defined(STACK_USE_HTTP2_SERVER)
    HTTPServer();
    #endif

    #if defined(STACK_USE_FTP_SERVER) && defined(STACK_USE_MPFS)
    FTPServer();
    #endif

    #if defined(STACK_USE_DHCP_SERVER)
    DHCPServerTask();
    #endif

    /* User command protocol */
    BSLTask();
}
