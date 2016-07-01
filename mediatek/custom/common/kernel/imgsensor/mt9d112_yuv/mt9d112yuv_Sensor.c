/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   sensor.c
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Source code of Sensor driver
 *
 *
 * Author:
 * -------
 *   PC Huang (MTK02204)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision:$
 * $Modtime:$
 * $Log:$
 *
 * 01 04 2012 hao.wang
 * [ALPS00109603] getsensorid func check in
 * .
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include <linux/xlog.h>

//#include <mach/mt6516_pll.h>

#include "kd_camera_hw.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"

#include "mt9d112yuv_Sensor.h"
#include "mt9d112yuv_Camera_Sensor_para.h"
#include "mt9d112yuv_CameraCustomized.h"

#define MT9D112YUV_DEBUG
#ifdef MT9D112YUV_DEBUG
#define SENSORDB(fmt, arg...) xlog_printk(ANDROID_LOG_INFO ,"[MT9D112YUV]", fmt, ##arg)
#else
#define SENSORDB(fmt, arg...)
#endif

//DAVID.LIU ERR

#define WINMO_USE 0
#define Sleep(ms) mdelay(ms)
#define RETAILMSG(x,...)

static int awbMode = AWB_MODE_AUTO;

static MSDK_SCENARIO_ID_ENUM MT9D112_currentScenarioId = MSDK_SCENARIO_ID_CAMERA_PREVIEW;
struct MT9D112_sensor_struct MT9D112_Sensor_Driver;
MSDK_SENSOR_CONFIG_STRUCT MT9D112SensorConfigData;

static int zsd_setting = 0;
static DEFINE_SPINLOCK(MT9D112_drv_lock);
extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);

kal_uint16 MT9D112_write_cmos_sensor(kal_uint32 addr, kal_uint32 para)
{
	char puSendCmd[4] = {(char)(addr >> 8) , (char)(addr & 0xFF) ,(char)(para >> 8),(char)(para & 0xFF)};
	iWriteRegI2C(puSendCmd , 4,MT9D112_WRITE_ID);
    return ERROR_NONE;
}

kal_uint16 MT9D112_read_cmos_sensor(kal_uint32 addr)
{
	kal_uint16 get_byte=0;
    char puSendCmd[2] = {(char)(addr >> 8) , (char)(addr & 0xFF) };
	iReadRegI2C(puSendCmd , 2, (u8*)&get_byte,2,MT9D112_WRITE_ID);
    return ((get_byte<<8)&0xff00)|((get_byte>>8)&0x00ff);
}

kal_uint16 MT9D112_read_cmos_sensor_8(kal_uint32 addr)
{
	kal_uint16 get_byte=0;
    char puSendCmd[2] = {(char)(addr >> 8) , (char)(addr & 0xFF) };
	iReadRegI2C(puSendCmd , 2, (u8*)&get_byte,1,MT9D112_WRITE_ID);
    return get_byte;
}

void MT9D112_write_cmos_sensor_8(kal_uint32 addr, kal_uint32 para)
{
	char puSendCmd[4] = {(char)(addr >> 8) , (char)(addr & 0xFF) ,(char)(para & 0xFF)};
	iWriteRegI2C(puSendCmd , 3,MT9D112_WRITE_ID);
}

