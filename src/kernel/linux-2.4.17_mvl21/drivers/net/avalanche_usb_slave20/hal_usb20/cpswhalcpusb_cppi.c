/*************************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2002,2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE: cppi.c
 *
 *  DESCRIPTION:
 *      This file contains shared code for all CPPI modules.
 *
 *  HISTORY:
 *   7Aug02 Greg RC1.00  Original Version created.
 *  27Sep02 Mick RC1.01  Merged for use by CPMAC/CPSAR
 *  16Oct02 Mick RC1.02  Performance Tweaks (see cppihist.txt)
 *  12Nov02 Mick RC1.02  Updated to use cpmac_reg.h
 *  09Jan03 Mick RC3.01  Removed modification to RxBuffer ptr
 *  28Mar03 Mick 1.03    cpswHalCpusbRxReturn now returns error if Malloc Fails
 *  10Apr03 Mick 1.03.02 Added Needs Buffer Support
 *  11Jun03 Mick 1.06.02 Send() errors corrected
 *  07Jul03 Menaka       Updated for USB20
 *  09Oct03 Mick         Variable Queue hardcoded to 0 for some modules
 *
 *  @author  Greg Guyotte
 *  @version 1.00
 *  @date    7-Aug-2002
 *****************************************************************************/
#include "cpswhalcpusb_private.h"               /* +GSG 030320 */

HAL_CONTROL_KEY hkVersion_CPPI_cpswHalCpusb[]="CPPI 02.00.01 "__DATE__" "__TIME__;

#define CB_SOF_BIT         (1<<31)
#define CB_EOF_BIT         (1<<30)
#define CB_OWNERSHIP_BIT   (1<<29)
#define CB_EOQ_BIT         (1<<28)
#define CB_SIZE_MASK       0x0000ffff
#define CB_OFFSET_MASK     0xffff0000
#define TEARDOWN_VAL       0xfffffffc

#define CPPI_QUEUE_VALUE 0
#define CPPI_FRAGINDEX   0

/*************************************************************************
*
*     I N T E R N A L  F U N C T I O N S
*
*************************************************************************/
void cpswHalCpusbFreeRx(HAL_DEVICE *HalDev, int Ch);
void cpswHalCpusbFreeTx(HAL_DEVICE *HalDev, int Ch, int Queue);

int cpswHalCpusbRxTeardownInt(HAL_DEVICE *HalDev, int Ch);
int cpswHalCpusbTxTeardownInt(HAL_DEVICE *HalDev, int Ch, int Queue);

int cpswHalCpusbInitRcb(HAL_DEVICE *HalDev, int Ch);
int cpswHalCpusbInitTcb(HAL_DEVICE *HalDev, int Ch);

int cpswHalCpusbChannelTeardown(HAL_DEVICE *HalDev, int Ch, bit32 Mode);




/* +GSG 030421 */
void cpswHalCpusbAddToRxQueue(HAL_DEVICE *HalDev, HAL_RCB *FirstRcb, HAL_RCB *LastRcb, int FragCount, int Ch)
  {
  cppi_rxcntl_s *pRxCtl=&HalDev->RxCtlBlk[Ch]; /* +GSG 030321 */

  if (pRxCtl->RxActQueueHead==0)
    {
    dbgprint(1<<4,"[cppi]cpswHalCpusbRxReturn: Adding Rcb %08x to head, %08x is tail,
             RxActive[%d]=%d\n", (bit32u)FirstRcb, (bit32u)LastRcb,
             Ch, pRxCtl->RxActive);

    pRxCtl->RxActQueueHead=FirstRcb;
    pRxCtl->RxActQueueTail=LastRcb;
    if (!pRxCtl->RxActive)
      {
      /* write Rx Queue Head Descriptor Pointer */
      RX_DMA_STATE_HDP(HalDev->dev_base,Ch) = VirtToPhys(FirstRcb) - HalDev->offset;
      pRxCtl->RxActive=TRUE;
      }
    }
   else
    {
    register HAL_RCB *OldTailRcb;
    register bit32u rmode;

    dbgprint(1<<4,"[cppi]cpswHalCpusbRxReturn: Adding Rcb %08x to head, %08x is tail,
             RxActive[%d]=%d\n", (bit32u)FirstRcb, (bit32u)LastRcb,
             Ch, pRxCtl->RxActive);

    OldTailRcb=pRxCtl->RxActQueueTail;
    OldTailRcb->Next=(void *)FirstRcb;

    /* The previous tail need to be updated from noncached memory, The Emerald would write
       the whole cache line which could coincide with a HW write, one would be lost */

    OldTailRcb=VirtToVirtNoCache(OldTailRcb);
    OldTailRcb->HNext=VirtToPhys(FirstRcb) - HalDev->offset;
    pRxCtl->RxActQueueTail=LastRcb;
    HalDev->OsFunc->CriticalOn();
    rmode=OldTailRcb->mode;
    if (rmode&CB_EOQ_BIT)
      {
      (RX_DMA_STATE_HDP( HalDev->dev_base , Ch ))  = VirtToPhys(FirstRcb) - HalDev->offset;
      rmode&=~CB_EOQ_BIT;
      OldTailRcb->mode=rmode;
      }
    HalDev->OsFunc->CriticalOff();
    }
  pRxCtl->RxActQueueCount+=FragCount;

  USB_FREE_BUFFER_COUNT(HalDev->dev_base,Ch)=FragCount;
  }

/**
 *  @ingroup CPHAL_Functions
 *  This function is called to indicate to the CPHAL that the upper layer
 *  software has finished processing the receive data (given to it by
 *  osReceive()).  The CPHAL will then return the appropriate receive buffers
 *  and buffer descriptors to the available pool.
 *
 *  @param   HalReceiveInfo   Start of receive buffer descriptor chain returned to
 *                      CPHAL.
 *  @param   StripFlag  Flag indicating whether the upper layer software has
 *                      retained ownership of the receive data buffers.
 *<BR>
 * 'FALSE' means  that the CPHAL can reuse the receive data buffers.
 *<BR>
 * 'TRUE' : indicates the data buffers were retained by the OS
 *<BR>
 * NOTE:  If StripFlag is TRUE, it is the responsibility of the upper layer software to free the buffers when they are no longer needed.
 *
 *  @return EC_NO_ERRORS (ok). <BR>
 *          Possible Error Codes:<BR>
 *          @ref EC_VAL_INVALID_STATE "EC_VAL_INVALID_STATE"<BR>
 *          @ref EC_VAL_RCB_NEEDS_BUFFER "EC_VAL_RCB_NEEDS_BUFFER"<BR>
 *          @ref EC_VAL_RCB_DROPPED "EC_VAL_RCB_DROPPED"<BR>
 */
