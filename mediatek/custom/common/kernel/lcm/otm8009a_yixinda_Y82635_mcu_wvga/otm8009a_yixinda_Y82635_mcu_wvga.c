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
#define LCM_ID       (0x40)
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


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
static int get_lcd_id(void);
static unsigned int lcm_compare_id(void);
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
        printf("mycat LCD otm8009a_aizhuoertai_a4019n50q0_mcu_wvga \n");
#endif
#if 0
send_cmd(0xff00); send_data(0x80);
send_cmd(0xff01); send_data(0x09);
send_cmd(0xff02); send_data(0x01);
send_cmd(0xff80); send_data(0x80);
send_cmd(0xff81); send_data(0x09);
send_cmd(0xff03); send_data(0x01);
send_cmd(0xc5b1); send_data(0xA9);	
send_cmd(0xc591); send_data(0x0F);    
send_cmd(0xc0B4); send_data(0x50);
	
send_cmd(0xE100); send_data(0x00);
send_cmd(0xE101); send_data(0x09);
send_cmd(0xE102); send_data(0x0F);
send_cmd(0xE103); send_data(0x0E);
send_cmd(0xE104); send_data(0x07);
send_cmd(0xE105); send_data(0x10);
send_cmd(0xE106); send_data(0x0B);
send_cmd(0xE107); send_data(0x0A);
send_cmd(0xE108); send_data(0x04);
send_cmd(0xE109); send_data(0x07);
send_cmd(0xE10A); send_data(0x0B);
send_cmd(0xE10B); send_data(0x08);
send_cmd(0xE10C); send_data(0x0F);
send_cmd(0xE10D); send_data(0x10);
send_cmd(0xE10E); send_data(0x0A);
send_cmd(0xE10F); send_data(0x01);
	
send_cmd(0xE200); send_data(0x00);
send_cmd(0xE201); send_data(0x09);
send_cmd(0xE202); send_data(0x0F);
send_cmd(0xE203); send_data(0x0E);
send_cmd(0xE204); send_data(0x07);
send_cmd(0xE205); send_data(0x10);
send_cmd(0xE206); send_data(0x0B);
send_cmd(0xE207); send_data(0x0A);
send_cmd(0xE208); send_data(0x04);
send_cmd(0xE209); send_data(0x07);
send_cmd(0xE20A); send_data(0x0B);
send_cmd(0xE20B); send_data(0x08);
send_cmd(0xE20C); send_data(0x0F);
send_cmd(0xE20D); send_data(0x10);
send_cmd(0xE20E); send_data(0x0A);
send_cmd(0xE20F); send_data(0x01);
	
