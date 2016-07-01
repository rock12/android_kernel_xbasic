/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/* BMA220 motion sensor driver
 *
 *
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

//#include <mach/mt_devs.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>

#define POWER_NONE_MACRO MT65XX_POWER_NONE



#include <cust_acc.h>
#include <linux/hwmsensor.h>
#include <linux/hwmsen_dev.h>
#include <linux/sensors_io.h>
#include "bma220.h"
#include <linux/hwmsen_helper.h>

extern struct acc_hw* bma220_get_cust_acc_hw(void);
/*----------------------------------------------------------------------------*/
#define I2C_DRIVERID_BMA220 150
/*----------------------------------------------------------------------------*/
#define DEBUG 1
/*----------------------------------------------------------------------------*/
//#define CONFIG_BMA220_LOWPASS   /*apply low pass filter on output*/       
#define SW_CALIBRATION

/*----------------------------------------------------------------------------*/
#define BMA220_AXIS_X          0
#define BMA220_AXIS_Y          1
#define BMA220_AXIS_Z          2
#define BMA220_AXES_NUM        3
#define BMA220_DATA_LEN        3
#define BMA220_DEV_NAME        "BMA220"
#define FTM_CUST_ACC "/data/bma220"
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
static const struct i2c_device_id bma220_i2c_id[] = {{"BMA220",0},{}};
static struct i2c_board_info __initdata i2c_bma220={ I2C_BOARD_INFO("BMA220", (BMA220_I2C_SLAVE_WRITE_ADDR>>1))};
/*the adapter id will be available in customization*/
//static unsigned short bma220_force[] = {0x00, BMA220_I2C_SLAVE_WRITE_ADDR, I2C_CLIENT_END, I2C_CLIENT_END};
//static const unsigned short *const bma220_forces[] = { bma220_force, NULL };
//static struct i2c_client_address_data bma220_addr_data = { .forces = bma220_forces,};

/*----------------------------------------------------------------------------*/
static int bma220_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id); 
static int bma220_i2c_remove(struct i2c_client *client);
static int bma220_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);
#if defined(MTK_AUTO_DETECT_ACCELEROMETER)
static int  bma220_local_init(void);
static int  bma220_remove(void);

static int bma220_init_flag =0; // 0<==>OK -1 <==> fail
#endif

/*----------------------------------------------------------------------------*/
typedef enum {
    ADX_TRC_FILTER  = 0x01,
    ADX_TRC_RAWDATA = 0x02,
    ADX_TRC_IOCTL   = 0x04,
    ADX_TRC_CALI	= 0X08,
    ADX_TRC_INFO	= 0X10,
} ADX_TRC;
/*----------------------------------------------------------------------------*/
struct scale_factor{
    u8  whole;
    u8  fraction;
};
/*----------------------------------------------------------------------------*/
struct data_resolution {
    struct scale_factor scalefactor;
    int                 sensitivity;
};
/*----------------------------------------------------------------------------*/
#define C_MAX_FIR_LENGTH (32)
/*----------------------------------------------------------------------------*/
struct data_filter {
    s16 raw[C_MAX_FIR_LENGTH][BMA220_AXES_NUM];
    int sum[BMA220_AXES_NUM];
    int num;
    int idx;
};

#if defined(MTK_AUTO_DETECT_ACCELEROMETER)
static struct sensor_init_info bma220_init_info = {
		.name = "BMA220",
		.init = bma220_local_init,
		.uninit = bma220_remove,
	
};
#endif

/*----------------------------------------------------------------------------*/
struct bma220_i2c_data {
    struct i2c_client *client;
    struct acc_hw *hw;
    struct hwmsen_convert   cvt;
    
    /*misc*/
    struct data_resolution *reso;
    atomic_t                trace;
    atomic_t                suspend;
    atomic_t                selftest;
	atomic_t				filter;
    s16                     cali_sw[BMA220_AXES_NUM+1];

    /*data*/
    s8                      offset[BMA220_AXES_NUM+1];  /*+1: for 4-byte alignment*/
    s16                     data[BMA220_AXES_NUM+1];

#if defined(CONFIG_BMA220_LOWPASS)
    atomic_t                firlen;
    atomic_t                fir_en;
    struct data_filter      fir;
#endif 
    /*early suspend*/
#if defined(CONFIG_HAS_EARLYSUSPEND)
    struct early_suspend    early_drv;
#endif     
};
/*----------------------------------------------------------------------------*/
static struct i2c_driver bma220_i2c_driver = {
    .driver = {
//        .owner          = THIS_MODULE,
        .name           = "BMA220",
    },
	.probe      		= bma220_i2c_probe,
	.remove    			= bma220_i2c_remove,
	.detect				= bma220_i2c_detect,
#if !defined(CONFIG_HAS_EARLYSUSPEND)    
    .suspend            = bma220_suspend,
    .resume             = bma220_resume,
#endif
	.id_table = bma220_i2c_id,
//	.address_data = &bma220_addr_data,
};

/*----------------------------------------------------------------------------*/
static struct i2c_client *bma220_i2c_client = NULL;
#if !defined(MTK_AUTO_DETECT_ACCELEROMETER)
static struct platform_driver bma220_gsensor_driver;
#endif
static struct bma220_i2c_data *obj_i2c_data = NULL;
static bool sensor_power = true;
static GSENSOR_VECTOR3D gsensor_gain;
static char selftestRes[8]= {0}; 

//add by huangrunming 20120907
static int auto_now_data[BMA220_AXES_NUM] = {0};

/*----------------------------------------------------------------------------*/
#define GSE_TAG                  "[Gsensor] "
#define GSE_FUN(f)               printk(KERN_INFO GSE_TAG"%s\n", __FUNCTION__)
#define GSE_ERR(fmt, args...)    printk(KERN_ERR GSE_TAG"%s %d : "fmt, __FUNCTION__, __LINE__, ##args)
#define GSE_LOG(fmt, args...)    printk(KERN_INFO GSE_TAG fmt, ##args)
/*----------------------------------------------------------------------------*/
static struct data_resolution bma220_data_resolution[1] = {
 /* combination by {FULL_RES,RANGE}*/
    {{ 62, 5}, 16},   // dataformat +/-2g  in 10-bit resolution;  { 3, 9} = 3.9 = (2*2*1000)/(2^10);  256 = (2^10)/(2*2)          
};
/*----------------------------------------------------------------------------*/
static struct data_resolution bma220_offset_resolution = {{15, 6}, 64};

