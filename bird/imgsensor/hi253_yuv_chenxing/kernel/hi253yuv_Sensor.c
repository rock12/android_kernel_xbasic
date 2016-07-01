/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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
 *   Sensor.c
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Image sensor driver function
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
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

#include "hi253yuv_Sensor.h"
#include "hi253yuv_Camera_Sensor_para.h"
#include "hi253yuv_CameraCustomized.h"


#define HI253YUV_DEBUG
#ifdef HI253YUV_DEBUG
#define SENSORDB printk
#else
#define SENSORDB(x,...)
#endif

UINT16 HI253_wb=0;

extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);
/*************************************************************************
* FUNCTION
*    HI253_write_cmos_sensor
*
* DESCRIPTION
*    This function wirte data to CMOS sensor through I2C
*
* PARAMETERS
*    addr: the 16bit address of register
*    para: the 8bit value of register
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void HI253_write_cmos_sensor(kal_uint8 addr, kal_uint8 para)
{
kal_uint8 out_buff[2];

    out_buff[0] = addr;
    out_buff[1] = para;

    iWriteRegI2C((u8*)out_buff , (u16)sizeof(out_buff), HI253_WRITE_ID); 

#if (defined(__HI253_DEBUG_TRACE__))
  if (sizeof(out_buff) != rt) printk("I2C write %x, %x error\n", addr, para);
#endif
}

/*************************************************************************
* FUNCTION
*    HI253_read_cmos_sensor
*
* DESCRIPTION
*    This function read data from CMOS sensor through I2C.
*
* PARAMETERS
*    addr: the 16bit address of register
*
* RETURNS
*    8bit data read through I2C
*
* LOCAL AFFECTED
*
*************************************************************************/
static kal_uint8 HI253_read_cmos_sensor(kal_uint8 addr)
{
  kal_uint8 in_buff[1] = {0xFF};
  kal_uint8 out_buff[1];
  
  out_buff[0] = addr;
   
    if (0 != iReadRegI2C((u8*)out_buff , (u16) sizeof(out_buff), (u8*)in_buff, (u16) sizeof(in_buff), HI253_WRITE_ID)) {
        SENSORDB("ERROR: HI253_read_cmos_sensor \n");
    }

#if (defined(__HI253_DEBUG_TRACE__))
  if (size != rt) printk("I2C read %x error\n", addr);
#endif

  return in_buff[0];
}


#define	HI253_LIMIT_EXPOSURE_LINES				(1253)
#define	HI253_VIDEO_NORMALMODE_30FRAME_RATE       (30)
#define	HI253_VIDEO_NORMALMODE_FRAME_RATE         (15)
#define	HI253_VIDEO_NIGHTMODE_FRAME_RATE          (7.5)
#define BANDING50_30HZ
/* Global Valuable */

static kal_uint32 zoom_factor = 0; 
static kal_uint8 HI253_exposure_line_h = 0, HI253_exposure_line_l = 0,HI253_extra_exposure_line_h = 0, HI253_extra_exposure_line_l = 0;

static kal_bool HI253_gPVmode = KAL_TRUE; //PV size or Full size
static kal_bool HI253_VEDIO_encode_mode = KAL_FALSE; //Picture(Jpeg) or Video(Mpeg4)
static kal_bool HI253_sensor_cap_state = KAL_FALSE; //Preview or Capture

static kal_uint16 HI253_dummy_pixels=0, HI253_dummy_lines=0;

static kal_uint16 HI253_exposure_lines=0, HI253_extra_exposure_lines = 0;


static kal_int8 HI253_DELAY_AFTER_PREVIEW = -1;

static kal_uint8 HI253_Banding_setting = AE_FLICKER_MODE_50HZ;  //Wonder add

/****** OVT 6-18******/
static kal_uint16 HI253_Capture_Max_Gain16= 6*16;
static kal_uint16 HI253_Capture_Gain16=0 ;    
static kal_uint16 HI253_Capture_Shutter=0;
static kal_uint16 HI253_Capture_Extra_Lines=0;

static kal_uint16  HI253_PV_Dummy_Pixels =0, HI253_Capture_Dummy_Pixels =0, HI253_Capture_Dummy_Lines =0;
static kal_uint16  HI253_PV_Gain16 = 0;
static kal_uint16  HI253_PV_Shutter = 0;
static kal_uint16  HI253_PV_Extra_Lines = 0;

kal_uint16 HI253_sensor_gain_base=0,HI253_FAC_SENSOR_REG=0,HI253_iHI253_Mode=0,HI253_max_exposure_lines=0;
kal_uint32 HI253_capture_pclk_in_M=520,HI253_preview_pclk_in_M=390,HI253_PV_dummy_pixels=0,HI253_PV_dummy_lines=0,HI253_isp_master_clock=0;

static kal_uint32  HI253_sensor_pclk=390;

kal_uint32 HI253_pv_HI253_exposure_lines = 0x0249f0,HI253_cp_HI253_exposure_lines=0;
kal_uint8 HI253_HV_Mirror;
kal_uint8 HI253_Sleep_Mode;

UINT8 HI253_PixelClockDivider=0;

//SENSOR_REG_STRUCT HI253SensorCCT[FACTORY_END_ADDR]=CAMERA_SENSOR_CCT_DEFAULT_VALUE;
//SENSOR_REG_STRUCT HI253SensorReg[ENGINEER_END]=CAMERA_SENSOR_REG_DEFAULT_VALUE;
//	camera_para.SENSOR.cct	SensorCCT	=> SensorCCT
//	camera_para.SENSOR.reg	SensorReg
MSDK_SENSOR_CONFIG_STRUCT HI253SensorConfigData;
 

void HI253_set_dummy(kal_uint16 pixels, kal_uint16 lines)
{
}    /* HI253_set_dummy */

kal_uint16 HI253_read_HI253_gain(void)
{
	return 0;
}  /* HI253_read_HI253_gain */
kal_uint16 HI253_read_shutter(void)
{
    kal_uint32 shutter;

	HI253_write_cmos_sensor(0x03, 0x20);
	shutter  = (HI253_read_cmos_sensor(0x83) << 16) |
				 (HI253_read_cmos_sensor(0x84) << 8)|
				  HI253_read_cmos_sensor(0x85);

	return shutter;
}    /* HI253_read_shutter */

void HI253_write_HI253_gain(kal_uint16 gain)
{
}  /* HI253_write_HI253_gain */

static void HI253_write_shutter(kal_uint16 shutter)
{
    HI253_write_cmos_sensor(0x03, 0x20); 
	HI253_write_cmos_sensor(0x10, 0x1c); 

	HI253_write_cmos_sensor(0x83, shutter >> 16);
	HI253_write_cmos_sensor(0x84, (shutter >> 8) & 0x000000FF);
	HI253_write_cmos_sensor(0x85, shutter & 0x000000FF);

	HI253_write_cmos_sensor(0x10, 0x9c);

}    /* HI253_write_shutter */

void HI253_set_isp_driving_current(kal_uint8 current)
{
}



/*************************************************************************
* FUNCTION
*	HI253_night_mode
*
* DESCRIPTION
*	This function night mode of HI253.
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
void HI253_night_mode(kal_bool enable)
{
			//FAE modified

			if (enable)
			{
				HI253_write_cmos_sensor(0x03, 0x10);
				HI253_write_cmos_sensor(0x41, 0x90);
				HI253_write_cmos_sensor(0x03, 0x20);
				HI253_write_cmos_sensor(0xb2, 0xf0);
				msleep(10);
			}
			else
			{
				HI253_write_cmos_sensor(0x03, 0x10);
				HI253_write_cmos_sensor(0x41, 0x00);
				HI253_write_cmos_sensor(0x03, 0x20);
				HI253_write_cmos_sensor(0xb2, 0xa0);
				msleep(10);
			}

}	/* HI253_night_mode */

/*****************************************************************************/
/* Windows Mobile Sensor Interface */
/*****************************************************************************/

