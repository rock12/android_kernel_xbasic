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
 *	 sensor.c
 *
 * Project:
 * --------
 *	 RAW
 *
 * Description:
 * ------------
 *	 Source code of Sensor driver
 *
 *
 * Author:
 * -------
 *	 HengJun (MTK70677)
 *
 *============================================================================
 *			   HISTORY
 * Below this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: 1.1 $
 * $Modtime:$
 * $Log: ov5647_Sensor.h,v $
 * Revision 1.1  2013/05/23 03:38:40  ningzhiyu
 * Editor: ningzhiyu
 * Inspector:ningzhiyu
 * Reason: create 72 JB3 module
 * Reference:none
 *
 * Revision 1.1  2013/04/18 12:40:03  huangwenjian
 * Bug[none]
 *
 * Reason:12Y36 T1000  ov8825 & ov5647
 * Author:Huangwenjian
 *
 * Revision 1.2  2013/04/13 07:08:58  huangwenjian
 * Bug[none]
 *
 * Reason:12Y36 T1000  ov5647
 * Author:Huangwenjian
 *
 * Revision 1.1  2013/04/13 03:49:15  huangwenjian
 * Bug[none]
 *
 * Reason:12Y36 T1000 sub ov5647 AR0833
 * Author:Huangwenjian
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#ifndef _OV5647_SENSOR_H
#define _OV5647_SENSOR_H

//#define OV5647_DEBUG
//#define OV5647_DRIVER_TRACE
//#define OV5647_TEST_PATTEM
#ifdef OV5647_DEBUG
//#define SENSORDB printk
#else
//#define SENSORDB(x,...)
#endif

#define OV5647_FACTORY_START_ADDR 0
#define OV5647_ENGINEER_START_ADDR 10
 
typedef enum OV5647_group_enum
{
  OV5647_PRE_GAIN = 0,
  OV5647_CMMCLK_CURRENT,
  OV5647_FRAME_RATE_LIMITATION,
  OV5647_REGISTER_EDITOR,
  OV5647_GROUP_TOTAL_NUMS
} OV5647_FACTORY_GROUP_ENUM;

typedef enum OV5647_register_index
{
  OV5647_SENSOR_BASEGAIN = OV5647_FACTORY_START_ADDR,
  OV5647_PRE_GAIN_R_INDEX,
  OV5647_PRE_GAIN_Gr_INDEX,
  OV5647_PRE_GAIN_Gb_INDEX,
  OV5647_PRE_GAIN_B_INDEX,
  OV5647_FACTORY_END_ADDR
} OV5647_FACTORY_REGISTER_INDEX;

typedef enum OV5647_engineer_index
{
  OV5647_CMMCLK_CURRENT_INDEX = OV5647_ENGINEER_START_ADDR,
  OV5647_ENGINEER_END
} OV5647_FACTORY_ENGINEER_INDEX;

typedef struct _sensor_data_struct
{
  SENSOR_REG_STRUCT reg[OV5647_ENGINEER_END];
  SENSOR_REG_STRUCT cct[OV5647_FACTORY_END_ADDR];
} sensor_data_struct;

/* SENSOR PREVIEW/CAPTURE VT CLOCK */
#define __OV5647_54M__

#if defined(__OV5647_48M__)/*Preview PClk=48.1MHz*/
	#define OV5647_PREVIEW_CLK                     48100000
	#define OV5647_CAPTURE_CLK                     80600000
#elif defined(__OV5647_52M__)/*Preview PClk=52.866666MHz*/
	#define OV5647_PREVIEW_CLK                     52866667
	#define OV5647_CAPTURE_CLK                     80600000
#elif defined(__OV5647_54M__)/*Preview PClk=54.6MHz*/
	#define OV5647_PREVIEW_CLK                     54600000
	#define OV5647_CAPTURE_CLK                     80600000
#elif defined(__OV5647_56M__)/*Preview PClk=56.333MHz*/
	#define OV5647_PREVIEW_CLK                     56333333
	#define OV5647_CAPTURE_CLK                     80600000
#else
	#error "This Sensor PCLK Setting is not configuration"
#endif

#define OV5647_COLOR_FORMAT                    SENSOR_OUTPUT_FORMAT_RAW_B //SENSOR_OUTPUT_FORMAT_RAW_Gb //SENSOR_OUTPUT_FORMAT_RAW_R

