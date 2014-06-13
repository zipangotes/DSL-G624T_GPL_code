/*---------------------------------------------------------------------
           Copyright (c) 2003 Texas Instruments Incorporated
                        ALL RIGHTS RESERVED

     "This computer program is subject to a separate license agreement
     signed by Texas Instruments Incorporated and the licensee, and is
     subject to the restrictions therein.  No other rights to reproduce,
     use, or disseminate this computer program, whether in part or in
     whole, are granted."
------------------------------------------------------------------------

Configuration Manager

Name: voice_api (Voice API)

Purpose: Acts as a liason between the Voice application (MXP, MGCP, etc.)
                and the NSP.


Revision History:

Date            Description
-----------------------------------------------------------------------
1/20/04         Initial implementation 
-----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "input.h"
#include "voice_api.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define COMM_TIMEOUT 10

#ifdef DMALLOC
#include <dmalloc.h>
#endif /* DMALLOC */


/* externs */

extern void lib_close_messages(void);
extern int comm_start(char *name, int send_fl);
extern char* io_b64enc(const char* chunk, int size);
extern char* io_b64dec(const char* chunk, int *size);
//extern int RLE_Compress( unsigned char *in, unsigned char *out, unsigned int insize );
//extern int RLE_Uncompress( unsigned char *in, unsigned char *out, unsigned int insize );

/* globals */

vcfg_buffer vcb;
static int nsp_cli_debug = 0;

int	init_nsp_api(void)
{
	int	voiceapi_fd;
	int	retval;

	openlog("voice_api", LOG_CONS, 0);
	syslog(LOG_DEBUG, "Starting voice app");

	if((voiceapi_fd=comm_start(CM_VOICEAPP_NAME, 0)) < 0)
        {
            syslog(LOG_DEBUG, "Did not setup socket properly");
            return 0;
        }
        
	setup_input(voiceapi_fd, COMM_TIMEOUT);
        retval = run_nsp_api();
        
	lib_close_messages();
        closelog();

	return retval;
}

