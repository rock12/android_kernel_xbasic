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
#define LCM_Write_DATA(h,l)  send_data_cmd(h<<8|l)
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
// Select Command Page '1'
  send_ctrl_cmd(0xF000); send_data_cmd(0x55); 
  send_ctrl_cmd(0xF001); send_data_cmd(0xAA);
  send_ctrl_cmd(0xF002); send_data_cmd(0x52);
  send_ctrl_cmd(0xF003); send_data_cmd(0x08);
  send_ctrl_cmd(0xF004); send_data_cmd(0x01);

  send_ctrl_cmd(0xB000); send_data_cmd(0x0D); // etting AVDD Voltage
  send_ctrl_cmd(0xB001); send_data_cmd(0x0D);
  send_ctrl_cmd(0xB002); send_data_cmd(0x0D);

  send_ctrl_cmd(0xB600); send_data_cmd(0x34); // etting AVDD Voltage
  send_ctrl_cmd(0xB601); send_data_cmd(0x34);
  send_ctrl_cmd(0xB602); send_data_cmd(0x34);

  send_ctrl_cmd(0xB100); send_data_cmd(0x0D); // etting AVEE Voltage
  send_ctrl_cmd(0xB101); send_data_cmd(0x0D);
  send_ctrl_cmd(0xB102); send_data_cmd(0x0D);

  send_ctrl_cmd(0xB700); send_data_cmd(0x43); // etting AVDD Voltage
  send_ctrl_cmd(0xB701); send_data_cmd(0x43);
  send_ctrl_cmd(0xB702); send_data_cmd(0x43);

  send_ctrl_cmd(0xB200); send_data_cmd(0x01); // CL Setting for LVGL
  send_ctrl_cmd(0xB201); send_data_cmd(0x01);
  send_ctrl_cmd(0xB202); send_data_cmd(0x01);

  send_ctrl_cmd(0xB800); send_data_cmd(0x24); // ower Control for VCL
  send_ctrl_cmd(0xB801); send_data_cmd(0x24);
  send_ctrl_cmd(0xB802); send_data_cmd(0x24);

  send_ctrl_cmd(0xB300); send_data_cmd(0x05); // etting VGH Voltage
  send_ctrl_cmd(0xB301); send_data_cmd(0x05);
  send_ctrl_cmd(0xB302); send_data_cmd(0x05);

  send_ctrl_cmd(0xB900); send_data_cmd(0x34); // etting VGH Voltage
  send_ctrl_cmd(0xB901); send_data_cmd(0x34);
  send_ctrl_cmd(0xB902); send_data_cmd(0x34);

  send_ctrl_cmd(0xB500); send_data_cmd(0x0B); // etting VGL_REG Voltage
  send_ctrl_cmd(0xB501); send_data_cmd(0x0B);
  send_ctrl_cmd(0xB502); send_data_cmd(0x0B);

  send_ctrl_cmd(0xBA00); send_data_cmd(0x34); // ower Control for VGLX
  send_ctrl_cmd(0xBA01); send_data_cmd(0x34);
  send_ctrl_cmd(0xBA02); send_data_cmd(0x34);

  send_ctrl_cmd(0xBC00); send_data_cmd(0x00); // etting VGMP and VGSP Voltage
  send_ctrl_cmd(0xBC01); send_data_cmd(0x80);
  send_ctrl_cmd(0xBC02); send_data_cmd(0x00);

  send_ctrl_cmd(0xBD00); send_data_cmd(0x00); // etting VGMN and VGSN Voltage
  send_ctrl_cmd(0xBD01); send_data_cmd(0x80);
  send_ctrl_cmd(0xBD02); send_data_cmd(0x00);

  send_ctrl_cmd(0xBE00); send_data_cmd(0x00); // etting VCOM Offset Voltage
  send_ctrl_cmd(0xBE01); send_data_cmd(0x56);

