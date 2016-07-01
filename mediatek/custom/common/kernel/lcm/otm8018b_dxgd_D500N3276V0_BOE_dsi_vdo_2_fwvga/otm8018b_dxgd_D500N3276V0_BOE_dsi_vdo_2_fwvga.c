
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
 
#define FRAME_WIDTH                                        (480)
#define FRAME_HEIGHT                                       (854) 
 
#define REGFLAG_DELAY                                       0XFD
#define REGFLAG_END_OF_TABLE                                0xFE   // END OF REGISTERS MARKER
 
#define LCM_DSI_CMD_MODE                                   0
#define LCM_ID                                      0x8009

#define GPIO_LCD_ID_PIN 	GPIO_LCM_ID_PIN

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
 
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)   lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)      lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                     lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                 lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg                                           lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)                      lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)       
 
static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = 

{
	//Gamma2.2

 	// BOE5.0FW+OTM8018B  DXGD
	{0x00,1,{0x00}},
	{0xff,3,{0x80,0x09,0x01}},	// enable EXTC
	
	{0x00,1,{0x80}},  // shift address
	{0xff,2,{0x80,0x09}}, // enable Orise mode

  ///////////////////
	{0x00,1,{0x80}},
	{0xC4,1,{0x30}},//放电代码
	{REGFLAG_DELAY, 10, {}},

	{0x00,1,{0x8A}},
	{0xC4,1,{0x40}},//放电代码
	{REGFLAG_DELAY, 10, {}},
 /////////////////////

	{0x00,1,{0x03}},
	{0xFF,1,{0x01}},

	{0x00,1,{0x90}},
	{0xB3,1,{0x02}},//480*854

	{0x00,1,{0x92}},
	{0xB3,1,{0x45}},

	{0x00,1,{0xa6}},
	{0xB3,2,{0x20,0x01}},

	{0x00,1,{0xA3}},
	{0xC0,1,{0x1B}},

	{0x00,1,{0x80}},  // mclk shift 
	{0xC0,9,{0x00,0x58,0x00,0x14,0x16,0x00,0x58,0x14,0x16}}, 

	{0x00,1,{0xB4}}, // 
	{0xC0,1,{0x50}}, //50---Column Inversion

//{0x00,1,{0xB5}},
//{0xC0,1,{0x18}},//18---Rotate 180

	{0x00,1,{0x81}},
	{0xC4,1,{0x04}},

	{0x00,1,{0x80}},
	{0xC5,1,{0x03}},

	{0x00,1,{0x90}},  // mclk shift 
	{0xC0,6,{0x00,0x44,0x00,0x00,0x00,0x03}},  //20130608	

	{0x00,1,{0x82}},
	{0xC5,1,{0x03}},
	{REGFLAG_DELAY, 1, {}},

	{0x00,1,{0x90}},
	{0xC5,5,{0x96,0x34,0x04,0x7b,0x33}},   //20140311

	{0x00,1,{0x00}},  // GVDD
	{0xD8,1,{0x46}}, //4f               //20130620

	{0x00,1,{0x01}},  // NGVDD 
	{0xD8,1,{0x46}},  //4f               //20130620

	{0x00,1,{0x00}},
	{0xD9,1,{0x1C}}, //1B is OK for VCOM

	{0x00,1,{0x81}},
	{0xC1,1,{0x66}},//77


	{0x00,1,{0x00}},
	{0xE1,16,{0x00,0x07,0x0d,0x0d,0x06,0x0e,0x0a,0x08,0x05,0x08,0x0d,0x09,0x0f,0x17,0x12,0x00}}, //2.2+                              
								// 255 251  247  239   231  203  175  147  108  80   52   24   16   8    4    0      NB
	{0x00,1,{0x00}},
	{0xE2,16,{0x00,0x08,0x0d,0x0d,0x06,0x0e,0x0a,0x08,0x05,0x08,0x0d,0x09,0x0f,0x17,0x12,0x00}}, //2.2-                             
								//V0     V4	V8  V16   V24   V52  V80  V108 V147 V175 V203 V231 V239 V247 V251 V255   NW
											
// CE8x : vst1, vst2, vst3, vst4
	{0x00,1,{0x80}},  // shift address
	{0xce,12,{0x86,0x01,0x00,0x85,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

//CEAx : clka1, clka2
	{0x00,1,{0xa0}},  // shift address
	{0xCE,14,{0x18,0x05,0x83,0x5a,0x86,0x00,0x00,0x18,0x04,0x83,0x5b,0x86,0x00,0x00}},   //20130620

//CEBx : clka3, clka4
	{0x00,1,{0xb0}},  // shift address
	{0xCE,14,{0x18,0x03,0x83,0x5c,0x86,0x00,0x00,0x18,0x02,0x83,0x5d,0x86,0x00,0x00}},    //20130620

//CFCx : 
	{0x00,1,{0xc0}},  // shift address
	{0xCF,10,{0x01,0x01,0x20,0x20,0x00,0x00,0x01,0x01,0x00,0x00}},     //20130620

//CFDx : 
	{0x00,1,{0xd0}},  // shift address
	{0xCF,1,{0x00}},

//--------------------------------------------------------------------------------
//		initial setting 3 < Panel setting >
//--------------------------------------------------------------------------------

// CB8x
	{0x00,1,{0x80}},  // shift address
	{0xCB,10,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},


// cb9x
	{0x00,1,{0x90}},  // shift address
	{0xCB,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

// cbax
	{0x00,1,{0xa0}},  // shift address
	{0xCB,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

// cbbx
	{0x00,1,{0xb0}},  // shift address
	{0xCB,10,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

// cbcx
	{0x00,1,{0xc0}},  // shift address
	{0xCB,15,{0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

// cbdx  
	{0x00,1,{0xD0}},  // shift address
	{0xCB,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00}},
	
// cbex
	{0x00,1,{0xE0}},  // shift address
	{0xCB,10,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	
// cbfx
	{0x00,1,{0xF0}},  // shift address
	{0xCB,10,{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}},
	
// cc8x
	{0x00,1,{0x80}},  // shift address
	{0xCC,10,{0x00,0x26,0x09,0x0b,0x01,0x25,0x00,0x00,0x00,0x00}},
	
// cc9x
	{0x00,1,{0x90}},  // shift address
	{0xCC,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0x0a,0x0c,0x02}},

// ccax
	{0x00,1,{0xA0}},  // shift address
	{0xCC,15,{0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

// ccbx
	{0x00,1,{0xB0}},  // shift address
	{0xCC,10,{0x00,0x25,0x0A,0x0C,0x02,0x26,0x00,0x00,0x00,0x00}},

// cccx
	{0x00,1,{0xC0}},  // shift address
	{0xCC,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x25,0x09,0x0B,0x01}},

// ccdx
	{0x00,1,{0xD0}},  // shift address
	{0xCC,15,{0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

	
	{0x00,1,{0xA0}},
	{0xC1,1,{0xEA}},      //20130608

	{0x00,1,{0xA6}},  // Horizontal sync shift 
	{0xC1,3,{0x01,0x00,0x00}},     //20130608

	{0x00,1,{0xC6}},
	{0xB0,1,{0x03}},      //20130608

	{0x00,1,{0x81}},   
	{0xC5,1,{0x66}},   ////add, Auto pump 2x

	{0x00,1,{0xb6}},  
	{0xF5,1,{0x06}},   // add, VGL_REG timing setting.

	{0x00,1,{0x8B}},   // for ULPM mode
	{0xB0,1,{0x40}},   //20130608

	{0x00,1,{0x00}},
	{0xff,3,{0xff,0xff,0xff}},
		
    {0x00,1,{0x00}}, 
    {0x11,1,{0x00}}, // sleep out 
    {REGFLAG_DELAY, 200, {}},
    
    {0x00,1,{0x00}}, 
    {0x29,1,{0x00}}, // display on 	
    {REGFLAG_DELAY, 50, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};

static int get_lcd_id(void)
{
	mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
	mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
	mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
	mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,0);
	MDELAY(1);
	
	return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}
 
static struct LCM_setting_table lcm_set_window[] = {
   {0x2A,  4,  {0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
   {0x2B,  4,  {0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
   {REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
   {0x11, 1, {0x00}},
   {REGFLAG_DELAY, 200, {}},
 
    // Display ON
   {0x29, 1, {0x00}},
   {REGFLAG_DELAY, 5, {}},
   {REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
   // Sleep Mode On
   {0x10, 1, {0x00}},
   {REGFLAG_DELAY, 200, {}},
   // Display off sequence
   {0x28, 1, {0x00}},
 
   {REGFLAG_END_OF_TABLE, 0x00, {}}
};
 
static struct LCM_setting_table lcm_backlight_level_setting[] = {
   {0x51, 1, {0xFF}},
   {REGFLAG_END_OF_TABLE, 0x00, {}}
};
 
static struct LCM_setting_table lcm_compare_id_setting[] = {
   {0x00,  1,  {0x50}},
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
	params->dbi.te_mode                 = LCM_DBI_TE_MODE_VSYNC_ONLY; ///LCM_DBI_TE_MODE_DISABLED;
	params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;
	
#if (LCM_DSI_CMD_MODE)
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
	// Not support in MT6573
	params->dsi.packet_size=256;
	
	// Video mode setting		
	params->dsi.intermediat_buffer_num = 2;
	
	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
	
	params->dsi.vertical_sync_active				= 4;//2
	params->dsi.vertical_backporch					= 8;
	params->dsi.vertical_frontporch					= 8;
	params->dsi.vertical_active_line				= FRAME_HEIGHT; 

	params->dsi.horizontal_sync_active				= 6;
	params->dsi.horizontal_backporch				= 37;
	params->dsi.horizontal_frontporch				= 37;
	params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

//	params->dsi.PLL_CLOCK=182;
        params->dsi.pll_div1=1;//48;		// fref=26MHz, fvco=fref*(div1+1)	(div1=0~63, fvco=500MHZ~1GHz)
	params->dsi.pll_div2=0; 		// div2=0~15: fout=fvo/(2*div2)
	//params->dsi.fbk_sel=1;	//add
        params->dsi.fbk_div =19;//32
}

static unsigned int lcm_compare_id(void)
{
	int   array[4];
	char  buffer[5];
	char  id_high=0;
	char  id_low=0;
	int   id=0;

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
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(10);
	
	array[0] = 0x00053700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0xA1,buffer, 5);
	
	id_high = buffer[2];            //should be 0x80
	id_low  = buffer[3];            //should be 0x18
	id      = (id_high<<8)|id_low;  //should be 0x8018
	
	Lcd_Log("%s, id = 0x%08x\n", __func__, id); 
	
	return ((LCM_ID == id) && (0 == get_lcd_id())) ? 1 : 0;
}

static void lcm_init(void)
{
	int flag=0;
	
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
	
	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(1);
	SET_RESET_PIN(1);
	MDELAY(10);

    Lcd_Log("mycat otm8018b flag=%d  \n",flag);
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
   push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);

}

static void lcm_resume(void)
{
	
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
   data_array[3]= 0x00053902;
   data_array[4]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
   data_array[5]= (y1_LSB);
   data_array[6]= 0x002c3909;
 
   dsi_set_cmdq(&data_array, 7, 0);
}

static void lcm_setbacklight(unsigned int level)
{
   unsigned int default_level = 145;
   unsigned int mapped_level = 0;
 
   //for LGE backlight IC mapping table
   if(level > 255) 
           level = 255;
 
   if(level >0) 
           mapped_level = default_level+(level)*(255-default_level)/(255);
   else
           mapped_level=0;
 
   // Refresh value of backlight level.
   lcm_backlight_level_setting[0].para_list[0] = mapped_level;
 
   push_table(lcm_backlight_level_setting, sizeof(lcm_backlight_level_setting) / sizeof(struct LCM_setting_table), 1);
}
 
static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK
	char  buffer[3];
	int   array[4];


	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0x0A, buffer, 1);
	printk("mycat dxc otm8018_dxgd esd reg0A=0x%02x\n",buffer[0]);
	if(buffer[0]==0x9C)
	{
		return FALSE;
	}
	else
	{			 
		return TRUE;
	}
#endif

}

static unsigned int lcm_esd_recover(void)
{
	lcm_init();
	return TRUE;
}

LCM_DRIVER otm8018b_dxgd_D500N3276V0_BOE_dsi_vdo_2_fwvga_lcm_drv = 
{
    .name			= "otm8018b_dxgd_D500N3276V0_BOE_dsi_vdo_2_fwvga",
	.set_util_funcs = lcm_set_util_funcs,
    .compare_id    = lcm_compare_id,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
	.esd_check 			= lcm_esd_check,
	.esd_recover 		= lcm_esd_recover,
};
