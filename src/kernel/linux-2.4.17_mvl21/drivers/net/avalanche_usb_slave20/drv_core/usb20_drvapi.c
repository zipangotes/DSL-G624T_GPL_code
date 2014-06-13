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

/******************************************************************************
 * FILE NAME    :  usb20_drvapi.c  
 *
 * DESCRIPTION  :  Implements all protocol driver exposed APIs to upper 
 *                 class driver.
 *
 * HISTORY 	:  
 *
 * Aug 05    -   sabyasachi   -   created.
 * Sep 11    -   sabyasachi   -   Modified for HAL Control Parameters.
 * Dec 01    -   sabyasachi   -   Updated IOCTL command set.
 *
 * (C) Copyright 2003, Texas Instruments, Inc.
 *****************************************************************************/


#include "usb20_drvapi.h"
#include "usb20_drv.h"
#include "usb20_drvhal.h"
#include "usb20_config.h"
#include "usb20_drvrndis.h"
#include "usb_sal.h"


/* Globals */
#ifdef USB_ENABLE_TASK
UINT32      global_int_value;
SAL_EVENT      g_event_int;
#endif


/* Protect by mutex later */
#define MAX_SEND_INFO_BUF 512
USB_SENDINFO g_send_info[MAX_SEND_INFO_BUF];
UINT32  g_send_info_ptr;


/* externs */
extern void usb_drv_service_int(USB_DEV* dev, int int_val);
extern char **usb_app_strings;
extern void usb20_clear_log_level(unsigned int );
extern INT32 usb_drv_ccpu_open(USB_DEV *);
extern USB_STATUS usb_drv_set_dev(USB_DEV * );
extern INT32 usb_drv_channel_setup_all(USB_DEV *);
extern INT32 usb_drv_control_send(USB_DEV *, USB_PKT *, USB_SENDINFO *);
extern void usb_drv_cache_writeback(void * , INT32 );
extern CHANNEL_INFO *usb_drv_alloc_hal_channel(USB_PIPE * , int );
extern int rndis_usb_get_mib_stats( void* ptr_rndis_mcb, int command, void * data);


void *usb_receive_task(USB_DEV *);

#ifdef USB_ENABLE_TASK
void *usb_task(USB_DEV *p_dev)
{
    usb_drv_service_int(p_dev,global_int_value);
    return NULL;
}
#endif /* USB_ENABLE_TASK */
/**
 *  @ingroup Protocol_Driver
 *
 *  Protocol Driver initialization routine. This routine should be called
 *  first by the class driver to initialize the protocol driver and under-
 *  lying HAL layer. The initialization sequence is like this : 
 *
 *  	usb_drv_init() --> usb_drv_start() --> usb_drv_ep_open() 
 *  	--> usb_drv_send() --> usb_drv_ep_close() --> usb_drv_ep_stop() --> usb_drv_shutdown()
 * 
 *    @param  class_dev   IN  Class Driver Information
 *
 *    @param  dev  OUT  Handle for USB Protocol Driver
 *   
 *    @return  
 *                - USB_STATUS_OK (success)
 *                - Error otherwise  (See @link Error_Codes Error Codes @endlink)
 */

