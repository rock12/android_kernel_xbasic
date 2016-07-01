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

#define LCM_ID 									(0x6818)
#define GPIO_LCD_ID_PIN 								97
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



static struct LCM_setting_table lcm_initialization_setting[] = {

	{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
	{0xB1,1,{0xFC}},
	{0xB4,1,{0x10}},
	{0xB6,1,{0x01}},
	{0xB8,4,{0x01,0x04,0x04,0x04}},
	{0xBC,3,{0x00,0x00,0x00}},
	{0xBD,5,{0x01,0x10,0x0A,0x07,0x00}},
	{0xBE,5,{0x01,0x10,0x0A,0x07,0x00}},
	{0xBF,5,{0x01,0x10,0x0A,0x07,0x00}},
	{0xC9,2,{0xC0,0x01}},
	
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},
	{0xB0,3,{0x05,0x05,0x05}},
	{0xB1,3,{0x05,0x05,0x05}},
	{0xB6,3,{0x34,0x34,0x34}},
	{0xB7,3,{0x34,0x34,0x34}},
	{0xB8,3,{0x24,0x24,0x24}},
	{0xB9,3,{0x34,0x34,0x34}},
	{0xBA,3,{0x04,0x04,0x04}},
	{0xBC,3,{0x00,0x96,0x02}},
	{0xBD,3,{0x00,0x98,0x00}},
	{0xBE,2,{0x00,0x63}},  //3d
	{0xCC,1,{0x05}},
	{0xD1,52,{0x00,0x00,0x00,0x06,0x00,0x1A,0x00,0x3F,0x00,0x66,0x00,0xAD,0x00,0xE1,0x01,0x29,0x01,0x58,0x01,0x97,0x01,0xC2,0x01,0xFF,0x02,0x2D,0x02,0x2F,0x02,0x58,0x02,0x81,0x02,0x98,0x02,0xB3,0x02,0xC4,0x02,0xDB,0x02,0xE9,0x02,0xFD,0x03,0x0B,0x03,0x21,0x03,0x55,0x03,0xF8}},
	{0xD2,52,{0x00,0x00,0x00,0x06,0x00,0x1A,0x00,0x3F,0x00,0x66,0x00,0xAD,0x00,0xE1,0x01,0x29,0x01,0x58,0x01,0x97,0x01,0xC2,0x01,0xFF,0x02,0x2D,0x02,0x2F,0x02,0x58,0x02,0x81,0x02,0x98,0x02,0xB3,0x02,0xC4,0x02,0xDB,0x02,0xE9,0x02,0xFD,0x03,0x0B,0x03,0x21,0x03,0x55,0x03,0xF8}},
	{0xD3,52,{0x00,0x00,0x00,0x06,0x00,0x1A,0x00,0x3F,0x00,0x66,0x00,0xAD,0x00,0xE1,0x01,0x29,0x01,0x58,0x01,0x97,0x01,0xC2,0x01,0xFF,0x02,0x2D,0x02,0x2F,0x02,0x58,0x02,0x81,0x02,0x98,0x02,0xB3,0x02,0xC4,0x02,0xDB,0x02,0xE9,0x02,0xFD,0x03,0x0B,0x03,0x21,0x03,0x55,0x03,0xF8}},
	{0xD4,52,{0x00,0x00,0x00,0x06,0x00,0x1A,0x00,0x3F,0x00,0x66,0x00,0xAD,0x00,0xE1,0x01,0x29,0x01,0x58,0x01,0x97,0x01,0xC2,0x01,0xFF,0x02,0x2D,0x02,0x2F,0x02,0x58,0x02,0x81,0x02,0x98,0x02,0xB3,0x02,0xC4,0x02,0xDB,0x02,0xE9,0x02,0xFD,0x03,0x0B,0x03,0x21,0x03,0x55,0x03,0xF8}},
	{0xD5,52,{0x00,0x00,0x00,0x06,0x00,0x1A,0x00,0x3F,0x00,0x66,0x00,0xAD,0x00,0xE1,0x01,0x29,0x01,0x58,0x01,0x97,0x01,0xC2,0x01,0xFF,0x02,0x2D,0x02,0x2F,0x02,0x58,0x02,0x81,0x02,0x98,0x02,0xB3,0x02,0xC4,0x02,0xDB,0x02,0xE9,0x02,0xFD,0x03,0x0B,0x03,0x21,0x03,0x55,0x03,0xF8}},
	{0xD6,52,{0x00,0x00,0x00,0x06,0x00,0x1A,0x00,0x3F,0x00,0x66,0x00,0xAD,0x00,0xE1,0x01,0x29,0x01,0x58,0x01,0x97,0x01,0xC2,0x01,0xFF,0x02,0x2D,0x02,0x2F,0x02,0x58,0x02,0x81,0x02,0x98,0x02,0xB3,0x02,0xC4,0x02,0xDB,0x02,0xE9,0x02,0xFD,0x03,0x0B,0x03,0x21,0x03,0x55,0x03,0xF8}},
	
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x02}},
	{0xF6,1,{0x60}},

  {0x35,1,{0x00}},
  {0x44,2,{0x00,0x10}},
  {0x36,1,{0x00}},
	{0x3a,1,{0x77}},
	
	{0x11,1,{0x00}},
	{REGFLAG_DELAY, 60, {}},
	{0x29,1,{0x00}},
	{REGFLAG_DELAY, 10, {}},
	//{0x2C,1,{0x00}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};



