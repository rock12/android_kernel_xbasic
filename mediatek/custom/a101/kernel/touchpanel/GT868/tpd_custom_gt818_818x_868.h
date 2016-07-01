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

#ifndef TPD_CUSTOM_GT818B_H__
#define TPD_CUSTOM_GT818B_H__



/* Pre-defined definition */
#define TPD_AUTOUPDATE_FIRMWARE

#define TPD_TYPE_CAPACITIVE
//#define TPD_TYPE_RESISTIVE
#define TPD_POWER_SOURCE         MT6323_POWER_LDO_VGP1
#define TPD_I2C_NUMBER           0
#define TPD_WAKEUP_TRIAL         60
#define TPD_WAKEUP_DELAY         100

#define TPD_DELAY                (2*HZ/100)
#define TPD_CALIBRATION_MATRIX  {962,0,0,0,1600,0,0,0};

#define TPD_HAVE_CALIBRATION
#define TPD_HAVE_BUTTON               //report key as coordinate,Vibration feedback
//#define TPD_HAVE_TOUCH_KEY            //report key as key_value
//#define TPD_CREATE_WR_NODE
//#define TPD_ESD_PROTECT
#define TPD_HAVE_TREMBLE_ELIMINATION

#define TPD_NO_GPIO
#define TPD_RESET_PIN_ADDR   (PERICFG_BASE + 0xC000)
//#define TPD_DITO_SENSOR
//#define TPD_CONDITION_SWITCH
#define TPD_RESET_ISSUE_WORKAROUND
#define TPD_MAX_RESET_COUNT 3

#define TPD_X_RES (320)
#define TPD_Y_RES (480)

//#define TPD_WARP_X		//mirror
//#define TPD_WARP_Y		//flip

#ifdef TPD_WARP_Y
#undef TPD_WARP_Y
#define TPD_WARP_Y(y) ( TPD_Y_RES - 1 - y )
#else
#define TPD_WARP_Y(y) y
#endif

#ifdef TPD_WARP_X
#undef TPD_WARP_X
#define TPD_WARP_X(x) ( TPD_X_RES - 1 - x )
#else
#define TPD_WARP_X(x) x
#endif

#ifdef BIRD_13C43_S8_TP_PARAM
#define TPD_KEY_COUNT   3
#define key_1           60,530              //auto define  
#define key_2           160,530
#define key_3           260,530

#define TPD_KEYS        {KEY_MENU,KEY_HOMEPAGE,KEY_BACK}
#define TPD_KEYS_DIM    {{key_1,50,30},{key_2,50,30},{key_3,50,30}}
#elif defined(BIRD_13E27_I600)

#define TPD_KEY_COUNT   3
#define key_1           80,900              //auto define  
#define key_2           220,900
#define key_3           400,900

#define TPD_KEYS        {KEY_MENU,KEY_HOMEPAGE,KEY_BACK}
#define TPD_KEYS_DIM    {{key_1,50,30},{key_2,50,30},{key_3,50,30}}
#else
#define TPD_KEY_COUNT   4
#define key_1           60,850              //auto define  
#define key_2           180,850
#define key_3           300,850
#define key_4           420,850

#define TPD_KEYS        {KEY_BACK, KEY_HOMEPAGE, KEY_MENU, KEY_SEARCH}
#define TPD_KEYS_DIM    {{key_1,50,30},{key_2,50,30},{key_3,50,30},{key_4,50,30}}
#endif

enum
{
    GT818,
    GT818X,
    GT868
};


#ifdef BIRD_13C43_S8_TP_PARAM
static u8 gt868_cfg_data_vendor1[] =
{  	
	0x0E,0x0C,0x0A,0x08,0x06,0x04,0x02,0x00,0x10,0x12,
	0x00,0x00,0x10,0x00,0x20,0x00,0x30,0x00,0x40,0x00,
	0x50,0x00,0x60,0x00,0x70,0x00,0x80,0x00,0x91,0x11,
	0xA3,0x33,0xB6,0x66,0xC0,0x00,0xD0,0x00,0xE0,0x00,
	0xF0,0x00,0x17,0x03,0x08,0x08,0x08,0x2A,0x2A,0x2A,
	0x0D,0x0C,0x08,0x4F,0x3F,0x85,0x03,0x00,0x02,0x40,
	0x01,0xE0,0x01,0x00,0x00,0x54,0x53,0x4F,0x4E,0x00,
	0x00,0x29,0x19,0x02,0x09,0x80,0x00,0x00,0x00,0x00,
	0x0A,0x10,0x00,0x00,0x00,0x40,0x1B,0x08,0x38,0x00,
	0x10,0x50,0x80,0x00,0x0A,0x4F,0x3F,0x30,0x20,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x01
};

