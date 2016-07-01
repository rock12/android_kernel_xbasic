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
 
#include <linux/interrupt.h>
#include <cust_eint.h>
#include <linux/i2c.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/rtpm_prio.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/time.h>
 
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif 

/*ergate-013*/
#include <linux/dma-mapping.h>

#include "tpd_custom_msg21xx.h"
#include "tpd.h"

/*ergate-001*/
#include "cust_gpio_usage.h"

/*ergate-037*/
#include <linux/types.h>

#ifdef MT6575
#include <mach/mt6575_pm_ldo.h>
#include <mach/mt6575_typedefs.h>
#include <mach/mt6575_boot.h>
#endif


#include <mach/mt_pm_ldo.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_boot.h>
#include <mach/mt_gpio.h>
#ifdef TPD_PROXIMITY
#include <linux/hwmsensor.h>
#include <linux/hwmsen_dev.h>
#include <linux/sensors_io.h>
#endif
#if defined(HQ_PROJECT_A512P_SOP) || defined(HQ_PROJECT_A512P_TETC)
#define HQ_MSG2133_PS_SUPPORT
#endif

//zym 2011-12-26 for lcm_resume_in_lcd

#if defined(HQ_TP_IMPROVE)
#include "lcm_drv.h"
extern LCM_DRIVER  *lcm_drv;
extern unsigned int tpd_button_triggered;
#endif
/*ergate-016 start*/
//#undef GPIO_CAPTOUCH_EINT_PIN
//#undef GPIO_CAPTOUCH_EINT_PIN_M_EINT
//#undef CUST_EINT_TOUCH_PANEL_NUM
//#undef GPIO_CTP_EN_PIN
//static int GPIO_CAPTOUCH_EINT_PIN,GPIO_CAPTOUCH_EINT_PIN_M_EINT,CUST_EINT_TOUCH_PANEL_NUM;
//static int GPIO_CTP_EN_PIN;
/*ergate-016 end*/

 
extern struct tpd_device *tpd;
 
static struct i2c_client *i2c_client = NULL;
static struct task_struct *thread = NULL;
 
static DECLARE_WAIT_QUEUE_HEAD(waiter);
 
static struct early_suspend early_suspend;
 
#ifdef CONFIG_HAS_EARLYSUSPEND
static void tpd_early_suspend(struct early_suspend *handler);
static void tpd_late_resume(struct early_suspend *handler);
#endif 
 


	extern void mt65xx_eint_unmask(unsigned int line);
	extern void mt65xx_eint_mask(unsigned int line);
	extern void mt65xx_eint_set_polarity(unsigned int eint_num, unsigned int pol);
	extern void mt65xx_eint_set_hw_debounce(unsigned int eint_num, unsigned int ms);
	extern unsigned int mt65xx_eint_set_sens(unsigned int eint_num, unsigned int sens);
	extern void mt65xx_eint_registration(unsigned int eint_num, unsigned int is_deb_en, unsigned int pol, void (EINT_FUNC_PTR)(void), unsigned int is_auto_umask);


 
static void tpd_eint_interrupt_handler(void);
static int tpd_get_bl_info(int show);
static int __devinit tpd_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int tpd_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);
static int __devexit tpd_remove(struct i2c_client *client);
static int touch_event_handler(void *unused);
static int tpd_initialize(struct i2c_client * client);

extern int tpd_firmware_version[2];

#if defined(HQ_TP_IMPROVE)
static int pre_count =0;
static int pre_x1 =0;
static int pre_y1 =0;
static int pre_x2 =0;
static int pre_y2 =0;
#endif

#define TPD_OK 0

#define TPD_PACKET_HEAD_CMD    0x54
#define TPD_PACKET_HEAD_RSP    0x52

#define TPD_PACKET_CMD_VER    0x10

#define TPD_PACKET_CMD_MODE_READ    0x20
#define TPD_PACKET_CMD_MODE_WRITE   0x21
#define TPD_MODE_ACTIVE             0x10
#define TPD_MODE_FAST               0x20
#define TPD_MODE_FREEZE             0x90

#define TPD_PACKET_CMD_FORMAT_READ    0x60
#define TPD_PACKET_CMD_FORMAT_WRITE   0x61
#define TPD_PACKET_FORMAT_1           0x00
#define TPD_PACKET_FORMAT_2           0x01
#define TPD_PACKET_FORMAT_3           0x03

/*ergate-012 start*/
#ifdef TPD_HAVE_BUTTON 
static int tpd_keys_local[TPD_KEY_COUNT] = TPD_KEYS;
static int tpd_keys_dim_local[TPD_KEY_COUNT][4] = TPD_KEYS_DIM;
#endif
/*ergate-012 end*/

struct TpdPacketT
{
    U8 head;
    U8 command;
    U8 data_1;
    U8 data_2;
};

//////////////////////////////////////////////////////////////////////////////add by lan
#ifdef TPD_PROXIMITY
#define APS_ERR(fmt,arg...)           	printk("<<proximity>> "fmt"\n",##arg)
#define TPD_PROXIMITY_DEBUG(fmt,arg...) printk("<<proximity>> "fmt"\n",##arg)
#define TPD_PROXIMITY_DMESG(fmt,arg...) printk("<<proximity>> "fmt"\n",##arg)

static u8 tpd_proximity_flag 			= 0;
static u8 tpd_proximity_flag_one 		= 0; //add for tpd_proximity by wangdongfang
static u8 tpd_proximity_detect 			= 1;//0-->close ; 1--> far away

enum
{
    DISABLE_CTP_PS,
    ENABLE_CTP_PS,
    RESET_CTP_PS
};
//struct class *firmware_class;
//struct device *firmware_cmd_dev;
#endif

struct TpdTouchDataT
{
    U8 packet_id;
    U8 x_h_y_h;
    U8 x_l;
    U8 y_l;
    U8 disx_h_disy_h;
    U8 disx_l;
    U8 disy_l;
    U8 checksum;
};

struct TouchInfoT
{
    int x1, y1;
    int x2, y2;
    int pressure;
    int count;
#ifdef TPD_HAVE_BUTTON 
    int key_id;
    int key_value;
#endif
};

//static u8 *tempData = NULL;
static int tpd_flag = 0;

static struct TpdTouchDataT TpdTouchData;
 
static const struct i2c_device_id tpd_id[] = {{"msg2133",0},{}};
/*ergate-001*/
static unsigned short force[] = {0,0x4c,I2C_CLIENT_END,I2C_CLIENT_END};
static const unsigned short * const forces[] = { force, NULL };
//static struct i2c_client_address_data addr_data = { .forces = forces, };
static struct i2c_board_info __initdata i2c_tpd={ I2C_BOARD_INFO("msg2133", (0x4c>>1))};
static struct i2c_driver tpd_driver = 
{
    .driver = 
    {
        .name = "msg2133",
        .owner = THIS_MODULE,
    },
    .probe = tpd_probe,
    .remove = __devexit_p(tpd_remove),
    .id_table = tpd_id,
    .detect = tpd_detect,
   // .address_list = (const unsigned short*) forces,
};

#if 0

/*FW UPDATE*/
#define DBG printk
#define FW_ADDR_MSG2133   (0xc4>>1) //device address of msg2133
#define FW_UPDATE_ADDR_MSG2133   (0x92>>1)
static U8 temp[94][1024];   //
static int FwDataCnt;
//static int FwVersion;
struct class *firmware_class;
struct device *firmware_cmd_dev;
static  char *fw_version;

#endif
/*ergate-026 start*/
static int log_level = 2; //0-2(verbose)
#define TPD_LOGI(fmt, arg...)    if(log_level >= 1) \
        printk(KERN_INFO fmt, ##arg)

#define TPD_LOGV(fmt, arg...)    if(log_level >= 2) \
        printk(KERN_INFO fmt, ##arg)


static ssize_t show_loglevel(struct device_driver *ddri, char *buf);
static ssize_t store_loglevel(struct device_driver *ddri, char *buf, size_t count);
static DRIVER_ATTR(loglevel,   S_IWUSR | S_IRUGO, show_loglevel,      store_loglevel);
//extern int tpd_create_attr(struct driver_attribute *attr); 

static ssize_t show_loglevel(struct device_driver *ddri, char *buf)
{
    snprintf(buf,PAGE_SIZE,"level:%d\nrange:0-2(verbose)\n",log_level);
}


static ssize_t store_loglevel(struct device_driver *ddri, char *buf, size_t count)
{
    sscanf(buf,"%d",&log_level);
    return count;
}
/*ergate-026 end*/

////////////////////////////////////////////////update   




#define __FIRMWARE_UPDATE__
#ifdef __FIRMWARE_UPDATE__

/*adair:0777为打开apk升级功能，0664为关闭apk升级功能，无需将宏__FIRMWARE_UPDATE__关闭*/
#define CTP_AUTHORITY 0777//0664

//#define ENABLE_AUTO_UPDATA
#if 0
#define TP_DEBUG(format, ...)	printk(KERN_INFO "MSG2133_MSG21XXA_update_INFO***" format "\n", ## __VA_ARGS__)
#else
#define TP_DEBUG(format, ...)
#endif
#if 0//adair:正式版本关闭
#define TP_DEBUG_ERR(format, ...)	printk(KERN_ERR "MSG2133_MSG21XXA_update_ERR***" format "\n", ## __VA_ARGS__)
#else
#define TP_DEBUG_ERR(format, ...)
#endif
static  char *fw_version;
static u8 temp[94][1024];
//u8  Fmr_Loader[1024];
u32 crc_tab[256];
static u8 g_dwiic_info_data[1024];   // Buffer for info data

static int FwDataCnt;
struct class *firmware_class;
struct device *firmware_cmd_dev;

#define N_BYTE_PER_TIME (8)//adair:1024的约数,根据平台修改
#define UPDATE_TIMES (1024/N_BYTE_PER_TIME)

#if 1//adair:根据平台不同选择不同位的i2c地址
#define FW_ADDR_MSG21XX   (0xC4>>1)
#define FW_ADDR_MSG21XX_TP   (0x4C>>1)
#define FW_UPDATE_ADDR_MSG21XX   (0x92>>1)
#else
#define FW_ADDR_MSG21XX   (0xC4>>1)
#define FW_ADDR_MSG21XX_TP   (0x4C>>1)
#define FW_UPDATE_ADDR_MSG21XX   (0x92>>1)
#endif

/*adair:以下5个以Hal开头的函数需要根据平台修改*/
/*disable irq*/
static void HalDisableIrq(void)
{
	mt65xx_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);
    mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY, NULL, 1);
}
/*enable irq*/
static void HalEnableIrq(void)
{
    mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY, tpd_eint_interrupt_handler, 1);
	mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);
}
/*reset the chip*/
static void _HalTscrHWReset(void)
{
/*	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_EN_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, CTP_EN_DISABLE);
    msleep(120);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, Ctp_enable_level);
	msleep(300);
*/
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
	msleep(10);
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);  
	msleep(50);
	TPD_DMESG(" msg2133 reset\n");
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
	msleep(150);
}
static void HalTscrCReadI2CSeq(u8 addr, u8* read_data, u16 size)
{
    int ret;
    i2c_client->addr = addr;
    ret = i2c_master_recv(i2c_client, read_data, size);
    i2c_client->addr = FW_ADDR_MSG21XX_TP;
    
    if(ret <= 0)
    {
		TP_DEBUG_ERR("HalTscrCReadI2CSeq error %d,addr = %d\n", ret,addr);
	}
}

static void i2c_write(u8 addr, u8 *pbt_buf, int dw_lenth)
{
	int ret;
	i2c_client->addr = addr;
	i2c_client->addr|=I2C_ENEXT_FLAG;
	ret = i2c_master_send(i2c_client, pbt_buf, dw_lenth);
	i2c_client->addr = FW_ADDR_MSG21XX_TP;
	i2c_client->addr|=I2C_ENEXT_FLAG;

	if(ret <= 0)
	{
		printk("i2c_write_interface error line = %d, ret = %d\n", __LINE__, ret);
	}
}


int i2c_read_bytes(struct i2c_client *client, u8 addr, u8 *rxbuf, int len)
{
    u8 buffer[8];
    u8 retry;
    u16 left = len;
    u16 offset = 0;

		client->addr = addr;
    struct i2c_msg msg =
    {
        //.addr = ((client->addr &I2C_MASK_FLAG) | (I2C_ENEXT_FLAG)),
        .addr = ((client->addr &I2C_MASK_FLAG) | (I2C_PUSHPULL_FLAG)),
        .flags = ((client->flags & I2C_M_TEN)|I2C_M_RD),
        .buf = buffer,
        .len = 8,
        .timing = 100,
     };

    if (rxbuf == NULL)
        return -1;

    TP_DEBUG_ERR("i2c_read_bytes to device %02X len %d\n", client->addr, len);
        
    while (left > 0)
    {
        msg.buf = &rxbuf[offset];

        if (left > 8)
        {
            msg.len = 8;
            left -= 8;
            offset += 8;
        }
        else
        {
            msg.len = left;
            left = 0;
        }

        retry = 0;

        while (i2c_transfer(client->adapter, &msg, 1) != 1)
        {
            retry++;
						TP_DEBUG_ERR("I2C read failed %d th time\n", retry);
            if (retry == 20)
            {
                TP_DEBUG_ERR("I2C read length=%d failed final\n", len);
                return -1;
            }
        }
    }

    return 0;
}

/*******************************************************************************
* miBurstWriteReg
********************************************************************************/
#define MAX_CMD_LEN          255
int miBurstWriteReg(struct i2c_client *client, u8 *pData, u32 bytes, u8 i2cId)
{

    u32 phyAddr = 0;
    u8 *buf = NULL;
    u32 old_addr = 0;
    int ret = 0;
    int retry = 0;

    {
	    if (bytes > MAX_CMD_LEN) {
	        TP_DEBUG_ERR("[iBurstWriteReg] exceed the max write length \n");
	        return 1;
	    }
	    phyAddr = 0;

	    buf = dma_alloc_coherent(0, bytes, &phyAddr, GFP_KERNEL);

	    if (NULL == buf) {
	        TP_DEBUG_ERR("[iBurstWriteReg] Not enough memory \n");
	        return -1;
	    }

	    memcpy(buf, pData, bytes);
	    //TP_DEBUG_ERR("[iBurstWriteReg] bytes = %d, phy addr = 0x%x \n", bytes, phyAddr );

	    old_addr = client->addr;
		//spin_lock(&kdsensor_drv_lock);
	    client->addr = ((i2cId &  I2C_MASK_FLAG) | I2C_DMA_FLAG );
		//spin_unlock(&kdsensor_drv_lock);

	    ret = 0;
	    retry = 3;
	    do {
	        ret = i2c_master_send(client, (u8*)phyAddr, bytes);
	        retry --;
	        if (ret != bytes) {
	            TP_DEBUG_ERR("Error sent I2C ret = %d %d th times\n", ret, (3-retry));
	        }
	    }while ((ret != bytes) && (retry > 0));

	    dma_free_coherent(0, bytes, buf, phyAddr);

		//spin_lock(&kdsensor_drv_lock);
	    client->addr = old_addr;
		//spin_unlock(&kdsensor_drv_lock);
    }
    return 0;
}

static void HalTscrCDevWriteI2CSeq(u8 addr, u8* data, u16 size)
{
    int ret;
    i2c_client->addr = addr;
    ret = i2c_master_send(i2c_client, data, size);
    i2c_client->addr = FW_ADDR_MSG21XX_TP;

    if(ret <= 0)
    {
		TP_DEBUG_ERR("HalTscrCDevWriteI2CSeq error %d,addr = %d\n", ret,addr);
	}
}

/*
static void Get_Chip_Version(void)
{
    printk("[%s]: Enter!\n", __func__);
    unsigned char dbbus_tx_data[3];
    unsigned char dbbus_rx_data[2];

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0xCE;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, &dbbus_tx_data[0], 3);
    HalTscrCReadI2CSeq(FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2);
    if (dbbus_rx_data[1] == 0)
    {
        // it is Catch2
        TP_DEBUG(printk("*** Catch2 ***\n");)
        //FwVersion  = 2;// 2 means Catch2
    }
    else
    {
        // it is catch1
        TP_DEBUG(printk("*** Catch1 ***\n");)
        //FwVersion  = 1;// 1 means Catch1
    }

}
*/

static void dbbusDWIICEnterSerialDebugMode(void)
{
    u8 data[5];

    // Enter the Serial Debug Mode
    data[0] = 0x53;
    data[1] = 0x45;
    data[2] = 0x52;
    data[3] = 0x44;
    data[4] = 0x42;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, data, 5);
}

static void dbbusDWIICStopMCU(void)
{
    u8 data[1];

    // Stop the MCU
    data[0] = 0x37;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, data, 1);
}

static void dbbusDWIICIICUseBus(void)
{
    u8 data[1];

    // IIC Use Bus
    data[0] = 0x35;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, data, 1);
}

