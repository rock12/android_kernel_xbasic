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

#define LCDSPI_InitCMD send_ctrl_cmd
#define LCDSPI_InitDAT send_data_cmd

static void init_lcm_registers(void)
{
#if defined(BUILD_UBOOT)
    printf("MYCAT LCD nt35510_hansen_HS40134WVNS39_00_HYDIS_mcu18_wvga");
#endif

  //PAGE1   
  LCDSPI_InitCMD(0xFF00); LCDSPI_InitDAT(0xAA);
  LCDSPI_InitCMD(0xFF01); LCDSPI_InitDAT(0x55);
  LCDSPI_InitCMD(0xFF02); LCDSPI_InitDAT(0x25);
  LCDSPI_InitCMD(0xFF03); LCDSPI_InitDAT(0x01);

  LCDSPI_InitCMD(0xF300); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xF301); LCDSPI_InitDAT(0x32);
  LCDSPI_InitCMD(0xF302); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xF303); LCDSPI_InitDAT(0x38);
  LCDSPI_InitCMD(0xF304); LCDSPI_InitDAT(0x31);
  LCDSPI_InitCMD(0xF305); LCDSPI_InitDAT(0x08);
  LCDSPI_InitCMD(0xF306); LCDSPI_InitDAT(0x11);
  LCDSPI_InitCMD(0xF307); LCDSPI_InitDAT(0x00);


// Select Command Page '0'
  LCDSPI_InitCMD(0xF000); LCDSPI_InitDAT(0x55); 
  LCDSPI_InitCMD(0xF001); LCDSPI_InitDAT(0xAA);
  LCDSPI_InitCMD(0xF002); LCDSPI_InitDAT(0x52);
  LCDSPI_InitCMD(0xF003); LCDSPI_InitDAT(0x08);
  LCDSPI_InitCMD(0xF004); LCDSPI_InitDAT(0x00);

  LCDSPI_InitCMD(0xB100); LCDSPI_InitDAT(0xFC); // Display Option Control
  LCDSPI_InitCMD(0xB101); LCDSPI_InitDAT(0x00);
  
  LCDSPI_InitCMD(0xB400); LCDSPI_InitDAT(0x10);

  LCDSPI_InitCMD(0xB600); LCDSPI_InitDAT(0x03); // Source Output Data Hold Time Control
  LCDSPI_InitCMD(0xB700); LCDSPI_InitDAT(0x70); // EQ Control Function for Gate Signals
  LCDSPI_InitCMD(0xB701); LCDSPI_InitDAT(0x70);
  LCDSPI_InitCMD(0xB800); LCDSPI_InitDAT(0x00); // EQ Control Function for Source Driver
  LCDSPI_InitCMD(0xB801); LCDSPI_InitDAT(0x06);
  LCDSPI_InitCMD(0xB802); LCDSPI_InitDAT(0x06); 
  LCDSPI_InitCMD(0xB803); LCDSPI_InitDAT(0x06);

  LCDSPI_InitCMD(0xBC00); LCDSPI_InitDAT(0x00); // EQ Control Function for Source Driver
  LCDSPI_InitCMD(0xBC01); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xBC02); LCDSPI_InitDAT(0x00);

  LCDSPI_InitCMD(0xBD00); LCDSPI_InitDAT(0x01); // Display Timing Control
  LCDSPI_InitCMD(0xBD01); LCDSPI_InitDAT(0x84);
  LCDSPI_InitCMD(0xBD02); LCDSPI_InitDAT(0x06);
  LCDSPI_InitCMD(0xBD03); LCDSPI_InitDAT(0x50);
  LCDSPI_InitCMD(0xBD04); LCDSPI_InitDAT(0x00);

  LCDSPI_InitCMD(0xCC01); LCDSPI_InitDAT(0x03); // Display Timing Control (Gateless)
  LCDSPI_InitCMD(0xCC02); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xCC03); LCDSPI_InitDAT(0x06);


