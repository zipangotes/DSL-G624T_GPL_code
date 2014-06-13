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
**|         Copyright (c) 1998-2003 Texas Instruments Incorporated           |**
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
 * FILE PURPOSE :  RNDIS USB Implementation.    
 **************************************************************************
 * FILE NAME    :  usbs_rndis.c
 *
 * DESCRIPTION  :
 *  Implements the protocol which resides on top of the USB layer. This file
 *  implements the RNDIS Protocol Version 1.0.
 *
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/

#include "psp_types.h"
#include "ndis.h"
#include "usb20_rndis.h"
#include "usb20_def.h"
#include "usb20_lib.h"
#include "usb20_config.h"


/*********************** Application USB Specific Definations *******************/

#define RNDIS_VENDOR_DVR_VER_MAJOR  0x0     /* Driver Major Number. */
#define RNDIS_VENDOR_DVR_VER_MINOR  0x1     /* Driver Minor Number. */

#define RNDIS_MAX_TX_BUFFER_SPACE   1600    /* Max. Buffer size.    */

/* Application strings should be defined as needed.
 * NOTES: The first three strings are hardcoded in the CODE. Do not
 * change their positions ! */
char **usb_app_strings; 

/*********************** Application USB Specific Structures *******************/

/* Supported list of Object Identifiers. */
static UINT32 RNDIS_Supported_OID[] =
{
    HAL_SYS_SWAPL(OID_GEN_RNDIS_CONFIG_PARAMETER),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_SUPPORTED_LIST),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_HARDWARE_STATUS),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_MEDIA_SUPPORTED),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_MEDIA_IN_USE),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_MAXIMUM_FRAME_SIZE),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_LINK_SPEED),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_TRANSMIT_BUFFER_SPACE),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_TRANSMIT_BLOCK_SIZE),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_RECEIVE_BLOCK_SIZE),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_VENDOR_ID),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_VENDOR_DESCRIPTION),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_CURRENT_PACKET_FILTER),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_MAXIMUM_TOTAL_SIZE),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_MAC_OPTIONS),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_MEDIA_CONNECT_STATUS),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_VENDOR_DRIVER_VERSION),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_PHYSICAL_MEDIUM),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_XMIT_OK),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_RCV_OK),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_XMIT_ERROR),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_RCV_ERROR),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_RCV_NO_BUFFER),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_DIRECTED_BYTES_XMIT),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_DIRECTED_FRAMES_XMIT),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_MULTICAST_BYTES_XMIT),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_MULTICAST_FRAMES_XMIT),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_BROADCAST_BYTES_XMIT),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_BROADCAST_FRAMES_XMIT),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_DIRECTED_BYTES_RCV),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_DIRECTED_FRAMES_RCV),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_MULTICAST_BYTES_RCV),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_MULTICAST_FRAMES_RCV),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_BROADCAST_BYTES_RCV),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_BROADCAST_FRAMES_RCV),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_RCV_CRC_ERROR),
    HAL_SYS_SWAPL(RNDIS_OID_GEN_TRANSMIT_QUEUE_LENGTH),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_PERMANENT_ADDRESS),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_CURRENT_ADDRESS),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_MULTICAST_LIST),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_MAXIMUM_LIST_SIZE),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_RCV_ERROR_ALIGNMENT),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_XMIT_ONE_COLLISION),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_XMIT_MORE_COLLISIONS),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_XMIT_DEFERRED),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_XMIT_MAX_COLLISIONS),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_RCV_OVERRUN),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_XMIT_UNDERRUN),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_XMIT_HEARTBEAT_FAILURE),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_XMIT_TIMES_CRS_LOST),
    HAL_SYS_SWAPL(RNDIS_OID_802_3_XMIT_LATE_COLLISIONS),
};



/************************** STATIC Declarations ***************************/

/* RNDIS Layer Initialization Functions. */
static RNDIS_MCB*             rndis_usb_init (HAL_HND h_end,HAL_HND h_device,char *ptr_mac_address,HAL_SYS_TBL*);
static RNDIS_USB_CONFIG*      rndis_initialize_config_descriptor (RNDIS_MCB *ptr_rndis_mcb);
static USB_DEVICE_DESCR* rndis_initialize_device_descriptor (RNDIS_MCB *ptr_rndis_mcb);
static HAL_USB_STRING_ENTRY*  rndis_initialize_string_descriptor (RNDIS_MCB *ptr_rndis_mcb);
static HAL_USB_STRING_ENTRY*  rndis_find_str_descriptor (RNDIS_MCB* ptr_rndis_mcb, UINT8 index);

/* RNDIS USB HAL Call back routines. */
static int   rndis_usb_notify (HAL_HND h_device, UINT32 events);
static void* rndis_usb_ioctl  (HAL_HND h_device, UINT32 command, void *arg1, void*arg2, void*arg3);

/* Control PATH Routines. */
static int   rndis_usb_control_receive (HAL_HND endpoint, UINT32 data[], UINT32 len[], UINT32 num);
static int   rndis_usb_control_tx_complete (HAL_HND h_device, UINT32 chain_id);

/* Bulk PATH Routines. */
static int  rndis_receive_bulk_data (HAL_HND endpoint, USB_PKT *pkt);
static int  rndis_send_bulk_data_complete (HAL_HND h_device, UINT32 chain_id);

/* RNDIS Stack Functions related to CONTROL endpoints */
static void rndis_process_initialize_message (HAL_HND h_device, RNDIS_MESSAGE *p_message);
static void rndis_process_query_message (HAL_HND h_device, RNDIS_MESSAGE *p_message);
static void rndis_process_keep_alive_message (HAL_HND h_device, RNDIS_MESSAGE *p_message);
static void rndis_process_halt_message  (HAL_HND h_device, RNDIS_MESSAGE *p_message);
static void rndis_process_set_message   (HAL_HND h_device, RNDIS_MESSAGE *p_message);
static void rndis_process_reset_message (HAL_HND h_device, RNDIS_MESSAGE *p_message);
static void rndis_usb_reset (RNDIS_MCB* ptr_rndis_mcb);

/* RNDIS Stack functions related to BULK endpoints. */
void rndis_usb_poll (void* h_usbapp);

/* General Utiltiy Function. */
static void convert_string_to_unicode(char *string, UINT16* unicode_string, UINT16 *length);
static void  rndis_initialize_app_strings(HAL_SYS_TBL * ptr_sys_tbl);

	
/************************** EXTERN Declarations ***************************/

extern int   usblogMsg (int level, char *fmt, ...);

/* OS Required calls. */
extern int   sysUsb_load (HAL_HND h_usbapp, void* poll_rtn);
extern int   sysUsb_start(HAL_HND h_device);
extern int   sysUsb_stop (HAL_HND h_sys, HAL_HND h_device);
extern void  sys_usb_start_polling (HAL_HND h_sys);
extern void  sys_usb_stop_polling  (HAL_HND h_sys);
extern int   sys_critical_start (HAL_SYS_CRIT_CNT* p_context);
extern int   sys_critical_end (HAL_SYS_CRIT_CNT* p_context);

/* OS End Driver required calls. */
extern void  sysRndisDeallocateBuffer (HAL_HND h_end, char *ptr, UINT32 rx_chain_id);
extern char* sysRndisAllocateBuffer (HAL_HND h_end, UINT32 *rx_chain_id);
extern int   sysRndisEndReceiveData (HAL_HND h_end, char *ptr_data, UINT16 packet_length, UINT32 rx_chain_id);
extern void  sysRndisEndSndPktComplete (UINT32 chain_id);

/* Configurable Callouts. */
extern UINT16 hal_usb_get_PID (RNDIS_MCB *ptr_rndis_mcb);
extern UINT16 hal_usb_get_VID (RNDIS_MCB *ptr_rndis_mcb);
extern void hal_usb_get_manufacturer_string (char *ptr_manufacturer_string);
extern void hal_usb_get_product_description (char *ptr_product_string);
extern int hal_usb_get_serial_string (RNDIS_MCB *ptr_rndis_mcb, char *ptr_serial_string);

/****************************** FUNCTIONS *********************************/

/**************************************************************************
 * FUNCTION NAME : rndis_usb_load
 **************************************************************************
 * DESCRIPTION   :
 *  This function is called to initialize the RNDIS layer. The RNDIS 
 *  layer also initializes the USB HAL.
 *  
 * RETURNS       :
 *  USB Device handle       - Success.
 *  NULL                    - Error.
 ***************************************************************************/
void* rndis_usb_load 
(
    HAL_HND         h_end,              /* HANDLE of the RNDIS End driver.  */
    char*           ptr_mac_address,    /* MAC Address of the HOST.         */
    HAL_SYS_TBL*    ptr_sys_tbl         /* System Support Table used by HAL */
)
{
    HAL_HND     h_device;
    RNDIS_MCB*  ptr_rndis_mcb;

    /* Open the USB device. */
    h_device = hal_usb_dev_open (ptr_sys_tbl);
    if (h_device == NULL)
    {
        usblogMsg (HAL_SYS_FATAL, "Error: Unable to open the USB device.\n",0,0,0,0,0);
        return NULL;
    }

    /* Initialize the strings */
    rndis_initialize_app_strings(ptr_sys_tbl);
	
    /* Initialize the USB Application. */
    ptr_rndis_mcb = rndis_usb_init (h_end, h_device, ptr_mac_address, ptr_sys_tbl);
    if (ptr_rndis_mcb == NULL)
    {
        usblogMsg (HAL_SYS_FATAL, "Error: Unable to initialize the USB Application.\n",0,0,0,0,0);
        return NULL;
    }
    
    return ((void *)ptr_rndis_mcb);
}

/**************************************************************************
 * FUNCTION NAME : rndis_usb_start
 **************************************************************************
 * DESCRIPTION   :
 *  Starts the RNDIS layer and USB HAL and opens any necessary endpoints.
 *  
 * RETURNS       :
 *  USB Device handle       - Success.
 *  NULL                    - Error.
 ***************************************************************************/
void rndis_usb_start (HAL_HND hnd_rndis)
{
    HAL_USB_APP_INFO*   ptr_app_info;
    RNDIS_MCB*          ptr_rndis_mcb;

    /* Get the RNDIS Master control block. */
    ptr_rndis_mcb = (RNDIS_MCB *)hnd_rndis;

    /* Allocate memory for the APPLICATION information block. */
    ptr_app_info = (HAL_USB_APP_INFO *)ptr_rndis_mcb->sys.malloc (sizeof (HAL_USB_APP_INFO));
    if (ptr_app_info == NULL)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Memory Allocation failed.\n", 0,0,0,0,0);
        return;
    }

    /* Construct the DEVICE configuration which needs to be passed to the USB HAL. */
    (HAL_SYS_MEMSET_RTN)(ptr_rndis_mcb->sys.memset)((void *)ptr_app_info, 0, sizeof (HAL_USB_APP_INFO));

    /* Fill in the APPLICATION Information. */
    ptr_app_info->unit              = 1;
    ptr_app_info->h_usbapp          = (HAL_HND)ptr_rndis_mcb;
    ptr_app_info->receive_rtn       = rndis_usb_control_receive;
    ptr_app_info->notify_rtn        = rndis_usb_notify;
    ptr_app_info->tx_cmplt_rtn      = rndis_usb_control_tx_complete;
    ptr_app_info->ioctl_rtn         = rndis_usb_ioctl;

    /* Start the USB HAL. */
    if (hal_usb_dev_start (ptr_rndis_mcb->h_device, ptr_app_info) < 0)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Unable to start the USB device.\n",0,0,0,0,0);
        return;
    }


    /********** Application opens the pipes that are required by it. ***********/

    /* Control IN Pipe. */
    ptr_rndis_mcb->ctrl_in = hal_usb_open_pipe (ptr_rndis_mcb->h_device, HAL_USB_CTRL_IN, NULL);
    if (ptr_rndis_mcb->ctrl_in == NULL)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: RNDIS cannot open the pipe handle.\n",0,0,0,0,0);
        return;
    }
    
    /* Control OUT Pipe. */
    ptr_rndis_mcb->ctrl_out = hal_usb_open_pipe (ptr_rndis_mcb->h_device, HAL_USB_CTRL_OUT, NULL);
    if (ptr_rndis_mcb->ctrl_out == NULL)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: RNDIS cannot open the pipe handle.\n",0,0,0,0,0);
        return;
    }

    /* Interrupt IN endpoint. */
    ptr_rndis_mcb->intr_in = hal_usb_open_pipe (ptr_rndis_mcb->h_device, HAL_USB_INTERRUPT_IN, NULL);
    if (ptr_rndis_mcb->intr_in == NULL)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: RNDIS cannot open the pipe handle.\n",0,0,0,0,0);
        return;
    }

    /* BULK Out endpoints. */
    ptr_rndis_mcb->bulk_out = hal_usb_open_pipe (ptr_rndis_mcb->h_device, HAL_USB_BULK_OUT, 
                                                 (void *)rndis_receive_bulk_data);
    if (ptr_rndis_mcb->bulk_out == NULL)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: RNDIS cannot open the pipe handle.\n",0,0,0,0,0);
        return;
    }
    
    /* BULK IN Endpoint. */
    ptr_rndis_mcb->bulk_in = hal_usb_open_pipe (ptr_rndis_mcb->h_device, HAL_USB_BULK_IN, 
                                                (void *)rndis_send_bulk_data_complete);
    if (ptr_rndis_mcb->bulk_out == NULL)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: RNDIS cannot open the pipe handle.\n",0,0,0,0,0);
        return;
    }

    return;
}

/**************************************************************************
 * FUNCTION NAME : rndis_usb_unload
 **************************************************************************
 * DESCRIPTION   :
 *  This function is used to unload the USB HAL and close the RNDIS layer. 
 *
 * RETURNS       :
 *  < 0         -  Error
 *  0           -  Success
 ***************************************************************************/
int rndis_usb_unload (HAL_HND h_rndis)
{
    RNDIS_MCB*              ptr_rndis_mcb;
    void*                   ptr_node;
    HAL_USB_STRING_ENTRY*   ptr_str_entry;
    
    /* Get the RNDIS Master control block. */
    ptr_rndis_mcb = (RNDIS_MCB *)h_rndis;

    /****************** RNDIS Layer Cleanup *******************************/

    /* RESET the USB layer. */
    rndis_usb_reset (ptr_rndis_mcb);

    /* Cleanup memory associated with the RNDIS-USB layer. */
    ptr_rndis_mcb->sys.free ((void *)ptr_rndis_mcb->ptr_device_desc);
    ptr_rndis_mcb->sys.free ((void *)ptr_rndis_mcb->ptr_config_desc);

    /* Cleanup memory for the string descriptor */  
    ptr_str_entry = (HAL_USB_STRING_ENTRY *)usb_list_remove ( (USB_LIST_NODE **)&ptr_rndis_mcb->ptr_string_desc);
    while (ptr_str_entry != NULL)
    {
        /* Clean up memory of the contents of the string descriptor. */
        ptr_rndis_mcb->sys.free ((void*)ptr_str_entry->p_usb_str_descr);
            
        /* Cleanup the current string entry. */
        ptr_rndis_mcb->sys.free ((void *)ptr_str_entry);

        /* Get the next NODE. */
        ptr_str_entry = (HAL_USB_STRING_ENTRY *)usb_list_remove ( (USB_LIST_NODE **)&ptr_rndis_mcb->ptr_string_desc);
    }

    /* Cleanup the CONTROL buffers... */
    ptr_node = (void *)usb_list_remove ( (USB_LIST_NODE **)&ptr_rndis_mcb->p_free_list);
    while (ptr_node != NULL)
    {
        /* Cleanup the NODE. */
        ptr_rndis_mcb->sys.free ((void *)ptr_node);

        /* Get the next NODE. */
        ptr_node = (void *)usb_list_remove ( (USB_LIST_NODE **)&ptr_rndis_mcb->p_free_list);
    }

    /* Cleanup the DATA buffers... */
    ptr_node = (void *)usb_list_remove ( (USB_LIST_NODE **)&ptr_rndis_mcb->ptr_bulk_free_list);
    while (ptr_node != NULL)
    {
        /* Cleanup the NODE. */
        ptr_rndis_mcb->sys.free ((void *)ptr_node);

        /* Get the next NODE. */
        ptr_node = (void *)usb_list_remove ( (USB_LIST_NODE **)&ptr_rndis_mcb->ptr_bulk_free_list);
    }
    
    /********************************************************************************************/
    
    /* Close the USB Device. */
    if (hal_usb_dev_close (ptr_rndis_mcb->h_device) < 0)
    {
        usblogMsg (HAL_SYS_FATAL, "Error: Unable to close USB HAL.\n",0,0,0,0,0);
        return -1;
    }

    /* Cleanup the RNDIS master control block. */
    ptr_rndis_mcb->sys.free ((void *)ptr_rndis_mcb);
    
    return (0);
}

/**************************************************************************
 * FUNCTION NAME : rndis_initialize_device_descriptor
 **************************************************************************
 * DESCRIPTION   :
 *  The function initializes the device descriptor that will be requested
 *  by the HOST during the enumeration process. The DEVICE descriptor is 
 *  initialized and defined as per the RNDIS specification.
 *  
 * RETURNS       :
 *  NULL                         - Error.
 *  Pointer to Device Descriptor - Success.
 ***************************************************************************/
