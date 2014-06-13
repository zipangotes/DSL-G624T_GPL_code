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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#ifndef ___NSP_UPGMSG_H___
#define	___NSP_UPGMSG_H___

/* Public place where the message Q id is stored for external access */
#define	NSP_UPG_QUEUE_ID	"/nspmsg.qid"
#define	NSP_UPGMSG_CMD		1	/* Message type for msgsnd() and msgrcv() */

typedef	enum nsp_upgmsg_type
{
	NSP_UPGMSG_NOP		= 0,	/* No operation */
	NSP_UPGMSG_SETENV	= 1,	/* setenv */
	NSP_UPGMSG_COPY		= 2,	/* copy src file to dst file */
	NSP_UPGMSG_REBOOT	= 3,	/* reboot the box */
	NSP_UPGMSG_LEDS		= 4,	/* in-progress LEDs (on, off, etc) */
	NSP_UPGMSG_PAUSE	= 5,	/* */
	NSP_UPGMSG_SETMTDS	= 6,	/* set MTD variables */
	NSP_UPGMSG_FLASH	= 7		/* program flash memory */
} nsp_upgmsg_type;

typedef struct nsp_upg_msg
{
	long					mtype;	/* IPC message type */
	struct {
		nsp_upgmsg_type	cmd;		/* command (see message types above) */
		union {
			struct {
				char name[64];		/* name of environment variable */
				char value[64];		/* value of environment variable */
			}				env;
			struct {
				char src[64];		/* source file name (presumably in /var) */
				char dst[64];		/* destination file name (presumably in /dev) */
			}				file;
			struct {
				int		num;		/* Number of LEDs we are setting up */
				void* id[16];		/* LED ids (pointers, indeces, etc) */
				int state[16];		/* State of each */
			} leds;
			int		time;			/* pause time */
		}					args;	/* message arguments */
	} mtext;						/* text message */
} nsp_upg_msg;

#define	NSP_UPG_MSGCMD(m)	((m)->mtext.cmd)
#define	NSP_UPG_MSGSRC(m)	((m)->mtext.args.file.src)
#define	NSP_UPG_MSGDST(m)	((m)->mtext.args.file.dst)
#define	NSP_UPG_MSGNAME(m)	((m)->mtext.args.env.name)
#define	NSP_UPG_MSGVALUE(m)	((m)->mtext.args.env.value)
#define	NSP_UPG_MSGTIME(m)	((m)->mtext.args.time)

#define	NSP_UPG_MSGSIZE		(sizeof(struct nsp_upg_msg))
#define	NSP_UPG_MSGBUF(p)	{p=(struct nsp_upg_msg*)malloc(NSP_UPG_MSGSIZE);NSP_UPG_NUL_RETURN(p,NSP_UPG_ERR_ERROR);}
#define	NSP_UPG_MSGFREE(p)	{free(p);}

#define	NSP_UPG_MSGCMD_COPY(m,s,d)	{(m)->mtype=NSP_UPGMSG_CMD;(m)->mtext.cmd=NSP_UPGMSG_COPY;strcpy((m)->mtext.args.file.src,s);strcpy((m)->mtext.args.file.dst,d);}
#define	NSP_UPG_MSGCMD_PAUSE(m,t)	{(m)->mtype=NSP_UPGMSG_CMD;(m)->mtext.cmd=NSP_UPGMSG_PAUSE;(m)->mtext.args.time=t;}

#define	NSP_UPG_MSG_SEND(m,q)	{int res=nsp_upg_msgsnd(q,m);NSP_UPG_MSGFREE(m);NSP_UPG_ERR_RETURN(res,errno);}

extern int nsp_upg_msgget(char* n);
extern int nsp_upg_msgqread(char* n);
extern int nsp_upg_msgsnd(char* n, struct nsp_upg_msg* m);
extern int nsp_upg_msgrcv(char* n, struct nsp_upg_msg* m);

#endif /* ___NSP_UPGMSG_H___ */




