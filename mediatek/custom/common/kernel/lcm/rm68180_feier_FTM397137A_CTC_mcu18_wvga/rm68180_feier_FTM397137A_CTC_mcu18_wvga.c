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
#define FRAME_HEIGHT (800)
#define LCM_ID       (0x8000)
#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN

#define   __LCM_TE_ON__
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

#define WriteComm send_ctrl_cmd
#define WriteData send_data_cmd

static void init_lcm_registers(void)
{
#if defined(BUILD_UBOOT)
    printf("MYCAT LCD nt35510_hansen_hs4033wvn35_02_mcu_wvga");
#endif
//LV2 Page 1 enable
WriteComm(0xF000);WriteData(0x55);
WriteComm(0xF001);WriteData(0xAA);
WriteComm(0xF002);WriteData(0x52);
WriteComm(0xF003);WriteData(0x08);
WriteComm(0xF004);WriteData(0x01);

WriteComm(0xCC00);WriteData(0x05);

//AVDD Set AVDD 5.2V
WriteComm(0xB000);WriteData(0x0D);
WriteComm(0xB001);WriteData(0x0D);
WriteComm(0xB002);WriteData(0x0D);

//AVDD ratio
WriteComm(0xB600);WriteData(0x34);
WriteComm(0xB601);WriteData(0x34);
WriteComm(0xB602);WriteData(0x34);

//AVEE -5.2V
WriteComm(0xB100);WriteData(0x05);
WriteComm(0xB101);WriteData(0x05);
WriteComm(0xB102);WriteData(0x05);

//AVEE ratio
WriteComm(0xB700);WriteData(0x34);
WriteComm(0xB701);WriteData(0x34);
WriteComm(0xB702);WriteData(0x34);

//VCL -2.5V
WriteComm(0xB200);WriteData(0x00);
WriteComm(0xB201);WriteData(0x00);
WriteComm(0xB202);WriteData(0x00);

//VCL ratio
WriteComm(0xB800);WriteData(0x24);
WriteComm(0xB801);WriteData(0x24);
WriteComm(0xB802);WriteData(0x24);

//VGH 15V (Free pump)
/*WriteComm(0xBF00);WriteData(0x01);
WriteComm(0xB300);WriteData(0x08);
WriteComm(0xB301);WriteData(0x08);
WriteComm(0xB302);WriteData(0x08);*/

//VGH ratio
WriteComm(0xB900);WriteData(0x34);
WriteComm(0xB901);WriteData(0x34);
WriteComm(0xB902);WriteData(0x34);
				 
//VGL_REG -10V
/*WriteComm(0xB500);WriteData(0x08);
WriteComm(0xB501);WriteData(0x08);
WriteComm(0xB502);WriteData(0x08);

WriteComm(0xC200);WriteData(0x03);*/
									 
//VGLX tatio
WriteComm(0xBA00);WriteData(0x04);
WriteComm(0xBA01);WriteData(0x04);
WriteComm(0xBA02);WriteData(0x04);

//VGMN/VGSN 4.5V/0V
WriteComm(0xBC00);WriteData(0x00);
WriteComm(0xBC01);WriteData(0x90);	 //\B6Աȶ\C8
WriteComm(0xBC02);WriteData(0x00);

//VGMN/VGSN -4.5V/0V
WriteComm(0xBD00);WriteData(0x00);
WriteComm(0xBD01);WriteData(0x90);  //\B6Աȶ\C8
WriteComm(0xBD02);WriteData(0x00);

//VCOM -1.25V
WriteComm(0xBE00);WriteData(0x00);
WriteComm(0xBE01);WriteData(0x75);	//BD

//Gamma Setting
WriteComm(0xD100);WriteData(0x00);
WriteComm(0xD101);WriteData(0x00);
WriteComm(0xD102);WriteData(0x00);
WriteComm(0xD103);WriteData(0x01);
WriteComm(0xD104);WriteData(0x00);
WriteComm(0xD105);WriteData(0x18);
WriteComm(0xD106);WriteData(0x00);
WriteComm(0xD107);WriteData(0x3B);
WriteComm(0xD108);WriteData(0x00);
WriteComm(0xD109);WriteData(0x60);
WriteComm(0xD10A);WriteData(0x00);
WriteComm(0xD10B);WriteData(0x9E);
WriteComm(0xD10C);WriteData(0x00);
WriteComm(0xD10D);WriteData(0xCF);
WriteComm(0xD10E);WriteData(0x01);
WriteComm(0xD10F);WriteData(0x17);
WriteComm(0xD110);WriteData(0x01);
WriteComm(0xD111);WriteData(0x4B);
WriteComm(0xD112);WriteData(0x01);
WriteComm(0xD113);WriteData(0x93);
WriteComm(0xD114);WriteData(0x01);
WriteComm(0xD115);WriteData(0xC6);
WriteComm(0xD116);WriteData(0x02);
WriteComm(0xD117);WriteData(0x10);
WriteComm(0xD118);WriteData(0x02);
WriteComm(0xD119);WriteData(0x48);
WriteComm(0xD11A);WriteData(0x02);
WriteComm(0xD11B);WriteData(0x49);
WriteComm(0xD11C);WriteData(0x02);
WriteComm(0xD11D);WriteData(0x79);
WriteComm(0xD11E);WriteData(0x02);
WriteComm(0xD11F);WriteData(0xAA);
WriteComm(0xD120);WriteData(0x02);
WriteComm(0xD121);WriteData(0xC4);
WriteComm(0xD122);WriteData(0x02);
WriteComm(0xD123);WriteData(0xE4);
WriteComm(0xD124);WriteData(0x02);
WriteComm(0xD125);WriteData(0xF8);
WriteComm(0xD126);WriteData(0x03);
WriteComm(0xD127);WriteData(0x10);
WriteComm(0xD128);WriteData(0x03);
WriteComm(0xD129);WriteData(0x20);
WriteComm(0xD12A);WriteData(0x03);
WriteComm(0xD12B);WriteData(0x36);
WriteComm(0xD12C);WriteData(0x03);
WriteComm(0xD12D);WriteData(0x46);
WriteComm(0xD12E);WriteData(0x03);
WriteComm(0xD12F);WriteData(0x5C);
WriteComm(0xD130);WriteData(0x03);
WriteComm(0xD131);WriteData(0x8B);
WriteComm(0xD132);WriteData(0x03);
WriteComm(0xD133);WriteData(0xFF);

WriteComm(0xD200);WriteData(0x00);
WriteComm(0xD201);WriteData(0x00);
WriteComm(0xD202);WriteData(0x00);
WriteComm(0xD203);WriteData(0x01);
WriteComm(0xD204);WriteData(0x00);
WriteComm(0xD205);WriteData(0x18);
WriteComm(0xD206);WriteData(0x00);
WriteComm(0xD207);WriteData(0x3B);
WriteComm(0xD208);WriteData(0x00);
WriteComm(0xD209);WriteData(0x60);
WriteComm(0xD20A);WriteData(0x00);
WriteComm(0xD20B);WriteData(0x9E);
WriteComm(0xD20C);WriteData(0x00);
WriteComm(0xD20D);WriteData(0xCF);
WriteComm(0xD20E);WriteData(0x01);
WriteComm(0xD20F);WriteData(0x17);
WriteComm(0xD210);WriteData(0x01);
WriteComm(0xD211);WriteData(0x4B);
WriteComm(0xD212);WriteData(0x01);
WriteComm(0xD213);WriteData(0x93);
WriteComm(0xD214);WriteData(0x01);
WriteComm(0xD215);WriteData(0xC6);
WriteComm(0xD216);WriteData(0x02);
WriteComm(0xD217);WriteData(0x10);
WriteComm(0xD218);WriteData(0x02);
WriteComm(0xD219);WriteData(0x48);
WriteComm(0xD21A);WriteData(0x02);
WriteComm(0xD21B);WriteData(0x49);
WriteComm(0xD21C);WriteData(0x02);
WriteComm(0xD21D);WriteData(0x79);
WriteComm(0xD21E);WriteData(0x02);
WriteComm(0xD21F);WriteData(0xAA);
WriteComm(0xD220);WriteData(0x02);
WriteComm(0xD221);WriteData(0xC4);
WriteComm(0xD222);WriteData(0x02);
WriteComm(0xD223);WriteData(0xE4);
WriteComm(0xD224);WriteData(0x02);
WriteComm(0xD225);WriteData(0xF8);
WriteComm(0xD226);WriteData(0x03);
WriteComm(0xD227);WriteData(0x10);
WriteComm(0xD228);WriteData(0x03);
WriteComm(0xD229);WriteData(0x20);
WriteComm(0xD22A);WriteData(0x03);
WriteComm(0xD22B);WriteData(0x36);
WriteComm(0xD22C);WriteData(0x03);
WriteComm(0xD22D);WriteData(0x46);
WriteComm(0xD22E);WriteData(0x03);
WriteComm(0xD22F);WriteData(0x5C);
WriteComm(0xD230);WriteData(0x03);
WriteComm(0xD231);WriteData(0x8B);
WriteComm(0xD232);WriteData(0x03);
WriteComm(0xD233);WriteData(0xFF);

WriteComm(0xD300);WriteData(0x00);
WriteComm(0xD301);WriteData(0x00);
WriteComm(0xD302);WriteData(0x00);
WriteComm(0xD303);WriteData(0x01);
WriteComm(0xD304);WriteData(0x00);
WriteComm(0xD305);WriteData(0x18);
WriteComm(0xD306);WriteData(0x00);
WriteComm(0xD307);WriteData(0x3B);
WriteComm(0xD308);WriteData(0x00);
WriteComm(0xD309);WriteData(0x60);
WriteComm(0xD30A);WriteData(0x00);
WriteComm(0xD30B);WriteData(0x9E);
WriteComm(0xD30C);WriteData(0x00);
WriteComm(0xD30D);WriteData(0xCF);
WriteComm(0xD30E);WriteData(0x01);
WriteComm(0xD30F);WriteData(0x17);
WriteComm(0xD310);WriteData(0x01);
WriteComm(0xD311);WriteData(0x4B);
WriteComm(0xD312);WriteData(0x01);
WriteComm(0xD313);WriteData(0x93);
WriteComm(0xD314);WriteData(0x01);
WriteComm(0xD315);WriteData(0xC6);
WriteComm(0xD316);WriteData(0x02);
WriteComm(0xD317);WriteData(0x10);
WriteComm(0xD318);WriteData(0x02);
WriteComm(0xD319);WriteData(0x48);
WriteComm(0xD31A);WriteData(0x02);
WriteComm(0xD31B);WriteData(0x49);
WriteComm(0xD31C);WriteData(0x02);
WriteComm(0xD31D);WriteData(0x79);
WriteComm(0xD31E);WriteData(0x02);
WriteComm(0xD31F);WriteData(0xAA);
WriteComm(0xD320);WriteData(0x02);
WriteComm(0xD321);WriteData(0xC4);
WriteComm(0xD322);WriteData(0x02);
WriteComm(0xD323);WriteData(0xE4);
WriteComm(0xD324);WriteData(0x02);
WriteComm(0xD325);WriteData(0xF8);
WriteComm(0xD326);WriteData(0x03);
WriteComm(0xD327);WriteData(0x10);
WriteComm(0xD328);WriteData(0x03);
WriteComm(0xD329);WriteData(0x20);
WriteComm(0xD32A);WriteData(0x03);
WriteComm(0xD32B);WriteData(0x36);
WriteComm(0xD32C);WriteData(0x03);
WriteComm(0xD32D);WriteData(0x46);
WriteComm(0xD32E);WriteData(0x03);
WriteComm(0xD32F);WriteData(0x5C);
WriteComm(0xD330);WriteData(0x03);
WriteComm(0xD331);WriteData(0x8B);
WriteComm(0xD332);WriteData(0x03);
WriteComm(0xD333);WriteData(0xFF);

WriteComm(0xD400);WriteData(0x00);
WriteComm(0xD401);WriteData(0x00);
WriteComm(0xD402);WriteData(0x00);
WriteComm(0xD403);WriteData(0x01);
WriteComm(0xD404);WriteData(0x00);
WriteComm(0xD405);WriteData(0x18);
WriteComm(0xD406);WriteData(0x00);
WriteComm(0xD407);WriteData(0x3B);
WriteComm(0xD408);WriteData(0x00);
WriteComm(0xD409);WriteData(0x60);
WriteComm(0xD40A);WriteData(0x00);
WriteComm(0xD40B);WriteData(0x9E);
WriteComm(0xD40C);WriteData(0x00);
WriteComm(0xD40D);WriteData(0xCF);
WriteComm(0xD40E);WriteData(0x01);
WriteComm(0xD40F);WriteData(0x17);
WriteComm(0xD410);WriteData(0x01);
WriteComm(0xD411);WriteData(0x4B);
WriteComm(0xD412);WriteData(0x01);
WriteComm(0xD413);WriteData(0x93);
WriteComm(0xD414);WriteData(0x01);
WriteComm(0xD415);WriteData(0xC6);
WriteComm(0xD416);WriteData(0x02);
WriteComm(0xD417);WriteData(0x10);
WriteComm(0xD418);WriteData(0x02);
WriteComm(0xD419);WriteData(0x48);
WriteComm(0xD41A);WriteData(0x02);
WriteComm(0xD41B);WriteData(0x49);
WriteComm(0xD41C);WriteData(0x02);
WriteComm(0xD41D);WriteData(0x79);
WriteComm(0xD41E);WriteData(0x02);
WriteComm(0xD41F);WriteData(0xAA);
WriteComm(0xD420);WriteData(0x02);
WriteComm(0xD421);WriteData(0xC4);
WriteComm(0xD422);WriteData(0x02);
WriteComm(0xD423);WriteData(0xE4);
WriteComm(0xD424);WriteData(0x02);
WriteComm(0xD425);WriteData(0xF8);
WriteComm(0xD426);WriteData(0x03);
WriteComm(0xD427);WriteData(0x10);
WriteComm(0xD428);WriteData(0x03);
WriteComm(0xD429);WriteData(0x20);
WriteComm(0xD42A);WriteData(0x03);
WriteComm(0xD42B);WriteData(0x36);
WriteComm(0xD42C);WriteData(0x03);
WriteComm(0xD42D);WriteData(0x46);
WriteComm(0xD42E);WriteData(0x03);
WriteComm(0xD42F);WriteData(0x5C);
WriteComm(0xD430);WriteData(0x03);
WriteComm(0xD431);WriteData(0x8B);
WriteComm(0xD432);WriteData(0x03);
WriteComm(0xD433);WriteData(0xFF);

WriteComm(0xD500);WriteData(0x00);
WriteComm(0xD501);WriteData(0x00);
WriteComm(0xD502);WriteData(0x00);
WriteComm(0xD503);WriteData(0x01);
WriteComm(0xD504);WriteData(0x00);
WriteComm(0xD505);WriteData(0x18);
WriteComm(0xD506);WriteData(0x00);
WriteComm(0xD507);WriteData(0x3B);
WriteComm(0xD508);WriteData(0x00);
WriteComm(0xD509);WriteData(0x60);
WriteComm(0xD50A);WriteData(0x00);
WriteComm(0xD50B);WriteData(0x9E);
WriteComm(0xD50C);WriteData(0x00);
WriteComm(0xD50D);WriteData(0xCF);
WriteComm(0xD50E);WriteData(0x01);
WriteComm(0xD50F);WriteData(0x17);
WriteComm(0xD510);WriteData(0x01);
WriteComm(0xD511);WriteData(0x4B);
WriteComm(0xD512);WriteData(0x01);
WriteComm(0xD513);WriteData(0x93);
WriteComm(0xD514);WriteData(0x01);
WriteComm(0xD515);WriteData(0xC6);
WriteComm(0xD516);WriteData(0x02);
WriteComm(0xD517);WriteData(0x10);
WriteComm(0xD518);WriteData(0x02);
WriteComm(0xD519);WriteData(0x48);
WriteComm(0xD51A);WriteData(0x02);
WriteComm(0xD51B);WriteData(0x49);
WriteComm(0xD51C);WriteData(0x02);
WriteComm(0xD51D);WriteData(0x79);
WriteComm(0xD51E);WriteData(0x02);
WriteComm(0xD51F);WriteData(0xAA);
WriteComm(0xD520);WriteData(0x02);
WriteComm(0xD521);WriteData(0xC4);
WriteComm(0xD522);WriteData(0x02);
WriteComm(0xD523);WriteData(0xE4);
WriteComm(0xD524);WriteData(0x02);
WriteComm(0xD525);WriteData(0xF8);
WriteComm(0xD526);WriteData(0x03);
WriteComm(0xD527);WriteData(0x10);
WriteComm(0xD528);WriteData(0x03);
WriteComm(0xD529);WriteData(0x20);
WriteComm(0xD52A);WriteData(0x03);
WriteComm(0xD52B);WriteData(0x36);
WriteComm(0xD52C);WriteData(0x03);
WriteComm(0xD52D);WriteData(0x46);
WriteComm(0xD52E);WriteData(0x03);
WriteComm(0xD52F);WriteData(0x5C);
WriteComm(0xD530);WriteData(0x03);
WriteComm(0xD531);WriteData(0x8B);
WriteComm(0xD532);WriteData(0x03);
WriteComm(0xD533);WriteData(0xFF);

WriteComm(0xD600);WriteData(0x00);
WriteComm(0xD601);WriteData(0x00);
WriteComm(0xD602);WriteData(0x00);
WriteComm(0xD603);WriteData(0x01);
WriteComm(0xD604);WriteData(0x00);
WriteComm(0xD605);WriteData(0x18);
WriteComm(0xD606);WriteData(0x00);
WriteComm(0xD607);WriteData(0x3B);
WriteComm(0xD608);WriteData(0x00);
WriteComm(0xD609);WriteData(0x60);
WriteComm(0xD60A);WriteData(0x00);
WriteComm(0xD60B);WriteData(0x9E);
WriteComm(0xD60C);WriteData(0x00);
WriteComm(0xD60D);WriteData(0xCF);
WriteComm(0xD60E);WriteData(0x01);
WriteComm(0xD60F);WriteData(0x17);
WriteComm(0xD610);WriteData(0x01);
WriteComm(0xD611);WriteData(0x4B);
WriteComm(0xD612);WriteData(0x01);
WriteComm(0xD613);WriteData(0x93);
WriteComm(0xD614);WriteData(0x01);
WriteComm(0xD615);WriteData(0xC6);
WriteComm(0xD616);WriteData(0x02);
WriteComm(0xD617);WriteData(0x10);
WriteComm(0xD618);WriteData(0x02);
WriteComm(0xD619);WriteData(0x48);
WriteComm(0xD61A);WriteData(0x02);
WriteComm(0xD61B);WriteData(0x49);
WriteComm(0xD61C);WriteData(0x02);
WriteComm(0xD61D);WriteData(0x79);
WriteComm(0xD61E);WriteData(0x02);
WriteComm(0xD61F);WriteData(0xAA);
WriteComm(0xD620);WriteData(0x02);
WriteComm(0xD621);WriteData(0xC4);
WriteComm(0xD622);WriteData(0x02);
WriteComm(0xD623);WriteData(0xE4);
WriteComm(0xD624);WriteData(0x02);
WriteComm(0xD625);WriteData(0xF8);
WriteComm(0xD626);WriteData(0x03);
WriteComm(0xD627);WriteData(0x10);
WriteComm(0xD628);WriteData(0x03);
WriteComm(0xD629);WriteData(0x20);
WriteComm(0xD62A);WriteData(0x03);
WriteComm(0xD62B);WriteData(0x36);
WriteComm(0xD62C);WriteData(0x03);
WriteComm(0xD62D);WriteData(0x46);
WriteComm(0xD62E);WriteData(0x03);
WriteComm(0xD62F);WriteData(0x5C);
WriteComm(0xD630);WriteData(0x03);
WriteComm(0xD631);WriteData(0x8B);
WriteComm(0xD632);WriteData(0x03);
WriteComm(0xD633);WriteData(0xFF);

//LV2 Page 0 enable
WriteComm(0xF000);WriteData(0x55);
WriteComm(0xF001);WriteData(0xAA);
WriteComm(0xF002);WriteData(0x52);
WriteComm(0xF003);WriteData(0x08);
WriteComm(0xF004);WriteData(0x00);

//480x800
WriteComm(0xB500);WriteData(0x50);

//Display control
WriteComm(0xB100);WriteData(0xFC);
WriteComm(0xB101);WriteData(0x00);

WriteComm(0xB400);WriteData(0x10);

//Source hold time
WriteComm(0xB600);WriteData(0x01);

//Gate EQ control
WriteComm(0xB700);WriteData(0x70);
WriteComm(0xB701);WriteData(0x70);

//Source EQ control (Mode 2)
WriteComm(0xB800);WriteData(0x01);
WriteComm(0xB801);WriteData(0x04);
WriteComm(0xB802);WriteData(0x04);
WriteComm(0xB803);WriteData(0x04);

//Inversion mode (2-dot)
WriteComm(0xBC00);WriteData(0x02);
WriteComm(0xBC01);WriteData(0x02);
WriteComm(0xBC02);WriteData(0x02);

//Frame rate
WriteComm(0xBD00);WriteData(0x01);
WriteComm(0xBD01);WriteData(0x10);
WriteComm(0xBD02);WriteData(0x0A);
WriteComm(0xBD03);WriteData(0x07);
WriteComm(0xBD04);WriteData(0x00);

WriteComm(0xBE00);WriteData(0x01);
WriteComm(0xBE01);WriteData(0x10);
WriteComm(0xBE02);WriteData(0x0A);
WriteComm(0xBE03);WriteData(0x07);
WriteComm(0xBE04);WriteData(0x00);

WriteComm(0xBF00);WriteData(0x01);
WriteComm(0xBF01);WriteData(0x10);
WriteComm(0xBF02);WriteData(0x0A);
WriteComm(0xBF03);WriteData(0x07);
WriteComm(0xBF04);WriteData(0x00);


//Timing control 4H w/4-Delay
// WriteComm(0xC900);WriteData(0xD0);
// WriteComm(0xC901);WriteData(0x82);
// WriteComm(0xC902);WriteData(0x50);
// WriteComm(0xC903);WriteData(0x50);
// WriteComm(0xC904);WriteData(0x50);
		
WriteComm(0xC900);WriteData(0xC0);	//3H:0x50;4H:0xD0	 //D
WriteComm(0xC901);WriteData(0x01);  //HSD:05;Nova:02
						  
WriteComm(0x3600);WriteData(0x00); //C0  //µ¹װֻŜ¸ņ½̨µ¹ɨ
#ifdef  __LCM_TE_ON__
WriteComm(0x3500);WriteData(0x00);
#else
WriteComm(0x3400);
#endif
WriteComm(0x4400);WriteData(0x00);
WriteComm(0x4401);WriteData(0x50);

WriteComm(0x3A00);WriteData(0x66);

WriteComm(0xF000);WriteData(0x55);
WriteComm(0xF001);WriteData(0xAA);
WriteComm(0xF002);WriteData(0x52);
WriteComm(0xF003);WriteData(0x08);
WriteComm(0xF004);WriteData(0x02);

WriteComm(0xF600);WriteData(0x60);

WriteComm(0x1100);
//DELAY 120
    MDELAY(200);
//Display On
WriteComm(0x2900);

    send_ctrl_cmd(0x2C00);
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

    params->dbi.port                    = 0;  //DBI port must be 0 or 1 on mt6575, should not be 2
    params->dbi.data_width              = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dbi.data_format.trans_seq   = LCM_DBI_TRANS_SEQ_MSB_FIRST;
    params->dbi.data_format.padding     = LCM_DBI_PADDING_ON_LSB;
    params->dbi.data_format.format      = LCM_DBI_FORMAT_RGB666;
    params->dbi.data_format.width       = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.cpu_write_bits          = LCM_DBI_CPU_WRITE_32_BITS;
    params->dbi.io_driving_current      = LCM_DRIVING_CURRENT_6575_8MA;

    params->dbi.parallel.write_setup    = 0;
    params->dbi.parallel.write_hold     = 0;
    params->dbi.parallel.write_wait     = 2;
    params->dbi.parallel.read_setup     = 5;
    params->dbi.parallel.read_hold      = 0;
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
	MDELAY(100);

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
    send_ctrl_cmd(0xDB00);
    id = read_data_cmd()<<8;
    send_ctrl_cmd(0xDC00);
    id |= read_data_cmd();


    Lcd_Log("MYCAT Read RM68180 id  = %x\n",  id);


    return (LCM_ID == id&&get_lcd_id()==1)?1:0; 
    //return (get_lcd_id()==1)?1:0;
}

LCM_DRIVER rm68180_feier_FTM397137A_CTC_mcu18_wvga_lcm_drv =
    {
        .name			= "rm68180_feier_FTM397137A_CTC_mcu18_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id     = lcm_compare_id
    };