USB_STATUS  usb_drv_init (CLASS_DEV *class_dev, USB_DEV **dev)
{
    INT32            status = USB_STATUS_OK;  /* Return Code */
    USB_DEV          *p_dev = NULL;
    USB_DEV_PARAM  *p_dev_param = NULL;
    UINT32           hal_func_size;
    UINT32           value;

#ifdef USB_ENABLE_TASK
    INT32        result;
    SAL_THREAD  taskId;
    SAL_THREAD_ATTR  thread_attr;
#endif

    USB_ENTER;

       usb20_clear_log_level(USB20_LOG_ALL);
		
    if ( NULL == class_dev 
         || NULL == class_dev->funcs 
	 || NULL == class_dev->funcs->MallocBuffer 
	 || NULL == class_dev->funcs->FreeBuffer )
    {
	status = USB_STATUS_INV_ARG;
        goto init_error;
    }

	    
    p_dev = (USB_DEV *)SAL_malloc(sizeof(USB_DEV));
    
    if ( NULL == p_dev ) 
    {
	status = USB_STATUS_NO_MEM;
	USB_LOG(USB20_FATAL,"ERROR : Memory allocation for USB_DEV failed\n",0,0,0,0);
	goto init_error;
    }
    memset((void *)p_dev, 0, sizeof(USB_DEV));

    usb_drv_set_class_dev_funcs(class_dev->funcs);
    p_dev->class_dev = class_dev;
    
    p_dev->os_funcs = (OS_FUNCTIONS *)SAL_calloc(1, sizeof(OS_FUNCTIONS));
    if ( NULL == p_dev->os_funcs )
    {
	status = USB_STATUS_NO_MEM; 
	USB_LOG(DEBUG_USB20,"ERROR : Memory allocation for OS funcs failed\n",0,0,0,0);
	goto init_error;
    }

    if ( usb_drv_ccpu_open(p_dev) != USB_STATUS_OK )
    {
	status = USB_STATUS_NO_MEM;
	goto init_error;
    }

    /*
     * Populate OS functions table
     */ 
    if ( ( status = usb_drv_load_os_functions(p_dev->os_funcs) )!= USB_STATUS_OK ) 
    {
	USB_LOG(DEBUG_USB20,"OS Functions load failed : %d\n",status,0,0,0);
	status = USB_STATUS_ERROR;
	goto init_error;
    }
    

    USB_LOG(DEBUG_USB20,"Initializing USB HAL module\n",0,0,0,0);
    /*
     * Initialize HAL module
     */
    if ( (status = halCpusbInitModule( (HAL_DEVICE **)&(p_dev->hal_dev),
                           p_dev, 
                           &(p_dev->hal_funcs),
                           p_dev->os_funcs,
                           sizeof(OS_FUNCTIONS), 
                           &hal_func_size,
                           0 )) != 0 )
    {
	USB_LOG(USB20_FATAL,"HAL Init failed : %d\n",status,0,0,0);
	status = USB_STATUS_HAL_INIT_FAILED;
	goto init_error;
    }

    if ( hal_func_size != sizeof(HAL_FUNCTIONS) || (NULL == p_dev->hal_dev) )
    {
	USB_LOG(USB20_FATAL,"HAL Init did not return HAL Funcs \n",0,0,0,0);
	status = USB_STATUS_HAL_INIT_FAILED;
        goto init_error;
    }


    /* Define all Device configuration parameters */

    p_dev->link_status  = USB_LINK_DOWN;

    p_dev_param = usb20_config_get_dev_param();

    if ( p_dev_param == NULL ) 
    {
    	 USB_LOG(USB20_FATAL,"Device configuration NULL. \n",0,0,0,0);
	 status = USB_STATUS_ERROR;
        goto init_error;
    }
	
    p_dev->params.usb_base          = p_dev_param->usb_base;
    p_dev->params.usb_offset         =  p_dev_param->usb_offset;
    p_dev->params.reset_base        = p_dev_param->reset_base;
    p_dev->params.reset_bit           = p_dev_param->reset_bit;
    p_dev->params.clkc_base          = p_dev_param->clkc_base;
    p_dev->params.clkc_mask         = p_dev_param->clkc_mask;
    p_dev->params.usb_irq             = p_dev_param->usb_irq;
    p_dev->params.vbus_irq           = p_dev_param->vbus_irq;
    p_dev->params.ucp_base          = p_dev_param->ucp_base;
    p_dev->params.ucp_disable_val  = p_dev_param->ucp_disable_val;
    p_dev->params.ucp_disable_mask = p_dev_param->ucp_disable_mask;
    p_dev->params.speed       = USB_HIGH_LINK_SPEED  /* defined in usb20_config.h */;	
    p_dev->params.mode        = HC_USB_MODE_RNDIS; /* HC_USB_MODE_DEFAULT;*/
    p_dev->params.en_double_buf  = FALSE;
    p_dev->params.ctrl_ep_size =  USB_CTRL_EP_SIZE;
    p_dev->params.bulk_ep_size = USB_HS_BULK_EP_SIZE;
    p_dev->params.int_ep_size = USB_INT_EP_SIZE;
    p_dev->params.pid  = usb20_config_get_pid();
    p_dev->params.vid  = usb20_config_get_vid();
    usb20_config_get_serial_no((char *)&(p_dev->params.serial_no[0]), USB_SERIAL_NO_CHAR_MAX);
	usb20_config_get_manufacturer_string((char *)&(p_dev->params.manufacturer_string[0]),USB_MANUFACTURER_STRING_CHAR_MAX);
	usb20_config_get_product_string((char *)&(p_dev->params.product_string[0]),USB_PRODUCT_STRING_CHAR_MAX);
    p_dev->params.max_power = usb20_config_get_max_power();

    
    /* Pass on all configuration paramaters to HAL */
	
    HAL_CONTROL(p_dev, hkBase, haSETbyVAL, p_dev->params.usb_base);
    HAL_CONTROL(p_dev, hkOffset, haSETbyVAL, p_dev->params.usb_offset);
    HAL_CONTROL(p_dev, hkUsbIntBit, haSETbyVAL, p_dev->params.usb_irq);
    HAL_CONTROL(p_dev, hkVbusIntBit, haSETbyVAL, p_dev->params.vbus_irq);
   	
    HAL_CONTROL(p_dev, hkResetBase, haSETbyVAL, p_dev->params.reset_base);
    HAL_CONTROL(p_dev, hkResetBit, haSETbyVAL, p_dev->params.reset_bit);
    
    HAL_CONTROL(p_dev, hkClkRegBase, haSETbyVAL, p_dev->params.clkc_base);
    HAL_CONTROL(p_dev, hkClkMask, haSETbyVAL, p_dev->params.clkc_mask);
    
    HAL_CONTROL(p_dev, hkUsbIntBit, haSETbyVAL, p_dev->params.usb_irq);
    HAL_CONTROL(p_dev, hkVbusIntBit, haSETbyVAL, p_dev->params.vbus_irq);
    
    HAL_CONTROL(p_dev, hkUcpPresent, haSETbyVAL, TRUE);
    HAL_CONTROL(p_dev, hkUcpRegBase, haSETbyVAL, p_dev->params.ucp_base);
    HAL_CONTROL(p_dev, hkUcpDisVal, haSETbyVAL, p_dev->params.ucp_disable_val);
    HAL_CONTROL(p_dev, hkUcpDisClrMask, haSETbyVAL, p_dev->params.ucp_disable_mask);
    
    HAL_CONTROL(p_dev, hkModeSelect, haSETbyVAL, p_dev->params.mode);
    
    if ( USB_FULL_LINK_SPEED == p_dev->params.speed )
        HAL_CONTROL(p_dev, hkFullSpeed, haSETbyVAL, TRUE);

#ifdef USB_BIG_ENDIAN
    HAL_CONTROL(p_dev, hkChEndianess, haSETbyVAL, HC_BIG_ENDIAN); 
#else
    HAL_CONTROL(p_dev, hkChEndianess, haSETbyVAL, HC_LITTLE_ENDIAN); 
#endif
    value = HC_USB_SUSR_IE | HC_USB_RSTR_IE | HC_USB_VBUS_IE | HC_USB_STPOW_IE | HC_USB_RESR_IE | HC_USB_SETUP_IE;
    HAL_CONTROL(p_dev, hkUsbIntrMask, haSETbyVAL, value );
    value = FALSE;
    HAL_CONTROL(p_dev, hkRemWakeup, haSETbyVAL, value);
    HAL_CONTROL(p_dev, hkLowPwrMode, haSETbyVAL, value);


#ifdef USB_ENABLE_TASK

    g_event_int = SAL_event_create(SAL_EVENT_Q_FIFO, SAL_EVENT_EMPTY);
    strcpy(thread_attr.name,USB_ISR_TASK_NAME);
    thread_attr.priority = 5;
    thread_attr.os_options = 0;
    thread_attr.stacksize = 30000;
    result = SAL_thread_create((SAL_THREAD *)&taskId, (SAL_THREAD_ATTR *)&thread_attr, (SAL_THREAD_HANDLER)usb_task, (void *)p_dev);
    if ( taskId == NULL ) 
    {
	    usb20_logMsg("USB ISR Task Creation Failed\n",0,0,0,0,0,0);
	    return USB_STATUS_ERROR;
    }
#else
#endif /* USB_ENABLE_TASK */ 

   
    USB_EXIT;
	
    *dev = p_dev;                 /* Return back the handle to USB Control Block */
    usb_drv_set_dev(p_dev);  
    USB_LOG(USB20_CONTROL,"USB Driver Initialization complete.\n",0,0,0,0);
    return status;

init_error : 
    SAL_free(p_dev->os_funcs);
    SAL_free(p_dev);
    return status;
}



