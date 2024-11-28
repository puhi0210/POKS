#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>

static int __init init_modul (void) {

    printk(KERN_INFO "\nHello World!\n");
    return 0;

}

static void __exit cleanup_modul(void){
    printk(KERN_INFO "\nGoodbye world!\n");
}

module_init(init_modul);
module_exit(cleanup_modul);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mihael Zeme");
MODULE_DESCRIPTION("Demo modul");