/*--------------------BMA220 power control function----------------------------------*/
static void BMA220_power(struct acc_hw *hw, unsigned int on) 
{
	static unsigned int power_on = 0;

	if(hw->power_id != POWER_NONE_MACRO)		// have externel LDO
	{        
		GSE_LOG("power %s\n", on ? "on" : "off");
		if(power_on == on)	// power status not change
		{
			GSE_LOG("ignore power control: %d\n", on);
		}
		else if(on)	// power on
		{
			if(!hwPowerOn(hw->power_id, hw->power_vol, "BMA220"))
			{
				GSE_ERR("power on fails!!\n");
			}
		}
		else	// power off
		{
			if (!hwPowerDown(hw->power_id, "BMA220"))
			{
				GSE_ERR("power off fail!!\n");
			}			  
		}
	}
	power_on = on;    
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
static int BMA220_SetDataResolution(struct bma220_i2c_data *obj)
{
	int err;
	u8  dat, reso;

/*set g sensor dataresolution here*/

/*BMA220 only can set to 10-bit dataresolution, so do nothing in bma220 driver here*/

/*end of set dataresolution*/


 
 /*we set measure range from -2g to +2g in BMA220_SetDataFormat(client, BMA220_RANGE_2G), 
                                                    and set 10-bit dataresolution BMA220_SetDataResolution()*/
                                                    
 /*so bma220_data_resolution[0] set value as {{ 3, 9}, 256} when declaration, and assign the value to obj->reso here*/  

 	obj->reso = &bma220_data_resolution[0];
	return 0;
	
/*if you changed the measure range, for example call: BMA220_SetDataFormat(client, BMA220_RANGE_4G), 
you must set the right value to bma220_data_resolution*/

}
/*----------------------------------------------------------------------------*/
static int BMA220_ReadData(struct i2c_client *client, s16 data[BMA220_AXES_NUM])
{
	struct bma220_i2c_data *priv = i2c_get_clientdata(client);        
	u8 addr = BMA220_REG_DATAXLOW;
	u8 buf[BMA220_DATA_LEN] = {0};
	int err = 0;
	int i;
	int tmp=0;
	u8 ofs[3];

	if(NULL == client)
	{
		err = -EINVAL;
	}
	else if(err = hwmsen_read_block(client, addr, buf, BMA220_DATA_LEN))
	{
		GSE_ERR("error: %d\n", err);
	}
	else
	{
		for(i=0;i<BMA220_AXES_NUM;i++)
		{
			buf[i]  = buf[i] >>2;
			if(buf[i]  & 0x20)
				data[i] = (s16)buf[i] | 0xFFC0;
			else
				data[i] = (s16)(buf[i]);
		}
		//data[BMA220_AXIS_X] = (s16)((buf[BMA220_AXIS_X])<< 8)/(1<<10) ;
		//data[BMA220_AXIS_Y] = (s16)((buf[BMA220_AXIS_Y])<< 8)/(1<<10) ;
		//data[BMA220_AXIS_Z] = (s16)((buf[BMA220_AXIS_Z])<< 8)/(1<<10) ;
		if(atomic_read(&priv->trace) & ADX_TRC_RAWDATA)
		{
			GSE_LOG(" raw data1[%08X %08X %08X] => [%5d %5d %5d]\n", buf[BMA220_AXIS_X], buf[BMA220_AXIS_Y], buf[BMA220_AXIS_Z],
		                               data[BMA220_AXIS_X], data[BMA220_AXIS_Y], data[BMA220_AXIS_Z]);
		}
#ifdef CONFIG_BMA220_LOWPASS
		if(atomic_read(&priv->filter))
		{
			if(atomic_read(&priv->fir_en) && !atomic_read(&priv->suspend))
			{
				int idx, firlen = atomic_read(&priv->firlen);   
				if(priv->fir.num < firlen)
				{                
					priv->fir.raw[priv->fir.num][BMA220_AXIS_X] = data[BMA220_AXIS_X];
					priv->fir.raw[priv->fir.num][BMA220_AXIS_Y] = data[BMA220_AXIS_Y];
					priv->fir.raw[priv->fir.num][BMA220_AXIS_Z] = data[BMA220_AXIS_Z];
					priv->fir.sum[BMA220_AXIS_X] += data[BMA220_AXIS_X];
					priv->fir.sum[BMA220_AXIS_Y] += data[BMA220_AXIS_Y];
					priv->fir.sum[BMA220_AXIS_Z] += data[BMA220_AXIS_Z];
					if(atomic_read(&priv->trace) & ADX_TRC_FILTER)
					{
						GSE_LOG("add [%2d] [%5d %5d %5d] => [%5d %5d %5d]\n", priv->fir.num,
							priv->fir.raw[priv->fir.num][BMA220_AXIS_X], priv->fir.raw[priv->fir.num][BMA220_AXIS_Y], priv->fir.raw[priv->fir.num][BMA220_AXIS_Z],
							priv->fir.sum[BMA220_AXIS_X], priv->fir.sum[BMA220_AXIS_Y], priv->fir.sum[BMA220_AXIS_Z]);
					}
					priv->fir.num++;
					priv->fir.idx++;
				}
				else
				{
					idx = priv->fir.idx % firlen;
					priv->fir.sum[BMA220_AXIS_X] -= priv->fir.raw[idx][BMA220_AXIS_X];
					priv->fir.sum[BMA220_AXIS_Y] -= priv->fir.raw[idx][BMA220_AXIS_Y];
					priv->fir.sum[BMA220_AXIS_Z] -= priv->fir.raw[idx][BMA220_AXIS_Z];
					priv->fir.raw[idx][BMA220_AXIS_X] = data[BMA220_AXIS_X];
					priv->fir.raw[idx][BMA220_AXIS_Y] = data[BMA220_AXIS_Y];
					priv->fir.raw[idx][BMA220_AXIS_Z] = data[BMA220_AXIS_Z];
					priv->fir.sum[BMA220_AXIS_X] += data[BMA220_AXIS_X];
					priv->fir.sum[BMA220_AXIS_Y] += data[BMA220_AXIS_Y];
					priv->fir.sum[BMA220_AXIS_Z] += data[BMA220_AXIS_Z];
					priv->fir.idx++;
					data[BMA220_AXIS_X] = priv->fir.sum[BMA220_AXIS_X]/firlen;
					data[BMA220_AXIS_Y] = priv->fir.sum[BMA220_AXIS_Y]/firlen;
					data[BMA220_AXIS_Z] = priv->fir.sum[BMA220_AXIS_Z]/firlen;
					if(atomic_read(&priv->trace) & ADX_TRC_FILTER)
					{
						GSE_LOG("add [%2d] [%5d %5d %5d] => [%5d %5d %5d] : [%5d %5d %5d]\n", idx,
						priv->fir.raw[idx][BMA220_AXIS_X], priv->fir.raw[idx][BMA220_AXIS_Y], priv->fir.raw[idx][BMA220_AXIS_Z],
						priv->fir.sum[BMA220_AXIS_X], priv->fir.sum[BMA220_AXIS_Y], priv->fir.sum[BMA220_AXIS_Z],
						data[BMA220_AXIS_X], data[BMA220_AXIS_Y], data[BMA220_AXIS_Z]);
					}
				}
			}
		}	
#endif         
	}
	return err;
}
/*----------------------------------------------------------------------------*/
static int BMA220_ReadOffset(struct i2c_client *client, s8 ofs[BMA220_AXES_NUM])
{    
	int err;
#ifdef SW_CALIBRATION
	ofs[0]=ofs[1]=ofs[2]=0x0;
#else
	if(err = hwmsen_read_block(client, BMA220_REG_OFSX, ofs, BMA220_AXES_NUM))
	{
		GSE_ERR("error: %d\n", err);
	}
#endif
	//printk("offesx=%x, y=%x, z=%x",ofs[0],ofs[1],ofs[2]);
	
	return err;    
}
/*----------------------------------------------------------------------------*/
static int BMA220_ResetCalibration(struct i2c_client *client)
{
	struct bma220_i2c_data *obj = i2c_get_clientdata(client);
	u8 ofs[4]={0,0,0,0};
	int err;
	
	#ifdef SW_CALIBRATION
		
	#else
		if(err = hwmsen_write_block(client, BMA220_REG_OFSX, ofs, BMA220_AXES_NUM))
		{
			GSE_ERR("error: %d\n", err);
		}
	#endif

	memset(obj->cali_sw, 0x00, sizeof(obj->cali_sw));
	memset(obj->offset, 0x00, sizeof(obj->offset));
	return err;    
}
/*----------------------------------------------------------------------------*/
static int BMA220_ReadCalibration(struct i2c_client *client, int dat[BMA220_AXES_NUM])
{
    struct bma220_i2c_data *obj = i2c_get_clientdata(client);
    int err;
    int mul;

	#ifdef SW_CALIBRATION
		mul = 0;//only SW Calibration, disable HW Calibration
	#else
	    if ((err = BMA220_ReadOffset(client, obj->offset))) {
        GSE_ERR("read offset fail, %d\n", err);
        return err;
    	}    
    	mul = obj->reso->sensitivity/bma220_offset_resolution.sensitivity;
	#endif

    dat[obj->cvt.map[BMA220_AXIS_X]] = obj->cvt.sign[BMA220_AXIS_X]*(obj->offset[BMA220_AXIS_X]*mul + obj->cali_sw[BMA220_AXIS_X]);
    dat[obj->cvt.map[BMA220_AXIS_Y]] = obj->cvt.sign[BMA220_AXIS_Y]*(obj->offset[BMA220_AXIS_Y]*mul + obj->cali_sw[BMA220_AXIS_Y]);
    dat[obj->cvt.map[BMA220_AXIS_Z]] = obj->cvt.sign[BMA220_AXIS_Z]*(obj->offset[BMA220_AXIS_Z]*mul + obj->cali_sw[BMA220_AXIS_Z]);                        
                                       
    return 0;
}
/*----------------------------------------------------------------------------*/
static int BMA220_ReadCalibrationEx(struct i2c_client *client, int act[BMA220_AXES_NUM], int raw[BMA220_AXES_NUM])
{  
	/*raw: the raw calibration data; act: the actual calibration data*/
	struct bma220_i2c_data *obj = i2c_get_clientdata(client);
	int err;
	int mul;

 

	#ifdef SW_CALIBRATION
		mul = 0;//only SW Calibration, disable HW Calibration
	#else
		if(err = BMA220_ReadOffset(client, obj->offset))
		{
			GSE_ERR("read offset fail, %d\n", err);
			return err;
		}   
		mul = obj->reso->sensitivity/bma220_offset_resolution.sensitivity;
	#endif
	
	raw[BMA220_AXIS_X] = obj->offset[BMA220_AXIS_X]*mul + obj->cali_sw[BMA220_AXIS_X];
	raw[BMA220_AXIS_Y] = obj->offset[BMA220_AXIS_Y]*mul + obj->cali_sw[BMA220_AXIS_Y];
	raw[BMA220_AXIS_Z] = obj->offset[BMA220_AXIS_Z]*mul + obj->cali_sw[BMA220_AXIS_Z];

	act[obj->cvt.map[BMA220_AXIS_X]] = obj->cvt.sign[BMA220_AXIS_X]*raw[BMA220_AXIS_X];
	act[obj->cvt.map[BMA220_AXIS_Y]] = obj->cvt.sign[BMA220_AXIS_Y]*raw[BMA220_AXIS_Y];
	act[obj->cvt.map[BMA220_AXIS_Z]] = obj->cvt.sign[BMA220_AXIS_Z]*raw[BMA220_AXIS_Z];                        
	                       
	return 0;
}
/*----------------------------------------------------------------------------*/
static int BMA220_WriteCalibration(struct i2c_client *client, int dat[BMA220_AXES_NUM])
{
	struct bma220_i2c_data *obj = i2c_get_clientdata(client);
	int err;
	int cali[BMA220_AXES_NUM], raw[BMA220_AXES_NUM];
	int lsb = bma220_offset_resolution.sensitivity;
	int divisor = obj->reso->sensitivity/lsb;

	if(err = BMA220_ReadCalibrationEx(client, cali, raw))	/*offset will be updated in obj->offset*/
	{ 
		GSE_ERR("read offset fail, %d\n", err);
		return err;
	}

	GSE_LOG("OLDOFF: (%+3d %+3d %+3d): (%+3d %+3d %+3d) / (%+3d %+3d %+3d)\n", 
		raw[BMA220_AXIS_X], raw[BMA220_AXIS_Y], raw[BMA220_AXIS_Z],
		obj->offset[BMA220_AXIS_X], obj->offset[BMA220_AXIS_Y], obj->offset[BMA220_AXIS_Z],
		obj->cali_sw[BMA220_AXIS_X], obj->cali_sw[BMA220_AXIS_Y], obj->cali_sw[BMA220_AXIS_Z]);

	/*calculate the real offset expected by caller*/
	cali[BMA220_AXIS_X] += dat[BMA220_AXIS_X];
	cali[BMA220_AXIS_Y] += dat[BMA220_AXIS_Y];
	cali[BMA220_AXIS_Z] += dat[BMA220_AXIS_Z];

	GSE_LOG("UPDATE: (%+3d %+3d %+3d)\n", 
		dat[BMA220_AXIS_X], dat[BMA220_AXIS_Y], dat[BMA220_AXIS_Z]);

#ifdef SW_CALIBRATION
	obj->cali_sw[BMA220_AXIS_X] = obj->cvt.sign[BMA220_AXIS_X]*(cali[obj->cvt.map[BMA220_AXIS_X]]);
	obj->cali_sw[BMA220_AXIS_Y] = obj->cvt.sign[BMA220_AXIS_Y]*(cali[obj->cvt.map[BMA220_AXIS_Y]]);
	obj->cali_sw[BMA220_AXIS_Z] = obj->cvt.sign[BMA220_AXIS_Z]*(cali[obj->cvt.map[BMA220_AXIS_Z]]);	
#else
	obj->offset[BMA220_AXIS_X] = (s8)(obj->cvt.sign[BMA220_AXIS_X]*(cali[obj->cvt.map[BMA220_AXIS_X]])/(divisor));
	obj->offset[BMA220_AXIS_Y] = (s8)(obj->cvt.sign[BMA220_AXIS_Y]*(cali[obj->cvt.map[BMA220_AXIS_Y]])/(divisor));
	obj->offset[BMA220_AXIS_Z] = (s8)(obj->cvt.sign[BMA220_AXIS_Z]*(cali[obj->cvt.map[BMA220_AXIS_Z]])/(divisor));

	/*convert software calibration using standard calibration*/
	obj->cali_sw[BMA220_AXIS_X] = obj->cvt.sign[BMA220_AXIS_X]*(cali[obj->cvt.map[BMA220_AXIS_X]])%(divisor);
	obj->cali_sw[BMA220_AXIS_Y] = obj->cvt.sign[BMA220_AXIS_Y]*(cali[obj->cvt.map[BMA220_AXIS_Y]])%(divisor);
	obj->cali_sw[BMA220_AXIS_Z] = obj->cvt.sign[BMA220_AXIS_Z]*(cali[obj->cvt.map[BMA220_AXIS_Z]])%(divisor);

	GSE_LOG("NEWOFF: (%+3d %+3d %+3d): (%+3d %+3d %+3d) / (%+3d %+3d %+3d)\n", 
		obj->offset[BMA220_AXIS_X]*divisor + obj->cali_sw[BMA220_AXIS_X], 
		obj->offset[BMA220_AXIS_Y]*divisor + obj->cali_sw[BMA220_AXIS_Y], 
		obj->offset[BMA220_AXIS_Z]*divisor + obj->cali_sw[BMA220_AXIS_Z], 
		obj->offset[BMA220_AXIS_X], obj->offset[BMA220_AXIS_Y], obj->offset[BMA220_AXIS_Z],
		obj->cali_sw[BMA220_AXIS_X], obj->cali_sw[BMA220_AXIS_Y], obj->cali_sw[BMA220_AXIS_Z]);

	if(err = hwmsen_write_block(obj->client, BMA220_REG_OFSX, obj->offset, BMA220_AXES_NUM))
	{
		GSE_ERR("write offset fail: %d\n", err);
		return err;
	}
#endif

	return err;
}
/*----------------------------------------------------------------------------*/
static int BMA220_CheckDeviceID(struct i2c_client *client)
{
	u8 databuf[2];    
	int res = 0;

	memset(databuf, 0, sizeof(u8)*2);    
	databuf[0] = BMA220_REG_DEVID;    
	
       //printk("MYCAT BMA220_CheckDeviceID\n");
	res = i2c_master_send(client, databuf, 0x1);
	if(res <= 0)
	{
	     printk("MYCAT BMA220_CheckDeviceID error 1=%d\n", res);
		 databuf[0] = BMA220_REG_DEVID;    
	    res = i2c_master_send(client, databuf, 0x1);
	}

	if(res <= 0)
	{
	     printk("MYCAT BMA220_CheckDeviceID error 2=%d\n", res);
		 databuf[0] = BMA220_REG_DEVID;    
	    res = i2c_master_send(client, databuf, 0x1);
	}


      if(res <= 0)
	{
	     printk("MYCAT BMA220_CheckDeviceID error 3=%d\n", res);
		 databuf[0] = BMA220_REG_DEVID;    
	    res = i2c_master_send(client, databuf, 0x1);
	}

	if(res <= 0)
	{
	printk("MYCAT BMA220_CheckDeviceID error 4=%d\n", res);
		goto exit_BMA220_CheckDeviceID;
	}
	
	udelay(500);

	databuf[0] = 0x0;        
	res = i2c_master_recv(client, databuf, 0x01);
	if(res <= 0)
	{
	       printk("MYCAT BMA220_CheckDeviceID error 2=%d\n", res);
		goto exit_BMA220_CheckDeviceID;
	}
	

	if(databuf[0]!=BMA220_FIXED_DEVID)
	{
		printk("MYCAT BMA220_CheckDeviceID %d failt!\n ", databuf[0]);
		return BMA220_ERR_IDENTIFICATION;
	}
	else
	{
		//printk("MYCAT BMA220_CheckDeviceID %d pass!\n ", databuf[0]);
	}

	exit_BMA220_CheckDeviceID:
	if (res <= 0)
	{
		return BMA220_ERR_I2C;
	}
	
	return BMA220_SUCCESS;
}
/*----------------------------------------------------------------------------*/
static int BMA220_SetPowerMode(struct i2c_client *client, bool enable)
{
	u8 databuf[2];    
	int res = 0;
	u8 addr = BMA220_REG_POWER_CTL;
	struct bma220_i2c_data *obj = i2c_get_clientdata(client);
	
	
	if(enable == sensor_power )
	{
		GSE_LOG("Sensor power status is newest!\n");
		return BMA220_SUCCESS;
	}

	if(hwmsen_read_block(client, addr, databuf, 0x01))
	{
		GSE_ERR("read power ctl register err!\n");
		return BMA220_ERR_I2C;
	}
	GSE_LOG("BMA220_SetPowerMode databuf[0]=%x!\n",databuf[0]);
	
	if(enable == TRUE)
	{
		databuf[0] &= ~BMA220_SLEEP_EN_MODE;
		databuf[0] |= (BMA220_EN_CHANNEL_X | BMA220_EN_CHANNEL_Y | BMA220_EN_CHANNEL_Z);
	}
	else
	{
		databuf[0] &= ~BMA220_CHANNEL_MASK;
		databuf[0] |= BMA220_SLEEP_EN_MODE;
	}
	databuf[1] = databuf[0];
	databuf[0] = BMA220_REG_POWER_CTL;
	

	res = i2c_master_send(client, databuf, 0x2);

	if(res <= 0)
	{
		GSE_LOG("set power mode failed!\n");
		return BMA220_ERR_I2C;
	}
	else if(atomic_read(&obj->trace) & ADX_TRC_INFO)
	{
		GSE_LOG("set power mode ok %d!\n", databuf[1]);
	}

	GSE_LOG("BMA220_SetPowerMode ok enable=%d!\n",enable);


	sensor_power = enable;

	mdelay(20);
	
	return BMA220_SUCCESS;    
}
/*----------------------------------------------------------------------------*/
static int BMA220_SetDataFormat(struct i2c_client *client, u8 dataformat)
{
	struct bma220_i2c_data *obj = i2c_get_clientdata(client);
	u8 databuf[10];    
	int res = 0;

	memset(databuf, 0, sizeof(u8)*10);    

	if(hwmsen_read_block(client, BMA220_REG_DATA_FORMAT, databuf, 0x01))
	{
		printk("bma220 read Dataformat failt \n");
		return BMA220_ERR_I2C;
	}

	databuf[0] &= ~BMA220_RANGE_MASK;
	databuf[0] |= dataformat;
	databuf[1] = databuf[0];
	databuf[0] = BMA220_REG_DATA_FORMAT;


	res = i2c_master_send(client, databuf, 0x2);

	if(res <= 0)
	{
		return BMA220_ERR_I2C;
	}
	
	//printk("BMA220_SetDataFormat OK! \n");
	

	return BMA220_SetDataResolution(obj);    
}
/*----------------------------------------------------------------------------*/
static int BMA220_SetBWRate(struct i2c_client *client, u8 bwrate)
{
	u8 databuf[10];    
	int res = 0;

	memset(databuf, 0, sizeof(u8)*10);    

	if(hwmsen_read_block(client, BMA220_REG_BW_RATE, databuf, 0x01))
	{
		printk("bma220 read rate failt \n");
		return BMA220_ERR_I2C;
	}

	databuf[0] &= ~BMA220_BW_MASK;
	databuf[0] |= bwrate;
	databuf[1] = databuf[0];
	databuf[0] = BMA220_REG_BW_RATE;


	res = i2c_master_send(client, databuf, 0x2);

	if(res <= 0)
	{
		return BMA220_ERR_I2C;
	}
	
	//printk("BMA220_SetBWRate OK! \n");
	
	return BMA220_SUCCESS;    
}
/*----------------------------------------------------------------------------*/
static int BMA220_SetIntEnable(struct i2c_client *client, u8 intenable)
{
			u8 databuf[10];    
			int res = 0;
		
			res = hwmsen_write_byte(client, BMA220_INT_REG, 0x00);
			if(res != BMA220_SUCCESS) 
			{
				return res;
			}
			res = hwmsen_write_byte(client, BMA220_INT1_REG, 0x00);
			if(res != BMA220_SUCCESS) 
			{
				return res;
			}			
			printk("BMA220 disable interrupt ...\n");
		
			/*for disable interrupt function*/
			
			return BMA220_SUCCESS;	  
}

/*----------------------------------------------------------------------------*/
static int bma220_init_client(struct i2c_client *client, int reset_cali)
{
	struct bma220_i2c_data *obj = i2c_get_clientdata(client);
	int res = 0;

	res = BMA220_CheckDeviceID(client); 
	if(res != BMA220_SUCCESS)
	{
		return res;
	}	
	
	res = BMA220_SetPowerMode(client, false);
	if(res != BMA220_SUCCESS)
	{
		return res;
	}
	printk("BMA220_SetPowerMode OK!\n");
	
	res = BMA220_SetBWRate(client, BMA220_BW_125HZ);
	if(res != BMA220_SUCCESS ) 
	{
		return res;
	}
	printk("BMA220_SetBWRate OK!\n");
	
	res = BMA220_SetDataFormat(client, BMA220_RANGE_2G);
	if(res != BMA220_SUCCESS) 
	{
		return res;
	}
	printk("BMA220_SetDataFormat OK!\n");

	gsensor_gain.x = gsensor_gain.y = gsensor_gain.z = obj->reso->sensitivity;


	res = BMA220_SetIntEnable(client, 0x00);        
	if(res != BMA220_SUCCESS)
	{
		return res;
	}
	printk("BMA220 disable interrupt function!\n");


	if(0 != reset_cali)
	{ 
		/*reset calibration only in power on*/
		res = BMA220_ResetCalibration(client);
		if(res != BMA220_SUCCESS)
		{
			return res;
		}
	}
	printk("bma220_init_client OK!\n");
#ifdef CONFIG_BMA220_LOWPASS
	memset(&obj->fir, 0x00, sizeof(obj->fir));  
#endif

	mdelay(20);

	return BMA220_SUCCESS;
}
/*----------------------------------------------------------------------------*/
static int BMA220_ReadChipInfo(struct i2c_client *client, char *buf, int bufsize)
{
	u8 databuf[10];    

	memset(databuf, 0, sizeof(u8)*10);

	if((NULL == buf)||(bufsize<=30))
	{
		return -1;
	}
	
	if(NULL == client)
	{
		*buf = 0;
		return -2;
	}

	sprintf(buf, "BMA220 Chip");
	return 0;
}
/*----------------------------------------------------------------------------*/
static int BMA220_ReadSensorData(struct i2c_client *client, char *buf, int bufsize)
{
	struct bma220_i2c_data *obj = (struct bma220_i2c_data*)i2c_get_clientdata(client);
	u8 databuf[20];
	int acc[BMA220_AXES_NUM];
	int res = 0;
	memset(databuf, 0, sizeof(u8)*10);

	if(NULL == buf)
	{
		return -1;
	}
	if(NULL == client)
	{
		*buf = 0;
		return -2;
	}

	if(sensor_power == FALSE)
	{
		res = BMA220_SetPowerMode(client, true);
		if(res)
		{
			GSE_ERR("Power on bma220 error %d!\n", res);
		}
	}

	if(res = BMA220_ReadData(client, obj->data))
	{        
		GSE_ERR("I2C error: ret value=%d", res);
		return -3;
	}
	else
	{
		mm_segment_t old_fs = get_fs();
		char strbuf[BMA220_BUFSIZE];
		s16   data[BMA220_AXES_NUM+1];
		struct file *fp;
	    	set_fs(KERNEL_DS);
		
		fp = filp_open(FTM_CUST_ACC,O_RDONLY,0);

		if(fp != 0xfffffffe)//open file suscess
		{
			//fp->f_op->llseek(fp, 0, 0);
			fp->f_pos = 0;
			fp->f_op->read(fp,
						  strbuf,
						  BMA220_BUFSIZE,
						  &fp->f_pos);	
			filp_close(fp, NULL);	
			set_fs(old_fs);	
			sscanf(strbuf, "%02x %02x %02x", &data[BMA220_AXIS_X], &data[BMA220_AXIS_Y], &data[BMA220_AXIS_Z]);
			obj->data[BMA220_AXIS_X] -= data[BMA220_AXIS_X];
			obj->data[BMA220_AXIS_Y] -= data[BMA220_AXIS_Y];
			obj->data[BMA220_AXIS_Z] -= (data[BMA220_AXIS_Z]-(obj->cvt.sign[BMA220_AXIS_Z]*obj->reso->sensitivity));		
			//printk("BMA220_SET_CALIBRATION value is %x %x %x -> %d %d %d\r\n",data[BMA220_AXIS_X], data[BMA220_AXIS_Y], data[BMA220_AXIS_Z],obj->data[BMA220_AXIS_X],obj->data[BMA220_AXIS_Y],obj->data[BMA220_AXIS_Z]);
		}
#ifdef 	BIRD_GSENSOR_BMA220_X	
		else{
			data[BMA220_AXIS_X]=simple_strtol(BIRD_GSENSOR_BMA220_X, NULL, 0);
			data[BMA220_AXIS_Y]=simple_strtol(BIRD_GSENSOR_BMA220_Y, NULL, 0);
			data[BMA220_AXIS_Z]=simple_strtol(BIRD_GSENSOR_BMA220_Z, NULL, 0);		
			obj->data[BMA220_AXIS_X] -= data[BMA220_AXIS_X];
			obj->data[BMA220_AXIS_Y] -= data[BMA220_AXIS_Y];
			obj->data[BMA220_AXIS_Z] -= (data[BMA220_AXIS_Z]-(obj->cvt.sign[BMA220_AXIS_Z]*obj->reso->sensitivity));		
			//printk("BMA220_SET_CALIBRATION value is %x %x %x -> %d %d %d\r\n",data[BMA220_AXIS_X], data[BMA220_AXIS_Y], data[BMA220_AXIS_Z],obj->data[BMA220_AXIS_X],obj->data[BMA220_AXIS_Y],obj->data[BMA220_AXIS_Z]);
		}
#endif

		
		//printk("raw data x=%d, y=%d, z=%d \n",obj->data[BMA220_AXIS_X],obj->data[BMA220_AXIS_Y],obj->data[BMA220_AXIS_Z]);
		obj->data[BMA220_AXIS_X] += obj->cali_sw[BMA220_AXIS_X];
		obj->data[BMA220_AXIS_Y] += obj->cali_sw[BMA220_AXIS_Y];
		obj->data[BMA220_AXIS_Z] += obj->cali_sw[BMA220_AXIS_Z];
		
		//printk("cali_sw x=%d, y=%d, z=%d \n",obj->cali_sw[BMA220_AXIS_X],obj->cali_sw[BMA220_AXIS_Y],obj->cali_sw[BMA220_AXIS_Z]);
		
		/*remap coordinate*/
		acc[obj->cvt.map[BMA220_AXIS_X]] = obj->cvt.sign[BMA220_AXIS_X]*obj->data[BMA220_AXIS_X];
		acc[obj->cvt.map[BMA220_AXIS_Y]] = obj->cvt.sign[BMA220_AXIS_Y]*obj->data[BMA220_AXIS_Y];
		acc[obj->cvt.map[BMA220_AXIS_Z]] = obj->cvt.sign[BMA220_AXIS_Z]*obj->data[BMA220_AXIS_Z];
		//printk("cvt x=%d, y=%d, z=%d \n",obj->cvt.sign[BMA220_AXIS_X],obj->cvt.sign[BMA220_AXIS_Y],obj->cvt.sign[BMA220_AXIS_Z]);


		//GSE_LOG("Mapped gsensor data: %d, %d, %d!\n", acc[BMA220_AXIS_X], acc[BMA220_AXIS_Y], acc[BMA220_AXIS_Z]);

		//Out put the mg
		//printk("mg acc=%d, GRAVITY=%d, sensityvity=%d \n",acc[BMA220_AXIS_X],GRAVITY_EARTH_1000,obj->reso->sensitivity);
		acc[BMA220_AXIS_X] = acc[BMA220_AXIS_X] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
		acc[BMA220_AXIS_Y] = acc[BMA220_AXIS_Y] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
		acc[BMA220_AXIS_Z] = acc[BMA220_AXIS_Z] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;		
		

		sprintf(buf, "%04x %04x %04x", acc[BMA220_AXIS_X], acc[BMA220_AXIS_Y], acc[BMA220_AXIS_Z]);
		if(atomic_read(&obj->trace) & ADX_TRC_IOCTL)
		{
			GSE_LOG("gsensor data: %s!\n", buf);
		}
	}
	
	return 0;
}

static int BMA220_ReadSensorData_factory_autotest(struct i2c_client *client, char *buf, int bufsize)
{
	struct bma220_i2c_data *obj = (struct bma220_i2c_data*)i2c_get_clientdata(client);
	u8 databuf[20];
	int acc[BMA220_AXES_NUM];
	int res = 0;
	memset(databuf, 0, sizeof(u8)*10);

	if(NULL == buf)
	{
		return -1;
	}
	if(NULL == client)
	{
		*buf = 0;
		return -2;
	}

	if(sensor_power == FALSE)
	{
		res = BMA220_SetPowerMode(client, true);
		if(res)
		{
			GSE_ERR("Power on bma220 error %d!\n", res);
		}
	}

	if(res = BMA220_ReadData(client, obj->data))
	{        
		GSE_ERR("I2C error: ret value=%d", res);
		return -3;
	}
	else
	{
		mm_segment_t old_fs = get_fs();
		char strbuf[BMA220_BUFSIZE];
		s16   data[BMA220_AXES_NUM+1];
		struct file *fp;
	    	set_fs(KERNEL_DS);
		
		fp = filp_open(FTM_CUST_ACC,O_RDONLY,0);

		if(fp != 0xfffffffe)//open file suscess
		{
			//fp->f_op->llseek(fp, 0, 0);
			fp->f_pos = 0;
			fp->f_op->read(fp,
						  strbuf,
						  BMA220_BUFSIZE,
						  &fp->f_pos);	
			filp_close(fp, NULL);	
			set_fs(old_fs);	
			sscanf(strbuf, "%02x %02x %02x", &data[BMA220_AXIS_X], &data[BMA220_AXIS_Y], &data[BMA220_AXIS_Z]);
			obj->data[BMA220_AXIS_X] -= data[BMA220_AXIS_X];
			obj->data[BMA220_AXIS_Y] -= data[BMA220_AXIS_Y];
			obj->data[BMA220_AXIS_Z] -= (data[BMA220_AXIS_Z]-(obj->cvt.sign[BMA220_AXIS_Z]*obj->reso->sensitivity));		
			//printk("BMA220_SET_CALIBRATION value is %x %x %x -> %d %d %d\r\n",data[BMA220_AXIS_X], data[BMA220_AXIS_Y], data[BMA220_AXIS_Z],obj->data[BMA220_AXIS_X],obj->data[BMA220_AXIS_Y],obj->data[BMA220_AXIS_Z]);
		}	
		//printk("raw data x=%d, y=%d, z=%d \n",obj->data[BMA220_AXIS_X],obj->data[BMA220_AXIS_Y],obj->data[BMA220_AXIS_Z]);
		obj->data[BMA220_AXIS_X] += obj->cali_sw[BMA220_AXIS_X];
		obj->data[BMA220_AXIS_Y] += obj->cali_sw[BMA220_AXIS_Y];
		obj->data[BMA220_AXIS_Z] += obj->cali_sw[BMA220_AXIS_Z];
		
		//printk("cali_sw x=%d, y=%d, z=%d \n",obj->cali_sw[BMA220_AXIS_X],obj->cali_sw[BMA220_AXIS_Y],obj->cali_sw[BMA220_AXIS_Z]);
		
		/*remap coordinate*/
		acc[obj->cvt.map[BMA220_AXIS_X]] = obj->cvt.sign[BMA220_AXIS_X]*obj->data[BMA220_AXIS_X];
		acc[obj->cvt.map[BMA220_AXIS_Y]] = obj->cvt.sign[BMA220_AXIS_Y]*obj->data[BMA220_AXIS_Y];
		acc[obj->cvt.map[BMA220_AXIS_Z]] = obj->cvt.sign[BMA220_AXIS_Z]*obj->data[BMA220_AXIS_Z];
		//printk("cvt x=%d, y=%d, z=%d \n",obj->cvt.sign[BMA220_AXIS_X],obj->cvt.sign[BMA220_AXIS_Y],obj->cvt.sign[BMA220_AXIS_Z]);


		//GSE_LOG("Mapped gsensor data: %d, %d, %d!\n", acc[BMA220_AXIS_X], acc[BMA220_AXIS_Y], acc[BMA220_AXIS_Z]);

		//Out put the mg
		//printk("mg acc=%d, GRAVITY=%d, sensityvity=%d \n",acc[BMA220_AXIS_X],GRAVITY_EARTH_1000,obj->reso->sensitivity);
		acc[BMA220_AXIS_X] = acc[BMA220_AXIS_X] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
		acc[BMA220_AXIS_Y] = acc[BMA220_AXIS_Y] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
		acc[BMA220_AXIS_Z] = acc[BMA220_AXIS_Z] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;		
		
		//add by huangrunming 20120907
		#ifdef FACTORY_AUTOTEST_GSENSOR_VALUE
		        auto_now_data[BMA220_AXIS_X]= acc[BMA220_AXIS_X];
		        auto_now_data[BMA220_AXIS_Y]= acc[BMA220_AXIS_Y];
		        auto_now_data[BMA220_AXIS_Z]= acc[BMA220_AXIS_Z];
		        
		        if((acc[BMA220_AXIS_X] > 5000) || (acc[BMA220_AXIS_X] < -5000)){
		        printk("auto_now_data x fail: \n");
	        			return -4;
		        }
		        if(acc[BMA220_AXIS_Y] > 5000 || acc[BMA220_AXIS_Y] < -5000){
		        printk("auto_now_data y fail: \n");
	        			return -5;
		        }
		        //up,down
		        if((acc[BMA220_AXIS_Z] > 5000 && acc[BMA220_AXIS_Z] < 15000) || (acc[BMA220_AXIS_Z] > -15000 && acc[BMA220_AXIS_Z] < -5000))
		        {
		        	//ok
		        }else{
		            printk("auto_now_data z fail: \n");
	        	    return -6;
		        }
		#endif
		
		sprintf(buf, "%04x %04x %04x", acc[BMA220_AXIS_X], acc[BMA220_AXIS_Y], acc[BMA220_AXIS_Z]);
		if(atomic_read(&obj->trace) & ADX_TRC_IOCTL)
		{
			GSE_LOG("gsensor data: %s!\n", buf);
		}
	}
	
	return 0;
}
/*----------------------------------------------------------------------------*/
static int BMA220_ReadRawData(struct i2c_client *client, char *buf)
{
	struct bma220_i2c_data *obj = (struct bma220_i2c_data*)i2c_get_clientdata(client);
	int res = 0;

	if (!buf || !client)
	{
		return EINVAL;
	}
	
	if(res = BMA220_ReadData(client, obj->data))
	{        
		GSE_ERR("I2C error: ret value=%d", res);
		return EIO;
	}
	else
	{
		sprintf(buf, "BMA220_ReadRawData %04x %04x %04x", obj->data[BMA220_AXIS_X], 
			obj->data[BMA220_AXIS_Y], obj->data[BMA220_AXIS_Z]);
	
	}
	
	return 0;
}
/*----------------------------------------------------------------------------*/
static int BMA220_InitSelfTest(struct i2c_client *client)
{
	int res = 0;

	BMA220_SetPowerMode(client, true);

	res = BMA220_SetBWRate(client, BMA220_BW_32HZ);
	if(res != BMA220_SUCCESS ) //0x2C->BW=100Hz
	{
		return res;
	}
	
	res = BMA220_SetDataFormat(client, BMA220_RANGE_4G);
	if(res != BMA220_SUCCESS) //0x2C->BW=100Hz
	{
		return res;
	}
	mdelay(60);
	
	return BMA220_SUCCESS;
}
/*----------------------------------------------------------------------------*/
static int BMA220_JudgeTestResult(struct i2c_client *client)
{

	struct bma220_i2c_data *obj = (struct bma220_i2c_data*)i2c_get_clientdata(client);
	int res = 0;
	s16  acc[BMA220_AXES_NUM];
	int  self_result;

	
	if(res = BMA220_ReadData(client, acc))
	{        
		GSE_ERR("I2C error: ret value=%d", res);
		return EIO;
	}
	else
	{			
		printk("0 step: %d %d %d\n", acc[0],acc[1],acc[2]);

		acc[BMA220_AXIS_X] = acc[BMA220_AXIS_X] * 1000 / 128;
		acc[BMA220_AXIS_Y] = acc[BMA220_AXIS_Y] * 1000 / 128;
		acc[BMA220_AXIS_Z] = acc[BMA220_AXIS_Z] * 1000 / 128;
		
		printk("1 step: %d %d %d\n", acc[0],acc[1],acc[2]);
		
		self_result = acc[BMA220_AXIS_X]*acc[BMA220_AXIS_X] 
			+ acc[BMA220_AXIS_Y]*acc[BMA220_AXIS_Y] 
			+ acc[BMA220_AXIS_Z]*acc[BMA220_AXIS_Z];
			
		
		printk("2 step: result = %d", self_result);

	    if ( (self_result>550000) && (self_result<1700000) ) //between 0.55g and 1.7g 
	    {												 
			GSE_ERR("BMA220_JudgeTestResult successful\n");
			return BMA220_SUCCESS;
		}
		{
	        GSE_ERR("BMA220_JudgeTestResult failt\n");
	        return -EINVAL;
	    }
	
	}
	
}

/*----------------------------------------------------------------------------*/
static int BMA220_SET_CALIBRATION(struct i2c_client *client)
{
	struct bma220_i2c_data *obj = (struct bma220_i2c_data*)i2c_get_clientdata(client);
	char strbuf[BMA220_BUFSIZE];
	s16   data[BMA220_AXES_NUM+1];
	struct file *fp;
	BMA220_ReadData(client, data);
	sprintf(strbuf, "%02x %02x %02x", data[BMA220_AXIS_X], data[BMA220_AXIS_Y], data[BMA220_AXIS_Z]);	
	//write to nvram
	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);

	fp = filp_open(FTM_CUST_ACC,O_WRONLY|O_CREAT, 0644);
	fp->f_pos = 0;
	fp->f_op->write(fp,
				  strbuf,
				  BMA220_BUFSIZE,
				  &fp->f_pos);	
	filp_close(fp, NULL);	
	printk("BMA220_SET_CALIBRATION value is %x %x %x\r\n",data[BMA220_AXIS_X], data[BMA220_AXIS_Y], data[BMA220_AXIS_Z]);

	set_fs(old_fs);	
	return 0;
}

