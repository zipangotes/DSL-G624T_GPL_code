/*************************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE: cpswhalcpusb_isr.c
 *
 *  DESCRIPTION:
 *      This file contains the isrs for the CPUSB module.
 *
 *  HISTORY:
 *  07Jul03 Menaka  1.0  Created.
 *
 *
 *****************************************************************************/
#include "cpswhalcpusb_private.h"

#define static 

/* interrupt vector masks */
     /* DMA INTERRUPTS */

#define TXDMA_CH0_PEND     0x80
#define TXDMA_CH1_PEND     0x40
#define TXDMA_CH2_PEND     0x20
#define TXDMA_CH3_PEND     0x10
#define RXDMA_CH0_PEND     0x8
#define RXDMA_CH1_PEND     0x4
#define RXDMA_CH2_PEND     0x2
#define RXDMA_CH3_PEND     0x1
#define CPDMA_INT_PENDING  0xff

     /* USB Aggregated INTR */
#define VBUS_PEND          0x100
#define SOF_PEND           0x80
#define SUSPEND_PEND       0x40
#define RESUME_PEND        0x20
#define SETUP_PEND         0x10
#define SETUPOW_PEND       0x8
#define USB_RST_PEND       0x4
#define IEP0_PEND          0x2
#define OEP0_PEND          0x1
#define USB_INT_PENDING  0x1ff


     /*----- USB Raw Interrupts */
#define INT_STPOW                   0x01000000    /* Setup overwrite intr*/
#define INT_SETUP                   0x02000000    /* Setup packet interrupt */
#define INT_RESR                    0x08000000    /* Reset resume interrupt */
#define INT_SUSR                    0x10000000    /* Suspend resume interrupt */
#define INT_RSTR                    0x20000000    /* Reset request interrupt */
#define INT_IEP0_ACK                0x00000080    /* Input endpt0 ack intr */
#define INT_VBUS                    ( 0x1 << 26 )
#define INT_OEP0_ACK                0x00000020    /* Output endpt 0 intr*/

extern int  cpswHalCpusbRxInt          (HAL_DEVICE *HalDev, int Ch, int *MoreWork);
extern int  cpswHalCpusbTxInt          (HAL_DEVICE *HalDev, int Ch, int Queue, int *MoreWork);

static int halISR_RemWkup(HAL_DEVICE *HalDev, int *MoreWork);
static int halISR_USB(HAL_DEVICE *HalDev, int *MoreWork);
static int halISR_VBUS(HAL_DEVICE *HalDev, int *MoreWork);

#if RESET_CLEANUP
int cpswhalCpusbChannelCleanup(HAL_DEVICE *HalDev, int Ch);

int cpswhalCpusbChannelCleanup(HAL_DEVICE *HalDev, int Ch)
{
   HAL_TCB *CurrentTcb,*LastTcbProcessed,*FirstTcbProcessed;
   int PacketsServiced;
   cppi_txcntl_s *pTxCtl=&HalDev->TxCtlBlk[Ch][0];     
   OS_FUNCTIONS *OsFunc = HalDev->OsFunc;

   dbgprint(1,"[cppi]cpswHalCpusbTxInt(HalDev:%08x, Ch:%d,MoreWork:%08x)\n", (bit32u)HalDev, Ch, (bit32u)MoreWork); 

   CurrentTcb = pTxCtl->TxActQueueHead;                             
   FirstTcbProcessed=CurrentTcb;

   if (CurrentTcb==0)
     {
      /* No TCB - Where did the interrupt come from?? */

      TX_DMA_STATE_CP(HalDev->dev_base,Ch,0) = VirtToPhys(pTxCtl->LastTcbProcessed) - HalDev->offset;
      return(EC_NO_ERRORS);
     }

   osfuncDataCacheHitInvalidate((void *)CurrentTcb, 16);
   PacketsServiced=0;

   OsFunc->CriticalOn();                                  
   do
   {
         /* Pop TCB(s) for packet from the stack */
         LastTcbProcessed=CurrentTcb->Eop;

	 /* Writing completion pointer without actual interrupt can generate an interrupt ?? */
         TX_DMA_STATE_CP(HalDev->dev_base,Ch,0) = VirtToPhys(LastTcbProcessed) - HalDev->offset;

         pTxCtl->TxActQueueHead = LastTcbProcessed->Next;

         if (LastTcbProcessed != CurrentTcb)
           osfuncDataCacheHitInvalidate((void *)LastTcbProcessed, 16);

         /* There is no end of queue so do not do that processing */
         pTxCtl->TxActive=FALSE;

         CurrentTcb->SendComplete(CurrentTcb->OsInfo); /* HAL 2.0 */

         /* Push Tcb(s) back onto the stack */
         CurrentTcb = LastTcbProcessed->Next;
         LastTcbProcessed->Next=pTxCtl->TcbPool;
         pTxCtl->TcbPool=FirstTcbProcessed;

         PacketsServiced++;

         /* set the first(SOP) pointer for the next packet */
         FirstTcbProcessed = CurrentTcb;

         if (CurrentTcb)
         {
            osfuncDataCacheHitInvalidate((void *)CurrentTcb, 16);
	 }

   }while(CurrentTcb);

   pTxCtl->LastTcbProcessed = LastTcbProcessed;
   OsFunc->CriticalOff();  


  return(EC_NO_ERRORS);

}
#endif

