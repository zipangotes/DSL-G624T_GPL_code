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
#ifndef _NSPIMGHDR_H_
#define _NSPIMGHDR_H_

/* ************************************************************************
*	NSP Image Identification Header
*************************************************************************** */
typedef struct NSP_IMG_HDR_ID
{
	unsigned int	magic;			/* Magic number to identify the file */
	unsigned int	prod_id;		/* Product ID number (filled at release time by CM) */
	unsigned int	rel_id;			/* Release ID number (filled at release time by CM) */
	unsigned int	flags;			/* Flags (filled at build time) */
	unsigned int	hdr_size;		/* NSP_IMG_HDR size */
	unsigned int	img_size;		/* Size of the file minus sizeof(NSP_IMG_HDR) */
	char			prod_info[64];	/* Product information string */
} NSP_IMG_HDR_ID;

/* ************************************************************************
*	NSP Image Integrity Header
*************************************************************************** */
typedef struct NSP_IMG_HDR_ITG
{
	unsigned int	hdr_chksum;		/* Header checksum */
	unsigned int	img_chksum;		/* Image checksum */
} NSP_IMG_HDR_ITG;

/* ************************************************************************
*	NSP Boot Header
*************************************************************************** */
typedef struct NSP_IMG_HDR_BOOT
{
	unsigned int	id;				/* Section ID that contains the bootable image */
	unsigned int	dram_addr;		/* DRAM address where to copy the boot image */
	unsigned int	dram_size;		/* Number of bytes to copy from boot image to DRAM */
	unsigned int	jump_offset;	/* Offset from the DRAM address where to jump in order to boot */
} NSP_IMG_HDR_BOOT;

/* ************************************************************************
*	NSP Section Descriptor
*************************************************************************** */
typedef struct NSP_IMG_HDR_SECDESC
{
	unsigned int	sec_offset;		/* Section Offset from the start of the NSP_IMG_HDR */
	unsigned int	sec_size;		/* Section Size */
	unsigned int	sec_flags;		/* Section Flags */
	unsigned int	sec_chksum;		/* Section Checksum (based on 'sec_size' number of bytes) */
} NSP_IMG_HDR_SECDESC;

/* ************************************************************************
*	NSP Image Header
*************************************************************************** */
typedef struct NSP_IMG_HDR
{
	NSP_IMG_HDR_ID		id;			/* Identification */
	NSP_IMG_HDR_ITG		itg;		/* Integrity */
	NSP_IMG_HDR_BOOT	boot;		/* Boot */
	NSP_IMG_HDR_SECDESC desc[2];	/* Section Descriptors */
} NSP_IMG_HDR;

/* ************************************************************************
*	NSP Image Header Convenience Macros
*************************************************************************** */
#define	NSP_IMG_KOFFSET(h)	((h).desc[0].sec_offset)
#define	NSP_IMG_FOFFSET(h)	((h).desc[1].sec_offset)
#define	NSP_IMG_KSIZE(h)	((h).desc[0].sec_size)
#define	NSP_IMG_FSIZE(h)	((h).desc[1].sec_size)

#endif

