/*
 * Jeff Harrell, jharrell@ti.com
 * Copyright (C) 2001 Texas Instruments, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Texas Instruments Sangam specific setup.
 */
#include <linux/config.h>
#include <asm/avalanche/generic/pal.h>


void prom_printf(char *fmt, ...);
void avalanche_set_vbus_freq(unsigned int new_vbus_freq);
void  vlynq_dev_init(void);
unsigned int avalanche_get_vbus_freq(void);

#include <asm/avalanche/generic/pal_vlynq.h>
#include <linux/proc_fs.h>

extern SET_MDIX_ON_CHIP_FN_T p_set_mdix_on_chip_fn;

#define AUTO_MDIX_ENABLE_PIN        30
#define MDIX_NORMAL_MODE            0

#define AUTO_MDIX_GPIO_PIN          28
#define MDIX_MODE                   1
#define MDI_MODE                    0

#define AUTO_MDIX_CNTL_MASK        ( (1 << AUTO_MDIX_ENABLE_PIN) \
                                    |(1 << AUTO_MDIX_GPIO_PIN))
/* We ignore MDIX_NORMAL_MODE as it is 0 and we get to set 0 from the below
   macros for the AUTO_MDIX_ENABLE_PIN. */
#define AUTO_MDIX_ON_WORD          (MDIX_MODE << AUTO_MDIX_GPIO_PIN)
#define AUTO_MDIX_OFF_WORD         (MDI_MODE << AUTO_MDIX_GPIO_PIN)

int sangam_set_mdix_on_chip(unsigned int base_addr, unsigned int operation)
{
    if((!p_set_mdix_on_chip_fn) || (base_addr != AVALANCHE_LOW_CPMAC_BASE))
        return (-1);

    if(operation)
        /* 0perating on bit 29 and 30. Writing 1 to bit 29 and 0 to bit 30. */
        PAL_sysGpioOutValue(AUTO_MDIX_ON_WORD, AUTO_MDIX_CNTL_MASK,0);
    else
        /* 0perating on bit 29 and 30. Writing 0 to bit 29 and 0 to bit 30. */
        PAL_sysGpioOutValue(AUTO_MDIX_OFF_WORD, AUTO_MDIX_CNTL_MASK,0);

    return (0);
}


void ar7_delay(int milisec)
{
#define MAX_CPU_FREQUENCY_IN_MHZ        150

    volatile unsigned int tick= milisec * 1000 * MAX_CPU_FREQUENCY_IN_MHZ;

    while(tick--);

}

int avalanche_detect_sangam_cpmac_phy_setup(void)
{

#define SELECT_INT_PHY_MAC 0x00001
#define SELECT_EXT_PHY_MAC 0x00002
#define SELECT_EXT_SWITCH  0x10000

    volatile unsigned long *reset_cntl = (unsigned long*)AVALANCHE_RESET_CONTROL_BASE, *mdio_cntl  = (unsigned long*)((int)AVALANCHE_MDIO_BASE + 0x4);
    unsigned int j= 0, detected_phy_map = 0, auto_select = SELECT_INT_PHY_MAC;
                                                                                                                   
    *reset_cntl |= (1 << AVALANCHE_MDIO_RESET_BIT)       | 
                   (1 << AVALANCHE_LOW_CPMAC_RESET_BIT)  | 
                   (1 << AVALANCHE_HIGH_CPMAC_RESET_BIT) | 
                   (1 << AVALANCHE_LOW_EPHY_RESET_BIT);                                 

    *mdio_cntl   = (1 << 30) | (avalanche_get_vbus_freq()/2200000);                                              
                                                                                                                   
    for(j=0;j < 300000; j++) 
    { 
        if(j%100000) continue;

        detected_phy_map = *(mdio_cntl + 1);
        if(detected_phy_map) 
        {
            detected_phy_map &= ~AVALANCHE_LOW_CPMAC_PHY_MASK;
             
            if(!detected_phy_map)   
	        continue; /* Let us try again. */

            if(!(detected_phy_map & (detected_phy_map - 1)))
            {
                auto_select = SELECT_EXT_PHY_MAC;
                break;
            }
            else
	    {
	        /* It is the external switch */
	        auto_select  = SELECT_EXT_PHY_MAC;
                auto_select |= SELECT_EXT_SWITCH;
                break;
            }
        } 
    }

    return(auto_select);
}