void MT9D112_Initial_Setting(void)
{
	SENSORDB( "\n ****** MT9D112_Initial_Setting \n");
    //MCLK:24MHZ,PCLK:56MHZ,Frames:15.8fps!
    //[INITIAL]
    //[RESET]
    MT9D112_write_cmos_sensor(0x301A, 0x0ACC);    // RESET_REGISTER
    MT9D112_write_cmos_sensor(0x3202, 0x0008);    // STANDBY_CONTROL
    MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0
    //[PLL]
    MT9D112_write_cmos_sensor(0x341E, 0x8F09);    // PLL_CLK_IN_CONTROL
    MT9D112_write_cmos_sensor(0x341C, 0x0238);    // PLL_DIVIDERS1
    MT9D112_write_cmos_sensor(0x341E, 0x8F09);    // PLL_CLK_IN_CONTROL
    MT9D112_write_cmos_sensor(0x341E, 0x8F08);    // PLL_CLK_IN_CONTROL
    MT9D112_write_cmos_sensor(0x3044, 0x0542);    // RESERVED_CORE_3044
    MT9D112_write_cmos_sensor(0x3216, 0x02CF);    // INTERNAL_CLOCK_CONTROL
    MT9D112_write_cmos_sensor(0x321C, 0x0402);    // OF_CONTROL_STATUS
    MT9D112_write_cmos_sensor(0x3212, 0x0001);    // FACTORY_BYPASS
    MT9D112_write_cmos_sensor(0x3214, 0x06E6);    // FACTORY_BYPASS
    mdelay(5);
    //[timing]
    MT9D112_write_cmos_sensor(0x338C, 0x2703);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0320);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2705);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0258);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2707);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0640);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2709);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x04B0);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x270D);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0000);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x270F);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0000);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2711);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x04BD);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2713);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x064D);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2715);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0433);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2717);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x2112);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2719);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x046C);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x271B);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0122);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x271D);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x007B);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x271F);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x013F);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2721);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x00AB);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2723);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x02C4);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2725);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x04E2);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2727);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x1010);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2729);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x2010);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x272B);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x1010);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x272D);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x1007);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x272F);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0004);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2731);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0004);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2733);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x04BB);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2735);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x064B);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2737);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0088);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2739);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x2111);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x273B);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0024);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x273D);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0120);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x273F);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x00A4);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2741);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0169);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2743);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x00A4);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2745);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x05FA);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2747);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x09C4);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2751);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0000);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2753);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0320);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2755);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0000);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2757);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0258);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x275F);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0000);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2761);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0640);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2763);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0000);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2765);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x04B0);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x222E);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x005D);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0xA404);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0010);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0xA408);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0011);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0xA409);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0013);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0xA40A);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0015);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0xA40B);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0017);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2411);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x005D);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2413);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0070);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2415);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x005D);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0x2417);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0070);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0xA40D);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0002);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0xA410);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0001);    // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
    mdelay(100);
    MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS
    MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0
    mdelay(100);
    MT9D112_write_cmos_sensor(0x338C, 0xA115); 
    MT9D112_write_cmos_sensor(0x3390, 0x00EF);
    MT9D112_write_cmos_sensor(0x338C, 0xA102); 
    MT9D112_write_cmos_sensor(0x3390, 0x000F);
		   
		MT9D112_write_cmos_sensor(0x338C, 0x2306);   //  MCU_ADDRESS [AWB_CCM_L_0]
		MT9D112_write_cmos_sensor(0x3390, 0x02EF);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2308);   //  MCU_ADDRESS [AWB_CCM_L_1]
		MT9D112_write_cmos_sensor(0x3390, 0xFEA5);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x230A);   //  MCU_ADDRESS [AWB_CCM_L_2]
		MT9D112_write_cmos_sensor(0x3390, 0xFF98);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x230C);   //  MCU_ADDRESS [AWB_CCM_L_3]
		MT9D112_write_cmos_sensor(0x3390, 0xFF64);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x230E);   //  MCU_ADDRESS [AWB_CCM_L_4]
		MT9D112_write_cmos_sensor(0x3390, 0x02DA);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2310);   //  MCU_ADDRESS [AWB_CCM_L_5]
		MT9D112_write_cmos_sensor(0x3390, 0xFF16);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2312);   //  MCU_ADDRESS [AWB_CCM_L_6]
		MT9D112_write_cmos_sensor(0x3390, 0xFF74);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2314);   //  MCU_ADDRESS [AWB_CCM_L_7]
		MT9D112_write_cmos_sensor(0x3390, 0xFD2A);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2316);   //  MCU_ADDRESS [AWB_CCM_L_8]
		MT9D112_write_cmos_sensor(0x3390, 0x04EB);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2318);   //  MCU_ADDRESS [AWB_CCM_L_9]
		MT9D112_write_cmos_sensor(0x3390, 0x0024);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x231A);   //  MCU_ADDRESS [AWB_CCM_L_10]
		MT9D112_write_cmos_sensor(0x3390, 0x003F);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x231C);   //  MCU_ADDRESS [AWB_CCM_RL_0]
		MT9D112_write_cmos_sensor(0x3390, 0xFFF3);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x231E);   //  MCU_ADDRESS [AWB_CCM_RL_1]
		MT9D112_write_cmos_sensor(0x3390, 0x000D);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2320);   //  MCU_ADDRESS [AWB_CCM_RL_2]
		MT9D112_write_cmos_sensor(0x3390, 0x0049);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2322);   //  MCU_ADDRESS [AWB_CCM_RL_3]
		MT9D112_write_cmos_sensor(0x3390, 0x0029);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2324);   //  MCU_ADDRESS [AWB_CCM_RL_4]
		MT9D112_write_cmos_sensor(0x3390, 0xFFEE);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2326);   //  MCU_ADDRESS [AWB_CCM_RL_5]
		MT9D112_write_cmos_sensor(0x3390, 0xFFFB);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2328);   //  MCU_ADDRESS [AWB_CCM_RL_6]
		MT9D112_write_cmos_sensor(0x3390, 0x007E);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x232A);   //  MCU_ADDRESS [AWB_CCM_RL_7]
		MT9D112_write_cmos_sensor(0x3390, 0x016F);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x232C);   //  MCU_ADDRESS [AWB_CCM_RL_8]
		MT9D112_write_cmos_sensor(0x3390, 0xFDB5);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x232E);   //  MCU_ADDRESS [AWB_CCM_RL_9]
		MT9D112_write_cmos_sensor(0x3390, 0x0018);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x2330);   //  MCU_ADDRESS [AWB_CCM_RL_10]
		MT9D112_write_cmos_sensor(0x3390, 0xFFEC);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA348);   //  MCU_ADDRESS [AWB_GAIN_BUFFER_SPEED]
		MT9D112_write_cmos_sensor(0x3390, 0x0008);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA349);   //  MCU_ADDRESS [AWB_JUMP_DIVISOR]
		MT9D112_write_cmos_sensor(0x3390, 0x0002);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA34A);   //  MCU_ADDRESS [AWB_GAIN_MIN]
		MT9D112_write_cmos_sensor(0x3390, 0x005A);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA34B);   //  MCU_ADDRESS [AWB_GAIN_MAX]
		MT9D112_write_cmos_sensor(0x3390, 0x00A6);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA34F);   //  MCU_ADDRESS [AWB_CCM_POSITION_MIN]
		MT9D112_write_cmos_sensor(0x3390, 0x0000);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA350);   //  MCU_ADDRESS [AWB_CCM_POSITION_MAX]
		MT9D112_write_cmos_sensor(0x3390, 0x007F);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA35B);   //  MCU_ADDRESS [AWB_STEADY_BGAIN_OUT_MIN]
		MT9D112_write_cmos_sensor(0x3390, 0x0078);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA35C);   //  MCU_ADDRESS [AWB_STEADY_BGAIN_OUT_MAX]
		MT9D112_write_cmos_sensor(0x3390, 0x0086);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA35D);   //  MCU_ADDRESS [AWB_STEADY_BGAIN_IN_MIN]
		MT9D112_write_cmos_sensor(0x3390, 0x007E);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA35E);   //  MCU_ADDRESS [AWB_STEADY_BGAIN_IN_MAX]
		MT9D112_write_cmos_sensor(0x3390, 0x0082);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0x235F);   //  MCU_ADDRESS [AWB_CNT_PXL_TH]
		MT9D112_write_cmos_sensor(0x3390, 0x0040);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA361);   //  MCU_ADDRESS [AWB_TG_MIN0]
		MT9D112_write_cmos_sensor(0x3390, 0x00C8);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA362);   //  MCU_ADDRESS [AWB_TG_MAX0]
		MT9D112_write_cmos_sensor(0x3390, 0x00E1);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA302);   //  MCU_ADDRESS [AWB_WINDOW_POS]
		MT9D112_write_cmos_sensor(0x3390, 0x0000);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA303);   //  MCU_ADDRESS [AWB_WINDOW_SIZE]
		MT9D112_write_cmos_sensor(0x3390, 0x00EF);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA364);   //  MCU_ADDRESS [AWB_WINDOW_SIZE]
		MT9D112_write_cmos_sensor(0x3390, 0x0080);   //  MCU_DATA_0
		MT9D112_write_cmos_sensor(0x338C, 0xA365);   //  MCU_ADDRESS [AWB_WINDOW_SIZE]
		MT9D112_write_cmos_sensor(0x3390, 0x0087);   //  MCU_DATA_0
		////  LENS SHADING
		MT9D112_write_cmos_sensor(0x34CE, 0x81A0); 	// LENS_CORRECTION_CONTROL
		MT9D112_write_cmos_sensor(0x34D0, 0x6432); 	// ZONE_BOUNDS_X1_X2
		MT9D112_write_cmos_sensor(0x34D2, 0x3296); 	// ZONE_BOUNDS_X0_X3
		MT9D112_write_cmos_sensor(0x34D4, 0x9664); 	// ZONE_BOUNDS_X4_X5
		MT9D112_write_cmos_sensor(0x34D6, 0x4623); 	// ZONE_BOUNDS_Y1_Y2
		MT9D112_write_cmos_sensor(0x34D8, 0x286A); 	// ZONE_BOUNDS_Y0_Y3
		MT9D112_write_cmos_sensor(0x34DA, 0x7750); 	// ZONE_BOUNDS_Y4_Y5
		MT9D112_write_cmos_sensor(0x34DC, 0xF700); 	// CENTER_OFFSET
		MT9D112_write_cmos_sensor(0x34DE, 0x014D); 	// FX_RED
		MT9D112_write_cmos_sensor(0x34E6, 0x00CA); 	// FY_RED
		MT9D112_write_cmos_sensor(0x34EE, 0x0DF7); 	// DF_DX_RED
		MT9D112_write_cmos_sensor(0x34F6, 0x0C59); 	// DF_DY_RED
		MT9D112_write_cmos_sensor(0x3500, 0x01F5); 	// SECOND_DERIV_ZONE_0_RED
		MT9D112_write_cmos_sensor(0x3508, 0xF801); 	// SECOND_DERIV_ZONE_1_RED
		MT9D112_write_cmos_sensor(0x3510, 0x362D); 	// SECOND_DERIV_ZONE_2_RED
		MT9D112_write_cmos_sensor(0x3518, 0x4143); 	// SECOND_DERIV_ZONE_3_RED
		MT9D112_write_cmos_sensor(0x3520, 0x2342); 	// SECOND_DERIV_ZONE_4_RED
		MT9D112_write_cmos_sensor(0x3528, 0x364B); 	// SECOND_DERIV_ZONE_5_RED
		MT9D112_write_cmos_sensor(0x3530, 0x1EE7); 	// SECOND_DERIV_ZONE_6_RED
		MT9D112_write_cmos_sensor(0x3538, 0xD6B5); 	// SECOND_DERIV_ZONE_7_RED
		MT9D112_write_cmos_sensor(0x354C, 0x0508); 	// K_FACTOR_IN_K_FX_FY_R_TL
		MT9D112_write_cmos_sensor(0x3544, 0x0521); 	// K_FACTOR_IN_K_FX_FY_R_TR
		MT9D112_write_cmos_sensor(0x355C, 0x046A); 	// K_FACTOR_IN_K_FX_FY_R_BL
		MT9D112_write_cmos_sensor(0x3554, 0x0481); 	// K_FACTOR_IN_K_FX_FY_R_BR
		MT9D112_write_cmos_sensor(0x34E0, 0x0148); 	// FX_GREEN
		MT9D112_write_cmos_sensor(0x34E8, 0x006A); 	// FY_GREEN
		MT9D112_write_cmos_sensor(0x34F0, 0x0E64); 	// DF_DX_GREEN
		MT9D112_write_cmos_sensor(0x34F8, 0x0E3F); 	// DF_DY_GREEN
		MT9D112_write_cmos_sensor(0x3502, 0x0EF4); 	// SECOND_DERIV_ZONE_0_GREEN
		MT9D112_write_cmos_sensor(0x350A, 0xE5EB); 	// SECOND_DERIV_ZONE_1_GREEN
		MT9D112_write_cmos_sensor(0x3512, 0x1A2E); 	// SECOND_DERIV_ZONE_2_GREEN
		MT9D112_write_cmos_sensor(0x351A, 0x1F2E); 	// SECOND_DERIV_ZONE_3_GREEN
		MT9D112_write_cmos_sensor(0x3522, 0x242C); 	// SECOND_DERIV_ZONE_4_GREEN
		MT9D112_write_cmos_sensor(0x352A, 0x181C); 	// SECOND_DERIV_ZONE_5_GREEN
		MT9D112_write_cmos_sensor(0x3532, 0x1503); 	// SECOND_DERIV_ZONE_6_GREEN
		MT9D112_write_cmos_sensor(0x353A, 0xEADF); 	// SECOND_DERIV_ZONE_7_GREEN
		MT9D112_write_cmos_sensor(0x354E, 0x0617); 	// K_FACTOR_IN_K_FX_FY_G1_TL
		MT9D112_write_cmos_sensor(0x3546, 0x0604); 	// K_FACTOR_IN_K_FX_FY_G1_TR
		MT9D112_write_cmos_sensor(0x355E, 0x0449); 	// K_FACTOR_IN_K_FX_FY_G1_BL
		MT9D112_write_cmos_sensor(0x3556, 0x04D8); 	// K_FACTOR_IN_K_FX_FY_G1_BR
		MT9D112_write_cmos_sensor(0x34E4, 0x010B); 	// FX_BLUE
		MT9D112_write_cmos_sensor(0x34EC, 0x005B); 	// FY_BLUE
		MT9D112_write_cmos_sensor(0x34F4, 0x0E70); 	// DF_DX_BLUE
		MT9D112_write_cmos_sensor(0x34FC, 0x0DB3); 	// DF_DY_BLUE
		MT9D112_write_cmos_sensor(0x3506, 0x1DFD); 	// SECOND_DERIV_ZONE_0_BLUE
		MT9D112_write_cmos_sensor(0x350E, 0xF2F7); 	// SECOND_DERIV_ZONE_1_BLUE
		MT9D112_write_cmos_sensor(0x3516, 0x1925); 	// SECOND_DERIV_ZONE_2_BLUE
		MT9D112_write_cmos_sensor(0x351E, 0x3122); 	// SECOND_DERIV_ZONE_3_BLUE
		MT9D112_write_cmos_sensor(0x3526, 0x1221); 	// SECOND_DERIV_ZONE_4_BLUE
		MT9D112_write_cmos_sensor(0x352E, 0x1117); 	// SECOND_DERIV_ZONE_5_BLUE
		MT9D112_write_cmos_sensor(0x3536, 0x1BEF); 	// SECOND_DERIV_ZONE_6_BLUE
		MT9D112_write_cmos_sensor(0x353E, 0xD402); 	// SECOND_DERIV_ZONE_7_BLUE
		MT9D112_write_cmos_sensor(0x3552, 0x0650); 	// K_FACTOR_IN_K_FX_FY_B_TL
		MT9D112_write_cmos_sensor(0x354A, 0x057D); 	// K_FACTOR_IN_K_FX_FY_B_TR
		MT9D112_write_cmos_sensor(0x3562, 0x0036); 	// K_FACTOR_IN_K_FX_FY_B_BL
		MT9D112_write_cmos_sensor(0x355A, 0x0486); 	// K_FACTOR_IN_K_FX_FY_B_BR
		MT9D112_write_cmos_sensor(0x34E2, 0x00F2); 	// FX_GREEN2
		MT9D112_write_cmos_sensor(0x34EA, 0x0064); 	// FY_GREEN2
		MT9D112_write_cmos_sensor(0x34F2, 0x0E92); 	// DF_DX_GREEN2
		MT9D112_write_cmos_sensor(0x34FA, 0x0DBB); 	// DF_DY_GREEN2
		MT9D112_write_cmos_sensor(0x3504, 0x0EF6); 	// SECOND_DERIV_ZONE_0_GREEN2
		MT9D112_write_cmos_sensor(0x350C, 0xFA03); 	// SECOND_DERIV_ZONE_1_GREEN2
		MT9D112_write_cmos_sensor(0x3514, 0x221C); 	// SECOND_DERIV_ZONE_2_GREEN2
		MT9D112_write_cmos_sensor(0x351C, 0x3331); 	// SECOND_DERIV_ZONE_3_GREEN2
		MT9D112_write_cmos_sensor(0x3524, 0x1E33); 	// SECOND_DERIV_ZONE_4_GREEN2
		MT9D112_write_cmos_sensor(0x352C, 0x1D28); 	// SECOND_DERIV_ZONE_5_GREEN2
		MT9D112_write_cmos_sensor(0x3534, 0x16EE); 	// SECOND_DERIV_ZONE_6_GREEN2
		MT9D112_write_cmos_sensor(0x353C, 0xECD1); 	// SECOND_DERIV_ZONE_7_GREEN2
		MT9D112_write_cmos_sensor(0x3550, 0x054C); 	// K_FACTOR_IN_K_FX_FY_G2_TL
		MT9D112_write_cmos_sensor(0x3548, 0x06B7); 	// K_FACTOR_IN_K_FX_FY_G2_TR
		MT9D112_write_cmos_sensor(0x3560, 0x048B); 	// K_FACTOR_IN_K_FX_FY_G2_BL
		MT9D112_write_cmos_sensor(0x3558, 0x044F); 	// K_FACTOR_IN_K_FX_FY_G2_BR
		MT9D112_write_cmos_sensor(0x3540, 0x0080); 	// X2_FACTORS
		MT9D112_write_cmos_sensor(0x3542, 0x0000); 	// GLOBAL_OFFSET_FXY_FUNCTION
		MT9D112_write_cmos_sensor(0x3210, 0x01FC); 	// COLOR_PIPELINE_CONTROL
		MT9D112_write_cmos_sensor(0x338C, 0xA103);   //  MCU_ADDRESS [SEQ_CMD]
		MT9D112_write_cmos_sensor(0x3390, 0x0005);   //  MCU_DATA_0
		 mdelay(100);
    
    }
void MT9D112_Normal_Pre(void)
{
	SENSORDB( "\n ****** MT9D112_Normal_Pre \n");

    //MT9D112_8404_Polling(0x8404,0x06);
    mDELAY(5);
}

