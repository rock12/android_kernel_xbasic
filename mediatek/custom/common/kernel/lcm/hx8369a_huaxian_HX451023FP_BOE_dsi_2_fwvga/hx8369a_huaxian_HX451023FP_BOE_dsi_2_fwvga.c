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

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (854)
#define REGFLAG_DELAY             							0xAB
#define REGFLAG_END_OF_TABLE      							0xAA   // END OF REGISTERS MARKER
#define GPIO_LCD_ID_PIN 50


#define LCM_DSI_CMD_MODE									1


// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

#define LCM_ID       (0x69)

//static unsigned int lcm_esd_test = 0;      ///only for ESD test

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)									lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)				lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

static unsigned int lcm_compare_id();



static struct LCM_setting_table {
    unsigned char cmd;
    unsigned char count;
    unsigned char para_list[64];
};

#define DISPLAY_DIRECTION_0_MODE

static struct LCM_setting_table lcm_initialization_setting[] = {

		{0xB9,3,{0xFF,0x83,0x69}}, 
		{0xB1,19,{0x01,0x00,0x44,0x0A,0x00,0x0f,0x0f,0x2C,0x2C,0x3F,0x3F,0x01,0x23,0x01,0xE6,0xE6,0xE6,0xE6,0xE6}}, 
		{0xB2,15,{0x00,0x10,0x05,0x0A,0x70,0x00,0xFF,0x00,0x00,0x00,0x00,0x03,0x03,0x00,0x01}}, 
		{0xB4,5,{0x02,0x1D,0x50,0x00,0x00}},//02 
		{0xB6,2,{0x47,0x47}},							
		{0xcc,1,{0x02}},//0x08 
		{0x36,1,{0x00}},//0x08 
		{0xD5,26,{0x00,0x01,0x03,0x28,0x01,0x04,0x05,0x70,0x11,0x13,0x00,0x00,0x60,0x04,0x71,0x05,0x00,0x00,0x71,0x05,0x60,0x04,0x07,0x0F,0x04,0x04}}, 
		{0xE0,34,{0x00,0x20,0x26,0x34,0x38,0x3f,0x33,0x4b,0x09,0x13,0x0e,0x15,0x16,0x14,0x15,0x11,0x17,0x00,0x20,0x26,0x34,0x38,0x3f,0x33,0x4b,0x09,0x13,0x0e,0x15,0x16,0x14,0x15,0x11,0x17}}, 
		{0x3A,1,{0x77}}, 
		{0xBA,13,{0x00,0xA0,0xC6,0x80,0x0A,0x00,0x10,0x30,0x6F,0x02,0x11,0x18,0x40}},				  
		{REGFLAG_DELAY, 1, {}}, 
		{0x35,		  1,		{0x00}}, 
		{0x20,		  1,		{0x00}}, 
		{0x11,1,{0X00}}, //Sleep Out 
		{REGFLAG_DELAY, 120, {}}, 
		{0x29,1,{0X00}}, //Display On 
		{REGFLAG_DELAY, 120, {}}, 
		{0x2C,1,{0X00}}, //Display On 

		{REGFLAG_END_OF_TABLE, 0x00, {}}, 


        };


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
    params->dbi.te_edge_polarity       = LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
    params->dsi.mode   = CMD_MODE;
#else
    params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif

    // DSI
    /* Command mode setting */

    params->dsi.LANE_NUM=LCM_TWO_LANE;
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;
    
    params->dsi.intermediat_buffer_num = 0;//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
    
    params->dsi.word_count=FRAME_WIDTH*3;	//DSI CMD mode need set these two bellow params, different to 6577
    params->dsi.vertical_active_line=FRAME_HEIGHT;
    params->dsi.compatibility_for_nvk = 0;		// this parameter would be set to 1 if DriverIC is NTK's and when force match DSI clock for NTK's

    // Bit rate calculation
    params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4
    params->dsi.pll_div2=0;		// div2=0,1,2,3;div2_real=1,2,4,4
    params->dsi.fbk_div =13;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)

}