static USB_DEVICE_DESCR* rndis_initialize_device_descriptor (RNDIS_MCB *ptr_rndis_mcb)
{
    USB_DEVICE_DESCR*  p_device;

    /* Allocate memory for the device descriptor. */
    p_device = (USB_DEVICE_DESCR* )ptr_rndis_mcb->sys.malloc (sizeof(USB_DEVICE_DESCR));
    if (p_device == NULL)
        return NULL;

    /* Fill in the device descriptor. */
    p_device->bLength           = sizeof (USB_DEVICE_DESCR);
    p_device->bDescriptorType   = USB_DEVICE;
    p_device->bcdUSB            = HAL_SYS_SWAP (USB_VERSION_2_0);
    p_device->bDeviceClass      = USB_CDC_COMMUNICATION_CLASS;
    p_device->bDeviceSubclass   = 0x0;
    p_device->bDeviceProtocol   = USB_CDC_DATA_NO_PROTOCOL;
    p_device->bMaxPacketSize0   = USB20_EP0_MAXPKTSIZE;
    p_device->idVendor          = HAL_SYS_SWAP (hal_usb_get_VID(ptr_rndis_mcb));
    p_device->idProduct         = HAL_SYS_SWAP (hal_usb_get_PID(ptr_rndis_mcb));
    p_device->bcdDevice         = HAL_SYS_SWAP (RNDIS_USB_DEV_CODE);
    p_device->iManufacturer     = RNDIS_USB_DEV_MANUF_STR;
    p_device->iProduct          = RNDIS_USB_DEV_PROD_STR;

    /* Check if serial number support is present or not ? The code
     * assumes that if present the serial number string will be copied at
     * an index of 2. */
    if (hal_usb_get_serial_string (ptr_rndis_mcb, usb_app_strings[2]) == 1)
    {
        /* Serial number string support present. The serial number will be present
         * at index 3 in the string descriptor table. */
        p_device->iSerialNumber     = 3;
    }
    else
    {
        /* No serial number support present.        */
        p_device->iSerialNumber     = 3;
    }

    p_device->bNumConfigurations= 0x1;
    /* Return back the device descriptor. */
    return p_device;
}


/**************************************************************************
 * FUNCTION NAME : rndis_initialize_config_descriptor
 **************************************************************************
 * DESCRIPTION   :
 *  The function initializes the RNDIS Configuration descriptor that is 
 *  passed to the HOST during the enumeration process. The CONFIG descriptor
 *  is defined and initialized as specified by the RNDIS Specification 
 *  version 1.0
 *  
 * RETURNS       :
 *  NULL                         - Error. Unable to allocate memory.
 *  Pointer to config descriptor - Success.
 ***************************************************************************/
static RNDIS_USB_CONFIG *rndis_initialize_config_descriptor (RNDIS_MCB *ptr_rndis_mcb)
{
    RNDIS_USB_CONFIG*   ptr_cfg;
    UINT16              ctrlIn_size;
    UINT16              ctrlOut_size;
    UINT16              bulkIn_size;
    UINT16              bulkOut_size;
    UINT16              interruptIn_size;
    int                 arg1;
    UINT8               arg2;
            
    /* Query the USB HAL for endpoint sizes.. */
    arg1 = HAL_USB_CTRL_IN;
    ctrlIn_size = hal_usb_dev_ioctl (ptr_rndis_mcb->h_device,HAL_USB_GET_EP_SIZE, &arg1);
    if (ctrlIn_size == 0xFFFF)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Ioctl command NOT supported.\n",0,0,0,0,0);
        return NULL;
    }
    
    arg1 = HAL_USB_CTRL_OUT;
    ctrlOut_size = hal_usb_dev_ioctl (ptr_rndis_mcb->h_device,HAL_USB_GET_EP_SIZE, &arg1);
    if (ctrlOut_size == 0xFFFF)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Ioctl command NOT supported.\n",0,0,0,0,0);
        return NULL;
    }
    
    arg1 = HAL_USB_BULK_IN;
    bulkIn_size = hal_usb_dev_ioctl (ptr_rndis_mcb->h_device,HAL_USB_GET_EP_SIZE, &arg1);
    if (bulkIn_size == 0xFFFF)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Ioctl command NOT supported.\n",0,0,0,0,0);
        return NULL;
    }
    arg1 = HAL_USB_BULK_OUT;
    bulkOut_size = hal_usb_dev_ioctl (ptr_rndis_mcb->h_device,HAL_USB_GET_EP_SIZE, &arg1);
    if (bulkOut_size == 0xFFFF)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Ioctl command NOT supported.\n",0,0,0,0,0);
        return NULL;
    }
    arg1 = HAL_USB_INTERRUPT_IN;
    interruptIn_size = hal_usb_dev_ioctl (ptr_rndis_mcb->h_device,HAL_USB_GET_EP_SIZE, &arg1);
    if (interruptIn_size == 0xFFFF)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Ioctl command NOT supported.\n",0,0,0,0,0);
        return NULL;
    }

    /* Allocate memory for the configuration. */
    ptr_cfg = (RNDIS_USB_CONFIG *)ptr_rndis_mcb->sys.malloc (sizeof (RNDIS_USB_CONFIG));
    if (ptr_cfg == NULL)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Unable to allocate memory.\n",0,0,0,0,0);
        return NULL;
    }

    /********************** USB CONFIGURATION Descriptor ******************/
    
    ptr_cfg->cfg.bLength            = 0x9;                  /* Size of the descriptor.              */
    ptr_cfg->cfg.bDescriptorType    = USB_CONFIGURATION;    /* Confiuration Descriptor.             */
    ptr_cfg->cfg.wTotalLength       = HAL_SYS_SWAP (sizeof(RNDIS_USB_CONFIG));  /* Total Size       */
    ptr_cfg->cfg.bNumInterfaces     = 0x2;                  /* Two Interface - Data + Communication */
    ptr_cfg->cfg.bConfigurationValue= 0x1;                  /* ID of this configuration.            */
    ptr_cfg->cfg.iConfiguration     = 0x0;                  /* Unused.                              */

    /* Ask the USB layer for the attributes. */
    if (hal_usb_dev_ioctl (ptr_rndis_mcb->h_device, HAL_USB_IOCTL_GET_ATTRIB, &arg2) < 0)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Ioctl command NOT supported.\n",0,0,0,0,0);
        return NULL;
    }
    ptr_cfg->cfg.bmAttributes =  0xc0; /*(UINT8)arg2;*/

    /* Power Consumption. */
    if (hal_usb_dev_ioctl (ptr_rndis_mcb->h_device, HAL_USB_IOCTL_GET_MAX_POWER, &arg2) < 0)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Ioctl command NOT supported.\n",0,0,0,0,0);
        return NULL;
    }
    ptr_cfg->cfg.MaxPower = 50; /*(UINT8)arg2;*/
    
    /********************** Interface Descriptor ******************/

    /* After the configuration descriptor, the interface descriptor needs to 
     * be returned to the HOST. See USB Specification 9.4.3. 
     * The example is for RNDIS which describes the first interface descriptor
     * to be a COMMUNICATION interface. */
    ptr_cfg->comm_iface.bLength          = 0x9;                 /* Size of the descriptor.          */
    ptr_cfg->comm_iface.bDescriptorType  = USB_INTERFACE;       /* Interface Descriptor.            */
    ptr_cfg->comm_iface.bInterfaceNumber = 0x0;                 /* Index of this interface.         */
    ptr_cfg->comm_iface.bAlternateSetting= 0x0;                 /* Index of this setting.           */
    ptr_cfg->comm_iface.bNumEndpoints    = 0x1;                 /* Number of endpoint for this iface*/
    ptr_cfg->comm_iface.bInterfaceClass  = USB_CDC_COMMUNICATION_INTERFACE; /* Communication class  */
    ptr_cfg->comm_iface.bInterfaceSubClass = USB_CDC_ABSTRACT_MODEL;        /* Abstract Contol Model*/
    ptr_cfg->comm_iface.bInterfaceProtocol = USB_CDC_COMM_VENDOR_PROTOCOL;  /* Vendor Specific      */
    ptr_cfg->comm_iface.iInterface         = RNDIS_USB_COMM_IFACE_STRING;   /* Comm. String index   */ 
    
    /********************** Class Specific descriptors ******************/

    /* According to the USB specification 1.1 - 9.4.3. Class specific descriptors
     * follow the standard descriptors which they extend. */

    /********** Header Function Descriptor **********/

    /* Each header functional descriptor can only be 5 bytes long. This is fixed
     * by the USB specfication. */
    ptr_cfg->header_func.bLength            = 5;
    ptr_cfg->header_func.bDescriptorType    = USB_CDC_CS_INTERFACE;
    ptr_cfg->header_func.bDescriptorSubtype = USB_CDC_HEADER_FUNCTIONAL_SUBTYPE;
    ptr_cfg->header_func.bbcdCDC            = HAL_SYS_SWAP (USB_CDC_VER);

    /********** Call Management Descriptor **********/

    /* Each call management descriptor can only be 5 bytes long. */
    ptr_cfg->call_mgmt.bLength              = 5;
    ptr_cfg->call_mgmt.bDescriptorType      = USB_CDC_CS_INTERFACE;
    ptr_cfg->call_mgmt.bDescriptorSubtype   = USB_CDC_CALL_MGMT_SUBTYPE;
    ptr_cfg->call_mgmt.bmCapabilities       = USB_CDC_CALL_MGMT_NOT_SUPPORTED;
    ptr_cfg->call_mgmt.bDataInterface       = 0x1;

    /********** Abstract Control Descriptor **********/

    /* Abstract Control Descriptor is always 4 bytes long. */
    ptr_cfg->abstr_ctrl.bLength             = 4; 
    ptr_cfg->abstr_ctrl.bDescriptorType     = USB_CDC_CS_INTERFACE;
    ptr_cfg->abstr_ctrl.bDescriptorSubtype  = USB_CDC_ABSTRACT_CTRL_MGMT_SUBTYPE;
    ptr_cfg->abstr_ctrl.bmCapabilities      = USB_CDC_ABST_CTRL_NO_CMD_SUPPORT;

    /********** Union Functional Descriptor **********/

    ptr_cfg->union_func.bLength             = 5;                    /* Size of descriptor.      */
    ptr_cfg->union_func.bDescriptorType     = USB_CDC_CS_INTERFACE; /* INTERFACE descriptor.    */
    ptr_cfg->union_func.bDescriptorSubtype  = USB_CDC_UNION_FUNCT_SUBTYPE;  /* Union Func. Desc.*/
    ptr_cfg->union_func.bMasterInterface    = 0x0;                  /* Master Interface - Comm  */
    ptr_cfg->union_func.bSlaveInterface     = 0x1;                  /* Slave  Interface - Data  */

    /********************** Endpoint descriptors ******************/

    /* Once the descriptor and all class specific descriptors have been
     * placed. The endpoint descriptors now need to be initialized. */

    /* Each endpoint descriptor is 7 bytes long. */
    ptr_cfg->interrupt_ep.bLength = 0x7;

    /* This is an endpoint descriptor. */
    ptr_cfg->interrupt_ep.bDescriptorType = USB_ENDPOINT;
    
    /* Interrupt endpoint is a IN direction endpoint (Device->Host)*/
    ptr_cfg->interrupt_ep.bEndpointAddress = USB_EP_IN_DIR | HAL_USB_APP_EP_INT_ID;

    /* This is an interrupt endpoint. */
    ptr_cfg->interrupt_ep.bmAttributes = USB_EP_INTERRUPT;

    /* Max packet size that can be supported by this endpoint. We have already queried the
     * host for the size of the endpoints. */
    interruptIn_size = 8; 
    ptr_cfg->interrupt_ep.wMaxPacketSize = HAL_SYS_SWAP(interruptIn_size);

    /* Interrupt endpoint polling interval. */
    ptr_cfg->interrupt_ep.bInterval = RNDIS_USB_INT_POLL_TIME;

    /********************** Data Interface Descriptor ******************/

    /* The communication class interface has been fully configured. The second interface
     * now needs to be configured. */
    
    /* Interface descriptor can be 9 bytes long. */
    ptr_cfg->data_iface.bLength = 0x9;

    /* This is an interface descriptor. */
    ptr_cfg->data_iface.bDescriptorType = USB_INTERFACE;

    /* Identifiers for this interface. */
    ptr_cfg->data_iface.bInterfaceNumber  = 0x1;
    ptr_cfg->data_iface.bAlternateSetting = 0x0;

    /* In RNDIS there are 2 endpoints for the data interface. BULK In and BULK Out. */
    ptr_cfg->data_iface.bNumEndpoints     = 0x2;
    
    /* This interface is used for exchange of data. */
    ptr_cfg->data_iface.bInterfaceClass    = USB_CDC_DATA_INTERFACE;
    ptr_cfg->data_iface.bInterfaceSubClass = USB_CDC_DATA_SUBCLASS_CODE;
    ptr_cfg->data_iface.bInterfaceProtocol = USB_CDC_DATA_NO_PROTOCOL;

    /* String description for the interface. */
    ptr_cfg->data_iface.iInterface = RNDIS_USB_IFACE2_STRING;

    /********************** Bulk OUT Endpoint descriptors ******************/
    
    /* Each endpoint descriptor is 7 bytes long. */
    ptr_cfg->bulkOut_ep.bLength = 0x7;

    /* This is an endpoint descriptor. */
    ptr_cfg->bulkOut_ep.bDescriptorType = USB_ENDPOINT;

    /* Bulk Out endpoint is (Host -> Device). */
    ptr_cfg->bulkOut_ep.bEndpointAddress = USB_EP_OUT_DIR | HAL_USB_APP_EP_BULK_ID;

    /* This is a BULK endpoint. */
    ptr_cfg->bulkOut_ep.bmAttributes = USB_EP_BULK;

    /* The max size of data that can be handled by the endpoint has already been requested
     * from the USB HAL. */
    /*ptr_cfg->bulkOut_ep.wMaxPacketSize = HAL_SYS_SWAP(bulkOut_size);*/
    /*bulkOut_size = 64;*/
    ptr_cfg->bulkOut_ep.wMaxPacketSize = HAL_SYS_SWAP(bulkOut_size);
    ptr_cfg->bulkOut_ep.bInterval      = USB_EP_POLL_NOT_USED;

    /********************** Bulk IN Endpoint descriptors ******************/
    
    /* Each endpoint descriptor is 7 bytes long. */
    ptr_cfg->bulkIn_ep.bLength          = 0x7;

    /* This is an endpoint descriptor. */   
    ptr_cfg->bulkIn_ep.bDescriptorType  = USB_ENDPOINT;
    
    /* Bulk IN endpoint is (Device -> HOST). */
    ptr_cfg->bulkIn_ep.bEndpointAddress = USB_EP_IN_DIR | HAL_USB_APP_EP_BULK_ID;

    /* This is a BULK endpoint. */
    ptr_cfg->bulkIn_ep.bmAttributes     = USB_EP_BULK;
    
    /* The max size of data that can be handled by the endpoint has already been requested
     * from the USB HAL. */
    /* bulkIn_size = 64;*/
    ptr_cfg->bulkIn_ep.wMaxPacketSize   = HAL_SYS_SWAP(bulkIn_size);
    ptr_cfg->bulkIn_ep.bInterval        = USB_EP_POLL_NOT_USED;

    /* Return back the parameters. */
    return ptr_cfg;
}

/**************************************************************************
 * FUNCTION NAME : rndis_initialize_string_descriptor
 **************************************************************************
 * DESCRIPTION   :
 *  The function allocates memory and initializes the STRING descriptor that
 *  are to be used by the USB HAL. 
 *  
 * RETURNS       :
 *  NULL                         - Error. Unable to allocate memory.
 *  Pointer to string descriptor - Success.
 ***************************************************************************/
