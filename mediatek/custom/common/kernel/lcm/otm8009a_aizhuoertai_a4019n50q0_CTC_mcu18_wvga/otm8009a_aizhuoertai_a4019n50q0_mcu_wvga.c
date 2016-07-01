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
static unsigned char  temk=0;
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



#define LCD_WRITE_COMMAND	send_ctrl_cmd
#define LCD_WRITE_DATA 	send_data_cmd
#define DelayMS MDELAY
#define Delayms MDELAY

static void init_lcm_registers(void)
{
    int i;

#if defined(BUILD_LK)
        printf("mycat LCD otm8009a_aizhuoertai_a4019n50q0_mcu_wvga \n");
#endif


        LCD_WRITE_COMMAND(0xff00); LCD_WRITE_DATA(0x80);
        LCD_WRITE_COMMAND(0xff01); LCD_WRITE_DATA(0x09);     //enable EXTC
        LCD_WRITE_COMMAND(0xff02); LCD_WRITE_DATA(0x01);
        LCD_WRITE_COMMAND(0xff80); LCD_WRITE_DATA(0x80);     //enable Orise mode
        LCD_WRITE_COMMAND(0xff81); LCD_WRITE_DATA(0x09);
        LCD_WRITE_COMMAND(0xff03); LCD_WRITE_DATA(0x01);     //enable SPI+I2C cmd2 read



        LCD_WRITE_COMMAND(0xf580); LCD_WRITE_DATA(0x01);
        LCD_WRITE_COMMAND(0xf581); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf582); LCD_WRITE_DATA(0x02);
        LCD_WRITE_COMMAND(0xf583); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf584); LCD_WRITE_DATA(0x10);
        LCD_WRITE_COMMAND(0xf585); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf586); LCD_WRITE_DATA(0x02);
        LCD_WRITE_COMMAND(0xf587); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf588); LCD_WRITE_DATA(0x0e);
        LCD_WRITE_COMMAND(0xf589); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf58A); LCD_WRITE_DATA(0x0f);
        LCD_WRITE_COMMAND(0xf58B); LCD_WRITE_DATA(0x20);


        LCD_WRITE_COMMAND(0xf590); LCD_WRITE_DATA(0x02);
        LCD_WRITE_COMMAND(0xf591); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf592); LCD_WRITE_DATA(0x08);
        LCD_WRITE_COMMAND(0xf593); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf594); LCD_WRITE_DATA(0x06);
        LCD_WRITE_COMMAND(0xf595); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf596); LCD_WRITE_DATA(0x0d);
        LCD_WRITE_COMMAND(0xf597); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf598); LCD_WRITE_DATA(0x0b);
        LCD_WRITE_COMMAND(0xf599); LCD_WRITE_DATA(0x18);




        LCD_WRITE_COMMAND(0xf5a0); LCD_WRITE_DATA(0x10);
        LCD_WRITE_COMMAND(0xf5a1); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf5a2); LCD_WRITE_DATA(0x01);
        LCD_WRITE_COMMAND(0xf5a3); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf5a4); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf5a5); LCD_WRITE_DATA(0x14);
        LCD_WRITE_COMMAND(0xf5a6); LCD_WRITE_DATA(0x14);
        LCD_WRITE_COMMAND(0xf5a7); LCD_WRITE_DATA(0x18);




        LCD_WRITE_COMMAND(0xf5b0); LCD_WRITE_DATA(0x14);
        LCD_WRITE_COMMAND(0xf5b1); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf5b2); LCD_WRITE_DATA(0x12);
        LCD_WRITE_COMMAND(0xf5b3); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf5b4); LCD_WRITE_DATA(0x13);
        LCD_WRITE_COMMAND(0xf5b5); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf5b6); LCD_WRITE_DATA(0x11);
        LCD_WRITE_COMMAND(0xf5b7); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf5b8); LCD_WRITE_DATA(0x13);
        LCD_WRITE_COMMAND(0xf5b9); LCD_WRITE_DATA(0x18);
        LCD_WRITE_COMMAND(0xf5bA); LCD_WRITE_DATA(0x00);
        LCD_WRITE_COMMAND(0xf5bB); LCD_WRITE_DATA(0x00);





