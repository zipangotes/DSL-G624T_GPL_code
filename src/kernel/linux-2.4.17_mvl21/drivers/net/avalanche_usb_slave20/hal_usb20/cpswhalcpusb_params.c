/******************************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE:  cpswhalcpusb_params.c
 *
 *  DESCRIPTION:
 *      Contains all the code accessing the PARAMS information
 *
 *  HISTORY:
 *  9Jul03 Menaka Venkateswaran 1.00    Created from cpmac_params.c
 *****************************************************************************/
#define _CPHAL_CPUSB
#define _CPHAL
#define _CPSW_HAL

#include "cpswhalcpusb_private.h"
#define HALDEV_PARAMS_INIT   (SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE)


/* Place all these strings in cpswhalutlsupport.h later */
int ProcessUsbRegs(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value);

int ProcessUsbChBufferSize(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value);
int ProcessUsbRegDevAddr(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value);
int ProcessUsbRegDataDirCtrl(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value);
int ProcessUsbRegHighSpeedTestModes(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value);
int ProcessUsbRegInStall(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value);
int ProcessUsbRegRemoteWakeup(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value);
int ProcessUsbRegOutStall(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value);


/**  USB HAL Substructure definition
 *    Substructure 0 : HalDev
 *    Substructure 1 : ChannelInfoHal
 *    Substructure 2 : Stats
 *    Substructure 3 : TxCppiCtrlBlk
 *    Substructure 4 : RxCppiCtrlBlk
 *    Substructure 5 : ControlEp
 *    Substructure 6 : ChannelStats
 *
 *     GROUP definition
 *    GROUP 1:  Stats
 *    GROUP 2:  Channel Stats
 *    GROUP 3:  CPPI Ctrl Blk
 *    GROUP 4:  Channel Info
 *    GROUP 5:  HalDev parameters to be pushed before Open
 *    GROUP 6:  Control EP 
 *   
 *
 */

