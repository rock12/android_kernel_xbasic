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
#define LCM_ID       (0x84)
#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN

#define   __LCM_TE_ON__
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

static __inline void send_ctrl_cmd(unsigned int cmd)
{
	lcm_util.send_cmd(cmd);
}

static __inline void send_data_cmd(unsigned int data)
{
	lcm_util.send_data(data);
}

static __inline unsigned int read_data_cmd(void)
{
    return lcm_util.read_data();
}

static __inline void set_lcm_register(unsigned int regIndex,
		unsigned int regData)
{
	send_ctrl_cmd(regIndex);
	send_data_cmd(regData);
}


static void sw_clear_panel(unsigned int color)
{
	unsigned short x0, y0, x1, y1, x, y;
	unsigned short h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;

	x0 = (unsigned short)0;
	y0 = (unsigned short)0;
	x1 = (unsigned short)FRAME_WIDTH-1;
	y1 = (unsigned short)FRAME_HEIGHT-1;

	h_X_start=((x0&0xFF00)>>8);
	l_X_start=(x0&0x00FF);
	h_X_end=((x1&0xFF00)>>8);
	l_X_end=(x1&0x00FF);

	h_Y_start=((y0&0xFF00)>>8);
	l_Y_start=(y0&0x00FF);
	h_Y_end=((y1&0xFF00)>>8);
	l_Y_end=(y1&0x00FF);

  send_ctrl_cmd(0x2A);
	send_data_cmd(h_X_start); 	
	send_data_cmd(l_X_start); 	
	send_data_cmd(h_X_end); 	
	send_data_cmd(l_X_end); 

	send_ctrl_cmd(0x2B);
	send_data_cmd(h_Y_start); 
	send_data_cmd(l_Y_start); 
	send_data_cmd(h_Y_end); 
	send_data_cmd(l_Y_end); 

//color=0x8410 ;
// color=0xF800 ;
//color=0xFFFF ;
	send_ctrl_cmd(0x29); 

	send_ctrl_cmd(0x2C);
    for (y = y0; y <= y1; ++ y) {
        for (x = x0; x <= x1; ++ x) {
            lcm_util.send_data(color);
        }
    }
}

