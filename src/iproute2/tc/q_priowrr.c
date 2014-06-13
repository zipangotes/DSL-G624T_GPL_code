/*
 * q_priowrr.c		PRIO_WRR.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "utils.h"
#include "tc_util.h"

static void explain(void)
{
	fprintf(stderr, "Usage: ... priowrr weightmap W1 W2 W3 W4 qlenmap L1 L2 L3 L4\n");
	fprintf(stderr, "  W1,W2,W3,W4 - weights corresponding two High, Medium and Low priority queues\n");
	fprintf(stderr, "                in percentage; W1 defaults 100 and Sum(W2,W3)=100\n");
	fprintf(stderr, "  L1,L2,L3,L4 - queue lengths correspnding to two High, Medium and Low priority queues\n");
}

#define usage() return(-1)

static int prio_parse_opt(struct qdisc_util *qu, int argc, char **argv, struct nlmsghdr *n)
{
	int wmap_mode = 0, lmap_mode = 0;
	int idx = 0;
	struct tc_priowrr_qopt opt={{ 100, 100, 60, 40}, {15, 20, 0, 0}};

	while (argc > 0) {
		if (strcmp(*argv, "weightmap") == 0) {
			if (wmap_mode) {
				fprintf(stderr, "Error: duplicate weightmap\n");
				return -1;
			}
			wmap_mode = 1;
			lmap_mode = 0;
			idx = 0;
		} else if (strcmp(*argv, "qlenmap") == 0) {
			if (lmap_mode) {
				fprintf(stderr, "Error: duplicate qlenmap\n");
				return -1;
			}
			lmap_mode = 1;
			wmap_mode = 0;
			idx = 0;
		} else if (strcmp(*argv, "help") == 0) {
			explain();
			return -1;
		} else {
			unsigned value;
			if (!wmap_mode && !lmap_mode) {
				fprintf(stderr, "What is \"%s\"?\n", *argv);
				explain();
				return -1;
			}
			if (get_unsigned(&value, *argv, 10)) {
				fprintf(stderr, "Illegal \"%s\" element\n", wmap_mode==1?"weightmap":"qlenmap");
				return -1;
			}
			if (idx >= TCQ_PRIOWRR_BANDS) {
				fprintf(stderr, "\"%s\" index > bands=%u\n", wmap_mode==1?"weightmap":"qlenmap",TCQ_PRIOWRR_BANDS);
				return -1;
			}
			if (wmap_mode)
				opt.weightmap[idx++] = value;
			else
				opt.qlenmap[idx++] = value;
		}
		argc--; argv++;
	}

	opt.weightmap[0] = 100; /* Weight for EFQ1 is always 100. */
	opt.weightmap[1] = 100; /* Weight for EFQ2 is always 100. */
	
	addattr_l(n, 1024, TCA_OPTIONS, &opt, sizeof(opt));
	return 0;
}

static int prio_print_opt(struct qdisc_util *qu, FILE *f, struct rtattr *opt)
{
	int i;
	struct tc_priowrr_qopt *qopt;

	if (opt == NULL)
		return 0;

	if (RTA_PAYLOAD(opt)  < sizeof(*qopt))
		return -1;
	qopt = RTA_DATA(opt);
	fprintf(f, "weightmap");
	for (i=0; i<TCQ_PRIOWRR_BANDS; i++)
		fprintf(f, " %d", qopt->weightmap[i]);
	fprintf(f, " qlenmap");	
	for (i=0; i<TCQ_PRIOWRR_BANDS; i++)
		fprintf(f, " %d", qopt->qlenmap[i]);
	return 0;
}

static int prio_print_xstats(struct qdisc_util *qu, FILE *f, struct rtattr *xstats)
{
	return 0;
}


struct qdisc_util priowrr_util = {
	NULL,
	"priowrr",
	prio_parse_opt,
	prio_print_opt,
	prio_print_xstats,
};

