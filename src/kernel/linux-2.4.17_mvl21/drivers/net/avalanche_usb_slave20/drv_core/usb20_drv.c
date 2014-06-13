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
 * FILE NAME    :   usb20_drv.c
 *
 * DESCRIPTION  :   Core USB Protocol Driver 
 *
 * HISTORY 	:
 *
 * Aug 10   -   Sabyasachi   -  Created.
 * Sep 05   -   Sabyasachi   -  Completed control command processor
 * Sep 11   -   Sabyasachi   -  Updated Tx logic 
 * Oct 04    -   Sabyasachi   -  Modified control_read()/write() to migrate to HAL 2.0
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/


#include "usb20_drv.h"
#include "usb_sal.h"




/* To Acknowledge Control Transfer */
#define IN_ACK(base) 	*((volatile unsigned int *)((base)+0x224)) &= ~0xFF

/* extern declarations */
#ifdef USB_ENABLE_TASK
extern SAL_EVENT g_event_int;
extern UINT32 global_int_value;
#endif
/* forward declarations */

INT32  usb_drv_start_control_tx(USB_DEV *);
INLINE static void usb_drv_cmd_handler (USB_DEV *, USB_SETUP *);
INLINE static INT32 usb_drv_process_standard_cmd (USB_DEV *);

static USB_DEV  *p_usb_dev_mcb = NULL;

#if 0
int dump_packet(unsigned char *p, int len)
{
     while ( len -- ) 
	 	printf("%02x ",*p++);
     printf("\nEnd of Buffer Dump\n");

     return 0;
}

#endif

/**
 * @ingroup Protocol_Driver 
 * 
 *    Call this routine to obtain handle to the usb driver during runtime. 
 * 
 *    @return  Returns a pointer to the open driver control block.
 */
USB_DEV *usb_drv_get_dev()
{
    return p_usb_dev_mcb;
}

USB_STATUS usb_drv_set_dev(USB_DEV *ptr)
{
    p_usb_dev_mcb = ptr;
    return USB_STATUS_OK;
}


/*
 * This routine is the ISR registered with system interrupt controller.
 * This routine will just call hal ISR and will not do any other processing.
 */


void usb_drv_isr(UINT32 arg1, UINT32 arg2, UINT32 arg3)
{
    USB_DEV  *p_dev;
    HAL_ISR_T hal_isr;	
    INT32      ignore;

    p_dev =  (USB_DEV *)arg1;
    hal_isr  =  (HAL_ISR_T)arg2;
	
    hal_isr(p_dev->hal_dev, &ignore);
}

INT32 usb_drv_link_up(USB_DEV *p_dev)
{

    return 0;
}

INT32 usb_drv_link_down(USB_DEV *p_dev)
{
	
    p_dev->link_status = USB_LINK_DOWN;
    /*p_dev->class_dev->funcs->usb_class_dev_event(USB_EVENT_LINK_DOWN, NULL);*/

    return 0;
}

INT32 usb_drv_reset_handler(USB_DEV *p_dev)
{
    USB_ENTER;

    if ( p_dev ) 
             p_dev->ccpu->state = CCPU_SETUP_REQUEST;
    else
		USB_LOG(USB20_ISR,"#### NULLL Pointer ####\n",0,0,0,0);	

    p_dev->class_dev->funcs->usb_class_dev_event(USB_EVENT_RESET, NULL);
    
    USB_EXIT;

    return 0;
}


/*
 * Routine to read/write data from control End Point
 */
INT32 usb_drv_control_read(USB_DEV *p_dev)
{
  INT32  status;

  if ( p_dev->ccpu->xy_buf_sel[DIRECTION_RX] == SEL_BUFFER_X ) 
  {
      status = p_dev->hal_funcs->Ext->CtrlEPReadX(p_dev->hal_dev,&(p_dev->ccpu->ep_buf->len),(UINT32 *)p_dev->ccpu->ep_buf->data);
  }
  else
  {
      status = p_dev->hal_funcs->Ext->CtrlEPReadY(p_dev->hal_dev,&(p_dev->ccpu->ep_buf->len),(UINT32 *)p_dev->ccpu->ep_buf->data);
  }

  if ( status )
  {
      /* Read failed */
      USB_LOG((USB20_FATAL|USB20_ISR),"Trying read ... control OUT EP not ready\n",0,0,0,0);
      return USB_STATUS_ERROR;
  }
  else
  {
      /* Read success, toggle buffer for next read) */
      if ( p_dev->params.en_double_buf == TRUE )
          p_dev->ccpu->xy_buf_sel[DIRECTION_RX] ^= 0x1; /* Toggle buffer */
      return USB_STATUS_OK;
  }
}


INT32 usb_drv_control_write(USB_DEV *p_dev)
{
    INT32  status;

    if ( p_dev->ccpu->xy_buf_sel[DIRECTION_TX] == SEL_BUFFER_X )
    {
        status = p_dev->hal_funcs->Ext->CtrlEPWriteX(p_dev->hal_dev, p_dev->ccpu->ep_buf->len, (UINT32 *)p_dev->ccpu->ep_buf->data);
    }
    else
    {
        status = p_dev->hal_funcs->Ext->CtrlEPWriteY(p_dev->hal_dev, p_dev->ccpu->ep_buf->len, (UINT32 *)p_dev->ccpu->ep_buf->data);
    }
	
    if (status)
    {
        USB_LOG(USB20_ISR,"Trying write ... control IN EP not ready\n",0,0,0,0);
        return USB_STATUS_ERROR;
    }
    else
    {
        if ( p_dev->params.en_double_buf == TRUE ) 
            p_dev->ccpu->xy_buf_sel[DIRECTION_TX] ^= 0x1;  /* Toggle buffer */
        return USB_STATUS_OK;
    }
}




INT32  usb_drv_flush_control_tx_queue(USB_DEV *p_dev)
{
    USB_CTL_TX_PKT   *p_pkt = (USB_CTL_TX_PKT *)&(p_dev->current_ctl_tx_pkt);
    
    USB_ENTER;
    SAL_memset((void *)p_pkt,0,sizeof(USB_CTL_TX_PKT));

    USB_EXIT;

    return 0;
}


/**
 *  usb_drv_alloc_hal_channel
 *
 *  Will allocate one free HAL channel on request.
 */

CHANNEL_INFO  *usb_drv_alloc_hal_channel(USB_PIPE *pipe, int num)
{
    CHANNEL_INFO      *p_ch = NULL;
    CHANNEL_INFO_EXT  *p_ch_ext = NULL;
    
    USB_ENTER;

    if ( (p_ch = (CHANNEL_INFO *)SAL_calloc(1,sizeof(CHANNEL_INFO)))) 
    {
        /* Setup the Channel Info fields */
        p_ch->Channel       =  num;
        if ( USB_EP_DIR_INOUT == pipe->dir ) 
            p_ch->Direction     =  DIRECTION_BOTH;
        else if (USB_EP_DIR_IN == pipe->dir )
            p_ch->Direction     =  DIRECTION_TX;
        else if (USB_EP_DIR_OUT == pipe->dir ) 
            p_ch->Direction     =  DIRECTION_RX;
        else
        {
            SAL_free (p_ch);
            p_ch = NULL;
            goto end_alloc_channel;
        }

        p_ch->Receive       =  usb_drv_hal_receive;

        /* Setup extended fields */
        if ( (p_ch_ext = (CHANNEL_INFO_EXT *)SAL_calloc(1,sizeof(CHANNEL_INFO_EXT))))
        {
            /* Setup Ext fields */
            if ( DIRECTION_TX == p_ch->Direction || DIRECTION_BOTH == p_ch->Direction ) 
            {
                if ( pipe->type == USB_EP_T_BULK ) 
                    p_ch_ext->INEpBuffSize  =  USB_HS_BULK_EP_SIZE; 
                else 
                    p_ch_ext->INEpBuffSize  =  8; 
                p_ch_ext->INEpDbuf      =  1;
            }
            if ( DIRECTION_RX == p_ch->Direction || DIRECTION_BOTH == p_ch->Direction ) 
            {
                if ( pipe->type == USB_EP_T_BULK ) 
                    p_ch_ext->OUTEpBuffSize  =  USB_HS_BULK_EP_SIZE; 
                else 
                    p_ch_ext->OUTEpBuffSize  =  8; 
                p_ch_ext->OUTEpDbuf     =  1;
            }
        
        }
        else
        {
            SAL_free(p_ch);
            p_ch = NULL;
        }

        p_ch->Ext  =  p_ch_ext; 
    }
    
    USB_EXIT;

end_alloc_channel : 
    return p_ch;
}

 

