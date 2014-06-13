/*
 * $Id: avalanche-flash.c,v 1.1.1.1.6.1 2003/09/03 18:13:29 jaa Exp $
 *
 * Normal mappings of chips in physical memory
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/config.h>
#include <linux/mtd/partitions.h>
#include <asm/avalanche/generic/pal.h>

#define WINDOW_ADDR CONFIG_MTD_AVALANCHE_START
#define WINDOW_SIZE CONFIG_MTD_AVALANCHE_LEN
#define BUSWIDTH CONFIG_MTD_AVALANCHE_BUSWIDTH

#define AVALANCHE_FLASH2_START           0x1d000000


#if defined CONFIG_MIPS_TNETV1050SDB
#define NUM_OF_MTD_DEVICES  2
#else
#define NUM_OF_MTD_DEVICES  1
#endif

/* avalanche__partition_info gives details on the logical partitions that splits
 * the flash device into. If the size if zero we use up to the end of
 * the device. */
#define MAX_NUM_PARTITIONS 5


#include <asm/mips-boards/prom.h>

struct avalanche_chip_info
{
    char *name;
    int addr;
    int size;
    int buswidth;
};

struct avalanche_mtd
{
    struct avalanche_chip_info *avalanche_chip_info;
    struct mtd_partition avalanche_partition_info[MAX_NUM_PARTITIONS];
    struct mtd_info *avalanche_mtd_info; 
    struct map_info avalanche_map;
    int num_of_partitions;
};


extern char *prom_getenv(char *name);

static int create_mtd_partitions(void);
static void __exit avalanche_mtd_cleanup(void);
	



struct avalanche_chip_info avalanche_chip_info[NUM_OF_MTD_DEVICES] = {   
                                                                       { "Physically mapped flash:0",WINDOW_ADDR, WINDOW_SIZE, BUSWIDTH },
#if defined CONFIG_MIPS_TNETV1050SDB
                                                                       { "Physically mapped flash:1",AVALANCHE_FLASH2_START, WINDOW_SIZE, BUSWIDTH },
#endif
                                                                     };                                                                  



struct avalanche_mtd avalanche_mtd[NUM_OF_MTD_DEVICES];


int avalanche_mtd_ready=0;

__u8 avalanche_read8(struct map_info *map, unsigned long ofs)
{
	return __raw_readb(map->map_priv_1 + ofs);
}

__u16 avalanche_read16(struct map_info *map, unsigned long ofs)
{
	return __raw_readw(map->map_priv_1 + ofs);
}

__u32 avalanche_read32(struct map_info *map, unsigned long ofs)
{
	return __raw_readl(map->map_priv_1 + ofs);
}

void avalanche_copy_from(struct map_info *map, void *to, unsigned long from, ssize_t len)
{
	memcpy_fromio(to, map->map_priv_1 + from, len);
}

void avalanche_write8(struct map_info *map, __u8 d, unsigned long adr)
{
	__raw_writeb(d, map->map_priv_1 + adr);
	mb();
}

void avalanche_write16(struct map_info *map, __u16 d, unsigned long adr)
{
	__raw_writew(d, map->map_priv_1 + adr);
	mb();
}

void avalanche_write32(struct map_info *map, __u32 d, unsigned long adr)
{
	__raw_writel(d, map->map_priv_1 + adr);
	mb();
}

void avalanche_copy_to(struct map_info *map, unsigned long to, const void *from, ssize_t len)
{
	memcpy_toio(map->map_priv_1 + to, from, len);
}


int avalanche_flash_init(struct avalanche_mtd *dev)
{
	struct avalanche_chip_info *chip_info = dev->avalanche_chip_info;
	struct map_info * avalanche_map = &dev->avalanche_map;
	struct mtd_info * avalanche_mtd_info;
        
        avalanche_map->name = chip_info->name;
        avalanche_map->size = chip_info->size;
        avalanche_map->buswidth = chip_info->buswidth;
        avalanche_map->read8 = avalanche_read8;
        avalanche_map->read16 = avalanche_read16;
        avalanche_map->read32 = avalanche_read32;
        avalanche_map->copy_from = avalanche_copy_from;
        avalanche_map->write8 = avalanche_write8;
        avalanche_map->write16 = avalanche_write16;
        avalanche_map->write32 = avalanche_write32;
        avalanche_map->copy_to = avalanche_copy_to;

       	printk(KERN_NOTICE "avalanche flash device: 0x%lx at 0x%lx.\n",(unsigned long) chip_info->size, (unsigned long)chip_info->addr);
	avalanche_map->map_priv_1 = (unsigned long)ioremap_nocache(chip_info->addr, chip_info->size);

	if (!avalanche_map->map_priv_1) {
		printk("Failed to ioremap\n");
		return -EIO;
	}

	avalanche_mtd_info = do_map_probe("cfi_probe", avalanche_map);
	if (!avalanche_mtd_info)
	{
		printk("registering mtd failed\n");
		return -ENXIO;
	}

	dev->avalanche_mtd_info = avalanche_mtd_info;	
	avalanche_mtd_info->module = THIS_MODULE;
	

#if 0
	if (!(num_of_partitions = create_mtd_partitions(dev)));
		add_mtd_device(avalanche_mtd_info);
	else		
		add_mtd_partitions(avalanche_mtd_info, dev->avalanche_partition_info, num_of_partitions);

#endif	
	return 0;	
}


