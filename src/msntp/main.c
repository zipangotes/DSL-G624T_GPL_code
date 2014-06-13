/*  Copyright (C) 1996, 1997, 2000 N.M. Maclaren
    Copyright (C) 1996, 1997, 2000 The University of Cambridge

This is a complete SNTP implementation, which was easier to write than to port
xntp to a new version of Unix with any hope of maintaining it thereafter.  It
supports the full SNTP (RFC 2030) client- and server-side challenge-response
and broadcast protocols.  It should achieve nearly optimal accuracy with very
few transactions, provided only that a client has access to a trusted server
and that communications are not INVARIABLY slow.  As this is the environment in
which 90-99% of all NTP systems are run ....

The specification of this program is:

    msntp [ --help | -h | -? ] 
            [ { -r } [ -P prompt ] [ -l lockfile ] ]
            [ -c count ] [ -d delay ] [ address(es) ] ]

    --help, -h and -? all print the syntax of the command.

     The default is that it should behave as a client, and the following options
are then relevant:

    -r indicates that the system clock should be reset by 'settimeofday'.
Naturally, this will work only if the user has enough privilege.

    The default is to write the current date and time to the standard output in
a format like '1996 Oct 15 20:17:25.123 + 4.567 +/- 0.089 secs', indicating the
estimated true (local) time and the error in the local clock.  In daemon mode,
it will add drift information in a format like ' + 1.3 +/- 0.1 ppm', and
display this at roughly 'separation' intervals.

    'prompt' is the maximum clock change that will be made automatically.
Acceptable values are from 1 to 3600, and the default is 30.  If the program is
being run interactively, larger values will cause a prompt.  The value may also
be 'no', and the change will be made without prompting.

    'count' is the maximum number of NTP packets to require.  Acceptable values
are from 1 to 25 if 'address' is specified and '-x' is not, and from 5 to 25
otherwise; the default is 5.  If the maximum isn't enough, you need a better
consistency algorithm than this program uses.  Don't increase it.

    'delay' is a rough limit on the total running time in seconds.  Acceptable
values are from 1 to 3600, and the default is 15 if 'address' is specified and
300 otherwise.

    'address' is the DNS name or IP number of a host to poll; if no name is
given, the program waits for broadcasts.  Note that a single component numeric
address is not allowed.

For sanity, it is also required that 'minerr' < 'maxerr' < 'delay' (if
listening for broadcasts, 'delay/count' and, in daemon mode, 'separation') and,
for sordid Unixish reasons, that 2*'count' < 'delay'.  The last could be fixed,
but isn't worth it.  Note that none of the above values are closely linked to
the limits described in the NTP protocol (RFC 1305).  Do not increase the
compiled-in bounds excessively, or the code will fail.

It assumes that floating-point arithmetic is tolerably efficient, which is true
for even the cheapest personal computer nowadays.  If, however, you want to
port this to a toaster, you may have problems!

In its terminating modes, its return code is EXIT_SUCCESS if the operation was
completed successfully and EXIT_FAILURE otherwise.

WARNING: this program has reached its 'hack count' and needs restructuring,
badly.  Perhaps the worst code is in run_daemon().  You are advised not to
fiddle unless you really have to. */


#include "header.h"
#include "internet.h"

#include <limits.h>
#include <float.h>
#include <math.h>
#include <signal.h>

#define MAIN
#include "kludges.h"
#undef MAIN


/* NTP definitions.  Note that these assume 8-bit bytes - sigh.  There is
little point in parameterising everything, as it is neither feasible nor
useful.  It would be very useful if more fields could be defined as
unspecified.  The NTP packet-handling routines contain a lot of extra
assumptions. */

#define JAN_1970   2208988800.0        /* 1970 - 1900 in seconds */
#define NTP_SCALE  4294967296.0        /* 2^32, of course! */

#define NTP_PACKET_MIN       48        /* Without authentication */
#define NTP_PACKET_MAX       68        /* With authentication (ignored) */
#define NTP_DISP_FIELD        8        /* Offset of dispersion field */
#define NTP_REFERENCE        16        /* Offset of reference timestamp */
#define NTP_ORIGINATE        24        /* Offset of originate timestamp */
#define NTP_RECEIVE          32        /* Offset of receive timestamp */
#define NTP_TRANSMIT         40        /* Offset of transmit timestamp */

