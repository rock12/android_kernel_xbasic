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

#define FRAME_WIDTH  										(540)
#define FRAME_HEIGHT 										(960)

#define REGFLAG_DELAY           				(0XFE)
#define REGFLAG_END_OF_TABLE    				(0xFFF)	// END OF REGISTERS MARKER
static unsigned int lcm_esd_test = 0;      ///only for ESD test

#define LCM_DSI_CMD_MODE									0
#define LCM_RM68191_ID 						(0x8911)  //8191,  c5h
//#define GPIO_LCD_ID_PIN 								97
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_compare_id_setting[] = {
	// Display off sequence
	{0xf0, 5, {0x55, 0xaa, 0x52, 0x08, 0x01}},
	{REGFLAG_DELAY, 10, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
static struct LCM_setting_table lcm_initialization_setting[] = {

//RM68191  IVO5.43
//RM68191  IVO5.43
	{0xF0,  5, {0x55,0xAA,0x52,0x08,0x03}},
	{0x90,  9, {0x05,0x16,0x09,0x03,0xCD,0x00,0x8C,0x00,0x00}},
	{0x91,  9, {0x05,0x16,0x0B,0x03,0xCF,0x00,0x8C,0x00,0x00}}, 
	{0x92, 11, {0x40,0x0B,0x0C,0x0D,0x0E,0x00,0x54,0x00,0x00,0x05,0x08}}, 
	{0x94,  8, {0x00,0x08,0x0B,0x03,0xD0,0x03,0xD2,0x0C}}, 
	{0x95, 16, {0x40,0x0F,0x00,0x10,0x00,0x11,0x00,0x12,0x00,0x54,0x00,0x00,0x00,0x05,0x00,0x08}}, 
	{0x99,  2, {0x00,0x00}},
	{0x9A, 11, {0x80,0x0E,0x03,0xD4,0x03,0xD6,0x00,0x00,0x00,0x00,0x50}}, 
	{0x9B,  6, {0x00,0x00,0x00,0x00,0x00,0x00}}, 
	{0x9C,  2, {0x00,0x00}},
	{0x9D,  8, {0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00}}, 
	{0x9E,  2, {0x00,0x00}},
	{0xA0, 10, {0x9F,0x1F,0x1F,0x1F,0x04,0x1F,0x00,0x1F,0x08,0x1F}}, 
	{0xA1, 10, {0x0A,0x1F,0x0C,0x1F,0x0E,0x1F,0x02,0x1F,0x06,0x1F}}, 
	{0xA2, 10, {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}}, 
	{0xA3, 10, {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}},  
	{0xA4, 10, {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}}, 
	{0xA5, 10, {0x1F,0x07,0x1F,0x03,0x1F,0x0F,0x1F,0x0D,0x1F,0x0B}},   
	{0xA6, 10, {0x1F,0x09,0x1F,0x01,0x1F,0x05,0x1F,0x1F,0x1F,0x1F}}, 
	{0xA7, 10, {0x1F,0x1F,0x1F,0x1F,0x03,0x1F,0x07,0x1F,0x0B,0x1F}}, 
	{0xA8, 10, {0x09,0x1F,0x0F,0x1F,0x0D,0x1F,0x05,0x1F,0x01,0x1F}}, 
	{0xA9, 10, {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}}, 
	{0xAA, 10, {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}}, 
	{0xAB, 10, {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}},   
	{0xAC, 10, {0x1F,0x00,0x1F,0x04,0x1F,0x0C,0x1F,0x0E,0x1F,0x08}}, 
	{0xAD, 10, {0x1F,0x0A,0x1F,0x06,0x1F,0x02,0x1F,0x1F,0x1F,0x1F}},   
	{0xF0,  5, {0x55,0xAA,0x52,0x08,0x00}},  
	{0xB1,  1, {0xFC}},    
	{0xBC,  3, {0x00,0x00,0x00}}, 
	{0xB8,  4, {0x01,0x82,0xB2,0x80}}, 
	{0xF0,  5, {0x55,0xAA,0x52,0x08,0x01}},
	{0xB0,  3, {0x05,0x05,0x05}},
	{0xB1,  3, {0x05,0x05,0x05}},
	{0xB6,  3, {0x44,0x44,0x44}},
	{0xB7,  3, {0x34,0x34,0x34}},  
	{0xB3,  3, {0x10,0x10,0x10}}, 
	{0xB9,  3, {0x34,0x34,0x34}},   
	{0xB4,  3, {0x06,0x06,0x06}}, 
	{0xBA,  3, {0x14,0x14,0x14}},   
	{0xBC,  3, {0x00,0x70,0x00}}, 
	{0xBD,  3, {0x00,0x70,0x00}}, 
	{0xBE,  1, {0x40}},//7d
	{0xD1, 16, {0x00,0x00,0x00,0x06,0x00,0x12,0x00,0x1E,0x00,0x28,0x00,0x3D,0x00,0x50,0x00,0x73}}, 
	{0xD2, 16, {0x00,0x92,0x00,0xC8,0x00,0xF6,0x01,0x43,0x01,0x86,0x01,0x88,0x01,0xCB,0x02,0x1C}}, 
	{0xD3, 16, {0x02,0x50,0x02,0x9B,0x02,0xCE,0x03,0x11,0x03,0x3C,0x03,0x71,0x03,0x92,0x03,0xB6}}, 
	{0xD4,  4, {0x03,0xD6,0x03,0xFF}},
	{0xD5, 16, {0x00,0x00,0x00,0x06,0x00,0x12,0x00,0x1E,0x00,0x28,0x00,0x3D,0x00,0x50,0x00,0x73}}, 
	{0xD6, 16, {0x00,0x92,0x00,0xC8,0x00,0xF6,0x01,0x43,0x01,0x86,0x01,0x88,0x01,0xCB,0x02,0x1C}}, 
	{0xD7, 16, {0x02,0x50,0x02,0x9B,0x02,0xCE,0x03,0x11,0x03,0x3C,0x03,0x71,0x03,0x92,0x03,0xB6}}, 
	{0xD8,  4, {0x03,0xD6,0x03,0xFF}},
	{0xD9, 16, {0x00,0x00,0x00,0x06,0x00,0x12,0x00,0x1E,0x00,0x28,0x00,0x3D,0x00,0x50,0x00,0x73}}, 
	{0xDD, 16, {0x00,0x92,0x00,0xC8,0x00,0xF6,0x01,0x43,0x01,0x86,0x01,0x88,0x01,0xCB,0x02,0x1C}}, 
	{0xDE, 16, {0x02,0x50,0x02,0x9B,0x02,0xCE,0x03,0x11,0x03,0x3C,0x03,0x71,0x03,0x92,0x03,0xB6}}, 
	{0xDF,  4, {0x03,0xD6,0x03,0xFF}},
	{0xE0, 16, {0x00,0x00,0x00,0x06,0x00,0x12,0x00,0x1E,0x00,0x28,0x00,0x3D,0x00,0x50,0x00,0x73}}, 
	{0xE1, 16, {0x00,0x92,0x00,0xC8,0x00,0xF6,0x01,0x43,0x01,0x86,0x01,0x88,0x01,0xCB,0x02,0x1C}}, 
	{0xE2, 16, {0x02,0x50,0x02,0x9B,0x02,0xCE,0x03,0x11,0x03,0x3C,0x03,0x71,0x03,0x92,0x03,0xB6}}, 
	{0xE3,  4, {0x03,0xD6,0x03,0xFF}},
	{0xE4, 16, {0x00,0x00,0x00,0x06,0x00,0x12,0x00,0x1E,0x00,0x28,0x00,0x3D,0x00,0x50,0x00,0x73}}, 
	{0xE5, 16, {0x00,0x92,0x00,0xC8,0x00,0xF6,0x01,0x43,0x01,0x86,0x01,0x88,0x01,0xCB,0x02,0x1C}}, 
	{0xE6, 16, {0x02,0x50,0x02,0x9B,0x02,0xCE,0x03,0x11,0x03,0x3C,0x03,0x71,0x03,0x92,0x03,0xB6}}, 
	{0xE7,  4, {0x03,0xD6,0x03,0xFF}},
	{0xE8, 16, {0x00,0x00,0x00,0x06,0x00,0x12,0x00,0x1E,0x00,0x28,0x00,0x3D,0x00,0x50,0x00,0x73}}, 
	{0xE9, 16, {0x00,0x92,0x00,0xC8,0x00,0xF6,0x01,0x43,0x01,0x86,0x01,0x88,0x01,0xCB,0x02,0x1C}}, 
	{0xEA, 16, {0x02,0x50,0x02,0x9B,0x02,0xCE,0x03,0x11,0x03,0x3C,0x03,0x71,0x03,0x92,0x03,0xB6}}, 
	{0xEB,  4, {0x03,0xD6,0x03,0xFF}},
	{0xF0,  5, {0x55,0xAA,0x52,0x08,0x02}},
	{0xF6,  2, {0xCA,0x69}},
	{0xFB,  3, {0x09,0x03,0x08}},
	{0xF1,  3, {0x22,0x22,0x32}},

	{0x4C,  1, {0x11}},

	{0x35,  1, {0x00}},
	{0x36,  1, {0x00}},
	{0x3A,  1, {0x77}},
	{0x11,  1, {0x00}}, 
	{REGFLAG_DELAY, 300, {}},
	{0x29,  1, {0x00}}, 
	{REGFLAG_DELAY, 300, {}},
	{0x2C,  1, {0x00}}, 

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};



static struct LCM_setting_table lcm_sleep_out_setting[] = {
	

    // Sleep Out
	{0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 0, {0x00}},
    {REGFLAG_DELAY, 100, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
	
	
	
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	
	// Display off sequence
	{0x28, 0, {0x00}},
   // {REGFLAG_DELAY, 100, {}},
    // Sleep Mode On
	{0x11, 0, {0x00}},
	{REGFLAG_DELAY, 5, {}},
	
	{0x4F,  1, {0x01}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
	
	
	
};






static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) {
		
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
				//UDELAY(5);//soso add or it will fail to send register
       	}
    }
	
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
	#if 0
	memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

    // enable tearing-free
    params->dbi.te_mode 				= LCM_DBI_TE_MODE_DISABLED;  //LCM_DBI_TE_MODE_VSYNC_ONLY;
    params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
    params->dsi.mode   = CMD_MODE;
#else
    params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif
	
		// DSI
		/* Command mode setting */
		params->dsi.LANE_NUM				= LCM_TWO_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		// Not support in MT6573
		params->dsi.packet_size=256;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 2;

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

		params->dsi.vertical_sync_active = 5;
		params->dsi.vertical_backporch = 5;
		params->dsi.vertical_frontporch = 5;
		params->dsi.vertical_active_line = FRAME_HEIGHT;
		
		params->dsi.horizontal_sync_active = 20;
		params->dsi.horizontal_backporch = 46;
		params->dsi.horizontal_frontporch = 31;	//21;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
		
		// Bit rate calculation
		params->dsi.pll_div1 = 0;	// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		params->dsi.pll_div2 = 1;	// div2=0,1,2,3;div1_real=1,2,4,4
		params->dsi.fbk_div = 14;	//15;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)

	#else
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

    // enable tearing-free
    params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY; //LCM_DBI_TE_MODE_DISABLED;  //
    params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
    params->dsi.mode   = CMD_MODE;
#else
    params->dsi.mode   = BURST_VDO_MODE;  //EVENT    SYNC_PULSE_VDO_MODE
#endif
	
		// DSI
		/* Command mode setting */
		params->dsi.LANE_NUM				= LCM_TWO_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		// Not support in MT6573
	//	params->dsi.packet_size=256;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 2;

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		
		
		//params->dsi.word_count =FRAME_WIDTH*3;
		//params->dsi.vertical_active_line =FRAME_HEIGHT;

		params->dsi.vertical_sync_active = 2;
		params->dsi.vertical_backporch = 16;
		params->dsi.vertical_frontporch = 16;
		params->dsi.vertical_active_line = FRAME_HEIGHT;
		
		params->dsi.horizontal_sync_active = 2;
		params->dsi.horizontal_backporch = 16;
		params->dsi.horizontal_frontporch = 16;	//21;
		params->dsi.horizontal_blanking_pixel = 40;	//21;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
		
		// Bit rate calculation
		params->dsi.pll_div1 = 0;	// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		params->dsi.pll_div2 = 1;	// div2=0,1,2,3;div1_real=1,2,4,4
		params->dsi.fbk_div = 15;	//15;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)
		
		#endif

		
		/* ESD or noise interference recovery For video mode LCM only. */ // Send TE packet to LCM in a period of n frames and check the response.
		params->dsi.lcm_int_te_monitor = FALSE;
		params->dsi.lcm_int_te_period = 1; // Unit : frames
		
		// Need longer FP for more opportunity to do int. TE monitor applicably.
		if (params->dsi.lcm_int_te_monitor)
		params->dsi.vertical_frontporch *= 2;
		
		// Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.)
		params->dsi.lcm_ext_te_monitor = FALSE;  // 1
		// Non-continuous clock
		params->dsi.noncont_clock = TRUE;
		params->dsi.noncont_clock_period = 2; // Unit : frames
}

static unsigned int lcm_compare_id(void);

static void lcm_init(void)
{
    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(20);//Must > 5ms
    SET_RESET_PIN(1);
    MDELAY(120);//Must > 50ms
		//lcm_compare_id();
#if defined(BUILD_LK)
	printf("wqcat RM68191_xingliangda_XLD0550110V1_25_IVO_dsi_2_vdo_qhd lcm init\n");
#endif

#ifndef BUILD_LK
	printk("wqcat RM68191_xingliangda_XLD0550110V1_25_IVO_dsi_2_vdo_qhd lcm init\n");
#endif
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_resume(void)
{
	lcm_init();
	
	push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}

/*static int get_lcd_id(void)
{
    mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
    mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,1);
    MDELAY(1);
#if defined(BUILD_LK)
	printf("wqcat %s, get_lcd_id=%d\n", __func__, mt_get_gpio_in(GPIO_LCD_ID_PIN));
#endif

#ifndef BUILD_LK
	printk("wqcat %s, get_lcd_id=%d\n", __func__, mt_get_gpio_in(GPIO_LCD_ID_PIN));
#endif
    return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}*/

static unsigned int lcm_compare_id(void)
{
	unsigned int id = 0;
	unsigned char buffer[3];
	unsigned int array[16];
	
	SET_RESET_PIN(1);  //NOTE:should reset LCM firstly
	SET_RESET_PIN(0);
	MDELAY(6);
	SET_RESET_PIN(1);
	MDELAY(50);

	array[0] = 0x00033700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0x04, buffer, 3);
	id = ((buffer[1]<<8)&0xFF00)|(buffer[0]&0x00FF); //we only need ID
#if defined(BUILD_LK)
	printf("wqcat %s, id1 = 0x%08x, id2 = 0x%08x\n", __func__, buffer[0], buffer[1]);
#endif

#ifndef BUILD_LK
	printk("wqcat %s, id1 = 0x%08x, id2 = 0x%08x\n", __func__, buffer[0], buffer[1]);
#endif
  return (id==LCM_RM68191_ID)?1:0;
}


#if defined (BIRD_ESD_CHECK)
static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK
	static int ok_count = 0;
	 unsigned char buffer[12];
	 unsigned int array[16];
	 int i;
	 
	if(lcm_esd_test)
  {
      lcm_esd_test = 0;
      return 1;
  }
	 	
	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);
	//MDELAY(1);
	read_reg_v2(0x0A, buffer, 1);
	 
	printk("mycat$$ jingqi ###*** esd check buffer[0] = 0x%x\r\n",buffer[0]);
	
	if(buffer[0] == 0x9C)//9C
		return 0;
	else
		return 1;
 			
#endif
}

static unsigned int lcm_esd_recover(void)
{
  SET_RESET_PIN(1);
  SET_RESET_PIN(0);
  MDELAY(50);//Must > 5ms
  SET_RESET_PIN(1);
  MDELAY(120);//Must > 50ms
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
	
#ifndef BUILD_LK	
	printk("mycat jingqi ### lcm_esd_recover \r\n");
#endif
	
	return 1;
}
#endif



LCM_DRIVER RM68191_xingliangda_XLD0550110V1_25_IVO_dsi_2_vdo_qhd_lcm_drv = 
{
    .name			= "RM68191_xingliangda_XLD0550110V1_25_IVO_dsi_2_vdo_qhd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id    = lcm_compare_id,
#if defined (BIRD_ESD_CHECK)//rm esd resolve LCD  flick
  .esd_check = lcm_esd_check,
  .esd_recover = lcm_esd_recover,
#endif
};