static void dbbusDWIICIICReshape(void)
{
    u8 data[1];

    // IIC Re-shape
    data[0] = 0x71;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, data, 1);
}

static void dbbusDWIICIICNotUseBus(void)
{
    u8 data[1];

    // IIC Not Use Bus
    data[0] = 0x34;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, data, 1);
}

static void dbbusDWIICNotStopMCU(void)
{
    u8 data[1];

    // Not Stop the MCU
    data[0] = 0x36;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, data, 1);
}

static void dbbusDWIICExitSerialDebugMode(void)
{
    u8 data[1];

    // Exit the Serial Debug Mode
    data[0] = 0x45;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, data, 1);

    // Delay some interval to guard the next transaction
    udelay ( 150);//200 );        // delay about 0.2ms
}

static void drvISP_EntryIspMode(void)
{
    u8 bWriteData[5] =
    {
        0x4D, 0x53, 0x54, 0x41, 0x52
    };
	TP_DEBUG("\n******%s come in*******\n",__FUNCTION__);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, bWriteData, 5);
    udelay ( 150 );//200 );        // delay about 0.1ms
}

static u8 drvISP_Read(u8 n, u8* pDataToRead)    //First it needs send 0x11 to notify we want to get flash data back.
{
    u8 Read_cmd = 0x11;
    unsigned char dbbus_rx_data[2] = {0};
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, &Read_cmd, 1);
    //msctpc_LoopDelay ( 1 );        // delay about 100us*****
    udelay( 800 );//200);
    if (n == 1)
    {
        HalTscrCReadI2CSeq(FW_UPDATE_ADDR_MSG21XX, &dbbus_rx_data[0], 2);
        *pDataToRead = dbbus_rx_data[0];
        TP_DEBUG("dbbus=%d,%d===drvISP_Read=====\n",dbbus_rx_data[0],dbbus_rx_data[1]);
  	}
    else if (n<=8)
    {
        HalTscrCReadI2CSeq(FW_UPDATE_ADDR_MSG21XX, pDataToRead, n);
    }
    else
  	{
  		if (i2c_read_bytes(i2c_client, FW_UPDATE_ADDR_MSG21XX, pDataToRead, n))
  			TP_DEBUG("i2c_read_bytes failed===drvISP_Read=====\n");
  	}

    return 0;
}

static void drvISP_WriteEnable(void)
{
    u8 bWriteData[2] =
    {
        0x10, 0x06
    };
    u8 bWriteData1 = 0x12;
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, bWriteData, 2);
    udelay(150);//1.16
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1);
}


static void drvISP_ExitIspMode(void)
{
    u8 bWriteData = 0x24;
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, &bWriteData, 1);
    udelay( 150 );//200);
}

static u8 drvISP_ReadStatus(void)
{
    u8 bReadData = 0;
    u8 bWriteData[2] =
    {
        0x10, 0x05
    };
    u8 bWriteData1 = 0x12;

    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, bWriteData, 2);
    //msctpc_LoopDelay ( 1 );        // delay about 100us*****
    udelay(150);//200);
    drvISP_Read(1, &bReadData);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1);
    return bReadData;
}


static void drvISP_BlockErase(u32 addr)
{
    u8 bWriteData[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
    u8 bWriteData1 = 0x12;
	TP_DEBUG("\n******%s come in*******\n",__FUNCTION__);
	u32 timeOutCount=0;
    drvISP_WriteEnable();

    //Enable write status register
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x50;
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, bWriteData, 2);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1);

    //Write Status
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x01;
    bWriteData[2] = 0x00;
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, bWriteData, 3);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1);

    //Write disable
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x04;
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, bWriteData, 2);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1);
	//msctpc_LoopDelay ( 1 );        // delay about 100us*****
	udelay(150);//200);
    timeOutCount=0;
	while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
	{
		timeOutCount++;
		if ( timeOutCount >= 100000 ) break; /* around 1 sec timeout */
	}
    drvISP_WriteEnable();

    bWriteData[0] = 0x10;
    bWriteData[1] = 0xC7;//0xD8;        //Block Erase
    //bWriteData[2] = ((addr >> 16) & 0xFF) ;
    //bWriteData[3] = ((addr >> 8) & 0xFF) ;
    //bWriteData[4] = (addr & 0xFF) ;
	HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, bWriteData, 2);
    //HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, &bWriteData, 5);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1);
		//msctpc_LoopDelay ( 1 );        // delay about 100us*****
	udelay(150);//200);
	timeOutCount=0;
	while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
	{
		timeOutCount++;
		if ( timeOutCount >= 500000 ) break; /* around 5 sec timeout */
	}
}

static void drvISP_Program(u16 k, u8* pDataToWrite)
{
    u16 i = 0;
    u16 j = 0;
    //u16 n = 0;
    u8 TX_data[133];
    u8 bWriteData1 = 0x12;
    u32 addr = k * 1024;
		u32 timeOutCount=0;
    for (j = 0; j < 8; j++)   //128*8 cycle
    {
        TX_data[0] = 0x10;
        TX_data[1] = 0x02;// Page Program CMD
        TX_data[2] = (addr + 128 * j) >> 16;
        TX_data[3] = (addr + 128 * j) >> 8;
        TX_data[4] = (addr + 128 * j);
        for (i = 0; i < 128; i++)
        {
            TX_data[5 + i] = pDataToWrite[j * 128 + i];
        }
        //msctpc_LoopDelay ( 1 );        // delay about 100us*****
        udelay(150);//200);
       
        timeOutCount=0;
		while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
		{
			timeOutCount++;
			if ( timeOutCount >= 100000 ) break; /* around 1 sec timeout */
		}
  
        drvISP_WriteEnable();
        //HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, TX_data, 133);   //write 133 byte per cycle
        if (miBurstWriteReg(i2c_client, TX_data, 133, FW_UPDATE_ADDR_MSG21XX))
        	TP_DEBUG("iBurstWriteReg failed===drvISP_Read=====\n");
        HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1);
    }
}

static ssize_t firmware_update_show ( struct device *dev,
                                      struct device_attribute *attr, char *buf )
{
    return sprintf ( buf, "%s\n", fw_version );
}


static void drvISP_Verify ( u16 k, u8* pDataToVerify )
{
    u16 i = 0, j = 0;
    u8 bWriteData[5] ={ 0x10, 0x03, 0, 0, 0 };
    u8 RX_data[256];
    u8 bWriteData1 = 0x12;
    u32 addr = k * 1024;
    u8 index = 0;
    u32 timeOutCount;
    for ( j = 0; j < 8; j++ ) //128*8 cycle
    {
        bWriteData[2] = ( u8 ) ( ( addr + j * 128 ) >> 16 );
        bWriteData[3] = ( u8 ) ( ( addr + j * 128 ) >> 8 );
        bWriteData[4] = ( u8 ) ( addr + j * 128 );
        udelay ( 100 );        // delay about 100us*****

        timeOutCount = 0;
        while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
        {
            timeOutCount++;
            if ( timeOutCount >= 100000 ) break; /* around 1 sec timeout */
        }

        HalTscrCDevWriteI2CSeq ( FW_UPDATE_ADDR_MSG21XX, bWriteData, 5 ); //write read flash addr
        udelay ( 100 );        // delay about 100us*****
        drvISP_Read ( 128, RX_data );
        HalTscrCDevWriteI2CSeq ( FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1 ); //cmd end
        for ( i = 0; i < 128; i++ ) //log out if verify error
        {
            if ( ( RX_data[i] != 0 ) && index < 10 )
            {
                //TP_DEBUG("j=%d,RX_data[%d]=0x%x\n",j,i,RX_data[i]);
                index++;
            }
            if ( RX_data[i] != pDataToVerify[128 * j + i] )
            {
                TP_DEBUG ( "k=%d,j=%d,i=%d===============Update Firmware Error================", k, j, i );
            }
        }
    }
}

static void drvISP_ChipErase()
{
    u8 bWriteData[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
    u8 bWriteData1 = 0x12;
    u32 timeOutCount = 0;
    drvISP_WriteEnable();

    //Enable write status register
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x50;
    HalTscrCDevWriteI2CSeq ( FW_UPDATE_ADDR_MSG21XX, bWriteData, 2 );
    HalTscrCDevWriteI2CSeq ( FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1 );

    //Write Status
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x01;
    bWriteData[2] = 0x00;
    HalTscrCDevWriteI2CSeq ( FW_UPDATE_ADDR_MSG21XX, bWriteData, 3 );
    HalTscrCDevWriteI2CSeq ( FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1 );

    //Write disable
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x04;
    HalTscrCDevWriteI2CSeq ( FW_UPDATE_ADDR_MSG21XX, bWriteData, 2 );
    HalTscrCDevWriteI2CSeq ( FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1 );
    udelay ( 100 );        // delay about 100us*****
    timeOutCount = 0;
    while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
    {
        timeOutCount++;
        if ( timeOutCount >= 100000 ) break; /* around 1 sec timeout */
    }
    drvISP_WriteEnable();

    bWriteData[0] = 0x10;
    bWriteData[1] = 0xC7;

    HalTscrCDevWriteI2CSeq ( FW_UPDATE_ADDR_MSG21XX, bWriteData, 2 );
    HalTscrCDevWriteI2CSeq ( FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1 );
    udelay ( 100 );        // delay about 100us*****
    timeOutCount = 0;
    while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
    {
        timeOutCount++;
        if ( timeOutCount >= 500000 ) break; /* around 5 sec timeout */
    }
}

/* update the firmware part, used by apk*/
/*show the fw version*/

static ssize_t firmware_update_c2 ( struct device *dev,
                                    struct device_attribute *attr, const char *buf, size_t size )
{
    u8 i;
    u8 dbbus_tx_data[4];
    unsigned char dbbus_rx_data[2] = {0};

    // set FRO to 50M
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x11;
    dbbus_tx_data[2] = 0xE2;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 3 );
    dbbus_rx_data[0] = 0;
    dbbus_rx_data[1] = 0;
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2 );
    TP_DEBUG ( "dbbus_rx_data[0]=0x%x", dbbus_rx_data[0] );
    dbbus_tx_data[3] = dbbus_rx_data[0] & 0xF7;  //Clear Bit 3
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // set MCU clock,SPI clock =FRO
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x22;
    dbbus_tx_data[3] = 0x00;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x23;
    dbbus_tx_data[3] = 0x00;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // Enable slave's ISP ECO mode
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x08;
    dbbus_tx_data[2] = 0x0c;
    dbbus_tx_data[3] = 0x08;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // Enable SPI Pad
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x02;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 3 );
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2 );
    TP_DEBUG ( "dbbus_rx_data[0]=0x%x", dbbus_rx_data[0] );
    dbbus_tx_data[3] = ( dbbus_rx_data[0] | 0x20 ); //Set Bit 5
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // WP overwrite
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x0E;
    dbbus_tx_data[3] = 0x02;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // set pin high
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x10;
    dbbus_tx_data[3] = 0x08;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    dbbusDWIICIICNotUseBus();
    dbbusDWIICNotStopMCU();
    dbbusDWIICExitSerialDebugMode();

    drvISP_EntryIspMode();
    drvISP_ChipErase();
    _HalTscrHWReset();
    mdelay ( 300 );

    // Program and Verify
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();

    // Disable the Watchdog
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x60;
    dbbus_tx_data[3] = 0x55;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x61;
    dbbus_tx_data[3] = 0xAA;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    //Stop MCU
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x0F;
    dbbus_tx_data[2] = 0xE6;
    dbbus_tx_data[3] = 0x01;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // set FRO to 50M
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x11;
    dbbus_tx_data[2] = 0xE2;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 3 );
    dbbus_rx_data[0] = 0;
    dbbus_rx_data[1] = 0;
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2 );
    TP_DEBUG ( "dbbus_rx_data[0]=0x%x", dbbus_rx_data[0] );
    dbbus_tx_data[3] = dbbus_rx_data[0] & 0xF7;  //Clear Bit 3
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // set MCU clock,SPI clock =FRO
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x22;
    dbbus_tx_data[3] = 0x00;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x23;
    dbbus_tx_data[3] = 0x00;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // Enable slave's ISP ECO mode
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x08;
    dbbus_tx_data[2] = 0x0c;
    dbbus_tx_data[3] = 0x08;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // Enable SPI Pad
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x02;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 3 );
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2 );
    TP_DEBUG ( "dbbus_rx_data[0]=0x%x", dbbus_rx_data[0] );
    dbbus_tx_data[3] = ( dbbus_rx_data[0] | 0x20 ); //Set Bit 5
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // WP overwrite
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x0E;
    dbbus_tx_data[3] = 0x02;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // set pin high
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x10;
    dbbus_tx_data[3] = 0x08;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    dbbusDWIICIICNotUseBus();
    dbbusDWIICNotStopMCU();
    dbbusDWIICExitSerialDebugMode();

    ///////////////////////////////////////
    // Start to load firmware
    ///////////////////////////////////////
    drvISP_EntryIspMode();

    for ( i = 0; i < 94; i++ ) // total  94 KB : 1 byte per R/W
    {
        drvISP_Program ( i, temp[i] ); // program to slave's flash
        drvISP_Verify ( i, temp[i] ); //verify data
    }
    TP_DEBUG_ERR ( "update_C2 OK\n" );
    drvISP_ExitIspMode();
    _HalTscrHWReset();
    FwDataCnt = 0;
    HalEnableIrq();	
    return size;
}

static u32 Reflect ( u32 ref, char ch ) //unsigned int Reflect(unsigned int ref, char ch)
{
    u32 value = 0;
    u32 i = 0;

    for ( i = 1; i < ( ch + 1 ); i++ )
    {
        if ( ref & 1 )
        {
            value |= 1 << ( ch - i );
        }
        ref >>= 1;
    }
    return value;
}

u32 Get_CRC ( u32 text, u32 prevCRC, u32 *crc32_table )
{
    u32  ulCRC = prevCRC;
	ulCRC = ( ulCRC >> 8 ) ^ crc32_table[ ( ulCRC & 0xFF ) ^ text];
    return ulCRC ;
}
static void Init_CRC32_Table ( u32 *crc32_table )
{
    u32 magicnumber = 0x04c11db7;
    u32 i = 0, j;

    for ( i = 0; i <= 0xFF; i++ )
    {
        crc32_table[i] = Reflect ( i, 8 ) << 24;
        for ( j = 0; j < 8; j++ )
        {
            crc32_table[i] = ( crc32_table[i] << 1 ) ^ ( crc32_table[i] & ( 0x80000000L ) ? magicnumber : 0 );
        }
        crc32_table[i] = Reflect ( crc32_table[i], 32 );
    }
}

typedef enum
{
	EMEM_ALL = 0,
	EMEM_MAIN,
	EMEM_INFO,
} EMEM_TYPE_t;

static void drvDB_WriteReg8Bit ( u8 bank, u8 addr, u8 data )
{
    u8 tx_data[4] = {0x10, bank, addr, data};
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, tx_data, 4 );
}

static void drvDB_WriteReg ( u8 bank, u8 addr, u16 data )
{
    u8 tx_data[5] = {0x10, bank, addr, data & 0xFF, data >> 8};
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, tx_data, 5 );
}

static unsigned short drvDB_ReadReg ( u8 bank, u8 addr )
{
    u8 tx_data[3] = {0x10, bank, addr};
    u8 rx_data[2] = {0};

    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, tx_data, 3 );
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &rx_data[0], 2 );
    return ( rx_data[1] << 8 | rx_data[0] );
}

static int drvTP_erase_emem_c32 ( void )
{
    /////////////////////////
    //Erase  all
    /////////////////////////
    
    //enter gpio mode
    drvDB_WriteReg ( 0x16, 0x1E, 0xBEAF );

    // before gpio mode, set the control pin as the orginal status
    drvDB_WriteReg ( 0x16, 0x08, 0x0000 );
    drvDB_WriteReg8Bit ( 0x16, 0x0E, 0x10 );
    mdelay ( 10 ); //MCR_CLBK_DEBUG_DELAY ( 10, MCU_LOOP_DELAY_COUNT_MS );

    // ptrim = 1, h'04[2]
    drvDB_WriteReg8Bit ( 0x16, 0x08, 0x04 );
    drvDB_WriteReg8Bit ( 0x16, 0x0E, 0x10 );
    mdelay ( 10 ); //MCR_CLBK_DEBUG_DELAY ( 10, MCU_LOOP_DELAY_COUNT_MS );

    // ptm = 6, h'04[12:14] = b'110
    drvDB_WriteReg8Bit ( 0x16, 0x09, 0x60 );
    drvDB_WriteReg8Bit ( 0x16, 0x0E, 0x10 );

    // pmasi = 1, h'04[6]
    drvDB_WriteReg8Bit ( 0x16, 0x08, 0x44 );
    // pce = 1, h'04[11]
    drvDB_WriteReg8Bit ( 0x16, 0x09, 0x68 );
    // perase = 1, h'04[7]
    drvDB_WriteReg8Bit ( 0x16, 0x08, 0xC4 );
    // pnvstr = 1, h'04[5]
    drvDB_WriteReg8Bit ( 0x16, 0x08, 0xE4 );
    // pwe = 1, h'04[9]
    drvDB_WriteReg8Bit ( 0x16, 0x09, 0x6A );
    // trigger gpio load
    drvDB_WriteReg8Bit ( 0x16, 0x0E, 0x10 );

    return ( 1 );
}