int cpswHalCpusbRxReturn(HAL_RECEIVEINFO *HalReceiveInfo, int StripFlag)
  {
  int Ch=HalReceiveInfo->Ch;
  HAL_RCB *LastRcb;
  HAL_DEVICE *HalDev = HalReceiveInfo->HalDev;
  int RcbSize;
  int FragCount = HalReceiveInfo->FragCount;

  if (HalDev->State != enOpened)
    return(EC_MODULE|EC_FUNC_RXRETURN|EC_VAL_INVALID_STATE);

  dbgprint(1,"[cppi]cpswHalCpusbRxReturn(HalReceiveInfo:%08x, StripFlag:%d)\n",
           (bit32u)HalReceiveInfo, StripFlag); /* ~GSG 030321 */

  LastRcb=(HAL_RCB *)HalReceiveInfo->Eop;
  LastRcb->HNext=0;
  LastRcb->Next=0;

  RcbSize = HalDev->ChData[Ch].RxBufSize;
  if (FragCount>1)
    {
    LastRcb->Off_BLen=RcbSize;
    LastRcb->mode=CB_OWNERSHIP_BIT;
    }

  HalReceiveInfo->Off_BLen=RcbSize;
  HalReceiveInfo->mode=CB_OWNERSHIP_BIT;

  /* If OS has kept the buffers for this packet, attempt to alloc new buffers */
  if (StripFlag)
    {
    HAL_RCB *TempRcb;
    HAL_RCB *CurrHeadRcb = HalReceiveInfo, *LastGoodRcb=0;         /* +GSG 030421 */
    char *pBuf;
    int rc=0,i;                                                      /*MJH+030417*/
    int GoodCount=0;                                               /*GSG+030421*/

    TempRcb = HalReceiveInfo;
    for (i=0; i<FragCount; i++)
      {
      if (TempRcb == 0)
        {
        dbgPrintf("Rx Return error while allocating new buffers\n");
        dbgPrintf("Rcb = %08x, Rcb->Eop = %08x, FragCount = %d:%d\n",
                  (bit32u)HalReceiveInfo, (bit32u)HalReceiveInfo->Eop, FragCount,i);
        HalDev->OsFunc->Control(HalDev->OsDev, hkSioFlush, haNULL, 0);

        return(EC_DEV_CPPI|EC_FUNC_RXRETURN|EC_VAL_CORRUPT_RCB_CHAIN);
        }

      dbgprint(0xa,"[cppi->os]MallocRxBuffer(Size:%d, MemBase:%08x, MemRange:%08x, OsSetup:%08x, HalReceiveInfo:%08x, OsReceiveInfo:%08x, OsDev:%08x\n",
               RcbSize,0,0xf,(bit32u)HalDev->RxCtlBlk[Ch].OsSetup, (bit32u)TempRcb,
              (bit32u)&TempRcb->OsInfo, (bit32u)HalDev->OsDev);

      /* size = ((RcbSize+15) & ~15) + 15;*/                      /*-3.01b*/
      /*size = RcbSize + 15;*/                                               /* -GSG 030421 */
      pBuf= (char *) HalDev->OsFunc->MallocRxBuffer(RcbSize,0,
                                                    0xF,HalDev->RxCtlBlk[Ch].OsSetup,
                                                    (void *)TempRcb,
                                                    (void *)&TempRcb->OsInfo,
                                                    (void *) HalDev->OsDev);
      if (!pBuf)
        {
        /* malloc failed, add this RCB to Needs Buffer List */
        TempRcb->FragCount = 1;                                             /*MJH+030417*/
        (HAL_RCB *)TempRcb->Eop = TempRcb;                                  /* GSG +030430 */

        if(HalDev->NeedsCount < MAX_NEEDS)                                  /* +MJH 030410 */
          {                                                                 /* +MJH 030410 */
          HalDev->Needs[HalDev->NeedsCount] = (HAL_RECEIVEINFO *) TempRcb; /* +MJH 030410 */
          HalDev->NeedsCount++;                                            /* +MJH 030410 */
          rc = (EC_DEV_CPPI|EC_FUNC_RXRETURN|EC_VAL_RCB_NEEDS_BUFFER);         /* ~MJH 030417 */
          }                                                                 /* +MJH 030410 */
         else                                                               /* +MJH 030410 */
          rc = (EC_DEV_CPPI|EC_FUNC_RXRETURN|EC_VAL_RCB_DROPPED);              /* ~MJH 030417 */

        /* requeue any previous RCB's that were ready to go before this one */
        if (GoodCount > 0)                                                  /* +GSG 030421 */
          {                                                                 /* +GSG 030421 */
          LastGoodRcb->HNext=0;                                            /* +GSG 030430 */
          LastGoodRcb->Next=0;                                             /* +GSG 030430 */
          osfuncDataCacheHitWritebackAndInvalidate((void *)LastGoodRcb, 16);            /* +GSG 030430 */

          cpswHalCpusbAddToRxQueue(HalDev, CurrHeadRcb, LastGoodRcb, GoodCount, Ch);   /* +GSG 030421 */
          GoodCount = 0;                                                   /* +GSG 030421 */
          }                                                                 /* +GSG 030421 */

        CurrHeadRcb = TempRcb->Next;                                        /* +GSG 030421 */
        }
       else                                                                  /* +GSG 030421 */
        {                                                                    /* +GSG 030421 */
        /* malloc succeeded, requeue the RCB to the hardware */
        TempRcb->BufPtr=VirtToPhys(pBuf) - HalDev->offset;
        TempRcb->DatPtr=pBuf;
        /* Emerald fix 10/29 */
        osfuncDataCacheHitWritebackAndInvalidate((void *)TempRcb, 16);

        /* i store the last good RCB in case the malloc fails for the
           next fragment.  This ensures that I can go ahead and return
           a partial chain of RCB's to the hardware */
        LastGoodRcb = TempRcb;                                              /* +GSG 030421 */
        GoodCount++;                                                        /* +GSG 030421 */
        }                                                                    /* +GSG 030421 */
      TempRcb = TempRcb->Next;
      } /*  end of Frag loop */
    /* if there any good RCB's to requeue, do so here */
    if (GoodCount > 0)                                                        /* +GSG 030421 */
      {
      cpswHalCpusbAddToRxQueue(HalDev, CurrHeadRcb, LastGoodRcb, GoodCount, Ch);          /* +GSG 030421 */
      }
    return(rc);                                                               /* ~GSG 030421 */
    }
   else
    {
    /*  Not Stripping */
    /* Emerald */
    /* Write Back SOP and last RCB */
    osfuncDataCacheHitWritebackAndInvalidate((void *)HalReceiveInfo, 16);

    if (FragCount > 1)
      {
      osfuncDataCacheHitWritebackAndInvalidate((void *)LastRcb, 16);
      }
    /* if not stripping buffers, always add to queue */
    cpswHalCpusbAddToRxQueue(HalDev, HalReceiveInfo, LastRcb, FragCount, Ch); /*MJH~030520*/
    }
  return(EC_NO_ERRORS);
  }

/* +MJH 030410
   Trys to liberate an RCB until liberation fails.
   Note: If liberation fails then cpswHalCpusbRxReturn will re-add the RCB to the
         Needs list.
*/

void cpswHalCpusbNeedsCheck(HAL_DEVICE *HalDev)
  {
   HalDev->OsFunc->CriticalOn();
   while(HalDev->NeedsCount)
     {
     int rc;
     HAL_RECEIVEINFO* HalRcb;

     HalDev->NeedsCount--;
     HalRcb = HalDev->Needs[HalDev->NeedsCount];
     rc =  cpswHalCpusbRxReturn(HalRcb, 1);
     /* short circuit if cpswHalCpusbRxReturn starts to fail */
     if (rc != 0)
       break;
     }
   HalDev->OsFunc->CriticalOff();
  }

/**
 *  @ingroup CPHAL_Functions
 *  This function transmits the data in FragList using available transmit
 *  buffer descriptors.  More information on the use of the Mode parameter
 *  is available in the module-specific appendices.  Note:  The OS should
 *  not call Send() for a channel that has been requested to be torndown.
 *
 *  @param   HalDev      CPHAL module instance. (set by cphalInitModule())
 *  @param   FragList    Fragment List structure.
 *  @param   FragCount   Number of fragments in FragList.
 *  @param   PacketSize  Number of bytes to transmit.
 *  @param   OsSendInfo  OS Send Information structure. <BR>
 *  @param   SendComplete Function pointer for per-packet SendComplete() function. <BR>
 *  @param   Mode        32-bit value with the following bit fields: <BR>
 *                       31-16: Mode  (used for module specific data). <BR>
 *                       15-08: Queue (transmit queue to send on). <BR>
 *                       07-00: Channel (channel number to send on).
 *
 *  @return  EC_NO_ERRORS (ok). <BR>
 *           Possible Error Codes:<BR>
 *           @ref EC_VAL_INVALID_STATE "EC_VAL_INVALID_STATE"<BR>
 *           @ref EC_VAL_NOT_LINKED "EC_VAL_NOT_LINKED"<BR>
 *           @ref EC_VAL_INVALID_CH "EC_VAL_INVALID_CH"<BR>
 *           @ref EC_VAL_OUT_OF_TCBS "EC_VAL_OUT_OF_TCBS"<BR>
 *           @ref EC_VAL_NO_TCBS "EC_VAL_NO_TCBS"<BR>
 */