static HAL_USB_STRING_ENTRY* rndis_initialize_string_descriptor (RNDIS_MCB *ptr_rndis_mcb)
{
    HAL_USB_STRING_ENTRY*   ptr_str_entry;
    int                     index = 0;
    UINT16                  languageId;
    UINT16                  length;
    UINT16                  unicode_string[RNDIS_USB_MAX_UNICODE_LENGTH];
        
    /* The first node will always be a list of all the languages that are supported
     * by the application. */
    ptr_str_entry = (HAL_USB_STRING_ENTRY *)ptr_rndis_mcb->sys.malloc (sizeof (HAL_USB_STRING_ENTRY));
    if (ptr_str_entry == NULL)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Unable to allocate memory for the string entry.\n",
                   0,0,0,0,0);
        return NULL;
    }

    /* Fill in the fields. Language ID are identified by the index of 0. DO NOT change
     * this index. */
    ptr_str_entry->index = 0;

    /* Allocate memory for all the languages that can be supported. Each language supported
     * will have a 2 byte language ID that needs to be accounted for. */
    length = sizeof (UINT16) * RNDIS_USB_NUM_OF_LANG_SUPPORT;

    /* String Descriptor Length will always be 2 bytes more than all the language Ids. */
    ptr_str_entry->str_descr_length = length + 2;

    /* Allocate a block of memory. */
    ptr_str_entry->p_usb_str_descr = (char *)ptr_rndis_mcb->sys.malloc (ptr_str_entry->str_descr_length);
    if (ptr_str_entry->p_usb_str_descr == NULL)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Unable to allocate memory for LANGUAGE Id block.\n",
                   0,0,0,0,0);
        return NULL;
    }

    /* The first byte will always be the total length of the string descriptor in bytes. */
    ptr_str_entry->p_usb_str_descr[0] = (char)ptr_str_entry->str_descr_length;

    /* The next byte identifies the string descriptor. */
    ptr_str_entry->p_usb_str_descr[1] = USB_STRING;
    
    /* English is supported. Application may add any more language support Id's as required. */
    languageId = HAL_SYS_SWAP(USB_LANG_ID_ENG);

    /* Copy the language ID into the string descriptor. */
    ptr_rndis_mcb->sys.memcpy ((void *)&ptr_str_entry->p_usb_str_descr[2],
                               (void*)&languageId, sizeof (languageId));

    /* Add the string entry into the LIST. */
    usb_list_add ( (USB_LIST_NODE **)&ptr_rndis_mcb->ptr_string_desc, (USB_LIST_NODE *)ptr_str_entry);

    /* Query for the manufacturer string. We assume that the manufacturer string will always 
     * be stored at index 0. */
    hal_usb_get_manufacturer_string (usb_app_strings[0]);
    
    /* Query for the product description string. We assume that the product string will always 
     * be stored at index 1. */
    hal_usb_get_product_description (usb_app_strings[1]);

    /* Process all the entries specified by the USB application. */
    while ( usb_app_strings[index] != NULL)
    {
        /* Allocate memory for the string entry. */
        ptr_str_entry = (HAL_USB_STRING_ENTRY *)ptr_rndis_mcb->sys.malloc (sizeof (HAL_USB_STRING_ENTRY));
        if (ptr_str_entry == NULL)
        {
            usblogMsg (HAL_SYS_FATAL,"Error: Unable to allocate memory for the string entry.\n",
                       0,0,0,0,0);
            return NULL;
        }

        /* Fill in the fields. */
        ptr_str_entry->index = index + 1;

        /* Convert the string to UNICODE format. */
        convert_string_to_unicode(usb_app_strings[index], &unicode_string[0], &length);

        /* String Descriptor Length will always be 2 bytes more than the UNICODE string. */
        ptr_str_entry->str_descr_length = length + 2;
        
        /* Create the string descriptor. Each string descriptor is 2 bytes more than the UNICODE string length. */
        ptr_str_entry->p_usb_str_descr = (char *)ptr_rndis_mcb->sys.malloc (ptr_str_entry->str_descr_length);
        if (ptr_str_entry->p_usb_str_descr == NULL)
            return NULL;
        
        /* Fill in the STRING descriptor. */
        
        /* The first byte will always be the total length of the string descriptor in bytes. */
        ptr_str_entry->p_usb_str_descr[0] = (char)ptr_str_entry->str_descr_length;

        /* The next byte identifies the string descriptor. */
        ptr_str_entry->p_usb_str_descr[1] = USB_STRING;

        /* Copy the UNICODE string into the string descriptor. */
        ptr_rndis_mcb->sys.memcpy ((void *)&ptr_str_entry->p_usb_str_descr[2], (void*)&unicode_string[0], length);

        /* Add the string entry into the LIST. */
        usb_list_add ( (USB_LIST_NODE **)&ptr_rndis_mcb->ptr_string_desc, (USB_LIST_NODE *)ptr_str_entry);

        /* Jump to the next entry. */
        index = index + 1;
    }

    /* Return the head of the string descriptor list. */
    return (ptr_rndis_mcb->ptr_string_desc);
}

/**************************************************************************
 * FUNCTION NAME : rndis_usb_init
 **************************************************************************
 * DESCRIPTION   :
 *  This function is called to initialize RNDIS stack layer. It allocates
 *  and initializes the RNDIS master control block structure and all internal
 *  fields.
 *  
 * RETURNS       :
 *  Pointer to RNDIS Master Control Block   -   Success.
 *  NULL                                    -   Error.
 ***************************************************************************/
static RNDIS_MCB* rndis_usb_init 
(
    HAL_HND         h_end,
    HAL_HND         h_device,
    char*           ptr_mac_address,
    HAL_SYS_TBL*    ptr_sys_tbl
)
{
    UINT32          index;
    RNDIS_RESPONSE* p_buffer;
    RNDIS_HEADER*   p_header;
    RNDIS_MCB*      ptr_rndis_mcb;
    
    /* Allocate memory for the RNDIS Master control block. */
    ptr_rndis_mcb = (RNDIS_MCB *)ptr_sys_tbl->malloc (sizeof (RNDIS_MCB));
    if (ptr_rndis_mcb == NULL)
    {
        usblogMsg (HAL_SYS_FATAL,"Error: Unable to allocate memory for the RNDIS control block.\n",
                   0,0,0,0,0);
        return NULL;
    }

    /* Initialize the memory block. */
    (HAL_SYS_MEMSET_RTN)(ptr_sys_tbl->memset)((void *)ptr_rndis_mcb, 0, sizeof(RNDIS_MCB));

    /* Copy the SYSTEM table into the RNDIS block. */
    ptr_sys_tbl->memcpy ((void *)&ptr_rndis_mcb->sys, (void *)ptr_sys_tbl, sizeof (HAL_SYS_TBL));

    /* Remember the device handle to which the application is connected. */
    ptr_rndis_mcb->h_device = h_device; 
    ptr_rndis_mcb->h_end    = h_end;    

    /**************************** RNDIS USB Device Configurations *************************/

    /* Initialize the DEVICE descriptor. */
    ptr_rndis_mcb->ptr_device_desc = rndis_initialize_device_descriptor (ptr_rndis_mcb);

    /* Initialize the CONFIGURATION descriptor. */
    ptr_rndis_mcb->ptr_config_desc = rndis_initialize_config_descriptor (ptr_rndis_mcb);

    /* Initialize the STRING descriptors. */
    ptr_rndis_mcb->ptr_string_desc = rndis_initialize_string_descriptor (ptr_rndis_mcb);

    /*************************** RNDIS STACK Initializations ************************/

    /* Remember the MAC address of the HOST. */
    ptr_rndis_mcb->sys.memcpy ((void *)&ptr_rndis_mcb->host_mac_address[0], (void *)ptr_mac_address, 6);
    ptr_rndis_mcb->sys.memcpy ((void *)&ptr_rndis_mcb->permanent_mac_address[0], (void *)ptr_mac_address, 6);

    /* Initialize the PACKET Filter to accept all packets. DIRECTED / MULTICAST and BROADCAST */
    ptr_rndis_mcb->packet_filter =  NDIS_PACKET_TYPE_PROMISCUOUS | NDIS_PACKET_TYPE_DIRECTED      | NDIS_PACKET_TYPE_MULTICAST |
                                    NDIS_PACKET_TYPE_ALL_MULTICAST | NDIS_PACKET_TYPE_BROADCAST;

    /* The RNDIS is in the UNINITIALIZED state. */
    ptr_rndis_mcb->state = RNDIS_UNINITIALIZED;

    /*************************** CONTROL PATH Initializations ************************/

    /* Allocate memory for the RNDIS free memory pool. */
    for (index =0; index < RNDIS_CONTROL_MAX_BUFFERS; index++)
    {
        /* Allocate memory for the RNDIS buffer. */
        p_buffer = (RNDIS_RESPONSE *)ptr_rndis_mcb->sys.malloc (sizeof (RNDIS_RESPONSE));
        if (p_buffer == NULL)
        {
            usblogMsg (HAL_SYS_FATAL,"Error: Unable to allocate memory for the RNDIS buffer list.\n",0,0,0,0,0);
            return NULL;
        }

        /* Initialize the memory block. */
        (HAL_SYS_MEMSET_RTN)(ptr_rndis_mcb->sys.memset)((void *)p_buffer, 0, sizeof (RNDIS_RESPONSE));

        /* Add the buffer to the free list. */
        usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->p_free_list, (USB_LIST_NODE *)p_buffer);
    }

    /* Initially there are no responses scheduled.*/
    ptr_rndis_mcb->p_response_list = NULL;

    /* Initialize the response message. */
    ptr_rndis_mcb->response_available.bmRequestType = 0x1;
    ptr_rndis_mcb->response_available.bRequest      = USB_RESPONSE_AVAILABLE;
    ptr_rndis_mcb->response_available.wValue        = 0x0000;
    ptr_rndis_mcb->response_available.wIndex        = 0x0000;
    ptr_rndis_mcb->response_available.wLength       = 0x0000;

    /*************************** BULK Receive PATH Initializations ************************/

    /* Initialize all the receive state variables. */
    ptr_rndis_mcb->rx_buffer        = NULL;
    ptr_rndis_mcb->rx_buffer_ptr    = NULL;
    ptr_rndis_mcb->received_len     = 0;
    ptr_rndis_mcb->data_payload_len = 0;

    /* Initialize the Receive state machine. */
    ptr_rndis_mcb->receive_state = RNDIS_WAIT_FOR_HEADER_STAGE; 
    
    /*************************** BULK Transmit PATH Initializations ************************/

    /* Allocate memory for the RNDIS free memory pool. */
    for (index =0; index < RNDIS_BULK_MAX_BUFFERS; index++)
    {
        /* Allocate memory for the RNDIS buffer. */
        p_header = (RNDIS_HEADER *)ptr_rndis_mcb->sys.malloc (sizeof (RNDIS_HEADER));
        if (p_header == NULL)
        {
            usblogMsg (HAL_SYS_FATAL,"Error: Unable to allocate memory for the RNDIS buffer list.\n",0,0,0,0,0);
            return NULL;
        }
        
        /* Initialize the memory block. */
        (HAL_SYS_MEMSET_RTN)(ptr_rndis_mcb->sys.memset)((void *)p_header, 0, sizeof (RNDIS_HEADER));

        /* Add the buffer to the free list. */
        usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->ptr_bulk_free_list, (USB_LIST_NODE *)p_header);
    }
    return ptr_rndis_mcb;
}

/**************************************************************************
 * FUNCTION NAME : rndis_usb_poll
 **************************************************************************
 * DESCRIPTION   :
 *  Polling function called by the USB TASK to check if there is any data
 *  that was received on the BULK endpoint. / Any data that needs to be 
 *  transmitted on the endpoint.
 ***************************************************************************/
void rndis_usb_poll (void* h_usbapp)
{
    RNDIS_MCB* ptr_rndis_mcb = (RNDIS_MCB *)h_usbapp;

    /* Check if there is any data that is received. */
    hal_usb_dev_poll (ptr_rndis_mcb->h_device, ptr_rndis_mcb->bulk_out);

    /* Check if there is any data that is to be transmitted. */
    hal_usb_dev_poll (ptr_rndis_mcb->h_device, ptr_rndis_mcb->bulk_in);
}

/**************************************************************************
 * FUNCTION NAME : rndis_usb_notify
 **************************************************************************
 * DESCRIPTION   :
 *  Callback function registered by the application to be called when 
 *  USB events are detected. Current possible USB Events are :-
 *      1)  HAL_USB_RESET_EVENT
 * 
 * RETURNS       :
 *  0           -   Success
 *  < 0         -   Error.
 ***************************************************************************/
static int rndis_usb_notify (HAL_HND h_device, UINT32 events)
{
    RNDIS_MCB*      ptr_rndis_mcb;

    /* Get the RNDIS MCB. */
    ptr_rndis_mcb = (RNDIS_MCB *)hal_usb_get_app_handle (h_device);

    if (events & HAL_USB_RESET_EVENT)
    {
        /* Logging debug messages - application was posted a USB RESET event */
        usblogMsg (HAL_SYS_DEBUG, "DBG: USB Application resetting itself.\n",0,0,0,0,0);

        /* Reset the USB application. */
        rndis_usb_reset (ptr_rndis_mcb);
    }
	
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : rndis_usb_reset
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called to reset the USB application. This resets all
 *  the software structures that the application maintains ansd puts them
 *  to the initial startup values. 
 ***************************************************************************/
static void rndis_usb_reset (RNDIS_MCB* ptr_rndis_mcb)
{
    RNDIS_RESPONSE* p_buffer;
    
    /************************* STACK STATE Cleanup **************************/
        
    /* Initialize the PACKET Filter to accept all packets. DIRECTED / MULTICAST and BROADCAST */
    ptr_rndis_mcb->packet_filter =  NDIS_PACKET_TYPE_PROMISCUOUS |
                                    NDIS_PACKET_TYPE_DIRECTED | 
                                    NDIS_PACKET_TYPE_MULTICAST |
                                    NDIS_PACKET_TYPE_ALL_MULTICAST |
                                    NDIS_PACKET_TYPE_BROADCAST;
    
    /* The RNDIS module has been reset. Switch back to the PERMANENT mac address. */    
    ptr_rndis_mcb->sys.memcpy ((void *)&ptr_rndis_mcb->host_mac_address[0], 
                               (void *)&ptr_rndis_mcb->permanent_mac_address[0], 6);
        
    /* The RNDIS is in the UNINITIALIZED state. */
    ptr_rndis_mcb->state = RNDIS_UNINITIALIZED;

    /************************* CONTROL PATH Cleanup ************************/
    
    /* Dequeue any control responses that were queued up. */
    p_buffer = (RNDIS_RESPONSE *)usb_list_remove( (USB_LIST_NODE**)&ptr_rndis_mcb->p_response_list);
    while (p_buffer != NULL)
    {
        /* LOG Debug messages. */
        usblogMsg (HAL_SYS_DEBUG, "DBG: Descheduling RESPONSE as APP reset.\n",0,0,0,0,0);

        /* Add the buffer back to the free list. */
        usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->p_free_list, (USB_LIST_NODE *)p_buffer);

        /* Are there more responses scheduled ? */
        p_buffer = (RNDIS_RESPONSE *)usb_list_remove( (USB_LIST_NODE**)&ptr_rndis_mcb->p_response_list);
    }

    /************************* BULK Receive PATH Cleanup ************************/

    /* Initialize the Receive state machine related parameters. */
    ptr_rndis_mcb->rx_buffer        = NULL;
    ptr_rndis_mcb->rx_buffer_ptr    = NULL;
    ptr_rndis_mcb->received_len     = 0;
    ptr_rndis_mcb->data_payload_len = 0;
            
    /* Initialize the Receive state machine. */
    ptr_rndis_mcb->receive_state = RNDIS_WAIT_FOR_HEADER_STAGE;

     /* Clear the stats */
     memset((void *)(&ptr_rndis_mcb->stats),0,sizeof(RNDIS_STATS));

	 
    return;
}

/**************************************************************************
 * FUNCTION NAME : rndis_usb_ioctl
 **************************************************************************
 * DESCRIPTION   :
 *  Registered call back IOCTL routine called by the HAL layer to inform
 *  the application layer about certain USB events related to configuration
 *  management.
 ***************************************************************************/
static void* rndis_usb_ioctl 
(
    HAL_HND     h_device, 
    UINT32      command, 
    void*       arg1, 
    void*       arg2,
    void*       arg3
)
{
    RNDIS_MCB*      ptr_rndis_mcb;

    /* Get the RNDIS MCB. */
    ptr_rndis_mcb = (RNDIS_MCB *)hal_usb_get_app_handle (h_device);
        
    /* Process the IOCTL request on the basis of the command. */
    switch (command)
    {
        case USB_APP_IOTCL_GET_DEVICE_DESC:
        {
            /* The USB HAL requested for the DEVICE descriptor. */
            int*    size;

            /* The USB HAL would send the parameters in the following format
             *  1)  arg1, arg2, arg3 - NOT used.    
             * The USB HAL would require a response in the following format.
             *  1)  arg1 - Return the length of the device descriptor.
             *  2)  Return the pointer to the device descriptor */
            size = (int *)arg1;
            *size = sizeof (USB_DEVICE_DESCR);
            
            /* Return the pointer to the USB Device descriptor. */
            arg1 = (void *)&size;

            /* Return the device descriptor. */
            return (void *)ptr_rndis_mcb->ptr_device_desc;
        }
        case USB_APP_IOTCL_GET_CONFIG_DESC:
        {
            /* The USB HAL requested for the CONFIGURATION Descriptor. */
            int*    size;

            /* The USB HAL would send the parameters in the following format
             *  1)  arg1, arg2, arg3 - NOT used.    
             * The USB HAL would require a response in the following format.
             *  1)  arg1 - Return the length of the configuration descriptor.
             *  2)  Return the pointer to the configuration descriptor */
            size = (int *)arg1;
            *size = sizeof (RNDIS_USB_CONFIG);
                    
            /* Return config descriptor. */
            return (void *)ptr_rndis_mcb->ptr_config_desc;
        }
        case USB_APP_IOTCL_GET_STRING_DESC:
        {
            /* The USB HAL requested for the STRING Descriptor. */
            HAL_USB_STRING_ENTRY*   ptr_string_entry;
            UINT8*                  index;

            /* The USB HAL would send the parameters in the following format
             *  1)  arg1 - Index of the string descriptor to be returned.
             *  2)  arg2, arg3 - NOT used.  
             * The USB HAL would require a response in the following format.
             *  1)  Return the pointer to the string descriptor */
            index = (UINT8 *)arg1;

            /* Find this string descriptor. */
            ptr_string_entry = rndis_find_str_descriptor (ptr_rndis_mcb, *index);
            if (ptr_string_entry == NULL)
            {
                /* Could NOT find the string descriptor return an Error. */
                usblogMsg (HAL_SYS_FATAL, "Error: Unable to find string descriptor with index:%d.\n",
                           *index,0,0,0,0);
                return NULL;
            }

            /* Return the string descriptor. */
            return (void *)ptr_string_entry;
        }
        case USB_APP_IOTCL_GET_INTERFACE:
        {
            /* The USB HAL has received a GET interface request */
            UINT16* iface_index;

            /* The USB HAL would send the parameters in the following format
             *  1)  arg1 - Pointer to the interface id.
             *  2)  arg2, arg3 - NOT used.
             * The USB HAL would require a response in the following format.
             *  1)  arg2 - Pointer to the alternate setting buffer to be filled up. */
            iface_index = (UINT16 *)arg1;

            /* Since there is no alternative settings for the RNDIS interfaces. Always return 0. */ 
            *(UINT8 *)arg2 = 0x0;
            return NULL;
        }
        case USB_APP_IOCTL_SET_INTERFACE:
        {
            /* The USB HAL has received a SET interface request. */
            UINT16* iface_index;
            UINT16* alt_setting;

            /* The USB HAL would send the parameters in the following format
             *  1)  arg1 - Pointer to the interface id.
             *  2)  arg2 - Pointer to the alternate settings sent by HOST.
             *  3)  arg3 - NOT used.
             * The USB HAL would require a response in the following format.
             *  No Response is required. */
            iface_index     = (UINT16 *)arg1;
            alt_setting     = (UINT16 *)arg2;

            /* TODO: Application specific processing. */
            return NULL;
        }
        case USB_APP_SET_DEVICE_DESCRIPTOR:
        {
            /* The USB HAL has received a SET DEVICE descriptor request. */
            USB_DEVICE_DESCR*  ptr_device_descriptor;
            UINT16*                 length;
            
            /* The USB HAL would send the parameters in the following format
             *  1)  arg1 - Pointer to the new device descriptor received.
             *  2)  arg2 - Pointer to the length of the new device descriptor 
             *  3)  arg3 - NULL
             * The USB HAL would require a response in the following format.
             *  No Response is required. */
            ptr_device_descriptor = (USB_DEVICE_DESCR *)arg1;
            length                = (UINT16 *)arg2;

            /* TODO: Any application specific processing that would be required. */
            return NULL;
        }
        case USB_APP_SET_CONFIG_DESCRIPTOR:
        {
            /* The USB HAL has received a SET CONFIG descriptor request. */
            char*   ptr_config_descriptor;
            int*    length;
            
            /* The USB HAL would send the parameters in the following format
             *  1)  arg1 - Pointer to the new config descriptor received.
             *  2)  arg2 - Pointer to the length of the new config descriptor 
             *  3)  arg3 - NULL
             * The USB HAL would require a response in the following format.
             *  No Response is required. */
            ptr_config_descriptor = (char *)arg1;
            length                = (int *)arg2;

            /* TODO: Any application specific processing that would be required. */
            return NULL;
        }
        case USB_APP_SET_STRING_DESCRIPTOR:
        {
            /* The USB HAL has received a SET STRING descriptor request. */
            char*   ptr_string_descriptor;
            int*    length;
            UINT8*  index;
            
            /* The USB HAL would send the parameters in the following format
             *  1)  arg1 - Pointer to the new string descriptor received.
             *  2)  arg2 - Pointer to the length of the new string descriptor
             *  3)  arg3 - Index of the string descriptor to be changed.
             * The USB HAL would require a response in the following format.
             *  No Response is required. */
            ptr_string_descriptor = (char *)arg1;
            length                = (int *)arg2;
            index                 = (UINT8 *)arg3;

            /* TODO: Any application specific processing that would be required. */
            return NULL;
        }       
        default:
        {
            usblogMsg (HAL_SYS_FATAL,"ERROR: USB HAL called IOCTL with invalid command:%d.\n", 
                       command,0,0,0,0);
            break;
        }       
    }
    return NULL;
}