INT32 usb_drv_channel_setup_all(USB_DEV *usb_dev)
{
        USB_EP  this_ep;
	  UINT32  ep, result;
	
        /*USB_LOG(USB20_CONTROL|USB20_ISR,"All Channels are being configured\n",0,0,0,0);*/
        for ( ep = 0; ep < USB20_NUM_EP ; ep++)
            {
	        this_ep = usb_dev->ep_table[ep];
                if ( this_ep.state == USB20_EP_STATE_READY )
                {
		    /* Setup this channel */
                    result = usb_dev->hal_funcs->ChannelSetup (
		                        usb_dev->hal_dev, 
					this_ep.hal_ch, 
					(OS_SETUP *)this_ep.pipe);
                    if ( EC_NO_ERRORS == result ) 
                    {
		           this_ep.state = USB20_EP_STATE_OPEN;
                        /*USB_LOG(USB20_CONTROL|USB20_ISR,"Channel %d is configured.\n", (this_ep.hal_ch)->Channel,0,0,0);*/
                    }
                }
	    }
	    return USB_STATUS_OK;

}


/**
 *  usb_drv_control_send
 *
 *  This routine will enqueue a packet, ready for transmission over 
 *  control end point, to the EP0 TX list. 
 *
 *  Assumption :  All Control Data is in a single buffer. Although 
 *  USB_PKT is used in this API to maintain consistency across EPs
 *  but control data MUST be sent in a single buffer only. No 
 *  fragmentation.
 */
INT32  usb_drv_control_send
(
                   USB_DEV *p_dev, 
		   USB_PKT *pkt, 
		   USB_SENDINFO *sendinfo
)
{
    INT32  status = USB_STATUS_OK;
    USB_CTL_TX_PKT   *p_ctl_pkt = (USB_CTL_TX_PKT   *)&(p_dev->current_ctl_tx_pkt);


    USB_ENTER;
    /* 
     * ASSUMPTION : All Control data MUST be in a single buffer to avoid
     * Copy here.
     */
    if ( pkt->num_buf > 1 ) 
    {
        status = USB_STATUS_INV_ARG;
	goto end_send;
    }
  
    p_ctl_pkt->sendinfo     = sendinfo;
    p_ctl_pkt->pkt          = pkt;
    p_ctl_pkt->data         = pkt->buf_list[0].data;
    p_ctl_pkt->len          = pkt->buf_list[0].len;

    
    /*
     * Kick start the transmission. This is required since there is no 
     * IN interrupt from Silicon. Therefore this needs to be kick started
     * as soon as data for transmission is available. 
     */
    usb_drv_start_control_tx(p_dev);

    USB_EXIT;

end_send : 
    return status;
}


INT32  usb_drv_start_control_tx(USB_DEV *p_dev)
{
    INT32   status = USB_STATUS_OK;
    USB_CTL_TX_PKT   *p_ctl_pkt = (USB_CTL_TX_PKT   *)&(p_dev->current_ctl_tx_pkt);


    USB_ENTER;

    /* Check if we have data to be transmitted in this packet */
   if( p_ctl_pkt->len != 0 )
    {
        /* Data is there to transmit */
	  p_dev->ccpu->ep_buf->len = p_ctl_pkt->len > p_dev->ep_control.max_size ? \
	 p_dev->ep_control.max_size  : p_ctl_pkt->len ;
        p_dev->ccpu->ep_buf->data = (UINT8 *)p_ctl_pkt->data;

	while (( status = usb_drv_control_write(p_dev)) != USB_STATUS_OK) ;
	if ( status == USB_STATUS_OK ) 
	{
	   USB_LOG(USB20_CONTROL,"Sent %d bytes\n",p_dev->ccpu->ep_buf->len,0,0,0);
	   p_ctl_pkt->len -= p_dev->ccpu->ep_buf->len;  /* Decrement count */
	   p_ctl_pkt->data += p_dev->ccpu->ep_buf->len;
	}

	/* Check if this packet if fully transmitted */
	if ( p_ctl_pkt->len == 0 ) 
	{
	    /* No more data in this packet, call sendcomplete */
	    if ( p_ctl_pkt->sendinfo->owner == USB_CLASS_DRIVER ) 
            {
		p_dev->ep_control.pipe->send_complete \
			(p_dev->ep_control.pipe, p_ctl_pkt->sendinfo->os_sendinfo);
	    }
            else
	    {
	        /* As of now nothing needs to be done for Protocol Driver */
	    }
	 
   
	}
    }

       USB_EXIT;
    
    return status;
}


/**
 *  usb_drv_control_receive
 *
 *  This routine is called when data is received on Control End Point.
 */
INT32 usb_drv_control_receive(USB_DEV *p_dev)
{
    INT32  status = USB_STATUS_OK;
    USB_SETUP  *p_setup;
    
    USB_ENTER;

    p_setup = (USB_SETUP *)&(p_dev->ccpu->work_buf[0]);

    if (p_dev->ccpu->state != CCPU_DATA_OUT ) 
    {
        status = USB_STATUS_INV_STATE;
	goto end_receive;
    }
    else
    {

	/* Read all data in single shot since we are not getting OUT int */
	p_dev->ccpu->ep_buf->data = (UINT8 *)&(p_dev->ccpu->work_buf[p_dev->ccpu->work_buf_len]);

        if ( usb_drv_control_read(p_dev) != 0 )
	{
	    USB_LOG(USB20_FATAL,"Control Read failed at %d in %s\n",(UINT32) __LINE__, (UINT32) __FILE__,0,0);
	    return -1;
	}
	
	p_dev->ccpu->work_buf_len += p_dev->ccpu->ep_buf->len;

	if ( (p_dev->ccpu->work_buf_len - sizeof(USB_SETUP)) >= USB_SWAPW(p_setup->wLength) ) 
	{
		 IN_ACK(p_dev->params.usb_base);
		 USB_LOG(USB20_CONTROL,"Classs Driver SETUP OUT OUT ... ACK\n",0,0,0,0);
	        usb_drv_cmd_handler(p_dev,p_setup);
	}
    }
    
    USB_EXIT;

end_receive : 
    return status;
}


/***************************************************************************
 *   
 *    USB Standard Command Processor. 
 *
 *    Routines implemented below implements a standard USB 2.0 Protocol 
 *    command processor. Each Control Transfer consists of the three 
 *    following stages : 
 *
 *        -  SETUP  
 *        -  DATA   (optional)
 *        -  STATUS 
 *
 *    DATA stage is optional. Should there be a data stage, amount of data,
 *    direction of transfer etc. are specified during SETUP stage.
 *
 ***************************************************************************/