#define INV_INIT 0xffffffff
static const DEVICE_PARAMS CpusbParams[] = 
{

  /* Register bits after Open */
  /* PASS_THROUGH for NOW, handle in ProcessUsbRegs */
  {hkUsbDirCtrl,               PASS_THROUGH,0,0,0,1,ProcessUsbRegDataDirCtrl},
  {hkInStall ,                 PASS_THROUGH,0,0,0,1,ProcessUsbRegInStall},
  {hkOutStall,                 PASS_THROUGH,0,0,0,1,ProcessUsbRegOutStall},
  {hkUsbDevAddr,               SUBSTRUCT(0)|GROUP5|DO_INIT|WRITEALL,offsetof(HAL_DEVICE,UsbDevAddr),0,0,255,ProcessUsbRegDevAddr},
  {hkUsbTestMode,              PASS_THROUGH,0,0,0,1,ProcessUsbRegHighSpeedTestModes},  
  {hkUsbRemWakeup,             PASS_THROUGH,0,0,0,1,ProcessUsbRegRemoteWakeup},
  {hkRxCrcErrors,              PASS_THROUGH,0,0,0,0,ProcessUsbRegs},
  {hkWrongAckErrors,           PASS_THROUGH,0,0,0,0,ProcessUsbRegs},
  {hkRxErrors,                 PASS_THROUGH,0,0,0,0,ProcessUsbRegs},
  {hkAckErrors,                PASS_THROUGH,0,0,0,0,ProcessUsbRegs},

#ifdef _CPHAL_STATS 
  /* stats (stat group 1) */
  {hkNumSuspInt,              SUBSTRUCT(0)|GROUP1|WRITEALL,offsetof(HAL_DEVICE,NumSuspInt ),0,0,0xffffffff,0},
  {hkNumResrInt,              SUBSTRUCT(0)|GROUP1|WRITEALL,offsetof(HAL_DEVICE,NumResrInt),0,0,0xffffffff,0},
  {hkNumSetupInt,             SUBSTRUCT(0)|GROUP1|WRITEALL,offsetof(HAL_DEVICE,NumSetupInt ),0,0,0xffffffff,0},
  {hkNumSetupOwInt,           SUBSTRUCT(0)|GROUP1|WRITEALL,offsetof(HAL_DEVICE,NumSetupOwInt ),0,0,0xffffffff,0},
  {hkNumIEP0Int,              SUBSTRUCT(0)|GROUP1|WRITEALL,offsetof(HAL_DEVICE,NumIEP0Int),0,0,0xffffffff,0},
  {hkNumVbusInt,              SUBSTRUCT(0)|GROUP1|WRITEALL,offsetof(HAL_DEVICE,NumVbusInt),0,0,0xffffffff,0},
  {hkNumRstrInt,              SUBSTRUCT(0)|GROUP1|WRITEALL,offsetof(HAL_DEVICE,NumRstrInt),0,0,0xffffffff,0},
  {hkNumOEP0Int,              SUBSTRUCT(0)|GROUP1|WRITEALL,offsetof(HAL_DEVICE,NumOEP0Int),0,0,0xffffffff,0},

  /* CPPI S/W Channel statistics (stat group 2) */
  {hkTxMisQCnt,               SUBSTRUCT(2)|GROUP2|WRITEALL,offsetof(cppi_txcntl_s,TxMisQCnt),0,0,0xffffffff,0},
  {hkRxMisQCnt,               SUBSTRUCT(3)|GROUP2|WRITEALL,offsetof(cppi_rxcntl_s,RxMisQCnt),0,0,0xffffffff,0},
  {hkTxEOQCnt,                SUBSTRUCT(2)|GROUP2|WRITEALL,offsetof(cppi_txcntl_s,TxEOQCnt),0,0,0xffffffff,0},
  {hkRxEOQCnt,                SUBSTRUCT(3)|GROUP2|WRITEALL,offsetof(cppi_rxcntl_s,RxEOQCnt),0,0,0xffffffff,0},
  {hkRxMaxServiced,           SUBSTRUCT(3)|GROUP2|WRITEALL,offsetof(cppi_rxcntl_s,RxMaxServiced),0,0,0xffffffff,0},
  {hkTxMaxServiced,           SUBSTRUCT(2)|GROUP2|WRITEALL,offsetof(cppi_txcntl_s,TxMaxServiced),0,0,0xffffffff,0},
  {hkRxPacketsServiced,       SUBSTRUCT(3)|GROUP2|WRITEALL,offsetof(cppi_rxcntl_s,RxPacketsServiced),0,0,0xffffffff,0},
  {hkTxPacketsServiced,       SUBSTRUCT(2)|GROUP2|WRITEALL,offsetof(cppi_txcntl_s,TxPacketsServiced),0,0,0xffffffff,0},
  {hkNumRxInt,                SUBSTRUCT(3)|GROUP2|WRITEALL,offsetof(cppi_rxcntl_s,NumRxInt),0,0,0xffffffff,0},
  {hkNumTxInt,                SUBSTRUCT(2)|GROUP2|WRITEALL,offsetof(cppi_txcntl_s,NumTxInt),0,0,0xffffffff,0},
  {hkRxTotal,                 SUBSTRUCT(0)|GROUP2|WRITEALL,offsetof(HAL_DEVICE,RxTotal),0,0,0xffffffff,0},
  {hkTxTotal,                 SUBSTRUCT(0)|GROUP2|WRITEALL,offsetof(HAL_DEVICE,TxTotal),0,0,0xffffffff,0},

  /* CPPI S/W debug stats (stat group 3) */
  {hkTcbPool,                 SUBSTRUCT(2)|GROUP3|DO_INIT,offsetof(cppi_txcntl_s, TcbPool),0,0,0,0},
  {hkTxActQueueCount,         SUBSTRUCT(2)|GROUP3|DO_INIT,offsetof(cppi_txcntl_s, TxActQueueCount),0,0,0,0},
  {hkTxActQueueHead,          SUBSTRUCT(2)|GROUP3|DO_INIT,offsetof(cppi_txcntl_s, TxActQueueHead),0,0,0,0},
  {hkTxActQueueTail,          SUBSTRUCT(2)|GROUP3|DO_INIT,offsetof(cppi_txcntl_s, TxActQueueTail),0,0,0,0},
  {hkTxActive,                SUBSTRUCT(2)|GROUP3|DO_INIT,offsetof(cppi_txcntl_s, TxActive),0,0,0,0},
  {hkTcbStart,                SUBSTRUCT(2)|GROUP3|DO_INIT,offsetof(cppi_txcntl_s, TcbStart),0,0,0,0},
  {hkRcbPool,                 SUBSTRUCT(3)|GROUP3|DO_INIT,offsetof(cppi_rxcntl_s, RcbPool),0,0,0,0},
  {hkRxActQueueCount,         SUBSTRUCT(3)|GROUP3|DO_INIT,offsetof(cppi_rxcntl_s, RxActQueueCount),0,0,0,0},
  {hkRxActQueueHead,          SUBSTRUCT(3)|GROUP3|DO_INIT,offsetof(cppi_rxcntl_s, RxActQueueHead),0,0,0,0},
  {hkRxActQueueTail,          SUBSTRUCT(3)|GROUP3|DO_INIT,offsetof(cppi_rxcntl_s, RxActQueueTail),0,0,0,0},
  {hkRxActive,                SUBSTRUCT(3)|GROUP3|DO_INIT,offsetof(cppi_rxcntl_s, RxActive),0,0,0,0},
  {hkRcbStart,                SUBSTRUCT(3)|GROUP3|DO_INIT,offsetof(cppi_rxcntl_s, RcbStart),0,0,0,0},
#endif 

  {hkRxServiceMax,            SUBSTRUCT(3)|GROUP3|WRITEALL|DO_INIT|WRITEALL,offsetof(cppi_rxcntl_s, RxServiceMax),20,0,0xFFF,0},
  {hkTxServiceMax,            SUBSTRUCT(2)|GROUP3|WRITEALL|DO_INIT|WRITEALL,offsetof(cppi_txcntl_s, TxServiceMax),20,0,0xFFF,0},

  /* channel parameters (stat group 4) */
  {hkRxBufSize,               SUBSTRUCT(1)|GROUP4|DO_INIT|WRITABLE,offsetof(CHANNEL_INFO_HAL,RxBufSize),1564,0,0xffffffff,0},
  {hkRxBufferOffset,          SUBSTRUCT(1)|GROUP4|DO_INIT|WRITABLE,offsetof(CHANNEL_INFO_HAL,RxBufferOffset),0,0,65534,0},
  {hkRxNumBuffers,            SUBSTRUCT(1)|GROUP4|DO_INIT|WRITABLE,offsetof(CHANNEL_INFO_HAL,RxNumBuffers),128,0,2048,0},
  {hkTxNumBuffers,            SUBSTRUCT(1)|GROUP4|DO_INIT|WRITABLE,offsetof(CHANNEL_INFO_HAL,TxNumBuffers),128,0,2048,0},
  /*  {hkTxNumQueues,             SUBSTRUCT(1)|GROUP4|DO_INIT, offsetof(CHANNEL_INFO_HAL,TxNumQueues),1,1,1,0},*/
  {hkOsSetup,                 SUBSTRUCT(1)|GROUP4|DO_INIT|WRITEALL,offsetof(CHANNEL_INFO_HAL,OsSetup),0,0,0xffffffff,0},
  {hkINEpBuffSize,            SUBSTRUCT(1)|GROUP4|DO_INIT|WRITEALL,offsetof(CHANNEL_INFO_HAL,INEpBuffSize),512,0,512,ProcessUsbChBufferSize},
  {hkINEpDbuf,                SUBSTRUCT(1)|GROUP4|DO_INIT|WRITEALL,offsetof(CHANNEL_INFO_HAL,INEpDbuf),1,0,1,0},
  {hkOUTEpBuffSize,           SUBSTRUCT(1)|GROUP4|DO_INIT|WRITEALL,offsetof(CHANNEL_INFO_HAL,OUTEpBuffSize),512,0,512,ProcessUsbChBufferSize},
  {hkOUTEpDbuf,               SUBSTRUCT(1)|GROUP4|DO_INIT|WRITEALL,offsetof(CHANNEL_INFO_HAL,OUTEpDbuf),1,0,1,0},
  {hkINEpBuffBaseAddrX,       SUBSTRUCT(1)|GROUP4|DO_INIT,offsetof(CHANNEL_INFO_HAL,INEpBuffBaseAddrX),0,0,0,0},
  {hkINEpBuffBaseAddrY,       SUBSTRUCT(1)|GROUP4|DO_INIT,offsetof(CHANNEL_INFO_HAL,INEpBuffBaseAddrY),0,0,0,0},
  {hkOUTEpBuffBaseAddrX,      SUBSTRUCT(1)|GROUP4|DO_INIT,offsetof(CHANNEL_INFO_HAL,OUTEpBuffBaseAddrX),0,0,0,0},
  {hkOUTEpBuffBaseAddrY,      SUBSTRUCT(1)|GROUP4|DO_INIT,offsetof(CHANNEL_INFO_HAL,OUTEpBuffBaseAddrY),0,0,0,0},

 
  /* primary parameters (group 0)
   *  These parameters must be pushed before open
  */
  {hkBase,                    SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE|ISBASE,offsetof(HAL_DEVICE,dev_base),INV_INIT,0,0xffc00000,0},
  {hkOffset,                  SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,offset),0,0,0xffffffff,0},
  {hkInst,                    SUBSTRUCT(0)|GROUP5,offsetof(HAL_DEVICE, Inst),0,0,100,0},
  {hkDebug,                   SUBSTRUCT(0)|GROUP5|DO_INIT|WRITEALL,offsetof(HAL_DEVICE,debug),0,0,0xffffffff,0},
  {hkId,                      SUBSTRUCT(0)|GROUP5|DO_INIT|ISSTRING,offsetof(HAL_DEVICE,id),(bit32u)hkCpusbModuleName, 0, 50,0},
  {hkVersion,                 SUBSTRUCT(0)|GROUP5|DO_INIT|ISSTRING,offsetof(HAL_DEVICE,Version),(bit32u)hkVersion_CPUSB, 0, 50,0},  
  {hkVersionCppi,             SUBSTRUCT(0)|GROUP5|DO_INIT|ISSTRING,offsetof(HAL_DEVICE,VersionCppi),(bit32u)hkVersion_CPPI_cpswHalCpusb, 0,50,0},
  {hkCpuFrequency,            SUBSTRUCT(0)|ISENV|WRITABLE|DO_INIT,offsetof(HAL_DEVICE, CpuFrequency),50000000,0,1000000000,0},
  
  /* USB primary parameters (stat group 5 ?)*/
  {hkUsbIntBit,               SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,intr_usb),INV_INIT,0,0xffff0000|32,0},
  {hkVbusIntBit,              SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,intr_vbus),INV_INIT,0,0xffff0000|32,0},
  {hkRemWakeupIntBit,         SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,intr_remwakeup),INV_INIT,0,0xffff0000|32,0},
 
  {hkModeSelect,              SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,ModeSelect),0,0,1,0},
  {hkLatencyTest,             SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,LatencyTest),0,0,1,0},
  {hkFullSpeed,               SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,FullSpeed),0,0,1,0},
  {hkUsbIntrMask,             SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,UsbIntrMask),0,0,0xffffffff,0},
  {hkChEndianess,             SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,ChEndianess),0,0,0xf,0},
  {hkRemWakeupIntrEn,         SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,RemWakeupIntrEn),0,0,0xffffffff,0},

  /* Parameters returned thru OsControl after Open   */
  {hkUcpPresent,              SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,UcpPresent),0,0,1,0},
  {hkUcpRegBase,              SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE|ISBASE,offsetof(HAL_DEVICE,UcpRegBase),0,0,0xffffffff,0},
  {hkUcpDisVal,               SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,UcpDisVal),0x0,0,0xffffffff,0},
  {hkUcpDisClrMask,           SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,UcpDisClrMask),0,0,0xffffffff,0},
  {hkResetBit,                SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,ResetBit),0,0,0xffffffff,0},
  {hkResetBase,               SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE|ISBASE,offsetof(HAL_DEVICE,ResetBase),0,0,0xffffffff,0},
  {hkClkRegBase,              SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE|ISBASE,offsetof(HAL_DEVICE,ClkRegBase),0,0,0xffffffff,0},
  {hkClkMask,                 SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,ClkMask),0,0,0xffffffff,0},
  {hkRemWakeup,               SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,RemWakeup),0,0,1,0},
  {hkRemWakeupRegBase,        SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE|ISBASE,offsetof(HAL_DEVICE,RemWakeupRegBase),0,0,0xffffffff,0},
  {hkRemWakeupEventMask,      SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,RemWakeupEventMask),0,0,0xffffffff,0},
  {hkRemWakeupEventClrMask,   SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,RemWakeupEventClrMask),0,0,0xffffffff,0},
  {hkLowPwrMode,              SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,LowPwrMode) ,0,0,1,0},
  {hkLowPwrRegBase,           SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE|ISBASE,offsetof(HAL_DEVICE,LowPwrRegBase),0,0,0xffffffff,0},
  {hkLowPwrModeMask,          SUBSTRUCT(0)|GROUP5|DO_INIT|WRITABLE,offsetof(HAL_DEVICE,LowPwrModeMask),0,0,0xffffffff,0},
  {hkAvailBuffRamSize,        SUBSTRUCT(0)|GROUP5|DO_INIT,offsetof(HAL_DEVICE,AvailBuffRamSize),0,0,USB20_USB_BUFFRAM_SIZE,0},
  {hkRxMaxFrags,              SUBSTRUCT(0)|GROUP5|DO_INIT|WRITEALL,offsetof(HAL_DEVICE,MaxFrags),2,0,0x100,0},
  {hkRxMaxChan,               SUBSTRUCT(0)|GROUP5|DO_INIT|WRITEALL,offsetof(HAL_DEVICE,RxMaxChan),4,0,4,0},
  {hkTxMaxChan,               SUBSTRUCT(0)|GROUP5|DO_INIT|WRITEALL,offsetof(HAL_DEVICE,TxMaxChan),4,0,4,0},

  /* channel parameter for control channel (stats group 5) */
  {hkCtrlEPBuffSize,          SUBSTRUCT(4)|GROUP6|DO_INIT|WRITABLE,offsetof(CONTROL_EP,CtrlEPBuffSize),0x40,8,0x40,0},
  {hkCtrlEPDblBuff,           SUBSTRUCT(4)|GROUP6|DO_INIT|WRITABLE,offsetof(CONTROL_EP,CtrlEPDblBuff),0,0,1,0},
  {hkInUSBIE,                 SUBSTRUCT(4)|GROUP6|DO_INIT|WRITABLE,offsetof(CONTROL_EP,InUSBIE),1,0,1,0},
  {hkOutUSBIE,                SUBSTRUCT(4)|GROUP6|DO_INIT|WRITABLE,offsetof(CONTROL_EP,OutUSBIE),1,0,1,0},

  /* end of array */
  {0,0,0,0,0,0,0}
};  


