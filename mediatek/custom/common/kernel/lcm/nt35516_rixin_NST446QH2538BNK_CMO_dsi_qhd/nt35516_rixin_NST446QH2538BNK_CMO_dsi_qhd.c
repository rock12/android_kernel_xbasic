#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#else
	#include <linux/string.h>
	#if defined(BUILD_UBOOT)
		#include <asm/arch/mt_gpio.h>
	#else
		#include <mach/mt_gpio.h>
		#include <mach/mt_pm_ldo.h>
	#endif
#endif
#include "lcm_drv.h"
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  			(540)
#define FRAME_HEIGHT 			(960)

#define REGFLAG_DELAY          	0XFE
#define REGFLAG_END_OF_TABLE  	0xFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE			0


#ifndef TRUE
    #define   TRUE     1
#endif
 
#ifndef FALSE
    #define   FALSE    0
#endif



// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    	(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 		(lcm_util.udelay(n))
#define MDELAY(n) 		(lcm_util.mdelay(n))



// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)						lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)		lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg					lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)		lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static unsigned int lcm_compare_id(void);


static struct LCM_setting_table lcm_initialization_setting[] = {
//*************************************
{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
//#Source hold time
{0xB6,1,{0x05}},
//#Gate EQ control
{0xB7,2,{0x72,0x72}},
//#Source EQ control (Mode 2)
{0xB8,4,{0x01,0x04,0x04,0x04}},
//// Inversion
{0xBC,3,{0x00,0x00,0x00}},
//// Display Timing: Dual 8-phase 4-overlap
{0xC7,11,{0x00,0xFF,0xFF,0x10,0x10,0x0A,0x0B,0x0A,0x0A,0xFF,0xFF}},
//*************************************
// Select CMD2, Page 1
//*************************************
{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},
// AVDD: 5.5V
{0xB0,3,{0x0A,0x0A,0x0A}},
// AVEE: -5.5V
{0xB1,3,{0x0A,0x0A,0x0A}},
// VCL: -3.5V
{0xB2,3,{0x02,0x02,0x02}},
// VGH: 15.0V
{0xB3,3,{0x10,0x10,0x10}},
// VGLX: -10.0V
{0xB4,3,{0x06,0x06,0x06}},
// AVDD: 2xVDDB
{0xB6,3,{0x34,0x34,0x34}},
// AVEE: -2.5xVDDB
{0xB7,3,{0x34,0x34,0x34}},
// VCL: -1.5xVDDB
{0xB8,3,{0x20,0x20,0x20}},
// VGH: 2xAVDD-AVEE
{0xB9,3,{0x35,0x35,0x35}},
// VGLX: AVEE-AVDD
{0xBA,3,{0x14,0x14,0x14}},
// Set VGMP = 5V / VGSP = 0V
{0xBC,3,{0x00,0xA0,0x00}},
// Set VGMN = -5V / VGSN = 0V
{0xBD,3,{0x00,0xA0,0x00}},
// Set VCOM_offset -1.6625V
{0xBE,1,{0x6D}},
// Pump:0x00
{0xC2,1,{0x00}},
// Gamma Gradient Control
{0xD0,4,{0x0F,0x0F,0x10,0x10}},    
// Positive Red Gamma
{0xD1,16,{0x00,0x00,0x00,0x1A,0x00,0x3F,0x00,0x5C,0x00,0x6F,0x00,0x99,0x00,0xB7,0x00,0xE8}},
{0xD2,16,{0x01,0x0F,0x01,0x4C,0x01,0x7C,0x01,0xC7,0x02,0x03,0x02,0x05,0x02,0x3C,0x02,0x77}},
{0xD3,16,{0x02,0x9C,0x02,0xCB,0x02,0xEA,0x03,0x10,0x03,0x27,0x03,0x40,0x03,0x4B,0x03,0x54}},
{0xD4,4,{0x03,0x5A,0x03,0x5A}},
// Positive Green Gamma
{0xD5,16,{0x00,0x00,0x00,0x1A,0x00,0x3F,0x00,0x5C,0x00,0x6F,0x00,0x99,0x00,0xB7,0x00,0xE8}},
{0xD6,16,{0x01,0x0F,0x01,0x4C,0x01,0x7C,0x01,0xC7,0x02,0x03,0x02,0x05,0x02,0x3C,0x02,0x77}},
{0xD7,16,{0x02,0x9C,0x02,0xCB,0x02,0xEA,0x03,0x10,0x03,0x27,0x03,0x40,0x03,0x4B,0x03,0x54}},
{0xD8,4,{0x03,0x5A,0x03,0x5A}},
// Positive Blue Gamma
{0xD9,16,{0x00,0x00,0x00,0x1A,0x00,0x3F,0x00,0x5C,0x00,0x6F,0x00,0x99,0x00,0xB7,0x00,0xE8}},
{0xDD,16,{0x01,0x0F,0x01,0x4C,0x01,0x7C,0x01,0xC7,0x02,0x03,0x02,0x05,0x02,0x3C,0x02,0x77}},
{0xDE,16,{0x02,0x9C,0x02,0xCB,0x02,0xEA,0x03,0x10,0x03,0x27,0x03,0x40,0x03,0x4B,0x03,0x54}},
{0xDF,4,{0x03,0x5A,0x03,0x5A}},
// Negative Red Gamma
{0xE0,16,{0x00,0x00,0x00,0x1A,0x00,0x3F,0x00,0x5C,0x00,0x6F,0x00,0x99,0x00,0xB7,0x00,0xE8}},
{0xE1,16,{0x01,0x0F,0x01,0x4C,0x01,0x7C,0x01,0xC7,0x02,0x03,0x02,0x05,0x02,0x3C,0x02,0x77}},
{0xE2,16,{0x02,0x9C,0x02,0xCB,0x02,0xEA,0x03,0x10,0x03,0x27,0x03,0x40,0x03,0x4B,0x03,0x54}},
{0xE3,4,{0x03,0x5A,0x03,0x5A}},                                                           
// Negative Green Gamma
{0xE4,16,{0x00,0x00,0x00,0x1A,0x00,0x3F,0x00,0x5C,0x00,0x6F,0x00,0x99,0x00,0xB7,0x00,0xE8}},
{0xE5,16,{0x01,0x0F,0x01,0x4C,0x01,0x7C,0x01,0xC7,0x02,0x03,0x02,0x05,0x02,0x3C,0x02,0x77}},
{0xE6,16,{0x02,0x9C,0x02,0xCB,0x02,0xEA,0x03,0x10,0x03,0x27,0x03,0x40,0x03,0x4B,0x03,0x54}},
{0xE7,4,{0x03,0x5A,0x03,0x5A}},                                                            
// Negative Blue Gamma
{0xE8,16,{0x00,0x00,0x00,0x1A,0x00,0x3F,0x00,0x5C,0x00,0x6F,0x00,0x99,0x00,0xB7,0x00,0xE8}},
{0xE9,16,{0x01,0x0F,0x01,0x4C,0x01,0x7C,0x01,0xC7,0x02,0x03,0x02,0x05,0x02,0x3C,0x02,0x77}},
{0xEA,16,{0x02,0x9C,0x02,0xCB,0x02,0xEA,0x03,0x10,0x03,0x27,0x03,0x40,0x03,0x4B,0x03,0x54}},
{0xEB,4,{0x03,0x5A,0x03,0x5A}},   
{0x11, 0, {0x00}},
{REGFLAG_DELAY, 120, {}},
{0x29, 0, {0x00}},
{REGFLAG_DELAY, 50, {}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) {
		
        unsigned cmd;
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
	
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
    
    params->type   = LCM_TYPE_DSI;
    
    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;
    
    // enable tearing-free
//    params->dbi.te_mode				= LCM_DBI_TE_MODE_VSYNC_ONLY;
//    params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

    params->dsi.mode   = BURST_VDO_MODE;
   
    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_TWO_LANE;
    
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq	= LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding 	= LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format	  = LCM_DSI_FORMAT_RGB888;
    
    // Video mode setting		
    params->dsi.intermediat_buffer_num = 2;
    
    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
    
    params->dsi.word_count=FRAME_WIDTH*3;	//DSI CMD mode need set these two bellow params, different to 6577
    params->dsi.vertical_active_line=FRAME_HEIGHT;

	params->dsi.vertical_sync_active	= 4;// 3    2
	params->dsi.vertical_backporch		= 60;// 20   1
	params->dsi.vertical_frontporch		= 60; // 1  12
	params->dsi.vertical_active_line	= FRAME_HEIGHT; 

	params->dsi.horizontal_sync_active	= 4;// 50  2
	params->dsi.horizontal_backporch	= 60;
	params->dsi.horizontal_frontporch	= 60;
  params->dsi.horizontal_blanking_pixel	= 60;
	params->dsi.horizontal_active_pixel	= FRAME_WIDTH;

	// Bit rate calculation
	//1 Every lane speed
	params->dsi.pll_div1 = 1;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps  0
	params->dsi.pll_div2 = 0;		// div2=0,1,2,3;div1_real=1,2,4,4	                1
	params->dsi.fbk_div = 19;      // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)      17

}


static void lcm_init(void)
{
#if defined(BUILD_LK)
		upmu_set_rg_vgp2_vosel(5);
		upmu_set_rg_vgp2_en(1);
		upmu_set_rg_vgp3_vosel(3);
		upmu_set_rg_vgp3_en(1);	
#else
		hwPowerOn(MT6323_POWER_LDO_VGP2, VOL_2800, "Lance_LCM");
		hwPowerOn(MT6323_POWER_LDO_VGP3, VOL_1800, "Lance_LCM");
#endif
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(150);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	dsi_set_cmdq(&data_array, 3, 1);
	//MDELAY(1);
	
	data_array[0]= 0x00053902;
	data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[2]= (y1_LSB);
	dsi_set_cmdq(&data_array, 3, 1);
	//MDELAY(1);
	
	data_array[0]= 0x00290508;
	dsi_set_cmdq(&data_array, 1, 1);
	//MDELAY(1);
	
	data_array[0]= 0x002c3909;
	dsi_set_cmdq(&data_array, 1, 0);
	//MDELAY(1);

}

static void lcm_suspend(void)
{
	unsigned int data_array[16];
	data_array[0]=0x00280500;
	dsi_set_cmdq(&data_array,1,1);
	MDELAY(10);
	data_array[0]=0x00100500;
	dsi_set_cmdq(&data_array,1,1);
	MDELAY(100);
}


static void lcm_resume(void)
{
	unsigned int data_array[16];
	data_array[0]=0x00110500;
	dsi_set_cmdq(&data_array,1,1);
	MDELAY(100);
	data_array[0]=0x00290500;
	dsi_set_cmdq(&data_array,1,1);
	MDELAY(10);
}


static unsigned int lcm_compare_id(void)
{
	unsigned int id = 0,id1=0,id2 = 0;
	unsigned char buffer[2]; 
	unsigned int data_array[16];
#if defined(BUILD_LK)
    upmu_set_rg_vgp2_vosel(5);
    upmu_set_rg_vgp2_en(1);

    upmu_set_rg_vgp3_vosel(3);
    upmu_set_rg_vgp3_en(1);
#else
    hwPowerOn(MT6323_POWER_LDO_VGP2, VOL_2800, "Lance_LCM");
    hwPowerOn(MT6323_POWER_LDO_VGP3, VOL_1800, "Lance_LCM");
#endif
    MDELAY(100);

	SET_RESET_PIN(1);  //NOTE:should reset LCM firstly 
	MDELAY(10); 
	SET_RESET_PIN(0);
	MDELAY(10); 
	SET_RESET_PIN(1); 
	MDELAY(10);        
	//*************Enable CMD2 Page1  *******************//
	
	data_array[0]=0x00063902;
	data_array[1]=0x52AA55F0;
	data_array[2]=0x00000108;
	dsi_set_cmdq(data_array, 3, 1); 
	MDELAY(10); 
	data_array[0] = 0x00023700;// read id return two byte,version and id
	dsi_set_cmdq(data_array, 1, 1); 
	MDELAY(10); 
	read_reg_v2(0xC5, buffer, 2); 
	id1 = buffer[0]; //we only need ID
	id2= buffer[1]; //we test buffer 1
	id = (id1<<8) | id2;
	return (0x5516 == id)?1:0;

}

static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK
	int array[4];
	char buffer[3]={0};
	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0x0A, buffer, 1);
	if(0x9c == buffer[0])
		return FALSE;
	else if(0x02 == buffer[0])
	{
		array[0] = 0x00013700;
		dsi_set_cmdq(array, 1, 1);
		read_reg_v2(0x0A, buffer, 1);
		if(0x9c == buffer[0])
			return FALSE;
		else if(0x02 == buffer[0])
		{
			printk(KERN_ERR "nt35517:read register 0x0A=%4x\n",buffer[0]);
			return TRUE;
		}
	}
#endif
}

static unsigned int lcm_esd_recover(void)
{
    lcm_init();
    return TRUE;
}


LCM_DRIVER nt35516_rixin_NST446QH2538BNK_CMO_dsi_qhd_lcm_drv = 
{
	.name		= "nt35516_rixin_NST446QH2538BNK_CMO_dsi_qhd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
#if (LCM_DSI_CMD_MODE)
	.compare_id    = lcm_compare_id,
	.update         = lcm_update,
#endif
   // .esd_check      = lcm_esd_check,
   // .esd_recover    = lcm_esd_recover,
};

