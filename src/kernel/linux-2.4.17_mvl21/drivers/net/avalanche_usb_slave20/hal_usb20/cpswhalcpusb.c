/** @file***********************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  cpswhalcpusb.c
 *
 *  version
 *	22May03 Menaka  	   1.00  Original Version created.
  *	10Oct03 Menaka  	   2.00  Merged with HAL 2.0.
 *
 *  @author  Menaka Venkateswaran
 *  @date    22-May-2003
 *****************************************************************************/
#define _CPHAL_CPUSB
#define _CPHAL
#define _CPHAL_CPPI_OFFSET  /* support for offset */
#define _CPSW_HAL
/*#define _CPHAL_STATS*/

/**
@defgroup CPUSB_Functions Additional Functions for CPUSB Implementation
These functions are used only by the CPUSB module.
*/

/* include CPHAL header files */

#include "cpswhalcpusb_private.h"

#define static

/* CPPI Function definitions */
extern int  cpswHalCpusbChannelTeardown(HAL_DEVICE *HalDev, int Channel, int Mode);
extern int  cpswHalCpusbClose(HAL_DEVICE * HalDev, bit32 Mode) ;
extern int  cpswHalCpusbRxReturn(HAL_RECEIVEINFO * HalReceiveInfo, int StripFlag);
extern int  cpswHalCpusbSend(HAL_DEVICE * HalDev, FRAGLIST * FragList, int FragCount, int PacketSize, OS_SENDINFO * OsSendInfo, int(* SendComplete)(OS_SENDINFO *), bit32u Mode) ;  
extern int  cpswHalCpusbInitTcb        (HAL_DEVICE *HalDev, int Ch);
extern int  cpswHalCpusbInitRcb        (HAL_DEVICE *HalDev, int Ch);
     /* ISR definitions */
extern static int  halISR_VBUS(HAL_DEVICE *HalDev, int *MoreWork);
extern static int  halISR_USB(HAL_DEVICE *HalDev, int *MoreWork);
extern static int  halISR_RemWkup(HAL_DEVICE *HalDev, int *MoreWork);

/*
 *   Re-entrancy Issues
 *   In order to ensure successful re-entrancy certain sections of the
 *   CPHAL code will be bracketed as Critical.
 *   The OS will provide the function Os.CriticalSection(BOOL), which
 *   will be passed a TRUE to enter the Critical Section and FALSE to exit.
 */


/* Function definitions */


/* Function configuration definitions*/

static int  cpswHalCpusbOpen(HAL_DEVICE *HalDev);
static int  cpswHalCpusbShutdown(HAL_DEVICE *HalDev);
int         halCpusbInitModule( HAL_DEVICE **pHalDev, OS_DEVICE *OsDev, HAL_FUNCTIONS **pHalFunc, OS_FUNCTIONS *OsFunc, int OsFuncSize, int *HalFuncSize, int Inst);
static int  cpswHalCpusbTick( HAL_DEVICE *HalDev);

static int  ControlEPConfigApply(HAL_DEVICE *HalDev, CONTROL_EP *HalChn);
static int  ControlEPConfigInit(HAL_DEVICE *HalDev);
static int  cpswHalCpusbChannelConfigApply(HAL_DEVICE *HalDev, int Ch);
static int  cpswHalCpusbChannelSetup(HAL_DEVICE *HalDev, CHANNEL_INFO *HalChn, OS_SETUP *OsSetup);

static int  cpswHalCpusbPacketProcessEnd(HAL_DEVICE *HalDev);

/* New API Functions */
static int  cpswHalCpusbCtrlEPReadX    (HAL_DEVICE *HalDev, bit32u *len, bit32u *data);
static int  cpswHalCpusbCtrlEPWriteX   (HAL_DEVICE *HalDev, bit32u len, bit32u *data);
static int  cpswHalCpusbCtrlEPReadY    (HAL_DEVICE *HalDev, bit32u *len, bit32u *data);
static int  cpswHalCpusbCtrlEPWriteY   (HAL_DEVICE *HalDev, bit32u len, bit32u *data);

/* Internal functions */
#ifdef _CPHAL_DEBUG
static void dbgChannelConfigDump(HAL_DEVICE *HalDev, int Ch);
static void dbgConfigDump(HAL_DEVICE *HalDev);
#endif

/* Function declarations */
/** 
 *  @ingroup CPHAL_Functions
 *
 *  This function will:
 *  -# allocate a HalDev that will be used by the OS for future communications with the device
 *  -# save OsDev for use when calling OS functions
 *  -# allocate and populate HalFunc with the addresses of CPHAL functions.
 *  -# check OsFuncSize to see if it meets the minimum requirement.
 *  -# return the size of the HAL_FUNCTIONS structure through the HalFuncSize pointer.  The OS
 *     should check this value to ensure that the HAL meets its minimum requirement.
 *
 *  Version checking between the OS and the CPHAL is done using the OsFuncSize and 
 *  HalFuncSize.  Future versions of the CPHAL may add new functions to either
 *  HAL_FUNCTIONS or OS_FUNCTIONS, but will never remove functionality.  This enables
 *  both the HAL and OS to check the size of the function structure to ensure that
 *  the current OS and CPHAL are compatible.
 *
 *  Note:  This is the only function exported by a CPHAL module. 
 *
 *  Please refer to the section "@ref hal_init" for example code.
 *
 *  @param   HalDev  Pointer to pointer to CPHAL module information.  This will 
 *                   be used by the OS when communicating to this module via 
 *                   CPHAL. Allocated during the call.
 *  @param   OsDev   Pointer to OS device information.  This will be saved by
 *                   the CPHAL and returned to the OS when required.
 *  @param   HalFunc Pointer to pointer to structure containing function pointers for all CPHAL 
 *                   interfaces.  Allocated during the call.
 *  @param   OsFunc  Pointer to structure containing function pointers for all OS
 *                   provided interfaces.  Must be allocated by OS prior to call.
 *  @param   OsFuncSize  Size of OS_FUNCTIONS structure.
 *  @param   HalFuncSize    Pointer to the size of the HAL_FUNCTIONS structure.
 *  @param   Inst    The instance number of the module to initialize. (start at
 *                   0).
 * 
 *  @return  0 OK, Nonzero - error.
 */
