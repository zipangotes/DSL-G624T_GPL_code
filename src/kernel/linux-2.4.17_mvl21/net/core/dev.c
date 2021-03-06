/*
 * 	NET3	Protocol independent device support routines.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *	Derived from the non IP parts of dev.c 1.0.19
 * 		Authors:	Ross Biro, <bir7@leland.Stanford.Edu>
 *				Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *				Mark Evans, <evansmp@uhura.aston.ac.uk>
 *
 *	Additional Authors:
 *		Florian la Roche <rzsfl@rz.uni-sb.de>
 *		Alan Cox <gw4pts@gw4pts.ampr.org>
 *		David Hinds <dhinds@allegro.stanford.edu>
 *		Alexey Kuznetsov <kuznet@ms2.inr.ac.ru>
 *		Adam Sulmicki <adam@cfar.umd.edu>
 *              Pekka Riikonen <priikone@poesidon.pspt.fi>
 *
 *	Changes:
 *              D.J. Barrow     :       Fixed bug where dev->refcnt gets set to 2
 *                                      if register_netdev gets called before
 *                                      net_dev_init & also removed a few lines
 *                                      of code in the process.
 *		Alan Cox	:	device private ioctl copies fields back.
 *		Alan Cox	:	Transmit queue code does relevant stunts to
 *					keep the queue safe.
 *		Alan Cox	:	Fixed double lock.
 *		Alan Cox	:	Fixed promisc NULL pointer trap
 *		????????	:	Support the full private ioctl range
 *		Alan Cox	:	Moved ioctl permission check into drivers
 *		Tim Kordas	:	SIOCADDMULTI/SIOCDELMULTI
 *		Alan Cox	:	100 backlog just doesn't cut it when
 *					you start doing multicast video 8)
 *		Alan Cox	:	Rewrote net_bh and list manager.
 *		Alan Cox	: 	Fix ETH_P_ALL echoback lengths.
 *		Alan Cox	:	Took out transmit every packet pass
 *					Saved a few bytes in the ioctl handler
 *		Alan Cox	:	Network driver sets packet type before calling netif_rx. Saves
 *					a function call a packet.
 *		Alan Cox	:	Hashed net_bh()
 *		Richard Kooijman:	Timestamp fixes.
 *		Alan Cox	:	Wrong field in SIOCGIFDSTADDR
 *		Alan Cox	:	Device lock protection.
 *		Alan Cox	: 	Fixed nasty side effect of device close changes.
 *		Rudi Cilibrasi	:	Pass the right thing to set_mac_address()
 *		Dave Miller	:	32bit quantity for the device lock to make it work out
 *					on a Sparc.
 *		Bjorn Ekwall	:	Added KERNELD hack.
 *		Alan Cox	:	Cleaned up the backlog initialise.
 *		Craig Metz	:	SIOCGIFCONF fix if space for under
 *					1 device.
 *	    Thomas Bogendoerfer :	Return ENODEV for dev_open, if there
 *					is no device open function.
 *		Andi Kleen	:	Fix error reporting for SIOCGIFCONF
 *	    Michael Chastain	:	Fix signed/unsigned for SIOCGIFCONF
 *		Cyrus Durgin	:	Cleaned for KMOD
 *		Adam Sulmicki   :	Bug Fix : Network Device Unload
 *					A network device unload needs to purge
 *					the backlog queue.
 *	Paul Rusty Russell	:	SIOCSIFNAME
 *              Pekka Riikonen  :	Netdev boot-time settings code
 *              Andrew Morton   :       Make unregister_netdevice wait indefinitely on dev->refcnt
 * 		J Hadi Salim	:	- Backlog queue sampling
 *				        - netif_rx() feedback	
 */

//-------------------------------------------------------------------------------------
// Copyright 2004, Texas Instruments Incorporated
//
// This program has been modified from its original operation by Texas Instruments
// to do the following:
// 
// 1. Performance optimizations
// 2. Voice-Data coexistence
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
//-------------------------------------------------------------------------------------

#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/bitops.h>
#include <linux/config.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/if_ether.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/notifier.h>
#include <linux/skbuff.h>
#include <linux/brlock.h>
#include <net/sock.h>
#include <linux/rtnetlink.h>
#include <linux/proc_fs.h>
#include <linux/stat.h>
#include <linux/if_bridge.h>
#include <linux/divert.h>
#include <net/dst.h>
#include <net/pkt_sched.h>
#include <net/profile.h>
#include <net/checksum.h>
#include <linux/highmem.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/module.h>
#if defined(CONFIG_NET_RADIO) || defined(CONFIG_NET_PCMCIA_RADIO)
#include <linux/wireless.h>		/* Note : will define WIRELESS_EXT */
#include <net/iw_handler.h>
#endif	/* CONFIG_NET_RADIO || CONFIG_NET_PCMCIA_RADIO */

#include <linux/trace.h>

#include <linux/udp.h>

/* PANKAJ: Added the variable which decides the mode in which the voice is
 * running. */
int gIsVoiceEnabled = 0;

static int voice_port_initialization (void);

#ifdef CONFIG_PLIP
extern int plip_init(void);
#endif


/* This define, if set, will randomly drop a packet when congestion
 * is more than moderate.  It helps fairness in the multi-interface
 * case when one of them is a hog, but it kills performance for the
 * single interface case so it is off now by default.
 */
#undef RAND_LIE

/* Setting this will sample the queue lengths and thus congestion
 * via a timer instead of as each packet is received.
 */
#undef OFFLINE_SAMPLE

NET_PROFILE_DEFINE(dev_queue_xmit)
NET_PROFILE_DEFINE(softnet_process)

const char *if_port_text[] = {
  "unknown",
  "BNC",
  "10baseT",
  "AUI",
  "100baseT",
  "100baseTX",
  "100baseFX"
};

/*
 *	The list of packet types we will receive (as opposed to discard)
 *	and the routines to invoke.
 *
 *	Why 16. Because with 16 the only overlap we get on a hash of the
 *	low nibble of the protocol value is RARP/SNAP/X.25.
 *
 *      NOTE:  That is no longer true with the addition of VLAN tags.  Not
 *             sure which should go first, but I bet it won't make much
 *             difference if we are running VLANs.  The good news is that
 *             this protocol won't be in the list unless compiled in, so
 *             the average user (w/out VLANs) will not be adversly affected.
 *             --BLG
 *
 *		0800	IP
 *		8100    802.1Q VLAN
 *		0001	802.3
 *		0002	AX.25
 *		0004	802.2
 *		8035	RARP
 *		0005	SNAP
 *		0805	X.25
 *		0806	ARP
 *		8137	IPX
 *		0009	Localtalk
 *		86DD	IPv6
 */

static struct packet_type *ptype_base[16];		/* 16 way hashed list */
static struct packet_type *ptype_all = NULL;		/* Taps */

#ifdef OFFLINE_SAMPLE
static void sample_queue(unsigned long dummy);
static struct timer_list samp_timer = { function: sample_queue };
#endif

#ifdef CONFIG_HOTPLUG
static int net_run_sbin_hotplug(struct net_device *dev, char *action);
#else
#define net_run_sbin_hotplug(dev, action) ({ 0; })
#endif

/*
 *	Our notifier list
 */
 
static struct notifier_block *netdev_chain=NULL;

/*
 *	Device drivers call our routines to queue packets here. We empty the
 *	queue in the local softnet handler.
 */
struct softnet_data softnet_data[NR_CPUS] __cacheline_aligned;

#ifdef CONFIG_NET_FASTROUTE
int netdev_fastroute;
int netdev_fastroute_obstacles;
#endif

#ifdef TI_SLOW_PATH
static inline int __dev_queue_xmit_orig(struct sk_buff *skb);
#endif

/******************************************************************************************

		Protocol management and registration routines

*******************************************************************************************/

/*
 *	For efficiency
 */

int netdev_nit=0;

/*
 *	Add a protocol ID to the list. Now that the input handler is
 *	smarter we can dispense with all the messy stuff that used to be
 *	here.
 *
 *	BEWARE!!! Protocol handlers, mangling input packets,
 *	MUST BE last in hash buckets and checking protocol handlers
 *	MUST start from promiscous ptype_all chain in net_bh.
 *	It is true now, do not change it.
 *	Explantion follows: if protocol handler, mangling packet, will
 *	be the first on list, it is not able to sense, that packet
 *	is cloned and should be copied-on-write, so that it will
 *	change it and subsequent readers will get broken packet.
 *							--ANK (980803)
 */

/**
 *	dev_add_pack - add packet handler
 *	@pt: packet type declaration
 * 
 *	Add a protocol handler to the networking stack. The passed &packet_type
 *	is linked into kernel lists and may not be freed until it has been
 *	removed from the kernel lists.
 */
 
void dev_add_pack(struct packet_type *pt)
{
	int hash;

	br_write_lock_bh(BR_NETPROTO_LOCK);

#ifdef CONFIG_NET_FASTROUTE
	/* Hack to detect packet socket */
	if (pt->data) {
		netdev_fastroute_obstacles++;
		dev_clear_fastroute(pt->dev);
	}
#endif
	if (pt->type == htons(ETH_P_ALL)) {
		netdev_nit++;
		pt->next=ptype_all;
		ptype_all=pt;
	} else {
		hash=ntohs(pt->type)&15;
		pt->next = ptype_base[hash];
		ptype_base[hash] = pt;
	}
	br_write_unlock_bh(BR_NETPROTO_LOCK);
}


/**
 *	dev_remove_pack	 - remove packet handler
 *	@pt: packet type declaration
 * 
 *	Remove a protocol handler that was previously added to the kernel
 *	protocol handlers by dev_add_pack(). The passed &packet_type is removed
 *	from the kernel lists and can be freed or reused once this function
 *	returns.
 */
 
void dev_remove_pack(struct packet_type *pt)
{
	struct packet_type **pt1;

	br_write_lock_bh(BR_NETPROTO_LOCK);

	if (pt->type == htons(ETH_P_ALL)) {
		netdev_nit--;
		pt1=&ptype_all;
	} else {
		pt1=&ptype_base[ntohs(pt->type)&15];
	}

	for (; (*pt1) != NULL; pt1 = &((*pt1)->next)) {
		if (pt == (*pt1)) {
			*pt1 = pt->next;
#ifdef CONFIG_NET_FASTROUTE
			if (pt->data)
				netdev_fastroute_obstacles--;
#endif
			br_write_unlock_bh(BR_NETPROTO_LOCK);
			return;
		}
	}
	br_write_unlock_bh(BR_NETPROTO_LOCK);
	printk(KERN_WARNING "dev_remove_pack: %p not found.\n", pt);
}

/******************************************************************************

		      Device Boot-time Settings Routines

*******************************************************************************/

/* Boot time configuration table */
static struct netdev_boot_setup dev_boot_setup[NETDEV_BOOT_SETUP_MAX];

/**
 *	netdev_boot_setup_add	- add new setup entry
 *	@name: name of the device
 *	@map: configured settings for the device
 *
 *	Adds new setup entry to the dev_boot_setup list.  The function
 *	returns 0 on error and 1 on success.  This is a generic routine to
 *	all netdevices.
 */
int netdev_boot_setup_add(char *name, struct ifmap *map)
{
	struct netdev_boot_setup *s;
	int i;

	s = dev_boot_setup;
	for (i = 0; i < NETDEV_BOOT_SETUP_MAX; i++) {
		if (s[i].name[0] == '\0' || s[i].name[0] == ' ') {
			memset(s[i].name, 0, sizeof(s[i].name));
			strcpy(s[i].name, name);
			memcpy(&s[i].map, map, sizeof(s[i].map));
			break;
		}
	}

	if (i >= NETDEV_BOOT_SETUP_MAX)
		return 0;

	return 1;
}

/**
 *	netdev_boot_setup_check	- check boot time settings
 *	@dev: the netdevice
 *
 * 	Check boot time settings for the device.
 *	The found settings are set for the device to be used
 *	later in the device probing.
 *	Returns 0 if no settings found, 1 if they are.
 */
int netdev_boot_setup_check(struct net_device *dev)
{
	struct netdev_boot_setup *s;
	int i;

	s = dev_boot_setup;
	for (i = 0; i < NETDEV_BOOT_SETUP_MAX; i++) {
		if (s[i].name[0] != '\0' && s[i].name[0] != ' ' &&
		    !strncmp(dev->name, s[i].name, strlen(s[i].name))) {
			dev->irq 	= s[i].map.irq;
			dev->base_addr 	= s[i].map.base_addr;
			dev->mem_start 	= s[i].map.mem_start;
			dev->mem_end 	= s[i].map.mem_end;
			return 1;
		}
	}
	return 0;
}

/*
 * Saves at boot time configured settings for any netdevice.
 */
int __init netdev_boot_setup(char *str)
{
	int ints[5];
	struct ifmap map;

	str = get_options(str, ARRAY_SIZE(ints), ints);
	if (!str || !*str)
		return 0;

	/* Save settings */
	memset(&map, 0, sizeof(map));
	if (ints[0] > 0)
		map.irq = ints[1];
	if (ints[0] > 1)
		map.base_addr = ints[2];
	if (ints[0] > 2)
		map.mem_start = ints[3];
	if (ints[0] > 3)
		map.mem_end = ints[4];

	/* Add new entry to the list */	
	return netdev_boot_setup_add(str, &map);
}

__setup("netdev=", netdev_boot_setup);

/*****************************************************************************************

			    Device Interface Subroutines

******************************************************************************************/

/**
 *	__dev_get_by_name	- find a device by its name 
 *	@name: name to find
 *
 *	Find an interface by name. Must be called under RTNL semaphore
 *	or @dev_base_lock. If the name is found a pointer to the device
 *	is returned. If the name is not found then %NULL is returned. The
 *	reference counters are not incremented so the caller must be
 *	careful with locks.
 */
 

struct net_device *__dev_get_by_name(const char *name)
{
	struct net_device *dev;

