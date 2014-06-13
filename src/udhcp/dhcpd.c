/* dhcpd.c
 *
 * Moreton Bay DHCP Server
 * Copyright (C) 1999 Matthew Ramsay <matthewr@moreton.com.au>
 *			Chris Trew <ctrew@moreton.com.au>
 *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>

#include "debug.h"
#include "dhcpd.h"
#include "arpping.h"
#include "socket.h"
#include "options.h"
#include "files.h"
#include "leases.h"
#include "packet.h"
#include "serverpacket.h"
#include "pidfile.h"


/* globals */
struct dhcpOfferedAddr *leases;
struct server_config_t server_config[MAX_INTERFACES];
int no_of_ifaces = 0;

#define LEASE_ADD		1
#define LEASE_DEL 	2
extern void write_to_delta(u_int8_t *chaddr, u_int32_t yiaddr, u_int8_t *hname,unsigned long leasetime,u_int8_t action);

/* Exit and cleanup */
static void exit_server(int retval, int ifid)
{
	pidfile_delete(server_config[ifid].pidfile);
	CLOSE_LOG();
	exit(retval);
}


/* SIGTERM handler */
static void udhcpd_killed(int sig, int ifid)
{
	sig = 0;
	LOG(LOG_INFO, "Received SIGTERM");
	exit_server(0, ifid);
}