int run_nsp_api(void)
{
    struct msg_base *msg;
    unsigned int notify;
    unsigned int info;
    unsigned int size,discard;
    unsigned int ipaddr;
    int chunk_size;
    char *chunk_ptr;
    char *db;
    char *c_ptr;
    char *r_ptr;   
    int r_size;
    int b64dec_len,seq_cnt,total_loops;
    int value,i;
    struct in_addr iface;
    FILE *file;
    vcfg_buffer *vcb_ptr;
    NSP_TO_VOP_IF* p_n2v = NULL;    
#if 0
    char fname[64] = "/var/tmp/b64dec.jah";
#endif

    vcb_ptr = &vcb;
    vcb_ptr->last_seq_cnt = 0;
    vcb_ptr->size = 0;

    p_n2v = (NSP_TO_VOP_IF *)get_nsp_to_vop_interface();

    if(!p_n2v)
    {
        syslog(LOG_DEBUG, "Could not get nsp_to_vop interface");
        return -1;
    }
    
    while(1)
    {
        /* process incoming messsages */
        msg = get_input();
	if(!msg)
        {
		/* Communication error, just dump what we got */
		syslog(LOG_WARNING, "Socket communication error");
                continue;
	}


        syslog(LOG_DEBUG, "n2v_message received %s", msg->transaction->key);

	/* Check for non-transaction */
	if(msg->msg_type != MSG_TRANSACTION)
        {
		syslog(LOG_WARNING, "Received non-transaction message");
                msg_free(msg);
                continue;
	} 
       
	if(!msg->transaction)
        {
		syslog(LOG_WARNING, "Received malformed reply");
                msg_free(msg);
                continue;
	}

//        syslog(LOG_DEBUG, "got message %s", msg->transaction->key);

        /* process message */
        if(msg->transaction->type == MSG_TRANS_SET)
        {
            /*-----------------*/
            /*    n2v_notify   */
            /*-----------------*/


            if(!safe_strcmp(msg->transaction->key, "command/n2v_notify"))
            {
                if(msg->transaction->values->value[0])
                  value = atoi(msg->transaction->values->value[0]);
                else
                {
                    syslog(LOG_DEBUG, "NULL value in n2v_notify\n");
                    continue;
                }

                discard = 0;
                switch(value)
                {
                  case VOPNSPIF_NOTIFY_NOERR:
                    syslog(LOG_NOTICE,"VOPNSPIF_NOTIFY_NOERR");
                    notify = VOPNSPIF_NOTIFY_NOERR;
                    break;
                  case N2V_REQ_CFG:
                    syslog(LOG_NOTICE,"N2V_REQ_CFG");
                    notify = N2V_REQ_CFG;
                    break;
                  case N2V_PREP_RESTART:
                    syslog(LOG_NOTICE,"N2V_PREP_RESTART");
                    notify = N2V_PREP_RESTART;
                    break;
                  default:
                    discard = 1;
                    break;
                }
                info = 0;
                if(msg->transaction->values->value[1])
                  info = atoi(msg->transaction->values->value[1]);
                if(discard == 0)
                  p_n2v->n2v_notify(notify,info);
            }

            /*-----------------*/
            /*  n2v_config_cmd */
            /*-----------------*/

            if(!safe_strcmp(msg->transaction->key, "command/n2v_config_cmd"))
            {
                syslog(LOG_DEBUG, "n2v_config_cmd");
                if(!msg || !msg->transaction || !msg->transaction->values)
                {
                  return VOPNSPIF_INVALID_SYNTAX;
                }

                if(!msg->transaction->values->value[0])
                {
                    syslog(LOG_DEBUG, "n2v string: Got NULL value\n");
                    p_n2v->n2v_config_cmd("");
                }
                else
                {
                    syslog(LOG_DEBUG, "n2v string: %s\n", msg->transaction->values->value[0]);
                    if(!safe_strcmp(msg->transaction->values->value[0], "activate"))
                      sleep(3);
                    p_n2v->n2v_config_cmd(msg->transaction->values->value[0]);
                }
            }

            /*-----------------*/
            /*  n2v_configure  */
            /*-----------------*/

            if(!safe_strcmp(msg->transaction->key, "command/n2v_configure"))
            {
//                syslog(LOG_DEBUG, "n2v_configure [%s:%s]", msg->transaction->values->value[1], msg->transaction->values->value[2]);

                /*-----------------------------*/
                /* Process the starting buffer */
                /*-----------------------------*/

                if(!safe_strncmp(msg->transaction->values->value[2], "START", 5))
                {
                    
                    /* r_ptr is where we will capture the base64 decoded data */

                    r_ptr = &(vcb_ptr->rle_buff[0]);

                    /* clean the decompression and RLE buffers */

                    memset(&(vcb_ptr->rle_buff[0]), 0,  VOICE_RLE_BUFFER_SIZE);
                    vcb_ptr->size = 0;
                    vcb_ptr->last_seq_cnt = 0;
                }

                /*-----------------------*/
                /* Default configuration */
                /*-----------------------*/

                if(!safe_strncmp(msg->transaction->values->value[2], "DEFAULT_CONFIG", 14 ))
                {
                    /* 
                       This is a default configuration, this is sent if no configuration 
                       is found within the flash
                    */
                    p_n2v->n2v_configure(0,(char *)0, atoi(msg->transaction->values->value[4]));
                    continue;
                }


                seq_cnt = atoi(msg->transaction->values->value[1]);
                chunk_size= strlen(msg->transaction->values->value[0]);

                /* If a zero length string..don't process */

                if(chunk_size > 0)
                {
                    /* If the incoming data is larger than the buffer...bail */

                    if((vcb_ptr->size + chunk_size) > VOICE_RLE_BUFFER_SIZE)
                    {
                        syslog(LOG_CRIT, "Voice configuration file larger than buffer\n");
                        continue;
                    }

                    c_ptr = (r_ptr + vcb_ptr->size);

                    /* Replace missing sequences with "A"'s */

                    if(((vcb_ptr->last_seq_cnt + 1) != seq_cnt)  && (seq_cnt != 0))
                    {
                        total_loops = (seq_cnt - vcb_ptr->last_seq_cnt - 1) * (VOICE_CONFIG_BUFFER_CHUNK - 1);
                        if((c_ptr + total_loops) > (c_ptr + (VOICE_RLE_BUFFER_SIZE)))
                        {
                            syslog(LOG_CRIT, "Invalid configuration load");
                            continue;
                        }
                        for(i=0; i < total_loops; i++)
                        {
                            *c_ptr = 'A';
                            c_ptr++;
                        }
                        

                        syslog(LOG_NOTICE,"Filling with %d A's\n",total_loops);

                        /* finish by inserting new "non-blank" data */

                        strncpy(c_ptr, msg->transaction->values->value[0], chunk_size);
                        vcb_ptr->size += (chunk_size + total_loops);
                    }
                    else
                    {
                        strncpy(c_ptr, msg->transaction->values->value[0], chunk_size);
                        vcb_ptr->size += chunk_size;
                    }
                    

                    vcb_ptr->last_seq_cnt = seq_cnt;

                    syslog(LOG_NOTICE, "Received:[%s]:%d:%d\n",msg->transaction->values->value[1],chunk_size, vcb_ptr->size);
                }
                else
                  syslog(LOG_NOTICE, "zero length string[%s]\n",msg->transaction->values->value[1]);


                /*----------------------------------*/
                /* Process the last incoming buffer */
                /*----------------------------------*/


                if(!safe_strncmp(msg->transaction->values->value[2], "EOB_VCONFIG",11))                    
                {
                    /*--------------------------*/
                    /* Base64 decode the buffer */
                    /*--------------------------*/
                    r_ptr  =  &(vcb_ptr->rle_buff[0]);

#if 0
                    file = fopen(fname,"w");
                    fwrite(r_ptr, 1, vcb_ptr->size, file);
                    fclose(file);
#endif

                    if((db = io_b64dec(r_ptr, &b64dec_len)) == NULL)
                    {
                        syslog(LOG_CRIT, "ERROR: Could not base64 Decode Voice config file\n");
                        continue;
                    }

                    /* one last check .... */
                    if(b64dec_len != atoi(msg->transaction->values->value[3]))
                    {
                        syslog(LOG_CRIT, "ERROR: Image size does not match stored image\n");
                        continue;
                    }

                    syslog(LOG_NOTICE, "base64 decoded size = %d\n", b64dec_len);
                    
                    p_n2v->n2v_configure(b64dec_len, db, atoi(msg->transaction->values->value[4]));                  
                }

            }

            /*------------------*/
            /* n2v_send_ip_addr */
            /*------------------*/

            if(!safe_strcmp(msg->transaction->key, "command/n2v_send_ip_addr"))
            {

                if(inet_aton(msg->transaction->values->value[0], &iface) < 0)
                {
                    syslog(LOG_DEBUG, "improper IP address");
                    continue;
                }
                syslog(LOG_DEBUG, "n2v_send_ip_address: %s %x ", msg->transaction->values->value[0],iface.s_addr );
                p_n2v->n2v_send_ip_addr(iface.s_addr);
            }

            /*------------------*/
            /* cli_debug_mode   */
            /*------------------*/
            if(!safe_strcmp(msg->transaction->key, "command/cli_debug_mode"))
            {
                syslog(LOG_DEBUG, "setting cli debug mode\n");
                if(atoi(msg_getTransValue(msg, 0, 0)))
                {
                    /* turn cli debug mode on */
                    nsp_cli_debug=1;
                }
                else
                {
                    /* turn cli debug mode off */
                    nsp_cli_debug=0;
                }
            }
        }
        msg_free(msg);
    }
}