// Select Command Page '1'
  LCDSPI_InitCMD(0xF000); LCDSPI_InitDAT(0x55); 
  LCDSPI_InitCMD(0xF001); LCDSPI_InitDAT(0xAA);
  LCDSPI_InitCMD(0xF002); LCDSPI_InitDAT(0x52);
  LCDSPI_InitCMD(0xF003); LCDSPI_InitDAT(0x08);
  LCDSPI_InitCMD(0xF004); LCDSPI_InitDAT(0x01);

  LCDSPI_InitCMD(0xB000); LCDSPI_InitDAT(0x05); // Setting AVDD Voltage
  LCDSPI_InitCMD(0xB001); LCDSPI_InitDAT(0x05);
  LCDSPI_InitCMD(0xB002); LCDSPI_InitDAT(0x05);

  LCDSPI_InitCMD(0xB100); LCDSPI_InitDAT(0x05); // Setting AVEE Voltage
  LCDSPI_InitCMD(0xB101); LCDSPI_InitDAT(0x05);
  LCDSPI_InitCMD(0xB102); LCDSPI_InitDAT(0x05);

  LCDSPI_InitCMD(0xB200); LCDSPI_InitDAT(0x03); // VCL Setting for LVGL
  LCDSPI_InitCMD(0xB201); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xB202); LCDSPI_InitDAT(0x03);

  LCDSPI_InitCMD(0xB800); LCDSPI_InitDAT(0x25); // Power Control for VCL
  LCDSPI_InitCMD(0xB801); LCDSPI_InitDAT(0x25);
  LCDSPI_InitCMD(0xB802); LCDSPI_InitDAT(0x25);

  LCDSPI_InitCMD(0xB300); LCDSPI_InitDAT(0x0B); // Setting VGH Voltage
  LCDSPI_InitCMD(0xB301); LCDSPI_InitDAT(0x0B);
  LCDSPI_InitCMD(0xB302); LCDSPI_InitDAT(0x0B);

  LCDSPI_InitCMD(0xB900); LCDSPI_InitDAT(0x34); // Setting VGH Voltage
  LCDSPI_InitCMD(0xB901); LCDSPI_InitDAT(0x34);
  LCDSPI_InitCMD(0xB902); LCDSPI_InitDAT(0x34);

  LCDSPI_InitCMD(0xBF00); LCDSPI_InitDAT(0x01); // VGH Output Voltage

  LCDSPI_InitCMD(0xB500); LCDSPI_InitDAT(0x08); // Setting VGL_REG Voltage
  LCDSPI_InitCMD(0xB501); LCDSPI_InitDAT(0x08);
  LCDSPI_InitCMD(0xB502); LCDSPI_InitDAT(0x08);

  LCDSPI_InitCMD(0xBA00); LCDSPI_InitDAT(0x24); // Power Control for VGLX
  LCDSPI_InitCMD(0xBA01); LCDSPI_InitDAT(0x24);
  LCDSPI_InitCMD(0xBA02); LCDSPI_InitDAT(0x24);

  LCDSPI_InitCMD(0xB400); LCDSPI_InitDAT(0x2E); // Setting VRGH Voltage
  LCDSPI_InitCMD(0xB401); LCDSPI_InitDAT(0x2E);
  LCDSPI_InitCMD(0xB402); LCDSPI_InitDAT(0x2E);

  LCDSPI_InitCMD(0xBC00); LCDSPI_InitDAT(0x00); // Setting VGMP and VGSP Voltage
  LCDSPI_InitCMD(0xBC01); LCDSPI_InitDAT(0x50);
  LCDSPI_InitCMD(0xBC02); LCDSPI_InitDAT(0x00);

  LCDSPI_InitCMD(0xBD00); LCDSPI_InitDAT(0x00); // Setting VGMN and VGSN Voltage
  LCDSPI_InitCMD(0xBD01); LCDSPI_InitDAT(0x60);
  LCDSPI_InitCMD(0xBD02); LCDSPI_InitDAT(0x00);

  LCDSPI_InitCMD(0xBE00); LCDSPI_InitDAT(0x00); // Setting VCOM Offset Voltage
  LCDSPI_InitCMD(0xBE01); LCDSPI_InitDAT(0x58);

// Select Command Page '1'
  LCDSPI_InitCMD(0xF000); LCDSPI_InitDAT(0x55); 
  LCDSPI_InitCMD(0xF001); LCDSPI_InitDAT(0xAA);
  LCDSPI_InitCMD(0xF002); LCDSPI_InitDAT(0x52);
  LCDSPI_InitCMD(0xF003); LCDSPI_InitDAT(0x01);

  LCDSPI_InitCMD(0xD000); LCDSPI_InitDAT(0x09); // Gradient Control for Gamma Voltage
  LCDSPI_InitCMD(0xD001); LCDSPI_InitDAT(0x14);
  LCDSPI_InitCMD(0xD002); LCDSPI_InitDAT(0x07);
  LCDSPI_InitCMD(0xD003); LCDSPI_InitDAT(0x0D);