INT32 usb_drv_ccpu_open(USB_DEV *p_dev)
{
    INT32  status = USB_STATUS_OK;

    USB_ENTER;

    p_dev->ccpu = (USB_CCPU *)SAL_malloc(sizeof(USB_CCPU));

    if ( p_dev->ccpu == NULL ) 
	status = USB_STATUS_ERROR;
    else
    {
        SAL_memset((void *)p_dev->ccpu, 0, sizeof(USB_CCPU));
        p_dev->ccpu->state                    =  CCPU_SETUP_REQUEST;
	 p_dev->ccpu->work_buf_len             =  0;
	 p_dev->ccpu->xy_buf_sel[DIRECTION_TX] = SEL_BUFFER_X;
        p_dev->ccpu->xy_buf_sel[DIRECTION_RX] = SEL_BUFFER_X;
	 p_dev->ccpu->ep_buf = (EP0_BUFFER *)SAL_malloc(sizeof(EP0_BUFFER));
    }
    
    USB_EXIT;

    return status;
}


/**
 *  usb_drv_command_handler :
 *
 *  This routine will be called when SETUP packet has been received.
 *  This routine will determine what command the SETUP packet carries
 *  and take necessary action.
 */
INLINE static void usb_drv_cmd_handler (USB_DEV *p_dev, USB_SETUP *p_setup)
{
    USB_PKT  pkt;
    USB_BUF  buf;
    int key;
    
    USB_ENTER;

    /* Start Critical Section */
  SAL_int_off((UINT32 *) &key);

    /* Process the SETUP Request on the basis of its type. */
    switch ((p_setup->bmRequestType & USB_SETUP_TYPE_MASK))
    {
        case USB_SETUP_REQ_STANDARD:
        {
            /* Received a standard request. This can be processed in the CCU. */
            if (usb_drv_process_standard_cmd (p_dev) == -1)
            {
                /* 
		 * The CCPU was unable to process the command. This could be 
		 * a status of the HOST sending an unsupported command or a 
                 * command with invalid parameters. 
		 */
            }
            break;
        }
        case USB_SETUP_REQ_CLASS:
        case USB_SETUP_REQ_VENDOR:
        {
            /* 
	     * Class and vendor specific commands are not part of the 
             * standard enumeration which the CCPU can handle. Hence pass 
             * all such messages to Class Driver.
	     */
            if (p_dev->ep_control.pipe->receive != NULL)
            {
		pkt.buf_list  = &buf;
		pkt.num_buf   = 1;
		pkt.pkt_size  = p_dev->ccpu->work_buf_len;
		buf.data      = (UINT8 *)&(p_dev->ccpu->work_buf[0]);
		buf.len       = pkt.pkt_size;
                p_dev->ep_control.pipe->receive (p_dev->ep_control.pipe, (USB_PKT *)&pkt);
            }
            else
            {
                USB_LOG(DEBUG_USB20, "Error: Class Driver has NOT registered a callback routine.",0,0,0,0);
            }
            break;
        }
        default:
        {
            /* Any other request from the HOST violates the USB Spec.  */
            USB_LOG(DEBUG_USB20, "Error: Unrecognized request type %d Received from USB HOST. \n", (p_setup->bmRequestType & USB_SETUP_TYPE_MASK),0,0,0);
        }
    }

    /* 
     * Once the command has been fully processed. Reset the buffer and its 
     * contents. Also the CCU state machine will be waiting for more SETUP 
     * packets. 
     */
    p_dev->ccpu->state = CCPU_SETUP_REQUEST;
    SAL_memset ((void *)&p_dev->ccpu->work_buf[0], 0, CCPU_CONTROL_DATA_MAX_LEN);

    /* End of Critical Section */
     SAL_int_on((UINT32 *) &key);
 
    USB_EXIT;
    return;
}



INLINE static INT32  usb_drv_setup_handler(USB_DEV *p_dev)
{
    USB_SETUP  *p_setup;
    int        key;
    
    USB_ENTER;

    HAL_CONTROL(p_dev, hkUsbDirCtrl, haSETbyVAL, HC_USB_DATADIR_OUT);

    /* Read the SETUP packet from HAL */
    p_dev->ccpu->ep_buf->data = (UINT8 *)&(p_dev->ccpu->work_buf[0]);
    if ( usb_drv_control_read(p_dev) )
    {
	    USB_LOG((USB20_FATAL|USB20_CONTROL),"usb_drv_control_read failed\n",0,0,0,0);
	    return USB_STATUS_ERROR;
    }
    if ( p_dev->ccpu->ep_buf->len != sizeof(USB_SETUP))
    {
           USB_LOG((USB20_FATAL|USB20_CONTROL),"Received wrong SETUP packet. Length : %d\n",p_dev->ccpu->ep_buf->len,0,0,0);
	    return USB_STATUS_ERROR;
    }

    /* 
     * We received a valid SETUP packet 
     */
    p_setup = (USB_SETUP *)(p_dev->ccpu->ep_buf->data);
    p_dev->ccpu->work_buf_len = sizeof(USB_SETUP);

    USB_LOG(USB20_CONTROL,"0x%08x 0x%08x\n",(unsigned int)*((UINT32 *)p_setup),(unsigned int)*(((UINT32 *)p_setup)+1),0,0);

    /* 
     * Flush the Control Tx Queue as we are starting a fresh 
     * control transaction 
     */
    usb_drv_flush_control_tx_queue(p_dev);

    /* Critical Section starts */
  SAL_int_off((UINT32 *) &key);

    /* 
     * Determine the direction of the SETUP packet. 
     * 
     * DEVICE_TO_HOST : 
     * 
     * Case (1)
     * 
     *    SETUP IN IN IN IN ... IN OUT (Handshake)
     *
     * HOST_TO_DEVICE : 
     *
     * Case (2) 
     *
     *    SETUP OUT OUT OUT ... OUT IN (Handshake - driver has to do) 
     *
     * Case (3) 
     *
     *    SETUP IN (Handshake - driver has to do) 
     */


    if ( (p_setup->bmRequestType & USB_SETUP_DIRECTION_MASK) == USB_SETUP_DEVICE_TO_HOST)
    {
        /* 
	 * The data stage is from the Device to HOST. The HOST has sent 
         * us all the data the device needs to parse the packet and send the 
         * response. Let us process the command. 
	 */
	/* This is case (1) */
        p_dev->ccpu->state = CCPU_PROCESS_COMMAND;
	HAL_CONTROL(p_dev, hkUsbDirCtrl, haSETbyVAL, HC_USB_DATADIR_IN);
    }
    else
    {
        /* 
	 * The data stage is from the HOST to Device. The device will be 
         * receiving more data from the HOST. Hence processing of this 
	 * packet may not be completed till all the data has been received. 
	 *
	 * First check if data stage exists.
	 */
	if ( USB_SWAPW(p_setup->wLength) == 0 )
	{
	    /* No DATA stage */
	    /* ACK IN End Point */
	    /* This is case (3) */
	    p_dev->ccpu->state = CCPU_PROCESS_COMMAND;
	}
	else
	{
            /* This is case (2) */
            p_dev->ccpu->state        = CCPU_DATA_OUT;
	}
    }

    if ( p_dev->ccpu->state == CCPU_PROCESS_COMMAND ) 
	usb_drv_cmd_handler (p_dev, p_setup);
    
    USB_EXIT;
    return 0;
}