static int halISR_RemWkup(HAL_DEVICE *HalDev, int *MoreWork)
{
  FUNC_START;
  /* Clear the serial suspend -- V2U */
  if ( HalDev->LowPwrMode )
  {
    *(volatile bit32u *)(HalDev->LowPwrRegBase) &= ~(HalDev->LowPwrModeMask);
  }

 *(volatile bit32u *)HalDev->RemWakeupRegBase |= HalDev->RemWakeupEventClrMask;  
	    

  FUNC_STOP;
  return(EC_NO_ERRORS);  
}

static int halISR_USB(HAL_DEVICE *HalDev, int *MoreWork)
{
  register bit32u vecint;
  bit32u TotalPkts;
  
  FUNC_START;
  TotalPkts=0;
 
  /* service interrupts while there is more work to do*/
  while (((vecint=USB20_USB_INTVEC(HalDev->dev_base))& USB_INT_PENDING) && (TotalPkts < 500))
  {
    
    if ( vecint & VBUS_PEND )
    {  
       USB20_USB_STAT(HalDev->dev_base) = INT_VBUS;
       vecint &=~ VBUS_PEND;

       /*If remote wakeup was enabled*/
       if ( HalDev->RemWakeup )
       {
	  /* If LowPwrMode was enabled  */
          if ( HalDev->LowPwrMode )
          {
            /* Clear serial suspend */
            *(volatile bit32u *)(HalDev->LowPwrRegBase) &= ~(HalDev->LowPwrModeMask);
  
            /*  Clear the VBUS Wakeup Event also               
                HalDev->OsFunc->Printf("VBUS Wakeup set\n"); */
	    *(volatile bit32u *)HalDev->RemWakeupRegBase |= 0x1000;  
	  }
   
       }

       /* Inform the driver of VBUS connect or disconnect state */
      if (USB20_USB_IF_STAT(HalDev->dev_base) & 0x40  ) 
      {
          /*HalDev->OsFunc->Printf("CONT\n"); */
          USB20_USB_CTRL(HalDev->dev_base) |=  CONT; 
          HalDev->OsFunc->Control(HalDev->OsDev,hkStateChange,haSETbyVAL,(void*)(CPUSB_OS_USB_INT|CPUSB_INT_VBUS_CONT));
      }
      else
      {
        /*HalDev->OsFunc->Printf("DISCONT\n");*/
        USB20_USB_CTRL(HalDev->dev_base) &= ~CONT; 
        HalDev->OsFunc->Control(HalDev->OsDev,hkStateChange,haSETbyVAL,(void*)(CPUSB_OS_USB_INT|CPUSB_INT_VBUS_DISCONT));
        HalDev->LinkedUp = FALSE;
      }
#if _CPHAL_STATS
      HalDev->NumVbusInt++;
#endif
    } /* VBUS_PEND Interrupt */
 
    if ( vecint & SETUPOW_PEND) 
    {                
      USB20_USB_STAT(HalDev->dev_base) = INT_STPOW; /* clr interrupt source */
      /* Set NAK bit for Input endpoint and clear NAK for Output endpoint    */ 
      USB20_USB_EP0_CNT(HalDev->dev_base) |= USB_IEP0_XNAK | (USB_IEP0_YNAK*HalDev->ControlData.CtrlEPDblBuff); 
      HalDev->OsFunc->Control(HalDev->OsDev,hkStateChange,haSETbyVAL,(void*)(CPUSB_OS_USB_INT|CPUSB_INT_STPOW));
#if _CPHAL_STATS
      HalDev->NumSetupOwInt++;
#endif  

    }

    if ( vecint & RESUME_PEND )
    {
      USB20_USB_STAT(HalDev->dev_base) = INT_RESR;
   
      /* Clear the serial suspend -- V2U */
      if ( HalDev->LowPwrMode )
      {
        *(bit32u *)(HalDev->LowPwrRegBase) &= ~(HalDev->LowPwrModeMask);
      }

      /* Set NAK bit for Input endpoint and clear NAK for Output endpoint */
      USB20_USB_EP0_CNT(HalDev->dev_base) = USB_IEP0_XNAK|(USB_IEP0_YNAK*HalDev->ControlData.CtrlEPDblBuff); 
                          
      HalDev->OsFunc->Control(HalDev->OsDev, hkStateChange, haSETbyVAL,(void*)(CPUSB_OS_USB_INT| CPUSB_INT_RESR));

#if _CPHAL_STATS
      HalDev->NumResrInt++;
#endif
    }
  
    if ( vecint & USB_RST_PEND )
    {
      USB20_USB_STAT(HalDev->dev_base) = INT_RSTR;
      /* Set NAK bit for Input endpoint and clear NAK for Output endpoint*/
      USB20_USB_EP0_CNT(HalDev->dev_base) = USB_IEP0_XNAK 
                                          | (USB_IEP0_YNAK*HalDev->ControlData.CtrlEPDblBuff); 

#if RESET_CLEANUP
      /* Fix : On Reset,
         we should first handle any CPPI interrupt which has not been serviced.
         we must then remove unwanted send packets from the queues. We should also prevent 
	 other packets from being sent until the device is ready */
      {
        int tmp=0;
        int Ch;
        do
        {
           tmp= halISR_VBUS(HalDev,&tmp);
        }while(tmp);

        tmp =HalDev->VbusTxIntrEnMask; 
        for(Ch=0;Ch<4;Ch++, tmp>>=1)
	{
          if ( tmp & 0x1 )
            cpswhalCpusbChannelCleanup(HalDev,Ch);
	}
      }
#endif

      HalDev->OsFunc->Control(HalDev->OsDev,hkStateChange,haSETbyVAL,(void*)(CPUSB_OS_USB_INT| CPUSB_INT_RSTR));

#if _CPHAL_STATS
      HalDev->NumRstrInt++;
#endif
    }


    if ( vecint & SETUP_PEND )
    {

#if _CPHAL_STATS
      HalDev->NumSetupInt++;
#endif
      /* Setup the Channels */
      if ( HalDev->LinkedUp == FALSE )
      {
        HalDev->LinkedUp = TRUE;
        HalDev->Speed = USB20_USB_IF_STAT(HalDev->dev_base) & CPUSB_LINK_FULL_SPEED;
        HalDev->OsFunc->Control(HalDev->OsDev,  hkStateChange, haSETbyVAL,(void*)(CPUSB_OS_LINKEDUP|HalDev->Speed));
      }

      /* Set NAK bit for Input endpoint and clear NAK for Output endpoint */
      USB20_USB_EP0_CNT(HalDev->dev_base) |= USB_IEP0_XNAK | (USB_IEP0_YNAK*HalDev->ControlData.CtrlEPDblBuff);
      HalDev->OsFunc->Control(HalDev->OsDev, hkStateChange, haSETbyVAL,(void*)(CPUSB_OS_USB_INT|CPUSB_INT_SETUP));
      USB20_USB_STAT(HalDev->dev_base) = INT_SETUP; 
    }

    if ( vecint & SOF_PEND ) {}
    if ( vecint &  OEP0_PEND )
    {
/*      HalDev->OsFunc->Printf("INT OPE0 ACK\n");*/
      HalDev->OsFunc->Control(HalDev->OsDev, hkStateChange, haSETbyVAL,(void*)(CPUSB_OS_USB_INT| CPUSB_INT_OEP0_ACK));
      USB20_USB_STAT(HalDev->dev_base) = INT_OEP0_ACK;
#if _CPHAL_STATS
      HalDev->NumOEP0Int++;
#endif

    }
                
    if ( vecint &  IEP0_PEND )
    { 
      /* HalDev->OsFunc->Printf("INT IPE0 ACK\n");*/
      USB20_USB_STAT(HalDev->dev_base) = INT_IEP0_ACK;
      HalDev->OsFunc->Control(HalDev->OsDev, hkStateChange, haSETbyVAL,(void*)(CPUSB_OS_USB_INT| CPUSB_INT_IEP0_ACK));
#if _CPHAL_STATS
      HalDev->NumIEP0Int++;
#endif  

    }
    
    if ( vecint & SUSPEND_PEND )
    {
      /* HalDev->OsFunc->Printf("INT SUSR\n");*/
      /* Set NAK bit for Input endpoint and clear NAK for Output endpoint */
      USB20_USB_EP0_CNT(HalDev->dev_base) = USB_IEP0_XNAK | (USB_IEP0_YNAK*HalDev->ControlData.CtrlEPDblBuff); 
      USB20_USB_STAT(HalDev->dev_base) = INT_SUSR;
      HalDev->OsFunc->Control(HalDev->OsDev,hkStateChange, haSETbyVAL, (void*)(CPUSB_OS_USB_INT| CPUSB_INT_SUSR)); 
      /*
      if ( HalDev->LowPwrMode )
      {
        printf("Low Pwr Mode \n");MySioFlush();
        *(volatile bit32u *)(HalDev->LowPwrRegBase) |= HalDev->LowPwrModeMask;
      }
     */
      HalDev->LinkedUp = FALSE;
#if _CPHAL_STATS
      HalDev->NumSuspInt++;      
#endif  

    } 

  } /* while ends  */

   USB20_USB_EOI(HalDev->dev_base)=0; 
   return (TotalPkts);
}

