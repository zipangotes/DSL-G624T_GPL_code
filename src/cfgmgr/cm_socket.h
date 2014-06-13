/* -*- Mode: C; tab-width: 2; indent-tabs-mode: t; c-basic-offset: 2 -*-
 * File: socket.h
 *
 * Created: Wed Dec 11 15:44:11 2002
 *
 * $Id: cm_socket.h,v 1.2 2003/12/12 19:49:25 jharrell Exp $
 */

#ifndef CM_SOCKET_H
#define CM_SOCKET_H

typedef struct _sSocket
{
	int sock;
	struct _sSocket *next;
	struct _sSocket *prev;
} sSocket;

sSocket *sock_new(int);
void sock_free(sSocket*);

#endif