	for (dev = dev_base; dev != NULL; dev = dev->next) {
		if (strncmp(dev->name, name, IFNAMSIZ) == 0)
			return dev;
	}
	return NULL;
}

/**
 *	dev_get_by_name		- find a device by its name
 *	@name: name to find
 *
 *	Find an interface by name. This can be called from any 
 *	context and does its own locking. The returned handle has
 *	the usage count incremented and the caller must use dev_put() to
 *	release it when it is no longer needed. %NULL is returned if no
 *	matching device is found.
 */

struct net_device *dev_get_by_name(const char *name)
{
	struct net_device *dev;

	read_lock(&dev_base_lock);
	dev = __dev_get_by_name(name);
	if (dev)
		dev_hold(dev);
	read_unlock(&dev_base_lock);
	return dev;
}

/* 
   Return value is changed to int to prevent illegal usage in future.
   It is still legal to use to check for device existance.

   User should understand, that the result returned by this function
   is meaningless, if it was not issued under rtnl semaphore.
 */

/**
 *	dev_get	-	test if a device exists
 *	@name:	name to test for
 *
 *	Test if a name exists. Returns true if the name is found. In order
 *	to be sure the name is not allocated or removed during the test the
 *	caller must hold the rtnl semaphore.
 *
 *	This function primarily exists for back compatibility with older
 *	drivers. 
 */
 
int dev_get(const char *name)
{
	struct net_device *dev;

	read_lock(&dev_base_lock);
	dev = __dev_get_by_name(name);
	read_unlock(&dev_base_lock);
	return dev != NULL;
}

/**
 *	__dev_get_by_index - find a device by its ifindex
 *	@ifindex: index of device
 *
 *	Search for an interface by index. Returns %NULL if the device
 *	is not found or a pointer to the device. The device has not
 *	had its reference counter increased so the caller must be careful
 *	about locking. The caller must hold either the RTNL semaphore
 *	or @dev_base_lock.
 */

struct net_device * __dev_get_by_index(int ifindex)
{
	struct net_device *dev;

	for (dev = dev_base; dev != NULL; dev = dev->next) {
		if (dev->ifindex == ifindex)
			return dev;
	}
	return NULL;
}


/**
 *	dev_get_by_index - find a device by its ifindex
 *	@ifindex: index of device
 *
 *	Search for an interface by index. Returns NULL if the device
 *	is not found or a pointer to the device. The device returned has 
 *	had a reference added and the pointer is safe until the user calls
 *	dev_put to indicate they have finished with it.
 */

struct net_device * dev_get_by_index(int ifindex)
{
	struct net_device *dev;

	read_lock(&dev_base_lock);
	dev = __dev_get_by_index(ifindex);
	if (dev)
		dev_hold(dev);
	read_unlock(&dev_base_lock);
	return dev;
}

/**
 *	dev_getbyhwaddr - find a device by its hardware address
 *	@type: media type of device
 *	@ha: hardware address
 *
 *	Search for an interface by MAC address. Returns NULL if the device
 *	is not found or a pointer to the device. The caller must hold the
 *	rtnl semaphore. The returned device has not had its ref count increased
 *	and the caller must therefore be careful about locking
 *
 *	BUGS:
 *	If the API was consistent this would be __dev_get_by_hwaddr
 */

struct net_device *dev_getbyhwaddr(unsigned short type, char *ha)
{
	struct net_device *dev;

	ASSERT_RTNL();

	for (dev = dev_base; dev != NULL; dev = dev->next) {
		if (dev->type == type &&
		    memcmp(dev->dev_addr, ha, dev->addr_len) == 0)
			return dev;
	}
	return NULL;
}

/**
 *	dev_alloc_name - allocate a name for a device
 *	@dev: device 
 *	@name: name format string
 *
 *	Passed a format string - eg "lt%d" it will try and find a suitable
 *	id. Not efficient for many devices, not called a lot. The caller
 *	must hold the dev_base or rtnl lock while allocating the name and
 *	adding the device in order to avoid duplicates. Returns the number
 *	of the unit assigned or a negative errno code.
 */

int dev_alloc_name(struct net_device *dev, const char *name)
{
	int i;
	char buf[32];
	char *p;

	/*
	 * Verify the string as this thing may have come from
	 * the user.  There must be either one "%d" and no other "%"
	 * characters, or no "%" characters at all.
	 */
	p = strchr(name, '%');
	if (p && (p[1] != 'd' || strchr(p+2, '%')))
		return -EINVAL;

	/*
	 * If you need over 100 please also fix the algorithm...
	 */
	for (i = 0; i < 100; i++) {
		snprintf(buf,sizeof(buf),name,i);
		if (__dev_get_by_name(buf) == NULL) {
			strcpy(dev->name, buf);
			return i;
		}
	}
	return -ENFILE;	/* Over 100 of the things .. bail out! */
}

/**
 *	dev_alloc - allocate a network device and name
 *	@name: name format string
 *	@err: error return pointer
 *
 *	Passed a format string, eg. "lt%d", it will allocate a network device
 *	and space for the name. %NULL is returned if no memory is available.
 *	If the allocation succeeds then the name is assigned and the 
 *	device pointer returned. %NULL is returned if the name allocation
 *	failed. The cause of an error is returned as a negative errno code
 *	in the variable @err points to.
 *
 *	The caller must hold the @dev_base or RTNL locks when doing this in
 *	order to avoid duplicate name allocations.
 */

struct net_device *dev_alloc(const char *name, int *err)
{
	struct net_device *dev=kmalloc(sizeof(struct net_device), GFP_KERNEL);
	if (dev == NULL) {
		*err = -ENOBUFS;
		return NULL;
	}
	memset(dev, 0, sizeof(struct net_device));
	*err = dev_alloc_name(dev, name);
	if (*err < 0) {
		kfree(dev);
		return NULL;
	}
	return dev;
}

/**
 *	netdev_state_change - device changes state
 *	@dev: device to cause notification
 *
 *	Called to indicate a device has changed state. This function calls
 *	the notifier chains for netdev_chain and sends a NEWLINK message
 *	to the routing socket.
 */
 
void netdev_state_change(struct net_device *dev)
{
	if (dev->flags&IFF_UP) {
		notifier_call_chain(&netdev_chain, NETDEV_CHANGE, dev);
		rtmsg_ifinfo(RTM_NEWLINK, dev, 0);
	}
}


#ifdef CONFIG_KMOD

/**
 *	dev_load 	- load a network module
 *	@name: name of interface
 *
 *	If a network interface is not present and the process has suitable
 *	privileges this function loads the module. If module loading is not
 *	available in this kernel then it becomes a nop.
 */

void dev_load(const char *name)
{
	if (!dev_get(name) && capable(CAP_SYS_MODULE))
		request_module(name);
}

#else

extern inline void dev_load(const char *unused){;}

#endif

static int default_rebuild_header(struct sk_buff *skb)
{
	printk(KERN_DEBUG "%s: default_rebuild_header called -- BUG!\n", skb->dev ? skb->dev->name : "NULL!!!");
	kfree_skb(skb);
	return 1;
}

/**
 *	dev_open	- prepare an interface for use. 
 *	@dev:	device to open
 *
 *	Takes a device from down to up state. The device's private open
 *	function is invoked and then the multicast lists are loaded. Finally
 *	the device is moved into the up state and a %NETDEV_UP message is
 *	sent to the netdev notifier chain.
 *
 *	Calling this function on an active interface is a nop. On a failure
 *	a negative errno code is returned.
 */
 
int dev_open(struct net_device *dev)
{
	int ret = 0;

	/*
	 *	Is it already up?
	 */

	if (dev->flags&IFF_UP)
		return 0;

	/*
	 *	Is it even present?
	 */
	if (!netif_device_present(dev))
		return -ENODEV;

	/*
	 *	Call device private open method
	 */
	if (try_inc_mod_count(dev->owner)) {
		if (dev->open) {
			ret = dev->open(dev);
			if (ret != 0 && dev->owner)
				__MOD_DEC_USE_COUNT(dev->owner);
		}
	} else {
		ret = -ENODEV;
	}

	/*
	 *	If it went open OK then:
	 */
	 
	if (ret == 0) 
	{
		/*
		 *	Set the flags.
		 */
		dev->flags |= IFF_UP;

		set_bit(__LINK_STATE_START, &dev->state);

		/*
		 *	Initialize multicasting status 
		 */
		dev_mc_upload(dev);

		/*
		 *	Wakeup transmit queue engine
		 */
		dev_activate(dev);

		/*
		 *	... and announce new interface.
		 */
		notifier_call_chain(&netdev_chain, NETDEV_UP, dev);
	}
	return(ret);
}

#ifdef CONFIG_NET_FASTROUTE

static void dev_do_clear_fastroute(struct net_device *dev)
{
	if (dev->accept_fastpath) {
		int i;

		for (i=0; i<=NETDEV_FASTROUTE_HMASK; i++) {
			struct dst_entry *dst;

			write_lock_irq(&dev->fastpath_lock);
			dst = dev->fastpath[i];
			dev->fastpath[i] = NULL;
			write_unlock_irq(&dev->fastpath_lock);

			dst_release(dst);
		}
	}
}

void dev_clear_fastroute(struct net_device *dev)
{
	if (dev) {
		dev_do_clear_fastroute(dev);
	} else {
		read_lock(&dev_base_lock);
		for (dev = dev_base; dev; dev = dev->next)
			dev_do_clear_fastroute(dev);
		read_unlock(&dev_base_lock);
	}
}
#endif

/**
 *	dev_close - shutdown an interface.
 *	@dev: device to shutdown
 *
 *	This function moves an active device into down state. A 
 *	%NETDEV_GOING_DOWN is sent to the netdev notifier chain. The device
 *	is then deactivated and finally a %NETDEV_DOWN is sent to the notifier
 *	chain.
 */
 
int dev_close(struct net_device *dev)
{
	if (!(dev->flags&IFF_UP))
		return 0;

	/*
	 *	Tell people we are going down, so that they can
	 *	prepare to death, when device is still operating.
	 */
	notifier_call_chain(&netdev_chain, NETDEV_GOING_DOWN, dev);

	dev_deactivate(dev);

	clear_bit(__LINK_STATE_START, &dev->state);

	/*
	 *	Call the device specific close. This cannot fail.
	 *	Only if device is UP
	 *
	 *	We allow it to be called even after a DETACH hot-plug
	 *	event.
	 */
	 
	if (dev->stop)
		dev->stop(dev);

	/*
	 *	Device is now down.
	 */

	dev->flags &= ~IFF_UP;
#ifdef CONFIG_NET_FASTROUTE
	dev_clear_fastroute(dev);
#endif

	/*
	 *	Tell people we are down
	 */
	notifier_call_chain(&netdev_chain, NETDEV_DOWN, dev);

	/*
	 * Drop the module refcount
	 */
	if (dev->owner)
		__MOD_DEC_USE_COUNT(dev->owner);

	return(0);
}


/*
 *	Device change register/unregister. These are not inline or static
 *	as we export them to the world.
 */
 
/**
 *	register_netdevice_notifier - register a network notifier block
 *	@nb: notifier
 *
 *	Register a notifier to be called when network device events occur.
 *	The notifier passed is linked into the kernel structures and must
 *	not be reused until it has been unregistered. A negative errno code
 *	is returned on a failure.
 */

int register_netdevice_notifier(struct notifier_block *nb)
{
	return notifier_chain_register(&netdev_chain, nb);
}

/**
 *	unregister_netdevice_notifier - unregister a network notifier block
 *	@nb: notifier
 *
 *	Unregister a notifier previously registered by
 *	register_netdevice_notifier(). The notifier is unlinked into the
 *	kernel structures and may then be reused. A negative errno code
 *	is returned on a failure.
 */

int unregister_netdevice_notifier(struct notifier_block *nb)
{
	return notifier_chain_unregister(&netdev_chain,nb);
}

/*
 *	Support routine. Sends outgoing frames to any network
 *	taps currently in use.
 */

void dev_queue_xmit_nit(struct sk_buff *skb, struct net_device *dev)
{
	struct packet_type *ptype;
	get_fast_time(&skb->stamp);

	br_read_lock(BR_NETPROTO_LOCK);
	for (ptype = ptype_all; ptype!=NULL; ptype = ptype->next) 
	{
		/* Never send packets back to the socket
		 * they originated from - MvS (miquels@drinkel.ow.org)
		 */
		if ((ptype->dev == dev || !ptype->dev) &&
			((struct sock *)ptype->data != skb->sk))
		{
			struct sk_buff *skb2;
			if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL)
				break;

			/* skb->nh should be correctly
			   set by sender, so that the second statement is
			   just protection against buggy protocols.
			 */
			skb2->mac.raw = skb2->data;

			if (skb2->nh.raw < skb2->data || skb2->nh.raw > skb2->tail) {
				if (net_ratelimit())
					printk(KERN_DEBUG "protocol %04x is buggy, dev %s\n", skb2->protocol, dev->name);
				skb2->nh.raw = skb2->data;
			}

			skb2->h.raw = skb2->nh.raw;
			skb2->pkt_type = PACKET_OUTGOING;
			ptype->func(skb2, skb->dev, ptype);
		}
	}
	br_read_unlock(BR_NETPROTO_LOCK);
}

/* Calculate csum in the case, when packet is misrouted.
 * If it failed by some reason, ignore and send skb with wrong
 * checksum.
 */
struct sk_buff * skb_checksum_help(struct sk_buff *skb)
{
	int offset;
	unsigned int csum;

	offset = skb->h.raw - skb->data;
	if (offset > (int)skb->len)
		BUG();
	csum = skb_checksum(skb, offset, skb->len-offset, 0);

	offset = skb->tail - skb->h.raw;
	if (offset <= 0)
		BUG();
	if (skb->csum+2 > offset)
		BUG();

	*(u16*)(skb->h.raw + skb->csum) = csum_fold(csum);
	skb->ip_summed = CHECKSUM_NONE;
	return skb;
}

