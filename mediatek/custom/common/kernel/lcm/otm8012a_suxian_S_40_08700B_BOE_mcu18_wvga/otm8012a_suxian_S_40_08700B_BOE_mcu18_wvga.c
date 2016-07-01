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

//#define   __LCM_TE_ON__
//#define   __LCM_WINDOW_CFG__
//static unsigned char  temk=0;
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
static int get_lcd_id(void);
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



#define send_cmd	send_ctrl_cmd
#define send_data 	send_data_cmd
#define DelayMS MDELAY
#define Delayms MDELAY



static void init_lcm_registers(void)
{
    int i;

#if defined(BUILD_LK)
        printf("mycat LCD otm8012a_suxian_S_40_08700B_BOE_mcu18_wvga \n");
#endif

	//Send_data send_cmd = self->info.mcu->ops->send_cmd;
	//Send_data send_data = self->info.mcu->ops->send_data;

	/* reset the lcd */
	//self->ops->lcd_reset(self);
	//mdelay(50);

send_cmd(0xFF00); send_data(0x80);
send_cmd(0xFF01); send_data(0x12);
send_cmd(0xFF02); send_data(0x01);

send_cmd(0xFF80); send_data(0x80);
send_cmd(0xFF81); send_data(0x12);

send_cmd(0xFF03); send_data(0x01);

send_cmd(0xC0B4); send_data(0x55);

send_cmd(0xD800); send_data(0x70);
send_cmd(0xD801); send_data(0x70);

send_cmd(0xD900); send_data(0x32);//0x34

send_cmd(0xC181); send_data(0x56);

send_cmd(0xC084); send_data(0x08);

send_cmd(0xC1A1); send_data(0x08);

send_cmd(0xC5B1); send_data(0x28);



send_cmd(0xC594); send_data(0x33);
send_cmd(0xC595); send_data(0x33);

send_cmd(0xC580); send_data(0x50);

send_cmd(0xC481); send_data(0x04);

send_cmd(0xC596); send_data(0x46);

send_cmd(0xF5BA); send_data(0x01);
send_cmd(0xF5BB); send_data(0x11);

send_cmd(0xC590); send_data(0x02);

send_cmd(0xC0B4); send_data(0x55);

send_cmd(0xC482); send_data(0xF2);

send_cmd(0xF5C2); send_data(0x00);

send_cmd(0xC582); send_data(0xF0);

send_cmd(0xC0A3); send_data(0x05);
send_cmd(0xC0A4); send_data(0x15);

send_cmd(0xCFC7); send_data(0x80);

send_cmd(0xCFC9); send_data(0x0A);
send_cmd(0xCFCA); send_data(0x06);

send_cmd(0xC48B); send_data(0x24);

send_cmd(0xB3A6); send_data(0x20);
send_cmd(0xB3A7); send_data(0x01);



send_cmd(0xC492); send_data(0x00);
send_cmd(0xB3A1); send_data(0x00);

send_cmd(0xCE80); send_data(0x85);
send_cmd(0xCE81); send_data(0x01);
send_cmd(0xCE82); send_data(0x00);
send_cmd(0xCE83); send_data(0x84);
send_cmd(0xCE84); send_data(0x01);
send_cmd(0xCE85); send_data(0x00);

send_cmd(0xCEA0); send_data(0x18);
send_cmd(0xCEA1); send_data(0x04);
send_cmd(0xCEA2); send_data(0x03);
send_cmd(0xCEA3); send_data(0x39);
send_cmd(0xCEA4); send_data(0x00);
send_cmd(0xCEA5); send_data(0x00);
send_cmd(0xCEA6); send_data(0x00);
send_cmd(0xCEA7); send_data(0x18);
send_cmd(0xCEA8); send_data(0x03);
send_cmd(0xCEA9); send_data(0x03);
send_cmd(0xCEAA); send_data(0x3A);
send_cmd(0xCEAB); send_data(0x00);
send_cmd(0xCEAC); send_data(0x00);
send_cmd(0xCEAD); send_data(0x00);

send_cmd(0xCEB0); send_data(0x18);
send_cmd(0xCEB1); send_data(0x02);
send_cmd(0xCEB2); send_data(0x03);
send_cmd(0xCEB3); send_data(0x3B);
send_cmd(0xCEB4); send_data(0x00);
send_cmd(0xCEB5); send_data(0x00);
send_cmd(0xCEB6); send_data(0x00);
send_cmd(0xCEB7); send_data(0x18);
send_cmd(0xCEB8); send_data(0x01);
send_cmd(0xCEB9); send_data(0x03);
send_cmd(0xCEBA); send_data(0x3C);
send_cmd(0xCEBB); send_data(0x00);
send_cmd(0xCEBC); send_data(0x00);
send_cmd(0xCEBD); send_data(0x00);

send_cmd(0xCFC0); send_data(0x01);
send_cmd(0xCFC1); send_data(0x01);
send_cmd(0xCFC2); send_data(0x20);
send_cmd(0xCFC3); send_data(0x20);
send_cmd(0xCFC4); send_data(0x00);
send_cmd(0xCFC5); send_data(0x00);
send_cmd(0xCFC6); send_data(0x01);
send_cmd(0xCFC7); send_data(0x00);
send_cmd(0xCFC8); send_data(0x00);
send_cmd(0xCFC9); send_data(0x00);

send_cmd(0xCFD0); send_data(0x00);

send_cmd(0xCB80); send_data(0x00);
send_cmd(0xCB81); send_data(0x00);
send_cmd(0xCB82); send_data(0x00);
send_cmd(0xCB83); send_data(0x00);
send_cmd(0xCB84); send_data(0x00);
send_cmd(0xCB85); send_data(0x00);
send_cmd(0xCB86); send_data(0x00);
send_cmd(0xCB87); send_data(0x00);
send_cmd(0xCB88); send_data(0x00);
send_cmd(0xCB89); send_data(0x00);

send_cmd(0xCB90); send_data(0x00);
send_cmd(0xCB91); send_data(0x00);
send_cmd(0xCB92); send_data(0x00);
send_cmd(0xCB93); send_data(0x00);
send_cmd(0xCB94); send_data(0x00);
send_cmd(0xCB95); send_data(0x00);
send_cmd(0xCB96); send_data(0x00);
send_cmd(0xCB97); send_data(0x00);
send_cmd(0xCB98); send_data(0x00);
send_cmd(0xCB99); send_data(0x00);
send_cmd(0xCB9A); send_data(0x00);
send_cmd(0xCB9B); send_data(0x00);
send_cmd(0xCB9C); send_data(0x00);
send_cmd(0xCB9D); send_data(0x00);
send_cmd(0xCB9E); send_data(0x00);

send_cmd(0xCBA0); send_data(0x00);
send_cmd(0xCBA1); send_data(0x00);
send_cmd(0xCBA2); send_data(0x00);
send_cmd(0xCBA3); send_data(0x00);
send_cmd(0xCBA4); send_data(0x00);
send_cmd(0xCBA5); send_data(0x00);
send_cmd(0xCBA6); send_data(0x00);
send_cmd(0xCBA7); send_data(0x00);
send_cmd(0xCBA8); send_data(0x00);
send_cmd(0xCBA9); send_data(0x00);
send_cmd(0xCBAA); send_data(0x00);
send_cmd(0xCBAB); send_data(0x00);
send_cmd(0xCBAC); send_data(0x00);
send_cmd(0xCBAD); send_data(0x00);
send_cmd(0xCBAE); send_data(0x00);

send_cmd(0xCBB0); send_data(0x00);
send_cmd(0xCBB1); send_data(0x00);
send_cmd(0xCBB2); send_data(0x00);
send_cmd(0xCBB3); send_data(0x00);
send_cmd(0xCBB4); send_data(0x00);
send_cmd(0xCBB5); send_data(0x00);
send_cmd(0xCBB6); send_data(0x00);
send_cmd(0xCBB7); send_data(0x00);
send_cmd(0xCBB8); send_data(0x00);
send_cmd(0xCBB9); send_data(0x00);

send_cmd(0xCBC0); send_data(0x00);
send_cmd(0xCBC1); send_data(0x04);
send_cmd(0xCBC2); send_data(0x04);
send_cmd(0xCBC3); send_data(0x04);
send_cmd(0xCBC4); send_data(0x04);
send_cmd(0xCBC5); send_data(0x04);
send_cmd(0xCBC6); send_data(0x00);
send_cmd(0xCBC7); send_data(0x00);
send_cmd(0xCBC8); send_data(0x00);
send_cmd(0xCBC9); send_data(0x00);
send_cmd(0xCBCA); send_data(0x00);
send_cmd(0xCBCB); send_data(0x00);
send_cmd(0xCBCC); send_data(0x00);
send_cmd(0xCBCD); send_data(0x00);
send_cmd(0xCBCE); send_data(0x00);

send_cmd(0xCBD0); send_data(0x00);
send_cmd(0xCBD1); send_data(0x00);
send_cmd(0xCBD2); send_data(0x00);
send_cmd(0xCBD3); send_data(0x00);
send_cmd(0xCBD4); send_data(0x00);
send_cmd(0xCBD5); send_data(0x00);
send_cmd(0xCBD6); send_data(0x00);
send_cmd(0xCBD7); send_data(0x00);
send_cmd(0xCBD8); send_data(0x00);
send_cmd(0xCBD9); send_data(0x00);
send_cmd(0xCBDA); send_data(0x00);
send_cmd(0xCBDB); send_data(0x00);
send_cmd(0xCBDC); send_data(0x00);
send_cmd(0xCBDD); send_data(0x00);
send_cmd(0xCBDE); send_data(0x00);

send_cmd(0xCBE0); send_data(0x04);
send_cmd(0xCBE1); send_data(0x04);
send_cmd(0xCBE2); send_data(0x04);
send_cmd(0xCBE3); send_data(0x04);
send_cmd(0xCBE4); send_data(0x04);
send_cmd(0xCBE5); send_data(0x00);
send_cmd(0xCBE6); send_data(0x00);
send_cmd(0xCBE7); send_data(0x00);
send_cmd(0xCBE8); send_data(0x00);
send_cmd(0xCBE9); send_data(0x00);

send_cmd(0xCBF0); send_data(0xFF);
send_cmd(0xCBF1); send_data(0xFF);
send_cmd(0xCBF2); send_data(0xFF);
send_cmd(0xCBF3); send_data(0xFF);
send_cmd(0xCBF4); send_data(0xFF);
send_cmd(0xCBF5); send_data(0xFF);
send_cmd(0xCBF6); send_data(0xFF);
send_cmd(0xCBF7); send_data(0xFF);
send_cmd(0xCBF8); send_data(0xFF);
send_cmd(0xCBF9); send_data(0xFF);

send_cmd(0xCC80); send_data(0x00);
send_cmd(0xCC81); send_data(0x26);
send_cmd(0xCC82); send_data(0x09);
send_cmd(0xCC83); send_data(0x0B);
send_cmd(0xCC84); send_data(0x01);
send_cmd(0xCC85); send_data(0x25);
send_cmd(0xCC86); send_data(0x00);
send_cmd(0xCC87); send_data(0x00);
send_cmd(0xCC88); send_data(0x00);
send_cmd(0xCC89); send_data(0x00);

send_cmd(0xCC90); send_data(0x00);
send_cmd(0xCC91); send_data(0x00);
send_cmd(0xCC92); send_data(0x00);
send_cmd(0xCC93); send_data(0x00);
send_cmd(0xCC94); send_data(0x00);
send_cmd(0xCC95); send_data(0x00);
send_cmd(0xCC96); send_data(0x00);
send_cmd(0xCC97); send_data(0x00);
send_cmd(0xCC98); send_data(0x00);
send_cmd(0xCC99); send_data(0x00);
send_cmd(0xCC9A); send_data(0x00);
send_cmd(0xCC9B); send_data(0x00);
send_cmd(0xCC9C); send_data(0x00);
send_cmd(0xCC9D); send_data(0x00);
send_cmd(0xCC9E); send_data(0x00);

send_cmd(0xCCA0); send_data(0x00);
send_cmd(0xCCA1); send_data(0x00);
send_cmd(0xCCA2); send_data(0x00);
send_cmd(0xCCA3); send_data(0x00);
send_cmd(0xCCA4); send_data(0x00);
send_cmd(0xCCA5); send_data(0x25);
send_cmd(0xCCA6); send_data(0x02);
send_cmd(0xCCA7); send_data(0x0C);
send_cmd(0xCCA8); send_data(0x0A);
send_cmd(0xCCA9); send_data(0x26);
send_cmd(0xCCAA); send_data(0x00);
send_cmd(0xCCAB); send_data(0x00);
send_cmd(0xCCAC); send_data(0x00);
send_cmd(0xCCAD); send_data(0x00);
send_cmd(0xCCAE); send_data(0x00);

send_cmd(0xCCB0); send_data(0x00);
send_cmd(0xCCB1); send_data(0x25);
send_cmd(0xCCB2); send_data(0x0C);
send_cmd(0xCCB3); send_data(0x0A);
send_cmd(0xCCB4); send_data(0x02);
send_cmd(0xCCB5); send_data(0x26);
send_cmd(0xCCB6); send_data(0x00);
send_cmd(0xCCB7); send_data(0x00);
send_cmd(0xCCB8); send_data(0x00);
send_cmd(0xCCB9); send_data(0x00);

send_cmd(0xCCC0); send_data(0x00);
send_cmd(0xCCC1); send_data(0x00);
send_cmd(0xCCC2); send_data(0x00);
send_cmd(0xCCC3); send_data(0x00);
send_cmd(0xCCC4); send_data(0x00);
send_cmd(0xCCC5); send_data(0x00);
send_cmd(0xCCC6); send_data(0x00);
send_cmd(0xCCC7); send_data(0x00);
send_cmd(0xCCC8); send_data(0x00);
send_cmd(0xCCC9); send_data(0x00);
send_cmd(0xCCCA); send_data(0x00);
send_cmd(0xCCCB); send_data(0x00);
send_cmd(0xCCCC); send_data(0x00);
send_cmd(0xCCCD); send_data(0x00);
send_cmd(0xCCCE); send_data(0x00);

send_cmd(0xCCD0); send_data(0x00);
send_cmd(0xCCD1); send_data(0x00);
send_cmd(0xCCD2); send_data(0x00);
send_cmd(0xCCD3); send_data(0x00);
send_cmd(0xCCD4); send_data(0x00);
send_cmd(0xCCD5); send_data(0x25);
send_cmd(0xCCD6); send_data(0x0B);
send_cmd(0xCCD7); send_data(0x09);
send_cmd(0xCCD8); send_data(0x01);
send_cmd(0xCCD9); send_data(0x26);
send_cmd(0xCCDA); send_data(0x00);
send_cmd(0xCCDB); send_data(0x00);
send_cmd(0xCCDC); send_data(0x00);
send_cmd(0xCCDD); send_data(0x00);
send_cmd(0xCCDE); send_data(0x00);


send_cmd(0x3A00); send_data(0x66);

send_cmd(0x1100);
Delayms(150);

send_cmd(0xC492); send_data(0x08);
Delayms(1);
send_cmd(0xC492); send_data(0x00);
Delayms(1);
send_cmd(0xC492); send_data(0x08);
Delayms(1);
send_cmd(0xC492); send_data(0x00);
Delayms(1);
send_cmd(0xC492); send_data(0x08);
Delayms(1);
send_cmd(0xC492); send_data(0x00);
Delayms(1);
send_cmd(0xC492); send_data(0x08);
Delayms(1);
send_cmd(0xC492); send_data(0x00);
Delayms(1);
send_cmd(0xC492); send_data(0x08);
Delayms(1);
send_cmd(0xC492); send_data(0x00);
Delayms(1);
send_cmd(0xC492); send_data(0x08);
Delayms(1);
send_cmd(0xC492); send_data(0x00);
Delayms(1);
send_cmd(0xC492); send_data(0x08);
Delayms(1);
send_cmd(0xC492); send_data(0x00);
Delayms(1);
send_cmd(0xC492); send_data(0x08);
Delayms(1);
send_cmd(0xC492); send_data(0x00);
Delayms(1);
send_cmd(0xC492); send_data(0x08);
Delayms(1);
send_cmd(0xC492); send_data(0x00);
Delayms(1);
send_cmd(0xC492); send_data(0x08);

/******************8
#if 1
send_cmd(0x3600);  send_data(0xD0);// Display Direction 180
send_cmd(0x3500);  send_data(0x00);// TE( Fmark ) Signal On
send_cmd(0x4400);  send_data(0x01);
send_cmd(0x4401);  send_data(0xFF);// TE( Fmark ) Signal Output Position
#else
send_cmd(0x3600);  send_data(0x00);// Display Direction 0
send_cmd(0x3500);  send_data(0x00);// TE( Fmark ) Signal On
send_cmd(0x4400);  send_data(0x01);
send_cmd(0x4401);  send_data(0x22);// TE( Fmark ) Signal Output Position
#endif
***********************/
send_cmd(0x3600);  send_data(0x00);//c0
send_cmd(0x3500);  send_data(0x00);// TE( Fmark ) Signal On
send_cmd(0x4400);  send_data(0x01);
send_cmd(0x4401);  send_data(0x22);// TE( Fmark ) Signal Output Position

#ifdef LCD_BACKLIGHT_CONTROL_MODE
send_cmd(0x5100);  send_data(0xFF);// Backlight Level Control
send_cmd(0x5300);  send_data(0x2C);// Backlight On
send_cmd(0x5500);  send_data(0x00);// CABC Function Off
#endif

send_cmd(0x2A00); send_data(0x00);
send_cmd(0x2A01); send_data(0x00);
send_cmd(0x2A02); send_data(0x01);
send_cmd(0x2A03); send_data(0xDF);
send_cmd(0x2B00); send_data(0x00);
send_cmd(0x2B01); send_data(0x00);
send_cmd(0x2B02); send_data(0x03);  
send_cmd(0x2B03); send_data(0x1F);

send_cmd(0xFF00); send_data(0xFF);
send_cmd(0xFF01); send_data(0xFF);
send_cmd(0xFF02); send_data(0xFF);

send_cmd(0x2900);
Delayms(20);

//sw_clear_panel(self,0x0);
sw_clear_panel(0x0);
Delayms(20);

#if 0
send_cmd(0x2C00);

    if(1){
	    //int i;
	    for (i=0; i<480*800; i++)
		    send_data(0xf100);
        Delayms(200);

	    for (i=0; i<480*800; i++)
		    send_data(0x07E0);
        Delayms(200);

	    for (i=0; i<480*800; i++)
		    send_data(0x001F);
        Delayms(200);

    }
    while(0)
    {
	    int i;
	    for (i=0; i<480*800/2; i++)
	    {
		    send_data(0x55AA);
		    send_data(0xAA55);
		}
	}

#endif
	return 0;
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
//    params->dbi.te_mode                 = 0;
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

		//lcm_compare_id();

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
    MDELAY(10);
    send_ctrl_cmd(0x1000); // Sleep in
    MDELAY(120);	
}


