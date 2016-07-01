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

#define GPIO_LCD_ID_PIN 50
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (800)
#define LCM_ID       (0x69)

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

static __inline void send_ctrl_cmd(unsigned int cmd)
{
    lcm_util.send_cmd(cmd);
}

static __inline void send_data_cmd(unsigned int data)
{
    lcm_util.send_data(data&0xff);
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

    send_ctrl_cmd(0x29);

    send_ctrl_cmd(0x2C);
    for (y = y0; y <= y1; ++ y) {
        for (x = x0; x <= x1; ++ x) {
            lcm_util.send_data(color);
        }
    }
}
static unsigned int lcm_compare_id(void)
{
    unsigned int id = 0,ID_PIN =2;
    send_ctrl_cmd(0xB9);  // SET password
    send_data_cmd(0xFF);
    send_data_cmd(0x83);
    send_data_cmd(0x69);
    send_ctrl_cmd(0xC3);
    send_data_cmd(0xFF);

    send_ctrl_cmd(0xF4);
    read_data_cmd();
    id = read_data_cmd();
	
    Lcd_Log("xxxxx MYCAT hx8369 READ ID = 0x%x\n", id);
	
    mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
    mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,1);
    MDELAY(1);

    ID_PIN=mt_get_gpio_in(GPIO_LCD_ID_PIN);

    Lcd_Log("xxxxx MYCAT ID_PIN = 0x%x\n", ID_PIN);

    return ((LCM_ID == id)&&(ID_PIN==1))?1:0;
}

#define WMLCDCOM  send_ctrl_cmd
#define WMLCDDATA send_data_cmd
#define Delayms   MDELAY