int xxxInitModule(HAL_DEVICE **HalDevPtr, 
                 OS_DEVICE *OsDev, 
                 HAL_FUNCTIONS **HalFunc, 
                 OS_FUNCTIONS *OsFunc, 
                 int OsFuncSize,
                 int *HalFuncSize,
                 int Inst);
int halCpusbInitModule( HAL_DEVICE **HalDevPtr,
                     OS_DEVICE *OsDev, 
                     HAL_FUNCTIONS **HalFuncPtr,
                     OS_FUNCTIONS *OsFunc,
                     int OsFuncSize, 
                     int *HalFuncSize,
                     int Inst)
{

  HAL_DEVICE *HalDev;
  HAL_FUNCTIONS *HalFunc;
  HAL_FUNCTIONS_EXT *HalFuncExt;
  int i;

  
  if (OsFuncSize < sizeof(OS_FUNCTIONS))
    return (EC_DEV_CPUSB|EC_FUNC_HAL_INIT|EC_VAL_OS_VERSION_NOT_SUPPORTED); 

  /* Malloc HalDev */
  HalDev = (HAL_DEVICE *) OsFunc->MallocDev(sizeof(HAL_DEVICE));
  if (!HalDev)
    return (EC_DEV_CPUSB|EC_FUNC_HAL_INIT|EC_VAL_MALLOC_DEV_FAILED);

  HalFunc = (HAL_FUNCTIONS *) OsFunc->Malloc(sizeof(HAL_FUNCTIONS));
   if (!HalFunc)
     {
      OsFunc->FreeDev(HalDev);
      return (EC_DEV_CPUSB|EC_FUNC_HAL_INIT|EC_VAL_MALLOC_FAILED);
     }

   HalFuncExt = (HAL_FUNCTIONS_EXT *) OsFunc->Malloc(sizeof(HAL_FUNCTIONS_EXT));
   if (!HalFuncExt)
     {
      OsFunc->FreeDev(HalDev);
      OsFunc->Free(HalFunc);
      return(EC_DEV_CPUSB|EC_FUNC_HAL_INIT|EC_VAL_MALLOC_FAILED);
     }

   /* Initialize the size of hal functions */
   *HalFuncSize = sizeof (HAL_FUNCTIONS);

  /* clear the device structure */
  cpswHalCommonmemset(HalDev, 0, sizeof(HAL_DEVICE));

  /* Clear/initialize the function pointers */
  cpswHalCommonmemset(HalFuncExt, 0, sizeof(HAL_FUNCTIONS_EXT));
  cpswHalCommonmemset(HalFunc, 0, sizeof(HAL_FUNCTIONS));

  /*****************************************************************/
  /*                 POPULATE HALDEV                              */
  /****************************************************************/
  HalDev->OsDev                = OsDev;
  HalDev->Inst                 = Inst;
  HalDev->OsFunc               = OsFunc;
  HalDev->HalFunc              = HalFunc;

  
  /****************************************************************/
  /*                 POPULATE HALFUNC                             */
  /****************************************************************/
  HalFunc->ChannelSetup        = cpswHalCpusbChannelSetup;
  HalFunc->ChannelTeardown     = cpswHalCpusbChannelTeardown; 
  HalFunc->Close               = cpswHalCpusbClose; 
  HalFunc->Control             = cpswHalCpusbControl; 
  HalFunc->Init                = 0;
  HalFunc->Open                = cpswHalCpusbOpen;
  HalFunc->PacketProcessEnd    = cpswHalCpusbPacketProcessEnd;
  HalFunc->Probe               = 0;
  HalFunc->RxReturn            = cpswHalCpusbRxReturn;
  HalFunc->Send                = cpswHalCpusbSend;
  HalFunc->Shutdown            = cpswHalCpusbShutdown;
  HalFunc->Tick                = cpswHalCpusbTick;


  /****************************************************************/
  /*                 POPULATE HALFUNCEXT                             */
  /****************************************************************/
  HalFuncExt->CtrlEPReadX      = cpswHalCpusbCtrlEPReadX;
  HalFuncExt->CtrlEPReadY      = cpswHalCpusbCtrlEPReadY;
  HalFuncExt->CtrlEPWriteX     = cpswHalCpusbCtrlEPWriteX;
  HalFuncExt->CtrlEPWriteY     = cpswHalCpusbCtrlEPWriteY;

  HalFunc->Ext                 = HalFuncExt;

 /* Initialize various HalDev members.  This is probably overkill, since these
     are initialized in ChannelSetup() and HalDev is cleared in InitModule(). */
  for (i=0; i<NUM_USB_CHAN; i++)
    { 
     HalDev->ChIsOpen[i][DIRECTION_TX] = FALSE;
     HalDev->ChIsOpen[i][DIRECTION_RX] = FALSE;
     HalDev->TxCtlBlk[i][0].TcbStart   = 0;
     HalDev->TxCtlBlk[i][1].TcbStart   = 0;
     HalDev->RxCtlBlk[i].RcbStart      = 0;
    }

  /* pass the device pointer back to the caller */
  *HalDevPtr = HalDev;
  *HalFuncPtr = HalFunc;
 
#ifdef _CPHAL_STATS
  /* initialize stats */
  cpusbStatsClear(HalDev);
#endif

   /* initialize the Params tables here */
   cpusbInitParams(HalDev);

   /* Init the Control endpoint with default values */
   ControlEPConfigInit(HalDev);
     
  /* set device state to HalInit */
  HalDev->State = enConnected;   
  HalDev->State = enInitialized; 

  /* NOTE: CPUSB HAL supports ONLY Push Model */
  

  FUNC_STOP;   
  return(0);
}


