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

#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>

#define Lcd_Log printf
#else
#include <linux/string.h>
#include <linux/kernel.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>

#define Lcd_Log printk
#endif

#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (800)
#define LCM_ID       (0x8009)
#define REGFLAG_DELAY             							0xAB
#define REGFLAG_END_OF_TABLE      							0xAA   // END OF REGISTERS MARKER
#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)									lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)				lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

#define DISPLAY_DIRECTION_0_MODE

static struct LCM_setting_table lcm_initialization_setting[] = {
           /*{0x00,1,{0x00}},
            {0xff,3,{0x80,0x09,0x01}},

            {0x00,1,{0x80}},
            {0xff,2,{0x80,0x09}},

            {0x00,1,{0x80}},
            {0xf5,12,{0x01,0x18,0x02,0x18,0x10,0x18,0x02,0x18,0x0e,0x18,0x0f,0x20}},

            {0x00,1,{0x90}},
            {0xf5,10,{0x02,0x18,0x08,0x18,0x06,0x18,0x0d,0x18,0x0b,0x18}},

            {0x00,1,{0xa0}},
            {0xf5,8,{0x10,0x18,0x01,0x18,0x14,0x18,0x14,0x18}},

            {0x00,1,{0xb0}},
            {0xf5,12,{0x14,0x18,0x12,0x18,0x13,0x18,0x11,0x18,0x13,0x18,0x00,0x00}},

            {0x00,1,{0xb4}},
            {0xc0,1,{0x00}},

            {0x00,1,{0x82}},
            {0xc5,1,{0xa3}},

            {0x00,1,{0x90}},
            {0xc5,2,{0xd6,0x76}},
            {0x00,1,{0x00}},
            {0xd8,2,{0xa7,0xa7}},

            {0x00,1,{0x00}},
            {0xd9,1,{0x80}},

            {0x00,1,{0x81}},
            {0xc1,1,{0x22}}, //0x08

            {0x00,1,{0xa1}},
            {0xc1,1,{0x08}},

            {0x00,1,{0xa3}},
            {0xc0,1,{0x1b}},

            {0x00,1,{0x81}},
            {0xc4,1,{0x83}},

            {0x00,1,{0x92}},
            {0xc5,1,{0x01}},

            {0x00,1,{0xb1}},
            {0xc5,1,{0xa9}},

            {0x00,1,{0x80}},
            {0xce,6,{0x84,0x03,0x00,0x83,0x03,0x00}},

            {0x00,1,{0x90}},
            {0xb3,1,{0x02}},

            {0x00,1,{0x92}},
            {0xb3,1,{0x45}},

            {0x00,1,{0x80}},
            {0xc0,5,{0x00,0x57,0x00,0x15,0x15}},

            {0x00,1,{0x90}},
            {0xce,6,{0x33,0x5d,0x00,0x33,0x5e,0x00}},


            {0x00,1,{0xa0}},

            {0xce,14,{0x38,0x02,0x03,0x57,0x00,0x00,0x00,0x38,0x01,0x03,0x58,0x00,0x00,0x00}},
            {0x00,1,{0xb0}},
            {0xce,14,{0x38,0x00,0x03,0x59,0x00,0x00,0x00,0x30,0x00,0x03,0x5a,0x00,0x00,0x00}},
            {0x00,1,{0xc0}},
            {0xce,14,{0x30,0x01,0x03,0x5b,0x00,0x00,0x00,0x30,0x02,0x03,0x5c,0x00,0x00,0x00}},

            {0x00,1,{0xd0}},
            {0xce,14,{0x30,0x03,0x03,0x5d,0x00,0x00,0x00,0x30,0x04,0x03,0x5e,0x00,0x00,0x00}},

            {0x00,1,{0xc7}},
            {0xcf,1,{0x00}},

            {0x00,1,{0xc0}},
            {0xcb,15,{0x00,0x00,0x00,0x00,0x54,0x54,0x54,0x54,0x00,0x54,0x00,0x54,0x00,0x00,0x00}},

            {0x00,1,{0xd0}},
            {0xcb,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x54,0x54,0x54,0x00,0x54}},

            {0x00,1,{0xe0}},
            {0xcb,9,{0x00,0x54,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

            {0x00,1,{0x80}},
            {0xcc,10,{0x00,0x00,0x00,0x00,0x0c,0x0a,0x10,0x0e,0x00,0x02}},

            {0x00,1,{0x90}},
            {0xcc,15,{0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B}},

            {0x00,1,{0xa0}},
            {0xcc,15,{0x09,0x0f,0x0d,0x00,0x01,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},



            {0x00,1,{0xb0}},
            {0xcc,10,{0x00,0x00,0x00,0x00,0x0d,0x0f,0x09,0x0b,0x00,0x05}},

            {0x00, 1 ,{0xc0}},
            {0xcc, 15  ,{0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e}},
            {0x00,1,{0xd0}},
            {0xcc,15,{0x10,0x0a,0x0c,0x00,0x06,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

            {0x00, 1 ,{0x00}},
            {0xe1, 16  ,{0x00,0x0E,0x14,0x0F,0x08,0x0F,0x0b,0x09,0x05,0x08,0x0F,0x09,0x10,0x11,0x0B,0x09}},
            {0x00,1,{0x00}},
            {0xe2,16,{0x00,0x0E,0x14,0x0F,0x08,0x0F,0x0b,0x09,0x05,0x08,0x0F,0x09,0x10,0x11,0x0B,0x09}},

            {0x00, 1 ,{0x00}},
            {0x26, 1  ,{0x00}},

            {0x00, 1 ,{0x00}},
            {0x35, 1  ,{0x00}},
            
            {0x00, 1 ,{0x00}},
            {0x44, 2 ,{0x01,0x00}},

            {0x00, 1 ,{0x00}},
            {0x2A,4,{0x00,0x00,0x01,0xDF}},
            {0x00,1,{0x00}},
            {0x2b,4,{0x00,0x00,0x03,0x1F}},

            {0x00, 1 ,{0x00}},
            {0x11, 1 ,{0x00}},
            {REGFLAG_DELAY, 150, {}},
            {0x00, 1 ,{0x00}},
            {0x29, 1 ,{0x00}},
            {REGFLAG_DELAY, 200, {}},*/
            
{0x00, 1 ,{0x00}},
{0xff, 3  ,{0x80,0x09,0x01}},
{0x00, 1 ,{0x80}},
{0xff, 2  ,{0x80,0x09}},
{0x00, 1 ,{0x03}},
{0xff, 1  ,{0x01}},
{0x00, 1 ,{0xb4}},
{0xc0, 1  ,{0x10}},

{0x00, 1 ,{0x80}},
{0xC4, 1  ,{0x30}},

{0x00, 1 ,{0x8a}},
{0xC4, 1  ,{0x40}},

{0x00, 1 ,{0x89}},
{0xC4, 1  ,{0x08}},
{0x00, 1 ,{0xa3}},
{0xC0, 1  ,{0x00}},
{0x00, 1 ,{0x82}},
{0xC5, 1  ,{0xA3}},
{0x00, 1 ,{0x90}},
{0xC5, 2  ,{0x96,0x89}},
{0x00, 1 ,{0x00}},
{0xD8, 2  ,{0x73,0x73}},
{0x00, 1 ,{0x00}},
{0xD9, 1  ,{0x68}},//73-->68

//{0x00, 1 ,{0x80}},
//{0xD6, 1  ,{0x58}},//add code

{0x00, 1 ,{0x00}},
{0xE1, 16 ,{0x00,0x09,0x0F,0x0E,0x07,0x10,0x0B,0x0A,0x04,0x07,0x0B,0x08,0x0F,0x10,0x0A,0x01}},
{0x00, 1 ,{0x00}},
{0xE2, 16 ,{0x00,0x09,0x0F,0x0E,0x07,0x10,0x0B,0x0A,0x04,0x07,0x0B,0x08,0x0F,0x10,0x0A,0x01}},
{0x00, 1 ,{0x81}},
{0xC1, 1  ,{0x22}},
{0x00, 1 ,{0xa1}},
{0xC1, 1  ,{0x08}},
{0x00, 1 ,{0xa3}},
{0xC0, 1  ,{0x1b}},
{0x00, 1 ,{0x81}},
{0xC4, 1  ,{0x83}},
{0x00, 1 ,{0x92}},
{0xC5, 1  ,{0x01}},
{0x00, 1 ,{0xB1}},
{0xC5, 1  ,{0xA9}},
{0x00, 1 ,{0x80}},
{0xCE, 12 ,{0x85,0x03,0x00,0x84,0x03,0x00,0x83,0x03,0x00,0x82,0x03,0x00}},
{0x00, 1 ,{0xa0}},
{0xCE, 14 ,{0x38,0x02,0x03,0x21,0x00,0x00,0x00,0x38,0x01,0x03,0x22,0x00,0x00,0x00}},                                     
{0x00, 1 ,{0xb0}},
{0xCE, 14 ,{0x38,0x00,0x03,0x23,0x00,0x00,0x00,0x30,0x00,0x03,0x24,0x00,0x00,0x00}},     
{0x00, 1 ,{0xc0}},
{0xCE, 14 ,{0x30,0x01,0x03,0x25,0x00,0x00,0x00,0x30,0x02,0x03,0x26,0x00,0x00,0x00}},                                     
{0x00, 1 ,{0xd0}},
{0xCE, 14 ,{0x30,0x03,0x03,0x27,0x00,0x00,0x00,0x30,0x04,0x03,0x28,0x00,0x00,0x00}},                                     
{0x00, 1 ,{0xc0}},
{0xCF, 10 ,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1 ,{0xd0}},
{0xCF, 1  ,{0x00}},
{0x00, 1 ,{0xc0}},
{0xCB, 15 ,{0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00}},                   
{0x00, 1 ,{0xd0}},
{0xCB, 15 ,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04}},
{0x00, 1 ,{0xe0}},
{0xCB, 10 ,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1 ,{0x80}},
{0xCC, 10 ,{0x00,0x00,0x00,0x00,0x0C,0x0A,0x10,0x0E,0x03,0x04}},                            
{0x00, 1 ,{0x90}},
{0xCC, 15 ,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B}},                            
{0x00, 1 ,{0xa0}},
{0xCC, 15 ,{0x09,0x0F,0x0D,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},                           
{0x00, 1 ,{0xb0}},
{0xCC, 10 ,{0x00,0x00,0x00,0x00,0x0D,0x0F,0x09,0x0B,0x02,0x01}},                           
{0x00, 1 ,{0xc0}},
{0xCC, 15 ,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E}},                            
{0x00, 1 ,{0xd0}},
{0xCC, 15 ,{0x10,0x0A,0x0C,0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

{0x3A, 1 ,{0x77}},

#ifdef DISPLAY_DIRECTION_0_MODE
{0x36, 1 ,{0x00}},// Display Direction 0
{0x35, 1 ,{0x00}},// TE( Fmark ) Signal On
{0x44, 2 ,{0x01,0x10}},// TE( Fmark ) Signal Output Position 0100
#endif

#ifdef DISPLAY_DIRECTION_180_MODE
{0x36, 1 ,{0xD0}},// Display Direction 180
{0x35, 1 ,{0x00}},// TE( Fmark ) Signal On
{0x44, 2 ,{0x01,0xFF}},// TE( Fmark ) Signal Output Position
#endif

#ifdef LCD_BACKLIGHT_CONTROL_MODE
{0x51, 1 ,{0xFF}},// Backlight Level Control
{0x53, 1 ,{0x2C}},// Backlight On
{0x55, 1 ,{0x00}},// CABC Function Off
#endif

{0x11, 1 ,{0x00}}, 
{REGFLAG_DELAY, 150, {}},

{0x29, 1 ,{0x00}}, 
{REGFLAG_DELAY, 200, {}},

//{0x2C, 1 ,{0x00}}, 

// Note
// Strongly recommend not to set Sleep out / Display On here. That will cause messed frame to be shown as later the backlight is on.

// Setting ending by predefined flag
{REGFLAG_END_OF_TABLE, 0x00, {}}

        };



static struct LCM_setting_table lcm_sleep_out_setting[] = {
  // Sleep Out
	{0x11, 1, {0x00}},
  {REGFLAG_DELAY, 120, {}},
  // Display ON
	{0x29, 1, {0x00}},
  {REGFLAG_DELAY, 100, {}},
  
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},
  {REGFLAG_DELAY, 120, {}},
  // Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for (i = 0; i < count; i++) {

        unsigned cmd;
        cmd = table[i].cmd;

        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }

}