//Positive Gamma for RED
  LCDSPI_InitCMD(0xD100); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD101); LCDSPI_InitDAT(0x37);
  LCDSPI_InitCMD(0xD102); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD103); LCDSPI_InitDAT(0x61);
  LCDSPI_InitCMD(0xD104); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD105); LCDSPI_InitDAT(0x92);
  LCDSPI_InitCMD(0xD106); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD107); LCDSPI_InitDAT(0xB4);
  LCDSPI_InitCMD(0xD108); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD109); LCDSPI_InitDAT(0xCF);
  LCDSPI_InitCMD(0xD10A); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD10B); LCDSPI_InitDAT(0x06);
  LCDSPI_InitCMD(0xD10C); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD10D); LCDSPI_InitDAT(0x41);
  LCDSPI_InitCMD(0xD10E); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD10F); LCDSPI_InitDAT(0x8A);
  LCDSPI_InitCMD(0xD110); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD111); LCDSPI_InitDAT(0xA6);
  LCDSPI_InitCMD(0xD112); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD113); LCDSPI_InitDAT(0xD1);
  LCDSPI_InitCMD(0xD114); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD115); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD116); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD117); LCDSPI_InitDAT(0x3D);
  LCDSPI_InitCMD(0xD118); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD119); LCDSPI_InitDAT(0x77);
  LCDSPI_InitCMD(0xD11A); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD11B); LCDSPI_InitDAT(0x79);
  LCDSPI_InitCMD(0xD11C); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD11D); LCDSPI_InitDAT(0xA5);
  LCDSPI_InitCMD(0xD11E); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD11F); LCDSPI_InitDAT(0xD1);
  LCDSPI_InitCMD(0xD120); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD121); LCDSPI_InitDAT(0xF9);
  LCDSPI_InitCMD(0xD122); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD123); LCDSPI_InitDAT(0x25);
  LCDSPI_InitCMD(0xD124); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD125); LCDSPI_InitDAT(0x43);
  LCDSPI_InitCMD(0xD126); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD127); LCDSPI_InitDAT(0x6E);
  LCDSPI_InitCMD(0xD128); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD129); LCDSPI_InitDAT(0x77);
  LCDSPI_InitCMD(0xD12A); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD12B); LCDSPI_InitDAT(0x94);
  LCDSPI_InitCMD(0xD12C); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD12D); LCDSPI_InitDAT(0x9F);
  LCDSPI_InitCMD(0xD12E); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD12F); LCDSPI_InitDAT(0xAC);
  LCDSPI_InitCMD(0xD130); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD131); LCDSPI_InitDAT(0xBA);
  LCDSPI_InitCMD(0xD132); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD133); LCDSPI_InitDAT(0xF1);

//Positive Gamma for GREEN
  LCDSPI_InitCMD(0xD200); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD201); LCDSPI_InitDAT(0x37);
  LCDSPI_InitCMD(0xD202); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD203); LCDSPI_InitDAT(0x61);
  LCDSPI_InitCMD(0xD204); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD205); LCDSPI_InitDAT(0x92);
  LCDSPI_InitCMD(0xD206); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD207); LCDSPI_InitDAT(0xB4);
  LCDSPI_InitCMD(0xD208); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD209); LCDSPI_InitDAT(0xCF);
  LCDSPI_InitCMD(0xD20A); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD20B); LCDSPI_InitDAT(0x06);
  LCDSPI_InitCMD(0xD20C); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD20D); LCDSPI_InitDAT(0x41);
  LCDSPI_InitCMD(0xD20E); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD20F); LCDSPI_InitDAT(0x8A);
  LCDSPI_InitCMD(0xD210); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD211); LCDSPI_InitDAT(0xA6);
  LCDSPI_InitCMD(0xD212); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD213); LCDSPI_InitDAT(0xD1);
  LCDSPI_InitCMD(0xD214); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD215); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD216); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD217); LCDSPI_InitDAT(0x3D);
  LCDSPI_InitCMD(0xD218); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD219); LCDSPI_InitDAT(0x77);
  LCDSPI_InitCMD(0xD21A); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD21B); LCDSPI_InitDAT(0x79);
  LCDSPI_InitCMD(0xD21C); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD21D); LCDSPI_InitDAT(0xA5);
  LCDSPI_InitCMD(0xD21E); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD21F); LCDSPI_InitDAT(0xD1);
  LCDSPI_InitCMD(0xD220); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD221); LCDSPI_InitDAT(0xF9);
  LCDSPI_InitCMD(0xD222); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD223); LCDSPI_InitDAT(0x25);
  LCDSPI_InitCMD(0xD224); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD225); LCDSPI_InitDAT(0x43);
  LCDSPI_InitCMD(0xD226); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD227); LCDSPI_InitDAT(0x6E);
  LCDSPI_InitCMD(0xD228); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD229); LCDSPI_InitDAT(0x77);
  LCDSPI_InitCMD(0xD22A); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD22B); LCDSPI_InitDAT(0x94);
  LCDSPI_InitCMD(0xD22C); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD22D); LCDSPI_InitDAT(0x9F);
  LCDSPI_InitCMD(0xD22E); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD22F); LCDSPI_InitDAT(0xAC);
  LCDSPI_InitCMD(0xD230); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD231); LCDSPI_InitDAT(0xBA);
  LCDSPI_InitCMD(0xD232); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD233); LCDSPI_InitDAT(0xF1);


