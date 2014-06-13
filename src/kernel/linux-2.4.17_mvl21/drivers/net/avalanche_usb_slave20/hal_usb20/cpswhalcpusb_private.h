/****************************************************************************
        TNETD73xx Software Support
        Copyright(c) 2003, Texas Instruments Incorporated. All Rights Reserved.

        FILE: cpswhalcpusb_private.h   Register definitions for the CPUSB module

         DESCRIPTION:
                This include file contains private structure definitions for the
                CPUSB module.

         HISTORY:
              10Oct03 MENAKA Created from cpswhalcpmac_private.h to suit cpusb module
*****************************************************************************/
#ifndef _INC_USB_PRIVATE_H
#define _INC_USB_PRIVATE_H

#define _CPPI_TEST                        /** @todo remove for release */
#define __CPHAL_CPPI_OFFSET               /* support use of offset */
#define _CPHAL       
#define _CPHAL_CPUSB           
#define _CPSW_HAL                         /* What is this */

/*  OS Data Structure definitions  */
typedef void OS_PRIVATE;
typedef void OS_DEVICE;
typedef void OS_SENDINFO;
typedef void OS_RECEIVEINFO;
typedef void OS_SETUP;

/*  HAL Data Structure definitions  */
typedef struct _phy_device PHY_DEVICE;    /* What is this */
typedef struct hal_device  HAL_DEVICE;
typedef struct hal_private HAL_PRIVATE;
typedef struct hal_private HAL_RECEIVEINFO;

#include "cpswhal_cpusb.h"
#include "cpswhalcpusb_regs.h"
#include <cpswhalcommon_support.h>
#include <cpswhalcommon_params.h>

#define FUNC_START  /*dbgPrintf(__FUNCTION__": Entered\n")*/
#define FUNC_STOP   /* dbgPrintf(__FUNCTION__": Completed\n")*/

/** \namespace CPUSB_Version
This documents version 02.00.00 of the CPUSB CPHAL.
*/

extern HAL_CONTROL_KEY hkVersion_CPPI_cpswHalCpusb[];

/* Default HAL values */
#define MAX_FRAG            2
#define MAX_CHAN            MAX_USB_CHAN
#define MAX_QUEUE           1
#define MAX_DIRECTION       2
#define MAX_CONFIGURATION   2              /* O-FULL SPEED, 2- HIGH SPEED */
#define CPUSB_QUEUE_NUM     0
#define CPPI_TX_NUM_BUFFERS 128
#define CPPI_TX_NUM_QUEUES  1
#define CPPI_TX_SERVICE_MAX 128
#define CPPI_RX_NUM_BUFFERS 128
#define CPPI_RX_NUM_QUEUES  1
#define CPPI_RX_SERVICE_MAX 128
#define CPPI_RX_BUF_SIZE    1564
#define CPPI_TX_BUF_SIZE    1564
#define MAX_NEEDS           128                                            
#define MAX_CPUSB_SUB_STRUCT  7

#define USB_IEP0_XNAK 0x00000080
#define USB_IEP0_YNAK 0x00008000
#define USB_OEP0_XNAK 0x00800000
#define USB_OEP0_YNAK 0x80000000

/* This structure is specific to the USB module and
   describes the USB control endpoint */
 typedef struct
 {
    int CtrlEPBuffSize;
    int CtrlEPDblBuff;
    int InUSBIE;
    int OutUSBIE;
    bit32u InEpBuffBaseAddrX;
    bit32u InEpBuffBaseAddrY;
    bit32u OutEpBuffBaseAddrX;
    bit32u OutEpBuffBaseAddrY;
 } CONTROL_EP;


typedef struct channel_info_hal_s
{
  /*************************************************************************
                         USB Protocol related information
  *************************************************************************/   
   bit32u INEpBuffSize;                       /* EP buffer size */
   bit32u INEpBuffBaseAddrX;                  /* EP base Address for buffer X */
   bit32u INEpBuffBaseAddrY;                  /* EP base Address for buffer X */
   bit32u OUTEpBuffSize;                      /* EP buffer size */
   bit32u OUTEpBuffBaseAddrX;                 /* EP base Address for buffer X */
   bit32u OUTEpBuffBaseAddrY;                 /* EP base Address for buffer X */
   int INEpDbuf;                              /* Enable Double buffering */
   int OUTEpDbuf;                             /* Enable Double buffering */
  
  /************************************************************************
                         Common across all HALS 
  **************************************************************************/
  OS_SETUP *OsSetup;                          /* Os Information from ChannelSetup*/
  int Channel;                                /* Channel number. */
  int Direction;                              /* Rx,Tx or both */
  int   (*Receive)(OS_DEVICE *OsDev,FRAGLIST *FragList,bit32u FragCount,
                 bit32u PacketSize,HAL_RECEIVEINFO *HalReceiveInfo, bit32u Mode);
  int   (*SendComplete)(OS_SENDINFO *OsSendInfo);

  /**********************************************************************************
                           CPPI DMA related information
  *********************************************************************************/
   int RxBufSize;                           /* Size (in bytes) for each Rx buffer. */
   int RxBufferOffset;                      /* Num of bytes to offset rx data from
                                               start of buffer (must be less
                                               than buffer size).  */
   int RxNumBuffers;                        /* Num of CPPI Rx buffer desc */
   int RxServiceMax;                        /* Max num of pkts serviced at one time*/
   int TxNumBuffers;                        /* Num of CPPI Tx buffer desc */
   int TxNumQueues;                         /* USB always has only one Queue*/
   int TxServiceMax;                        /* Max num of pkts serviced at one time*/
}CHANNEL_INFO_HAL;

