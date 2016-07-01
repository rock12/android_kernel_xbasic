/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2005
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
 *   GC0329Syuv_Sensor.c
 *
 * Project:
 * --------
 *   MAUI
 *
 * Description:
 * ------------
 *   Image sensor driver function
 *   V1.2.3
 *
 * Author:
 * -------
 *   Leo
 *
 *=============================================================
 *             HISTORY
 * Below this line, this part is controlled by GCoreinc. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Log: gc0329syuv_Sensor.c,v $
 * Revision 1.2  2013/09/22 06:49:30  dinghongzhen
 * huanwenjian modify
 *
 * Revision 1.1  2013/07/17 02:13:39  huangwenjian
 * Bug[none]
 *
 * Reason:13C43 Y05C2 compatible hm5065
 * Author:Huangwenjian
 *
 * Revision 1.1  2013/07/09 07:28:56  huangwenjian
 * Bug[none]
 *
 * Reason:13C43 Y05C1 borad debug
 * Author:Huangwenjian
 *
 * Revision 1.1  2013/06/27 07:50:24  wangqiang
 * 13E43-H82
 * Ч���Ż���
 *
 * Revision 1.3  2013/06/14 12:36:18  maliejun
 * driver update
 *
 * Revision 1.2  2013/06/05 07:54:34  birdlibo
 * no message
 *
 * Revision 1.1  2013/05/23 03:43:01  ningzhiyu
 * Editor: ningzhiyu
 * Inspector:ningzhiyu
 * Reason: create 72 JB3 module
 * Reference:none
 *
 * Revision 1.6  2012/09/17 12:58:13  wangqiang
 * Bug 86348:ҹ��ģʽ������������˳����κ󣬻���ʾ���ܱ�Ǹ�������ֹͣ����
 *
 * Revision 1.5  2012/09/10 11:38:04  wangtinglong
 * 12K21 X21
 *
 * Revision 1.4  2012/08/28 09:36:36  luxujun
 * //yanan modify for camer driver
 *
 * Revision 1.3  2012/07/30 06:15:16  maliejun
 * Editor: maliejun
 * Inspector:maliejun
 * Reason:[84290] [N803]������Ҫ�Ƶ�ICS_V2��
 * Reference:none
 *
 * Revision 1.2  2012/07/05 02:18:04  zhuyaozhong
 * merge code from BIRD75_ICS_V1
 *
 * Revision 1.3  2012/06/18 01:45:33  birdlibo
 * no message
 *
 * Revision 1.3  2012/06/07 01:26:35  birdlibo
 * no message
 *
 * 2012.02.29  kill bugs
 *   
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by GCoreinc. DO NOT MODIFY!!
 *=============================================================
 ******************************************************************************/
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>

#include "kd_camera_hw.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"

#include "gc0329syuv_Sensor.h"
#include "gc0329syuv_Camera_Sensor_para.h"
#include "gc0329syuv_CameraCustomized.h"

#define GC0329SYUV_DEBUG
#ifdef GC0329SYUV_DEBUG
#define SENSORDB printk
#else
#define SENSORDB(x,...)
#endif

extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);

kal_uint16 GC0329S_write_cmos_sensor(kal_uint8 addr, kal_uint8 para)
{
    char puSendCmd[2] = {(char)(addr & 0xFF) , (char)(para & 0xFF)};
	
	iWriteRegI2C(puSendCmd , 2, GC0329S_WRITE_ID);

}
kal_uint16 GC0329S_read_cmos_sensor(kal_uint8 addr)
{
	kal_uint16 get_byte=0;
    char puSendCmd = { (char)(addr & 0xFF) };
	iReadRegI2C(&puSendCmd , 1, (u8*)&get_byte, 1, GC0329S_READ_ID);
	
    return get_byte;
}


/*******************************************************************************
 * // Adapter for Winmo typedef
 ********************************************************************************/
#define WINMO_USE 0

#define Sleep(ms) mdelay(ms)
#define RETAILMSG(x,...)
#define TEXT

kal_bool   GC0329S_MPEG4_encode_mode = KAL_FALSE;
kal_uint16 GC0329S_dummy_pixels = 0, GC0329S_dummy_lines = 0;
kal_bool   GC0329S_MODE_CAPTURE = KAL_FALSE;
kal_bool   GC0329S_NIGHT_MODE = KAL_FALSE;

kal_uint32 GC0329S_isp_master_clock;
static kal_uint32 GC0329S_g_fPV_PCLK = 26;

kal_uint8 GC0329S_sensor_write_I2C_address = GC0329S_WRITE_ID;
kal_uint8 GC0329S_sensor_read_I2C_address = GC0329S_READ_ID;

UINT8 GC0329SPixelClockDivider=0;

MSDK_SENSOR_CONFIG_STRUCT GC0329SSensorConfigData;

#define GC0329S_SET_PAGE0 	GC0329S_write_cmos_sensor(0xfe, 0x00)
#define GC0329S_SET_PAGE1 	GC0329S_write_cmos_sensor(0xfe, 0x01)

void GC0329SGammaSelect(kal_uint32 GammaLvl);
/*************************************************************************
 * FUNCTION
 *	GC0329S_SetShutter
 *
 * DESCRIPTION
 *	This function set e-shutter of GC0329S to change exposure time.
 *
 * PARAMETERS
 *   iShutter : exposured lines
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
void GC0329S_Set_Shutter(kal_uint16 iShutter)
{
} /* Set_GC0329S_Shutter */


/*************************************************************************
 * FUNCTION
 *	GC0329S_read_Shutter
 *
 * DESCRIPTION
 *	This function read e-shutter of GC0329S .
 *
 * PARAMETERS
 *  None
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
kal_uint16 GC0329S_Read_Shutter(void)
{
    	kal_uint8 temp_reg1, temp_reg2;
	kal_uint16 shutter;

	temp_reg1 = GC0329S_read_cmos_sensor(0x04);
	temp_reg2 = GC0329S_read_cmos_sensor(0x03);

	shutter = (temp_reg1 & 0xFF) | (temp_reg2 << 8);

	return shutter;
} /* GC0329S_read_shutter */


/*************************************************************************
 * FUNCTION
 *	GC0329S_write_reg
 *
 * DESCRIPTION
 *	This function set the register of GC0329S.
 *
 * PARAMETERS
 *	addr : the register index of GC0329S
 *  para : setting parameter of the specified register of GC0329S
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
void GC0329S_write_reg(kal_uint32 addr, kal_uint32 para)
{
	GC0329S_write_cmos_sensor(addr, para);
} /* GC0329S_write_reg() */


/*************************************************************************
 * FUNCTION
 *	GC0329S_read_cmos_sensor
 *
 * DESCRIPTION
 *	This function read parameter of specified register from GC0329S.
 *
 * PARAMETERS
 *	addr : the register index of GC0329S
 *
 * RETURNS
 *	the data that read from GC0329S
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
kal_uint32 GC0329S_read_reg(kal_uint32 addr)
{
	return GC0329S_read_cmos_sensor(addr);
} /* OV7670_read_reg() */


/*************************************************************************
* FUNCTION
*	GC0329S_awb_enable
*
* DESCRIPTION
*	This function enable or disable the awb (Auto White Balance).
*
* PARAMETERS
*	1. kal_bool : KAL_TRUE - enable awb, KAL_FALSE - disable awb.
*
* RETURNS
*	kal_bool : It means set awb right or not.
*
*************************************************************************/
static void GC0329S_awb_enable(kal_bool enalbe)
{	 
	kal_uint16 temp_AWB_reg = 0;

	temp_AWB_reg = GC0329S_read_cmos_sensor(0x42);
	
	if (enalbe)
	{
		GC0329S_write_cmos_sensor(0x42, (temp_AWB_reg |0x02));
	}
	else
	{
		GC0329S_write_cmos_sensor(0x42, (temp_AWB_reg & (~0x02)));
	}

}


/*************************************************************************
 * FUNCTION
 *	GC0329S_config_window
 *
 * DESCRIPTION
 *	This function config the hardware window of GC0329S for getting specified
 *  data of that window.
 *
 * PARAMETERS
 *	start_x : start column of the interested window
 *  start_y : start row of the interested window
 *  width  : column widht of the itnerested window
 *  height : row depth of the itnerested window
 *
 * RETURNS
 *	the data that read from GC0329S
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
void GC0329S_config_window(kal_uint16 startx, kal_uint16 starty, kal_uint16 width, kal_uint16 height)
{
} /* GC0329S_config_window */


/*************************************************************************
 * FUNCTION
 *	GC0329S_SetGain
 *
 * DESCRIPTION
 *	This function is to set global gain to sensor.
 *
 * PARAMETERS
 *   iGain : sensor global gain(base: 0x40)
 *
 * RETURNS
 *	the actually gain set to sensor.
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
kal_uint16 GC0329S_SetGain(kal_uint16 iGain)
{
	return iGain;
}


/*************************************************************************
 * FUNCTION
 *	GC0329S_NightMode
 *
 * DESCRIPTION
 *	This function night mode of GC0329S.
 *
 * PARAMETERS
 *	bEnable: KAL_TRUE -> enable night mode, otherwise, disable night mode
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
void GC0329SNightMode(kal_bool bEnable)
{
	if (bEnable)
	{	
		GC0329S_write_cmos_sensor(0xfe, 0x01);
		if(GC0329S_MPEG4_encode_mode == KAL_TRUE)
			GC0329S_write_cmos_sensor(0x33, 0x00);
		else
			GC0329S_write_cmos_sensor(0x33, 0x30);
		GC0329S_write_cmos_sensor(0xfe, 0x00);
		GC0329SGammaSelect(GC0329S_RGB_Gamma_night);
		GC0329S_NIGHT_MODE = KAL_TRUE;
	}
	else 
	{
		GC0329S_write_cmos_sensor(0xfe, 0x01);
		if(GC0329S_MPEG4_encode_mode == KAL_TRUE)
			GC0329S_write_cmos_sensor(0x33, 0x00);
		else
			GC0329S_write_cmos_sensor(0x33, 0x20);
		GC0329S_write_cmos_sensor(0xfe, 0x00);
		GC0329SGammaSelect(GC0329S_RGB_Gamma_m3);
		GC0329S_NIGHT_MODE = KAL_FALSE;
	}
} /* GC0329S_NightMode */

