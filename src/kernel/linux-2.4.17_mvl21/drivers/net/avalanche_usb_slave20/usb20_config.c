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

/**************************************************************************
 * FILE NAME    :  usb20_config.c 
 *
 * DESCRIPTION  :  Configuration Routines are implemented in this file.
 *
 * HISTORY 	:
 *
 *  10/25/2003  - sabyasachi - Created.
 *  12/11/2003  - sabyasachi - Added routines for retrieving Host/Device MAC Address.
 *  16 Apr 2004 - Prashanth. Adopted for Linux. File converted to be OS specific.
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/

#include "psp_types.h"
#include "usb20_config.h"
#include <asm/mips-boards/prom.h>
#include <asm/io.h>



/*********Globals*******************/
#if 0
INT32 vlynq_port_num = V2U_VLYNQ_PORT;       /* The Vlynq port number */
#endif

/**
 * @defgroup Config_API Configuration API
 *
 */


/**
 * @ingroup Config_API
 * 
 *  Implement this routine to provide Vendor ID in implementation specific ways (from Environment Space,
 *  Silicon Registers, Applications, or anything else.
 *
 *  @return  USB Vendor ID
 */
UINT16   usb20_config_get_vid(void)
{
		int vendor_id = USB_VID; /*initialide to default value */
		char *vid=NULL;
		
	    /* Read the VID from environment */	
		vid=prom_getenv("usb20_vid");
		if(vid!=NULL)
		{
				vendor_id = (UINT16) simple_strtol(vid,NULL,0);
		}
      
		printk("USB 2.0: VID = 0x%x\n", vendor_id);
    
		return (vendor_id);
}


/**
 * @ingroup Config_API
 * 
 *  Implement this routine to provide Product ID in implementation specific ways (from Environment Space,
 *  Silicon Registers, Applications, or anything else).
 *
 *  @return  USB Product ID
 */
UINT16   usb20_config_get_pid()
{
		int product_id = USB_PID; /* initialize to default value */
		char *pid=NULL;
	
	    /* Read the PID from environment */	
		pid=prom_getenv("usb20_pid");
		if(pid!=NULL)
		{
				product_id = (UINT16) simple_strtol(pid,NULL,0);
		}
      
		printk("USB 2.0: PID = 0x%x\n", product_id);
    
		return (product_id);
}

/**
 * @ingroup Config_API
 * 
 *  Implement this routine to provide Serial No String in implementation specific ways (from Environment Space,
 *  Silicon Registers, Applications, or anything else).
 *
 *  @param ptr  OUT  Pointer of (size) bytes where the string will be copied.
 *
 *  @param size IN Size of the buffer (first parameter) in bytes.
 * 
 *  @return  Size of the serial number string 
 */

UINT32  usb20_config_get_serial_no(char *ptr, unsigned int size)
{
    int  len;
	char *ptr_temp=NULL;

	/*Read the serial string from environment */
	ptr_temp=prom_getenv("usb20_serial");
	if(ptr_temp != NULL)
	{
			len = strlen(ptr_temp);
			if(size < len +1 )
					return -1;
			
			memcpy(ptr,ptr_temp,len+1);
	}
	else /* take a default serial string */
	{

    len  =  strlen(USB_SERIAL_NO);
    if ( size < len+1 ) 
		return -1;

    strcpy(ptr, USB_SERIAL_NO);
    ptr[len] = 0;
	}
	printk("USB 2.0: Serial Number String = %s\n",ptr);
    return len;
}

/**
 * @ingroup Config_API
 * 
 *  Implement this routine to provide Manufacturer String in implementation specific ways (from Environment Space,
 *  Silicon Registers, Applications, or anything else).
 *
 *  @param ptr  OUT  Pointer of (size) bytes where the string will be copied.
 *
 *  @param size IN Size of the buffer (first parameter) in bytes.
 * 
 *  @return  Size of the manufacturer  number string 
 */

UINT32  usb20_config_get_manufacturer_string(char *ptr, unsigned int size)
{
    int  len;
	char *ptr_temp=NULL;

	/*Read the manufacturer string from environment */
	ptr_temp=prom_getenv("usb20_man");
	if(ptr_temp != NULL)
	{
			len = strlen(ptr_temp);
			if(size < len +1 )
					return -1;
			
			memcpy(ptr,ptr_temp,len+1);
	}
	else /* take a default serial string */
	{

    len  =  strlen(USB_MANUFACTURER_STRING);
    if ( size < len+1 ) 
		return -1;

    strcpy(ptr, USB_MANUFACTURER_STRING);
    ptr[len] = 0;
	}
	printk("USB 2.0: Manufacturer String = %s\n",ptr);
    return len;
}


/**
 * @ingroup Config_API
 * 
 *  Implement this routine to provide Product Description String in implementation specific ways (from Environment Space,
 *  Silicon Registers, Applications, or anything else).
 *
 *  @param ptr  OUT  Pointer of (size) bytes where the string will be copied.
 *
 *  @param size IN Size of the buffer (first parameter) in bytes.
 * 
 *  @return  Size of the product string 
 */

