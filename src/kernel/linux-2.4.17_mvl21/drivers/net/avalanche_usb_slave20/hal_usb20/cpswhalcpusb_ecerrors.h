/**@file**********************************************************************/
/* Copyright(c) 2003, Texas Instruments Incorporated. All Rights Reserved.   */
/*                                                                           */
/* FILE:  cpswhalcpusb_ecerrors.h                                            */
/*                                                                           */
/* DESCRIPTION:                                                              */
/*  This file contains definitions and function declarations for             */
/*  error code support for the cpusb modul                                   */
/*                                                                           */
/* HISTORY:                                                                  */
/*  22Oct03 Menaka    Created errors file for cpusb                          */
/*****************************************************************************/
#ifndef _CPSWHALCPUSB_ECERRORS_H
#define _CPSWHALCPUSB_ECERRORS_H

#define CPUSB_EC_FUNC(ErrCode)    (EC_FUNC(0xff-ErrCode))
#define CPUSB_EC_ERR(ErrCode)     (EC_ERR(0xbff-ErrCode))


#define EC_FUNC_CHSETUPALL                     EC_FUNC(0xff) 
#define EC_FUNC_CTRLEPSETUP                    EC_FUNC(0xfe) 
#define EC_FUNC_EP0_READ                       EC_FUNC(0xfd) 
#define EC_FUNC_EP0_WRITE                      EC_FUNC(0xfc) 


#define EC_VAL_MODULE_IN_RST                  EC_ERR(0xbff) 
#define EC_VAL_USB_INVALID_BUF_OFFSET         EC_ERR(0xbfe) 
#define EC_VAL_USB_BUF_RAM_OVERFLOW           EC_ERR(0xbfd) 
#define EC_VAL_CTRLEP_CONFIG_APPLY            EC_ERR(0xbfc) 
#define EC_VAL_PARAMS_INVALID_ACTION          EC_ERR(0xbfb)
#define EC_VAL_OS_MALLOC_FAILED               EC_ERR(0xbfa)

/** @} */
#endif /* _CPSWHALCPUSB_ECERRORS */
