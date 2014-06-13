/* -*- Mode: C; tab-width: 2; indent-tabs-mode: t; c-basic-offset: 2 -*-
 * File: base64.h
 *
 * Created: Tue Mar 11 18:11:14 2003
 *
 * $Id: cm_base64.h,v 1.2 2003/12/12 19:49:25 jharrell Exp $
 */

#ifndef CM_BASE64_H
#define CM_BASE64_H

char* io_b64enc(const char* chunk, int size);
char* io_b64dec(const char* string, int *size);

#endif
