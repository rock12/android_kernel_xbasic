#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/earlysuspend.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/bitops.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/byteorder/generic.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/rtpm_prio.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <cust_eint.h>
#include <linux/jiffies.h>
#include <linux/cdev.h>
#include <mach/mt_pm_ldo.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_boot.h>
#include <mach/mt_gpio.h>
#include "cust_gpio_usage.h"

static DECLARE_WAIT_QUEUE_HEAD(waiter);  
static void mt65xx_hall_eint_interrupt_handler(void);

#define MT65XX_HALL_DEVNAME            "mt65xx_hall_driver"

#define DELAY_MS(ms) {mdelay(ms);}
#define DELAY_US(us) {mdelay(us);}
#define DELAY_NS(ns) {mdelay(ns);}

static struct task_struct *thread = NULL;
static struct input_dev *kpd_mt65xx_hall_dev;

static int mt65xx_hall_flag = 0;
static int mt65xx_hall_state = CUST_EINT_POLARITY_HIGH;
#define LEVEL_LOW 0

extern void mt65xx_eint_mask(unsigned int eint_num);
extern void mt65xx_eint_unmask(unsigned int eint_num);
extern void mt65xx_eint_set_hw_debounce(unsigned int eint_num, unsigned int ms);
extern void mt65xx_eint_set_polarity(kal_uint8 eint_num, kal_bool ACT_Polarity);
extern unsigned int mt65xx_eint_set_sens(unsigned int eint_num, unsigned int sens);
extern void mt65xx_eint_registration(unsigned int eint_num, unsigned int is_deb_en, 
			unsigned int pol, void (EINT_FUNC_PTR)(void), 
			unsigned int is_auto_umask);

static int mt65xx_hall_event_handler(void *unused)
 {
	 do
	 {
		mt65xx_eint_unmask(CUST_EINT_HALL_1_NUM); 
		set_current_state(TASK_INTERRUPTIBLE); 
		wait_event_interruptible(waiter,mt65xx_hall_flag!=0);
		mt65xx_hall_flag = 0;
		set_current_state(TASK_RUNNING);
		if(mt65xx_hall_state==LEVEL_LOW)
		{
			input_report_key(kpd_mt65xx_hall_dev, KEY_FLIP_CLOSE, 1); 
			input_sync(kpd_mt65xx_hall_dev);
      		       input_report_key(kpd_mt65xx_hall_dev, KEY_FLIP_CLOSE, 0);	
			input_sync(kpd_mt65xx_hall_dev);		
		}
		else
		{
			 input_report_key(kpd_mt65xx_hall_dev,KEY_FLIP_OPEN,1);
                      input_sync(kpd_mt65xx_hall_dev);
			 input_report_key(kpd_mt65xx_hall_dev, KEY_FLIP_OPEN, 0);
			 input_sync(kpd_mt65xx_hall_dev);	
		}
           
	mt65xx_hall_state= !mt65xx_hall_state;
	mt65xx_eint_set_polarity(CUST_EINT_HALL_1_NUM, mt65xx_hall_state);
 	}while(!kthread_should_stop());
 
	 return 0;
 }


 static void mt65xx_hall_eint_interrupt_handler(void)
 {
	 mt65xx_hall_flag = 1;
	 wake_up_interruptible(&waiter);
 }

/*----------------------------------------------------------------------------*/
static int mt65xx_hall_probe(struct platform_device *pdev) 
{
      int retval = 0;
	mt_set_gpio_mode(GPIO_HALL_1_PIN, GPIO_HALL_1_PIN_M_EINT);    
       mt_set_gpio_dir(GPIO_HALL_1_PIN, GPIO_DIR_IN);
	mt_set_gpio_pull_enable(GPIO_HALL_1_PIN, GPIO_PULL_ENABLE);
	mt_set_gpio_pull_select(GPIO_HALL_1_PIN, GPIO_PULL_UP);

	mt65xx_eint_set_sens(CUST_EINT_HALL_1_NUM , CUST_EINT_HALL_1_SENSITIVE);
	mt65xx_eint_set_polarity(CUST_EINT_HALL_1_NUM, mt65xx_hall_state);
	mt65xx_eint_set_hw_debounce(CUST_EINT_HALL_1_NUM , CUST_EINT_HALL_1_DEBOUNCE_EN );
	mt65xx_eint_registration(CUST_EINT_HALL_1_NUM , CUST_EINT_HALL_1_DEBOUNCE_EN , mt65xx_hall_state, mt65xx_hall_eint_interrupt_handler, 1); 

	mt65xx_eint_unmask(CUST_EINT_HALL_1_NUM );

		thread = kthread_run(mt65xx_hall_event_handler, 0, MT65XX_HALL_DEVNAME);
	
	 if (IS_ERR(thread))
	{ 
		  retval = PTR_ERR(thread);
	}

	 kpd_mt65xx_hall_dev = input_allocate_device();
	if (!kpd_mt65xx_hall_dev) 
	{
		printk(" input_allocate_device : fail!\n");
	}
	set_bit(EV_KEY, kpd_mt65xx_hall_dev->evbit);
	__set_bit(KEY_FLIP_CLOSE, kpd_mt65xx_hall_dev->keybit);
	__set_bit(KEY_FLIP_OPEN, kpd_mt65xx_hall_dev->keybit);
    
	kpd_mt65xx_hall_dev->id.bustype = BUS_HOST;
	kpd_mt65xx_hall_dev->name = "mt65xx_hall_DRV";
	if(input_register_device(kpd_mt65xx_hall_dev))
	{
		printk(" input_register_device : fail!\n");
	}else
	{
		printk(" input_register_device : success!!\n");
	} 
	return 0;
}
/*----------------------------------------------------------------------------*/
static int mt65xx_hall_remove(struct platform_device *pdev)
{
    return 0;
}

/*----------------------------------------------------------------------------*/
static struct platform_driver mt65xx_hall_driver = {
	.probe      = mt65xx_hall_probe,
	.remove   = mt65xx_hall_remove,    
	.driver     = {
		.name  = "mt65xx_hall",
		.owner = THIS_MODULE,
	}
};

/*----------------------------------------------------------------------------*/
static int __init mt65xx_hall_init(void)
{
	if(platform_driver_register(&mt65xx_hall_driver))
	{
		printk("hmh_hall failed to register hall driver");
		return -ENODEV;
	}
	return 0;    
}
/*----------------------------------------------------------------------------*/
static void __exit mt65xx_hall_exit(void)
{
	platform_driver_unregister(&mt65xx_hall_driver);
}
/*----------------------------------------------------------------------------*/
module_init(mt65xx_hall_init);
module_exit(mt65xx_hall_exit);
/*----------------------------------------------------------------------------*/
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HALL driver");
MODULE_AUTHOR("huangmaohua@qq.com");

