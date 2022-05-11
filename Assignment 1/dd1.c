#include<linux/init.h>
#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/random.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include "adc8dev.h"



static dev_t adc8;
static struct cdev c_dev;
static struct class *cls;
static uint16_t ch = 0;
static char al = 'r';
uint16_t randomgenerator(void);


static int adc_open(struct inode *i,struct file *f)
	{
		
		printk(KERN_INFO "Driver: open()\n");
		return 0;
	}


static int adc_close(struct inode *i,struct file *f)   
	{
		
		printk(KERN_INFO "Driver: close()\n");
		return 0;

	}


static ssize_t adc_read(struct file *f, char __user *buf, size_t len, loff_t *off){
    	int k_user;
    	uint16_t k;
    	k = randomgenerator();
	printk(KERN_INFO " random number is %d",k);
				if(al == 'l')
		{									
				k = k * 64;				
		}
	
    	k_user = copy_to_user(buf,&k,sizeof(k));
    	if(k_user!=0)
    	{
    		printk(KERN_INFO "Failed to copy %d bytes to the userspace ",k_user);
    	}
		return sizeof(k);

	}


long adc8_ioctl(struct file *file,unsigned int ioctl_num,unsigned long ioctl_param){

                switch(ioctl_num){
                  case CHANNELSELECTION:
			ch = ioctl_param;
                    	printk(KERN_INFO "selected channel is %d",ch);
			break;

                  case ALSELECTION:
                    	al = (char)ioctl_param;
                    	printk(KERN_INFO "selected allignment is %c",al);
			break;
			
		 case SEL_CONV:
                    	al = ioctl_param;
                    	printk(KERN_INFO "conversion is %c",al);
			break;
                }
              return 0;
  }

uint16_t randomgenerator(void)
{                                                  // ---------------this function is generating random number-------------
    unsigned int num;
    get_random_bytes(&num, 2);
    num &= 0x03FF;
    return (uint16_t)num;
  }

static struct file_operations fops =                                         // ----------file operation and character driver linking-------------------
					{
					.owner = THIS_MODULE,
					.open = adc_open,
					.release = adc_close,
                                        .unlocked_ioctl = adc8_ioctl,
					.read = adc_read
					};

static int __init adcdriver_init(void)
{
	                                                               // ---------major and minor number allocation.---------------

		if(alloc_chrdev_region(&adc8,0,5,"dheeraj")<0)
	{
		return -1;
	}
	printk(KERN_INFO "<major , minor>:<%d,%d>\n",MAJOR(adc8),MINOR(adc8));

	                                                                        //-----------Creation of device file.-------------

		if((cls=class_create(THIS_MODULE,"adcclass"))==NULL)
	{
			unregister_chrdev_region(adc8,1);
			return -1;
	}
		if((device_create(cls,NULL,adc8,NULL,"adc-dev"))==NULL)
	{
			class_destroy(cls);
			unregister_chrdev_region(adc8,1);
			return -1;
	}
	                                                                    //---------Link fops and cdev to device node--------------

	cdev_init(&c_dev,&fops);
	                                                                   
		if(cdev_add(&c_dev,adc8, 1)==-1)
	{
		device_destroy(cls,adc8);
		class_destroy(cls);
		unregister_chrdev_region(adc8,1);
		return -1;
	}
	return 0;
}


static void __exit adcdriver_exit(void){
	cdev_del(&c_dev);
	device_destroy(cls,adc8);
	class_destroy(cls);
	unregister_chrdev_region(adc8,3);
	printk(KERN_INFO "Driver unregistered");
}


module_init(adcdriver_init);
module_exit(adcdriver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ADC by dheeraj shakya");
MODULE_AUTHOR("dheeraj shakya <h20210164@pilani.bits-pilani.ac.in>");
