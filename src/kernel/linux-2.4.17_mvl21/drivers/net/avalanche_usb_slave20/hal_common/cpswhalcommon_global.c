/******************************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE:
 *
 *  DESCRIPTION:
 *   This file contains all the global constructs for the CPSW HAL
 *
 *  HISTORY:
 *   Date      Modifier      Ver    Notes
 *  05Jun03 Michael Hanrahan 1.00   Original Version
 *****************************************************************************/

#include "cpswhalcommon_stddef.h"

HAL_CONTROL_KEY hkVersion_COMMON[] = "CPSWHAL COMMON 02.00.04.01 "__DATE__" "__TIME__;

HAL_CONTROL_KEY hkVersionCommon[]  = "vercommon";
HAL_CONTROL_KEY hkVersionCppi[]    = "vercppi";

/* Keys which do not need to appear in the document (unused by OS code) */
HAL_CONTROL_KEY hkReset[]              = "reset";
HAL_CONTROL_KEY hkLocator[]            = "locator";
HAL_CONTROL_KEY hkVlynq[]              = "vlynq";
HAL_CONTROL_KEY hkInst[]               = "inst";
HAL_CONTROL_KEY hkStats[]              = "Stats";
HAL_CONTROL_KEY hkNULL[]               = "";

/**
@defgroup Common_Config_Params Common Configuration Parameters

This section documents the configuration parameters that are valid across
all CPHAL devices.
@{
*/

/********************************************************************
**
**  Environment Parameters - Environment is case-sensitive,
**                           parameters do not need to match but it is desirable
**
********************************************************************/

/** @hideinitializer CPU Frequency. */
HAL_CONTROL_KEY hkCpuFrequency[]                 = "cpufrequency";

/********************************************************************
**
**  OPTIONS.CONF Parameters - Must match value in options.conf
**
********************************************************************/

/** @hideinitializer Id string used in options.conf */
HAL_CONTROL_KEY hkId[]                 = "id";
/** @hideinitializer Base address for the module. */
HAL_CONTROL_KEY hkBase[]               = "base";
/** @hideinitializer Size of the register map for the module */
HAL_CONTROL_KEY hkSize[]               = "size";
/** @hideinitializer Reset bit for the module. */
HAL_CONTROL_KEY hkResetBit[]           = "reset_bit";
/** @hideinitializer Interrupt line for the module. */
HAL_CONTROL_KEY hkIntLine[]            = "int_line";
/** @hideinitializer VLYNQ offset for the module.  Disregard if not using VLYNQ. */
HAL_CONTROL_KEY hkOffset[]             = "offset";
/** @hideinitializer Reset base address for the module. */
HAL_CONTROL_KEY hkResetBase[]          = "ResetBase";
/** @hideinitializer This is the debug level.  The field is bit defined, such that the user
should set to 1 all the bits corresponding to desired debug outputs.  The following
are the meanings for each debug bit:
- bit0 (LSB): CPHAL Function Trace
- b1 : OS Function call trace
- b2 : Critical section entry/exit
- b3 : Memory allocation/destruction
- b4 : Detailed information in Rx path
- b5 : Detailed information in Tx path
- b6 : Extended error information
- b7 : General info
*/
HAL_CONTROL_KEY hkDebug[]                        = "debug";
/** @hideinitializer The OS may "Get" this parameter, which is a pointer
    to a character string that indicates the version of CPHAL. */
HAL_CONTROL_KEY hkVersion[]                      = "version";
/** @hideinitializer The maximum number of Rx channels supported by the module. */
HAL_CONTROL_KEY hkRxMaxChan[]                    = "RxMaxChan";
/** @hideinitializer The maximum number of Tx channels supported by the module. */
HAL_CONTROL_KEY hkTxMaxChan[]                    = "TxMaxChan";
/** @hideinitializer Used to determine the status of the module.  Currently used by the
    CPMAC module only. */
HAL_CONTROL_KEY hkStatus[]                       = "Status";
/** @hideinitializer Used by the HAL to signal the OS that the system is ready to be taken
    out of reset.  This gives the OS a window to perform any desired configuration changes
    prior to the module being started.  Currently used by the CPMAC module only. */
