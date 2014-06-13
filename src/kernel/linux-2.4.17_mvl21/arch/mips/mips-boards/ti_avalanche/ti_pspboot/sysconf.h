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
 * FILE PURPOSE:    PSPboot configuration parameters.
 *******************************************************************************
 * FILE NAME:       sysconf.h
 *
 * DESCRIPTION:     PSPboot configuration parameters.
 *
 ******************************************************************************/

#ifndef _SYSCONF_H_
#define _SYSCONF_H_

#define MEG(x)      ((x) << 20)
#define MHZ(x)      ((x) * 1000 * 1000)
#define KHZ(x)      ((x) * 1000)

#define CACHE_WT     1
#define CACHE_NONE   2
#define CACHE_WB     3

/* Pre-processor macro to get the board name string into the code
 * This depends of BOARD=XXX and XXX=XXX being defined externally 
 * (probably through a makefile). The BOARDNAME macro will return the 
 * string "XXX"
 */
#define BRDSTR(E)   #E
#define MKSTR(X)    BRDSTR(X)
#define BOARDNAME   MKSTR(BOARD)

/* Currently they are completely the same */
#if defined( AR7VW )
#define AR7VWi
#endif

/* Currently they are completely the same except of the External PHY - connecting to port 0 */
#if defined( TNETV1060SDB_WR )
#define TNETV1050SDB
#define EXTERNAL_PHY_PC_PORT0
#endif

/* Cable Modem 401B board: Avalanche I SoC */
#if defined (TNETC401B) 

#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* Cable Modem 520 board: Puma S SoC */
#elif defined (TNETC520)

#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* Cable Modem 620 board: Puma S SoC */
#elif defined (TNETC620)

#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(8)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* Cable Modem 420 board: Puma S SoC */
#elif defined (TNETC420)

#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(2)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* Cable Modem 405T board: Avalanche D SoC */
#elif defined (TNETC405T)

#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(2)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* AR7 Verification and Debug Board. */
#elif defined(AR7VDB)   
#ifndef TNETD73XX_BOARD
#define TNETD73XX_BOARD
#endif
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   KHZ(35328)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(32)
#define CONF_FLASH_SZ       MEG(16)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WB

/* AR7 development boards */
#elif defined(AR7DB) 
#ifndef TNETD73XX_BOARD
#define TNETD73XX_BOARD
#endif
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   KHZ(35328)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WB

#elif defined(AR7RD) 
#ifndef TNETD73XX_BOARD
#define TNETD73XX_BOARD
#endif
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   KHZ(35328)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       IS_OHIO_CHIP() ? MHZ(212) : MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(2)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

#elif defined(AR7WRD) 
#ifndef TNETD73XX_BOARD
#define TNETD73XX_BOARD
#endif
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   KHZ(35328)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       IS_OHIO_CHIP() ? MHZ(212) : MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

#elif defined(AR7Wi)
#ifndef TNETD73XX_BOARD
#define TNETD73XX_BOARD
#endif
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   KHZ(35328)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

#elif defined(AR7V)
#ifndef TNETD73XX_BOARD
#define TNETD73XX_BOARD
#endif
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   KHZ(35328)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

#elif defined (AR7VWi)
#ifndef TNETD73XX_BOARD
#define TNETD73XX_BOARD
#endif
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   KHZ(35328)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       IS_OHIO_CHIP() ? MHZ(212) : MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

#elif defined (AR7L0)
#ifndef TNETD73XX_BOARD
#define TNETD73XX_BOARD
#endif
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   KHZ(35328)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       IS_OHIO_CHIP() ? MHZ(212) : MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(2)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB    

/* TNETV1050 Verification and Debug Board */
#elif defined(TNETV1050VDB)   
#define TNETV1050_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AUDIO_FREQ     KHZ(8192)
#define CONF_ALT_FREQ       MHZ(12)
#define CONF_CPU_FREQ       KHZ(162500)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(64)
#define CONF_FLASH_SZ       MEG(16)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WT