static unsigned int lcm_compare_id(void)
{
    unsigned int id = 0;
	SET_RESET_PIN(1);
	MDELAY(20);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(100);
	
	
 send_ctrl_cmd(0xB9); //SETEXTC: enable extention command (B9h)
	send_data_cmd(0xFF);  
	send_data_cmd(0x83);  
	send_data_cmd(0x63); 
	
	
	send_ctrl_cmd(0xF4);

	read_data_cmd();
  id = read_data_cmd();

	return (LCM_ID == id)?1:0;
	
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

static void init_lcm_registers(void)
{

#if defined(BUILD_UBOOT)
	printf("mycat LCD huarui lcd +++000 \n");	
#endif
#if 0
	send_ctrl_cmd(0xB9); //SETEXTC: enable extention command (B9h)
	send_data_cmd(0xFF);  
	send_data_cmd(0x83);  
	send_data_cmd(0x63); 
 
 
   	send_ctrl_cmd(0xBF);  // SET PTBA for VCOM=-2.5V 
   	send_data_cmd(0x05);   
   	send_data_cmd(0x60);   
   send_data_cmd(0x00);   
   	send_data_cmd(0x10);   
 
	send_ctrl_cmd(0xB1);//SETPOWER: Set power (B1h)
	send_data_cmd(0x01);//
	send_data_cmd(0x00);//
	send_data_cmd(0x11);//44
	send_data_cmd(0x07);//	 
	send_data_cmd(0x00);//01	 
	send_data_cmd(0x11);//	
	send_data_cmd(0x11);//	
	send_data_cmd(0x3A);//  
	send_data_cmd(0x42);// 
	send_data_cmd(0x3F);//
	send_data_cmd(0x3F);//
	send_data_cmd(0x40);// 
	send_data_cmd(0x02);// 32
	send_data_cmd(0x00);// 
	send_data_cmd(0xE6);// 
	send_data_cmd(0xE6);// 
	send_data_cmd(0xE6);// 
	send_data_cmd(0xE6);// 
	send_data_cmd(0xE6);// 
	
	send_ctrl_cmd(0xB2);//SETDISP: Set display related register (B2h)
	send_data_cmd(0x08);//  
	send_data_cmd(0x00);//  
	
	send_ctrl_cmd(0xB4);//SETCYC: Set display waveform cycle (B4h)
	send_data_cmd(0x09);//NW_PE[2:0] NW[2:0]  1-dot inversion  
	send_data_cmd(0x18);//  
	send_data_cmd(0x9C);//
	send_data_cmd(0x08);//
	send_data_cmd(0x18);//
	send_data_cmd(0x04);// 
	send_data_cmd(0x72);// 
	
	send_ctrl_cmd(0xB6); //SETVCOM: Set VCOM voltage (B6h)
	send_data_cmd(0x2B);  //Optimal VCOM Tuning
	
	send_ctrl_cmd(0xCC);//SETPANEL (CCh)
	send_data_cmd(0x0B);//

	send_ctrl_cmd(0xE0);  // SET Gamma
	//Positive of GAMMA
  send_data_cmd(0x0D);//1
	send_data_cmd(0x1E);//2
	send_data_cmd(0x25);//3
	send_data_cmd(0x16);//4
	send_data_cmd(0x15);//5
	send_data_cmd(0x38);//6

	send_data_cmd(0x34);//7
	send_data_cmd(0x3C);//8

	send_data_cmd(0x04);//9
	send_data_cmd(0x0B);//10
	send_data_cmd(0x12);//11
	send_data_cmd(0x17);//12
	send_data_cmd(0x18);//13
	send_data_cmd(0x16);//14
	send_data_cmd(0x17);//15
	send_data_cmd(0x16);//16
	send_data_cmd(0x19);//17

	//Negtive of GAMMA
  send_data_cmd(0x0D);//1
	send_data_cmd(0x1E);//2
	send_data_cmd(0x25);//3
	send_data_cmd(0x16);//4
	send_data_cmd(0x15);//5
	send_data_cmd(0x38);//6

	send_data_cmd(0x34);//7
	send_data_cmd(0x3C);//8

	send_data_cmd(0x04);//9
	send_data_cmd(0x0B);//10
	send_data_cmd(0x12);//11
	send_data_cmd(0x17);//12
	send_data_cmd(0x18);//13
	send_data_cmd(0x16);//14
	send_data_cmd(0x17);//15
	send_data_cmd(0x16);//16
	send_data_cmd(0x19);//17

	send_ctrl_cmd(0xC2); // SETDSPIF: Set display interface mode (C2h) 
	send_data_cmd(0x03); //0x04:24-bit, 0x03:18-bit,0x02: 16-bit ,0x01: 9-bit ,0x00: 8-bit 

 	send_ctrl_cmd(0x3A);//  Interface Pixel Format
	send_data_cmd(0x66); // 0x77:24-bit, 0x66:18-bit,0x55: 16-bit 
 
  send_ctrl_cmd(0x35);//
	send_ctrl_cmd(0x13);//
	send_ctrl_cmd(0x38);//
	send_ctrl_cmd(0x11); //
	MDELAY(120);
  send_ctrl_cmd(0x29);//
#else
////////////////////////////////////////////////////////////////////////

  	send_ctrl_cmd(0xB9);  // SET password		// B9h(Set OTP Related Setting) is the command in HX8369A
		send_data_cmd(0xFF);
		send_data_cmd(0x83);
		send_data_cmd(0x63);
		MDELAY(1);	

	   send_ctrl_cmd(0x11); //Display On
		MDELAY(300);

		send_ctrl_cmd(0xB1); //SET Power
		send_data_cmd(0x78);
		send_data_cmd(0x00);
		send_data_cmd(0x11);//44
		send_data_cmd(0x07);
		send_data_cmd(0x00);//01
		send_data_cmd(0x0E);
		send_data_cmd(0x0E);
		send_data_cmd(0x21);
		send_data_cmd(0x29);
		send_data_cmd(0x3F);
		send_data_cmd(0x3F);
		send_data_cmd(0x40);
		send_data_cmd(0x02);//32
		send_data_cmd(0x00);
		send_data_cmd(0xE6);
		send_data_cmd(0xE6);
		send_data_cmd(0xE6);
		send_data_cmd(0xE6);
		send_data_cmd(0xE6);
		MDELAY(10);
		send_ctrl_cmd(0xB2); // SET DISP
		send_data_cmd(0x08);
		send_data_cmd(0x00);
		MDELAY(1);

		send_ctrl_cmd(0xB4); // SET CYC
		send_data_cmd(0x02);
		send_data_cmd(0x18);
		send_data_cmd(0x9C);
		send_data_cmd(0x08);
		send_data_cmd(0x18);
		send_data_cmd(0x04);
		send_data_cmd(0x72);
		MDELAY(1);

		send_ctrl_cmd(0xBF); // SET VCOM for Max=-2.5V
		send_data_cmd(0x05);
		send_data_cmd(0x60);
		send_data_cmd(0x00);
		send_data_cmd(0x10);
		MDELAY(1);

		send_ctrl_cmd(0xB6); // SET VCOM
		send_data_cmd(0x42);//4e
		MDELAY(1);

		send_ctrl_cmd(0xCC); // SET Panel
		send_data_cmd(0x03);
		MDELAY(1);

		send_ctrl_cmd(0xE0); // SET Gamma
		send_data_cmd(0x00);
send_data_cmd(0x02);
send_data_cmd(0x02);
send_data_cmd(0x16);
send_data_cmd(0x18);
send_data_cmd(0x2F);
send_data_cmd(0x70);
send_data_cmd(0x20);
send_data_cmd(0x89);
send_data_cmd(0x8E);
send_data_cmd(0xCF);
send_data_cmd(0xD7);
send_data_cmd(0xDD);
send_data_cmd(0x15);
send_data_cmd(0x0A);
send_data_cmd(0x0F);
send_data_cmd(0x1F);
send_data_cmd(0x38);
send_data_cmd(0x2F);
send_data_cmd(0x2D);
send_data_cmd(0x16);
send_data_cmd(0x16);
send_data_cmd(0x2F);
send_data_cmd(0x0F);
send_data_cmd(0x47);
send_data_cmd(0x9F);
send_data_cmd(0x9F);
send_data_cmd(0xD3);
send_data_cmd(0xD4);
send_data_cmd(0xCF);
send_data_cmd(0x11);
send_data_cmd(0x19);
send_data_cmd(0x03);
send_data_cmd(0x0C);
		MDELAY(1);

send_ctrl_cmd(0xC1); // SET DGC
send_data_cmd(0x01);
//R
send_data_cmd(0x07);
send_data_cmd(0x09);
send_data_cmd(0x10);
send_data_cmd(0x19);
send_data_cmd(0x22);
send_data_cmd(0x2B);
send_data_cmd(0x32);
send_data_cmd(0x3B);
send_data_cmd(0x43);
send_data_cmd(0x4B);
send_data_cmd(0x53);
send_data_cmd(0x5B);
send_data_cmd(0x63);
send_data_cmd(0x6C);
send_data_cmd(0x76);
send_data_cmd(0x7F);
send_data_cmd(0x88);
send_data_cmd(0x90);
send_data_cmd(0x97);
send_data_cmd(0x9D);
send_data_cmd(0xA3);
send_data_cmd(0xA9);
send_data_cmd(0xAF);
send_data_cmd(0xB5);
send_data_cmd(0xBB);
send_data_cmd(0xC1);
send_data_cmd(0xC8);
send_data_cmd(0xD1);
send_data_cmd(0xDD);
send_data_cmd(0xE6);
send_data_cmd(0xED);
send_data_cmd(0xF8);
send_data_cmd(0xFF);
send_data_cmd(0xEF);
send_data_cmd(0xF3);
send_data_cmd(0xA3);
send_data_cmd(0x49);
send_data_cmd(0xCB);
send_data_cmd(0x56);
send_data_cmd(0xD0);
send_data_cmd(0x87);
send_data_cmd(0xC0);

//G
send_data_cmd(0x07);
send_data_cmd(0x09);
send_data_cmd(0x10);
send_data_cmd(0x19);
send_data_cmd(0x22);
send_data_cmd(0x2B);
send_data_cmd(0x32);
send_data_cmd(0x3B);
send_data_cmd(0x43);
send_data_cmd(0x4B);
send_data_cmd(0x53);
send_data_cmd(0x5B);
send_data_cmd(0x63);
send_data_cmd(0x6C);
send_data_cmd(0x76);
send_data_cmd(0x7F);
send_data_cmd(0x88);
send_data_cmd(0x90);
send_data_cmd(0x97);
send_data_cmd(0x9D);
send_data_cmd(0xA3);
send_data_cmd(0xA9);
send_data_cmd(0xAF);
send_data_cmd(0xB5);
send_data_cmd(0xBB);
send_data_cmd(0xC1);
send_data_cmd(0xC8);
send_data_cmd(0xD1);
send_data_cmd(0xDD);
send_data_cmd(0xE6);
send_data_cmd(0xED);
send_data_cmd(0xF8);
send_data_cmd(0xFF);
send_data_cmd(0xEF);
send_data_cmd(0xF3);
send_data_cmd(0xA3);
send_data_cmd(0x49);
send_data_cmd(0xCB);
send_data_cmd(0x56);
send_data_cmd(0xD0);
send_data_cmd(0x87);
send_data_cmd(0xC0);
//B
send_data_cmd(0x07);
send_data_cmd(0x09);
send_data_cmd(0x10);
send_data_cmd(0x19);
send_data_cmd(0x22);
send_data_cmd(0x2B);
send_data_cmd(0x32);
send_data_cmd(0x3B);
send_data_cmd(0x43);
send_data_cmd(0x4B);
send_data_cmd(0x53);
send_data_cmd(0x5B);
send_data_cmd(0x63);
send_data_cmd(0x6C);
send_data_cmd(0x76);
send_data_cmd(0x7F);
send_data_cmd(0x88);
send_data_cmd(0x90);
send_data_cmd(0x97);
send_data_cmd(0x9D);
send_data_cmd(0xA3);
send_data_cmd(0xA9);
send_data_cmd(0xAF);
send_data_cmd(0xB5);
send_data_cmd(0xBB);
send_data_cmd(0xC1);
send_data_cmd(0xC8);
send_data_cmd(0xD1);
send_data_cmd(0xDD);
send_data_cmd(0xE6);
send_data_cmd(0xED);
send_data_cmd(0xF8);
send_data_cmd(0xFF);
send_data_cmd(0xEF);
send_data_cmd(0xF3);
send_data_cmd(0xA3);
send_data_cmd(0x49);
send_data_cmd(0xCB);
send_data_cmd(0x56);
send_data_cmd(0xD0);
send_data_cmd(0x87);
send_data_cmd(0xC0);
		MDELAY(1);

		send_ctrl_cmd(0xC2); // SET DISPIF
		send_data_cmd(0x03); //04 24bit
		MDELAY(1);

		send_ctrl_cmd(0xBA); //Set MIPI Control
		send_data_cmd(0x11);//0x11 = 2 data lane
		send_data_cmd(0x00);
		send_data_cmd(0x56);
		send_data_cmd(0xC6);
		send_data_cmd(0x10);
		send_data_cmd(0x89);
		send_data_cmd(0xFF);
		send_data_cmd(0x0F);
		send_data_cmd(0x33);
		send_data_cmd(0x6E);
		send_data_cmd(0x04);
		send_data_cmd(0x07);
		send_data_cmd(0x9A);
		send_data_cmd(0x10);
		MDELAY(1);
		send_ctrl_cmd(0x36); //Memory Access
        send_data_cmd(0x44);//00
        	send_ctrl_cmd(0x35); //Memory Access
        send_data_cmd(0x00);

		send_ctrl_cmd(0x3A); // SET DISPIF
		send_data_cmd(0x66); //24bit 55 16BIT

		send_ctrl_cmd(0x29); //Display On
		MDELAY(20);
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

    params->type   = LCM_TYPE_DBI;
    params->ctrl   = LCM_CTRL_PARALLEL_DBI;
    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;
    params->io_select_mode = 1; //note:this para is different between 6573 and 6575
    /* serial host interface configurations */
    
    params->dbi.port                    = 0;
    params->dbi.data_width              = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dbi.data_format.trans_seq   = LCM_DBI_TRANS_SEQ_MSB_FIRST;
    params->dbi.data_format.padding     = LCM_DBI_PADDING_ON_LSB;
    params->dbi.data_format.format      = LCM_DBI_FORMAT_RGB666;
    params->dbi.data_format.width       = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.cpu_write_bits          = LCM_DBI_CPU_WRITE_16_BITS;
    params->dbi.io_driving_current      = LCM_DRIVING_CURRENT_6575_8MA;

    params->dbi.parallel.write_setup    = 0;
    params->dbi.parallel.write_hold     = 0;
    params->dbi.parallel.write_wait     = 2;
    params->dbi.parallel.read_setup     = 5;
    params->dbi.parallel.read_hold      = 1;
    params->dbi.parallel.read_latency   = 18;
    params->dbi.parallel.wait_period    = 2;
    params->dbi.parallel.cs_high_width  = 0; //cycles of cs high level between each transfer
    
#ifdef  __LCM_TE_ON__
	// enable tearing-free
    params->dbi.te_mode                 = LCM_DBI_TE_MODE_VSYNC_ONLY;
    params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;	
 #else 
    params->dbi.te_mode                 = LCM_DBI_TE_MODE_DISABLED;  
#endif
}


static void lcm_init(void)
{

#ifdef BUILD_LK
	pmic_config_interface(0x0532,5,0x7,5);//add by libo for VGP2 POWER ON
	pmic_config_interface(0x050C,1,0x1,15);
#else
	hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
#endif
//lcm_compare_id();

    MDELAY(100);

	SET_RESET_PIN(1);
	MDELAY(20);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(100);

	init_lcm_registers();
	
	sw_clear_panel(0x00);
	MDELAY(10);
}


static void lcm_suspend(void)
{
	send_ctrl_cmd(0x28); // Display off
	MDELAY(10);
	send_ctrl_cmd(0x10); // Sleep in
	MDELAY(120);
}


static void lcm_resume(void)
{

	//add by xia lei jie for LCD INIT
	//lcm_init();
	
	send_ctrl_cmd(0x11);
	MDELAY(120);
	send_ctrl_cmd(0x29);
	MDELAY(50);

}






static void lcm_update(unsigned int x, unsigned int y,
		unsigned int width, unsigned int height)
{
	unsigned short x0, y0, x1, y1;
	unsigned short h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;

	x0 = (unsigned short)x;
	y0 = (unsigned short)y;
	x1 = (unsigned short)x+width-1;
	y1 = (unsigned short)y+height-1;

	h_X_start=((x0&0xFF00)>>8);
	l_X_start=(x0&0x00FF);
	h_X_end=((x1&0xFF00)>>8);
	l_X_end=(x1&0x00FF);

	h_Y_start=((y0&0xFF00)>>8);
	l_Y_start=(y0&0x00FF);
	h_Y_end=((y1&0xFF00)>>8);
	l_Y_end=(y1&0x00FF);

  send_ctrl_cmd(0x2A);
	send_data_cmd(h_X_start); 	
	send_data_cmd(l_X_start); 
	send_data_cmd(h_X_end); 
	send_data_cmd(l_X_end); 

	send_ctrl_cmd(0x2B);
	send_data_cmd(h_Y_start); 
	send_data_cmd(l_Y_start); 
	send_data_cmd(h_Y_end); 
	send_data_cmd(l_Y_end); 

	send_ctrl_cmd(0x29); 

	send_ctrl_cmd(0x2C);
}


// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER hx8363_songrui_ST040MOMS59_mcu18_wvga_lcm_drv = 
{
  .name			= "hx8363_songrui_ST040MOMS59_mcu18_wvga",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.update         = lcm_update,
	.compare_id    = lcm_compare_id,
};
