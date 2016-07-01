#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#else
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
    #include <linux/string.h>
   
#endif
#include "lcm_drv.h"




// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (800)
#define LCM_ID       (0x55)
#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xFF   // END OF REGISTERS MARKER
//#define LCM_DSI_CMD_MODE									0

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
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)               

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


///////
static struct LCM_setting_table lcm_initialization_setting[] = {
{ 0xF0 ,5,{0x55,0xAA,0x52,0x08,0x01}},
{ 0xB6,3,{0x34,0x34,0x34 }},  
{ 0xB0,3,{0x09,0x09,0x09}},
{ 0xB7,3,{0x24,0x24,0x24}},
{ 0xB1,3,{0x09,0x09,0x09}},
{ 0xB8,3,{0x34,0x34,0x34 }},
{ 0xB2,3,{0x00,0x00,0x00 }}, //
{ 0xB9,3,{0x24,0x24,0x24}},
{ 0xB3,3,{0x05,0x05,0x05 }}, // 
{ 0xBF,1,{0x01 }}, // 
{ 0xBA,3,{0x34,0x34,0x34}},
{ 0xB5,3,{0x0B,0x0B,0x0B}},//
{ 0xBC,3,{0x00,0xA3,0x00}},
{ 0xBD,3,{0x00,0xA3,0x00}},
{ 0xBE,2,{0x00,0x19}},//0x55,0x60,0x8a,0x93         
{ 0xD1,52,{0x00,0x37,0x00,0x52,0x00,0x7B,0x00,0x99,0x00,0xB1,0x00,0xD2,0x00,0xF6,0x01,0x27,0x01,0x4E,0x01,0x8C,0x01,0xBE,0x02,0x0B,0x02,0x48,0x02,0x4A,0x02,0x7E,0x02,0xBC,0x02,0xE1,0x03,0x10,0x03,0x31,0x03,0x5A,0x03,0x73,0x03,0x94,0x03,0x9F,0x03,0xB3,0x03,0xB9,0x03,0xC1}},      
{ 0xD2,52,{0x00,0x37,0x00,0x52,0x00,0x7B,0x00,0x99,0x00,0xB1,0x00,0xD2,0x00,0xF6,0x01,0x27,0x01,0x4E,0x01,0x8C,0x01,0xBE,0x02,0x0B,0x02,0x48,0x02,0x4A,0x02,0x7E,0x02,0xBC,0x02,0xE1,0x03,0x10,0x03,0x31,0x03,0x5A,0x03,0x73,0x03,0x94,0x03,0x9F,0x03,0xB3,0x03,0xB9,0x03,0xC1}},         
{ 0xD3,52,{0x00,0x37,0x00,0x52,0x00,0x7B,0x00,0x99,0x00,0xB1,0x00,0xD2,0x00,0xF6,0x01,0x27,0x01,0x4E,0x01,0x8C,0x01,0xBE,0x02,0x0B,0x02,0x48,0x02,0x4A,0x02,0x7E,0x02,0xBC,0x02,0xE1,0x03,0x10,0x03,0x31,0x03,0x5A,0x03,0x73,0x03,0x94,0x03,0x9F,0x03,0xB3,0x03,0xB9,0x03,0xC1}},           
{ 0xD4,52,{0x00,0x37,0x00,0x52,0x00,0x7B,0x00,0x99,0x00,0xB1,0x00,0xD2,0x00,0xF6,0x01,0x27,0x01,0x4E,0x01,0x8C,0x01,0xBE,0x02,0x0B,0x02,0x48,0x02,0x4A,0x02,0x7E,0x02,0xBC,0x02,0xE1,0x03,0x10,0x03,0x31,0x03,0x5A,0x03,0x73,0x03,0x94,0x03,0x9F,0x03,0xB3,0x03,0xB9,0x03,0xC1}},        
{ 0xD5,52,{0x00,0x37,0x00,0x52,0x00,0x7B,0x00,0x99,0x00,0xB1,0x00,0xD2,0x00,0xF6,0x01,0x27,0x01,0x4E,0x01,0x8C,0x01,0xBE,0x02,0x0B,0x02,0x48,0x02,0x4A,0x02,0x7E,0x02,0xBC,0x02,0xE1,0x03,0x10,0x03,0x31,0x03,0x5A,0x03,0x73,0x03,0x94,0x03,0x9F,0x03,0xB3,0x03,0xB9,0x03,0xC1}},         
{ 0xD6,52,{0x00,0x37,0x00,0x52,0x00,0x7B,0x00,0x99,0x00,0xB1,0x00,0xD2,0x00,0xF6,0x01,0x27,0x01,0x4E,0x01,0x8C,0x01,0xBE,0x02,0x0B,0x02,0x48,0x02,0x4A,0x02,0x7E,0x02,0xBC,0x02,0xE1,0x03,0x10,0x03,0x31,0x03,0x5A,0x03,0x73,0x03,0x94,0x03,0x9F,0x03,0xB3,0x03,0xB9,0x03,0xC1}},

                  
//########################
//#ENABLE CMD2 Page 0               
{ 0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
//########################       

{ 0xB0,5,{0x00,0x05,0x02,0x05,0x02}},

//#RAM Keep                    
//{ 0xB1,1,{0xFC}},             

//#Vivid Color
//{ 0xB4,1,{0x10}},       

//## SDT:
{ 0xB6,1,{0x0A}},

//#Set Gate EQ        
{ 0xB7,2,{0x00,0x00}},///////////

//#Set Source EQ        
{ 0xB8,4,{0x01,0x05,0x05,0x05}},


                       
//#Inversion Control                
{ 0xBC,3,{0x01,0x01,0x01}},            
{ 0xCC,3,{0x03,0x00,0x00}},


//#Porch Adjust                
{ 0xBD,5,{0x01,0x84,0x07,0x31,0x00 }}, 
{ 0xBA,1,{0x01}},
                                          
{ 0x36,1,{0xc8}}, ////
{ 0x35,1,{0x00}},             
                             


{ 0x3A,1,{0x77 }}, 


//#StartUp  
{0x11,0,{}} ,
 {REGFLAG_DELAY, 180, {}},
 //#Display On 
 {0x29,0,{}} ,
 {REGFLAG_DELAY, 20, {}},
   {0x2C,0,{}} ,
   //#ENABLE CMD2 Page 0               
//{ 0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
//{ 0xB1,2,{0xE8,0x0E}},
//{REGFLAG_END_OF_TABLE, 0x00, {}}


};
/////




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
	params->dbi.te_mode 				= LCM_DBI_TE_MODE_DISABLED;
		//params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

