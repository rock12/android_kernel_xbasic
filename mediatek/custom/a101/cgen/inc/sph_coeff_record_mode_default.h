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

/*******************************************************************************
 *
 * Filename:
 * ---------
 *
 *
 * Project:
 * --------
 *   ALPS
 *
 * Description:
 * ------------
 *
 * Author:
 * -------
 * Charlie Lu
 *
 *------------------------------------------------------------------------------
 * $Revision: 1.4 $ 1.0.0
 * $Modtime:$
 * $Log: sph_coeff_record_mode_default.h,v $
 * Revision 1.4  2013/10/12 03:12:46  pengyajun
 * BIRD_AUDIO_PARAM_i600X
 *
 * Revision 1.3  2013/09/13 06:50:12  pengyajun
 * BIRD_AUDIO_PARAM_S100A
 *
 * Revision 1.2  2013/09/11 07:25:20  pengyajun
 * BIRD_AUDIO_PARAM_E700S
 *
 * Revision 1.1  2013/08/09 05:24:41  wangtinglong
 * 13E43 T9 audio param
 *
 *
 *
 *
 *******************************************************************************/
#ifndef HD_RECORD_MODE_SPEECH_COEFF_DEFAULT_H
#define HD_RECORD_MODE_SPEECH_COEFF_DEFAULT_H

#ifdef BIRD_AUDIO_PARAM_T9
#define DEFAULT_HD_RECORD_SPH_MODE_PAR \
     0,	479, 16388, 36892, 37124, 8192,  784, 4,  4048, 197, 611, 0, 0, 0, 0, 8192, \
     0,	479, 16388, 36892, 37124, 8192,  784, 4,  4048, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200,12,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200,12,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 11200, 4,  272, 197, 611, 0, 0, 0, 0, 8192

#define DEFAULT_HD_RECORD_MODE_DEV_MAPPING \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3

#define DEFAULT_HD_RECORD_MODE_INPUT_SRC_MAPPING \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3

#define DEFAULT_STEREO_FLAG \
    1, 0
#define DEFAULT_MONO_FLAG \
    0, 0

#define DEFAULT_HD_RECORD_MODE_STEREO_FLAGS \
    DEFAULT_MONO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG

#define DEFAULT_HD_RECORD_NUM_VOICE_RECOGNITION_SCENES 1
#define DEFAULT_HD_RECORD_NUM_VOICE_SCENES 3
#define DEFAULT_HD_RECORD_NUM_VIDEO_SCENES 2
#define DEFAULT_HD_RECORD_NUM_VOICE_UNLOCK_SCENES 1
#define DEFAULT_HD_RECORD_NUM_CUSTOMIZATION_SCENES 3 

#define DEFAULT_HD_RECORD_SCENE_TABLE \
    0,  1,  0xFF,\
    2,  3,  0xFF,\
    4,  5,  0xFF,\
    6,  7,  0xFF,\
    8,  9,  0xFF,\
    10, 11, 0xFF,\
    12, 13, 0xFF,\
    14, 15, 0xFF,\
    16, 17, 0xFF,\
    18, 19, 0xFF

#define DEFAULT_HD_RECORD_SCENE_NAME \
    "VR",\
    "Normal",\
    "Meeting",\
    "Lecture",\
    "Normal",\
    "Meeting",\
    "VoiceUnLk",\
    "Reserve1",\
    "Reserve2",\
    "Reserve3"

#elif defined(BIRD_AUDIO_PARAM_E700S)||defined(BIRD_AUDIO_PARAM_S100A)||defined(BIRD_AUDIO_PARAM_i600X)
#define DEFAULT_HD_RECORD_SPH_MODE_PAR \
     0,   479, 16388, 36892, 37127,  8192, 11200,     0,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37127,  8192, 11200,     0,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37127,  8192, 11200,     0,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37127,  8192, 11200,     0,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37127,  8192, 11200,     0,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37127,  8192, 11200,     0,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37127,  8192, 11200,     8,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37127,  8192, 11200,     8,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37127,  8192, 11200,     0,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37127,  8192, 11200,     0,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37127,  8192, 11200,     0,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37127,  8192, 11200,     0,   272,   197,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,  2245,   611,     0,     0,     0,     0,  8192, \
     0,   479, 16388, 36892, 37124,  8192,   768,     0,  4048,   197,   611,     0,     0,     0,     0,  8192

