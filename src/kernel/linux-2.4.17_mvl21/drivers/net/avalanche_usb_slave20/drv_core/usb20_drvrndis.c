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
 * FILE NAME    :   usb20_drvrndis.c
 *
 * DESCRIPTION  : RNDIS-Protocol Driver interface
 *
 * HISTORY :
 *
 * Aug 05   -   Prashanth Acharya   -   Created.
 * Sep 10   -   sabyasachi          -   Fixed string descriptor length
 * Sep 19   -   sabyasachi          -   Fixed string desc population
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/

#include "usb20_drvrndis.h"
#include "usb20_rndis.h"
#include "usb_sal.h"

/***************************Externs*************************************/
extern void  sysRndisDeallocateBuffer (HAL_HND h_end, char *ptr, UINT32 rx_chain_id);
extern char* sysRndisAllocateBuffer (HAL_HND h_end, UINT32 *rx_chain_id);
extern char ** usb_app_strings;



/*********************Data Structures *****************************/
typedef struct USB20_RNDIS_WRAP_MCB
{
    HAL_USB_TX_CMPLT_RTN rndis_ctrl_tx_complete;
    HAL_USB_RECEIVE_RTN rndis_ctrl_rcv;
    FUNCPTR rndis_bulk_tx_complete;
    FUNCPTR rndis_bulk_rcv;
    FUNCPTR rndis_intr_tx_complete;

    HAL_USB_IOCTL ioctl_rtn;
    HAL_USB_NOTIFY_RTN notify_rtn;

    USB_PIPE rndis_ctrl_ep;
    USB_PIPE rndis_bulk_ep;
    USB_PIPE rndis_intr_ep;

    USB_EP * hnd_bulk_ep;
    USB_EP * hnd_ctrl_ep;
    USB_EP * hnd_intr_ep;

    CLASS_DEV_FUNCS cd_funcs;
    CLASS_DEV rndis_class_dev;


    USB_INTERFACE_DESCR * intf_desc[2];
    USB_ENDPOINT_DESCR * endp_desc[3];
    USB_STRING_DESCR * str_desc[6];   /*Assuming max 6 string descriptors */
    USB_CONFIG usb20_rndis_config;


    HAL_HND * hnd_device;
    RNDIS_MCB* hnd_rndis_mcb;
    
    USB20_RNDIS_PKT *ep_tx_free_list;                             
}USB20_RNDIS_WRAP_MCB_T;



USB20_RNDIS_WRAP_MCB_T* usb20_rndis_mcb;

/*****************Definitions*******************************************/


/*********** Forward declarations **********************************/
static INT32 usb_rndis_ctrl_tx_complete_cb (USB_PIPE * ep, void * pkt_id);
static INT32 usb_rndis_bulk_tx_complete_cb (USB_PIPE * ep, void * pkt_id);
static INT32 usb_rndis_intr_tx_complete_cb (USB_PIPE * ep, void * pkt_id);
static INT32 usb_rndis_notify_cb(UINT32 event, void * data);
static INT32 usb_rndis_query_cb(UINT32 query_id, void * data);
static void * usb_rndis_alloc_rx_buffer_cb (UINT32 size,USB_PIPE * ep,void * buf_info);
static INT32 usb_rndis_ctrl_rcv_cb (USB_PIPE * ep, USB_PKT *pkt );
static INT32 usb_rndis_bulk_rcv_cb (USB_PIPE * ep, USB_PKT *pkt);
static void usb_rndis_free_rx_buffer_cb (void * mem_ptr, void * buf_info);
static USB20_RNDIS_PKT *  usb20_rndis_get_pkt(void);
static int usb20_rndis_free_pkt(USB20_RNDIS_PKT * pkt);
int usblogMsg (unsigned int,char *,int, int, int, int,int);
static void usb_rndis_reset_handler(void);
int sys_critical_start (void* p_context);
int sys_critical_end (void* p_context);


