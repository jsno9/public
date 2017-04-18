#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

#include "helloworld_notify.h"

static struct proc_dir_entry *helloworld_proc_file;
static struct class *helloworld_class;
static int chr_num;
static struct kobject *helloworldobj;

struct hello_notify_event{
	void *data;
};

static int helloworld_chain_callback1(struct notifier_block *nb,unsigned long val, void *data)
{
	struct hello_notify_event *event11=data;
	unsigned int a;
	a = *(int *)(event11->data);
	printk("helloworld callback 1\n");
	printk("val= %ld,a= %d\n",val,a);
	return 1;
}

static int helloworld_chain_callback2(struct notifier_block *nb,unsigned long val, void *data)
{
	struct hello_notify_event *event12=data;
	unsigned int a;
	a = *(int *)(event12->data);
	printk("helloworld callback 2\n");
	printk("val= %ld,a= %d\n",val,a);
	return 1;
}

static struct notifier_block helloworld_noti_block1 = {
	.notifier_call = helloworld_chain_callback1,
};

static struct notifier_block helloworld_noti_block2 = {
	.notifier_call = helloworld_chain_callback2,
};


static int helloworld_dev_open(struct inode *inode, struct file *filp)
{
	printk("hellowrld_dev_open\n");
	return 0;
}

static long helloworld_dev_ioctl(struct file *file, unsigned int cmd,unsigned long arg)
{
	printk("hellowrld_dev_ioctl\n");
	return 0;
}

static ssize_t helloworld_dev_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	printk("helloworld_dev_read\n");
	return 0;
}

static ssize_t helloworld_dev_write(struct file *filp, const char __user *buff, size_t len, loff_t *data)
{
	int temp;
	char messages[256];
	struct hello_notify_event event1;
	int a=10;
	event1.data=&a;

	if(len>256) len=256;
	if (copy_from_user(messages, buff, len)) return -EFAULT;
	sscanf(buff,"%d", &temp);

	hello_notifier_call_chain(2,&event1);
	printk("helloworld_dev_write\n");
	return len;
}

static int helloworld_proc_read(struct seq_file *buf, void *v)
{
	seq_printf(buf, "helloworldproc\n");
	return 0;
}
static int helloworld_proc_open(struct inode *inode, struct  file *file)
{
    	return single_open(file, helloworld_proc_read, NULL);
		
}

static struct file_operations helloworld_proc_ops = {
	.open = helloworld_proc_open,
	.read = seq_read,
	.release = single_release,
};

static const struct file_operations hellowrld_dev_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = helloworld_dev_ioctl,
	.open = helloworld_dev_open,
	.read	= helloworld_dev_read,
	.write	= helloworld_dev_write,

};

static ssize_t helloworld_obj_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf)
{
	printk("hello helloworld_obj\n");
	return 0;
}

static struct kobj_attribute helloobj = __ATTR_RO(helloworld_obj);//RO表示read属性



static struct attribute *attrs[] = {
	&helloobj.attr,
	NULL
};
static struct attribute_group attr_group = {
	.attrs = attrs,
};


static int __init helloworld_init(void)
{
	int err;	
	
	printk("helloworld init start\n");
	helloworld_proc_file = proc_create("helloworldproc", 0666,NULL, &helloworld_proc_ops);

	chr_num = register_chrdev(0,"helloworlddev",&hellowrld_dev_fops);
	if (chr_num < 0) {
		printk("helloworld Failed to register char device!\n");
	}
	
	helloworld_class = class_create(THIS_MODULE , "helloworldclass");
	if (IS_ERR(helloworld_class)) {
		unregister_chrdev(chr_num, "helloworlddev");
		printk("Failed to create class.\n");
		return PTR_ERR(helloworld_class);
	}
	device_create(helloworld_class, NULL, MKDEV(chr_num, 0),NULL,"helloworld0");

	helloworldobj = kobject_create_and_add("helloworld1", kernel_kobj);
	err = sysfs_create_group(helloworldobj, &attr_group);	

	hello_register_client(&helloworld_noti_block1);
	hello_register_client(&helloworld_noti_block2);
	
	
	printk("helloworld init end\n");
	return 0;
}

static void __exit helloworld_exit (void)
{
	dev_t devno = MKDEV(chr_num, 0);
	device_destroy(helloworld_class, MKDEV(chr_num, 0));         //delete device node under /dev
	class_destroy(helloworld_class);                               //delete class created by us
	unregister_chrdev(devno, "helloworlddev");
	printk ("helloworld_exit\n");
}
module_init(helloworld_init);
module_exit(helloworld_exit);