/** 
 *  @ingroup CPHAL_Functions
 *  This function shuts down the CPHAL module completely.  The caller must call
 *  Close() to put the device in reset prior shutting down.  This call will free
 *  the HalDev and the HAL function pointer structure, effectively ending
 *  communications between the driver and the CPHAL.  Further use of the module
 *  must be initiated by a call to xxxInitModule(), which starts the entire process
 *  over again.
 *
 *  @param   HalDev   CPHAL module instance. (set by xxxInitModule())  
 *
 *  @return  0 OK, Non-Zero Not OK
 */
static int cpswHalCpusbShutdown(HAL_DEVICE *HalDev)
{
 
#ifdef __CPHAL_DEBUG
   if (DBG(0))
     {
      dbgprint (1,"[cpusb20]halShutdown(HalDev:%08x)\n", (bit32u)HalDev);
     }
#endif  


   /* Verify proper device state */
   if (HalDev->State == enOpened)
     cpswHalCpusbClose(HalDev, 3); /* GSG +030429 */
   
   /* free the HalFunc */
   HalDev->OsFunc->Free(HalDev->HalFunc);


  /* free the HAL device */
   HalDev->OsFunc->FreeDev(HalDev);

   return(EC_NO_ERRORS);
}

static int ControlEPConfigInit(HAL_DEVICE *HalDev)
{
  CONTROL_EP *CtrlEP = &(HalDev->ControlData);
  
  FUNC_START;

  HalDev->AvailBuffRamSize = 0;
  
  /*
  CtrlEP->CtrlEPBuffSize = 64;
  CtrlEP->CtrlEPDblBuff = 0;  */

  if (  CtrlEP->CtrlEPDblBuff )
  {
    CtrlEP->OutEpBuffBaseAddrX  = 0; 
    CtrlEP->OutEpBuffBaseAddrY  = CtrlEP->CtrlEPBuffSize ;
    CtrlEP->InEpBuffBaseAddrX   = CtrlEP->CtrlEPBuffSize*2;
    CtrlEP->InEpBuffBaseAddrY   = CtrlEP->CtrlEPBuffSize*3;
    HalDev->AvailBuffRamSize    = CtrlEP->CtrlEPBuffSize * 4;
  }
  else
  {
    CtrlEP->OutEpBuffBaseAddrX  = 0;
    CtrlEP->OutEpBuffBaseAddrY  = 0;
    CtrlEP->InEpBuffBaseAddrX   = CtrlEP->CtrlEPBuffSize;
    CtrlEP->InEpBuffBaseAddrY   = 0;
    HalDev->AvailBuffRamSize    = CtrlEP->CtrlEPBuffSize * 2;
  }

  HalDev->CtrlEpIsSetup = TRUE;

  FUNC_STOP;
  return 0;
}


/**
 * @ingroup CPHAL_Functions
 * This function opens the specified channel.  The caller must populate
 * the @p HalCh structure.  CPHAL default values may be requested for any or al
 *  
 * @param   HalDev  CPHAL module instance. (set by xxxInitModule())   
 * @param   HalCh   Per channel information structure.  Implementation specific.
 * @param   OsSetup Pointer to an OS-defined data structure.
 *
 * @return  0 OK, Non-zero Not OK
 */