//gamma DC
        LCD_WRITE_COMMAND(0xc0b4); LCD_WRITE_DATA(0x00);     //column inversion  0x50  0x00 0x01

        LCD_WRITE_COMMAND(0xC0a3); LCD_WRITE_DATA(0x1b);     //pre-charge //V02
        LCD_WRITE_COMMAND(0xC582); LCD_WRITE_DATA(0xA3);     //REG-pump23
        LCD_WRITE_COMMAND(0xC590); LCD_WRITE_DATA(0xd6);     //Pump setting (3x=D6)-->(2x=96)//v02 01/11
        LCD_WRITE_COMMAND(0xC591); LCD_WRITE_DATA(0x76);     //Pump setting(VGH/VGL)
        LCD_WRITE_COMMAND(0xD800); LCD_WRITE_DATA(0x95);     //GVDD=5V
        LCD_WRITE_COMMAND(0xD801); LCD_WRITE_DATA(0x95);     //NGVDD=5V

//VCOMDC
        LCD_WRITE_COMMAND(0xD900); LCD_WRITE_DATA(0x6c);     // VCOMDC=
        Delayms(20);

        LCD_WRITE_COMMAND(0xC181); LCD_WRITE_DATA(0x66);     //Frame rate 65Hz//V02

// RGB I/F setting VSYNC for OTM8018 0x0e
        LCD_WRITE_COMMAND(0xC1a1); LCD_WRITE_DATA(0x08);     //external Vsync,Hsync,DE
        LCD_WRITE_COMMAND(0xC0a3); LCD_WRITE_DATA(0x1b);     //pre-charge //V02
        LCD_WRITE_COMMAND(0xC481); LCD_WRITE_DATA(0x83);     //source bias //V02
        LCD_WRITE_COMMAND(0xC592); LCD_WRITE_DATA(0x01);     //Pump45
        LCD_WRITE_COMMAND(0xC5B1); LCD_WRITE_DATA(0xA9);     //DC voltage setting ;[0]GVDD output, default: 0xa8



// Positive
        LCD_WRITE_COMMAND(0xE100); LCD_WRITE_DATA(0x00);
        LCD_WRITE_COMMAND(0xE101); LCD_WRITE_DATA(0x0b);
        LCD_WRITE_COMMAND(0xE102); LCD_WRITE_DATA(0x0f);
        LCD_WRITE_COMMAND(0xE103); LCD_WRITE_DATA(0x10);
        LCD_WRITE_COMMAND(0xE104); LCD_WRITE_DATA(0x09);
        LCD_WRITE_COMMAND(0xE105); LCD_WRITE_DATA(0x12);
        LCD_WRITE_COMMAND(0xE106); LCD_WRITE_DATA(0x0a);
        LCD_WRITE_COMMAND(0xE107); LCD_WRITE_DATA(0x06);
        LCD_WRITE_COMMAND(0xE108); LCD_WRITE_DATA(0x07);
        LCD_WRITE_COMMAND(0xE109); LCD_WRITE_DATA(0x07);
        LCD_WRITE_COMMAND(0xE10A); LCD_WRITE_DATA(0x0a);
        LCD_WRITE_COMMAND(0xE10B); LCD_WRITE_DATA(0x05);
        LCD_WRITE_COMMAND(0xE10C); LCD_WRITE_DATA(0x1d);
        LCD_WRITE_COMMAND(0xE10D); LCD_WRITE_DATA(0x17);
        LCD_WRITE_COMMAND(0xE10E); LCD_WRITE_DATA(0x0d);
        LCD_WRITE_COMMAND(0xE10F); LCD_WRITE_DATA(0x06);

// Negative
        LCD_WRITE_COMMAND(0xE200); LCD_WRITE_DATA(0x00);
        LCD_WRITE_COMMAND(0xE201); LCD_WRITE_DATA(0x0b);
        LCD_WRITE_COMMAND(0xE202); LCD_WRITE_DATA(0x0f);
        LCD_WRITE_COMMAND(0xE203); LCD_WRITE_DATA(0x10);
        LCD_WRITE_COMMAND(0xE204); LCD_WRITE_DATA(0x0a);
        LCD_WRITE_COMMAND(0xE205); LCD_WRITE_DATA(0x11);
        LCD_WRITE_COMMAND(0xE206); LCD_WRITE_DATA(0x0e);
        LCD_WRITE_COMMAND(0xE207); LCD_WRITE_DATA(0x0e);
        LCD_WRITE_COMMAND(0xE208); LCD_WRITE_DATA(0x01);
        LCD_WRITE_COMMAND(0xE209); LCD_WRITE_DATA(0x05);
        LCD_WRITE_COMMAND(0xE20A); LCD_WRITE_DATA(0x0c);
        LCD_WRITE_COMMAND(0xE20B); LCD_WRITE_DATA(0x0a);
        LCD_WRITE_COMMAND(0xE20C); LCD_WRITE_DATA(0x13);
        LCD_WRITE_COMMAND(0xE20D); LCD_WRITE_DATA(0x0d);
        LCD_WRITE_COMMAND(0xE20E); LCD_WRITE_DATA(0x09);
        LCD_WRITE_COMMAND(0xE20F); LCD_WRITE_DATA(0x01);