HAL_CONTROL_KEY hkReady2Go[]                     = "Ready2Go";
/** @hideinitializer Key to control Phy connect ability */
HAL_CONTROL_KEY hkMdioPhyMode[]                  = "MdioPhyMode";
/** @hideinitializer Controls setting of MDI/MDIX */
HAL_CONTROL_KEY hkMdioMdixSwitch[]               = "MdioMDIX";
/** @hideinitializer Specifies which PHYs support AutoMidx */
HAL_CONTROL_KEY hkMdixMask[]                     = "MdixMask";
/** @hideinitializer Controls PHY groupings */
HAL_CONTROL_KEY hkPhyMask[]                      = "PhyMask";
/** @hideinitializer Specifies LINK usage per PHY */
HAL_CONTROL_KEY hkMLink[]                        = "MLink";

/* @} */

/**
@defgroup Common_Control_Params Common Keys for [os]Control()

This section documents the keys used with the OS @c Control() interface that
are required by CPHAL devices.

@{
*/

/** @hideinitializer The HAL will call the OS during initialization to configure the tick
    timer using this key.  The associated value should be used as an interval to time OS
    calls to the HAL Tick() function. Currently used by the CPMAC module only. */
HAL_CONTROL_KEY hkTick[]                         = "Tick";
/** @hideinitializer Used to wait for an integer number of cpu cycles.  No actions are defined. */
HAL_CONTROL_KEY hkSleep[]                        = "Sleep";
/** @hideinitializer Used by the HAL to signal a change in state to the OS.  The OS should
    then check the module status by performing an haGETbyREF action on the key hkStatus. */
HAL_CONTROL_KEY hkStateChange[]                  = "StateChange";
/** @hideinitializer Requests the OS to flush it's I/O buffers.  No actions are defined. */
HAL_CONTROL_KEY hkSioFlush[]                     = "SioFlush";
/* @} */


/**
@defgroup Common_Control_Params_MAC_HDMA Other Configuration Keys

This section documents keys which are currently used by the CPMAC
and HDMA modules only.  This includes keys for accessing GPIO and
CPMDIO fields, among others.

@{
*/

/* Mick, please document these keys. */
HAL_CONTROL_KEY hkGpio[]                         = "gpio";
HAL_CONTROL_KEY hkGpioMux[]                      = "gpio_mux";
HAL_CONTROL_KEY hkGpioBase[]                     = "GpioBase";
HAL_CONTROL_KEY hkGpioResetBit[]                 = "GpioReset";

HAL_CONTROL_KEY hkCpmdio[]                       = "cpmdio";
HAL_CONTROL_KEY hkCpmdioBase[]                   = "CpmdioBase";
HAL_CONTROL_KEY hkCpmdioIntLine[]                = "CpmdioInt";
HAL_CONTROL_KEY hkCpmdioResetBit[]               = "CpmdioReset";
HAL_CONTROL_KEY hkMdioBusFrequency[]             = "MdioBusFrequency";
HAL_CONTROL_KEY hkMdioClockFrequency[]           = "MdioClockFrequency";
/** @hideinitializer Valid after Link seen. Returns the current PHY number. */
HAL_CONTROL_KEY hkPhyNum[]                       = "PhyNum";
HAL_CONTROL_KEY hkPhyAccess[]                    = "PhyAccess";
HAL_CONTROL_KEY hkMacAddr[]                      = "MacAddr";

/** @} */

/* Below are all the CPPI specific keys */
/* Configurable items */

/**
@defgroup Common_CPPI_Params Configuration Parameters for CPPI Modules

This section documents the configuration parameters that are valid across
all CPPI-based CPHAL modules.
@{
*/
/** @hideinitializer Rx buffer size. */
HAL_CONTROL_KEY hkRxBufSize[]      = "RxBufSize";
/** @hideinitializer Rx buffer offset. */
HAL_CONTROL_KEY hkRxBufferOffset[] = "RxBufferOffset";
/** @hideinitializer Number of Rx buffers to use. */
HAL_CONTROL_KEY hkRxNumBuffers[]   = "RxNumBuffers";
/** @hideinitializer Maximum number of Rx packets to service at once during an interrupt. */
HAL_CONTROL_KEY hkRxServiceMax[]   = "RxServiceMax";
/** @hideinitializer Number of Tx buffer descriptors to use. */
HAL_CONTROL_KEY hkTxNumBuffers[]   = "TxNumBuffers";
/** @hideinitializer Maximum number of Tx packets to service at once during an interrupt. */
HAL_CONTROL_KEY hkTxServiceMax[]   = "TxServiceMax";
/** @hideinitializer This is the maximum number of Rx fragments to support in the system.  The
    default value is documented on a module-by-module basis.  */