/*----------------------------------*/
/* messages from Voice to NSP (V2N) */
/*----------------------------------*/
VOPNSPIF_ERR_T nspapi_v2n_notify(VOPNSPIF_NOTIFY_T notification, UINT32 info)
{
    char* key;
    char notif_value[25];
    char info_buff[11];
    char * nv_ptr;
    struct msg_base *msg;
    
    key = "command/v2n_notify";

//    syslog(LOG_DEBUG, "nspapi_v2n_notify" );   

    msg = msgr_makeTransBase("voice_app", "voice", MSG_TRANS_SET, "voiceapp_group", key, 0, 0, 0, 0);
    if(!msg)
     return VOPNSPIF_INVALID_RET;

    nv_ptr = (char *)&notif_value;
    
    switch(notification)
    {
      case V2N_CFG_ERR:
        syslog(LOG_DEBUG, "V2N_CFG_ERR");
        sprintf(nv_ptr, "%d", V2N_CFG_ERR);
        break;
      case V2N_REQ_RESTART:
        syslog(LOG_DEBUG, "V2N_REQ_RESTART");
        sprintf(nv_ptr, "%d", V2N_REQ_RESTART);
        break;
      case V2N_READY:
        syslog(LOG_DEBUG, "V2N_READY");
        sprintf(nv_ptr, "%d", V2N_READY);
        break;
      case V2N_TIMEOUT:
        syslog(LOG_DEBUG, "V2N_TIMEOUT");
        sprintf(nv_ptr, "%d", V2N_TIMEOUT);
        break;
      default:
        syslog(LOG_DEBUG, "V2N_BAD_MESSAGE");
        msg_free(msg);
        return VOPNSPIF_INVALID_PARAM;
    }
    sprintf((char *)info_buff, "%d", info);

    if(msgr_addTransValues(msg, 2, notif_value, info_buff) < 0)
	{
        syslog(LOG_DEBUG,"V2N_BAD_VALUE");
        msg_free(msg);
        return VOPNSPIF_INVALID_RET;
	}

    if(send_to_vm(msg)<0)
      return VOPNSPIF_INVALID_PARAM;

    return VOPNSPIF_NO_ERR;
}

