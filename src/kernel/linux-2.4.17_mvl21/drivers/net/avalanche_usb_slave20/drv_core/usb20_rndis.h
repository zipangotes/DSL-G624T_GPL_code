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
 * FILE PURPOSE :   USB Function layer related structures.
 **************************************************************************
 * FILE NAME    :   usbs_rndis.h
 *
 * DESCRIPTION  :
 *  Configurable definations related to the RNDIS USB Protocol layer. These 
 *  are provided as a template and could be changed by the user to suit 
 *  their own needs.
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/

#ifndef __RNDIS_USB_H__
#define __RNDIS_USB_H__

#include "rndis.h"
#include "usb20_drvrndis.h"
#include "usb20_config.h"



#define USB_RNDIS_MANUFACTURER_DESC_STRING "Texas Instruments"
#define USB_RNDIS_PRODUCT_DESC_STRING            "TI USB RNDIS Adapter"
#define USB_RNDIS_SERIAL_NO_STRING                   "TI USB 001 "
#define USB_RNDIS_COMM_INTF_STRING                  "RNDIS Communication Interface"
#define USB_RNDIS_DATA_INTF_STRING                  "RNDIS Data Interface "


/* NOTE: This is a RNDIS Specific DEFINATION which should have been present in the RNDIS.H
 * file that was provided by Microsoft. For some reason they do not have this defination
 * so it has placed it in our file, because we dont want to change the RNDIS.H file
 * provided by Microsoft. See Section 3.4.3 of the RNDIS specification for more information */
typedef struct RNDIS_OID_GEN_CONFIG_PARAM
{
    UINT32      ParameterNameOffset;
    UINT32      ParameterNameLength;
    UINT32      ParameterType;
    UINT32      ParameterValueOffset;
    UINT32      ParameterValueLength;
}RNDIS_OID_GEN_CONFIG_PARAM;

#define OID_GEN_RNDIS_CONFIG_PARAMETER      0x0001021B

/*********************** USB Application Definations  *******************/

/* All these need to be filled in / changed by the application layer */

/* Application Programmable defination in the device descriptor. */
#define RNDIS_USB_PRODUCT_ID          USB_PID    /* 0x6060  Product ID                                       */
#define RNDIS_USB_VENDOR_ID            USB_VID    /* 0x0454  Vendor ID  -- Sabya                          */
#define RNDIS_USB_DEV_CODE             0x0000  /* USB Device Release Code.                         */
#define RNDIS_USB_DEV_MANUF_STR    0x1     /* Manufacturer String Index                        */
#define RNDIS_USB_DEV_PROD_STR      0x2     /* Product Description String Index                 */
#define RNDIS_USB_DEV_SNUM_STR      0x0     /* Serial Number String Index                       */

/* Communication Class Interface */
#define RNDIS_USB_COMM_IFACE_STRING 0x4     /* String Description Index for this interface.     */

/* Second interface description. */
#define RNDIS_USB_IFACE2_STRING     0x5     /* String description Index for this interface.     */

/* Interrupt Endpoint polling interval. */
#define RNDIS_USB_INT_POLL_TIME     0x01

/* Maximum packet size (Data) that can be transmitted by the application. */
#define RNDIS_USB_MTU               1500

/* Maximum data size (Data + header) that can be transmitted by an application. */
#define RNDIS_USB_MSS               (RNDIS_USB_MTU + 6 + 6 + 2)

/* Maximum number of MULTICAST elements in the list. */
#define HAL_USB_MAX_MAC_MCAST_LIST          32

/* Number of languages supported by the application. */
#define RNDIS_USB_NUM_OF_LANG_SUPPORT       0x1

/* Application defined max unicode length. If application wants to define
 * larger string change this constant also. */
#define RNDIS_USB_MAX_UNICODE_LENGTH        256



/*********************** Application Specific Structures *******************/

/* CONTROL Buffer Definations.  */
#define RNDIS_CONTROL_MAX_BUFFERS   5       /* Max. Number of control buffer.*/
#define RNDIS_MAX_BUFFER_SIZE       512     /* Max. size of a control buffer.*/

