#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

int notify_value_changed(const char *val, const struct kernel_param *kp);

static int value = 1, arr[4];
static int cb_value = 0;
static char * str = "hello";
const struct kernel_param_ops my_param_ops = 
{
    .set = &notify_value_changed,
    .get = &param_get_int,
};

/*  It is possible to pass the argument to the Linux Device Driver
    @module_param(name, type, perm)
    This macro is used to initialize the arguments, defined in linux/moduleparam.h
        @name: the name of the variable
        @type: its type
            ex) bool, invbool, charp, int, long, short, uint, ulong, ushort
        @perm: a permissions mask to be used for an accompanying sysfs entry
            ex) S_IRUGO: read permission to user, group, and others
    This macro creates the sub-directory under /sys/module
    
    @module_param_array(name, type, num, perm)
    This macro is used to send the array as an argument to the Linux device driver

    @module_param_cb(name, ops, arg, perm)
    This macro is used to register the callback
    Whenever the argument (parameter) got changed, this callback function will be called
        @name: the name of the variable
        @ops: file operation, the type is "struct kernel_param_ops"
        @arg: args for @ops, the real address of the variable
        @per: a permissions mask to be used for an accompanying sysfs entry
*/
module_param(value, int, S_IRUSR|S_IWUSR);
module_param(str, charp, S_IRUSR|S_IWUSR);
module_param_array(arr, int, NULL, S_IRUSR|S_IWUSR);
module_param_cb(cb_value, &my_param_ops, &cb_value, S_IRUGO|S_IWUSR );

/* This function is called whenever {cb_value} be set */
int notify_value_changed(const char *val, const struct kernel_param *kp)
{
    int old_value = cb_value;
    int res = param_set_int(val, kp);
    if (res == 0) {
        pr_info("Callback function called!\n");
        if (old_value != cb_value)
            pr_info("New value of cb_value = %d\n", cb_value);
        return 0;
    }

    return -1;
}

static int __init param_module_init(void)
{
    int i;

    pr_info("Hello Param Module ^o^\n");
    pr_info("value = %d\n", value);
    pr_info("cb_value = %d\n", cb_value);
    pr_info("str = %s\n", str);

    for (i = 0; i < sizeof(arr) / sizeof(int); i++) {
        pr_info("arr[%d] = %d\n", i, arr[i]);
    }

    return 0;
}

static void __exit param_module_exit(void)
{
    pr_info("Bye Param Module ~.~\n");
}

module_init(param_module_init);
module_exit(param_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lsb <dlatjdq34@github.com>");
MODULE_DESCRIPTION("tutorial export module2");
MODULE_VERSION("1.0.0");