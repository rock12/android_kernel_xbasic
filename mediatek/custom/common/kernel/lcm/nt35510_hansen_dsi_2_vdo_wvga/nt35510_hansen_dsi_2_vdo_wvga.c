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



		{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}}, 
		{0xB0,3,{0x09,0x09,0x09}}, 
		{0xB6,3,{0x34,0x34,0x34}},           
		{0xB1,3,{0x09,0x09,0x09}}, 
		{0xB7,3,{0x24,0x24,0x24}},           
		{0xB3,3,{0x05,0x05,0x05}},      
		{0xB9,3,{0x24,0x24,0x24}},              
		{0xbf,1,{0x01}},          
		{0xB5,3,{0x0b,0x0b,0x0b}},            
		{0xBA,3,{0x34,0x24,0x24}},             
		{0xBC,3,{0x00,0xA3,0X00}},            
		{0xBD,3,{0x00,0xA3,0x00}},              
		{0xBE,2,{0x00,0x4a}},  //0x40 
		             
		{0xD1,52,{0x00,0x37,0x00,0x64,0x00,0x84,0x00,0xA3,0x00,0xB6,0x00,0xDC,0x00,0xF4,0x01,0x2a,0x01,0x4A,0x01,0x8a,0x01,0xbb,0x02,0x0C,0x02,0x48,
		          0x02,0x4A,0x02,0x82,0x02,0xbC,0x02,0xE1,0x03,0x0F,0x03,0x32,0x03,0x5B,0x03,0x73,0x03,0x91,0x03,0xA0,0x03,0xAF,0x03,0xBA,0x03,0xC1}}, 
		          
		{0xD2,52,{0x00,0x37,0x00,0x64,0x00,0x84,0x00,0xA3,0x00,0xB6,0x00,0xDC,0x00,0xF4,0x01,0x2a,0x01,0x4A,0x01,0x8a,0x01,0xbb,0x02,0x0C,0x02,0x48,
							0x02,0x4A,0x02,0x82,0x02,0xbC,0x02,0xE1,0x03,0x0F,0x03,0x32,0x03,0x5B,0x03,0x73,0x03,0x91,0x03,0xA0,0x03,0xAF,0x03,0xBA,0x03,0xC1}}, 
		            
		{0xD3,52,{0x00,0x37,0x00,0x64,0x00,0x84,0x00,0xA3,0x00,0xB6,0x00,0xDC,0x00,0xF4,0x01,0x2a,0x01,0x4A,0x01,0x8a,0x01,0xbb,0x02,0x0C,0x02,0x48,
		          0x02,0x4A,0x02,0x82,0x02,0xbC,0x02,0xE1,0x03,0x0F,0x03,0x32,0x03,0x5B,0x03,0x73,0x03,0x91,0x03,0xA0,0x03,0xAF,0x03,0xBA,0x03,0xC1}},        
		          
		{0xD4,52,{0x00,0x37,0x00,0x64,0x00,0x84,0x00,0xA3,0x00,0xB6,0x00,0xDC,0x00,0xF4,0x01,0x2a,0x01,0x4A,0x01,0x8a,0x01,0xbb,0x02,0x0C,0x02,0x48,
		          0x02,0x4A,0x02,0x82,0x02,0xbC,0x02,0xE1,0x03,0x0F,0x03,0x32,0x03,0x5B,0x03,0x73,0x03,0x91,0x03,0xA0,0x03,0xAF,0x03,0xBA,0x03,0xC1}}, 
		          
		{0xD5,52,{0x00,0x37,0x00,0x64,0x00,0x84,0x00,0xA3,0x00,0xB6,0x00,0xDC,0x00,0xF4,0x01,0x2a,0x01,0x4A,0x01,0x8a,0x01,0xbb,0x02,0x0C,0x02,0x48,
							0x02,0x4A,0x02,0x82,0x02,0xbC,0x02,0xE1,0x03,0x0F,0x03,0x32,0x03,0x5B,0x03,0x73,0x03,0x91,0x03,0xA0,0x03,0xAF,0x03,0xBA,0x03,0xC1}}, 
		            
		{0xD6,52,{0x00,0x37,0x00,0x64,0x00,0x84,0x00,0xA3,0x00,0xB6,0x00,0xDC,0x00,0xF4,0x01,0x2a,0x01,0x4A,0x01,0x8a,0x01,0xbb,0x02,0x0C,0x02,0x48,
		          0x02,0x4A,0x02,0x82,0x02,0xbC,0x02,0xE1,0x03,0x0F,0x03,0x32,0x03,0x5B,0x03,0x73,0x03,0x91,0x03,0xA0,0x03,0xAF,0x03,0xBA,0x03,0xC1}},       
		                    
		          
		              
		
		//{0xD1,52,{0x00,0x37,0x00,0x64,0x00,0x84,0x00,0xA3,0x00,0xB6,0x00,0xDC,0x00,0xF4,0x01,0x2a,0x01,0x4A,0x01,0x8a,0x01,0xbb,0x02,0x0C,0x02,0x48,
		//          0x02,0x4A,0x02,0x82,0x02,0xbC,0x02,0xE1,0x03,0x0F,0x03,0x32,0x03,0x5B,0x03,0x73,0x03,0x91,0x03,0xA0,0x03,0xAF,0x03,0xBA,0x03,0xC1,  
			
		              
		{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}}, 
		   
		{0xB6,1,{0x0a}}, 
		       
		{0xB7,2,{0x00,0x00}}, 
		     
		{0xB8,4,{0x01,0x05,0x05,0x05}}, 
		        
		{0xBA,1,{0x01}}, 
		        
		{0xBC,3,{0x00,0x00,0x00}}, 
		            
		{0xBD,5,{0x01,0x84,0x07,0x31,0x00}}, 
		            
		{0xBE,5,{0x01,0x84,0x07,0x31,0x00}}, 
		           
		{0xBF,5,{0x01,0x84,0x07,0x31,0x00}}, 
		
		{0xCC,3,{0x03,0x00,0x00}}, 
		            
		{0xB1,2,{0xF8,0x00}}, 
		              
     {0x36, 1, {0x00}},
     	   
    {0x35, 1, {0x00}},	
    {REGFLAG_DELAY, 1, {}},

    {0x44,1,{0x00,0x00}},

    {0x3A, 1, {0x77}},	
    {REGFLAG_DELAY, 1, {}},

    {0x11, 0, {0x00}},	
    {REGFLAG_DELAY, 120, {}},

    {0x29, 0, {0x00}},	
    {REGFLAG_DELAY, 20, {}},
    
    {0x2C, 0, {0x00}},	
    {REGFLAG_DELAY, 20, {}},

    {REGFLAG_END_OF_TABLE, 0x00, {}}


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
    params->dsi.fbk_div =16;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real) 18 25 18 15

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
//lcm_initialization_setting[12].para_list[1]+=1;
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


static unsigned int lcm_compare_id(void)
{
	unsigned int id = 0, id2 = 0;
    unsigned char buffer[2];
    unsigned int data_array[16];
    

    SET_RESET_PIN(1);  //NOTE:should reset LCM firstly
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(10);	
    
    /*	
    data_array[0] = 0x00110500;		// Sleep Out
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(120);
    */
    
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
    id = buffer[0]; //we only need ID
    id2= buffer[1]; //we test buffer 1
    
    return (LCM_ID == id)?1:0;
}





// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER nt35510_hansen_dsi_2_vdo_wvga_lcm_drv = 
{
    .name			= "nt35510_hansen_dsi_2_vdo_wvga",
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
