/* drivers/hwmon/mt6516/amit/epl2182.c - EPL2182 ALS/PS driver
 *
 * Author: MingHsien Hsieh <minghsien.hsieh@mediatek.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/** VERSION: 1.03**/

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
#include <linux/hwmsen_helper.h>
#include <linux/kernel.h>
#include "epl2182.h"


#ifdef MT6516
#include <mach/mt6516_devs.h>
#include <mach/mt6516_typedefs.h>
#include <mach/mt6516_gpio.h>
#include <mach/mt6516_pll.h>
#endif

#ifdef MT6573
#include <mach/mt6573_devs.h>
#include <mach/mt6573_typedefs.h>
#include <mach/mt6573_gpio.h>
#include <mach/mt6573_pll.h>
#endif

#ifdef MT6513
#include <mach/mt6513_devs.h>
#include <mach/mt6513_typedefs.h>
#include <mach/mt6513_gpio.h>
#include <mach/mt6513_pll.h>
#endif

#ifdef MT6575
#include <mach/mt6575_devs.h>
#include <mach/mt6575_typedefs.h>
#include <mach/mt6575_gpio.h>
#include <mach/mt6575_pm_ldo.h>
#endif

#ifdef MT6577
#include <mach/mt6577_devs.h>
#include <mach/mt6577_typedefs.h>
#include <mach/mt6577_gpio.h>
#include <mach/mt6577_pm_ldo.h>
#endif

#ifdef MT6589
#include <mach/devs.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#endif

#ifdef MT6572
#include <mach/devs.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#endif

//#include <Mt6575.h>
//bob.chen add begin
//add for fix resume issue
#include <linux/earlysuspend.h>
#include <linux/wakelock.h>
//add for fix resume issue end
//bob.chen add end


/******************************************************************************
 * extern functions
*******************************************************************************/
#ifdef MT6516
extern void MT6516_EINTIRQUnmask(unsigned int line);
extern void MT6516_EINTIRQMask(unsigned int line);
extern void MT6516_EINT_Set_Polarity(kal_uint8 eintno, kal_bool ACT_Polarity);
extern void MT6516_EINT_Set_HW_Debounce(kal_uint8 eintno, kal_uint32 ms);
extern kal_uint32 MT6516_EINT_Set_Sensitivity(kal_uint8 eintno, kal_bool sens);
extern void MT6516_EINT_Registration(kal_uint8 eintno, kal_bool Dbounce_En,
                                     kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
                                     kal_bool auto_umask);
#endif


#ifdef MT6513
extern void mt65xx_eint_unmask(unsigned int line);
extern void mt65xx_eint_mask(unsigned int line);
extern void mt65xx_eint_set_polarity(kal_uint8 eintno, kal_bool ACT_Polarity);
extern void mt65xx_eint_set_hw_debounce(kal_uint8 eintno, kal_uint32 ms);
extern kal_uint32 mt65xx_eint_set_sens(kal_uint8 eintno, kal_bool sens);
extern void mt65xx_eint_registration(kal_uint8 eintno, kal_bool Dbounce_En,
                                     kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
                                     kal_bool auto_umask);
#endif

#ifdef MT6573
extern void mt65xx_eint_unmask(unsigned int line);
extern void mt65xx_eint_mask(unsigned int line);
extern void mt65xx_eint_set_polarity(kal_uint8 eintno, kal_bool ACT_Polarity);
extern void mt65xx_eint_set_hw_debounce(kal_uint8 eintno, kal_uint32 ms);
extern kal_uint32 mt65xx_eint_set_sens(kal_uint8 eintno, kal_bool sens);
extern void mt65xx_eint_registration(kal_uint8 eintno, kal_bool Dbounce_En,
                                     kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
                                     kal_bool auto_umask);
#endif


#ifdef MT6575
extern void mt65xx_eint_unmask(unsigned int line);
extern void mt65xx_eint_mask(unsigned int line);
extern void mt65xx_eint_set_polarity(kal_uint8 eintno, kal_bool ACT_Polarity);
extern void mt65xx_eint_set_hw_debounce(kal_uint8 eintno, kal_uint32 ms);
extern kal_uint32 mt65xx_eint_set_sens(kal_uint8 eintno, kal_bool sens);
extern void mt65xx_eint_registration(kal_uint8 eintno, kal_bool Dbounce_En,
                                     kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
                                     kal_bool auto_umask);
#endif

#ifdef MT6577
extern void mt65xx_eint_unmask(unsigned int line);
extern void mt65xx_eint_mask(unsigned int line);
extern void mt65xx_eint_set_polarity(kal_uint8 eintno, kal_bool ACT_Polarity);
extern void mt65xx_eint_set_hw_debounce(kal_uint8 eintno, kal_uint32 ms);
extern kal_uint32 mt65xx_eint_set_sens(kal_uint8 eintno, kal_bool sens);
extern void mt65xx_eint_registration(kal_uint8 eintno, kal_bool Dbounce_En,
                                     kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
                                     kal_bool auto_umask);
#endif

#ifdef MT6589
extern void mt65xx_eint_unmask(unsigned int line);
extern void mt65xx_eint_mask(unsigned int line);
extern void mt65xx_eint_set_polarity(kal_uint8 eintno, kal_bool ACT_Polarity);
extern void mt65xx_eint_set_hw_debounce(kal_uint8 eintno, kal_uint32 ms);
extern kal_uint32 mt65xx_eint_set_sens(kal_uint8 eintno, kal_bool sens);
extern void mt65xx_eint_registration(kal_uint8 eintno, kal_bool Dbounce_En,
                                     kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
                                     kal_bool auto_umask);
#endif

#ifdef MT6572
extern void mt65xx_eint_unmask(unsigned int line);
extern void mt65xx_eint_mask(unsigned int line);
extern void mt65xx_eint_set_polarity(kal_uint8 eintno, kal_bool ACT_Polarity);
extern void mt65xx_eint_set_hw_debounce(kal_uint8 eintno, kal_uint32 ms);
extern kal_uint32 mt65xx_eint_set_sens(kal_uint8 eintno, kal_bool sens);
extern void mt65xx_eint_registration(kal_uint8 eintno, kal_bool Dbounce_En,
                                     kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
                                     kal_bool auto_umask);
#endif

/*-------------------------MT6516&MT6573 define-------------------------------*/
#ifdef MT6516
#define POWER_NONE_MACRO MT6516_POWER_NONE
#endif
#ifdef MT6573
#define POWER_NONE_MACRO MT65XX_POWER_NONE
#endif
#ifdef MT6513
#define POWER_NONE_MACRO MT65XX_POWER_NONE
#endif
#ifdef MT6575
#define POWER_NONE_MACRO MT65XX_POWER_NONE
#endif
#ifdef MT6577
#define POWER_NONE_MACRO MT65XX_POWER_NONE
#endif
#ifdef MT6589
#define POWER_NONE_MACRO MT65XX_POWER_NONE
#endif
#ifdef MT6572
#define POWER_NONE_MACRO MT65XX_POWER_NONE
#endif
/******************************************************************************
 * configuration
*******************************************************************************/
#define LUX_PER_COUNT		1100              // 1100 = 1.1 * 1000
#define PS_DRIVE				EPL_DRIVE_120MA

static int PS_INTT 				= 4;
static int ALS_INTT 			= 7;

#define PS_DELAY 			15
#define ALS_DELAY 			55
/******************************************************************************
*******************************************************************************/

#define TXBYTES 				2
#define RXBYTES 				2

#define PACKAGE_SIZE 		2
#define I2C_RETRY_COUNT 	10

#define EPL2182_DEV_NAME     "EPL2182"

typedef struct _epl_raw_data
{
    u8 raw_bytes[PACKAGE_SIZE];
    u16 ps_state;
    u16 ps_raw;
    u16 als_ch0_raw;
    u16 als_ch1_raw;
    u16 als_lux;
} epl_raw_data;

 struct PS_CALI_DATA_STRUCT
{
    int high_thd;
    int low_thd;
    int valid;
} ;

static struct PS_CALI_DATA_STRUCT ps_cali={0,0,0};
static UINT16 distance_ps_threshold_flag = 0;	

/*----------------------------------------------------------------------------*/
#define APS_TAG                  "[ALS/PS] "
#define APS_FUN(f)               printk(KERN_ERR APS_TAG"%s\n", __FUNCTION__)
#define APS_ERR(fmt, args...)    printk(KERN_ERR  APS_TAG"%s %d : "fmt, __FUNCTION__, __LINE__, ##args)
#define APS_LOG(fmt, args...)    printk(KERN_ERR APS_TAG fmt, ##args)
#define APS_DBG(fmt, args...)    printk(KERN_ERR fmt, ##args)
#define FTM_CUST_ALSPS "/data/epl2182"

/*----------------------------------------------------------------------------*/
static struct i2c_client *epl2182_i2c_client = NULL;


/*----------------------------------------------------------------------------*/
static const struct i2c_device_id epl2182_i2c_id[] = {{"EPL2182",0},{}};
static struct i2c_board_info __initdata i2c_EPL2182= { I2C_BOARD_INFO("EPL2182", (0X92>>1))};
/*the adapter id & i2c address will be available in customization*/
//static unsigned short epl2182_force[] = {0x00, 0x92, I2C_CLIENT_END, I2C_CLIENT_END};
//static const unsigned short *const epl2182_forces[] = { epl2182_force, NULL };
//static struct i2c_client_address_data epl2182_addr_data = { .forces = epl2182_forces,};