#ifdef CONFIG_HIGHMEM
/* Actually, we should eliminate this check as soon as we know, that:
 * 1. IOMMU is present and allows to map all the memory.
 * 2. No high memory really exists on this machine.
 */

static inline int
illegal_highdma(struct net_device *dev, struct sk_buff *skb)
{
	int i;

	if (dev->features&NETIF_F_HIGHDMA)
		return 0;

	for (i=0; i<skb_shinfo(skb)->nr_frags; i++)
		if (skb_shinfo(skb)->frags[i].page >= highmem_start_page)
			return 1;

	return 0;
}
#else
#define illegal_highdma(dev, skb)	(0)
#endif

/**
 *	dev_queue_xmit - transmit a buffer
 *	@skb: buffer to transmit
 *	
 *	Queue a buffer for transmission to a network device. The caller must
 *	have set the device and priority and built the buffer before calling this 
 *	function. The function can be called from an interrupt.
 *
 *	A negative errno code is returned on a failure. A success does not
 *	guarantee the frame will be transmitted as it may be dropped due
 *	to congestion or traffic shaping.
 */
#ifndef TI_SLOW_PATH
int dev_queue_xmit(struct sk_buff *skb)
{
	struct net_device*  dev = skb->dev;
	struct Qdisc*       q = dev->qdisc;
    int                 ret;

    /* Send the frame only if the device is up. */	
    if (dev->flags && IFF_UP)
    {
        /* If there is NO queueing discipline (ex. Loopback device) or if the driver
         * maintains its own queues and does not want OS support. Send the packet 
         * immediately. */
        if ((dev->tx_queue_len == 0) || (q->enqueue == NULL))
        {
            /* Send the frame immediately, only if the driver has not stopped the scheduling. */
			spin_lock(&dev->xmit_lock);
			if (!netif_queue_stopped(dev)) 
            {
				if (dev->hard_start_xmit(skb, dev) == 0) 
    	        {
        	        /* Packet succesfully transmitted. Unlock and return success. */
					spin_unlock(&dev->xmit_lock);
					return 0;
            	}
			}
 
            /* This will be the case when the driver is NOT able to send the packet. Clean
             * up the memory and release the lock. */
			spin_unlock(&dev->xmit_lock);
			kfree_skb(skb);
			return -ENETDOWN;
        }
        else
        {
            /* QOS Enabled or device wants us to handle queueing. Grab device queue */
        	spin_lock_bh(&dev->queue_lock);
      		ret = q->enqueue(skb, q);
            qdisc_run(dev);
        	spin_unlock_bh(&dev->queue_lock);
		    return ret == NET_XMIT_BYPASS ? NET_XMIT_SUCCESS : ret;
        }
    }
	kfree_skb(skb);
	return -ENETDOWN;
}
#else
int dev_queue_xmit(struct sk_buff *skb)
{
#if defined (CONFIG_MIPS_AVALANCHE_SOC)
	struct net_device *dev = skb->dev;
	struct Qdisc  *q = dev->qdisc;

        if( (dev->flags && IFF_UP) && !( (netif_queue_stopped(dev))       ||      
                                         (skb_shinfo(skb)->frag_list)     ||      
                                         (skb_shinfo(skb)->nr_frags)      ||      
                                         (skb->ip_summed == CHECKSUM_HW)  ||      
                                         (netdev_nit)                        ))
        {     
            if(!q->enqueue)    
            {                                                                    
                int cpu = smp_processor_id();
	        if(dev->xmit_lock_owner == cpu)
	            goto dev_queue_xmit_orig_lb;
      
                spin_lock(&dev->xmit_lock);
	        dev->xmit_lock_owner = cpu;
                if(dev->hard_start_xmit(skb,dev)== 0) 
                {     
	            dev->xmit_lock_owner = -1;
                    spin_unlock(&dev->xmit_lock);                        
                    return 0;
                }
	        dev->xmit_lock_owner = -1;
                spin_unlock(&dev->xmit_lock);                        
            }
            else
            {  
                int ret;
		int flags;
                spin_lock_bh(&dev->queue_lock);
		local_irq_save(flags);
                ret = q->enqueue(skb, q);
		local_irq_restore(flags);
		qdisc_run(dev);
		spin_unlock_bh(&dev->queue_lock);
		return ret == NET_XMIT_BYPASS ? NET_XMIT_SUCCESS : ret;
            }
                                                                         
	}       

dev_queue_xmit_orig_lb:
#endif
        return  __dev_queue_xmit_orig(skb);                                                  
}

static inline int __dev_queue_xmit_orig(struct sk_buff *skb)
{

	struct net_device *dev = skb->dev;
	struct Qdisc  *q;

	if (skb_shinfo(skb)->frag_list &&
	    !(dev->features&NETIF_F_FRAGLIST) &&
	    skb_linearize(skb, GFP_ATOMIC) != 0) {
		kfree_skb(skb);
		return -ENOMEM;
	}

	/* Fragmented skb is linearized if device does not support SG,
	 * or if at least one of fragments is in highmem and device
	 * does not support DMA from it.
	 */
	if (skb_shinfo(skb)->nr_frags &&
	    (!(dev->features&NETIF_F_SG) || illegal_highdma(dev, skb)) &&
	    skb_linearize(skb, GFP_ATOMIC) != 0) {
		kfree_skb(skb);
		return -ENOMEM;
	}

	/* If packet is not checksummed and device does not support
	 * checksumming for this protocol, complete checksumming here.
	 */
	if (skb->ip_summed == CHECKSUM_HW &&
	    (!(dev->features&(NETIF_F_HW_CSUM|NETIF_F_NO_CSUM)) &&
	     (!(dev->features&NETIF_F_IP_CSUM) ||
	      skb->protocol != htons(ETH_P_IP)))) {
		if ((skb = skb_checksum_help(skb)) == NULL)
			return -ENOMEM;
	}

	TRACE_NETWORK(TRACE_EV_NETWORK_PACKET_OUT, skb->protocol);

	/* Grab device queue */
	spin_lock_bh(&dev->queue_lock);
	q = dev->qdisc;
	if (q->enqueue) {
                int ret = q->enqueue(skb, q);
                //printk("enqueing a packet\n");
		qdisc_run(dev);

		spin_unlock_bh(&dev->queue_lock);
		return ret == NET_XMIT_BYPASS ? NET_XMIT_SUCCESS : ret;
	}

	/* The device has no queue. Common case for software devices:
	   loopback, all the sorts of tunnels...

	   Really, it is unlikely that xmit_lock protection is necessary here.
	   (f.e. loopback and IP tunnels are clean ignoring statistics counters.)
	   However, it is possible, that they rely on protection
	   made by us here.

	   Check this and shot the lock. It is not prone from deadlocks.
	   Either shot noqueue qdisc, it is even simpler 8)
	 */
	if (dev->flags&IFF_UP) {
		int cpu = smp_processor_id();

		if (dev->xmit_lock_owner != cpu) {
                        /*
                         * The spin_lock effectivly does a preempt lock, but 
                         * we are about to drop that...
                         */
                        preempt_disable();
			spin_unlock(&dev->queue_lock);
			spin_lock(&dev->xmit_lock);
			dev->xmit_lock_owner = cpu;
                        preempt_enable();

			if (!netif_queue_stopped(dev)) {
				if (netdev_nit)
				    dev_queue_xmit_nit(skb,dev);

				if (dev->hard_start_xmit(skb, dev) == 0) {
					dev->xmit_lock_owner = -1;
					spin_unlock_bh(&dev->xmit_lock);
					return 0;
				}
			}
			dev->xmit_lock_owner = -1;
			spin_unlock_bh(&dev->xmit_lock);
			if (net_ratelimit())
				printk(KERN_DEBUG "Virtual device %s asks to queue packet!\n", dev->name);
			kfree_skb(skb);
			return -ENETDOWN;
		} else {
			/* Recursion is detected! It is possible, unfortunately */
			if (net_ratelimit())
				printk(KERN_DEBUG "Dead loop on virtual device %s, fix it urgently!\n", dev->name);
		}
	}
	spin_unlock_bh(&dev->queue_lock);

	kfree_skb(skb);
	return -ENETDOWN;
}

#endif /* TI_SLOW_PATH

/*=======================================================================
			Receiver routines
  =======================================================================*/

int netdev_max_backlog = 300;
/* These numbers are selected based on intuition and some
 * experimentatiom, if you have more scientific way of doing this
 * please go ahead and fix things.
 */
int no_cong_thresh = 10;
int no_cong = 20;
int lo_cong = 100;
int mod_cong = 290;

struct netif_rx_stats netdev_rx_stat[NR_CPUS];


#ifdef CONFIG_NET_HW_FLOWCONTROL
atomic_t netdev_dropping = ATOMIC_INIT(0);
static unsigned long netdev_fc_mask = 1;
unsigned long netdev_fc_xoff = 0;
spinlock_t netdev_fc_lock = SPIN_LOCK_UNLOCKED;

static struct
{
	void (*stimul)(struct net_device *);
	struct net_device *dev;
} netdev_fc_slots[BITS_PER_LONG];

int netdev_register_fc(struct net_device *dev, void (*stimul)(struct net_device *dev))
{
int bit = 0;
	unsigned long flags;

	spin_lock_irqsave(&netdev_fc_lock, flags);
	if (netdev_fc_mask != ~0UL) {
		bit = ffz(netdev_fc_mask);
		netdev_fc_slots[bit].stimul = stimul;
		netdev_fc_slots[bit].dev = dev;
		set_bit(bit, &netdev_fc_mask);
		clear_bit(bit, &netdev_fc_xoff);
	}
	spin_unlock_irqrestore(&netdev_fc_lock, flags);
	return bit;
}

void netdev_unregister_fc(int bit)
{
	unsigned long flags;

	spin_lock_irqsave(&netdev_fc_lock, flags);
	if (bit > 0) {
		netdev_fc_slots[bit].stimul = NULL;
		netdev_fc_slots[bit].dev = NULL;
		clear_bit(bit, &netdev_fc_mask);
		clear_bit(bit, &netdev_fc_xoff);
	}
	spin_unlock_irqrestore(&netdev_fc_lock, flags);
}

static void netdev_wakeup(void)
{
	unsigned long xoff;

	spin_lock(&netdev_fc_lock);
	xoff = netdev_fc_xoff;
	netdev_fc_xoff = 0;
	while (xoff) {
		int i = ffz(~xoff);
		xoff &= ~(1<<i);
		netdev_fc_slots[i].stimul(netdev_fc_slots[i].dev);
	}
	spin_unlock(&netdev_fc_lock);
}
#endif

void get_sample_stats(int cpu)
{
#ifdef TI_SLOW_PATH

#ifdef RAND_LIE
	unsigned long rd;
	int rq;
#endif
	int blog = softnet_data[cpu].input_pkt_queue.qlen;
	int avg_blog = softnet_data[cpu].avg_blog;

	avg_blog = (avg_blog >> 1)+ (blog >> 1);

	if (avg_blog > mod_cong) {
		/* Above moderate congestion levels. */
		softnet_data[cpu].cng_level = NET_RX_CN_HIGH;
#ifdef RAND_LIE
		rd = net_random();
		rq = rd % netdev_max_backlog;
		if (rq < avg_blog) /* unlucky bastard */
			softnet_data[cpu].cng_level = NET_RX_DROP;
#endif
	} else if (avg_blog > lo_cong) {
		softnet_data[cpu].cng_level = NET_RX_CN_MOD;
#ifdef RAND_LIE
		rd = net_random();
		rq = rd % netdev_max_backlog;
			if (rq < avg_blog) /* unlucky bastard */
				softnet_data[cpu].cng_level = NET_RX_CN_HIGH;
#endif
	} else if (avg_blog > no_cong) 
		softnet_data[cpu].cng_level = NET_RX_CN_LOW;
	else  /* no congestion */
		softnet_data[cpu].cng_level = NET_RX_SUCCESS;

	softnet_data[cpu].avg_blog = avg_blog;
#else
    /* PANKAJ: Debugging voice-data coexistence. */
	int blog = softnet_data[cpu].input_pkt_queue.qlen;
	if (blog > mod_cong) 
    {
		/* High Congestion. */
		softnet_data[cpu].cng_level = NET_RX_CN_HIGH;
	} 
    else if (blog > lo_cong) 
    {
        /* Moderate congestion. */
		softnet_data[cpu].cng_level = NET_RX_CN_MOD;
	}
    else if (blog > no_cong)
    {
        /* Low congestion. */
		softnet_data[cpu].cng_level = NET_RX_CN_LOW;
    }
	else
    {  
        /* no congestion */
		softnet_data[cpu].cng_level = NET_RX_SUCCESS;
    }
#endif /* TI_SLOW_PATH */
}

#ifdef OFFLINE_SAMPLE
static void sample_queue(unsigned long dummy)
{
/* 10 ms 0r 1ms -- i dont care -- JHS */
/* run from timer interrupt, preemption is always off here */
	int next_tick = 1;
	int cpu = smp_processor_id();

	get_sample_stats(cpu);
	next_tick += jiffies;
	mod_timer(&samp_timer, next_tick);
}
#endif

/**
 *	netif_rx	-	post buffer to the network code
 *	@skb: buffer to post
 *
 *	This function receives a packet from a device driver and queues it for
 *	the upper (protocol) levels to process.  It always succeeds. The buffer
 *	may be dropped during processing for congestion control or by the 
 *	protocol layers.
 *      
 *	return values:
 *	NET_RX_SUCCESS	(no congestion)           
 *	NET_RX_CN_LOW     (low congestion) 
 *	NET_RX_CN_MOD     (moderate congestion)
 *	NET_RX_CN_HIGH    (high congestion) 
 *	NET_RX_DROP    (packet was dropped)
 *      
 *      
 */

