/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 1998-2003 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission is hereby granted to licensees of Texas Instruments           |**
**| Incorporated (TI) products to use this computer program for the sole     |**
**| purpose of implementing a licensee product based on TI products.         |**
**| No other rights to reproduce, use, or disseminate this computer          |**
**| program, whether in part or in whole, are granted.                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/
 
/**************************************************************************
 * FILE PURPOSE :   USB Admin Tool
 **************************************************************************
 * FILE NAME    :   usb20_tool.c
 *
 * DESCRIPTION  :
 *
 * HISTORY : 
 *
 * 10/18/2003   sabyasachi    Created.
 * 10/26/2003   sabyasachi    Modified for stat
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usb20_tool.h"
#include "usb20_rndis.h"
#include "usb20_drv.h"
#include "usb20_mib2.h"
#include "usb20_config.h"
#include "usb20_drvrndis.h"

extern void usb_stat(void);
extern int rndis_usb_get_mib_stats( void* ptr_rndis_mcb, int command, void * data);
extern int isalnum(char);

static USB_DEV *usb_cb;

static int cmd_size;
static USB_ADMIN_CMD *cmd_list;

static void usb20_help(char **cmd, int n_cmd)
{
     int  ii;

     if ( 1 == n_cmd )
     	{
		printf("usb admin help.\n ");
     	     for ( ii = 1 ; ii < cmd_size ; ii++ ) 
	     {
			printf("\t%s\t\t%s\n",cmd_list[ii].name, cmd_list[ii].help_str);
	     }	
     	}
     else 
     	{
     	for ( ii = 1 ; ii < cmd_size ; ii++ ) 
     	    {
		if ( strcmp(cmd[1], cmd_list[ii].name) == 0 )
		{
			cmd_list[ii].help_action();
			return;
		}
     	     }
     	}

	 return;
}

static void usb20_set_help()
{
    printf("\nset <param> <value>\n");
    printf("\tvid\n");
    printf("\tpid\n");
    printf("\tserial_no\n");
#ifdef USB_MIB2
    printf("\tadmin_status\n");
#endif 
    printf("\n");
}

static void usb20_set(char **cmd_list, int n_cmd)
{
     printf("set is not supported now.\n");
#if 0	 
     if ( n_cmd < 3 ) 
	  return;
	 
     if (strcmp(cmd_list[1],"vid") == 0 )
    {
	    sscanf(cmd_list[2],"%d",&usb_cb->params.vid);
    }
    else if ( strcmp(cmd_list[1],"pid") == 0 ) 
    {
	    sscanf(cmd_list[2],"%d",&usb_cb->params.pid);
    }
    else if ( strcmp(cmd_list[1],"serial_no") == 0 ) 
    {
    	   strcpy(usb_cb->params.serial_no, cmd_list[2]);
    }
#ifdef USB_MIB2
   else if ( strcmp(cmd_list[1], "admin_status") == 0 ) 
   	{
   	}
#endif
    else
    	{
    	printf("Invalid command\n");
    	}
#endif 	
return ;
}

static void usb20_show_help()
{
    printf("\nshow <param>\n");
    printf("\tvid\n");
    printf("\tpid\n");
    printf("\tserial_no\n");
    printf("\tctrl_ep_size\n");
    printf("\tbulk_ep_size\n");
    printf("\tint_ep_size\n");
    printf("\taddress\n");
    printf("\tspeed\n");
    printf("\tlink_status\n");
    printf("\tall\n");
    printf("\n");
}

static void usb20_show(char **cmd_list, int n_cmd)
{
    printf("\n");
    if ( (n_cmd <= 1 ) || strcmp(cmd_list[1],"all") == 0 ) 
    {
	printf("Vendor ID    \t\t:  0x%x\n",usb_cb->params.vid);
	printf("Product ID   \t\t:  0x%x\n",usb_cb->params.pid);
	printf("Serial No    \t\t:  %s\n",(char *)usb_cb->params.serial_no);
	printf("Control EP Size \t:  %d\n",usb_cb->params.ctrl_ep_size);
	printf("Bulk EP Size \t\t:  %d\n",usb_cb->params.bulk_ep_size);
	printf("Interrupt EP Size \t:  %d\n",usb_cb->params.int_ep_size);
	printf("Address \t\t:  %d\n",usb_cb->params.address);
	printf("Link Status \t\t:  %s\n",(usb_cb->link_status == 1 ? "UP" : "DOWN"));
	if ( usb_cb->link_status ) 
	{
	    printf("Link Speed  \t\t:  %d Mbps [%s]\n",(usb_cb->params.speed)/1000000,(usb_cb->params.speed == 12000000 ? "Full Speed":"High Speed"));
	}
    }
    else if (strcmp(cmd_list[1],"vid") == 0 )
    {
	printf("Vendor ID : 0x%x\n",usb_cb->params.vid);
    }
    else if ( strcmp(cmd_list[1],"pid") == 0 ) 
    {
	printf("Product ID : 0x%x\n",usb_cb->params.pid);
    }
    else if ( strcmp(cmd_list[1],"serial_no") == 0 ) 
    {
	printf("Serial No : %s\n", (char *)usb_cb->params.serial_no);
	
    }
    else if ( strcmp(cmd_list[1],"bulk_ep_size") == 0 ) 
    {
	printf("Bulk EP Size : %d\n",usb_cb->params.bulk_ep_size);
    }
    else if ( strcmp(cmd_list[1],"speed") == 0 ) 
    {
       if ( usb_cb->link_status ) 
	    printf("Link Speed : %d Mbps [%s]\n",(usb_cb->params.speed)/1000000, (usb_cb->params.speed == 12000000 ? "Full Speed":"High Speed"));
	 else
	    printf("Link Speed : 0 Mbps. Link Down.\n");
    }
    else if ( strcmp(cmd_list[1],"ctrl_ep_size") == 0 ) 
    {
	printf("Control EP Size : %d\n",usb_cb->params.ctrl_ep_size);
    }
    else if ( strcmp(cmd_list[1],"int_ep_size") == 0 ) 
    {
	printf("Interrupt EP Size : %d\n",usb_cb->params.int_ep_size);
    }
    else if ( strcmp(cmd_list[1],"link_status") == 0 ) 
    {
	printf("Link Status : %s\n",(usb_cb->link_status == 1 ? "UP" : "DOWN"));
    }
    else if (strcmp(cmd_list[1],"address") == 0)
    {
    	printf("Address \t\t:  %d\n",usb_cb->params.address);
    }
    printf("\n");
}


static void usb20_exit_help()
{
	printf("\nexit\t\tquit from usb admin shell.\n\n");
}

static void usb20_statshow(char **cmd_list, int n_cmd)
{
    usb_stat();
}

#ifdef USB_MIB2

static void usb20_mib2stats(char **cmd_list, int n_cmd)
{
    char str1[8], str2[8];
    USB_MIB2_USB_PARAMS    usb_param;
    USB_MIB2_IF_COUNTERS   if_cntr;

    usb_param.usbCDCEtherPacketFilter = &str1[0];
    usb_param.usbCDCEtherStatCapabilities = &str2[0];
    rndis_usb_get_mib_stats( hal_usb_get_app_handle(usb_cb), SIOCGUSBPARAMS, (void *)&usb_param);
    rndis_usb_get_mib_stats( hal_usb_get_app_handle(usb_cb), SIOCGINTFCOUNTERS, (void *)&if_cntr);
	
    printf("---------------------------------------------------\n");
    printf("                MIB2 IF COUNTERS                   \n");  
    printf("---------------------------------------------------\n");
 
    printf("%s\t\t:  %lu\n",  "inBytesLow", if_cntr.inBytesLow);   
    printf("%s\t\t:  %lu\n",  "inBytesHigh", if_cntr.inBytesHigh);      
    printf("%s\t:  %lu\n",  "inUnicastPktsLow", if_cntr.inUnicastPktsLow);
    printf("%s\t:  %lu\n",  "inUnicastPktsHigh", if_cntr.inUnicastPktsHigh);  
    printf("%s\t:  %lu\n",  "inMulticastPktsLow", if_cntr.inMulticastPktsLow); 
    printf("%s\t:  %lu\n",  "inMulticastPktsHigh", if_cntr.inMulticastPktsHigh); 
    printf("%s\t:  %lu\n",  "inBroadcastPktsLow", if_cntr.inBroadcastPktsLow); 
    printf("%s\t:  %lu\n",  "inBroadcastPktsHigh", if_cntr.inBroadcastPktsHigh); 
    printf("%s\t\t:  %lu\n",  "inDiscardPkts", if_cntr.inDiscardPkts);  
    printf("%s\t\t:  %lu\n",  "inErrorPkts", if_cntr.inErrorPkts);   
    printf("%s\t:  %lu\n",  "inUnknownProtPkts", if_cntr.inUnknownProtPkts);
    printf("%s\t\t:  %lu\n",  "outBytesLow", if_cntr.outBytesLow);   
    printf("%s\t\t:  %lu\n",  "outBytesHigh", if_cntr.outBytesHigh); 
    printf("%s\t:  %lu\n",  "outUnicastPktsLow", if_cntr.outUnicastPktsLow); 
    printf("%s\t:  %lu\n",  "outUnicastPktsHigh", if_cntr.outUnicastPktsHigh);  
    printf("%s\t:  %lu\n",  "outMulticastPktsLow", if_cntr.outMulticastPktsLow);
    printf("%s\t:  %lu\n",  "outMulticastPktsHigh", if_cntr.outMulticastPktsHigh);
    printf("%s\t:  %lu\n",  "outBroadcastPktsLow", if_cntr.outBroadcastPktsLow); 
    printf("%s\t:  %lu\n",  "outBroadcastPktsHigh", if_cntr.outBroadcastPktsHigh);
    printf("%s\t\t:  %lu\n",  "outDiscardPkts", if_cntr.outDiscardPkts);     
    printf("%s\t\t:  %lu\n",  "outErrorPkts", if_cntr.outErrorPkts);      


    printf("---------------------------------------------------\n"); 
    printf("                MIB2 USB PARAMS                    \n"); 
    printf("---------------------------------------------------\n"); 

    printf("%s\t\t\t:  %ld\n", "usbDevicePower", usb_param.usbDevicePower);
    printf("%s\t\t:  %lu\n", "usbDeviceVendorID", usb_param.usbDeviceVendorID);
    printf("%s\t\t:  %lu\n", "usbDeviceProductID", usb_param.usbDeviceProductID); 
    printf("%s\t:  %ld\n", "usbDeviceNumberConfigurations", usb_param.usbDeviceNumberConfigurations);
    printf("%s\t\t:  %ld\n", "usbDeviceActiveClass", usb_param.usbDeviceActiveClass);
    printf("%s\t\t\t:  %ld\n", "usbDeviceStatus", usb_param.usbDeviceStatus);
    printf("%s\t\t:  %lu\n", "usbDeviceNumEnumCounter", usb_param.usbDeviceNumEnumCounter);
    printf("%s\t\t:  %ld\n", "usbDeviceRemoteWakeup", usb_param.usbDeviceRemoteWakeup);
    printf("%s\t\t:  %ld\n", "usbDeviceRemoteWakeupOn", usb_param.usbDeviceRemoteWakeupOn);
    printf("%s\t\t\t:  %ld\n", "usbCDCSubclass", usb_param.usbCDCSubclass);
    printf("%s\t\t:  %ld\n", "usbCDCDataTransferType", usb_param.usbCDCDataTransferType);
    printf("%s\t\t\t:  %ld\n", "usbCDCEndpoints", usb_param.usbCDCEndpoints);
    printf("%s\t\t\t:  %lu\n", "usbCDCStalls", usb_param.usbCDCStalls);
    usb_param.usbCDCEtherPacketFilter[usb_param.usbCDCEtherPacketFilterLength] = '\0'; 
    printf("%s\t\t:  %s\n", "usbCDCEtherPacketFilter", usb_param.usbCDCEtherPacketFilter);
    printf("%s\t\t:  %ld\n", "usbCDCEtherPacketFilterLength", usb_param.usbCDCEtherPacketFilterLength);
    usb_param.usbCDCEtherStatCapabilities[usb_param.usbCDCEtherStatCapabilitiesLength] = '\0'; 
    printf("%s\t\t:  %s\n", "usbCDCEtherStatCapabilities", usb_param.usbCDCEtherStatCapabilities); 
    printf("%s\t\t:  %ld\n", "usbCDCEtherStatCapabilitiesLen", usb_param.usbCDCEtherStatCapabilitiesLength);
    printf("%s\t\t\t:  %hu\n", "usbCDCVersion", usb_param.usbCDCVersion);
}
#endif 

static void usb20_stat_help()
{
	printf("\nstat\t\tdisplay USB traffic statistics.\n\n");
}

/* 
  * Populate the command list.
  */
  