static void lcm_resume(void)
{
#if 1
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
    //Lcd_Log("lcm_update otm8012a_suxian_S_40_08700B_mcu18_wvga\r\n");

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
    unsigned int id=0;
    unsigned int idA1=0,idA2=0,idA3=0,idA4=0,idA5=0;

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
    idA1 = read_data_cmd();
    Lcd_Log("mycat READ OTM8012A LCD ID%s, idA1 = 0x%x\n", __func__, idA1);
    idA2 = read_data_cmd();
    Lcd_Log("mycat READ OTM8012A LCD ID%s, idA2 = 0x%x\n", __func__, idA2);
    idA3 = read_data_cmd();
    Lcd_Log("mycat READ OTM8012A LCD ID%s, idA3 = 0x%x\n", __func__, idA3);
    idA4 = read_data_cmd();
    Lcd_Log("mycat READ OTM8012A LCD ID%s, idA4 = 0x%x\n", __func__, idA4);
    idA5 = read_data_cmd();
    Lcd_Log("mycat READ OTM8012A LCD ID%s, idA5 = 0x%x\n", __func__, idA5);
           
    id=((idA4 & 0xff) << 8) | idA5;
    Lcd_Log("mycat READ OTM8012A LCD ID%s, ID = 0x%x\n", __func__, id);
    return (LCM_ID ==id)?1:0;

}

//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER otm8012a_suxian_S_40_08700B_BOE_mcu18_wvga_lcm_drv =
    {
        .name		= "otm8012a_suxian_S_40_08700B_BOE_mcu18_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id    = lcm_compare_id,
    };


