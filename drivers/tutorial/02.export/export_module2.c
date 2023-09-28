#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

/*  This module should be loaded after export_module1 is loaded
    Or, you would get error message like below
        "insmod: can't insert 'export_module2.ko': unknown symbol in module, or unknown parameter"
*/

extern int shared_count;
void shared_func(void);

static int __init export_module_init(void)
{
    pr_info("Hello Export Module2 ^o^\n");
    shared_func();
    pr_info("Shared function called %d times!\n", shared_count);
    return 0;
}

static void __exit export_module_exit(void)
{
    pr_info("Bye Export Module2 ~.~\n");
}

module_init(export_module_init);
module_exit(export_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lsb <dlatjdq34@github.com>");
MODULE_DESCRIPTION("tutorial export module2");
MODULE_VERSION("1.0.0");