/*************************************************************************
* FUNCTION
*	GC0329S_Sensor_Init
*
* DESCRIPTION
*	This function apply all of the initial setting to sensor.
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
*************************************************************************/
void GC0329S_Sensor_Init(void)
{
	GC0329S_write_cmos_sensor(0xfe, 0x80);
	GC0329S_write_cmos_sensor(0xfe, 0x80);
	GC0329S_write_cmos_sensor(0xfc, 0x16);
	GC0329S_write_cmos_sensor(0xfc, 0x16);
	GC0329S_write_cmos_sensor(0xfc, 0x16);
	GC0329S_write_cmos_sensor(0xfc, 0x16);
	GC0329S_SET_PAGE0;
	//GC0329S_write_cmos_sensor(0xf0, 0x07);
	GC0329S_write_cmos_sensor(0xf1, 0x01);

	GC0329S_write_cmos_sensor(0x73, 0x90);
	GC0329S_write_cmos_sensor(0x74, 0x80);
	GC0329S_write_cmos_sensor(0x75, 0x80);
	GC0329S_write_cmos_sensor(0x76, 0x94);

	GC0329S_write_cmos_sensor(0x42, 0x00);
	GC0329S_write_cmos_sensor(0x77, 0x57);
	GC0329S_write_cmos_sensor(0x78, 0x4d);
	GC0329S_write_cmos_sensor(0x79, 0x45);
	//GC0329S_write_cmos_sensor(0x42, 0xfc);

	////////////////////analog////////////////////
	GC0329S_write_cmos_sensor(0x0a, 0x02);
	GC0329S_write_cmos_sensor(0x0c, 0x02);
#if defined(BIRD_12TX6_N803C)||defined(BIRD_12TX6_N804)||defined(BIRD_GC0329S_HV_MIRROR)
	GC0329S_write_cmos_sensor(0x17, 0x17);
#else
	GC0329S_write_cmos_sensor(0x17, 0x14); // 16   lanking 20120423  mirror setting
#endif
	GC0329S_write_cmos_sensor(0x19, 0x05);
	GC0329S_write_cmos_sensor(0x1b, 0x24);
	GC0329S_write_cmos_sensor(0x1c, 0x04);
	GC0329S_write_cmos_sensor(0x1e, 0x08);
	GC0329S_write_cmos_sensor(0x1f, 0xc0);
	GC0329S_write_cmos_sensor(0x20, 0x00);
	GC0329S_write_cmos_sensor(0x21, 0x48);
	GC0329S_write_cmos_sensor(0x22, 0xba);
	GC0329S_write_cmos_sensor(0x23, 0x22);
	GC0329S_write_cmos_sensor(0x24, 0x16);

	////////////////////blk////////////////////
	GC0329S_write_cmos_sensor(0x26, 0xf7); 
	GC0329S_write_cmos_sensor(0x29, 0x80); 
	GC0329S_write_cmos_sensor(0x32, 0x04);
	GC0329S_write_cmos_sensor(0x33, 0x20);
	GC0329S_write_cmos_sensor(0x34, 0x20);
	GC0329S_write_cmos_sensor(0x35, 0x20);
	GC0329S_write_cmos_sensor(0x36, 0x20);

	////////////////////ISP BLOCK ENABL////////////////////
	GC0329S_write_cmos_sensor(0x40, 0xff);
	GC0329S_write_cmos_sensor(0x41, 0x44);
	GC0329S_write_cmos_sensor(0x42, 0x7e);
	GC0329S_write_cmos_sensor(0x44, 0xa0);
	GC0329S_write_cmos_sensor(0x46, 0x02);
	GC0329S_write_cmos_sensor(0x4b, 0xca);
	GC0329S_write_cmos_sensor(0x4d, 0x01);
	GC0329S_write_cmos_sensor(0x4f, 0x01);
	GC0329S_write_cmos_sensor(0x70, 0x48);

	//GC0329S_write_cmos_sensor(0xb0, 0x00);
	//GC0329S_write_cmos_sensor(0xbc, 0x00);
	//GC0329S_write_cmos_sensor(0xbd, 0x00);
	//GC0329S_write_cmos_sensor(0xbe, 0x00);
	////////////////////DNDD////////////////////
	GC0329S_write_cmos_sensor(0x80, 0xe7); 
	GC0329S_write_cmos_sensor(0x82, 0x5f);//55  
	GC0329S_write_cmos_sensor(0x83, 0x01);  // add by lanking 20120423
	GC0329S_write_cmos_sensor(0x87, 0x4a);

	////////////////////INTPEE////////////////////
	GC0329S_write_cmos_sensor(0x95, 0x54);

	////////////////////ASDE////////////////////
	//GC0329S_SET_PAGE1;
	//GC0329S_write_cmos_sensor(0x18, 0x22);
	//GC0329S_SET_PAGE0;
	//GC0329S_write_cmos_sensor(0x9c, 0x0a);
	//GC0329S_write_cmos_sensor(0xa0, 0xaf);
	//GC0329S_write_cmos_sensor(0xa2, 0xff);
	//GC0329S_write_cmos_sensor(0xa4, 0x50);
	//GC0329S_write_cmos_sensor(0xa5, 0x21);
	//GC0329S_write_cmos_sensor(0xa7, 0x35);

	////////////////////RGB gamma////////////////////
	//RGB gamma m4'
	GC0329S_write_cmos_sensor(0xbf, 0x06);
	GC0329S_write_cmos_sensor(0xc0, 0x14);
	GC0329S_write_cmos_sensor(0xc1, 0x27);
	GC0329S_write_cmos_sensor(0xc2, 0x3b);
	GC0329S_write_cmos_sensor(0xc3, 0x4f);
	GC0329S_write_cmos_sensor(0xc4, 0x62);
	GC0329S_write_cmos_sensor(0xc5, 0x72);
	GC0329S_write_cmos_sensor(0xc6, 0x8d);
	GC0329S_write_cmos_sensor(0xc7, 0xa4);
	GC0329S_write_cmos_sensor(0xc8, 0xb8);
	GC0329S_write_cmos_sensor(0xc9, 0xc9);
	GC0329S_write_cmos_sensor(0xcA, 0xd6);
	GC0329S_write_cmos_sensor(0xcB, 0xe0);
	GC0329S_write_cmos_sensor(0xcC, 0xe8);
	GC0329S_write_cmos_sensor(0xcD, 0xf4);
	GC0329S_write_cmos_sensor(0xcE, 0xFc);
	GC0329S_write_cmos_sensor(0xcF, 0xFF);

	//////////////////CC///////////////////
	GC0329S_SET_PAGE0;

	GC0329S_write_cmos_sensor(0xb3, 0x44);
	GC0329S_write_cmos_sensor(0xb4, 0xfd);
	GC0329S_write_cmos_sensor(0xb5, 0x02);
	GC0329S_write_cmos_sensor(0xb6, 0xfa);
	GC0329S_write_cmos_sensor(0xb7, 0x48);
	GC0329S_write_cmos_sensor(0xb8, 0xf0);

	// crop 						   
	GC0329S_write_cmos_sensor(0x50, 0x01);

	////////////////////YCP////////////////////
	GC0329S_SET_PAGE0;
	GC0329S_write_cmos_sensor(0xd1, 0x38);
	GC0329S_write_cmos_sensor(0xd2, 0x38);
	GC0329S_write_cmos_sensor(0xdd, 0x54);

	////////////////////AEC////////////////////
	GC0329S_SET_PAGE1;
	GC0329S_write_cmos_sensor(0x10, 0x40);
	GC0329S_write_cmos_sensor(0x11, 0x21);
	GC0329S_write_cmos_sensor(0x12, 0x07);
	GC0329S_write_cmos_sensor(0x13, 0x50);
	GC0329S_write_cmos_sensor(0x17, 0x88);
	GC0329S_write_cmos_sensor(0x21, 0xb0);
	GC0329S_write_cmos_sensor(0x22, 0x48);
	GC0329S_write_cmos_sensor(0x3c, 0x95);
	GC0329S_write_cmos_sensor(0x3d, 0x50);
	GC0329S_write_cmos_sensor(0x3e, 0x48); 

	////////////////////AWB////////////////////
	GC0329S_SET_PAGE1;
	GC0329S_write_cmos_sensor(0x06, 0x16);
	GC0329S_write_cmos_sensor(0x07, 0x06);
	GC0329S_write_cmos_sensor(0x08, 0x98);
	GC0329S_write_cmos_sensor(0x09, 0xee);
	GC0329S_write_cmos_sensor(0x50, 0xfc);
	GC0329S_write_cmos_sensor(0x51, 0x20);
	GC0329S_write_cmos_sensor(0x52, 0x0b);
	GC0329S_write_cmos_sensor(0x53, 0x20);
	GC0329S_write_cmos_sensor(0x54, 0x40);
	GC0329S_write_cmos_sensor(0x55, 0x10);
	GC0329S_write_cmos_sensor(0x56, 0x20);
	//GC0329S_write_cmos_sensor(0x57, 0x40);
	GC0329S_write_cmos_sensor(0x58, 0xa0);
	GC0329S_write_cmos_sensor(0x59, 0x28);
	GC0329S_write_cmos_sensor(0x5a, 0x02);
	GC0329S_write_cmos_sensor(0x5b, 0x63);
	GC0329S_write_cmos_sensor(0x5c, 0x34);
	GC0329S_write_cmos_sensor(0x5d, 0x73);
	GC0329S_write_cmos_sensor(0x5e, 0x11);
	GC0329S_write_cmos_sensor(0x5f, 0x40);
	GC0329S_write_cmos_sensor(0x60, 0x40);
	GC0329S_write_cmos_sensor(0x61, 0xc8);
	GC0329S_write_cmos_sensor(0x62, 0xa0);
	GC0329S_write_cmos_sensor(0x63, 0x40);
	GC0329S_write_cmos_sensor(0x64, 0x50);
	GC0329S_write_cmos_sensor(0x65, 0x98);
	GC0329S_write_cmos_sensor(0x66, 0xfa);
	GC0329S_write_cmos_sensor(0x67, 0x70);
	GC0329S_write_cmos_sensor(0x68, 0x58);
	GC0329S_write_cmos_sensor(0x69, 0x85);
	GC0329S_write_cmos_sensor(0x6a, 0x40);
	GC0329S_write_cmos_sensor(0x6b, 0x39);
	GC0329S_write_cmos_sensor(0x6c, 0x18);
	GC0329S_write_cmos_sensor(0x6d, 0x28);
	GC0329S_write_cmos_sensor(0x6e, 0x41);
	GC0329S_write_cmos_sensor(0x70, 0x02);
	GC0329S_write_cmos_sensor(0x71, 0x00);
	GC0329S_write_cmos_sensor(0x72, 0x10);
	GC0329S_write_cmos_sensor(0x73, 0x40);

	//GC0329S_write_cmos_sensor(0x74, 0x32);
	//GC0329S_write_cmos_sensor(0x75, 0x40);
	//GC0329S_write_cmos_sensor(0x76, 0x30);
	//GC0329S_write_cmos_sensor(0x77, 0x48);
	//GC0329S_write_cmos_sensor(0x7a, 0x50);
	//GC0329S_write_cmos_sensor(0x7b, 0x20); 

	GC0329S_write_cmos_sensor(0x80, 0x60);
	GC0329S_write_cmos_sensor(0x81, 0x50);
	GC0329S_write_cmos_sensor(0x82, 0x42);
	GC0329S_write_cmos_sensor(0x83, 0x40);
	GC0329S_write_cmos_sensor(0x84, 0x40);
	GC0329S_write_cmos_sensor(0x85, 0x40);

	GC0329S_write_cmos_sensor(0x74, 0x40);
	GC0329S_write_cmos_sensor(0x75, 0x58);
	GC0329S_write_cmos_sensor(0x76, 0x24);
	GC0329S_write_cmos_sensor(0x77, 0x40);
	GC0329S_write_cmos_sensor(0x78, 0x20);
	GC0329S_write_cmos_sensor(0x79, 0x60);
	GC0329S_write_cmos_sensor(0x7a, 0x58);
	GC0329S_write_cmos_sensor(0x7b, 0x20);
	GC0329S_write_cmos_sensor(0x7c, 0x30);
	GC0329S_write_cmos_sensor(0x7d, 0x35);
	GC0329S_write_cmos_sensor(0x7e, 0x10);
	GC0329S_write_cmos_sensor(0x7f, 0x08);

	////////////////////ABS////////////////////
	GC0329S_write_cmos_sensor(0x9c, 0x02); 
	GC0329S_write_cmos_sensor(0x9d, 0x20);
	//GC0329S_write_cmos_sensor(0x9f, 0x40);	

	////////////////////CC-AWB////////////////////
	GC0329S_write_cmos_sensor(0xd0, 0x00);
	GC0329S_write_cmos_sensor(0xd2, 0x2c);
	GC0329S_write_cmos_sensor(0xd3, 0x80); 

	////////////////////LSC///////////////////
	GC0329S_SET_PAGE1;
	GC0329S_write_cmos_sensor(0xa0, 0x00);
	GC0329S_write_cmos_sensor(0xa1, 0x3c);
	GC0329S_write_cmos_sensor(0xa2, 0x50);
	GC0329S_write_cmos_sensor(0xa3, 0x00);
	GC0329S_write_cmos_sensor(0xa8, 0x0f);
	GC0329S_write_cmos_sensor(0xa9, 0x08);
	GC0329S_write_cmos_sensor(0xaa, 0x00);
	GC0329S_write_cmos_sensor(0xab, 0x04);
	GC0329S_write_cmos_sensor(0xac, 0x00);
	GC0329S_write_cmos_sensor(0xad, 0x07);
	GC0329S_write_cmos_sensor(0xae, 0x0e);
	GC0329S_write_cmos_sensor(0xaf, 0x00);
	GC0329S_write_cmos_sensor(0xb0, 0x00);
	GC0329S_write_cmos_sensor(0xb1, 0x09);
	GC0329S_write_cmos_sensor(0xb2, 0x00);
	GC0329S_write_cmos_sensor(0xb3, 0x00);
	GC0329S_write_cmos_sensor(0xb4, 0x31);
	GC0329S_write_cmos_sensor(0xb5, 0x19);
	GC0329S_write_cmos_sensor(0xb6, 0x24);
	GC0329S_write_cmos_sensor(0xba, 0x3a);
	GC0329S_write_cmos_sensor(0xbb, 0x24);
	GC0329S_write_cmos_sensor(0xbc, 0x2a);
	GC0329S_write_cmos_sensor(0xc0, 0x17);
	GC0329S_write_cmos_sensor(0xc1, 0x13);
	GC0329S_write_cmos_sensor(0xc2, 0x17);
	GC0329S_write_cmos_sensor(0xc6, 0x21);
	GC0329S_write_cmos_sensor(0xc7, 0x1c);
	GC0329S_write_cmos_sensor(0xc8, 0x1c);
	GC0329S_write_cmos_sensor(0xb7, 0x00);
	GC0329S_write_cmos_sensor(0xb8, 0x00);
	GC0329S_write_cmos_sensor(0xb9, 0x00);
	GC0329S_write_cmos_sensor(0xbd, 0x00);
	GC0329S_write_cmos_sensor(0xbe, 0x00);
	GC0329S_write_cmos_sensor(0xbf, 0x00);
	GC0329S_write_cmos_sensor(0xc3, 0x00);
	GC0329S_write_cmos_sensor(0xc4, 0x00);
	GC0329S_write_cmos_sensor(0xc5, 0x00);
	GC0329S_write_cmos_sensor(0xc9, 0x00);
	GC0329S_write_cmos_sensor(0xca, 0x00);
	GC0329S_write_cmos_sensor(0xcb, 0x00);
	GC0329S_write_cmos_sensor(0xa4, 0x00);
	GC0329S_write_cmos_sensor(0xa5, 0x00);
	GC0329S_write_cmos_sensor(0xa6, 0x00);
	GC0329S_write_cmos_sensor(0xa7, 0x00);
	GC0329S_SET_PAGE0;	

	////////////////////asde ///////////////////
	//GC0329S_write_cmos_sensor(0xa0, 0xaf);
	//GC0329S_write_cmos_sensor(0xa2, 0xff);
	mdelay(400);
	GC0329S_write_cmos_sensor(0xf0, 0x07);
	GC0329S_write_cmos_sensor(0x44, 0xa0);



}


