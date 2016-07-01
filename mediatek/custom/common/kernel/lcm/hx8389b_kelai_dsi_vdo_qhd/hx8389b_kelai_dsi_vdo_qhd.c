
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

#define FRAME_WIDTH  										(540)
#define FRAME_HEIGHT 										(960)

#define REGFLAG_DELAY             							0XEE
#define REGFLAG_END_OF_TABLE      							0xEF   // END OF REGISTERS MARKER

#define LCM_ID	0x48655

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
//static unsigned int vcom_level = 0x45;

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
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

 struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[164];
};





static struct LCM_setting_table lcm_initialization_setting[] = {
//mx1031b_sb_s6 //suxian 
/********************
{0xB9,3,{0xFF,0x83,0x69}},
{0x3A,1,{0x70}},  
{0xD5,92,{0x00,0x00,0x13,0x03,0x35,0x00,0x01,0x10,0x01,0x00,0x00,0x00,0x01,0x7a,0x16,0x04,0x04,0x13,0x07,0x40,0x13,0x00,0x00,0x00,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x48,0x88,0x85,0x42,0x00,0x88,0x88,0x00,0x00,0x18,0x88,0x86,0x71,0x35,0x88,0x88,0x00,0x00,0x58,0x88,0x87,0x63,0x11,0x88,0x88,0x00,0x00,0x08,0x88,0x84,0x50,0x24,0x88,0x88,0x00,0x00,0x00,0x51,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x0F,0x00,0x00,0x0F,0x00,0x0F,0x00,0x01,0x5A}},
{0xB1,10,{0x09,0x83,0x17,0x00,0x98,0x12,0x16,0x16,0x0C,0x0a}},
{0xB3,7,{0x83,0x00,0x31,0x03,0x01,0x0b,0x08}}, 
{0xB4,1,{0x01}},
{0xB6,2,{0x80,0x80}},
{0xCC,1,{0x02}},
{0xC1,127,{0x03,0x00,0x09,0x11,0x1A,0x21,0x29,0x31,0x38,0x40,0x48,0x50,0x58,0x61,0x69,0x70,0x78,0x80,0x88,0x90,0x9A,0xA1,0xaa,0xB3,0xBA,0xC2,0xCB,0xD2,0xDA,0xE3,0xE9,0xF1,0xF8,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x11,0x1A,0x21,0x29,0x31,0x38,0x40,0x48,0x50,0x58,0x61,0x69,0x70,0x78,0x80,0x88,0x90,0x9A,0xA1,0xaa,0xB3,0xBA,0xC2,0xCB,0xD2,0xDA,0xE3,0xE9,0xF1,0xF8,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x11,0x1A,0x21,0x29,0x31,0x38,0x40,0x48,0x50,0x58,0x61,0x69,0x70,0x78,0x80,0x88,0x90,0x9A,0xA1,0xaa,0xB3,0xBA,0xC2,0xCB,0xD2,0xDA,0xE3,0xE9,0xF1,0xF8,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xC6,4,{0x41,0xFF,0x7D,0xFF}},
{0xB3,1,{0x01}},
{0xEA,1,{0x72}},
{0xE3,4,{0x07,0x0F,0x07,0x0F}},
{0xC0,6,{0x73,0x50,0x00,0x3c,0xC4,0x08}},
{0xE0,35,{0x00,0x05,0x0B,0x2F,0x2F,0x30,0x1B,0x3D,0x07,0x0D,0x0E,0x12,0x13,0x12,0x13,0x11,0x1A,0x00,0x05,0x0B,0x2F,0x2F,0x30,0x1B,0x3D,0x07,0x0D,0x0E,0x12,0x13,0x12,0x13,0x11,0x1A,0x01}},  
{0xBA,15,{0x31,0x00,0x16,0xCA,0xB0,0x0A,0x00,0x10,0x28,0x02,0x21,0x21,0x9A,0x1A,0x8F}},
{0x11,0,{}},
{REGFLAG_DELAY, 150, {}},
{0x29,0,{}},
{REGFLAG_DELAY, 50, {}},
{0x2C,0,{}},
*******************************/
{0xB9,3, {0xFF,0x83,0x89}},//20130904
  
  {0xBA,7, {0x01,0x92,0x00,0x16,0xC4,0x00,0x18}}, 
  
  {0xDE,3, {0x05,0x58,0x10}}, 
  
