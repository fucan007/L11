#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/io.h>


struct led_desc{
	unsigned int majorNo; 
	struct class *cls;
	struct device *dev; 
	void *base_addr; 
};


#define GPX1_CON 0x11000C20  
#define GPX1_SIZE 8



struct led_desc *led_dev;
static int kernel_val = 555;
static char *devname = "led_test";


ssize_t led_drv_read(struct file *filp, char __user *buf, size_t count, loff_t *fpos)
{
	
	return 0;

}
ssize_t led_drv_write(struct file *filp, const char __user *buf, size_t count, loff_t *fpos)
{
	int ret;
	int value;
	
	ret = copy_from_user(&value,  buf, count);
	if(ret > 0)
	{
		printk("copy_to_user error\n");
		return -EFAULT;
	}

	if(value){
		writel( readl(led_dev->base_addr + 4) | (1<<7),   led_dev->base_addr + 4 );
		
	}else{
		writel( readl(led_dev->base_addr + 4) & ~(1<<7),   led_dev->base_addr + 4 );
	}
	
	return 0;
	
}
int led_drv_open(struct inode *inode, struct file *filp)
{
	printk("-------%s-------\n", __FUNCTION__);
	
	return 0;
}


int led_drv_close(struct inode *inode, struct file *filp)
{
	printk("-------%s-------\n", __FUNCTION__);
	return 0;

}

const struct file_operations my_fops = {
	.open = led_drv_open,
	.read = led_drv_read,
	.write = led_drv_write,
	.release = led_drv_close,
};

static int __init led_dev_init(void)
{
	int ret;

	led_dev = kmalloc(sizeof(struct led_desc), GFP_KERNEL);
	if(led_dev == NULL)
	{
		printk(KERN_ERR "malloc error\n");
		return -ENOMEM;
	}

	led_dev->majorNo = register_chrdev(0, devname, &my_fops);
	if(led_dev->majorNo < 0)
	{
		printk(KERN_ERR "register_chrdev error\n");
		ret = -ENODEV;
		goto err_0;
	}

	led_dev->cls = class_create(THIS_MODULE, "led_cls");
	if(IS_ERR(led_dev->cls))
	{
		printk(KERN_ERR "class_create error\n");
		ret = PTR_ERR(led_dev->cls); 
		goto err_1;
	}

	
	led_dev->dev = device_create(led_dev->cls, NULL, 
				MKDEV(led_dev->majorNo, 0), NULL, "led%d", 0);
	if(IS_ERR(led_dev->dev))
	{
		printk(KERN_ERR "device_create error\n");
		ret = PTR_ERR(led_dev->dev); //将指针出错的具体原因转换成一个出错码
		goto err_2;
	}

	
	led_dev->base_addr = ioremap(GPX1_CON, GPX1_SIZE);
	if(led_dev->base_addr == NULL)
	{
		
		printk(KERN_ERR "ioremap error\n");
		ret = -ENOMEM;
		goto err_3;
	}
	u32 value = readl(led_dev->base_addr);
	value &= ~(0xf<<28);
	value |= (0x1<<28);
	writel(value, led_dev->base_addr);	
	
	return 0;
	

err_3:
	device_destroy(led_dev->cls, MKDEV(led_dev->majorNo, 0));

err_2:
	class_destroy(led_dev->cls);

err_1:
	unregister_chrdev(led_dev->majorNo, devname);

err_0:
	kfree(led_dev);
	return ret;

}

static void __exit led_dev_exit(void)
{
	iounmap(led_dev->base_addr);
	device_destroy(led_dev->cls, MKDEV(led_dev->majorNo, 0));
	class_destroy(led_dev->cls);
	unregister_chrdev(led_dev->majorNo, devname);
	kfree(led_dev);

}


module_init(led_dev_init);
module_exit(led_dev_exit);
MODULE_LICENSE("GPL");












