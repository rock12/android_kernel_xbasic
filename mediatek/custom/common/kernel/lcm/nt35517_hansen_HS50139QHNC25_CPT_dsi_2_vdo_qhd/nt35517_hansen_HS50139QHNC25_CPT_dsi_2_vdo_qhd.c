#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
	#define LCM_LOG  printf
#else
 #define LCM_LOG  printk
	#include <linux/string.h>
	#if defined(BUILD_UBOOT)
		#include <asm/arch/mt_gpio.h>
	#else
		#include <mach/mt_gpio.h>
	#endif
#endif
#include "lcm_drv.h"
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  			(540)
#define FRAME_HEIGHT 			(960)

#define REGFLAG_DELAY          	0XFE
#define REGFLAG_END_OF_TABLE  	0xFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_ MODE			0


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

#define SET_RESET_PIN(v)    	(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 		(lcm_util.udelay(n))
#define MDELAY(n) 		(lcm_util.mdelay(n))



// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)						lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)		lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg					lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)		lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static unsigned int lcm_compare_id(void);
static int vcom = 0x4a;

//#ifndef TYD_LCD_USE_CUSTOM_NT35517_QHD
static struct LCM_setting_table lcm_initialization_setting[] = {


//=========================================================
//  Main LDI Power ON Sequence
//=========================================================

