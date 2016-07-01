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

static unsigned int lcm_esd_test = 0;      ///only for ESD test

#include "lcm_drv.h"


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(480)
#define FRAME_HEIGHT 										(854)

#define REGFLAG_DELAY             							0XFFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE									0

#define LCM_ID                                      0x9816
#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN 

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


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)                      lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)       

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


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

	
	{0xff,3,{0xff,0x98,0x16}},
	{REGFLAG_DELAY, 5, {}},

	{0xba,1,{0x60}},
	{REGFLAG_DELAY, 5, {}},

	{0xb0,1,{0x01}},
	{REGFLAG_DELAY, 5, {}},
	
		{0xC7,1,{0x75}},//6F
	{REGFLAG_DELAY, 5, {}},
	
	{0xbc,18,{0x03,0x0d,0x03,0x63,0x01,0x01,0x1b,0x11,0x6e,0x00,0x00,0x00,0x01,0x01,0x16,0x00,0xff,0xf2}},
	{REGFLAG_DELAY, 5, {}},

   {0xbd,8,{0x02,0x13,0x45,0x67,0x45,0x67,0x01,0x23}},
	{REGFLAG_DELAY, 5, {}},

	{0xbe,17,{0x03,0x22,0x22,0x22,0x22,0xdd,0xcc,0xbb,0xaa,0x66,0x77,0x22,0x22,0x22,0x22,0x22,0x22}},
	{REGFLAG_DELAY, 5, {}},

  {0xed,2,{0x7f,0x0f}},
	{REGFLAG_DELAY, 5, {}},

	{0xF3,1,{0x70}},
	{REGFLAG_DELAY, 5, {}}, 
	
	{0xB4,2,{0x02,0x02}},	//02
	{REGFLAG_DELAY, 5, {}},
	
	{0xC0,3,{0x0F,0x0b,0x0a}},
	{REGFLAG_DELAY, 5, {}},
	
	{0xC1,4,{0x17,0x88,0x70,0x20}},
	{REGFLAG_DELAY, 5, {}},
	
	{0xd8,1,{0x50}},
	{REGFLAG_DELAY, 5, {}},
	
	{0xFC,1,{0x07}},
	{REGFLAG_DELAY, 5, {}},
	
	{0xE0,16,{0x00,0x04,0x12,0x11,0x13,0x1e,0xC8,0x08,0x02,0x09,0x03,0x0c,0x0c,0x2d,0x2a,0x00}},
	{REGFLAG_DELAY, 5, {}},
	
	{0xE1,16,{0x00,0x02,0x07,0x0d,0x11,0x16,0x7A,0x09,0x05,0x09,0x06,0x0c,0x0B,0x2f,0x2a,0x00}},
	{REGFLAG_DELAY, 5, {}},
	
	{0xD5,8,{0x0d,0x08,0x08,0x09,0xCB,0xA5,0x01,0x04}},
	{REGFLAG_DELAY, 5, {}},
	
	{0xF7,1,{0x89}},
	{REGFLAG_DELAY, 5, {}},
	

	
	{0x3A,1,{0x77}},
	{REGFLAG_DELAY, 5, {}},
	
	{0x35,    1,   {0x00}},//TE

	//{0x11, 1, {0x00}},
  //{REGFLAG_DELAY, 120, {}},
  //{0x29, 1, {0x00}},
	//{REGFLAG_DELAY, 5, {}},
	//{0x2c,1,{0x00}},
	//{REGFLAG_DELAY, 5, {}},
	
	#if defined(BIRD_ESD_CHECK)
{0xd6,8, {0xff,0xa0,0x88,0x14,0x04,0x64,0x28,0x3a}},//added for after add esd function ,the scree is flashing
#endif
 