// CE8x : vst1, vst2, vst3, vst4
        LCD_WRITE_COMMAND(0xCE80); LCD_WRITE_DATA(0x84);	// ce81[7:0] : vst1_shift[7:0]
        LCD_WRITE_COMMAND(0xCE81); LCD_WRITE_DATA(0x03);	// ce82[7:0] : 0000,	vst1_width[3:0]
        LCD_WRITE_COMMAND(0xCE82); LCD_WRITE_DATA(0x00);	// ce83[7:0] : vst1_tchop[7:0]
        LCD_WRITE_COMMAND(0xCE83); LCD_WRITE_DATA(0x83);	// ce84[7:0] : vst2_shift[7:0]
        LCD_WRITE_COMMAND(0xCE84); LCD_WRITE_DATA(0x03);	// ce85[7:0] : 0000,	vst2_width[3:0]
        LCD_WRITE_COMMAND(0xCE85); LCD_WRITE_DATA(0x00);	// ce86[7:0] : vst2_tchop[7:0]

// CE8x : vst1, vst2, vst3, vst4
        LCD_WRITE_COMMAND(0xCE90); LCD_WRITE_DATA(0x33);	// ce81[7:0] : vst1_shift[7:0]
        LCD_WRITE_COMMAND(0xCE91); LCD_WRITE_DATA(0x27);	// ce82[7:0] : 0000,	vst1_width[3:0]
        LCD_WRITE_COMMAND(0xCE92); LCD_WRITE_DATA(0x00);	// ce83[7:0] : vst1_tchop[7:0]
        LCD_WRITE_COMMAND(0xCE93); LCD_WRITE_DATA(0x33);	// ce84[7:0] : vst2_shift[7:0]
        LCD_WRITE_COMMAND(0xCE94); LCD_WRITE_DATA(0x28);	// ce85[7:0] : 0000,	vst2_width[3:0]
        LCD_WRITE_COMMAND(0xCE95); LCD_WRITE_DATA(0x00);	// ce86[7:0] : vst2_tchop[7:0]

//CEAx : clka1, clka2
        LCD_WRITE_COMMAND(0xCEa0); LCD_WRITE_DATA(0x38);	// cea1[7:0] : clka1_width[3:0], clka1_shift[11:8]
        LCD_WRITE_COMMAND(0xCEa1); LCD_WRITE_DATA(0x02);	// cea2[7:0] : clka1_shift[7:0]
        LCD_WRITE_COMMAND(0xCEa2); LCD_WRITE_DATA(0x03);	// cea3[7:0] : clka1_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]
        LCD_WRITE_COMMAND(0xCEa3); LCD_WRITE_DATA(0x21);	// cea4[7:0] : clka1_switch[7:0]
        LCD_WRITE_COMMAND(0xCEa4); LCD_WRITE_DATA(0x00);	// cea5[7:0] : clka1_extend[7:0]
        LCD_WRITE_COMMAND(0xCEa5); LCD_WRITE_DATA(0x00);	// cea6[7:0] : clka1_tchop[7:0]
        LCD_WRITE_COMMAND(0xCEa6); LCD_WRITE_DATA(0x00);	// cea7[7:0] : clka1_tglue[7:0]
        LCD_WRITE_COMMAND(0xCEa7); LCD_WRITE_DATA(0x38);	// cea8[7:0] : clka2_width[3:0], clka2_shift[11:8]
        LCD_WRITE_COMMAND(0xCEa8); LCD_WRITE_DATA(0x01);	// cea9[7:0] : clka2_shift[7:0]
        LCD_WRITE_COMMAND(0xCEa9); LCD_WRITE_DATA(0x03);	// ceaa[7:0] : clka2_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]
        LCD_WRITE_COMMAND(0xCEaa); LCD_WRITE_DATA(0x22);	// ceab[7:0] : clka2_switch[7:0]
        LCD_WRITE_COMMAND(0xCEab); LCD_WRITE_DATA(0x00);	// ceac[7:0] : clka2_extend
        LCD_WRITE_COMMAND(0xCEac); LCD_WRITE_DATA(0x00);	// cead[7:0] : clka2_tchop
        LCD_WRITE_COMMAND(0xCEad); LCD_WRITE_DATA(0x00);	// ceae[7:0] : clka2_tglue

