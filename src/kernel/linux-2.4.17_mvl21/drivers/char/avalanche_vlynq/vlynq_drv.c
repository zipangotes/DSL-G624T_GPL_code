/******************************************************************************
 * FILE PURPOSE:    Vlynq Linux Device Driver Source
 ******************************************************************************
 * FILE NAME:       vlynq_drv.c
 *
 * DESCRIPTION:     Vlynq Linux Device Driver Source
 *
 * REVISION HISTORY:
 *
 * Date           Description                       Author
 *-----------------------------------------------------------------------------
 * 17 July 2003   Initial Creation                  Anant Gole
 * 17 Dec  2003   Updates                           Sharath Kumar
 * 12 Oct  2004   Revamped for PF 1.0 VLYNQ         Suraj Iyer
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 *******************************************************************************/
 
#include <linux/config.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/smp_lock.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/capability.h>
#include <asm/avalanche/generic/pal_vlynq.h>
#include <asm/avalanche/generic/avalanche_intc.h>


#define    TI_VLYNQ_VERSION                 "0.4"

#define    ERRPRINT   printk

/* Define the max vlynq ports this driver will support. 
   Device name strings are statically added here */
#define MAX_VLYNQ_PORTS 2

static int
vlynq_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    return 0;
}

static struct file_operations vlynq_fops = {
    owner:      THIS_MODULE,
    ioctl:      vlynq_ioctl,
};

/* Vlynq device object */
static struct miscdevice vlynq_miscdev [MAX_VLYNQ_PORTS] = {
    { MISC_DYNAMIC_MINOR , "vlynq0", &vlynq_fops },
    { MISC_DYNAMIC_MINOR , "vlynq1", &vlynq_fops },
};

static int vlynq_read_all_vlynq(char *buf, char **start, off_t offset, 
                                int count, int *eof, void *data)
{
    return(PAL_vlynqDump(NULL, PAL_VLYNQ_DUMP_ALL_ROOT, buf, count - 80, eof));  
}

static int vlynq_read_registers(char *buf, char **start, off_t offset, 
                                int count, int *eof, void *data)
{
    return(PAL_vlynqDump(data, PAL_VLYNQ_DUMP_ALL_REGS, buf, count - 80, eof));
}

static int vlynq_read_raw(char *buf, char **start, off_t offset, 
                          int count, int *eof, void *data)
{
    return(PAL_vlynqDump(data, PAL_VLYNQ_DUMP_RAW_DATA, buf, count - 80, eof));
}

static int vlynq_read_status_word(char *buf, char **start, off_t offset, 
                                  int count, int *eof, void *data)
{
    return(PAL_vlynqDump(data, PAL_VLYNQ_DUMP_STS_REG, buf, count - 80, eof));
}

static int vlynq_read_control_word(char *buf, char **start, off_t offset, 
                                   int count, int *eof, void *data)
{
    return(PAL_vlynqDump(data, PAL_VLYNQ_DUMP_CNTL_REG, buf, count - 80, eof));
}


/* Proc read function */
static int vlynq_read_link_proc(char *buf, char **start, off_t offset, 
                                int count, int *eof, void *data)
{
    int len = 0;
    int root, root_count;

    char *link_msg[] = {" DOWN "," UP "};

    root_count = PAL_vlynqGetNumRoot();    

    for(root = 0; root < root_count; root++)
    {
        int hop  = 0;
        int link;
        PAL_VLYNQ_HND *p_vlynq_hnd;

        len += sprintf(buf + len, "Vlynq Root 0:\n");
        p_vlynq_hnd = PAL_vlynqGetRoot(root);
         
        if(!p_vlynq_hnd)
            continue;
       
        do
        {
          link = PAL_vlynqGetLinkStatus(p_vlynq_hnd);

          len += sprintf(buf + len, "  Vlynq Bridge %d%d: %s\n", root, hop, link_msg[link]);

          p_vlynq_hnd = PAL_vlynqGetNext(p_vlynq_hnd);

        } while(link && !PAL_vlynqIsLast(p_vlynq_hnd));
    }

    return len;
}


/* Proc function to display driver version */                                                                       
static int                                                                                     
vlynq_read_ver_proc(char *buf, char **start, off_t offset, int count, int *eof, void *data)        
{                                                                                              
	int len=0;                                                                                 
                                                                                               
	len += sprintf(buf +len,"\nTI Linux VLYNQ Driver Version %s\n",TI_VLYNQ_VERSION);         
	return len;                                                                                
}                                                                                              


