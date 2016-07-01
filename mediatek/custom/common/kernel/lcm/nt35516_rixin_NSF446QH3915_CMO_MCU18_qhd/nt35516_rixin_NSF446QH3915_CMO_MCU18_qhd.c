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

#define FRAME_WIDTH  (540)
#define FRAME_HEIGHT (960)
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
//static int ddh=0,ddl=0;
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
#define LCD_Write_COM(h,l)	send_ctrl_cmd(h<<8|l)

#define LCD_Write_DATA(h,l)  send_data_cmd(h<<8|l)
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

//NT35516_CMI4.46_MCU_V1	
//*************************************
// Select CMD2);send_data_cmd(Page 0
//*************************************
send_ctrl_cmd(0xF000);send_data_cmd(0x55);
send_ctrl_cmd(0xF001);send_data_cmd(0xAA);
send_ctrl_cmd(0xF002);send_data_cmd(0x52);
send_ctrl_cmd(0xF003);send_data_cmd(0x08);
send_ctrl_cmd(0xF004);send_data_cmd(0x00);

//#Source hold time
send_ctrl_cmd(0xB600);send_data_cmd(0x05);

//#Gate EQ control
send_ctrl_cmd(0xB700);send_data_cmd(0x72);
send_ctrl_cmd(0xB701);send_data_cmd(0x72);

//#Source EQ control (Mode 2)
send_ctrl_cmd(0xB800);send_data_cmd(0x01);
send_ctrl_cmd(0xB801);send_data_cmd(0x04);
send_ctrl_cmd(0xB802);send_data_cmd(0x04);
send_ctrl_cmd(0xB803);send_data_cmd(0x04);

// Inversion
send_ctrl_cmd(0xBC00);send_data_cmd(0x00);
send_ctrl_cmd(0xBC01);send_data_cmd(0x00);
send_ctrl_cmd(0xBC02);send_data_cmd(0x00);

// Display Timing: Dual 8-phase 4-overlap
send_ctrl_cmd(0xC700);send_data_cmd(0x00);
send_ctrl_cmd(0xC701);send_data_cmd(0xFF);
send_ctrl_cmd(0xC702);send_data_cmd(0xFF);
send_ctrl_cmd(0xC703);send_data_cmd(0x10);
send_ctrl_cmd(0xC704);send_data_cmd(0x10);
send_ctrl_cmd(0xC705);send_data_cmd(0x0A);
send_ctrl_cmd(0xC706);send_data_cmd(0x0B);
send_ctrl_cmd(0xC707);send_data_cmd(0x0A);
send_ctrl_cmd(0xC708);send_data_cmd(0x0A);
send_ctrl_cmd(0xC709);send_data_cmd(0xFF);
send_ctrl_cmd(0xC70A);send_data_cmd(0xFF);


//*************************************
// Select CMD2);send_data_cmd(Page 1
//*************************************
send_ctrl_cmd(0xF000);send_data_cmd(0x55);
send_ctrl_cmd(0xF001);send_data_cmd(0xAA);
send_ctrl_cmd(0xF002);send_data_cmd(0x52);
send_ctrl_cmd(0xF003);send_data_cmd(0x08);
send_ctrl_cmd(0xF004);send_data_cmd(0x01);


// AVDD: 5.5V
send_ctrl_cmd(0xB000);send_data_cmd(0x0A);
send_ctrl_cmd(0xB001);send_data_cmd(0x0A);
send_ctrl_cmd(0xB002);send_data_cmd(0x0A);

// AVEE: -5.5V
send_ctrl_cmd(0xB100);send_data_cmd(0x0A);
send_ctrl_cmd(0xB101);send_data_cmd(0x0A);
send_ctrl_cmd(0xB102);send_data_cmd(0x0A);

// VCL: -3.5V
send_ctrl_cmd(0xB200);send_data_cmd(0x02);
send_ctrl_cmd(0xB201);send_data_cmd(0x02);
send_ctrl_cmd(0xB202);send_data_cmd(0x02);

// VGH: 15.0V
send_ctrl_cmd(0xB300);send_data_cmd(0x10);
send_ctrl_cmd(0xB301);send_data_cmd(0x10);
send_ctrl_cmd(0xB302);send_data_cmd(0x10);