/*************************************************************************
* FUNCTION
*	HI253Open
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
UINT32 HI253Open(void)
{
	volatile signed char i;
	kal_uint16 sensor_id=0;
	zoom_factor = 0; 
  HI253_wb=0;

	sensor_id = HI253_read_cmos_sensor(0x04);
	printk("[Hi253] sensor id = 0x%x\n", sensor_id);
	if (HI253_SENSOR_ID != sensor_id)
		return ERROR_SENSOR_CONNECT_FAIL;
	HI253_write_cmos_sensor(0x01, 0x79); //sleep on
	HI253_write_cmos_sensor(0x08, 0x0f); //Hi-Z on
	HI253_write_cmos_sensor(0x01, 0x78); //sleep off

	HI253_write_cmos_sensor(0x03, 0x00); // Dummy 750us START
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00); // Dummy 750us END

	HI253_write_cmos_sensor(0x0e, 0x03); //PLL On
	HI253_write_cmos_sensor(0x0e, 0x73); //PLLx2

	HI253_write_cmos_sensor(0x03, 0x00); // Dummy 750us START
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00); // Dummy 750us END

	HI253_write_cmos_sensor(0x0e, 0x00); //PLL off
	HI253_write_cmos_sensor(0x01, 0x71); //sleep on
	HI253_write_cmos_sensor(0x08, 0x00); //Hi-Z off

	HI253_write_cmos_sensor(0x01, 0x73);
	HI253_write_cmos_sensor(0x01, 0x71);

	// PAGE 20
	HI253_write_cmos_sensor(0x03, 0x20); //page 20
	HI253_write_cmos_sensor(0x10, 0x1c); //ae off

	// PAGE 22
	HI253_write_cmos_sensor(0x03, 0x22); //page 22
	HI253_write_cmos_sensor(0x10, 0x69); //awb off


	//Initial Start
	/////// PAGE 0 START ///////
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x10, 0x11); // Sub1/2_Preview2 Mode_H binning
	HI253_write_cmos_sensor(0x11, 0x90);	//90
	HI253_write_cmos_sensor(0x12, 0x04);

	HI253_write_cmos_sensor(0x0b, 0xaa); // ESD Check Register
	HI253_write_cmos_sensor(0x0c, 0xaa); // ESD Check Register
	HI253_write_cmos_sensor(0x0d, 0xaa); // ESD Check Register

	HI253_write_cmos_sensor(0x20, 0x00); // Windowing start point Y
	HI253_write_cmos_sensor(0x21, 0x04);
	HI253_write_cmos_sensor(0x22, 0x00); // Windowing start point X
	HI253_write_cmos_sensor(0x23, 0x07);

	HI253_write_cmos_sensor(0x24, 0x04);
	HI253_write_cmos_sensor(0x25, 0xb0);
	HI253_write_cmos_sensor(0x26, 0x06);
	HI253_write_cmos_sensor(0x27, 0x40); // WINROW END

	HI253_write_cmos_sensor(0x40, 0x01); //Hblank 408
	HI253_write_cmos_sensor(0x41, 0x98); 
	HI253_write_cmos_sensor(0x42, 0x00); //Vblank 20
	HI253_write_cmos_sensor(0x43, 0x82);

	HI253_write_cmos_sensor(0x45, 0x04);
	HI253_write_cmos_sensor(0x46, 0x18);
	HI253_write_cmos_sensor(0x47, 0xd8);

	//BLC
	HI253_write_cmos_sensor(0x80, 0x2e);
	HI253_write_cmos_sensor(0x81, 0x7e);
	HI253_write_cmos_sensor(0x82, 0x90);
	HI253_write_cmos_sensor(0x83, 0x00);
	HI253_write_cmos_sensor(0x84, 0x0c);
	HI253_write_cmos_sensor(0x85, 0x00);
	HI253_write_cmos_sensor(0x90, 0x0c); //BLC_TIME_TH_ON
	HI253_write_cmos_sensor(0x91, 0x0c); //BLC_TIME_TH_OFF 
	HI253_write_cmos_sensor(0x92, 0x98); //BLC_AG_TH_ON
	HI253_write_cmos_sensor(0x93, 0x90); //BLC_AG_TH_OFF
	HI253_write_cmos_sensor(0x94, 0x75);
	HI253_write_cmos_sensor(0x95, 0x70);
	HI253_write_cmos_sensor(0x96, 0xdc);
	HI253_write_cmos_sensor(0x97, 0xfe);
	HI253_write_cmos_sensor(0x98, 0x38);

	//OutDoor  BLC
	HI253_write_cmos_sensor(0x99, 0x43);
	HI253_write_cmos_sensor(0x9a, 0x43);
	HI253_write_cmos_sensor(0x9b, 0x43);
	HI253_write_cmos_sensor(0x9c, 0x43);

	//Dark BLC
	HI253_write_cmos_sensor(0xa0, 0x40);
	HI253_write_cmos_sensor(0xa2, 0x40);
	HI253_write_cmos_sensor(0xa4, 0x40);
	HI253_write_cmos_sensor(0xa6, 0x40);

	//Normal BLC
	HI253_write_cmos_sensor(0xa8, 0x43);
	HI253_write_cmos_sensor(0xaa, 0x43);
	HI253_write_cmos_sensor(0xac, 0x43);
	HI253_write_cmos_sensor(0xae, 0x43);

	HI253_write_cmos_sensor(0x03, 0x02); //Page 02
	HI253_write_cmos_sensor(0x10, 0x00); //Mode_test
	HI253_write_cmos_sensor(0x11, 0x00); //Mode_dead_test
	HI253_write_cmos_sensor(0x12, 0x03); //pwr_ctl_ctl1
	HI253_write_cmos_sensor(0x13, 0x03); //Mode_ana_test
	HI253_write_cmos_sensor(0x14, 0x00); //mode_memory
	HI253_write_cmos_sensor(0x16, 0x00); //dcdc_ctl1
	HI253_write_cmos_sensor(0x17, 0x8c); //dcdc_ctl2
	HI253_write_cmos_sensor(0x18, 0x4C); //analog_func1
	HI253_write_cmos_sensor(0x19, 0x00); //analog_func2
	HI253_write_cmos_sensor(0x1a, 0x39); //analog_func3
	HI253_write_cmos_sensor(0x1b, 0x00); //analog_func4
	HI253_write_cmos_sensor(0x1c, 0x09); //dcdc_ctl3
	HI253_write_cmos_sensor(0x1d, 0x40); //dcdc_ctl4
	HI253_write_cmos_sensor(0x1e, 0x30); //analog_func7
	HI253_write_cmos_sensor(0x1f, 0x10); //analog_func8
	HI253_write_cmos_sensor(0x20, 0x77); //pixel bias
	HI253_write_cmos_sensor(0x21, 0xde); //adc,asp bias
	HI253_write_cmos_sensor(0x22, 0xa7); //main,bus bias
	HI253_write_cmos_sensor(0x23, 0x30); //clamp
	HI253_write_cmos_sensor(0x24, 0x4a);		
	HI253_write_cmos_sensor(0x25, 0x10);		
	HI253_write_cmos_sensor(0x27, 0x3c);		
	HI253_write_cmos_sensor(0x28, 0x00);		
	HI253_write_cmos_sensor(0x29, 0x0c);		
	HI253_write_cmos_sensor(0x2a, 0x80);		
	HI253_write_cmos_sensor(0x2b, 0x80);		
	HI253_write_cmos_sensor(0x2c, 0x02);		
	HI253_write_cmos_sensor(0x2d, 0xa0);		
	HI253_write_cmos_sensor(0x2e, 0x00); // 0x11->0x00 [20110809 update]
	HI253_write_cmos_sensor(0x2f, 0x00);// 0xa1->0x00 [20110809 update]		
	HI253_write_cmos_sensor(0x30, 0x05); //swap_ctl
	HI253_write_cmos_sensor(0x31, 0x99);		
	HI253_write_cmos_sensor(0x32, 0x00);		
	HI253_write_cmos_sensor(0x33, 0x00);		
	HI253_write_cmos_sensor(0x34, 0x22);		
	HI253_write_cmos_sensor(0x38, 0x88);		
	HI253_write_cmos_sensor(0x39, 0x88);		
	HI253_write_cmos_sensor(0x50, 0x20);		
	HI253_write_cmos_sensor(0x51, 0x03);		
	HI253_write_cmos_sensor(0x52, 0x01);		
	HI253_write_cmos_sensor(0x53, 0xc1);		
	HI253_write_cmos_sensor(0x54, 0x10);		
	HI253_write_cmos_sensor(0x55, 0x1c);		
	HI253_write_cmos_sensor(0x56, 0x11);		
	HI253_write_cmos_sensor(0x58, 0x10);		
	HI253_write_cmos_sensor(0x59, 0x0e);		
	HI253_write_cmos_sensor(0x5d, 0xa2);		
	HI253_write_cmos_sensor(0x5e, 0x5a);		
	HI253_write_cmos_sensor(0x60, 0x87);		
	HI253_write_cmos_sensor(0x61, 0x99);		
	HI253_write_cmos_sensor(0x62, 0x88);		
	HI253_write_cmos_sensor(0x63, 0x97);		
	HI253_write_cmos_sensor(0x64, 0x88);		
	HI253_write_cmos_sensor(0x65, 0x97);		
	HI253_write_cmos_sensor(0x67, 0x0c);		
	HI253_write_cmos_sensor(0x68, 0x0c);		
	HI253_write_cmos_sensor(0x69, 0x0c);		
	HI253_write_cmos_sensor(0x6a, 0xb4);		
	HI253_write_cmos_sensor(0x6b, 0xc4);		
	HI253_write_cmos_sensor(0x6c, 0xb5);		
	HI253_write_cmos_sensor(0x6d, 0xc2);		
	HI253_write_cmos_sensor(0x6e, 0xb5);		
	HI253_write_cmos_sensor(0x6f, 0xc0);		
	HI253_write_cmos_sensor(0x70, 0xb6);		
	HI253_write_cmos_sensor(0x71, 0xb8);		
	HI253_write_cmos_sensor(0x72, 0x89);		
	HI253_write_cmos_sensor(0x73, 0x96);		
	HI253_write_cmos_sensor(0x74, 0x89);		
	HI253_write_cmos_sensor(0x75, 0x96);		
	HI253_write_cmos_sensor(0x76, 0x89);		
	HI253_write_cmos_sensor(0x77, 0x96);		
	HI253_write_cmos_sensor(0x7c, 0x85);		
	HI253_write_cmos_sensor(0x7d, 0xaf);		
	HI253_write_cmos_sensor(0x80, 0x01);		
	HI253_write_cmos_sensor(0x81, 0x7f);		
	HI253_write_cmos_sensor(0x82, 0x13); //rx_on1_read
	HI253_write_cmos_sensor(0x83, 0x24);		
	HI253_write_cmos_sensor(0x84, 0x7D);		
	HI253_write_cmos_sensor(0x85, 0x81);		
	HI253_write_cmos_sensor(0x86, 0x7D);		
	HI253_write_cmos_sensor(0x87, 0x81);		
	HI253_write_cmos_sensor(0x88, 0xab);		
	HI253_write_cmos_sensor(0x89, 0xbc);		
	HI253_write_cmos_sensor(0x8a, 0xac);		
	HI253_write_cmos_sensor(0x8b, 0xba);		
	HI253_write_cmos_sensor(0x8c, 0xad);		
	HI253_write_cmos_sensor(0x8d, 0xb8);		
	HI253_write_cmos_sensor(0x8e, 0xae);		
	HI253_write_cmos_sensor(0x8f, 0xb2);		
	HI253_write_cmos_sensor(0x90, 0xb3);		
	HI253_write_cmos_sensor(0x91, 0xb7);		
	HI253_write_cmos_sensor(0x92, 0x48);		
	HI253_write_cmos_sensor(0x93, 0x54);		
	HI253_write_cmos_sensor(0x94, 0x7D);		
	HI253_write_cmos_sensor(0x95, 0x81);		
	HI253_write_cmos_sensor(0x96, 0x7D);		
	HI253_write_cmos_sensor(0x97, 0x81);		
	HI253_write_cmos_sensor(0xa0, 0x02);		
	HI253_write_cmos_sensor(0xa1, 0x7B);		
	HI253_write_cmos_sensor(0xa2, 0x02);		
	HI253_write_cmos_sensor(0xa3, 0x7B);		
	HI253_write_cmos_sensor(0xa4, 0x7B);		
	HI253_write_cmos_sensor(0xa5, 0x02);		
	HI253_write_cmos_sensor(0xa6, 0x7B);		
	HI253_write_cmos_sensor(0xa7, 0x02);		
	HI253_write_cmos_sensor(0xa8, 0x85);		
	HI253_write_cmos_sensor(0xa9, 0x8C);		
	HI253_write_cmos_sensor(0xaa, 0x85);		
	HI253_write_cmos_sensor(0xab, 0x8C);		
	HI253_write_cmos_sensor(0xac, 0x10); //Rx_pwr_off1_read
	HI253_write_cmos_sensor(0xad, 0x16); //Rx_pwr_on1_read
	HI253_write_cmos_sensor(0xae, 0x10); //Rx_pwr_off2_read
	HI253_write_cmos_sensor(0xaf, 0x16); //Rx_pwr_on1_read
	HI253_write_cmos_sensor(0xb0, 0x99);		
	HI253_write_cmos_sensor(0xb1, 0xA3);		
	HI253_write_cmos_sensor(0xb2, 0xA4);		
	HI253_write_cmos_sensor(0xb3, 0xAE);		
	HI253_write_cmos_sensor(0xb4, 0x9B);		
	HI253_write_cmos_sensor(0xb5, 0xA2);		
	HI253_write_cmos_sensor(0xb6, 0xA6);		
	HI253_write_cmos_sensor(0xb7, 0xAC);		
	HI253_write_cmos_sensor(0xb8, 0x9B);		
	HI253_write_cmos_sensor(0xb9, 0x9F);		
	HI253_write_cmos_sensor(0xba, 0xA6);		
	HI253_write_cmos_sensor(0xbb, 0xAA);		
	HI253_write_cmos_sensor(0xbc, 0x9B);		
	HI253_write_cmos_sensor(0xbd, 0x9F);		
	HI253_write_cmos_sensor(0xbe, 0xA6);		
	HI253_write_cmos_sensor(0xbf, 0xaa);		
	HI253_write_cmos_sensor(0xc4, 0x2c);		
	HI253_write_cmos_sensor(0xc5, 0x43);		
	HI253_write_cmos_sensor(0xc6, 0x63);		
	HI253_write_cmos_sensor(0xc7, 0x79);		
	HI253_write_cmos_sensor(0xc8, 0x2d);		
	HI253_write_cmos_sensor(0xc9, 0x42);		
	HI253_write_cmos_sensor(0xca, 0x2d);		
	HI253_write_cmos_sensor(0xcb, 0x42);		
	HI253_write_cmos_sensor(0xcc, 0x64);		
	HI253_write_cmos_sensor(0xcd, 0x78);		
	HI253_write_cmos_sensor(0xce, 0x64);		
	HI253_write_cmos_sensor(0xcf, 0x78);		
	HI253_write_cmos_sensor(0xd0, 0x0a);		
	HI253_write_cmos_sensor(0xd1, 0x09);		
	HI253_write_cmos_sensor(0xd2, 0x20);		
	HI253_write_cmos_sensor(0xd3, 0x00);	
		
	HI253_write_cmos_sensor(0xd4, 0x0c);		
	HI253_write_cmos_sensor(0xd5, 0x0c);		
	HI253_write_cmos_sensor(0xd6, 0x98);		
	HI253_write_cmos_sensor(0xd7, 0x90);
			
	HI253_write_cmos_sensor(0xe0, 0xc4);		
	HI253_write_cmos_sensor(0xe1, 0xc4);		
	HI253_write_cmos_sensor(0xe2, 0xc4);		
	HI253_write_cmos_sensor(0xe3, 0xc4);		
	HI253_write_cmos_sensor(0xe4, 0x00);		
	HI253_write_cmos_sensor(0xe8, 0x80);		
	HI253_write_cmos_sensor(0xe9, 0x40);		
	HI253_write_cmos_sensor(0xea, 0x7f);		
	HI253_write_cmos_sensor(0xf0, 0x01); //sram1_cfg
	HI253_write_cmos_sensor(0xf1, 0x01); //sram2_cfg
	HI253_write_cmos_sensor(0xf2, 0x01); //sram3_cfg
	HI253_write_cmos_sensor(0xf3, 0x01); //sram4_cfg
	HI253_write_cmos_sensor(0xf4, 0x01); //sram5_cfg

	/////// PAGE 3 ///////
	HI253_write_cmos_sensor(0x03, 0x03);
	HI253_write_cmos_sensor(0x10, 0x10);

	/////// PAGE 10 START ///////
	HI253_write_cmos_sensor(0x03, 0x10);
	HI253_write_cmos_sensor(0x10, 0x03); // CrYCbY // For Demoset 0x03
	HI253_write_cmos_sensor(0x12, 0x30);
	HI253_write_cmos_sensor(0x13, 0x0a); // contrast on
	HI253_write_cmos_sensor(0x20, 0x00);

	HI253_write_cmos_sensor(0x30, 0x00);
	HI253_write_cmos_sensor(0x31, 0x00);
	HI253_write_cmos_sensor(0x32, 0x00);
	HI253_write_cmos_sensor(0x33, 0x00);

	HI253_write_cmos_sensor(0x34, 0x30);
	HI253_write_cmos_sensor(0x35, 0x00);
	HI253_write_cmos_sensor(0x36, 0x00);
	HI253_write_cmos_sensor(0x38, 0x00);
	HI253_write_cmos_sensor(0x3e, 0x58);
	HI253_write_cmos_sensor(0x3f, 0x00);

	HI253_write_cmos_sensor(0x40, 0x80); // YOFS
	HI253_write_cmos_sensor(0x41, 0x00); // DYOFS
	HI253_write_cmos_sensor(0x48, 0x98); // Contrast

	HI253_write_cmos_sensor(0x60, 0x67);
	HI253_write_cmos_sensor(0x61, 0x7e); //7e //8e //88 //80
	HI253_write_cmos_sensor(0x62, 0x7e); //7e //8e //88 //80
	HI253_write_cmos_sensor(0x63, 0x50); //Double_AG 50->30
	HI253_write_cmos_sensor(0x64, 0x41);

	HI253_write_cmos_sensor(0x66, 0x42);
	HI253_write_cmos_sensor(0x67, 0x20);

	HI253_write_cmos_sensor(0x6a, 0x80); //8a
	HI253_write_cmos_sensor(0x6b, 0x84); //74
	HI253_write_cmos_sensor(0x6c, 0x80); //7e //7a
	HI253_write_cmos_sensor(0x6d, 0x80); //8e

	//Don't touch//////////////////////////
	//HI253_write_cmos_sensor(0x72, 0x84);
	//HI253_write_cmos_sensor(0x76, 0x19);
	//HI253_write_cmos_sensor(0x73, 0x70);
	//HI253_write_cmos_sensor(0x74, 0x68);
	//HI253_write_cmos_sensor(0x75, 0x60); // white protection ON
	//HI253_write_cmos_sensor(0x77, 0x0e); //08 //0a
	//HI253_write_cmos_sensor(0x78, 0x2a); //20
	//HI253_write_cmos_sensor(0x79, 0x08);
	////////////////////////////////////////

	/////// PAGE 11 START ///////
	HI253_write_cmos_sensor(0x03, 0x11);
	HI253_write_cmos_sensor(0x10, 0x7f);
	HI253_write_cmos_sensor(0x11, 0x40);
	HI253_write_cmos_sensor(0x12, 0x0a); // Blue Max-Filter Delete
	HI253_write_cmos_sensor(0x13, 0xbb);

	HI253_write_cmos_sensor(0x26, 0x31); // Double_AG 31->20
	HI253_write_cmos_sensor(0x27, 0x34); // Double_AG 34->22
	HI253_write_cmos_sensor(0x28, 0x0f);
	HI253_write_cmos_sensor(0x29, 0x10);
	HI253_write_cmos_sensor(0x2b, 0x30);
	HI253_write_cmos_sensor(0x2c, 0x32);

	//Out2 D-LPF th
	HI253_write_cmos_sensor(0x30, 0x70);
	HI253_write_cmos_sensor(0x31, 0x10);
	HI253_write_cmos_sensor(0x32, 0x58);
	HI253_write_cmos_sensor(0x33, 0x09);
	HI253_write_cmos_sensor(0x34, 0x06);
	HI253_write_cmos_sensor(0x35, 0x03);

	//Out1 D-LPF th
	HI253_write_cmos_sensor(0x36, 0x70);
	HI253_write_cmos_sensor(0x37, 0x18);
	HI253_write_cmos_sensor(0x38, 0x58);
	HI253_write_cmos_sensor(0x39, 0x09);
	HI253_write_cmos_sensor(0x3a, 0x06);
	HI253_write_cmos_sensor(0x3b, 0x03);

	//Indoor D-LPF th
	HI253_write_cmos_sensor(0x3c, 0x80);
	HI253_write_cmos_sensor(0x3d, 0x18);
	HI253_write_cmos_sensor(0x3e, 0xa0); //80
	HI253_write_cmos_sensor(0x3f, 0x0c);
	HI253_write_cmos_sensor(0x40, 0x09);
	HI253_write_cmos_sensor(0x41, 0x06);

	HI253_write_cmos_sensor(0x42, 0x80);
	HI253_write_cmos_sensor(0x43, 0x18);
	HI253_write_cmos_sensor(0x44, 0xa0); //80
	HI253_write_cmos_sensor(0x45, 0x12);
	HI253_write_cmos_sensor(0x46, 0x10);
	HI253_write_cmos_sensor(0x47, 0x10);

	HI253_write_cmos_sensor(0x48, 0x90);
	HI253_write_cmos_sensor(0x49, 0x40);
	HI253_write_cmos_sensor(0x4a, 0x80);
	HI253_write_cmos_sensor(0x4b, 0x13);
	HI253_write_cmos_sensor(0x4c, 0x10);
	HI253_write_cmos_sensor(0x4d, 0x11);

	HI253_write_cmos_sensor(0x4e, 0x80);
	HI253_write_cmos_sensor(0x4f, 0x30);
	HI253_write_cmos_sensor(0x50, 0x80);
	HI253_write_cmos_sensor(0x51, 0x13);
	HI253_write_cmos_sensor(0x52, 0x10);
	HI253_write_cmos_sensor(0x53, 0x13);

	HI253_write_cmos_sensor(0x54, 0x11);
	HI253_write_cmos_sensor(0x55, 0x17);
	HI253_write_cmos_sensor(0x56, 0x20);
	HI253_write_cmos_sensor(0x57, 0x01);
	HI253_write_cmos_sensor(0x58, 0x00);
	HI253_write_cmos_sensor(0x59, 0x00);

	HI253_write_cmos_sensor(0x5a, 0x1f); //18
	HI253_write_cmos_sensor(0x5b, 0x00);
	HI253_write_cmos_sensor(0x5c, 0x00);

	HI253_write_cmos_sensor(0x60, 0x3f);
	HI253_write_cmos_sensor(0x62, 0x60);
	HI253_write_cmos_sensor(0x70, 0x06);

	/////// PAGE 12 START ///////
	HI253_write_cmos_sensor(0x03, 0x12);
	HI253_write_cmos_sensor(0x20, 0x0f);
	HI253_write_cmos_sensor(0x21, 0x0f);

	HI253_write_cmos_sensor(0x25, 0x00); //0x30

	HI253_write_cmos_sensor(0x28, 0x00);
	HI253_write_cmos_sensor(0x29, 0x00);
	HI253_write_cmos_sensor(0x2a, 0x00);

	HI253_write_cmos_sensor(0x30, 0x50);
	HI253_write_cmos_sensor(0x31, 0x18);
	HI253_write_cmos_sensor(0x32, 0x32);
	HI253_write_cmos_sensor(0x33, 0x40);
	HI253_write_cmos_sensor(0x34, 0x50);
	HI253_write_cmos_sensor(0x35, 0x70);
	HI253_write_cmos_sensor(0x36, 0xa0);

	HI253_write_cmos_sensor(0x3b, 0x06);
	HI253_write_cmos_sensor(0x3c, 0x06);


	//Out2 th
	HI253_write_cmos_sensor(0x40, 0xa0);
	HI253_write_cmos_sensor(0x41, 0x40);
	HI253_write_cmos_sensor(0x42, 0xa0);
	HI253_write_cmos_sensor(0x43, 0x90);
	HI253_write_cmos_sensor(0x44, 0x90);
	HI253_write_cmos_sensor(0x45, 0x80);

	//Out1 th
	HI253_write_cmos_sensor(0x46, 0xb0);
	HI253_write_cmos_sensor(0x47, 0x55);
	HI253_write_cmos_sensor(0x48, 0xa0);
	HI253_write_cmos_sensor(0x49, 0x90);
	HI253_write_cmos_sensor(0x4a, 0x90);
	HI253_write_cmos_sensor(0x4b, 0x80);

	//Indoor th
	HI253_write_cmos_sensor(0x4c, 0xb0);
	HI253_write_cmos_sensor(0x4d, 0x40);
	HI253_write_cmos_sensor(0x4e, 0x90);
	HI253_write_cmos_sensor(0x4f, 0x90);
	HI253_write_cmos_sensor(0x50, 0xa0);
	HI253_write_cmos_sensor(0x51, 0x80);

	//Dark1 th
	HI253_write_cmos_sensor(0x52, 0xb0);
	HI253_write_cmos_sensor(0x53, 0x60);
	HI253_write_cmos_sensor(0x54, 0xc0);
	HI253_write_cmos_sensor(0x55, 0xc0);
	HI253_write_cmos_sensor(0x56, 0xc0);
	HI253_write_cmos_sensor(0x57, 0x80);

	//Dark2 th
	HI253_write_cmos_sensor(0x58, 0x90);
	HI253_write_cmos_sensor(0x59, 0x40);
	HI253_write_cmos_sensor(0x5a, 0xd0);
	HI253_write_cmos_sensor(0x5b, 0xd0);
	HI253_write_cmos_sensor(0x5c, 0xe0);
	HI253_write_cmos_sensor(0x5d, 0x80);

	//Dark3 th
	HI253_write_cmos_sensor(0x5e, 0x88);
	HI253_write_cmos_sensor(0x5f, 0x40);
	HI253_write_cmos_sensor(0x60, 0xe0);
	HI253_write_cmos_sensor(0x61, 0xe0);
	HI253_write_cmos_sensor(0x62, 0xe0);
	HI253_write_cmos_sensor(0x63, 0x80);

	HI253_write_cmos_sensor(0x70, 0x15);
	HI253_write_cmos_sensor(0x71, 0x01); //Don't Touch register

	HI253_write_cmos_sensor(0x72, 0x18);
	HI253_write_cmos_sensor(0x73, 0x01); //Don't Touch register

	HI253_write_cmos_sensor(0x74, 0x25);
	HI253_write_cmos_sensor(0x75, 0x15);


	HI253_write_cmos_sensor(0x90, 0x5d); //DPC
	HI253_write_cmos_sensor(0x91, 0x88);		
	HI253_write_cmos_sensor(0x98, 0x7d);		
	HI253_write_cmos_sensor(0x99, 0x28);		
	HI253_write_cmos_sensor(0x9A, 0x14);		
	HI253_write_cmos_sensor(0x9B, 0xc8);		
	HI253_write_cmos_sensor(0x9C, 0x02);		
	HI253_write_cmos_sensor(0x9D, 0x1e);		
	HI253_write_cmos_sensor(0x9E, 0x28);		
	HI253_write_cmos_sensor(0x9F, 0x07);		
	HI253_write_cmos_sensor(0xA0, 0x32);		
	HI253_write_cmos_sensor(0xA4, 0x04);		
	HI253_write_cmos_sensor(0xA5, 0x0e);		
	HI253_write_cmos_sensor(0xA6, 0x0c);		
	HI253_write_cmos_sensor(0xA7, 0x04);		
	HI253_write_cmos_sensor(0xA8, 0x3c);		

	HI253_write_cmos_sensor(0xAA, 0x14);		
	HI253_write_cmos_sensor(0xAB, 0x11);		
	HI253_write_cmos_sensor(0xAC, 0x0f);		
	HI253_write_cmos_sensor(0xAD, 0x16);		
	HI253_write_cmos_sensor(0xAE, 0x15);		
	HI253_write_cmos_sensor(0xAF, 0x14);		

	HI253_write_cmos_sensor(0xB1, 0xaa);		
	HI253_write_cmos_sensor(0xB2, 0x96);		
	HI253_write_cmos_sensor(0xB3, 0x28);		
	//HI253_write_cmos_sensor(0xB6,read); only//dpc_flat_thres
	//HI253_write_cmos_sensor(0xB7,read); only//dpc_grad_cnt
	HI253_write_cmos_sensor(0xB8, 0x78);		
	HI253_write_cmos_sensor(0xB9, 0xa0);		
	HI253_write_cmos_sensor(0xBA, 0xb4);		
	HI253_write_cmos_sensor(0xBB, 0x14);		
	HI253_write_cmos_sensor(0xBC, 0x14);		
	HI253_write_cmos_sensor(0xBD, 0x14);		
	HI253_write_cmos_sensor(0xBE, 0x64);		
	HI253_write_cmos_sensor(0xBF, 0x64);		
	HI253_write_cmos_sensor(0xC0, 0x64);		
	HI253_write_cmos_sensor(0xC1, 0x64);		
	HI253_write_cmos_sensor(0xC2, 0x04);		
	HI253_write_cmos_sensor(0xC3, 0x03);		
	HI253_write_cmos_sensor(0xC4, 0x0c);		
	HI253_write_cmos_sensor(0xC5, 0x30);		
	HI253_write_cmos_sensor(0xC6, 0x2a);		
	HI253_write_cmos_sensor(0xD0, 0x0c); //CI Option/CI DPC
	HI253_write_cmos_sensor(0xD1, 0x80);		
	HI253_write_cmos_sensor(0xD2, 0x67);		
	HI253_write_cmos_sensor(0xD3, 0x00);		
	HI253_write_cmos_sensor(0xD4, 0x00);		
	HI253_write_cmos_sensor(0xD5, 0x02);		
	HI253_write_cmos_sensor(0xD6, 0xff);		
	HI253_write_cmos_sensor(0xD7, 0x18);	

	/////// PAGE 13 START ///////
	HI253_write_cmos_sensor(0x03, 0x13);
	//Edge
	HI253_write_cmos_sensor(0x10, 0xcb);
	HI253_write_cmos_sensor(0x11, 0x7b);
	HI253_write_cmos_sensor(0x12, 0x07);
	HI253_write_cmos_sensor(0x14, 0x00);

	HI253_write_cmos_sensor(0x20, 0x15);
	HI253_write_cmos_sensor(0x21, 0x13);
	HI253_write_cmos_sensor(0x22, 0x33);
	HI253_write_cmos_sensor(0x23, 0x05);
	HI253_write_cmos_sensor(0x24, 0x09);

	HI253_write_cmos_sensor(0x25, 0x0a);

	HI253_write_cmos_sensor(0x26, 0x18);
	HI253_write_cmos_sensor(0x27, 0x30);
	HI253_write_cmos_sensor(0x29, 0x12);
	HI253_write_cmos_sensor(0x2a, 0x50);

	//Low clip th
	HI253_write_cmos_sensor(0x2b, 0x00); //Out2 02
	HI253_write_cmos_sensor(0x2c, 0x00); //Out1 02 //01
	HI253_write_cmos_sensor(0x25, 0x06);
	HI253_write_cmos_sensor(0x2d, 0x0c);
	HI253_write_cmos_sensor(0x2e, 0x12);
	HI253_write_cmos_sensor(0x2f, 0x12);

	//Out2 Edge
	HI253_write_cmos_sensor(0x50, 0x18); //0x10 //0x16
	HI253_write_cmos_sensor(0x51, 0x1c); //0x14 //0x1a
	HI253_write_cmos_sensor(0x52, 0x1a); //0x12 //0x18
	HI253_write_cmos_sensor(0x53, 0x14); //0x0c //0x12
	HI253_write_cmos_sensor(0x54, 0x17); //0x0f //0x15
	HI253_write_cmos_sensor(0x55, 0x14); //0x0c //0x12

	//Out1 Edge          //Edge
	HI253_write_cmos_sensor(0x56, 0x18); //0x10 //0x16
	HI253_write_cmos_sensor(0x57, 0x1c); //0x13 //0x1a
	HI253_write_cmos_sensor(0x58, 0x1a); //0x12 //0x18
	HI253_write_cmos_sensor(0x59, 0x14); //0x0c //0x12
	HI253_write_cmos_sensor(0x5a, 0x17); //0x0f //0x15
	HI253_write_cmos_sensor(0x5b, 0x14); //0x0c //0x12

	//Indoor Edge
	HI253_write_cmos_sensor(0x5c, 0x0a);
	HI253_write_cmos_sensor(0x5d, 0x0b);
	HI253_write_cmos_sensor(0x5e, 0x0a);
	HI253_write_cmos_sensor(0x5f, 0x08);
	HI253_write_cmos_sensor(0x60, 0x09);
	HI253_write_cmos_sensor(0x61, 0x08);

	//Dark1 Edge
	HI253_write_cmos_sensor(0x62, 0x08);
	HI253_write_cmos_sensor(0x63, 0x08);
	HI253_write_cmos_sensor(0x64, 0x08);
	HI253_write_cmos_sensor(0x65, 0x06);
	HI253_write_cmos_sensor(0x66, 0x06);
	HI253_write_cmos_sensor(0x67, 0x06);

	//Dark2 Edge
	HI253_write_cmos_sensor(0x68, 0x07);
	HI253_write_cmos_sensor(0x69, 0x07);
	HI253_write_cmos_sensor(0x6a, 0x07);
	HI253_write_cmos_sensor(0x6b, 0x05);
	HI253_write_cmos_sensor(0x6c, 0x05);
	HI253_write_cmos_sensor(0x6d, 0x05);

	//Dark3 Edge
	HI253_write_cmos_sensor(0x6e, 0x07);
	HI253_write_cmos_sensor(0x6f, 0x07);
	HI253_write_cmos_sensor(0x70, 0x07);
	HI253_write_cmos_sensor(0x71, 0x05);
	HI253_write_cmos_sensor(0x72, 0x05);
	HI253_write_cmos_sensor(0x73, 0x05);

	//2DY
	HI253_write_cmos_sensor(0x80, 0xfd);
	HI253_write_cmos_sensor(0x81, 0x1f);
	HI253_write_cmos_sensor(0x82, 0x05);
	HI253_write_cmos_sensor(0x83, 0x31);

	HI253_write_cmos_sensor(0x90, 0x05);
	HI253_write_cmos_sensor(0x91, 0x05);
	HI253_write_cmos_sensor(0x92, 0x33);
	HI253_write_cmos_sensor(0x93, 0x30);
	HI253_write_cmos_sensor(0x94, 0x03);
	HI253_write_cmos_sensor(0x95, 0x14);
	HI253_write_cmos_sensor(0x97, 0x20);
	HI253_write_cmos_sensor(0x99, 0x20);

	HI253_write_cmos_sensor(0xa0, 0x01);
	HI253_write_cmos_sensor(0xa1, 0x02);
	HI253_write_cmos_sensor(0xa2, 0x01);
	HI253_write_cmos_sensor(0xa3, 0x02);
	HI253_write_cmos_sensor(0xa4, 0x05);
	HI253_write_cmos_sensor(0xa5, 0x05);
	HI253_write_cmos_sensor(0xa6, 0x07);
	HI253_write_cmos_sensor(0xa7, 0x08);
	HI253_write_cmos_sensor(0xa8, 0x07);
	HI253_write_cmos_sensor(0xa9, 0x08);
	HI253_write_cmos_sensor(0xaa, 0x07);
	HI253_write_cmos_sensor(0xab, 0x08);

	//Out2 
	HI253_write_cmos_sensor(0xb0, 0x22);
	HI253_write_cmos_sensor(0xb1, 0x2a);
	HI253_write_cmos_sensor(0xb2, 0x28);
	HI253_write_cmos_sensor(0xb3, 0x22);
	HI253_write_cmos_sensor(0xb4, 0x2a);
	HI253_write_cmos_sensor(0xb5, 0x28);

	//Out1 
	HI253_write_cmos_sensor(0xb6, 0x22);
	HI253_write_cmos_sensor(0xb7, 0x2a);
	HI253_write_cmos_sensor(0xb8, 0x28);
	HI253_write_cmos_sensor(0xb9, 0x22);
	HI253_write_cmos_sensor(0xba, 0x2a);
	HI253_write_cmos_sensor(0xbb, 0x28);

	//Indoor 
	HI253_write_cmos_sensor(0xbc, 0x25);
	HI253_write_cmos_sensor(0xbd, 0x2a);
	HI253_write_cmos_sensor(0xbe, 0x27);
	HI253_write_cmos_sensor(0xbf, 0x25);
	HI253_write_cmos_sensor(0xc0, 0x2a);
	HI253_write_cmos_sensor(0xc1, 0x27);

	//Dark1
	HI253_write_cmos_sensor(0xc2, 0x1e);
	HI253_write_cmos_sensor(0xc3, 0x24);
	HI253_write_cmos_sensor(0xc4, 0x20);
	HI253_write_cmos_sensor(0xc5, 0x1e);
	HI253_write_cmos_sensor(0xc6, 0x24);
	HI253_write_cmos_sensor(0xc7, 0x20);

	//Dark2
	HI253_write_cmos_sensor(0xc8, 0x18);
	HI253_write_cmos_sensor(0xc9, 0x20);
	HI253_write_cmos_sensor(0xca, 0x1e);
	HI253_write_cmos_sensor(0xcb, 0x18);
	HI253_write_cmos_sensor(0xcc, 0x20);
	HI253_write_cmos_sensor(0xcd, 0x1e);

	//Dark3 
	HI253_write_cmos_sensor(0xce, 0x18);
	HI253_write_cmos_sensor(0xcf, 0x20);
	HI253_write_cmos_sensor(0xd0, 0x1e);
	HI253_write_cmos_sensor(0xd1, 0x18);
	HI253_write_cmos_sensor(0xd2, 0x20);
	HI253_write_cmos_sensor(0xd3, 0x1e);

	/////// PAGE 14 START ///////
	HI253_write_cmos_sensor(0x03, 0x14);
	HI253_write_cmos_sensor(0x10, 0x11);

	HI253_write_cmos_sensor(0x14, 0x80); // GX
	HI253_write_cmos_sensor(0x15, 0x80); // GY
	HI253_write_cmos_sensor(0x16, 0x80); // RX
	HI253_write_cmos_sensor(0x17, 0x80); // RY
	HI253_write_cmos_sensor(0x18, 0x80); // BX
	HI253_write_cmos_sensor(0x19, 0x80); // BY

	HI253_write_cmos_sensor(0x20, 0x60); //X 60 //a0
	HI253_write_cmos_sensor(0x21, 0x80); //Y

	HI253_write_cmos_sensor(0x22, 0x90); 
	HI253_write_cmos_sensor(0x23, 0x76);
	HI253_write_cmos_sensor(0x24, 0x76);

	HI253_write_cmos_sensor(0x30, 0xc8);
	HI253_write_cmos_sensor(0x31, 0x2b);
	HI253_write_cmos_sensor(0x32, 0x00);
	HI253_write_cmos_sensor(0x33, 0x00);
	HI253_write_cmos_sensor(0x34, 0x90);

	HI253_write_cmos_sensor(0x40, 0x48); //31
	HI253_write_cmos_sensor(0x50, 0x34); //23 //32
	HI253_write_cmos_sensor(0x60, 0x29); //1a //27
	HI253_write_cmos_sensor(0x70, 0x34); //23 //32

	/////// PAGE 15 START ///////

	HI253_write_cmos_sensor(0x03, 0x15); // Page 15 
	HI253_write_cmos_sensor(0x10, 0x0f); // CMCCTL
	HI253_write_cmos_sensor(0x14, 0x3c); // CMCOFSGH
	HI253_write_cmos_sensor(0x15, 0x38); // CMCOFSGM
	HI253_write_cmos_sensor(0x16, 0x28); // CMCOFSGL
	HI253_write_cmos_sensor(0x17, 0x2f); // CMCSIGN

	//CMC
	HI253_write_cmos_sensor(0x30, 0x84); // CMC11
	HI253_write_cmos_sensor(0x31, 0x55); // CMC12
	HI253_write_cmos_sensor(0x32, 0x11); // CMC13
	HI253_write_cmos_sensor(0x33, 0x23); // CMC21
	HI253_write_cmos_sensor(0x34, 0x68); // CMC22
	HI253_write_cmos_sensor(0x35, 0x05); // CMC23
	HI253_write_cmos_sensor(0x36, 0x03); // CMC31
	HI253_write_cmos_sensor(0x37, 0x57); // CMC32
	HI253_write_cmos_sensor(0x38, 0x9a); // CMC33

	//CMC OFS
	HI253_write_cmos_sensor(0x40, 0x8e); // CMCOFSL11
	HI253_write_cmos_sensor(0x41, 0x14); // CMCOFSL12
	HI253_write_cmos_sensor(0x42, 0x06); // CMCOFSL13
	HI253_write_cmos_sensor(0x43, 0x09); // CMCOFSL21
	HI253_write_cmos_sensor(0x44, 0x11); // CMCOFSL22
	HI253_write_cmos_sensor(0x45, 0x98); // CMCOFSL23
	HI253_write_cmos_sensor(0x46, 0x8c); // CMCOFSL31
	HI253_write_cmos_sensor(0x47, 0x84); // CMCOFSL32
	HI253_write_cmos_sensor(0x48, 0x11); // CMCOFSL33

	//CMC POFS
	HI253_write_cmos_sensor(0x50, 0x90); // CMCOFSH11
	HI253_write_cmos_sensor(0x51, 0x54); // CMCOFSH12
	HI253_write_cmos_sensor(0x52, 0xc4); // CMCOFSH13
	HI253_write_cmos_sensor(0x53, 0x34); // CMCOFSH21
	HI253_write_cmos_sensor(0x54, 0x10); // CMCOFSH22
	HI253_write_cmos_sensor(0x55, 0xc4); // CMCOFSH23
	HI253_write_cmos_sensor(0x56, 0x00); // CMCOFSH31
	HI253_write_cmos_sensor(0x57, 0x00); // CMCOFSH32
	HI253_write_cmos_sensor(0x58, 0x00); // CMCOFSH33

	HI253_write_cmos_sensor(0x80, 0x03);
	HI253_write_cmos_sensor(0x85, 0x40);
	HI253_write_cmos_sensor(0x87, 0x02);
	HI253_write_cmos_sensor(0x88, 0x00);
	HI253_write_cmos_sensor(0x89, 0x00);
	HI253_write_cmos_sensor(0x8a, 0x00);

	/////// PAGE 16 START ///////
	HI253_write_cmos_sensor(0x03, 0x16);
	HI253_write_cmos_sensor(0x10, 0x31);
	HI253_write_cmos_sensor(0x18, 0x5e);// Double_AG 5e->37
	HI253_write_cmos_sensor(0x19, 0x5d);// Double_AG 5e->36
	HI253_write_cmos_sensor(0x1a, 0x0e);
	HI253_write_cmos_sensor(0x1b, 0x01);
	HI253_write_cmos_sensor(0x1c, 0xdc);
	HI253_write_cmos_sensor(0x1d, 0xfe);

	//GMA Default
	HI253_write_cmos_sensor(0x30, 0x00);
	HI253_write_cmos_sensor(0x31, 0x0a);
	HI253_write_cmos_sensor(0x32, 0x1f);
	HI253_write_cmos_sensor(0x33, 0x33);
	HI253_write_cmos_sensor(0x34, 0x53);
	HI253_write_cmos_sensor(0x35, 0x6c);
	HI253_write_cmos_sensor(0x36, 0x81);
	HI253_write_cmos_sensor(0x37, 0x94);
	HI253_write_cmos_sensor(0x38, 0xa4);
	HI253_write_cmos_sensor(0x39, 0xb3);
	HI253_write_cmos_sensor(0x3a, 0xc0);
	HI253_write_cmos_sensor(0x3b, 0xcb);
	HI253_write_cmos_sensor(0x3c, 0xd5);
	HI253_write_cmos_sensor(0x3d, 0xde);
	HI253_write_cmos_sensor(0x3e, 0xe6);
	HI253_write_cmos_sensor(0x3f, 0xee);
	HI253_write_cmos_sensor(0x40, 0xf5);
	HI253_write_cmos_sensor(0x41, 0xfc);
	HI253_write_cmos_sensor(0x42, 0xff);
	//RGMA
	HI253_write_cmos_sensor(0x50, 0x00);
	HI253_write_cmos_sensor(0x51, 0x09);
	HI253_write_cmos_sensor(0x52, 0x1f);
	HI253_write_cmos_sensor(0x53, 0x37);
	HI253_write_cmos_sensor(0x54, 0x5b);
	HI253_write_cmos_sensor(0x55, 0x76);
	HI253_write_cmos_sensor(0x56, 0x8d);
	HI253_write_cmos_sensor(0x57, 0xa1);
	HI253_write_cmos_sensor(0x58, 0xb2);
	HI253_write_cmos_sensor(0x59, 0xbe);
	HI253_write_cmos_sensor(0x5a, 0xc9);
	HI253_write_cmos_sensor(0x5b, 0xd2);
	HI253_write_cmos_sensor(0x5c, 0xdb);
	HI253_write_cmos_sensor(0x5d, 0xe3);
	HI253_write_cmos_sensor(0x5e, 0xeb);
	HI253_write_cmos_sensor(0x5f, 0xf0);
	HI253_write_cmos_sensor(0x60, 0xf5);
	HI253_write_cmos_sensor(0x61, 0xf7);
	HI253_write_cmos_sensor(0x62, 0xf8);

	HI253_write_cmos_sensor(0x70, 0x00);
	HI253_write_cmos_sensor(0x71, 0x08);
	HI253_write_cmos_sensor(0x72, 0x17);
	HI253_write_cmos_sensor(0x73, 0x2f);
	HI253_write_cmos_sensor(0x74, 0x53);
	HI253_write_cmos_sensor(0x75, 0x6c);
	HI253_write_cmos_sensor(0x76, 0x81);
	HI253_write_cmos_sensor(0x77, 0x94);
	HI253_write_cmos_sensor(0x78, 0xa4);
	HI253_write_cmos_sensor(0x79, 0xb3);
	HI253_write_cmos_sensor(0x7a, 0xc0);
	HI253_write_cmos_sensor(0x7b, 0xcb);
	HI253_write_cmos_sensor(0x7c, 0xd5);
	HI253_write_cmos_sensor(0x7d, 0xde);
	HI253_write_cmos_sensor(0x7e, 0xe6);
	HI253_write_cmos_sensor(0x7f, 0xee);
	HI253_write_cmos_sensor(0x80, 0xf4);
	HI253_write_cmos_sensor(0x81, 0xfa);
	HI253_write_cmos_sensor(0x82, 0xff);

	/////// PAGE 17 START ///////
	HI253_write_cmos_sensor(0x03, 0x17);
	HI253_write_cmos_sensor(0x10, 0xf7);

	/////// PAGE 20 START ///////
	HI253_write_cmos_sensor(0x03, 0x20);
	HI253_write_cmos_sensor(0x11, 0x1c);
	HI253_write_cmos_sensor(0x18, 0x30);
	HI253_write_cmos_sensor(0x1a, 0x08);
	HI253_write_cmos_sensor(0x20, 0x01); //05_lowtemp Y Mean off
	HI253_write_cmos_sensor(0x21, 0x30);
	HI253_write_cmos_sensor(0x22, 0x10);
	HI253_write_cmos_sensor(0x23, 0x00);
	HI253_write_cmos_sensor(0x24, 0x00); //Uniform Scene Off

	HI253_write_cmos_sensor(0x28, 0xe7);
	HI253_write_cmos_sensor(0x29, 0x0d); //20100305 ad->0d
	HI253_write_cmos_sensor(0x2a, 0xff);
	HI253_write_cmos_sensor(0x2b, 0x04); //f4->Adaptive off

	HI253_write_cmos_sensor(0x2c, 0xc2);
	HI253_write_cmos_sensor(0x2d, 0xcf);  //fe->AE Speed option
	HI253_write_cmos_sensor(0x2e, 0x33);
	HI253_write_cmos_sensor(0x30, 0x78); //f8
	HI253_write_cmos_sensor(0x32, 0x03);
	HI253_write_cmos_sensor(0x33, 0x2e);
	HI253_write_cmos_sensor(0x34, 0x30);
	HI253_write_cmos_sensor(0x35, 0xd4);
	HI253_write_cmos_sensor(0x36, 0xfe);
	HI253_write_cmos_sensor(0x37, 0x32);
	HI253_write_cmos_sensor(0x38, 0x04);

	HI253_write_cmos_sensor(0x39, 0x22); //AE_escapeC10
	HI253_write_cmos_sensor(0x3a, 0xde); //AE_escapeC11

	HI253_write_cmos_sensor(0x3b, 0x22); //AE_escapeC1
	HI253_write_cmos_sensor(0x3c, 0xde); //AE_escapeC2

	HI253_write_cmos_sensor(0x50, 0x45);
	HI253_write_cmos_sensor(0x51, 0x88);

	HI253_write_cmos_sensor(0x56, 0x03);
	HI253_write_cmos_sensor(0x57, 0xf7);
	HI253_write_cmos_sensor(0x58, 0x14);
	HI253_write_cmos_sensor(0x59, 0x88);
	HI253_write_cmos_sensor(0x5a, 0x04);

	//New Weight For Samsung
	//HI253_write_cmos_sensor(0x60, 0xff);
	//HI253_write_cmos_sensor(0x61, 0xff);
	//HI253_write_cmos_sensor(0x62, 0xea);
	//HI253_write_cmos_sensor(0x63, 0xab);
	//HI253_write_cmos_sensor(0x64, 0xea);
	//HI253_write_cmos_sensor(0x65, 0xab);
	//HI253_write_cmos_sensor(0x66, 0xeb);
	//HI253_write_cmos_sensor(0x67, 0xeb);
	//HI253_write_cmos_sensor(0x68, 0xeb);
	//HI253_write_cmos_sensor(0x69, 0xeb);
	//HI253_write_cmos_sensor(0x6a, 0xea);
	//HI253_write_cmos_sensor(0x6b, 0xab);
	//HI253_write_cmos_sensor(0x6c, 0xea);
	//HI253_write_cmos_sensor(0x6d, 0xab);
	//HI253_write_cmos_sensor(0x6e, 0xff);
	//HI253_write_cmos_sensor(0x6f, 0xff);

	HI253_write_cmos_sensor(0x60, 0x55); // AEWGT1
	HI253_write_cmos_sensor(0x61, 0x55); // AEWGT2
	HI253_write_cmos_sensor(0x62, 0x6a); // AEWGT3
	HI253_write_cmos_sensor(0x63, 0xa9); // AEWGT4
	HI253_write_cmos_sensor(0x64, 0x6a); // AEWGT5
	HI253_write_cmos_sensor(0x65, 0xa9); // AEWGT6
	HI253_write_cmos_sensor(0x66, 0x6a); // AEWGT7
	HI253_write_cmos_sensor(0x67, 0xa9); // AEWGT8
	HI253_write_cmos_sensor(0x68, 0x6b); // AEWGT9
	HI253_write_cmos_sensor(0x69, 0xe9); // AEWGT10
	HI253_write_cmos_sensor(0x6a, 0x6a); // AEWGT11
	HI253_write_cmos_sensor(0x6b, 0xa9); // AEWGT12
	HI253_write_cmos_sensor(0x6c, 0x6a); // AEWGT13
	HI253_write_cmos_sensor(0x6d, 0xa9); // AEWGT14
	HI253_write_cmos_sensor(0x6e, 0x55); // AEWGT15
	HI253_write_cmos_sensor(0x6f, 0x55); // AEWGT16

	HI253_write_cmos_sensor(0x70, 0x70); //6e
	HI253_write_cmos_sensor(0x71, 0x89); //00 //-4

	// haunting control
	HI253_write_cmos_sensor(0x76, 0x43);
	HI253_write_cmos_sensor(0x77, 0xe2); //04 //f2

	HI253_write_cmos_sensor(0x78, 0x23); //Yth1
	HI253_write_cmos_sensor(0x79, 0x42); //Yth2 //46
	HI253_write_cmos_sensor(0x7a, 0x23); //23
	HI253_write_cmos_sensor(0x7b, 0x22); //22
	HI253_write_cmos_sensor(0x7d, 0x23);

	HI253_write_cmos_sensor(0x83, 0x01); //EXP Normal 33.33 fps 
	HI253_write_cmos_sensor(0x84, 0x7a); 
	HI253_write_cmos_sensor(0x85, 0x00); 

	HI253_write_cmos_sensor(0x86, 0x02); //EXPMin 5859.38 fps
	HI253_write_cmos_sensor(0x87, 0x00); 

	HI253_write_cmos_sensor(0x88, 0x05); //EXP Max 8.00 fps 
	HI253_write_cmos_sensor(0x89, 0xe8); 
	HI253_write_cmos_sensor(0x8a, 0x00); 

	HI253_write_cmos_sensor(0x8B, 0x7e); //EXP100 
	HI253_write_cmos_sensor(0x8C, 0x00); 
	HI253_write_cmos_sensor(0x8D, 0x69); //EXP120 
	HI253_write_cmos_sensor(0x8E, 0x00); 

	HI253_write_cmos_sensor(0x9c, 0x18); //EXP Limit 488.28 fps 
	HI253_write_cmos_sensor(0x9d, 0x00); 
	HI253_write_cmos_sensor(0x9e, 0x02); //EXP Unit 
	HI253_write_cmos_sensor(0x9f, 0x00); 

	//AE_Middle Time option
	//HI253_write_cmos_sensor(0xa0, 0x03);
	//HI253_write_cmos_sensor(0xa1, 0xa9);
	//HI253_write_cmos_sensor(0xa2, 0x80);

	HI253_write_cmos_sensor(0xb0, 0x18);
	HI253_write_cmos_sensor(0xb1, 0x14); //ADC 400->560
	HI253_write_cmos_sensor(0xb2, 0xa0); 
	HI253_write_cmos_sensor(0xb3, 0x18);
	HI253_write_cmos_sensor(0xb4, 0x1a);
	HI253_write_cmos_sensor(0xb5, 0x44);
	HI253_write_cmos_sensor(0xb6, 0x2f);
	HI253_write_cmos_sensor(0xb7, 0x28);
	HI253_write_cmos_sensor(0xb8, 0x25);
	HI253_write_cmos_sensor(0xb9, 0x22);
	HI253_write_cmos_sensor(0xba, 0x21);
	HI253_write_cmos_sensor(0xbb, 0x20);
	HI253_write_cmos_sensor(0xbc, 0x1f);
	HI253_write_cmos_sensor(0xbd, 0x1f);

	HI253_write_cmos_sensor(0xc0, 0x14);
	HI253_write_cmos_sensor(0xc1, 0x1f);
	HI253_write_cmos_sensor(0xc2, 0x1f);
	HI253_write_cmos_sensor(0xc3, 0x18); //2b
	HI253_write_cmos_sensor(0xc4, 0x10); //08

	HI253_write_cmos_sensor(0xc8, 0x80);
	HI253_write_cmos_sensor(0xc9, 0x40);

	/////// PAGE 22 START ///////
	HI253_write_cmos_sensor(0x03, 0x22);
	HI253_write_cmos_sensor(0x10, 0xfd);
	HI253_write_cmos_sensor(0x11, 0x2e);
	HI253_write_cmos_sensor(0x19, 0x01); // Low On //
	HI253_write_cmos_sensor(0x20, 0x30);
	HI253_write_cmos_sensor(0x21, 0x80);
	HI253_write_cmos_sensor(0x24, 0x01);
	//HI253_write_cmos_sensor(0x25, 0x00); //7f New Lock Cond & New light stable

	HI253_write_cmos_sensor(0x30, 0x80);
	HI253_write_cmos_sensor(0x31, 0x80);
	HI253_write_cmos_sensor(0x38, 0x11);
	HI253_write_cmos_sensor(0x39, 0x34);

	HI253_write_cmos_sensor(0x40, 0xf7); //
	HI253_write_cmos_sensor(0x41, 0x55); //44
	HI253_write_cmos_sensor(0x42, 0x33); //43

	HI253_write_cmos_sensor(0x43, 0xf7);
	HI253_write_cmos_sensor(0x44, 0x55); //44
	HI253_write_cmos_sensor(0x45, 0x44); //33

	HI253_write_cmos_sensor(0x46, 0x00);
	HI253_write_cmos_sensor(0x50, 0xb2);
	HI253_write_cmos_sensor(0x51, 0x81);
	HI253_write_cmos_sensor(0x52, 0x98);

	HI253_write_cmos_sensor(0x80, 0x40); //3e
	HI253_write_cmos_sensor(0x81, 0x20);
	HI253_write_cmos_sensor(0x82, 0x3e);

	HI253_write_cmos_sensor(0x83, 0x5e); //5e
	HI253_write_cmos_sensor(0x84, 0x1e); //24
	HI253_write_cmos_sensor(0x85, 0x5e); //54 //56 //5a
	HI253_write_cmos_sensor(0x86, 0x22); //24 //22

	HI253_write_cmos_sensor(0x87, 0x40);
	HI253_write_cmos_sensor(0x88, 0x30);
	HI253_write_cmos_sensor(0x89, 0x3f); //38
	HI253_write_cmos_sensor(0x8a, 0x28); //2a

	HI253_write_cmos_sensor(0x8b, 0x40); //47
	HI253_write_cmos_sensor(0x8c, 0x33); 
	HI253_write_cmos_sensor(0x8d, 0x39); 
	HI253_write_cmos_sensor(0x8e, 0x30); //2c

	HI253_write_cmos_sensor(0x8f, 0x53); //4e
	HI253_write_cmos_sensor(0x90, 0x52); //4d
	HI253_write_cmos_sensor(0x91, 0x51); //4c
	HI253_write_cmos_sensor(0x92, 0x4e); //4a
	HI253_write_cmos_sensor(0x93, 0x4a); //46
	HI253_write_cmos_sensor(0x94, 0x45);
	HI253_write_cmos_sensor(0x95, 0x3d);
	HI253_write_cmos_sensor(0x96, 0x31);
	HI253_write_cmos_sensor(0x97, 0x28);
	HI253_write_cmos_sensor(0x98, 0x24);
	HI253_write_cmos_sensor(0x99, 0x20);
	HI253_write_cmos_sensor(0x9a, 0x20);

	HI253_write_cmos_sensor(0x9b, 0x77);
	HI253_write_cmos_sensor(0x9c, 0x77);
	HI253_write_cmos_sensor(0x9d, 0x48);
	HI253_write_cmos_sensor(0x9e, 0x38);
	HI253_write_cmos_sensor(0x9f, 0x30);

	HI253_write_cmos_sensor(0xa0, 0x60);
	HI253_write_cmos_sensor(0xa1, 0x34);
	HI253_write_cmos_sensor(0xa2, 0x6f);
	HI253_write_cmos_sensor(0xa3, 0xff);

	HI253_write_cmos_sensor(0xa4, 0x14); //1500fps
	HI253_write_cmos_sensor(0xa5, 0x2c); // 700fps
	HI253_write_cmos_sensor(0xa6, 0xcf);

	HI253_write_cmos_sensor(0xad, 0x40);
	HI253_write_cmos_sensor(0xae, 0x4a);

	HI253_write_cmos_sensor(0xaf, 0x28);  // low temp Rgain
	HI253_write_cmos_sensor(0xb0, 0x26);  // low temp Rgain

	HI253_write_cmos_sensor(0xb1, 0x00); //0x20 -> 0x00 0405 modify
	HI253_write_cmos_sensor(0xb4, 0xea);
	HI253_write_cmos_sensor(0xb8, 0xa0); //a2: b-2, R+2  //b4 B-3, R+4 lowtemp
	HI253_write_cmos_sensor(0xb9, 0x00);

	// PAGE 20
	HI253_write_cmos_sensor(0x03, 0x20); //page 20
	HI253_write_cmos_sensor(0x10, 0x9c); //ae off

	// PAGE 22
	HI253_write_cmos_sensor(0x03, 0x22); //page 22
	HI253_write_cmos_sensor(0x10, 0xe9); //awb off

	// PAGE 0
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x0e, 0x03); //PLL On
	HI253_write_cmos_sensor(0x0e, 0x73); //PLLx2

	HI253_write_cmos_sensor(0x03, 0x00); // Dummy 750us
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x03, 0x00);

	HI253_write_cmos_sensor(0x03, 0x00); // Page 0
	HI253_write_cmos_sensor(0x01, 0xf0); // Sleep Off 0xf8->0x50 for solve green line issue
	
	HI253_write_cmos_sensor(0x03, 0x20); 
	HI253_pv_HI253_exposure_lines=(HI253_read_cmos_sensor(0x80)<<16 ) |
									( HI253_read_cmos_sensor(0x81)<<8) |
									HI253_read_cmos_sensor(0x82);
	
	return ERROR_NONE;
}	/* HI253Open() *///by Michael.Xie  2010/11/30