#define NTP_LI_FUDGE          0        /* The current 'status' */
#define NTP_VERSION           3        /* The current version */
#define NTP_VERSION_MAX       4        /* The maximum valid version */
#define NTP_STRATUM          15        /* The current stratum as a server */
#define NTP_STRATUM_MAX      15        /* The maximum valid stratum */
#define NTP_POLLING           8        /* The current 'polling interval' */
#define NTP_PRECISION         0        /* The current 'precision' - 1 sec. */

#define NTP_ACTIVE            1        /* NTP symmetric active request */
#define NTP_PASSIVE           2        /* NTP symmetric passive response */
#define NTP_CLIENT            3        /* NTP client request */
#define NTP_SERVER            4        /* NTP server response */
#define NTP_BROADCAST         5        /* NTP server broadcast */

#define NTP_INSANITY     3600.0        /* Errors beyond this are hopeless */
#define RESET_MIN            15        /* Minimum period between resets */
#define ABSCISSA            3.0        /* Scale factor for standard errors */


/* Local definitions and global variables (mostly options).  These are all of
the quantities that control the main actions of the program.  The first three 
are the only ones that are exported to other modules. */

const char *argv0 = NULL;              /* For diagnostics only - not NULL */
int operation = 0;                     /* Defined in header.h - see action */
const char *lockname = NULL;           /* The name of the lock file */

#define COUNT_MAX          25          /* Do NOT increase this! */

#define action_display      1          /* Just display the result */
#define action_reset        2          /* Reset using 'settimeofday' */

static const char version[] = VERSION; /* For reverse engineering :-) */
static int action = 0,                 /* Defined above - see operation */
    period = 0,                        /* -B value in seconds (broadcast) */
    count = 0,                         /* -c value in seconds */
    delay = 0,                         /* -d or -x value in seconds */
    attempts = 0,                      /* Packets transmitted up to 2*count */
    waiting = 0,                       /* -d/-c except for in daemon mode */
    locked = 0;                        /* set_lock(1) has been called */
static double outgoing[2*COUNT_MAX],   /* Transmission timestamps */
    minerr = 0.0,                      /* -e value in seconds */
    maxerr = 0.0,                      /* -E value in seconds */
    prompt = 0.0,                      /* -p value in seconds */
    dispersion = 0.0;                  /* The source dispersion in seconds */


/* The unpacked NTP data structure, with all the fields even remotely relevant
to SNTP. */

typedef struct NTP_DATA {
    unsigned char status, version, mode, stratum, polling, precision;
    double dispersion, reference, originate, receive, transmit, current;
} ntp_data;


int timeout, retry, poll_interval;
int curr_serv;
int curr_attempt = 0;
char *sntp_serv[3] = {NULL,NULL,NULL};



void fatal (int syserr, const char *message, const char *insert) {

    /* Issue a diagnostic and stop.  Be a little paranoid about recursion. */

    int k = errno;
    static int called = 0;

    if (message != NULL) {
        fprintf(stderr,"%s: ",argv0);
        fprintf(stderr,message,insert);
        fprintf(stderr,"\n");
    }
    errno = k;
    if (syserr) perror(argv0);
    if (! called) {
        called = 1;
        if (locked) set_lock(0);
    }
 
return;
}


void syntax (int halt) {

    /* The standard, unfriendly Unix error message.  Some errors are diagnosed more
    helpfully.  This is called before any files or sockets are opened. */

    fprintf(stderr,"Syntax: %s [ --help | -h | -? ] ] \n",argv0);
    fprintf(stderr,"        [ { -r retry_count } [ -t timeout ] [ -p poll_interval ] ]\n");
    fprintf(stderr,"        [ -s address(es) ] ]\n");
    if (halt) exit(EXIT_FAILURE);
}


