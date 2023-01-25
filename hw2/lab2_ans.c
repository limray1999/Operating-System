#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/time.h>
#include<linux/jiffies.h>
MODULE_LICENSE("Dual BSD/GPL");

unsigned long start_a;
ktime_t start_b; 
static int hello_init(void)
{
    start_a = jiffies;
    start_b = ktime_get_boottime();// refer to the link about this function for more info
    // in LDD chapter7 as we follow the hint to search the jiffer and HZ in LDD books, we know that HZ refers to # of ticks inside a second, so it is easy to get the time for single tick in ms: 1000/HZ
    printk(KERN_ALERT"Hello,World\n");
    printk(KERN_ALERT"Insert tick time:%d ms\n",1000/HZ);
    return 0;
}

static void hello_exit(void)
{
    //Method a) Using the difference in the value of jiffies from inserting the module to removing the module
    unsigned long diff_a = (jiffies-start_a);
    
    //Method b) Using the time difference obtained by reading the timer
    ktime_t end = ktime_get_boottime();
    unsigned long diff_b = end - start_b;
   
    printk(KERN_ALERT"Goodbye,cruel world\n");
    
    // printk(KERN_ALERT"Time difference with a): %d.%d s\n",diff_a/1000,diff_a%1000);
    printk(KERN_ALERT"Time difference with a): %d ms\n",diff_a*1000/HZ);
    
    printk(KERN_ALERT"Time difference with b): %d ms\n",diff_b/1000000);
}

module_init(hello_init);
module_exit(hello_exit);
