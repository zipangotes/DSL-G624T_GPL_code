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

/*****************************************************************************
 * Clock Control
 *****************************************************************************/

#include "puma-s_clk_cntl.h" 

#define CLKC_CLKCR(x)          (AVALANCHE_CLOCK_CONTROL_BASE + 0x20 + (0x20 * (x)))
#define CLKC_CLKPLLCR(x)       (AVALANCHE_CLOCK_CONTROL_BASE + 0x30 + (0x20 * (x)))

#define CLKC_NUM               5
#define BOOTCR_PLL_BYPASS      0x00000020
#define PLL_MUL_MAXFACTOR      15

#define PLL_LOCK_TIME          10100 * 75
#define DIVIDER_LOCK_TIME      10100

#define MAX_PRE_DIV_VALUE      32
#define MAX_DIV_VALUE          16


#define DIV_MASK               0xFFFFFFF0
#define PRE_DIV_MASK           0xFFE0FFFF
#define PRE_DIV_BIT_POS        16

#define BOOTCR_SYSPLSEL_XTAL   (1 << 17)
#define BOOTCR_USBPLSEL_XTAL   (1 << 18)

#define USB_OUT_FREQ           CLK_MHZ(48)
#if 0
#define USB_PLL1               1
#define USB_DIV1               1
#define USB_PLL2               2
#define USB_DIV2               2


#define KSEG1_BASE              0xa0000000

#ifndef KSEG_INV_MASK
#define KSEG_INV_MASK               0x1FFFFFFF /* Inverted mask for kseg address */
#endif

#ifndef PHYS_ADDR
#define PHYS_ADDR(addr)             ((addr) & KSEG_INV_MASK)
#endif

#ifndef PHYS_TO_K1
#define PHYS_TO_K1(addr)            (PHYS_ADDR(addr)|KSEG1_BASE)
#endif

#ifndef REG8_ADDR
#define REG8_ADDR(addr)             (volatile UINT8 *)(PHYS_TO_K1(addr))
#define REG8_DATA(addr)             (*(volatile UINT8 *)(PHYS_TO_K1(addr)))
#define REG8_WRITE(addr, data)      REG8_DATA(addr) = data;
#define REG8_READ(addr, data)       data = (UINT8) REG8_DATA(addr);
#endif

#ifndef REG16_ADDR
#define REG16_ADDR(addr)            (volatile unsigned short *)(PHYS_TO_K1(addr))
#define REG16_DATA(addr)            (*(volatile unsigned short *)(PHYS_TO_K1(addr)))
#define REG16_WRITE(addr, data)     REG16_DATA(addr) = data;
#define REG16_READ(addr, data)      data = (unsigned short) REG16_DATA(addr);
#endif

#ifndef REG32_ADDR
#define REG32_ADDR(addr)            (volatile unsigned int *)(PHYS_TO_K1(addr))
#define REG32_DATA(addr)            (*(volatile unsigned int *)(PHYS_TO_K1(addr)))
#define REG32_WRITE(addr, data)     REG32_DATA(addr) = data;
#define REG32_READ(addr, data)      data = (unsigned int) REG32_DATA(addr);
#endif
#endif /* #if 0*/

/* Enum defining the types of BBIF Clock sources available */
typedef enum PAL_SYS_BBIF_CLK_SEL_tag
{

    BBIF_CLK_ONE_HALF_VBUS   = 0,
    BBIF_CLK_VBUS            = 1,
    BBIF_CLK_BBCLK           = 2

} PAL_SYS_BBIF_CLK_SEL_T;


/* Enum defining the types of USB Clock sources available */
typedef enum PAL_SYS_USB_CLK_SEL_tag
{
    USB_CLK_USBCLKI          = 0,
    USB_CLK_REFCLKI          = 1

} PAL_SYS_USB_CLK_SEL_T;

/* Enum defining the types of PCI Clock sources available */
typedef enum PAL_SYS_PCI_CLK_SEL_tag
{
    PCI_CLK_ONE_HALF_VBUS    = 0,
    PCI_CLK_PCLK             = 1

} PAL_SYS_PCI_CLK_SEL_T;

