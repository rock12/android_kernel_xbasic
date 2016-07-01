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
#define FRAME_HEIGHT                                       (800) 
 
#define REGFLAG_DELAY                                       0XFD
#define REGFLAG_END_OF_TABLE                                0xFE   // END OF REGISTERS MARKER
 
#define LCM_DSI_CMD_MODE                                   	0
#define LCM_ID                                      				0x8009
#define GPIO_LCD_ID_PIN 																		GPIO_LCM_ID_PIN 
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

static struct LCM_setting_table lcm_initialization_setting1[] = {
#if 0
	{0x00,1,{0x80}},
	{0xff,3,{0x80,0x09,0x01}}, // enable EXTC
	{0x00,1,{0x80}},  		  // shift address
	{0xff,2,{0x80,0x09}}, 	  // enable Orise mode
	
	{0x00,1,{0xB4}},  		  // Column Inversion
	{0xc0,1,{0x10}},//10
	
	{0x00,1,{0x92}},           // pump45 01->X6X6 00->X8X6
	{0xc5,1,{0x01}},
	
	{0x00,1,{0x90}},           // pump1 X2 ratio
	{0xc5,1,{0xD6}},//2X
	
	{0x00,1,{0x95}},           //  pump34 CLK Line Rate 
	{0xc5,1,{0x34}},
	
	{0x00,1,{0x94}},           //  pump12 clk freq(Normal)
	{0xc5,1,{0x33}},
	
	{0x00,1,{0x82}},           //  REG-Pump23 AVEE VCL
	{0xc5,1,{0x83}},
	
	{0x00,1,{0xB1}},           // VDD_18V=1.6V GVDD test on
	{0xc5,1,{0xA8}},
	
	{0x00,1,{0x91}},
	{0xc5,1,{0x16}},	//26			//VGL=-13V, VGH=13V
	
	{0x00,1,{0x00}},
	{0xD8,1,{0x6F}}, // GVDD=4.5V//6fA
	
	{0x00,1,{0x01}},
	{0xD8,1,{0x6F}}, // NGVDD=-4.5V//6f
	
	{0x00,1,{0x81}},           // Frame rate = 66Hz
	{0xc1,1,{0x66}},
	
	{0x00,1,{0xA0}},          
	{0xc1,1,{0xEA}},
	
	{0x00,1,{0x00}},           // VCOM
	{0xD9,1,{0x52}}, //56          // -1.0875V
	//VRGH Disable
	{0x00,1,{0xB2}},
	{0xF5,1,{0x15}},
	//VRGH Disable
	{0x00,1,{0xB3}},
	{0xF5,1,{0x00}},
	//VRGH_S Disable
	{0x00,1,{0xB4}},
	{0xF5,1,{0x15}},
	//VRGH_S Disable
	{0x00,1,{0xB5}},
	{0xF5,1,{0x00}},
	//VRGH minimum
	{0x00,1,{0x93}},
	{0xC5,1,{0x03}},
	//調整GP
	{0x00,1,{0x96}},
	{0xC5,1,{0x23}},
	//調整SAP
	{0x00,1,{0x81}},
	{0xC4,1,{0x81}},
	//開啟Pre-Charge
	{0x00,1,{0x87}},
	{0xC4,1,{0x00}},
	//開啟Pre-Charge
	{0x00,1,{0x89}},
	{0xC4,1,{0x00}},
	//Disable 溫度SENSOR
	{0x00,1,{0xC0}},
	{0xC5,1,{0x00}},
	//ESD 機制
	{0x00,1,{0x8B}},
	{0xB0,1,{0x40}},
	
	{0x00,1,{0xC6}},
	{0xB0,1,{0x03}},
	
