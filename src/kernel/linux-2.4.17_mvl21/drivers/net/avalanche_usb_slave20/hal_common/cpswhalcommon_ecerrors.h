/**@file**********************************************************************/
/* Copyright(c) 2001, Texas Instruments Incorporated. All Rights Reserved.   */
/*                                                                           */
/* FILE:  cpswhalcommon_ec_errors.h                                                        */
/*                                                                           */
/* DESCRIPTION:                                                              */
/*  This file contains definitions and function declarations for             */
/*  error code support.                                                      */
/*                                                                           */
/* HISTORY:                                                                  */
/*  14Dec00 MJH       Added masking to EC_CLASS etc macros                   */
/*  17Sep02 GSG       Added HAL support (new class&devices)                  */
/*****************************************************************************/
#ifndef _CPSWHAL_ECERRORS_H
#define _CPSWHAL_ECERRORS_H

/**
@page Error_Codes

All CPHAL functions return a 32-bit value which indicates the error code for the
function.  The errors may be decoded by the driver according to the following
bit-mapping:

- 31 (MSB): CRITICAL (if set indicates a critical error - reset required)
- 30 - 28 : INSTANCE (instance number of the device)
- 27 - 20 : DEVICE (identifies the device, i.e. CPMAC, AAL5, etc.)
- 19 - 12 : FUNCTION (identifies the function returning the error)
- 11 - 00 : ERROR CODE (up to 4096 separate error codes)

More information on the field content is given in the sections below.  In the future,
new additions to any of the fields may occur, but the values will not be changed,
allowing any code which interprets these errors to operate as expected.

@latexonly
\input{group__Error__Defines}
\input{group__Device__Codes}
\input{group__Function__Codes}
\input{group__Error__Codes}
@endlatexonly
*/

/*---------------------------------------------------------------------------*/
/* Useful defines for accessing fields within error code                     */
/*---------------------------------------------------------------------------*/

/**
@defgroup Error_Defines Useful Error Code Macros


The error code macros are defined to allow simple access to the
error code bit fields.  To parse the error code returned by a
HAL function, pass it to one of the defined macros.  For example,
to retrieve the error number, use EC_GET_ERR(code), where 'code'
is the error code returned by a HAL function.  Compare the
value to the error code values in this section, or in the module
specific documentation, to determine the cause of the error.
@{
*/
#define EC_NO_ERRORS       0    /* Indicates no error was seen */
#define CRITICAL_SHIFT    31
#define INSTANCE_SHIFT    28
#define DEVICE_SHIFT      20
#define FUNCTION_SHIFT    12
#define ERROR_CODE_SHIFT   0

#define CRITICAL_MASK     1
#define INSTANCE_MASK     0x7
#define DEVICE_MASK       0xFF
#define FUNCTION_MASK     0xFF
#define ERROR_CODE_MASK   0xFFF

/** Use this macro to determine if the error is critical, requiring a system reset. */
#define EC_GET_ERR_CRITICAL(code)    ((code&(CRITICAL_MASK << CRITICAL_SHIFT)) >> CRITICAL_SHIFT)
/** Use this macro to get the instance of the device reporting an error. */
#define EC_GET_ERR_INSTANCE(code)    ((code&(INSTANCE_MASK << INSTANCE_SHIFT)) >> INSTANCE_SHIFT)
/** Use this macro to determine the identity of the device reporting an error. */
#define EC_GET_ERR_DEVICE(code)      ((code&(DEVICE_MASK << DEVICE_SHIFT)) >> DEVICE_SHIFT)
/** Use this macro to determine which internal HAL function reported the error. */
#define EC_GET_ERR_FUNC(code)        ((code&(FUNCTION_MASK << FUNCTION_SHIFT)) >> FUNC_SHIFT)
/** Use this macro to determine to precise error code indication. */
#define EC_GET_ERR(code)             (code&(ERROR_CODE_MASK))
/** @} */

/* These macros are used internally in the HAL */
#define EC_CRITICAL       (CRITICAL_MASK           << CRITICAL_SHIFT)
#define EC_INSTANCE(val)  ((val & INSTANCE_MASK)   << INSTANCE_SHIFT)
#define EC_DEVICE(val)    ((val & DEVICE_MASK)     << DEVICE_SHIFT)
#define EC_FUNC(val)      ((val & FUNCTION_MASK)   << FUNCTION_SHIFT)
#define EC_ERR(val)       ((val & ERROR_CODE_MASK) << ERROR_CODE_SHIFT)

/*---------------------------------------------------------------------------*/
/* Device types                                                              */
/*---------------------------------------------------------------------------*/