/*----------------------------------------------------------------------------*/
static int epl2182_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int epl2182_i2c_remove(struct i2c_client *client);
static int epl2182_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);


/*----------------------------------------------------------------------------*/
static int epl2182_i2c_suspend(struct i2c_client *client, pm_message_t msg);
static int epl2182_i2c_resume(struct i2c_client *client);

static void epl2182_eint_func(void);
static int set_psensor_intr_threshold(uint16_t low_thd, uint16_t high_thd);

static struct epl2182_priv *g_epl2182_ptr = NULL;

/*----------------------------------------------------------------------------*/
typedef enum
{
    CMC_TRC_ALS_DATA 	= 0x0001,
    CMC_TRC_PS_DATA 	= 0X0002,
    CMC_TRC_EINT    		= 0x0004,
    CMC_TRC_IOCTL   		= 0x0008,
    CMC_TRC_I2C     		= 0x0010,
    CMC_TRC_CVT_ALS 	= 0x0020,
    CMC_TRC_CVT_PS  		= 0x0040,
    CMC_TRC_DEBUG   		= 0x0800,
} CMC_TRC;

/*----------------------------------------------------------------------------*/
typedef enum
{
    CMC_BIT_ALS    = 1,
    CMC_BIT_PS     = 2,
} CMC_BIT;

/*----------------------------------------------------------------------------*/
struct epl2182_i2c_addr      /*define a series of i2c slave address*/
{
    u8  write_addr;
    u8  ps_thd;     /*PS INT threshold*/
};

/*----------------------------------------------------------------------------*/
struct epl2182_priv
{
    struct alsps_hw  *hw;
    struct i2c_client *client;
    struct delayed_work  eint_work;
    struct delayed_work  polling_work;

    /*i2c address group*/
    struct epl2182_i2c_addr  addr;


    /*misc*/
    atomic_t    trace;
    atomic_t    als_suspend;
    atomic_t    ps_suspend;

    /*data*/
    u16		lux_per_count;
    ulong       enable;         /*record HAL enalbe status*/
    ulong       pending_intr;   /*pending interrupt*/

    /*data*/
    u16         als_level_num;
    u16         als_value_num;
    u32         als_level[C_CUST_ALS_LEVEL-1];
    u32         als_value[C_CUST_ALS_LEVEL];

    /*early suspend*/
#if defined(CONFIG_HAS_EARLYSUSPEND)
    struct early_suspend    early_drv;
#endif
};



/*----------------------------------------------------------------------------*/
static struct i2c_driver epl2182_i2c_driver =
{
    .probe      	= epl2182_i2c_probe,
    .remove     = epl2182_i2c_remove,
    .detect     	= epl2182_i2c_detect,
    .suspend    = epl2182_i2c_suspend,
    .resume     = epl2182_i2c_resume,
    .id_table   	= epl2182_i2c_id,
    //.address_data = &epl2182_addr_data,
    .driver = {
        //.owner          = THIS_MODULE,
        .name           = EPL2182_DEV_NAME,
    },
};


static struct epl2182_priv *epl2182_obj = NULL;
static struct platform_driver epl2182_alsps_driver;
static struct wake_lock ps_lock;
static epl_raw_data	gRawData;


/*
//====================I2C write operation===============//
//regaddr: ELAN epl2182 Register Address.
//bytecount: How many bytes to be written to epl2182 register via i2c bus.
//txbyte: I2C bus transmit byte(s). Single byte(0X01) transmit only slave address.
//data: setting value.
//
// Example: If you want to write single byte to 0x1D register address, show below
//	      elan_epl2182_I2C_Write(client,0x1D,0x01,0X02,0xff);
//
*/
static int elan_epl2182_I2C_Write(struct i2c_client *client, uint8_t regaddr, uint8_t bytecount, uint8_t txbyte, uint8_t data)
{
    uint8_t buffer[2];
    int ret = 0;
    int retry;

    buffer[0] = (regaddr<<3) | bytecount ;
    buffer[1] = data;

    for(retry = 0; retry < I2C_RETRY_COUNT; retry++)
    {
        ret = i2c_master_send(client, buffer, txbyte);

        if (ret == txbyte)
        {
            break;
        }

        APS_ERR("i2c write error,TXBYTES %d\n",ret);
        mdelay(10);
    }


    if(retry>=I2C_RETRY_COUNT)
    {
        APS_ERR("i2c write retry over %d\n", I2C_RETRY_COUNT);
        return -EINVAL;
    }

    return ret;
}




/*
//====================I2C read operation===============//
*/
static int elan_epl2182_I2C_Read(struct i2c_client *client)
{
    uint8_t buffer[RXBYTES];
    int ret = 0, i =0;
    int retry;

    for(retry = 0; retry < I2C_RETRY_COUNT; retry++)
    {
        ret = i2c_master_recv(client, buffer, RXBYTES);

        if (ret == RXBYTES)
            break;

        APS_ERR("i2c read error,RXBYTES %d\r\n",ret);
        mdelay(10);
    }

    if(retry>=I2C_RETRY_COUNT)
    {
        APS_ERR("i2c read retry over %d\n", I2C_RETRY_COUNT);
        return -EINVAL;
    }

    for(i=0; i<PACKAGE_SIZE; i++)
        gRawData.raw_bytes[i] = buffer[i];

    return ret;
}




static int elan_epl2182_psensor_enable(struct epl2182_priv *epl_data, int enable)
{
    int ret = 0;
    int ps_state;
    uint8_t regdata;
		unsigned int    ps_threshold;
		int count_temp;	
		char buf[4];
		struct file *fp;
		uint16_t low_thd_val,high_thd_val;
    struct i2c_client *client = epl_data->client;

    //printk("mycat [ELAN epl2182] %s enable = %d distance_ps_threshold_flag=%d \n", __func__, enable,distance_ps_threshold_flag);
    
    if(distance_ps_threshold_flag == 0)
		{
			distance_ps_threshold_flag = 1;
			mm_segment_t old_fs = get_fs();
		    	set_fs(KERNEL_DS);
			
			fp = filp_open(FTM_CUST_ALSPS,O_RDONLY,0);
	    printk("mycat open file fp = 0x%x \r\n",fp);	
	    
			if(fp != 0xfffffffe)//open file suscess
			{
				printk("mycat open file suscess!\n");	
				//fp->f_op->llseek(fp, 0, 0);
				fp->f_pos = 0;
				count_temp = fp->f_op->read(fp,
											  &buf,
											  4,
											  &fp->f_pos);	
				filp_close(fp, NULL);	
				set_fs(old_fs);
				//atomic_set(&obj->ps_thd_val,  ps_threshold); 			
				
				//obj->ps_thd_val = ps_threshold;
				ps_cali.high_thd = ((buf[0]<<8))&0xff00 | (buf[1]);
				ps_cali.low_thd = ((buf[2]<<8))&0xff00 | (buf[3]);
				ps_cali.valid = 1;
				//printk("mycat in file is ps_cali.high_thd = %d ps_cali.low_thd = %d ps_cali.valid = %d buf[0] = %x,buf[1] = %x,buf[2] = %x,buf[3] = %x,\n", ps_cali.high_thd,ps_cali.low_thd,ps_cali.valid,buf[0],buf[1],buf[2],buf[3]);	
			}
	
		}

    ret = elan_epl2182_I2C_Write(client,REG_9,W_SINGLE_BYTE,0x02,EPL_INT_DISABLE | PS_DRIVE);

    if(enable)
    {
        regdata = EPL_SENSING_2_TIME | EPL_PS_MODE | EPL_L_GAIN ;
        regdata = regdata | (epl_data->hw->polling_mode_ps == 0 ? EPL_C_SENSING_MODE : EPL_S_SENSING_MODE);
        ret = elan_epl2182_I2C_Write(client,REG_0,W_SINGLE_BYTE,0X02,regdata);

        regdata = PS_INTT<<4 | EPL_PST_1_TIME | EPL_10BIT_ADC;
        ret = elan_epl2182_I2C_Write(client,REG_1,W_SINGLE_BYTE,0X02,regdata);
				
				printk("mycat ps_cali.valid = 0x%x ps_cali.low_thd = %d ps_cali.high_thd = %d \r\n",ps_cali.valid,ps_cali.low_thd,ps_cali.high_thd);	
				if(ps_cali.valid == 1)
				{
						set_psensor_intr_threshold(ps_cali.low_thd,ps_cali.high_thd);
				}
				else
				{
						#ifdef BIRD_PS_HIGH_THD_VAL
						    high_thd_val = simple_strtol(BIRD_PS_HIGH_THD_VAL, NULL, 0);
						#else
								high_thd_val = epl_data->hw ->ps_threshold_high;
						#endif
						
						#ifdef BIRD_PS_LOW_THD_VAL
						    low_thd_val = simple_strtol(BIRD_PS_LOW_THD_VAL, NULL, 0);
						#else
								low_thd_val = epl_data->hw ->ps_threshold_low;
						#endif
						
						set_psensor_intr_threshold(low_thd_val,high_thd_val);
				}

        ret = elan_epl2182_I2C_Write(client,REG_7,W_SINGLE_BYTE,0X02,EPL_C_RESET);
        ret = elan_epl2182_I2C_Write(client,REG_7,W_SINGLE_BYTE,0x02,EPL_C_START_RUN);

        msleep(PS_DELAY);

        if(epl_data->hw->polling_mode_ps == 0)
        {
        
            elan_epl2182_I2C_Write(client,REG_13,R_SINGLE_BYTE,0x01,0);
            elan_epl2182_I2C_Read(client);
            ps_state= !((gRawData.raw_bytes[0]&0x04)>>2);

            if(gRawData.ps_state != ps_state)
            {
                elan_epl2182_I2C_Write(client,REG_9,W_SINGLE_BYTE,0x02,EPL_INT_FRAME_ENABLE|PS_DRIVE);
            }
            else
            {
                elan_epl2182_I2C_Write(client,REG_9,W_SINGLE_BYTE,0x02,EPL_INT_ACTIVE_LOW |PS_DRIVE);
            }
        }

    }
    else
    {
        regdata = EPL_SENSING_2_TIME | EPL_PS_MODE | EPL_L_GAIN | EPL_S_SENSING_MODE;
        ret = elan_epl2182_I2C_Write(client,REG_0,W_SINGLE_BYTE,0X02,regdata);
    }

    if(ret<0)
    {
        APS_ERR("[ELAN epl2182 error]%s: ps enable %d fail\n",__func__,ret);
    }
    else
    {
        ret = 0;
    }

    return ret;
}


