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
**|         Copyright (c) 1998-2004 Texas Instruments Incorporated           |**
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

#ifndef _TITAN_CLK_CNTL_H_
#define _TITAN_CLK_CNTL_H_

/*****************************************************************************
 * Clock Control 
 *****************************************************************************/

#define CLK_MHZ(x)    ( (x) * 1000000 )

/* The order of ENUMs here should not be altered since
 * the register addresses are derived from the order
 */

typedef enum PAL_SYS_CLKC_ID_tag
{
    CLKC_VBUS = 0,  /* Clock driving VBUS */
    CLKC_MIPS,      /* Clock to MIPS */
    CLKC_USB,       /* Clock to USB slave */
    CLKC_LCD,       /* Clock to LCD controller */
    CLKC_VBUSP,      /* Clock to VBUSP */
    CLKC_SYS        /* Clock to SYS */
} PAL_SYS_CLKC_ID_T;

/**
 * This stucture is populated and passed on to the pal_sys_clkc_init function
 */
typedef struct PAL_SYS_Tnetv1050Init_tag
{
    UINT32 audclk;
    UINT32 refclk;
    UINT32 altclk;
    
} PAL_SYS_Tnetv1050Init;

#endif
