/* 
 * files.c -- DHCP server file manipulation *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 */
 
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <netdb.h>

#include "debug.h"
#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "leases.h"


#define LEASE_ADD		1
#define LEASE_DEL 	2
extern void write_to_delta(u_int8_t *chaddr, u_int32_t yiaddr, u_int8_t *hname,unsigned long leasetime,u_int8_t action);

/* on these functions, make sure you datatype matches */
static int read_ip(char *line, void *arg)
{
	struct in_addr *addr = arg;
	struct hostent *host;
	int retval = 1;

	if (!inet_aton(line, addr)) {
		if ((host = gethostbyname(line))) 
			addr->s_addr = *((unsigned long *) host->h_addr_list[0]);
		else retval = 0;
	}
	return retval;
}


static int read_str(char *line, void *arg)
{
	char **dest = arg;
	
	if (*dest) free(*dest);
	*dest = strdup(line);
	
	return 1;
}


static int read_u32(char *line, void *arg)
{
	u_int32_t *dest = arg;
	char *endptr;
	*dest = strtoul(line, &endptr, 0);
	return endptr[0] == '\0';
}


static int read_yn(char *line, void *arg)
{
	char *dest = arg;
	int retval = 1;

	if (!strcasecmp("yes", line))
		*dest = 1;
	else if (!strcasecmp("no", line))
		*dest = 0;
	else retval = 0;
	
	return retval;
}


/* read a dhcp option and add it to opt_list */
static int read_opt(char *line, void *arg)
{
	struct option_set **opt_list = arg;
	char *opt, *val, *endptr;
	struct dhcp_option *option = NULL;
	int retval = 0, length = 0;
	char buffer[255];
	u_int16_t result_u16;
	u_int32_t result_u32;
	int i;

	if (!(opt = strtok(line, " \t="))) return 0;
	
	for (i = 0; options[i].code; i++)
		if (!strcmp(options[i].name, opt))
			option = &(options[i]);
		
	if (!option) return 0;
	
	do {
		val = strtok(NULL, ", \t");
		if (val) {
			length = option_lengths[option->flags & TYPE_MASK];
			retval = 0;
			switch (option->flags & TYPE_MASK) {
			case OPTION_IP:
				retval = read_ip(val, buffer);
				break;
			case OPTION_IP_PAIR:
				retval = read_ip(val, buffer);
				if (!(val = strtok(NULL, ", \t/-"))) retval = 0;
				if (retval) retval = read_ip(val, buffer + 4);
				break;
			case OPTION_STRING:
				length = strlen(val);
				if (length > 0) {
					if (length > 254) length = 254;
					memcpy(buffer, val, length);
					retval = 1;
				}
				break;
			case OPTION_BOOLEAN:
				retval = read_yn(val, buffer);
				break;
			case OPTION_U8:
				buffer[0] = strtoul(val, &endptr, 0);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_U16:
				result_u16 = htons(strtoul(val, &endptr, 0));
				memcpy(buffer, &result_u16, 2);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_S16:
				result_u16 = htons(strtol(val, &endptr, 0));
				memcpy(buffer, &result_u16, 2);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_U32:
				result_u32 = htonl(strtoul(val, &endptr, 0));
				memcpy(buffer, &result_u32, 4);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_S32:
				result_u32 = htonl(strtol(val, &endptr, 0));	
				memcpy(buffer, &result_u32, 4);
				retval = (endptr[0] == '\0');
				break;
			default:
				break;
			}
			if (retval) 
				attach_option(opt_list, option, buffer, length);
		};
	} while (val && retval && option->flags & OPTION_LIST);
	return retval;
}

