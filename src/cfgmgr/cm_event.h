/* -*- Mode: C; tab-width: 2; indent-tabs-mode: t; c-basic-offset: 2 -*-
 * File: cm_event.h
 *
 * Created: Fri Dec 20 13:35:36 2002
 *
 * $Id: cm_event.h,v 1.2 2003/12/12 19:49:25 jharrell Exp $
 */

#ifndef  CM_EVENT_H
#define  CM_EVENT_H

#include "cm_params.h"

struct cm_module;

#define DIR_UNKNOWN 0
#define DIR_BELOW   1
#define DIR_ABOVE   2
#define DIR_DIRECT  3
//#define DIR_BCAST   4

#define RET_EVT_ERROR -1
#define RET_EVT_PASS  0
#define RET_EVT_DROP  1
//#define RET_EVT_TRANSFORMED  2

/* Common events */
#define EVT_UNKNOWN                    0
#define EVT_CMD_IFACE_UP               1
#define EVT_CMD_IFACE_DOWN             2
#define EVT_QUERY_LINK                 3
#define EVT_QUERY_CARRIER              4
#define EVT_QUERY_MODULE_INFO          5
#define EVT_REPORT_IFACE_LINK_STATE    6
#define EVT_REPORT_IFACE_CREATED       7
#define EVT_REPORT_IFACE_DELETED       8
#define EVT_REPORT_CARRIER_STATE       9
#define EVT_REPORT_CONFIG_PLANNED      10
#define EVT_REPORT_CONFIG_FINISHED     11
#define EVT_REPORT_MODULE_START        12
#define EVT_REPORT_MODULE_STOP         13
#define EVT_REPORT_MODULE_EXIT         14
#define EVT_REPORT_MODULE_INFO         15

/* 802.3 events */
#define EVT_QUERY_IFACE_NAME           20
#define EVT_QUERY_IFACE_STATS          21
#define EVT_REPORT_IFACE_NAME          22
#define EVT_REPORT_IFACE_STATS         23
#define EVT_QUERY_IP_ADDR              24
#define EVT_REPORT_IFACE_CONFIGURED    25 /* should be deprecated soon */
#define EVT_REPORT_IP_ADDR             EVT_REPORT_IFACE_CONFIGURED
#define EVT_REPORT_MAC_CHANGED         26

/* VC events */
#define EVT_QUERY_VC_VPI_VCI           30
#define EVT_QUERY_VC_QOS               31
#define EVT_QUERY_VC_QOS_TYPE          32
#define EVT_QUERY_VC_QOS_RATE          33
#define EVT_REPORT_VC_VPI_VCI          34
#define EVT_REPORT_VC_QOS              35
#define EVT_REPORT_VC_QOS_TYPE         36
#define EVT_REPORT_VC_QOS_RATE         37
#define EVT_QUERY_AUTO_VC	       38

/* Firewall related events */
#define EVT_QUERY_FW                   40
#define EVT_REPORT_FW_RUNNING          41
#define EVT_REPORT_FW_SHUTDOWN         42
#define EVT_REPORT_FW_PROXY            43
#define EVT_REPORT_FW_RIP              44
#define EVT_REPORT_FW_DHCP_RELAY       45
#define EVT_REPORT_FW_DEL_LANIP        46
#define EVT_REPORT_FW_ACTION           47
#define EVT_REPORT_FW_LOGIN            48

/* ACL related events */
#define EVT_REPORT_ACL_FLUSH           50
#define EVT_REPORT_ACL_INSTALL         51
#define EVT_REPORT_RMSTATE_FLUSH       52
#define EVT_REPORT_RMSTATE_INSTALL     53
#define EVT_REPORT_ACL_RMSTATE_FLUSH   54
#define EVT_REPORT_ACL_RMSTATE_INSTALL 55
#define EVT_REPORT_ACL_TOGGLE          56
#define EVT_REPORT_ACL_LAN_ID          57

/* CLIP related events */
#define EVT_REPORT_ARPD_RUNNING        60

/* SNTP events */
#define EVT_REPORT_SNTP_CONFIGURED     70

/* Voice/QoS events */
#define EVT_CMD_QOS_VOICE              80
#define EVT_REPORT_VOICE_DISABLE       81

/* Bridge events */
#define EVT_REPORT_BR_ADD_IFACE        90
#define EVT_REPORT_BR_DEL_IFACE        91

/* Timer events */
#define EVT_REPORT_TIMER               100

/* Lock events */
#define EVT_REPORT_LOCK_ACQUIRED       105

/* Global events */
#define EVT_GL_REPORT_IFACE_LINK_STATE 501
#define EVT_GL_REPORT_CARRIER_STATE    502
#define EVT_GL_REPORT_IFACE_NAME       503
#define EVT_GL_REPORT_IFACE_STATE      504
#define EVT_GL_REPORT_IFACE_CONFIGURED 505 /* should be deprecated soon */
#define EVT_GL_REPORT_IP_ADDR          EVT_GL_REPORT_IFACE_CONFIGURED
#define EVT_GL_REPORT_HANDLE_DELETED   506
#define EVT_GL_REPORT_HANDLE_CHANGED   507
#define EVT_GL_REPORT_NAT_STATE        508
#define EVT_GL_REPORT_CONFIG_SAVE      509
#define EVT_GL_QUERY_NAT_STATE         510

enum link_state {ifDown=0, ifUp=1};
enum carrier_state {crDown=0, crUp=1};
enum islan_state {notLan=0, isLan=1};
enum nat_state {natOff=0, natOn=1};

typedef enum {
	FW_OPEN=1, 
	FW_CLOSE=2
} FW_ACTION_T;

typedef enum {
	FW_UDP=1,
	FW_TCP=2
} FW_PROTOCOL_T;

struct ev_nat_state
{
  enum nat_state state;
  char ifname[16];
};

struct ev_fw_action
{
	FW_ACTION_T action;
	FW_PROTOCOL_T protocol;
	unsigned int port_start;
	unsigned int port_end;
    unsigned long remote_ip;
    unsigned long remote_mask;
};

struct ev_fw_tcpmss
{
	int action; /* 1|0 SET|RESET */
	unsigned int tcpmss;	
};

struct ev_iface_info
{
	enum islan_state lan;
	char name[12];
	char description[32];
};

struct ev_iface_state
{
	enum link_state state;
	char name[12];
};

struct ev_iface_config
{
	const char *name;
	const char *addr;
	const char *mask;
	const char *gw;
};

/* Voice Qos Configuration */
struct ev_qos_config
{
	int command; /* 0|1 = Stop|Start */
	char *qos_type; /* pppoe|ip */
};

struct cm_event
{
	long direction;
	long type;
	void *body;

	/* nothing below should be accessible by a module */
/*	struct cm_module *orig;
	struct cm_module *dest;*/
	char orig[MAX_SMSG_LEN];
	char dest[MAX_SMSG_LEN];
	char last[MAX_SMSG_LEN];

	struct cm_event *next;
	struct cm_event *prev;
};

int event_send(struct cm_module *mod, long direction, long type, void *body);
int event_reply(struct cm_module *mod, struct cm_event *event, long type, void *body);
int event_direct(struct cm_module *mod, char *id, long type, void *body);
int event_broadcast(struct cm_module *mod, char *mtype, long type, void *body);
int event_global(struct cm_module *mod, long type, void *body);

int event_global_register(struct cm_module *mod);
int event_global_unregister(struct cm_module *mod);

#endif
