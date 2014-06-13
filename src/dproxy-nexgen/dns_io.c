/*-------------------------------------------------------------------------------------
// Copyright 2005, Texas Instruments Incorporated
//
// This program has been modified from its original operation by Texas Instruments
// to do the following:
//
// 1. Cache only if a valid address or name is received in the response. There was
//    a possibility of caching incorrect values in ip and cname variables  if the 
//    response is success, but there was no name or host address in the response
//
// THIS MODIFIED SOFTWARE AND DOCUMENTATION ARE PROVIDED
// "AS IS," AND TEXAS INSTRUMENTS MAKES NO REPRESENTATIONS
// OR WARRENTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO, WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE SOFTWARE OR
// DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS,
// COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.
// See The GNU General Public License for more details.
//
// These changes are covered under version 2 of the GNU General Public License,
// dated June 1991.
//-------------------------------------------------------------------------------------*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dns_io.h"

/*****************************************************************************/
int dns_read_packet(int sock, dns_request_t *m)
{
  struct sockaddr_in sa;
  int salen;
  
  /* Read in the actual packet */
  salen = sizeof(sa);
  
  m->numread = recvfrom(sock, m->original_buf, sizeof(m->original_buf), 0,
		     (struct sockaddr *)&sa, &salen);
  
  if ( m->numread < 0) {
    debug_perror("dns_read_packet: recvfrom\n");
    return -1;
  }
  
  /* TODO: check source addr against list of allowed hosts */

  /* record where it came from */
  memcpy( (void *)&m->src_addr, (void *)&sa.sin_addr, sizeof(struct in_addr));
  m->src_port = ntohs( sa.sin_port );

  /* check that the message is long enough */
  if( m->numread < sizeof (m->message.header) ){
    debug("dns_read_packet: packet from '%s' to short to be dns packet", 
	  inet_ntoa (sa.sin_addr) );
    return -1;
  }

  /* reset cache flag */
  m->cache=0;

  /* pass on for full decode */
  dns_decode_request( m );

  return 0;
}
/*****************************************************************************/
int dns_write_packet(int sock, struct in_addr in, int port, dns_request_t *m)
{
  struct sockaddr_in sa;
  int retval;

  /* Zero it out */
  memset((void *)&sa, 0, sizeof(sa));

  /* Fill in the information */
  //inet_aton( "203.12.160.35", &in );
  memcpy( &sa.sin_addr.s_addr, &in, sizeof(in) );
  sa.sin_port = htons(port);
  sa.sin_family = AF_INET;

  retval = sendto(sock, m->original_buf, m->numread, 0, 
		(struct sockaddr *)&sa, sizeof(sa));
  
  if( retval < 0 ){
    debug_perror("dns_write_packet: sendto");
  }

  return retval;
}