// Select Command Page '0'
  send_ctrl_cmd(0xF000); send_data_cmd(0x55); 
  send_ctrl_cmd(0xF001); send_data_cmd(0xAA);
  send_ctrl_cmd(0xF002); send_data_cmd(0x52);
  send_ctrl_cmd(0xF003); send_data_cmd(0x08);
  send_ctrl_cmd(0xF004); send_data_cmd(0x00);

  send_ctrl_cmd(0xB100); send_data_cmd(0xFC); //F8 isplay Option Control
  send_ctrl_cmd(0xB101); send_data_cmd(0x00);

  send_ctrl_cmd(0x3600); send_data_cmd(0x00);//02
  send_ctrl_cmd(0xB400); send_data_cmd(0x10); // isplay Clock in RGB Interface
  send_ctrl_cmd(0xB500); send_data_cmd(0x50);
  send_ctrl_cmd(0xB600); send_data_cmd(0x0A); // ource Output Data Hold Time Control
  send_ctrl_cmd(0xB700); send_data_cmd(0x00); // Q Control Function for Gate Signals
  send_ctrl_cmd(0xB701); send_data_cmd(0x00);
  send_ctrl_cmd(0xB702); send_data_cmd(0x00);
  send_ctrl_cmd(0xB800); send_data_cmd(0x01); // Q Control Function for Source Driver
  send_ctrl_cmd(0xB801); send_data_cmd(0x05);
  send_ctrl_cmd(0xB802); send_data_cmd(0x05); 
  send_ctrl_cmd(0xB803); send_data_cmd(0x05);

  send_ctrl_cmd(0xBC00); send_data_cmd(0x00); // Q Control Function for Source Driver
  send_ctrl_cmd(0xBC01); send_data_cmd(0x00);
  send_ctrl_cmd(0xBC02); send_data_cmd(0x00);

  send_ctrl_cmd(0xBD00); send_data_cmd(0x01); // isplay Timing Control
  send_ctrl_cmd(0xBD01); send_data_cmd(0x84);
  send_ctrl_cmd(0xBD02); send_data_cmd(0x07);
  send_ctrl_cmd(0xBD03); send_data_cmd(0x31);
  send_ctrl_cmd(0xBD04); send_data_cmd(0x00);
 
  send_ctrl_cmd(0xBA00); send_data_cmd(0x01);
 send_ctrl_cmd(0xCC00); send_data_cmd(0x03);
  send_ctrl_cmd(0xCC01); send_data_cmd(0x00); // isplay Timing Control (Gateless)
  send_ctrl_cmd(0xCC02); send_data_cmd(0x00);