static INT32  check_cfg(USB_CONFIG *);


/**
 *  @ingroup Protocol_Driver 
 * 
 *  Before calling this routine you must have a successful call to usb_drv_init(). This routine will start the device, hook
 *  necessary interrupt service routines and will bring the driver into a state where data transfer is possible. 
 *
 *  @param  p_dev  IN  Handle of USB Protocol Driver (obtained by usb_drv_start)
 *
 *  @param usbcfg   IN Configuration for USB Protocol Driver
 *
 *  @return  
 *                - USB_STATUS_OK (success)
 *                - Error otherwise  (See @link Error_Codes Error Codes @endlink)
 */
 
USB_STATUS usb_drv_start (USB_DEV *p_dev, USB_CONFIG  *usbcfg)
{
    INT32  status;

    USB_ENTER;
    /**
     * No Channel Setup here. Channels could be setup ONLY when driver
     * has reached to CONNECTED state. This state transition happens ONLY
     * when device detects bus speed correctly. So we shall wait till the 
     * point when Speed is detected.
     */
    p_dev->usb_cfg = usbcfg;


    usb_drv_channel_setup_all(p_dev);
	
    if ( check_cfg(p_dev->usb_cfg) == USB_STATUS_OK ) 
    {
        if ( (status = p_dev->hal_funcs->Open(p_dev->hal_dev)) != 0 )
        {
	    USB_LOG(DEBUG_USB20,"halOpen failed. Return code %d\n",status,0,0,0);
	    status = USB_STATUS_START_FAILED;
        }
        else 
        {
          usb_drv_state_init();
	    status = USB_STATUS_OK;
        }
    }
    else
    {
	status = USB_STATUS_ERROR;
	usb20_logMsg("USB Driver Start failed\n",0,0,0,0,0,0);
    }
    USB_LOG(USB20_CONTROL, "USB Driver Start Complete.\n",0,0,0,0);
    /*sysHwIntEnable (21);  Vlynq Interrupt */
    USB_EXIT;

    return (status);
}



