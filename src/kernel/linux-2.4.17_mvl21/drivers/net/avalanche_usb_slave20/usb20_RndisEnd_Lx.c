 /*******************************************************************************
 * FILE PURPOSE:    RNDIS Interface to the NET 
 *******************************************************************************
 * FILE NAME:   	sysLinRndisEnd.c
 *
 * DESCRIPTION:  	
 * 	This is a RNDIS End Interface which is added to the END device table list in 
 * 	the configNet header file. 
 * 	
 * (C) Copyright 2003, Texas Instruments, Inc.
 ********************************************************************************/

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/autoconf.h>

#if defined(CONFIG_MODVERSIONS) && !defined(MODVERSIONS)
#define MODEVERSIONS
#endif

#ifdef MODVERSIONS
#include <linux/modversions.h>
#endif

#include <linux/module.h>

#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/proc_fs.h>
#include <asm/io.h>

#include <asm/mips-boards/prom.h>
#include <asm/uaccess.h>

#include "usb20_config.h"
#include "usb20_rndis.h"


/************************* LOCAL Definations ***************************/

#define USB_ERROR 	"USB 2.0 ERROR:"
#define USB 		"USB 2.0: "

#define USB20_RNDIS_DRIVER_VERSION "0.0.2"

/************************* STRUCTURE Declarations ***************************/

struct SYS_RNDIS_DEVICE
{
	int						unit;           	/* unit number 				 */
	void*					hnd_rndis;			/* Handle to RNDIS layer.	 */
	struct net_device*		ptr_netdev;			/* Pointer to the NET device */
	struct net_device_stats stats;   			/* Ethernet statistics */
};

/************************* STATIC Declarations ***************************/

static int      USB20_init_module(void);
static void     USB20_cleanup_module(void);
static int      USB20_Init(struct net_device * dev);
static int      USB_Open(struct net_device *dev);
static int      USB_Close(struct net_device *);
static int      USB_sendTx(struct sk_buff *skb, struct net_device *dev);
#ifdef USB_MIB2
static int usb_do_ioctl( struct net_device *dev, struct ifreq *if_req, int cmd);
#endif /* USB_MIB2 */
static int 	usbrndis_proc_link(char *buf, char **start, off_t offset, int count, int *eof, void *data);
static int 	usbrndis_proc_version(char *buf, char **start, off_t offset, int count, int *eof, void *data);
static int 	usbrndis_read_proc_stats(char* buf, char **start, off_t offset, int count, int *eof, void *data);
static int 	usbrndis_write_proc_stats (struct file *fp,const char * buf, unsigned long count, void * data);
static int 	usbrndis_read_proc_info(char* buf, char **start, off_t offset, int count, int *eof, void *data);
static int 	usbrndis_write_proc_info (struct file *fp,const char * buf, unsigned long count, void * data);
static struct net_device_stats *USB_get_stats(struct net_device *dev);

/************************* EXTERN Declarations ***************************/

extern void* rndis_usb_load (HAL_HND h_end, char *ptr_mac_address, HAL_SYS_TBL *ptr_sys_tbl);
extern void  rndis_usb_start (void* hnd_rndis);
extern int   rndis_usb_send_packet (void *h_rndis, char *ptr_data, int data_length, UINT32 chain_id);
extern int 	 rndis_usb_unload (HAL_HND h_rndis);
extern RNDIS_STATS* rndis_usb_get_statistics (HAL_HND h_usbapp);
extern int rndis_usb_reset_statistics(HAL_HND h_usbapp);
#ifdef USB_MIB2
extern int rndis_usb_get_mib_stats(RNDIS_MCB * ptr_rndis_mcb, int command, void * data);
#endif
extern int rndis_usb_get_host_mac_address(HAL_HND h_usbapp, unsigned char e_addr[6]);
extern int  sys_critical_start (HAL_SYS_CRIT_CNT* p_context);
extern int  sys_critical_end   (HAL_SYS_CRIT_CNT* p_context);
extern int	usblogMsg (int level, char* fmt, int arg1, int arg2, int arg3, int arg4, int arg5);