//Positive Gamma for RED
  send_ctrl_cmd(0xD100); send_data_cmd(0x00);
  send_ctrl_cmd(0xD101); send_data_cmd(0x37);
  send_ctrl_cmd(0xD102); send_data_cmd(0x00);
  send_ctrl_cmd(0xD103); send_data_cmd(0x5F);
  send_ctrl_cmd(0xD104); send_data_cmd(0x00);
  send_ctrl_cmd(0xD105); send_data_cmd(0x8A);
  send_ctrl_cmd(0xD106); send_data_cmd(0x00);
  send_ctrl_cmd(0xD107); send_data_cmd(0xA9);
  send_ctrl_cmd(0xD108); send_data_cmd(0x00);
  send_ctrl_cmd(0xD109); send_data_cmd(0xB7);
  send_ctrl_cmd(0xD10A); send_data_cmd(0x00);
  send_ctrl_cmd(0xD10B); send_data_cmd(0xD9);
  send_ctrl_cmd(0xD10C); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10D); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10E); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10F); send_data_cmd(0x31);
  send_ctrl_cmd(0xD110); send_data_cmd(0x01);
  send_ctrl_cmd(0xD111); send_data_cmd(0x56);
  send_ctrl_cmd(0xD112); send_data_cmd(0x01);
  send_ctrl_cmd(0xD113); send_data_cmd(0x91);
  send_ctrl_cmd(0xD114); send_data_cmd(0x01);
  send_ctrl_cmd(0xD115); send_data_cmd(0xC1);
  send_ctrl_cmd(0xD116); send_data_cmd(0x02);
  send_ctrl_cmd(0xD117); send_data_cmd(0x0D);
  send_ctrl_cmd(0xD118); send_data_cmd(0x02);
  send_ctrl_cmd(0xD119); send_data_cmd(0x48);
  send_ctrl_cmd(0xD11A); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11B); send_data_cmd(0x4A);
  send_ctrl_cmd(0xD11C); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11D); send_data_cmd(0x81);
  send_ctrl_cmd(0xD11E); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11F); send_data_cmd(0xbB);
  send_ctrl_cmd(0xD120); send_data_cmd(0x02);
  send_ctrl_cmd(0xD121); send_data_cmd(0xDE);
  send_ctrl_cmd(0xD122); send_data_cmd(0x03);
  send_ctrl_cmd(0xD123); send_data_cmd(0x0C);
  send_ctrl_cmd(0xD124); send_data_cmd(0x03);
  send_ctrl_cmd(0xD125); send_data_cmd(0x2A);
  send_ctrl_cmd(0xD126); send_data_cmd(0x03);
  send_ctrl_cmd(0xD127); send_data_cmd(0x53);
  send_ctrl_cmd(0xD128); send_data_cmd(0x03);
  send_ctrl_cmd(0xD129); send_data_cmd(0x6C);
  send_ctrl_cmd(0xD12A); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12B); send_data_cmd(0x8C);
  send_ctrl_cmd(0xD12C); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12D); send_data_cmd(0x9C);
  send_ctrl_cmd(0xD12E); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12F); send_data_cmd(0xaD);
  send_ctrl_cmd(0xD130); send_data_cmd(0x03);
  send_ctrl_cmd(0xD131); send_data_cmd(0xbF);
  send_ctrl_cmd(0xD132); send_data_cmd(0x03);
  send_ctrl_cmd(0xD133); send_data_cmd(0xC1);

//Positive Gamma for GREEN
   send_ctrl_cmd(0xD100); send_data_cmd(0x00);
  send_ctrl_cmd(0xD101); send_data_cmd(0x37);
  send_ctrl_cmd(0xD102); send_data_cmd(0x00);
  send_ctrl_cmd(0xD103); send_data_cmd(0x5F);
  send_ctrl_cmd(0xD104); send_data_cmd(0x00);
  send_ctrl_cmd(0xD105); send_data_cmd(0x8A);
  send_ctrl_cmd(0xD106); send_data_cmd(0x00);
  send_ctrl_cmd(0xD107); send_data_cmd(0xA9);
  send_ctrl_cmd(0xD108); send_data_cmd(0x00);
  send_ctrl_cmd(0xD109); send_data_cmd(0xB7);
  send_ctrl_cmd(0xD10A); send_data_cmd(0x00);
  send_ctrl_cmd(0xD10B); send_data_cmd(0xD9);
  send_ctrl_cmd(0xD10C); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10D); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10E); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10F); send_data_cmd(0x31);
  send_ctrl_cmd(0xD110); send_data_cmd(0x01);
  send_ctrl_cmd(0xD111); send_data_cmd(0x56);
  send_ctrl_cmd(0xD112); send_data_cmd(0x01);
  send_ctrl_cmd(0xD113); send_data_cmd(0x91);
  send_ctrl_cmd(0xD114); send_data_cmd(0x01);
  send_ctrl_cmd(0xD115); send_data_cmd(0xC1);
  send_ctrl_cmd(0xD116); send_data_cmd(0x02);
  send_ctrl_cmd(0xD117); send_data_cmd(0x0D);
  send_ctrl_cmd(0xD118); send_data_cmd(0x02);
  send_ctrl_cmd(0xD119); send_data_cmd(0x48);
  send_ctrl_cmd(0xD11A); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11B); send_data_cmd(0x4A);
  send_ctrl_cmd(0xD11C); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11D); send_data_cmd(0x81);
  send_ctrl_cmd(0xD11E); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11F); send_data_cmd(0xbB);
  send_ctrl_cmd(0xD120); send_data_cmd(0x02);
  send_ctrl_cmd(0xD121); send_data_cmd(0xDE);
  send_ctrl_cmd(0xD122); send_data_cmd(0x03);
  send_ctrl_cmd(0xD123); send_data_cmd(0x0C);
  send_ctrl_cmd(0xD124); send_data_cmd(0x03);
  send_ctrl_cmd(0xD125); send_data_cmd(0x2A);
  send_ctrl_cmd(0xD126); send_data_cmd(0x03);
  send_ctrl_cmd(0xD127); send_data_cmd(0x53);
  send_ctrl_cmd(0xD128); send_data_cmd(0x03);
  send_ctrl_cmd(0xD129); send_data_cmd(0x6C);
  send_ctrl_cmd(0xD12A); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12B); send_data_cmd(0x8C);
  send_ctrl_cmd(0xD12C); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12D); send_data_cmd(0x9C);
  send_ctrl_cmd(0xD12E); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12F); send_data_cmd(0xaD);
  send_ctrl_cmd(0xD130); send_data_cmd(0x03);
  send_ctrl_cmd(0xD131); send_data_cmd(0xbF);
  send_ctrl_cmd(0xD132); send_data_cmd(0x03);
  send_ctrl_cmd(0xD133); send_data_cmd(0xC1);