// VGLX: -10.0V
send_ctrl_cmd(0xB400);send_data_cmd(0x06);
send_ctrl_cmd(0xB401);send_data_cmd(0x06);
send_ctrl_cmd(0xB402);send_data_cmd(0x06);

// AVDD: 2xVDDB
send_ctrl_cmd(0xB600);send_data_cmd(0x34);
send_ctrl_cmd(0xB601);send_data_cmd(0x34);
send_ctrl_cmd(0xB602);send_data_cmd(0x34);

// AVEE: -2.5xVDDB
send_ctrl_cmd(0xB700);send_data_cmd(0x34);
send_ctrl_cmd(0xB701);send_data_cmd(0x34);
send_ctrl_cmd(0xB702);send_data_cmd(0x34);

// VCL: -1.5xVDDB
send_ctrl_cmd(0xB800);send_data_cmd(0x20);
send_ctrl_cmd(0xB801);send_data_cmd(0x20);
send_ctrl_cmd(0xB802);send_data_cmd(0x20);

// VGH: 2xAVDD-AVEE
send_ctrl_cmd(0xB900);send_data_cmd(0x35);
send_ctrl_cmd(0xB901);send_data_cmd(0x35);
send_ctrl_cmd(0xB902);send_data_cmd(0x35);

// VGLX: AVEE-AVDD
send_ctrl_cmd(0xBA00);send_data_cmd(0x14);
send_ctrl_cmd(0xBA01);send_data_cmd(0x14);
send_ctrl_cmd(0xBA02);send_data_cmd(0x14);

// Set VGMP = 5V / VGSP = 0V
send_ctrl_cmd(0xBC00);send_data_cmd(0x00);
send_ctrl_cmd(0xBC01);send_data_cmd(0xA0);
send_ctrl_cmd(0xBC02);send_data_cmd(0x00);

// Set VGMN = -5V / VGSN = 0V
send_ctrl_cmd(0xBD00);send_data_cmd(0x00); 
send_ctrl_cmd(0xBD01);send_data_cmd(0xA0);
send_ctrl_cmd(0xBD02);send_data_cmd(0x00);

// Set VCOM_offset -1.6625V
send_ctrl_cmd(0xBE00);send_data_cmd(0x6D);

// Pump:0x00
send_ctrl_cmd(0xC200);send_data_cmd(0x00);

// Gamma Gradient Control
send_ctrl_cmd(0xD000);send_data_cmd(0x0F);
send_ctrl_cmd(0xD001);send_data_cmd(0x0F);
send_ctrl_cmd(0xD002);send_data_cmd(0x10);
send_ctrl_cmd(0xD003);send_data_cmd(0x10);     

