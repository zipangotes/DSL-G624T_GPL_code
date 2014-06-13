/***************************************************************************
**+----------------------------------------------------------------------+**
**|                                ****                                  |**
**|                                ****                                  |**
**|                                ******o***                            |**
**|                          ********_///_****                           |**
**|                           ***** /_//_/ ****                          |**
**|                            ** ** (__/ ****                           |**
**|                                *********                             |**
**|                                 ****                                 |**
**|                                  ***                                 |**
**|                                                                      |**
**|     Copyright (c) 1998 - 2004 Texas Instruments Incorporated         |**
**|                        ALL RIGHTS RESERVED                           |**
**|                                                                      |**
**| Permission is hereby granted to licensees of Texas Instruments       |**
**| Incorporated (TI) products to use this computer program for the sole |**
**| purpose of implementing a licensee product based on TI products.     |**
**| No other rights to reproduce, use, or disseminate this computer      |**
**| program, whether in part or in whole, are granted.                   |**
**|                                                                      |**
**| TI makes no representation or warranties with respect to the         |**
**| performance of this computer program, and specifically disclaims     |**
**| any responsibility for any damages, special or consequential,        |**
**| connected with the use of this program.                              |**
**|                                                                      |**
**+----------------------------------------------------------------------+**
***************************************************************************/

/*!
 *                                                                         
 *   \file  dda_wpaauth_ioctl.h                                                
 *   \brief PURPOSE: defintion WPA configuration structure
 *                                                                         
 */

#ifndef __DDA_WPAAUTH_IOCTL_H
#define __DDA_WPAAUTH_IOCTL_H

#include "dda_wext_ioctl.h"

#define TIAP_WPA_MAX_RADIUS_SECRET_LEN 64
#define TIAP_WPA_MAX_PSK_PWD_LEN 63
#define TIAP_WPA_MIN_PSK_PWD_LEN 8


typedef enum {
	TIAP_WPA_WEPAUTH_OPEN,
	TIAP_WPA_WEPAUTH_SHARED,
	TIAP_WPA_WEPAUTH_BOTH
} tiap_wpa_wepauth_type_t;

#define TIAP_WPA_WEP_KEY_LEN_MAX 	29
#define TIAP_WPA_DEFS_KEY_NAL_LEN 	4
#define TIAP_WPA_WEP_40_KEY_LEN 	5
#define TIAP_WPA_WEP_104_KEY_LEN 	13
#define TIAP_WPA_WEP_232_KEY_LEN 	29

#define TIAP_MAX_NUM_OF_SUPPORTED_PROFILES   5


enum {
	TIAP_WPA_PSK_PARAM_NONE = 0,
	TIAP_WPA_PSK_PARAM_HEX,
	TIAP_WPA_PSK_PARAM_ASCII
};

#define TIAP_WPA_MIN_GTK_PERIOD		30

typedef struct {
	__u8 len; /*range: TIAP_WPA_WEP_40_KEY_LEN, TIAP_WPA_WEP_104_KEY_LEN,
                         TIAP_WPA_WEP_232_KEY_LEN default: TIAP_WPA_WEP_40_KEY_LEN*/
	__u8 key[TIAP_WPA_WEP_KEY_LEN_MAX]; /*range: any, default: {1,2,3,4,5}*/
} tiap_wpa_wep_key_t;

/*! \brief Pairwise and group cipher suites */ 
typedef enum {
    TIAP_WPA_CIPHER_NON     = 0x0,
    TIAP_WPA_CIPHER_TKIP    = 0x1,
    TIAP_WPA_CIPHER_WEP_40  = 0x2,
    TIAP_WPA_CIPHER_CCMP    = 0x4,
    TIAP_WPA_CIPHER_WEP_232 = 0x8,
	TIAP_WPA_CIPHER_WRAP    = 0x16,
    TIAP_WPA_CIPHER_WEP_104 = 0x32,
}tiap_wpa_cipher_t;

typedef struct {
	  tiap_param_security_type_t type;
	  __u8 wep_privacy;   /*0=OFF, 1=ON*/
	  tiap_wpa_wepauth_type_t wep_auth_type;
	  tiap_wpa_wep_key_t keys[TIAP_WPA_DEFS_KEY_NAL_LEN];
	  __u8 wep_def_key_id;   /*0-3, default: 0*/
	  __u32 default_cipher;
	  __u32 interval;     /*in milliseconds, default: 3600*1000 (1 hour)*/
} tiap_wpa_security_t;


typedef struct {
	__u32 serv_ip_addr;
	__u16 port;		/* default: 1812 */
	__s8 secret[TIAP_WPA_MAX_RADIUS_SECRET_LEN];
	__u8 secret_len;
} tiap_wpa_radius_t;

typedef struct {
	__s8 psk_passwd[TIAP_WPA_MAX_PSK_PWD_LEN];
	__u32 pass_phrase_mode;
	__u8 psk_passwd_len;
} tiap_wpa_auth_psk_t;

typedef struct {
	__u8 pre_auth_flag;
	__u32 sa_max_duration; /* max duration of PMK cache security record (SA) */
} tiap_wpa_pmk_t;

typedef struct {
	tiap_wpa_auth_psk_t psk;
	tiap_wpa_security_t sec;
	tiap_wpa_radius_t radius;
	tiap_wpa_pmk_t	    pmk;
	int is_active;   	
} tiap_sec_profile_t;


typedef struct {
	tiap_sec_profile_t profile[TIAP_MAX_NUM_OF_SUPPORTED_PROFILES];
	char dev_name[IFNAMSIZ];
} tiap_wpa_auth_t;

typedef struct {
	tiap_sec_profile_t sec_profiles[TIAP_MAX_NUM_OF_SUPPORTED_PROFILES];
	tiap_param_multi_ssid_t mssid;
	__u32 dbg_report_sev; /* WPA debug reports severity */
	__u32 dbg_report_mod; /* WPA debug reports subject */
}tiap_wpa_config_t;
#endif