//Positive Gamma for BLUE
   send_ctrl_cmd(0xD100); send_data_cmd(0x00);
  send_ctrl_cmd(0xD101); send_data_cmd(0x37);
  send_ctrl_cmd(0xD102); send_data_cmd(0x00);
  send_ctrl_cmd(0xD103); send_data_cmd(0x5F);
  send_ctrl_cmd(0xD104); send_data_cmd(0x00);
  send_ctrl_cmd(0xD105); send_data_cmd(0x8A);
  send_ctrl_cmd(0xD106); send_data_cmd(0x00);
  send_ctrl_cmd(0xD107); send_data_cmd(0xA9);
  send_ctrl_cmd(0xD108); send_data_cmd(0x00);
  send_ctrl_cmd(0xD109); send_data_cmd(0xB7);
  send_ctrl_cmd(0xD10A); send_data_cmd(0x00);
  send_ctrl_cmd(0xD10B); send_data_cmd(0xD9);
  send_ctrl_cmd(0xD10C); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10D); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10E); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10F); send_data_cmd(0x31);
  send_ctrl_cmd(0xD110); send_data_cmd(0x01);
  send_ctrl_cmd(0xD111); send_data_cmd(0x56);
  send_ctrl_cmd(0xD112); send_data_cmd(0x01);
  send_ctrl_cmd(0xD113); send_data_cmd(0x91);
  send_ctrl_cmd(0xD114); send_data_cmd(0x01);
  send_ctrl_cmd(0xD115); send_data_cmd(0xC1);
  send_ctrl_cmd(0xD116); send_data_cmd(0x02);
  send_ctrl_cmd(0xD117); send_data_cmd(0x0D);
  send_ctrl_cmd(0xD118); send_data_cmd(0x02);
  send_ctrl_cmd(0xD119); send_data_cmd(0x48);
  send_ctrl_cmd(0xD11A); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11B); send_data_cmd(0x4A);
  send_ctrl_cmd(0xD11C); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11D); send_data_cmd(0x81);
  send_ctrl_cmd(0xD11E); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11F); send_data_cmd(0xbB);
  send_ctrl_cmd(0xD120); send_data_cmd(0x02);
  send_ctrl_cmd(0xD121); send_data_cmd(0xDE);
  send_ctrl_cmd(0xD122); send_data_cmd(0x03);
  send_ctrl_cmd(0xD123); send_data_cmd(0x0C);
  send_ctrl_cmd(0xD124); send_data_cmd(0x03);
  send_ctrl_cmd(0xD125); send_data_cmd(0x2A);
  send_ctrl_cmd(0xD126); send_data_cmd(0x03);
  send_ctrl_cmd(0xD127); send_data_cmd(0x53);
  send_ctrl_cmd(0xD128); send_data_cmd(0x03);
  send_ctrl_cmd(0xD129); send_data_cmd(0x6C);
  send_ctrl_cmd(0xD12A); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12B); send_data_cmd(0x8C);
  send_ctrl_cmd(0xD12C); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12D); send_data_cmd(0x9C);
  send_ctrl_cmd(0xD12E); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12F); send_data_cmd(0xaD);
  send_ctrl_cmd(0xD130); send_data_cmd(0x03);
  send_ctrl_cmd(0xD131); send_data_cmd(0xbF);
  send_ctrl_cmd(0xD132); send_data_cmd(0x03);
  send_ctrl_cmd(0xD133); send_data_cmd(0xC1);