/**************************************************************************
 * FUNCTION NAME : rndis_usb_control_tx_complete
 **************************************************************************
 * DESCRIPTION   :
 *  The function is the registered callback routine that is used to called
 *  after a packet has been fully transmitted on the CONTROL Endpoint.
 *  The 'h_device' is the handle to the USB HAL device
 *  The 'chain_id' is the address of the buffer that needs to be cleaned up.
 *  
 * RETURNS       :
 *  Always returns 0.
 ***************************************************************************/
static int rndis_usb_control_tx_complete (HAL_HND h_device, UINT32 chain_id)
{
    RNDIS_RESPONSE* p_buffer;
    RNDIS_MCB*      ptr_rndis_mcb;

    /* Get the RNDIS MCB. */
    ptr_rndis_mcb = (RNDIS_MCB *)hal_usb_get_app_handle (h_device);

    /* Do we have a valid chain_id that needs to be cleaned up. ? */
    if (chain_id != 0)
    {
        /* YES. Get the RNDIS Buffer. */
        p_buffer = (RNDIS_RESPONSE *)chain_id;

        /* Add the buffer back to the free list. */
        usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->p_free_list, (USB_LIST_NODE *)p_buffer);
    }
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : rndis_send_bulk_data_complete
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called to clean up memory of a packet that has been 
 *  transmitted on the BULK endpoint. 
 *  
 * RETURNS       :
 *  0       - Success.
 *  < 0     - Error.
 *
 * NOTES         :
 *  The current USB implementation polls for data on the BULK endpoints. So 
 *  this function will be called from the context of the USB Polling task. 
 *  Transmission of data i.e. the sending function is called from the context
 *  of the NET task. CRITICAL Sections are required and are done so in the 
 *  USB HAL. 
 ***************************************************************************/
static int rndis_send_bulk_data_complete (HAL_HND h_device, UINT32 chain_id)
{
    RNDIS_HEADER*   ptr_rndis_header;
    RNDIS_MCB*      ptr_rndis_mcb;
    
    /* Get the RNDIS Master control block. */
    ptr_rndis_mcb = hal_usb_get_app_handle (h_device);

    /* Do we have a valid chain identifier ? */
    if (chain_id == 0)
    {
        usblogMsg (HAL_SYS_FATAL, "ERROR: USB HAL posted an invalid chain_id.\n",0,0,0,0,0);
        return -1;
    }

    /* Remove the header from the scheduled list. */
    ptr_rndis_header = (RNDIS_HEADER *)chain_id;
    
    /* Use the chain_id stored in the header to clean up data. */
    sysRndisEndSndPktComplete (ptr_rndis_header->chain_id);
    /* Add the header back to the free list. */ 
    usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->ptr_bulk_free_list, (USB_LIST_NODE *)ptr_rndis_header);
    return 0;
}

/**
 * @ingroup Rndis_Driver
 *
 *  The function is the registered as call back routine with USB Protocol driver. 
 *  It will be called once data is received on the BULK endpoint.
 *
 *  @param  pipe   The communication pipe on which data has been received.
 *
 *  @param  pkt  The USB packet which has the received data.
 *  
 *  @return   0       - Success.
 *  < 0     - Error.
 */

static int rndis_receive_bulk_data (HAL_HND pipe, USB_PKT *pkt)
{
    RNDIS_MESSAGE   *ptr_rndis_message;
    int             index;
    RNDIS_PACKET     *ptr_rndis_packet;
    char*           ptr_data;
    UINT16          payload_length;
    UINT8            *dst_mac_address;
    RNDIS_MCB*      ptr_rndis_mcb;

    /* Get the device handle. */
    ptr_rndis_mcb = (RNDIS_MCB *)hal_usb_get_app_handle_from_endpoint (pipe);

    /* Cycle through all the buffers that have been received */
    for (index = 0; index < pkt->num_buf; index ++)
    {
               /* YES. We are waiting for the RNDIS header + data payload. */
                ptr_rndis_message = (RNDIS_MESSAGE *)(pkt->buf_list[index].data);
              
                /* Make sure that the received packet is a RNDIS packet. */
                if (ptr_rndis_message->NdisMessageType != HAL_SYS_SWAPL(REMOTE_NDIS_PACKET_MSG))
                {
                    /* Increment the error counter. */
                    ptr_rndis_mcb->stats.receive_packets_dropped++;

                    /* Log Debug Messages. */
                    usblogMsg (HAL_SYS_DEBUG, "ERROR: Received Invalid RNDIS packet from HOST. Type:0x%x.\n", 
                                ptr_rndis_message->NdisMessageType,0,0,0,0);
                    return -1;
                }

                /* Get the RNDIS packet. */
                ptr_rndis_packet = (RNDIS_PACKET *)(&(ptr_rndis_message->Message.Packet));

                /* Validate the fields in the RNDIS packet. For connectionless devices the handle is 0. */
                if ( (ptr_rndis_packet->Reserved != 0x0) || (ptr_rndis_packet->PacketVcHandle != 0x0) )
                {
                    /* Increment the error counter. */
                    ptr_rndis_mcb->stats.receive_packets_dropped++;
                    usblogMsg (HAL_SYS_DEBUG, "ERROR: RNDIS Spec violation Reserved:0x%x VcHandle:0x%x.\n",
                        ptr_rndis_packet->Reserved, ptr_rndis_packet->PacketVcHandle,0,0,0);
                    return -1;
                }

             
    
               /* Remember the total length of the data payload expected. */
                ptr_rndis_mcb->data_payload_len = HAL_SYS_SWAPL (ptr_rndis_packet->DataLength);

                /* Jump to the data payload. The 'data payload' will be at DataOffset bytes
                 * from the start of the RNDIS packet. */
                ptr_data = (char *)ptr_rndis_packet + HAL_SYS_SWAPL (ptr_rndis_packet->DataOffset);

                /* Compute the length of data payload. We received len[index] bytes out of which
                 * sizeof (RNDIS_PACKET) + sizeof (NdisMessageType) + sizeof (MessageLength) are
                 * the header sizes that need to be accounted for. The remaining length is the size 
                 * of the data payload. */
                payload_length = pkt->buf_list[index].len - ( sizeof (RNDIS_PACKET) + 
                                                sizeof (ptr_rndis_message->NdisMessageType) +
                                                sizeof (ptr_rndis_message->MessageLength) );
                
                ptr_rndis_mcb->rx_buffer = (char *)ptr_data; 
                
                
                if(payload_length != ptr_rndis_mcb->data_payload_len)
                {
                    usblogMsg(HAL_SYS_FATAL,"Incorrect size RNDIS packet received. Expected %d Actual %d\n",
                                ptr_rndis_mcb->data_payload_len,payload_length,0,0,0);
			if ( (payload_length == (ptr_rndis_mcb->data_payload_len+1)) && 
				(*(ptr_data+payload_length-1) == 0x0) ) 
				payload_length -=1;
			else
				{
				     USB_LOG(USB20_FATAL,"Packet Dropped\n",0,0,0,0);
                               return -1;
				}
                  }

                  ptr_rndis_mcb->received_len = payload_length;
                
                    /* All the data has been received. Determine the MAC address and 
                     * increment the appropriate statistics.
                    dst_mac_address[0] = *((UINT16*)ptr_rndis_mcb->rx_buffer);
                    dst_mac_address[1] = *((UINT16*)(ptr_rndis_mcb->rx_buffer+2));
                    dst_mac_address[2] = *((UINT16*)(ptr_rndis_mcb->rx_buffer+4));
                    */

                    dst_mac_address = (UINT8 *)ptr_rndis_mcb->rx_buffer;
                    
                    /* Is the PACKET a multicast / broadcast ? */
                    if ( (dst_mac_address[0] & 1) == 1)
                    {                       
                        /* Is the packet a BROADCAST packet ? */
                        if ((*((UINT16 *)&(dst_mac_address[0])) == 0xFFFF) && 
			    (*((UINT16 *)&(dst_mac_address[2])) == 0xFFFF) && 
                            (*((UINT16 *)&(dst_mac_address[4])) == 0xFFFF))
                        {
                            /* BROADCAST packet. Increment stats. */
                            ptr_rndis_mcb->stats.broadcast_pkts_rxed  = ptr_rndis_mcb->stats.broadcast_pkts_rxed  + 1;
                            ptr_rndis_mcb->stats.broadcast_bytes_rxed = ptr_rndis_mcb->stats.broadcast_bytes_rxed + 
                                                                  ptr_rndis_mcb->received_len;
                        }
                        else
                        {
                            /* MULTICAST packet. Increment stats. */
                            ptr_rndis_mcb->stats.multicast_pkts_rxed  = ptr_rndis_mcb->stats.multicast_pkts_rxed  + 1;
                            ptr_rndis_mcb->stats.multicast_bytes_rxed = ptr_rndis_mcb->stats.multicast_bytes_rxed + 
                                                                  ptr_rndis_mcb->received_len;
                        }
                    }
                    else
                    {
                        /* UNICAST packet. Increment the appropriate statistics. */
                        ptr_rndis_mcb->stats.unicast_pkts_rxed  = ptr_rndis_mcb->stats.unicast_pkts_rxed  + 1;
                        ptr_rndis_mcb->stats.unicast_bytes_rxed = ptr_rndis_mcb->stats.unicast_bytes_rxed + 
                                                            ptr_rndis_mcb->received_len;
                    }
                
             /* Pass the data to the UPPER layer. */
             sysRndisEndReceiveData (ptr_rndis_mcb->h_end, ptr_rndis_mcb->rx_buffer,
                                            ptr_rndis_mcb->received_len, (UINT32)pkt->buf_list[index].info);

        }

    return 0;
    
}


/**
 * @ingroup Rndis_Driver
 *
 *  The function is the registered as call back routine with USB Protocol driver. 
 *  It will be called once data is received on the BULK endpoint.  
 *
 *  @param  endpoint   The control endpoint on which data has been received.
 *
 *  @param  data   An array of data buffers.
 *
 *  @param  len   An array of length of the above data buffers.
 *
 *  @param  num  Number of data buffers.
 *  
 *  @return   0       - Success.
 *  < 0         -   Error.
 */

static int rndis_usb_control_receive 
(
    HAL_HND     endpoint, 
    UINT32      data[], 
    UINT32      len[], 
    UINT32      num
)
{
    USB_SETUP*      p_setup;
    RNDIS_MESSAGE*  pMessage;
    UINT32          rndis_msg_type;
    HAL_HND         h_usbapp;
    RNDIS_RESPONSE* p_buffer;
    RNDIS_MCB*      ptr_rndis_mcb;

    /* Get the application handle. */
    h_usbapp        = hal_usb_get_app_handle_from_endpoint (endpoint);
    ptr_rndis_mcb   = (RNDIS_MCB *)h_usbapp;


    /* Pick up the setup packet. */
    p_setup = (USB_SETUP *)data[0];

    /* Parse the SETUP request. */
    switch (p_setup->bRequest)
    {       
        case USB_SEND_ENCAPSULATED_CMD :
        {
            USB_LOG(USB20_CONTROL,"RNDIS Send Encap Cmd\n",0,0,0,0);
            /* Get the RNDIS message. Skip the USB Setup packet. */
            pMessage = (RNDIS_MESSAGE *)( (char *)data[0] + sizeof (USB_SETUP));

            /* Process the RNDIS message on the basis of the message type. */
            rndis_msg_type = HAL_SYS_SWAPL(pMessage->NdisMessageType);
            switch(rndis_msg_type)
            {
                case REMOTE_NDIS_INITIALIZE_MSG:
                    /* Received an Initialization Message. */
                    USB_LOG(USB20_CONTROL,"RNDIS Initialize msg\n",0,0,0,0);
                    rndis_process_initialize_message (h_usbapp, pMessage);
                    break;
                case REMOTE_NDIS_HALT_MSG:
                    /* Received a HALT Message. */
                    USB_LOG(USB20_CONTROL,"RNDIS Halt Msg\n",0,0,0,0);
                    rndis_process_halt_message (h_usbapp, pMessage);
                    break;
                case REMOTE_NDIS_QUERY_MSG:
                    /* Received a Query Reques */
                    USB_LOG(USB20_CONTROL,"RNDIS Query Msg\n",0,0,0,0);
                    rndis_process_query_message (h_usbapp, pMessage);
                    break;
                case REMOTE_NDIS_SET_MSG:
                    /* Received a Set Request. */
                    USB_LOG(USB20_CONTROL,"RNDIS Set Msg\n",0,0,0,0);
                    rndis_process_set_message (h_usbapp, pMessage);
                    break;
                case REMOTE_NDIS_RESET_MSG:
                    /* Received a Reset Request. */
                    USB_LOG(USB20_CONTROL,"RNDIS Reset Msg\n",0,0,0,0);
                    rndis_process_reset_message (h_usbapp, pMessage);
                    break;
                case REMOTE_NDIS_KEEPALIVE_MSG:
                    /* Received a Keep Alive Message. */
                    USB_LOG(USB20_CONTROL,"RNDIS Keep Alive Msg\n",0,0,0,0);
                    rndis_process_keep_alive_message (h_usbapp, pMessage);
                    break;
                default:
                    /* All Other Messages received are in ERROR. */
                    usblogMsg (HAL_SYS_FATAL, "Error: Invalid RNDIS Control packet received. \n",0,0,0,0,0);
                    break;
            }
            break;
        }
        case USB_GET_ENCAPSULATED_RESP :
        {
            USB_LOG(USB20_CONTROL,"RNDIS Get Encap Msg\n",0,0,0,0);
            /* The HOST has requested for a RESPONSE to be sent to it. */
            p_buffer = (RNDIS_RESPONSE *)usb_list_remove( (USB_LIST_NODE**)&ptr_rndis_mcb->p_response_list);
            if (p_buffer == NULL)
            {
                /* FATAL Error: Host is requesting for a response and we dont have any. */
                usblogMsg (HAL_SYS_FATAL, "Error: NO response available. RNDIS layer corrupted.\n",0,0,0,0,0);
		   USB_LOG(USB20_CONTROL,"ERROR : No Response Available\n",0,0,0,0);
                return -1;
            }

            /* Transmit the response back to the HOST. */
            data[0] = (UINT32)&p_buffer->data[0];
            len [0] = p_buffer->len;
            
	    if (hal_usb_dev_send_pipe (ptr_rndis_mcb->ctrl_in, data, len, 1, (UINT32)p_buffer) < 0)
            {
                usblogMsg (HAL_SYS_FATAL, "Error: Unable to send control message.\n",0,0,0,0,0);
            }
            break;
        }
        default:
        {
            usblogMsg(HAL_SYS_FATAL, "Error: Received an unknown command: %d", p_setup->bRequest,0,0,0,0); 
            break;
        }
    }
    return 0;
}

