/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          eth.c                                   */
/*   Description:   Ethernethandling                        */
/*   Autor:         Johannes Natter                         */
/*   Date:          29.10.2007                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "cstdlib.h"
#include "spi.h"

#include "TCPIP.h"

/* enc instruction set */
#define ENC_OP_RD_CTRL_REG      0x00
#define ENC_OP_WR_CTRL_REG      0x40
#define ENC_OP_RD_BUFFER_MEM    0x3A
#define ENC_OP_WR_BUFFER_MEM    0x7A
#define ENC_OP_ST_CTRL_REG      0x80
#define ENC_OP_CR_CTRL_REG      0xA0
#define ENC_OP_RESET            0xFF

/* enc control register addresses */
/* all banks */
#define ENC_REG_ADR_EIR         0x1C
#define ENC_REG_ADR_ESTAT       0x1D
#define ENC_REG_ADR_ECON2       0x1E
#define ENC_REG_ADR_ECON1       0x1F
/* bank 0 */
#define ENC_REG_ADR_ERDPTL      0x00
#define ENC_REG_ADR_ERDPTH      0x01
#define ENC_REG_ADR_EWRPTL      0x02
#define ENC_REG_ADR_EWRPTH      0x03
#define ENC_REG_ADR_ETXSTL      0x04
#define ENC_REG_ADR_ETXSTH      0x05
#define ENC_REG_ADR_ETXNDL      0x06
#define ENC_REG_ADR_ETXNDH      0x07
#define ENC_REG_ADR_ERXSTL      0x08        /* receive buffer startaddress low byte */
#define ENC_REG_ADR_ERXSTH      0x09        /* receive buffer startaddress high byte */
#define ENC_REG_ADR_ERXNDL      0x0A        /* receive buffer endaddress low byte */
#define ENC_REG_ADR_ERXNDH      0x0B        /* receive buffer endaddress high byte */
#define ENC_REG_ADR_ERXRDPTL    0x0C        /* receive buffer startaddress where overwrite is forbidden low byte */
#define ENC_REG_ADR_ERXRDPTH    0x0D        /* receive buffer startaddress where overwrite is forbidden high byte */
#define ENC_REG_ADR_ERXWRPTL    0x0E        /* receive buffer startaddress of current eth-packet low byte */
#define ENC_REG_ADR_ERXWRPTH    0x0F        /* receive buffer startaddress of current eth-packet high byte */
#define ENC_REG_ADR_EDMASTL     0x10
#define ENC_REG_ADR_EDMASTH     0x11
#define ENC_REG_ADR_EDMANDL     0x12
#define ENC_REG_ADR_EDMANDH     0x13
#define ENC_REG_ADR_EDMADSTL    0x14
#define ENC_REG_ADR_EDMADSTH    0x15
#define ENC_REG_ADR_EDMACSL     0x16
#define ENC_REG_ADR_EDMACSH     0x17
/* bank 1 */
#define ENC_REG_ADR_ERXFCON     0x38
#define ENC_REG_ADR_EPKTCNT     0x39
/* bank 2 */
#define ENC_REG_ADR_M_LOW       0x3F
#define ENC_REG_ADR_MACON1      0x40
#define ENC_REG_ADR_MACON3      0x42
#define ENC_REG_ADR_MACON4      0x43
#define ENC_REG_ADR_MABBIPG     0x44
#define ENC_REG_ADR_MAIPGL      0x46
#define ENC_REG_ADR_MAIPGH      0x47
#define ENC_REG_ADR_MACLCON1    0x48
#define ENC_REG_ADR_MACLCON2    0x49
#define ENC_REG_ADR_MAMXFLL     0x4A
#define ENC_REG_ADR_MAMXFLH     0x4B
#define ENC_REG_ADR_MICMD       0x52
#define ENC_REG_ADR_MIREGADR    0x54
#define ENC_REG_ADR_MIWRL       0x56
#define ENC_REG_ADR_MIWRH       0x57
#define ENC_REG_ADR_MIRDL       0x58
#define ENC_REG_ADR_MIRDH       0x59
/* bank 3 */
#define ENC_REG_ADR_MAADR5      0x60
#define ENC_REG_ADR_MAADR6      0x61
#define ENC_REG_ADR_MAADR3      0x62
#define ENC_REG_ADR_MAADR4      0x63
#define ENC_REG_ADR_MAADR1      0x64
#define ENC_REG_ADR_MAADR2      0x65
#define ENC_REG_ADR_EBSTSD      0x66
#define ENC_REG_ADR_M_HIGH      ENC_REG_ADR_EBSTSD
#define ENC_REG_ADR_EBSTCON     0x67
#define ENC_REG_ADR_EBSTCSL     0x68
#define ENC_REG_ADR_EBSTCSH     0x69
#define ENC_REG_ADR_MISTAT      0x6A
#define ENC_REG_ADR_EREVID      0x72
#define ENC_REG_ADR_ECOCON      0x75

/* enc register bits */
#define ENC_EIR_RXERIF          0x01
#define ENC_EIR_TXERIF          0x02
#define ENC_EIR_TXIF            0x08
#define ENC_EIR_PKTIF           0x40
#define ENC_ESTAT_CLKRDY        0x01
#define ENC_ESTAT_TXABRT        0x02
#define ENC_ECON2_PKTDEC        0x40
#define ENC_ECON2_AUTOINC       0x80
#define ENC_ECON1_RXEN          0x04
#define ENC_ECON1_TXRTS         0x08
#define ENC_ECON1_CSUMEN        0x10
#define ENC_ECON1_DMAST         0x20
#define ENC_ECON1_TXRST         0x80
#define ENC_MICMD_MIIRD         0x01
#define ENC_EBSTCON_BIST_START  0x01
#define ENC_EBSTCON_TEST_EN     0x02
#define ENC_EBSTCON_ADR_FILL    0x04
#define ENC_MISTAT_BUSY         0x01

/* enc phy register BYTE addresses */
#define PHY_REG_ADR_PHCON1      0x00
#define PHY_REG_ADR_PHSTAT1     0x02
#define PHY_REG_ADR_PHID1       0x04
#define PHY_REG_ADR_PHID2       0x06
#define PHY_REG_ADR_PHCON2      0x20
#define PHY_REG_ADR_PHSTAT2     0x22
#define PHY_REG_ADR_PHIE        0x24
#define PHY_REG_ADR_PHIR        0x26
#define PHY_REG_ADR_PHLCON      0x28

/* enc phy register bits */
#define PHY_PHSTAT1L_LLSTAT     0x04

/* bank selection */
#define BANK_SEL_ADR_MASK       0x1F
#define BANK_SEL_REG_BANKED     0x1A
#define BANK_SEL_BANK_NR_SHIFT  0x05
#define BANK_SEL_BANK_MASK      0x03

/* MAC settings */
#define MACON1_MARXEN           0x01
#define MACON1_PASSALL          0x02
#define MACON1_RXPAUS           0x04
#define MACON1_TXPAUS           0x08

#define MACON3_FULDPX           0x01
#define MACON3_FRMLNEN          0x02
#define MACON3_HFRMEN           0x04
#define MACON3_PHDREN           0x08
#define MACON3_TXCRCEN          0x10
#define MACON3_PADCFG0          0x20
#define MACON3_PADCFG1          0x40
#define MACON3_PADCFG2          0x80