static ssize_t firmware_update_c32 ( struct device *dev, struct device_attribute *attr,
                                     const char *buf, size_t size,  EMEM_TYPE_t emem_type )
{
    u8  dbbus_tx_data[4];
    u8  dbbus_rx_data[2] = {0};
      // Buffer for slave's firmware

    u32 i, j;
    u32 crc_main, crc_main_tp;
    u32 crc_info, crc_info_tp;
    u16 reg_data = 0;

    crc_main = 0xffffffff;
    crc_info = 0xffffffff;

#if 1
    /////////////////////////
    // Erase  all
    /////////////////////////
    drvTP_erase_emem_c32();
    mdelay ( 1000 ); //MCR_CLBK_DEBUG_DELAY ( 1000, MCU_LOOP_DELAY_COUNT_MS );

    //ResetSlave();
    _HalTscrHWReset();
    //drvDB_EnterDBBUS();
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();
    mdelay ( 300 );

    // Reset Watchdog
    drvDB_WriteReg8Bit ( 0x3C, 0x60, 0x55 );
    drvDB_WriteReg8Bit ( 0x3C, 0x61, 0xAA );

    /////////////////////////
    // Program
    /////////////////////////

    //polling 0x3CE4 is 0x1C70
    do
    {
        reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
    }
    while ( reg_data != 0x1C70 );


    drvDB_WriteReg ( 0x3C, 0xE4, 0xE38F );  // for all-blocks

    //polling 0x3CE4 is 0x2F43
    do
    {
        reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
    }
    while ( reg_data != 0x2F43 );


    //calculate CRC 32
    Init_CRC32_Table ( &crc_tab[0] );

    for ( i = 0; i < 33; i++ ) // total  33 KB : 2 byte per R/W
    {
        if ( i < 32 )   //emem_main
        {
            if ( i == 31 )
            {
                temp[i][1014] = 0x5A; //Fmr_Loader[1014]=0x5A;
                temp[i][1015] = 0xA5; //Fmr_Loader[1015]=0xA5;

                for ( j = 0; j < 1016; j++ )
                {
                    //crc_main=Get_CRC(Fmr_Loader[j],crc_main,&crc_tab[0]);
                    crc_main = Get_CRC ( temp[i][j], crc_main, &crc_tab[0] );
                }
            }
            else
            {
                for ( j = 0; j < 1024; j++ )
                {
                    //crc_main=Get_CRC(Fmr_Loader[j],crc_main,&crc_tab[0]);
                    crc_main = Get_CRC ( temp[i][j], crc_main, &crc_tab[0] );
                }
            }
        }
        else  // emem_info
        {
            for ( j = 0; j < 1024; j++ )
            {
                //crc_info=Get_CRC(Fmr_Loader[j],crc_info,&crc_tab[0]);
                crc_info = Get_CRC ( temp[i][j], crc_info, &crc_tab[0] );
            }
        }

        //drvDWIIC_MasterTransmit( DWIIC_MODE_DWIIC_ID, 1024, Fmr_Loader );
        HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX_TP, temp[i], 1024 );

        // polling 0x3CE4 is 0xD0BC
        do
        {
            reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
        }
        while ( reg_data != 0xD0BC );

        drvDB_WriteReg ( 0x3C, 0xE4, 0x2F43 );
    }

    //write file done
    drvDB_WriteReg ( 0x3C, 0xE4, 0x1380 );

    mdelay ( 10 ); //MCR_CLBK_DEBUG_DELAY ( 10, MCU_LOOP_DELAY_COUNT_MS );
    // polling 0x3CE4 is 0x9432
    do
    {
        reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
    }
    while ( reg_data != 0x9432 );

    crc_main = crc_main ^ 0xffffffff;
    crc_info = crc_info ^ 0xffffffff;

    // CRC Main from TP
    crc_main_tp = drvDB_ReadReg ( 0x3C, 0x80 );
    crc_main_tp = ( crc_main_tp << 16 ) | drvDB_ReadReg ( 0x3C, 0x82 );
 
    //CRC Info from TP
    crc_info_tp = drvDB_ReadReg ( 0x3C, 0xA0 );
    crc_info_tp = ( crc_info_tp << 16 ) | drvDB_ReadReg ( 0x3C, 0xA2 );

    TP_DEBUG ( "crc_main=0x%x, crc_info=0x%x, crc_main_tp=0x%x, crc_info_tp=0x%x\n",
               crc_main, crc_info, crc_main_tp, crc_info_tp );

    //drvDB_ExitDBBUS();
    if ( ( crc_main_tp != crc_main ) || ( crc_info_tp != crc_info ) )
    {
        TP_DEBUG_ERR ( "update_C32 FAILED\n" );
		_HalTscrHWReset();
        FwDataCnt = 0;
    	HalEnableIrq();		
        return ( 0 );
    }

    TP_DEBUG_ERR ( "update_C32 OK\n" );
	_HalTscrHWReset();
    FwDataCnt = 0;
	HalEnableIrq();	

    return size;
#endif
}

static int drvTP_erase_emem_c33 ( EMEM_TYPE_t emem_type )
{
    // stop mcu
    drvDB_WriteReg ( 0x0F, 0xE6, 0x0001 );

    //disable watch dog
    drvDB_WriteReg8Bit ( 0x3C, 0x60, 0x55 );
    drvDB_WriteReg8Bit ( 0x3C, 0x61, 0xAA );

    // set PROGRAM password
    drvDB_WriteReg8Bit ( 0x16, 0x1A, 0xBA );
    drvDB_WriteReg8Bit ( 0x16, 0x1B, 0xAB );

    //proto.MstarWriteReg(F1.loopDevice, 0x1618, 0x80);
    drvDB_WriteReg8Bit ( 0x16, 0x18, 0x80 );

    if ( emem_type == EMEM_ALL )
    {
        drvDB_WriteReg8Bit ( 0x16, 0x08, 0x10 ); //mark
    }

    drvDB_WriteReg8Bit ( 0x16, 0x18, 0x40 );
    mdelay ( 10 );

    drvDB_WriteReg8Bit ( 0x16, 0x18, 0x80 );

    // erase trigger
    if ( emem_type == EMEM_MAIN )
    {
        drvDB_WriteReg8Bit ( 0x16, 0x0E, 0x04 ); //erase main
    }
    else
    {
        drvDB_WriteReg8Bit ( 0x16, 0x0E, 0x08 ); //erase all block
    }

    return ( 1 );
}

static int drvTP_read_emem_dbbus_c33 ( EMEM_TYPE_t emem_type, u16 addr, size_t size, u8 *p, size_t set_pce_high )
{
    u32 i;

    // Set the starting address ( must before enabling burst mode and enter riu mode )
    drvDB_WriteReg ( 0x16, 0x00, addr );

    // Enable the burst mode ( must before enter riu mode )
    drvDB_WriteReg ( 0x16, 0x0C, drvDB_ReadReg ( 0x16, 0x0C ) | 0x0001 );

    // Set the RIU password
    drvDB_WriteReg ( 0x16, 0x1A, 0xABBA );

    // Enable the information block if pifren is HIGH
    if ( emem_type == EMEM_INFO )
    {
        // Clear the PCE
        drvDB_WriteReg ( 0x16, 0x18, drvDB_ReadReg ( 0x16, 0x18 ) | 0x0080 );
        mdelay ( 10 );

        // Set the PIFREN to be HIGH
        drvDB_WriteReg ( 0x16, 0x08, 0x0010 );
    }

    // Set the PCE to be HIGH
    drvDB_WriteReg ( 0x16, 0x18, drvDB_ReadReg ( 0x16, 0x18 ) | 0x0040 );
    mdelay ( 10 );

    // Wait pce becomes 1 ( read data ready )
    while ( ( drvDB_ReadReg ( 0x16, 0x10 ) & 0x0004 ) != 0x0004 );

    for ( i = 0; i < size; i += 4 )
    {
        // Fire the FASTREAD command
        drvDB_WriteReg ( 0x16, 0x0E, drvDB_ReadReg ( 0x16, 0x0E ) | 0x0001 );

        // Wait the operation is done
        while ( ( drvDB_ReadReg ( 0x16, 0x10 ) & 0x0001 ) != 0x0001 );

        p[i + 0] = drvDB_ReadReg ( 0x16, 0x04 ) & 0xFF;
        p[i + 1] = ( drvDB_ReadReg ( 0x16, 0x04 ) >> 8 ) & 0xFF;
        p[i + 2] = drvDB_ReadReg ( 0x16, 0x06 ) & 0xFF;
        p[i + 3] = ( drvDB_ReadReg ( 0x16, 0x06 ) >> 8 ) & 0xFF;
    }

    // Disable the burst mode
    drvDB_WriteReg ( 0x16, 0x0C, drvDB_ReadReg ( 0x16, 0x0C ) & ( ~0x0001 ) );

    // Clear the starting address
    drvDB_WriteReg ( 0x16, 0x00, 0x0000 );

    //Always return to main block
    if ( emem_type == EMEM_INFO )
    {
        // Clear the PCE before change block
        drvDB_WriteReg ( 0x16, 0x18, drvDB_ReadReg ( 0x16, 0x18 ) | 0x0080 );
        mdelay ( 10 );
        // Set the PIFREN to be LOW
        drvDB_WriteReg ( 0x16, 0x08, drvDB_ReadReg ( 0x16, 0x08 ) & ( ~0x0010 ) );

        drvDB_WriteReg ( 0x16, 0x18, drvDB_ReadReg ( 0x16, 0x18 ) | 0x0040 );
        while ( ( drvDB_ReadReg ( 0x16, 0x10 ) & 0x0004 ) != 0x0004 );
    }

    // Clear the RIU password
    drvDB_WriteReg ( 0x16, 0x1A, 0x0000 );

    if ( set_pce_high )
    {
        // Set the PCE to be HIGH before jumping back to e-flash codes
        drvDB_WriteReg ( 0x16, 0x18, drvDB_ReadReg ( 0x16, 0x18 ) | 0x0040 );
        while ( ( drvDB_ReadReg ( 0x16, 0x10 ) & 0x0004 ) != 0x0004 );
    }

    return ( 1 );
}


static int drvTP_read_info_dwiic_c33 ( void )
{
    u8  dwiic_tx_data[5];
    u8  dwiic_rx_data[4];
    u16 reg_data=0;
    mdelay ( 300 );

    // Stop Watchdog
    drvDB_WriteReg8Bit ( 0x3C, 0x60, 0x55 );
    drvDB_WriteReg8Bit ( 0x3C, 0x61, 0xAA );

    drvDB_WriteReg ( 0x3C, 0xE4, 0xA4AB );

	drvDB_WriteReg ( 0x1E, 0x04, 0x7d60 );

    // TP SW reset
    drvDB_WriteReg ( 0x1E, 0x04, 0x829F );
	mdelay ( 1 );
    dwiic_tx_data[0] = 0x10;
    dwiic_tx_data[1] = 0x0F;
    dwiic_tx_data[2] = 0xE6;
    dwiic_tx_data[3] = 0x00;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dwiic_tx_data, 4 );	
    mdelay ( 100 );
    do{
        reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
    }
    while ( reg_data != 0x5B58 );
    dwiic_tx_data[0] = 0x72;
    dwiic_tx_data[1] = 0x80;
    dwiic_tx_data[2] = 0x00;
    dwiic_tx_data[3] = 0x04;
    dwiic_tx_data[4] = 0x00;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX_TP , dwiic_tx_data, 5 );
    mdelay ( 50 );

    // recive info data
    //HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX_TP, &g_dwiic_info_data[0], 1024 );
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX_TP, &g_dwiic_info_data[0], 8 );
    return ( 1 );
}

static int drvTP_info_updata_C33 ( u16 start_index, u8 *data, u16 size )
{
    // size != 0, start_index+size !> 1024
    u16 i;
    for ( i = 0; i < size; i++ )
    {
        g_dwiic_info_data[start_index] = * ( data + i );
        start_index++;
    }
    return ( 1 );
}

static ssize_t firmware_update_c33 ( struct device *dev, struct device_attribute *attr,
                                     const char *buf, size_t size, EMEM_TYPE_t emem_type )
{
    u8  dbbus_tx_data[4];
    u8  dbbus_rx_data[2] = {0};
    u8  life_counter[2];
    u32 i, j;
    u32 crc_main, crc_main_tp;
    u32 crc_info, crc_info_tp;
  
    int update_pass = 1;
    u16 reg_data = 0;

    crc_main = 0xffffffff;
    crc_info = 0xffffffff;
    drvTP_read_info_dwiic_c33();
	
    if ( 0/*g_dwiic_info_data[0] == 'M' && g_dwiic_info_data[1] == 'S' && g_dwiic_info_data[2] == 'T' && g_dwiic_info_data[3] == 'A' && g_dwiic_info_data[4] == 'R' && g_dwiic_info_data[5] == 'T' && g_dwiic_info_data[6] == 'P' && g_dwiic_info_data[7] == 'C' */)
    {
        // updata FW Version
        //drvTP_info_updata_C33 ( 8, &temp[32][8], 5 );

		g_dwiic_info_data[8]=temp[32][8];
		g_dwiic_info_data[9]=temp[32][9];
		g_dwiic_info_data[10]=temp[32][10];
		g_dwiic_info_data[11]=temp[32][11];
        // updata life counter
        life_counter[1] = (( ( (g_dwiic_info_data[13] << 8 ) | g_dwiic_info_data[12]) + 1 ) >> 8 ) & 0xFF;
        life_counter[0] = ( ( (g_dwiic_info_data[13] << 8 ) | g_dwiic_info_data[12]) + 1 ) & 0xFF;
		g_dwiic_info_data[12]=life_counter[0];
		g_dwiic_info_data[13]=life_counter[1];
        //drvTP_info_updata_C33 ( 10, &life_counter[0], 3 );
        drvDB_WriteReg ( 0x3C, 0xE4, 0x78C5 );
		drvDB_WriteReg ( 0x1E, 0x04, 0x7d60 );
        // TP SW reset
        drvDB_WriteReg ( 0x1E, 0x04, 0x829F );

        mdelay ( 50 );
        do
        {
            reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );

        }
        while ( reg_data != 0x2F43 );
        // transmit lk info data
        HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX_TP , &g_dwiic_info_data[0], 1024 );
        //polling 0x3CE4 is 0xD0BC
        do
        {
            reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
        }
        while ( reg_data != 0xD0BC );
    }

    //erase main
    drvTP_erase_emem_c33 ( EMEM_MAIN );
    mdelay ( 1000 );

    //ResetSlave();
    _HalTscrHWReset();

    //drvDB_EnterDBBUS();
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();
    mdelay ( 300 );

    /////////////////////////
    // Program
    /////////////////////////

    //polling 0x3CE4 is 0x1C70
    if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_MAIN ) )
    {
        do
        {
            reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
        }
        while ( reg_data != 0x1C70 );
    }

    switch ( emem_type )
    {
        case EMEM_ALL:
            drvDB_WriteReg ( 0x3C, 0xE4, 0xE38F );  // for all-blocks
            break;
        case EMEM_MAIN:
            drvDB_WriteReg ( 0x3C, 0xE4, 0x7731 );  // for main block
            break;
        case EMEM_INFO:
            drvDB_WriteReg ( 0x3C, 0xE4, 0x7731 );  // for info block

            drvDB_WriteReg8Bit ( 0x0F, 0xE6, 0x01 );

            drvDB_WriteReg8Bit ( 0x3C, 0xE4, 0xC5 ); //
            drvDB_WriteReg8Bit ( 0x3C, 0xE5, 0x78 ); //

            drvDB_WriteReg8Bit ( 0x1E, 0x04, 0x9F );
            drvDB_WriteReg8Bit ( 0x1E, 0x05, 0x82 );

            drvDB_WriteReg8Bit ( 0x0F, 0xE6, 0x00 );
            mdelay ( 100 );
            break;
    }
    // polling 0x3CE4 is 0x2F43
    do
    {
        reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
    }
    while ( reg_data != 0x2F43 );
    // calculate CRC 32
    Init_CRC32_Table ( &crc_tab[0] );

    for ( i = 0; i < 33; i++ ) // total  33 KB : 2 byte per R/W
    {
        if ( emem_type == EMEM_INFO )
			i = 32;

        if ( i < 32 )   //emem_main
        {
            if ( i == 31 )
            {
                temp[i][1014] = 0x5A; //Fmr_Loader[1014]=0x5A;
                temp[i][1015] = 0xA5; //Fmr_Loader[1015]=0xA5;

                for ( j = 0; j < 1016; j++ )
                {
                    //crc_main=Get_CRC(Fmr_Loader[j],crc_main,&crc_tab[0]);
                    crc_main = Get_CRC ( temp[i][j], crc_main, &crc_tab[0] );
                }
            }
            else
            {
                for ( j = 0; j < 1024; j++ )
                {
                    //crc_main=Get_CRC(Fmr_Loader[j],crc_main,&crc_tab[0]);
                    crc_main = Get_CRC ( temp[i][j], crc_main, &crc_tab[0] );
                }
            }
        }
        else  //emem_info
        {
            for ( j = 0; j < 1024; j++ )
            {
                //crc_info=Get_CRC(Fmr_Loader[j],crc_info,&crc_tab[0]);
                crc_info = Get_CRC ( g_dwiic_info_data[j], crc_info, &crc_tab[0] );
            }
            if ( emem_type == EMEM_MAIN ) break;
        }
        //drvDWIIC_MasterTransmit( DWIIC_MODE_DWIIC_ID, 1024, Fmr_Loader );
        #if 1
        {
            u32 n = 0;
            for(n=0;n<UPDATE_TIMES;n++)
            {
                HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX_TP, temp[i]+n*N_BYTE_PER_TIME, N_BYTE_PER_TIME );
            }
        }
        #else
        HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX_TP, temp[i], 1024 );
        #endif
        // polling 0x3CE4 is 0xD0BC
        do
        {
            reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
        }
        while ( reg_data != 0xD0BC );
        drvDB_WriteReg ( 0x3C, 0xE4, 0x2F43 );
    }
    if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_MAIN ) )
    {
        // write file done and check crc
        drvDB_WriteReg ( 0x3C, 0xE4, 0x1380 );
    }
    mdelay ( 10 ); //MCR_CLBK_DEBUG_DELAY ( 10, MCU_LOOP_DELAY_COUNT_MS );

    if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_MAIN ) )
    {
        // polling 0x3CE4 is 0x9432
        do
        {
            reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
        }while ( reg_data != 0x9432 );
    }

    crc_main = crc_main ^ 0xffffffff;
    crc_info = crc_info ^ 0xffffffff;

    if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_MAIN ) )
    {
        // CRC Main from TP
        crc_main_tp = drvDB_ReadReg ( 0x3C, 0x80 );
        crc_main_tp = ( crc_main_tp << 16 ) | drvDB_ReadReg ( 0x3C, 0x82 );

        // CRC Info from TP
        crc_info_tp = drvDB_ReadReg ( 0x3C, 0xA0 );
        crc_info_tp = ( crc_info_tp << 16 ) | drvDB_ReadReg ( 0x3C, 0xA2 );
    }
    TP_DEBUG ( "crc_main=0x%x, crc_info=0x%x, crc_main_tp=0x%x, crc_info_tp=0x%x\n",
               crc_main, crc_info, crc_main_tp, crc_info_tp );

    //drvDB_ExitDBBUS();
    update_pass = 1;
	if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_MAIN ) )
    {
        if ( crc_main_tp != crc_main )
            update_pass = 0;

        if ( crc_info_tp != crc_info )
            update_pass = 0;
    }
    if ( !update_pass )
    {
        TP_DEBUG_ERR ( "update_C33 FAILED\n" );
		_HalTscrHWReset();
        FwDataCnt = 0;
    	HalEnableIrq();	
        return size;
    }
    TP_DEBUG_ERR ( "update_C33 OK\n" );
	_HalTscrHWReset();
    FwDataCnt = 0;
    HalEnableIrq();	
    return size;
}