struct proc_dir_entry *vlynq_proc_root;

/* Wrapper for vlynq ISR */
static void lnx_vlynq_root_isr(int irq, void * arg, struct pt_regs *regs)
{
   PAL_vlynqRootIsr(arg);
}

int vlynq_init_module(void)
{
    int root_count, root;
    int instance_count = CONFIG_MIPS_AVALANCHE_VLYNQ_PORTS;

    /* If num of configured vlynq ports > supported by driver return error */
    if (instance_count > MAX_VLYNQ_PORTS)
    {
        ERRPRINT("ERROR: vlynq_init_module(): Max %d supported while configured for %d.\n", MAX_VLYNQ_PORTS,
                  instance_count);
        return (-1);
    }

    vlynq_proc_root = proc_mkdir("bus/vlynq",NULL);
    if(!vlynq_proc_root)
        return -ENOMEM;

    create_proc_read_entry("vlynq_data", 0, vlynq_proc_root, vlynq_read_all_vlynq, NULL);

    root_count = PAL_vlynqGetNumRoot( );
    if(!root_count)
        return (-1);

    for(root = 0; root < root_count; root++)
    {
        PAL_VLYNQ_HND *p_root_vlynq, *p_vlynq_hnd = PAL_vlynqGetRoot(root);
        char name[50];
        int hop = 0, irq;
        unsigned int base_addr;
   
        if(!p_vlynq_hnd)
            continue;	

        if(!PAL_vlynqGetLinkStatus(p_vlynq_hnd))
            continue;

        if(PAL_vlynqGetBaseAddr(p_vlynq_hnd, &base_addr))
            continue;

        if(base_addr == AVALANCHE_LOW_VLYNQ_CONTROL_BASE)
            irq = LNXINTNUM(AVALANCHE_LOW_VLYNQ_INT);
        else if(base_addr == AVALANCHE_HIGH_VLYNQ_CONTROL_BASE)
            irq = LNXINTNUM(AVALANCHE_HIGH_VLYNQ_INT);
        else
            continue;

        p_root_vlynq = p_vlynq_hnd;

        for(; ;p_vlynq_hnd = PAL_vlynqGetNext(p_vlynq_hnd))
        {
            sprintf(name, "vlynq%d%d_regs", root, hop);
            create_proc_read_entry(name, 0, vlynq_proc_root, vlynq_read_registers, p_vlynq_hnd);

            sprintf(name, "vlynq%d%d_raw", root, hop);
            create_proc_read_entry(name, 0, vlynq_proc_root, vlynq_read_raw, p_vlynq_hnd);
        
            sprintf(name, "vlynq%d%d_status", root, hop);
            create_proc_read_entry(name, 0, vlynq_proc_root, vlynq_read_status_word, p_vlynq_hnd);

            sprintf(name, "vlynq%d%d_control", root, hop);
            create_proc_read_entry(name, 0, vlynq_proc_root, vlynq_read_control_word, p_vlynq_hnd);

            hop++;
   
            if(PAL_vlynqIsLast(p_vlynq_hnd))
                break;
        }

        if(misc_register(&vlynq_miscdev[root]) < -1)
            continue;

        request_irq(irq, lnx_vlynq_root_isr, 0, vlynq_miscdev[root].name, p_root_vlynq);

        printk("Installed misc driver %s: it handles vlynq bridge%d with %d hop(s).\n", 
                vlynq_miscdev[root].name, root, hop);
    }

    /* Creating proc entry for the devices */
    create_proc_read_entry("avalanche/vlynq_link", 0, NULL, vlynq_read_link_proc, NULL);
    create_proc_read_entry("avalanche/vlynq_ver",  0, NULL, vlynq_read_ver_proc, NULL);

    return 0;
}

void vlynq_cleanup_module(void)
{
    /* 
     * Note: This is not complete. It is ok for now as this 
     * driver is built into the kernel. */

    int unit = 0;
    
    for (unit = 0; unit < CONFIG_MIPS_AVALANCHE_VLYNQ_PORTS; unit++)
    {
        printk("vlynq_cleanup_module(): Unregistring misc device %s\n",vlynq_miscdev[unit].name);
        misc_deregister(&vlynq_miscdev[unit]);
    }

    remove_proc_entry("avalanche/vlynq_link", NULL);
    remove_proc_entry("avalanche/vlynq_ver", NULL);
}


module_init(vlynq_init_module);
module_exit(vlynq_cleanup_module);