/**
 * @ingroup  Protocol_Driver
 * 
 *   Call this routine to stop the driver. Driver may not release all the resources grabbed so far. However this will 
 *   disable all interrupt service routines. Not data transfer will be possible after this routine is called.
 *
 *   @param  p_dev  IN  Handle for USB Driver (obtained through usb_drv_init)
 * 
 *   @return  
 *                - USB_STATUS_OK (success)
 *                - Error otherwise  (See @link Error_Codes Error Codes @endlink)
 */
 
USB_STATUS usb_drv_stop (USB_DEV *p_dev)
{
    INT32  status;
    INT32  i;

    USB_ENTER;

    /* Mark all EPs as ready, do not keep them in OPEN state */
    for ( i = 0 ; i < USB20_NUM_EP ; i++ ) 
    {
        if (p_dev->ep_table[i].state == USB20_EP_STATE_OPEN )
	     p_dev->ep_table[i].state = USB20_EP_STATE_READY;
    }
   
    if ( (status = p_dev->hal_funcs->Close(p_dev->hal_dev, 0)) != 0 ) 
    {
	 USB_LOG(DEBUG_USB20,"Hal Close failed. Return Code : %d\n",status,0,0,0);
        status = USB_STATUS_STOP_FAILED;
    }

    USB_EXIT;

    return status;
}