int cpswHalCpusbSend(HAL_DEVICE *HalDev,FRAGLIST *FragList,
                      int FragCount,int PacketSize, OS_SENDINFO *OsSendInfo,
                      int   (*SendComplete)(OS_SENDINFO *), bit32u Mode)
  {
  HAL_TCB *tcb_ptr;
  int i;
  int rc;
  int Ch;
  cppi_txcntl_s *pTxCtl;


  dbgprint(1,"[cppi]cpswHalCpusbSend(HalDev:%08x, FragList:%08x, FragCount:%d, PacketSize:%d, OsSendInfo:%08x, Mode:%08x)\n",
           (bit32u)HalDev, (bit32u)FragList, FragCount, PacketSize, (bit32u)OsSendInfo, Mode);

  if (HalDev->State != enOpened)
     return(EC_DEV_CPPI|EC_FUNC_SEND|EC_VAL_INVALID_STATE);


   Ch = Mode & 0xFF;

   if (HalDev->ChIsOpen[Ch][DIRECTION_TX] == 0)                    /*MJH~030611*/ /*PITS 148*/
    return(EC_MODULE|EC_FUNC_SEND|EC_VAL_INVALID_CH);              /*+GSG 030303*/

   dbgprint(0x6,"[cppi->os]CriticalOn()\n");                           /* ~GSG 030321 */


   pTxCtl=&HalDev->TxCtlBlk[Ch][CPPI_QUEUE_VALUE]; /* +GSG 030321 */




   FragList[0].len |= Mode & ~0x0ffff;
   Mode = ((Mode>>8)&0x1)<<23;                /* Put the Zero byte enable in bit23 */

   HalDev->OsFunc->CriticalOn();

   tcb_ptr = pTxCtl->TcbPool;                                   /* ~GSG 030321 */

   if(tcb_ptr==0)
     {
      rc = EC_DEV_CPPI|EC_FUNC_SEND|EC_VAL_NO_TCBS;
      goto ExitSend;
     }
   else
     {
      HAL_TCB *head;

      head = tcb_ptr;
      Mode |= (PacketSize|CB_SOF_BIT|CB_OWNERSHIP_BIT);

      for (i=CPPI_FRAGINDEX; i<FragCount; i++)
        {
         /* Setup Tx mode and size */
         tcb_ptr->Off_BLen     = FragList[i].len;
         tcb_ptr->mode         = Mode;
         tcb_ptr->BufPtr       = VirtToPhys((bit32 *)FragList[i].data) - HalDev->offset;
         tcb_ptr->OsInfo       = OsSendInfo;
         tcb_ptr->SendComplete = SendComplete;         /* HAL 2.0 */

         if (i == (FragCount - 1))
           {
            /* last fragment */
            tcb_ptr->mode |= CB_EOF_BIT;

            /* since this is the last fragment, set the TcbPool pointer before
               nulling out the Next pointers */

            pTxCtl->TcbPool = tcb_ptr->Next; /* ~GSG 030321 */

            tcb_ptr->Next = 0;
            tcb_ptr->HNext = 0;

            /* In the Tx Interrupt handler, we will need to know which TCB is EOP,
               so we can save that information in the SOP */
            head->Eop = tcb_ptr;

            /* Emerald fix 10/29 */
            osfuncDataCacheHitWritebackAndInvalidate((void *)tcb_ptr, 16);

          }
         else
          {
           tcb_ptr->HNext    = VirtToPhys((bit32 *)tcb_ptr->Next) - HalDev->offset;
           Mode=CB_OWNERSHIP_BIT;

           /* Emerald fix 10/29 */
           osfuncDataCacheHitWritebackAndInvalidate((void *)tcb_ptr, 16);
           tcb_ptr = tcb_ptr->Next;  /* what about the end of TCB list?? */

           if(tcb_ptr == 0)
             {
              rc = EC_DEV_CPPI|EC_FUNC_SEND|EC_VAL_OUT_OF_TCBS;
              goto ExitSend;
             }
          }
       } /* for */

     /* put it on the high priority queue */
     if (pTxCtl->TxActQueueHead == 0)                       /*~GSG 030321*/
       {
        pTxCtl->TxActQueueHead=head;                        /*~GSG 030321*/
        pTxCtl->TxActQueueTail=tcb_ptr;                     /*~GSG 030321*/
        if (!pTxCtl->TxActive)                              /*~GSG 030321*/
          {
              /* write CPPI TX HDP */
              TX_DMA_STATE_HDP(HalDev->dev_base,Ch,CPPI_QUEUE_VALUE) = VirtToPhys(head)  - HalDev->offset;
              pTxCtl->TxActive=TRUE;                        /*~GSG 030321*/

          }
       }
      else  /* QueueHead not zero */
       {
        register volatile HAL_TCB *pTailTcb;
        register bit32u tmode;
        register bit32u pCurrentTcb;

        pTailTcb=pTxCtl->TxActQueueTail;
        pTailTcb->Next=head;

        /* The previous tail need to be updated from noncached memory, The Emerald would write
           the whole cache line which could coincide with a HW write, one would be lost */

        pTailTcb=VirtToVirtNoCache(pTailTcb);

        pCurrentTcb=VirtToPhys(head)  - HalDev->offset;
        pTailTcb->HNext=pCurrentTcb;
        pTxCtl->TxActQueueTail=tcb_ptr;
        tmode=pTailTcb->mode;
        if (tmode&CB_EOQ_BIT)
          {
           (TX_DMA_STATE_HDP(HalDev->dev_base,Ch,CPPI_QUEUE_VALUE))  = pCurrentTcb;
           tmode&=~CB_EOQ_BIT;
           pTailTcb->mode=tmode;
          }
       }
#ifdef __CPHAL_DEBUG                                        /*+GSG 030303*/
     pTxCtl->TxActQueueCount+=FragCount;                 /*+GSG 030303*/
#endif                                                      /*+GSG 030303*/

     rc = EC_NO_ERRORS;
     goto ExitSend;
    } /* tcb_ptr valid */

ExitSend:
   dbgprint(0x6,"[cppi->os]CriticalOff()\n");                /* ~GSG 030321 */

   HalDev->OsFunc->CriticalOff();
   return(rc);
  }

/*
 *  This function processes receive interrupts.  It traverses the receive
 *  buffer queue, extracting the data and passing it to the upper layer software via
 *  osReceive().  It handles all error conditions and fragments without valid data by
 *  immediately returning the RCB's to the RCB pool.
 *
 *  @param   HalDev   CPHAL module instance. (set by cphalInitModule())
 *  @param   Ch       Channel Number.
 *  @param   MoreWork Flag that indicates that there is more work to do when set to 1.
 *
 *  @return  0 if OK, non-zero otherwise.
 */