/* Enum defining the types of Codec PCM Clock sources available */
typedef enum PAL_SYS_PCM_CLK_SEL_tag
{
    PCM_CLK_ONE_HALF_VBUS    = 0,
    PCM_CLK_PCLK             = 1

} PAL_SYS_PCM_CLK_SEL_T;


/* Enum defining the types of DSP Subsystem Clock sources available */
typedef enum PAL_SYS_DSP_CLK_SEL_tag
{
    DSP_CLK_DSPCLKI          = 0,
    DSP_CLK_REFCLKI          = 1

} PAL_SYS_DSP_CLK_SEL_T;

typedef enum PAL_SYS_CRYSTAL_ID_tag
{
    REFCLKI = 0,
    USBCLKI
}PAL_SYS_CRYSTAL_ID_T;

static UINT32 crystal_freq[5];

typedef enum TNETD53XX_CRYSTAL_ID_tag
{
    REFCLKI = 0,
    USBCLKI,
}TNETD53XX_CRYSTAL_ID_T;

/*****************************************************************
 * Type:        CLKC_STRUCT_T
 *****************************************************************
 * Description: This type defines the hardware overlay of clock PLL
 * and control registers with respect to each clock.
 *
 ******************************************************************/
typedef struct CLKC_STRUCT_tag 
{
    UINT32 CLKCR;    
    UINT32 PAD[3];
    UINT32 CLKPLLCR;        
} CLKC_STRUCT_T;

/*****************************************************************
 * Type:        PAL_SYS_CLKC_STRUCT_T
 *****************************************************************
 * Description: This type defines the hardware overlay of clock 
 * control module .
 *
 ******************************************************************/
typedef struct PAL_SYS_CLKC_STRUCT_tag
{
    CLKC_STRUCT_T clk[CLKC_NUM];
} PAL_SYS_CLKC_STRUCT_T;

/* forward declaraions */
static INT32 avalanche_set_pll(int clk_id, UINT32 mult);
static UINT32 avalanche_get_usb_clk(void);
static int avalanche_set_usb_freq(void);
static UINT32 avalanche_get_sys_freq(void);

static volatile PAL_SYS_CLKC_STRUCT_T *pclk_regs;
static volatile int  *bootcr_reg;

/****************************************************************************
 * FUNCTION: PAL_sysClkcInit
 ****************************************************************************
 * Description: The routine initializes the internal variables depending on
 *              on the sources selected for different clocks.
 ***************************************************************************/
void PAL_sysClkcInit(void* param)
{
	PAL_SYS_Tnetd53xxInit* ptr = (PAL_SYS_Tnetd53xxInit *)param;
    crystal_freq[REFCLKI] = ptr->ref_clk;
    crystal_freq[USBCLKI] = ptr->usb_clk;
	
     /* Initialize clk ctrl reg array */
     pclk_regs = AVALANCHE_CLOCK_CONTROL_BASE + 0x20; 
     bootcr_reg = AVALANCHE_DCL_BOOTCR;
}

/**
 * \brief PAL System Set Clock Frequency
 * \note this function may not be able to set the frequency requested if it
 * finds that the hardware does not allow it and/or the output desired is 
 * unfeasible. In all cases, the function just returns the clock frequency 
 * before reprogramming. It is the responsibility of the caller to call its 
 * 'get' counterpart after the 'set' to determine if the change actually did 
 * take place. 
 * \note This function waits for the PLLs to lock in a tight loop. Think twice
 * before calling this function in an interrupt context (for example)
 */
int PAL_sysClkcSetFreq(PAL_SYS_CLKC_ID_T clk_id, unsigned int output_freq)
{
	  if(clk_id != CLKC_USB || output_freq != CLK_MHZ(48))
	  {
		  return -1;
	  }

	  return avalanche_set_usb_freq();
}

/**
 * \brief PAL System Get Clock Frequency
 */
