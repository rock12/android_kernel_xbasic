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

static unsigned int lcm_compare_id(void);

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(480)
#define FRAME_HEIGHT 										(800)

#define REGFLAG_DELAY             							0xFFFE
#define REGFLAG_END_OF_TABLE      							0xFFFF   // END OF REGISTERS MARKER

#define GPIO_LCD_ID_PIN 97

#define LCM_ID	0x01


#define LCM_DSI_CMD_MODE									0

#ifndef TRUE
    #define   TRUE     1
#endif
 
#ifndef FALSE
    #define   FALSE    0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

#define LCM_ID       (0x8369)


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

static unsigned int lcm_compare_id();



static struct LCM_setting_table {
    unsigned char cmd;
    unsigned char count;
    unsigned char para_list[64];
};

#define DISPLAY_DIRECTION_0_MODE

static struct LCM_setting_table lcm_initialization_setting[] = {
	
	/*
	Note :

	Data ID will depends on the following rule.
	
		count of parameters > 1	=> Data ID = 0x39
		count of parameters = 1	=> Data ID = 0x15
		count of parameters = 0	=> Data ID = 0x05

	Structure Format :

	{DCS command, count of parameters, {parameter list}}
	{REGFLAG_DELAY, milliseconds of time, {}},

	...

	Setting ending by predefined flag
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
	*/
		{0xB9,	3,	{0xFF, 0x83, 0x69}},
	
	{0xBA,	15,	{0x31, 0x00, 0x16, 0xCA,
						 0xB1, 0x0A, 0x00, 0x10,
						 0x28, 0x02, 0x21, 0x21,
						 0x9A, 0x1A, 0x8F}},
						 
	
	{0xD5,	92, {0x00, 0x00, 0x0A, 0x00,
						 0x0C, 0x00, 0x00, 0x10,
						 0x01, 0x00, 0x00, 0x00,
						 0x01, 0x50, 0x37, 0x00,
						 0x00, 0x0C, 0x0A, 0x0B,
						 0x47, 0x00, 0x00, 0x00,
						 0x60, 0x00, 0x00, 0x00,
						 0x00, 0x00, 0x00, 0x00,
						 0x03, 0x00, 0x00, 0x26,
						 0x00, 0x00, 0x91, 0x13,
						 0x35, 0x57, 0x75, 0x18,
						 0x00, 0x00, 0x00, 0x86,
						 0x64, 0x42, 0x20, 0x00,
						 0x49, 0x00, 0x00, 0x00,
						 0x90, 0x02, 0x24, 0x46,
						 0x64, 0x08, 0x00, 0x00,
						 0x00, 0x87, 0x75, 0x53,
						 0x31, 0x11, 0x59, 0x00,
						 0x00, 0x00, 0x00, 0x01,
						 0x00, 0x00, 0x00, 0x0F,
						 0x00, 0x0F, 0xFF, 0xFF,
						 0x0F, 0x00, 0x0F, 0xFF,
						 0xFF, 0x00, 0x80, 0x5A}},

	
	{0xB1,	10,	{0x0C, 0x83, 0x77, 0x00,
						 0x0F, 0x0F, 0x18, 0x18,
						 0x0C, 0x02}},

	
	{0xB3,	4,	{0x83, 0x00, 0x31, 0x03,}},


	{0xB4, 1, {0x02}},


	{0xB6,	2,	{0xB1, 0xB1}},

	
	{0xCC,	1,	{0x06}},

	
	{0xC6,	3,	{0x41, 0xFF, 0x7A}},

	
	{0xE3,	4,	{0x07, 0x0F, 0x07, 0x0F,}},

	
	{0xC0,	4,	{0x73, 0x50, 0x00, 0x34,
						0xC4, 0x09}},

	
	{0xC1, 127, {0x03, 0x00, 0x08, 0x10,
						0x18, 0x20, 0x29, 0x30,
						0x37, 0x41, 0x48, 0x50,
						0x58, 0x60, 0x68, 0x70,
						0x78, 0x80, 0x88, 0x90,
						0x98, 0xA0, 0xA8, 0xB0,
						0xB8, 0xC0, 0xC8, 0xCF,
						0xD7, 0xDF, 0xE7, 0xEF,
						0xF8, 0xFF, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00,
						0x08, 0x10, 0x18, 0x20,
						0x29, 0x30, 0x37, 0x41,
						0x48, 0x50, 0x58, 0x60,
						0x68, 0x70, 0x78, 0x80,
						0x88, 0x90, 0x98, 0xA0,
						0xA8, 0xB0, 0xB8, 0xC0,
						0xC8, 0xCF, 0xD7, 0xDF,
						0xE7, 0xEF, 0xF8, 0xFF,
						0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x08, 0x10,
						0x18, 0x20, 0x29, 0x30,
						0x37, 0x41, 0x48, 0x50,
						0x58, 0x60, 0x68, 0x70,
						0x78, 0x80, 0x88, 0x90,
						0x98, 0xA0, 0xA8, 0xB0,
						0xB8, 0xC0, 0xC8, 0xCF,
						0xD7, 0xDF, 0xE7, 0xEF,
						0xF8, 0xFF, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00}},