{0x11,1, {0x00}},     
{REGFLAG_DELAY, 120, {}},                    
#if defined(BIRD_ESD_CHECK)
{0xEE,9, {0x0A, 0x1B, 0x5F, 0x40, 0x00, 0x00, 0x10, 0x00, 0x58}},//added for esd read reg maliejun 20130823
#endif
	
  {0x29, 1, {0x00}},
  {0x2c,1,{0x00}},
	{REGFLAG_DELAY, 5, {}},
		// Note
	// Strongly recommend not to set Sleep out / Display On here. That will cause messed frame to be shown as later the backlight is on.


	// Setting ending by predefined flag
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_set_window[] = {
	{0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
	{0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 1, {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},

    // Sleep Mode On
	{0x10, 1, {0x00}},
  {REGFLAG_DELAY, 50, {}},
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

	    params->dsi.vertical_sync_active				= 8;
	    params->dsi.vertical_backporch					= 12;
	    params->dsi.vertical_frontporch					= 2;
	    params->dsi.vertical_active_line				= FRAME_HEIGHT;

	    params->dsi.horizontal_sync_active				= 8;
	    params->dsi.horizontal_backporch				= 80;
	    params->dsi.horizontal_frontporch				= 80;
	    params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

    	// Bit rate calculation
    	params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4
    	params->dsi.pll_div2=0;		// div2=0,1,2,3;div2_real=1,2,4,4
    	params->dsi.fbk_div =15;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)		

		/* ESD or noise interference recovery For video mode LCM only. */
		// Send TE packet to LCM in a period of n frames and check the response.
		//params->dsi.lcm_int_te_monitor = FALSE;
		//params->dsi.lcm_int_te_period = 1;		// Unit : frames

		// Need longer FP for more opportunity to do int. TE monitor applicably.
		//if(params->dsi.lcm_int_te_monitor)
		//	params->dsi.vertical_frontporch *= 2;
		
		// Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.)
    //       #ifdef BIRD_ESD_CHECK
          
     //      params->dsi.lcm_ext_te_monitor = 1;// FALSE;
     //      #else
     //      params->dsi.lcm_ext_te_monitor = 0;
     //      #endif
		// Non-continuous clock
		//params->dsi.noncont_clock = FALSE;
	  //params->dsi.noncont_clock_period = 2;	// Unit : frames
}

static void lcm_init(void)
{
	  
//#ifdef BUILD_LK
//	pmic_config_interface(0x0532,5,0x7,5);//add by libo for VGP2 POWER ON
//	pmic_config_interface(0x050C,1,0x1,15);
#ifdef BUILD_LK
	upmu_set_rg_vgp2_vosel(5);
	upmu_set_rg_vgp2_en(1);
	MDELAY(200);
	upmu_set_rg_vgp2_en(0);
	MDELAY(30);
	upmu_set_rg_vgp2_vosel(3);//1.8v
	upmu_set_rg_vgp2_en(1);
	MDELAY(50);
	upmu_set_rg_vgp2_vosel(4);//2.5v
	upmu_set_rg_vgp2_en(1);
	MDELAY(50);
	upmu_set_rg_vgp2_vosel(5);//2.8v
	upmu_set_rg_vgp2_en(1);
	MDELAY(100);
#else
	hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
#endif
    MDELAY(100);
	
    SET_RESET_PIN(1);
    MDELAY(25);
    SET_RESET_PIN(0);
    MDELAY(50); 
    SET_RESET_PIN(1);
    MDELAY(100); 

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);

	//SET_RESET_PIN(0);
//	MDELAY(1);
//    SET_RESET_PIN(1);
}


static void lcm_resume(void)
{
	//lcm_initialization_setting[3].para_list[0]+=1;
	//Lcd_Log("WANGDONG vcom id = 0x%02x", lcm_initialization_setting[3].para_list[0]);
	//lcm_init();
	
	push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
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
  /*  unsigned int id = 0, id2 = 0;
    unsigned char buffer[3];
    unsigned int data_array[16];

#ifdef BUILD_LK
		  pmic_config_interface(0x0532,5,0x7,5);//add by libo for VGP2 POWER ON
		  pmic_config_interface(0x050C,1,0x1,15);
#else
		  hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
#endif
    MDELAY(100);

    SET_RESET_PIN(1);  //NOTE:should reset LCM firstly
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(120);	
    */
    /*	
    data_array[0] = 0x00110500;		// Sleep Out
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(120);
    */
    //*************Enable CMD2 Page1  *******************//
   /* data_array[0]=0x00043902;
    data_array[1]=0x1698FFFF;
    dsi_set_cmdq(data_array, 2, 1);
    MDELAY(20); 
    
    data_array[0] = 0x00033700;// read id return two byte,version and id
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(20); 
    
    read_reg_v2(0xD3, buffer, 3);
    id = buffer[1]; //we only need ID
    id2= buffer[1] << 8 | buffer[2]; //we test buffer 1
    
 
    Lcd_Log("%s, id=0x%x,id2=0x%x\n", __func__, id,id2);
 
 */
   // return (LCM_ID == id2)?1:0;

	    return (get_lcd_id()==0)?1:0;


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
	//MDELAY(1);
	read_reg_v2(0x0A, buffer, 1);
	 
	printk("mycat$$ huarui ###*** esd check buffer[0] = 0x%x\r\n",buffer[0]);
	
	if(buffer[0] == 0x9C)//9C
		return 0;
	else
		return 1;
 			
#endif
}

static unsigned int lcm_esd_recover(void)
{
  lcm_init();
	
#ifndef BUILD_LK	
	printk("mycat huarui ### lcm_esd_recover \r\n");
#endif
	
	return 1;
}
#endif



// ---------------------------------------------------------------------------
LCM_DRIVER ili9806_azet_A45119N50HI_dsi_2_HSD_fwvga_vdo_lcm_drv = 
{
    .name			= "ili9806_azet_A45119N50HI_dsi_2_HSD_fwvga_vdo",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,

    .compare_id    = lcm_compare_id,
#if defined (BIRD_ESD_CHECK)//rm esd resolve LCD  flick
	.esd_check = lcm_esd_check,
  .esd_recover = lcm_esd_recover,
#endif

};

