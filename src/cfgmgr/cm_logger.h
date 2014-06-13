/* -*- Mode: C; tab-width: 2; indent-tabs-mode: t; c-basic-offset: 2 -*-
 * File: cm_logger.h
 *
 * Created: Fri Dec 20 13:57:11 2002
 *
 * $Id: cm_logger.h,v 1.2 2003/12/12 19:49:25 jharrell Exp $
 */

#ifndef CM_LOGGER_H
#define CM_LOGGER_H

#include <syslog.h>

#include "cm_module.h"

#ifdef	DEVELOPER
#define DLOGGER(...)	logger(__VA_ARGS__)
#define DLOGGER2(...)	logger2(__VA_ARGS__)
#else
#define DLOGGER(...)	while(0){;}
#define DLOGGER2(...)	while(0){;}
#endif

int logger(struct cm_module *mod, int facility, int level, const char *text, ...);

#endif