VOPNSPIF_ERR_T nspapi_v2n_config_response(VOPNSPIF_CONFIG_RESP_T response)
{
    char* key = "command/v2n_config_response";
    char resp_value[25];
    char *rv_ptr=NULL;
    struct msg_base *msg = msgr_makeTransBase("voice_app", "voice", MSG_TRANS_SET, "voiceapp_group", key, 0, 0, 0, 0);
    if(!msg)
    {
     return VOPNSPIF_INVALID_RET;
    }

//    syslog(LOG_DEBUG, "v2n_config_response");
    rv_ptr = (char *)resp_value;

    switch(response)
    {
      case V2N_CFG_RESP_NOERR:
        sprintf(rv_ptr, "%d", V2N_CFG_RESP_NOERR);
        break;
      case V2N_CFG_RESP_ERR:
        sprintf(rv_ptr, "%d", V2N_CFG_RESP_ERR);
        break;
      case V2N_CFG_RESP_UNAVAILABLE:
        sprintf(rv_ptr, "%d", V2N_CFG_RESP_UNAVAILABLE);
        break;
      default:
        syslog(LOG_DEBUG,"V2N_BAD_RESPONSE");
        msg_free(msg);
        return VOPNSPIF_INVALID_PARAM;
    }
    
    if(msgr_addTransValues(msg, 1, resp_value) < 0)
    {
        syslog(LOG_DEBUG,"V2N_BAD_RESPONSE_VAL");
        msg_free(msg);
      return VOPNSPIF_INVALID_RET;
    }

    if(send_to_vm(msg)<0)
      return VOPNSPIF_INVALID_RET;
    
    return VOPNSPIF_NO_ERR;
}


