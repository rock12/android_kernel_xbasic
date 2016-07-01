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
#define LCM_ID       (0x55)//(0x8000)
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

static void init_lcm_registers(void)
{
#if defined(BUILD_UBOOT)
    printf("MYCAT LCD nt35510_tengsheng_43008fn_mcu_wvga");
#endif
    LCD_Write_COM(0xF0,0x00);    LCD_Write_DATA(0x00,0x55);
    LCD_Write_COM(0xF0,0x01);    LCD_Write_DATA(0x00,0xAA);
    LCD_Write_COM(0xF0,0x02);    LCD_Write_DATA(0x00,0x52);
    LCD_Write_COM(0xF0,0x03);    LCD_Write_DATA(0x00,0x08);
    LCD_Write_COM(0xF0,0x04);    LCD_Write_DATA(0x00,0x01);



    //Set AVDD Ratio
    LCD_Write_COM(0xB6,0x00);    LCD_Write_DATA(0x00,0x34);
    LCD_Write_COM(0xB6,0x01);    LCD_Write_DATA(0x00,0x34);
    LCD_Write_COM(0xB6,0x02);    LCD_Write_DATA(0x00,0x34);

    //Set AVDD 5.2V
    LCD_Write_COM(0xB0,0x00);    LCD_Write_DATA(0x00,0x09);
    LCD_Write_COM(0xB0,0x01);    LCD_Write_DATA(0x00,0x09);
    LCD_Write_COM(0xB0,0x02);    LCD_Write_DATA(0x00,0x09);

    //Set AVEE Ratio
    LCD_Write_COM(0xB7,0x00);    LCD_Write_DATA(0x00,0x24);
    LCD_Write_COM(0xB7,0x01);    LCD_Write_DATA(0x00,0x24);
    LCD_Write_COM(0xB7,0x02);    LCD_Write_DATA(0x00,0x24);

    //Set AVEE 5.2V
    LCD_Write_COM(0xB1,0x00);    LCD_Write_DATA(0x00,0x09);
    LCD_Write_COM(0xB1,0x01);    LCD_Write_DATA(0x00,0x09);
    LCD_Write_COM(0xB1,0x02);    LCD_Write_DATA(0x00,0x09);

    LCD_Write_COM(0xB8,0x00);    LCD_Write_DATA(0x00,0x34);
    LCD_Write_COM(0xB2,0x00);    LCD_Write_DATA(0x00,0x00);


    //Set VCL -2.5V
    LCD_Write_COM(0xB9,0x00);    LCD_Write_DATA(0x00,0x24);
    LCD_Write_COM(0xB9,0x01);    LCD_Write_DATA(0x00,0x24);
    LCD_Write_COM(0xB9,0x02);    LCD_Write_DATA(0x00,0x24);


    //VGH=15V(1V/step)	Free pump
    LCD_Write_COM(0xB3,0x00);    LCD_Write_DATA(0x00,0x05);		//08
    LCD_Write_COM(0xB3,0x01);    LCD_Write_DATA(0x00,0x05);		//08
    LCD_Write_COM(0xB3,0x02);    LCD_Write_DATA(0x00,0x05);		//08

    LCD_Write_COM(0xBF,0x00);    LCD_Write_DATA(0x00,0x01);

    //VGLX Ratio
    LCD_Write_COM(0xBA,0x00);    LCD_Write_DATA(0x00,0x34);
    LCD_Write_COM(0xBA,0x01);    LCD_Write_DATA(0x00,0x34);
    LCD_Write_COM(0xBA,0x02);    LCD_Write_DATA(0x00,0x34);

    //VGL_REG=-10(1V/step)
    LCD_Write_COM(0xB5,0x00);    LCD_Write_DATA(0x00,0x0b);
    LCD_Write_COM(0xB5,0x01);    LCD_Write_DATA(0x00,0x0b);
    LCD_Write_COM(0xB5,0x02);    LCD_Write_DATA(0x00,0x0b);

    //VGMP/VGSP=4.5V/0V
    LCD_Write_COM(0xBC,0x00);    LCD_Write_DATA(0x00,0x00);		//00
    LCD_Write_COM(0xBC,0x01);    LCD_Write_DATA(0x00,0xa3);		//C8 =5.5V/90=4.8V
    LCD_Write_COM(0xBC,0x02);    LCD_Write_DATA(0x00,0x00);		//01

    //VGMN/VGSN=-4.5V/0V
    LCD_Write_COM(0xBD,0x00);    LCD_Write_DATA(0x00,0x00); //00
    LCD_Write_COM(0xBD,0x01);    LCD_Write_DATA(0x00,0xa3); //90
    LCD_Write_COM(0xBD,0x02);    LCD_Write_DATA(0x00,0x00);

    //Vcom=-1.4V(12.5mV/step)
    LCD_Write_COM(0xBE,0x00);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xBE,0x01);    LCD_Write_DATA(0x00,0x5A); // 50


    LCD_Write_COM(0xD1,0x00);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD1,0x01);    LCD_Write_DATA(0x00,0x37);
    LCD_Write_COM(0xD1,0x02);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD1,0x03);    LCD_Write_DATA(0x00,0x52);
    LCD_Write_COM(0xD1,0x04);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD1,0x05);    LCD_Write_DATA(0x00,0x7B);
    LCD_Write_COM(0xD1,0x06);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD1,0x07);    LCD_Write_DATA(0x00,0x99);
    LCD_Write_COM(0xD1,0x08);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD1,0x09);    LCD_Write_DATA(0x00,0xB1);
    LCD_Write_COM(0xD1,0x0A);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD1,0x0B);    LCD_Write_DATA(0x00,0xD2);
    LCD_Write_COM(0xD1,0x0C);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD1,0x0D);    LCD_Write_DATA(0x00,0xf6);
    LCD_Write_COM(0xD1,0x0E);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD1,0x0F);    LCD_Write_DATA(0x00,0x27);
    LCD_Write_COM(0xD1,0x10);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD1,0x11);    LCD_Write_DATA(0x00,0x41);
    LCD_Write_COM(0xD1,0x12);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD1,0x13);    LCD_Write_DATA(0x00,0x8C);
    LCD_Write_COM(0xD1,0x14);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD1,0x15);    LCD_Write_DATA(0x00,0xBE);
    LCD_Write_COM(0xD1,0x16);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD1,0x17);    LCD_Write_DATA(0x00,0x0B);
    LCD_Write_COM(0xD1,0x18);    LCD_Write_DATA(0x00,0x02);////////////////////
    LCD_Write_COM(0xD1,0x19);    LCD_Write_DATA(0x00,0x48);
    LCD_Write_COM(0xD1,0x1A);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD1,0x1B);    LCD_Write_DATA(0x00,0x4A);
    LCD_Write_COM(0xD1,0x1C);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD1,0x1D);    LCD_Write_DATA(0x00,0x7E);
    LCD_Write_COM(0xD1,0x1E);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD1,0x1F);    LCD_Write_DATA(0x00,0xBC);
    LCD_Write_COM(0xD1,0x20);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD1,0x21);    LCD_Write_DATA(0x00,0xe1);
    LCD_Write_COM(0xD1,0x22);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD1,0x23);    LCD_Write_DATA(0x00,0x10);
    LCD_Write_COM(0xD1,0x24);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD1,0x25);    LCD_Write_DATA(0x00,0x31);
    LCD_Write_COM(0xD1,0x26);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD1,0x27);    LCD_Write_DATA(0x00,0x5A);
    LCD_Write_COM(0xD1,0x28);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD1,0x29);    LCD_Write_DATA(0x00,0x73);
    LCD_Write_COM(0xD1,0x2A);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD1,0x2B);    LCD_Write_DATA(0x00,0x94);
    LCD_Write_COM(0xD1,0x2C);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD1,0x2D);    LCD_Write_DATA(0x00,0x9F);
    LCD_Write_COM(0xD1,0x2E);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD1,0x2F);    LCD_Write_DATA(0x00,0xb3);
    LCD_Write_COM(0xD1,0x30);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD1,0x31);    LCD_Write_DATA(0x00,0xb9);
    LCD_Write_COM(0xD1,0x32);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD1,0x33);    LCD_Write_DATA(0x00,0xc1);





    //Gamma (G+)
    LCD_Write_COM(0xD2,0x00);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD2,0x01);    LCD_Write_DATA(0x00,0x37);
    LCD_Write_COM(0xD2,0x02);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD2,0x03);    LCD_Write_DATA(0x00,0x52);
    LCD_Write_COM(0xD2,0x04);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD2,0x05);    LCD_Write_DATA(0x00,0x7B);
    LCD_Write_COM(0xD2,0x06);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD2,0x07);    LCD_Write_DATA(0x00,0x99);
    LCD_Write_COM(0xD2,0x08);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD2,0x09);    LCD_Write_DATA(0x00,0xB1);
    LCD_Write_COM(0xD2,0x0A);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD2,0x0B);    LCD_Write_DATA(0x00,0xD2);
    LCD_Write_COM(0xD2,0x0C);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD2,0x0D);    LCD_Write_DATA(0x00,0xf6);
    LCD_Write_COM(0xD2,0x0E);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD2,0x0F);    LCD_Write_DATA(0x00,0x27);
    LCD_Write_COM(0xD2,0x10);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD2,0x11);    LCD_Write_DATA(0x00,0x41);
    LCD_Write_COM(0xD2,0x12);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD2,0x13);    LCD_Write_DATA(0x00,0x8C);
    LCD_Write_COM(0xD2,0x14);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD2,0x15);    LCD_Write_DATA(0x00,0xBE);
    LCD_Write_COM(0xD2,0x16);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD2,0x17);    LCD_Write_DATA(0x00,0x0B);
    LCD_Write_COM(0xD2,0x18);    LCD_Write_DATA(0x00,0x02);////////////////////
    LCD_Write_COM(0xD2,0x19);    LCD_Write_DATA(0x00,0x48);
    LCD_Write_COM(0xD2,0x1A);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD2,0x1B);    LCD_Write_DATA(0x00,0x4A);
    LCD_Write_COM(0xD2,0x1C);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD2,0x1D);    LCD_Write_DATA(0x00,0x7E);
    LCD_Write_COM(0xD2,0x1E);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD2,0x1F);    LCD_Write_DATA(0x00,0xBC);
    LCD_Write_COM(0xD2,0x20);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD2,0x21);    LCD_Write_DATA(0x00,0xe1);
    LCD_Write_COM(0xD2,0x22);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD2,0x23);    LCD_Write_DATA(0x00,0x10);
    LCD_Write_COM(0xD2,0x24);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD2,0x25);    LCD_Write_DATA(0x00,0x31);
    LCD_Write_COM(0xD2,0x26);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD2,0x27);    LCD_Write_DATA(0x00,0x5A);
    LCD_Write_COM(0xD2,0x28);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD2,0x29);    LCD_Write_DATA(0x00,0x73);
    LCD_Write_COM(0xD2,0x2A);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD2,0x2B);    LCD_Write_DATA(0x00,0x94);
    LCD_Write_COM(0xD2,0x2C);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD2,0x2D);    LCD_Write_DATA(0x00,0x9F);
    LCD_Write_COM(0xD2,0x2E);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD2,0x2F);    LCD_Write_DATA(0x00,0xb3);
    LCD_Write_COM(0xD2,0x30);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD2,0x31);    LCD_Write_DATA(0x00,0xb9);
    LCD_Write_COM(0xD2,0x32);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD2,0x33);    LCD_Write_DATA(0x00,0xc1);

    //Gamma (B+)
    LCD_Write_COM(0xD3,0x00);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD3,0x01);    LCD_Write_DATA(0x00,0x37);
    LCD_Write_COM(0xD3,0x02);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD3,0x03);    LCD_Write_DATA(0x00,0x52);
    LCD_Write_COM(0xD3,0x04);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD3,0x05);    LCD_Write_DATA(0x00,0x7B);
    LCD_Write_COM(0xD3,0x06);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD3,0x07);    LCD_Write_DATA(0x00,0x99);
    LCD_Write_COM(0xD3,0x08);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD3,0x09);    LCD_Write_DATA(0x00,0xB1);
    LCD_Write_COM(0xD3,0x0A);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD3,0x0B);    LCD_Write_DATA(0x00,0xD2);
    LCD_Write_COM(0xD3,0x0C);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD3,0x0D);    LCD_Write_DATA(0x00,0xf6);
    LCD_Write_COM(0xD3,0x0E);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD3,0x0F);    LCD_Write_DATA(0x00,0x27);
    LCD_Write_COM(0xD3,0x10);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD3,0x11);    LCD_Write_DATA(0x00,0x41);
    LCD_Write_COM(0xD3,0x12);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD3,0x13);    LCD_Write_DATA(0x00,0x8C);
    LCD_Write_COM(0xD3,0x14);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD3,0x15);    LCD_Write_DATA(0x00,0xBE);
    LCD_Write_COM(0xD3,0x16);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD3,0x17);    LCD_Write_DATA(0x00,0x0B);
    LCD_Write_COM(0xD3,0x18);    LCD_Write_DATA(0x00,0x02);////////////////////
    LCD_Write_COM(0xD3,0x19);    LCD_Write_DATA(0x00,0x48);
    LCD_Write_COM(0xD3,0x1A);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD3,0x1B);    LCD_Write_DATA(0x00,0x4A);
    LCD_Write_COM(0xD3,0x1C);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD3,0x1D);    LCD_Write_DATA(0x00,0x7E);
    LCD_Write_COM(0xD3,0x1E);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD3,0x1F);    LCD_Write_DATA(0x00,0xBC);
    LCD_Write_COM(0xD3,0x20);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD3,0x21);    LCD_Write_DATA(0x00,0xe1);
    LCD_Write_COM(0xD3,0x22);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD3,0x23);    LCD_Write_DATA(0x00,0x10);
    LCD_Write_COM(0xD3,0x24);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD3,0x25);    LCD_Write_DATA(0x00,0x31);
    LCD_Write_COM(0xD3,0x26);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD3,0x27);    LCD_Write_DATA(0x00,0x5A);
    LCD_Write_COM(0xD3,0x28);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD3,0x29);    LCD_Write_DATA(0x00,0x73);
    LCD_Write_COM(0xD3,0x2A);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD3,0x2B);    LCD_Write_DATA(0x00,0x94);
    LCD_Write_COM(0xD3,0x2C);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD3,0x2D);    LCD_Write_DATA(0x00,0x9F);
    LCD_Write_COM(0xD3,0x2E);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD3,0x2F);    LCD_Write_DATA(0x00,0xb3);
    LCD_Write_COM(0xD3,0x30);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD3,0x31);    LCD_Write_DATA(0x00,0xb9);
    LCD_Write_COM(0xD3,0x32);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD3,0x33);    LCD_Write_DATA(0x00,0xc1);

    //Gamma (R-)
    LCD_Write_COM(0xD4,0x00);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD4,0x01);    LCD_Write_DATA(0x00,0x37);
    LCD_Write_COM(0xD4,0x02);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD4,0x03);    LCD_Write_DATA(0x00,0x52);
    LCD_Write_COM(0xD4,0x04);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD4,0x05);    LCD_Write_DATA(0x00,0x7B);
    LCD_Write_COM(0xD4,0x06);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD4,0x07);    LCD_Write_DATA(0x00,0x99);
    LCD_Write_COM(0xD4,0x08);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD4,0x09);    LCD_Write_DATA(0x00,0xB1);
    LCD_Write_COM(0xD4,0x0A);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD4,0x0B);    LCD_Write_DATA(0x00,0xD2);
    LCD_Write_COM(0xD4,0x0C);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD4,0x0D);    LCD_Write_DATA(0x00,0xf6);
    LCD_Write_COM(0xD4,0x0E);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD4,0x0F);    LCD_Write_DATA(0x00,0x27);
    LCD_Write_COM(0xD4,0x10);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD4,0x11);    LCD_Write_DATA(0x00,0x41);
    LCD_Write_COM(0xD4,0x12);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD4,0x13);    LCD_Write_DATA(0x00,0x8C);
    LCD_Write_COM(0xD4,0x14);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD4,0x15);    LCD_Write_DATA(0x00,0xBE);
    LCD_Write_COM(0xD4,0x16);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD4,0x17);    LCD_Write_DATA(0x00,0x0B);
    LCD_Write_COM(0xD4,0x18);    LCD_Write_DATA(0x00,0x02);////////////////////
    LCD_Write_COM(0xD4,0x19);    LCD_Write_DATA(0x00,0x48);
    LCD_Write_COM(0xD4,0x1A);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD4,0x1B);    LCD_Write_DATA(0x00,0x4A);
    LCD_Write_COM(0xD4,0x1C);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD4,0x1D);    LCD_Write_DATA(0x00,0x7E);
    LCD_Write_COM(0xD4,0x1E);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD4,0x1F);    LCD_Write_DATA(0x00,0xBC);
    LCD_Write_COM(0xD4,0x20);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD4,0x21);    LCD_Write_DATA(0x00,0xe1);
    LCD_Write_COM(0xD4,0x22);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD4,0x23);    LCD_Write_DATA(0x00,0x10);
    LCD_Write_COM(0xD4,0x24);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD4,0x25);    LCD_Write_DATA(0x00,0x31);
    LCD_Write_COM(0xD4,0x26);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD4,0x27);    LCD_Write_DATA(0x00,0x5A);
    LCD_Write_COM(0xD4,0x28);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD4,0x29);    LCD_Write_DATA(0x00,0x73);
    LCD_Write_COM(0xD4,0x2A);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD4,0x2B);    LCD_Write_DATA(0x00,0x94);
    LCD_Write_COM(0xD4,0x2C);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD4,0x2D);    LCD_Write_DATA(0x00,0x9F);
    LCD_Write_COM(0xD4,0x2E);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD4,0x2F);    LCD_Write_DATA(0x00,0xb3);
    LCD_Write_COM(0xD4,0x30);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD4,0x31);    LCD_Write_DATA(0x00,0xb9);
    LCD_Write_COM(0xD4,0x32);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD4,0x33);    LCD_Write_DATA(0x00,0xc1);

    //Gamma (G-)
    LCD_Write_COM(0xD5,0x00);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD5,0x01);    LCD_Write_DATA(0x00,0x37);
    LCD_Write_COM(0xD5,0x02);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD5,0x03);    LCD_Write_DATA(0x00,0x52);
    LCD_Write_COM(0xD5,0x04);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD5,0x05);    LCD_Write_DATA(0x00,0x7B);
    LCD_Write_COM(0xD5,0x06);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD5,0x07);    LCD_Write_DATA(0x00,0x99);
    LCD_Write_COM(0xD5,0x08);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD5,0x09);    LCD_Write_DATA(0x00,0xB1);
    LCD_Write_COM(0xD5,0x0A);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD5,0x0B);    LCD_Write_DATA(0x00,0xD2);
    LCD_Write_COM(0xD5,0x0C);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD5,0x0D);    LCD_Write_DATA(0x00,0xf6);
    LCD_Write_COM(0xD5,0x0E);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD5,0x0F);    LCD_Write_DATA(0x00,0x27);
    LCD_Write_COM(0xD5,0x10);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD5,0x11);    LCD_Write_DATA(0x00,0x41);
    LCD_Write_COM(0xD5,0x12);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD5,0x13);    LCD_Write_DATA(0x00,0x8C);
    LCD_Write_COM(0xD5,0x14);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD5,0x15);    LCD_Write_DATA(0x00,0xBE);
    LCD_Write_COM(0xD5,0x16);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD5,0x17);    LCD_Write_DATA(0x00,0x0B);
    LCD_Write_COM(0xD5,0x18);    LCD_Write_DATA(0x00,0x02);////////////////////
    LCD_Write_COM(0xD5,0x19);    LCD_Write_DATA(0x00,0x48);
    LCD_Write_COM(0xD5,0x1A);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD5,0x1B);    LCD_Write_DATA(0x00,0x4A);
    LCD_Write_COM(0xD5,0x1C);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD5,0x1D);    LCD_Write_DATA(0x00,0x7E);
    LCD_Write_COM(0xD5,0x1E);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD5,0x1F);    LCD_Write_DATA(0x00,0xBC);
    LCD_Write_COM(0xD5,0x20);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD5,0x21);    LCD_Write_DATA(0x00,0xe1);
    LCD_Write_COM(0xD5,0x22);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD5,0x23);    LCD_Write_DATA(0x00,0x10);
    LCD_Write_COM(0xD5,0x24);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD5,0x25);    LCD_Write_DATA(0x00,0x31);
    LCD_Write_COM(0xD5,0x26);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD5,0x27);    LCD_Write_DATA(0x00,0x5A);
    LCD_Write_COM(0xD5,0x28);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD5,0x29);    LCD_Write_DATA(0x00,0x73);
    LCD_Write_COM(0xD5,0x2A);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD5,0x2B);    LCD_Write_DATA(0x00,0x94);
    LCD_Write_COM(0xD5,0x2C);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD5,0x2D);    LCD_Write_DATA(0x00,0x9F);
    LCD_Write_COM(0xD5,0x2E);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD5,0x2F);    LCD_Write_DATA(0x00,0xb3);
    LCD_Write_COM(0xD5,0x30);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD5,0x31);    LCD_Write_DATA(0x00,0xb9);
    LCD_Write_COM(0xD5,0x32);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD5,0x33);    LCD_Write_DATA(0x00,0xc1);

    //Gamma (B-)
    LCD_Write_COM(0xD6,0x00);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD6,0x01);    LCD_Write_DATA(0x00,0x37);
    LCD_Write_COM(0xD6,0x02);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD6,0x03);    LCD_Write_DATA(0x00,0x52);
    LCD_Write_COM(0xD6,0x04);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD6,0x05);    LCD_Write_DATA(0x00,0x7B);
    LCD_Write_COM(0xD6,0x06);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD6,0x07);    LCD_Write_DATA(0x00,0x99);
    LCD_Write_COM(0xD6,0x08);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD6,0x09);    LCD_Write_DATA(0x00,0xB1);
    LCD_Write_COM(0xD6,0x0A);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD6,0x0B);    LCD_Write_DATA(0x00,0xD2);
    LCD_Write_COM(0xD6,0x0C);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0xD6,0x0D);    LCD_Write_DATA(0x00,0xf6);
    LCD_Write_COM(0xD6,0x0E);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD6,0x0F);    LCD_Write_DATA(0x00,0x27);
    LCD_Write_COM(0xD6,0x10);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD6,0x11);    LCD_Write_DATA(0x00,0x41);
    LCD_Write_COM(0xD6,0x12);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD6,0x13);    LCD_Write_DATA(0x00,0x8C);
    LCD_Write_COM(0xD6,0x14);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xD6,0x15);    LCD_Write_DATA(0x00,0xBE);
    LCD_Write_COM(0xD6,0x16);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD6,0x17);    LCD_Write_DATA(0x00,0x0B);
    LCD_Write_COM(0xD6,0x18);    LCD_Write_DATA(0x00,0x02);////////////////////
    LCD_Write_COM(0xD6,0x19);    LCD_Write_DATA(0x00,0x48);
    LCD_Write_COM(0xD6,0x1A);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD6,0x1B);    LCD_Write_DATA(0x00,0x4A);
    LCD_Write_COM(0xD6,0x1C);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD6,0x1D);    LCD_Write_DATA(0x00,0x7E);
    LCD_Write_COM(0xD6,0x1E);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD6,0x1F);    LCD_Write_DATA(0x00,0xBC);
    LCD_Write_COM(0xD6,0x20);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xD6,0x21);    LCD_Write_DATA(0x00,0xe1);
    LCD_Write_COM(0xD6,0x22);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD6,0x23);    LCD_Write_DATA(0x00,0x10);
    LCD_Write_COM(0xD6,0x24);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD6,0x25);    LCD_Write_DATA(0x00,0x31);
    LCD_Write_COM(0xD6,0x26);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD6,0x27);    LCD_Write_DATA(0x00,0x5A);
    LCD_Write_COM(0xD6,0x28);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD6,0x29);    LCD_Write_DATA(0x00,0x73);
    LCD_Write_COM(0xD6,0x2A);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD6,0x2B);    LCD_Write_DATA(0x00,0x94);
    LCD_Write_COM(0xD6,0x2C);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD6,0x2D);    LCD_Write_DATA(0x00,0x9F);
    LCD_Write_COM(0xD6,0x2E);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD6,0x2F);    LCD_Write_DATA(0x00,0xb3);
    LCD_Write_COM(0xD6,0x30);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD6,0x31);    LCD_Write_DATA(0x00,0xb9);
    LCD_Write_COM(0xD6,0x32);    LCD_Write_DATA(0x00,0x03);
    LCD_Write_COM(0xD6,0x33);    LCD_Write_DATA(0x00,0xc1);



    //PAGE0
    LCD_Write_COM(0xF0,0x00);    LCD_Write_DATA(0x00,0x55);
    LCD_Write_COM(0xF0,0x01);    LCD_Write_DATA(0x00,0xAA);
    LCD_Write_COM(0xF0,0x02);    LCD_Write_DATA(0x00,0x52);
    LCD_Write_COM(0xF0,0x03);    LCD_Write_DATA(0x00,0x08);
    LCD_Write_COM(0xF0,0x04);    LCD_Write_DATA(0x00,0x00);

    LCD_Write_COM(0xb0,0x00);    LCD_Write_DATA(0x00,0x08);
    LCD_Write_COM(0xb0,0x01);    LCD_Write_DATA(0x00,0x05);
    LCD_Write_COM(0xb0,0x02);    LCD_Write_DATA(0x00,0x02);
    LCD_Write_COM(0xb0,0x03);    LCD_Write_DATA(0x00,0x05);
    LCD_Write_COM(0xb0,0x04);    LCD_Write_DATA(0x00,0x02);




    //Source hold time (Nova non-used)
    LCD_Write_COM(0xB6,0x00);    LCD_Write_DATA(0x00,0x0A);

    //Gate EQ control	 (Nova non-used)
    LCD_Write_COM(0xB7,0x00);    LCD_Write_DATA(0x00,0x00);  //HSD:70;Nova:77
    LCD_Write_COM(0xB7,0x01);    LCD_Write_DATA(0x00,0x70);	//HSD:70;Nova:77

    //Source EQ control (Nova non-used)
    LCD_Write_COM(0xB8,0x00);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xB8,0x01);    LCD_Write_DATA(0x00,0x05);	//HSD:05;Nova:07	 //03
    LCD_Write_COM(0xB8,0x02);    LCD_Write_DATA(0x00,0x05);	//HSD:05;Nova:07	  //03
    LCD_Write_COM(0xB8,0x03);    LCD_Write_DATA(0x00,0x05);	//HSD:05;Nova:07	  03


    // LCD_Write_COM(0xBa,0x00);    LCD_Write_DATA(0x00,0x01);	//HSD:05;Nova:07	  03
    //Inversion mode: column
    LCD_Write_COM(0xBC,0x00);    LCD_Write_DATA(0x00,0x00);	//00: column
    LCD_Write_COM(0xBC,0x01);    LCD_Write_DATA(0x00,0x00);	//01:1dot
    LCD_Write_COM(0xBC,0x02);    LCD_Write_DATA(0x00,0x00);




    //LGD timing control(4H/4-Delay)
    LCD_Write_COM(0xCc,0x00);    LCD_Write_DATA(0x00,0x03);	//3H:0x50;4H:0xD0	 //D
    LCD_Write_COM(0xCc,0x01);    LCD_Write_DATA(0x00,0x00);  //HSD:05;Nova:02
    LCD_Write_COM(0xCc,0x02);    LCD_Write_DATA(0x00,0x00);	//HSD:05;Nova:50

    LCD_Write_COM(0xBd,0x00);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0xBd,0x01);    LCD_Write_DATA(0x00,0x9F);	//HSD:05;Nova:07	 //03  84
    LCD_Write_COM(0xBd,0x02);    LCD_Write_DATA(0x00,0x07);	//HSD:05;Nova:07	  //03
    LCD_Write_COM(0xBd,0x03);    LCD_Write_DATA(0x00,0x31);	//HSD:05;Nova:07	  03
    LCD_Write_COM(0xBd,0x04);    LCD_Write_DATA(0x00,0x00);	//HSD:05;Nova:07	  03


    LCD_Write_COM(0xBA,0x00);    LCD_Write_DATA(0x00,0x01);	//HSD:05;Nova:07	  03


    LCD_Write_COM(0xFF,0x00);    LCD_Write_DATA(0x00,0xAA);	//HSD:05;Nova:07	  03
    LCD_Write_COM(0xFF,0x01);    LCD_Write_DATA(0x00,0x55);	//HSD:05;Nova:07	  03
    LCD_Write_COM(0xFF,0x02);    LCD_Write_DATA(0x00,0x25);	//HSD:05;Nova:07	  03
    LCD_Write_COM(0xFF,0x03);    LCD_Write_DATA(0x00,0x01);	//HSD:05;Nova:07	  03

    LCD_Write_COM(0x35,0x00);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0x44,0x00);    LCD_Write_DATA(0x00,0x01);
    LCD_Write_COM(0x44,0x01);    LCD_Write_DATA(0x00,0x20);

    LCD_Write_COM(0x36,0x00);    LCD_Write_DATA(0x00,0x00);
    LCD_Write_COM(0x3A,0x00);    LCD_Write_DATA(0x00,0x66);


    //Sleep out
    LCD_Write_COM(0x11,0x00);	   // Sleep Out (1100h)
    delayms(160);
    //Display on
    LCD_Write_COM(0x29,0x00);
    MDELAY(200);
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
    //params->dbi.te_mode                 = LCM_DBI_TE_MODE_VSYNC_ONLY;
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
#if 1
    send_ctrl_cmd(0x1100);
    MDELAY(120);
    send_ctrl_cmd(0x2900);
    MDELAY(20);
#else
    //add by xia lei jie for LCD INIT
    lcm_init();
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
    mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,1);
    MDELAY(1);

    return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}

static unsigned int lcm_compare_id(void)
{
    unsigned int id = 0;
    Lcd_Log("MYCAT lcm_compare_id\n");


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
//    send_ctrl_cmd(0xDB00);
//    id = read_data_cmd();
//    Lcd_Log("MYCAT id2 = 0x%08x\n", id);
//    id= (id<<8);
//    send_ctrl_cmd(0xDC00);
//    id |= read_data_cmd();

//    Lcd_Log("MYCAT id3 = 0x%08x\n", id);

    //return (LCM_ID == id)?1:0;
    return (LCM_ID == id&&get_lcd_id()==1)?1:0;
    //return (get_lcd_id()==1)?1:0;

}

LCM_DRIVER nt35510_jinzhengyuan_JZY397P22A_mcu18_BOE_wvga_lcm_drv =
    {
        .name			= "nt35510_jinzhengyuan_JZY397P22A_mcu18_BOE_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id     = lcm_compare_id
    };
