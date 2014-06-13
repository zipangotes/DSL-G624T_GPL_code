/* serverpacket.c
 *
 * Constuct and send DHCP server packets
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#include "packet.h"
#include "debug.h"
#include "dhcpd.h"
#include "options.h"
#include "leases.h"

static int copy_till(char *inp , char *dest, char sep ,int length)
{
	int i = 0;
  while( (inp[i] != 0x00) && (inp[i] != sep) && (i < length ))
		*(dest + i) = inp[i++];
  *(dest + i) = 0x00;
  return i; 	
}

/* send a packet to giaddr using the kernel ip stack */
static int send_packet_to_relay(struct dhcpMessage *payload, int ifid)
{
	DEBUG(LOG_INFO, "Forwarding packet to relay");

	return kernel_packet(payload, server_config[ifid].server, SERVER_PORT,
			payload->giaddr, SERVER_PORT);
}


/* send a packet to a specific arp address and ip address by creating our own ip packet */
static int send_packet_to_client(struct dhcpMessage *payload, int force_broadcast, int ifid)
{
	unsigned char *chaddr;
	u_int32_t ciaddr;
	
	if (force_broadcast) {
		DEBUG(LOG_INFO, "broadcasting packet to client (NAK)");
		ciaddr = INADDR_BROADCAST;
		chaddr = MAC_BCAST_ADDR;
	} else if (payload->ciaddr) {
		DEBUG(LOG_INFO, "unicasting packet to client ciaddr");
		ciaddr = payload->ciaddr;
		chaddr = payload->chaddr;
	} else if (ntohs(payload->flags) & BROADCAST_FLAG) {
		DEBUG(LOG_INFO, "broadcasting packet to client (requested)");
		ciaddr = INADDR_BROADCAST;
		chaddr = MAC_BCAST_ADDR;
	} else {
		DEBUG(LOG_INFO, "unicasting packet to client yiaddr");
		ciaddr = payload->yiaddr;
		chaddr = payload->chaddr;
	}
	return raw_packet(payload, server_config[ifid].server, SERVER_PORT, 
			ciaddr, CLIENT_PORT, chaddr, server_config[ifid].ifindex);
}


/* send a dhcp packet, if force broadcast is set, the packet will be broadcast to the client */
static int send_packet(struct dhcpMessage *payload, int force_broadcast, int ifid)
{
	int ret;

	if (payload->giaddr)
		ret = send_packet_to_relay(payload, ifid);
	else ret = send_packet_to_client(payload, force_broadcast, ifid);
	return ret;
}


static void init_packet(struct dhcpMessage *packet, struct dhcpMessage *oldpacket, char type, int ifid)
{
	init_header(packet, type);
	packet->xid = oldpacket->xid;
	memcpy(packet->chaddr, oldpacket->chaddr, 16);
	packet->flags = oldpacket->flags;
	packet->giaddr = oldpacket->giaddr;
	packet->ciaddr = oldpacket->ciaddr;
	add_simple_option(packet->options, DHCP_SERVER_ID, server_config[ifid].server);
}


/* add in the bootp options */
static void add_bootp_options(struct dhcpMessage *packet, int ifid)
{
	packet->siaddr = server_config[ifid].siaddr;
	if (server_config[ifid].sname)
		strncpy(packet->sname, server_config[ifid].sname, sizeof(packet->sname) - 1);
	if (server_config[ifid].boot_file)
		strncpy(packet->file, server_config[ifid].boot_file, sizeof(packet->file) - 1);
}
	

