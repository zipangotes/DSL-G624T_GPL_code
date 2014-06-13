/* -*- Mode: C; tab-width: 2; indent-tabs-mode: t; c-basic-offset: 2 -*-
 * File: comm.h
 *
 * Created: Fri Dec  6 13:36:32 2002
 *
 * $Id: cm_comm.h,v 1.2 2003/12/12 19:49:25 jharrell Exp $
 */

#ifndef CM_LIB_COMM_H
#define CM_LIB_COMM_H

/* The sys/un.h file does not define this. It is defined in the linux/un.h,
 * we don't want to use that one
 */
#define UNIX_PATH_MAX	108

#include <sys/un.h>
#include <sys/socket.h>

#include "cm_params.h"

#define CM_MAX_MESSAGE_LEN	4096

#define CM_SOCKET_PATH "/var/tmp"

#define COMM_FAIL_SOCK			-1
#define COMM_FAIL_BIND			-2
#define COMM_FAIL_SECOND_INSTANCE	-3
#define COMM_FAIL_UNLINK		-4

int comm_start(char *name, int send_fl);
int comm_unlink(char *name);
int comm_sendto(int fd, const void *msg, size_t len, char *name);
#define comm_recv(fd, msg, len, flags) recv(fd, msg, len, flags)
#define comm_recvfrom(fd, msg, len, flags, from, fromlen) recvfrom(fd, msg, len, flags, from, fromlen)

#endif
