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
 * FILE NAME    :  usb20_def.h  
 *
 * DESCRIPTION  :  USB 2.0 Protocol related definitions
 *
 * HISTORY 	:
 *
 *  Jul 30  -  sabyasachi  -  created
 *  Sep 04  -  sabyasachi  -  added 2.0 specific extensions 
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/

#ifndef __USB20_DEF_H_
#define __USB20_DEF_H_

#include "psp_types.h"


/* USB Version supported is version 2.0 */
#define USB_VERSION                 0x0200

/* 
 * USB 2.0 defined link speeds. 
 *    FULL SPEED -   12 Mbps
 *    HIGH SPEED -  480 Mbps   
 */
#define USB_FULL_LINK_SPEED              12000000
#define USB_HIGH_LINK_SPEED             480000000


/* 
 * Define Various End Point Sizes
 */

#define USB_CTRL_EP_SIZE    64
#define USB_FS_BULK_EP_SIZE   64
#define USB_HS_BULK_EP_SIZE   512  
#define USB_INT_EP_SIZE     8



#define MAX_UNICODE_LENGTH  256 

/* Standard Descriptor Types */
#define USB_DEVICE                  0x01
#define USB_CONFIGURATION           0x02
#define USB_STRING                  0x03
#define USB_INTERFACE               0x04
#define USB_ENDPOINT                0x05
#define USB_DEVICE_QUALIFIER        0x06
#define USB_OTHER_SPEED_CFG         0x07
#define USB_INTERFACE_POWER         0x08

/*************** Endpoint Descriptor Specific Definations. **************/

/* Endpoint Transfer Types. */
#define USB_EP_CONTROL              0x00
#define USB_EP_ISOCHRONOUS          0x01
#define USB_EP_BULK                 0x02
#define USB_EP_INTERRUPT            0x03

/* Direction of endpoint. */
#define USB_EP_IN_DIR               0x80
#define USB_EP_OUT_DIR              0x00

/* Polling interval for the endpoint. */
#define USB_EP_POLL_NOT_USED        0x00

/* Endpoint Address Mask. */
#define USB_EP_ADDR_MASK            0x0F

/**************************** Setup Packet *******************************/

#define USB_SETUP_DIRECTION_MASK    0x80
#define USB_SETUP_HOST_TO_DEVICE    0x00
#define USB_SETUP_DEVICE_TO_HOST    0x80

#define USB_SETUP_TYPE_MASK         0x60
#define USB_SETUP_REQ_STANDARD      0x00
#define USB_SETUP_REQ_CLASS         0x20
#define USB_SETUP_REQ_VENDOR        0x40
#define USB_SETUP_REQ_RESERVED      0x60

#define USB_SETUP_RECEPIENT_MASK    0x1F
#define USB_SETUP_RECP_DEVICE       0x00
#define USB_SETUP_RECP_INTERFACE    0x01
#define USB_SETUP_RECP_ENDPOINT     0x02

/*************************** Standard Request Codes ***********************/

#define USB_GET_STATUS              0x00
#define USB_CLEAR_FEATURE           0x01
#define USB_RESERVED_CODE2          0x02
#define USB_SET_FEATURE             0x03
#define USB_RESERVED_CODE4          0x04
#define USB_SET_ADDRESS             0x05
#define USB_GET_DESCRIPTOR          0x06
#define USB_SET_DESCRIPTOR          0x07
#define USB_GET_CONFIGURATION       0x08
#define USB_SET_CONFIGURATION       0x09
#define USB_GET_INTERFACE           0x0a
#define USB_SET_INTERFACE           0x0b
#define USB_SYNCH_FRAME             0x0c

/* Class Specific Commands. */
#define USB_SEND_ENCAPSULATED_CMD   0x00
#define USB_GET_ENCAPSULATED_RESP   0x01
#define USB_RESPONSE_AVAILABLE      0x00

/*************************** Descriptor Types *****************************/