send_cmd(0xD900); send_data(0x4E);
send_cmd(0xc181); send_data(0x66);
send_cmd(0xc1a1); send_data(0x08);
send_cmd(0xc592); send_data(0x01);
send_cmd(0xc595); send_data(0x34);
send_cmd(0xd800); send_data(0x79);	
send_cmd(0xd801); send_data(0x79);	
send_cmd(0xc594); send_data(0x33);
send_cmd(0xc0a3); send_data(0x1B);    
send_cmd(0xc582); send_data(0x83);
send_cmd(0xc481); send_data(0x83);
send_cmd(0xc1a1); send_data(0x0E);
send_cmd(0xb3a6); send_data(0x20);
send_cmd(0xb3a7); send_data(0x01);
send_cmd(0xce80); send_data(0x85);
send_cmd(0xce81); send_data(0x01); 
send_cmd(0xce82); send_data(0x00);	
send_cmd(0xce83); send_data(0x84); 
send_cmd(0xce84); send_data(0x01); 
send_cmd(0xce85); send_data(0x00);
send_cmd(0xcea0); send_data(0x18); 
send_cmd(0xcea1); send_data(0x04); 
send_cmd(0xcea2); send_data(0x03); 
send_cmd(0xcea3); send_data(0x39); 
send_cmd(0xcea4); send_data(0x00); 
send_cmd(0xcea5); send_data(0x00); 
send_cmd(0xcea6); send_data(0x00);
send_cmd(0xcea7); send_data(0x18); 
send_cmd(0xcea8); send_data(0x03);
send_cmd(0xcea9); send_data(0x03); 
send_cmd(0xceaa); send_data(0x3a);
send_cmd(0xceab); send_data(0x00); 
send_cmd(0xceac); send_data(0x00); 
send_cmd(0xcead); send_data(0x00); 
send_cmd(0xceb0); send_data(0x18);
send_cmd(0xceb1); send_data(0x02); 
send_cmd(0xceb2); send_data(0x03); 
send_cmd(0xceb3); send_data(0x3b); 
send_cmd(0xceb4); send_data(0x00); 
send_cmd(0xceb5); send_data(0x00); 
send_cmd(0xceb6); send_data(0x00);
send_cmd(0xceb7); send_data(0x18);
send_cmd(0xceb8); send_data(0x01); 
send_cmd(0xceb9); send_data(0x03); 
send_cmd(0xceba); send_data(0x3c); 
send_cmd(0xcebb); send_data(0x00); 
send_cmd(0xcebc); send_data(0x00); 
send_cmd(0xcebd); send_data(0x00);
send_cmd(0xcfc0); send_data(0x01); 
send_cmd(0xcfc1); send_data(0x01); 
send_cmd(0xcfc2); send_data(0x20); 
send_cmd(0xcfc3); send_data(0x20); 
send_cmd(0xcfc4); send_data(0x00); 
send_cmd(0xcfc5); send_data(0x00); 
send_cmd(0xcfc6); send_data(0x01); 
send_cmd(0xcfc7); send_data(0x00);    
send_cmd(0xcfc8); send_data(0x00); 
send_cmd(0xcfc9); send_data(0x00);    
send_cmd(0xcfd0); send_data(0x00);
send_cmd(0xcb80); send_data(0x00); 
send_cmd(0xcb81); send_data(0x00); 
send_cmd(0xcb82); send_data(0x00); 
send_cmd(0xcb83); send_data(0x00); 
send_cmd(0xcb84); send_data(0x00); 
send_cmd(0xcb85); send_data(0x00); 
send_cmd(0xcb86); send_data(0x00); 
send_cmd(0xcb87); send_data(0x00); 
send_cmd(0xcb88); send_data(0x00); 
send_cmd(0xcb89); send_data(0x00); 
send_cmd(0xcb90); send_data(0x00);
send_cmd(0xcb91); send_data(0x00);
send_cmd(0xcb92); send_data(0x00);
send_cmd(0xcb93); send_data(0x00);
send_cmd(0xcb94); send_data(0x00);
send_cmd(0xcb95); send_data(0x00);
send_cmd(0xcb96); send_data(0x00);
send_cmd(0xcb97); send_data(0x00);
send_cmd(0xcb98); send_data(0x00);
send_cmd(0xcb99); send_data(0x00);
send_cmd(0xcb9a); send_data(0x00);
send_cmd(0xcb9b); send_data(0x00);
send_cmd(0xcb9c); send_data(0x00);
send_cmd(0xcb9d); send_data(0x00);
send_cmd(0xcb9e); send_data(0x00);
send_cmd(0xcba0); send_data(0x00); 
send_cmd(0xcba1); send_data(0x00); 
send_cmd(0xcba2); send_data(0x00); 
send_cmd(0xcba3); send_data(0x00); 
send_cmd(0xcba4); send_data(0x00); 
send_cmd(0xcba5); send_data(0x00); 
send_cmd(0xcba6); send_data(0x00); 
send_cmd(0xcba7); send_data(0x00); 
send_cmd(0xcba8); send_data(0x00); 
send_cmd(0xcba9); send_data(0x00); 
send_cmd(0xcbaa); send_data(0x00); 
send_cmd(0xcbab); send_data(0x00); 
send_cmd(0xcbac); send_data(0x00); 
send_cmd(0xcbad); send_data(0x00); 
send_cmd(0xcbae); send_data(0x00);
send_cmd(0xcbb0); send_data(0x00); 
send_cmd(0xcbb1); send_data(0x00); 
send_cmd(0xcbb2); send_data(0x00); 
send_cmd(0xcbb3); send_data(0x00); 
send_cmd(0xcbb4); send_data(0x00); 
send_cmd(0xcbb5); send_data(0x00); 
send_cmd(0xcbb6); send_data(0x00); 
send_cmd(0xcbb7); send_data(0x00); 
send_cmd(0xcbb8); send_data(0x00); 
send_cmd(0xcbb9); send_data(0x00);
send_cmd(0xcbc0); send_data(0x00); 
send_cmd(0xcbc1); send_data(0x04); 
send_cmd(0xcbc2); send_data(0x04); 
send_cmd(0xcbc3); send_data(0x04); 
send_cmd(0xcbc4); send_data(0x04); 
send_cmd(0xcbc5); send_data(0x04); 
send_cmd(0xcbc6); send_data(0x00); 
send_cmd(0xcbc7); send_data(0x00); 
send_cmd(0xcbc8); send_data(0x00); 
send_cmd(0xcbc9); send_data(0x00); 
send_cmd(0xcbca); send_data(0x00); 
send_cmd(0xcbcb); send_data(0x00); 
send_cmd(0xcbcc); send_data(0x00); 
send_cmd(0xcbcd); send_data(0x00); 
send_cmd(0xcbce); send_data(0x00);
send_cmd(0xcbd0); send_data(0x00); 
send_cmd(0xcbd1); send_data(0x00); 
send_cmd(0xcbd2); send_data(0x00); 
send_cmd(0xcbd3); send_data(0x00); 
send_cmd(0xcbd4); send_data(0x00); 
send_cmd(0xcbd5); send_data(0x00); 
send_cmd(0xcbd6); send_data(0x04); 
send_cmd(0xcbd7); send_data(0x04); 
send_cmd(0xcbd8); send_data(0x04); 
send_cmd(0xcbd9); send_data(0x04); 
send_cmd(0xcbda); send_data(0x04); 
send_cmd(0xcbdb); send_data(0x00); 
send_cmd(0xcbdc); send_data(0x00); 
send_cmd(0xcbdd); send_data(0x00); 
send_cmd(0xcbde); send_data(0x00);
send_cmd(0xcbe0); send_data(0x00); 
send_cmd(0xcbe1); send_data(0x00); 
send_cmd(0xcbe2); send_data(0x00); 
send_cmd(0xcbe3); send_data(0x00); 
send_cmd(0xcbe4); send_data(0x00); 
send_cmd(0xcbe5); send_data(0x00); 
send_cmd(0xcbe6); send_data(0x00); 
send_cmd(0xcbe7); send_data(0x00); 
send_cmd(0xcbe8); send_data(0x00); 
send_cmd(0xcbe9); send_data(0x00);
send_cmd(0xcbf0); send_data(0xFF); 
send_cmd(0xcbf1); send_data(0xFF); 
send_cmd(0xcbf2); send_data(0xFF); 
send_cmd(0xcbf3); send_data(0xFF); 
send_cmd(0xcbf4); send_data(0xFF); 
send_cmd(0xcbf5); send_data(0xFF); 
send_cmd(0xcbf6); send_data(0xFF); 
send_cmd(0xcbf7); send_data(0xFF); 
send_cmd(0xcbf8); send_data(0xFF); 
send_cmd(0xcbf9); send_data(0xFF);
send_cmd(0xcc80); send_data(0x00); 
send_cmd(0xcc81); send_data(0x26); 
send_cmd(0xcc82); send_data(0x09); 
send_cmd(0xcc83); send_data(0x0B); 
send_cmd(0xcc84); send_data(0x01); 
send_cmd(0xcc85); send_data(0x25); 
send_cmd(0xcc86); send_data(0x00); 
send_cmd(0xcc87); send_data(0x00); 
send_cmd(0xcc88); send_data(0x00); 
send_cmd(0xcc89); send_data(0x00);
send_cmd(0xcc90); send_data(0x00); 
send_cmd(0xcc91); send_data(0x00); 
send_cmd(0xcc92); send_data(0x00); 
send_cmd(0xcc93); send_data(0x00); 
send_cmd(0xcc94); send_data(0x00); 
send_cmd(0xcc95); send_data(0x00); 
send_cmd(0xcc96); send_data(0x00); 
send_cmd(0xcc97); send_data(0x00); 
send_cmd(0xcc98); send_data(0x00); 
send_cmd(0xcc99); send_data(0x00); 
send_cmd(0xcc9a); send_data(0x00); 
send_cmd(0xcc9b); send_data(0x26); 
send_cmd(0xcc9c); send_data(0x0A); 
send_cmd(0xcc9d); send_data(0x0C); 
send_cmd(0xcc9e); send_data(0x02);
send_cmd(0xcca0); send_data(0x25); 
send_cmd(0xcca1); send_data(0x00); 
send_cmd(0xcca2); send_data(0x00); 
send_cmd(0xcca3); send_data(0x00); 
send_cmd(0xcca4); send_data(0x00); 
send_cmd(0xcca5); send_data(0x00); 
send_cmd(0xcca6); send_data(0x00); 
send_cmd(0xcca7); send_data(0x00); 
send_cmd(0xcca8); send_data(0x00); 
send_cmd(0xcca9); send_data(0x00); 
send_cmd(0xccaa); send_data(0x00); 
send_cmd(0xccab); send_data(0x00); 
send_cmd(0xccac); send_data(0x00); 
send_cmd(0xccad); send_data(0x00); 
send_cmd(0xccae); send_data(0x00);
send_cmd(0xccb0); send_data(0x00); 
send_cmd(0xccb1); send_data(0x25); 
send_cmd(0xccb2); send_data(0x0C); 
send_cmd(0xccb3); send_data(0x0A); 
send_cmd(0xccb4); send_data(0x02); 
send_cmd(0xccb5); send_data(0x26); 
send_cmd(0xccb6); send_data(0x00); 
send_cmd(0xccb7); send_data(0x00); 
send_cmd(0xccb8); send_data(0x00); 
send_cmd(0xccb9); send_data(0x00);
send_cmd(0xccc0); send_data(0x00); 
send_cmd(0xccc1); send_data(0x00); 
send_cmd(0xccc2); send_data(0x00); 
send_cmd(0xccc3); send_data(0x00); 
send_cmd(0xccc4); send_data(0x00); 
send_cmd(0xccc5); send_data(0x00); 
send_cmd(0xccc6); send_data(0x00); 
send_cmd(0xccc7); send_data(0x00); 
send_cmd(0xccc8); send_data(0x00); 
send_cmd(0xccc9); send_data(0x00); 
send_cmd(0xccca); send_data(0x00); 
send_cmd(0xcccb); send_data(0x25); 
send_cmd(0xcccc); send_data(0x0B); 
send_cmd(0xcccd); send_data(0x09); 
send_cmd(0xccce); send_data(0x01);
send_cmd(0xccd0); send_data(0x26); 
send_cmd(0xccd1); send_data(0x00); 
send_cmd(0xccd2); send_data(0x00); 
send_cmd(0xccd3); send_data(0x00); 
send_cmd(0xccd4); send_data(0x00); 
send_cmd(0xccd5); send_data(0x00); 
send_cmd(0xccd6); send_data(0x00); 
send_cmd(0xccd7); send_data(0x00); 
send_cmd(0xccd8); send_data(0x00); 
send_cmd(0xccd9); send_data(0x00); 
send_cmd(0xccda); send_data(0x00); 
send_cmd(0xccdb); send_data(0x00); 
send_cmd(0xccdc); send_data(0x00); 
send_cmd(0xccdd); send_data(0x00); 
send_cmd(0xccde); send_data(0x00); 