void pack_ntp (unsigned char *packet, int length, ntp_data *data) {

    /* Pack the essential data into an NTP packet, bypassing struct layout and
    endian problems.  Note that it ignores fields irrelevant to SNTP. */

    int i, k;
    double d;

    memset(packet,0,(size_t)length);
    packet[0] = (data->status<<6)|(data->version<<3)|data->mode;
    packet[1] = data->stratum;
    packet[2] = data->polling;
    packet[3] = data->precision;
    d = data->originate/NTP_SCALE;
    for (i = 0; i < 8; ++i) {
        if ((k = (int)(d *= 256.0)) >= 256) k = 255;
        packet[NTP_ORIGINATE+i] = k;
        d -= k;
    }
    d = data->receive/NTP_SCALE;
    for (i = 0; i < 8; ++i) {
        if ((k = (int)(d *= 256.0)) >= 256) k = 255;
        packet[NTP_RECEIVE+i] = k;
        d -= k;
    }
    d = data->transmit/NTP_SCALE;
    for (i = 0; i < 8; ++i) {
        if ((k = (int)(d *= 256.0)) >= 256) k = 255;
        packet[NTP_TRANSMIT+i] = k;
        d -= k;
    }
}


void unpack_ntp (ntp_data *data, unsigned char *packet, int length) {

    /* Unpack the essential data from an NTP packet, bypassing struct layout and
    endian problems.  Note that it ignores fields irrelevant to SNTP. */

    int i;
    double d;

    data->current = current_time(JAN_1970);    /* Best to come first */
    data->status = (packet[0] >> 6);
    data->version = (packet[0] >> 3)&0x07;
    data->mode = packet[0]&0x07;
    data->stratum = packet[1];
    data->polling = packet[2];
    data->precision = packet[3];
    d = 0.0;
    for (i = 0; i < 4; ++i) d = 256.0*d+packet[NTP_DISP_FIELD+i];
    data->dispersion = d/65536.0;
    d = 0.0;
    for (i = 0; i < 8; ++i) d = 256.0*d+packet[NTP_REFERENCE+i];
    data->reference = d/NTP_SCALE;
    d = 0.0;
    for (i = 0; i < 8; ++i) d = 256.0*d+packet[NTP_ORIGINATE+i];
    data->originate = d/NTP_SCALE;
    d = 0.0;
    for (i = 0; i < 8; ++i) d = 256.0*d+packet[NTP_RECEIVE+i];
    data->receive = d/NTP_SCALE;
    d = 0.0;
    for (i = 0; i < 8; ++i) d = 256.0*d+packet[NTP_TRANSMIT+i];
    data->transmit = d/NTP_SCALE;
}


void make_packet (ntp_data *data, int mode) {

    /* Create an outgoing NTP packet, either from scratch or starting from a
    request from a client.  Note that it implements the NTP specification, even
    when this is clearly misguided, except possibly for the setting of LI.  It
    would be easy enough to add a sanity flag, but I am not in the business of
    designing an alternative protocol (however much better it might be). */

    data->status = NTP_LI_FUDGE<<6;
    data->stratum = NTP_STRATUM;
    data->reference = data->dispersion = 0.0;
    if (mode == NTP_SERVER) {
        data->mode = (data->mode == NTP_CLIENT ? NTP_SERVER : NTP_PASSIVE);
        data->originate = data->transmit;
        data->receive = data->current;
    } else {
        data->version = NTP_VERSION;
        data->mode = mode;
        data->polling = NTP_POLLING;
        data->precision = NTP_PRECISION;
        data->receive = data->originate = 0.0;
    }
    data->current = data->transmit = current_time(JAN_1970);
}