int PAL_sysClkcGetFreq(PAL_SYS_CLKC_ID_T clk_id)
{
	switch(clk_id)
	{
		case CLKC_USB:
			return 	avalanche_get_usb_clk();

		case CLKC_VBUS:
			return avalanche_get_sys_freq() / 2;
		break;

		case CLKC_MIPS:
		case CLKC_SYS:
			return avalanche_get_sys_freq();

		default:
			return -1;
	}
}


/*******************************************************************************
 * FUNCTION:    avalanche_set_pll
 *******************************************************************************
 * DESCRIPTION: Function to set the pll of the clock specified.
 *
 ******************************************************************************/
static int avalanche_set_pll
(
    int clk_id,
    UINT32 mult
)
{
    volatile UINT32*    ptr;
    volatile UINT32*    ptr32;
    volatile UINT32     value32;
    volatile UINT32 temp;

    /* Check the parameter for validity */
    if( mult > PLL_MUL_MAXFACTOR)
    {
        return -1;
    }

    if(clk_id == CLKC_VBUS)
    {
        return (-1);
    }

    /* See if PLL is in bypass mode. In this case, we can't set it up. */

	if((*bootcr_reg) * BOOTCR_PLL_BYPASS)
    {
        /* Our hardware is in BYPASS mode, we can't do much */
        return(-1);
    }

    /*
     * The actual value that we have to write out is (mult-1). That is,
     * to set mutliplication factor to 5, we have to write out 4 to the PLL
     * register.
     */

    mult    = mult - 1;

    mult    = mult & 0xf;
    mult    = mult << 12;
    mult    = mult | ((255<<3)|0x0e);


    /* Bring PLL to DIV mode */
	pclk_reg->clk[clk_id].CLKPLLCR = 0x4;
	
    /* Wait to enter DIV mode */
	while((pclk_reg->clk[clk_id].CLKPLLCR & 0x1) != 0)
    {
        /* do nothing */
    }


    /* Put requested PLL value in */
	pclk_reg->clk[clk_id].CLKPLLCR = mult;
	
    /* Wait to exit DIV mode */
	while((pclk_reg->clk[clk_id].CLKPLLCR & 0x1) == 0)
    {
        /* do nothing */
    }

    /*wait for External pll to lock*/
    for(temp =0; temp < PLL_LOCK_TIME; temp++);

    return 0;
}



/*******************************************************************************
 * FUNCTION:    avalanche_set_post_div
 *******************************************************************************
 * DESCRIPTION: Function to set the post div of the clock specified.
 *
 * RETURNS:     AVALANCHE_ERR_OK on success, TNETD53XX_ERR_ERROR on failure
 *
 ******************************************************************************/
static int avalanche_set_post_div
(
    PAL_SYS_CLKC_ID_T clk_id,
    UINT32 div
)
{
    volatile UINT32*    ptr;
    volatile UINT32 temp;

    /* Check the parameter for validity */
    if( div > MAX_DIV_VALUE)
    {
        return(-1);
    }

    if(clk_id == CLKC_VBUS)
    {
        return (-1);
    }

    /*
     * The actual value that we have to write out is (div-1). That is,
     * to set div factor to 5, we have to write out 4 to the div register.
     */

    div = div - 1;

	pclk_reg->clk[clk_id].CLKCR &= DIV_MASK;
	pclk_reg->clk[clk_id].CLKCR |=div;
	
    /*wait for divider output to stabilise*/
    for(temp =0; temp < DIVIDER_LOCK_TIME; temp++);

    return 0;
}

/*******************************************************************************
 * FUNCTION:    avalanche_set_pre_div
 *******************************************************************************
 * DESCRIPTION: Function to set the pre div of the clock specified.
 *
 * RETURNS:     AVALANCHE_ERR_OK on success, TNETD53XX_ERR_ERROR on failure
 *
 ******************************************************************************/
