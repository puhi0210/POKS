#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);

static ssize_t device_read(struct file *, char *, size_t, loff_t*);
static ssize_t device_write(struct file *, const char *, size_t, loff_t*);

int device_ioctl (struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 100

static int Major;
static int ReadCount = 2;
static char msgR[BUF_LEN] = "Tekst iz kernela \n\0";
static char msgW[BUF_LEN];

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int __init init_modul(void){
    
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if(Major < 0){
        printk(KERN_ALERT "Device registration failed %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "Device number assigned %d", Major);
    printk(KERN_INFO "Create driver mknod /dev/%s c %d 0 \n", DEVICE_NAME, Major);

    return SUCCESS;
}

static void __exit cleanup_modul(void){
    printk(KERN_INFO "Goodbye Major = %d\n", Major);
    unregister_chrdev(Major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file){
    printk(KERN_INFO "Device_open Major = %d\n", Major);
    ReadCount = 2;
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file){
    printk(KERN_INFO "Device_released Major = %d\n", Major);
    return SUCCESS;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t* offsize){
    int bytes_read = 0;
    if(ReadCount--){
        bytes_read = strlen(msgR);
        printk(KERN_INFO "device_read %s \n", msgR);
        raw_copy_to_user(buffer, msgR, bytes_read);
    }else{
        bytes_read = 0;
        printk(KERN_INFO "device_read bytes_read = 0\n");
    }
    
    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t* offsize){
    raw_copy_to_user(msgW, buffer, length);
    printk(KERN_INFO "device_write %s \n", msgW);
    strncpy(msgR, msgW, BUF_LEN);
    return length;
}

module_init(init_modul);
module_exit(cleanup_modul);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tilen");
MODULE_DESCRIPTION("chardev");