/* send a DHCP OFFER to a DHCP DISCOVER */
int sendOffer(struct dhcpMessage *oldpacket, int ifid)
{
	struct dhcpMessage packet;
	struct dhcpOfferedAddr *lease = NULL;
	u_int32_t req_align, lease_time_align = server_config[ifid].lease,leasetime = 0;
	unsigned char *req, *lease_time;
	struct option_set *curr;
	struct in_addr addr;
  u_int8_t hostname[50] = "unknown" , *hname;
  int length = 0;

	init_packet(&packet, oldpacket, DHCPOFFER, ifid);
	
	/* ADDME: if static, short circuit */
	/* the client is in our lease/offered table */
	if ((lease = find_lease_by_chaddr(oldpacket->chaddr, ifid)))
  {
		if (!lease_expired(lease,ifid))
		{
			if(lease->expires == server_config[ifid].inflease_time)
				lease_time_align = server_config[ifid].inflease_time;
      else
				lease_time_align = lease->expires - time(0);
		}
		packet.yiaddr = lease->yiaddr;
		
	/* Or the client has a requested ip */
	} else if ((req = get_option(oldpacket, DHCP_REQUESTED_IP)) &&

		   /* Don't look here (ugly hackish thing to do) */
		   memcpy(&req_align, req, 4) &&

		   /* and the ip is in the lease range */
		   ntohl(req_align) >= ntohl(server_config[ifid].start) &&
		   ntohl(req_align) <= ntohl(server_config[ifid].end) &&
		   
		   /* and its not already taken/offered */ /* ADDME: check that its not a static lease */
		   ((!(lease = find_lease_by_yiaddr(req_align, ifid)) ||
		   
		   /* or its taken, but expired */ /* ADDME: or maybe in here */
		   lease_expired(lease,ifid)))) {
		   /* check id addr is not taken by a static ip */
		   if(!check_ip(req_align, ifid)) 	
				packet.yiaddr = req_align; /* FIXME: oh my, is there a host using this IP? */
		   else {
			   packet.yiaddr = find_address(0, ifid);

			   /* try for an expired lease */
			   if (!packet.yiaddr) packet.yiaddr = find_address(1, ifid);
			}

			

	/* otherwise, find a free IP */ /*ADDME: is it a static lease? */
	}
  else
  {
		packet.yiaddr = find_address(0, ifid);
		/* try for an expired lease */
		if (!packet.yiaddr) packet.yiaddr = find_address(1, ifid);
	}
	
	if(!packet.yiaddr) {
		LOG(LOG_WARNING, "no IP addresses to give -- OFFER abandoned");
		return -1;
	}

  hname = get_option(oldpacket,DHCP_HOST_NAME);
  if(hname)
  {
    add_option_string(packet.options, hname - 2);
		memset(hostname,0x00,50);
    length = get_option_length(oldpacket,DHCP_HOST_NAME);
    copy_till(hname , hostname, '.',length);
		LOG(LOG_INFO, "SENDING OFFER to %s\n",hostname);
  }	

  /*Check for infinite lease */
	if ((lease = find_lease_by_chaddr(oldpacket->chaddr, ifid)))
	{
		if(lease->expires == server_config[ifid].inflease_time)
			leasetime = server_config[ifid].inflease_time;
		else
				leasetime = server_config[ifid].offer_time;
	}
  else
		leasetime = server_config[ifid].offer_time;

	if (!add_lease(packet.chaddr, packet.yiaddr, leasetime /*server_config[ifid].offer_time*/, ifid,hostname)) {
		LOG(LOG_WARNING, "lease pool is full -- OFFER abandoned");
		return -1;
	}		

	if ((lease_time = get_option(oldpacket, DHCP_LEASE_TIME))) {
		memcpy(&lease_time_align, lease_time, 4);
		lease_time_align = ntohl(lease_time_align);
		if (lease_time_align > server_config[ifid].lease) 
			lease_time_align = server_config[ifid].lease;
	}

	/* Make sure we aren't just using the lease time from the previous offer */
	if (lease_time_align < server_config[ifid].min_lease) 
		lease_time_align = server_config[ifid].lease;

  /* For inifinite leases change the lease time */
  if( leasetime == server_config[ifid].inflease_time)
    lease_time_align = leasetime;
	/* ADDME: end of short circuit */		
	add_simple_option(packet.options, DHCP_LEASE_TIME, htonl(lease_time_align));

	curr = server_config[ifid].options;
	while (curr) {
		if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
			add_option_string(packet.options, curr->data);
		curr = curr->next;
	}

	add_bootp_options(&packet, ifid);
	
	addr.s_addr = packet.yiaddr;
	LOG(LOG_INFO, "sending OFFER of %s", inet_ntoa(addr));
	return send_packet(&packet, 0, ifid);
}


int sendNAK(struct dhcpMessage *oldpacket, int ifid)
{
	struct dhcpMessage packet;

	init_packet(&packet, oldpacket, DHCPNAK, ifid);
	
	DEBUG(LOG_INFO, "sending NAK");
	return send_packet(&packet, 1, ifid);
}


int sendACK(struct dhcpMessage *oldpacket, u_int32_t yiaddr, int ifid)
{
	struct dhcpMessage packet;
	struct option_set *curr;
	unsigned char *lease_time;
	u_int32_t lease_time_align = server_config[ifid].lease;
	struct in_addr addr;
  u_int8_t hostname[50]="" , *hname;
  int length = 0;
	struct dhcpOfferedAddr *lease = NULL;

	init_packet(&packet, oldpacket, DHCPACK, ifid);
	packet.yiaddr = yiaddr;
	
	if ((lease_time = get_option(oldpacket, DHCP_LEASE_TIME))) {
		memcpy(&lease_time_align, lease_time, 4);
		lease_time_align = ntohl(lease_time_align);
		if (lease_time_align > server_config[ifid].lease) 
			lease_time_align = server_config[ifid].lease;
		else if (lease_time_align < server_config[ifid].min_lease) 
			lease_time_align = server_config[ifid].lease;
	}
	
  /* If the existing lease entry has infinite entry give it infinite time */
	if ( (lease = find_lease_by_chaddr(oldpacket->chaddr, ifid)) )
	{
		if(lease->expires == server_config[ifid].inflease_time)
			lease_time_align = server_config[ifid].inflease_time;
	}

	add_simple_option(packet.options, DHCP_LEASE_TIME, htonl(lease_time_align));

  hname = get_option(oldpacket,DHCP_HOST_NAME);
  if(hname)
  {
    add_option_string(packet.options, hname - 2 );
    memset(hostname,0x00,50);
    length = get_option_length(oldpacket,DHCP_HOST_NAME);
    copy_till(hname , hostname, '.',length);
		LOG(LOG_INFO, "SENDING ACK to %s\n",hostname);
  }	
	
	curr = server_config[ifid].options;
	while (curr) {
		if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
			add_option_string(packet.options, curr->data);
		curr = curr->next;
	}

	add_bootp_options(&packet, ifid);

	addr.s_addr = packet.yiaddr;
	LOG(LOG_INFO, "sending ACK to %s", inet_ntoa(addr));

	if (send_packet(&packet, 0, ifid) < 0) 
		return -1;

	add_lease(packet.chaddr, packet.yiaddr, lease_time_align, ifid,hostname);

	return 0;
}


int send_inform(struct dhcpMessage *oldpacket, int ifid)
{
	struct dhcpMessage packet;
	struct option_set *curr;

	init_packet(&packet, oldpacket, DHCPACK, ifid);
	
	curr = server_config[ifid].options;
	while (curr) {
		if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
			add_option_string(packet.options, curr->data);
		curr = curr->next;
	}

	add_bootp_options(&packet, ifid);

	return send_packet(&packet, 0, ifid);
}