/**************************************************************************
 * FUNCTION NAME : hal_usb_dev_open
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
 
HAL_HND hal_usb_dev_open (HAL_SYS_TBL * sys_ptr)
{
    HAL_HND h_device;
    UINT32 index;
    int error_status=0;

    usb20_rndis_mcb = (USB20_RNDIS_WRAP_MCB_T *) SAL_malloc(sizeof(USB20_RNDIS_WRAP_MCB_T));
    if(usb20_rndis_mcb == NULL)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Unable to allocate memory for USB20 RNDIS MCB.\n",0,0,0,0,0);
        return NULL;

    }
    
    memset((void *)usb20_rndis_mcb,0,sizeof(USB20_RNDIS_WRAP_MCB_T));

    /********** Initialize EP structures***************/

    /* Initialize control endpoint structure */
    usb20_rndis_mcb->rndis_ctrl_ep.type = USB_EP_T_CONTROL;
    usb20_rndis_mcb->rndis_ctrl_ep.dir  = USB_EP_DIR_INOUT;
    usb20_rndis_mcb->rndis_ctrl_ep.receive = (EP_RCV_CB) usb_rndis_ctrl_rcv_cb;
    usb20_rndis_mcb->rndis_ctrl_ep.send_complete = (EP_SEND_COMPLETE_CB) usb_rndis_ctrl_tx_complete_cb;
    usb20_rndis_mcb->rndis_ctrl_ep.class_drv_info = (void *)usb20_rndis_mcb;

    /*Initialize bulk endpoint structure */
    usb20_rndis_mcb->rndis_bulk_ep.type = USB_EP_T_BULK;
    usb20_rndis_mcb->rndis_bulk_ep.dir  = USB_EP_DIR_INOUT;
    usb20_rndis_mcb->rndis_bulk_ep.receive = (EP_RCV_CB)usb_rndis_bulk_rcv_cb;
    usb20_rndis_mcb->rndis_bulk_ep.send_complete = (EP_SEND_COMPLETE_CB)usb_rndis_bulk_tx_complete_cb;
    usb20_rndis_mcb->rndis_bulk_ep.class_drv_info = (void *)usb20_rndis_mcb;

    /* Initialize interrupt endpoint structure */
    usb20_rndis_mcb->rndis_intr_ep.type = USB_EP_T_INT;
    usb20_rndis_mcb->rndis_intr_ep.dir  = USB_EP_DIR_IN;
    usb20_rndis_mcb->rndis_intr_ep.receive = NULL;
    usb20_rndis_mcb->rndis_intr_ep.send_complete = usb_rndis_intr_tx_complete_cb;
    usb20_rndis_mcb->rndis_intr_ep.class_drv_info = (void *)usb20_rndis_mcb;

 
    /*******Initialize the class driver funcs structure********/
    usb20_rndis_mcb->cd_funcs.usb_class_dev_event = (void *) usb_rndis_notify_cb;
    usb20_rndis_mcb->cd_funcs.usb_class_dev_query = (void *)usb_rndis_query_cb;
    usb20_rndis_mcb->cd_funcs.MallocBuffer = (void *) usb_rndis_alloc_rx_buffer_cb ;
    usb20_rndis_mcb->cd_funcs.FreeBuffer = (void *) usb_rndis_free_rx_buffer_cb;


        
    /******* Initialize the class dev structure *********/
    usb20_rndis_mcb->rndis_class_dev.funcs = &usb20_rndis_mcb->cd_funcs;
    usb20_rndis_mcb->rndis_class_dev.priv = NULL;  /* what is this used for ? */
    

    {   /********** Initialize the tx list ***************/

        for (index =0; index < USB20_RNDIS_MAX_PKTS; index++)
        {
            USB20_RNDIS_PKT * p_pkt;
        
            /* Allocate memory for the RNDIS PKT */
            p_pkt = (USB20_RNDIS_PKT *)SAL_malloc (sizeof (USB20_RNDIS_PKT));
            if (p_pkt == NULL)
            {
                usblogMsg (HAL_SYS_FATAL,"Error: Unable to allocate memory for the RNDIS PKT list.\n",0,0,0,0,0);
                return NULL;
            }

        /* Initialize the memory block. */
        memset((void *)p_pkt, 0, sizeof (USB20_RNDIS_PKT));
        p_pkt->usb20_pkt.buf_list = &p_pkt->buf[0];  /* buf_list uses the buf[] of RNDIS_PKT */

        /* Add the buffer to the free list. */
        usb_list_add ((USB_LIST_NODE **)&usb20_rndis_mcb->ep_tx_free_list, (USB_LIST_NODE *)p_pkt);
        }
    } /********** Done initializing the tx list *****************/
    

    /* Call the Protocol driver init function */
    error_status = usb_drv_init(&usb20_rndis_mcb->rndis_class_dev,(USB_DEV **)&h_device);
    if(error_status)
    {
  	 usb20_logMsg("Driver Init failed\n",0,0,0,0,0,0);
        h_device= NULL;
    }
	
    USB_LOG(USB20_CONTROL, "Driver Init Successful\n",0,0,0,0);
    usb20_rndis_mcb->hnd_device = h_device;

    return h_device;
}