static int elan_epl2182_lsensor_enable(struct epl2182_priv *epl_data, int enable)
{
    int ret = 0;
    uint8_t regdata;
    int mode;
    struct i2c_client *client = epl_data->client;

    APS_LOG("[ELAN epl2182] %s enable = %d\n", __func__, enable);

    if(enable)
    {
        regdata = EPL_INT_DISABLE;
        ret = elan_epl2182_I2C_Write(client,REG_9,W_SINGLE_BYTE,0x02, regdata);

        regdata = EPL_S_SENSING_MODE | EPL_SENSING_8_TIME | EPL_ALS_MODE | EPL_AUTO_GAIN;
        ret = elan_epl2182_I2C_Write(client,REG_0,W_SINGLE_BYTE,0X02,regdata);

        regdata = ALS_INTT<<4 | EPL_PST_1_TIME | EPL_10BIT_ADC;
        ret = elan_epl2182_I2C_Write(client,REG_1,W_SINGLE_BYTE,0X02,regdata);

        ret = elan_epl2182_I2C_Write(client,REG_10,W_SINGLE_BYTE,0X02,EPL_GO_MID);
        ret = elan_epl2182_I2C_Write(client,REG_11,W_SINGLE_BYTE,0x02,EPL_GO_LOW);

        ret = elan_epl2182_I2C_Write(client,REG_7,W_SINGLE_BYTE,0X02,EPL_C_RESET);
        ret = elan_epl2182_I2C_Write(client,REG_7,W_SINGLE_BYTE,0x02,EPL_C_START_RUN);
        msleep(ALS_DELAY);
    }


    if(ret<0)
    {
        APS_ERR("[ELAN epl2182 error]%s: als_enable %d fail\n",__func__,ret);
    }
    else
    {
        ret = 0;
    }

    return ret;
}

static int epl2182_get_als_value(struct epl2182_priv *obj, u16 als)
{
    int idx;
    int invalid = 0;
    int lux = 0;

    lux = (als * obj->lux_per_count)/1000;

    for(idx = 0; idx < obj->als_level_num; idx++)
    {
        if(lux < obj->hw->als_level[idx])
        {
            break;
        }
    }

    if(idx >= obj->als_value_num)
    {
        APS_ERR("exceed range\n");
        idx = obj->als_value_num - 1;
    }

    if(!invalid)
    {
        gRawData.als_lux = obj->hw->als_value[idx];
        APS_DBG("ALS: %05d => %05d\n", als, obj->hw->als_value[idx]);
        return obj->hw->als_value[idx];
    }
    else
    {
        APS_ERR("ALS: %05d => %05d (-1)\n", als, obj->hw->als_value[idx]);
        return gRawData.als_lux;
    }
}


static int set_psensor_intr_threshold(uint16_t low_thd, uint16_t high_thd)
{
    int ret = 0;
    struct epl2182_priv *epld = epl2182_obj;
    struct i2c_client *client = epld->client;

    uint8_t high_msb ,high_lsb, low_msb, low_lsb;

    APS_LOG("%s\n", __func__);

    high_msb = (uint8_t) (high_thd >> 8);
    high_lsb = (uint8_t) (high_thd & 0x00ff);
    low_msb  = (uint8_t) (low_thd >> 8);
    low_lsb  = (uint8_t) (low_thd & 0x00ff);

    APS_LOG("%s: low_thd = 0x%X, high_thd = 0x%x \n",__func__, low_thd, high_thd);

    elan_epl2182_I2C_Write(client,REG_2,W_SINGLE_BYTE,0x02,high_lsb);
    elan_epl2182_I2C_Write(client,REG_3,W_SINGLE_BYTE,0x02,high_msb);
    elan_epl2182_I2C_Write(client,REG_4,W_SINGLE_BYTE,0x02,low_lsb);
    elan_epl2182_I2C_Write(client,REG_5,W_SINGLE_BYTE,0x02,low_msb);

    return ret;
}



/*----------------------------------------------------------------------------*/
static void epl2182_dumpReg(struct i2c_client *client)
{
    APS_LOG("chip id REG 0x00 value = %8x\n", i2c_smbus_read_byte_data(client, 0x00));
    APS_LOG("chip id REG 0x01 value = %8x\n", i2c_smbus_read_byte_data(client, 0x08));
    APS_LOG("chip id REG 0x02 value = %8x\n", i2c_smbus_read_byte_data(client, 0x10));
    APS_LOG("chip id REG 0x03 value = %8x\n", i2c_smbus_read_byte_data(client, 0x18));
    APS_LOG("chip id REG 0x04 value = %8x\n", i2c_smbus_read_byte_data(client, 0x20));
    APS_LOG("chip id REG 0x05 value = %8x\n", i2c_smbus_read_byte_data(client, 0x28));
    APS_LOG("chip id REG 0x06 value = %8x\n", i2c_smbus_read_byte_data(client, 0x30));
    APS_LOG("chip id REG 0x07 value = %8x\n", i2c_smbus_read_byte_data(client, 0x38));
    APS_LOG("chip id REG 0x09 value = %8x\n", i2c_smbus_read_byte_data(client, 0x48));
    APS_LOG("chip id REG 0x0D value = %8x\n", i2c_smbus_read_byte_data(client, 0x68));
    APS_LOG("chip id REG 0x0E value = %8x\n", i2c_smbus_read_byte_data(client, 0x70));
    APS_LOG("chip id REG 0x0F value = %8x\n", i2c_smbus_read_byte_data(client, 0x71));
    APS_LOG("chip id REG 0x10 value = %8x\n", i2c_smbus_read_byte_data(client, 0x80));
    APS_LOG("chip id REG 0x11 value = %8x\n", i2c_smbus_read_byte_data(client, 0x88));
    APS_LOG("chip id REG 0x13 value = %8x\n", i2c_smbus_read_byte_data(client, 0x98));

}


/*----------------------------------------------------------------------------*/
int hw8k_init_device(struct i2c_client *client)
{
    APS_LOG("hw8k_init_device.........\r\n");

    epl2182_i2c_client=client;

    APS_LOG(" I2C Addr==[0x%x],line=%d\n",epl2182_i2c_client->addr,__LINE__);

    return 0;
}

/*----------------------------------------------------------------------------*/
int epl2182_get_addr(struct alsps_hw *hw, struct epl2182_i2c_addr *addr)
{
    if(!hw || !addr)
    {
        return -EFAULT;
    }
    addr->write_addr= hw->i2c_addr[0];
    return 0;
}


/*----------------------------------------------------------------------------*/
static void epl2182_power(struct alsps_hw *hw, unsigned int on)
{
    static unsigned int power_on = 0;

    //APS_LOG("power %s\n", on ? "on" : "off");

    if(hw->power_id != POWER_NONE_MACRO)
    {
        if(power_on == on)
        {
            APS_LOG("ignore power control: %d\n", on);
        }
        else if(on)
        {
            if(!hwPowerOn(hw->power_id, hw->power_vol, "EPL2182"))
            {
                APS_ERR("power on fails!!\n");
            }
        }
        else
        {
            if(!hwPowerDown(hw->power_id, "EPL2182"))
            {
                APS_ERR("power off fail!!\n");
            }
        }
    }
    power_on = on;
}



/*----------------------------------------------------------------------------*/
static int epl2182_check_intr(struct i2c_client *client)
{
    struct epl2182_priv *obj = i2c_get_clientdata(client);
    int mode;

    APS_LOG("int pin = %d\n", mt_get_gpio_in(GPIO_ALS_EINT_PIN));

    //if (mt_get_gpio_in(GPIO_ALS_EINT_PIN) == 1) /*skip if no interrupt*/
    //   return 0;

    elan_epl2182_I2C_Write(obj->client,REG_13,R_SINGLE_BYTE,0x01,0);
    elan_epl2182_I2C_Read(obj->client);
    mode = gRawData.raw_bytes[0]&(3<<4);
    APS_LOG("mode %d\n", mode);

    if(mode==0x10)// PS
    {
        set_bit(CMC_BIT_PS, &obj->pending_intr);
    }
    else
    {
        clear_bit(CMC_BIT_PS, &obj->pending_intr);
    }


    if(atomic_read(&obj->trace) & CMC_TRC_DEBUG)
    {
        APS_LOG("check intr: 0x%08X\n", obj->pending_intr);
    }

    return 0;

}