void avalanche_soc_platform_init(void)
{
	if(IS_OHIO_CHIP())
	{
      prom_printf("Auto Detection OHIO chip\n");
	}
	else
	{
      prom_printf("Auto Detection SANGAM chip\n");
	}

    /* GPIO INIT */
    PAL_sysGpioInit();
    
    PAL_sysResetCtrl(AVALANCHE_UART0_RESET_BIT, OUT_OF_RESET);
    PAL_sysResetCtrl(AVALANCHE_GPIO_RESET_BIT, OUT_OF_RESET);

    /* Bring Timer 16 modules out of reset */
    PAL_sysResetCtrl(AVALANCHE_TIMER0_RESET_BIT, OUT_OF_RESET);
    PAL_sysResetCtrl(AVALANCHE_TIMER1_RESET_BIT, OUT_OF_RESET);

    /* Bring McDMA out of reset */
    PAL_sysResetCtrl(AVALANCHE_MCDMA_RESET_BIT, OUT_OF_RESET);    

//    avalanche_gpio_out_value(0xf3fc3ff0, 0xf3fc3ff0, 0);
    /* hack for now, to be cleaned up. */
    *(unsigned int *)0xa861090c = 0xf3fc3ff0;

    PAL_sysResetCtrl(AVALANCHE_LOW_EPHY_RESET_BIT,IN_RESET);

    /* Also put CPMAC0 and CPMAC1 to reset */
    PAL_sysResetCtrl(AVALANCHE_LOW_CPMAC_RESET_BIT,IN_RESET);
    PAL_sysResetCtrl(AVALANCHE_HIGH_CPMAC_RESET_BIT,IN_RESET);

#if (!defined(CONFIG_MIPS_AR7WRD) && !defined(CONFIG_MIPS_AR7VWI) && !defined(CONFIG_MIPS_AR7VW))

    PAL_sysResetCtrl(AVALANCHE_LOW_EPHY_RESET_BIT,OUT_OF_RESET);

#else
    if(IS_OHIO_CHIP()) /* Ohio has only EPHY0 - The External Switch is connected to CPMAC0 */
	{
		*(unsigned long*) AVALANCHE_MII_SEL_REG |= 0x00000001;
	}

    /* Reset the Marvell switch via  GPIO pin 17 */
    PAL_sysGpioCtrl(17, GPIO_PIN,GPIO_OUTPUT_PIN);
    PAL_sysGpioOutBit(17, 0);
    
    /* wait for 20 milisec */
    ar7_delay(20);
    
    PAL_sysGpioOutBit(17, 1);

#endif

    {
        PAL_SYS_Tnetd73xxInit clkc;
        clkc.afeclk  = AFECLK_FREQ;
        clkc.refclk  = REFCLK_FREQ;
        clkc.xtal3in = OSC3_FREQ;
    
        /* CLK CTRl INIT */                                                               
        PAL_sysClkcInit(&clkc);                   
     }

    /* initialize vbus frequency */
    avalanche_set_vbus_freq(PAL_sysClkcGetFreq(CLKC_VBUS));

    {        
        volatile unsigned int cvr_reg = *(volatile unsigned int*) AVALANCHE_CVR;
        /* Takes care of the CVR bug in the Sangam Chip as well. */
        cvr_reg = (cvr_reg >> 20) & 0xF;

        if(cvr_reg >= 2 || IS_OHIO_CHIP())   
        {
            p_set_mdix_on_chip_fn = &sangam_set_mdix_on_chip;

            PAL_sysGpioCtrl(AUTO_MDIX_ENABLE_PIN, GPIO_PIN, GPIO_OUTPUT_PIN);
            PAL_sysGpioCtrl(AUTO_MDIX_GPIO_PIN, GPIO_PIN, GPIO_OUTPUT_PIN);

            prom_printf("This SOC has MDIX cababilities on chip.\n");
        } 
    }

#if defined (CONFIG_MIPS_AVALANCHE_VLYNQ)
    vlynq_dev_init();
#endif

}

#ifdef CONFIG_AVALANCHE_INTC_PACING
unsigned int avalanche_get_intc_input_freq(void)
{
    return PAL_sysClkcGetFreq(CLKC_SYS);
}
#endif


