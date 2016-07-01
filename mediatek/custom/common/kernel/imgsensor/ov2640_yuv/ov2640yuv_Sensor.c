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
 * $Revision: 1.1 $
 * $Modtime:$
 * $Log: ov2640yuv_Sensor.c,v $
 * Revision 1.1  2013/05/23 03:43:02  ningzhiyu
 * Editor: ningzhiyu
 * Inspector:ningzhiyu
 * Reason: create 72 JB3 module
 * Reference:none
 *
 * Revision 1.1  2013/01/15 04:15:23  zhuyaozhong
 * merge code from BIRD77_ICS2_V13_20130112
 *
 * Revision 1.1  2012/11/27 04:31:41  birdlibo
 * OV2640
 *
 * Revision 1.2  2012/11/16 06:27:13  bianhuannan
 * no message
 *
 * Revision 1.1  2012/11/13 03:25:34  bianhuannan
 * no message
 *
 *
 * 10 13 2010 sean.cheng
 * [ALPS00021684] [Need Patch] [Volunteer Patch] CCT new feature
 * .
 *
 * 09 10 2010 jackie.su
 * [ALPS00002279] [Need Patch] [Volunteer Patch] ALPS.Wxx.xx Volunteer patch for
 * .10y dual sensor
 *
 * 09 02 2010 jackie.su
 * [ALPS00002279] [Need Patch] [Volunteer Patch] ALPS.Wxx.xx Volunteer patch for
 * .roll back dual sensor
 *
 * 07 27 2010 sean.cheng
 * [ALPS00003112] [Need Patch] [Volunteer Patch] ISP/Sensor driver modification for Customer support
 * .1. add master clock switcher 
 *  2. add master enable/disable 
 *  3. add dummy line/pixel for sensor 
 *  4. add sensor driving current setting
 *
 * 07 01 2010 sean.cheng
 * [ALPS00121215][Camera] Change color when switch low and high 
 * .Add video delay frame.
 *
 * 07 01 2010 sean.cheng
 * [ALPS00002805][Need Patch] [Volunteer Patch]10X Patch for DS269 Video Frame Rate 
 * .Change the sensor clock to let frame rate to be 30fps in vidoe mode
 *
 * 06 13 2010 sean.cheng
 * [ALPS00002514][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for E1k Camera 
 * .
 * 1. Add set zoom factor and capdelay frame for YUV sensor 
 * 2. Modify e1k sensor setting
 *
 * 05 25 2010 sean.cheng
 * [ALPS00002250][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for YUV video frame rate 
 * .
 * Add 15fps option for video mode
 *
 * 05 03 2010 sean.cheng
 * [ALPS00001357][Meta]CameraTool 
 * .
 * Fix OV2640 YUV sensor frame rate to 30fps in vidoe mode
 *
 * Mar 4 2010 mtk70508
 * [DUMA00154792] Sensor driver
 * 
 *
 * Mar 4 2010 mtk70508
 * [DUMA00154792] Sensor driver
 * 
 *
 * Mar 1 2010 mtk01118
 * [DUMA00025869] [Camera][YUV I/F & Query feature] check in camera code
 * 
 *
 * Feb 24 2010 mtk01118
 * [DUMA00025869] [Camera][YUV I/F & Query feature] check in camera code
 * 
 *
 * Nov 24 2009 mtk02204
 * [DUMA00015869] [Camera Driver] Modifiy camera related drivers for dual/backup sensor/lens drivers.
 * 
 *
 * Oct 29 2009 mtk02204
 * [DUMA00015869] [Camera Driver] Modifiy camera related drivers for dual/backup sensor/lens drivers.
 * 
 *
 * Oct 27 2009 mtk02204
 * [DUMA00015869] [Camera Driver] Modifiy camera related drivers for dual/backup sensor/lens drivers.
 * 
 *
 * Aug 13 2009 mtk01051
 * [DUMA00009217] [Camera Driver] CCAP First Check In
 * 
 *
 * Aug 5 2009 mtk01051
 * [DUMA00009217] [Camera Driver] CCAP First Check In
 * 
 *
 * Jul 17 2009 mtk01051
 * [DUMA00009217] [Camera Driver] CCAP First Check In
 * 
 *
 * Jul 7 2009 mtk01051
 * [DUMA00008051] [Camera Driver] Add drivers for camera high ISO binning mode.
 * Add ISO query info for Sensor
 *
 * May 18 2009 mtk01051
 * [DUMA00005921] [Camera] LED Flashlight first check in
 * 
 *
 * May 16 2009 mtk01051
 * [DUMA00005921] [Camera] LED Flashlight first check in
 * 
 *
 * May 16 2009 mtk01051
 * [DUMA00005921] [Camera] LED Flashlight first check in
 * 
 *
 * Apr 7 2009 mtk02204
 * [DUMA00004012] [Camera] Restructure and rename camera related custom folders and folder name of came
 * 
 *
 * Mar 27 2009 mtk02204
 * [DUMA00002977] [CCT] First check in of MT6516 CCT drivers
 *
 *
 * Mar 25 2009 mtk02204
 * [DUMA00111570] [Camera] The system crash after some operations
 *
 *
 * Mar 20 2009 mtk02204
 * [DUMA00002977] [CCT] First check in of MT6516 CCT drivers
 *
 *
 * Mar 2 2009 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Feb 24 2009 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Dec 27 2008 MTK01813
 * DUMA_MBJ CheckIn Files
 * created by clearfsimport
 *
 * Dec 10 2008 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Oct 27 2008 mtk01051
 * [DUMA00000851] Camera related drivers check in
 * Modify Copyright Header
 *
 * Oct 24 2008 mtk02204
 * [DUMA00000851] Camera related drivers check in
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
#include <asm/io.h>
	 
#include "kd_camera_hw.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"
	 
#include "ov2640yuv_Sensor.h"
#include "ov2640yuv_Camera_Sensor_para.h"
#include "ov2640yuv_CameraCustomized.h" 


#define OV2640YUV_DEBUG
#ifdef OV2640YUV_DEBUG
#define OV2640SENSORDB printk
#else
#define OV2640SENSORDB(x,...)
#endif

#define BINNING_EN //mingji add

static struct
{
	kal_uint8   Banding;
	kal_bool	NightMode;
	kal_bool    VideoEncodeMode;
	kal_bool    SensorCapState;
	kal_bool    CurrentMode;
	kal_uint16  SensorMode;
	kal_uint16  Fps;
	kal_uint8   IsPVmode;
	kal_uint8   VideoMpeg4;
	kal_uint32  PreviewDummyPixels;
	kal_uint32  PreviewDummyLines;
	kal_uint32  CaptureDummyPixels;
	kal_uint32  CaptureDummyLines;
	kal_uint32  PreviewPclk;
	kal_uint32  CapturePclk;
	kal_uint16  PreviewShutter;
	kal_uint16  CaptureShutter;
	kal_uint16  PreviewGain16;
	kal_uint16  CaptureGain16;
	kal_uint8   Preview_PCLK_Division;
	kal_uint8   Capture_PCLK_Division;
	kal_uint16  Preview_Extra_Lines;
	kal_uint16  Capture_Extra_Lines;
}OV2640Sensor;

MSDK_SCENARIO_ID_ENUM CurrentScenarioId = MSDK_SCENARIO_ID_CAMERA_PREVIEW;

extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);

#define mDELAY(ms)  mdelay(ms)

kal_uint16 OV2640WriteCmosSensor(kal_uint32 Addr, kal_uint32 Para)
{
	char pSendCmd[2] = {(char)(Addr & 0xFF) ,(char)(Para & 0xFF)};

	iWriteRegI2C(pSendCmd , 2,OV2640_WRITE_ID);
}

kal_uint16 OV2640ReadCmosSensor(kal_uint32 Addr)
{
	char pGetByte=0;
	char pSendCmd = (char)(Addr & 0xFF);
  
	iReadRegI2C(&pSendCmd , 1, &pGetByte,1,OV2640_WRITE_ID);
  
	return pGetByte;
}

/* Global Valuable */
static kal_bool OV2640_AE_ENABLE = KAL_TRUE; 
kal_uint16 OV2640CaptureMaxGain16= 8*16;	//8*16;//can change mingji 120212
kal_uint16 OV2640CaptureShutter= 300;
kal_uint16 OV2640CaptureGain16= 32;
kal_uint16 OV2640CaptureExtraLines=0;
/**************************************************************/

//SENSOR_REG_STRUCT OV2640SensorCCT[FACTORY_END_ADDR]=CAMERA_SENSOR_CCT_DEFAULT_VALUE;
//SENSOR_REG_STRUCT OV2640SensorReg[ENGINEER_END]=CAMERA_SENSOR_REG_DEFAULT_VALUE;
//	camera_para.SENSOR.cct	SensorCCT	=> SensorCCT
//	camera_para.SENSOR.reg	SensorReg
MSDK_SENSOR_CONFIG_STRUCT OV2640SensorConfigData;


/*************************************************************************
* FUNCTION
*	OV2640SetDummy
*
* DESCRIPTION
*	This function set the dummy pixels(Horizontal Blanking) & dummy lines(Vertical Blanking), it can be
*	used to adjust the frame rate or gain more time for back-end process.
*	
*	IMPORTANT NOTICE: the base shutter need re-calculate for some sensor, or else flicker may occur.
*
* PARAMETERS
*	1. kal_uint16 : Dummy Pixels (Horizontal Blanking)
*	2. kal_uint16 : Dummy Lines (Vertical Blanking)
*
* RETURNS
*	None
*
*************************************************************************/
static void OV2640SetDummy(kal_uint16 dummy_pixels, kal_uint16 dummy_lines)
{
	OV2640WriteCmosSensor(0xFF, 0x01);
	OV2640WriteCmosSensor(0x2A, (dummy_pixels & 0x0F00) >> 4);
	OV2640WriteCmosSensor(0x2B, dummy_pixels & 0x00FF);
	OV2640WriteCmosSensor(0x46, dummy_lines & 0x00FF);
	OV2640WriteCmosSensor(0x47, dummy_lines >> 8);
}    /* OV2640_set_dummy */

void OV2640WriteExtraLine(kal_uint16 shutter)
{
	kal_uint8 temp_reg1, temp_reg2;

	temp_reg1=shutter&0xff;
	temp_reg2=(shutter&0xff00)>>8;
	
	OV2640WriteCmosSensor(0xFF, 0x01);
	OV2640WriteCmosSensor(0x2d,temp_reg1);	// ADVFL(LSB of extra exposure lines)
	OV2640WriteCmosSensor(0x2e,temp_reg2);	// ADVFH(MSB of extra exposure lines)
}	/* write_OV2640_extral_shutter */

kal_uint16 OV2640ReadExtraLine(void)
{
	kal_uint8 temp_reg1, temp_reg2;
	kal_uint16 temp_reg;

	OV2640WriteCmosSensor(0xFF, 0x01);
	temp_reg1=OV2640ReadCmosSensor(0x2d);										
	temp_reg2=OV2640ReadCmosSensor(0x2e);	
	temp_reg=(temp_reg2<<8)|(temp_reg1);	

	return temp_reg;
}	/* read_OV2640_extral_shutter */

/*************************************************************************
* FUNCTION
*	OV2640WriteShutter
*
* DESCRIPTION
*	This function used to write the shutter.
*
* PARAMETERS
*	1. kal_uint16 : The shutter want to apply to sensor.
*
* RETURNS
*	None
*
*************************************************************************/
static void OV2640WriteShutter(kal_uint16 shutter)
{
	kal_uint8 temp_reg;
	OV2640WriteCmosSensor(0xFF, 0x01);
	temp_reg=OV2640ReadCmosSensor(0x04);
	OV2640WriteCmosSensor(0x04,( (temp_reg&0xFC) | (shutter&0x3) )); // AEC[b1~b0]
	OV2640WriteCmosSensor(0x10,((shutter&0x3FC)>>2)); // AEC[b9~b2]
	OV2640WriteCmosSensor(0x45,((shutter&0xFC00)>>10)); // AEC[b10]/AEC[b15~b10]
}    /* OV2640_write_shutter */

/*************************************************************************
* FUNCTION
*	OV2640ReadShutter
*
* DESCRIPTION
*	This function read current shutter for calculate the exposure.
*
* PARAMETERS
*	None
*
* RETURNS
*	kal_uint16 : The current shutter value.
*
*************************************************************************/
static kal_uint16 OV2640ReadShutter(void)
{
	kal_uint8 temp_reg1, temp_reg2, temp_reg3;
	kal_uint16 temp_reg;

	OV2640WriteCmosSensor(0xFF, 0x01);
	temp_reg1=OV2640ReadCmosSensor(0x04); // AEC[b1~b0]
	temp_reg2=OV2640ReadCmosSensor(0x10); // AEC[b9~b2]
	temp_reg3=OV2640ReadCmosSensor(0x45); // AEC[b15~b10]

	temp_reg=((temp_reg3&0x3F)<<10)|(temp_reg2<<2)|(temp_reg1&0x3); // AEC[b10]/AEC[b15~b10]

	return temp_reg;
}    /* OV2640_read_shutter */

/*************************************************************************
* FUNCTION
*	OV2640WriteWensorGain
*
* DESCRIPTION
*	This function used to write the sensor gain.
*
* PARAMETERS
*	1. kal_uint16 : The sensor gain want to apply to sensor.
*
* RETURNS
*	None
*
*************************************************************************/
static void OV2640WriteSensorGain(kal_uint16 gain)
{
	kal_uint16 temp_reg;

	if(gain > 248)  ASSERT(0);

	temp_reg = 0;
	if (gain > 31)
	{
		temp_reg |= 0x10;
		gain = gain >> 1;
	}
	if (gain > 31)
	{
		temp_reg |= 0x20;
		gain = gain >> 1;
	}

	if (gain > 31)
	{
		temp_reg |= 0x40;
		gain = gain >> 1;
	}
	if (gain > 31)
	{
		temp_reg |= 0x80;
		gain = gain >> 1;
	}

	if (gain > 16)
	{
		temp_reg |= ((gain -16) & 0x0f);
	}

	OV2640WriteCmosSensor(0xFF,0x01);
	OV2640WriteCmosSensor(0x00,temp_reg);
}  /* OV2640_write_sensor_gain */

/*************************************************************************
* FUNCTION
*	OV2640ReadSensorGain
*
* DESCRIPTION
*	This function read current sensor gain for calculate the exposure.
*
* PARAMETERS
*	None
*
* RETURNS
*	kal_uint16 : The current sensor gain value.
*
*************************************************************************/
static kal_uint16 OV2640ReadSensorGain(void)
{
	kal_uint8  temp_reg;
	kal_uint16 sensor_gain;

	OV2640WriteCmosSensor(0xFF,0x01);
	temp_reg=OV2640ReadCmosSensor(0x00);

	sensor_gain=(16+(temp_reg&0x0F));  // ∑≈¥Û¡À16X
	if(temp_reg&0x10)
		sensor_gain<<=1;
	if(temp_reg&0x20)
		sensor_gain<<=1;

	if(temp_reg&0x40)
		sensor_gain<<=1;

	if(temp_reg&0x80)
		sensor_gain<<=1;

	return sensor_gain;
}  /* OV2640ReadSensorGain */

//To avoid increase the saturation.
void OV2640ComputerAECAGC(void) 
{ 
	kal_uint16 PV_Line_Width; 
	kal_uint16 Capture_Line_Width; 
	kal_uint16 Capture_Maximum_Shutter; 
	kal_uint16 Capture_Exposure; 
	kal_uint16 Capture_Gain16; 
	kal_uint16 Capture_Banding_Filter; 
	kal_uint32 Gain_Exposure; 
	kal_uint8 temp;// for low light capture 

	OV2640WriteCmosSensor(0xFF,0x01); 
	temp = OV2640ReadCmosSensor(0x00); 

	PV_Line_Width = OV2640_PV_PERIOD_PIXEL_NUMS + OV2640Sensor.PreviewDummyPixels; 

	Capture_Line_Width = OV2640_FULL_PERIOD_PIXEL_NUMS + OV2640Sensor.CaptureDummyPixels; 
	Capture_Maximum_Shutter = OV2640_FULL_EXPOSURE_LIMITATION + OV2640Sensor.CaptureDummyLines; 

	//Calculate banding filter value, pclk requency uint 100K.
	Capture_Banding_Filter = OV2640Sensor.CapturePclk*100000/(OV2640Sensor.Capture_PCLK_Division+1)/100/ (2*Capture_Line_Width)+1/2; 

	Gain_Exposure = OV2640Sensor.PreviewGain16; 

	Gain_Exposure *=(OV2640Sensor.PreviewShutter+OV2640Sensor.Preview_Extra_Lines); 
	Gain_Exposure *=PV_Line_Width; 
	// Gain_Exposure /=OV2640_g_Preview_PCLK_Frequency; 
	Gain_Exposure /=Capture_Line_Width; 

	//Base the clock and divider, calculate the gain exposure.
	Gain_Exposure = ((2 * Gain_Exposure) * (OV2640Sensor.Preview_PCLK_Division+ 1) / (OV2640Sensor.Capture_PCLK_Division+ 1) );
	Gain_Exposure = (Gain_Exposure * OV2640Sensor.CapturePclk) / OV2640Sensor.PreviewPclk;

	if (temp > 0x78)
		//Gain_Exposure = 1.2 * Gain_Exposure;
		Gain_Exposure = (12 * Gain_Exposure) / 10;
	else if (temp > 0x70)
		//Gain_Exposure = 1.05 * Gain_Exposure;
		Gain_Exposure = (105 * Gain_Exposure) /100;
	else
		Gain_Exposure = Gain_Exposure;
	//End
	
	//redistribute gain and exposure 
	if (Gain_Exposure < Capture_Banding_Filter * 16) // Exposure < 1/100/120 
	{ 
		if(Gain_Exposure<16){//exposure line smaller than 2 lines and gain smaller than 0x08 
			Gain_Exposure = Gain_Exposure*4; 
			Capture_Exposure = 1; 
			Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2/4; 
		} 
		else 
		{ 
			Capture_Exposure = Gain_Exposure /16; 
			Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2; 
		} 
	} 
	else 
	{ 
		if (Gain_Exposure > Capture_Maximum_Shutter * 16) // Exposure > Capture_Maximum_Shutter 
		{ 

			Capture_Exposure = Capture_Maximum_Shutter/Capture_Banding_Filter*Capture_Banding_Filter; 
			Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2; 
			if (Capture_Gain16 > OV2640CaptureMaxGain16) 
			{ 
				// gain reach maximum, insert extra line 
				//Capture_Exposure = Gain_Exposure*1.1/OV2640CaptureMaxGain16; 
				Capture_Exposure = (Gain_Exposure*11)/(10*OV2640CaptureMaxGain16); 

				// Exposure = n/100/120 
				Capture_Exposure = Capture_Exposure/Capture_Banding_Filter; 
				Capture_Exposure =Capture_Exposure * Capture_Banding_Filter; 
				Capture_Gain16 = ((Gain_Exposure *4)/Capture_Exposure + 3)/4; 
			} 
		} 
		else // 1/100 < Exposure < Capture_Maximum_Shutter, Exposure = n/100/120 
		{ 
			Capture_Exposure = Gain_Exposure/16/Capture_Banding_Filter; 
			Capture_Exposure = Capture_Exposure * Capture_Banding_Filter; 
			Capture_Gain16 = (Gain_Exposure*2 +1) / Capture_Exposure/2; 
		} 
	} 

	OV2640Sensor.CaptureGain16 = Capture_Gain16; 
	OV2640Sensor.Capture_Extra_Lines = (Capture_Exposure > Capture_Maximum_Shutter)? 
	(Capture_Exposure - Capture_Maximum_Shutter/Capture_Banding_Filter*Capture_Banding_Filter):0; 

	OV2640Sensor.CaptureShutter = (Capture_Exposure - OV2640Sensor.Capture_Extra_Lines); 
}

static void OV2640_set_AE_mode(kal_bool AE_enable)
{
	kal_uint8 temp_AE_reg = 0;
	
	if (AE_enable == KAL_TRUE)
	{
		// turn on AEC/AGC
		OV2640WriteCmosSensor(0xFF ,0x01);
		temp_AE_reg = OV2640ReadCmosSensor(0x13);
		OV2640WriteCmosSensor(0x13, temp_AE_reg| 0x05);
		//OV2640_write_cmos_sensor(0x13, temp_AE_reg| 0x01);
	}
	else
	{
		// turn off AEC/AGC
		OV2640WriteCmosSensor(0xFF ,0x01);
		temp_AE_reg = OV2640ReadCmosSensor(0x13);
		OV2640WriteCmosSensor(0x13, temp_AE_reg&~0x05);
		//OV2640_write_cmos_sensor(0x13, temp_AE_reg&~0x01);
	}
}

static void OV2640_set_AWB_mode(kal_bool AWB_enable)
{
	OV2640SENSORDB("[OV2640]OV2640_set_AWB_mode enter :\n ");

	kal_uint8 temp_AWB_reg = 0;

	//return ;

	if (AWB_enable == KAL_TRUE)
	{
		OV2640SENSORDB("[OV2640]OV2640_set_AWB_mode AWB_enable == KAL_TRUE \n ");
		//enable Auto WB ??? (need check)
		//OV2640WriteCmosSensor(OV2640_PAGE_SETTING_REG ,0x00);
		//temp_AWB_reg = OV2640ReadCmosSensor(0xc7);
		//OV2640WriteCmosSensor(0xc7, temp_AWB_reg | 0x40);
		
		//enable Auto WB
		OV2640WriteCmosSensor(0xFF ,0x00);
		temp_AWB_reg = OV2640ReadCmosSensor(0xc3);
		OV2640WriteCmosSensor(0xc3, temp_AWB_reg | 0x08);
	}
	else
	{
		OV2640SENSORDB("[OV2640]OV2640_set_AWB_mode AWB_enable == KAL_FALSE \n ");
		// Disable Auto WB ?? (need check)
		//OV2640WriteCmosSensor(OV2640_PAGE_SETTING_REG ,0x00);
		//temp_AWB_reg = OV2640ReadCmosSensor(0xc7);
		//OV2640WriteCmosSensor(0xc7, temp_AWB_reg & ~0x40);
		
		//turn off AWB
		OV2640WriteCmosSensor(0xFF ,0x00);
		temp_AWB_reg = OV2640ReadCmosSensor(0xc3);
		OV2640WriteCmosSensor(0xc3, temp_AWB_reg & ~0x08);
	}
	
	OV2640SENSORDB("[OV2640]OV2640_set_AWB_mode exit :\n ");
}

/*************************************************************************
* FUNCTION
*	OV2640NightMode
*
* DESCRIPTION
*	This function night mode of OV2640.
*
* PARAMETERS
*	none
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void OV2640NightMode(kal_bool enable)
{
	OV2640SENSORDB("[OV2640]OV2640NightMode enter :\n ");

	OV2640WriteCmosSensor(0xFF ,0x01);

	if (enable) {
		OV2640SENSORDB("[OV2640]OV2640NightMode night mode \n ");
		OV2640WriteCmosSensor(0x11, 0x01);		// night mode: 15fps - MPEG4, 10fps - MJPG

		// The following setting use to fix frame rate.
		OV2640WriteCmosSensor(0x03,0x0f);

		OV2640WriteCmosSensor(0x0f, 0x43);		// Disable 0x2D, 0x2E update automatically.
		OV2640WriteCmosSensor(0x2d, 0x00);
		OV2640WriteCmosSensor(0x2e, 0x00);

		OV2640WriteCmosSensor(0x14,0x28);
		OV2640WriteCmosSensor(0x03,0x8f);
		OV2640WriteCmosSensor(0x0f,0x4b);
		OV2640Sensor.Preview_PCLK_Division=1;
		OV2640Sensor.CurrentMode = KAL_TRUE;
	}
	else{
		OV2640SENSORDB("[OV2640]OV2640NightMode normal mode \n ");
	
		OV2640WriteCmosSensor(0x11, 0x00);       // Normal: 30fps - MPEG4, 20fps - MJPG
		OV2640WriteCmosSensor(0x03, 0x0f);

		OV2640WriteCmosSensor(0x0f, 0x43);	// Disable 0x2D, 0x2E update automatically.
		OV2640WriteCmosSensor(0x2d, 0x00);
		OV2640WriteCmosSensor(0x2e, 0x00);

		OV2640WriteCmosSensor(0x14,0x28);
		OV2640WriteCmosSensor(0x03,0x4f);
		OV2640WriteCmosSensor(0x0f,0x4b);
		OV2640Sensor.Preview_PCLK_Division=0;
		OV2640Sensor.CurrentMode = KAL_FALSE;
	}
	OV2640SENSORDB("[OV2640]OV2640NightMode exit :\n ");

}	/* OV2640NightMode */

/*************************************************************************
* FUNCTION
*	OV2640GetSensorID
*
* DESCRIPTION
*	This function get the sensor ID
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
static kal_uint32 OV2640GetSensorID(UINT32 *sensorID)
{
	//  Read sensor ID to adjust I2C is OK?
	*sensorID = (OV2640ReadCmosSensor(0x0A) << 8) | OV2640ReadCmosSensor(0x0B);
	printk("MYCAT [OV2640]OV2640GetSensorID : sensorID=0x%x \n ",*sensorID);
	if(*sensorID != OV2640_SENSOR_ID)
	{
		return ERROR_SENSOR_CONNECT_FAIL;
	}
	return ERROR_NONE;
}   

/*************************************************************************
* FUNCTION
*    OV2640InitialSetting
*
* DESCRIPTION
*    This function initialize the registers of CMOS sensor.
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void OV2640InitialSetting(void)
{
	OV2640WriteCmosSensor(0xff, 0x00);
	OV2640WriteCmosSensor(0x2c, 0xff);
	OV2640WriteCmosSensor(0x2e, 0xdf);
	OV2640WriteCmosSensor(0xff, 0x01);
	OV2640WriteCmosSensor(0x3c, 0x32);
	OV2640WriteCmosSensor(0x11, 0x00);

	OV2640WriteCmosSensor(0x09, 0x02);
	OV2640WriteCmosSensor(0x04, 0xF8); //78 up and down 28 up down and left light68
	OV2640WriteCmosSensor(0x13, 0xe5); 
	//OV2640WriteCmosSensor(0x14, 0x68);		// 128x
	OV2640WriteCmosSensor(0x14, 0x28);//0x68 20101118	
	
	OV2640WriteCmosSensor(0x2c, 0x0c); 
	OV2640WriteCmosSensor(0x33, 0x78); 
	OV2640WriteCmosSensor(0x3a, 0x33);
	OV2640WriteCmosSensor(0x3b, 0xfb);

	OV2640WriteCmosSensor(0x3e, 0x00); 
	OV2640WriteCmosSensor(0x43, 0x11);
	OV2640WriteCmosSensor(0x16, 0x10);

	OV2640WriteCmosSensor(0x39, 0x02);//soso
	//OV2640WriteCmosSensor(0x39, 0x82);	//Bypass the internal regulater

	OV2640WriteCmosSensor(0x35, 0xda);////
	OV2640WriteCmosSensor(0x22, 0x19);
	OV2640WriteCmosSensor(0x37, 0xc3);
	OV2640WriteCmosSensor(0x23, 0x00);
	OV2640WriteCmosSensor(0x34, 0xc0);
	OV2640WriteCmosSensor(0x36, 0x1a);
	OV2640WriteCmosSensor(0x06, 0x88);
	OV2640WriteCmosSensor(0x07, 0xc0);
	OV2640WriteCmosSensor(0x0d, 0x8f);	//
	OV2640WriteCmosSensor(0x0e, 0x41);
	OV2640WriteCmosSensor(0x4c, 0x00);

	OV2640WriteCmosSensor(0x4a, 0x81);
	OV2640WriteCmosSensor(0x21, 0x99);

	OV2640WriteCmosSensor(0x24, 0x40);	 //40
	OV2640WriteCmosSensor(0x25, 0x38);	 //38
	OV2640WriteCmosSensor(0x26, 0x82);	 //82
	OV2640WriteCmosSensor(0x5c, 0x00);
	OV2640WriteCmosSensor(0x63, 0x00);

	OV2640WriteCmosSensor(0x46, 0x86);//103
	OV2640WriteCmosSensor(0x0c, 0x3c);//60hz

	//OV2640WriteCmosSensor(0x5d, 0x55);//0x55
	//OV2640WriteCmosSensor(0x5e, 0x55);//0x7d
	//OV2640WriteCmosSensor(0x5f, 0x55);//0x7d
	//OV2640WriteCmosSensor(0x5e, 0x55);	//To control the average weight option.
	//OV2640WriteCmosSensor(0x5f, 0x55);
	
	//OV2640WriteCmosSensor(0x60, 0x55);//0x55

	OV2640WriteCmosSensor(0x61, 0x70);
	OV2640WriteCmosSensor(0x62, 0x80);
	OV2640WriteCmosSensor(0x7c, 0x05); 
	OV2640WriteCmosSensor(0x20, 0x80);
	OV2640WriteCmosSensor(0x28, 0x30);
	OV2640WriteCmosSensor(0x6c, 0x00);
	OV2640WriteCmosSensor(0x6d, 0x80);
	OV2640WriteCmosSensor(0x6e, 0x00);
	OV2640WriteCmosSensor(0x70, 0x02);
	OV2640WriteCmosSensor(0x71, 0x94);
	OV2640WriteCmosSensor(0x73, 0xc1);

	OV2640WriteCmosSensor(0x3d, 0x38);	
	OV2640WriteCmosSensor(0x5a, 0x33);	 
	OV2640WriteCmosSensor(0x4f, 0xC9);	 
	OV2640WriteCmosSensor(0x50, 0xA8);	 
	OV2640WriteCmosSensor(0x12, 0x40);
	OV2640WriteCmosSensor(0x17, 0x11);
	OV2640WriteCmosSensor(0x18, 0x43);
	//OV2640WriteCmosSensor(0x19, 0x01);
	//OV2640WriteCmosSensor(0x1A, 0x4c);
	OV2640WriteCmosSensor(0x19, 0x00);
	OV2640WriteCmosSensor(0x1A, 0x4b);
	
	OV2640WriteCmosSensor(0x32, 0x09);
	OV2640WriteCmosSensor(0x6D, 0x00);

	OV2640WriteCmosSensor(0x03, 0x4f);
	OV2640WriteCmosSensor(0x0f, 0x4b);

	OV2640WriteCmosSensor(0xff, 0x00);
	OV2640WriteCmosSensor(0xe5, 0x7f);
	OV2640WriteCmosSensor(0xf9, 0xc0);
	OV2640WriteCmosSensor(0x41, 0x24);
	OV2640WriteCmosSensor(0xe0, 0x14);
	OV2640WriteCmosSensor(0x76, 0xff);
	OV2640WriteCmosSensor(0x33, 0xa0);
	OV2640WriteCmosSensor(0x42, 0x20);
	OV2640WriteCmosSensor(0x43, 0x18);
	OV2640WriteCmosSensor(0x4c, 0x00);
	OV2640WriteCmosSensor(0x87, 0xd5);//LENS
	OV2640WriteCmosSensor(0x88, 0x3f);
	OV2640WriteCmosSensor(0xd7, 0x03);
	OV2640WriteCmosSensor(0xd9, 0x10);
	OV2640WriteCmosSensor(0xd3, 0x82);

	//OV2640WriteCmosSensor(0xc8, 0x08);
	OV2640WriteCmosSensor(0xc8, 0x04);	//To decrease the suturation, low light. 10-08
	OV2640WriteCmosSensor(0xc9, 0x40);

	OV2640WriteCmosSensor(0x7c, 0x00);
	OV2640WriteCmosSensor(0x7d, 0x06);
	OV2640WriteCmosSensor(0x7c, 0x03);
	OV2640WriteCmosSensor(0x7d, 0x4d);
	OV2640WriteCmosSensor(0x7d, 0x4d);
	OV2640WriteCmosSensor(0x7c, 0x08);
	OV2640WriteCmosSensor(0x7d, 0x24);
	OV2640WriteCmosSensor(0x7d, 0x10);
	OV2640WriteCmosSensor(0x7d, 0x0e);

	//gamma
#if 1  //nice
	OV2640WriteCmosSensor(0x90, 0x00);
	OV2640WriteCmosSensor(0x91, 0x0c);
	OV2640WriteCmosSensor(0x91, 0x19);
	OV2640WriteCmosSensor(0x91, 0x28);
	OV2640WriteCmosSensor(0x91, 0x40);
	OV2640WriteCmosSensor(0x91, 0x4b);
	OV2640WriteCmosSensor(0x91, 0x56);
	OV2640WriteCmosSensor(0x91, 0x60);
	
	OV2640WriteCmosSensor(0x91, 0x67);
	OV2640WriteCmosSensor(0x91, 0x70);
	OV2640WriteCmosSensor(0x91, 0x7a);
	OV2640WriteCmosSensor(0x91, 0x89);
	OV2640WriteCmosSensor(0x91, 0x95);
	OV2640WriteCmosSensor(0x91, 0xaf);
	OV2640WriteCmosSensor(0x91, 0xc8);
	OV2640WriteCmosSensor(0x91, 0xdf);
	OV2640WriteCmosSensor(0x91, 0x2c);
#else
	OV2640WriteCmosSensor(0x90, 0x00);
	OV2640WriteCmosSensor(0x91, 0x04);
	OV2640WriteCmosSensor(0x91, 0x0c);
	OV2640WriteCmosSensor(0x91, 0x20);
	OV2640WriteCmosSensor(0x91, 0x4c);
	OV2640WriteCmosSensor(0x91, 0x60);
	OV2640WriteCmosSensor(0x91, 0x74);
	OV2640WriteCmosSensor(0x91, 0x82);
	OV2640WriteCmosSensor(0x91, 0x8e);
	OV2640WriteCmosSensor(0x91, 0x97);
	OV2640WriteCmosSensor(0x91, 0x9f);
	OV2640WriteCmosSensor(0x91, 0xaa);
	OV2640WriteCmosSensor(0x91, 0xb4);
	OV2640WriteCmosSensor(0x91, 0xc8);
	OV2640WriteCmosSensor(0x91, 0xd9);
	OV2640WriteCmosSensor(0x91, 0xe8);
	OV2640WriteCmosSensor(0x91, 0x20);
#endif
	OV2640WriteCmosSensor(0x92, 0x00);
	OV2640WriteCmosSensor(0x93, 0x02);//  06 
	OV2640WriteCmosSensor(0x93, 0xe4);//  e3 
	OV2640WriteCmosSensor(0x93, 0x02);//  03 
	OV2640WriteCmosSensor(0x93, 0x06);//  03 
	OV2640WriteCmosSensor(0x93, 0x00); 
	OV2640WriteCmosSensor(0x93, 0x02);

	//matrix
	OV2640WriteCmosSensor(0x96, 0x00);
	OV2640WriteCmosSensor(0x97, 0x08);
	OV2640WriteCmosSensor(0x97, 0x19);
	OV2640WriteCmosSensor(0x97, 0x02);
	OV2640WriteCmosSensor(0x97, 0x0c);//cmx-3
	OV2640WriteCmosSensor(0x97, 0x24);
	OV2640WriteCmosSensor(0x97, 0x30);
	OV2640WriteCmosSensor(0x97, 0x28);
	OV2640WriteCmosSensor(0x97, 0x26);
	OV2640WriteCmosSensor(0x97, 0x02);
	OV2640WriteCmosSensor(0x97, 0x98);
	OV2640WriteCmosSensor(0x97, 0x80);
	OV2640WriteCmosSensor(0x97, 0x00);
	OV2640WriteCmosSensor(0x97, 0x00);
	OV2640WriteCmosSensor(0xc7, 0x00);//add 0221

	//awb
	OV2640WriteCmosSensor(0xbf, 0x00);
	OV2640WriteCmosSensor(0xba, 0xdc);
	OV2640WriteCmosSensor(0xbb, 0x08);
	#if 0
	OV2640WriteCmosSensor(0xb6, 0x4e);//0x20
	OV2640WriteCmosSensor(0xb8, 0x5a);//0x30
	OV2640WriteCmosSensor(0xb7, 0x34);//0x20
	OV2640WriteCmosSensor(0xb9, 0x40);//0x30
	OV2640WriteCmosSensor(0xb3, 0xcd);//0xb4
	OV2640WriteCmosSensor(0xb4, 0x70);//0xca
	OV2640WriteCmosSensor(0xb5, 0x45);//0x35
	OV2640WriteCmosSensor(0xb0, 0x85);//0x46
	OV2640WriteCmosSensor(0xb1, 0x50);//0x46
	OV2640WriteCmosSensor(0xb2, 0x03);//0x06
	#else
	OV2640WriteCmosSensor(0xb6, 0x20);
	OV2640WriteCmosSensor(0xb8, 0x30);
	OV2640WriteCmosSensor(0xb7, 0x20);
	OV2640WriteCmosSensor(0xb9, 0x30);
	OV2640WriteCmosSensor(0xb3, 0xb4);
	OV2640WriteCmosSensor(0xb4, 0xca);
	OV2640WriteCmosSensor(0xb5, 0x35);
	OV2640WriteCmosSensor(0xb0, 0x46);
	OV2640WriteCmosSensor(0xb1, 0x46);
	OV2640WriteCmosSensor(0xb2, 0x06);
	#endif
	OV2640WriteCmosSensor(0xc7, 0x00);
	OV2640WriteCmosSensor(0xc6, 0x51);
	OV2640WriteCmosSensor(0xc5, 0x11);
	OV2640WriteCmosSensor(0xc4, 0x9c);
	OV2640WriteCmosSensor(0xbc, 0x60);
	OV2640WriteCmosSensor(0xbd, 0x40);
	OV2640WriteCmosSensor(0xbe, 0x60);

	OV2640WriteCmosSensor(0xca, 0x01);
	OV2640WriteCmosSensor(0xcb, 0x58);
	OV2640WriteCmosSensor(0xcb, 0x40);
	OV2640WriteCmosSensor(0xcb, 0x4c);

	//lens
	/*
	OV2640WriteCmosSensor(0xa6, 0x00);
	OV2640WriteCmosSensor(0xa7, 0x80);//ae
	OV2640WriteCmosSensor(0xa7, 0x98);
	OV2640WriteCmosSensor(0xa7, 0x27);//0x2d
	OV2640WriteCmosSensor(0xa7, 0x21);
	OV2640WriteCmosSensor(0xa7, 0x0 );
	OV2640WriteCmosSensor(0xa7, 0x28);
	OV2640WriteCmosSensor(0xa7, 0x80);
	OV2640WriteCmosSensor(0xa7, 0xa0);
	OV2640WriteCmosSensor(0xa7, 0x21);
	OV2640WriteCmosSensor(0xa7, 0x21);
	OV2640WriteCmosSensor(0xa7, 0x0 );
	OV2640WriteCmosSensor(0xa7, 0x28);
	OV2640WriteCmosSensor(0xa7, 0x80);
	OV2640WriteCmosSensor(0xa7, 0xa0);
	OV2640WriteCmosSensor(0xa7, 0x21);
	OV2640WriteCmosSensor(0xa7, 0x21);
	OV2640WriteCmosSensor(0xa7, 0x0 );
	OV2640WriteCmosSensor(0xa7, 0x28);*/// modified
	OV2640WriteCmosSensor(0xff, 0x00);
	OV2640WriteCmosSensor(0xa6, 0x00);
	OV2640WriteCmosSensor(0xa7, 0x20);//ae
	OV2640WriteCmosSensor(0xa7, 0x80);
	OV2640WriteCmosSensor(0xa7, 0x21);//0x2d
	OV2640WriteCmosSensor(0xa7, 0x31);
	OV2640WriteCmosSensor(0xa7, 0x10);
	OV2640WriteCmosSensor(0xa7, 0x28);
	OV2640WriteCmosSensor(0xa7, 0x20);
	OV2640WriteCmosSensor(0xa7, 0x80);
	OV2640WriteCmosSensor(0xa7, 0x19);
	OV2640WriteCmosSensor(0xa7, 0x31);
	OV2640WriteCmosSensor(0xa7, 0x10);
	OV2640WriteCmosSensor(0xa7, 0x28);
	OV2640WriteCmosSensor(0xa7, 0x20);
	OV2640WriteCmosSensor(0xa7, 0x80);
	OV2640WriteCmosSensor(0xa7, 0x17);
	OV2640WriteCmosSensor(0xa7, 0x31);
	OV2640WriteCmosSensor(0xa7, 0x00);
	OV2640WriteCmosSensor(0xa7, 0x28);
	OV2640WriteCmosSensor(0xc3, 0x02);

	OV2640WriteCmosSensor(0xc0, 0x64);
	OV2640WriteCmosSensor(0xc1, 0x4b);
	OV2640WriteCmosSensor(0x8c, 0x00);
	OV2640WriteCmosSensor(0x86, 0x3d);//enable dcw 
	OV2640WriteCmosSensor(0x50, 0x00);
	OV2640WriteCmosSensor(0x51, 0xc8);
	OV2640WriteCmosSensor(0x52, 0x96);
	OV2640WriteCmosSensor(0x53, 0x00);
	OV2640WriteCmosSensor(0x54, 0x00);
	OV2640WriteCmosSensor(0x55, 0x00);
	OV2640WriteCmosSensor(0x57, 0x00);
	OV2640WriteCmosSensor(0x5a, 0xa0);
	OV2640WriteCmosSensor(0x5b, 0x78);
	OV2640WriteCmosSensor(0x5c, 0x00);
	OV2640WriteCmosSensor(0xd3, 0x02);

	OV2640WriteCmosSensor(0xc3, 0xef);
	OV2640WriteCmosSensor(0x7f, 0x00);

	OV2640WriteCmosSensor(0xda, 0x00);//0x00 modified

	OV2640WriteCmosSensor(0xe5, 0x1f);
	OV2640WriteCmosSensor(0xe1, 0x67);
	OV2640WriteCmosSensor(0xe0, 0x00);
	OV2640WriteCmosSensor(0xdd, 0xff);
	OV2640WriteCmosSensor(0x05, 0x00);
}                                  

