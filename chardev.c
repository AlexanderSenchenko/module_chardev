#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
// #include <asm/uaccess.h>

// int init_module(void);
// void cleanup_mudule(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 80

static int major;
static int dev_open = 0;

static char msg[BUF_LEN];
static char *msg_Ptr;

static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
};

int __init chardev_init(void)
{
  major = register_chrdev(0, DEVICE_NAME, &fops);

  if (major < 0) {    
    printk("Registering the character device failed with %d\n", major);
    return major;
  }

  printk("<1>I was assigned major number %d.  To talk to\n", major);
  printk("<1>the driver, create a dev file with\n");
  printk("'mknod /dev/chardev c %d 0'.\n", major);
  printk("<1>Try various minor numbers.  Try to cat and echo to\n");
  printk("the device file.\n");
  printk("<1>Remove the device file and module when done.\n");

  return 0;
}

void __exit chardev_exit(void)
{
  unregister_chrdev(major, DEVICE_NAME);
  // int ret = unregister_chrdev(major, DEVICE_NAME);
  // if (ret < 0)
  //   printk("Error in unregister_chrdev: %d\n", ret);
  printk(KERN_ALERT "Goodbye world.\n");
}

static int device_open(struct inode *inode, struct file *file)
{
  static int counter = 0;
  if (dev_open)
    return -EBUSY;
  
  dev_open++;
  sprintf(msg, "I already told you %d times Hello world!\n", counter++);
  msg_Ptr = msg;

  try_module_get(THIS_MODULE);

  return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
  dev_open--;

  module_put(THIS_MODULE); 

  return 0;
}

#if 0
static ssize_t device_read(struct file *filp, char *buffer,
                           size_t length, loff_t * offset)
{
  int bytes_read = 0;

  if (*msg_Ptr == 0)
    return 0;

  while (length && *msg_Ptr) {
    put_user(*(msg_Ptr++), buffer++);

    length--;
    bytes_read++;
  }

  return bytes_read;
}
#else
static ssize_t device_read(struct file *filp,
                           char __user *buf, size_t count,
                           loff_t *ppos)
{
  return simple_read_from_buffer(buf, count, ppos, msg_Ptr, BUF_LEN);
}
#endif

static ssize_t device_write(struct file *filp, const char *buff,
                            size_t len, loff_t * off)
{
  printk("<1>Sorry, this operation isn't supported.\n");
  return -EINVAL;
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");