static int halISR_VBUS(HAL_DEVICE *HalDev, int *MoreWork)  
{
   register bit32u tmp;
   register int i;
   int tmpWork=0;

   FUNC_START; 
   /*printf("VBUS ISR \n"); MySioFlush(); */

   /* To avoid VLYNQ register read, check all active channels for interrupts*/
   tmp = (HalDev->VbusRxIntrEnMask | (HalDev->VbusTxIntrEnMask << 4)); 
   /*while((tmp=USB20_VBUS_INTVEC(HalDev->dev_base))& CPDMA_INT_PENDING)*/
   {


      /* if ( tmp & HalDev->VbusRxIntrEnMask )*/
      {
         for(i=0;i<4;i++, tmp>>=1)
	 {
           if ( tmp & 0x1 )
	   {
             /* printf("Rx %08x & %d\n",tmp, i);MySioFlush();*/
	      cpswHalCpusbRxInt(HalDev,i, &tmpWork);
#ifdef _CPHAL_STATS 
              HalDev->RxCtlBlk[i].NumRxInt++;
#endif 
              *MoreWork |= tmpWork;
	   }
	 }
      }

      /* note: tmp has been shifted to the right by 4 */
      /* if ( tmp & HalDev->VbusTxIntrEnMask )*/
      {
         for(i=0;i<4;i++, tmp>>=1)
	 {
           if ( tmp & 0x1 )
	   {
	      /*printf("Tx %d\n",i);MySioFlush();*/
	      cpswHalCpusbTxInt(HalDev,i,CPUSB_QUEUE_NUM, &tmpWork);
#ifdef _CPHAL_STATS 
              HalDev->TxCtlBlk[i][0].NumTxInt++;
#endif 
              *MoreWork |= tmpWork;
	   }
	 }         
      }
   }

   /* Menaka - removed packet process end 6Apr2004 */ 
   /* Replaced with with halPacketProcessEnd. Inform the driver person of this change. */
   /*USB20_VBUS_EOI(HalDev->dev_base)=0; */
   return (EC_NO_ERRORS);
}	