send_cmd(0x4400); send_data(0x00);
send_cmd(0x4401); send_data(0x0E);

send_cmd(0x3A00); send_data(0x66); 
send_cmd(0x3500); send_data(0x00);
send_cmd(0x3600); send_data(0xC0);
//send_cmd(0x2100);
send_cmd(0x1100);
Delayms(150);
send_cmd(0x2900);
Delayms(200);

send_cmd(0x2A00); send_data(0x00);
send_cmd(0x2A01); send_data(0x00);
send_cmd(0x2A02); send_data(0x01);
send_cmd(0x2A03); send_data(0xDF);
send_cmd(0x2B00); send_data(0x00);
send_cmd(0x2B01); send_data(0x00);
send_cmd(0x2B02); send_data(0x03);  
send_cmd(0x2B03); send_data(0x1F);
send_cmd(0x2C00);

        sw_clear_panel(0x00);
        MDELAY(10);
#else
send_cmd(0xff00); send_data(0x80);
send_cmd(0xff01); send_data(0x09); //enable EXTC
send_cmd(0xff02); send_data(0x01);
send_cmd(0xff80); send_data(0x80); //enable Orise mode
send_cmd(0xff81); send_data(0x09);
send_cmd(0xff03); send_data(0x01); //enable SPI+I2C cmd2 read

//gamma DC
send_cmd(0xc0b4); send_data(0x50); //column inversion 	
send_cmd(0xC489); send_data(0x08); //reg off	
send_cmd(0xC0a3); send_data(0x00); //pre-charge //V02             
send_cmd(0xC582); send_data(0xA3); //REG-pump23
send_cmd(0xC590); send_data(0x96); //Pump setting (3x=D6)-->(2x=96)//v02 01/11
send_cmd(0xC591); send_data(0x87); //Pump setting(VGH/VGL)   
send_cmd(0xD800); send_data(0x73); //GVDD=4.5V  73   
send_cmd(0xD801); send_data(0x71); //NGVDD=4.5V 71  

//VCOMDC                                                                 
send_cmd(0xD900); send_data(0x73); // 0x73