#define _FW_UPDATE_C3_
#ifdef _FW_UPDATE_C3_
static ssize_t firmware_update_store ( struct device *dev,
                                       struct device_attribute *attr, const char *buf, size_t size )
{
    u8 i;
    u8 dbbus_tx_data[4];
    unsigned char dbbus_rx_data[2] = {0};
	HalDisableIrq();

    _HalTscrHWReset();

    // Erase TP Flash first
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();
    mdelay ( 300 );

    // Disable the Watchdog
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x60;
    dbbus_tx_data[3] = 0x55;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x61;
    dbbus_tx_data[3] = 0xAA;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );
    // Stop MCU
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x0F;
    dbbus_tx_data[2] = 0xE6;
    dbbus_tx_data[3] = 0x01;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );
    /////////////////////////
    // Difference between C2 and C3
    /////////////////////////
	// c2:2133 c32:2133a(2) c33:2138
    //check id
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0xCC;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 3 );
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2 );
    TP_DEBUG_ERR ( "111dbbus_rx version[0]=0x%x", dbbus_rx_data[0] );
    if ( dbbus_rx_data[0] == 2 )
    {
        // check version
        dbbus_tx_data[0] = 0x10;
        dbbus_tx_data[1] = 0x3C;
        dbbus_tx_data[2] = 0xEA;
        HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 3 );
        HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2 );
        TP_DEBUG_ERR ( "dbbus_rx version[0]=0x%x", dbbus_rx_data[0] );

        if ( dbbus_rx_data[0] == 3 ){
            return firmware_update_c33 ( dev, attr, buf, size, EMEM_MAIN );
		}
        else{

            return firmware_update_c32 ( dev, attr, buf, size, EMEM_ALL );
        }
    }
    else
    {
        return firmware_update_c2 ( dev, attr, buf, size );
    } 
}
#else
static ssize_t firmware_update_store ( struct device *dev,
                                       struct device_attribute *attr, const char *buf, size_t size )
{
    u8 i;
    u8 dbbus_tx_data[4];
    unsigned char dbbus_rx_data[2] = {0};

    _HalTscrHWReset();

    // 1. Erase TP Flash first
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();
    mdelay ( 300 );

    // Disable the Watchdog
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x60;
    dbbus_tx_data[3] = 0x55;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x61;
    dbbus_tx_data[3] = 0xAA;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // Stop MCU
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x0F;
    dbbus_tx_data[2] = 0xE6;
    dbbus_tx_data[3] = 0x01;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // set FRO to 50M
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x11;
    dbbus_tx_data[2] = 0xE2;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 3 );
    dbbus_rx_data[0] = 0;
    dbbus_rx_data[1] = 0;
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2 );
    TP_DEBUG ( "dbbus_rx_data[0]=0x%x", dbbus_rx_data[0] );
    dbbus_tx_data[3] = dbbus_rx_data[0] & 0xF7;  //Clear Bit 3
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // set MCU clock,SPI clock =FRO
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x22;
    dbbus_tx_data[3] = 0x00;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x23;
    dbbus_tx_data[3] = 0x00;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // Enable slave's ISP ECO mode
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x08;
    dbbus_tx_data[2] = 0x0c;
    dbbus_tx_data[3] = 0x08;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // Enable SPI Pad
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x02;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 3 );
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2 );
    TP_DEBUG ( "dbbus_rx_data[0]=0x%x", dbbus_rx_data[0] );
    dbbus_tx_data[3] = ( dbbus_rx_data[0] | 0x20 ); //Set Bit 5
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // WP overwrite
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x0E;
    dbbus_tx_data[3] = 0x02;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // set pin high
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x10;
    dbbus_tx_data[3] = 0x08;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    dbbusDWIICIICNotUseBus();
    dbbusDWIICNotStopMCU();
    dbbusDWIICExitSerialDebugMode();

    drvISP_EntryIspMode();
    drvISP_ChipErase();
    _HalTscrHWReset();
    mdelay ( 300 );

    // 2.Program and Verify
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();

    // Disable the Watchdog
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x60;
    dbbus_tx_data[3] = 0x55;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x61;
    dbbus_tx_data[3] = 0xAA;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // Stop MCU
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x0F;
    dbbus_tx_data[2] = 0xE6;
    dbbus_tx_data[3] = 0x01;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // set FRO to 50M
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x11;
    dbbus_tx_data[2] = 0xE2;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 3 );
    dbbus_rx_data[0] = 0;
    dbbus_rx_data[1] = 0;
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2 );
    TP_DEBUG ( "dbbus_rx_data[0]=0x%x", dbbus_rx_data[0] );
    dbbus_tx_data[3] = dbbus_rx_data[0] & 0xF7;  //Clear Bit 3
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // set MCU clock,SPI clock =FRO
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x22;
    dbbus_tx_data[3] = 0x00;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x23;
    dbbus_tx_data[3] = 0x00;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // Enable slave's ISP ECO mode
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x08;
    dbbus_tx_data[2] = 0x0c;
    dbbus_tx_data[3] = 0x08;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // Enable SPI Pad
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x02;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 3 );
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2 );
    TP_DEBUG ( "dbbus_rx_data[0]=0x%x", dbbus_rx_data[0] );
    dbbus_tx_data[3] = ( dbbus_rx_data[0] | 0x20 ); //Set Bit 5
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // WP overwrite
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x0E;
    dbbus_tx_data[3] = 0x02;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    // set pin high
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x10;
    dbbus_tx_data[3] = 0x08;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );

    dbbusDWIICIICNotUseBus();
    dbbusDWIICNotStopMCU();
    dbbusDWIICExitSerialDebugMode();

    ///////////////////////////////////////
    // Start to load firmware
    ///////////////////////////////////////
    drvISP_EntryIspMode();

    for ( i = 0; i < 94; i++ ) // total  94 KB : 1 byte per R/W
    {
        drvISP_Program ( i, temp[i] ); // program to slave's flash
        drvISP_Verify ( i, temp[i] ); //verify data
    }
    TP_DEBUG ( "update OK\n" );
    drvISP_ExitIspMode();
    FwDataCnt = 0;
    
    return size;
}
#endif
static DEVICE_ATTR(update, CTP_AUTHORITY, firmware_update_show, firmware_update_store);
#if 0
/*test=================*/
static ssize_t firmware_clear_show(struct device *dev,
                                    struct device_attribute *attr, char *buf)
{
	printk(" +++++++ [%s] Enter!++++++\n", __func__);
	u16 k=0,i = 0, j = 0;
	u8 bWriteData[5] =
	{
        0x10, 0x03, 0, 0, 0
	};
	u8 RX_data[256];
	u8 bWriteData1 = 0x12;
	u32 addr = 0;
	u32 timeOutCount=0;
	for (k = 0; k < 94; i++)   // total  94 KB : 1 byte per R/W
	{
		addr = k * 1024;
		for (j = 0; j < 8; j++)   //128*8 cycle
		{
			bWriteData[2] = (u8)((addr + j * 128) >> 16);
			bWriteData[3] = (u8)((addr + j * 128) >> 8);
			bWriteData[4] = (u8)(addr + j * 128);
			//msctpc_LoopDelay ( 1 );        // delay about 100us*****
			udelay(150);//200);

			timeOutCount=0;
			while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
			{
				timeOutCount++;
				if ( timeOutCount >= 100000 ) 
					break; /* around 1 sec timeout */
	  		}
        
			HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, bWriteData, 5);    //write read flash addr
			//msctpc_LoopDelay ( 1 );        // delay about 100us*****
			udelay(150);//200);
			drvISP_Read(128, RX_data);
			HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG21XX, &bWriteData1, 1);    //cmd end
			for (i = 0; i < 128; i++)   //log out if verify error
			{
				if (RX_data[i] != 0xFF)
				{
					//TP_DEBUG(printk("k=%d,j=%d,i=%d===============erase not clean================",k,j,i);)
					printk("k=%d,j=%d,i=%d  erase not clean !!",k,j,i);
				}
			}
		}
	}
	TP_DEBUG("read finish\n");
	return sprintf(buf, "%s\n", fw_version);
}

static ssize_t firmware_clear_store(struct device *dev,
                                     struct device_attribute *attr, const char *buf, size_t size)
{

	u8 dbbus_tx_data[4];
	unsigned char dbbus_rx_data[2] = {0};
	printk(" +++++++ [%s] Enter!++++++\n", __func__);
	//msctpc_LoopDelay ( 100 ); 	   // delay about 100ms*****

	// Enable slave's ISP ECO mode

	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x08;
	dbbus_tx_data[2] = 0x0c;
	dbbus_tx_data[3] = 0x08;
	
	// Disable the Watchdog
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 4);

	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x11;
	dbbus_tx_data[2] = 0xE2;
	dbbus_tx_data[3] = 0x00;
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 4);

	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x3C;
	dbbus_tx_data[2] = 0x60;
	dbbus_tx_data[3] = 0x55;
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 4);

	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x3C;
	dbbus_tx_data[2] = 0x61;
	dbbus_tx_data[3] = 0xAA;
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 4);

	//Stop MCU
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x0F;
	dbbus_tx_data[2] = 0xE6;
	dbbus_tx_data[3] = 0x01;
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 4);

	//Enable SPI Pad
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x02;
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 3);
	HalTscrCReadI2CSeq(FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2);
	TP_DEBUG(printk("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);)
	dbbus_tx_data[3] = (dbbus_rx_data[0] | 0x20);  //Set Bit 5
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 4);

	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x25;
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 3);

	dbbus_rx_data[0] = 0;
	dbbus_rx_data[1] = 0;
	HalTscrCReadI2CSeq(FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2);
	TP_DEBUG(printk("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);)
	dbbus_tx_data[3] = dbbus_rx_data[0] & 0xFC;  //Clear Bit 1,0
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 4);

	//WP overwrite
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x0E;
	dbbus_tx_data[3] = 0x02;
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 4);


	//set pin high
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x10;
	dbbus_tx_data[3] = 0x08;
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 4);
	//set FRO to 50M
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x11;
	dbbus_tx_data[2] = 0xE2;
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 3);
	dbbus_rx_data[0] = 0;
	dbbus_rx_data[1] = 0;
	HalTscrCReadI2CSeq(FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2);
	TP_DEBUG(printk("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);)
	dbbus_tx_data[3] = dbbus_rx_data[0] & 0xF7;  //Clear Bit 1,0
	HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX, dbbus_tx_data, 4);

	dbbusDWIICIICNotUseBus();
	dbbusDWIICNotStopMCU();
	dbbusDWIICExitSerialDebugMode();

    ///////////////////////////////////////
    // Start to load firmware
    ///////////////////////////////////////
    drvISP_EntryIspMode();
	TP_DEBUG(printk("chip erase+\n");)
    drvISP_BlockErase(0x00000);
	TP_DEBUG(printk("chip erase-\n");)
    drvISP_ExitIspMode();
    return size;
}
static DEVICE_ATTR(clear, CTP_AUTHORITY, firmware_clear_show, firmware_clear_store);
#endif //0
/*test=================*/
/*Add by Tracy.Lin for update touch panel firmware and get fw version*/

static ssize_t firmware_version_show(struct device *dev,
                                     struct device_attribute *attr, char *buf)
{
    TP_DEBUG("*** firmware_version_show fw_version = %s***\n", fw_version);
    return sprintf(buf, "%s\n", fw_version);
}

static ssize_t firmware_version_store(struct device *dev,
                                      struct device_attribute *attr, const char *buf, size_t size)
{
    unsigned char dbbus_tx_data[3];
    unsigned char dbbus_rx_data[4] ;
    unsigned short major=0, minor=0;
/*
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();

*/
    fw_version = kzalloc(sizeof(char), GFP_KERNEL);

    //Get_Chip_Version();
    dbbus_tx_data[0] = 0x53;
    dbbus_tx_data[1] = 0x00;
    dbbus_tx_data[2] = 0x74;//2133A: 0x2A;  2133: 0x74
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX_TP, &dbbus_tx_data[0], 3);
    HalTscrCReadI2CSeq(FW_ADDR_MSG21XX_TP, &dbbus_rx_data[0], 4);

    major = (dbbus_rx_data[1]<<8)+dbbus_rx_data[0];
    minor = (dbbus_rx_data[3]<<8)+dbbus_rx_data[2];

    TP_DEBUG_ERR("***major = %d ***\n", major);
    TP_DEBUG_ERR("***minor = %d ***\n", minor);
    sprintf(fw_version,"%03d%03d", major, minor);
    //TP_DEBUG(printk("***fw_version = %s ***\n", fw_version);)

    return size;
}
static DEVICE_ATTR(version, CTP_AUTHORITY, firmware_version_show, firmware_version_store);

static ssize_t firmware_data_show(struct device *dev,
                                  struct device_attribute *attr, char *buf)
{
    return FwDataCnt;
}

static ssize_t firmware_data_store(struct device *dev,
                                   struct device_attribute *attr, const char *buf, size_t size)
{
    int i;
	TP_DEBUG_ERR("***FwDataCnt = %d ***\n", FwDataCnt);
   // for (i = 0; i < 1024; i++)
    {
        memcpy(temp[FwDataCnt], buf, 1024);
    }
    FwDataCnt++;
    return size;
}
static DEVICE_ATTR(data, CTP_AUTHORITY, firmware_data_show, firmware_data_store);
#ifdef ENABLE_AUTO_UPDATA
static unsigned char mstar_fw_array[]=
{
	#include "FW_AW290_1.5.i"
};

#define	CTP_ID_MSG21XX		0
#define	CTP_ID_MSG21XXA		1