/*----------------------------------------------------------------------------*/

int epl2182_read_als(struct i2c_client *client, u16 *data)
{
    struct epl2182_priv *obj = i2c_get_clientdata(client);
    uint8_t setting;
    u16 ch1;

    if(client == NULL)
    {
        APS_DBG("CLIENT CANN'T EQUL NULL\n");
        return -1;
    }

    elan_epl2182_I2C_Write(client,REG_13,R_SINGLE_BYTE,0x01,0);
    elan_epl2182_I2C_Read(client);
    setting = gRawData.raw_bytes[0];
    if((setting&(3<<4))!=0x00)
    {
        APS_ERR("read als data in wrong mode\n");
    }

    elan_epl2182_I2C_Write(obj->client,REG_16,R_TWO_BYTE,0x01,0x00);
    elan_epl2182_I2C_Read(obj->client);
    ch1 = (gRawData.raw_bytes[1]<<8) | gRawData.raw_bytes[0];

    // FIX: mid gain and low gain cannot report ff in auton gain
    if(setting>>7 ==0&& gRawData.als_ch1_raw==65535)
    {
        printk("mycat setting %d, gain %x, als %d \r\n", setting, setting>>7,  gRawData.als_ch1_raw);
        printk("mycat skip FF in auto gain \r\n");
    }
    else
    {
        gRawData.als_ch1_raw=ch1;
        *data =  gRawData.als_ch1_raw;
        printk("mycat read als raw data = %d \r\n", gRawData.als_ch1_raw);
    }

    return 0;
}


/*----------------------------------------------------------------------------*/
long epl2182_read_ps(struct i2c_client *client, u16 *data)
{
    struct epl2182_priv *obj = i2c_get_clientdata(client);
    uint8_t setting;

    if(client == NULL)
    {
        APS_DBG("CLIENT CANN'T EQUL NULL\n");
        return -1;
    }

    elan_epl2182_I2C_Write(obj->client,REG_13,R_SINGLE_BYTE,0x01,0);
    elan_epl2182_I2C_Read(obj->client);
    setting = gRawData.raw_bytes[0];
    if((setting&(3<<4))!=0x10)
    {
        APS_ERR("read ps data in wrong mode\n");
    }
    gRawData.ps_state= !((gRawData.raw_bytes[0]&0x04)>>2);


    elan_epl2182_I2C_Write(obj->client,REG_16,R_TWO_BYTE,0x01,0x00);
    elan_epl2182_I2C_Read(obj->client);
    gRawData.ps_raw = (gRawData.raw_bytes[1]<<8) | gRawData.raw_bytes[0];

    *data = gRawData.ps_raw ;
    APS_LOG("read ps raw data = %d\n", gRawData.ps_raw);
    APS_LOG("read ps binary data = %d\n", gRawData.ps_state);
		printk("mycat+++ read ps raw data = %d  read ps binary data = %d \r\n", gRawData.ps_raw,gRawData.ps_state);
		
    return 0;
}

void epl2182_restart_polling(void)
{
    struct epl2182_priv *obj = epl2182_obj;
    cancel_delayed_work(&obj->polling_work);
    schedule_delayed_work(&obj->polling_work, msecs_to_jiffies(50));
}


void epl2182_polling_work(struct work_struct *work)
{
    struct epl2182_priv *obj = epl2182_obj;
    struct i2c_client *client = obj->client;

    bool enable_ps = test_bit(CMC_BIT_PS, &obj->enable) && atomic_read(&obj->ps_suspend)==0;
    bool enable_als = test_bit(CMC_BIT_ALS, &obj->enable) && atomic_read(&obj->als_suspend)==0;

    APS_LOG("als / ps enable: %d / %d \n", enable_als, enable_ps);

    cancel_delayed_work(&obj->polling_work);

    if((enable_ps&& obj->hw->polling_mode_ps == 1) || enable_als==true )
    {
        schedule_delayed_work(&obj->polling_work, msecs_to_jiffies(ALS_DELAY+2*PS_DELAY+30));
    }


    if(enable_als)
    {
        elan_epl2182_lsensor_enable(obj, 1);
        epl2182_read_als(client, &gRawData.als_ch1_raw);
    }

    if(enable_ps)
    {
        elan_epl2182_psensor_enable(obj, 1);
        if(obj->hw->polling_mode_ps == 1)
        {
            epl2182_read_ps(client, &gRawData.ps_raw);
        }
    }


    if(enable_als==false && enable_ps==false)
    {
        APS_LOG("disable sensor\n");
        cancel_delayed_work(&obj->polling_work);
        elan_epl2182_I2C_Write(client,REG_9,W_SINGLE_BYTE,0x02,EPL_INT_DISABLE);
        elan_epl2182_I2C_Write(client,REG_0,W_SINGLE_BYTE,0X02,EPL_S_SENSING_MODE);
    }

}


/*----------------------------------------------------------------------------*/
void epl2182_eint_func(void)
{
    struct epl2182_priv *obj = g_epl2182_ptr;

    // APS_LOG(" interrupt fuc\n");

    if(!obj)
    {
        return;
    }

    mt65xx_eint_mask(CUST_EINT_ALS_NUM);
    schedule_delayed_work(&obj->eint_work, 0);
}



/*----------------------------------------------------------------------------*/
static void epl2182_eint_work(struct work_struct *work)
{
    struct epl2182_priv *epld = g_epl2182_ptr;
    int err;
    hwm_sensor_data sensor_data;

    APS_LOG("xxxxx eint work\n");
    if((err = epl2182_check_intr(epld->client)))
    {
        APS_ERR("check intrs: %d\n", err);
    }

    if(epld->pending_intr)
    {
        elan_epl2182_I2C_Write(epld->client,REG_13,R_SINGLE_BYTE,0x01,0);
        elan_epl2182_I2C_Read(epld->client);
        gRawData.ps_state= !((gRawData.raw_bytes[0]&0x04)>>2);
        APS_LOG("ps state = %d\n", gRawData.ps_state);

        elan_epl2182_I2C_Write(epld->client,REG_16,R_TWO_BYTE,0x01,0x00);
        elan_epl2182_I2C_Read(epld->client);
        gRawData.ps_raw = (gRawData.raw_bytes[1]<<8) | gRawData.raw_bytes[0];
        APS_LOG("ps raw_data = %d\n", gRawData.ps_raw);
        printk("mycat*** read ps raw data = %d  read ps binary data = %d \r\n", gRawData.ps_raw,gRawData.ps_state);

        sensor_data.values[0] = gRawData.ps_state;
        sensor_data.value_divide = 1;
        sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;

        //let up layer to know
        if((err = hwmsen_get_interrupt_data(ID_PROXIMITY, &sensor_data)))
        {
            APS_ERR("get interrupt data failed\n");
            APS_ERR("call hwmsen_get_interrupt_data fail = %d\n", err);
        }
    }

    elan_epl2182_I2C_Write(epld->client,REG_9,W_SINGLE_BYTE,0x02,EPL_INT_ACTIVE_LOW | PS_DRIVE);
    elan_epl2182_I2C_Write(epld->client,REG_7,W_SINGLE_BYTE,0x02,EPL_DATA_UNLOCK);

exit:
#ifdef MT6516
    MT6516_EINTIRQUnmask(CUST_EINT_ALS_NUM);
#endif

#ifdef MT6573
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif

#ifdef MT6513
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif

#ifdef MT6575
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif

#ifdef MT6577
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif

#ifdef MT6589
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif

#ifdef MT6572
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif
}



