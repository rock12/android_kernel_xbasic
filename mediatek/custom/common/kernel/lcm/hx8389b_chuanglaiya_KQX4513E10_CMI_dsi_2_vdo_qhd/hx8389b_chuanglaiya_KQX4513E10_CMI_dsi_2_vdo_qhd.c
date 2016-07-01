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




static void RX_LCM_INIT(void)
{

#if 1

//奇彩
unsigned int data_array[33];

		data_array[0]= 0x00043902;
		data_array[1]= 0x8983FFB9;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);

		data_array[0]= 0x00083902;
		data_array[1]= 0x008341BA;
		data_array[2]= 0x1800A416;
		dsi_set_cmdq(&data_array, 3, 1);
		MDELAY(1);

		data_array[0]= 0x00023902;
		data_array[1]= 0x000008C6;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);

		data_array[0]= 0x00143902;
		data_array[1]= 0x040000B1;
		data_array[2]= 0x11105AE8;
		data_array[3]= 0x3E36F1B1;
		data_array[4]= 0x01422828;
		data_array[5]= 0xE600F75A;
		dsi_set_cmdq(&data_array, 6, 1);
		MDELAY(1);

		data_array[0]= 0x00043902;
		data_array[1]= 0x500000B7;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);

		data_array[0]= 0x00083902;
		data_array[1]= 0x780000B2;
		data_array[2]= 0x803F0B08;
		dsi_set_cmdq(&data_array, 3, 1);
		MDELAY(1);

		data_array[0]= 0x00203902;
		data_array[1]= 0x000880B4;
		data_array[2]= 0x30061032;
		data_array[3]= 0x0000D213;
		data_array[4]= 0x400A3F00;
		data_array[5]= 0x400A3708;
		data_array[6]= 0x0A504614;
		data_array[7]= 0x400A400A;
		data_array[8]= 0x0A504614;
		dsi_set_cmdq(&data_array, 9, 1);
		MDELAY(1);

		data_array[0]= 0x00313902;
		data_array[1]= 0x000000D5;
		data_array[2]= 0x00000100;
		data_array[3]= 0x8810A000;
		data_array[4]= 0x08018888;
		data_array[5]= 0x28858148;
		data_array[6]= 0x05878368;
		data_array[7]= 0x88884888;
		data_array[8]= 0x88888888;
		data_array[9]= 0x38781088;
		data_array[10]= 0x10588286;
		data_array[11]= 0x80848084;
		data_array[12]= 0x88888858;
		data_array[13]= 0x00000088;
		dsi_set_cmdq(&data_array, 14, 1);
		MDELAY(1);

		data_array[0]= 0x00233902;
		data_array[1]= 0x141105E0;
		data_array[2]= 0x203F3F37;
		data_array[3]= 0x0D0E084F;
		data_array[4]= 0x14121412;
		data_array[5]= 0x11051C1D;
		data_array[6]= 0x3F3F3714;
		data_array[7]= 0x0E084F20;
		data_array[8]= 0x1214120D;
		data_array[9]= 0x001C1D14;
		dsi_set_cmdq(&data_array, 10, 1);
		MDELAY(1);

		data_array[0]=	0x00213902;
		data_array[1]=	0x070001C1;
		data_array[2]=	0x3F342A1C;
		data_array[3]=	0x635B5249;
		data_array[4]=	0x7F78716A;
		data_array[5]=	0x97918B85;
		data_array[6]=	0xAFA8A39D;
		data_array[7]=	0xC9C6BEB5;
		data_array[8]=	0xE6E1D7CD;
        data_array[9]=	0x000000F1;
        dsi_set_cmdq(&data_array, 10, 1);
		MDELAY(1);

        data_array[0]=	0x00212902;
		data_array[1]=	0x34FFF7C1;
		data_array[2]=	0x54D94FCB;
		data_array[3]=	0xC04A3CC8;
		data_array[4]=	0x2A1C0700;
		data_array[5]=	0x52493F34;
		data_array[6]=	0x716A635B;
		data_array[7]=	0x8B857F78;
		data_array[8]=	0xA39D9791;
		data_array[9]=	0x000000A8;
        dsi_set_cmdq(&data_array, 10, 1);
		MDELAY(1);

        data_array[0]=	0x00212902; 
		data_array[1]=	0xBEB5AFC1;
		data_array[2]=	0xD7CDC9C6;
		data_array[3]=	0xF7F1E6E1;
		data_array[4]=	0x4FCB34FF;
		data_array[5]=	0x3CC854D9;
		data_array[6]=	0x0700C04A;
		data_array[7]=	0x3F342A1C;
		data_array[8]=	0x635B5249;
        data_array[9]=	0x0000006A;
        dsi_set_cmdq(&data_array, 10, 1);
		MDELAY(1);

        data_array[0]=	0x00202902; 
		data_array[1]=	0x7F7871C1;
		data_array[2]=	0x97918B85;
		data_array[3]=	0xAFA8A39D;
		data_array[4]=	0xC9C6BEB5;
		data_array[5]=	0xE6E1D7CD;
		data_array[6]=	0x34FFF7F1;
		data_array[7]=	0x54D94FCB;
		data_array[8]=	0xC04A3CC8;
		dsi_set_cmdq(&data_array, 9, 1);
		MDELAY(1);

		data_array[0]= 0x00023902;
		data_array[1]= 0x000002CC;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);

		data_array[0]= 0x00023902;
		data_array[1]= 0x0000773A;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);

		data_array[0]= 0x00023902;
		data_array[1]= 0x00000035;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);

		data_array[0]= 0x00053902;
		data_array[1]= 0x00A500B6;
		data_array[2]= 0x000000A5;
		dsi_set_cmdq(&data_array, 3, 1);
		MDELAY(1);
	
		data_array[0] = 0x00110500; 	   //exit sleep mode 
		dsi_set_cmdq(&data_array, 1, 1); 
		MDELAY(150); 
		
		data_array[0] = 0x00290500; 	   //exit sleep mode 
		dsi_set_cmdq(&data_array, 1, 1); 
		MDELAY(20);