/**************************************************************************
 * FUNCTION NAME : hal_usb_dev_start
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/

HAL_USB_STRING_ENTRY  *p_str; /* Added by Sabya */
int hal_usb_dev_start (HAL_HND hnd_device, HAL_USB_APP_INFO* ptr_app_info)
{
    UINT32 error_status;
    RNDIS_MCB * ptr_rndis_mcb;
    HAL_USB_APP_INFO * p_app_info;
    UINT8  index = 0;      /* Local for looping through string descriptors*/

    
    p_app_info = (HAL_USB_APP_INFO *)ptr_app_info;
    
    ptr_rndis_mcb = (RNDIS_MCB * )p_app_info->h_usbapp;
    usb20_rndis_mcb->rndis_ctrl_rcv = p_app_info->receive_rtn;
    usb20_rndis_mcb->rndis_ctrl_tx_complete = p_app_info->tx_cmplt_rtn;
    usb20_rndis_mcb->ioctl_rtn  = p_app_info->ioctl_rtn;
    usb20_rndis_mcb->notify_rtn = p_app_info->notify_rtn;
    
    /* Store this back pointer for later use */
    usb20_rndis_mcb->hnd_rndis_mcb = ptr_rndis_mcb;

    /***************Prepare the config structure **************/
    usb20_rndis_mcb->usb20_rndis_config.device_desc = ptr_rndis_mcb->ptr_device_desc;
    usb20_rndis_mcb->usb20_rndis_config.config = (void *)ptr_rndis_mcb->ptr_config_desc;
    usb20_rndis_mcb->usb20_rndis_config.config_desc_length = sizeof(RNDIS_USB_CONFIG);
    
    usb20_rndis_mcb->intf_desc[0]= &(ptr_rndis_mcb->ptr_config_desc->comm_iface);
    usb20_rndis_mcb->intf_desc[1]=&(ptr_rndis_mcb->ptr_config_desc->data_iface);
    usb20_rndis_mcb->endp_desc[0]=&(ptr_rndis_mcb->ptr_config_desc->interrupt_ep);
    usb20_rndis_mcb->endp_desc[1]=&(ptr_rndis_mcb->ptr_config_desc->bulkIn_ep);
    usb20_rndis_mcb->endp_desc[2]=&(ptr_rndis_mcb->ptr_config_desc->bulkOut_ep);

    usb20_rndis_mcb->usb20_rndis_config.if_desc_tab = &usb20_rndis_mcb->intf_desc[0];
    usb20_rndis_mcb->usb20_rndis_config.num_if_desc = 2;
    usb20_rndis_mcb->usb20_rndis_config.ep_desc_tab = &usb20_rndis_mcb->endp_desc[0];
    usb20_rndis_mcb->usb20_rndis_config.num_ep_desc = 3;


    {
    /* ************Create the string descriptor entries ********/
    
    /* The first descriptor is the "supported Languages" descriptor */
    index=0;
    p_str = (HAL_USB_STRING_ENTRY *)p_app_info->ioctl_rtn ((HAL_HND) hnd_device, USB_APP_IOTCL_GET_STRING_DESC, (void *)&index, NULL, NULL);

    /* Added by Sabya */
    if ( p_str ) 
    {
        usb20_rndis_mcb->str_desc[index] = (USB_STRING_DESCR *)(p_str->p_usb_str_descr);
        index++;
    }
    else
    {
	usb20_logMsg("ERROR in String Initialization\n",0,0,0,0,0,0);
	return -1;
	
    }

        /* Now run thru all the remaining descriptors */
        while(usb_app_strings[index-1]!=NULL)
        {
            p_str = p_app_info->ioctl_rtn ((HAL_HND) hnd_device, USB_APP_IOTCL_GET_STRING_DESC, (void *)&index, NULL, NULL);
    /* Added by Sabya */
	    if ( p_str ) 
	    {
                usb20_rndis_mcb->str_desc[index] = (USB_STRING_DESCR *)(p_str->p_usb_str_descr);

                index++;
	    }
	    else
	    {
	        usb20_logMsg("ERROR in String Initialization\n",0,0,0,0,0,0);
	        return -1;
	    }
    
    
        }
    
    } /****finished creating string descriptors entries*****/
    
     /* update in the config structure */   
    usb20_rndis_mcb->usb20_rndis_config.string = (USB_STRING_DESCR **)&usb20_rndis_mcb->str_desc[0];
    usb20_rndis_mcb->usb20_rndis_config.num_strings = index;

    /**************Done initializing the config structure ************/
    

    /*******Update EP structure with the new info *********/
    /*Update pipe info for bulk EP*/
    usb20_rndis_mcb->rndis_bulk_ep.epd_in = &ptr_rndis_mcb->ptr_config_desc->bulkIn_ep;
    usb20_rndis_mcb->rndis_bulk_ep.epd_out = &ptr_rndis_mcb->ptr_config_desc->bulkOut_ep;

    /* Update pipe info for interrupt EP */
    usb20_rndis_mcb->rndis_intr_ep.epd_in = &ptr_rndis_mcb->ptr_config_desc->interrupt_ep;
    usb20_rndis_mcb->rndis_intr_ep.epd_out = NULL;
    
    /* No descriptors for EP0 */
    usb20_rndis_mcb->rndis_ctrl_ep.epd_in = NULL;
    usb20_rndis_mcb->rndis_ctrl_ep.epd_out = NULL;


     /***************** open the EPs ***********************/
    {
           /* open all pipes and maintain static list */

         error_status = usb_drv_ep_open(hnd_device,&usb20_rndis_mcb->rndis_ctrl_ep,&usb20_rndis_mcb->hnd_ctrl_ep );
         if(error_status)
           {
            usblogMsg(HAL_SYS_FATAL,"Unable to open control endpoint\n",0,0,0,0,0);
            return error_status;
           }

         error_status = usb_drv_ep_open(hnd_device,&usb20_rndis_mcb->rndis_bulk_ep,&usb20_rndis_mcb->hnd_bulk_ep );
         if(error_status)
           {
            usblogMsg(HAL_SYS_FATAL,"Unable to open bulk endpoint\n",0,0,0,0,0);
            return error_status;
           }

         error_status = usb_drv_ep_open(hnd_device,&usb20_rndis_mcb->rndis_intr_ep, &usb20_rndis_mcb->hnd_intr_ep );           
         if(error_status)
           {
            usblogMsg(HAL_SYS_FATAL,"Unable to open interrupt endpoint\n",0,0,0,0,0);
            return error_status;
           }
           
    }
     /*******finished opening EPs*************************/
    

    /* Call the Protocol driver start function */
    error_status = usb_drv_start(hnd_device, &usb20_rndis_mcb->usb20_rndis_config);


    return error_status;
}