/*----------------------------------------------------------------------------*/
static ssize_t show_chipinfo_value(struct device_driver *ddri, char *buf)
{
	struct i2c_client *client = bma220_i2c_client;
	char strbuf[BMA220_BUFSIZE];
	if(NULL == client)
	{
		GSE_ERR("i2c client is null!!\n");
		return 0;
	}
	
	BMA220_ReadChipInfo(client, strbuf, BMA220_BUFSIZE);
	return snprintf(buf, PAGE_SIZE, "%s\n", strbuf);        
}

static ssize_t gsensor_init(struct device_driver *ddri, char *buf, size_t count)
	{
		struct i2c_client *client = bma220_i2c_client;
		char strbuf[BMA220_BUFSIZE];
		
		if(NULL == client)
		{
			GSE_ERR("i2c client is null!!\n");
			return 0;
		}
		bma220_init_client(client, 1);
		return snprintf(buf, PAGE_SIZE, "%s\n", strbuf);			
	}



/*----------------------------------------------------------------------------*/
static ssize_t show_sensordata_value(struct device_driver *ddri, char *buf)
{
	struct i2c_client *client = bma220_i2c_client;
	char strbuf[BMA220_BUFSIZE];
	
	if(NULL == client)
	{
		GSE_ERR("i2c client is null!!\n");
		return 0;
	}
	BMA220_ReadSensorData(client, strbuf, BMA220_BUFSIZE);
	//BMA220_ReadRawData(client, strbuf);
	return snprintf(buf, PAGE_SIZE, "%s\n", strbuf);            
}