UINT32  usb20_config_get_product_string(char *ptr, unsigned int size)
{
    int  len;
	char *ptr_temp=NULL;

	/*Read the serial string from environment */
	ptr_temp=prom_getenv("usb20_prod");
	if(ptr_temp != NULL)
	{
			len = strlen(ptr_temp);
			if(size < len +1 )
					return -1;
			
			memcpy(ptr,ptr_temp,len+1);
	}
	else /* take a default product string */
	{

    len  =  strlen(USB_PRODUCT_STRING);
    if ( size < len+1 ) 
		return -1;

   strcpy(ptr, USB_PRODUCT_STRING);
    ptr[len] = 0;
	}
	printk("USB 2.0: Product Description = %s\n",ptr);
    return len;
}

/**
 * @ingroup Config_API
 * 
 *  Implement this routine to provide Max Power in implementation specific ways (from Environment Space,
 *  Silicon Registers, Applications, or anything else).
 *
 *  @return  USB Serial No String
 */

UINT16  usb20_config_get_max_power()
{
    return USB_MAX_POWER;
}





/**************************************************************************
 * FUNCTION NAME : str2hexnum
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
static unsigned char str2hexnum(unsigned char c)
{
	if(c >= '0' && c <= '9')
    	return c - '0';
	if(c >= 'a' && c <= 'f')
    	return c - 'a' + 10;
	if(c >= 'A' && c <= 'F')
    	return c - 'A' + 10;
	return 0;
}

/**************************************************************************
 * FUNCTION NAME : str2eaddr
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
static void str2eaddr(unsigned char *ea, unsigned char *str)
{
        int i;
        unsigned char num;
        for(i = 0; i < 6; i++) {
                if((*str == '.') || (*str == ':'))
                        str++;
                num = str2hexnum(*str++) << 4;
                num |= (str2hexnum(*str++));
                ea[i] = num;
        }
}






/**
 * @ingroup Config_API
 * 
 *  Implement this routine to provide Host MAC Address in implementation specific ways 
 *  (from Environment Space, Silicon Registers, Applications, or anything else).
 *
 *  @param  addr   OUT   Host (PC) MAC Address
 *
 *  @return  USB Serial No String
 */

UINT32  usb20_config_get_host_mac_addr(unsigned char *addr)
{
    
	char  *mac_string;
    unsigned char mac_addr[6];
	int i;
    

	/* Get the MAC address for the USB RNDIS NET device. */ 
	mac_string=prom_getenv("usb20_rndis_mac");
	if(!mac_string)
    {
    	mac_string= USB_RNDIS_HOST_MAC_ADDR ;
        printk("USB 2.0: Unable to get host mac address from bootloader enviroment.\n");
        printk("USB 2.0: Using default host mac address.\n");
	}
	
	str2eaddr(mac_addr,mac_string);
	for (i=0; i <= 6; i++)
    {
    	addr[i] = mac_addr[i];   /* This sets the hardware address */
    }

        printk("USB 2.0: Host   mac address: %s\n",mac_string);
    return 0;
}


/**
 * @ingroup Config_API
 * 
 *  Implement this routine to provide Device MAC Address in implementation specific ways 
 *  (from Environment Space, Silicon Registers, Applications, or anything else).
 *
 *  @param  addr   OUT   Device MAC Address
 *
 *  @return  USB Serial No String
 */

UINT32  usb20_config_get_device_mac_addr(unsigned char *addr)
{
	char  *mac_string;
    unsigned char mac_addr[6];
	int i;
    

	/* Get the MAC address for the USB RNDIS NET device. */ 
	mac_string=prom_getenv("usb20_board_mac");
	if(!mac_string)
    {
    	mac_string= USB_RNDIS_DEVICE_MAC_ADDR ;
        printk("USB 2.0: Unable to get device mac address from bootloader enviroment.\n");
        printk("USB 2.0: Using default device mac address.\n");
	}
	
	str2eaddr(mac_addr,mac_string);
	for (i=0; i <= 6; i++)
    {
    	addr[i] = mac_addr[i];   /* This sets the hardware address */
    }
        printk("USB 2.0: Device mac address: %s\n",mac_string);
    return 0;
}



USB_DEV_PARAM v2u_dev[1]; /* The params structure to initialize */


/* These 3 are module parameters.
 * All default values will be for Vlynq 0 on Titan.
 */

UINT32 usb20_base_address=0xAC000000; /* Vlynq0 base on Titan */
UINT32 usb20_usb_irq=82; /* Default value = 80 + 2 */
UINT32 usb20_vbus_irq=83; /*Default value = 80 + 3 */

/**
 * @ingroup Config_API
 * 
 *  Implement this routine to provide device parameters in implementation specific ways (from Environment Space,
 *  Silicon Registers, Applications, or anything else).
 *
 *  @return  USB Serial No String
 */
USB_DEV_PARAM * usb20_config_get_dev_param()
{
		v2u_dev[0].usb_base         = usb20_base_address + 0x00020000;
        v2u_dev[0].usb_offset       =  0x14000000;
        v2u_dev[0].reset_base       = usb20_base_address + 0x00000200; 
        v2u_dev[0].reset_bit        = 1;
        v2u_dev[0].clkc_base        = usb20_base_address + 0x0000020C; 
        v2u_dev[0].clkc_mask        =  0x80;
        v2u_dev[0].usb_irq          =  usb20_usb_irq;
        v2u_dev[0].vbus_irq         = usb20_vbus_irq;
        v2u_dev[0].ucp_base         =  usb20_base_address + 0x0000020C;
        v2u_dev[0].ucp_disable_val  =  0x5A3C0000;
        v2u_dev[0].ucp_disable_mask =  0xffff0000;
		
	
    return (USB_DEV_PARAM *)&v2u_dev[0];
}