//CEBx : clka3, clka4
        LCD_WRITE_COMMAND(0xCEb0); LCD_WRITE_DATA(0x38);	// ceb1[7:0] : clka3_width[3:0], clka3_shift[11:8]
        LCD_WRITE_COMMAND(0xCEb1); LCD_WRITE_DATA(0x00);	// ceb2[7:0] : clka3_shift[7:0]
        LCD_WRITE_COMMAND(0xCEb2); LCD_WRITE_DATA(0x03);	// ceb3[7:0] : clka3_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]
        LCD_WRITE_COMMAND(0xCEb3); LCD_WRITE_DATA(0x23);	// ceb4[7:0] : clka3_switch[7:0]
        LCD_WRITE_COMMAND(0xCEb4); LCD_WRITE_DATA(0x00);	// ceb5[7:0] : clka3_extend[7:0]
        LCD_WRITE_COMMAND(0xCEb5); LCD_WRITE_DATA(0x00);	// ceb6[7:0] : clka3_tchop[7:0]
        LCD_WRITE_COMMAND(0xCEb6); LCD_WRITE_DATA(0x00);	// ceb7[7:0] : clka3_tglue[7:0]
        LCD_WRITE_COMMAND(0xCEb7); LCD_WRITE_DATA(0x30);	// ceb8[7:0] : clka4_width[3:0], clka2_shift[11:8]
        LCD_WRITE_COMMAND(0xCEb8); LCD_WRITE_DATA(0x00);	// ceb9[7:0] : clka4_shift[7:0]
        LCD_WRITE_COMMAND(0xCEb9); LCD_WRITE_DATA(0x03);	// ceba[7:0] : clka4_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]
        LCD_WRITE_COMMAND(0xCEba); LCD_WRITE_DATA(0x24);	// cebb[7:0] : clka4_switch[7:0]
        LCD_WRITE_COMMAND(0xCEbb); LCD_WRITE_DATA(0x00);	// cebc[7:0] : clka4_extend
        LCD_WRITE_COMMAND(0xCEbc); LCD_WRITE_DATA(0x00);	// cebd[7:0] : clka4_tchop
        LCD_WRITE_COMMAND(0xCEbd); LCD_WRITE_DATA(0x00);	// cebe[7:0] : clka4_tglue

//CECx : clkb1, clkb2
        LCD_WRITE_COMMAND(0xCEc0); LCD_WRITE_DATA(0x30);	// cec1[7:0] : clkb1_width[3:0], clkb1_shift[11:8]
        LCD_WRITE_COMMAND(0xCEc1); LCD_WRITE_DATA(0x01);	// cec2[7:0] : clkb1_shift[7:0]
        LCD_WRITE_COMMAND(0xCEc2); LCD_WRITE_DATA(0x03);	// cec3[7:0] : clkb1_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]
        LCD_WRITE_COMMAND(0xCEc3); LCD_WRITE_DATA(0x25);	// cec4[7:0] : clkb1_switch[7:0]
        LCD_WRITE_COMMAND(0xCEc4); LCD_WRITE_DATA(0x00);	// cec5[7:0] : clkb1_extend[7:0]
        LCD_WRITE_COMMAND(0xCEc5); LCD_WRITE_DATA(0x00);	// cec6[7:0] : clkb1_tchop[7:0]
        LCD_WRITE_COMMAND(0xCEc6); LCD_WRITE_DATA(0x00);	// cec7[7:0] : clkb1_tglue[7:0]
        LCD_WRITE_COMMAND(0xCEc7); LCD_WRITE_DATA(0x30);	// cec8[7:0] : clkb2_width[3:0], clkb2_shift[11:8]
        LCD_WRITE_COMMAND(0xCEc8); LCD_WRITE_DATA(0x02);	// cec9[7:0] : clkb2_shift[7:0]
        LCD_WRITE_COMMAND(0xCEc9); LCD_WRITE_DATA(0x03);	// ceca[7:0] : clkb2_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]
        LCD_WRITE_COMMAND(0xCEca); LCD_WRITE_DATA(0x26);	// cecb[7:0] : clkb2_switch[7:0]
        LCD_WRITE_COMMAND(0xCEcb); LCD_WRITE_DATA(0x00);	// cecc[7:0] : clkb2_extend
        LCD_WRITE_COMMAND(0xCEcc); LCD_WRITE_DATA(0x00);	// cecd[7:0] : clkb2_tchop
        LCD_WRITE_COMMAND(0xCEcd); LCD_WRITE_DATA(0x00);	// cece[7:0] : clkb2_tglue