static int IsChOpen(HAL_DEVICE *HalDev, int Ch, int Dir)
  {
   return (((HAL_DEVICE *)HalDev)->ChIsOpen[Ch][Dir]);
  }

static int IsControlEPOpen(HAL_DEVICE *HalDev,int unused1, int unused2 )
  {
   return (((HAL_DEVICE *)HalDev)->CtrlEpIsOpen);

  }

static int IsHalOpen(HAL_DEVICE *HalDev, int unused1, int unused2)
  {
   int rc = 0;

   if (((HAL_DEVICE *)HalDev)->State == enOpened)
     rc = 1;

   return (rc);
  }  



/**
 *  @ingroup CPHAL_Functions
 *  Performs a variety of control functions on the CPHAL module.  It is used to
 *  modify/read configuration parameters and to initiate internal functions.
 *  The @p Key indicates the function to perform or the parameter to access (note 
 *  that these Keys are identical to those used in accessing the configuration data
 *  store).  @p Action is applicable to parameters only, and indicates what the
 *  CPHAL should do with the parameter (i.e. "Set", "Get", etc..).  The actions
 *  for each parameter are defined in the module specific documentation.
 *  
 *  @param   HalDev      CPHAL module instance. (set by xxxInitModule())
 *  @param   Key         Key specifying the parameter to change or internal function to initiate.  See module specific documentation for available keys.
 *  @param   Action      Specifies the action to take.  See module specific documentation for available actions.
 *  @param   Value       Pointer to new value for given @p Key parameter ("Set"), or returned value of Key ("Get").
 *  
 *  @return  0 OK, Otherwise error.
 */

