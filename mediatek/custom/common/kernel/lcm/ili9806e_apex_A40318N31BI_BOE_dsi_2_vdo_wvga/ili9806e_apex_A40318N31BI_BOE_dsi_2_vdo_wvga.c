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
#define FRAME_HEIGHT 										(800)

#define REGFLAG_DELAY             							0XFFE
#define REGFLAG_END_OF_TABLE      						0xFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE									0

#define LCM_ID                                      0x980604
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
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)                      lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)       

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[120];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
{0xFF,5,{0xFF, 0x98, 0x06, 0x04, 0x01}},  
{0x08,1,{0x10}},
{0x21,1,{0x01}},
{0x30,1,{0x02}},
{0x31,1,{0x02}},

{0x60,1,{0x07}},
{0x61,1,{0x06}},
{0x62,1,{0x06}},
{0x63,1,{0x04}},

{0x40,1,{0x15}},
{0x41,1,{0x22}},
{0x42,1,{0x00}},
{0x43,1,{0x8B}},
{0x44,1,{0x8C}},
{0x45,1,{0x1B}},
{0x50,1,{0x78}},
{0x51,1,{0x78}},
{0x52,1,{0x00}},
{0x53,1,{0x47}},//67 changed 0423
//++++++++++++++++++ Gamma Setting ++++++++++++++++++//
{0xA0,1,{0x00}},
{0xA1,1,{0x14}},
{0xA2,1,{0x1A}},
{0xA3,1,{0x06}},
{0xA4,1,{0x02}},
{0xA5,1,{0x04}}, 
{0xA6,1,{0x1C}},
{0xA7,1,{0x00}},
{0xA8,1,{0x19}},
{0xA9,1,{0x0C}},
{0xAA,1,{0x0E}},
{0xAB,1,{0x04}}, 
{0xAC,1,{0x07}}, 
{0xAD,1,{0x1A}},
{0xAE,1,{0x02}}, 
{0xAF,1,{0x00}}, 
///==============Nagitive
{0xC0,1,{0x00}},
{0xC1,1,{0x01}},
{0xC2,1,{0x0A}},
{0xC3,1,{0x10}},
{0xC4,1,{0x07}},
{0xC5,1,{0x10}}, 
{0xC6,1,{0x01}},
{0xC7,1,{0x0F}}, 
{0xC8,1,{0x06}},
{0xC9,1,{0x0A}},
{0xCA,1,{0x12}}, 
{0xCB,1,{0x0B}}, 
{0xCC,1,{0x14}}, 
{0xCD,1,{0x17}}, 
{0xCE,1,{0x16}},
{0xCF,1,{0x00}}, 
//**************************************************************************//
//****************************** Page 7 Command ****************************//
//**************************************************************************//
{0xFF,5,{0xFF,0x98,0x06,0x04,0x07}}, 
{0x18,1,{0x1D}}, 
{0x17,1,{0x12}}, 
//****************************************************************************//
//****************************** Page 6 Command ******************************//
//****************************************************************************//
{0xFF,5,{0xFF,0x98,0x06,0x04,0x06}},  
{0x00,1,{0x20}},
{0x01,1,{0x05}},
{0x02,1,{0x00}},
{0x03,1,{0x00}},
{0x04,1,{0x01}},
{0x05,1,{0x01}}, 
{0x06,1,{0x88}}, 
{0x07,1,{0x04}},
{0x08,1,{0x01}},
{0x09,1,{0x90}},
{0x0A,1,{0x04}},
{0x0B,1,{0x01}},
{0x0C,1,{0x01}},
{0x0D,1,{0x01}}, 
{0x0E,1,{0x00}}, 
{0x0F,1,{0x00}}, 
{0x10,1,{0x55}}, 
{0x11,1,{0x50}}, 
{0x12,1,{0x01}}, 
{0x13,1,{0x0C}}, 
{0x14,1,{0x0D}}, 
{0x15,1,{0x43}}, 
{0x16,1,{0x0B}}, 
{0x17,1,{0x00}}, 
{0x18,1,{0x00}}, 
{0x19,1,{0x00}}, 
{0x1A,1,{0x00}}, 
{0x1B,1,{0x00}}, 
{0x1C,1,{0x00}}, 
{0x1D,1,{0x00}}, 
{0x20,1,{0x01}}, 
{0x21,1,{0x23}}, 
{0x22,1,{0x45}}, 
{0x23,1,{0x67}}, 
{0x24,1,{0x01}}, 
{0x25,1,{0x23}}, 
{0x26,1,{0x45}}, 
{0x27,1,{0x67}}, 
{0x30,1,{0x02}}, 
{0x31,1,{0x22}}, 
{0x32,1,{0x11}}, 
{0x33,1,{0xAA}}, 
{0x34,1,{0xBB}}, 
{0x35,1,{0x66}}, 
{0x36,1,{0x00}}, 
{0x37,1,{0x22}}, 
{0x38,1,{0x22}}, 
{0x39,1,{0x22}}, 
{0x3A,1,{0x22}}, 
{0x3B,1,{0x22}}, 
{0x3C,1,{0x22}}, 
{0x3D,1,{0x22}}, 
{0x3E,1,{0x22}}, 
{0x3F,1,{0x22}}, 
{0x40,1,{0x22}},
{0x53,1,{0x1A}},

