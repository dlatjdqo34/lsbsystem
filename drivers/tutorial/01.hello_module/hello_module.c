#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

static int __init hello_module_init(void)
{
    pr_info("Hello Kernel Module ^o^\n");
    return 0;
}

static void __exit hello_module_exit(void)
{
    pr_info("Bye Kernel Module ~.~\n");
}

module_init(hello_module_init);
module_exit(hello_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lsb <dlatjdq34@github.com>");
MODULE_DESCRIPTION("tutorial hello module");
MODULE_VERSION("1.0.0");