USB_ADMIN_CMD  usb_adm_cmd_list[] = 
{
   	{ "help", (USB_ADMIN_FUNC)usb20_help, NULL, NULL},
   	{ "set", usb20_set, usb20_set_help, "set various USB 2.0 Driver Config."},
   	{ "show", usb20_show, usb20_show_help, "display various USB 2.0 Driver Config."},
   	{ "usbstat", usb20_statshow, usb20_stat_help, "display USB RNDIS statistics."},
#ifdef USB_MIB2
   	{ "mib2stats", usb20_mib2stats, usb20_show_help, "display MIB-II statistics."},
#endif 
	{ "exit", NULL, usb20_exit_help, "quit from usb admin shell."},
};


static void get_cmd(char *cmd) 
{
	char ch;
	int  len = 0;
	while(len < 256)
	{
		ch = getchar();
		ch &= 0x7f;

		if (isalnum(ch) || ch == ' ') {
			if ((len + 1) < 256) {
				cmd[len++] = ch;
				/*putchar(ch);*/
			}
		} else if (ch == '\n' || ch == '\015') {
			cmd[len++] = '\0';
			/*putchar('\015');*/
			goto end;
		} else if (ch == '\010') {
			if (len) {
				len--;
				putchar('\010');
				putchar(' ');
				putchar('\010');
			}
			else
			   goto end;
		} 
		else
			len++;

		/*else {
			putchar('\007');
		}*/
	} /* while ( len < 256 ) */
end : 
	return;
}