#define MACON4_NOBKOFF          0x10
#define MACON4_BPEN             0x20
#define MACON4_DEFER            0x40

#define ERXFCON_UCEN            0x80
#define ERXFCON_AND             0x40 /* packets will be rejected unless all enabled filters accept the packet */
#define ERXFCON_CRCEN           0x20
#define ERXFCON_PMEN            0x10
#define ERXFCON_MPEN            0x08
#define ERXFCON_HTEN            0x04
#define ERXFCON_MCEN            0x02
#define ERXFCON_BCEN            0x01

#define ENC_BIST_CHKSUM_OK      0xF807

#define ENC_RCV_FILTER          (ERXFCON_AND | ERXFCON_CRCEN)

#define ENC_RECOMMENDED_MABBIPG 0x12

#define MAC_MAX_FRM_LEN         1522

#define ENC_RECOMMENDED_MAIPGL  0x12

#define ENC_RECOMMENDED_MAIPGH  0x0C

MAC_ADDR mac_address_enc        = {{0x00, 0x1A, 0x6B, 0x35, 0x77, 0x00}};
MAC_ADDR mac_address_bc         = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

/* PHY settings */
#define PHCON1_PDPXMD           0x0100

#define PHCON2_JABBER_CORR_DIS  0x0400
#define PHCON2_HDLDIS           0x0100

/* LED Settings */
#define LCFG_TMIT_ACT           0x01
#define LCFG_RCV_ACT            0x02
#define LCFG_COLL_ACT           0x03
#define LCFG_LINK_STAT          0x04
#define LCFG_DPX_STAT           0x05
#define LCFG_TMIT_RCV_ACT       0x07
#define LCFG_ON                 0x08
#define LCFG_OFF                0x09
#define LCFG_BLINK_FAST         0x0A
#define LCFG_BLINK_SLOW         0x0B
#define LCFG_LINK_RCV_ACT       0x0C
#define LCFG_LINK_TMIT_RCV      0x0D
#define LCFG_DPX_COLL           0x0E

#define LFRQ_040MS              0x00
#define LFRQ_070MS              0x04
#define LFRQ_140MS              0x08
#define LFRQ_ENABLE             0x02

#define PHLCON_RESERVED         0x3000
#define LEDACFG                 (((UINT2)LCFG_LINK_RCV_ACT)<<8)  /* green */
#define LEDBCFG                 (LCFG_TMIT_ACT<<4)  /* orange */
#define PULSE_STRETCH           LFRQ_040MS | LFRQ_ENABLE

/* Transmit status vector defines */
/* Byte 0 */
/* Byte 1 */
/* Byte 2 */
/* Byte 3 */
#define ENC_TMIT_B3_LATE_COLL   0x20
/* Byte 4 */
/* Byte 5 */
/* Byte 6 */

/* Receive status defines */
/* Byte 2 */
#define ENC_RCV_B2_CRCERR       0x10
#define ENC_RCV_B2_RCVOK        0x80
/* Byte 3 */
#define ENC_RCV_B3_ZERO         0x80

typedef struct
{
    UINT2               p_next_packet;
    UINT2               rcv_byte_cnt;
    UINT1               rcv_stat_b2;
    UINT1               rcv_stat_b3;
} RCV_STATUS_TYPE;

typedef struct
{
    UINT1               b0;
    UINT1               b1;
    UINT1               b2;
    UINT1               b3;
    UINT1               b4;
    UINT1               b5;
    UINT1               b6;
} TMIT_STATUS_TYPE;

typedef struct
{
    MAC_ADDR            dst_mac_addr;
    MAC_ADDR            src_mac_addr;
    UINT1               type_lenh;
    UINT1               type_lenl;
} ETH_HEADER_TYPE;

typedef struct
{
    RCV_STATUS_TYPE     rcv_status;
    /* TMIT_STATUS_TYPE    tmit_status; */
    UINT2               p_cur_packet;
    UINT1               cur_reg_bank;
    BOOL                pkt_discarded;
} ENC28_TYPE;

ENC28_TYPE              enc;

/* Microchip: Pseudo Functions and variables */
/* ---------------------------------------------------------- */
#define LOW(a)                  ((a) & 0xFF)
#define HIGH(a)                 (((a)>>8) & 0xFF)
#define ETHER_IP    (0x00u)
#define ETHER_ARP   (0x06u)
/* ---------------------------------------------------------- */

/* ******************** LOCAL FUNCTIONS ******************* */
/* static  void            enc_init_bist           (void); */
static  void            enc_init_buffer_mem     (void);
static  void            enc_init_mac            (void);
static  void            enc_init_phy            (void);
static  void            enc_reset               (void);
static  void            enc_reg_write           (UINT1 reg_adr, UINT1 data);
static  UINT1           enc_reg_read            (UINT1 reg_adr);
static  void            enc_reg_bit_set         (UINT1 reg_adr, UINT1 bit_mask);
static  void            enc_reg_bit_clear       (UINT1 reg_adr, UINT1 bit_mask);
static  void            enc_reg_phy_write       (UINT1 reg_adr, UINT2 data);
static  UINT1           enc_reg_phy_read        (UINT1 reg_adr);
static  void            enc_select_bank         (UINT1 reg_adr);
static  UINT1           check_mac_mii_reg       (UINT1 reg_adr);
/* ******************************************************** */

/******************************************************************************
 * Function:        void MACInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        MACInit sets up the PIC's SPI module and all the
 *                  registers in the ENC28J60 so that normal operation can
 *                  begin.
 *
 * Note:            None
 *****************************************************************************/
void eth_init(void)
{
    enc_reset();

    enc_reg_bit_clear(ENC_REG_ADR_ECON1, BANK_SEL_BANK_MASK);
    enc.cur_reg_bank = 0;

    /* enc_init_bist(); */

    /* Start up in Bank 0 and configure the receive buffer boundary pointers
       and the buffer write protect pointer (receive buffer read pointer) */
    enc_init_buffer_mem();

    enc_init_mac();

    enc_init_phy();

    /* Disable the CLKOUT output to reduce EMI generation */
    enc_reg_write(ENC_REG_ADR_ECOCON, 0x00);   /* Output off (0V) */

    /* Get the Rev ID so that we can implement the correct errata workarounds */
    /* enc.revision_id = enc_reg_read(ENC_REG_ADR_EREVID); */

    /* Enable packet reception */
    enc_reg_bit_set(ENC_REG_ADR_ECON1, ENC_ECON1_RXEN);
}

/* **************************************************** */
/* Send diagnose packet                                 */
/* **************************************************** */
void MACSendDiag(UINT1 *p_data, BLOCK_LEN_TYPE size)
{
    MACPutHeader(&mac_address_bc, 0, size);
    MACPutArray(p_data, size);
    MACFlush();
}

/******************************************************************************
 * Function:        void MACPutHeader(MAC_ADDR *remote, BYTE type, WORD dataLen)
 *
 * PreCondition:    MACIsTxReady() must return TRUE.
 *
 * Input:           *remote: Pointer to memory which contains the destination
 *                           MAC address (6 bytes)
 *                  type: The constant ETHER_ARP or ETHER_IP, defining which
 *                        value to write into the Ethernet header's type field.
 *                  dataLen: Length of the Ethernet data payload
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            Because of the dataLen parameter, it is probably
 *                  advantagous to call this function immediately before
 *                  transmitting a packet rather than initially when the
 *                  packet is first created.  The order in which the packet
 *                  is constructed (header first or data first) is not
 *                  important.
 *****************************************************************************/