INLINE static INT32  usb_drv_cmd_get_status(USB_DEV *p_dev)
{
    USB_SETUP  *p_setup;
    UINT16     index;
    INT32      status = USB_STATUS_OK;
    USB_PKT    *pkt;
    INT32      ep_to_stall;
    USB_SENDINFO  *sendinfo;
    UINT8      response[2];  /* This is a potential BUG, data should be in heap */
    
    USB_ENTER;

    p_setup = (USB_SETUP *)&(p_dev->ccpu->work_buf[0]);
    /* Validate the arguments in the SETUP packet. */
    if ( (p_setup->wValue != 0) || (p_setup->wLength != (USB_SWAPW(2))) )
    {
        /* The HOST violated the USB Specification 1.1. */
        USB_LOG(DEBUG_USB20, "ERROR: GET status violation by HOST.\n",0,0,0,0);
        status = USB_STATUS_INV_COMMAND;
	goto end_get_status;
    }
    
    SAL_memset((void *)&response[0], 0, sizeof(response));
    pkt = (USB_PKT *)&(p_dev->ccpu->pkt);
    pkt->buf_list = (USB_BUF *)&(p_dev->ccpu->pkt_buf);
    sendinfo = (USB_SENDINFO *)&(p_dev->ccpu->sendinfo);

    switch (p_setup->bmRequestType & USB_SETUP_RECEPIENT_MASK) 
    {
	case USB_SETUP_RECP_DEVICE :
	    /*
	     * Right now USB 2.0 is self-powered and remote wakeup 
	     * is not supported. Modify later.
	     */
	    response[0] = USB_CONFIG_ATTRIB_SELF_PWR >> 0x6; 
	    pkt->buf_list[0].data = (UINT8 *)&response[0];
	    pkt->buf_list[0].len  = 2;
	    sendinfo->ep          = (USB_EP *)&p_dev->ep_control;
            /* sendinfo->os_sendinfo = ; Need Anything - Saby */
            sendinfo->owner       = USB_PROTOCOL_DRIVER;
            status = usb_drv_control_send(p_dev, pkt, sendinfo);
	    break;

	case USB_SETUP_RECP_ENDPOINT :
            index = USB_SWAPW(p_setup->wIndex);
            ep_to_stall = index & USB_EP_ADDR_MASK;
	    ep_to_stall = ep_to_stall > 2 ? 0 : ep_to_stall;
	    /*if ( p_dev->channel[ep_to_stall-1].status != USB20_CHANNEL_UP )
	        ep_to_stall = 0; */
            USB_LOG((USB20_CONTROL|USB20_ISR), "Get Status : End Point Halt\n",0,0,0,0);
            if ((index & USB_EP_IN_DIR) == USB_EP_IN_DIR)
            {
		if ( 0 == ep_to_stall )
		{
		    if ( p_dev->ep_control.status & USB20_EP_IN_HALT ) 
	                response[0] = 1;
		}
		else
		{    
		    if ( p_dev->ep_table[ep_to_stall-1].status & USB20_EP_IN_HALT ) 
	                response[0] = 1;
		}
	    }
	    else
	    {
		if ( 0 == ep_to_stall )
		{
		    if ( p_dev->ep_control.status & USB20_EP_OUT_HALT ) 
	                response[0] = 1;
		}
		else
		{    
		    if ( p_dev->ep_table[ep_to_stall-1].status & USB20_EP_OUT_HALT ) 
	                response[0] = 1;
		}
	    }

	    pkt->buf_list[0].data = (UINT8 *)&response[0];
	    pkt->buf_list[0].len  = 2;
	    sendinfo->ep          = (USB_EP *)&p_dev->ep_control;
            /* sendinfo->os_sendinfo = ; Need Anything - Saby */
            sendinfo->owner       = USB_PROTOCOL_DRIVER;
            status = usb_drv_control_send(p_dev, pkt, sendinfo);
	    break;

	case USB_SETUP_RECP_INTERFACE :
	    /* Send all 0 for Interface status, Section 9.4.5 */
	    pkt->buf_list[0].data = (UINT8 *)&response[0];
	    pkt->buf_list[0].len  = 2;
	    sendinfo->ep          = (USB_EP *)&p_dev->ep_control;
            /* sendinfo->os_sendinfo = ; Need Anything - Saby */
            sendinfo->owner       = USB_PROTOCOL_DRIVER;
            status = usb_drv_control_send(p_dev, pkt, sendinfo);
	    break;

	default : 
	    USB_LOG(DEBUG_USB20,"Unknown recipient for GET_STATUS\n",0,0,0,0);
	    status = USB_STATUS_INV_ARG;
    }
    
end_get_status :
    USB_EXIT;
    return status;
}


/* 
 *  Standard test modes which need to be applied to the USB20 hardware
 *  in Test mode. See USB 2.0 Silicon Reference and USB 2.0 Protocol Specification 
 *  for more details.
 */
static UINT8 HSTestModeValues[] =
{
    0,       /* Invalid command */
    0x2,   /* Test J state */
    0x3,   /* Test K state */
    0x1,   /* Test SEO NAK */
    0x4,   /* Test Packet */
    0x5    /* Force full speed */
};


/* 
 *  Standard Test pattern which is transmitted in response to TEST_PACKET command.
 *  This has been created from Electrical Test section of USB 2.0 Protocol Specification.
 *  
 *  Note : Do not touch it or edit it.
 */
static UINT32 usb_hs_test_packet_pattern[]=
{
  0x00000000,
  0x00000000,
  0x00555555,
  0x55555555,
  0x55777777,
  0x77777777,
  0x77feffff,
  0xffffffff,
  0xffffffff,
  0xffffe7df,
  0xbf7ffefd,
  0xf9fbe7df,
  0xbf7ffefd, 
  0xfb010101,  /* Test pattern ends here */
  0x00000000,
  0x00000000,
};


