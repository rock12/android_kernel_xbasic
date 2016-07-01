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

#define FRAME_WIDTH  (320)
#define FRAME_HEIGHT (480)
#define LCM_ID       (0x9488)
#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN

#define __LCM_TE_ON__
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
    //MDELAY(120);
    
    send_ctrl_cmd(0x29); 

    send_ctrl_cmd(0x2C);
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
    unsigned char i,n;
    Lcd_Log("mycat LCD ili9488_UNIX23510600200_mcu18_hvga\n");    
        
    // VCI=2.8V 
    //************* Reset LCD Driver ****************// 
    //LCD_nRESET = 1; 
    //Delayms(1);            // Delay 1ms 
    //LCD_nRESET = 0; 
    //Delayms(10);            // Delay 10ms, This delay time is necessary 
    //LCD_nRESET = 1; 
    //delayms(150);            // Delay 100 ms 
    //************* Start Initial Sequence **********// 

send_ctrl_cmd(0xE0); //P-Gamma
send_data_cmd(0x00);
send_data_cmd(0x08);
send_data_cmd(0x0B);
send_data_cmd(0x03);
send_data_cmd(0x10);
send_data_cmd(0x05);
send_data_cmd(0x30);
send_data_cmd(0x45);
send_data_cmd(0x48);
send_data_cmd(0x02);
send_data_cmd(0x05);
send_data_cmd(0x01);
send_data_cmd(0x3E);
send_data_cmd(0x2E);
send_data_cmd(0x0F);

send_ctrl_cmd(0XE1); //N-Gamma
send_data_cmd(0x00);
send_data_cmd(0x02);
send_data_cmd(0x02);
send_data_cmd(0x00);
send_data_cmd(0x09);
send_data_cmd(0x03);
send_data_cmd(0x27);
send_data_cmd(0x35);
send_data_cmd(0x44);
send_data_cmd(0x06);
send_data_cmd(0x10);
send_data_cmd(0x0C);
send_data_cmd(0x29);
send_data_cmd(0x30);
send_data_cmd(0x0F);

send_ctrl_cmd(0XC0); //Power Control 1
send_data_cmd(0x13); //Vreg1out
send_data_cmd(0x13); //Verg2out

send_ctrl_cmd(0xC1); //Power Control 2
send_data_cmd(0x41); //VGH,VGL

send_ctrl_cmd(0xC5); //Power Control 3
send_data_cmd(0x00);
send_data_cmd(0x28); //Vcom 07
send_data_cmd(0x80);

#ifdef __LCM_TE_ON__
send_ctrl_cmd(0x35);
send_data_cmd(0x00);
#endif

send_ctrl_cmd(0x36); //Memory Access
send_data_cmd(0x48);

send_ctrl_cmd(0x3A); // Interface Pixel Format
send_data_cmd(0x66);

send_ctrl_cmd(0XB0); // Interface Mode Control
send_data_cmd(0x00);

send_ctrl_cmd(0xB1); //Frame rate
send_data_cmd(0xB0); //60Hz

send_ctrl_cmd(0xB4); //Display Inversion Control
send_data_cmd(0x02); //2-dot

send_ctrl_cmd(0XB6); //RGB/MCU Interface Control
send_data_cmd(0x02); //MCU
send_data_cmd(0x02); //Source,Gate scan dieection

send_ctrl_cmd(0XE9); // Set Image Function
send_data_cmd(0x00); // Disable 24 bit data input

send_ctrl_cmd(0xF7);
send_data_cmd(0xA9);
send_data_cmd(0x51);
send_data_cmd(0x2C);
send_data_cmd(0x82);


send_ctrl_cmd(0x11); //Sleep out
MDELAY(120);
send_ctrl_cmd(0x29); //Display on
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
    params->dbi.cpu_write_bits          = LCM_DBI_CPU_WRITE_16_BITS;
    params->dbi.io_driving_current      = LCM_DRIVING_CURRENT_6575_8MA;

  
    params->dbi.parallel.write_setup    = 1;
    params->dbi.parallel.write_hold     = 1;
    params->dbi.parallel.write_wait     = 4;
    params->dbi.parallel.read_setup     = 5;
    params->dbi.parallel.read_hold      = 0;
    params->dbi.parallel.read_latency   = 18;
    params->dbi.parallel.wait_period    = 4;
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
    upmu_set_rg_vgp2_vosel(5);
    upmu_set_rg_vgp2_en(1);
    MDELAY(50);

    upmu_set_rg_vgp3_vosel(3);
    upmu_set_rg_vgp3_en(1);
#else
    hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM28");
    hwPowerOn(MT6323_POWER_LDO_VGP3,VOL_1800,"LCM18");
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
    send_ctrl_cmd(0x28);
    MDELAY(120);
    send_ctrl_cmd(0x10);
    MDELAY(120);
    //send_ctrl_cmd(0xb7);
    //send_data_cmd(0x0f);
}


static void lcm_resume(void)
{
#if 0
    send_ctrl_cmd(0x11);
    MDELAY(200);
    send_ctrl_cmd(0x29);
//    sw_clear_panel(0x0);
    MDELAY(10);
#else
    lcm_init();
#endif
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
    //MDELAY(120);
    
    send_ctrl_cmd(0x29); 

    send_ctrl_cmd(0x2C);
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
    unsigned int id1,id2,id3,id4,id=0;

#ifdef BUILD_LK
    upmu_set_rg_vgp2_vosel(5);
    upmu_set_rg_vgp2_en(1);
    MDELAY(50);

    upmu_set_rg_vgp3_vosel(3);
    upmu_set_rg_vgp3_en(1);
#else
    hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM28");
    hwPowerOn(MT6323_POWER_LDO_VGP3,VOL_1800,"LCM18");
#endif

    MDELAY(100);    //MDELAY

    SET_RESET_PIN(0);
    MDELAY(200);
    SET_RESET_PIN(1);
    MDELAY(120);

    send_ctrl_cmd(0xD3);
    id1 = read_data_cmd();
    id2 = read_data_cmd();
    id3 = read_data_cmd();
    id4 = read_data_cmd();

    id=((id3<<8)&0xff00)|id4; // 0x9488,16bit ID

    Lcd_Log("mycat ili9488_UNIX23510600200 ID %s, id = 0x%x\n", __func__, id);
    return ((LCM_ID == id) && (0 == get_lcd_id()))?1:0;
}

LCM_DRIVER ili9488_UNIX23510600200_mcu18_hvga_lcm_drv =
    {
        .name            = "ili9488_UNIX23510600200_mcu18_hvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id     = lcm_compare_id
    };
