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
#define FRAME_WIDTH  										(480)
#define FRAME_HEIGHT 										(800)

#define REGFLAG_DELAY           				(0XFEF)
#define REGFLAG_END_OF_TABLE    				(0xFFF)	// END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE								0
#define LCM_RM68190_ID 									(0x8911)
#define GPIO_LCD_ID_PIN 								97
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------
static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    						(lcm_util.set_reset_pin((v)))
#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))
#define LCM_ID       										(0x8011)
static unsigned int lcm_esd_test = 0;		///only for ESD test

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)		lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)				lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)																			lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)									lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg																						lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   						lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size) 
   
#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = {
{0xF0, 5,{0x55,0xaa,0x52,0x08,0x01}},
{0xb6 ,3,{0x34 ,0x34, 0x34}},
{0xB0 ,3,{0x0d ,0x0d, 0x0d}},//09
{0xb7 ,3,{0x34 ,0x34, 0x34}},
{0xB1 ,3,{0x0d ,0x0d, 0x0d}}, 
{0xB8 ,1,{0x24 ,0x24, 0x24}},//{0x34}},
{0xB9 ,3,{0x34 ,0x34, 0x34}},
{0xB3 ,3,{0x08 ,0x08, 0x08}},
{0xBa ,3,{0x14 ,0x14, 0x14}},//24
{0xB5 ,3,{0x08 ,0x08, 0x08}},
{0xBc ,3,{0x00 ,0x88, 0x00}},//78-88
{0xBd ,3,{0x00 ,0x88, 0x00}},
{0xBe ,2,{0x00 ,0x3C}},//30-3C   vcom  flicker
{0xBf ,1,{0x01}},
{0xD1,	52, {0x00,0x03,0x00,0x0C,0x00,0x1B,0x00,0x3D,0x00,0x4F,0x00,0x6C,0x00,0x76,0x00,0x7A,\
               0x00,0x9B,0x00,0xE1,0x01,0x1D,0x01,0x82,0x01,0xD5,0x01,0xD7,0x02,0x13,0x02,0x7D,\
               0x02,0xC2,0x03,0x36,0x03,0x4F,0x03,0x68,0x03,0x81,0x03,0x9A,0x03,0xB3,0x03,0xCC,\
               0x03,0xE5,0x03,0xFF}},
{0xD2,	52, {0x00,0x03,0x00,0x0C,0x00,0x1B,0x00,0x3D,0x00,0x4F,0x00,0x6C,0x00,0x76,0x00,0x7A,\
               0x00,0x9B,0x00,0xE1,0x01,0x1D,0x01,0x82,0x01,0xD5,0x01,0xD7,0x02,0x13,0x02,0x7D,\
               0x02,0xC2,0x03,0x36,0x03,0x4F,0x03,0x68,0x03,0x81,0x03,0x9A,0x03,0xB3,0x03,0xCC,\
               0x03,0xE5,0x03,0xFF}},
{0xD3,	52, {0x00,0x03,0x00,0x0C,0x00,0x1B,0x00,0x3D,0x00,0x4F,0x00,0x6C,0x00,0x76,0x00,0x7A,\
               0x00,0x9B,0x00,0xE1,0x01,0x1D,0x01,0x82,0x01,0xD5,0x01,0xD7,0x02,0x13,0x02,0x7D,\
               0x02,0xC2,0x03,0x36,0x03,0x4F,0x03,0x68,0x03,0x81,0x03,0x9A,0x03,0xB3,0x03,0xCC,\
               0x03,0xE5,0x03,0xFF}},
{0xD4,	52, {0x00,0x03,0x00,0x0C,0x00,0x1B,0x00,0x3D,0x00,0x4F,0x00,0x6C,0x00,0x76,0x00,0x7A,\
               0x00,0x9B,0x00,0xE1,0x01,0x1D,0x01,0x82,0x01,0xD5,0x01,0xD7,0x02,0x13,0x02,0x7D,\
               0x02,0xC2,0x03,0x36,0x03,0x4F,0x03,0x68,0x03,0x81,0x03,0x9A,0x03,0xB3,0x03,0xCC,\
               0x03,0xE5,0x03,0xFF}},
{0xD5,	52, {0x00,0x03,0x00,0x0C,0x00,0x1B,0x00,0x3D,0x00,0x4F,0x00,0x6C,0x00,0x76,0x00,0x7A,\
               0x00,0x9B,0x00,0xE1,0x01,0x1D,0x01,0x82,0x01,0xD5,0x01,0xD7,0x02,0x13,0x02,0x7D,\
               0x02,0xC2,0x03,0x36,0x03,0x4F,0x03,0x68,0x03,0x81,0x03,0x9A,0x03,0xB3,0x03,0xCC,\
               0x03,0xE5,0x03,0xFF}},
{0xD6,	52, {0x00,0x03,0x00,0x0C,0x00,0x1B,0x00,0x3D,0x00,0x4F,0x00,0x6C,0x00,0x76,0x00,0x7A,\
               0x00,0x9B,0x00,0xE1,0x01,0x1D,0x01,0x82,0x01,0xD5,0x01,0xD7,0x02,0x13,0x02,0x7D,\
               0x02,0xC2,0x03,0x36,0x03,0x4F,0x03,0x68,0x03,0x81,0x03,0x9A,0x03,0xB3,0x03,0xCC,\
               0x03,0xE5,0x03,0xFF}},
{0xF0 ,5,{0x55, 0xaa, 0x52, 0x08, 0x00}},
{0xB1 ,1,{0xFC}},{REGFLAG_DELAY, 50, {}},//ADD
{0xB6 ,1,{0x05}},
{0xB7 ,2,{0x70,0x70}},
{0xB8 ,4,{0x01,0x05,0x05,0x05}},
{0xBC ,3,{0x00,0x00,0x00}},
{0xBD,4,{0x01,0x10,0x1c,0x1c}},
{0xCC ,3,{0x03,0x00,0x00}},//50
{0x2A ,4,{0x00, 0x00, 0x01, 0xDF}},//480//ADD
{0x2B ,4,{0x00, 0x00, 0x03, 0x1F}},//854//ADD
{0x35 ,1,{0x00}},//ADD
{0x44 ,2,{0x00, 0x90}},//ADD
{0x11, 0, {0x00}},
{REGFLAG_DELAY, 120, {}},
{0x29, 0, {0x00}},
{REGFLAG_DELAY, 50, {}},
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};