 //page=0 
{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
{0xBC, 3,{0x00, 0x00, 0x00}},  //Inversion
 //set frame rate 60Hz
//{0xBD,5,{0x01,0x38,0x08,0x40,0x01}},
 {0xB3,1,{ 0x00}},// PAGE0 ·Åµç´úÂë
//Display Control
//Normal Quad Mode
 {0xC7,1,{ 0x70}},
 {0xCA,11,{ 0x01, 0xE4, 0xE4, 0xE4, 0xE4, 0xE4, 0xE4, 0x08, 0x08, 0x00, 0x00}},
//=====================================================
//page1
{0xF0, 5,{0x55, 0xAA, 0x52, 0x08, 0x01}},
{0xB0,1,{ 0x0A}},//AVDD: 5.5V  (2.5­¿)
{0xB6,1,{ 0x44}},
{0xB1, 1,{0x0A}},//AVEE: -5.5V (2.5­¿)
{0xB7, 1,{0x34}},
{0xB2, 1,{0x02}},//VCL: -3.5V 
{0xB8, 1,{0x15}},
{0xB3,1,{0x10}},//VGH: 15.0V
{0xB9,1,{ 0x34}},
{0xB4, 1,{0x06}},//VGLX: -10.0V
{0xBA, 1,{0x24}},


// SET VOLTAGE
//{0xFF,4,{0xAA, 0x55, 0x25, 0x01}},
//{0xFA,12,{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x03}},

{0xBC, 3,{0x00, 0x8C, 0x02}},//Set VGMP = 5V / VGSP = 0.4936V
{0xBD,3,{ 0x00, 0x8D, 0x03}},//Set VGMN = -5V / VGSN = -0.4936V
{0xBE,1,{0x3C}},//Set DCVCOM=-0.7375V
//GAMMA+
//R+
{0xD1,16,{0x00,0x00,0x00,0x1F,0x00,0x3F,0x00,0x60,0x00,0x7D,0x00,0x9F,0x00,0xC0,0x01,0x00}},  
{0xD2,16,{0x01,0x2C,0x01,0x73,0x01,0xA8,0x02,0x02,0x02,0x47,0x02,0x49,0x02,0x84,0x02,0xC8}},  
{0xD3,16,{0x02,0xEC,0x03,0x1E,0x03,0x3F,0x03,0x6B,0x03,0x85,0x03,0xAA,0x03,0xC0,0x03,0xD6}},  
{0xD4,4,{0x03,0xE0,0x03,0xFF}},

//G+
{0xD5,16,{0x00,0x00,0x00,0x1F,0x00,0x3F,0x00,0x60,0x00,0x7D,0x00,0x9F,0x00,0xC0,0x01,0x00}},   
{0xD6,16,{0x01,0x2C,0x01,0x73,0x01,0xA8,0x02,0x02,0x02,0x47,0x02,0x49,0x02,0x84,0x02,0xC8}},  
{0xD7,16,{0x02,0xEC,0x03,0x1E,0x03,0x3F,0x03,0x6B,0x03,0x85,0x03,0xAA,0x03,0xC0,0x03,0xD6}},  
{0xD8,4,{0x03,0xE0,0x03,0xFF}},
  
//B+
{0xD9,16,{0x00,0x00,0x00,0x1F,0x00,0x3F,0x00,0x60,0x00,0x7D,0x00,0x9F,0x00,0xC0,0x01,0x00}},
{0xDD,16,{0x01,0x2C,0x01,0x73,0x01,0xA8,0x02,0x02,0x02,0x47,0x02,0x49,0x02,0x84,0x02,0xC8}},   
{0xDE,16,{0x02,0xEC,0x03,0x1E,0x03,0x3F,0x03,0x6B,0x03,0x85,0x03,0xAA,0x03,0xC0,0x03,0xD6}},  
{0xDF,4,{0x03,0xE0,0x03,0xFF}},


//GAMMA-
//R-
{0xE0,16,{0x00,0x00,0x00,0x1F,0x00,0x3F,0x00,0x60,0x00,0x7D,0x00,0x9F,0x00,0xC0,0x01,0x00}},  
{0xE1,16,{0x01,0x2C,0x01,0x73,0x01,0xA8,0x02,0x02,0x02,0x47,0x02,0x49,0x02,0x84,0x02,0xC8}},  
{0xE2,16,{0x02,0xEC,0x03,0x1E,0x03,0x3F,0x03,0x6B,0x03,0x85,0x03,0xAA,0x03,0xC0,0x03,0xD6}},     
{0xE3,4,{0x03,0xE0,0x03,0xFF}}, 
   
//G-
{0xE4,16,{0x00,0x00,0x00,0x1F,0x00,0x3F,0x00,0x60,0x00,0x7D,0x00,0x9F,0x00,0xC0,0x01,0x00}},  
{0xE5,16,{0x01,0x2C,0x01,0x73,0x01,0xA8,0x02,0x02,0x02,0x47,0x02,0x49,0x02,0x84,0x02,0xC8}},  
{0xE6,16,{0x02,0xEC,0x03,0x1E,0x03,0x3F,0x03,0x6B,0x03,0x85,0x03,0xAA,0x03,0xC0,0x03,0xD6}},  
{0xE7,4,{0x03,0xE0,0x03,0xFF}}, 
  
//B-
{0xE8,16,{0x00,0x00,0x00,0x1F,0x00,0x3F,0x00,0x60,0x00,0x7D,0x00,0x9F,0x00,0xC0,0x01,0x00}},
{0xE9,16,{0x01,0x2C,0x01,0x73,0x01,0xA8,0x02,0x02,0x02,0x47,0x02,0x49,0x02,0x84,0x02,0xC8}},   
{0xEA,16,{0x02,0xEC,0x03,0x1E,0x03,0x3F,0x03,0x6B,0x03,0x85,0x03,0xAA,0x03,0xC0,0x03,0xD6}},  
{0xEB,4,{0x03,0xE0,0x03,0xFF}}, 
//*************************************
// Sleep Out
//*************************************
{0x11,1,{0x00}},

{REGFLAG_DELAY, 120, {}},
//*************************************
// Display On
//*************************************
{0x29, 1 ,{0x00}},
{REGFLAG_END_OF_TABLE, 0x00, {}}


};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
//	{0x28, 1, {0x00}},
//	{REGFLAG_DELAY, 50, {}},

    // Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 200, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

/*
static struct LCM_setting_table lcm_backlight_level_setting[] = {
	{0x51, 1, {0xFF}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
*/

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
    params->dbi.te_mode				= LCM_DBI_TE_MODE_VSYNC_ONLY;
    params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

    params->dsi.mode   = SYNC_EVENT_VDO_MODE;
   
    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_TWO_LANE;
    
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq	= LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding 	= LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format	  = LCM_DSI_FORMAT_RGB888;
    
    // Video mode setting		
    //params->dsi.intermediat_buffer_num = 2;
    
    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
    
    //params->dsi.word_count=FRAME_WIDTH*3;	//DSI CMD mode need set these two bellow params, different to 6577
    params->dsi.vertical_active_line=FRAME_HEIGHT;

	params->dsi.vertical_sync_active	= 2;// 3    2
	params->dsi.vertical_backporch		= 60;// 20   1
	params->dsi.vertical_frontporch		= 60; // 1  12
	params->dsi.vertical_active_line	= FRAME_HEIGHT; 

