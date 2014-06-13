#ifndef CM_LIB_FIREWALL_H
#define CM_LIB_FIREWALL_H

#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "cm_module.h"
#include "cm_event.h"
#include "cm_message.h"
#include "cm_tree.h"
#include "cm_logger.h"
#include "cm_utils.h"

#define ENOMEM 1
#define MAX_IPTABLES_ARGS 256
#define IF_NAME_SIZE  16
#define IF_ADDR_SIZE  16 

#define FWAN 1
#define FLAN 2

#define FLAN_ACL        0
#define FWAN_ACL        1


/* ACL modes */

#define ACL_CHANGE        0
#define STATE_CHANGE      1
#define ACL_STATE_CHANGE  2
#define ACL_TOGGLE        3
#define ACL_DO_RMT_MGT    4
#define ACL_DO_FLUSH      5 /* used to insure we do the drop rule */

#define INSTALL	1
#define FLUSH 0

typedef enum fw_fsm {
	STOP,
	START,
	FDB_SERVICE_READY,
	NO_INTERFACE,
	INTERFACE_DELETED,
	RUNNING
} fw_fsm;

typedef enum fw_chain_mode {
	CHAIN_UNCONFIGURED = -1,
	CHAIN_PASSIVE,
	CHAIN_ACTIVE
} fw_chain_mode;

typedef enum fw_chain_action {
	CHAIN_BLOCK = 1,
	CHAIN_INPUT,
	CHAIN_OUTPUT,
	CHAIN_PASS,
	CHAIN_NAT
} fw_chain_action;

#define FW_MAX_VOICE_RULES 4 /* dependency to the definition in voice/voice.h */

typedef struct voice_fw_rule
{
	unsigned int present;
	int added;
	struct ev_fw_action rule;
} voice_fw_rule;

typedef struct fw_priv
{
    char ifname[IF_NAME_SIZE];
    char ifaddr[IF_ADDR_SIZE];
    char gw[IF_ADDR_SIZE];
    unsigned short state;
    unsigned short type;
    unsigned short spi;
    unsigned short nat;
    unsigned short tproxy;
    char *protocol;
    char *portstart;
    char *portend;
    char *portmap;
    char *InternalClient;
    char *RemoteHost;
    struct tree_node *fw_public, *fw_protected, *fw_private;
    struct tree_node *fdb;
	struct voice_fw_rule voice_rules[FW_MAX_VOICE_RULES];
    char container_id[IF_NAME_SIZE];

} fw_priv;

int fw_no_msg(struct cm_module *mod, struct msg_base *msg);
int fw_no_evt(struct cm_module *mod, struct cm_event *evt);
int fw_msg_rep(struct cm_module *mod, struct msg_base *msg);
int fw_start(struct cm_module *mod);
int fw_detach(struct cm_module *mod);
int fw_evt_rep_fw_del_lanip(struct cm_module *mod, struct cm_event *evt);
int fdb_rule_delete(struct cm_module *mod, struct tree_node *node, int type, char *ip);
struct tree_node *fw_fdb_node(struct tree_node *head, char *key);
void proto_port_extract(struct cm_module *, struct tree_node *);
void proto_port_walk (struct cm_module *, struct tree_node *, int, unsigned short, void (*fn)());
void just_do_it(struct cm_module *mod);

void install_chain_nat(struct cm_module *mod, struct tree_node *p, unsigned short flag);
void install_chain(struct cm_module *mod, struct tree_node *p, int action, unsigned short flag);

void chain_block(struct cm_module *mod, struct tree_node *fdb_node, unsigned short action);
void redirect_chain(struct cm_module *mod, struct tree_node *node, struct msg_base *msg, int chain_type);
int chain_install(struct cm_module *mod, struct tree_node *my_node, int, int);
void do_chain(struct cm_module *mod, struct tree_node *my_node, struct tree_node *fdb_node,
		                int, int);
void do_chain_prerouting(struct cm_module *mod, struct tree_node *my_node,
		                struct tree_node *fdb_node, int chain_type, int action);

void walk_rule_list(struct cm_module *, struct tree_node *, struct msg_base *, int, void (*fn)());