/* BULK Buffer Definations.     */
#define RNDIS_BULK_MAX_BUFFERS      256     /* Max. Number of BULK buffers.  */

/* Textual description of the device. */
#define RNDIS_USB_VENDOR_DESC       "Texas Instruments"




typedef struct RNDIS_STATS
{
    /* Transmission counters. */
    UINT32          broadcast_pkts_txed;
    UINT32          broadcast_bytes_txed;
    UINT32          multicast_pkts_txed;
    UINT32          multicast_bytes_txed;
    UINT32          unicast_pkts_txed;
    UINT32          unicast_bytes_txed;
    UINT32          transmit_packets_dropped;
    
    /* Receive counters */
    UINT32          broadcast_pkts_rxed;
    UINT32          broadcast_bytes_rxed;
    UINT32          multicast_pkts_rxed;
    UINT32          multicast_bytes_rxed;
    UINT32          unicast_pkts_rxed;
    UINT32          unicast_bytes_rxed;
    UINT32          receive_packets_dropped;
}RNDIS_STATS;

#ifdef USB_MIB2
/*    for MIB support */
typedef struct mib2_ifCounters USB_MIB2_IF_COUNTERS;
typedef struct mib2_ifParams  USB_MIB2_IF_PARAMS;
typedef struct mib2_ethParams USB_MIB2_ETH_PARAMS;
typedef struct mib2_phyCounters USB_MIB2_PHY_COUNTERS;
typedef struct mib2_usbParams USB_MIB2_USB_PARAMS;
typedef struct mib2_ifCommand USB_MIB2_IF_COMMAND;
#endif


/**************************************************************************
 * STRUCTURE -  RNDIS_USB_CONFIG
 **************************************************************************
 *  This is the RNDIS USB Configuration structure that is specified in the
 *  RNDIS specification to be returned when the HOST asks for the device 
 *  configuration through a GET_CONFIG_DESCRIPTOR request. The CONFIGUARTION
 *  descriptor is as per the RNDIS specification.
 **************************************************************************/
typedef struct
{
    USB_CONFIG_DESCR                cfg;
    USB_INTERFACE_DESCR             comm_iface;
    USB_CDC_HEADER_FUNCT_DESC       header_func;
    USB_CDC_CALL_MGMT_DESC          call_mgmt;
    USB_CDC_ABSTRACT_CONTROL_DESC   abstr_ctrl;
    USB_CDC_UNION_FUNCT_DESC        union_func;
    USB_ENDPOINT_DESCR              interrupt_ep;
    USB_INTERFACE_DESCR             data_iface;
    USB_ENDPOINT_DESCR              bulkIn_ep;
    USB_ENDPOINT_DESCR              bulkOut_ep;
} __attribute__ ((packed)) RNDIS_USB_CONFIG;


/**************************************************************************
 * STRUCTURE -  RNDIS_STATE
 **************************************************************************
 *  The structure defines the valid states that the RNDIS stack can be in as
 *  specified by the RNDIS Specification. 
 **************************************************************************/
typedef enum RNDIS_STATE
{
    RNDIS_UNINITIALIZED         = 0x0,
    RNDIS_INITIALIZED           = 0x1,
    RNDIS_DATA_INITIALIZED      = 0x2,
    RNDIS_SUSPENDED1            = 0x3,
    RNDIS_SUSPENDED2            = 0x4,
    RNDIS_SUSPENDED3            = 0x5
}RNDIS_STATE;

/**************************************************************************
 * STRUCTURE -  RNDIS_RX_STATE
 **************************************************************************
 *  This is used internally by the RNDIS receive state machine. The data 
 *  arrives fragmented over the USB cable and it needs to be reassembled 
 *  before it can be passed to the upper layer. The reassembly for received
 *  data is handled using a state machine, the following structure defines 
 *  the valid states.
 **************************************************************************/
typedef enum RNDIS_RX_STATE
{
    RNDIS_WAIT_FOR_HEADER_STAGE = 0x0,
    RNDIS_DATA_RX_STAGE         = 0x1
}RNDIS_RX_STATE;

/**************************************************************************
 * STRUCTURE -  RNDIS_RESPONSE
 **************************************************************************
 *  The structure is used to respond to RNDIS control messages that arrive
 *  over the default control pipe. 
 **************************************************************************/