extern void *linux_malloc(size_t size);
extern int atoi (const char *__nptr);

extern int usbDbgLevel;


/************************* GLOBAL Declarations ***************************/
static struct SYS_RNDIS_DEVICE*	pDrvCtrl;


/*int usbs_link_state = FALSE;*/  /* False == link down 
			      *	True == link up
			      */


MODULE_PARM(usb20_base_address, "l");
MODULE_PARM_DESC(usb20_base_address, "Base address of V2USB device");
MODULE_PARM(usb20_usb_irq, "i");
MODULE_PARM_DESC(usb20_usb_irq, "IRQ number for the USB interrupt");
MODULE_PARM(usb20_vbus_irq, "i");
MODULE_PARM_DESC(usb20_vbus_irq, "IRQ number for the VBUS interrupt");


MODULE_DESCRIPTION  ("TI USB 2.0 RNDIS Device Driver");
MODULE_AUTHOR 		("Texas Instruments");

/************************* FUNCTIONS *************************************/

/**************************************************************************
 * FUNCTION NAME : USB20_Init
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
//static int __init USB20_Init (struct net_device *dev)
static int USB20_Init (struct net_device *dev)
{
	/*printk(USB "Entering USB20_Init.\n");*/
    
	ether_setup (dev);  			/* assign some of the fields */

    dev->addr_len 			= 6;
    dev->open 				= &USB_Open;
    dev->hard_start_xmit 	= &USB_sendTx;
    dev->stop 				= &USB_Close;
    dev->get_stats 			= &USB_get_stats;
#ifdef USB_MIB2
   	dev->do_ioctl                       = &usb_do_ioctl; /*added for MIB2 Support */
#endif /* USB_MIB2 */

	/*printk(USB "Leaving USB20_Init.\n");*/
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : USB_Open
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
static int USB_Open (struct net_device *dev)
{
	MOD_INC_USE_COUNT;

	return 0;
}

/**************************************************************************
 * FUNCTION NAME : USB_Close
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
static int USB_Close (struct net_device *dev)
{
	MOD_DEC_USE_COUNT;

    return 0;
}

/**************************************************************************
 * FUNCTION NAME : USB_sendTx
 **************************************************************************
 * DESCRIPTION   :
 *	The function is used to send data to the RNDIS layer.
 *
 * RETURNS		 :
 * 	
***************************************************************************/
static int USB_sendTx (struct sk_buff *skb,struct net_device *dev)
{

	/* Send the packet to the lower layer. */
	if (rndis_usb_send_packet (pDrvCtrl->hnd_rndis, skb->data,skb->len, (UINT32)skb) < 0)
	{
		/* Error - Unable to send the packet. */
		dev_kfree_skb_any (skb);
	}
	return 0;
}