int cpswHalCpusbControl(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value)
  {
   int rc;
   
#ifdef _CPHAL_STATS 
 if (cpswHalCommonstrstr(Key, hkStats) != 0)
     {
      if (Action == haGETbyREF)
        {
         int Level, Ch, Queue;
         char *TmpKey = (char *)Key;

         TmpKey += cpswHalCommonstrlen(hkStats) + 1;
         Level = cpswHalCommonstrtoul(TmpKey, &TmpKey, 10);
         TmpKey++;
         Ch = cpswHalCommonstrtoul(TmpKey, &TmpKey, 10);
         TmpKey++;
         Queue = cpswHalCommonstrtoul(TmpKey, &TmpKey, 10);
         TmpKey++;
         return (cpswHalCommonStatsGet(HalDev->OsFunc, CpusbParams, HalDev->SubStructs, (void **)Value, Level, Ch, Queue));
        }
      if (Action == haCLEAR)
        return (cpusbStatsClear(HalDev));
     }
#endif 

   rc = cpswHalCommonParamsControl(HalDev,HalDev->OsFunc, CpusbParams,HalDev->SubStructs,Key,Action,Value);
   if (rc) 
     {
      return(EC_MODULE|rc); 
     }
   return (EC_NO_ERRORS);
  }

 int cpusbInitParams(HAL_DEVICE *HalDev)
  {
    /*Setup the substruct */
   HalDev->SubStructs[0].sub_struct = (void *)HalDev;
   HalDev->SubStructs[0].sub_struct_size = 1;
   HalDev->SubStructs[0].sub_index_size1 = 1;
   HalDev->SubStructs[0].sub_index_size2 = 1;
   HalDev->SubStructs[0].IsOpen = IsHalOpen;

   /*Setup the substruct */
   HalDev->SubStructs[1].sub_struct = (void *)&(HalDev->ChData[0]);
   HalDev->SubStructs[1].sub_struct_size = sizeof (CHANNEL_INFO_HAL);
   HalDev->SubStructs[1].sub_index_size1 = NUM_USB_CHAN;
   HalDev->SubStructs[1].sub_index_size2 = 1;
   HalDev->SubStructs[1].IsOpen = IsChOpen;
 
   /*Setup the substruct */
   HalDev->SubStructs[2].sub_struct = (void *)&(HalDev->TxCtlBlk[0]);
   HalDev->SubStructs[2].sub_struct_size = sizeof (cppi_txcntl_s);
   HalDev->SubStructs[2].sub_index_size1 = MAX_CHAN;
   HalDev->SubStructs[2].sub_index_size2 = MAX_QUEUE;
   HalDev->SubStructs[2].IsOpen = IsHalOpen; 

   /*Setup the substruct*/
   HalDev->SubStructs[3].sub_struct = (void *)&(HalDev->RxCtlBlk[0]);
   HalDev->SubStructs[3].sub_struct_size = sizeof (cppi_rxcntl_s);
   HalDev->SubStructs[3].sub_index_size1 = MAX_CHAN;
   HalDev->SubStructs[3].sub_index_size2 = 1;
   HalDev->SubStructs[3].IsOpen = IsHalOpen; 

   /* Setup the substruct */
   HalDev->SubStructs[4].sub_struct = (void *)&(HalDev->ControlData);
   HalDev->SubStructs[4].sub_struct_size = sizeof (CONTROL_EP);
   HalDev->SubStructs[4].sub_index_size1 = 1;
   HalDev->SubStructs[4].sub_index_size2 = 1;
   HalDev->SubStructs[4].IsOpen = IsControlEPOpen; 

   return(cpswHalCommonParamsInit(HalDev,CpusbParams,HalDev->SubStructs));
  }

 int cpusbCheckAllParams(HAL_DEVICE *HalDev)
  {
   return(cpswHalCommonParamsCheckAllParams(HalDev,CpusbParams, HalDev->SubStructs));
  }

