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

#ifndef _TITAN_BOARDS_H
#define _TITAN_BOARDS_H

/* Let us define board specific information here.  */

#if defined(CONFIG_MIPS_TNETV1050SDB) || defined(TNETV1050SDB)

#define AUDCLK_FREQ                                  8192000
#define REFCLK_FREQ                                 25000000
#define ALTCLK_FREQ                                 12000000

/* CPMAC with low base address */
#define AVALANCHE_LOW_CPMAC_PHY_MASK                0x40000000 
/* CPMAC with high base address*/ 
#define AVALANCHE_HIGH_CPMAC_PHY_MASK               0x80000000 

#define AVALANCHE_LOW_CPMAC_HAS_EXT_SWITCH          0
#define AVALANCHE_HIGH_CPMAC_HAS_EXT_SWITCH         0

#endif

#endif /* _TITAN_BOARDS_H */