/*************************************************************************
* FUNCTION
*    OV2640PreviewSetting
*
* DESCRIPTION
*    This function config Preview setting related registers of CMOS sensor.
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void OV2640PreviewSetting(void)
{
	//-----------------------------------
	//From capture to preview
	//-----------------------------------
	OV2640WriteCmosSensor(0xff,0x01);
	OV2640WriteCmosSensor(0x12,0x40);
	OV2640WriteCmosSensor(0x17,0x11);
	OV2640WriteCmosSensor(0x18,0x43);
	OV2640WriteCmosSensor(0x32,0x09);
	//OV2640WriteCmosSensor(0x19,0x01);
	//OV2640WriteCmosSensor(0x1a,0x4c);

	// add 0222
	OV2640WriteCmosSensor(0x04,( (OV2640ReadCmosSensor(0x04)&0xFC) | (OV2640Sensor.CaptureShutter&0x3) )); // AEC[b1~b0]
	OV2640WriteCmosSensor(0x10,((OV2640Sensor.CaptureShutter>>2)&0x3FC)); // AEC[b9~b2]
	OV2640WriteCmosSensor(0x45,((OV2640Sensor.CaptureShutter>>10)&0xFC00)); // AEC[b10]/AEC[b15~b10]
	OV2640WriteCmosSensor(0x00,OV2640Sensor.CaptureGain16);
	
	OV2640WriteCmosSensor(0x19,0x00);
	OV2640WriteCmosSensor(0x1A,0x4b);
	
	//OV2640WriteCmosSensor(0x03,0x00);

	OV2640WriteCmosSensor(0x03,0x4f);
	OV2640WriteCmosSensor(0x0f,0x4b);
	
	OV2640WriteCmosSensor(0x13,0xe5);

	OV2640WriteCmosSensor(0x2d,0x00);	
	OV2640WriteCmosSensor(0x2e,0x00);

	OV2640WriteCmosSensor(0x11,0x00);

	OV2640WriteCmosSensor(0x4f,0xc9);
	OV2640WriteCmosSensor(0x50,0xa8);
	OV2640WriteCmosSensor(0x5a,0x33);
	OV2640WriteCmosSensor(0x6d,0x00);
	OV2640WriteCmosSensor(0x3d,0x38);

	OV2640WriteCmosSensor(0x39,0x12); //soso change to not bypass internal regulater
	//OV2640WriteCmosSensor(0x39, 0x92);	//Bit[7] Bypass the internal regulater
	
	OV2640WriteCmosSensor(0x35,0xda);
	//OV2640WriteCmosSensor(0x22,0x1a); 
	OV2640WriteCmosSensor(0x22,0x19); 	//From setting.

	OV2640WriteCmosSensor(0x37,0xc3); 
	OV2640WriteCmosSensor(0x23,0x00); 
	OV2640WriteCmosSensor(0x34,0xc0); 
	OV2640WriteCmosSensor(0x36,0x1a);
	OV2640WriteCmosSensor(0x06,0x88); 
	OV2640WriteCmosSensor(0x07,0xc0);
	OV2640WriteCmosSensor(0x0d,0x8f); 
	OV2640WriteCmosSensor(0x0e,0x41); 
	OV2640WriteCmosSensor(0x4c,0x00);
	OV2640WriteCmosSensor(0xff,0x00);
	OV2640WriteCmosSensor(0x87,0xd5);  

	OV2640WriteCmosSensor(0xe0,0x04);

	OV2640WriteCmosSensor(0xc0,0x64);
	OV2640WriteCmosSensor(0xc1,0x4b);
	OV2640WriteCmosSensor(0x8c,0x00);

	OV2640WriteCmosSensor(0x86,0x3D);
	//OV2640WriteCmosSensor(0x86,0x35);	//Turn off U/V auto adjust.
	OV2640WriteCmosSensor(0xff,0x00);
	OV2640WriteCmosSensor(0x7c,0x00);
	OV2640WriteCmosSensor(0x7d,(OV2640ReadCmosSensor(0x7d)& 0xfd));
	OV2640WriteCmosSensor(0x7c,0x03);
	OV2640WriteCmosSensor(0x7d,0x40);
	OV2640WriteCmosSensor(0x7d,0x40);

	OV2640WriteCmosSensor(0xc9,0x40);	//Auto U/V

	OV2640WriteCmosSensor(0x50,0x00);
	OV2640WriteCmosSensor(0x51,0xC8);
	OV2640WriteCmosSensor(0x52,0x96);
	OV2640WriteCmosSensor(0x53,0x00);
	OV2640WriteCmosSensor(0x54,0x00);
	OV2640WriteCmosSensor(0x55,0x00);
	OV2640WriteCmosSensor(0x5a,0xC8);
	OV2640WriteCmosSensor(0x5b,0x96);
	OV2640WriteCmosSensor(0x5c,0x00);
	OV2640WriteCmosSensor(0xd3,0x82);
	OV2640WriteCmosSensor(0xdd,0xff);
	OV2640WriteCmosSensor(0xe0,0x00);

	OV2640Sensor.IsPVmode = KAL_TRUE;
	OV2640Sensor.PreviewPclk= 480;
}

/*************************************************************************
* FUNCTION
*     OV2640FullSizeCaptureSetting
*
* DESCRIPTION
*    This function config full size capture setting related registers of CMOS sensor.
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void OV2640FullSizeCaptureSetting(void)
{
	//From jerry latest update.
	OV2640WriteCmosSensor(0xff,0x01);
	OV2640WriteCmosSensor(0x12,0x00);
	OV2640WriteCmosSensor(0x11,0x01);
	OV2640WriteCmosSensor(0x17,0x11);
	OV2640WriteCmosSensor(0x18,0x75);
	OV2640WriteCmosSensor(0x32,0x36);
	//OV2640WriteCmosSensor(0x19,0x01);
	//OV2640WriteCmosSensor(0x1a,0x97);
	OV2640WriteCmosSensor(0x19,0x02);	//For the offset
	OV2640WriteCmosSensor(0x1a,0x98);
	
	OV2640WriteCmosSensor(0x37,0x40);

	OV2640WriteCmosSensor(0x4f,0xb4);
	OV2640WriteCmosSensor(0x50,0x9c);
	OV2640WriteCmosSensor(0x5a,0x57);
	OV2640WriteCmosSensor(0x6d,0x80);
	OV2640WriteCmosSensor(0x3d,0x38);

	OV2640WriteCmosSensor(0x39,0x02);//soso
	//OV2640WriteCmosSensor(0x39, 0x82);	//Bypass the internal regulater

	OV2640WriteCmosSensor(0x35,0x88);

	OV2640WriteCmosSensor(0x22,0x09); //bit[4] for fix fix pattern noise
	OV2640WriteCmosSensor(0x37,0x40);
	OV2640WriteCmosSensor(0x23,0x00);
	OV2640WriteCmosSensor(0x34,0xa0);
	OV2640WriteCmosSensor(0x36,0x1a);
	OV2640WriteCmosSensor(0x06,0x02);
	OV2640WriteCmosSensor(0x07,0xc0);
	OV2640WriteCmosSensor(0x0d,0xbf);
	OV2640WriteCmosSensor(0x0e,0x01);
	OV2640WriteCmosSensor(0x4c,0x00);
	
	OV2640WriteCmosSensor(0x03,0x0f);
	OV2640WriteCmosSensor(0x0f,0x43);
	
	OV2640WriteCmosSensor(0x2d,0x00);
	OV2640WriteCmosSensor(0x2e,0x00);

	OV2640WriteCmosSensor(0xff,0x00);
	OV2640WriteCmosSensor(0x87,0xd0);		
	OV2640WriteCmosSensor(0xc0,0xc8);
	OV2640WriteCmosSensor(0xc1,0x96);
	OV2640WriteCmosSensor(0x86,0x15);//turn off auto uv
	OV2640WriteCmosSensor(0x50,0x00);
	OV2640WriteCmosSensor(0x51,0x90);
	OV2640WriteCmosSensor(0x52,0x2c);
	OV2640WriteCmosSensor(0x53,0x00);
	OV2640WriteCmosSensor(0x54,0x00);
	OV2640WriteCmosSensor(0x55,0x88);		
	OV2640WriteCmosSensor(0x5a,0x90);
	OV2640WriteCmosSensor(0x5b,0x2c);
	OV2640WriteCmosSensor(0x5c,0x05);
	OV2640WriteCmosSensor(0xd3,0x82);

	OV2640Sensor.IsPVmode = KAL_FALSE;
	OV2640Sensor.CapturePclk= 480;
}

/*************************************************************************
* FUNCTION
*    OV2640SetHVMirror
*
* DESCRIPTION
*    This function set sensor Mirror
*
* PARAMETERS
*    Mirror
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void OV2640SetHVMirror(kal_uint8 Mirror, kal_uint16 *iStartX, kal_uint16 *iStartY)
{
	kal_uint8 iTemp = 0;
	
	OV2640WriteCmosSensor(0xFF ,0x01);
	iTemp = OV2640ReadCmosSensor(0x04) & 0x3f;

	OV2640SENSORDB("[OV2640]OV2640SetHVMirror ENTER  Mirror==%d:\n "+Mirror);
	switch (Mirror)
	{
	case IMAGE_NORMAL:
		*iStartX = 1;
		*iStartY = 1;
		// do nothing
	    	break;
	case IMAGE_H_MIRROR:
		*iStartX = 1;
		*iStartY = 1;
	    	iTemp |= 0x80;//0x80
	    	break;
	case IMAGE_V_MIRROR:	//Flip Register 0x04[6] and 0x04[4] (FF = 01)
		*iStartX = 1;
		*iStartY = 1;
	    	iTemp |= 0x50;//0x50
	   	break;

	case IMAGE_HV_MIRROR:
		*iStartX = 1;
		*iStartY = 1;
	    	iTemp |= 0xd0;//0xd0
	    	break;

	default:
	    	ASSERT(0);
	}
	OV2640WriteCmosSensor(0x04, iTemp);
}

/*************************************************************************
* FUNCTION
*	OV2640Open
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
UINT32 OV2640Open(void)
{
	volatile signed int i;
	kal_uint16 sensor_id = 0;

	printk("MYCAT [OV2640]OV2640Open enter :\n ");
	OV2640WriteCmosSensor(0xFF,0x01);
	// Adviced by vendor, reset PLL 2times, it's used to solve the sensor power on sequence not stalbe.
	OV2640WriteCmosSensor(0x3c, 0x3f);	// Reset PLL
	mDELAY(2);
	//OV2640WriteCmosSensor(0x3c, 0x3f);	// Reset PLL
	//mDELAY(2);

	OV2640WriteCmosSensor(0x12,0x80);
	mDELAY(3);

	//  Read sensor ID to adjust I2C is OK?
	for(i=0;i<3;i++)
	{
		sensor_id = (OV2640ReadCmosSensor(0x0A) << 8) | OV2640ReadCmosSensor(0x0B);
		printk("MYCAT OV2640Open OV2640 READ ID :%x\n",sensor_id);
		if(sensor_id != OV2640_SENSOR_ID)
		{
			return ERROR_SENSOR_CONNECT_FAIL;
		}
	}
	
	OV2640InitialSetting();	
	
	OV2640Sensor.IsPVmode = KAL_TRUE;
	OV2640Sensor.VideoEncodeMode = KAL_FALSE;
	OV2640Sensor.SensorCapState = KAL_FALSE;
	OV2640Sensor.VideoMpeg4 = KAL_FALSE;
	OV2640Sensor.PreviewDummyPixels = 0;
	OV2640Sensor.PreviewDummyLines = 0;
	OV2640Sensor.CaptureDummyPixels = 0;
	OV2640Sensor.CaptureDummyLines = 0;
	OV2640Sensor.PreviewPclk = 480;
	OV2640Sensor.CapturePclk = 480;
	OV2640Sensor.PreviewShutter = 1;
	OV2640Sensor.CaptureShutter = 300;
	OV2640Sensor.PreviewGain16 = 1;
	OV2640Sensor.CaptureGain16 = 32;
	OV2640Sensor.Preview_PCLK_Division = 0;
	OV2640Sensor.Capture_PCLK_Division = 1;
	OV2640Sensor.Preview_Extra_Lines = 0;
	OV2640Sensor.Capture_Extra_Lines = 0;
	
	OV2640Sensor.SensorMode = SENSOR_MODE_INIT;// add
	OV2640Sensor.CurrentMode = KAL_FALSE;
	
	printk("MYCAT [OV2640]OV2640Open exit :\n ");

	return ERROR_NONE;
}	/* OV2640Open() */

