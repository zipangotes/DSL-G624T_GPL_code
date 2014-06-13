/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 2003 Texas Instruments Incorporated                |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission is hereby granted to licensees of Texas Instruments           |**
**| Incorporated (TI) products to use this computer program for the sole     |**
**| purpose of implementing a licensee product based on TI products.         |**
**| No other rights to reproduce, use, or disseminate this computer          |**
**| program, whether in part or in whole, are granted.                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/**************************************************************************
 * FILE NAME    :   usb20_drv.h
 *
 * DESCRIPTION  :
 *
 * HISTORY 	:
 *
 *  Aug 05  -  sabyasachi  -  Created
 *  Sep 01  -  sabyasachi  -  Added USB Command Processor Unit defs
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/

#ifndef __USB20_DRV_H_
#define __USB20_DRV_H_


#include "psp_types.h"
#include "usb20_drvapi.h"
#include "usb20_drvhal.h"
#include "usb20_def.h"
#include "usb_sal.h"

typedef struct USB_PARAMS_T  USB_PARAMS;
typedef struct USB_STATS_T   USB_STATS;
typedef struct USB_CONTROL_TX_PKT_T USB_CTL_TX_PKT;
typedef struct USB_CCPU_T     USB_CCPU;   

/* Maximum number of elements in Control Tx Buffer free list */
#define USB_CTL_TX_FREE_LIST_SIZE  5

/* Maximum number of End Points HAL/Silicon */
#define USB20_NUM_EP          4   /* Except Control End Point */

/* Maximum number of channels HAL/Silicon supports */
#define USB20_NUM_CHANNEL     4

/* Maximum number of fragments in a received packet from HAL */
#define  MAX_FRAG_PER_PACKET     4

/*
 *  USB End Point States
 *
 *  CLOSED  :  No operation (other than ep_open) can be performed on this
 *             end point.
 *
 *  READY   :  End Point open has been requested by Class Driver however
 *             corresponding CPPI channel is not open yet. One of the 
 *             following situations may be possible : 
 *                -  Cable Disconnected
 *                -  Bus reset happened 
 *                -  Class Driver asked Protocol driver to Stop (usb_drv_stop)
 *  
 *  OPEN    :  End Point is ready for normal data transfer.
 */

#define USB20_EP_STATE_CLOSED     0
#define USB20_EP_STATE_READY      1
#define USB20_EP_STATE_OPEN       2

/* values for End Point status */
#define USB20_EP_IN_HALT          0x01
#define USB20_EP_OUT_HALT         0x10
#define USB20_EP_IN_HALT_MASK     0x01
#define USB20_EP_OUT_HALT_MASK    0x10


struct USB_EP_T
{
    USB_PIPE   *pipe;      /* Pipe configured by Class Driver */
    struct USB_DEV_T    *usb_dev;   /* Pointer to USB Device structure, driver MCB */
    CHANNEL_INFO *hal_ch; /* HAL Specific channel information, type is opaque 
			      to upper layer */
    UINT8       id;    	   /* End Point Number */
    UINT8       type;      /* BULK | CONTROL | INT */
    UINT8       dir;       /* End Point Direction */
    UINT16       max_size;  /* Maximum Transfer Size on this End Point */
    UINT16      state;	  
    UINT16      status;
} ;


#define USB20_FULL_SPEED     0
#define USB20_HIGH_SPEED     1

#define USB_SERIAL_NO_CHAR_MAX  256
#define USB_MANUFACTURER_STRING_CHAR_MAX  256
#define USB_PRODUCT_STRING_CHAR_MAX  256

struct USB_PARAMS_T
{
    UINT32    usb_base;  /* USB Module base Address */
    UINT32    usb_offset;  /* USB Module Address Offset */
    UINT32    reset_base;
    UINT32    reset_bit;
    UINT32    clkc_base;
    UINT32    clkc_mask;
    UINT32    speed;      /* Full | High ?? Or should we keep just enable */
    UINT32    usb_irq;
    UINT32    vbus_irq;
    UINT32    ucp_base;
    UINT32    ucp_disable_val;
    UINT32    ucp_disable_mask;
    UINT32    bulk_ep_size;   /* Size of BULK End Point */
    UINT32    ctrl_ep_size;    /* Size of CONTROL End Point */
    UINT32    int_ep_size;     /* Size of INTERRUPT End Point */
    UINT32    mode;       /* RNDIS / Normal ?? */
    UINT32    en_double_buf;  /* Double buffering for Control EP */
    UINT16    pid;
    UINT16    vid;
    UINT16    address;
    UINT16    max_power;
    UINT8      serial_no[USB_SERIAL_NO_CHAR_MAX];
	UINT8     manufacturer_string[USB_MANUFACTURER_STRING_CHAR_MAX];
	UINT8     product_string[USB_PRODUCT_STRING_CHAR_MAX];
	
} ;

