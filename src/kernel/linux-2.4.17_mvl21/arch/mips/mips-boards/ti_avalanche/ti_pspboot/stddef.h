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
**|         Copyright (c) 1998-2005 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    Standard definitions
 *******************************************************************************
 * FILE NAME:       stddef.h
 *
 * DESCRIPTION:     Standard definitions
 *
 ******************************************************************************/

#ifndef ___STDDEF_H___
#define ___STDDEF_H___

typedef char           bit8;
typedef short          bit16;
typedef int            bit32;

typedef unsigned char  bit8u;
typedef unsigned short bit16u;
typedef unsigned int   bit32u;
#ifdef _STANDALONE
typedef char           INT8;
typedef short          INT16;
typedef int            INT32;

typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned int   UINT32;

typedef unsigned int   size_t;

typedef unsigned char  BOOL;
#ifndef __cplusplus
typedef unsigned char  bool;
#endif
#endif

typedef int (*FUNC_PTR)(int, char**);

#ifndef TRUE
#define TRUE (1==1)
#endif

#ifndef FALSE
#define FALSE (1==2)
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef EOF
#define EOF    (-1)
#endif



#endif /* ___STDDEF_H___ */