static struct config_kw_arr k_arr[MAX_INTERFACES] = {
	{/* keyword[14]	handler   variable address		default[20] */
	{
  {"start",	read_ip,  &(server_config[0].start),	"192.168.0.20"},
	{"end",		read_ip,  &(server_config[0].end),		"192.168.0.254"},
	{"interface",	read_str, &(server_config[0].interface),	"eth0"},
	{"inflease_time",read_u32,&(server_config[0].inflease_time),"604800"},
	{"option",	read_opt, &(server_config[0].options),	""},
	{"opt",		read_opt, &(server_config[0].options),	""},
	{"max_leases",	read_u32, &(server_config[0].max_leases),	"254"},
	{"remaining",	read_yn,  &(server_config[0].remaining),	"yes"},
	{"auto_time",	read_u32, &(server_config[0].auto_time),	"1"/*"7200"*/},
	{"decline_time",read_u32, &(server_config[0].decline_time),"3600"},
	{"conflict_time",read_u32,&(server_config[0].conflict_time),"3600"},
	{"offer_time",	read_u32, &(server_config[0].offer_time),	"60"},
	{"min_lease",	read_u32, &(server_config[0].min_lease),	"60"},
	{"lease_file",	read_str, &(server_config[0].lease_file),	"/var/lib/misc/udhcpd0.leases"},
	{"pidfile",	read_str, &(server_config[0].pidfile),	"/var/run/udhcpd.pid"},
	{"notify_file", read_str, &(server_config[0].notify_file),	""},
	{"siaddr",	read_ip,  &(server_config[0].siaddr),	"0.0.0.0"},
	{"sname",	read_str, &(server_config[0].sname),	""},
	{"boot_file",	read_str, &(server_config[0].boot_file),	""}
	/*ADDME: static lease */}},
	{/* keyword[14]	handler   variable address		default[20] */
	{{"start",	read_ip,  &(server_config[1].start),	"192.168.1.20"},
	{"end",		read_ip,  &(server_config[1].end),		"192.168.1.254"},
	{"interface",	read_str, &(server_config[1].interface),	"eth1"},
	{"inflease_time",read_u32,&(server_config[1].inflease_time),"604800"},
	{"option",	read_opt, &(server_config[1].options),	""},
	{"opt",		read_opt, &(server_config[1].options),	""},
	{"max_leases",	read_u32, &(server_config[1].max_leases),	"254"},
	{"remaining",	read_yn,  &(server_config[1].remaining),	"yes"},
	{"auto_time",	read_u32, &(server_config[1].auto_time),	"1"/*"7200"*/},
	{"decline_time",read_u32, &(server_config[1].decline_time),"3600"},
	{"conflict_time",read_u32,&(server_config[1].conflict_time),"3600"},
	{"offer_time",	read_u32, &(server_config[1].offer_time),	"60"},
	{"min_lease",	read_u32, &(server_config[1].min_lease),	"60"},
	{"lease_file",	read_str, &(server_config[1].lease_file),	"/var/lib/misc/udhcpd1.leases"},
	{"pidfile",	read_str, &(server_config[1].pidfile),	"/var/run/udhcpd.pid"},
	{"notify_file", read_str, &(server_config[1].notify_file),	""},
	{"siaddr",	read_ip,  &(server_config[1].siaddr),	"0.0.0.0"},
	{"sname",	read_str, &(server_config[1].sname),	""},
	{"boot_file",	read_str, &(server_config[1].boot_file),	""}
	/*ADDME: static lease */}},
	{/* keyword[14]	handler   variable address		default[20] */
	{{"start",	read_ip,  &(server_config[2].start),	"192.168.2.20"},
	{"end",		read_ip,  &(server_config[2].end),		"192.168.2.254"},
	{"interface",	read_str, &(server_config[2].interface),	"usbrndis"},
	{"inflease_time",read_u32,&(server_config[2].inflease_time),"604800"},
	{"option",	read_opt, &(server_config[2].options),	""},
	{"opt",		read_opt, &(server_config[2].options),	""},
	{"max_leases",	read_u32, &(server_config[2].max_leases),	"254"},
	{"remaining",	read_yn,  &(server_config[2].remaining),	"yes"},
	{"auto_time",	read_u32, &(server_config[2].auto_time),	"1"/*"7200"*/},
	{"decline_time",read_u32, &(server_config[2].decline_time),"3600"},
	{"conflict_time",read_u32,&(server_config[2].conflict_time),"3600"},
	{"offer_time",	read_u32, &(server_config[2].offer_time),	"60"},
	{"min_lease",	read_u32, &(server_config[2].min_lease),	"60"},
	{"lease_file",	read_str, &(server_config[2].lease_file),	"/var/lib/misc/udhcpd2.leases"},
	{"pidfile",	read_str, &(server_config[2].pidfile),	"/var/run/udhcpd.pid"},
	{"notify_file", read_str, &(server_config[2].notify_file),	""},
	{"siaddr",	read_ip,  &(server_config[2].siaddr),	"0.0.0.0"},
	{"sname",	read_str, &(server_config[2].sname),	""},
	{"boot_file",	read_str, &(server_config[2].boot_file),	""}
	/*ADDME: static lease */}},
        {/* keyword[14] handler   variable address              default[20] */
        {{"start",      read_ip,  &(server_config[3].start),    "192.168.3.20"},
        {"end",         read_ip,  &(server_config[3].end),              "192.168.3.254"},
        {"interface",   read_str, &(server_config[3].interface),        "usbrndis"},
        {"inflease_time",read_u32,&(server_config[3].inflease_time),"604800"},
        {"option",      read_opt, &(server_config[3].options),  ""},
        {"opt",         read_opt, &(server_config[3].options),  ""},
        {"max_leases",  read_u32, &(server_config[3].max_leases),       "254"},
        {"remaining",   read_yn,  &(server_config[3].remaining),        "yes"},
        {"auto_time",   read_u32, &(server_config[3].auto_time),        "1"/*"7200"*/},
        {"decline_time",read_u32, &(server_config[3].decline_time),"3600"},
        {"conflict_time",read_u32,&(server_config[3].conflict_time),"3600"},
        {"offer_time",  read_u32, &(server_config[3].offer_time),       "60"},
        {"min_lease",   read_u32, &(server_config[3].min_lease),        "60"},
        {"lease_file",  read_str, &(server_config[3].lease_file),       "/var/lib/misc/udhcpd2.leases"},
        {"pidfile",     read_str, &(server_config[3].pidfile),  "/var/run/udhcpd.pid"},
        {"notify_file", read_str, &(server_config[3].notify_file),      ""},
        {"siaddr",      read_ip,  &(server_config[3].siaddr),   "0.0.0.0"},
        {"sname",       read_str, &(server_config[3].sname),    ""},
        {"boot_file",   read_str, &(server_config[3].boot_file),        ""}
        /*ADDME: static lease */}},
        {/* keyword[14] handler   variable address              default[20] */
        {{"start",      read_ip,  &(server_config[4].start),    "192.168.4.20"},
        {"end",         read_ip,  &(server_config[4].end),              "192.168.4.254"},
        {"interface",   read_str, &(server_config[4].interface),        "usbrndis"},
        {"inflease_time",read_u32,&(server_config[4].inflease_time),"604800"},
        {"option",      read_opt, &(server_config[4].options),  ""},
        {"opt",         read_opt, &(server_config[4].options),  ""},
        {"max_leases",  read_u32, &(server_config[4].max_leases),       "254"},
        {"remaining",   read_yn,  &(server_config[4].remaining),        "yes"},
        {"auto_time",   read_u32, &(server_config[4].auto_time),        "1"/*"7200"*/},
        {"decline_time",read_u32, &(server_config[4].decline_time),"3600"},
        {"conflict_time",read_u32,&(server_config[4].conflict_time),"3600"},
        {"offer_time",  read_u32, &(server_config[4].offer_time),       "60"},
        {"min_lease",   read_u32, &(server_config[4].min_lease),        "60"},
        {"lease_file",  read_str, &(server_config[4].lease_file),       "/var/lib/misc/udhcpd2.leases"},
        {"pidfile",     read_str, &(server_config[4].pidfile),  "/var/run/udhcpd.pid"},
        {"notify_file", read_str, &(server_config[4].notify_file),      ""},
        {"siaddr",      read_ip,  &(server_config[4].siaddr),   "0.0.0.0"},
        {"sname",       read_str, &(server_config[4].sname),    ""},
        {"boot_file",   read_str, &(server_config[4].boot_file),        ""}
        /*ADDME: static lease */}},
        {/* keyword[14] handler   variable address              default[20] */
        {{"start",      read_ip,  &(server_config[5].start),    "192.168.5.20"},
        {"end",         read_ip,  &(server_config[5].end),              "192.168.5.254"},
        {"interface",   read_str, &(server_config[5].interface),        "usbrndis"},
        {"inflease_time",read_u32,&(server_config[5].inflease_time),"604800"},
        {"option",      read_opt, &(server_config[5].options),  ""},
        {"opt",         read_opt, &(server_config[5].options),  ""},
        {"max_leases",  read_u32, &(server_config[5].max_leases),       "254"},
        {"remaining",   read_yn,  &(server_config[5].remaining),        "yes"},
        {"auto_time",   read_u32, &(server_config[5].auto_time),        "1"/*"7200"*/},
        {"decline_time",read_u32, &(server_config[5].decline_time),"3600"},
        {"conflict_time",read_u32,&(server_config[5].conflict_time),"3600"},
        {"offer_time",  read_u32, &(server_config[5].offer_time),       "60"},
        {"min_lease",   read_u32, &(server_config[5].min_lease),        "60"},
        {"lease_file",  read_str, &(server_config[5].lease_file),       "/var/lib/misc/udhcpd2.leases"},
        {"pidfile",     read_str, &(server_config[5].pidfile),  "/var/run/udhcpd.pid"},
        {"notify_file", read_str, &(server_config[5].notify_file),      ""},
        {"siaddr",      read_ip,  &(server_config[5].siaddr),   "0.0.0.0"},
        {"sname",       read_str, &(server_config[5].sname),    ""},
        {"boot_file",   read_str, &(server_config[5].boot_file),        ""}
        /*ADDME: static lease */}}
 
};