int netif_rx(struct sk_buff *skb)
{
	int this_cpu = smp_processor_id();
	struct softnet_data *queue;
	unsigned long flags;

#ifndef TI_SLOW_PATH
    /* Check if the packet is meant for the bridge and if so pass the packet
     * to the bridge directly. There is no need to enqueue and then dequeue
     * the packet. */ 
    if ((skb->dev->br_port != NULL) && (br_handle_frame_hook != NULL))
    {
        /* Remember the physical port on which data came in */
		skb->__unused = skb->dev->if_port;
        br_handle_frame_hook(skb);

        /* PANKAJ: Debugging the voice-data coexistence. Return a snap shot of
         * the congestion level reported by the kernel. This can be used by
         * the drivers to stop pushing data up the stack. */
        return softnet_data[this_cpu].cng_level;
    }
#else
#if defined(CONFIG_MIPS_AVALANCHE_FAST_BRIDGE)
/* For now, we are basing the above config on CONFIG_BRIDGE_MODULE */

#ifdef CONFIG_NET_FASTROUTE
	if (skb->pkt_type == PACKET_FASTROUTE)
            ;
        else
#endif
        /* We have not accounted for the diverter as yet. */
        if (skb->dev->br_port != NULL && br_handle_frame_hook != NULL && !ptype_all) 
        {
#ifdef CONFIG_NET_CLS_U32
			/* Remember the physical port on which data came in */
			skb->__unused = skb->dev->if_port; 
#endif
            br_handle_frame_hook(skb);
            return (0);
        }
#endif
#endif /* TI_SLOW_PATH*/

	if (skb->stamp.tv_sec == 0)
		get_fast_time(&skb->stamp);

	/* The code is rearranged so that the path is the most
	   short when CPU is congested, but is still operating.
	 */
	queue = &softnet_data[this_cpu];

	local_irq_save(flags);

	netdev_rx_stat[this_cpu].total++;
	if (queue->input_pkt_queue.qlen <= netdev_max_backlog) {
		if (queue->input_pkt_queue.qlen) {
			if (queue->throttle)
				goto drop;

enqueue:
			dev_hold(skb->dev);
			__skb_queue_tail(&queue->input_pkt_queue,skb);
			/* Runs from irqs or BH's, no need to wake BH */
			cpu_raise_softirq(this_cpu, NET_RX_SOFTIRQ);

			local_irq_restore(flags);
#ifndef OFFLINE_SAMPLE
			get_sample_stats(this_cpu);
#endif
			return softnet_data[this_cpu].cng_level;
		}

		if (queue->throttle) {
			queue->throttle = 0;
#ifdef CONFIG_NET_HW_FLOWCONTROL
			if (atomic_dec_and_test(&netdev_dropping))
				netdev_wakeup();
#endif
		}
		goto enqueue;
	}

	if (queue->throttle == 0) {
		queue->throttle = 1;
		netdev_rx_stat[this_cpu].throttled++;
#ifdef CONFIG_NET_HW_FLOWCONTROL
		atomic_inc(&netdev_dropping);
#endif
	}

drop:
	netdev_rx_stat[this_cpu].dropped++;
	local_irq_restore(flags);

	kfree_skb(skb);
	return NET_RX_DROP;
}

/* Deliver skb to an old protocol, which is not threaded well
   or which do not understand shared skbs.
 */
static int deliver_to_old_ones(struct packet_type *pt, struct sk_buff *skb, int last)
{
	static spinlock_t net_bh_lock = SPIN_LOCK_UNLOCKED;
	int ret = NET_RX_DROP;


	if (!last) {
		skb = skb_clone(skb, GFP_ATOMIC);
		if (skb == NULL)
			return ret;
	}
	if (skb_is_nonlinear(skb) && skb_linearize(skb, GFP_ATOMIC) != 0) {
		kfree_skb(skb);
		return ret;
	}

	/* The assumption (correct one) is that old protocols
	   did not depened on BHs different of NET_BH and TIMER_BH.
	 */

	/* Emulate NET_BH with special spinlock */
	spin_lock(&net_bh_lock);

	/* Disable timers and wait for all timers completion */
	tasklet_disable(bh_task_vec+TIMER_BH);

	ret = pt->func(skb, skb->dev, pt);

	tasklet_hi_enable(bh_task_vec+TIMER_BH);
	spin_unlock(&net_bh_lock);
	return ret;
}

/* Reparent skb to master device. This function is called
 * only from net_rx_action under BR_NETPROTO_LOCK. It is misuse
 * of BR_NETPROTO_LOCK, but it is OK for now.
 */
static __inline__ void skb_bond(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	
	if (dev->master)
    {
        printk ("DEBUG: The dev %s has a master %s.\n", dev->name, dev->master->name);
		dev_hold(dev->master);
		skb->dev = dev->master;
		dev_put(dev);
	}
}

static void net_tx_action(struct softirq_action *h)
{
	int cpu = smp_processor_id();

	if (softnet_data[cpu].completion_queue) {
		struct sk_buff *clist;

		local_irq_disable();
		clist = softnet_data[cpu].completion_queue;
		softnet_data[cpu].completion_queue = NULL;
		local_irq_enable();

		while (clist != NULL) {
			struct sk_buff *skb = clist;
			clist = clist->next;

			BUG_TRAP(atomic_read(&skb->users) == 0);
			__kfree_skb(skb);
		}
	}

	if (softnet_data[cpu].output_queue) {
		struct net_device *head;

		local_irq_disable();
		head = softnet_data[cpu].output_queue;
		softnet_data[cpu].output_queue = NULL;
		local_irq_enable();

		while (head != NULL) {
			struct net_device *dev = head;
			head = head->next_sched;

			smp_mb__before_clear_bit();
			clear_bit(__LINK_STATE_SCHED, &dev->state);

			if (spin_trylock(&dev->queue_lock)) {
				qdisc_run(dev);
				spin_unlock(&dev->queue_lock);
			} else {
				netif_schedule(dev);
			}
		}
	}
}

/**
 *	net_call_rx_atomic
 *	@fn: function to call
 *
 *	Make a function call that is atomic with respect to the protocol
 *	layers.
 */
 
void net_call_rx_atomic(void (*fn)(void))
{
	br_write_lock_bh(BR_NETPROTO_LOCK);
	fn();
	br_write_unlock_bh(BR_NETPROTO_LOCK);
}

#if defined(CONFIG_BRIDGE) || defined(CONFIG_BRIDGE_MODULE)
void (*br_handle_frame_hook)(struct sk_buff *skb) = NULL;
#endif

static __inline__ int handle_bridge(struct sk_buff *skb,
				     struct packet_type *pt_prev)
{
	int ret = NET_RX_DROP;

	if (pt_prev) {
		if (!pt_prev->data)
			ret = deliver_to_old_ones(pt_prev, skb, 0);
		else {
			atomic_inc(&skb->users);
			ret = pt_prev->func(skb, skb->dev, pt_prev);
		}
	}

	br_handle_frame_hook(skb);
	return ret;
}


#ifdef CONFIG_NET_DIVERT
static inline void handle_diverter(struct sk_buff *skb)
{
	/* if diversion is supported on device, then divert */
	if (skb->dev->divert && skb->dev->divert->divert)
		divert_frame(skb);
}
#endif   /* CONFIG_NET_DIVERT */


#ifndef TI_SLOW_PATH

/**************************************************************************
 * FUNCTION NAME : net_rx_error_handler
 **************************************************************************
 * DESCRIPTION   :
 *  This function is called to clean packet memory on an error condition
 *  while processing a received packet. This was moved to a seperate function
 *  as this is the slow path. 
 ***************************************************************************/
static void net_rx_error_handler(struct sk_buff* skb)
{
    kfree_skb(skb);
    return;
}

/**************************************************************************
 * FUNCTION NAME : net_pass_data_handlers
 **************************************************************************
 * DESCRIPTION   :
 *  While optimizing the code, the logic to pass data to other handlers was
 *  removed. This was exposed while running the DHCP Client on the WAN 
 *  interface. The problem arises when the DHCP client opens another protocol
 *  listener through the function 'dev_add_pack'. The orignal code had to be
 *  put back but with some more optimizations so that we can still preserve
 *  the small fast path.
 *
 * NOTES         :
 *  There are 2 things missing from this implementation.
 *   1. No checking on the protocol type if the types fall in the same bucket.
 *   2. The dev passed through ptype is NOT used.
 *
 *  This is done on purpose. Add it when necessary.
***************************************************************************/
static void net_pass_data_other_handlers (struct packet_type* ptype,struct sk_buff* skb)
{
    struct packet_type *pt_prev = NULL;

    /* Cycle through all the registered protocol handlers. */
    while (ptype != NULL) 
    {
        /* Check if we have a previously remembered handler on which we need to 
         * send the packet. */
        if (pt_prev)
        {
            /* YES. Increment usage and pass packet to that handler. */
            atomic_inc(&skb->users);
			pt_prev->func(skb,skb->dev, pt_prev);
        }

        /* Remember the current protocol handler. */
        pt_prev = ptype;

        /* Jumpt to the next handler. */
        ptype = ptype->next;
    }

    /* Pass packet to the last handler. */
	if (pt_prev)
        pt_prev->func(skb, skb->dev, pt_prev);

    return;
}

/**************************************************************************
 * FUNCTION NAME : net_rx_action
 **************************************************************************
 * DESCRIPTION   :
 *  The network receive bottom half handler. The function dequeues the 
 *  network packets and passes them to the appropriate registered protocol
 *  handlers.
***************************************************************************/
static void net_rx_action(struct softirq_action *h)
{
    register struct sk_buff*    skb;
	struct softnet_data*        queue = &softnet_data[smp_processor_id()];
	unsigned long               start_time = jiffies;
	int                         bugdet = netdev_max_backlog;    
	struct net_device*          rx_dev;
    struct packet_type*         ptype;
    int                         softnet_break = 0;
    int                         drop_packet;

    /* Start Critical section. */
	br_read_lock(BR_NETPROTO_LOCK);

	for (;;) 
    {
        /* Disable interrupts and dequeue a packet from the NET queue. */
		local_irq_disable();
		skb = __skb_dequeue(&queue->input_pkt_queue);
		local_irq_enable();

        /* Check if there were any packets in the queue or not ? */
		if (skb != NULL)
        {
            /* YES. Process the dequeued packets. Save the device information on 
             * which the packet was received. */
    		rx_dev = skb->dev;

            /* Initialize the data pointers. */
    		skb->h.raw = skb->nh.raw = skb->data;
			
            /* Check if voice is enabled. */
            if (gIsVoiceEnabled == 1)
            {
                /* Voice is enabled. Check the current congestion level as reported by the System and
                 * start dropping data packets. */
#if 1
                if ((softnet_data[smp_processor_id()].cng_level >= NET_RX_CN_HIGH) && (skb->cb[46] != 0x1))
#else
                if ((softnet_data[smp_processor_id()].input_pkt_queue.qlen > mod_cong) && (skb->cb[46] != 0x1))
#endif
                    drop_packet = 1;
                else
                    drop_packet = 0;
            }
            else
            {
                /* Voice is NOT enabled. Don't drop any packets. Try and process as much data as we can. */
                drop_packet = 0;
            }

            /* Proceed only if we were not allowed to drop the packet. */
            if (drop_packet == 0)
            {
            /* Determine the protocol handler. */
            ptype=ptype_base[ntohs(skb->protocol)&15];
            if (ptype != NULL)
            {
                    /* Check if there is another registered handler for the same packet. */
                    if (ptype->next == NULL)
                    {
                        /* NO. Pass the packet to the handler */
	    		ptype->func(skb, skb->dev, ptype);
            }
            else
            {
                        /* YES. Push data to all registered protocol handlers. */
                        net_pass_data_other_handlers (ptype, skb);
                    }
                }
                else
                {
                /* Invalid packet as there is no registered handler. Clean memory
                 * and drop the packet. This is not the fast path so call the error 
                 * function to do the needful. Reducing the amount of code in the fast
                 * path. */
                net_rx_error_handler(skb);
            }
            }
            else
            {
                /* Drop the packet because of the congestion. */
                net_rx_error_handler(skb);
            }

            /* Release the receive device on which the packet was received. */
    		dev_put(rx_dev);

            /* Check if the NET RX BH has reached its execution limits. */
    		if ((bugdet-- < 0) || (jiffies - start_time) > 1)
            {
	    		softnet_break = 1;
                break;
            }
        }
        else
        {
            /* No more packets left, we are done. */
            break;
        }
	}

    /* Exit Criticial section. */
	br_read_unlock(BR_NETPROTO_LOCK);

    /* Disable Interrupts. */
    local_irq_disable();
    if (softnet_break == 0)
    {
        /* Check if the queue was being throttled and if so reset the flag. */
	    if (queue->throttle) 
    		queue->throttle = 0;
    }
    else
    {
        /* Net receive bottom half exceeded its time; increment the statistics. 
         * and reschedule the bottom half for execution again. */
	    netdev_rx_stat[smp_processor_id()].time_squeeze++;
	    cpu_raise_softirq(smp_processor_id(), NET_RX_SOFTIRQ);
    }

    /* Enable Interrupts. */
    local_irq_enable();

	NET_PROFILE_LEAVE(softnet_process);
	return;
}

#else

