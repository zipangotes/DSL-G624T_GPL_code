/******************************************************************************    
 * FILE PURPOSE:     - LED driver module Source                                       
 ******************************************************************************    
 * FILE NAME:     led_drv.c                                                        
 *                                                                                 
 * DESCRIPTION:   Linux LED character driver implementation                                                  
 *                                                                                 
 * REVISION HISTORY:                                                               
 * 27 Aug 2003    Initial Creation                      Sharath Kumar<krs@ti.com>  
 * 
 * 16 Dec 2003    Updates for 5.7                       Sharath Kumar<krs@ti.com>       
 *
 * 07 Jan 2004    Wrapper for DSL                       Sharath Kumar<krs@ti.com>                                                   
 *                                                                                 
 * (C) Copyright 2002, Texas Instruments, Inc                                      
 *******************************************************************************/  
                                                                                   
#include <linux/config.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <linux/delay.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/wait.h>
#include <linux/proc_fs.h>
#include <asm/avalanche/generic/pal.h>

#include "led_config.h"
#include "led_hal.h"
#include "led_ioctl.h"
#include "led_platform.h"


#define TI_LED_VERSION          "0.1"
#define GPIO_MAP_LEN   ((MAX_GPIO_PIN_NUM/32) + 1)

static int gpio_off_state[GPIO_MAP_LEN] = AVALANCHE_GPIO_OFF_MAP;



static int led_ioctl( struct inode * inode, struct file * file,
                             unsigned int cmd, unsigned long arg )
{

    int ret = 0;
//    char name[80];

    switch ( cmd )
    {
        case LED_CONFIG:
        {
            LED_CONFIG_T led_cfg;
            if (copy_from_user((char *)&led_cfg, (char *)arg, sizeof(led_cfg)))
            {
                ret = -EFAULT;
                break;
            }
            ret = avalanche_led_config_set(&led_cfg);
        }
        break;

        case LED_GET_HANDLE:
        {
            LED_MODULE_T led_mod;
            int handle;

            if (copy_from_user((char *)&led_mod, (char *)arg, sizeof(led_mod)))
            {
                ret =  -EFAULT;
                break;
            }

            handle = (int)avalanche_led_register(led_mod.name,led_mod.instance);

            if (copy_to_user((char *)(&(((LED_MODULE_T *)arg)->handle)), (char *)(&handle), sizeof(int)))
            {
                ret =  -EFAULT;
                break;
            }

            if(handle)
                ret = 0;
            else
                ret = -1;
        }
        break;

        case LED_ACTION:
        {
            LED_STATE_T led_state;

            if (copy_from_user((char *)&led_state, (char *)arg, sizeof(led_state)))
            {
                ret =  -EFAULT;
                break;
            }
            else
                avalanche_led_action((void *)led_state.handle,led_state.state_id);
        
                        
        }
        break;

        case LED_RELEASE_HANDLE:
            ret = avalanche_led_unregister((void *)arg);
        break;

        default:
            ret = -EINVAL;
    }
    return ret;

}

static int led_open( struct inode * inode, struct file * file )
{
    return 0;
}

static int led_close( struct inode * inode, struct file * file )
{
    return 0;
}

struct file_operations led_fops = {
    ioctl:    led_ioctl,
    open:    led_open,
    release:  led_close
};


/* Proc function to display driver version */                                                                       
static int                                                                                     
led_ver_info(char *buf, char **start, off_t offset, int count, int *eof, void *data)        
{                                                                                              
//	int instance;                                                                              
	int len=0;                                                                                 
                                                                                               
	len += sprintf(buf +len,"\nTI Linux LED Driver Version %s\n",TI_LED_VERSION);         
	return len;                                                                                
}                                                                                              


/* proc interface /proc/avalanche/led  */
int led_cfg_info(char* buf, char **start, off_t offset, int count,
                              int *eof, void *data)
{
    int mod_count = 0;
    int len=0;
    int limit = count - 80;
    char *msg[5]={"LED OFF", "LED_ON", "LED_ONESHOT_OFF", "LED_ONESHOT_ON","LED_FLASH"};

    for(mod_count = 0;mod_count<MAX_MODULE_ENTRIES;mod_count++)
    {
        int instance;

        for(instance = 0; instance < MAX_MODULE_INSTANCES; instance++)
        {  
            int state_id;
            int flag = 0;
          
            for(state_id =0 ;state_id < MAX_STATE_ENTRIES; state_id++)
            {
                LED_CONFIG_T led_cfg;
                if(avalanche_led_config_get(&led_cfg,mod_count,instance,state_id) == 0)
                { 
                    if(flag == 0)
                    {
                        flag = 1;
                        if(len <= limit)
                            len+= sprintf(buf+len, "\nModule:%s\n",led_cfg.name);
                        
                        if(len <= limit)
                            len+= sprintf(buf+len, "\n    Instance:%d\n",instance);
                        
                    }
                    
                    if(len <= limit)
                        len+= sprintf(buf+len, "\n        State: %d\n",state_id);
                    
                    if(len <= limit)
                        len+= sprintf(buf+len, "\n            mode: %s\n",msg[led_cfg.mode[0]% 5 ]);
                    
                    if(len <= limit)
                        len+= sprintf(buf+len, "\n            gpio: %d\n",led_cfg.gpio[0]);
                    
                    if(len <= limit)
                        len+= sprintf(buf+len, "\n            param1: %d\n",led_cfg.param1);
                    
                    if(len <= limit)
                        len+= sprintf(buf+len, "\n            param2: %d\n",led_cfg.param2);
                }
            }
        }
    }       
       
                                
                        
    return len;
}    



int __init led_init(void)
{

    /* register LED device */
    devfs_register(NULL, "led", DEVFS_FL_AUTO_DEVNUM, 0, 0,
                            S_IFCHR | S_IRUGO | S_IWUGO, &led_fops,NULL);


    avalanche_led_hal_init(gpio_off_state,AVALANCHE_GPIO_PIN_COUNT);

    /* create proc entry */
    create_proc_read_entry("avalanche/led_cfg", 0, NULL, led_cfg_info, NULL);
    create_proc_read_entry("avalanche/led_ver", 0, NULL, led_ver_info, NULL);

    return 0;
}

void led_exit(void)
{
    avalanche_led_hal_exit();
    return;
}

module_init(led_init);
module_exit(led_exit);


EXPORT_SYMBOL_NOVERS(avalanche_led_register);
EXPORT_SYMBOL_NOVERS(avalanche_led_action);
EXPORT_SYMBOL_NOVERS(avalanche_led_unregister);
EXPORT_SYMBOL_NOVERS(avalanche_led_config_set);
EXPORT_SYMBOL_NOVERS(avalanche_led_config_get);