/**
@defgroup Device_Codes CPHAL Device Codes

These are the device codes possible for the DEVICE field.  The value
indicates which device returned the error.
@{
*/
#define EC_DEV_CPSAR                           EC_DEVICE(0x1A)
#define EC_DEV_AAL5                            EC_DEVICE(0x1B)
#define EC_DEV_AAL2                            EC_DEVICE(0x1C)
#define EC_DEV_CPMAC                           EC_DEVICE(0x1D)
#define EC_DEV_VDMA                            EC_DEVICE(0x1E)
#define EC_DEV_VLYNQ                           EC_DEVICE(0x1F)
#define EC_DEV_CPPI                            EC_DEVICE(0x20)
#define EC_DEV_CPMDIO                          EC_DEVICE(0x21)
#define EC_DEV_HDMA                            EC_DEVICE(0x22)
#define EC_DEV_ESWITCH                         EC_DEVICE(0x23)
#define EC_DEV_CPUSB                           EC_DEVICE(0x24)
#define EC_DEV_CPOTG                           EC_DEVICE(0x25)
/** @} */


/*---------------------------------------------------------------------------*/
/* Function types                                                            */
/*---------------------------------------------------------------------------*/

/**
@defgroup Function_Codes CPHAL Function Codes

These are the function codes possible for the FUNCTION field.
This value indicates the function in which the error occurred.
@{
*/
#define EC_FUNC_HAL_INIT                       EC_FUNC(0x01)
#define EC_FUNC_CHSETUP                        EC_FUNC(0x02)
#define EC_FUNC_CHTEARDOWN                     EC_FUNC(0x03)
#define EC_FUNC_RXRETURN                       EC_FUNC(0x04)
#define EC_FUNC_SEND                           EC_FUNC(0x05)
#define EC_FUNC_RXINT                          EC_FUNC(0x06)
#define EC_FUNC_TXINT                          EC_FUNC(0x07)
/* BLANK - fill with future common function codes*/
#define EC_FUNC_OPTIONS                        EC_FUNC(0x09)
#define EC_FUNC_PROBE                          EC_FUNC(0x0A)
#define EC_FUNC_OPEN                           EC_FUNC(0x0B)
#define EC_FUNC_CONTROL                        EC_FUNC(0x0C)
#define EC_FUNC_DEVICE_INT                     EC_FUNC(0x0D)
#define EC_FUNC_STATUS                         EC_FUNC(0x0E)
#define EC_FUNC_TICK                           EC_FUNC(0x0F)
#define EC_FUNC_CLOSE                          EC_FUNC(0x10)
#define EC_FUNC_SHUTDOWN                       EC_FUNC(0x11)
/* BLANK - fill with future common function codes*/
/* BLANK - fill with future common function codes*/
/* BLANK - fill with future common function codes*/
/* BLANK - fill with future common function codes*/
/* BLANK - fill with future common function codes*/
/* BLANK - fill with future common function codes*/
#define EC_FUNC_READ_CONF                      EC_FUNC(0x18)
#define EC_FUNC_CHECK_ALL_PARAMS               EC_FUNC(0x19)
#define EC_FUNC_INIT_PARAMS                    EC_FUNC(0x20)
#define EC_FUNC_PARAMS_GET_ENV                 EC_FUNC(0x21)
#define EC_FUNC_CONFIG_GET                     EC_FUNC(0x22)
/** @} */

