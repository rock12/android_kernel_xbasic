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

#define LCM_DSI_CMD_MODE									0
#define LCM_RM68190_ID 						(0x8911)
#define GPIO_LCD_ID_PIN 								97
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


#define LCM_ID       (0x71)
#define LCM_ID1       (0xC1)
#define LCM_ID2       (0x80)
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

	 {0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
{0xB1,1,{0xFC}},
{0xB4,1,{0x10}},
{0xB7,2,{0x22,0x22}},
{0xB8,4,{0x01,0x03,0x03,0x03}},
{0xBC,3,{0x02,0x02,0x02}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},
{0xD1,52,{0x00,0x00,0x00,0x2A,0x00,0x57,0x00,0x75,0x00,0x8D,0x00,0xB1,0x00,0xCF,0x00,0xFD,0x01,0x22,0x01,0x5D,0x01,0x8C,0x01,0xD6,0x02,0x14,0x02,0x15,0x02,0x4F,0x02,0x8D,0x02,0xB2,0x02,0xE2,0x03,0x01,0x03,0x28,0x03,0x40,0x03,0x5B,0x03,0x6B,0x03,0x7A,0x03,0x84,0x03,0x85}},
{0xD2,52,{0x00,0x00,0x00,0x2A,0x00,0x57,0x00,0x75,0x00,0x8D,0x00,0xB1,0x00,0xCF,0x00,0xFD,0x01,0x22,0x01,0x5D,0x01,0x8C,0x01,0xD6,0x02,0x14,0x02,0x15,0x02,0x4F,0x02,0x8D,0x02,0xB2,0x02,0xE2,0x03,0x01,0x03,0x28,0x03,0x40,0x03,0x5B,0x03,0x6B,0x03,0x7A,0x03,0x84,0x03,0x85}},
{0xD3,52,{0x00,0x00,0x00,0x2A,0x00,0x57,0x00,0x75,0x00,0x8D,0x00,0xB1,0x00,0xCF,0x00,0xFD,0x01,0x22,0x01,0x5D,0x01,0x8C,0x01,0xD6,0x02,0x14,0x02,0x15,0x02,0x4F,0x02,0x8D,0x02,0xB2,0x02,0xE2,0x03,0x01,0x03,0x28,0x03,0x40,0x03,0x5B,0x03,0x6B,0x03,0x7A,0x03,0x84,0x03,0x85}},
{0xD4,52,{0x00,0x00,0x00,0x2A,0x00,0x57,0x00,0x75,0x00,0x8D,0x00,0xB1,0x00,0xCF,0x00,0xFD,0x01,0x22,0x01,0x5D,0x01,0x8C,0x01,0xD6,0x02,0x14,0x02,0x15,0x02,0x4F,0x02,0x8D,0x02,0xB2,0x02,0xE2,0x03,0x01,0x03,0x28,0x03,0x40,0x03,0x5B,0x03,0x6B,0x03,0x7A,0x03,0x84,0x03,0x85}},
{0xD5,52,{0x00,0x00,0x00,0x2A,0x00,0x57,0x00,0x75,0x00,0x8D,0x00,0xB1,0x00,0xCF,0x00,0xFD,0x01,0x22,0x01,0x5D,0x01,0x8C,0x01,0xD6,0x02,0x14,0x02,0x15,0x02,0x4F,0x02,0x8D,0x02,0xB2,0x02,0xE2,0x03,0x01,0x03,0x28,0x03,0x40,0x03,0x5B,0x03,0x6B,0x03,0x7A,0x03,0x84,0x03,0x85}},
{0xD6,52,{0x00,0x00,0x00,0x2A,0x00,0x57,0x00,0x75,0x00,0x8D,0x00,0xB1,0x00,0xCF,0x00,0xFD,0x01,0x22,0x01,0x5D,0x01,0x8C,0x01,0xD6,0x02,0x14,0x02,0x15,0x02,0x4F,0x02,0x8D,0x02,0xB2,0x02,0xE2,0x03,0x01,0x03,0x28,0x03,0x40,0x03,0x5B,0x03,0x6B,0x03,0x7A,0x03,0x84,0x03,0x85}},
{0xB0,3,{0x0D,0x0D,0x0D}},
{0xB1,3,{0x0D,0x0D,0x0D}},
{0xB3,3,{0x09,0x09,0x09}},//ADD
{0xB5,3,{0x08,0x08,0x08}},//ADD
{0xB6,3,{0x34,0x34,0x34}},
{0xB7,3,{0x34,0x34,0x34}},
{0xB8,3,{0x24,0x24,0x24}},//ADD
{0xB9,3,{0x34,0x34,0x34}},
{0xBA,3,{0x24,0x24,0x24}},
{0xBC,3,{0x00,0x98,0x00}},
{0xBD,3,{0x00,0x98,0x00}},
{0xBE,2,{0x00,0xA0}},// VCOM 
{0xBF,1,{0x01}},

{0x35,1,{0x00}},
{0x44,2,{0x00,0x90}},
{0x11,0,{0x00}},
{REGFLAG_DELAY,100,{}},
{0x29,0,{0x00}},
{REGFLAG_DELAY,20,{}},
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
    {REGFLAG_DELAY, 100, {}},
    // Sleep Mode On
	{0x10, 0, {0x00}},
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
		
		/* ESD or noise interference recovery For video mode LCM only. */ // Send TE packet to LCM in a period of n frames and check the response.
		params->dsi.lcm_int_te_monitor = FALSE;
		params->dsi.lcm_int_te_period = 1; // Unit : frames
		
		// Need longer FP for more opportunity to do int. TE monitor applicably.
		if (params->dsi.lcm_int_te_monitor)
		params->dsi.vertical_frontporch *= 2;
		
		// Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.)
		params->dsi.lcm_ext_te_monitor = FALSE;
		// Non-continuous clock
		params->dsi.noncont_clock = TRUE;
		params->dsi.noncont_clock_period = 2; // Unit : frames
}

