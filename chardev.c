#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
// #include <asm/uaccess.h>

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
static char *msg_ptr;
static int size_msg;

static char write_msg[BUF_LEN];

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

  printk(KERN_ALERT "Goodbye world.\n");
}

static int device_open(struct inode *inode, struct file *file)
{
  static int counter = 0;
  if (dev_open)
    return -EBUSY;
  
  dev_open++;
  size_msg = sprintf(msg, "I already told you %d times Hello world!\n", counter++);
  msg_ptr = msg;

  try_module_get(THIS_MODULE);

  return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
  dev_open--;

  module_put(THIS_MODULE); 

  return 0;
}

static ssize_t device_read(struct file *filp,
                           char __user *buf, size_t count,
                           loff_t *ppos)
{
  return simple_read_from_buffer(buf, count, ppos, msg_ptr, size_msg);
}

static ssize_t device_write(struct file *filp,
                            const char __user *buf, size_t len,
                            loff_t *ppos)
{
  ssize_t ret;

  ret = simple_write_to_buffer(write_msg, 4, ppos, buf, len);
  if (ret < 0)
    return ret;

  if (ret < BUF_LEN)
    write_msg[ret] = '\0';

  printk(KERN_ALERT "msg: %s\n", write_msg);

  return ret;
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");