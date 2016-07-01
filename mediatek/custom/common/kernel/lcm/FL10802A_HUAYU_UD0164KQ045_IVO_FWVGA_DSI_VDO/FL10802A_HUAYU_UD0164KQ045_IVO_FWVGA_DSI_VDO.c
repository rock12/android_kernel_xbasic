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

#define FRAME_WIDTH  										(480)
#define FRAME_HEIGHT 										(854)

#define REGFLAG_DELAY             							0XFFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE									0

#define LCM_ID                                      0x1080
#define GPIO_LCD_ID_PIN 97 

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))
#define BIRD_ESD_CHECK
static unsigned int lcm_esd_test = 0;      ///only for ESD test


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)                      lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)       

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[120];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
/*
{0xB9,3,{0xF1,0x08,0x01 }},	
{0xB1,4,{0x22,0x1A,0x1A,0x87}}, 	
{0xB2,1,{0x22}}, 	
{0xB3,8,{0x01,0x00,0x06,0x06,0x16,0x12,0x37,0x34}}, 	
{0xBA,11,{0x31,0x00 ,0x44 ,0x25 ,0x91 ,0x0A ,0x00 ,0x00 ,0xC1 ,0x00 ,0x00 ,0x00 ,0x0D ,0x02 ,0x4F ,0xB9 ,0xEE }}, 
{0xE3,5,{0x05,0x05,0x01,0x01,0x00}}, 	
{0xB4,1,{0x02}}, 	
{0xB5,2,{0x09,0x09}}, 	
{0xB6,2,{0x6E,0x82}}, 	// Set VCOM
{0xB8,2,{0x64,0x28}}, 	
{0xCC,1,{0X00}}, 	
{0xBC,1,{0x47}}, 	
{0xE9,33,{0x00,0x00,0x0F,0x03,0x69,0x0A,0x8A,0x12,0x31,0x23,0x37,0x11,0x0A,0x8A,0x37,0x00,0x06,0x18,0x00,0x00,0x00,0x10,0x88,0x83,0x11,0x35,0x75,0x78,0x88,0x88,0x88,0x88,0x82,0x00,0x24,0x64,0x68,0x88,0x88,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, 	//GIP
{0xEA,16,{0x90,0x00,0x00,0x00,0x88,0x84,0x60,0x64,0x22,0x08,0x88,0x88,0x88,0x88,0x85,0x71,0x75,0x33,0x18,0x88,0x88,0x88}}, 	// Set GIP2
{0xE0,22,{0x00,0x00,0x11,0x16,0x16,0x1F,0x2D,0x3C,0x0C,0x10,0x12,0x16,0x17,0x15,0x15,0x13,0x17,0x00,0x00,0x11,0x16,0x16,0x1F,0x2D,0x3C,0x0C,0x10,0x12,0x16,0x17,0x15,0x15,0x13,0x17}}, //Gamma	

	{0x11, 1, {0x00}},
  {REGFLAG_DELAY, 120, {}},
  {0x29, 1, {0x00}},
	{REGFLAG_DELAY, 5, {}},
	{0x2c,1,{0x00}},
	{REGFLAG_DELAY, 5, {}},
	

	{REGFLAG_END_OF_TABLE, 0x00, {}}*/
{0xB9,	3,	{0xF1,0x08,0x01}},	
{0xB1,	4,	{0x22,0x1A,0x1A,0x87}},						
{0xB2,	1,	{0x22}},								
{0xB3,	8,	{0x01,0x00,0x06,0x06,0x16,0x12,0x37,0x34}},
{0xBA,	17,	{0x31,0x00,0x44,0x25,0x91,0x0A,0x00,0x00,0xC1,0x00,0x00,0x00,0x0D,0x02,0x4F,0xB9,0xEE}},	
{0xE3,	5,	{0x05,0x05,0x01,0x01,0x00}},	
{0xB4,	1,	{0x02}},
{0xB5,	2,	{0x09,0x09}},
{0xB6,	2,	{0x6c,0x6c}},		//vcom
{0xB8,	2,	{0x64,0x28}},
{0xCC,	1,	{0x00}},						
{0xBC,	1,	{0x47}},							
{0xE9,	51,	{0x00,0x00,0x0f,0x03,0x69,0x0a,0x8a,0x12,0x31,0x23,0x37,0x11,0x0a,0x8a,0x37,0x00,0x06,0x18,0x00,0x00,0x00,0x10,0x88,0x83,0x11,0x35,0x75,0x78,0x88,0x88,0x88,0x88,0x82,0x00,0x24,0x64,0x68,0x88,0x88,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, 									
{0xEA,	22,	{0x90,0x00,0x00,0x00,0x88,0x84,0x60,0x64,0x22,0x08,0x88,0x88,0x88,0x88,0x85,0x71,0x75,0x33,0x18,0x88,0x88,0x88}}, 							
{0xE0,	34,	{0x00,0x00,0x11,0x16,0x16,0x1F,0x2d,0x3c,0x0c,0x10,0x12,0x16,0x17,0x15,0x15,0x13,0x17,0x00,0x00,0x11,0x16,0x16,0x1F,0x2d,0x3c,0x0c,0x10,0x12,0x16,0x17,0x15,0x15,0x13,0x17}},   								
							
{0x11,	1,	{0x00}},
{REGFLAG_DELAY,250,{}},
{0x29,	1,	{0x00}},
{REGFLAG_DELAY,250,{}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_set_window[] = {
	{0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
	{0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 0, {0x00}},
    {REGFLAG_DELAY, 100, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 0, {0x00}},
        {REGFLAG_DELAY, 120, {}},
    // Sleep Mode On
	{0x10, 0, {0x00}},
        {REGFLAG_DELAY, 20, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_backlight_level_setting[] = {
	{0x51, 1, {0xFF}},
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
       params->dbi.te_mode                 = LCM_DBI_TE_MODE_VSYNC_ONLY;
//		params->dbi.te_mode 			= LCM_DBI_TE_MODE_DISABLED; 
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

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

	    params->dsi.vertical_sync_active				= 4;//8
	    params->dsi.vertical_backporch					= 18;//12
	    params->dsi.vertical_frontporch					= 18;//2
	    params->dsi.vertical_active_line				= FRAME_HEIGHT;

	    params->dsi.horizontal_sync_active				= 4;//8
	    params->dsi.horizontal_backporch				= 32;//30
	    params->dsi.horizontal_frontporch				= 32;//30
	    params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

    	// Bit rate calculation
    	params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4
    	params->dsi.pll_div2=1;		// div2=0,1,2,3;div2_real=1,2,4,4
    	params->dsi.fbk_div =28;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)		

		/* ESD or noise interference recovery For video mode LCM only. */
		// Send TE packet to LCM in a period of n frames and check the response.
		//params->dsi.lcm_int_te_monitor = FALSE; 
		//params->dsi.lcm_int_te_period = 1; // Unit : frames 

		// Need longer FP for more opportunity to do int. TE monitor applicably.
		//if(params->dsi.lcm_int_te_monitor) 
		//	params->dsi.vertical_frontporch *= 2; 
		
		// Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.)
		//params->dsi.lcm_ext_te_monitor = FALSE; 
		// Non-continuous clock 
		//params->dsi.noncont_clock = TRUE; 
		//params->dsi.noncont_clock_period = 2; // Unit : frames
}

static void lcm_init(void)
{
	unsigned int data_array[64];

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
    SET_RESET_PIN(0);
    MDELAY(10);//Must > 10ms
    SET_RESET_PIN(1);
    MDELAY(40);//Must > 120ms

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);

	SET_RESET_PIN(0);
	MDELAY(1);
    SET_RESET_PIN(1);

}


static void lcm_resume(void)
{
lcm_init();
	
			//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);

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
    unsigned int id = 0, id2 = 0;
    unsigned char buffer[3];
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
    MDELAY(120);	
    
    /*	
    data_array[0] = 0x00110500;		// Sleep Out
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(120);
    */
    //*************Enable CMD2 Page1  *******************//
    data_array[0]=0x00043902;
    data_array[1]=0x0108F1B9;
    dsi_set_cmdq(data_array, 2, 1);
    MDELAY(20); 
    /*
    data_array[0] = 0x00033700;// read id return two byte,version and id
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(20); 
    */
    read_reg_v2(0x04, buffer, 3);
   // id = buffer[1]; //we only need ID
    id2= buffer[0] << 8 | buffer[1]; //we test buffer 1
    
 
    Lcd_Log("%s, id=0x%x,id2=0x%x\n", __func__, id,id2);
 
 return (LCM_ID == id2)?1:0;
   
   // return (LCM_ID == id2&&get_lcd_id()==1)?1:0;

	   // return (get_lcd_id()==1)?1:0;


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
	//MDELAY(10);
	read_reg_v2(0x0A, buffer, 1);
	 
	printk("mycat$$ truly ### esd check buffer[0] = 0x%x\r\n",buffer[0]);
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
	printk("mycat truly ### lcm_esd_recover \r\n");
#endif
	
	return 1;
}
#endif

LCM_DRIVER FL10802A_HUAYU_UD0164KQ045_IVO_FWVGA_DSI_VDO_lcm_drv = 
{
    .name			= "FL10802A_HUAYU_UD0164KQ045_IVO_FWVGA_DSI_VDO",
	.set_util_funcs = lcm_set_util_funcs,
    .compare_id    = lcm_compare_id,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
#if defined (BIRD_ESD_CHECK)//rm esd resolve LCD  flick
	.esd_check = lcm_esd_check,
  .esd_recover = lcm_esd_recover,
#endif
};

