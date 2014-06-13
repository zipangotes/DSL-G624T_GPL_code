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
#ifndef ___NSP_UPG_ERROR_H___
#define	___NSP_UPG_ERROR_H___

typedef	enum nsp_upg_err
{
	NSP_UPG_ERR_OK		= 0,	/* No error */
	NSP_UPG_ERR_ERROR	= -1	/* Unknown error */
} nsp_upg_err;

extern char* nsp_upg_err_get(void);
extern int nsp_upg_err_print(void);
extern int nsp_upg_err_printf(char* f,...);
extern int nsp_upg_err_sprintf(char* b,char* f,...);

extern nsp_upg_err nsp_upg_error;

#endif /* ___NSP_UPGMSG_H___ */