//CEDx : clkb3, clkb4
        LCD_WRITE_COMMAND(0xCEd0); LCD_WRITE_DATA(0x30);	// ced1[7:0] : clkb3_width[3:0], clkb3_shift[11:8]
        LCD_WRITE_COMMAND(0xCEd1); LCD_WRITE_DATA(0x03);	// ced2[7:0] : clkb3_shift[7:0]
        LCD_WRITE_COMMAND(0xCEd2); LCD_WRITE_DATA(0x03);	// ced3[7:0] : clkb3_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]
        LCD_WRITE_COMMAND(0xCEd3); LCD_WRITE_DATA(0x27);	// ced4[7:0] : clkb3_switch[7:0]
        LCD_WRITE_COMMAND(0xCEd4); LCD_WRITE_DATA(0x00);	// ced5[7:0] : clkb3_extend[7:0]
        LCD_WRITE_COMMAND(0xCEd5); LCD_WRITE_DATA(0x00);	// ced6[7:0] : clkb3_tchop[7:0]
        LCD_WRITE_COMMAND(0xCEd6); LCD_WRITE_DATA(0x00);	// ced7[7:0] : clkb3_tglue[7:0]
        LCD_WRITE_COMMAND(0xCEd7); LCD_WRITE_DATA(0x30);	// ced8[7:0] : clkb4_width[3:0], clkb4_shift[11:8]
        LCD_WRITE_COMMAND(0xCEd8); LCD_WRITE_DATA(0x04);	// ced9[7:0] : clkb4_shift[7:0]
        LCD_WRITE_COMMAND(0xCEd9); LCD_WRITE_DATA(0x03);	// ceda[7:0] : clkb4_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]
        LCD_WRITE_COMMAND(0xCEda); LCD_WRITE_DATA(0x28);	// cedb[7:0] : clkb4_switch[7:0]
        LCD_WRITE_COMMAND(0xCEdb); LCD_WRITE_DATA(0x00);	// cedc[7:0] : clkb4_extend
        LCD_WRITE_COMMAND(0xCEdc); LCD_WRITE_DATA(0x00);	// cedd[7:0] : clkb4_tchop
        LCD_WRITE_COMMAND(0xCEdd); LCD_WRITE_DATA(0x00);	// cede[7:0] : clkb4_tglue

//CFCx :

        LCD_WRITE_COMMAND(0xCFc7); LCD_WRITE_DATA(0x00);	// cfc8[7:0] : reg_goa_gnd_opt, reg_goa_dpgm_tail_set, reg_goa_f_gating_en, reg_goa_f_odd_gating, toggle_mod1, 2, 3, 4
//--------------------------------------------------------------------------------
//		initial setting 3 < Panel setting >
//--------------------------------------------------------------------------------
// cbcx
        LCD_WRITE_COMMAND(0xCBc0); LCD_WRITE_DATA(0x00);	//cbc1[7:0] : enmode H-byte of sig1  (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBc1); LCD_WRITE_DATA(0x00);	//cbc2[7:0] : enmode H-byte of sig2  (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBc2); LCD_WRITE_DATA(0x00);	//cbc3[7:0] : enmode H-byte of sig3  (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBc3); LCD_WRITE_DATA(0x00);	//cbc4[7:0] : enmode H-byte of sig4  (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBc4); LCD_WRITE_DATA(0x54);	//cbc5[7:0] : enmode H-byte of sig5  (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBc5); LCD_WRITE_DATA(0x54);	//cbc6[7:0] : enmode H-byte of sig6  (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBc6); LCD_WRITE_DATA(0x54);	//cbc7[7:0] : enmode H-byte of sig7  (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBc7); LCD_WRITE_DATA(0x54);	//cbc8[7:0] : enmode H-byte of sig8  (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBc8); LCD_WRITE_DATA(0x00);	//cbc9[7:0] : enmode H-byte of sig9  (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBc9); LCD_WRITE_DATA(0x54);	//cbca[7:0] : enmode H-byte of sig10 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBca); LCD_WRITE_DATA(0x00);	//cbcb[7:0] : enmode H-byte of sig11 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBcb); LCD_WRITE_DATA(0x54);	//cbcc[7:0] : enmode H-byte of sig12 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBcc); LCD_WRITE_DATA(0x00);	//cbcd[7:0] : enmode H-byte of sig13 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBcd); LCD_WRITE_DATA(0x00);	//cbce[7:0] : enmode H-byte of sig14 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBce); LCD_WRITE_DATA(0x00);	//cbcf[7:0] : enmode H-byte of sig15 (pwrof_0, pwrof_1, norm, pwron_4 )

