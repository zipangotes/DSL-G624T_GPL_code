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
 * FILE NAME    :  usb20_drvrndis.h  
 *
 * DESCRIPTION  :  Definitions for RNDIS-Protocol Driver interface  
 *
 * HISTORY 	:  
 *
 * 	Aug 05  -  Prashanth Acharya  -  created. 
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *****************************************************************************/

#ifndef _USB20_RNDISAPI_H_
#define _USB20_RNDISAPI_H_

#include "usb20_lib.h"
#include "usb20_drvapi.h"
#include "usb20_config.h"
#include "usb20_drv.h"

typedef void * HAL_HND;

#ifdef USB_BIG_ENDIAN 
    /* BIG Endian. */
    #define HAL_SYS_SWAP(v)     ((((v) >>  8) & 0x00FF) | (((v) <<  8) & 0xFF00))
    #define HAL_SYS_SWAPL(v)            \
    (                                   \
        (((v) >> 24) & 0x000000FF) |    \
        (((v) >>  8) & 0x0000FF00) |    \
        (((v) <<  8) & 0x00FF0000) |    \
        (((v) << 24) & 0xFF000000)      \
    )
#else
    /* Little Endian. */
    #define HAL_SYS_SWAPL(v)    v
    #define HAL_SYS_SWAP(v)     v
#endif


/* **************************************************************************************************
*   Critical Section Context
***************************************************************************************************** */
typedef struct SYS_CRIT_CNT_
{
/*  spinlock_t lock;
    unsigned long flags;*/
    int mutex;

}SYS_CRIT_CNT;

typedef SYS_CRIT_CNT HAL_SYS_CRIT_CNT;

/* **************************************************************************************************
*   Routines provided for the operating system support.
***************************************************************************************************** */
typedef void*   (*HAL_SYS_MALLOC_RNT)(unsigned int size);                   /* malloc() */
typedef void    (*HAL_SYS_FREE_RTN)(void* p_memory);                        /* free() */

typedef int     (*HAL_SYS_START_CRIT_RTN)(HAL_SYS_CRIT_CNT* p_context);     /* start critical section */
typedef int     (*HAL_SYS_END_CRIT_RTN)(HAL_SYS_CRIT_CNT* p_context);       /* end critical section */

typedef void*   (*HAL_SYS_MEMCPY_RTN)(void*, const void*, unsigned int);    /* memcpy() */
typedef void*   (*HAL_SYS_MEMSET_RTN)(void*, int, unsigned int);            /* memset() */
typedef int     (*HAL_SYS_MEMCMP_RTN)(const void*, const void*, unsigned int);  /* memcmp() */

typedef char*   (*HAL_SYS_STRCPY_RTN)(char*, const char*);                  /* strcpy() */
typedef int     (*HAL_SYS_STRCMP_RTN)(const char*, const char*);            /* strcmp() */
typedef char*   (*HAL_SYS_STRNCPY_RTN)(char*, const char*, unsigned int);   /* strncpy() */
typedef int     (*HAL_SYS_STRNCMP_RTN)(const char*, const char*, unsigned int);  /* strncmp() */

typedef int     (*HAL_SYS_LOG_RTN)(int level, char* fmt, int arg1,int arg2,int arg3,int arg4,int arg5); /* system log() */
typedef int     (*HAL_SYS_SPRINTF_RTN)(char* buf, const char* fmt, ...);    /* sprintf() */

typedef UINT32  (*HAL_SYS_TIME_STAMP)(void);