void MACPutHeader(MAC_ADDR *remote, BYTE type, WORD dataLen)
{
    // Set the SPI write pointer to the beginning of the transmit buffer (post per packet control byte)
    enc_reg_write(ENC_REG_ADR_EWRPTL, LOW(TXSTART+1)); /* +1 = PKT_CTRL_BYTE */
    enc_reg_write(ENC_REG_ADR_EWRPTH, HIGH(TXSTART+1));

    // Set the per-packet control byte and write the Ethernet destination
    // address
    MACPutArray((BYTE*)remote, sizeof(*remote));

    // Write our MAC address in the Ethernet source field
    MACPutArray((BYTE*)&AppConfig.MyMACAddr, sizeof(AppConfig.MyMACAddr));

    // Write the appropriate Ethernet Type WORD for the protocol being used
    if(type == MAC_IP)
    {
        MACPut(0x08); MACPut(ETHER_IP);
    }else
    if(type == MAC_ARP)
    {
        MACPut(0x08); MACPut(ETHER_ARP);
    }else{
        MACPut(HIGH(dataLen)); MACPut(LOW(dataLen));
    }

    // Calculate where to put the TXND pointer
    dataLen += (WORD)sizeof(ETH_HEADER_TYPE) + TXSTART;

    // Write the TXND pointer into the registers, given the dataLen given
    enc_reg_write(ENC_REG_ADR_ETXNDL, ((WORD_VAL*)&dataLen)->v[0]);
    enc_reg_write(ENC_REG_ADR_ETXNDH, ((WORD_VAL*)&dataLen)->v[1]);
}

/******************************************************************************
 * Function:        BOOL MACGetHeader(MAC_ADDR *remote, BYTE* type)
 *
 * PreCondition:    None
 *
 * Input:           *remote: Location to store the Source MAC address of the
 *                           received frame.
 *                  *type: Location of a BYTE to store the constant
 *                         MAC_UNKNOWN, ETHER_IP, or ETHER_ARP, representing
 *                         the contents of the Ethernet type field.
 *
 * Output:          TRUE: If a packet was waiting in the RX buffer.  The
 *                        remote, and type values are updated.
 *                  FALSE: If a packet was not pending.  remote and type are
 *                         not changed.
 *
 * Side Effects:    Last packet is discarded if MACDiscardRx() hasn't already
 *                  been called.
 *
 * Overview:        None
 *
 * Note:            None
 *****************************************************************************/
BOOL MACGetHeader(MAC_ADDR *remote, BYTE* type)
{
    RCV_STATUS_TYPE *p_rcv = &enc.rcv_status;
    ETH_HEADER_TYPE header;

    /* Make absolutely certain that any previous packet was discarded */
    if(FALSE == enc.pkt_discarded)
    {
        MACDiscardRx();
        return FALSE;
    }

    /* Test if at least one packet has been received and is waiting */
    if(0 == enc_reg_read(ENC_REG_ADR_EPKTCNT))
        return FALSE;

    /* Set the SPI read pointer to the beginning of the next unprocessed packet */
    enc.p_cur_packet = enc.rcv_status.p_next_packet;
    enc_reg_write(ENC_REG_ADR_ERDPTL, LOW(enc.p_cur_packet));
    enc_reg_write(ENC_REG_ADR_ERDPTH, HIGH(enc.p_cur_packet));

    /* Obtain receive status vector */
    MACGetArray((BYTE *)p_rcv, sizeof(*p_rcv));

    /* Obtain the MAC header from the Ethernet buffer */
    MACGetArray((BYTE *)&header, sizeof(header));

    /* Validate the data returned from the ENC28J60. Random data corruption,
       such as if a single SPI bit error occurs while communicating or a
       momentary power glitch could cause this to occur in rare circumstances. */
    if((p_rcv->p_next_packet > RXSTOP) ||
       (p_rcv->p_next_packet & 1) ||
       (p_rcv->rcv_stat_b3 & ENC_RCV_B3_ZERO) ||
       (p_rcv->rcv_stat_b2 & ENC_RCV_B2_CRCERR) ||
       (p_rcv->rcv_byte_cnt > 1518u) ||
      !(p_rcv->rcv_stat_b2 & ENC_RCV_B2_RCVOK))
    {
        p_rcv->p_next_packet = enc.p_cur_packet;
        return FALSE;
    }

    /* Return the Ethernet frame's Source MAC address field to the caller
       This parameter is useful for replying to requests without requiring an
       ARP cycle. */
    mcpy(remote->v, header.src_mac_addr.v, sizeof(*remote));

    /* Return a simplified version of the EtherType field to the caller */
    *type = MAC_UNKNOWN;
    if( (header.type_lenh == 0x08u) &&
        ((header.type_lenl == ETHER_IP) || (header.type_lenl == ETHER_ARP)) )
    {
        *type = header.type_lenl;
    }

    /* Mark this packet as discardable */
    enc.pkt_discarded = FALSE;

    return TRUE;
}

/* **************************************************** */
/* Write data to the tmit-buffer                        */
/* **************************************************** */
void MACPutArray(UINT1 *p_buffer, MCP_LEN_TYPE size)
{
    spi_wr_wr_n(ENC_OP_WR_BUFFER_MEM, SPI_CS_MAIN_ENC, size, (UINT1 *)p_buffer);
}

/* **************************************************** */
/* Get data from the rcv-buffer                         */
/* **************************************************** */
MCP_LEN_TYPE MACGetArray(UINT1 *p_buffer, MCP_LEN_TYPE size)
{
    spi_wr_rd_n(ENC_OP_RD_BUFFER_MEM, SPI_CS_MAIN_ENC, size, (UINT1 *)p_buffer);

    return size;
}

/******************************************************************************
 * Function:        void MACPut(BYTE val)
 *
 * PreCondition:    SPI bus must be initialized (done in MACInit()).
 *                  EWRPT must point to the location to begin writing.
 *
 * Input:           Byte to write into the ENC28J60 buffer memory
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        MACPut outputs the Write Buffer Memory opcode/constant
 *                  (8 bits) and data to write (8 bits) over the SPI.
 *                  EWRPT is incremented after the write.
 *
 * Note:            None
 *****************************************************************************/
void MACPut(BYTE val)
{
    spi_wr_wr_n(ENC_OP_WR_BUFFER_MEM, SPI_CS_MAIN_ENC, 1, &val);
}

/******************************************************************************
 * Function:        BYTE MACGet()
 *
 * PreCondition:    SPI bus must be initialized (done in MACInit()).
 *                  ERDPT must point to the place to read from.
 *
 * Input:           None
 *
 * Output:          Byte read from the ENC28J60's RAM
 *
 * Side Effects:    None
 *
 * Overview:        MACGet returns the byte pointed to by ERDPT and
 *                  increments ERDPT so MACGet() can be called again.  The
 *                  increment will follow the receive buffer wrapping boundary.
 *
 * Note:            None
 *****************************************************************************/
BYTE MACGet(void)
{
    BYTE result;
    spi_wr_rd_n(ENC_OP_RD_BUFFER_MEM, SPI_CS_MAIN_ENC, 1, &result);
    return result;
}

