#ifndef __CM_CONTAINER_H__
#define __CM_CONTAINER_H__

#include "cm_tree.h"
//#include "cm_params.h"
//#include "cm_interface.h"
//#include "cm_module.h"

#define		CONTAINER_UNKNOWN	0
#define		CONTAINER_SELECTOR	2

struct path_node{
	/* Used for the actual graph */
	struct path_node	*next;
	struct path_node	*prev;

	struct path_node	*children;
	
	/* These are for the raw list of the nodes */
	struct path_node	*raw_next;

	char	module_type[MAX_SMSG_LEN];
};

/* A path container a graph of the path from the
   top to bottom. There can only be a single top
   and bottom node. */
struct path{
	struct path	*next;
	struct path	*prev;

	char	name[MAX_SMSG_LEN];

	struct path_node		*raw_path_node_list;
	struct path_node		*top_module;
	struct path_node		*bottom_module;
};

struct container_def;

struct instance_wrapper{
	struct	instance_wrapper		*next;
	struct	instance_wrapper		*prev;

	/* We have a problem. Due to the way that the parsing engine
	   works, we do not create a module's instance until after
	   all the configuration for that instance is handled. But
	   we need a place to put the info that we got. Now, for
	   a normal module, that isn't that bad, but for a container,
	   we can instanciate sub-modules in the process of creating
	   that module. But where do store that info? We create a
	   holding pen in this wrapper that is created when we START
	   a container. We put all the relevent info in here, and then
	   the module can get to it when it is ready.
	*/

	struct cm_module_list	*modules_list;

	struct cm_module_list	*top_module;
	struct cm_module_list	*bottom_module;
	char	temp_string[MAX_SMSG_LEN];

	char	instance_name[MAX_SMSG_LEN];
	char	instance_active[MAX_SMSG_LEN];
	char	instance_lan[MAX_SMSG_LEN];
	char	instance_description[MAX_LMSG_LEN];
	char	container_name[MAX_SMSG_LEN];


	/* Stores the current path name */
	char		path_name[MAX_SMSG_LEN];
	struct	path	*path;

	struct container_def	*container_def;
};

enum handle_state {HANDLE_OFF, HANDLE_ON, HANDLE_RSVD};

struct handle
{
	enum handle_state state;
};

struct container_def
{
	struct container_def *next;
	struct container_def *prev;

	char container_name[MAX_SMSG_LEN]; /* Container name */
	int type;

	int top_interface_type;
	int bottom_interface_type;

	char default_path[MAX_SMSG_LEN];
	struct path *paths;

	struct instance_wrapper *wrappers;
	int wrapper_count;

	int max_handles;
	struct handle *handles;
};

int	handle_new_container_class(struct container_def *cinfo);
int	container_manager_message(struct msg_base *msg);
int	container_instantiate_module(struct instance_wrapper *wrapper,
		const char *name, int active, int lan, struct tree_node *config_head);
int	instantiate_container( struct instance_wrapper *wrapper, struct tree_node *config_head);
int	container_link_from_graph(struct container_def *container_def,
		struct path *path,
		struct instance_wrapper *wrapper,
		struct cm_module *container_module);
int	container_instantiate_modules_from_graph(struct instance_wrapper *wrapper, struct path *path);
int	delete_wrapper(struct instance_wrapper	*wrapper);
void	stop_embedded_modules( struct cm_module	*module );
void	start_embedded_modules( struct cm_module	*module );
void container_free_all();
struct container_def *container_search(char *name, int namelen);
struct msg_base *mgr_listGroup(struct msg_base *msg, char *name, int len);
const char *mgr_getModuleContainer(const char *modid);

#endif /*__CM_CONTAINER_H__*/
