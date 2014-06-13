/************************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE: cpswhal_cpusb.h
 *
 *  DESCRIPTION:
 *      User include file, contains data definitions shared between the CPUSB HAL2.0
 *      and the upper-layer software.
 *
 *  HISTORY:
 *      Date      Modifier  Ver    Notes
 *      10Oct03   Menaka    1.0    Created for CPUSB 
 *
 *****************************************************************************/
#ifndef _INC_CPSWHAL_CPUSB_H
#define _INC_CPSWHAL_CPUSB_H

#include <cpswhal.h>
#include "cpswhalcpusb_global.h"
#include "cpswhalcpusb_ecerrors.h"


/*
- bit0 (LSB): CPHAL Function Trace
- b1 : OS Function call trace
- b2 : Critical section entry/exit
- b3 : Memory allocation/destruction
- b4 : Detailed information in Rx path
- b5 : Detailed information in Tx path
- b6 : Extended error information
- b7 : General info
*/

#define DBG_CPHAL_FUNC     (0x1)
#define DBG_OS_FUNC        (0x2)
#define DBG_CRIT_SECTION   (0x4)
#define DBG_MEM_MGMT       (0x8)
#define DBG_RX             (0x10)
#define DBG_TX             (0x20)
#define DBG_ERR_INFO       (0x40)
#define DBG_GENERAL        (0x80)

/* USB20 common global constants */
/* USB20 Channels */
#define NUM_USB_CHAN  4
#define MAX_USB_CHAN  4

/* Macros for control parameters*/
#define HC_BIG_ENDIAN         0xf
#define HC_LITTLE_ENDIAN      0x0

#define HC_USB_DATADIR_OUT    0x0
#define HC_USB_DATADIR_IN     0x1

#define HC_USB_MODE_RNDIS      0x1
#define HC_USB_MODE_DEFAULT    0x0

/* USB Interrupt Enables   */
#define HC_USB_SOF_IE       (1 << 30)
#define HC_USB_RSTR_IE      (1 << 29)
#define HC_USB_SUSR_IE      (1 << 28)
#define HC_USB_RESR_IE      (1 << 27)
#define HC_USB_VBUS_IE      (1 << 26)
#define HC_USB_SETUP_IE     (1 << 25)
#define HC_USB_STPOW_IE     (1 << 24)


/* CPPI Direction*/
#define DIRECTION_BOTH      0x2

/* USb high speed test modes - set packet */
#define TEST_J_MODE          0x1
#define TEST_K_MODE          0x2
#define TEST_SE0_NAK_MODE    0x3
#define TEST_PKT_MODE        0x4
#define TEST_FORCE_EN_MODE   0x5

/* Set in hardware */
#define HC_TEST_J_MODE          0x2
#define HC_TEST_K_MODE          0x3
#define HC_TEST_SE0_NAK_MODE    0x1
#define HC_TEST_PKT_MODE        0x4
#define HC_TEST_FORCE_EN_MODE   0x5


/* OsStateChanges */
#define CPUSB_OS_LINKEDUP   0x00020000
#define CPUSB_LINK_FULL_SPEED     0x30 
#define CPUSB_LINK_HIGH_SPEED        0 

#define CPUSB_OS_USB_INT    0x00010000
#define CPUSB_INT_STPOW            0x1  /* Setup packet overwrite interrupt */
#define CPUSB_INT_SETUP            0x2  /* Setup packet received interrupt */
#define CPUSB_INT_RESR             0x3  /* Reset resume interrupt */
#define CPUSB_INT_SUSR             0x4  /* Suspend resume interrupt */
#define CPUSB_INT_RSTR             0x5  /* Reset request interrupt */
#define CPUSB_INT_IEP0_ACK         0x6  /* Input endpoint 0 interrupt */
#define CPUSB_INT_VBUS_CONT        0x7  /* Connected VBUS interrupt */
#define CPUSB_INT_VBUS_DISCONT     0x8  /* Disconnected VBUS interrupt */
#define CPUSB_INT_OEP0_ACK         0x9  /* Output endpoint 0 interrupt */


typedef struct hal_functions_ext_s
{
  /* Control EP Function */
  int  (*CtrlEPReadX)    (HAL_DEVICE *HalDev, bit32u *len, bit32u *data);
  int  (*CtrlEPWriteX)   (HAL_DEVICE *HalDev, bit32u len,  bit32u *data);
  int  (*CtrlEPReadY)    (HAL_DEVICE *HalDev, bit32u *len, bit32u *data);
  int  (*CtrlEPWriteY)   (HAL_DEVICE *HalDev, bit32u len,  bit32u *data);
}hal_functions_ext;

typedef struct os_functions_ext_s
{

} os_functions_ext;




/* The channel info extension structure contains  USB protocol related information */
typedef struct channel_info_ext_s
{
  bit32u INEpBuffSize;             /* EP buffer size*/
  int    INEpDbuf;                 /* Enable Double buffering*/
  bit32u OUTEpBuffSize;            /* EP buffer size*/
  int    OUTEpDbuf;                /* Enable Double buffering*/
}channel_info_ext;



int halCpusbInitModule(HAL_DEVICE **HalDev, 
		 OS_DEVICE *OsDev, 
		 HAL_FUNCTIONS **HalFunc, 
		 OS_FUNCTIONS *OsFunc,
		 int OsFuncSize,
		 int *HalFuncSize,
		 int Inst);



#endif  /*  end of _INC_CPSWHAL_CPUSB_H  */