/*************************************************************************
* FUNCTION
*	HI253Close
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
UINT32 HI253Close(void)
{
	HI253_write_cmos_sensor(0x03, 0x00); //enter	page 0
	HI253_write_cmos_sensor(0x01, (HI253_read_cmos_sensor(0x01)|0x01));
//	CISModulePowerOn(FALSE);
	return ERROR_NONE;
}	/* HI253Close() */

/*************************************************************************
* FUNCTION
*	HI253Preview
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
UINT32 HI253Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	kal_uint8 iTemp, temp_AE_reg, temp_AWB_reg;
    kal_uint16 iDummyPixels = 0, iDummyLines = 0, iStartX = 1, iStartY = 1;
     
    HI253_sensor_cap_state = KAL_FALSE;
    
    //4  <1> preview config sequence
	HI253_sensor_pclk=390;
    
    HI253_gPVmode = KAL_TRUE;

	if(sensor_config_data->SensorOperationMode==MSDK_SENSOR_OPERATION_MODE_VIDEO)		// MPEG4 Encode Mode
	{
		HI253_VEDIO_encode_mode = KAL_TRUE;

	}
	else
	{
        HI253_VEDIO_encode_mode = KAL_FALSE;
        HI253_iHI253_Mode = HI253_MODE_PREVIEW;
	}
	HI253_write_cmos_sensor(0x03,0x00); 	
	HI253_Sleep_Mode = (HI253_read_cmos_sensor(0x01) & 0xfe);
	HI253_Sleep_Mode |= 0x01;
	HI253_write_cmos_sensor(0x01, HI253_Sleep_Mode);
	
	HI253_write_cmos_sensor(0x03, 0x20); 
	HI253_write_cmos_sensor(0x10, 0x1c);
	HI253_write_cmos_sensor(0x03, 0x22);
	HI253_write_cmos_sensor(0x10, 0x69);

	HI253_write_cmos_sensor(0x03, 0x00);
	HI253_write_cmos_sensor(0x10, 0x11);
	HI253_write_cmos_sensor(0x12, 0x04);//0x00

	HI253_write_cmos_sensor(0x20, 0x00); 
	HI253_write_cmos_sensor(0x21, 0x04);
	HI253_write_cmos_sensor(0x22, 0x00);
	HI253_write_cmos_sensor(0x23, 0x07);

	HI253_write_cmos_sensor(0x40, 0x01);// 408
	HI253_write_cmos_sensor(0x41, 0x98);
	HI253_write_cmos_sensor(0x42, 0x00);
	HI253_write_cmos_sensor(0x43, 0x82);  //0X14_20120215

	HI253_write_cmos_sensor(0x03, 0x10);
	HI253_write_cmos_sensor(0x3f, 0x02); 
	
	HI253_write_cmos_sensor(0x03, 0x12);
	HI253_write_cmos_sensor(0x20, 0x0f); 
	HI253_write_cmos_sensor(0x21, 0x0f);
	HI253_write_cmos_sensor(0x90, 0x5d);

	HI253_write_cmos_sensor(0x03, 0x13); 
	HI253_write_cmos_sensor(0x80, 0x00); 

	HI253_write_cmos_sensor(0x03, 0x20); 
	HI253_write_cmos_sensor(0x18, 0x38);

	//HI253_write_cmos_sensor(0x83, 0x01);
	//HI253_write_cmos_sensor(0x84, 0x7a);
	//HI253_write_cmos_sensor(0x85, 0x00);
	
	HI253_write_cmos_sensor(0x86, 0x02);
	HI253_write_cmos_sensor(0x87, 0x00);
	
	HI253_write_cmos_sensor(0x88, 0x05); //0X04_20120215 
	HI253_write_cmos_sensor(0x89, 0xe8); //0xec_20120215 
	HI253_write_cmos_sensor(0x8a, 0x00); 

	HI253_write_cmos_sensor(0x8b, 0x7e);
	HI253_write_cmos_sensor(0x8c, 0x00);
	HI253_write_cmos_sensor(0x8d, 0x69);
	HI253_write_cmos_sensor(0x8e, 0x00);

	HI253_write_cmos_sensor(0x9c, 0x18);
	HI253_write_cmos_sensor(0x9d, 0x00);
	
	HI253_write_cmos_sensor(0x9e, 0x02);
	HI253_write_cmos_sensor(0x9f, 0x00);

	HI253_Sleep_Mode = (HI253_read_cmos_sensor(0x01) & 0xfe);
	HI253_Sleep_Mode |= 0x00;
	HI253_write_cmos_sensor(0x01, HI253_Sleep_Mode);

	HI253_write_cmos_sensor(0x03, 0x20);//page 20
	HI253_write_cmos_sensor(0x10, 0x9c);//AE ON
		
	HI253_write_cmos_sensor(0x03, 0x22);
	HI253_write_cmos_sensor(0x10, 0xe9);//AWB ON
	
	HI253_write_cmos_sensor(0x03, 0x20);
	HI253_write_cmos_sensor(0x18, 0x30);
    //4 <3> set mirror and flip
    HI253_write_cmos_sensor(0x03,0x00); 	
	HI253_HV_Mirror = (HI253_read_cmos_sensor(0x11) & 0xfc);
#if defined(BIRD_HI253_HV_MIRROR)
 switch (sensor_config_data->SensorImageMirror) 
 	{
    case IMAGE_NORMAL:
//    	HI253_HV_Mirror |= 0x00; 
         HI253_HV_Mirror |= 0x03; 
        break;
    case IMAGE_H_MIRROR:
        HI253_HV_Mirror |= 0x02; 
        break;
    case IMAGE_V_MIRROR:
        HI253_HV_Mirror |= 0x01;
        break;
    case IMAGE_HV_MIRROR:
        HI253_HV_Mirror |= 0x00; 
        break;
    default:
        break; 
    }
#else
    switch (sensor_config_data->SensorImageMirror) {
    case IMAGE_NORMAL:
    	HI253_HV_Mirror |= 0x00; 
//        HI253_HV_Mirror |= 0x03; 
        break;
    case IMAGE_H_MIRROR:
        HI253_HV_Mirror |= 0x02; 
        break;
    case IMAGE_V_MIRROR:
        HI253_HV_Mirror |= 0x01; 
        break;
    case IMAGE_HV_MIRROR:
        HI253_HV_Mirror |= 0x00; 
        break;
    default:
        break;
    }
#endif	
	HI253_write_cmos_sensor(0x11, HI253_HV_Mirror);


    //4 <7> set shutter
    image_window->GrabStartX = iStartX;
    image_window->GrabStartY = iStartY;
    image_window->ExposureWindowWidth = HI253_IMAGE_SENSOR_PV_WIDTH;
    image_window->ExposureWindowHeight = HI253_IMAGE_SENSOR_PV_HEIGHT;
    
    HI253_DELAY_AFTER_PREVIEW = 1;

	// copy sensor_config_data
	memcpy(&HI253SensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
  	return ERROR_NONE;
}	/* HI253Preview() *///by Michael.Xie  2010/11/30