static void init_lcm_registers(void)
{
    unsigned int i = 0;
    WMLCDCOM(0xB9); // SET password
    WMLCDDATA(0xFF);
    WMLCDDATA(0x83);
    WMLCDDATA(0x69);
    Delayms(10);

    WMLCDCOM(0xB2); // SET Display related register
    WMLCDDATA(0x00);
    WMLCDDATA(0x20);//2b---RGB
    WMLCDDATA(0x03);
    WMLCDDATA(0x03);
    WMLCDDATA(0x70);
    WMLCDDATA(0x00);
    WMLCDDATA(0xff);
    WMLCDDATA(0x00);
    WMLCDDATA(0x00);
    WMLCDDATA(0x00);
    WMLCDDATA(0x00);
    WMLCDDATA(0x03);
    WMLCDDATA(0x03);
    WMLCDDATA(0x00);
    WMLCDDATA(0x01);

    WMLCDCOM(0xB4); // SET Display column inversion
    WMLCDDATA(0x00);//
    WMLCDDATA(0x1d);
    WMLCDDATA(0x80);
    WMLCDDATA(0x06);
    WMLCDDATA(0x02);


    WMLCDCOM(0x36);
    WMLCDDATA(0x00);//ÆÁÐý×ª180¶ÈÊ¹ÓÃD0,Õý³£Ê¹ÓÃ00

    WMLCDCOM(0xB6); //VCOM Setting
    WMLCDDATA(0x40);//3e adjust flicker
    WMLCDDATA(0x40);//3e adjust flicker


    WMLCDCOM(0xD5);  // Set GIP
    WMLCDDATA(0x00);
    WMLCDDATA(0x01);
    WMLCDDATA(0x03);
    WMLCDDATA(0x25);
    WMLCDDATA(0x01);
    WMLCDDATA(0x02);
    WMLCDDATA(0x28);
    WMLCDDATA(0x70);
    WMLCDDATA(0x11);
    WMLCDDATA(0x13);
    WMLCDDATA(0x00);
    WMLCDDATA(0x00);
    WMLCDDATA(0x40);
    WMLCDDATA(0x26);
    WMLCDDATA(0x51);
    WMLCDDATA(0x37);
    WMLCDDATA(0x00);
    WMLCDDATA(0x00);
    WMLCDDATA(0x71);
    WMLCDDATA(0x35);
    WMLCDDATA(0x60);
    WMLCDDATA(0x24);
    WMLCDDATA(0x07);
    WMLCDDATA(0x0f);
    WMLCDDATA(0x04);
    WMLCDDATA(0x04);
    Delayms(10);

    WMLCDCOM(0xB1); //Set Power
    WMLCDDATA(0x01);
    WMLCDDATA(0x00);
    WMLCDDATA(0x34);
    WMLCDDATA(0x03);
    WMLCDDATA(0x00);
    WMLCDDATA(0x11);
    WMLCDDATA(0x11);
    WMLCDDATA(0x32);
    WMLCDDATA(0x2f);
    WMLCDDATA(0x3f);
    WMLCDDATA(0x3f);
    WMLCDDATA(0x01);
    WMLCDDATA(0x3a);
    WMLCDDATA(0x01);
    WMLCDDATA(0xe6);
    WMLCDDATA(0xE6);
    WMLCDDATA(0xE6);
    WMLCDDATA(0xE6);
    WMLCDDATA(0xE6);
    Delayms(10);

    WMLCDCOM(0xCC); // SET panel
    WMLCDDATA(0x02); //normal black
    Delayms(10);

    WMLCDCOM(0xE0);	// Set Gamma 2.2
    WMLCDDATA(0x00);	//G1_VRP0[5:0]
    WMLCDDATA(0x11);	//G1_VRP1[5:0] WMLCDDATA(0x0f)
    WMLCDDATA(0x1b);	//G1_VRP2[5:0] )WMLCDDATA(0x0f)
    WMLCDDATA(0x35);	//G1_VRP3[5:0]
    WMLCDDATA(0x3b);	//G1_VRP4[5:0]
    WMLCDDATA(0x3F);	//G1_VRP5[5:0]
    WMLCDDATA(0x31);	//G1_PRP0[6:0] ;WMLCDDATA(0x0c)
    WMLCDDATA(0x51);	//G1_PRP1[6:0]
    WMLCDDATA(0x07); //G1_CGMP0[1:0]	  G1_PKP0[4:0]
    WMLCDDATA(0x0d); //G1_CGMP1[1:0]	  G1_PKP1[4:0]
    WMLCDDATA(0x10); //G1_CGMP2[1:0]	  G1_PKP2[4:0]
    WMLCDDATA(0x14); //G1_CGMP3[1:0]	  G1_PKP3[4:0]
    WMLCDDATA(0x16); //G1_CGMP4,G1_CGMP5	  G1_PKP4[4:0]
    WMLCDDATA(0x13);	//G1_PKP5[4:0]
    WMLCDDATA(0x14);	//G1_PKP6[4:0]
    WMLCDDATA(0x19);	//G1_PKP7[4:0]
    WMLCDDATA(0x1f);	//G1_PKP8[4:0]
    WMLCDDATA(0x00);	//G1_VRP0[5:0]
    WMLCDDATA(0x11);	//G1_VRP1[5:0] WMLCDDATA(0x0f)
    WMLCDDATA(0x1b);	//G1_VRP2[5:0] )WMLCDDATA(0x0f)
    WMLCDDATA(0x35);	//G1_VRP3[5:0]
    WMLCDDATA(0x3b);	//G1_VRP4[5:0]
    WMLCDDATA(0x3F);	//G1_VRP5[5:0]
    WMLCDDATA(0x31);	//G1_PRP0[6:0] ;WMLCDDATA(0x0c)
    WMLCDDATA(0x51);	//G1_PRP1[6:0]
    WMLCDDATA(0x07); //G1_CGMP0[1:0]	  G1_PKP0[4:0]
    WMLCDDATA(0x0d); //G1_CGMP1[1:0]	  G1_PKP1[4:0]
    WMLCDDATA(0x10); //G1_CGMP2[1:0]	  G1_PKP2[4:0]
    WMLCDDATA(0x14); //G1_CGMP3[1:0]	  G1_PKP3[4:0]
    WMLCDDATA(0x16); //G1_CGMP4,G1_CGMP5	  G1_PKP4[4:0]
    WMLCDDATA(0x13);	//G1_PKP5[4:0]
    WMLCDDATA(0x14);	//G1_PKP6[4:0]
    WMLCDDATA(0x19);	//G1_PKP7[4:0]
    WMLCDDATA(0x1f);	//G1_PKP8[4:0]
    Delayms(10);

#if 0
	////////////////////16bits///////////////////
			 send_ctrl_cmd(0x2d);//SET LUT
			 for(i=0;i<=63;i++)
			  send_data_cmd(i*8);
			 for(i=0;i<=63;i++)
			  send_data_cmd(i*4);
			  for(i=0;i<=63;i++)
			  send_data_cmd(i*8);
#else
	
	////////////////////18bits///////////////////
	 send_ctrl_cmd(0x2D); //Look up table// 
		 
	 for(i=0;i<64;i++)
	   send_data_cmd(4*i);//RED  
	 for(i=0;i<64;i++)
	   send_data_cmd(4*i);//Green 
	 for(i=0;i<64;i++)
	   send_data_cmd(4*i);//Blue*/
#endif	  
	 
	
	MDELAY(10);


    WMLCDCOM(0x3A); //SET COLMOD
    WMLCDDATA(0x66);//77-24bit,55-16bit MCU

    WMLCDCOM(0x35);
    WMLCDDATA(0x00);//TE ON

    Delayms(10);
    WMLCDCOM(0x11);
    Delayms(120);

    WMLCDCOM(0x29);
    Delayms(100);
    WMLCDCOM(0x2C);


}