int cpswHalCpusbRxInt(HAL_DEVICE *HalDev, int Ch, int *MoreWork)
  {
   HAL_RCB *CurrentRcb, *LastRcb, *SopRcb, *EofRcb, *EopRcb;
   bit32u RxBufStatus,PacketsServiced, RxPktLen, RxSopStatus, FrmFrags, TotalFrags, FrmLen;
   cppi_rxcntl_s *pRxCtl=&HalDev->RxCtlBlk[Ch];  /*+GSG 030321*/
   int FragIndex; /* +GSG 030508 */
   int EarlyReturn = 0; /* +GSG 030521 */

   int mode;
#ifdef __CPHAL_CPPI_OFFSET
   bit32u SopOffset;
#endif

   dbgprint(0x1,"[cppi]cpswHalCpusbRxInt(HalDev:%08x, Ch:%d, MoreWork:%08x)\n", (bit32u)HalDev, Ch, (bit32u)MoreWork); /*~GSG 030321*/

   if(HalDev->NeedsCount) /* +MJH 030410 */
     cpswHalCpusbNeedsCheck(HalDev);  /* +MJH 030410 */

   /* Handle case of teardown interrupt */
   if (HalDev->RxTeardownPending[Ch] != 0)
     {
      int Ret;

      Ret = cpswHalCpusbRxTeardownInt(HalDev, Ch);
      if (Ret == 0)
        {                                                          /*+GSG 030303*/
         *MoreWork = 0; /* bug fix 1/6 */                          /*+GSG 030303*/
         return (EC_NO_ERRORS);
        }                                                          /*+GSG 030303*/
     }

   CurrentRcb=pRxCtl->RxActQueueHead;                              /*~GSG 030321 */

   osfuncDataCacheHitInvalidate((void*)CurrentRcb, 16);

   RxBufStatus=CurrentRcb->mode;
   PacketsServiced=0;

   while((CurrentRcb)&&((RxBufStatus&CB_OWNERSHIP_BIT)==0)&&
     (PacketsServiced<pRxCtl->RxServiceMax)) /*~GSG 030307*/
     {
      dbgprint(1<<16,"  [cppi Ch %d cpswHalCpusbRxInt()]Processing SOP descriptor = %08x\n",
               Ch, (bit32u)CurrentRcb);    /*~GSG 030321*/

      PacketsServiced++;                          /*~GSG 030307*/
      SopRcb=CurrentRcb;
      RxSopStatus=RxBufStatus;
      RxPktLen = RxSopStatus&CB_SIZE_MASK;

#ifdef __CPHAL_CPPI_OFFSET
      SopOffset=(SopRcb->Off_BLen&CB_OFFSET_MASK)>>16;
#endif
      FrmFrags=0;
      TotalFrags=0;
      FragIndex=0;
      FrmLen=0;
      EofRcb=0;

      do
        {
         bit32u DmaLen;

#ifdef __CPHAL_CPPI_OFFSET
         DmaLen=CurrentRcb->Off_BLen&CB_SIZE_MASK;
#else
         DmaLen=CurrentRcb->Off_BLen;
#endif
         FrmLen+=DmaLen;
         TotalFrags++;
         if (!EofRcb)
           {
            HalDev->fraglist[FragIndex].data=((char *)CurrentRcb->DatPtr);       /* ~GSG 030508 */
#ifdef __CPHAL_CPPI_OFFSET
            HalDev->fraglist[FragIndex].data+=((FrmFrags==0)?SopOffset:0);       /* ~GSG 030508 */
#endif
            HalDev->fraglist[FragIndex].len=DmaLen;                              /* ~GSG 030508 */
            HalDev->fraglist[FragIndex].OsInfo = CurrentRcb->OsInfo;             /* ~GSG 030508 */

            FrmFrags++;
            FragIndex++;                                                         /* ~GSG 030508 */
            if (FrmLen>=RxPktLen)
              EofRcb=CurrentRcb;
           }
         LastRcb=CurrentRcb;
         CurrentRcb=LastRcb->Next;
         if(CurrentRcb)
           osfuncDataCacheHitInvalidate((void*)CurrentRcb,16);
        }while(((LastRcb->mode&CB_EOF_BIT)==0)&&(CurrentRcb));

      /* New location for interrupt acknowledge */
      /* Write the completion pointer */
      RX_DMA_STATE_CP(HalDev->dev_base,Ch) = VirtToPhys(LastRcb) - HalDev->offset;

      EopRcb=LastRcb;
      pRxCtl->RxActQueueHead=CurrentRcb;      /*~GSG 030321*/
      pRxCtl->RxActQueueCount-=TotalFrags;    /*~GSG 030321*/

      if (LastRcb->mode&CB_EOQ_BIT)
        {
         if (CurrentRcb)
           {
#ifdef _CPHAL_STATS
            pRxCtl->RxMisQCnt++;
#endif
            RX_DMA_STATE_HDP(HalDev->dev_base,Ch) = LastRcb->HNext;
           }
          else
           {
#ifdef _CPHAL_STATS
            /* Rx EOQ */
            pRxCtl->RxMisQCnt++;
#endif
            pRxCtl->RxActive=FALSE;          /*~GSG 030321*/
           }
        }

      EopRcb->Next=0;

      /* setup SopRcb for the packet */
      SopRcb->Eop=(void*)EopRcb;
      SopRcb->FragCount=TotalFrags;


      /* do not pass up the packet if we're out of RCB's (or have an errored packet)*/
      if ((CurrentRcb == 0) || (EarlyReturn == 1))
        {
         cpswHalCpusbRxReturn((HAL_RECEIVEINFO *)SopRcb,0);
        }
       else
        {
         mode = Ch&0xff;

         dbgprint(0x10002,"[cppi->os]Receive(OsDev:%08x, FragList:%08x, FragCount:%d, PacketSize:%d, HalReceiveInfo:%08x, Mode:%d\n",
                  (bit32u)HalDev->OsDev, (bit32u)HalDev->fraglist,FragIndex,RxPktLen,(bit32u)SopRcb,mode);

         HalDev->ChData[Ch].Receive(HalDev->OsDev,HalDev->fraglist,FragIndex,RxPktLen,
                                   (HAL_RECEIVEINFO *)SopRcb,mode);                     /* HAL 2.0 */

        }

      if (CurrentRcb)                                                                  /*MJH+030522*/
        RxBufStatus=CurrentRcb->mode;
     } /* while */

   if ((CurrentRcb)&&((RxBufStatus&CB_OWNERSHIP_BIT)==0)) /*~GSG 030307*/
     {
      *MoreWork = 1;
     }
    else
     {
      *MoreWork = 0;
     }

   if (PacketsServiced == 0)
     {
      dbgprint(1<<16,"  [cppi cpswHalCpusbRxInt()]No Rx packets serviced on int!\n"); /*~GSG 030321*/
      dbgprint(1<<16,"                RxActQueueHead[%d] = %08x\n", Ch, (bit32u)pRxCtl->RxActQueueHead);/*~GSG 030321*/
     }

#ifdef _CPHAL_STATS
   /* update stats */
   pRxCtl->RxPacketsServiced += PacketsServiced;
   HalDev->RxTotal += PacketsServiced;
   if (pRxCtl->RxMaxServiced < PacketsServiced)
     pRxCtl->RxMaxServiced = PacketsServiced;
#endif

   return (EC_NO_ERRORS);
  }

/*
 *  This function processes transmit interrupts.  It traverses the
 *  transmit buffer queue, detecting sent data buffers and notifying the upper
 *  layer software via osSendComplete().  (for SAR, i originally had this split
 *  into two functions, one for each queue, but joined them on 8/8/02)
 *
 *  @param   HalDev   CPHAL module instance. (set by cphalInitModule())
 *  @param   Queue    Queue number to service (always 0 for MAC, Choose 1 for SAR to service low priority queue)
 *  @param   MoreWork Flag that indicates that there is more work to do when set to 1.
 *
 *  @return  0 if OK, non-zero otherwise.
 */
int cpswHalCpusbTxInt(HAL_DEVICE *HalDev, int Ch, int Queue, int *MoreWork)
  {
   HAL_TCB *CurrentTcb,*LastTcbProcessed,*FirstTcbProcessed;
   int PacketsServiced;
   bit32u TxFrameStatus;
   cppi_txcntl_s *pTxCtl=&HalDev->TxCtlBlk[Ch][CPPI_QUEUE_VALUE];                 /*+GSG 030321*/
   OS_FUNCTIONS *OsFunc = HalDev->OsFunc;


#ifdef __CPHAL_DEBUG                                                  /*+GSG 030303*/
   HAL_TCB *DbgTcb;                                                    /*+GSG 030303*/
#endif                                                                /*+GSG 030303*/

   dbgprint(1,"[cppi]cpswHalCpusbTxInt(HalDev:%08x, Ch:%d, Queue:%d, MoreWork:%08x)\n", (bit32u)HalDev, Ch,
            CPPI_QUEUE_VALUE, (bit32u)MoreWork); /*~GSG 030321*/

   /* Handle case of teardown interrupt.  This must be checked at
      the top of the function rather than the bottom, because
      the normal data processing can wipe out the completion
      pointer which is used to determine teardown complete. */
   if (HalDev->TxTeardownPending[Ch] != 0)
     {
      int Ret;

      Ret = cpswHalCpusbTxTeardownInt(HalDev, Ch, CPPI_QUEUE_VALUE);
      if (Ret == 0)
        {                                                              /*+GSG 030303*/
         *MoreWork = 0; /* bug fix 1/6 */                              /*+GSG 030303*/
         return (EC_NO_ERRORS);
        }                                                              /*+GSG 030303*/
     }

   CurrentTcb = pTxCtl->TxActQueueHead;                                /*~GSG 030321*/
   FirstTcbProcessed=CurrentTcb;

   if (CurrentTcb==0)
     {
      /* @todo If this happens on the SAR, should report an error via halStatus */

      TX_DMA_STATE_CP(HalDev->dev_base,Ch,CPPI_QUEUE_VALUE) = VirtToPhys(pTxCtl->LastTcbProcessed) - HalDev->offset;
      return(EC_NO_ERRORS);
     }

   osfuncDataCacheHitInvalidate((void *)CurrentTcb, 16);
   TxFrameStatus=CurrentTcb->mode;
   PacketsServiced=0;

   /* should the ownership bit check be inside of the loop?? could make it a
      while-do loop and take this check away */
   if ((TxFrameStatus&CB_OWNERSHIP_BIT)==0)
     {
      OsFunc->CriticalOn();                                     /*+GSG 030307*/
      do
        {
         /* Pop TCB(s) for packet from the stack */
         LastTcbProcessed=CurrentTcb->Eop;

         /* new location for acknowledge */
         /* Write the completion pointer */
         TX_DMA_STATE_CP(HalDev->dev_base,Ch,CPPI_QUEUE_VALUE) = VirtToPhys(LastTcbProcessed) - HalDev->offset;

         pTxCtl->TxActQueueHead = LastTcbProcessed->Next;

#ifdef __CPHAL_DEBUG                                                  /*+GSG 030303*/
         /* simply count the number of TCB's processed - TxActQueueCount
            only valid for debug! */                                     /*+GSG 030303*/
         DbgTcb = CurrentTcb;                                            /*+GSG 030303*/
         while (DbgTcb != LastTcbProcessed)                              /*+GSG 030303*/
           {                                                             /*+GSG 030303*/
            pTxCtl->TxActQueueCount--;                                   /*+GSG 030303*/
            DbgTcb = DbgTcb->Next;                                       /*+GSG 030303*/
           }                                                             /*+GSG 030303*/
         pTxCtl->TxActQueueCount--; /* count the last one*/              /*+GSG 030303*/
#endif                                                                  /*+GSG 030303*/

         if (LastTcbProcessed != CurrentTcb)
           osfuncDataCacheHitInvalidate((void *)LastTcbProcessed, 16);

         if (LastTcbProcessed->mode&CB_EOQ_BIT)
           {
            if (LastTcbProcessed->Next)
              {
               /* Misqueued packet */
#ifdef _CPHAL_STATS
               pTxCtl->TxMisQCnt++;
#endif
               TX_DMA_STATE_HDP(HalDev->dev_base,Ch,CPPI_QUEUE_VALUE) = LastTcbProcessed->HNext;
              }
             else
              {
               /* Tx End of Queue */
#ifdef _CPHAL_STATS
               pTxCtl->TxEOQCnt++;
#endif
               pTxCtl->TxActive=FALSE;
              }
           }

         dbgprint(0x22,"[cppi->os]SendComplete(OsSendInfo:%08x)\n", (bit32u)CurrentTcb->OsInfo);

         CurrentTcb->SendComplete(CurrentTcb->OsInfo); /* HAL 2.0 */

         /* Push Tcb(s) back onto the stack */
         CurrentTcb = LastTcbProcessed->Next;
         LastTcbProcessed->Next=pTxCtl->TcbPool;
         pTxCtl->TcbPool=FirstTcbProcessed;

         PacketsServiced++;

         TxFrameStatus=CB_OWNERSHIP_BIT;
         /* set the first(SOP) pointer for the next packet */
         FirstTcbProcessed = CurrentTcb;
         if (CurrentTcb)
           {
            osfuncDataCacheHitInvalidate((void *)CurrentTcb, 16);
            TxFrameStatus=CurrentTcb->mode;
           }

        }while(((TxFrameStatus&CB_OWNERSHIP_BIT)==0)
             &&(PacketsServiced<pTxCtl->TxServiceMax));

      pTxCtl->LastTcbProcessed = LastTcbProcessed;


    OsFunc->CriticalOff();  /*+GSG 030307*/

    if ((TxFrameStatus&CB_OWNERSHIP_BIT)==0)
      {
       *MoreWork = 1;
      }
     else
      {
       *MoreWork = 0;
      }
    }

#ifdef __CPHAL_DEBUG
  if (PacketsServiced == 0)
    {
     dbgprint(1<<16,"  [cppi Ch %d cpswHalCpusbTxInt()]No Tx packets serviced on int!\n", Ch);
    }
#endif

#ifdef _CPHAL_STATS
  /* update stats */
  pTxCtl->TxPacketsServiced += PacketsServiced;
  HalDev->TxTotal += PacketsServiced;
  if (pTxCtl->TxMaxServiced < PacketsServiced)
    pTxCtl->TxMaxServiced = PacketsServiced;
#endif

  return(EC_NO_ERRORS);
  }