UINT32 HI253Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
    volatile kal_uint32 shutter = HI253_exposure_lines, temp_reg;
    kal_uint8 temp_AE_reg, temp;
    kal_uint16 AE_setting_delay = 0;
    kal_uint8 CLK_DIV_REG = 0;

    HI253_sensor_cap_state = KAL_TRUE;

    if ((image_window->ImageTargetWidth<=HI253_IMAGE_SENSOR_PV_WIDTH)&&
        (image_window->ImageTargetHeight<=HI253_IMAGE_SENSOR_PV_HEIGHT))
    {
    	/* Less than PV Mode */
        HI253_capture_pclk_in_M = HI253_preview_pclk_in_M;   //Don't change the clk

        HI253_write_cmos_sensor(0x03, 0x00);

		HI253_write_cmos_sensor(0x20, 0x00); 
		HI253_write_cmos_sensor(0x21, 0x04); 
		HI253_write_cmos_sensor(0x22, 0x00); 
		HI253_write_cmos_sensor(0x23, 0x07); 
	
		HI253_write_cmos_sensor(0x40, 0x01); 
		HI253_write_cmos_sensor(0x41, 0x98); 
		HI253_write_cmos_sensor(0x42, 0x00); 
		HI253_write_cmos_sensor(0x43, 0x14); 
	
		
		HI253_write_cmos_sensor(0x03, 0x10);
		HI253_write_cmos_sensor(0x3f, 0x00);
	
		//Page12
		HI253_write_cmos_sensor(0x03, 0x12); //Function
		HI253_write_cmos_sensor(0x20, 0x0f);
		HI253_write_cmos_sensor(0x21, 0x0f);
		HI253_write_cmos_sensor(0x90, 0x5d);  
	
		//Page13
		HI253_write_cmos_sensor(0x03, 0x13); //Function
		HI253_write_cmos_sensor(0x80, 0xfd); //Function
	
		// 800*600	
		HI253_write_cmos_sensor(0x03, 0x00);
		HI253_write_cmos_sensor(0x10, 0x11);
	
		HI253_write_cmos_sensor(0x03, 0x20); 
		HI253_write_cmos_sensor(0x86, 0x02);
		HI253_write_cmos_sensor(0x87, 0x00);
		
		HI253_write_cmos_sensor(0x8b, 0x7e);
		HI253_write_cmos_sensor(0x8c, 0x00);
		HI253_write_cmos_sensor(0x8d, 0x69);
		HI253_write_cmos_sensor(0x8e, 0x00);
	
		HI253_write_cmos_sensor(0x9c, 0x18);
		HI253_write_cmos_sensor(0x9d, 0x00);
		HI253_write_cmos_sensor(0x9e, 0x02);
		HI253_write_cmos_sensor(0x9f, 0x00);
		
		HI253_write_cmos_sensor(0x03, 0x20);
		HI253_pv_HI253_exposure_lines = (HI253_read_cmos_sensor(0x80) << 16)|(HI253_read_cmos_sensor(0x81) << 8)|HI253_read_cmos_sensor(0x82);
		HI253_cp_HI253_exposure_lines=HI253_pv_HI253_exposure_lines;	
	
		if(HI253_cp_HI253_exposure_lines<1)
			HI253_cp_HI253_exposure_lines=1;
		if(HI253_gPVmode == KAL_FALSE)
		{	
			HI253_write_cmos_sensor(0x03, 0x20); 
			HI253_write_cmos_sensor(0x83, HI253_cp_HI253_exposure_lines >> 16);
			HI253_write_cmos_sensor(0x84, (HI253_cp_HI253_exposure_lines >> 8) & 0x000000FF);
			HI253_write_cmos_sensor(0x85, HI253_cp_HI253_exposure_lines & 0x000000FF);
		}
        
        image_window->GrabStartX = 1;
        image_window->GrabStartY = 1;
        image_window->ExposureWindowWidth= HI253_IMAGE_SENSOR_PV_WIDTH;
        image_window->ExposureWindowHeight = HI253_IMAGE_SENSOR_PV_HEIGHT;

    }
    else 
    {    
		/* 2M FULL Mode */
        
        HI253_write_cmos_sensor(0x03,0x00); 	
		HI253_Sleep_Mode = (HI253_read_cmos_sensor(0x01) & 0xfe);
		HI253_Sleep_Mode |= 0x01;
		HI253_write_cmos_sensor(0x01, HI253_Sleep_Mode);
	
		CLK_DIV_REG=(HI253_read_cmos_sensor(0x12)&0xFc);    // don't divide,PCLK=48M
		CLK_DIV_REG |= 0x00;
		//read the shutter (manual exptime)
		HI253_write_cmos_sensor(0x03, 0x20);
		HI253_pv_HI253_exposure_lines = (HI253_read_cmos_sensor(0x80) << 16)|(HI253_read_cmos_sensor(0x81) << 8)|HI253_read_cmos_sensor(0x82);
		HI253_cp_HI253_exposure_lines = HI253_pv_HI253_exposure_lines;
		
		HI253_write_cmos_sensor(0x03, 0x00);
	
		HI253_write_cmos_sensor(0x20, 0x00); 
		HI253_write_cmos_sensor(0x21, 0x0a); 
		HI253_write_cmos_sensor(0x22, 0x00); 
		HI253_write_cmos_sensor(0x23, 0x0a); 
	
		HI253_write_cmos_sensor(0x40, 0x01); //360
		HI253_write_cmos_sensor(0x41, 0x98); 
		HI253_write_cmos_sensor(0x42, 0x00);
		HI253_write_cmos_sensor(0x43, 0x14);
	
		HI253_write_cmos_sensor(0x03, 0x10);
		HI253_write_cmos_sensor(0x3f, 0x00);
	
		//Page12
		HI253_write_cmos_sensor(0x03, 0x12);
		HI253_write_cmos_sensor(0x20, 0x0f);
		HI253_write_cmos_sensor(0x21, 0x0f);
		HI253_write_cmos_sensor(0x90, 0x5d);
	
		//Page13
		HI253_write_cmos_sensor(0x03, 0x13);
		HI253_write_cmos_sensor(0x80, 0xfd);
	
		// 1600*1200	
		HI253_write_cmos_sensor(0x03,0x00);
		HI253_write_cmos_sensor(0x10,0x00);
              
        if ((image_window->ImageTargetWidth<=HI253_IMAGE_SENSOR_FULL_WIDTH)&&
        (image_window->ImageTargetHeight<=HI253_IMAGE_SENSOR_FULL_HEIGHT))
        {
        	HI253_capture_pclk_in_M = 520;
        	HI253_sensor_pclk = 520;                 
        }
        else//Interpolate to 3M
        {
        	HI253_capture_pclk_in_M = 520;
        	HI253_sensor_pclk = 520;                
        }
        
		HI253_write_cmos_sensor(0x03, 0x00); 
		HI253_write_cmos_sensor(0x12, 0x04);
	
		HI253_write_cmos_sensor(0x03, 0x20); 
		HI253_write_cmos_sensor(0x86, 0x02);
		HI253_write_cmos_sensor(0x87, 0x00);
		
		HI253_write_cmos_sensor(0x8b, 0x7e);
		HI253_write_cmos_sensor(0x8c, 0x00);
		HI253_write_cmos_sensor(0x8d, 0x69);
		HI253_write_cmos_sensor(0x8e, 0x00);
	
		HI253_write_cmos_sensor(0x9c, 0x18);
		HI253_write_cmos_sensor(0x9d, 0x00);
		HI253_write_cmos_sensor(0x9e, 0x02);
		HI253_write_cmos_sensor(0x9f, 0x00);
	
	
		if(HI253_cp_HI253_exposure_lines<1)
			HI253_cp_HI253_exposure_lines=1;
		if(HI253_gPVmode == KAL_FALSE)
		{
			HI253_write_cmos_sensor(0x03, 0x20); 
			HI253_write_cmos_sensor(0x83, HI253_cp_HI253_exposure_lines >> 16);
			HI253_write_cmos_sensor(0x84, (HI253_cp_HI253_exposure_lines >> 8) & 0x000000FF);
			HI253_write_cmos_sensor(0x85, HI253_cp_HI253_exposure_lines & 0x000000FF);
		}
	
		HI253_write_cmos_sensor(0x03,0x00); 	
		HI253_Sleep_Mode = (HI253_read_cmos_sensor(0x01) & 0xfe);
		HI253_Sleep_Mode |= 0x00;
		HI253_write_cmos_sensor(0x01, HI253_Sleep_Mode);       
        
        image_window->GrabStartX=1;
        image_window->GrabStartY=1;
        image_window->ExposureWindowWidth=HI253_IMAGE_SENSOR_FULL_WIDTH;
        image_window->ExposureWindowHeight=HI253_IMAGE_SENSOR_FULL_HEIGHT;
    }

    // AEC/AGC/AWB will be enable in preview and param_wb function
    /* total delay 4 frame for AE stable */
	HI253_gPVmode=KAL_TRUE;

	HI253_DELAY_AFTER_PREVIEW = 2;

	// copy sensor_config_data
	memcpy(&HI253SensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	return ERROR_NONE;
}	/* HI253Capture() */

