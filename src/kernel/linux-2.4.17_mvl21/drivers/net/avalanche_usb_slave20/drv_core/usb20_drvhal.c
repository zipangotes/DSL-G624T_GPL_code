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
 * FILE NAME    :  usb20_drvhal.c 
 *
 * DESCRIPTION  :  This file implements all HAL interface routines.
 *
 * HISTORY 	:
 * 
 * Date        Name          Notes 
 * ------------------------------------------------------------------------
 * 25 Jul 03   Sabyasachi    Created.
 * 
 *  (C) Copyright 2003, Texas Instruments, Inc.
 **************************************************************************/

#include "psp_types.h"
#include "usb20_drv.h"
#include "usb20_drvapi.h"
#include "usb20_drvhal.h"
#include "usb20_def.h"
#include "usb20_config.h"
#include "usb_sal.h"


/* External Definitions */
#ifdef USB_ENABLE_TASK
extern SAL_EVENT g_event_int;
extern UINT32 global_int_value;
#endif

extern void usb_drv_isr(void *);
extern void usb_drv_service_int(USB_DEV *, int);

extern void usb_drv_register_isr(OS_DEVICE *os_dev, int (*halISR)(HAL_DEVICE *, int *),int intr);
extern void usb_drv_unregister_isr (OS_DEVICE *os_dev, int intr);
extern void usb_drv_cache_invalidate(void *address, int size);
extern void usb_drv_cache_writeback(void *address, int size);

/* Definitions for local routines */
static int critical_section_lock;

static void *(*class_drv_malloc_Rx_buffer)(UINT32 size, USB_PIPE *pipe, void *receive_info) = NULL;
static void (*class_drv_free_Rx_buffer)(void *mem_ptr, void *receive_info) = NULL;


/**************************************************************************
 *  OS Functions. These functions are implemented by driver and used by
 *  HAL. All functions are passed through OS_FUNCTIONS structure.
 ************************************************************************/

static INT32  usb_drv_control
(
    OS_DEVICE  *os_dev,
    HAL_CONTROL_KEY *key,
    HAL_CONTROL_ACTION  action,
    void *value
)
{
    USB_DEV  *usb_dev;
/*    int    result;*/
    UINT32  state_type; 
    UINT32  state_val; 
    char bufSizeKey[50];

    usb_dev = (USB_DEV *)os_dev;
    state_type = ((UINT32)value) & 0xffff0000;
    state_val  = ((UINT32)value) & 0x0000ffff;

    if (key ==  hkStateChange)
    {
        if ( state_type == CPUSB_OS_USB_INT )
        {    
#ifdef USB_ENABLE_TASK            
	    global_int_value = state_val;
	    SAL_event_post(g_event_int, 0);
#else
            usb_drv_service_int(usb_dev,state_val);
#endif 
        }
	else if ( state_type == CPUSB_OS_LINKEDUP )
        {

            if ( state_val == CPUSB_LINK_FULL_SPEED )
            {
		    /* Full Speed (12 Mbps) has been detected */
		    usb_dev->params.speed = USB_FULL_LINK_SPEED;
		    usb_dev->params.bulk_ep_size = USB_FS_BULK_EP_SIZE;
		    USB_LOG(DEBUG_USB20,"Link Speed : 12 Mbps.\n",0,0,0,0);
            }
            else
            {       
		     /* High Speed (480 Mbps) has been detected */
		     usb_dev->params.speed = USB_HIGH_LINK_SPEED;
		     usb_dev->params.bulk_ep_size = USB_HS_BULK_EP_SIZE;
		     USB_LOG(DEBUG_USB20,"Link Speed : 480 Mbps.\n",0,0,0,0);
            }
	      usb_dev->class_dev->funcs->usb_class_dev_event(USB_EVENT_LINK_UP, (UINT8 *) usb_dev->params.bulk_ep_size);
	      strcpy(bufSizeKey, hkOUTEpBuffSize);
	      strcat(bufSizeKey, ".0");
	      usb_dev->hal_funcs->Control(usb_dev->hal_dev, bufSizeKey, haSETbyVAL,(void *) usb_dev->params.bulk_ep_size);
  	      strcpy(bufSizeKey, hkINEpBuffSize);
	      strcat(bufSizeKey, ".0");	  
  	      usb_dev->hal_funcs->Control(usb_dev->hal_dev, bufSizeKey, haSETbyVAL, (void *) usb_dev->params.bulk_ep_size);
		usb_dev->link_status = USB_LINK_UP;
		
        }  /* if state_type == USB_LINK_DETECT */
    }
    
    return 0;
}