/**************************************************************************
 * FUNCTION NAME : hal_usb_open_pipe
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/

HAL_HND hal_usb_open_pipe(HAL_HND dev_ptr, UINT32 pipe_type, void* cb_rtn)
{
    HAL_HND hnd_pipe = NULL;


    switch(pipe_type)
    {
        case HAL_USB_CTRL_IN:
            {
                hnd_pipe = usb20_rndis_mcb->hnd_ctrl_ep;
                break;
            }
        case HAL_USB_CTRL_OUT:
            {
                hnd_pipe = usb20_rndis_mcb->hnd_ctrl_ep;
                break;
            }
        case HAL_USB_BULK_IN:
            {
                usb20_rndis_mcb->rndis_bulk_tx_complete = (FUNCPTR) cb_rtn;
                hnd_pipe = usb20_rndis_mcb->hnd_bulk_ep;
                break;
            }
        case HAL_USB_BULK_OUT:
            {
                usb20_rndis_mcb->rndis_bulk_rcv= (FUNCPTR) cb_rtn;
                hnd_pipe = usb20_rndis_mcb->hnd_bulk_ep;
                break;
            }
        case HAL_USB_INTERRUPT_IN:
            {
                hnd_pipe = usb20_rndis_mcb->hnd_intr_ep;
                break;
            }
        default:
            {
                usblogMsg(HAL_SYS_FATAL,"Trying to open invalid endpoint\n",0,0,0,0,0);
                break;
            }

    }

return hnd_pipe;
}

/**************************************************************************
 * FUNCTION NAME : hal_usb_dev_close
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/

int hal_usb_dev_close(HAL_HND dev_ptr)
{


		
    /* Close all the EPs */
    usb_drv_ep_close(usb20_rndis_mcb->hnd_bulk_ep);
    usb_drv_ep_close(usb20_rndis_mcb->hnd_ctrl_ep);
    usb_drv_ep_close(usb20_rndis_mcb->hnd_intr_ep);

    /* Stop the protocol driver  */
    usb_drv_stop(dev_ptr);

    /* Shutdown the protocol driver */
    usb_drv_shutdown(dev_ptr);

    /* Close and free all the local data structures */

      /* Free the pool of PKT structures */

    {
        USB20_RNDIS_PKT * p_pkt;

        p_pkt = usb20_rndis_mcb->ep_tx_free_list;

        while (p_pkt != NULL)
        {
         p_pkt= usb20_rndis_get_pkt();
         SAL_free((void *)p_pkt);
        }
    }

    /* free the MCB structure */
    SAL_free((void *)usb20_rndis_mcb);

    return 0;
    
}

