#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>

#include "input.h"
#include "cm_message.h"
#include "cm_comm.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif /* DMALLOC */

fd_set	master_readfds;
int	max_readfd = 0;
int	input_fd;
int	input_timeout=10;

//char	msg_buffer[CM_MAX_MESSAGE_LEN];
char	*msg_buffer=NULL;

void	setup_input(int fd, int timeout)
{
    
	msg_buffer=(char*)malloc(CM_MAX_MESSAGE_LEN);

	FD_ZERO(&master_readfds);
	FD_SET(fd, &master_readfds);
	max_readfd=fd+1;
	input_fd=fd;

	/* Set the timeout */
	input_timeout=timeout;
}

struct msg_base	*get_input( void )
{
	fd_set		readfds;
	ssize_t		recvlen;
	struct msg_base	*msg;
	int		active_fds;
	struct timeval	select_tv;

	/* Copy the master fd sets into our local fd sets */
	memcpy(&readfds, &master_readfds, sizeof(fd_set));


	active_fds=select(max_readfd, &readfds, NULL, NULL, (struct timeval *)0);
	if(active_fds<0){
            syslog(LOG_WARNING,"Select failed: Error:[%d]\n",active_fds);
            return NULL;
	}

	/* Check for new message */
	if( FD_ISSET(input_fd, &readfds) )
        {
            struct sockaddr_un	from;
            socklen_t		fromlen=sizeof(struct sockaddr_un);
            
            recvlen = comm_recvfrom(input_fd, msg_buffer, CM_MAX_MESSAGE_LEN -1, MSG_DONTWAIT,
                                    (struct sockaddr *)&from, &fromlen);
            if( recvlen >0)
            {
                msg_buffer[recvlen]=0;
                msg = msg_parse(msg_buffer, recvlen);
                return msg;
            } 
            else 
            {
                /* Error */
                if(recvlen == EAGAIN)
                    syslog(LOG_WARNING, "No data after select call waiting for message\n");
                else 
                    syslog(LOG_WARNING, "unknown error in comm_recvfrom after select call\n");
                return NULL;
            }
	}
        syslog(LOG_WARNING, "No message available\n");
	return NULL;
}
