/* utelnetd.c
 *
 * Simple telnet server
 *
 * Bjorn Wesen, Axis Communications AB 
 * <bjornw@axis.com>
 * 
 * Joerg Schmitz-Linneweber, Aston GmbH
 * <schmitz-linneweber@aston-technologie.de>
 *
 * Vladimir Oleynik
 * <dzo@simtreas.ru>
 *
 * Robert Schwebel, Pengutronix
 * <r.schwebel@pengutronix.de>
 * 
 *   10/11/2004    M.Karicheri     Filter out LF from CRLF sequence to work utelnetd
 *                                 with clients that sends out CRLF when <Enter> key 
 *                                 is hit
 *   11/16/2004    M.Karicheri     Merged changes related to cleanup session from  
 *                                 0.1.9. Leaving other changes to be picked up later as 
 *                                 needed.CHANGELOG and Makefiles are merged along with 
 *
 * This file is distributed under the GNU General Public License (GPL),
 * please see the file LICENSE for further information.
 * 
 * ---------------------------------------------------------------------------
 * (C) 2000, 2001, 2002 by the authors mentioned above
 * ---------------------------------------------------------------------------
 *
 * The telnetd manpage says it all:
 *
 *   Telnetd operates by allocating a pseudo-terminal device (see pty(4))  for
 *   a client, then creating a login process which has the slave side of the
 *   pseudo-terminal as stdin, stdout, and stderr. Telnetd manipulates the
 *   master side of the pseudo-terminal, implementing the telnet protocol and
 *   passing characters between the remote client and the login process.
 */

#include <sys/wait.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>

#define __USE_XOPEN
#define __USE_GNU
#include <stdlib.h>

#ifdef DEBUG
#define TELCMDS
#define TELOPTS
#endif
#include <arpa/telnet.h>

#define BUFSIZE 4000

#define MIN(a,b) ((a) > (b) ? (b) : (a))

static char *loginpath = NULL;

/* shell name and arguments */

static char *argv_init[] = {NULL, NULL};

/* structure that describes a session */

struct tsession {
	int sockfd, ptyfd;
	int shell_pid;
	/* two circular buffers */
	char *buf1, *buf2;
	int rdidx1, wridx1, size1;
	int rdidx2, wridx2, size2;
};

#ifdef DEBUG
#define DEBUG_OUT(...) fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG_OUT(...)
//static inline void DEBUG_OUT(const char *format, ...) {};
#endif

/*

   This is how the buffers are used. The arrows indicate the movement
   of data.

   +-------+     wridx1++     +------+     rdidx1++     +----------+
   |       | <--------------  | buf1 | <--------------  |          |
   |       |     size1--      +------+     size1++      |          |
   |  pty  |                                            |  socket  |
   |       |     rdidx2++     +------+     wridx2++     |          |
   |       |  --------------> | buf2 |  --------------> |          |
   +-------+     size2++      +------+     size2--      +----------+

   Each session has got two buffers.

*/

static int maxfd = 0;

static struct tsession *session;

/* 
 * This code was ported from a version which was made for busybox. 
 * So we have to define some helper functions which are originally 
 * available in busybox...
 */  

void show_usage(void)
{
	printf("Usage: telnetd [-l loginprogram]\n");
	printf("\n");
	printf("   -l loginprogram  program started by the server\n");
	printf("\n");         
	exit(1);
}

void perror_msg_and_die(char *text)
{
	fprintf(stderr,text);
	exit(1);
}

void error_msg_and_die(char *text, char *foo)
{
	perror_msg_and_die(text);
}


/* 
   Remove all IAC's from the buffer pointed to by bf (recieved IACs are ignored
   and must be removed so as to not be interpreted by the terminal).  Make an
   uninterrupted string of characters fit for the terminal.  Do this by packing
   all characters meant for the terminal sequentially towards the end of bf. 

   Return a pointer to the beginning of the characters meant for the terminal.
   and make *processed equal to the number of characters that were actually
   processed and *num_totty the number of characters that should be sent to
   the terminal.  
   
   Note - If an IAC (3 byte quantity) starts before (bf + len) but extends
   past (bf + len) then that IAC will be left unprocessed and *processed will be
   less than len.
  
   FIXME - if we mean to send 0xFF to the terminal then it will be escaped,
   what is the escape character?  We aren't handling that situation here.

  */
