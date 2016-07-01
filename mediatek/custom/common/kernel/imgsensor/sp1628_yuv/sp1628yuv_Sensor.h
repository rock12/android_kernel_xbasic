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
 *   sensor.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   CMOS sensor header file
 *
 ****************************************************************************/
#ifndef __SENSOR_H
#define __SENSOR_H

#include "image_sensor.h"//get IMAGE_SENSOR_DRVNAME
#include "kd_imgsensor.h"
#define IMAGE_SENSOR_DRVNAME SENSOR_DRVNAME_SP1628_YUV
//UINT32 SP1628_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
    //------------------------Engineer mode---------------------------------
#define FACTORY_START_ADDR 	0
#define ENGINEER_START_ADDR	10

    typedef enum group_enum {
       PRE_GAIN=0,
	   CMMCLK_CURRENT,
	   FRAME_RATE_LIMITATION,
	   REGISTER_EDITOR,
	   GROUP_TOTAL_NUMS
    } FACTORY_REGISTER_INDEX;

    typedef enum register_index {
        SENSOR_BASEGAIN=FACTORY_START_ADDR,
	      PRE_GAIN_R_INDEX,
	      PRE_GAIN_Gr_INDEX,
	      PRE_GAIN_Gb_INDEX,
	      PRE_GAIN_B_INDEX,
	      FACTORY_END_ADDR
    } CCT_REGISTER_INDEX;
    
 typedef enum engineer_index
{   
	CMMCLK_CURRENT_INDEX=ENGINEER_START_ADDR,
	ENGINEER_END
} FACTORY_ENGINEER_INDEX; 

    //------------------------Engineer mode---------------------------------
    typedef struct {
        SENSOR_REG_STRUCT Reg[ENGINEER_END];
        SENSOR_REG_STRUCT CCT[FACTORY_END_ADDR];
    } SENSOR_DATA_STRUCT,*PSENSOR_DATA_STRUCT;




/*
 typedef struct _sensor_data_struct
 {
   SENSOR_REG_STRUCT reg[MT9P015_ENGINEER_END];
   SENSOR_REG_STRUCT cct[MT9P015_FACTORY_END_ADDR];
 } sensor_data_struct;
*/
	
 #define CAM_PREVIEW_30FPS
 #define SYSTEM_CLK                           (52*1000*1000)
		/* PIXEL CLOCK USED BY BANDING FILTER CACULATION*/
#if defined(CAM_PREVIEW_15FPS)
  #define PIXEL_CLK							    (SYSTEM_CLK/8)		// 52/8 MHz
#elif defined(CAM_PREVIEW_22FPS)
   #define PIXEL_CLK							    (SYSTEM_CLK/6)		// 52/6 MHz
#elif defined(CAM_PREVIEW_30FPS)
   #define PIXEL_CLK 						      	(SYSTEM_CLK/4)		// 52/4 MHz
#endif

   #define SP1628_VIDEO_NORMALMODE_FRAME_RATE							30		// Limitation for MPEG4 Encode Only
   #define SP1628_VIDEO_NIGHTMODE_FRAME_RATE							15
	/* SENSOR PIXEL/LINE NUMBERS IN ONE PERIOD */
	#define VGA_PERIOD_PIXEL_NUMS					784
	#define VGA_PERIOD_LINE_NUMS					510

	/* SENSOR EXPOSURE LINE LIMITATION */
	#define VGA_EXPOSURE_LIMITATION					510

	/* SENSOR GLOBAL GAIN AT NIGHT MODE */
	#define SP1628_SENSOR_NIGHT_MODE_GAIN					0x08	// Please refer to OV7670 Implementation Guide
	
	/* SENSOR VGA SIZE */
	#define IMAGE_SENSOR_VGA_WIDTH					(640-8)
	#define IMAGE_SENSOR_VGA_HEIGHT					(480-6)


    #define IMAGE_SENSOR_FULL_WIDTH          (1280-8) 
    #define IMAGE_SENSOR_FULL_HEIGHT         (960-6) 

    #define IMAGE_SENSOR_PV_WIDTH   IMAGE_SENSOR_FULL_WIDTH//IMAGE_SENSOR_VGA_WIDTH
    #define IMAGE_SENSOR_PV_HEIGHT  IMAGE_SENSOR_FULL_HEIGHT//IMAGE_SENSOR_VGA_HEIGHT
    
	/* SETUP TIME NEED TO BE INSERTED */
	#define IMAGE_SENSOR_VGA_INSERTED_PIXELS		128
	#define IMAGE_SENSOR_VGA_INSERTED_LINES		    17
	
	#define SP1628_WRITE_ID								0x78//0x7A
	#define SP1628_READ_ID								0x79//0x7B
	
  typedef struct
  {
	  kal_uint32 (*Open)(struct i2c_client *i2c_clit);
	  kal_uint32 (*Close)(void);
  
	  kal_uint32 (*GetResolution)(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution);
	  kal_uint32 (*GetInfo)(MSDK_SCENARIO_ID_ENUM ScenarioId,MSDK_SENSOR_INFO_STRUCT *pSensorInfo,MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
  
	  kal_uint32 (*Control)(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);	  
	  kal_uint32 (* FeatureControl)(MSDK_SENSOR_FEATURE_ENUM ScenarioId, kal_uint8 *para, kal_uint32 *len);
  
  }image_sensor_func_struct;
  
  

 void image_sensor_func_config(void);


struct SP1628_Sensor_Struct
{
	struct i2c_client *i2c_clit;
	MSDK_SENSOR_CONFIG_STRUCT cfg_data;
	SENSOR_DATA_STRUCT eng; /* engineer mode */
	MSDK_SENSOR_ENG_INFO_STRUCT eng_info;

	
	kal_bool sensor_night_mode;
	kal_bool MPEG4_encode_mode;

	kal_uint16 dummy_pixels;
	kal_uint16 dummy_lines;
	kal_uint16 extra_exposure_lines;
	kal_uint16 exposure_lines;

	kal_bool MODE_CAPTURE;
	kal_uint16 iBackupExtraExp;


	
	kal_uint32 fPV_PCLK; //26000000;
	kal_uint16 iPV_Pixels_Per_Line;

	kal_bool  bNight_mode; // to distinguish night mode or auto mode, default: auto mode setting
	kal_bool  bBanding_value; // to distinguish between 50HZ and 60HZ.
	kal_uint8 u8Wb_value;
	kal_uint8 u8Effect_value;
	kal_uint8 u8Ev_value;
};


#endif /* __SENSOR_H */ 