/**************************************************************************
 * FUNCTION NAME : USB20_init_module
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
//static int __init USB_init_module(void)
static int USB20_init_module(void)
{
	struct net_device*		dev;
	int						result;
    size_t 					dev_size;
    unsigned char 			mac_addr[ETH_ALEN];
	HAL_SYS_TBL 			sys;
	struct proc_dir_entry * proc_stats, *proc_info;
	
	
	printk(USB "Initializing USB 2.0 module.\n");

	printk(USB "Using parameters Base address:0x%08x USB IRQ:%d VBUS IRQ:%d.\n", usb20_base_address, usb20_usb_irq, usb20_vbus_irq);

    /* Setup proc entry */
	create_proc_read_entry ("avalanche/usb0_link",0,NULL,usbrndis_proc_link,NULL);
	create_proc_read_entry ("avalanche/usb0_ver",0,NULL,usbrndis_proc_version,NULL);
	
	proc_stats=create_proc_entry("avalanche/usb0_stat",0644,NULL);
	if(proc_stats == NULL)
	{
		printk("Error creating procfs entry for USB modules!\n");
	}
	else
	{
		proc_stats->read_proc = usbrndis_read_proc_stats;
		proc_stats->write_proc = usbrndis_write_proc_stats;

	}

	proc_info=create_proc_entry("avalanche/usb0_info",0644,NULL);
	if(proc_info == NULL)
	{
		printk("Error creating procfs entry for USB modules!\n");
	}
	else
	{
		proc_info->read_proc = usbrndis_read_proc_info;
		proc_info->write_proc = usbrndis_write_proc_info;

	}

	/* Determine the size of the RNDIS Device. */
	dev_size = sizeof(struct net_device) + sizeof(struct SYS_RNDIS_DEVICE);

	/* Allocate memory for the RNDIS Device. */
	dev = (struct net_device *) kmalloc(dev_size, GFP_KERNEL);
    if (dev == NULL)
    {
    	printk (USB_ERROR "unable to malloc for USB\n");
		return -ENODEV;
	}

    /* Reset all the fields in the device structure. */
    memset( (void *)dev, 0, dev_size);
    
	/* Initialize the DRIVER control object. */
	pDrvCtrl = (struct SYS_RNDIS_DEVICE *)(((void *) dev) + sizeof(struct net_device));
	pDrvCtrl->ptr_netdev 	= dev;
	pDrvCtrl->unit 			= 1;	

	/* Initialize all the fields in the NET DEVICE structure. */
	dev->init = USB20_Init;
    dev->priv = (void *)pDrvCtrl;
	strcpy (dev->name, "usb0");


	/* Get the device mac address */
	usb20_config_get_device_mac_addr((unsigned char *)&(dev->dev_addr[0]));

	/* Get the host side mac address */
    usb20_config_get_host_mac_addr((unsigned char *)&(mac_addr[0]));
	
	if(memcmp(mac_addr,dev->dev_addr,ETH_ALEN)==0)
	{
		mac_addr[5] +=1;
		printk(USB "Invalid MAC address. Address already in use\n");
		printk(USB "Using host mac address %02x:%02x:%02x:%02x:%02x:%02x\n",mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
	}
	
	/* Fill in the sys structure. */
	sys.version 	= 0x00010000;		/* Version 1.0 */
	sys.malloc 		= linux_malloc;
	sys.free 		= (HAL_SYS_FREE_RTN) kfree;
	sys.crit_start 	= sys_critical_start;
	sys.crit_end 	= sys_critical_end;
	sys.memcpy 		= memcpy;
	sys.memset 		= memset;
	sys.memcmp 		= memcmp;
	sys.strcpy 		= NULL;
	sys.strcmp 		= NULL;
	sys.strncpy 	= NULL;
	sys.strncmp 	= NULL;
	sys.sprintf 	= sprintf;
	sys.log 		= usblogMsg;


	/* Load the RNDIS layer and remember the handle. */
	pDrvCtrl->hnd_rndis = rndis_usb_load ( (void*)pDrvCtrl, (char *)mac_addr, &sys);
	if (pDrvCtrl->hnd_rndis == NULL)
		return -ENODEV;
	
	/* Start the RNDIS Stack. */
	rndis_usb_start (pDrvCtrl->hnd_rndis);
	
	/* Register the NET device. */
	result = register_netdev(dev);
	if(result)
    {
    	printk(USB_ERROR "USB: Could not register device.\n");
		kfree(dev);
	}
	
	printk(USB "USB 2.0 Driver Initialization complete.\n");
	
	return  0;
}

/**************************************************************************
 * FUNCTION NAME : USB20_cleanup_module
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
static void USB20_cleanup_module (void)
{
	struct net_device *dev;
	UINT32 lock;

	printk(USB "Entering USB20_cleanup_module.\n");	

	/* Get the NET device from the structure. */
	dev = pDrvCtrl->ptr_netdev;

    SAL_int_off(&lock);
	
	/* Stop and unload the RNDIS layer. */
	rndis_usb_unload (pDrvCtrl->hnd_rndis);

	SAL_int_on(&lock);

	/* Unregister the device. */
	unregister_netdev(dev);
	/* Cleanup device memory. */
	kfree(dev);
	/* Remove PROC entries. */
	remove_proc_entry("avalanche/usb0_stat",NULL);
    remove_proc_entry("avalanche/usb0_info",NULL);
    remove_proc_entry("avalanche/usb0_link",NULL);
	remove_proc_entry("avalanche/usb0_ver", NULL );
	
	printk(USB "Leaving USB20_cleanup_module.\n");	

	return;
}

