/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Cobalt Qube specific PCI support.
 */
#include <linux/config.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <asm/cobalt/cobalt.h>
#include <asm/pci.h>
#include <asm/io.h>

#ifdef CONFIG_PCI

#define SELF 0

static void qube_expansion_slot_bist(void)
{
       	unsigned char ctrl;
       	int timeout = 100000;

       	pcibios_read_config_byte(0, (0x0a<<3), PCI_BIST, &ctrl);
       	if(!(ctrl & PCI_BIST_CAPABLE))
               	return;

       	pcibios_write_config_byte(0, (0x0a<<3), PCI_BIST, ctrl|PCI_BIST_START);
       	do {
               	pcibios_read_config_byte(0, (0x0a<<3), PCI_BIST, &ctrl);
               	if(!(ctrl & PCI_BIST_START))
                       	break;
       	} while(--timeout > 0);
       	if((timeout <= 0) || (ctrl & PCI_BIST_CODE_MASK))
               	printk("PCI: Expansion slot card failed BIST with code %x\n",
                      	(ctrl & PCI_BIST_CODE_MASK));
}

static void qube_expansion_slot_fixup(void)
{
	unsigned short pci_cmd;
       	unsigned long ioaddr_base = 0x10108000; /* It's magic, ask Doug. */
       	unsigned long memaddr_base = 0x12000000;
       	int i;

       	/* Enable bits in COMMAND so driver can talk to it. */
       	pcibios_read_config_word(0, (0x0a<<3), PCI_COMMAND, &pci_cmd);
       	pci_cmd |= (PCI_COMMAND_IO | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER);
       	pcibios_write_config_word(0, (0x0a<<3), PCI_COMMAND, pci_cmd);

       	/* Give it a working IRQ. */
       	pcibios_write_config_byte(0, (0x0a<<3), PCI_INTERRUPT_LINE, 9);

       	/* Fixup base addresses, we only support I/O at the moment. */
       	for(i = 0; i <= 5; i++) {
               	unsigned int regaddr = (PCI_BASE_ADDRESS_0 + (i * 4));
               	unsigned int rval, mask, size, alignme, aspace;
               	unsigned long *basep = &ioaddr_base;

               	/* Check type first, punt if non-IO. */
               	pcibios_read_config_dword(0, (0x0a<<3), regaddr, &rval);
               	aspace = (rval & PCI_BASE_ADDRESS_SPACE);
               	if(aspace != PCI_BASE_ADDRESS_SPACE_IO)
                       	basep = &memaddr_base;

               	/* Figure out how much it wants, if anything. */
               	pcibios_write_config_dword(0, (0x0a<<3), regaddr, 0xffffffff);
               	pcibios_read_config_dword(0, (0x0a<<3), regaddr, &rval);

               	/* Unused? */
               	if(rval == 0)
                       	continue;

               	rval &= PCI_BASE_ADDRESS_IO_MASK;
               	mask = (~rval << 1) | 0x1;
               	size = (mask & rval) & 0xffffffff;
               	alignme = size;
               	if(alignme < 0x400)
                       	alignme = 0x400;
               	rval = ((*basep + (alignme - 1)) & ~(alignme - 1));
               	*basep = (rval + size);
               	pcibios_write_config_dword(0,(0x0a<<3), regaddr, rval | aspace);
       	}
       	qube_expansion_slot_bist();
}

#define DEFAULT_BMIBA   0xcc00          /* in case ROM did not init it */

static void qube_raq_via_bmIDE_fixup(struct pci_dev *dev)
{
       	unsigned short cfgword;
       	unsigned char lt;
       	unsigned int bmiba;
       	int try_again = 1;

       	/* Enable Bus Mastering and fast back to back. */
        pci_read_config_word(dev, PCI_COMMAND, &cfgword);
        cfgword |= (PCI_COMMAND_FAST_BACK | PCI_COMMAND_MASTER);
        pci_write_config_word(dev, PCI_COMMAND, cfgword);

        /* Enable interfaces.  ROM only enables primary one.  */
        {
#ifdef CONFIG_BLK_DEV_COBALT_SECONDARY
                unsigned char iface_enable = 0xb;
#else
               	unsigned char iface_enable = 0xa;
#endif
                pci_write_config_byte(dev, 0x40, iface_enable);
        }

        /* Set latency timer to reasonable value. */
        pci_read_config_byte(dev, PCI_LATENCY_TIMER, &lt);
        if (lt < 64)
               	pci_write_config_byte(dev, PCI_LATENCY_TIMER, 64);
        pci_write_config_byte(dev, PCI_CACHE_LINE_SIZE, 7);

        /* Get the bmiba base address. */
        do {
                pci_read_config_dword(dev, 0x20, &bmiba);
                bmiba &= 0xfff0;        /* extract port base address */
                if (bmiba) {
                        break;
                } else {
              	 	printk("ide: BM-DMA base register is invalid (0x%08x)\n",bmiba);
                        if (inb(DEFAULT_BMIBA) != 0xff || !try_again)
                                break;
                        printk("ide: setting BM-DMA base register to 0x%08x\n",DEFAULT_BMIBA);
                        pci_write_config_dword(dev, 0x20, DEFAULT_BMIBA|1);
                }
       	} while (try_again--);

        bmiba += 0x10000000;

       	dev->resource[4].start = bmiba;
}

