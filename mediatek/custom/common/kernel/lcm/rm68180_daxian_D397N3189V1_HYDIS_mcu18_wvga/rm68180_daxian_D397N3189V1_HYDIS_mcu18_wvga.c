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
#define LCM_ID       (0x6818)
#define GPIO_LCD_ID_PIN 50

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))
static unsigned int lcm_compare_id(void);
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

static int get_lcd_id(void)
{
    mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
    mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,0);
    MDELAY(1);

    return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}

static void init_lcm_registers(void)
{
#if defined(BUILD_UBOOT)
    printf("MYCAT LCD rm68180_daxian_D397N3189V1_HYDIS_mcu18_wvga\n");
#endif
    if(get_lcd_id())// HY daxian
    {
			// VCI=2.8V
			//************* Reset LCD Driver ****************//
			//LCD_nRESET = 1;
			//MDELAY(1); 	         // Delay 1ms
			//LCD_nRESET = 0;
			//MDELAY(10); 	         // Delay 10ms, This delay time is necessary
			//LCD_nRESET = 1;
			//MDELAY(120); 	         // Delay 120 ms at least
			//************* Reset LCD Driver ****************//
			
			//************* Start Initial Sequence **********//
			 /** Select Command Page '1' **/
			send_ctrl_cmd(0xF000);  send_data_cmd(0x55);
			send_ctrl_cmd(0xF001);  send_data_cmd(0xAA);
			send_ctrl_cmd(0xF002);  send_data_cmd(0x52);
			send_ctrl_cmd(0xF003);  send_data_cmd(0x08);
			send_ctrl_cmd(0xF004);  send_data_cmd(0x01);
			
			// Setting AVDD Voltage
			send_ctrl_cmd(0xB000);  send_data_cmd(0x05);   //AVDD=6V
			send_ctrl_cmd(0xB001);  send_data_cmd(0x05);
			send_ctrl_cmd(0xB002);  send_data_cmd(0x05);
			
			// Setting AVEE Voltage
			send_ctrl_cmd(0xB100);  send_data_cmd(0x05);  //AVEE=-6V
			send_ctrl_cmd(0xB101);  send_data_cmd(0x05);
			send_ctrl_cmd(0xB102);  send_data_cmd(0x05);
			
			// Setting VCL Voltage
			send_ctrl_cmd(0xB200);  send_data_cmd(0x03);  //VCL=-4V
			send_ctrl_cmd(0xB201);  send_data_cmd(0x03);
			send_ctrl_cmd(0xB202);  send_data_cmd(0x03);
			
			// Setting VGH Voltag
			send_ctrl_cmd(0xB300);  send_data_cmd(0x0A);  //17V
			send_ctrl_cmd(0xB301);  send_data_cmd(0x0A);
			send_ctrl_cmd(0xB302);  send_data_cmd(0x0A);
			
			// Setting VRGH Voltag
			send_ctrl_cmd(0xB400);  send_data_cmd(0x2D);  //5.5V
			send_ctrl_cmd(0xB401);  send_data_cmd(0x2D);
			send_ctrl_cmd(0xB402);  send_data_cmd(0x2D);
			
			// Setting VGL_REG Voltag
			send_ctrl_cmd(0xB500);  send_data_cmd(0x08);  //-10V
			send_ctrl_cmd(0xB501);  send_data_cmd(0x08);
			send_ctrl_cmd(0xB502);  send_data_cmd(0x08);
			
			// AVDD Ratio setting
			send_ctrl_cmd(0xB600);  send_data_cmd(0x34);  //x 2.0
			send_ctrl_cmd(0xB601);  send_data_cmd(0x34);  
			send_ctrl_cmd(0xB602);  send_data_cmd(0x34);
			
			// AVEE Ratio setting
			send_ctrl_cmd(0xB700);  send_data_cmd(0x24);  //x 2.0
			send_ctrl_cmd(0xB701);  send_data_cmd(0x24);
			send_ctrl_cmd(0xB702);  send_data_cmd(0x24);
			
			// Power Control for VCL
			send_ctrl_cmd(0xB800);  send_data_cmd(0x24);  //VCL=-1 x VCI
			send_ctrl_cmd(0xB801);  send_data_cmd(0x24);
			send_ctrl_cmd(0xB802);  send_data_cmd(0x24);
			
			// Setting VGH Ratio
			send_ctrl_cmd(0xB900);  send_data_cmd(0x34);
			send_ctrl_cmd(0xB901);  send_data_cmd(0x34);
			send_ctrl_cmd(0xB902);  send_data_cmd(0x34);
			
			
			// VGLX Ratio
			send_ctrl_cmd(0xBA00);  send_data_cmd(0x14);
			send_ctrl_cmd(0xBA01);  send_data_cmd(0x14);
			send_ctrl_cmd(0xBA02);  send_data_cmd(0x14);
			
			// Setting VGMP and VGSP Voltag
			send_ctrl_cmd(0xBC00);  send_data_cmd(0x00);
			send_ctrl_cmd(0xBC01);  send_data_cmd(0x68);  //VGMP=5.3V
			send_ctrl_cmd(0xBC02);  send_data_cmd(0x00);  
			
			// Setting VGMN and VGSN Voltage
			send_ctrl_cmd(0xBD00);  send_data_cmd(0x00);
			send_ctrl_cmd(0xBD01);  send_data_cmd(0x68);  //VGMN=-5.3V
			send_ctrl_cmd(0xBD02);  send_data_cmd(0x00);  
			
			// Setting VCOM Offset Voltage
			send_ctrl_cmd(0xBE00);  send_data_cmd(0x00);  
			send_ctrl_cmd(0xBE01);  send_data_cmd(0x0C); //0E VCOM=-1.38V
			
			// Control VGH booster voltage rang
			send_ctrl_cmd(0xBF00);  send_data_cmd(0x01);
			
			//Page 1 setting for Gamma setting
			send_ctrl_cmd(0xF000);  send_data_cmd(0x55);
			send_ctrl_cmd(0xF001);  send_data_cmd(0xAA);
			send_ctrl_cmd(0xF002);  send_data_cmd(0x52);
			send_ctrl_cmd(0xF003);  send_data_cmd(0x08);
			send_ctrl_cmd(0xF004);  send_data_cmd(0x01);
			
			
			//Gamma Setting
			send_ctrl_cmd(0xD100);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD101);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD102);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD103);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD104);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD105);  send_data_cmd(0x25);
			send_ctrl_cmd(0xD106);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD107);  send_data_cmd(0x3B);
			send_ctrl_cmd(0xD108);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD109);  send_data_cmd(0x53);
			send_ctrl_cmd(0xD10A);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD10B);  send_data_cmd(0x7B);
			send_ctrl_cmd(0xD10C);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD10D);  send_data_cmd(0x97);
			send_ctrl_cmd(0xD10E);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD10F);  send_data_cmd(0xC8);
			send_ctrl_cmd(0xD110);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD111);  send_data_cmd(0xF3);
			send_ctrl_cmd(0xD112);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD113);  send_data_cmd(0x38);
			send_ctrl_cmd(0xD114);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD115);  send_data_cmd(0x6C);
			send_ctrl_cmd(0xD116);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD117);  send_data_cmd(0xBF);
			send_ctrl_cmd(0xD118);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD119);  send_data_cmd(0x10);
			send_ctrl_cmd(0xD11A);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD11B);  send_data_cmd(0x12);
			send_ctrl_cmd(0xD11C);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD11D);  send_data_cmd(0x57);
			send_ctrl_cmd(0xD11E);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD11F);  send_data_cmd(0xAC);
			send_ctrl_cmd(0xD120);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD121);  send_data_cmd(0xE6);
			send_ctrl_cmd(0xD122);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD123);  send_data_cmd(0xFF);
			send_ctrl_cmd(0xD124);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD125);  send_data_cmd(0x0C);
			send_ctrl_cmd(0xD126);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD127);  send_data_cmd(0x34);
			send_ctrl_cmd(0xD128);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD129);  send_data_cmd(0x50);
			send_ctrl_cmd(0xD12A);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD12B);  send_data_cmd(0xF1);
			send_ctrl_cmd(0xD12C);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD12D);  send_data_cmd(0x85);
			send_ctrl_cmd(0xD12E);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD12F);  send_data_cmd(0xA1);
			send_ctrl_cmd(0xD130);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD131);  send_data_cmd(0xBC);
			send_ctrl_cmd(0xD132);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD133);  send_data_cmd(0x83);
			
			send_ctrl_cmd(0xD200);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD201);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD202);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD203);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD204);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD205);  send_data_cmd(0x25);
			send_ctrl_cmd(0xD206);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD207);  send_data_cmd(0x3B);
			send_ctrl_cmd(0xD208);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD209);  send_data_cmd(0x53);
			send_ctrl_cmd(0xD20A);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD20B);  send_data_cmd(0x7B);
			send_ctrl_cmd(0xD20C);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD20D);  send_data_cmd(0x97);
			send_ctrl_cmd(0xD20E);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD20F);  send_data_cmd(0xC8);
			send_ctrl_cmd(0xD210);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD211);  send_data_cmd(0xF3);
			send_ctrl_cmd(0xD212);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD213);  send_data_cmd(0x38);
			send_ctrl_cmd(0xD214);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD215);  send_data_cmd(0x6C);
			send_ctrl_cmd(0xD216);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD217);  send_data_cmd(0xBF);
			send_ctrl_cmd(0xD218);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD219);  send_data_cmd(0x10);
			send_ctrl_cmd(0xD21A);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD21B);  send_data_cmd(0x12);
			send_ctrl_cmd(0xD21C);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD21D);  send_data_cmd(0x57);
			send_ctrl_cmd(0xD21E);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD21F);  send_data_cmd(0xAC);
			send_ctrl_cmd(0xD220);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD221);  send_data_cmd(0xE6);
			send_ctrl_cmd(0xD222);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD223);  send_data_cmd(0xFF);
			send_ctrl_cmd(0xD224);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD225);  send_data_cmd(0x0C);
			send_ctrl_cmd(0xD226);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD227);  send_data_cmd(0x34);
			send_ctrl_cmd(0xD228);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD229);  send_data_cmd(0x50);
			send_ctrl_cmd(0xD22A);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD22B);  send_data_cmd(0xF1);
			send_ctrl_cmd(0xD22C);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD22D);  send_data_cmd(0x85);
			send_ctrl_cmd(0xD22E);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD22F);  send_data_cmd(0xA1);
			send_ctrl_cmd(0xD230);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD231);  send_data_cmd(0xBC);
			send_ctrl_cmd(0xD232);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD233);  send_data_cmd(0x83);
			
			send_ctrl_cmd(0xD300);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD301);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD302);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD303);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD304);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD305);  send_data_cmd(0x25);
			send_ctrl_cmd(0xD306);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD307);  send_data_cmd(0x3B);
			send_ctrl_cmd(0xD308);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD309);  send_data_cmd(0x53);
			send_ctrl_cmd(0xD30A);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD30B);  send_data_cmd(0x7B);
			send_ctrl_cmd(0xD30C);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD30D);  send_data_cmd(0x97);
			send_ctrl_cmd(0xD30E);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD30F);  send_data_cmd(0xC8);
			send_ctrl_cmd(0xD310);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD311);  send_data_cmd(0xF3);
			send_ctrl_cmd(0xD312);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD313);  send_data_cmd(0x38);
			send_ctrl_cmd(0xD314);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD315);  send_data_cmd(0x6C);
			send_ctrl_cmd(0xD316);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD317);  send_data_cmd(0xBF);
			send_ctrl_cmd(0xD318);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD319);  send_data_cmd(0x10);
			send_ctrl_cmd(0xD31A);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD31B);  send_data_cmd(0x12);
			send_ctrl_cmd(0xD31C);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD31D);  send_data_cmd(0x57);
			send_ctrl_cmd(0xD31E);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD31F);  send_data_cmd(0xAC);
			send_ctrl_cmd(0xD320);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD321);  send_data_cmd(0xE6);
			send_ctrl_cmd(0xD322);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD323);  send_data_cmd(0xFF);
			send_ctrl_cmd(0xD324);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD325);  send_data_cmd(0x0C);
			send_ctrl_cmd(0xD326);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD327);  send_data_cmd(0x34);
			send_ctrl_cmd(0xD328);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD329);  send_data_cmd(0x50);
			send_ctrl_cmd(0xD32A);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD32B);  send_data_cmd(0xF1);
			send_ctrl_cmd(0xD32C);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD32D);  send_data_cmd(0x85);
			send_ctrl_cmd(0xD32E);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD32F);  send_data_cmd(0xA1);
			send_ctrl_cmd(0xD330);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD331);  send_data_cmd(0xBC);
			send_ctrl_cmd(0xD332);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD333);  send_data_cmd(0x83);
			
			send_ctrl_cmd(0xD400);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD401);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD402);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD403);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD404);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD405);  send_data_cmd(0x25);
			send_ctrl_cmd(0xD406);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD407);  send_data_cmd(0x3B);
			send_ctrl_cmd(0xD408);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD409);  send_data_cmd(0x53);
			send_ctrl_cmd(0xD40A);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD40B);  send_data_cmd(0x7B);
			send_ctrl_cmd(0xD40C);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD40D);  send_data_cmd(0x97);
			send_ctrl_cmd(0xD40E);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD40F);  send_data_cmd(0xC8);
			send_ctrl_cmd(0xD410);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD411);  send_data_cmd(0xF3);
			send_ctrl_cmd(0xD412);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD413);  send_data_cmd(0x38);
			send_ctrl_cmd(0xD414);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD415);  send_data_cmd(0x6C);
			send_ctrl_cmd(0xD416);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD417);  send_data_cmd(0xBF);
			send_ctrl_cmd(0xD418);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD419);  send_data_cmd(0x10);
			send_ctrl_cmd(0xD41A);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD41B);  send_data_cmd(0x12);
			send_ctrl_cmd(0xD41C);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD41D);  send_data_cmd(0x57);
			send_ctrl_cmd(0xD41E);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD41F);  send_data_cmd(0xAC);
			send_ctrl_cmd(0xD420);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD421);  send_data_cmd(0xE6);
			send_ctrl_cmd(0xD422);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD423);  send_data_cmd(0xFF);
			send_ctrl_cmd(0xD424);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD425);  send_data_cmd(0x0C);
			send_ctrl_cmd(0xD426);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD427);  send_data_cmd(0x34);
			send_ctrl_cmd(0xD428);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD429);  send_data_cmd(0x50);
			send_ctrl_cmd(0xD42A);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD42B);  send_data_cmd(0xF1);
			send_ctrl_cmd(0xD42C);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD42D);  send_data_cmd(0x85);
			send_ctrl_cmd(0xD42E);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD42F);  send_data_cmd(0xA1);
			send_ctrl_cmd(0xD430);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD431);  send_data_cmd(0xBC);
			send_ctrl_cmd(0xD432);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD433);  send_data_cmd(0x83);
			
			send_ctrl_cmd(0xD500);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD501);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD502);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD503);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD504);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD505);  send_data_cmd(0x25);
			send_ctrl_cmd(0xD506);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD507);  send_data_cmd(0x3B);
			send_ctrl_cmd(0xD508);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD509);  send_data_cmd(0x53);
			send_ctrl_cmd(0xD50A);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD50B);  send_data_cmd(0x7B);
			send_ctrl_cmd(0xD50C);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD50D);  send_data_cmd(0x97);
			send_ctrl_cmd(0xD50E);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD50F);  send_data_cmd(0xC8);
			send_ctrl_cmd(0xD510);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD511);  send_data_cmd(0xF3);
			send_ctrl_cmd(0xD512);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD513);  send_data_cmd(0x38);
			send_ctrl_cmd(0xD514);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD515);  send_data_cmd(0x6C);
			send_ctrl_cmd(0xD516);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD517);  send_data_cmd(0xBF);
			send_ctrl_cmd(0xD518);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD519);  send_data_cmd(0x10);
			send_ctrl_cmd(0xD51A);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD51B);  send_data_cmd(0x12);
			send_ctrl_cmd(0xD51C);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD51D);  send_data_cmd(0x57);
			send_ctrl_cmd(0xD51E);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD51F);  send_data_cmd(0xAC);
			send_ctrl_cmd(0xD520);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD521);  send_data_cmd(0xE6);
			send_ctrl_cmd(0xD522);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD523);  send_data_cmd(0xFF);
			send_ctrl_cmd(0xD524);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD525);  send_data_cmd(0x0C);
			send_ctrl_cmd(0xD526);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD527);  send_data_cmd(0x34);
			send_ctrl_cmd(0xD528);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD529);  send_data_cmd(0x50);
			send_ctrl_cmd(0xD52A);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD52B);  send_data_cmd(0xF1);
			send_ctrl_cmd(0xD52C);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD52D);  send_data_cmd(0x85);
			send_ctrl_cmd(0xD52E);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD52F);  send_data_cmd(0xA1);
			send_ctrl_cmd(0xD530);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD531);  send_data_cmd(0xBC);
			send_ctrl_cmd(0xD532);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD533);  send_data_cmd(0x83);
			
			send_ctrl_cmd(0xD600);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD601);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD602);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD603);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD604);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD605);  send_data_cmd(0x25);
			send_ctrl_cmd(0xD606);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD607);  send_data_cmd(0x3B);
			send_ctrl_cmd(0xD608);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD609);  send_data_cmd(0x53);
			send_ctrl_cmd(0xD60A);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD60B);  send_data_cmd(0x7B);
			send_ctrl_cmd(0xD60C);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD60D);  send_data_cmd(0x97);
			send_ctrl_cmd(0xD60E);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD60F);  send_data_cmd(0xC8);
			send_ctrl_cmd(0xD610);  send_data_cmd(0x00);
			send_ctrl_cmd(0xD611);  send_data_cmd(0xF3);
			send_ctrl_cmd(0xD612);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD613);  send_data_cmd(0x38);
			send_ctrl_cmd(0xD614);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD615);  send_data_cmd(0x6C);
			send_ctrl_cmd(0xD616);  send_data_cmd(0x01);
			send_ctrl_cmd(0xD617);  send_data_cmd(0xBF);
			send_ctrl_cmd(0xD618);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD619);  send_data_cmd(0x10);
			send_ctrl_cmd(0xD61A);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD61B);  send_data_cmd(0x12);
			send_ctrl_cmd(0xD61C);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD61D);  send_data_cmd(0x57);
			send_ctrl_cmd(0xD61E);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD61F);  send_data_cmd(0xAC);
			send_ctrl_cmd(0xD620);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD621);  send_data_cmd(0xE6);
			send_ctrl_cmd(0xD622);  send_data_cmd(0x02);
			send_ctrl_cmd(0xD623);  send_data_cmd(0xFF);
			send_ctrl_cmd(0xD624);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD625);  send_data_cmd(0x0C);
			send_ctrl_cmd(0xD626);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD627);  send_data_cmd(0x34);
			send_ctrl_cmd(0xD628);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD629);  send_data_cmd(0x50);
			send_ctrl_cmd(0xD62A);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD62B);  send_data_cmd(0xF1);
			send_ctrl_cmd(0xD62C);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD62D);  send_data_cmd(0x85);
			send_ctrl_cmd(0xD62E);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD62F);  send_data_cmd(0xA1);
			send_ctrl_cmd(0xD630);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD631);  send_data_cmd(0xBC);
			send_ctrl_cmd(0xD632);  send_data_cmd(0x03);
			send_ctrl_cmd(0xD633);  send_data_cmd(0x83);
			
			/** Select Command Page '0' **/
			send_ctrl_cmd(0xF000);  send_data_cmd(0x55);
			send_ctrl_cmd(0xF001);  send_data_cmd(0xAA);
			send_ctrl_cmd(0xF002);  send_data_cmd(0x52);
			send_ctrl_cmd(0xF003);  send_data_cmd(0x08);
			send_ctrl_cmd(0xF004);  send_data_cmd(0x00);
			
			// Display Option Control
			send_ctrl_cmd(0xB100);  send_data_cmd(0xCC); 
			
			// Vivid Color
			send_ctrl_cmd(0xB400);  send_data_cmd(0x10); 
			
			// Source Driver Output Delay
			send_ctrl_cmd(0xB600);  send_data_cmd(0x01); //08
			
			// EQ Control Function for Source Driver
			send_ctrl_cmd(0xB800);  send_data_cmd(0x01);  
			send_ctrl_cmd(0xB801);  send_data_cmd(0x04);  
			send_ctrl_cmd(0xB802);  send_data_cmd(0x04);  
			send_ctrl_cmd(0xB803);  send_data_cmd(0x04);  
			
			// Display Timing Control
			send_ctrl_cmd(0xC900);  send_data_cmd(0xA0); 
			
			// Inversion Driver Control
			send_ctrl_cmd(0xBC00);  send_data_cmd(0x02);  
			send_ctrl_cmd(0xBC01);  send_data_cmd(0x02);  
			send_ctrl_cmd(0xBC02);  send_data_cmd(0x02);  
			
			send_ctrl_cmd(0x3500);  send_data_cmd(0x00);//03 TE Enable 
			send_ctrl_cmd(0x3600);  send_data_cmd(0x00);//00Õý·Å£¬C0µ¹·Å
			send_ctrl_cmd(0x3A00);  send_data_cmd(0x66);//18bit---66 
			 
			send_ctrl_cmd(0x1100);  send_data_cmd(0x00);//Sleep out  
			MDELAY(120);//delay 120ms  
			    
			send_ctrl_cmd(0x2900);  send_data_cmd(0x00);//Display on 
		}
		else// BOE yashu
		{
			send_ctrl_cmd(0xF000);send_data_cmd(0X55);
			send_ctrl_cmd(0xF001);send_data_cmd(0XAA);
			send_ctrl_cmd(0xF002);send_data_cmd(0X52);
			send_ctrl_cmd(0xF003);send_data_cmd(0X08);
			send_ctrl_cmd(0xF004);send_data_cmd(0X00);
			
			send_ctrl_cmd(0xB000);send_data_cmd(0X08);
			
			send_ctrl_cmd(0xB100);send_data_cmd(0XFC);
			
			send_ctrl_cmd(0xB101);send_data_cmd(0X00);//top left right bottom
			
			send_ctrl_cmd(0xB400);send_data_cmd(0X10);
			
			send_ctrl_cmd(0xB500);send_data_cmd(0X50);
			
			send_ctrl_cmd(0xB600);send_data_cmd(0X08);//
			
			send_ctrl_cmd(0xB800);send_data_cmd(0X00);
			send_ctrl_cmd(0xB801);send_data_cmd(0X02);
			send_ctrl_cmd(0xB802);send_data_cmd(0X02);
			send_ctrl_cmd(0xB803);send_data_cmd(0X02);
		
			send_ctrl_cmd(0xBC00);send_data_cmd(0X00);//0x02
			send_ctrl_cmd(0xBC01);send_data_cmd(0X00);
			send_ctrl_cmd(0xBC02);send_data_cmd(0X00);
		
			send_ctrl_cmd(0xCC00);send_data_cmd(0X03);
		
		
			send_ctrl_cmd(0xF000);send_data_cmd(0X55);
			send_ctrl_cmd(0xF001);send_data_cmd(0XAA);
			send_ctrl_cmd(0xF002);send_data_cmd(0X52);
			send_ctrl_cmd(0xF003);send_data_cmd(0X08);
			send_ctrl_cmd(0xF004);send_data_cmd(0X01);
		
			send_ctrl_cmd(0xB000);send_data_cmd(0X00);
			send_ctrl_cmd(0xB001);send_data_cmd(0X00);
			send_ctrl_cmd(0xB002);send_data_cmd(0X00);
		
			send_ctrl_cmd(0xB100);send_data_cmd(0X00);
			send_ctrl_cmd(0xB101);send_data_cmd(0X00);
			send_ctrl_cmd(0xB102);send_data_cmd(0X00);
		
			send_ctrl_cmd(0xB300);send_data_cmd(0X0F);
			send_ctrl_cmd(0xB301);send_data_cmd(0X0F);
			send_ctrl_cmd(0xB302);send_data_cmd(0X0F);
		
			send_ctrl_cmd(0xB500);send_data_cmd(0X08);
			send_ctrl_cmd(0xB501);send_data_cmd(0X08);
			send_ctrl_cmd(0xB502);send_data_cmd(0X08);
			
			send_ctrl_cmd(0xB600);send_data_cmd(0X34);
			send_ctrl_cmd(0xB601);send_data_cmd(0X34);
			send_ctrl_cmd(0xB602);send_data_cmd(0X34);
			
			send_ctrl_cmd(0xB700);send_data_cmd(0X34);
			send_ctrl_cmd(0xB701);send_data_cmd(0X34);
			send_ctrl_cmd(0xB702);send_data_cmd(0X34);
			
			send_ctrl_cmd(0xB800);send_data_cmd(0X24);
			send_ctrl_cmd(0xB801);send_data_cmd(0X24);
			send_ctrl_cmd(0xB802);send_data_cmd(0X24);
			
			send_ctrl_cmd(0xB900);send_data_cmd(0X14);
			send_ctrl_cmd(0xB901);send_data_cmd(0X14);
			send_ctrl_cmd(0xB902);send_data_cmd(0X14);
			
			send_ctrl_cmd(0xBA00);send_data_cmd(0X14);
			send_ctrl_cmd(0xBA01);send_data_cmd(0X14);
			send_ctrl_cmd(0xBA02);send_data_cmd(0X14);
			
			send_ctrl_cmd(0xBC00);send_data_cmd(0X00);
			send_ctrl_cmd(0xBC01);send_data_cmd(0X38);//0x48
			send_ctrl_cmd(0xBC02);send_data_cmd(0X00);
		
			send_ctrl_cmd(0xBD00);send_data_cmd(0X00);
			send_ctrl_cmd(0xBD01);send_data_cmd(0X38);
			send_ctrl_cmd(0xBD02);send_data_cmd(0X00);
		
			send_ctrl_cmd(0xBE00);send_data_cmd(0X00);
			send_ctrl_cmd(0xBE01);send_data_cmd(0X40);//0x2A
		
			send_ctrl_cmd(0xBF00);send_data_cmd(0X01);
		
			send_ctrl_cmd(0xF000);send_data_cmd(0X55);
			send_ctrl_cmd(0xF001);send_data_cmd(0XAA);
			send_ctrl_cmd(0xF002);send_data_cmd(0X52);
			send_ctrl_cmd(0xF003);send_data_cmd(0X08);
			send_ctrl_cmd(0xF004);send_data_cmd(0X01);
		
		
		send_ctrl_cmd(0xD100);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD101);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD102);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD103);  send_data_cmd(0x0C);
		send_ctrl_cmd(0xD104);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD105);  send_data_cmd(0x1B);
		send_ctrl_cmd(0xD106);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD107);  send_data_cmd(0x3D);
		send_ctrl_cmd(0xD108);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD109);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD10A);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD10B);  send_data_cmd(0x6c);
		send_ctrl_cmd(0xD10C);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD10D);  send_data_cmd(0x76);
		send_ctrl_cmd(0xD10E);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD10F);  send_data_cmd(0x7A);
		send_ctrl_cmd(0xD110);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD111);  send_data_cmd(0x9B);
		send_ctrl_cmd(0xD112);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD113);  send_data_cmd(0xE1);
		send_ctrl_cmd(0xD114);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD115);  send_data_cmd(0x1D);
		send_ctrl_cmd(0xD116);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD117);  send_data_cmd(0x82);
		send_ctrl_cmd(0xD118);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD119);  send_data_cmd(0xD5);
		send_ctrl_cmd(0xD11A);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD11B);  send_data_cmd(0xD7);
		send_ctrl_cmd(0xD11C);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD11D);  send_data_cmd(0x13);
		send_ctrl_cmd(0xD11E);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD11F);  send_data_cmd(0x7D);
		send_ctrl_cmd(0xD120);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD121);  send_data_cmd(0xC2);
		send_ctrl_cmd(0xD122);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD123);  send_data_cmd(0x36);
		send_ctrl_cmd(0xD124);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD125);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD126);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD127);  send_data_cmd(0x68);
		send_ctrl_cmd(0xD128);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD129);  send_data_cmd(0x81);
		send_ctrl_cmd(0xD12A);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD12B);  send_data_cmd(0x9A);
		send_ctrl_cmd(0xD12C);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD12D);  send_data_cmd(0xB3);
		send_ctrl_cmd(0xD12E);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD12F);  send_data_cmd(0xCC);
		send_ctrl_cmd(0xD130);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD131);  send_data_cmd(0xE5);
		send_ctrl_cmd(0xD132);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD133);  send_data_cmd(0xFF);
		
		send_ctrl_cmd(0xD200);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD201);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD202);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD203);  send_data_cmd(0x0C);
		send_ctrl_cmd(0xD204);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD205);  send_data_cmd(0x1B);
		send_ctrl_cmd(0xD206);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD207);  send_data_cmd(0x3D);
		send_ctrl_cmd(0xD208);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD209);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD20A);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD20B);  send_data_cmd(0x6c);
		send_ctrl_cmd(0xD20C);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD20D);  send_data_cmd(0x76);
		send_ctrl_cmd(0xD20E);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD20F);  send_data_cmd(0x7A);
		send_ctrl_cmd(0xD210);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD211);  send_data_cmd(0x9B);
		send_ctrl_cmd(0xD212);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD213);  send_data_cmd(0xE1);
		send_ctrl_cmd(0xD214);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD215);  send_data_cmd(0x1D);
		send_ctrl_cmd(0xD216);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD217);  send_data_cmd(0x82);
		send_ctrl_cmd(0xD218);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD219);  send_data_cmd(0xD5);
		send_ctrl_cmd(0xD21A);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD21B);  send_data_cmd(0xD7);
		send_ctrl_cmd(0xD21C);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD21D);  send_data_cmd(0x13);
		send_ctrl_cmd(0xD21E);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD21F);  send_data_cmd(0x7D);
		send_ctrl_cmd(0xD220);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD221);  send_data_cmd(0xC2);
		send_ctrl_cmd(0xD222);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD223);  send_data_cmd(0x36);
		send_ctrl_cmd(0xD224);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD225);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD226);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD227);  send_data_cmd(0x68);
		send_ctrl_cmd(0xD228);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD229);  send_data_cmd(0x81);
		send_ctrl_cmd(0xD22A);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD22B);  send_data_cmd(0x9A);
		send_ctrl_cmd(0xD22C);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD22D);  send_data_cmd(0xB3);
		send_ctrl_cmd(0xD22E);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD22F);  send_data_cmd(0xCC);
		send_ctrl_cmd(0xD230);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD231);  send_data_cmd(0xE5);
		send_ctrl_cmd(0xD232);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD233);  send_data_cmd(0xFF);
		//G+
		send_ctrl_cmd(0xD300);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD301);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD302);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD303);  send_data_cmd(0x0C);
		send_ctrl_cmd(0xD304);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD305);  send_data_cmd(0x1B);
		send_ctrl_cmd(0xD306);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD307);  send_data_cmd(0x3D);
		send_ctrl_cmd(0xD308);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD309);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD30A);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD30B);  send_data_cmd(0x6c);
		send_ctrl_cmd(0xD30C);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD30D);  send_data_cmd(0x76);
		send_ctrl_cmd(0xD30E);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD30F);  send_data_cmd(0x7A);
		send_ctrl_cmd(0xD310);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD311);  send_data_cmd(0x9B);
		send_ctrl_cmd(0xD312);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD313);  send_data_cmd(0xE1);
		send_ctrl_cmd(0xD314);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD315);  send_data_cmd(0x1D);
		send_ctrl_cmd(0xD316);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD317);  send_data_cmd(0x82);
		send_ctrl_cmd(0xD318);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD319);  send_data_cmd(0xD5);
		send_ctrl_cmd(0xD31A);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD31B);  send_data_cmd(0xD7);
		send_ctrl_cmd(0xD31C);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD31D);  send_data_cmd(0x13);
		send_ctrl_cmd(0xD31E);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD31F);  send_data_cmd(0x7D);
		send_ctrl_cmd(0xD320);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD321);  send_data_cmd(0xC2);
		send_ctrl_cmd(0xD322);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD323);  send_data_cmd(0x36);
		send_ctrl_cmd(0xD324);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD325);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD326);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD327);  send_data_cmd(0x68);
		send_ctrl_cmd(0xD328);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD329);  send_data_cmd(0x81);
		send_ctrl_cmd(0xD32A);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD32B);  send_data_cmd(0x9A);
		send_ctrl_cmd(0xD32C);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD32D);  send_data_cmd(0xB3);
		send_ctrl_cmd(0xD32E);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD32F);  send_data_cmd(0xCC);
		send_ctrl_cmd(0xD330);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD331);  send_data_cmd(0xE5);
		send_ctrl_cmd(0xD332);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD333);  send_data_cmd(0xFF);
		//G+
		
		send_ctrl_cmd(0xD400);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD401);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD402);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD403);  send_data_cmd(0x0C);
		send_ctrl_cmd(0xD404);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD405);  send_data_cmd(0x1B);
		send_ctrl_cmd(0xD406);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD407);  send_data_cmd(0x3D);
		send_ctrl_cmd(0xD408);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD409);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD40A);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD40B);  send_data_cmd(0x6c);
		send_ctrl_cmd(0xD40C);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD40D);  send_data_cmd(0x76);
		send_ctrl_cmd(0xD40E);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD40F);  send_data_cmd(0x7A);
		send_ctrl_cmd(0xD410);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD411);  send_data_cmd(0x9B);
		send_ctrl_cmd(0xD412);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD413);  send_data_cmd(0xE1);
		send_ctrl_cmd(0xD414);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD415);  send_data_cmd(0x1D);
		send_ctrl_cmd(0xD416);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD417);  send_data_cmd(0x82);
		send_ctrl_cmd(0xD418);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD419);  send_data_cmd(0xD5);
		send_ctrl_cmd(0xD41A);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD41B);  send_data_cmd(0xD7);
		send_ctrl_cmd(0xD41C);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD41D);  send_data_cmd(0x13);
		send_ctrl_cmd(0xD41E);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD41F);  send_data_cmd(0x7D);
		send_ctrl_cmd(0xD420);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD421);  send_data_cmd(0xC2);
		send_ctrl_cmd(0xD422);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD423);  send_data_cmd(0x36);
		send_ctrl_cmd(0xD424);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD425);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD426);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD427);  send_data_cmd(0x68);
		send_ctrl_cmd(0xD428);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD429);  send_data_cmd(0x81);
		send_ctrl_cmd(0xD42A);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD42B);  send_data_cmd(0x9A);
		send_ctrl_cmd(0xD42C);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD42D);  send_data_cmd(0xB3);
		send_ctrl_cmd(0xD42E);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD42F);  send_data_cmd(0xCC);
		send_ctrl_cmd(0xD430);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD431);  send_data_cmd(0xE5);
		send_ctrl_cmd(0xD432);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD433);  send_data_cmd(0xFF);
		//G+
		send_ctrl_cmd(0xD500);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD501);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD502);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD503);  send_data_cmd(0x0C);
		send_ctrl_cmd(0xD504);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD505);  send_data_cmd(0x1B);
		send_ctrl_cmd(0xD506);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD507);  send_data_cmd(0x3D);
		send_ctrl_cmd(0xD508);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD509);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD50A);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD50B);  send_data_cmd(0x6c);
		send_ctrl_cmd(0xD50C);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD50D);  send_data_cmd(0x76);
		send_ctrl_cmd(0xD50E);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD50F);  send_data_cmd(0x7A);
		send_ctrl_cmd(0xD510);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD511);  send_data_cmd(0x9B);
		send_ctrl_cmd(0xD512);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD513);  send_data_cmd(0xE1);
		send_ctrl_cmd(0xD514);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD515);  send_data_cmd(0x1D);
		send_ctrl_cmd(0xD516);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD517);  send_data_cmd(0x82);
		send_ctrl_cmd(0xD518);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD519);  send_data_cmd(0xD5);
		send_ctrl_cmd(0xD51A);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD51B);  send_data_cmd(0xD7);
		send_ctrl_cmd(0xD51C);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD51D);  send_data_cmd(0x13);
		send_ctrl_cmd(0xD51E);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD51F);  send_data_cmd(0x7D);
		send_ctrl_cmd(0xD520);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD521);  send_data_cmd(0xC2);
		send_ctrl_cmd(0xD522);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD523);  send_data_cmd(0x36);
		send_ctrl_cmd(0xD524);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD525);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD526);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD527);  send_data_cmd(0x68);
		send_ctrl_cmd(0xD528);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD529);  send_data_cmd(0x81);
		send_ctrl_cmd(0xD52A);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD52B);  send_data_cmd(0x9A);
		send_ctrl_cmd(0xD52C);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD52D);  send_data_cmd(0xB3);
		send_ctrl_cmd(0xD52E);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD52F);  send_data_cmd(0xCC);
		send_ctrl_cmd(0xD530);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD531);  send_data_cmd(0xE5);
		send_ctrl_cmd(0xD532);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD533);  send_data_cmd(0xFF);
		//G+
		send_ctrl_cmd(0xD600);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD601);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD602);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD603);  send_data_cmd(0x0C);
		send_ctrl_cmd(0xD604);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD605);  send_data_cmd(0x1B);
		send_ctrl_cmd(0xD606);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD607);  send_data_cmd(0x3D);
		send_ctrl_cmd(0xD608);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD609);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD60A);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD60B);  send_data_cmd(0x6c);
		send_ctrl_cmd(0xD60C);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD60D);  send_data_cmd(0x76);
		send_ctrl_cmd(0xD60E);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD60F);  send_data_cmd(0x7A);
		send_ctrl_cmd(0xD610);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD611);  send_data_cmd(0x9B);
		send_ctrl_cmd(0xD612);  send_data_cmd(0x00);
		send_ctrl_cmd(0xD613);  send_data_cmd(0xE1);
		send_ctrl_cmd(0xD614);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD615);  send_data_cmd(0x1D);
		send_ctrl_cmd(0xD616);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD617);  send_data_cmd(0x82);
		send_ctrl_cmd(0xD618);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD619);  send_data_cmd(0xD6);
		send_ctrl_cmd(0xD61A);  send_data_cmd(0x01);
		send_ctrl_cmd(0xD61B);  send_data_cmd(0xD7);
		send_ctrl_cmd(0xD61C);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD61D);  send_data_cmd(0x13);
		send_ctrl_cmd(0xD61E);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD61F);  send_data_cmd(0x7D);
		send_ctrl_cmd(0xD620);  send_data_cmd(0x02);
		send_ctrl_cmd(0xD621);  send_data_cmd(0xC2);
		send_ctrl_cmd(0xD622);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD623);  send_data_cmd(0x36);
		send_ctrl_cmd(0xD624);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD625);  send_data_cmd(0x4F);
		send_ctrl_cmd(0xD626);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD627);  send_data_cmd(0x68);
		send_ctrl_cmd(0xD628);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD629);  send_data_cmd(0x81);
		send_ctrl_cmd(0xD62A);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD62B);  send_data_cmd(0x9A);
		send_ctrl_cmd(0xD62C);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD62D);  send_data_cmd(0xB3);
		send_ctrl_cmd(0xD62E);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD62F);  send_data_cmd(0xCC);
		send_ctrl_cmd(0xD630);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD631);  send_data_cmd(0xE5);
		send_ctrl_cmd(0xD632);  send_data_cmd(0x03);
		send_ctrl_cmd(0xD633);  send_data_cmd(0xFF);
		
		
			send_ctrl_cmd(0x3600);send_data_cmd(0x00);
			
			send_ctrl_cmd(0x3500);send_data_cmd(0x00);
			send_ctrl_cmd(0x4400);send_data_cmd(0x00);
			send_ctrl_cmd(0x4401);send_data_cmd(0x22);
		
			send_ctrl_cmd(0x3A00);send_data_cmd(0x66);
		
			send_ctrl_cmd(0x1100);
			MDELAY(150);
		
			send_ctrl_cmd(0x2900);
			MDELAY(10);
		}   

    MDELAY(200);
    sw_clear_panel(0x0);    // Clean panel as black
    MDELAY(10);
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

    params->dbi.port                    = 0;
    params->dbi.data_width              = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dbi.data_format.trans_seq   = LCM_DBI_TRANS_SEQ_MSB_FIRST;
    params->dbi.data_format.padding     = LCM_DBI_PADDING_ON_LSB;
    params->dbi.data_format.format      = LCM_DBI_FORMAT_RGB666;
    params->dbi.data_format.width       = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.cpu_write_bits          = LCM_DBI_CPU_WRITE_32_BITS;
    params->dbi.io_driving_current      = LCM_DRIVING_CURRENT_6575_8MA;

    params->dbi.parallel.write_setup    = 2;//0
    params->dbi.parallel.write_hold     = 2;//0
    params->dbi.parallel.write_wait     = 6;//2
    params->dbi.parallel.read_setup     = 5;//5
    params->dbi.parallel.read_hold      = 0;
    params->dbi.parallel.read_latency   = 31;//15
    params->dbi.parallel.wait_period    = 6;//1
    params->dbi.parallel.cs_high_width  = 0; //cycles of cs high level between each transfer
    // enable tearing-free
    params->dbi.te_mode                 = LCM_DBI_TE_MODE_VSYNC_ONLY;
    params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;
}


