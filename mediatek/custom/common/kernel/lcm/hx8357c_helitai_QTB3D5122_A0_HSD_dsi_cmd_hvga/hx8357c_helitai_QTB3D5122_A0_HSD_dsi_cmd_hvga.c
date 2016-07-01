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
#define FRAME_WIDTH  (320)
#define FRAME_HEIGHT (480)

#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN
#define LCM_DSI_CMD_MODE
#define LCM_ID  (0x80)

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
#define dsi_set_cmdq(pdata, queue_size, force_update)			lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)																		lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)								lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg_v2(cmd, buffer, buffer_size)   					lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)  


static void init_lcm_registers(void)
{
    unsigned int data_array[20];
#if 0
		data_array[0]=0x00043902;//Enable external Command
		data_array[1]=0x5783FFB9; 
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(5);
		data_array[0]=0x53B61500; //
		dsi_set_cmdq(&data_array,1,1);
		
		data_array[0] = 0x00110500; //0x11,exit sleep mode,1byte
		dsi_set_cmdq(&data_array, 1, 1);
		MDELAY(150);
		
		data_array[0]=0x00350500; //TE on
		dsi_set_cmdq(&data_array,1,1);
		
		data_array[0] = 0x663A1500; //0x3A,set pixel format,2bytes: 18BPP
		dsi_set_cmdq(&data_array, 1, 1);
		
		//data_array[0] = 0xD0361500; //0x36,set address mode,2bytes
		//dsi_set_cmdq(&data_array, 1, 1);
		
		// data_array[0] = 0x00210500; //0x21,enter inversion mode,1byte
		//dsi_set_cmdq(&data_array, 1, 1);
		
		data_array[0] = 0x68B01500; //70HZ
		dsi_set_cmdq(&data_array, 1, 1);
		
		MDELAY(100);//If not  delay before Command 0x44, the command doesn't take effect, i dont why?
		data_array[0] = 0x05CC1500; //set Pannel  soso pannel V inveserse
		dsi_set_cmdq(&data_array, 1, 1);
		
		data_array[0]=0x00073902;
		data_array[1]=0x1C1500B1;
		data_array[2]=0x0044831C;
		dsi_set_cmdq(&data_array, 3, 1);
		
		data_array[0]=0x00083902;
		data_array[1]=0x004002B4;
		data_array[2]=0x780D2A2A;
		dsi_set_cmdq(&data_array, 3, 1);
		
		data_array[0]=0x00073902;//Panel Driving Setting (C0h),6byte
		data_array[1]=0x015050C0;
		data_array[2]=0x0008C83C;
		dsi_set_cmdq(&data_array, 3, 1);
		
		//Gamma
		/*
		data_array[0]=0x00463902;
		data_array[1]=0x300000E0;
		data_array[2]=0x3B003700;
		data_array[3]=0x44003F00;
		data_array[4]=0x59005200;
		data_array[5]=0x3A006000;	
		data_array[6]=0x2F003500;
		data_array[7]=0x22002900;
		data_array[8]=0x19001C00;
		data_array[9]=0x30000300;
		data_array[10]=0x3B003700;	
		data_array[11]=0x44003F00;	
		data_array[12]=0x59005200;
		data_array[13]=0x3A006000;
		data_array[14]=0x2F003500;
		data_array[15]=0x22002900;	
		data_array[16]=0x19001C00;
		data_array[17]=0x00000300;
		data_array[18]=0x00000100;
		dsi_set_cmdq(&data_array, 19, 1);
		*/
		
		data_array[0] = 0x00290500; //0x29,Display On,1byte
		dsi_set_cmdq(&data_array, 1, 1);
		MDELAY(25);
#else
    /*data_array[0] = 0x00110500; //0x11,exit sleep mode,1byte
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(130);*/
    
    data_array[0] = 0x00110500; //0x29,Display On,1byte
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(150);		
    
		data_array[0]=0x00043902;
    data_array[1]=0x5783FFB9; //Power_Setting (D0h),4bytes
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(1);
    
    data_array[0]=0x00073902;
    data_array[1]=0x1C1500B1;
    data_array[2]=0x0021C31C;
    dsi_set_cmdq(&data_array, 3, 1);
    MDELAY(1);
    
    data_array[0]=0x00053902;
    data_array[1]=0x060000B3;
    data_array[2]=0x00000006;
    dsi_set_cmdq(&data_array, 3, 1);
    MDELAY(1);
    
    data_array[0]=0x00053902;
    data_array[1]=0x004022B4;
    data_array[2]=0x4c122A2A;
    dsi_set_cmdq(&data_array, 3, 1);
    MDELAY(1);
    
    data_array[0]=0x00033902;
    data_array[1]=0x000505B5;
    dsi_set_cmdq(&data_array,2,1);
    MDELAY(1);

    data_array[0]=0x29B61500;//35 2C
    dsi_set_cmdq(&data_array,1,1);
    MDELAY(1);

    data_array[0]=0x00033902;
    data_array[1]=0x000155B0;
    dsi_set_cmdq(&data_array,2,1);
    MDELAY(1);


    data_array[0]=0x00073902;
    data_array[1]=0x002424C0; //Panel Driving Setting (C0h),6bytes
    data_array[2]=0x0008C810;
    dsi_set_cmdq(&data_array, 3, 1);
    MDELAY(1);

    data_array[0]=0x00043902;
    data_array[1]=0x040400C2;
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(1);
    
    data_array[0]=0x01CC1500;
    dsi_set_cmdq(&data_array,1,1);
    MDELAY(1);

  	data_array[0]=0x00233902;
    data_array[1]=0x080000E0;
    data_array[2]=0x473b1810;
    data_array[3]=0x3a424850;
    data_array[4]=0x24292e32;
    data_array[5]=0x08000019;
    data_array[6]=0x473b1810;
    data_array[7]=0x3a424850;
    data_array[8]=0x24292e32;
    data_array[9]=0x00010019;
    dsi_set_cmdq(&data_array, 10, 1);
    MDELAY(1);
    
    data_array[0]=0x00033902;
    data_array[1]=0x000199b0;
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(1);

    data_array[0] = 0x40361500; //0x36,set address mode,2bytes
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(10);
       
    data_array[0]=0x00023902;
    data_array[1]=0x00000035;
    dsi_set_cmdq(&data_array, 2, 1);
    MDELAY(1);
    
    data_array[0]=0x03EA1500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(1);

    data_array[0] = 0x663A1500; //0x3A,set pixel format,2bytes
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(120);


    data_array[0] = 0x00290500; //0x29,Display On,1byte
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(10);
    
    data_array[0] = 0x002C0500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(10);
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
#if defined(LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
#else
		params->dsi.mode   = SYNC_EVENT_VDO_MODE;
#endif	

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;
		
		//enable tearing-free
		params->dbi.te_mode             = LCM_DBI_TE_MODE_VSYNC_ONLY;
		//params->dbi.te_mode 						= LCM_DBI_TE_MODE_DISABLED; 
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
	
		// DSI
		params->dsi.LANE_NUM=LCM_ONE_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB666;
		

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 2;
		params->dsi.PS=LCM_PACKED_PS_18BIT_RGB666;
	
	  
    	// Bit rate calculation
  	params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4
  	params->dsi.pll_div2=0;//1		// div2=0,1,2,3;div2_real=1,2,4,4
  	params->dsi.fbk_div =14;//14		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)		

}