typedef struct
  {
  bit32 HNext;      /*< Hardware's pointer to next buffer descriptor  */
  bit32 BufPtr;     /*< Pointer to the data buffer                    */
  bit32 Off_BLen;   /*< Contains buffer offset and buffer length      */
  bit32 mode;       /*< SOP, EOP, Ownership, EOQ, Teardown, Q Starv, Length */
  void *Next;
  void *OsInfo;
  void *Eop;
  int   (*SendComplete)(OS_SENDINFO *OsSendInfo);
  }HAL_TCB;

typedef volatile struct hal_private
  {
  bit32 HNext;      /*< Hardware's pointer to next buffer descriptor     */
  bit32 BufPtr;     /*< Pointer to the data buffer                       */
  bit32 Off_BLen;   /*< Contains buffer offset and buffer length         */
  bit32 mode;       /*< SOP, EOP, Ownership, EOQ, Teardown Complete bits */
  void *DatPtr;
  void *Next;
  void *OsInfo;
  HAL_DEVICE *HalDev;
  unsigned int Ch;
  void *Eop;
  bit32 FragCount;
  }HAL_RCB;

typedef struct
{
  HAL_TCB *TcbPool;
  HAL_TCB *TxActQueueHead;
  HAL_TCB *TxActQueueTail;
  bit32   TxActQueueCount;
  bit32   TxActive;
  int     TxServiceMax;
  HAL_TCB *LastTcbProcessed;
  char    *TcbStart;
#ifdef _CPHAL_STATS
  bit32   TxMisQCnt;
  bit32   TxEOQCnt;
  bit32   TxPacketsServiced;
  bit32   TxMaxServiced;
  bit32u  NumTxInt;
#endif  
}cppi_txcntl_s;


typedef struct
{
  HAL_RCB *RcbPool;
  HAL_RCB *RxActQueueHead;
  HAL_RCB *RxActQueueTail;
  bit32   RxActQueueCount;
  bit32   RxActive;
  bit32u  RxServiceMax;
  char    *RcbStart;
  OS_SETUP *OsSetup;
#ifdef _CPHAL_STATS
  bit32u  RxMisQCnt;
  bit32u  RxEOQCnt;
  bit32u  RxMaxServiced;
  bit32u  RxPacketsServiced;
  bit32u  NumRxInt;
#endif
}cppi_rxcntl_s;