#else

	unsigned int data_array[16];

	data_array[0]=0x00043902;
	data_array[1]=0x8983FFB9; 
	dsi_set_cmdq(&data_array, 2, 1);	

	data_array[0]=0x00083902;
	data_array[1]=0x008341BA; 
	data_array[2]=0x1810A416; 
	dsi_set_cmdq(&data_array, 3, 1);
	
	data_array[0]=0x00143902;
	data_array[1]=0x040000B1;
	data_array[2]=0x111050E8; 
	data_array[3]=0x3E36F1B1;
	data_array[4]=0x01422828; 
	data_array[5]=0xE600F75A;  
	dsi_set_cmdq(&data_array, 6, 1);
	
	data_array[0]=0x00083902;
	data_array[1]=0x780000B2; 
	data_array[2]=0x803F0B08;  
	dsi_set_cmdq(&data_array, 3, 1);


	data_array[0]=0x00203902;
	data_array[1]=0x000880B4; 
	data_array[2]=0x30061032; 
	data_array[3]=0x0000D213; 
	data_array[4]=0x400A3F00; 
	data_array[5]=0x400A3708; 
	data_array[6]=0x0A504614; 
	data_array[7]=0x400A400A;
	data_array[8]=0x0A504614;
	dsi_set_cmdq(&data_array, 9, 1);


	data_array[0]=0x00313902;
	data_array[1]=0x000000D5; 
	data_array[2]=0x00000100; 
	data_array[3]=0x8810A000; 
	data_array[4]=0x08018888; 
	data_array[5]=0x28858148; 
	data_array[6]=0x05878368; 
	data_array[7]=0x88884888;
	data_array[8]=0x88888888; 
	data_array[9]=0x38781088; 
	data_array[10]=0x10588286; 
	data_array[11]=0x80848084; 
	data_array[12]=0x88888858; 
	data_array[13]=0x00000088; 
	dsi_set_cmdq(&data_array, 14, 1);


	data_array[0]=0x00233902;
	data_array[1]=0x141105E0; 
	data_array[2]=0x203F3F37; 
	data_array[3]=0x0D0E084F; 
	data_array[4]=0x14121412; 
	data_array[5]=0x11051C1D; 
	data_array[6]=0x3F3F3714; 
	data_array[7]=0x0E084F20; 
	data_array[8]=0x1214120D; 
	data_array[9]=0x001C1D14; 
	dsi_set_cmdq(&data_array, 10, 1);
	
	
	data_array[0]=0x002B3902;
	data_array[1]=0x070001C1; 
	data_array[2]=0x3F342A1C; 
	data_array[3]=0x635B5249; 
	data_array[4]=0x7F78716A; 
	data_array[5]=0x97918B85; 
	data_array[6]=0xAFA8A39D; 
	data_array[7]=0xC9C6BEB5;
	data_array[8]=0xE6E1D7CD; 
	data_array[9]=0x34FFF7F1; 
	data_array[10]=0x54D94FCB; 
	data_array[11]=0x004A3CC8; 
	dsi_set_cmdq(&data_array, 12, 1);
	
	
	data_array[0]=0x002A3902;
	data_array[1]=0x1C0700C0; 
	data_array[2]=0x493F342A; 
	data_array[3]=0x6A635B52; 
	data_array[4]=0x857F7871; 
	data_array[5]=0x9D97918B; 
	data_array[6]=0xB5AFA8A3; 
	data_array[7]=0xCDC9C6BE;
	data_array[8]=0xF1E6E1D7;
	data_array[9]=0xCB34FFF7; 
	data_array[10]=0xC854D94F;  
	data_array[11]=0x00004A3C; 
	dsi_set_cmdq(&data_array, 12, 1);
	
	
	data_array[0]=0x002B3902;
	data_array[1]=0x1C0700C0; 
	data_array[2]=0x493F342A; 
	data_array[3]=0x6A635B52; 
	data_array[4]=0x857F7871; 
	data_array[5]=0x9D97918B; 
	data_array[6]=0xB5AFA8A3; 
	data_array[7]=0xCDC9C6BE;
	data_array[8]=0xF1E6E1D7;
	data_array[9]=0xCB34FFF7; 
	data_array[10]=0xC854D94F;  
	data_array[11]=0x00C04A3C; 
	dsi_set_cmdq(&data_array, 12, 1);
	
	
	data_array[0]=0x00023902;
  data_array[1]=0x000002CC;
  dsi_set_cmdq(data_array, 2, 1);
  
  data_array[0]=0x00053902;
	data_array[1]=0x009E00B6; 
	data_array[2]=0x0000009E; 
	dsi_set_cmdq(&data_array, 3, 1);
	
	data_array[0]=0x00023902;
  data_array[1]=0x000008C6;
  dsi_set_cmdq(data_array, 2, 1);
  
  data_array[0]=0x00023902;
  data_array[1]=0x0000773A;
  dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00110500;	
	dsi_set_cmdq(&data_array, 1, 1);			
	MDELAY(200);


	data_array[0] = 0x00290500;	
	dsi_set_cmdq(&data_array, 1, 1);			
	MDELAY(120);

	#endif

}