/**
 *  @ingroup CPHAL_Functions
 *  This function closes the CPHAL module.  The module will be reset.
 *  All channels will be torndown and channel resources freed.
 *
 *  @param   HalDev   CPHAL module instance. (set by xxxInitModule())
 *  @param   Mode     This parameter is unused.  Passing 0 is recommended.
 *
 *  @return  EC_NO_ERRORS (ok).<BR>
 *           Possible Error Codes:<BR>
 *           @ref EC_VAL_INVALID_STATE "EC_VAL_INVALID_STATE"<BR>
 *           Any error code from cpswHalCpusbChannelTeardown().<BR>
 */
int cpswHalCpusbClose(HAL_DEVICE *HalDev, bit32 Mode)
  {
   int Ch, Ret;



   dbgprint(1,"[cppi]cpswHalCpusbClose(HalDev:%08x, Mode:%d)\n", (bit32u)HalDev, Mode);

   /* Verify proper device state */
   if (HalDev->State != enOpened)
     return (EC_MODULE | EC_FUNC_CLOSE|EC_VAL_INVALID_STATE);

   /* NOTE:  It's possible that a PARTIAL channel teardown was called prior to Close.  In
      this case, the teardown calls here will fail (because the channel(s) are already
      torndown, but the failure case will trigger the freeing of all buffer resources, to
      avoid a memory leak. */

   /* Teardown all open Tx channels */
   for (Ch = 0; Ch < HalDev->TxMaxChan; Ch++)
     {
      if (HalDev->ChIsOpen[Ch][DIRECTION_TX] == TRUE)
        {
         Ret = cpswHalCpusbChannelTeardown(HalDev, Ch, TX_TEARDOWN | FULL_TEARDOWN | BLOCKING_TEARDOWN);
         if (Ret)
           {
            int Queue=0;

        /* if teardown failed, at least attempt to free buffer resources */
            for (Queue=0; Queue<HalDev->ChData[Ch].TxNumQueues; Queue++)
              cpswHalCpusbFreeTx(HalDev, Ch, Queue);
           }
        }
     }

   /* Teardown all open Rx channels */
   for (Ch = 0; Ch < HalDev->RxMaxChan; Ch++)
     {
      if (HalDev->ChIsOpen[Ch][DIRECTION_RX] == TRUE)
        {
         Ret = cpswHalCpusbChannelTeardown(HalDev, Ch, RX_TEARDOWN | FULL_TEARDOWN | BLOCKING_TEARDOWN);
         if (Ret)
           {
            /* if teardown failed, at least attempt to free buffer resources */
            cpswHalCpusbFreeRx(HalDev, Ch);
           }
        }
     }

   /* free fraglist in HalDev */
   HalDev->OsFunc->Free(HalDev->fraglist);
   HalDev->fraglist = 0;

   /* unregister the interrupt */
   HalDev->OsFunc->IsrUnRegister(HalDev->OsDev, HalDev->intr_usb);
   HalDev->OsFunc->IsrUnRegister(HalDev->OsDev, HalDev->intr_vbus);
   if ( HalDev->RemWakeup )
     HalDev->OsFunc->IsrUnRegister(HalDev->OsDev, HalDev->intr_remwakeup);


   /* Disable the CPPI DMAs */
   USB20_TXCTL(HalDev->dev_base) = 0;
   USB20_RXCTL(HalDev->dev_base) = 0;

   /* Disable the Cell DMA */
   USB20_CELLDMA_EN(HalDev->dev_base) = 0;
   USB20_CELLDMA_EN(HalDev->dev_base) = 0;

   /* Disable all the CPPI Interrupts */
   USB20_TXINTREN_SET(HalDev->dev_base) =  0;
   USB20_RXINTREN_SET(HalDev->dev_base) =  0;

   /* Clear USB control register */
   USB20_USB_CTRL(HalDev->dev_base)=0;

   /* Close down the USB control endpoint */
   USB20_USB_EP0_CFG(HalDev->dev_base) = 0;
   HalDev->CtrlEpIsOpen = FALSE;  /* Note: Control edpt is still in setup state*/

   /* Should we reset the usb20 module ?? */
   *(volatile bit32u*)HalDev->ResetBase |= HalDev->ResetBit;




   HalDev->State = enInitialized;

   return(EC_NO_ERRORS);
  }