/**
 *  @ingroup Rndis_Driver
 *
 *  The function is called from the END driver to send a packet 'ptr_data'
 *  of length 'data_length' over the USB interface. The function appends the
 *  RNDIS header and then passes this data to the USB interface. The 'chain_id'
 *  is a callback value that the upper layer function registers with the 
 *  packet. This should be used to clean packet memory after transmission is 
 *  over. 
 * 
 *  @param  hnd_rndis   Handle of RNDIS driver.
 *
 *  @param  ptr_data  Pointer to the data buffer. 
 * 
 *  @param  data_length   Length of data buffer.
 * 
 *  @param  chain_id   OS Specific buffer identifier. 
 *
 *  @return   0           -   Success
 *  < 0         -   Error.
 *  
 */
 
int rndis_usb_send_packet (void* hnd_rndis, char*ptr_data, int data_length, UINT32 chain_id)
{
    RNDIS_HEADER*       ptr_rndis_header;
    RNDIS_MESSAGE*      ptr_rndis_message;
    RNDIS_PACKET*       ptr_rndis_packet;
    UINT32              data[2];
    UINT32              len[2];
    char*               ptr_rndis_data;
/*    UINT16              bytes_left;*/
    HAL_SYS_CRIT_CNT    context;
    int                 result;
    UINT8               dst_mac_address[6];
    RNDIS_MCB*          ptr_rndis_mcb;  

    /* Get the RNDIS master control block. */
    ptr_rndis_mcb = (RNDIS_MCB *)hnd_rndis;
            
    /* Make sure that the RNDIS layer is running. */
/*  if (ptr_rndis_mcb->state != RNDIS_DATA_INITIALIZED)
        return -1;
*/

    /*Fixed to enable testing on IXIA setup -- Prashanth*/
    if(ptr_rndis_mcb->state == RNDIS_UNINITIALIZED )
        return -1;
    
    /* Retreive the destination MAC Address from the ethernet packet. */
    ptr_rndis_mcb->sys.memcpy ((void *)&dst_mac_address[0], (void *)ptr_data, 6);

    /* Is the packet a broadcast or Multicast packet ? */
    if (dst_mac_address[0] & 0x1)
    {
        /* Is the packet a BROADCAST packet ? */
        if ( (dst_mac_address[0] == 0xFF) && (dst_mac_address[1] == 0xFF) && (dst_mac_address[2] == 0xFF) && 
             (dst_mac_address[3] == 0xFF) && (dst_mac_address[4] == 0xFF) && (dst_mac_address[5] == 0xFF))
        {
            /* YES. The packet is a BROADCAST packet. Is the filter set up appropriately ? */
            if ((ptr_rndis_mcb->packet_filter & (NDIS_PACKET_TYPE_PROMISCUOUS | NDIS_PACKET_TYPE_BROADCAST)) == 0)
            {
                /* NO. The filter does NOT allow broadcast packets. Drop it ! */
                return -1;
            }
            else
            {
                /* BROADCAST packet and allowed by the FILTER. Increment the appropriate statistics. */
                ptr_rndis_mcb->stats.broadcast_pkts_txed  = ptr_rndis_mcb->stats.broadcast_pkts_txed  + 1;
                ptr_rndis_mcb->stats.broadcast_bytes_txed = ptr_rndis_mcb->stats.broadcast_bytes_txed + data_length;
            }
        }
        else
        {
            /* NO. The packet is a MULTICAST packet. Is the filter setup appropriately ? */
            if ((ptr_rndis_mcb->packet_filter & NDIS_PACKET_TYPE_PROMISCUOUS) == 0)
            {
                if ( (ptr_rndis_mcb->packet_filter & NDIS_PACKET_TYPE_ALL_MULTICAST) == 0)
                {
                    /* No. Is the filter using a MULTICAST list ? */
            if (ptr_rndis_mcb->packet_filter & (NDIS_PACKET_TYPE_MULTICAST))
            {
                /* YES. Check the MAC address with the multicast list. */
                int     list_processed  = 0;
                int     index           = 0;

                /* Traverse through the list. */
                while (list_processed < ptr_rndis_mcb->mcast_list_size)
                {
                    /* Check the packet address with the Multicast list address. */
                    if ( (ptr_rndis_mcb->mcast_list[index][0] == dst_mac_address[0]) &&
                         (ptr_rndis_mcb->mcast_list[index][1] == dst_mac_address[1]) &&
                         (ptr_rndis_mcb->mcast_list[index][2] == dst_mac_address[2]) &&
                         (ptr_rndis_mcb->mcast_list[index][3] == dst_mac_address[3]) &&
                         (ptr_rndis_mcb->mcast_list[index][4] == dst_mac_address[4]) &&
                         (ptr_rndis_mcb->mcast_list[index][5] == dst_mac_address[5]) )
                    {
                        /* We have a match ! The packet will be sent out. */
                        break;
                    }   

                    /* Jump to the next entry. */
                    list_processed = list_processed + 6;
                    index          = index + 1;
                }

                /* Did we get a match ? */
                if (list_processed >= ptr_rndis_mcb->mcast_list_size)
                {
                    /* No. There was NO match. Drop the packet ! */
                    return -1;
                }
                    }
                    else
                    {
                        /* Multicast Address but the filter is NOT promisuous or all multicast or multicast. */
                        return -1;
                    }
                }
            }
            /* MULTICAST packet and allowed by the FILTER. Increment the appropriate statistics. */
            ptr_rndis_mcb->stats.multicast_pkts_txed  = ptr_rndis_mcb->stats.multicast_pkts_txed  + 1;
            ptr_rndis_mcb->stats.multicast_bytes_txed = ptr_rndis_mcb->stats.multicast_bytes_txed + data_length;
        }
    }
    else
    {
        /* UNICAST packet. Are we running in promiscuous mode ? */ 
        if ((ptr_rndis_mcb->packet_filter & NDIS_PACKET_TYPE_PROMISCUOUS) == 0)
        {
            /* NO. We are not running in promiscuous mode. Check if we can accept directed packets ? */
            if ((ptr_rndis_mcb->packet_filter & NDIS_PACKET_TYPE_DIRECTED) == 0)
            {
                /* Directed packets are NOT allowed. Drop it ! */
                return -1;
            }
            else
            {
                /* Directed Packets are allowed. */
                if ( (ptr_rndis_mcb->host_mac_address[0] != dst_mac_address[0]) || 
                     (ptr_rndis_mcb->host_mac_address[1] != dst_mac_address[1]) ||
                     (ptr_rndis_mcb->host_mac_address[2] != dst_mac_address[2]) ||
                     (ptr_rndis_mcb->host_mac_address[3] != dst_mac_address[3]) ||
                     (ptr_rndis_mcb->host_mac_address[4] != dst_mac_address[4]) ||
                     (ptr_rndis_mcb->host_mac_address[5] != dst_mac_address[5]) )
                {
                    /* Packet is NOT directed. Drop it ! */
                    return -1;
                }
                /* Packet is Directed. Send it ! Drop through. */
            }
        }

        /* UNICAST packet. Increment the appropriate statistics. */
        ptr_rndis_mcb->stats.unicast_pkts_txed  = ptr_rndis_mcb->stats.unicast_pkts_txed  + 1;
        ptr_rndis_mcb->stats.unicast_bytes_txed = ptr_rndis_mcb->stats.unicast_bytes_txed + data_length;        
    }

    /* Get a RNDIS Header from the BULK Free list. This is a CRITICAL Section as this list
     * is manipulated both from the NET task and from the USB Polling task. */
    sys_critical_start ((HAL_SYS_CRIT_CNT *)&context);
    ptr_rndis_header = (RNDIS_HEADER *)usb_list_remove ( (USB_LIST_NODE **)&ptr_rndis_mcb->ptr_bulk_free_list);
    if (ptr_rndis_header == NULL)
    {
        /* DEBUG CODE */
        usblogMsg (HAL_SYS_FATAL, "Error: Unable to get RNDIS Header.\n",0,0,0,0,0);
        sys_critical_end ((HAL_SYS_CRIT_CNT *)&context);
        return -1;
    }
    sys_critical_end ((HAL_SYS_CRIT_CNT *)&context);
    
    /* Fill up the RNDIS header. */
    ptr_rndis_header->chain_id = chain_id;

    /* Get the RNDIS message pointer which is to be filled in and appended to the data packet 
     * before it is transmitted. */
    ptr_rndis_message = (RNDIS_MESSAGE *)&ptr_rndis_header->message;

    /* Fill up the RNDIS Message. Remember the message length is the total length of the entire message
     * which includes DATA length + RNDIS Packet length + SIZE OF Message Type + SIZE of Message length. */
    ptr_rndis_message->NdisMessageType      = HAL_SYS_SWAPL (REMOTE_NDIS_PACKET_MSG);
    ptr_rndis_message->MessageLength        = HAL_SYS_SWAPL (data_length + sizeof(RNDIS_PACKET) + 2*sizeof(UINT32));
    
    /* Get the pointer to the RNDIS packet. */
    ptr_rndis_packet = &ptr_rndis_message->Message.Packet;

    /* Fill up the RNDIS packet. */
    ptr_rndis_packet->DataOffset            = HAL_SYS_SWAPL (sizeof(RNDIS_PACKET));
    ptr_rndis_packet->DataLength            = HAL_SYS_SWAPL (data_length);
    ptr_rndis_packet->OOBDataOffset         = 0x0;
    ptr_rndis_packet->OOBDataLength         = 0x0;
    ptr_rndis_packet->NumOOBDataElements    = 0x0;
    ptr_rndis_packet->PerPacketInfoOffset   = 0x0;
    ptr_rndis_packet->PerPacketInfoLength   = 0x0;
    ptr_rndis_packet->PacketVcHandle        = 0x0;
    ptr_rndis_packet->Reserved              = 0x0;

    /* After the header has been filled up. There is some place left for the data to be copied. */
    ptr_rndis_data = (char *)ptr_rndis_packet + sizeof (RNDIS_PACKET);
#if 0
    /* Check the space left in the FIRST Buffer ? */
    bytes_left = 64 - (sizeof (RNDIS_PACKET) + 2*sizeof(UINT32));
    /* Copy the bytes into the first buffer. */
    ptr_rndis_mcb->sys.memcpy ((void *)ptr_rndis_data, (void *)ptr_data, bytes_left);
    
    /* Account for the DATA that has already been copied. */
    ptr_data = ptr_data + bytes_left;
#endif
    /* We now have 2 buffers 
     *  1.  RNDIS header + Data payload. (This will always be 64 bytes)
     *  2.  Data payload. 
     * Send these buffers over the USB. */
    data[0] = (UINT32)ptr_rndis_message;
    len [0] = /* 64 */ (sizeof (RNDIS_PACKET)+ 2*sizeof(UINT32)); 
    data[1] = (UINT32)ptr_data;
    len [1] = (data_length /*- bytes_left*/);

    /* Send the data on the pipe. */
    result = hal_usb_dev_send_pipe (ptr_rndis_mcb->bulk_in, data, len, 2, (UINT32)ptr_rndis_header);

    /* Check if there was an error returned by the HAL layer. */
    if (result < 0)
    {
        /* Increment the error counter. */
        ptr_rndis_mcb->stats.transmit_packets_dropped = ptr_rndis_mcb->stats.transmit_packets_dropped + 1;
            
        /* Error reported by HAL. Put the rndis header back into the free list. */
        sys_critical_start ((HAL_SYS_CRIT_CNT *)&context);
        usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->ptr_bulk_free_list, (USB_LIST_NODE *)ptr_rndis_header);
        sys_critical_end ((HAL_SYS_CRIT_CNT *)&context);
    }
    return result;
}

/**************************************************************************
 * FUNCTION NAME : rndis_process_initialize_message
 **************************************************************************
 * DESCRIPTION   :
 *  RNDIS Processing function for the INITIALIZE message sent by the HOST.
 *  The function creates an initialize complete message and sends a 
 *  notification message to the HOST. The initialize complete message is 
 *  queued up in response list. 
 ***************************************************************************/
static void rndis_process_initialize_message 
(
    HAL_HND         h_usbapp, 
    RNDIS_MESSAGE*  p_message
)
{   
    RNDIS_MESSAGE*              ptr_response;
    RNDIS_RESPONSE*             p_buffer;
    RNDIS_INITIALIZE_COMPLETE*  ptr_init_complete;
    UINT32                      data[1];
    UINT32                      len[1];
    RNDIS_MCB*                  ptr_rndis_mcb;

    /* Get the RNDIS MCB. */
    ptr_rndis_mcb = (RNDIS_MCB *)h_usbapp;
    
    /* Get a buffer from the free list. */
    p_buffer = (RNDIS_RESPONSE *)usb_list_remove( (USB_LIST_NODE**)&ptr_rndis_mcb->p_free_list);
    if (p_buffer == NULL)
    {
        usblogMsg (HAL_SYS_FATAL, "Error: No free control buffers to send a response.\n",0,0,0,0,0);
        return;
    }
    
    /* Create the response message. */
    ptr_response = (RNDIS_MESSAGE *) &p_buffer->data[0];

    /* Fill up the Initialization complete message. */
    ptr_init_complete = (RNDIS_INITIALIZE_COMPLETE *)&ptr_response->Message;
    ptr_init_complete->RequestId            = p_message->Message.InitializeRequest.RequestId;
    ptr_init_complete->Status               = HAL_SYS_SWAPL(RNDIS_STATUS_SUCCESS);
    ptr_init_complete->MajorVersion         = HAL_SYS_SWAPL(RNDIS_MAJOR_VERSION);
    ptr_init_complete->MinorVersion         = HAL_SYS_SWAPL(RNDIS_MINOR_VERSION);
    ptr_init_complete->DeviceFlags          = HAL_SYS_SWAPL(RNDIS_DF_CONNECTIONLESS);
    ptr_init_complete->Medium               = HAL_SYS_SWAPL(RNdisMedium802_3);
    ptr_init_complete->MaxPacketsPerMessage = HAL_SYS_SWAPL(1);
    ptr_init_complete->MaxTransferSize      = HAL_SYS_SWAPL(2048);
    ptr_init_complete->PacketAlignmentFactor= HAL_SYS_SWAPL(3);
    ptr_init_complete->AFListOffset         = HAL_SYS_SWAPL(0);
    ptr_init_complete->AFListSize           = HAL_SYS_SWAPL(0);

    /* Fill Up the Response Message. */ 
    ptr_response->NdisMessageType   = HAL_SYS_SWAPL(REMOTE_NDIS_INITIALIZE_CMPLT);
    ptr_response->MessageLength     = HAL_SYS_SWAPL(RNDIS_MESSAGE_SIZE(RNDIS_INITIALIZE_COMPLETE));
    
    /* Validate the RNDIS Initialize Message. If our major version greater than that of the
     * host, we are stuck. If major versions match, we check for the minor version. If our minor
     * version is greater than that of the host, we are stuck too. */
    if ( (HAL_SYS_SWAPL(p_message->Message.InitializeRequest.MajorVersion) < RNDIS_MAJOR_VERSION) ||
        ((HAL_SYS_SWAPL(p_message->Message.InitializeRequest.MajorVersion) == RNDIS_MAJOR_VERSION) && 
         (HAL_SYS_SWAPL(p_message->Message.InitializeRequest.MinorVersion) < RNDIS_MINOR_VERSION)) ) 
    {
        /* Versions DOES NOT match. */
        usblogMsg (HAL_SYS_FATAL, "RNDIS Error: Incorrect Version Information received. \n",0,0,0,0,0);

        /* Indicate bad status */
        ptr_init_complete->Status = HAL_SYS_SWAPL(RNDIS_STATUS_BAD_VERSION);
    }
    else
    {
        /* Move to the INITIALIZED state only if version matching was OK. */
        ptr_rndis_mcb->state = RNDIS_INITIALIZED;
    }

    /* Remember the length. */
    p_buffer->len = RNDIS_MESSAGE_SIZE(RNDIS_INITIALIZE_COMPLETE);
    
    /* Queue up the response. */
    usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->p_response_list, (USB_LIST_NODE *)p_buffer);


    /* Send the NOTIFICATION to the HOST. */
    data[0] = (UINT32)&ptr_rndis_mcb->response_available;
    len [0] = 8;
    hal_usb_dev_send_pipe (ptr_rndis_mcb->intr_in, data, len, 1, 0);

    usblogMsg(HAL_SYS_DEBUG, "DBG: INITIALIZE message from host. Setting INITIALIZED state.\n",0,0,0,0,0);
    return;
}

/**************************************************************************
 * FUNCTION NAME : rndis_process_query_message
 **************************************************************************
 * DESCRIPTION   :
 *  RNDIS Processing function for the QUERY message sent by the HOST. The 
 *  function parses the request message to determine the information 
 *  requested by the HOST, creates an appropriate query response message and
 *  sends a NOTIFICATION message to the HOST.
 ***************************************************************************/
