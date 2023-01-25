#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include<linux/time.h>
#include<linux/rtc.h>
#include<linux/timekeeping.h>
MODULE_LICENSE("Dual BSD/GPL");

static char message[100];

struct rtc_time tm;

static int misc_open(struct inode* inode, struct file* filp){
    ktime_t curtime = ktime_get_real();
    printk(KERN_ALERT"Current time in seconds: %lld\n",curtime/1000000000);
    rtc_time_to_tm(curtime/1000000000-5*3600,&tm);
    sprintf(message, "Hello world, the current date is %02d/%02d/%d. The current time is %02d:%02d:%02d\n",tm.tm_mon+1,tm.tm_mday,tm.tm_year+1900,tm.tm_hour,tm.tm_min,tm.tm_sec);
	return 0;
}

static ssize_t misc_read(struct file* filp, char* buffer, size_t length, loff_t* ppos){
	int max_bytes;
	int bytes_to_read;
	
	// If *ppos equals to the size of the message, stop reading copying to buffer.
	if (*ppos == sizeof(message)){
		return 0;
	}
	
	max_bytes = sizeof(message) - *ppos;
	
	if (max_bytes < length){
		bytes_to_read = max_bytes;
	} else{
		bytes_to_read = length;
	}
	
	copy_to_user(buffer, message + *ppos, bytes_to_read);	// Send the message to user.
	*ppos += bytes_to_read;
	
	return bytes_to_read;
}

static int misc_release(struct inode* inode, struct file* filp){
	return 0;
}

static const struct file_operations fops = {
    .read = misc_read,
    .open = misc_open,
    .release = misc_release
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "lab3",
	.fops = &fops,
};

int misc_init(void){
	int register_return;
        
	if ((register_return = misc_register(&misc))){
		printk(KERN_ERR "Failed to register the device\n");
		return register_return;
	}
	return 0;
}

void misc_exit(void){
	misc_deregister(&misc);
}

module_init(misc_init)
module_exit(misc_exit)