VOPNSPIF_ERR_T nspapi_v2n_config_save(UINT32 size, UINT8 *p_config, VOPNSPIF_CONFIG_T config_type  )
{
    char* key = "command/v2n_config_save";
    char* buffer;
    char* buffer_save;
    char* buffer_start;
    char* r_ptr;
    char* vrls_ptr;
    char  buffer_chunk[VOICE_CONFIG_BUFFER_CHUNK];
    char seq_cnt_buff[5];
    char size_buff[10];
    char config_type_buff[8];
    int chunk_count,seq_cnt,even_buffer_cnt,skip_chunk;
    int i,j, r_size, match_count, last_buffer, buffer_size;
    struct msg_base *msg;
    vcfg_buffer *vcb_ptr;
    char v_release_string[V_RELEASE_STRING_SIZE];

    syslog(LOG_NOTICE, "v2n_config_save: %d", config_type);
    syslog(LOG_NOTICE, "binary image size: %d\n", size);

    vcb_ptr = &vcb;
    r_ptr   = &vcb_ptr->rle_buff[0];
    memset(r_ptr, 0 , VOICE_RLE_BUFFER_SIZE);
    
	sprintf(config_type_buff, "%d", config_type);

#if defined(RLE_COMPRESSION)
    /* Run length encode the binary file */
    r_size = RLE_Compress(p_config, r_ptr, size);
    syslog(LOG_NOTICE, "RLE buffer size = %d\n",r_size);
    buffer = io_b64enc(r_ptr, r_size);
#endif

    buffer = io_b64enc(p_config, size);
    buffer_save = buffer;
    buffer_size = strlen(buffer_save);
    syslog(LOG_NOTICE, "buffer size: %d\n", buffer_size);
    
    even_buffer_cnt=0;
    chunk_count=0;
    seq_cnt=0;
    skip_chunk = 0;
    last_buffer = 0;
    
    sprintf(size_buff, "%d", size);

    /*---------------------------------------------*/
    /* Get the release string from the binary blob */
    /*---------------------------------------------*/
    vrls_ptr = p_config;

    strncpy(&v_release_string[0], vrls_ptr, V_RELEASE_STRING_SIZE);
    syslog(LOG_NOTICE, "Voice Release string: %s\n", v_release_string);
    
    /*------------------------------------------------*/
    /* divide up message up the message into "chunks" */
    /*------------------------------------------------*/

    for(i=0; i < buffer_size; i++)
    {
        buffer_chunk[chunk_count] = *buffer++;
        chunk_count++;
    
        if((chunk_count == (VOICE_CONFIG_BUFFER_CHUNK-1)))
        {
            buffer_chunk[chunk_count] = '\0';

            /*---------------------------*/
            /*   poor man's compression  */
            /*---------------------------*/

            match_count = 0;
            for(j=0; j < chunk_count; j++)
            {
                if(!safe_strncmp(&buffer_chunk[j], "A", 1))
                  match_count++;
            }

            if(match_count == (VOICE_CONFIG_BUFFER_CHUNK -1))
            {
                if(seq_cnt == 0)
                  skip_chunk = 0;
                else
                  skip_chunk = 1;
            }

            msg = msgr_makeTransBase("voice_app", "voice", MSG_TRANS_SET, "voiceapp_group", key, 0, 0, 0, 0);
            if(!msg)
            {
                syslog(LOG_NOTICE, "could not create message\n");
                return VOPNSPIF_INVALID_RET;
            }
            sprintf(seq_cnt_buff, "%d", seq_cnt);
            
            if(i == (buffer_size - 1))
            {
                /*-------------------------*/
                /* This is the last buffer */
                /*-------------------------*/

                nspapi_v2n_text_put_string(".");                                                                      

                /* this is the last buffer...tag it so.. */
                if(msgr_addTransValues(msg, 6, buffer_chunk, seq_cnt_buff, "EOB_VCONFIG", size_buff, v_release_string, config_type_buff)<0)
                {
                    syslog(LOG_NOTICE, "[voice_app]:config_save: could not add value to msg\n");
                    msg_free(msg);
                    return VOPNSPIF_INVALID_RET;
                }
                even_buffer_cnt = 1;
                last_buffer = 1;
            }
            else 
            {   
                if(seq_cnt == 0)
                {   
                    /*-----------------------*/
                    /* This the first buffer */
                    /*-----------------------*/
                    if(msgr_addTransValues(msg, 6, buffer_chunk, seq_cnt_buff, "START", size_buff, v_release_string, config_type_buff)<0)
                    {
                        syslog(LOG_NOTICE, "[voice_app]:config_save: could not add value to msg\n");
                        msg_free(msg);
                        return VOPNSPIF_INVALID_RET;
                    }
                }
                else
                {
                    /*--------------------------------*/
                    /* This is a continuation buffer  */
                    /*--------------------------------*/

                    if(msgr_addTransValues(msg, 6, buffer_chunk, seq_cnt_buff, "CONT", size_buff, v_release_string, config_type_buff)<0)
                    {
                        syslog(LOG_NOTICE, "[voice_app]:config_save: could not add value to msg\n");
                        msg_free(msg);
                        return VOPNSPIF_INVALID_RET;
                    }
                }
            }

            if(skip_chunk == 0)
              nspapi_v2n_text_put_string(".");

            seq_cnt++;
            chunk_count=0;

            /*----------------------*/ 
            /* Prep for next buffer */
            /*----------------------*/

            memset(seq_cnt_buff, 0, sizeof(seq_cnt_buff));
            memset(buffer_chunk, 0, sizeof(buffer_chunk));

            /*------------------*/
            /* send the message */
            /*------------------*/
            if((skip_chunk == 0) || (last_buffer == 1))
            {
                if(send_to_vm(msg)<0)
                {
                    syslog(LOG_NOTICE, "[voice_app]:config_save: could not send the msg\n");
                    return VOPNSPIF_INVALID_RET;
                }
            }
            else
              free(msg);
            skip_chunk = 0;
        }
    }


    /* if we filled the last buffer exactly...skip this */
 
   if(even_buffer_cnt == 0)
    {
        msg = msgr_makeTransBase("voice_app", "voice", MSG_TRANS_SET, "voiceapp_group", key, 0, 0, 0, 0);
        if(!msg)
        {
            syslog(LOG_DEBUG, "could not create message\n");
            msg_free(msg);
            return VOPNSPIF_INVALID_RET;
        }
	#if 0
        buffer_chunk[chunk_count-1] = '\0';   
	#else
        buffer_chunk[chunk_count] = '\0';
	#endif
        sprintf(seq_cnt_buff, "%d", seq_cnt);
        syslog(LOG_NOTICE, "last_buffer[%s i=%d size=%d]\n",seq_cnt_buff,i,chunk_count);
        if(msgr_addTransValues(msg, 6, buffer_chunk, seq_cnt_buff, "EOB_VCONFIG", size_buff, v_release_string, config_type_buff)<0)
        {
            syslog(LOG_DEBUG,"could not add value to message");
            msg_free(msg);
          return VOPNSPIF_INVALID_RET;
        }
        if(send_to_vm(msg)<0)
          return VOPNSPIF_INVALID_RET;
    }
    
    sleep(15);

    free(buffer_save);

    return VOPNSPIF_NO_ERR;
}