  {0xB1,19, {0x00,0x00,0x07,0xE8,0x52,0x10,0x10,0x90,0xF0,0x02,
     0x0A,0x10,0x10,0x42,0x00,0x5A,0xFb,0x00,0x08}},
     
  {0xB2,7, {0x00,0x00,0x78,0x04,0x07,0x3F,0x80}}, 
  
  {0xB4,23, {0x00,0x0E,0x00,0x32,0x10,0x02,0x32,0x13,0xC6,0x32,
     0x10,0x00,0x17,0x01,0x48,0x06,0x17,0x01,0x48,0x14,
     0x50,0x5A,0x0C}},     
        {0xD5,48, {0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0xE0,0x18,
     0x88,0x88,0x01,0x23,0x08,0x48,0x81,0x85,0x28,0x68,
     0x83,0x87,0x87,0x85,0x68,0x48,0x88,0x88,0x88,0x88,
     0x88,0x76,0x54,0x78,0x38,0x86,0x82,0x58,0x18,0x84,
     0x80,0x80,0x82,0x18,0x38,0x88,0x88,0x88}},
     
  {0xE0,34, {0x00,0x0A,0x0F,0x21,0x33,0x3D,0x1F,0x3C,0x07,0x0D,
     0x0F,0x12,0x14,0x11,0x14,0x12,0x1E,0x00,0x0A,0x0F,
     0x21,0x33,0x3B,0x1F,0x3C,0x07,0x0D,0x0F,0x12,0x14,
     0x11,0x14,0x12,0x1E}},     
     {0xCC,1, {0x02}},
     {0xB6,4, {0x00,0x70,0x00,0x70}}, 
      
     {0xCB,2, {0x07,0x07}},
     
       {0x11,0, {0x00}},
  
  {REGFLAG_DELAY,120,{}},
  
  {0x29,0, {0x00}},
  {REGFLAG_DELAY,10,{}},
	// Noteotm9605a_qhd_dsi_vdo_hsd_lianxin_fpccxd50035a
	// Strongly recommend not to set Sleep out / Display On here. That will cause messed frame to be shown as later the backlight is on.

	// Setting ending by predefined flag
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};




static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 1, {0x00}},
    {REGFLAG_DELAY, 20, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 20, {}},

    // Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},


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



static void lcm_init_setting()
{

	
		unsigned int data_array[33];
		//SET_RESET_PIN(1);
   // SET_RESET_PIN(0);
   // MDELAY(1);
    //SET_RESET_PIN(1);
  //  MDELAY(10);

		data_array[0]= 0x00043902;
		data_array[1]= 0x8983FFB9;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);

		data_array[0]= 0x00083902;
		data_array[1]= 0x009201BA;
		data_array[2]= 0x1800C416;
		dsi_set_cmdq(&data_array, 3, 1);
		MDELAY(1);
		
		
		//data_array[0]= 0x00083902;
		//data_array[1]= 0x008341BA;
		//data_array[2]= 0x1800A416;
		//dsi_set_cmdq(&data_array, 3, 1);
		//MDELAY(1);

		data_array[0]= 0x00043902;
		data_array[1]= 0x105805DE;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);

//		data_array[0]= 0x00023902;
//		data_array[1]= 0x000008C6;
//		dsi_set_cmdq(&data_array, 2, 1);
//		MDELAY(1);

		data_array[0]= 0x00143902;
		data_array[1]= 0x070000B1;
		data_array[2]= 0x10105cE8;
		data_array[3]= 0x0A02F0e0;
		data_array[4]= 0x03421010;
		data_array[5]= 0x0800FB5A;
		dsi_set_cmdq(&data_array, 6, 1);
		MDELAY(1);

		data_array[0]= 0x00043902;
		data_array[1]= 0x520000B7;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);

		data_array[0]= 0x00083902;
		data_array[1]= 0x780000B2;
		data_array[2]= 0x803F0704;
		dsi_set_cmdq(&data_array, 3, 1);
		MDELAY(1);

		data_array[0]= 0x00183902;
		data_array[1]= 0x000E00B4;
		data_array[2]= 0x32021032;
		data_array[3]= 0x1032C613;
		data_array[4]= 0x48011700;
		data_array[5]= 0x48011706;
		data_array[6]= 0x0C5A5014;
		dsi_set_cmdq(&data_array, 7, 1);
		MDELAY(1);

		data_array[0]= 0x00313902;
		data_array[1]= 0x000000D5;
		data_array[2]= 0x00000100;
		data_array[3]= 0x8818E001;
		data_array[4]= 0x08230188;
		data_array[5]= 0x28858148;
		data_array[6]= 0x87878368;
		data_array[7]= 0x88486885;
		data_array[8]= 0x88888888;
		data_array[9]= 0x38785476;
		data_array[10]= 0x18588286,
		data_array[11]= 0x82808084;
		data_array[12]= 0x88883818;
		data_array[13]= 0x00000088;
		dsi_set_cmdq(&data_array, 14, 1);
		MDELAY(1);

		data_array[0]= 0x00233902;
		data_array[1]= 0x0F0A00E0;
		data_array[2]= 0x1F3D3321;
		data_array[3]= 0x0F0D073C;
		data_array[4]= 0x14111412;
		data_array[5]= 0x0A001E12;
		data_array[6]= 0x3B33210F;
		data_array[7]= 0x0D073C1F;
		data_array[8]= 0x1114120F;
		data_array[9]= 0x001E1214;
		dsi_set_cmdq(&data_array, 10, 1);
		MDELAY(1);