// cbdx
        LCD_WRITE_COMMAND(0xCBd0); LCD_WRITE_DATA(0x00);	//cbd1[7:0] : enmode H-byte of sig16 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBd1); LCD_WRITE_DATA(0x00);	//cbd2[7:0] : enmode H-byte of sig17 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBd2); LCD_WRITE_DATA(0x00);	//cbd3[7:0] : enmode H-byte of sig18 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBd3); LCD_WRITE_DATA(0x00);	//cbd4[7:0] : enmode H-byte of sig19 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBd4); LCD_WRITE_DATA(0x00);	//cbd5[7:0] : enmode H-byte of sig20 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBd5); LCD_WRITE_DATA(0x00);	//cbd6[7:0] : enmode H-byte of sig21 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBd6); LCD_WRITE_DATA(0x00);	//cbd7[7:0] : enmode H-byte of sig22 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBd7); LCD_WRITE_DATA(0x00);	//cbd8[7:0] : enmode H-byte of sig23 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBd8); LCD_WRITE_DATA(0x00);	//cbd9[7:0] : enmode H-byte of sig24 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBd9); LCD_WRITE_DATA(0x54);	//cbda[7:0] : enmode H-byte of sig25 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBda); LCD_WRITE_DATA(0x54);	//cbdb[7:0] : enmode H-byte of sig26 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBdb); LCD_WRITE_DATA(0x54);	//cbdc[7:0] : enmode H-byte of sig27 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBdc); LCD_WRITE_DATA(0x54);	//cbdd[7:0] : enmode H-byte of sig28 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBdd); LCD_WRITE_DATA(0x00);	//cbde[7:0] : enmode H-byte of sig29 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBde); LCD_WRITE_DATA(0x54);	//cbdf[7:0] : enmode H-byte of sig30 (pwrof_0, pwrof_1, norm, pwron_4 )

// cbex
        LCD_WRITE_COMMAND(0xCBe0); LCD_WRITE_DATA(0x00);	//cbe1[7:0] : enmode H-byte of sig31 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBe1); LCD_WRITE_DATA(0x54);	//cbe2[7:0] : enmode H-byte of sig32 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBe2); LCD_WRITE_DATA(0x00);	//cbe3[7:0] : enmode H-byte of sig33 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBe3); LCD_WRITE_DATA(0x00);	//cbe4[7:0] : enmode H-byte of sig34 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBe4); LCD_WRITE_DATA(0x00);	//cbe5[7:0] : enmode H-byte of sig35 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBe5); LCD_WRITE_DATA(0x00);	//cbe6[7:0] : enmode H-byte of sig36 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBe6); LCD_WRITE_DATA(0x00);	//cbe7[7:0] : enmode H-byte of sig37 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBe7); LCD_WRITE_DATA(0x00);	//cbe8[7:0] : enmode H-byte of sig38 (pwrof_0, pwrof_1, norm, pwron_4 )
        LCD_WRITE_COMMAND(0xCBe8); LCD_WRITE_DATA(0x00);	//cbe9[7:0] : enmode H-byte of sig39 (pwrof_0, pwrof_1, norm, pwron_4 )


// cc8x
        LCD_WRITE_COMMAND(0xCC80); LCD_WRITE_DATA(0x00);	//cc81[7:0] : reg setting for signal01 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC81); LCD_WRITE_DATA(0x00);	//cc82[7:0] : reg setting for signal02 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC82); LCD_WRITE_DATA(0x00);	//cc83[7:0] : reg setting for signal03 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC83); LCD_WRITE_DATA(0x00);	//cc84[7:0] : reg setting for signal04 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC84); LCD_WRITE_DATA(0x0C);	//cc85[7:0] : reg setting for signal05 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC85); LCD_WRITE_DATA(0x0A);	//cc86[7:0] : reg setting for signal06 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC86); LCD_WRITE_DATA(0x10);	//cc87[7:0] : reg setting for signal07 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC87); LCD_WRITE_DATA(0x0E);	//cc88[7:0] : reg setting for signal08 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC88); LCD_WRITE_DATA(0x00);	//cc89[7:0] : reg setting for signal09 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC89); LCD_WRITE_DATA(0x02);	//cc8a[7:0] : reg setting for signal10 selection with u2d mode