	params->dsi.mode   = SYNC_EVENT_VDO_MODE;


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
    
    params->dsi.word_count=480*3;	//DSI CMD mode need set these two bellow params, different to 6577
    params->dsi.vertical_active_line=854;
  
     params->dsi.vertical_sync_active				= 2;//6;
    params->dsi.vertical_backporch					= 20;// 70;
    params->dsi.vertical_frontporch					= 20;//70;	
    params->dsi.vertical_active_line				= FRAME_HEIGHT;
    
    params->dsi.horizontal_sync_active				= 2;//6;
    params->dsi.horizontal_backporch				= 80;//22;
    params->dsi.horizontal_frontporch				= 80;//120;
    params->dsi.horizontal_blanking_pixel				= 60;

	// Bit rate calculation
//	params->dsi.pll_div1=1;		// 
//	params->dsi.pll_div2=1;			// 
//	params->dsi.fbk_div=32;
    params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4
    params->dsi.pll_div2=0;		// div2=0,1,2,3;div2_real=1,2,4,4
    params->dsi.fbk_div =15;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real) 18 25 18 15

    /* ESD or noise interference recovery For video mode LCM only. */ // Send TE packet to LCM in a period of n frames and check the response.
}


static void lcm_init(void)
{
    SET_RESET_PIN(1);
    MDELAY(20);
    SET_RESET_PIN(0);
    MDELAY(80);
    SET_RESET_PIN(1);
    MDELAY(200);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
    unsigned int data_array[16];
#if defined(BUILD_LK)

#else
      data_array[0] = 0x00002200;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(20);
#endif

    data_array[0] = 0x00280500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(10);

    data_array[0] = 0x00100500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(120);


}

static void lcm_resume(void)
{
	//add by xia lei jie for LCD INIT	
	//lcm_init();
//lcm_initialization_setting[14].para_list[1]+=2;
lcm_init();
}




#define GPIO_LCD_PIN_ID GPIO97
static unsigned int lcm_compare_id(void)
{
	int i;
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
  	mt_set_gpio_mode(GPIO_LCD_PIN_ID, GPIO_MODE_00);
	 mt_set_gpio_dir(GPIO_LCD_PIN_ID, GPIO_DIR_IN);
	//mt_set_gpio_pull_enable(GPIO_LCD_PIN_ID, 1);
	//mt_set_gpio_pull_select(GPIO_LCD_PIN_ID, GPIO_PULL_UP);	
  	
 	i=mt_get_gpio_in(GPIO_LCD_PIN_ID);  
#ifdef BUILD_LK
	printf("MYCAT NT35512 lk  lcm_compare_id=%d\n",i);
#else
     printk("MYCAT NT35512 kernel  lcm_compare_id=%d\n",i);
#endif
	if(i == 1)
		return 1;
	else
    		return 1;
}





// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER nt35512_aizhuoertai_A40249N50XN_A_Hyundai_vdo_wvga_lcm_drv = 
{
    .name			= "nt35512_aizhuoertai_A40249N50XN_A_Hyundai_vdo_wvga",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,

    .compare_id    = lcm_compare_id,
   // .update         = lcm_update
};