//		data_array[0]=	0x00213902;
//		data_array[1]=	0x070001C1;
//		data_array[2]=	0x3F342A1C;
//		data_array[3]=	0x635B5249;
//		data_array[4]=	0x7F78716A;
//		data_array[5]=	0x97918B85;
//		data_array[6]=	0xAFA8A39D;
//		data_array[7]=	0xC9C6BEB5;
//		data_array[8]=	0xE6E1D7CD;
//                data_array[9]=	0x000000F1;
//                dsi_set_cmdq(&data_array, 10, 1);
//		MDELAY(1);

//                data_array[0]=	0x00212902;
//		data_array[1]=	0x34FFF7C1;
//		data_array[2]=	0x54D94FCB;
//		data_array[3]=	0xC04A3CC8;
//		data_array[4]=	0x2A1C0700;
//		data_array[5]=	0x52493F34;
//		data_array[6]=	0x716A635B;
//		data_array[7]=	0x8B857F78;
//		data_array[8]=	0xA39D9791;
//		data_array[9]=	0x000000A8;
//                dsi_set_cmdq(&data_array, 10, 1);
//		MDELAY(1);

//                data_array[0]=	0x00212902; 
//		data_array[1]=	0xBEB5AFC1;
//		data_array[2]=	0xD7CDC9C6;
//		data_array[3]=	0xF7F1E6E1;
//		data_array[4]=	0x4FCB34FF;
//		data_array[5]=	0x3CC854D9;
//		data_array[6]=	0x0700C04A;
//		data_array[7]=	0x3F342A1C;
//		data_array[8]=	0x635B5249;
//    data_array[9]=	0x0000006A;
//   dsi_set_cmdq(&data_array, 10, 1);
//		MDELAY(1);

//    data_array[0]=	0x00202902; 
//		data_array[1]=	0x7F7871C1;
//		data_array[2]=	0x97918B85;
//		data_array[3]=	0xAFA8A39D;
//		data_array[4]=	0xC9C6BEB5;
//		data_array[5]=	0xE6E1D7CD;
//		data_array[6]=	0x34FFF7F1;
//		data_array[7]=	0x54D94FCB;
//		data_array[8]=	0xC04A3CC8;
//		dsi_set_cmdq(&data_array, 9, 1);
//		MDELAY(1);
		
//		data_array[0]= 0x00023902;
//		data_array[1]= 0x000001E6;
//		dsi_set_cmdq(&data_array, 2, 1);
//		MDELAY(1);
//		
//		data_array[0]= 0x00023902;
//		data_array[1]= 0x000004E4;
//		dsi_set_cmdq(&data_array, 2, 1);
//		MDELAY(1);		
		
		data_array[0]= 0x00023902;
		data_array[1]= 0x000002CC;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);

