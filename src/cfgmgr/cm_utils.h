#ifndef	__CM_UTILS_H__
#define	__CM_UTILS_H__

#include <stdio.h>
#include <stdlib.h>

#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <string.h>

#include "cm_params.h"

#define strcmp(a,b) Please_use_safe_strcmp
#define strncmp(a,b,c) Please_use_safe_strncmp

#define STRING_MAX_LEN	MAX_LMSG_LEN
//#define STRING_MAX_LEN	2*MAX_LMSG_LEN

//struct string *make_string(char *txt, ...);
//struct string *make_string_exact(char *txt);

#define STRNCPY(dest, src, n)									\
	{ strncpy(dest, src, n); \
		dest[n-1] = 0; \
	}

/* The following functions are used for doublely linked
 * lists. It will use any structure, as long as it has
 * a next and prev pointer.
 */

#define NODE_ADD(head, node)	\
	{	node->next=head;		\
		node->prev=NULL;		\
		if(head) (head)->prev=node;	\
		head=node;			\
	}

/* This enqueues a node to the end of a list. As we don't have a temp node,
   we fake it out by using the node's next variable, which will be NULL, but
   is of the correct type */
#define NODE_ENQUEUE(head, node)	\
	{	node->next=head;				\
		if(node->next)					\
			while(node->next->next)			\
				node->next=node->next->next;	\
		node->prev=node->next;				\
		node->next=NULL;				\
		if(!head) head=node;				\
		else node->prev->next=node;				\
	}

#define NODE_DEL(head, node)	\
	{	if(node->next) node->next->prev=node->prev;	\
		if(node->prev) node->prev->next=node->next;	\
		else head=node->next;				\
	}

void lib_close(void);

#define	CLEAR_TEXT(var)			\
	{	if(var) free(var);		\
		var=NULL;				\
	}

#define	SET_TEXT(var, string)			\
	{	if(var) free(var);		\
		var=NULL;			\
		if(string) var=strdup(string);	\
	}

#define	SET_N_TEXT(var, string, size)		\
	{	if(var) free(var);		\
		var=NULL;			\
		if(string) var=strndup(string, size);	\
	}

int	safe_strcmp(const char *s1, const char *s2);
int	safe_strncmp(const char *s1, const char *s2, size_t n);
size_t  safe_strlen(const char *s);
int     safe_atoi(const char *s);
long    safe_atol(const char *s);
char *env_getvar(char *name);
int env_setvar(char *name, char *var);

#endif	//_CM_UTILS_H__