#ifdef _CPHAL_STATS 
/*
 *  @ingroup CPHAL_Functions
 *  Clears the statistics information.
 *
 *  @param  HalDev   CPHAL module instance. (set by xxxInitModule())
 *
 *  @return 0 OK, Non-zero not OK
 */
 int cpusbStatsClear(HAL_DEVICE *HalDev)
  { 
   int i;
   cppi_rxcntl_s *pRxCtl;
   cppi_txcntl_s *pTxCtl;

   /* clear stats */
   for (i=0; i< NUM_USB_CHAN; i++)
     {
      pRxCtl = &HalDev->RxCtlBlk[i];
      pTxCtl = &HalDev->TxCtlBlk[i][0];

      pTxCtl->TxMisQCnt=0;
      pTxCtl->TxEOQCnt=0;  
      pTxCtl->NumTxInt=0;
      pTxCtl->TxPacketsServiced=0;
      pTxCtl->TxMaxServiced=0;

      pRxCtl->RxMisQCnt=0;
      pRxCtl->RxEOQCnt=0;
      pRxCtl->NumRxInt=0;
      pRxCtl->RxPacketsServiced=0;
      pRxCtl->RxMaxServiced=0;
     }
   HalDev->RxTotal=0;
   HalDev->TxTotal=0;
   HalDev->NumSuspInt=0;
   HalDev->NumResrInt=0;
   HalDev->NumRstrInt=0;
   HalDev->NumSetupInt=0;
   HalDev->NumSetupOwInt=0;
   HalDev->NumIEP0Int=0;
   HalDev->NumVbusInt=0;

   return (EC_NO_ERRORS);
  }