static void net_rx_action(struct softirq_action *h)
{
	int this_cpu = smp_processor_id();
	struct softnet_data *queue = &softnet_data[this_cpu];
	unsigned long start_time = jiffies;
	int bugdet = netdev_max_backlog;

	br_read_lock(BR_NETPROTO_LOCK);

	for (;;) {
		struct sk_buff *skb;
		struct net_device *rx_dev;

		local_irq_disable();
		skb = __skb_dequeue(&queue->input_pkt_queue);
		local_irq_enable();

		if (skb == NULL)
			break;

		skb_bond(skb);

		rx_dev = skb->dev;

		TRACE_NETWORK(TRACE_EV_NETWORK_PACKET_IN, skb->protocol);

#ifdef CONFIG_NET_FASTROUTE
		if (skb->pkt_type == PACKET_FASTROUTE) {
			netdev_rx_stat[this_cpu].fastroute_deferred_out++;
			dev_queue_xmit(skb);
			dev_put(rx_dev);
			continue;
		}
#endif
		skb->h.raw = skb->nh.raw = skb->data;
		{
			struct packet_type *ptype, *pt_prev;
			unsigned short type = skb->protocol;

			pt_prev = NULL;
			for (ptype = ptype_all; ptype; ptype = ptype->next) {
				if (!ptype->dev || ptype->dev == skb->dev) {
					if (pt_prev) {
						if (!pt_prev->data) {
							deliver_to_old_ones(pt_prev, skb, 0);
						} else {
							atomic_inc(&skb->users);
							pt_prev->func(skb,
								      skb->dev,
								      pt_prev);
						}
					}
					pt_prev = ptype;
				}
			}

#ifdef CONFIG_NET_DIVERT
			if (skb->dev->divert && skb->dev->divert->divert)
				handle_diverter(skb);
#endif /* CONFIG_NET_DIVERT */

			
#if defined(CONFIG_BRIDGE) || defined(CONFIG_BRIDGE_MODULE)
			if (skb->dev->br_port != NULL &&
			    br_handle_frame_hook != NULL) {
				handle_bridge(skb, pt_prev);
				dev_put(rx_dev);
				continue;
			}
#endif

			for (ptype=ptype_base[ntohs(type)&15];ptype;ptype=ptype->next) {
				if (ptype->type == type &&
				    (!ptype->dev || ptype->dev == skb->dev)) {
					if (pt_prev) {
						if (!pt_prev->data)
							deliver_to_old_ones(pt_prev, skb, 0);
						else {
							atomic_inc(&skb->users);
							pt_prev->func(skb,
								      skb->dev,
								      pt_prev);
						}
					}
					pt_prev = ptype;
				}
			}

			if (pt_prev) {
				if (!pt_prev->data)
					deliver_to_old_ones(pt_prev, skb, 1);
				else
					pt_prev->func(skb, skb->dev, pt_prev);
			} else
				kfree_skb(skb);
		}

		dev_put(rx_dev);

		if (bugdet-- < 0 || jiffies - start_time > 1)
			goto softnet_break;

#ifdef CONFIG_NET_HW_FLOWCONTROL
	if (queue->throttle && queue->input_pkt_queue.qlen < no_cong_thresh ) {
		if (atomic_dec_and_test(&netdev_dropping)) {
			queue->throttle = 0;
			netdev_wakeup();
			goto softnet_break;
		}
	}
#endif

	}
	br_read_unlock(BR_NETPROTO_LOCK);

	local_irq_disable();
	if (queue->throttle) {
		queue->throttle = 0;
#ifdef CONFIG_NET_HW_FLOWCONTROL
		if (atomic_dec_and_test(&netdev_dropping))
			netdev_wakeup();
#endif
	}
	local_irq_enable();

	NET_PROFILE_LEAVE(softnet_process);
	return;

softnet_break:
	br_read_unlock(BR_NETPROTO_LOCK);

	local_irq_disable();
	netdev_rx_stat[this_cpu].time_squeeze++;
	/* This already runs in BH context, no need to wake up BH's */
	cpu_raise_softirq(this_cpu, NET_RX_SOFTIRQ);

	local_irq_enable();

	NET_PROFILE_LEAVE(softnet_process);
	return;
}
#endif /* TI_SLOW_PATH */
static gifconf_func_t * gifconf_list [NPROTO];

/**
 *	register_gifconf	-	register a SIOCGIF handler
 *	@family: Address family
 *	@gifconf: Function handler
 *
 *	Register protocol dependent address dumping routines. The handler
 *	that is passed must not be freed or reused until it has been replaced
 *	by another handler.
 */
 
int register_gifconf(unsigned int family, gifconf_func_t * gifconf)
{
	if (family>=NPROTO)
		return -EINVAL;
	gifconf_list[family] = gifconf;
	return 0;
}


/*
 *	Map an interface index to its name (SIOCGIFNAME)
 */

/*
 *	We need this ioctl for efficient implementation of the
 *	if_indextoname() function required by the IPv6 API.  Without
 *	it, we would have to search all the interfaces to find a
 *	match.  --pb
 */

static int dev_ifname(struct ifreq *arg)
{
	struct net_device *dev;
	struct ifreq ifr;

	/*
	 *	Fetch the caller's info block. 
	 */
	
	if (copy_from_user(&ifr, arg, sizeof(struct ifreq)))
		return -EFAULT;

	read_lock(&dev_base_lock);
	dev = __dev_get_by_index(ifr.ifr_ifindex);
	if (!dev) {
		read_unlock(&dev_base_lock);
		return -ENODEV;
	}

	strcpy(ifr.ifr_name, dev->name);
	read_unlock(&dev_base_lock);

	if (copy_to_user(arg, &ifr, sizeof(struct ifreq)))
		return -EFAULT;
	return 0;
}

/*
 *	Perform a SIOCGIFCONF call. This structure will change
 *	size eventually, and there is nothing I can do about it.
 *	Thus we will need a 'compatibility mode'.
 */

static int dev_ifconf(char *arg)
{
	struct ifconf ifc;
	struct net_device *dev;
	char *pos;
	int len;
	int total;
	int i;

	/*
	 *	Fetch the caller's info block. 
	 */
	
	if (copy_from_user(&ifc, arg, sizeof(struct ifconf)))
		return -EFAULT;

	pos = ifc.ifc_buf;
	len = ifc.ifc_len;

	/*
	 *	Loop over the interfaces, and write an info block for each. 
	 */

	total = 0;
	for (dev = dev_base; dev != NULL; dev = dev->next) {
		for (i=0; i<NPROTO; i++) {
			if (gifconf_list[i]) {
				int done;
				if (pos==NULL) {
					done = gifconf_list[i](dev, NULL, 0);
				} else {
					done = gifconf_list[i](dev, pos+total, len-total);
				}
				if (done<0) {
					return -EFAULT;
				}
				total += done;
			}
		}
  	}

	/*
	 *	All done.  Write the updated control block back to the caller. 
	 */
	ifc.ifc_len = total;

	if (copy_to_user(arg, &ifc, sizeof(struct ifconf)))
		return -EFAULT; 

	/* 
	 * 	Both BSD and Solaris return 0 here, so we do too.
	 */
	return 0;
}

/*
 *	This is invoked by the /proc filesystem handler to display a device
 *	in detail.
 */

#ifdef CONFIG_PROC_FS

static int sprintf_stats(char *buffer, struct net_device *dev)
{
	struct net_device_stats *stats = (dev->get_stats ? dev->get_stats(dev): NULL);
	int size;
	
	if (stats)
		size = sprintf(buffer, "%6s:%8lu %7lu %4lu %4lu %4lu %5lu %10lu %9lu %8lu %7lu %4lu %4lu %4lu %5lu %7lu %10lu\n",
 		   dev->name,
		   stats->rx_bytes,
		   stats->rx_packets, stats->rx_errors,
		   stats->rx_dropped + stats->rx_missed_errors,
		   stats->rx_fifo_errors,
		   stats->rx_length_errors + stats->rx_over_errors
		   + stats->rx_crc_errors + stats->rx_frame_errors,
		   stats->rx_compressed, stats->multicast,
		   stats->tx_bytes,
		   stats->tx_packets, stats->tx_errors, stats->tx_dropped,
		   stats->tx_fifo_errors, stats->collisions,
		   stats->tx_carrier_errors + stats->tx_aborted_errors
		   + stats->tx_window_errors + stats->tx_heartbeat_errors,
		   stats->tx_compressed);
	else
		size = sprintf(buffer, "%6s: No statistics available.\n", dev->name);

	return size;
}

/*
 *	Called from the PROCfs module. This now uses the new arbitrary sized /proc/net interface
 *	to create /proc/net/dev
 */
 
static int dev_get_info(char *buffer, char **start, off_t offset, int length)
{
	int len = 0;
	off_t begin = 0;
	off_t pos = 0;
	int size;
	struct net_device *dev;


	size = sprintf(buffer, 
		"Inter-|   Receive                                                |  Transmit\n"
		" face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed\n");
	
	pos += size;
	len += size;
	

	read_lock(&dev_base_lock);
	for (dev = dev_base; dev != NULL; dev = dev->next) {
		size = sprintf_stats(buffer+len, dev);
		len += size;
		pos = begin + len;
				
		if (pos < offset) {
			len = 0;
			begin = pos;
		}
		if (pos > offset + length)
			break;
	}
	read_unlock(&dev_base_lock);

	*start = buffer + (offset - begin);	/* Start of wanted data */
	len -= (offset - begin);		/* Start slop */
	if (len > length)
		len = length;			/* Ending slop */
	if (len < 0)
		len = 0;
	return len;
}

static int dev_proc_stats(char *buffer, char **start, off_t offset,
			  int length, int *eof, void *data)
{
	int i, lcpu;
	int len=0;

	for (lcpu=0; lcpu<smp_num_cpus; lcpu++) {
		i = cpu_logical_map(lcpu);
		len += sprintf(buffer+len, "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n",
			       netdev_rx_stat[i].total,
			       netdev_rx_stat[i].dropped,
			       netdev_rx_stat[i].time_squeeze,
			       netdev_rx_stat[i].throttled,
			       netdev_rx_stat[i].fastroute_hit,
			       netdev_rx_stat[i].fastroute_success,
			       netdev_rx_stat[i].fastroute_defer,
			       netdev_rx_stat[i].fastroute_deferred_out,
#if 0
			       netdev_rx_stat[i].fastroute_latency_reduction
#else
			       netdev_rx_stat[i].cpu_collision,
#endif
                   softnet_data[0].input_pkt_queue.qlen
			       );
	}

	len -= offset;

	if (len > length)
		len = length;
	if (len < 0)
		len = 0;

	*start = buffer + offset;
	*eof = 1;

	return len;
}

#endif	/* CONFIG_PROC_FS */


#ifdef WIRELESS_EXT
#ifdef CONFIG_PROC_FS

/*
 * Print one entry of /proc/net/wireless
 * This is a clone of /proc/net/dev (just above)
 */
static int sprintf_wireless_stats(char *buffer, struct net_device *dev)
{
	/* Get stats from the driver */
	struct iw_statistics *stats = (dev->get_wireless_stats ?
				       dev->get_wireless_stats(dev) :
				       (struct iw_statistics *) NULL);
	int size;

	if (stats != (struct iw_statistics *) NULL) {
		size = sprintf(buffer,
			       "%6s: %04x  %3d%c  %3d%c  %3d%c  %6d %6d %6d %6d %6d   %6d\n",
			       dev->name,
			       stats->status,
			       stats->qual.qual,
			       stats->qual.updated & 1 ? '.' : ' ',
			       stats->qual.level,
			       stats->qual.updated & 2 ? '.' : ' ',
			       stats->qual.noise,
			       stats->qual.updated & 4 ? '.' : ' ',
			       stats->discard.nwid,
			       stats->discard.code,
			       stats->discard.fragment,
			       stats->discard.retries,
			       stats->discard.misc,
			       stats->miss.beacon);
		stats->qual.updated = 0;
	}
	else
		size = 0;

	return size;
}

/*
 * Print info for /proc/net/wireless (print all entries)
 * This is a clone of /proc/net/dev (just above)
 */
static int dev_get_wireless_info(char * buffer, char **start, off_t offset,
			  int length)
{
	int		len = 0;
	off_t		begin = 0;
	off_t		pos = 0;
	int		size;
	
	struct net_device *	dev;

	size = sprintf(buffer,
		       "Inter-| sta-|   Quality        |   Discarded packets               | Missed\n"
		       " face | tus | link level noise |  nwid  crypt   frag  retry   misc | beacon\n"
			);
	
	pos += size;
	len += size;

	read_lock(&dev_base_lock);
	for (dev = dev_base; dev != NULL; dev = dev->next) {
		size = sprintf_wireless_stats(buffer + len, dev);
		len += size;
		pos = begin + len;

		if (pos < offset) {
			len = 0;
			begin = pos;
		}
		if (pos > offset + length)
			break;
	}
	read_unlock(&dev_base_lock);

	*start = buffer + (offset - begin);	/* Start of wanted data */
	len -= (offset - begin);		/* Start slop */
	if (len > length)
		len = length;			/* Ending slop */
	if (len < 0)
		len = 0;

	return len;
}
#endif	/* CONFIG_PROC_FS */

/*
 *	Allow programatic access to /proc/net/wireless even if /proc
 *	doesn't exist... Also more efficient...
 */
static inline int dev_iwstats(struct net_device *dev, struct ifreq *ifr)
{
	/* Get stats from the driver */
	struct iw_statistics *stats = (dev->get_wireless_stats ?
				       dev->get_wireless_stats(dev) :
				       (struct iw_statistics *) NULL);

	if (stats != (struct iw_statistics *) NULL) {
		struct iwreq *	wrq = (struct iwreq *)ifr;

		/* Copy statistics to the user buffer */
		if(copy_to_user(wrq->u.data.pointer, stats,
				sizeof(struct iw_statistics)))
			return -EFAULT;

		/* Check if we need to clear the update flag */
		if(wrq->u.data.flags != 0)
			stats->qual.updated = 0;
		return(0);
	} else
		return -EOPNOTSUPP;
}
#endif	/* WIRELESS_EXT */

/**
 *	netdev_set_master	-	set up master/slave pair
 *	@slave: slave device
 *	@master: new master device
 *
 *	Changes the master device of the slave. Pass %NULL to break the
 *	bonding. The caller must hold the RTNL semaphore. On a failure
 *	a negative errno code is returned. On success the reference counts
 *	are adjusted, %RTM_NEWLINK is sent to the routing socket and the
 *	function returns zero.
 */
 