UINT32 HI253GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
	pSensorResolution->SensorFullWidth=HI253_IMAGE_SENSOR_FULL_WIDTH;  //modify by yanxu
	pSensorResolution->SensorFullHeight=HI253_IMAGE_SENSOR_FULL_HEIGHT;
	pSensorResolution->SensorPreviewWidth=HI253_IMAGE_SENSOR_PV_WIDTH ;
	pSensorResolution->SensorPreviewHeight=HI253_IMAGE_SENSOR_PV_HEIGHT;

	return ERROR_NONE;
}	/* HI253GetResolution() */

UINT32 HI253GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
					  MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	pSensorInfo->SensorPreviewResolutionX=HI253_IMAGE_SENSOR_PV_WIDTH;
	pSensorInfo->SensorPreviewResolutionY=HI253_IMAGE_SENSOR_PV_HEIGHT;
	pSensorInfo->SensorFullResolutionX=HI253_IMAGE_SENSOR_FULL_WIDTH;
	pSensorInfo->SensorFullResolutionY=HI253_IMAGE_SENSOR_FULL_HEIGHT;

	pSensorInfo->SensorCameraPreviewFrameRate=30;
	pSensorInfo->SensorVideoFrameRate=30;
	pSensorInfo->SensorStillCaptureFrameRate=10;
	pSensorInfo->SensorWebCamCaptureFrameRate=15;
	pSensorInfo->SensorResetActiveHigh=FALSE;
	pSensorInfo->SensorResetDelayCount=1;
	pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_YUYV;
	pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;	/*??? */
	pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorInterruptDelayLines = 1;
	pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_PARALLEL;

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
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_800_MODE].BinningEnable=TRUE;

	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].MaxWidth=CAM_SIZE_1M_WIDTH;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].MaxHeight=CAM_SIZE_1M_HEIGHT;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].ISOSupported=TRUE;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].BinningEnable=TRUE;
	pSensorInfo->CaptureDelayFrame = 1; 
	pSensorInfo->PreviewDelayFrame = 0; 
	pSensorInfo->VideoDelayFrame = 4; 		
	pSensorInfo->SensorMasterClockSwitch = 0; 
    pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;   		

	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount=	3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
			pSensorInfo->SensorGrabStartX = 1; 
			pSensorInfo->SensorGrabStartY = 1;
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount=	3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
			pSensorInfo->SensorGrabStartX = 1; 
			pSensorInfo->SensorGrabStartY = 1;             
		break;
		default:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount=3;
			pSensorInfo->SensorClockRisingCount=0;
			pSensorInfo->SensorClockFallingCount=2;
			pSensorInfo->SensorPixelClockCount=3;
			pSensorInfo->SensorDataLatchCount=2;
			pSensorInfo->SensorGrabStartX = 1; 
			pSensorInfo->SensorGrabStartY = 1;             
		break;
	}
	HI253_PixelClockDivider=pSensorInfo->SensorPixelClockCount;
	memcpy(pSensorConfigData, &HI253SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	return ERROR_NONE;
}	/* HI253GetInfo() */


