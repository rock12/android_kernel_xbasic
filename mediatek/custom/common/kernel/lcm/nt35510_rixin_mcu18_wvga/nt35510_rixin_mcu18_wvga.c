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
static int i =  0xB5;
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

#define REGW(x,y)   do{ \
	send_ctrl_cmd(x); \
	send_data_cmd(y); \
}while(0)

#define Delay(x) MDELAY(x)

static void init_lcm_registers(void)
{
#if defined(BUILD_UBOOT)
    printf("MYCAT LCD nt35510_hansen_HS40134WVNS39_00_HYDIS_mcu18_wvga");
#endif



REGW(0xFF00,0xAA);
REGW(0xFF01,0x55);
REGW(0xFF02,0xA5);
REGW(0xFF03,0x80);

REGW(0xF200,0x00);
REGW(0xF201,0x84);
REGW(0xF202,0x08);

REGW(0x6F00,0x0A);
REGW(0xF400,0x13);

REGW(0xFF00,0xAA);
REGW(0xFF01,0x55);
REGW(0xFF02,0xA5);
REGW(0xFF03,0x00);


REGW(0xF000,0x55);
REGW(0xF001,0xAA);
REGW(0xF002,0x52);
REGW(0xF003,0x08);
REGW(0xF004,0x01);


REGW(0xB000,0x0C);
REGW(0xB001,0x0C);
REGW(0xB002,0x0C);


REGW(0xB600,0x34);
REGW(0xB601,0x34);
REGW(0xB602,0x34);


REGW(0xB100,0x0C);
REGW(0xB101,0x0C);
REGW(0xB102,0x0C);


REGW(0xB700,0x35);
REGW(0xB701,0x35);
REGW(0xB702,0x35);


REGW(0xB200,0x00);
REGW(0xB201,0x00);
REGW(0xB202,0x00);


REGW(0xB800,0x24);
REGW(0xB801,0x24);
REGW(0xB802,0x24);


REGW(0xBF00,0x01);
REGW(0xB300,0x08);
REGW(0xB301,0x08);
REGW(0xB302,0x08);


REGW(0xB900,0x37);
REGW(0xB901,0x37);
REGW(0xB902,0x37);


REGW(0xBA00,0x24);
REGW(0xBA01,0x24);
REGW(0xBA02,0x24);


REGW(0xBC00,0x00);
REGW(0xBC01,0xA0);
REGW(0xBC02,0x00);


REGW(0xBD00,0x00);
REGW(0xBD01,0xA0);
REGW(0xBD02,0x00);


REGW(0xBE00,0x00);
REGW(0xBE01,0x9e);



REGW(0xD100,0x00);
REGW(0xD101,0x02);
REGW(0xD102,0x00);
REGW(0xD103,0x18);
REGW(0xD104,0x00);
REGW(0xD105,0x2E);
REGW(0xD106,0x00);
REGW(0xD107,0x56);
REGW(0xD108,0x00);
REGW(0xD109,0x62);
REGW(0xD10A,0x00);
REGW(0xD10B,0x99);
REGW(0xD10C,0x00);
REGW(0xD10D,0xBA);
REGW(0xD10E,0x00);
REGW(0xD10F,0xF0);
REGW(0xD110,0x01);
REGW(0xD111,0x1A);
REGW(0xD112,0x01);
REGW(0xD113,0x5A);
REGW(0xD114,0x01);
REGW(0xD115,0x8C);
REGW(0xD116,0x01);
REGW(0xD117,0xDA);
REGW(0xD118,0x02);
REGW(0xD119,0x19);
REGW(0xD11A,0x02);
REGW(0xD11B,0x1B);
REGW(0xD11C,0x02);
REGW(0xD11D,0x56);
REGW(0xD11E,0x02);
REGW(0xD11F,0x91);
REGW(0xD120,0x02);
REGW(0xD121,0xB6);
REGW(0xD122,0x02);
REGW(0xD123,0xE8);
REGW(0xD124,0x03);
REGW(0xD125,0x0A);
REGW(0xD126,0x03);
REGW(0xD127,0x4F);
REGW(0xD128,0x03);
REGW(0xD129,0x96);
REGW(0xD12A,0x03);
REGW(0xD12B,0xC7);
REGW(0xD12C,0x03);
REGW(0xD12D,0xD7);
REGW(0xD12E,0x03);
REGW(0xD12F,0xE2);
REGW(0xD130,0x03);
REGW(0xD131,0xE7);
REGW(0xD132,0x03);
REGW(0xD133,0xE8);

REGW(0xD200,0x00);
REGW(0xD201,0x02);
REGW(0xD202,0x00);
REGW(0xD203,0x18);
REGW(0xD204,0x00);
REGW(0xD205,0x2E);
REGW(0xD206,0x00);
REGW(0xD207,0x56);
REGW(0xD208,0x00);
REGW(0xD209,0x62);
REGW(0xD20A,0x00);
REGW(0xD20B,0x99);
REGW(0xD20C,0x00);
REGW(0xD20D,0xBA);
REGW(0xD20E,0x00);
REGW(0xD20F,0xF0);
REGW(0xD210,0x01);
REGW(0xD211,0x1A);
REGW(0xD212,0x01);
REGW(0xD213,0x5A);
REGW(0xD214,0x01);
REGW(0xD215,0x8C);
REGW(0xD216,0x01);
REGW(0xD217,0xDA);
REGW(0xD218,0x02);
REGW(0xD219,0x19);
REGW(0xD21A,0x02);
REGW(0xD21B,0x1B);
REGW(0xD21C,0x02);
REGW(0xD21D,0x56);
REGW(0xD21E,0x02);
REGW(0xD21F,0x91);
REGW(0xD220,0x02);
REGW(0xD221,0xB6);
REGW(0xD222,0x02);
REGW(0xD223,0xE8);
REGW(0xD224,0x03);
REGW(0xD225,0x0A);
REGW(0xD226,0x03);
REGW(0xD227,0x4F);
REGW(0xD228,0x03);
REGW(0xD229,0x96);
REGW(0xD22A,0x03);
REGW(0xD22B,0xC7);
REGW(0xD22C,0x03);
REGW(0xD22D,0xD7);
REGW(0xD22E,0x03);
REGW(0xD22F,0xE2);
REGW(0xD230,0x03);
REGW(0xD231,0xE7);
REGW(0xD232,0x03);
REGW(0xD233,0xE8);

REGW(0xD300,0x00);
REGW(0xD301,0x02);
REGW(0xD302,0x00);
REGW(0xD303,0x18);
REGW(0xD304,0x00);
REGW(0xD305,0x2E);
REGW(0xD306,0x00);
REGW(0xD307,0x56);
REGW(0xD308,0x00);
REGW(0xD309,0x62);
REGW(0xD30A,0x00);
REGW(0xD30B,0x99);
REGW(0xD30C,0x00);
REGW(0xD30D,0xBA);
REGW(0xD30E,0x00);
REGW(0xD30F,0xF0);
REGW(0xD310,0x01);
REGW(0xD311,0x1A);
REGW(0xD312,0x01);
REGW(0xD313,0x5A);
REGW(0xD314,0x01);
REGW(0xD315,0x8C);
REGW(0xD316,0x01);
REGW(0xD317,0xDA);
REGW(0xD318,0x02);
REGW(0xD319,0x19);
REGW(0xD31A,0x02);
REGW(0xD31B,0x1B);
REGW(0xD31C,0x02);
REGW(0xD31D,0x56);
REGW(0xD31E,0x02);
REGW(0xD31F,0x91);
REGW(0xD320,0x02);
REGW(0xD321,0xB6);
REGW(0xD322,0x02);
REGW(0xD323,0xE8);
REGW(0xD324,0x03);
REGW(0xD325,0x0A);
REGW(0xD326,0x03);
REGW(0xD327,0x4F);
REGW(0xD328,0x03);
REGW(0xD329,0x96);
REGW(0xD32A,0x03);
REGW(0xD32B,0xC7);
REGW(0xD32C,0x03);
REGW(0xD32D,0xD7);
REGW(0xD32E,0x03);
REGW(0xD32F,0xE2);
REGW(0xD330,0x03);
REGW(0xD331,0xE7);
REGW(0xD332,0x03);
REGW(0xD333,0xE8);

REGW(0xD400,0x00);
REGW(0xD401,0x02);
REGW(0xD402,0x00);
REGW(0xD403,0x18);
REGW(0xD404,0x00);
REGW(0xD405,0x2E);
REGW(0xD406,0x00);
REGW(0xD407,0x56);
REGW(0xD408,0x00);
REGW(0xD409,0x62);
REGW(0xD40A,0x00);
REGW(0xD40B,0x99);
REGW(0xD40C,0x00);
REGW(0xD40D,0xBA);
REGW(0xD40E,0x00);
REGW(0xD40F,0xF0);
REGW(0xD410,0x01);
REGW(0xD411,0x1A);
REGW(0xD412,0x01);
REGW(0xD413,0x5A);
REGW(0xD414,0x01);
REGW(0xD415,0x8C);
REGW(0xD416,0x01);
REGW(0xD417,0xDA);
REGW(0xD418,0x02);
REGW(0xD419,0x19);
REGW(0xD41A,0x02);
REGW(0xD41B,0x1B);
REGW(0xD41C,0x02);
REGW(0xD41D,0x56);
REGW(0xD41E,0x02);
REGW(0xD41F,0x91);
REGW(0xD420,0x02);
REGW(0xD421,0xB6);
REGW(0xD422,0x02);
REGW(0xD423,0xE8);
REGW(0xD424,0x03);
REGW(0xD425,0x0A);
REGW(0xD426,0x03);
REGW(0xD427,0x4F);
REGW(0xD428,0x03);
REGW(0xD429,0x96);
REGW(0xD42A,0x03);
REGW(0xD42B,0xC7);
REGW(0xD42C,0x03);
REGW(0xD42D,0xD7);
REGW(0xD42E,0x03);
REGW(0xD42F,0xE2);
REGW(0xD430,0x03);
REGW(0xD431,0xE7);
REGW(0xD432,0x03);
REGW(0xD433,0xE8);

REGW(0xD500,0x00);
REGW(0xD501,0x02);
REGW(0xD502,0x00);
REGW(0xD503,0x18);
REGW(0xD504,0x00);
REGW(0xD505,0x2E);
REGW(0xD506,0x00);
REGW(0xD507,0x56);
REGW(0xD508,0x00);
REGW(0xD509,0x62);
REGW(0xD50A,0x00);
REGW(0xD50B,0x99);
REGW(0xD50C,0x00);
REGW(0xD50D,0xBA);
REGW(0xD50E,0x00);
REGW(0xD50F,0xF0);
REGW(0xD510,0x01);
REGW(0xD511,0x1A);
REGW(0xD512,0x01);
REGW(0xD513,0x5A);
REGW(0xD514,0x01);
REGW(0xD515,0x8C);
REGW(0xD516,0x01);
REGW(0xD517,0xDA);
REGW(0xD518,0x02);
REGW(0xD519,0x19);
REGW(0xD51A,0x02);
REGW(0xD51B,0x1B);
REGW(0xD51C,0x02);
REGW(0xD51D,0x56);
REGW(0xD51E,0x02);
REGW(0xD51F,0x91);
REGW(0xD520,0x02);
REGW(0xD521,0xB6);
REGW(0xD522,0x02);
REGW(0xD523,0xE8);
REGW(0xD524,0x03);
REGW(0xD525,0x0A);
REGW(0xD526,0x03);
REGW(0xD527,0x4F);
REGW(0xD528,0x03);
REGW(0xD529,0x96);
REGW(0xD52A,0x03);
REGW(0xD52B,0xC7);
REGW(0xD52C,0x03);
REGW(0xD52D,0xD7);
REGW(0xD52E,0x03);
REGW(0xD52F,0xE2);
REGW(0xD530,0x03);
REGW(0xD531,0xE7);
REGW(0xD532,0x03);
REGW(0xD533,0xE8);

REGW(0xD600,0x00);
REGW(0xD601,0x02);
REGW(0xD602,0x00);
REGW(0xD603,0x18);
REGW(0xD604,0x00);
REGW(0xD605,0x2E);
REGW(0xD606,0x00);
REGW(0xD607,0x56);
REGW(0xD608,0x00);
REGW(0xD609,0x62);
REGW(0xD60A,0x00);
REGW(0xD60B,0x99);
REGW(0xD60C,0x00);
REGW(0xD60D,0xBA);
REGW(0xD60E,0x00);
REGW(0xD60F,0xF0);
REGW(0xD610,0x01);
REGW(0xD611,0x1A);
REGW(0xD612,0x01);
REGW(0xD613,0x5A);
REGW(0xD614,0x01);
REGW(0xD615,0x8C);
REGW(0xD616,0x01);
REGW(0xD617,0xDA);
REGW(0xD618,0x02);
REGW(0xD619,0x19);
REGW(0xD61A,0x02);
REGW(0xD61B,0x1B);
REGW(0xD61C,0x02);
REGW(0xD61D,0x56);
REGW(0xD61E,0x02);
REGW(0xD61F,0x91);
REGW(0xD620,0x02);
REGW(0xD621,0xB6);
REGW(0xD622,0x02);
REGW(0xD623,0xE8);
REGW(0xD624,0x03);
REGW(0xD625,0x0A);
REGW(0xD626,0x03);
REGW(0xD627,0x4F);
REGW(0xD628,0x03);
REGW(0xD629,0x96);
REGW(0xD62A,0x03);
REGW(0xD62B,0xC7);
REGW(0xD62C,0x03);
REGW(0xD62D,0xD7);
REGW(0xD62E,0x03);
REGW(0xD62F,0xE2);
REGW(0xD630,0x03);
REGW(0xD631,0xE7);
REGW(0xD632,0x03);
REGW(0xD633,0xE8);


REGW(0xF000,0x55);
REGW(0xF001,0xAA);
REGW(0xF002,0x52);
REGW(0xF003,0x08);
REGW(0xF004,0x00);


REGW(0xB100,0xFC);
REGW(0xB101,0x00);


REGW(0xB600,0x05);


REGW(0xB700,0x70);
REGW(0xB701,0x70);


REGW(0xB800,0x01);
REGW(0xB801,0x05);
REGW(0xB802,0x05);
REGW(0xB803,0x05);


REGW(0xBC00,0x03);
REGW(0xBC01,0x03);
REGW(0xBC02,0x03);


REGW(0xC900,0xD0);
REGW(0xC901,0x82);
REGW(0xC902,0x50);
REGW(0xC903,0x50);
REGW(0xC904,0x50);

REGW(0x3A00,0x66);
REGW(0x1100,0x00);
Delay(120);
REGW(0x2900,0x00);
 

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
    i -= 2;
    Lcd_Log("mycat lcm_resume i = %d\n",i);
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



    return (LCM_ID == id)?1:0;
}

LCM_DRIVER nt35510_rixin_mcu18_wvga_lcm_drv =
    {
        .name			= "nt35510_rixin_mcu18_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id     = lcm_compare_id
    };