#define USB_DESCRIPTOR_DEVICE                0x01
#define USB_DESCRIPTOR_CONFIG                0x02
#define USB_DESCRIPTOR_STRING                0x03
#define USB_DESCRIPTOR_IFACE                 0x04
#define USB_DESCRIPTOR_ENDPNT                0x05
#define USB_DESCRIPTOR_DEVICE_QUALIFIER      0x06      /* Defined new in USB 2.0 */
#define USB_DESCRIPTOR_OTHER_SPEED_CFG       0x07     /* Defined new in USB 2.0 */
#define USB_DESCRIPTOR_INTERFACE_POWER       0x08    /* Defined new in USB 2.0 */

/* Standard Feature selectors. */
#define USB_ENDPOINT_HALT           0x0
#define USB_DEVICE_REMOTE_WAKEUP    0x1
#define USB_HIGH_SPEED_TEST_MODE  0x2

/* Test Mode Definitions */

#define USB_HS_TEST_J_MODE               0x1
#define USB_HS_TEST_K_MODE               0x2
#define USB_HS_TEST_SE0_NAK_MODE    0x3
#define USB_HS_TEST_PACKET_MODE    0x4
#define USB_HS_TEST_FORCE_EN_MODE    0x5



/*********************** Configuration Descriptor **************************/

/* Attribute field bit masks.  */
#define USB_CONFIG_RESERVED_BIT7    0x80

#define USB_CONFIG_ATTRIB_SELF_PWR  0x40
#define USB_CONFIG_ATTRIB_REMT_WKUP 0x20

/*********************** String Descriptor Defines *************************/

/* Language ID codes. */
#define USB_LANG_ID_ENG             0x0409

/**************************************************************************/

/* USB SETUP Packet Structure */
typedef struct 
{
    UINT8   bmRequestType;      /* data transfer direction, req type & recipient            */
    UINT8   bRequest;           /* specific request code                                    */
    UINT16  wValue;             /* value to be passed to the USB device                     */
    UINT16  wIndex;             /* index or offset of an endpoint or an interface           */
    UINT16  wLength;            /* number of bytes to transfer                              */
}USB_SETUP;

/* Standard Device Descriptor Structure */
typedef struct 
{
    UINT8   bLength;            /* size of this descriptor in bytes                         */
    UINT8   bDescriptorType;    /* DEVICE descriptor type                                   */
    UINT16  bcdUSB;             /* USB Specification release number in bcd                  */
    UINT8   bDeviceClass;       /* class code (assigned by the USB)                         */
    UINT8   bDeviceSubclass;    /* subclass code (assigned by the USB)                      */
    UINT8   bDeviceProtocol;    /* protocol code (assigned by the USB)                      */
    UINT8   bMaxPacketSize0;    /* max packet size for endpoint 0                           */
    UINT16  idVendor;           /* vendor ID (assigned by the USB)                          */
    UINT16  idProduct;          /* product ID (assigned by the maufacturer)                 */
    UINT16  bcdDevice;          /* device release number in bcd value                       */
    UINT8   iManufacturer;      /* index of manufacturer string descriptor                  */
    UINT8   iProduct;           /* index of product string descriptor                       */
    UINT8   iSerialNumber;      /* index of serial number string descriptor                 */
    UINT8   bNumConfigurations; /* number of possible configurations                        */
}__attribute__ ((packed)) USB_DEVICE_DESCR;



/* Standard Configuration Descriptor Structure */
typedef struct 
{
    UINT8   bLength;            /* size of this descriptor in bytes                         */
    UINT8   bDescriptorType;    /* CONFIGURATION descriptor type                            */
    UINT16  wTotalLength;       /* total length of data (config, i/f,ep,class descriptors)  */
    UINT8   bNumInterfaces;     /* number of interfaces supported                           */
    UINT8   bConfigurationValue;/* configuration identiifer                                 */ 
    UINT8   iConfiguration;     /* index of string descriptor describing this configuration */
    UINT8   bmAttributes;       /* configuration characteristics                            */
    UINT8   MaxPower;           /* max power consumption of USB device from the bus         */
}__attribute__ ((packed)) USB_CONFIG_DESCR;