#endif 

int ProcessUsbChBufferSize(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value)
{

   char *TmpKey = (char *)Key;
   int Index;
   bit32u tmpBuff;

  if (cpswHalCommonstrstr(Key,hkOUTEpBuffSize))
  {
      /* extract channel number */
      TmpKey += cpswHalCommonstrlen(hkOUTEpBuffSize) + 1;
      Index = cpswHalCommonstrtoul(TmpKey, &TmpKey, 10);
      if ( IsChOpen(HalDev,Index,DIRECTION_RX))
      {
         tmpBuff = USB20_EP_CFG(HalDev->dev_base,Index);
         tmpBuff &= ~(0xff000000);
         tmpBuff |= (HalDev->ChData[Index].OUTEpBuffSize>>3)<<24;
         USB20_EP_CFG(HalDev->dev_base,Index)= tmpBuff;
      }
  }

  if (cpswHalCommonstrstr(Key,hkINEpBuffSize))
  {
      /* extract channel number */
      TmpKey += cpswHalCommonstrlen(hkINEpBuffSize) + 1;
      Index = cpswHalCommonstrtoul(TmpKey, &TmpKey, 10);
      if ( IsChOpen(HalDev,Index,DIRECTION_TX))
      {
         tmpBuff = USB20_EP_CFG(HalDev->dev_base,Index);
         tmpBuff &= ~(0x00ff0000);
         tmpBuff |= (HalDev->ChData[Index].INEpBuffSize>>3)<<16;
         USB20_EP_CFG(HalDev->dev_base,Index)= tmpBuff;
      }
  }

   return EC_NO_ERRORS;
}

