#include<linux/init.h>
#include<linux/module.h>

//int a=8,b=9,c=10;
int mod1_init(void)
 {
	printk(KERN_ALERT "Hello World!\n");
	//printk(KERN_ALERT "%d\n",(a*b*c));
	
	return 0;
 }


void mod1_cleanup(void)
 {
	printk(KERN_ALERT "Bye!! :(\n");
	
	return 0;
 }

module_init(mod1_init);
module_exit(mod1_cleanup);

