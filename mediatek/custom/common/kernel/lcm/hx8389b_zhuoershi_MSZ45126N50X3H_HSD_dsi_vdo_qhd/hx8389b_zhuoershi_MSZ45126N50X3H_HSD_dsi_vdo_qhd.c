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

static unsigned int lcm_esd_test = 0;      ///only for ESD test
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(540)
#define FRAME_HEIGHT 										(960)

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xFF   // END OF REGISTERS MARKER

////#define LCM_DSI_CMD_MODE									0

#define LCM_ID_HX8389B 0x89
#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN

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
	#if 1//TRULY CODE
{0xB9,3,{0xFF,0x83,0x89}},
{0xBA,7,{0x41,0x93,0x00,0x16,0xa4,0x10,0x18}},
{REGFLAG_DELAY, 15, {}},
{0xC6,1,{0x08}},//0X08
{0xde,3,{0x05,0x58,0x10}},
{0xB1,19,{0x00,0x00,0x07,0xe8,0x40,0x10,0x11,0xD2,0xf2,0x32,0x3A,0x29,0x29,0x42,0x01,0x3A,0xF7,0x00,0xE6}},
{REGFLAG_DELAY, 15, {}},
{0xB7,3,{0x00,0x00,0x4d}},
{0xB2,7,{0x00,0x00,0x78,0x0C,0x07,0x3F,0x40}},
{0xb4,23,{0x80,0x08,0x00,0x32,0x10,0x04,0x32,0x10,0x00,0x32,0x10,0x00,0x37,0x0a,0x40,0x08,0x37,0x0A,0x40,0x14,0x46,0x50,0x0a}},
{0xD5,56,{0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x60,0x00,0x99,0x88,0xAA,0xBB,0x88,0x23,0x88,0x01,0x88,0x67,0x88,0x45,0x01,0x23,0x88,0x88,
0x88,0x88,0x88,0x88,0x99,0x88,0xBB,0xAA,0x54,0x88,0x76,0x88,0x10,0x88,0x32,0x32,0x10,0x88,0x88,0x88,0x88,0x88,0x00,0x04,0x00,0x00,0x00,0x00,
0x00,0x00}},
{REGFLAG_DELAY, 15, {}},
{0xE0,34,{0x05,0x1E,0x21,0x39,0x3F,0x3F,0x2F,0x4A,0x09,0x0E,0x0F,0x12,0x14,0x11,0x14,0x10,0x18,0x05,0x1E,
0x21,0x39,0x3F,0x3F,0x2F,0x4A,0x09,0x0E,0x0F,0x12,0x14,0x11,0x14,0x10,0x18}},
{REGFLAG_DELAY, 15, {}},
{0xCC,1,{0x02}},
{REGFLAG_DELAY, 25, {}},
{0xb6,4,{0x00,0x9C,0x00,0x9C}},
{0x11, 0, {0x00}},
	{REGFLAG_DELAY, 200, {}},
    	// Display ON
	{0x29, 0, {0x00}},
	{REGFLAG_DELAY, 150, {}},	
	#endif


	// Note
	// Strongly recommend not to set Sleep out / Display On here. That will cause messed frame to be shown as later the backlight is on.

	// Setting ending by predefined flag
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void RX_LCM_INIT(void)
{
			//HX8389 HSD4.5
   		unsigned int data_array[25];   

		  data_array[0]=0x00043902;//Enable external Command
			data_array[1]=0x8983FFB9; 
			dsi_set_cmdq(&data_array, 2, 1);
			MDELAY(1);//3000

			data_array[0]=0x00083902;//Enable external Command//3
			data_array[1]=0x009341ba; 
			data_array[2]=0x1810a416; 
			dsi_set_cmdq(&data_array, 3, 1);
			MDELAY(1);//15//3000

			data_array[0]=0x00023902;//Enable external Command
			data_array[1]=0x000008c6; 
			dsi_set_cmdq(&data_array, 2, 1);
			MDELAY(1);//3000
			/*
			data_array[0]=0x00043902;//Enable external Command
			data_array[1]=0x105805de; 
			dsi_set_cmdq(&data_array, 2, 1);
			MDELAY(1);//3000
			*/
			data_array[0]=0x00143902;
			data_array[1]=0x040000B1;
			data_array[2]=0x111050eB;//111059e8
			data_array[3]=0x372FF090;  //0x332bf272   2a22f272
			data_array[4]=0x01422626; //01422a2a //01422929
			data_array[5]=0xE600FC3A;  
			dsi_set_cmdq(&data_array, 6, 1);
			MDELAY(1);//15//3000

			data_array[0]=0x00043902;//Enable external Command//3
			data_array[1]=0x500000b7;//500000b7
			dsi_set_cmdq(&data_array, 2, 1);
			MDELAY(1);//3000
		
			data_array[0]=0x00043902;//Enable external Command//3
			data_array[1]=0x780000b2;
			data_array[3]=0x403f070c;
			dsi_set_cmdq(&data_array, 3, 1);
			MDELAY(1);//3000
			
			
			data_array[0]=0x00183902;//Enable external Command//3
			data_array[1]=0x000480b4;   //0x000480b4  0x000882b4
			data_array[2]=0x32041032; 
			data_array[3]=0x10320010; 
			data_array[4]=0x400a3700; 
			data_array[5]=0x400a3708; //0x380a1301
			data_array[6]=0x0a504614; 
			dsi_set_cmdq(&data_array, 7, 1);
			MDELAY(1);//3000
		
			data_array[0]=0x00393902;//Enable external Command//3
			data_array[1]=0x000000d5; 
			data_array[2]=0x00000100; 
			data_array[3]=0x99006000; 
			data_array[4]=0x88bbaa88; 
			data_array[5]=0x88018823; 
			data_array[6]=0x01458867; 
			data_array[7]=0x88888823; 
			data_array[8]=0x99888888; 
			data_array[9]=0x54AABB88; 
			data_array[10]=0x10887688; 
			data_array[11]=0x10323288; 
			data_array[12]=0x88888888; 
			data_array[13]=0x00040088; 
			data_array[14]=0x00000000; 
			data_array[15]=0x00000000; 
			dsi_set_cmdq(&data_array, 16, 1);
			MDELAY(1);//15//3000

			data_array[0]=0x00053902;//Enable external Command
			data_array[1]=0x00ac00b6; //column 0xAC最好；dot inversion 0xB3最好。
			data_array[2]=0x000000ac; 
			dsi_set_cmdq(&data_array, 3, 1);
			MDELAY(1);//3000
			
			data_array[0]=0x00233902;
			data_array[1]=0x211e05e0; 
			data_array[2]=0x2f3f3f39; 
			data_array[3]=0x0f0e094a; 
			data_array[4]=0x14111412; 
			data_array[5]=0x1e051810; 
			data_array[6]=0x3f3f3921; 
			data_array[7]=0x0e094a2f; 
			data_array[8]=0x1114120f; 
			data_array[9]=0x00181014; 
			dsi_set_cmdq(&data_array, 10, 1);
			MDELAY(1);//15//3000
			
			data_array[0]=0x00023902;//e
			data_array[1]=0x000002cc;//
			dsi_set_cmdq(&data_array, 2, 1);
			MDELAY(1);//25			
		
			data_array[0]=0x00033902;//e
			data_array[1]=0x00ff0fc9;//
			dsi_set_cmdq(&data_array, 2, 1);
			MDELAY(1);//25

			data_array[0] = 0x00110500; //0x11,exit sleep mode,1byte
			dsi_set_cmdq(&data_array, 1, 1);
			MDELAY(150);//5000

      data_array[0] = 0x00290500; //0x11,exit sleep mode,1byte
      dsi_set_cmdq(&data_array, 1, 1);
      MDELAY(50);//5000

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
                params->dsi.fbk_div =30;

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
	
	////push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

///////////////
static void lcm_suspend(void)
{
		unsigned int data_array[16];
	
	  SET_RESET_PIN(1);
    MDELAY(5);
    SET_RESET_PIN(0);
    MDELAY(5);
    SET_RESET_PIN(1);
    MDELAY(150);//Must over 6 ms,SPEC request
	
	data_array[0] = 0x00280500; //0x11,exit sleep mode,1byte
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(120);//5000


	data_array[0] = 0x00100500; //0x10,entry sleep mode,1byte
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(150);//5
	//push_table(lcm_sleep_in_setting, sizeof(lcm_sleep_in_setting) / sizeof(struct LCM_setting_table), 1);
		
	MDELAY(80);
		
}


#if 0
///////////
static void lcm_suspend(void)
{

	

	push_table(lcm_sleep_in_setting, sizeof(lcm_sleep_in_setting) / sizeof(struct LCM_setting_table), 1);
	  MDELAY(20);//Must > 10ms

     SET_RESET_PIN(0);
    MDELAY(50);//Must > 10ms
	   SET_RESET_PIN(1);
      MDELAY(20);//Must > 10ms
	   SET_RESET_PIN(0);
		#if defined(BUILD_LK)
	upmu_set_rg_vgp2_en(0);
	upmu_set_rg_vgp3_en(0);	
   #else

	       hwPowerDown(MT6323_POWER_LDO_VGP2,"Lance_LCM");
		hwPowerDown(MT6323_POWER_LDO_VGP3, "Lance_LCM");
		#endif
		 MDELAY(50);//Must > 10ms

}
#endif

static void lcm_resume(void)
{

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

static int get_lcd_id(void)
{
    mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
    mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,0);
    MDELAY(1);

    return mt_get_gpio_in(GPIO_LCD_ID_PIN);
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
	
#if defined(BUILD_UBOOT)
	printf("%s, id = 0x%08x\n", __func__, id);
#endif

	return ((LCM_ID_HX8389B == id) && (get_lcd_id() == 0))?1:0;

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
	//MDELAY(20);
	read_reg_v2(0x0A, buffer, 1);
	 
	printk("mycat$$  ### esd check buffer[0] = 0x%x\r\n",buffer[0]);
	
	if(buffer[0] == 0x1C)//1C
		return 0;
	else
		return 1;
 			
#endif
}

static unsigned int lcm_esd_recover(void)
{
  lcm_init();
	
#ifndef BUILD_LK	
	printk("mycat  ### lcm_esd_recover \r\n");
#endif
	
	return 1;
}
#endif
// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER hx8389b_zhuoershi_MSZ45126N50X3H_HSD_dsi_vdo_qhd_lcm_drv = 
{
    .name			= "hx8389b_zhuoershi_MSZ45126N50X3H_HSD_dsi_vdo_qhd",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id    = lcm_compare_id,

#if defined (BIRD_ESD_CHECK)
	.esd_check = lcm_esd_check,
  .esd_recover = lcm_esd_recover,
#endif

};

