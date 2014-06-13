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
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

/* **************************************************************************
	int nsp_upg_dirchk(char* d)

	This function checks the specified directory and creates it if it is missing.

	Arguments:
		d					- directory to check
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_dirchk(char* d)
{
 DIR *dirp;int r;
 if(d==NULL){return(NSP_UPG_ERR_ERROR);};
 dirp=opendir(d);
 if(dirp==NULL)
 {
  switch(errno)
  {
   case ENOENT:
   /* directory is not there, create it */
   NSP_UPG_PRINTF("Creating %s...",d);
   r=mkdir(d,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
   if(r<0){NSP_UPG_PRINTF("Unable to create %s\n",d);return(NSP_UPG_ERR_ERROR);}
   NSP_UPG_PRINTF("OK\n");
   break;
  default:
   NSP_UPG_PRINTF("System error %d checking %s\n",errno,d);return(NSP_UPG_ERR_ERROR);
   break;
  }
 }
 closedir(dirp);
 return(NSP_UPG_ERR_OK);
}

/* **************************************************************************
	int nsp_upg_change_root(char* p)

	This function changes root file system to the specified in the argument.

	Arguments:
		p					- new root
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_change_root(char* p)
{
 int r;char b[NSP_UPG_MAX_NAMELEN];

 /* Move PROC to a new location */
 memset(b,0,sizeof(b));strcpy(b,p);strcat(b,NSP_UPG_PROC_DIR);
 NSP_UPG_PRINTF("Mounting PROC on %s...",b);
 r=mount(NSP_UPG_PROC_DIR,b,NSP_UPG_PROC_TYPE,MS_BIND,0);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);NSP_UPG_PRINTF("OK\n");

 /* Move DEV to a new location */ 
 memset(b,0,sizeof(b));strcpy(b,p);strcat(b,NSP_UPG_DEV_DIR);
 NSP_UPG_PRINTF("Mounting DEV on %s...",b);nsp_upg_printf_close();nsp_upg_closeall();
 r=mount(NSP_UPG_DEV_DIR,b,NSP_UPG_DEV_TYPE,MS_BIND,0);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);NSP_UPG_PRINTF("OK\n");
 
 NSP_UPG_PRINTF("Pivoting / to %s...",p);nsp_upg_printf_close();nsp_upg_closeall();
 memset(b,0,sizeof(b));strcpy(b,p);strcat(b,NSP_UPG_FLASH_DIR);
 r=pivot_root(p,b);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);NSP_UPG_PRINTF("OK\n");

 NSP_UPG_PRINTF("Changing work dir to / ...");
 r=chdir("/");NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);NSP_UPG_PRINTF("OK\n");

 memset(b,0,sizeof(b));strcat(b,NSP_UPG_FLASH_DIR);strcat(b,NSP_UPG_PROC_DIR);
 NSP_UPG_PRINTF("Unmounting %s...",b);nsp_upg_printf_close();nsp_upg_closeall();
 r=umount2(b, MNT_FORCE);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);NSP_UPG_PRINTF("OK\n");

 memset(b,0,sizeof(b));strcat(b,NSP_UPG_FLASH_DIR);strcat(b,NSP_UPG_DEV_DIR);
 NSP_UPG_PRINTF("Unmounting %s...",b);nsp_upg_printf_close();nsp_upg_closeall();
 r=umount2(b, MNT_FORCE);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);NSP_UPG_PRINTF("OK\n");

 /* Unmount flash */
 NSP_UPG_PRINTF("Unmounting %s...",NSP_UPG_FLASH_DIR);
 r=umount2(NSP_UPG_FLASH_DIR, MNT_FORCE);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);NSP_UPG_PRINTF("OK\n");
 return(NSP_UPG_ERR_OK);
}