//Positive Gamma for BLUE
  LCDSPI_InitCMD(0xD300); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD301); LCDSPI_InitDAT(0x37);
  LCDSPI_InitCMD(0xD302); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD303); LCDSPI_InitDAT(0x61);
  LCDSPI_InitCMD(0xD304); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD305); LCDSPI_InitDAT(0x92);
  LCDSPI_InitCMD(0xD306); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD307); LCDSPI_InitDAT(0xB4);
  LCDSPI_InitCMD(0xD308); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD309); LCDSPI_InitDAT(0xCF);
  LCDSPI_InitCMD(0xD30A); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD30B); LCDSPI_InitDAT(0x06);
  LCDSPI_InitCMD(0xD30C); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD30D); LCDSPI_InitDAT(0x41);
  LCDSPI_InitCMD(0xD30E); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD30F); LCDSPI_InitDAT(0x8A);
  LCDSPI_InitCMD(0xD310); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD311); LCDSPI_InitDAT(0xA6);
  LCDSPI_InitCMD(0xD312); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD313); LCDSPI_InitDAT(0xD1);
  LCDSPI_InitCMD(0xD314); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD315); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD316); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD317); LCDSPI_InitDAT(0x3D);
  LCDSPI_InitCMD(0xD318); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD319); LCDSPI_InitDAT(0x77);
  LCDSPI_InitCMD(0xD31A); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD31B); LCDSPI_InitDAT(0x79);
  LCDSPI_InitCMD(0xD31C); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD31D); LCDSPI_InitDAT(0xA5);
  LCDSPI_InitCMD(0xD31E); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD31F); LCDSPI_InitDAT(0xD1);
  LCDSPI_InitCMD(0xD320); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD321); LCDSPI_InitDAT(0xF9);
  LCDSPI_InitCMD(0xD322); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD323); LCDSPI_InitDAT(0x25);
  LCDSPI_InitCMD(0xD324); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD325); LCDSPI_InitDAT(0x43);
  LCDSPI_InitCMD(0xD326); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD327); LCDSPI_InitDAT(0x6E);
  LCDSPI_InitCMD(0xD328); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD329); LCDSPI_InitDAT(0x77);
  LCDSPI_InitCMD(0xD32A); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD32B); LCDSPI_InitDAT(0x94);
  LCDSPI_InitCMD(0xD32C); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD32D); LCDSPI_InitDAT(0x9F);
  LCDSPI_InitCMD(0xD32E); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD32F); LCDSPI_InitDAT(0xAC);
  LCDSPI_InitCMD(0xD330); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD331); LCDSPI_InitDAT(0xBA);
  LCDSPI_InitCMD(0xD332); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD333); LCDSPI_InitDAT(0xF1);