#ifdef USB_MIB2
/**************************************************************************
 * FUNCTION NAME : usb_do_ioctl
 **************************************************************************
 * DESCRIPTION   : IOCTL command handler;
 *
 * RETURNS		 :
***************************************************************************/
static int usb_do_ioctl( struct net_device *dev, struct ifreq *if_req, int cmd)
{
	int ret = 0;
	HAL_HND hnd_rndis = pDrvCtrl->hnd_rndis;

    TI_SNMP_CMD_T *mib_command;

	if (cmd != SIOCDEVPRIVATE)
	{
			return -EFAULT;
	}
	
	mib_command = (TI_SNMP_CMD_T *) if_req->ifr_data;

	switch(mib_command->cmd)
	{
		case SIOCGETHERCOUNTERS: 
		case SIOCGUSBCOUNTERS: 
			{
				USB_MIB2_PHY_COUNTERS localCounters;
				ret = rndis_usb_get_mib_stats(hnd_rndis,mib_command->cmd, (void *) &localCounters);
				printk("eth counters\n");
				if(!ret)
				{
					if(copy_to_user((char *)mib_command->data, (char *)&localCounters, sizeof(USB_MIB2_PHY_COUNTERS)))
							ret = -EFAULT;
				}
				else
				{
				ret = -EFAULT;
				}
				
				break;
			}
			
		case SIOCGINTFCOUNTERS: 
			{
				USB_MIB2_IF_COUNTERS localCounters;
				ret = rndis_usb_get_mib_stats(hnd_rndis,mib_command->cmd, (void *) &localCounters);
				printk("usb counters\n");
				if(!ret)
				{
					if(copy_to_user((char *)mib_command->data, (char *)&localCounters, sizeof(USB_MIB2_IF_COUNTERS)))
					{
							ret = -EFAULT;
					}
				}
				else
				{
				ret = -EFAULT;
				}
				
				break;
			}
			
		case SIOCGUSBPARAMS: 
			{
				USB_MIB2_USB_PARAMS localParams;
				USB_MIB2_USB_PARAMS * temp;		
				char packet_filter[5],stats_capabilities[5];
				char * orig_packet_filter,*orig_stats_capabilities;
				
				localParams.usbCDCEtherPacketFilter = &packet_filter[0];
				localParams.usbCDCEtherStatCapabilities = &stats_capabilities[0];
				/* save char * pointers for updating later */
				temp = (USB_MIB2_USB_PARAMS *)mib_command->data;
				orig_packet_filter = temp->usbCDCEtherPacketFilter;
				orig_stats_capabilities = temp->usbCDCEtherStatCapabilities;
				ret = rndis_usb_get_mib_stats(hnd_rndis,mib_command->cmd, (void *) &localParams);
				if(!ret)
				{
						/* reinstall the original pointers */
			    localParams.usbCDCEtherPacketFilter = orig_packet_filter;
			    localParams.usbCDCEtherStatCapabilities = orig_stats_capabilities ;
					if(copy_to_user((char *)mib_command->data, (char *)&localParams, sizeof(USB_MIB2_USB_PARAMS)))
					{
							ret = -EFAULT;
					}
				/* copy the strings seperately */	
					if(copy_to_user(orig_packet_filter, &packet_filter[0], localParams.usbCDCEtherPacketFilterLength))
					{
							ret = -EFAULT;
					}
					if(copy_to_user(orig_stats_capabilities, &stats_capabilities[0], localParams.usbCDCEtherStatCapabilitiesLength))
					{
							ret = -EFAULT;
					}

					
				}
				else
				{
				ret = -EFAULT;
				}
				
				break;
			}
			
		case SIOCGETHERPARAMS: 
			{
				USB_MIB2_ETH_PARAMS localParams;
				ret = rndis_usb_get_mib_stats(hnd_rndis,mib_command->cmd, (void *) &localParams);
				if(!ret)
				{
					if(copy_to_user((char *)mib_command->data, (char *)&localParams, sizeof(USB_MIB2_ETH_PARAMS)))
							ret = -EFAULT;
				}
				else
				{
				ret = -EFAULT;
				}
				
				break;
			}
			
		case SIOCGINTFPARAMS: 
			{
				USB_MIB2_IF_PARAMS localParams;
				ret = rndis_usb_get_mib_stats(hnd_rndis,mib_command->cmd, (void *) &localParams);
				if(!ret)
				{
					localParams.ifPromiscuousMode = (dev->flags & IFF_PROMISC) ? MIB2_TRUTH_VALUE_TRUE : MIB2_TRUTH_VALUE_FALSE;
					if(copy_to_user((char *)mib_command->data, (char *)&localParams, sizeof(USB_MIB2_IF_PARAMS)))
							ret = -EFAULT;
				}
				else
				{
				ret = -EFAULT;
				}
				
				break;
			}
			
		case SIOCSINTFADMINSTATUS: 
			{
				USB_MIB2_IF_COMMAND localParams;
				ret = rndis_usb_get_mib_stats(hnd_rndis,mib_command->cmd, (void *) &localParams);
				if(!ret)
				{
					/*if(copy_to_user((char *)mib_command->data, (char *)&localParams, sizeof(USB_MIB2_ETHER_PARAMS)))
							ret = -EFAULT;*/
				}
				else
				{
				ret = -EFAULT;
				}
				
				break;
			}
		default: 
			{
					printk("Unknown USB MIB Command\n");
				ret = -ENOTSUPP;
				break;
			}
	
	}

	return ret;
}
#endif /* USB_MIB2 */

