#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>
struct Timer
{
unsigned long start;
unsigned long end;
};

struct Timer t;

/* This function is called when the module is loaded. */
int simple_init(void)
{
t.start = jiffies;
printk(KERN_INFO "Hello world %lu\n", (t.start*1000)/HZ);
return 0;
}
/* This function is called when the module is removed. */
void simple_exit(void)
{
t.end = jiffies;
printk(KERN_INFO "Goodbye cruel world %lu\n", ((t.end-t.start)*1000)/HZ);
}
/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("Dual BSD/GPL");