/*************************************************************************
* FUNCTION
*	OV2640Close
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
UINT32 OV2640Close(void)
{
//	CISModulePowerOn(FALSE);
	return ERROR_NONE;
}	/* OV2640Close() */

/*************************************************************************
* FUNCTION
*	OV2640Preview
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
UINT32 OV2640Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	kal_uint8 iTemp, temp_AE_reg, temp_AWB_reg;
	kal_uint16 iDummyPixels = 0, iDummyLines = 0, iStartX = 0, iStartY = 0;
	kal_uint32 shutter = 0;
	
	OV2640SENSORDB("[OV2640]OV2640Preview enter :\n ");
	OV2640Sensor.SensorCapState = KAL_FALSE;
	OV2640Sensor.SensorMode = SENSOR_MODE_PREVIEW;// add 

	OV2640PreviewSetting();

	//PCLK--48MHz. PCLK is same as fINT CLK for YUV mode. if RAW mode, PCLK will be fINT CLK/2
	//fclk = (0x40 - 0x3D[5:0]) * XVCLK / M
	//fINT CLK = fCLK / (2 x (0x11[5:0] + 1))
	OV2640WriteCmosSensor(0xFF ,0x01);
	OV2640WriteCmosSensor(0x3D, 0x38);
	OV2640WriteCmosSensor(0x11, 0x00);
	//To restore the shutter which is writened when capture.
	//OV2640_restore_shutter();

	OV2640_set_AE_mode(KAL_TRUE);
	OV2640_set_AWB_mode(KAL_TRUE);

	//OV2640SetHVMirror(sensor_config_data->SensorImageMirror,&iStartX, &iStartY);
	//OV2640NightMode(1);
	
	OV2640SetDummy(OV2640Sensor.PreviewDummyPixels, OV2640Sensor.PreviewDummyLines);

	//mDELAY(150);//Delay for apply setting.
	OV2640SENSORDB("[OV2640]OV2640Preview exit :\n ");
  	return TRUE ;//ERROR_NONE;
  	
}	/* OV2640Preview() */

