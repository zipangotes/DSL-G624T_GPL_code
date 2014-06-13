/******************************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE:
 *
 *  DESCRIPTION:
 *   This file contains various support functions
 *
 *  HISTORY:
 *   Date    Modifier      Ver    Notes
 *   01Oct03 Denis         1.00   Original Version
 *****************************************************************************/
 
#include "cpswhalcommon_stddef.h"
#include "cpswhalcommon_support.h"

void cpswHalCommonUnReset(bit32u ResetBase,bit32u ResetBit)
  {
  bit32u *base=(bit32u*)ResetBase;
  cpswHalCommonReset(ResetBase,ResetBit);
  *base|=(1<<ResetBit);
  cpswHalCommonClockWait(ResetBase,64);
  }

void cpswHalCommonReset(bit32u ResetBase,bit32u ResetBit)
  {
  bit32u *base=(bit32u*)ResetBase;
  *base&=~(1<<ResetBit);
  cpswHalCommonClockWait(ResetBase,64);
  }

void cpswHalCommonClockWait(bit32u resetbase,unsigned int et)
  {
  volatile unsigned *rb=(volatile unsigned *)resetbase;
  unsigned int i;

  et/=4;
  while(et--)
    i=*rb;
  }
