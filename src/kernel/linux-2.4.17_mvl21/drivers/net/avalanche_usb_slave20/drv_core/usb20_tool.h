/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 1998-2003 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission is hereby granted to licensees of Texas Instruments           |**
**| Incorporated (TI) products to use this computer program for the sole     |**
**| purpose of implementing a licensee product based on TI products.         |**
**| No other rights to reproduce, use, or disseminate this computer          |**
**| program, whether in part or in whole, are granted.                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/**************************************************************************
 * FILE PURPOSE :   Definitions for USB Admin Tool
 **************************************************************************
 * FILE NAME    :   usb20_tool.h
 *
 * DESCRIPTION  :
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/

#ifndef __USB20_USER_H_
#define __USB20_USER_H_

#include "usb20_config.h"

#define USB_TOOL_VERSION_MAJOR   0
#define USB_TOOL_VERSION_MINOR   1

typedef void (*USB_ADMIN_FUNC)(char **, int );
typedef void (*USB_ADMIN_HELP_FUNC)();

typedef struct _USB_ADMIN_CMD
{
     char  *name;                           /* command name */
     USB_ADMIN_FUNC   action;        /* command action routine */
     USB_ADMIN_HELP_FUNC help_action;
     char  *help_str;
} USB_ADMIN_CMD;



#endif  /* __USB20_USER_H_ */


