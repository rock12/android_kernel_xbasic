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
#define LCM_ID       (0x1202)
#define REGFLAG_DELAY             							0xAB
#define REGFLAG_END_OF_TABLE      							0xAA   // END OF REGISTERS MARKER
#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};
static unsigned int lcm_esd_test = FALSE; 
#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)		lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)				lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)																			lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)									lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg_v2(cmd, buffer, buffer_size)   						lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

static unsigned int lcm_compare_id();
//static unsigned int vcom = 0x50;
#define LCM_DSI_CMD_MODE    0

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};



static struct LCM_setting_table lcm_initialization_setting[] = {
	//Display Resolution:480*800, IVO3.97
	//Interface: MIPI 
	//Porch Setting:VBP=10,VFP=14,HBP=60,HFP=20
	//Invertion Type: 2 dot invertion 
	
	{0xff,4,{0xcc,0x33,0x12,0x82}},
	{REGFLAG_DELAY, 1, {}},
	
	{0xf7,18,{0x65,0xc0,0x40,0x00,0x01,0x02,0xb0,0x98,0x00,0x02,0x40,0x54,0x00,0x00,0xc0,0x00,0x42,0x28}},  
	{REGFLAG_DELAY, 1, {}},
					 
	//Select Page 0 
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
	{REGFLAG_DELAY, 1, {}},
	
	//VBP HBP VFP HFP
	{0xB0,5,{0x00,0x0A,0x0E,0x3C,0x14}},
	{REGFLAG_DELAY, 1, {}},
	
	//Display Option Control
	{0xB1,2,{0xFC,0x00}},
	{REGFLAG_DELAY, 1, {}},
	
	//Color Enhance Function Control
	{0xB4,1,{0x10}},
	{REGFLAG_DELAY, 1, {}},
	
	//Display Resolution Control
	{0xB5,1,{0x50}},  
	{REGFLAG_DELAY, 1, {}},
	
	//Source Output Data Hold Time Control
	{0xB6,1,{0x02}},
	{REGFLAG_DELAY, 1, {}},
	
	//EQ Control Function For Gate Signals
	{0xB7,2,{0x30,0x30}},
	{REGFLAG_DELAY, 1, {}},
	
	//EQ Function For Source Driver
	{0xB8,4,{0x01,0x07,0x07,0x07}},
	{REGFLAG_DELAY, 1, {}},
	
	//Inversion Driving Control
	{0xBC,3,{0x02,0x02,0x02}},			
	{REGFLAG_DELAY, 1, {}},
		  	 
	//GIP Setting
	{0xC0,2,{0xA5,0xC1}},
	{REGFLAG_DELAY, 1, {}},
	
	{0xC1,32,{0x00,0x00,0x00,0x02,0x03,0x06,0x07,0x08,0x09,0x04,0x05,0x00,0x00,0x00,0x00,0x00,
						0x00,0x00,0x00,0x00,0x00,0x12,0x13,0x0A,0x0B,0x0C,0x0D,0x14,0x15,0x00,0x00,0x00}}, 			
	{REGFLAG_DELAY, 1, {}},
	
	{0xC2,10,{0x84,0x33,0x10,0x86,0x10,0x9A,0x33,0x10,0x98,0x10}},	
	{REGFLAG_DELAY, 1, {}},
	
	{0xC3,42,{0x84,0x73,0x03,0x23,0x10,0x00,0x00,0x82,0x73,0x03,0x23,0x10,0x00,0x00,0x00,0x73,
						0x03,0x27,0x10,0x00,0x00,0x02,0x73,0x03,0x27,0x10,0x00,0x00,0x03,0x73,0x03,0x28,
						0x10,0x00,0x00,0x01,0x73,0x03,0x27,0x10,0x00,0x00}},      			
	{REGFLAG_DELAY, 1, {}},
	
	{0xC4,14,{0x81,0x73,0x03,0x24,0x10,0x00,0x00,0x83,0x73,0x03,0x23,0x10,0x00,0x00}},	
	{REGFLAG_DELAY, 1, {}},
	
	{0xC5,10,{0x97,0x33,0x10,0x99,0x10,0x85,0x33,0x10,0x83,0x10}},	
	{REGFLAG_DELAY, 1, {}},
	
