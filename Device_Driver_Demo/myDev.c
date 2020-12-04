/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for put_user */

// module attributtes
MODULE_LICENSE("GPL"); // this avoids kernel traint warning
MODULE_DESCRIPTION("Device Driver Demo");
MODULE_AUTHOR("Santosh Kumar Mishra");

static char msg[100] = {0};
static short readPos = 0;
static short times = 0;
static int major;		/* Major number assigned to our device driver */

// prototypes, else the structure initialization that follows fail
static int dev_open(struct inode *, struct file *);
static int dev_rls(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *filp, const char *buff, size_t len, loff_t *f_pos);

// structure containing callbacks
static struct file_operations fops = {
	.read = dev_read, // address of dev_read
	.write = dev_write, // address of dev_write
	.open = dev_open, // address of dev_open
	.release = dev_rls, // address of dev_rls
};

// called when module is loaded, similar to main()
int init_module(void) {
	
	major = register_chrdev(0, "myDev", &fops); // register driver

	if (major < 0) {
		printk(KERN_ALERT "Device registration failed...\n");
	} else {
		printk(KERN_ALERT "Device registered...\n");
	}

	return major;
}

// called when module is unloaded, similar to destructor in OOP
void cleanup_module(void) {

	unregister_chrdev(major, "myDev");
}

// called when 'open' system call is done on the device file
static int dev_open(struct inode *inode, struct file *filp) {

	times++;
	printk(KERN_ALERT "Device opened %d times\n", times);
	return 0;
}

// called when 'read' system call is done on the device file
static ssize_t dev_read(struct file *filp, char *buff, size_t len, loff_t *off) {

	short count = 0;
	while (len && (msg[readPos] != 0)) {

		put_user(msg[readPos], buff++); // copy byte from kernel space to user space
		count++;
		len--;
		readPos++;
	}
	return count;
}

// called when 'write' system call is done on the device file
static ssize_t dev_write(struct file *filp, const char *buff, size_t len, loff_t *f_pos) {

	short ind = len - 1;
	short count = 0;
	memset(msg, 0, 100);

	while (len > 0) {

		msg[count++] = buff[ind--]; // copy the given string to the driver but in reverse 
		len--;
	}
	return count;
}

// called when 'close' system call is done on the device file
static int dev_rls(struct inode *inod, struct file *filp) {

	printk(KERN_ALERT "Device closed\n");
	return 0;
}

















