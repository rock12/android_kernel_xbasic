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
#define LCM_ID       (0x1408)
//#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN

#define   __LCM_TE_ON__
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

//color=0x8410 ;
// color=0xF800 ;
//color=0xFFFF ;
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
  unsigned int id=0, id1=0;
	SET_RESET_PIN(1);
	MDELAY(20);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(100);
	
	send_ctrl_cmd(0x00B0);
	send_data_cmd(0x0004);
	
	send_ctrl_cmd(0xBF);
	read_data_cmd();
	read_data_cmd();
  read_data_cmd();
  id=read_data_cmd();
  id1=read_data_cmd();

#if defined(BUILD_LK)
	printf("mycat READ LCD ID%s, id= 0x%x, id1= 0x%x\n", __func__, id, id1);
#endif

   id = ((id<<8)&0xff00)|id1;
   
	return (LCM_ID == id)?1:0;
	
}

/*static int get_lcd_id(void)
{
	mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
	mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
	mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
	mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,1);
	MDELAY(1);
	
	return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}*/

static unsigned int lcm_compare_id(void);
static void init_lcm_registers(void)
{
		lcm_compare_id();
#if defined(BUILD_LK)
    printf("MYCAT LCD r61408_nisin_NST397WV3938ANP_V1_LG_mcu18_wvga\n");
#endif
		send_ctrl_cmd(0x00B0);
		send_data_cmd(0x0004);
		
		send_ctrl_cmd(0x00B3);
		send_data_cmd(0x0010);
		send_data_cmd(0x0000);
		
		send_ctrl_cmd(0x00BD);
		send_data_cmd(0x0000);
		
		send_ctrl_cmd(0x00C0);
		send_data_cmd(0x0002);
		send_data_cmd(0x0087);
		
		
		send_ctrl_cmd(0x00C1);
		send_data_cmd(0x0003); //SEQ_SEL WCVDC2 EN_6Fh xx REV xx BGR SS
		send_data_cmd(0x0031); //NL[5:0);
		send_data_cmd(0x0039); //BLREV[1:0] PTREV GIPPAT[2:0] GIPMODE[1:0]
		send_data_cmd(0x0021); //GSPF[5:0]
		send_data_cmd(0x0020); //GSPS[5:0]
		send_data_cmd(0x0000); //STVG[1:0] STVGA[1:0] xx T_GALH GLOL[1:0]
		send_data_cmd(0x0010); //DVI[3:0] xx xx FL1 xx
		send_data_cmd(0x0028); //RTN
		send_data_cmd(0x000c); //BPX16 VBPE xx BP[4:0]
		send_data_cmd(0x000c); //EFX16 xx xx FP[4:0]
		send_data_cmd(0x0000); //ACBF1[1:0] ACF[1:0] ACBR[1:0] ACR[1:0]
		send_data_cmd(0x0000); //ACBF2[1:0] ACF2[1:0] ACBR2[1:0] ACR2[1:0]
		send_data_cmd(0x0000); //ACCYC[1:0] VGSET ACFIX[1:0]
		send_data_cmd(0x0021);
		send_data_cmd(0x0001);
		
		send_ctrl_cmd(0x00C2);
		send_data_cmd(0x0000); //LINEINV[2:0] PNSET
		send_data_cmd(0x0003); 
		send_data_cmd(0x0001);
		send_data_cmd(0x0001);
		send_data_cmd(0x0003);
		send_data_cmd(0x0000);
		
		send_ctrl_cmd(0x00C8);
		send_data_cmd(0x0010);
		send_data_cmd(0x0017);
		send_data_cmd(0x001F);
		send_data_cmd(0x0029);
		send_data_cmd(0x0033);
		send_data_cmd(0x004b);
		send_data_cmd(0x003a);
		send_data_cmd(0x0028);
		send_data_cmd(0x0019);
		send_data_cmd(0x0011);
		send_data_cmd(0x000a);
		send_data_cmd(0x0008);
		
		send_data_cmd(0x0010);
		send_data_cmd(0x0017);
		send_data_cmd(0x001F);
		send_data_cmd(0x0029);
		send_data_cmd(0x0033);
		send_data_cmd(0x004b);
		send_data_cmd(0x003a);
		send_data_cmd(0x0028);
		send_data_cmd(0x0019);
		send_data_cmd(0x0011);
		send_data_cmd(0x000a);
		send_data_cmd(0x0008);
		
		send_ctrl_cmd(0x00C9);
		send_data_cmd(0x0010);
		send_data_cmd(0x0017);
		send_data_cmd(0x001F);
		send_data_cmd(0x0029);
		send_data_cmd(0x0033);
		send_data_cmd(0x004b);
		send_data_cmd(0x003a);
		send_data_cmd(0x0028);
		send_data_cmd(0x0019);
		send_data_cmd(0x0011);
		send_data_cmd(0x000a);
		send_data_cmd(0x0008);
		
		send_data_cmd(0x0010);
		send_data_cmd(0x0017);
		send_data_cmd(0x001F);
		send_data_cmd(0x0029);
		send_data_cmd(0x0033);
		send_data_cmd(0x004b);
		send_data_cmd(0x003a);
		send_data_cmd(0x0028);
		send_data_cmd(0x0019);
		send_data_cmd(0x0011);
		send_data_cmd(0x000a);
		send_data_cmd(0x0008);
		
		send_ctrl_cmd(0x00CA);
		send_data_cmd(0x0010);
		send_data_cmd(0x0017);
		send_data_cmd(0x001F);
		send_data_cmd(0x0029);
		send_data_cmd(0x0033);
		send_data_cmd(0x004b);
		send_data_cmd(0x003a);
		send_data_cmd(0x0028);
		send_data_cmd(0x0019);
		send_data_cmd(0x0011);
		send_data_cmd(0x000a);
		send_data_cmd(0x0008);
		
		send_data_cmd(0x0010);
		send_data_cmd(0x0017);
		send_data_cmd(0x001F);
		send_data_cmd(0x0029);
		send_data_cmd(0x0033);
		send_data_cmd(0x004b);
		send_data_cmd(0x003a);
		send_data_cmd(0x0028);
		send_data_cmd(0x0019);
		send_data_cmd(0x0011);
		send_data_cmd(0x000a);
		send_data_cmd(0x0008);
		
		send_ctrl_cmd(0x00D0);
		send_data_cmd(0x0015);
		send_data_cmd(0x0003);
		send_data_cmd(0x00de);
		send_data_cmd(0x0096);
		send_data_cmd(0x0000);    //0C
		send_data_cmd(0x0071);
		send_data_cmd(0x0020);
		send_data_cmd(0x0010);
		send_data_cmd(0x0001);
		send_data_cmd(0x0000);
		send_data_cmd(0x0001);
		send_data_cmd(0x0001);
		send_data_cmd(0x0000);
		send_data_cmd(0x0003);
		send_data_cmd(0x0001);
		send_data_cmd(0x0000);
		
		send_ctrl_cmd(0x00D1);
		send_data_cmd(0x0018);
		send_data_cmd(0x000c);
		send_data_cmd(0x0023);
		send_data_cmd(0x0003);
		send_data_cmd(0x0044);
		send_data_cmd(0x0002);
		send_data_cmd(0x0050);
		
		send_ctrl_cmd(0x00D3);
		send_data_cmd(0x0033);
		
		send_ctrl_cmd(0x00D5);
		send_data_cmd(0x002a);
		send_data_cmd(0x002a);
		
		send_ctrl_cmd(0x00DE);
		send_data_cmd(0x0001);
		send_data_cmd(0x0051);
		
		send_ctrl_cmd(0x00E6);
		send_data_cmd(0x0051);
		MDELAY(100);
		
		send_ctrl_cmd(0x00FA);
		send_data_cmd(0x0003);
		MDELAY(100);
		
		send_ctrl_cmd(0x002A);
		send_data_cmd(0x0000);
		send_data_cmd(0x0000);
		send_data_cmd(0x0001);
		send_data_cmd(0x00DF);
		
		send_ctrl_cmd(0x002B);
		send_data_cmd(0x0000);
		send_data_cmd(0x0000);
		send_data_cmd(0x0003);
		send_data_cmd(0x001f);
		
		send_ctrl_cmd(0x0036);
		send_data_cmd(0x0000);           //ÉÏÏÂ·´
		
		send_ctrl_cmd(0x0035);
		send_data_cmd(0x0000);  
		
		send_ctrl_cmd(0x003A);
		send_data_cmd(0x0066);
		
		send_ctrl_cmd(0x00D6);
		send_data_cmd(0x0028);
		
		send_ctrl_cmd(0x0011);
		MDELAY(20);
		
		send_ctrl_cmd(0x0029);
		MDELAY(40);
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

    params->dbi.parallel.write_setup    = 0;
    params->dbi.parallel.write_hold     = 0;
    params->dbi.parallel.write_wait     = 2;
    params->dbi.parallel.read_setup     = 5;
    params->dbi.parallel.read_hold      = 1;
    params->dbi.parallel.read_latency   = 18;
    params->dbi.parallel.wait_period    = 2;
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
	MDELAY(100);

	SET_RESET_PIN(1);
	MDELAY(20);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(100);

	init_lcm_registers();
	
	sw_clear_panel(0x00);
	MDELAY(10);
}


static void lcm_suspend(void)
{
	send_ctrl_cmd(0x28); // Display off
	MDELAY(10);
	send_ctrl_cmd(0x10); // Sleep in
	MDELAY(120);
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
LCM_DRIVER r61408_nisin_NST397WV3938ANP_V1_LG_mcu18_wvga_lcm_drv = 
{
  .name			= "r61408_nisin_NST397WV3938ANP_V1_LG_mcu18_wvga",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.update         = lcm_update,
	.compare_id    = lcm_compare_id,
};


