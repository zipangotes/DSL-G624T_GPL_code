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
**+----------------------------------------------------------------------------------+*/
#include "nsp_upg.h"

/* **************************************************************************
	static int nsp_upg_msgexec(struct nsp_upg_msg* m)

	This function executes actions specified in the message.

	Arguments:
		m					- pointer to the message
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
static int nsp_upg_msgexec(struct nsp_upg_msg* m)
{
 int r;nsp_upgmsg_type t;
 r=0;t=NSP_UPG_MSGCMD(m);
 switch(t)
 {
  case NSP_UPGMSG_PAUSE:
   NSP_UPG_PRINTF("PAUSE %d seconds... ",NSP_UPG_MSGTIME(m));
   if((NSP_UPG_MSGTIME(m)<0)||(NSP_UPG_MSGTIME(m)>360)){NSP_UPG_PRINTF("bad time value to pause!\n");break;}
   r=sleep(NSP_UPG_MSGTIME(m));if(r<0){NSP_UPG_PRINTF("PAUSE %d failed.\n",NSP_UPG_MSGTIME(m));break;}
   NSP_UPG_PRINTF("OK\n");
  break;
  case NSP_UPGMSG_SETENV:
   NSP_UPG_PRINTF("SETENV: %s => %s.\n",NSP_UPG_MSGNAME(m),NSP_UPG_MSGVALUE(m));
   r=nsp_upg_setenv(NSP_UPG_MSGNAME(m),NSP_UPG_MSGVALUE(m));
   if(r<0){NSP_UPG_PRINTF("SETENV failed - %s => %s",NSP_UPG_MSGNAME(m),NSP_UPG_MSGVALUE(m));break;}
   NSP_UPG_PRINTF("OK\n");
   break;
  case NSP_UPGMSG_LEDS:
   NSP_UPG_PRINTF("LEDS... ");
   NSP_UPG_PRINTF("OK\n");
   break;
  case NSP_UPGMSG_COPY:
   NSP_UPG_PRINTF("COPY %s -> %s.\n",NSP_UPG_MSGSRC(m),NSP_UPG_MSGDST(m));
   r=nsp_upg_msg_copy(NSP_UPG_MSGSRC(m),NSP_UPG_MSGDST(m));
   if(r<0){NSP_UPG_PRINTF("COPY failed - %s => %s",NSP_UPG_MSGSRC(m),NSP_UPG_MSGDST(m));break;}
   break;
  case NSP_UPGMSG_FLASH:
   NSP_UPG_PRINTF("FLASH %s...\n",NSP_UPG_MSGSRC(m));
   r=nsp_upg_msg_flash(NSP_UPG_MSGSRC(m));
   if(r<0){NSP_UPG_PRINTF("FLASH %s failed.\n",NSP_UPG_MSGSRC(m));break;}
   break;
  case NSP_UPGMSG_REBOOT:
   NSP_UPG_PRINTF("REBOOT.\n");
   nsp_upg_reboot();
   break;
  case NSP_UPGMSG_NOP:
   NSP_UPG_PRINTF("NOP... ");
   NSP_UPG_PRINTF("OK\n");
   break;
  case NSP_UPGMSG_SETMTDS:
   NSP_UPG_PRINTF("SETMTDS %s : %s.\n",NSP_UPG_MSGSRC(m),NSP_UPG_MSGDST(m));
   r=nsp_upg_msg_setmtds(NSP_UPG_MSGSRC(m),NSP_UPG_MSGDST(m));
   if(r<0){NSP_UPG_PRINTF("SETMTDS failed.\n");break;}
   /*NSP_UPG_PRINTF("OK\n");*/
   break;
  default:
   NSP_UPG_PRINTF("Bogus message received!\n");
  break;
 }
 return(r);
}

/* **************************************************************************
	int	nsp_upg_msgqread(char* n)

	This function reads message queue "n" and calls the executing function
	to process each message.

	Arguments:
		n					- queue name
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int	nsp_upg_msgqread(char* n)
{
 key_t q;struct nsp_upg_msg *m;int c,r;
 /*NSP_UPG_PRINTF("Reading upgrader message queue...\n",n);*/
 c=0;r=0;q=nsp_upg_msgget(n);NSP_UPG_ERR_RETURN(q,NSP_UPG_ERR_ERROR);
 m=(struct nsp_upg_msg*)malloc(sizeof(struct nsp_upg_msg));
 if(m==NULL){NSP_UPG_PRINTF("nsp_upg_msgqread(): can't allocate a message buffer...\n");NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
 while(1)
 {
  /* Read and execute each message in the queue */
  r=nsp_upg_msgrcv(n,m);if(r<0){if(errno==ENOMSG){return(c);}NSP_UPG_ERR_RETURN(r,errno);}
  r=nsp_upg_msgexec(m);if(r<0){NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);};c++;
 }
  return(c);
}