//TODO: puts your vendor2 cfg data(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_vendor2[] =
{
};

//TODO: puts your vendor3 cfg data(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_vendor3[] =
{
};

//If you have no cfg_data_with_charger(*.cfg),please keep it the same with the above corresponding cfg_data
static u8 gt868_cfg_data_with_charger_vendor1[] =
{

  	0x0E,0x0C,0x0A,0x08,0x06,0x04,0x02,0x00,0x10,0x12,
  	0x00,0x00,0x10,0x00,0x20,0x00,0x30,0x00,0x40,0x00,
  	0x50,0x00,0x60,0x00,0x70,0x00,0x80,0x00,0x91,0x11,
  	0xA3,0x33,0xB6,0x66,0xC0,0x00,0xD0,0x00,0xE0,0x00,
  	0xF0,0x00,0x17,0x03,0x08,0x08,0x08,0x2A,0x2A,0x2A,
  	0x0D,0x0C,0x08,0x4F,0x3F,0x85,0x03,0x00,0x02,0x40,
  	0x01,0xE0,0x01,0x00,0x00,0x54,0x53,0x4F,0x4E,0x00,
  	0x00,0x29,0x19,0x02,0x09,0x80,0x00,0x00,0x00,0x00,
  	0x0A,0x10,0x00,0x00,0x00,0x40,0x1B,0x08,0x38,0x00,
  	0x10,0x50,0x80,0x00,0x0A,0x4F,0x3F,0x30,0x20,0x00,
  	0x00,0x00,0x00,0x00,0x0F,0x01
};

//TODO: puts your vendor2 cfg data with charger(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_with_charger_vendor2[] =
{
};

//TODO: puts your vendor3 cfg data with charger(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_with_charger_vendor3[] =
{
};	

#elif defined(BIRD_13E27_I600)
//If there is only one vendor provide TP for you,puts the only one cfg_data(*.cfg) here,otherwise puts your vendor1 cfg_data here.
static u8 gt868_cfg_data_vendor1[] =
{  	
  	0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,0x12,
		0x00,0x00,0x10,0x00,0x20,0x00,0x30,0x00,0x40,0x00,
		0x50,0x00,0x60,0x00,0x70,0x00,0x80,0x00,0x90,0x00,
		0xA0,0x00,0xB0,0x00,0xC0,0x00,0xD0,0x00,0xE0,0x00,
		0xF0,0x00,0x07,0x03,0x40,0x40,0x40,0x21,0x21,0x21,
		0x0F,0x0E,0x0A,0x40,0x30,0x0D,0x03,0x00,0x05,0xE0,
		0x01,0x20,0x03,0x00,0x00,0x58,0x52,0x52,0x4D,0x00,
		0xC7,0x2F,0x14,0x05,0x2F,0x40,0x00,0x00,0x00,0x00,
		0x10,0x08,0x00,0x00,0x20,0x40,0x07,0x40,0x21,0x00,
		0x21,0x61,0x91,0x00,0x0D,0x45,0x35,0x25,0x20,0x00,
		0x00,0x00,0x00,0x00,0x00,0x01
};

//TODO: puts your vendor2 cfg data(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_vendor2[] =
{
};

//TODO: puts your vendor3 cfg data(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_vendor3[] =
{
};

//If you have no cfg_data_with_charger(*.cfg),please keep it the same with the above corresponding cfg_data
static u8 gt868_cfg_data_with_charger_vendor1[] =
{

  	0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,0x12,
		0x00,0x00,0x10,0x00,0x20,0x00,0x30,0x00,0x40,0x00,
		0x50,0x00,0x60,0x00,0x70,0x00,0x80,0x00,0x90,0x00,
		0xA0,0x00,0xB0,0x00,0xC0,0x00,0xD0,0x00,0xE0,0x00,
		0xF0,0x00,0x07,0x03,0x40,0x40,0x40,0x21,0x21,0x21,
		0x0F,0x0E,0x0A,0x40,0x30,0x0D,0x03,0x00,0x05,0xE0,
		0x01,0x20,0x03,0x00,0x00,0x58,0x52,0x52,0x4D,0x00,
		0xC7,0x2F,0x14,0x05,0x2F,0x40,0x00,0x00,0x00,0x00,
		0x10,0x08,0x00,0x00,0x20,0x40,0x07,0x40,0x21,0x00,
		0x21,0x61,0x91,0x00,0x0D,0x45,0x35,0x25,0x20,0x00,
		0x00,0x00,0x00,0x00,0x00,0x01
};