static ssize_t show_sensorrawdata_value(struct device_driver *ddri, char *buf, size_t count)
	{
		struct i2c_client *client = bma220_i2c_client;
		char strbuf[BMA220_BUFSIZE];
		
		if(NULL == client)
		{
			GSE_ERR("i2c client is null!!\n");
			return 0;
		}
		//BMA220_ReadSensorData(client, strbuf, BMA220_BUFSIZE);
		BMA220_ReadRawData(client, strbuf);
		return snprintf(buf, PAGE_SIZE, "%s\n", strbuf);			
	}

/*----------------------------------------------------------------------------*/
static ssize_t show_cali_value(struct device_driver *ddri, char *buf)
{
	struct i2c_client *client = bma220_i2c_client;
	struct bma220_i2c_data *obj;
	int err, len = 0, mul;
	int tmp[BMA220_AXES_NUM];

	if(NULL == client)
	{
		GSE_ERR("i2c client is null!!\n");
		return 0;
	}

	obj = i2c_get_clientdata(client);



	if(err = BMA220_ReadOffset(client, obj->offset))
	{
		return -EINVAL;
	}
	else if(err = BMA220_ReadCalibration(client, tmp))
	{
		return -EINVAL;
	}
	else
	{    
		mul = obj->reso->sensitivity/bma220_offset_resolution.sensitivity;
		len += snprintf(buf+len, PAGE_SIZE-len, "[HW ][%d] (%+3d, %+3d, %+3d) : (0x%02X, 0x%02X, 0x%02X)\n", mul,                        
			obj->offset[BMA220_AXIS_X], obj->offset[BMA220_AXIS_Y], obj->offset[BMA220_AXIS_Z],
			obj->offset[BMA220_AXIS_X], obj->offset[BMA220_AXIS_Y], obj->offset[BMA220_AXIS_Z]);
		len += snprintf(buf+len, PAGE_SIZE-len, "[SW ][%d] (%+3d, %+3d, %+3d)\n", 1, 
			obj->cali_sw[BMA220_AXIS_X], obj->cali_sw[BMA220_AXIS_Y], obj->cali_sw[BMA220_AXIS_Z]);

		len += snprintf(buf+len, PAGE_SIZE-len, "[ALL]    (%+3d, %+3d, %+3d) : (%+3d, %+3d, %+3d)\n", 
			obj->offset[BMA220_AXIS_X]*mul + obj->cali_sw[BMA220_AXIS_X],
			obj->offset[BMA220_AXIS_Y]*mul + obj->cali_sw[BMA220_AXIS_Y],
			obj->offset[BMA220_AXIS_Z]*mul + obj->cali_sw[BMA220_AXIS_Z],
			tmp[BMA220_AXIS_X], tmp[BMA220_AXIS_Y], tmp[BMA220_AXIS_Z]);
		
		return len;
    }
}
/*----------------------------------------------------------------------------*/
static ssize_t store_cali_value(struct device_driver *ddri, char *buf, size_t count)
{
	struct i2c_client *client = bma220_i2c_client;  
	int err, x, y, z;
	int dat[BMA220_AXES_NUM];

	if(!strncmp(buf, "rst", 3))
	{
		if(err = BMA220_ResetCalibration(client))
		{
			GSE_ERR("reset offset err = %d\n", err);
		}	
	}
	else if(3 == sscanf(buf, "0x%02X 0x%02X 0x%02X", &x, &y, &z))
	{
		dat[BMA220_AXIS_X] = x;
		dat[BMA220_AXIS_Y] = y;
		dat[BMA220_AXIS_Z] = z;
		if(err = BMA220_WriteCalibration(client, dat))
		{
			GSE_ERR("write calibration err = %d\n", err);
		}		
	}
	else
	{
		GSE_ERR("invalid format\n");
	}
	
	return count;
}
/*----------------------------------------------------------------------------*/
static ssize_t show_self_value(struct device_driver *ddri, char *buf)
{
	struct i2c_client *client = bma220_i2c_client;
	struct bma220_i2c_data *obj;

	if(NULL == client)
	{
		GSE_ERR("i2c client is null!!\n");
		return 0;
	}

	//obj = i2c_get_clientdata(client);
	
    return snprintf(buf, 8, "%s\n", selftestRes);
}
/*----------------------------------------------------------------------------*/
static ssize_t store_self_value(struct device_driver *ddri, char *buf, size_t count)
{   /*write anything to this register will trigger the process*/
	struct item{
	s16 raw[BMA220_AXES_NUM];
	};
	
	struct i2c_client *client = bma220_i2c_client;  
	int idx, res, num;
	struct item *prv = NULL, *nxt = NULL;


	if(1 != sscanf(buf, "%d", &num))
	{
		GSE_ERR("parse number fail\n");
		return count;
	}
	else if(num == 0)
	{
		GSE_ERR("invalid data count\n");
		return count;
	}

	prv = kzalloc(sizeof(*prv) * num, GFP_KERNEL);
	nxt = kzalloc(sizeof(*nxt) * num, GFP_KERNEL);
	if (!prv || !nxt)
	{
		goto exit;
	}


	GSE_LOG("NORMAL:\n");
	BMA220_SetPowerMode(client,true); 

	/*initial setting for self test*/
	BMA220_InitSelfTest(client);
	GSE_LOG("SELFTEST:\n");    

	if(!BMA220_JudgeTestResult(client))
	{
		GSE_LOG("SELFTEST : PASS\n");
		strcpy(selftestRes,"y");
	}	
	else
	{
		GSE_LOG("SELFTEST : FAIL\n");		
		strcpy(selftestRes,"n");
	}
	
	exit:
	/*restore the setting*/    
	bma220_init_client(client, 0);
	kfree(prv);
	kfree(nxt);
	return count;
}
/*----------------------------------------------------------------------------*/
static ssize_t show_selftest_value(struct device_driver *ddri, char *buf)
{
	struct i2c_client *client = bma220_i2c_client;
	struct bma220_i2c_data *obj;

	if(NULL == client)
	{
		GSE_ERR("i2c client is null!!\n");
		return 0;
	}

	obj = i2c_get_clientdata(client);
	return snprintf(buf, PAGE_SIZE, "%d\n", atomic_read(&obj->selftest));
}
/*----------------------------------------------------------------------------*/
static ssize_t store_selftest_value(struct device_driver *ddri, char *buf, size_t count)
{
	struct bma220_i2c_data *obj = obj_i2c_data;
	int tmp;

	if(NULL == obj)
	{
		GSE_ERR("i2c data obj is null!!\n");
		return 0;
	}
	
	
	if(1 == sscanf(buf, "%d", &tmp))
	{        
		if(atomic_read(&obj->selftest) && !tmp)
		{
			/*enable -> disable*/
			bma220_init_client(obj->client, 0);
		}
		else if(!atomic_read(&obj->selftest) && tmp)
		{
			/*disable -> enable*/
			BMA220_InitSelfTest(obj->client);            
		}
		
		GSE_LOG("selftest: %d => %d\n", atomic_read(&obj->selftest), tmp);
		atomic_set(&obj->selftest, tmp); 
	}
	else
	{ 
		GSE_ERR("invalid content: '%s', length = %d\n", buf, count);   
	}
	return count;
}
/*----------------------------------------------------------------------------*/
static ssize_t show_firlen_value(struct device_driver *ddri, char *buf)
{
#ifdef CONFIG_BMA220_LOWPASS
	struct i2c_client *client = bma220_i2c_client;
	struct bma220_i2c_data *obj = i2c_get_clientdata(client);
	if(atomic_read(&obj->firlen))
	{
		int idx, len = atomic_read(&obj->firlen);
		GSE_LOG("len = %2d, idx = %2d\n", obj->fir.num, obj->fir.idx);

		for(idx = 0; idx < len; idx++)
		{
			GSE_LOG("[%5d %5d %5d]\n", obj->fir.raw[idx][BMA220_AXIS_X], obj->fir.raw[idx][BMA220_AXIS_Y], obj->fir.raw[idx][BMA220_AXIS_Z]);
		}
		
		GSE_LOG("sum = [%5d %5d %5d]\n", obj->fir.sum[BMA220_AXIS_X], obj->fir.sum[BMA220_AXIS_Y], obj->fir.sum[BMA220_AXIS_Z]);
		GSE_LOG("avg = [%5d %5d %5d]\n", obj->fir.sum[BMA220_AXIS_X]/len, obj->fir.sum[BMA220_AXIS_Y]/len, obj->fir.sum[BMA220_AXIS_Z]/len);
	}
	return snprintf(buf, PAGE_SIZE, "%d\n", atomic_read(&obj->firlen));
#else
	return snprintf(buf, PAGE_SIZE, "not support\n");
#endif
}
/*----------------------------------------------------------------------------*/
static ssize_t store_firlen_value(struct device_driver *ddri, char *buf, size_t count)
{
#ifdef CONFIG_BMA220_LOWPASS
	struct i2c_client *client = bma220_i2c_client;  
	struct bma220_i2c_data *obj = i2c_get_clientdata(client);
	int firlen;

	if(1 != sscanf(buf, "%d", &firlen))
	{
		GSE_ERR("invallid format\n");
	}
	else if(firlen > C_MAX_FIR_LENGTH)
	{
		GSE_ERR("exceeds maximum filter length\n");
	}
	else
	{ 
		atomic_set(&obj->firlen, firlen);
		if(NULL == firlen)
		{
			atomic_set(&obj->fir_en, 0);
		}
		else
		{
			memset(&obj->fir, 0x00, sizeof(obj->fir));
			atomic_set(&obj->fir_en, 1);
		}
	}
#endif    
	return count;
}
/*----------------------------------------------------------------------------*/
static ssize_t show_trace_value(struct device_driver *ddri, char *buf)
{
	ssize_t res;
	struct bma220_i2c_data *obj = obj_i2c_data;
	if (obj == NULL)
	{
		GSE_ERR("i2c_data obj is null!!\n");
		return 0;
	}
	
	res = snprintf(buf, PAGE_SIZE, "0x%04X\n", atomic_read(&obj->trace));     
	return res;    
}
/*----------------------------------------------------------------------------*/
static ssize_t store_trace_value(struct device_driver *ddri, char *buf, size_t count)
{
	struct bma220_i2c_data *obj = obj_i2c_data;
	int trace;
	if (obj == NULL)
	{
		GSE_ERR("i2c_data obj is null!!\n");
		return 0;
	}
	
	if(1 == sscanf(buf, "0x%x", &trace))
	{
		atomic_set(&obj->trace, trace);
	}	
	else
	{
		GSE_ERR("invalid content: '%s', length = %d\n", buf, count);
	}
	
	return count;    
}
/*----------------------------------------------------------------------------*/
static ssize_t show_status_value(struct device_driver *ddri, char *buf)
{
	ssize_t len = 0;    
	struct bma220_i2c_data *obj = obj_i2c_data;
	if (obj == NULL)
	{
		GSE_ERR("i2c_data obj is null!!\n");
		return 0;
	}	
	
	if(obj->hw)
	{
		len += snprintf(buf+len, PAGE_SIZE-len, "CUST: %d %d (%d %d)\n", 
	            obj->hw->i2c_num, obj->hw->direction, obj->hw->power_id, obj->hw->power_vol);   
	}
	else
	{
		len += snprintf(buf+len, PAGE_SIZE-len, "CUST: NULL\n");
	}
	return len;    
}
/*----------------------------------------------------------------------------*/
static ssize_t show_power_status_value(struct device_driver *ddri, char *buf)
{
	if(sensor_power)
		printk("G sensor is in work mode, sensor_power = %d\n", sensor_power);
	else
		printk("G sensor is in standby mode, sensor_power = %d\n", sensor_power);

	return 0;
}
/*----------------------------------------------------------------------------*/
static DRIVER_ATTR(chipinfo,   S_IWUSR | S_IRUGO, show_chipinfo_value,      NULL);
//static DRIVER_ATTR(cpsdata, S_IWUSR | S_IRUGO, show_cpsdata_value,    NULL);
//static DRIVER_ATTR(cpsopmode, S_IRUGO|S_IWUSR, show_cpsopmode_value,    store_cpsopmode_value);
//static DRIVER_ATTR(cpsrange, S_IRUGO|S_IWUSR, show_cpsrange_value,    store_cpsrange_value);
//static DRIVER_ATTR(cpsbandwidth, S_IRUGO|S_IWUSR, show_cpsbandwidth_value,    store_cpsbandwidth_value);
static DRIVER_ATTR(sensordata, S_IWUSR | S_IRUGO, show_sensordata_value,    NULL);
static DRIVER_ATTR(cali,       S_IWUSR | S_IRUGO, show_cali_value,          store_cali_value);
static DRIVER_ATTR(selftest, S_IWUSR | S_IRUGO, show_self_value,  store_self_value);
static DRIVER_ATTR(self,   S_IWUSR | S_IRUGO, show_selftest_value,      store_selftest_value);
static DRIVER_ATTR(firlen,     S_IWUSR | S_IRUGO, show_firlen_value,        store_firlen_value);
static DRIVER_ATTR(trace,      S_IWUSR | S_IRUGO, show_trace_value,         store_trace_value);
static DRIVER_ATTR(status,               S_IRUGO, show_status_value,        NULL);
static DRIVER_ATTR(powerstatus,               S_IRUGO, show_power_status_value,        NULL);