UINT32 OV2640Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
#if 0
    	kal_uint32 shutter = 0, gain=0,temp_reg = 0;
  	kal_uint32 prev_line_len = 0;
  	kal_uint32 cap_line_len = 0;

	OV2640SENSORDB("[OV2640]OV2640Capture enter :\n ");
	if(OV2640Sensor.SensorMode == SENSOR_MODE_PREVIEW)
	{	
		OV2640SENSORDB("[OV2640]NormalShot\n\n ");
		shutter=OV2640ReadShutter();
		OV2640SENSORDB("[OV2640]mingji test OV2640Sensor.SensorMode == SENSOR_MODE_PREVIEW shutter===%d :\n ",shutter);
		OV2640Sensor.CaptureShutter=shutter;
  		gain=OV2640ReadSensorGain();
		OV2640Sensor.CaptureGain16=gain;
	}
	else
	{
		OV2640SENSORDB("[OV2640]BurstShot\n\n ");
		shutter=OV2640Sensor.CaptureShutter;
		gain=OV2640Sensor.CaptureGain16;
	}

	OV2640SENSORDB("[OV2640]mingji test shutter===%d :\n ",shutter);
	OV2640SENSORDB("[OV2640]mingji test OV2640Sensor.CaptureShutter=%d :\n ",OV2640Sensor.CaptureShutter);
	OV2640SENSORDB("[OV2640]mingji test OV2640ReadShutter()===%d :\n ",OV2640ReadShutter());
	OV2640SENSORDB("[OV2640]mingji test gain===%d :\n ",gain);
	OV2640SENSORDB("[OV2640]mingji test OV2640Sensor.CaptureGain16=%d :\n ",OV2640Sensor.CaptureGain16);
	OV2640SENSORDB("[OV2640]mingji test OV2640ReadSensorGain()===%d :\n ",OV2640ReadSensorGain());

	OV2640Sensor.SensorMode = SENSOR_MODE_CAPTURE;
	OV2640_set_AE_mode(KAL_FALSE);
	OV2640_set_AWB_mode(KAL_FALSE);
	mDELAY(20);
	
	OV2640FullSizeCaptureSetting();

	OV2640WriteCmosSensor(0xFF ,0x01);
	OV2640WriteCmosSensor(0x3D, 0x38);		//fCLK = 96MHz
	
	OV2640Sensor.CaptureDummyPixels = 0;
  	OV2640Sensor.CaptureDummyLines = 0;
  	OV2640SetDummy(OV2640Sensor.CaptureDummyPixels, OV2640Sensor.CaptureDummyLines);

	mDELAY(50);
	prev_line_len = OV2640_PV_PERIOD_PIXEL_NUMS + OV2640Sensor.PreviewDummyPixels;
  	cap_line_len = OV2640_FULL_PERIOD_PIXEL_NUMS + OV2640Sensor.CaptureDummyPixels;
  	shutter = (shutter * OV2640Sensor.CapturePclk) / OV2640Sensor.PreviewPclk;
  	shutter = (shutter * prev_line_len) / cap_line_len;
	#ifdef BINNING_EN/* By sensor design */
 		shutter*=2;
 	#endif
		
  	OV2640WriteShutter(shutter);

  	OV2640WriteSensorGain(gain);
	OV2640SENSORDB("[OV2640]mingji test shutter=%d :\n ",shutter);
	OV2640SENSORDB("[OV2640]mingji test gain===%d :\n ",gain);
	OV2640SENSORDB("[OV2640]OV2640Capture exit :\n ");
	mDELAY(20);
	
	OV2640SENSORDB("[OV2640]OV2640Capture exit :\n ");
	return ERROR_NONE; 