/*  HAL Device Structure definition */
typedef struct hal_device
{


/************************************************************************/
/*                                                                      */
/*      Common HAL Variables                                            */
/*                                                                      */
/************************************************************************/
  OS_DEVICE *OsDev;
  OS_FUNCTIONS *OsFunc;
  DEVICE_STATE State;
  HAL_FUNCTIONS *HalFunc;
  HAL_FUNCTIONS_EXT *Ext;

  bit32u RxPassCrc;
  bit32u RxCaf;
  bit32u RxCef;
  bit32u RxBcast;
  bit32u RxBcastCh;

  bit32 TxTeardownPending[MAX_CHAN];
  bit32 RxTeardownPending[MAX_CHAN];
  bit32 ChIsOpen[MAX_CHAN][2];
  bit32 ChIsSetup[MAX_CHAN][2];
  FRAGLIST *fraglist;

  bit32u NeedsCount;                                              /*MJH+030409*/
  HAL_RECEIVEINFO *Needs[MAX_NEEDS];                              /*MJH+030409*/
  int MaxFrags;
  SUB_STRUCT SubStructs[MAX_CPUSB_SUB_STRUCT];

  cppi_txcntl_s TxCtlBlk[MAX_CHAN][MAX_QUEUE];
  cppi_rxcntl_s RxCtlBlk[MAX_CHAN];
  int RxMaxChan;
  int TxMaxChan;

  CHANNEL_INFO_HAL ChData[NUM_USB_CHAN]; 

  bit32u  dev_base;
  bit32u  offset;
  bit32u  Inst;
  bit32u  debug;
  char    *DeviceInfo;
  char    *id; 
  char    *Version;
  char    *VersionCppi;
  int     CpuFrequency;

  bit32u ResetBase;
  bit32u ResetBit;


  /***************************************************************/
  /*           CHANNEL STRUCTURES                                */
  /***************************************************************/
  /* USB specific */
  bit32 CtrlEpIsOpen;
  bit32 CtrlEpIsSetup; 
  CONTROL_EP  ControlData;

  /***************************************************************/
  /*      DEVICE PARAMETERS                                      */
  /***************************************************************/
  /* Interrupt numbers*/
  bit32 intr_usb;
  bit32u intr_vbus; 
  bit32u intr_remwakeup;

  /* Interrupt Enable Maskes */
  bit32u UsbIntrMask;
  bit32u VbusTxIntrEnMask;
  bit32u VbusRxIntrEnMask;
  bit32u RemWakeupIntrEn;            /* Not yet implemented */

  /* Device Modes */
  bit32u ModeSelect;               /* RnDis Mode vs Transparant */
  bit32u LatencyTest;              /* Enables Latency tesing    */
  bit32u FullSpeed;                /* Full Speed force enabled  */
  bit32u ChEndianess;              /* Enables big endian mode   */

  /* Registers and their masks */
  bit32u UcpPresent;               /* UCP module present indicater*/
  bit32u UcpRegBase;               /* UCP disable register base addr */
  bit32u UcpDisVal;                /* Value which disables the UCP */
  bit32u UcpDisClrMask;               /* Mask to clear the disable field in reg */

  bit32u RemWakeup;                /* Is remote wakeup feature enabled */
  bit32u RemWakeupRegBase;         /* Remote wakeup base register      */
  bit32u RemWakeupEventMask;       /* Mask to enable the wakeup events */
  bit32u RemWakeupEventClrMask;    /* Mask to clear the rem wakeup event */
  bit32u ClkRegBase;               /* Clock control register addr */
  bit32u ClkMask;                  /* Mask to enable the clock controller */

  bit32u LowPwrMode;               /* Is Low Pwr mode enabled */
  bit32u LowPwrRegBase;            /* Low power enable register base addr */
  bit32u LowPwrModeMask;           /* Mask to enable the low pwr mode */


  /* Register bits */
  bit32u UsbDevAddr;              /* Usb addr as set by the USB host controller */
  /* void *OsOpen; */

  /* Parameters returned thru OsControl after Open   */
  bit32u AvailBuffRamSize;
  bit32u LinkedUp;
  bit32u Speed;

/************************************************************************/
/*                                                                      */
/*      STATS                                                           */
/*                                                                      */
/************************************************************************/
#ifdef _CPHAL_STATS
  bit32u NumSuspInt;
  bit32u NumResrInt;
  bit32u NumRstrInt;
  bit32u NumSetupInt;
  bit32u NumSetupOwInt;
  bit32u NumIEP0Int;
  bit32u NumVbusInt;
  bit32u NumOEP0Int;
  bit32u RxTotal;
  bit32u TxTotal;
#endif
  }HALDEVICE;


/* function headers from cpswhalcpaal5_params.c */
int  cpswHalCpusbControl(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value);
int  cpusbStatsClear(HAL_DEVICE * HalDev);
int  cpusbInitParams(HAL_DEVICE *HalDev);
int  cpusbCheckAllParams(HAL_DEVICE *HalDev);
int  cpusbStatus(HAL_DEVICE *HalDev);
int  cpusbProcessStats(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value);

/* CPPI registers */
#define TRANSMIT_PORT_TEARDOWN_REG(base) (USB20_TXTRDN(base)) 
#define TX_DMA_STATE_HDP(base,ch,q)      (USB20_TXDMA_STWORD0(base,ch))
#define TX_DMA_STATE_CP(base,ch,q)       (USB20_TXDMA_COMPPTR(base,ch))
#define RX_DMA_STATE_HDP(base,ch)        (USB20_RXDMA_STWORD1(base,ch))
#define RX_DMA_STATE_CP(base,ch)         (USB20_RXDMA_COMPPTR(base,ch))
#define USB_FREE_BUFFER_COUNT(base,ch)   (USB20_RXCH_FREEBUFCNT(base,ch))

/* the following defines are not CPPI specific */

#define EC_MODULE                       (EC_DEV_CPUSB|EC_INSTANCE(HalDev->Inst))
#define HIGHEST_CHANNEL                 (MAX_CHAN-1)




#endif /* _INC_USB_PRIVATE_H */
