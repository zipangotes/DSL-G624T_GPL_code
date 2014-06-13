/* outgoing (NSP bound) message structure */

#ifndef CM_VOICE_API_H
#define CM_VOICE_API_H

//#include "vopglueinc.h"
#include "vniuinc.h"
#include <vopnspif.h>
#include <cm_message.h>
#include <cm_comm.h>
#include <cm_logger.h>
#include <cm_utils.h>
#include <cm_message.h>
#include <cm_base64.h>

#define CM_VOICEAPP_NAME "voice_app"  /* Voice application */
#define CM_VOICEAPI_NAME "voice_api"  /* NSP Cfgman module */

#define VOICE_CONFIG_BUFFER_CHUNK 201  /* Encoding */
#define VOICE_RLE_BUFFER_SIZE  30000
#define BINARY_VCFG_FILE 

#define V_RELEASE_STRING_SIZE 64     /* size of the Voice release string */


typedef struct vcfg_buffer
{
    char rle_buff[VOICE_RLE_BUFFER_SIZE];        /* Run length encoding buffer    */
    char* chunk_ptr;                             /* pointer within current buffer */
    int size;                                    /* size of current image         */
    int last_seq_cnt;                            /* last seq_num received         */ 
}vcfg_buffer;
    
/* prototypes */
int init_nsp_api(void);
int run_nsp_api(void);
VOPNSPIF_ERR_T nspapi_v2n_notify(VOPNSPIF_NOTIFY_T notification, UINT32 info);
VOPNSPIF_ERR_T nspapi_v2n_config_response(VOPNSPIF_CONFIG_RESP_T response);
VOPNSPIF_ERR_T nspapi_v2n_config_save(UINT32 size, UINT8 *p_config, VOPNSPIF_CONFIG_T config_type);
VOPNSPIF_ERR_T nspapi_v2n_call_notification(UINT32 num_of_calls, UINT32 voice_packet_size, UINT32 inter_packet_time);
VOPNSPIF_ERR_T nspapi_v2n_firewall_action(VOPNSPIF_FIREWALL_ACTION_T action, VOPNSPIF_TRANSPORT_PROT_T protocol, UINT32 port_start, UINT32 port_end, VOPNSPIF_PORT_TYPE_T port_type);
VOPNSPIF_ERR_T nspapi_v2n_text_put_string(UINT8 *string);
VOPNSPIF_ERR_T nspapi_v2n_syslog(UINT8 *string);
void* get_vop_to_nsp_interface(void);
int send_to_vm(struct msg_base *msg);



/* interface table */
VOP_TO_NSP_IF_T vop_to_nsp_if=
{   
    nspapi_v2n_config_response,
    nspapi_v2n_notify,
    nspapi_v2n_config_save,
    nspapi_v2n_call_notification,
    nspapi_v2n_firewall_action,
    nspapi_v2n_text_put_string,
    nspapi_v2n_syslog
};

#endif
