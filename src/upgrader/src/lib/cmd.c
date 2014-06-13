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

/* ****************************************************************
	int nsp_upg_cmd_copy(char* f, char* t)

	This function posts a COPY message to the upgrader application.

	Arguments:
		s			- source
		d			- destination
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

******************************************************************* */
int nsp_upg_cmd_copy(char* s,char* d)
{
 struct nsp_upg_msg* m;char name[NSP_UPG_MAX_NAMELEN];
 NSP_UPG_NUL_RETURN(s,NSP_UPG_ERR_ERROR);NSP_UPG_NUL_RETURN(d,NSP_UPG_ERR_ERROR);
 NSP_UPG_STRCAT(name,NSP_UPGTEST_MSGQ_DIR,NSP_UPG_QUEUE_ID);
 NSP_UPG_MSGBUF(m);NSP_UPG_MSGCMD_COPY(m,s,d);NSP_UPG_MSG_SEND(m,name);
 return(NSP_UPG_ERR_OK);
}

/* ****************************************************************
	int nsp_upg_cmd_pause(int s)

	This function posts a PAUSE message to the upgrader application.

	Arguments:
		t		- time to pause
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:
******************************************************************* */
int nsp_upg_cmd_pause(int t)
{
 struct nsp_upg_msg* m;char name[NSP_UPG_MAX_NAMELEN];
 NSP_UPG_STRCAT(name,NSP_UPGTEST_MSGQ_DIR,NSP_UPG_QUEUE_ID);
 NSP_UPG_MSGBUF(m);NSP_UPG_MSGCMD_PAUSE(m,t);NSP_UPG_MSG_SEND(m,name);
 return(NSP_UPG_ERR_OK);
}

/* ****************************************************************
	int nsp_upg_cmd_start(void)

	This function starts the upgrader application.

	Arguments:
		none
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:
******************************************************************* */
int nsp_upg_cmd_start(void)
{
 int r;
 r=kill(1,NSP_UPG_START_SIGNAL);
 return(NSP_UPG_ERR_OK);
}

/* ****************************************************************
	int nsp_upg_cmd_setenv(char* n, char* v)

	This function posts a message to the upgarder application to
	set a bootloader environment variable.

	Arguments:
		n			- variable name
		v			- variable value
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:
******************************************************************* */
int nsp_upg_cmd_setenv(char* n, char* v)
{
 struct nsp_upg_msg* m;int r;char name[NSP_UPG_MAX_NAMELEN];
 NSP_UPG_NUL_RETURN(n,NSP_UPG_ERR_ERROR);
 NSP_UPG_STRCAT(name,NSP_UPGTEST_MSGQ_DIR,NSP_UPG_QUEUE_ID);
 NSP_UPG_MSGBUF(m);NSP_UPG_NUL_RETURN(m,NSP_UPG_ERR_ERROR);
 m->mtype=NSP_UPGMSG_CMD;NSP_UPG_MSGCMD(m)=NSP_UPGMSG_SETENV;strcpy(NSP_UPG_MSGNAME(m),n);strcpy(NSP_UPG_MSGVALUE(m),v);
 r=nsp_upg_msgsnd(name,m);NSP_UPG_MSGFREE(m);NSP_UPG_ERR_RETURN(r,errno);
 return(NSP_UPG_ERR_OK);
}

/* ****************************************************************
	int nsp_upg_cmd_reboot(void)

	This function posts a REBOOT message to the upgrader application.

	Arguments:
		none
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:
******************************************************************* */
int nsp_upg_cmd_reboot(void)
{
 struct nsp_upg_msg* m;int r;char name[NSP_UPG_MAX_NAMELEN];
 NSP_UPG_STRCAT(name,NSP_UPGTEST_MSGQ_DIR,NSP_UPG_QUEUE_ID);
 NSP_UPG_MSGBUF(m);NSP_UPG_NUL_RETURN(m,NSP_UPG_ERR_ERROR);
 m->mtype=NSP_UPGMSG_CMD;NSP_UPG_MSGCMD(m)=NSP_UPGMSG_REBOOT;
 r=nsp_upg_msgsnd(name,m);NSP_UPG_MSGFREE(m);NSP_UPG_ERR_RETURN(r,errno);
 return(NSP_UPG_ERR_OK);
}

/* ****************************************************************
	int nsp_upg_cmd_init(void)

	This function initializes the upgrader application.

	Arguments:
		none
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:
******************************************************************* */
int nsp_upg_cmd_init(void)
{
 int r;key_t q;char name[NSP_UPG_MAX_NAMELEN];
 NSP_UPG_STRCAT(name,NSP_UPGTEST_MSGQ_DIR,NSP_UPG_QUEUE_ID);
 r=nsp_upg_msgqcrt(name);NSP_UPG_ERR_RETURN(r,-1);
 q=nsp_upg_msgget(name);NSP_UPG_ERR_RETURN(q,-1);
 return(NSP_UPG_ERR_OK);
}

/* ****************************************************************
	int nsp_upg_cmd_setmtds(char* f,char* i)

	This function posts a SETMTDS message to the upgrader application.

	Arguments:
		s			- source file
		d			- destination
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:
******************************************************************* */
int nsp_upg_cmd_setmtds(char* s,char* d)
{
 struct nsp_upg_msg* m;int r;char name[NSP_UPG_MAX_NAMELEN];
 NSP_UPG_NUL_RETURN(s,NSP_UPG_ERR_ERROR);NSP_UPG_NUL_RETURN(d,NSP_UPG_ERR_ERROR);
 NSP_UPG_STRCAT(name,NSP_UPGTEST_MSGQ_DIR,NSP_UPG_QUEUE_ID);
 NSP_UPG_MSGBUF(m);NSP_UPG_NUL_RETURN(m,NSP_UPG_ERR_ERROR);
 m->mtype=NSP_UPGMSG_CMD;NSP_UPG_MSGCMD(m)=NSP_UPGMSG_SETMTDS;strcpy(NSP_UPG_MSGSRC(m),s);strcpy(NSP_UPG_MSGDST(m),d);
 r=nsp_upg_msgsnd(name,m);NSP_UPG_MSGFREE(m);NSP_UPG_ERR_RETURN(r,errno);
 return(NSP_UPG_ERR_OK);
}

/* ****************************************************************
	int nsp_upg_cmd_flash(char* s)

	This function posts a FLASH message to the upgrader application.

	Arguments:
		s			- filename to flash
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:
******************************************************************* */
int nsp_upg_cmd_flash(char* s)
{
 struct nsp_upg_msg* m;int r;char name[NSP_UPG_MAX_NAMELEN];
 NSP_UPG_NUL_RETURN(s,NSP_UPG_ERR_ERROR);
 NSP_UPG_STRCAT(name,NSP_UPGTEST_MSGQ_DIR,NSP_UPG_QUEUE_ID);
 NSP_UPG_MSGBUF(m);NSP_UPG_NUL_RETURN(m,NSP_UPG_ERR_ERROR);
 m->mtype=NSP_UPGMSG_CMD;NSP_UPG_MSGCMD(m)=NSP_UPGMSG_FLASH;strcpy(NSP_UPG_MSGSRC(m),s);
 r=nsp_upg_msgsnd(name,m);NSP_UPG_MSGFREE(m);NSP_UPG_ERR_RETURN(r,errno);
 return(NSP_UPG_ERR_OK);
}