/******************************************************************************
 * Function:        void MACFlush(void)
 *
 * PreCondition:    A packet has been created by calling MACPut() and
 *                  MACPutHeader().
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        MACFlush causes the current TX packet to be sent out on
 *                  the Ethernet medium.  The hardware MAC will take control
 *                  and handle CRC generation, collision retransmission and
 *                  other details.
 *
 * Note:            After transmission completes (MACIsTxReady() returns TRUE),
 *                  the packet can be modified and transmitted again by calling
 *                  MACFlush() again.  Until MACPutHeader() or MACPut() is
 *                  called (in the TX data area), the data in the TX buffer
 *                  will not be corrupted.
 *****************************************************************************/
void MACFlush(void)
{
    // Reset transmit logic if a TX Error has previously occured
    // This is a silicon errata workaround
    enc_reg_bit_set(ENC_REG_ADR_ECON1, ENC_ECON1_TXRST);
    enc_reg_bit_clear(ENC_REG_ADR_ECON1, ENC_ECON1_TXRST);
    enc_reg_bit_clear(ENC_REG_ADR_EIR, ENC_EIR_TXERIF | ENC_EIR_TXIF);

    // Start the transmission
    // After transmission completes (MACIsTxReady() returns TRUE), the packet
    // can be modified and transmitted again by calling MACFlush() again.
    // Until MACPutHeader() is called, the data in the TX buffer will not be
    // corrupted.
    enc_reg_bit_set(ENC_REG_ADR_ECON1, ENC_ECON1_TXRTS);

#if 0
    // Revision B5 and B7 silicon errata workaround
    //if(enc.revision_id == 0x05u || enc.revision_id == 0x06u)
    {
        WORD AttemptCounter = 0x0000;
        while(!(enc_reg_read(ENC_REG_ADR_EIR) & (ENC_EIR_TXERIF | ENC_EIR_TXIF)) && (++AttemptCounter < 1000u));
        if((enc_reg_read(ENC_REG_ADR_EIR) & ENC_EIR_TXERIF) || (AttemptCounter >= 1000u))
        {
            WORD_VAL ReadPtrSave;
            WORD_VAL TXEnd;
            TMIT_STATUS_TYPE TXStatus;
            BYTE i;

            // Cancel the previous transmission if it has become stuck set
            enc_reg_bit_clear(ENC_REG_ADR_ECON1, ENC_ECON1_TXRTS);

            // Save the current read pointer (controlled by application)
            ReadPtrSave.v[0] = enc_reg_read(ENC_REG_ADR_ERDPTL);
            ReadPtrSave.v[1] = enc_reg_read(ENC_REG_ADR_ERDPTH);

            // Get the location of the transmit status vector
            TXEnd.v[0] = enc_reg_read(ENC_REG_ADR_ETXNDL);
            TXEnd.v[1] = enc_reg_read(ENC_REG_ADR_ETXNDH);
            TXEnd.Val++;

            // Read the transmit status vector
            enc_reg_write(ENC_REG_ADR_ERDPTL, TXEnd.v[0]);
            enc_reg_write(ENC_REG_ADR_ERDPTH, TXEnd.v[1]);
            MACGetArray((BYTE*)&TXStatus, sizeof(TXStatus));

            // Implement retransmission if a late collision occured (this can
            // happen on B5 when certain link pulses arrive at the same time
            // as the transmission)
            for(i = 0; i < 16u; i++)
            {
                if((enc_reg_read(ENC_REG_ADR_EIR) & ENC_EIR_TXERIF) && (TXStatus.b3 & ENC_TMIT_B3_LATE_COLL))
                {
                    // Reset the TX logic
                    enc_reg_bit_set(ENC_REG_ADR_ECON1, ENC_ECON1_TXRST);
                    enc_reg_bit_clear(ENC_REG_ADR_ECON1, ENC_ECON1_TXRST);
                    enc_reg_bit_clear(ENC_REG_ADR_EIR, ENC_EIR_TXERIF | ENC_EIR_TXIF);

                    // Transmit the packet again
                    enc_reg_bit_set(ENC_REG_ADR_ECON1, ENC_ECON1_TXRTS);
                    while(!(enc_reg_read(ENC_REG_ADR_EIR) & (ENC_EIR_TXERIF | ENC_EIR_TXIF)));

                    // Cancel the previous transmission if it has become stuck set
                    enc_reg_bit_clear(ENC_REG_ADR_ECON1, ENC_ECON1_TXRTS);

                    // Read transmit status vector
                    enc_reg_write(ENC_REG_ADR_ERDPTL, TXEnd.v[0]);
                    enc_reg_write(ENC_REG_ADR_ERDPTH, TXEnd.v[1]);
                    MACGetArray((BYTE*)&TXStatus, sizeof(TXStatus));
                }
                else
                {
                    break;
                }
            }

            // Restore the current read pointer
            enc_reg_write(ENC_REG_ADR_ERDPTL, ReadPtrSave.v[0]);
            enc_reg_write(ENC_REG_ADR_ERDPTH, ReadPtrSave.v[1]);
        }
    }
#endif
}

/******************************************************************************
 * Function:        WORD MACSetWritePtr(WORD Address)
 *
 * PreCondition:    None
 *
 * Input:           Address: Address to seek to
 *
 * Output:          WORD: Old EWRPT location
 *
 * Side Effects:    None
 *
 * Overview:        SPI write pointer is updated.  All calls to
 *                  MACPut() and MACPutArray() will use this new value.
 *
 * Note:            None
 *****************************************************************************/
UINT2 MACSetWritePtr(UINT2 addr)
{
    WORD_VAL oldVal;

    oldVal.v[0] = enc_reg_read(ENC_REG_ADR_EWRPTL);
    oldVal.v[1] = enc_reg_read(ENC_REG_ADR_EWRPTH);

    // Set the SPI write pointer to the new calculated value
    enc_reg_write(ENC_REG_ADR_EWRPTL, ((WORD_VAL*)&addr)->v[0]);
    enc_reg_write(ENC_REG_ADR_EWRPTH, ((WORD_VAL*)&addr)->v[1]);

    return oldVal.Val;
}

/******************************************************************************
 * Function:        BOOL MACIsTxReady(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          TRUE: If no Ethernet transmission is in progress
 *                  FALSE: If a previous transmission was started, and it has
 *                         not completed yet.  While FALSE, the data in the
 *                         transmit buffer and the TXST/TXND pointers must not
 *                         be changed.
 *
 * Side Effects:    None
 *
 * Overview:        Returns the ECON1.TXRTS bit
 *
 * Note:            None
 *****************************************************************************/
BOOL MACIsTxReady(void)
{
    return ~enc_reg_read(ENC_REG_ADR_ECON1) & ENC_ECON1_TXRTS;
}

/******************************************************************************
 * Function:        void MACMemCopyAsync(WORD destAddr, WORD sourceAddr, WORD len)
 *
 * PreCondition:    SPI bus must be initialized (done in MACInit()).
 *
 * Input:           destAddr:   Destination address in the Ethernet memory to
 *                              copy to.  If the MSb is set, the current EWRPT
 *                              value will be used instead.
 *                  sourceAddr: Source address to read from.  If the MSb is
 *                              set, the current ERDPT value will be used
 *                              instead.
 *                  len:        Number of bytes to copy
 *
 * Output:          Byte read from the ENC28J60's RAM
 *
 * Side Effects:    None
 *
 * Overview:        Bytes are asynchrnously transfered within the buffer.  Call
 *                  MACIsMemCopyDone() to see when the transfer is complete.
 *
 * Note:            If a prior transfer is already in progress prior to
 *                  calling this function, this function will block until it
 *                  can start this transfer.
 *
 *                  If a negative value is used for the sourceAddr or destAddr
 *                  parameters, then that pointer will get updated with the
 *                  next address after the read or write.
 *****************************************************************************/