/*************************************************************************
* FUNCTION
*	GC329_Lens_Select
*
* DESCRIPTION
*	This function is served for FAE to select the appropriate lens parameter.
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void GC0329S_Lens_Select(kal_uint8 Lens_Tag)
{
	switch(Lens_Tag)
	{
		case CHT_806C_2:
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0xa0, 0x00);
			GC0329S_write_cmos_sensor(0xa1, 0x3c);
			GC0329S_write_cmos_sensor(0xa2, 0x50);
			GC0329S_write_cmos_sensor(0xa3, 0x00);
			GC0329S_write_cmos_sensor(0xa4, 0x00);
			GC0329S_write_cmos_sensor(0xa5, 0x00);
			GC0329S_write_cmos_sensor(0xa6, 0x00);
			GC0329S_write_cmos_sensor(0xa7, 0x04);
			
			GC0329S_write_cmos_sensor(0xa8, 0x0f);
			GC0329S_write_cmos_sensor(0xa9, 0x08);
			GC0329S_write_cmos_sensor(0xaa, 0x00);
			GC0329S_write_cmos_sensor(0xab, 0x04);
			GC0329S_write_cmos_sensor(0xac, 0x00);
			GC0329S_write_cmos_sensor(0xad, 0x07);
			GC0329S_write_cmos_sensor(0xae, 0x0e);
			GC0329S_write_cmos_sensor(0xaf, 0x00);
			GC0329S_write_cmos_sensor(0xb0, 0x00);
			GC0329S_write_cmos_sensor(0xb1, 0x09);
			GC0329S_write_cmos_sensor(0xb2, 0x00);
			GC0329S_write_cmos_sensor(0xb3, 0x00);

			GC0329S_write_cmos_sensor(0xb4, 0x30);
			GC0329S_write_cmos_sensor(0xb5, 0x19);
			GC0329S_write_cmos_sensor(0xb6, 0x21);
			GC0329S_write_cmos_sensor(0xba, 0x3e);
			GC0329S_write_cmos_sensor(0xbb, 0x26);
			GC0329S_write_cmos_sensor(0xbc, 0x2f);
			GC0329S_write_cmos_sensor(0xc0, 0x15);
			GC0329S_write_cmos_sensor(0xc1, 0x11);
			GC0329S_write_cmos_sensor(0xc2, 0x15);
			GC0329S_write_cmos_sensor(0xc6, 0x1f);
			GC0329S_write_cmos_sensor(0xc7, 0x16);
			GC0329S_write_cmos_sensor(0xc8, 0x16);

			GC0329S_write_cmos_sensor(0xb7, 0x00);
			GC0329S_write_cmos_sensor(0xb8, 0x00);
			GC0329S_write_cmos_sensor(0xb9, 0x00);
			GC0329S_write_cmos_sensor(0xbd, 0x00);
			GC0329S_write_cmos_sensor(0xbe, 0x00);
			GC0329S_write_cmos_sensor(0xbf, 0x00);
			GC0329S_write_cmos_sensor(0xc3, 0x00);
			GC0329S_write_cmos_sensor(0xc4, 0x00);
			GC0329S_write_cmos_sensor(0xc5, 0x00);
			GC0329S_write_cmos_sensor(0xc9, 0x0d);
			GC0329S_write_cmos_sensor(0xca, 0x00);
			GC0329S_write_cmos_sensor(0xcb, 0x00);
			
			GC0329S_write_cmos_sensor(0xfe, 0x00);
			break;

		case CHT_808C_2:
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0xa0, 0x00);
			GC0329S_write_cmos_sensor(0xa1, 0x3c);
			GC0329S_write_cmos_sensor(0xa2, 0x50);
			GC0329S_write_cmos_sensor(0xa3, 0x00);
			GC0329S_write_cmos_sensor(0xa4, 0x00);
			GC0329S_write_cmos_sensor(0xa5, 0x02);
			GC0329S_write_cmos_sensor(0xa6, 0x00);
			GC0329S_write_cmos_sensor(0xa7, 0x00);

			GC0329S_write_cmos_sensor(0xa8, 0x0c);
			GC0329S_write_cmos_sensor(0xa9, 0x03);
			GC0329S_write_cmos_sensor(0xaa, 0x00);
			GC0329S_write_cmos_sensor(0xab, 0x05);
			GC0329S_write_cmos_sensor(0xac, 0x01);
			GC0329S_write_cmos_sensor(0xad, 0x07);
			GC0329S_write_cmos_sensor(0xae, 0x0e);
			GC0329S_write_cmos_sensor(0xaf, 0x00);
			GC0329S_write_cmos_sensor(0xb0, 0x00);
			GC0329S_write_cmos_sensor(0xb1, 0x08);
			GC0329S_write_cmos_sensor(0xb2, 0x02);
			GC0329S_write_cmos_sensor(0xb3, 0x00);

			GC0329S_write_cmos_sensor(0xb4, 0x30);
			GC0329S_write_cmos_sensor(0xb5, 0x0f);
			GC0329S_write_cmos_sensor(0xb6, 0x16);
			GC0329S_write_cmos_sensor(0xba, 0x44);
			GC0329S_write_cmos_sensor(0xbb, 0x24);
			GC0329S_write_cmos_sensor(0xbc, 0x2a);
			GC0329S_write_cmos_sensor(0xc0, 0x13);
			GC0329S_write_cmos_sensor(0xc1, 0x0e);
			GC0329S_write_cmos_sensor(0xc2, 0x11);
			GC0329S_write_cmos_sensor(0xc6, 0x28);
			GC0329S_write_cmos_sensor(0xc7, 0x21);
			GC0329S_write_cmos_sensor(0xc8, 0x20);

			GC0329S_write_cmos_sensor(0xb7, 0x00);
			GC0329S_write_cmos_sensor(0xb8, 0x00);
			GC0329S_write_cmos_sensor(0xb9, 0x01);
			GC0329S_write_cmos_sensor(0xbd, 0x00);
			GC0329S_write_cmos_sensor(0xbe, 0x00);
			GC0329S_write_cmos_sensor(0xbf, 0x00);
			GC0329S_write_cmos_sensor(0xc3, 0x00);
			GC0329S_write_cmos_sensor(0xc4, 0x00);
			GC0329S_write_cmos_sensor(0xc5, 0x00);
			GC0329S_write_cmos_sensor(0xc9, 0x00);
			GC0329S_write_cmos_sensor(0xca, 0x00);
			GC0329S_write_cmos_sensor(0xcb, 0x00);

			GC0329S_write_cmos_sensor(0xfe, 0x00);
			break;
			
		case LY_982A_H114:
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0xa0, 0x00);
			GC0329S_write_cmos_sensor(0xa1, 0x3c);
			GC0329S_write_cmos_sensor(0xa2, 0x50);
			GC0329S_write_cmos_sensor(0xa3, 0x00);
			GC0329S_write_cmos_sensor(0xa4, 0x00);
			GC0329S_write_cmos_sensor(0xa5, 0x00);
			GC0329S_write_cmos_sensor(0xa6, 0x00);
			GC0329S_write_cmos_sensor(0xa7, 0x00);

			GC0329S_write_cmos_sensor(0xa8, 0x0c);
			GC0329S_write_cmos_sensor(0xa9, 0x06);
			GC0329S_write_cmos_sensor(0xaa, 0x02);
			GC0329S_write_cmos_sensor(0xab, 0x13);
			GC0329S_write_cmos_sensor(0xac, 0x06);
			GC0329S_write_cmos_sensor(0xad, 0x05);
			GC0329S_write_cmos_sensor(0xae, 0x0b);
			GC0329S_write_cmos_sensor(0xaf, 0x03);
			GC0329S_write_cmos_sensor(0xb0, 0x00);
			GC0329S_write_cmos_sensor(0xb1, 0x08);
			GC0329S_write_cmos_sensor(0xb2, 0x01);
			GC0329S_write_cmos_sensor(0xb3, 0x00);

			GC0329S_write_cmos_sensor(0xb4, 0x34);
			GC0329S_write_cmos_sensor(0xb5, 0x29);
			GC0329S_write_cmos_sensor(0xb6, 0x2e);
			GC0329S_write_cmos_sensor(0xba, 0x30);
			GC0329S_write_cmos_sensor(0xbb, 0x24);
			GC0329S_write_cmos_sensor(0xbc, 0x28);
			GC0329S_write_cmos_sensor(0xc0, 0x1c);
			GC0329S_write_cmos_sensor(0xc1, 0x19);
			GC0329S_write_cmos_sensor(0xc2, 0x19);
			GC0329S_write_cmos_sensor(0xc6, 0x1a);
			GC0329S_write_cmos_sensor(0xc7, 0x19);
			GC0329S_write_cmos_sensor(0xc8, 0x1b);

			GC0329S_write_cmos_sensor(0xb7, 0x01);
			GC0329S_write_cmos_sensor(0xb8, 0x01);
			GC0329S_write_cmos_sensor(0xb9, 0x00);
			GC0329S_write_cmos_sensor(0xbd, 0x00);
			GC0329S_write_cmos_sensor(0xbe, 0x00);
			GC0329S_write_cmos_sensor(0xbf, 0x00);
			GC0329S_write_cmos_sensor(0xc3, 0x00);
			GC0329S_write_cmos_sensor(0xc4, 0x00);
			GC0329S_write_cmos_sensor(0xc5, 0x03);
			GC0329S_write_cmos_sensor(0xc9, 0x00);
			GC0329S_write_cmos_sensor(0xca, 0x00);
			GC0329S_write_cmos_sensor(0xcb, 0x00);

			GC0329S_write_cmos_sensor(0xfe, 0x00);
			break;

		case XY_046A:
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0xa0, 0x00);
			GC0329S_write_cmos_sensor(0xa1, 0x3c);
			GC0329S_write_cmos_sensor(0xa2, 0x50);
			GC0329S_write_cmos_sensor(0xa3, 0x00);
			GC0329S_write_cmos_sensor(0xa4, 0x00);
			GC0329S_write_cmos_sensor(0xa5, 0x00);
			GC0329S_write_cmos_sensor(0xa6, 0x10);
			GC0329S_write_cmos_sensor(0xa7, 0x00);

			GC0329S_write_cmos_sensor(0xa8, 0x11);
			GC0329S_write_cmos_sensor(0xa9, 0x0a);
			GC0329S_write_cmos_sensor(0xaa, 0x05);
			GC0329S_write_cmos_sensor(0xab, 0x04);
			GC0329S_write_cmos_sensor(0xac, 0x03);
			GC0329S_write_cmos_sensor(0xad, 0x00);
			GC0329S_write_cmos_sensor(0xae, 0x08);
			GC0329S_write_cmos_sensor(0xaf, 0x01);
			GC0329S_write_cmos_sensor(0xb0, 0x00);
			GC0329S_write_cmos_sensor(0xb1, 0x09);
			GC0329S_write_cmos_sensor(0xb2, 0x02);
			GC0329S_write_cmos_sensor(0xb3, 0x03);

			GC0329S_write_cmos_sensor(0xb4, 0x2e);
			GC0329S_write_cmos_sensor(0xb5, 0x16);
			GC0329S_write_cmos_sensor(0xb6, 0x24);
			GC0329S_write_cmos_sensor(0xba, 0x3a);
			GC0329S_write_cmos_sensor(0xbb, 0x1e);
			GC0329S_write_cmos_sensor(0xbc, 0x24);
			GC0329S_write_cmos_sensor(0xc0, 0x09);
			GC0329S_write_cmos_sensor(0xc1, 0x02);
			GC0329S_write_cmos_sensor(0xc2, 0x06);
			GC0329S_write_cmos_sensor(0xc6, 0x25);
			GC0329S_write_cmos_sensor(0xc7, 0x21);
			GC0329S_write_cmos_sensor(0xc8, 0x23);

			GC0329S_write_cmos_sensor(0xb7, 0x00);
			GC0329S_write_cmos_sensor(0xb8, 0x00);
			GC0329S_write_cmos_sensor(0xb9, 0x0f);
			GC0329S_write_cmos_sensor(0xbd, 0x00);
			GC0329S_write_cmos_sensor(0xbe, 0x00);
			GC0329S_write_cmos_sensor(0xbf, 0x00);
			GC0329S_write_cmos_sensor(0xc3, 0x00);
			GC0329S_write_cmos_sensor(0xc4, 0x00);
			GC0329S_write_cmos_sensor(0xc5, 0x00);
			GC0329S_write_cmos_sensor(0xc9, 0x00);
			GC0329S_write_cmos_sensor(0xca, 0x00);
			GC0329S_write_cmos_sensor(0xcb, 0x00);

			GC0329S_write_cmos_sensor(0xfe, 0x00);
			break;

		case XY_0620:
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0xa0, 0x00);
			GC0329S_write_cmos_sensor(0xa1, 0x3c);
			GC0329S_write_cmos_sensor(0xa2, 0x50);
			GC0329S_write_cmos_sensor(0xa3, 0x00);
			GC0329S_write_cmos_sensor(0xa4, 0x00);
			GC0329S_write_cmos_sensor(0xa5, 0x00);
			GC0329S_write_cmos_sensor(0xa6, 0x00);
			GC0329S_write_cmos_sensor(0xa7, 0x00);

			GC0329S_write_cmos_sensor(0xa8, 0x0f);
			GC0329S_write_cmos_sensor(0xa9, 0x06);
			GC0329S_write_cmos_sensor(0xaa, 0x00);
			GC0329S_write_cmos_sensor(0xab, 0x07);
			GC0329S_write_cmos_sensor(0xac, 0x05);
			GC0329S_write_cmos_sensor(0xad, 0x08);
			GC0329S_write_cmos_sensor(0xae, 0x13);
			GC0329S_write_cmos_sensor(0xaf, 0x06);
			GC0329S_write_cmos_sensor(0xb0, 0x00);
			GC0329S_write_cmos_sensor(0xb1, 0x06);
			GC0329S_write_cmos_sensor(0xb2, 0x01);
			GC0329S_write_cmos_sensor(0xb3, 0x04);

			GC0329S_write_cmos_sensor(0xb4, 0x2d);
			GC0329S_write_cmos_sensor(0xb5, 0x18);
			GC0329S_write_cmos_sensor(0xb6, 0x22);
			GC0329S_write_cmos_sensor(0xba, 0x45);
			GC0329S_write_cmos_sensor(0xbb, 0x2d);
			GC0329S_write_cmos_sensor(0xbc, 0x34);
			GC0329S_write_cmos_sensor(0xc0, 0x16);
			GC0329S_write_cmos_sensor(0xc1, 0x13);
			GC0329S_write_cmos_sensor(0xc2, 0x19);
			GC0329S_write_cmos_sensor(0xc6, 0x21);
			GC0329S_write_cmos_sensor(0xc7, 0x1c);
			GC0329S_write_cmos_sensor(0xc8, 0x18);

			GC0329S_write_cmos_sensor(0xb7, 0x00);
			GC0329S_write_cmos_sensor(0xb8, 0x00);
			GC0329S_write_cmos_sensor(0xb9, 0x00);
			GC0329S_write_cmos_sensor(0xbd, 0x00);
			GC0329S_write_cmos_sensor(0xbe, 0x00);
			GC0329S_write_cmos_sensor(0xbf, 0x08);
			GC0329S_write_cmos_sensor(0xc3, 0x00);
			GC0329S_write_cmos_sensor(0xc4, 0x00);
			GC0329S_write_cmos_sensor(0xc5, 0x01);
			GC0329S_write_cmos_sensor(0xc9, 0x00);
			GC0329S_write_cmos_sensor(0xca, 0x00);
			GC0329S_write_cmos_sensor(0xcb, 0x10);

			GC0329S_write_cmos_sensor(0xfe, 0x00);
			break;

		case XY_078V: 
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0xa0, 0x00);
			GC0329S_write_cmos_sensor(0xa1, 0x3c);
			GC0329S_write_cmos_sensor(0xa2, 0x50);
			GC0329S_write_cmos_sensor(0xa3, 0x00);
			GC0329S_write_cmos_sensor(0xa4, 0x00);
			GC0329S_write_cmos_sensor(0xa5, 0x00);
			GC0329S_write_cmos_sensor(0xa6, 0x00);
			GC0329S_write_cmos_sensor(0xa7, 0x00);

			GC0329S_write_cmos_sensor(0xa8, 0x14);
			GC0329S_write_cmos_sensor(0xa9, 0x08);
			GC0329S_write_cmos_sensor(0xaa, 0x0a);
			GC0329S_write_cmos_sensor(0xab, 0x11);
			GC0329S_write_cmos_sensor(0xac, 0x05);
			GC0329S_write_cmos_sensor(0xad, 0x07);
			GC0329S_write_cmos_sensor(0xae, 0x0b);
			GC0329S_write_cmos_sensor(0xaf, 0x03);
			GC0329S_write_cmos_sensor(0xb0, 0x00);
			GC0329S_write_cmos_sensor(0xb1, 0x09);
			GC0329S_write_cmos_sensor(0xb2, 0x04);
			GC0329S_write_cmos_sensor(0xb3, 0x01);

			GC0329S_write_cmos_sensor(0xb4, 0x2f);
			GC0329S_write_cmos_sensor(0xb5, 0x2a);
			GC0329S_write_cmos_sensor(0xb6, 0x2c);
			GC0329S_write_cmos_sensor(0xba, 0x3a);
			GC0329S_write_cmos_sensor(0xbb, 0x2b);
			GC0329S_write_cmos_sensor(0xbc, 0x32);
			GC0329S_write_cmos_sensor(0xc0, 0x1b);
			GC0329S_write_cmos_sensor(0xc1, 0x18);
			GC0329S_write_cmos_sensor(0xc2, 0x1a);
			GC0329S_write_cmos_sensor(0xc6, 0x12);
			GC0329S_write_cmos_sensor(0xc7, 0x10);
			GC0329S_write_cmos_sensor(0xc8, 0x12);

			GC0329S_write_cmos_sensor(0xb7, 0x0a);
			GC0329S_write_cmos_sensor(0xb8, 0x00);
			GC0329S_write_cmos_sensor(0xb9, 0x00);
			GC0329S_write_cmos_sensor(0xbd, 0x00);
			GC0329S_write_cmos_sensor(0xbe, 0x00);
			GC0329S_write_cmos_sensor(0xbf, 0x00);
			GC0329S_write_cmos_sensor(0xc3, 0x00);
			GC0329S_write_cmos_sensor(0xc4, 0x00);
			GC0329S_write_cmos_sensor(0xc5, 0x00);
			GC0329S_write_cmos_sensor(0xc9, 0x0d);
			GC0329S_write_cmos_sensor(0xca, 0x00);
			GC0329S_write_cmos_sensor(0xcb, 0x00);

			GC0329S_write_cmos_sensor(0xfe, 0x00);
			break;

		case YG1001A_F:
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0xa0, 0x00);
			GC0329S_write_cmos_sensor(0xa1, 0x3c);
			GC0329S_write_cmos_sensor(0xa2, 0x50);
			GC0329S_write_cmos_sensor(0xa3, 0x00);
			GC0329S_write_cmos_sensor(0xa4, 0x00);
			GC0329S_write_cmos_sensor(0xa5, 0x00);
			GC0329S_write_cmos_sensor(0xa6, 0x00);
			GC0329S_write_cmos_sensor(0xa7, 0x00);

			GC0329S_write_cmos_sensor(0xa8, 0x0e);
			GC0329S_write_cmos_sensor(0xa9, 0x05);
			GC0329S_write_cmos_sensor(0xaa, 0x01);
			GC0329S_write_cmos_sensor(0xab, 0x07);
			GC0329S_write_cmos_sensor(0xac, 0x00);
			GC0329S_write_cmos_sensor(0xad, 0x07);
			GC0329S_write_cmos_sensor(0xae, 0x0e);
			GC0329S_write_cmos_sensor(0xaf, 0x02);
			GC0329S_write_cmos_sensor(0xb0, 0x00);
			GC0329S_write_cmos_sensor(0xb1, 0x0d);
			GC0329S_write_cmos_sensor(0xb2, 0x00);
			GC0329S_write_cmos_sensor(0xb3, 0x00);

			GC0329S_write_cmos_sensor(0xb4, 0x2a);
			GC0329S_write_cmos_sensor(0xb5, 0x0f);
			GC0329S_write_cmos_sensor(0xb6, 0x14);
			GC0329S_write_cmos_sensor(0xba, 0x40);
			GC0329S_write_cmos_sensor(0xbb, 0x26);
			GC0329S_write_cmos_sensor(0xbc, 0x2a);
			GC0329S_write_cmos_sensor(0xc0, 0x0e);
			GC0329S_write_cmos_sensor(0xc1, 0x0a);
			GC0329S_write_cmos_sensor(0xc2, 0x0d);
			GC0329S_write_cmos_sensor(0xc6, 0x27);
			GC0329S_write_cmos_sensor(0xc7, 0x20);
			GC0329S_write_cmos_sensor(0xc8, 0x1f);

			GC0329S_write_cmos_sensor(0xb7, 0x00);
			GC0329S_write_cmos_sensor(0xb8, 0x00);
			GC0329S_write_cmos_sensor(0xb9, 0x00);
			GC0329S_write_cmos_sensor(0xbd, 0x00);
			GC0329S_write_cmos_sensor(0xbe, 0x00);
			GC0329S_write_cmos_sensor(0xbf, 0x00);
			GC0329S_write_cmos_sensor(0xc3, 0x00);
			GC0329S_write_cmos_sensor(0xc4, 0x00);
			GC0329S_write_cmos_sensor(0xc5, 0x00);
			GC0329S_write_cmos_sensor(0xc9, 0x00);
			GC0329S_write_cmos_sensor(0xca, 0x00);
			GC0329S_write_cmos_sensor(0xcb, 0x00);

			GC0329S_write_cmos_sensor(0xfe, 0x00);
			break;

		default:
			break;
	}
}


/*************************************************************************
* FUNCTION
*	GC0329S_GAMMA_Select
*
* DESCRIPTION
*	This function is served for FAE to select the appropriate GAMMA curve.
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/

UINT32 GC0329SGetSensorID(UINT32 *sensorID)
{
    kal_uint16 sensor_id=0;
    int i;

    GC0329S_write_cmos_sensor(0xfc, 0x16);
    Sleep(20);

    do
    {
        	// check if sensor ID correct
        	for(i = 0; i < 3; i++)
		{
	            	sensor_id = GC0329S_read_cmos_sensor(0x00);
	            	printk("MYCAT GC0329S Sensor id = %x\n", sensor_id);
	            	if (sensor_id == GC0329_SENSOR_ID)
			{
	               	break;
	            	}
        	}
        	mdelay(50);
    }while(0);

    if(sensor_id != GC0329_SENSOR_ID)
    {
        printk("MYCAT GC0329S Sensor id read failed, ID = %x\n", sensor_id);
        *sensorID = 0xffffffff;
        return ERROR_SENSOR_CONNECT_FAIL;
    }

    *sensorID = GC0329S_SENSOR_ID;

    RETAILMSG(1, (TEXT("Sensor Read ID OK \r\n")));
	
    return ERROR_NONE;
}

void GC0329SGammaSelect(kal_uint32 GammaLvl)
{
	switch(GammaLvl)
	{
		case GC0329S_RGB_Gamma_m1:						//smallest gamma curve
			GC0329S_write_cmos_sensor(0xfe, 0x00);
			GC0329S_write_cmos_sensor(0xbf, 0x06);
			GC0329S_write_cmos_sensor(0xc0, 0x12);
			GC0329S_write_cmos_sensor(0xc1, 0x22);
			GC0329S_write_cmos_sensor(0xc2, 0x35);
			GC0329S_write_cmos_sensor(0xc3, 0x4b);
			GC0329S_write_cmos_sensor(0xc4, 0x5f);
			GC0329S_write_cmos_sensor(0xc5, 0x72);
			GC0329S_write_cmos_sensor(0xc6, 0x8d);
			GC0329S_write_cmos_sensor(0xc7, 0xa4);
			GC0329S_write_cmos_sensor(0xc8, 0xb8);
			GC0329S_write_cmos_sensor(0xc9, 0xc8);
			GC0329S_write_cmos_sensor(0xca, 0xd4);
			GC0329S_write_cmos_sensor(0xcb, 0xde);
			GC0329S_write_cmos_sensor(0xcc, 0xe6);
			GC0329S_write_cmos_sensor(0xcd, 0xf1);
			GC0329S_write_cmos_sensor(0xce, 0xf8);
			GC0329S_write_cmos_sensor(0xcf, 0xfd);
			break;
		case GC0329S_RGB_Gamma_m2:
			GC0329S_write_cmos_sensor(0xBF, 0x08);
			GC0329S_write_cmos_sensor(0xc0, 0x0F);
			GC0329S_write_cmos_sensor(0xc1, 0x21);
			GC0329S_write_cmos_sensor(0xc2, 0x32);
			GC0329S_write_cmos_sensor(0xc3, 0x43);
			GC0329S_write_cmos_sensor(0xc4, 0x50);
			GC0329S_write_cmos_sensor(0xc5, 0x5E);
			GC0329S_write_cmos_sensor(0xc6, 0x78);
			GC0329S_write_cmos_sensor(0xc7, 0x90);
			GC0329S_write_cmos_sensor(0xc8, 0xA6);
			GC0329S_write_cmos_sensor(0xc9, 0xB9);
			GC0329S_write_cmos_sensor(0xcA, 0xC9);
			GC0329S_write_cmos_sensor(0xcB, 0xD6);
			GC0329S_write_cmos_sensor(0xcC, 0xE0);
			GC0329S_write_cmos_sensor(0xcD, 0xEE);
			GC0329S_write_cmos_sensor(0xcE, 0xF8);
			GC0329S_write_cmos_sensor(0xcF, 0xFF);
			break;
			
		case GC0329S_RGB_Gamma_m3:			
			GC0329S_write_cmos_sensor(0xBF, 0x0B);
			GC0329S_write_cmos_sensor(0xc0, 0x16);
			GC0329S_write_cmos_sensor(0xc1, 0x29);
			GC0329S_write_cmos_sensor(0xc2, 0x3C);
			GC0329S_write_cmos_sensor(0xc3, 0x4F);
			GC0329S_write_cmos_sensor(0xc4, 0x5F);
			GC0329S_write_cmos_sensor(0xc5, 0x6F);
			GC0329S_write_cmos_sensor(0xc6, 0x8A);
			GC0329S_write_cmos_sensor(0xc7, 0x9F);
			GC0329S_write_cmos_sensor(0xc8, 0xB4);
			GC0329S_write_cmos_sensor(0xc9, 0xC6);
			GC0329S_write_cmos_sensor(0xcA, 0xD3);
			GC0329S_write_cmos_sensor(0xcB, 0xDD);
			GC0329S_write_cmos_sensor(0xcC, 0xE5);
			GC0329S_write_cmos_sensor(0xcD, 0xF1);
			GC0329S_write_cmos_sensor(0xcE, 0xFA);
			GC0329S_write_cmos_sensor(0xcF, 0xFF);
			break;
			
		case GC0329S_RGB_Gamma_m4:
			GC0329S_write_cmos_sensor(0xBF, 0x0E);
			GC0329S_write_cmos_sensor(0xc0, 0x1C);
			GC0329S_write_cmos_sensor(0xc1, 0x34);
			GC0329S_write_cmos_sensor(0xc2, 0x48);
			GC0329S_write_cmos_sensor(0xc3, 0x5A);
			GC0329S_write_cmos_sensor(0xc4, 0x6B);
			GC0329S_write_cmos_sensor(0xc5, 0x7B);
			GC0329S_write_cmos_sensor(0xc6, 0x95);
			GC0329S_write_cmos_sensor(0xc7, 0xAB);
			GC0329S_write_cmos_sensor(0xc8, 0xBF);
			GC0329S_write_cmos_sensor(0xc9, 0xCE);
			GC0329S_write_cmos_sensor(0xcA, 0xD9);
			GC0329S_write_cmos_sensor(0xcB, 0xE4);
			GC0329S_write_cmos_sensor(0xcC, 0xEC);
			GC0329S_write_cmos_sensor(0xcD, 0xF7);
			GC0329S_write_cmos_sensor(0xcE, 0xFD);
			GC0329S_write_cmos_sensor(0xcF, 0xFF);
			break;
			
		case GC0329S_RGB_Gamma_m5:
			GC0329S_write_cmos_sensor(0xBF, 0x10);
			GC0329S_write_cmos_sensor(0xc0, 0x20);
			GC0329S_write_cmos_sensor(0xc1, 0x38);
			GC0329S_write_cmos_sensor(0xc2, 0x4E);
			GC0329S_write_cmos_sensor(0xc3, 0x63);
			GC0329S_write_cmos_sensor(0xc4, 0x76);
			GC0329S_write_cmos_sensor(0xc5, 0x87);
			GC0329S_write_cmos_sensor(0xc6, 0xA2);
			GC0329S_write_cmos_sensor(0xc7, 0xB8);
			GC0329S_write_cmos_sensor(0xc8, 0xCA);
			GC0329S_write_cmos_sensor(0xc9, 0xD8);
			GC0329S_write_cmos_sensor(0xcA, 0xE3);
			GC0329S_write_cmos_sensor(0xcB, 0xEB);
			GC0329S_write_cmos_sensor(0xcC, 0xF0);
			GC0329S_write_cmos_sensor(0xcD, 0xF8);
			GC0329S_write_cmos_sensor(0xcE, 0xFD);
			GC0329S_write_cmos_sensor(0xcF, 0xFF);
			break;
			
		case GC0329S_RGB_Gamma_m6:										// largest gamma curve
			GC0329S_write_cmos_sensor(0xBF, 0x14);
			GC0329S_write_cmos_sensor(0xc0, 0x28);
			GC0329S_write_cmos_sensor(0xc1, 0x44);
			GC0329S_write_cmos_sensor(0xc2, 0x5D);
			GC0329S_write_cmos_sensor(0xc3, 0x72);
			GC0329S_write_cmos_sensor(0xc4, 0x86);
			GC0329S_write_cmos_sensor(0xc5, 0x95);
			GC0329S_write_cmos_sensor(0xc6, 0xB1);
			GC0329S_write_cmos_sensor(0xc7, 0xC6);
			GC0329S_write_cmos_sensor(0xc8, 0xD5);
			GC0329S_write_cmos_sensor(0xc9, 0xE1);
			GC0329S_write_cmos_sensor(0xcA, 0xEA);
			GC0329S_write_cmos_sensor(0xcB, 0xF1);
			GC0329S_write_cmos_sensor(0xcC, 0xF5);
			GC0329S_write_cmos_sensor(0xcD, 0xFB);
			GC0329S_write_cmos_sensor(0xcE, 0xFE);
			GC0329S_write_cmos_sensor(0xcF, 0xFF);
			break;
		case GC0329S_RGB_Gamma_night:									//Gamma for night mode
			GC0329S_write_cmos_sensor(0xBF, 0x0B);
			GC0329S_write_cmos_sensor(0xc0, 0x16);
			GC0329S_write_cmos_sensor(0xc1, 0x29);
			GC0329S_write_cmos_sensor(0xc2, 0x3C);
			GC0329S_write_cmos_sensor(0xc3, 0x4F);
			GC0329S_write_cmos_sensor(0xc4, 0x5F);
			GC0329S_write_cmos_sensor(0xc5, 0x6F);
			GC0329S_write_cmos_sensor(0xc6, 0x8A);
			GC0329S_write_cmos_sensor(0xc7, 0x9F);
			GC0329S_write_cmos_sensor(0xc8, 0xB4);
			GC0329S_write_cmos_sensor(0xc9, 0xC6);
			GC0329S_write_cmos_sensor(0xcA, 0xD3);
			GC0329S_write_cmos_sensor(0xcB, 0xDD);
			GC0329S_write_cmos_sensor(0xcC, 0xE5);
			GC0329S_write_cmos_sensor(0xcD, 0xF1);
			GC0329S_write_cmos_sensor(0xcE, 0xFA);
			GC0329S_write_cmos_sensor(0xcF, 0xFF);
			break;
		default:
			//GC0329S_RGB_Gamma_m1
			GC0329S_write_cmos_sensor(0xfe, 0x00);
			GC0329S_write_cmos_sensor(0xbf, 0x06);
			GC0329S_write_cmos_sensor(0xc0, 0x12);
			GC0329S_write_cmos_sensor(0xc1, 0x22);
			GC0329S_write_cmos_sensor(0xc2, 0x35);
			GC0329S_write_cmos_sensor(0xc3, 0x4b);
			GC0329S_write_cmos_sensor(0xc4, 0x5f);
			GC0329S_write_cmos_sensor(0xc5, 0x72);
			GC0329S_write_cmos_sensor(0xc6, 0x8d);
			GC0329S_write_cmos_sensor(0xc7, 0xa4);
			GC0329S_write_cmos_sensor(0xc8, 0xb8);
			GC0329S_write_cmos_sensor(0xc9, 0xc8);
			GC0329S_write_cmos_sensor(0xca, 0xd4);
			GC0329S_write_cmos_sensor(0xcb, 0xde);
			GC0329S_write_cmos_sensor(0xcc, 0xe6);
			GC0329S_write_cmos_sensor(0xcd, 0xf1);
			GC0329S_write_cmos_sensor(0xce, 0xf8);
			GC0329S_write_cmos_sensor(0xcf, 0xfd);
			break;
	}
}



/*************************************************************************
* FUNCTION
*	GC0329S_Write_More_Registers
*
* DESCRIPTION
*	This function is served for FAE to modify the necessary Init Regs. Do not modify the regs
*     in init_GC0329S() directly.
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void GC0329S_Write_More_Registers(void)
{
	////////////20120427/////////////////////////
	GC0329S_write_cmos_sensor(0xfe,0x01); 																																
	GC0329S_write_cmos_sensor(0x18,0x22); 																																
	GC0329S_write_cmos_sensor(0x21,0xc0); 																																
	GC0329S_write_cmos_sensor(0x06,0x12);																																																																									
	GC0329S_write_cmos_sensor(0x08,0x9c);																																																																									
	GC0329S_write_cmos_sensor(0x51,0x28);																																																																									
	GC0329S_write_cmos_sensor(0x52,0x10);																																																																									
	GC0329S_write_cmos_sensor(0x53,0x20);																																																																									
	GC0329S_write_cmos_sensor(0x54,0x40);																																																																									
	GC0329S_write_cmos_sensor(0x55,0x16);																																																																									
	GC0329S_write_cmos_sensor(0x56,0x30);																																																																									
	GC0329S_write_cmos_sensor(0x58,0x60);																																																																									
	GC0329S_write_cmos_sensor(0x59,0x08);																																																																									
	GC0329S_write_cmos_sensor(0x5c,0x35);																																																																									
	GC0329S_write_cmos_sensor(0x5d,0x72);																																																																									
	GC0329S_write_cmos_sensor(0x67,0x80);																																																																									
	GC0329S_write_cmos_sensor(0x68,0x60);																																																																									
	GC0329S_write_cmos_sensor(0x69,0x90);																																																																									
	GC0329S_write_cmos_sensor(0x6c,0x30);																																																																									
	GC0329S_write_cmos_sensor(0x6d,0x60);																																																																									
	GC0329S_write_cmos_sensor(0x70,0x10);	
																																																																									
	GC0329S_write_cmos_sensor(0xfe,0x00);																																																																									
	GC0329S_write_cmos_sensor(0x9c,0x0a);																																																																									
	GC0329S_write_cmos_sensor(0xa0,0xaf);																																																																									
	GC0329S_write_cmos_sensor(0xa2,0xff);																																																																									
	GC0329S_write_cmos_sensor(0xa4,0x60);																																																																									
	GC0329S_write_cmos_sensor(0xa5,0x31);																																																																									
	GC0329S_write_cmos_sensor(0xa7,0x35);																																																																									
	GC0329S_write_cmos_sensor(0x42,0xfe);																																																																									
	GC0329S_write_cmos_sensor(0xd1,0x30);																																																																									
	GC0329S_write_cmos_sensor(0xd2,0x30);																																																																									
	GC0329S_write_cmos_sensor(0xfe,0x00);	
	///////////////////////////////////////////////
    	GC0329SGammaSelect(0);//0:use default
    	GC0329S_Lens_Select(0);//0:use default

#if 1
///////////////2013-06-26///////////////////////
////analog//
	GC0329S_SET_PAGE0;
	GC0329S_write_cmos_sensor(0x1e, 0x00);  //08
	GC0329S_write_cmos_sensor(0x20, 0x08); //00
////blk///
	GC0329S_write_cmos_sensor(0x29, 0x00); //80
	GC0329S_write_cmos_sensor(0x33, 0x1c);   ///  20
	GC0329S_write_cmos_sensor(0x34, 0x1c);
	GC0329S_write_cmos_sensor(0x35, 0x1c);
	GC0329S_write_cmos_sensor(0x36, 0x1c);
///////dndd//// 
	GC0329S_write_cmos_sensor(0x82, 0x10);   ///0e//  55
	GC0329S_write_cmos_sensor(0x83, 0x00);  // add by lanking 20120423
////intpee//
  GC0329S_write_cmos_sensor(0x95, 0x43);  //   54 /74
  GC0329S_write_cmos_sensor(0x96, 0x75);  //   88
///aec//////
	GC0329S_SET_PAGE1;
	GC0329S_write_cmos_sensor(0x12, 0x17);  //17// 07 
	GC0329S_write_cmos_sensor(0x15, 0xf2);  /// f2
	GC0329S_write_cmos_sensor(0x21, 0xe8);  
	GC0329S_write_cmos_sensor(0x22, 0x4a);   /// 48
	GC0329S_write_cmos_sensor(0x3c, 0xe0);  /// 95
	GC0329S_write_cmos_sensor(0x3d, 0x60);  /// 50
	GC0329S_write_cmos_sensor(0x3e, 0x60);  /// 48
	////////////////////ABS////////////////////
	GC0329S_SET_PAGE1;
	GC0329S_write_cmos_sensor(0x9c, 0x02);   ////  02 /01
	GC0329S_write_cmos_sensor(0x9d, 0x10);
//ɫ�ʣ���ɫ
///////cc/////
	GC0329S_SET_PAGE0;
	GC0329S_write_cmos_sensor(0xb3, 0x40);  ///  44
	GC0329S_write_cmos_sensor(0xb4, 0xfe);  // fd
	GC0329S_write_cmos_sensor(0xb5, 0x00);   /// 02
	GC0329S_write_cmos_sensor(0xb6, 0xfa);
	GC0329S_write_cmos_sensor(0xb7, 0x48);  /// 48
	GC0329S_write_cmos_sensor(0xb8, 0x0e);  ///  10
//////awb//////
	GC0329S_SET_PAGE1;
	GC0329S_write_cmos_sensor(0x06, 0x08);  ////  12
	GC0329S_write_cmos_sensor(0x07, 0x06);  // 06
	GC0329S_write_cmos_sensor(0x08, 0xa6);  // 9c
	GC0329S_write_cmos_sensor(0x09, 0xf4);  // ee
	GC0329S_write_cmos_sensor(0x50, 0xf3);  // fc
	GC0329S_write_cmos_sensor(0x51, 0x0e);  // 28   10/////////
	GC0329S_write_cmos_sensor(0x52, 0x10);  /// 0b   10
	GC0329S_write_cmos_sensor(0x53, 0x3c);  /// 38/////////
	GC0329S_write_cmos_sensor(0x54, 0x4a);  /// 48//////////////
	GC0329S_write_cmos_sensor(0x55, 0x2c);  /// 10  28
	GC0329S_write_cmos_sensor(0x56, 0x20);  /// 20
	//GC0329S_write_cmos_sensor(0x57, 0x40);
	GC0329S_write_cmos_sensor(0x58, 0x40);  // a0
	GC0329S_write_cmos_sensor(0x59, 0x08);  // 28
	GC0329S_write_cmos_sensor(0x5a, 0x02);
	GC0329S_write_cmos_sensor(0x5b, 0x63);
	GC0329S_write_cmos_sensor(0x5c, 0x31);  /// 35
	GC0329S_write_cmos_sensor(0x5d, 0x72);  // 73
	GC0329S_write_cmos_sensor(0x5e, 0x11);  ////  11
	GC0329S_write_cmos_sensor(0x5f, 0x40);
	GC0329S_write_cmos_sensor(0x60, 0x40);
	GC0329S_write_cmos_sensor(0x61, 0xc8);
	GC0329S_write_cmos_sensor(0x62, 0xa0);
	GC0329S_write_cmos_sensor(0x63, 0x40);
	GC0329S_write_cmos_sensor(0x64, 0x50); //// 50
	GC0329S_write_cmos_sensor(0x65, 0x98);   // 98
	GC0329S_write_cmos_sensor(0x66, 0xfa);  ///  fa
	GC0329S_write_cmos_sensor(0x67, 0x80);  //// 70
	GC0329S_write_cmos_sensor(0x68, 0x60);  ///// 58
	GC0329S_write_cmos_sensor(0x69, 0x90);  // 85
	GC0329S_write_cmos_sensor(0x6a, 0x4e);
	GC0329S_write_cmos_sensor(0x6b, 0x3a);
	GC0329S_write_cmos_sensor(0x6c, 0x1a); // 18
	GC0329S_write_cmos_sensor(0x6d, 0x60); // 28
	GC0329S_write_cmos_sensor(0x6e, 0x41);
	GC0329S_write_cmos_sensor(0x70, 0x10);  // 02
///////////////2013-06-26///////////////////////
#endif

}


/*************************************************************************
 * FUNCTION
 *	GC0329SOpen
 *
 * DESCRIPTION
 *	This function initialize the registers of CMOS sensor
 *
 * PARAMETERS
 *	None
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
UINT32 GC0329SOpen(void)
{
    kal_uint16 sensor_id=0;
    int i;

    GC0329S_write_cmos_sensor(0xfc, 0x16);
    Sleep(20);

    do
    {
        	// check if sensor ID correct
        	for(i = 0; i < 3; i++)
		{
	            	sensor_id = GC0329S_read_cmos_sensor(0x00);
	            	printk("MYCAT GC0329S Sensor id = %x\n", sensor_id);
	            	if (sensor_id == GC0329S_SENSOR_ID)
			{
	               	break;
	            	}
        	}
        	mdelay(50);
    }while(0);

    if(sensor_id != GC0329S_SENSOR_ID)
    {
        printk("MYCAT GC0329S Sensor id read failed, ID = %x\n", sensor_id);
        return ERROR_SENSOR_CONNECT_FAIL;
    }

    RETAILMSG(1, (TEXT("Sensor Read ID OK \r\n")));
    // initail sequence write in
    GC0329S_Sensor_Init();
    GC0329S_Write_More_Registers();
	
    return ERROR_NONE;
} /* GC0329SOpen */