	//Select Command Page 1
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},
	{REGFLAG_DELAY, 1, {}},
		
	//Setting AVDD Voltage
	{0xB0,3,{0x05,0x05,0x05}},
	{REGFLAG_DELAY, 1, {}},
		
	//Setting AVEE Voltage
	{0xB1,3,{0x05,0x05,0x05}},
	{REGFLAG_DELAY, 1, {}},
	
	//Setting VCL Voltage
	{0xB2,3,{0x00,0x00,0x00}},
	{REGFLAG_DELAY, 1, {}},
	
	//Setting VGH Voltage
	{0xB3,3,{0x0A,0x0A,0x0A}},
	{REGFLAG_DELAY, 1, {}},
		      
	//Setting VGL_REG Voltage
	{0xB5,3,{0x08,0x08,0x08}},
	{REGFLAG_DELAY, 1, {}},
			
	//BT1 Power Control for AVDD
	{0xB6,3,{0x44,0x44,0x44}},
	{REGFLAG_DELAY, 1, {}},
	
	//BT2 Power Control for AVEE
	{0xB7,3,{0x34,0x34,0x34}},
	{REGFLAG_DELAY, 1, {}},
		
	//BT3 Power Control for VCL		
	{0xB8,3,{0x35,0x35,0x35}},
	{REGFLAG_DELAY, 1, {}},
	
	//BT4 Power Control for VGH
	{0xB9,3,{0x34,0x34,0x34}},
	{REGFLAG_DELAY, 1, {}},
	    
	//BT5 Power Control for VGLX
	{0xBA,3,{0x24,0x24,0x24}},
	{REGFLAG_DELAY, 1, {}},
	   	 	
	//Setting VGMP and VGSP Voltage
	{0xBC,3,{0x00,0x68,0x00}},
	{REGFLAG_DELAY, 1, {}},
	    	 	
	//Setting VGMN and VGSN Voltage
	{0xBD,3,{0x00,0x68,0x00}},
	{REGFLAG_DELAY, 1, {}},
	   	 	
	//Setting VCOM Offset Voltage
	{0xBE,2,{0x00,0x85}},
	{REGFLAG_DELAY, 1, {}},
	 
	//VGH Output Voltage
	{0xBF,1,{0x01}},
	{REGFLAG_DELAY, 1, {}},
	 
	//Setting Gamma 2.2 Correction for Red(Positive)
	{0xd1,52,{0x00,0x00,0x00,0x0B,0x00,0x12,0x00,0x19,0x00,0x20,0x00,0x37,0x00,0x66,0x00,0xC4,0x01,0x0F,
						0x01,0x74,0x01,0xA8,0x01,0xF4,0x02,0x28,0x02,0x29,0x02,0x56,0x02,0x85,0x02,0x9E,0x02,0xBC,
						0x02,0xCF,0x02,0xE7,0x02,0xF6,0x03,0x09,0x03,0x1A,0x03,0x2F,0x03,0x66,0x03,0xE6}},
	{REGFLAG_DELAY, 1, {}},
	
	//Setting Gamma 2.2 Correction for GREEN(Positive)
	{0xd2,52,{0x00,0x00,0x00,0x0B,0x00,0x12,0x00,0x19,0x00,0x20,0x00,0x37,0x00,0x66,0x00,0xC4,0x01,0x0F,
						0x01,0x74,0x01,0xA8,0x01,0xF4,0x02,0x28,0x02,0x29,0x02,0x56,0x02,0x85,0x02,0x9E,0x02,0xBC,
						0x02,0xCF,0x02,0xE7,0x02,0xF6,0x03,0x09,0x03,0x1A,0x03,0x2F,0x03,0x66,0x03,0xE6}},
	{REGFLAG_DELAY, 1, {}},
	
	//Setting Gamma 2.2 Correction for BLUE(Positive)
	{0xd3,52,{0x00,0x00,0x00,0x0B,0x00,0x12,0x00,0x19,0x00,0x20,0x00,0x37,0x00,0x66,0x00,0xC4,0x01,0x0F,
						0x01,0x74,0x01,0xA8,0x01,0xF4,0x02,0x28,0x02,0x29,0x02,0x56,0x02,0x85,0x02,0x9E,0x02,0xBC,
						0x02,0xCF,0x02,0xE7,0x02,0xF6,0x03,0x09,0x03,0x1A,0x03,0x2F,0x03,0x66,0x03,0xE6}},
	{REGFLAG_DELAY, 1, {}},
	
	//Setting Gamma 2.2 Correction for Red(Negative)
	{0xd4,52,{0x00,0x00,0x00,0x0B,0x00,0x12,0x00,0x19,0x00,0x20,0x00,0x37,0x00,0x66,0x00,0xC4,0x01,0x0F,
						0x01,0x74,0x01,0xA8,0x01,0xF4,0x02,0x28,0x02,0x29,0x02,0x56,0x02,0x85,0x02,0x9E,0x02,0xBC,
						0x02,0xCF,0x02,0xE7,0x02,0xF6,0x03,0x09,0x03,0x1A,0x03,0x2F,0x03,0x66,0x03,0xE6}},
	{REGFLAG_DELAY, 1, {}},
	
	//Setting Gamma 2.2 Correction for GREEN(Negative)
	{0xd5,52,{0x00,0x00,0x00,0x0B,0x00,0x12,0x00,0x19,0x00,0x20,0x00,0x37,0x00,0x66,0x00,0xC4,0x01,0x0F,
						0x01,0x74,0x01,0xA8,0x01,0xF4,0x02,0x28,0x02,0x29,0x02,0x56,0x02,0x85,0x02,0x9E,0x02,0xBC,
						0x02,0xCF,0x02,0xE7,0x02,0xF6,0x03,0x09,0x03,0x1A,0x03,0x2F,0x03,0x66,0x03,0xE6}},
	{REGFLAG_DELAY, 1, {}},
	
	//Setting Gamma 2.2 Correction for BLUE(Negative)
	{0xd6,52,{0x00,0x00,0x00,0x0B,0x00,0x12,0x00,0x19,0x00,0x20,0x00,0x37,0x00,0x66,0x00,0xC4,0x01,0x0F,
						0x01,0x74,0x01,0xA8,0x01,0xF4,0x02,0x28,0x02,0x29,0x02,0x56,0x02,0x85,0x02,0x9E,0x02,0xBC,
						0x02,0xCF,0x02,0xE7,0x02,0xF6,0x03,0x09,0x03,0x1A,0x03,0x2F,0x03,0x66,0x03,0xE6}},
	{REGFLAG_DELAY, 1, {}},
	
	//Interface Pixel Format
	{0x3A,1,{0x77}},
	{REGFLAG_DELAY, 1, {}},
	
	//Sleep out
	{0x11,1,{}},
	{REGFLAG_DELAY, 200, {}},
	
	//Display On
	{0x29,1,{}},
	{REGFLAG_DELAY, 5, {}},
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {

            // Sleep Out
            {0x11, 1, {0x00}},
            {REGFLAG_DELAY, 120, {}},

            // Display ON
            {0x29, 1, {0x00}},
            {REGFLAG_DELAY, 20, {}},

            //{REGFLAG_END_OF_TABLE, 0x00, {}}
        };


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
            // Display off sequence
            {0x28, 1, {0x00}},
            {REGFLAG_DELAY, 50, {}},

            // Sleep Mode On
            {0x10, 1, {0x00}},
            {REGFLAG_DELAY, 100, {}},

            //{REGFLAG_END_OF_TABLE, 0x00, {}}
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
            //MDELAY(1);//MDELAY(10);
        }
    }

}