// Positive Red Gamma
send_ctrl_cmd(0xD100);send_data_cmd(0x00);
send_ctrl_cmd(0xD101);send_data_cmd(0x00);
send_ctrl_cmd(0xD102);send_data_cmd(0x00);
send_ctrl_cmd(0xD103);send_data_cmd(0x1A);
send_ctrl_cmd(0xD104);send_data_cmd(0x00);
send_ctrl_cmd(0xD105);send_data_cmd(0x3F);
send_ctrl_cmd(0xD106);send_data_cmd(0x00);
send_ctrl_cmd(0xD107);send_data_cmd(0x5C);
send_ctrl_cmd(0xD108);send_data_cmd(0x00);
send_ctrl_cmd(0xD109);send_data_cmd(0x6F);
send_ctrl_cmd(0xD10A);send_data_cmd(0x00);
send_ctrl_cmd(0xD10B);send_data_cmd(0x99);
send_ctrl_cmd(0xD10C);send_data_cmd(0x00);
send_ctrl_cmd(0xD10D);send_data_cmd(0xB7);
send_ctrl_cmd(0xD10E);send_data_cmd(0x00);
send_ctrl_cmd(0xD10F);send_data_cmd(0xE8);
send_ctrl_cmd(0xD200);send_data_cmd(0x01);
send_ctrl_cmd(0xD201);send_data_cmd(0x0F);
send_ctrl_cmd(0xD202);send_data_cmd(0x01);
send_ctrl_cmd(0xD203);send_data_cmd(0x4C);
send_ctrl_cmd(0xD204);send_data_cmd(0x01);
send_ctrl_cmd(0xD205);send_data_cmd(0x7C);
send_ctrl_cmd(0xD206);send_data_cmd(0x01);
send_ctrl_cmd(0xD207);send_data_cmd(0xC7);
send_ctrl_cmd(0xD208);send_data_cmd(0x02);
send_ctrl_cmd(0xD209);send_data_cmd(0x03);
send_ctrl_cmd(0xD20A);send_data_cmd(0x02);
send_ctrl_cmd(0xD20B);send_data_cmd(0x05);
send_ctrl_cmd(0xD20C);send_data_cmd(0x02);
send_ctrl_cmd(0xD20D);send_data_cmd(0x3C);
send_ctrl_cmd(0xD20E);send_data_cmd(0x02);
send_ctrl_cmd(0xD20F);send_data_cmd(0x77);
send_ctrl_cmd(0xD300);send_data_cmd(0x02);
send_ctrl_cmd(0xD301);send_data_cmd(0x9C);
send_ctrl_cmd(0xD302);send_data_cmd(0x02);
send_ctrl_cmd(0xD303);send_data_cmd(0xCB);
send_ctrl_cmd(0xD304);send_data_cmd(0x02);
send_ctrl_cmd(0xD305);send_data_cmd(0xEA);
send_ctrl_cmd(0xD306);send_data_cmd(0x03);
send_ctrl_cmd(0xD307);send_data_cmd(0x10);
send_ctrl_cmd(0xD308);send_data_cmd(0x03);
send_ctrl_cmd(0xD309);send_data_cmd(0x27);
send_ctrl_cmd(0xD30A);send_data_cmd(0x03);
send_ctrl_cmd(0xD30B);send_data_cmd(0x40);
send_ctrl_cmd(0xD30C);send_data_cmd(0x03);
send_ctrl_cmd(0xD30D);send_data_cmd(0x4B);
send_ctrl_cmd(0xD30E);send_data_cmd(0x03);
send_ctrl_cmd(0xD30F);send_data_cmd(0x54);
send_ctrl_cmd(0xD400);send_data_cmd(0x03);
send_ctrl_cmd(0xD401);send_data_cmd(0x5A);
send_ctrl_cmd(0xD402);send_data_cmd(0x03);
send_ctrl_cmd(0xD403);send_data_cmd(0x5A);
            
            
// Positive Green Gamma
send_ctrl_cmd(0xD500);send_data_cmd(0x00);
send_ctrl_cmd(0xD501);send_data_cmd(0x00);
send_ctrl_cmd(0xD502);send_data_cmd(0x00);
send_ctrl_cmd(0xD503);send_data_cmd(0x1A);
send_ctrl_cmd(0xD504);send_data_cmd(0x00);
send_ctrl_cmd(0xD505);send_data_cmd(0x3F);
send_ctrl_cmd(0xD506);send_data_cmd(0x00);
send_ctrl_cmd(0xD507);send_data_cmd(0x5C);
send_ctrl_cmd(0xD508);send_data_cmd(0x00);
send_ctrl_cmd(0xD509);send_data_cmd(0x6F);
send_ctrl_cmd(0xD50A);send_data_cmd(0x00);
send_ctrl_cmd(0xD50B);send_data_cmd(0x99);
send_ctrl_cmd(0xD50C);send_data_cmd(0x00);
send_ctrl_cmd(0xD50D);send_data_cmd(0xB7);
send_ctrl_cmd(0xD50E);send_data_cmd(0x00);
send_ctrl_cmd(0xD50F);send_data_cmd(0xE8);
send_ctrl_cmd(0xD600);send_data_cmd(0x01);
send_ctrl_cmd(0xD601);send_data_cmd(0x0F);
send_ctrl_cmd(0xD602);send_data_cmd(0x01);
send_ctrl_cmd(0xD603);send_data_cmd(0x4C);
send_ctrl_cmd(0xD604);send_data_cmd(0x01);
send_ctrl_cmd(0xD605);send_data_cmd(0x7C);
send_ctrl_cmd(0xD606);send_data_cmd(0x01);
send_ctrl_cmd(0xD607);send_data_cmd(0xC7);
send_ctrl_cmd(0xD608);send_data_cmd(0x02);
send_ctrl_cmd(0xD609);send_data_cmd(0x03);
send_ctrl_cmd(0xD60A);send_data_cmd(0x02);
send_ctrl_cmd(0xD60B);send_data_cmd(0x05);
send_ctrl_cmd(0xD60C);send_data_cmd(0x02);
send_ctrl_cmd(0xD60D);send_data_cmd(0x3C);
send_ctrl_cmd(0xD60E);send_data_cmd(0x02);
send_ctrl_cmd(0xD60F);send_data_cmd(0x77);
send_ctrl_cmd(0xD700);send_data_cmd(0x02);
send_ctrl_cmd(0xD701);send_data_cmd(0x9C);
send_ctrl_cmd(0xD702);send_data_cmd(0x02);
send_ctrl_cmd(0xD703);send_data_cmd(0xCB);
send_ctrl_cmd(0xD704);send_data_cmd(0x02);
send_ctrl_cmd(0xD705);send_data_cmd(0xEA);
send_ctrl_cmd(0xD706);send_data_cmd(0x03);
send_ctrl_cmd(0xD707);send_data_cmd(0x10);
send_ctrl_cmd(0xD708);send_data_cmd(0x03);
send_ctrl_cmd(0xD709);send_data_cmd(0x27);
send_ctrl_cmd(0xD70A);send_data_cmd(0x03);
send_ctrl_cmd(0xD70B);send_data_cmd(0x40);
send_ctrl_cmd(0xD70C);send_data_cmd(0x03);
send_ctrl_cmd(0xD70D);send_data_cmd(0x4B);
send_ctrl_cmd(0xD70E);send_data_cmd(0x03);
send_ctrl_cmd(0xD70F);send_data_cmd(0x54);
send_ctrl_cmd(0xD800);send_data_cmd(0x03);
send_ctrl_cmd(0xD801);send_data_cmd(0x5A);
send_ctrl_cmd(0xD802);send_data_cmd(0x03);
send_ctrl_cmd(0xD803);send_data_cmd(0x5A);