send_cmd(0xE100); send_data(0x00);
send_cmd(0xE101); send_data(0x09);
send_cmd(0xE102); send_data(0x0F);
send_cmd(0xE103); send_data(0x0E);
send_cmd(0xE104); send_data(0x07);
send_cmd(0xE105); send_data(0x10);
send_cmd(0xE106); send_data(0x0B);
send_cmd(0xE107); send_data(0x0A);
send_cmd(0xE108); send_data(0x04);
send_cmd(0xE109); send_data(0x07);
send_cmd(0xE10A); send_data(0x0B);
send_cmd(0xE10B); send_data(0x08);
send_cmd(0xE10C); send_data(0x0F);
send_cmd(0xE10D); send_data(0x10);
send_cmd(0xE10E); send_data(0x0A);
send_cmd(0xE10F); send_data(0x01);
                
send_cmd(0xE200); send_data(0x00);
send_cmd(0xE201); send_data(0x09);
send_cmd(0xE202); send_data(0x0F);
send_cmd(0xE203); send_data(0x0E);
send_cmd(0xE204); send_data(0x07);
send_cmd(0xE205); send_data(0x10);
send_cmd(0xE206); send_data(0x0B);
send_cmd(0xE207); send_data(0x0A);
send_cmd(0xE208); send_data(0x04);
send_cmd(0xE209); send_data(0x07);
send_cmd(0xE20A); send_data(0x0B);
send_cmd(0xE20B); send_data(0x08);
send_cmd(0xE20C); send_data(0x0F);
send_cmd(0xE20D); send_data(0x10);
send_cmd(0xE20E); send_data(0x0A);
send_cmd(0xE20F); send_data(0x01);

send_cmd(0xC181); send_data(0x66);     //Frame rate 65Hz//V02   

// RGB I/F setting VSYNC for OTM8018 0x0e
send_cmd(0xC1a1); send_data(0x08);     //external Vsync,Hsync,DE
send_cmd(0xC0a3); send_data(0x1b);     //pre-charge //V02
send_cmd(0xC481); send_data(0x83);     //source bias //V02
send_cmd(0xC592); send_data(0x01);     //Pump45
send_cmd(0xC5B1); send_data(0xA9);     //DC voltage setting ;[0]GVDD output, default: 0xa8

// CE8x : vst1, vst2, vst3, vst4
send_cmd(0xCE80); send_data(0x85);	// ce81[7:0] : vst1_shift[7:0]
send_cmd(0xCE81); send_data(0x03);	// ce82[7:0] : 0000,	vst1_width[3:0]
send_cmd(0xCE82); send_data(0x00);	// ce83[7:0] : vst1_tchop[7:0]
send_cmd(0xCE83); send_data(0x84);	// ce84[7:0] : vst2_shift[7:0]
send_cmd(0xCE84); send_data(0x03);	// ce85[7:0] : 0000,	vst2_width[3:0]
send_cmd(0xCE85); send_data(0x00);	// ce86[7:0] : vst2_tchop[7:0]	                                                                                                                      
send_cmd(0xCE86); send_data(0x83);	// ce87[7:0] : vst3_shift[7:0]	                                                                                                                      
send_cmd(0xCE87); send_data(0x03);	// ce88[7:0] : 0000,	vst3_width[3:0]
send_cmd(0xCE88); send_data(0x00);	// ce89[7:0] : vst3_tchop[7:0]                                                     
send_cmd(0xCE89); send_data(0x82);	// ce8a[7:0] : vst4_shift[7:0]
send_cmd(0xCE8a); send_data(0x03);	// ce8b[7:0] : 0000,	vst4_width[3:0]
send_cmd(0xCE8b); send_data(0x00);	// ce8c[7:0] : vst4_tchop[7:0]

//CEAx : clka1, clka2
send_cmd(0xCEa0); send_data(0x38);	// cea1[7:0] : clka1_width[3:0], clka1_shift[11:8]
send_cmd(0xCEa1); send_data(0x02);	// cea2[7:0] : clka1_shift[7:0]
send_cmd(0xCEa2); send_data(0x03);	// cea3[7:0] : clka1_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
send_cmd(0xCEa3); send_data(0x21);	// cea4[7:0] : clka1_switch[7:0]                                                                                        
send_cmd(0xCEa4); send_data(0x00);	// cea5[7:0] : clka1_extend[7:0]                                                                                        
send_cmd(0xCEa5); send_data(0x00);	// cea6[7:0] : clka1_tchop[7:0]                                                                                         
send_cmd(0xCEa6); send_data(0x00);	// cea7[7:0] : clka1_tglue[7:0]                                                                                         
send_cmd(0xCEa7); send_data(0x38);	// cea8[7:0] : clka2_width[3:0], clka2_shift[11:8]                                                                      
send_cmd(0xCEa8); send_data(0x01);	// cea9[7:0] : clka2_shift[7:0]                                                                                         
send_cmd(0xCEa9); send_data(0x03);	// ceaa[7:0] : clka2_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
send_cmd(0xCEaa); send_data(0x22);	// ceab[7:0] : clka2_switch[7:0]                                                                                        
send_cmd(0xCEab); send_data(0x00);	// ceac[7:0] : clka2_extend                                                                                             
send_cmd(0xCEac); send_data(0x00);	// cead[7:0] : clka2_tchop                                                                                              
send_cmd(0xCEad); send_data(0x00);	// ceae[7:0] : clka2_tglue                                                                                              
                                                                                                                                                            
//CEBx : clka3, clka4                                                                                                                                       
send_cmd(0xCEb0); send_data(0x38);	// ceb1[7:0] : clka3_width[3:0], clka3_shift[11:8]                                                                      
send_cmd(0xCEb1); send_data(0x00);	// ceb2[7:0] : clka3_shift[7:0]                                                                                         
send_cmd(0xCEb2); send_data(0x03);	// ceb3[7:0] : clka3_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
send_cmd(0xCEb3); send_data(0x23);	// ceb4[7:0] : clka3_switch[7:0]                                                                                        
send_cmd(0xCEb4); send_data(0x00);	// ceb5[7:0] : clka3_extend[7:0]                                                                                        
send_cmd(0xCEb5); send_data(0x00);	// ceb6[7:0] : clka3_tchop[7:0]                                                                                         
send_cmd(0xCEb6); send_data(0x00);	// ceb7[7:0] : clka3_tglue[7:0]                                                                                         
send_cmd(0xCEb7); send_data(0x30);	// ceb8[7:0] : clka4_width[3:0], clka2_shift[11:8]                                                                      
send_cmd(0xCEb8); send_data(0x00);	// ceb9[7:0] : clka4_shift[7:0]                                                                                         
send_cmd(0xCEb9); send_data(0x03);	// ceba[7:0] : clka4_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
send_cmd(0xCEba); send_data(0x24);	// cebb[7:0] : clka4_switch[7:0]                                                                                        
send_cmd(0xCEbb); send_data(0x00);	// cebc[7:0] : clka4_extend                                                                                             
send_cmd(0xCEbc); send_data(0x00);	// cebd[7:0] : clka4_tchop                                                                                              
send_cmd(0xCEbd); send_data(0x00);	// cebe[7:0] : clka4_tglue                                                                                              
                                                                                                                                                            