static void init_lcm_registers(void)
{
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
    //enable tearing-free
		//params->dbi.te_mode				= LCM_DBI_TE_MODE_VSYNC_ONLY;
		//params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
    
#if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
#else
    params->dsi.mode   = SYNC_EVENT_VDO_MODE;
#endif    
    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_TWO_LANE;
    
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq	= LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding 	= LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format	  = LCM_DSI_FORMAT_RGB888;
    
    // Video mode setting		
    params->dsi.intermediat_buffer_num = 2;
    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
    
    params->dsi.word_count=FRAME_WIDTH*3;	//DSI CMD mode need set these two bellow params, different to 6577
    params->dsi.vertical_active_line=FRAME_HEIGHT;

		params->dsi.vertical_sync_active	= 4;// 3    2
		params->dsi.vertical_backporch		= 6;//5
		params->dsi.vertical_frontporch		= 14;//10; // 1  12
		params->dsi.vertical_active_line	= FRAME_HEIGHT; 
	
		params->dsi.horizontal_sync_active	= 10;// 50  2
		params->dsi.horizontal_backporch		= 50;//60
		params->dsi.horizontal_frontporch		= 20;//10;//150
		//params->dsi.horizontal_blanking_pixel	= 60;
		params->dsi.horizontal_active_pixel	= FRAME_WIDTH;
		//params->dsi.compatibility_for_nvk = 0;		// this parameter would be set to 1 if DriverIC is NTK's and when force match DSI clock for NTK's	

	  // Bit rate calculation
		//1 Every lane speed
		params->dsi.pll_div1 = 0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps  0
		params->dsi.pll_div2 = 1;		// div2=0,1,2,3;div1_real=1,2,4,4	                1
		//params->dsi.fbk_sel = 1;
		params->dsi.fbk_div = 15;   // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)
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
    SET_RESET_PIN(1);
    MDELAY(25);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(100);

		push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
    MDELAY(50);//Must > 10ms
}

