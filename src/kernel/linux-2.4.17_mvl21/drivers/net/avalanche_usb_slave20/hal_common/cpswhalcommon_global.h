/*************************************************************************
 *    TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 * 
 *  FILE:
 *
 *  DESCRIPTION:
 *   This file contains all commom CPHAL COntrol Keys
 *
 *  HISTORY:
 *  Date      Modifier           Notes
 *  04Jun04 Greg Guyotte         From original global.h
 *  11Jun04 Michael Hanrahan     Added Keys for enabling/disabling module Interrupt 
 ***************************************************************************/
#ifndef _cpswhalcommon_GLOBAL_H
#define _cpswhalcommon_GLOBAL_H

extern const char Version_COMMON[];

#define COMMON_KEY_START 10000

enum HAL_COMMON_KEY
{
 /**
 @defgroup Common_Config_Params Common Configuration Parameters

 This section documents the configuration parameters that are valid across
 all CPHAL devices.
 @{
 */

 hkVersionCommon=COMMON_KEY_START, 
 hkVersionCppi,  
 hkReset,           /**< @internal */
 hkLocator,         /**< @internal */
 hkVlynq,           /**< @internal */
 hkInst,            /**< @internal */
 hkStats,           /**< @internal */
 hkNULL,            /**< @internal */
 hkCpuFrequency,    /**< CPU Frequency. */
 hkId,              /**< Id string used in options.conf */
 hkBase,            /**< Base address for the module. */
 hkSize,            /**< Size of the register map for the module */
 hkResetBit,        /**< Reset bit for the module. */
 hkIntLine,         /**< Interrupt line for the module. */
 hkOffset,          /**< VLYNQ offset for the module.  Disregard if not using VLYNQ. */
 hkResetBase,       /**< Reset base address for the module. */
 hkDebug,           /**< This is the debug level.  The field is bit defined, such that the user
                        should set to 1 all the bits corresponding to desired debug outputs.  The following
                        are the meanings for each debug bit:
                        - bit0 (LSB): CPHAL Function Trace
                        - b1 : OS Function call trace
                        - b2 : Critical section entry/exit
                        - b3 : Memory allocation/destruction
                        - b4 : Detailed information in Rx path
                        - b5 : Detailed information in Tx path
                        - b6 : Extended error information
                        - b7 : General info */
 hkVersion,         /**< The OS may "Get" this parameter, which is a pointer
                        to a character string that indicates the version of CPHAL. */
 hkRxMaxChan,       /**< The maximum number of Rx channels supported by the module. */
 hkTxMaxChan,       /**< The maximum number of Tx channels supported by the module. */ 
 hkTxIntMaskClear,  /**< Disables the module's Tx Interrupt processing for specified Channel. */
 hkTxIntMaskSet,    /**< Enables  the module's Tx Interrupt processing for specified Channel. */
 hkTxCompleteCheck, /**< Enables checking for Tx Interrupts during Packet Send */
 hkTxNumQueues,     /**< Number of Tx queues supported by the module. */
 hkStatus,          /**< Used to determine the status of the module.  Currently used by the
                        CPMAC module only. */
 hkReady2Go,        /**< Used by the HAL to signal the OS that the system is ready to be taken
                        out of reset.  This gives the OS a window to perform any desired configuration changes
                        prior to the module being started.  Currently used by the CPMAC module only. */
 hkMdioPhyMode,     /**< Key to control Phy connect ability */
 hkMdioMdixSwitch,  /**< Controls setting of MDI/MDIX */
 hkMdixMask,        /**< Specifies which PHYs support AutoMidx */
 hkPhyMask,         /**< Controls PHY groupings */
 hkMLink,           /**< Specifies LINK usage per PHY */
 /* @} */