static void init_lcm_registers(void)
{
    int id;

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);

}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));

    params->type   = LCM_TYPE_DSI;

    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;

		// enable tearing-free
                params->dbi.te_mode            = LCM_DBI_TE_MODE_VSYNC_ONLY;
                params->dbi.te_edge_polarity   = LCM_POLARITY_RISING;

		params->dsi.mode   = CMD_MODE;
		//params->dsi.mode   = SYNC_EVENT_VDO_MODE;

    // DSI
	/* Command mode setting */
    params->dsi.LANE_NUM=LCM_TWO_LANE;
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		params->dsi.packet_size=256;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 2;
	
                params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		params->dsi.word_count= 480*3;	
		params->dsi.vertical_active_line=800;

		// Video mode setting		
		params->dsi.line_byte=2180;		// 2256 = 752*3
		params->dsi.horizontal_sync_active_byte=26;
		params->dsi.horizontal_backporch_byte=206;
		params->dsi.horizontal_frontporch_byte=206;	
		params->dsi.rgb_byte=(480*3+6);	
	
		params->dsi.horizontal_sync_active_word_count=20;	
		params->dsi.horizontal_backporch_word_count=200;
		params->dsi.horizontal_frontporch_word_count=200;

		// Bit rate calculation
		params->dsi.pll_div1=1;		// fref=26MHz, fvco=fref*(div1+1)	(div1=0~63, fvco=500MHZ~1GHz)  25
		params->dsi.pll_div2=1;			// div2=0~15: fout=fvo/(2*div2
		params->dsi.fbk_div=27;	
}