int read_packet (int which, ntp_data *data, double *off, double *err) {

    /* Check the packet and work out the offset and optionally the error.  Note
    that this contains more checking than xntp does.  This returns 0 for success, 1
    for failure and 2 for an ignored broadcast packet (a kludge for servers).  Note
    that it must not change its arguments if it fails. */

    unsigned char receive[NTP_PACKET_MAX+1];
    double delay1, delay2, x, y;
    int response = 0, failed, length, i, k;

    /* Read the packet and deal with diagnostics. */

    length = read_socket(which,receive,NTP_PACKET_MAX+1,waiting);
 
    if (length == -1)
      return -1;
    if (length <= 0)
        return 1;
    if (length < NTP_PACKET_MIN || length > NTP_PACKET_MAX) {
        return 1;
    }
    unpack_ntp(data,receive,length);

    /* Start by checking that the packet looks reasonable.  Be a little paranoid,
    but allow for version 1 semantics and sick clients. */

    failed = (data->mode != NTP_SERVER && data->mode != NTP_PASSIVE);
    response = 1;

    if (failed || data->status != 0 || data->version < 1 ||
            data->version > NTP_VERSION_MAX ||
            data->stratum > NTP_STRATUM_MAX) 
    {
        return 1;
    }

    /* Note that the conventions are very poorly defined in the NTP protocol, so we
    have to guess.  Any full NTP server perpetrating completely unsynchronised
    packets is an abomination, anyway, so reject it. */

    delay1 = data->transmit-data->receive;
    delay2 = data->current-data->originate;
    failed = ((data->stratum != 0 && data->stratum != NTP_STRATUM_MAX &&
                data->reference == 0.0) || (data->transmit == 0.0));
    if (response &&
            (data->originate == 0.0 || data->receive == 0.0 ||
                (data->reference != 0.0 && data->receive < data->reference) ||
                delay1 < 0.0 || delay1 > NTP_INSANITY || delay2 < 0.0 ||
                data->dispersion > NTP_INSANITY))
        failed = 1;
    if (failed) 
    {
        return 1;
    }

    /* If it is a response, check that it corresponds to one of our requests and
    has got here in a reasonable length of time. */

    if (response) {
        k = 0;
        for (i = 0; i < attempts; ++i)
            if (data->originate == outgoing[i]) {
                outgoing[i] = 0.0;
                ++k;
            }
        if (k != 1 || delay2 > NTP_INSANITY) 
        {
            return 1;
        }
    }

    /* Now return the time information.  If it is a server response, it contains
    enough information that we can be almost certain that we have not been fooled
    too badly.  Heaven help us with broadcasts - make a wild kludge here, and see
    elsewhere for other kludges. */

    if (dispersion < data->dispersion) dispersion = data->dispersion;

    x = data->receive-data->originate;
    y = (data->transmit == 0.0 ? 0.0 : data->transmit-data->current);
    *off = 0.5*(x+y);
    *err = x-y;
    x = data->current-data->originate;
    if (0.5*x > *err) *err = 0.5*x;

    return 0;
}


void format_time (char *text, int length, double offset, double error,
    double drift, double drifterr) {

    /* Format the current time into a string, with the extra information as
    requested.  Note that the rest of the program uses the correction needed, which
    is what is printed for diagnostics, but this formats the error in the local
    system for display to users.  So the results from this are the negation of
    those printed by the verbose options. */

    int milli, len;
    time_t now;
    struct tm *gmt;
    static const char *months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    /* Work out and format the current local time.  Note that some semi-ANSI
    systems do not set the return value from (s)printf. */

    now = convert_time(current_time(offset),&milli);
    errno = 0;
    if ((gmt = localtime(&now)) == NULL)
        fatal(1,"unable to work out local time",NULL);
    len = 24;
    if (length <= len) fatal(0,"internal error calling format_time",NULL);
    errno = 0;
    sprintf(text,"%.4d %s %.2d %.2d:%.2d:%.2d.%.3d",
            gmt->tm_year+1900,months[gmt->tm_mon],gmt->tm_mday,
            gmt->tm_hour,gmt->tm_min,gmt->tm_sec,milli);
    if (strlen(text) != len)
        fatal(1,"unable to format current local time",NULL);

    /* Append the information about the offset, if requested. */

    if (error >= 0.0) {
        if (length < len+30)
            fatal(0,"internal error calling format_time",NULL);
        errno = 0;
        sprintf(&text[len]," %c %.3f +/- %.3f secs",(offset > 0.0 ? '-' : '+'),
                (offset > 0.0 ? offset : -offset),dispersion+error);
        if (strlen(&text[len]) < 22)
            fatal(1,"unable to format clock correction",NULL);
    }

    /* Append the information about the drift, if requested. */

    if (drifterr >= 0.0) {
        len = strlen(text);
        if (length < len+25)
            fatal(0,"internal error calling format_time",NULL);
        errno = 0;
        sprintf(&text[len]," %c %.1f +/- %.1f ppm",
                (drift > 0.0 ? '-' : '+'),1.0e6*fabs(drift),
                1.0e6*drifterr);
        if (strlen(&text[len]) < 17)
            fatal(1,"unable to format clock correction",NULL);
    }

    /* It would be better to check for field overflow, but it is a lot of code to
    trap extremely implausible scenarios.  This will usually stop chaos from
    spreading. */

    if (strlen(text) >= length)
        fatal(0,"internal error calling format_time",NULL);
}