/*----------------------------------------------------------------------------*/
static struct driver_attribute *bma220_attr_list[] = {
	&driver_attr_chipinfo,     /*chip information*/
	&driver_attr_sensordata,   /*dump sensor data*/
	&driver_attr_cali,         /*show calibration data*/
	&driver_attr_self,         /*self test demo*/
	&driver_attr_selftest,     /*self control: 0: disable, 1: enable*/
	&driver_attr_firlen,       /*filter length: 0: disable, others: enable*/
	&driver_attr_trace,        /*trace log*/
	&driver_attr_status,
	&driver_attr_powerstatus,
	//&driver_attr_cpsdata,	/*g sensor data for compass tilt compensation*/
	//&driver_attr_cpsopmode,	/*g sensor opmode for compass tilt compensation*/
	//&driver_attr_cpsrange,	/*g sensor range for compass tilt compensation*/
	//&driver_attr_cpsbandwidth,	/*g sensor bandwidth for compass tilt compensation*/
};
/*----------------------------------------------------------------------------*/
static int bma220_create_attr(struct device_driver *driver) 
{
	int idx, err = 0;
	int num = (int)(sizeof(bma220_attr_list)/sizeof(bma220_attr_list[0]));
	if (driver == NULL)
	{
		return -EINVAL;
	}

	for(idx = 0; idx < num; idx++)
	{
		if(err = driver_create_file(driver, bma220_attr_list[idx]))
		{            
			GSE_ERR("driver_create_file (%s) = %d\n", bma220_attr_list[idx]->attr.name, err);
			break;
		}
	}    
	return err;
}
/*----------------------------------------------------------------------------*/
static int bma220_delete_attr(struct device_driver *driver)
{
	int idx ,err = 0;
	int num = (int)(sizeof(bma220_attr_list)/sizeof(bma220_attr_list[0]));

	if(driver == NULL)
	{
		return -EINVAL;
	}
	

	for(idx = 0; idx < num; idx++)
	{
		driver_remove_file(driver, bma220_attr_list[idx]);
	}
	

	return err;
}

