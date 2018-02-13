#include <linux/module.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <linux/time.h>
#include <linux/kernel.h>             
#include <linux/fs.h>
#include <linux/uaccess.h>        /* for copy_from_user and copy_to_user */
#include <linux/time.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/kdev_t.h>
#include<linux/init.h>
#include "timer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////

#define DEVICE_NAME "timer0"
#define CONTROL_REG 0x43
#define CHANNEL0 0x40

//////////////////////////////////////////////////////////////////////////////////////////////////////


static dev_t first; // variable for device number
static struct cdev c_dev; // variable for the character device structure
static struct class *cls;

/////////////////////////////////////////////////////////////////////////////////////////////////////


struct timespec ts,ts_end;  // for showing the timestamp in log



///////////////////////////////////////////////////////////////////////////////////////////////////////



static int timer0_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "timer0 : open()\n");
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
 
static int timer0_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "timer0 : close()\n");
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////


static ssize_t timer0_read(struct file *f, char __user *arg, size_t len, loff_t *off)     
{
  int high,low;
  int current_count =0x00;
////////////////********************** Read Back Current Count Value ********************//////////////////


	outb_p(0x00, CONTROL_REG);  //counter latch command for channel 0
	low = inb(CHANNEL0);   //For lowbyte
	high = inb(CHANNEL0);  // For highbyte
	current_count=high;
	current_count=current_count<<8;          //Left shift by 8 for highbyte
	current_count+=low;
	
	if (copy_to_user((int *)arg, &current_count, sizeof(u16)))  //copy the value to user space
			{
				return -EACCES;
			}
	
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
 
static ssize_t timer0_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "timer0 : write()\n");
    return len;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

static long timer0_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	int sleep_time;
	int i;
	//u16 current_count;
	u8 status;


	switch (cmd)
	{
		case IOCTL_DELAY:
			if (copy_from_user(&sleep_time, (int *)arg, sizeof(int)))
				{
				return -EACCES;
				}
			
			else
			    {
					 
					 outb_p(0x30,CONTROL_REG);   //Set proper mode (mode 0 is set here)  
					 printk(KERN_INFO "sleep time given by user:%d second(s)\n\n", (sleep_time/1000));
					 getnstimeofday(&ts);

					 printk(KERN_INFO "TIME: %.2lu:%.2lu:%.2lu:%.3lu Entering sleep mode\n",
                   (ts.tv_sec / 3600) % (24),
                   (ts.tv_sec / 60) % (60),
                   ts.tv_sec % 60,
                   ts.tv_nsec / 1000000);   //Current Timestamp



					outb_p(0xAA,CHANNEL0);   //lowbyte
					outb_p(0x04,CHANNEL0);   //highbyte

				  	for( i=1;i<sleep_time;i++)
					{
					
					while(1)
						{	
						outb_p(0xE2, CONTROL_REG);  //Read back command for channel 0
						status = inb(CHANNEL0);
						if(status & 0x80)           //If output becomes 1 that means 1 ms delay is completed
							break;              // So break the loop
						}

					//Set value(0x04AAH) for 1ms delay

					outb_p(0x30,CONTROL_REG);
					outb_p(0xAA,CHANNEL0);   //lowbyte
					outb_p(0x04,CHANNEL0);   //highbyte

					}
					getnstimeofday(&ts_end);
					 printk(KERN_INFO "TIME: %.2lu:%.2lu:%.2lu:%.3lu Exiting sleep mode\n",
                   (ts_end.tv_sec / 3600) % (24),
                   (ts_end.tv_sec / 60) % (60),
                   ts_end.tv_sec % 60,
                   ts_end.tv_nsec / 1000000);
				}

  
			break;
		
		default:
			return -EINVAL;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct file_operations Fops = {
	.owner = THIS_MODULE,
        .read = timer0_read,
        .write = timer0_write,
        .unlocked_ioctl = timer0_ioctl,
        .open = timer0_open,
        .release = timer0_close,   
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



static int __init timer0_init(void)   //Constructor
{
    struct device *dev_ret;
    printk(KERN_INFO "Hello: Timer0 driver registered\n");
	
     
    //  reserve <major, minor>
    if (alloc_chrdev_region(&first, 0, 1, "timer0") < 0)
    {
        return -1;
    }
	
     
    // dynamically create device node in /dev directory
    if (IS_ERR(cls = class_create(THIS_MODULE, "timer0_class")))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cls);
    }
	
	
    if (IS_ERR(dev_ret = device_create(cls, NULL, first, NULL, "timer0")))
    {
        class_destroy(cls);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(dev_ret);
    }
     
	 
    // Link fops and cdev to device node
    cdev_init(&c_dev, &Fops);
	
    if (cdev_add(&c_dev, first, 1) < 0)
    {
        device_destroy(cls, first);
        class_destroy(cls);
        unregister_chrdev_region(first, 1);
        return -1;
    }
	
	
    return 0;
}
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void __exit timer0_exit(void)   //Destructor
{
    cdev_del(&c_dev);
    device_destroy(cls, first);
    class_destroy(cls);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "Bye: timer0 driver unregistered\n\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
module_init(timer0_init);
module_exit(timer0_exit);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sourima Sarkar");
MODULE_DESCRIPTION("Low Level IO mapped access to Timer");
