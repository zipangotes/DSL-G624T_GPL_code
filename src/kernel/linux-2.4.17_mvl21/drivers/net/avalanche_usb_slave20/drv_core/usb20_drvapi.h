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

/******************************************************************************
 * FILE NAME    :  usb20_drvapi.h 
 *
 * DESCRIPTION  :  Header file for all API and data structures implemented
 *                 and exposed by Protocol driver.
 *
 * HISTORY 	:
 *
 * 	Aug 05  -  sabyasachi  -  created. 
 * 	
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *****************************************************************************/

#ifndef __USB20_DRVAPI_H_
#define __USB20_DRVAPI_H_

#include "psp_types.h"     /* PSP Specific type definitions */
#include "usb20_def.h"
#include "usb20_state.h"
#include "usb20_config.h"

typedef INT32 USB_STATUS;  /* Error Code and Return status type for all if routines */

/* End Point types supported by this driver */

#define USB_EP_T_CONTROL     0x1
#define USB_EP_T_INT         0x2
#define USB_EP_T_BULK        0x3
 
/* End Point Directions */

#define USB_EP_DIR_IN	     0x1
#define USB_EP_DIR_OUT       0x2
#define USB_EP_DIR_INOUT     0x3

/* End Point Max Transfer Size */

#define USB_EP_CONTROL_MAX_SIZE  64   
#define USB_EP_INT_MAX_SIZE      64      
#define USB_EP_BULK_MAX_SIZE    512 

typedef struct USB_PIPE_T  USB_PIPE;
typedef struct USB_BUF_T   USB_BUF;
typedef struct USB_PKT_T   USB_PKT;
/* typedef struct USB_DEV_T   USB_DEV; */
typedef struct USB_EP_T    USB_EP;


typedef INT32 (*EP_RCV_CB)(USB_PIPE *pipe, USB_PKT *pkt);
typedef INT32 (*EP_SEND_COMPLETE_CB)(USB_PIPE *pipe, void *sendinfo); 

/*
 *  USB_PIPE structure is populated by upper layer class driver and 
 *  passed to protocol driver during creating end points. Each end 
 *  point can be configured as both IN and OUT channel. Class driver
 *  has to implement two mandatory callback routines :
 *      -  sendComplete() is called when data is sent over bus 
 *      -  receive() is called when data is received on this end point
 *
 *  PIPE is a bidirectional communication entity. Class drivers open 
 *  one or more pipes (in unidirectional or bidirectional mode). Inter-
 *  nally USB translated them to End Points. 
 */
 
struct USB_PIPE_T
{
    UINT8      type;              /*!< EP Type (CONTROL|BULK|INT) */
    UINT8      dir;               /*!< Direction of communication */
    UINT8      reserved1;
    UINT8      reserved2;
    EP_RCV_CB  receive;
    EP_SEND_COMPLETE_CB  send_complete; 
    USB_ENDPOINT_DESCR *epd_in;   /* Associated End Point Descriptor */
    USB_ENDPOINT_DESCR *epd_out;  /* Associated End Point Descriptor */
    void       *class_drv_info;   /* Class Driver Private Information */
};


/*
 *
 *  USB Buffer Structure. USB Packet is formed of as a list of few such buffers.
 */ 
 
struct USB_BUF_T {
    UINT32  len;    
    UINT8   *data;
    void    *info;
} ;


struct USB_PKT_T 
{
    USB_BUF  *buf_list;
    UINT32   num_buf;
    UINT32   pkt_size;
} ;

/*
 *  The following structure maintains cache of all USB descriptors.
 *  All descriptors are basically owned by Class Driver sitting on 
 *  top of USB Protocol Driver. But USB Driver keeps cache of them
 *  to respond quickly to the host queries (this response takes 
 *  place in interrupt context, hence time is valuable.
 */
typedef struct USB_CONFIG_T
{
    USB_DEVICE_DESCR *device_desc;      /* Standard Device Descriptor */
    void             *config;           /* All configuration descriptors */
    UINT32 config_desc_length;          /* length of config_desc */
    USB_CONFIG_DESCR *config_desc;      /* Standard Config Descriptor */
    USB_INTERFACE_DESCR  **if_desc_tab; /* Table of Interface Descriptors */
    UINT32 num_if_desc;                 /* Number of Interface Descriptors */
    USB_ENDPOINT_DESCR **ep_desc_tab;   /* Table of End Point Descriptors */
    UINT32 num_ep_desc;                 /* Number of End Point Descriptors */
    USB_STRING_DESCR **string;          /* String descriptors */
    UINT32 num_strings;                 /* number of string descriptors */
} USB_CONFIG;


/* List of USB Events */

#define USB_EVENT_LINK_UP       0x1
#define USB_EVENT_LINK_DOWN     0x2
#define USB_EVENT_RESET         0x3
#define USB_EVENT_SUSPEND       0x4
#define USB_EVENT_DATA_XMIT     0x5
#define USB_EVENT_DATA_RECV     0x6