typedef struct RNDIS_RESPONSE
{
    USB_LIST_NODE      link;
    char                      data[RNDIS_MAX_BUFFER_SIZE];
    UINT32                  len;
}RNDIS_RESPONSE;

/**************************************************************************
 * STRUCTURE -  RNDIS_HEADER
 **************************************************************************
 *  This structure is used by the RNDIS stack in the data path. When an 
 *  ethernet frame is passed to the RNDIS stack a header needs to be 
 *  prepended to the packet before the data can be transmitted over the USB 
 *  cable. This structure is used to prepend the header. 
 *  
 * NOTES:
 *  The 'chain_id' is the orignal packet identifer that was passed to the
 *  RNDIS stack layer.
 **************************************************************************/
typedef struct RNDIS_HEADER
{
    USB_LIST_NODE               link;
    char                    message[64];
    UINT32                  chain_id;
}RNDIS_HEADER;

/**************************************************************************
 * STRUCTURE -  RNDIS_MCB
 **************************************************************************
 *  The master control block structure for the RNDIS stack layer.
 **************************************************************************/
typedef struct RNDIS_MCB
{
    /********************** USB Descriptor ******************************/
    USB_DEVICE_DESCR *              ptr_device_desc;
    RNDIS_USB_CONFIG *              ptr_config_desc;
    HAL_USB_STRING_ENTRY*           ptr_string_desc;
        
    /********************** HANDLE **************************************/
    HAL_HND         h_device;       /* USB Device handle                */
    HAL_HND         h_sys;          /* Handle to the OS USB Layer.      */
    HAL_HND         h_end;          /* Handle of the END driver.        */
    HAL_HND         ctrl_in;        /* Control IN endpoint handle       */
    HAL_HND         ctrl_out;       /* Control OUT endpoint handle      */
    HAL_HND         intr_in;        /* INTERRUPT endpoint handle        */
    HAL_HND         bulk_in;        /* BULK IN endpoint handle          */
    HAL_HND         bulk_out;       /* BULK OUT endpoint handle         */

    HAL_SYS_TBL     sys;            /* OS Support Layer.                */
    
    /********************* RNDIS Protocol *******************************/
    
    RNDIS_STATE     state;                      /* RNDIS Stack State            */
    UINT8           host_mac_address[6];        /* HOST MAC Address             */
    UINT8           permanent_mac_address[6];   /* Permanent HOST MAC Address   */
    UINT32          packet_filter;              /* PACKET Filter                */
    
    /* These lists handle the responses for control messages. The FREE list is a list 
     * of responses messages that are free and can be used by the function layer. The
     * response lists contains all the responses that have been queued and will be txed
     * to the HOST as and when the HOST will request for them through a GET_ENCAPSULATED
     * message. */
    RNDIS_RESPONSE* p_free_list;
    RNDIS_RESPONSE* p_response_list;

    /* Notification response to be sent to the HOST is always constant. Create it once
     * and store it in the structure. */
    USB_SETUP       response_available;

    /* Receieve State machine -- For BULK Data. */
    RNDIS_RX_STATE  receive_state;
    char*           rx_buffer;
    char*           rx_buffer_ptr;
    UINT16          received_len;
    UINT16          data_payload_len;
    UINT32          receive_id;

    /* Transmit - For BULK data. */
    RNDIS_HEADER*   ptr_bulk_free_list;

    /* Multicast Address. The RNDIS layer needs to keep track of the Multicast 
     * address information that is passed from the HOST. */
    UINT8           mcast_list[HAL_USB_MAX_MAC_MCAST_LIST][6];  /* Array for Multicast address  */
    int             mcast_list_size;                            /* List size in BYTES.          */

    /* Statistics */
    RNDIS_STATS     stats;
}RNDIS_MCB;


#define USB_VERSION_1_1    0x0110
#define USB_VERSION_2_0    0x0200

#define USB20_EP0_MAXPKTSIZE 64

#define USB_LINK_FULL_SPEED  12000000
#define USB_LINK_HI_SPEED   480000000

#endif
