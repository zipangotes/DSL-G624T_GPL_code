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

/* local definitions */
#define	NSP_UPG_ENV_STATE_START	0
#define	NSP_UPG_ENV_STATE_NAME	1
#define	NSP_UPG_ENV_STATE_VALUE	2

#define	NSP_UPG_ENV_YES			1
#define	NSP_UPG_ENV_NO			0

#define	NSP_UPG_ENV_DATA_START	0
#define	NSP_UPG_ENV_MORE_LINES	1

/* **************************************************************************
	int nsp_upg_setenv(char* n,char* v)

	This function sets the value of the requested environment variable to
	the specified value.

	Arguments:
		n					- variable name
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_setenv(char* n,char* v)
{
 char *s; int f,l,w,r;
 s=(char*)malloc(strlen(n)+strlen(v)+2);if(s==NULL){return(NSP_UPG_ERR_ERROR);};
 l=0;w=0;r=0;f=open(NSP_UPG_BOOT_ENV,O_WRONLY);if(f<0){return(f);};
 strcpy(s,n);if(v!=NULL&&v[0]!=NSP_UPG_EOL){strcat(s," ");strcat(s,v);};l=strlen(s)+1;
 while(w!=l){r=write(f,&s[w],l-w);if(r<0){close(f);return(NSP_UPG_ERR_ERROR);}else{w+=r;}}free(s);close(f);
 return(NSP_UPG_ERR_OK);
}

/* **************************************************************************
	int nsp_upg_unsetenv(char* n)

	This function unsets the value of the requested environment variable.

	Arguments:
		n					- variable name
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_unsetenv(char* n)
{
 int	r;char b[1];
 r=nsp_upg_getenv(n,NULL,0);if(r<0){return(NSP_UPG_ERR_OK);}
 b[0]=NSP_UPG_EOL;r=nsp_upg_setenv(n,b);if(r<0){return(r);};
 return(NSP_UPG_ERR_OK);
}

/* **************************************************************************
	int nsp_upg_getenv(char* n,char v[],int vs)

	This function returns the value of the requested environment variable.

	Arguments:
		n					- variable name
		v					- value
		vs					- value size
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_getenv(char* n,char v[],int vs)
{
 char *a,*b; int c,d,e,f,i,l,p,s,t,nl,vl;
 a=(char*)malloc(NSP_UPG_BOOT_ENV_SZ);if(a==NULL){return(NSP_UPG_ERR_ERROR);}
 b=(char*)malloc(NSP_UPG_BOOT_ENV_VSZ);if(b==NULL){free(a);return(NSP_UPG_ERR_ERROR);}
 /* the environment block is not seekable, so don't try anything fancy */
 f=open(NSP_UPG_BOOT_ENV,O_RDONLY);l=(f<0)?(NSP_UPG_ERR_ERROR):read(f,a,NSP_UPG_BOOT_ENV_SZ);if(!(f<0)){close(f);};if(l<0){free(a);free(b);return(l);};
 p=NSP_UPG_ENV_DATA_START;f=NSP_UPG_ENV_NO;e=NSP_UPG_ENV_NO;d=NSP_UPG_ENV_DATA_START;s=NSP_UPG_ENV_STATE_NAME;nl=NSP_UPG_ENV_DATA_START;vl=NSP_UPG_ENV_DATA_START;
 /* process the whole block one character at a time */
 while(NSP_UPG_ENV_MORE_LINES)
 {
  /* finished parsing? if not, stub the location of the next character for safety with the line end */
  if(f||e){break;};if(d>=NSP_UPG_BOOT_ENV_VSZ){free(a);free(b);return(NSP_UPG_ERR_ERROR);}if(p>=l){e=NSP_UPG_ENV_YES;continue;}else{c=a[p++];};b[d]=NSP_UPG_EOL;
  /* check what this character is */
  switch(c)
  {
   /* found end of line */
   case NSP_UPG_EOL:case NSP_UPG_LF:case NSP_UPG_CR:
    switch(s)
	{
	 /* consider every state */
     case NSP_UPG_ENV_STATE_START:s=NSP_UPG_ENV_STATE_NAME;d=NSP_UPG_ENV_DATA_START;continue;break;
     case NSP_UPG_ENV_STATE_NAME:if(nl){if(!strcmp(n,b)){vl=NSP_UPG_ENV_DATA_START;f=NSP_UPG_ENV_YES;d=NSP_UPG_ENV_DATA_START;if((vs>0)&&(v!=NULL)){v[0]=NSP_UPG_EOL;};continue;}else{vl=NSP_UPG_ENV_DATA_START;nl=NSP_UPG_ENV_DATA_START;d=NSP_UPG_ENV_DATA_START;continue;}}else{continue;};break;
     case NSP_UPG_ENV_STATE_VALUE:if(vl){if((vs>0)&&(v!=NULL)){t=(vl>=vs)?(vs-1):vl;for(i=0;i<t;i++){v[i]=b[i];};v[t]=NSP_UPG_EOL;};free(a);free(b);return(0);}else{f=NSP_UPG_ENV_YES;if((vs>0)&&(v!=NULL)){v[0]=NSP_UPG_EOL;};continue;} break;
     default:break;
	} break;
   /* found a space */
   case NSP_UPG_TAB:case NSP_UPG_SPACE:
	d=NSP_UPG_ENV_DATA_START;
    switch(s)
	{
	 /* consider every state */
     case NSP_UPG_ENV_STATE_START:nl=NSP_UPG_ENV_DATA_START;vl=NSP_UPG_ENV_DATA_START;break;
     case NSP_UPG_ENV_STATE_NAME:if(nl){if(!strcmp(n,b)){s=NSP_UPG_ENV_STATE_VALUE;vl=NSP_UPG_ENV_DATA_START;continue;}else{s=NSP_UPG_ENV_STATE_START;vl=NSP_UPG_ENV_DATA_START;nl=NSP_UPG_ENV_DATA_START;continue;}}break;
     case NSP_UPG_ENV_STATE_VALUE:continue;break;
     default:break;
	} break;
   /* found something else */
   default:
    switch(s)
    {
	 /* consider every state */
     case NSP_UPG_ENV_STATE_START:d=NSP_UPG_ENV_DATA_START;nl=NSP_UPG_ENV_DATA_START;vl=NSP_UPG_ENV_DATA_START;break;
     case NSP_UPG_ENV_STATE_NAME:nl++;break;
     case NSP_UPG_ENV_STATE_VALUE:vl++;break;
     default:break;
    }
   /* save it */
   b[d++]=c;break;
  }
}
 /* free memory and return the result */
 s=(f==NSP_UPG_ENV_YES)?(NSP_UPG_ERR_OK):(NSP_UPG_ERR_ERROR);free(a);free(b);return(s);
}