//TODO: puts your vendor2 cfg data with charger(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_with_charger_vendor2[] =
{
};

//TODO: puts your vendor3 cfg data with charger(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_with_charger_vendor3[] =
{
};

#else
//If there is only one vendor provide TP for you,puts the only one cfg_data(*.cfg) here,otherwise puts your vendor1 cfg_data here.
static u8 gt868_cfg_data_vendor1[] =
{
  	0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,0x12,
  	0x00,0x00,0x10,0x00,0x20,0x00,0x30,0x00,0x40,0x00,
  	0x50,0x00,0x60,0x00,0x70,0x00,0x80,0x00,0x90,0x00,
  	0xA0,0x00,0xB0,0x00,0xC0,0x00,0xD0,0x00,0xE0,0x00,
  	0xF0,0x00,0x07,0x03,0x40,0x40,0x40,0x25,0x25,0x25,
  	0x0F,0x0E,0x0A,0x40,0x30,0x0D,0x03,0x00,0x05,0xE0,
  	0x01,0x20,0x03,0x00,0x00,0x58,0x52,0x52,0x4D,0x00,
  	0xC7,0x2A,0x14,0x05,0x2F,0xA0,0x00,0x00,0x00,0x00,
  	0x14,0x10,0x00,0x00,0x20,0x40,0x07,0x40,0x2A,0x00,
  	0x21,0x61,0x91,0x00,0x0D,0x35,0x25,0x25,0x20,0x00,
  	0x00,0x00,0x00,0x00,0x00,0x01
};

//TODO: puts your vendor2 cfg data(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_vendor2[] =
{
};

//TODO: puts your vendor3 cfg data(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_vendor3[] =
{
};

//If you have no cfg_data_with_charger(*.cfg),please keep it the same with the above corresponding cfg_data
static u8 gt868_cfg_data_with_charger_vendor1[] =
{
		0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,0x12,
  	0x00,0x00,0x10,0x00,0x20,0x00,0x30,0x00,0x40,0x00,
  	0x50,0x00,0x60,0x00,0x70,0x00,0x80,0x00,0x90,0x00,
  	0xA0,0x00,0xB0,0x00,0xC0,0x00,0xD0,0x00,0xE0,0x00,
  	0xF0,0x00,0x07,0x03,0x40,0x40,0x40,0x25,0x25,0x25,
  	0x0F,0x0E,0x0A,0x40,0x30,0x0D,0x03,0x00,0x05,0xE0,
  	0x01,0x20,0x03,0x00,0x00,0x58,0x52,0x52,0x4D,0x00,
  	0xC7,0x2A,0x14,0x05,0x2F,0xA0,0x00,0x00,0x00,0x00,
  	0x14,0x10,0x00,0x00,0x20,0x40,0x07,0x40,0x2A,0x00,
  	0x21,0x61,0x91,0x00,0x0D,0x35,0x25,0x25,0x20,0x00,
  	0x00,0x00,0x00,0x00,0x00,0x01
};

//TODO: puts your vendor2 cfg data with charger(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_with_charger_vendor2[] =
{
};

//TODO: puts your vendor3 cfg data with charger(*.cfg) here,in case there are more than one vendors to provide TP for you.
//Otherwise please remain it EMPTY!
static u8 gt868_cfg_data_with_charger_vendor3[] =
{
};
#endif


static u8 gt818_cfg_data_vendor1[] =
{
};
static u8 gt818x_cfg_data_vendor1[] =
{
};
static u8 gt818_cfg_data_vendor2[] =
{
};
static u8 gt818x_cfg_data_vendor2[] =
{
};
static u8 gt818_cfg_data_vendor3[] =
{
};
static u8 gt818x_cfg_data_vendor3[] =
{
};