int __init avalanche_mtd_init(void)
{      
	int i;
	
	for(i=0;i <NUM_OF_MTD_DEVICES; i++)
	{
		int ret;

		avalanche_mtd[i].avalanche_chip_info = &avalanche_chip_info[i];
		ret = avalanche_flash_init(avalanche_mtd + i);	
		
		if(ret !=0)
			printk("failed initializing  flash dev %d\n",i);

	}

	create_mtd_partitions(); 
	/* Avalanche flash is initialized */
	avalanche_mtd_ready=1;

	return 0;
}


static char *strdup(char *str)
{
	int n = strlen(str)+1;
	char *s = kmalloc(n, GFP_KERNEL);
	if (!s) return NULL;
	return strcpy(s, str);
}

static int avalanche_check_mtd(struct avalanche_mtd *dev, unsigned int flash_base, unsigned int flash_end)
{
	int window_start = dev->avalanche_chip_info->addr;
        int window_size = dev->avalanche_chip_info->size;

	flash_base = (unsigned int)virt_to_bus((char *)flash_base);	
	flash_end = (unsigned int)virt_to_bus((char *)flash_end);

#if 0	
	printk("flash base = %x, flash_end = %x, window_start = %x, window_end = %x\n",flash_base,flash_end,window_start,window_size);
#endif

	if( (flash_base >= window_start && flash_base <= (window_start + window_size) )  &&
		 (flash_end >= window_start && flash_end <= (window_start + window_size)) )
        		return 1;

	return 0;

}

static void avalanche_add_partition(char * env_name, char * flash_base, char * flash_end)
{
	
	int i = 0;
	int found =0;
	int mtd_partition;
	int offset;
	int size;
		
	struct avalanche_mtd *dev =NULL;	
        struct mtd_partition *avalanche_partition_info;
        struct avalanche_chip_info *avalanche_chip_info;
 	struct mtd_info *avalanche_mtd_info;

	for(i = 0; i< NUM_OF_MTD_DEVICES; i++)
	{
		dev = &avalanche_mtd[i];
		if( !avalanche_check_mtd(dev,(unsigned int)flash_base, (unsigned int)flash_end))
		{
			continue;
		}
		found = 1;
		break;
	}
                        
 	if(!found)
		return;

       	avalanche_partition_info = dev->avalanche_partition_info;
        avalanche_chip_info = dev->avalanche_chip_info;
	avalanche_mtd_info = dev->avalanche_mtd_info;

	offset = virt_to_bus(flash_base) - avalanche_chip_info->addr;
	size = flash_end - flash_base;
	
	printk("Found a %s image (0x%x), with size (0x%x).\n",env_name, offset, size);

	/* Setup the partition info. We duplicate the env_name for the partiton name */
	mtd_partition = dev->num_of_partitions;
	avalanche_partition_info[mtd_partition].name = strdup(env_name);
	avalanche_partition_info[mtd_partition].offset = offset;
	avalanche_partition_info[mtd_partition].size = size;
	avalanche_partition_info[mtd_partition].mask_flags = 0;
	add_mtd_partitions(avalanche_mtd_info, &avalanche_partition_info[mtd_partition], 1);
	dev->num_of_partitions++;
}




static int create_mtd_partitions(void)
{
	unsigned char *flash_base;
	unsigned char *flash_end;
	char *env_ptr;
	char *base_ptr;
	char *end_ptr;
        int num_of_partitions = 0;

	
	do {
		char	env_name[20];


		/* get base and end addresses of flash file system from environment */
		sprintf(env_name, "mtd%1u", num_of_partitions);
		printk("Looking for mtd device :%s:\n", env_name);
		env_ptr = prom_getenv(env_name);
		if(env_ptr == NULL) {
			/* No more partitions to find */
			break;
		}

		/* Extract the start and stop addresses of the partition */
		base_ptr = strtok(env_ptr, ",");
		end_ptr = strtok(NULL, ",");
		if ((base_ptr == NULL) || (end_ptr == NULL)) {	
			printk("ERROR: Invalid %s start,end.\n", env_name);
			break;
		}

		flash_base = (unsigned char*) simple_strtol(base_ptr, NULL, 0);
		flash_end = (unsigned char*) simple_strtol(end_ptr, NULL, 0);
		if((!flash_base) || (!flash_end)) {
			printk("ERROR: Invalid %s start,end.\n", env_name);
			break;
		}
                
		avalanche_add_partition(env_name,flash_base,flash_end);
		num_of_partitions++;

	} while (num_of_partitions < MAX_NUM_PARTITIONS);

	return num_of_partitions;
}

static void __exit avalanche_mtd_cleanup(void)
{
	int i;

	avalanche_mtd_ready=0;
	for(i=0;i <NUM_OF_MTD_DEVICES; i++)
	{
		struct mtd_info *avalanche_mtd_info = avalanche_mtd[i].avalanche_mtd_info;		
		struct map_info *avalanche_map = &avalanche_mtd[i].avalanche_map;		

		if (avalanche_mtd_info) {
			del_mtd_partitions(avalanche_mtd_info);
			del_mtd_device(avalanche_mtd_info);
			map_destroy(avalanche_mtd_info);
		}

		if (avalanche_map->map_priv_1) {
			iounmap((void *)avalanche_map->map_priv_1);
			avalanche_map->map_priv_1 = 0;
		}
	}
}

module_init(avalanche_mtd_init);
module_exit(avalanche_mtd_cleanup);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Avalanche CFI map driver");