extern USB_DEV *usb_drv_get_dev();

int usbadmin()
{
   char  cmd[256];
   char  *subcmd[10];
   int     n_cmd, ii;

   usb_cb = (USB_DEV *)usb_drv_get_dev();
   if ( NULL == usb_cb )
   {
   	printf("Error : Can not find USB device\n");
	return USB_STATUS_ERROR;
   }
   
   cmd_size = sizeof(usb_adm_cmd_list) / sizeof(USB_ADMIN_CMD);
   cmd_list = usb_adm_cmd_list;

   printf("\n\n");
   printf("\t####  USB 2.0 Driver Management Utility (Version %d.%d)  ####\n",USB_TOOL_VERSION_MAJOR,USB_TOOL_VERSION_MINOR);
   printf("\n\n");
   while ( 1 ) {
   printf("usb>");
   get_cmd(cmd);
   fflush(stdout);

   n_cmd = 0;
   subcmd[n_cmd] = strtok(cmd," ,");
   if ( subcmd[0] == NULL ) 
	   continue;
   if ( strcmp(subcmd[0],"exit") == 0 ) 
	   break;
   do 
   {
       n_cmd++;
       subcmd[n_cmd] = strtok(NULL," ,");
   }
   while ( subcmd[n_cmd] != NULL && n_cmd < 10);

   for ( ii = 0 ; ii < cmd_size ; ii++ ) 
   {
       if ( strcmp(subcmd[0], usb_adm_cmd_list[ii].name) == 0 ) 
       {
       	     usb_adm_cmd_list[ii].action(subcmd, n_cmd);
	     break;
       }
       if ( ii == cmd_size ) 
	       usb20_help(NULL, 1);
   }
   }

   return 0;
}