//CECx : clkb1, clkb2                                                                                                                                       
send_cmd(0xCEc0); send_data(0x30);	// cec1[7:0] : clkb1_width[3:0], clkb1_shift[11:8]                                                                      
send_cmd(0xCEc1); send_data(0x01);	// cec2[7:0] : clkb1_shift[7:0]                                                                                         
send_cmd(0xCEc2); send_data(0x03);	// cec3[7:0] : clkb1_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
send_cmd(0xCEc3); send_data(0x25);	// cec4[7:0] : clkb1_switch[7:0]                                                                                        
send_cmd(0xCEc4); send_data(0x00);	// cec5[7:0] : clkb1_extend[7:0]                                                                                        
send_cmd(0xCEc5); send_data(0x00);	// cec6[7:0] : clkb1_tchop[7:0]                                                                                         
send_cmd(0xCEc6); send_data(0x00);	// cec7[7:0] : clkb1_tglue[7:0]                                                                                         
send_cmd(0xCEc7); send_data(0x30);	// cec8[7:0] : clkb2_width[3:0], clkb2_shift[11:8]                                                                      
send_cmd(0xCEc8); send_data(0x02);	// cec9[7:0] : clkb2_shift[7:0]                                                                                         
send_cmd(0xCEc9); send_data(0x03);	// ceca[7:0] : clkb2_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
send_cmd(0xCEca); send_data(0x26);	// cecb[7:0] : clkb2_switch[7:0]                                                                                        
send_cmd(0xCEcb); send_data(0x00);	// cecc[7:0] : clkb2_extend                                                                                             
send_cmd(0xCEcc); send_data(0x00);	// cecd[7:0] : clkb2_tchop                                                                                              
send_cmd(0xCEcd); send_data(0x00);	// cece[7:0] : clkb2_tglue                                                                                              
                                   	                                                                                                                        
//CEDx : clkb3, clkb4                                                                                                                                       
send_cmd(0xCEd0); send_data(0x30);	// ced1[7:0] : clkb3_width[3:0], clkb3_shift[11:8]                                                                      
send_cmd(0xCEd1); send_data(0x03);	// ced2[7:0] : clkb3_shift[7:0]                                                                                         
send_cmd(0xCEd2); send_data(0x03);	// ced3[7:0] : clkb3_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
send_cmd(0xCEd3); send_data(0x27);	// ced4[7:0] : clkb3_switch[7:0]                                                                                        
send_cmd(0xCEd4); send_data(0x00);	// ced5[7:0] : clkb3_extend[7:0]                                                                                        
send_cmd(0xCEd5); send_data(0x00);	// ced6[7:0] : clkb3_tchop[7:0]                                                                                         
send_cmd(0xCEd6); send_data(0x00);	// ced7[7:0] : clkb3_tglue[7:0]                                                                                         
send_cmd(0xCEd7); send_data(0x30);	// ced8[7:0] : clkb4_width[3:0], clkb4_shift[11:8]                                                                      
send_cmd(0xCEd8); send_data(0x04);	// ced9[7:0] : clkb4_shift[7:0]                                                                                         
send_cmd(0xCEd9); send_data(0x03);	// ceda[7:0] : clkb4_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
send_cmd(0xCEda); send_data(0x28);	// cedb[7:0] : clkb4_switch[7:0]                                                                                        
send_cmd(0xCEdb); send_data(0x00);	// cedc[7:0] : clkb4_extend                                                                                             
send_cmd(0xCEdc); send_data(0x00);	// cedd[7:0] : clkb4_tchop                                                                                              
send_cmd(0xCEdd); send_data(0x00);	// cede[7:0] : clkb4_tglue                                                                                              
                                                                                                                                                            
//CFCx :                                                                                                                                                    
send_cmd(0xCFc0); send_data(0x00);	// cfc1[7:0] : eclk_normal_width[7:0]                                                                                   
send_cmd(0xCFc1); send_data(0x00);	// cfc2[7:0] : eclk_partial_width[7:0]                                                                                  
send_cmd(0xCFc2); send_data(0x00);	// cfc3[7:0] : all_normal_tchop[7:0]                                                                                    
send_cmd(0xCFc3); send_data(0x00);	// cfc4[7:0] : all_partial_tchop[7:0]                                                                                   
send_cmd(0xCFc4); send_data(0x00);	// cfc5[7:0] : eclk1_follow[3:0], eclk2_follow[3:0]                                                                     
send_cmd(0xCFc5); send_data(0x00);	// cfc6[7:0] : eclk3_follow[3:0], eclk4_follow[3:0]                                                                     
send_cmd(0xCFc6); send_data(0x00);	// cfc7[7:0] : 00, vstmask, vendmask, 00, dir1, dir2 (0=VGL, 1=VGH)                                                     
send_cmd(0xCFc7); send_data(0x00);	// cfc8[7:0] : reg_goa_gnd_opt, reg_goa_dpgm_tail_set, reg_goa_f_gating_en, reg_goa_f_odd_gating, toggle_mod1, 2, 3, 4  
send_cmd(0xCFc8); send_data(0x00);	// cfc9[7:0] : duty_block[3:0], DGPM[3:0]                                                                               
send_cmd(0xCFc9); send_data(0x00);	// cfca[7:0] : reg_goa_gnd_period[7:0]                                                                                  

