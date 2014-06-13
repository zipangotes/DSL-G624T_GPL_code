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
**|         Copyright (c) 2003-2006 Texas Instruments Incorporated           |**
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
 * FILE NAME    :  usb20_os_linux.c  
 *
 * DESCRIPTION  :  Linux specific routines for the USB driver.
 *
 * HISTORY 	:
 *
 *  15 Apr 2004 - Prashanth - Created.
 *
 *
 *  (C) Copyright 2003-2006, Texas Instruments, Inc.
 *************************************************************************/

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <asm/io.h>
#include <asm/avalanche/avalanche_map.h>
#include <asm/avalanche/generic/avalanche_intc.h>

#include "usb_sal.h"
#include "usb20_drvapi.h"
#include "usb20_drv.h"
#include "usb20_config.h"


/****************************Externs****************************************/
extern void usb_drv_isr(UINT32 arg1, UINT32 arg2, UINT32 arg3);
extern int vlynq_port_num;




/**************************** Globals ***************************************/
unsigned int usbDbgLevel =0;  /* Debug level */

static struct tasklet_struct usb20_drv_tasklet;



/************************** USB Net task routines ****************************/

/**
 *  usb20_net_task :
 *
 *  This routine is invoked to do the actual packet processing. This routine 
 *  runs in the context of a "net task".
 *
 */ 

void  usb20_net_tasklet(unsigned long p_dev)
{
INT32 more_packets;
USB_DEV * p_usb_dev = (USB_DEV *) p_dev;


        if(p_usb_dev->hal_vbus_isr != NULL)
        {
            /* While there are packets to process, call HAL ISR */
                more_packets=0;
				p_usb_dev->hal_vbus_isr(p_usb_dev->hal_dev, &more_packets);

				if(more_packets)
				{
						/* There are more packets to handle.
						 * Schedule ourselves again.
						 */
						tasklet_schedule(&usb20_drv_tasklet);
						
				}
				else
				{
						/* No more packets to process. Inform HAL 
						 * to enable interrupts again
						 */
            			p_usb_dev->hal_funcs->PacketProcessEnd(p_usb_dev->hal_dev);
        		}
		}
        else
        {   
            usb20_logMsg("USB PANIC!!! HAL ISR not yet registered\n",0,0,0,0,0,0);
        }
    
    return;
}

/**
 *  usb20_drv_net_task_create :
 *
 *  This function will create all the necessary mechanisms to have a "net task"
 *  i.e. a seperate thread that will do the packet processing in a different 
 *  context rather than the interrupt context.
 *  In VxWorks, we create a Task which waits on an event (semaphore). So here 
 *  we create an event g_event_rcv and a task with USB_NET_TASK_NAME giviing 
 *  us the name of the task.
 *
 *  In Linux, we create a tasklet, which will execute once the ISR is completed.
 *
 */ 

INT32 usb20_drv_net_task_create(void *p_dev)
{
    
	tasklet_init(&usb20_drv_tasklet, usb20_net_tasklet, (unsigned long) p_dev);

    return USB_STATUS_OK;
}


/************************* HAL support functions *****************************/


/************************ ISR related functions ******************************/

void usb20_usb_isr(int irq, void * tc_instance, struct pt_regs * regs)
{
		USB_DEV * p_usb_dev = (USB_DEV *)tc_instance;
		int dummy;

		p_usb_dev->hal_usb_isr(p_usb_dev->hal_dev, &dummy);
		return;
		
}


void usb20_vbus_isr(int irq, void * tc_instance, struct pt_regs * regs)
{
	/* Schedule a tasklet and do all processing in the tasklet */
	tasklet_schedule(&usb20_drv_tasklet);

 	return;
}


/**
 *  usb_drv_register_isr :
 *
 *  This routine will be called by the HAL to register its ISR. This routine 
 *  will register the ISR supplied by HAL with VLYNQ driver (or system INTC, as 
 *  the case maybe). Additionally, OS specific wrappers can be used for the HAL 
 *  ISRs.
 */ 
 
