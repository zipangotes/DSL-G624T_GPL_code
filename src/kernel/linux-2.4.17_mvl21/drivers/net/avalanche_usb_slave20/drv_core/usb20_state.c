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
 * FILE NAME    :  usb20_state.c  
 *
 * DESCRIPTION  :  Implements USB20 Driver State Machine
 *
 * HISTORY 	:  
 *
 * 	Aug 05  -  sabyasachi  -  created. 
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *****************************************************************************/

#include "usb20_state.h"

static INT32  usb20_state;  /* The state machine */

INT32 usb_drv_state_init()
{
    USB_ENTER;
    usb20_state = USB20_DRV_STATE_DETACHED;
    USB_EXIT;
    return 0;
}

INT32 usb_drv_set_state(UINT32 state)
{
    USB_ENTER;
    if ( state >= USB20_DRV_STATE_DETACHED && state <= USB20_DRV_STATE_CONFIGURED )
        usb20_state = state;
    else 
	 return -1;
    USB_EXIT;
    return usb20_state;
}

INT32 usb_drv_get_state()
{
    USB_ENTER;
    USB_EXIT;
    return usb20_state;
}

INT32 usb_drv_state_shutdown()
{
    USB_ENTER;
    usb20_state = USB20_DRV_STATE_DETACHED;
    USB_EXIT;
    /* nothing to do as of now */
    return 0;
}