#define ChannelExtUpdate(Field) HalDev->ChData[Ch].Field = HalChn->Ext->Field
static int cpswHalCpusbChannelSetup(HAL_DEVICE *HalDev, CHANNEL_INFO *HalChn, OS_SETUP *OsSetup)
{
  int Dir;
  int Ch;
  int rc;
  bit32u currBase=0;
  CHANNEL_INFO_EXT *Ext;

  FUNC_START;

  /* Verify that the Device State is atleast Init */
  if (HalDev->State < enInitialized)
    return (EC_MODULE|EC_FUNC_CHSETUP|EC_VAL_INVALID_STATE);

  /* Channel struct should have all parameters supplied */
  if (HalChn == NULL)
  {
    return(EC_MODULE|EC_FUNC_CHSETUP|EC_VAL_NULL_CH_STRUCT);
  }  
  
  Dir = HalChn->Direction;
  Ch = HalChn->Channel;

  /* Verify that the Channel # is valid.
     Ch should be a value from (0, MAX_USB_CHAN-1) */
  if ((HalChn->Channel < 0) || (HalChn->Channel >= (MAX_USB_CHAN)))
  {
    return(EC_MODULE|EC_FUNC_CHSETUP|EC_VAL_INVALID_CH);
  }
  
  /* Verify that the current channel has not been setup
     else return error "Channel already Setup" */
  if ( Dir == DIRECTION_BOTH )
  {
    if ( HalDev->ChIsSetup[Ch][DIRECTION_TX] != FALSE ||  HalDev->ChIsSetup[Ch][DIRECTION_RX] != FALSE)
       return(EC_MODULE|EC_FUNC_CHSETUP|EC_VAL_CH_ALREADY_OPEN);
  }
  else
  {
    if ( HalDev->ChIsSetup[Ch][Dir] != FALSE )
       return(EC_MODULE|EC_FUNC_CHSETUP|EC_VAL_CH_ALREADY_OPEN);
  }

  Ext = HalChn->Ext;
  currBase = HalDev->AvailBuffRamSize;

  /* Find out if the channel can be supported */
  /* Calculate the Base Addresses in the Buffer RAM */
  if ( HalChn->Direction == DIRECTION_RX || HalChn->Direction == DIRECTION_BOTH )
  {
      if ( Ext->OUTEpDbuf )
      {
        HalDev->ChData[Ch].OUTEpBuffBaseAddrX = currBase;
        HalDev->ChData[Ch].OUTEpBuffBaseAddrY = currBase +  Ext->OUTEpBuffSize;
        currBase += Ext->OUTEpBuffSize*2;   
      } 
      else
      {
       HalDev->ChData[Ch].OUTEpBuffBaseAddrX = currBase;
       HalDev->ChData[Ch].OUTEpBuffBaseAddrY = 0;
        currBase += Ext->OUTEpBuffSize;   
      }

     if ( currBase >= USB20_USB_BUFFRAM_SIZE )
     {
       HalDev->ChData[Ch].OUTEpBuffBaseAddrX = 0;
       HalDev->ChData[Ch].OUTEpBuffBaseAddrY = 0;
       return (EC_MODULE|EC_FUNC_CHSETUP|EC_VAL_USB_BUF_RAM_OVERFLOW);
     }

     HalDev->ChIsSetup[Ch][DIRECTION_RX] = TRUE;
     /* dbgPrintf("Channel is setup Rx %d %d\n",Ch,TRUE);*/
  }

   if ( HalChn->Direction == DIRECTION_TX || HalChn->Direction == DIRECTION_BOTH )
   {
     if ( Ext->INEpDbuf )
     {
        HalDev->ChData[Ch].INEpBuffBaseAddrX = currBase;
        HalDev->ChData[Ch].INEpBuffBaseAddrY = currBase +  Ext->INEpBuffSize;
        currBase += Ext->INEpBuffSize*2;   
     } 
     else
     {
       HalDev->ChData[Ch].INEpBuffBaseAddrX = currBase;
       HalDev->ChData[Ch].INEpBuffBaseAddrY = 0;
       currBase += Ext->INEpBuffSize;   
     }

     if ( currBase >= USB20_USB_BUFFRAM_SIZE )
     {
       HalDev->ChData[Ch].INEpBuffBaseAddrX = 0;
       HalDev->ChData[Ch].INEpBuffBaseAddrY = 0;
       return (EC_MODULE|EC_FUNC_CHSETUP|EC_VAL_USB_BUF_RAM_OVERFLOW);
     }

      HalDev->ChIsSetup[Ch][DIRECTION_TX] = TRUE;
      /*dbgPrintf("Channel is setup Tx %d %d\n",Ch,TRUE);*/
   }

  
  HalDev->AvailBuffRamSize = currBase;

  /* Configure default channel values */ 
  HalDev->ChData[Ch].Channel = Ch;
  HalDev->ChData[Ch].Direction = Dir;

  /* Store OS_SETUP */
  HalDev->ChData[Ch].OsSetup = OsSetup;

  ChannelExtUpdate(OUTEpBuffSize);
  ChannelExtUpdate(OUTEpDbuf); 
  ChannelExtUpdate(INEpBuffSize);
  ChannelExtUpdate(INEpDbuf);
  HalDev->ChData[Ch].Receive = HalChn->Receive;

  /*
  dbgPrintf("HAL OutBuffSize %08x INBuffSize %08x\n",HalDev->ChData[Ch].OUTEpBuffSize,HalDev->ChData[Ch].INEpBuffSize);
  dbgPrintf("HAL IN X %08x IN Y %08x OUT X %08x OUT Y %08x\n",HalDev->ChData[Ch].INEpBuffBaseAddrX,HalDev->ChData[Ch].INEpBuffBaseAddrY,HalDev->ChData[Ch].OUTEpBuffBaseAddrX,HalDev->ChData[Ch].OUTEpBuffBaseAddrY);
  dbgPrintf("HAL InDbuff %08x %08x\n",HalDev->ChData[Ch].INEpDbuf,HalDev->ChData[Ch].OUTEpDbuf); */


  /* cppi.c needs to use Rx/TxServiceMax */
   /* Fix for new CPPI code 08/25/03 GSG
      Tx/RxServiceMax are sourced from TxCtlBlk/RxCtlBlk in CPPI,
      but our params code currently sets these values in CHANNEL_INFO_HAL.
      As a KLUGE, we must copy these values from the channel structure
      to the TxCtlBlk/RxCtlBlk. This is a simple HACK to verify that the new
      CPPI is working.
   HalDev->RxCtlBlk[0].RxServiceMax = HalDev->ChData[0].RxServiceMax;
   HalDev->TxCtlBlk[0][0].TxServiceMax = HalDev->ChData[0].TxServiceMax;
  */
  
  rc = EC_NO_ERRORS;

  /* If the hardware has been opened (is out of reset),
     then configure the channel in the hardware. */
  if (HalDev->State == enOpened)
  {
       rc = cpswHalCpusbChannelConfigApply(HalDev, Ch);   
  }

  /*dbgPrintf("Return BuffRam %08x\n",HalDev->AvailBuffRamSize);*/
  FUNC_STOP;
  return (rc);
}



/** 
 *  @ingroup CPHAL_Functions
 *  This function starts the operation of the CPHAL device.  It takes the device
 *  out of reset, and calls @c IsrRegister().  This function should be called after 
 *  calling the @c Init() function.
 *
 *  @param  HalDev   CPHAL module instance. (set by xxxInitModule())
 *
 *  @return 0 OK, Non-Zero Not OK
 */

