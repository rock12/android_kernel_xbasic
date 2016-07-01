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
#define LCM_ID       (0x8000)
#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))
static unsigned int lcm_compare_id(void);

#define LCD_Write_COM(h,l)	send_ctrl_cmd(h<<8|l)
#define LCD_Write_DATA(h,l)  send_data_cmd(h<<8|l)

#define WriteCmd(h,l)	send_ctrl_cmd(h<<8|l)
#define WriteDat(h,l)  send_data_cmd(h<<8|l)

#define delayms	MDELAY

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

static __inline void send_ctrl_cmd(unsigned int cmd)
{
    lcm_util.send_cmd(cmd);
}

static __inline void send_data_cmd(unsigned int data)
{
    lcm_util.send_data(data&0xff);
}

static __inline unsigned short read_data_cmd(void)
{
    return (unsigned short)(lcm_util.read_data());
}

static __inline void set_lcm_register(unsigned int regIndex,
                                      unsigned int regData)
{
    send_ctrl_cmd(regIndex);
    send_data_cmd(regData);
}

static void sw_clear_panel(unsigned int color)
{
    short  x0, y0, x1, y1, x, y;
    short   h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;


    x0 = (short)0;
    y0 = (short)0;
    x1 = (short)FRAME_WIDTH-1;
    y1 = (short)FRAME_HEIGHT-1;

    h_X_start=((x0&0x0300)>>8);
    l_X_start=(x0&0x00FF);
    h_X_end=((x1&0x0300)>>8);
    l_X_end=(x1&0x00FF);

    h_Y_start=((y0&0x0300)>>8);
    l_Y_start=(y0&0x00FF);
    h_Y_end=((y1&0x0300)>>8);
    l_Y_end=(y1&0x00FF);

    send_ctrl_cmd( 0x2A00 );
    send_data_cmd( h_X_start);
    send_ctrl_cmd( 0x2A01 );
    send_data_cmd( l_X_start);
    send_ctrl_cmd( 0x2A02);
    send_data_cmd( h_X_end );
    send_ctrl_cmd( 0x2A03);
    send_data_cmd( l_X_end );
    send_ctrl_cmd( 0x2B00 );
    send_data_cmd( h_Y_start);
    send_ctrl_cmd( 0x2B01 );
    send_data_cmd( l_Y_start);
    send_ctrl_cmd( 0x2B02);
    send_data_cmd( h_Y_end );
    send_ctrl_cmd( 0x2B03);
    send_data_cmd( l_Y_end );
    //send_ctrl_cmd(0x3601);  //enable HSM mode
    //send_data_cmd(0x01);
    send_ctrl_cmd( 0x2C00 );

    // 18-bit mode (256K color) coding
    for (y = y0; y <= y1; ++ y) {
        for (x = x0; x <= x1; ++ x) {
            lcm_util.send_data(color);
        }
    }
}

#define LCDSPI_InitCMD send_ctrl_cmd
#define LCDSPI_InitDAT send_data_cmd