static void init_lcm_registers(void)
{
	unsigned int data_array[25];
	
	 data_array[0]= 0x00043902; 
        data_array[1]= 0x6983FFB9; 
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(10);   
        
        data_array[0]= 0x00143902; 
        data_array[1]= 0x240001b1;//220001b1;
        data_array[2]= 0x0f0f000a;
        data_array[3]= 0x3f3f2a2a;
        data_array[4]= 0xe6011301;
        data_array[5]= 0xe6e6e6e6;
        dsi_set_cmdq(&data_array, 6, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x00103902; 
        data_array[1]= 0x051000b2;
        data_array[2]= 0xff00700a;
        data_array[3]= 0x00000000;
        data_array[4]= 0x01000303;
        dsi_set_cmdq(&data_array, 5, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x00063902; 
        data_array[1]= 0x841d00b4;//801d00b4;
        data_array[2]= 0x00000000;
        dsi_set_cmdq(&data_array, 3, 1); 
        MDELAY(1); 

 	
       
        
        data_array[0]= 0x00023902; 
        data_array[1]= 0x000002cc;
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 

          data_array[0]= 0x00023902; 
        data_array[1]= 0x00000036;
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x00033902; 
        data_array[1]= 0x004c4cb6;
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x001b3902; 
        data_array[1]= 0x030100d5;
        data_array[2]= 0x05040128;
        data_array[3]= 0x00131170;
        data_array[4]= 0x71046000;
        data_array[5]= 0x71000005;
        data_array[6]= 0x07046005;
        data_array[7]= 0x0004040f;
        dsi_set_cmdq(&data_array, 8, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x00233902; 
        data_array[1]= 0x262000e0;
        data_array[2]= 0x333f3834;
        data_array[3]= 0x0e13094b;
        data_array[4]= 0x15141615;
        data_array[5]= 0x20001711;
        data_array[6]= 0x3f383426;
        data_array[7]= 0x13094b33;
        data_array[8]= 0x1416150e;
        data_array[9]= 0x00171115;
        dsi_set_cmdq(&data_array, 10, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x00023902; 
        data_array[1]= 0x0000773a;
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x000e3902; 
        data_array[1]= 0xc6a000ba;
        data_array[2]= 0x10020a80;
        data_array[3]= 0x11026c30;
        data_array[4]= 0x00004018;
        dsi_set_cmdq(&data_array, 5, 1); 
        MDELAY(1); 
        
        data_array[0]= 0x00023902; 
        data_array[1]= 0x00000035;
        dsi_set_cmdq(&data_array, 2, 1); 
        MDELAY(1); 

        data_array[0] = 0x00110500;        //exit sleep mode 
        dsi_set_cmdq(&data_array, 1, 1); 
        MDELAY(150);   

        data_array[0] = 0x00290500;        //exit sleep mode 
        dsi_set_cmdq(&data_array, 1, 1); 
        MDELAY(20); 

        data_array[0] = 0x002c0500;        //exit sleep mode 
        dsi_set_cmdq(&data_array, 1, 1); 
        MDELAY(20); 
	
	 
	
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
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(120);

    //push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
		init_lcm_registers();
		
}


static void lcm_suspend(void)
{
    //push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
     SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(120);
}


static void lcm_resume(void)
{

  	init_lcm_registers();
    //push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);


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
//	return 0;
	
    unsigned int id = 0, id2 = 0;
    unsigned char buffer[2];
    unsigned int data_array[16];

#ifdef BUILD_LK
    pmic_config_interface(0x0532,5,0x7,5);//add by libo for VGP2 POWER ON
    pmic_config_interface(0x050C,1,0x1,15);
#else
    hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
#endif
    MDELAY(100);

#if 1
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
    return (get_lcd_id()==0)?1:0;
#endif
}

#if 0
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
	 	
	array[0] = 0x00023700;
	dsi_set_cmdq(array, 1, 1);
	//MDELAY(20);
	read_reg_v2(0x0A, buffer, 2);
	 
	printk("mycat$$  hx8369a_huaxian_HX451023FP_BOE_dsi_2_fwvga ### esd check buffer[0] = 0x%x\r\n",buffer[0]);
	
	if(buffer[0] == 0x9C)//9C
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
#endif

LCM_DRIVER hx8369a_huaxian_HX451023FP_BOE_dsi_2_fwvga_lcm_drv =
    {
        .name			= "hx8369a_huaxian_HX451023FP_BOE_dsi_2_fwvga",
        .set_util_funcs = lcm_set_util_funcs,
        .compare_id    = lcm_compare_id,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
#if (LCM_DSI_CMD_MODE)
        .update         = lcm_update,
#endif
//#if defined (BIRD_ESD_CHECK)
//	.esd_check = lcm_esd_check,
//  .esd_recover = lcm_esd_recover,
//#endif
    };