static int cpswHalCpusbOpen(HAL_DEVICE *HalDev)
{
  int Ch;
  int Ret;
  bit32u *memPtr, i;
  bit32u *Reg;

  FUNC_START;  

  /* Verify device state is atleast Init*/
  if (HalDev->State < enInitialized)
    return (EC_MODULE|EC_FUNC_OPEN|EC_VAL_INVALID_STATE);    

  /*verify that all parameter values are within range */
  Ret = cpusbCheckAllParams(HalDev);
  /*dbgPrintf("\nAll Params checked. Ret code %08x INDbuff %08x\n", Ret, HalDev->ChData[0].INEpDbuf);*/
  /*if (Ret) return (Ret); 
    dbgPrintf("All Params checked\n"); */

  /* If and only if UCP is present, disable UCP */
  if (HalDev->UcpPresent)
  { 
    /*cpusbHalLog(CPUSB_LOG_CMN,"UCP Present\n");*/
    Reg = (bit32u *)(HalDev->UcpRegBase);
    if ( Reg )
    {
      *Reg &= ~(HalDev->UcpDisClrMask);
      *Reg |= HalDev->UcpDisVal;  
      /* dbgPrintf("UCP disabled %08x = %08x\n", (bit32u )Reg, *Reg);*/
    }
    /* else, do you need to flag an error */
  }
  
  Reg =  (bit32u *)(HalDev->ResetBase);
  if ( Reg )
    { /* HalDev->OsFunc->Printf("ResetBase %08x, Reset Bit %08x\n",(bit32u)Reg,HalDev->ResetBit);*/
    *Reg |= HalDev->ResetBit;
  }

  /*  dbgPrintf("USB Device out of reset  %08x = %08x, %08x\n", (bit32u )Reg, *Reg, HalDev->ResetBit);*/

  /* Wait for sometime before checking if the module is out of reset.
     Test if Module is out of reset 
   */
  i=0;Ch=0;
  while(USB20_MODULE_RST_REG(HalDev->dev_base) != 0x1)
  {
    /* printf("** "); MySioFlush(); */
   Ch =  i++;
  }
 
  /* printf("\n"); */

  if ( USB20_MODULE_RST_REG(HalDev->dev_base) != 0x1 ) 
    return(EC_MODULE|EC_FUNC_OPEN|EC_VAL_MODULE_IN_RST);

  if ( HalDev->debug )
  dbgPrintf("Device out of reset\n");

  /* Clock control */
  Reg = (bit32u *)(HalDev->ClkRegBase);
  if ( Reg )
  {
    *Reg |= HalDev->ClkMask;
    /* dbgPrintf("Clk control  %08x = %08x, %08x\n", (bit32u )Reg, *Reg, HalDev->ClkMask);*/
  }

  /* Set device state to Open */
  HalDev->State = enOpened; 
  /*dbgPrintf("State Open\n");*/

  /* After Reset clear the Transmit and Receive DMA Head Descriptor Pointers for all channels*/
  /* Initialize the CPPI TX, RX DMA State RAM to 0 
     Make sure that the Ch is not open state*/
  for(Ch=0;Ch<MAX_USB_CHAN;Ch++)
  {
    USB20_TXDMA_COMPPTR(HalDev->dev_base, Ch)=0;
    USB20_RXDMA_COMPPTR(HalDev->dev_base, Ch)=0;
    for(i=0;i<5;i++)
    {
      USB20_TXDMA_STWORD(HalDev->dev_base,Ch,i)=0;
      USB20_RXDMA_STWORD(HalDev->dev_base,Ch,i)=0;
    }
    /* RX CPPI State RAM has  words while TX has 5 */
    USB20_RXDMA_STWORD(HalDev->dev_base,Ch,5)=0;
  }

  /* Clear the buffer RAM */ 
  memPtr =(bit32u*) pUSB20_USB_BUFFRAM(HalDev->dev_base);
  for(i=0;i<0x1000/4;i++)
    memPtr[i]=0;
  /* dbgPrintf("Cleared State and Buffer RAM\n");*/

  /* Initialize the CPPI DMA State RAM as required by the hardware */   
  /* Set the Mode bit in the Register - Transparent mode or RNDIS Performance mode */
  USB20_CPPI_MODESEL(HalDev->dev_base) = HalDev->ModeSelect; 
  /* dbgPrintf(" ModeSelect %d\n", USB20_CPPI_MODESEL(HalDev->dev_base)); */

  /* "register" the interrupt handler */  
  /* intr_usb and intr_vbus have the locator and map_int number embedded */
  HalDev->OsFunc->IsrRegister(HalDev->OsDev, halISR_USB, HalDev->intr_usb);
  HalDev->OsFunc->IsrRegister(HalDev->OsDev, halISR_VBUS, HalDev->intr_vbus);
  /* dbgPrintf("ISR Registered : USB int %d VBUS Int %d\n",HalDev->intr_usb,HalDev->intr_vbus); */

  /* If and only if remote wakeup supported, register ISR and enable the wakeup events */
  if ( HalDev->RemWakeup )
  {

    HalDev->OsFunc->IsrRegister(HalDev->OsDev, halISR_RemWkup, HalDev->intr_remwakeup);

    /*cpusbHalLog(CPUSB_LOG_CMN,"RemWkup Enabled\n");*/
    if ((bit32u *)(HalDev->RemWakeupRegBase) )
    {
      *(volatile bit32u *)(HalDev->RemWakeupRegBase) |=  HalDev->RemWakeupEventMask | HalDev->RemWakeupIntrEn;
    }
    /* else, do you need to flag an error */


  }

  /* If control endpoint has been initialized but not opened call ControlEPConfigApply */ 
  Ret = ControlEPConfigApply(HalDev, &(HalDev->ControlData));
  if ( Ret )  return Ret;    

  /* ChannelConfigApply*/
  /* HalDev->MaxFrags=2; */
  HalDev->fraglist = HalDev->OsFunc->Malloc(HalDev->MaxFrags * sizeof(FRAGLIST)); 
  if ( !HalDev->fraglist )
     return (EC_MODULE|EC_FUNC_OPEN|EC_VAL_OS_MALLOC_FAILED);
 

  for(i=0;i<NUM_USB_CHAN;i++)
  {
    /*if ((HalDev->ChIsSetup[i]==TRUE) && (HalDev->ChIsOpen[i]==FALSE))*/
     {  
        Ret = cpswHalCpusbChannelConfigApply(HalDev, i);
        if ( Ret )
          return Ret;
     
     }
  }


  /* Init Tx and Rx DMA */
  USB20_TXCTL(HalDev->dev_base) = TX_EN;
  USB20_RXCTL(HalDev->dev_base) = RX_EN;
  
  /* Init Cell DMA */
  USB20_CELLDMA_EN(HalDev->dev_base) = TX_CELLDMA_EN | RX_CELLDMA_EN;

  /* Endian Control registers */
  USB20_TXENDCTL(HalDev->dev_base) = HalDev->ChEndianess; 
  USB20_RXENDCTL(HalDev->dev_base) = HalDev->ChEndianess; 

  /* Cannot be used only here as the channels can be setup and opened any time */
  USB20_TXINTREN_SET(HalDev->dev_base) = HalDev->VbusTxIntrEnMask;
  USB20_RXINTREN_SET(HalDev->dev_base) = HalDev->VbusRxIntrEnMask;


  /** Configure the USB20 registers */
  USB20_USB_CTRL(HalDev->dev_base) =  (FULL_SPEED*HalDev->FullSpeed) 
                                     | (HalDev->UsbIntrMask) 
                                     | ( RWUPEN*HalDev->RemWakeup)
                                     | ( 0x40*HalDev->LowPwrMode)
                                     | ( 0x1000*HalDev->ControlData.InUSBIE)
				     | ( 0x2000*HalDev->ControlData.OutUSBIE);
  
  /*dbgPrintf("USB20_USB_CTRL %08x , addr %08x\n",USB20_USB_CTRL(HalDev->dev_base), (bit32u)pUSB20_USB_CTRL(HalDev->dev_base));*/


  FUNC_STOP;
 
  return(EC_NO_ERRORS);
}