//Negative Gamma for RED
  send_ctrl_cmd(0xD100); send_data_cmd(0x00);
  send_ctrl_cmd(0xD101); send_data_cmd(0x37);
  send_ctrl_cmd(0xD102); send_data_cmd(0x00);
  send_ctrl_cmd(0xD103); send_data_cmd(0x5F);
  send_ctrl_cmd(0xD104); send_data_cmd(0x00);
  send_ctrl_cmd(0xD105); send_data_cmd(0x8A);
  send_ctrl_cmd(0xD106); send_data_cmd(0x00);
  send_ctrl_cmd(0xD107); send_data_cmd(0xA9);
  send_ctrl_cmd(0xD108); send_data_cmd(0x00);
  send_ctrl_cmd(0xD109); send_data_cmd(0xB7);
  send_ctrl_cmd(0xD10A); send_data_cmd(0x00);
  send_ctrl_cmd(0xD10B); send_data_cmd(0xD9);
  send_ctrl_cmd(0xD10C); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10D); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10E); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10F); send_data_cmd(0x31);
  send_ctrl_cmd(0xD110); send_data_cmd(0x01);
  send_ctrl_cmd(0xD111); send_data_cmd(0x56);
  send_ctrl_cmd(0xD112); send_data_cmd(0x01);
  send_ctrl_cmd(0xD113); send_data_cmd(0x91);
  send_ctrl_cmd(0xD114); send_data_cmd(0x01);
  send_ctrl_cmd(0xD115); send_data_cmd(0xC1);
  send_ctrl_cmd(0xD116); send_data_cmd(0x02);
  send_ctrl_cmd(0xD117); send_data_cmd(0x0D);
  send_ctrl_cmd(0xD118); send_data_cmd(0x02);
  send_ctrl_cmd(0xD119); send_data_cmd(0x48);
  send_ctrl_cmd(0xD11A); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11B); send_data_cmd(0x4A);
  send_ctrl_cmd(0xD11C); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11D); send_data_cmd(0x81);
  send_ctrl_cmd(0xD11E); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11F); send_data_cmd(0xbB);
  send_ctrl_cmd(0xD120); send_data_cmd(0x02);
  send_ctrl_cmd(0xD121); send_data_cmd(0xDE);
  send_ctrl_cmd(0xD122); send_data_cmd(0x03);
  send_ctrl_cmd(0xD123); send_data_cmd(0x0C);
  send_ctrl_cmd(0xD124); send_data_cmd(0x03);
  send_ctrl_cmd(0xD125); send_data_cmd(0x2A);
  send_ctrl_cmd(0xD126); send_data_cmd(0x03);
  send_ctrl_cmd(0xD127); send_data_cmd(0x53);
  send_ctrl_cmd(0xD128); send_data_cmd(0x03);
  send_ctrl_cmd(0xD129); send_data_cmd(0x6C);
  send_ctrl_cmd(0xD12A); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12B); send_data_cmd(0x8C);
  send_ctrl_cmd(0xD12C); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12D); send_data_cmd(0x9C);
  send_ctrl_cmd(0xD12E); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12F); send_data_cmd(0xaD);
  send_ctrl_cmd(0xD130); send_data_cmd(0x03);
  send_ctrl_cmd(0xD131); send_data_cmd(0xbF);
  send_ctrl_cmd(0xD132); send_data_cmd(0x03);
  send_ctrl_cmd(0xD133); send_data_cmd(0xC1);



