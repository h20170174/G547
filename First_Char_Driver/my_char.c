#include<linux/version.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>

static dev_t first;
int first_char_init(void)
 {
	printk(KERN_INFO "Registered\n");
	if(alloc_chrdev_region(&first,0,4,"My First Char Driver\n") <0)
	return -1;
	printk(KERN_INFO "Major no: %d, Minor no: %d\n",MAJOR(first),MINOR(first));
	return 0;
 }


void first_char_exit(void)
 {
	unregister_chrdev_region(first,4);
	printk(KERN_INFO" Unregistered :(\n");
	
	
	return 0;
 }

module_init(first_char_init);
module_exit(first_char_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sourima");
MODULE_DESCRIPTION("My First Char Device Driver");