static void qube_raq_tulip_fixup(struct pci_dev *dev)
{
       	unsigned short pci_cmd;
       	extern int cobalt_is_raq;
       	unsigned int tmp;

       	/* Fixup the first tulip located at device PCICONF_ETH0 */
       	if (dev->devfn == PCI_DEVSHFT(COBALT_PCICONF_ETH0)) {
               /*
                * Now tell the Ethernet device that we expect an interrupt at
                * IRQ 13 and not the default 189.
                *
                * The IRQ of the first Tulip is different on Qube and RaQ
                * hardware except for the weird first RaQ bringup board,
                */
              	if (! cobalt_is_raq) {
                       	/* All Qube's route this the same way. */
                       	pci_write_config_byte(dev, PCI_INTERRUPT_LINE,
                                         	COBALT_ETHERNET_IRQ);
               	} else {
                       	/* Setup the first Tulip on the RAQ */
#ifndef RAQ_BOARD_1_WITH_HWHACKS
                       	pci_write_config_byte(dev, PCI_INTERRUPT_LINE, 4);
#else
                       	pci_write_config_byte(dev, PCI_INTERRUPT_LINE, 13);
#endif
               	}
       	/* Fixup the second tulip located at device PCICONF_ETH1 */
       	} else if (dev->devfn == PCI_DEVSHFT(COBALT_PCICONF_ETH1)) {
             	/* XXX Check for the second Tulip on the RAQ(Already got it!) */
               	pci_read_config_dword(dev, PCI_VENDOR_ID, &tmp);
               	if(tmp == 0xffffffff || tmp == 0x00000000)
                       	return;

               	/* Enable the second Tulip device. */
               	pci_read_config_word(dev, PCI_COMMAND, &pci_cmd);
               	pci_cmd |= (PCI_COMMAND_IO | PCI_COMMAND_MASTER);
               	pci_write_config_word(dev, PCI_COMMAND, pci_cmd);

               	/* Give it it's IRQ. */
               	/* NOTE: RaQ board #1 has a bunch of green wires which swapped
                 * the IRQ line values of Tulip 0 and Tulip 1.  All other
               	 * boards have eth0=4,eth1=13.  -DaveM
                 */
#ifndef RAQ_BOARD_1_WITH_HWHACKS
               	pci_write_config_byte(dev, PCI_INTERRUPT_LINE, 13);
#else
               	pci_write_config_byte(dev, PCI_INTERRUPT_LINE, 4);
#endif
               	/* And finally, a usable I/O space allocation, right after what
                 * the first Tulip uses.
                 */
               	pci_write_config_dword(dev, PCI_BASE_ADDRESS_0, 0x10101001);
       	}
}

static void qube_raq_scsi_fixup(struct pci_dev *dev)
{
       	unsigned short pci_cmd;
       	extern int cobalt_is_raq;
       	unsigned int tmp;

        /*
         * Tell the SCSI device that we expect an interrupt at
         * IRQ 7 and not the default 0.
         */
       	pci_write_config_byte(dev, PCI_INTERRUPT_LINE, COBALT_SCSI_IRQ);

       	if (cobalt_is_raq) {
               	/* Check for the SCSI on the RAQ */
               	pci_read_config_dword(dev, PCI_VENDOR_ID, &tmp);
               	if(tmp == 0xffffffff || tmp == 0x00000000)
                       	return;

               	/* Enable the device. */
               	pci_read_config_word(dev, PCI_COMMAND, &pci_cmd);

      		pci_cmd |= (PCI_COMMAND_IO | PCI_COMMAND_MASTER | PCI_COMMAND_MEMORY | PCI_COMMAND_INVALIDATE);
               	pci_write_config_word(dev, PCI_COMMAND, pci_cmd);

               	/* Give it it's IRQ. */
               	pci_write_config_byte(dev, PCI_INTERRUPT_LINE, 4);

               	/* And finally, a usable I/O space allocation, right after what
                 * the second Tulip uses.
                 */
               	pci_write_config_dword(dev, PCI_BASE_ADDRESS_0, 0x10102001);
               	pci_write_config_dword(dev, PCI_BASE_ADDRESS_1, 0x00002000);
               	pci_write_config_dword(dev, PCI_BASE_ADDRESS_2, 0x00100000);
       	}
}