{0xFF,5,{0xFF,0x98,0x06,0x04,0x00}}, 
{0x35,1,{0x00}},//TE

{0x11,1,{0x00}},
{REGFLAG_DELAY,120,{}},
{0x29,1,{0x00}},
{REGFLAG_DELAY,5,{}},
// Note
// Strongly recommend not to set Sleep out / Display On here. That will cause messed frame to be shown as later the backlight is on.
// Setting ending by predefined flag
{REGFLAG_END_OF_TABLE,0x00,{}}
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

		//enable tearing-free
		params->dbi.te_mode                 = LCM_DBI_TE_MODE_VSYNC_ONLY;
		//params->dbi.te_mode 			= LCM_DBI_TE_MODE_DISABLED; 
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

		params->dsi.vertical_sync_active				= 4;
		params->dsi.vertical_backporch					= 16;
		params->dsi.vertical_frontporch 				= 20;
		params->dsi.vertical_active_line				= FRAME_HEIGHT;
		
		params->dsi.horizontal_sync_active			= 10;
		params->dsi.horizontal_backporch				= 80; //60
		params->dsi.horizontal_frontporch				= 80; //200
		params->dsi.horizontal_blanking_pixel		= 60; //
		params->dsi.horizontal_active_pixel 		= FRAME_WIDTH;

  	// Bit rate calculation
  	params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4
  	params->dsi.pll_div2=1;		// div2=0,1,2,3;div2_real=1,2,4,4
  	params->dsi.fbk_div =28;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)		
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
    MDELAY(100);//Must > 120ms

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_resume(void)
{
	//lcm_initialization_setting[18].para_list[0] +=2;
	Lcd_Log("junchao %s, lcm_initialization_setting[18].para_list[0]=0x%2x\n", __func__, lcm_initialization_setting[18].para_list[0]);
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
#if defined(BUILD_LK)
		upmu_set_rg_vgp2_vosel(5);
		upmu_set_rg_vgp2_en(1);
		
		upmu_set_rg_vgp3_vosel(3);
		upmu_set_rg_vgp3_en(1);	
#else
		hwPowerOn(MT6323_POWER_LDO_VGP2, VOL_2800, "Lance_LCM");
		hwPowerOn(MT6323_POWER_LDO_VGP3, VOL_1800, "Lance_LCM");
#endif
        int array[4];
        char buffer[1];
        char id_high=0;
        char id_midd=0;
        char id_low=0;
        int id=0;

        //Do reset here
        SET_RESET_PIN(1);
        SET_RESET_PIN(0);
        MDELAY(25);       
        SET_RESET_PIN(1);
        MDELAY(50);      
       
        array[0]=0x00063902;
        array[1]=0x0698ffff;
        array[2]=0x00000104;
        dsi_set_cmdq(array, 3, 1);
        MDELAY(10);

        array[0]=0x00023700;
        dsi_set_cmdq(array, 1, 1);
        //read_reg_v2(0x04, buffer, 3);//if read 0x04,should get 0x008000,that is both OK.
    
        read_reg_v2(0x00, buffer,1);
        id_high = buffer[0]; ///////////////////////0x98

        read_reg_v2(0x01, buffer,1);
        id_midd = buffer[0]; ///////////////////////0x06

        read_reg_v2(0x02, buffer,1);
        id_low = buffer[0]; ////////////////////////0x04

        id = (id_high << 16) | (id_midd << 8) | id_low;
        
        Lcd_Log("wqcat %s, 0x00=0x%x,0x01=0x%x,0x02=0x%x,id=0x%x\n", __func__, id_high,id_midd,id_low,id);
        return ((LCM_ID == id)?1:0)&& ((get_lcd_id()==0)?1:0);

        }


static unsigned int lcm_esd_recover()
{
	 lcm_init();
   //push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}

LCM_DRIVER ili9806e_apex_A40318N31BI_BOE_dsi_2_vdo_wvga_lcm_drv = 
{
    .name			= "ili9806e_apex_A40318N31BI_BOE_dsi_2_vdo_wvga",
	.set_util_funcs = lcm_set_util_funcs,
    .compare_id    = lcm_compare_id,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
};