UINT32 HI253Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
			HI253Preview(pImageWindow, pSensorConfigData);
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
			HI253Capture(pImageWindow, pSensorConfigData);
		break;
		default:
		    break; 
	}
	return TRUE;
}	/* HI253Control() */ 

BOOL HI253_set_param_wb(UINT16 para)
{
		SENSORDB("[Enter]HI253 set_param_wb func:para = %d\n",para);
		
		//if(HI253_wb == para) return KAL_TRUE;	

		HI253_wb = para;
	
    switch (para)
    {
    case AWB_MODE_AUTO:
        HI253_write_cmos_sensor(0x03, 0x22);			
		HI253_write_cmos_sensor(0x11, 0x2e);		
		HI253_write_cmos_sensor(0x83, 0x5e);
		HI253_write_cmos_sensor(0x84, 0x1e);
		HI253_write_cmos_sensor(0x85, 0x52);  //0x5e_20120215
		HI253_write_cmos_sensor(0x86, 0x22);
        break;
    case AWB_MODE_CLOUDY_DAYLIGHT: //cloudy
        HI253_write_cmos_sensor(0x03, 0x22);
		HI253_write_cmos_sensor(0x11, 0x28);
		HI253_write_cmos_sensor(0x80, 0x71);
		HI253_write_cmos_sensor(0x82, 0x2b);
		HI253_write_cmos_sensor(0x83, 0x72);
		HI253_write_cmos_sensor(0x84, 0x70);
		HI253_write_cmos_sensor(0x85, 0x2b);
		HI253_write_cmos_sensor(0x86, 0x28);
        break;
    case AWB_MODE_DAYLIGHT: //sunny
        HI253_write_cmos_sensor(0x03, 0x22);
		HI253_write_cmos_sensor(0x11, 0x28);		  
		HI253_write_cmos_sensor(0x80, 0x59);
		HI253_write_cmos_sensor(0x82, 0x29);
		HI253_write_cmos_sensor(0x83, 0x60);
		HI253_write_cmos_sensor(0x84, 0x50);
		HI253_write_cmos_sensor(0x85, 0x2f);
		HI253_write_cmos_sensor(0x86, 0x23);
        break;
    case AWB_MODE_INCANDESCENT: //office
        HI253_write_cmos_sensor(0x03, 0x22);
		HI253_write_cmos_sensor(0x11, 0x28);		  
		HI253_write_cmos_sensor(0x80, 0x29);
		HI253_write_cmos_sensor(0x82, 0x54);
		HI253_write_cmos_sensor(0x83, 0x2e);
		HI253_write_cmos_sensor(0x84, 0x23);
		HI253_write_cmos_sensor(0x85, 0x58);
		HI253_write_cmos_sensor(0x86, 0x4f);
        break;
    case AWB_MODE_TUNGSTEN: //home
        HI253_write_cmos_sensor(0x03, 0x22);
		HI253_write_cmos_sensor(0x80, 0x24);
		HI253_write_cmos_sensor(0x81, 0x20);
		HI253_write_cmos_sensor(0x82, 0x58);
		HI253_write_cmos_sensor(0x83, 0x27);
		HI253_write_cmos_sensor(0x84, 0x22);
		HI253_write_cmos_sensor(0x85, 0x58);
		HI253_write_cmos_sensor(0x86, 0x52);
        break;
    case AWB_MODE_FLUORESCENT:
        HI253_write_cmos_sensor(0x03, 0x22);
		HI253_write_cmos_sensor(0x11, 0x28);
		HI253_write_cmos_sensor(0x80, 0x41);
		HI253_write_cmos_sensor(0x82, 0x42);
		HI253_write_cmos_sensor(0x83, 0x44);
		HI253_write_cmos_sensor(0x84, 0x34);
		HI253_write_cmos_sensor(0x85, 0x46);
		HI253_write_cmos_sensor(0x86, 0x3a);
        break; 
    default:
        return FALSE;
    }

    return TRUE;
} /* HI253_set_param_wb */