/**************************************************************************
 * FUNCTION NAME : hal_usb_dev_ioctl
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/

int hal_usb_dev_ioctl(HAL_HND device,UINT32 command, void * arg)
{
    UINT32 return_val = -1;

        
    switch(command)
    {
        case HAL_USB_GET_EP_SIZE:
            {
                HAL_USB_EP_ID * ep_id = (HAL_USB_EP_ID *)arg;
                    switch(*ep_id)
                    {
                        case HAL_USB_CTRL_IN:
                        case HAL_USB_CTRL_OUT:
                            usb_drv_ioctl(device,IOCTL_GET_CTRL_EP_SIZE,(UINT8 *)&return_val);
                            break;

                        case HAL_USB_BULK_IN:
                        case HAL_USB_BULK_OUT:
                            usb_drv_ioctl(device,IOCTL_GET_BULK_EP_SIZE,(UINT8 *)&return_val);
                            break;

                        case HAL_USB_INTERRUPT_IN:
                            usb_drv_ioctl(device,IOCTL_GET_INT_EP_SIZE,(UINT8 *)&return_val);
                            break;
                            
                        default :
                            usblogMsg(HAL_SYS_FATAL,"Invalid Endpoint\n",0,0,0,0,0);
                            break;
                    }
             }
		break;
        case HAL_USB_IOCTL_GET_ATTRIB:
            return_val = usb_drv_ioctl(device,IOCTL_GET_ATTRIB,(UINT8 *)arg);
            break;
            
        case HAL_USB_IOCTL_GET_MAX_POWER:
            return_val = usb_drv_ioctl(device, IOCTL_GET_MAX_POWER,(UINT8 *)arg);
            break;
            
        default:
            usblogMsg(HAL_SYS_FATAL,"Invalid IOCTL command\n",0,0,0,0,0);

            break;
    }
    return return_val;
}

/**************************************************************************
 * FUNCTION NAME : hal_usb_dev_poll
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/

int  hal_usb_dev_poll (HAL_HND device, HAL_HND endpoint)
{
    /*Do we need to poll ?*/
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : hal_usb_get_app_handle
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/

HAL_HND hal_usb_get_app_handle (HAL_HND dev)
{
    if(dev == usb20_rndis_mcb->hnd_device && dev != NULL )
        return (HAL_HND)usb20_rndis_mcb->hnd_rndis_mcb;
    else
    {
        usblogMsg(HAL_SYS_FATAL,"Invalid device handle\n",0,0,0,0,0);
        return NULL;
    }
    return NULL;     
}

/**************************************************************************
 * FUNCTION NAME : hal_usb_get_app_handle
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/

HAL_HND hal_usb_get_app_handle_from_endpoint(HAL_HND endpoint)
{

    USB_EP * ep;

    ep= (USB_EP *)endpoint;

/* Sabya - just a simple hack */
        return usb20_rndis_mcb->hnd_rndis_mcb;

    if(ep==usb20_rndis_mcb->hnd_bulk_ep || ep==usb20_rndis_mcb->hnd_ctrl_ep  
            || ep==usb20_rndis_mcb->hnd_intr_ep )
    {
        return usb20_rndis_mcb->hnd_rndis_mcb;
    }
    else
    {
        usblogMsg(HAL_SYS_FATAL,"Unknown endpoint\n",0,0,0,0,0);
        return NULL;
    }
            
    return NULL;
}


/**************************************************************************
 * FUNCTION NAME : hal_usb_dev_send_pipe
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
int hal_usb_dev_send_pipe(HAL_HND ep, UINT32 data[], UINT32 len[], UINT32 num_buf, UINT32 pkt_id)
{
    USB20_RNDIS_PKT *rndis_pkt;
    USB_PKT * pkt;
    UINT32 index;
	
    /* Get a rndis packet from the pool */
    rndis_pkt = usb20_rndis_get_pkt();

    if(rndis_pkt == NULL)
    {
        usblogMsg(HAL_SYS_FATAL,"Unable to get free USB20 RNDIS PKT\n",0,0,0,0,0);
        return -1;
    }

    /* Copy the OS packet id */
    rndis_pkt->os_pkt_id = pkt_id;
    
    /* Prepare the packet structure to be passed to protocol driver */
    pkt = &(rndis_pkt->usb20_pkt);
    pkt->num_buf= num_buf;
    pkt->pkt_size=0;
    
    for(index=0;index<num_buf;index++)
    {
        pkt->buf_list[index].data = (UINT8 *)data[index];
        pkt->buf_list[index].len = len[index];
        pkt->pkt_size+=len[index];   /* update packet size */
    }

    return usb_drv_send((USB_DEV* )usb20_rndis_mcb->hnd_device,(USB_EP *)ep,pkt,(void *)rndis_pkt);
}