static void usb_drv_critical_on(void)
{
    SAL_int_off((UINT32 *)&critical_section_lock);
}


static void usb_drv_critical_off(void)
{
    SAL_int_on((UINT32 *)&critical_section_lock);
}



static void usb_drv_free_Rx_buffer 
(
                     OS_RECEIVEINFO *os_receive_info, 
		     void *mem_ptr
)
{
    if (class_drv_free_Rx_buffer) 
        return class_drv_free_Rx_buffer(mem_ptr, (void *)os_receive_info);
}


static void *usb_drv_malloc_Rx_buffer 
(
		     UINT32 size, 
		     void *mem_base, 
		     UINT32 mem_range, 
		     OS_SETUP *os_setup, 
		     HAL_RECEIVEINFO *hal_receive_info, 
		     OS_RECEIVEINFO **os_receive_info, 
		     OS_DEVICE *os_dev
)
{
    USB_PIPE  *pipe = (USB_PIPE *)os_setup;
    void      *ret_val = NULL;

    USB_ENTER;

    if (class_drv_malloc_Rx_buffer) 
	ret_val = class_drv_malloc_Rx_buffer(size, pipe, (void *)os_receive_info);

    USB_EXIT;
    return ret_val;
}


 

 static int usb_drv_printf(const char *fmt, ...)
{
    return 0;
}


INT32 usb_drv_hal_receive
(
                   OS_DEVICE *os_dev, 
		      FRAGLIST  *frag_list, 
		      UINT32     frag_count,
		      UINT32     pkt_size, 
		      HAL_RECEIVEINFO *hal_receive_info, 
		      UINT32 mode
)
{
    UINT32  i;
    USB_PKT usb_pkt;
    USB_BUF usb_buf[MAX_FRAG_PER_PACKET];
    int ch_no,ep_no;
    USB_DEV * usb_dev = (USB_DEV *)os_dev;
    USB_PIPE * pipe;


    if ( frag_count > MAX_FRAG_PER_PACKET ) 
    {
    	     usb20_logMsg("====================================\n",0,0,0,0,0,0);
    	     usb20_logMsg("ERROR!! ERROR!! ERROR!! Frag count exceeded!!!\n",0,0,0,0,0,0);
    	     usb20_logMsg("====================================\n",0,0,0,0,0,0);    	              
    }
    else 
    {
    	 /* Prepare the USB packet structure */
        usb_pkt.buf_list = &(usb_buf[0]);

        for ( i = 0 ; i < frag_count; i++)
	    {
	        usb_pkt.buf_list[i].data = frag_list[i].data;
	        usb_pkt.buf_list[i].len   = frag_list[i].len;
            usb_pkt.buf_list[i].info = frag_list[i].OsInfo;
            USB_LOG(USB20_ISR,"0x%08x | %d | %d \n",(UINT32)frag_list[i].data, frag_count, pkt_size,0);
	    }
        
	    usb_pkt.num_buf  = frag_count;
   	    usb_pkt.pkt_size  = pkt_size;


    /*
     * Determine the end point where the packet is received.
     * Invoke the receive callback routine for that particular end point
     * to push the data up to class driver
     */
     
    /* Channel number over which this packet is received */
    ch_no = mode & HAL_CHANNEL_MASK;
    /* Get the end point from channel */
    ep_no = usb_dev->channel[ch_no].ep_no; 

   /* Invalidate Data Cache */
   for ( i = 0 ; i < usb_pkt.num_buf; i++)
   {
   	      usb_drv_cache_invalidate((void *)usb_pkt.buf_list[i].data, (INT32) usb_pkt.buf_list[i].len);
   }
   
    pipe = (USB_PIPE *)(usb_dev->ep_table[ep_no -1 ].pipe);  /* EP N is located in N-1 position in table */
    if ( pipe->receive(pipe, &usb_pkt) != 0 ) 
    {
	/* Error */
	USB_LOG((USB20_FATAL|USB20_ISR),"RNDIS Receive returned error %s ",(INT32)  __FUNCTION__,0,0,0);
    }
    
    /* 
     * Reception by class driver successful, free up memory 
     * and notify HAL
     */
    usb_dev->hal_funcs->RxReturn(hal_receive_info, HAL_DO_NOT_FREE_BUFFER);

        
    }
 	
    return 0;
    
}