static char *
remove_iacs(unsigned char *bf, int len, int *processed, int *num_totty) {
	unsigned char *ptr = bf;
	unsigned char *totty = bf;
	unsigned char *end = bf + len;
   
	while (ptr < end) {
		if (*ptr != IAC) {
	    *totty++ = *ptr++;
		}
		else {
	    if ((ptr+2) < end) {
				/* the entire IAC is contained in the buffer 
					 we were asked to process. */
				DEBUG_OUT("Ignoring IAC 0x%02x, %s, %s\n", *ptr, TELCMD(*(ptr+1)), TELOPT(*(ptr+2)));
				ptr += 3;
	    } else {
				/* only the beginning of the IAC is in the 
					 buffer we were asked to process, we can't
					 process this char. */
				break;
	    }
		}
	}

	*processed = ptr - bf;
	*num_totty = totty - bf;
	/* move the chars meant for the terminal towards the end of the 
		 buffer. */
	return memmove(ptr - *num_totty, bf, *num_totty);
}

/* This function will remove LF that follows a CR. Some telnet clients sends 
   CR followed by LF whenever user types an Enter key. Example windows 2000
   client
*/
static int subst_crlf_cr(unsigned char *bf, int len)
{
	unsigned char *ptr = bf;
	unsigned char *end = bf + len;
	int cr_found = 0;
	int lf_removed = 0;

	while (ptr < end) 
    {
		/* check if we have \r, then if it is followed by \n remove \n and write to terminal
           buffer
         */
		if (*ptr == '\r')
		{
			cr_found = 1;
		}
		else 
		{
			if ((*ptr == '\n') && (cr_found))
			{
				memmove(ptr,ptr+1, end-(ptr+1));
				lf_removed++;
				end--;
				ptr--;
			}
			cr_found = 0;
		}
		ptr++;
	}
	return lf_removed;
}
			
static int getpty(char *line)
{
	int p;

	p = getpt();
	if (p < 0) {
		DEBUG_OUT("getpty(): couldn't get pty\n");
		close(p);
		return -1;
	}
	if (grantpt(p)<0 || unlockpt(p)<0) {
		DEBUG_OUT("getpty(): couldn't grant and unlock pty\n");
		close(p);
		return -1;
	}
	DEBUG_OUT("getpty(): got pty %s\n",ptsname(p));
	strcpy(line, (const char*)ptsname(p));

	return(p);
}


static void
send_iac(struct tsession *ts, unsigned char command, int option)
{
	/* We rely on that there is space in the buffer for now.  */
	char *b = ts->buf2 + ts->rdidx2;
	*b++ = IAC;
	*b++ = command;
	*b++ = option;
	ts->rdidx2 += 3;
	ts->size2 += 3;
}