int netdev_set_master(struct net_device *slave, struct net_device *master)
{
	struct net_device *old = slave->master;

	ASSERT_RTNL();

    /* Debug message: NSP does not use the master/slave feature. This is a
     * debug message that will be printed on the console in case the feature
     * is used. In such a case we will need to fix the net_rx_bh as the
     * skb_bond was removed for optimizations. */
    printk ("NSP Error: Setting the device %s as a SLAVE to %s.\n", slave->name, master->name);

	if (master) {
		if (old)
			return -EBUSY;
		dev_hold(master);
	}

	br_write_lock_bh(BR_NETPROTO_LOCK);
	slave->master = master;
	br_write_unlock_bh(BR_NETPROTO_LOCK);

	if (old)
		dev_put(old);

	if (master)
		slave->flags |= IFF_SLAVE;
	else
		slave->flags &= ~IFF_SLAVE;

	rtmsg_ifinfo(RTM_NEWLINK, slave, IFF_SLAVE);
	return 0;
}

/**
 *	dev_set_promiscuity	- update promiscuity count on a device
 *	@dev: device
 *	@inc: modifier
 *
 *	Add or remove promsicuity from a device. While the count in the device
 *	remains above zero the interface remains promiscuous. Once it hits zero
 *	the device reverts back to normal filtering operation. A negative inc
 *	value is used to drop promiscuity on the device.
 */
 
void dev_set_promiscuity(struct net_device *dev, int inc)
{
	unsigned short old_flags = dev->flags;

	dev->flags |= IFF_PROMISC;
	if ((dev->promiscuity += inc) == 0)
		dev->flags &= ~IFF_PROMISC;
	if (dev->flags^old_flags) {
#ifdef CONFIG_NET_FASTROUTE
		if (dev->flags&IFF_PROMISC) {
			netdev_fastroute_obstacles++;
			dev_clear_fastroute(dev);
		} else
			netdev_fastroute_obstacles--;
#endif
		dev_mc_upload(dev);
		printk(KERN_INFO "device %s %s promiscuous mode\n",
		       dev->name, (dev->flags&IFF_PROMISC) ? "entered" : "left");
	}
}

/**
 *	dev_set_allmulti	- update allmulti count on a device
 *	@dev: device
 *	@inc: modifier
 *
 *	Add or remove reception of all multicast frames to a device. While the
 *	count in the device remains above zero the interface remains listening
 *	to all interfaces. Once it hits zero the device reverts back to normal
 *	filtering operation. A negative @inc value is used to drop the counter
 *	when releasing a resource needing all multicasts.
 */

void dev_set_allmulti(struct net_device *dev, int inc)
{
	unsigned short old_flags = dev->flags;

	dev->flags |= IFF_ALLMULTI;
	if ((dev->allmulti += inc) == 0)
		dev->flags &= ~IFF_ALLMULTI;
	if (dev->flags^old_flags)
		dev_mc_upload(dev);
}

int dev_change_flags(struct net_device *dev, unsigned flags)
{
	int ret;
	int old_flags = dev->flags;

	/*
	 *	Set the flags on our device.
	 */

	dev->flags = (flags & (IFF_DEBUG|IFF_NOTRAILERS|IFF_NOARP|IFF_DYNAMIC|
			       IFF_MULTICAST|IFF_PORTSEL|IFF_AUTOMEDIA)) |
				       (dev->flags & (IFF_UP|IFF_VOLATILE|IFF_PROMISC|IFF_ALLMULTI));

	/*
	 *	Load in the correct multicast list now the flags have changed.
	 */				

	dev_mc_upload(dev);

	/*
	 *	Have we downed the interface. We handle IFF_UP ourselves
	 *	according to user attempts to set it, rather than blindly
	 *	setting it.
	 */

	ret = 0;
	if ((old_flags^flags)&IFF_UP)	/* Bit is different  ? */
	{
		ret = ((old_flags & IFF_UP) ? dev_close : dev_open)(dev);

		if (ret == 0) 
			dev_mc_upload(dev);
	}

	if (dev->flags&IFF_UP &&
	    ((old_flags^dev->flags)&~(IFF_UP|IFF_PROMISC|IFF_ALLMULTI|IFF_VOLATILE)))
		notifier_call_chain(&netdev_chain, NETDEV_CHANGE, dev);

	if ((flags^dev->gflags)&IFF_PROMISC) {
		int inc = (flags&IFF_PROMISC) ? +1 : -1;
		dev->gflags ^= IFF_PROMISC;
		dev_set_promiscuity(dev, inc);
	}

	/* NOTE: order of synchronization of IFF_PROMISC and IFF_ALLMULTI
	   is important. Some (broken) drivers set IFF_PROMISC, when
	   IFF_ALLMULTI is requested not asking us and not reporting.
	 */
	if ((flags^dev->gflags)&IFF_ALLMULTI) {
		int inc = (flags&IFF_ALLMULTI) ? +1 : -1;
		dev->gflags ^= IFF_ALLMULTI;
		dev_set_allmulti(dev, inc);
	}

	if (old_flags^dev->flags)
		rtmsg_ifinfo(RTM_NEWLINK, dev, old_flags^dev->flags);

	return ret;
}

/*
 *	Perform the SIOCxIFxxx calls. 
 */
 
static int dev_ifsioc(struct ifreq *ifr, unsigned int cmd)
{
	struct net_device *dev;
	int err;

	if ((dev = __dev_get_by_name(ifr->ifr_name)) == NULL)
		return -ENODEV;

	switch(cmd) 
	{
		case SIOCGIFFLAGS:	/* Get interface flags */
			ifr->ifr_flags = (dev->flags&~(IFF_PROMISC|IFF_ALLMULTI|IFF_RUNNING))
				|(dev->gflags&(IFF_PROMISC|IFF_ALLMULTI));
			if (netif_running(dev) && netif_carrier_ok(dev))
				ifr->ifr_flags |= IFF_RUNNING;
			return 0;

		case SIOCSIFFLAGS:	/* Set interface flags */
			return dev_change_flags(dev, ifr->ifr_flags);
		
		case SIOCGIFMETRIC:	/* Get the metric on the interface (currently unused) */
			ifr->ifr_metric = 0;
			return 0;
			
		case SIOCSIFMETRIC:	/* Set the metric on the interface (currently unused) */
			return -EOPNOTSUPP;
	
		case SIOCGIFMTU:	/* Get the MTU of a device */
			ifr->ifr_mtu = dev->mtu;
			return 0;
	
		case SIOCSIFMTU:	/* Set the MTU of a device */
			if (ifr->ifr_mtu == dev->mtu)
				return 0;

			/*
			 *	MTU must be positive.
			 */
			 
			if (ifr->ifr_mtu<0)
				return -EINVAL;

			if (!netif_device_present(dev))
				return -ENODEV;

			if (dev->change_mtu)
				err = dev->change_mtu(dev, ifr->ifr_mtu);
			else {
				dev->mtu = ifr->ifr_mtu;
				err = 0;
			}
			if (!err && dev->flags&IFF_UP)
				notifier_call_chain(&netdev_chain, NETDEV_CHANGEMTU, dev);
			return err;

		case SIOCGIFHWADDR:
			memcpy(ifr->ifr_hwaddr.sa_data,dev->dev_addr, MAX_ADDR_LEN);
			ifr->ifr_hwaddr.sa_family=dev->type;
			return 0;
				
		case SIOCSIFHWADDR:
			if (dev->set_mac_address == NULL)
				return -EOPNOTSUPP;
			if (ifr->ifr_hwaddr.sa_family!=dev->type)
				return -EINVAL;
			if (!netif_device_present(dev))
				return -ENODEV;
			err = dev->set_mac_address(dev, &ifr->ifr_hwaddr);
			if (!err)
				notifier_call_chain(&netdev_chain, NETDEV_CHANGEADDR, dev);
			return err;
			
		case SIOCSIFHWBROADCAST:
			if (ifr->ifr_hwaddr.sa_family!=dev->type)
				return -EINVAL;
			memcpy(dev->broadcast, ifr->ifr_hwaddr.sa_data, MAX_ADDR_LEN);
			notifier_call_chain(&netdev_chain, NETDEV_CHANGEADDR, dev);
			return 0;

		case SIOCGIFMAP:
			ifr->ifr_map.mem_start=dev->mem_start;
			ifr->ifr_map.mem_end=dev->mem_end;
			ifr->ifr_map.base_addr=dev->base_addr;
			ifr->ifr_map.irq=dev->irq;
			ifr->ifr_map.dma=dev->dma;
			ifr->ifr_map.port=dev->if_port;
			return 0;
			
		case SIOCSIFMAP:
			if (dev->set_config) {
				if (!netif_device_present(dev))
					return -ENODEV;
				return dev->set_config(dev,&ifr->ifr_map);
			}
			return -EOPNOTSUPP;
			
		case SIOCADDMULTI:
			if (dev->set_multicast_list == NULL ||
			    ifr->ifr_hwaddr.sa_family != AF_UNSPEC)
				return -EINVAL;
			if (!netif_device_present(dev))
				return -ENODEV;
			dev_mc_add(dev,ifr->ifr_hwaddr.sa_data, dev->addr_len, 1);
			return 0;

		case SIOCDELMULTI:
			if (dev->set_multicast_list == NULL ||
			    ifr->ifr_hwaddr.sa_family!=AF_UNSPEC)
				return -EINVAL;
			if (!netif_device_present(dev))
				return -ENODEV;
			dev_mc_delete(dev,ifr->ifr_hwaddr.sa_data,dev->addr_len, 1);
			return 0;

		case SIOCGIFINDEX:
			ifr->ifr_ifindex = dev->ifindex;
			return 0;

		case SIOCGIFTXQLEN:
			ifr->ifr_qlen = dev->tx_queue_len;
			return 0;

		case SIOCSIFTXQLEN:
			if (ifr->ifr_qlen<0)
				return -EINVAL;
			dev->tx_queue_len = ifr->ifr_qlen;
			return 0;

		case SIOCSIFNAME:
			if (dev->flags&IFF_UP)
				return -EBUSY;
			if (__dev_get_by_name(ifr->ifr_newname))
				return -EEXIST;
			memcpy(dev->name, ifr->ifr_newname, IFNAMSIZ);
			dev->name[IFNAMSIZ-1] = 0;
			notifier_call_chain(&netdev_chain, NETDEV_CHANGENAME, dev);
			return 0;

#ifdef WIRELESS_EXT
		case SIOCGIWSTATS:
			return dev_iwstats(dev, ifr);
#endif	/* WIRELESS_EXT */

#ifdef CONFIG_NET_SCH_PRIOWRR
		case SIOCGIFASYMMTU:	/* Get the Asymmetric MTU of a device */
			ifr->ifr_mtu = dev->unused_alignment_fixer;
			return 0;
	
		case SIOCSIFASYMMTU:	/* Set the Asymmetric MTU of a device */
			printk("SIOCSIFASYMMTU\n");
			if (ifr->ifr_mtu == dev->unused_alignment_fixer)
				return 0;

			/*	MTU must be positive and should be less than MTU. */
			 
			if ((ifr->ifr_mtu<0) || (ifr->ifr_mtu>dev->mtu))
				return -EINVAL;

			if (!netif_device_present(dev))
				return -ENODEV;

			dev->unused_alignment_fixer = (unsigned short)ifr->ifr_mtu;
			err = 0;
			
			/*
			if (!err && dev->flags&IFF_UP)
				notifier_call_chain(&netdev_chain, NETDEV_CHANGEMTU, dev);
			*/
			return err;
#endif /* CONFIG_NET_SCH_PRIOWRR */

		/*
		 *	Unknown or private ioctl
		 */

		default:
			if ((cmd >= SIOCDEVPRIVATE &&
			    cmd <= SIOCDEVPRIVATE + 15) ||
			    cmd == SIOCBONDENSLAVE ||
			    cmd == SIOCBONDRELEASE ||
			    cmd == SIOCBONDSETHWADDR ||
			    cmd == SIOCBONDSLAVEINFOQUERY ||
			    cmd == SIOCBONDINFOQUERY ||
			    cmd == SIOCBONDCHANGEACTIVE ||
			    cmd == SIOCETHTOOL ||
			    cmd == SIOCGMIIPHY ||
			    cmd == SIOCGMIIREG ||
			    cmd == SIOCSMIIREG) {
				if (dev->do_ioctl) {
					if (!netif_device_present(dev))
						return -ENODEV;
					return dev->do_ioctl(dev, ifr, cmd);
				}
				return -EOPNOTSUPP;
			}

#ifdef WIRELESS_EXT
			if (cmd >= SIOCIWFIRST && cmd <= SIOCIWLAST) {
				if (dev->do_ioctl) {
					if (!netif_device_present(dev))
						return -ENODEV;
					return dev->do_ioctl(dev, ifr, cmd);
				}
				return -EOPNOTSUPP;
			}
#endif	/* WIRELESS_EXT */

	}
	return -EINVAL;
}

/*
 *	This function handles all "interface"-type I/O control requests. The actual
 *	'doing' part of this is dev_ifsioc above.
 */

/**
 *	dev_ioctl	-	network device ioctl
 *	@cmd: command to issue
 *	@arg: pointer to a struct ifreq in user space
 *
 *	Issue ioctl functions to devices. This is normally called by the
 *	user space syscall interfaces but can sometimes be useful for 
 *	other purposes. The return value is the return from the syscall if
 *	positive or a negative errno code on error.
 */

