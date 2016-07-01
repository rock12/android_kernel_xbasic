/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */

#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/earlysuspend.h>
#include <linux/platform_device.h>
#include <asm/atomic.h>

#include <linux/hwmsensor.h>
#include <linux/hwmsen_dev.h>
#include <linux/sensors_io.h>
#include <asm/io.h>
#include <cust_eint.h>
#include <cust_alsps.h>
#include "qre1113.h"
#include <linux/hwmsen_helper.h>


#include <mach/mt_typedefs.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>


#include "mach/mt_clkmgr.h"
#include <linux/wakelock.h>

#include <mach/mt_auxadc_sw.h>
#include <mach/mt_auxadc_hw.h>

#include <mach/hardware.h>
#include <mach/mt_gpt.h>
#include <mach/mt_clkmgr.h>
#include <mach/sync_write.h>
#include <cust_adc.h> // generate by DCT Tool

extern int IMM_GetOneChannelValue(int dwChannel, int data[4], int* rawdata);

int qre1113_GetOneChannelValue(int dwChannel, int deCount);

#define POWER_NONE_MACRO MT65XX_POWER_NONE
#define MT65XX_PDN_PERI_AUXADC MT_CG_AUX_SW_CG_ADC

/******************************************************************************
 * configuration
*******************************************************************************/
#define I2C_DRIVERID_QRE1113 3006
/*----------------------------------------------------------------------------*/
#define QRE1113_I2C_ADDR_RAR 0   /*!< the index in obj->hw->i2c_addr: alert response address */
#define QRE1113_I2C_ADDR_ALS 1   /*!< the index in obj->hw->i2c_addr: ALS address */
#define QRE1113_I2C_ADDR_PS  2   /*!< the index in obj->hw->i2c_addr: PS address */
#define QRE1113_DEV_NAME     "QRE1113"
/*----------------------------------------------------------------------------*/
#define APS_TAG                  "[ALS/PS] "
#define APS_FUN(f)               printk(KERN_INFO APS_TAG"%s\n", __FUNCTION__)
#define APS_ERR(fmt, args...)    printk(KERN_ERR  APS_TAG"%s %d : "fmt, __FUNCTION__, __LINE__, ##args)
#define APS_LOG(fmt, args...)    printk(KERN_INFO APS_TAG fmt, ##args)
#define APS_DBG(fmt, args...)    printk(KERN_INFO fmt, ##args)
#define FTM_CUST_ALSPS "/data/qre1113"
/******************************************************************************
 * extern functions
*******************************************************************************/
/*----------------------------------------------------------------------------*/
#define mt6516_I2C_DATA_PORT        ((base) + 0x0000)
#define mt6516_I2C_SLAVE_ADDR       ((base) + 0x0004)
#define mt6516_I2C_INTR_MASK        ((base) + 0x0008)
#define mt6516_I2C_INTR_STAT        ((base) + 0x000c)
#define mt6516_I2C_CONTROL          ((base) + 0x0010)
#define mt6516_I2C_TRANSFER_LEN     ((base) + 0x0014)
#define mt6516_I2C_TRANSAC_LEN      ((base) + 0x0018)
#define mt6516_I2C_DELAY_LEN        ((base) + 0x001c)
#define mt6516_I2C_TIMING           ((base) + 0x0020)
#define mt6516_I2C_START            ((base) + 0x0024)
#define mt6516_I2C_FIFO_STAT        ((base) + 0x0030)
#define mt6516_I2C_FIFO_THRESH      ((base) + 0x0034)
#define mt6516_I2C_FIFO_ADDR_CLR    ((base) + 0x0038)
#define mt6516_I2C_IO_CONFIG        ((base) + 0x0040)
#define mt6516_I2C_DEBUG            ((base) + 0x0044)
#define mt6516_I2C_HS               ((base) + 0x0048)
#define mt6516_I2C_DEBUGSTAT        ((base) + 0x0064)
#define mt6516_I2C_DEBUGCTRL        ((base) + 0x0068)
/*----------------------------------------------------------------------------*/
static struct i2c_client *qre1113_i2c_client = NULL;
/*----------------------------------------------------------------------------*/
static const struct i2c_device_id qre1113_i2c_id[] = {{QRE1113_DEV_NAME,0},{}};
static struct i2c_board_info __initdata i2c_qre1113={ I2C_BOARD_INFO("QRE1113", (QRE1113_I2C_SLAVE_ADDR>>1))};
/*the adapter id & i2c address will be available in customization*/
//static unsigned short qre1113_force[] = {0x00, QRE1113_I2C_SLAVE_ADDR, I2C_CLIENT_END, I2C_CLIENT_END};
//static const unsigned short *const qre1113_forces[] = { qre1113_force, NULL };
//static struct i2c_client_address_data qre1113_addr_data = { .forces = qre1113_forces,};
/*----------------------------------------------------------------------------*/
static int qre1113_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int qre1113_i2c_remove(struct i2c_client *client);
//static int qre1113_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);
/*----------------------------------------------------------------------------*/
static int qre1113_i2c_suspend(struct i2c_client *client, pm_message_t msg);
static int qre1113_i2c_resume(struct i2c_client *client);


static struct qre1113_priv *g_qre1113_ptr = NULL;
static UINT16 distance_ps_threshold_flag = 1;

/*----------------------------------------------------------------------------*/
typedef enum {
    CMC_TRC_APS_DATA = 0x0002,
    CMC_TRC_EINT    = 0x0004,
    CMC_TRC_IOCTL   = 0x0008,
    CMC_TRC_I2C     = 0x0010,
    CMC_TRC_CVT_ALS = 0x0020,
    CMC_TRC_CVT_PS  = 0x0040,
    CMC_TRC_DEBUG   = 0x8000,
} CMC_TRC;
/*----------------------------------------------------------------------------*/
typedef enum {
    CMC_BIT_ALS    = 1,
    CMC_BIT_PS     = 2,
} CMC_BIT;
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
struct qre1113_priv {
    struct alsps_hw  *hw;
    struct i2c_client *client;
    struct delayed_work  eint_work;
    //struct timer_list   first_read_ps_timer;
    //struct timer_list   first_read_als_timer;