INLINE static INT32  usb_drv_cmd_set_feature(USB_DEV *p_dev)
{
    USB_SETUP  *p_setup;
    INT32      ep_to_stall;
    INT32      is_test_mode = 0;
    UINT16    index, test_selector;
    INT32      status = USB_STATUS_OK;
    
    USB_ENTER;

    p_setup = (USB_SETUP *)&(p_dev->ccpu->work_buf[0]);
    index    =  USB_SWAPW(p_setup->wIndex);
	
    /* Validate the arguments in the SETUP packet. */
    if (p_setup->wLength != 0x0)
    {
        /* The HOST violated the USB 2.0 Spec */
        USB_LOG((USB20_CONTROL|USB20_ISR), "ERROR: SET FEATURE violation by HOST.\n",0,0,0,0);
        status = USB_STATUS_INV_COMMAND;
    }

    /* Processing is based on the RECEPIENT type. */
    switch ( (p_setup->bmRequestType & USB_SETUP_RECEPIENT_MASK) )
    {
        case USB_SETUP_RECP_DEVICE:
            /* 
	     * The DEVICE is recepient. Validate the parameters passed. 
             * The FEATURE selector has to be REMOTE WAKEUP as per the USB 
             * specification. 
	     */
	     switch ( USB_SWAPW(p_setup->wValue))
	     	{					
		     case USB_HIGH_SPEED_TEST_MODE : 
			 	   USB_LOG((USB20_CONTROL|USB20_ISR), "SET FEATURE : High Speed Test Mode.\n",0,0,0,0);
				   test_selector = index >> 8;
                               if ( test_selector < USB_HS_TEST_J_MODE || test_selector> USB_HS_TEST_FORCE_EN_MODE ) 
                               {
                                     ep_to_stall = 0;
				     p_dev->ep_control.status |= USB20_EP_IN_HALT;
               		             p_dev->ep_control.status |= USB20_EP_OUT_HALT;
                                     p_dev->hal_funcs->Control(p_dev->hal_dev, hkInStall, haSETbyVAL, (void *)ep_to_stall);
				     p_dev->hal_funcs->Control(p_dev->hal_dev, hkOutStall, haSETbyVAL, (void *)ep_to_stall);	
                               }
				   else
				   {                                      
				   	    /* Send a zero byte data */
                                       p_dev->ccpu->ep_buf->len = 0;
					    ep_to_stall = 0;   /* Used for dummy data */
                                       p_dev->ccpu->ep_buf->data = (UINT8 *)&ep_to_stall;
                                       usb_drv_control_write(p_dev);
                                       if (HSTestModeValues[test_selector] == USB_HS_TEST_PACKET_MODE)
   	                               {    
   	                                     is_test_mode = 1;
   	                                     USB_LOG((USB20_CONTROL|USB20_ISR), "SET FEATURE : High Speed Test Packet Mode.\n",0,0,0,0);
   	                                     HAL_CONTROL(p_dev, hkUsbDirCtrl, haSETbyVAL, HC_USB_DATADIR_IN);
					         p_dev->ccpu->ep_buf->len = 0x35;
                                            p_dev->ccpu->ep_buf->data = (char *)&usb_hs_test_packet_pattern[0];
						  *(volatile unsigned int *)0xa4020224 |= 0x8080;
                                            while ( usb_drv_control_write(p_dev) != USB_STATUS_OK ) ;
						  USB_LOG(USB20_CONTROL,"0x%08x\n",*(volatile unsigned int *)0xa4020224,0,0,0);
                                       }
					    else 
					    {

					    }
					 
                                        /* Apply Test mode into the hardware */                 
                                       *(volatile unsigned int *)0xa4020204 &= ~(0xe);
                                       *(volatile unsigned int *)0xa4020204 |= (HSTestModeValues[test_selector] << 1);
					    USB_LOG((USB20_CONTROL|USB20_ISR), "SET FEATURE : High Speed Test Mode : 0x%08x.\n",*(volatile unsigned int *)0xa4020204,0,0,0);
				   }
				   break;

	     	     case  USB_DEVICE_REMOTE_WAKEUP : 				 	
                               /* DEVICE Remote wakeup is NOT supported. */
                               USB_LOG((USB20_FATAL|USB20_ISR), "ERROR: SET FEATURE Remote Wakeup NOT supported.\n",0,0,0,0);
                               status = USB_STATUS_NO_SUPPORT;
		     default : 			 
                               USB_LOG(USB20_CONTROL,"End Points are halted\n",0,0,0,0);
				      ep_to_stall = 0;
				      p_dev->ep_control.status |= USB20_EP_IN_HALT;
               		      p_dev->ep_control.status |= USB20_EP_OUT_HALT;
                                  p_dev->hal_funcs->Control(p_dev->hal_dev, hkInStall, haSETbyVAL, (void *)ep_to_stall);
				      p_dev->hal_funcs->Control(p_dev->hal_dev, hkOutStall, haSETbyVAL, (void *)ep_to_stall);	
	     	}
	    break;
        
        case USB_SETUP_RECP_ENDPOINT:
            /* 
	     * Endpoint is recepient. Validate the parameters. 
	     * The FEATURE selector has to be ENDPOINT_HALT as 
	     * per the USB specification. 
	     */
            if (p_setup->wValue != (USB_SWAPW (USB_ENDPOINT_HALT)) )
            {
                /* The HOST violated the USB Specification 1.1. */
                 USB_LOG((USB20_CONTROL|USB20_ISR), "ERROR: SET Feature violation. Endpoint Feature Selector=%d.\n", p_setup->wValue,0,0,0);
		   ep_to_stall = 0;
		   p_dev->ep_control.status |= USB20_EP_IN_HALT;
                 p_dev->ep_control.status |= USB20_EP_OUT_HALT;
                 p_dev->hal_funcs->Control(p_dev->hal_dev, hkInStall, haSETbyVAL, (void *)ep_to_stall);
		   p_dev->hal_funcs->Control(p_dev->hal_dev, hkOutStall, haSETbyVAL, (void *)ep_to_stall);			
                status = USB_STATUS_INV_ARG;
            }
	    else
	    {
	     USB_LOG((USB20_CONTROL|USB20_ISR),"Set Feature : END Point Halt.\n",0,0,0,0);
           
            index = USB_SWAPW(p_setup->wIndex);
            /* Processing is based on the END point address. */
            ep_to_stall = index & USB_EP_ADDR_MASK;
            ep_to_stall = ep_to_stall > 2 ? 0 : ep_to_stall;
            if ((index & USB_EP_IN_DIR) == USB_EP_IN_DIR)
            {
		    USB_LOG((USB20_CONTROL|USB20_ISR),"End Points are halted\n",0,0,0,0);
                p_dev->hal_funcs->Control(p_dev->hal_dev, hkInStall, haSETbyVAL, (void *)ep_to_stall);
		   if ( ep_to_stall == 0 ) 
		        p_dev->ep_control.status |= USB20_EP_IN_HALT;
		   else
		        p_dev->ep_table[ep_to_stall-1].status |= USB20_EP_IN_HALT;
                USB_LOG((USB20_CONTROL|USB20_ISR), "DEBUG: Endpoint %d IN is Halted.\n",ep_to_stall, 0,0,0);
            }
            else
            {
                p_dev->hal_funcs->Control(p_dev->hal_dev, hkOutStall, haSETbyVAL, (void *)ep_to_stall);
		if ( ep_to_stall == 0 ) 
		    p_dev->ep_control.status |= USB20_EP_OUT_HALT;
		else
		    p_dev->ep_table[ep_to_stall-1].status |= USB20_EP_OUT_HALT;
                USB_LOG(USB20_CONTROL, "DEBUG: Endpoint %d OUT is Halted.\n",ep_to_stall,0,0,0);
            }
		}
            break;
        
        default:
        {
            /* The HOST violated the USB 2.0 Spec */
            USB_LOG(USB20_CONTROL, "ERROR: SET Feature violation by HOST. Recepient=%d.\n", (p_setup->bmRequestType & USB_SETUP_RECEPIENT_MASK),0,0,0 );
	      ep_to_stall = 0;
	      p_dev->ep_control.status |= USB20_EP_IN_HALT;
             p_dev->ep_control.status |= USB20_EP_OUT_HALT;
             p_dev->hal_funcs->Control(p_dev->hal_dev, hkInStall, haSETbyVAL, (void *)ep_to_stall);
	      p_dev->hal_funcs->Control(p_dev->hal_dev, hkOutStall, haSETbyVAL, (void *)ep_to_stall);	
            status = USB_STATUS_INV_ARG;
        }
    }

    if ( !is_test_mode )
    	{
		IN_ACK(p_dev->params.usb_base);
    	}
    else 
	usb20_logMsg("Test Packet Mode : IN_ACK is ignored. Not a case of SETUP OUT OUT .... OUT IN\n",0,0,0,0,0,0);
	
    USB_EXIT;
    return status;
}