//Negative Gamma for RED
  LCDSPI_InitCMD(0xD400); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD401); LCDSPI_InitDAT(0x37);
  LCDSPI_InitCMD(0xD402); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD403); LCDSPI_InitDAT(0x50);
  LCDSPI_InitCMD(0xD404); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD405); LCDSPI_InitDAT(0x89);
  LCDSPI_InitCMD(0xD406); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD407); LCDSPI_InitDAT(0xA9);
  LCDSPI_InitCMD(0xD408); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD409); LCDSPI_InitDAT(0xC0);
  LCDSPI_InitCMD(0xD40A); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD40B); LCDSPI_InitDAT(0x06);
  LCDSPI_InitCMD(0xD40C); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD40D); LCDSPI_InitDAT(0x26);
  LCDSPI_InitCMD(0xD40E); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD40F); LCDSPI_InitDAT(0x54);
  LCDSPI_InitCMD(0xD410); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD411); LCDSPI_InitDAT(0x79);
  LCDSPI_InitCMD(0xD412); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD413); LCDSPI_InitDAT(0xB8);
  LCDSPI_InitCMD(0xD414); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD415); LCDSPI_InitDAT(0xDF);
  LCDSPI_InitCMD(0xD416); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD417); LCDSPI_InitDAT(0x2F);
  LCDSPI_InitCMD(0xD418); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD419); LCDSPI_InitDAT(0x68);
  LCDSPI_InitCMD(0xD41A); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD41B); LCDSPI_InitDAT(0x6A);
  LCDSPI_InitCMD(0xD41C); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD41D); LCDSPI_InitDAT(0xA3);
  LCDSPI_InitCMD(0xD41E); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD41F); LCDSPI_InitDAT(0xE0);
  LCDSPI_InitCMD(0xD420); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD421); LCDSPI_InitDAT(0xF9);
  LCDSPI_InitCMD(0xD422); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD423); LCDSPI_InitDAT(0x25);
  LCDSPI_InitCMD(0xD424); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD425); LCDSPI_InitDAT(0x43);
  LCDSPI_InitCMD(0xD426); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD427); LCDSPI_InitDAT(0x6E);
  LCDSPI_InitCMD(0xD428); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD429); LCDSPI_InitDAT(0x77);
  LCDSPI_InitCMD(0xD42A); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD42B); LCDSPI_InitDAT(0x94);
  LCDSPI_InitCMD(0xD42C); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD42D); LCDSPI_InitDAT(0x9F);
  LCDSPI_InitCMD(0xD42E); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD42F); LCDSPI_InitDAT(0xAC);
  LCDSPI_InitCMD(0xD430); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD431); LCDSPI_InitDAT(0xBA);
  LCDSPI_InitCMD(0xD432); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD433); LCDSPI_InitDAT(0xF1);



//Negative Gamma for GERREN
  LCDSPI_InitCMD(0xD500); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD501); LCDSPI_InitDAT(0x37);
  LCDSPI_InitCMD(0xD502); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD503); LCDSPI_InitDAT(0x50);
  LCDSPI_InitCMD(0xD504); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD505); LCDSPI_InitDAT(0x89);
  LCDSPI_InitCMD(0xD506); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD507); LCDSPI_InitDAT(0xA9);
  LCDSPI_InitCMD(0xD508); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD509); LCDSPI_InitDAT(0xC0);
  LCDSPI_InitCMD(0xD50A); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD50B); LCDSPI_InitDAT(0x06);
  LCDSPI_InitCMD(0xD50C); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD50D); LCDSPI_InitDAT(0x26);
  LCDSPI_InitCMD(0xD50E); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD50F); LCDSPI_InitDAT(0x54);
  LCDSPI_InitCMD(0xD510); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD511); LCDSPI_InitDAT(0x79);
  LCDSPI_InitCMD(0xD512); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD513); LCDSPI_InitDAT(0xB8);
  LCDSPI_InitCMD(0xD514); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD515); LCDSPI_InitDAT(0xDF);
  LCDSPI_InitCMD(0xD516); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD517); LCDSPI_InitDAT(0x2F);
  LCDSPI_InitCMD(0xD518); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD519); LCDSPI_InitDAT(0x68);
  LCDSPI_InitCMD(0xD51A); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD51B); LCDSPI_InitDAT(0x6A);
  LCDSPI_InitCMD(0xD51C); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD51D); LCDSPI_InitDAT(0xA3);
  LCDSPI_InitCMD(0xD51E); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD51F); LCDSPI_InitDAT(0xE0);
  LCDSPI_InitCMD(0xD520); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD521); LCDSPI_InitDAT(0xF9);
  LCDSPI_InitCMD(0xD522); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD523); LCDSPI_InitDAT(0x25);
  LCDSPI_InitCMD(0xD524); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD525); LCDSPI_InitDAT(0x43);
  LCDSPI_InitCMD(0xD526); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD527); LCDSPI_InitDAT(0x6E);
  LCDSPI_InitCMD(0xD528); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD529); LCDSPI_InitDAT(0x77);
  LCDSPI_InitCMD(0xD52A); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD52B); LCDSPI_InitDAT(0x94);
  LCDSPI_InitCMD(0xD52C); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD52D); LCDSPI_InitDAT(0x9F);
  LCDSPI_InitCMD(0xD52E); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD52F); LCDSPI_InitDAT(0xAC);
  LCDSPI_InitCMD(0xD530); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD531); LCDSPI_InitDAT(0xBA);
  LCDSPI_InitCMD(0xD532); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD533); LCDSPI_InitDAT(0xF1);