    /*misc*/
    atomic_t    trace;
    atomic_t    i2c_retry;
    atomic_t    ps_mask;        /*mask ps: always return far away*/
    atomic_t    ps_debounce;    /*debounce time after enabling ps*/
    atomic_t    ps_deb_on;      /*indicates if the debounce is on*/
    atomic_t    ps_deb_end;     /*the jiffies representing the end of debounce*/
    atomic_t    ps_suspend;

    int adc_channel;                                    /*!< the adc channel used by ALS/PS */
    int mros_en;                                    /*!< the gpio enable used by ALS/PS */
    int mros_gpio;                                    /*!< the gpio enable used by ALS/PS */
    /*data*/
    u8          ps;

    bool    ps_enable;     /*record current ps status*/
    unsigned int    ps_thd_val;     /*the cmd value can't be read, stored in ram*/
    ulong       enable;         /*record HAL enalbe status*/
    //ulong        first_read;   // record first read ps and als
    /*early suspend*/
#if defined(CONFIG_HAS_EARLYSUSPEND)
    struct early_suspend    early_drv;
#endif
};
/*----------------------------------------------------------------------------*/
static struct i2c_driver qre1113_i2c_driver = {
            .probe      = qre1113_i2c_probe,
                          .remove     = qre1113_i2c_remove,
                                        //.detect     = qre1113_i2c_detect,
                                        .suspend    = qre1113_i2c_suspend,
                                                      .resume     = qre1113_i2c_resume,
                                                                    .id_table   = qre1113_i2c_id,
                                                                                  //.address_data = &qre1113_addr_data,
                                                                                  .driver = {
                                                                                                //	.owner          = THIS_MODULE,
                                                                                                .name           = QRE1113_DEV_NAME,
                                                                                            },
                                                                                        };

static struct qre1113_priv *qre1113_obj = NULL;
static struct platform_driver qre1113_alsps_driver;

static int qre1113_get_ps_value(struct qre1113_priv *obj, u8 ps);

struct wake_lock  ps_lock;
static void qre1113_dumpReg(struct i2c_client *client)
{
    return;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

#define MT65XX_PDN_PERI_AUXADC MT_CG_AUX_SW_CG_ADC

#define DRV_ClearBits(addr,data)     {\
   kal_uint16 temp;\
   temp = DRV_Reg(addr);\
   temp &=~(data);\
   mt65xx_reg_sync_writew(temp, addr);\
}

#define DRV_SetBits(addr,data)     {\
   kal_uint16 temp;\
   temp = DRV_Reg(addr);\
   temp |= (data);\
   mt65xx_reg_sync_writew(temp, addr);\
}

#define DRV_SetData(addr, bitmask, value)     {\
   kal_uint16 temp;\
   temp = (~(bitmask)) & DRV_Reg(addr);\
   temp |= (value);\
   mt65xx_reg_sync_writew(temp, addr);\
}

#define AUXADC_DRV_ClearBits16(addr, data)           DRV_ClearBits(addr,data)
#define AUXADC_DRV_SetBits16(addr, data)             DRV_SetBits(addr,data)
#define AUXADC_DRV_WriteReg16(addr, data)            mt65xx_reg_sync_writew(data, addr)
#define AUXADC_DRV_ReadReg16(addr)                   DRV_Reg(addr)
#define AUXADC_DRV_SetData16(addr, bitmask, value)   DRV_SetData(addr, bitmask, value)