/* Standard Interface Descriptor Structure */
typedef struct 
{
    UINT8   bLength;            /* size of this descriptor in bytes                         */
    UINT8   bDescriptorType;    /* INTERFACE descriptor type                                */
    UINT8   bInterfaceNumber;   /* number of interface                                      */
    UINT8   bAlternateSetting;  /* value used to select alternate setting for the interface */
    UINT8   bNumEndpoints;      /* number of endpoints used by this interface               */
    UINT8   bInterfaceClass;    /* class code (assigned by the USB)                         */
    UINT8   bInterfaceSubClass; /* subclass code (assigned by the USB)                      */
    UINT8   bInterfaceProtocol; /* protocol code (assigned by the USB)                      */
    UINT8   iInterface;         /* index of string descriptor describing this interface     */
}__attribute__ ((packed)) USB_INTERFACE_DESCR;

/* Standard Endpoint Descriptor Interface */
typedef struct 
{
    UINT8   bLength;            /* size of this descriptor in bytes                         */
    UINT8   bDescriptorType;    /* ENDPOINT descriptor type                                 */
    UINT8   bEndpointAddress;   /* address of the endpoint ( EP number & direction)         */
    UINT8   bmAttributes;       /* Endpoint's attributes                                    */
    UINT16  wMaxPacketSize;     /* max packet size this endpoint                            */
    UINT8   bInterval;          /* interval for polling endpoint for data transfers         */
}__attribute__ ((packed)) USB_ENDPOINT_DESCR;

/* Standard String Descriptor */
typedef struct 
{
    UINT8   bLength;            /* size of this descriptor in bytes */
    UINT8   bDescriptorType;    /* STRING descriptor type */
    UINT16  bString[MAX_UNICODE_LENGTH]; /* Unicode String */
} __attribute__ ((packed))  USB_STRING_DESCR;

/* Standard Device Qualifier Descriptor */
typedef struct 
{
    UINT8   bLength;            /* size of this descriptor in bytes                         */
    UINT8   bDescriptorType;    /* DEVICE descriptor type                                   */
    UINT16  bcdUSB;             /* USB Specification release number in bcd                  */
    UINT8   bDeviceClass;       /* class code (assigned by the USB)                         */
    UINT8   bDeviceSubclass;    /* subclass code (assigned by the USB)                      */
    UINT8   bDeviceProtocol;    /* protocol code (assigned by the USB)                      */
    UINT8   bMaxPacketSize0;    /* max packet size for endpoint 0                           */
    UINT8   bNumConfigurations; /* number of possible configurations                        */
    UINT8   bReserved; 
} __attribute__ ((packed))  USB_DEVICE_QUALIFIER_DESCR;

/* Standard Other_Speed_Configuration Descriptor */
typedef struct 
{
    UINT8   bLength;            /* size of this descriptor in bytes                         */
    UINT8   bDescriptorType;    /* CONFIGURATION descriptor type                            */
    UINT16  wTotalLength;       /* total length of data (config, i/f,ep,class descriptors)  */
    UINT8   bNumInterfaces;     /* number of interfaces supported                           */
    UINT8   bConfigurationValue;/* configuration identiifer                                 */ 
    UINT8   iConfiguration;     /* index of string descriptor describing this configuration */
    UINT8   bmAttributes;       /* configuration characteristics                            */
    UINT8   bMaxPower;           /* max power consumption of USB device from the bus         */
} __attribute__ ((packed))  USB_OTHER_SPEED_CONFIG_DESCR;

#if 0
/* Standard Interface_Power Descriptor */
typedef struct 
{
    UINT8   bLength;            /* size of this descriptor in bytes */
    UINT8   bDescriptorType;    /* STRING descriptor type */
    UINT16  bString[MAX_UNICODE_LENGTH]; /* Unicode String */
} __attribute__ ((packed))  USB_INTERFACE_POWER_DESCR;
#endif 

/************** Communication class specific Structures and constants **************/

/* Using CDC Version 1.1 */
#define USB_CDC_VER                         0x0110

/* Communication device class code. */
#define USB_CLASS_NOT_USED                  0x0
#define USB_CDC_COMMUNICATION_CLASS         0x2
#define USB_CDC_VENDOR_CLASS                0xFF

/* Communication device interface class code. */
#define USB_CDC_COMMUNICATION_INTERFACE     0x2