INT32 usb_drv_hal_send_complete(OS_SENDINFO  *os_sendinfo)
{
    USB_SENDINFO  *info = (USB_SENDINFO *)os_sendinfo;
    USB_PIPE      *pipe = (USB_PIPE *)info->ep->pipe;
    /*static unsigned count=1;*/
    
    USB_LOG(DEBUG_USB20,"Send Complete called\n",0,0,0,0);
    /*if ( pipe->type == USB_EP_T_BULK ) 
	usb20_logMsg("Txed %d\n",count++,0,0,0,0,0);*/
    return (pipe->send_complete(pipe, info->os_sendinfo));
}

/******************************************************************************
 *
 *   Utilities to complete implementation of Driver-HAL Interface 
 *
 *****************************************************************************/

static void * usb_drv_hal_malloc_dma_xfer(UINT32 size, void *base, UINT32 range)
{
     return SAL_malloc(size);
}


INT32 usb_drv_load_os_functions(OS_FUNCTIONS *p_os_funcs)
{
    USB_ENTER;

    if (p_os_funcs)
    {
        p_os_funcs->Control        = usb_drv_control;
        p_os_funcs->CriticalOn     = usb_drv_critical_on;
        p_os_funcs->CriticalOff    = usb_drv_critical_off; 
        p_os_funcs->DataCacheHitInvalidate = usb_drv_cache_invalidate;
        p_os_funcs->DataCacheHitWriteback  = usb_drv_cache_writeback;
        p_os_funcs->DeviceFindInfo = NULL;     
        p_os_funcs->DeviceFindParmUint     = NULL;
        p_os_funcs->DeviceFindParmValue    = NULL; 
        p_os_funcs->Free           = SAL_free;
        p_os_funcs->FreeRxBuffer   = usb_drv_free_Rx_buffer;
        p_os_funcs->FreeDev        = SAL_free;
        p_os_funcs->FreeDmaXfer    = SAL_free;
        p_os_funcs->IsrRegister    = usb_drv_register_isr;
        p_os_funcs->IsrUnRegister  = usb_drv_unregister_isr;
        p_os_funcs->Malloc         = SAL_malloc;
        p_os_funcs->MallocDev      = SAL_malloc;
        p_os_funcs->MallocDmaXfer  = usb_drv_hal_malloc_dma_xfer;
        p_os_funcs->MallocRxBuffer = usb_drv_malloc_Rx_buffer;
        p_os_funcs->Printf         = usb_drv_printf;
        p_os_funcs->Sprintf        = (void*)sprintf;
	/*
        p_os_funcs->Receive        = usb_drv_hal_receive;
        p_os_funcs->SendComplete   = usb_drv_hal_send_complete;
	*/
        p_os_funcs->TeardownComplete = NULL;  /* Required for async teardown */
	/*
        p_os_funcs->Strcmpi        = usb_drv_strcmpi;
        p_os_funcs->Strlen         = Strcmpi;
        p_os_funcs->Strstr         = Strcmpi;
        p_os_funcs->Strtoul        = Strcmpi;
	*/
        p_os_funcs->Ext            = NULL;
    }
    else 
    {
        USB_LOG(DEBUG_USB20,"NULL Pointer\n",0,0,0,0);
	return USB_STATUS_ERROR;
    }

    USB_EXIT;
    return USB_STATUS_OK;
}

void usb_drv_set_class_dev_funcs(CLASS_DEV_FUNCS *class_funcs)
{
    class_drv_malloc_Rx_buffer = (void*)class_funcs->MallocBuffer;
    class_drv_free_Rx_buffer   = class_funcs->FreeBuffer;
    return;
}