static void lcm_init(void)
{
#ifdef BUILD_LK
	upmu_set_rg_vgp2_vosel(5);
	upmu_set_rg_vgp2_en(1);

	upmu_set_rg_vgp3_vosel(3);
	upmu_set_rg_vgp3_en(1);	
#else
  hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
	hwPowerOn(MT6323_POWER_LDO_VGP3,VOL_1800,"LCM");
#endif
    MDELAY(100);

    SET_RESET_PIN(1);
    MDELAY(25);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(100);
    init_lcm_registers();

}

static void lcm_suspend(void)
{
    push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
/*    
#if defined(BUILD_LK)
	upmu_set_rg_vgp2_en(0);
	upmu_set_rg_vgp3_en(0);	
#else

	hwPowerDown(MT6323_POWER_LDO_VGP2,"LCM");
	hwPowerDown(MT6323_POWER_LDO_VGP3, "Lance_LCM");
#endif
	MDELAY(50);//Must > 10ms
*/
}

static void lcm_resume(void)
{
#if 1
    push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
#else

#ifndef BUILD_LK 
	lcm_init(); 
#else 
	upmu_set_rg_vgp2_vosel(5);
	upmu_set_rg_vgp2_en(1);

	upmu_set_rg_vgp3_vosel(3);
	upmu_set_rg_vgp3_en(1);	
 	MDELAY(100);
#endif 

#endif
}