#else
    	volatile kal_uint32 shutter = 0;
	kal_uint32 extra_line = 0;
	kal_uint32 pv_gain = 0, cap_gain = 0;
	kal_uint8 temp2,temp;//for cal auto uv
	kal_uint32 prev_line_len = 0;
  	kal_uint32 cap_line_len = 0;
	
	OV2640SENSORDB("[OV2640]OV2640Capture enter :\n ");
	shutter = OV2640ReadShutter();
	extra_line = OV2640ReadExtraLine();
	pv_gain = OV2640ReadSensorGain();
		
	OV2640_set_AE_mode(KAL_FALSE);
	OV2640_set_AWB_mode(KAL_FALSE);
	mDELAY(20);

	OV2640Sensor.SensorCapState = KAL_TRUE;
				
	//OV2640WriteCmosSensor(0xFF,0x01);//Turn off AE
	//OV2640WriteCmosSensor(0x13, OV2640ReadCmosSensor(0x13)&0xfa);  

	OV2640WriteCmosSensor(0x71,0x00);//freeze blc
	OV2640Sensor.PreviewShutter =shutter;		
	OV2640Sensor.Preview_Extra_Lines = extra_line;
	OV2640Sensor.PreviewGain16 = pv_gain;

	//OV2640WriteCmosSensor(0xFF,0x00);	//Turn off AWB
	//OV2640WriteCmosSensor(0xc3,0xe7);
	
	//solve color shift for continuous snap
	
	temp2 = OV2640ReadCmosSensor(0xc9);
	temp = 2*(temp2&0x3f);
	OV2640WriteCmosSensor(0xc9,(temp2&0x1f)|0x20);//fix uv

	OV2640WriteCmosSensor(0x86,0x35);//turn off auto uv
	OV2640WriteCmosSensor(0xFF,0x00);
	OV2640WriteCmosSensor(0x7c,0x00);
	OV2640WriteCmosSensor(0x7d,(OV2640ReadCmosSensor(0x7d)|0x02));
	OV2640WriteCmosSensor(0x7c,0x03);
	OV2640WriteCmosSensor(0x7d,temp);
	OV2640WriteCmosSensor(0x7d,temp);
   
	OV2640FullSizeCaptureSetting();		// 1600x1200
	
	OV2640Sensor.IsPVmode = KAL_FALSE;

	OV2640WriteCmosSensor(0xFF ,0x01);
	OV2640WriteCmosSensor(0x3D, 0x38);		//fCLK = 96MHz  15 fps SVGA mode
		
	OV2640WriteCmosSensor(0x11,0x01);
	OV2640Sensor.CapturePclk = 480;		//48MHz, decide by Page1:0x3d
	OV2640Sensor.Capture_PCLK_Division = 1; //Must same as Page1:0x11
	
	OV2640Sensor.CaptureDummyPixels = 0;//0x400;
	OV2640Sensor.CaptureDummyPixels = 0;

	OV2640ComputerAECAGC();	//Calculate the capture gain, shutter and extra exposure line.

	if(OV2640Sensor.SensorMode == SENSOR_MODE_PREVIEW)
	{
		OV2640ComputerAECAGC();	//Calculate the capture gain, shutter and extra exposure line.
		OV2640SENSORDB("[OV2640]NormalShot\n\n ");
		OV2640CaptureShutter=OV2640Sensor.CaptureShutter;
		shutter=OV2640CaptureShutter;
		OV2640CaptureExtraLines=OV2640Sensor.Capture_Extra_Lines;
		cap_line_len=OV2640CaptureExtraLines;
  		OV2640CaptureGain16=OV2640Sensor.CaptureGain16;
		cap_gain=OV2640CaptureGain16;

	}
	else
	{
		OV2640SENSORDB("[OV2640]BurstShot\n\n ");
		shutter=OV2640CaptureShutter;
		cap_line_len=OV2640CaptureExtraLines;
		cap_gain=OV2640CaptureGain16;
	}
	
	OV2640Sensor.SensorMode = SENSOR_MODE_CAPTURE;
	
	// set dummy
	OV2640SetDummy(OV2640Sensor.CaptureDummyPixels, OV2640Sensor.CaptureDummyLines);

	OV2640WriteShutter(shutter);//OV2640Sensor.CaptureShutter
	OV2640WriteExtraLine(cap_line_len);//OV2640Sensor.Capture_Extra_Lines
	
	/*if(OV2640Sensor.CaptureGain16>62)
		OV2640WriteSensorGain(16); 
	else
		OV2640WriteSensorGain((OV2640Sensor.CaptureGain16+5)); */

	OV2640WriteCmosSensor(0x71,0x94);//unfreeze blc
	OV2640WriteSensorGain(cap_gain); //OV2640Sensor.CaptureGain16

	OV2640SENSORDB("[OV2640]OV2640Capture exit :\n ");
	return ERROR_NONE; 