static struct tsession *
make_new_session(int sockfd)
{
	struct termios termbuf;
	int pty, pid;
	static char tty_name[32];
	struct tsession *ts = (struct tsession *)malloc(sizeof(struct tsession));
	int t1, t2;

	ts->buf1 = (char *)malloc(BUFSIZE);
	ts->buf2 = (char *)malloc(BUFSIZE);

	ts->sockfd = sockfd;

	ts->rdidx1 = ts->wridx1 = ts->size1 = 0;
	ts->rdidx2 = ts->wridx2 = ts->size2 = 0;

	/* Got a new connection, set up a tty and spawn a shell.  */

	pty = getpty(tty_name);

	if (pty < 0) {
		fprintf(stderr, "All network ports in use!\n");
		return 0;
	}

	if (pty > maxfd)
		maxfd = pty;

	ts->ptyfd = pty;

	/* Make the telnet client understand we will echo characters so it 
	 * should not do it locally. We don't tell the client to run linemode,
	 * because we want to handle line editing and tab completion and other
	 * stuff that requires char-by-char support.
	 */

	send_iac(ts, DO, TELOPT_ECHO);
	send_iac(ts, DO, TELOPT_LFLOW);
	send_iac(ts, WILL, TELOPT_ECHO);
	send_iac(ts, WILL, TELOPT_SGA);


	if ((pid = fork()) < 0) {
		perror("fork");
	}
	if (pid == 0) {
		/* In child, open the child's side of the tty.  */
		int i, t;

		for(i = 0; i <= maxfd; i++)
			close(i);
		/* make new process group */
		if (setsid() < 0)
			perror_msg_and_die("setsid");

		//t = open(tty_name, O_RDWR | O_NOCTTY);
		t = open(tty_name, O_RDWR);
		if (t < 0)
			perror_msg_and_die("Could not open tty");

		t1 = dup(0);
		t2 = dup(1);

		tcsetpgrp(0, getpid());
		if (ioctl(t, TIOCSCTTY, NULL)) {
			perror_msg_and_die("could not set controlling tty");
		} 

  
		/* The pseudo-terminal allocated to the client is configured to operate in
		 * cooked mode, and with XTABS CRMOD enabled (see tty(4)).
		 */

		tcgetattr(t, &termbuf);
		termbuf.c_lflag |= ECHO; /* if we use readline we dont want this */
		termbuf.c_oflag |= ONLCR|XTABS;
		termbuf.c_iflag |= ICRNL;
		termbuf.c_iflag &= ~IXOFF;
		/* termbuf.c_lflag &= ~ICANON; */
		tcsetattr(t, TCSANOW, &termbuf);

		DEBUG_OUT("stdin, stdout, stderr: %d %d %d\n", t, t1, t2);

		/* exec shell, with correct argv and env */
		execv(loginpath, argv_init);
		
		/* NOT REACHED */
		perror_msg_and_die("execv");
	}

	ts->shell_pid = pid;

	return ts;
}

static void
free_session(struct tsession *ts)
{
	free(ts->buf1);
	free(ts->buf2);

	kill(ts->shell_pid, SIGKILL);

	wait4(ts->shell_pid, NULL, 0, NULL);

	close(ts->ptyfd);
	close(ts->sockfd);

	if(ts->ptyfd == maxfd || ts->sockfd == maxfd)
		maxfd--;
	if(ts->ptyfd == maxfd || ts->sockfd == maxfd)
		maxfd--;

	free(ts);
}