int ProcessUsbRegDevAddr(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value)
{
  register bit32u tmp;

  if ( IsHalOpen(HalDev,0,0) )
  {
     tmp =  USB20_USB_CTRL(HalDev->dev_base);
     tmp &= ~(0x7f0000);
     tmp |= (HalDev->UsbDevAddr << 16);
     USB20_USB_CTRL(HalDev->dev_base) = tmp;
   }
   else
     return EC_MODULE|EC_VAL_INVALID_STATE;
   return EC_NO_ERRORS;
}

int ProcessUsbRegDataDirCtrl(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value)
{
    /* Value represents the direction of data transfer
       in a USB setup transaction */
  
   if ( !IsHalOpen(HalDev,0,0) )
        return EC_MODULE|EC_VAL_INVALID_STATE;

   if ( Action == haSETbyVAL )
   {
     if ((bit32u)Value == HC_USB_DATADIR_OUT )
     {
       USB20_USB_CTRL(HalDev->dev_base) &= ~0x1 ;
     }
     else
     {
       USB20_USB_CTRL(HalDev->dev_base) |= 0x1;
     }
   }
   else
     return EC_MODULE|EC_VAL_PARAMS_INVALID_ACTION;
 
   return EC_NO_ERRORS;
}

int ProcessUsbRegHighSpeedTestModes(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value)
{
    register int tmp;

    if ( !IsHalOpen(HalDev,0,0) )
      return EC_MODULE|EC_VAL_INVALID_STATE;

    if  (Action == haSETbyVAL )
    {
      tmp =  USB20_USB_CTRL(HalDev->dev_base);
      tmp &= ~(0xe);
      tmp |= ((bit32u)Value) << 1;
      USB20_USB_CTRL(HalDev->dev_base) = tmp;
    }
    else
       return EC_MODULE|EC_VAL_PARAMS_INVALID_ACTION;

    return EC_NO_ERRORS;
}

