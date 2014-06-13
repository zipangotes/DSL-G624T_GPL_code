/*
 * Copyright (c) 2002 
 *      Texas Instruments.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Texas Instruments. 
 * 4. Neither the name of the Company nor of the product may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


/**************************************************************************
 * FILE PURPOSE	:  	Header file for the linked list library.
 **************************************************************************
 * FILE NAME	:   listlib.h
 *
 * DESCRIPTION	:
 * 	Contains structures and exported function that are used by the linked
 * 	list library.
 *
 *	(C) Copyright 2002, Texas Instruments, Inc.
 *************************************************************************/

#ifndef __LISTLIB_H__
#define __LISTLIB_H__

/**************************************************************************
 * STRUCTURE -  LIST_NODE
 **************************************************************************
 *	The structure defines a LIST NODE structure that contains links to the 
 *	previous and next element in the list.
 **************************************************************************/
typedef struct LIST_NODE
{
	void*	p_next;		/* Pointer to the next element in the list. 	*/	
}LIST_NODE;

/************************ EXTERN Functions *********************************/

extern void list_add (LIST_NODE **ptr_list, LIST_NODE *ptr_node);
extern LIST_NODE* list_remove (LIST_NODE **ptr_list);
extern LIST_NODE* list_get_head (LIST_NODE **ptr_list);
extern LIST_NODE* list_get_next (LIST_NODE *ptr_list);
extern int list_remove_node (LIST_NODE **ptr_list, LIST_NODE *ptr_remove);
extern void list_cat (LIST_NODE **ptr_dst, LIST_NODE **ptr_src);

#endif	/* __LISTLIB_H__ */