// Positive Blue Gamma
send_ctrl_cmd(0xD900);send_data_cmd(0x00);
send_ctrl_cmd(0xD901);send_data_cmd(0x00);
send_ctrl_cmd(0xD902);send_data_cmd(0x00);
send_ctrl_cmd(0xD903);send_data_cmd(0x1A);
send_ctrl_cmd(0xD904);send_data_cmd(0x00);
send_ctrl_cmd(0xD905);send_data_cmd(0x3F);
send_ctrl_cmd(0xD906);send_data_cmd(0x00);
send_ctrl_cmd(0xD907);send_data_cmd(0x5C);
send_ctrl_cmd(0xD908);send_data_cmd(0x00);
send_ctrl_cmd(0xD909);send_data_cmd(0x6F);
send_ctrl_cmd(0xD90A);send_data_cmd(0x00);
send_ctrl_cmd(0xD90B);send_data_cmd(0x99);
send_ctrl_cmd(0xD90C);send_data_cmd(0x00);
send_ctrl_cmd(0xD90D);send_data_cmd(0xB7);
send_ctrl_cmd(0xD90E);send_data_cmd(0x00);
send_ctrl_cmd(0xD90F);send_data_cmd(0xE8);
send_ctrl_cmd(0xDD00);send_data_cmd(0x01);
send_ctrl_cmd(0xDD01);send_data_cmd(0x0F);
send_ctrl_cmd(0xDD02);send_data_cmd(0x01);
send_ctrl_cmd(0xDD03);send_data_cmd(0x4C);
send_ctrl_cmd(0xDD04);send_data_cmd(0x01);
send_ctrl_cmd(0xDD05);send_data_cmd(0x7C);
send_ctrl_cmd(0xDD06);send_data_cmd(0x01);
send_ctrl_cmd(0xDD07);send_data_cmd(0xC7);
send_ctrl_cmd(0xDD08);send_data_cmd(0x02);
send_ctrl_cmd(0xDD09);send_data_cmd(0x03);
send_ctrl_cmd(0xDD0A);send_data_cmd(0x02);
send_ctrl_cmd(0xDD0B);send_data_cmd(0x05);
send_ctrl_cmd(0xDD0C);send_data_cmd(0x02);
send_ctrl_cmd(0xDD0D);send_data_cmd(0x3C);
send_ctrl_cmd(0xDD0E);send_data_cmd(0x02);
send_ctrl_cmd(0xDD0F);send_data_cmd(0x77);
send_ctrl_cmd(0xDE00);send_data_cmd(0x02);
send_ctrl_cmd(0xDE01);send_data_cmd(0x9C);
send_ctrl_cmd(0xDE02);send_data_cmd(0x02);
send_ctrl_cmd(0xDE03);send_data_cmd(0xCB);
send_ctrl_cmd(0xDE04);send_data_cmd(0x02);
send_ctrl_cmd(0xDE05);send_data_cmd(0xEA);
send_ctrl_cmd(0xDE06);send_data_cmd(0x03);
send_ctrl_cmd(0xDE07);send_data_cmd(0x10);
send_ctrl_cmd(0xDE08);send_data_cmd(0x03);
send_ctrl_cmd(0xDE09);send_data_cmd(0x27);
send_ctrl_cmd(0xDE0A);send_data_cmd(0x03);
send_ctrl_cmd(0xDE0B);send_data_cmd(0x40);
send_ctrl_cmd(0xDE0C);send_data_cmd(0x03);
send_ctrl_cmd(0xDE0D);send_data_cmd(0x4B);
send_ctrl_cmd(0xDE0E);send_data_cmd(0x03);
send_ctrl_cmd(0xDE0F);send_data_cmd(0x54);
send_ctrl_cmd(0xDF00);send_data_cmd(0x03);
send_ctrl_cmd(0xDF01);send_data_cmd(0x5A);
send_ctrl_cmd(0xDF02);send_data_cmd(0x03);
send_ctrl_cmd(0xDF03);send_data_cmd(0x5A);

