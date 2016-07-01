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
#else
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
    #include <linux/string.h>
   
#endif
#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(540)
#define FRAME_HEIGHT 										(960)

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xFF   // END OF REGISTERS MARKER

////#define LCM_DSI_CMD_MODE									0

#define LCM_ID_HX8389B 0x89

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
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


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


	//must use 0x39 for init setting for all register.
	/*CMI46306FPC*/
	#if 0//TRULY CODE
{0xB9,3,{0xFF,0x83,0x89}},
//{0xBA,7,{0x41,0x93,0x00,0x16,0xa4,0x10,0x18}},
//{0xC6,1,{0xE0}},//0X08
{0xB1,19,{0x00,0x00,0x06,0xE8,0x59,0x10,0x11,0xD1,0xF1,0x3D,0x45,0x2E,0x2E,0x42,0x01,0x5A,0xF7,0x00,0xE6}},
{0xB7,3,{0x00,0x00,0x4F}},
{0xB2,7,{0x00,0x00,0x78,0x0C,0x07,0x3F,0x30}},
{0xB4,23,{0x90,0x08,0x00,0x32,0x10,0x04,0x32,0x10,0x00,0x32,0x10,0x00,0x37,0x0A,0x40,0x08,0x37,0x02,0x40,0x14,0x50,0x58,0x0A}},
{0xD5,56,{0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x60,0x00,0x99,0x88,0xAA,0xBB,0x88,0x23,0x88,0x01,0x88,0x67,0x88,0x45,0x01,
0x23,0x88,0x88,0x88,0x88,0x88,0x88,0x99,0xBB,0xAA,0x88,0x54,0x88,0x76,0x88,0x10,0x88,0x32,0x32,0x10,0x88,0x88,0x88,0x88,0x88,
0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xB6,4,{0x00,0x9A,0x00,0x9A}},
{0xE0,34,{0x00,0x23,0x25,0x3C,0x3E,0x3F,0x2D,0x47,0x07,0x0E,0x0E,0x11,0x13,0x0F,0x11,0x11,0x19,
	0x00,0x23,0x25,0x3C,0x3E,0x3F,0x2D,0x47,0x07,0x0E,0x0E,0x11,0x13,0x0F,0x11,0x11,0x19}},
{0xCC,1,{0x02}},

	{0x11, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},
{0xC6,1,{0xE0}},
    	// Display ON
	{0x29, 0, {0x00}},
	{REGFLAG_DELAY, 150, {}},	

#else
#if 1
{0xB9,3,{0xFF,0x83,0x89}},
{0xBA,7,{0x41, 0x83, 0x00, 0x16, 0xA4, 0x10, 0x18}},
{0xC6,1,{0x08}},//0X08
{0xDE,2,{0x05, 0x58}},//0X08
{0xB1,19,{0x00,0x00,0x04,0xD9,0x8F,0x10,0x11,0xEC,0xEC,0x1D,0x25,0x1D,0x1D,0x42,0x01,0x5A,0xF7,0x20,0x80}},
{0xB2,7,{0x00,0x00,0x78,0x01,0x02,0x3F,0x80}},
{0xB4,31,{0x80,0x04,0x00,0x32,0x10,0x00,0x32,0x10,0x00,0x00,0x00,0x00,0x17,0x0A,0x40,0x01,0x13,0x0A,0x40,0x14,0x46,0x50,0x0A,0x0A,0x3C,0x0A,0x3C,0x14,0x46,0x50,0x0A}},
{0xD5,48,{0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x20,0x00,0x99,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x01,0x88,0x23,0x01,0x88,0x88,0x88,
0x88,0x88,0x88,0x88,0x99,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x32,0x88,0x10,0x10,0x88,0x88,0x88,0x88,0x88,0x88}},
          
{0xE0,34,{0x05,0x11,0x14,0x37,0x3F,0x3F,0x20,0x43,0x05,0x0E,0x0D,0x12,0x14,0x12,0x14,0x10,0x1C,
        0x05,0x11,0x14,0x37,0x3F,0x3F,0x20,0x43,0x05,0x0E,0x0D,0x12,0x14,0x12,0x14,0x10,0x1C}},
               
{0xCC,1,{0x02}},
{0xB6,4,{0x00,0x8A,0x00,0x8A}},

	{0x11, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},
//{0xC6,1,{0xE0}},
    	// Display ON
	{0x29, 0, {0x00}},
	{REGFLAG_DELAY, 150, {}},
#else
{0XB9, 3, {0XFF,0X83,0X89}},
	{REGFLAG_DELAY, 10, {}},

#if 0//CLOSE IC ESD Protect enhance
	{0XBA, 17, {0X41,0X83,0X00,0X16,
				0XA4,0X00,0X18,0XFF,
				0X0F,0X21,0X03,0X21,
				0X23,0X25,0X20,0X02, 
		  		0X31}}, 
	{REGFLAG_DELAY, 10, {}},
#else //open IC ESD  Protect enhance 
{0XBA, 17, {0X41,0X83,0X00,0X16,
			0XA4,0X00,0X18,0XFF,
			0X0F,0X21,0X03,0X21,
			0X23,0X25,0X20,0X02, 
			0X35}}, 
{REGFLAG_DELAY, 10, {}},

#endif

	{0XDE, 2, {0X05,0X58}},
	{REGFLAG_DELAY, 10, {}},

	{0XB1, 19, {0X00,0X00,0X04,0XD9,
				0XCF,0X10,0X11,0XAC,
				0X0C,0X1D,0X25,0X1D,
				0X1D,0X42,0X01,0X58,
		  		0XF7,0X20,0X80}},
	{REGFLAG_DELAY, 10, {}},


	{0XB2, 5,   {0X00,0X00,0X78,0X03,
				 0X02}},
	{REGFLAG_DELAY, 10, {}},
	
	{0XB4, 31, {0X82,0X04,0X00,0X32,
				0X10,0X00,0X32,0X10,
				0X00,0X00,0X00,0X00,
				0X17,0X0A,0X40,0X01,
				0X13,0X0A,0X40,0X14,
				0X46,0X50,0X0A,0X0A,
				0X3C,0X0A,0X3C,0X14,
		  		0X46,0X50,0X0A}}, 
	{REGFLAG_DELAY, 10, {}},
	
	{0XD5, 48, {0X00,0X00,0X00,0X00,
				0X01,0X00,0X00,0X00,
				0X20,0X00,0X99,0X88,
				0X88,0X88,0X88,0X88,
				0X88,0X88,0X88,0X01,
				0X88,0X23,0X01,0X88,
				0X88,0X88,0X88,0X88,
				0X88,0X88,0X99,0X88,
				0X88,0X88,0X88,0X88,
				0X88,0X88,0X32,0X88,
				0X10,0X10,0X88,0X88,
				0X88,0X88,0X88,0X88}}, 
	{REGFLAG_DELAY, 10, {}},
	

	{0XB6, 4,   {0X00,0X8A,0X00,0X8A}},
	{REGFLAG_DELAY, 10, {}},

	{0XCC, 1,   {0X02}},
	{REGFLAG_DELAY, 10, {}},


	{0X35, 1,   {0X00}},//TE on
	{REGFLAG_DELAY, 10, {}},
#endif

	#endif


	// Note
	// Strongly recommend not to set Sleep out / Display On here. That will cause messed frame to be shown as later the backlight is on.

	// Setting ending by predefined flag
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void RX_LCM_INIT(void)
{
	unsigned int data_array[16];

	data_array[0]=0x00043902;
	data_array[1]=0x8983FFB9; 
	dsi_set_cmdq(&data_array, 2, 1);

	data_array[0]=0x00083902;
	data_array[1]=0x008341BA; 
	data_array[2]=0x16A41018;  
	dsi_set_cmdq(&data_array, 3, 1);

	data_array[0]=0x08C61500;
	dsi_set_cmdq(&data_array, 1, 1);

	data_array[0]=0x00033902;
	data_array[1]=0x005805DE; 
	dsi_set_cmdq(&data_array, 2, 1);

	
	data_array[0]=0x00143902;
	data_array[1]=0x040000B1; //06->04
	data_array[2]=0x11108FD9; 
	data_array[3]=0x251DECEC;//F1->F4 
	data_array[4]=0x01421D1D; 
	data_array[5]=0x8020F75A; 
	dsi_set_cmdq(&data_array, 6, 1);

	data_array[0]=0x00083902;
	data_array[1]=0x780000B2; 
	data_array[2]=0x803F0201;  
	dsi_set_cmdq(&data_array, 3, 1);

	data_array[0]=0x00183902;
	data_array[1]=0x000480B4; 
	data_array[2]=0x32001032; 
	data_array[3]=0x00000010; 
	data_array[4]=0x400A1700; 
	data_array[5]=0x400A1301; 
	data_array[6]=0x0A504614; 
	data_array[7]=0x3C0A3C0A; 
	data_array[8]=0x0A504614; 
	dsi_set_cmdq(&data_array, 9, 1);


	data_array[0]=0x00383902;
	data_array[1]=0x000000D5; 
	data_array[2]=0x00000100; 
	data_array[3]=0x99002000; 
	data_array[4]=0x88888888; 
	data_array[5]=0x88888888; 
	data_array[6]=0x01238801; 
	data_array[7]=0x88888823;
	data_array[8]=0x88888888; 
	data_array[9]=0x99888888; 
	data_array[10]=0x88888888; 
	data_array[11]=0x32888888; 
	data_array[12]=0x88101088; 
	data_array[13]=0x88888888; 
	data_array[14]=0x00000088; 
	dsi_set_cmdq(&data_array, 15, 1);

	data_array[0]=0x00233902;
	data_array[1]=0x141105E0; 
	data_array[2]=0x203F3F37; 
	data_array[3]=0x0D0E0543; 
	data_array[4]=0x14121412; 
	data_array[5]=0x11051C10; 
	data_array[6]=0x3F3F3714; 
	data_array[7]=0x0E054320; 
	data_array[8]=0x1214120D; 
	data_array[9]=0x001C1014; 
	dsi_set_cmdq(&data_array, 10, 1);


	data_array[0]=0x02CC1500;
	dsi_set_cmdq(&data_array, 1, 1);

	//data_array[0]=0x00043902;
	//data_array[1]=0x500000B7; 
	//dsi_set_cmdq(&data_array, 2, 1);



	data_array[0]=0x00053902;
	data_array[1]=0x007D00B6; 
	data_array[2]=0x0000007D; 
	dsi_set_cmdq(&data_array, 3, 1);

	data_array[0] = 0x00350500;	
	dsi_set_cmdq(&data_array, 1, 1);

	data_array[0] = 0x00110500;	
	dsi_set_cmdq(&data_array, 1, 1);			
	MDELAY(120);

	
	data_array[0] = 0x00290500;	
	dsi_set_cmdq(&data_array, 1, 1);			
	MDELAY(50);
}

static struct LCM_setting_table lcm_set_window[] = {
	{0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
	{0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 0, {0x00}},
	{REGFLAG_DELAY, 10, {}},
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_in_setting[] = {
	// Display off sequence
	{0x28, 0, {0x00}},

    // Sleep Mode On
	{0x10, 0, {0x00}},

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
		params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

///#if (LCM_DSI_CMD_MODE)
		///params->dsi.mode   = CMD_MODE;
///#else
		params->dsi.mode   = SYNC_PULSE_VDO_MODE;
////#endif
	
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
		params->dsi.vertical_frontporch					= 9;
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 6;
		params->dsi.horizontal_backporch				= 60;
		params->dsi.horizontal_frontporch				= 22;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

                    params->dsi.pll_div1=1;//48;		// fref=26MHz, fvco=fref*(div1+1)	(div1=0~63, fvco=500MHZ~1GHz)
		params->dsi.pll_div2=1; 		// div2=0~15: fout=fvo/(2*div2)
                params->dsi.fbk_div =31;

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
MDELAY(5);
    SET_RESET_PIN(0);
    MDELAY(5);
    SET_RESET_PIN(1);
    MDELAY(150);//Must over 6 ms,SPEC request

	RX_LCM_INIT();
	
	//push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
SET_RESET_PIN(1);
MDELAY(5);	
SET_RESET_PIN(0);	
MDELAY(3);	
	SET_RESET_PIN(1);
	MDELAY(150);	

	///push_table(lcm_sleep_in_setting, sizeof(lcm_sleep_in_setting) / sizeof(struct LCM_setting_table), 1);

}


static void lcm_resume(void)
{
#ifdef BUILD_LK

#else
printk("hwj huaxian lcd resume!!!");
#endif
lcm_init();
	///push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
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
	data_array[3]= 0x00053902;
	data_array[4]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[5]= (y1_LSB);
	data_array[6]= 0x002c3909;

	dsi_set_cmdq(&data_array, 7, 0);

}


static unsigned int lcm_compare_id(void)
{
	unsigned int id=0;
	unsigned char buffer[2];
	unsigned int array[16];  

    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(1);
    SET_RESET_PIN(1);
    MDELAY(10);//Must over 6 ms

	array[0]=0x00043902;
	array[1]=0x8983FFB9;// page enable
	dsi_set_cmdq(&array, 2, 1);
	MDELAY(10);

	array[0] = 0x00023700;// return byte number
	dsi_set_cmdq(&array, 1, 1);
	MDELAY(10);

	read_reg_v2(0xF4, buffer, 2);
	id = buffer[0]; 
	
#if defined(BUILD_LK)
	printf("%s, id = 0x%08x\n", __func__, id);
#endif

	return (LCM_ID_HX8389B == id)?1:0;

}


// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER hx8389b_huaxian_HX451139QP_BOE_dsi_2_qhd_lcm_drv = 
{
    .name			= "hx8389b_huaxian_HX451139QP_BOE_dsi_2_qhd",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id    = lcm_compare_id,
};

