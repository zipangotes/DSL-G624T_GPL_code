/**@file*********************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE: cpswhal.h
 *
 *  DESCRIPTION:
 *      User include file, contains data definitions shared between the CPHAL
 *      and the upper-layer software.
 *
 *  HISTORY:
 *      Date      Modifier  Notes
 *      06May03   Mick      From CPHAL 1.5
 *      14Nov03   Mick      stddef replaced with defs.h
 *      14Jun04   Mick      added Control Macros, moved CPHAL definitions to defs.h
 *****************************************************************************/

#ifndef _CPSWHAL_H
#define _CPSWHAL_H

#include "cpswhalcommon_defs.h"
#include "cpswhalcommon_ecerrors.h"
#include "cpswhalcommon_global.h"

/*
 *
 *  Control macros for Setting values in the CPHAL and extracting values from the CPHAL
 *  usage:  cpswhalSet(HalDev, HalFunc, hkCpuFrequency, 150000000)
 *          cpswhalGet(HalDev, HalFunc, hkCpuFrequency, &cpuFreq)
 *
 */
#define cpswhalGet(HalDev, HalFunc, key, value) HalFunc->Control(HalDev,key,haGETbyREF,value)
#define cpswhalSet(HalDev, HalFunc, key, value) HalFunc->Control(HalDev,key,haSETbyVAL,(void*)value)

/*
 *
 * Following macros set individual Channel and Queue parameters
 *  usage:  cpswhalSetChannel(HalDev, HalFunc, hkNumBufs, 15, 3)
 *          sets the Number of Rx buffers for Channel 3 to 15.
 *
 */

#define cpswhalSetChannel(HalDev, HalFunc, key, value, ch) HalFunc->Control(HalDev,key,ACTION_CH(haSETbyVAL,ch),(void*)value)
#define cpswhalGetChannel(HalDev, HalFunc, key, value, ch) HalFunc->Control(HalDev,key,ACTION_CH(haGETbyREF,ch),value)

#define cpswhalSetChannelQueue(HalDev, HalFunc, key, value, ch,q) HalFunc->Control(HalDev,key,ACTION_CH_Q(haSETbyVAL,ch,q),(void*)value)
#define cpswhalGetChannelQueue(HalDev, HalFunc, key, value, ch,q) HalFunc->Control(HalDev,key,ACTION_CH_Q(haGETbyREF,ch,q),value)

/*
 *
 * Extension structure typedefs
 *
 */ 

typedef struct os_functions_ext_s   OS_FUNCTIONS_EXT;

/**
@defgroup shared_data Shared Data Structures

The data structures documented here are shared by all modules.
*/

/**
 *  @ingroup shared_data
 *  Channel specific configuration information.  This structure should be
 *  populated by upper-layer software prior to calling @c ChannelSetup().  Any
 *  configuration item that can be changed on a per channel basis should
 *  be represented here.  Each module may define this structure with additional
 *  module-specific members through the Ext pointer.
 */
typedef struct
{
   int Channel;        /**< Channel number. */
   int Direction;      /**< Use the supplied #define DIRECTION_RX or DIRECTION_TX. */
   /**
    *  Per channel @c osReceive() function pointer.  See the @link OS_Functions OS Functions API
    *  @endlink documentation for complete information.
    */
   int   (*Receive)(OS_DEVICE *OsDev,FRAGLIST *FragList,bit32u FragCount,
                 bit32u PacketSize,HAL_RECEIVEINFO *HalReceiveInfo, bit32u Mode);
    /** May be used by each module to add additional channel configuration items.  If used,
        the documentation for such items will be located in the module specific appendices. */
   CHANNEL_INFO_EXT *Ext;
}CHANNEL_INFO;

/**
 *  @ingroup shared_data
 *  The HAL_FUNCTIONS struct defines the function pointers used by upper layer
 *  software.  The upper layer software receives these pointers through the
 *  call to cpswHal<module>InitModule().
 */
typedef struct
{
  int  (*ChannelSetup)     (HAL_DEVICE *HalDev, CHANNEL_INFO *Channel, OS_SETUP *OsSetup);
  int  (*ChannelTeardown)  (HAL_DEVICE *HalDev, int Channel, int Mode);
  int  (*Close)            (HAL_DEVICE *HalDev, int Mode);
  int  (*Control)          (HAL_DEVICE *HalDev, HAL_CONTROL_KEY Key, HAL_CONTROL_ACTION Action, void *Value);
  int  (*Open)             (HAL_DEVICE *HalDev);
  int  (*PacketProcessEnd) (HAL_DEVICE *HalDev);
  int  (*RxReturn)         (HAL_RECEIVEINFO *HalReceiveInfo, int StripFlag);
  int  (*Send)             (HAL_DEVICE *HalDev,FRAGLIST *FragList,int FragCount,int PacketSize, OS_SENDINFO *OsSendInfo,int (*SendComplete)(OS_SENDINFO *),bit32u Mode);
  int  (*Shutdown)         (HAL_DEVICE *HalDev);
  int  (*Tick)             (HAL_DEVICE *HalDev);
  /** Each module may define extra HAL functions through the Ext pointer.  See the appropriate module specific
      appendix for related documentation. */
  HAL_FUNCTIONS_EXT        *Ext;
} HAL_FUNCTIONS;


/**
 *  @ingroup shared_data
 *  The OS_FUNCTIONS struct defines the function pointers for all upper layer
 *  functions accessible to the CPHAL.  The upper layer software is responsible
 *  for providing the correct OS-specific implementations for the following
 *  functions. It is populated by calling InitModule() (done by the CPHAL in
 *  cpswHal<module>InitModule().
 */
typedef struct
{
  int   (*Control)                    (OS_DEVICE *OsDev, HAL_CONTROL_KEY Key, HAL_CONTROL_ACTION Action, void *Value);
  void  (*CriticalOn)                 (void);
  void  (*CriticalOff)                (void);
  void  (*DataCacheHitInvalidate)     (void *MemPtr, int Size);
  void  (*DataCacheHitWriteback)      (void *MemPtr, int Size);
  void  (*Free)                       (void *MemPtr);
  void  (*FreeRxBuffer)               (OS_RECEIVEINFO *OsReceiveInfo, void *MemPtr);
  void  (*FreeDev)                    (void *MemPtr);
  void  (*FreeDmaXfer)                (void *MemPtr);
  void  (*IsrRegister)                (OS_DEVICE *OsDev, int (*halISR)(HAL_DEVICE*, int*), int InterruptBit);
  void  (*IsrUnRegister)              (OS_DEVICE *OsDev, int InterruptBit);
  void* (*Malloc)                     (bit32u size);
  void* (*MallocDev)                  (bit32u Size);
  void* (*MallocDmaXfer)              (bit32u size, void *MemBase, bit32u MemRange);
  void* (*MallocRxBuffer)             (bit32u size, void *MemBase, bit32u MemRange,
                                       OS_SETUP *OsSetup, HAL_RECEIVEINFO *HalReceiveInfo,
                                       OS_RECEIVEINFO **OsReceiveInfo, OS_DEVICE *OsDev);
  int   (*Printf)                     (const char *Format, ...);
  int   (*Sprintf)                    (char *S, const char *Format, ...);
  void  (*TeardownComplete)           (OS_DEVICE *OsDev, int Ch, int Direction);
  /** The OS may define extra OS functions through the Ext pointer. */
  OS_FUNCTIONS_EXT *Ext;
} OS_FUNCTIONS;

#endif