int ProcessUsbRegInStall(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value)
{
  if ( !IsHalOpen(HalDev,0,0) )
     return EC_MODULE|EC_VAL_INVALID_STATE;

  if ( Action == haCLEAR )
  {
    /* Value represents the endpoint we wish to stall */
    if ( *(bit32u*)Value == 0 )
      USB20_USB_EP0_CFG(HalDev->dev_base) &= ~0x8; 
    else
      USB20_EP_CFG(HalDev->dev_base, (*(bit32u *)Value - 1)) &= ~0x8;

  }
  else
  {
    if ( (bit32u)Value == 0 )
      USB20_USB_EP0_CFG(HalDev->dev_base) |= 0x8; 
    else
       USB20_EP_CFG(HalDev->dev_base, ((bit32u)Value - 1)) |= 0x8;
  }



   return EC_NO_ERRORS;
}

int ProcessUsbRegRemoteWakeup(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value)
{
   if ( !IsHalOpen(HalDev,0,0) )
        return EC_MODULE|EC_VAL_INVALID_STATE;

    if ( Action != haSETbyVAL || Action != haCLEAR )
       return EC_MODULE|EC_VAL_PARAMS_INVALID_ACTION;

    if  (Action == haSETbyVAL )
    {
      USB20_USB_CTRL(HalDev->dev_base) |= RWUPEN;
    }
    else
    if ( Action == haCLEAR )
      USB20_USB_CTRL(HalDev->dev_base) &= ~RWUPEN;

    return EC_NO_ERRORS;

}

int ProcessUsbRegOutStall(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value)
{
   if ( !IsHalOpen(HalDev,0,0) )
        return EC_MODULE|EC_VAL_INVALID_STATE;

  if ( Action == haCLEAR )
  {

    /* Value represents the endpoint we wish to stall */
    if ( *(bit32u*)Value == 0 )
      USB20_USB_EP0_CFG(HalDev->dev_base) &= ~0x80000; 
    else
      USB20_EP_CFG(HalDev->dev_base, (*(bit32u *)Value - 1)) &= ~0x0800;

  }
  else
  {
  
    if ( (bit32u)Value == 0 )
      USB20_USB_EP0_CFG(HalDev->dev_base) |= 0x80000; 
    else
       USB20_EP_CFG(HalDev->dev_base, ((bit32u)Value - 1)) |= 0x0800;

  }

  return EC_NO_ERRORS;
}

int ProcessUsbRegs(HAL_DEVICE *HalDev, HAL_CONTROL_KEY *Key, HAL_CONTROL_ACTION Action, void *Value)
{

  /* Read only bits of the ERROR register */
  if (cpswHalCommonstrcmpi(Key, hkRxCrcErrors) == 0)
  {
    *(bit32u*)Value = (USB20_USB_IF_ERR(HalDev->dev_base) >> RX_CRC_ERRORS_SFT) &  0xff ;
    return EC_NO_ERRORS;
  }

  if (cpswHalCommonstrcmpi(Key, hkWrongAckErrors) == 0)
  {
    *(bit32u*)Value = (USB20_USB_IF_ERR(HalDev->dev_base) >> WRONG_ACK_ERRORS_SFT) &  0xff ;
    return EC_NO_ERRORS;
  }

  if (cpswHalCommonstrcmpi(Key, hkAckErrors) == 0)
  {
    *(bit32u*)Value = (USB20_USB_IF_ERR(HalDev->dev_base) >> ACK_ERRORS_SFT) &  0xff ;
    return EC_NO_ERRORS;
  }

  if (cpswHalCommonstrcmpi(Key, hkRxErrors) == 0)
  {
    *(bit32u*)Value = (USB20_USB_IF_ERR(HalDev->dev_base) >> RX_ERRORCNT_SFT) &  0xff ;
    return EC_NO_ERRORS;
  }

   return (EC_NO_ERRORS);
}