static void rndis_process_query_message 
(
    HAL_HND         h_usbapp, 
    RNDIS_MESSAGE*  p_message
)
{
    RNDIS_MESSAGE*              ptr_response;
    RNDIS_RESPONSE*             p_buffer;
    RNDIS_QUERY_COMPLETE*       ptr_query_cmplt;
    UINT32*                     ptr_reply_buffer;
    UINT32                      data[1];
    UINT32                      len[1];
    UINT32                      response_len;
    int                         index;
    RNDIS_MCB*                  ptr_rndis_mcb;
    UINT16                      temp_uint16;
    UINT32                      temp_uint32;


    /* Get the RNDIS MCB. */
    ptr_rndis_mcb = (RNDIS_MCB *)h_usbapp;

    /* Get a buffer from the free list. */
    p_buffer = (RNDIS_RESPONSE *)usb_list_remove( (USB_LIST_NODE**)&ptr_rndis_mcb->p_free_list);
    if (p_buffer == NULL)
    {
        usblogMsg (HAL_SYS_FATAL, "Error: No free control buffers to send a response.\n",0,0,0,0,0);
        return;
    }

    /* Create a response message. */    
    ptr_response = (RNDIS_MESSAGE *) &p_buffer->data[0];
    ptr_query_cmplt = (RNDIS_QUERY_COMPLETE *)&(ptr_response->Message);

    /* Append the Query response towards the end of the Initialize complete message. */
    ptr_reply_buffer = (UINT32 *)((char *)ptr_query_cmplt + sizeof(RNDIS_QUERY_COMPLETE));

    switch (HAL_SYS_SWAPL(p_message->Message.QueryRequest.Oid))
    {
        case RNDIS_OID_GEN_SUPPORTED_LIST:
        {
            /* The object identifier is used by the HOST to return a list of object 
             * identifiers that are supported by the device. */
            usblogMsg(HAL_SYS_DEBUG, "DBG: QUERY message (SUPPORTED OIDs) from host.\n",0,0,0,0,0);
            for (index = 0; index < sizeof(RNDIS_Supported_OID) / 4; index++)
                *ptr_reply_buffer++ = RNDIS_Supported_OID[index];
            break;
        }
        case RNDIS_OID_GEN_HARDWARE_STATUS:
        {
            /* The object identifier is used by the HOST to determine the current hardware 
             * status of the device. */
            usblogMsg(HAL_SYS_DEBUG, "DBG: QUERY message (HARDWARE STATUS) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(NdisHardwareStatusReady);
            break;
        }
        case RNDIS_OID_GEN_MEDIA_SUPPORTED:
        case RNDIS_OID_GEN_MEDIA_IN_USE:
        {
            /* The object identifier is used to determine the media types that the device can support. */               
            usblogMsg(HAL_SYS_DEBUG, "DBG: QUERY message (SUPPORTED MEDIA) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(NdisMedium802_3);
            break;
        }
        case RNDIS_OID_GEN_MAXIMUM_FRAME_SIZE:
        {
            /* The object identifier is used to determine the maximum network packet size, 
             * in bytes, that the device supports. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (FRAME SIZE) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL (RNDIS_USB_MTU);
            break;
        }
        case RNDIS_OID_GEN_LINK_SPEED:
        {
            /* The object identifier is used to determine the maximum speed of the device in kbps. 
             * The unit of measurement is 100 bps. */
            usb_drv_ioctl((USB_DEV *) ptr_rndis_mcb->h_device, IOCTL_GET_USB_SPEED, (void *)&temp_uint32);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL ( /*USB_FULL_LINK_SPEED*/ temp_uint32 / 100);
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY (LINK SPEED) from host. Speed : %u\n",temp_uint32,0,0,0,0);
            break;
        }
        case RNDIS_OID_GEN_TRANSMIT_BLOCK_SIZE:
        {
            /* The object identifier is used to specify the amount of memory in bytes that is 
             * available for buffering transmit data. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (TX BLOCK SIZE) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(RNDIS_USB_MSS);
            break;
        }
        case RNDIS_OID_GEN_RECEIVE_BLOCK_SIZE:
        {
            /* The object identifier is used to specify the amount of memory in bytes that is 
             * available for buffering receive data. */             
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (RX BLOCK SIZE) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(RNDIS_USB_MSS);
            break;
        }
        case RNDIS_OID_GEN_VENDOR_ID:
        {
            /* The object identifier is used to retreive the vendor Id of the device. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (VENDOR ID) from host.\n",0,0,0,0,0);
			temp_uint16 = ptr_rndis_mcb->ptr_device_desc->idVendor;
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(temp_uint16);
            break;
        }
        case RNDIS_OID_GEN_VENDOR_DESCRIPTION:
        {
            /* The object identifier is used to retreive a textual description of the device. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (VENDOR DESCRIPTION) from host.\n",0,0,0,0,0);
/*          strcpy ( (char *)ptr_reply_buffer, RNDIS_USB_VENDOR_DESC);*/
			strcpy ( (char *)ptr_reply_buffer, usb_app_strings[0]);
            ptr_reply_buffer = (UINT32 *) ( (UINT8 *)ptr_reply_buffer + strlen((char*)ptr_reply_buffer) + 1);
            break;
        }
        case RNDIS_OID_GEN_TRANSMIT_BUFFER_SPACE:
        {
            /* The object id specifies the amount of memory, in bytes, on the device that is
             * available for buffering transmit data */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (TRANSMIT BUFFER SPACE) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL (RNDIS_MAX_TX_BUFFER_SPACE);
            break;
        }
        case RNDIS_OID_GEN_CURRENT_PACKET_FILTER:
        {
            /* The object identifier specifies the types of net packets for which a protocol receives
             * indications from the device. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (PACKET FILTER) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL (ptr_rndis_mcb->packet_filter);
            break;
        }
        case RNDIS_OID_GEN_MAXIMUM_TOTAL_SIZE:
        {
            /* The object identifier specifies the maximum total packet length, in bytes,
             * the device supports */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (MAX TOTAL SIZE) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL (RNDIS_USB_MSS);
            break;
        }
        case RNDIS_OID_GEN_MAC_OPTIONS:
        {
            /* The object Id specifies a bitmask that defines optional properties of the 
             * underlying driver or its NIC. A protocol that initiates this query can determine 
             * which of the flags the underlying driver sets, and can optionally take advantage of 
             * them. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (MAC OPTIONS) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL (NDIS_MAC_OPTION_NO_LOOPBACK);
            break;
        }
        case RNDIS_OID_GEN_MEDIA_CONNECT_STATUS:
        {
            /* The object id returns the connection status of the device on the network. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (MEDIA CONNECT STATUS) from host.\n",0,0,0,0,0);


            if (hal_usb_pform_connection_status () == 1)
                *ptr_reply_buffer++ = HAL_SYS_SWAPL (RNdisMediaStateConnected);
            else
                *ptr_reply_buffer++ = HAL_SYS_SWAPL (RNdisMediaStateDisconnected);

            break;
        }
        case RNDIS_OID_GEN_VENDOR_DRIVER_VERSION:
        {
            /* The object identifier returns the vendor assigned driver version. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (DRIVER VERSION) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL( (RNDIS_VENDOR_DVR_VER_MAJOR << 16) |
                                                 (RNDIS_VENDOR_DVR_VER_MINOR));
            break;
        }
        case RNDIS_OID_GEN_PHYSICAL_MEDIUM:
        {
            /* The object identifier returns the physical medium */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (PHYSICAL MEDIUM) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(hal_usb_get_phy_medium());
            break;
        }
        
        /********************************************************************************
         ************************************* Statistics *******************************
         ********************************************************************************/
        
        case RNDIS_OID_GEN_XMIT_OK:
        {
            /* The object identifier is used to retreive the frames transmitted without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF TX FRAMES W/O ERRORS) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.broadcast_pkts_txed + 
                                                ptr_rndis_mcb->stats.multicast_pkts_txed +
                                                ptr_rndis_mcb->stats.unicast_pkts_txed);
            break;
        }
        case RNDIS_OID_GEN_RCV_OK:
        {
            /* The object identifier is used to retreive the frames received without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF RX FRAMES W/O ERRORS) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.broadcast_pkts_rxed + 
                                                ptr_rndis_mcb->stats.multicast_pkts_rxed +
                                                ptr_rndis_mcb->stats.unicast_pkts_rxed);
            break;
        }
        case RNDIS_OID_GEN_XMIT_ERROR:
        {
            /* The object identifier is used to retreive the frames transmitted with error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF TX FRAMES WITH ERRORS) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = 0;
            break;
        }
        case RNDIS_OID_GEN_RCV_ERROR:
        {
            /* The object identifier is used to retreive the frames received with error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF RX FRAMES WITH ERRORS) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = 0;
            break;
        }
        case RNDIS_OID_GEN_RCV_NO_BUFFER:
        {
            /* The object identifier is used to retreive the number of frames dropped because of NO buffers */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NO RX BUFFERS) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = 0;
            break;
        }
        case RNDIS_OID_GEN_DIRECTED_BYTES_XMIT:
        {
            /* The object identifier is used to retreive the number of directed bytes txed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF TX UNICAST BYTES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.unicast_bytes_txed);
            break;
        }
        case RNDIS_OID_GEN_DIRECTED_FRAMES_XMIT:
        {
            /* The object identifier is used to retreive the number of directed frames txed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF TX FRAMES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.unicast_pkts_txed);
            break;
        }
        case RNDIS_OID_GEN_MULTICAST_BYTES_XMIT:
        {
            /* The object identifier is used to retreive the number of multicast bytes txed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF TX MULTICAST BYTES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.multicast_bytes_txed);
            break;
        }
        case RNDIS_OID_GEN_MULTICAST_FRAMES_XMIT:
        {
            /* The object identifier is used to retreive the number of multicast frames txed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF TX MULTICAST FRAMES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.multicast_pkts_txed);
            break;
        }
        case RNDIS_OID_GEN_BROADCAST_BYTES_XMIT:
        {
            /* The object identifier is used to retreive the number of broadcast bytes txed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF TX BROADCAST BYTES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.broadcast_bytes_txed);
            break;
        }
        case RNDIS_OID_GEN_BROADCAST_FRAMES_XMIT:
        {
            /* The object identifier is used to retreive the number of broadcast frames txed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF TX BROADCAST FRAMES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.broadcast_pkts_txed);
            break;
        }
        case RNDIS_OID_GEN_DIRECTED_BYTES_RCV:
        {
            /* The object identifier is used to retreive the number of directed bytes rxed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF RX UNICAST BYTES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.unicast_bytes_rxed);
            break;
        }
        case RNDIS_OID_GEN_DIRECTED_FRAMES_RCV:
        {
            /* The object identifier is used to retreive the number of directed frames rxed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF RX FRAMES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.unicast_pkts_rxed);
            break;
        }
        case RNDIS_OID_GEN_MULTICAST_BYTES_RCV:
        {
            /* The object identifier is used to retreive the number of multicast bytes rxed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF RX MULTICAST BYTES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.multicast_bytes_rxed);
            break;
        }
        case RNDIS_OID_GEN_MULTICAST_FRAMES_RCV:
        {
            /* The object identifier is used to retreive the number of multicast frames rxed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF RX MULTICAST FRAMES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.multicast_pkts_rxed);
            break;
        }
        case RNDIS_OID_GEN_BROADCAST_BYTES_RCV:
        {
            /* The object identifier is used to retreive the number of broadcast bytes rxed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF RX BROADCAST BYTES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.broadcast_bytes_rxed);
            break;
        }
        case RNDIS_OID_GEN_BROADCAST_FRAMES_RCV:
        {
            /* The object identifier is used to retreive the number of broadcast frames rxed without error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF RX BROACAST FRAMES) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(ptr_rndis_mcb->stats.broadcast_pkts_rxed);
            break;
        }
        case RNDIS_OID_GEN_RCV_CRC_ERROR:
        {
            /* The object identifier is used to retreive the number of frames rxed with CRC error. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (NUM OF RX FRAMES WITH CRC ERRORS) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = 0;
            break;
        }
        case RNDIS_OID_GEN_TRANSMIT_QUEUE_LENGTH:
        {
            /* The object identifier is used to retreive the length of transmit queue. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (TX QUEUE LENGTH) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = 0;
            break;
        }
            
        /********************************************************************************
         ********************************* 802.3 Object Ids *****************************
         ********************************************************************************/
        
        case RNDIS_OID_802_3_PERMANENT_ADDRESS:
        {
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (PERMANENT MAC ADDRESS) from host.\n",0,0,0,0,0);
            ptr_rndis_mcb->sys.memcpy((UINT8 *)ptr_reply_buffer,(void *)ptr_rndis_mcb->permanent_mac_address, 6);
            ptr_reply_buffer = (UINT32 *)((UINT8 *)ptr_reply_buffer + 6);
            break;          
        }
        case RNDIS_OID_802_3_CURRENT_ADDRESS:
        {
            /* The object identifier is used to get the MAC address for the network connection
             * at the HOST. */
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (MAC ADDRESS) from host.\n",0,0,0,0,0);
            ptr_rndis_mcb->sys.memcpy((UINT8 *)ptr_reply_buffer,(void *)ptr_rndis_mcb->host_mac_address, 6);            
            ptr_reply_buffer = (UINT32 *)((UINT8 *)ptr_reply_buffer + 6);
            break;
        }       
        case RNDIS_OID_802_3_MULTICAST_LIST:
        {
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (MULTICAST LIST) from host.\n",0,0,0,0,0);

            /* Return the multicast information that is present. */
            ptr_rndis_mcb->sys.memcpy ((void *)ptr_reply_buffer, (void *)&ptr_rndis_mcb->mcast_list[0], 
                                       ptr_rndis_mcb->mcast_list_size);

            ptr_reply_buffer = (UINT32 *)((UINT8 *)ptr_reply_buffer + ptr_rndis_mcb->mcast_list_size);
            break;
        }
        case RNDIS_OID_802_3_MAXIMUM_LIST_SIZE:
        {
            usblogMsg(HAL_SYS_DEBUG,"DBG: QUERY message (MAX MULTICAST LIST SIZE) from host.\n",0,0,0,0,0);
            *ptr_reply_buffer++ = HAL_SYS_SWAPL(HAL_USB_MAX_MAC_MCAST_LIST);
            break;
        }
        
        case RNDIS_OID_802_3_RCV_ERROR_ALIGNMENT:
        case RNDIS_OID_802_3_XMIT_ONE_COLLISION:
        case RNDIS_OID_802_3_XMIT_MORE_COLLISIONS:
        case RNDIS_OID_802_3_XMIT_DEFERRED:
        case RNDIS_OID_802_3_XMIT_MAX_COLLISIONS:
        case RNDIS_OID_802_3_RCV_OVERRUN:
        case RNDIS_OID_802_3_XMIT_UNDERRUN:
        case RNDIS_OID_802_3_XMIT_HEARTBEAT_FAILURE:
        case RNDIS_OID_802_3_XMIT_TIMES_CRS_LOST:
        case RNDIS_OID_802_3_XMIT_LATE_COLLISIONS:
        {
            /* Return 0 for any 802.3 statistics. */
            *ptr_reply_buffer++ = 0;
            break;
        }
        default:
        {
            /* Unsupported OID. */
            usblogMsg(HAL_SYS_FATAL,"ERROR: QUERY message (UNSUPPORTED REQUEST) from host.\n",0,0,0,0,0);
            break;
        }
    }

    /* Compute the length of the response. */
    response_len = (UINT32)((UINT8 *)ptr_reply_buffer -
                            (UINT8 *)((char *)ptr_query_cmplt + sizeof(RNDIS_QUERY_COMPLETE)));
    
    /* Fill up the query complete message. */
    ptr_query_cmplt->RequestId  = p_message->Message.QueryComplete.RequestId;
    ptr_query_cmplt->Status     = HAL_SYS_SWAPL (RNDIS_STATUS_SUCCESS);
    ptr_query_cmplt->InformationBufferLength = HAL_SYS_SWAPL(response_len);
    ptr_query_cmplt->InformationBufferOffset = HAL_SYS_SWAPL(sizeof(RNDIS_QUERY_COMPLETE));

    /* Fill up the response message. */ 
    ptr_response->NdisMessageType = HAL_SYS_SWAPL(REMOTE_NDIS_QUERY_CMPLT);
    ptr_response->MessageLength   = HAL_SYS_SWAPL(RNDIS_MESSAGE_SIZE(RNDIS_QUERY_COMPLETE) + response_len);

    /* Remember the length. */
    p_buffer->len = RNDIS_MESSAGE_SIZE(RNDIS_QUERY_COMPLETE) + response_len;

    /* Schedule the response. */    
    usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->p_response_list, (USB_LIST_NODE *)p_buffer);

    /* Send the NOTIFICATION to the HOST. */
    data[0] = (UINT32)&ptr_rndis_mcb->response_available;
    len [0] = 8;
    hal_usb_dev_send_pipe (ptr_rndis_mcb->intr_in, data, len, 1, 0);

    return; 
}

/**************************************************************************
 * FUNCTION NAME : rndis_process_keep_alive_message
 **************************************************************************
 * DESCRIPTION   :
 *  RNDIS Processing function for the KEEP ALIVE message sent by the HOST.
 *  The function generates a KEEP ALIVE Response message and sends a 
 *  NOTIFICATION message to the host.
 ***************************************************************************/
static void rndis_process_keep_alive_message 
(
    HAL_HND         h_usbapp, 
    RNDIS_MESSAGE*  p_message
)
{
    RNDIS_MESSAGE*              ptr_response;
    RNDIS_RESPONSE*             p_buffer;
    RNDIS_KEEPALIVE_COMPLETE*   ptr_keep_alive_cmplt;
    UINT32                      data[1];
    UINT32                      len[1];
    RNDIS_MCB*                  ptr_rndis_mcb;

    /* Get the RNDIS MCB. */
    ptr_rndis_mcb = (RNDIS_MCB *)h_usbapp;

    /* Get a buffer from the free list. */
    p_buffer = (RNDIS_RESPONSE *)usb_list_remove( (USB_LIST_NODE**)&ptr_rndis_mcb->p_free_list);
    if (p_buffer == NULL)
    {
        usblogMsg (HAL_SYS_FATAL, "Error: No free control buffers to send a response.\n",0,0,0,0,0);
        return;
    }

    /* Create a response message. */    
    ptr_response = (RNDIS_MESSAGE *) &p_buffer->data[0];
    ptr_keep_alive_cmplt = (RNDIS_KEEPALIVE_COMPLETE *)&(ptr_response->Message);

    /* Fill up the KEEP Alive complete message. */
    ptr_keep_alive_cmplt->RequestId = p_message->Message.KeepaliveRequest.RequestId;
    ptr_keep_alive_cmplt->Status    = HAL_SYS_SWAPL(RNDIS_STATUS_SUCCESS);
    
    /* Fill up the response message. */
    ptr_response->NdisMessageType   = HAL_SYS_SWAPL(REMOTE_NDIS_KEEPALIVE_CMPLT);
    ptr_response->MessageLength     = HAL_SYS_SWAPL(RNDIS_MESSAGE_SIZE(RNDIS_KEEPALIVE_COMPLETE));

    /* Remember the length. */
    p_buffer->len = RNDIS_MESSAGE_SIZE(RNDIS_KEEPALIVE_COMPLETE);

    /* Schedule the response. */    
    usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->p_response_list, (USB_LIST_NODE *)p_buffer);

    /* Send the NOTIFICATION to the HOST. */
    data[0] = (UINT32)&ptr_rndis_mcb->response_available;
    len [0] = 8;
    if (hal_usb_dev_send_pipe (ptr_rndis_mcb->intr_in, data, len, 1, 0) < 0)
        usblogMsg (HAL_SYS_FATAL, "Error: Unable to send keep alive message.\n",0,0,0,0,0);

    return;
}