//		data_array[0]= 0x00023902;
//		data_array[1]= 0x0000773A;
//		dsi_set_cmdq(&data_array, 2, 1);
//		MDELAY(1);

		data_array[0]= 0x00023902;
		data_array[1]= 0x00000035;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);

		data_array[0]= 0x00053902;
		data_array[1]= 0x006c00B6;
	  data_array[2]= 0x0000006c;
		
	
		dsi_set_cmdq(&data_array, 3, 1);
		MDELAY(1);
		
		data_array[0]= 0x00033902;
		data_array[1]= 0x000707CB;
		dsi_set_cmdq(&data_array, 2, 1);
		MDELAY(1);
	
		data_array[0] = 0x00110500; 	   //exit sleep mode 
		dsi_set_cmdq(&data_array, 1, 1); 
		MDELAY(150);
		
		data_array[0] = 0x00290500; 	   //exit sleep mode 
		dsi_set_cmdq(&data_array, 1, 1); 
		MDELAY(20);
}

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
		params->dbi.te_mode 				= LCM_DBI_TE_MODE_DISABLED;  //LCM_DBI_TE_MODE_VSYNC_ONLY;
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

		params->dsi.mode   = SYNC_PULSE_VDO_MODE;

	
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

		params->dsi.vertical_sync_active				= 2;
		params->dsi.vertical_backporch					= 6;
		params->dsi.vertical_frontporch					= 9;
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 40;
		params->dsi.horizontal_backporch				= 40;
		params->dsi.horizontal_frontporch				= 70;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

		// Bit rate calculation
		params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4
		params->dsi.pll_div2=1;		// div2=0,1,2,3;div2_real=1,2,4,4
		params->dsi.fbk_div =34;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)		

		/* ESD or noise interference recovery For video mode LCM only.  // Send TE packet to LCM in a period of n frames and check the response. 
		params->dsi.lcm_int_te_monitor = FALSE; 
		params->dsi.lcm_int_te_period = 1; // Unit : frames 

		// Need longer FP for more opportunity to do int. TE monitor applicably. 
		if(params->dsi.lcm_int_te_monitor) 
		params->dsi.vertical_frontporch *= 2; 

		// Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.) 
		params->dsi.lcm_ext_te_monitor = FALSE; 
		// Non-continuous clock 
		params->dsi.noncont_clock = TRUE; 
		params->dsi.noncont_clock_period = 2; // Unit : frames*/
}


static void lcm_init(void)
{

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(30);
	SET_RESET_PIN(1);
	MDELAY(120);

	lcm_init_setting();
	//push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);



}


static void lcm_suspend(void)
{
		
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(30);
	SET_RESET_PIN(1);
	MDELAY(120);

	//unsigned int data_array[10];
	//	data_array[0] = 0x00280500;
		//dsi_set_cmdq(&data_array, 1, 1); 
	//	MDELAY(150); 
	//	data_array[0] = 0x00100500;
	//	dsi_set_cmdq(&data_array, 1, 1); 
	//	MDELAY(150);
	//push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);

}


static void lcm_resume(void)
{
//	#if defined(BUILD_LK)

//	#else
	//printk("crystal_lcd_id = hx8389\n");	
	//#endif
	
	
		//lcm_init_setting();
	
	lcm_init();

}




static unsigned int lcm_compare_id(void)
{/*
	unsigned int lcd_id = 0;

	lcd_id = ((mt_get_gpio_in(GPIO_LCD_ID1_PIN) << 1) & 0x2) | mt_get_gpio_in(GPIO_LCD_ID2_PIN);

#if defined(BUILD_LK)
	printf("crystal_lcd_id =%x\n", lcd_id);	
	printf("crystal_lcd_id_pin1 =%x\n", mt_get_gpio_in(GPIO_LCD_ID1_PIN));	
	printf("crystal_lcd_id_pin2 =%x\n", mt_get_gpio_in(GPIO_LCD_ID2_PIN));
#else
	printk("crystal_lcd_id =%x\n", lcd_id);	
	printk("crystal_lcd_id_pin1 =%x\n", mt_get_gpio_in(GPIO_LCD_ID1_PIN));	
	printk("crystal_lcd_id_pin2 =%x\n", mt_get_gpio_in(GPIO_LCD_ID2_PIN));	
#endif 
	return (lcd_id  == 1)?1:0;*/
	int array[4];
	char buffer[5];
	char id_high=0;
	char id_low=0;
	int id=0;

	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(200);

	array[0] = 0x00053700;
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0xa1, buffer, 5);

	id_high = buffer[2];
	id_low = buffer[3];
	id = (id_high<<8) | id_low;

	Lcd_Log("OTM9605A id=%d \n",id);


	return (LCM_ID == id)?1:0;
}

LCM_DRIVER hx8389b_kelai_dsi_vdo_qhd_lcm_drv = 
{
    	.name		= "hx8389b_kelai_dsi_vdo_qhd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id    = lcm_compare_id,
};