HAL_CONTROL_KEY hkRxMaxFrags[]     = "RxMaxFrags";

/** @} */
/* Internal information, for debug only (do not generate documentation for these) */

HAL_CONTROL_KEY hkTxActQueueCount[]   ="TxActQueueCount";
HAL_CONTROL_KEY hkRxActQueueCount[]   ="RxActQueueCount";
HAL_CONTROL_KEY hkTxActQueueHead[]    ="TxActQueueHead";
HAL_CONTROL_KEY hkRxActQueueHead[]    ="RxActQueueHead";
HAL_CONTROL_KEY hkTxActQueueTail[]    ="TxActQueueTail";
HAL_CONTROL_KEY hkRxActQueueTail[]    ="RxActQueueTail";
HAL_CONTROL_KEY hkTxActive[]          ="TxActive";
HAL_CONTROL_KEY hkRxActive[]          ="RxActive";
HAL_CONTROL_KEY hkTcbPool[]           ="TcbPool";
HAL_CONTROL_KEY hkRcbPool[]           ="RcbPool";
HAL_CONTROL_KEY hkTcbStart[]          ="TcbStart";
HAL_CONTROL_KEY hkRcbStart[]          ="RcbStart";
HAL_CONTROL_KEY hkDumpParams[]        ="DumpParams";

/* Useful stats */
/** @hideinitializer Number of RX CRC errors reported by the SAR and USB modules. This error is only
    reported for AAL5,OAM and USB channels. */
HAL_CONTROL_KEY hkRxCrcErrors[]="RxCrcErrors";
/** @hideinitializer Incremented if the sum of the fragment lengths differs from the packet length. */
HAL_CONTROL_KEY hkDmaLenErrors[]="DmaLenErrors";
/** @hideinitializer Number of times the Tx controller needed to requeue a send request (not an error). */
HAL_CONTROL_KEY hkTxMisQCnt[]="TxMisQCnt";
/** @hideinitializer Number of times the Rx controller needed to requeue a send request (not an error). */
HAL_CONTROL_KEY hkRxMisQCnt[]="RxMisQCnt";
/** @hideinitializer Number of times the module reached the end of the Tx queue. */
HAL_CONTROL_KEY hkTxEOQCnt[]="TxEOQCnt";
/** @hideinitializer Number of times the module reached the end of the Rx queue. */
HAL_CONTROL_KEY hkRxEOQCnt[]="RxEOQCnt";
/** @hideinitializer Number of Tx packets serviced for a given channel and queue. */
HAL_CONTROL_KEY hkTxPacketsServiced[]="TxPacketsServiced";
/** @hideinitializer Number of Rx packets serviced for a given channel. */
HAL_CONTROL_KEY hkRxPacketsServiced[]="RxPacketsServiced";
/** @hideinitializer The maximum number of Tx packets ever serviced on a given channel and queue. */
HAL_CONTROL_KEY hkTxMaxServiced[]="TxMaxServiced";
/** @hideinitializer The maximum number of Rx packets ever serviced on a given channel. */
HAL_CONTROL_KEY hkRxMaxServiced[]="RxMaxServiced";
/** @hideinitializer The total number of Tx packets serviced across all channels and queues. */
HAL_CONTROL_KEY hkTxTotal[]="TxTotal";
/** @hideinitializer The total number of Rx packets serviced across all channels. */
HAL_CONTROL_KEY hkRxTotal[]="RxTotal";
HAL_CONTROL_KEY hkLastTcbProcessed[]="LastTcbProcessed";

/* Internal */
HAL_CONTROL_KEY hkDio[]                = "Dio";
HAL_CONTROL_KEY hkPhyReset[]           = "rp";
