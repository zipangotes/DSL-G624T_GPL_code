#ifndef __CM_MESSAGE_H__
#define __CM_MESSAGE_H__

struct msg_base;

#include <stdarg.h>

#include "cm_params.h"
#include "cm_module.h"

#define RET_MSG_PASS      -6
#define RET_MSG_BAD_GROUP -5
#define RET_MSG_BAD_VALUE -4
#define RET_MSG_BAD_KEY   -3
#define RET_MSG_ERROR     -2
#define RET_MSG_BLOCK     -1
#define RET_MSG_OK        0

#define MSG_UNKNOWN     0
#define MSG_REPORT      1
//#define MSG_QUERY       2
//#define MSG_RESPONSE    3
//#define MSG_SET         4
//#define MSG_APPLY       5
//#define MSG_ERROR       6
#define MSG_LOG         7
#define MSG_MONITOR     8
#define MSG_PROGRAM     9
#define MSG_TRANSACTION	10

#define MSG_TRANS_UNKNOWN       0
#define MSG_TRANS_QUERY         1
#define MSG_TRANS_RESPONSE      2
#define MSG_TRANS_RESPONSE_CONT 3
#define MSG_TRANS_SET           4
#define MSG_TRANS_GROUP_BEGIN   5
#define MSG_TRANS_GROUP_END     6
#define MSG_TRANS_GROUP_ERROR   9
#define MSG_TRANS_APPLY         7
#define MSG_TRANS_REJECT        8

//#define MAX_TRANS_VALUE_COLS	9
#define MAX_TRANS_VALUE_COLS	30

#define MSG_ERROR_SUCCESS   0
#define MSG_ERROR_BAD_KEY   -1
#define MSG_ERROR_BAD_VALUE -2
#define MSG_ERROR_CONFLICT  -3
#define MSG_ERROR_GROUP     -4

/* TODO: replace with a real error code */
#define MSG_ERROR_NOT_VALID	0		/* Used for query handles when
						   no more handle present. The
						   key/value is good, just out
						   of the scope */

struct msg_value {
	struct msg_value *next;
	struct msg_value *prev;
	int count;
	char *value[MAX_TRANS_VALUE_COLS];
};

struct msg_transaction {
	int type;
	char *key;
	char *re_seq;
	char *gid;
	int error_code;
	char *error_msg;
	int subseq;
	struct msg_value *values;
};

struct msg_log {
	char *facility;
	char *level;
	char *text;
};

struct msg_search {
	struct msg_search *next;
	struct msg_search *prev;
	char *name;
	char *regex;
};

struct msg_monitor {
	char *action;
	char *filename;
	struct msg_search *searches;
};

struct msg_arg {
	struct msg_arg *next;
	struct msg_arg *prev;
	char *arg;
};

struct msg_program {
	char *action;
	char *name;
	char *pid;
	char *signal;
	struct msg_arg *args;
	struct msg_search *search_output;
};

struct msg_report {
	char *name;
	char *pid;
	char *text;
};

struct msg_base {
	char *to;
	char *from;
	char *seq;
	int msg_type;
	int expire; // used internaly to expire undeliverable messages

	struct msg_monitor *monitor;
	struct msg_program *program;
	struct msg_report *report;
	struct msg_transaction *transaction;
	struct msg_log *log;

	/* module is not supposed to touch anything below */
	struct msg_base *next;
	struct msg_base *prev;
};

enum msg_route_dest {RT_UNKNOWN, RT_INTERNAL, RT_EXTERNAL, RT_LOGIC};

struct msg_route_t
{
	char *name;
	enum msg_route_dest route;
	int (*message)(struct msg_base *msg);
};

/***********************************************************************************/
/* base message */
struct msg_base *msgr_makeBase(char *from, char *to); // raw
struct msg_base *msgm_makeBase(struct cm_module *mod, char *to); // module
// does not make sense to send out empty message