static void qube_raq_galileo_fixup(struct pci_dev *dev)
{
       	unsigned short galileo_id;

       	/* Fix PCI latency-timer and cache-line-size values in Galileo
         * host bridge.
         */
       	pci_write_config_byte(dev, PCI_LATENCY_TIMER, 64);
       	pci_write_config_byte(dev, PCI_CACHE_LINE_SIZE, 7);

       	/* On all machines prior to Q2, we had the STOP line disconnected
         * from Galileo to VIA on PCI.  The new Galileo does not function
         * correctly unless we have it connected.
         *
         * Therefore we must set the disconnect/retry cycle values to
         * something sensible when using the new Galileo.
         */
        pci_read_config_word(dev, PCI_REVISION_ID, &galileo_id);
        galileo_id &= 0xff;     /* mask off class info */
        if (galileo_id == 0x10) {
                /* New Galileo, assumes PCI stop line to VIA is connected. */
                *((volatile unsigned int *)0xb4000c04) = 0x00004020;
        } else if (galileo_id == 0x1 || galileo_id == 0x2) {
               	unsigned int timeo;
                /* XXX WE MUST DO THIS ELSE GALILEO LOCKS UP! -DaveM */
                timeo = *((volatile unsigned int *)0xb4000c04);
                /* Old Galileo, assumes PCI STOP line to VIA is disconnected. */
                *((volatile unsigned int *)0xb4000c04) = 0x0000ffff;
        }
}

static void
qube_pcibios_fixup(struct pci_dev *dev)
{
      	extern int cobalt_is_raq;
       	unsigned int tmp;


       	if (! cobalt_is_raq) {
               	/* See if there is a device in the expansion slot, if so
                 * fixup IRQ, fix base addresses, and enable master +
                 * I/O + memory accesses in config space.
                 */
               	pcibios_read_config_dword(0, 0x0a<<3, PCI_VENDOR_ID, &tmp);
               	if(tmp != 0xffffffff && tmp != 0x00000000)
                       	qube_expansion_slot_fixup();
       	} else {
               	/* And if we are a 2800 we have to setup the expansion slot
                 * too.
                 */
               	pcibios_read_config_dword(0, 0x0a<<3, PCI_VENDOR_ID, &tmp);
               	if(tmp != 0xffffffff && tmp != 0x00000000)
                       	qube_expansion_slot_fixup();
       	}
}

struct pci_fixup pcibios_fixups[] = {
	/* TBD:: Add each device here and divvy up pcibios_fixup */
 	{ PCI_FIXUP_HEADER, PCI_VENDOR_ID_VIA, PCI_DEVICE_ID_VIA_82C586_1, qube_raq_via_bmIDE_fixup },
 	{ PCI_FIXUP_HEADER, PCI_VENDOR_ID_DEC, PCI_DEVICE_ID_DEC_21142, qube_raq_tulip_fixup },
 	{ PCI_FIXUP_HEADER, PCI_VENDOR_ID_GALILEO, PCI_ANY_ID, qube_raq_galileo_fixup },
	/* Not sure about what scsi chips are available on the RAQ, put an 
	   entry for all */
  	{ PCI_FIXUP_HEADER, PCI_VENDOR_ID_NCR, PCI_DEVICE_ID_NCR_53C860, qube_raq_scsi_fixup },
       	{ PCI_FIXUP_HEADER, PCI_ANY_ID, PCI_ANY_ID, qube_pcibios_fixup }
};

static __inline__ int pci_range_ck(struct pci_dev *dev)
{
	if ((dev->bus->number == 0)
             && ((PCI_SLOT (dev->devfn) == 0)
             || ((PCI_SLOT (dev->devfn) > 6)
             && (PCI_SLOT (dev->devfn) <= 12))))
               	return 0;  /* OK device number  */
       	return -1;  /* NOT ok device number */
}

#define PCI_CFG_DATA   ((volatile unsigned long *)0xb4000cfc)
#define PCI_CFG_CTRL   ((volatile unsigned long *)0xb4000cf8)

#define PCI_CFG_SET(dev,where) \
       ((*PCI_CFG_CTRL) = (0x80000000 | (PCI_SLOT ((dev)->devfn) << 11) | \
                           (PCI_FUNC ((dev)->devfn) << 8) | (where)))

