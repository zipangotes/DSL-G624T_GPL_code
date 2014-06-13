#ifndef __CM_ATM_H__
#define __CM_ATM_H__

struct cm_atm_qos
{
    char *traffic_class;
    int pcr;
    int scr;
    int mbs;
    int cdv;
};

struct vcc_lut_t 
{
    int inuse;
    int vpi;
    int vci;
	int auto_vc; /* 1 - auto vc, 0 - no */
};


/*---------------------------*/
/*     Auto VC structures    */
/*---------------------------*/
typedef struct vcc_usage_info
{
    char vpi[6];
    char vci[6];
    int in_use;
    int ping_successful;
}vcc_usage_info;

typedef struct auto_vc_table
{
    int vc_defined;
    vcc_usage_info vcc_info[8];
} auto_vc_table;

typedef struct auto_vc_evt_msg
{
    struct msg_base *msg;
    char * key;
} auto_vc_evt_msg;

#endif /*__CM_ATM_H__ */