double reset_clock (double offset, double error, int daemon) {

    /* Reset the clock, if appropriate, and return the correction actually used.
    This contains most of the checking for whether changes are worthwhile, except
    in daemon mode. */

    double absoff = (offset < 0 ? -offset : offset);
    char text[50];
    char ch;

    /* If the correction is large, ask for confirmation before proceeding. */

    if (absoff > prompt) {
        if (! daemon && ftty(stdin) && ftty(stdout)) {
            printf("The time correction is %.3f +/- %.3f+%.3f seconds\n", offset,dispersion,error);
        } else {
            sprintf(text,"%.3f +/- %.3f+%.3f",offset,dispersion,error);
            fatal(0,"time correction too large: %s seconds",text);
        }
    }

    /* See if the correction is reasonably reliable and worth making. */

    if (absoff < (daemon ? 0.5 : 1.0)*minerr) {
        return 0.0;
    } else if (absoff < 2.0*error) {
        return 0.0;
    }

    /* Make the correction.  Provide some protection against the previous
    correction not having completed, but it will rarely help much. */

    adjust_time(offset,(action == action_reset ? 1 : 0), (daemon ? 2.0*minerr : 0.0));
    return offset;
}


int run_client (char *hostnames[], int current) {

    /* Get enough responses to do something with; or not, as the case may be.  Note
    that it allows for half of the packets to be bad, so may make up to twice as
    many attempts as specified by the -c value.  The deadline checking is merely
    paranoia, to protect against broken signal handling - it cannot easily be
    triggered if the signal handling works. */

    double offset, error, deadline, a, b, x, y;
    int accepts = 0, rejects = 0, flushes = 0, replicates = 0, cycle = 0, k;
    unsigned char transmit[NTP_PACKET_MIN];
    ntp_data data;
    char text[100];
    int result, ret;

    ret = open_socket(0,hostnames[curr_serv],delay);
    if (ret < 1)
        return ret;

    attempts = 0;
    deadline = current_time(JAN_1970)+delay;
    offset = 0.0;
    error = NTP_INSANITY;
    while (accepts < count && attempts < 2*count) 
    {
        if (current_time(JAN_1970) > deadline)
            fatal(0,"not enough valid responses received in time",NULL);
        make_packet(&data,NTP_CLIENT);
        outgoing[attempts++] = data.transmit;
        pack_ntp(transmit,NTP_PACKET_MIN,&data);
//        flushes += flush_socket(cycle);
        write_socket(cycle,transmit,NTP_PACKET_MIN);

        result = read_packet(cycle,&data,&x,&y);
        if (result == -1)
        {
            close_socket(0);
            return -1;
        }
        else if (result) {
            if (++rejects > count)
                fatal(0,"too many bad or lost packets",NULL);
            else
                continue;
        } else
            ++accepts;
        if (++cycle >= 1) cycle = 0;

        /* Work out the most accurate time, and check that it isn't more accurate than
        the results warrant. */

        if ((a = x-offset) < 0.0) a = -a;
        if (accepts <= 1) a = 0.0;
        b = error+y;
        if (y < error) {
            offset = x;
            error = y;
        }
        if (a > b) {
            sprintf(text,"%d",cycle);
            fatal(0,"inconsistent times got from NTP server on socket %s",
                text);
        }
        if (error <= minerr) break;
    }

    /* Tidy up the socket, issues diagnostics and perform the action. */

    close_socket(0);

    if (accepts == 0) 
        fatal(0,"no acceptable packets received",NULL);
    if (error > NTP_INSANITY)
        fatal(0,"unable to get a reasonable time estimate",NULL);
    if (action == action_display) 
    {
        format_time(text,75,offset,error,0.0,-1.0);
        printf("%s\n",text);
        if (locked) set_lock(0);
    } 
    else
    {	
        (void)reset_clock(offset,error,0);
	return (1);
    }
    
    return (1);
}