/**
 * @ingroup  Protocol_Driver
 * 
 *   @param p_dev IN   Handle for USB Driver (obtained through usb_drv_init)
 * 
 *   @return  
 *                - USB_STATUS_OK (success)
 *                - Error otherwise  (See @link Error_Codes Error Codes @endlink)
 *
 */

USB_STATUS usb_drv_shutdown (USB_DEV *p_dev)
{
    USB_ENTER;

    p_dev->hal_funcs->Shutdown(p_dev->hal_dev);
    SAL_free(p_dev->os_funcs);
    SAL_free(p_dev);
    
    USB_EXIT;
	
    return USB_STATUS_OK;
}



/**
 * @ingroup  Protocol_Driver
 * 
 *   @param p_dev IN  Handle for USB Driver (obtained through usb_drv_init)
 * 
 *   @param ep IN Handle for the End Point over which data transfer is intended.
 * 
 *   @param pkt IN Pointer to a USB Packet.
 * 
 *   @param priv IN Pointer to a private data structure. This pointer will be passed to caller during send_complete call.
 *
 *   @return  
 *                - USB_STATUS_OK (success)
 *                - Error otherwise  (See @link Error_Codes Error Codes @endlink) 
 */



USB_STATUS usb_drv_send (USB_DEV *dev, USB_EP *ep, USB_PKT *pkt, void *priv)
{
    INT32         status;
    USB_SENDINFO  *send_info = NULL;
    INT32  i;
    USB20_RNDIS_PKT  *rndis_pkt;
    
    USB_ENTER;

    /* If the link is not up yet do not send the packet to HAL */
    if ( dev->link_status != USB_LINK_UP )
        return USB_STATUS_ERROR;
    
    rndis_pkt = (USB20_RNDIS_PKT *)priv;

    send_info = &g_send_info[g_send_info_ptr];
    g_send_info_ptr = (g_send_info_ptr+1)%MAX_SEND_INFO_BUF;
    
    if ( send_info == NULL ) 
    {
	 USB_LOG((USB20_FATAL|USB20_ISR),"Malloc failed\n",0,0,0,0);
        status = USB_STATUS_NO_MEM;
	goto end_send;
    }

    send_info->ep          = ep;
    send_info->os_sendinfo = priv;
    send_info->owner       = USB_CLASS_DRIVER;

    if ( USB_EP_T_BULK == ep->type ) 
    {
	    USB_LOG(USB20_DATA,"\n==================\n",0,0,0,0);
	    USB_LOG(USB20_DATA,"Sending BULK Pkt\n",0,0,0,0);
	    USB_LOG(USB20_DATA,"==================\n",0,0,0,0);

    }
    if ( (ep->type == USB_EP_T_BULK) && ((pkt->pkt_size % ep->usb_dev->params.bulk_ep_size) == 0 ) ) 
    	{
    	     rndis_pkt->short_data     = 0x0;
    	     pkt->buf_list[pkt->num_buf].data =  (UINT8 *)&(rndis_pkt->short_data);
	     pkt->buf_list[pkt->num_buf].len   =  1;
	     pkt->pkt_size++;
	     pkt->num_buf++;
    	}
		
    if (  USB_EP_T_CONTROL == ep->type ) 
    {
        /* Enqueue in the control Tx list */
	status = usb_drv_control_send(dev, pkt, send_info);
    }
    else
    {

	/* Flush The Data from Cache */
	for ( i = 0 ; i < pkt->num_buf ; i++ ) 
	{
	      usb_drv_cache_writeback((void *)pkt->buf_list[i].data, (INT32) pkt->buf_list[i].len);
	}

	/* For Interrupt / Bulk end point use CPPI channel send */
        status    = dev->hal_funcs->Send(
		        dev->hal_dev, 
		        (FRAGLIST *)pkt->buf_list, 
		        pkt->num_buf, 
		        pkt->pkt_size, 
		        send_info,
			    usb_drv_hal_send_complete,
		        ep->hal_ch->Channel );

        USB_LOG(DEBUG_USB20,"Return code from Send : %d\n",status,0,0,0);
    }
    
    if ( status == 0 ) 
    {
        status = USB_STATUS_OK;
	 USB_LOG(USB20_CONTROL,"Sent %d\n",pkt->pkt_size,0,0,0);
#if 0 /* Do we need to send 0 byte ?? */
        if ( pkt->pkt_size % ep->max_size == 0  )
	{
	      
             status  = dev->hal_funcs->Send( dev->hal_dev, pkt->buf_list, pkt->num_buf, pkt->pkt_size, send_info, usb_drv_hal_send_complete, ep->hal_ch->Channel );
	}
#endif 
    }
    else
    {
	status = USB_STATUS_ERROR;
	USB_LOG((USB20_FATAL|USB20_ISR),"Send failed\n",0,0,0,0);
    }
    /* Send Zero byte packet in case packet is multiple of buffer size */
		    
    
    USB_EXIT;

end_send :
    return status;
}


