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

/* **************************************************************************
	int nsp_upg_setenv(char* n,char* v)

	This application upgrades flash memory.

	Arguments:
		argc				- number of command line arguments
		argv				- array of arguments
		envp				- array of environment variables
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int main(int argc, char* argv[], char* envp[])
{
 int r;
#ifdef NSP_UPG_DEBUG
 int i;char v[NSP_UPG_MAX_NAMELEN];char n[64];
#endif
 sleep(1);NSP_UPG_PRINTF("\n\n");
  NSP_UPG_PRINTF("****************** NSP Firmware Upgrade ******************\n");sleep(1);
 /* Check if we have all directories in place */
 r=nsp_upg_dirchk("/var/proc");NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
 r=nsp_upg_dirchk("/var/dev");NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
 r=nsp_upg_dirchk("/var/flash");NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);

 /* Change ROOT to /var so we can unplug flash */
 r=nsp_upg_change_root("/var");
 if(!(r<0))
 {
   NSP_UPG_PRINTF("Upgrading flash memory...\n",n);
   r=nsp_upg_msgqread(NSP_UPG_QUEUE_ID);
   if(r<0){NSP_UPG_PRINTF("*** FATAL ERROR. Use PC App now. ***\n");sleep(1);}
#ifdef NSP_UPG_DEBUG
   for(i=0;i<=4;i++){sprintf(n,"mtd%d",i);r=nsp_upg_getenv(n,v,sizeof(v));if(!(r<0)){NSP_UPG_PRINTF("%s = %s\n", n, v);};}
#endif
 }
 else{NSP_UPG_PRINTF("*** FATAL ERROR: modem is dead. Use PC App. ***\n");}
 nsp_upg_reboot();
 return(NSP_UPG_ERR_OK);
}