 /**
 @defgroup Common_Control_Params Common Keys for [os]Control()

 This section documents the keys used with the OS @c Control() interface that
 are required by CPHAL devices.

 @{
 */
 hkTick,            /**< The HAL will call the OS during initialization to configure the tick
                        timer using this key.  The associated value should be used as an interval to time OS
                        calls to the HAL Tick() function. Currently used by the CPMAC module only. */
 hkSleep,           /**< Used to wait for an integer number of cpu cycles.  No actions are defined. */
 hkStateChange,     /**< Used by the HAL to signal a change in state to the OS.  The OS should
                        then check the module status by performing an haGETbyREF action on the key hkStatus. */
 hkSioFlush,        /**< Requests the OS to flush it's I/O buffers.  No actions are defined. */
 /* @} */

 /**
 @defgroup Common_Control_Params_MAC_HDMA Other Configuration Keys

 This section documents keys which are currently used by the CPMAC
 and HDMA modules only.  This includes keys for accessing GPIO and
 CPMDIO fields, among others.

 @{ 
 */
 /* Mick, please document these keys. */
 hkGpio,
 hkGpioMux,
 hkGpioBase,
 hkGpioResetBit,
 hkCpmdio,
 hkCpmdioBase,
 hkCpmdioIntLine,
 hkCpmdioResetBit,
 hkMdioBusFrequency,
 hkMdioClockFrequency,
 hkPhyNum,           /**< Valid after Link seen. Returns the current PHY number. */
 hkPhyAccess,
 hkMacAddr,
 /** @} */

 /**
 @defgroup Common_CPPI_Params Configuration Parameters for CPPI Modules

 This section documents the configuration parameters that are valid across
 all CPPI-based CPHAL modules.
 @{ 
 */
 hkRxBufSize,        /**< Rx buffer size. */
 hkRxBufferOffset,   /**< Rx buffer offset. */
 hkRxNumBuffers,     /**< Number of Rx buffers to use. */
 hkRxServiceMax,     /**< Maximum number of Rx packets to service at once during an interrupt. */
 hkTxNumBuffers,     /**< Number of Tx buffer descriptors to use. */
 hkTxServiceMax,     /**< Maximum number of Tx packets to service at once during an interrupt. */
 hkRxMaxFrags,       /**< This is the maximum number of Rx fragments to support in the system.  The
                         default value is documented on a module-by-module basis.  */
 /** @} */

 /* Internal information, for debug only (do not generate documentation for these) */
 hkTxActQueueCount,
 hkRxActQueueCount,
 hkTxActQueueHead,
 hkRxActQueueHead,
 hkTxActQueueTail,
 hkRxActQueueTail,
 hkTxActive,
 hkRxActive,
 hkTcbPool,
 hkRcbPool,
 hkTcbStart,
 hkRcbStart,
 hkDumpParams,
 hkDio,
 hkPhyReset,

 /* Useful stats */
 hkRxCrcErrors,       /**< Number of RX CRC errors reported by the SAR and USB modules. This error is only
                          reported for AAL5,OAM and USB channels. */
 hkDmaLenErrors,      /**< Incremented if the sum of the fragment lengths differs from the packet length. */
 hkTxMisQCnt,         /**< Number of times the Tx controller needed to requeue a send request (not an error). */
 hkRxMisQCnt,         /**< Number of times the Rx controller needed to requeue a send request (not an error). */
 hkTxEOQCnt,          /**< Number of times the module reached the end of the Tx queue. */
 hkRxEOQCnt,          /**< Number of times the module reached the end of the Rx queue. */
 hkTxPacketsServiced, /**< Number of Tx packets serviced for a given channel and queue. */
 hkRxPacketsServiced, /**< Number of Rx packets serviced for a given channel. */
 hkTxMaxServiced,     /**< The maximum number of Tx packets ever serviced on a given channel and queue. */
 hkRxMaxServiced,     /**< The maximum number of Rx packets ever serviced on a given channel. */
 hkTxTotal,           /**< The total number of Tx packets serviced across all channels and queues. */
 hkRxTotal,           /**< The total number of Rx packets serviced across all channels. */
 hkLastTcbProcessed
};

#endif /*ends #ifndef _cpswhalcommon_GLOBAL_H*/