VOPNSPIF_ERR_T nspapi_v2n_call_notification(UINT32 num_of_calls, 
                                     UINT32 voice_packet_size, 
                                     UINT32 inter_packet_time)
{
    char* key;
    char notif_value[25];
    char noc_buff[2];
    char vps_buff[11];
    char ipt_buff[11];
    struct msg_base *msg;

    key = "command/v2n_call_notification";

    msg = msgr_makeTransBase("voice_app", "voice", MSG_TRANS_SET, "voiceapp_group", key, 0, 0, 0, 0);
    if(!msg)
     return VOPNSPIF_INVALID_RET;

    syslog(LOG_DEBUG, "v2n_call_notification" );
    
    sprintf(noc_buff, "%d", num_of_calls);
    sprintf(vps_buff, "%d", voice_packet_size);
    sprintf(ipt_buff, "%d", inter_packet_time);
    if(msgr_addTransValues(msg, 3, noc_buff, vps_buff, ipt_buff ) < 0)
    {
        syslog(LOG_DEBUG,"V2N_CALL_NOTIF_ERROR");
        msg_free(msg);
        return VOPNSPIF_INVALID_RET;
    }

    if(send_to_vm(msg)<0)
      return VOPNSPIF_INVALID_RET;

    return VOPNSPIF_NO_ERR;
}