//Negative Gamma for BLUE
  LCDSPI_InitCMD(0xD600); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD601); LCDSPI_InitDAT(0x37);
  LCDSPI_InitCMD(0xD602); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD603); LCDSPI_InitDAT(0x50);
  LCDSPI_InitCMD(0xD604); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD605); LCDSPI_InitDAT(0x89);
  LCDSPI_InitCMD(0xD606); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD607); LCDSPI_InitDAT(0xA9);
  LCDSPI_InitCMD(0xD608); LCDSPI_InitDAT(0x00);
  LCDSPI_InitCMD(0xD609); LCDSPI_InitDAT(0xC0);
  LCDSPI_InitCMD(0xD60A); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD60B); LCDSPI_InitDAT(0x06);
  LCDSPI_InitCMD(0xD60C); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD60D); LCDSPI_InitDAT(0x26);
  LCDSPI_InitCMD(0xD60E); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD60F); LCDSPI_InitDAT(0x54);
  LCDSPI_InitCMD(0xD610); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD611); LCDSPI_InitDAT(0x79);
  LCDSPI_InitCMD(0xD612); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD613); LCDSPI_InitDAT(0xB8);
  LCDSPI_InitCMD(0xD614); LCDSPI_InitDAT(0x01);
  LCDSPI_InitCMD(0xD615); LCDSPI_InitDAT(0xDF);
  LCDSPI_InitCMD(0xD616); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD617); LCDSPI_InitDAT(0x2F);
  LCDSPI_InitCMD(0xD618); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD619); LCDSPI_InitDAT(0x68);
  LCDSPI_InitCMD(0xD61A); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD61B); LCDSPI_InitDAT(0x6A);
  LCDSPI_InitCMD(0xD61C); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD61D); LCDSPI_InitDAT(0xA3);
  LCDSPI_InitCMD(0xD61E); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD61F); LCDSPI_InitDAT(0xE0);
  LCDSPI_InitCMD(0xD620); LCDSPI_InitDAT(0x02);
  LCDSPI_InitCMD(0xD621); LCDSPI_InitDAT(0xF9);
  LCDSPI_InitCMD(0xD622); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD623); LCDSPI_InitDAT(0x25);
  LCDSPI_InitCMD(0xD624); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD625); LCDSPI_InitDAT(0x43);
  LCDSPI_InitCMD(0xD626); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD627); LCDSPI_InitDAT(0x6E);
  LCDSPI_InitCMD(0xD628); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD629); LCDSPI_InitDAT(0x77);
  LCDSPI_InitCMD(0xD62A); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD62B); LCDSPI_InitDAT(0x94);
  LCDSPI_InitCMD(0xD62C); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD62D); LCDSPI_InitDAT(0x9F);
  LCDSPI_InitCMD(0xD62E); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD62F); LCDSPI_InitDAT(0xAC);
  LCDSPI_InitCMD(0xD630); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD631); LCDSPI_InitDAT(0xBA);
  LCDSPI_InitCMD(0xD632); LCDSPI_InitDAT(0x03);
  LCDSPI_InitCMD(0xD633); LCDSPI_InitDAT(0xF1);

  LCDSPI_InitCMD(0x3A00); LCDSPI_InitDAT(0x66); //Data Format
  
  LCDSPI_InitCMD(0x1100); // Sleep out
  MDELAY(120);
  LCDSPI_InitCMD(0x2900); // Display On


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
#if 0
    send_ctrl_cmd(0x1100);
    MDELAY(200);
    send_ctrl_cmd(0x2900);
#else
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



    return (LCM_ID == id&&get_lcd_id()==0)?1:0;
}

LCM_DRIVER nt35510_hansen_HS40134WVNS39_00_HYDIS_mcu18_wvga_lcm_drv =
    {
        .name			= "nt35510_hansen_HS40134WVNS39_00_HYDIS_mcu18_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id     = lcm_compare_id
    };