	// SET GAMMA
	{0xE0,	37,	{0x00, 0x0A, 0x0E, 0x31,
						 0x32, 0x3F, 0x20, 0x3E,
						 0x08, 0x0D, 0x0F, 0x13,
						 0x15, 0x14, 0x15, 0x11,
						 0x17, 0x00, 0x0A, 0x0E,
						 0x31, 0x32, 0x3F, 0x20,
						 0x3E, 0x08, 0x0D, 0x0F,
						 0x13, 0x15, 0x14, 0x15,
						 0x11, 0x17, 0x01}},
						 
	
	{0x36,	1,	{0x40}},
	{0x3A,	1,	{0x77}},
	


	{0x11,0,{}},
	{REGFLAG_DELAY, 120, {}},
	{0x29,0,{}}
};

/*

static struct LCM_setting_table lcm_set_window[] = {
            {0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
            {0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
            {REGFLAG_END_OF_TABLE, 0x00, {}}
        };


static struct LCM_setting_table lcm_sleep_out_setting[] = {

            // Sleep Out
            {0x11, 1, {0x00}},
            {REGFLAG_DELAY, 120, {}},

            // Display ON
            {0x29, 1, {0x00}},
            {REGFLAG_DELAY, 20, {}},

            {REGFLAG_END_OF_TABLE, 0x00, {}}
        };


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
            // Display off sequence
            {0x28, 1, {0x00}},
            {REGFLAG_DELAY, 50, {}},

            // Sleep Mode On
            {0x10, 1, {0x00}},
            {REGFLAG_DELAY, 100, {}},

            {REGFLAG_END_OF_TABLE, 0x00, {}}
        };

static struct LCM_setting_table lcm_compare_id_setting[] = {

            {0x00,1,{0x00}},
            {0xff,3,{0x80,0x09,0x01}},
            {0x00,1,{0x80}},
            {0xff,2,{0x80,0x09}},
        };

static struct LCM_setting_table lcm_backlight_level_setting[] = {
	{0x51, 1, {0xFF}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
*/

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
            //MDELAY(1);//MDELAY(10);
        }
    }
	
}

