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
 * FILE NAME    :  usb20_drvhal.h  
 *
 * DESCRIPTION  :  
 *
 * HISTORY 	:  
 *
 * Date        Name          Notes 
 * ------------------------------------------------------------------------
 * 25 Jul 03   Sabyasachi    Created.
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *****************************************************************************/
#ifndef _USB20_DRVHAL_H_
#define _USB20_DRVHAL_H_

#include "psp_types.h"

typedef VOID OS_PRIVATE;
typedef VOID OS_DEVICE;
typedef VOID OS_SETUP;
typedef VOID OS_SENDINFO;
typedef VOID OS_RECEIVEINFO;

#include "cpswhal_cpusb.h"
#include "usb20_lib.h"
#include "usb20_drvapi.h"

typedef int (*HAL_ISR_T)(HAL_DEVICE *, int *);


#define HAL_CHANNEL_MASK             0xf
#define HAL_FREE_BUFFER              0
#define HAL_DO_NOT_FREE_BUFFER       1


/* Macros to access HAL functions */
#define HAL_CONTROL(usb_dev,key,action,value)   \
            usb_dev->hal_funcs->Control(usb_dev->hal_dev, \
			    key, action, (void *)value)
#define HAL_OPEN(usb_dev) \
	    usb_dev->hal_funcs->Open(usb_dev->hal_dev)
	    
#define HAL_SEND(p)      p->hal_funcs->Send

INT32 usb_drv_load_os_functions(OS_FUNCTIONS *); 
void  usb_drv_set_class_dev_funcs(CLASS_DEV_FUNCS *);

extern INT32 usb_drv_hal_receive
(
                   OS_DEVICE *os_dev, 
		      FRAGLIST  *frag_list, 
		      UINT32     frag_count,
		      UINT32     pkt_size, 
		      HAL_RECEIVEINFO *hal_receive_info, 
		      UINT32 mode
);

extern INT32 usb_drv_hal_send_complete(OS_SENDINFO  *os_sendinfo); 
extern INT32 usb_drv_receive(OS_DEVICE *os_dev, USB_PKT    *pkt, 
                                HAL_RECEIVEINFO *hal_receive_info, UINT32 mode);

#endif /*_USB20_DRVHAL_H_*/