// Negative Red Gamma
send_ctrl_cmd(0xE000);send_data_cmd(0x00);
send_ctrl_cmd(0xE001);send_data_cmd(0x00);
send_ctrl_cmd(0xE002);send_data_cmd(0x00);
send_ctrl_cmd(0xE003);send_data_cmd(0x1A);
send_ctrl_cmd(0xE004);send_data_cmd(0x00);
send_ctrl_cmd(0xE005);send_data_cmd(0x3F);
send_ctrl_cmd(0xE006);send_data_cmd(0x00);
send_ctrl_cmd(0xE007);send_data_cmd(0x5C);
send_ctrl_cmd(0xE008);send_data_cmd(0x00);
send_ctrl_cmd(0xE009);send_data_cmd(0x6F);
send_ctrl_cmd(0xE00A);send_data_cmd(0x00);
send_ctrl_cmd(0xE00B);send_data_cmd(0x99);
send_ctrl_cmd(0xE00C);send_data_cmd(0x00);
send_ctrl_cmd(0xE00D);send_data_cmd(0xB7);
send_ctrl_cmd(0xE00E);send_data_cmd(0x00);
send_ctrl_cmd(0xE00F);send_data_cmd(0xE8);
send_ctrl_cmd(0xE100);send_data_cmd(0x01);
send_ctrl_cmd(0xE101);send_data_cmd(0x0F);
send_ctrl_cmd(0xE102);send_data_cmd(0x01);
send_ctrl_cmd(0xE103);send_data_cmd(0x4C);
send_ctrl_cmd(0xE104);send_data_cmd(0x01);
send_ctrl_cmd(0xE105);send_data_cmd(0x7C);
send_ctrl_cmd(0xE106);send_data_cmd(0x01);
send_ctrl_cmd(0xE107);send_data_cmd(0xC7);
send_ctrl_cmd(0xE108);send_data_cmd(0x02);
send_ctrl_cmd(0xE109);send_data_cmd(0x03);
send_ctrl_cmd(0xE10A);send_data_cmd(0x02);
send_ctrl_cmd(0xE10B);send_data_cmd(0x05);
send_ctrl_cmd(0xE10C);send_data_cmd(0x02);
send_ctrl_cmd(0xE10D);send_data_cmd(0x3C);
send_ctrl_cmd(0xE10E);send_data_cmd(0x02);
send_ctrl_cmd(0xE10F);send_data_cmd(0x77);
send_ctrl_cmd(0xE200);send_data_cmd(0x02);
send_ctrl_cmd(0xE201);send_data_cmd(0x9C);
send_ctrl_cmd(0xE202);send_data_cmd(0x02);
send_ctrl_cmd(0xE203);send_data_cmd(0xCB);
send_ctrl_cmd(0xE204);send_data_cmd(0x02);
send_ctrl_cmd(0xE205);send_data_cmd(0xEA);
send_ctrl_cmd(0xE206);send_data_cmd(0x03);
send_ctrl_cmd(0xE207);send_data_cmd(0x10);
send_ctrl_cmd(0xE208);send_data_cmd(0x03);
send_ctrl_cmd(0xE209);send_data_cmd(0x27);
send_ctrl_cmd(0xE20A);send_data_cmd(0x03);
send_ctrl_cmd(0xE20B);send_data_cmd(0x40);
send_ctrl_cmd(0xE20C);send_data_cmd(0x03);
send_ctrl_cmd(0xE20D);send_data_cmd(0x4B);
send_ctrl_cmd(0xE20E);send_data_cmd(0x03);
send_ctrl_cmd(0xE20F);send_data_cmd(0x54);
send_ctrl_cmd(0xE300);send_data_cmd(0x03);
send_ctrl_cmd(0xE301);send_data_cmd(0x5A);
send_ctrl_cmd(0xE302);send_data_cmd(0x03);
send_ctrl_cmd(0xE303);send_data_cmd(0x5A);