static void lcm_resume(void)
{
	lcm_init();
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

static unsigned int lcm_compare_id(void)
{
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

    SET_RESET_PIN(1);  //NOTE:should reset LCM firstly
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(10);	
    
    	
    data_array[0] = 0x00000508;		
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(1);
    
    
    //*************Enable CMD2 Page1  *******************//
    data_array[0]=0x00063902;
    data_array[1]=0x52AA55F0;
    data_array[2]=0x00000108;
    dsi_set_cmdq(data_array, 3, 1);
    MDELAY(10); 
    
    data_array[0] = 0x00023700;// read id return two byte,version and id
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(10); 
    
    read_reg_v2(0xC6, buffer, 2);
    id = buffer[0]<<8|buffer[1];

    Lcd_Log("\n %s sh1282 0x%x , 0x%x , 0x%x \n",__func__,buffer[0],buffer[1],id);

	
    return (id == LCM_ID)?1:0;
    //return (id == LCM_ID&&get_lcd_id()==1)?1:0;
}

static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK
	unsigned char buffer[6];
	unsigned int data_array[16];
	unsigned int id = 0;
	unsigned int idD0 = 0;
	unsigned int idD1 = 0;
	unsigned int idD2 = 0;
	unsigned int idD3 = 0;
	unsigned int idB0 = 0;
	unsigned int idB1 = 0;
	unsigned int idB2 = 0;
	unsigned int idB3 = 0;
	unsigned int idB4 = 0;

	if(lcm_esd_test)
	{
	    lcm_esd_test = FALSE;
	    return TRUE;
	}

	data_array[0] = 0x00023700;
	dsi_set_cmdq(data_array, 1, 1);
	read_reg_v2(0x0a, buffer, 2);
	id = buffer[0];

//	printk("[%s] hct_sh1282_dsi_vdo_fwvga_boe esd check: id = %x\n", __FUNCTION__, id);

    data_array[0]=0x00063902;
    data_array[1]=0x52AA55F0;
    data_array[2]=0x00000108;
    dsi_set_cmdq(data_array, 3, 1);
    
    data_array[0] = 0x00043700;// read id return two byte,version and id
    dsi_set_cmdq(data_array, 1, 1);
    
    read_reg_v2(0xD0, buffer, 4);
    idD0 = buffer[0];
		idD1 = buffer[1];
		idD2 = buffer[2];
		idD3 = buffer[3];
//	printk("[%s] hct_sh1282_dsi_vdo_fwvga_boe esd check: idD0 = %x\n", __FUNCTION__, idD0);
	
//	printk("[%s] hct_sh1282_dsi_vdo_fwvga_boe esd check: idD1 = %x\n", __FUNCTION__, idD1);
	
//	printk("[%s] hct_sh1282_dsi_vdo_fwvga_boe esd check: idD2 = %x\n", __FUNCTION__, idD2);
	
//	printk("[%s] hct_sh1282_dsi_vdo_fwvga_boe esd check: idD3 = %x\n", __FUNCTION__, idD3);
		data_array[0]=0x00063902;
    data_array[1]=0x52AA55F0;
    data_array[2]=0x00000008;
    dsi_set_cmdq(data_array, 3, 1);
    data_array[0] = 0x00053700;// read id return two byte,version and id
    dsi_set_cmdq(data_array, 1, 1);
    
    read_reg_v2(0xB0, buffer, 5);
    idB0 = buffer[0];
		idB1 = buffer[1];
		idB2 = buffer[2];
		idB3 = buffer[3];	
		idB4 = buffer[4];
//	printk("[%s] hct_sh1282_dsi_vdo_fwvga_boe esd check: idB0 = %x\n", __FUNCTION__, idB0);
	
//	printk("[%s] hct_sh1282_dsi_vdo_fwvga_boe esd check: idB1 = %x\n", __FUNCTION__, idB1);
	
//	printk("[%s] hct_sh1282_dsi_vdo_fwvga_boe esd check: idB2 = %x\n", __FUNCTION__, idB2);
	
//	printk("[%s] hct_sh1282_dsi_vdo_fwvga_boe esd check: idB3 = %x\n", __FUNCTION__, idB3);
	
//	printk("[%s] hct_sh1282_dsi_vdo_fwvga_boe esd check: idB4 = %x\n", __FUNCTION__, idB4);
	
	if((id == 0x9c)&&(idD0==0x0b)&&(idD1==0x0f)&&(idD2==0x10)&&(idD3==0x10)&&(idB0==0x00)&&(idB1==0x1E)&&(idB2==0x0a)&&(idB3==0x64)&&(idB4==0x0A))
	{
	  return FALSE;
	}
	else
	{            
	    return TRUE;
	}
#endif
}

static unsigned int lcm_esd_recover(void)
{
    lcm_init();
    return TRUE;
}


LCM_DRIVER SH1282_shengji_SJWV40N0247C0_IVO_DSI_2_WVGA_lcm_drv =
{
    .name			= "SH1282_shengji_SJWV40N0247C0_IVO_DSI_2_WVGA",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
#if (LCM_DSI_CMD_MODE)       
     .update         = lcm_update,
#endif   
//				.esd_check   = lcm_esd_check,	
//    		.esd_recover   = lcm_esd_recover,		
		.compare_id    = lcm_compare_id,
};
