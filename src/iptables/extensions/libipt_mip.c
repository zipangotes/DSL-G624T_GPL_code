/* Shared library to add-on to iptables to add multiple source IP support */

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ipt_mip.h>

/* Function which prints out usage message. */
static void
help(void)
{
    printf(
"multi source IP v%s options:\n"
" --source-ips ip[,ip,ip...]\n"
" --sips ...                    match source IP address(es)\n" 
" --isource-ips ip[,ip,ip...]\n"
" --isips...                     invert the source IP match \n",
NETFILTER_VERSION);
}   

static struct option opts[] = {
    { "source-ips", 1, 0, '1' },
    { "sips", 1, 0, '1' },    /* synonym */
    { "isource-ips", 1, 0, '2' },
    { "isips", 1, 0, '2'},    /* synonym */
    {0}
};


/*-------------------------------------------------------------
  Convert IP strings to uint32
  ------------------------------------------------------------*/
static u_int32_t *
parse_mip(const char *ipstring)
{
        struct in_addr *ip;

        ip = dotted_to_addr(ipstring);
        if (!ip)
          exit_error(PARAMETER_PROBLEM, "ip match: Bad IP address `%s'\n", ipstring);
        else 
          return (u_int32_t *)ip;
}

/*-------------------------------------------------------------
  Parse the multiple source IP strings
  -------------------------------------------------------------*/
static unsigned int
parse_multi_ips(const char *ipstring, u_int32_t *msip)
{
	char *buffer, *cp, *next;
	u_int32_t i;
        u_int32_t *tmp_ip;

	buffer = strdup(ipstring);
	if (!buffer) exit_error(OTHER_PROBLEM, "strdup failed");

	for (cp=buffer, i=0, next=(char *)1; cp && i<(IPT_MULTIIP_NUM); cp=next,i++)
	{
            next=strchr(cp, ',');
            if (next) *next++='\0';
//            printf("[i=%d] next:%s cp:%s\n",i,next,cp);
            if(*cp != 0)
            {
                tmp_ip = parse_mip(cp);                     /* Convert IP string to int */
//                printf("tmp_ip = 0x%x\n",*tmp_ip);
                msip[i] = *tmp_ip;
            }
        }
//	if (cp) exit_error(PARAMETER_PROBLEM, "too many source ip addresses specified");
	free(buffer);
//        printf("Count = %d\n",i);
	return (i);
}

/*-----------------------*/
/* Initialize the match. */
/*-----------------------*/
static void
init(struct ipt_entry_match *m, unsigned int *nfcache)
{
}

/*-------------------------------------------------------------
   Function which parses command options; returns true if it
   ate an option 
 -------------------------------------------------------------*/
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      unsigned int *nfcache,
      struct ipt_entry_match **match)
{
	struct ipt_multiip *multiinfo
		= (struct ipt_multiip *)(*match)->data;
        
        
	switch (c) {
	case '1':
                multiinfo->flags = IPT_MIP_NORM_MATCH;
		multiinfo->count = parse_multi_ips(argv[optind-1], multiinfo->multip);
		*nfcache |= NFC_IP_SRC_PT;
		break;
        case '2':
               multiinfo->flags = IPT_MIP_INV_MATCH;
               multiinfo->count = parse_multi_ips(argv[optind-1], multiinfo->multip);
               *nfcache |= NFC_IP_SRC_PT;
               break;
	default:
		return 0;
	}    

	if (*flags)
		exit_error(PARAMETER_PROBLEM,
			   "multi-ip can only have one option");
        *flags = 1;
        return 1;
}

/*--------------------------------------*/
/* Final check; must specify something. */
/*--------------------------------------*/
static void
final_check(unsigned int flags)
{
	if (!flags)
		exit_error(PARAMETER_PROBLEM, "multiIP expects an option!");
}

/* --------------------------*/
/* Prints out the matchinfo. */
/* --------------------------*/
static void
print(const struct ipt_ip *ip,
      const struct ipt_entry_match *match,
      int numeric)
{
	const struct ipt_multiip *multiinfo
		= (const struct ipt_multiip *)match->data;
	u_int32_t i;
        struct in_addr ip_addr;
	
	if(multiinfo->flags & IPT_MIP_INV_MATCH)
           printf("multiple source IP (inv): ");
	else
	   printf("multiple source IP: ");
        ip_addr.s_addr = 0;

        for (i=0; i < (multiinfo->count); i++)
        {
            ip_addr.s_addr = multiinfo->multip[i];
            if(ip_addr.s_addr != 0)
              printf("%s,", addr_to_dotted((const struct in_addr *)&ip_addr));
            ip_addr.s_addr = 0;
        }
        printf(" ");
}


/*-----------------------------------------------------------*/
/* Saves the union ipt_matchinfo in parsable form to stdout. */
/*-----------------------------------------------------------*/
static void 
save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
	const struct ipt_multiip *multiinfo
		= (const struct ipt_multiip *)match->data;
	u_int32_t i;
        struct in_addr ip_addr;

	printf("multiple source IP: ");
        ip_addr.s_addr = 0;
        for (i=0; i < (multiinfo->count); i++)
        {
            ip_addr.s_addr = multiinfo->multip[i];
            if(ip_addr.s_addr != 0)
              printf("%s,", addr_to_dotted((const struct in_addr *)&ip_addr));
            ip_addr.s_addr = 0;
        }
        printf(" ");
}

static
struct iptables_match mip
= { NULL,
    "mip",
    NETFILTER_VERSION,
    IPT_ALIGN(sizeof(struct ipt_multiip)),
    IPT_ALIGN(sizeof(struct ipt_multiip)),
    &help,
    &init,
    &parse,
    &final_check,
    &print,
    &save,
    opts
};

void
_init(void)
{
	register_match(&mip);
}