//Negative Gamma for GERREN
   send_ctrl_cmd(0xD100); send_data_cmd(0x00);
  send_ctrl_cmd(0xD101); send_data_cmd(0x37);
  send_ctrl_cmd(0xD102); send_data_cmd(0x00);
  send_ctrl_cmd(0xD103); send_data_cmd(0x5F);
  send_ctrl_cmd(0xD104); send_data_cmd(0x00);
  send_ctrl_cmd(0xD105); send_data_cmd(0x8A);
  send_ctrl_cmd(0xD106); send_data_cmd(0x00);
  send_ctrl_cmd(0xD107); send_data_cmd(0xA9);
  send_ctrl_cmd(0xD108); send_data_cmd(0x00);
  send_ctrl_cmd(0xD109); send_data_cmd(0xB7);
  send_ctrl_cmd(0xD10A); send_data_cmd(0x00);
  send_ctrl_cmd(0xD10B); send_data_cmd(0xD9);
  send_ctrl_cmd(0xD10C); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10D); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10E); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10F); send_data_cmd(0x31);
  send_ctrl_cmd(0xD110); send_data_cmd(0x01);
  send_ctrl_cmd(0xD111); send_data_cmd(0x56);
  send_ctrl_cmd(0xD112); send_data_cmd(0x01);
  send_ctrl_cmd(0xD113); send_data_cmd(0x91);
  send_ctrl_cmd(0xD114); send_data_cmd(0x01);
  send_ctrl_cmd(0xD115); send_data_cmd(0xC1);
  send_ctrl_cmd(0xD116); send_data_cmd(0x02);
  send_ctrl_cmd(0xD117); send_data_cmd(0x0D);
  send_ctrl_cmd(0xD118); send_data_cmd(0x02);
  send_ctrl_cmd(0xD119); send_data_cmd(0x48);
  send_ctrl_cmd(0xD11A); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11B); send_data_cmd(0x4A);
  send_ctrl_cmd(0xD11C); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11D); send_data_cmd(0x81);
  send_ctrl_cmd(0xD11E); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11F); send_data_cmd(0xbB);
  send_ctrl_cmd(0xD120); send_data_cmd(0x02);
  send_ctrl_cmd(0xD121); send_data_cmd(0xDE);
  send_ctrl_cmd(0xD122); send_data_cmd(0x03);
  send_ctrl_cmd(0xD123); send_data_cmd(0x0C);
  send_ctrl_cmd(0xD124); send_data_cmd(0x03);
  send_ctrl_cmd(0xD125); send_data_cmd(0x2A);
  send_ctrl_cmd(0xD126); send_data_cmd(0x03);
  send_ctrl_cmd(0xD127); send_data_cmd(0x53);
  send_ctrl_cmd(0xD128); send_data_cmd(0x03);
  send_ctrl_cmd(0xD129); send_data_cmd(0x6C);
  send_ctrl_cmd(0xD12A); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12B); send_data_cmd(0x8C);
  send_ctrl_cmd(0xD12C); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12D); send_data_cmd(0x9C);
  send_ctrl_cmd(0xD12E); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12F); send_data_cmd(0xaD);
  send_ctrl_cmd(0xD130); send_data_cmd(0x03);
  send_ctrl_cmd(0xD131); send_data_cmd(0xbF);
  send_ctrl_cmd(0xD132); send_data_cmd(0x03);
  send_ctrl_cmd(0xD133); send_data_cmd(0xC1);