/**
 * @ingroup  Protocol_Driver
 * 
 *   @param p_dev IN    Handle for USB Driver (obtained through usb_drv_init) 
 * 
 *   @param command IN  Action to be taken. A list of possible commands are 
 *   documented \link ioctl_cmds here \endlink.
 * 
 *   @param data INOUT  A pointer. Semantics of the pointer is command specific. 
 * 
 *   @return  
 *                - USB_STATUS_OK (success)
 *                - Error otherwise  (See @link Error_Codes Error Codes @endlink)   
 */


USB_STATUS usb_drv_ioctl (USB_DEV *p_dev, UINT32 command, void *data)
{
    INT32  status = USB_STATUS_OK;
    
    USB_ENTER;

    switch ( command )
    {
        case IOCTL_GET_USB_CONFIG    :
		break;
		
        case IOCTL_SET_USB_CONFIG    :
		break;
		
        case IOCTL_GET_USB_STATS   :
		*(unsigned int *)data = (UINT32)&(p_dev->stats);
		break;

        case IOCTL_GET_USB_SPEED   :
		*(unsigned int *)data = (UINT32)(p_dev->params.speed);
		break;
        
        case IOCTL_SET_SERIAL_NO     :
		if (data == NULL)
		    return USB_STATUS_ERROR;
		strcpy((char *)&(p_dev->params.serial_no[0]), (char *)data);
		break;

        case IOCTL_GET_SERIAL_NO     :
		*(unsigned int *)data = (UINT32) &(p_dev->params.serial_no[0]);
		break;

        case IOCTL_SET_VENDOR_ID     :
		p_dev->params.vid = *(UINT16 *)data;
		break;

        case IOCTL_GET_VENDOR_ID     :
		*(UINT16 *)data = p_dev->params.vid;
		break;

        case IOCTL_SET_PRODUCT_ID    :
		p_dev->params.pid = *(UINT16 *)data;
		break;

        case IOCTL_GET_PRODUCT_ID    :
		*(UINT16 *)data = p_dev->params.pid;
		break;

        case IOCTL_SET_MAX_POWER     :
		break;

        case IOCTL_GET_MAX_POWER     :
		*(INT32 *)data = 0;
		break;

        case IOCTL_GET_CTRL_EP_SIZE  :
		*(INT32 *)data = p_dev->params.ctrl_ep_size;
		break;

        case IOCTL_GET_BULK_EP_SIZE  :
             *(INT32 *)data = p_dev->params.bulk_ep_size;
		break;

        case IOCTL_GET_INT_EP_SIZE   :
		*(INT32 *)data = p_dev->params.int_ep_size;
		break;

        case IOCTL_GET_ATTRIB	     :
		*(INT32 *)data = 0;
		break;
#ifdef USB_MIB2	
        case IOCTL_GET_USB_PARAMS : 
              /* Assumed that pointer data is pointing to a valid memory block of proper size, see usb20_mib2.h for structure definition */              
              rndis_usb_get_mib_stats( hal_usb_get_app_handle(p_dev), SIOCGUSBPARAMS, (void *)data);
		break;

	 case IOCTL_GET_IF_COUNTERS : 	 				  
              /* Assumed that pointer data is pointing to a valid memory block of proper size, see usb20_mib2.h for structure definition */              
              rndis_usb_get_mib_stats( hal_usb_get_app_handle(p_dev), SIOCGINTFCOUNTERS, (void *)data);	
	 	break;
#endif /* USB_MIB2 */
		
        case IOCTL_GET_MANUFACTURER_STRING     :
		*(unsigned int *)data = (UINT32) &(p_dev->params.manufacturer_string[0]);
		break;

        case IOCTL_GET_PRODUCT_STRING     :
		*(unsigned int *)data = (UINT32) &(p_dev->params.product_string[0]);
		break;
	default : 
		break;
    }

    USB_EXIT;

    return status;
}