static void lcm_init(void)
{
	SET_RESET_PIN(1); 
  MDELAY(20);
  SET_RESET_PIN(0);
  MDELAY(30);
  SET_RESET_PIN(1);
  MDELAY(120);

  init_lcm_registers();
}

static void lcm_suspend(void)
{
    unsigned int data_array[16];

    data_array[0]=0x00280500; // Display Off
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(10); 

    data_array[0] = 0x00100500; // Sleep In
    dsi_set_cmdq(&data_array, 1, 1);
    
    MDELAY(100);
}

static void lcm_resume(void)
{
	lcm_init();
}

static int get_lcd_id(void)
{
    mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
    mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,1);
    MDELAY(1);
		Lcd_Log("wqcat apex GPIO_LCD_ID_PIN=%d\n", mt_get_gpio_in(GPIO_LCD_ID_PIN));
    return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}

static unsigned int lcm_compare_id(void)
{
	unsigned int id = 0;
	unsigned char buffer[2];
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
  MDELAY(10);
  SET_RESET_PIN(0);
  MDELAY(50);
  SET_RESET_PIN(1);
  MDELAY(100);
  
	array[0] = 0x00043700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);
	
	read_reg_v2(0x04, buffer, 4);
	id = buffer[1]; //we only need ID

	Lcd_Log("wqcat read HX8369A id=%x,%x\n", buffer[0],buffer[1]);
	return ((LCM_ID == id)&&(get_lcd_id()==0))?1:0;
}

#if defined(LCM_DSI_CMD_MODE)
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
    unsigned int x0 = x;
    unsigned int y0 = y;
    unsigned int x1 = x0 + width - 1;
    unsigned int y1 = y0 + height - 1 ;

    unsigned char x0_MSB = ((x0>>8)&0xFF);
    unsigned char x0_LSB = (x0&0xFF);
    unsigned char x1_MSB = ((x1>>8)&0xFF);
    unsigned char x1_LSB = (x1&0xFF);
    unsigned char y0_MSB = ((y0>>8)&0xFF);
    unsigned char y0_LSB = (y0&0xFF);
    unsigned char y1_MSB = ((y1>>8)&0xFF);
    unsigned char y1_LSB = (y1&0xFF);

    unsigned int data_array[3];

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
#endif

LCM_DRIVER hx8357c_helitai_QTB3D5122_A0_HSD_dsi_cmd_hvga_lcm_drv = 
{
	.name		= "hx8357c_helitai_QTB3D5122_A0_HSD_dsi_cmd_hvga",
	.set_util_funcs 		= lcm_set_util_funcs,
	.get_params 				= lcm_get_params,
	.init								= lcm_init,
	.suspend						= lcm_suspend,
	.resume 						= lcm_resume,
#if defined(LCM_DSI_CMD_MODE)
	.update 						= lcm_update,
#endif
	.compare_id    			= lcm_compare_id,
};