static int avalanche_set_pre_div
(
    PAL_SYS_CLKC_ID_T clk_id,
    UINT32 div
)
{
    volatile UINT32*    ptr;
    volatile UINT32 temp;

    /* Check the parameter for validity */
    if( div > MAX_PRE_DIV_VALUE)
    {
        return(-1);
    }

    if(clk_id == CLKC_VBUS)
    {
        return (-1);
    }

    /*
     * The actual value that we have to write out is (div-1). That is,
     * to set div factor to 5, we have to write out 4 to the div register.
     */

    div = div - 1;

	pclk_reg->clk[clk_id].CLKCR &= PRE_DIV_MASK;
	pclk_reg->clk[clk_id].CLKCR |= (div << PRE_DIV_BIT_POS);
	
    /*wait for divider output to stabilise*/
    for(temp =0; temp < DIVIDER_LOCK_TIME; temp++);

    return 0;
}


/*******************************************************************************
 * FUNCTION:    avalanche_set_usb_freq
 *******************************************************************************
 * DESCRIPTION: Function to set the USB clock.
 *
 * RETURNS:     AVALANCHE_ERR_OK on success, TNETD53XX_ERR_ERROR on failure
 *
 ******************************************************************************/
static int avalanche_set_usb_freq(void)
{
    avalanche_set_pll(CLKC_USB1, 8); 	/* multiply input crystal(25MHz) by 8 */
    avalanche_set_post_div(CLKC_USB1, 5); 	/* set Divider1 to 5 */
    avalanche_set_pre_div(CLKC_USB2, 5); 	/* set Divider2 to  5 */
    avalanche_set_pll(CLKC_USB2, 6); 		/* set Multiplier2 to 6 */	
    return 0;
}

/*******************************************************************************
 * FUNCTION:    avalanche_get_sys_freq
 *******************************************************************************
 * DESCRIPTION: Function to get the system frequency
 *
 * RETURNS:     clock frequency on success, zero on error.
 *
 ******************************************************************************/
static UINT32 avalanche_get_sys_freq()
{
    UINT32  mips_pll_setting;
    UINT32  mips_div_setting;
    UINT16  pll_factor;
    UINT32  base_clk;

    if((*bootcr_reg) & BOOTCR_SYSPLSEL_XTAL)
    {
        base_clk = crystal_freq[REFCLKI];
    }
    else
    {
        base_clk = crystal_freq[USBCLKI];
    }


    {
        UINT32 pre_div;

		pre_div = ((pclk_reg->clk[CLKC_SYS].CLKCR & ~(PRE_DIV_MASK))
                                                            >> PRE_DIV_BIT_POS);
		
        pre_div += 1;

        base_clk = base_clk / pre_div;
    }

    if((*bootcr_reg) & BOOTCR_PLL_BYPASS)
    {
        return (base_clk);  /* PLLs bypassed.*/
    }

	mips_pll_setting = pclk_reg->clk[CLKC_SYS].CLKPLLCR;

    /* Get the PLL multiplication factor */
    pll_factor = ((mips_pll_setting & 0xF000) >>12) + 1;

    /* Check if we're in divide mode or multiply mode */
    if((mips_pll_setting & 0x1) == 1)
    {
        base_clk = (base_clk*pll_factor);
    }
    else        /* We're in divide mode */
    {
        if((mips_pll_setting & 0x0802) == 0x0802) /* See if PLLNDIV & PLLDIV are set */
        {
            if(pll_factor & 0x1)
            {
                base_clk = ((base_clk * pll_factor) >> 1);
            }
            else
            {
                base_clk = ((base_clk * (pll_factor - 1)) >>2);
            }
        }
        else
        {
            if(pll_factor < 0x10)
            {
                base_clk = (base_clk >> 1);
            }
            else
            {
                base_clk = (base_clk >> 2);
            }
        }
    }

    /* Divisor is a 4 bit N+1 divisor */
	mips_div_setting = (0xF & pclk_reg->clk[CLKC_SYS].CLKCR)+1;

    base_clk = base_clk / mips_div_setting;

    return(base_clk);
}