/**************************************************************************
 * FUNCTION NAME : usbrndis_proc_link
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
static int usbrndis_proc_link(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
	int len=0;

   USB_DEV * usbdev = usb_drv_get_dev();

   if(usbdev->link_status == 1)
   {
	len += sprintf(buf+len, "USB 2.0  RNDIS:Link is UP\n");

	len += sprintf(buf+len, "Link speed: %d Mbps [%s]\n", (usbdev->params.speed)/1000000, (usbdev->params.speed ==12000000 ? "Full Speed" : "High Speed"));
   }
   else if(usbdev->link_status == 0)
   {
	len += sprintf(buf+len, "USB 2.0 RNDIS:Link is DOWN\n");
   }
   else
	len += sprintf(buf+len, "USB 2.0 RNDIS:Unknown Link State\n");
	
	/*
	if(usbs_link_state == FALSE)
	{
	len += sprintf(buf+len, "USB RNDIS:Link is DOWN\n");
	}
	else if ( usbs_link_state == TRUE)
	{
	len += sprintf(buf+len, "USB RNDIS:Link is UP\n");
	}
	else
	len += sprintf(buf+len, "USB RNDIS:Unknown Link State\n");*/
  	return len;
}

/**************************************************************************
 * FUNCTION NAME : usbrndis_proc_version
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
static int usbrndis_proc_version(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
	int len=0;
/* Driver Version */
	len += sprintf(buf+len, "Texas Instruments USB 2.0 RNDIS Driver Version ");
	len += sprintf(buf+len, USB20_RNDIS_DRIVER_VERSION" \n");
	return len;
}	

