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
**|         Copyright (c) 2003 Texas Instruments Incorporated                |**
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

/******************************************************************************
 * FILE NAME:       psp_types.h
 *
 * DESCRIPTION:     Platform Support Package type definitions.
 *                  Intented to be used by HAL/Drivers etc.
 *
 * REVISION HISTORY:
 * 
 *
 * (C) Copyright 2003, Texas Instruments, Inc.
 *******************************************************************************/

#ifndef __PSP_TYPES_H__
#define __PSP_TYPES_H__



#include <linux/types.h>
#include <asm/avalanche/avalanche_map.h>
#include <asm/ioctl.h>


typedef void VOID;
typedef int (*FUNCPTR)(unsigned int,...);


#ifndef INLINE
#define INLINE /*__inline__ */
#endif




#if 0
/* Use for VxWorks */

#ifdef TORNADO2_0
#include "vxWorks.h"

#else
typedef char    INT8;
typedef short   INT16;
typedef int     INT32;

typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned int    UINT32;

typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
typedef unsigned int    UINT;
typedef unsigned long   ULONG;

typedef void    VOID;
typedef int     BOOL;
typedef int     STATUS;
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef NULL
#define NULL 0
#endif

#define INLINE /*__inline__ */

#endif  /* for VxWorks*/

#endif /*  __PSP_TYPES_H__ */
