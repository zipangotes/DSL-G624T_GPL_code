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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <signal.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/reboot.h>
#include <linux/reboot.h>
#include "msg.h"
#include "error.h"

#ifndef ___NSP_UPG_H___
#define	___NSP_UPG_H___

#define	NSP_UPG_DEBUG	1

#define	SIMAGE_MAGIC_NUMBER		(0x4D544434)

/* Public place for the message queue */
#define	NSP_UPG_CONSOLE			"/dev/console"
#define	NSP_UPG_BOOT_ENV		"/proc/ticfg/env"
#define	NSP_UPGTEST_MSGQ_DIR	"/var"

/* file system related constants */
#define	NSP_UPG_PROC_DIR		"/proc"
#define	NSP_UPG_DEV_DIR			"/dev"
#define	NSP_UPG_FLASH_DIR		"/flash"

/* internal constants */
#define	NSP_UPG_PROC_TYPE		"proc"
#define	NSP_UPG_DEV_TYPE		"devfs"

/* progress bar related */
#define	NSP_UPG_PROGRESS_FORMAT	"%s: |%s|  %d%%\r"
#define	NSP_UPG_PROGRESS_MARK	'*'
#define	NSP_UPG_PROGRESS_COUNT	32

/* parsing related constants */
#define	NSP_UPG_LF				'\n'
#define	NSP_UPG_CR				'\r'
#define	NSP_UPG_EOL				'\0'
#define	NSP_UPG_TAB				'\t'
#define	NSP_UPG_SPACE			' '

/* mtd related */
#define	NSP_UPG_MTD_DIVIDER		','

#define	NSP_UPG_MTD_FILESYS		"mtd0"
#define	NSP_UPG_MTD_KERNEL		"mtd1"
#define	NSP_UPG_MTD_BOOTLOADER	"mtd2"
#define	NSP_UPG_MTD_NSPCONFIG	"mtd3"
#define	NSP_UPG_MTD_SIMAGE		"mtd4"

#define	NSP_UPG_MTD_FBLOCK		"/dev/mtdblock/0"
#define	NSP_UPG_MTD_KBLOCK		"/dev/mtdblock/1"
#define	NSP_UPG_MTD_SBLOCK		"/dev/mtdblock/4"

/* size related */
#define	NSP_UPG_BOOT_ENV_VSZ	1024	/* max length of a bootloader environment variable */
#define	NSP_UPG_BOOT_ENV_SZ		8000	/* max size of the bootloader environment block */
#define	NSP_UPG_MAXFD			100		/* max file descriptor count per process */
#define	NSP_UPG_START_SIGNAL	15		/* start signal */
#define	NSP_UPG_START_PAUSE		1		/* start pause (seconds) */
#define	NSP_UPG_MAX_NAMELEN		256		/* file/directory max name length (combined) */

/* internal API */
extern int nsp_upg_printf(char*,...);
extern int nsp_upg_printf_close(void);
extern void nsp_upg_closeall(void);

extern int nsp_upg_change_root(char*);
extern int nsp_upg_msgqcrt(char*);
extern int nsp_upg_getenv(char*,char[],int);
extern int nsp_upg_setenv(char*,char*);
extern int nsp_upg_copyfile(char*,char*);
extern int nsp_upg_reboot(void);
extern int nsp_upg_dirchk(char* d);

extern int nsp_upg_hex2dig(char);
extern int nsp_upg_hex2int(char*,unsigned int*);

/* message parsers */
extern int nsp_upg_msg_copy(char*,char*);
extern int nsp_upg_msg_setmtds(char*,char*);
extern int nsp_upg_msg_flash(char*);

/* debugging related */
#ifdef	NSP_UPG_DEBUG
#define NSP_UPG_LOGERR			nsp_upg_err_print()
#define	NSP_UPG_PRINTF(f,...)	nsp_upg_printf(f, ## __VA_ARGS__)
#define	NSP_UPG_DPRINTF(f,...)	nsp_upg_printf(f, ## __VA_ARGS__)
#define	NSP_UPG_ERR_SET(v)		{nsp_upg_error=(v);}
#define	NSP_UPG_ERR_RETURN(e,v)	{if((e)<0){nsp_upg_error=(v);return(-1);}}
#define	NSP_UPG_NUL_RETURN(p,v)	{if((p)==0){nsp_upg_error=(v);return(-1);}}
#define	NSP_UPG_WCOPEN(hnd,nm)	{hnd=open(nm,O_WRONLY|O_CREAT|O_SYNC);if(hnd<0){nsp_upg_error=errno;return(NSP_UPG_ERR_ERROR);}}
#define	NSP_UPG_WOPEN(hnd,nm)	{hnd=open(nm,O_WRONLY|O_SYNC);if(hnd<0){nsp_upg_error=errno;return(NSP_UPG_ERR_ERROR);}}
#define	NSP_UPG_WRITE(c,b,f)	{int wrb,res;wrb=0;res=0;while(wrb!=(c)){res=write((f),&b[wrb],(c)-wrb);if(res<0){nsp_upg_error=errno;close(f);return(NSP_UPG_ERR_ERROR);};wrb+=res;}}
#define	NSP_UPG_ROPEN(hnd,nm)	{hnd=open(nm,O_RDONLY);if(hnd<0){nsp_upg_error=errno;return(NSP_UPG_ERR_ERROR);}}
#define	NSP_UPG_READ(c,b,f)		{c=read(f,b,sizeof(b));if(c<0){nsp_upg_error=errno;close(f);return(NSP_UPG_ERR_ERROR);}}
#else
#define NSP_UPG_LOGERR
#define	NSP_UPG_PRINTF(f,...)	;
#define	NSP_UPG_DPRINTF(f,...)	;
#define	NSP_UPG_ERR_SET(v)		;
#define	NSP_UPG_ERR_RETURN(e,v)	{if((e)<0){return(-1);}}
#define	NSP_UPG_WCOPEN(hnd,nm)	{hnd=open(nm,O_WRONLY|O_CREAT|O_SYNC);if(hnd<0){return(NSP_UPG_ERR_ERROR);}}
#define	NSP_UPG_WOPEN(hnd,nm)	{hnd=open(nm,O_WRONLY|O_SYNC);if(hnd<0){return(NSP_UPG_ERR_ERROR);}}
#endif

#define	NSP_UPG_BZERO(b)		{memset(b,0,sizeof(b));}
#define	NSP_UPG_VZERO(v)		{memset(&v,0,sizeof(v));}

#define	NSP_UPG_STRCAT(r,o,t)	{strcpy(r,o);strcat(r,t);}

/* missing prototypes (from Linux API) */
extern int pivot_root(char*,char*);

/* external API */
extern int nsp_upg_cmd_init(void);
extern int nsp_upg_cmd_copy(char*,char*);
extern int nsp_upg_cmd_setmtds(char*,char*);
extern int nsp_upg_cmd_pause(int);
extern int nsp_upg_cmd_setenv(char*,char*);
extern int nsp_upg_cmd_reboot(void);
extern int nsp_upg_cmd_start(void);
extern int nsp_upg_cmd_flash(char*);

#endif