void MACMemCopyAsync(WORD destAddr, WORD sourceAddr, WORD len)
{
    WORD_VAL ReadSave, WriteSave;
    BOOL UpdateWritePointer = FALSE;
    BOOL UpdateReadPointer = FALSE;

    if(((WORD_VAL*)&destAddr)->bits.b15)
    {
        UpdateWritePointer = TRUE;
        ((WORD_VAL*)&destAddr)->v[0] = enc_reg_read(ENC_REG_ADR_EWRPTL);
        ((WORD_VAL*)&destAddr)->v[1] = enc_reg_read(ENC_REG_ADR_EWRPTH);
    }
    if(((WORD_VAL*)&sourceAddr)->bits.b15)
    {
        UpdateReadPointer = TRUE;
        ((WORD_VAL*)&sourceAddr)->v[0] = enc_reg_read(ENC_REG_ADR_ERDPTL);
        ((WORD_VAL*)&sourceAddr)->v[1] = enc_reg_read(ENC_REG_ADR_ERDPTH);
    }

    // Handle special conditions where len == 0 or len == 1
    // The DMA module is not capable of handling those corner cases
    if(len <= 1u)
    {
        if(!UpdateReadPointer)
        {
            ReadSave.v[0] = enc_reg_read(ENC_REG_ADR_ERDPTL);
            ReadSave.v[1] = enc_reg_read(ENC_REG_ADR_ERDPTH);
        }
        if(!UpdateWritePointer)
        {
            WriteSave.v[0] = enc_reg_read(ENC_REG_ADR_EWRPTL);
            WriteSave.v[1] = enc_reg_read(ENC_REG_ADR_EWRPTH);
        }
        enc_reg_write(ENC_REG_ADR_ERDPTL, ((WORD_VAL*)&sourceAddr)->v[0]);
        enc_reg_write(ENC_REG_ADR_ERDPTH, ((WORD_VAL*)&sourceAddr)->v[1]);
        enc_reg_write(ENC_REG_ADR_EWRPTL, ((WORD_VAL*)&destAddr)->v[0]);
        enc_reg_write(ENC_REG_ADR_EWRPTH, ((WORD_VAL*)&destAddr)->v[1]);
        while(len--)
            MACPut(MACGet());
        if(!UpdateReadPointer)
        {
            enc_reg_write(ENC_REG_ADR_ERDPTL, ReadSave.v[0]);
            enc_reg_write(ENC_REG_ADR_ERDPTH, ReadSave.v[1]);
        }
        if(!UpdateWritePointer)
        {
            enc_reg_write(ENC_REG_ADR_EWRPTL, WriteSave.v[0]);
            enc_reg_write(ENC_REG_ADR_EWRPTH, WriteSave.v[1]);
        }
    }
    else
    {
        if(UpdateWritePointer)
        {
            WriteSave.Val = destAddr + len;
            enc_reg_write(ENC_REG_ADR_EWRPTL, WriteSave.v[0]);
            enc_reg_write(ENC_REG_ADR_EWRPTH, WriteSave.v[1]);
        }
        len += sourceAddr - 1;
        while(enc_reg_read(ENC_REG_ADR_ECON1) & ENC_ECON1_DMAST);
        enc_reg_write(ENC_REG_ADR_EDMASTL, ((WORD_VAL*)&sourceAddr)->v[0]);
        enc_reg_write(ENC_REG_ADR_EDMASTH, ((WORD_VAL*)&sourceAddr)->v[1]);
        enc_reg_write(ENC_REG_ADR_EDMADSTL, ((WORD_VAL*)&destAddr)->v[0]);
        enc_reg_write(ENC_REG_ADR_EDMADSTH, ((WORD_VAL*)&destAddr)->v[1]);
        if((sourceAddr <= RXSTOP) && (len > RXSTOP)) //&& (sourceAddr >= RXSTART))
            len -= RXSIZE;
        enc_reg_write(ENC_REG_ADR_EDMANDL, ((WORD_VAL*)&len)->v[0]);
        enc_reg_write(ENC_REG_ADR_EDMANDH, ((WORD_VAL*)&len)->v[1]);
        enc_reg_bit_clear(ENC_REG_ADR_ECON1, ENC_ECON1_CSUMEN);
        enc_reg_bit_set(ENC_REG_ADR_ECON1, ENC_ECON1_DMAST);
        if(UpdateReadPointer)
        {
            len++;
            if((sourceAddr <= RXSTOP) && (len > RXSTOP)) //&& (sourceAddr >= RXSTART))
                len -= RXSIZE;
            enc_reg_write(ENC_REG_ADR_ERDPTL, ((WORD_VAL*)&len)->v[0]);
            enc_reg_write(ENC_REG_ADR_ERDPTH, ((WORD_VAL*)&len)->v[1]);
        }
    }
}

BOOL MACIsMemCopyDone(void)
{
    return !(enc_reg_read(ENC_REG_ADR_ECON1) & ENC_ECON1_DMAST);
}

/******************************************************************************
 * Function:        WORD MACCalcRxChecksum(WORD offset, WORD len)
 *
 * PreCondition:    None
 *
 * Input:           offset  - Number of bytes beyond the beginning of the
 *                          Ethernet data (first byte after the type field)
 *                          where the checksum should begin
 *                  len     - Total number of bytes to include in the checksum
 *
 * Output:          16-bit checksum as defined by RFC 793.
 *
 * Side Effects:    None
 *
 * Overview:        This function performs a checksum calculation in the MAC
 *                  buffer itself
 *
 * Note:            None
 *****************************************************************************/
WORD MACCalcRxChecksum(WORD offset, WORD len)
{
    WORD_VAL temp;
    WORD_VAL RDSave;

    // Add the offset requested by firmware plus the Ethernet header
    temp.Val = enc.p_cur_packet + sizeof(RCV_STATUS_TYPE) + sizeof(ETH_HEADER_TYPE) + offset;
    if(temp.Val > RXSTOP)       // Adjust value if a wrap is needed
    {
        temp.Val -= RXSIZE;
    }

    RDSave.v[0] = enc_reg_read(ENC_REG_ADR_ERDPTL);
    RDSave.v[1] = enc_reg_read(ENC_REG_ADR_ERDPTH);

    enc_reg_write(ENC_REG_ADR_ERDPTL, temp.v[0]);
    enc_reg_write(ENC_REG_ADR_ERDPTH, temp.v[1]);

    temp.Val = CalcIPBufferChecksum(len);

    enc_reg_write(ENC_REG_ADR_ERDPTL, RDSave.v[0]);
    enc_reg_write(ENC_REG_ADR_ERDPTH, RDSave.v[1]);

    return temp.Val;
}

