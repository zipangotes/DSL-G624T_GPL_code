/*
 * Copyright (c) 1983, 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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

/*
 * From: @(#)output.c	5.15 (Berkeley) 2/28/91
 * From: @(#)output.c	8.1 (Berkeley) 6/5/93
 */
char output_rcsid[] = 
  "$Id: output.c,v 1.7 1999/08/01 19:19:16 dholland Exp $";


/*
 * Routing Table Management Daemon
 */

#include "defs.h"

/*
 * Apply the function "f" to all non-passive
 * interfaces.  If the interface supports the
 * use of broadcasting use it, otherwise address
 * the output to the known router.   inet_addr
 */
extern void printroute(struct rt_entry *rt);

void toall(void (*f)(struct sockaddr *, int, struct interface *, int), 
	int rtstate, struct interface *skipif)
{
	struct interface *ifp;
	struct sockaddr *dst;
	int flags;
  struct sockaddr_in *ptemp,dst_ip;
	unsigned char options;

	for (ifp = ifnet; ifp; ifp = ifp->int_next) {
		if (ifp->int_flags & IFF_PASSIVE || ifp == skipif)
			continue;

		/* Check if this interface should send responses */
		options = if_cfg_getopt(ifp);
		if( !(options & CFG_ALLOW_OUT))
			continue;
		
		dst = ifp->int_flags & IFF_BROADCAST ? &ifp->int_broadaddr :
		      ifp->int_flags & IFF_POINTOPOINT ? &ifp->int_dstaddr :
		      &ifp->int_addr;
		flags = ifp->int_flags & IFF_INTERFACE ? MSG_DONTROUTE : 0;

    /*!!! If rip v2(Only) dst is multicast address */
    if( comp_switch == RIP_V2)
		{
      setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &ifp->int_addr, sizeof(ifp->int_addr));
      dst_ip.sin_family = AF_INET;     /* host byte order */
      dst_ip.sin_port = rip_port; /* short, network byte order */
      dst_ip.sin_addr.s_addr = inet_addr(RIP_GROUP);
      dst = (struct sockaddr *)&dst_ip;
      //flags = 0;
		}
    ptemp = (struct sockaddr_in *)dst;
		(*f)(dst, flags, ifp, rtstate);
	}
}

void toone(void (*f)(struct sockaddr *, int, struct interface *, int),
	int rtstate, struct sockaddr *from)
{
  struct interface *ifp;
  struct sockaddr *dst;
  int flags;
  struct sockaddr_in dst_ip,*ptemp,*ptemp1;
  unsigned char options;

  if((ifp = if_iflookup(from)) == 0 )
    return;

  /* RFC STATES RIP SHOULKD RESPOND EVEN IF IT RUNS IN SILENT MODE 
     Check if this interface should send responses 
     options = if_cfg_getopt(ifp);
     if( !(options & CFG_ALLOW_OUT))
       return; */

  /* From this interface information decipher the destination address 
  dst = ifp->int_flags & IFF_BROADCAST ? &ifp->int_broadaddr :
                   ifp->int_flags & IFF_POINTOPOINT ? &ifp->int_dstaddr :
		      &ifp->int_addr;    */
  dst = ifp->int_flags & IFF_BROADCAST ? from :
                   ifp->int_flags & IFF_POINTOPOINT ? &ifp->int_dstaddr :
		      from;   
  flags = ifp->int_flags & IFF_INTERFACE ? MSG_DONTROUTE : 0;

  /* For RIPv2 the detination address is always RIP_GROUP but the
     socket needs to be bound to the correct interface          */
  if( comp_switch == RIP_V2)
  {
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &ifp->int_addr, sizeof(ifp->int_addr));
    dst_ip.sin_family = AF_INET;     /* host byte order */
    ptemp = (struct sockaddr_in *)from;
    //printf("RIPv2 DEST PORT = %d\n",htons(ptemp->sin_port));
    dst_ip.sin_port = ptemp->sin_port;//rip_port;      /* short, network byte order */
    dst_ip.sin_addr.s_addr = inet_addr(RIP_GROUP);
    dst = (struct sockaddr *)&dst_ip;
  }
  ptemp = (struct sockaddr_in *)from;
  ptemp1 = (struct sockaddr_in *)dst;
  ptemp1->sin_port = ptemp->sin_port;/* short, network byte order */
  /* Send the message */
  (*f)(dst, flags, ifp, rtstate);
}

/*
 * Output a preformed packet.
 */

void sndmsg(struct sockaddr *dst, int flags, struct interface *ifp, int rtstate)
{
  int size = 0;
	(void)rtstate;
  size = (pwd==1)? sizeof( struct rip ) + sizeof(struct netinfo):
                                                     sizeof( struct rip );
	(*afswitch[dst->sa_family].af_output)(sock, flags,
					      dst, size);
	TRACE_OUTPUT(ifp, dst, size);
}

/*
 * Supply dst with the contents of the routing tables.
 * If this won't fit in one packet, chop it up into several.
 */
 
