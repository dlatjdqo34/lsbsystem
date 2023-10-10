#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define BUF_SIZE 1024

/*  @MKDEV(major, minor): Return 32bit number of major and minor number */
static dev_t dev = MKDEV(235, 0);
static char kernel_read_buffer[BUF_SIZE];
static char kernel_write_buffer[BUF_SIZE];
static struct class *chr_class;
static struct cdev chr_device;

#define DRIVER_NAME "static_char_driver"
#define DRIVER_CLASS "static_char_class"

static ssize_t chr_driver_read(struct file *fp, char __user *buf, size_t count, loff_t *offset)
{
    if (copy_to_user(buf, kernel_read_buffer, BUF_SIZE)) {
        pr_err("Can`t read from %s\n", DRIVER_NAME);
    }
    pr_info()
}


static struct file_operations fops = {
    .owner = THIS_MODULE,
};


static int __init static_character_init(void)
{
    /* Alllocate device major and minor numbers */
    if (register_chrdev_region(dev, 1, , DRIVER_NAME) < 0) {
        pr_info("Device Driver could not be allocated!\n");
        return -1;
    }

    pr_info("Allocated Major = %d and Minor = %d\n", MAJOR(dev), MINOR(dev));

    /* Create device class */
    if ((chr_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
        pr_info("Device class can`t be created!\n");
        goto class_error;
    }

    /* Create device file */
    if (device_create(chr_class, NULL, dev, NULL, DRIVER_NAME) == NULL) {
        pr_info("Device file can`t be created!\n");
        goto device_error;
    }

    /* Initialize character device file */
    cdev_init(&chr_device, &fops);


device_error:

class_error:

}

static void __exit static_character_exit(void)
{

}


module_init(static_character_init);
module_exit(static_character_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lsb <dlatjdq34@github.com>");
MODULE_DESCRIPTION("tutorial hello module");
MODULE_VERSION("1.0.0");