#define WriteCmd	send_ctrl_cmd
#define WriteDat   send_data_cmd
static int vcom=0x80;
static void init_lcm_registers(void)
{
#if defined(BUILD_UBOOT)
    printf("MYCAT LCD nt35510_hansen_HS40134WVNS39_00_HYDIS_mcu18_wvga");
#endif

     
WriteCmd(0xFF00);WriteDat(0xAA);
WriteCmd(0xFF01);WriteDat(0x55);
WriteCmd(0xFF02);WriteDat(0xA5);
WriteCmd(0xFF03);WriteDat(0x80);

WriteCmd(0xF200);WriteDat(0x00);
WriteCmd(0xF201);WriteDat(0x84);
WriteCmd(0xF202);WriteDat(0x08);

WriteCmd(0x6f00);WriteDat(0x0a);
WriteCmd(0xF400);WriteDat(0x13);

WriteCmd(0xFF00);WriteDat(0xAA);
WriteCmd(0xFF01);WriteDat(0x55);
WriteCmd(0xFF02);WriteDat(0xA5);
WriteCmd(0xFF03);WriteDat(0x00);


//#LV2 Page 1 enable
WriteCmd(0xF000);WriteDat(0x55);
WriteCmd(0xF001);WriteDat(0xAA);
WriteCmd(0xF002);WriteDat(0x52);
WriteCmd(0xF003);WriteDat(0x08);
WriteCmd(0xF004);WriteDat(0x01);

//#AVDD Set AVDD 5.2V
WriteCmd(0xB000);WriteDat(0x0A);  //0d

//#AVDD ratio
WriteCmd(0xB600);WriteDat(0x34);

//#AVEE ratio
WriteCmd(0xB700);WriteDat(0x24);

//#AVEE  -5.2V
WriteCmd(0xB100);WriteDat(0x0A);//0d

//#VCL ratio
WriteCmd(0xB800);WriteDat(0x35);

//#VCL  -2.5V
WriteCmd(0xB200);WriteDat(0x00);

//#VGH ratio
WriteCmd(0xB900);WriteDat(0x24);//34  sanyi
WriteCmd(0xB300);WriteDat(0x05);
//#VGH 15V  
WriteCmd(0xBF00);WriteDat(0x01);


//#VGLX ratio
WriteCmd(0xBA00);WriteDat(0x24);

WriteCmd(0xB500);WriteDat(0x0A);

//#VGMP/VGSP 4.7V/0V
WriteCmd(0xBC00);WriteDat(0x00);
WriteCmd(0xBC01);WriteDat(0x9C);
WriteCmd(0xBC02);WriteDat(0x00);

//#VGMN/VGSN -4.7V/0V
WriteCmd(0xBD00);WriteDat(0x00);
WriteCmd(0xBD01);WriteDat(0x9C);
WriteCmd(0xBD02);WriteDat(0x00);

//#VCOM 1.525V 
WriteCmd(0xBE00);WriteDat(0x00);
WriteCmd(0xBE01);WriteDat(0x50);//89
#if 0
WriteCmd(0xD000);WriteDat(0x0F);
WriteCmd(0xD001);WriteDat(0x0F);
WriteCmd(0xD002);WriteDat(0x10);
WriteCmd(0xD003);WriteDat(0x10);

WriteCmd(0xD700);WriteDat(0x01);

//#Gamma Setting
WriteCmd(0xD100);WriteDat(0x00);
WriteCmd(0xD101);WriteDat(0x05);
WriteCmd(0xD102);WriteDat(0x00);
WriteCmd(0xD103);WriteDat(0x31);
WriteCmd(0xD104);WriteDat(0x00);
WriteCmd(0xD105);WriteDat(0x60);
WriteCmd(0xD106);WriteDat(0x00);
WriteCmd(0xD107);WriteDat(0x7D);
WriteCmd(0xD108);WriteDat(0x00);
WriteCmd(0xD109);WriteDat(0x94);
WriteCmd(0xD10A);WriteDat(0x00);
WriteCmd(0xD10B);WriteDat(0xB7);
WriteCmd(0xD10C);WriteDat(0x00);
WriteCmd(0xD10D);WriteDat(0xD6);
WriteCmd(0xD10E);WriteDat(0x01);
WriteCmd(0xD10F);WriteDat(0x04);
WriteCmd(0xD110);WriteDat(0x01);
WriteCmd(0xD111);WriteDat(0x28);
WriteCmd(0xD112);WriteDat(0x01);
WriteCmd(0xD113);WriteDat(0x61);
WriteCmd(0xD114);WriteDat(0x01);
WriteCmd(0xD115);WriteDat(0x8D);
WriteCmd(0xD116);WriteDat(0x01);
WriteCmd(0xD117);WriteDat(0xD2);
WriteCmd(0xD118);WriteDat(0x02);
WriteCmd(0xD119);WriteDat(0x0B);
WriteCmd(0xD11A);WriteDat(0x02);
WriteCmd(0xD11B);WriteDat(0x0D);
WriteCmd(0xD11C);WriteDat(0x02);
WriteCmd(0xD11D);WriteDat(0x42);
WriteCmd(0xD11E);WriteDat(0x02);
WriteCmd(0xD11F);WriteDat(0x79);
WriteCmd(0xD120);WriteDat(0x02);
WriteCmd(0xD121);WriteDat(0x9A);
WriteCmd(0xD122);WriteDat(0x02);
WriteCmd(0xD123);WriteDat(0xC6);
WriteCmd(0xD124);WriteDat(0x02);
WriteCmd(0xD125);WriteDat(0xE2);
WriteCmd(0xD126);WriteDat(0x03);
WriteCmd(0xD127);WriteDat(0x0A);
WriteCmd(0xD128);WriteDat(0x03);
WriteCmd(0xD129);WriteDat(0x23);
WriteCmd(0xD12A);WriteDat(0x03);
WriteCmd(0xD12B);WriteDat(0x43);
WriteCmd(0xD12C);WriteDat(0x03);
WriteCmd(0xD12D);WriteDat(0x58);
WriteCmd(0xD12E);WriteDat(0x03);
WriteCmd(0xD12F);WriteDat(0x6E);
WriteCmd(0xD130);WriteDat(0x03);
WriteCmd(0xD131);WriteDat(0x87);
WriteCmd(0xD132);WriteDat(0x03);
WriteCmd(0xD133);WriteDat(0xFF);
#endif
//#LV2 Page 0 enable
WriteCmd(0xF000);WriteDat(0x55);
WriteCmd(0xF001);WriteDat(0xAA);
WriteCmd(0xF002);WriteDat(0x52);
WriteCmd(0xF003);WriteDat(0x08);
WriteCmd(0xF004);WriteDat(0x00);

//#Display control
WriteCmd(0xB100);WriteDat(0xFC);
WriteCmd(0xB101);WriteDat(0x00);

//#480x854
WriteCmd(0xB500);WriteDat(0x6B);

//#Source hold time
WriteCmd(0xB600);WriteDat(0x05);

//#Gate EQ control
WriteCmd(0xB700);WriteDat(0x00);
WriteCmd(0xB701);WriteDat(0x00);

//#Source EQ control (Mode 2)
WriteCmd(0xB800);WriteDat(0x01);
WriteCmd(0xB801);WriteDat(0x05);
WriteCmd(0xB802);WriteDat(0x05);
WriteCmd(0xB803);WriteDat(0x05);

//#Inversion mode  (Column)
WriteCmd(0xBC00);WriteDat(0x02);//00
WriteCmd(0xBC01);WriteDat(0x00);
WriteCmd(0xBC02);WriteDat(0x00);


WriteCmd(0xCC00);WriteDat(0x03);//00
WriteCmd(0xCC01);WriteDat(0x00);
WriteCmd(0xCC02);WriteDat(0x00);

#if 0
//#Timing control 8phase dual side/4H/4delay/RST_EN
WriteCmd(0xC900);WriteDat(0xD0);
WriteCmd(0xC901);WriteDat(0x82);
WriteCmd(0xC902);WriteDat(0x50);
WriteCmd(0xC903);WriteDat(0x28);
WriteCmd(0xC904);WriteDat(0x28);
#endif

//#Timing control - Frame Rate
WriteCmd(0xBD00);WriteDat(0x01);
WriteCmd(0xBD01);WriteDat(0x84);
WriteCmd(0xBD02);WriteDat(0x07);
WriteCmd(0xBD03);WriteDat(0x31);
WriteCmd(0xBD04);WriteDat(0x00);


WriteCmd(0x3a00);WriteDat(0x66);
WriteCmd(0x3500);WriteDat(0x00);
WriteCmd(0x3600);WriteDat(0x00);
//WriteCmd(0x4400); WriteDat(0x00);//TE Start line
//WriteCmd(0x4401); WriteDat(0x32);//FF Rotate 180

WriteCmd(0xF000);WriteDat(0x55);
WriteCmd(0xF001);WriteDat(0xAA);
WriteCmd(0xF002);WriteDat(0x52);
WriteCmd(0xF003);WriteDat(0x08);
WriteCmd(0xF004);WriteDat(0x01);

WriteCmd(0xD000);WriteDat(0x0F);
WriteCmd(0xD001);WriteDat(0x0F);
WriteCmd(0xD002);WriteDat(0x10);
WriteCmd(0xD003);WriteDat(0x10);

WriteCmd(0xD700);WriteDat(0x01);

//#Gamma Setting
WriteCmd(0xD100);WriteDat(0x00);
WriteCmd(0xD101);WriteDat(0x6D);
WriteCmd(0xD102);WriteDat(0x00);
WriteCmd(0xD103);WriteDat(0x74);
WriteCmd(0xD104);WriteDat(0x00);
WriteCmd(0xD105);WriteDat(0x83);
WriteCmd(0xD106);WriteDat(0x00);
WriteCmd(0xD107);WriteDat(0x92);
WriteCmd(0xD108);WriteDat(0x00);
WriteCmd(0xD109);WriteDat(0x9D);
WriteCmd(0xD10A);WriteDat(0x00);
WriteCmd(0xD10B);WriteDat(0xB5);
WriteCmd(0xD10C);WriteDat(0x00);
WriteCmd(0xD10D);WriteDat(0xCD);
WriteCmd(0xD10E);WriteDat(0x00);
WriteCmd(0xD10F);WriteDat(0xF5);
WriteCmd(0xD110);WriteDat(0x01);
WriteCmd(0xD111);WriteDat(0x18);
WriteCmd(0xD112);WriteDat(0x01);
WriteCmd(0xD113);WriteDat(0x4E);
WriteCmd(0xD114);WriteDat(0x01);
WriteCmd(0xD115);WriteDat(0x7D);
WriteCmd(0xD116);WriteDat(0x01);
WriteCmd(0xD117);WriteDat(0xCC);
WriteCmd(0xD118);WriteDat(0x02);
WriteCmd(0xD119);WriteDat(0x0B);
WriteCmd(0xD11A);WriteDat(0x02);
WriteCmd(0xD11B);WriteDat(0x0C);
WriteCmd(0xD11C);WriteDat(0x02);
WriteCmd(0xD11D);WriteDat(0x39);
WriteCmd(0xD11E);WriteDat(0x02);
WriteCmd(0xD11F);WriteDat(0x7E);
WriteCmd(0xD120);WriteDat(0x02);
WriteCmd(0xD121);WriteDat(0xAB);
WriteCmd(0xD122);WriteDat(0x02);
WriteCmd(0xD123);WriteDat(0xE2);
WriteCmd(0xD124);WriteDat(0x03);
WriteCmd(0xD125);WriteDat(0x05);
WriteCmd(0xD126);WriteDat(0x03);
WriteCmd(0xD127);WriteDat(0x35);
WriteCmd(0xD128);WriteDat(0x03);
WriteCmd(0xD129);WriteDat(0x54);
WriteCmd(0xD12A);WriteDat(0x03);
WriteCmd(0xD12B);WriteDat(0x7C);
WriteCmd(0xD12C);WriteDat(0x03);
WriteCmd(0xD12D);WriteDat(0x91);
WriteCmd(0xD12E);WriteDat(0x03);
WriteCmd(0xD12F);WriteDat(0xAE);
WriteCmd(0xD130);WriteDat(0x03);
WriteCmd(0xD131);WriteDat(0xDB);
WriteCmd(0xD132);WriteDat(0x03);
WriteCmd(0xD133);WriteDat(0xEC);


WriteCmd(0x1100);
MDELAY(120);
WriteCmd(0x2900);
 

//    MDELAY(200);
  //  sw_clear_panel(0x0);    // Clean panel as black
  //  MDELAY(10);
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

    params->dbi.port                    = 0;  //DBI port must be 0 or 1 on mt6575, should not be 2
    params->dbi.data_width              = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dbi.data_format.trans_seq   = LCM_DBI_TRANS_SEQ_MSB_FIRST;
    params->dbi.data_format.padding     = LCM_DBI_PADDING_ON_LSB;
    params->dbi.data_format.format      = LCM_DBI_FORMAT_RGB666;
    params->dbi.data_format.width       = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.cpu_write_bits          = LCM_DBI_CPU_WRITE_32_BITS;
    params->dbi.io_driving_current      = LCM_DRIVING_CURRENT_6575_8MA;

    params->dbi.parallel.write_setup    = 1;
    params->dbi.parallel.write_hold     = 1;
    params->dbi.parallel.write_wait     = 4;
    params->dbi.parallel.read_setup     = 5;
    params->dbi.parallel.read_hold      = 0;
    params->dbi.parallel.read_latency   = 18;
    params->dbi.parallel.wait_period    = 4;
    params->dbi.parallel.cs_high_width  = 0; //cycles of cs high level between each transfer
    // enable tearing-free
    params->dbi.te_mode                 = 0;
    params->dbi.te_mode                 = LCM_DBI_TE_MODE_VSYNC_ONLY;
    params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;
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
    SET_RESET_PIN(0);
    MDELAY(100);
    SET_RESET_PIN(1);
    MDELAY(120);
    init_lcm_registers();

}