//CFDx :
send_cmd(0xCFd0);	send_data(0x00);// cfd1[7:0] : 0000000, reg_goa_frame_odd_high
// PARAMETER 1

//--------------------------------------------------------------------------------
//		initial setting 3 < Panel setting >
//--------------------------------------------------------------------------------
// cbcx
send_cmd(0xCBc0); send_data(0x00);	//cbc1[7:0] : enmode H-byte of sig1  (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBc1); send_data(0x00);	//cbc2[7:0] : enmode H-byte of sig2  (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBc2); send_data(0x00);	//cbc3[7:0] : enmode H-byte of sig3  (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBc3); send_data(0x00);	//cbc4[7:0] : enmode H-byte of sig4  (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBc4); send_data(0x04);	//cbc5[7:0] : enmode H-byte of sig5  (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBc5); send_data(0x04);	//cbc6[7:0] : enmode H-byte of sig6  (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBc6); send_data(0x04);	//cbc7[7:0] : enmode H-byte of sig7  (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBc7); send_data(0x04);	//cbc8[7:0] : enmode H-byte of sig8  (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBc8); send_data(0x04);	//cbc9[7:0] : enmode H-byte of sig9  (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBc9); send_data(0x04);	//cbca[7:0] : enmode H-byte of sig10 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBca); send_data(0x00);	//cbcb[7:0] : enmode H-byte of sig11 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBcb); send_data(0x00);	//cbcc[7:0] : enmode H-byte of sig12 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBcc); send_data(0x00);	//cbcd[7:0] : enmode H-byte of sig13 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBcd); send_data(0x00);	//cbce[7:0] : enmode H-byte of sig14 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBce); send_data(0x00);	//cbcf[7:0] : enmode H-byte of sig15 (pwrof_0, pwrof_1, norm, pwron_4 )                          
                                                                                                                                     
// cbdx                                                                                                                              
send_cmd(0xCBd0); send_data(0x00);	//cbd1[7:0] : enmode H-byte of sig16 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBd1); send_data(0x00);	//cbd2[7:0] : enmode H-byte of sig17 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBd2); send_data(0x00);	//cbd3[7:0] : enmode H-byte of sig18 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBd3); send_data(0x00);	//cbd4[7:0] : enmode H-byte of sig19 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBd4); send_data(0x00);	//cbd5[7:0] : enmode H-byte of sig20 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBd5); send_data(0x00);	//cbd6[7:0] : enmode H-byte of sig21 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBd6); send_data(0x00);	//cbd7[7:0] : enmode H-byte of sig22 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBd7); send_data(0x00);	//cbd8[7:0] : enmode H-byte of sig23 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBd8); send_data(0x00);	//cbd9[7:0] : enmode H-byte of sig24 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBd9); send_data(0x04);	//cbda[7:0] : enmode H-byte of sig25 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBda); send_data(0x04);	//cbdb[7:0] : enmode H-byte of sig26 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBdb); send_data(0x04);	//cbdc[7:0] : enmode H-byte of sig27 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBdc); send_data(0x04);	//cbdd[7:0] : enmode H-byte of sig28 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBdd); send_data(0x04);	//cbde[7:0] : enmode H-byte of sig29 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBde); send_data(0x04);	//cbdf[7:0] : enmode H-byte of sig30 (pwrof_0, pwrof_1, norm, pwron_4 )                          
                                                             
// cbex                                                      
send_cmd(0xCBe0); send_data(0x00);	//cbe1[7:0] : enmode H-byte of sig31 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBe1); send_data(0x00);	//cbe2[7:0] : enmode H-byte of sig32 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBe2); send_data(0x00);	//cbe3[7:0] : enmode H-byte of sig33 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBe3); send_data(0x00);	//cbe4[7:0] : enmode H-byte of sig34 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBe4); send_data(0x00);	//cbe5[7:0] : enmode H-byte of sig35 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBe5); send_data(0x00);	//cbe6[7:0] : enmode H-byte of sig36 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBe6); send_data(0x00);	//cbe7[7:0] : enmode H-byte of sig37 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBe7); send_data(0x00);	//cbe8[7:0] : enmode H-byte of sig38 (pwrof_0, pwrof_1, norm, pwron_4 )                          
send_cmd(0xCBe8); send_data(0x00);	//cbe9[7:0] : enmode H-byte of sig39 (pwrof_0, pwrof_1, norm, pwron_4 )                                                                                                  
send_cmd(0xCBe9); send_data(0x00);	//cbea[7:0] : enmode H-byte of sig40 (pwrof_0, pwrof_1, norm, pwron_4 )                                                                                                  
                                                  
// cc8x                                           
send_cmd(0xCC80); send_data(0x00);	//cc81[7:0] : reg setting for signal01 selection with u2d mode                                   
send_cmd(0xCC81); send_data(0x00);	//cc82[7:0] : reg setting for signal02 selection with u2d mode                                   
send_cmd(0xCC82); send_data(0x00);	//cc83[7:0] : reg setting for signal03 selection with u2d mode                                   
send_cmd(0xCC83); send_data(0x00);	//cc84[7:0] : reg setting for signal04 selection with u2d mode                                   
send_cmd(0xCC84); send_data(0x0C);	//cc85[7:0] : reg setting for signal05 selection with u2d mode                                   
send_cmd(0xCC85); send_data(0x0A);	//cc86[7:0] : reg setting for signal06 selection with u2d mode                                   
send_cmd(0xCC86); send_data(0x10);	//cc87[7:0] : reg setting for signal07 selection with u2d mode                                   
send_cmd(0xCC87); send_data(0x0E);	//cc88[7:0] : reg setting for signal08 selection with u2d mode                                   
send_cmd(0xCC88); send_data(0x03);	//cc89[7:0] : reg setting for signal09 selection with u2d mode                                   
send_cmd(0xCC89); send_data(0x04);	//cc8a[7:0] : reg setting for signal10 selection with u2d mode                                   
                                                                                                                                     
