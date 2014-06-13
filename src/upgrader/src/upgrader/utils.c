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

static int	nspc=-1;
static char msgbuf[1024];

/* **************************************************************************
	int nsp_upg_printf_close(void)

	This function closes the NSP upgrader print queue.

	Arguments:
		none
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_printf_close(void)
{
 if(!(nspc<0)){close(nspc);nspc=-1;}
 return(NSP_UPG_ERR_OK);
}

/* **************************************************************************
	int nsp_upg_printf(char* f,...)

	This function closes the NSP upgrader print queue.

	Arguments:
		f					- format string
		...					- variable number of arguments
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_printf(char* f,...)
{
 int w,s,r;va_list a;
 w=0;s=0;va_start(a,f);vsnprintf(msgbuf,sizeof(msgbuf),f,a);va_end(a);
 if(nspc<0){nspc=open(NSP_UPG_CONSOLE,O_WRONLY|O_SYNC);}if(nspc<0){return(NSP_UPG_ERR_ERROR);}
 s=strlen(msgbuf);while(w!=s){r=write(nspc,&msgbuf[w],s-w);if(r<0){return(NSP_UPG_ERR_ERROR);};w+=r;}
 return(NSP_UPG_ERR_OK);
}

/* **************************************************************************
	void nsp_upg_closeall(void)

	This function closes all file descriptors.

	Arguments:
		none
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
void nsp_upg_closeall(void)
{
 int f;
 for(f=0;f<NSP_UPG_MAXFD;f++){close(f);}
}


/* **************************************************************************
	int nsp_upg_hex2dig(char c)

	This function converts a hex digit into binary.

	Arguments:
		none
	Returns:
		positive value		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_hex2dig(char c)
{
 if((c>='0')&&(c<='9')){return(c-'0');}
 else{if((c>='A')&&(c<='F')){return(c-'A'+10);}else{if((c>='a')&&(c<='f')){return(c-'a'+10);}}}
 NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);
 return(NSP_UPG_ERR_ERROR);
}

/* **************************************************************************
	int nsp_upg_hex2int(char *b, unsigned int *v)

	This function converts a hex number into binary integer.

	Arguments:
		b					- hex ASCII
		v					- result value
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_hex2int(char *b, unsigned int *v)
{
 unsigned int r;int i;char *p;
 r=0;if(b==NULL||v==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);};
 p=strchr(b,'x');if(p==NULL){NSP_UPG_ERR_SET(-1);return(NSP_UPG_ERR_ERROR);};p++;
 while(1){i=nsp_upg_hex2dig(*p);if(i<0){break;}else{r=r*16+i;};p++;};*v=r;
 NSP_UPG_ERR_SET(NSP_UPG_ERR_OK);return(NSP_UPG_ERR_OK);
}