/*----------------------------------------------------------------------------*/
int epl2182_setup_eint(struct i2c_client *client)
{
    struct epl2182_priv *obj = i2c_get_clientdata(client);

    APS_LOG("epl2182_setup_eint\n");


    g_epl2182_ptr = obj;

    /*configure to GPIO function, external interrupt*/

    mt_set_gpio_mode(GPIO_ALS_EINT_PIN, GPIO_ALS_EINT_PIN_M_EINT);
    mt_set_gpio_dir(GPIO_ALS_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_ALS_EINT_PIN, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_select(GPIO_ALS_EINT_PIN, GPIO_PULL_UP);

#ifdef MT6516
    MT6516_EINT_Set_Sensitivity(CUST_EINT_ALS_NUM, CUST_EINT_EDGE_SENSITIVE);
    MT6516_EINT_Set_Polarity(CUST_EINT_ALS_NUM, CUST_EINT_ALS_POLARITY);
    MT6516_EINT_Set_HW_Debounce(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_CN);
    MT6516_EINT_Registration(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_EN, CUST_EINT_POLARITY_LOW, epl2182_eint_func, 0);
    MT6516_EINTIRQUnmask(CUST_EINT_ALS_NUM);
#endif

#ifdef MT6513
    mt65xx_eint_set_sens(CUST_EINT_ALS_NUM, CUST_EINT_EDGE_SENSITIVE);
    mt65xx_eint_set_polarity(CUST_EINT_ALS_NUM, CUST_EINT_ALS_POLARITY);
    mt65xx_eint_set_hw_debounce(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_CN);
    mt65xx_eint_registration(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_EN, CUST_EINT_POLARITY_LOW, epl2182_eint_func, 0);
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif

#ifdef MT6573
    mt65xx_eint_set_sens(CUST_EINT_ALS_NUM, CUST_EINT_EDGE_SENSITIVE);
    mt65xx_eint_set_polarity(CUST_EINT_ALS_NUM, CUST_EINT_ALS_POLARITY);
    mt65xx_eint_set_hw_debounce(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_CN);
    mt65xx_eint_registration(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_EN, CUST_EINT_POLARITY_LOW, epl2182_eint_func, 0);
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif

#ifdef  MT6575
    mt65xx_eint_set_sens(CUST_EINT_ALS_NUM, CUST_EINT_EDGE_SENSITIVE);
    mt65xx_eint_set_polarity(CUST_EINT_ALS_NUM, CUST_EINT_ALS_POLARITY);
    mt65xx_eint_set_hw_debounce(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_CN);
    mt65xx_eint_registration(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_EN, CUST_EINT_POLARITY_LOW, epl2182_eint_func, 0);
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif

#ifdef  MT6577
    mt65xx_eint_set_sens(CUST_EINT_ALS_NUM, CUST_EINT_EDGE_SENSITIVE);
    mt65xx_eint_set_polarity(CUST_EINT_ALS_NUM, CUST_EINT_ALS_POLARITY);
    mt65xx_eint_set_hw_debounce(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_CN);
    mt65xx_eint_registration(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_EN, CUST_EINT_POLARITY_LOW, epl2182_eint_func, 0);
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif

#ifdef  MT6589
    mt65xx_eint_set_sens(CUST_EINT_ALS_NUM, CUST_EINT_EDGE_SENSITIVE);
    mt65xx_eint_set_polarity(CUST_EINT_ALS_NUM, CUST_EINT_ALS_POLARITY);
    mt65xx_eint_set_hw_debounce(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_CN);
    mt65xx_eint_registration(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_EN, CUST_EINT_POLARITY_LOW, epl2182_eint_func, 0);
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif

#ifdef  MT6572
    mt65xx_eint_set_sens(CUST_EINT_ALS_NUM, CUST_EINT_EDGE_SENSITIVE);
    mt65xx_eint_set_polarity(CUST_EINT_ALS_NUM, CUST_EINT_ALS_POLARITY);
    mt65xx_eint_set_hw_debounce(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_CN);
    mt65xx_eint_registration(CUST_EINT_ALS_NUM, CUST_EINT_ALS_DEBOUNCE_EN, CUST_EINT_POLARITY_LOW, epl2182_eint_func, 0);
    mt65xx_eint_unmask(CUST_EINT_ALS_NUM);
#endif

    return 0;
}




/*----------------------------------------------------------------------------*/
static int epl2182_init_client(struct i2c_client *client)
{
    struct epl2182_priv *obj = i2c_get_clientdata(client);
    int err=0;

    APS_LOG("[Agold spl] I2C Addr==[0x%x],line=%d\n",epl2182_i2c_client->addr,__LINE__);

    /*  interrupt mode */


    APS_FUN();

    if(obj->hw->polling_mode_ps == 0)
    {
        mt65xx_eint_mask(CUST_EINT_ALS_NUM);

        if((err = epl2182_setup_eint(client)))
        {
            APS_ERR("setup eint: %d\n", err);
            return err;
        }
        APS_LOG("epl2182 interrupt setup\n");
    }


    if((err = hw8k_init_device(client)) != 0)
    {
        APS_ERR("init dev: %d\n", err);
        return err;
    }


    if((err = epl2182_check_intr(client)))
    {
        APS_ERR("check/clear intr: %d\n", err);
        return err;
    }


    /*  interrupt mode */
//if(obj->hw->polling_mode_ps == 0)
    //     mt65xx_eint_unmask(CUST_EINT_ALS_NUM);

    return err;
}

static int elan_epl2182_psensor_enable_cali(struct epl2182_priv *epl_data, int enable)
{
    int ret = 0;
    int ps_state;
    uint8_t regdata;
    struct i2c_client *client = epl_data->client;

    APS_LOG("[ELAN epl2182] %s enable = %d\n", __func__, enable);

    ret = elan_epl2182_I2C_Write(client,REG_9,W_SINGLE_BYTE,0x02,EPL_INT_DISABLE | PS_DRIVE);

    if(enable)
    {
        regdata = EPL_SENSING_2_TIME | EPL_PS_MODE | EPL_L_GAIN ;
        regdata = regdata | (epl_data->hw->polling_mode_ps == 0 ? EPL_C_SENSING_MODE : EPL_S_SENSING_MODE);
        ret = elan_epl2182_I2C_Write(client,REG_0,W_SINGLE_BYTE,0X02,regdata);

        regdata = PS_INTT<<4 | EPL_PST_1_TIME | EPL_10BIT_ADC;
        ret = elan_epl2182_I2C_Write(client,REG_1,W_SINGLE_BYTE,0X02,regdata);

        set_psensor_intr_threshold(epl_data->hw ->ps_threshold_low,epl_data->hw ->ps_threshold_high);

        ret = elan_epl2182_I2C_Write(client,REG_7,W_SINGLE_BYTE,0X02,EPL_C_RESET);
        ret = elan_epl2182_I2C_Write(client,REG_7,W_SINGLE_BYTE,0x02,EPL_C_START_RUN);

        msleep(PS_DELAY);

        if(epl_data->hw->polling_mode_ps == 0)
        {
        
            elan_epl2182_I2C_Write(client,REG_13,R_SINGLE_BYTE,0x01,0);
            elan_epl2182_I2C_Read(client);
            ps_state= !((gRawData.raw_bytes[0]&0x04)>>2);

            if(gRawData.ps_state != ps_state)
            {
                elan_epl2182_I2C_Write(client,REG_9,W_SINGLE_BYTE,0x02,EPL_INT_FRAME_ENABLE|PS_DRIVE);
            }
            else
            {
                elan_epl2182_I2C_Write(client,REG_9,W_SINGLE_BYTE,0x02,EPL_INT_ACTIVE_LOW |PS_DRIVE);
            }
        }

    }
    else
    {
        regdata = EPL_SENSING_2_TIME | EPL_PS_MODE | EPL_L_GAIN | EPL_S_SENSING_MODE;
        ret = elan_epl2182_I2C_Write(client,REG_0,W_SINGLE_BYTE,0X02,regdata);
    }

    if(ret<0)
    {
        APS_ERR("[ELAN epl2182 error]%s: ps enable %d fail\n",__func__,ret);
    }
    else
    {
        ret = 0;
    }

    return ret;
}

static int epl2182_calibrate(struct i2c_client *client)
{
	struct epl2182_priv *obj = i2c_get_clientdata(client); 
	struct file *fp;
	int ret = 0;
	int w,count_temp;
	int readback_lo=0, readback_hi=0, readback=0;
	UINT16 distance_temp_low,distance_temp_high;
	char buf[4];

	distance_ps_threshold_flag = 0;
	printk("hgw111111:epl2182_calibrate open nvram file!\r\n");
	elan_epl2182_psensor_enable_cali(obj, 1);
	//msleep(100);
	epl2182_read_ps(obj->client, &gRawData.ps_raw);
	elan_epl2182_psensor_enable_cali(obj, 0);
	
	printk("epl2182_calibrate value is gRawData.ps_raw = %d \r\n",gRawData.ps_raw);
	
#if defined(BIRD_PS_CALI_ADD_VAL)
	distance_temp_high = (gRawData.ps_raw) + simple_strtol(BIRD_PS_CALI_ADD_VAL, NULL, 0);
#else
	distance_temp_high = (gRawData.ps_raw) + 16;
#endif

#if defined(BIRD_PS_CALI_ADD_VAL2)
	distance_temp_low = (gRawData.ps_raw) + simple_strtol(BIRD_PS_CALI_ADD_VAL2, NULL, 0);
#else
	distance_temp_low = (gRawData.ps_raw) + 8;
#endif

	//if(distance_temp<3)
	//	distance_temp=atomic_read(&obj->ps_thd_val);

	//write to nvram
	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);

	buf[0] = (distance_temp_high>>8)&0xff;
	buf[1] = (distance_temp_high)&0xff;
	buf[2] = (distance_temp_low>>8)&0xff;
	buf[3] = (distance_temp_low)&0xff;
	fp = filp_open(FTM_CUST_ALSPS,O_WRONLY|O_CREAT, 0644);
	fp->f_pos = 0;
	count_temp = fp->f_op->write(fp,
								  &buf,
								  4,
								  &fp->f_pos);	
	filp_close(fp, NULL);	
	printk("epl2182_calibrate value is count_temp = %x,buf[0] = %x buf[1] = %x buf[2] = %x buf[3] = %x distance_temp_high=%x distance_temp_low=%x\r\n",count_temp,buf[0],buf[1],buf[2],buf[3],distance_temp_high,distance_temp_low);

	 set_fs(old_fs);
	//atomic_set(&obj->ps_thd_val,  distance_temp);
	//obj->ps_thd_val = distance_temp;
	return distance_temp_high;

}

