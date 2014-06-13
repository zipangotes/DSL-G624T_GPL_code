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

/* **************************************************************************
	int nsp_upg_upgrade_split(char* n)

	This function performs a "split" upgrade, i.e. it takes a single NSP
	image, splits it into kernel and file system parts, and programs flash.

	Arguments:
		n					- NSP image name
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_upgrade_split(char* n)
{
 int o,c,w,m,d,i,s,r,f,t,F,T;FILE *h;NSP_IMG_HDR nspimghdr;
 char b[NSP_UPG_MAX_NAMELEN],*p,P[NSP_UPG_PROGRESS_COUNT+1];

 /* take care of the locals */
 o=0;c=0;w=0;m=0;d=0;i=0;s=0;r=0;f=0;t=0;F=0;T=0;
 nsp_upg_printf("Split upgrade...\n");

 /* see if the destination mtdblocks are there */
 d=open(NSP_UPG_MTD_KBLOCK,O_RDONLY);if(d<0){return(NSP_UPG_ERR_ERROR);}close(d);
 d=open(NSP_UPG_MTD_FBLOCK,O_RDONLY);if(d<0){return(NSP_UPG_ERR_ERROR);}close(d);

 /* read image header in */
 h=fopen(n,"rb");if(h==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
 r=fread(&nspimghdr,sizeof(NSP_IMG_HDR),1,h);fclose(h);if(r!=1){return(NSP_UPG_ERR_ERROR);}
 
 /* see if the kernel piece fits into mtd1 */
 r=nsp_upg_getenv(NSP_UPG_MTD_KERNEL,b,sizeof(b));
 if(!(r<0))
 {
  r=nsp_upg_hex2int(b,&f);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
  p=strchr(b,',');if(p==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
  p++;r=nsp_upg_hex2int(p,&t);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
  if(NSP_IMG_KSIZE(nspimghdr)>(t-f)){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
 }

 /* see if the file system piece fits into mtd0 */
 r=nsp_upg_getenv(NSP_UPG_MTD_FILESYS,b,sizeof(b));
 if(!(r<0))
 {
  r=nsp_upg_hex2int(b,&F);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
  p=strchr(b,',');if(p==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
  p++;r=nsp_upg_hex2int(p,&T);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
  if(NSP_IMG_FSIZE(nspimghdr)>(T-F)){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
 }

 /* copy the kernel piece into mtd1 */
 d=open(NSP_UPG_MTD_KBLOCK,O_WRONLY|O_SYNC);if(d<0){nsp_upg_printf("mtd4 was in there, but got removed!\n");return(NSP_UPG_ERR_ERROR);}
 h=fopen(n,"rb");if(h==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
 r=fseek(h,NSP_IMG_KOFFSET(nspimghdr),SEEK_SET);if(r<0){return(NSP_UPG_ERR_ERROR);}w=0;
 memset(P,NSP_UPG_SPACE,sizeof(P));P[NSP_UPG_PROGRESS_COUNT]='\0';c=0;
 for(s=0;s<NSP_IMG_KSIZE(nspimghdr);s+=w)
 {
  /* read and write data */
  r=fread(&b,1,sizeof(b),h);if(r<0){fclose(h);close(d);return(NSP_UPG_ERR_ERROR);};w=0;
  while(w!=r){o=write(d,&b[w],r-w);if(o<0){fclose(h);close(d);return(NSP_UPG_ERR_ERROR);};w+=o;m+=o;}
  if(m>=(NSP_IMG_KSIZE(nspimghdr)/NSP_UPG_PROGRESS_COUNT)){P[c++]=NSP_UPG_PROGRESS_MARK;nsp_upg_printf(NSP_UPG_PROGRESS_FORMAT,"Kernel",P,c*100/NSP_UPG_PROGRESS_COUNT);m=0;}
 }
 close(d);fclose(h);if(c<=NSP_UPG_PROGRESS_COUNT){P[c]=NSP_UPG_PROGRESS_MARK;nsp_upg_printf(NSP_UPG_PROGRESS_FORMAT,"Kernel",P,100);}nsp_upg_printf("\n");

 /* copy the file system piece into mtd0 */
 d=open(NSP_UPG_MTD_FBLOCK,O_WRONLY|O_SYNC);if(d<0){nsp_upg_printf("mtd4 was in there, but got removed!\n");return(NSP_UPG_ERR_ERROR);}

 h=fopen(n,"rb");if(h==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}

 fseek(h,NSP_IMG_FOFFSET(nspimghdr),SEEK_SET);if(r<0){return(NSP_UPG_ERR_ERROR);};w=0;

 memset(P,NSP_UPG_SPACE,sizeof(P));P[NSP_UPG_PROGRESS_COUNT]='\0';c=0;
 for(s=0;s<NSP_IMG_FSIZE(nspimghdr);s+=w)
 {
  /* read and write data */
  r=fread(&b,1,sizeof(b),h);if(r<0){fclose(h);close(d);return(NSP_UPG_ERR_ERROR);}w=0;
  while(w!=r){o=write(d,&b[w],r-w);if(o<0){fclose(h);close(d);return(NSP_UPG_ERR_ERROR);}w+=o;m+=o;}
  if(m>=(NSP_IMG_FSIZE(nspimghdr)/NSP_UPG_PROGRESS_COUNT)){P[c++]=NSP_UPG_PROGRESS_MARK;nsp_upg_printf(NSP_UPG_PROGRESS_FORMAT,"SQFS  ",P,c*100/NSP_UPG_PROGRESS_COUNT);m=0;}
 }
 close(d);fclose(h);if(c<=NSP_UPG_PROGRESS_COUNT){P[c]=NSP_UPG_PROGRESS_MARK;nsp_upg_printf(NSP_UPG_PROGRESS_FORMAT,"SQFS  ",P,100);}nsp_upg_printf("\n");
 return(NSP_UPG_ERR_OK);
}

/* **************************************************************************
	int nsp_upg_msg_flash(char* n)

	This function executes FLASH message type.

	Arguments:
		n					- NSP image name
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int nsp_upg_msg_flash(char* n)
{
 FILE* h;NSP_IMG_HDR nspimghdr;int s,d,r,F,T;char b[NSP_UPG_MAX_NAMELEN],*p;
 
 /* read image header in and verify this is an NSP firmware image */
 h=fopen(n,"rb");if(h==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
 r=fread(&nspimghdr,sizeof(NSP_IMG_HDR),1,h);fclose(h);if(r!=1){return(NSP_UPG_ERR_ERROR);}
 if(nspimghdr.id.magic!=SIMAGE_MAGIC_NUMBER){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);nsp_upg_printf("Wrong NSP magic number.\n");return(NSP_UPG_ERR_ERROR);}

 /* see if mtd4 is defined */
 r=nsp_upg_getenv(NSP_UPG_MTD_SIMAGE,b,sizeof(b));
 if(!(r<0))
 {
  /* see if the destination mtdblock is there and the new image fits into it */
  d=open(NSP_UPG_MTD_SBLOCK,O_WRONLY);if(d<0){r=nsp_upg_upgrade_split(n);return(r);}close(d);
  h=fopen(n,"rb");if(h==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
  r=fseek(h,0,SEEK_END);if(r<0){fclose(h);NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
  s=ftell(h);fclose(h);NSP_UPG_ERR_RETURN(F,NSP_UPG_ERR_ERROR);
  r=nsp_upg_hex2int(b,&F);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
  p=strchr(b,',');if(p==NULL){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);return(NSP_UPG_ERR_ERROR);}
  p++;r=nsp_upg_hex2int(p,&T);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
  if(s>(T-F)){NSP_UPG_ERR_SET(NSP_UPG_ERR_ERROR);nsp_upg_printf("Upgrade image does not fit!\n");return(NSP_UPG_ERR_ERROR);}
  r=nsp_upg_msg_copy(n,NSP_UPG_MTD_SBLOCK);NSP_UPG_ERR_RETURN(r,NSP_UPG_ERR_ERROR);
 }
 else {r=nsp_upg_upgrade_split(n);return(r);}
 return(NSP_UPG_ERR_OK);
}


