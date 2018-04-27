#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/ctype.h>
#include <linux/leds.h>
#include <linux/slab.h> // for printk()
#include <linux/device.h> // for printk()
#include<linux/sched.h>


/*
	structure: triggerable_trig_data
	functionalities like timer can be added to this structure
*/
struct triggerable_trig_data {
//	struct timer_list timer;
};

/*
	function: led_triggerable_function
	initializzes brightness to 0	
*/
static void led_triggerable_function(unsigned long data)
{
	struct led_classdev *led_cdev = (struct led_classdev *) data;
	printk("\nled\n");
	led_set_brightness(led_cdev, LED_OFF);
}

/*
	function: trigger_store
	this function is passed to DEVICE_ATTR()	
*/
static ssize_t trigger_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct triggerable_trig_data *kdata = led_cdev->trigger_data;


	printk("\nstore\n");
	//delay = msecs_to_jiffies(200);

	//mod_timer(&kdata->timer, jiffies + delay);

	led_set_brightness(led_cdev, LED_FULL);

	return size;
}
static DEVICE_ATTR(trigger, 0200, NULL, trigger_store);

/*
	function: triggerable_trig_activate
	this function is called when the triggeris aactivated	
*/
static void triggerable_trig_activate(struct led_classdev *led_cdev)
{
	int p;
	struct triggerable_trig_data *triggerable_data;

	led_set_brightness(led_cdev, LED_FULL);
	triggerable_data = kzalloc(sizeof(*triggerable_data), GFP_KERNEL);
	if (!triggerable_data)
		return;
	printk("\nactivate0\n");
	led_cdev->trigger_data = triggerable_data;
	printk("\nactivate1\n");

	printk("\nactivate5\n");

	for(p=0;p<10;p++)
	{
		led_set_brightness(led_cdev, LED_OFF);
		printk("\nactivate6\n");
		mdelay(500);
		led_set_brightness(led_cdev, LED_FULL);
		printk("\nactivate6\n");
		mdelay(500);
	}
}

/*
	function: triggerable_trig_deactivate
	this function is called when the triggeris deactivated	
*/
static void triggerable_trig_deactivate(struct led_classdev *led_cdev)
{
	struct triggerable_trig_data *kdata = led_cdev->trigger_data;
	printk("\ndeactivate\n");
	if (led_cdev->activated) {
		//del_timer_sync(&kdata->timer);
		kfree(kdata);
	}

	led_set_brightness(led_cdev, LED_OFF);
}

static struct led_trigger triggerable_led_trigger = {
	.name     = "triggerable",
	.activate = triggerable_trig_activate,
	.deactivate = triggerable_trig_deactivate,
};

static int __init triggerable_trig_init(void)
{
	printk("\ninit\n");	
	return led_trigger_register(&triggerable_led_trigger);
}

static void __exit triggerable_trig_exit(void)
{
led_trigger_unregister(&triggerable_led_trigger);
}

module_init(triggerable_trig_init);
module_exit(triggerable_trig_exit);

MODULE_AUTHOR("Sourima Sarkar");
MODULE_DESCRIPTION("triggerable LED trigger");
MODULE_LICENSE("GPL");
