/*------------------------------------------------------------------------------

 Copyright (c) 2003 Texas Instruments Incorporated

 ALL RIGHTS RESERVED



"This computer program is subject to a separate license agreement

 signed by Texas Instruments Incorporated and the licensee, and is

 subject to the restrictions therein.  No other rights to reproduce,

 use, or disseminate this computer program, whether in part or in

 whole, are granted."



--------------------------------------------------------------------------------

 Module Name:   Kernel extension in netfilter.New target CFG



 Module Purpose:   To Control the execution of udhcp client



--------------------------------------------------------------------------------

 Revision History:

 Date and Type of Change.
 04/08/04 -> Creation date

------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>


/* Function which prints out usage message. */
static void
help(void)
{
  printf("Records Packet's Source Interface \n");
}

static struct option opts[] = {};

static void
init(struct ipt_entry_target *t, unsigned int *nfcache)
{
}

static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      struct ipt_entry_target **target)
{
	return 0;
}


static void final_check(unsigned int flags)
{
}

static void
print(const struct ipt_ip *ip,
      const struct ipt_entry_target *target,
      int numeric)
{
  printf("Records Packet's Source Interface \n");
}

static void save(const struct ipt_ip *ip, const struct ipt_entry_target *target)
{
}

static
struct iptables_target cfg
= { NULL,
    "CFG",
    NETFILTER_VERSION,
    IPT_ALIGN(0),
    IPT_ALIGN(0),
    &help,
    &init,
    &parse,
    &final_check,
    &print,
    &save,
    opts
};

void _init(void)
{
	register_target(&cfg);
}