INLINE static INT32  usb_drv_cmd_clear_feature(USB_DEV *p_dev)
{
    USB_SETUP  *p_setup;
    INT32   ep_to_stall;
    UINT16  index;
    INT32      status = USB_STATUS_OK;
    
    USB_ENTER;
    p_setup = (USB_SETUP *)&(p_dev->ccpu->work_buf[0]);

    /* Validate the arguments in the SETUP packet. */
    if (p_setup->wLength != 0x0)
    {
        /* The HOST violated the USB 2.0 Spec */
        USB_LOG(DEBUG_USB20, "ERROR: SET FEATURE violation by HOST.\n",0,0,0,0);
        status = USB_STATUS_INV_COMMAND;
    }	

    /* Processing is based on the RECEPIENT type. */
    switch ( (p_setup->bmRequestType & USB_SETUP_RECEPIENT_MASK) )
    {
        case USB_SETUP_RECP_DEVICE:
            /* 
	     * The DEVICE is recepient. Validate the parameters passed. 
             * The FEATURE selector has to be REMOTE WAKEUP as per the USB 
             * specification. 
	     */
            if (p_setup->wValue != (USB_SWAPW (USB_DEVICE_REMOTE_WAKEUP)) )
            {
                /* The HOST violated the USB 2.0 Spec */
                USB_LOG(DEBUG_USB20, "ERROR: SET Feature violation. DEVICE Feature Selector=%d.\n", p_setup->wValue,0,0,0);
                status = USB_STATUS_INV_ARG;
            }

            /* DEVICE Remote wakeup is NOT supported. */
            USB_LOG(DEBUG_USB20, "ERROR: SET FEATURE Remote Wakeup NOT supported.\n",0,0,0,0);
            status = USB_STATUS_NO_SUPPORT;
	    break;
        
        case USB_SETUP_RECP_ENDPOINT:
            /* 
	     * Endpoint is recepient. Validate the parameters. 
	     * The FEATURE selector has to be ENDPOINT_HALT as 
	     * per the USB specification. 
	     */
            if (p_setup->wValue != (USB_SWAPW (USB_ENDPOINT_HALT)) )
            {
                /* The HOST violated the USB Specification 1.1. */
                USB_LOG(DEBUG_USB20, "ERROR: SET Feature violation. Endpoint Feature Selector=%d.\n", p_setup->wValue,0,0,0);
                status = USB_STATUS_INV_ARG;
            }
    
            index = USB_SWAPW(p_setup->wIndex);

            /* Processing is based on the END point address. */
            ep_to_stall = index & USB_EP_ADDR_MASK;
	    ep_to_stall = ep_to_stall > 4 ? 0 : ep_to_stall;
	    /*
	    if ( p_dev->channel[ep_to_stall-1].status != USB20_CHANNEL_UP )
	        ep_to_stall = 0;
             */
            if ((index & USB_EP_IN_DIR) == USB_EP_IN_DIR)
            {
                p_dev->hal_funcs->Control(p_dev->hal_dev, hkInStall, haCLEAR, &ep_to_stall);
                USB_LOG(USB20_CONTROL,"EP %d IN Halt Cleared.\n",ep_to_stall,0,0,0);
		if ( ep_to_stall == 0 ) 
		    p_dev->ep_control.status &= ~USB20_EP_IN_HALT;
		else
		    p_dev->ep_table[ep_to_stall-1].status &= ~USB20_EP_IN_HALT;
                USB_LOG(USB20_CONTROL, "DEBUG: Endpoint %d IN is Halt Cleared.\n",ep_to_stall, 0,0,0);
            }
            else
            {
                p_dev->hal_funcs->Control(p_dev->hal_dev, hkOutStall, haCLEAR, &ep_to_stall);
                USB_LOG(USB20_CONTROL,"EP %d OUT Halt Cleared.\n",ep_to_stall,0,0,0);
		if ( ep_to_stall == 0 ) 
		    p_dev->ep_control.status &= ~USB20_EP_OUT_HALT;
		else
		    p_dev->ep_table[ep_to_stall-1].status &= ~USB20_EP_OUT_HALT;
                USB_LOG(USB20_CONTROL, "DEBUG: Endpoint %d OUT Halt Cleared.\n",ep_to_stall,0,0,0);
            }
            break;
        
        default:
        {
            /* The HOST violated the USB 2.0 Spec */
            USB_LOG(USB20_CONTROL, "ERROR: SET Feature violation by HOST. Recepient=%d.\n", (p_setup->bmRequestType & USB_SETUP_RECEPIENT_MASK),0,0,0 );
            status = USB_STATUS_INV_ARG;
        }
    }
    
    USB_EXIT;
    return 0;
}

INLINE static INT32  usb_drv_cmd_set_address(USB_DEV  *p_dev)
{
    USB_SETUP  *p_setup;
    UINT32     address; 
    
    USB_ENTER;
    p_setup = (USB_SETUP *)&(p_dev->ccpu->work_buf[0]);
    address = USB_SWAPW(p_setup->wValue);
    HAL_CONTROL(p_dev, hkUsbDevAddr, haSETbyVAL, address);
    p_dev->params.address = address;

    USB_EXIT;

    return USB_STATUS_OK;  
}


/*
  * Instance of Device Qualifier Descriptor 
  */

USB_DEVICE_QUALIFIER_DESCR  dev_qualifier_desc = 
{
    10,                             /* bLength */
    USB_DESCRIPTOR_DEVICE_QUALIFIER,    /* bDescriptorType  DEVICE */
    USB_SWAPW(0x0200),                 /* bcdUSB           USB 1.1 compliant  */
    0xFF,                           /* bDeviceClass     class is vendor specific */
    0x00,                           /* bDeviceSubClass  vendor specific */
    0x00,                           /* bDeviceProtocol  vendor specific */
    0x40,               /* bMaxPacketSize0 */
    0,                              /* bNumConfigurations */
    0
};


INLINE static INT32  usb_drv_cmd_get_descriptor(USB_DEV  *p_dev)
{
    INT32  status = USB_STATUS_OK;
    UINT8  descriptor_type;
    UINT8  descriptor_index;
    USB_SETUP     *p_setup = NULL;
    USB_PKT       *pkt = NULL;
    USB_SENDINFO  *sendinfo = NULL;
    
    USB_ENTER;
    
    p_setup = (USB_SETUP *)&(p_dev->ccpu->work_buf[0]);
    descriptor_type  = ((USB_SWAPW(p_setup->wValue)) & 0xFF00) >> 8;
    descriptor_index = ((USB_SWAPW(p_setup->wValue)) & 0x00FF);

    pkt = (USB_PKT *)&(p_dev->ccpu->pkt);
    pkt->buf_list = (USB_BUF *)&(p_dev->ccpu->pkt_buf);
    pkt->num_buf = 1;
    sendinfo = (USB_SENDINFO *)&(p_dev->ccpu->sendinfo);

    switch ( descriptor_type)
    {
        case USB_DESCRIPTOR_DEVICE : 
		pkt->buf_list[0].data = (UINT8 *)(p_dev->usb_cfg->device_desc);
		pkt->buf_list[0].len  = USB_MIN(sizeof(USB_DEVICE_DESCR), USB_SWAPW(p_setup->wLength));
	        sendinfo->ep          = (USB_EP *)&p_dev->ep_control;
                /* sendinfo->os_sendinfo = ; Need Anything - Saby */
                sendinfo->owner       = USB_PROTOCOL_DRIVER;
		status = usb_drv_control_send(p_dev, pkt, sendinfo);
		break;
		
	case USB_DESCRIPTOR_CONFIG :
		pkt->buf_list[0].data = (UINT8 *)(p_dev->usb_cfg->config);
		/*
		 * Send whatever number of data Host requested for 
		 */
		pkt->buf_list[0].len  = USB_MIN(p_dev->usb_cfg->config_desc_length,USB_SWAPW(p_setup->wLength));
	        sendinfo->ep          = (USB_EP *)&p_dev->ep_control;
                /* sendinfo->os_sendinfo = ; Need Anything - Saby */
                sendinfo->owner       = USB_PROTOCOL_DRIVER;
		status = usb_drv_control_send(p_dev, pkt, sendinfo);
		break;
		
	case USB_DESCRIPTOR_STRING : 
		pkt->buf_list[0].data = (UINT8 *)(p_dev->usb_cfg->string[descriptor_index]);
		/* First byte in string descriptor is length of descriptor */
		pkt->buf_list[0].len  = USB_MIN(USB_SWAPW(p_setup->wLength),pkt->buf_list[0].data[0]);
	        sendinfo->ep          = (USB_EP *)&p_dev->ep_control;
                /* sendinfo->os_sendinfo = ; Need Anything - Saby */
                sendinfo->owner       = USB_PROTOCOL_DRIVER;
		status = usb_drv_control_send(p_dev, pkt, sendinfo);
		break;
	
	case USB_DESCRIPTOR_DEVICE_QUALIFIER : 
		/*USB_LOG((USB20_CONTROL|USB20_ISR),"Device Qualifier Descriptor Not supported now\n",0,0,0,0);*/
		pkt->buf_list[0].data = (UINT8 *)&dev_qualifier_desc;
		pkt->buf_list[0].len  = USB_MIN(sizeof(USB_DEVICE_QUALIFIER_DESCR), USB_SWAPW(p_setup->wLength));
	        sendinfo->ep          = (USB_EP *)&p_dev->ep_control;
                /* sendinfo->os_sendinfo = ; Need Anything - Saby */
                sendinfo->owner       = USB_PROTOCOL_DRIVER;
		status = usb_drv_control_send(p_dev, pkt, sendinfo);
		/* status = USB_STATUS_NO_SUPPORT;*/
		break;
		
	case USB_DESCRIPTOR_OTHER_SPEED_CFG : 
		USB_LOG((USB20_CONTROL|USB20_ISR),"Other Speed Descriptor Not supported now\n",0,0,0,0);
		/* TODO: Need to support this. - Prashanth */
		status = USB_STATUS_NO_SUPPORT;
		break;			
		
	case USB_DESCRIPTOR_INTERFACE_POWER : 
		USB_LOG((USB20_CONTROL|USB20_ISR),"Interface Power Descriptor Not supported now\n",0,0,0,0);
		status = USB_STATUS_NO_SUPPORT;
		break;
		
	default : 
		status = USB_STATUS_INV_ARG;
		USB_LOG((USB20_CONTROL|USB20_ISR),"Wrong get descriptor type : %d\n",descriptor_type,0,0,0);
    }

    USB_EXIT;
    return status;
}