/* process control */
struct msg_base *msgr_makeProgramStart(char *from, char *name, char *pid,
                                       int argc, const char *argv[],
                                       int outputc, const char *outputv[][2]); // raw
struct msg_base *msgm_makeProgramStart(struct cm_module *mod, char *name, char *pid,
                                       int argc, const char *argv[],
                                       int outputc, const char *outputv[][2]); // module
int msgr_sendProgramStart(char *from, char *name, char *pid,
                          int argc, const char *argv[],
                          int outputc, const char *outputv[][2]); // raw
int msgm_sendProgramStart(struct cm_module *mod, char *name, char *pid,
                          int argc, const char *argv[],
                          int outputc, const char *outputv[][2]); // module

struct msg_base *msgr_makeProgramStop(char *from, char *pid); // raw
struct msg_base *msgm_makeProgramStop(struct cm_module *mod, char *pid); // module
int msgr_sendProgramStop(char *from, char *pid); // raw
int msgm_sendProgramStop(struct cm_module *mod, char *pid); // module

struct msg_base *msgr_makeProgramSignal(char *from, char *pid, char *sig); // raw
struct msg_base *msgm_makeProgramSignal(struct cm_module *mod, char *pid, char *sig); // module
int msgr_sendProgramSignal(char *from, char *pid, char *sig); // raw
int msgm_sendProgramSignal(struct cm_module *mod, char *pid, char *sig); // module

struct msg_base *msgr_makeProgramReboot(char *from); // raw
struct msg_base *msgm_makeProgramReboot(struct cm_module *mod); // module
int msgr_sendProgramReboot(char *from); // raw
int msgm_sendProgramReboot(struct cm_module *mod); // module

struct msg_base *msgr_makeProgramAction(char *from, char *action, char *name, char *pid, char *sig);

/* monitor */
struct msg_base *msgr_makeMonitorAdd(char *from, char *filename,
                                     char *reportname, char *regex, int period);
struct msg_base *msgm_makeMonitorAdd(struct cm_module *mod, char *filename,
                                     char *reportname, char *regex, int period);
int msgr_sendMonitorAdd(char *from, char *filename,
                        char *reportname, char *regex, int period);
int msgm_sendMonitorAdd(struct cm_module *mod, char *filename,
                        char *reportname, char *regex, int period);

struct msg_base *msgr_makeMonitorDelete(char *from,
                                        char *filename, char *reportname);
struct msg_base *msgm_makeMonitorDelete(struct cm_module *mod,
                                        char *filename, char *reportname);
int msgr_sendMonitorDelete(char *from, char *filename, char *reportname);
int msgm_sendMonitorDelete(struct cm_module *mod, char *filename, char *reportname);

/* report from pc | monitor */
struct msg_base *msgr_makeReport(char *from, char *to, char *name,
                                 char *pid, char *text, int textlen);
int msgr_sendReport(char *from, char *to, char *name,
                    char *pid, char *text, int textlen);
// modules never send reports

/* base transaction */
struct msg_base *msgr_makeTransBaseVar(char *from, char *to, int type, char *gid,
                                       char *key, char *re_seq, int subseq,
                                       int error, char *error_msg, va_list ap);
struct msg_base *msgr_makeTransBase(char *from, char *to, int type, char *gid,
                                    char *key, char *re_seq, int subseq,
                                    int error, char *error_msg, ...);
struct msg_base *msgm_makeTransBase(struct cm_module *mod, char *to, int type,
                                    char *gid, char *key, char *re_seq, int subseq,
                                    int error, char *error_msg, ...);
int msgr_addTransValuesVar(struct msg_base *msg, int count, va_list ap);
int msgr_addTransValues(struct msg_base *msg, int count, ...);
int msgr_addTransValueSingle(struct msg_base *msg, char *value);
// does not make sense to send out empty transaction message

