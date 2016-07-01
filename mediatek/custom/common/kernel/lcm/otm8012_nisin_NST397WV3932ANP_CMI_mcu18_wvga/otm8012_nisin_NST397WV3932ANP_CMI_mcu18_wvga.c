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
#define LCM_ID       (0x8012)
#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN

#define   __LCM_TE_ON__
#define   __LCM_WINDOW_CFG__

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
static int get_lcd_id(void);
//static unsigned int lcm_compare_id(void);
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
#ifdef  __LCM_WINDOW_CFG__
    send_ctrl_cmd(0x2A00);
    send_data_cmd(h_X_start);
    send_ctrl_cmd(0x2A01);
    send_data_cmd(l_X_start);
    send_ctrl_cmd(0x2A02);
    send_data_cmd(h_X_end);
    send_ctrl_cmd(0x2A03);
    send_data_cmd(l_X_end);

    send_ctrl_cmd(0x2B00);
    send_data_cmd(h_Y_start);
    send_ctrl_cmd(0x2B01);
    send_data_cmd(l_Y_start);
    send_ctrl_cmd(0x2B02);
    send_data_cmd(h_Y_end);
    send_ctrl_cmd(0x2B03);
    send_data_cmd(l_Y_end);
#else
    send_ctrl_cmd(0x2A00);
    send_data_cmd(h_X_start);
    send_data_cmd(l_X_start);
    send_data_cmd(h_X_end);
    send_data_cmd(l_X_end);

    send_ctrl_cmd(0x2B00);
    send_data_cmd(h_Y_start);
    send_data_cmd(l_Y_start);
    send_data_cmd(h_Y_end);
    send_data_cmd(l_Y_end);
#endif
//color=0x8410 ;
// color=0xF800 ;
//color=0xFFFF ;
    send_ctrl_cmd(0x2900);

    send_ctrl_cmd(0x2C00);
    for (y = y0; y <= y1; ++ y) {
        for (x = x0; x <= x1; ++ x) {
            lcm_util.send_data(color);
        }
    }
}

#define Delayms  MDELAY
#define W_COM	 send_ctrl_cmd
#define W_DATA   send_data_cmd