/*----------------------------------------------------------------------------*/
static ssize_t epl2182_show_reg(struct device_driver *ddri, char *buf)
{
    if(!epl2182_obj)
    {
        APS_ERR("epl2182_obj is null!!\n");
        return 0;
    }
    ssize_t len = 0;
    struct i2c_client *client = epl2182_obj->client;

    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x00 value = %8x\n", i2c_smbus_read_byte_data(client, 0x00));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x01 value = %8x\n", i2c_smbus_read_byte_data(client, 0x08));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x02 value = %8x\n", i2c_smbus_read_byte_data(client, 0x10));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x03 value = %8x\n", i2c_smbus_read_byte_data(client, 0x18));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x04 value = %8x\n", i2c_smbus_read_byte_data(client, 0x20));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x05 value = %8x\n", i2c_smbus_read_byte_data(client, 0x28));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x06 value = %8x\n", i2c_smbus_read_byte_data(client, 0x30));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x07 value = %8x\n", i2c_smbus_read_byte_data(client, 0x38));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x09 value = %8x\n", i2c_smbus_read_byte_data(client, 0x48));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x0D value = %8x\n", i2c_smbus_read_byte_data(client, 0x68));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x0E value = %8x\n", i2c_smbus_read_byte_data(client, 0x70));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x0F value = %8x\n", i2c_smbus_read_byte_data(client, 0x71));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x10 value = %8x\n", i2c_smbus_read_byte_data(client, 0x80));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x11 value = %8x\n", i2c_smbus_read_byte_data(client, 0x88));
    len += snprintf(buf+len, PAGE_SIZE-len, "chip id REG 0x13 value = %8x\n", i2c_smbus_read_byte_data(client, 0x98));

    return len;

}

/*----------------------------------------------------------------------------*/
static ssize_t epl2182_show_status(struct device_driver *ddri, char *buf)
{
    ssize_t len = 0;
    struct epl2182_priv *epld = epl2182_obj;
    u16 ch0, ch1;

    if(!epl2182_obj)
    {
        APS_ERR("epl2182_obj is null!!\n");
        return 0;
    }
    elan_epl2182_I2C_Write(epld->client,REG_7,W_SINGLE_BYTE,0x02,EPL_DATA_LOCK);

    elan_epl2182_I2C_Write(epld->client,REG_14,R_TWO_BYTE,0x01,0x00);
    elan_epl2182_I2C_Read(epld->client);
    ch0 = (gRawData.raw_bytes[1]<<8) | gRawData.raw_bytes[0];
    elan_epl2182_I2C_Write(epld->client,REG_16,R_TWO_BYTE,0x01,0x00);
    elan_epl2182_I2C_Read(epld->client);
    ch1 = (gRawData.raw_bytes[1]<<8) | gRawData.raw_bytes[0];

    elan_epl2182_I2C_Write(epld->client,REG_7,W_SINGLE_BYTE,0x02,EPL_DATA_UNLOCK);

    len += snprintf(buf+len, PAGE_SIZE-len, "als/ps int time is %d-%d\n",ALS_INTT, PS_INTT);
    len += snprintf(buf+len, PAGE_SIZE-len, "ch0 ch1 raw is %d-%d\n",ch0, ch1);
    len += snprintf(buf+len, PAGE_SIZE-len, "threshold is %d/%d\n",epld->hw->ps_threshold_low, epld->hw->ps_threshold_high);


    return len;
}

/*----------------------------------------------------------------------------*/
static ssize_t epl2182_store_als_int_time(struct device_driver *ddri, const char *buf, size_t count)
{
    if(!epl2182_obj)
    {
        APS_ERR("epl2182_obj is null!!\n");
        return 0;
    }

    sscanf(buf, "%d", &ALS_INTT);
    APS_LOG("als int time is %d\n", ALS_INTT);
    return count;
}


/*----------------------------------------------------------------------------*/
static ssize_t epl2182_show_ps_cal_raw(struct device_driver *ddri, char *buf)
{
    u16 *tmp = (u16*)buf;
    struct epl2182_priv *obj = epl2182_obj;
    u16 ch1;
    int ch1_all=0;
    int count =5;
    int i;

    if(!epl2182_obj)
    {
        APS_ERR("epl2182_obj is null!!\n");
        return 0;
    }

    atomic_set(&obj->als_suspend, 1);


    for(i=0; i<count; i++)
    {
        msleep(PS_DELAY);

        if(obj->hw->polling_mode_ps)
        {
            ch1_all = ch1_all+ gRawData.ps_raw;
        }
        else
        {
            elan_epl2182_I2C_Write(obj->client,REG_7,W_SINGLE_BYTE,0x02,EPL_DATA_LOCK);
            elan_epl2182_I2C_Write(obj->client,REG_16,R_TWO_BYTE,0x01,0x00);
            elan_epl2182_I2C_Read(obj->client);
            ch1_all = ch1_all+ ((gRawData.raw_bytes[1]<<8) | gRawData.raw_bytes[0]);
            elan_epl2182_I2C_Write(obj->client,REG_7,W_SINGLE_BYTE,0x02,EPL_DATA_UNLOCK);
        }
    }

    ch1 = ch1_all/count;
    tmp[0]=ch1;


    atomic_set(&obj->als_suspend,0);
    return 2;
}


/*----------------------------------------------------------------------------*/
static ssize_t epl2182_store_ps_int_time(struct device_driver *ddri, const char *buf, size_t count)
{
    if(!epl2182_obj)
    {
        APS_ERR("epl2182_obj is null!!\n");
        return 0;
    }
    sscanf(buf, "%d", &PS_INTT);
    APS_LOG("ps int time is %d\n", PS_INTT);
    return count;
}

/*----------------------------------------------------------------------------*/
static ssize_t epl2182_store_ps_threshold(struct device_driver *ddri, const char *buf, size_t count)
{
    if(!epl2182_obj)
    {
        APS_ERR("epl2182_obj is null!!\n");
        return 0;
    }
    sscanf(buf, "%d%d", &epl2182_obj->hw ->ps_threshold_low, &epl2182_obj->hw ->ps_threshold_high);
    return count;
}

/*----------------------------------------------------------------------------*/
static DRIVER_ATTR(elan_status, 			S_IWUSR | S_IRUGO, epl2182_show_status,  NULL);
static DRIVER_ATTR(elan_reg,     		S_IWUSR | S_IRUGO, epl2182_show_reg,   	 NULL);
static DRIVER_ATTR(als_int_time,     	S_IROTH  | S_IWOTH, NULL,   				 epl2182_store_als_int_time);
static DRIVER_ATTR(ps_cal_raw, 			S_IROTH  | S_IWOTH, epl2182_show_ps_cal_raw, 	  		NULL);
static DRIVER_ATTR(ps_int_time,    	S_IROTH  | S_IWOTH, NULL,   				 epl2182_store_ps_int_time);
static DRIVER_ATTR(ps_threshold,     			S_IROTH  | S_IWOTH, NULL,   					 		epl2182_store_ps_threshold			);

/*----------------------------------------------------------------------------*/
static struct device_attribute * epl2182_attr_list[] =
{
    &driver_attr_elan_status,
    &driver_attr_elan_reg,
    &driver_attr_als_int_time,
    &driver_attr_ps_cal_raw,
    &driver_attr_ps_int_time,
    &driver_attr_ps_threshold,
};

/*----------------------------------------------------------------------------*/
static int epl2182_create_attr(struct device_driver *driver)
{
    int idx, err = 0;
    int num = (int)(sizeof(epl2182_attr_list)/sizeof(epl2182_attr_list[0]));
    if (driver == NULL)
    {
        return -EINVAL;
    }

    for(idx = 0; idx < num; idx++)
    {
        if(err = driver_create_file(driver, epl2182_attr_list[idx]))
        {
            APS_ERR("driver_create_file (%s) = %d\n", epl2182_attr_list[idx]->attr.name, err);
            break;
        }
    }
    return err;
}



/*----------------------------------------------------------------------------*/
static int epl2182_delete_attr(struct device_driver *driver)
{
    int idx ,err = 0;
    int num = (int)(sizeof(epl2182_attr_list)/sizeof(epl2182_attr_list[0]));

    if (!driver)
        return -EINVAL;

    for (idx = 0; idx < num; idx++)
    {
        driver_remove_file(driver, epl2182_attr_list[idx]);
    }

    return err;
}



/******************************************************************************
 * Function Configuration
******************************************************************************/
static int epl2182_open(struct inode *inode, struct file *file)
{
    file->private_data = epl2182_i2c_client;

    APS_FUN();

    if (!file->private_data)
    {
        APS_ERR("null pointer!!\n");
        return -EINVAL;
    }

    return nonseekable_open(inode, file);
}

/*----------------------------------------------------------------------------*/
static int epl2182_release(struct inode *inode, struct file *file)
{
    APS_FUN();
    file->private_data = NULL;
    return 0;
}

