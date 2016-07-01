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
 *   sensor.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Header file of Sensor driver
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
 * $Log: ov2640yuv_Sensor.h,v $
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
 * Revision 1.1  2012/11/13 03:25:34  bianhuannan
 * no message
 *
 *
 * 09 10 2010 jackie.su
 * [ALPS00002279] [Need Patch] [Volunteer Patch] ALPS.Wxx.xx Volunteer patch for
 * .10y dual sensor
 *
 * 09 02 2010 jackie.su
 * [ALPS00002279] [Need Patch] [Volunteer Patch] ALPS.Wxx.xx Volunteer patch for
 * .roll back dual sensor
 *
 * Mar 4 2010 mtk70508
 * [DUMA00154792] Sensor driver
 * 
 *
 * Feb 24 2010 mtk01118
 * [DUMA00025869] [Camera][YUV I/F & Query feature] check in camera code
 * 
 *
 * Aug 5 2009 mtk01051
 * [DUMA00009217] [Camera Driver] CCAP First Check In
 * 
 *
 * Apr 7 2009 mtk02204
 * [DUMA00004012] [Camera] Restructure and rename camera related custom folders and folder name of came
 * 
 *
 * Mar 26 2009 mtk02204
 * [DUMA00003515] [PC_Lint] Remove PC_Lint check warnings of camera related drivers.
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
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
/* SENSOR FULL SIZE */
#ifndef __SENSOR_H
#define __SENSOR_H


typedef enum {
    SENSOR_MODE_INIT = 0,
    SENSOR_MODE_PREVIEW,
    SENSOR_MODE_CAPTURE
} OV2659_SENSOR_MODE;

typedef enum _OV2640_OP_TYPE_ {
        OV2640_MODE_NONE,
        OV2640_MODE_PREVIEW,
        OV2640_MODE_CAPTURE,
        OV2640_MODE_QCIF_VIDEO,
        OV2640_MODE_CIF_VIDEO,
        OV2640_MODE_QVGA_VIDEO
    } OV2640_OP_TYPE;

extern OV2640_OP_TYPE OV2640_g_iOV2640_Mode;

#define OV2640_ID_REG                          (0x300A)
#define OV2640_INFO_REG                        (0x300B)
 
/* sensor size */
#define OV2640_IMAGE_SENSOR_SVGA_WIDTH          (800)
#define OV2640_IMAGE_SENSOR_SVGA_HEIGHT         (600)
#define OV2640_IMAGE_SENSOR_UVGA_WIDTH        (1600) 
#define OV2640_IMAGE_SENSOR_UVGA_HEIGHT       (1200)

/* SENSOR PIXEL/LINE NUMBERS IN ONE PERIOD */
//#define OV2640_FULL_PERIOD_PIXEL_NUMS  (1922)  // default pixel#(w/o dummy pixels) in UXGA mode
//#define OV2640_FULL_PERIOD_LINE_NUMS   (1248)  // default line#(w/o dummy lines) in UXGA mode
//#define OV2640_PV_PERIOD_PIXEL_NUMS    (1190)  // default pixel#(w/o dummy pixels) in SVGA mode
//#define OV2640_PV_PERIOD_LINE_NUMS     (672)   // default line#(w/o dummy lines) in SVGA mode

/* SENSOR EXPOSURE LINE LIMITATION */ //check with FAE
//#define FULL_EXPOSURE_LIMITATION    (1234)
//#define PV_EXPOSURE_LIMITATION      (616)
//#define OV2640_FULL_EXPOSURE_LIMITATION    (1248)
//#define OV2640_PV_EXPOSURE_LIMITATION      (672)

/* Sesnor Pixel/Line Numbers in One Period */	
#define OV2640_PV_PERIOD_PIXEL_NUMS    		(1300)  		/* Default preview line length */
#define OV2640_PV_PERIOD_LINE_NUMS     		(616)   	/* Default preview frame length */
#define OV2640_FULL_PERIOD_PIXEL_NUMS  		(1951)  	/* Default full size line length */
#define OV2640_FULL_PERIOD_LINE_NUMS   		(1232)  	/* Default full size frame length */

/* Sensor Exposure Line Limitation */
#define OV2640_PV_EXPOSURE_LIMITATION      	(616-4)
#define OV2640_FULL_EXPOSURE_LIMITATION    	(1232-4)

/* Config the ISP grab start x & start y, Config the ISP grab width & height */
#define OV2640_PV_GRAB_START_X 				(1)
#define OV2640_PV_GRAB_START_Y  			(1)
#define OV2640_PV_GRAB_WIDTH				(OV2640_IMAGE_SENSOR_SVGA_WIDTH - 8)
#define OV2640_PV_GRAB_HEIGHT				(OV2640_IMAGE_SENSOR_SVGA_HEIGHT - 6)

#define OV2640_FULL_GRAB_START_X   			(1)
#define OV2640_FULL_GRAB_START_Y	  		(1)
#define OV2640_FULL_GRAB_WIDTH				(OV2640_IMAGE_SENSOR_UVGA_WIDTH - 16)
#define OV2640_FULL_GRAB_HEIGHT				(OV2640_IMAGE_SENSOR_UVGA_HEIGHT - 12)

/*50Hz,60Hz*/
#define OV2640_NUM_50HZ                        (50 * 2)
#define OV2640_NUM_60HZ                        (60 * 2)

/* FRAME RATE UNIT */
#define OV2640_FRAME_RATE_UNIT                 (10)

/* MAX CAMERA FRAME RATE */
#define OV2640_MAX_CAMERA_FPS                  (OV2640_FRAME_RATE_UNIT * 30)


/* DUMMY NEEDS TO BE INSERTED */
/* SETUP TIME NEED TO BE INSERTED */


/* SENSOR READ/WRITE ID */
	#define OV2640_WRITE_ID							    0x60
	#define OV2640_READ_ID								0x61

//export functions
UINT32 OV2640Open(void);
UINT32 OV2640GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution);
UINT32 OV2640GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_INFO_STRUCT *pSensorInfo, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 OV2640Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 OV2640FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId, UINT8 *pFeaturePara,UINT32 *pFeatureParaLen);
UINT32 OV2640Close(void);

UINT32 OV2640_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);


#endif /* __SENSOR_H */