static void init_lcm_registers(void)
{
    int i;

#if defined(BUILD_LK)
        printf("mycat LCD otm8012_nisin_NST397WV3932ANP_mcu_wvga_lcm_drv \n");
#endif



W_COM(0xFF00); W_DATA(0x80);
W_COM(0xFF01); W_DATA(0x12);
W_COM(0xFF02); W_DATA(0x01);

W_COM(0xFF80); W_DATA(0x80);
W_COM(0xFF81); W_DATA(0x12);

W_COM(0xB0C6); W_DATA(0x03);

W_COM(0xB3A1); W_DATA(0x00);

W_COM(0xB3A6); W_DATA(0x20);
W_COM(0xB3A7); W_DATA(0x01);

W_COM(0xB3B0); W_DATA(0x18);

W_COM(0xC0A3); W_DATA(0x05);
W_COM(0xC0A4); W_DATA(0x15);

W_COM(0xC0B4); W_DATA(0x11);

W_COM(0xC481); W_DATA(0x04);
W_COM(0xC482); W_DATA(0xF2);

W_COM(0xC48B); W_DATA(0x24);

W_COM(0xC492); W_DATA(0x00);

W_COM(0xC580); W_DATA(0x50);

W_COM(0xC582); W_DATA(0xF0);

W_COM(0xC590); W_DATA(0x03);
W_COM(0xC591); W_DATA(0x76);
W_COM(0xC592); W_DATA(0x01);

W_COM(0xC594); W_DATA(0x44);
W_COM(0xC595); W_DATA(0x44);
W_COM(0xC596); W_DATA(0x46);

W_COM(0xF5BA); W_DATA(0x01);
W_COM(0xF5BB); W_DATA(0x11);

W_COM(0xF5C2); W_DATA(0x00);

W_COM(0xC090); W_DATA(0x00);
W_COM(0xC091); W_DATA(0x44);
W_COM(0xC092); W_DATA(0x00);
W_COM(0xC093); W_DATA(0x00);
W_COM(0xC094); W_DATA(0x00);
W_COM(0xC095); W_DATA(0x03);

W_COM(0xCE80); W_DATA(0x85);
W_COM(0xCE81); W_DATA(0x03);
W_COM(0xCE82); W_DATA(0x00);
W_COM(0xCE83); W_DATA(0x84);
W_COM(0xCE84); W_DATA(0x03);
W_COM(0xCE85); W_DATA(0x00);
W_COM(0xCE86); W_DATA(0x87);
W_COM(0xCE87); W_DATA(0x03);
W_COM(0xCE88); W_DATA(0x00);
W_COM(0xCE89); W_DATA(0x86);
W_COM(0xCE8A); W_DATA(0x03);
W_COM(0xCE8B); W_DATA(0x00);

W_COM(0xCE90); W_DATA(0x33);
W_COM(0xCE91); W_DATA(0x26);
W_COM(0xCE92); W_DATA(0x00);
W_COM(0xCE93); W_DATA(0x33);
W_COM(0xCE94); W_DATA(0x27);
W_COM(0xCE95); W_DATA(0x00);

W_COM(0xCEA0); W_DATA(0x38);
W_COM(0xCEA1); W_DATA(0x03);
W_COM(0xCEA2); W_DATA(0x03);
W_COM(0xCEA3); W_DATA(0x20);
W_COM(0xCEA4); W_DATA(0x00);
W_COM(0xCEA5); W_DATA(0x00);
W_COM(0xCEA6); W_DATA(0x00);
W_COM(0xCEA7); W_DATA(0x38);
W_COM(0xCEA8); W_DATA(0x02);
W_COM(0xCEA9); W_DATA(0x03);
W_COM(0xCEAA); W_DATA(0x21);
W_COM(0xCEAB); W_DATA(0x00);
W_COM(0xCEAC); W_DATA(0x00);
W_COM(0xCEAD); W_DATA(0x00);

W_COM(0xCEB0); W_DATA(0x38);
W_COM(0xCEB1); W_DATA(0x01);
W_COM(0xCEB2); W_DATA(0x03);
W_COM(0xCEB3); W_DATA(0x22);
W_COM(0xCEB4); W_DATA(0x00);
W_COM(0xCEB5); W_DATA(0x00);
W_COM(0xCEB6); W_DATA(0x00);
W_COM(0xCEB7); W_DATA(0x38);
W_COM(0xCEB8); W_DATA(0x00);
W_COM(0xCEB9); W_DATA(0x03);
W_COM(0xCEBA); W_DATA(0x23);
W_COM(0xCEBB); W_DATA(0x00);
W_COM(0xCEBC); W_DATA(0x00);
W_COM(0xCEBD); W_DATA(0x00);

W_COM(0xCEC0); W_DATA(0x30);
W_COM(0xCEC1); W_DATA(0x00);
W_COM(0xCEC2); W_DATA(0x03);
W_COM(0xCEC3); W_DATA(0x24);
W_COM(0xCEC4); W_DATA(0x00);
W_COM(0xCEC5); W_DATA(0x00);
W_COM(0xCEC6); W_DATA(0x00);
W_COM(0xCEC7); W_DATA(0x30);
W_COM(0xCEC8); W_DATA(0x01);
W_COM(0xCEC9); W_DATA(0x03);
W_COM(0xCECA); W_DATA(0x25);
W_COM(0xCECB); W_DATA(0x00);
W_COM(0xCECC); W_DATA(0x00);
W_COM(0xCECD); W_DATA(0x00);

W_COM(0xCED0); W_DATA(0x30);
W_COM(0xCED1); W_DATA(0x02);
W_COM(0xCED2); W_DATA(0x03);
W_COM(0xCED3); W_DATA(0x26);
W_COM(0xCED4); W_DATA(0x00);
W_COM(0xCED5); W_DATA(0x00);
W_COM(0xCED6); W_DATA(0x00);
W_COM(0xCED7); W_DATA(0x30);
W_COM(0xCED8); W_DATA(0x03);
W_COM(0xCED9); W_DATA(0x03);
W_COM(0xCEDA); W_DATA(0x27);
W_COM(0xCEDB); W_DATA(0x00);
W_COM(0xCEDC); W_DATA(0x00);
W_COM(0xCEDD); W_DATA(0x00);

W_COM(0xCFC7); W_DATA(0x00);

W_COM(0xCFC9); W_DATA(0x0A);
W_COM(0xCFCA); W_DATA(0x06);

W_COM(0xCBC0); W_DATA(0x00);
W_COM(0xCBC1); W_DATA(0x00);
W_COM(0xCBC2); W_DATA(0x00);
W_COM(0xCBC3); W_DATA(0x00);
W_COM(0xCBC4); W_DATA(0x54);
W_COM(0xCBC5); W_DATA(0x54);
W_COM(0xCBC6); W_DATA(0x54);
W_COM(0xCBC7); W_DATA(0x54);
W_COM(0xCBC8); W_DATA(0x00);
W_COM(0xCBC9); W_DATA(0x54);
W_COM(0xCBCA); W_DATA(0x00);
W_COM(0xCBCB); W_DATA(0x54);
W_COM(0xCBCC); W_DATA(0x00);
W_COM(0xCBCD); W_DATA(0x00);
W_COM(0xCBCE); W_DATA(0x00);

W_COM(0xCBD0); W_DATA(0x00);
W_COM(0xCBD1); W_DATA(0x00);
W_COM(0xCBD2); W_DATA(0x00);
W_COM(0xCBD3); W_DATA(0x00);
W_COM(0xCBD4); W_DATA(0x00);
W_COM(0xCBD5); W_DATA(0x00);
W_COM(0xCBD6); W_DATA(0x00);
W_COM(0xCBD7); W_DATA(0x00);
W_COM(0xCBD8); W_DATA(0x00);
W_COM(0xCBD9); W_DATA(0x54);
W_COM(0xCBDA); W_DATA(0x00);
W_COM(0xCBDB); W_DATA(0x54);
W_COM(0xCBDC); W_DATA(0x00);
W_COM(0xCBDD); W_DATA(0x54);
W_COM(0xCBDE); W_DATA(0x54);

W_COM(0xCBE0); W_DATA(0x54);
W_COM(0xCBE1); W_DATA(0x54);
W_COM(0xCBE2); W_DATA(0x00);
W_COM(0xCBE3); W_DATA(0x00);
W_COM(0xCBE4); W_DATA(0x00);
W_COM(0xCBE5); W_DATA(0x00);
W_COM(0xCBE6); W_DATA(0x00);
W_COM(0xCBE7); W_DATA(0x00);
W_COM(0xCBE8); W_DATA(0x00);

W_COM(0xCC80); W_DATA(0x00);
W_COM(0xCC81); W_DATA(0x00);
W_COM(0xCC82); W_DATA(0x00);
W_COM(0xCC83); W_DATA(0x00);
W_COM(0xCC84); W_DATA(0x0C);
W_COM(0xCC85); W_DATA(0x0A);
W_COM(0xCC86); W_DATA(0x10);
W_COM(0xCC87); W_DATA(0x0E);
W_COM(0xCC88); W_DATA(0x00);
W_COM(0xCC89); W_DATA(0x02);

W_COM(0xCC90); W_DATA(0x00);
W_COM(0xCC91); W_DATA(0x06);
W_COM(0xCC92); W_DATA(0x00);
W_COM(0xCC93); W_DATA(0x00);
W_COM(0xCC94); W_DATA(0x00);
W_COM(0xCC95); W_DATA(0x00);
W_COM(0xCC96); W_DATA(0x00);
W_COM(0xCC97); W_DATA(0x00);
W_COM(0xCC98); W_DATA(0x00);
W_COM(0xCC99); W_DATA(0x00);
W_COM(0xCC9A); W_DATA(0x00);
W_COM(0xCC9B); W_DATA(0x00);
W_COM(0xCC9C); W_DATA(0x00);
W_COM(0xCC9D); W_DATA(0x00);
W_COM(0xCC9E); W_DATA(0x05);

W_COM(0xCCA0); W_DATA(0x00);
W_COM(0xCCA1); W_DATA(0x01);
W_COM(0xCCA2); W_DATA(0x00);
W_COM(0xCCA3); W_DATA(0x0D);
W_COM(0xCCA4); W_DATA(0x0F);
W_COM(0xCCA5); W_DATA(0x09);
W_COM(0xCCA6); W_DATA(0x0B);
W_COM(0xCCA7); W_DATA(0x00);
W_COM(0xCCA8); W_DATA(0x00);
W_COM(0xCCA9); W_DATA(0x00);
W_COM(0xCCAA); W_DATA(0x00);
W_COM(0xCCAB); W_DATA(0x00);
W_COM(0xCCAC); W_DATA(0x00);
W_COM(0xCCAD); W_DATA(0x00);
W_COM(0xCCAE); W_DATA(0x00);

W_COM(0xCCB0); W_DATA(0x00);
W_COM(0xCCB1); W_DATA(0x00);
W_COM(0xCCB2); W_DATA(0x00);
W_COM(0xCCB3); W_DATA(0x00);
W_COM(0xCCB4); W_DATA(0x0D);
W_COM(0xCCB5); W_DATA(0x0F);
W_COM(0xCCB6); W_DATA(0x09);
W_COM(0xCCB7); W_DATA(0x0B);
W_COM(0xCCB8); W_DATA(0x00);
W_COM(0xCCB9); W_DATA(0x05);

W_COM(0xCCC0); W_DATA(0x00);
W_COM(0xCCC1); W_DATA(0x01);
W_COM(0xCCC2); W_DATA(0x00);
W_COM(0xCCC3); W_DATA(0x00);
W_COM(0xCCC4); W_DATA(0x00);
W_COM(0xCCC5); W_DATA(0x00);
W_COM(0xCCC6); W_DATA(0x00);
W_COM(0xCCC7); W_DATA(0x00);
W_COM(0xCCC8); W_DATA(0x00);
W_COM(0xCCC9); W_DATA(0x00);
W_COM(0xCCCA); W_DATA(0x00);
W_COM(0xCCCB); W_DATA(0x00);
W_COM(0xCCCC); W_DATA(0x00);
W_COM(0xCCCD); W_DATA(0x00);
W_COM(0xCCCE); W_DATA(0x02);

W_COM(0xCCD0); W_DATA(0x00);
W_COM(0xCCD1); W_DATA(0x06);
W_COM(0xCCD2); W_DATA(0x00);
W_COM(0xCCD3); W_DATA(0x0C);
W_COM(0xCCD4); W_DATA(0x0A);
W_COM(0xCCD5); W_DATA(0x10);
W_COM(0xCCD6); W_DATA(0x0E);
W_COM(0xCCD7); W_DATA(0x00);
W_COM(0xCCD8); W_DATA(0x00);
W_COM(0xCCD9); W_DATA(0x00);
W_COM(0xCCDA); W_DATA(0x00);
W_COM(0xCCDB); W_DATA(0x00);
W_COM(0xCCDC); W_DATA(0x00);
W_COM(0xCCDD); W_DATA(0x00);
W_COM(0xCCDE); W_DATA(0x00);

W_COM(0xD800); W_DATA(0x83);
W_COM(0xD801); W_DATA(0x83);

W_COM(0xD900); W_DATA(0x6E);

W_COM(0xE100); W_DATA(0x0C);
W_COM(0xE101); W_DATA(0x0F);
W_COM(0xE102); W_DATA(0x14);
W_COM(0xE103); W_DATA(0x1E);
W_COM(0xE104); W_DATA(0x32);
W_COM(0xE105); W_DATA(0x4A);
W_COM(0xE106); W_DATA(0x58);
W_COM(0xE107); W_DATA(0x8C);
W_COM(0xE108); W_DATA(0x7A);
W_COM(0xE109); W_DATA(0x8F);
W_COM(0xE10A); W_DATA(0x78);
W_COM(0xE10B); W_DATA(0x69);
W_COM(0xE10C); W_DATA(0x82);
W_COM(0xE10D); W_DATA(0x6F);
W_COM(0xE10E); W_DATA(0x75);
W_COM(0xE10F); W_DATA(0x6F);
W_COM(0xE110); W_DATA(0x68);
W_COM(0xE111); W_DATA(0x60);
W_COM(0xE112); W_DATA(0x56);
W_COM(0xE113); W_DATA(0x20);

W_COM(0xE200); W_DATA(0x0C);
W_COM(0xE201); W_DATA(0x0F);
W_COM(0xE202); W_DATA(0x14);
W_COM(0xE203); W_DATA(0x1E);
W_COM(0xE204); W_DATA(0x32);
W_COM(0xE205); W_DATA(0x4A);
W_COM(0xE206); W_DATA(0x58);
W_COM(0xE207); W_DATA(0x8C);
W_COM(0xE208); W_DATA(0x7A);
W_COM(0xE209); W_DATA(0x8F);
W_COM(0xE20A); W_DATA(0x78);
W_COM(0xE20B); W_DATA(0x69);
W_COM(0xE20C); W_DATA(0x82);
W_COM(0xE20D); W_DATA(0x6F);
W_COM(0xE20E); W_DATA(0x75);
W_COM(0xE20F); W_DATA(0x6F);
W_COM(0xE210); W_DATA(0x68);
W_COM(0xE211); W_DATA(0x60);
W_COM(0xE212); W_DATA(0x56);
W_COM(0xE213); W_DATA(0x20);

W_COM(0x3A00); W_DATA(0x66);

W_COM(0x1100);
Delayms(150);

W_COM(0xC492); W_DATA(0x08);
Delayms(1);
W_COM(0xC492); W_DATA(0x00);
Delayms(1);
W_COM(0xC492); W_DATA(0x08);
Delayms(1);
W_COM(0xC492); W_DATA(0x00);
Delayms(1);
W_COM(0xC492); W_DATA(0x08);
Delayms(1);
W_COM(0xC492); W_DATA(0x00);
Delayms(1);
W_COM(0xC492); W_DATA(0x08);
Delayms(1);
W_COM(0xC492); W_DATA(0x00);
Delayms(1);
W_COM(0xC492); W_DATA(0x08);
Delayms(1);
W_COM(0xC492); W_DATA(0x00);
Delayms(1);
W_COM(0xC492); W_DATA(0x08);
Delayms(1);
W_COM(0xC492); W_DATA(0x00);
Delayms(1);
W_COM(0xC492); W_DATA(0x08);
Delayms(1);
W_COM(0xC492); W_DATA(0x00);
Delayms(1);
W_COM(0xC492); W_DATA(0x08);
Delayms(1);
W_COM(0xC492); W_DATA(0x00);
Delayms(1);
W_COM(0xC492); W_DATA(0x08);
Delayms(1);
W_COM(0xC492); W_DATA(0x00);
Delayms(1);
W_COM(0xC492); W_DATA(0x08);

W_COM(0xFF00); W_DATA(0xFF);
W_COM(0xFF01); W_DATA(0xFF);
W_COM(0xFF02); W_DATA(0xFF);

W_COM(0x2900);
Delayms(20);

W_COM(0x2A00); W_DATA(0x00);
W_COM(0x2A01); W_DATA(0x00);
W_COM(0x2A02); W_DATA(0x01);
W_COM(0x2A03); W_DATA(0xDF);

W_COM(0x2B00); W_DATA(0x00);
W_COM(0x2B01); W_DATA(0x00);
W_COM(0x2B02); W_DATA(0x03);
W_COM(0x2B03); W_DATA(0x1F);

		send_ctrl_cmd(0x3500); send_data_cmd(0x0000);     //TE on
		send_ctrl_cmd(0x4400); send_data_cmd(0x0001);     //write TE
		send_ctrl_cmd(0x4401); send_data_cmd(0x0022);
		
		send_ctrl_cmd(0x1100);
		MDELAY(120);	        
		send_ctrl_cmd(0x2900);
		MDELAY(200);         
		send_ctrl_cmd(0x2C00);
		MDELAY(200);
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
    params->dbi.parallel.read_hold      = 5;
    params->dbi.parallel.read_latency   = 31;
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

		send_ctrl_cmd(0xA100);
		read_data_cmd();
		read_data_cmd();
		read_data_cmd();
		id = read_data_cmd();
		id = (id<<8) | read_data_cmd();
    Lcd_Log("mycat READ OTM8009 LCD ID2%s, id = 0x%x\n", __func__, id);

    return (LCM_ID == id)?1:0;

}


