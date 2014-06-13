/**@file*********************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE: CPSWHALCOMMON_SUPPORT.H
 *
 *  DESCRIPTION:
 *      General include file for COMMON
 *
 *  HISTORY:
 *      Date      Modifier              Ver    Notes
 *      01Oct03   Michael Hanrahan      1.00
 *****************************************************************************/
#ifndef _CPSWHALCOMMON_SUPPORT_H
#define _CPSWHALCOMMON_SUPPORT_H

#include "cpswhalcommon_stddef.h"

size_t         cpswHalCommonstrlen(const char *s);
char *         cpswHalCommonstrcpy(char *s1, const char *s2);
void *         cpswHalCommonmemset(void *s, int c, size_t n);
unsigned long  cpswHalCommonstrtoul(const char *str, char **endptr, int base);

#ifdef __CPHAL_FUTURE_USE
unsigned long  cpswHalCommonatoul(const char *str);
int            cpswHalCommonstrcmpi(const char *s1, const char *s2);
char *         cpswHalCommonstrstr(const char *s1, const char *s2);
char *         cpswHalCommonstrchr(const char *s, int c);
int            cpswHalCommonstrcmp(const char *s1,const char  *s2);
void           cpswHalCommoncatdnum(char *s,int num);
char *         cpswHalCommonstrcat(char *s1, const char *s2);
#endif

#ifdef __CPHAL_MEMTEST
bit32u         cpswHalCommonMemBasicDataTest(bit32u base);
bit32u         cpswHalCommonMemAddressTest(bit32u base,bit32u size);
bit32u         cpswHalCommonMemDataTest(bit32u base,bit32u size,int verbose);
#endif

/*
 *
 *     R A N D O M  N U M B E R  S U P P O R T
 *
 *
 */
int  cpswHalCommonRandom(void);
int  cpswHalCommonRandomRange(int min, int max);
void cpswHalCommonSRandom(unsigned seed);

void cpswHalCommonGpioFunctional(int base, int bit);
void cpswHalCommonUnReset(bit32u ResetBase,bit32u ResetBit);
void cpswHalCommonReset(bit32u ResetBase,bit32u ResetBit);
void cpswHalCommonClockWait(bit32u resetbase,unsigned int et);


#define VirtToPhys(a)                 (((int)a)&~0xe0000000)
#define VirtToVirtNoCache(a)          ((void*)((VirtToPhys(a))|0xa0000000))
#define VirtToVirtCache(a)            ((void*)((VirtToPhys(a))|0x80000000))
#define PhysToVirtNoCache(a)          ((void*)(((int)a)|0xa0000000))
#define PhysToVirtCache(a)            ((void*)(((int)a)|0x80000000))

#ifndef dbgPrintf
#define dbgPrintf HalDev->OsFunc->Printf
#endif

#define DBG(level)  (HalDev->debug & (1<<(level)))

#ifdef __CPHAL_DEBUG
#define dbgprint(DebugFlag, format, args...)  \
  {                                           \
  if(HalDev->debug&(DebugFlag))               \
    {                                         \
    dbgPrintf(format, ## args);               \
    HalDev->OsFunc->Control(HalDev->OsDev, hkSioFlush, haNULL, 0); \
    }                                         \
  }
#else
#define dbgprint(DebugFlag, format, args...)  {}
#endif

#define VOLATILE32(addr)  (*(volatile bit32u *)(addr))

/*
 *  Enumerated hardware states.
 */
typedef enum
{
   enConnected=1, enDevFound, enInitialized, enOpened
}DEVICE_STATE;

#ifdef _CPSWHAL_NO_ASM
  #define osfuncDataCacheLineInvalidate(MemPtr)               HalDev->OsFunc->DataCacheLineInvalidate(MemPtr, 16)
  #define osfuncDataCacheLineWritebackAndInvalidate(MemPtr)   HalDev->OsFunc->DataCacheLineWriteback(MemPtr, 16)
#else
  #define osfuncDataCacheLineInvalidate(MemPtr)               __asm__(" .set mips3; cache  17, (%0); .set mips0" : : "r" (MemPtr))
  #define osfuncDataCacheLineWritebackAndInvalidate(MemPtr)   __asm__(" .set mips3; cache  21, (%0); .set mips0" : : "r" (MemPtr))
#endif

#endif