/**
 *  @ingroup CPHAL_Functions
 *  This function performs a teardown for the given channel.  The value of the
 *  Mode parameter controls the operation of the function, as documented below.
 *
 *  Note: If bit 3 of Mode is set, this call is blocking, and will not return
 *  until the teardown interrupt has occurred and been processed. While waiting
 *  for a blocking teardown to complete, cpswHalCpusbChannelTeardown() will signal the OS
 *  (via Control(.."Sleep"..)) to allow the OS to perform other tasks if
 *  necessary.  If and only if bit 3 of Mode is clear, the CPHAL will call the
 *  OS TeardownComplete() function to indicate that the teardown has completed.
 *
 *  @param   HalDev  CPHAL module instance. (set by xxxInitModule())
 *  @param   Ch      Channel number.
 *  @param   Mode    Bit 0 (LSB): Perform Tx teardown (if set).<BR>
 *                   Bit 1: Perform Rx teardown (if set). <BR>
 *                   Bit 2: If set, perform full teardown (free buffers/descriptors).
 *                          If clear, perform partial teardown (keep buffers). <BR>
 *                   Bit 3 (MSB): If set, call is blocking.
 *                            If clear, call is non-blocking.
 *
 *  @return  EC_NO_ERRORS (ok). <BR>
 *           Possible Error Codes:<BR>
 *           @ref EC_VAL_INVALID_STATE "EC_VAL_INVALID_STATE"<BR>
 *           @ref EC_VAL_INVALID_CH "EC_VAL_INVALID_CH"<BR>
 *           @ref EC_VAL_TX_TEARDOWN_ALREADY_PEND "EC_VAL_TX_TEARDOWN_ALREADY_PEND"<BR>
 *           @ref EC_VAL_RX_TEARDOWN_ALREADY_PEND "EC_VAL_RX_TEARDOWN_ALREADY_PEND"<BR>
 *           @ref EC_VAL_TX_CH_ALREADY_TORNDOWN "EC_VAL_TX_CH_ALREADY_TORNDOWN"<BR>
 *           @ref EC_VAL_RX_CH_ALREADY_TORNDOWN "EC_VAL_RX_CH_ALREADY_TORNDOWN"<BR>
 *           @ref EC_VAL_TX_TEARDOWN_TIMEOUT "EC_VAL_TX_TEARDOWN_TIMEOUT"<BR>
 *           @ref EC_VAL_RX_TEARDOWN_TIMEOUT "EC_VAL_RX_TEARDOWN_TIMEOUT"<BR>
 *           @ref EC_VAL_LUT_NOT_READY "EC_VAL_LUT_NOT_READY"<BR>
 */
int cpswHalCpusbChannelTeardown(HAL_DEVICE *HalDev, int Ch, bit32 Mode)
  {
   int DoTx, DoRx, Sleep=2048, timeout=0;                         /*MJH~030306*/

/* Set the module, used for error returns */
   /* dummy loop variable */ /* 030707+MV */
   int tmp=0;

   dbgprint(1,"[cppi]cpswHalCpusbChannelTeardown(HalDev:%08x, Ch:%d, Mode:%d)\n",
            (bit32u)HalDev, Ch, Mode);

   if (HalDev->State < enInitialized)
     return(EC_MODULE|EC_FUNC_CHTEARDOWN|EC_VAL_INVALID_STATE);


   /* USB only supports tearing down all channels at once and only supports full teardown*/
   Mode = (Mode | TX_TEARDOWN | RX_TEARDOWN |FULL_TEARDOWN );
   /* Make sure that the CPPI DMAs are in IDLE state before tearing down the channels */
   while ( USB20_TEARDOWN_REG(HalDev->dev_base) != 0 ){ tmp++;}
   /* Disable all the interrupts */
   USB20_TXINTREN_CLR(HalDev->dev_base) = HalDev->VbusTxIntrEnMask;
   USB20_RXINTREN_CLR(HalDev->dev_base) = HalDev->VbusRxIntrEnMask;

   DoTx = (Mode & TX_TEARDOWN);
   DoRx = (Mode & RX_TEARDOWN);

   if (DoTx)
     {
      if ((Ch < 0) || (Ch >= HalDev->TxMaxChan))
        {
         return(EC_MODULE|EC_FUNC_CHTEARDOWN|EC_VAL_INVALID_CH);
        }
     }

   if (DoRx)
     {
      if ((Ch < 0) || (Ch >= HalDev->RxMaxChan))
        {
         return(EC_MODULE|EC_FUNC_CHTEARDOWN|EC_VAL_INVALID_CH);
        }
     }

   /* set teardown pending bits before performing the teardown, because they
      will be used in the int handler (this is done for AAL5) */
   if (DoTx)
     {
      if (HalDev->TxTeardownPending[Ch] != 0)
        return(EC_MODULE|EC_FUNC_CHTEARDOWN|EC_VAL_TX_TEARDOWN_ALREADY_PEND);

      /* If a full teardown, this also means that the user must
      setup all channels again to use them */
      if (Mode & FULL_TEARDOWN)
        HalDev->ChIsSetup[Ch][DIRECTION_TX] = 0;

      if (HalDev->State < enOpened)
        {
         /* if the hardware has never been opened, the channel has never actually
         been setup in the hardware, so I just need to reset the software flag
         and leave */
         HalDev->ChIsSetup[Ch][DIRECTION_TX] = 0;
         return (EC_NO_ERRORS);
        }
       else
        {
         if (HalDev->ChIsOpen[Ch][DIRECTION_TX] == 0)
           {
            return(EC_MODULE|EC_FUNC_CHTEARDOWN|EC_VAL_TX_CH_ALREADY_TORNDOWN);
           }

         /* set teardown flag */
         HalDev->TxTeardownPending[Ch] = Mode;
        }
     }

   if (DoRx)
     {
      if (HalDev->RxTeardownPending[Ch] != 0)
        return(EC_MODULE|EC_FUNC_CHTEARDOWN|EC_VAL_RX_TEARDOWN_ALREADY_PEND);

      if (Mode & FULL_TEARDOWN)
        HalDev->ChIsSetup[Ch][DIRECTION_RX] = 0;

      if (HalDev->State < enOpened)
        {
         HalDev->ChIsSetup[Ch][DIRECTION_RX] = 0;
         return (EC_NO_ERRORS);
        }
       else
        {
         if (HalDev->ChIsOpen[Ch][DIRECTION_RX] == 0)
           return(EC_MODULE|EC_FUNC_CHTEARDOWN|EC_VAL_RX_CH_ALREADY_TORNDOWN);

         HalDev->RxTeardownPending[Ch] = Mode;
        }
     }


   /* Perform Tx Teardown Duties */
   if ((DoTx) && (HalDev->State == enOpened))
     {
      /* Request TX channel teardown */
      TRANSMIT_PORT_TEARDOWN_REG(HalDev->dev_base) = Ch;

      /* wait until teardown has completed */
      if(Mode & BLOCKING_TEARDOWN)
        {
         timeout = 0;
         while (HalDev->ChIsOpen[Ch][DIRECTION_TX] == TRUE)
           {
            HalDev->OsFunc->Control(HalDev->OsDev, hkSleep, haSETbyVAL , (void*)Sleep);
            timeout++;
            if(timeout > 100000)
              {
               return(EC_MODULE|EC_FUNC_CHTEARDOWN|EC_VAL_TX_TEARDOWN_TIMEOUT);
              }
           }
        }
     } /* if DoTx */

   /* Perform Rx Teardown Duties */
   if ((DoRx) && (HalDev->State == enOpened))
     {

      if(Mode & BLOCKING_TEARDOWN)
        {
         timeout = 0;
         while (HalDev->ChIsOpen[Ch][DIRECTION_RX] == TRUE)
           {
            HalDev->OsFunc->Control(HalDev->OsDev, hkSleep, haSETbyVAL , (void*)Sleep);
            timeout++;
            if(timeout > 100000)
              {
              return(EC_MODULE|EC_FUNC_CHTEARDOWN|EC_VAL_RX_TEARDOWN_TIMEOUT);
              }
           }
        }
     } /* if DoRx */

   return (EC_NO_ERRORS);
  }

/* return of 0 means that this code executed, -1 means the interrupt was not
   a teardown interrupt */
int cpswHalCpusbRxTeardownInt(HAL_DEVICE *HalDev, int Ch)
  {
   cppi_rxcntl_s *pRxCtl=&HalDev->RxCtlBlk[Ch]; /* +GSG 030321 */


   dbgprint(1,"[cppi]cpswHalCpusbRxTeardownInt(HalDev:%08x, Ch:%d)\n", (bit32u)HalDev, Ch); /* ~GSG 030321 */

   /* check to see if the interrupt is a teardown interrupt */
   if ((RX_DMA_STATE_CP(HalDev->dev_base,Ch) & TEARDOWN_VAL) != TEARDOWN_VAL)
     return (-1);
   /* finish channel teardown */

   /* Free channel resources on a FULL teardown */
   if (HalDev->RxTeardownPending[Ch] & FULL_TEARDOWN)
   {
        cpswHalCpusbFreeRx(HalDev, Ch);
   }

   /* bug fix - clear Rx channel pointers on teardown */
   pRxCtl->RcbPool         = 0;     /* ~GSG 030321 */
   pRxCtl->RxActQueueHead  = 0;     /* ~GSG 030321 */
   pRxCtl->RxActQueueCount = 0;     /* ~GSG 030321 */
   pRxCtl->RxActive        = FALSE; /* ~GSG 030321 */

   /* write completion pointer */
   RX_DMA_STATE_CP(HalDev->dev_base,Ch) = TEARDOWN_VAL;

   /* use direction bit as a teardown pending bit! May be able to
      use only one teardown pending integer in HalDev */

   HalDev->RxTeardownPending[Ch] &= ~RX_TEARDOWN;

   HalDev->ChIsOpen[Ch][DIRECTION_RX] = 0;



   return (EC_NO_ERRORS);
  }