static void init_lcm_registers(void)
{
#if 0
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
#else
					/*ren_guan_20130722*/
		unsigned int data_array[25];         
        data_array[0]= 0x00043902; 
        data_array[1]= 0x6983FFB9; 
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1);   
    
        data_array[0]= 0x00103902; 
        data_array[1]= 0x160031BA; 
        data_array[2]= 0x000AB1CA; 
        data_array[3]= 0x21022810; 
        data_array[4]= 0x8F1A9A21; 
        dsi_set_cmdq(&data_array, 5, 1); 
        MDELAY(1); 

        data_array[0]= 0x00023902; 
        data_array[1]= 0x0000703A; 
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1);       

        data_array[0]= 0x005D3902; 
        data_array[1]= 0x080000d5; 
        data_array[2]= 0x00000A00; 
        data_array[3]= 0x00000110; 
        data_array[4]= 0x37490100; 
        data_array[5]= 0x0A0A0000; 
        data_array[6]= 0x0000470B; 
        data_array[7]= 0x00006000; 
				data_array[8]= 0x00000000; 
				data_array[9]= 0x00000300; 
				data_array[10]= 0x91000026; 
				data_array[11]= 0x75573513; 
				data_array[12]= 0x00000018; 
				data_array[13]= 0x20426486; 
				data_array[14]= 0x00004900; 
				data_array[15]= 0x24029000; 
				data_array[16]= 0x00086446; 
				data_array[17]= 0x75870000;// 
				data_array[18]= 0x59113153; 
				data_array[19]= 0x00000000; 
				data_array[20]= 0x00000001; 
				data_array[21]= 0xFF0F000F; 
				data_array[22]= 0x0F000FFF; 
				data_array[23]= 0x8000FFFF; 
				data_array[24]= 0x0000005A; 
        dsi_set_cmdq(&data_array, 25, 1); 
        MDELAY(1); 

        data_array[0]= 0x000B3902; 
        data_array[1]= 0x77830Cb1; 
        data_array[2]= 0x180f0F00; 
        data_array[3]= 0x00020C18; 
        dsi_set_cmdq(&data_array, 4, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x00033902; 
        data_array[1]= 0x007000b2; 

        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 
    
        data_array[0]= 0x00053902; 
        data_array[1]= 0x310083b3; 
        data_array[2]= 0x00000003; 
        dsi_set_cmdq(&data_array, 3, 1); 
        MDELAY(1);   
    
        data_array[0]= 0x00023902; 
        data_array[1]= 0x000000b4; 
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x00033902; 
        data_array[1]= 0x009a9ab6;//a0 
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 

        data_array[0]= 0x00023902; 
        data_array[1]= 0x00006DCB; 
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 
  
        data_array[0]= 0x00023902; 
        data_array[1]= 0x000002cc; //0e
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x00043902; 
        data_array[1]= 0x7AFF41C6; 
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x00043902; 
        data_array[1]= 0x000072EA; 
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x00043902; 
        data_array[1]= 0x070F07E3; 
        data_array[2]= 0x0000000F; 
        dsi_set_cmdq(&data_array, 3, 1); 
        MDELAY(1); 

        data_array[0]= 0x00233902; 
        data_array[1]= 0x0C0700e0; 
        data_array[2]= 0x1C3f3230; 
        data_array[3]= 0x100D083A; 
        data_array[4]= 0x15141614; 
        data_array[5]= 0x0700120E; 
        data_array[6]= 0x3f32300C; 
        data_array[7]= 0x0D083A1C;//334b0913 
        data_array[8]= 0x14161410; 
        data_array[9]= 0x01120E15; 
        dsi_set_cmdq(&data_array, 10, 1); 
        MDELAY(1); 
        

        data_array[0] = 0x00110500;        //exit sleep mode 
        dsi_set_cmdq(&data_array, 1, 1); 
        MDELAY(150);   

        data_array[0] = 0x00290500;        //exit sleep mode 
        dsi_set_cmdq(&data_array, 1, 1); 
        MDELAY(20); 
#endif
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
		params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;//LCM_DBI_TE_MODE_DISABLED;
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


		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

#if (LCM_DSI_CMD_MODE)
		params->dsi.intermediat_buffer_num = 0;//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage
		params->dsi.word_count=FRAME_WIDTH*3;	//DSI CMD mode need set these two bellow params, different to 6577
		params->dsi.vertical_active_line=FRAME_HEIGHT;
#else
		// Video mode setting		
		params->dsi.intermediat_buffer_num = 2;

		// Highly depends on LCD driver capability.
		// Not support in MT6573
		params->dsi.packet_size=256;

		params->dsi.vertical_sync_active				= 2;//10;
		params->dsi.vertical_backporch					= 22;//50;
		params->dsi.vertical_frontporch					= 20;//50;
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 32;//10;
		params->dsi.horizontal_backporch				= 135;//60;
		params->dsi.horizontal_frontporch				= 150;//60;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
#endif

    // Bit rate calculation
	#ifdef CONFIG_MT6589_FPGA
	    params->dsi.pll_div1=2;		// div1=0,1,2,3;div1_real=1,2,4,4
	    params->dsi.pll_div2=2;		// div2=0,1,2,3;div2_real=1,2,4,4
	    params->dsi.fbk_div =8;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)
	#else
	    params->dsi.pll_div1=2;		// div1=0,1,2,3;div1_real=1,2,4,4
	    params->dsi.pll_div2=0;		// div2=0,1,2,3;div2_real=1,2,4,4
	    params->dsi.fbk_div =35;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)		
	#endif
}