/*
HAL Error Codes.  The list below defines every type of error
used in all HAL modules. DO NOT CHANGE THESE VALUES!  Add new
values in integer order to the bottom of the list.
*/
/**
@defgroup Error_Codes CPHAL Error Codes

These are the error codes possible for the ERROR CODE field.
This value indicates the actual error that has occurred.  Critical errors
are OR'd with the EC_CRITICAL macro below.  These error code values
will never change, but new codes may be defined and added to the list
in the future.
@{
*/
/* BLANK - fill with future common error codes*/
#define EC_VAL_NO_RESET                        EC_ERR(0x002)
#define EC_VAL_DEVICE_NOT_FOUND                EC_ERR(0x003)|EC_CRITICAL
#define EC_VAL_BASE_ADDR_NOT_FOUND             EC_ERR(0x004)|EC_CRITICAL
#define EC_VAL_RESET_BIT_NOT_FOUND             EC_ERR(0x005)|EC_CRITICAL
#define EC_VAL_CH_INFO_NOT_FOUND               EC_ERR(0x006)
#define EC_VAL_RX_STATE_RAM_NOT_CLEARED        EC_ERR(0x007)|EC_CRITICAL
#define EC_VAL_TX_STATE_RAM_NOT_CLEARED        EC_ERR(0x008)|EC_CRITICAL
#define EC_VAL_MALLOC_DEV_FAILED               EC_ERR(0x009)
#define EC_VAL_OS_VERSION_NOT_SUPPORTED        EC_ERR(0x00A)|EC_CRITICAL
/* BLANK - fill with future common error codes*/
/* BLANK - fill with future common error codes*/
/* BLANK - fill with future common error codes*/
#define EC_VAL_INVALID_CH                      EC_ERR(0x00E)
#define EC_VAL_NULL_CH_STRUCT                  EC_ERR(0x00F)
#define EC_VAL_RX_TEARDOWN_ALREADY_PEND        EC_ERR(0x010)
#define EC_VAL_TX_TEARDOWN_ALREADY_PEND        EC_ERR(0x011)
#define EC_VAL_RX_CH_ALREADY_TORNDOWN          EC_ERR(0x012)
#define EC_VAL_TX_CH_ALREADY_TORNDOWN          EC_ERR(0x013)
#define EC_VAL_TX_TEARDOWN_TIMEOUT             EC_ERR(0x014)
#define EC_VAL_RX_TEARDOWN_TIMEOUT             EC_ERR(0x015)
#define EC_VAL_CH_ALREADY_TORNDOWN             EC_ERR(0x016)
#define EC_VAL_INVALID_STATE                   EC_ERR(0x017)
#define EC_VAL_INVALID_STATE_NOT_CONNECTED     EC_ERR(0x018)
#define EC_VAL_INVALID_STATE_NOT_DEVICE_FOUND  EC_ERR(0x019)
#define EC_VAL_INVALID_STATE_NOT_INITIALISED   EC_ERR(0x01A)
#define EC_VAL_INVALID_STATE_NOT_OPENED        EC_ERR(0x01B)
/* BLANK - fill with future common error codes*/
/* BLANK - fill with future common error codes*/
#define EC_VAL_CH_NOT_SETUP                    EC_ERR(0x01E)
#define EC_VAL_CH_ALREADY_OPEN                 EC_ERR(0x01F)
#define EC_VAL_RCB_MALLOC_FAILED               EC_ERR(0x020)
#define EC_VAL_RX_BUFFER_MALLOC_FAILED         EC_ERR(0x021)
#define EC_VAL_OUT_OF_TCBS                     EC_ERR(0x022)
#define EC_VAL_NO_TCBS                         EC_ERR(0x023)
#define EC_VAL_NULL_RCB                        EC_ERR(0x024)|EC_CRITICAL
#define EC_VAL_SOP_ERROR                       EC_ERR(0x025)|EC_CRITICAL
#define EC_VAL_EOP_ERROR                       EC_ERR(0x026)|EC_CRITICAL
#define EC_VAL_NULL_TCB                        EC_ERR(0x027)|EC_CRITICAL
#define EC_VAL_CORRUPT_RCB_CHAIN               EC_ERR(0x028)|EC_CRITICAL
#define EC_VAL_TCB_MALLOC_FAILED               EC_ERR(0x029)
#define EC_VAL_DISABLE_POLLING_FAILED          EC_ERR(0x02A)
#define EC_VAL_KEY_NOT_FOUND                   EC_ERR(0x02B)
#define EC_VAL_MALLOC_FAILED                   EC_ERR(0x02C)
#define EC_VAL_RESET_BASE_NOT_FOUND            EC_ERR(0x02D)|EC_CRITICAL
/* BLANK - fill with future common error codes*/
#define EC_VAL_NO_TXH_WORK_TO_DO               EC_ERR(0x02F)
#define EC_VAL_NO_TXL_WORK_TO_DO               EC_ERR(0x030)
#define EC_VAL_NO_RX_WORK_TO_DO                EC_ERR(0x031)
#define EC_VAL_NOT_LINKED                      EC_ERR(0x032)
#define EC_VAL_INTERRUPT_NOT_FOUND             EC_ERR(0x033)
#define EC_VAL_OFFSET_NOT_FOUND                EC_ERR(0x034)
#define EC_VAL_MODULE_ALREADY_CLOSED           EC_ERR(0x035)
#define EC_VAL_MODULE_ALREADY_SHUTDOWN         EC_ERR(0x036)
#define EC_VAL_ACTION_NOT_FOUND                EC_ERR(0x037)
#define EC_VAL_RX_CH_ALREADY_SETUP             EC_ERR(0x038)
#define EC_VAL_TX_CH_ALREADY_SETUP             EC_ERR(0x039)
#define EC_VAL_RX_CH_ALREADY_OPEN              EC_ERR(0x03A)
#define EC_VAL_TX_CH_ALREADY_OPEN              EC_ERR(0x03B)
#define EC_VAL_CH_ALREADY_SETUP                EC_ERR(0x03C)
#define EC_VAL_RCB_NEEDS_BUFFER                EC_ERR(0x03D) /* +GSG 030410 */
#define EC_VAL_RCB_DROPPED                     EC_ERR(0x03E) /* +GSG 030410 */
#define EC_VAL_PARAM_NOT_WRITABLE              EC_ERR(0x03F)
/* BLANK - fill with future common error codes*/
/* BLANK - fill with future common error codes*/
/* BLANK - fill with future common error codes*/
#define EC_VAL_OUT_OF_RANGE                    EC_ERR(0x043)
#define EC_VAL_INVALID_ACTION                  EC_ERR(0x044)
/** @} */
#endif /* #define for include */