/**
 * @ingroup  Protocol_Driver
 * 
 *   Opens End Point when requested by Class Driver. 
 *
 *   @param p_dev IN  Handle for USB Driver (obtained through usb_drv_init)  
 *
 *   @param pipe IN Pointer to the pipe structure. 
 *
 *   @param p_ep OUT  A handle to an open end point will be passed back to the class driver.  
 *
 *   @return  
 *                - USB_STATUS_OK (success)
 *                - Error otherwise  (See @link Error_Codes Error Codes @endlink)   
 */


USB_STATUS usb_drv_ep_open (USB_DEV *p_dev, USB_PIPE *pipe, USB_EP **p_ep)
{
    INT32          status    = USB_STATUS_OK;
    USB_EP         *ep_table;
    CHANNEL_INFO   *p_ch     = NULL;

    USB_ENTER;

    ep_table = p_dev->ep_table;
    *p_ep = NULL;  /* Set it to NULL in case we can't allocate a valid EP */
    
    switch (pipe->type)
    {
	
	/**
	 * Class Driver CONTROL End Point open request will be served by opening 
	 * End Point Number 0 (No Channel is associated with Control EP). 
	 */
        case USB_EP_T_CONTROL : 
		if ( p_dev->ep_control.state != USB20_EP_STATE_CLOSED )
		{
		    /* Control End point already in use */
		    status = USB_STATUS_EP_IN_USE;
		    USB_LOG(DEBUG_USB20,"Control End Point in use.\n",0,0,0,0);
		}
		else
		{
		    /* Setup the control end point */
		    p_dev->ep_control.type     = USB_EP_T_CONTROL;
		    p_dev->ep_control.dir      = pipe->dir;
		    p_dev->ep_control.id       = 0;
		    p_dev->ep_control.max_size = USB_EP_CONTROL_MAX_SIZE;
		    p_dev->ep_control.pipe     = pipe;
		    p_dev->ep_control.usb_dev  = p_dev;
		    p_dev->ep_control.hal_ch   = NULL;  /* No CPPI Channel for EP0 */
		    p_dev->ep_control.status   = 0;
                  p_dev->ep_control.state    = USB20_EP_STATE_OPEN; /* Control EP is setup by default by HAL */
		    *p_ep = &(p_dev->ep_control);
		}
		break;
		
	/**
	 * Class Driver INT End Point open request will be served by opening 
	 * End Point Number 0 (Channel number 0). 
	 */	
	case USB_EP_T_INT :
		/* Validate direction, Interrupt End Point can only be IN */
		if ( pipe->dir != USB_EP_DIR_IN )
		{
		    status = USB_STATUS_INV_EP_DIR;
		    goto end_ep_open;
		}

		if ( ep_table[1].state != USB20_EP_STATE_CLOSED )
		{
		    /* Control End point already in use */
		    status = USB_STATUS_EP_IN_USE;
		    USB_LOG(DEBUG_USB20,"Interrupt End Point in use.\n",0,0,0,0);
		}
		else
		{
		    /* Allocate a channel structure */
		    if ( (p_ch = usb_drv_alloc_hal_channel(pipe,1) ) == NULL)
                    {
                        status = USB_STATUS_NO_CHANNEL;
			USB_LOG(DEBUG_USB20,"Channel Allocation failed\n",0,0,0,0);
                    }
		    else 
		    {
			/* Put the channel map in mcb */
			p_dev->channel[1].ep_no = 2;
			p_dev->channel[1].dir   = pipe->dir;
			p_dev->channel[1].status = USB20_CHANNEL_DOWN; /* Still not up */
			p_dev->channel[1].halch = p_ch;
				
			/* Setup the control end point */
		          ep_table[1].type     = USB_EP_T_INT;
		          ep_table[1].dir      = pipe->dir;
                        ep_table[1].id       = 2;
                        ep_table[1].max_size = USB_EP_INT_MAX_SIZE;
                        ep_table[1].pipe     = pipe;
                        ep_table[1].usb_dev  = p_dev;
                        ep_table[1].hal_ch   = p_ch;
                        ep_table[1].status   = 0;
                        ep_table[1].state    = USB20_EP_STATE_READY; /* Control EP is setup by default by HAL */
                        *p_ep = &(ep_table[1]);
			
		    }
		}
		break;
		
	/**
	 * Class Driver BULK End Point open request will be served by opening 
	 * End Point Number 1 (Channel number 1). 
	 */
	case USB_EP_T_BULK : 
		if ( ep_table[0].state != USB20_EP_STATE_CLOSED )
		{
		    /* Control End point already in use */
		    status = USB_STATUS_EP_IN_USE;
		    USB_LOG(DEBUG_USB20,"Bulk End Point in use.\n",0,0,0,0);
		}
		else
		{
		    /* Allocate a channel structure */
		    if ( (p_ch = (CHANNEL_INFO *)usb_drv_alloc_hal_channel(pipe,0) ) == NULL)
                    {
                        status = USB_STATUS_NO_CHANNEL;
			   USB_LOG(DEBUG_USB20,"Channel Allocation failed\n",0,0,0,0);
                    }
		    else 
		    {
			/* Put the channel map in mcb */
			   p_dev->channel[0].ep_no  = 1;
			   p_dev->channel[0].dir    = pipe->dir;
			   p_dev->channel[0].status = USB20_CHANNEL_DOWN; /* Still not up */
			   p_dev->channel[0].halch  = p_ch;
				
			/* Setup the control end point */
		          ep_table[0].type     = USB_EP_T_BULK;
		          ep_table[0].dir      = pipe->dir;
                        ep_table[0].id       = 1;
                        ep_table[0].max_size = USB_EP_BULK_MAX_SIZE;
                        ep_table[0].pipe     = pipe;
                        ep_table[0].usb_dev  = p_dev;
                        ep_table[0].hal_ch   = p_ch;
                        ep_table[0].status   = 0;
                        ep_table[0].state    = USB20_EP_STATE_READY; /* Control EP is setup by default by HAL */
                        *p_ep = &(ep_table[0]);

		    }
		}
		break;
		
	default :
		status = USB_STATUS_INV_EP_TYPE;  /* Invalid End point request */
		USB_LOG(DEBUG_USB20,"Unknown End Point type.\n",0,0,0,0);
		
    }
    USB_EXIT;

end_ep_open : 
    return status;
}



/**
 * @ingroup  Protocol_Driver
 * 
 *   This routine may be called to close an open end point. The closed end point will be reused in 
 *   future open calls. 
 * 
 *   @param p_ep IN  A handle to an open end point.  
 *
 *   @return  
 *                - USB_STATUS_OK (success)
 *                - Error otherwise  (See @link Error_Codes Error Codes @endlink)   
 */

USB_STATUS usb_drv_ep_close (USB_EP *ep)
{
    USB_ENTER;
    USB_EXIT;
    return USB_STATUS_OK;
}



static INT32 check_cfg(USB_CONFIG *cfg)
{
   if ( !cfg->device_desc || !cfg->config || !cfg->string )
	   return USB_STATUS_ERROR;
   else
	   return USB_STATUS_OK;
}

