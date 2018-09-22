#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");

static int data = 55;

const int MAJORNO = 241;
static char *mydevname = "test_dev";
struct test_dev{
	int    majorNo;
	struct class *devcls;
	struct device *dev;
};


struct test_dev *my_dev;


const struct file_operations test_fops = {
	
};



static int __init test_dev_init(void)
{
	int ret;
	my_dev = kzalloc(sizeof(struct test_dev),  GFP_KERNEL);
	if(my_dev == NULL)
	{
		printk("kmalloc failed\n");
		return -ENOMEM;
	}
	my_dev->majorNo = MAJORNO;
	ret = register_chrdev(my_dev->majorNo, mydevname, &test_fops);
	if(ret == 0){
		printk("register ok\n");
	}else{
		printk("register failed\n");
		return -EINVAL;
	}

	my_dev->devcls = class_create(THIS_MODULE, "test_char");
	my_dev->dev = device_create(my_dev->devcls, NULL, MKDEV(my_dev->majorNo, 0), NULL, "test_chr");
	
	
	return 0;
}

static void __exit test_dev_exit(void)
{
	device_destroy(my_dev->devcls, MKDEV(my_dev->majorNo, 0));
	class_destroy(my_dev->devcls);
	unregister_chrdev(my_dev->majorNo, mydevname);
	kfree(my_dev);
}

module_init(test_dev_init);
module_exit(test_dev_exit);