// Negative Green Gamma
send_ctrl_cmd(0xE400);send_data_cmd(0x00);
send_ctrl_cmd(0xE401);send_data_cmd(0x00);
send_ctrl_cmd(0xE402);send_data_cmd(0x00);
send_ctrl_cmd(0xE403);send_data_cmd(0x1A);
send_ctrl_cmd(0xE404);send_data_cmd(0x00);
send_ctrl_cmd(0xE405);send_data_cmd(0x3F);
send_ctrl_cmd(0xE406);send_data_cmd(0x00);
send_ctrl_cmd(0xE407);send_data_cmd(0x5C);
send_ctrl_cmd(0xE408);send_data_cmd(0x00);
send_ctrl_cmd(0xE409);send_data_cmd(0x6F);
send_ctrl_cmd(0xE40A);send_data_cmd(0x00);
send_ctrl_cmd(0xE40B);send_data_cmd(0x99);
send_ctrl_cmd(0xE40C);send_data_cmd(0x00);
send_ctrl_cmd(0xE40D);send_data_cmd(0xB7);
send_ctrl_cmd(0xE40E);send_data_cmd(0x00);
send_ctrl_cmd(0xE40F);send_data_cmd(0xE8);
send_ctrl_cmd(0xE500);send_data_cmd(0x01);
send_ctrl_cmd(0xE501);send_data_cmd(0x0F);
send_ctrl_cmd(0xE502);send_data_cmd(0x01);
send_ctrl_cmd(0xE503);send_data_cmd(0x4C);
send_ctrl_cmd(0xE504);send_data_cmd(0x01);
send_ctrl_cmd(0xE505);send_data_cmd(0x7C);
send_ctrl_cmd(0xE506);send_data_cmd(0x01);
send_ctrl_cmd(0xE507);send_data_cmd(0xC7);
send_ctrl_cmd(0xE508);send_data_cmd(0x02);
send_ctrl_cmd(0xE509);send_data_cmd(0x03);
send_ctrl_cmd(0xE50A);send_data_cmd(0x02);
send_ctrl_cmd(0xE50B);send_data_cmd(0x05);
send_ctrl_cmd(0xE50C);send_data_cmd(0x02);
send_ctrl_cmd(0xE50D);send_data_cmd(0x3C);
send_ctrl_cmd(0xE50E);send_data_cmd(0x02);
send_ctrl_cmd(0xE50F);send_data_cmd(0x77);
send_ctrl_cmd(0xE600);send_data_cmd(0x02);
send_ctrl_cmd(0xE601);send_data_cmd(0x9C);
send_ctrl_cmd(0xE602);send_data_cmd(0x02);
send_ctrl_cmd(0xE603);send_data_cmd(0xCB);
send_ctrl_cmd(0xE604);send_data_cmd(0x02);
send_ctrl_cmd(0xE605);send_data_cmd(0xEA);
send_ctrl_cmd(0xE606);send_data_cmd(0x03);
send_ctrl_cmd(0xE607);send_data_cmd(0x10);
send_ctrl_cmd(0xE608);send_data_cmd(0x03);
send_ctrl_cmd(0xE609);send_data_cmd(0x27);
send_ctrl_cmd(0xE60A);send_data_cmd(0x03);
send_ctrl_cmd(0xE60B);send_data_cmd(0x40);
send_ctrl_cmd(0xE60C);send_data_cmd(0x03);
send_ctrl_cmd(0xE60D);send_data_cmd(0x4B);
send_ctrl_cmd(0xE60E);send_data_cmd(0x03);
send_ctrl_cmd(0xE60F);send_data_cmd(0x54);
send_ctrl_cmd(0xE700);send_data_cmd(0x03);
send_ctrl_cmd(0xE701);send_data_cmd(0x5A);
send_ctrl_cmd(0xE702);send_data_cmd(0x03);
send_ctrl_cmd(0xE703);send_data_cmd(0x5A);