#ifdef COMBINED_BINARY	
int udhcpd(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{	
	fd_set rfds;
	struct timeval tv;
	int server_socket[MAX_INTERFACES];
	int bytes, retval;
	struct dhcpMessage packet;
	unsigned char *state;
	unsigned char *server_id, *requested;
	u_int32_t server_id_align, requested_align;
	unsigned long timeout_end[MAX_INTERFACES];
	struct option_set *option;
	struct dhcpOfferedAddr *lease;
	int pid_fd;
	int i;
  //int totalleases;

	
	OPEN_LOG("udhcpd");
	LOG(LOG_INFO, "udhcp server (v%s) started", VERSION);

	for (i = 0; i < MAX_INTERFACES; i++)
		memset(&server_config[i], 0, sizeof(struct server_config_t));
	
	if (argc < 2)
		read_config(DHCPD_CONF_FILE);
	else read_config(argv[1]);

	if (no_of_ifaces == 0)
		exit(0);

    /*  
	for (i = 0; i < no_of_ifaces; i++)
    totalleases += server_config[i].max_leases;
	leases = malloc(sizeof(struct dhcpOfferedAddr) * totalleases);
	memset(leases, 0, sizeof(struct dhcpOfferedAddr) * totalleases);  */
  
      		
	for (i = 0; i < no_of_ifaces; i++)
	{
		pid_fd = pidfile_acquire(server_config[i].pidfile);
		pidfile_write_release(pid_fd);

		if ((option = find_option(server_config[i].options, DHCP_LEASE_TIME))) {
			memcpy(&server_config[i].lease, option->data + 2, 4);
			server_config[i].lease = ntohl(server_config[i].lease);
		}
		else server_config[i].lease = LEASE_TIME;
	
		//leases = malloc(sizeof(struct dhcpOfferedAddr) * server_config[i].max_leases);
		//memset(leases, 0, sizeof(struct dhcpOfferedAddr) * server_config[i].max_leases);
    server_config[i].leases = malloc(sizeof(struct dhcpOfferedAddr) * server_config[i].max_leases);
		memset(server_config[i].leases, 0, sizeof(struct dhcpOfferedAddr) * server_config[i].max_leases);
		read_leases(server_config[i].lease_file, i);

		if (read_interface(server_config[i].interface, &server_config[i].ifindex,
			   &server_config[i].server, server_config[i].arp) < 0)
			server_config[i].active = FALSE;
		else
			server_config[i].active = TRUE;

#ifndef DEBUGGING
		pid_fd = pidfile_acquire(server_config[i].pidfile); /* hold lock during fork. */
		/* cfgmr req: do not fork */
		/*
		if (daemon(0, 0) == -1) {
			perror("fork");
			exit_server(1, i);
		}
		*/

		pidfile_write_release(pid_fd);
#endif
	
		signal(SIGUSR1, write_leases);
		signal(SIGTERM, udhcpd_killed);
	}

	for (i = 0; i < no_of_ifaces; i++) {
		timeout_end[i] = time(0) + server_config[i].auto_time;
		server_socket[i] = -1;
		LOG(LOG_INFO, "interface: %s, start : %x end : %x\n", server_config[i].interface, server_config[i].start, server_config[i].end);
	}

	while(1) { /* loop until universe collapses */
		for (i = 0; i < no_of_ifaces; i++)
		{			
			if (server_config[i].active == FALSE)
					continue;

			if (server_socket[i] < 0)				
				if ((server_socket[i] = listen_socket(INADDR_ANY, SERVER_PORT, server_config[i].interface)) < 0) {
					LOG(LOG_ERR, "FATAL: couldn't create server socket, %s", sys_errlist[errno]);
					exit_server(0, i);
				}			

			FD_ZERO(&rfds);
			FD_SET(server_socket[i], &rfds);
			
			if (server_config[i].auto_time) {
				tv.tv_sec = timeout_end[i] - time(0);
				if (tv.tv_sec <= 0) {
					tv.tv_sec = server_config[i].auto_time;
					timeout_end[i] = time(0) + server_config[i].auto_time;
					write_leases(i);
				}
				tv.tv_usec = 0;
			}
			retval = select(server_socket[i] + 1, &rfds, NULL, NULL, server_config[i].auto_time ? &tv : NULL);

			if (retval == 0) {
				write_leases(i);
				timeout_end[i] = time(0) + server_config[i].auto_time;
				continue;
			} else if (retval < 0) {
				DEBUG(LOG_INFO, "error on select");
				continue;
			}
		
			if ((bytes = get_packet(&packet, server_socket[i])) < 0) { /* this waits for a packet - idle */
				if (bytes == -1 && errno != EINTR) {
					DEBUG(LOG_INFO, "error on read, %s, reopening socket", sys_errlist[errno]);
					close(server_socket[i]);
					server_socket[i] = -1;
				}
				continue;
			}

			if ((state = get_option(&packet, DHCP_MESSAGE_TYPE)) == NULL) {
				DEBUG(LOG_ERR, "couldn't get option from packet, ignoring");
				continue;
			}
		
			/* ADDME: look for a static lease */
			lease = find_lease_by_chaddr(packet.chaddr, i);
			switch (state[0]) {
				case DHCPDISCOVER:
					DEBUG(LOG_INFO,"received DISCOVER");
					if (sendOffer(&packet, i) < 0)
						LOG(LOG_ERR, "send OFFER failed");
					break;			
 		
				case DHCPREQUEST:
					DEBUG(LOG_INFO, "received REQUEST");
					requested = get_option(&packet, DHCP_REQUESTED_IP);
					server_id = get_option(&packet, DHCP_SERVER_ID);
					if (requested) memcpy(&requested_align, requested, 4);
					if (server_id) memcpy(&server_id_align, server_id, 4);
	
					if (lease)
          { /*ADDME: or static lease */
						if (server_id)
            {
							/* SELECTING State */
							DEBUG(LOG_INFO, "server_id = %08x", ntohl(server_id_align));
							if (server_id_align == server_config[i].server && requested && 
					    		requested_align == lease->yiaddr)
								sendACK(&packet, lease->yiaddr, i);
						}
            else
            {
							if (requested)
              {
								/* INIT-REBOOT State */
								if (lease->yiaddr == requested_align)
									sendACK(&packet, lease->yiaddr, i);
								else
                  sendNAK(&packet, i);
							}
              else
              {
								/* RENEWING or REBINDING State */
								if (lease->yiaddr == packet.ciaddr)
									sendACK(&packet, lease->yiaddr, i);
								else
									/* don't know what to do!!!! */
									sendNAK(&packet, i);
							}						
						}
            /* what to do if we have no record of the client */
					}
          else if (server_id)
					  /* SELECTING State */
						sendNAK(&packet,i);
          else if (requested)
          {
					  /* INIT-REBOOT State */
					  if ((lease = find_lease_by_yiaddr(requested_align,i)))
            {
					    if (lease_expired(lease,i))
					      /* probably best if we drop this lease */
						    memset(lease->chaddr, 0, 16);
						  /* make some contention for this address */
              else
							  sendNAK(&packet,i);
			      }
            else if (requested_align < server_config[i].start ||
					                   requested_align > server_config[i].end)
					    sendNAK(&packet,i);
            else
						  /* else remain silent */
							sendNAK(&packet,i);
		      }
          else
            /* RENEWING or REBINDING State */
						sendNAK(&packet,i);
          break;
				case DHCPDECLINE:
					DEBUG(LOG_INFO,"received DECLINE");
					if (lease) {
						memset(lease->chaddr, 0, 16);
						lease->expires = time(0) + server_config[i].decline_time;
					}			
					break;
				
				case DHCPRELEASE:
					DEBUG(LOG_INFO,"received RELEASE");
					if (lease)
					{
						if( lease->expires != server_config[i].inflease_time)
						{
							lease->expires = time(0);
							write_to_delta(lease->chaddr, lease->yiaddr, lease->hostname,0,LEASE_DEL);
						}
					}
					break;
		
				case DHCPINFORM:
					DEBUG(LOG_INFO,"received INFORM");
					send_inform(&packet, i);
					break;	
				
				default:
					LOG(LOG_WARNING, "unsupported DHCP message (%02x) -- ignoring", state[0]);
			}
		}
	}
	return 0;
}