/**************************************************************************
 * FUNCTION NAME : rndis_process_set_message
 **************************************************************************
 * DESCRIPTION   :
 *  RNDIS Processing function for the SET message sent by the HOST. The 
 *  function determines the parameter that needs to be SET and take appropriate
 *  action by generating a SET Complete Response message and sending a  
 *  NOTIFICATION message to the host.
 ***************************************************************************/
static void rndis_process_set_message 
(
    HAL_HND         h_usbapp, 
    RNDIS_MESSAGE*  p_message
)
{
    RNDIS_MESSAGE*              ptr_response;
    RNDIS_RESPONSE*             p_buffer;
    RNDIS_SET_COMPLETE*         ptr_set_cmplt;
    UINT32                      data[1];
    UINT32                      len[1];
    char*                       ptr_info_buffer;
    UINT32                      buffer_offset;
    RNDIS_STATUS                status = RNDIS_STATUS_SUCCESS;
    RNDIS_MCB*                  ptr_rndis_mcb;
    RNDIS_OID_GEN_CONFIG_PARAM* ptr_config;
    UINT16                      parameter_name[20];
    UINT16                      length;
    int                         index;

    /* Get the RNDIS MCB. */
    ptr_rndis_mcb = (RNDIS_MCB *)h_usbapp;
            
    /* Get a buffer from the free list. */
    p_buffer = (RNDIS_RESPONSE *)usb_list_remove( (USB_LIST_NODE**)&ptr_rndis_mcb->p_free_list);
    if (p_buffer == NULL)
    {
        usblogMsg (HAL_SYS_FATAL, "Error: No free control buffers to send a response.\n",0,0,0,0,0);
        return;
    }

    /* Get the INFORMATION buffer that has been passed by the HOST. */
    buffer_offset   = HAL_SYS_SWAPL (p_message->Message.SetRequest.InformationBufferOffset);
    ptr_info_buffer = (char *)&p_message->Message.SetRequest + buffer_offset;

    /* Processing of the SET Request message. */
    switch (HAL_SYS_SWAPL (p_message->Message.SetRequest.Oid) )
    {
        case RNDIS_OID_GEN_CURRENT_PACKET_FILTER:
        {
            /* The HOST has sent a packet filter. If it non-zero we move to the data-initialized state.*/               
            UINT32  packet_filter = HAL_SYS_SWAPL (*(UINT32 *)ptr_info_buffer);
            if (packet_filter != 0x0)
            {
                /* Received a valid packet filter. Store it. */
                ptr_rndis_mcb->packet_filter = packet_filter;

                /* The RNDIS stack is now in the data initialized state. */
                ptr_rndis_mcb->state = RNDIS_DATA_INITIALIZED;
                USB_LOG(USB20_CONTROL,"\n\t======================================================\n",0,0,0,0);
	        USB_LOG(USB20_CONTROL,"\t      USB 2.0 Link Up. RNDIS Driver Initialized. \n",0,0,0,0);
                USB_LOG(USB20_CONTROL,"\t======================================================\n",0,0,0,0);
                /* Log Debug messages.*/
                if (packet_filter & NDIS_PACKET_TYPE_DIRECTED)
                    usblogMsg (HAL_SYS_DEBUG, "DBG: Directed filter.\n",0,0,0,0,0);
                else
                    usblogMsg (HAL_SYS_DEBUG, "DBG: SET message Received valid PACKET filter.\n",0,0,0,0,0);
            }
            break;
        }
        case RNDIS_OID_802_3_MULTICAST_LIST:
        {
            /* Get the Information Buffer length that has been passed. */
            UINT32  buffer_length = HAL_SYS_SWAPL(p_message->Message.SetRequest.InformationBufferLength);

            /* Data has been passed in the Multicast */
            if (buffer_length != 0)
            {
                /* Store the RNDIS Multicast list passed by the HOST. */
                ptr_rndis_mcb->sys.memcpy ((void *)&ptr_rndis_mcb->mcast_list[0], 
                                           (void *)ptr_info_buffer, buffer_length);
                /* Log Debug messages. */
                usblogMsg (HAL_SYS_DEBUG, "DBG: SET Multicast List request received.\n",0,0,0,0,0);       
            }
            else
            {
                /* Empty Multicast list received. */
                usblogMsg (HAL_SYS_DEBUG, "DBG: EMPTY Multicast List request received.\n",0,0,0,0,0);

                /* Clear Multicast list information. */
                (HAL_SYS_MEMSET_RTN)(ptr_rndis_mcb->sys.memset) ((void *)&ptr_rndis_mcb->mcast_list[0], 0, 
                                           sizeof (ptr_rndis_mcb->mcast_list));
            }

            /* Remember the size of the list. */
            ptr_rndis_mcb->mcast_list_size = buffer_length;
            
            break;
        }
        case OID_GEN_RNDIS_CONFIG_PARAMETER:
        {
            /* HOST has sent information to change the device parameters. Decode it
             * and take appropriate action. */
            ptr_config = (RNDIS_OID_GEN_CONFIG_PARAM *)ptr_info_buffer;

            /* Use the Parameter Name offset to skip to the name. */
            ptr_info_buffer = ptr_info_buffer + HAL_SYS_SWAPL(ptr_config->ParameterNameOffset);

            /* The ONLY parameter that we support as of now is the NETWORK Address. Change 
             * appropriately. */
            convert_string_to_unicode ("NetworkAddress",&parameter_name[0], &length);

            /* Make sure that the length is the same. */
            if (length == HAL_SYS_SWAPL(ptr_config->ParameterNameLength))
            {
                /* Compare the 2 unicode strings. */
                if (ptr_rndis_mcb->sys.memcmp((void *)parameter_name, (void *)ptr_info_buffer, 
                                              length) == 0)
                {
                    /* Get the Paramater value length. */
                    length = HAL_SYS_SWAPL(ptr_config->ParameterValueLength);
                    
                    /* Get the start of the CONFIG buffer. */
                    ptr_info_buffer = (char *)ptr_config;

                    /* Use the Value offset to get the parameter value. */
                    ptr_info_buffer = ptr_info_buffer + HAL_SYS_SWAPL(ptr_config->ParameterValueOffset);

                    /* Cycle through the value and store it. */
                    index = 0;
                    while (length != 0x0)
                    {
                        UINT8       first;
                        UINT8       second;
                        UINT16      value;
                        UINT16*     ptr_unicode;

                        /* Get the first nibble. */
                        ptr_unicode = (UINT16 *)ptr_info_buffer;
                        value = HAL_SYS_SWAP (*ptr_unicode);
                        if ( (value >= '0') && (value <= '9') )
                        {
                            first = value - '0';
                        }
                        else
                        {
                            if ( (value >= 'A') && (value <= 'F') )
                            {                                   
                                first = value - '7';
                            }
                            else
                            {
                                if ( (value >= 'a') && (value <= 'f') )
                                {
                                    first = value - 'W';
                                }
                                else
                                {
                                    status = RNDIS_STATUS_FAILURE;
                                    usblogMsg (HAL_SYS_FATAL, "ERROR: Incorrect MAC Address from HOST.\n",0,0,0,0,0);
                                    break;
                                }
                            }
                        }
                        length = length - 2;

                        /* Get the second nibble. */
                        ptr_info_buffer = ptr_info_buffer + 2;
                        ptr_unicode = (UINT16 *)ptr_info_buffer;
                        value = HAL_SYS_SWAP (*ptr_unicode);
                        if ( (value >= '0') && (value <= '9') )
                        {
                            second = value - '0';
                        }
                        else
                        {
                            if ( (value >= 'A') && (value <= 'F') )
                            {                                   
                                second = value - '7';
                            }
                            else
                            {
                                if ( (value >= 'a') && (value <= 'f') )
                                {
                                    second = value - 'W';
                                }
                                else
                                {
                                    status = RNDIS_STATUS_FAILURE;
                                    usblogMsg (HAL_SYS_FATAL, "ERROR: Incorrect MAC Address from HOST.\n",0,0,0,0,0);
                                    break;
                                }
                            }
                        }
                        length = length - 2;
                        
                        /* Combine the nibbles to get the byte mac address. */
                        ptr_rndis_mcb->host_mac_address[index] = (first << 4) | second;
                        index = index + 1;

                        /* Increment the Infobuffer to accomodate the bytes processed. */
                        ptr_info_buffer = ptr_info_buffer + 2;
                    }
                    usblogMsg (HAL_SYS_DEBUG, "DBG: New MAC Address 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:",
                               ptr_rndis_mcb->host_mac_address[0], ptr_rndis_mcb->host_mac_address[1],
                               ptr_rndis_mcb->host_mac_address[2], ptr_rndis_mcb->host_mac_address[3],
                               ptr_rndis_mcb->host_mac_address[4]);
                    usblogMsg (HAL_SYS_DEBUG, "0x%02x\n", ptr_rndis_mcb->host_mac_address[5], 0,0,0,0);
                }
                else
                {
                    /* The RNDIS does NOT support this parameter, give this to the platform
                     * code to process. */
                    if (hal_usb_pform_config_parameter (ptr_config) == 0)
                    {
                        /* The Platform specific was unable to process the CONFIG parameter.
                         * Return an Error back to the HOST. */
                        status = RNDIS_STATUS_FAILURE;
                    }
                }
            }
            else
            {
                if (hal_usb_pform_config_parameter (ptr_config) == 0)
                {
                    /* The Platform specific was unable to process the CONFIG parameter.
                     * Return an Error back to the HOST. */
                    status = RNDIS_STATUS_FAILURE;                  
                }
            }
            break;
        }
        default:
        {
            usblogMsg (HAL_SYS_FATAL, "ERROR: Received OID for SET :%x.\n", 
                       HAL_SYS_SWAPL (p_message->Message.SetRequest.Oid),0,0,0,0);
            status = RNDIS_STATUS_FAILURE;
        }
    }

    /* Create a response message. */    
    ptr_response = (RNDIS_MESSAGE *)&p_buffer->data[0];

    /* Get the pointer to the SET complete message. */
    ptr_set_cmplt = (RNDIS_SET_COMPLETE *)&(ptr_response->Message);

    /* Fill up the SET complete message. */
    ptr_set_cmplt->RequestId    = p_message->Message.KeepaliveRequest.RequestId;
    ptr_set_cmplt->Status       = HAL_SYS_SWAPL(status);

    /* Fill up the response message. */
    ptr_response->NdisMessageType   = HAL_SYS_SWAPL(REMOTE_NDIS_SET_CMPLT);
    ptr_response->MessageLength     = HAL_SYS_SWAPL(RNDIS_MESSAGE_SIZE(RNDIS_SET_COMPLETE));

    /* Remember the length. */
    p_buffer->len = RNDIS_MESSAGE_SIZE(RNDIS_SET_COMPLETE);

    /* Schedule the response. */    
    usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->p_response_list, (USB_LIST_NODE *)p_buffer);

    /* Send the NOTIFICATION to the HOST. */
    data[0] = (UINT32)&ptr_rndis_mcb->response_available;
    len [0] = 8;
    hal_usb_dev_send_pipe (ptr_rndis_mcb->intr_in, data, len, 1, 0);
    return;
}

/**************************************************************************
 * FUNCTION NAME : rndis_process_reset_message
 **************************************************************************
 * DESCRIPTION   :
 *  RNDIS Processing function for the RESET message sent by the HOST. The
 *  function generates a RESET complete message and sends a NOTIFICATION to
 *  the HOST.
 ***************************************************************************/
static void rndis_process_reset_message 
(
    HAL_HND         h_usbapp, 
    RNDIS_MESSAGE*  p_message
)
{
    RNDIS_MESSAGE*              ptr_response;
    RNDIS_RESPONSE*             p_buffer;
    RNDIS_RESET_COMPLETE*       ptr_reset_cmplt;
    UINT32                      data[1];
    UINT32                      len[1];
    RNDIS_MCB*                  ptr_rndis_mcb;

    /* Get the RNDIS MCB. */
    ptr_rndis_mcb = (RNDIS_MCB *)h_usbapp;

    /* Get a buffer from the free list. */
    p_buffer = (RNDIS_RESPONSE *)usb_list_remove( (USB_LIST_NODE**)&ptr_rndis_mcb->p_free_list);
    if (p_buffer == NULL)
    {
        usblogMsg (HAL_SYS_FATAL, "Error: No free control buffers to send a response.\n",0,0,0,0,0);
        return;
    }

    /* Create a response message. */    
    ptr_response = (RNDIS_MESSAGE *)&p_buffer->data[0];

    /* Get the pointer to the RESET complete message. */
    ptr_reset_cmplt = (RNDIS_RESET_COMPLETE *)&(ptr_response->Message);

    /* Fill up the RESET complete message. */
    ptr_reset_cmplt->Status             = HAL_SYS_SWAPL(RNDIS_STATUS_SUCCESS);
    ptr_reset_cmplt->AddressingReset    = 0;

    /* Fill up the response message. */
    ptr_response->NdisMessageType   = HAL_SYS_SWAPL(REMOTE_NDIS_RESET_CMPLT);
    ptr_response->MessageLength     = HAL_SYS_SWAPL(RNDIS_MESSAGE_SIZE(RNDIS_RESET_COMPLETE));

    /* Remember the length. */
    p_buffer->len = RNDIS_MESSAGE_SIZE(RNDIS_RESET_COMPLETE);

    /* Schedule the response. */    
    usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->p_response_list, (USB_LIST_NODE *)p_buffer);

    /* Send the NOTIFICATION to the HOST. */
    data[0] = (UINT32)&ptr_rndis_mcb->response_available;
    len [0] = 8;
    if (hal_usb_dev_send_pipe (ptr_rndis_mcb->intr_in, data, len, 1, 0) < 0)
        usblogMsg (HAL_SYS_FATAL, "Error: Unable to send reset message.\n",0,0,0,0,0);

    /* Log Debug messages */
    usblogMsg(HAL_SYS_DEBUG, "DBG: RESET_COMPLETE message to host.\n",0,0,0,0,0);
    return;
}

/**************************************************************************
 * FUNCTION NAME : rndis_process_halt_message
 **************************************************************************
 * DESCRIPTION   :
 *  RNDIS Processing function for the HALT Message. There is no need to 
 *  generate a response for this message. The function layer resets itself.
 ***************************************************************************/
static void rndis_process_halt_message (HAL_HND h_usbapp, RNDIS_MESSAGE *p_message)
{
    RNDIS_MCB*  ptr_rndis_mcb;

    /* Get the RNDIS MCB. */
    ptr_rndis_mcb = (RNDIS_MCB *)h_usbapp;
        
    /* Reset the application layer. */
    rndis_usb_reset (ptr_rndis_mcb);

    /* LOG Debug messages. */
    usblogMsg(HAL_SYS_DEBUG, "DBG: HALT message from host. Setting UNINITIALIZED state.\n",0,0,0,0,0);
    return;
}

/**************************************************************************
 * FUNCTION NAME : rndis_app_notify_link_state
 **************************************************************************
 * DESCRIPTION   :
 *  The function informs the HOST about the status of the LINK through a 
 *  LINK_STATUS message. 
 ***************************************************************************/