/*----------------------------------------------------------------------------*/
static long epl2182_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct i2c_client *client = (struct i2c_client*)file->private_data;
    struct epl2182_priv *obj = i2c_get_clientdata(client);
    int err = 0;
    void __user *ptr = (void __user*) arg;
    int dat;
    uint32_t enable;


    switch (cmd)
    {
        case ALSPS_SET_PS_MODE:
            if(copy_from_user(&enable, ptr, sizeof(enable)))
            {
                err = -EFAULT;
                goto err_out;
            }

            if(enable)
            {
                if(obj->hw->polling_mode_ps == 0)
                {
                    if((err = elan_epl2182_psensor_enable(obj, 1))!=0)
                    {
                        APS_ERR("enable ps fail: %d\n", err);
                        return -1;
                    }
                }
                set_bit(CMC_BIT_PS, &obj->enable);
            }
            else
            {
                if(obj->hw->polling_mode_ps == 0)
                {
                    if((err = elan_epl2182_psensor_enable(obj, 0))!=0)
                    {
                        APS_ERR("disable ps fail: %d\n", err);
                        return -1;
                    }
                }
                clear_bit(CMC_BIT_PS, &obj->enable);
            }
            break;


        case ALSPS_GET_PS_MODE:
            enable=test_bit(CMC_BIT_PS, &obj->enable);
            if(copy_to_user(ptr, &enable, sizeof(enable)))
            {
                err = -EFAULT;
                goto err_out;
            }
            break;


        case ALSPS_GET_PS_DATA:
            if((err = elan_epl2182_psensor_enable(obj, 1))!=0)
            {
                APS_ERR("enable ps fail: %d\n", err);
                return -1;
            }
            epl2182_read_ps(obj->client, &gRawData.ps_raw);
            dat = gRawData.ps_state;

            APS_LOG("ioctl ps state value = %d \n", dat);

            if(copy_to_user(ptr, &dat, sizeof(dat)))
            {
                err = -EFAULT;
                goto err_out;
            }
            break;


        case ALSPS_GET_PS_RAW_DATA:
            if((err = elan_epl2182_psensor_enable(obj, 1))!=0)
            {
                APS_ERR("enable ps fail: %d\n", err);
                return -1;
            }
            epl2182_read_ps(obj->client, &gRawData.ps_raw);
            dat = gRawData.ps_raw;

            APS_LOG("ioctl ps raw value = %d \n", dat);
            if(copy_to_user(ptr, &dat, sizeof(dat)))
            {
                err = -EFAULT;
                goto err_out;
            }
            break;


        case ALSPS_SET_ALS_MODE:
            if(copy_from_user(&enable, ptr, sizeof(enable)))
            {
                err = -EFAULT;
                goto err_out;
            }
            if(enable)
                set_bit(CMC_BIT_ALS, &obj->enable);
            else
                clear_bit(CMC_BIT_ALS, &obj->enable);

            break;



        case ALSPS_GET_ALS_MODE:
            enable=test_bit(CMC_BIT_ALS, &obj->enable);
            if(copy_to_user(ptr, &enable, sizeof(enable)))
            {
                err = -EFAULT;
                goto err_out;
            }
            break;



        case ALSPS_GET_ALS_DATA:
            if((err = elan_epl2182_lsensor_enable(obj, 1))!=0)
            {
                APS_ERR("disable als fail: %d\n", err);
                return -1;
            }

            epl2182_read_als(obj->client, &gRawData.als_ch1_raw);
            dat = epl2182_get_als_value(obj, gRawData.als_ch1_raw);
            APS_LOG("ioctl get als data = %d\n", dat);


            if(test_bit(CMC_BIT_PS, &obj->enable) && obj->hw->polling_mode_ps == 0)
            {
                if((err = elan_epl2182_psensor_enable(obj, 1))!=0)
                {
                    APS_ERR("disable ps fail: %d\n", err);
                    return -1;
                }
            }

            if(copy_to_user(ptr, &dat, sizeof(dat)))
            {
                err = -EFAULT;
                goto err_out;
            }
            break;


        case ALSPS_GET_ALS_RAW_DATA:
            if((err = elan_epl2182_lsensor_enable(obj, 1))!=0)
            {
                APS_ERR("disable als fail: %d\n", err);
                return -1;
            }

            epl2182_read_als(obj->client, &gRawData.als_ch1_raw);
            dat = gRawData.als_ch1_raw;
            APS_LOG("ioctl get als raw data = %d\n", dat);


            if(test_bit(CMC_BIT_PS, &obj->enable)&& obj->hw->polling_mode_ps == 0)
            {
                if((err = elan_epl2182_psensor_enable(obj, 1))!=0)
                {
                    APS_ERR("disable ps fail: %d\n", err);
                    return -1;
                }
            }
            if(copy_to_user(ptr, &dat, sizeof(dat)))
            {
                err = -EFAULT;
                goto err_out;
            }
            break;
            
				case ALSPS_SET_PS_CALIBRATION:    
						dat = epl2182_calibrate(obj->client);
						break;

        default:
            APS_ERR("%s not supported = 0x%04x", __FUNCTION__, cmd);
            err = -ENOIOCTLCMD;
            break;
    }

err_out:
    return err;
}


/*----------------------------------------------------------------------------*/
static struct file_operations epl2182_fops =
{
    .owner = THIS_MODULE,
    .open = epl2182_open,
    .release = epl2182_release,
    .unlocked_ioctl = epl2182_unlocked_ioctl,
};


/*----------------------------------------------------------------------------*/
static struct miscdevice epl2182_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "als_ps",
    .fops = &epl2182_fops,
};


/*----------------------------------------------------------------------------*/
static int epl2182_i2c_suspend(struct i2c_client *client, pm_message_t msg)
{
    struct epl2182_priv *obj = i2c_get_clientdata(client);
    int err;
    APS_FUN();

    if(msg.event == PM_EVENT_SUSPEND)
    {
        if(!obj)
        {
            APS_ERR("null pointer!!\n");
            return -EINVAL;
        }

        atomic_set(&obj->als_suspend, 1);
        atomic_set(&obj->ps_suspend, 1);

        if(test_bit(CMC_BIT_PS,  &obj->enable) && obj->hw->polling_mode_ps==0)
            epl2182_restart_polling();

        epl2182_power(obj->hw, 0);
    }

    return 0;

}



/*----------------------------------------------------------------------------*/
static int epl2182_i2c_resume(struct i2c_client *client)
{
    struct epl2182_priv *obj = i2c_get_clientdata(client);
    int err;
    APS_FUN();

    if(!obj)
    {
        APS_ERR("null pointer!!\n");
        return -EINVAL;
    }

    epl2182_power(obj->hw, 1);

    msleep(50);

    atomic_set(&obj->ps_suspend, 0);

    if(err = epl2182_init_client(client))
    {
        APS_ERR("initialize client fail!!\n");
        return err;
    }

    if(obj->hw->polling_mode_ps == 0)
        epl2182_setup_eint(client);


    if(test_bit(CMC_BIT_PS,  &obj->enable))
        epl2182_restart_polling();

    return 0;
}



/*----------------------------------------------------------------------------*/
static void epl2182_early_suspend(struct early_suspend *h)
{
    /*early_suspend is only applied for ALS*/
    struct epl2182_priv *obj = container_of(h, struct epl2182_priv, early_drv);
    int err;
    APS_FUN();

    if(!obj)
    {
        APS_ERR("null pointer!!\n");
        return;
    }

    atomic_set(&obj->als_suspend, 1);
}



/*----------------------------------------------------------------------------*/
static void epl2182_late_resume(struct early_suspend *h)
{
    /*late_resume is only applied for ALS*/
    struct epl2182_priv *obj = container_of(h, struct epl2182_priv, early_drv);
    int err;
    APS_FUN();

    if(!obj)
    {
        APS_ERR("null pointer!!\n");
        return;
    }

    atomic_set(&obj->als_suspend, 0);
    atomic_set(&obj->ps_suspend, 0);

    if(test_bit(CMC_BIT_ALS, &obj->enable))
        epl2182_restart_polling();

}


/*----------------------------------------------------------------------------*/
int epl2182_ps_operate(void* self, uint32_t command, void* buff_in, int size_in,
                       void* buff_out, int size_out, int* actualout)
{
    int err = 0;
    int value;
    hwm_sensor_data* sensor_data;
    struct epl2182_priv *obj = (struct epl2182_priv *)self;

    APS_LOG("epl2182_ps_operate command = %x\n",command);
    switch (command)
    {
        case SENSOR_DELAY:
            if((buff_in == NULL) || (size_in < sizeof(int)))
            {
                APS_ERR("Set delay parameter error!\n");
                err = -EINVAL;
            }
            break;


        case SENSOR_ENABLE:
            if((buff_in == NULL) || (size_in < sizeof(int)))
            {
                APS_ERR("Enable sensor parameter error!\n");
                err = -EINVAL;
            }
            else
            {
                value = *(int *)buff_in;
                APS_LOG("ps enable = %d\n", value);


                if(value)
                {
                    if(obj->hw->polling_mode_ps==0)
                        gRawData.ps_state=2;
                    set_bit(CMC_BIT_PS, &obj->enable);
                }
                else
                {
                    clear_bit(CMC_BIT_PS, &obj->enable);
                }
                epl2182_restart_polling();
            }

            break;



        case SENSOR_GET_DATA:
            APS_LOG(" get ps data !!!!!!\n");
            if((buff_out == NULL) || (size_out< sizeof(hwm_sensor_data)))
            {
                APS_ERR("get sensor data parameter error!\n");
                err = -EINVAL;
            }
            else
            {
                APS_LOG("---SENSOR_GET_DATA---\n\n");

                sensor_data = (hwm_sensor_data *)buff_out;
                sensor_data->values[0] =gRawData.ps_state;
                sensor_data->values[1] =gRawData.ps_raw;
                sensor_data->value_divide = 1;
                sensor_data->status = SENSOR_STATUS_ACCURACY_MEDIUM;
            }
            break;


        default:
            APS_ERR("proxmy sensor operate function no this parameter %d!\n", command);
            err = -1;
            break;



    }

    return err;

}