BOOL HI253_set_param_effect(UINT16 para)
{
   BOOL  ret = TRUE;
   //UINT8  temp_reg;
   //temp_reg=HI253_read_cmos_sensor(0x3391);
    switch (para)
    {
    case MEFFECT_OFF:
        HI253_write_cmos_sensor(0x03, 0x10);
		HI253_write_cmos_sensor(0x11, 0x03);
		HI253_write_cmos_sensor(0x12, 0x30);
		HI253_write_cmos_sensor(0x13, 0x02);
		HI253_write_cmos_sensor(0x44, 0x80);
		HI253_write_cmos_sensor(0x45, 0x80);
        break;
    case MEFFECT_SEPIA:
        HI253_write_cmos_sensor(0x03, 0x10);
		HI253_write_cmos_sensor(0x11, 0x03);
		HI253_write_cmos_sensor(0x12, 0x33);
		HI253_write_cmos_sensor(0x13, 0x02);
		HI253_write_cmos_sensor(0x44, 0x70);
		HI253_write_cmos_sensor(0x45, 0x98);
        break;
    case MEFFECT_NEGATIVE:
        HI253_write_cmos_sensor(0x03, 0x10);
		HI253_write_cmos_sensor(0x11, 0x03);
		HI253_write_cmos_sensor(0x12, 0x08);
		HI253_write_cmos_sensor(0x13, 0x02);
		HI253_write_cmos_sensor(0x14, 0x00);
        break;
    case MEFFECT_SEPIAGREEN:
        HI253_write_cmos_sensor(0x03, 0x10);
		HI253_write_cmos_sensor(0x11, 0x03);
		HI253_write_cmos_sensor(0x12, 0x03);
		HI253_write_cmos_sensor(0x40, 0x00);
		HI253_write_cmos_sensor(0x13, 0x02);
		HI253_write_cmos_sensor(0x44, 0x30);
		HI253_write_cmos_sensor(0x45, 0x50);
        break;
    case MEFFECT_SEPIABLUE:
        HI253_write_cmos_sensor(0x03, 0x10);
		HI253_write_cmos_sensor(0x11, 0x03);
		HI253_write_cmos_sensor(0x12, 0x03);
		HI253_write_cmos_sensor(0x40, 0x00);
		HI253_write_cmos_sensor(0x13, 0x02);
		HI253_write_cmos_sensor(0x44, 0xb0);
		HI253_write_cmos_sensor(0x45, 0x40);
        break;
	case MEFFECT_MONO: //B&W
        HI253_write_cmos_sensor(0x03, 0x10);
		HI253_write_cmos_sensor(0x11, 0x03);
		HI253_write_cmos_sensor(0x12, 0x03);
		HI253_write_cmos_sensor(0x13, 0x02);
		HI253_write_cmos_sensor(0x40, 0x00);
		HI253_write_cmos_sensor(0x44, 0x80);
		HI253_write_cmos_sensor(0x45, 0x80);
		break; 
    default:
        ret = FALSE;
    }

    return ret;

} /* HI253_set_param_effect */

