#ifndef _IP_CONNTRACK_H
#define _IP_CONNTRACK_H
/* Connection state tracking for netfilter.  This is separated from,
   but required by, the NAT layer; it can also be used by an iptables
   extension. */

#include <linux/config.h>
#include <linux/netfilter_ipv4/ip_conntrack_tuple.h>
#include <asm/atomic.h>

enum ip_conntrack_info
{
	/* Part of an established connection (either direction). */
	IP_CT_ESTABLISHED,

	/* Like NEW, but related to an existing connection, or ICMP error
	   (in either direction). */
	IP_CT_RELATED,

	/* Started a new connection to track (only
           IP_CT_DIR_ORIGINAL); may be a retransmission. */
	IP_CT_NEW,

	/* >= this indicates reply direction */
	IP_CT_IS_REPLY,

	/* Number of distinct IP_CT types (no NEW in reply dirn). */
	IP_CT_NUMBER = IP_CT_IS_REPLY * 2 - 1
};

/* marian: moved outside the ifdef __KERNEL__ to be able to compile 
 * libipt_conntrack.c in iptables
 */
/* Bitset representing status of connection. */
enum ip_conntrack_status {
        /* It's an expected connection: bit 0 set.  This bit never changed */
        IPS_EXPECTED_BIT = 0,
        IPS_EXPECTED = (1 << IPS_EXPECTED_BIT),

        /* We've seen packets both ways: bit 1 set.  Can be set, not unset. */
        IPS_SEEN_REPLY_BIT = 1,
        IPS_SEEN_REPLY = (1 << IPS_SEEN_REPLY_BIT),

        /* Conntrack should never be early-expired. */
        IPS_ASSURED_BIT = 2,
        IPS_ASSURED = (1 << IPS_ASSURED_BIT),
};

#ifdef __KERNEL__

#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/netfilter_ipv4/ip_conntrack_tcp.h>
#include <linux/netfilter_ipv4/ip_conntrack_icmp.h>
#include <linux/netfilter_ipv4/ip_conntrack_proto_gre.h>

#ifdef CONFIG_NF_DEBUG
#define IP_NF_ASSERT(x)							\
do {									\
	if (!(x))							\
		/* Wooah!  I'm tripping my conntrack in a frenzy of	\
		   netplay... */					\
		printk("NF_IP_ASSERT: %s:%i(%s)\n",			\
		       __FILE__, __LINE__, __FUNCTION__);		\
} while(0)
#else
#define IP_NF_ASSERT(x)
#endif

#if 0
/* Bitset representing status of connection. */
enum ip_conntrack_status {
	/* It's an expected connection: bit 0 set.  This bit never changed */
	IPS_EXPECTED_BIT = 0,
	IPS_EXPECTED = (1 << IPS_EXPECTED_BIT),

	/* We've seen packets both ways: bit 1 set.  Can be set, not unset. */
	IPS_SEEN_REPLY_BIT = 1,
	IPS_SEEN_REPLY = (1 << IPS_SEEN_REPLY_BIT),

	/* Conntrack should never be early-expired. */
	IPS_ASSURED_BIT = 2,
	IPS_ASSURED = (1 << IPS_ASSURED_BIT),
};
#endif

#ifdef CONFIG_IP_NF_NAT_NEEDED
#include <linux/netfilter_ipv4/ip_nat.h>
#include <linux/netfilter_ipv4/ip_nat_pptp.h>

#endif

/* Add protocol helper include file here */
#include <linux/netfilter_ipv4/ip_conntrack_talk.h>
#include <linux/netfilter_ipv4/ip_conntrack_pptp.h>
#include <linux/netfilter_ipv4/ip_conntrack_h323.h>

#include <linux/netfilter_ipv4/ip_conntrack_ftp.h>
#include <linux/netfilter_ipv4/ip_conntrack_irc.h>

struct ip_conntrack_expect
{
	/* Internal linked list (global expectation list) */
	struct list_head list;

	/* expectation list for this master */
	struct list_head expected_list;

	/* The conntrack of the master connection */
	struct ip_conntrack *expectant;

	/* The conntrack of the sibling connection, set after
	 * expectation arrived */
	struct ip_conntrack *sibling;

	/* Tuple saved for conntrack */
	struct ip_conntrack_tuple ct_tuple;

	/* Timer function; deletes the expectation. */
	struct timer_list timeout;

	/* Data filled out by the conntrack helpers follow: */

	/* We expect this tuple, with the following mask */
	struct ip_conntrack_tuple tuple, mask;

	/* Function to call after setup and insertion */
	int (*expectfn)(struct ip_conntrack *new);

	/* At which sequence number did this expectation occur */
	u_int32_t seq;