/******************************************************************************
 * Function:        WORD CalcIPBufferChecksum(WORD len)
 *
 * PreCondition:    Read buffer pointer set to starting of checksum data
 *
 * Input:           len: Total number of bytes to calculate the checksum over.
 *                       The first byte included in the checksum is the byte
 *                       pointed to by ERDPT, which is updated by calls to
 *                       MACSetReadPtr(), MACGet(), MACGetArray(),
 *                       MACGetHeader(), etc.
 *
 * Output:          16-bit checksum as defined by RFC 793
 *
 * Side Effects:    None
 *
 * Overview:        This function performs a checksum calculation in the MAC
 *                  buffer itself.  The ENC28J60 has a hardware DMA module
 *                  which can calculate the checksum faster than software, so
 *                  this function replaces the CaclIPBufferChecksum() function
 *                  defined in the helpers.c file.  Through the use of
 *                  preprocessor defines, this replacement is automatic.
 *
 * Note:            This function works either in the RX buffer area or the TX
 *                  buffer area.  No validation is done on the len parameter.
 *****************************************************************************/
WORD CalcIPBufferChecksum(WORD len)
{
    WORD_VAL Start;
    DWORD Checksum = 0x00;
    DWORD_VAL *p_Checksum = (DWORD_VAL *)&Checksum;
    WORD ChunkLen;
    WORD DataBuffer[10];
    WORD *DataPtr;

    // Save the SPI read pointer starting address
    Start.v[0] = enc_reg_read(ENC_REG_ADR_ERDPTL);
    Start.v[1] = enc_reg_read(ENC_REG_ADR_ERDPTH);

    while(len)
    {
        // Obtain a chunk of data (less SPI overhead compared
        // to requesting one byte at a time)
        ChunkLen = len > sizeof(DataBuffer) ? sizeof(DataBuffer) : len;
        MACGetArray((BYTE*)DataBuffer, ChunkLen);

        len -= ChunkLen;

        // Take care of a last odd numbered data byte
        if(((WORD_VAL*)&ChunkLen)->bits.b0)
        {
            ((BYTE*)DataBuffer)[ChunkLen] = 0x00;
            ChunkLen++;
        }

        // Calculate the checksum over this chunk
        DataPtr = DataBuffer;
        while(ChunkLen)
        {
            Checksum += *DataPtr++;
            ChunkLen -= 2;
        }
    }

    // Restore old read pointer location
    enc_reg_write(ENC_REG_ADR_ERDPTL, Start.v[0]);
    enc_reg_write(ENC_REG_ADR_ERDPTH, Start.v[1]);

    // Do an end-around carry (one's complement arrithmatic)
    Checksum = (DWORD)p_Checksum->w[0] + (DWORD)p_Checksum->w[1];

    // Do another end-around carry in case if the prior add
    // caused a carry out
    p_Checksum->w[0] += p_Checksum->w[1];

    // Return the resulting checksum
    return ~p_Checksum->w[0];
}

/******************************************************************************
 * Function:        BOOL MACIsLinked(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          TRUE: If the PHY reports that a link partner is present
 *                        and the link has been up continuously since the last
 *                        call to MACIsLinked()
 *                  FALSE: If the PHY reports no link partner, or the link went
 *                         down momentarily since the last call to MACIsLinked()
 *
 * Side Effects:    None
 *
 * Overview:        Returns the PHSTAT1.LLSTAT bit.
 *
 * Note:            None
 *****************************************************************************/
BOOL MACIsLinked(void)
{
    // LLSTAT is a latching low link status bit.  Therefore, if the link
    // goes down and comes back up before a higher level stack program calls
    // MACIsLinked(), MACIsLinked() will still return FALSE.  The next
    // call to MACIsLinked() will return TRUE (unless the link goes down
    // again).
    return enc_reg_phy_read(PHY_REG_ADR_PHSTAT1) & PHY_PHSTAT1L_LLSTAT;
}

/* **************************************************** */
/* Broadcast MAC address allocated in ethernet driver   */
/* **************************************************** */
MAC_ADDR *get_bc_addr(void)
{
    return &mac_address_bc;
}

/******************************************************************************
 * Function:        void MACSetReadPtrInRx(WORD offset)
 *
 * PreCondition:    A packet has been obtained by calling MACGetHeader() and
 *                  getting a TRUE result.
 *
 * Input:           offset: WORD specifying how many bytes beyond the Ethernet
 *                          header's type field to relocate the SPI read
 *                          pointer.
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        SPI read pointer are updated.  All calls to
 *                  MACGet() and MACGetArray() will use these new values.
 *
 * Note:            RXSTOP must be statically defined as being > RXSTART for
 *                  this function to work correctly.  In other words, do not
 *                  define an RX buffer which spans the 0x1FFF->0x0000 memory
 *                  boundary.
 *****************************************************************************/
void MACSetReadPtrInRx(WORD offset)
{
    WORD_VAL ReadPT;

    // Determine the address of the beginning of the entire packet
    // and adjust the address to the desired location
    ReadPT.Val = enc.p_cur_packet + sizeof(RCV_STATUS_TYPE) + sizeof(ETH_HEADER_TYPE) + offset;

    // Since the receive buffer is circular, adjust if a wraparound is needed
    if(ReadPT.Val > RXSTOP)
        ReadPT.Val -= RXSTOP - RXSTART;

    // Set the SPI read pointer to the new calculated value
    enc_reg_write(ENC_REG_ADR_ERDPTL, ReadPT.v[0]);
    enc_reg_write(ENC_REG_ADR_ERDPTH, ReadPT.v[1]);
}

/******************************************************************************
 * Function:        WORD MACSetReadPtr(WORD Address)
 *
 * PreCondition:    None
 *
 * Input:           Address: Address to seek to
 *
 * Output:          WORD: Old ERDPT value
 *
 * Side Effects:    None
 *
 * Overview:        SPI write pointer is updated.  All calls to
 *                  MACPut() and MACPutArray() will use this new value.
 *
 * Note:            None
 *****************************************************************************/
WORD MACSetReadPtr(WORD address)
{
    WORD_VAL oldVal;

    oldVal.v[0] = enc_reg_read(ENC_REG_ADR_ERDPTL);
    oldVal.v[1] = enc_reg_read(ENC_REG_ADR_ERDPTH);

    // Set the SPI write pointer to the new calculated value
    enc_reg_write(ENC_REG_ADR_ERDPTL, ((WORD_VAL*)&address)->v[0]);
    enc_reg_write(ENC_REG_ADR_ERDPTH, ((WORD_VAL*)&address)->v[1]);

    return oldVal.Val;
}

/******************************************************************************
 * Function:        void MACDiscardRx(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Marks the last received packet (obtained using
 *                  MACGetHeader())as being processed and frees the buffer
 *                  memory associated with it
 *
 * Note:            It is safe to call this function multiple times between
 *                  MACGetHeader() calls.  Extra packets won't be thrown away
 *                  until MACGetHeader() makes it available.
 *****************************************************************************/
void MACDiscardRx(void)
{
    WORD_VAL NewRXRDLocation;

    // Make sure the current packet was not already discarded
    if(enc.pkt_discarded == TRUE)
        return;
    enc.pkt_discarded = TRUE;

    // Decrement the next packet pointer before writing it into
    // the ERXRDPT registers.  This is a silicon errata workaround.
    // RX buffer wrapping must be taken into account if the
    // NextPacketLocation is precisely RXSTART.
    NewRXRDLocation.Val = enc.rcv_status.p_next_packet - 1;
    if(NewRXRDLocation.Val > RXSTOP)
        NewRXRDLocation.Val = RXSTOP;

    // Decrement the RX packet counter register, EPKTCNT
    enc_reg_bit_set(ENC_REG_ADR_ECON2, ENC_ECON2_PKTDEC);

    // Move the receive read pointer to unwrite-protect the memory used by the
    // last packet.  The writing order is important: set the low byte first,
    // high byte last.
    enc_reg_write(ENC_REG_ADR_ERXRDPTL, NewRXRDLocation.v[0]);
    enc_reg_write(ENC_REG_ADR_ERXRDPTH, NewRXRDLocation.v[1]);
}

