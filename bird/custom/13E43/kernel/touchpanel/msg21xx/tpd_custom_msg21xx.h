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
/*ergate-001*/


#ifndef TOUCHPANEL_H
#define TOUCHPANEL_H

#define TPD_TYPE_CAPACITIVE
//#define TPD_TYPE_RESISTIVE
#define TPD_POWER_SOURCE_CUSTOM	MT6323_POWER_LDO_VGP1
//#define TPD_POWER_SOURCE         MT6323_POWER_LDO_VGP1

#define TPD_I2C_NUMBER           1
#define TPD_WAKEUP_TRIAL         15
#define TPD_WAKEUP_DELAY         100

#define TPD_DELAY                (2*HZ/100)

#if defined(BIRD_TP_PARAM_WVGA)
#define TPD_RES_X                480
#define TPD_RES_Y                800
#else
#define TPD_RES_X                540
#define TPD_RES_Y                960
#endif

/* to turn on calibration, unmark these two lines */
#define TPD_CALIBRATION_MATRIX  {962,0,0,0,1600,0,0,0};
#define TPD_HAVE_CALIBRATION
//#define TPD_CUSTOM_CALIBRATION

#define TPD_HAVE_POWER_ON_OFF

//#define TPD_WARP_START          {11,0,300,470};
//#define TPD_WARP_END            {7,0,314,475};


#define TPD_HAVE_BUTTON
//#define    HQ_TP_IMPROVE

#if defined(BIRD_TP_PARAM_WVGA)
#define TPD_BUTTON_HEIGHT 			900
#define TPD_KEY_COUNT           3
#define TPD_KEYS                {KEY_MENU, KEY_HOMEPAGE, KEY_BACK}
#define TPD_KEYS_DIM            {{80,900,60,60},{120,900,60,60},{240,900,60,60}}
#else 
#define TPD_BUTTON_HEIGHT 			1000
#define TPD_KEY_COUNT           3
#define TPD_KEYS                {KEY_MENU, KEY_HOMEPAGE, KEY_BACK}
#define TPD_KEYS_DIM            {{90,1020,60,60},{270,1020,60,60},{450,1020,60,60}}
#endif

#define TPD_HAVE_TREMBLE_ELIMINATION
#define TPD_CUSTOM_TREMBLE_TOLERANCE
#define PRESSURE_FACTOR 10



//#define HAVE_SLEEP_NORMAL_MODE
//#define HAVE_SINGLE_MULTIPLE_SCAN_MODE
/*
static u8 msg2133_bin_oufeiguang[94*1024]=
{
	#include "MSG2133.I"
};
*/
#endif