int main (int argc, char *argv[]) {

    /* This is the entry point and all that.  It decodes the arguments and calls
    one of the specialised routines to do the work. */

    int args = argc-1, k;
    char c;
    int res;

    if (argv[0] == NULL || argv[0][0] == '\0')
        argv0 = "msntp";
    else if ((argv0 = strrchr(argv[0],'/')) != NULL)
        ++argv0;
    else
        argv0 = argv[0];

    setvbuf(stdout,NULL,_IOLBF,BUFSIZ);
    setvbuf(stderr,NULL,_IOLBF,BUFSIZ);
    if (INT_MAX < 2147483647) 
	fatal(0,"msntp requires >= 32-bit ints",NULL);

    if (DBL_EPSILON > 1.0e-13)
        fatal(0,"msntp requires doubles with eps <= 1.0e-13",NULL);

    if (argc == 1)
    {
        syntax(args == 1);
	exit(EXIT_FAILURE);
    }

    if ( (strcmp(argv[1],"--help") == 0) || (strcmp(argv[1],"-h") == 0) || (strcmp(argv[1],"-?") == 0) )
        syntax(args == 1);

    if (argc < 11)
    {
        fatal(0,"Argument list not correct. ",NULL);
        syntax(args == 1);
	exit(EXIT_FAILURE);
    }

    action = action_reset;
    curr_serv = 0;

    /* Decode the arguments. */

    while (argc > 1) {
        k = 1;

	if (strcmp(argv[1],"-s") == 0) 
	{
            operation = op_client;
            if( argv[2][0] == '\0' || argv[2][0] == '-' ||
                argv[3][0] == '\0' || argv[3][0] == '-' ||
                argv[4][0] == '\0' || argv[4][0] == '-' )
                fatal(0,"invalid Internet address '%s'",argv[k]);
	    sntp_serv[0] = argv[2];
	    sntp_serv[1] = argv[3];
	    sntp_serv[2] = argv[4];
            k = 2;
        } 
	else if (strcmp(argv[1],"-t") == 0) 
	{
	    if (!isdigit(argv[2][0]))
	    {
	        printf("Invalid Arguments.\n");
                syntax(args == 1);
	    }  
	    timeout = atoi(argv[2]);
            k = 2;
        } 
	else if (strcmp(argv[1],"-r") == 0) 
	{
	    if (!isdigit(argv[2][0]))
	    {
	        printf("Invalid Arguments.\n");
                syntax(args == 1);
	    }  
	    retry = atoi(argv[2]);
            k = 2;
        } 
	else if (strcmp(argv[1],"-p") == 0) 
	{
	    if (!isdigit(argv[2][0]))
	    {
	        printf("Invalid Arguments.\n");
                syntax(args == 1);
	    }  
	    poll_interval = atoi(argv[2])*60;
            k = 2;
        } 
	else
            break;
        argc -= k;
        argv += k;
    }

    if (action == 0) action = action_display;
    if (count == 0) count = (argc-1 < 5 ? 5 : argc-1);
    if ((action == action_reset) && lockname == NULL)
        lockname = LOCKNAME;

    /* The '-x' option changes the implications of many other settings, though this
    is not usually apparent to the caller.  Most of the time delays are to ensure
    that stuck states terminate, and do not affect the result. */

    if (prompt == 0.0) prompt = 30.0;

    /* Diagnose where we are, if requested, and separate out the classes of 
    operation.  The calls do not return. */
  
    delay = 15;
    waiting = 3;
    count = 5;

    for(;;)
    {
        res = run_client(sntp_serv,1);        
        while(res == -1)
        {
          res = run_client(sntp_serv,2);        
	  if (res == 0)
    	    return EXIT_FAILURE;
        }
        curr_serv = 0;
        curr_attempt = 0;
        sleep(poll_interval-50);
    }

    fatal(0,"internal error at end of main",NULL);

    return EXIT_FAILURE;
}