void MT9D112_ZSD_Pre(void)
{
	SENSORDB( "\n ****** MT9D112_ZSD_Pre \n");
    //[**********Step1*************]

   /// off
}

static void MT9D112__AWB_Lock(void)
{

}

static void MT9D112__AWB_UnLock(void)
{
    SENSORDB("\n ****** MT9D112__AWB_UnLock\n");
   
}

//manuel exposure enable
static void MT9D112__AE_Lock(void)
{
    SENSORDB("\n ****** MT9D112__AE_Lock\n");

}

//ae enable
static void MT9D112__AE_UnLock(void)
{
  SENSORDB("\n ****** MT9D112__AE_UnLock\n");
  //MT9D112_write_cmos_sensor(0x098E, 0x8416);

}

static void MT9D112_SCENE_PORTRAIT(void)
{
	SENSORDB("SCENE_MODE_PORTRAIT");  /// 10~30 fps	
  
}

void MT9D112_night_mode(kal_bool enable)
{
	SENSORDB("\n ****** MT9D112_night_mode \n ");
    SENSORDB("[Enter]MT9D112 night mode func:enable = %d\n",enable);

	spin_lock(&MT9D112_drv_lock);
    MT9D112_Sensor_Driver.NightMode = enable;
	spin_unlock(&MT9D112_drv_lock);

    	if (enable)
    	{
			            MT9D112_write_cmos_sensor(0x338C, 0xA20C ); 
            MT9D112_write_cmos_sensor(0x3390, 0x0018);
            MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0
    	}
    	else
    	{
           MT9D112_write_cmos_sensor(0x338C, 0xA20C ); 
            MT9D112_write_cmos_sensor(0x3390, 0x000a);
            MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0 
    	}
 

   
}






static void MT9D112_SCENE_LANDSCAPE(void)
{
	SENSORDB("SCENE_MODE_LANDSCAPE");	///10~30fps
}

static void MT9D112_SCENE_SUNSET(void)
{
	SENSORDB("MT9D112_SCENE_SUNSET");	///10~30fps
}

static void MT9D112_SCENE_SPORTS(void)
{
	SENSORDB("MT9D112_SCENE_SPORTS");	///10~30fps
}

static void MT9D112_SCENE_NIGHTSCENE(void)
{
	SENSORDB("MT9D112_SCENE_NIGHTSCENE");  /// 5~30fps	
}

static void MT9D112_SCENE_NORMAL(void)
{
	SENSORDB("SCENE_MODE_OFF");	 /// 10~30fps
	
   
    
}


BOOL MT9D112_set_param_banding(UINT16 para)    /// okay
{
    SENSORDB("\n ****** MT9D112_set_param_banding\n");
	SENSORDB("[Enter]MT9D112 set_param_banding func:para = %d\n",para);

//DAVID.LIU DEBUG
 
    spin_lock(&MT9D112_drv_lock);    
    MT9D112_Sensor_Driver.Banding = para;
    spin_unlock(&MT9D112_drv_lock);	
	switch (para)
	{
	 case AE_FLICKER_MODE_50HZ:
        SENSORDB("[MT9D112YUV]:banding 50Hz\n");  
  

        //[50Hz]
        MT9D112_write_cmos_sensor(0x338C, 0xA404);    // MCU_ADDRESS [FD_MODE]
        MT9D112_write_cmos_sensor(0x3390, 0x00C0);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
        break;

    case AE_FLICKER_MODE_60HZ:
        SENSORDB("[MT9D112YUV]:banding 60Hz\n");  

        //[60Hz]
        MT9D112_write_cmos_sensor(0x338C, 0xA404);    // MCU_ADDRESS [FD_MODE]
        MT9D112_write_cmos_sensor(0x3390, 0x0080);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0

		case AE_FLICKER_MODE_OFF:
		{
		    MT9D112_write_cmos_sensor(0x338C, 0xA404);    // MCU_ADDRESS [FD_MODE]
        MT9D112_write_cmos_sensor(0x3390, 0x00c0);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
				}
		break;

		case AE_FLICKER_MODE_AUTO:
        default:
        {
	  		 MT9D112_write_cmos_sensor(0x338C, 0xA404);    // MCU_ADDRESS [FD_MODE]
        MT9D112_write_cmos_sensor(0x3390, 0x0000);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
        }
        break;
	}
	
       return KAL_TRUE;
}

void MT9D112__set_saturation(UINT16 para)
{
    SENSORDB("\n ****** MT9D112__set_saturation\n");

    switch (para)
    {
        case ISP_SAT_HIGH:
        	
            MT9D112_write_cmos_sensor(0x338C, 0xA115 ); 
            MT9D112_write_cmos_sensor(0x3390, 0x00EF);
            MT9D112_write_cmos_sensor(0x338C, 0xA118 ); 
            MT9D112_write_cmos_sensor(0x3390, 0x00a0);
            MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        		MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0

            break;
        case ISP_SAT_LOW:
            MT9D112_write_cmos_sensor(0x338C, 0xA115); 
            MT9D112_write_cmos_sensor(0x3390, 0x00EF);
            MT9D112_write_cmos_sensor(0x338C, 0xA118 ); 
            MT9D112_write_cmos_sensor(0x3390, 0x0060);
            MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        		MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0
            break;
        case ISP_SAT_MIDDLE:
            MT9D112_write_cmos_sensor(0x338C, 0xA115 ); 
            MT9D112_write_cmos_sensor(0x3390, 0x00EF);
            MT9D112_write_cmos_sensor(0x338C, 0xA118 ); 
            MT9D112_write_cmos_sensor(0x3390, 0x0090);
            MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        		MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0      
        default:
             break;
    }
                return ERROR_NONE;
}

void MT9D112__set_contrast(UINT16 para)
{
    SENSORDB("\n ****** MT9D112__set_contrast\n");

    switch (para)
    {
        case ISP_CONTRAST_HIGH:
        	
						  MT9D112_write_cmos_sensor(0x338C, 0xA76D); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_0]
							MT9D112_write_cmos_sensor(0x3390, 0x0003); 	// MCU_DATA_0       	
							MT9D112_write_cmos_sensor(0x338C, 0xA76F); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_0]
							MT9D112_write_cmos_sensor(0x3390, 0x0000); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA770); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_1]
							MT9D112_write_cmos_sensor(0x3390, 0x0009); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA771); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_2]
							MT9D112_write_cmos_sensor(0x3390, 0x001E); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA772); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_3]
							MT9D112_write_cmos_sensor(0x3390, 0x005A); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA773); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_4]
							MT9D112_write_cmos_sensor(0x3390, 0x008E); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA774); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_5]
							MT9D112_write_cmos_sensor(0x3390, 0x00A6); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA775); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_6]
							MT9D112_write_cmos_sensor(0x3390, 0x00B5); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA776); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_7]
							MT9D112_write_cmos_sensor(0x3390, 0x00C1); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA777); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_8]
							MT9D112_write_cmos_sensor(0x3390, 0x00CB); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA778); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_9]
							MT9D112_write_cmos_sensor(0x3390, 0x00D3); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA779); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_10]
							MT9D112_write_cmos_sensor(0x3390, 0x00DA); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77A); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_11]
							MT9D112_write_cmos_sensor(0x3390, 0x00E1); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77B); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_12]
							MT9D112_write_cmos_sensor(0x3390, 0x00E6); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77C); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_13]
							MT9D112_write_cmos_sensor(0x3390, 0x00EB); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77D); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_14]
							MT9D112_write_cmos_sensor(0x3390, 0x00F0); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77E); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_15]
							MT9D112_write_cmos_sensor(0x3390, 0x00F4); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77F); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_16]
							MT9D112_write_cmos_sensor(0x3390, 0x00F8); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA780); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_17]
							MT9D112_write_cmos_sensor(0x3390, 0x00FC); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA781); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_18]
							MT9D112_write_cmos_sensor(0x3390, 0x00FF); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA103); 	// MCU_ADDRESS [SEQ_CMD]
							MT9D112_write_cmos_sensor(0x3390, 0x0005); 	// MCU_DATA_0

         
            break;
 
        case ISP_CONTRAST_LOW:
        			MT9D112_write_cmos_sensor(0x338C, 0xA76D); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_0]
							MT9D112_write_cmos_sensor(0x3390, 0x0003); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA76F); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_0]
							MT9D112_write_cmos_sensor(0x3390, 0x0000); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA770); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_1]
							MT9D112_write_cmos_sensor(0x3390, 0x0002); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA771); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_2]
							MT9D112_write_cmos_sensor(0x3390, 0x0006); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA772); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_3]
							MT9D112_write_cmos_sensor(0x3390, 0x0013); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA773); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_4]
							MT9D112_write_cmos_sensor(0x3390, 0x002E); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA774); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_5]
							MT9D112_write_cmos_sensor(0x3390, 0x0045); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA775); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_6]
							MT9D112_write_cmos_sensor(0x3390, 0x0059); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA776); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_7]
							MT9D112_write_cmos_sensor(0x3390, 0x006D); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA777); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_8]
							MT9D112_write_cmos_sensor(0x3390, 0x0081); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA778); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_9]
							MT9D112_write_cmos_sensor(0x3390, 0x0093); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA779); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_10]
							MT9D112_write_cmos_sensor(0x3390, 0x00A4); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77A); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_11]
							MT9D112_write_cmos_sensor(0x3390, 0x00B3); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77B); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_12]
							MT9D112_write_cmos_sensor(0x3390, 0x00C0); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77C); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_13]
							MT9D112_write_cmos_sensor(0x3390, 0x00CC); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77D); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_14]
							MT9D112_write_cmos_sensor(0x3390, 0x00D8); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77E); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_15]
							MT9D112_write_cmos_sensor(0x3390, 0x00E3); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77F); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_16]
							MT9D112_write_cmos_sensor(0x3390, 0x00ED); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA780); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_17]
							MT9D112_write_cmos_sensor(0x3390, 0x00F6); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA781); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_18]
							MT9D112_write_cmos_sensor(0x3390, 0x00FF); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA103); 	// MCU_ADDRESS [SEQ_CMD]
							MT9D112_write_cmos_sensor(0x3390, 0x0005); 	// MCU_DATA_0

            break;
            
        case ISP_CONTRAST_MIDDLE:        
        default:
                //[contrast_m--]
              MT9D112_write_cmos_sensor(0x338C, 0xA76D); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_0]
							MT9D112_write_cmos_sensor(0x3390, 0x0003); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA76F); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_0]
							MT9D112_write_cmos_sensor(0x3390, 0x0000); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA770); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_1]
							MT9D112_write_cmos_sensor(0x3390, 0x0004); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA771); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_2]
							MT9D112_write_cmos_sensor(0x3390, 0x000E); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA772); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_3]
							MT9D112_write_cmos_sensor(0x3390, 0x0029); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA773); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_4]
							MT9D112_write_cmos_sensor(0x3390, 0x0050); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA774); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_5]
							MT9D112_write_cmos_sensor(0x3390, 0x006A); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA775); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_6]
							MT9D112_write_cmos_sensor(0x3390, 0x0081); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA776); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_7]
							MT9D112_write_cmos_sensor(0x3390, 0x0094); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA777); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_8]
							MT9D112_write_cmos_sensor(0x3390, 0x00A5); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA778); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_9]
							MT9D112_write_cmos_sensor(0x3390, 0x00B2); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA779); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_10]
							MT9D112_write_cmos_sensor(0x3390, 0x00BE); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77A); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_11]
							MT9D112_write_cmos_sensor(0x3390, 0x00C9); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77B); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_12]
							MT9D112_write_cmos_sensor(0x3390, 0x00D3); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77C); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_13]
							MT9D112_write_cmos_sensor(0x3390, 0x00DC); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77D); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_14]
							MT9D112_write_cmos_sensor(0x3390, 0x00E4); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77E); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_15]
							MT9D112_write_cmos_sensor(0x3390, 0x00EB); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA77F); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_16]
							MT9D112_write_cmos_sensor(0x3390, 0x00F2); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA780); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_17]
							MT9D112_write_cmos_sensor(0x3390, 0x00F9); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA781); 	// MCU_ADDRESS [MODE_GAM_TABLE_A_18]
							MT9D112_write_cmos_sensor(0x3390, 0x00FF); 	// MCU_DATA_0
							MT9D112_write_cmos_sensor(0x338C, 0xA103); 	// MCU_ADDRESS [SEQ_CMD]
							MT9D112_write_cmos_sensor(0x3390, 0x0005); 	// MCU_DATA_0

            break;
    	}

	return;
	
}