BOOL HI253_set_param_banding(UINT16 para)
{

    kal_uint8 banding;

    //banding = HI253_read_cmos_sensor(0x3014);
    switch (para)
    {
	case AE_FLICKER_MODE_50HZ:
		HI253_Banding_setting = AE_FLICKER_MODE_50HZ;
		HI253_write_cmos_sensor(0x03,0x20);
		HI253_write_cmos_sensor(0x10,0x9c);
		break;
    case AE_FLICKER_MODE_60HZ:			
        HI253_Banding_setting = AE_FLICKER_MODE_60HZ;
        HI253_write_cmos_sensor(0x03,0x20);
		HI253_write_cmos_sensor(0x10,0x8c);			 
        break;
      default:
          return FALSE;
    }

    return TRUE;
} /* HI253_set_param_banding */

BOOL HI253_set_param_exposure(UINT16 para)
{
    HI253_write_cmos_sensor(0x03,0x10);
	HI253_write_cmos_sensor(0x12,(HI253_read_cmos_sensor(0x12)|0x10));//make sure the Yoffset control is opened.

    switch (para)
    {
    case AE_EV_COMP_n13:
        HI253_write_cmos_sensor(0x40, 0xd0);
        break;
    case AE_EV_COMP_n10:
        HI253_write_cmos_sensor(0x40,0xc0);
        break;
    case AE_EV_COMP_n07:
        HI253_write_cmos_sensor(0x40,0xb0);
        break;
    case AE_EV_COMP_n03:
        HI253_write_cmos_sensor(0x40,0xa0);
        break;
    case AE_EV_COMP_00:
        HI253_write_cmos_sensor(0x40,0x00);
        break;
    case AE_EV_COMP_03:
        HI253_write_cmos_sensor(0x40,0x10);
        break;
    case AE_EV_COMP_07:
        HI253_write_cmos_sensor(0x40,0x20);
        break;
    case AE_EV_COMP_10:
        HI253_write_cmos_sensor(0x40,0x30);
        break;
    case AE_EV_COMP_13:
        HI253_write_cmos_sensor(0x40,0x40);
        break;
    default:
        return FALSE;
    }

    return TRUE;
} /* HI253_set_param_exposure */



UINT32 HI253YUVSensorSetting(FEATURE_ID iCmd, UINT32 iPara)
{
//   if( HI253_sensor_cap_state == KAL_TRUE)
//	   return TRUE;
	
	switch (iCmd) {
	case FID_SCENE_MODE:	    
//	    printk("Set Scene Mode:%d\n", iPara); 
	    if (iPara == SCENE_MODE_OFF)
	    {
	        HI253_night_mode(0); 
	    }
	    else if (iPara == SCENE_MODE_NIGHTSCENE)
	    {
               HI253_night_mode(1); 
	    }	    
	    break; 	    
	case FID_AWB_MODE:
//	    printk("Set AWB Mode:%d\n", iPara); 	    
           HI253_set_param_wb(iPara);
	break;
	case FID_COLOR_EFFECT:
//	    printk("Set Color Effect:%d\n", iPara); 	    	    
           HI253_set_param_effect(iPara);
	break;
	case FID_AE_EV: 	    
//           printk("Set EV:%d\n", iPara); 	    	    
           HI253_set_param_exposure(iPara);
	break;
	case FID_AE_FLICKER:
//           printk("Set Flicker:%d\n", iPara); 	    	    	    
           HI253_set_param_banding(iPara);
	break;
	case FID_ZOOM_FACTOR:
	    zoom_factor = iPara; 
	default:
	break;
	}
	return TRUE;
}   /* MT9P012YUVSensorSetting */

UINT32 HI253YUVSetVideoMode(UINT16 u2FrameRate)
{
    kal_uint8 iTemp;
    /* to fix VSYNC, to fix frame rate */
    //printk("Set YUV Video Mode \n");     
   // iTemp = HI253_read_cmos_sensor(0x3014);
    //HI253_write_cmos_sensor(0x3014, iTemp & 0xf7); //Disable night mode

    if (u2FrameRate == 30)
    {
        //HI253_write_cmos_sensor(0x302d, 0x00);
        //HI253_write_cmos_sensor(0x302e, 0x00);
    }
    else if (u2FrameRate == 15)       
    {
        //HI253_write_cmos_sensor(0x300e, 0x34);
        //HI253_write_cmos_sensor(0x302A, HI253_VIDEO_15FPS_FRAME_LENGTH>>8);  /*  15fps*/
        //HI253_write_cmos_sensor(0x302B, HI253_VIDEO_15FPS_FRAME_LENGTH&0xFF);
                
        // clear extra exposure line
   // HI253_write_cmos_sensor(0x302d, 0x00);
    //HI253_write_cmos_sensor(0x302e, 0x00);
    }
    else 
    {
        printk("Wrong frame rate setting \n");
    }
    HI253_VEDIO_encode_mode = KAL_TRUE; 
        
    return TRUE;
}

void HI253GetAFMaxNumFocusAreas(UINT32 *pFeatureReturnPara32)
{	
    *pFeatureReturnPara32 = 0;    
    SENSORDB(" HI253GetAFMaxNumFocusAreas, *pFeatureReturnPara32 = %d\n",  *pFeatureReturnPara32);

}

void HI253GetAFMaxNumMeteringAreas(UINT32 *pFeatureReturnPara32)
{	
    *pFeatureReturnPara32 = 0;    
    SENSORDB(" HI253GetAFMaxNumMeteringAreas,*pFeatureReturnPara32 = %d\n",  *pFeatureReturnPara32);

}

void HI253GetExifInfo(UINT32 exifAddr)
{
    SENSOR_EXIF_INFO_STRUCT* pExifInfo = (SENSOR_EXIF_INFO_STRUCT*)exifAddr;
    pExifInfo->FNumber = 28;
    pExifInfo->AEISOSpeed = AE_ISO_100;
    pExifInfo->AWBMode = HI253_wb;
    pExifInfo->CapExposureTime = 0;
    pExifInfo->FlashLightTimeus = 0;
    pExifInfo->RealISOValue = AE_ISO_100;
}

UINT32 HI253FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,
							 UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
{
	UINT16 *pFeatureReturnPara16=(UINT16 *) pFeaturePara;
	UINT16 *pFeatureData16=(UINT16 *) pFeaturePara;
	UINT32 *pFeatureReturnPara32=(UINT32 *) pFeaturePara;
	UINT32 *pFeatureData32=(UINT32 *) pFeaturePara;
	MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData=(MSDK_SENSOR_CONFIG_STRUCT *) pFeaturePara;
	MSDK_SENSOR_REG_INFO_STRUCT *pSensorRegData=(MSDK_SENSOR_REG_INFO_STRUCT *) pFeaturePara; 
	
	switch (FeatureId)
	{
		case SENSOR_FEATURE_GET_RESOLUTION:
			*pFeatureReturnPara16++=HI253_IMAGE_SENSOR_FULL_WIDTH;
			*pFeatureReturnPara16=HI253_IMAGE_SENSOR_FULL_HEIGHT;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_GET_PERIOD:
			*pFeatureReturnPara16++=HI253_PV_PERIOD_PIXEL_NUMS+HI253_PV_dummy_pixels;
			*pFeatureReturnPara16=HI253_PV_PERIOD_LINE_NUMS+HI253_PV_dummy_lines;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			*pFeatureReturnPara32 = HI253_sensor_pclk/10;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_SET_ESHUTTER:
		break;
		case SENSOR_FEATURE_SET_NIGHTMODE:
			HI253_night_mode((BOOL) *pFeatureData16);
		break;
		case SENSOR_FEATURE_SET_GAIN:
		case SENSOR_FEATURE_SET_FLASHLIGHT:
		break;
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			HI253_isp_master_clock=*pFeatureData32;
		break;
		case SENSOR_FEATURE_SET_REGISTER:
			HI253_write_cmos_sensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
		break;
		case SENSOR_FEATURE_GET_REGISTER:
			pSensorRegData->RegData = HI253_read_cmos_sensor(pSensorRegData->RegAddr);
		break;
		case SENSOR_FEATURE_GET_CONFIG_PARA:
			memcpy(pSensorConfigData, &HI253SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
			*pFeatureParaLen=sizeof(MSDK_SENSOR_CONFIG_STRUCT);
		break;
		case SENSOR_FEATURE_SET_CCT_REGISTER:
		case SENSOR_FEATURE_GET_CCT_REGISTER:
		case SENSOR_FEATURE_SET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_REGISTER_DEFAULT:

		case SENSOR_FEATURE_CAMERA_PARA_TO_SENSOR:
		case SENSOR_FEATURE_SENSOR_TO_CAMERA_PARA:
		//case SENSOR_FEATURE_GET_GROUP_COUNT:
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
//		       printk("HI253 YUV sensor Setting:%d, %d \n", *pFeatureData32,  *(pFeatureData32+1));
			HI253YUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
		break; 			
		case SENSOR_FEATURE_SET_VIDEO_MODE:
		       HI253YUVSetVideoMode(*pFeatureData16);
		       break; 

		case SENSOR_FEATURE_GET_AF_MAX_NUM_FOCUS_AREAS:
        
			//*pFeatureData32=0;         
      //*pFeatureParaLen=4;
        SENSORDB("SENSOR_FEATURE_GET_AF_MAX_NUM_FOCUS_AREAS \n");
      	HI253GetAFMaxNumFocusAreas(pFeatureReturnPara32);            
        *pFeatureParaLen=4;
       		        break;     
    		case SENSOR_FEATURE_GET_AE_MAX_NUM_METERING_AREAS:
			//*pFeatureData32=0;            
      //*pFeatureParaLen=4;
      	SENSORDB("SENSOR_FEATURE_GET_AE_MAX_NUM_METERING_AREAS \n");
      	HI253GetAFMaxNumMeteringAreas(pFeatureReturnPara32);            
        *pFeatureParaLen=4;
       		        break;
   	       case SENSOR_FEATURE_GET_EXIF_INFO:
       		    SENSORDB("SENSOR_FEATURE_GET_EXIF_INFO\n");
                    SENSORDB("EXIF addr = 0x%x\n",*pFeatureData32);          
	    //*pFeatureData32=0;  
	     HI253GetExifInfo(*pFeatureData32);
                        break;

		
		default:
			break;			
	}
	return ERROR_NONE;
}	/* HI253FeatureControl() *///by Michael.Xie  2010/11/30
SENSOR_FUNCTION_STRUCT	SensorFuncHI253=
{
	HI253Open,
	HI253GetInfo,
	HI253GetResolution,
	HI253FeatureControl,
	HI253Control,
	HI253Close
};

UINT32 HI253_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&SensorFuncHI253;

	return ERROR_NONE;
}	/* SensorInit() */



