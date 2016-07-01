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
#include <mach/upmu_common.h>
   
#endif
#include "lcm_drv.h"
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(480)
#define FRAME_HEIGHT 										(854)

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#define LCM_ID_OTM8018B	0x8009

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
static unsigned int lcm_esd_test = FALSE; 
static LCM_UTIL_FUNCS lcm_util = {0};

#if 0
#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))
#else
#define SET_RESET_PIN(v)      do{\
                                mt_set_gpio_mode(GPIO59, 0);\
	                              mt_set_gpio_dir(GPIO59, 1);\
	                              mt_set_gpio_out(GPIO59, v);\
                                }while(0)
#endif 

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
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

 struct LCM_setting_table {
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
	
	{0x00, 1 , {0x00}},
	{0xFF,  3 ,{0x80,0x09,0x01}},

	{0x00, 1 , {0x80}},
	{0xFF,  2 ,{0x80,0x09}},
	{0x00,  3 ,{0xD2,0xD2,0xD2}},
	{0xB0,  1 ,{0x01}},

	{0x00, 1 , {0x80}},
	{0xC4,  1 ,{0x30}},
	{REGFLAG_DELAY, 10, {}},

	{0x00, 1 , {0x8A}},
	{0xC4,  1 ,{0x40}},
	{REGFLAG_DELAY, 10, {}},

	{0x00, 1 , {0xB4}},
	{0xC0,  1 ,{0x50}},

	{0x00, 1 , {0x82}},
	{0xC5,  1 ,{0xA3}},

	{0x00, 1 , {0x90}},
	{0xC5,  2 ,{0x96,0x87}},

	{0x00, 1 , {0x00}},
	{0xD8,  2 ,{0x87,0x87}},

	{0x00, 1 , {0x00}},
	{0xD9,  1 ,{0x45}},

	{0x00, 1 , {0x81}},
	{0xC1,  1 ,{0x66}},

	{0x00, 1 , {0x89}},
	{0xC4,  1 ,{0x08}},

	{0x00, 1 , {0xA3}},
	{0xC0,  1 ,{0x00}},

	{0x00, 1 , {0x81}},
	{0xC4,  1 ,{0x83}},

	{0x00, 1 , {0x80}},
	{0xC4,  1 ,{0x30}},

	{0x00, 1 , {0x92}},
	{0xC5,  1 ,{0x01}},

	{0x00, 1 , {0xB1}},
	{0xC5,  1 ,{0xA9}},

	{0x00, 1 , {0xC7}},
	{0xCF,  1 ,{0x02}},

	{0x00, 1 , {0x90}},
	{0xB3,  1 ,{0x02}},

	{0x00, 1 , {0x92}},
	{0xB3,  1 ,{0x45}},

	{0x00, 1 , {0x80}},
	{0xC0,  9 ,{0x00,0x58,0x00,0x15,0x15,0x00,0x58,0x15,0x15}},

	{0x00, 1 , {0x90}},
	{0xC0,  6 ,{0x00,0x56,0x00,0x00,0x00,0x03}},

	{0x00, 1 , {0xA6}},
	{0xC1,  3 ,{0x01,0x00,0x00}},

	{0x00, 1 , {0x80}},
	{0xCE, 12 ,{0x87,0x03,0x00,0x86,0x03,0x00,0x85,0x03,0x00,0x84,0x03,0x00}},

	{0x00, 1 , {0x90}},
	{0xCE, 14 ,{0x33,0x52,0x00,0x33,0x53,0x00,0x33,0x54,0x00,0x33,0x55,0x00,0x00,0x00}},

	{0x00, 1 , {0xA0}},
	{0xCE, 14 ,{0x38,0x05,0x03,0x56,0x00,0x00,0x00,0x38,0x04,0x03,0x57,0x00,0x00,0x00}},

	{0x00, 1 , {0xB0}},
	{0xCE, 14 ,{0x38,0x03,0x03,0x58,0x00,0x00,0x00,0x38,0x02,0x03,0x59,0x00,0x00,0x00}},

	{0x00, 1 , {0xC0}},
	{0xCE, 14 ,{0x38,0x01,0x03,0x5A,0x00,0x00,0x00,0x38,0x00,0x03,0x5C,0x00,0x00,0x00}},

	{0x00, 1 , {0xD0}},
	{0xCE, 14 ,{0x30,0x00,0x03,0x5C,0x00,0x00,0x00,0x30,0x01,0x03,0x5D,0x00,0x00,0x00}},

	{0x00, 1 , {0xC3}},
	{0xCB,  8 ,{0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04}},

	{0x00, 1 , {0xD8}},
	{0xCB,  7 ,{0x04,0x04,0x04,0x04,0x04,0x04,0x04}},

	{0x00, 1 , {0xE0}},
	{0xCB,  1 ,{0x04}},

	{0x00, 1 , {0x83}},
	{0xCC,  7 ,{0x03,0x01,0x09,0x0B,0x0D,0x0F,0x05}},

	{0x00, 1 , {0x90}},
	{0xCC,  1 ,{0x07}},

	{0x00, 1 , {0x9D}},
	{0xCC,  2 ,{0x04,0x02}},

	{0x00, 1 , {0xA0}},
	{0xCC,  6 ,{0x0A,0x0C,0x0E,0x10,0x06,0x08}},

	{0x00, 1 , {0xB3}},
	{0xCC,  7 ,{0x06,0x08,0x0A,0x10,0x0E,0x0C,0x04}},

	{0x00, 1 , {0xC0}},
	{0xCC,  1 ,{0x02}},

	{0x00, 1 , {0xCD}},
	{0xCC,  2 ,{0x05,0x07}},

	{0x00, 1 , {0xD0}},
	{0xCC,  6 ,{0x09,0x0F,0x0D,0x0B,0x03,0x01}},

	{0x00, 1 , {0x00}},
	{0xE1, 16 ,{0x09,0x0F,0x15,0x0A,0x04,0x1D,0x0E,0x0F,0x00,0x04,0x01,0x08,0x0F,0x1F,0x1C,0x12}},

	{0x00, 1 , {0x00}},
	{0xE2, 16 ,{0x09,0x0F,0x15,0x0A,0x04,0x1D,0x0E,0x0F,0x00,0x04,0x02,0x08,0x0F,0x1E,0x1B,0x12}},

	{0x00, 1 , {0xC0}},
	{0xC5,  1 ,{0x00}},

	{0x00, 1 , {0xA0}},
	{0xC1,  1 ,{0xEA}},

	{0x00, 1 , {0xA6}},
	{0xC1,  3 ,{0x01,0x00,0x00}},

	{0x00, 1 , {0xC6}},
	{0xB0,  1 ,{0x03}},

	{0x00, 1 , {0x81}},
	{0xC5,  1 ,{0x66}},

	{0x00, 1 , {0xB6}},
	{0xF5,  1 ,{0x06}},

	{0x00, 1 , {0x8B}},
	{0xB0,  1 ,{0x40}},

	{0x00, 1 , {0xB1}},
	{0xB0,  1 ,{0x80}},

	{0x00, 1 , {0x00}},
	{0xFF,  3 ,{0xFF,0xFF,0xFF}},
	{0x13,  1,  {0x00}},
	{REGFLAG_DELAY, 10, {}},

    {0x11,  1,  {0x00}},
    {REGFLAG_DELAY, 120, {}},
    {0x29,  1,  {0x00}},
    {REGFLAG_DELAY, 20, {}},

// Setting ending by predefined flag
{REGFLAG_END_OF_TABLE, 0x00, {}}

};