/*----------------------------------------------------------------------------*/
int gsensor_operate(void* self, uint32_t command, void* buff_in, int size_in,
		void* buff_out, int size_out, int* actualout)
{
	int err = 0;
	int value, sample_delay;	
	struct bma220_i2c_data *priv = (struct bma220_i2c_data*)self;
	hwm_sensor_data* gsensor_data;
	char buff[BMA220_BUFSIZE];
	
	//GSE_FUN(f);
	switch (command)
	{
		case SENSOR_DELAY:
			if((buff_in == NULL) || (size_in < sizeof(int)))
			{
				GSE_ERR("Set delay parameter error!\n");
				err = -EINVAL;
			}
			else
			{
				value = *(int *)buff_in;
				if(value <= 4)
				{
					sample_delay = BMA220_BW_250HZ;
				}
				else if(value <= 8)
				{
					sample_delay = BMA220_BW_125HZ;
				}
				else
				{
					sample_delay = BMA220_BW_64HZ;
				}
				
				err = BMA220_SetBWRate(priv->client, sample_delay);
				if(err != BMA220_SUCCESS ) //0x2C->BW=100Hz
				{
					GSE_ERR("Set delay parameter error!\n");
				}

				if(value >= 50)
				{
					atomic_set(&priv->filter, 0);
				}
				else
				{	
				#if defined(CONFIG_BMA220_LOWPASS)
					priv->fir.num = 0;
					priv->fir.idx = 0;
					priv->fir.sum[BMA220_AXIS_X] = 0;
					priv->fir.sum[BMA220_AXIS_Y] = 0;
					priv->fir.sum[BMA220_AXIS_Z] = 0;
					atomic_set(&priv->filter, 1);
				#endif
				}
			}
			break;

		case SENSOR_ENABLE:
			if((buff_in == NULL) || (size_in < sizeof(int)))
			{
				GSE_ERR("Enable sensor parameter error!\n");
				err = -EINVAL;
			}
			else
			{
				value = *(int *)buff_in;
				if(((value == 0) && (sensor_power == false)) ||((value == 1) && (sensor_power == true)))
				{
					GSE_LOG("Gsensor device have updated!\n");
				}
				else
				{
					err = BMA220_SetPowerMode( priv->client, !sensor_power);
				}
			}
			break;

		case SENSOR_GET_DATA:
			if((buff_out == NULL) || (size_out< sizeof(hwm_sensor_data)))
			{
				GSE_ERR("get sensor data parameter error!\n");
				err = -EINVAL;
			}
			else
			{
				gsensor_data = (hwm_sensor_data *)buff_out;
				BMA220_ReadSensorData(priv->client, buff, BMA220_BUFSIZE);
				sscanf(buff, "%x %x %x", &gsensor_data->values[0], 
					&gsensor_data->values[1], &gsensor_data->values[2]);				
				gsensor_data->status = SENSOR_STATUS_ACCURACY_MEDIUM;				
				gsensor_data->value_divide = 1000;
			}
			break;
		default:
			GSE_ERR("gsensor operate function no this parameter %d!\n", command);
			err = -1;
			break;
	}
	
	return err;
}