int read_config(char *file)
{
	FILE *in;
	char buffer[80], orig[80], *token, *line;
	int i, j, index;

	for (index = 0; index < MAX_INTERFACES; index++)
		for (j = 0; j < MAX_KEYWORDS; j++)
			if (strlen(k_arr[index].keywords[j].def))
				k_arr[index].keywords[j].handler(k_arr[index].keywords[j].def, k_arr[index].keywords[j].var);

	
	if (!(in = fopen(file, "r"))) {
		LOG(LOG_ERR, "unable to open config file: %s", file);
		return 0;
	}

	index = -1;	
	while (fgets(buffer, 80, in)) {
		if (strchr(buffer, '\n')) *(strchr(buffer, '\n')) = '\0';
		strncpy(orig, buffer, 80);
		if (strchr(buffer, '#')) *(strchr(buffer, '#')) = '\0';
		token = buffer + strspn(buffer, " \t");
		if (*token == '\0') continue;
		line = token + strcspn(token, " \t=");
		if (*line == '\0') continue;
		*line = '\0';
		line++;
		
		/* eat leading whitespace */
		line = line + strspn(line, " \t=");
		/* eat trailing whitespace */
		for (i = strlen(line) ; i > 0 && isspace(line[i-1]); i--);
		line[i] = '\0';
	
		for (j = 0; j < MAX_KEYWORDS; j++)
		{
			if (!strcasecmp(token, "start")) {
				index++;
			}		
			if (!strcasecmp(token, k_arr[index].keywords[j].keyword)) {
				if (!k_arr[index].keywords[j].handler(line, k_arr[index].keywords[j].var)) {
					LOG(LOG_ERR, "unable to parse '%s'", orig);
					/* reset back to the default value */
					k_arr[index].keywords[j].handler(k_arr[index].keywords[j].def, k_arr[index].keywords[j].var);
				}
				break;
			}
		}
	}
	no_of_ifaces = index+1;
	fclose(in);
	return 1;
}


