/* -*- Mode: C; tab-width: 2; indent-tabs-mode: t; c-basic-offset: 2 -*-
 * File: cfgmgr_cksum.h
 * Created: Wed Aug 14 18:25:30 2002
 * $Id: cm_cksum.h,v 1.1 2003/12/13 18:27:30 jharrell Exp $
 *
 * Copyright (c) 2002 Texas Instruments Incorporated
 * ALL RIGHTS RESERVED
 *
 * This computer program is subject to a separate license agreement signed
 * by Texas Instruments Incorporated and the licensee, and is subject to the
 * restrictions therein.  No other rights to reproduce, use, or disseminate
 * this computer program, whether in part or in whole, are granted.
 */

#ifndef CFGMGR_CKSUM_H
#define CFGMGR_CKSUM_H

#include <inttypes.h>
#include <stdio.h>
#include <errno.h>

int cs_is_tagged(FILE*);
unsigned long cs_read_sum(FILE*);
int cs_calc_sum(FILE*, unsigned long*, int);
int cs_set_sum(FILE*, unsigned long, int);
void cs_get_sum(FILE*, unsigned long*);
unsigned long cs_calc_buf_sum(char*, int);

#endif
