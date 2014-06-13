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
	int nsp_upg_msg_copy(char* f,char* t)

	This function copies source to destination.

	Arguments:
		f					- source
		t					- destination
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_msg_copy(char* f,char* t)
{
 int s,d,r,w,o,i,c,m;char b[512],P[NSP_UPG_PROGRESS_COUNT+1];FILE *p;
 c=0;m=0;i=0;s=0;d=0;r=0;w=0;o=0;
 p=fopen(f,"rb");if(p==NULL){return(NSP_UPG_ERR_ERROR);};r=fseek(p,0,SEEK_END);if(r<0){fclose(p);return(NSP_UPG_ERR_ERROR);};i=ftell(p);fclose(p);NSP_UPG_ERR_RETURN(i,NSP_UPG_ERR_ERROR);
 memset(P,' ',sizeof(P));P[NSP_UPG_PROGRESS_COUNT]='\0';
 s=open(f,O_RDONLY);NSP_UPG_ERR_RETURN(s,NSP_UPG_ERR_ERROR);d=open(t,O_WRONLY|O_SYNC);if(d<0){close(s);return(NSP_UPG_ERR_ERROR);}
 /*nsp_upg_printf("Copying %s => %s. Every mark is %d bytes.\n",f,t,(i/NSP_UPG_PROGRESS_COUNT));*/
 while((r=read(s,b,sizeof(b)))>0)
 {
  w=0;while(r!=w)
  {
   o=write(d,&b[w],r-w);if(o<0){close(s);close(d);return(NSP_UPG_ERR_ERROR);}w+=o;m+=o;
   if(m>=(i/NSP_UPG_PROGRESS_COUNT)){P[c++]=NSP_UPG_PROGRESS_MARK;nsp_upg_printf(NSP_UPG_PROGRESS_FORMAT,"Progress",P,c*100/NSP_UPG_PROGRESS_COUNT);m=0;}
  }
 }
 if(c<=NSP_UPG_PROGRESS_COUNT){P[c]=NSP_UPG_PROGRESS_MARK;nsp_upg_printf(NSP_UPG_PROGRESS_FORMAT,"Progress",P,100);}
 nsp_upg_printf("\n");close(d);close(s);
 return(NSP_UPG_ERR_OK);
}