/* **************************************************** */
/* Close Ethernet                                       */
/* **************************************************** */
void eth_close(void) {}

/* **************************************************** */
/* Initialize the built-in self test of the enc         */
/* **************************************************** */
#if 0
static void enc_init_bist(void)
{
    WORD_VAL chk_sum_dma, chk_sum_bist;
    ULONG4 wait_cnt = 0;

    /* Test entire buffer memory */
    enc_reg_write(ENC_REG_ADR_EDMASTL, 0x00);
    enc_reg_write(ENC_REG_ADR_EDMASTH, 0x00);
    enc_reg_write(ENC_REG_ADR_EDMANDL, 0xFF);
    enc_reg_write(ENC_REG_ADR_EDMANDH, 0x1F);

    /* To avoid endless DMA calc. Init. RCV-Buffer */
    enc_reg_write(ENC_REG_ADR_ERXNDL,  0xFF);
    enc_reg_write(ENC_REG_ADR_ERXNDH,  0x1F);

    enc_reg_bit_set(ENC_REG_ADR_ECON1, ENC_ECON1_CSUMEN);

    /* BIST */
    enc_reg_write(ENC_REG_ADR_EBSTCON, ENC_EBSTCON_ADR_FILL | ENC_EBSTCON_TEST_EN);
    enc_reg_bit_set(ENC_REG_ADR_EBSTCON, ENC_EBSTCON_BIST_START);
    /* DMA */
    enc_reg_bit_set(ENC_REG_ADR_ECON1, ENC_ECON1_DMAST);

    while((++wait_cnt < 100000) &&
          (enc_reg_read(ENC_REG_ADR_ECON1) & ENC_ECON1_DMAST));

    enc_reg_bit_clear(ENC_REG_ADR_ECON1, ENC_ECON1_CSUMEN);

    /* Abort */
    enc_reg_bit_clear(ENC_REG_ADR_EBSTCON, ENC_EBSTCON_BIST_START);
    enc_reg_bit_clear(ENC_REG_ADR_ECON1, ENC_ECON1_DMAST);

    chk_sum_dma.v[0]  = enc_reg_read(ENC_REG_ADR_EDMACSL);
    chk_sum_dma.v[1]  = enc_reg_read(ENC_REG_ADR_EDMACSH);
    chk_sum_bist.v[0] = enc_reg_read(ENC_REG_ADR_EBSTCSL);
    chk_sum_bist.v[1] = enc_reg_read(ENC_REG_ADR_EBSTCSH);

    enc_reset();

    /* Check */
    if(ENC_BIST_CHKSUM_OK != chk_sum_dma.Val ||
       ENC_BIST_CHKSUM_OK != chk_sum_bist.Val)
        push_error_fifo(ENC28_BUFF_ERR);
}
#endif

/* **************************************************** */
/* Initialize the receive/transmit buffer of the enc    */
/* **************************************************** */
static void enc_init_buffer_mem(void)
{
    /* receive buffer */
    enc_reg_write(ENC_REG_ADR_ERXSTL, (UINT1)(RXSTART));
    enc_reg_write(ENC_REG_ADR_ERXSTH, (UINT1)(RXSTART>>8));

    enc_reg_write(ENC_REG_ADR_ERXNDL, (UINT1)(RXSTOP));
    enc_reg_write(ENC_REG_ADR_ERXNDH, (UINT1)(RXSTOP>>8));

    enc_reg_write(ENC_REG_ADR_ERXRDPTL, (UINT1)(RXSTOP));
    enc_reg_write(ENC_REG_ADR_ERXRDPTH, (UINT1)(RXSTOP>>8));

    enc_reg_write(ENC_REG_ADR_ERDPTL, (UINT1)(RXSTART));
    enc_reg_write(ENC_REG_ADR_ERDPTH, (UINT1)(RXSTART>>8));
    enc.rcv_status.p_next_packet = RXSTART;

    /* transmit buffer */
    enc_reg_write(ENC_REG_ADR_EWRPTL, (UINT1)(TXSTART));
    enc_reg_write(ENC_REG_ADR_EWRPTH, (UINT1)(TXSTART>>8));
    MACPut(0x00);

    enc_reg_write(ENC_REG_ADR_ETXSTL, (UINT1)(TXSTART));
    enc_reg_write(ENC_REG_ADR_ETXSTH, (UINT1)(TXSTART>>8));

    enc_reg_bit_set(ENC_REG_ADR_ECON2, ENC_ECON2_AUTOINC);

    enc.pkt_discarded = TRUE;
}

/* **************************************************** */
/* Initialize the mac of the enc                        */
/* **************************************************** */
static void enc_init_mac(void)
{
    enc_reg_write(ENC_REG_ADR_MACON1, MACON1_PASSALL | MACON1_MARXEN);
    /* Late collisions occur beyond 63+8 bytes (8 bytes for preamble/start of frame delimiter)
       55 is all that is needed for IEEE 802.3, but ENC28J60 B5 errata for improper link pulse
       collisions will occur less often with a larger number */
    enc_reg_write(ENC_REG_ADR_MACLCON2, 63);
    enc_reg_write(ENC_REG_ADR_MACON3,   MACON3_PADCFG0 | MACON3_TXCRCEN);
    enc_reg_write(ENC_REG_ADR_MACON4,   MACON4_DEFER);

    enc_reg_write(ENC_REG_ADR_MAMXFLL,  (UINT1)(MAC_MAX_FRM_LEN));
    enc_reg_write(ENC_REG_ADR_MAMXFLH,  (UINT1)(MAC_MAX_FRM_LEN>>8));

    enc_reg_write(ENC_REG_ADR_MABBIPG,  ENC_RECOMMENDED_MABBIPG);
    enc_reg_write(ENC_REG_ADR_MAIPGL,   ENC_RECOMMENDED_MAIPGL);
    enc_reg_write(ENC_REG_ADR_MAIPGH,   ENC_RECOMMENDED_MAIPGH);

    /* Initialize physical MAC address registers */
    memcpy(AppConfig.MyMACAddr.v, mac_address_enc.v, sizeof(mac_address_enc));

    enc_reg_write(ENC_REG_ADR_MAADR1, AppConfig.MyMACAddr.v[0]);
    enc_reg_write(ENC_REG_ADR_MAADR2, AppConfig.MyMACAddr.v[1]);
    enc_reg_write(ENC_REG_ADR_MAADR3, AppConfig.MyMACAddr.v[2]);
    enc_reg_write(ENC_REG_ADR_MAADR4, AppConfig.MyMACAddr.v[3]);
    enc_reg_write(ENC_REG_ADR_MAADR5, AppConfig.MyMACAddr.v[4]);
    enc_reg_write(ENC_REG_ADR_MAADR6, AppConfig.MyMACAddr.v[5]);

    enc_reg_write(ENC_REG_ADR_ERXFCON, ENC_RCV_FILTER);
}