// cc9x                                                                                                                              
send_cmd(0xCC90); send_data(0x00);	//cc91[7:0] : reg setting for signal11 selection with u2d mode                                   
send_cmd(0xCC91); send_data(0x00);	//cc92[7:0] : reg setting for signal12 selection with u2d mode                                   
send_cmd(0xCC92); send_data(0x00);	//cc93[7:0] : reg setting for signal13 selection with u2d mode                                   
send_cmd(0xCC93); send_data(0x00);	//cc94[7:0] : reg setting for signal14 selection with u2d mode                                   
send_cmd(0xCC94); send_data(0x00);	//cc95[7:0] : reg setting for signal15 selection with u2d mode                                   
send_cmd(0xCC95); send_data(0x00);	//cc96[7:0] : reg setting for signal16 selection with u2d mode                                   
send_cmd(0xCC96); send_data(0x00);	//cc97[7:0] : reg setting for signal17 selection with u2d mode                                   
send_cmd(0xCC97); send_data(0x00);	//cc98[7:0] : reg setting for signal18 selection with u2d mode                                   
send_cmd(0xCC98); send_data(0x00);	//cc99[7:0] : reg setting for signal19 selection with u2d mode                                   
send_cmd(0xCC99); send_data(0x00);	//cc9a[7:0] : reg setting for signal20 selection with u2d mode                                   
send_cmd(0xCC9a); send_data(0x00);	//cc9b[7:0] : reg setting for signal21 selection with u2d mode                                   
send_cmd(0xCC9b); send_data(0x00);	//cc9c[7:0] : reg setting for signal22 selection with u2d mode                                   
send_cmd(0xCC9c); send_data(0x00);	//cc9d[7:0] : reg setting for signal23 selection with u2d mode                                   
send_cmd(0xCC9d); send_data(0x00);	//cc9e[7:0] : reg setting for signal24 selection with u2d mode                                   
send_cmd(0xCC9e); send_data(0x0B);	//cc9f[7:0] : reg setting for signal25 selection with u2d mode                                   
                                                                                                                                     
// ccax                                                                                                                              
send_cmd(0xCCa0); send_data(0x09);	//cca1[7:0] : reg setting for signal26 selection with u2d mode                                   
send_cmd(0xCCa1); send_data(0x0F);	//cca2[7:0] : reg setting for signal27 selection with u2d mode                                   
send_cmd(0xCCa2); send_data(0x0D);	//cca3[7:0] : reg setting for signal28 selection with u2d mode                                   
send_cmd(0xCCa3); send_data(0x01);	//cca4[7:0] : reg setting for signal29 selection with u2d mode                                   
send_cmd(0xCCa4); send_data(0x02);	//cca5[7:0] : reg setting for signal20 selection with u2d mode                                   
send_cmd(0xCCa5); send_data(0x00);	//cca6[7:0] : reg setting for signal31 selection with u2d mode                                   
send_cmd(0xCCa6); send_data(0x00);	//cca7[7:0] : reg setting for signal32 selection with u2d mode                                   
send_cmd(0xCCa7); send_data(0x00);	//cca8[7:0] : reg setting for signal33 selection with u2d mode                                   
send_cmd(0xCCa8); send_data(0x00);	//cca9[7:0] : reg setting for signal34 selection with u2d mode                                   
send_cmd(0xCCa9); send_data(0x00);	//ccaa[7:0] : reg setting for signal35 selection with u2d mode                                   
send_cmd(0xCCaa); send_data(0x00);	//ccab[7:0] : reg setting for signal36 selection with u2d mode                                   
send_cmd(0xCCab); send_data(0x00);	//ccac[7:0] : reg setting for signal37 selection with u2d mode                                   
send_cmd(0xCCac); send_data(0x00);	//ccad[7:0] : reg setting for signal38 selection with u2d mode                                   
send_cmd(0xCCad); send_data(0x00);	//ccae[7:0] : reg setting for signal39 selection with u2d mode                                   
send_cmd(0xCCae); send_data(0x00);	//ccaf[7:0] : reg setting for signal40 selection with u2d mode                                   
                                                                                                                                     
// ccbx                                                                                                                              
send_cmd(0xCCb0); send_data(0x00);	//ccb1[7:0] : reg setting for signal01 selection with d2u mode                                   
send_cmd(0xCCb1); send_data(0x00);	//ccb2[7:0] : reg setting for signal02 selection with d2u mode                                   
send_cmd(0xCCb2); send_data(0x00);	//ccb3[7:0] : reg setting for signal03 selection with d2u mode                                   
send_cmd(0xCCb3); send_data(0x00);	//ccb4[7:0] : reg setting for signal04 selection with d2u mode                                   
send_cmd(0xCCb4); send_data(0x0D);	//ccb5[7:0] : reg setting for signal05 selection with d2u mode                                   
send_cmd(0xCCb5); send_data(0x0F);	//ccb6[7:0] : reg setting for signal06 selection with d2u mode                                   
send_cmd(0xCCb6); send_data(0x09);	//ccb7[7:0] : reg setting for signal07 selection with d2u mode                                   
send_cmd(0xCCb7); send_data(0x0B);	//ccb8[7:0] : reg setting for signal08 selection with d2u mode                                   
send_cmd(0xCCb8); send_data(0x02);	//ccb9[7:0] : reg setting for signal09 selection with d2u mode                                   
send_cmd(0xCCb9); send_data(0x01);	//ccba[7:0] : reg setting for signal10 selection with d2u mode                                   
                                                                                                                                     
