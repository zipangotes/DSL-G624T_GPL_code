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

nsp_upg_err nsp_upg_error;

static char* nsp_upg_err_map[]=
{
	"No error"
};

/* **************************************************************************
	char* nsp_upg_err_get(void)

	This function returns an ASCII string that describes the last error.

	Arguments:
		none
	Returns:
		!NULL	- success
		NULL	- failure
	Errors:

***************************************************************************** */
char* nsp_upg_err_get(void)
{
 char* m;
 if(nsp_upg_error<0){return(NULL);}
 if(nsp_upg_error>=sizeof(nsp_upg_err_map)/sizeof(char*)){return(NULL);};
 m=nsp_upg_err_map[nsp_upg_error];nsp_upg_error=NSP_UPG_ERR_OK;
 return(m);
}

/* **************************************************************************
	int nsp_upg_err_print(void)

	This function prints an ASCII string describing the last error.

	Arguments:
		none
	Returns:
		NSP_UPG_ERR_OK	- success
	Errors:

***************************************************************************** */
int nsp_upg_err_print(void)
{
	return(NSP_UPG_ERR_OK);
}
