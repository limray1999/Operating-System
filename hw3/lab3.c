#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/rtc.h>

static char msg[200];
static char *msg_Ptr;
static int Device_Open = 0;  /* Used to prevent multiple access to the device */
//ktime_t curr_time;
int sec, hr, min;
int month, day, year;
struct rtc_time tm;

static int lab3_open(struct inode *inode, struct file *filp)
{
	if (Device_Open) return -EBUSY;
	Device_Open++;
	
	tm = rtc_ktime_to_tm(ktime_get_real());
    	sec = tm.tm_sec;
    	min = tm.tm_min;
    	hr = tm.tm_hour;
    	year = 1900+tm.tm_year;
    	month = tm.tm_mon;
    	day = tm.tm_mday;
    	//curr_time = ktime_get_real()/1000000000;
    	//sec = curr_time % 60;
    	//min = ((curr_time-sec) / 60) % 60;
    	//hr = ((curr_time-sec-min*60) / 3600) % 24;
    	//time64_to_tm(curr_time, 0, &tv);
    	//year = tv.tm_year+1900;
    	//month = tv.tm_mon;
    	//day = tv.tm_mday;
    	
	sprintf(msg,
		"Hello world, the current date is %d, %d %d. The current time is %d:%d:%d\n", 
		month,
		day,
		year,
		hr,
		min,
		sec);
	msg_Ptr = msg;
	printk(KERN_INFO "I am in\n");
	return 0;
}

static int lab3_close(struct inode *inode, struct file *filp)
{
	Device_Open--;
	printk(KERN_INFO "I am out\n");
	return 0;
}

static ssize_t lab3_read(struct file *filp, char __user *buff,
		       size_t size, loff_t *offp)
{
   	int bytes_read = 0;
   	while (size && *msg_Ptr)  {
         	put_user(*(msg_Ptr++), buff++);
         	size--;
         	bytes_read++;
   	}		
   	return bytes_read;
}		 

static ssize_t lab3_write(struct file *filp, const char __user *buff,
		       size_t size, loff_t *offp)
{
    	printk(KERN_INFO "Write operation isn't supported.\n");
   	return -EINVAL;
}

static const struct file_operations lab3_fops = {
    .owner = THIS_MODULE,
    .write = lab3_write,
    .open = lab3_open,
    .release = lab3_close,
    .read = lab3_read,
};

struct miscdevice lab3_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "lab3",
    .fops = &lab3_fops,
};

static int __init misc_init(void)
{
    int error;

    error = misc_register(&lab3_device);
    if (error) {
        pr_err("can't misc_register :(\n");
        return error;
    }

    pr_info("Hello World\n");
    return 0;
}

static void __exit misc_exit(void)
{
    misc_deregister(&lab3_device);
    pr_info("Goodbye World\n");
}

module_init(misc_init)
module_exit(misc_exit)
MODULE_LICENSE("GPL");
