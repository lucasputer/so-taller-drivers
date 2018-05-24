#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

static struct class *hello_class;
struct cdev hello_dev;
static dev_t hello_devno;
int x = 0;
int y = 0;

ssize_t hello_read(struct file *filp, char __user *data,size_t s, loff_t *off) {
    printk(KERN_ALERT "A %d-%d: \n",x,y);
    return 0;
}
ssize_t hello_write(struct file *filp, const char __user *data, size_t s, loff_t *off) {
    int i;
    char* t;
    t = vmalloc(s);
    printk(KERN_ALERT "me escribieron!\n");
    copy_from_user(t,data,s);

    for(i = 0; i < s; i++){
        if(t[i] == 'E' && x < 9){
            x++;
        }else if(t[i] == 'W' && x > 0){
            x--;
        }else if(t[i] == 'N' && y > 0){
            y--;
        }else if(t[i] == 'S' && y < 9){
            y++;
        }
    }

    
    return s;
}

static struct file_operations mis_operaciones = {
    .owner = THIS_MODULE,
    .read = hello_read,
    .write = hello_write,
};

static int __init hello_init(void) {
     char* pepe = vmalloc(9);
     pepe = "A 0-0: \n\n";
    printk(KERN_ALERT "Hola, Sistemas Operativos!\n");
    printk(KERN_ALERT "%s\n", pepe);

    cdev_init(&hello_dev, &mis_operaciones);

    alloc_chrdev_region(&hello_devno, 0, 1, pepe);

    cdev_add(&hello_dev, hello_devno, 1);

    hello_class = class_create(THIS_MODULE, "hello");
    device_create(hello_class, NULL, hello_devno, NULL, "hello");


    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_ALERT "Adios, mundo cruel...\n");
    device_destroy(hello_class, hello_devno);
    class_destroy(hello_class);

    unregister_chrdev_region(hello_devno, 1);
    cdev_del(&hello_dev);
}


module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Juan de los Palotes");
MODULE_DESCRIPTION("Una suerte de ’Hola, mundo’");