static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 1, {0x00}},
	{REGFLAG_DELAY, 150, {}},
	// Display ON
	{0x29, 1, {0x00}},
	{REGFLAG_DELAY, 200, {}},
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},
    {REGFLAG_DELAY, 150, {}},

	// Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	
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
	 memset(params, 0, sizeof(LCM_PARAMS));
	params->type   = LCM_TYPE_DSI;
	params->width  = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;

	// enable tearing-free
	params->dbi.te_mode 			= LCM_DBI_TE_MODE_DISABLED;
	params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;



	params->dsi.mode   =SYNC_PULSE_VDO_MODE;//SYNC_EVENT_VDO_MODE;


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
	params->dsi.vertical_sync_active				= 2;
	params->dsi.vertical_backporch					= 14;
	params->dsi.vertical_frontporch					= 16;
	params->dsi.vertical_active_line				= FRAME_HEIGHT; 
	params->dsi.horizontal_sync_active				= 10;
	params->dsi.horizontal_backporch				= 60;
	params->dsi.horizontal_frontporch				= 60;
	//params->dsi.horizontal_blanking_pixel		       = 60;
	params->dsi.horizontal_active_pixel		       = FRAME_WIDTH;

    params->dsi.word_count=480*3;	//DSI CMD mode need set these two bellow params, different to 6577
    params->dsi.vertical_active_line=800;
	// Bit rate calculation
	params->dsi.pll_div1=0;		// div1=0,1,2,3;div1_real=1,2,4,4
	params->dsi.pll_div2=1;		// div2=0,1,2,3;div2_real=1,2,4,4
	//params->dsi.fbk_sel=1;		 // fbk_sel=0,1,2,3;fbk_sel_real=1,2,4,4
	params->dsi.fbk_div =17;//12;//9;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	
} 