void usb_drv_register_isr
(
                  OS_DEVICE *os_dev, 
		     int (*halISR)(HAL_DEVICE *, int *),
		     int int_num
)
{
    USB_DEV   *usb_dev = (USB_DEV *)os_dev;

    USB_ENTER;
	
		

    if ( usb_dev->params.usb_irq == int_num ) 
    {
		usb_dev->hal_usb_isr = halISR;
        request_irq( int_num, (void *)usb20_usb_isr, SA_INTERRUPT, "USB 2.0 USB Interrupt",(void *)os_dev);
    }
    else if ( usb_dev->params.vbus_irq == int_num ) 
    {
     int result;
       /* For the Vbus IRQ, we create a "net task" which will handle packets in a task mode */

      /* Create the Net Task */
     result = usb20_drv_net_task_create(usb_dev);
     if(result != USB_STATUS_OK)
     {
        printk("USB Net Tasklet Creation Failed\n");
	    return;
     }
    /* Remember the ISR supplied by HAL */
    usb_dev->hal_vbus_isr = halISR;
    /* Register our ISR */
	request_irq(int_num, (void *)usb20_vbus_isr, SA_INTERRUPT,"USB 2.0 VBUS Interrupt", (void *)os_dev);
    
    }
    
    USB_LOG(USB20_CONTROL,"Interrupt %d is registered\n",int_num,0,0,0);
    
    USB_EXIT;
    return;
}

/**
 *  usb_drv_unregister_isr :
 *
 *  This routine will be called by the HAL to unregister its ISR. This routine 
 *  will unregister the ISR supplied by HAL with VLYNQ driver (or system INTC, as 
 *  the case maybe). 
 */
 

void usb_drv_unregister_isr (OS_DEVICE *os_dev, int int_num)
{
    USB_DEV   *usb_dev = (USB_DEV *)os_dev;
	
    USB_ENTER;
	
    if ( usb_dev->params.vbus_irq == int_num ) 
			tasklet_kill(&usb20_drv_tasklet);

       free_irq(int_num, (void *) os_dev);

    USB_LOG(USB20_CONTROL,"Interrupt %d is unregistered\n",int_num, 0, 0, 0);

    USB_EXIT;
    return;
}


/*********************** Cache related functions **************************/

/**
 *  usb_drv_cache_invalidate :
 *
 *  This routine will be passed to HAL. HAL will use this routine to invalidate
 *  the cache regions.
 */ 

void usb_drv_cache_invalidate(void *address, int size)
{
		dma_cache_inv((unsigned long)address, size);

}


/**
 *  usb_drv_cache_writeback :
 *
 *  This routine will be passed to HAL. HAL will use this routine to flush 
 *  (write-back) the cache regions.
 */ 
 
void usb_drv_cache_writeback(void *address, int size)
{
		dma_cache_wback_inv((unsigned long)address, size);
}


/************************** Driver logging functions *************************/


/**************************************************************************
 * FUNCTION NAME : usblogMsg
 **************************************************************************
 * DESCRIPTION   :
 *  Logs messages from the RNDIS driver. 
 ***************************************************************************/
int usblogMsg 
(
    unsigned int     level,  /* Debug Level of the message.  */
    char*   fmt,    /* Format String                */
    int     arg1,   /* Argument 1                   */
    int     arg2,   /* Argument 2                   */
    int     arg3,   /* Argument 3                   */
    int     arg4,   /* Argument 4                   */
    int     arg5    /* Argument 5                   */
)
{
    /* Check if the message needs to be logged ? */ 
  if (level > usbDbgLevel) 
        return 0;

    /* YES. The Message DEBUG level is greater than the module debug level. */
    printk (fmt, arg1, arg2, arg3, arg4, arg5, 0);

    return 0;
}



/**************************************************************************
 * FUNCTION NAME : usb20_logMsg
 **************************************************************************
 * DESCRIPTION   :
 *  Logs messages from the USB HAL. 
 ***************************************************************************/
int usb20_logMsg 
(
    char*   fmt,    /* Format String                */
    int     arg1,   /* Argument 1                   */
    int     arg2,   /* Argument 2                   */
    int     arg3,   /* Argument 3                   */
    int     arg4,   /* Argument 4                   */
    int     arg5,    /* Argument 5                   */
    int     arg6    /* Argument 6                   */
)
{
    printk (fmt, arg1, arg2, arg3, arg4, arg5, arg6);
    return 0;
}


