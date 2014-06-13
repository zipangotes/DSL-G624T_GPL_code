/*  Copyright (C) 1996, 2000 N.M. Maclaren
    Copyright (C) 1996, 2000 The University of Cambridge

This includes all of the code needed to handle Berkeley sockets.  It is way
outside current POSIX, unfortunately.  It should be easy to convert to a system
that uses another mechanism.  It does not currently use socklen_t, because
the only system that the author uses that has it is Linux. */



#include "header.h"
#include "internet.h"
#include <fcntl.h>

#define SOCKET
#include "kludges.h"
#undef SOCKET


/* The code needs to set some variables during the open, for use by later
functions. */

static int initial = 1, descriptors[MAX_SOCKETS];
static struct sockaddr_in here[MAX_SOCKETS], there[MAX_SOCKETS];


/* There needs to be some disgusting grobble for handling timeouts, that is
identical to the grobble in internet.c. */


void display_in_hex (const void *data, int length) {
    int i;

    for (i = 0; i < length; ++i)
        fprintf(stderr,"%.2x",((const unsigned char *)data)[i]);
}



int open_socket (int which, char *hostname, int timespan) {

/* Locate the specified NTP server, set up a couple of addresses and open a
socket. */

    int port, k;
    struct in_addr address, anywhere, everywhere;
    int ret;

/* Initialise and find out the server and port number.  Note that the port
number is in network format. */

    if (initial) for (k = 0; k < MAX_SOCKETS; ++k) descriptors[k] = -1;
    initial = 0;

    ret = find_address(&address,&anywhere,&everywhere,&port,hostname,timespan);
    if (ret < 1)
    {
        if (curr_serv < 2)
        {
            curr_serv++;
            curr_attempt=0;
            return -1;
        }
        else
           return (0);
    }

/* Set up our own and the target addresses.  Note that the target address will
be reset before use in server mode. */

    memset(&here[which],0,sizeof(struct sockaddr_in));
    here[which].sin_family = AF_INET;
    here[which].sin_port =
        (operation == op_listen || operation == op_server ? port : 0);
    here[which].sin_addr = anywhere;
    memset(&there[which],0,sizeof(struct sockaddr_in));
    there[which].sin_family = AF_INET;
    there[which].sin_port = port;
    there[which].sin_addr = (operation == op_broadcast ? everywhere : address);

/* Allocate a local UDP socket and configure it. */

    errno = 0;
    if ((descriptors[which] = socket(AF_INET,SOCK_DGRAM,0)) < 0 ||
            bind(descriptors[which],(struct sockaddr *)&here[which],
                    sizeof(here[which]))  < 0)
        fatal(1,"unable to allocate socket for NTP",NULL);
    if (operation == op_broadcast) {
        errno = 0;
        k = setsockopt(descriptors[which],SOL_SOCKET,SO_BROADCAST,
                (void *)&k,sizeof(k));
        if (k != 0) fatal(1,"unable to set permission to broadcast",NULL);
    }
    return 1;
}



extern void write_socket (int which, void *packet, int length) {

/* Any errors in doing this are fatal - including blocking.  Yes, this leaves a
server vulnerable to a denial of service attack. */

    int k;

    if (which < 0 || which >= MAX_SOCKETS || descriptors[which] < 0)
        fatal(0,"socket index out of range or not open",NULL);
    errno = 0;
    k = sendto(descriptors[which],packet,(size_t)length,0,
            (struct sockaddr *)&there[which],sizeof(there[which]));
    if (k != length) fatal(1,"unable to send NTP packet",NULL);
}


extern int read_socket (int which, void *packet, int length, int waiting) {

/* Read a packet and return its length or -1 for failure.  Only incorrect
length and timeout are not fatal. */

    struct sockaddr_in scratch, *ptr;
    int n,k;
    int temp;
    fd_set  fds;
    int     active_fds;
    struct  timeval to;

/* Under normal circumstances, set up a timeout. */

    if (which < 0 || which >= MAX_SOCKETS || descriptors[which] < 0)
        fatal(0,"socket index out of range or not open",NULL);

    to.tv_sec = timeout;
    to.tv_usec = 0;

    FD_ZERO(&fds);
    FD_SET(descriptors[which],&fds);
                                                                                                                                                                                    
    active_fds = select(descriptors[which]+1, &fds, NULL, NULL, &to);
    if(active_fds < 0)
    {
        printf("fds < 0.\n");
    }                                                                                                                                                                               
    if( (to.tv_sec == 0) && (to.tv_usec == 0))
    {
        if (curr_attempt < 2)
	{
	    curr_attempt++;
	}
	else if (curr_attempt == 2)
	{
	    if (curr_serv < 2)
	    {
	        curr_serv++;
		curr_attempt=0;
	    }
	    else
	    {
		curr_serv = 0;
		curr_attempt = 0;
	    }
	}
	return -1;
    }


/* Get the packet and clear the timeout, if any.  */

    memcpy(ptr = &scratch,&there[which],sizeof(struct sockaddr_in));
    n = sizeof(struct sockaddr_in);
    errno = 0;
    k = recvfrom(descriptors[which],packet,(size_t)length,0, (struct sockaddr *)ptr,&n);

/* Now issue some low-level diagnostics. */

    if (k <= 0) fatal(1,"unable to receive NTP packet from server",NULL);
    return k;
}


extern int flush_socket (int which) {

/* Get rid of any outstanding input, because it may have been hanging around
for a while.  Ignore packet length oddities and return the number of packets
skipped. */

    struct sockaddr_in scratch;
    int n;
    char buffer[256];
    int flags, count = 0, total = 0, k;

/* The code is the obvious. */

    if (which < 0 || which >= MAX_SOCKETS || descriptors[which] < 0)
        fatal(0,"socket index out of range or not open",NULL);
    errno = 0;
    if ((flags = fcntl(descriptors[which],F_GETFL,0)) < 0 ||
            fcntl(descriptors[which],F_SETFL,flags|O_NONBLOCK) == -1)
        fatal(1,"unable to set non-blocking mode",NULL);
    while (1) {
        n = sizeof(struct sockaddr_in);
        errno = 0;
        k = recvfrom(descriptors[which],buffer,256,0,
            (struct sockaddr *)&scratch,&n);
        if (k < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            fatal(1,"unable to flush socket",NULL);
        }
        ++count;
        total += k;
    }
    errno = 0;
    if (fcntl(descriptors[which],F_SETFL,flags) == -1)
        fatal(1,"unable to restore blocking mode",NULL);
    return count;
}



extern void close_socket (int which) {

/* There is little point in shielding this with a timeout, because any hangs
are unlikely to be interruptible.  It can get called when the sockets haven't
been opened, so ignore that case. */


    if (which < 0 || which >= MAX_SOCKETS)
        fatal(0,"socket index out of range",NULL);
    if (descriptors[which] < 0) return;
    errno = 0;
    if (close(descriptors[which])) fatal(1,"unable to close NTP socket",NULL);
}