static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
    unsigned int x0 = x;
    unsigned int y0 = y;
    unsigned int x1 = x0 + width - 1;
    unsigned int y1 = y0 + height - 1;

    unsigned char x0_MSB = ((x0>>8)&0xFF);
    unsigned char x0_LSB = (x0&0xFF);
    unsigned char x1_MSB = ((x1>>8)&0xFF);
    unsigned char x1_LSB = (x1&0xFF);
    unsigned char y0_MSB = ((y0>>8)&0xFF);
    unsigned char y0_LSB = (y0&0xFF);
    unsigned char y1_MSB = ((y1>>8)&0xFF);
    unsigned char y1_LSB = (y1&0xFF);


    unsigned int data_array[16];


    data_array[0]= 0x00053902;
    data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
    data_array[2]= (x1_LSB);
    dsi_set_cmdq(data_array, 3, 1);

    data_array[0]= 0x00053902;
    data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
    data_array[2]= (y1_LSB);
    dsi_set_cmdq(data_array, 3, 1);

    data_array[0]= 0x002c3909;
    dsi_set_cmdq(data_array, 1, 0);
}

static int get_lcd_id(void)
{
    mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
    mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,1);
    MDELAY(1);

    return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}

static unsigned int lcm_compare_id(void)
{
	unsigned int id=0,id2=0;
	unsigned char buffer[5],buffer2[2];
	unsigned int array[16];  
	
	#ifdef BUILD_LK
	upmu_set_rg_vgp2_vosel(5);
	upmu_set_rg_vgp2_en(1);

	upmu_set_rg_vgp3_vosel(3);
	upmu_set_rg_vgp3_en(1);	
#else
  hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
	hwPowerOn(MT6323_POWER_LDO_VGP3,VOL_1800,"LCM");
#endif
    MDELAY(100);

	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(25);
	SET_RESET_PIN(1);
	MDELAY(50);

	array[0]=0x00043902;
	array[1]=0x010980ff; 
	dsi_set_cmdq(array, 2, 1); //{0xff, 3 ,{0x80,0x09,0x01}}, // Command2 Enable
	
	array[0]=0x80001500;
	dsi_set_cmdq(array, 1, 1); //{0x00, 1 ,{0x80}},
	
	array[0]=0x00033902;
	array[1]=0x000980ff;
	dsi_set_cmdq(array, 2, 1); //{0xff, 2 ,{0x80,0x09}}, // Orise Mode Enable
	
	array[0]=0xC6001500;
	dsi_set_cmdq(array, 1, 1); //{0x00, 1 ,{0xC6}},
	
	array[0]=0x03B01500;
	dsi_set_cmdq(array, 1, 1); //{0xB0, 1 ,{0x03}}, // Read Contention Error
	MDELAY(10);

	array[0] = 0x00053700;// set return byte number
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xA1, &buffer, 5); // Read Register 0xA1 : 0x01,0x8B,0x80,0x09         (OTM8009A/OTM8018B);

	id = buffer[2]<<8 |buffer[3]; 

	Lcd_Log("[LSQ] -- otm8018b 0x%x , 0x%x , 0x%x \n",buffer[2],buffer[3],id);

	array[0] = 0x50001500;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xF8, &buffer2, 2); // Read Register 0xF850 : 0x02,0x35 (OTM8018B); 
	
	id2 = buffer2[0]<<8 |buffer2[1]; 
	Lcd_Log("[LSQ] -- otm8018b 0x%x , 0x%x , 0x%x \n",buffer2[0],buffer2[1],id2);
	
	return ((id2!=0x0235)&&(id==0x8009))?1:0;
}

static unsigned int lcm_esd_recover()
{
    unsigned int data_array[16];

#if defined(BUILD_LK)
    printf("hwj lcm_esd_recover enter");
#else
    printk("hwj lcm_esd_recover enter");
#endif

    lcm_init();

    data_array[0]=0x00110500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(50);

    data_array[0]=0x00290500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0]= 0x00023902;
    data_array[1]= 0xFF51;
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(10);
}


LCM_DRIVER otm8009a_haoshitong_HW397130_HSD_dsi_2_cmd_wvga_lcm_drv = 
{
    .name			= "otm8009a_haoshitong_HW397130_HSD_dsi_2_cmd_wvga",
		.set_util_funcs = lcm_set_util_funcs,
		.get_params     = lcm_get_params,
		.init           = lcm_init,
		.suspend        = lcm_suspend,
		.resume         = lcm_resume,
	  .update         = lcm_update,
	  .compare_id    = lcm_compare_id,
    };
