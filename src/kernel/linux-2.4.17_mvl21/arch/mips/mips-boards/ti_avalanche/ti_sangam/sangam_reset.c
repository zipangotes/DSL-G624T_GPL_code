#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/spinlock.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/timer.h>
#include <linux/module.h>

#include <asm/avalanche/generic/pal.h>

#define AR7_RESET_FILE "led_mod/ar7reset"
#define AR7_RESET_GPIO 11
#define RESET_POLL_TIME 1
#define RESET_HOLD_TIME 4

static struct proc_dir_entry *reset_file;
static int res_state = 0;
static int count;
static struct timer_list reset_timer, *pTimer = NULL;
static ssize_t proc_read_reset_fops(struct file *filp,
                                    char *buf,size_t count , loff_t *offp);

struct file_operations reset_fops = {
                                     read: proc_read_reset_fops
                                   };


static ssize_t proc_read_reset_fops(struct file *filp,
                                 char *buf,size_t count , loff_t *offp)
{
  char * pdata = NULL;
  char line[3];
  int len = 0;
  if( *offp != 0 )
  	return 0;

  pdata = buf;
	len = sprintf(line,"%d\n", res_state );
  res_state = 0;
  copy_to_user(buf,line,len );
	*offp = len;
	return len;
}

static void reset_timer_func(unsigned long data)
{
  count = (PAL_sysGpioInBit(AR7_RESET_GPIO) == 0) ? count + 1: 0;
  if( count >= RESET_HOLD_TIME/RESET_POLL_TIME )
    res_state = 1;
	pTimer->expires = jiffies + HZ*RESET_POLL_TIME;
	add_timer (pTimer);
  return;
}

void sangam_reset_init(void)
{


  /* Create board reset proc file */
  proc_mkdir("led_mod",NULL); 
  reset_file = create_proc_entry( AR7_RESET_FILE, 0777, NULL);
  if( reset_file == NULL)
    goto reset_file;
  reset_file->owner = THIS_MODULE;
  reset_file->proc_fops = &reset_fops;

  /* Initialise GPIO 11 for input */
  PAL_sysGpioCtrl(AR7_RESET_GPIO,GPIO_PIN,GPIO_INPUT_PIN);

  /* Create a timer which fires every seconds */
  pTimer = &reset_timer;
	init_timer( pTimer );
	pTimer->function = reset_timer_func;
	pTimer->data = 0;
  /* Start the timer */
  reset_timer_func(0);
  return ;

  reset_file:

    remove_proc_entry("AR7_RESET_FILE",NULL);
	return;
}









