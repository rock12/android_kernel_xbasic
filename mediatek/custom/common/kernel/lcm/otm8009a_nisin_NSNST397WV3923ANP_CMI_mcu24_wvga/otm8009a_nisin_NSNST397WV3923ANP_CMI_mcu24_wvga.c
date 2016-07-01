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

#define   __LCM_TE_ON__
#define   __LCM_WINDOW_CFG__

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
	//color=0xF800 ;
	//color=0xFFFF ;
	send_ctrl_cmd(0x2900); 

	send_ctrl_cmd(0x2C00);
    for (y = y0; y <= y1; ++ y) {
        for (x = x0; x <= x1; ++ x) {
            lcm_util.send_data(color);
        }
    }
}


#define WriteCmd send_ctrl_cmd
#define WriteDat send_data_cmd

static void init_lcm_registers(void)
{
WriteCmd(0xff00);WriteDat(0x0080);
WriteCmd(0xff01);WriteDat(0x0009);     //enable EXTC
WriteCmd(0xff02);WriteDat(0x0001);     
WriteCmd(0xff80);WriteDat(0x0080);     //enable Orise mode
WriteCmd(0xff81);WriteDat(0x0009);     
WriteCmd(0xff03);WriteDat(0x0001);     //enable SPI+I2C cmd2 read
MDELAY(200);     

WriteCmd(0xf580);WriteDat(0x0001);
WriteCmd(0xf581);WriteDat(0x0018);
WriteCmd(0xf582);WriteDat(0x0002);
WriteCmd(0xf583);WriteDat(0x0018);
WriteCmd(0xf584);WriteDat(0x0010);
WriteCmd(0xf585);WriteDat(0x0018);
WriteCmd(0xf586);WriteDat(0x0002);
WriteCmd(0xf587);WriteDat(0x0018);
WriteCmd(0xf588);WriteDat(0x000e);
WriteCmd(0xf589);WriteDat(0x0018);
WriteCmd(0xf58A);WriteDat(0x000f);
WriteCmd(0xf58B);WriteDat(0x0020);
MDELAY(10);      

WriteCmd(0xf590);WriteDat(0x0002);
WriteCmd(0xf591);WriteDat(0x0018);
WriteCmd(0xf592);WriteDat(0x0008);
WriteCmd(0xf593);WriteDat(0x0018);
WriteCmd(0xf594);WriteDat(0x0006);
WriteCmd(0xf595);WriteDat(0x0018);
WriteCmd(0xf596);WriteDat(0x000d);
WriteCmd(0xf597);WriteDat(0x0018);
WriteCmd(0xf598);WriteDat(0x000b);
WriteCmd(0xf599);WriteDat(0x0018);

MDELAY(10);      

WriteCmd(0xf5a0);WriteDat(0x0010);
WriteCmd(0xf5a1);WriteDat(0x0018);
WriteCmd(0xf5a2);WriteDat(0x0001);
WriteCmd(0xf5a3);WriteDat(0x0018);
WriteCmd(0xf5a4);WriteDat(0x0018);
WriteCmd(0xf5a5);WriteDat(0x0014);
WriteCmd(0xf5a6);WriteDat(0x0014);
WriteCmd(0xf5a7);WriteDat(0x0018);
MDELAY(10);      


WriteCmd(0xf5b0);WriteDat(0x0014);
WriteCmd(0xf5b1);WriteDat(0x0018);
WriteCmd(0xf5b2);WriteDat(0x0012);
WriteCmd(0xf5b3);WriteDat(0x0018);
WriteCmd(0xf5b4);WriteDat(0x0013);
WriteCmd(0xf5b5);WriteDat(0x0018);
WriteCmd(0xf5b6);WriteDat(0x0011);
WriteCmd(0xf5b7);WriteDat(0x0018);
WriteCmd(0xf5b8);WriteDat(0x0013);
WriteCmd(0xf5b9);WriteDat(0x0018);
WriteCmd(0xf5bA);WriteDat(0x0000);
WriteCmd(0xf5bB);WriteDat(0x0000);
MDELAY(10);      


              
//gamma DC                          
WriteCmd(0xc0b4);WriteDat(0x0050);     //column inversion 	
	               
WriteCmd(0xC0a3);WriteDat(0x001b);     //pre-charge //V02             
WriteCmd(0xC582);WriteDat(0x00A3);     //REG-pump23
WriteCmd(0xC590);WriteDat(0x00d6);     //Pump setting (3x=D6)-->(2x=96)//v02 01/11
WriteCmd(0xC591);WriteDat(0x0076);     //Pump setting(VGH/VGL)   
WriteCmd(0xD800);WriteDat(0x0095);     //GVDD=5V  
WriteCmd(0xD801);WriteDat(0x0095);     //NGVDD=5V   
                 
//VCOMDC                                                                 
WriteCmd(0xD900);WriteDat(0x006c);     // VCOMDC=   
MDELAY(20);                        

WriteCmd(0xC181);WriteDat(0x0066);     //Frame rate 65Hz//V02    

// RGB I/F setting VSYNC for OTM8018 0x0e
WriteCmd(0xC1a1);WriteDat(0x0008);     //external Vsync,Hsync,DE
WriteCmd(0xC0a3);WriteDat(0x001b);     //pre-charge //V02
WriteCmd(0xC481);WriteDat(0x0083);     //source bias //V02
WriteCmd(0xC592);WriteDat(0x0001);     //Pump45
WriteCmd(0xC5B1);WriteDat(0x00A9);     //DC voltage setting ;[0]GVDD output, default: 0xa8  



// Positive         
WriteCmd(0xE100);WriteDat(0x0000);
WriteCmd(0xE101);WriteDat(0x000b);
WriteCmd(0xE102);WriteDat(0x000f);
WriteCmd(0xE103);WriteDat(0x0010);
WriteCmd(0xE104);WriteDat(0x0009);
WriteCmd(0xE105);WriteDat(0x0012);
WriteCmd(0xE106);WriteDat(0x000a);
WriteCmd(0xE107);WriteDat(0x0006);
WriteCmd(0xE108);WriteDat(0x0007);
WriteCmd(0xE109);WriteDat(0x0007);
WriteCmd(0xE10A);WriteDat(0x000a);
WriteCmd(0xE10B);WriteDat(0x0005);
WriteCmd(0xE10C);WriteDat(0x001d);
WriteCmd(0xE10D);WriteDat(0x0017);
WriteCmd(0xE10E);WriteDat(0x000d);
WriteCmd(0xE10F);WriteDat(0x0006);
 
// Negative         
WriteCmd(0xE200);WriteDat(0x0000);
WriteCmd(0xE201);WriteDat(0x000b);
WriteCmd(0xE202);WriteDat(0x000f);
WriteCmd(0xE203);WriteDat(0x0010);
WriteCmd(0xE204);WriteDat(0x000a);
WriteCmd(0xE205);WriteDat(0x0011);
WriteCmd(0xE206);WriteDat(0x000e);
WriteCmd(0xE207);WriteDat(0x000e);
WriteCmd(0xE208);WriteDat(0x0001);
WriteCmd(0xE209);WriteDat(0x0005);
WriteCmd(0xE20A);WriteDat(0x000c);
WriteCmd(0xE20B);WriteDat(0x000a);
WriteCmd(0xE20C);WriteDat(0x0013);
WriteCmd(0xE20D);WriteDat(0x000d);
WriteCmd(0xE20E);WriteDat(0x0009);
WriteCmd(0xE20F);WriteDat(0x0001);
 
					

 

 
// CE8x : vst1, vst2, vst3, vst4
WriteCmd(0xCE80);WriteDat(0x0084);	// ce81[7:0] : vst1_shift[7:0]
WriteCmd(0xCE81);WriteDat(0x0003);	// ce82[7:0] : 0000,	vst1_width[3:0]
WriteCmd(0xCE82);WriteDat(0x0000);	// ce83[7:0] : vst1_tchop[7:0]
WriteCmd(0xCE83);WriteDat(0x0083);	// ce84[7:0] : vst2_shift[7:0]
WriteCmd(0xCE84);WriteDat(0x0003);	// ce85[7:0] : 0000,	vst2_width[3:0]
WriteCmd(0xCE85);WriteDat(0x0000);	// ce86[7:0] : vst2_tchop[7:0]	                                                                                                                      

// CE8x : vst1, vst2, vst3, vst4
WriteCmd(0xCE90);WriteDat(0x0033);	// ce81[7:0] : vst1_shift[7:0]
WriteCmd(0xCE91);WriteDat(0x0027);	// ce82[7:0] : 0000,	vst1_width[3:0]
WriteCmd(0xCE92);WriteDat(0x0000);	// ce83[7:0] : vst1_tchop[7:0]
WriteCmd(0xCE93);WriteDat(0x0033);	// ce84[7:0] : vst2_shift[7:0]
WriteCmd(0xCE94);WriteDat(0x0028);	// ce85[7:0] : 0000,	vst2_width[3:0]
WriteCmd(0xCE95);WriteDat(0x0000);	// ce86[7:0] : vst2_tchop[7:0]	                               
 
//CEAx : clka1, clka2 
WriteCmd(0xCEa0);WriteDat(0x0038);	// cea1[7:0] : clka1_width[3:0], clka1_shift[11:8]
WriteCmd(0xCEa1);WriteDat(0x0002);	// cea2[7:0] : clka1_shift[7:0]
WriteCmd(0xCEa2);WriteDat(0x0003);	// cea3[7:0] : clka1_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
WriteCmd(0xCEa3);WriteDat(0x0021);	// cea4[7:0] : clka1_switch[7:0]                                                                                        
WriteCmd(0xCEa4);WriteDat(0x0000);	// cea5[7:0] : clka1_extend[7:0]                                                                                        
WriteCmd(0xCEa5);WriteDat(0x0000);	// cea6[7:0] : clka1_tchop[7:0]                                                                                         
WriteCmd(0xCEa6);WriteDat(0x0000);	// cea7[7:0] : clka1_tglue[7:0]                                                                                         
WriteCmd(0xCEa7);WriteDat(0x0038);	// cea8[7:0] : clka2_width[3:0], clka2_shift[11:8]                                                                      
WriteCmd(0xCEa8);WriteDat(0x0001);	// cea9[7:0] : clka2_shift[7:0]                                                                                         
WriteCmd(0xCEa9);WriteDat(0x0003);	// ceaa[7:0] : clka2_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
WriteCmd(0xCEaa);WriteDat(0x0022);	// ceab[7:0] : clka2_switch[7:0]                                                                                        
WriteCmd(0xCEab);WriteDat(0x0000);	// ceac[7:0] : clka2_extend                                                                                             
WriteCmd(0xCEac);WriteDat(0x0000);	// cead[7:0] : clka2_tchop                                                                                              
WriteCmd(0xCEad);WriteDat(0x0000);	// ceae[7:0] : clka2_tglue                                                                                              
                                                                                                                                     
//CEBx : clka3, clka4                                                                                                                                     
WriteCmd(0xCEb0);WriteDat(0x0038);	// ceb1[7:0] : clka3_width[3:0], clka3_shift[11:8]                                                                      
WriteCmd(0xCEb1);WriteDat(0x0000);	// ceb2[7:0] : clka3_shift[7:0]                                                                                         
WriteCmd(0xCEb2);WriteDat(0x0003);	// ceb3[7:0] : clka3_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
WriteCmd(0xCEb3);WriteDat(0x0023);	// ceb4[7:0] : clka3_switch[7:0]                                                                                        
WriteCmd(0xCEb4);WriteDat(0x0000);	// ceb5[7:0] : clka3_extend[7:0]                                                                                        
WriteCmd(0xCEb5);WriteDat(0x0000);	// ceb6[7:0] : clka3_tchop[7:0]                                                                                         
WriteCmd(0xCEb6);WriteDat(0x0000);	// ceb7[7:0] : clka3_tglue[7:0]                                                                                         
WriteCmd(0xCEb7);WriteDat(0x0030);	// ceb8[7:0] : clka4_width[3:0], clka2_shift[11:8]                                                                      
WriteCmd(0xCEb8);WriteDat(0x0000);	// ceb9[7:0] : clka4_shift[7:0]                                                                                         
WriteCmd(0xCEb9);WriteDat(0x0003);	// ceba[7:0] : clka4_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
WriteCmd(0xCEba);WriteDat(0x0024);	// cebb[7:0] : clka4_switch[7:0]                                                                                        
WriteCmd(0xCEbb);WriteDat(0x0000);	// cebc[7:0] : clka4_extend                                                                                             
WriteCmd(0xCEbc);WriteDat(0x0000);	// cebd[7:0] : clka4_tchop                                                                                              
WriteCmd(0xCEbd);WriteDat(0x0000);	// cebe[7:0] : clka4_tglue                                                                                              
                                                                                                                                     
//CECx : clkb1, clkb2                                                                                                                                     
WriteCmd(0xCEc0);WriteDat(0x0030);	// cec1[7:0] : clkb1_width[3:0], clkb1_shift[11:8]                                                                      
WriteCmd(0xCEc1);WriteDat(0x0001);	// cec2[7:0] : clkb1_shift[7:0]                                                                                         
WriteCmd(0xCEc2);WriteDat(0x0003);	// cec3[7:0] : clkb1_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
WriteCmd(0xCEc3);WriteDat(0x0025);	// cec4[7:0] : clkb1_switch[7:0]                                                                                        
WriteCmd(0xCEc4);WriteDat(0x0000);	// cec5[7:0] : clkb1_extend[7:0]                                                                                        
WriteCmd(0xCEc5);WriteDat(0x0000);	// cec6[7:0] : clkb1_tchop[7:0]                                                                                         
WriteCmd(0xCEc6);WriteDat(0x0000);	// cec7[7:0] : clkb1_tglue[7:0]                                                                                         
WriteCmd(0xCEc7);WriteDat(0x0030);	// cec8[7:0] : clkb2_width[3:0], clkb2_shift[11:8]                                                                      
WriteCmd(0xCEc8);WriteDat(0x0002);	// cec9[7:0] : clkb2_shift[7:0]                                                                                         
WriteCmd(0xCEc9);WriteDat(0x0003);	// ceca[7:0] : clkb2_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
WriteCmd(0xCEca);WriteDat(0x0026);	// cecb[7:0] : clkb2_switch[7:0]                                                                                        
WriteCmd(0xCEcb);WriteDat(0x0000);	// cecc[7:0] : clkb2_extend                                                                                             
WriteCmd(0xCEcc);WriteDat(0x0000);	// cecd[7:0] : clkb2_tchop                                                                                              
WriteCmd(0xCEcd);WriteDat(0x0000);	// cece[7:0] : clkb2_tglue                                                                                              
            	                                                                                                                        
//CEDx : clkb3, clkb4                                                                                                                                     
WriteCmd(0xCEd0);WriteDat(0x0030);	// ced1[7:0] : clkb3_width[3:0], clkb3_shift[11:8]                                                                      
WriteCmd(0xCEd1);WriteDat(0x0003);	// ced2[7:0] : clkb3_shift[7:0]                                                                                         
WriteCmd(0xCEd2);WriteDat(0x0003);	// ced3[7:0] : clkb3_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
WriteCmd(0xCEd3);WriteDat(0x0027);	// ced4[7:0] : clkb3_switch[7:0]                                                                                        
WriteCmd(0xCEd4);WriteDat(0x0000);	// ced5[7:0] : clkb3_extend[7:0]                                                                                        
WriteCmd(0xCEd5);WriteDat(0x0000);	// ced6[7:0] : clkb3_tchop[7:0]                                                                                         
WriteCmd(0xCEd6);WriteDat(0x0000);	// ced7[7:0] : clkb3_tglue[7:0]                                                                                         
WriteCmd(0xCEd7);WriteDat(0x0030);	// ced8[7:0] : clkb4_width[3:0], clkb4_shift[11:8]                                                                      
WriteCmd(0xCEd8);WriteDat(0x0004);	// ced9[7:0] : clkb4_shift[7:0]                                                                                         
WriteCmd(0xCEd9);WriteDat(0x0003);	// ceda[7:0] : clkb4_sw_tg, odd_high, flat_head, flat_tail, switch[11:8]                                                
WriteCmd(0xCEda);WriteDat(0x0028);	// cedb[7:0] : clkb4_switch[7:0]                                                                                        
WriteCmd(0xCEdb);WriteDat(0x0000);	// cedc[7:0] : clkb4_extend                                                                                             
WriteCmd(0xCEdc);WriteDat(0x0000);	// cedd[7:0] : clkb4_tchop                                                                                              
WriteCmd(0xCEdd);WriteDat(0x0000);	// cede[7:0] : clkb4_tglue                                                                                              
                                                                                                                                     
//CFCx :                                                                                                                                                

WriteCmd(0xCFc7);WriteDat(0x0000);	// cfc8[7:0] : reg_goa_gnd_opt, reg_goa_dpgm_tail_set, reg_goa_f_gating_en, reg_goa_f_odd_gating, toggle_mod1, 2, 3, 4  

 

 
//--------------------------------------------------------------------------------
//		initial setting 3 < Panel setting >
//--------------------------------------------------------------------------------
// cbcx             
WriteCmd(0xCBc0);WriteDat(0x0000);	//cbc1[7:0] : enmode H-byte of sig1  (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBc1);WriteDat(0x0000);	//cbc2[7:0] : enmode H-byte of sig2  (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBc2);WriteDat(0x0000);	//cbc3[7:0] : enmode H-byte of sig3  (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBc3);WriteDat(0x0000);	//cbc4[7:0] : enmode H-byte of sig4  (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBc4);WriteDat(0x0054);	//cbc5[7:0] : enmode H-byte of sig5  (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBc5);WriteDat(0x0054);	//cbc6[7:0] : enmode H-byte of sig6  (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBc6);WriteDat(0x0054);	//cbc7[7:0] : enmode H-byte of sig7  (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBc7);WriteDat(0x0054);	//cbc8[7:0] : enmode H-byte of sig8  (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBc8);WriteDat(0x0000);	//cbc9[7:0] : enmode H-byte of sig9  (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBc9);WriteDat(0x0054);	//cbca[7:0] : enmode H-byte of sig10 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBca);WriteDat(0x0000);	//cbcb[7:0] : enmode H-byte of sig11 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBcb);WriteDat(0x0054);	//cbcc[7:0] : enmode H-byte of sig12 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBcc);WriteDat(0x0000);	//cbcd[7:0] : enmode H-byte of sig13 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBcd);WriteDat(0x0000);	//cbce[7:0] : enmode H-byte of sig14 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBce);WriteDat(0x0000);	//cbcf[7:0] : enmode H-byte of sig15 (pwrof_0, pwrof_1, norm, pwron_4 )                          
                                                                                                              
// cbdx                                                                                                                          
WriteCmd(0xCBd0);WriteDat(0x0000);	//cbd1[7:0] : enmode H-byte of sig16 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBd1);WriteDat(0x0000);	//cbd2[7:0] : enmode H-byte of sig17 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBd2);WriteDat(0x0000);	//cbd3[7:0] : enmode H-byte of sig18 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBd3);WriteDat(0x0000);	//cbd4[7:0] : enmode H-byte of sig19 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBd4);WriteDat(0x0000);	//cbd5[7:0] : enmode H-byte of sig20 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBd5);WriteDat(0x0000);	//cbd6[7:0] : enmode H-byte of sig21 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBd6);WriteDat(0x0000);	//cbd7[7:0] : enmode H-byte of sig22 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBd7);WriteDat(0x0000);	//cbd8[7:0] : enmode H-byte of sig23 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBd8);WriteDat(0x0000);	//cbd9[7:0] : enmode H-byte of sig24 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBd9);WriteDat(0x0054);	//cbda[7:0] : enmode H-byte of sig25 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBda);WriteDat(0x0054);	//cbdb[7:0] : enmode H-byte of sig26 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBdb);WriteDat(0x0054);	//cbdc[7:0] : enmode H-byte of sig27 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBdc);WriteDat(0x0054);	//cbdd[7:0] : enmode H-byte of sig28 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBdd);WriteDat(0x0000);	//cbde[7:0] : enmode H-byte of sig29 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBde);WriteDat(0x0054);	//cbdf[7:0] : enmode H-byte of sig30 (pwrof_0, pwrof_1, norm, pwron_4 )                          
                                      
// cbex                                                  
WriteCmd(0xCBe0);WriteDat(0x0000);	//cbe1[7:0] : enmode H-byte of sig31 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBe1);WriteDat(0x0054);	//cbe2[7:0] : enmode H-byte of sig32 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBe2);WriteDat(0x0000);	//cbe3[7:0] : enmode H-byte of sig33 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBe3);WriteDat(0x0000);	//cbe4[7:0] : enmode H-byte of sig34 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBe4);WriteDat(0x0000);	//cbe5[7:0] : enmode H-byte of sig35 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBe5);WriteDat(0x0000);	//cbe6[7:0] : enmode H-byte of sig36 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBe6);WriteDat(0x0000);	//cbe7[7:0] : enmode H-byte of sig37 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBe7);WriteDat(0x0000);	//cbe8[7:0] : enmode H-byte of sig38 (pwrof_0, pwrof_1, norm, pwron_4 )                          
WriteCmd(0xCBe8);WriteDat(0x0000);	//cbe9[7:0] : enmode H-byte of sig39 (pwrof_0, pwrof_1, norm, pwron_4 )                                                                                                  
                                                                            
                           
// cc8x                                       
WriteCmd(0xCC80);WriteDat(0x0000);	//cc81[7:0] : reg setting for signal01 selection with u2d mode                                   
WriteCmd(0xCC81);WriteDat(0x0000);	//cc82[7:0] : reg setting for signal02 selection with u2d mode                                   
WriteCmd(0xCC82);WriteDat(0x0000);	//cc83[7:0] : reg setting for signal03 selection with u2d mode                                   
WriteCmd(0xCC83);WriteDat(0x0000);	//cc84[7:0] : reg setting for signal04 selection with u2d mode                                   
WriteCmd(0xCC84);WriteDat(0x000C);	//cc85[7:0] : reg setting for signal05 selection with u2d mode                                   
WriteCmd(0xCC85);WriteDat(0x000A);	//cc86[7:0] : reg setting for signal06 selection with u2d mode                                   
WriteCmd(0xCC86);WriteDat(0x0010);	//cc87[7:0] : reg setting for signal07 selection with u2d mode                                   
WriteCmd(0xCC87);WriteDat(0x000E);	//cc88[7:0] : reg setting for signal08 selection with u2d mode                                   
WriteCmd(0xCC88);WriteDat(0x0000);	//cc89[7:0] : reg setting for signal09 selection with u2d mode                                   
WriteCmd(0xCC89);WriteDat(0x0002);	//cc8a[7:0] : reg setting for signal10 selection with u2d mode                                   
                                                                                                              
// cc9x                                                                                                                          
WriteCmd(0xCC90);WriteDat(0x0000);	//cc91[7:0] : reg setting for signal11 selection with u2d mode                                   
WriteCmd(0xCC91);WriteDat(0x0006);	//cc92[7:0] : reg setting for signal12 selection with u2d mode                                   
WriteCmd(0xCC92);WriteDat(0x0000);	//cc93[7:0] : reg setting for signal13 selection with u2d mode                                   
WriteCmd(0xCC93);WriteDat(0x0000);	//cc94[7:0] : reg setting for signal14 selection with u2d mode                                   
WriteCmd(0xCC94);WriteDat(0x0000);	//cc95[7:0] : reg setting for signal15 selection with u2d mode                                   
WriteCmd(0xCC95);WriteDat(0x0000);	//cc96[7:0] : reg setting for signal16 selection with u2d mode                                   
WriteCmd(0xCC96);WriteDat(0x0000);	//cc97[7:0] : reg setting for signal17 selection with u2d mode                                   
WriteCmd(0xCC97);WriteDat(0x0000);	//cc98[7:0] : reg setting for signal18 selection with u2d mode                                   
WriteCmd(0xCC98);WriteDat(0x0000);	//cc99[7:0] : reg setting for signal19 selection with u2d mode                                   
WriteCmd(0xCC99);WriteDat(0x0000);	//cc9a[7:0] : reg setting for signal20 selection with u2d mode                                   
WriteCmd(0xCC9a);WriteDat(0x0000);	//cc9b[7:0] : reg setting for signal21 selection with u2d mode                                   
WriteCmd(0xCC9b);WriteDat(0x0000);	//cc9c[7:0] : reg setting for signal22 selection with u2d mode                                   
WriteCmd(0xCC9c);WriteDat(0x0000);	//cc9d[7:0] : reg setting for signal23 selection with u2d mode                                   
WriteCmd(0xCC9d);WriteDat(0x0000);	//cc9e[7:0] : reg setting for signal24 selection with u2d mode                                   
WriteCmd(0xCC9e);WriteDat(0x000B);	//cc9f[7:0] : reg setting for signal25 selection with u2d mode                                   
                                                                                                              
// ccax                                                                                                                          
WriteCmd(0xCCa0);WriteDat(0x0009);	//cca1[7:0] : reg setting for signal26 selection with u2d mode                                   
WriteCmd(0xCCa1);WriteDat(0x000F);	//cca2[7:0] : reg setting for signal27 selection with u2d mode                                   
WriteCmd(0xCCa2);WriteDat(0x000D);	//cca3[7:0] : reg setting for signal28 selection with u2d mode                                   
WriteCmd(0xCCa3);WriteDat(0x0000);	//cca4[7:0] : reg setting for signal29 selection with u2d mode                                   
WriteCmd(0xCCa4);WriteDat(0x0001);	//cca5[7:0] : reg setting for signal20 selection with u2d mode                                   
WriteCmd(0xCCa5);WriteDat(0x0000);	//cca6[7:0] : reg setting for signal31 selection with u2d mode                                   
WriteCmd(0xCCa6);WriteDat(0x0005);	//cca7[7:0] : reg setting for signal32 selection with u2d mode                                   
WriteCmd(0xCCa7);WriteDat(0x0000);	//cca8[7:0] : reg setting for signal33 selection with u2d mode                                   
WriteCmd(0xCCa8);WriteDat(0x0000);	//cca9[7:0] : reg setting for signal34 selection with u2d mode                                   
WriteCmd(0xCCa9);WriteDat(0x0000);	//ccaa[7:0] : reg setting for signal35 selection with u2d mode                                   
WriteCmd(0xCCaa);WriteDat(0x0000);	//ccab[7:0] : reg setting for signal36 selection with u2d mode                                   
WriteCmd(0xCCab);WriteDat(0x0000);	//ccac[7:0] : reg setting for signal37 selection with u2d mode                                   
WriteCmd(0xCCac);WriteDat(0x0000);	//ccad[7:0] : reg setting for signal38 selection with u2d mode                                   
WriteCmd(0xCCad);WriteDat(0x0000);	//ccae[7:0] : reg setting for signal39 selection with u2d mode                                   
WriteCmd(0xCCae);WriteDat(0x0000);	//ccaf[7:0] : reg setting for signal40 selection with u2d mode                                   
                                                                                                              
// ccbx                                                                                                                          
WriteCmd(0xCCb0);WriteDat(0x0000);	//ccb1[7:0] : reg setting for signal01 selection with d2u mode                                   
WriteCmd(0xCCb1);WriteDat(0x0000);	//ccb2[7:0] : reg setting for signal02 selection with d2u mode                                   
WriteCmd(0xCCb2);WriteDat(0x0000);	//ccb3[7:0] : reg setting for signal03 selection with d2u mode                                   
WriteCmd(0xCCb3);WriteDat(0x0000);	//ccb4[7:0] : reg setting for signal04 selection with d2u mode                                   
WriteCmd(0xCCb4);WriteDat(0x000D);	//ccb5[7:0] : reg setting for signal05 selection with d2u mode                                   
WriteCmd(0xCCb5);WriteDat(0x000F);	//ccb6[7:0] : reg setting for signal06 selection with d2u mode                                   
WriteCmd(0xCCb6);WriteDat(0x0009);	//ccb7[7:0] : reg setting for signal07 selection with d2u mode                                   
WriteCmd(0xCCb7);WriteDat(0x000B);	//ccb8[7:0] : reg setting for signal08 selection with d2u mode                                   
WriteCmd(0xCCb8);WriteDat(0x0000);	//ccb9[7:0] : reg setting for signal09 selection with d2u mode                                   
WriteCmd(0xCCb9);WriteDat(0x0005);	//ccba[7:0] : reg setting for signal10 selection with d2u mode                                   
                                                                                                              
// cccx                                                                                                                          
WriteCmd(0xCCc0);WriteDat(0x0000);	//ccc1[7:0] : reg setting for signal11 selection with d2u mode                                   
WriteCmd(0xCCc1);WriteDat(0x0001);	//ccc2[7:0] : reg setting for signal12 selection with d2u mode                                   
WriteCmd(0xCCc2);WriteDat(0x0000);	//ccc3[7:0] : reg setting for signal13 selection with d2u mode                                   
WriteCmd(0xCCc3);WriteDat(0x0000);	//ccc4[7:0] : reg setting for signal14 selection with d2u mode                                   
WriteCmd(0xCCc4);WriteDat(0x0000);	//ccc5[7:0] : reg setting for signal15 selection with d2u mode                                   
WriteCmd(0xCCc5);WriteDat(0x0000);	//ccc6[7:0] : reg setting for signal16 selection with d2u mode                                   
WriteCmd(0xCCc6);WriteDat(0x0000);	//ccc7[7:0] : reg setting for signal17 selection with d2u mode                                   
WriteCmd(0xCCc7);WriteDat(0x0000);	//ccc8[7:0] : reg setting for signal18 selection with d2u mode                                   
WriteCmd(0xCCc8);WriteDat(0x0000);	//ccc9[7:0] : reg setting for signal19 selection with d2u mode                                   
WriteCmd(0xCCc9);WriteDat(0x0000);	//ccca[7:0] : reg setting for signal20 selection with d2u mode                                   
WriteCmd(0xCCca);WriteDat(0x0000);	//cccb[7:0] : reg setting for signal21 selection with d2u mode                                   
WriteCmd(0xCCcb);WriteDat(0x0000);	//cccc[7:0] : reg setting for signal22 selection with d2u mode                                   
WriteCmd(0xCCcc);WriteDat(0x0000);	//cccd[7:0] : reg setting for signal23 selection with d2u mode                                   
WriteCmd(0xCCcd);WriteDat(0x0000);	//ccce[7:0] : reg setting for signal24 selection with d2u mode                                   
WriteCmd(0xCCce);WriteDat(0x000E);	//cccf[7:0] : reg setting for signal25 selection with d2u mode                                   
                                                                                                              
// ccdx                                                                                                                          
WriteCmd(0xCCd0);WriteDat(0x0010);	//ccd1[7:0] : reg setting for signal26 selection with d2u mode                                   
WriteCmd(0xCCd1);WriteDat(0x000A);	//ccd2[7:0] : reg setting for signal27 selection with d2u mode                                   
WriteCmd(0xCCd2);WriteDat(0x000C);	//ccd3[7:0] : reg setting for signal28 selection with d2u mode                                   
WriteCmd(0xCCd3);WriteDat(0x0000);	//ccd4[7:0] : reg setting for signal29 selection with d2u mode                                   
WriteCmd(0xCCd4);WriteDat(0x0006);	//ccd5[7:0] : reg setting for signal30 selection with d2u mode                                   
WriteCmd(0xCCd5);WriteDat(0x0000);	//ccd6[7:0] : reg setting for signal31 selection with d2u mode                                   
WriteCmd(0xCCd6);WriteDat(0x0002);	//ccd7[7:0] : reg setting for signal32 selection with d2u mode                                   
WriteCmd(0xCCd7);WriteDat(0x0000);	//ccd8[7:0] : reg setting for signal33 selection with d2u mode                                   
WriteCmd(0xCCd8);WriteDat(0x0000);	//ccd9[7:0] : reg setting for signal34 selection with d2u mode                                   
WriteCmd(0xCCd9);WriteDat(0x0000);	//ccda[7:0] : reg setting for signal35 selection with d2u mode                                   
WriteCmd(0xCCda);WriteDat(0x0000);	//ccdb[7:0] : reg setting for signal36 selection with d2u mode                                   
WriteCmd(0xCCdb);WriteDat(0x0000);	//ccdc[7:0] : reg setting for signal37 selection with d2u mode                                   
WriteCmd(0xCCdc);WriteDat(0x0000);	//ccdd[7:0] : reg setting for signal38 selection with d2u mode                                   
WriteCmd(0xCCdd);WriteDat(0x0000);	//ccde[7:0] : reg setting for signal39 selection with d2u mode                    
WriteCmd(0xCCde);WriteDat(0x0000);	//ccdf[7:0] : reg setting for signal40 selection with d2u mode                    
 
///============================init_R61408_LG397
WriteCmd(0x3A00);WriteDat(0x0066);     //MCU 16bits D[17:0]
//WriteCmd(0x3600);WriteDat(0x00C0);
WriteCmd(0x3500);WriteDat(0x0000);
WriteCmd(0x4400);WriteDat(0x0000);
WriteCmd(0x4401);WriteDat(0x0050);


WriteCmd(0x2000);
WriteCmd(0x1100);
MDELAY(200);  
WriteCmd(0x2900);
WriteCmd(0x2c00);
MDELAY(200);      

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


	Lcd_Log("[JACZEN] lcm_suspend\r\n\r\n");	


}


static void lcm_resume(void)
{

	//add by xia lei jie for LCD INIT
	lcm_init();
}

static void lcm_update(unsigned int x, unsigned int y,
		unsigned int width, unsigned int height)
{
	unsigned short x0, y0, x1, y1;
	unsigned short h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;
	Lcd_Log("lcm_update otm8009a_nisin_mcu_wvga\r\n");

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
    unsigned int id,id1,id2,id3,id4;

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

    send_ctrl_cmd(0xA100);
    read_data_cmd();
	read_data_cmd();
	read_data_cmd();
	id = read_data_cmd();
	id = (id<<8) | read_data_cmd();

    Lcd_Log("mycat READ OTM8009 LCD ID%s, id = 0x%x\n", __func__, id);


    return (LCM_ID == id)?1:0;

}
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER otm8009a_nisin_NSNST397WV3923ANP_CMI_mcu24_wvga_lcm_drv = 
{
    .name			= "otm8009a_nisin_NSNST397WV3923ANP_CMI_mcu24_wvga",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.update         = lcm_update,
	.compare_id    = lcm_compare_id,
};