// cc9x
        LCD_WRITE_COMMAND(0xCC90); LCD_WRITE_DATA(0x00);	//cc91[7:0] : reg setting for signal11 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC91); LCD_WRITE_DATA(0x06);	//cc92[7:0] : reg setting for signal12 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC92); LCD_WRITE_DATA(0x00);	//cc93[7:0] : reg setting for signal13 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC93); LCD_WRITE_DATA(0x00);	//cc94[7:0] : reg setting for signal14 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC94); LCD_WRITE_DATA(0x00);	//cc95[7:0] : reg setting for signal15 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC95); LCD_WRITE_DATA(0x00);	//cc96[7:0] : reg setting for signal16 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC96); LCD_WRITE_DATA(0x00);	//cc97[7:0] : reg setting for signal17 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC97); LCD_WRITE_DATA(0x00);	//cc98[7:0] : reg setting for signal18 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC98); LCD_WRITE_DATA(0x00);	//cc99[7:0] : reg setting for signal19 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC99); LCD_WRITE_DATA(0x00);	//cc9a[7:0] : reg setting for signal20 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC9a); LCD_WRITE_DATA(0x00);	//cc9b[7:0] : reg setting for signal21 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC9b); LCD_WRITE_DATA(0x00);	//cc9c[7:0] : reg setting for signal22 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC9c); LCD_WRITE_DATA(0x00);	//cc9d[7:0] : reg setting for signal23 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC9d); LCD_WRITE_DATA(0x00);	//cc9e[7:0] : reg setting for signal24 selection with u2d mode
        LCD_WRITE_COMMAND(0xCC9e); LCD_WRITE_DATA(0x0B);	//cc9f[7:0] : reg setting for signal25 selection with u2d mode

// ccax
        LCD_WRITE_COMMAND(0xCCa0); LCD_WRITE_DATA(0x09);	//cca1[7:0] : reg setting for signal26 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCa1); LCD_WRITE_DATA(0x0F);	//cca2[7:0] : reg setting for signal27 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCa2); LCD_WRITE_DATA(0x0D);	//cca3[7:0] : reg setting for signal28 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCa3); LCD_WRITE_DATA(0x00);	//cca4[7:0] : reg setting for signal29 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCa4); LCD_WRITE_DATA(0x01);	//cca5[7:0] : reg setting for signal20 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCa5); LCD_WRITE_DATA(0x00);	//cca6[7:0] : reg setting for signal31 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCa6); LCD_WRITE_DATA(0x05);	//cca7[7:0] : reg setting for signal32 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCa7); LCD_WRITE_DATA(0x00);	//cca8[7:0] : reg setting for signal33 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCa8); LCD_WRITE_DATA(0x00);	//cca9[7:0] : reg setting for signal34 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCa9); LCD_WRITE_DATA(0x00);	//ccaa[7:0] : reg setting for signal35 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCaa); LCD_WRITE_DATA(0x00);	//ccab[7:0] : reg setting for signal36 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCab); LCD_WRITE_DATA(0x00);	//ccac[7:0] : reg setting for signal37 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCac); LCD_WRITE_DATA(0x00);	//ccad[7:0] : reg setting for signal38 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCad); LCD_WRITE_DATA(0x00);	//ccae[7:0] : reg setting for signal39 selection with u2d mode
        LCD_WRITE_COMMAND(0xCCae); LCD_WRITE_DATA(0x00);	//ccaf[7:0] : reg setting for signal40 selection with u2d mode

// ccbx
        LCD_WRITE_COMMAND(0xCCb0); LCD_WRITE_DATA(0x00);	//ccb1[7:0] : reg setting for signal01 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCb1); LCD_WRITE_DATA(0x00);	//ccb2[7:0] : reg setting for signal02 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCb2); LCD_WRITE_DATA(0x00);	//ccb3[7:0] : reg setting for signal03 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCb3); LCD_WRITE_DATA(0x00);	//ccb4[7:0] : reg setting for signal04 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCb4); LCD_WRITE_DATA(0x0D);	//ccb5[7:0] : reg setting for signal05 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCb5); LCD_WRITE_DATA(0x0F);	//ccb6[7:0] : reg setting for signal06 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCb6); LCD_WRITE_DATA(0x09);	//ccb7[7:0] : reg setting for signal07 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCb7); LCD_WRITE_DATA(0x0B);	//ccb8[7:0] : reg setting for signal08 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCb8); LCD_WRITE_DATA(0x00);	//ccb9[7:0] : reg setting for signal09 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCb9); LCD_WRITE_DATA(0x05);	//ccba[7:0] : reg setting for signal10 selection with d2u mode