/* **************************************************************************************************
*   HAL_SYS_TBL system support table (operating system-provided services)
***************************************************************************************************** */
typedef struct HAL_SYS_TBL
{
    UINT32                  version;    /* Version: upper 16 bits for major, lower 16 bit for minor */

    /* Memory management items (this is global for all modules) */
    HAL_SYS_MALLOC_RNT      malloc;     /* malloc() routine to allocate permanent memory */
    HAL_SYS_FREE_RTN        free;       /* free() routine to free permanent memory */

    /* Critical section support */
    HAL_SYS_START_CRIT_RTN  crit_start; /* crit_start() routine */
    HAL_SYS_END_CRIT_RTN    crit_end;   /* crit_end() routine */

    /* Memory functions provided by the operating system */
    HAL_SYS_MEMCPY_RTN      memcpy;     /* memcpy() routine */
    HAL_SYS_MEMSET_RTN      memset;     /* memset() routine */
    HAL_SYS_MEMCMP_RTN      memcmp;     /* memcmp() routine */

    /* String library */
    HAL_SYS_STRCPY_RTN      strcpy;     /* strcpy() routine */
    HAL_SYS_STRCMP_RTN      strcmp;     /* strcmp() routine */
    HAL_SYS_STRNCPY_RTN     strncpy;    /* strncpy() routine */
    HAL_SYS_STRNCMP_RTN     strncmp;    /* strncmp() routine */

    HAL_SYS_SPRINTF_RTN     sprintf;    /* sprintf() routine */
    HAL_SYS_LOG_RTN         log;        /* system log() function */
} HAL_SYS_TBL;

/* HAL Logging Levels */
#define HAL_SYS_FATAL       2
#define HAL_SYS_DEBUG       10          /* Make sure that this MACRO always has the lowest value. */


/***************************************************************************/

typedef int   (*HAL_USB_RECEIVE_RTN) (HAL_HND endpoint, UINT32 data[], UINT32 len[], UINT32 num);
typedef int   (*HAL_USB_TX_CMPLT_RTN)(HAL_HND h_device, UINT32 chain_id);
typedef int   (*HAL_USB_NOTIFY_RTN)  (HAL_HND h_device, UINT32 events);
typedef void* (*HAL_USB_IOCTL)       (HAL_HND h_device, UINT32 command, void* arg1, void* arg2, void* arg3);




/**************************************************************************
 * STRUCTURE -  HAL_USB_APP_INFO
 ***************************************************************************
 *  The structure is the application information structure that is filled 
 *  by the application and passed to the USB HAL. It contains information 
 *  about call back routines which will be used to pass information between 
 *  the USB HAL and application layer.
 ***************************************************************************/
typedef struct HAL_USB_APP_INFO
{
    /* Unit number of the USB Device. */
    int                     unit;

    /* USB Application handle.      */
    HAL_HND                 h_usbapp;
    
    /* This is the registered receive routine that is to be called when 
     * data is received on the CONTROL OUT endpoint. */ 
    HAL_USB_RECEIVE_RTN     receive_rtn;

    /* This is the NOTIFICATION routine that will be called by the USB HAL 
     * to inform the application about USB events like RESET etc. */
    HAL_USB_NOTIFY_RTN      notify_rtn;

    /* This is the call back routine to inform the USB layer that data has 
     * been txed on the CONTROL IN endpoint and that the associated memory 
     * block can be cleaned up. */
    HAL_USB_TX_CMPLT_RTN    tx_cmplt_rtn;

    /* Registered callback IOCTL command which will be used by the CCU layer 
     * to GET / SET device configuration. */
    HAL_USB_IOCTL           ioctl_rtn;
}HAL_USB_APP_INFO;




#define     HAL_USB_GET_EP_SIZE             0x1
#define     HAL_USB_IOCTL_GET_ATTRIB        0x7
#define     HAL_USB_IOCTL_GET_MAX_POWER     0x8



/*********** Application Provided IOCTL commands ***************************/

/* This a list of IOCTL commands which MUST be provided by the application. 
 * These are called by the USB HAL to get / set USB configuration 
 * information.  */
#define     USB_APP_IOTCL_GET_DEVICE_DESC   0x1
#define     USB_APP_IOTCL_GET_CONFIG_DESC   0x2
#define     USB_APP_IOTCL_GET_STRING_DESC   0x3
#define     USB_APP_IOTCL_GET_INTERFACE     0x4
#define     USB_APP_IOCTL_SET_INTERFACE     0x5
#define     USB_APP_SET_DEVICE_DESCRIPTOR   0x6
#define     USB_APP_SET_CONFIG_DESCRIPTOR   0x7
#define     USB_APP_SET_STRING_DESCRIPTOR   0x8




#define     HAL_USB_RESET_EVENT             0x1

/**************************************************************************
 * STRUCTURE -  HAL_USB_EP_ID
 **************************************************************************
 *  The following is a list of valid endpoint identifiers. 
 **************************************************************************/