/* return of 0 means that this code executed, -1 means the interrupt was not
   a teardown interrupt */
int cpswHalCpusbTxTeardownInt(HAL_DEVICE *HalDev, int Ch, int Queue)
  {
   HAL_TCB *Last, *Curr, *First;                                    /*+GSG 030303*/
   cppi_txcntl_s *pTxCtl=&HalDev->TxCtlBlk[Ch][CPPI_QUEUE_VALUE];              /*+GSG 030321*/


   dbgprint(1,"[cppi]cpswHalCpusbTxTeardownInt(HalDev:%08x, Ch:%d, Queue:%d)\n", (bit32u)HalDev, Ch, CPPI_QUEUE_VALUE);

   if ((TX_DMA_STATE_CP(HalDev->dev_base,Ch,CPPI_QUEUE_VALUE) & TEARDOWN_VAL) == TEARDOWN_VAL)
     {
      /* return outstanding buffers to OS                             +RC3.02*/
      Curr = pTxCtl->TxActQueueHead;                                /*+GSG 030303*/
      First = Curr;                                                 /*+GSG 030303*/
      while (Curr)                                                  /*+GSG 030303*/
        {                                                           /*+GSG 030303*/
         /* Pop TCB(s) for packet from the stack */                 /*+GSG 030303*/
         Last = Curr->Eop;                                          /*+GSG 030303*/
         pTxCtl->TxActQueueHead = Last->Next;                       /*+GSG 030303*/
                                                                    /*+GSG 030303*/
         /* return to OS */                                         /*+GSG 030303*/
         Curr->SendComplete(Curr->OsInfo); /* HAL 2.0 */
                                                                    /*+GSG 030303*/
         /* Push Tcb(s) back onto the stack */                      /*+GSG 030303*/
         Curr = Last->Next;                                         /*+GSG 030303*/
         Last->Next = pTxCtl->TcbPool;                              /*+GSG 030303*/
         pTxCtl->TcbPool = First;                                   /*+GSG 030303*/
                                                                    /*+GSG 030303*/
         /* set the first(SOP) pointer for the next packet */       /*+GSG 030303*/
         First = Curr;                                              /*+GSG 030303*/
        }                                                           /*+GSG 030303*/

      /* finish channel teardown */

      /* save the OsInfo to pass to upper layer
         THIS WAS CRASHING - because it's possible that I get the teardown
         notification and the TcbHPool is null.  In this case, the buffers
         to free can be found in the TxHActiveQueue.  If I need to get OsInfo
         in the future, I can get it from one of those buffers.
         OsInfo = pTxCtl->TcbHPool->OsInfo; */

      if (HalDev->TxTeardownPending[Ch] & FULL_TEARDOWN)
        {
         cpswHalCpusbFreeTx(HalDev, Ch, CPPI_QUEUE_VALUE);
        } /* if FULL teardown */

      /* bug fix - clear Tx channel pointers on teardown */
      pTxCtl->TcbPool = 0;             /* ~GSG 030321 */
      pTxCtl->TxActQueueHead  = 0;     /* ~GSG 030321 */
      pTxCtl->TxActQueueCount = 0;     /* ~GSG 030321 */
      pTxCtl->TxActive        = FALSE; /* ~GSG 030321 */

      /* write completion pointer */
      TX_DMA_STATE_CP(HalDev->dev_base,Ch,CPPI_QUEUE_VALUE) = TEARDOWN_VAL;

      /* no longer pending teardown */
      HalDev->TxTeardownPending[Ch] &= ~TX_TEARDOWN;

      HalDev->ChIsOpen[Ch][DIRECTION_TX] = 0;



      return (EC_NO_ERRORS);
     }
   return (-1);
  }

void cpswHalCpusbFreeRx(HAL_DEVICE *HalDev, int Ch)
  {
   HAL_RCB *rcb_ptr;                                                /*+GSG 030303*/
   int rcbSize = (sizeof(HAL_RCB)+0xf)&~0xf;                        /*+GSG 030303*/
   int Num = HalDev->ChData[Ch].RxNumBuffers, i;                    /*+GSG 030303*/
   cppi_rxcntl_s *pRxCtl=&HalDev->RxCtlBlk[Ch];                     /*+GSG 030320*/

   dbgprint(1<<0,"cpswHalCpusbFreeRx(HalDev:%08x, Ch:%d)\n", (bit32u)HalDev, Ch); /*~GSG 030320 */

   /* Free Rx data buffers attached to descriptors, if necessary */
   if (pRxCtl->RcbStart != 0)                                       /*~GSG 030320*/
     {                                                              /*+GSG 030303*/
      for(i=0;i<Num;i++)                                            /*+GSG 030303*/
        {                                                           /*+GSG 030303*/
         rcb_ptr = (HAL_RCB *)(pRxCtl->RcbStart + (i*rcbSize));     /*+GSG 030303*/

         /* free the data buffer */
         if (rcb_ptr->DatPtr != 0)
           {
            dbgprint(0xa,"[os]FreeRxBuffer(MemPtr:%08x)\n", (bit32u)rcb_ptr->DatPtr);
            dbgprint(1<<6,"  Freeing Data Buffer, Ch %d\n", Ch);    /*+GSG 030303 */

            HalDev->OsFunc->FreeRxBuffer((void *)rcb_ptr->OsInfo, (void *)rcb_ptr->DatPtr);
            rcb_ptr->OsInfo=0;                                        /*MJH+030522*/
            rcb_ptr->DatPtr=0;                                        /*MJH+030522*/
           }
        }                                                           /*+GSG 030303*/
     }                                                              /*+GSG 030303*/

   dbgprint(0xa,"[os]Free(MemPtr:%08x)\n", (bit32u)pRxCtl->RcbStart); /*~GSG 030320 */
   dbgprint(1<<6,"  Freeing Rx buffer descriptors, Ch %d\n", Ch);     /*~GSG 030320 */

   /* free up all desciptors at once */
   if (pRxCtl->RcbStart)
     HalDev->OsFunc->FreeDmaXfer(pRxCtl->RcbStart);

   /* mark buffers as freed */
   pRxCtl->RcbStart = 0;
  }


/*
 *  This function allocates transmit buffer descriptors (internal CPHAL function).
 *  It creates a high priority transmit queue by default for a single Tx
 *  channel.  If QoS is enabled for the given CPHAL device, this function
 *  will also allocate a low priority transmit queue.
 *
 *  @param   HalDev   CPHAL module instance. (set by cphalInitModule())
 *  @param   Ch       Channel number.
 *
 *  @return  0 OK, Non-Zero Not OK
 */
int cpswHalCpusbInitTcb(HAL_DEVICE *HalDev, int Ch)
  {
   int i, Num = HalDev->ChData[Ch].TxNumBuffers;
   HAL_TCB *pTcb=0;
   char *AllTcb;
   int  tcbSize;
   int SizeMalloc;
   int  Queue;

   dbgprint(1,"[cppi]cpswHalCpusbInitTcb(HalDev:%08x, Ch:%d)\n", (bit32u)HalDev, Ch); /* ~GSG 030321 */
   dbgprint(1<<6,"  [cppi cpswHalCpusbInitTcb()]TxNumBuffers:%d\n", Num);             /* ~GSG 030321 */

   tcbSize = (sizeof(HAL_TCB)+0xf)&~0xf;
   SizeMalloc = (tcbSize*Num)+0xf;

   for (Queue=0; Queue < HalDev->ChData[Ch].TxNumQueues; Queue++)
     {
      cppi_txcntl_s *pTxCtl=&HalDev->TxCtlBlk[Ch][Queue]; /* +GSG 030321 */

      if (pTxCtl->TcbStart == 0) /* ~GSG 030321 */
        {
         dbgprint(1<<6,"  [cppi cpswHalCpusbInitTcb()]Allocate Tx buffer descriptors\n"); /* ~GSG 030321 */
         dbgprint(0xa,"[cppi->os]MallocDmaXfer(Size:%d, MemBase:%08x, MemRange:%08x)\n", SizeMalloc,0,0xffffffff); /* ~GSG 030321 */

         /* malloc all TCBs at once */
         AllTcb = (char *)HalDev->OsFunc->MallocDmaXfer(SizeMalloc,0,0xffffffff);
         if (!AllTcb)
           {
            return(EC_DEV_CPPI|EC_FUNC_HAL_INIT|EC_VAL_TCB_MALLOC_FAILED);
           }

         dbgprint(1<<6,"  [cppi cpswHalCpusbInitTcb()]Clear Tx buffer descriptors\n"); /* ~GSG 030321 */

         cpswHalCommonmemset(AllTcb, 0, SizeMalloc);

         /* keep this address for freeing later */
         pTxCtl->TcbStart = AllTcb;                      /* ~GSG 030321 */
        }
       else
        {
         /* if the memory has already been allocated, simply reuse it! */
         AllTcb = pTxCtl->TcbStart;                      /* ~GSG 030321 */
        }

      /* align to cache line */
      AllTcb = (char *)(((bit32u)AllTcb + 0xf) &~ 0xf); /*PITS #143  MJH~030522*/

      /* default High priority transmit queue */
      pTxCtl->TcbPool=0;                                 /* ~GSG 030321 */
      for(i=0;i<Num;i++)
        {
         /*pTcb=(HAL_TCB *) OsFunc->MallocDmaXfer(sizeof(HAL_TCB),0,0xffffffff); */
         pTcb= (HAL_TCB *)(AllTcb + (i*tcbSize));
         pTcb->mode=0;
         pTcb->BufPtr=0;
         pTcb->Next=pTxCtl->TcbPool;                     /* ~GSG 030321 */
         pTcb->Off_BLen=0;
         pTxCtl->TcbPool=pTcb;                           /* ~GSG 030321 */
        }
     }

   return(EC_NO_ERRORS);
  }