/* **************************************************** */
/* Initialize the phy of the enc                        */
/* **************************************************** */
static void enc_init_phy(void)
{
    enc_reg_phy_write(PHY_REG_ADR_PHCON2, PHCON2_JABBER_CORR_DIS | PHCON2_HDLDIS);

    enc_reg_phy_write(PHY_REG_ADR_PHLCON, PHLCON_RESERVED | LEDACFG | LEDBCFG | PULSE_STRETCH);

    /* Set the MAC and PHY into the proper duplex state */
    enc_reg_phy_write(PHY_REG_ADR_PHCON1, 0x0000);
}

#if 0
void get_rcv_status(UINT1 *p_data, UINT2 len)
{
    if(len > sizeof(ENC28_TYPE))
        len = sizeof(ENC28_TYPE);

    mcpy(p_data, (UINT1 *)&enc, len);
}
void get_rcv_buff(UINT1 *p_data, UINT2 start, UINT2 len)
{
    WORD_VAL ReadPtrSave;

    // Save the current read pointer (controlled by application)
    ReadPtrSave.v[0] = enc_reg_read(ENC_REG_ADR_ERDPTL);
    ReadPtrSave.v[1] = enc_reg_read(ENC_REG_ADR_ERDPTH);

    enc_reg_write(ENC_REG_ADR_ERDPTL, (UINT1)(start));
    enc_reg_write(ENC_REG_ADR_ERDPTH, (UINT1)(start>>8));

    MACGetArray(p_data, len);
    // Restore the current read pointer
    enc_reg_write(ENC_REG_ADR_ERDPTL, ReadPtrSave.v[0]);
    enc_reg_write(ENC_REG_ADR_ERDPTH, ReadPtrSave.v[1]);
}
#endif

/* **************************************************** */
/* Restart enc                                          */
/* **************************************************** */
static void enc_reset(void)
{
    BYTE i;
    ULONG4 wait_cnt = 0, try_cnt = 0;

    /* RESET the entire ENC28J60, clearing all registers
       Also wait for CLKRDY to become set.
       Bit 3 in ESTAT is an unimplemented bit.  If it reads out as '1' that
       means the part is in RESET or there is something wrong with the SPI
       connection.  This loop makes sure that we can communicate with the
       ENC28J60 before proceeding. */

    do
    {
        spi_write(ENC_OP_RESET, SPI_CS_MAIN_ENC);

        /* Errata B4 - 2 */
        while(++wait_cnt < 3000);  /* wait some time (1.4ms for scarts at 50MHz) */
        wait_cnt = 0;

        i = enc_reg_read(ENC_REG_ADR_ESTAT);

        if(try_cnt++ > 3)
            return;
    } while((i & 0x08) || (~i & ENC_ESTAT_CLKRDY));
}

/* **************************************************** */
/* Write data to a control register of the enc          */
/* **************************************************** */
static void enc_reg_write(UINT1 reg_adr, UINT1 data)
{
    enc_select_bank(reg_adr);
    reg_adr &= BANK_SEL_ADR_MASK;
    spi_wr_wr(reg_adr | ENC_OP_WR_CTRL_REG, data, SPI_CS_MAIN_ENC);
}

/* **************************************************** */
/* Read data from a control register of the enc         */
/* **************************************************** */
static UINT1 enc_reg_read(UINT1 reg_adr)
{
    enc_select_bank(reg_adr);

    if(check_mac_mii_reg(reg_adr))
    {
        UINT2 reg_data = 0;
        spi_wr_rd_n((reg_adr & BANK_SEL_ADR_MASK) | ENC_OP_RD_CTRL_REG, SPI_CS_MAIN_ENC, sizeof(reg_data), (UINT1 *)&reg_data);
        return (UINT1)(reg_data>>8);
    }else
        return spi_wr_rd((reg_adr & BANK_SEL_ADR_MASK) | ENC_OP_RD_CTRL_REG, SPI_CS_MAIN_ENC);
}

/* **************************************************** */
/* Set bits in one of the eth-registers only            */
/* **************************************************** */
static void enc_reg_bit_set(UINT1 reg_adr, UINT1 bit_mask)
{
    if(check_mac_mii_reg(reg_adr))
        return;

    enc_select_bank(reg_adr);
    reg_adr &= BANK_SEL_ADR_MASK;
    spi_wr_wr(reg_adr | ENC_OP_ST_CTRL_REG, bit_mask, SPI_CS_MAIN_ENC);
}

/* **************************************************** */
/* Clear bits in one of the eth-registers only          */
/* **************************************************** */
static void enc_reg_bit_clear(UINT1 reg_adr, UINT1 bit_mask)
{
    if(check_mac_mii_reg(reg_adr))
        return;

    enc_select_bank(reg_adr);
    reg_adr &= BANK_SEL_ADR_MASK;
    spi_wr_wr(reg_adr | ENC_OP_CR_CTRL_REG, bit_mask, SPI_CS_MAIN_ENC);
}

/* **************************************************** */
/* Write data to a phy register of the enc              */
/* **************************************************** */
static void enc_reg_phy_write(UINT1 reg_adr, UINT2 data)
{
    enc_reg_write(ENC_REG_ADR_MIREGADR, reg_adr>>1);

    enc_reg_write(ENC_REG_ADR_MIWRL, (UINT1)(data));
    enc_reg_write(ENC_REG_ADR_MIWRH, (UINT1)(data>>8));

    while(enc_reg_read(ENC_REG_ADR_MISTAT) & ENC_MISTAT_BUSY);
}

/* **************************************************** */
/* Read data from a phy register of the enc             */
/* **************************************************** */
static UINT1 enc_reg_phy_read(UINT1 reg_adr)
{
    enc_reg_write(ENC_REG_ADR_MIREGADR, reg_adr>>1);
    enc_reg_write(ENC_REG_ADR_MICMD, ENC_MICMD_MIIRD);

    while(enc_reg_read(ENC_REG_ADR_MISTAT) & ENC_MISTAT_BUSY);

    enc_reg_write(ENC_REG_ADR_MICMD, 0x00);

    if(reg_adr & 0x01) /* odd address. just return high-byte */
        return enc_reg_read(ENC_REG_ADR_MIRDH);
    else
        return enc_reg_read(ENC_REG_ADR_MIRDL);
}

/* **************************************************** */
/* Select one of the registerbanks                      */
/* **************************************************** */
static void enc_select_bank(UINT1 reg_adr)
{
    UINT1 bank_nr;

    if((reg_adr & BANK_SEL_ADR_MASK) > BANK_SEL_REG_BANKED)
        return;

    bank_nr = (reg_adr >> BANK_SEL_BANK_NR_SHIFT) & BANK_SEL_BANK_MASK;

    if(enc.cur_reg_bank == bank_nr)
        return;

    enc_reg_bit_clear(ENC_REG_ADR_ECON1, BANK_SEL_BANK_MASK);
    enc_reg_bit_set(ENC_REG_ADR_ECON1, bank_nr);

    enc.cur_reg_bank = bank_nr;
}

/* **************************************************** */
/* Check if adress is mac or mii register               */
/* **************************************************** */
static UINT1 check_mac_mii_reg(UINT1 reg_adr)
{
    if(((reg_adr & BANK_SEL_ADR_MASK) < BANK_SEL_REG_BANKED) &&
        (((reg_adr > ENC_REG_ADR_M_LOW) && (reg_adr < ENC_REG_ADR_M_HIGH)) || (ENC_REG_ADR_MISTAT == reg_adr)))
        return 1;
    else
        return 0;
}