void MT9D112__set_brightness(UINT16 para)   /// NO SUPPORT
{
    SENSORDB("\n ****** MT9D112__set_brightness\n");
    #if 1
    switch (para)
    {
        case ISP_BRIGHT_HIGH:
           
    
            break;
 
        case ISP_BRIGHT_LOW:
 
            break;
        case ISP_BRIGHT_MIDDLE:
  
      
        default:
             break;
    }    
    #endif
	return ERROR_NONE;
}

void MT9D112__set_iso(UINT16 para)
{
    SENSORDB("\n ****** MT9D112__set_iso\n");
    switch (para)
    {
        case AE_ISO_100:
             //ISO 100
			        MT9D112_write_cmos_sensor(0x338C, 0xA20E);    // MCU_ADDRESS [AE_TARGET]
			        MT9D112_write_cmos_sensor(0x3390, 0x0020);    // MCU_DATA_0  
			        MT9D112_write_cmos_sensor(0x338C, 0xA216);    // MCU_ADDRESS [AE_TARGET]
			        MT9D112_write_cmos_sensor(0x3390, 0x0020);    // MCU_DATA_0  
			        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
			        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0
             break;
        case AE_ISO_200:
             //ISO 200
			        MT9D112_write_cmos_sensor(0x338C, 0xA20E);    // MCU_ADDRESS [AE_TARGET]
			        MT9D112_write_cmos_sensor(0x3390, 0x0040);    // MCU_DATA_0 
			        MT9D112_write_cmos_sensor(0x338C, 0xA216);    // MCU_ADDRESS [AE_TARGET]
			        MT9D112_write_cmos_sensor(0x3390, 0x0040);    // MCU_DATA_0  
			        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
			        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0
             break;
             
        case AE_ISO_400:
             //ISO 400
			        MT9D112_write_cmos_sensor(0x338C, 0xA20E);    // MCU_ADDRESS [AE_TARGET]
			        MT9D112_write_cmos_sensor(0x3390, 0x0060);    // MCU_DATA_0 
			        MT9D112_write_cmos_sensor(0x338C, 0xA216);    // MCU_ADDRESS [AE_TARGET]
			        MT9D112_write_cmos_sensor(0x3390, 0x0040);    // MCU_DATA_0                        
			        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
			        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0
             break;
             
        default:
        case AE_ISO_AUTO:
             //ISO Auto
              MT9D112_write_cmos_sensor(0x338C, 0xA20E);    // MCU_ADDRESS [AE_TARGET]
        			MT9D112_write_cmos_sensor(0x3390, 0x0080);    // MCU_DATA_0 
        			MT9D112_write_cmos_sensor(0x338C, 0xA216);    // MCU_ADDRESS [AE_TARGET]
			        MT9D112_write_cmos_sensor(0x3390, 0x0078);    // MCU_DATA_0         
        			MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
			        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0
             break;
    }
    return;
}

BOOL MT9D112_set_param_wb(UINT16 para)
{
	SENSORDB("\n ****** MT9D112_set_param_wb \n ");   ///okay
	SENSORDB("MT9D112_set_param_wb = %d\n",para);
     spin_lock(&MT9D112_drv_lock);
     awbMode = para;
   	 spin_unlock(&MT9D112_drv_lock);
   
	  switch (para)
	  { 		   
		  case AWB_MODE_AUTO:
			  {
        MT9D112_write_cmos_sensor(0x338C, 0xA353);    // MCU_ADDRESS [AWB_MODE]
        MT9D112_write_cmos_sensor(0x3390, 0x0002);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA351);    // MCU_ADDRESS [AWB_CCM_POSITION]
        MT9D112_write_cmos_sensor(0x3390, 0x007F);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
 			  } 			   
			  break;
			  
		  case AWB_MODE_CLOUDY_DAYLIGHT:
			  { 
        MT9D112_write_cmos_sensor(0x338C, 0xA353);    // MCU_ADDRESS [AWB_MODE]
        MT9D112_write_cmos_sensor(0x3390, 0x0022);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA351);    // MCU_ADDRESS [AWB_CCM_POSITION]
        MT9D112_write_cmos_sensor(0x3390, 0x006F);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
	          }	      			   
			  break;
			  
		  case AWB_MODE_DAYLIGHT:
			  {
        MT9D112_write_cmos_sensor(0x338C, 0xA353);    // MCU_ADDRESS [AWB_MODE]
        MT9D112_write_cmos_sensor(0x3390, 0x0022);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA351);    // MCU_ADDRESS [AWB_CCM_POSITION]
        MT9D112_write_cmos_sensor(0x3390, 0x007F);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0    
    		  } 	 
			  break;
			  
		  case AWB_MODE_INCANDESCENT: 
			  {
 			  MT9D112_write_cmos_sensor(0x338C, 0xA353);    // MCU_ADDRESS [AWB_MODE]
        MT9D112_write_cmos_sensor(0x3390, 0x0022);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA351);    // MCU_ADDRESS [AWB_CCM_POSITION]
        MT9D112_write_cmos_sensor(0x3390, 0x0010);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
			  } 	  
			  break;  
		  case AWB_MODE_FLUORESCENT:
			  {

        MT9D112_write_cmos_sensor(0x338C, 0xA353);    // MCU_ADDRESS [AWB_MODE]
        MT9D112_write_cmos_sensor(0x3390, 0x0022);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA351);    // MCU_ADDRESS [AWB_CCM_POSITION]
        MT9D112_write_cmos_sensor(0x3390, 0x0079);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
			  }   
			  break;
			  
		  case AWB_MODE_TUNGSTEN:
			 {
			  MT9D112_write_cmos_sensor(0x338C, 0xA353);    // MCU_ADDRESS [AWB_MODE]
        MT9D112_write_cmos_sensor(0x3390, 0x0022);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA351);    // MCU_ADDRESS [AWB_CCM_POSITION]
        MT9D112_write_cmos_sensor(0x3390, 0x0000);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
			 }
			 break;
			 
		  default:
			  return ERROR_NONE;
	  }
  	  
    return ERROR_NONE; 
}

BOOL MT9D112_set_param_effect(UINT16 para)
{
	BOOL ret = TRUE;
	SENSORDB("\n ****** MT9D112_set_param_effect \n ");   ////okay
	SENSORDB("MT9D112_set_param_effect = %d\n",para);

       switch (para)
    {
    case MEFFECT_OFF:
        SENSORDB("[MT9D112YUV]:effect off \n");
        MT9D112_write_cmos_sensor(0x338C, 0x2799);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
        MT9D112_write_cmos_sensor(0x3390, 0x6440);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0x279B);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
        MT9D112_write_cmos_sensor(0x3390, 0x6440);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0
        break;
        
    case MEFFECT_SEPIA:
        SENSORDB("[MT9D112YUV]:effect SEPIA \n");
        //[Special Effect-Sepia]
        MT9D112_write_cmos_sensor(0x338C, 0x2799);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
        MT9D112_write_cmos_sensor(0x3390, 0x6442);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0x279B);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
        MT9D112_write_cmos_sensor(0x3390, 0x6442);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x334A, 0xB023);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0    
        break;  
        
    case MEFFECT_NEGATIVE:        
        SENSORDB("[MT9D112YUV]:effect negative \n");
        //[Special Effect-Negative]
        MT9D112_write_cmos_sensor(0x338C, 0x2799);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
        MT9D112_write_cmos_sensor(0x3390, 0x6443);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0x279B);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
        MT9D112_write_cmos_sensor(0x3390, 0x6443);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0
        break; 
        
    case MEFFECT_SEPIAGREEN:        
        SENSORDB("[MT9D112YUV]:effect sepiaGreen \n");
        MT9D112_write_cmos_sensor(0x338C, 0x2799);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
        MT9D112_write_cmos_sensor(0x3390, 0x6442);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0x279B);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
        MT9D112_write_cmos_sensor(0x3390, 0x6442);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x334A, 0xECAF);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0    
        break;
        
    case MEFFECT_SEPIABLUE:    
        SENSORDB("[MT9D112YUV]:effect sepiablue \n");
        MT9D112_write_cmos_sensor(0x338C, 0x2799);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
        MT9D112_write_cmos_sensor(0x3390, 0x6442);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0x279B);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
        MT9D112_write_cmos_sensor(0x3390, 0x6442);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x334A, 0x4600 );    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0
        break;    
        
    case MEFFECT_MONO:                
        SENSORDB("[MT9D112YUV]:effect mono \n");
        //[Special Effect-Black/White]
        MT9D112_write_cmos_sensor(0x338C, 0x2799);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
        MT9D112_write_cmos_sensor(0x3390, 0x6441);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0x279B);    // MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
        MT9D112_write_cmos_sensor(0x3390, 0x6441);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0005);    // MCU_DATA_0              
        break;

    default:
        ret = FALSE;
    }

    return ret;
} /* MT9D112_set_param_effect */



static void MT9D112_HVMirror(kal_uint8 image_mirror)
{
	SENSORDB("\n ****** MT9D112_HVMirror \n ");
    switch (image_mirror) 
    {
    case IMAGE_NORMAL:
        //[Default Orientation]
        #if 0
        MT9D112_write_cmos_sensor(0x338C, 0x2719);    // MCU_ADDRESS [MODE_SENSOR_READ_MODE_A]
        MT9D112_write_cmos_sensor(0x3390, 0x046C);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0x273B);    // MCU_ADDRESS [MODE_SENSOR_READ_MODE_B]
        MT9D112_write_cmos_sensor(0x3390, 0x0024);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
        #endif
        break;
        
    case IMAGE_H_MIRROR:
        //[Horizontal Mirror]
        #if 0
        MT9D112_write_cmos_sensor(0x338C, 0x2719);    // MCU_ADDRESS [MODE_SENSOR_READ_MODE_A]
        MT9D112_write_cmos_sensor(0x3390, 0x046D);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0x273B);    // MCU_ADDRESS [MODE_SENSOR_READ_MODE_B]
        MT9D112_write_cmos_sensor(0x3390, 0x0025);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
        #endif
        break;
        
    case IMAGE_V_MIRROR:    //Flip Register 0x04[6] and 0x04[4] (FF = 01)
        //[Vertical Flip]
        #if 0
        MT9D112_write_cmos_sensor(0x338C, 0x2719);    // MCU_ADDRESS [MODE_SENSOR_READ_MODE_A]
        MT9D112_write_cmos_sensor(0x3390, 0x046E);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0x273B);    // MCU_ADDRESS [MODE_SENSOR_READ_MODE_B]
        MT9D112_write_cmos_sensor(0x3390, 0x0026);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
        #endif
        break;

    case IMAGE_HV_MIRROR:
        //[Flip and Mirror]
        #if 0
        MT9D112_write_cmos_sensor(0x338C, 0x2719);    // MCU_ADDRESS [MODE_SENSOR_READ_MODE_A]
        MT9D112_write_cmos_sensor(0x3390, 0x046F);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0x273B);    // MCU_ADDRESS [MODE_SENSOR_READ_MODE_B]
        MT9D112_write_cmos_sensor(0x3390, 0x0027);    // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0006);    // MCU_DATA_0
        #endif
        break;

    default:
        ASSERT(0);
    }
}