unsigned char getchipType(void)
{
    u8 dbbus_tx_data[4];
    unsigned char dbbus_rx_data[2] = {0};
	
	_HalTscrHWReset();
    mdelay ( 100 );
    
	dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();
    mdelay ( 100 );

    // Disable the Watchdog
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x60;
    dbbus_tx_data[3] = 0x55;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x61;
    dbbus_tx_data[3] = 0xAA;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );
    // Stop MCU
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x0F;
    dbbus_tx_data[2] = 0xE6;
    dbbus_tx_data[3] = 0x01;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 4 );
    /////////////////////////
    // Difference between C2 and C3
    /////////////////////////
	// c2:2133 c32:2133a(2) c33:2138
    //check id
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0xCC;
    HalTscrCDevWriteI2CSeq ( FW_ADDR_MSG21XX, dbbus_tx_data, 3 );
    HalTscrCReadI2CSeq ( FW_ADDR_MSG21XX, &dbbus_rx_data[0], 2 );
    if ( dbbus_rx_data[0] == 2 )
    {
    	return CTP_ID_MSG21XXA;
    }
    else
    {
    	return CTP_ID_MSG21XX;
    }
    
}

unsigned int getFWPrivateVersion(void)
{
    unsigned char dbbus_tx_data[3];
    unsigned char dbbus_rx_data[4] ;
    unsigned short major=0, minor=0;

	_HalTscrHWReset();
    mdelay ( 100 );
    
	dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();
    mdelay ( 100 );
    
    dbbus_tx_data[0] = 0x53;
    dbbus_tx_data[1] = 0x00;
    dbbus_tx_data[2] = 0x2A;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG21XX_TP, &dbbus_tx_data[0], 3);
    HalTscrCReadI2CSeq(FW_ADDR_MSG21XX_TP, &dbbus_rx_data[0], 4);

    major = (dbbus_rx_data[1]<<8)+dbbus_rx_data[0];
    minor = (dbbus_rx_data[3]<<8)+dbbus_rx_data[2];

    TP_DEBUG_ERR("***FW Version major = %d ***\n", major);
    TP_DEBUG_ERR("***FW Version minor = %d ***\n", minor);
    
    _HalTscrHWReset();
    mdelay ( 100 );
    
    return ((major<<16)|(minor));
}
static int fwAutoUpdate(void *unused)
{
    firmware_update_store(NULL, NULL, NULL, 0);	
}
#endif

#endif  


static int check_i2c_read(u8 addr, u8 *pbt_buf, int dw_lenth)
{
	int ret;
	i2c_client->addr = addr;
	i2c_client->addr|=I2C_ENEXT_FLAG;
	ret = i2c_master_recv(i2c_client, pbt_buf, dw_lenth);
	i2c_client->addr = FW_ADDR_MSG21XX_TP;
	i2c_client->addr|=I2C_ENEXT_FLAG;

	return ret;
}



/////////////////////////////////////////////////////////////



#if 0
/*FW UPDATE*/
static int i2c_master_send_ext(struct i2c_client *client,const char *buf ,int count);
static int i2c_master_recv_ext(struct i2c_client *client, char *buf ,int count);


void HalTscrCDevWriteI2CSeq(u8 addr, u8* data, u16 size)
{
    u8 addr_bak;

    addr_bak = i2c_client->addr;
    i2c_client->addr = addr;
    i2c_client->addr |= I2C_ENEXT_FLAG;
    i2c_master_send_ext(i2c_client,data,size);
    i2c_client->addr = addr_bak;
}


void HalTscrCReadI2CSeq(u8 addr, u8* read_data, u8 size)
{
    u8 addr_bak;

    addr_bak = i2c_client->addr;
    i2c_client->addr = addr;
    i2c_client->addr |= I2C_ENEXT_FLAG;
    i2c_master_recv_ext(i2c_client,read_data,size);
    i2c_client->addr = addr_bak;
}


void Get_Chip_Version(void)
{

    unsigned char dbbus_tx_data[3];
    unsigned char dbbus_rx_data[2];

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0xCE;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, &dbbus_tx_data[0], 3);
    HalTscrCReadI2CSeq(FW_ADDR_MSG2133, &dbbus_rx_data[0], 2);
    if (dbbus_rx_data[1] == 0)
    {
        // it is Catch2
        DBG("*** Catch2 ***\n");
        //FwVersion  = 2;// 2 means Catch2
    }
    else
    {
        // it is catch1
        DBG("*** Catch1 ***\n");
        //FwVersion  = 1;// 1 means Catch1
    }

}

void dbbusDWIICEnterSerialDebugMode()
{
    U8 data[5];

    // Enter the Serial Debug Mode
    data[0] = 0x53;
    data[1] = 0x45;
    data[2] = 0x52;
    data[3] = 0x44;
    data[4] = 0x42;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, data, 5);
}

void dbbusDWIICStopMCU()
{
    U8 data[1];

    // Stop the MCU
    data[0] = 0x37;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, data, 1);
}

void dbbusDWIICIICUseBus()
{
    U8 data[1];

    // IIC Use Bus
    data[0] = 0x35;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, data, 1);
}

void dbbusDWIICIICReshape()
{
    U8 data[1];

    // IIC Re-shape
    data[0] = 0x71;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, data, 1);
}

void dbbusDWIICIICNotUseBus()
{
    U8 data[1];

    // IIC Not Use Bus
    data[0] = 0x34;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, data, 1);
}

void dbbusDWIICNotStopMCU()
{
    U8 data[1];

    // Not Stop the MCU
    data[0] = 0x36;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, data, 1);
}

void dbbusDWIICExitSerialDebugMode()
{
    U8 data[1];

    // Exit the Serial Debug Mode
    data[0] = 0x45;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, data, 1);

    // Delay some interval to guard the next transaction
    //udelay ( 200 );        // delay about 0.2ms
}

void drvISP_EntryIspMode(void)
{
    U8 bWriteData[5] =
    {
        0x4D, 0x53, 0x54, 0x41, 0x52
    };

    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, bWriteData, 5);

    //udelay ( 1000 );        // delay about 1ms
}

U8 drvISP_Read(U8 n, U8* pDataToRead)    //First it needs send 0x11 to notify we want to get flash data back.
{
    U8 Read_cmd = 0x11;
    unsigned char dbbus_rx_data[2] = {0};
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, &Read_cmd, 1);
    udelay(100);//10 zzf
    if (n == 1)
    {
        HalTscrCReadI2CSeq(FW_UPDATE_ADDR_MSG2133, &dbbus_rx_data[0], 2);
        *pDataToRead = dbbus_rx_data[0];
    }
    else
        HalTscrCReadI2CSeq(FW_UPDATE_ADDR_MSG2133, pDataToRead, n);

}

void drvISP_WriteEnable(void)
{
    U8 bWriteData[2] =
    {
        0x10, 0x06
    };
    U8 bWriteData1 = 0x12;
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, bWriteData, 2);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, &bWriteData1, 1);
}


void drvISP_ExitIspMode(void)
{
    U8 bWriteData = 0x24;
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, &bWriteData, 1);
}

U8 drvISP_ReadStatus()
{
    U8 bReadData = 0;
    U8 bWriteData[2] =
    {
        0x10, 0x05
    };
    U8 bWriteData1 = 0x12;

    udelay(100);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, bWriteData, 2);
    udelay(100);
    drvISP_Read(1, &bReadData);   
    mdelay(10);//3->10 zzf
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, &bWriteData1, 1);
    return bReadData;
}


void drvISP_BlockErase(U32 addr)
{
    U8 bWriteData[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
    U8 bWriteData1 = 0x12;
    u32 timeOutCount=0;
    drvISP_WriteEnable();

    //Enable write status register
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x50;
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, bWriteData, 2);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, &bWriteData1, 1);

    //Write Status
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x01;
    bWriteData[2] = 0x00;
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, bWriteData, 3);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, &bWriteData1, 1);

    //Write disable
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x04;
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, bWriteData, 2);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, &bWriteData1, 1);

    udelay(100);
    timeOutCount=0;
    while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
    {
           timeOutCount++;
         if ( timeOutCount >= 10000 ) 
            break; /* around 1 sec timeout */
    }
    drvISP_WriteEnable();

    drvISP_ReadStatus();//zzf

    bWriteData[0] = 0x10;
    bWriteData[1] = 0xc7;//0xD8;        //Block Erase
    //bWriteData[2] = ((addr >> 16) & 0xFF) ;
    //bWriteData[3] = ((addr >> 8) & 0xFF) ;
    //bWriteData[4] = (addr & 0xFF) ;
    //HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, &bWriteData, 5);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, bWriteData, 2);
    HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, &bWriteData1, 1);

    udelay(100);
    timeOutCount=0;
    while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
    {
        timeOutCount++;
        if ( timeOutCount >= 50000 ) 
            break; /* around 5 sec timeout */
    }
}

void drvISP_Program(U16 k, U8* pDataToWrite)
{
    U16 i = 0;
    U16 j = 0;
    //U16 n = 0;
    U8 TX_data[133];
    U8 bWriteData1 = 0x12;
    U32 addr = k * 1024;
    U32 timeOutCount = 0;

    for (j = 0; j < 8; j++)   //256*4 cycle
    {
        TX_data[0] = 0x10;
        TX_data[1] = 0x02;// Page Program CMD
        TX_data[2] = (addr + 128 * j) >> 16;
        TX_data[3] = (addr + 128 * j) >> 8;
        TX_data[4] = (addr + 128 * j);
        for (i = 0; i < 128; i++)
        {
            TX_data[5 + i] = pDataToWrite[j * 128 + i];
        }

    udelay(100);
    timeOutCount=0;
    while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
    {
           timeOutCount++;
         if ( timeOutCount >= 10000 ) 
            break; /* around 1 sec timeout */
    }

        drvISP_WriteEnable();
        HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, TX_data, 133);   //write 256 byte per cycle
        HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, &bWriteData1, 1);
    }
}


int drvISP_Verify(U16 k, U8* pDataToVerify)
{
    U16 i = 0, j = 0, m = 0;
    U8 bWriteData[5] =
    {
        0x10, 0x03, 0, 0, 0
    };
    U8 RX_data[256];
    U8 bWriteData1 = 0x12;
    U32 addr = k * 1024;
    U32 timeOutCount = 0;
    u8 index=0;
    int result = -1;

    for (j = 0; j < 8; j++)   //128*8 cycle
    {
        bWriteData[2] = (U8)((addr + j * 128) >> 16);
        bWriteData[3] = (U8)((addr + j * 128) >> 8);
        bWriteData[4] = (U8)(addr + j * 128);
        //while ((drvISP_ReadStatus() & 0x01) == 0x01);     //wait until not in write operation
        udelay(100);
        timeOutCount=0;
        while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
        {
               timeOutCount++;
             if ( timeOutCount >= 100000 ) 
                break; /* around 1 sec timeout */
        }

        HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, bWriteData, 5);    //write read flash addr
        udelay(100);
        drvISP_Read(128, &RX_data);
        HalTscrCDevWriteI2CSeq(FW_UPDATE_ADDR_MSG2133, &bWriteData1, 1);    //cmd end
        for (i = 0; i < 128; i++)   //log out if verify error
        {
            if((RX_data[i]!=0)&&index<10)
            {
            //TP_DEBUG("j=%d,RX_data[%d]=0x%x\n",j,i,RX_data[i]);
                index++;
            }
            if (RX_data[i] != pDataToVerify[128 * j + i])
            {
                printk("k=%d,j=%d,i=%d===============Update Firmware Error================",k,j,i);
                result = i;
                break;
            }
        }
    }

    return result;
}

static ssize_t firmware_update_show(struct device *dev,
                                    struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%s\n", fw_version);
}


static void tpd_hw_enable(unsigned int on);
static void tpd_hw_power(unsigned int on);
static ssize_t firmware_update_store(struct device *dev,
                                     struct device_attribute *attr, const char *buf, size_t size)
{
    U8 i;
    U8 dbbus_tx_data[4];
    unsigned char dbbus_rx_data[2] = {0};
    int fd;
    int count = 0;
    //msctpc_LoopDelay ( 100 );        // delay about 100ms*****
    //add mask interrupt
    mt65xx_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);
    mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY,          NULL, 1);

    tpd_hw_power(0);
    mdelay(10);
    tpd_hw_power(1);
    mdelay(300);
    drvISP_EntryIspMode();
    drvISP_BlockErase(0x00000);
    mdelay(300);
    tpd_hw_power(0);
    mdelay(10);
    tpd_hw_power(1);

    count = 0;
    mdelay(300);

#if 1
    // Enable slave's ISP ECO mode
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x08;
    dbbus_tx_data[2] = 0x0c;
    dbbus_tx_data[3] = 0x08;

    // Disable the Watchdog
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);


    //Get_Chip_Version();

        dbbus_tx_data[0] = 0x10;
        dbbus_tx_data[1] = 0x11;
        dbbus_tx_data[2] = 0xE2;
        dbbus_tx_data[3] = 0x00;

        HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x60;
    dbbus_tx_data[3] = 0x55;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x61;
    dbbus_tx_data[3] = 0xAA;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

//Stop MCU
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x0F;
    dbbus_tx_data[2] = 0xE6;
    dbbus_tx_data[3] = 0x01;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

//Enable SPI Pad
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x02;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 3);
    HalTscrCReadI2CSeq(FW_ADDR_MSG2133, &dbbus_rx_data[0], 2);
    DBG("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
    dbbus_tx_data[3] = (dbbus_rx_data[0] | 0x20);  //Set Bit 5
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x25;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 3);
    dbbus_rx_data[0] = 0;
    dbbus_rx_data[1] = 0;
    HalTscrCReadI2CSeq(FW_ADDR_MSG2133, &dbbus_rx_data[0], 2);
    DBG("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
    dbbus_tx_data[3] = dbbus_rx_data[0] & 0xFC;  //Clear Bit 1,0
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

//WP overwrite
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x0E;
    dbbus_tx_data[3] = 0x02;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

//set pin high
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x10;
    dbbus_tx_data[3] = 0x08;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

//set FRO to 50M
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x11;
    dbbus_tx_data[2] = 0xE2;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 3);
    dbbus_rx_data[0] = 0;
    dbbus_rx_data[1] = 0;
    HalTscrCReadI2CSeq(FW_ADDR_MSG2133, &dbbus_rx_data[0], 2);
    printk("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
    dbbus_tx_data[3] = dbbus_rx_data[0] & 0xF7;  //Clear Bit 1,0
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

    dbbusDWIICIICNotUseBus();
    dbbusDWIICNotStopMCU();
    dbbusDWIICExitSerialDebugMode();

    ///////////////////////////////////////
    // Start to load firmware
    ///////////////////////////////////////
    drvISP_EntryIspMode();
    //drvISP_BlockErase(0x00000);//////zzf 
        //DBG("FwVersion=2");
    for (i = 0; i < 94; i++)   // total  94 KB : 1 byte per R/W
    {
        drvISP_Program(i, temp[i]);    // program to slave's flash
#if 0
        if(drvISP_Verify ( i, temp[i] ) != -1 && count < 3)//; //verify data
        {
            count++;
            if(count == 3)
            {
                count = 0;
                continue;
            }
            i--;
        }
        else
        {
            count = 0;
        }
#endif
        mdelay(1);
    }
#endif
    drvISP_ExitIspMode();
    FwDataCnt = 0;

    mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY, tpd_eint_interrupt_handler,1);
    mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);


    return size;
}

static void masterBUT_LoadFwToTarget(u8 *pData)
{
    U8 i;
    U8 dbbus_tx_data[4];
    unsigned char dbbus_rx_data[2] = {0};
    int fd;
    int count = 0;
    //msctpc_LoopDelay ( 100 );        // delay about 100ms*****
    //add mask interrupt
    mt65xx_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);
    mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY,          NULL, 0);

    tpd_hw_power(0);
    mdelay(10);
    tpd_hw_power(1);
    tpd_hw_enable(1);
    mdelay(300);
    drvISP_EntryIspMode();
    drvISP_BlockErase(0x00000);
    mdelay(300);
    tpd_hw_power(0);
    mdelay(10);
    tpd_hw_power(1);
    tpd_hw_enable(1);

    count = 0;
    mdelay(300);

#if 1
    // Enable slave's ISP ECO mode
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x08;
    dbbus_tx_data[2] = 0x0c;
    dbbus_tx_data[3] = 0x08;

    // Disable the Watchdog
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);


    //Get_Chip_Version();

        dbbus_tx_data[0] = 0x10;
        dbbus_tx_data[1] = 0x11;
        dbbus_tx_data[2] = 0xE2;
        dbbus_tx_data[3] = 0x00;

        HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x60;
    dbbus_tx_data[3] = 0x55;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x61;
    dbbus_tx_data[3] = 0xAA;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

//Stop MCU
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x0F;
    dbbus_tx_data[2] = 0xE6;
    dbbus_tx_data[3] = 0x01;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

