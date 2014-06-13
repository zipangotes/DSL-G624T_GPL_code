#ifndef  __KEYVALUE_H__
#define  __KEYVALUE_H__

#include "cm_params.h"

struct keyvalue{
//	char	key[MAX_SMSG_LEN];
//	char	value[MAX_MMSG_LEN];
	char	*key;
	char	*value;

	struct keyvalue	*next;
	struct keyvalue	*prev;
};

/* The append function allows you to append duplicate values onto the list.
   This may not be what you want. */
int		keyvalue_append(struct keyvalue **head, char *key, char *value);
int		keyvalue_set(struct keyvalue **head, char *key, char *value);
struct keyvalue	*keyvalue_find(struct keyvalue **head, char *key);
int		keyvalue_del(struct keyvalue **head, struct keyvalue *kv);
int		keyvalue_flush(struct keyvalue **head);

#endif /*__KEYVALUE_H__*/