typedef struct CLASS_DEV_FUNCS_T
{
    INT32 (*usb_class_dev_event)(UINT32 event, UINT8 *data);
    INT32 (*usb_class_dev_query)(UINT32 command, UINT8 *data);
    void  *(*MallocBuffer)(UINT32 size, USB_PIPE *pipe, void **buf_info);
    void  (*FreeBuffer)(void *mem_ptr, void *buf_info);
} CLASS_DEV_FUNCS;

typedef struct CLASS_DEV_T
{
    UINT8   *name;
    UINT8    version_major;
    UINT8    version_minor;
    CLASS_DEV_FUNCS  *funcs;
    void  *priv;
} CLASS_DEV;


/**
 * @defgroup ioctl_cmds IOCTL Commands 
 *
 * @{
 *
 * Below is a list of IOCTL commands supported by USB driver. 
 */

/* IOCTL Commands */

#define IOCTL_GET_USB_CONFIG    0x00000001 	
#define IOCTL_SET_USB_CONFIG    0x00000002 	
#define IOCTL_GET_USB_STATS     0x00000003 /*!< Returns USB Stats */
#define IOCTL_SET_SERIAL_NO     0x00000004
#define IOCTL_GET_SERIAL_NO     0x00000005 /*!< Returns USB Serial No */
#define IOCTL_SET_VENDOR_ID     0x00000006
#define IOCTL_GET_VENDOR_ID     0x00000007 /*!< Returns USB Vendor ID */
#define IOCTL_SET_PRODUCT_ID    0x00000008
#define IOCTL_GET_PRODUCT_ID    0x00000009 /*!< Returns USB Product ID */
#define IOCTL_SET_MAX_POWER     0x0000000A
#define IOCTL_GET_MAX_POWER     0x0000000B /*!< Returns USB Max Power Value */
#define IOCTL_GET_CTRL_EP_SIZE	0x0000000C /*!< Returns size of Control End Point */
#define IOCTL_GET_BULK_EP_SIZE	0x0000000D /*!< Returns size of Bulk End Point */
#define IOCTL_GET_INT_EP_SIZE	0x0000000E /*!< Returns size of Interrupt End Point */
#define IOCTL_GET_ATTRIB	0x0000000F /*!< Bus/Self Power, Remote Wakeup */
#define IOCTL_GET_USB_PARAMS  0x00000010  /*!< MIB-II USB Params, see usb20_mib2_h for mib2_usbParams structure */
#define IOCTL_GET_IF_COUNTERS 0x00000020  /*!< MIB-II IF Counter, see usb20_mib2_h for mib2_ifCounters */
#define IOCTL_GET_USB_SPEED  0x00000030  /*!< USB Link Speed */
#define IOCTL_GET_MANUFACTURER_STRING  0x00000040  /*!< Returns USB Manufacturer sring */
#define IOCTL_GET_PRODUCT_STRING  0x00000050  /*!< Returns USB Product String */

/* IOCTL Command Data Structures */


/**
 * @} */

 
/* API Definitions */

struct USB_DEV_T;

USB_STATUS   usb_drv_init (CLASS_DEV *class_dev, struct USB_DEV_T **dev);
USB_STATUS   usb_drv_start (struct USB_DEV_T *dev, USB_CONFIG  *usbcfg);
USB_STATUS   usb_drv_stop (struct USB_DEV_T *dev);
USB_STATUS   usb_drv_shutdown (struct USB_DEV_T *dev);
USB_STATUS   usb_drv_send (struct USB_DEV_T *dev, USB_EP *ep, USB_PKT *pkt, void *sendinfo);
USB_STATUS   usb_drv_ioctl (struct USB_DEV_T *dev, UINT32 command, void  *data); 
USB_STATUS   usb_drv_ep_open (struct USB_DEV_T *dev, USB_PIPE *pipe, USB_EP **ep);
USB_STATUS   usb_drv_ep_close (USB_EP *ep);
struct USB_DEV_T *    usb_drv_get_dev(void);

/**
 * @defgroup Error_Codes Error Codes 
 *
 * @{
 */
/* Error Codes */

#define USB_STATUS_OK                                0
#define USB_STATUS_ERROR                             -1 
#define USB_STATUS_INV_ARG                           -2
#define USB_STATUS_NO_SUPPORT                        -3
#define USB_STATUS_INV_COMMAND                       -4
#define USB_STATUS_NO_MEM                            -5
#define USB_STATUS_HAL_ERROR                         -6
#define USB_STATUS_CTL_QUEUE_FULL                    -7                   
#define USB_STATUS_INV_STATE                         -8
#define USB_STATUS_EP_IN_USE                         -9
#define USB_STATUS_NO_CHANNEL                        -10
#define USB_STATUS_START_FAILED                      -11
#define USB_STATUS_STOP_FAILED                       -12
#define USB_STATUS_HAL_INIT_FAILED                   -13
#define USB_STATUS_CHANNEL_SETUP_FAILED              -14
#define USB_STATUS_INV_EP_DIR                        -15
#define USB_STATUS_INV_EP_TYPE                       -16

/**
 * @}
 */
#endif 

