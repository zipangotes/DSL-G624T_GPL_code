/* -*- Mode: C; tab-width: 2; indent-tabs-mode: t; c-basic-offset: 2 -*-
 * File: io.h
 *
 * Created: Mon Dec 16 14:32:31 2002
 *
 * $Id: io.h,v 1.2 2003/12/12 19:49:25 jharrell Exp $
 */

#ifndef CM_IO_H
#define CM_IO_H

#define CMIO_MAGICNUM   0x434D4D4C // CMML
#define CMIO_PLAIN      0x0200
#define CMIO_COMPRESSED 0x0300

#define BLOCK_SIZE (1 << 16)

//#define MTD_CONFIG	"/dev/mtdblock/3"

int io_load_mtd(char *mtdname, char **buf, unsigned long *full_size);
int io_load_mtd_header(char *mtdname, char **buf, unsigned long *full_size);
int io_save_mtd(char *mtdname, char *buf, int orig_size);
int io_save_mtd_header(char *mtdname, char *buf, int size);
int io_clear_mtd(char *mtdname);
void io_restore_defaults(char *mtdname);
void io_reboot();

#endif