int dev_ioctl(unsigned int cmd, void *arg)
{
	struct ifreq ifr;
	int ret;
	char *colon;

	/* One special case: SIOCGIFCONF takes ifconf argument
	   and requires shared lock, because it sleeps writing
	   to user space.
	 */
	   
	if (cmd == SIOCGIFCONF) {
		rtnl_shlock();
		ret = dev_ifconf((char *) arg);
		rtnl_shunlock();
		return ret;
	}
	if (cmd == SIOCGIFNAME) {
		return dev_ifname((struct ifreq *)arg);
	}

	if (copy_from_user(&ifr, arg, sizeof(struct ifreq)))
		return -EFAULT;

	ifr.ifr_name[IFNAMSIZ-1] = 0;

	colon = strchr(ifr.ifr_name, ':');
	if (colon)
		*colon = 0;

	/*
	 *	See which interface the caller is talking about. 
	 */
	 
	switch(cmd) 
	{
		/*
		 *	These ioctl calls:
		 *	- can be done by all.
		 *	- atomic and do not require locking.
		 *	- return a value
		 */
		 
		case SIOCGIFFLAGS:
		case SIOCGIFMETRIC:
		case SIOCGIFMTU:
		case SIOCGIFHWADDR:
		case SIOCGIFSLAVE:
		case SIOCGIFMAP:
		case SIOCGIFINDEX:
		case SIOCGIFTXQLEN:
#ifdef CONFIG_NET_SCH_PRIOWRR
		case SIOCGIFASYMMTU:
#endif
			dev_load(ifr.ifr_name);
			read_lock(&dev_base_lock);
			ret = dev_ifsioc(&ifr, cmd);
			read_unlock(&dev_base_lock);
			if (!ret) {
				if (colon)
					*colon = ':';
				if (copy_to_user(arg, &ifr, sizeof(struct ifreq)))
					return -EFAULT;
			}
			return ret;

		/*
		 *	These ioctl calls:
		 *	- require superuser power.
		 *	- require strict serialization.
		 *	- return a value
		 */
		 
		case SIOCETHTOOL:
		case SIOCGMIIPHY:
		case SIOCGMIIREG:
			if (!capable(CAP_NET_ADMIN))
				return -EPERM;
			dev_load(ifr.ifr_name);
			dev_probe_lock();
			rtnl_lock();
			ret = dev_ifsioc(&ifr, cmd);
			rtnl_unlock();
			dev_probe_unlock();
			if (!ret) {
				if (colon)
					*colon = ':';
				if (copy_to_user(arg, &ifr, sizeof(struct ifreq)))
					return -EFAULT;
			}
			return ret;

		/*
		 *	These ioctl calls:
		 *	- require superuser power.
		 *	- require strict serialization.
		 *	- do not return a value
		 */
		 
		case SIOCSIFFLAGS:
		case SIOCSIFMETRIC:
		case SIOCSIFMTU:
		case SIOCSIFMAP:
		case SIOCSIFHWADDR:
		case SIOCSIFSLAVE:
		case SIOCADDMULTI:
		case SIOCDELMULTI:
		case SIOCSIFHWBROADCAST:
		case SIOCSIFTXQLEN:
		case SIOCSIFNAME:
		case SIOCSMIIREG:
		case SIOCBONDENSLAVE:
		case SIOCBONDRELEASE:
		case SIOCBONDSETHWADDR:
		case SIOCBONDSLAVEINFOQUERY:
		case SIOCBONDINFOQUERY:
		case SIOCBONDCHANGEACTIVE:
#ifdef CONFIG_NET_SCH_PRIOWRR
		case SIOCSIFASYMMTU:
#endif
			if (!capable(CAP_NET_ADMIN))
				return -EPERM;
			dev_load(ifr.ifr_name);
			dev_probe_lock();
			rtnl_lock();
			ret = dev_ifsioc(&ifr, cmd);
			rtnl_unlock();
			dev_probe_unlock();
			return ret;
	
		case SIOCGIFMEM:
			/* Get the per device memory space. We can add this but currently
			   do not support it */
		case SIOCSIFMEM:
			/* Set the per device memory buffer space. Not applicable in our case */
		case SIOCSIFLINK:
			return -EINVAL;

		/*
		 *	Unknown or private ioctl.
		 */	
		 
		default:
			if (cmd >= SIOCDEVPRIVATE &&
			    cmd <= SIOCDEVPRIVATE + 15) {
				dev_load(ifr.ifr_name);
				dev_probe_lock();
				rtnl_lock();
				ret = dev_ifsioc(&ifr, cmd);
				rtnl_unlock();
				dev_probe_unlock();
				if (!ret && copy_to_user(arg, &ifr, sizeof(struct ifreq)))
					return -EFAULT;
				return ret;
			}
#ifdef WIRELESS_EXT
			/* Take care of Wireless Extensions */
			if (cmd >= SIOCIWFIRST && cmd <= SIOCIWLAST) {
				/* If command is `set a parameter', or
				 * `get the encoding parameters', check if
				 * the user has the right to do it */
				if (IW_IS_SET(cmd) || (cmd == SIOCGIWENCODE)) {
					if(!capable(CAP_NET_ADMIN))
						return -EPERM;
				}
				dev_load(ifr.ifr_name);
				rtnl_lock();
				/* Follow me in net/core/wireless.c */
				ret = wireless_process_ioctl(&ifr, cmd);
				rtnl_unlock();
				if (!ret && IW_IS_GET(cmd) &&
				    copy_to_user(arg, &ifr, sizeof(struct ifreq)))
					return -EFAULT;
				return ret;
			}
#endif	/* WIRELESS_EXT */
			return -EINVAL;
	}
}


/**
 *	dev_new_index	-	allocate an ifindex
 *
 *	Returns a suitable unique value for a new device interface
 *	number.  The caller must hold the rtnl semaphore or the
 *	dev_base_lock to be sure it remains unique.
 */
 
int dev_new_index(void)
{
	static int ifindex;
	for (;;) {
		if (++ifindex <= 0)
			ifindex=1;
		if (__dev_get_by_index(ifindex) == NULL)
			return ifindex;
	}
}

static int dev_boot_phase = 1;

/**
 *	register_netdevice	- register a network device
 *	@dev: device to register
 *	
 *	Take a completed network device structure and add it to the kernel
 *	interfaces. A %NETDEV_REGISTER message is sent to the netdev notifier
 *	chain. 0 is returned on success. A negative errno code is returned
 *	on a failure to set up the device, or if the name is a duplicate.
 *
 *	Callers must hold the rtnl semaphore.  See the comment at the
 *	end of Space.c for details about the locking.  You may want
 *	register_netdev() instead of this.
 *
 *	BUGS:
 *	The locking appears insufficient to guarantee two parallel registers
 *	will not get the same name.
 */

int net_dev_init(void);

int register_netdevice(struct net_device *dev)
{
	struct net_device *d, **dp;
#ifdef CONFIG_NET_DIVERT
	int ret;
#endif

	spin_lock_init(&dev->queue_lock);
	spin_lock_init(&dev->xmit_lock);
	dev->xmit_lock_owner = -1;
#ifdef CONFIG_NET_FASTROUTE
	dev->fastpath_lock=RW_LOCK_UNLOCKED;
#endif

	if (dev_boot_phase)
		net_dev_init();

#ifdef CONFIG_NET_DIVERT
	ret = alloc_divert_blk(dev);
	if (ret)
		return ret;
#endif /* CONFIG_NET_DIVERT */
	
	dev->iflink = -1;

	/* Init, if this function is available */
	if (dev->init && dev->init(dev) != 0) {
#ifdef CONFIG_NET_DIVERT
		free_divert_blk(dev);
#endif
		return -EIO;
	}

#ifdef CONFIG_NET_SCH_PRIOWRR
	dev->unused_alignment_fixer = (unsigned short) dev->mtu;
	printk("Default Asymmetric MTU for %s %d\n", dev->name, dev->unused_alignment_fixer);
#endif /* CONFIG_NET_SCH_PRIOWRR */

	dev->ifindex = dev_new_index();
	if (dev->iflink == -1)
		dev->iflink = dev->ifindex;

	/* Check for existence, and append to tail of chain */
	for (dp=&dev_base; (d=*dp) != NULL; dp=&d->next) {
		if (d == dev || strcmp(d->name, dev->name) == 0) {
#ifdef CONFIG_NET_DIVERT
			free_divert_blk(dev);
#endif
			return -EEXIST;
		}
	}
	/*
	 *	nil rebuild_header routine,
	 *	that should be never called and used as just bug trap.
	 */

	if (dev->rebuild_header == NULL)
		dev->rebuild_header = default_rebuild_header;

	/*
	 *	Default initial state at registry is that the
	 *	device is present.
	 */

	set_bit(__LINK_STATE_PRESENT, &dev->state);

	dev->next = NULL;
	dev_init_scheduler(dev);
	write_lock_bh(&dev_base_lock);
	*dp = dev;
	dev_hold(dev);
	dev->deadbeaf = 0;
	write_unlock_bh(&dev_base_lock);

	/* Notify protocols, that a new device appeared. */
	notifier_call_chain(&netdev_chain, NETDEV_REGISTER, dev);

	net_run_sbin_hotplug(dev, "register");

	return 0;
}

/**
 *	netdev_finish_unregister - complete unregistration
 *	@dev: device
 *
 *	Destroy and free a dead device. A value of zero is returned on
 *	success.
 */
 
int netdev_finish_unregister(struct net_device *dev)
{
	BUG_TRAP(dev->ip_ptr==NULL);
	BUG_TRAP(dev->ip6_ptr==NULL);
	BUG_TRAP(dev->dn_ptr==NULL);

	if (!dev->deadbeaf) {
		printk(KERN_ERR "Freeing alive device %p, %s\n", dev, dev->name);
		return 0;
	}
#ifdef NET_REFCNT_DEBUG
	printk(KERN_DEBUG "netdev_finish_unregister: %s%s.\n", dev->name,
	       (dev->features & NETIF_F_DYNALLOC)?"":", old style");
#endif
	if (dev->destructor)
		dev->destructor(dev);
	if (dev->features & NETIF_F_DYNALLOC)
		kfree(dev);
	return 0;
}

/**
 *	unregister_netdevice - remove device from the kernel
 *	@dev: device
 *
 *	This function shuts down a device interface and removes it
 *	from the kernel tables. On success 0 is returned, on a failure
 *	a negative errno code is returned.
 *
 *	Callers must hold the rtnl semaphore.  See the comment at the
 *	end of Space.c for details about the locking.  You may want
 *	unregister_netdev() instead of this.
 */

int unregister_netdevice(struct net_device *dev)
{
	unsigned long now, warning_time;
	struct net_device *d, **dp;

	/* If device is running, close it first. */
	if (dev->flags & IFF_UP)
		dev_close(dev);

	BUG_TRAP(dev->deadbeaf==0);
	dev->deadbeaf = 1;

	/* And unlink it from device chain. */
	for (dp = &dev_base; (d=*dp) != NULL; dp=&d->next) {
		if (d == dev) {
			write_lock_bh(&dev_base_lock);
			*dp = d->next;
			write_unlock_bh(&dev_base_lock);
			break;
		}
	}
	if (d == NULL) {
		printk(KERN_DEBUG "unregister_netdevice: device %s/%p never was registered\n", dev->name, dev);
		return -ENODEV;
	}

	/* Synchronize to net_rx_action. */
	br_write_lock_bh(BR_NETPROTO_LOCK);
	br_write_unlock_bh(BR_NETPROTO_LOCK);

	if (dev_boot_phase == 0) {
#ifdef CONFIG_NET_FASTROUTE
		dev_clear_fastroute(dev);
#endif

		/* Shutdown queueing discipline. */
		dev_shutdown(dev);

		net_run_sbin_hotplug(dev, "unregister");

		/* Notify protocols, that we are about to destroy
		   this device. They should clean all the things.
		 */
		notifier_call_chain(&netdev_chain, NETDEV_UNREGISTER, dev);

		/*
		 *	Flush the multicast chain
		 */
		dev_mc_discard(dev);
	}

	if (dev->uninit)
		dev->uninit(dev);

	/* Notifier chain MUST detach us from master device. */
	BUG_TRAP(dev->master==NULL);

#ifdef CONFIG_NET_DIVERT
	free_divert_blk(dev);
#endif

	if (dev->features & NETIF_F_DYNALLOC) {
#ifdef NET_REFCNT_DEBUG
		if (atomic_read(&dev->refcnt) != 1)
			printk(KERN_DEBUG "unregister_netdevice: holding %s refcnt=%d\n", dev->name, atomic_read(&dev->refcnt)-1);
#endif
		dev_put(dev);
		return 0;
	}

	/* Last reference is our one */
	if (atomic_read(&dev->refcnt) == 1) {
		dev_put(dev);
		return 0;
	}

#ifdef NET_REFCNT_DEBUG
	printk("unregister_netdevice: waiting %s refcnt=%d\n", dev->name, atomic_read(&dev->refcnt));
#endif

	/* EXPLANATION. If dev->refcnt is not now 1 (our own reference)
	   it means that someone in the kernel still has a reference
	   to this device and we cannot release it.

	   "New style" devices have destructors, hence we can return from this
	   function and destructor will do all the work later.  As of kernel 2.4.0
	   there are very few "New Style" devices.

	   "Old style" devices expect that the device is free of any references
	   upon exit from this function.
	   We cannot return from this function until all such references have
	   fallen away.  This is because the caller of this function will probably
	   immediately kfree(*dev) and then be unloaded via sys_delete_module.

	   So, we linger until all references fall away.  The duration of the
	   linger is basically unbounded! It is driven by, for example, the
	   current setting of sysctl_ipfrag_time.

	   After 1 second, we start to rebroadcast unregister notifications
	   in hope that careless clients will release the device.

	 */

	now = warning_time = jiffies;
	while (atomic_read(&dev->refcnt) != 1) {
		if ((jiffies - now) > 1*HZ) {
			/* Rebroadcast unregister notification */
			notifier_call_chain(&netdev_chain, NETDEV_UNREGISTER, dev);
		}
		current->state = TASK_INTERRUPTIBLE;
		schedule_timeout(HZ/4);
		current->state = TASK_RUNNING;
		if ((jiffies - warning_time) > 10*HZ) {
			printk(KERN_EMERG "unregister_netdevice: waiting for %s to "
					"become free. Usage count = %d\n",
					dev->name, atomic_read(&dev->refcnt));
			warning_time = jiffies;
		}
	}
	dev_put(dev);
	return 0;
}


