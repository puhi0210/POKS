#include <linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>

#include<asm/uaccess.h>

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);


static ssize_read device_read(struct file*, char*, size_t, loff_t*);
static ssize_read device_write(struct file*, const char*, size_t, loff_t*);

int device_ioctl (struct inode *inde, struct file *file, unsigned int ioctl_num, unsigned long ioctl_parm);

#define SUCCESS 0
#define DEVICE_NAME "chardev2"
#define BUF_LEN 100

static int Major;
static int ReadCount = 2;
static char msgR[BUF_LEN]="Tekst iz kernela \n\0";
static char msgW[BUF_LEN];

static struct file_operation flops ={
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int __init init_module(void){
    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if(Major<0){
        printk(KERN_ALERT"Device registration faild %d\n", Major);
        return Major;
    }
    printk(KERN_INFO"Device number assigned %d", Major);
    printk(KERN_INFO"Create driver mknod /dev/%s c %d 0", DEVICE_NAME, Major);
    
    return SUCCESS;
}

static void __exit cleanup_modul(void){
    printk(KERN_INFO "Goodbye Major=%d\n", Major);
    unregister_chrdev(Major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "device_open Major=%d\n", Major);
  ReadCount=2;
  return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "device_release Major=%d\n", Major);
  return SUCCESS;
}

static ssize_t device_read(struct file *filp,  
         char *buffer,	
         size_t length,	
         loff_t * offset)
{
  int bytes_read = 0;

  if (ReadCount--)
  {
    bytes_read=strlen(msgR);
    printk(KERN_INFO "device_read. %s", msgR);
    copy_to_user(buffer, msgR, bytes_read);
  }
  else
  {
    bytes_read=0;
    printk(KERN_INFO "device_read bytes_read=0");
  }
  return bytes_read;
}

static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * offset)
{
  copy_from_user(msgW,buff,len);
  printk(KERN_INFO "device_write. %s\n",msgW);
  return len;
}

module_init(init_modul);
module_exit(cleanup_modul);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mihael Zeme");	
MODULE_DESCRIPTION("vaja chardev");	
MODULE_SUPPORTED_DEVICE("test");