//Enable SPI Pad
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x02;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 3);
    HalTscrCReadI2CSeq(FW_ADDR_MSG2133, &dbbus_rx_data[0], 2);
    DBG("+++lcm+++dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
    dbbus_tx_data[3] = (dbbus_rx_data[0] | 0x20);  //Set Bit 5
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x25;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 3);
    dbbus_rx_data[0] = 0;
    dbbus_rx_data[1] = 0;
    HalTscrCReadI2CSeq(FW_ADDR_MSG2133, &dbbus_rx_data[0], 2);
    DBG("+++lcm+++dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
    dbbus_tx_data[3] = dbbus_rx_data[0] & 0xFC;  //Clear Bit 1,0
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

//WP overwrite
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x0E;
    dbbus_tx_data[3] = 0x02;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

//set pin high
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0x10;
    dbbus_tx_data[3] = 0x08;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

//set FRO to 50M
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x11;
    dbbus_tx_data[2] = 0xE2;
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 3);
    dbbus_rx_data[0] = 0;
    dbbus_rx_data[1] = 0;
    HalTscrCReadI2CSeq(FW_ADDR_MSG2133, &dbbus_rx_data[0], 2);
    printk("+++lcm+++dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
    dbbus_tx_data[3] = dbbus_rx_data[0] & 0xF7;  //Clear Bit 1,0
    HalTscrCDevWriteI2CSeq(FW_ADDR_MSG2133, dbbus_tx_data, 4);

    dbbusDWIICIICNotUseBus();
    dbbusDWIICNotStopMCU();
    dbbusDWIICExitSerialDebugMode();

    ///////////////////////////////////////
    // Start to load firmware
    ///////////////////////////////////////
    drvISP_EntryIspMode();
    //drvISP_BlockErase(0x00000);//////zzf 
        //DBG("FwVersion=2");
    for (i = 0; i < 94; i++)   // total  94 KB : 1 byte per R/W
    {
        printk("+++lcm+++dbbus_rx_data[0]=0x%,i=%d\n", dbbus_rx_data[0],i);
        drvISP_Program(i, &pData[i*1024]);    // program to slave's flash
        mdelay(1);
    }
#endif
    drvISP_ExitIspMode();
    FwDataCnt = 0;

    mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY, tpd_eint_interrupt_handler,1);
    mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);

}

static DEVICE_ATTR(update, 0777, firmware_update_show, firmware_update_store);

/*Add by Tracy.Lin for update touch panel firmware and get fw version*/

static ssize_t firmware_version_show(struct device *dev,
                                     struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%s\n", fw_version);
}

static ssize_t firmware_version_store(struct device *dev,
                                      struct device_attribute *attr, const char *buf, size_t size)
{

    //dbbusDWIICEnterSerialDebugMode();
    //dbbusDWIICStopMCU();
    //dbbusDWIICIICUseBus();
    //dbbusDWIICIICReshape();

    unsigned char dbbus_tx_data[3];
    unsigned char dbbus_rx_data[4] ;
    unsigned short major=0, minor=0;
    fw_version = kzalloc(sizeof(char), GFP_KERNEL);

    Get_Chip_Version();
    dbbus_tx_data[0] = 0x53;
    dbbus_tx_data[1] = 0x00;
    dbbus_tx_data[2] = 0x74;
    HalTscrCDevWriteI2CSeq(i2c_client->addr, &dbbus_tx_data[0], 3);
    HalTscrCReadI2CSeq(i2c_client->addr, &dbbus_rx_data[0], 4);
    major = (dbbus_rx_data[1]<<8)+dbbus_rx_data[0];
    minor = (dbbus_rx_data[3]<<8)+dbbus_rx_data[2];
    DBG("***major = %d ***\n", major);
    DBG("***minor = %d ***\n", minor);
    sprintf(fw_version,"%03d%03d", major, minor);
    DBG("***fw_version = %s ***\n", fw_version);

    return size;
    
}
static DEVICE_ATTR(version, 0777, firmware_version_show, firmware_version_store);

static ssize_t firmware_data_show(struct device *dev,
                                  struct device_attribute *attr, char *buf)
{
    return FwDataCnt;
}

static ssize_t firmware_data_store(struct device *dev,
                                   struct device_attribute *attr, const char *buf, size_t size)
{
    DBG("***FwDataCnt = %d ***\n", FwDataCnt);
    int i;
    //for (i = 0; i < 1024; i++)
    {
        memcpy(temp[FwDataCnt], buf, 1024);
    }
    FwDataCnt++;
    return size;
}
static DEVICE_ATTR(data, 0777, firmware_data_show, firmware_data_store);



#endif 



/*ergate-001 start*/
static void tpd_hw_power(unsigned int on)
{
#if  1 //
    if(on)
    {
		hwPowerOn(TPD_POWER_SOURCE_CUSTOM, VOL_2800, "TP");
    }
    else
    {
    	hwPowerDown(TPD_POWER_SOURCE_CUSTOM, "TP");
    }
    msleep(30);
#else
    mt_set_gpio_mode(GPIO_CTP_EN_PIN,GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_CTP_EN_PIN,GPIO_DIR_OUT);
    if(on)
    {
        mt_set_gpio_out(GPIO_CTP_EN_PIN,GPIO_OUT_ONE);
    }
    else
    {
        mt_set_gpio_out(GPIO_CTP_EN_PIN,GPIO_OUT_ZERO);
    }
#endif
}


static void tpd_hw_reset(void)
{
}


static void tpd_hw_enable(unsigned int on)
{
    mt_set_gpio_mode(GPIO_CTP_RST_PIN,GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN,GPIO_DIR_OUT);
    mdelay(10);
    if(on)
    {
        mt_set_gpio_out(GPIO_CTP_RST_PIN,GPIO_OUT_ONE);
    }
    else
    {
        mt_set_gpio_out(GPIO_CTP_RST_PIN,GPIO_OUT_ZERO);
    }
    mdelay(50);
}


////////////////////////////////////////////////////////////////////////////////////////////add by lan
#ifdef TPD_PROXIMITY
#if 0
void i2c_write(u8 addr, u8* data, u16 size)
{
	u8 addr_bak;

	addr_bak = i2c_client->addr;
	i2c_client->addr = addr;
	i2c_client->addr |= I2C_ENEXT_FLAG;
	i2c_master_send_ext(i2c_client,data,size);
	i2c_client->addr = addr_bak;
}
#endif
int tpd_read_ps(void)
{
	tpd_proximity_detect;
	return 0;    
}

static int tpd_get_ps_value(void)
{
	return tpd_proximity_detect;
}

static int tpd_enable_ps(int enable)
{
	u8 ps_store_data[4];
	if (enable)
	{
		ps_store_data[0] = 0x52;
		ps_store_data[1] = 0x00;
		ps_store_data[2] = 0x62;
		ps_store_data[3] = 0xa0;//0xa2//0xa4
		tpd_proximity_flag = 1;
		TPD_PROXIMITY_DEBUG("xxxxx mycat tpd_enable_ps function is on\n");
	}
	else
	{
		ps_store_data[0] = 0x52;
		ps_store_data[1] = 0x00;
		ps_store_data[2] = 0x62;
		ps_store_data[3] = 0xa1;
		tpd_proximity_flag = 0;
		TPD_PROXIMITY_DEBUG("xxxxx mycat tpd_enable_ps function is off\n");
	}
	i2c_write(0x26, &ps_store_data[0], 4);
	return 0;
}


static ssize_t show_proximity_sensor(struct device *dev, struct device_attribute *attr, char *buf)
{
	static char temp=2;
	int err = 0;
if((err = tpd_read_ps()))
{
	err = -1;
	return err;
}
else
{
       if (buf != NULL)
	    *buf = tpd_get_ps_value();
    if(temp!=*buf)
    {
    printk("xxxxx mycat proximity_sensor_show: buf=%d\n\n", *buf);
    temp=*buf;
    }
	return 1;
}

}

static ssize_t store_proximity_sensor(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{

    if(buf != NULL && size != 0)
    {
        if(DISABLE_CTP_PS == *buf)
        {
        
		if((tpd_enable_ps(0) != 0))
			{
			APS_ERR("enable ps fail: %d\n"); 
			return -1;
			}

        }
        else if(ENABLE_CTP_PS == *buf)
        {
        
		if((tpd_enable_ps(1) != 0))
			{
			APS_ERR("disable ps fail: %d\n"); 
			return -1;
			}
		
        }


	printk("xxxxx mycat tpd_proximity_state_on_off: buf=%d\n\n", *buf);
    }

    return size;
}
static DEVICE_ATTR(proximity_sensor, 0777, show_proximity_sensor, store_proximity_sensor);


#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*
mode:
0: output 0
1: output 1
2: eint
*/
static void tpd_hw_config_eint_pin(int mode)
{
    switch(mode)
    {
    case 0:
        mt_set_gpio_mode(GPIO_CTP_EINT_PIN,GPIO_MODE_00);
        mt_set_gpio_dir(GPIO_CTP_EINT_PIN,GPIO_DIR_OUT);
        mt_set_gpio_out(GPIO_CTP_EINT_PIN,GPIO_OUT_ZERO);
        break;
    case 1:
        mt_set_gpio_mode(GPIO_CTP_EINT_PIN,GPIO_MODE_00);
        mt_set_gpio_dir(GPIO_CTP_EINT_PIN,GPIO_DIR_OUT);
        mt_set_gpio_out(GPIO_CTP_EINT_PIN,GPIO_OUT_ONE);
        break;
    case 2:
       // mt_set_gpio_mode(GPIO_CAPTOUCH_EINT_PIN, GPIO_CAPTOUCH_EINT_PIN_M_EINT);
      //  mt_set_gpio_dir(GPIO_CAPTOUCH_EINT_PIN, GPIO_DIR_IN);
      //  mt_set_gpio_pull_enable(GPIO_CAPTOUCH_EINT_PIN, GPIO_PULL_ENABLE);
      //  mt_set_gpio_pull_select(GPIO_CAPTOUCH_EINT_PIN, GPIO_PULL_UP);


		mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_EINT);
		mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
		//mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_DISABLE);
		mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_ENABLE);
		mt_set_gpio_pull_select(GPIO_CTP_EINT_PIN, GPIO_PULL_UP);
		
        break;
    default:
        break;          
    }
}
/*ergate-001 end*/


#if 1
/*ergate-009 start*/
static ssize_t show_calibrate(struct device_driver *ddri, char *buf);
static ssize_t store_calibrate(struct device_driver *ddri, char *buf, size_t count);
static DRIVER_ATTR(calibrate,   S_IWUSR | S_IRUGO, show_calibrate,      store_calibrate);


static int tpd_hw_calibrate(void)
{
    int retval = TPD_OK;

    return 0;
}

static ssize_t show_calibrate(struct device_driver *ddri, char *buf)
{
    TPD_LOGI("tpd show_calibrate\n");

    snprintf(buf,PAGE_SIZE,"%s\n","tpd show_calibrate");
}


static ssize_t store_calibrate(struct device_driver *ddri, char *buf, size_t count)
{
    int retval = TPD_OK;
    TPD_LOGI("tpd store_calibrate:%s\n",buf);
    if(!strncmp(buf,"cali",4))
    {
        TPD_LOGI("tpd begin calibration...\n");
        retval = tpd_hw_calibrate();
        TPD_LOGI("tpd end calibration=%d\n",retval);
    }
    else
    {
        TPD_LOGI("Invalid format\n");
    }
}
/*ergate-009 end*/
#endif


/*ergate-013 start*/
#define MAX_CMD_LEN 255
static int i2c_master_send_ext(struct i2c_client *client,const char *buf ,int count)
{
#if 0
    u32 phyAddr = 0; 
    u8 *buf_dma = NULL;
    u32 old_addr = 0; 
    int ret = 0; 
    int retry = 3; 
    
    if (count > MAX_CMD_LEN) {
        TPD_LOGI("[i2c_master_send_ext] exceed the max write length \n"); 
        return -1; 
    }

    phyAddr = 0; 
    buf_dma = dma_alloc_coherent(0, count, &phyAddr, GFP_KERNEL);
    if (NULL == buf_dma) {
        TPD_LOGI("[i2c_master_send_ext] Not enough memory \n"); 
        return -1; 
    }
    memcpy(buf_dma, buf, count); 
    old_addr = client->addr; 
    //client->addr = ( (client->addr &  I2C_MASK_FLAG) | I2C_DMA_FLAG ); 
    client->addr = ( client->addr | I2C_DMA_FLAG ); //save flag I2C_ENEXT_FLAG

#if 0
    int i =0; 
    for (i = 0; i < count; i++) {
        TPD_LOGV("0x%02x ", buf_dma[i]); 
    }
#endif     

    do {
        ret = i2c_master_send(client, (u8*)phyAddr, count);     
        retry --; 
        if (ret != count) {
            TPD_LOGI("[i2c_master_send_ext] Error sent I2C ret = %d\n", ret); 
        }
    }while ((ret != count) && (retry > 0)); 

    dma_free_coherent(0, count, buf_dma, phyAddr);

    client->addr = old_addr; 

    return ret; 
#endif
    u8 *buf_dma = NULL;
    u32 old_addr = 0; 
    int ret = 0; 
    int retry = 3; 

    if (count > MAX_CMD_LEN) {
        printk("[i2c_master_send_ext] exceed the max write length \n"); 
        return -1; 
    }
    
    buf_dma= kmalloc(count,GFP_KERNEL);
    
    old_addr = client->addr; 
    client->addr |= I2C_ENEXT_FLAG ; 
    
    memcpy(buf_dma, buf, count); 
    
    do {
        ret = i2c_master_send(client, (u8*)buf_dma, count);     
        retry --; 
        if (ret != count) {
            printk("MYCAT [i2c_master_send_ext] Error sent I2C ret = %d\n", ret); 
        }
    }while ((ret != count) && (retry > 0)); 

    client->addr = old_addr; 
    kfree(buf_dma);
    
    return ret; 

}


static int i2c_master_recv_ext(struct i2c_client *client, char *buf ,int count)
{
    u32 phyAddr = 0; 
    u8  buf_dma[8] = {0};
    u32 old_addr = 0; 
    int ret = 0; 
    int retry = 3; 
    int i = 0; 
   // u8  *buf_test ;
//  buf_test = &buf_dma[0];

    old_addr = client->addr; 
    client->addr |= I2C_ENEXT_FLAG ;
    
    //printk("MYCAT [i2c_master_recv_ext] client->addr = %x\n", client->addr); 

    do {
        ret = i2c_master_recv(client, buf_dma, count);   
        retry --; 
        if (ret != count) {
            printk("MYCAT [i2c_master_recv_ext] Error sent I2C ret = %d\n", ret); 
        }
    }while ((ret != count) && (retry > 0)); 
    
    memcpy(buf, buf_dma, count); 
    
    client->addr = old_addr; 

    return ret; 
}
/*ergate-013 end*/


static int tpd_switch_mode(U8 mode)
{
    int retval = TPD_OK;
    struct TpdPacketT packet = 
    {
        TPD_PACKET_HEAD_CMD,
        TPD_PACKET_CMD_MODE_WRITE,
        0x00,
        mode
    };
 
    retval = i2c_master_send(i2c_client, &packet, sizeof(packet));
    msleep(5);
    if(mode != TPD_MODE_FREEZE)
        retval = i2c_master_recv(i2c_client, &packet, sizeof(packet));

    if(retval < TPD_OK)
        return -1;

    return 0;
}


static void tpd_down(int x, int y, int p)
{
    input_report_abs(tpd->dev, ABS_PRESSURE, p);
    input_report_key(tpd->dev, BTN_TOUCH, 1);
    //input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, p);
    input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
    input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
    TPD_LOGV("######tpd_down[%4d %4d %4d] ", x, y, p);
    input_mt_sync(tpd->dev);
    TPD_DOWN_DEBUG_TRACK(x,y);
}