/*
 *  This function allocates receive buffer descriptors (internal CPHAL function).
 *  After allocation, the function 'queues' (gives to the hardware) the newly
 *  created receive buffers to enable packet reception.
 *
 *  @param   HalDev   CPHAL module instance. (set by cphalInitModule())
 *  @param   Ch    Channel number.
 *
 *  @return  0 OK, Non-Zero Not OK
 */
int cpswHalCpusbInitRcb(HAL_DEVICE *HalDev, int Ch)
  {
   int i, Num = HalDev->ChData[Ch].RxNumBuffers;
   int Size = HalDev->ChData[Ch].RxBufSize;
   HAL_RCB *pRcb;
   char *pBuf;
   char *AllRcb;
   int  rcbSize;
   int  DoMalloc = 0;
   int SizeMalloc;
   int MallocSize;
   cppi_rxcntl_s *pRxCtl=&HalDev->RxCtlBlk[Ch];  /* +GSG 030321 */

   dbgprint(1,"[cppi]cpswHalCpusbInitRcb(HalDev:%08x, Ch:%d)\n", (bit32u)HalDev, Ch); /* ~GSG 030321 */
   dbgprint(1<<6,"  [cppi cpswHalCpusbInitRcb()]RxNumBuffers:%d\n", Num);             /* ~GSG 030321 */
   dbgprint(1<<6,"  [cppi cpswHalCpusbInitRcb()]RxBufSize   :%d\n", Size);            /* ~GSG 030321 */

   rcbSize = (sizeof(HAL_RCB)+0xf)&~0xf;
   SizeMalloc = (rcbSize*Num)+0xf;

   if (pRxCtl->RcbStart == 0) /* ~GSG 030321 */
     {
      DoMalloc = 1;

      dbgprint(1<<6,"  [cppi Ch %d cpswHalCpusbInitRcb()]Allocate Rx buffer descriptors\n", Ch); /* ~GSG 030321 */
      dbgprint(0xa,"[cppi->os]MallocDmaXfer(Size:%d, MemBase:%08x, MemRange:%08x)\n",SizeMalloc,0,0xffffffff); /* ~GSG 030321 */

      /* malloc all RCBs at once */
      AllRcb= (char *)HalDev->OsFunc->MallocDmaXfer(SizeMalloc,0,0xffffffff);
      if (!AllRcb)
        {
         return(EC_DEV_CPPI|EC_FUNC_HAL_INIT|EC_VAL_RCB_MALLOC_FAILED);
        }

      dbgprint(1<<6,"  [cppi Ch %d cpswHalCpusbInitRcb()]Clear Rx buffer descriptors\n", Ch); /* ~GSG 030321 */

      cpswHalCommonmemset(AllRcb, 0, SizeMalloc);

      /* keep this address for freeing later */
      pRxCtl->RcbStart = AllRcb;     /* ~GSG 030321 */
     }
    else
     {
      /* if the memory has already been allocated, simply reuse it! */
      AllRcb = pRxCtl->RcbStart;     /* ~GSG 030321 */
     }

   /* align to cache line */
   AllRcb = (char *)(((bit32u)AllRcb + 0xf)&~0xf);  /*PITS #143  MJH~030522*/

   pRxCtl->RcbPool=0;                /* ~GSG 030321 */
   for(i=0;i<Num;i++)
     {
      pRcb = (HAL_RCB *)(AllRcb + (i*rcbSize));

      if (DoMalloc == 1)
        {
         dbgprint(0xa,"[cppi->os]MallocRxBuffer(Size:%d, MemBase:%08x, MemRange:%08x, OsSetup:%08x, HalReceiveInfo:%08x, OsReceiveInfo:%08x, OsDev:%08x)\n",
              Size,0,0xf,(bit32u)HalDev->RxCtlBlk[Ch].OsSetup, (bit32u)pRcb,(bit32u)&pRcb->OsInfo, (bit32u)HalDev->OsDev);

         MallocSize = Size;                                       /*~3.01 */
         pBuf= (char *) HalDev->OsFunc->MallocRxBuffer(MallocSize,0,0xF,HalDev->RxCtlBlk[Ch].OsSetup, (void *)pRcb, (void *)&pRcb->OsInfo, (void *) HalDev->OsDev);
         if (!pBuf)
           {
            return(EC_DEV_CPPI|EC_FUNC_HAL_INIT|EC_VAL_RX_BUFFER_MALLOC_FAILED);
           }
         /* -RC3.01 pBuf = (char *)(((bit32u)pBuf+0xF) & ~0xF); */
         pRcb->BufPtr=VirtToPhys(pBuf) - HalDev->offset;
         pRcb->DatPtr=pBuf;
         /*pRcb->BufSize=Size;*/
        }
      pRcb->mode=0;
      pRcb->Ch=Ch;
      pRcb->Next=(void *)pRxCtl->RcbPool; /* ~GSG 030321 */
      pRcb->Off_BLen=0;
      pRcb->HalDev = HalDev;
      pRxCtl->RcbPool=pRcb;               /* ~GSG 030321 */
     }

   /* Give all of the Rx buffers to hardware */

   dbgprint(1<<16,"  [cppi Ch %d cpswHalCpusbInitRcb()]First free buffer: %08x\n", Ch, (bit32u)pRxCtl->RcbPool); /* ~GSG 030321 */

   while(pRxCtl->RcbPool)                 /* ~GSG 030321 */
     {
      pRcb=pRxCtl->RcbPool;               /* ~GSG 030321 */
      pRxCtl->RcbPool=pRcb->Next;         /* ~GSG 030321 */
      pRcb->Eop=(void*)pRcb;
      pRcb->FragCount=1;
      cpswHalCpusbRxReturn((HAL_RECEIVEINFO *)pRcb, 0); /* ~GSG 030321 */
     }

   return(EC_NO_ERRORS);
  }

void cpswHalCpusbFreeTx(HAL_DEVICE *HalDev, int Ch, int Queue)
  {
   cppi_txcntl_s *pTxCtl=&HalDev->TxCtlBlk[Ch][CPPI_QUEUE_VALUE]; /* +GSG 030321 */

   dbgprint(1,"[cppi]cpswHalCpusbFreeTx(HalDev:%08x, Ch:%d, Queue:%d)\n", (bit32u)HalDev, Ch, CPPI_QUEUE_VALUE); /* +GSG 030321 */

   dbgprint(0xa,"[cppi->os]FreeDmaXfer(MemPtr:%08x)\n", (bit32u)pTxCtl->TcbStart); /* +GSG 030321 */
   dbgprint(1<<6,"  [cppi cpswHalCpusbFreeTx()]Freeing Tx buffer descriptors, Ch %d, Queue %d\n", Ch, CPPI_QUEUE_VALUE); /* +GSG 030321 */

   /* free all descriptors at once */
   if (pTxCtl->TcbStart)
     HalDev->OsFunc->FreeDmaXfer(pTxCtl->TcbStart); /* ~GSG 030321 */

   pTxCtl->TcbStart = 0; /* ~GSG 030321 */
  }