        union {
	   /* insert l4proto helper private data (expect) herre */
	   struct ip_ct_gre_expect gre;
	} proto;	   

	union {
		/* insert conntrack helper private data (expect) here */
		struct ip_ct_talk_expect exp_talk_info;
		struct ip_ct_h225_expect exp_h225_info;
		struct ip_ct_ftp_expect exp_ftp_info;
		struct ip_ct_irc_expect exp_irc_info;
		struct ip_ct_pptp_expect exp_pptp_info;

#ifdef CONFIG_IP_NF_NAT_NEEDED
 		union {
			/* insert nat helper private data (expect) here */
		} nat;
#endif
	} help;
};

struct ip_conntrack
{
	/* Usage count in here is 1 for hash table/destruct timer, 1 per skb,
           plus 1 for any connection(s) we are `master' for */
	struct nf_conntrack ct_general;

	/* These are my tuples; original and reply */
	struct ip_conntrack_tuple_hash tuplehash[IP_CT_DIR_MAX];

	/* Have we seen traffic both ways yet? (bitset) */
	volatile unsigned long status;

	/* Timer function; drops refcnt when it goes off. */
	struct timer_list timeout;

	/* If we're expecting another related connection, this will be
           in expected linked list */
	struct list_head sibling_list;

	/* Current number of expected connections */
	unsigned int expecting;

	/* If we were expected by an expectation, this will be it */
	struct ip_conntrack_expect *master;

	/* Helper, if any. */
	struct ip_conntrack_helper *helper;

	/* Our various nf_ct_info structs specify *what* relation this
           packet has to the conntrack */
	struct nf_ct_info infos[IP_CT_NUMBER];

	/* Storage reserved for other modules: */

	union {
		struct ip_ct_tcp tcp;
		struct ip_ct_icmp icmp;
		struct ip_ct_gre gre;
	} proto;

	union {
		/* insert conntrack helper private data (master) here */
		struct ip_ct_talk_master ct_talk_info;
		struct ip_ct_pptp_master ct_pptp_info;
		struct ip_ct_h225_master ct_h225_info;
		struct ip_ct_ftp_master ct_ftp_info;
		struct ip_ct_irc_master ct_irc_info;
	} help;

#ifdef CONFIG_IP_NF_NAT_NEEDED
	struct {
		struct ip_nat_info info;
		union {
			/* insert nat helper private data here */
			struct ip_nat_pptp nat_pptp_info;
		} help;
#if defined(CONFIG_IP_NF_TARGET_MASQUERADE) || \
	defined(CONFIG_IP_NF_TARGET_MASQUERADE_MODULE)
		int masq_index;
#endif
	} nat;
#endif /* CONFIG_IP_NF_NAT_NEEDED */

};

/* get master conntrack via master expectation */
#define master_ct(conntr) (conntr->master ? conntr->master->expectant : NULL)

/* Alter reply tuple (maybe alter helper).  If it's already taken,
   return 0 and don't do alteration. */
extern int
ip_conntrack_alter_reply(struct ip_conntrack *conntrack,
			 const struct ip_conntrack_tuple *newreply);

/* Is this tuple taken? (ignoring any belonging to the given
   conntrack). */
extern int
ip_conntrack_tuple_taken(const struct ip_conntrack_tuple *tuple,
			 const struct ip_conntrack *ignored_conntrack);

/* Return conntrack_info and tuple hash for given skb. */
extern struct ip_conntrack *
ip_conntrack_get(struct sk_buff *skb, enum ip_conntrack_info *ctinfo);

extern struct module *ip_conntrack_module;

extern int invert_tuplepr(struct ip_conntrack_tuple *inverse,
			  const struct ip_conntrack_tuple *orig);

/* Refresh conntrack for this many jiffies */
extern void ip_ct_refresh(struct ip_conntrack *ct,
			  unsigned long extra_jiffies);

/* These are for NAT.  Icky. */
/* Call me when a conntrack is destroyed. */
extern void (*ip_conntrack_destroyed)(struct ip_conntrack *conntrack);

/* Returns new sk_buff, or NULL */
struct sk_buff *
ip_ct_gather_frags(struct sk_buff *skb);

/* Delete all conntracks which match. */
extern void
ip_ct_selective_cleanup(int (*kill)(const struct ip_conntrack *i, void *data),
			void *data);

/* It's confirmed if it is, or has been in the hash table. */
static inline int is_confirmed(struct ip_conntrack *ct)
{
	return ct->tuplehash[IP_CT_DIR_ORIGINAL].list.next != NULL;
}

extern unsigned int ip_conntrack_htable_size;
#endif /* __KERNEL__ */
#endif /* _IP_CONNTRACK_H */
