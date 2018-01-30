#include <linux/kernel.h>       
#include <linux/module.h>       
#include <linux/fs.h>
#include <linux/uaccess.h>        /* for copy_from_user and copy_to_user */
#include <linux/time.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/random.h>      /* Random no generation---> rand_int_get() */
#include <linux/kdev_t.h>
#include<linux/init.h>
#include "adc.h"
#define DEVICE_NAME "adc0"



static dev_t first; // variable for device number
static struct cdev c_dev; // variable for the character device structure
static struct class *cls;





static int adc0_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "adc0 : open()\n");
    return 0;
}
 
static int adc0_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "adc0 : close()\n");
    return 0;
}

static ssize_t adc0_read(struct file *f, char __user *buf, size_t len, loff_t *off)     //We can implement the required functionality without
{
    printk(KERN_INFO "adc0 : read()\n");                                                //Read and Write functions here
    return 0;
}
 
static ssize_t adc0_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "adc0 : write()\n");
    return len;
}

static long adc0_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	adc_arg_t q;
	int tmp;

	switch (cmd)
	{
		case IOCTL_SET_CHANNEL:
			if (copy_from_user(&q, (adc_arg_t *)arg, sizeof(adc_arg_t)))
			{
				return -EACCES;
			}
		

   //Random no genration

		while(1)                  //As we need only positive values
	{
		tmp = get_random_int();
		q.value = tmp % 1023;
		if(q.value >= 0)
		break;
	}

		if (copy_to_user((adc_arg_t *)arg, &q, sizeof(adc_arg_t)))
			{
				return -EACCES;
			}
			break;
		
		default:
			return -EINVAL;
	}

	return 0;
}






struct file_operations Fops = {
	.owner = THIS_MODULE,
        .read = adc0_read,
        .write = adc0_write,
        .unlocked_ioctl = adc0_ioctl,
        .open = adc0_open,
        .release = adc0_close,   
};



static int __init adc0_init(void)   //Constructor
{
    struct device *dev_ret;
    printk(KERN_INFO "Hello: adc driver registered\n");
     
    //  reserve <major, minor>
    if (alloc_chrdev_region(&first, 0, 1, "adc0") < 0)
    {
        return -1;
    }
     
    // dynamically create device node in /dev directory
    if (IS_ERR(cls = class_create(THIS_MODULE, "adc0_class")))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cls);
    }
    if (IS_ERR(dev_ret = device_create(cls, NULL, first, NULL, "adc0")))
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
  


static void __exit adc0_exit(void)   //Destructor
{
    cdev_del(&c_dev);
    device_destroy(cls, first);
    class_destroy(cls);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "Bye: adc0 driver unregistered\n\n");
}
  
module_init(adc0_init);
module_exit(adc0_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sourima");
MODULE_DESCRIPTION("10 Bit ADC Driver");
