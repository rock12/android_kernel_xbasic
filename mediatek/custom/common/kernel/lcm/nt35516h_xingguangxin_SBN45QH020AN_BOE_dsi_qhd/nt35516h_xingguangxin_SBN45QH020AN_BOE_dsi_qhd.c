#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
	#include <string.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (540)
#define FRAME_HEIGHT (960)
#define LCM_ID       (0x5516)
#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xFF   // END OF REGISTERS MARKER
#define LCM_DSI_CMD_MODE

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

static unsigned int lcm_esd_test = 0;      ///only for ESD test

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
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)									lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)				lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)     

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

struct LCM_setting_table {
    unsigned char cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_set_window[] = {
	{0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
	{0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {

    // Sleep Out
	{0x11, 1, {0x00}},
    {REGFLAG_DELAY, 250, {}},

    // Display ON
	{0x29, 1, {0x00}},
	{REGFLAG_DELAY, 20, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}},
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},
    {REGFLAG_DELAY, 60, {}},
    // Sleep Mode On
	{0x10, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
	
};


static struct LCM_setting_table lcm_backlight_level_setting[] = {
	///{0x51, 1, {0xFF}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_compare_id_setting[] = {
	// Display off sequence
	
	{0xF0,	5,	{0x55,0xAA,0x52,0x08,0x01}},
	{REGFLAG_DELAY, 10, {}},

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

static struct LCM_setting_table lcm_initialization_setting[] = {
	
	/*
	Note :

	Data ID will depends on the following rule.
	
		count of parameters > 1	=> Data ID = 0x39
		count of parameters = 1	=> Data ID = 0x15
		count of parameters = 0	=> Data ID = 0x05

	Structure Format :

	{DCS command, count of parameters, {parameter list}}
	{REGFLAG_DELAY, milliseconds of time, {}},

	...

	Setting ending by predefined flag
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
	*/


	{0xFF, 4, {0xAA, 0x55, 0x25, 0x01}},
	{0xF3, 7, {0x02, 0x03, 0x07, 0x44, 0x88, 0xD1, 0x0D}},
	{0xF4, 5, {0x00, 0x48, 0x00, 0x00, 0x00}},
	{0xF0, 5, {0x55, 0xAA, 0x52, 0x08, 0x00}}, //Page 0
	{0xBC, 1, {0x00}},
	{0xCC, 1, {0x01}},
	{0xB0, 5, {0x00, 0x0C, 0x40, 0x3C, 0x3C}},
	{0xB7, 2, {0x72, 0x72}},
	{0xBA, 1, {0x05}},
	{0xF0, 5, {0x55, 0xAA, 0x52, 0x08, 0x01}}, //Page1
	{0xB0, 1, {0x0A}}, // AVDD: 5.5V
	{0xB6, 1, {0x54}}, // AVDD: 3.0x
	{0xB1, 1, {0x0A}}, // AVEE: -5.5V
	{0xB7, 1, {0x24}}, // AVEE: -2.0x
	{0xB2, 1, {0x03}}, // VCL: -4.0V
	{0xB8, 1, {0x30}},// VCL: -2.0x
	{0xB3, 1, {0x0D}}, // VGH: 13.5V
	{0xB9, 1, {0x24}}, // VGH: AVDD - AVEE + VDDB
	{0xB4, 1, {0x0A}},// VGLX: -12.0V
	{0xBA, 1, {0x24}}, // VGLX: AVEE + VCL - AVDD0
	{0xB5, 1, {0x07}},
	{0xBB, 1, {0x44}},
	{0xBC, 3, {0x00, 0xA0, 0x01}}, // VGMP: 5.0V, VGSP: 0.3V
	{0xBD, 3, {0x00, 0xA0, 0x01}}, // VGMN: -5.0V, VGSN: -0.3V
	{0xBE, 1, {0X48}},
	//Gamma
	{0xD1, 16, {0x00, 0x00, 0x00, 0x10, 0x00, 0x30, 0x00, 0x5F, 0x00, 0x7D, 0x00, 0xAA, 0x00, 0xDF, 0x01, 0x26}},
	{0xD2, 16, {0x01, 0x53, 0x01, 0x91, 0x01, 0xC2, 0x02, 0x18, 0x02, 0x58, 0x02, 0x5C, 0x02, 0x9E, 0x02, 0xDC}},
	{0xD3, 16, {0x02, 0xFF, 0x03, 0x2D, 0x03, 0x4D, 0x03, 0x77, 0x03, 0x93, 0x03, 0xB7, 0x03, 0xC7, 0x03, 0xE5}},
	{0xD4, 4, {0x03, 0xF5, 0x03, 0xFF}},
	{0xD5, 16, {0x00, 0x00, 0x00, 0x10, 0x00, 0x30, 0x00, 0x5F, 0x00, 0x7D, 0x00, 0xAA, 0x00, 0xDF, 0x01, 0x26}},
	{0xD6, 16, {0x01, 0x53, 0x01, 0x91, 0x01, 0xC2, 0x02, 0x18, 0x02, 0x58, 0x02, 0x5C, 0x02, 0x9E, 0x02, 0xDC}},
	{0xD7, 16, {0x02, 0xFF, 0x03, 0x2D, 0x03, 0x4D, 0x03, 0x77, 0x03, 0x93, 0x03, 0xB7, 0x03, 0xC7, 0x03, 0xE5}},
	{0xD8, 4, {0x03, 0xF5, 0x03, 0xFF}},
	{0xD9, 16, {0x00, 0x00, 0x00, 0x10, 0x00, 0x30, 0x00, 0x5F, 0x00, 0x7D, 0x00, 0xAA, 0x00, 0xDF, 0x01, 0x26}},
	{0xDD, 16, {0x01, 0x53, 0x01, 0x91, 0x01, 0xC2, 0x02, 0x18, 0x02, 0x58, 0x02, 0x5C, 0x02, 0x9E, 0x02, 0xDC}},
	{0xDE, 16, {0x02, 0xFF, 0x03, 0x2D, 0x03, 0x4D, 0x03, 0x77, 0x03, 0x93, 0x03, 0xB7, 0x03, 0xC7, 0x03, 0xE5}},
	{0xDF, 4, {0x03, 0xF5, 0x03, 0xFF}},
	{0xE0, 16, {0x00, 0x00, 0x00, 0x10, 0x00, 0x30, 0x00, 0x5F, 0x00, 0x7D, 0x00, 0xAA, 0x00, 0xDF, 0x01, 0x26}},
	{0xE1, 16, {0x01, 0x53, 0x01, 0x91, 0x01, 0xC2, 0x02, 0x18, 0x02, 0x58, 0x02, 0x5C, 0x02, 0x9E, 0x02, 0xDC}},
	{0xE2, 16, {0x02, 0xFF, 0x03, 0x2D, 0x03, 0x4D, 0x03, 0x77, 0x03, 0x93, 0x03, 0xB7, 0x03, 0xC7, 0x03, 0xE5}},
	{0xE3, 4, {0x03, 0xF5, 0x03, 0xFF}},
	{0xE4, 16, {0x00, 0x00, 0x00, 0x10, 0x00, 0x30, 0x00, 0x5F, 0x00, 0x7D, 0x00, 0xAA, 0x00, 0xDF, 0x01, 0x26}},
	{0xE5, 16, {0x01, 0x53, 0x01, 0x91, 0x01, 0xC2, 0x02, 0x18, 0x02, 0x58, 0x02, 0x5C, 0x02, 0x9E, 0x02, 0xDC}},
	{0xE6, 16, {0x02, 0xFF, 0x03, 0x2D, 0x03, 0x4D, 0x03, 0x77, 0x03, 0x93, 0x03, 0xB7, 0x03, 0xC7, 0x03, 0xE5}},
	{0xE7, 4, {0x03, 0xF5, 0x03, 0xFF}},
	{0xE8, 16, {0x00, 0x00, 0x00, 0x10, 0x00, 0x30, 0x00, 0x5F, 0x00, 0x7D, 0x00, 0xAA, 0x00, 0xDF, 0x01, 0x26}},
	{0xE9, 16, {0x01, 0x53, 0x01, 0x91, 0x01, 0xC2, 0x02, 0x18, 0x02, 0x58, 0x02, 0x5C, 0x02, 0x9E, 0x02, 0xDC}},
	{0xEA, 16, {0x02, 0xFF, 0x03, 0x2D, 0x03, 0x4D, 0x03, 0x77, 0x03, 0x93, 0x03, 0xB7, 0x03, 0xC7, 0x03, 0xE5}},
	{0xEB, 4, {0x03, 0xF5, 0x03, 0xFF}},
	//{0x11
	//DELAY 120
	//{0x29

	{0x11,1,{0x00}},
	{REGFLAG_DELAY, 120, {}},

	{0x29,1,{0x00}},
	{REGFLAG_DELAY, 40, {}},
	
	//{0x2C,1,{0x00}},
	// Setting ending by predefined flag
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

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
	params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
#if defined(LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
#else
		params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif
	
		// DSI
		/* Command mode setting */
		params->dsi.LANE_NUM				= LCM_TWO_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		params->dsi.packet_size=256;

	// Video mode setting
	params->dsi.intermediat_buffer_num = 2;
	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.word_count=540*3;
	params->dsi.vertical_sync_active=3;
	params->dsi.vertical_backporch=12;
	params->dsi.vertical_frontporch=2;
	params->dsi.vertical_active_line=960;

	params->dsi.line_byte=2048;		// 2256 = 752*3
	params->dsi.horizontal_sync_active_byte=26;
	params->dsi.horizontal_backporch_byte=146;
	params->dsi.horizontal_frontporch_byte=146;
	params->dsi.rgb_byte=(540*3+6);

	params->dsi.horizontal_sync_active_word_count=20;
	params->dsi.horizontal_backporch_word_count=140;
	params->dsi.horizontal_frontporch_word_count=140;

	// Bit rate calculation
	params->dsi.pll_div1=1;		// 
	params->dsi.pll_div2=1;			// 
	params->dsi.fbk_div=32;
}


static void lcm_init(void)
{
    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(200);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
	//push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	unsigned int data_array[16];
	unsigned int array[16];
	unsigned char buffer[4]; 

    array[0] = 0x00023700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0x0A, buffer, 2);	

	
	data_array[0]=0x00280500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(50);
	
	data_array[0]=0x00100500;
	dsi_set_cmdq(data_array, 1, 1);	
	MDELAY(150);
}


static void lcm_resume(void)
{
	//add by xia lei jie for LCD INIT	
	lcm_init();
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
	dsi_set_cmdq(data_array, 3, 1);
	
	data_array[0]= 0x00053902;
	data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[2]= (y1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

    data_array[0]= 0x002c3909;
    dsi_set_cmdq(data_array, 1, 0);

	}





static unsigned int lcm_compare_id()
{
	unsigned int id = 0;
	unsigned char buffer[2];
	unsigned int array[16];
  SET_RESET_PIN(1);  //NOTE:should reset LCM firstly
  SET_RESET_PIN(0);
  MDELAY(1);
  SET_RESET_PIN(1);
  MDELAY(10);

	push_table(lcm_compare_id_setting, sizeof(lcm_compare_id_setting) / sizeof(struct LCM_setting_table), 1);

	array[0] = 0x00023700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);

  read_reg_v2(0xc5, buffer, 2);
	id = buffer[0]; 
	id=(id<<8)+buffer[1];
	return (LCM_ID == id)?1:0;
        
}

#if defined (BIRD_ESD_CHECK)
static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK
	static int ok_count = 0;
	 unsigned char buffer[12];
	 unsigned int array[16];
	 int i;
	 
	if(lcm_esd_test)
  {
      lcm_esd_test = 0;
      return 1;
  }
	 	
	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);
	MDELAY(10);
	read_reg_v2(0x0A, buffer, 1);
	 
	printk("mycat$$ dijing ### esd check buffer[0] = 0x%x\r\n",buffer[0]);
	if(buffer[0] == 0x9C)
		return 0;
	else
		return 1;
 			
#endif
}

static unsigned int lcm_esd_recover(void)
{
  lcm_init();
	
#ifndef BUILD_LK	
	printk("mycat dijing ### lcm_esd_recover \r\n");
#endif
	
	return 1;
}
#endif
LCM_DRIVER nt35516h_xingguangxin_SBN45QH020AN_BOE_dsi_qhd_lcm_drv = 
{
  .name			= "nt35516h_xingguangxin_SBN45QH020AN_BOE_dsi_qhd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.update         = lcm_update,
	//.set_backlight	= lcm_setbacklight,
	//.set_pwm        = lcm_setpwm,
	//.get_pwm        = lcm_getpwm  
   .compare_id    = lcm_compare_id,
#if defined (BIRD_ESD_CHECK)//rm esd resolve LCD  flick
	.esd_check = lcm_esd_check,
    .esd_recover = lcm_esd_recover,
#endif

};