static void lcm_init(void)
{
    SET_RESET_PIN(0);
    MDELAY(200);
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

	  //add by xia lei jie for LCD INIT   
    lcm_init();
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

static unsigned int lcm_compare_id(void)
{
    unsigned int id = 0;    	
    unsigned int ID1,ID2;
         
		//LV2 Page 1 enable
		send_ctrl_cmd(0xF000); send_data_cmd(0x55);
		send_ctrl_cmd(0xF001); send_data_cmd(0xAA);
		send_ctrl_cmd(0xF002); send_data_cmd(0x52);
		send_ctrl_cmd(0xF003); send_data_cmd(0x08);
		send_ctrl_cmd(0xF004); send_data_cmd(0x01);
     
     send_ctrl_cmd(0xC500); MDELAY(2);
     ID1 = read_data_cmd(); //0X18

     send_ctrl_cmd(0xC501); MDELAY(2);    
     ID2 = read_data_cmd(); //0X68    

     id=((ID2 & 0xff) << 8) | ID1; //0x6818
     
     return (LCM_ID == id)?1:0;
}

LCM_DRIVER rm68180_daxian_D397N3189V1_HYDIS_mcu18_wvga_lcm_drv =
    {
        .name			= "rm68180_daxian_D397N3189V1_HYDIS_mcu18_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id     = lcm_compare_id
    };
