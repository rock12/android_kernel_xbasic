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
#define LCM_DSI_CMD_MODE							1		//1

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

   
{ 0xF0 ,5,{0X55,0XAA,0X52,0X08,0X00}},
{ 0xB0 ,3,{0x0D,0x0D,0x0D}},
{ 0xB6 ,3,{0x34,0x34,0x34}},
{ 0xB1 ,3,{0x0D,0x0D,0x0D}},
{ 0xB7 ,3,{0x43,0x43,0x43}},
{ 0xB2 ,3,{0x01,0x01,0x01}},
{ 0xB8 ,3,{0x24,0x24,0x24}},
{ 0xB3 ,3,{0x05,0x05,0x05}},
{ 0xB9 ,3,{0x34,0x34,0x34}},
{ 0xB5 ,3,{0x0B,0x0B,0x0B}},
{ 0xBa ,3,{0x34,0x34,0x34}},
{ 0xBc ,3,{0x00,0x80,0x00}},
{ 0xBd ,3,{0x00,0x80,0x00}},
{ 0xBe,2,{0x00,0x56}},
{ 0xF0 ,5,{0X55,0XAA,0X52,0X08,0X00}},
{ 0xB1,2,{0xF8,0x00}},
{ 0xB4,1,{0x10}}, // 
{ 0xB5,1,{0x50}}, //
{ 0xB6,1,{0x0A}}, //
{ 0xB7 ,3,{0x00,0x00,0x00}},
{ 0xB8,4,{0x01,0x05,0x05,0x05}},
{ 0xBC,3,{0x00,0x00,0x00}},//0X00
{ 0xBD,5,{0x01,0x84,0x07,0x31,0x00}},
{ 0xBa,1,{0x01}}, //
{ 0xcc ,3,{0x03,0x00,0x00}},

 



       
{ 0xD1,52,{0x00,0x37,0x00,0x5F,0x00,0x8A,0x00,0xA9,0x00,0xB7,0x00,0xD9,0x01,0x01,0x01,0x31,0x01,0x56,0x01,0x91,0x01,0xC1,0x02,0x0D,0x02,0x48,0x02,0x4A,0x02,0x81,0x02,0xBB,0x02,0xDE,0x03,0x0C,0x03,0x2A,0x03,0x53,0x03,0x6C,0x03,0x8C,0x03,0x9C,0x03,0xAD,0x03,0xBF,0x03,0xC1}},      
{ 0xD2,52,{0x00,0x37,0x00,0x5F,0x00,0x8A,0x00,0xA9,0x00,0xB7,0x00,0xD9,0x01,0x01,0x01,0x31,0x01,0x56,0x01,0x91,0x01,0xC1,0x02,0x0D,0x02,0x48,0x02,0x4A,0x02,0x81,0x02,0xBB,0x02,0xDE,0x03,0x0C,0x03,0x2A,0x03,0x53,0x03,0x6C,0x03,0x8C,0x03,0x9C,0x03,0xAD,0x03,0xBF,0x03,0xC1}},      
{ 0xD3,52,{0x00,0x37,0x00,0x5F,0x00,0x8A,0x00,0xA9,0x00,0xB7,0x00,0xD9,0x01,0x01,0x01,0x31,0x01,0x56,0x01,0x91,0x01,0xC1,0x02,0x0D,0x02,0x48,0x02,0x4A,0x02,0x81,0x02,0xBB,0x02,0xDE,0x03,0x0C,0x03,0x2A,0x03,0x53,0x03,0x6C,0x03,0x8C,0x03,0x9C,0x03,0xAD,0x03,0xBF,0x03,0xC1}},      
{ 0xD4,52,{0x00,0x37,0x00,0x5F,0x00,0x8A,0x00,0xA9,0x00,0xB7,0x00,0xD9,0x01,0x01,0x01,0x31,0x01,0x56,0x01,0x91,0x01,0xC1,0x02,0x0D,0x02,0x48,0x02,0x4A,0x02,0x81,0x02,0xBB,0x02,0xDE,0x03,0x0C,0x03,0x2A,0x03,0x53,0x03,0x6C,0x03,0x8C,0x03,0x9C,0x03,0xAD,0x03,0xBF,0x03,0xC1}},      
{ 0xD5,52,{0x00,0x37,0x00,0x5F,0x00,0x8A,0x00,0xA9,0x00,0xB7,0x00,0xD9,0x01,0x01,0x01,0x31,0x01,0x56,0x01,0x91,0x01,0xC1,0x02,0x0D,0x02,0x48,0x02,0x4A,0x02,0x81,0x02,0xBB,0x02,0xDE,0x03,0x0C,0x03,0x2A,0x03,0x53,0x03,0x6C,0x03,0x8C,0x03,0x9C,0x03,0xAD,0x03,0xBF,0x03,0xC1}},                        
{ 0xD6,52,{0x00,0x37,0x00,0x5F,0x00,0x8A,0x00,0xA9,0x00,0xB7,0x00,0xD9,0x01,0x01,0x01,0x31,0x01,0x56,0x01,0x91,0x01,0xC1,0x02,0x0D,0x02,0x48,0x02,0x4A,0x02,0x81,0x02,0xBB,0x02,0xDE,0x03,0x0C,0x03,0x2A,0x03,0x53,0x03,0x6C,0x03,0x8C,0x03,0x9C,0x03,0xAD,0x03,0xBF,0x03,0xC1}},                 
                             


{ 0x3A,1,{0x77 }}, 
{ 0x2a,4,{0x00,0x00,0x01,0xDF}},
{ 0x2b,4,{0x00,0x00,0x03,0x55}},

{ 0x36,1,{0x00 }},
{ 0x35,1,{0x00 }},
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

    params->dbi.te_mode             = LCM_DBI_TE_MODE_DISABLED;
    params->dbi.te_edge_polarity       = LCM_POLARITY_RISING;

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
		params->dsi.packet_size=256;

	// Video mode setting
	params->dsi.intermediat_buffer_num = 2;
	  params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
    
    params->dsi.word_count=480*3;	//DSI CMD mode need set these two bellow params, different to 6577
    params->dsi.vertical_active_line=800;
  
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
    params->dsi.fbk_div =12;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real) 18 25 18 15

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
LCM_DRIVER nt35510_aizhuoertai_A40249N50AN_BOE_dsi_wvga_lcm_drv = 
{
    .name			= "nt35510_aizhuoertai_A40249N50AN_BOE_dsi_wvga",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,

    .compare_id    = lcm_compare_id,
   // .update         = lcm_update
   #if (LCM_DSI_CMD_MODE)
        .update         = lcm_update,
#endif
};