#define USB20_CHANNEL_UP              0x00
#define USB20_CHANNEL_DOWN            0x01
#define USB20_CHANNEL_UPDOWN_MASK     0x01

#define USB20_CHANNEL_DIR_IN          0x01
#define USB20_CHANNEL_DIR_OUT         0x02
#define USB20_CHANNEL_DIR_BOTH        0x03

typedef struct USB_CHANNEL_T
{
    UINT8  ep_no;    /* End Point numbers */
    UINT8  dir;      /* Channel Direction */
    UINT8  status;   /* Status of this channel */
    UINT8  reserved;
    CHANNEL_INFO *halch;  /* Reference to the corresponding HAL channel */
} USB_CHANNEL;


struct USB_STATS_T
{
    UINT32    EnumCount;
    UINT32    LinkStatus;  /* Link up / Link down */
};

/* definitions for link_status field in USB_DEV */
#define USB_LINK_DOWN  0
#define USB_LINK_UP    1


typedef struct USB_SENDINFO_T
{
   USB_EP   *ep;
   UINT8    owner;         /* Who send ? Class driver or protocol driver ? */
   UINT16   flags;       
   void     *os_sendinfo;  /* Sendinfo passed from Class Driver */
} USB_SENDINFO;


/** 
 *  Control Tx Packet Definition
 */

struct USB_CONTROL_TX_PKT_T
{
    USB_LIST_NODE    list;         /* To be used by list library */
    USB_SENDINFO     *sendinfo;    /* Send info to be used by sendcomplete */
    USB_PKT          *pkt;         /* Control Packet */
    UINT32           len;          /* Size of control data */
    UINT8            *data;        /* Control Data buffer */
};

typedef struct USB_DEV_T
{
    /* Add a mutex here */
    HAL_DEVICE        *hal_dev;
    HAL_FUNCTIONS     *hal_funcs;
    INT32             (*hal_usb_isr)(HAL_DEVICE *, INT32 *);
    INT32             (*hal_vbus_isr)(HAL_DEVICE *, INT32 *);
    OS_FUNCTIONS      *os_funcs;
    USB_CONFIG        *usb_cfg;
    CLASS_DEV         *class_dev;      /* Class Driver Control Block */
    USB_EP            ep_control;   /* Control End Point */
    USB_EP            ep_table[USB20_NUM_EP];
    USB_CCPU          *ccpu;        /* Control Command Processor Unit */
    USB_CTL_TX_PKT    current_ctl_tx_pkt; 
    USB_PARAMS        params;
    USB_STATS         stats;
    USB_CHANNEL       channel[USB20_NUM_CHANNEL]; /*Status per channel*/
    UINT32            status;       /* Status of driver */
    UINT32            link_status;  /* Link is up or down */
    VOID              *priv;        /* Private Data for USB */
} USB_DEV;

/* Macros to access USB Device elements */
#define USB_PARAM(d,p)  d->params.p

#define USB_PROTOCOL_DRIVER   0
#define USB_CLASS_DRIVER      1




/*************************************************************************
 *
 *    All Definitions for Control Command Processor Unit 
 *
 ************************************************************************/

typedef enum  USB_CCPU_STATE
{
    CCPU_SETUP_REQUEST = 1,    /* CCPU is waiting for SETUP packet */
    CCPU_DATA_OUT,             /* CCPU is waiting for Control Data */
    CCPU_PROCESS_COMMAND,      /* CCPU is processing Control Commands */
    CCPU_RESET                 /* CCPU has received RESET from host */
} USB_CCPU_STATE;

#define CCPU_CONTROL_DATA_MAX_LEN     256

/** 
 * USB Control Command Processor Unit (CCPU) Structure
 */

typedef struct EP0_BUFFER_T
{
	UINT8  *data;
	UINT32 len;
}EP0_BUFFER;


#define SEL_BUFFER_X 0
#define SEL_BUFFER_Y 1

struct USB_CCPU_T
{
    USB_CCPU_STATE  state;          /* State of the Command Processor Unit */
    EP0_BUFFER      *ep_buf;        /* EP0_Buffer for Control End Point */
    UINT8           work_buf[CCPU_CONTROL_DATA_MAX_LEN];
                                    /* Buffer for receving control data */
    UINT32          work_buf_len;   /* Length of above buffer */
    USB_PKT         pkt;            /* Packet to send control data */
    USB_BUF         pkt_buf;        /* Data buffer to send control data */
    USB_SENDINFO    sendinfo;       /* For sending control data */
    UINT8           current_config; /* Current Config set by Host */
    UINT8           xy_buf_sel[2];  /* X/Y Buffer Selector */
};



#endif 