static void lcm_init(void)
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

    init_lcm_registers();

    
}


static void lcm_suspend(void)
{
    send_ctrl_cmd(0x2800); // Display off
    MDELAY(20);
    send_ctrl_cmd(0x1000); // Sleep in
    MDELAY(120);

	Lcd_Log("\n %s \n",__func__);


#ifdef BUILD_LK
    pmic_config_interface(0x0532,0,0x7,5);//add by libo for VGP2 POWER OFF
    pmic_config_interface(0x050C,0,0x1,15);
#else
    hwPowerDown(MT6323_POWER_LDO_VGP2,"LCM");
#endif
}


static void lcm_resume(void)
{
    Lcd_Log("\n %s \n",__func__);

#if 0
    send_ctrl_cmd(0x1100);
    MDELAY(120);
    send_ctrl_cmd(0x2900);
    MDELAY(50);
#else
	lcm_init();

#endif


}

static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
    unsigned short x0, y0, x1, y1;
    unsigned short h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;
    //Lcd_Log("lcm_update otm8009a_nisin_nst397wv3502anb_mcu_wvga\r\n");

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

#ifdef  __LCM_WINDOW_CFG__
    send_ctrl_cmd(0x2A00);
    send_data_cmd(h_X_start);
    send_ctrl_cmd(0x2A01);
    send_data_cmd(l_X_start);
    send_ctrl_cmd(0x2A02);
    send_data_cmd(h_X_end);
    send_ctrl_cmd(0x2A03);
    send_data_cmd(l_X_end);

    send_ctrl_cmd(0x2B00);
    send_data_cmd(h_Y_start);
    send_ctrl_cmd(0x2B01);
    send_data_cmd(l_Y_start);
    send_ctrl_cmd(0x2B02);
    send_data_cmd(h_Y_end);
    send_ctrl_cmd(0x2B03);
    send_data_cmd(l_Y_end);
#else

    send_ctrl_cmd(0x2A00);
    send_data_cmd(h_X_start);
    send_data_cmd(l_X_start);
    send_data_cmd(h_X_end);
    send_data_cmd(l_X_end);

    send_ctrl_cmd(0x2B00);
    send_data_cmd(h_Y_start);
    send_data_cmd(l_Y_start);
    send_data_cmd(h_Y_end);
    send_data_cmd(l_Y_end);

#endif

    send_ctrl_cmd(0x2900);

    send_ctrl_cmd(0x2C00);
}


//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER otm8012_nisin_NST397WV3932ANP_CMI_mcu18_wvga_lcm_drv =
    {
        .name		= "otm8012_nisin_NST397WV3932ANP_CMI_mcu18_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id    = lcm_compare_id,
    };