/*
 *	Initialize the DEV module. At boot time this walks the device list and
 *	unhooks any devices that fail to initialise (normally hardware not 
 *	present) and leaves us with a valid list of present and active devices.
 *
 */

extern void net_device_init(void);
extern void ip_auto_config(void);
#ifdef CONFIG_NET_DIVERT
extern void dv_init(void);
#endif /* CONFIG_NET_DIVERT */

static int voice_write_level (struct file *file, const char *buffer, unsigned long count, void *data) 
{
	char voicestate[5];

	/* Validate the length of data passed. */
	if (count > 5)
		count = 5; 
	
	/* Copy from user space. */	
	if (copy_from_user (&voicestate, buffer, count))
		return -EFAULT;

	/* Store the new debug level. */
	gIsVoiceEnabled = (unsigned short) simple_strtol(voicestate, NULL, 0);
	return count;
}

static int voice_read_level (char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
	int len = 0;
	len = len + sprintf (buf + len, "Currently Voice Mode is %d.\n",gIsVoiceEnabled); 
	return len;
}

/*
 *       Callers must hold the rtnl semaphore.  See the comment at the
 *       end of Space.c for details about the locking.
 */
int __init net_dev_init(void)
{
	struct net_device *dev, **dp;
	int i;
    struct proc_dir_entry*	ptr_dir_entry;

	if (!dev_boot_phase)
		return 0;

#ifdef CONFIG_NET_DIVERT
	dv_init();
#endif /* CONFIG_NET_DIVERT */
	
	/*
	 *	Initialise the packet receive queues.
	 */

	for (i = 0; i < NR_CPUS; i++) {
		struct softnet_data *queue;

		queue = &softnet_data[i];
		skb_queue_head_init(&queue->input_pkt_queue);
		queue->throttle = 0;
		queue->cng_level = 0;
		queue->avg_blog = 10; /* arbitrary non-zero */
		queue->completion_queue = NULL;
	}
	
#ifdef CONFIG_NET_PROFILE
	net_profile_init();
	NET_PROFILE_REGISTER(dev_queue_xmit);
	NET_PROFILE_REGISTER(softnet_process);
#endif

#ifdef OFFLINE_SAMPLE
	samp_timer.expires = jiffies + (10 * HZ);
	add_timer(&samp_timer);
#endif

	/*
	 *	Add the devices.
	 *	If the call to dev->init fails, the dev is removed
	 *	from the chain disconnecting the device until the
	 *	next reboot.
	 *
	 *	NB At boot phase networking is dead. No locking is required.
	 *	But we still preserve dev_base_lock for sanity.
	 */

	dp = &dev_base;
	while ((dev = *dp) != NULL) {
		spin_lock_init(&dev->queue_lock);
		spin_lock_init(&dev->xmit_lock);
#ifdef CONFIG_NET_FASTROUTE
		dev->fastpath_lock = RW_LOCK_UNLOCKED;
#endif
		dev->xmit_lock_owner = -1;
		dev->iflink = -1;
		dev_hold(dev);

		/*
		 * Allocate name. If the init() fails
		 * the name will be reissued correctly.
		 */
		if (strchr(dev->name, '%'))
			dev_alloc_name(dev, dev->name);

		/* 
		 * Check boot time settings for the device.
		 */
		netdev_boot_setup_check(dev);

		if (dev->init && dev->init(dev)) {
			/*
			 * It failed to come up. It will be unhooked later.
			 * dev_alloc_name can now advance to next suitable
			 * name that is checked next.
			 */
			dev->deadbeaf = 1;
			dp = &dev->next;
		} else {
			dp = &dev->next;
			dev->ifindex = dev_new_index();
			if (dev->iflink == -1)
				dev->iflink = dev->ifindex;
			if (dev->rebuild_header == NULL)
				dev->rebuild_header = default_rebuild_header;
			dev_init_scheduler(dev);
			set_bit(__LINK_STATE_PRESENT, &dev->state);
		}
	}

	/*
	 * Unhook devices that failed to come up
	 */
	dp = &dev_base;
	while ((dev = *dp) != NULL) {
		if (dev->deadbeaf) {
			write_lock_bh(&dev_base_lock);
			*dp = dev->next;
			write_unlock_bh(&dev_base_lock);
			dev_put(dev);
		} else {
			dp = &dev->next;
		}
	}

#ifdef CONFIG_PROC_FS
	proc_net_create("dev", 0, dev_get_info);
	create_proc_read_entry("net/softnet_stat", 0, 0, dev_proc_stats, NULL);
#ifdef WIRELESS_EXT
	/* Available in net/core/wireless.c */
	proc_net_create("wireless", 0, dev_get_wireless_info);
#endif	/* WIRELESS_EXT */
#endif	/* CONFIG_PROC_FS */

	dev_boot_phase = 0;

	open_softirq(NET_TX_SOFTIRQ, net_tx_action, NULL);
	open_softirq(NET_RX_SOFTIRQ, net_rx_action, NULL);

	dst_init();
	dev_mcast_init();

#ifdef CONFIG_NET_SCHED
	pktsched_init();
#endif

    /* PANKAJ: Create a PROC entry for VOICE mode. This could be made board
     * specific. Added here */

	/* Create a proc entry that is used to enable / disable the firewall. */
	ptr_dir_entry = create_proc_entry("net/voice" ,0644, NULL);
	if (ptr_dir_entry == NULL)
	{
		printk ("Error: Unable to create the voice proc entry.\n");
		return -1;
	}
	ptr_dir_entry->data 	  = NULL;
	ptr_dir_entry->read_proc  = voice_read_level;
	ptr_dir_entry->write_proc = voice_write_level;
	ptr_dir_entry->owner 	  = THIS_MODULE;    

    /* Voice port initialization. */
    voice_port_initialization ();

	/*
	 *	Initialise network devices
	 */
	 
	net_device_init();

	return 0;
}

#ifdef CONFIG_HOTPLUG

/* Notify userspace when a netdevice event occurs,
 * by running '/sbin/hotplug net' with certain
 * environment variables set.
 */

static int net_run_sbin_hotplug(struct net_device *dev, char *action)
{
	char *argv[3], *envp[5], ifname[12 + IFNAMSIZ], action_str[32];
	int i;

	sprintf(ifname, "INTERFACE=%s", dev->name);
	sprintf(action_str, "ACTION=%s", action);

        i = 0;
        argv[i++] = hotplug_path;
        argv[i++] = "net";
        argv[i] = 0;

	i = 0;
	/* minimal command environment */
	envp [i++] = "HOME=/";
	envp [i++] = "PATH=/sbin:/bin:/usr/sbin:/usr/bin";
	envp [i++] = ifname;
	envp [i++] = action_str;
	envp [i] = 0;
	
	return call_usermodehelper(argv [0], argv, envp);
}
#endif

/* Max. number of voice ports that can be active in a system at a given point
 * in time.  */
#define MAX_NUMBER_VOICE_PORTS  10

/**************************************************************************
 * STRUCTURE -  VOICE_PORT
 **************************************************************************
 *  The structure contains information about the voice ports that are being
 *  used by the voice application for exchange of data. 
 **************************************************************************/
typedef struct VOICE_PORT
{
    /* Links to the voice port*/ 
    struct list_head        links;

    /* The port on which the voice application will exchange data */
    unsigned int            port;
}VOICE_PORT;

typedef struct VOICE_PORT_MCB
{
    /* List of all pre-allocated voice ports. */
    struct list_head        available_port_list;

    /* List of active voice ports. */
    struct list_head        active_port_list;
}VOICE_PORT_MCB;

VOICE_PORT_MCB      voice_port_mcb;

static int voice_port_initialization (void)
{
    int index = 0;

    printk ("DEBUG: Initializing the voice port management module. \n");

    /* Initialize the port lists. */
    INIT_LIST_HEAD(&voice_port_mcb.active_port_list);
    INIT_LIST_HEAD(&voice_port_mcb.available_port_list);

    /* Allocate memory for the available port lists. */
    for (index = 0; index < MAX_NUMBER_VOICE_PORTS; index++)
    {
        /* Allocate memory for the voice port. */
        VOICE_PORT* ptr_voice_port = kmalloc (sizeof(VOICE_PORT), GFP_KERNEL);
        if (ptr_voice_port == NULL)
        {
            printk ("ERROR: Unable to allocate memory for the voice port management module.\n");
            return -1;
        }

        /* Initialize the memory block. */
        memset ((void *)ptr_voice_port, 0, sizeof(VOICE_PORT));

        /* Add the list of available ports. */
        list_add_tail((struct list_head *)&ptr_voice_port->links,&voice_port_mcb.available_port_list);
    }

    printk ("DEBUG: Initialization of the voice port management module successful..\n");
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : voice_register_protocol_port
 **************************************************************************
 * DESCRIPTION   :
 *  The function registers a protocol port associated with a voice 
 *  connection. This is used by the protocol inspector to prioritize data 
 *  being received on the interfaces.
 *
 * RETURNS       :
 *      0   - Success
 *      <0  - Error
 ***************************************************************************/
int voice_register_protocol_port (unsigned int port)
{
    struct list_head*   ptr_list_head;
    VOICE_PORT*         ptr_voice_port;

    printk ("DEBUG: Registering port %d.\n", ntohs(port));

    /* Cycle through the list of voice ports to see if the current port has
     * already been registered. */
    list_for_each(ptr_list_head, &voice_port_mcb.active_port_list)
    {
        ptr_voice_port = (VOICE_PORT *)ptr_list_head;

        /* Do we have a match. */
        if (ptr_voice_port->port == port)
            return -1;
    }

    /* Allocate a voice port block from the available list. But if the list is
     * empty there are none available.*/
    if (list_empty((struct list_head *)&voice_port_mcb.available_port_list))
    {
        printk ("DEBUG: No more voiceport blocks available. \n");
        return -1;
    }

    /* Dequeue the head from the list. */
    ptr_list_head  = ((struct list_head*)&voice_port_mcb.available_port_list)->next;
    list_del ((struct list_head *)ptr_list_head );

    /* Initialize the port structure. */
    ptr_voice_port = (VOICE_PORT *)ptr_list_head;
    ptr_voice_port->port = port;

    /* Add to the active port list. */
    list_add ((struct list_head *)ptr_voice_port, (struct list_head *)&voice_port_mcb.active_port_list);

    printk ("DEBUG: Succesful registration\n");

#if 0
    /* Debug Work. */
    printk (" -------------------------------------------------------------- \n");
    printk (" Active Port List.\n");
    list_for_each(ptr_list_head, &voice_port_mcb.active_port_list)
    {
        printk (" 0x%p.\n", ptr_list_head);
    }
    printk (" -------------------------------------------------------------- \n");
    printk (" Available Port List.\n");
    list_for_each(ptr_list_head, &voice_port_mcb.available_port_list)
    {
        printk (" 0x%p.\n", ptr_list_head);
    }
    printk (" -------------------------------------------------------------- \n");
#endif

    return 0;
}

/**************************************************************************
 * FUNCTION NAME : voice_deregister_protocol_port
 **************************************************************************
 * DESCRIPTION   :
 *  The function deregisters a protocol port associated with a voice 
 *  connection. 
 *
 * RETURNS       :
 *      0   - Success
 *      <0  - Error
 ***************************************************************************/
int voice_deregister_protocol_port (int port)
{
    struct list_head*   ptr_list_head;
    VOICE_PORT*         ptr_voice_port = NULL;

    printk ("DEBUG: Deregistering port %d.\n", ntohs(port));

    /* Cycle through the list of voice ports to see if the current port has
     * already been registered. */
    list_for_each(ptr_list_head, &voice_port_mcb.active_port_list)
    {
        ptr_voice_port = (VOICE_PORT *)ptr_list_head;

        /* Do we have a match. */
        if (ptr_voice_port->port == port)
            break;
    }

    /* Did we find a match. */
    if (ptr_voice_port == NULL)
    {
        printk ("Error: No voice port information.\n");
        return -1;
    }

    /* Delete the node from the ACTIVE Port List. */
    list_del ((struct list_head *)ptr_voice_port);

    /* Initialize the port structure. */
    ptr_voice_port->port = 0xFFFFFF;

    /* Add to the available port list. */
    list_add ((struct list_head *)ptr_voice_port, (struct list_head *)&voice_port_mcb.available_port_list);

    printk ("DEBUG: Succesful deregistration\n");

#if 0
    /* Debug Work. */
    printk (" -------------------------------------------------------------- \n");
    printk (" Active Port List.\n");
    list_for_each(ptr_list_head, &voice_port_mcb.active_port_list)
    {
        printk (" 0x%p.\n", ptr_list_head);
    }
    printk (" -------------------------------------------------------------- \n");
    printk (" Available Port List.\n");
    list_for_each(ptr_list_head, &voice_port_mcb.available_port_list)
    {
        printk (" 0x%p.\n", ptr_list_head);
    }
    printk (" -------------------------------------------------------------- \n");
#endif
    return 0;

}

/**************************************************************************
 * FUNCTION NAME : voice_port_match
 **************************************************************************
 * DESCRIPTION   :
 *  The function searches the active list of voice ports and matches the
 *  'port' with an element in the list. 
 *
 * RETURNS       :
 *      0   - No match found.
 *      1   - Match found.
 ***************************************************************************/
int voice_port_match (unsigned int port)
{
    struct list_head*   ptr_list_head;
    VOICE_PORT*         ptr_voice_port = NULL;

    /* Cycle through the list of voice ports to see if the current port has
     * already been registered. */
    list_for_each(ptr_list_head, &voice_port_mcb.active_port_list)
    {
        /* Get the voice port information. */
        ptr_voice_port = (VOICE_PORT *)ptr_list_head;

        /* Do we have a match. */
        if (ptr_voice_port->port == port)
            return 1;
    }

    /* No match found. */
    return 0;
}