/*
 * Sets up control endpoint0 in the hardware.
 */
static int ControlEPConfigApply(HAL_DEVICE *HalDev, CONTROL_EP *HalChn)
{

  bit32u tmpBuff=0;
  
  FUNC_START;  
 
  /* Check if the Channel has been set up, Not an error for the channel to not be setup */
  if (  HalDev->CtrlEpIsSetup != TRUE )
    return EC_MODULE|EC_FUNC_CTRLEPSETUP|EC_VAL_CH_ALREADY_OPEN;

  /* Verify that the channel is not opened */
  if (HalDev->CtrlEpIsOpen  == TRUE) 
    return EC_MODULE|EC_FUNC_CTRLEPSETUP|EC_VAL_CH_ALREADY_OPEN;

  /*Initialize all channel-specific HalDev run time parameters */
  /* Configure IN Endpt - EN and Buff size*/
  /* Buffer Size should be in increments of 8 bytes and not greater than 64 bytes*/
  /*
  if ( HalChn->CtrlEPBuffSize == 8 )
     tmpBuff=0;
  if ( HalChn->CtrlEPBuffSize == 16 )
     tmpBuff=1;
  if ( HalChn->CtrlEPBuffSize == 32 )
     tmpBuff=2;
  if ( HalChn->CtrlEPBuffSize == 64 )
     tmpBuff=3;
  */
  tmpBuff = (((int)((HalChn->CtrlEPBuffSize)/16)<3)?((HalChn->CtrlEPBuffSize)/16):3);
  tmpBuff |= (HalChn->CtrlEPDblBuff<<4) | CPUSB_EP0_IN_EN | CPUSB_EP0_OUT_EN |CPUSB_EP0_OUT_INT_EN | CPUSB_EP0_IN_INT_EN ;
  USB20_USB_EP0_CFG(HalDev->dev_base)= tmpBuff ;      
   
  /* Set control endpoint state to Open */  
  HalDev->CtrlEpIsOpen = TRUE; /* channel is open */

  FUNC_STOP;
  return (EC_NO_ERRORS);
}

/*
 * Sets up channel parameters in the hardware, and initializes the CPPI
 * TX and RX buffer descriptors and buffers.
 */