static struct LCM_setting_table lcm_sleep_out_setting[] = {
  // Sleep Out
	{0x11, 1, {0x00}},
  {REGFLAG_DELAY, 60, {}},
  // Display ON
	{0x29, 1, {0x00}},
  {REGFLAG_DELAY, 10, {}},
  
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},
  {REGFLAG_DELAY, 120, {}},
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
                params->dbi.te_mode            = LCM_DBI_TE_MODE_VSYNC_ONLY;
                params->dbi.te_edge_polarity   = LCM_POLARITY_RISING;

		params->dsi.mode   = CMD_MODE;
		//params->dsi.mode   = SYNC_EVENT_VDO_MODE;

    // DSI
	/* Command mode setting */
    params->dsi.LANE_NUM=LCM_TWO_LANE;
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		params->dsi.packet_size=256;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 2;
	
                params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		params->dsi.word_count= 480*3;	
		params->dsi.vertical_active_line=800;

		// Video mode setting		
		params->dsi.line_byte=2180;		// 2256 = 752*3
		params->dsi.horizontal_sync_active_byte=26;
		params->dsi.horizontal_backporch_byte=206;
		params->dsi.horizontal_frontporch_byte=206;	
		params->dsi.rgb_byte=(480*3+6);	
	
		params->dsi.horizontal_sync_active_word_count=20;	
		params->dsi.horizontal_backporch_word_count=200;
		params->dsi.horizontal_frontporch_word_count=200;

		// Bit rate calculation
		params->dsi.pll_div1=0;		// fref=26MHz, fvco=fref*(div1+1)	(div1=0~63, fvco=500MHZ~1GHz)  25
		params->dsi.pll_div2=1;			// div2=0~15: fout=fvo/(2*div2
		params->dsi.fbk_div=14;	
}

static void lcm_init(void)
{
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
    MDELAY(5);//10
    SET_RESET_PIN(0);
    MDELAY(20);//Must > 5ms 50
    SET_RESET_PIN(1);
    MDELAY(60);//Must > 50ms 120

		Lcd_Log("wqcat RM68190_jingqi_JM466QH009_YOUDA_dsi_2_vdo_qhd lcm init\n");
		push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
    push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
/*    
#if defined(BUILD_LK)
	upmu_set_rg_vgp2_en(0);
	upmu_set_rg_vgp3_en(0);	
#else

	hwPowerDown(MT6323_POWER_LDO_VGP2,"LCM");
	hwPowerDown(MT6323_POWER_LDO_VGP3, "Lance_LCM");
#endif
	MDELAY(50);//Must > 10ms
*/
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
    mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,1);
    MDELAY(1);

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
	
	array[0]=0x00063902;
	array[1]=0x52AA55F0;
	array[2]=0x00000108;
	dsi_set_cmdq(array, 3, 1);
	MDELAY(10); 

	array[0] = 0x00033700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);
	
	read_reg_v2(0xC5, buffer, 2);
	id = ((buffer[1]<<8)&0xFF00)|(buffer[0]&0x00FF); //we only need ID

	Lcd_Log("wqcat %s, id1 = 0x%08x, id2 = 0x%08x, get_lcd_id=%d\n", __func__, buffer[0], buffer[1], get_lcd_id());
  //return (id==LCM_ID)?1:0;
  	return 1;
}


LCM_DRIVER rm68180_jiusheng_JST397C_CTC_dsi_2_vdo_wvga_lcm_drv = 
{
    .name			= "rm68180_jiusheng_JST397C_CTC_dsi_2_vdo_wvga",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.update         = lcm_update,
	.compare_id    = lcm_compare_id,
};

