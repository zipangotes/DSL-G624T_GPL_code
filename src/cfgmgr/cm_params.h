#ifndef  __CM_PARAMS_H__
#define  __CM_PARAMS_H__

#define CM_LOGIC_NAME   "logic"
#define CM_LOGGER_NAME  "logger"
#define CM_PC_NAME      "pc"
#define CM_MONITOR_NAME "monitor"
#define CM_WEBCM_NAME   "webcm"
#define CM_CLI_NAME     "cli"
#define CM_FW_NAME      "fw"
#define CM_SNMP_NAME    "snmpd"
#define CM_LOGIN_NAME   "login"
#define CM_TEXTUI_NAME  "textui"
#define CM_VOICE_NAME   "voice"
#define CM_VOICEAPP_NAME "voice_app"
#define CM_CMCLI_NAME   "cmcli"

#define XML_BUF_SIZE    65536*2-16

#define FILE_VOICE_CONFIG  "/var/tmp/voice_config.txt"

#define MAX_PARAMS      64

#define MAX_SMSG_LEN	  64
#define MAX_MMSG_LEN	  256
#define MAX_LMSG_LEN	  1024

#define	CYCLE_TIME	    10

#define SL_R_GUEST      (1<<0)
#define SL_R_USER       (1<<1)
#define SL_R_ADMIN      (1<<2)
#define SL_R_EXPERT     (1<<3)
#define SL_R_ISP        (1<<4)
#define SL_W_GUEST      (1<<8)
#define SL_W_USER       (1<<9)
#define SL_W_ADMIN      (1<<10)
#define SL_W_EXPERT     (1<<11)
#define SL_W_ISP        (1<<12)

#endif //__CM_PARAMS_H__
