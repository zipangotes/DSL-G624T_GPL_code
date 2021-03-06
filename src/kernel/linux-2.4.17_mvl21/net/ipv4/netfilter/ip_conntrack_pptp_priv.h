#ifndef _IP_CT_PPTP_PRIV_H
#define _IP_CT_PPTP_PRIV_H

/* PptpControlMessageType names */
static const char *strMName[] = {
       "UNKNOWN_MESSAGE",
       "START_SESSION_REQUEST",
       "START_SESSION_REPLY",
       "STOP_SESSION_REQUEST",
       "STOP_SESSION_REPLY",
       "ECHO_REQUEST",
       "ECHO_REPLY",
       "OUT_CALL_REQUEST",
       "OUT_CALL_REPLY",
       "IN_CALL_REQUEST",
       "IN_CALL_REPLY",
       "IN_CALL_CONNECT",
       "CALL_CLEAR_REQUEST",
       "CALL_DISCONNECT_NOTIFY",
       "WAN_ERROR_NOTIFY",
       "SET_LINK_INFO"
};

#endif