void rndis_app_notify_link_state (HAL_HND handle, int link_state)
{
    RNDIS_MCB*                  ptr_rndis_mcb;
    RNDIS_MESSAGE*              ptr_response;
    RNDIS_RESPONSE*             p_buffer;
    RNDIS_INDICATE_STATUS*      ptr_indicate_status_msg;
    UINT32                      data[1];
    UINT32                      len[1];

    /* Get the pointer to the RNDIS Master control block. */
    ptr_rndis_mcb = (RNDIS_MCB *)handle;

    /* Get a buffer from the free list. */
    p_buffer = (RNDIS_RESPONSE *)usb_list_remove( (USB_LIST_NODE**)&ptr_rndis_mcb->p_free_list);
    if (p_buffer == NULL)
    {
        usblogMsg (HAL_SYS_FATAL, "Error: No free control buffers to send a response.\n",0,0,0,0,0);
        return;
    }

    /* Create a response message. */    
    ptr_response = (RNDIS_MESSAGE *)&p_buffer->data[0];
        
    /* Get a pointer to the RNDIS Indicate status message. */
    ptr_indicate_status_msg = (RNDIS_INDICATE_STATUS *)&(ptr_response->Message);

    /* Fill up the indicate status message. */
    if (link_state == 1)
    {
        /* The Media connection is UP. */
        ptr_indicate_status_msg->Status = HAL_SYS_SWAPL(RNDIS_STATUS_MEDIA_CONNECT); 
    }
    else
    {
        /* The Media connection is DOWN. */
        ptr_indicate_status_msg->Status = HAL_SYS_SWAPL(RNDIS_STATUS_MEDIA_DISCONNECT);
    }
    ptr_indicate_status_msg->StatusBufferLength = 0x0;
    ptr_indicate_status_msg->StatusBufferOffset = 0x0;

    /* Fill Up the Response Message. */
    ptr_response->NdisMessageType   = HAL_SYS_SWAPL (REMOTE_NDIS_INDICATE_STATUS_MSG);
    ptr_response->MessageLength     = HAL_SYS_SWAPL (RNDIS_MESSAGE_SIZE(RNDIS_INDICATE_STATUS));

    /* Remember the length. */
    p_buffer->len = RNDIS_MESSAGE_SIZE(RNDIS_INDICATE_STATUS);

    /* Schedule the response. */    
    usb_list_add ((USB_LIST_NODE **)&ptr_rndis_mcb->p_response_list, (USB_LIST_NODE *)p_buffer);

    /* Send the NOTIFICATION to the HOST. */
    data[0] = (UINT32)&ptr_rndis_mcb->response_available;
    len [0] = 8;
    hal_usb_dev_send_pipe (ptr_rndis_mcb->intr_in, data, len, 1, 0);

    /* Log Debug messages */
    usblogMsg(HAL_SYS_DEBUG, "DBG: INDICATE status message with state:%d sent to host.\n", link_state,0,0,0,0);
    return;
}

/**************************************************************************
 * FUNCTION NAME : rndis_usb_get_statistics
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called from the upper layer to request the RNDIS layer
 *  for the statistics. 
 *  
 * RETURNS       :
 *  RNDIS statistics structure.
 ***************************************************************************/
RNDIS_STATS* rndis_usb_get_statistics (HAL_HND h_usbapp)
{
    RNDIS_MCB*  ptr_rndis_mcb;

    /* Get the RNDIS MCB. */
    ptr_rndis_mcb = (RNDIS_MCB *)h_usbapp;
        
    return &(ptr_rndis_mcb->stats);
}

/***************************** UTILITY Functions *************************/

/**************************************************************************
 * FUNCTION NAME : convert_string_to_unicode
 **************************************************************************
 * DESCRIPTION   :
 *  Utility function that converts ASCII Strings to UNICODE strings.
 ***************************************************************************/
static void convert_string_to_unicode (char *string, UINT16 *unicode_string, UINT16 *length)
{
    UINT32  index;
    
    /* The UNICODE string will be twice the length of the ANSI string. */
    *length = 2 * strlen (string); 

    /* Convert the ANSI String to UNICODE format. */
    for (index = 0; index < strlen (string); index++)
        unicode_string[index] = HAL_SYS_SWAP((UINT16)(string[index]));
}

/**************************************************************************
 * FUNCTION NAME : rndis_find_str_descriptor
 **************************************************************************
 * DESCRIPTION   :
 *  The function is used to find a string descriptor with the specified index
 *
 * RETURNS       :
 *  NULL                -   If no string descriptor was found.
 *  String Descriptor   -   If a valid string descriptor was found.
 ***************************************************************************/
static HAL_USB_STRING_ENTRY* rndis_find_str_descriptor 
(
    RNDIS_MCB*  ptr_rndis_mcb, 
    UINT8       index
)
{
    HAL_USB_STRING_ENTRY* p_string;
    HAL_USB_STRING_ENTRY* p_head;

    /* Get the head of the string descriptor list. */
    p_head = (HAL_USB_STRING_ENTRY *)usb_list_get_head ((USB_LIST_NODE **)&ptr_rndis_mcb->ptr_string_desc);

    /* Remember the head. */
    p_string = p_head;

    /* Traverse the list searching for the string descriptor. */
    while (1)
    {
        /* Do we have a HIT ? */
        if (p_string->index == index)
            return p_string;

        /* Jump to the next node in the list. */
        p_string = (HAL_USB_STRING_ENTRY *)usb_list_get_next ((USB_LIST_NODE *)p_string);

        /* Since the linked list is a doubly linked circular list. The tail will point 
         * back to the head. Check if we have looped back. */       
        if (p_head == p_string)
        {
            /* YES we have looped back. There was no matching string descriptor found. */
            return NULL;
        }
    }
}


void rndis_initialize_app_strings(HAL_SYS_TBL * ptr_sys_tbl)
{
     	usb_app_strings = (char **)ptr_sys_tbl->malloc(6 * sizeof(char *));

	usb_app_strings[0]=(char *) ptr_sys_tbl->malloc(MAX_UNICODE_LENGTH/2);
     	(HAL_SYS_MEMCPY_RTN)(ptr_sys_tbl->memcpy)(usb_app_strings[0],USB_RNDIS_MANUFACTURER_DESC_STRING,strlen(USB_RNDIS_MANUFACTURER_DESC_STRING)+1);
	 
     	usb_app_strings[1]=(char *) ptr_sys_tbl->malloc(MAX_UNICODE_LENGTH/2);
     	(HAL_SYS_MEMCPY_RTN)(ptr_sys_tbl->memcpy)(usb_app_strings[1],USB_RNDIS_PRODUCT_DESC_STRING,strlen(USB_RNDIS_PRODUCT_DESC_STRING)+1);
	  
	usb_app_strings[2]=(char *) ptr_sys_tbl->malloc(MAX_UNICODE_LENGTH/2);
      	(HAL_SYS_MEMCPY_RTN)(ptr_sys_tbl->memcpy)(usb_app_strings[2],USB_RNDIS_SERIAL_NO_STRING,strlen(USB_RNDIS_SERIAL_NO_STRING)+1);
	  
	usb_app_strings[3]=(char *) ptr_sys_tbl->malloc(strlen(USB_RNDIS_COMM_INTF_STRING)+1);
     	(HAL_SYS_MEMCPY_RTN)(ptr_sys_tbl->memcpy)(usb_app_strings[3],USB_RNDIS_COMM_INTF_STRING,strlen(USB_RNDIS_COMM_INTF_STRING)+1);
	 
	usb_app_strings[4]=(char *) ptr_sys_tbl->malloc(strlen(USB_RNDIS_DATA_INTF_STRING)+1);
      	(HAL_SYS_MEMCPY_RTN)(ptr_sys_tbl->memcpy)(usb_app_strings[4],USB_RNDIS_DATA_INTF_STRING,strlen(USB_RNDIS_DATA_INTF_STRING)+1);
	  
	usb_app_strings[5]=NULL;
 
}

#ifdef USB_MIB2

/**************** MIB2 support *****************************************/

#define USB_CDC_PACKET_TYPE_PROMISCUOUS     0x01
#define USB_CDC_PACKET_TYPE_ALL_MULTICAST   0x02
#define USB_CDC_PACKET_TYPE_DIRECTED        0x04
#define USB_CDC_PACKET_TYPE_BROADCAST       0x08
#define USB_CDC_PACKET_TYPE_MULTICAST       0x10
#if 0 
UINT32 device_suspend_state = TRUE;

int rndis_get_device_status(RNDIS_MCB * ptr_rndis_mcb)
{
   if(device_suspend_state)
   {
        if(ptr_rndis_mcb->state == RNDIS_UNINITIALIZED)
        {
            return MIB2_USB_STATUS_UNATTACHED;
        }
        else 
        {
            return MIB2_USB_STATUS_SUSPENDED;
        }
   }
   else
   {
        return MIB2_USB_STATUS_CONFIGURED;
   }

}
#endif
int rndis_usb_get_mib_stats(RNDIS_MCB * ptr_rndis_mcb, int command, void * data)
{
    USB_MIB2_USB_PARAMS    *ptr_usb_param;
    USB_MIB2_PHY_COUNTERS  *ptr_phy_cntr;
    USB_MIB2_ETH_PARAMS    *ptr_eth_param;
    USB_MIB2_IF_COUNTERS   *ptr_if_cntr;
	USB_MIB2_IF_PARAMS     *ptr_if_param;
    RNDIS_STATS            *ptr_rndis_stats;
    USB_STATS   *ptr_usb_stats;
    UINT32 temp_uint32;
    int error_status=0;
        

    switch(command)
    {
        case SIOCGINTFCOUNTERS:
            {
            ptr_rndis_stats = &(ptr_rndis_mcb->stats);
            ptr_if_cntr = (USB_MIB2_IF_COUNTERS *) data;
            memset ((void*)ptr_if_cntr,0,sizeof(USB_MIB2_IF_COUNTERS));

            ptr_if_cntr->inBytesLow = ptr_rndis_stats->unicast_bytes_rxed +
                                      ptr_rndis_stats->broadcast_bytes_rxed +
                                      ptr_rndis_stats->multicast_bytes_rxed ;
                                      
            ptr_if_cntr->inUnicastPktsLow = ptr_rndis_stats->unicast_pkts_rxed;
            ptr_if_cntr->inBroadcastPktsLow = ptr_rndis_stats->broadcast_pkts_rxed;
            ptr_if_cntr->inMulticastPktsLow = ptr_rndis_stats->multicast_pkts_rxed;
            ptr_if_cntr->inDiscardPkts = ptr_rndis_stats->receive_packets_dropped;

            ptr_if_cntr->outBytesLow = ptr_rndis_stats->unicast_bytes_txed +
                                      ptr_rndis_stats->broadcast_bytes_txed +
                                      ptr_rndis_stats->multicast_bytes_txed ;
                                      
            ptr_if_cntr->outUnicastPktsLow = ptr_rndis_stats->unicast_pkts_txed;
            ptr_if_cntr->outBroadcastPktsLow = ptr_rndis_stats->broadcast_pkts_txed;
            ptr_if_cntr->outMulticastPktsLow = ptr_rndis_stats->multicast_pkts_txed;
            ptr_if_cntr->outDiscardPkts = ptr_rndis_stats->transmit_packets_dropped;

            break;
            }

        case SIOCGUSBPARAMS:
        {
   			USB_DEV * usbdev = usb_drv_get_dev();
            ptr_usb_param = (USB_MIB2_USB_PARAMS *)data;

            ptr_usb_param->usbDevicePower = (((ptr_rndis_mcb->ptr_config_desc->cfg.bmAttributes)&0x40)
                                                ? MIB2_USB_POWER_SELF : MIB2_USB_POWER_BUS);
            ptr_usb_param->usbDeviceVendorID = HAL_SYS_SWAP(ptr_rndis_mcb->ptr_device_desc->idVendor);
            ptr_usb_param->usbDeviceProductID = HAL_SYS_SWAP(ptr_rndis_mcb->ptr_device_desc->idProduct);
            ptr_usb_param->usbDeviceNumberConfigurations = ptr_rndis_mcb->ptr_device_desc->bNumConfigurations;
            ptr_usb_param->usbDeviceActiveClass = MIB2_USB_ACTIVE_CLASS_CDC;

            ptr_usb_param->usbDeviceStatus = ((usbdev->link_status == 1) ? MIB2_USB_STATUS_UNATTACHED : MIB2_USB_STATUS_CONFIGURED);

            usb_drv_ioctl(ptr_rndis_mcb->h_device, IOCTL_GET_USB_STATS, (void *)&ptr_usb_stats);
            ptr_usb_param->usbDeviceNumEnumCounter = ptr_usb_stats->EnumCount;

            /* We don't support Remote Wakeup */
            ptr_usb_param->usbDeviceRemoteWakeup = MIB2_TRUTH_VALUE_FALSE;
            ptr_usb_param->usbDeviceRemoteWakeupOn = MIB2_TRUTH_VALUE_FALSE;

            ptr_usb_param->usbCDCSubclass = MIB2_USB_SUBCLASS_ACM; /* Abstract Control Model */
            /* Since we use BULK endpoints for data transfer, Tx type is Async */
            ptr_usb_param->usbCDCDataTransferType = MIB2_USB_TX_TYPE_ASYNC; 
            ptr_usb_param->usbCDCEndpoints = ptr_rndis_mcb->ptr_config_desc->data_iface.bNumEndpoints;
            ptr_usb_param->usbCDCStalls = 0; /* Not supported at present */
            /* Packet filter */
            ptr_usb_param->usbCDCEtherPacketFilterLength = 1; 
            ptr_usb_param->usbCDCEtherPacketFilter[0]= 
                            ((ptr_rndis_mcb->packet_filter & NDIS_PACKET_TYPE_PROMISCUOUS)?USB_CDC_PACKET_TYPE_PROMISCUOUS:0x00) 
                        |   ((ptr_rndis_mcb->packet_filter & NDIS_PACKET_TYPE_ALL_MULTICAST)?USB_CDC_PACKET_TYPE_ALL_MULTICAST:0x00)
                        |   ((ptr_rndis_mcb->packet_filter & NDIS_PACKET_TYPE_DIRECTED)?USB_CDC_PACKET_TYPE_DIRECTED:0x00)
                        |   ((ptr_rndis_mcb->packet_filter & NDIS_PACKET_TYPE_BROADCAST)?USB_CDC_PACKET_TYPE_BROADCAST:0x00)
                        |   ((ptr_rndis_mcb->packet_filter & NDIS_PACKET_TYPE_MULTICAST)?USB_CDC_PACKET_TYPE_MULTICAST:0x00);

            /* Stats Capabilities -- Using RNDIS capabilities*/
            temp_uint32 = 0x1FFFFFE;
            ptr_usb_param->usbCDCEtherStatCapabilitiesLength = 4;
            ptr_usb_param->usbCDCEtherStatCapabilities[0]= (char) ((HAL_SYS_SWAPL(temp_uint32)) & 0x000000FF) ;
            ptr_usb_param->usbCDCEtherStatCapabilities[1]= (char)(((HAL_SYS_SWAPL(temp_uint32)) & 0x0000FF00) >>  8);
            ptr_usb_param->usbCDCEtherStatCapabilities[2]= (char)(((HAL_SYS_SWAPL(temp_uint32)) & 0x00FF0000) >>  16);
            ptr_usb_param->usbCDCEtherStatCapabilities[3]= (char)(((HAL_SYS_SWAPL(temp_uint32)) & 0xFF000000) >> 24);
            
                        
            ptr_usb_param->usbCDCVersion = HAL_SYS_SWAP(ptr_rndis_mcb->ptr_config_desc->header_func.bbcdCDC);
            break;

        }

        case SIOCGETHERCOUNTERS:
        case SIOCGUSBCOUNTERS:
        {
            ptr_phy_cntr = (USB_MIB2_PHY_COUNTERS *)data;
            memset((void *) ptr_phy_cntr, 0, sizeof(USB_MIB2_PHY_COUNTERS));
            break;
        }

        case SIOCGETHERPARAMS:
        {
            ptr_eth_param = (USB_MIB2_ETH_PARAMS *)data;
            ptr_eth_param->ethDuplexStatus = MIB2_UNKNOWN_DUPLEX; /* No duplex mode for USB */
            break;
        }

		case SIOCGINTFPARAMS:
		{
				UINT32 temp_uint32;
				ptr_if_param = (USB_MIB2_IF_PARAMS *)data;
            usb_drv_ioctl((USB_DEV *) ptr_rndis_mcb->h_device, IOCTL_GET_USB_SPEED, (void *)&temp_uint32);
				ptr_if_param->ifSpeed = temp_uint32;
				ptr_if_param->ifHighSpeed = temp_uint32/1000000;
				ptr_if_param->ifOperStatus = MIB2_STATUS_UP;
		}
               
        default:
        {
            break;
        }
    }

    return error_status;
}

#endif /*USB_MIB2*/


/**************************************************************************
 * FUNCTION NAME : rndis_usb_reset_statistics
 **************************************************************************
 * DESCRIPTION   :
 * 	The function is called from the upper layer to request the RNDIS layer
 * 	to reset the statistics. 
 * 	
 * RETURNS		 :
 ***************************************************************************/
int rndis_usb_reset_statistics (HAL_HND h_usbapp)
{
	RNDIS_MCB*	ptr_rndis_mcb;

	/* Get the RNDIS MCB. */
	ptr_rndis_mcb = (RNDIS_MCB *)h_usbapp;

	(HAL_SYS_MEMSET_RTN)(ptr_rndis_mcb->sys.memset)((void*)&(ptr_rndis_mcb->stats),0,sizeof(RNDIS_STATS));
		
	return 0; 
}


/**************************************************************************
 * FUNCTION NAME : rndis_usb_get_host_mac_address
 **************************************************************************
 * DESCRIPTION   :
 * 	The function is called from the upper layer to request the RNDIS layer
 * 	for the PC mac address. 
 * 	
 * RETURNS		 :
 ***************************************************************************/
int rndis_usb_get_host_mac_address(HAL_HND h_usbapp, unsigned char e_addr[6])
{
		RNDIS_MCB* ptr_rndis_mcb;

		/* Get the RNDIS MCB */
		ptr_rndis_mcb = (RNDIS_MCB *)h_usbapp;

		ptr_rndis_mcb->sys.memcpy(e_addr,ptr_rndis_mcb->host_mac_address,6);
		return 0;
		
}