/*************************************************************************
 * FUNCTION
 *	GC0329SClose
 *
 * DESCRIPTION
 *	This function is to turn off sensor module power.
 *
 * PARAMETERS
 *	None
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
UINT32 GC0329SClose(void)
{
    return ERROR_NONE;
} /* GC0329SClose */


/*************************************************************************
 * FUNCTION
 * GC0329SPreview
 *
 * DESCRIPTION
 *	This function start the sensor preview.
 *
 * PARAMETERS
 *	*image_window : address pointer of pixel numbers in one period of HSYNC
 *  *sensor_config_data : address pointer of line numbers in one period of VSYNC
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
UINT32 GC0329SPreview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
        MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)

{
    kal_uint32 iTemp;
    kal_uint16 iStartX = 0, iStartY = 1;

    if(sensor_config_data->SensorOperationMode == MSDK_SENSOR_OPERATION_MODE_VIDEO)		// MPEG4 Encode Mode
    {
        RETAILMSG(1, (TEXT("Camera Video preview\r\n")));
        GC0329S_MPEG4_encode_mode = KAL_TRUE;
       
    }
    else
    {
        RETAILMSG(1, (TEXT("Camera preview\r\n")));
        GC0329S_MPEG4_encode_mode = KAL_FALSE;
    }

    image_window->GrabStartX= IMAGE_SENSOR_VGA_GRAB_PIXELS;
    image_window->GrabStartY= IMAGE_SENSOR_VGA_GRAB_LINES;
    image_window->ExposureWindowWidth = IMAGE_SENSOR_PV_WIDTH;
    image_window->ExposureWindowHeight =IMAGE_SENSOR_PV_HEIGHT;

    // copy sensor_config_data
    memcpy(&GC0329SSensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
    return ERROR_NONE;
} /* GC0329SPreview */