static int qube_pci_read_config_dword(struct pci_dev *dev, int where, u32 *val)
{
	if (where & 0x3)
               	return PCIBIOS_BAD_REGISTER_NUMBER;
       	if (pci_range_ck (dev)) {
               	*val = 0xFFFFFFFF;
               	return PCIBIOS_DEVICE_NOT_FOUND;
       	}
       	PCI_CFG_SET(dev, where);
       	*val = *PCI_CFG_DATA;
       	return PCIBIOS_SUCCESSFUL;
}

static int qube_pci_read_config_word(struct pci_dev *dev, int where, u16 *val)
{
        if (where & 0x1)
               	return PCIBIOS_BAD_REGISTER_NUMBER;
       	if (pci_range_ck (dev)) {
               	*val = 0xffff;
               	return PCIBIOS_DEVICE_NOT_FOUND;
       	}
       	PCI_CFG_SET(dev, (where & ~0x3));
       	*val = *PCI_CFG_DATA >> ((where & 3) * 8);
       	return PCIBIOS_SUCCESSFUL;
}

static int qube_pci_read_config_byte(struct pci_dev *dev, int where, u8 *val)
{
       	if (pci_range_ck (dev)) {
               	*val = 0xff;
               	return PCIBIOS_DEVICE_NOT_FOUND;
       	}
       	PCI_CFG_SET(dev, (where & ~0x3));
       	*val = *PCI_CFG_DATA >> ((where & 3) * 8);
       	return PCIBIOS_SUCCESSFUL;
}

static int qube_pci_write_config_dword(struct pci_dev *dev, int where, u32 val)
{
	if (where & 0x3)
               	return PCIBIOS_BAD_REGISTER_NUMBER;
       	if (pci_range_ck (dev))
               	return PCIBIOS_DEVICE_NOT_FOUND;
       	PCI_CFG_SET(dev, where);
       	*PCI_CFG_DATA = val;
       	return PCIBIOS_SUCCESSFUL;
}

static int qube_pci_write_config_word(struct pci_dev *dev, int where, u16 val)
{
	unsigned long tmp;

       	if (where & 0x1)
               	return PCIBIOS_BAD_REGISTER_NUMBER;
       	if (pci_range_ck (dev))
               	return PCIBIOS_DEVICE_NOT_FOUND;
       	PCI_CFG_SET(dev, (where & ~0x3));
       	tmp = *PCI_CFG_DATA;
       	tmp &= ~(0xffff << ((where & 0x3) * 8));
       	tmp |=  (val << ((where & 0x3) * 8));
       	*PCI_CFG_DATA = tmp;
       	return PCIBIOS_SUCCESSFUL;
}

static int qube_pci_write_config_byte(struct pci_dev *dev, int where, u8 val)
{
	unsigned long tmp;

       	if (pci_range_ck (dev))
               	return PCIBIOS_DEVICE_NOT_FOUND;
       	PCI_CFG_SET(dev, (where & ~0x3));
       	tmp = *PCI_CFG_DATA;
       	tmp &= ~(0xff << ((where & 0x3) * 8));
       	tmp |=  (val << ((where & 0x3) * 8));
       	*PCI_CFG_DATA = tmp;
       	return PCIBIOS_SUCCESSFUL;
}

struct pci_ops qube_pci_ops = {
       	qube_pci_read_config_byte,
       	qube_pci_read_config_word,
       	qube_pci_read_config_dword,
       	qube_pci_write_config_byte,
       	qube_pci_write_config_word,
       	qube_pci_write_config_dword
};

void __init pcibios_init(void)
{
       	printk("PCI: Probing PCI hardware\n");

       	ioport_resource.start = 0x00000000;
       	ioport_resource.end = 0x1cffffff;

       	pci_scan_bus(0, &qube_pci_ops, NULL);
}

char *pcibios_setup(char *str)
{
       	return str;
}

int pcibios_enable_device(struct pci_dev *dev)
{
       	u16 cmd, status;

       	pci_read_config_word(dev, PCI_COMMAND, &cmd);
       	pci_read_config_word(dev, PCI_STATUS, &status);
	printk("PCI: Enabling device %s (%04x  %04x)\n", dev->slot_name, cmd, status);
       	/* We'll sort this out when we know it isn't enabled ;) */
       	return 0;  /* pcibios_enable_resources */
}

void pcibios_align_resource(void *data, struct resource *res,unsigned long size)
{

       	panic("Uhhoh called pcibios_align_resource");
}

void pcibios_update_resource(struct pci_dev *dev, struct resource *root,
               			struct resource *res, int resource)
{
       	panic("Uhhoh called pcibios_update_resource");
}

void __init pcibios_fixup_bus(struct pci_bus *bus)
{
       	/* We don't appear to have sub-busses to fixup here */
}

unsigned __init int pcibios_assign_all_busses(void)
{
       return 1;
}
#endif /* CONFIG_PCI */