/**************************************************************************
 * FUNCTION NAME : USB_get_stats
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
static struct net_device_stats *USB_get_stats (struct net_device *dev)
{
	long 			flags;
	RNDIS_STATS*	ptr_rndis_stats;

	save_flags(flags);
	cli();

	/* Get the RNDIS stats from the lower layer. */
	ptr_rndis_stats = rndis_usb_get_statistics (pDrvCtrl->hnd_rndis);

	/* Total receive packets - Unicast + Multicast + Broadcast */
    pDrvCtrl->stats.rx_packets = ptr_rndis_stats->broadcast_pkts_rxed + 
								 ptr_rndis_stats->multicast_pkts_rxed +
								 ptr_rndis_stats->unicast_pkts_rxed;

	/* Total receive bytes - Unicast + Multicast + Broadcast */
	pDrvCtrl->stats.rx_bytes   = ptr_rndis_stats->broadcast_bytes_rxed + 
								 ptr_rndis_stats->multicast_bytes_rxed +
								 ptr_rndis_stats->unicast_bytes_rxed;

	/* Total transmitted packets - Unicast + Multicast + Broadcast */
    pDrvCtrl->stats.tx_packets = ptr_rndis_stats->broadcast_pkts_txed + 
								 ptr_rndis_stats->multicast_pkts_txed +
								 ptr_rndis_stats->unicast_pkts_txed;
   
	/* Total transmitted bytes - Unicast + Multicast + Broadcast */
    pDrvCtrl->stats.tx_bytes   = ptr_rndis_stats->broadcast_bytes_txed + 
								 ptr_rndis_stats->multicast_bytes_txed +
								 ptr_rndis_stats->unicast_bytes_txed;

	/* Total Multicast packets received.*/
	pDrvCtrl->stats.multicast  = ptr_rndis_stats->multicast_pkts_rxed;         

	sti();
    restore_flags(flags);
	
 	return &pDrvCtrl->stats;
}

/**************************************************************************
 * FUNCTION NAME : usbrndis_read_proc_stats
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
static int usbrndis_read_proc_stats
(
	char* 	buf, 
	char**	start, 
	off_t 	offset, 
	int 	count,
	int*	eof, 
	void*	data
)
{
    int 						len 	= 0;
    int 						limit 	= count - 80;
	RNDIS_STATS*				ptr_rndis_stats;

	/* Get the RNDIS stats from the lower layer. */
	ptr_rndis_stats = rndis_usb_get_statistics (pDrvCtrl->hnd_rndis);
 
	/* Transmit stats */
    if(len<=limit)
    	len+= sprintf(buf+len, "\nUSB 2.0 - RNDIS\n");
	if(len<=limit)
    	len+= sprintf(buf+len, " Transmit Stats\n");

	/* Print the Number of broadcast frames. */
	if(len<=limit)
		len+= sprintf(buf+len, "   Good Tx Broadcast Frames   :%u\n", ptr_rndis_stats->broadcast_pkts_txed);

 	/* Print the Number of Multicast frames that have transmitted. */	
    if(len<=limit)
    	len+= sprintf(buf+len, "   Good Tx Multicast Frames   :%u\n", ptr_rndis_stats->multicast_pkts_txed);

 	/* Print the Number of Unicast frames that have transmitted. */	
    if(len<=limit)
    	len+= sprintf(buf+len, "   Good Tx Unicast Frames     :%u\n", ptr_rndis_stats->unicast_pkts_txed);

	/* Print the Number of broadcast bytes. */
	if(len<=limit)
		len+= sprintf(buf+len, "   Good Tx Broadcast Bytes   :%u\n", ptr_rndis_stats->broadcast_bytes_txed);

	/* Print the Number of Multicast bytes. */
	if(len<=limit)
		len+= sprintf(buf+len, "   Good Tx Multicast Bytes   :%u\n", ptr_rndis_stats->multicast_bytes_txed);
	
	/* Print the Number of unicast bytes. */
	if(len<=limit)
		len+= sprintf(buf+len, "   Good Tx Unicast Bytes   :%u\n", ptr_rndis_stats->unicast_bytes_txed);
	
	/* Print the Number of packets dropped */
	if(len<=limit)
		len+= sprintf(buf+len, "   Tx Frames dropped       :%u\n", ptr_rndis_stats->transmit_packets_dropped);
	
	/* Receive Stats */
  	if(len<=limit)
    	len+= sprintf(buf+len, " Receive Stats\n");
 
	/* Print the Number of broadcast frames. */
	if(len<=limit)
		len+= sprintf(buf+len, "   Good Rx Broadcast Frames   :%u\n", ptr_rndis_stats->broadcast_pkts_rxed);

 	/* Print the Number of Multicast frames that have transmitted. */	
    if(len<=limit)
    	len+= sprintf(buf+len, "   Good Rx Multicast Frames   :%u\n", ptr_rndis_stats->multicast_pkts_rxed);

 	/* Print the Number of Unicast frames that have transmitted. */	
    if(len<=limit)
    	len+= sprintf(buf+len, "   Good Rx Unicast Frames     :%u\n", ptr_rndis_stats->unicast_pkts_rxed);

	/* Print the Number of broadcast bytes. */
	if(len<=limit)
		len+= sprintf(buf+len, "   Good Rx Broadcast Bytes    :%u\n", ptr_rndis_stats->broadcast_bytes_rxed);

	/* Print the Number of Multicast bytes. */
	if(len<=limit)
		len+= sprintf(buf+len, "   Good Rx Multicast Bytes   :%u\n", ptr_rndis_stats->multicast_bytes_rxed);
	
	/* Print the Number of unicast bytes. */
	if(len<=limit)
		len+= sprintf(buf+len, "   Good Rx Unicast Bytes   :%u\n", ptr_rndis_stats->unicast_bytes_rxed);
	
	/* Print the Number receive packets dropped */
	if(len<=limit)
		len+= sprintf(buf+len, "   Rx Frames Dropped       :%u\n", ptr_rndis_stats->receive_packets_dropped);
	return len;
}