/*************************************************************************
 * FUNCTION
 *	GC0329SCapture
 *
 * DESCRIPTION
 *	This function setup the CMOS sensor in capture MY_OUTPUT mode
 *
 * PARAMETERS
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
UINT32 GC0329SCapture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
        MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)

{
    GC0329S_MODE_CAPTURE=KAL_TRUE;

    image_window->GrabStartX = IMAGE_SENSOR_VGA_GRAB_PIXELS;
    image_window->GrabStartY = IMAGE_SENSOR_VGA_GRAB_LINES;
    image_window->ExposureWindowWidth= IMAGE_SENSOR_FULL_WIDTH;
    image_window->ExposureWindowHeight = IMAGE_SENSOR_FULL_HEIGHT;

    // copy sensor_config_data
    memcpy(&GC0329SSensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
    return ERROR_NONE;
} /* GC0329S_Capture() */



UINT32 GC0329SGetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
    pSensorResolution->SensorFullWidth=IMAGE_SENSOR_FULL_WIDTH;
    pSensorResolution->SensorFullHeight=IMAGE_SENSOR_FULL_HEIGHT;
    pSensorResolution->SensorPreviewWidth=IMAGE_SENSOR_PV_WIDTH;
    pSensorResolution->SensorPreviewHeight=IMAGE_SENSOR_PV_HEIGHT;
	pSensorResolution->SensorVideoWidth  = IMAGE_SENSOR_PV_WIDTH;
	pSensorResolution->SensorVideoHeight = IMAGE_SENSOR_PV_HEIGHT;
	
    return ERROR_NONE;
} /* GC0329SGetResolution() */