int main(int argc, char **argv)
{
	fd_set rdfdset, wrfdset;
	int selret;
	int c, ii;

	/* check if user supplied a port number */

	for (;;) {
		c = getopt( argc, argv, "l:");
		if (c == EOF) break;
		switch (c) {
			case 'l':
				loginpath = strdup (optarg);
				break;
		}
	}

	if (!loginpath) {
	  loginpath = "/bin/login";
	  if (access(loginpath, X_OK) < 0)
	    loginpath = "/bin/sh";
	}
	  
	if (access(loginpath, X_OK) < 0) {
		error_msg_and_die ("'%s' unavailable.", loginpath);
	}

	argv_init[0] = loginpath;

	session = make_new_session(0);
	if(!session)
		exit(1);

	do {
		int maxlen, w, r;
		struct timeval timeout;

		FD_ZERO(&rdfdset);
		FD_ZERO(&wrfdset);

		timeout.tv_sec = 600; // 10 min
		timeout.tv_usec = 0;

		/* buf1 is used from socket to pty
		 * buf2 is used from pty to socket
		 */
		if (session->size1 > 0) {
			FD_SET(session->ptyfd, &wrfdset);  /* can write to pty */
		}
		if (session->size1 < BUFSIZE) {
			FD_SET(session->sockfd, &rdfdset); /* can read from socket */
		}
		if (session->size2 > 0) {
			FD_SET(session->sockfd, &wrfdset); /* can write to socket */
		}
		if (session->size2 < BUFSIZE) {
			FD_SET(session->ptyfd, &rdfdset);  /* can read from pty */
		}

		selret = select(maxfd + 1, &rdfdset, &wrfdset, 0, &timeout);

		if (!selret)
		{
			// timeout occurred. Let us do some clean up and exit
            free_session(session);
			exit(0);
        }

		/* Then check for data tunneling.  */
		if (session->size1 && FD_ISSET(session->ptyfd, &wrfdset)) {
			int processed, num_totty, num_lfs;
			char *ptr;
			/* Write to pty from buffer 1.  */
				
			maxlen = MIN(BUFSIZE - session->wridx1,
									 session->size1);
			ptr = remove_iacs(session->buf1 + session->wridx1, maxlen, 
												&processed, &num_totty);
		
			/* the difference between processed and num_totty
				 is all the iacs we removed from the stream.
				 Adjust buf1 accordingly. */
			session->wridx1 += processed - num_totty;
			session->size1 -= processed - num_totty;

			/* Do some raw data handling for converting CRLF to CRNL. This is needed
 			since some of the telnet clients sends CRLF to indicate end of line */
			num_lfs=subst_crlf_cr(session->buf1 + session->wridx1, maxlen);
			session->size1-=num_lfs;
			num_totty-=num_lfs;
			
			w = write(session->ptyfd, ptr, num_totty);
			if (w < 0) {
				perror("write");
				free_session(session);
				exit(1);
			}
			session->wridx1 += w;
			session->size1 -= w;
			if (session->wridx1 == BUFSIZE)
				session->wridx1 = 0;
		}

		if (session->size2 && FD_ISSET(session->sockfd, &wrfdset)) {
			/* Write to socket from buffer 2.  */
			maxlen = MIN(BUFSIZE - session->wridx2,
									 session->size2);
			w = write(session->sockfd, session->buf2 + session->wridx2, maxlen);
			if (w < 0) {
				perror("write");
				free_session(session);
				exit(1);
			}
			session->wridx2 += w;
			session->size2 -= w;
			if (session->wridx2 == BUFSIZE)
				session->wridx2 = 0;
		}

		if (session->size1 < BUFSIZE && FD_ISSET(session->sockfd, &rdfdset)) {
			/* Read from socket to buffer 1. */
			maxlen = MIN(BUFSIZE - session->rdidx1,
									 BUFSIZE - session->size1);
			r = read(session->sockfd, session->buf1 + session->rdidx1, maxlen);
			if (!r || (r < 0 && errno != EINTR)) {
				free_session(session);
				exit(1);
			}
			if(!*(session->buf1 + session->rdidx1 + r - 1)) {
				r--;
				if(!r)
					continue;
			}
			session->rdidx1 += r;
			session->size1 += r;
			if (session->rdidx1 == BUFSIZE)
				session->rdidx1 = 0;
		}

		if (session->size2 < BUFSIZE && FD_ISSET(session->ptyfd, &rdfdset)) {
			/* Read from pty to buffer 2.  */
			maxlen = MIN(BUFSIZE - session->rdidx2,
									 BUFSIZE - session->size2);
			r = read(session->ptyfd, session->buf2 + session->rdidx2, maxlen);
			if (!r || (r < 0 && errno != EINTR)) {
				free_session(session);
				exit(1);
			}
			for (ii=0; ii < r; ii++)
				if (*(session->buf2 + session->rdidx2 + ii) == 3)
					fprintf(stderr, "found <CTRL>-<C> in data!\n");
			session->rdidx2 += r;
			session->size2 += r;
			if (session->rdidx2 == BUFSIZE)
				session->rdidx2 = 0;
		}

		if (session->size1 == 0) {
			session->rdidx1 = 0;
			session->wridx1 = 0;
		}
		if (session->size2 == 0) {
			session->rdidx2 = 0;
			session->wridx2 = 0;
		}

	} while (1);

	return 0;
}