#if 1//#if defined(HQ_TP_IMPROVE)
static int tpd_up(int x, int y,int p)
{
    input_report_abs(tpd->dev, ABS_PRESSURE, 0);
    input_report_key(tpd->dev, BTN_TOUCH, 0);
    input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 0);
    input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
    input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
    TPD_LOGV("######UP[%4d %4d %4d] ", x, y, p);
    input_mt_sync(tpd->dev);
    TPD_UP_DEBUG_TRACK(x,y);
}
#else
static int tpd_up(int x, int y,int *count)
{
    if(*count>0) 
    {
        input_report_abs(tpd->dev, ABS_PRESSURE, 0);
        input_report_key(tpd->dev, BTN_TOUCH, 0);
        input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 0);
        input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
        input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
        //TPD_LOGV("U[%4d %4d %4d] ", x, y, 0);
        input_mt_sync(tpd->dev);
        TPD_UP_DEBUG_TRACK(x,y);
        (*count)--;
        return 1;
    } 
    return 0;
}
#endif
#if defined(HQ_MSG2133_PS_SUPPORT)
static bool ps_state =true;
#endif
static int tpd_touchinfo(struct TouchInfoT *cinfo, struct TouchInfoT *pinfo)
{
    u32 retval;
	int x=0,y=0;
    u8 key,ii;
    u8 touchData = 0;
	u32 bitmask;
	
#ifdef TPD_PROXIMITY
	int err;
	hwm_sensor_data sensor_data;
#endif
	
   // printk("MYCAT tpd_touchinfo\n");
    //pinfo->count = cinfo->count;
    memcpy(pinfo, cinfo, sizeof(struct TouchInfoT));
    
    //add for sure addr correct
    //i2c_client->addr = 0x4c;
    retval = i2c_master_recv_ext(i2c_client, (u8 *)&TpdTouchData, sizeof(TpdTouchData));
#if defined(HQ_MSG2133_PS_SUPPORT)
//    printk("[zzf ctp ps]TpdTouchData.disx_l=%x\n",TpdTouchData.disx_l );
    if((TpdTouchData.packet_id==0x52)&&(TpdTouchData.x_h_y_h==0xff)&&(TpdTouchData.x_l==0xff)&&(TpdTouchData.y_l==0xff)&&(TpdTouchData.disx_h_disy_h==0xff)&&(TpdTouchData.disx_l==0x80)&&(TpdTouchData.disy_l==0xff))
        {
          printk("[zzf ctp ps] closd lcm \n"); 
          ps_state=0;
           return 1;
      }
     if((TpdTouchData.packet_id==0x52)&&(TpdTouchData.x_h_y_h==0xff)&&(TpdTouchData.x_l==0xff)&&(TpdTouchData.y_l==0xff)&&(TpdTouchData.disx_h_disy_h==0xff)&&(TpdTouchData.disx_l==0x40)&&(TpdTouchData.disy_l==0xff))
        {
            printk("[zzf ctp ps] open lcm \n");
            ps_state=1;
			      return 1;
        }
#endif
    if(TpdTouchData.packet_id != 0x52 )
       {
        return 0;
       }
    /*touch*/
    if(TpdTouchData.packet_id == 0x52)
    {
    
        if(TpdTouchData.x_h_y_h == 0xFF 
            && TpdTouchData.x_l == 0xFF 
            && TpdTouchData.y_l == 0xFF 
            && TpdTouchData.disx_h_disy_h == 0xFF 
          )
        {
#ifdef TPD_HAVE_BUTTON
         {
         
            U8 *p = &TpdTouchData;
            cinfo->key_value = 0;
            cinfo->key_value = *(p+5);          
            TPD_LOGV("+++++++zym+++++++TPD_HAVE_BUTTON:(%d)\n",cinfo->key_value);
			//printk("MYCAT TPD_HAVE_BUTTON:(%d)\n",cinfo->key_value);
            {
                //tpd_button_msg213x(cinfo->key_value);
                //tpd_button(0,0,cinfo->key_value);
                if(cinfo->key_value == 1)
                {
                    //touchData = tpd_keys_local[0];//KEY_BACK;
                    x  = tpd_keys_dim_local[0][0];
		    y =  tpd_keys_dim_local[0][1];
                }
                else if(cinfo->key_value == 2)
                {
                    //touchData = tpd_keys_local[1];//KEY_HOMEPAGE;
                    x  = tpd_keys_dim_local[1][0];
		    y =  tpd_keys_dim_local[1][1];
                }
                else if(cinfo->key_value == 4)
                {
                    //touchData = tpd_keys_local[2];//KEY_MENU;
                    x  = tpd_keys_dim_local[2][0];
		    y =  tpd_keys_dim_local[2][1];
                }
                else if(cinfo->key_value == 8)
                {
                    //touchData = KEY_SEARCH;
                    x  = tpd_keys_dim_local[3][0];
		    y =  tpd_keys_dim_local[3][1];
                }
                else
                {
                    touchData = 0;
                    x = 0;
                    y = 0;
                }
                //printk("MYCAT TP KEY:(%d)\n",touchData);
                if(touchData ||x||y)
                {
                    input_report_abs(tpd->dev, ABS_PRESSURE, 1);
                    //input_report_abs(tpd->dev, BTN_TOUCH, 1);
                    //input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 1);
                    tpd_down(x, y, 1);
                    //input_report_key(tpd->dev,touchData,1);
                    //input_mt_sync(tpd->dev);
                    input_sync(tpd->dev);
                }
                else
                {/*
                    input_report_key(tpd->dev,KEY_MENU,0);
                    input_report_key(tpd->dev,KEY_HOMEPAGE,0);
                    input_report_key(tpd->dev,KEY_BACK,0);
                    input_report_key(tpd->dev,KEY_SEARCH,0);
                    */
                    //input_report_abs(tpd->dev, ABS_MT_PRESSURE, 0);
                    //input_report_abs(tpd->dev, BTN_TOUCH, 0);
                    //input_mt_sync(tpd->dev);
                    tpd_up(x, y,1);
                    input_sync(tpd->dev);
                }
            }
            
         }

#endif
            cinfo->count = 0;
            
///////////////////////////////////////////////////////////////////////////add by lan
#ifdef TPD_PROXIMITY
TPD_PROXIMITY_DEBUG("mycat tpd_touchinfo reg_val[5] = %d\n", TpdTouchData.disx_l);

				if (tpd_proximity_flag == 1)
				{
					if(TpdTouchData.disx_l==0x40||TpdTouchData.disx_l==0x80)
					{
						if (TpdTouchData.disx_l==0x40)// leave
						{
							tpd_proximity_detect = 1;
						}
						else if (TpdTouchData.disx_l==0x80)// close to
						{
							tpd_proximity_detect = 0;
						}
						//get raw data
						TPD_PROXIMITY_DEBUG(" ps change\n");
						//map and store data to hwm_sensor_data
						sensor_data.values[0] = tpd_get_ps_value();
						sensor_data.value_divide = 1;
						sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;
						//let up layer to know
						if((err = hwmsen_get_interrupt_data(ID_PROXIMITY, &sensor_data)))
						{
							TPD_PROXIMITY_DEBUG("call hwmsen_get_interrupt_data fail = %d\n", err);
						}
						return  false;
					}
					else if(TpdTouchData.disx_l==0xC0)
					{
						tpd_enable_ps(1);
						return  false;
					}
				}
#endif
            
            
		return 1;
        }
        else if(TpdTouchData.disx_h_disy_h == 0
            && TpdTouchData.disx_l == 0 
            && TpdTouchData.disy_l == 0)
            cinfo->count = 1;
        else
            cinfo->count = 2;
        
        TPD_LOGV("cinfo: count=%d\n",cinfo->count);
        if(cinfo->count > 0)
        {
            int tmp_x,tmp_y;
            /*point1*/
            cinfo->x1 = (((TpdTouchData.x_h_y_h&0xF0)<<4) | (TpdTouchData.x_l));
            cinfo->y1 = (((TpdTouchData.x_h_y_h&0x0F)<<8) | (TpdTouchData.y_l));
            TPD_LOGV("+++zym+++(%3d,%3d)\n",cinfo->x1,cinfo->y1);
			//printk("MYCAT (%3d,%3d)\n",cinfo->x1,cinfo->y1);

            if(cinfo->count >1)
            {   
                /*point2*/
                short disx,disy;
    
                disx = (((TpdTouchData.disx_h_disy_h&0xF0)<<4) | (TpdTouchData.disx_l));
                disy = (((TpdTouchData.disx_h_disy_h&0x0F)<<8) | (TpdTouchData.disy_l));
                disy = (disy<<4);
                disy = disy/16;
                if(disx >= 2048)
                    disx -= 4096;
                if(disy >= 2048)
                    disy -= 4096;
                cinfo->x2 = cinfo->x1 + disx;
                cinfo->y2 = cinfo->y1 + disy;               

                tmp_x = cinfo->x2;
                tmp_y = cinfo->y2;
                cinfo->x2 = tmp_x * (TPD_RES_X)/ 2047;  //
                cinfo->y2 = tmp_y * (TPD_RES_Y) / 2047;   //  
            }
            tmp_x = cinfo->x1;
            tmp_y = cinfo->y1;
            cinfo->x1 = tmp_x * (TPD_RES_X ) / 2047;//
            cinfo->y1 = tmp_y * (TPD_RES_Y) / 2047;//
            
            //add by zym 2012-04-16
           // cinfo->x1 = TPD_RES_X -1 - cinfo->x1;
           // cinfo->x2 = TPD_RES_X - 1 -cinfo->x2;
            TPD_LOGV("++++++++zym+++++++++p1: (%3d,%3d)(%3d,%3d)\n",cinfo->x1,cinfo->y1,TPD_RES_X,TPD_RES_Y);
            cinfo->pressure = 1;
            TPD_LOGV("pressure: %d\n",cinfo->pressure);
        }
    }
    else
    {
        cinfo->count = 0;
    }

    /*ergate-012 start*/
    /*ergate-012 end*/

    return 1;
}
#if defined(HQ_MSG2133_PS_SUPPORT)
bool get_lcm_ps_stat()
{	 
	return  ps_state;
}
/*----------------------------------------------------------------------------*/
EXPORT_SYMBOL(get_lcm_ps_stat);
/*----------------------------------------------------------------------------*/
#endif
static int touch_event_handler(void *unused)
{
    int pending = 0;
    struct TouchInfoT cinfo, pinfo;
    struct sched_param param = { .sched_priority = RTPM_PRIO_TPD };
    sched_setscheduler(current, SCHED_RR, &param);

    memset(&cinfo, 0, sizeof(struct TouchInfoT));
    memset(&pinfo, 0, sizeof(struct TouchInfoT));
    do
    {
    
        set_current_state(TASK_INTERRUPTIBLE); 
        if(!kthread_should_stop())
        {
            TPD_DEBUG_CHECK_NO_RESPONSE;
            do
            {
                if (pending) 
                    wait_event_interruptible_timeout(waiter, tpd_flag != 0, HZ/10);
                else 
                    wait_event_interruptible_timeout(waiter,tpd_flag != 0, HZ*2);
                 
            }while(0);
             
            if (tpd_flag == 0 && !pending) 
                continue; // if timeout for no touch, then re-wait.
             
            if (tpd_flag != 0 && pending > 0)   
                pending = 0;
             
            tpd_flag = 0;
            TPD_DEBUG_SET_TIME; 
        }
        set_current_state(TASK_RUNNING);

        if (tpd_touchinfo(&cinfo, &pinfo))
        {
            if(cinfo.count >0)
            {
                #if defined(HQ_TP_IMPROVE)
                pre_count =cinfo.count;
                pre_x1 =cinfo.x1;
                pre_y1 =cinfo.y1;
                #endif
                tpd_down(cinfo.x1, cinfo.y1, cinfo.pressure);
                if(cinfo.count>1)
                {               
                 #if defined(HQ_TP_IMPROVE)
                    pre_x2 =cinfo.x2;
                    pre_y2 =cinfo.y2;
                 #endif 
                    tpd_down(cinfo.x2, cinfo.y2, cinfo.pressure);
                }
                input_sync(tpd->dev);       
                TPD_LOGV("press  --->\n");
            }
            else if(cinfo.count==0 && pinfo.count!=0)
            {
            
             #if defined(HQ_TP_IMPROVE)
                tpd_up(pre_x1, pre_y1, cinfo.pressure);
                if(pre_count > 1)
                {
                    tpd_up(pre_x2, pre_y2, cinfo.pressure);
                }               
                input_sync(tpd->dev);
             #else
                input_mt_sync(tpd->dev);
                input_sync(tpd->dev);
             #endif
                TPD_LOGV("release --->\n"); 
            }
        }          
    }while(!kthread_should_stop());

    return 0;
}

 
static int tpd_detect (struct i2c_client *client, int kind, struct i2c_board_info *info) 
{
    strcpy(info->type, "msg2133");
	printk("tpd_detct\n");
    return 0;
}

 
static void tpd_eint_interrupt_handler(void)
{
    // mt65xx_eint_mask(CUST_EINT_TOUCH_PANEL_NUM); //1009 mask eint
   // printk("MYCAT TPD interrupt has been triggered\n");
    tpd_flag = 1;
    wake_up_interruptible(&waiter);
}


static int __devinit tpd_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    TPD_DMESG("tpd_prob begin (Built %s @ %s)\n", __DATE__, __TIME__);   
    int error;
    int retval = TPD_OK;
    i2c_client = client;

    /*ergate-dbg*/
    i2c_client->addr |= I2C_ENEXT_FLAG; //I2C_HS_FLAG;
    i2c_client->timing = 100;

    extern int tpd_load_status;

    /*ergate-016 start*/
	printk("MYCAT tpd_probe begin\n");

	hwPowerOn(TPD_POWER_SOURCE_CUSTOM, VOL_2800, "TP");
   // set deep sleep off
    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);		//GPIO_CTP_RST_PIN
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);		
	mt_set_gpio_pull_enable(GPIO_CTP_RST_PIN, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_select(GPIO_CTP_RST_PIN, GPIO_PULL_UP);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
   
	msleep(100);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
		
		//eint
		//GPIO_CAPTOUCH_EINT_PIN = GPIO136;
		//GPIO_CAPTOUCH_EINT_PIN_M_EINT = GPIO_CTP_EINT_PIN_M_EINT;
		//CUST_EINT_TOUCH_PANEL_NUM = 1;
		
		/*ergate-016 end*/ 
    error = tpd_initialize(client);
    if(error)
    {
        TPD_LOGI("tpd_initialize error\n");
        tpd_load_status = 0;
        return -1;
    }

    /*ergate-009*/
    //tpd_create_attr(&driver_attr_calibrate);
    /*ergate-026*/
    ////tpd_create_attr(&driver_attr_loglevel);
 
#ifdef CONFIG_HAS_EARLYSUSPEND
    if(!(retval < TPD_OK)) 
    {
        early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
        early_suspend.suspend = tpd_early_suspend;
        early_suspend.resume = tpd_late_resume;
        register_early_suspend(&early_suspend);
    }
#endif
    
    /*ergate-001*/
    /*skip wrong EINT during init*/
    //msleep(500);  

    thread = kthread_run(touch_event_handler, 0, "msg2133");
    if(IS_ERR(thread))
    {
        TPD_DMESG(TPD_DEVICE " failed to create kernel thread\n");
    }

 		mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_EINT);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_select(GPIO_CTP_EINT_PIN, GPIO_PULL_UP);
    mt65xx_eint_set_sens(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_SENSITIVE);
    mt65xx_eint_set_hw_debounce(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_CN);
    mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY, tpd_eint_interrupt_handler, 1);
    mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);
 
    TPD_DMESG("Touch Panel Device Probe %s\n", (retval < TPD_OK) ? "FAIL" : "PASS");

    tpd_load_status = 1;

///////////////////////////////////////////////////////////////////////////////////////////
   #ifdef __FIRMWARE_UPDATE__
	firmware_class = class_create(THIS_MODULE, "ms-touchscreen-msg20xx");
    if (IS_ERR(firmware_class))
        pr_err("Failed to create class(firmware)!\n");
    firmware_cmd_dev = device_create(firmware_class,
                                     NULL, 0, NULL, "device");
    if (IS_ERR(firmware_cmd_dev))
        pr_err("Failed to create device(firmware_cmd_dev)!\n");

    // version
    if (device_create_file(firmware_cmd_dev, &dev_attr_version) < 0)
        pr_err("Failed to create device file(%s)!\n", dev_attr_version.attr.name);
    // update
    if (device_create_file(firmware_cmd_dev, &dev_attr_update) < 0)
        pr_err("Failed to create device file(%s)!\n", dev_attr_update.attr.name);
    // data
    if (device_create_file(firmware_cmd_dev, &dev_attr_data) < 0)
        pr_err("Failed to create device file(%s)!\n", dev_attr_data.attr.name);
	// clear
 //   if (device_create_file(firmware_cmd_dev, &dev_attr_clear) < 0)
 //       pr_err("Failed to create device file(%s)!\n", dev_attr_clear.attr.name);

	dev_set_drvdata(firmware_cmd_dev, NULL);
	
#ifdef TPD_PROXIMITY

	if(device_create_file(firmware_cmd_dev, &dev_attr_proximity_sensor) < 0) // /sys/class/mtk-tpd/device/proximity_sensor
		{
 			printk("xxxxx mycat Failed to create device file(%s)!\n", dev_attr_proximity_sensor.attr.name);
		}
#endif
	
	
    #ifdef	ENABLE_AUTO_UPDATA
	TP_DEBUG_ERR("[TP] check auto updata\n");
/*
	
	if(getchipType() == CTP_ID_MSG21XXA)
	{
		//msg2133A		
		TP_DEBUG_ERR("[TP] TP IC is msg21xxA Version = %d \n", (getFWPrivateVersion()&0xff));
		
		//	if((getFWPrivateVersion()&0xff) < 4)
			{
			    int i = 0, j=0;
				TP_DEBUG_ERR("[TP] TP FW version is less than 4\n");
				
				for (i = 0; i < 33; i++)
    			{
    				for (j = 0; j < 1024; j++)
    				{
        				temp[i][j] = mstar_fw_array[i*1024+j];
    				}
    			}
				//firmware_update_store(NULL, NULL, NULL, 0);	
				kthread_run(fwAutoUpdate, 0, client->name);
			}
	}
	*/