/* Communication device interface class sub-codes. */
#define USB_CDC_RESERVED                    0x00
#define USB_CDC_DIRECT_LINE_MODEL           0x01
#define USB_CDC_ABSTRACT_MODEL              0x02
#define USB_CDC_TELEPHONE_MODEL             0x03
#define USB_CDC_MULTI_CHANNEL_MODEL         0x04
#define USB_CDC_CAPI_MODEL                  0x05
#define USB_CDC_ETHERNET_MODEL              0x06
#define USB_CDC_ATM_MODEL                   0x07

/* Communication interface class protocol codes. */
#define USB_CDC_COMM_NO_PROTOCOL            0x0
#define USB_CDC_COMM_AT_PROTOCOL            0x1
#define USB_CDC_COMM_VENDOR_PROTOCOL        0xFF

/* Data interface class codes. */
#define USB_CDC_DATA_INTERFACE              0xA

/* Data interface sub-class codes. */
#define USB_CDC_DATA_SUBCLASS_CODE          0x00

/* Data interface class protocol codes. */
#define USB_CDC_DATA_NO_PROTOCOL            0x00
#define USB_CDC_DATA_VENDOR_PROTOCOL        0xFF

/* Descriptor Type */
#define USB_CDC_CS_INTERFACE                0x24
#define USB_CDC_CS_ENDPOINT                 0x25

/* Descriptor Subtypes.*/
#define USB_CDC_HEADER_FUNCTIONAL_SUBTYPE   0x00
#define USB_CDC_CALL_MGMT_SUBTYPE           0x01
#define USB_CDC_ABSTRACT_CTRL_MGMT_SUBTYPE  0x02
#define USB_CDC_UNION_FUNCT_SUBTYPE         0x06

/* Call Management Fields. */
#define USB_CDC_CALL_MGMT_NOT_SUPPORTED     0x00
#define USB_CDC_CALL_MGMT_SUPPORTED         0x01
#define USB_CDC_CALL_MGMT_COMM_IFACE        0x00
#define USB_CDC_CALL_MGMT_DATA_IFACE        0x02

/* Abstract Control Management fields. */
#define USB_CDC_ABST_CTRL_NO_CMD_SUPPORT    0x00

/* USB CDC Header functional descriptor. */
typedef struct 
{
    UINT8   bLength;            /* size of this descriptor in bytes */
    UINT8   bDescriptorType;    /* descriptor type CS_INTERFACE*/
    UINT8   bDescriptorSubtype; /* Identifier of Functional Descriptor*/
    UINT16  bbcdCDC;            /* CDC version */
}__attribute__ ((packed)) USB_CDC_HEADER_FUNCT_DESC;

/* USB CDC Call Management Descriptor - This describes the processing of calls by the
 * communication class interface. */
typedef struct
{
    UINT8   bLength;            /* size of this descriptor in bytes */
    UINT8   bDescriptorType;    /* descriptor type CS_INTERFACE*/
    UINT8   bDescriptorSubtype; /* Identifier of Functional Descriptor*/
    UINT8   bmCapabilities;     /* Capabilities that this function can support */
    UINT8   bDataInterface;     /* Interface Number of Data Class Interface */
}__attribute__ ((packed)) USB_CDC_CALL_MGMT_DESC;

/* USB CDC Abstract control management descriptor. */
typedef struct
{
    UINT8   bLength;            /* size of this descriptor in bytes */
    UINT8   bDescriptorType;    /* descriptor type CS_INTERFACE*/
    UINT8   bDescriptorSubtype; /* Identifier of Functional Descriptor*/
    UINT8   bmCapabilities;     /* Capabilities that this function can support */
}__attribute__ ((packed)) USB_CDC_ABSTRACT_CONTROL_DESC;

/* USB CDC Union functional descriptor. */
typedef struct
{
    UINT8   bLength;            /* size of this descriptor in bytes */
    UINT8   bDescriptorType;    /* descriptor type CS_INTERFACE*/
    UINT8   bDescriptorSubtype; /* Identifier of Functional Descriptor*/
    UINT8   bMasterInterface;   /* bInterfaceNumber of the Communication Class Interface */
    UINT8   bSlaveInterface;    /* bInterfaceNumber of the Data Class Interface */
}__attribute__ ((packed)) USB_CDC_UNION_FUNCT_DESC;

#endif  /* __USB20_DEF_H_ */