/*******************************************************************************
 * FUNCTION:    avalanche_get_usb_clk
 *******************************************************************************
 * DESCRIPTION: Function to get the usb clock frequency
 *
 * RETURNS:     clock frequecy on success, zero on error.
 *
 ******************************************************************************/
static UINT32 avalanche_get_usb_clk()
{
    UINT32 usb_pll_setting;
    UINT32 usb_div_setting;

    UINT32 usb_pll_setting1;
    UINT32 usb_div_setting1;

    UINT16 pll_factor;
    UINT32 base_clk;

	if((*bootcr_reg) & BOOTCR_USBPLSEL_XTAL)
    {
        base_clk = crystal_freq[REFCLKI];
    }
    else
    {
        base_clk = crystal_freq[USBCLKI];
    }


    /* Divisor is a 4 bit N+1 divisor */
	usb_div_setting = (0xF & pclk_reg->clk[CLKC_USB2].CLKCR) + 1;

    base_clk = base_clk / usb_div_setting;

    {
        UINT32 pre_div_setting;
        UINT32 pre_div_setting1;

        pre_div_setting = ((pclk_reg->clk[CLKC_USB1].CLKCR &
                                        ~(PRE_DIV_MASK)) >> PRE_DIV_BIT_POS);
        pre_div_setting += 1;

        base_clk = base_clk / pre_div_setting;

		usb_div_setting1 = (0xF & pclk_reg->clk[CLKC_USB2].CLKCR)+1;

        base_clk = base_clk / usb_div_setting1;

        pre_div_setting1 = ((pclk_reg->clk[CLKC_USB2].CLKCR &
                                        ~(PRE_DIV_MASK)) >> PRE_DIV_BIT_POS);
        pre_div_setting1 += 1;

        base_clk = base_clk / pre_div_setting1;
    }

    if((*bootcr_reg) & BOOTCR_PLL_BYPASS)
    {
        return (base_clk);  /* PLLs bypassed.*/
    }

	usb_pll_setting = pclk_reg->clk[CLKC_USB1].CLKPLLCR;

    /* Get the PLL multiplication factor */
    pll_factor = ((usb_pll_setting & 0xF000) >>12) + 1;

    /* Check if we're in divide mode or multiply mode */
    if((usb_pll_setting & 0x1) == 1)
    {
        base_clk = base_clk*pll_factor;
    }
    else        /* We're in divide mode */
    {
        if((usb_pll_setting & 0x0802) == 0x0802) /* See if PLLNDIV & PLLDIV are set */
        {
            if(pll_factor & 0x1)
            {
                base_clk = ((base_clk * pll_factor) >> 1);
            }
            else
            {
                base_clk = ((base_clk * (pll_factor - 1)) >>2);
            }
        }
        else
        {
            if(pll_factor < 0x10)
            {
                base_clk = (base_clk >> 1);
            }
            else
            {
                base_clk = (base_clk >> 2);
            }
        }
    }


    {
		usb_pll_setting = pclk_reg->clk[CLKC_USB2].CLKPLLCR;
			
        /* Get the PLL multiplication factor */
        pll_factor = ((usb_pll_setting1 & 0xF000) >>12) + 1;

        /* Check if we're in divide mode or multiply mode */
        if((usb_pll_setting1 & 0x1) == 1)
        {
            base_clk = (base_clk*pll_factor);
        }
        else        /* We're in divide mode */
        {
            if((usb_pll_setting1 & 0x0802) == 0x0802) /* See if PLLNDIV & PLLDIV are set */
            {
                if(pll_factor & 0x1)
                {
                    base_clk = ((base_clk * pll_factor) >> 1);
                }
                else
                {
                    base_clk = ((base_clk * (pll_factor - 1)) >>2);
                }
            }
            else
            {
                if(pll_factor < 0x10)
                {
                    base_clk = (base_clk >> 1);
                }
                else
                {
                    base_clk = (base_clk >> 2);
                }
            }
        }
    }

    return(base_clk);
}