// cccx
        LCD_WRITE_COMMAND(0xCCc0); LCD_WRITE_DATA(0x00);	//ccc1[7:0] : reg setting for signal11 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCc1); LCD_WRITE_DATA(0x01);	//ccc2[7:0] : reg setting for signal12 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCc2); LCD_WRITE_DATA(0x00);	//ccc3[7:0] : reg setting for signal13 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCc3); LCD_WRITE_DATA(0x00);	//ccc4[7:0] : reg setting for signal14 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCc4); LCD_WRITE_DATA(0x00);	//ccc5[7:0] : reg setting for signal15 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCc5); LCD_WRITE_DATA(0x00);	//ccc6[7:0] : reg setting for signal16 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCc6); LCD_WRITE_DATA(0x00);	//ccc7[7:0] : reg setting for signal17 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCc7); LCD_WRITE_DATA(0x00);	//ccc8[7:0] : reg setting for signal18 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCc8); LCD_WRITE_DATA(0x00);	//ccc9[7:0] : reg setting for signal19 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCc9); LCD_WRITE_DATA(0x00);	//ccca[7:0] : reg setting for signal20 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCca); LCD_WRITE_DATA(0x00);	//cccb[7:0] : reg setting for signal21 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCcb); LCD_WRITE_DATA(0x00);	//cccc[7:0] : reg setting for signal22 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCcc); LCD_WRITE_DATA(0x00);	//cccd[7:0] : reg setting for signal23 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCcd); LCD_WRITE_DATA(0x00);	//ccce[7:0] : reg setting for signal24 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCce); LCD_WRITE_DATA(0x0E);	//cccf[7:0] : reg setting for signal25 selection with d2u mode

// ccdx
        LCD_WRITE_COMMAND(0xCCd0); LCD_WRITE_DATA(0x10);	//ccd1[7:0] : reg setting for signal26 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCd1); LCD_WRITE_DATA(0x0A);	//ccd2[7:0] : reg setting for signal27 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCd2); LCD_WRITE_DATA(0x0C);	//ccd3[7:0] : reg setting for signal28 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCd3); LCD_WRITE_DATA(0x00);	//ccd4[7:0] : reg setting for signal29 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCd4); LCD_WRITE_DATA(0x06);	//ccd5[7:0] : reg setting for signal30 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCd5); LCD_WRITE_DATA(0x00);	//ccd6[7:0] : reg setting for signal31 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCd6); LCD_WRITE_DATA(0x02);	//ccd7[7:0] : reg setting for signal32 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCd7); LCD_WRITE_DATA(0x00);	//ccd8[7:0] : reg setting for signal33 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCd8); LCD_WRITE_DATA(0x00);	//ccd9[7:0] : reg setting for signal34 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCd9); LCD_WRITE_DATA(0x00);	//ccda[7:0] : reg setting for signal35 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCda); LCD_WRITE_DATA(0x00);	//ccdb[7:0] : reg setting for signal36 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCdb); LCD_WRITE_DATA(0x00);	//ccdc[7:0] : reg setting for signal37 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCdc); LCD_WRITE_DATA(0x00);	//ccdd[7:0] : reg setting for signal38 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCdd); LCD_WRITE_DATA(0x00);	//ccde[7:0] : reg setting for signal39 selection with d2u mode
        LCD_WRITE_COMMAND(0xCCde); LCD_WRITE_DATA(0x00);	//ccdf[7:0] : reg setting for signal40 selection with d2u mode


        LCD_WRITE_COMMAND(0x3A00); LCD_WRITE_DATA(0x55);     //MCU 16bits D[17:0]
        LCD_WRITE_COMMAND(0x3500); LCD_WRITE_DATA(0x00);     //TE ON
        LCD_WRITE_COMMAND(0x4400); LCD_WRITE_DATA(0x00);   
        LCD_WRITE_COMMAND(0x4401); LCD_WRITE_DATA(0x64);    

        
        LCD_WRITE_COMMAND(0x2000);

        LCD_WRITE_COMMAND(0x1100);
        Delayms(150);
        LCD_WRITE_COMMAND(0x2900);
        Delayms(200);

        LCD_WRITE_COMMAND(0x2C00);

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
    params->dbi.data_width              = LCM_DBI_DATA_WIDTH_16BITS;
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dbi.data_format.trans_seq   = LCM_DBI_TRANS_SEQ_MSB_FIRST;
    params->dbi.data_format.padding     = LCM_DBI_PADDING_ON_LSB;
    params->dbi.data_format.format      = LCM_DBI_FORMAT_RGB565;
    params->dbi.data_format.width       = LCM_DBI_DATA_WIDTH_16BITS;
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


    Lcd_Log("mycat READ OTM8009 LCD ID%s, id = 0x%x\n", __func__, id);


    return (LCM_ID == id)?1:0;

}

//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER otm8009a_aizhuoertai_a4019n50q0_mcu_wvga_lcm_drv =
    {
        .name		= "otm8009a_aizhuoertai_a4019n50q0_mcu_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id    = lcm_compare_id,
    };