UINT32 GC0329SGetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
        MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
        MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
    pSensorInfo->SensorPreviewResolutionX=IMAGE_SENSOR_PV_WIDTH;
    pSensorInfo->SensorPreviewResolutionY=IMAGE_SENSOR_PV_HEIGHT;
    pSensorInfo->SensorFullResolutionX=IMAGE_SENSOR_FULL_WIDTH;
    pSensorInfo->SensorFullResolutionY=IMAGE_SENSOR_FULL_WIDTH;

    pSensorInfo->SensorCameraPreviewFrameRate=30;
    pSensorInfo->SensorVideoFrameRate=30;
    pSensorInfo->SensorStillCaptureFrameRate=10;
    pSensorInfo->SensorWebCamCaptureFrameRate=15;
    pSensorInfo->SensorResetActiveHigh=FALSE;
    pSensorInfo->SensorResetDelayCount=1;
    pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_UYVY;//SENSOR_OUTPUT_FORMAT_YUYV;
    pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;
    pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
    pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
    pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
    pSensorInfo->SensorInterruptDelayLines = 1;
    pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_PARALLEL;
/*
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_100_MODE].MaxWidth=CAM_SIZE_5M_WIDTH;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_100_MODE].MaxHeight=CAM_SIZE_5M_HEIGHT;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_100_MODE].ISOSupported=TRUE;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_100_MODE].BinningEnable=FALSE;

    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_200_MODE].MaxWidth=CAM_SIZE_5M_WIDTH;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_200_MODE].MaxHeight=CAM_SIZE_5M_HEIGHT;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_200_MODE].ISOSupported=TRUE;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_200_MODE].BinningEnable=FALSE;

    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_400_MODE].MaxWidth=CAM_SIZE_5M_WIDTH;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_400_MODE].MaxHeight=CAM_SIZE_5M_HEIGHT;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_400_MODE].ISOSupported=TRUE;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_400_MODE].BinningEnable=FALSE;

    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_800_MODE].MaxWidth=CAM_SIZE_1M_WIDTH;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_800_MODE].MaxHeight=CAM_SIZE_1M_HEIGHT;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_800_MODE].ISOSupported=TRUE;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_800_MODE].BinningEnable=FALSE;

    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].MaxWidth=CAM_SIZE_1M_WIDTH;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].MaxHeight=CAM_SIZE_1M_HEIGHT;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].ISOSupported=TRUE;
    pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].BinningEnable=FALSE;
	*/
    pSensorInfo->CaptureDelayFrame = 1;
    pSensorInfo->PreviewDelayFrame = 4;
    pSensorInfo->VideoDelayFrame = 4;
    pSensorInfo->SensorMasterClockSwitch = 0;
    pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_2MA;

    switch (ScenarioId)
    {
    case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
    case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
    //case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
        pSensorInfo->SensorClockFreq=24;
        pSensorInfo->SensorClockDividCount=	3;
        pSensorInfo->SensorClockRisingCount= 0;
        pSensorInfo->SensorClockFallingCount= 2;
        pSensorInfo->SensorPixelClockCount= 3;
        pSensorInfo->SensorDataLatchCount= 2;
        pSensorInfo->SensorGrabStartX = IMAGE_SENSOR_VGA_GRAB_PIXELS;
        pSensorInfo->SensorGrabStartY = IMAGE_SENSOR_VGA_GRAB_LINES;

        break;
    case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
    //case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
        pSensorInfo->SensorClockFreq=24;
        pSensorInfo->SensorClockDividCount= 3;
        pSensorInfo->SensorClockRisingCount=0;
        pSensorInfo->SensorClockFallingCount=2;
        pSensorInfo->SensorPixelClockCount=3;
        pSensorInfo->SensorDataLatchCount=2;
        pSensorInfo->SensorGrabStartX = IMAGE_SENSOR_VGA_GRAB_PIXELS;
        pSensorInfo->SensorGrabStartY = IMAGE_SENSOR_VGA_GRAB_LINES;
        break;
    default:
        pSensorInfo->SensorClockFreq=24;
        pSensorInfo->SensorClockDividCount= 3;
        pSensorInfo->SensorClockRisingCount=0;
        pSensorInfo->SensorClockFallingCount=2;
        pSensorInfo->SensorPixelClockCount=3;
        pSensorInfo->SensorDataLatchCount=2;
        pSensorInfo->SensorGrabStartX = IMAGE_SENSOR_VGA_GRAB_PIXELS;
        pSensorInfo->SensorGrabStartY = IMAGE_SENSOR_VGA_GRAB_LINES;
        break;
    }
    GC0329SPixelClockDivider=pSensorInfo->SensorPixelClockCount;
    memcpy(pSensorConfigData, &GC0329SSensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
    return ERROR_NONE;
} /* GC0329SGetInfo() */