static void init_lcm_registers(void)
{
	unsigned int data_array[16];
	data_array[0]=0x00063902;       //page 2
	data_array[1]=0x52aa55f0;
	data_array[2]=0x00000208; 	
	dsi_set_cmdq(&data_array, 3, 1);
	
	data_array[0]=0x00063902;
	data_array[1]=0x098000fe;
	data_array[2]=0x0000a008;
	dsi_set_cmdq(&data_array, 3, 1);
	
	data_array[0]=0x00023902;
	data_array[1]=0x000060f6;
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00063902;       //page 0
	data_array[1]=0x52aa55f0;
	data_array[2]=0x00000008; 	
	dsi_set_cmdq(&data_array, 3, 1);
	
	data_array[0]=0x00063902;
	data_array[1]=0x3802d3c9;
	data_array[2]=0x00003838;	
	dsi_set_cmdq(&data_array, 3, 1);
	
	data_array[0]=0x00033902;
	data_array[1]=0x007777b7;	
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00033902;
	data_array[1]=0x008200e8;	
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00033902;
	data_array[1]=0x000e00b0;	
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00023902;
	data_array[1]=0x000052b5;	
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00023902;
	data_array[1]=0x000010b4;	
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00053902;
	data_array[1]=0x030301b8;
	data_array[2]=0x00000003;	
	dsi_set_cmdq(&data_array, 3, 1);
	
	data_array[0]=0x00043902;
	data_array[1]=0x020202bc;	
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00063902;       //page 1
	data_array[1]=0x52aa55f0;
	data_array[2]=0x00000108; 	
	dsi_set_cmdq(&data_array, 3, 1);
	
	data_array[0]=0x00043902;
	data_array[1]=0x0b0b0bb0;
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00043902;
	data_array[1]=0x0b0b0bb1;
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00043902;
	data_array[1]=0x080808b5;
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00043902;
	data_array[1]=0x343434b6;
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00043902;
	data_array[1]=0x343434b7;
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00043902;
	data_array[1]=0x242424b8;
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00043902;
	data_array[1]=0x343434b9;
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00043902;
	data_array[1]=0x040404ba;
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00043902;
	data_array[1]=0x137800bc;
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00043902;
	data_array[1]=0x137800bd;
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00033902;
	data_array[1]=0x006300be;     //vcom
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00353902;
	data_array[1]=0x000000d1;
	data_array[2]=0x006b000a;
	data_array[3]=0x01cf00a3;
	data_array[4]=0x01330109;
	data_array[5]=0x019b016f;
	data_array[6]=0x020402d8;
	data_array[7]=0x02790246;
	data_array[8]=0x02a7027b;
	data_array[9]=0x03ee02d4;
	data_array[10]=0x031e030c;
	data_array[11]=0x03430334;
	data_array[12]=0x03640357;
	data_array[13]=0x03a30379;
	data_array[14]=0x000000ff;	
	dsi_set_cmdq(&data_array, 15, 1);
	
	data_array[0]=0x00353902;
	data_array[1]=0x000000d2;
	data_array[2]=0x006b000a;
	data_array[3]=0x01cf00a3;
	data_array[4]=0x01330109;
	data_array[5]=0x019b016f;
	data_array[6]=0x020402d8;
	data_array[7]=0x02790246;
	data_array[8]=0x02a7027b;
	data_array[9]=0x03ee02d4;
	data_array[10]=0x031e030c;
	data_array[11]=0x03430334;
	data_array[12]=0x03640357;
	data_array[13]=0x03a30379;
	data_array[14]=0x000000ff;	
	dsi_set_cmdq(&data_array, 15, 1);
	
	data_array[0]=0x00353902;
	data_array[1]=0x000000d3;
	data_array[2]=0x006b000a;
	data_array[3]=0x01cf00a3;
	data_array[4]=0x01330109;
	data_array[5]=0x019b016f;
	data_array[6]=0x020402d8;
	data_array[7]=0x02790246;
	data_array[8]=0x02a7027b;
	data_array[9]=0x03ee02d4;
	data_array[10]=0x031e030c;
	data_array[11]=0x03430334;
	data_array[12]=0x03640357;
	data_array[13]=0x03a30379;
	data_array[14]=0x000000ff;	
	dsi_set_cmdq(&data_array, 15, 1);
	
	data_array[0]=0x00353902;
	data_array[1]=0x000000d4;
	data_array[2]=0x006b000a;
	data_array[3]=0x01cf00a3;
	data_array[4]=0x01330109;
	data_array[5]=0x019b016f;
	data_array[6]=0x020402d8;
	data_array[7]=0x02790246;
	data_array[8]=0x02a7027b;
	data_array[9]=0x03ee02d4;
	data_array[10]=0x031e030c;
	data_array[11]=0x03430334;
	data_array[12]=0x03640357;
	data_array[13]=0x03a30379;
	data_array[14]=0x000000ff;	
	dsi_set_cmdq(&data_array, 15, 1);
	
	data_array[0]=0x00353902;
	data_array[1]=0x000000d5;
	data_array[2]=0x006b000a;
	data_array[3]=0x01cf00a3;
	data_array[4]=0x01330109;
	data_array[5]=0x019b016f;
	data_array[6]=0x020402d8;
	data_array[7]=0x02790246;
	data_array[8]=0x02a7027b;
	data_array[9]=0x03ee02d4;
	data_array[10]=0x031e030c;
	data_array[11]=0x03430334;
	data_array[12]=0x03640357;
	data_array[13]=0x03a30379;
	data_array[14]=0x000000ff;	
	dsi_set_cmdq(&data_array, 15, 1);
	
	data_array[0]=0x00353902;
	data_array[1]=0x000000d6;
	data_array[2]=0x006b000a;
	data_array[3]=0x01cf00a3;
	data_array[4]=0x01330109;
	data_array[5]=0x019b016f;
	data_array[6]=0x020402d8;
	data_array[7]=0x02790246;
	data_array[8]=0x02a7027b;
	data_array[9]=0x03ee02d4;
	data_array[10]=0x031e030c;
	data_array[11]=0x03430334;
	data_array[12]=0x03640357;
	data_array[13]=0x03a30379;
	data_array[14]=0x000000ff;	
	dsi_set_cmdq(&data_array, 15, 1);
	
	data_array[0]=0x00023902;
	data_array[1]=0x00000036;	
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00023902;
	data_array[1]=0x00000035;	
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0]=0x00023902;
	data_array[1]=0x0000773a;	
	dsi_set_cmdq(&data_array, 2, 1);
	
	data_array[0] = 0x00110500; 
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(120);
	
	data_array[0] = 0x00290500; 
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(30);
}