static int cpswHalCpusbChannelConfigApply(HAL_DEVICE *HalDev, int Ch)
{
  int Ret;
  bit32u tmpBuff;

  /* Verify that the Device State is atleast Init */
  if (HalDev->State < enOpened)
    return (EC_MODULE|EC_FUNC_CHSETUP|EC_VAL_INVALID_STATE);

  /* If channel is not setup, do not apply. */
  if (HalDev->ChIsSetup[Ch][DIRECTION_TX] == TRUE)
  {
    if (HalDev->ChIsOpen[Ch][DIRECTION_TX] == TRUE)
    {
       dbgPrintf("Ch %d already open\n",Ch);
       /*  return EC_NO_ERRORS; */
      return(EC_MODULE|EC_FUNC_CHSETUP|EC_VAL_CH_ALREADY_OPEN);
      
    }
    else
    {
      dbgPrintf("TX Ch %d open, num buff tx %d\n",Ch, HalDev->ChData[0].TxNumBuffers);
   
      /* Temp Fix MV Jan04 until we remove that variable*/
      HalDev->ChData[Ch].TxNumQueues = 1;

      /* Initialize Queue Data */
      HalDev->TxCtlBlk[Ch][0].TxActQueueHead  = 0;
      HalDev->TxCtlBlk[Ch][0].TxActQueueCount = 0;
      HalDev->TxCtlBlk[Ch][0].TxActive        = FALSE;

      /* Clear Rx State RAM : done in halOpen */
      /*   USB20_TXDMA_STWORD0(HalDev->dev_base,Ch)=0; */
        
      /* Initialize buffer memory for the channel */
      Ret=cpswHalCpusbInitTcb(HalDev, Ch);
      if ( Ret )
        return Ret;

      USB20_TXINTREN_SET(HalDev->dev_base)|= (0x1<<Ch);  
      HalDev->VbusTxIntrEnMask |= (0x1<< Ch);     

      /* Configure IN Endpt - EN and Buff size*/
      /* Buffer Size should be in increments of 8 bytes */
      tmpBuff = HalDev->ChData[Ch].INEpBuffSize >> 3;
      USB20_EP_CFG(HalDev->dev_base,Ch) |= IEP_EN | (tmpBuff << 16) | (HalDev->ChData[Ch].INEpDbuf << 4);
      
      /* Set Endpt Buffer Base address */
      /* The data has to be shifted to the right by 4. The hardware will shift it back .*/
      USB20_EP_AD(HalDev->dev_base,Ch) |= (HalDev->ChData[Ch].INEpBuffBaseAddrX>>4) | (HalDev->ChData[Ch].INEpBuffBaseAddrY << 4);

      HalDev->ChIsOpen[Ch][DIRECTION_TX] = TRUE; /* channel is officially open */
    } /* TX is not open*/
   
  }/* TX setup ends*/
  /* else
     dbgPrintf("TX Ch %d is not setup\n",Ch);*/
 

  if (HalDev->ChIsSetup[Ch][DIRECTION_RX] == TRUE)
  {
    if (HalDev->ChIsOpen[Ch][DIRECTION_RX] != TRUE)
    {
      /* Rx Init  */
      USB20_RXDMA_STWORD1(HalDev->dev_base,Ch)=0;
      USB20_RXDMA_STWORD(HalDev->dev_base,Ch,0)=HalDev->ChData[Ch].RxBufferOffset;

      /* Initialize Queue Data */
      HalDev->RxCtlBlk[Ch].RxActQueueHead     = 0;
      HalDev->RxCtlBlk[Ch].RxActQueueCount    = 0;
      HalDev->RxCtlBlk[Ch].RxActive           = FALSE;   

            
      HalDev->VbusRxIntrEnMask |= (0x1<< Ch); 
      USB20_RXINTREN_SET(HalDev->dev_base)  |= (1<<Ch); 
  
      /* Configure OUT Endpt - EN and Buff size*/  
      tmpBuff = HalDev->ChData[Ch].OUTEpBuffSize >> 3;    
      USB20_EP_CFG(HalDev->dev_base,Ch) |= OEP_EN | ( tmpBuff << 24) | (HalDev->ChData[Ch].OUTEpDbuf << 12);

      /* Set Endpt Buffer Base address */
      /* The buffer addresses have to be shifted to the right by 4, 
         hence we have 12 and 20 instead of 16 and 24. 
         The hardware will shift it back .*/
      USB20_EP_AD(HalDev->dev_base,Ch) |= (HalDev->ChData[Ch].OUTEpBuffBaseAddrX << 12) | (HalDev->ChData[Ch].OUTEpBuffBaseAddrY << 20);    


      /* Initialize buffer memory for the channel */
      Ret=cpswHalCpusbInitRcb(HalDev, Ch);
      if ( Ret )
        return Ret;
      HalDev->ChIsOpen[Ch][DIRECTION_RX] = TRUE; /* channel is officially open */
      /*dbgPrintf("Ch RX %d is Open\n",Ch);*/
    }
    else
    {
      /* dbgPrintf("Ch %d is already Open\n",Ch);*/
      return (EC_MODULE|EC_FUNC_CHSETUP|EC_VAL_CH_ALREADY_OPEN);
    }
  }/* RX setup ends */
  /*  else
    dbgPrintf("RX Ch %d is not setup\n",Ch);*/


  FUNC_STOP;  
  return (EC_NO_ERRORS);
}




/*
 *  Called to service a module interrupt.  This function determines
 *  what type of interrupt occurred and dispatches the correct handler.
 *
 *  @param   HalDev   CPHAL module instance. (set by cphalInitModule())
 *
 *  @return  Number of Packets Serviced (Tx + Rx).  Use GetStats() for
 *           more statistics data.
 */






#ifdef __CPHAL_DEBUG
void dbgChannelConfigDump(HAL_DEVICE *HalDev, int Ch)
  {
   dbgPrintf("  [cpusb20 Inst %d, Ch %d] Config Dump:\n", HalDev->Inst, Ch);
   dbgPrintf("    RxNumBuffers  :%08d, RxBufSize   :%08d\n", 
             HalDev->ChData[Ch].RxNumBuffers, HalDev->ChData[Ch].RxBufSize);
   dbgPrintf("    TxServiceMax  :%08d, RxServiceMax:%08d\n", 
             HalDev->TxCtlBlk[Ch][0].TxServiceMax, HalDev->RxCtlBlk[Ch].RxServiceMax);
  }
#endif



/**
 *  @ingroup CPHAL_Functions
 *  Called to retrigger the interrupt mechanism after packets have been
 *  processed.  Call this function when the HalISR function indicates that
 *  there is no more work to do.  Proper use of this function will guarantee 
 *  that interrupts are never missed.  
 *
 *  @param  HalDev   CPHAL module instance. (set by xxxInitModule())  
 *
 *  @return 0 OK, Non-zero Not OK
 */
static int cpswHalCpusbPacketProcessEnd(HAL_DEVICE *HalDev)
{
  FUNC_START; 
  USB20_VBUS_EOI(HalDev->dev_base)=0; 
  FUNC_STOP;
  return (EC_NO_ERRORS);
}



/**
 *  @ingroup CPHAL_Functions
 *  Used to perform regular checks on the device.  This function should be 
 *  called at a regular interval specified by the @c Tick parameter. 
 *  
 *  @param   HalDev      CPHAL module instance. (set by xxxInitModule())
 *
 *  @return  0 OK, Non-zero error.
 */
static int cpswHalCpusbTick(HAL_DEVICE *HalDev)
{
  FUNC_START;
   if (HalDev->State != enOpened)
     return(EC_MODULE|EC_FUNC_TICK|EC_VAL_INVALID_STATE);

  FUNC_STOP;
  return(EC_NO_ERRORS);
}

void swapLong(bit32u  *pWord)
{
   bit8u *cp, c;
   cp=(bit8u *)pWord;
   c=cp[3];
   cp[3]=cp[0];
   cp[0]=c;
   c=cp[2];
   cp[2]=cp[1];
   cp[1]=c;
}