/**************************************************************************
 * FUNCTION NAME : usbrndis_write_proc_stats
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
static int usbrndis_write_proc_stats (struct file *fp,const char * buf, unsigned long count, void * data)
{
	char local_buf[31];
	if(count > 30)
	{
		printk("Error : Buffer Overflow\n");
		printk("Use \"echo 0 > usb0_stat\" to reset the statistics\n");
		return -EFAULT;
	}

	copy_from_user(local_buf,buf,count);
	local_buf[count-1]='\0'; /* Ignoring last \n char */
	
	if(strcmp("0",local_buf)==0)
	{
		/* Valid command */
		printk("Resetting statistics for usb0 interface.\n");
		if(rndis_usb_reset_statistics(pDrvCtrl->hnd_rndis))
		{
			printk("Error resetting statistics for usb0 interface\n");
		}
	}
	else
	{
		printk("Error: Unknown operation on usb0 statistics\n");
		printk("Use \"echo 0 > usb0_stat\" to reset the statistics\n");
		return -EFAULT;
	}
	
	return count;
}

/**************************************************************************
 * FUNCTION NAME : usbrndis_read_proc_info
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
static int usbrndis_read_proc_info(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
	int len=0;
	unsigned char mac_addr[6];
/* PC side Mac Address */
	rndis_usb_get_host_mac_address(pDrvCtrl->hnd_rndis,(unsigned char *)&mac_addr);
	len += sprintf(buf+len, "RNDIS PC MAC Address: %02x:%02x:%02x:%02x:%02x:%02x  \n", 
					mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3],mac_addr[4], mac_addr[5]);
			return len;
}

/**************************************************************************
 * FUNCTION NAME : usbrndis_write_proc_info
 **************************************************************************
 * DESCRIPTION   : Write function for the debug proc entry
 *
 * RETURNS		 :
***************************************************************************/
static int usbrndis_write_proc_info (struct file *fp,const char * buf, unsigned long count, void * data)
{
	
	return count;
}


/**************************************************************************
 * FUNCTION NAME : sysRndisAllocateBuffer
 **************************************************************************
 * DESCRIPTION   :
 * 	The function is called to allocate a data buffer that will be used to 
 * 	fill in data that has been received on the USB. 
 * 	
 * RETURNS		 :
 * 	NULL		- No more buffers available.
 * 	Non NULL	- Success
***************************************************************************/
char* sysRndisAllocateBuffer (HAL_HND h_end, UINT32 *receive_id)
{
	struct sk_buff* skb;

	/* Initialize the receive chain id to NULL */
	*receive_id = 0;

	/* Allocate a buffer. */
	/* BUffer size = 1514 (MTU) + 44 (RNDIS Header) +4 (alignment space) 
	 *               + 8 (VLAN tagging) = 1580                       
	 */
	skb = dev_alloc_skb (1580);
	if (skb == NULL)
	{
		printk ("ERROR: Memory allocation failed.\n");
		return NULL;
	}

	/* Store the address of the SKB into the receive identifier. */
	*receive_id = (UINT32)skb;
	
	/* Misalign the skb->data by 2 bytes, so that IP PACKET becomes 4-byte aligned. */
	skb_reserve(skb, 2);


	return ((char *)skb->data);
}