void MT9D112_set_scene_mode(UINT16 para)
{
	SENSORDB("\n ****** MT9D112_set_scene_mode \n ");	
	SENSORDB("[MT9D112_]  ******MT9D112__set_scene_mode=%d",para);	
    spin_lock(&MT9D112_drv_lock);
    MT9D112_Sensor_Driver.sceneMode= para;
	spin_unlock(&MT9D112_drv_lock);

    switch (para)
    { 
		case SCENE_MODE_NIGHTSCENE:

		    MT9D112_night_mode(1);
		  
			break;
			
        case SCENE_MODE_PORTRAIT:
  

            break;
            
        case SCENE_MODE_LANDSCAPE:


            break;
             
        case SCENE_MODE_SUNSET:
 
            break;
            
        case SCENE_MODE_SPORTS:
  

            break;
            
        case SCENE_MODE_HDR:
 			SENSORDB("SCENE_MODE_HDR");	
MT9D112_night_mode(0);
            break;
            
        case SCENE_MODE_OFF:
        default:
MT9D112_night_mode(0);
           break;
    }

	SENSORDB("[MT9D112]exit MT9D112_set_scene_mode function:\n ");
	return;
}


void MT9D112__get_exposure_gain()
{
    kal_uint32 again = 0, dgain = 0, evTime = 0;
    
    SENSORDB("\n ****** MT9D112__get_exposure_gain\n");
    again = MT9D112_read_cmos_sensor(0x3028);
    //dgain = MT9D112_read_cmos_sensor(0xc8e4);	
    evTime = MT9D112_read_cmos_sensor(0x3012);
    SENSORDB("again=%d, dgain=%d, evTime=%d", again,dgain, evTime);
    spin_lock(&MT9D112_drv_lock);
    MT9D112_Sensor_Driver.currentAGain = again;
    MT9D112_Sensor_Driver.currentDGain = dgain;
    MT9D112_Sensor_Driver.currentExposureTime = evTime;
    spin_unlock(&MT9D112_drv_lock);
}

void MT9D112_GetDelayInfo(UINT32 delayAddr)
{
    SENSOR_DELAY_INFO_STRUCT* pDelayInfo = (SENSOR_DELAY_INFO_STRUCT*)delayAddr;
	SENSORDB("\n ****** MT9D112_GetDelayInfo \n ");
	
    pDelayInfo->InitDelay = 3;
    pDelayInfo->EffectDelay = 3;
    pDelayInfo->AwbDelay = 3;
    pDelayInfo->AFSwitchDelayFrame=50;
}

BOOL MT9D112_set_param_exposure_for_HDR(UINT16 para)
{
    kal_uint32 AGain = 0, DGain = 0, exposureTime = 0;
    kal_uint32 temp = 0;
	SENSORDB("\n ****** MT9D112_set_param_exposure_for_HDR \n ");

    if(0 == MT9D112_Sensor_Driver.manualAEStart)
    {       
        MT9D112__AE_Lock();
        spin_lock(&MT9D112_drv_lock);	
        MT9D112_Sensor_Driver.manualAEStart = 1;
		spin_unlock(&MT9D112_drv_lock);
    }
	AGain = MT9D112_Sensor_Driver.currentAGain;
	DGain = MT9D112_Sensor_Driver.currentDGain;
    exposureTime = MT9D112_Sensor_Driver.currentExposureTime;
    SENSORDB("Kevin== 0x3028_gain = 0x%x,  0x3012_exposureTime=0x%x", MT9D112_read_cmos_sensor(0x3028), MT9D112_read_cmos_sensor(0x3012));
    SENSORDB("AGain = 0x%x, DGain =0x%x, exposureTime=0x%x", AGain, DGain, exposureTime);
      
	switch (para)
	{
	   case AE_EV_COMP_20:	//+2 EV
       case AE_EV_COMP_10:	// +1 EV
            do
            {
                SENSORDB("AE_EV_COMP_20 *2\n");
                temp = MT9D112_Sensor_Driver.currentAGain*2;
                if(temp <= AGAIN_LIMITATION)
                {
                    //AGain = AGain + 8;
                    AGain = 2*MT9D112_Sensor_Driver.currentAGain;
                    MT9D112_write_cmos_sensor(0x3028, AGain);
                }
                else
                {
                   AGain = MT9D112_Sensor_Driver.currentAGain;
                   MT9D112_write_cmos_sensor(0x3028, AGain);
                }
               exposureTime = MT9D112_Sensor_Driver.currentExposureTime<<1;
               MT9D112_write_cmos_sensor(0x3012, exposureTime);	
            }while(0);
 		    break;
		 
	   case AE_EV_COMP_00:	// +0 EV  
    	   {
            SENSORDB("AE_EV_COMP_00 * 0\n");
            MT9D112_write_cmos_sensor(0x3028, MT9D112_Sensor_Driver.currentAGain);
            MT9D112_write_cmos_sensor(0x3012, MT9D112_Sensor_Driver.currentExposureTime);	
           }
		   break;
		 
	   case AE_EV_COMP_n10:  // -1 EV
	   case AE_EV_COMP_n20:  // -2 EV
            do
            {
               SENSORDB("AE_EV_COMP_n10 * 0\n");
                temp = MT9D112_Sensor_Driver.currentAGain/2; // 2X
                if(temp > 0x08 )
                {
                    //AGain = AGain - 8;
                    AGain = MT9D112_Sensor_Driver.currentAGain/2;
                    MT9D112_write_cmos_sensor(0x3028, AGain);
                }
                else
                {
                    AGain = MT9D112_Sensor_Driver.currentAGain;
                    MT9D112_write_cmos_sensor(0x3028, AGain);
                }
                exposureTime = MT9D112_Sensor_Driver.currentExposureTime>>1;
                MT9D112_write_cmos_sensor(0x3012, exposureTime);
            }while(0);
		    break;
		    
	   default:
		 break;//return FALSE;
	}
	SENSORDB("[MT9D112]exit MT9D112_set_param_exposure_for_HDR function:\n ");
	return TRUE;
}

BOOL MT9D112_set_param_exposure(UINT16 para)   /// okay
{
	SENSORDB("\n ****** MT9D112_set_param_exposure \n ");
	SENSORDB("[Enter]MT9D112 set_param_exposure func:para = %d\n",para);
    if (SCENE_MODE_HDR == MT9D112_Sensor_Driver.sceneMode && 
    MT9D112_CAM_CAPTURE == MT9D112_Sensor_Driver.Camco_mode)
    {
      SENSORDB("david88 para = %d\n",para);
      MT9D112_set_param_exposure_for_HDR(para);
      return ERROR_NONE;
    }

	switch (para)
	{	
		case AE_EV_COMP_20:  //+4 EV
			//MT9D112_write_cmos_sensor(0x098E, 0xA401);		// [AE_BASETARGET]
	        //MT9D112_write_cmos_sensor_8(0xA401, 0x00);
	        //MT9D112_write_cmos_sensor_8(0xA805, 0x04);		// [SEQ_CMD]
	               MT9D112_write_cmos_sensor(0x338C, 0xA206);    // MCU_ADDRESS [AE_TARGET]
        MT9D112_write_cmos_sensor(0x3390, 0x0060);    // MCU_DATA_0
	        // MT9D112_write_cmos_sensor(0xA824, 0x0200);
	        //MT9D112_8404_Polling(0x8404, 0x06);
			break;  

		case AE_EV_COMP_10:  //+2 EV
			//MT9D112_write_cmos_sensor(0x098E, 0xA401);		// [AE_BASETARGET]
	        //MT9D112_write_cmos_sensor_8(0xA401, 0x00);
	        //MT9D112_write_cmos_sensor_8(0xA805, 0x04);		// [SEQ_CMD]
	         MT9D112_write_cmos_sensor(0x338C, 0xA206);    // MCU_ADDRESS [AE_TARGET]
        MT9D112_write_cmos_sensor(0x3390, 0x0052);    // MCU_DATA_0
	         //MT9D112_write_cmos_sensor(0xA824, 0x0180);
	        //MT9D112_8404_Polling(0x8404, 0x06);	
	        break;    
  
		case AE_EV_COMP_00:  // +0 EV
    		//MT9D112_write_cmos_sensor(0x098E, 0xA401);		// [AE_BASETARGET] 
            //MT9D112_write_cmos_sensor_8(0xA401, 0x00);                       
            //MT9D112_write_cmos_sensor_8(0xA805, 0x04);		// [SEQ_CMD]       
           MT9D112_write_cmos_sensor(0x338C, 0xA206);    // MCU_ADDRESS [AE_TARGET]
        MT9D112_write_cmos_sensor(0x3390, 0x0046);    // MCU_DATA_0
             //MT9D112_write_cmos_sensor(0xA824, 0x0100);
            //MT9D112_8404_Polling(0x8404, 0x06);
			break;  
			
		
		case AE_EV_COMP_n10:	// -2 EV	
            //MT9D112_write_cmos_sensor(0x098E, 0xA401);	
            //MT9D112_write_cmos_sensor_8(0xA401, 0x00); 
         MT9D112_write_cmos_sensor(0x338C, 0xA206);    // MCU_ADDRESS [AE_TARGET]
        MT9D112_write_cmos_sensor(0x3390, 0x0032);    // MCU_DATA_0
             //MT9D112_write_cmos_sensor(0xA824, 0x0100);
            //MT9D112_8404_Polling(0x8404, 0x06); 
			break;    

		case AE_EV_COMP_n20:  // -4 EV
            //MT9D112_write_cmos_sensor(0x098E, 0xA401);	
            //MT9D112_write_cmos_sensor_8(0xA401, 0x00); 
            //MT9D112_write_cmos_sensor_8(0xA805, 0x04);	
       MT9D112_write_cmos_sensor(0x338C, 0xA206);    // MCU_ADDRESS [AE_TARGET]
        MT9D112_write_cmos_sensor(0x3390, 0x0010);    // MCU_DATA_0
             //MT9D112_write_cmos_sensor(0xA824, 0x0080);
            //MT9D112_8404_Polling(0x8404, 0x06); 
			break;    
		default:
			break;
	}

	return ERROR_NONE;
}