static struct LCM_setting_table lcm_set_window[] = {
	{0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
	{0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {
	{0x13, 1, {0x00}},
  {REGFLAG_DELAY, 10, {}},
    // Sleep Out
	{0x11, 1, {0x00}},
  {REGFLAG_DELAY, 150, {}},
    // Display ON
	{0x29, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x22, 1, {0x00}},
	{REGFLAG_DELAY, 10, {}},	
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},
	{0x22, 1, {0x00}},
	{REGFLAG_DELAY, 10, {}},
    // Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 150, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_backlight_level_setting[] = {
	{0x51, 1, {0xFF}},
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
    params->dbi.te_mode				= LCM_DBI_TE_MODE_DISABLED;//LCM_DBI_TE_MODE_VSYNC_ONLY;
    //params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
    
    params->dsi.mode   = SYNC_PULSE_VDO_MODE;
    
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
    
    params->dsi.word_count=480*3;	//DSI CMD mode need set these two bellow params, different to 6577
    params->dsi.vertical_active_line=854;

		params->dsi.vertical_sync_active				= 4;
		params->dsi.vertical_backporch					= 16;
		params->dsi.vertical_frontporch					= 16;
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 10;//6;
		params->dsi.horizontal_backporch				= 50;
		params->dsi.horizontal_frontporch				= 50;
    	params->dsi.horizontal_blanking_pixel	= 60;
		params->dsi.horizontal_active_pixel			= FRAME_WIDTH;
    
    // Bit rate calculation
		params->dsi.pll_div1=1; 	// div1=0,1,2,3;div1_real=1,2,4,4
		params->dsi.pll_div2=1; 	// div2=0,1,2,3;div2_real=1,2,4,4
//		params->dsi.fbk_sel=1;		 // fbk_sel=0,1,2,3;fbk_sel_real=1,2,4,4
		params->dsi.fbk_div =30;//28		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)		
}

//static  char num1, num2, num3; 

static void lcm_init(void)
{
//	char buffer1[3], buffer2[3], buffer3[3], array[4];

	upmu_set_rg_vgp2_vosel(5);
	upmu_set_rg_vgp2_en(1);
	
	upmu_set_rg_vgp3_vosel(3);
	upmu_set_rg_vgp3_en(1);	

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(50);//20
	SET_RESET_PIN(1);
	MDELAY(90);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);

}


static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	
	upmu_set_rg_vgp2_en(0);
	upmu_set_rg_vgp3_en(0);