static void lcm_init(void)
{
	#if defined(BUILD_LK)
		upmu_set_rg_vgp2_vosel(5);
		upmu_set_rg_vgp2_en(1);
		
		upmu_set_rg_vgp3_vosel(3);
		upmu_set_rg_vgp3_en(1);	
#else
		hwPowerOn(MT6323_POWER_LDO_VGP2, VOL_2800, "Lance_LCM");
		hwPowerOn(MT6323_POWER_LDO_VGP3, VOL_1800, "Lance_LCM");
#endif
  SET_RESET_PIN(1);
  SET_RESET_PIN(0);
  MDELAY(50);//Must > 5ms
  SET_RESET_PIN(1);
  MDELAY(120);//Must > 50ms

	Lcd_Log("wqcat RM68171_zhuoershi_Z40252N00I4OMA_BOE_DSI_2_WVGA_VDO lcm init\n");
	
	//init_lcm_registers();
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
  SET_RESET_PIN(0);
	MDELAY(80); //10  50 
}


static void lcm_resume(void)
{
	lcm_init();
	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}

static int get_lcd_id(void)
{
  mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
  mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
  mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
  mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,1);
  MDELAY(1);

	Lcd_Log("wqcat %s, get_lcd_id=%d\n", __func__, mt_get_gpio_in(GPIO_LCD_ID_PIN));
    
  return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}

static unsigned int lcm_compare_id(void)
{
	#if defined(BUILD_LK)
		upmu_set_rg_vgp2_vosel(5);
		upmu_set_rg_vgp2_en(1);
		
		upmu_set_rg_vgp3_vosel(3);
		upmu_set_rg_vgp3_en(1);	
#else
		hwPowerOn(MT6323_POWER_LDO_VGP2, VOL_2800, "Lance_LCM");
		hwPowerOn(MT6323_POWER_LDO_VGP3, VOL_1800, "Lance_LCM");
#endif
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

	Lcd_Log("wqcat %s, id1 = 0x%08x, id2 = 0x%08x\n", __func__, buffer[0], buffer[1]);

  return (id==LCM_ID&&get_lcd_id()==0)?1:0;
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
	 
	printk("mycat$$ huarui ###*** esd check buffer[0] = 0x%x\r\n",buffer[0]);
	
	if(buffer[0] == 0x9C)//9C
		return 0;
	else
		return 1;
 			
#endif
}

static unsigned int lcm_esd_recover(void)
{
  lcm_init();
	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
	
	Lcd_Log("mycat songtuirui ### lcm_esd_recover \r\n");
	
	return 1;
}
#endif


LCM_DRIVER RM68171_QICAI_K40BM231423A1N_BOE_DSI_2_WVGA_VDO_lcm_drv = 
{
	.name						= "RM68171_QICAI_K40BM231423A1N_BOE_DSI_2_WVGA_VDO",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id    	= lcm_compare_id,
#if defined (BIRD_ESD_CHECK)
	.esd_check 			= lcm_esd_check,
  .esd_recover 		= lcm_esd_recover,
#endif
};