static void lcm_init(void)
{
#ifdef BUILD_LK
    pmic_config_interface(0x0532,5,0x7,5);//add by libo for VGP2 POWER ON
    pmic_config_interface(0x050C,1,0x1,15);
#else
    hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
#endif
    MDELAY(100);

    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(130);

		init_lcm_registers();
  
}

//static unsigned int lcm_suspended = 0;
static void lcm_suspend(void)
{
		SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(130);
//	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
//	lcm_suspended = 1;
}


static void lcm_resume(void)
{
	//if(lcm_suspended==0) return;
	//lcm_suspended = 0;
		lcm_compare_id();
	lcm_init();
	
//	push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
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
    mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,0);
    MDELAY(1);

    return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}

static unsigned int lcm_compare_id()
{
//	return 1;
#if 0	
    unsigned int id = 0, id2 = 0;
    unsigned char buffer[2];
    unsigned int data_array[16];

#if defined(BUILD_LK)
	upmu_set_rg_vgp2_vosel(5);
	upmu_set_rg_vgp2_en(1);

	upmu_set_rg_vgp3_vosel(3);
	upmu_set_rg_vgp3_en(1);	
#else
	hwPowerOn(MT6323_POWER_LDO_VGP2, VOL_2800, "Lance_LCM");
       hwPowerOn(MT6323_POWER_LDO_VGP3, VOL_1800, "Lance_LCM");
#endif
    MDELAY(100);


    SET_RESET_PIN(1);
    MDELAY(20);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(100);

    //*************Enable CMD2 Page1  *******************//
    data_array[0]=0x00043902;
    data_array[1]=0x6983ffb9;//52AA55F0;
    //data_array[2]=0x00000108;
    dsi_set_cmdq(data_array, 3, 1);
    MDELAY(10);

    data_array[0] = 0x00023700;// read id return two byte,version and id
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(10);

    read_reg_v2(0xF4, buffer, 2);
    id = buffer[0]; //we only need ID

    Lcd_Log("\n %s HX8369A 0x%x , 0x%x , 0x%x \n",__func__,buffer[0],buffer[1],id);

    return (id == LCM_ID)?1:0;
#else
    Lcd_Log("mycat xxxxxx get_lcd_id=0x%x\n",get_lcd_id());
    return (get_lcd_id()==1)?1:0;
#endif
}
#if 1

static int dummy_delay = 0;
static unsigned int lcm_esd_check(void)
{  
    unsigned int  data_array[16];
    unsigned char buffer_0a = 0;    
    unsigned int retval = 0;
    
    dummy_delay ++;

    if (dummy_delay >=10000)
        dummy_delay = 0;
    
    if(dummy_delay %2 == 0)
    {    


	    data_array[0] = 0x00013700;
	    dsi_set_cmdq(data_array, 1, 1);
	    read_reg_v2(0x0a,&buffer_0a, 1);



            #ifdef BUILD_LK
	    printf("build lk lcm_esd_check lcm -----------------\n");
	    printf("lcm_esd_check lcm 0x0A is %x-----------------\n", buffer_0a);
            #else
	    printk("not build lk lcm_esd_check lcm -----------------\n");	
            printk("lcm_esd_check lcm 0x0A is %x-----------------\n", buffer_0a);
            #endif
	
	    if (buffer_0a==0x9C)
               
            {	    
			#ifdef BUILD_LK
		   printf("diablox_lcd lcm_esd_check done\n");
			#else
			printk("diablox_lcd lcm_esd_check done\n");
			#endif
		    retval = 0;
	    }else{
			 #ifdef BUILD_LK
		    printf("diablox_lcd lcm_esd_check return true\n");
			#else
			 printk("diablox_lcd lcm_esd_check return true\n");
			#endif
		    retval = 1;
	    }
    }

	return retval;
  
}

static unsigned int lcm_esd_recover(void)
{
    //printk("%s \n",__FUNCTION__);
    
    lcm_init();

    return 1;
}
#endif

LCM_DRIVER hx8369b_liangjingjing_LT43062EHT01_BOE_dsi_wvga_lcm_drv =
    {
        .name			= "hx8369b_liangjingjing_LT43062EHT01_BOE_dsi_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .compare_id    = lcm_compare_id,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
#if 1
	.esd_check		= lcm_esd_check,
	.esd_recover	= lcm_esd_recover,
#endif
#if (LCM_DSI_CMD_MODE)
        .update         = lcm_update,
#endif
    };
