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
#define LCM_ID       (0x8009)
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

send_cmd(0x3600); send_data(0x00);//0xc0  180

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

		/**********************
    send_ctrl_cmd(0xDA00);
    read_data_cmd();

    id = read_data_cmd();
		*******************/
		
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

    //Lcd_Log("mycat READ OTM8009 LCD ID%s, id = 0x%x\n", __func__, id);


    return (LCM_ID == id)?1:0;

}

//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER otm8009a_suxian_S_40_08700A_mcu_wvga_lcm_drv =
    {
        .name		= "otm8009a_suxian_S_40_08700A_mcu_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id    = lcm_compare_id,
    };