	params->dsi.horizontal_sync_active	= 4;// 50  2
	params->dsi.horizontal_backporch	= 60;
	params->dsi.horizontal_frontporch	= 60;
  params->dsi.horizontal_blanking_pixel	= 60;
	params->dsi.horizontal_active_pixel	= FRAME_WIDTH;
//    params->dsi.compatibility_for_nvk = 0;		// this parameter would be set to 1 if DriverIC is NTK's and when force match DSI clock for NTK's	

	// Bit rate calculation
	//1 Every lane speed
	params->dsi.pll_div1 = 1;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps  0
	params->dsi.pll_div2 = 0;		// div2=0,1,2,3;div1_real=1,2,4,4	                1
	//params->dsi.fbk_sel = 1;
	params->dsi.fbk_div = 17;      // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)      17

}


static void lcm_init(void)
{
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(150);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
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
	dsi_set_cmdq(&data_array, 3, 1);
	//MDELAY(1);
	
	data_array[0]= 0x00053902;
	data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[2]= (y1_LSB);
	dsi_set_cmdq(&data_array, 3, 1);
	//MDELAY(1);
	
	data_array[0]= 0x00290508;
	dsi_set_cmdq(&data_array, 1, 1);
	//MDELAY(1);
	
	data_array[0]= 0x002c3909;
	dsi_set_cmdq(&data_array, 1, 0);
	//MDELAY(1);

}

static void lcm_suspend(void)
{
	unsigned int data_array[16];
	data_array[0]=0x00280500;
	dsi_set_cmdq(&data_array,1,1);
	MDELAY(10);
	data_array[0]=0x00100500;
	dsi_set_cmdq(&data_array,1,1);
	MDELAY(120);
}


static void lcm_resume(void)
{
	
	unsigned int data_array[16];
	data_array[0]=0x00110500;
	dsi_set_cmdq(&data_array,1,1);
	MDELAY(140);
	data_array[0]=0x00290500;
	dsi_set_cmdq(&data_array,1,1);
	MDELAY(10);

//lcm_initialization_setting[18].para_list[0]++;
//lcm_init();
//LCM_LOG("wangdong:0x%02x\n",lcm_initialization_setting[18].para_list[0]);
}


static unsigned int lcm_compare_id(void)
{
	//return 1;
       unsigned int id = 0,id1=0,id2 = 0;
 
    unsigned char buffer[2];
 
   unsigned int data_array[16];
 
    

 
 
    SET_RESET_PIN(1);  //NOTE:should reset LCM firstly
 
    MDELAY(10);
 
    SET_RESET_PIN(0);
 
    MDELAY(10);
 
    SET_RESET_PIN(1);
 
    MDELAY(10);        



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
 
    id1 = buffer[0]; //we only need ID
 
    id2= buffer[1]; //we test buffer 1

    id = (id1<<8) | id2;
    return (0x5517 == id)?1:0;

}

static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK

/*
#ifdef BUILD_LK
	printf("ccccccccccccccccccccccccccccccccccccccccccccccccccccc");
#else
	printk(KERN_ERR "ccccccccccccccccccccccccccccccccccccccccccccccccccccc");
	printk(KERN_ERR "ccccccccccccccccccccccccccccccccccccccccccccccccccccc");
	printk(KERN_ERR "ccccccccccccccccccccccccccccccccccccccccccccccccccccc");
#endif
*/
    int array[4];
    char buffer[3]={0};


	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0x0A, buffer, 1);
//#ifndef BUILD_LK
	printk("mycat linglong lcm_esd_check buffer0=%4x\n",buffer[0]);
//#endif
    if(0x9c == buffer[0])
        return FALSE;
    else if(0x02 == buffer[0])
    {
	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0x0A, buffer, 1);
    if(0x9c == buffer[0])
        return FALSE;
    else if(0x02 == buffer[0])
        {
        printk(KERN_ERR "nt35517:read register 0x0A=%4x\n",buffer[0]);
        return TRUE;
         }
    }
#endif


}

static unsigned int lcm_esd_recover(void)
{
/*#ifdef BUILD_LK
	printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
#else
	printk(KERN_ERR "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
#endif*/
    lcm_init();
    //lcm_resume();

    return TRUE;
}


LCM_DRIVER nt35517_hansen_HS50139QHNC25_CPT_dsi_2_vdo_qhd_lcm_drv = 
{
	.name		= "nt35517_hansen_HS50139QHNC25_CPT_dsi_2_vdo_qhd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
#if (LCM_DSI_CMD_MODE)
	.compare_id    = lcm_compare_id,
	.update         = lcm_update,
#endif

    .esd_check      = lcm_esd_check,
    .esd_recover    = lcm_esd_recover,
};