/* the dummy var is here so this can be a signal handler */
void write_leases(int ifid)
{
	FILE *fp;
	unsigned int i;
	char buf[255];
	time_t curr = time(0);
	unsigned int lease_time;
  int j;
  unsigned char line[100];
  struct in_addr in;
	
	if (!(fp = fopen(server_config[ifid].lease_file, "w"))) {
		LOG(LOG_ERR, "Unable to open %s for writing", server_config[ifid].lease_file);
		return;
	}
	
	for (i = 0; i < server_config[ifid].max_leases; i++) {
		if (server_config[ifid].leases[i].yiaddr != 0) {
			if (server_config[ifid].remaining) {
				if (lease_expired(&(server_config[ifid].leases[i]),ifid))
				{
					lease_time = 0;
					if( server_config[ifid].leases[i].expires != 0)
						write_to_delta(server_config[ifid].leases[i].chaddr,
                           server_config[ifid].leases[i].yiaddr,
                           server_config[ifid].leases[i].hostname,0,LEASE_DEL);
					server_config[ifid].leases[i].expires = 0;
				}
				else
				{
					if( server_config[ifid].leases[i].expires != server_config[ifid].inflease_time)
        		lease_time = server_config[ifid].leases[i].expires - curr;
					else
						lease_time = server_config[ifid].inflease_time;
				}
			}
			else
				lease_time = server_config[ifid].leases[i].expires;

      sprintf(line,"%02x:%02x:%02x:%02x:%02x:%02x ",server_config[ifid].leases[i].chaddr[0],
																								    server_config[ifid].leases[i].chaddr[1],
																								    server_config[ifid].leases[i].chaddr[2],
																								    server_config[ifid].leases[i].chaddr[3],
																								    server_config[ifid].leases[i].chaddr[4],
																								    server_config[ifid].leases[i].chaddr[5]);

      in.s_addr = server_config[ifid].leases[i].yiaddr;
      j = sprintf(&line[18],"%s %ld %s\n",inet_ntoa(in),lease_time,server_config[ifid].leases[i].hostname);
      fwrite( line, 18+j, 1, fp);
		}
	}
	fclose(fp);
	
	if (server_config[ifid].notify_file) {
		sprintf(buf, "%s %s", server_config[ifid].notify_file, server_config[ifid].lease_file);
		system(buf);
	}
}