////////////////////////////////////////////////////////////////////////////////////////
//////////////////////init & main function/////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void MT9D112_Init_Para(void)
{
	SENSORDB("******  MT9D112_Init_Para");		
	spin_lock(&MT9D112_drv_lock);
	MT9D112_Sensor_Driver.Preview_PClk = 56;// 12Mhz
	MT9D112_Sensor_Driver.CapturePclk= 56;
	MT9D112_Sensor_Driver.ZsdturePclk= 56;
	MT9D112_Sensor_Driver.Banding = AE_FLICKER_MODE_50HZ;
	MT9D112_Sensor_Driver.userAskAeLock = KAL_FALSE;
	MT9D112_Sensor_Driver.userAskAwbLock = KAL_FALSE;
	MT9D112_Sensor_Driver.manualAEStart=0;	
    MT9D112_Sensor_Driver.currentExposureTime = 0;
    MT9D112_Sensor_Driver.currentAGain = 0;	
    MT9D112_Sensor_Driver.currentDGain = 0;	
	MT9D112_Sensor_Driver.Min_Frame_Rate = 75;
	MT9D112_Sensor_Driver.Max_Frame_Rate = 300;
    MT9D112_Sensor_Driver.NightMode = 0;
	MT9D112_Sensor_Driver.Camco_mode = MT9D112_CAM_PREVIEW;
	spin_unlock(&MT9D112_drv_lock);
}

static kal_uint16 MT9D112_power_on(void)
{
	MT9D112_Sensor_Driver.sensor_id = 0;
	MT9D112_Sensor_Driver.sensor_id = MT9D112_read_cmos_sensor(0x0000);

	SENSORDB("****** MT9D112_power_on\n");
   	
	SENSORDB("[MT9D112]MT9D112_Sensor_Driver.sensor_id =%x\n",MT9D112_Sensor_Driver.sensor_id);
	return MT9D112_Sensor_Driver.sensor_id;
}


UINT32 MT9D112Open(void)
{
  	SENSORDB("****** MT9D112Open\n");

	 if (MT9D112_power_on() != MT9D112_SENSOR_ID) 
 	 {
 	   SENSORDB("[MT9D112]Error:read sensor ID fail\n");
	   return ERROR_SENSOR_CONNECT_FAIL;
 	 }
      
    /* Apply sensor initail setting*/
     MT9D112_Init_Para();
     MT9D112_Initial_Setting();	
     SENSORDB("[Exit]:MT9D112 Open func\n");
	return ERROR_NONE;
}	/* MT9D112Open() */


UINT32 MT9D112GetSensorID(UINT32 *sensorID)
{
  	SENSORDB("****** MT9D112GetSensorID\n");

	 *sensorID = MT9D112_power_on();

    *sensorID = MT9D112_SENSOR_ID;

	 if (*sensorID != MT9D112_SENSOR_ID) 
	 	{
	 	   SENSORDB("[MT9D112]Error:read sensor ID fail\n");
		   *sensorID = 0xFFFFFFFF;
		   return ERROR_SENSOR_CONNECT_FAIL;
	 	}
    
      
     SENSORDB("[Exit]:MT9D112 MT9D112GetSensorID func\n");
     
	return ERROR_NONE;
}	/* MT9D112Open() */

UINT32 MT9D112Close(void)
{
  	SENSORDB("****** MT9D112Close\n");

	return ERROR_NONE;
}	/* MT9D112Close() */


void MT9D112_GetAEAWBLock(UINT32 *pAElockRet32,UINT32 *pAWBlockRet32)
{
    *pAElockRet32 = 1;
	*pAWBlockRet32 = 1;

  	SENSORDB("****** MT9D112_GetAEAWBLock\n");
    SENSORDB("MT9D112_GetAEAWBLock,AE=%d ,AWB=%d\n,",*pAElockRet32,*pAWBlockRet32);
}

UINT32 MT9D112Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
     SENSORDB("******  MT9D112Capture");
     SENSORDB("%d, %d", __LINE__, MT9D112_Sensor_Driver.sceneMode);

    spin_lock(&MT9D112_drv_lock);
	MT9D112_Sensor_Driver.Camco_mode = MT9D112_CAM_CAPTURE;
	spin_unlock(&MT9D112_drv_lock);
	
    if((zsd_setting == 0)
        ||((1==zsd_setting)&&(SCENE_MODE_HDR == MT9D112_Sensor_Driver.sceneMode)))
    {
        MT9D112_write_cmos_sensor(0x338C, 0xA120);      // MCU_ADDRESS [PRI_B_NUM_OF_FRAMES_RUN]
        MT9D112_write_cmos_sensor(0x3390, 0x0072);      // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);      // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0002);      // MCU_DATA_0
        {
            kal_uint16 temp=0;
            while(temp <60)
            {
                    MT9D112_write_cmos_sensor(0x338C, 0xA104);      // MCU_DATA_0
                    if(7==MT9D112_read_cmos_sensor(0x3390))
                {
                 break;
                }
                mdelay(20);//DELAY=100
                temp+=1;
            }
        }
    }
    //MT9D112_Cap_Stream_On();
	image_window->GrabStartX = MT9D112_CAP_START_X;
	image_window->GrabStartY = MT9D112_CAP_START_Y;
	image_window->ExposureWindowWidth = MT9D112_IMAGE_SENSOR_FULL_HACTIVE;
	image_window->ExposureWindowHeight = MT9D112_IMAGE_SENSOR_FULL_VACTIVE;
   if(SCENE_MODE_HDR == MT9D112_Sensor_Driver.sceneMode)
    {  
        SENSORDB("get exposure gain\n");
        MT9D112__get_exposure_gain();
    }

 
	
	return ERROR_NONE;
}

static UINT32 MT9D112Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	SENSORDB("********  MT9D112Preview\n");

	spin_lock(&MT9D112_drv_lock);
    MT9D112_Sensor_Driver.Camco_mode = MT9D112_CAM_PREVIEW;
	MT9D112_Sensor_Driver.Preview_PClk = 56;// 12Mhz
	MT9D112_Sensor_Driver.Min_Frame_Rate = 75;
	MT9D112_Sensor_Driver.Max_Frame_Rate = 300;
    zsd_setting = 0;
	spin_unlock(&MT9D112_drv_lock);
 //[Go to preview]
    MT9D112_write_cmos_sensor(0x338C, 0xA120);      // MCU_ADDRESS [PRI_B_NUM_OF_FRAMES_RUN]
    MT9D112_write_cmos_sensor(0x3390, 0x0000);      // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0xA103);      // MCU_ADDRESS [SEQ_CMD]
    MT9D112_write_cmos_sensor(0x3390, 0x0001);      // MCU_DATA_0
    {    
        kal_uint16 temp=0;
        while(temp <60)
        {
              MT9D112_write_cmos_sensor(0x338C, 0xA104);      // MCU_DATA_0
            if(3==MT9D112_read_cmos_sensor(0x3390))
            {
             break;
            }
            mdelay(20);//DELAY=20
            temp+=1;
        }
    }
	image_window->GrabStartX = MT9D112_PRE_START_X;
	image_window->GrabStartY = MT9D112_PRE_START_Y;
	image_window->ExposureWindowWidth = MT9D112_IMAGE_SENSOR_PV_HACTIVE;
	image_window->ExposureWindowHeight = MT9D112_IMAGE_SENSOR_PV_VACTIVE;



    return ERROR_NONE; 
}	/* MT9D112_Preview */

static UINT32 MT9D112Video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	SENSORDB("********  MT9D112Video\n");

	spin_lock(&MT9D112_drv_lock);
    MT9D112_Sensor_Driver.Camco_mode = MT9D112_CAM_VIDEO;
	MT9D112_Sensor_Driver.Preview_PClk = 56;// 12Mhz
	MT9D112_Sensor_Driver.Min_Frame_Rate = 75;
	MT9D112_Sensor_Driver.Max_Frame_Rate = 300;
	zsd_setting = 0;
	spin_unlock(&MT9D112_drv_lock);

 //[Go to preview]
    MT9D112_write_cmos_sensor(0x338C, 0xA120);      // MCU_ADDRESS [PRI_B_NUM_OF_FRAMES_RUN]
    MT9D112_write_cmos_sensor(0x3390, 0x0000);      // MCU_DATA_0
    MT9D112_write_cmos_sensor(0x338C, 0xA103);      // MCU_ADDRESS [SEQ_CMD]
    MT9D112_write_cmos_sensor(0x3390, 0x0001);      // MCU_DATA_0
    {    
        kal_uint16 temp=0;
        while(temp <60)
        {
              MT9D112_write_cmos_sensor(0x338C, 0xA104);      // MCU_DATA_0
            if(3==MT9D112_read_cmos_sensor(0x3390))
            {
             break;
            }
            mdelay(20);//DELAY=20
            temp+=1;
        }
    }

	image_window->GrabStartX = MT9D112_PRE_START_X;
	image_window->GrabStartY = MT9D112_PRE_START_Y;
	image_window->ExposureWindowWidth = MT9D112_IMAGE_SENSOR_PV_HACTIVE;
	image_window->ExposureWindowHeight = MT9D112_IMAGE_SENSOR_PV_VACTIVE;





    return ERROR_NONE; 
}

void MT9D112_Preview_ZSD_Init(void)
{
	spin_lock(&MT9D112_drv_lock);
    MT9D112_Sensor_Driver.Camco_mode = MT9D112_CAM_PREVIEW_ZSD;
	MT9D112_Sensor_Driver.Preview_PClk = 56;// 12Mhz
	MT9D112_Sensor_Driver.Min_Frame_Rate = 75;
	MT9D112_Sensor_Driver.Max_Frame_Rate = 300;

	spin_unlock(&MT9D112_drv_lock);
}


static UINT32 MT9D112__Preview_ZSD(
    MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
    MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	SENSORDB("******  MT9D112__Preview_ZSD\n");
/*
    //MT9D112_Initial_Setting_ZSD();
    if((SCENE_MODE_HDR != MT9D112_Sensor_Driver.sceneMode) &&
          (MT9D112_CAM_CAPTURE == MT9D112_Sensor_Driver.Camco_mode))
    {
	    MT9D112_Preview_ZSD_Init();
	}
	else
*/	
	
	    MT9D112_Preview_ZSD_Init();
  
	
    //MT9D112_write_cmos_sensor_8(0x843C, 0xFF); 			   // (42) SYS_FIRST_BLACK_LEVEL
    //MT9D112_8404_Polling(0x8404, 0x02); 			   // (42) SYS_FIRST_BLACK_LEVEL
    //MT9D112_8405_Polling(7);
    //MT9D112_Cap_Stream_On();
        MT9D112_write_cmos_sensor(0x338C, 0xA120);      // MCU_ADDRESS [PRI_B_NUM_OF_FRAMES_RUN]
        MT9D112_write_cmos_sensor(0x3390, 0x0072);      // MCU_DATA_0
        MT9D112_write_cmos_sensor(0x338C, 0xA103);      // MCU_ADDRESS [SEQ_CMD]
        MT9D112_write_cmos_sensor(0x3390, 0x0002);      // MCU_DATA_0
        {
            kal_uint16 temp=0;
            while(temp <60)
            {
                    MT9D112_write_cmos_sensor(0x338C, 0xA104);      // MCU_DATA_0
                    if(7==MT9D112_read_cmos_sensor(0x3390))
                {
                 break;
                }
                mdelay(20);//DELAY=100
                temp+=1;
            }
        }
	image_window->GrabStartX = MT9D112_CAP_START_X;
	image_window->GrabStartY = MT9D112_CAP_START_Y;
	image_window->ExposureWindowWidth = MT9D112_IMAGE_SENSOR_FULL_HACTIVE;
	image_window->ExposureWindowHeight = MT9D112_IMAGE_SENSOR_FULL_VACTIVE;



    return ERROR_NONE; 
}


