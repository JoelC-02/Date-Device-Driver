#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/version.h>
#include<linux/string.h>
#include<linux/time.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/ktime.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joel Cecil");
MODULE_DESCRIPTION("Date Device Driver");
MODULE_VERSION("0.1");

static ssize_t readdate(struct file *,char *,size_t ,loff_t *);
static ssize_t writedate(struct file *,const char *,size_t ,loff_t *);
static int open_dev(struct inode *,struct file *);
static int close_dev(struct inode *,struct file *);
int major;

int atoi(const char *str) {
    int res=0,i=0;
    for(i=0;str[i]!='\0';i++)
        res=res*10+str[i]-'0';
    return res;
}

static struct file_operations fops={
    .open=open_dev,
    .release=close_dev,
    .read=readdate,
    .write=writedate,
};

static ssize_t readdate(struct file *filep,char *buff,size_t len,loff_t *f_ops) {
    char *b=kmalloc(40*sizeof(char),GFP_KERNEL);
    struct timespec64 t1;
    struct tm t2;
    ktime_get_real_ts64(&t1);
    long time = t1.tv_sec;
    time64_to_tm(time, 0, &t2);
    sprintf(b,"%d:%d:%d (GMT) %d/%d/%ld",t2.tm_hour, t2.tm_min, t2.tm_sec, t2.tm_mday, t2.tm_mon+1, t2.tm_year+1900);
    copy_to_user(buff,b,strlen(b));
    printk("Reading date and time\n");
    return len;
}

ssize_t writedate(struct file *filp,const char *buff,size_t len,loff_t *off) {
    char *b=kmalloc(30*sizeof(char),GFP_KERNEL);
    long int i;
    struct timespec64 t;
    copy_from_user(b,buff,len);
    i=atoi(b);
    t.tv_sec=i;
    t.tv_nsec=0;
    do_settimeofday64(&t);
    return len;
}

static int open_dev(struct inode *a,struct file *filep) {
    printk(KERN_INFO "Date device opened\n");
    return 0;
}

static int close_dev(struct inode *a,struct file *filep) {
    printk(KERN_INFO "Date device closed\n");
    return 0;
}

static int __init date_start(void) {
    if((major=(register_chrdev(0,"date",&fops)))<0) {
        return -1;
    }
    printk(KERN_INFO "Date device loaded%d\n",major);
    return 0;
}

static void __exit date_end(void) {
    unregister_chrdev(major,"date");
    printk(KERN_INFO "Date device unloaded\n");
}

module_init(date_start);
module_exit(date_end);