#endif

}/* OV2659Capture() */


UINT32 OV2640GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
	OV2640SENSORDB("[OV2640][OV2640GetResolution enter] :\n ");
	pSensorResolution->SensorPreviewWidth= OV2640_IMAGE_SENSOR_SVGA_WIDTH - 2 * OV2640_PV_GRAB_START_X;
	pSensorResolution->SensorPreviewHeight= OV2640_IMAGE_SENSOR_SVGA_HEIGHT - 2 * OV2640_PV_GRAB_START_Y;
	
	pSensorResolution->SensorFullWidth= OV2640_IMAGE_SENSOR_UVGA_WIDTH - 2 * OV2640_FULL_GRAB_START_X;  //modify 
	pSensorResolution->SensorFullHeight= OV2640_IMAGE_SENSOR_UVGA_HEIGHT - 2 * OV2640_FULL_GRAB_START_Y;
	OV2640SENSORDB("[OV2640][OV2640GetInfo exit] :\n ");
	return ERROR_NONE;
}	/* OV2640GetResolution() */

UINT32 OV2640GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
					  MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	OV2640SENSORDB("[OV2640]OV2640GetInfo enter :\n ");
	pSensorInfo->SensorPreviewResolutionX= OV2640_IMAGE_SENSOR_SVGA_WIDTH - 2 * OV2640_PV_GRAB_START_X;//8
	pSensorInfo->SensorPreviewResolutionY= OV2640_IMAGE_SENSOR_SVGA_HEIGHT - 2 * OV2640_PV_GRAB_START_Y;//6
	pSensorInfo->SensorFullResolutionX= OV2640_IMAGE_SENSOR_UVGA_WIDTH - 2 * OV2640_FULL_GRAB_START_X;//16
	pSensorInfo->SensorFullResolutionY= OV2640_IMAGE_SENSOR_UVGA_HEIGHT - 2 * OV2640_FULL_GRAB_START_Y;//12

	pSensorInfo->SensorCameraPreviewFrameRate=30;
	pSensorInfo->SensorVideoFrameRate=30;
	pSensorInfo->SensorStillCaptureFrameRate=10;
	pSensorInfo->SensorWebCamCaptureFrameRate=15;
	pSensorInfo->SensorResetActiveHigh=FALSE;
	pSensorInfo->SensorResetDelayCount=1;
	pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_UYVY;//SENSOR_OUTPUT_FORMAT_YUYV;
	pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;	/*??? */
	pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorInterruptDelayLines = 1;
	pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_PARALLEL;

	pSensorInfo->CaptureDelayFrame = 1; // 1
	pSensorInfo->PreviewDelayFrame = 3; // 2
	pSensorInfo->VideoDelayFrame = 4; 		
	pSensorInfo->SensorMasterClockSwitch = 0; 
       pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;   		

	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		//case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount=	3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
            		pSensorInfo->SensorGrabStartX = OV2640_PV_GRAB_START_X; 
            		pSensorInfo->SensorGrabStartY = OV2640_PV_GRAB_START_Y;             
			break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		//case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount=	3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
            		pSensorInfo->SensorGrabStartX = OV2640_FULL_GRAB_START_X; 
            		pSensorInfo->SensorGrabStartY = OV2640_FULL_GRAB_START_Y;             			
			break;
		default:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount=3;
			pSensorInfo->SensorClockRisingCount=0;
			pSensorInfo->SensorClockFallingCount=2;
			pSensorInfo->SensorPixelClockCount=3;
			pSensorInfo->SensorDataLatchCount=2;
            		pSensorInfo->SensorGrabStartX = OV2640_PV_GRAB_START_X; 
            		pSensorInfo->SensorGrabStartY = OV2640_PV_GRAB_START_Y;             
			break;
	}

	memcpy(pSensorConfigData, &OV2640SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	
	OV2640SENSORDB("[OV2640]OV2640GetInfo exit :\n ");
	
	return ERROR_NONE;
}	/* OV2640GetInfo() */