UINT32 GC0329SControl(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
        MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
    switch (ScenarioId)
    {
    case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
    case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
  //  case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
        GC0329SPreview(pImageWindow, pSensorConfigData);
        break;
    case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
   // case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
        GC0329SCapture(pImageWindow, pSensorConfigData);
        break;
    }


    return TRUE;
}	/* GC0329SControl() */

BOOL GC0329S_set_param_wb(UINT16 para)
{

	switch (para)
	{
		case AWB_MODE_OFF:

		break;
		
		case AWB_MODE_AUTO:
			GC0329S_write_cmos_sensor(0x77, 0x57);
			GC0329S_write_cmos_sensor(0x78, 0x4d);
			GC0329S_write_cmos_sensor(0x79, 0x45);
			GC0329S_awb_enable(KAL_TRUE);
		break;
		
		case AWB_MODE_CLOUDY_DAYLIGHT: //cloudy
			GC0329S_awb_enable(KAL_FALSE);
			GC0329S_write_cmos_sensor(0x77, 0x7c); //WB_manual_gain 
			GC0329S_write_cmos_sensor(0x78, 0x58);
			GC0329S_write_cmos_sensor(0x79, 0x40);
		break;
		
		case AWB_MODE_DAYLIGHT: //sunny
			GC0329S_awb_enable(KAL_FALSE);
			GC0329S_write_cmos_sensor(0x77, 0x74); 
			GC0329S_write_cmos_sensor(0x78, 0x52);
			GC0329S_write_cmos_sensor(0x79, 0x40);			
		break;
		
		case AWB_MODE_INCANDESCENT: //office
			GC0329S_awb_enable(KAL_FALSE);
			GC0329S_write_cmos_sensor(0x77, 0x48);
			GC0329S_write_cmos_sensor(0x78, 0x40);
			GC0329S_write_cmos_sensor(0x79, 0x5c);
		break;
		
		case AWB_MODE_TUNGSTEN: //home
			GC0329S_awb_enable(KAL_FALSE);
			GC0329S_write_cmos_sensor(0x77, 0x40);
			GC0329S_write_cmos_sensor(0x78, 0x54);
			GC0329S_write_cmos_sensor(0x79, 0x70);
		break;
		
		case AWB_MODE_FLUORESCENT:
			GC0329S_awb_enable(KAL_FALSE);
			GC0329S_write_cmos_sensor(0x77, 0x40);
			GC0329S_write_cmos_sensor(0x78, 0x42);
			GC0329S_write_cmos_sensor(0x79, 0x50);
		break;
		
		default:
		return FALSE;
	}

	return TRUE;
} /* GC0329S_set_param_wb */


BOOL GC0329S_set_param_effect(UINT16 para)
{
	kal_uint32  ret = KAL_TRUE;

	switch (para)
	{
		case MEFFECT_OFF:
			GC0329S_write_cmos_sensor(0x43 , 0x00);
		break;
		
		case MEFFECT_SEPIA:
			GC0329S_write_cmos_sensor(0x43 , 0x02);
			GC0329S_write_cmos_sensor(0xda , 0xd0);
			GC0329S_write_cmos_sensor(0xdb , 0x28);
		break;
		
		case MEFFECT_NEGATIVE:
			GC0329S_write_cmos_sensor(0x43 , 0x01);
		break;
		
		case MEFFECT_SEPIAGREEN:
			GC0329S_write_cmos_sensor(0x43 , 0x02);
			GC0329S_write_cmos_sensor(0xda , 0xc0);
			GC0329S_write_cmos_sensor(0xdb , 0xc0);
		break;
		
		case MEFFECT_SEPIABLUE:
			GC0329S_write_cmos_sensor(0x43 , 0x02);
			GC0329S_write_cmos_sensor(0xda , 0x50);
			GC0329S_write_cmos_sensor(0xdb , 0xe0);
		break;

		case MEFFECT_MONO:
			GC0329S_write_cmos_sensor(0x43 , 0x02);
			GC0329S_write_cmos_sensor(0xda , 0x00);
			GC0329S_write_cmos_sensor(0xdb , 0x00);
		break;
		default:
			ret = FALSE;
	}

	return ret;

} /* GC0329S_set_param_effect */


