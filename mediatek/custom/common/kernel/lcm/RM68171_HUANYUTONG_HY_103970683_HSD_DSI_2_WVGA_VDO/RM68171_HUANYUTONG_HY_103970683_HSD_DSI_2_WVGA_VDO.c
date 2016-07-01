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
//#define BIRD_ESD_CHECK

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


#define LCM_ID       (0x8011)
static unsigned int lcm_esd_test = 0;      ///only for ESD test


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    
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


static struct LCM_setting_table lcm_compare_id_setting[] = {
	// Display off sequence
	{0xf0, 5, {0x55, 0xaa, 0x52, 0x08, 0x01}},
	{REGFLAG_DELAY, 10, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
static struct LCM_setting_table lcm_initialization_setting[] = {

{0xF0,5,{0x55,0xAA,0x52,0x08,0x02}},
{0xF6,1,{0x60}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},
{0xB0,3,{0x0D,0x0D,0x0D}},
{0xB1,3,{0x0D,0x0D,0x0D}},
{0xB3,3,{0x0F,0x0F,0x0F}},
{0xB5,3,{0x08,0x08,0x08}},
{0xB6,3,{0x34,0x34,0x34}},
{0xB7,3,{0x34,0x34,0x34}},
{0xB8,3,{0x24,0x24,0x24}},
{0xB9,3,{0x34,0x34,0x34}},
{0xBA,3,{0x24,0x24,0x24}},
{0xBC,3,{0x00,0x78,0x13}},
{0xBD,3,{0x00,0x78,0x13}},
{0xBE,2,{0x00,0x7D}},//vcom  5F
{0xBF,1,{0x01}},
{0xD1,52,{0x00,0x00,0x00,0x01,0x00,0x0D,0x00,0x2C,0x00,0x4D,0x00,0x8A,0x00,0xBB,0x01,0x02,0x01,0x36,0x01,0x7D,0x01,0xB0,0x02,0x00,0x02,0x41,0x02,0x43,0x02,0x7E,0x02,0xBC,0x02,0xDF,0x03,0x08,0x03,0x21,0x03,0x40,0x03,0x53,0x03,0x6E,0x03,0x80,0x03,0x9A,0x03,0xC8,0x03,0xFF}},
{0xD2,52,{0x00,0x00,0x00,0x01,0x00,0x0D,0x00,0x2C,0x00,0x4D,0x00,0x8A,0x00,0xBB,0x01,0x02,0x01,0x36,0x01,0x7D,0x01,0xB0,0x02,0x00,0x02,0x41,0x02,0x43,0x02,0x7E,0x02,0xBC,0x02,0xDF,0x03,0x08,0x03,0x21,0x03,0x40,0x03,0x53,0x03,0x6E,0x03,0x80,0x03,0x9A,0x03,0xC8,0x03,0xFF}},
{0xD3,52,{0x00,0x00,0x00,0x01,0x00,0x0D,0x00,0x2C,0x00,0x4D,0x00,0x8A,0x00,0xBB,0x01,0x02,0x01,0x36,0x01,0x7D,0x01,0xB0,0x02,0x00,0x02,0x41,0x02,0x43,0x02,0x7E,0x02,0xBC,0x02,0xDF,0x03,0x08,0x03,0x21,0x03,0x40,0x03,0x53,0x03,0x6E,0x03,0x80,0x03,0x9A,0x03,0xC8,0x03,0xFF}},
{0xD4,52,{0x00,0x00,0x00,0x01,0x00,0x0D,0x00,0x2C,0x00,0x4D,0x00,0x8A,0x00,0xBB,0x01,0x02,0x01,0x36,0x01,0x7D,0x01,0xB0,0x02,0x00,0x02,0x41,0x02,0x43,0x02,0x7E,0x02,0xBC,0x02,0xDF,0x03,0x08,0x03,0x21,0x03,0x40,0x03,0x53,0x03,0x6E,0x03,0x80,0x03,0x9A,0x03,0xC8,0x03,0xFF}},
{0xD5,52,{0x00,0x00,0x00,0x01,0x00,0x0D,0x00,0x2C,0x00,0x4D,0x00,0x8A,0x00,0xBB,0x01,0x02,0x01,0x36,0x01,0x7D,0x01,0xB0,0x02,0x00,0x02,0x41,0x02,0x43,0x02,0x7E,0x02,0xBC,0x02,0xDF,0x03,0x08,0x03,0x21,0x03,0x40,0x03,0x53,0x03,0x6E,0x03,0x80,0x03,0x9A,0x03,0xC8,0x03,0xFF}},
{0xD6,52,{0x00,0x00,0x00,0x01,0x00,0x0D,0x00,0x2C,0x00,0x4D,0x00,0x8A,0x00,0xBB,0x01,0x02,0x01,0x36,0x01,0x7D,0x01,0xB0,0x02,0x00,0x02,0x41,0x02,0x43,0x02,0x7E,0x02,0xBC,0x02,0xDF,0x03,0x08,0x03,0x21,0x03,0x40,0x03,0x53,0x03,0x6E,0x03,0x80,0x03,0x9A,0x03,0xC8,0x03,0xFF}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
{0xB4,1,{0x10}},
{0xB8,4,{0x01,0x01,0x01,0x01}},
{0xBC,3,{0x02,0x02,0x02}},
{0x36,1,{0x00}},
{0x3A,1,{0x77}},
{0x11,0,{0x00}},
{REGFLAG_DELAY, 120, {}},
{0x29,0,{0x00}},
{REGFLAG_DELAY, 100, {}},

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
    params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;  //LCM_DBI_TE_MODE_DISABLED;
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

		params->dsi.vertical_sync_active = 3;
		params->dsi.vertical_backporch = 20;
		params->dsi.vertical_frontporch = 20;
		params->dsi.vertical_active_line = FRAME_HEIGHT;
		
		params->dsi.horizontal_sync_active = 10;
		params->dsi.horizontal_backporch = 50;
		params->dsi.horizontal_frontporch = 50;	//21;
		params->dsi.horizontal_blanking_pixel = 60;	
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
		
		// Bit rate calculation
		params->dsi.pll_div1 = 1;	// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		params->dsi.pll_div2 = 0;	// div2=0,1,2,3;div1_real=1,2,4,4
		params->dsi.fbk_div = 14;	//15;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)
		
		/* ESD or noise interference recovery For video mode LCM only. */ // Send TE packet to LCM in a period of n frames and check the response.
		//params->dsi.lcm_int_te_monitor = FALSE;
	//	params->dsi.lcm_int_te_period = 1; // Unit : frames
		
		// Need longer FP for more opportunity to do int. TE monitor applicably.
	//	if (params->dsi.lcm_int_te_monitor)
	//	params->dsi.vertical_frontporch *= 2;
		
		// Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.)
	//	params->dsi.lcm_ext_te_monitor = FALSE;
		// Non-continuous clock
	//	params->dsi.noncont_clock = TRUE;
		//params->dsi.noncont_clock_period = 2; // Unit : frames
		
			/*for video esd begin*/
	params->dsi.lcm_int_te_monitor = 0;//TRUE;
	params->dsi.lcm_int_te_period = 1;              // Unit : frames
	// Need longer FP for more opportunity to do int. TE monitor applicably.
	if(params->dsi.lcm_int_te_monitor)
	params->dsi.vertical_frontporch *= 2;
	/*\u6253\u5f00\u5916\u90e8esd*/
	// Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.)
	params->dsi.lcm_ext_te_monitor = 0;
	// Non-continuous clock
	params->dsi.noncont_clock = 1;
	params->dsi.noncont_clock_period = 2;   // Unit : frames	
	/*for video esd end*/
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
        SET_RESET_PIN(0);
	MDELAY(80); //10  50 
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
	id = ((buffer[0]<<8)&0xFF00)|(buffer[0]&0x00FF); //we only need ID
#if defined(BUILD_LK)
	printf("wqcat %s, id1 = 0x%08x, id2 = 0x%08x, get_lcd_id=%d\n", __func__, buffer[0], buffer[1], get_lcd_id());
#endif

#ifndef BUILD_LK
	printk("wqcat %s, id1 = 0x%08x, id2 = 0x%08x, get_lcd_id=%d\n", __func__, buffer[0], buffer[1], get_lcd_id());
#endif
  return (id==LCM_ID)?1:0;

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
	
	push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
	
#ifndef BUILD_LK	
	printk("mycat songtuirui ### lcm_esd_recover \r\n");
#endif
	
	return 1;
}
#endif


LCM_DRIVER RM68171_HUANYUTONG_HY_103970683_HSD_DSI_2_WVGA_VDO_lcm_drv = 
{
    .name			= "RM68171_HUANYUTONG_HY_103970683_HSD_DSI_2_WVGA_VDO",
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