static unsigned int lcm_compare_id(void);

static void lcm_init(void)
{
    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(50);//Must > 5ms
    SET_RESET_PIN(1);
    MDELAY(120);//Must > 50ms
		//lcm_compare_id();
#if defined(BUILD_LK)
	printf("wqcat RM68190_jingqi_JM466QH009_YOUDA_dsi_2_vdo_qhd lcm init\n");
#endif

#ifndef BUILD_LK
	printk("wqcat RM68190_jingqi_JM466QH009_YOUDA_dsi_2_vdo_qhd lcm init\n");
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

static int get_lcd_id(void)
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
}

static unsigned int lcm_compare_id(void)
{
  /* unsigned int lcd_id = 0;

   lcd_id = ((mt_get_gpio_in(GPIO_LCD_ID1_PIN) << 1) & 0x2) | mt_get_gpio_in(GPIO_LCD_ID2_PIN);

#if defined(BUILD_LK)
    printf("crystal_lcd_id =%x\n", lcd_id);	
    printf("crystal_lcd_id_pin1 =%x\n", mt_get_gpio_in(GPIO_LCD_ID1_PIN));	
    printf("crystal_lcd_id_pin2 =%x\n", mt_get_gpio_in(GPIO_LCD_ID2_PIN));
#else
    printk("crystal_lcd_id =%x\n", lcd_id);	
    printk("crystal_lcd_id_pin1 =%x\n", mt_get_gpio_in(GPIO_LCD_ID1_PIN));	
    printk("crystal_lcd_id_pin2 =%x\n", mt_get_gpio_in(GPIO_LCD_ID2_PIN));	
#endif 
   return (lcd_id  == 0)?1:0;*/
   return 1;
}


LCM_DRIVER RM68171_yihua_YH_397MD3902N0_CMI_DSI_2_WVGA_VDO_lcm_drv = 
{
    .name			= "RM68171_yihua_YH_397MD3902N0_CMI_DSI_2_WVGA_VDO",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id    = lcm_compare_id,
};