// cccx                                                                                                                              
send_cmd(0xCCc0); send_data(0x00);	//ccc1[7:0] : reg setting for signal11 selection with d2u mode                                   
send_cmd(0xCCc1); send_data(0x00);	//ccc2[7:0] : reg setting for signal12 selection with d2u mode                                   
send_cmd(0xCCc2); send_data(0x00);	//ccc3[7:0] : reg setting for signal13 selection with d2u mode                                   
send_cmd(0xCCc3); send_data(0x00);	//ccc4[7:0] : reg setting for signal14 selection with d2u mode                                   
send_cmd(0xCCc4); send_data(0x00);	//ccc5[7:0] : reg setting for signal15 selection with d2u mode                                   
send_cmd(0xCCc5); send_data(0x00);	//ccc6[7:0] : reg setting for signal16 selection with d2u mode                                   
send_cmd(0xCCc6); send_data(0x00);	//ccc7[7:0] : reg setting for signal17 selection with d2u mode                                   
send_cmd(0xCCc7); send_data(0x00);	//ccc8[7:0] : reg setting for signal18 selection with d2u mode                                   
send_cmd(0xCCc8); send_data(0x00);	//ccc9[7:0] : reg setting for signal19 selection with d2u mode                                   
send_cmd(0xCCc9); send_data(0x00);	//ccca[7:0] : reg setting for signal20 selection with d2u mode                                   
send_cmd(0xCCca); send_data(0x00);	//cccb[7:0] : reg setting for signal21 selection with d2u mode                                   
send_cmd(0xCCcb); send_data(0x00);	//cccc[7:0] : reg setting for signal22 selection with d2u mode                                   
send_cmd(0xCCcc); send_data(0x00);	//cccd[7:0] : reg setting for signal23 selection with d2u mode                                   
send_cmd(0xCCcd); send_data(0x00);	//ccce[7:0] : reg setting for signal24 selection with d2u mode                                   
send_cmd(0xCCce); send_data(0x0E);	//cccf[7:0] : reg setting for signal25 selection with d2u mode                                   
                                                                                                                                     
// ccdx                                                                                                                              
send_cmd(0xCCd0); send_data(0x10);	//ccd1[7:0] : reg setting for signal26 selection with d2u mode                                   
send_cmd(0xCCd1); send_data(0x0A);	//ccd2[7:0] : reg setting for signal27 selection with d2u mode                                   
send_cmd(0xCCd2); send_data(0x0C);	//ccd3[7:0] : reg setting for signal28 selection with d2u mode                                   
send_cmd(0xCCd3); send_data(0x04);	//ccd4[7:0] : reg setting for signal29 selection with d2u mode                                   
send_cmd(0xCCd4); send_data(0x03);	//ccd5[7:0] : reg setting for signal30 selection with d2u mode                                   
send_cmd(0xCCd5); send_data(0x00);	//ccd6[7:0] : reg setting for signal31 selection with d2u mode                                   
send_cmd(0xCCd6); send_data(0x00);	//ccd7[7:0] : reg setting for signal32 selection with d2u mode                                   
send_cmd(0xCCd7); send_data(0x00);	//ccd8[7:0] : reg setting for signal33 selection with d2u mode                                   
send_cmd(0xCCd8); send_data(0x00);	//ccd9[7:0] : reg setting for signal34 selection with d2u mode                                   
send_cmd(0xCCd9); send_data(0x00);	//ccda[7:0] : reg setting for signal35 selection with d2u mode                                   
send_cmd(0xCCda); send_data(0x00);	//ccdb[7:0] : reg setting for signal36 selection with d2u mode                                   
send_cmd(0xCCdb); send_data(0x00);	//ccdc[7:0] : reg setting for signal37 selection with d2u mode                                   
send_cmd(0xCCdc); send_data(0x00);	//ccdd[7:0] : reg setting for signal38 selection with d2u mode                                   
send_cmd(0xCCdd); send_data(0x00);	//ccde[7:0] : reg setting for signal39 selection with d2u mode                    
send_cmd(0xCCde); send_data(0x00);	//ccdf[7:0] : reg setting for signal40 selection with d2u mode  

send_cmd(0xff00); send_data(0xff); 
send_cmd(0xff01); send_data(0xff); 
send_cmd(0xff02); send_data(0xff);                   

//#ifdef DISPLAY_DIRECTION_0_MODE
send_cmd(0x3600);  send_data(0x00);// Display Direction 0
send_cmd(0x3500);  send_data(0x00);// TE( Fmark ) Signal On
send_cmd(0x4400);  send_data(0x01);
send_cmd(0x4401);  send_data(0x22);// TE( Fmark ) Signal Output Position
//#endif

#if 0
//#ifdeif DISPLAY_DIRECTION_180_MODE
send_cmd(0x3600);  send_data(0xD0);// Display Direction 180
send_cmd(0x3500);  send_data(0x00);// TE( Fmark ) Signal On
send_cmd(0x4400);  send_data(0x01);
send_cmd(0x4401);  send_data(0xFF);// TE( Fmark ) Signal Output Position
#endif

#if 0
//#ifdef LCD_BACKLIGHT_CONTROL_MODE
send_cmd(0x5100);  send_data(0xFF);// Backlight Level Control
send_cmd(0x5300);  send_data(0x2C);// Backlight On
send_cmd(0x5500);  send_data(0x00);// CABC Function Off
#endif

///=============================
send_cmd(0x3A00); send_data(0x66);     //MCU 16bits D[17:0]
send_cmd(0x1100);
MDELAY(150);	
send_cmd(0x2900);
MDELAY(200);
send_cmd(0x2A00); send_data(0x00);
send_cmd(0x2A01); send_data(0x00);
send_cmd(0x2A02); send_data(0x01);
send_cmd(0x2A03); send_data(0xDF);
send_cmd(0x2B00); send_data(0x00);
send_cmd(0x2B01); send_data(0x00);
send_cmd(0x2B02); send_data(0x03);  
send_cmd(0x2B03); send_data(0x1F);

send_cmd(0x2C00);

MDELAY(200);
        sw_clear_panel(0x00);
        MDELAY(10);
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

    send_ctrl_cmd(0xDA00);
    read_data_cmd();

    id = read_data_cmd();


    Lcd_Log("xxxxx mycat READ OTM8009 LCD ID%s, id = 0x%x\n", __func__, id);


    return (LCM_ID == id)?1:0;

}

//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER otm8009a_yixinda_Y82635_mcu_wvga_lcm_drv =
    {
        .name		= "otm8009a_yixinda_Y82635_mcu_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id    = lcm_compare_id,
    };