/* Call backs query and notify */

/**************************************************************************
 * FUNCTION NAME : usb_rndis_notify_cb
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
static INT32 usb_rndis_notify_cb(UINT32 event, void * data)
{
    UINT32  size;

    switch(event)
    {
        case USB_EVENT_LINK_UP:
		   /* Data is link speed */
		   size = (UINT32)data;
		   usb20_rndis_mcb->hnd_rndis_mcb->ptr_config_desc->bulkOut_ep.wMaxPacketSize = HAL_SYS_SWAP((UINT16)size);
   		   usb20_rndis_mcb->hnd_rndis_mcb->ptr_config_desc->bulkIn_ep.wMaxPacketSize   = HAL_SYS_SWAP((UINT16)size);
                break;
        case USB_EVENT_LINK_DOWN:
                break;
        case USB_EVENT_RESET :
                
                usb_rndis_reset_handler();

                usb20_rndis_mcb->notify_rtn(usb20_rndis_mcb->hnd_device, HAL_USB_RESET_EVENT);
                break;
        case USB_EVENT_SUSPEND:
                break;
        case USB_EVENT_DATA_XMIT:
                break;
        case USB_EVENT_DATA_RECV:
                break;
        default:
                usblogMsg(HAL_SYS_FATAL,"Unknown USB Event\n",0,0,0,0,0);
            break;
    }

return 0;
}

/**************************************************************************
 * FUNCTION NAME : rndis_query_cb
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
static INT32 usb_rndis_query_cb(UINT32 query_id, void * data)
{
/*    usb20_rndis_mcb->ioctl_rtn();*/ 

return 0;
}



/* local functions */

/*Endpoint callback functions*/

/**************************************************************************
 * FUNCTION NAME : usb_rndis_bulk_tx_complete_cb
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
static INT32 usb_rndis_bulk_tx_complete_cb (USB_PIPE * ep, void * pkt_id)
{
    USB20_RNDIS_PKT * pkt;

    pkt = (USB20_RNDIS_PKT *)pkt_id;
    
    if(usb20_rndis_mcb->rndis_bulk_tx_complete != NULL)
    {
        
        /* get the os pkt id from USB20_RNDIS_PKT structure 
         * and invoke the supplied callback
         */
         
        usb20_rndis_mcb->rndis_bulk_tx_complete((UINT32)(usb20_rndis_mcb->hnd_device),pkt->os_pkt_id);

    }

    /* Give up the USB20 RNDIS PKT structure */
    usb20_rndis_free_pkt(pkt);
    
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : usb_rndis_ctrl_tx_complete_cb
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
static INT32 usb_rndis_ctrl_tx_complete_cb (USB_PIPE * ep, void * pkt_id)
{
    USB20_RNDIS_PKT * pkt;

    pkt = (USB20_RNDIS_PKT *)pkt_id;
    
    if(usb20_rndis_mcb->rndis_ctrl_tx_complete != NULL)
    {
         /* get the os pkt id from USB20_RNDIS_PKT structure 
          * and invoke the supplied callback
          */
    
        usb20_rndis_mcb->rndis_ctrl_tx_complete(usb20_rndis_mcb->hnd_device,pkt->os_pkt_id);
    }

    /* Give up the USB20 RNDIS PKT structure */
    usb20_rndis_free_pkt(pkt);


    return 0;
}

/**************************************************************************
 * FUNCTION NAME : usb_rndis_intr_tx_complete_cb
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
static INT32 usb_rndis_intr_tx_complete_cb (USB_PIPE * ep, void * pkt_id)
{
    USB20_RNDIS_PKT * pkt;

    pkt = (USB20_RNDIS_PKT *)pkt_id;

    if(usb20_rndis_mcb->rndis_intr_tx_complete != NULL)
    {
         /* get the os pkt id from USB20_RNDIS_PKT structure 
          * and invoke the supplied callback
          */
    
        usb20_rndis_mcb->rndis_intr_tx_complete((UINT32)(usb20_rndis_mcb->hnd_device),pkt->os_pkt_id);
    }

    /* Give up the USB20 RNDIS PKT structure */
    usb20_rndis_free_pkt(pkt);

    return 0;
}