typedef enum HAL_USB_EP_ID
{
    HAL_USB_CTRL_IN         = 0x1,
    HAL_USB_CTRL_OUT        = 0x2,
    HAL_USB_BULK_IN         = 0x3,
    HAL_USB_BULK_OUT        = 0x4,
    HAL_USB_INTERRUPT_IN    = 0x5
}HAL_USB_EP_ID;

/* Bulk Endpoint Address. */
#define HAL_USB_APP_EP_BULK_ID      0x01

/* Interrupt Endpoint Address */
#define HAL_USB_APP_EP_INT_ID       0x02


/**************************************************************************
 * STRUCTURE -  HAL_USB_STRING_ENTRY
 **************************************************************************
 *  The structure identifes a string descriptor. While passing device 
 *  configuration to the HOST during enumeration, it is possible that the 
 *  device configuration give indexes into a string list which identify that 
 *  particular configuration. The string list is seperately requested later 
 *  in the enumeration process. For more information see 9.6.5 of the 
 *  USB Specification.
 **************************************************************************/
typedef struct HAL_USB_STRING_ENTRY
{
    /* Links the USB HAL string entries into a doubly linked list. */
    USB_LIST_NODE                       node;

    /* This is the index field which matches the configuration capability 
     * with the textual string. Care should be taken by the upper layer to make 
     * sure that the index is the same as what was advertised in the device 
     * configuration. CASE 0 - This is a special case which should always 
     * be present. This identifies all the languages that are supported. */
    UINT8                           index;
    
    /* Unicode string descriptor. */
    char*                           p_usb_str_descr;

    /* Length of string descriptor. */
    UINT16                          str_descr_length;
}HAL_USB_STRING_ENTRY;


/**************************************************************************
 * STRUCTURE -  USB20_RNDIS_PKT
 **************************************************************************
 * This structure define the structure of a RNDIS layer packet. It contains 
 * the USB_PKT structure that can be passed to the protocol driver.
 *
 * NOTE: The buf field can be used for the buf_list of USB_PKT.
 **************************************************************************/

typedef struct USB20_RNDIS_PKT
{
    USB_LIST_NODE node;
    USB_PKT usb20_pkt;
    UINT32 os_pkt_id;
    USB_BUF buf[3];              /* 
                                          *  For now, send pipe will have at most 3 buffers only 
                                          *  1 - RNDIS Header 
                                          *  2 - IP Data Payload
                                          *  3 - If short packet is required
	                                    */
                                         /* TODO: make this also as a dynamic list */
    UINT8     short_data;
}USB20_RNDIS_PKT;

#define USB20_RNDIS_MAX_PKTS    RNDIS_BULK_MAX_BUFFERS + RNDIS_CONTROL_MAX_BUFFERS + 16 
                                /* 16 pkts for the interrupt endpoint.
                                 * TODO: Remove arbitrary value 1. 
                                 */


extern HAL_HND hal_usb_dev_open (HAL_SYS_TBL *ptr_sys_tbl);
extern int hal_usb_dev_start (HAL_HND h_device, HAL_USB_APP_INFO* ptr_app_info);
extern int hal_usb_dev_close (HAL_HND h_device);
extern HAL_HND hal_usb_open_pipe (HAL_HND h_device, HAL_USB_EP_ID id, void *vptr);
extern int hal_usb_dev_close_pipe (HAL_HND h_pipe);
extern int hal_usb_dev_ep_stall (HAL_HND hnd_pipe);
extern int hal_usb_dev_send_pipe (HAL_HND h_pipe,UINT32 data[],UINT32 len[],UINT32 num_buffer,UINT32 chain_id);
extern int hal_usb_dev_ioctl (HAL_HND h_device, UINT32 command, void *argument);
extern int hal_usb_dev_poll (HAL_HND h_device, HAL_HND endpoint);
extern HAL_HND hal_usb_get_app_handle (HAL_HND h_device);
extern HAL_HND hal_usb_get_app_handle_from_endpoint (HAL_HND endpoint);

extern UINT32 hal_usb_get_phy_medium (void);
extern int hal_usb_pform_config_parameter (void *ptr_config);
extern UINT32 hal_usb_pform_connection_status (void);

#endif /* _USB20_RNDISAPI_H_*/