void MainLCD_Window_Set(UINT16 sax, UINT16 say, UINT16 eax, UINT16 eay)
{
    WMLCDCOM(0x2A);
    WMLCDDATA(sax&0xff00>>8);
    WMLCDDATA(sax&0x00ff);		//X·\u0153Ïò¿ªÊ\u0152µã
    WMLCDDATA(eax&0xff00>>8);
    WMLCDDATA(eax&0x00ff);		//X·\u0153Ïò\u0153áÊøµã

    WMLCDCOM(0x2B);
    WMLCDDATA(say&0xff00>>8);
    WMLCDDATA(say&0x00ff);		//Y·\u0153Ïò¿ªÊ\u0152µã
    WMLCDDATA(eay&0xff00>>8);	//Y·\u0153Ïò\u0153áÊøµã
    WMLCDDATA(eay&0x00ff);

    WMLCDCOM(0x2C);


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
    params->io_select_mode = 1;

    params->dbi.port                    = 0;
    params->dbi.data_width              = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dbi.data_format.trans_seq   = LCM_DBI_TRANS_SEQ_MSB_FIRST;
    params->dbi.data_format.padding     = LCM_DBI_PADDING_ON_LSB;
    params->dbi.data_format.format      = LCM_DBI_FORMAT_RGB666;
    params->dbi.data_format.width       = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.cpu_write_bits          = LCM_DBI_CPU_WRITE_16_BITS;
    params->dbi.io_driving_current      = LCM_DRIVING_CURRENT_6575_8MA;

    params->dbi.io_driving_current      = 0;

    params->dbi.parallel.write_setup    = 0;
    params->dbi.parallel.write_hold     = 0;
    params->dbi.parallel.write_wait     = 2;
    params->dbi.parallel.read_setup     = 3;
    params->dbi.parallel.read_latency   = 15;
    params->dbi.parallel.wait_period    = 2;

    // enable tearing-free
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
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(150);
//sw_clear_panel(0X00);
    init_lcm_registers();
    sw_clear_panel(0x00);
}


static void lcm_suspend(void)
{
    send_ctrl_cmd(0x0028);
    MDELAY(20);
    send_ctrl_cmd(0x0010);
    MDELAY(200);
}


static void lcm_resume(void)
{
#if 1
		send_ctrl_cmd(0x1100);
		MDELAY(120);
		send_ctrl_cmd(0x2900);
		MDELAY(50);
#else
		//add by xia lei jie for LCD INIT
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

    send_ctrl_cmd(0x29);

    send_ctrl_cmd(0x2C);
}
// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER hx8369a_hengsheng_HS040IH_01E_TM_mcu18_wvga_lcm_drv =
    {
        .name			= "hx8369a_hengsheng_HS040IH_01E_TM_mcu18_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id    = lcm_compare_id,
    };