// Negative Blue Gamma
send_ctrl_cmd(0xE800);send_data_cmd(0x00);
send_ctrl_cmd(0xE801);send_data_cmd(0x00);
send_ctrl_cmd(0xE802);send_data_cmd(0x00);
send_ctrl_cmd(0xE803);send_data_cmd(0x1A);
send_ctrl_cmd(0xE804);send_data_cmd(0x00);
send_ctrl_cmd(0xE805);send_data_cmd(0x3F);
send_ctrl_cmd(0xE806);send_data_cmd(0x00);
send_ctrl_cmd(0xE807);send_data_cmd(0x5C);
send_ctrl_cmd(0xE808);send_data_cmd(0x00);
send_ctrl_cmd(0xE809);send_data_cmd(0x6F);
send_ctrl_cmd(0xE80A);send_data_cmd(0x00);
send_ctrl_cmd(0xE80B);send_data_cmd(0x99);
send_ctrl_cmd(0xE80C);send_data_cmd(0x00);
send_ctrl_cmd(0xE80D);send_data_cmd(0xB7);
send_ctrl_cmd(0xE80E);send_data_cmd(0x00);
send_ctrl_cmd(0xE80F);send_data_cmd(0xE8);
send_ctrl_cmd(0xE900);send_data_cmd(0x01);
send_ctrl_cmd(0xE901);send_data_cmd(0x0F);
send_ctrl_cmd(0xE902);send_data_cmd(0x01);
send_ctrl_cmd(0xE903);send_data_cmd(0x4C);
send_ctrl_cmd(0xE904);send_data_cmd(0x01);
send_ctrl_cmd(0xE905);send_data_cmd(0x7C);
send_ctrl_cmd(0xE906);send_data_cmd(0x01);
send_ctrl_cmd(0xE907);send_data_cmd(0xC7);
send_ctrl_cmd(0xE908);send_data_cmd(0x02);
send_ctrl_cmd(0xE909);send_data_cmd(0x03);
send_ctrl_cmd(0xE90A);send_data_cmd(0x02);
send_ctrl_cmd(0xE90B);send_data_cmd(0x05);
send_ctrl_cmd(0xE90C);send_data_cmd(0x02);
send_ctrl_cmd(0xE90D);send_data_cmd(0x3C);
send_ctrl_cmd(0xE90E);send_data_cmd(0x02);
send_ctrl_cmd(0xE90F);send_data_cmd(0x77);
send_ctrl_cmd(0xEA00);send_data_cmd(0x02);
send_ctrl_cmd(0xEA01);send_data_cmd(0x9C);
send_ctrl_cmd(0xEA02);send_data_cmd(0x02);
send_ctrl_cmd(0xEA03);send_data_cmd(0xCB);
send_ctrl_cmd(0xEA04);send_data_cmd(0x02);
send_ctrl_cmd(0xEA05);send_data_cmd(0xEA);
send_ctrl_cmd(0xEA06);send_data_cmd(0x03);
send_ctrl_cmd(0xEA07);send_data_cmd(0x10);
send_ctrl_cmd(0xEA08);send_data_cmd(0x03);
send_ctrl_cmd(0xEA09);send_data_cmd(0x27);
send_ctrl_cmd(0xEA0A);send_data_cmd(0x03);
send_ctrl_cmd(0xEA0B);send_data_cmd(0x40);
send_ctrl_cmd(0xEA0C);send_data_cmd(0x03);
send_ctrl_cmd(0xEA0D);send_data_cmd(0x4B);
send_ctrl_cmd(0xEA0E);send_data_cmd(0x03);
send_ctrl_cmd(0xEA0F);send_data_cmd(0x54);
send_ctrl_cmd(0xEB00);send_data_cmd(0x03);
send_ctrl_cmd(0xEB01);send_data_cmd(0x5A);
send_ctrl_cmd(0xEB02);send_data_cmd(0x03);
send_ctrl_cmd(0xEB03);send_data_cmd(0x5A);
    
