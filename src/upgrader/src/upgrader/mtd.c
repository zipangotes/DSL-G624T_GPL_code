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
#include "nspimghdr.h"
#include <ctype.h>

#define	NSP_UPG_MTDSET_MODE0		0x1
#define	NSP_UPG_MTDSET_MODE1		0x2
#define	NSP_UPG_MTDSET_MODE_BOTH	(0x1|0x2)

/* **************************************************************************
	int nsp_upg_mtdset(char *n,char *ki,char *fi,char *si,unsigned int mode)

	This function sets bootloader environment variables (MTD0,MTD1,MTD4)
	to the proper values based on the information in the firmware
	upgrade file. The user must pass a complete path to the file, otherwise
	the MTD variables will not be set.

	Arguments:
		n					- name
		ki					- kernel image mtd
		fi					- file image mtd
		si					- single image mtd
		mode				- mode
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
static int nsp_upg_mtdset(char *n,char *ki,char *fi,char *si,unsigned int mode)
{
 char b[256];char *p;NSP_IMG_HDR nspimghdr;int r;unsigned int f,t,F,T;FILE *i;
 /*NSP_UPG_PRINTF("Setting MTD variables...\n");*/
 i=fopen(n,"rb");if(i==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);NSP_UPG_PRINTF("Can't open %s\n",n);return(NSP_UPG_ERR_ERROR);}
 r=fread(&nspimghdr,sizeof(NSP_IMG_HDR),1,i);fclose(i);if(r!=1){NSP_UPG_PRINTF("Can't read %s\n",n);return(NSP_UPG_ERR_ERROR);}
 if(nspimghdr.id.magic!=SIMAGE_MAGIC_NUMBER){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);NSP_UPG_PRINTF("Wrong magic number 0x%x != 0x%x.\n",nspimghdr.id.magic,SIMAGE_MAGIC_NUMBER);return(NSP_UPG_ERR_ERROR);}
 memset(b,0,sizeof(b));r=nsp_upg_getenv(si,b,sizeof(b));
 if(r<0)
 {
   /*NSP_UPG_PRINTF("MTD4 was NOT set.\n");*/
   r=nsp_upg_getenv(ki,b,sizeof(b));NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
   r=nsp_upg_hex2int(b,&f);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
   p=strchr(b,NSP_UPG_MTD_DIVIDER);if(p==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
   p++;r=nsp_upg_hex2int(p,&t);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
   r=nsp_upg_getenv(fi,b,sizeof(b));NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
   r=nsp_upg_hex2int(b,&F);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
   p=strchr(b,NSP_UPG_MTD_DIVIDER);if(p==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
   p++;r=nsp_upg_hex2int(p,&T);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);

   /* MTD4 = min(MTD1,MTD0),max(MTD1,MTD0) */
   f=(f<F)?f:F;t=(t>T)?t:T;
 }
 else
 {
  /*NSP_UPG_PRINTF("MTD4 was set to %s\n",b);*/
  r=nsp_upg_hex2int(b,&f);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
  p=strchr(b,NSP_UPG_MTD_DIVIDER);if(p==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
  p++;r=nsp_upg_hex2int(p,&t);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
 }
 /* align the range on the block boundary */
 f&=0xFFFF0000;t&=0xFFFF0000;/*NSP_UPG_PRINTF("MTD pair: 0x%x,0x%x\n",f,t);*/

 /* check if the image fits into flash */
 i=fopen(n,"rb");if(i==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}r=fseek(i,0,SEEK_END);if(r<0){fclose(i);NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
 F=ftell(i);fclose(i);NSP_UPG_ERR_RETURN(F,NSP_UPG_ERR_ERROR);if(F>(t-f)){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
 sprintf(b,"0x%x,0x%x",f,t);r=nsp_upg_setenv(si,b);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);NSP_UPG_PRINTF("MTD4 = %s\n",b);

 /* calculate new MTD1 */
 if(mode&NSP_UPG_MTDSET_MODE0){F=f+nspimghdr.desc[0].sec_offset;T=f+nspimghdr.desc[1].sec_offset;}
 else
 {
   r=nsp_upg_getenv(ki,b,sizeof(b));NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
   r=nsp_upg_hex2int(b,&F);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
   p=strchr(b,NSP_UPG_MTD_DIVIDER);if(p==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
   p++;r=nsp_upg_hex2int(p,&T);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
   F&=0xFFFF0000;T&=0xFFFF0000;
 }
 r=sprintf(b,"0x%x,0x%x",F,T);r=nsp_upg_setenv(ki,b);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);NSP_UPG_PRINTF("MTD1 = %s\n",b);
 
 /* calculate new MTD0 */
 if(mode&NSP_UPG_MTDSET_MODE1){F=f+nspimghdr.desc[1].sec_offset;T=t;}
 else
 {
   r=nsp_upg_getenv(fi,b,sizeof(b));NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
   r=nsp_upg_hex2int(b,&F);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
   p=strchr(b,NSP_UPG_MTD_DIVIDER);if(p==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
   p++;r=nsp_upg_hex2int(p,&T);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
   F&=0xFFFF0000;T&=0xFFFF0000;
 }
 r=snprintf(b,sizeof(b),"0x%x,0x%x",F,T);r=nsp_upg_setenv(fi,b);if(r<0){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}NSP_UPG_PRINTF("MTD0 = %s\n",b);

 NSP_UPG_ERR_SET(NSP_UPG_ERR_OK);
 return(NSP_UPG_ERR_OK);
}

/* **************************************************************************
	int nsp_upg_msg_setmtds(char* s, char* d)

	This function sets bootloader environment variables (MTD0,MTD1,MTD4)
	to the proper values based on the information in the firmware
	upgrade file. The user must pass a complete path to the file.

	Arguments:
		s					- source
		d					- destination
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_msg_setmtds(char* s, char* d)
{
 int r,f;unsigned int m;
 /* try to open mtd4; if it is not there, we had a split upgrade */
 f=open(NSP_UPG_MTD_SBLOCK,O_RDONLY);
 if(f<0){r=nsp_upg_mtdset(s,NSP_UPG_MTD_KERNEL,NSP_UPG_MTD_FILESYS,NSP_UPG_MTD_SIMAGE,0);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);}
 else
 {
   /* we drop here ONLY when mtd4 was defined */
   r=read(f,&m,4);close(f);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);if(r<4){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
   /* check if the device contains our image; if not, we aren't going to change mtd0 and mtd1 */
   if(m!=SIMAGE_MAGIC_NUMBER){r=nsp_upg_mtdset(s,NSP_UPG_MTD_KERNEL,NSP_UPG_MTD_FILESYS,NSP_UPG_MTD_SIMAGE,0);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);}
   else{r=nsp_upg_mtdset(s,NSP_UPG_MTD_KERNEL,NSP_UPG_MTD_FILESYS,NSP_UPG_MTD_SIMAGE,NSP_UPG_MTDSET_MODE_BOTH);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);}
 }
 NSP_UPG_ERR_SET(NSP_UPG_ERR_OK);return(NSP_UPG_ERR_OK);
}