static u8 gt818_cfg_data_with_charger_vendor1[] =
{
};
static u8 gt818x_cfg_data_with_charger_vendor1[] =
{
};
static u8 gt818_cfg_data_with_charger_vendor2[] =
{
};
static u8 gt818x_cfg_data_with_charger_vendor2[] =
{
};
static u8 gt818_cfg_data_with_charger_vendor3[] =
{
};
static u8 gt818x_cfg_data_with_charger_vendor3[] =
{
};

static u8 cfg_sensor_id = 0;
static u8 *gt818_cfg_data_ptr[] = { gt818_cfg_data_vendor1, gt818_cfg_data_vendor2, gt818_cfg_data_vendor3 };
static u8 *gt818x_cfg_data_ptr[] = { gt818x_cfg_data_vendor1, gt818x_cfg_data_vendor2, gt818x_cfg_data_vendor3 };
static u8 *gt868_cfg_data_ptr[] = { gt868_cfg_data_vendor1, gt868_cfg_data_vendor2, gt868_cfg_data_vendor3 };

static u8 gt818_cfg_data_len[] = { sizeof(gt818_cfg_data_vendor1) / sizeof(gt818_cfg_data_vendor1[0]),
                                   sizeof(gt818_cfg_data_vendor2) / sizeof(gt818_cfg_data_vendor2[0]),
                                   sizeof(gt818_cfg_data_vendor3) / sizeof(gt818_cfg_data_vendor3[0])
                                 };
static u8 gt818x_cfg_data_len[] = { sizeof(gt818x_cfg_data_vendor1) / sizeof(gt818x_cfg_data_vendor1[0]),
                                    sizeof(gt818x_cfg_data_vendor2) / sizeof(gt818x_cfg_data_vendor2[0]),
                                    sizeof(gt818x_cfg_data_vendor3) / sizeof(gt818x_cfg_data_vendor3[0])
                                  };
static u8 gt868_cfg_data_len[] = { sizeof(gt868_cfg_data_vendor1) / sizeof(gt868_cfg_data_vendor1[0]),
                                   sizeof(gt868_cfg_data_vendor2) / sizeof(gt868_cfg_data_vendor2[0]),
                                   sizeof(gt868_cfg_data_vendor3) / sizeof(gt868_cfg_data_vendor3[0])
                                 };

static u8 *gt818_cfg_data_with_charger_ptr[] = { gt818_cfg_data_with_charger_vendor1, gt818_cfg_data_with_charger_vendor2, gt818_cfg_data_with_charger_vendor3 };
static u8 *gt818x_cfg_data_with_charger_ptr[] = { gt818x_cfg_data_with_charger_vendor1, gt818x_cfg_data_with_charger_vendor2, gt818x_cfg_data_with_charger_vendor3 };
static u8 *gt868_cfg_data_with_charger_ptr[] = { gt868_cfg_data_with_charger_vendor1, gt868_cfg_data_with_charger_vendor2, gt868_cfg_data_with_charger_vendor3 };

static u8 gt818_cfg_data_with_charger_len[] = { sizeof(gt818_cfg_data_with_charger_vendor1) / sizeof(gt818_cfg_data_with_charger_vendor1[0]),
                                                sizeof(gt818_cfg_data_with_charger_vendor2) / sizeof(gt818_cfg_data_with_charger_vendor2[0]),
                                                sizeof(gt818_cfg_data_with_charger_vendor3) / sizeof(gt818_cfg_data_with_charger_vendor3[0])
                                              };
static u8 gt818x_cfg_data_with_charger_len[] = { sizeof(gt818x_cfg_data_with_charger_vendor1) / sizeof(gt818x_cfg_data_with_charger_vendor1[0]),
                                                 sizeof(gt818x_cfg_data_with_charger_vendor2) / sizeof(gt818x_cfg_data_with_charger_vendor2[0]),
                                                 sizeof(gt818x_cfg_data_with_charger_vendor3) / sizeof(gt818x_cfg_data_with_charger_vendor3[0])
                                               };
static u8 gt868_cfg_data_with_charger_len[] = { sizeof(gt868_cfg_data_with_charger_vendor1) / sizeof(gt868_cfg_data_with_charger_vendor1[0]),
                                                sizeof(gt868_cfg_data_with_charger_vendor2) / sizeof(gt868_cfg_data_with_charger_vendor2[0]),
                                                sizeof(gt868_cfg_data_with_charger_vendor3) / sizeof(gt868_cfg_data_with_charger_vendor3[0])
                                              };

#endif /* TOUCHPANEL_H__ */