UINT32 MT9D112GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
	SENSORDB("******  MT9D112GetResolution\n");
    SENSORDB("[Enter]:MT9D112 MT9D112_currentScenarioId = %d\n", MT9D112_currentScenarioId);
	pSensorResolution->SensorFullWidth=MT9D112_IMAGE_SENSOR_FULL_HACTIVE;  
	pSensorResolution->SensorFullHeight=MT9D112_IMAGE_SENSOR_FULL_VACTIVE;
    switch(MT9D112_currentScenarioId)
    {
       case MSDK_SCENARIO_ID_CAMERA_ZSD:
            pSensorResolution->SensorPreviewWidth   = MT9D112_IMAGE_SENSOR_FULL_HACTIVE;
            pSensorResolution->SensorPreviewHeight  = MT9D112_IMAGE_SENSOR_FULL_VACTIVE;
            break;
            
       default:
        	pSensorResolution->SensorPreviewWidth=MT9D112_IMAGE_SENSOR_PV_HACTIVE;
        	pSensorResolution->SensorPreviewHeight=MT9D112_IMAGE_SENSOR_PV_VACTIVE;
        	pSensorResolution->SensorVideoWidth=MT9D112_IMAGE_SENSOR_PV_HACTIVE;
        	pSensorResolution->SensorVideoHeight=MT9D112_IMAGE_SENSOR_PV_VACTIVE;
            break;
    }

	
	return ERROR_NONE;
}	/* MT9D112GetResolution() */

UINT32 MT9D112GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
					  MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	SENSORDB("******  MT9D112GetInfo\n");
    SENSORDB("[Enter]:MT9D112 getInfo func:ScenarioId = %d\n",ScenarioId);

    switch(ScenarioId)
    {
        case MSDK_SCENARIO_ID_CAMERA_ZSD:
          pSensorInfo->SensorPreviewResolutionX = MT9D112_IMAGE_SENSOR_FULL_HACTIVE;
          pSensorInfo->SensorPreviewResolutionY = MT9D112_IMAGE_SENSOR_FULL_VACTIVE;
          pSensorInfo->SensorCameraPreviewFrameRate=7;
          break;
        default:
          pSensorInfo->SensorPreviewResolutionX = MT9D112_IMAGE_SENSOR_PV_HACTIVE;
          pSensorInfo->SensorPreviewResolutionY = MT9D112_IMAGE_SENSOR_PV_VACTIVE;
          pSensorInfo->SensorCameraPreviewFrameRate=30;
          break;
    }

	pSensorInfo->SensorFullResolutionX=MT9D112_IMAGE_SENSOR_FULL_HACTIVE;
	pSensorInfo->SensorFullResolutionY=MT9D112_IMAGE_SENSOR_FULL_VACTIVE;
  pSensorInfo->SensorCameraPreviewFrameRate=30;
    pSensorInfo->SensorVideoFrameRate=30;
    pSensorInfo->SensorStillCaptureFrameRate=10;
    pSensorInfo->SensorWebCamCaptureFrameRate=15;
    pSensorInfo->SensorResetActiveHigh=FALSE;
    pSensorInfo->SensorResetDelayCount=1;
    pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_UYVY;
    pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;    
    pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
    pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
    pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_HIGH;
    pSensorInfo->SensorInterruptDelayLines = 1;

    pSensorInfo->CaptureDelayFrame = 3; 
    pSensorInfo->PreviewDelayFrame = 5; 
    pSensorInfo->VideoDelayFrame = 5; 
    pSensorInfo->YUVAwbDelayFrame = 2;  // add by lanking
    pSensorInfo->YUVEffectDelayFrame = 2;  // add by lanking
    pSensorInfo->SensorMasterClockSwitch = 0; 
    pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;           
    pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_PARALLEL;	
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
	      pSensorInfo->SensorClockFreq=24;
        pSensorInfo->SensorClockDividCount=    3;
        pSensorInfo->SensorClockRisingCount= 0;
        pSensorInfo->SensorClockFallingCount= 2;
        pSensorInfo->SensorPixelClockCount= 3;
        pSensorInfo->SensorDataLatchCount= 2;	
			pSensorInfo->SensorGrabStartX = MT9D112_PRE_START_X; 
			pSensorInfo->SensorGrabStartY = MT9D112_PRE_START_Y;
			
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_ZSD:		
       pSensorInfo->SensorClockFreq=24;
        pSensorInfo->SensorClockDividCount=    3;
        pSensorInfo->SensorClockRisingCount= 0;
        pSensorInfo->SensorClockFallingCount= 2;
        pSensorInfo->SensorPixelClockCount= 3;
        pSensorInfo->SensorDataLatchCount= 2;
			pSensorInfo->SensorGrabStartX = MT9D112_CAP_START_X; 
			pSensorInfo->SensorGrabStartY = MT9D112_CAP_START_Y;	
			//add  interface setting

		break;
		default:
       pSensorInfo->SensorClockFreq=24;
        pSensorInfo->SensorClockDividCount=    3;
        pSensorInfo->SensorClockRisingCount= 0;
        pSensorInfo->SensorClockFallingCount= 2;
        pSensorInfo->SensorPixelClockCount= 3;
        pSensorInfo->SensorDataLatchCount= 2;
			pSensorInfo->SensorGrabStartX = MT9D112_PRE_START_X; 
			pSensorInfo->SensorGrabStartY = MT9D112_PRE_START_Y;
            pSensorInfo->SensorPreviewResolutionX = MT9D112_IMAGE_SENSOR_PV_HACTIVE;
            pSensorInfo->SensorPreviewResolutionY = MT9D112_IMAGE_SENSOR_PV_VACTIVE;
        	pSensorInfo->SensorFullResolutionX=MT9D112_IMAGE_SENSOR_FULL_HACTIVE;
        	pSensorInfo->SensorFullResolutionY=MT9D112_IMAGE_SENSOR_FULL_VACTIVE;
		
		break;
	}

	memcpy(pSensorConfigData, &MT9D112SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));	
	return ERROR_NONE;
}	/* MT9D112GetInfo() */


UINT32 MT9D112Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	SENSORDB("******  MT9D112Control\n");

   SENSORDB("[Enter]:MT9D112 Control func:ScenarioId = %d\n",ScenarioId);

    spin_lock(&MT9D112_drv_lock);
	MT9D112_currentScenarioId = ScenarioId;
    spin_unlock(&MT9D112_drv_lock);

	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			 MT9D112Preview(pImageWindow, pSensorConfigData);
			 break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			 MT9D112Video(pImageWindow, pSensorConfigData);
			 break;			 
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:		
			 MT9D112Capture(pImageWindow, pSensorConfigData);
			 break;
        case MSDK_SCENARIO_ID_CAMERA_ZSD:
            MT9D112__Preview_ZSD(pImageWindow, pSensorConfigData);
            break;
			 
		default:
		     break; 
	}

   SENSORDB("[Exit]:MT9D112 Control func\n");
	
	return ERROR_NONE;
}	/* MT9D112Control() */

UINT32 MT9D112YUVSensorSetting(FEATURE_ID iCmd, UINT32 iPara)
{
	SENSORDB("******  MT9D112YUVSensorSetting\n");
    SENSORDB("[Enter]MT9D112YUVSensorSetting func:cmd = %d  === iPara = %d\n",iCmd,iPara);   


	switch (iCmd)
	{
    case FID_SCENE_MODE:	    //auto mode or night mode
    	   MT9D112_set_scene_mode(iPara);    	
	    break; 	  
	    
	case FID_AWB_MODE:
           MT9D112_set_param_wb(iPara);
	     break;
	     
	case FID_COLOR_EFFECT:
           MT9D112_set_param_effect(iPara);
	     break;
	     
	case FID_AE_EV:	
           MT9D112_set_param_exposure(iPara);
	     break;
	     
	case FID_AE_FLICKER:
           MT9D112_set_param_banding(iPara);
          
	     break;
	     
	case FID_AE_SCENE_MODE: 
		if (iPara == AE_MODE_OFF) 
		{            
		    MT9D112__AE_Lock();
		}
    	else 
		{
            MT9D112__AE_UnLock();
 		}
        break; 
        
	case FID_ISP_CONTRAST:
        MT9D112__set_contrast(iPara);
        break;
        
    case FID_ISP_BRIGHT:
        MT9D112__set_brightness(iPara);
        break;
        
    case FID_ISP_SAT:
        MT9D112__set_saturation(iPara);
        break;
        
	case FID_ZOOM_FACTOR:
		        //MT9D112SENSORDB("FID_ZOOM_FACTOR:%d\n", iPara); 	    
				//spin_lock(&MT9D112_drv_lock);
                //zoom_factor = iPara; 
				//spin_unlock(&MT9D112_drv_lock);
        break;
        
	case FID_AE_ISO:
        MT9D112__set_iso(iPara);
        break;
	     
	default:
	     break;
	}

	return ERROR_NONE;
}   /* MT9D112YUVSensorSetting */

UINT32 MT9D112YUVSetVideoMode(UINT16 u2FrameRate)
{
    SENSORDB("*******  MT9D112YUVSetVideoMode\n");

	if (u2FrameRate == 30)
    {

    }
    else if (u2FrameRate == 15)       
    {

    }
  
	
    return ERROR_NONE;
}

void MT9D112_GetAFMaxNumFocusAreas(UINT32 *pFeatureReturnPara32)
{	
    *pFeatureReturnPara32 = 0;    

	SENSORDB("******  MT9D112_GetAFMaxNumFocusAreas\n");
    
    SENSORDB("MT9V113_GetAFMaxNumFocusAreas, *pFeatureReturnPara32 = %d\n",*pFeatureReturnPara32);

}

void MT9D112_GetAFMaxNumMeteringAreas(UINT32 *pFeatureReturnPara32)
{	
    *pFeatureReturnPara32 = 0;   

	SENSORDB("******  MT9D112_GetAFMaxNumMeteringAreas\n");
}

