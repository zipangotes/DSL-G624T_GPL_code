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
 * FILE NAME    :  usb20_state.h  
 *
 * DESCRIPTION  :  Implements USB20 Driver State Machine
 *
 * HISTORY 	:  
 *
 * 	Aug 05  -  sabyasachi  -  created. 
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *****************************************************************************/


#ifndef __USB20_STATE_H
#define __USB20_STATE_H

#include "psp_types.h"
#include "usb20_lib.h"


/*****************************************************************************
 * 
 *                ===================================
 *                |  States for USB Protocol Driver | 
 *                ===================================
 *
 * These states are defined as per USB 2.0 Protocol Specification
 * 
 * State Name                     Description 
 * --------------------------------------------------------------------------

 ****************************************************************************/

typedef enum _USB20_STATE 
{
        USB20_DRV_STATE_DETACHED = 0,
        USB20_DRV_STATE_SUSPENDED,
        USB20_DRV_STATE_DEFAULT,
        USB20_DRV_STATE_ADDRESS,
        USB20_DRV_STATE_CONFIGURED
}USB20_STATE;


INT32 usb_drv_state_init(void);
INT32 usb_drv_set_state(UINT32 state);
INT32 usb_drv_get_state(void);
INT32 usb_drv_state_shutdown(void);


#endif  /* __USB20_STATE_H  */
