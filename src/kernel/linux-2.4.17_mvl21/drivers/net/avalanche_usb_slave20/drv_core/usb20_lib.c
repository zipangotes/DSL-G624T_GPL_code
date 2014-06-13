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
 * FILE PURPOSE :   Linked List library.
 **************************************************************************
 * FILE NAME    :   usb20_lib.c
 *
 * DESCRIPTION  :
 *  Implementation of a doubly linked circular list.
 *
 *  CALL-INs:
 *
 *  CALL-OUTs:
 *
 *  User-Configurable Items:
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "usb20_lib.h"

/**************************************************************************
 * FUNCTION NAME : usb_list_add
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called to add a node 'ptr_node' to the END of the 
 *  list - 'ptr_list'.
 ***************************************************************************/
void usb_list_add (USB_LIST_NODE **ptr_list, USB_LIST_NODE *ptr_node)
{
    USB_LIST_NODE*  ptr_head;
    USB_LIST_NODE*  ptr_tail;

    /* Check if the list is empty ? */
    if (*ptr_list == NULL)
    {
        /* YES the list is empty. Initialize the links */
        ptr_node->p_next = ptr_node;
        ptr_node->p_prev = ptr_node;

        /* Initialize the LIST */
        *ptr_list = ptr_node;
        return;
    }

    /* No the list was NOT empty. Add the node to the end of list. */

    /* Get the pointer to the head of the list. */
    ptr_head = *ptr_list;

    /* Get the tail of the list. */
    ptr_tail = ptr_head->p_prev;

    /* Link the last node with the one just added. */
    ptr_tail->p_next = ptr_node;
    ptr_node->p_prev = ptr_tail;

    /* The head of the list now points to the last node. */
    ptr_head->p_prev  = ptr_node;
    ptr_node->p_next  = ptr_head;

    return;
}

/**************************************************************************
 * FUNCTION NAME : usb_list_remove
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called to remove a node from the fron of the list.
 *
 * RETURNS       :
 *      NULL  - If there are no elements in the list.
 *      Pointer to the head of the list 
 ***************************************************************************/
USB_LIST_NODE* usb_list_remove (USB_LIST_NODE **ptr_list)
{
    USB_LIST_NODE*  ptr_head;
    USB_LIST_NODE*  ptr_next;
    USB_LIST_NODE*  ptr_last;

    /* Check if the list is empty ? */
    if (*ptr_list == NULL)
    {
        /* There is nothing in the list. */
        return NULL;
    }
    
    /* Remove the head of the list. */
    ptr_head = *ptr_list;

    /* Are we removing the last element in the list ? This can be determined
     * if the first and last element are the same. */
    if (ptr_head == ptr_head->p_prev)
    {
        /* YES. The last node was removed from the list. Make the list empty.*/
        *ptr_list = NULL;
    }
    else
    {
        /* No. There are still some elements left in the list. */

        /* Get the next list node. */
        ptr_next = ptr_head->p_next;

        /* Get the last list node. */
        ptr_last = ptr_head->p_prev;

        /* This NODE is now the head make it point to the end of list. */
        ptr_next->p_prev = ptr_head->p_prev;

        /* Make the last node point to the new head of list. */
        ptr_last->p_next = ptr_next;

        /* Mark the NEXT node as the new start of list. */
        *ptr_list = ptr_next;
    }

    /* Kill all the links before returning the node. */
    ptr_head->p_next = NULL;
    ptr_head->p_prev = NULL;
    return ptr_head;
}

/**************************************************************************
 * FUNCTION NAME : usb_list_cat
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called to append the src list to the destination list. 
 ***************************************************************************/
void usb_list_cat (USB_LIST_NODE **ptr_list_dst, USB_LIST_NODE **ptr_list_src)
{
    USB_LIST_NODE *ptr_head_dst;
    USB_LIST_NODE *ptr_tail_dst;
    USB_LIST_NODE *ptr_head_src;
    USB_LIST_NODE *ptr_tail_src;

    /* Check if the SOURCE list is empty. */
    if (*ptr_list_src == NULL)
    {
        /* The source list is EMPTY. There is nothing to do. */
        return;
    }

    /* Check if the DESTINATION list is empty. */
    if (*ptr_list_dst == NULL)
    {
        /* YES. In that case the destination list will be the same
         * as the source list. */
        *ptr_list_dst = *ptr_list_src;
        return;
    }

    /* Get the head and tail of the destination list. */
    ptr_head_dst = *ptr_list_dst;
    ptr_tail_dst = ptr_head_dst->p_prev;

    /* Get the head and tail of the source list. */
    ptr_head_src = *ptr_list_src;
    ptr_tail_src = ptr_head_src->p_prev;

    /* The tail of the destination will now point to the head of the src and vice versa. */
    ptr_tail_dst->p_next = ptr_head_src;
    ptr_head_src->p_prev = ptr_tail_dst;

    /* The head of the destination will now point to the tail of src and vice versa. */
    ptr_head_dst->p_prev = ptr_tail_src;
    ptr_tail_src->p_next = ptr_head_dst;

    return;
}

/**************************************************************************
 * FUNCTION NAME : usb_list_get_head
 **************************************************************************
 * DESCRIPTION   :
 *  The function returns the head element of the list.
 *
 * RETURNS       :
 *  Head LIST NODE.
 ***************************************************************************/
USB_LIST_NODE *usb_list_get_head (USB_LIST_NODE **ptr_list)
{
    return (*ptr_list);
}

/**************************************************************************
 * FUNCTION NAME : usb_list_get_next
 **************************************************************************
 * DESCRIPTION   :
 *  The function returns the next element.
 *
 * RETURNS       :
 *  Head LIST NODE.
 ***************************************************************************/
USB_LIST_NODE *usb_list_get_next (USB_LIST_NODE *ptr_list)
{
    return (ptr_list->p_next);
}

/**************************************************************************
 * FUNCTION NAME : usb_list_get_tail
 **************************************************************************
 * DESCRIPTION   :
 *  The function returns the tail element of the list.
 *  
 * RETURNS       :
 *  Tail LIST NODE.
 ***************************************************************************/
USB_LIST_NODE *usb_list_get_tail (USB_LIST_NODE **ptr_list)
{
    return ((*ptr_list)->p_prev);
}


/***************************************************************************
 *   DEBUG ROUTINES
 **************************************************************************/
/***** Global ********/
UINT32  usb20_log_level = 0;

void usb20_set_log_level(unsigned int module)
{
	usb20_log_level |= module;
}

void usb20_clear_log_level(unsigned int module)
{
	usb20_log_level &= ~module;
}