UINT32 OV2640Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	OV2640SENSORDB("[OV2640]OV2640Control enter :\n ");
	CurrentScenarioId = ScenarioId;
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		//case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
		#if defined(MT6575)
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
		#endif
			OV2640Preview(pImageWindow, pSensorConfigData);
			break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		//case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
			 //MSDK_SCENARIO_ID_CAMERA_ZSD;
			OV2640Capture(pImageWindow, pSensorConfigData);
		break;
		default:
		    break; 
	}
	OV2640SENSORDB("[OV2640]OV2640Control exit :\n ");
	return TRUE;
}	/* OV2640Control() */

/* [TC] YUV sensor */	

BOOL OV2640_set_param_wb(UINT16 para)
{
	kal_uint8 temp_reg = 0;
	
	//This sensor need more time to balance AWB, 
	//we suggest higher fps or drop some frame to avoid garbage color when preview initial

	OV2640WriteCmosSensor(0xFF ,0x00);
	temp_reg = OV2640ReadCmosSensor(0xc7);

	switch (para)
	{               
	case AWB_MODE_AUTO:
		OV2640WriteCmosSensor(0xc7,temp_reg&~0x40);   // Enable AWB Auto mode
		break;
		
	case AWB_MODE_CLOUDY_DAYLIGHT: //cloudy
		OV2640WriteCmosSensor(0xc7,temp_reg|0x40);  // Manual AWB mode
		OV2640WriteCmosSensor(0xCC, 0x55);
		OV2640WriteCmosSensor(0xCD, 0x40);
		OV2640WriteCmosSensor(0xCE, 0x44);
		break;	

	case AWB_MODE_DAYLIGHT: //sunny
		OV2640WriteCmosSensor(0xc7,temp_reg|0x40);  // Manual AWB mode   
		OV2640WriteCmosSensor(0xCC, 0x4e);
		OV2640WriteCmosSensor(0xCD, 0x40);
		OV2640WriteCmosSensor(0xCE, 0x48);
		break;

	case AWB_MODE_INCANDESCENT: //office
		OV2640WriteCmosSensor(0xc7,temp_reg|0x40);  // Manual AWB mode    
		OV2640WriteCmosSensor(0xCC, 0x30);
		OV2640WriteCmosSensor(0xCD, 0x40);
		OV2640WriteCmosSensor(0xCE, 0x66);
		break;

	case AWB_MODE_TUNGSTEN:
		OV2640WriteCmosSensor(0xc7,temp_reg|0x40);  // Manual AWB mode
		OV2640WriteCmosSensor(0xCC, 0x38);
		OV2640WriteCmosSensor(0xCD, 0x40);
		OV2640WriteCmosSensor(0xCE, 0x58);
		break;

	case AWB_MODE_FLUORESCENT:
		OV2640WriteCmosSensor(0xc7,temp_reg|0x40);  // Manual AWB mode    
		OV2640WriteCmosSensor(0xCC, 0x40);
		OV2640WriteCmosSensor(0xCD, 0x40);
		OV2640WriteCmosSensor(0xCE, 0x50);
		break;

	default:
	    return FALSE;
    }

    return TRUE;
} /* OV2640_set_param_wb */

BOOL OV2640_set_param_effect(UINT16 para)
{
	OV2640WriteCmosSensor(0xFF ,0x00);

	switch (para)
	{
	    case MEFFECT_OFF:
		OV2640WriteCmosSensor(0x7c,0x00);  
		OV2640WriteCmosSensor(0x7d,0x06);
		break;

	    case MEFFECT_SEPIA:
		OV2640WriteCmosSensor(0x7c,0x00);  
		OV2640WriteCmosSensor(0x7d,0x18); 
		OV2640WriteCmosSensor(0x7c,0x05); 
		OV2640WriteCmosSensor(0x7d,0x40);  
		OV2640WriteCmosSensor(0x7d,0xA0);
		break;
	    case MEFFECT_NEGATIVE:
		OV2640WriteCmosSensor(0x7c,0x00);  
		OV2640WriteCmosSensor(0x7d,0x40);
		break;

	    case MEFFECT_SEPIAGREEN:
		OV2640WriteCmosSensor(0x7c,0x00);  
		OV2640WriteCmosSensor(0x7d,0x18); 
		OV2640WriteCmosSensor(0x7c,0x05); 
		OV2640WriteCmosSensor(0x7d,0x40);  
		OV2640WriteCmosSensor(0x7d,0x40);
		break;

	    case MEFFECT_SEPIABLUE:
		OV2640WriteCmosSensor(0x7c,0x00);  
		OV2640WriteCmosSensor(0x7d,0x18); 
		OV2640WriteCmosSensor(0x7c,0x05); 
		OV2640WriteCmosSensor(0x7d,0xa0);  
		OV2640WriteCmosSensor(0x7d,0x40);
		break;
		
	    case MEFFECT_MONO: //B&W
		OV2640WriteCmosSensor(0x7c,0x00);  
		OV2640WriteCmosSensor(0x7d,0x20);
		break;
	    default:
	        return KAL_FALSE;
	}

    return KAL_FALSE;

} /* OV2640_set_param_effect */

BOOL OV2640_set_param_banding(UINT16 para)
{
	OV2640SENSORDB("[OV2640]OV2640_set_param_banding enter :\n ");

   	kal_uint8 banding;
	kal_uint16 temp_reg = 0;
  	kal_uint32 base_shutter = 0, max_shutter_step = 0, exposure_limitation = 0;
  	kal_uint32 line_length = 0, sensor_pixel_clock = 0;
  
	if (OV2640Sensor.IsPVmode == KAL_TRUE)
	{
		//OV2640SENSORDB("MINGJI test func calling OV2640Sensor.IsPVmode == KAL_TRUE\n");
		
		line_length = OV2640_PV_PERIOD_PIXEL_NUMS + OV2640Sensor.PreviewDummyPixels;
		exposure_limitation = OV2640_PV_PERIOD_LINE_NUMS + OV2640Sensor.PreviewDummyLines;
		sensor_pixel_clock = OV2640Sensor.PreviewPclk * 100 * 1000;
	}
	else
	{
		//OV2640SENSORDB("MINGJI test func calling OV2640Sensor.IsPVmode == KAL_FALSE\n");

		line_length = OV2640_FULL_PERIOD_PIXEL_NUMS + OV2640Sensor.CaptureDummyPixels;
		exposure_limitation = OV2640_FULL_PERIOD_LINE_NUMS + OV2640Sensor.CaptureDummyLines;
		sensor_pixel_clock = OV2640Sensor.CapturePclk * 100 * 1000;
	}

	line_length = line_length * 2;		/* Multiple 2 is because one YUV422 pixels need two clock. */
	
	OV2640WriteCmosSensor(0xFF ,0x01);
	
	switch (para)
	{
		case AE_FLICKER_MODE_50HZ:
			OV2640SENSORDB("[OV2640]OV2640_set_param_banding AE_FLICKER_MODE_50HZ\n");
			/* + (line_length/2) is used fot base_shutter + 0.5 */
			//base_shutter=framerate*max exposure line/100=(plck/2/(line_len*frame_len))*max exposure line/100
			base_shutter = ((sensor_pixel_clock/(OV2640Sensor.Preview_PCLK_Division+1)/100) + (line_length/2)) / line_length;
			max_shutter_step = (exposure_limitation / base_shutter) ;// - 1;
			//OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x0c==%x\n",OV2640ReadCmosSensor(0x0c));
			//OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x4f==%x\n",OV2640ReadCmosSensor(0x4f));
			//OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x5a==%x\n",OV2640ReadCmosSensor(0x5a));

			OV2640WriteCmosSensor(0x4f, 0x6d); //base_shutter
			OV2640WriteCmosSensor(0x5a, 0x57 );//(max_shutter_step << 4) & 0xF0 //0x57 mingji 0x55
			OV2640WriteCmosSensor(0x0c, 0x3c); //enable banding and 50 Hz  //0x3c mingji  0x3e

			//OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x0c==%x\n",OV2640ReadCmosSensor(0x0c));
			//OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x4f==%x\n",OV2640ReadCmosSensor(0x4f));
			//OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x5a==%x\n",OV2640ReadCmosSensor(0x5a));
			break;

	    case AE_FLICKER_MODE_60HZ:
			 OV2640SENSORDB("[OV2640]OV2640_set_param_banding AE_FLICKER_MODE_60HZ\n");
			 /* + (line_length/2) is used fot base_shutter + 0.5 */
			 base_shutter = ((sensor_pixel_clock/(OV2640Sensor.Preview_PCLK_Division+1)/120) + (line_length/2)) / line_length;
		 	 max_shutter_step = (exposure_limitation / base_shutter) ;// - 1;
		 	 //OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x0c==%x\n",OV2640ReadCmosSensor(0x0c));
			 //OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x50==%x\n",OV2640ReadCmosSensor(0x50));
			 //OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x5a==%x\n",OV2640ReadCmosSensor(0x5a));

		 	 OV2640WriteCmosSensor(0x50, 0x5b); //base_shutter
			 OV2640WriteCmosSensor(0x5a, 0x5c); //max_shutter_step & 0x0F  //0x5c mingji 0x55
			 OV2640WriteCmosSensor(0x0c, 0x38); //enable banding and 60 Hz //0x38 mingji  0x3b

			 //OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x0c==%x\n",OV2640ReadCmosSensor(0x0c));
			 //OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x50==%x\n",OV2640ReadCmosSensor(0x50));
			 //OV2640SENSORDB("MINGJI test AE_FLICKER_MODE_50HZ 0x5a==%x\n",OV2640ReadCmosSensor(0x5a));

			 break;

	    default:
	          return FALSE;
	}

	OV2640SENSORDB("[OV2640]OV2640_set_param_banding exit :\n ");
	return KAL_TRUE;
} /* OV2640_set_param_banding */