#define OV5647_MIN_ANALOG_GAIN				1	/* 1x */
#define OV5647_MAX_ANALOG_GAIN				32	/* 32x */


/* FRAME RATE UNIT */
#define OV5647_FPS(x)                          (10 * (x))
#define CAPTURE_15FPS

/* SENSOR PIXEL/LINE NUMBERS IN ONE PERIOD */
//#define OV5647_FULL_PERIOD_PIXEL_NUMS          2700 /* 9 fps */
#ifdef CAPTURE_15FPS
#define OV5647_FULL_PERIOD_PIXEL_NUMS          2700 /* 15 fps */
#define OV5647_FULL_PERIOD_LINE_NUMS           1968	
#else
#define OV5647_FULL_PERIOD_PIXEL_NUMS          3055 /* 8 fps */
#define OV5647_FULL_PERIOD_LINE_NUMS           1968
#endif


#if defined(__OV5647_48M__)
	#define OV5647_PV_PERIOD_PIXEL_NUMS            1630 /* 30 fps */
	#define OV5647_PV_PERIOD_LINE_NUMS             984
#elif defined(__OV5647_52M__)// TODO: 
	#define OV5647_PV_PERIOD_PIXEL_NUMS            1790 /* 30 fps */	
	#define OV5647_PV_PERIOD_LINE_NUMS             984
#elif defined(__OV5647_54M__)// TODO:
	#define OV5647_PV_PERIOD_PIXEL_NUMS 		   1850 /* 30 fps */	
	#define OV5647_PV_PERIOD_LINE_NUMS			   984
#elif defined(__OV5647_56M__)
	#define OV5647_PV_PERIOD_PIXEL_NUMS            1896 /* 30 fps */	
	#define OV5647_PV_PERIOD_LINE_NUMS             984
#endif


/* SENSOR START/END POSITION */
#define OV5647_FULL_X_START                    4   //(1+16+6)
#define OV5647_FULL_Y_START                    4  //(1+12+4)
#define OV5647_IMAGE_SENSOR_FULL_WIDTH         (2560 - 32) //(2592 - 16) /* 2560 */
#define OV5647_IMAGE_SENSOR_FULL_HEIGHT        (1920 - 24) //(1944 - 12) /* 1920 */
#define OV5647_PV_X_START                      4
#define OV5647_PV_Y_START                      4
#define OV5647_IMAGE_SENSOR_PV_WIDTH           (1280 - 16) /* 1264 */
#define OV5647_IMAGE_SENSOR_PV_HEIGHT          (960 - 12) /* 948 */

#define OV5647_IMAGE_SENSOR_VIDEO_WIDTH    OV5647_IMAGE_SENSOR_PV_WIDTH
#define OV5647_IMAGE_SENSOR_VIDEO_HEIGHT    OV5647_IMAGE_SENSOR_PV_HEIGHT

/* SENSOR READ/WRITE ID */
#define OV5647_WRITE_ID (0x6c)
#define OV5647_READ_ID  (0x6d)

/* SENSOR ID */
//#define OV5647_SENSOR_ID						(0x5647)

/* SENSOR PRIVATE STRUCT */
typedef struct OV5647_sensor_STRUCT
{
  MSDK_SENSOR_CONFIG_STRUCT cfg_data;
  sensor_data_struct eng; /* engineer mode */
  MSDK_SENSOR_ENG_INFO_STRUCT eng_info;
  kal_uint8 mirror;
  kal_bool pv_mode;
  kal_bool video_mode;
  kal_bool NightMode;
  kal_uint16 normal_fps; /* video normal mode max fps */
  kal_uint16 night_fps; /* video night mode max fps */
  kal_uint16 FixedFps;
  kal_uint16 shutter;
  kal_uint16 gain;
  kal_uint32 pclk;
  kal_uint16 frame_height;
  kal_uint16 frame_height_BackUp;
  kal_uint16 line_length;  
  kal_uint16 Prv_line_length;
} OV5647_sensor_struct;

//export functions
UINT32 OV5647Open(void);
UINT32 OV5647Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 OV5647FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId, UINT8 *pFeaturePara,UINT32 *pFeatureParaLen);
UINT32 OV5647GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_INFO_STRUCT *pSensorInfo, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 OV5647GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution);
UINT32 OV5647Close(void);

#define Sleep(ms) mdelay(ms)

#endif 