/****************************************************************************** 
 * Function Configuration
******************************************************************************/
static int bma220_open(struct inode *inode, struct file *file)
{
	file->private_data = bma220_i2c_client;

	if(file->private_data == NULL)
	{
		GSE_ERR("null pointer!!\n");
		return -EINVAL;
	}
	return nonseekable_open(inode, file);
}
/*----------------------------------------------------------------------------*/
static int bma220_release(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	return 0;
}
/*----------------------------------------------------------------------------*/
//static int bma220_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
static int bma220_unlocked_ioctl(struct file *file, unsigned int cmd,
       unsigned long arg)
{
	struct i2c_client *client = (struct i2c_client*)file->private_data;
	struct bma220_i2c_data *obj = (struct bma220_i2c_data*)i2c_get_clientdata(client);	
	char strbuf[BMA220_BUFSIZE];
	void __user *data;
	SENSOR_DATA sensor_data;
	int err = 0;
	int cali[3];
	signed char data_temp =0; 
	
	//printk("MYCAT bma220_unlocked_ioctl\n");

	//GSE_FUN(f);
	if(_IOC_DIR(cmd) & _IOC_READ)
	{
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	}
	else if(_IOC_DIR(cmd) & _IOC_WRITE)
	{
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	}

	if(err)
	{
		GSE_ERR("access error: %08X, (%2d, %2d)\n", cmd, _IOC_DIR(cmd), _IOC_SIZE(cmd));
		return -EFAULT;
	}

	switch(cmd)
	{
		case GSENSOR_IOCTL_INIT:
			bma220_init_client(client, 0);			
			break;

		case GSENSOR_IOCTL_READ_CHIPINFO:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;	  
			}
			
			BMA220_ReadChipInfo(client, strbuf, BMA220_BUFSIZE);
			if(copy_to_user(data, strbuf, strlen(strbuf)+1))
			{
				err = -EFAULT;
				break;
			}				 
			break;	  

		case GSENSOR_IOCTL_READ_SENSORDATA:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;	  
			}
			
			BMA220_ReadSensorData(client, strbuf, BMA220_BUFSIZE);
			if(copy_to_user(data, strbuf, strlen(strbuf)+1))
			{
				err = -EFAULT;
				break;	  
			}				 
			break;

		case GSENSOR_IOCTL_READ_GAIN:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;	  
			}			
			
			if(copy_to_user(data, &gsensor_gain, sizeof(GSENSOR_VECTOR3D)))
			{
				err = -EFAULT;
				break;
			}				 
			break;

		case GSENSOR_IOCTL_READ_RAW_DATA:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;	  
			}
			BMA220_ReadRawData(client, strbuf);
			if(copy_to_user(data, &strbuf, strlen(strbuf)+1))
			{
				err = -EFAULT;
				break;	  
			}
			break;	  

		case GSENSOR_IOCTL_SET_CALI:
			data = (void __user*)arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;	  
			}
			if(copy_from_user(&sensor_data, data, sizeof(sensor_data)))
			{
				err = -EFAULT;
				break;	  
			}
			if(atomic_read(&obj->suspend))
			{
				GSE_ERR("Perform calibration in suspend state!!\n");
				err = -EINVAL;
			}
			else
			{
				cali[BMA220_AXIS_X] = sensor_data.x * obj->reso->sensitivity / GRAVITY_EARTH_1000;
				cali[BMA220_AXIS_Y] = sensor_data.y * obj->reso->sensitivity / GRAVITY_EARTH_1000;
				cali[BMA220_AXIS_Z] = sensor_data.z * obj->reso->sensitivity / GRAVITY_EARTH_1000;			  
				err = BMA220_WriteCalibration(client, cali);			 
			}
			break;

		case GSENSOR_IOCTL_CLR_CALI:
			err = BMA220_ResetCalibration(client);
			break;

		case GSENSOR_IOCTL_GET_CALI:
			data = (void __user*)arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;	  
			}
			if(err = BMA220_ReadCalibration(client, cali))
			{
				break;
			}
			
			sensor_data.x = cali[BMA220_AXIS_X] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
			sensor_data.y = cali[BMA220_AXIS_Y] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
			sensor_data.z = cali[BMA220_AXIS_Z] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
			if(copy_to_user(data, &sensor_data, sizeof(sensor_data)))
			{
				err = -EFAULT;
				break;
			}		
			break;
	
		/*case GSENSOR_IOCTL_TEST:							//modify by wangwei for auto test
			
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;	  
			}
			
			//add by huangrunming 20120907
			#ifndef FACTORY_AUTOTEST_GSENSOR_VALUE
			    err = BMA220_ReadSensorData(client, strbuf, BMA220_BUFSIZE);
			#else
			    err = BMA220_ReadSensorData_factory_autotest(client, strbuf, BMA220_BUFSIZE);
			#endif

	
	
			if (err < 0) 
			{

				data_temp = 1;
			}
			else
			{
				data_temp = 2;
			}	
			
			if(copy_to_user((unsigned char*)arg,&data_temp,1)!=0)
			{
				printk("copy_to error\n");
				return -EFAULT;
			}
			break;
			*/
			/*
		case GSENSOR_IOCTL_SET_CALIBRATION:
						
			err = BMA220_SET_CALIBRATION(client);

			break;
			*/
		default:
			GSE_ERR("unknown IOCTL: 0x%08x\n", cmd);
			err = -ENOIOCTLCMD;
			break;
			
	}

	return err;
}