BOOL GC0329S_set_param_banding(UINT16 para)
{
	switch (para)
	{
		case AE_FLICKER_MODE_50HZ:
			GC0329S_write_cmos_sensor(0x05, 0x02); 	
			GC0329S_write_cmos_sensor(0x06, 0x2c); 
			GC0329S_write_cmos_sensor(0x07, 0x01);
			GC0329S_write_cmos_sensor(0x08, 0x18);
			
			GC0329S_SET_PAGE1;
			GC0329S_write_cmos_sensor(0x29, 0x00);   //anti-flicker step [11:8]
			GC0329S_write_cmos_sensor(0x2a, 0x60);   //anti-flicker step [7:0]
			
			GC0329S_write_cmos_sensor(0x2b, 0x03);   //exp level 0  14.28fps
			GC0329S_write_cmos_sensor(0x2c, 0x00); 
			GC0329S_write_cmos_sensor(0x2d, 0x04);   //exp level 1  12.50fps
			GC0329S_write_cmos_sensor(0x2e, 0x20); 
			GC0329S_write_cmos_sensor(0x2f, 0x06);   //exp level 2  10.00fps
			GC0329S_write_cmos_sensor(0x30, 0x60); 
			GC0329S_write_cmos_sensor(0x31, 0x07);   //exp level 3  7.14fps
			GC0329S_write_cmos_sensor(0x32, 0x80); 
			GC0329S_SET_PAGE0;
			break;

		case AE_FLICKER_MODE_60HZ:
			GC0329S_write_cmos_sensor(0x05, 0x02); 	
			GC0329S_write_cmos_sensor(0x06, 0x2c); 
			GC0329S_write_cmos_sensor(0x07, 0x01);
			GC0329S_write_cmos_sensor(0x08, 0x38);
			
			GC0329S_SET_PAGE1;
			GC0329S_write_cmos_sensor(0x29, 0x00);   //anti-flicker step [11:8]
			GC0329S_write_cmos_sensor(0x2a, 0x50);   //anti-flicker step [7:0]
			
			GC0329S_write_cmos_sensor(0x2b, 0x03);   //exp level 0  15.00fps
			GC0329S_write_cmos_sensor(0x2c, 0x20); 
			GC0329S_write_cmos_sensor(0x2d, 0x04);   //exp level 0  12.00fps
			GC0329S_write_cmos_sensor(0x2e, 0x10); 
			GC0329S_write_cmos_sensor(0x2f, 0x06);   //exp level 0  10.00fps
			GC0329S_write_cmos_sensor(0x30, 0x40); 
			GC0329S_write_cmos_sensor(0x31, 0x07);   //exp level 0  7.05fps
			GC0329S_write_cmos_sensor(0x32, 0x80); 
			GC0329S_SET_PAGE0;
		break;
		default:
		return FALSE;
	}

	return TRUE;
} /* GC0329S_set_param_banding */


BOOL GC0329S_set_param_exposure(UINT16 para)
{
	kal_uint8 value_luma, value_Y;
	value_luma = (GC0329S_NIGHT_MODE?0x2b:0xf8);//00 
	value_Y = (GC0329S_NIGHT_MODE?0x68:0x50);

	switch (para)
	{
		case AE_EV_COMP_n13:
			GC0329S_write_cmos_sensor(0xd5, value_luma - 0x48);
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0x13, value_Y - 0x28);
			GC0329S_write_cmos_sensor(0xfe, 0x00);
		break;
		
		case AE_EV_COMP_n10:
			GC0329S_write_cmos_sensor(0xd5, value_luma - 0x30);
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0x13, value_Y - 0x18);
			GC0329S_write_cmos_sensor(0xfe, 0x00);
		break;
		
		case AE_EV_COMP_n07:
			GC0329S_write_cmos_sensor(0xd5, value_luma - 0x20);
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0x13, value_Y - 0x10);
			GC0329S_write_cmos_sensor(0xfe, 0x00);
		break;
		
		case AE_EV_COMP_n03:
			GC0329S_write_cmos_sensor(0xd5, value_luma - 0x10);
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0x13, value_Y - 0x08);
			GC0329S_write_cmos_sensor(0xfe, 0x00);
		break;				
		
		case AE_EV_COMP_00:
			GC0329S_write_cmos_sensor(0xd5, value_luma);
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0x13, value_Y);
			GC0329S_write_cmos_sensor(0xfe, 0x00);
		break;

		case AE_EV_COMP_03:
			GC0329S_write_cmos_sensor(0xd5, value_luma + 0x10);
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0x13, value_Y + 0x10);
			GC0329S_write_cmos_sensor(0xfe, 0x00);
		break;
		
		case AE_EV_COMP_07:
			GC0329S_write_cmos_sensor(0xd5, value_luma + 0x20);
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0x13, value_Y + 0x20);
			GC0329S_write_cmos_sensor(0xfe, 0x00);
		break;
		
		case AE_EV_COMP_10:
			GC0329S_write_cmos_sensor(0xd5, value_luma + 0x30);
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0x13, value_Y + 0x30);
			GC0329S_write_cmos_sensor(0xfe, 0x00);
		break;
		
		case AE_EV_COMP_13:
			GC0329S_write_cmos_sensor(0xd5, value_luma + 0x48);
			GC0329S_write_cmos_sensor(0xfe, 0x01);
			GC0329S_write_cmos_sensor(0x13, value_Y + 0x48);
			GC0329S_write_cmos_sensor(0xfe, 0x00);
		break;
		default:
		return FALSE;
	}

	return TRUE;
} /* GC0329S_set_param_exposure */


UINT32 GC0329SYUVSensorSetting(FEATURE_ID iCmd, UINT16 iPara)
{
    switch (iCmd) {
    case FID_AWB_MODE:
        GC0329S_set_param_wb(iPara);
        break;
    case FID_COLOR_EFFECT:
        GC0329S_set_param_effect(iPara);
        break;
    case FID_AE_EV:
        GC0329S_set_param_exposure(iPara);
        break;
    case FID_AE_FLICKER:
        GC0329S_set_param_banding(iPara);
		break;
	case FID_SCENE_MODE:
		GC0329SNightMode(iPara);
        break;
    default:
        break;
    }
    return TRUE;
} /* GC0329SYUVSensorSetting */


UINT32 GC0329SFeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,
        UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
{
    UINT16 *pFeatureReturnPara16=(UINT16 *) pFeaturePara;
    UINT16 *pFeatureData16=(UINT16 *) pFeaturePara;
    UINT32 *pFeatureReturnPara32=(UINT32 *) pFeaturePara;
    UINT32 *pFeatureData32=(UINT32 *) pFeaturePara;
    UINT32 GC0329SSensorRegNumber;
    UINT32 i;
    MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData=(MSDK_SENSOR_CONFIG_STRUCT *) pFeaturePara;
    MSDK_SENSOR_REG_INFO_STRUCT *pSensorRegData=(MSDK_SENSOR_REG_INFO_STRUCT *) pFeaturePara;

    RETAILMSG(1, (_T("gaiyang GC0329SFeatureControl FeatureId=%d\r\n"), FeatureId));

    switch (FeatureId)
    {
    case SENSOR_FEATURE_GET_RESOLUTION:
        *pFeatureReturnPara16++=IMAGE_SENSOR_FULL_WIDTH;
        *pFeatureReturnPara16=IMAGE_SENSOR_FULL_HEIGHT;
        *pFeatureParaLen=4;
        break;
    case SENSOR_FEATURE_GET_PERIOD:
        *pFeatureReturnPara16++=(VGA_PERIOD_PIXEL_NUMS)+GC0329S_dummy_pixels;
        *pFeatureReturnPara16=(VGA_PERIOD_LINE_NUMS)+GC0329S_dummy_lines;
        *pFeatureParaLen=4;
        break;
    case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
        *pFeatureReturnPara32 = GC0329S_g_fPV_PCLK;
        *pFeatureParaLen=4;
        break;
    case SENSOR_FEATURE_SET_ESHUTTER:
        break;
    case SENSOR_FEATURE_SET_NIGHTMODE:
        //GC0329SNightMode((BOOL) *pFeatureData16);
        break;
    case SENSOR_FEATURE_SET_GAIN:
    case SENSOR_FEATURE_SET_FLASHLIGHT:
        break;
    case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
        GC0329S_isp_master_clock=*pFeatureData32;
        break;
    case SENSOR_FEATURE_SET_REGISTER:
        GC0329S_write_cmos_sensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
        break;
    case SENSOR_FEATURE_GET_REGISTER:
        pSensorRegData->RegData = GC0329S_read_cmos_sensor(pSensorRegData->RegAddr);
        break;
    case SENSOR_FEATURE_GET_CONFIG_PARA:
        memcpy(pSensorConfigData, &GC0329SSensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
        *pFeatureParaLen=sizeof(MSDK_SENSOR_CONFIG_STRUCT);
        break;
    case SENSOR_FEATURE_SET_CCT_REGISTER:
    case SENSOR_FEATURE_GET_CCT_REGISTER:
    case SENSOR_FEATURE_SET_ENG_REGISTER:
    case SENSOR_FEATURE_GET_ENG_REGISTER:
    case SENSOR_FEATURE_GET_REGISTER_DEFAULT:
    case SENSOR_FEATURE_CAMERA_PARA_TO_SENSOR:
    case SENSOR_FEATURE_SENSOR_TO_CAMERA_PARA:
    case SENSOR_FEATURE_GET_GROUP_COUNT:
    case SENSOR_FEATURE_GET_GROUP_INFO:
    case SENSOR_FEATURE_GET_ITEM_INFO:
    case SENSOR_FEATURE_SET_ITEM_INFO:
    case SENSOR_FEATURE_GET_ENG_INFO:
        break;
    case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
        // get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
        // if EEPROM does not exist in camera module.
        *pFeatureReturnPara32=LENS_DRIVER_ID_DO_NOT_CARE;
        *pFeatureParaLen=4;
        break;
    case SENSOR_FEATURE_SET_YUV_CMD:
        GC0329SYUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
        break;
    case SENSOR_FEATURE_CHECK_SENSOR_ID:
				GC0329SGetSensorID(pFeatureData32);
				break;
		case SENSOR_FEATURE_GET_AF_MAX_NUM_FOCUS_AREAS:
      	*pFeatureData32=0;         
       	*pFeatureParaLen=4;
       	break;     
    case SENSOR_FEATURE_GET_AE_MAX_NUM_METERING_AREAS:
				*pFeatureData32=0;            
      	*pFeatureParaLen=4;
       	break;
   	case SENSOR_FEATURE_GET_EXIF_INFO:
       	SENSORDB("SENSOR_FEATURE_GET_EXIF_INFO\n");
        SENSORDB("EXIF addr = 0x%x\n",*pFeatureData32);          
   			*pFeatureData32=0;  
        break;
    default:
        break;
	}
return ERROR_NONE;
}	/* GC0329SFeatureControl() */


SENSOR_FUNCTION_STRUCT	SensorFuncGC0329SYUV=
{
	GC0329SOpen,
	GC0329SGetInfo,
	GC0329SGetResolution,
	GC0329SFeatureControl,
	GC0329SControl,
	GC0329SClose
};


UINT32 GC0329S_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&SensorFuncGC0329SYUV;
	return ERROR_NONE;
} /* SensorInit() */
