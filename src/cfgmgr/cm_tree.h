#ifndef __CM_TREE_H__
#define __CM_TREE_H__

struct tree_node;
struct xml_buffer;

#include "cm_params.h"
#include "cm_keyvalue.h"
#include "cm_message.h"

/*
 * Each module type has certain information about the configuration tree
 * that it needs. Stuff like default configuration for a new node, List
 * names, etc. This structure holds it. It shouldn't really be used in
 * the settings tree inside of an instance. It's main job is to sit
 * in the modules global type structure for use with all the instances
 */
#define CONTENT_TYPE_UNKNOWN      0
#define CONTENT_TYPE_BRANCH       1
#define CONTENT_TYPE_STRING       2
#define CONTENT_TYPE_INT          3
#define CONTENT_TYPE_NET_ADDRESS  4
#define CONTENT_TYPE_LIST         5
#define CONTENT_TYPE_BINARY       6

#define NODE_REQUIRED       (1 << 0)
#define NODE_INITIAL_VALUE  (1 << 1)
#define NODE_READONLY       (1 << 2)

#define KEY_UNKNOWN        0
#define KEY_SETTINGS       1
#define KEY_STATUS         2
#define KEY_COMMAND        3
#define KEY_VALID_SETTINGS 4
#define KEY_SUBMODULE      5
#define KEY_TEMP           6

struct tree_node_meta_data {
	int content_type;
	char defaultvalue[MAX_SMSG_LEN];
	struct keyvalue *item_list;
	unsigned int flags;
};

struct tree_node {
	struct tree_node *children;	/* Children */
	struct tree_node *parent;	/* Parent */

	struct tree_node *next;	/* Next sibling */
	struct tree_node *prev;	/* Prev sibling */

	struct keyvalue *attributes;
	char *name;
	char *content;
	void *local_content;

	struct tree_node_meta_data *metadata; /* Stores info about the node. Don't use for instances */
	int content_type;

	unsigned int permissions; /* Modification permission */
};

struct key_parts {
	int keytype;
	char submodule[MAX_SMSG_LEN];
	int submodule_size;
	char *key;
};

int	key_split(char *key, struct key_parts *kp);
int	key_getParts(struct msg_base *msg, struct key_parts *kp);

/* There isn't a great way to tokenize the return key value.
   This is a better attempt at doing this. */
#define MAX_KEY_PATHS		10

struct key_tok
{
	int len[MAX_KEY_PATHS];
	char *key[MAX_KEY_PATHS];
};

int	key_tokenize(char *key, struct key_tok *tok);

/*
 * API for accessing tree
 */
/* Nodes */
struct tree_node *tree_newNode(void);
int tree_deleteNode(struct tree_node *node);
int tree_setNodeName(struct tree_node *node, char *name);
int tree_setNodeContent(struct tree_node *node, char *value);
struct tree_node *tree_findNodeChild(struct tree_node *node, char *name, int namelen);
struct tree_node *tree_addNodeChild(struct tree_node *node, char *name, int namelen);
struct tree_node *tree_addNodeChildDup(struct tree_node *node, char *name, int namelen);

/* Nodes attributes */
int tree_flushNodeAttributes(struct tree_node *node);
struct keyvalue *tree_findNodeAttribute(struct tree_node *node, char *attr);
char *tree_getNodeAttribute(struct tree_node *node, char *attr);
int	tree_addNodeAttribute(struct tree_node *node, char *name, char *value);
int	tree_deleteNodeAttribute(struct tree_node *node, char *name);

/* Keys */
struct tree_node *tree_findKey(struct tree_node *node, char *key);
struct tree_node *tree_addKeyDup(struct tree_node *node, char *key);
struct tree_node *tree_addKey(struct tree_node *node, char *key);
int tree_deleteKey(struct tree_node *node, char *key);
int tree_deleteKeyValue(struct tree_node *node, char *key, char *value);
char *tree_getKeyContent(struct tree_node *node, char *key);
int tree_setKeyContent(struct tree_node *node, char *key, char *value);

/* Keys attributes */
int tree_deleteKeyAttribute(struct tree_node *node, char *key, char *attr);
char *tree_getKeyAttribute(struct tree_node *node, char *key, char *attr);
int tree_setKeyAttribute(struct tree_node *node, char *key, char *attr, char *value);

/* Whole Trees */
int tree_flush(struct tree_node *node);
int tree_copy(struct tree_node *dst, struct tree_node *src);
int tree_move(struct tree_node *dst, struct tree_node *src);
int tree_subset(struct tree_node *haystack, struct tree_node *needle);
int tree_copyAttributes(struct tree_node *dst, struct tree_node *src);
int tree_isKeyEqual(struct tree_node *t1, struct tree_node *t2, char *key);
void tree_dump(struct cm_module *module, int level, struct tree_node *ptr);
void tree_dump2(char *name, int level, struct tree_node *ptr);
void tree_dumpXML(struct xml_buffer *buf, struct tree_node *node);

#endif /*__CM_TREE_H__*/
