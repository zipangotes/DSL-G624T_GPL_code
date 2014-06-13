#ifndef __CM_MODULE_H__
#define __CM_MODULE_H__

struct cm_module;

#include "cm_utils.h"
#include "cm_params.h"
#include "cm_tree.h"
#include "cm_message.h"
#include "cm_event.h"
#include "cm_container.h"

struct xml_buffer
{
	char *text;
	int off;
};

struct cm_module_list {
	struct cm_module_list *next;
	struct cm_module_list *prev;

	struct cm_module	*module;
};

struct cm_module {
	int (*start)(struct cm_module *module);
	int (*stop)(struct cm_module *module);
	int (*activate)(struct cm_module *module, char active);
	int (*do_message)(struct cm_module *module,
                    struct msg_base *message);
	int (*do_event)(struct cm_module *module, struct cm_event *event);
	int (*save)(struct cm_module *module, struct xml_buffer *buf);
	int (*detach)(struct cm_module *module);

	void *priv;

	/* This is the module's id. It is the unique id for this module */
	char id[MAX_SMSG_LEN];
	char type[MAX_SMSG_LEN];

	/* This is the module's flags */
	char active;
	char lan;
	char embedded;
	char started;
	char *description;

	/* Module's current configuration tree */
	struct tree_node *config;
	/* Module's temporary configuration tree */
	struct tree_node *temp;

	/*=====================================================*/
	/* Modules should not need to access things below this */

	/* Used to track modules above and below */
	struct cm_module_list	*above;
	struct cm_module_list	*below;

	struct instance_wrapper	*wrapper;
};

int register_module(const char *name, int (*init)(struct cm_module *module));
void mod_save_single(struct cm_module *module, struct xml_buffer *buf, int id);
int mgr_isLAN(char *name);
const char *mgr_getIfaceDesc(const char *modid);
struct msg_base *mgr_listAboveByType(struct msg_base *msg, char *modname, char *type);

const char *text(char id[8]);

#endif /*__CM_MODULE_H__ */
