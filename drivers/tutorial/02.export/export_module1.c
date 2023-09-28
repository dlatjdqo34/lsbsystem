#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

int shared_count = 0;

void shared_func(void)
{
    pr_info("Shared function been called!!!\n");
    shared_count++;
}

/*  Export symbols of this kernel module 
    So other kernel modules can use these variables or functions
    @EXPORT_SYMBOL()
*/
EXPORT_SYMBOL(shared_func);
EXPORT_SYMBOL(shared_count);

static int __init export_module_init(void)
{
    pr_info("Hello Export Module1 ^o^\n");
    shared_func();
    return 0;
}

static void __exit export_module_exit(void)
{
    pr_info("Bye Export Module1 ~.~\n");
}

module_init(export_module_init);
module_exit(export_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lsb <dlatjdq34@github.com>");
MODULE_DESCRIPTION("tutorial export module1");
MODULE_VERSION("1.0.0");