/**************************************************************************
 * FUNCTION NAME : usb_rndis_bulk_rcv_cb
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
static INT32 usb_rndis_bulk_rcv_cb (USB_PIPE * pipe, USB_PKT *pkt)
{
    int return_value  = -1 ;
    
    if(usb20_rndis_mcb->rndis_bulk_rcv != NULL)
    {

        if(pkt->num_buf ==1) /* We expect all data in a single buffer */
        {
            return_value = usb20_rndis_mcb->rndis_bulk_rcv((UINT32)pipe, pkt);
        }
        else
        {
            
            /* Temporarily flagging as an error */
            usblogMsg(HAL_SYS_FATAL,"Received bulk data in multiple buffers\n",0,0,0,0,0);
            return_value = -1;
        }
    }
    return return_value;
}

/**************************************************************************
 * FUNCTION NAME : usb_rndis_ctrl_rcv_cb
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
static int usb_rndis_ctrl_rcv_cb (USB_PIPE * ep, USB_PKT *pkt )
{
    int return_value = -1;


    if(usb20_rndis_mcb->rndis_ctrl_rcv != NULL)
    {
        if(pkt->num_buf ==1)                  /* usb20_rndis.c expects only one buffer of control data */
        {
            return_value = usb20_rndis_mcb->rndis_ctrl_rcv(ep,(UINT32 *)&(pkt->buf_list[0].data),(UINT32 *)&(pkt->buf_list[0].len),1);
        }
        else
        {
            /* TODO:copy into one buffer and send to rndis */
            /*  Can we expect this not to happen??? */ 
            /* Temporarily flagging as an error */
            usblogMsg(HAL_SYS_FATAL,"Received Control data in multiple buffers\n",0,0,0,0,0);
            return_value = -1;
        }
    }

    return return_value;
}

/**************************************************************************
 * FUNCTION NAME : usb_rndis_alloc_rx_buffer_cb
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/

static void * usb_rndis_alloc_rx_buffer_cb (UINT32 size,USB_PIPE * ep,void * buf_info)
{
    /* For now we ignore size, always return same size buffer */
    if(size > 1564)  /* using same value as SYS_RNDIS_END_NUM_CLUSTER_SIZE in usbRndisEnd.c */
    {
        usblogMsg(HAL_SYS_FATAL,"Cannot allocate buffer greater than 1564 bytes\n",0,0,0,0,0);
        return NULL;
    }

    return sysRndisAllocateBuffer(usb20_rndis_mcb->hnd_rndis_mcb->h_end, buf_info);
}


/**************************************************************************
 * FUNCTION NAME : usb_rndis_free_rx_cb
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
static void usb_rndis_free_rx_buffer_cb (void * mem_ptr, void * buf_info)
{
    sysRndisDeallocateBuffer(usb20_rndis_mcb->hnd_rndis_mcb->h_end,mem_ptr,(UINT32)buf_info);
    return;
}

/**************************************************************************
 * FUNCTION NAME : usb_rndis_reset_handler
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
static void usb_rndis_reset_handler(void)
{
    /* What needs to be done here ? */
    
    /* Should the endpoints be closed? */ 
    
    return;
}

/**************************************************************************
 * FUNCTION NAME : usb20_rndis_get_pkt
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
static USB20_RNDIS_PKT *  usb20_rndis_get_pkt(void)
{
   HAL_SYS_CRIT_CNT    context;
   USB20_RNDIS_PKT * new_pkt;
   

   sys_critical_start ((HAL_SYS_CRIT_CNT *)&context);

    /* Remove packet structure from the free list */
   new_pkt = (USB20_RNDIS_PKT *)usb_list_remove ( (USB_LIST_NODE **)&usb20_rndis_mcb->ep_tx_free_list);
   
   sys_critical_end((HAL_SYS_CRIT_CNT *)&context);

   return new_pkt; /* Caller of this function is expected to check for errors (NULL value) */

}


/**************************************************************************
 * FUNCTION NAME : usb20_rndis_free_pkt
 **************************************************************************
 * DESCRIPTION   :
 *  
 * RETURNS       :
  ***************************************************************************/
static int usb20_rndis_free_pkt(USB20_RNDIS_PKT * pkt)
{
   HAL_SYS_CRIT_CNT    context;

   sys_critical_start ((HAL_SYS_CRIT_CNT *)&context);

    /* Do we need to clean up the packet structure ??? */
    
    /* Add the packet structure to the  free list */
   usb_list_add ((USB_LIST_NODE **)&usb20_rndis_mcb->ep_tx_free_list, (USB_LIST_NODE *)pkt);

   sys_critical_end((HAL_SYS_CRIT_CNT *)&context);

   return 1;
    
}




/****************APIs from OS Support Layer ***************************/

