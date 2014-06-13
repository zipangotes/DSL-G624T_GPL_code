/*
 * Copyright (c) 2002 
 *      Texas Instruments.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Texas Instruments. 
 * 4. Neither the name of the Company nor of the product may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#ifndef __LED_APP__
#define __LED_APP__

#define CONF_FILE		"/etc/led.conf"
#define LED_PROC_FILE		"/proc/led_mod/led"

#define CONFIG_LED_MODULE

#define MAX_MOD_ID        25
#define MAX_STATE_ID   	  25
#define MAX_LED_ID 	  25

#define MOD_ADSL 1
#define DEF_ADSL_IDLE     1
#define DEF_ADSL_TRAINING 2
#define DEF_ADSL_SYNC     3
#define DEF_ADSL_ACTIVITY 4

#define MOD_WAN  2
#define DEF_WAN_IDLE      1
#define DEF_WAN_NEGOTIATE 2
#define DEF_WAN_SESSION   3

#define MOD_LAN  3
#define DEF_LAN_IDLE		  1
#define DEF_LAN_LINK_UP   2
#define DEF_LAN_ACTIVITY  3

#define MOD_WLAN 4
#define DEF_WLAN_IDLE     1
#define DEF_WLAN_LINK_UP  2
#define DEF_WLAN_ACTIVITY 3

#define MOD_USB  3
#define DEF_USB_IDLE		  1
#define DEF_USB_LINK_UP   2
#define DEF_USB_ACTIVITY  3

typedef struct config_elem{
  unsigned char name;
  unsigned char state;
  unsigned char mode;
  unsigned char led;
            int param;
}config_elem_t;

typedef struct led_reg{
	unsigned int param;
  void (*init)(unsigned long param);
  void (*onfunc)(unsigned long param);
  void (*offfunc)(unsigned long param);
}led_reg_t;

#endif