/*----------------------------------------------------------------------------*/
static struct file_operations bma220_fops = {
	//.owner = THIS_MODULE,
	.open = bma220_open,
	.release = bma220_release,
	.unlocked_ioctl = bma220_unlocked_ioctl,
};
/*----------------------------------------------------------------------------*/
static struct miscdevice bma220_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "gsensor",
	.fops = &bma220_fops,
};
/*----------------------------------------------------------------------------*/
#ifndef CONFIG_HAS_EARLYSUSPEND
/*----------------------------------------------------------------------------*/
static int bma220_suspend(struct i2c_client *client, pm_message_t msg) 
{
	struct bma220_i2c_data *obj = i2c_get_clientdata(client);    
	int err = 0;
	GSE_FUN();    

	if(msg.event == PM_EVENT_SUSPEND)
	{   
		if(obj == NULL)
		{
			GSE_ERR("null pointer!!\n");
			return -EINVAL;
		}
		atomic_set(&obj->suspend, 1);
		if(err = BMA220_SetPowerMode(obj->client, false))
		{
			GSE_ERR("write power control fail!!\n");
			return;
		}       
		BMA220_power(obj->hw, 0);
	}
	return err;
}
/*----------------------------------------------------------------------------*/
static int bma220_resume(struct i2c_client *client)
{
	struct bma220_i2c_data *obj = i2c_get_clientdata(client);        
	int err;
	GSE_FUN();

	if(obj == NULL)
	{
		GSE_ERR("null pointer!!\n");
		return -EINVAL;
	}

	BMA220_power(obj->hw, 1);
	if(err = bma220_init_client(client, 0))
	{
		GSE_ERR("initialize client fail!!\n");
		return err;        
	}
	atomic_set(&obj->suspend, 0);

	return 0;
}
/*----------------------------------------------------------------------------*/
#else /*CONFIG_HAS_EARLY_SUSPEND is defined*/
/*----------------------------------------------------------------------------*/
static void bma220_early_suspend(struct early_suspend *h) 
{
	struct bma220_i2c_data *obj = container_of(h, struct bma220_i2c_data, early_drv);   
	int err;
	GSE_FUN();    

	if(obj == NULL)
	{
		GSE_ERR("null pointer!!\n");
		return;
	}
	atomic_set(&obj->suspend, 1); 
	if(err = BMA220_SetPowerMode(obj->client, false))
	{
		GSE_ERR("write power control fail!!\n");
		return;
	}
	sensor_power = false;
	
	BMA220_power(obj->hw, 0);
}
/*----------------------------------------------------------------------------*/
static void bma220_late_resume(struct early_suspend *h)
{
	struct bma220_i2c_data *obj = container_of(h, struct bma220_i2c_data, early_drv);         
	int err;
	GSE_FUN();

	if(obj == NULL)
	{
		GSE_ERR("null pointer!!\n");
		return;
	}
    BMA220_power(obj->hw, 1);
	if(err = BMA220_SetPowerMode(obj->client, true))
	{
		GSE_ERR("write power control fail!!\n");
		return;
	}	

	atomic_set(&obj->suspend, 0);    
}
/*----------------------------------------------------------------------------*/
#endif /*CONFIG_HAS_EARLYSUSPEND*/
/*----------------------------------------------------------------------------*/
static int bma220_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info) 
{    
//printk("MYCAT bma220_i2c_detect\n");
	strcpy(info->type, "BMA220");
	return 0;
}

/*----------------------------------------------------------------------------*/
static int bma220_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct i2c_client *new_client;
	struct bma220_i2c_data *obj;
	struct hwmsen_object sobj;
	int err = 0;
	GSE_FUN();
	//printk("MYCAT bma220_i2c_probe00\n");

	//hwPowerOn(MT65XX_POWER_LDO_VGP2, VOL_2800, "TP");
	mdelay(100);
	
	if(!(obj = kzalloc(sizeof(*obj), GFP_KERNEL)))
	{
		err = -ENOMEM;
		goto exit;
	}
	
	memset(obj, 0, sizeof(struct bma220_i2c_data));

	obj->hw = bma220_get_cust_acc_hw();
	//printk("MYCAT bma220_i2c_probe01\n");
	if(err = hwmsen_get_convert(obj->hw->direction, &obj->cvt))
	{
		GSE_ERR("invalid direction: %d\n", obj->hw->direction);
		goto exit;
	}
	//printk("MYCAT bma220_i2c_probe02\n");

	obj_i2c_data = obj;
	obj->client = client;
	new_client = obj->client;
	i2c_set_clientdata(new_client,obj);
	
	atomic_set(&obj->trace, 0);
	atomic_set(&obj->suspend, 0);
	
#ifdef CONFIG_BMA220_LOWPASS
	if(obj->hw->firlen > C_MAX_FIR_LENGTH)
	{
		atomic_set(&obj->firlen, C_MAX_FIR_LENGTH);
	}	
	else
	{
		atomic_set(&obj->firlen, obj->hw->firlen);
	}
	
	if(atomic_read(&obj->firlen) > 0)
	{
		atomic_set(&obj->fir_en, 1);
	}
	
#endif

	bma220_i2c_client = new_client;	
	//printk("MYCAT bma220_i2c_probe03\n");

	if(err = bma220_init_client(new_client, 1))
	{
		goto exit_init_failed;
	}
	
	//printk("MYCAT bma220_i2c_probe04\n");

	if(err = misc_register(&bma220_device))
	{
		GSE_ERR("bma220_device register failed\n");
		goto exit_misc_device_register_failed;
	}
	//printk("MYCAT bma220_i2c_probe05\n");
#if !defined(MTK_AUTO_DETECT_ACCELEROMETER)
	if(err = bma220_create_attr(&bma220_gsensor_driver.driver))
	{
		GSE_ERR("create attribute err = %d\n", err);
		goto exit_create_attr_failed;
	}
	//printk("MYCAT bma220_i2c_probe06\n");
#else
	if((err = bma220_create_attr(&bma220_init_info.platform_diver_addr->driver)))
	{
		GSE_ERR("create attribute err = %d\n", err);
		goto exit_create_attr_failed;
	}
#endif

	sobj.self = obj;
    sobj.polling = 1;
    sobj.sensor_operate = gsensor_operate;
	if(err = hwmsen_attach(ID_ACCELEROMETER, &sobj))
	{
		GSE_ERR("attach fail = %d\n", err);
		goto exit_kfree;
	}
	//printk("MYCAT bma220_i2c_probe07\n");

#ifdef CONFIG_HAS_EARLYSUSPEND
	obj->early_drv.level    = EARLY_SUSPEND_LEVEL_DISABLE_FB - 1,
	obj->early_drv.suspend  = bma220_early_suspend,
	obj->early_drv.resume   = bma220_late_resume,    
	register_early_suspend(&obj->early_drv);
#endif 

#if defined(MTK_AUTO_DETECT_ACCELEROMETER)
	/* Add for auto detect feature */
	bma220_init_flag = 0;
#endif
	GSE_LOG("%s: OK\n", __func__);    
	return 0;

	exit_create_attr_failed:
	misc_deregister(&bma220_device);
	exit_misc_device_register_failed:
	exit_init_failed:
	//i2c_detach_client(new_client);
	printk("MYCAT bma220_i2c_probe fail\n");
	exit_kfree:
	kfree(obj);
	exit:
	GSE_ERR("%s: err = %d\n", __func__, err);        
#if defined(MTK_AUTO_DETECT_ACCELEROMETER)
	/* Add for auto detect feature */
	bma220_init_flag = -1;
#endif  
	return err;
}

/*----------------------------------------------------------------------------*/
static int bma220_i2c_remove(struct i2c_client *client)
{
	int err = 0;	
	//printk("MYCAT bma220_i2c_remove\n");
#if !defined(MTK_AUTO_DETECT_ACCELEROMETER)		
	if(err = bma220_delete_attr(&bma220_gsensor_driver.driver))
	{
		GSE_ERR("bma220_delete_attr fail: %d\n", err);
	}
#else
	if((err = bma220_delete_attr(&bma220_init_info.platform_diver_addr->driver)))
	{
		GSE_ERR("bma220_delete_attr fail: %d\n", err);
	} 
#endif	
	
	if(err = misc_deregister(&bma220_device))
	{
		GSE_ERR("misc_deregister fail: %d\n", err);
	}

	if(err = hwmsen_detach(ID_ACCELEROMETER))
	    

	bma220_i2c_client = NULL;
	i2c_unregister_device(client);
	kfree(i2c_get_clientdata(client));
	return 0;
}
#if !defined(MTK_AUTO_DETECT_ACCELEROMETER)
/*----------------------------------------------------------------------------*/
static int bma220_probe(struct platform_device *pdev) 
{
	struct acc_hw *hw = bma220_get_cust_acc_hw();
	GSE_FUN();
	//printk("MYCAT bma220_probe\n");

	BMA220_power(hw, 1);
	//bma220_force[0] = hw->i2c_num;
	if(i2c_add_driver(&bma220_i2c_driver))
	{
		printk("MYCAT add driver error\n");
		return -1;
	}
	return 0;
}
/*----------------------------------------------------------------------------*/
static int bma220_remove(struct platform_device *pdev)
{
    struct acc_hw *hw = bma220_get_cust_acc_hw();
//printk("MYCAT bma220_remove\n");
    GSE_FUN();    
    BMA220_power(hw, 0);    
    i2c_del_driver(&bma220_i2c_driver);
    return 0;
}
/*----------------------------------------------------------------------------*/
static struct platform_driver bma220_gsensor_driver = {
	.probe      = bma220_probe,
	.remove     = bma220_remove,    
	.driver     = {
		.name  = "gsensor",
		.owner = THIS_MODULE,
	}
};
#else
static int  bma220_local_init(void)
{
	struct acc_hw *hw = bma220_get_cust_acc_hw();
	GSE_FUN();
//printk("MYCAT bma220_local_init\n");
	BMA220_power(hw, 1);
	//bma220_force[0] = hw->i2c_num;
	if(i2c_add_driver(&bma220_i2c_driver))
	{
		printk("MYCAT add driver error\n");
		return -1;
	}

	if(-1 == bma220_init_flag)	
	{	   
		return -1;	
	}
	return 0;
}

static int bma220_remove(void)
{
	struct acc_hw *hw = bma220_get_cust_acc_hw();
//printk("MYCAT bma220_remove\n");
    GSE_FUN();    
    BMA220_power(hw, 0);    
    i2c_del_driver(&bma220_i2c_driver);
    return 0;
}

#endif

/*----------------------------------------------------------------------------*/
static int __init bma220_init(void)
{
	GSE_FUN();
	//printk("MYCAT bma220_init\n");

#if !defined(MTK_AUTO_DETECT_ACCELEROMETER)
	struct acc_hw *hw = bma220_get_cust_acc_hw();
	
	i2c_register_board_info(hw->i2c_num, &i2c_bma220, 1);

	if(platform_driver_register(&bma220_gsensor_driver))
	{
		printk("MYCAT failed to register driver");
		return -ENODEV;
	}
#else
	struct acc_hw *hw = bma220_get_cust_acc_hw();
	//aa+bb=cc;
	i2c_register_board_info(hw->i2c_num, &i2c_bma220, 1);
	hwmsen_gsensor_add(&bma220_init_info);
#endif
	return 0;    
}
/*----------------------------------------------------------------------------*/
static void __exit bma220_exit(void)
{
	GSE_FUN();
#if !defined(MTK_AUTO_DETECT_ACCELEROMETER)
	platform_driver_unregister(&bma220_gsensor_driver);
#endif
}
/*----------------------------------------------------------------------------*/
module_init(bma220_init);
module_exit(bma220_exit);
/*----------------------------------------------------------------------------*/
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("BMA220 I2C driver");
MODULE_AUTHOR("Dexiang.Liu@mediatek.com");