	//--------------------------------------------------------------------------------
	//	Initial Setting 1
	//--------------------------------------------------------------------------------
	{0x00,1,{0xa6}},           // turn off zigzag
	{0xb3,2,{0x20,0x01}},
	//C09x : mck_shift1/mck_shift2/mck_shift3
	{0x00,1,{0x90}},
	{0xc0,6,{0x00,0x44,0x00,0x00,0x00,0x03}},
	//C0Ax : hs_shift/vs_shift
	{0x00,1,{0xa6}},
	{0xc1,3,{0x00,0x00,0x00}},
	//--------------------------------------------------------------------------------
	//	Initial Setting 2 < tcon_goa_wave >-->GOA Timing Setting
													
	//CE8x : vst1, vst2, vst3, vst4
	{0x00,1,{0x80}},  // shift address
	{0xce,12,{0x86,0x01,0x00,0x85,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	//CEAx : clka1, clka2
	{0x00,1,{0xa0}},  // shift address
	{0xCE,14,{0x18,0x05,0x83,0x39,0x00,0x00,0x00,0x18,0x04,0x83,0x3a,0x00,0x00,0x00}},
	//CEBx : clka3, clka4
	{0x00,1,{0xb0}},  // shift address
	{0xCE,14,{0x18,0x03,0x83,0x3b,0x86,0x00,0x00,0x18,0x02,0x83,0x3c,0x88,0x00,0x00}},
	
	//CFCx : 
	{0x00,1,{0xc0}},  // shift address
	{0xCF,10,{0x01,0x01,0x20,0x20,0x00,0x00,0x01,0x01,0x00,0x00}},     //20130620
	
	//CFDx : 
	{0x00,1,{0xd0}},  // shift address
	{0xCF,1,{0x00}},//--------------------------------------------------------------------------------
	
	//--------------------------------------------------------------------------------
	//	Initial Setting 3 < Panel setting >-->Output PAD Assign Setting
	//--------------------------------------------------------------------------------
	//CBCx
	{0x00,1,{0xc0}},  // shift address
	{0xCB,15,{0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	//CBDx
	{0x00,1,{0xD0}},  // shift address
	{0xCB,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00}},
	//CBEx
	{0x00,1,{0xE0}},  // shift address
	{0xCB,10,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	//CC8x
	{0x00,1,{0x80}},  // shift address
	{0xCC,10,{0x00,0x26,0x09,0x0b,0x01,0x25,0x00,0x00,0x00,0x00}},
	//CC9x
	{0x00,1,{0x90}},  // shift address
	{0xCC,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0x0a,0x0c,0x02}},
	//CCAx
	{0x00,1,{0xA0}},  // shift address
	{0xCC,15,{0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	//CCBx
	{0x00,1,{0xB0}},  // shift address
	{0xCC,10,{0x00,0x25,0x0c,0x0a,0x02,0x26,0x00,0x00,0x00,0x00}},
	//CCCx
	{0x00,1,{0xC0}},  // shift address
	{0xCC,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x25,0x0b,0x09,0x01}},
	//CCDx
	{0x00,1,{0xD0}},  // shift address
	{0xCC,15,{0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	//GAMMA 2.6+  
	//	{0x00,1,{0x00}},
	//	{0xE1,16,{0x00,0x09,0x0f,0x0D,0x06,0x0E,0x0B,0x0A,0x03,0x07,0x0D,0x08,0x0F,0x0d,0x06,0x03}},
	//GAMMA 2.6-                       255  251  247  239  231  203  175  147  108  80  52    24   16   8    4     0
	//	{0x00,1,{0x00}},
	//	{0xE2,16,{0x00,0x09,0x0f,0x0E,0x07,0x0e,0x0B,0x0A,0x03,0x07,0x0E,0x08,0x0F,0x0d,0x07,0x03}},
	
	//GAMMA 2.2+  
	
	{0x00,1,{0x00}},
	{0xE1,16,{0x00,0x08,0x0d,0x0D,0x06,0x0E,0x0B,0x0A,0x03,0x07,0x0D,0x08,0x0F,0x14,0x0D,0x03}},
	//GAMMA 2.2-                       255  251  247  239  231  203  175  147  108  80  52    24   16   8    4     0
	{0x00,1,{0x00}},
	{0xE2,16,{0x00,0x08,0x0e,0x0E,0x07,0x0e,0x0B,0x0A,0x03,0x07,0x0E,0x08,0x0F,0x14,0x0D,0x03}},
	
	{0x00,1,{0x00}},
	{0xff,3,{0xFF,0xFF,0xFF}}, // enable EXTC
	
	{0x00,1,{0x00}},
	{0x11,1,{0x00}},
	{REGFLAG_DELAY, 150, {}},
	
	{0x00,1,{0x00}},
	{0x29,1,{0x00}},
	{REGFLAG_DELAY, 10, {}},
#else
	{0x00,1,{0x00}},
	{0xff,3,{0x80,0x09,0x01}},
	{0x00,1,{0x80}},
	{0xff,2,{0x80,0x09}},
	{0x00,1,{0xb4}},
	{0xc0,1,{0x10}},
	{0x00,1,{0x89}},
	{0xc4,1,{0x08}},
	{0x00,1,{0xa3}},
	{0xc0,1,{0x00}},
	{0x00,1,{0x82}},
	{0xc5,1,{0xa3}},
	{0x00,1,{0x90}},
	{0xc5,2,{0xd6,0x87}},
	{0x00,1,{0x00}},
	{0xd8,2,{0x74,0x72}},
	{0x00,1,{0x00}},
	{0xd9,1,{0x6B}},
	{0x00,1,{0x81}},
	{0xc1,1,{0x77}},
	{0x00,1,{0xa1}},
	{0xc1,1,{0x08}},
	{0x00,1,{0x81}},
	{0xc4,1,{0x83}},
	{0x00,1,{0x92}},
	{0xc5,1,{0x01}},
	{0x00,1,{0xb1}},
	{0xc5,1,{0xa9}},
	{0x00,1,{0x80}},
	{0xc4,1,{0x30}},
	{0x00,1,{0x80}},
	{0xce,12,{0x85,0x03,0x00,0x84,0x03,0x00,0x83,0x03,0x00,0x82,0x03,0x00}},
	{0x00,1,{0xa0}},
	{0xce,14,{0x38,0x02,0x03,0x21,0x00,0x00,0x00,0x38,0x01,0x03,0x22,0x00,0x00,0x00}},
	{0x00,1,{0xb0}},
	{0xce,14,{0x38,0x00,0x03,0x23,0x00,0x00,0x00,0x30,0x00,0x03,0x24,0x00,0x00,0x00}},
	{0x00,1,{0xc0}},
	{0xce,14,{0x30,0x01,0x03,0x25,0x00,0x00,0x00,0x30,0x02,0x03,0x26,0x00,0x00,0x00}},
	{0x00,1,{0xd0}},
	{0xce,14,{0x30,0x03,0x03,0x27,0x00,0x00,0x00,0x30,0x04,0x03,0x28,0x00,0x00,0x00}},
	{0x00,1,{0xc0}},
	{0xcf,10,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	{0x00,1,{0xd0}},
	{0xcf,1,{0x00}},
	{0x00,1,{0xc0}},
	{0xcb,15,{0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00}},
	{0x00,1,{0xd0}},
	{0xcb,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04}},
	{0x00,1,{0xe0}},
	{0xcb,10,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	{0x00,1,{0x80}},
	{0xcc,10,{0x00,0x00,0x00,0x00,0x0c,0x0a,0x10,0x0e,0x03,0x04}},
	{0x00,1,{0x90}},
	{0xcc,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0b}},
	{0x00,1,{0xa0}},
	{0xcc,15,{0x09,0x0f,0x0d,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	{0x00,1,{0xb0}},
	{0xcc,10,{0x00,0x00,0x00,0x00,0x0d,0x0f,0x09,0x0b,0x02,0x01}},
	{0x00,1,{0xc0}},
	{0xcc,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e}},
	{0x00,1,{0xd0}},
	{0xcc,15,{0x10,0x0a,0x0c,0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	{0x00,1,{0x00}},
	{0xe1,16,{0x09,0x0a,0x0e,0x0d,0x07,0x18,0x0d,0x0d,0x01,0x04,0x05,0x06,0x0e,0x25,0x22,0x05}},
	{0x00,1,{0x00}},
	{0xe2,16,{0x09,0x0a,0x0e,0x0d,0x07,0x18,0x0d,0x0d,0x01,0x04,0x05,0x06,0x0e,0x25,0x22,0x05}},
	
	{0x00,1,{0xA0}},
	{0xC1,1,{0xEA}},
	
	{0x00,1,{0xA6}},
	{0xC1,3,{0x01,0x00,0x00}},
	
	{0x00,1,{0xC6}},
	{0xB0,1,{0x03}},
	
	{0x00,1,{0x00}},
	{0xFF,3,{0xFF,0xFF,0xFF}},
	
	{0x00,1,{0x00}},
	{0x11,1,{0x00}},
	{REGFLAG_DELAY, 120, {}},
	
	{0x00,1,{0x00}},
	{0x29,1,{0x00}},
	{REGFLAG_DELAY, 10, {}},
#endif

{0x2C,1,{0x00}}, 
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
     params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
#else
		params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif
 
     // DSI
     /* Command mode setting */
     params->dsi.LANE_NUM                = LCM_TWO_LANE;
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
      
      params->dsi.vertical_sync_active 		= 4;//--4
      params->dsi.vertical_backporch 			= 20;//--8
      params->dsi.vertical_frontporch 		= 16;//--8
      params->dsi.vertical_active_line 		= FRAME_HEIGHT; 
  		/*
      params->dsi.horizontal_sync_active = 6;//---6
      params->dsi.horizontal_backporch =70;//--30
      params->dsi.horizontal_frontporch =70;//--30
  		*/  
    	// 20120618 modify. for ID 01,8b,80,09
      params->dsi.horizontal_sync_active 	= 8;//---6
      params->dsi.horizontal_backporch 		=50;//--37
      params->dsi.horizontal_frontporch 	=60;//--37
  
      /*  // 20120618 modify. for ID 02,8b,80,09
      params->dsi.horizontal_sync_active = 6;//---6
      params->dsi.horizontal_backporch =23;//--30
      params->dsi.horizontal_frontporch =24;//--30
       */
      params->dsi.horizontal_active_pixel = FRAME_WIDTH;
 
     	// Bit rate calculation
	    params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4
	    params->dsi.pll_div2=0;		// div2=0,1,2,3;div2_real=1,2,4,4    0
	    params->dsi.fbk_div =14;//15,16;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)   18
	 
      /* ESD or noise interference recovery For video mode LCM only. */ // Send TE packet to LCM in a period of n frames and check the response.
			params->dsi.lcm_int_te_monitor = 0;// FALSE;
			params->dsi.lcm_int_te_period = 1; // Unit : frames
			
			// Need longer FP for more opportunity to do int. TE monitor applicably.
			if(params->dsi.lcm_int_te_monitor)
			params->dsi.vertical_frontporch *= 2;
			
			// Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.)
			#if 0//def BIRD_ESD_CHECK
			params->dsi.lcm_ext_te_monitor = 1;// FALSE;
			#else
			params->dsi.lcm_ext_te_monitor = 0;
			#endif
			// Non-continuous clock
			params->dsi.noncont_clock = 1;// TRUE;
			params->dsi.noncont_clock_period = 2; // Unit : frames
}
 
 
static unsigned int lcm_compare_id(void)
{
	unsigned int id=0,id2=0;
	unsigned char buffer[5],buffer2[2];
	unsigned int array[16];  
	
#ifdef BUILD_LK
	upmu_set_rg_vgp2_vosel(5);
	upmu_set_rg_vgp2_en(1);

	upmu_set_rg_vgp3_vosel(3);
	upmu_set_rg_vgp3_en(1);	
#else
  hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
	hwPowerOn(MT6323_POWER_LDO_VGP3,VOL_1800,"LCM");
#endif
    MDELAY(100);

	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(25);
	SET_RESET_PIN(1);
	MDELAY(50);

	array[0]=0x00043902;
	array[1]=0x010980ff; 
	dsi_set_cmdq(array, 2, 1); //{0xff, 3 ,{0x80,0x09,0x01}}, // Command2 Enable
	
	array[0]=0x80001500;
	dsi_set_cmdq(array, 1, 1); //{0x00, 1 ,{0x80}},
	
	array[0]=0x00033902;
	array[1]=0x000980ff;
	dsi_set_cmdq(array, 2, 1); //{0xff, 2 ,{0x80,0x09}}, // Orise Mode Enable
	
	array[0]=0xC6001500;
	dsi_set_cmdq(array, 1, 1); //{0x00, 1 ,{0xC6}},
	
	array[0]=0x03B01500;
	dsi_set_cmdq(array, 1, 1); //{0xB0, 1 ,{0x03}}, // Read Contention Error
	MDELAY(10);

	array[0] = 0x00053700;// set return byte number
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xA1, &buffer, 5); // Read Register 0xA1 : 0x01,0x8B,0x80,0x09         (OTM8009A/OTM8018B);

	id = buffer[2]<<8 |buffer[3]; 

	Lcd_Log("[LSQ] -- otm8018b 0x%x , 0x%x , 0x%x \n",buffer[2],buffer[3],id);

	array[0] = 0x50001500;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xF8, &buffer2, 2); // Read Register 0xF850 : 0x02,0x35 (OTM8018B); 
	
	id2 = buffer2[0]<<8 |buffer2[1]; 
	Lcd_Log("[LSQ] -- otm8018b 0x%x , 0x%x , 0x%x \n",buffer2[0],buffer2[1],id2);
	
	return ((id2==0x0235)&&(id==0x8009))?1:0;
}
 
static void lcm_init(void)
{
	  int flag=0;
#ifdef BUILD_LK
	upmu_set_rg_vgp2_vosel(5);
	upmu_set_rg_vgp2_en(1);

	upmu_set_rg_vgp3_vosel(3);
	upmu_set_rg_vgp3_en(1);	
#else
  hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
	hwPowerOn(MT6323_POWER_LDO_VGP3,VOL_1800,"LCM");
#endif
	  MDELAY(100);

    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(1);
    SET_RESET_PIN(1);
    MDELAY(10);

 		Lcd_Log("mycat otm8018b flag=%d  \n",flag);
 		push_table(lcm_initialization_setting1, sizeof(lcm_initialization_setting1) / sizeof(struct LCM_setting_table), 1);
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
	 
	printk("mycat$$ yashu ###*** esd check buffer[0] = 0x%x\r\n",buffer[0]);
	
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
	printk("mycat yashu ### lcm_esd_recover \r\n");
#endif
	
	return 1;
}
#endif


LCM_DRIVER otm8018b_haoshitong_HW397148QRD_HSD_dsi_2_wvga_lcm_drv = 
{
		.name			= "otm8018b_haoshitong_HW397148QRD_HSD_dsi_2_wvga",
		.set_util_funcs = lcm_set_util_funcs,
		.compare_id    = lcm_compare_id,
		.get_params     = lcm_get_params,
		.init           = lcm_init,
		.suspend        = lcm_suspend,
		.resume         = lcm_resume,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
#if defined (BIRD_ESD_CHECK)
		.esd_check = lcm_esd_check,
	  .esd_recover = lcm_esd_recover,
#endif
    };
