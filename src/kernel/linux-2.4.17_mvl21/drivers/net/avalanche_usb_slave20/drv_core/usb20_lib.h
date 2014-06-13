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
 * FILE PURPOSE :   Common utilities library for USB driver
 **************************************************************************
 * FILE NAME    :   usb20_lib.h
 *
 * DESCRIPTION  :
 *  Contains structures and exported function that are used by the linked
 *  list library.
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/

#ifndef __USB20_LIB_H_
#define __USB20_LIB_H_

#include "usb20_config.h"

/* Extern definitions */
extern int	printf (const char *, ...);


/**************************************************************************
 *  The structure defines a LIST NODE structure that contains links to the 
 *  previous and next element in the list.
 **************************************************************************/
typedef struct USB_LIST_NODE
{
    void*   p_next;     /* Pointer to the next element in the list.     */
    void*   p_prev;     /* Pointer to the previous element in the list. */
}USB_LIST_NODE;

/************************ EXTERN Functions *********************************/

extern void usb_list_add (USB_LIST_NODE **ptr_list, USB_LIST_NODE *ptr_node);
extern USB_LIST_NODE* usb_list_remove (USB_LIST_NODE **ptr_list);
extern void usb_list_cat (USB_LIST_NODE **ptr_list_dst, USB_LIST_NODE **ptr_list_src);

extern USB_LIST_NODE *usb_list_get_head (USB_LIST_NODE **ptr_list);
extern USB_LIST_NODE *usb_list_get_tail (USB_LIST_NODE **ptr_list);
extern USB_LIST_NODE *usb_list_get_next (USB_LIST_NODE *ptr_list);


/**************************************************************************
 *   D E B U G G I N G    F R A M E W O R K 
 *************************************************************************/

#define DEBUG_USB20   1
#define DEBUG_RNDIS   2

#define USB20_LOG_ALL    0xFFFFFFFE   /* Except Fatal */
#define USB20_FATAL        0x01
#define USB20_CONTROL   0x02
#define USB20_DATA         0x04
#define USB20_ISR            0x08



extern UINT32  usb20_log_level;

#ifdef USB20_DEBUG 

#define USB_ENTER  /*printf("Entering : %s\n",__FUNCTION__)*/
#define USB_EXIT     /*printf("Returning from : %s\n",__FUNCTION__)*/

extern int usb20_logMsg (char*   fmt,int arg1,int arg2,int arg3,   
                         int arg4,int arg5,int arg6);

/*int usb20_dbg_level;  */
#define USB_LOG(level, format, arg1, arg2, arg3, arg4) \
	if ( usb20_log_level & level  ) \
		{\
		if ( usb20_log_level & USB20_ISR ) \
			usb20_logMsg(format,arg1,arg2,arg3,arg4,0,0);\
		else\
		      usb20_logMsg(format,arg1,arg2,arg3,arg4,0,0);\
		}
	/*if ( usb20_dbg_level & level ) usb20_logMsg(format, arg1, arg2, arg3, arg4,0,0);*/
	
#define USB_ISR_LOG(level, format, arg1, arg2, arg3, arg4) \
	if ( usb20_log_level & USB20_ISR) \
		usb20_logMsg(format, arg1, arg2, arg3, arg4,0,0);

#else
#define USB_LOG(level, format, arg1, arg2, arg3, arg4) 
#define USB_ISR_LOG(level, format, arg1, arg2, arg3, arg4) 
#define USB_ENTER 
#define USB_EXIT 
#endif  /* USB20_DEBUG */

/*
 * Byte swapping macros. Required to convert Little <--> Big endian 
 * multi-byte data. USB data on bus are always in little endian. 
 */

#ifdef USB_BIG_ENDIAN 
    #define USB_SWAPW(d)     ((((d) >>  8) & 0x00FF) | (((d) <<  8) & 0xFF00))
    #define USB_SWAPL(d)            \
    (                                   \
        (((d) >> 24) & 0x000000FF) |    \
        (((d) >>  8) & 0x0000FF00) |    \
        (((d) <<  8) & 0x00FF0000) |    \
        (((d) << 24) & 0xFF000000)      \
    )
#else
    #define USB_SWAPL(d)    d
    #define USB_SWAPW(d)    d
#endif  /* USB_BIG_ENDIAN */

#define USB_MAX(a,b)  (a>b?a:b)
#define USB_MIN(a,b)  (a<b?a:b)\

    
#endif  /* __USB20_LIB_H_ */