static struct LCM_setting_table lcm_set_window[] = {
	{0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
	{0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 0, {0x00}},
    {REGFLAG_DELAY, 130, {}},

    // Display ON
	{0x29, 0, {0x00}},
	{REGFLAG_DELAY, 50, {}},
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_in_setting[] = {
	// Display off sequence
	//{0x28, 0, {0x00}},

    // Sleep Mode On
	{0x10, 0, {0x00}},
	{REGFLAG_DELAY, 100, {}},
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


		params->dsi.vertical_sync_active				= 9; //2
		params->dsi.vertical_backporch					= 7; //14
		params->dsi.vertical_frontporch					= 7; //9
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 90; //6
		params->dsi.horizontal_backporch				= 46; //60
		params->dsi.horizontal_frontporch				= 21; //22
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

                    params->dsi.pll_div1=1;//48;		// fref=26MHz, fvco=fref*(div1+1)	(div1=0~63, fvco=500MHZ~1GHz)
		params->dsi.pll_div2=1; 		// div2=0~15: fout=fvo/(2*div2)
                params->dsi.fbk_div =34; //31

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
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(150);//Must over 6 ms,SPEC request

	RX_LCM_INIT();
	
	/////push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
	
	
//SET_RESET_PIN(1);
//MDELAY(5);	
//SET_RESET_PIN(0);	
//MDELAY(20);	
//SET_RESET_PIN(1);
//MDELAY(50);	

	push_table(lcm_sleep_in_setting, sizeof(lcm_sleep_in_setting) / sizeof(struct LCM_setting_table), 1);

}


static void lcm_resume(void)
{

//lcm_init();
	push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
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
	
#if defined(BUILD_LK)
	printf("%s, id = 0x%08x\n", __func__, id);
#endif

	return (LCM_ID_HX8389B == id)?1:0;
			//return ((LCM_ID_HX8389B == id) && (get_lcd_id() == 1))?1:0;


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
LCM_DRIVER hx8389b_chuanglaiya_KQX4513E10_CMI_dsi_2_vdo_qhd_lcm_drv = 
{
    .name			= "hx8389b_chuanglaiya_KQX4513E10_CMI_dsi_2_vdo_qhd",
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