/**************************************************************************
 * FUNCTION NAME : sys_critical_start
 **************************************************************************
 * DESCRIPTION   :
 *  Starts the CRITICAL section.
 ***************************************************************************/
int sys_critical_start (void* p_context)
{
    SAL_int_off((UINT32 *) p_context);
    return 1;
}

/**************************************************************************
 * FUNCTION NAME : sys_critical_end
 **************************************************************************
 * DESCRIPTION   :
 *  Ends the CRITICAL section.
 ***************************************************************************/
int sys_critical_end (void* p_context)
{
    int key = *(int *)p_context;
    SAL_int_on((UINT32 *) &key);
    return 1;
}
 



/**************************************************************************
 * FUNCTION NAME : hal_usb_get_phy_medium
 **************************************************************************
 * DESCRIPTION   :
 *  The RNDIS uses this to get the PHYSICAL medium.
 *  
 * RETURNS       :
 *  The BU should return the value as specified in the RNDIS_PHYSICAL_MEDIUM
 *  enumeration as specified in rndis.h
 *      Cable  - RNdisPhysicalMediumCableModem
 *      DSL    - RNdisPhysicalMediumDSL
 ***************************************************************************/
UINT32 hal_usb_get_phy_medium (void)
{
    return RNdisPhysicalMediumDSL;
}

/**************************************************************************
 * FUNCTION NAME : hal_usb_pform_config_parameter
 **************************************************************************
 * DESCRIPTION   :
 *  The RNDIS calls this function to pass additional CONFIG parameters to the
 *  platform specific code. RNDIS by itself just handles the request for
 *  the 'NetworkAddress'. All other requests need to be handled here. 
 *  See the RNDIS Specification 3.4.3 for more details.
 *  
 * RETURNS       :
 *  0           -   The HOST gets a RNDIS_FAILURE message.
 *  1           -   The HOST gets a RNDIS_SUCCESS message. 
 ***************************************************************************/
int hal_usb_pform_config_parameter (void *ptr_config)
{
    RNDIS_OID_GEN_CONFIG_PARAM * ptr;

    ptr = (RNDIS_OID_GEN_CONFIG_PARAM *) ptr_config;
    /* Currently no other parameter is SUPPORTED ! */
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : hal_usb_pform_connection_status
 **************************************************************************
 * DESCRIPTION   :
 *  Called from the USB HAL to determine the USB status.    
 ****************************************************************************/
UINT32 hal_usb_pform_connection_status (void)
{

    return 1;
}



/*****************************************************************************/

UINT16 hal_usb_get_PID(RNDIS_MCB *ptr_rndis_mcb)
{
    UINT16 pid;

    usb_drv_ioctl((USB_DEV *)ptr_rndis_mcb->h_device, IOCTL_GET_PRODUCT_ID, (void *) &pid);
    return pid;
}


UINT16 hal_usb_get_VID(RNDIS_MCB *ptr_rndis_mcb)
{

    UINT16 vid;

    usb_drv_ioctl((USB_DEV *)ptr_rndis_mcb->h_device, IOCTL_GET_VENDOR_ID, (void *) &vid);
    return vid;
}


void hal_usb_get_manufacturer_string(char * str)
{
		char * temp_ptr = NULL;
		usb_drv_ioctl((USB_DEV *)usb20_rndis_mcb->hnd_device, IOCTL_GET_MANUFACTURER_STRING, (void *) &temp_ptr);

		if(temp_ptr!=NULL)
				memcpy(str,temp_ptr,strlen(temp_ptr)+1);
   
#if 0	
        
    {
    memcpy(str,"Texas Instruments",strlen("Texas Instruments")+1);
    }
#endif
    return;
}

void hal_usb_get_product_description(char * str)
{
		char * temp_ptr = NULL;
		usb_drv_ioctl((USB_DEV *)usb20_rndis_mcb->hnd_device, IOCTL_GET_PRODUCT_STRING, (void *) &temp_ptr);

		if(temp_ptr!=NULL)
				memcpy(str,temp_ptr,strlen(temp_ptr)+1);
   
#if 0	
    {
    memcpy (str,"TI USB RNDIS Driver",strlen("TI USB RNDIS Driver") +1);
    }
#endif
    return;
}

int hal_usb_get_serial_string(RNDIS_MCB *ptr_rndis_mcb, char * str)
{
    char *serial_str;
	
    usb_drv_ioctl((USB_DEV *)ptr_rndis_mcb->h_device, IOCTL_GET_SERIAL_NO, (void *) &serial_str);

    if ( serial_str == NULL ) 
	   return 0; 
    else
    	{
    	    memcpy(str,serial_str,strlen(serial_str)+1);
	    return 1;
    	}

}