int epl2182_als_operate(void* self, uint32_t command, void* buff_in, int size_in,
                        void* buff_out, int size_out, int* actualout)
{
    int err = 0;
    int value;
    hwm_sensor_data* sensor_data;
    struct epl2182_priv *obj = (struct epl2182_priv *)self;

    APS_FUN();
    APS_LOG("epl2182_als_operate command = %x\n",command);

    switch (command)
    {
        case SENSOR_DELAY:
            if((buff_in == NULL) || (size_in < sizeof(int)))
            {
                APS_ERR("Set delay parameter error!\n");
                err = -EINVAL;
            }
            break;


        case SENSOR_ENABLE:
            if((buff_in == NULL) || (size_in < sizeof(int)))
            {
                APS_ERR("Enable sensor parameter error!\n");
                err = -EINVAL;
            }
            else
            {
                value = *(int *)buff_in;
                if(value)
                {
                    set_bit(CMC_BIT_ALS, &obj->enable);
                    epl2182_restart_polling();
                }
                else
                {
                    clear_bit(CMC_BIT_ALS, &obj->enable);
                }

            }
            break;


        case SENSOR_GET_DATA:
            APS_LOG("get als data !!!!!!\n");
            if((buff_out == NULL) || (size_out< sizeof(hwm_sensor_data)))
            {
                APS_ERR("get sensor data parameter error!\n");
                err = -EINVAL;
            }
            else
            {
                sensor_data = (hwm_sensor_data *)buff_out;
                sensor_data->values[0] = epl2182_get_als_value(obj, gRawData.als_ch1_raw);
                sensor_data->value_divide = 1;
                sensor_data->status = SENSOR_STATUS_ACCURACY_MEDIUM;
                APS_LOG("get als data->values[0] = %d\n", sensor_data->values[0]);
            }
            break;

        default:
            APS_ERR("light sensor operate function no this parameter %d!\n", command);
            err = -1;
            break;



    }

    return err;

}


/*----------------------------------------------------------------------------*/

static int epl2182_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info)
{
    strcpy(info->type, EPL2182_DEV_NAME);
    return 0;
}


/*----------------------------------------------------------------------------*/
static int epl2182_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    struct epl2182_priv *obj;
    struct hwmsen_object obj_ps, obj_als;
    int err = 0;
    APS_FUN();

    epl2182_dumpReg(client);

    if(!(obj = kzalloc(sizeof(*obj), GFP_KERNEL)))
    {
        err = -ENOMEM;
        goto exit;
    }

    memset(obj, 0, sizeof(*obj));

    epl2182_obj = obj;
    obj->hw = get_cust_alsps_hw();

    epl2182_get_addr(obj->hw, &obj->addr);

    obj->als_level_num = sizeof(obj->hw->als_level)/sizeof(obj->hw->als_level[0]);
    obj->als_value_num = sizeof(obj->hw->als_value)/sizeof(obj->hw->als_value[0]);
    BUG_ON(sizeof(obj->als_level) != sizeof(obj->hw->als_level));
    memcpy(obj->als_level, obj->hw->als_level, sizeof(obj->als_level));
    BUG_ON(sizeof(obj->als_value) != sizeof(obj->hw->als_value));
    memcpy(obj->als_value, obj->hw->als_value, sizeof(obj->als_value));

    INIT_DELAYED_WORK(&obj->eint_work, epl2182_eint_work);
    INIT_DELAYED_WORK(&obj->polling_work, epl2182_polling_work);

    obj->client = client;

    i2c_set_clientdata(client, obj);

    atomic_set(&obj->trace, 0x00);
    atomic_set(&obj->als_suspend, 0);
    atomic_set(&obj->ps_suspend, 0);

    obj->lux_per_count = LUX_PER_COUNT;
    obj->enable = 0;
    obj->pending_intr = 0;


    epl2182_i2c_client = client;

    err = elan_epl2182_I2C_Write(client,REG_0,W_SINGLE_BYTE,0x02, EPL_S_SENSING_MODE);
    err = elan_epl2182_I2C_Write(client,REG_9,W_SINGLE_BYTE,0x02,EPL_INT_DISABLE);

	if(err==-EINVAL )
	 goto exit_init_failed;

    if(err = epl2182_init_client(client))
    {
        goto exit_init_failed;
    }


    if(err = misc_register(&epl2182_device))
    {
        APS_ERR("epl2182_device register failed\n");
        goto exit_misc_device_register_failed;
    }

    if(err = epl2182_create_attr(&epl2182_alsps_driver.driver))
    {
        APS_ERR("create attribute err = %d\n", err);
        goto exit_create_attr_failed;
    }

    obj_ps.self = epl2182_obj;

    if( obj->hw->polling_mode_ps)
    {
        obj_ps.polling = 1;
        APS_LOG("ps_interrupt == false\n");
    }
    else
    {
        obj_ps.polling = 0;//interrupt mode
        APS_LOG("ps_interrupt == true\n");
    }


    obj_ps.sensor_operate = epl2182_ps_operate;



    if(err = hwmsen_attach(ID_PROXIMITY, &obj_ps))
    {
        APS_ERR("attach fail = %d\n", err);
        goto exit_create_attr_failed;
    }


    obj_als.self = epl2182_obj;
    obj_als.polling = 1;
    obj_als.sensor_operate = epl2182_als_operate;
    APS_LOG("als polling mode\n");


    if(err = hwmsen_attach(ID_LIGHT, &obj_als))
    {
        APS_ERR("attach fail = %d\n", err);
        goto exit_create_attr_failed;
    }



#if defined(CONFIG_HAS_EARLYSUSPEND)
    obj->early_drv.level    = EARLY_SUSPEND_LEVEL_DISABLE_FB - 1,
                   obj->early_drv.suspend  = epl2182_early_suspend,
                                  obj->early_drv.resume   = epl2182_late_resume,
                                                 register_early_suspend(&obj->early_drv);
#endif

    if(obj->hw->polling_mode_ps == 0)
        epl2182_setup_eint(client);

    APS_LOG("%s: OK\n", __func__);
    return 0;

exit_create_attr_failed:
    misc_deregister(&epl2182_device);
exit_misc_device_register_failed:
exit_init_failed:
    //i2c_detach_client(client);
//	exit_kfree:
    kfree(obj);
exit:
    epl2182_i2c_client = NULL;
#ifdef MT6516
    MT6516_EINTIRQMask(CUST_EINT_ALS_NUM);  /*mask interrupt if fail*/
#endif

    APS_ERR("%s: err = %d\n", __func__, err);
    return err;



}



/*----------------------------------------------------------------------------*/
static int epl2182_i2c_remove(struct i2c_client *client)
{
    int err;

    if(err = epl2182_delete_attr(&epl2182_i2c_driver.driver))
    {
        APS_ERR("epl2182_delete_attr fail: %d\n", err);
    }

    if(err = misc_deregister(&epl2182_device))
    {
        APS_ERR("misc_deregister fail: %d\n", err);
    }

    epl2182_i2c_client = NULL;
    i2c_unregister_device(client);
    kfree(i2c_get_clientdata(client));

    return 0;
}



/*----------------------------------------------------------------------------*/



static int epl2182_probe(struct platform_device *pdev)
{
    struct alsps_hw *hw = get_cust_alsps_hw();

    epl2182_power(hw, 1);

    //epl2182_force[0] = hw->i2c_num;

    if(i2c_add_driver(&epl2182_i2c_driver))
    {
        APS_ERR("add driver error\n");
        return -1;
    }
    return 0;
}



/*----------------------------------------------------------------------------*/
static int epl2182_remove(struct platform_device *pdev)
{
    struct alsps_hw *hw = get_cust_alsps_hw();
    APS_FUN();
    epl2182_power(hw, 0);

    APS_ERR("EPL2182 remove \n");
    i2c_del_driver(&epl2182_i2c_driver);
    return 0;
}



/*----------------------------------------------------------------------------*/
static struct platform_driver epl2182_alsps_driver =
{
    .probe      = epl2182_probe,
    .remove     = epl2182_remove,
    .driver     = {
        .name  = "als_ps",
        //.owner = THIS_MODULE,
    }
};
/*----------------------------------------------------------------------------*/
static int __init epl2182_init(void)
{
    struct alsps_hw *hw = get_cust_alsps_hw();
    APS_FUN();
    i2c_register_board_info(hw->i2c_num, &i2c_EPL2182, 1);
    if(platform_driver_register(&epl2182_alsps_driver))
    {
        APS_ERR("failed to register driver");
        return -ENODEV;
    }

    return 0;
}
/*----------------------------------------------------------------------------*/
static void __exit epl2182_exit(void)
{
    APS_FUN();
    platform_driver_unregister(&epl2182_alsps_driver);
}
/*----------------------------------------------------------------------------*/
module_init(epl2182_init);
module_exit(epl2182_exit);
/*----------------------------------------------------------------------------*/
MODULE_AUTHOR("renato.pan@eminent-tek.com");
MODULE_DESCRIPTION("EPL2182 ALPsr driver");
MODULE_LICENSE("GPL");