void read_leases(char *file, int ifid)
{
	FILE *fp;
	unsigned int n;
	struct dhcpOfferedAddr lease;
	
  char ipaddress[17],macaddr[18];
  char hname[50];
  unsigned int leasetime;
  struct in_addr ipaddr;

/*  mac ipaddress leasetime hostname */
#define	readentry(fp) \
	fscanf((fp), "%s %s %u %s\n", \
		 macaddr, ipaddress, &leasetime, hname)

	if (!(fp = fopen(file, "r"))) {
		LOG(LOG_ERR, "Unable to open %s for reading", file);
		return;
	}

	for (;;)
  {
		if ((n = readentry(fp)) == EOF)
			break;
		if (!inet_aton(ipaddress, &ipaddr))
			continue;
    lease.yiaddr = ipaddr.s_addr;
    memset(lease.chaddr,0x00,16);
    sscanf(macaddr,"%x:%x:%x:%x:%x:%x", &lease.chaddr[0],
																										&lease.chaddr[1],
																										&lease.chaddr[2],
																										&lease.chaddr[3],
																										&lease.chaddr[4],
																										&lease.chaddr[5]);
    lease.expires = leasetime;
    strcpy(lease.hostname,hname);

		if (lease.yiaddr >= server_config[ifid].start && lease.yiaddr <= server_config[ifid].end)
    {
			if (!server_config[ifid].remaining) lease.expires -= time(0);
			if (!(add_lease(lease.chaddr, lease.yiaddr, lease.expires, ifid, lease.hostname)))
      {
				LOG(LOG_WARNING, "Too many leases while loading %s\n", file);
				break;
			}
    }				
	}
	fclose(fp);
}
		
		