//Negative Gamma for BLUE
   send_ctrl_cmd(0xD100); send_data_cmd(0x00);
  send_ctrl_cmd(0xD101); send_data_cmd(0x37);
  send_ctrl_cmd(0xD102); send_data_cmd(0x00);
  send_ctrl_cmd(0xD103); send_data_cmd(0x5F);
  send_ctrl_cmd(0xD104); send_data_cmd(0x00);
  send_ctrl_cmd(0xD105); send_data_cmd(0x8A);
  send_ctrl_cmd(0xD106); send_data_cmd(0x00);
  send_ctrl_cmd(0xD107); send_data_cmd(0xA9);
  send_ctrl_cmd(0xD108); send_data_cmd(0x00);
  send_ctrl_cmd(0xD109); send_data_cmd(0xB7);
  send_ctrl_cmd(0xD10A); send_data_cmd(0x00);
  send_ctrl_cmd(0xD10B); send_data_cmd(0xD9);
  send_ctrl_cmd(0xD10C); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10D); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10E); send_data_cmd(0x01);
  send_ctrl_cmd(0xD10F); send_data_cmd(0x31);
  send_ctrl_cmd(0xD110); send_data_cmd(0x01);
  send_ctrl_cmd(0xD111); send_data_cmd(0x56);
  send_ctrl_cmd(0xD112); send_data_cmd(0x01);
  send_ctrl_cmd(0xD113); send_data_cmd(0x91);
  send_ctrl_cmd(0xD114); send_data_cmd(0x01);
  send_ctrl_cmd(0xD115); send_data_cmd(0xC1);
  send_ctrl_cmd(0xD116); send_data_cmd(0x02);
  send_ctrl_cmd(0xD117); send_data_cmd(0x0D);
  send_ctrl_cmd(0xD118); send_data_cmd(0x02);
  send_ctrl_cmd(0xD119); send_data_cmd(0x48);
  send_ctrl_cmd(0xD11A); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11B); send_data_cmd(0x4A);
  send_ctrl_cmd(0xD11C); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11D); send_data_cmd(0x81);
  send_ctrl_cmd(0xD11E); send_data_cmd(0x02);
  send_ctrl_cmd(0xD11F); send_data_cmd(0xbB);
  send_ctrl_cmd(0xD120); send_data_cmd(0x02);
  send_ctrl_cmd(0xD121); send_data_cmd(0xDE);
  send_ctrl_cmd(0xD122); send_data_cmd(0x03);
  send_ctrl_cmd(0xD123); send_data_cmd(0x0C);
  send_ctrl_cmd(0xD124); send_data_cmd(0x03);
  send_ctrl_cmd(0xD125); send_data_cmd(0x2A);
  send_ctrl_cmd(0xD126); send_data_cmd(0x03);
  send_ctrl_cmd(0xD127); send_data_cmd(0x53);
  send_ctrl_cmd(0xD128); send_data_cmd(0x03);
  send_ctrl_cmd(0xD129); send_data_cmd(0x6C);
  send_ctrl_cmd(0xD12A); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12B); send_data_cmd(0x8C);
  send_ctrl_cmd(0xD12C); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12D); send_data_cmd(0x9C);
  send_ctrl_cmd(0xD12E); send_data_cmd(0x03);
  send_ctrl_cmd(0xD12F); send_data_cmd(0xaD);
  send_ctrl_cmd(0xD130); send_data_cmd(0x03);
  send_ctrl_cmd(0xD131); send_data_cmd(0xbF);
  send_ctrl_cmd(0xD132); send_data_cmd(0x03);
  send_ctrl_cmd(0xD133); send_data_cmd(0xC1);
 
  send_ctrl_cmd(0x3A00); send_data_cmd(0x66);//55
  send_ctrl_cmd(0x3500); send_data_cmd(0x00);
  send_ctrl_cmd(0x1100); // Sleep out
  MDELAY(200);
  send_ctrl_cmd(0x2900); // Display On
    send_ctrl_cmd(0x2C00);
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


    SET_RESET_PIN(0);
    MDELAY(100);
    SET_RESET_PIN(1);
    MDELAY(120);
	lcm_compare_id();

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



    return (LCM_ID == id&&get_lcd_id()==1)?1:0;

}




LCM_DRIVER nt35510_aizhuoertai_MS_A40230N50BN_BOE_mcu18_wvga_lcm_drv =
{

        .name	= "nt35510_aizhuoertai_MS_A40230N50BN_BOE_mcu18_wvga",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .update         = lcm_update,
        .compare_id     = lcm_compare_id

};
