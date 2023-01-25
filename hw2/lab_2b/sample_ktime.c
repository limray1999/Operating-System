#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>
struct Timer
{
ktime_t start;
ktime_t end;
};

struct Timer t;

/* This function is called when the module is loaded. */
int simple_init(void)
{
t.start = ktime_get_boottime();
printk(KERN_INFO "Hello world %llu\n", t.start/1000000);
return 0;
}
/* This function is called when the module is removed. */
void simple_exit(void)
{
t.end = ktime_get_boottime();
printk(KERN_INFO "Goodbye cruel world %llu\n", (t.end-t.start)/1000000);
}
/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("Dual BSD/GPL");