/* transaction groups */
struct msg_base *msgr_makeTransGroupBegin(char *from, char *to, char *gid);
struct msg_base *msgm_makeTransGroupBegin(struct cm_module *mod, char *to, char *gid);
int msgr_sendTransGroupBegin(char *from, char *to, char *gid);
int msgm_sendTransGroupBegin(struct cm_module *mod, char *to, char *gid);

struct msg_base *msgr_makeTransGroupEnd(char *from, char *to, char *gid);
struct msg_base *msgm_makeTransGroupEnd(struct cm_module *mod, char *to, char *gid);
int msgr_sendTransGroupEnd(char *from, char *to, char *gid);
int msgm_sendTransGroupEnd(struct cm_module *mod, char *to, char *gid);

struct msg_base *msgr_makeTransApply(char *from, char *to, char *gid);
struct msg_base *msgm_makeTransApply(struct cm_module *mod, char *to, char *gid);
int msgr_sendTransApply(char *from, char *to, char *gid);
int msgm_sendTransApply(struct cm_module *mod, char *to, char *gid);

struct msg_base *msgr_makeTransReject(char *from, char *to, char *gid);
struct msg_base *msgm_makeTransReject(struct cm_module *mod, char *to, char *gid);
int msgr_sendTransReject(char *from, char *to, char *gid);
int msgm_sendTransReject(struct cm_module *mod, char *to, char *gid);

/* transactions */
struct msg_base *msgr_makeTransQuery(char *from, char *to, char *gid, char *key);
struct msg_base *msgm_makeTransQuery(struct cm_module *mod, char *to, char *gid, char *key);
int msgr_sendTransQuery(char *from, char *to, char *gid, char *key);
int msgm_sendTransQuery(struct cm_module *mod, char *to, char *gid, char *key);

struct msg_base *msgr_makeTransSet(char *from, char *to, char *gid,
                                   char *key, char *value);
struct msg_base *msgm_makeTransSet(struct cm_module *mod, char *to, char *gid,
                                   char *key, char *value);
int msgr_sendTransSet(char *from, char *to, char *gid, char *key, char *value);
int msgm_sendTransSet(struct cm_module *mod, char *to, char *gid, char *key, char *value);

struct msg_base *msgr_makeTransError(char *from, char *to, char *gid, char *re_seq,
                                     int error, char *error_msg, ...);
struct msg_base *msgm_makeTransError(struct cm_module *mod, char *to, char *gid,
                                     char *re_seq, int error, char *error_msg, ...);
int msgr_sendTransError(char *from, char *to, char *gid, char *re_seq,
                        int error, char *error_msg, ...);
int msgm_sendTransError(struct cm_module *mod, char *to, char *gid, char *re_seq,
                        int error, char *error_msg, ...);

struct msg_base *msgr_makeTransComboError(char *from, char *to, char *gid, char *re_seq,
                                     int error, char *error_msg, ...);
struct msg_base *msgm_makeTransComboError(struct cm_module *mod, char *to, char *gid,
                                     char *re_seq, int error, char *error_msg, ...);
int msgr_sendTransComboError(char *from, char *to, char *gid, char *re_seq,
                        int error, char *error_msg, ...);
int msgm_sendTransComboError(struct cm_module *mod, char *to, char *gid, char *re_seq,
                        int error, char *error_msg, ...);

struct msg_base *msgr_makeTransResponseVar(char *from, char *to, char *gid, char *re_seq,
                                           int subseq, char *key, int count, va_list ap);
struct msg_base *msgr_makeTransResponse(char *from, char *to, char *gid, char *re_seq,
                                        int subseq, char *key, int count, ...);
struct msg_base *msgm_makeTransResponse(struct cm_module *mod, char *to, char *gid,
                                        char *re_seq, int subseq, char *key, int count, ...);
int msgr_sendTransResponse(char *from, char *to, char *gid, char *re_seq,
                           int subseq, char *key, int count, ...);
int msgm_sendTransResponse(struct cm_module *mod, char *to, char *gid, char *re_seq,
                           int subseq, char *key, int count, ...);