INLINE static INT32  usb_drv_cmd_set_descriptor(USB_DEV *p_dev)
{   
    INT32         status = USB_STATUS_OK;
    UINT8         descriptor_type;
    UINT8         descriptor_index;
    USB_SETUP     *p_setup = NULL;
    
    USB_ENTER;
    
    p_setup = (USB_SETUP *)&(p_dev->ccpu->work_buf[0]);
    descriptor_type  = ((USB_SWAPW(p_setup->wValue)) & 0xFF00) >> 8;
    descriptor_index = ((USB_SWAPW(p_setup->wValue)) & 0x00FF);

    /*
     * Only allowed descriptor types for SET_DESCRIPTOR command are : 
     *    -  DEVICE
     *    -  CONFIGURATION 
     *    -  STRING 
     * See 2.0 Spec (Section 9.4.8).
     */
    switch ( descriptor_type)
    {
        case USB_DESCRIPTOR_DEVICE :
		USB_LOG(DEBUG_USB20,"Set Descriptor : Device\n",0,0,0,0);
		/* Nothing to do as of now */
		break;
		
	case USB_DESCRIPTOR_CONFIG : 
		USB_LOG(DEBUG_USB20,"Set Descriptor : Config\n",0,0,0,0);
		/* Nothing to do as of now */
		break;
		
	case USB_DESCRIPTOR_STRING : 
		USB_LOG(DEBUG_USB20,"Set Descriptor : String\n",0,0,0,0);
		/* Nothing to do as of now */
		break;
		
	default : 
		status = USB_STATUS_INV_ARG;
		USB_LOG(DEBUG_USB20,"Wrong get descriptor type : %d\n",descriptor_type,0,0,0);
    }

    USB_EXIT;
    return status;
}


INLINE static INT32  usb_drv_cmd_get_configuration(USB_DEV *p_dev)
{
    USB_SETUP  *p_setup;
    INT32      status = USB_STATUS_OK;
    
    USB_ENTER;
    p_setup = (USB_SETUP *)&(p_dev->ccpu->work_buf[0]);
    /* Validate setup packet */
    if (    (p_setup->wValue != 0) 
         || (p_setup->wIndex != 0) 
	 || (p_setup->wLength != (USB_SWAPW(1))) )
    {
        /* The HOST violated the USB Specification 2.0 */
        USB_LOG(DEBUG_USB20, "ERROR: GET configuration violation by HOST.\n",0,0,0,0);
        status = USB_STATUS_INV_COMMAND;
    }
    else
    {
        USB_PKT      *pkt;
	USB_SENDINFO *sendinfo;
	
        pkt = (USB_PKT *)&(p_dev->ccpu->pkt);
        pkt->buf_list = (USB_BUF *)&(p_dev->ccpu->pkt_buf);
        sendinfo = (USB_SENDINFO *)&(p_dev->ccpu->sendinfo);
        pkt->buf_list[0].data = (UINT8 *)&(p_dev->ccpu->current_config);
	pkt->buf_list[0].len  = 1;
	sendinfo->ep          = (USB_EP *)&p_dev->ep_control;
        /* sendinfo->os_sendinfo = ; Need Anything - Saby */
        sendinfo->owner       = USB_PROTOCOL_DRIVER;
	
	status = usb_drv_control_send(p_dev, pkt, sendinfo);
    }
    
    USB_EXIT;
    return status;
}


INLINE static INT32  usb_drv_cmd_set_configuration(USB_DEV *p_dev)
{
    USB_SETUP  *p_setup;
    USB_CONFIG_DESCR *p_config;
    INT32      status = USB_STATUS_OK;
    
    USB_ENTER;
    p_setup = (USB_SETUP *)&(p_dev->ccpu->work_buf[0]);
    /* Validate the arguments in the SETUP packet. */
    if ( (p_setup->wLength != 0) || (p_setup->wIndex != 0) )
    {
        /* The HOST violated the USB Specification 1.1. */
        USB_LOG((USB20_CONTROL|USB20_ISR), "ERROR: SET CONFIG violation by HOST.\n",0,0,0,0);
	status = USB_STATUS_INV_COMMAND;
    }
    else 
    {
	p_config = (USB_CONFIG_DESCR *)p_dev->usb_cfg->config;
	if ( (p_config->bConfigurationValue == USB_SWAPW(p_setup->wValue))
			||  (0 == p_setup->wValue))
	{
	    p_dev->ccpu->current_config = (UINT8)USB_SWAPW(p_setup->wValue);
	}
	else
            status = USB_STATUS_INV_ARG;
    }
    
    USB_EXIT;
    return status;
}

/*
 *
 * Since there is no alternate settings for RNDIS we always return 0.
 * Modify this later. There should be a placeholder for interface
 */

INLINE static INT32  usb_drv_cmd_get_interface(USB_DEV *p_dev)
{
    USB_SETUP *p_setup;
    INT32     status = USB_STATUS_OK;
    UINT8     iface = 0; 

    USB_ENTER;
    
    p_setup = (USB_SETUP *)&(p_dev->ccpu->work_buf[0]);
    
    /* Validate the arguments in the SETUP packet. */
    if ( (p_setup->wValue != 0) || (p_setup->wLength != (USB_SWAPW(1))) )
    {
        /* The HOST violated the USB 2.0 Specification */
        USB_LOG(DEBUG_USB20, "ERROR: GET INTERFACE violation by HOST.\n",0,0,0,0);
        status = USB_STATUS_INV_COMMAND;
    }
    else
    {
        USB_PKT      *pkt;
	USB_SENDINFO *sendinfo;
	
        pkt           =  (USB_PKT *)&(p_dev->ccpu->pkt);
        pkt->buf_list =  (USB_BUF *)&(p_dev->ccpu->pkt_buf);
        sendinfo      =  (USB_SENDINFO *)&(p_dev->ccpu->sendinfo);
        pkt->buf_list[0].data = (UINT8 *)&iface;
	pkt->buf_list[0].len  = 1;
	sendinfo->ep          = (USB_EP *)&p_dev->ep_control;
        /* sendinfo->os_sendinfo = ; Need Anything - Saby */
        sendinfo->owner       = USB_PROTOCOL_DRIVER;
	
	status = usb_drv_control_send(p_dev, pkt, sendinfo);
    }
    
    USB_EXIT;
    return status;
}


INLINE static INT32  usb_drv_cmd_set_interface(USB_DEV *p_dev)
{
    
    USB_ENTER;
    USB_LOG(DEBUG_USB20,"SET Interface Command from Host\n",0,0,0,0);
    /* For RNDIS Driver we should not reach here - Checkout later */
    USB_EXIT;

    return USB_STATUS_OK;
}