/* TNETV1050 Software Development Board */
#elif defined(TNETV1050SDB)   
#define TNETV1050_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AUDIO_FREQ     KHZ(8192)
#define CONF_ALT_FREQ       MHZ(12)
#define CONF_CPU_FREQ       KHZ(162500)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(64)
#define CONF_FLASH_SZ       MEG(8)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WB

/* TNETV1050 Reference Design Board */
#elif defined(TNETV1050RDB)   
#define TNETV1050_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AUDIO_FREQ     KHZ(8192)
#define CONF_ALT_FREQ       MHZ(12)
#define CONF_CPU_FREQ       KHZ(162500)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(8)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WB

/* Wireless LAN Software Development AP */
#elif defined(WLAN)   
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       FWBGet_flash_type()
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* Wireless LAN Gateway Cable Modem */
#elif defined(TNETC621)   
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       FWBGet_flash_type()
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* MIPS SEAD2 emulation platform */
#elif defined(SEAD2)   
#define CONF_REFXTAL_FREQ   MHZ(32)
#define CONF_CPU_FREQ       MHZ(32)
#define CONF_SYS_FREQ       MHZ(32)
#define CONF_SDRAM_SZ       MEG(32)
#define CONF_FLASH_SZ       MEG(32)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WT

/* Apex Verification and debug board */
#elif defined(TNETV1020VDB)   
#define TNETV1020_BOARD
#define CONF_XTAL1IN_FREQ   MHZ(25)
#define CONF_XTAL2IN_FREQ   MHZ(25)
#define CONF_OSCIN_FREQ     MHZ(25)
#define CONF_CPU_FREQ       MHZ(275)
#define CONF_SYS_FREQ       KHZ(137500)
#define CONF_SDRAM_SZ       MEG(32)
#define CONF_FLASH_SZ       MEG(16)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WB

/* WLAN Apex board */
#elif defined(TNETWA115VAG)   
#define TNETV1020_BOARD
#define CONF_XTAL1IN_FREQ   MHZ(25)
#define CONF_XTAL2IN_FREQ   MHZ(25)
#define CONF_OSCIN_FREQ     MHZ(25)
#define CONF_CPU_FREQ       MHZ(250)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

/* PUMA-3 VDB board */
#elif defined(TNETC4602MF)   
#define TNETC4602_BOARD
#define CONF_REFXTAL_FREQ   MHZ(30)
#define CONF_XTAL1IN_FREQ   MHZ(30)
#define CONF_XTAL2IN_FREQ   MHZ(30)
#define CONF_OSCIN_FREQ     MHZ(30)
#define CONF_CPU_FREQ       MHZ(210)
#define CONF_SYS_FREQ       MHZ(105)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(2)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

/* Cable modem TNETC430 Puma-3 based board */
#elif defined(TNETC430)   
#define TNETC4602_BOARD
#define CONF_REFXTAL_FREQ   MHZ(30)
#define CONF_XTAL1IN_FREQ   MHZ(30)
#define CONF_XTAL2IN_FREQ   MHZ(30)
#define CONF_OSCIN_FREQ     MHZ(30)
#define CONF_CPU_FREQ       MHZ(210)
#define CONF_SYS_FREQ       MHZ(105)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(2)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

/* Cable modem TNETC730 Puma-3 based board */
#elif defined(TNETC730)   
#define TNETC4602_BOARD
#define CONF_REFXTAL_FREQ   MHZ(30)
#define CONF_XTAL1IN_FREQ   MHZ(30)
#define CONF_XTAL2IN_FREQ   MHZ(30)
#define CONF_OSCIN_FREQ     MHZ(30)
#define CONF_CPU_FREQ       MHZ(210)
#define CONF_SYS_FREQ       MHZ(105)
#define CONF_SDRAM_SZ       MEG(32)
#define CONF_FLASH_SZ       MEG(8)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

#else
#error "Fatal Error: No board type defined"
#endif /* ifdef AVALANCHE_EVM3 */

#endif /* _SYSCONF_H_ */