/**************************************************************************
 * FUNCTION NAME : sysRndisDeallocateBuffer
 **************************************************************************
 * DESCRIPTION   :
 * 	The function is used to deallocate a previously allocated buffer. This
 * 	function would be called if the USB cable is removed in the middle of
 * 	the reception of a data packet. The allocated buffer is put back in the
 * 	buffer pool.
***************************************************************************/
void sysRndisDeallocateBuffer (HAL_HND h_end, char *p_buffer, UINT32 receive_id)
{
	struct sk_buff* skb;

	/* Get the SKB pointer from the receive id. */
	skb = (struct sk_buff *)receive_id;

	/* Clean up the memory for the SKB. */
	dev_kfree_skb_any(skb);
	return;
}

/**************************************************************************
 * FUNCTION NAME : sysRndisEndSndPktComplete 
 **************************************************************************
 * DESCRIPTION   :
 *	The function is used to cleanup memory of a packet that has been 
 *	transmitted on the USB.
***************************************************************************/
void  sysRndisEndSndPktComplete (UINT32 chain_id)
{
	struct sk_buff *skb;

	/* Was a valid chain id posted ? */
	if (chain_id == 0)
		return;
	
	/* Get the SKB structure from the chain id. */
	skb = (struct sk_buff *)chain_id;
	
	/* Cleanup the memory of the transmitted packet. */
	dev_kfree_skb_any(skb);

	return;
}

/**************************************************************************
 * FUNCTION NAME : sysRndisEndReceiveData
 **************************************************************************
 * DESCRIPTION   :
 *	The function is called from the RNDIS layer when an entire packet is 
 *	received and is now to be passed to the IP stack.
 *	
 * RETURNS		 :
 * 	Always returns 0.
***************************************************************************/
int sysRndisEndReceiveData 
(
	void*	h_end, 				/* END device handle.			*/
	char*	ptr_data, 			/* Pointer to start of data.	*/
	UINT16 	packet_length,		/* Length of received data.		*/
	UINT32	receive_id			/* Receive Buffer Identifier.	*/
)
{
	struct net_device *dev;
    struct sk_buff    *skb;



	/* Get the NET device. */
	dev = (struct net_device *)pDrvCtrl->ptr_netdev;

	/* Get the SKB structure from the data pointer. */
	skb = (struct sk_buff * )receive_id;
	
	/* TODO : replace 44 with correct sizeof operator */
	/* Skip over the 44 bytes RNDIS header in the skb */
	(char *)(skb->data) += 44;  

	/* TODO: copy ptr_data into skb->data instead of adding 44 
	 * bytes, in order to be agnostic of the underlying
	 * protocol, RNDIS/CDC.
	 */
	
	/* Cannot use skb_pull because skb structure is not yet filly
	 * intiialised.
	 */
	/*skb_pull(skb,44);*/ 

	

	/* Fill the SKB structure. */
	skb_put(skb, packet_length);
	skb->dev = dev;
    skb->protocol = eth_type_trans(skb, dev);
    skb->ip_summed = CHECKSUM_UNNECESSARY;

	/* Give the data to the IP stack. */
	netif_rx(skb);

	return 0;	
}

/**************************************************************************
 * FUNCTION NAME : linux_malloc
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
void sysInvalidateDataCache(UINT32 address, int size)
{
	dma_cache_wback_inv ((unsigned long)address, size);
}


/**************************************************************************
 * FUNCTION NAME : sysFlushDataCache(UINT32 address,int size)
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
void sysFlushDataCache(UINT32 address, int size)
{
    dma_cache_wback_inv((unsigned long)address,size);
}

/**************************************************************************
 * FUNCTION NAME : linux_malloc
 **************************************************************************
 * DESCRIPTION   :
 *
 * RETURNS		 :
***************************************************************************/
void *linux_malloc(size_t size)
{
	return (kmalloc(size, GFP_KERNEL));
}


module_init (USB20_init_module);
module_exit (USB20_cleanup_module);