int qre1113_GetOneChannelValueOne(int dwChannel, int data[4], int* rawdata)
{
    unsigned int channel[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int idle_count =0;
    int data_ready_count=0;

    AUXADC_DRV_ClearBits16(AUXADC_CON1, (1 << dwChannel));
    AUXADC_DRV_SetBits16(AUXADC_CON1, (1 << dwChannel));
    channel[dwChannel] = (*(volatile u16 *)(AUXADC_DAT0 + dwChannel * 0x04)) & 0x0FFF;
    if (NULL != rawdata)
    {
        *rawdata = channel[dwChannel];
    }
    data[0] = (channel[dwChannel] * 150 / 4096 / 100);
    data[1] = ((channel[dwChannel] * 150 / 4096) % 100);
    return 0;
}


int qre1113_GetOneChannelValue(int dwChannel, int deCount)
{

    int ret = 0;
    int data_value=0;

    int res =0,data[4] = {0,0,0,0}; //////
    int i,count1=0,count2=0,rawdata=0;///
    unsigned int u4channel=0;

    for (i = 0; i < deCount; i++)
    {
        //printk("[adc_driver]: i=%d\n",i);

        res = qre1113_GetOneChannelValueOne(dwChannel,data,&rawdata);
        if (res < 0)
        {
            // printk("[adc_driver]: get data error\n");
            break;

        }
        else
        {
            // printk("[adc_driver]: channel[%d]raw =%d\n",i,rawdata);
            // printk("[adc_driver]: channel[%d]=%d.%.02d \n",i,data[0],data[1]);
            u4channel+=rawdata;
            count1++;
        }


    }
    if (count1>0)
        data_value = u4channel/count1;


    //	printk("MYCAT :GPIO_Enable out data is %d",mt_get_gpio_out(GPIO_Enable));




    return data_value;


}

int qre1113_read_data(struct i2c_client *client, u8 *data)
{
    struct qre1113_priv *obj = i2c_get_clientdata(client);
    struct file *fp;
    int ret = 0;
    int ADC_Chanel = qre1113_obj->adc_channel;                                    /*adc chanel */
    int GPIO_Enable = qre1113_obj->mros_en;
    unsigned int    ps_threshold = qre1113_obj->ps_thd_val;
    int data_high,data_low,distance,count_temp;
    int res =0,data1[4] = {0,0,0,0}; //////
    int i,count1=0,count2=0,rawdata=0;///
    unsigned int u4channel=0;
    //APS_DBG("PS:ADC_Chanel=%d ,GPIO_Enable=%d ,ps_threshold=%d\n", ADC_Chanel,GPIO_Enable,ps_threshold);

    if (enable_clock(MT65XX_PDN_PERI_AUXADC,"AUXADC")); ////AD clock en

    mt_set_gpio_mode(GPIO_Enable,GPIO_MODE_GPIO);
    mt_set_gpio_dir(GPIO_Enable,GPIO_DIR_OUT);

    data_low =qre1113_GetOneChannelValue(ADC_Chanel, 10);

    mt_set_gpio_out(GPIO_Enable,GPIO_OUT_ONE);
    mdelay(1);

    data_high =qre1113_GetOneChannelValue(ADC_Chanel, 10);

    mt_set_gpio_out(GPIO_Enable,GPIO_OUT_ZERO);
    //APS_DBG("qre1113_read_data:data_low=%d data_high=%d ps_threshold=%d\n", data_low,data_high,ps_threshold);

    if (distance_ps_threshold_flag == 1)
    {
        distance_ps_threshold_flag = 0;
        data[0] = 1;//far away
        mm_segment_t old_fs = get_fs();
        set_fs(KERNEL_DS);

        fp = filp_open(FTM_CUST_ALSPS,O_RDONLY,0);

        if (fp != 0xfffffffe)//open file suscess
        {
            //fp->f_op->llseek(fp, 0, 0);
            fp->f_pos = 0;
            count_temp = fp->f_op->read(fp,
                                        &ps_threshold,
                                        2,
                                        &fp->f_pos);
            filp_close(fp, NULL);
            set_fs(old_fs);
            qre1113_obj->ps_thd_val = ps_threshold;
        }

    }
    printk("hgw:qre1113_read_ps value is data_low = %d,data_high = %d,distance_ps_threshold = %d\r\n",data_low,data_high,ps_threshold);
    if (data_low>1050 || (data_high<data_low))

        data[0] = 1;
    else
    {
        distance = data_high-data_low;
        if (distance < ps_threshold)
            data[0] = 1;//far away
        else
            data[0] = 0;//close
    }
    if (atomic_read(&obj->trace) & CMC_TRC_APS_DATA)
    {
        APS_DBG("APS:  0x%04X\n", (u32)(*data));
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
static void qre1113_power(struct alsps_hw *hw, unsigned int on)
{
    static unsigned int power_on = 0;

    //APS_LOG("power %s\n", on ? "on" : "off");

    if (hw->power_id != POWER_NONE_MACRO)
    {
        if (power_on == on)
        {
            APS_LOG("ignore power control: %d\n", on);
        }
        else if (on)
        {
            if (!hwPowerOn(hw->power_id, hw->power_vol, "QRE1113"))
            {
                APS_ERR("power on fails!!\n");
            }
        }
        else
        {
            if (!hwPowerDown(hw->power_id, "QRE1113"))
            {
                APS_ERR("power off fail!!\n");
            }
        }
    }
    power_on = on;
}

/*----------------------------------------------------------------------------*/
static int qre1113_enable_ps(struct i2c_client *client, bool enable)
{
    APS_LOG(" qre1113_enable_ps %d\n",enable);
    struct qre1113_priv *obj = i2c_get_clientdata(client);
    int err=0;
    int trc = atomic_read(&obj->trace);
    u8 regdata=0;
    int MROS_GPIO = qre1113_obj->mros_gpio;
    if (enable == obj->ps_enable)
    {
        return 0;
    }

    obj->ps_enable = enable;

    if (MROS_GPIO!= -1)
    {
        if (enable)
        {
            wake_lock(&ps_lock);
            APS_LOG(" qre1113_wake_lock(&ps_lock);");

            mt_set_gpio_mode(MROS_GPIO,GPIO_MODE_GPIO);
            mt_set_gpio_dir(MROS_GPIO,GPIO_DIR_OUT);
            mt_set_gpio_out(MROS_GPIO,GPIO_OUT_ONE);
        }
        else
        {
            wake_unlock(&ps_lock);
            APS_LOG(" qre1113_wake_unlock(&ps_lock);");

            mt_set_gpio_mode(MROS_GPIO,GPIO_MODE_GPIO);
            mt_set_gpio_dir(MROS_GPIO,GPIO_DIR_OUT);
            mt_set_gpio_out(MROS_GPIO,GPIO_OUT_ZERO);
        }
    }
    APS_LOG(" MROS_GPIO %d\n",MROS_GPIO);
    if (enable)
    {


        wake_lock(&ps_lock);

    }
    else
    {

        //if(has_wake_lock(WAKE_LOCK_SUSPEND))
        wake_unlock(&ps_lock);
    }

    if (trc & CMC_TRC_DEBUG)
    {
        APS_LOG("enable ps (%d)\n", enable);
    }

    return err;
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
static int qre1113_init_client(struct i2c_client *client)
{
    struct qre1113_priv *obj = i2c_get_clientdata(client);
    int err=0;
    APS_LOG("qre1113_init_client.........\r\n");

    return err;
}
/******************************************************************************
 * Sysfs attributes
*******************************************************************************/
static ssize_t qre1113_show_config(struct device_driver *ddri, char *buf)
{
    ssize_t res;

    if (!qre1113_obj)
    {
        APS_ERR("qre1113_obj is null!!\n");
        return 0;
    }

    res = snprintf(buf, PAGE_SIZE, "(%d %d %d %d)\n",
                   atomic_read(&qre1113_obj->i2c_retry),
                   atomic_read(&qre1113_obj->ps_mask), qre1113_obj->ps_thd_val, atomic_read(&qre1113_obj->ps_debounce));
    return res;
}
/*----------------------------------------------------------------------------*/
static ssize_t qre1113_store_config(struct device_driver *ddri, char *buf, size_t count)
{
    int retry, als_deb, ps_deb, mask, thres;
    if (!qre1113_obj)
    {
        APS_ERR("qre1113_obj is null!!\n");
        return 0;
    }

    if (5 == sscanf(buf, "%d %d %d %d", &retry, &mask, &thres, &ps_deb))
    {
        atomic_set(&qre1113_obj->i2c_retry, retry);
        //atomic_set(&qre1113_obj->als_debounce, als_deb);
        atomic_set(&qre1113_obj->ps_mask, mask);
        qre1113_obj->ps_thd_val= thres;
        atomic_set(&qre1113_obj->ps_debounce, ps_deb);
    }
    else
    {
        APS_ERR("invalid content: '%s', length = %d\n", buf, count);
    }
    return count;
}
/*----------------------------------------------------------------------------*/
static ssize_t qre1113_show_trace(struct device_driver *ddri, char *buf)
{
    ssize_t res;
    if (!qre1113_obj)
    {
        APS_ERR("qre1113_obj is null!!\n");
        return 0;
    }

    res = snprintf(buf, PAGE_SIZE, "0x%04X\n", atomic_read(&qre1113_obj->trace));
    return res;
}
/*----------------------------------------------------------------------------*/
static ssize_t qre1113_store_trace(struct device_driver *ddri, char *buf, size_t count)
{
    int trace;
    if (!qre1113_obj)
    {
        APS_ERR("qre1113_obj is null!!\n");
        return 0;
    }

    if (1 == sscanf(buf, "0x%x", &trace))
    {
        atomic_set(&qre1113_obj->trace, trace);
    }
    else
    {
        APS_ERR("invalid content: '%s', length = %d\n", buf, count);
    }
    return count;
}
/*----------------------------------------------------------------------------*/
static ssize_t qre1113_show_ps(struct device_driver *ddri, char *buf)
{
    ssize_t res;
    u8 dat=0;
    if (!qre1113_obj)
    {
        APS_ERR("qre1113_obj is null!!\n");
        return 0;
    }

    if (res = qre1113_read_data(qre1113_obj->client, &qre1113_obj->ps))
    {
        return snprintf(buf, PAGE_SIZE, "ERROR: %d\n", res);
    }
    else
    {
        dat = qre1113_obj->ps & 0x80;
        return snprintf(buf, PAGE_SIZE, "0x%04X\n", dat);
    }
}
/*----------------------------------------------------------------------------*/
static ssize_t qre1113_show_reg(struct device_driver *ddri, char *buf)
{
    if (!qre1113_obj)
    {
        APS_ERR("qre1113_obj is null!!\n");
        return 0;
    }

    /*read*/
    qre1113_dumpReg(qre1113_obj->client);

    return 0;
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
static ssize_t qre1113_show_status(struct device_driver *ddri, char *buf)
{
    ssize_t len = 0;

    if (!qre1113_obj)
    {
        APS_ERR("qre1113_obj is null!!\n");
        return 0;
    }

    if (qre1113_obj->hw)
    {

        len += snprintf(buf+len, PAGE_SIZE-len, "CUST: %d, (%d %d)\n",
                        qre1113_obj->hw->i2c_num, qre1113_obj->hw->power_id, qre1113_obj->hw->power_vol);

    }
    else
    {
        len += snprintf(buf+len, PAGE_SIZE-len, "CUST: NULL\n");
    }

#ifdef MT6516
    len += snprintf(buf+len, PAGE_SIZE-len, "EINT: %d (%d %d %d %d)\n", mt_get_gpio_in(GPIO_ALS_EINT_PIN),
                    CUST_EINT_ALS_NUM, CUST_EINT_ALS_POLARITY, CUST_EINT_ALS_DEBOUNCE_EN, CUST_EINT_ALS_DEBOUNCE_CN);

    len += snprintf(buf+len, PAGE_SIZE-len, "GPIO: %d (%d %d %d %d)\n",	GPIO_ALS_EINT_PIN,
                    mt_get_gpio_dir(GPIO_ALS_EINT_PIN), mt_get_gpio_mode(GPIO_ALS_EINT_PIN),
                    mt_get_gpio_pull_enable(GPIO_ALS_EINT_PIN), mt_get_gpio_pull_select(GPIO_ALS_EINT_PIN));
#endif

    len += snprintf(buf+len, PAGE_SIZE-len, "MISC: %d\n", atomic_read(&qre1113_obj->ps_suspend));

    return len;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#define IS_SPACE(CH) (((CH) == ' ') || ((CH) == '\n'))
/*----------------------------------------------------------------------------*/
static int read_int_from_buf(struct qre1113_priv *obj, const char* buf, size_t count,
                             u32 data[], int len)
{
    int idx = 0;
    char *cur = (char*)buf, *end = (char*)(buf+count);

    while (idx < len)
    {
        while ((cur < end) && IS_SPACE(*cur))
        {
            cur++;
        }

        if (1 != sscanf(cur, "%d", &data[idx]))
        {
            break;
        }

        idx++;
        while ((cur < end) && !IS_SPACE(*cur))
        {
            cur++;
        }
    }
    return idx;
}
/*----------------------------------------------------------------------------*/
static DRIVER_ATTR(ps,      S_IWUSR | S_IRUGO, qre1113_show_ps,    NULL);
static DRIVER_ATTR(config,  S_IWUSR | S_IRUGO, qre1113_show_config,qre1113_store_config);
static DRIVER_ATTR(trace,   S_IWUSR | S_IRUGO, qre1113_show_trace, qre1113_store_trace);
static DRIVER_ATTR(status,  S_IWUSR | S_IRUGO, qre1113_show_status,  NULL);
static DRIVER_ATTR(reg,     S_IWUSR | S_IRUGO, qre1113_show_reg,   NULL);
/////static DRIVER_ATTR(i2c,     S_IWUSR | S_IRUGO, qre1113_show_i2c,   qre1113_store_i2c);
/*----------------------------------------------------------------------------*/
static struct device_attribute *qre1113_attr_list[] = {
            &driver_attr_ps,
            &driver_attr_trace,        /*trace log*/
            &driver_attr_config,
            &driver_attr_status,
            //// &driver_attr_i2c,
            &driver_attr_reg,
        };
/*----------------------------------------------------------------------------*/
static int qre1113_create_attr(struct device_driver *driver)
{
    int idx, err = 0;
    int num = (int)(sizeof(qre1113_attr_list)/sizeof(qre1113_attr_list[0]));
    if (driver == NULL)
    {
        return -EINVAL;
    }

    for (idx = 0; idx < num; idx++)
    {
        if (err = driver_create_file(driver, qre1113_attr_list[idx]))
        {
            APS_ERR("driver_create_file (%s) = %d\n", qre1113_attr_list[idx]->attr.name, err);
            break;
        }
    }
    return err;
}
/*----------------------------------------------------------------------------*/
static int qre1113_delete_attr(struct device_driver *driver)
{
    int idx ,err = 0;
    int num = (int)(sizeof(qre1113_attr_list)/sizeof(qre1113_attr_list[0]));

    if (!driver)
        return -EINVAL;

    for (idx = 0; idx < num; idx++)
    {
        driver_remove_file(driver, qre1113_attr_list[idx]);
    }

    return err;
}
/******************************************************************************
 * Function Configuration
******************************************************************************/
static int qre1113_get_ps_value(struct qre1113_priv *obj, u8 ps)
{

    int val= -1;
    int invalid = 0;

    if (ps == 0)
    {
        val = 0;  /*close*/
    }
    else
    {
        val = 1;  /*far away*/
    }

    if (atomic_read(&obj->ps_suspend))
    {
        invalid = 1;
    }
    else if (1 == atomic_read(&obj->ps_deb_on))
    {
        unsigned long endt = atomic_read(&obj->ps_deb_end);
        if (time_after(jiffies, endt))
        {
            atomic_set(&obj->ps_deb_on, 0);
            //clear_bit(CMC_BIT_PS, &obj->first_read);
        }

        if (1 == atomic_read(&obj->ps_deb_on))
        {
            invalid = 1;
        }
    }

    if (!invalid)
    {
        if (unlikely(atomic_read(&obj->trace) & CMC_TRC_CVT_PS))
        {
            APS_DBG("PS:  %05d => %05d\n", ps, val);
        }
        return val;

    }
    else
    {
        if (unlikely(atomic_read(&obj->trace) & CMC_TRC_CVT_PS))
        {
            APS_DBG("PS:  %05d => %05d (-1)\n", ps, val);
        }
        return -1;
    }

}

//add by wangwei for factory auto test
static int qre1113_autotest(struct i2c_client *client)
{
    struct qre1113_priv *obj = i2c_get_clientdata(client);
    int ret = 0;
    int ADC_Chanel = qre1113_obj->adc_channel;                                    /*adc chanel */
    int GPIO_Enable = qre1113_obj->mros_en;
    unsigned int    ps_threshold = qre1113_obj->ps_thd_val;
    int data_high,data_low,distance;
    int distance_temp = 0;
    int res =0,data[4] = {0,0,0,0}; //////
    int i,count1=0,count2=0,rawdata=0;///
    unsigned int u4channel=0;

    APS_DBG("PS:ADC_Chanel=%d ,GPIO_Enable=%d ,ps_threshold=%d\n", ADC_Chanel,GPIO_Enable,ps_threshold);

    mt_set_gpio_mode(GPIO_Enable,GPIO_MODE_GPIO);
    mt_set_gpio_dir(GPIO_Enable,GPIO_DIR_OUT);
    printk("hgw000:GPIO_Enable out data is %d",mt_get_gpio_out(GPIO_Enable));
    /////////////////

    for (i = 0; i < 5; i++)
    {
        //printk("[adc_driver]: i=%d\n",i);

        res = IMM_GetOneChannelValue(ADC_Chanel,data,&rawdata);
        if (res < 0)
        {
            // printk("[adc_driver]: get data error\n");
            break;

        }
        else
        {
            // printk("[adc_driver]: channel[%d]raw =%d\n",i,rawdata);
            // printk("[adc_driver]: channel[%d]=%d.%.02d \n",i,data[0],data[1]);
            u4channel+=rawdata;
            count1++;
        }
        //u4channel+=rawdata;

    }
    if (count1>0) data_low = u4channel/count1;
////////////////

    ////data_low = IMM_GetOneChannelValue(ADC_Chanel,10);
    //data_low = data_low*1024/(2800*10);
    mt_set_gpio_out(GPIO_Enable,GPIO_OUT_ONE);
    mdelay(1);
    printk("hgw:GPIO_Enable out data is %d",mt_get_gpio_out(GPIO_Enable));
    /////////////////

    for (i = 0; i < 4; i++)
    {
        //printk("[adc_driver]: i=%d\n",i);

        res = IMM_GetOneChannelValue(ADC_Chanel,data,&rawdata);
        if (res < 0)
        {
            //printk("[adc_driver]: get data error\n");
            break;

        }
        else
        {
            // printk("[adc_driver]: channel[%d]raw =%d\n",i,rawdata);
            // printk("[adc_driver]: channel[%d]=%d.%.02d \n",i,data[0],data[1]);
            u4channel+=rawdata;
            count2++;
        }
        // u4channel+=rawdata;

    }
    if (count2>0)data_high= u4channel/count2;
//////////////

    ////data_high= IMM_GetOneChannelValue(ADC_Chanel,10);
    //data_high = data_high*1024/(2800*10);
    mt_set_gpio_out(GPIO_Enable,GPIO_OUT_ZERO);
    printk("hgw:qre1113_read_data:data_low=%d  data_high=%d\n", data_low,data_high);

    /*
    if(data_low>650 || (data_high<data_low))
    	//distance_temp = -1;//err
    else
    {
    	distance = data_high-data_low;
    	if(distance > 200)
    		distance_temp = 1;//ok
    	else
    		distance_temp = -1;//err
    }	
    *////modify by wangwei for qre1113 test

    //if(data_high > 800)
    //distance_temp = 1;//ok
    //else if(data_high < 800)
    //distance_temp = 3;//data_high is err,emitter is err

    //#if defined(FACTORY_AUTOTEST_ALSPS_QRE1113)
    /*
    #if 1

    if(data_low>650 || (data_high<data_low))
    //distance_temp = -1;//err
    else
    {
    distance = data_high-data_low;
    if(distance > 200)
    distance_temp = 1;//ok
    else
    distance_temp = -1;//err
    }	
    ///modify by wangwei for qre1113 test

    if(data_high > 800)
    distance_temp = 1;//ok
    else if(data_high < 800) 
    distance_temp = 3;//data_high is err,emitter is err
    #else
    if(data_low>650 || (data_high<data_low))
    distance_temp = -1;//err
    else
    {
    distance = data_high-data_low;
    if(distance > 200)
    distance_temp = 1;//ok
    else
    distance_temp = -1;//err
    }	
    #endif
    */
    if (data_low>1050 || (data_high<data_low)){
        distance_temp = 3;//far away
    }else{
        distance = data_high-data_low;
        //if(distance < ps_threshold){
        if (distance < 1000){
            distance_temp = 3;//far away
        }else{
            distance_temp = 1;//close
        }
    }


    return distance_temp;
}



static int qre1113_calibrate(struct i2c_client *client)
{

    struct qre1113_priv *obj = i2c_get_clientdata(client);
    struct file *fp;
    int ret = 0;
    int ADC_Chanel = qre1113_obj->adc_channel;                                    /*adc chanel */
    int GPIO_Enable = qre1113_obj->mros_en;
    unsigned int    ps_threshold = qre1113_obj->ps_thd_val;
    int data_high,data_low,count_temp;
    UINT16 distance_temp = 0;

    //APS_DBG("PS:ADC_Chanel=%d ,GPIO_Enable=%d ,ps_threshold=%d\n", ADC_Chanel,GPIO_Enable,ps_threshold);

    mt_set_gpio_mode(GPIO_Enable,GPIO_MODE_GPIO);
    mt_set_gpio_dir(GPIO_Enable,GPIO_DIR_OUT);
    data_low = qre1113_GetOneChannelValue(ADC_Chanel,10);
    mt_set_gpio_out(GPIO_Enable,GPIO_OUT_ONE);
    mdelay(1);
    data_high= qre1113_GetOneChannelValue(ADC_Chanel,10);
    mt_set_gpio_out(GPIO_Enable,GPIO_OUT_ZERO);

    //distance_temp = data_high - data_low+20;
    mdelay(1);

    mt_set_gpio_mode(GPIO_Enable,GPIO_MODE_GPIO);
    mt_set_gpio_dir(GPIO_Enable,GPIO_DIR_OUT);
    data_low = qre1113_GetOneChannelValue(ADC_Chanel,10);
    mt_set_gpio_out(GPIO_Enable,GPIO_OUT_ONE);
    mdelay(1);
    data_high= qre1113_GetOneChannelValue(ADC_Chanel,10);
    mt_set_gpio_out(GPIO_Enable,GPIO_OUT_ZERO);


#if defined(BIRD_PS_CALI_ADD_VAL)
    distance_temp = data_high - data_low+simple_strtol(BIRD_PS_CALI_ADD_VAL, NULL, 0);;
#else
    distance_temp = data_high - data_low+600;
#endif
    //printk("hgw111111:qre1113_calibrate open nvram file!\r\n");
    //printk("hgw2222:qre1113_calibrate value is count_temp = %x,data_low = %d , data_high = %d, distance = %d\r\n",count_temp,data_low,data_high,distance_temp);

    //write to nvram
    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);

    fp = filp_open(FTM_CUST_ALSPS,O_WRONLY|O_CREAT, 0644);
    fp->f_pos = 0;
    count_temp = fp->f_op->write(fp,
                                 &distance_temp,
                                 2,
                                 &fp->f_pos);
    filp_close(fp, NULL);
    printk("hgw:qre1113_calibrate value is count_temp = %x,data_low = %d , data_high = %d, distance = %d\r\n",count_temp,data_low,data_high,distance_temp);

    set_fs(old_fs);

    return distance_temp;

}


/******************************************************************************
 * Function Configuration
******************************************************************************/
static int qre1113_open(struct inode *inode, struct file *file)
{
    file->private_data = qre1113_i2c_client;

    if (!file->private_data)
    {
        APS_ERR("null pointer!!\n");
        return -EINVAL;
    }

    return nonseekable_open(inode, file);
}
/*----------------------------------------------------------------------------*/
static int qre1113_release(struct inode *inode, struct file *file)
{
    file->private_data = NULL;
    return 0;
}
/*----------------------------------------------------------------------------*/

//static int qre1113_ioctl(struct inode *inode, struct file *file, unsigned int cmd,unsigned long arg)
static int qre1113_unlocked_ioctl(struct file *file, unsigned int cmd,unsigned long arg)
{
    struct i2c_client *client = (struct i2c_client*)file->private_data;
    struct qre1113_priv *obj = i2c_get_clientdata(client);
    int err = 0;
    void __user *ptr = (void __user*) arg;
    int dat;
    uint32_t enable;
    signed char data_temp = -1;
    int data_flag = -1;
    switch (cmd)
    {
    case ALSPS_SET_PS_MODE:
        if (copy_from_user(&enable, ptr, sizeof(enable)))
        {
            err = -EFAULT;
            goto err_out;
        }
        if (enable)
        {
            if (err = qre1113_enable_ps(obj->client, true))
            {
                APS_ERR("enable ps fail: %d\n", err);
                goto err_out;
            }

            set_bit(CMC_BIT_PS, &obj->enable);
        }
        else
        {
            if (err = qre1113_enable_ps(obj->client, false))
            {
                APS_ERR("disable ps fail: %d\n", err);
                goto err_out;
            }

            clear_bit(CMC_BIT_PS, &obj->enable);
        }
        break;

    case ALSPS_GET_PS_MODE:
        enable = test_bit(CMC_BIT_PS, &obj->enable) ? (1) : (0);
        if (copy_to_user(ptr, &enable, sizeof(enable)))
        {
            err = -EFAULT;
            goto err_out;
        }
        break;

    case ALSPS_GET_PS_DATA:
        if (err = qre1113_read_data(obj->client, &obj->ps))
        {
            goto err_out;
        }
        dat = qre1113_get_ps_value(obj, obj->ps);
        if (copy_to_user(ptr, &dat, sizeof(dat)))
        {
            err = -EFAULT;
            goto err_out;
        }
        break;

    case ALSPS_GET_PS_RAW_DATA:
        if (err = qre1113_read_data(obj->client, &obj->ps))
        {
            goto err_out;
        }

        dat = obj->ps & 0x80;
        if (copy_to_user(ptr, &dat, sizeof(dat)))
        {
            err = -EFAULT;
            goto err_out;
        }
        break;

    case ALSPS_PS_TEST: 					//modify by wangwei for auto test

        if (err = qre1113_enable_ps(obj->client, true))
        {
            APS_ERR("enable ps fail: %d\n", err);
            err = -EFAULT;
            goto err_out;
        }

        data_flag = qre1113_autotest(obj->client);
        printk("hgw_ALSPS_PS_TEST:err is err = %d, data_flag =%d",err,data_flag);
        /*****************
        if(data_flag < 0)
        {
        	//printk("hgw:ioctl_ALSPS_PS_TEST return data_temp 111\r\n");
        	data_temp = 1;		//err
        }
        else
        {
        	//printk("hgw:ioctl_ALSPS_PS_TEST return data_temp 222\r\n");
        	data_temp = 2;		//ok
        }
        *****************/
        //data_temp = data_flag;


        //#if defined(FACTORY_AUTOTEST_ALSPS_QRE1113)
#if 1
        if (data_flag < 0)
        {
            //printk("hgw:ioctl_ALSPS_PS_TEST return data_temp 111\r\n");
            data_temp = 1;		//err
        }
        else
        {
            //printk("hgw:ioctl_ALSPS_PS_TEST return data_temp 222\r\n");
            data_temp = 2;		//ok
        }
        data_temp = data_flag;
#else
        if (data_flag < 0)
        {
            //printk("hgw:ioctl_ALSPS_PS_TEST return data_temp 111\r\n");
            data_temp = 1;		//err
        }
        else
        {
            //printk("hgw:ioctl_ALSPS_PS_TEST return data_temp 222\r\n");
            data_temp = 2;		//ok
        }
#endif

        if (copy_to_user(ptr, &data_temp, sizeof(data_temp)))
        {
            err = -EFAULT;
            goto err_out;
        }
        break;

    case ALSPS_SET_PS_CALIBRATION:
        if (err = qre1113_enable_ps(obj->client, true))
        {
            APS_ERR("enable ps fail: %d\n", err);
            err = -EFAULT;
            goto err_out;
        }
        qre1113_obj->ps_thd_val = qre1113_calibrate(obj->client);
        printk("hgw____:ALSPS_SET_PS_CALIBRATION distance = %d",qre1113_obj->ps_thd_val);
        if (err = qre1113_enable_ps(obj->client, false))
        {
            APS_ERR("enable ps fail: %d\n", err);
            err = -EFAULT;
            goto err_out;
        }
        break;
        /**/
    default:
        APS_ERR("%s not supported = 0x%04x", __FUNCTION__, cmd);
        err = -ENOIOCTLCMD;
        break;
    }

err_out:
    return err;
}
/*----------------------------------------------------------------------------*/
static struct file_operations qre1113_fops = {
            //.owner = THIS_MODULE,
            .open = qre1113_open,
                    .release = qre1113_release,
                               .unlocked_ioctl = qre1113_unlocked_ioctl,
                                             };
/*----------------------------------------------------------------------------*/
static struct miscdevice qre1113_device = {
            .minor = MISC_DYNAMIC_MINOR,
                     .name = "als_ps",
                             .fops = &qre1113_fops,
                                 };
/*----------------------------------------------------------------------------*/
static int qre1113_i2c_suspend(struct i2c_client *client, pm_message_t msg)
{
    struct qre1113_priv *obj = i2c_get_clientdata(client);
    int err;
    APS_FUN();

    if (msg.event == PM_EVENT_SUSPEND)
    {
        if (!obj)
        {
            APS_ERR("null pointer!!\n");
            return -EINVAL;
        }
        atomic_set(&obj->ps_suspend, 1);
        if (err = qre1113_enable_ps(client, false))
        {
            APS_ERR("disable ps:  %d\n", err);
            return err;
        }

        qre1113_power(obj->hw, 0);
    }
    return 0;
}
/*----------------------------------------------------------------------------*/
static int qre1113_i2c_resume(struct i2c_client *client)
{
    struct qre1113_priv *obj = i2c_get_clientdata(client);
    int err;
    APS_FUN();

    if (!obj)
    {
        APS_ERR("null pointer!!\n");
        return -EINVAL;
    }

    qre1113_power(obj->hw, 1);
    if (err = qre1113_init_client(client))
    {
        APS_ERR("initialize client fail!!\n");
        return err;
    }
    atomic_set(&obj->ps_suspend, 0);
    if (test_bit(CMC_BIT_PS,  &obj->enable))
    {
        if (err = qre1113_enable_ps(client, true))
        {
            APS_ERR("enable ps fail: %d\n", err);
        }
    }

    return 0;
}
/*----------------------------------------------------------------------------*/
static void qre1113_early_suspend(struct early_suspend *h)
{   /*early_suspend is only applied for ALS*/
    struct qre1113_priv *obj = container_of(h, struct qre1113_priv, early_drv);
    int err;
    APS_FUN();

    if (!obj)
    {
        APS_ERR("null pointer!!\n");
        return;
    }
}
/*----------------------------------------------------------------------------*/
static void qre1113_late_resume(struct early_suspend *h)
{   /*early_suspend is only applied for ALS*/
    struct qre1113_priv *obj = container_of(h, struct qre1113_priv, early_drv);
    int err;
    APS_FUN();

    if (!obj)
    {
        APS_ERR("null pointer!!\n");
        return;
    }
}

int qre1113_ps_operate(void* self, uint32_t command, void* buff_in, int size_in,
                       void* buff_out, int size_out, int* actualout)
{
    int err = 0;
    int value;
    hwm_sensor_data* sensor_data;
    struct qre1113_priv *obj = (struct qre1113_priv *)self;

    //APS_FUN(f);
    switch (command)
    {
    case SENSOR_DELAY:
        if ((buff_in == NULL) || (size_in < sizeof(int)))
        {
            APS_ERR("Set delay parameter error!\n");
            err = -EINVAL;
        }
        // Do nothing
        break;

    case SENSOR_ENABLE:
        if ((buff_in == NULL) || (size_in < sizeof(int)))
        {
            APS_ERR("Enable sensor parameter error!\n");
            err = -EINVAL;
        }
        else
        {
            value = *(int *)buff_in;
            if (value)
            {
                if (err = qre1113_enable_ps(obj->client, true))
                {
                    APS_ERR("enable ps fail: %d\n", err);
                    return -1;
                }
                set_bit(CMC_BIT_PS, &obj->enable);
            }
            else
            {
                if (err = qre1113_enable_ps(obj->client, false))
                {
                    APS_ERR("disable ps fail: %d\n", err);
                    return -1;
                }
                clear_bit(CMC_BIT_PS, &obj->enable);
            }
        }
        break;

    case SENSOR_GET_DATA:
        //APS_LOG("fwq get ps data !!!!!!\n");
        if ((buff_out == NULL) || (size_out< sizeof(hwm_sensor_data)))
        {
            APS_ERR("get sensor data parameter error!\n");
            err = -EINVAL;
        }
        else
        {
            sensor_data = (hwm_sensor_data *)buff_out;

            if (err = qre1113_read_data(obj->client, &obj->ps))
            {
                err = -1;;
            }
            else
            {
                while (-1 == qre1113_get_ps_value(obj, obj->ps))
                {
                    qre1113_read_data(obj->client, &obj->ps);
                    msleep(50);
                }

                sensor_data->values[0] = qre1113_get_ps_value(obj, obj->ps);
                sensor_data->value_divide = 1;
                sensor_data->status = SENSOR_STATUS_ACCURACY_MEDIUM;
                //APS_LOG("fwq get ps data =%d\n",sensor_data->values[0]);


            }
        }
        break;
    default:
        APS_ERR("proxmy sensor operate function no this parameter %d!\n", command);
        err = -1;
        break;
    }

    return err;
}

/*----------------------------------------------------------------------------*/
/*
static int qre1113_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info)
{
	strcpy(info->type, QRE1113_DEV_NAME);
	return 0;
}*/

/*----------------------------------------------------------------------------*/
static int qre1113_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    APS_FUN();
    struct qre1113_priv *obj;
    struct hwmsen_object obj_ps;
    int err = 0;

    if (!(obj = kzalloc(sizeof(*obj), GFP_KERNEL)))
    {
        err = -ENOMEM;
        goto exit;
    }
    memset(obj, 0, sizeof(*obj));
    qre1113_obj = obj;

    obj->hw = get_cust_alsps_hw();


    //INIT_DELAYED_WORK(&obj->eint_work, qre1113_eint_work);
    obj->client = client;
    i2c_set_clientdata(client, obj);

    atomic_set(&obj->ps_debounce, 1000);
    atomic_set(&obj->ps_deb_on, 0);
    atomic_set(&obj->ps_deb_end, 0);
    atomic_set(&obj->ps_mask, 0);

    atomic_set(&obj->trace, 0);

    obj->ps_enable = 0;
    obj->enable = 0;
    atomic_set(&obj->i2c_retry, 3);
    //pre set ps threshold
    obj->ps_thd_val = obj->hw->ps_threshold;
    obj->adc_channel= obj->hw->adc_channel;
    obj->mros_en= obj->hw->mros_en;
    obj->mros_gpio= obj->hw->mros_gpio;

    qre1113_i2c_client = client;


    if (err = qre1113_init_client(client))
    {
        goto exit_init_failed;
    }

    if (err = misc_register(&qre1113_device))
    {
        APS_ERR("qre1113_device register failed\n");
        goto exit_misc_device_register_failed;
    }

    if (err = qre1113_create_attr(&qre1113_alsps_driver.driver))
    {
        APS_ERR("create attribute err = %d\n", err);
        goto exit_create_attr_failed;
    }
    obj_ps.self = qre1113_obj;
    if (1 == obj->hw->polling_mode)
    {
        obj_ps.polling = 1;
    }
    else
    {
        obj_ps.polling = 0;//interrupt mode
    }
    obj_ps.sensor_operate = qre1113_ps_operate;
    if (err = hwmsen_attach(ID_PROXIMITY, &obj_ps))
    {
        APS_ERR("attach fail = %d\n", err);
        goto exit_create_attr_failed;
    }


#if defined(CONFIG_HAS_EARLYSUSPEND)
    obj->early_drv.level    = EARLY_SUSPEND_LEVEL_DISABLE_FB - 1,
                              obj->early_drv.suspend  = qre1113_early_suspend,
                                                        obj->early_drv.resume   = qre1113_late_resume,
                                                                                  register_early_suspend(&obj->early_drv);
#endif

    APS_LOG("%s: OK\n", __func__);
    return 0;

exit_create_attr_failed:
    misc_deregister(&qre1113_device);
exit_misc_device_register_failed:
exit_init_failed:
    //i2c_detach_client(client);
exit_kfree:
    kfree(obj);
exit:
    qre1113_i2c_client = NULL;
#ifdef MT6516
    MT6516_EINTIRQMask(CUST_EINT_ALS_NUM);  /*mask interrupt if fail*/
#endif
    APS_ERR("%s: err = %d\n", __func__, err);
    return err;
}
/*----------------------------------------------------------------------------*/
static int qre1113_i2c_remove(struct i2c_client *client)
{
    int err;

    if (err = qre1113_delete_attr(&qre1113_i2c_driver.driver))
    {
        APS_ERR("qre1113_delete_attr fail: %d\n", err);
    }

    if (err = misc_deregister(&qre1113_device))
    {
        APS_ERR("misc_deregister fail: %d\n", err);
    }

    qre1113_i2c_client = NULL;
    i2c_unregister_device(client);
    kfree(i2c_get_clientdata(client));

    return 0;
}
/*----------------------------------------------------------------------------*/
static int qre1113_probe(struct platform_device *pdev)
{
    struct alsps_hw *hw = get_cust_alsps_hw();
    //struct qre1113_i2c_addr addr;

    qre1113_power(hw, 1);
    //qre1113_get_addr(hw, &addr);
    wake_lock_init(&ps_lock, WAKE_LOCK_SUSPEND, "ps wakelock");
    //qre1113_force[0] = hw->i2c_num;
    //qre1113_force[1] = addr.init;
    if (i2c_add_driver(&qre1113_i2c_driver))
    {
        APS_ERR("add driver error\n");
        return -1;
    }
    return 0;
}
/*----------------------------------------------------------------------------*/
static int qre1113_remove(struct platform_device *pdev)
{
    struct alsps_hw *hw = get_cust_alsps_hw();
    APS_FUN();
    qre1113_power(hw, 0);
    i2c_del_driver(&qre1113_i2c_driver);
    return 0;
}
/*----------------------------------------------------------------------------*/
static struct platform_driver qre1113_alsps_driver = {
            .probe      = qre1113_probe,
                          .remove     = qre1113_remove,
                                        .driver     = {
                                                          .name  = "als_ps",
                                                          //	.owner = THIS_MODULE,
                                                      }
                                                  };
/*----------------------------------------------------------------------------*/
static int __init qre1113_init(void)
{
    APS_FUN();
    i2c_register_board_info(0, &i2c_qre1113, 1);
    if (platform_driver_register(&qre1113_alsps_driver))
    {
        APS_ERR("failed to register driver");
        return -ENODEV;
    }
    return 0;
}
/*----------------------------------------------------------------------------*/
static void __exit qre1113_exit(void)
{
    APS_FUN();
    platform_driver_unregister(&qre1113_alsps_driver);
}
/*----------------------------------------------------------------------------*/
module_init(qre1113_init);
module_exit(qre1113_exit);
/*----------------------------------------------------------------------------*/
MODULE_AUTHOR("MingHsien Hsieh");
MODULE_DESCRIPTION("QRE1113 ALS/PS driver");
MODULE_LICENSE("GPL");