#define DEFAULT_HD_RECORD_MODE_DEV_MAPPING \
 0, 1, 3, \
 0, 1, 3, \
 0, 1, 3, \
 0, 1, 3, \
 0, 1, 3, \
 0, 1, 3, \
 0, 1, 3, \
 0, 1, 3, \
 0, 1, 3, \
 0, 1, 3

#define DEFAULT_HD_RECORD_MODE_INPUT_SRC_MAPPING \
 5, 2, 3, \
 5, 2, 3, \
 5, 2, 3, \
 5, 2, 3, \
 5, 2, 3, \
 5, 2, 3, \
 5, 2, 3, \
 5, 2, 3, \
 5, 2, 3, \
 5, 2, 3

#define DEFAULT_STEREO_FLAG \
    1, 0
#define DEFAULT_MONO_FLAG \
    0, 0

#define DEFAULT_HD_RECORD_MODE_STEREO_FLAGS \
    DEFAULT_MONO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG

#define DEFAULT_HD_RECORD_NUM_VOICE_RECOGNITION_SCENES 1
#define DEFAULT_HD_RECORD_NUM_VOICE_SCENES 3
#define DEFAULT_HD_RECORD_NUM_VIDEO_SCENES 2
#define DEFAULT_HD_RECORD_NUM_VOICE_UNLOCK_SCENES 1
#define DEFAULT_HD_RECORD_NUM_CUSTOMIZATION_SCENES 3

#define DEFAULT_HD_RECORD_SCENE_TABLE \
0x0, 0x1, 0xff, \
0x2, 0x3, 0xff, \
0x4, 0x5, 0xff, \
0x6, 0x7, 0xff, \
0x8, 0x9, 0xff, \
0xa, 0xb, 0xff, \
0xc, 0xd, 0xff, \
0xe, 0xf, 0xff, \
0x10, 0x11, 0xff, \
0x12, 0x13, 0xff

#define DEFAULT_HD_RECORD_SCENE_NAME \
    "VR",\
    "Normal",\
    "Meeting",\
    "Lecture",\
    "Normal",\
    "Meeting",\
    "VoiceUnLk",\
    "Reserve1",\
    "Reserve2",\
    "Reserve3"

#else
#define DEFAULT_HD_RECORD_SPH_MODE_PAR \
     0,	479, 16388,     0,  4359, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0,	479, 16388,     0,  4359, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 1024, 0, 36560, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 1024, 0, 36560, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 1024, 8, 36688, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 1024, 8, 36688, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  773, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 1024, 0, 36560, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37127, 8192, 1024, 0, 36560, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048, 2245, 611, 0, 0, 0, 0, 8192, \
     0, 479, 16388, 36892, 37124, 8192,  768, 0,  4048,  197, 611, 0, 0, 0, 0, 8192

#define DEFAULT_HD_RECORD_MODE_DEV_MAPPING \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3, \
    0, 1, 3

#define DEFAULT_HD_RECORD_MODE_INPUT_SRC_MAPPING \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3, \
    5, 2, 3

#define DEFAULT_STEREO_FLAG \
    1, 0
#define DEFAULT_MONO_FLAG \
    0, 0

#define DEFAULT_HD_RECORD_MODE_STEREO_FLAGS \
    DEFAULT_MONO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_STEREO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG, \
    DEFAULT_MONO_FLAG

#define DEFAULT_HD_RECORD_NUM_VOICE_RECOGNITION_SCENES 1
#define DEFAULT_HD_RECORD_NUM_VOICE_SCENES 3
#define DEFAULT_HD_RECORD_NUM_VIDEO_SCENES 2
#define DEFAULT_HD_RECORD_NUM_VOICE_UNLOCK_SCENES 1
#define DEFAULT_HD_RECORD_NUM_CUSTOMIZATION_SCENES 3 

#define DEFAULT_HD_RECORD_SCENE_TABLE \
    0,  1,  0xFF,\
    2,  3,  0xFF,\
    4,  5,  0xFF,\
    6,  7,  0xFF,\
    8,  9,  0xFF,\
    10, 11, 0xFF,\
    12, 13, 0xFF,\
    14, 15, 0xFF,\
    16, 17, 0xFF,\
    18, 19, 0xFF

#define DEFAULT_HD_RECORD_SCENE_NAME \
    "VR",\
    "Normal",\
    "Meeting",\
    "Lecture",\
    "Normal",\
    "Meeting",\
    "VoiceUnLk",\
    "Reserve1",\
    "Reserve2",\
    "Reserve3"
#endif
#endif