/* transaction response helpers */
struct msg_base *msgr_makeTransResponseEmpty(char *from, char *to, char *gid,
                                             char *re_seq, int subseq, char *key);
struct msg_base *msgm_makeTransResponseEmpty(struct cm_module *mod, char *to, char *gid,
                                             char *re_seq, int subseq, char *key);
int msgr_sendTransResponseEmpty(char *from, char *to, char *gid, char *re_seq,
                                int subseq, char *key);
int msgm_sendTransResponseEmpty(struct cm_module *mod, char *to, char *gid, char *re_seq,
                                int subseq, char *key);

struct msg_base *msgr_makeTransResponseSingle(char *from, char *to, char *gid,
                                              char *re_seq, int subseq,
                                              char *key, char *value);
struct msg_base *msgm_makeTransResponseSingle(struct cm_module *mod, char *to, char *gid,
                                              char *re_seq, int subseq,
                                              char *key, char *value);
int msgr_sendTransResponseSingle(char *from, char *to, char *gid, char *re_seq,
                                 int subseq, char *key, char *value);
int msgm_sendTransResponseSingle(struct cm_module *mod, char *to, char *gid, char *re_seq,
                                 int subseq, char *key, char *value);
int msgm_replyTransResponseSingle(struct cm_module *mod, struct msg_base *msg, char *gid,
                                  int subseq, char *key, char *value);

struct msg_base *msgr_makeTransResponsePair(char *from, char *to, char *gid,
                                            char *re_seq, int subseq,
                                            char *key, char *value1, char *value2);
struct msg_base *msgm_makeTransResponsePair(struct cm_module *mod, char *to, char *gid,
                                            char *re_seq, int subseq,
                                            char *key, char *value1, char *value2);
int msgr_sendTransResponsePair(char *from, char *to, char *gid, char *re_seq,
                               int subseq, char *key, char *value1, char *value2);
int msgm_sendTransResponsePair(struct cm_module *mod, char *to, char *gid, char *re_seq,
                               int subseq, char *key, char *value1, char *value2);

/* message access helpers */
/* generic */
int msg_getType(struct msg_base *msg);
char *msg_getFrom(struct msg_base *msg);
char *msg_getTo(struct msg_base *msg);
char *msg_getSeq(struct msg_base *msg);

/* transactions */
int msg_isTrans(struct msg_base *msg);
int msg_getTransType(struct msg_base *msg);
char *msg_getTransReSeq(struct msg_base *msg);
char *msg_getTransGID(struct msg_base *msg);
char *msg_getTransKey(struct msg_base *msg);
int msg_getTransNumRows(struct msg_base *msg);
int msg_getTransNumCols(struct msg_base *msg, int row);
char *msg_getTransValue(struct msg_base *msg, int row, int col);
char *msg_getTransSingleValue(struct msg_base *msg);
int msg_getTransErrorCode(struct msg_base *msg);
char *msg_getTransErrorText(struct msg_base *msg);

/* report */
char *msg_getReportName(struct msg_base *msg);
char *msg_getReportPID(struct msg_base *msg);
char *msg_getReportText(struct msg_base *msg);

/******************************************************/
int msgm_replyIfaceStats(struct cm_module *module, struct msg_base *msg, char *iface);
int msg_setRouteTable(struct msg_route_t *rt);
int msg_setRouteHook(int (*hook)(struct msg_base *msg));
struct msg_value *new_value(void);
struct msg_base *msg_parse(char *msgtext, int msglen);
void msg_free(struct msg_base *msg);
struct msg_base *msg_copy(struct msg_base *msg);
int msg_makeXML(char *buf, int len, struct msg_base *msg);
int msg_send(struct msg_base *msg);
int msg_route(struct msg_base *msg);
void msg_flush(void);
int msg_sendOut(struct msg_base *msg, char *name);
void lib_close_messages(void);
int escapeXML(char *buf, int len, char *text);

#endif //__CM_MESSAGE_H__
