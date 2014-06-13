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
**|         Copyright (c) 2003-2006 Texas Instruments Incorporated           |**
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
 * FILE NAME    :  usb20_config.h  
 *
 * DESCRIPTION  :  All driver configuration are defined here
 *
 * HISTORY 	:
 *
 *  Jul 30 - sabyasachi - Created.
 *
 *  Oct 18 - sabyasachi - Updated with VID/PID/Rx Buffer Size.
 *
 *  (C) Copyright 2003-2006, Texas Instruments, Inc.
 *************************************************************************/


/**
 * @defgroup Config_Params Configuration Parameters  
 *
 *@{
 * 
 * List of Configuration Parameters : 
 * 
 * - <b> USB_VID : </b> USB Vendor ID. This uniquely identifies a vendor of a USB 
 *   product. Texas Instruments has registered 0x451 as VID. 
 *
 * - <b> USB_PID : </b> USB Product ID. This uniquely identifies a product 
 *   among a line of products from same vendor.
 *   
 * - <b> USB_SERIAL_NO : </b> USB Serial Number. This uniquely identifies a 
 *   particular instance of device among many who have same VID and PID.
 *   
 * - <b> USB_MAX_POWER : </b> USB Max Power. Indicated maximum power which this 
 *   device may consume from the bus. The power consumption is (2 x this value) 
 *   in mA.
 *   
 * - <b> USB_BIG_ENDIAN : </b> To compile the driver for little endian system 
 *   undefine the following macro.
 *
 * - <b> USB_DEBUG : </b> To enable Driver Debug log messages define the 
 *   following macro.
 *
 * - <b> USB_ENABLE_TASK : </b> Enable USB ISR and Packet Receive Processing in 
 *   Task mode. Default State: Disabled.
 *
 * - <b> USB_ISR_TASK_NAME : </b> Name of USB ISR Task. This task is a high 
 *   priority task which processes all USB ISR actions. 
 *
 * - <b> USB_NET_TASK_NAME : </b> Name of USB Net Task. This task processes all 
 *   received packets over USB interface.
 *
 * - <b> USB_RNDIS_HOST_MAC_ADDR : </b> MAC Address for RNDIS PC Host. 
 *
 * - <b> USB_RNDIS_DEVICE_MAC_ADDR : </b> MAC Address for USB RNDIS Device. 
 *
 * - <b> V2U_VLYNQ_PORT : </b> Defines whether USB 2.0 RNDIS Driver will run 
 *   over Vlynq0 or Vlynq1. For Vlynq0 set the value to 0, for Vlynq1 set the 
 *   value to 1.
 *
 * <b> USB_DEV_PARAM </b> structure defines all necessary values to access USB 
 * registers. Elements of this structure :
 *
 * - usb_base : Base address of device register set
 * - usb_offset : Offset (to convert address from MIPS <-> Vlynq space  
 * - reset_base : Address of reset register
 * - reset_bit : Bit in the reset register for USB module reset
 * - clkc_base : Address of USB Clock Control Register
 * - clkc_mask : Mask for USB Clock Control register
 * - usb_irq : Vlynq IRQ Vector for USB Interrupt (See USB 2.0 Silicon Spec)
 * - vbus_irq : Vlynq IRQ Vector for VBUS Interrupt (See USB2.0 Silicon Spec)
 * - ucp_base : Address of USB Command Processor Control Register
 * - ucp_disable_val : Value to disable USB Command Processor
 * - ucp_disable_mask : Mask for USB Command Processor Control Register
 *
 * @}
 */

#ifndef __USB_CONFIG_H
#define __USB_CONFIG_H

#include "psp_types.h"


#define USB_VID   0x0451
#define USB_PID   0x6100
#define USB_SERIAL_NO   "TI USB 001"
#define USB_MANUFACTURER_STRING "Texas Instruments"
#define USB_PRODUCT_STRING  "TI USB RNDIS Adapter"
#define USB_MAX_POWER  50

#undef USB_BIG_ENDIAN 
#define USB_LITTLE_ENDIAN


#define USB20_DEBUG 
#undef USB_ENABLE_TASK 
#define USB_ISR_TASK_NAME   "tUsbISR"
#define USB_NET_TASK_NAME   "tUsbNet"

#define USB_RNDIS_HOST_MAC_ADDR         "00:e0:a6:66:49:f9"
#define USB_RNDIS_DEVICE_MAC_ADDR      "00:e0:a6:02:75:70"

/* Define this to enable MIB-II support */

#define USB_MIB2

#ifdef USB_MIB2
#include <asm/avalanche/generic/mibIoctl.h>          /*for MIB2 Support */
#endif


typedef struct USB_DEV_PARAM_T
{
    UINT32      usb_base; 
    UINT32      usb_offset;
    UINT32      reset_base;
    UINT32      reset_bit;
    UINT32      clkc_base;
    UINT32      clkc_mask;
    UINT32      usb_irq;
    UINT32      vbus_irq;
    UINT32      ucp_base;
    UINT32      ucp_disable_val;
    UINT32      ucp_disable_mask;
} USB_DEV_PARAM;



extern UINT32 usb20_base_address;
extern UINT32 usb20_usb_irq;
extern UINT32 usb20_vbus_irq;

/**
 * Config routines. Edit these routines to provide platform specific values
 * for USB driver.
 */
 
UINT16  usb20_config_get_vid(void);
UINT16  usb20_config_get_pid(void);
UINT16  usb20_config_get_max_power(void);
UINT32  usb20_config_get_host_mac_addr(unsigned char * addr);
UINT32  usb20_config_get_device_mac_addr(unsigned char * addr);
UINT32  usb20_config_get_serial_no(char *, unsigned int );
UINT32  usb20_config_get_product_string(char *ptr, unsigned int size);
UINT32  usb20_config_get_manufacturer_string(char *ptr, unsigned int size);
USB_DEV_PARAM  *usb20_config_get_dev_param(void);

#endif  /* ifndef __USB_CONFIG_H */

