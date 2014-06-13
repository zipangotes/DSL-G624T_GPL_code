/*+----------------------------------------------------------------------------------+**
**|                            ****                                                  |**
**|                            ****                                                  |**
**|                            ******o***                                            |**
**|                      ********_///_****                                           |**
**|                      ***** /_//_/ ****                                           |**
**|                       ** ** (__/ ****                                            |**
**|                           *********                                              |**
**|                            ****                                                  |**
**|                            ***                                                   |**
**|                                                                                  |**
**|         Copyright (c) 1998-2004 Texas Instruments Incorporated                   |**
**|                        ALL RIGHTS RESERVED                                       |**
**|                                                                                  |**
**| Permission is hereby granted to licensees of Texas Instruments                   |**
**| Incorporated (TI) products to use this computer program for the sole             |**
**| purpose of implementing a licensee product based on TI products.                 |**
**| No other rights to reproduce, use, or disseminate this computer                  |**
**| program, whether in part or in whole, are granted.                               |**
**|                                                                                  |**
**| TI makes no representation or warranties with respect to the                     |**
**| performance of this computer program, and specifically disclaims                 |**
**| any responsibility for any damages, special or consequential,                    |**
**| connected with the use of this program.                                          |**
**|                                                                                  |**
**+----------------------------------------------------------------------------------+**/
#include "nsp_upg.h"

#define	NSP_UPG_MSGQ_NAME_SIZE		32
#define	NSP_UPG_MSGQ_KEY_FORMAT		"%d"

/* **************************************************************************
	int nsp_upg_msgqcrt(char* n)

	This function creates a message queue and stores the queue ID in the
	provided location.

	Arguments:
		n			- location of the unique queue ID
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_msgqcrt(char* n)
{
 int f,r,w,t;key_t q;char b[NSP_UPG_MSGQ_NAME_SIZE];
 f=0;r=0;w=0;q=0;t=0;NSP_UPG_BZERO(b);
 NSP_UPG_NUL_RETURN(n,NSP_UPG_ERR_ERROR);
 q=msgget(0,IPC_CREAT);NSP_UPG_ERR_RETURN(q,NSP_UPG_ERR_ERROR);
 NSP_UPG_WCOPEN(f,n);r=sprintf(b,NSP_UPG_MSGQ_KEY_FORMAT,q);NSP_UPG_WRITE(r,b,f);close(f);
 return(NSP_UPG_ERR_OK);
}

/* **************************************************************************
	key_t nsp_upg_msgget(char* n)

	This function retrieves the message queue ID from the specified location.

	Arguments:
		n			- location of the unique queue ID
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
key_t nsp_upg_msgget(char* n)
{
 int f,r;key_t q;char b[NSP_UPG_MSGQ_NAME_SIZE];
 NSP_UPG_NUL_RETURN(n,NSP_UPG_ERR_ERROR);
 NSP_UPG_ROPEN(f,n);NSP_UPG_READ(r,b,f);close(f);
 b[r]=NSP_UPG_EOL;q=atoi(b);
 return(q);
}

/* **************************************************************************
	int nsp_upg_msgsnd(char* n, struct nsp_upg_msg* m)

	This function sends a message to the specified message queue.

	Arguments:
		n			- location of the unique queue ID
		m			- pointer to the message (to send)
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:
***************************************************************************** */
int nsp_upg_msgsnd(char* n, struct nsp_upg_msg* m)
{
 int r;key_t q;
 NSP_UPG_NUL_RETURN(n,NSP_UPG_ERR_ERROR);r=0;q=nsp_upg_msgget(n);NSP_UPG_ERR_RETURN(q,NSP_UPG_ERR_ERROR);
 r=msgsnd(q,m,NSP_UPG_MSGSIZE,IPC_NOWAIT);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
 return(NSP_UPG_ERR_OK);
}

/* **************************************************************************
	int nsp_upg_msgrcv(char* n,nsp_upg_msg* m)

	This function receives a message from the specified queue.

	Arguments:
		n			- location of the unique queue ID
		m			- pointer to the message (where to receive it)
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_msgrcv(char* n,nsp_upg_msg* m)
{
 int o,r;key_t q;
 NSP_UPG_NUL_RETURN(n,NSP_UPG_ERR_ERROR);
 o=(MSG_NOERROR|IPC_NOWAIT);q=nsp_upg_msgget(n);NSP_UPG_ERR_RETURN(q,NSP_UPG_ERR_ERROR);
 r=msgrcv(q,m,NSP_UPG_MSGSIZE,NSP_UPGMSG_CMD,o);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
 return(NSP_UPG_ERR_OK);
}