	SET_RESET_PIN(0);
	MDELAY(20);
}


static void lcm_resume(void)
{
	lcm_init();
}

static unsigned int lcm_compare_id(void)
{
	int array[4];
	char buffer[5];
	char id_high=0;
	char id_low=0;
	int id=0;

	return 1;

	upmu_set_rg_vgp2_vosel(5);
	upmu_set_rg_vgp2_en(1);
	
	upmu_set_rg_vgp3_vosel(3);
	upmu_set_rg_vgp3_en(1);	

	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(200);

	array[0] = 0x00053700;
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0xa1, buffer, 5);

	id_high = buffer[2];
	id_low = buffer[3];
	id = (id_high<<8) | id_low;

	#if defined(BUILD_UBOOT)
		//printf("OTM8018B uboot %s \n", __func__);
		//printf("%s id = 0x%08x \n", __func__, id);
	#else
		//printk("OTM8018B kernel %s \n", __func__);
		//printk("%s id = 0x%08x \n", __func__, id);
	#endif

	return (LCM_ID_OTM8018B == id)?1:0;
}

/*
static void lcm_esd_recovery(void)
{
    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(60);
    SET_RESET_PIN(1);
    MDELAY(150);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK
		char  buffer[5];
//		char  buffer2[3];
//		char  buffer3[3];
		int   array[4];

        if(lcm_esd_test)
        {
            lcm_esd_test = FALSE;
            return TRUE;
        }
		array[0] = 0x00013700;// read id return two byte,version and id
		dsi_set_cmdq(array, 1, 1);
        read_reg_v2(0x0A, buffer, 1);
        if(buffer[0] != 0x9C)
			return  TRUE;

        read_reg_v2(0x0D, buffer, 1);
        if(buffer[0] != 0x00)
			return  TRUE;

        read_reg_v2(0x0E, buffer, 1);
        if(buffer[0] != 0x80)
			return  TRUE;
		
		return  FALSE;
#if 0
		array[0] = 0x00013700;// read id return two byte,version and id
		dsi_set_cmdq(array, 1, 1);
        read_reg_v2(0x0D, buffer2, 1);

		array[0] = 0x00013700;// read id return two byte,version and id
		dsi_set_cmdq(array, 1, 1);
        read_reg_v2(0x0E, buffer3, 1);
#endif
#endif
}*/

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
	MDELAY(10);
	read_reg_v2(0x0A, buffer, 1);
	 
	printk("mycat$$ truly ### esd check buffer[0] = 0x%x\r\n",buffer[0]);
	if(buffer[0] == 0x9C)
		return 0;
	else
		return 1;
 			
#endif
}

static unsigned int lcm_esd_recover(void)
{
  lcm_init();
	
#ifndef BUILD_LK	
	printk("mycat truly ### lcm_esd_recover \r\n");
#endif
	
	return 1;
}
LCM_DRIVER otm8018b_datong_DT50118A_IVO_dsi_2_vdo_fwvga_lcm_drv = 
{
    .name			= "otm8018b_datong_DT50118A_IVO_dsi_2_vdo_fwvga",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id    = lcm_compare_id,
    .esd_check      = lcm_esd_check,		
	.esd_recover = lcm_esd_recover,	
#if (LCM_DSI_CMD_MODE)
	.set_backlight	= lcm_setbacklight,
    .update         = lcm_update,
#endif
};