BOOL OV2640_set_param_exposure(UINT16 para)
{
    switch (para)
    {	
    	case AE_EV_COMP_n13://AE_EV_COMP_13
		OV2640WriteCmosSensor(0xFF, 0x01);
		OV2640WriteCmosSensor(0x24, 0x20);
		OV2640WriteCmosSensor(0x25, 0x18);
		OV2640WriteCmosSensor(0x26, 0x40);
		break;
	case AE_EV_COMP_n10://AE_EV_COMP_10
		OV2640WriteCmosSensor(0xFF, 0x01);
		OV2640WriteCmosSensor(0x24, 0x28);
		OV2640WriteCmosSensor(0x25, 0x20);
		OV2640WriteCmosSensor(0x26, 0x61);
		break;
	case AE_EV_COMP_n07://AE_EV_COMP_07
		OV2640WriteCmosSensor(0xFF, 0x01);
		OV2640WriteCmosSensor(0x24, 0x30);
		OV2640WriteCmosSensor(0x25, 0x28);
		OV2640WriteCmosSensor(0x26, 0x72);
		break;
	case AE_EV_COMP_n03://AE_EV_COMP_03
		OV2640WriteCmosSensor(0xFF, 0x01);
		OV2640WriteCmosSensor(0x24, 0x38);
		OV2640WriteCmosSensor(0x25, 0x30);
		OV2640WriteCmosSensor(0x26, 0x72);
		break;
	case AE_EV_COMP_00:
		OV2640WriteCmosSensor(0xFF, 0x01);
		OV2640WriteCmosSensor(0x24, 0x42);	//0x40
		OV2640WriteCmosSensor(0x25, 0x38);
		OV2640WriteCmosSensor(0x26, 0x82);	//0x91//0x82
		break;
	case AE_EV_COMP_03://AE_EV_COMP_n03
		OV2640WriteCmosSensor(0xFF, 0x01);
		OV2640WriteCmosSensor(0x24, 0x48);
		OV2640WriteCmosSensor(0x25, 0x40);
		OV2640WriteCmosSensor(0x26, 0x82);
		break;
	case AE_EV_COMP_07://AE_EV_COMP_n07
		OV2640WriteCmosSensor(0xFF, 0x01);
		OV2640WriteCmosSensor(0x24, 0x50);
		OV2640WriteCmosSensor(0x25, 0x48);
		OV2640WriteCmosSensor(0x26, 0x83);
		break;
	case AE_EV_COMP_10://AE_EV_COMP_n10
		OV2640WriteCmosSensor(0xFF, 0x01);
		OV2640WriteCmosSensor(0x24, 0x58);
		OV2640WriteCmosSensor(0x25, 0x50);
		OV2640WriteCmosSensor(0x26, 0x84);
		break;
	case AE_EV_COMP_13://AE_EV_COMP_n13
		OV2640WriteCmosSensor(0xFF, 0x01);
		OV2640WriteCmosSensor(0x24, 0x60);
		OV2640WriteCmosSensor(0x25, 0x58);
		OV2640WriteCmosSensor(0x26, 0x94);
		break;		
	default:
		return FALSE;
    }
    return TRUE;
} /* OV2640_set_param_exposure */

UINT32 OV2640YUVSensorSetting(FEATURE_ID iCmd, UINT32 iPara)
{
	OV2640SENSORDB("OV2640YUVSensorSetting:iCmd=%d,iPara=%d, %d \n",iCmd, iPara);

	switch (iCmd) {
		case FID_SCENE_MODE:
			OV2640SENSORDB("Night Mode:%d\n", iPara); 
	    	if (iPara == SCENE_MODE_OFF)
	    	{
	       	OV2640NightMode(KAL_FALSE); 
	    	}
	    	else if (iPara == SCENE_MODE_NIGHTSCENE)
	    	{
          		OV2640NightMode(KAL_TRUE); 
	    	}	    
	    	break; 	    
		case FID_AWB_MODE:
			OV2640SENSORDB("Set AWB Mode:%d\n", iPara); 	    
        		OV2640_set_param_wb(iPara);
			break;
		case FID_COLOR_EFFECT:
	     		OV2640SENSORDB("Set Color Effect:%d\n", iPara); 	    	    
         		OV2640_set_param_effect(iPara);
		 	break;
		case FID_AE_EV:
         		OV2640SENSORDB("Set EV:%d\n", iPara); 	    	    
         		OV2640_set_param_exposure(iPara);
		 break;
		case FID_AE_FLICKER:
         		OV2640SENSORDB("Set Flicker:%d\n", iPara); 	    	    	    
         		OV2640_set_param_banding(iPara);
		 	break;
    		case FID_AE_SCENE_MODE: 
    		
    		
    		
			OV2640SENSORDB("Set AE Mode:%d\n", iPara);
			
	         	if (iPara == AE_MODE_OFF) {
			 	OV2640_AE_ENABLE = KAL_FALSE; 
	         	}
	         	else {
			 	OV2640_AE_ENABLE = KAL_TRUE; 
		     	}
		     	//solve parnoma brightness
		     	/*
	         	OV2640_set_AE_mode(OV2640_AE_ENABLE);
	         	*/
	         	break; 
		default:
			 	break;
	}
	return TRUE;
}   /* OV2640YUVSensorSetting */

UINT32 OV2640YUVSetVideoMode(UINT16 u2FrameRate)
{
    return TRUE;
}

UINT32 OV2640FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,
							 UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
{
	UINT16 *pFeatureReturnPara16=(UINT16 *) pFeaturePara;
	UINT16 *pFeatureData16=(UINT16 *) pFeaturePara;
	UINT32 *pFeatureReturnPara32=(UINT32 *) pFeaturePara;
	UINT32 *pFeatureData32=(UINT32 *) pFeaturePara;
	MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData=(MSDK_SENSOR_CONFIG_STRUCT *) pFeaturePara;
	MSDK_SENSOR_REG_INFO_STRUCT *pSensorRegData=(MSDK_SENSOR_REG_INFO_STRUCT *) pFeaturePara;
	UINT32 Tony_Temp1 = 0;
	UINT32 Tony_Temp2 = 0;
	Tony_Temp1 = pFeaturePara[0];
	Tony_Temp2 = pFeaturePara[1];
	
	//OV2640SENSORDB("[OV2640]OV2640FeatureControl enter :\n ");
	OV2640SENSORDB("[OV2640][OV2640FeatureControl]feature id=%d \n",FeatureId);
	
	switch (FeatureId)
	{
		case SENSOR_FEATURE_GET_RESOLUTION:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_GET_RESOLUTION \n");
			*pFeatureReturnPara16++=OV2640_IMAGE_SENSOR_UVGA_WIDTH;
			*pFeatureReturnPara16=OV2640_IMAGE_SENSOR_UVGA_HEIGHT;
			*pFeatureParaLen=4;
			break;
		case SENSOR_FEATURE_GET_PERIOD:
			#if defined(MT6575)
			switch(CurrentScenarioId)
			{
				case MSDK_SCENARIO_ID_CAMERA_ZSD:
					//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_GET_PERIOD \n");
					*pFeatureReturnPara16++=OV2640_FULL_PERIOD_PIXEL_NUMS + OV2640Sensor.PreviewDummyPixels;
					*pFeatureReturnPara16=OV2640_FULL_PERIOD_LINE_NUMS + OV2640Sensor.PreviewDummyLines;
					*pFeatureParaLen=4;
					break;
				default:
					//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_GET_PERIOD \n");
					*pFeatureReturnPara16++=OV2640_PV_PERIOD_PIXEL_NUMS + OV2640Sensor.PreviewDummyPixels;
					*pFeatureReturnPara16=OV2640_PV_PERIOD_LINE_NUMS + OV2640Sensor.PreviewDummyLines;
					*pFeatureParaLen=4;
					break;
			}
			#else
				//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_GET_PERIOD \n");
				*pFeatureReturnPara16++=OV2640_PV_PERIOD_PIXEL_NUMS + OV2640Sensor.PreviewDummyPixels;
				*pFeatureReturnPara16=OV2640_PV_PERIOD_LINE_NUMS + OV2640Sensor.PreviewDummyLines;
				*pFeatureParaLen=4;
			#endif
			break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			#if defined(MT6575)
			switch(CurrentScenarioId)
			{
				case MSDK_SCENARIO_ID_CAMERA_ZSD:
					//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ \n");
					*pFeatureReturnPara32 = OV2640Sensor.CapturePclk/10;
					*pFeatureParaLen=4;
					break;
				default:
					//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ \n");
					*pFeatureReturnPara32 = OV2640Sensor.PreviewPclk/10;
					*pFeatureParaLen=4;
					break;
				}
			#else
				//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ \n");
				*pFeatureReturnPara32 = OV2640Sensor.PreviewPclk/10;
				*pFeatureParaLen=4;
			#endif
			break;
		case SENSOR_FEATURE_SET_ESHUTTER:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_SET_ESHUTTER \n");
			break;
		case SENSOR_FEATURE_SET_NIGHTMODE:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_SET_NIGHTMODE \n");
			OV2640NightMode((BOOL) *pFeatureData16);
			break;
		case SENSOR_FEATURE_SET_GAIN:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_SET_GAIN \n");
			break;
		case SENSOR_FEATURE_SET_FLASHLIGHT:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_SET_FLASHLIGHT \n");
			break;
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ \n");
			break;
		case SENSOR_FEATURE_SET_REGISTER:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_SET_REGISTER \n");
			OV2640WriteCmosSensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
			break;
		case SENSOR_FEATURE_GET_REGISTER:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_GET_REGISTER \n");
			pSensorRegData->RegData = OV2640ReadCmosSensor(pSensorRegData->RegAddr);
			break;
		case SENSOR_FEATURE_GET_CONFIG_PARA:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_GET_CONFIG_PARA \n");
			memcpy(pSensorConfigData, &OV2640SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
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
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_SET/get_CCT_xxxx ect \n");
			break;
		case SENSOR_FEATURE_GET_GROUP_COUNT:
            *pFeatureReturnPara32++=0;
            *pFeatureParaLen=4;	   
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_GET_GROUP_COUNT \n");
		    	break; 
		case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_GET_LENS_DRIVER_ID \n");
			// get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
			// if EEPROM does not exist in camera module.
			*pFeatureReturnPara32=LENS_DRIVER_ID_DO_NOT_CARE;
			*pFeatureParaLen=4;
			break;
		case SENSOR_FEATURE_CHECK_SENSOR_ID:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_CHECK_SENSOR_ID \n");
			OV2640GetSensorID(pFeatureData32);
			break;
		case SENSOR_FEATURE_SET_YUV_CMD:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_SET_YUV_CMD \n");
			OV2640YUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_SET_VIDEO_MODE:
			//OV2640SENSORDB("OV2640FeatureControl:SENSOR_FEATURE_SET_VIDEO_MODE \n");
			OV2640YUVSetVideoMode(*pFeatureData16);
			break; 
			/*
		#if defined(MT6575)
		case SENSOR_FEATURE_GET_EV_AWB_REF:
			OV2640GetEvAwbRef(*pFeaturePara);
			break;
		case SENSOR_FEATURE_GET_SHUTTER_GAIN_AWB_GAIN:
			OV2640GetCurAeAwbInfo(*pFeaturePara);			
			break;
		#endif
		*/
		default:
			OV2640SENSORDB("OV2640FeatureControl:default \n");
			break;			
	}
	//OV2640SENSORDB("[OV2640]OV2640FeatureControl exit :\n ");
	return ERROR_NONE;
}	/* OV2640FeatureControl() */

SENSOR_FUNCTION_STRUCT	SensorFuncOV2640=
{
	OV2640Open,
	OV2640GetInfo,
	OV2640GetResolution,
	OV2640FeatureControl,
	OV2640Control,
	OV2640Close
};

UINT32 OV2640_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&SensorFuncOV2640;

	return ERROR_NONE;
}	/* SensorInit() */