#if 0
UINT32 MT9D112MaxFramerateByScenario(MSDK_SCENARIO_ID_ENUM scenarioId, MUINT32 frameRate) 
{
		kal_uint32 pclk;
		kal_int16 dummyLine;
		kal_uint16 lineLength,frameHeight;
		MT9D112SENSORDB("MT9D112MaxFramerateByScenario: scenarioId = %d, frame rate = %d\n",scenarioId,frameRate);
		MT9D112SENSORDB("[MT9D112]enter MT9D112MaxFramerateByScenario function:\n ");
		switch (scenarioId) {
			case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
				pclk = 56000000;
				lineLength = MT9D112_IMAGE_SENSOR_SVGA_WIDTH;
				frameHeight = (10 * pclk)/frameRate/lineLength;
				dummyLine = frameHeight - MT9D112_IMAGE_SENSOR_SVGA_HEIGHT;
				if(dummyLine<0)
					dummyLine = 0;
				spin_lock(&MT9D112_drv_lock);
				MT9D112Sensor.SensorMode= SENSOR_MODE_PREVIEW;
				MT9D112Sensor.PreviewDummyLines = dummyLine;
				spin_unlock(&MT9D112_drv_lock);
				MT9D112SetDummy(MT9D112Sensor.PreviewDummyPixels, dummyLine);			
				break;			
			case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
				pclk = 56000000;
				lineLength = MT9D112_IMAGE_SENSOR_VIDEO_WITDH;
				frameHeight = (10 * pclk)/frameRate/lineLength;
				dummyLine = frameHeight - MT9D112_IMAGE_SENSOR_VIDEO_HEIGHT;
				if(dummyLine<0)
					dummyLine = 0;
				//spin_lock(&MT9D112_drv_lock);
				//ov8825.sensorMode = SENSOR_MODE_VIDEO;
				//spin_unlock(&MT9D112_drv_lock);
				MT9D112SetDummy(0, dummyLine);			
				break;			
			case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			case MSDK_SCENARIO_ID_CAMERA_ZSD:			
				pclk = 90000000;
				lineLength = MT9D112_IMAGE_SENSOR_QSXGA_WITDH;
				frameHeight = (10 * pclk)/frameRate/lineLength;
				dummyLine = frameHeight - MT9D112_IMAGE_SENSOR_QSXGA_HEIGHT;
				if(dummyLine<0)
					dummyLine = 0;
				spin_lock(&MT9D112_drv_lock);
				MT9D112Sensor.CaptureDummyLines = dummyLine;
				MT9D112Sensor.SensorMode= SENSOR_MODE_CAPTURE;
				spin_unlock(&MT9D112_drv_lock);
				MT9D112SetDummy(MT9D112Sensor.CaptureDummyPixels, dummyLine);			
				break;		
			case MSDK_SCENARIO_ID_CAMERA_3D_PREVIEW: //added
				break;
			case MSDK_SCENARIO_ID_CAMERA_3D_VIDEO:
				break;
			case MSDK_SCENARIO_ID_CAMERA_3D_CAPTURE: //added   
				break;		
			default:
				break;
		}	
		MT9D112SENSORDB("[MT9D112]exit MT9D112MaxFramerateByScenario function:\n ");
		return ERROR_NONE;
}
#endif


UINT32 MT9D112GetDefaultFramerateByScenario(MSDK_SCENARIO_ID_ENUM scenarioId, MUINT32 *pframeRate) 
{


	SENSORDB("******  MT9D112GetDefaultFramerateByScenario\n");

	switch (scenarioId) {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			 *pframeRate = 300;
			 break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			 *pframeRate = 75;
			break;		
        case MSDK_SCENARIO_ID_CAMERA_3D_PREVIEW: //added
        case MSDK_SCENARIO_ID_CAMERA_3D_VIDEO:
        case MSDK_SCENARIO_ID_CAMERA_3D_CAPTURE: //added   
			 *pframeRate = 300;
			break;		
		default:
			break;
	}

	return ERROR_NONE;
}

void MT9D112_GetExifInfo(UINT32 exifAddr)
{
	SENSORDB("****** MT9D112GetExifInfo\n");
    SENSOR_EXIF_INFO_STRUCT* pExifInfo = (SENSOR_EXIF_INFO_STRUCT*)exifAddr;
    pExifInfo->FNumber = 28;
    pExifInfo->AEISOSpeed = AE_ISO_100;
    pExifInfo->AWBMode = AWB_MODE_AUTO;
    pExifInfo->CapExposureTime = 0;
    pExifInfo->FlashLightTimeus = 0;
    pExifInfo->RealISOValue = AE_ISO_100;
}

void MT9D112_3ACtrl(ACDK_SENSOR_3A_LOCK_ENUM action)
{

	SENSORDB("****** MT9D112_3ACtrl\n");
	SENSORDB("[MT9D112]enter ACDK_SENSOR_3A_LOCK_ENUM function:action=%d\n",action);
   switch (action)
   {
      case SENSOR_3A_AE_LOCK:
          spin_lock(&MT9D112_drv_lock);
          MT9D112_Sensor_Driver.userAskAeLock = KAL_TRUE;
          spin_unlock(&MT9D112_drv_lock);
          MT9D112__AE_Lock();

      break;
      case SENSOR_3A_AE_UNLOCK:
          spin_lock(&MT9D112_drv_lock);
          MT9D112_Sensor_Driver.userAskAeLock = KAL_FALSE;
          spin_unlock(&MT9D112_drv_lock);
          MT9D112__AE_UnLock();

      break;

      case SENSOR_3A_AWB_LOCK:
          spin_lock(&MT9D112_drv_lock);
          MT9D112_Sensor_Driver.userAskAwbLock = KAL_TRUE;
          spin_unlock(&MT9D112_drv_lock);
          MT9D112__AWB_Lock();
      break;

      case SENSOR_3A_AWB_UNLOCK:
          spin_lock(&MT9D112_drv_lock);
          MT9D112_Sensor_Driver.userAskAwbLock = KAL_FALSE;
          spin_unlock(&MT9D112_drv_lock);
          MT9D112__AWB_UnLock();
      break;
      default:
      	break;
   }
   SENSORDB("[MT9D112]exit ACDK_SENSOR_3A_LOCK_ENUM function:action=%d\n",action);
   return;
}


UINT32 MT9D112FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,
							 UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
{
    //UINT16 u2Temp = 0; 
	UINT16 *pFeatureReturnPara16=(UINT16 *) pFeaturePara;
	UINT16 *pFeatureData16=(UINT16 *) pFeaturePara;
	UINT32 *pFeatureReturnPara32=(UINT32 *) pFeaturePara;
	UINT32 *pFeatureData32=(UINT32 *) pFeaturePara;
	MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData=(MSDK_SENSOR_CONFIG_STRUCT *) pFeaturePara;
	MSDK_SENSOR_REG_INFO_STRUCT *pSensorRegData=(MSDK_SENSOR_REG_INFO_STRUCT *) pFeaturePara;

	switch (FeatureId)
	{
		case SENSOR_FEATURE_GET_RESOLUTION:
			*pFeatureReturnPara16++=MT9D112_IMAGE_SENSOR_FULL_HACTIVE;
			*pFeatureReturnPara16=MT9D112_IMAGE_SENSOR_FULL_VACTIVE;
			*pFeatureParaLen=4;
		     break;
		     
		case SENSOR_FEATURE_GET_PERIOD:
			switch(MT9D112_currentScenarioId)
			{
				case MSDK_SCENARIO_ID_CAMERA_ZSD:
					*pFeatureReturnPara16++=MT9D112_IMAGE_SENSOR_FULL_HACTIVE;
					*pFeatureReturnPara16=MT9D112_IMAGE_SENSOR_FULL_VACTIVE;
					*pFeatureParaLen=4;
					break;
					
				default:
        			*pFeatureReturnPara16++=MT9D112_IMAGE_SENSOR_PV_HACTIVE;
        			*pFeatureReturnPara16=MT9D112_IMAGE_SENSOR_PV_VACTIVE;
        			*pFeatureParaLen=4;
		        break;
		     }
		     break;
		     
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:    
			switch(MT9D112_currentScenarioId)
			{
				case MSDK_SCENARIO_ID_CAMERA_ZSD:
					*pFeatureReturnPara32 = MT9D112_Sensor_Driver.ZsdturePclk * 1000 *1000;	 //unit: Hz				
					*pFeatureParaLen=4;
					break;
				default:
        			*pFeatureReturnPara32 = MT9D112_Sensor_Driver.Preview_PClk* 1000*1000;
        			*pFeatureParaLen=4;
					break;
			}			
		    break;
		    
		case SENSOR_FEATURE_SET_ESHUTTER:	
		     break;
		     
		case SENSOR_FEATURE_GET_EXIF_INFO:
            MT9D112_GetExifInfo(*pFeatureData32);
            break;
		     
		case SENSOR_FEATURE_SET_NIGHTMODE:
			 MT9D112_night_mode((BOOL) *pFeatureData16);
		     break;
		     
		case SENSOR_FEATURE_SET_GAIN:
			 break; 
			 
		case SENSOR_FEATURE_SET_FLASHLIGHT:
		     break;
		     
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
		     break;
		     
		case SENSOR_FEATURE_SET_REGISTER:
			 MT9D112_write_cmos_sensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
		     break;
		case SENSOR_FEATURE_GET_REGISTER:
			 pSensorRegData->RegData = MT9D112_read_cmos_sensor(pSensorRegData->RegAddr);
		     break;
		case SENSOR_FEATURE_GET_CONFIG_PARA:
			 memcpy(pSensorConfigData, &MT9D112SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
			 *pFeatureParaLen=sizeof(MSDK_SENSOR_CONFIG_STRUCT);
		     break;
		     
		case SENSOR_FEATURE_SET_CCT_REGISTER:
		case SENSOR_FEATURE_GET_CCT_REGISTER:
		case SENSOR_FEATURE_SET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_REGISTER_DEFAULT:
		case SENSOR_FEATURE_CAMERA_PARA_TO_SENSOR:
		case SENSOR_FEATURE_SENSOR_TO_CAMERA_PARA:
		case SENSOR_FEATURE_GET_GROUP_INFO:
		case SENSOR_FEATURE_GET_ITEM_INFO:
		case SENSOR_FEATURE_SET_ITEM_INFO:
		case SENSOR_FEATURE_GET_ENG_INFO:
		     break;
		case SENSOR_FEATURE_GET_GROUP_COUNT:
             *pFeatureReturnPara32++=0;
             *pFeatureParaLen=4;
		     break; 
		
		case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
			// get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
			// if EEPROM does not exist in camera module.
			*pFeatureReturnPara32=LENS_DRIVER_ID_DO_NOT_CARE;
			*pFeatureParaLen=4;
		     break;


		case SENSOR_FEATURE_SET_YUV_CMD:
			 MT9D112YUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
		     break;	
		case SENSOR_FEATURE_SET_YUV_3A_CMD:
            MT9D112_3ACtrl((ACDK_SENSOR_3A_LOCK_ENUM)*pFeatureData32);
            break;
		     
		case SENSOR_FEATURE_SET_VIDEO_MODE:
		     MT9D112YUVSetVideoMode(*pFeatureData16);
		     break; 
		case SENSOR_FEATURE_CHECK_SENSOR_ID:
             MT9D112GetSensorID(pFeatureReturnPara32); 
            break;  

		case SENSOR_FEATURE_GET_AE_AWB_LOCK_INFO:
			MT9D112_GetAEAWBLock(*pFeatureData32, *(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_GET_DELAY_INFO:
			SENSORDB("SENSOR_FEATURE_GET_DELAY_INFO\n");
			MT9D112_GetDelayInfo(*pFeatureData32);
			break;

		default:
			 break;			
	}
	return ERROR_NONE;
}	/* MT9D112FeatureControl() */

 SENSOR_FUNCTION_STRUCT	SensorFuncMT9D112=
{
	MT9D112Open,
	MT9D112GetInfo,
	MT9D112GetResolution,
	MT9D112FeatureControl,
	MT9D112Control,
	MT9D112Close
};

UINT32 MT9D112_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
    /* To Do : Check Sensor status here */
    if (pfFunc!=NULL)
        *pfFunc=&SensorFuncMT9D112;

    return ERROR_NONE;
}   /* SensorInit() */
