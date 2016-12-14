#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/slab.h>

#define VRAM_BASE 0x000A0000
#define VRAM_SIZE 0x00020000

#define NUMBER_OF_FRAMES 16
#define ART_SIZE 60

static char **frames;
static unsigned int index;
static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class

static int my_open(struct inode *ind, struct file *f) {
    frames = kmalloc(NUMBER_OF_FRAMES * sizeof(char*), GFP_KERNEL);
    frames[0] = " .--.             \n(____)            \n ))))             \n";
    frames[1] = " .--.             \n(____)            \n ((((             \n";
    frames[2] = " .---.            \n(_____)           \n // //            \n";
    frames[3] = "   .----.         \n  (______)        \n   / / //         \n";
    frames[4] = "     .----.       \n    (______)      \n     / / //       \n";
    frames[5] = "      .---.       \n     (_____)      \n      // //       \n";
    frames[6] = "      .---.       \n     (_____)      \n      (( ((       \n";
    frames[7] = "       .--.       \n      (____)      \n       ))))       \n";
    frames[8] = "       .--.       \n      (____)      \n       ((((       \n";
    frames[9] = "      .---.       \n     (_____)      \n      \\\\ \\\\       \n";
    frames[10] = "     .----.       \n    (______)      \n     \\\\ \\ \\       \n";
    frames[11] = "   .----.         \n  (______)        \n   \\\\ \\ \\         \n";
    frames[12] = " .---.            \n(_____)           \n \\\\ \\\\            \n";
    frames[13] = " .---.            \n(_____)           \n )) ))            \n";
    frames[14] = " .--.             \n(____)            \n ))))             \n";
    frames[15] = " .--.             \n(____)            \n ((((             \n";

    index = 0;

    return 0;
}

static int my_close(struct inode *i, struct file *f) {
    kfree(frames);
    return 0;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off) {
    if (copy_to_user(buf, frames[index], len) != 0)
        return -EFAULT;
    else
        return len;
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off) {
    char command;
    
    if (copy_from_user(&command, buf + len - 1, 1) != 0)
        return -EFAULT;
    
    if(command == 'r') { // reset
        index = 0;
    } else if(command == 'n') { // next
        index = (index + 1) % NUMBER_OF_FRAMES;
    } else if(command == 'p') { // previous
        if(index == 0) {
            index = NUMBER_OF_FRAMES - 1;
        } else {
            index--;
        }
    }

    return len;
}

static struct file_operations pugs_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .read = my_read,
    .write = my_write
};

static int __init asciiart_init(void) {
    if (alloc_chrdev_region(&first, 0, 3, "Shweta") < 0)
    {
        return -1;
    }

    printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));

    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL) {
        unregister_chrdev_region(first, 1);
        return -1;
    }
    if (device_create(cl, NULL, first, NULL, "asciiart") == NULL) {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    cdev_init(&c_dev, &pugs_fops);

    if (cdev_add(&c_dev, first, 1) == -1) {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    return 0;
}

static void __exit asciiart_exit(void) {
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 3);
}

module_init(asciiart_init);
module_exit(asciiart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Grupo 7");
MODULE_DESCRIPTION("Ascii Art Streaming");