VOPNSPIF_ERR_T nspapi_v2n_firewall_action(VOPNSPIF_FIREWALL_ACTION_T action, 
                                   VOPNSPIF_TRANSPORT_PROT_T protocol,
                                   UINT32 port_start,
                                   UINT32 port_end,
                                   VOPNSPIF_PORT_TYPE_T port_type)
{
    char* key;
    char act[2];
    char * a_ptr=NULL;
    char prot[2];
    char * p_ptr=NULL;
    char port_start_buff[11];
    char port_end_buff[11];
    char port_type_buff[11];
    struct msg_base *msg;
    
    key = "command/v2n_firewall_action";

    msg = msgr_makeTransBase("voice_app", "voice", MSG_TRANS_SET, "voiceapp_group", key, 0, 0, 0, 0);
    if(!msg)
    {
     return VOPNSPIF_INVALID_RET;
    }

    a_ptr=(char*)act;
    p_ptr=(char*)prot;

   switch(action)
    {
      case V2N_FIREWALL_OPEN:
        sprintf(a_ptr, "%d", V2N_FIREWALL_OPEN);
        break;
      case V2N_FIREWALL_CLOSE:
        sprintf(a_ptr, "%d", V2N_FIREWALL_CLOSE);
        break;
      default:
        syslog(LOG_DEBUG,"V2N_FIREWALL_ERROR1");
        msg_free(msg);
        return VOPNSPIF_INVALID_PARAM;
    }

   switch(protocol)
   {
     case V2N_TRANS_PROT_UDP:
        sprintf(p_ptr, "%d", V2N_TRANS_PROT_UDP);
        break;
      case V2N_TRANS_PROT_TCP:
        sprintf(p_ptr, "%d", V2N_TRANS_PROT_TCP);
        break;
      default:
        syslog(LOG_DEBUG,"V2N_FIREWALL_ERROR2");
        msg_free(msg);
        return VOPNSPIF_INVALID_PARAM;      
   }
   
   sprintf(port_start_buff, "%d", port_start);
   sprintf(port_end_buff, "%d", port_end);
   sprintf(port_type_buff, "%d", port_type);

   syslog(LOG_DEBUG, "v2n_firewall_action [a:%s pr:%s ps:%s pe:%s pt:%s]\n", act , prot, port_start_buff, port_end_buff, port_type_buff);   
   if(msgr_addTransValues(msg, 5, act , prot, port_start_buff, port_end_buff, port_type_buff) < 0)
	{
       syslog(LOG_DEBUG,"V2N_FIREWALL_VALUE_ERROR");
       msg_free(msg);
       return VOPNSPIF_INVALID_RET;
	}

   if(send_to_vm(msg)<0)
	{
     return VOPNSPIF_INVALID_RET;
	}
   
   return VOPNSPIF_NO_ERR;

}


/* Since this function is called from different VOP-NSP APIs, added critical section around it. */
int send_to_vm(struct msg_base *msg)
{
	CriticalSectionContext  crit;
	
	X_beginCriticalSection(&crit);
    msgr_sendTransGroupBegin("voice_app", "manager", "voiceapp_group");

	syslog(LOG_DEBUG, "(V2N): %s", msg->transaction->key);

    if(msg_send(msg)<0)
    {
        syslog(LOG_DEBUG, "Could not send message %s from send_to_vm", msg->transaction->key);
        msg_free(msg);
		X_endCriticalSection(crit);
        return -1;
    }

    msgr_sendTransGroupEnd("voice_app", "manager", "voiceapp_group");
//    msg_free(msg);
	
	X_endCriticalSection(crit);
	 
    return VOPNSPIF_NO_ERR;
}


VOPNSPIF_ERR_T nspapi_v2n_text_put_string(UINT8 *string)
{

    struct msg_base *msg;
    char* key = "command/v2n_text_put_string";

    if(!string)
    {
//        syslog(LOG_DEBUG, "null string");
        return VOPNSPIF_INVALID_RET;
    }

    if(nsp_cli_debug == 1)
    {
//        syslog(LOG_DEBUG, "v2n_text_put_string %s\n", string);
        msg = msgr_makeTransBase("voice_app", "voice", MSG_TRANS_SET, "voiceapp_group", key, 0, 0, 0, 0);
        if(!msg)
        {
            syslog(LOG_DEBUG, "Could not allocate msg");
            return VOPNSPIF_INVALID_RET;
        }
        
        
        if(msgr_addTransValues(msg, 1, string) < 0)
        {
            syslog(LOG_DEBUG, "Could not add value to message");
            msg_free(msg);
            return VOPNSPIF_INVALID_RET;
        }
        
        if(send_to_vm(msg)<0)
        {
            syslog(LOG_DEBUG, "Could not send message: %s %s", key, string );
            return VOPNSPIF_INVALID_RET;
        }
    }
    else
      return VOPNSPIF_NO_ERR;

//#if 0
//    if(!string)
//      return VOPNSPIF_INVALID_RET;
//    syslog(LOG_DEBUG, "%s", string);
//
//#endif
}


VOPNSPIF_ERR_T nspapi_v2n_syslog(UINT8 *string)
{
    if(!string)
      return VOPNSPIF_INVALID_RET;
    syslog(LOG_NOTICE, "%s", string);
}


 void* get_vop_to_nsp_interface(void)
{
    return ((void*) &vop_to_nsp_if);
}

                                  