#endif

#endif 
////////////////////////////////////////////////////////////////////////////////////////////
#if 0
    /*FW UPDATE*/
    /* set sysfs for frimware */
    firmware_class = class_create(THIS_MODULE, "ms-touchscreen-msg20xx");
    if (IS_ERR(firmware_class))
        pr_err("Failed to create class(firmware)!\n");

    firmware_cmd_dev = device_create(firmware_class,
                                     NULL, 0, NULL, "device");
    if (IS_ERR(firmware_cmd_dev))
        pr_err("Failed to create device(firmware_cmd_dev)!\n");

    // version
    if (device_create_file(firmware_cmd_dev, &dev_attr_version) < 0)
        pr_err("Failed to create device file(%s)!\n", dev_attr_version.attr.name);
    // update
    if (device_create_file(firmware_cmd_dev, &dev_attr_update) < 0)
        pr_err("Failed to create device file(%s)!\n", dev_attr_update.attr.name);
    // data
    if (device_create_file(firmware_cmd_dev, &dev_attr_data) < 0)
        pr_err("Failed to create device file(%s)!\n", dev_attr_data.attr.name);

    dev_set_drvdata(firmware_cmd_dev, NULL);

	#endif 

//add by zym 2012-05-02 
#if 0
    {
    unsigned char dbbus_tx_data[3];
    unsigned char dbbus_rx_data[4] ;
    unsigned short major=0, minor=0;    dbbus_tx_data[0] = 0x53;
    dbbus_tx_data[1] = 0x00;
    dbbus_tx_data[2] = 0x74;
    HalTscrCDevWriteI2CSeq(i2c_client->addr, &dbbus_tx_data[0], 3);
    HalTscrCReadI2CSeq(i2c_client->addr, &dbbus_rx_data[0], 4);
   // HalTscrCReadI2CSeq(0x4C, &dbbus_rx_data[0], 4);
    major = (dbbus_rx_data[1]<<8)+dbbus_rx_data[0];
    minor = (dbbus_rx_data[3]<<8)+dbbus_rx_data[2];

    tpd->hq_ctp_firmware_version = kzalloc(sizeof(char), GFP_KERNEL);
    sprintf(tpd->hq_ctp_firmware_version,"V%d.%03d", major, minor);
    printk("+++(1)lcm+++ %d.%03d\n",major,minor);
    ///*
    if(major == 1)
    {
     /*
        tpd->hq_ctp_module_name = "msg2133_shenyue";
        //tempData = msg2133_bin_shenyue;
        if(major == msg2133_bin_shenyue[729*17+14] && msg2133_bin_shenyue[729*17+12] > minor)
        {
            masterBUT_LoadFwToTarget(msg2133_bin_shenyue);
            printk("+++(2)lcm+++ shenyue:%d.%03d,fw:%d.%03d\n",major,minor,msg2133_bin_shenyue[729*17+14],msg2133_bin_shenyue[729*17+12]);
            sprintf(tpd->hq_ctp_firmware_version,"V%d.%03d",msg2133_bin_shenyue[729*17+14],msg2133_bin_shenyue[729*17+12]);
        }
        */
        tpd->hq_ctp_module_name = "msg2133_oufeiguang";
        //tempData = msg2133_bin_oufeiguang;
        if(major == msg2133_bin_oufeiguang[729*17+14] && msg2133_bin_oufeiguang[729*17+12] > minor)
        {
            masterBUT_LoadFwToTarget(msg2133_bin_oufeiguang);
            printk("+++(3)lcm+++ mudong:%d.%03d,fw:%d.%03d\n",major,minor,msg2133_bin_oufeiguang[729*17+14],msg2133_bin_oufeiguang[729*17+12]);
            sprintf(tpd->hq_ctp_firmware_version,"V%d.%03d",msg2133_bin_oufeiguang[729*17+14],msg2133_bin_oufeiguang[729*17+12]);
        }
    }
/*
    else if(major == 2)
    {
        tpd->hq_ctp_module_name = "msg2133_oufeiguang";
        //tempData = msg2133_bin_oufeiguang;
        if(major == msg2133_bin_oufeiguang[729*17+14] && msg2133_bin_oufeiguang[729*17+12] > minor)
        {
            masterBUT_LoadFwToTarget(msg2133_bin_oufeiguang);
            printk("+++(3)lcm+++ mudong:%d.%03d,fw:%d.%03d\n",major,minor,msg2133_bin_oufeiguang[729*17+14],msg2133_bin_oufeiguang[729*17+12]);
            sprintf(tpd->hq_ctp_firmware_version,"V%d.%03d",msg2133_bin_oufeiguang[729*17+14],msg2133_bin_oufeiguang[729*17+12]);
        }
    }
    */
    else
    {
        //tpd->hq_ctp_module_name = "default ctp";
        //masterBUT_LoadFwToTarget(msg2133_bin_oufeiguang);//set defaut is oufeiguang
        //sprintf(tpd->hq_ctp_firmware_version,"V%d.%03d",msg2133_bin_oufeiguang[729*17+14],msg2133_bin_oufeiguang[729*17+12]);
        //shengyue do nothing
    }
    //*/
    }
    //masterBUT_LoadFwToTarget(msg2133_bin_mudong);
#endif
    //end by zym
	printk("MYCAT tpd_prob end (Built %s @ %s)\n", __DATE__, __TIME__);   

 

    return 0;     
}
#if defined(HQ_MSG2133_PS_SUPPORT)
int msg2133_ps_mode_enable(bool enable)
{
	unsigned char dbbus_tx_data[4];
	if(!enable)
		{
      dbbus_tx_data[0] = 0x52;
      dbbus_tx_data[1] = 0x00;
      dbbus_tx_data[2] = 0x62;
      dbbus_tx_data[3] = 0xA1;
      HalTscrCDevWriteI2CSeq(0x26, &dbbus_tx_data[0], 4);//ZZF ADD FOR PS
      ps_state = 1; //kaka_12_1227
      printk("[zzf ctp ps] close\n");
      
		}
		else
	{
#if 0
        tpd_hw_power(1);
        msleep(10);
        mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);
        /*mt65xx_eint_mask cann't stop EINT, so doing following*/
        mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY,    tpd_eint_interrupt_handler, 1);
       tpd_hw_enable(1);
#endif       
      dbbus_tx_data[0] = 0x52;
      dbbus_tx_data[1] = 0x00;
      dbbus_tx_data[2] = 0x62;
      dbbus_tx_data[3] = 0xA0;
      HalTscrCDevWriteI2CSeq(0x26, &dbbus_tx_data[0], 4);//ZZF ADD FOR PS
      ps_state = 1; //kaka_12_1227
      printk("[zzf ctp ps] open");
			}
			return 0;
} 

EXPORT_SYMBOL(msg2133_ps_mode_enable);//zzf add
#endif 

static int tpd_power_on ()
{
    int retval = TPD_OK;
    int tries = 0;
    u8 host_reg;
   
    TPD_LOGI("Power on\n");


	hwPowerOn(TPD_POWER_SOURCE_CUSTOM, VOL_2800, "TP");


    /*ergate-001 start*/
    tpd_hw_power(0);
    tpd_hw_power(1);
	tpd_hw_enable(1);
	tpd_hw_enable(0);
	msleep(20);  
    tpd_hw_enable(1);
	msleep(20);
    tpd_hw_config_eint_pin(2);
    /*ergate-001 end*/
 
    return retval;
}
static int tpd_initialize(struct i2c_client * client)
{
    int retval = TPD_OK;
    struct TpdPacketT packet = 
    {
        TPD_PACKET_HEAD_CMD,
        TPD_PACKET_CMD_FORMAT_WRITE,
        0x00,
        0x00
    };
    struct TpdPacketT pr;
    char buf[4];
    buf[0] = 0x53;
    buf[1] = 0x00;        
    buf[2] = 0x74;   
    
    memset(&tpd_firmware_version,0, sizeof(tpd_firmware_version));
    retval = tpd_power_on();
     printk("MYCAT tpd_initialize 00 ret=%d\n",retval);
    msleep(200);
    retval = i2c_master_send(i2c_client, buf, 3);
    retval = i2c_master_recv(i2c_client, buf, 4);
	  printk("MYCAT tpd_initialize 01 ret=%d\n",retval);
    if(retval > TPD_OK)
        retval = TPD_OK;
    
    return retval;

}


static int __devexit tpd_remove(struct i2c_client *client) 
{
#ifdef TPD_PROXIMITY	
	if (tpd_proximity_flag == 1)
	{
		if(tpd_proximity_flag_one == 1)
		{
			tpd_proximity_flag_one = 0;	
			TPD_DMESG(TPD_DEVICE " tpd_proximity_flag_one \n"); 
			return;
		}
	}	
	TPD_PROXIMITY_DEBUG("xxxxx mycat tpd_proximity_flag = %d\n", tpd_proximity_flag);
#endif


    int error;
 
  #ifdef CONFIG_HAS_EARLYSUSPEND
     unregister_early_suspend(&early_suspend);
  #endif /* CONFIG_HAS_EARLYSUSPEND */
   
    TPD_LOGI("TPD removed\n");
 
    return 0;
}
 
 
static int tpd_local_init(void)
{
    int retval;
 
    TPD_DMESG("msg2133 I2C Touchscreen Driver (Built %s @ %s)\n", __DATE__, __TIME__);
    TPD_DMESG("msg2133 I2C Touchscreen Driver (Built %s @ %s)\n", __DATE__, __TIME__);
    TPD_DMESG("msg2133 I2C Touchscreen Driver (Built %s @ %s)\n", __DATE__, __TIME__);

    retval = i2c_add_driver(&tpd_driver);

    printk("mycat i2c_add_driver retval= %d\n", retval);

    /*ergate-012*/
#ifdef TPD_HAVE_BUTTON     
    tpd_button_setting(TPD_KEY_COUNT, tpd_keys_local, tpd_keys_dim_local);// initialize tpd button data
#endif 

    /*ergate-001*/
    tpd_type_cap = 1;
    return 0; 
}

static int tpd_resume(struct i2c_client *client)
{
    int retval = TPD_OK;
	//printk("MYCAT tpd_resume\n");

	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);

	msleep(100);
	mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);
	//printk("MYCAT tpd_resume end\n");	
    return retval;
	   
	 //i2c_master_recv_ext(i2c_client, (u8 *)&TpdTouchData, sizeof(TpdTouchData));
  // tpd_hw_enable(1);
    
 
}

#if defined(HQ_MSG2133_PS_SUPPORT)
extern volatile bool msg2133_ps_open;
#endif 
static int tpd_suspend(struct i2c_client *client, pm_message_t message)
{
    int retval = TPD_OK;
	//printk("MYCAT tpd_suspend\n"); 
	//return retval;

/////////////////////////////////////////////////////////////////add by lan
#ifdef TPD_PROXIMITY
	if (tpd_proximity_flag == 1)
	{
		tpd_proximity_flag_one = 1;	
		return;
	}	 
	TPD_PROXIMITY_DEBUG("xxxxx mycat tpd_proximity_flag = %d\n", tpd_proximity_flag);
#endif



	    mt65xx_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);
    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
	
		return retval;

   
}

void tpd_uniform_ctp(char *buf,int strlen)
{
    int retval = TPD_OK;
    char result = buf[strlen-1] + 1;
    int i = 0;


    for(i = 0;i < 5 && (result != buf[strlen-1]);i++)
    {
        i2c_client->addr = (0xc4>>1) | I2C_ENEXT_FLAG;
        retval = i2c_master_send(i2c_client, buf, strlen);
        udelay(100);
        retval = i2c_master_send(i2c_client, buf, strlen-1);
        i2c_client->addr = (0xc5>>1) | I2C_ENEXT_FLAG;
        retval = i2c_master_recv(i2c_client, &result, 1);
    }
}


#if defined(HQ_TP_IMPROVE) 
extern LCM_DRIVER  *lcm_drv;

int tpd_resume_in_lcm(void)
{
    int retval = TPD_OK;
    unsigned short addr_bake = 0;
    TPD_LOGI("TPD wake up\n");  
    msleep(30);
    tpd_hw_power(1);    
    tpd_hw_enable(1);   
    msleep(200);  
    
    //zym 2011-12-26
    //hupeng add to read out the ctp version
    char buf_1[5] = {0x53,0x45,0x52,0x44,0x42};
    char buf_2[1] = {0x37};
    char buf_3[1] = {0x35};
    char buf_4[1] = {0x71};
    char buf_5[1] = {0x45};
    
//lcd dijing 0x62
    //char buf_dj_reg1[4] = {0x10,0x11,0x0e,0x19};
    //char buf_dj_reg2[4] = {0x10,0x11,0x12,0x19};
    char buf_dj_reg3[4] = {0x10,0x11,0x20,0x7c};
    //char buf_dj_reg4[4] = {0x10,0x11,0x23,0x4B};
//lcd tcl 0x62
   // char buf_tcl_reg1[4] = {0x10,0x11,0x0e,0x00};
    //char buf_tcl_reg2[4] = {0x10,0x11,0x12,0x00};
    char buf_tcl_reg3[4] = {0x10,0x11,0x20,0x50};
   // char buf_tcl_reg4[4] = {0x10,0x11,0x23,0x83};
    
    addr_bake = i2c_client->addr;
    i2c_client->addr = 0xc4 | I2C_ENEXT_FLAG;
    retval = i2c_master_send(i2c_client, buf_1, 5);
    msleep(1);
    retval = i2c_master_send(i2c_client, buf_2, 1);
    msleep(1);
    retval = i2c_master_send(i2c_client, buf_3, 1);
    msleep(1);
    retval = i2c_master_send(i2c_client, buf_4, 1);
//zym 2011-12-26
    TPD_LOGI("<<>> current lcd is %s \n",lcm_drv->name);
    if(!strncmp(lcm_drv->name,"ili9341_dijing",14))
    {
        TPD_LOGI("<<>> current lcd is %s \n",lcm_drv->name);
        //tpd_uniform_ctp(buf_dj_reg1,4);
        //tpd_uniform_ctp(buf_dj_reg2,4);
        tpd_uniform_ctp(buf_dj_reg3,4);
        //tpd_uniform_ctp(buf_dj_reg4,4);
    }
    else if(!strncmp(lcm_drv->name,"ili9341_txd",11))
    {
        TPD_LOGI("<<>> current lcd is %s > \n",lcm_drv->name);
        //tpd_uniform_ctp(buf_tcl_reg1,4);
        //tpd_uniform_ctp(buf_tcl_reg2,4);  
        tpd_uniform_ctp(buf_tcl_reg3,4);
        //tpd_uniform_ctp(buf_tcl_reg4,4);
    }
    else
    {
        TPD_LOGI("current lcd is default \n");
        //donothing
    }
    i2c_client->addr = 0xc4 | I2C_ENEXT_FLAG;
    retval = i2c_master_send(i2c_client, buf_5, 1);
    msleep(1);
    i2c_client->addr = addr_bake;
//end by zym

    mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY,          tpd_eint_interrupt_handler, 1);
    mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);  
    return retval;
}
#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
static void tpd_early_suspend(struct early_suspend *handler)
{
	return;
  
   // printk("MYCAT tpd_early_suspend\n");
	//  i2c_master_recv_ext(i2c_client, (u8 *)&TpdTouchData, sizeof(TpdTouchData));
}


static void tpd_late_resume(struct early_suspend *handler)
{
    //tpd_resume(i2c_client);
    //printk("MYCAT tpd_late_resume\n");
	return;
	
	
}
#endif
 

static struct tpd_driver_t tpd_device_driver = 
{
        .tpd_device_name = "msg2133",
        .tpd_local_init = tpd_local_init,
        .suspend = tpd_suspend,
#if defined(HQ_TP_IMPROVE) 
        .resume = NULL,
#else
        .resume = tpd_resume,
#endif
#ifdef TPD_HAVE_BUTTON
        .tpd_have_button = 1,
#else
        .tpd_have_button = 0,
#endif      
};


/* called when loaded into kernel */
static int __init tpd_driver_init(void)
{
    printk("MYCAT msg21xx touch panel driver init\n");
    i2c_register_board_info(1, &i2c_tpd, 1);
    if(tpd_driver_add(&tpd_device_driver) < 0)
        printk("mycat add tpd driver failed\n");
    return 0;
}


/* should never be called */
static void __exit tpd_driver_exit(void)
{
    TPD_DMESG("MediaTek msg2133 touch panel driver exit\n");
    //input_unregister_device(tpd->dev);
    tpd_driver_remove(&tpd_device_driver);
}


module_init(tpd_driver_init);
module_exit(tpd_driver_exit);