void walk_public_rule_list(struct cm_module *, struct tree_node *, struct msg_base *, int, int (*fn)());

void chain_fdb_get_info(struct cm_module *mod, struct tree_node *node, struct msg_base *msg, int);
void chain_mark_and_do(struct cm_module *mod, struct tree_node *node, struct msg_base *msg, int);
void chain_mark_and_undo(struct cm_module *mod, struct tree_node *node, struct msg_base *msg, int);
int get_settings(struct cm_module *mod);
void install_block(struct cm_module *mod, struct tree_node *node, struct msg_base *msg, int flag);
int block_ip(struct cm_module *mod, char *buf, int buflen, char *action);
void do_spi(struct cm_module *, int);
void do_nat(struct cm_module *, int);
void do_drop(struct cm_module *, char *, char *);
void do_state(struct cm_module *, char *, char *);
void do_mtu(struct cm_module *, int);
void do_dmz_filter(struct cm_module *, char *, int);
void do_dmz_nat(struct cm_module *, char *, int);
void remove_spi_forward_top(struct cm_module *mod);
void add_spi_forward_top(struct cm_module *mod);
void spi_bottom(struct cm_module *mod, int);
void do_block(struct cm_module *mod, char *ip, unsigned short action);
void do_ping(struct cm_module *mod, int action);
void do_rip(struct cm_module *mod, int action);
void do_dhcp_relay(struct cm_module *mod, int action);
int query_parse(char *kp_key, struct key_tok *tok);
void query_response(struct cm_module *mod, struct tree_node *node, struct msg_base *msg, int ip_addr);

/* common handlers */
int fw_trans_query(struct cm_module *mod, struct msg_base *msg);
int fw_trans_set(struct cm_module *mod, struct msg_base *msg);
int fw_trans_reject(struct cm_module *mod, struct msg_base *msg);
int fw_trans_group_end(struct cm_module *mod, struct msg_base *msg);

int rule_add(struct cm_module *mod, struct key_tok *tok);
void new_rule(struct cm_module *mod, struct tree_node *node, char *tok, int tok_len);
int command_execute(struct cm_module *mod, struct msg_base *msg, struct key_tok *tok, char *action);
int validate_ip(struct cm_module *mod, struct tree_node *node, char *buf, int buflen);
struct tree_node *find_ip(struct cm_module *mod, struct tree_node *node, char *buf, int buflen);
int do_public_add(struct cm_module *mod, struct msg_base *msg);
int do_public_edit(struct cm_module *mod, struct msg_base *msg);
int do_public_delete(struct cm_module *mod, struct msg_base *msg);
int do_public_clear(struct cm_module *mod, struct msg_base *msg);
struct tree_node *fw_public_rule_add(struct cm_module *mod, struct msg_base *msg, int type);
int fw_public_rule_do(struct cm_module *mod, struct tree_node *node, struct msg_base *msg, int flag);
int fw_public_rule_set_node(struct cm_module *mod, struct tree_node *node, struct msg_base *msg);
void do_tproxy(struct cm_module *, char *, int);

/* functions from remote_mgmt_flib */
int apply_remote_mgmt(struct cm_module *mod, int action, int aclmode, int net);
int do_remote_mgmt(struct cm_module *mod, int action, int from_module);
int process_acl_table(struct cm_module *mod, struct tree_node *node, struct tree_node *aclnode, int action, int net, int aclmode );
int delete_lanside_blocks(struct cm_module *mod);
void remove_nat_drop_bottom(struct cm_module *mod);
void add_nat_drop_bottom(struct cm_module *mod);

int apply_ping(struct cm_module *);
void do_ping_fwan(struct cm_module *mod, int action);
void do_ping_flan(struct cm_module *mod, int action);

void do_fw_action(struct cm_module *mod, struct ev_fw_action *fw, int fw_state);
void do_remote_mgmt_nat(struct cm_module *mod, int type, char *ip, char *mask, char *port, int action);
void do_remote_mgmt_filter(struct cm_module *mod, int type, char *ip, char *mask, char *port, int action);

#endif