send_ctrl_cmd(0x3500);send_data_cmd(0x00);

send_ctrl_cmd(0x3600);send_data_cmd(0x00);

send_ctrl_cmd(0x4400);send_data_cmd(0x00);

send_ctrl_cmd(0x4401);send_data_cmd(0x50);

send_ctrl_cmd(0x3A00);send_data_cmd(0x66);//66  55  77

//*************************************
// Sleep Out
//*************************************

	send_ctrl_cmd(0x1100);


	MDELAY(200);	
	send_ctrl_cmd(0x2900);
	MDELAY(120);
	send_ctrl_cmd(0x2c00);
	MDELAY(100);
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

    params->io_select_mode = 3; //note:this para is different between 6573 and 6575

    params->dbi.port                    = 0;  //DBI port must be 0 or 1 on mt6575, should not be 2
    params->dbi.data_width              = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dbi.data_format.trans_seq   = LCM_DBI_TRANS_SEQ_MSB_FIRST;
    params->dbi.data_format.padding     = LCM_DBI_PADDING_ON_LSB;
    params->dbi.data_format.format      = LCM_DBI_FORMAT_RGB666;
    params->dbi.data_format.width       = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.cpu_write_bits          = LCM_DBI_CPU_WRITE_16_BITS;
    params->dbi.io_driving_current      = LCM_DRIVING_CURRENT_6575_8MA;

    params->dbi.parallel.write_setup    = 0; //1;
    params->dbi.parallel.write_hold     = 0; //1;
    params->dbi.parallel.write_wait     = 2; //4;
    params->dbi.parallel.read_setup     = 5;
    params->dbi.parallel.read_hold      = 0;
    params->dbi.parallel.read_latency   = 18;
    params->dbi.parallel.wait_period    = 4;
    params->dbi.parallel.cs_high_width  = 0; //cycles of cs high level between each transfer
    // enable tearing-free
//  params->dbi.te_mode                 = 0;
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
    MDELAY(80);
    SET_RESET_PIN(1);
    MDELAY(150);
	//lcm_compare_id();

    init_lcm_registers();
}


static void lcm_suspend(void)
{
    send_ctrl_cmd(0x2800);
	
	 MDELAY(120);

    send_ctrl_cmd(0x1000);
    MDELAY(20);
	
}


static void lcm_resume(void)
{
#if 0
    send_ctrl_cmd(0x1100);
    MDELAY(200);
    //lcm_compare_id();
    // xuecheng, do we need to write 0x2900??
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
    MDELAY(60);
    SET_RESET_PIN(1);
    MDELAY(150);
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




LCM_DRIVER NT35516_rixin_NSF446QH3915_CMO_MCU18_qhd_lcm_drv =
{

        .name	= "NT35516_rixin_NSF446QH3915_CMO_MCU18_qhd",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id     = lcm_compare_id

};
