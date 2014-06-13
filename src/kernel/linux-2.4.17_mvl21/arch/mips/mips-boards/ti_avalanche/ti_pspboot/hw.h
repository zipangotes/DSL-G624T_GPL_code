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
 * FILE PURPOSE:    SoC related definitions
 *******************************************************************************
 * FILE NAME:       hw.h
 *
 * DESCRIPTION:     SoC related definitions
 *
 ******************************************************************************/

#ifndef	_HW_H_
#define	_HW_H_

#ifdef _STANDALONE
#include <psbl/sysconf.h>
#else
#include "sysconf.h"
#endif

#define EMIF_SDRAM_BASE           0x94000000
#define EMIF_SDRAM_MAX_SIZE       0x08000000 /*0x08000000 or 0x01000000*/

#define ASCII_DISP_BASE           0xbc4001c3
#define ASCII_DISP_OFFSET         8


#if defined(SEAD2)  /* The board being used is SEAD2 */

#define SIO1__BASE     0xbc700000
#define SIO1__OFFSET   8
#define SIO1__EN       1
#define SIO1__FDEN     1
#define SIO1__RSTMASK  0x00
#define SIO0__BASE     0xa8610e00
#define SIO0__OFFSET   4
#define SIO0__EN       1
#define SIO0__FDEN     1
#define SIO0__RSTMASK  0x01

#else

#define SIO_OFFSET     4
#define SIO1_BASE     0xa8610f00
#define SIO1_RSTMASK  0x42
#define SIO0_BASE     0xa8610e00
#define SIO0_RSTMASK  0x01

#endif /* ifdef SEAD2 */

#define CS0_BASE 0xb0000000
#define CS0_SIZE CONF_FLASH_SZ

#define CS1_BASE 0xb4000000
#define CS1_SIZE CONF_SDRAM_SZ        

#define CS3_BASE 0xbc000000
#define CS3_SIZE (16*1024*1024)
        
#define CS4_BASE 0xbd000000

#ifdef TNETV1050SDB        
#define CS4_SIZE (8*1024*1024)
#else
#define CS4_SIZE (16*1024*1024)
#endif
        
#define CS5_BASE 0xbe000000
#define CS5_SIZE (16*1024*1024)

#define EMIF_BASE        0xA8610800
#define EMIF_REV        (*(volatile unsigned int*)(EMIF_BASE+0x00))
#define EMIF_GASYNC     (*(volatile unsigned int*)(EMIF_BASE+0x04))
#define EMIF_DRAMCTL    (*(volatile unsigned int*)(EMIF_BASE+0x08))
#define EMIF_REFRESH    (*(volatile unsigned int*)(EMIF_BASE+0x0c))
#define EMIF_ASYNC_CS0  (*(volatile unsigned int*)(EMIF_BASE+0x10))
#define EMIF_ASYNC_CS3  (*(volatile unsigned int*)(EMIF_BASE+0x14))
#define EMIF_ASYNC_CS4  (*(volatile unsigned int*)(EMIF_BASE+0x18))
#define EMIF_ASYNC_CS5  (*(volatile unsigned int*)(EMIF_BASE+0x1c))

#define GPIO_BASE     0xa8610900
#ifdef TNETV1050_BOARD
/* These defines are for GPIO EN,OUT and DAT 0 */
#define GPIO_EN         (*(volatile unsigned int *)(GPIO_BASE+0x18))
#define GPIO_OUT   		(*(volatile unsigned int *)(GPIO_BASE+0x8))
#define GPIO_DIR   		(*(volatile unsigned int *)(GPIO_BASE+0x10)) 
#else
#define GPIO_EN         (*(volatile unsigned int *)(GPIO_BASE+0xc))
#define GPIO_OUT   		(*(volatile unsigned int *)(GPIO_BASE+0x4))
#define GPIO_DIR   		(*(volatile unsigned int *)(GPIO_BASE+0x8)) 
#endif


#define RESET_BASE 0xa8611600
#define RESET_REG       (*(volatile unsigned *)(RESET_BASE+0x00))
#define RESET_STATUS    (*(volatile unsigned *)(RESET_BASE+0x08))

#ifdef TNETV1050SDB
#define SEC_FLASH_BASE          CS4_BASE
#define SEC_FLASH_SIZE          CS4_SIZE
#endif

#if defined (TNETD73XX_BOARD)
#define IS_OHIO_CHIP() (( (REG32_R( 0xA8610914,15,0 ) == 0x2b) || (REG32_R( 0xA8610914,15,0 ) == 0x18)) ? 1:0)
#define AVALANCHE_MII_SEL_REG         (0xa8611A08)
#endif
#endif /* _HW_H_ */
