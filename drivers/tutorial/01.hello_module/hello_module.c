#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

/* This function should register itself by using module_init() macro */
static int __init hello_module_init(void)
{
    /* pr_info() print logs of KERN_INFO level */
    pr_info("Hello Kernel Module ^o^\n");
    return 0;
}

/* This function should register itself by using module_exit() macro */
static void __exit hello_module_exit(void)
{
    pr_info("Bye Kernel Module ~.~\n");
}

/*  init function: start of the kernel module
    @module_init()
        Function that will execute first when the Linux device driver is loaded into the kernel 
        For example, when we load the driver using insmod, this function will execute 

    exit function: end of the kernel module
    @module_exit()
        Function that will execute last when the Linux device driver is unloaded from the kernel
        For example, when we unload the driver using rmmod, this function will execute
*/
module_init(hello_module_init);
module_exit(hello_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lsb <dlatjdq34@github.com>");
MODULE_DESCRIPTION("tutorial hello module");
MODULE_VERSION("1.0.0");