INLINE static INT32 usb_drv_process_standard_cmd (USB_DEV *p_dev)
{
    USB_SETUP   *p_setup;
    INT32       status = USB_STATUS_OK;
    INT32       ep_to_stall = 0;
    
    USB_ENTER;

    /* Pick up the setup packet. */
    p_setup = (USB_SETUP *)&(p_dev->ccpu->work_buf[0]);

    HAL_CONTROL(p_dev, hkUsbDirCtrl, haSETbyVAL, HC_USB_DATADIR_OUT);

    /* Process the request on the basis of the request type. */
    switch (p_setup->bRequest)
    {
        case USB_GET_STATUS:
        {

        	USB_LOG(USB20_CONTROL,"Get Status\n",0,0,0,0);
            /* Process the GET Status request. */
	      HAL_CONTROL(p_dev, hkUsbDirCtrl, haSETbyVAL, HC_USB_DATADIR_IN);
            status = usb_drv_cmd_get_status (p_dev);
            break;
        }
        case USB_CLEAR_FEATURE:
        {
            /* Process the CLEAR Feature Request. */
            status = usb_drv_cmd_clear_feature (p_dev);
	    IN_ACK(p_dev->params.usb_base);
            break;              
        }
        case USB_SET_FEATURE:
        {
            /* Process the SET Feature request. */
            status = usb_drv_cmd_set_feature (p_dev);
            break;              
        }
        case USB_SET_ADDRESS:
        {
        	USB_LOG(USB20_CONTROL,"Set Address\n",0,0,0,0);
            /* Process the SET Address Request. */
            status = usb_drv_cmd_set_address (p_dev);
	    IN_ACK(p_dev->params.usb_base);  
	    USB_LOG(USB20_CONTROL,"SetAddr ACK\n",0,0,0,0);
            break;
        }
        case USB_GET_DESCRIPTOR:
        {
        	USB_LOG(USB20_CONTROL,"Get Descriptor\n",0,0,0,0);
            /* Process the GET Descriptor Request. */
	    HAL_CONTROL(p_dev, hkUsbDirCtrl, haSETbyVAL, HC_USB_DATADIR_IN);
            status = usb_drv_cmd_get_descriptor (p_dev);
            break;
        }
        case USB_SET_DESCRIPTOR:
        {
        	USB_LOG(USB20_CONTROL,"Set Descriptor\n",0,0,0,0);
            /* Process the SET Descriptor Request. */
            status = usb_drv_cmd_set_descriptor (p_dev);
            break;              
        }
        case USB_GET_CONFIGURATION:
        {
        	USB_LOG(USB20_CONTROL,"Get Config\n",0,0,0,0);
            /* Process the GET configuration request. */
	    HAL_CONTROL(p_dev, hkUsbDirCtrl, haSETbyVAL, HC_USB_DATADIR_IN);
            status = usb_drv_cmd_get_configuration (p_dev);
            break;              
        }
        case USB_SET_CONFIGURATION:
        {
            /* Process the SET Configuration Request. */
             status = usb_drv_cmd_set_configuration (p_dev);
	     IN_ACK(p_dev->params.usb_base); 
	     p_dev->stats.EnumCount++;
	     USB_LOG(USB20_CONTROL,"Setconfig ACK\n",0,0,0,0);
	     USB_LOG(USB20_CONTROL,"===========================================\n",0,0,0,0);
	     USB_LOG(USB20_CONTROL,"       Device Enumeration Complete\n",0,0,0,0);
	     USB_LOG(USB20_CONTROL,"===========================================\n",0,0,0,0);
            break;              
        }
        case USB_GET_INTERFACE:
        {
            /* Process the GET Interface Request. */
	    HAL_CONTROL(p_dev, hkUsbDirCtrl, haSETbyVAL, HC_USB_DATADIR_IN);
            status = usb_drv_cmd_get_interface (p_dev);
            break;              
        }
        case USB_SET_INTERFACE:
        {
            /* Process the SET Interface Request. */
            status = usb_drv_cmd_set_interface (p_dev);
	    IN_ACK(p_dev->params.usb_base);
            break;              
        }
        case USB_SYNCH_FRAME:
        {
            USB_LOG(DEBUG_USB20, "DEBUG: Sync Frame command from HOST.\n",0,0,0,0);
            break;
        }
        default:
        {
            /* The HOST has sent a packet in violation of the USB standard. */
            USB_LOG(DEBUG_USB20, "ERROR: Standard Cmd Request Type: %d violates USB Spec 2.0.\n", p_setup->bRequest,0,0,0);
            status = USB_STATUS_INV_ARG;
        }
    }

    /* 
     *   Check if the request was succesfully processed or there was an error 
     *   doing so. The request may NOT be processed because of :  
     *             a) USB Specification was violated.
     *             b) Request was NOT handled. 
     *   The USB Specification in either case would like the control endpoints 
     *   to be stalled. 
     */

    /* Stall Control End Point */
    if ( status != USB_STATUS_OK ) 
    {
	 USB_LOG(USB20_CONTROL,"Stalling End Points\n",0,0,0,0);
        HAL_CONTROL(p_dev, hkInStall,  haSETbyVAL, ep_to_stall);
        HAL_CONTROL(p_dev, hkOutStall, haSETbyVAL, ep_to_stall);
    }

    USB_EXIT;
    /* Return the status */
    return status;
}



/***************************************************************************
 *
 *    Generic USB Interrupt Service Routine. This routine may be called
 *    either in Interrupt Context or in some Task context depending on 
 *    the design consideration. 
 *     
 *    Some of the USB interrupts are handled by HAL and some are passed 
 *    to driver for processing. This routine will process all such 
 *    interrupts which are not serviced by HAL. 
 *
 ***************************************************************************/

/*void usb_drv_service_int(USB_DEV  *p_dev, UINT32 val) */
void usb_drv_service_int(USB_DEV *p_dev, UINT32 int_val)
{
    UINT32  val;

#ifdef USB_ENABLE_TASK
while (1) 
{
    SAL_event_wait(g_event_int, WAIT_FOREVER);
    
    val = global_int_value;
#else
      val=int_val;
#endif
    switch (val)
    {
        case CPUSB_INT_STPOW : 
		USB_LOG(DEBUG_USB20,"Overwrite Int\n",0,0,0,0);
		/* Do nothing */
                break;
        
        case CPUSB_INT_SETUP : 
                USB_LOG(USB20_CONTROL,"SETUP\n",0,0,0,0);
		usb_drv_setup_handler(p_dev);
                break;
        
        case CPUSB_INT_RESR :  
               USB_LOG(USB20_CONTROL,"RESUME\n",0,0,0,0 );
		   /* Do nothing */
                break;

        case CPUSB_INT_SUSR : 
		USB_LOG(USB20_CONTROL,"Suspend Int\n",0,0,0,0);
		usb_drv_link_down(p_dev);
                break;

        case CPUSB_INT_RSTR : 
		USB_LOG(USB20_CONTROL,"Reset Int\n",0,0,0,0);
		usb_drv_reset_handler(p_dev);
                break;

        case CPUSB_INT_IEP0_ACK : 
		USB_LOG(USB20_CONTROL,"IAK Int\n",0,0,0,0);
		usb_drv_start_control_tx(p_dev);
                break;

        case CPUSB_INT_VBUS_CONT :
		USB_LOG(USB20_CONTROL,"Cnct Int\n",0,0,0,0);
		/* Set up all channels with CPPI */
                break;

        case CPUSB_INT_VBUS_DISCONT : 
		USB_LOG(USB20_CONTROL,"Discnt Int\n",0,0,0,0);
		usb_drv_link_down(p_dev);
		/* Stop the driver ? Notify upper layer ? */
                break;

	case CPUSB_INT_OEP0_ACK : 
		USB_LOG(USB20_CONTROL,"OAk Int\n",0,0,0,0);
		usb_drv_control_receive(p_dev);
                break;

        default : 
		USB_LOG((USB20_FATAL|USB20_CONTROL),"Unknown Interrupt\n",0,0,0,0);
    }
#ifdef USB_ENABLE_TASK
}
#endif 

    USB_EXIT;
}