static void lcm_suspend(void)
{
    send_ctrl_cmd(0x2800);
    send_ctrl_cmd(0x1000);
    MDELAY(20);
}


static void lcm_resume(void)
{
//vcom+=2;
#if 0
    send_ctrl_cmd(0x1100);
    MDELAY(200);
    send_ctrl_cmd(0x2900);
#else
	lcm_init();
 // Lcd_Log("wangdong 0x%02x\n",vcom);

#endif
}

static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
    short  x0, y0, x1, y1;
    short   h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;


    x0 = (short)x;
    y0 = (short)y;
    x1 = (short)x+width-1;
    y1 = (short)y+height-1;

    h_X_start=((x0&0x0300)>>8);
    l_X_start=(x0&0x00FF);
    h_X_end=((x1&0x0300)>>8);
    l_X_end=(x1&0x00FF);

    h_Y_start=((y0&0x0300)>>8);
    l_Y_start=(y0&0x00FF);
    h_Y_end=((y1&0x0300)>>8);
    l_Y_end=(y1&0x00FF);

    send_ctrl_cmd( 0x2A00 );
    send_data_cmd( h_X_start);
    send_ctrl_cmd( 0x2A01 );
    send_data_cmd( l_X_start);
    send_ctrl_cmd( 0x2A02);
    send_data_cmd( h_X_end );
    send_ctrl_cmd( 0x2A03);
    send_data_cmd( l_X_end );
    send_ctrl_cmd( 0x2B00 );
    send_data_cmd( h_Y_start);
    send_ctrl_cmd( 0x2B01 );
    send_data_cmd( l_Y_start);
    send_ctrl_cmd( 0x2B02);
    send_data_cmd( h_Y_end );
    send_ctrl_cmd( 0x2B03);
    send_data_cmd( l_Y_end );
    //send_ctrl_cmd(0x3601);  //enable HSM mode
    //send_data_cmd(0x01);
    send_ctrl_cmd( 0x2C00 );
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
    unsigned int id = 0;

#ifdef BUILD_LK
			pmic_config_interface(0x0532,5,0x7,5);//add by libo for VGP2 POWER ON
			pmic_config_interface(0x050C,1,0x1,15);
#else
			hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
#endif
			MDELAY(100);

    SET_RESET_PIN(1);
    MDELAY(20);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(100);
    send_ctrl_cmd(0xDA00);
    id = read_data_cmd();
		Lcd_Log("MYCAT id1 = 0x%08x\n", id);
    send_ctrl_cmd(0xDB00);
		id = read_data_cmd();
		Lcd_Log("MYCAT id2 = 0x%08x\n", id);
    id= (id<<8);
    send_ctrl_cmd(0xDC00);
    id |= read_data_cmd();


			Lcd_Log("MYCAT id3 = 0x%08x\n", id);



    return (LCM_ID == id&&get_lcd_id()==1)?1:0;
}

LCM_DRIVER nt35510_HONGSHENG_HS4501310A_BOE_mcu18_fwvga_lcm_drv =
    {
        .name			= "nt35510_HONGSHENG_HS4501310A_BOE_mcu18_fwvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id     = lcm_compare_id
    };