static int cpswHalCpusbCtrlEPReadX(HAL_DEVICE *HalDev, bit32u *len, bit32u *data)
{
  bit32u *sAddr; 
  bit32u Save_nWords=0;

  /*
  printf("READ_X %08x %08x\n",USB20_USB_EP0_CNT(HalDev->dev_base),USB20_USB_EP0_CFG(HalDev->dev_base));
  MySioFlush();
  */

  if ( (USB20_USB_EP0_CNT(HalDev->dev_base) & USB_OEP0_XNAK) == 0 )
  { 
    dbgprint(DBG_GENERAL,"READ_X %08x\n",USB20_USB_EP0_CNT(HalDev->dev_base));
    return 2;
  }
  

  *len = (USB20_USB_EP0_CNT(HalDev->dev_base) >> 16) & 0x7f;
  sAddr = (bit32u *) ((bit32u)pUSB20_USB_BUFFRAM(HalDev->dev_base) + HalDev->ControlData.OutEpBuffBaseAddrX);

  Save_nWords = ( (*len) / 4 ) + ( *len % 4 == 0 ? 0 : 1) ;
 
  while( Save_nWords-- )
  {
	  *data = (*sAddr++);
	  if ( HalDev->ChEndianess) 
	    swapLong(data);
	  data++;
      
  }

  /*
  if(IsCachedAddress(sAddr))
     dcache_i((void*)sAddr,nBytes*4);
  while( nBytes-- ) *dAddr++ = (*sAddr++);
  */
 
  USB20_USB_EP0_CNT(HalDev->dev_base) &= ~USB_OEP0_XNAK ;
  /*  printf("Cleared NAK ReadX\n"); MySioFlush();*/
  dbgprint(DBG_GENERAL,"EP0_CNT %08x\n",USB20_USB_EP0_CNT(HalDev->dev_base));

  return EC_NO_ERRORS;

}

static int  cpswHalCpusbCtrlEPReadY(HAL_DEVICE *HalDev, bit32u *len, bit32u *data)
{
  bit32u *sAddr; 
  bit32u Save_nWords=0;

  if ( (USB20_USB_EP0_CNT(HalDev->dev_base) & USB_OEP0_YNAK) == 0 )
  { 
    HalDev->OsFunc->Printf("READ_Y %08x\n",USB20_USB_EP0_CNT(HalDev->dev_base));
    return 2;
  }
  *len = (USB20_USB_EP0_CNT(HalDev->dev_base) >> 24) & 0x7f;
  sAddr = (bit32u *) ((bit32u)pUSB20_USB_BUFFRAM(HalDev->dev_base) + HalDev->ControlData.OutEpBuffBaseAddrY);

  Save_nWords = (*len / 4 ) + (*len % 4 == 0 ? 0 : 1) ;
  while( Save_nWords-- )
  {
	  *data = (*sAddr++);
	  if ( HalDev->ChEndianess) 
	    swapLong(data);
	  data++;
  }

  /*
  if(IsCachedAddress(sAddr))
     dcache_i((void*)sAddr,nBytes*4);
  while( nBytes-- ) *dAddr++ = (*sAddr++);
  */
 
  USB20_USB_EP0_CNT(HalDev->dev_base) &= ~USB_OEP0_YNAK ; 
  HalDev->OsFunc->Printf("EP0_CNT %08x\n",USB20_USB_EP0_CNT(HalDev->dev_base));

  return EC_NO_ERRORS;

}

static int cpswHalCpusbCtrlEPWriteX   (HAL_DEVICE *HalDev, bit32u len, bit32u *data)
{
  bit32u *dAddr,tmpData;
  bit32u Save_nWords=0;


  if ( (USB20_USB_EP0_CNT(HalDev->dev_base) & USB_IEP0_XNAK) == 0 )
    return 2;

  if ( len == 0 ) goto ACK;
  dAddr = (bit32u *) ((bit32u)pUSB20_USB_BUFFRAM(HalDev->dev_base) + HalDev->ControlData.InEpBuffBaseAddrX);

  Save_nWords = (len / 4 ) + ( len % 4 == 0 ? 0 : 1) ;


  while( Save_nWords-- )
    {
	  *dAddr = (*data++);
	  if ( HalDev->ChEndianess) 
            swapLong(dAddr);
          dAddr++;
    }

  /*
  if(IsCachedAddress(sAddr))
     dcache_i((void*)sAddr,nBytes*4);
  while( nBytes-- ) *dAddr++ = (*sAddr++);
  */

 ACK : 
  tmpData = USB20_USB_EP0_CNT(HalDev->dev_base);
  tmpData &= ~0xFF;
  tmpData |= len;
  USB20_USB_EP0_CNT(HalDev->dev_base) = tmpData;
  dbgprint(DBG_GENERAL,"WRITE_X  %08x\n",USB20_USB_EP0_CNT(HalDev->dev_base));
  return EC_NO_ERRORS;

}

static int cpswHalCpusbCtrlEPWriteY (HAL_DEVICE *HalDev, bit32u len, bit32u *data)
{
  bit32u *dAddr, tmpData;
  bit32u Save_nWords=0;

  if ( (USB20_USB_EP0_CNT(HalDev->dev_base) & USB_IEP0_YNAK) == 0 )
    return 2;

  if ( len == 0 ) goto ACK;

  dAddr = (bit32u *) ((bit32u)pUSB20_USB_BUFFRAM(HalDev->dev_base) + HalDev->ControlData.InEpBuffBaseAddrY);

  Save_nWords = ( len / 4 ) + ( len % 4 == 0 ? 0 : 1) ;
  while( Save_nWords-- )
  {
	  *dAddr = (*data++);
	  if ( HalDev->ChEndianess) 
            swapLong(dAddr);
	  data++;
  }

  /*
  if(IsCachedAddress(sAddr))
     dcache_i((void*)sAddr,nBytes*4);
  while( nBytes-- ) *dAddr++ = (*sAddr++);
  */

ACK :
  tmpData = USB20_USB_EP0_CNT(HalDev->dev_base);
  tmpData &= ~0xFF00;
  tmpData |= len << 8;
  USB20_USB_EP0_CNT(HalDev->dev_base) = tmpData;
 
  FUNC_STOP;
  dbgprint(DBG_GENERAL,"WRITE_Y  %08x\n",USB20_USB_EP0_CNT(HalDev->dev_base));
  return EC_NO_ERRORS;

}