void supply(struct sockaddr *dst, int flags, struct interface *ifp, int rtstate)
{
	struct rt_entry *rt;
	struct netinfo *n = msg->rip_nets;
  struct sockaddr *interface;
  struct sockaddr_in *ptr;

	struct rthash *rh;
	struct rthash *base = hosthash;
	int doinghost = 1, size;
	void (*output)(int,int,struct sockaddr *,int) = 
		afswitch[dst->sa_family].af_output;
	int (*sendroute)(struct rt_entry *, struct sockaddr *) = 
		afswitch[dst->sa_family].af_sendroute;
	int npackets = 0;

  struct sockaddr_in *temp ;
  unsigned char ifoptions=0;

 
	msg->rip_cmd = RIPCMD_RESPONSE;
  msg->rip_vers = ( comp_switch == RIP_V1) ? RIP_VERSION_1 : RIP_VERSION_2 ;

	memset(msg->rip_res1, 0, sizeof(msg->rip_res1));
  if( ( comp_switch > RIP_V1 ) && ( pwd == PWD_YES ) )
  {
		/*Fill in authentication information */
    msg->rip_nets[0].sa_family = htons(AUTH_FAMILY);
    msg->rip_nets[0].route_tag = htons(AUTH_CLRTXT); /* Actually Authentication Type */
    memset((char *)(&msg->rip_nets[0].ip_addr),0x00,MAX_PWD_LENGTH);
    strncpy((char *)(&msg->rip_nets[0].ip_addr),passwd,strlen(passwd));
    n = &msg->rip_nets[1];
  }

again:
	for (rh = base; rh < &base[ROUTEHASHSIZ]; rh++)
	for (rt = rh->rt_forw; rt != (struct rt_entry *)rh; rt = rt->rt_forw) {
		/*
		 * Don't resend the information on the network
		 * from which it was received (unless sending
		 * in response to a query).
		 */
		if (ifp && rt->rt_ifp == ifp &&
		    (rt->rt_state & RTS_INTERFACE) == 0)
			continue;
		if (rt->rt_state & RTS_EXTERNAL)
			continue;
		/*
		 * For dynamic updates, limit update to routes
		 * with the specified state.
		 */
		if (rtstate && (rt->rt_state & rtstate) == 0)
			continue;

		/* if interface is WAN type and NAT is on
		    do not send LAN type routes */
    ifoptions = if_cfg_getopt(ifp);
    if ( (ifoptions & CFG_IFF_WAN) &&
         (ifoptions & CFG_NAT_ON)  &&
         (!(rt->rt_options & RTO_WAN)) )
        continue;  

		/*
		 * Limit the spread of subnet information
		 * to those who are interested.
		 */
    /*!!! This check need not be done for RIP v2 mode */
		if (doinghost == 0 && rt->rt_state & RTS_SUBNET && comp_switch == RIP_V1) 
		{
			if (rt->rt_dst.sa_family != dst->sa_family)
				continue;
      		if( ifp == 0)
         		interface = dst;
      		else
      		{
        		/* Check if the SUBNET route should be sent on this interface */
		    	interface = ifp->int_flags & IFF_BROADCAST ? &ifp->int_broadaddr :
		            ifp->int_flags & IFF_POINTOPOINT ? &ifp->int_dstaddr :
		            &ifp->int_addr;
      		}
      		ptr = (struct sockaddr_in *)interface;
     	 	if ((*sendroute)(rt, interface) == 0)
				continue;
			/*if ((*sendroute)(rt, dst) == 0)
				continue;*/
		}

    if( (comp_switch != RIP_V1) &&
        (rt->rt_state & RTS_INTERFACE) &&
        (rt->rt_state & RTS_PASSIVE))
        continue;

		size = (char *)n - packet;
		if (size > MAXPACKETSIZE - (int)sizeof (struct netinfo)) {
			TRACE_OUTPUT(ifp, dst, size);
			(*output)(sock, flags, dst, size);
			/*
			 * If only sending to ourselves,
			 * one packet is enough to monitor interface.
			 */
			if (ifp && (ifp->int_flags &
			   (IFF_BROADCAST | IFF_POINTOPOINT | IFF_REMOTE)) == 0)
				return;
			n = msg->rip_nets;
			npackets++;
		}
    temp = (struct sockaddr_in *)(&rt->rt_dst);
    n->sa_family = rt->rt_dst.sa_family;
#if BSD < 198810
		if (sizeof(n->sa_family) > 1)	/* XXX */
			n->sa_family = htons(n->sa_family);
#endif
		n->rip_metric = htonl(rt->rt_metric);
    n->ip_addr =  temp->sin_addr.s_addr;
    if(  msg->rip_vers == RIP_VERSION_2 )
    {
      n->route_tag = htons(rt->rt_tag);
      if( rt->rt_netmask.s_addr == 0xFFFFFFFF)
        n->subnetmask = 0;
      else
        n->subnetmask = rt->rt_netmask.s_addr;
      /* If router's subnet is not same as this IF's subnet send
         if IP address as router */
      temp = (struct sockaddr_in *)(&rt->rt_router);
      n->nexthop_ip = temp->sin_addr.s_addr;
      if( (htonl(n->nexthop_ip) & ifp->int_subnetmask) != ifp->int_subnet)
		  {
        temp = (struct sockaddr_in *)(&ifp->int_addr);
        n->nexthop_ip = temp->sin_addr.s_addr;
		  }
    }
    else
    {
			/* Respond with the standard RIP v1 response packet */
      n->route_tag = 0;
      n->subnetmask = 0;
      n->nexthop_ip = 0;
    }
		n++;
	}
	if (doinghost) {
		doinghost = 0;
		base = nethash;
		goto again;
	}
	if (n != msg->rip_nets || (npackets == 0 && rtstate == 0)) {
		size = (char *)n - packet;
		TRACE_OUTPUT(ifp, dst, size);
		(*output)(sock, flags, dst, size);
	}
}
