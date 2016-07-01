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

#define FRAME_WIDTH  			(540)
#define FRAME_HEIGHT 			(960)

//#define BIRD_ESD_CHECK
#define REGFLAG_DELAY          	0XFE
#define REGFLAG_END_OF_TABLE  	0xFFF   // END OF REGISTERS MARKER
//static unsigned int lcm_esd_test = 0;      ///only for ESD test
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

//#ifndef TYD_LCD_USE_CUSTOM_NT35517_QHD  XLD0500940L1-25  2014-3-17
static struct LCM_setting_table lcm_initialization_setting[] = {
     /////////XLD0500940L1-25   NT35517+LG5.0////// 2014-3-17

 {0xFF,4,{0xAA,0x55,0x25,0x01}},
 {0xF0,5,{ 0x55, 0xAA, 0x52, 0x08, 0x00}}, 
 {0xB3,1,{0x00}},
//Backward Scan      CTB=CRL=1
 {0xB1,2,{0xFC,0x00}},
// Source EQ control (Mode 2)
 {0xB8,4,{0x01,0x04,0x05,0x06}},
// Display Timing: Dual 8-phase 4-overlap
 {0xC9,6,{0x63,0x06,0x0D,0x1A,0x17,0x00}},//6c
 {0xBC,3,{0x02,0x00,0x00}},

 //{0xE0,2,{0x01,0x03}},
//*************************************
// Select CMD2, Page 1
//*************************************
 {0xF0, 5,{0x55, 0xAA, 0x52, 0x08, 0x01}},
// AVDD: 5.5V
 {0xB0,3,{0x05,0x05,0x05}},
// AVEE: -5.5V
 {0xB1,1,{0x05,0x05,0x05}},
// VCL: -3.5V
 {0xB2,3,{0x02,0x02,0x02}},//02
// VGH: 15.0V
 {0xB3, 3,{0x0d,0x0d,0x0d}},//0e
// VGLX: -10.0V
 {0xB4,3,{0x09,0x09,0x09}},//09
// AVDD: 3xVDDB
 {0xB6, 3,{0x43,0x43,0x43}},//34
// AVEE: -2xVDDB
 {0xB7,3,{0x33,0x33,0x33}},
// VCL: -2xVDDB    pump clock : Hsync
 {0xB8,3,{0x34,0x34,0x34}},//15
// VGH: 2xAVDD-AVEE
 {0xB9,3,{0x26,0x26,0x26}},//22
// VGLX: AVEE-AVDD
 {0xBA,3,{0x26,0x26,0x26}},//22
// Set VGMP = 5.2V / VGSP = 0V
 {0xBC,3,{0x00,0xa9,0x00}},//0xA0
// Set VGMN = -5.2V / VGSN = 0V
 {0xBD,3,{0x00,0xa5,0x00}},//0xA0
// Set VCOM_offset
  {0xBf,1,{0x58}},//5A,0x55,0x58
  {0xBe,1,{0x58}},
//{0xBF,1,{0x60}}, //63 68 
 {0xC0,2,{0x04,0x00}},
 {0xCA,1,{0x00}},
//R(+) MCR cmd
 {0xD1,16,{0x00,0x16,0x00,0x21,0x00,0x37,0x00,0x4c,0x00,0x61,0x00,0x8c,0x00,0xa9,0x00,0xd6}},
 {0xD2,16,{0x00,0xF0,0x01,0x36,0x01,0x65,0x01,0xAF,0x01,0xEF,0x01,0xF1,0x02,0x28,0x02,0x6B}},
 {0xD3,16,{0x02,0x90,0x02,0xC3,0x02,0xE6,0x03,0x18,0x03,0x3A,0x03,0x69,0x03,0x84,0x03,0xAC}},
 {0xD4,4, {0x03,0xE3,0x03,0xFF}},
 {0xD5,16,{0x00,0x16,0x00,0x21,0x00,0x37,0x00,0x4c,0x00,0x61,0x00,0x8c,0x00,0xa9,0x00,0xd6}},
 {0xD6,16,{0x00,0xF0,0x01,0x36,0x01,0x65,0x01,0xAF,0x01,0xEF,0x01,0xF1,0x02,0x28,0x02,0x6B}},
 {0xD7,16,{0x02,0x90,0x02,0xC3,0x02,0xE6,0x03,0x18,0x03,0x3A,0x03,0x69,0x03,0x84,0x03,0xAC}},
 {0xD8,4, {0x03,0xE3,0x03,0xFF}},
 {0xD9,16,{0x00,0x16,0x00,0x21,0x00,0x37,0x00,0x4c,0x00,0x61,0x00,0x8c,0x00,0xa9,0x00,0xd6}},
 {0xDD,16,{0x00,0xF0,0x01,0x36,0x01,0x65,0x01,0xAF,0x01,0xEF,0x01,0xF1,0x02,0x28,0x02,0x6B}},
 {0xDE,16,{0x02,0x90,0x02,0xC3,0x02,0xE6,0x03,0x18,0x03,0x3A,0x03,0x69,0x03,0x84,0x03,0xAC}},
 {0xDF,4, {0x03,0xE3,0x03,0xFF}},
 {0xE0,16,{0x00,0x16,0x00,0x21,0x00,0x37,0x00,0x4c,0x00,0x61,0x00,0x8c,0x00,0xa9,0x00,0xd6}},
 {0xE1,16,{0x00,0xF0,0x01,0x36,0x01,0x65,0x01,0xAF,0x01,0xEF,0x01,0xF1,0x02,0x28,0x02,0x6B}},
 {0xE2,16,{0x02,0x90,0x02,0xC3,0x02,0xE6,0x03,0x18,0x03,0x3A,0x03,0x69,0x03,0x84,0x03,0xAC}},
 {0xE3,4, {0x03,0xE3,0x03,0xFF}},
 {0xE4,16,{0x00,0x16,0x00,0x21,0x00,0x37,0x00,0x4c,0x00,0x61,0x00,0x8c,0x00,0xa9,0x00,0xd6}},
 {0xE5,16,{0x00,0xF0,0x01,0x36,0x01,0x65,0x01,0xAF,0x01,0xEF,0x01,0xF1,0x02,0x28,0x02,0x6B}},
 {0xE6,16,{0x02,0x90,0x02,0xC3,0x02,0xE6,0x03,0x18,0x03,0x3A,0x03,0x69,0x03,0x84,0x03,0xAC}},
 {0xE7,4, {0x03,0xE3,0x03,0xFF}},
 {0xE8,16,{0x00,0x16,0x00,0x21,0x00,0x37,0x00,0x4c,0x00,0x61,0x00,0x8c,0x00,0xa9,0x00,0xd6}},
 {0xE9,16,{0x00,0xF0,0x01,0x36,0x01,0x65,0x01,0xAF,0x01,0xEF,0x01,0xF1,0x02,0x28,0x02,0x6B}},
 {0xEA,16,{0x02,0x90,0x02,0xC3,0x02,0xE6,0x03,0x18,0x03,0x3A,0x03,0x69,0x03,0x84,0x03,0xAC}},
 {0xEB,4, {0x03,0xE3,0x03,0xFF}},

  {0x35, 1, {0x00}},
  //{0x36, 1, {0x00}},
  {0x3A, 0, {0x77}},
 //{0x55, 2, {0x40}},

 {0x11, 0, {0x00}},
 {REGFLAG_DELAY, 120, {}},
 {0x29, 0, {0x00}},
 {REGFLAG_DELAY, 50, {}},
 {REGFLAG_END_OF_TABLE, 0x00, {}}

};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},

    // Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 200, {}},
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

	params->dsi.vertical_sync_active	= 2;//2;// 3    2
	params->dsi.vertical_backporch		= 60;// 20   1
	params->dsi.vertical_frontporch		= 60; // 1  12
	params->dsi.vertical_active_line	= FRAME_HEIGHT; 

	params->dsi.horizontal_sync_active	= 2;// 50  2
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
	params->dsi.fbk_div = 18;      // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)      17

    /* ESD or noise interference recovery For video mode LCM only. */ // Send TE packet to LCM in a period of n frames and check the response.
    params->dsi.lcm_int_te_monitor = 0;// FALSE;
    params->dsi.lcm_int_te_period = 1; // Unit : frames

    // Need longer FP for more opportunity to do int. TE monitor applicably.
    if(params->dsi.lcm_int_te_monitor)
    params->dsi.vertical_frontporch *= 2;

    // Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.)
    params->dsi.lcm_ext_te_monitor = 0;
    // Non-continuous clock
    params->dsi.noncont_clock = 1;// TRUE;
    params->dsi.noncont_clock_period = 2; // Unit : frames
}


static void lcm_init(void)
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
    MDELAY(100);
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
}


static unsigned int lcm_compare_id(void)
{
	//return 1;
    unsigned int id = 0,id1=0,id2 = 0;
    unsigned char buffer[2];
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
    id2= buffer[1]; //we test buffer
    id = (id1<<8) | id2;
    return (0x5517 == id)?1:0;

}

#if defined (BIRD_ESD_CHECK)
/*static unsigned int lcm_esd_check(void)
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
	 
	printk("mycat$$ hongsheng ###*** esd check buffer[0] = 0x%x\r\n",buffer[0]);
	
	if(buffer[0] == 0x9C)//9C
		return 0;
	else
		return 1;
 			
#endif
}*/
static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK
    static int err_count = 0,uncount=0,count=0;
    unsigned char buffer_1[12];
    unsigned int array[16];
    int i;
    unsigned char fResult;

    printk("lcm_esd_check<<<\n");
    for(i = 0; i < 11; i++)
      buffer_1[i] = 0x00;

    //---------------------------------
    // Set Maximum Return Size
    //---------------------------------    
    array[0] = 0x00013700;
    dsi_set_cmdq(array, 1, 1);

    //---------------------------------
    // Read [9Ch, 00h, ECC] + Error Report(4 Bytes)
    //---------------------------------
    read_reg_v2(0x0A, buffer_1, 7);

    printk(KERN_EMERG "lcm_esd_check: read(0x0A)\n");
    for(i = 0; i < 7; i++)                
      printk(KERN_EMERG "buffer_1[%d]:0x%x \n",i,buffer_1[i]);

    //---------------------------------
    // Judge Readout & Error Report
    //---------------------------------
    if(buffer_1[3] == 0x02) // Check data identifier of error report
    {
      if(buffer_1[4] & 0x02) // Check SOT sync error
        err_count++;
      else
        err_count = 0;
    }
    else
    {
      err_count = 0;
    }

    printk(KERN_EMERG "lcm_esd_check err_count=%d\n",err_count);
    if((buffer_1[0] != 0x9C) || (err_count >= 2))
    {
      err_count = 0;
      uncount++;

      printk(KERN_EMERG "lcm_esd_check unnormal uncount=%d\n",uncount);
      printk("lcm_esd_check>>>\n");
      
      fResult = 1;
      //return TRUE;
    }
    else
    {        
      count++;
      printk(KERN_EMERG "lcm_esd_check normal count=%d\n",count);
      printk("lcm_esd_check>>>\n");
      
      fResult = 0;
      //return FALSE;
    }

    //---------------------------------
    // Shut-Down Peripherial
    //---------------------------------    
    array[0] = 0x00002200;
    dsi_set_cmdq(array, 1, 1);

    //---------------------------------
    // Set Maximum Return Size
    //---------------------------------    
    array[0] = 0x00033700;
    dsi_set_cmdq(array, 1, 1);

    //---------------------------------
    // Clear D-PHY Buffer
    // Read [WC, WC, ECC, P1, P2, P3, CRC0, CRC1]+ Error Report(4 Bytes)
    //---------------------------------
    read_reg_v2(0xBC, buffer_1, 12);

    printk(KERN_EMERG "lcm_esd_check: read(0xBC)\n");
    for(i = 0; i < 12; i++)                
      printk(KERN_EMERG "buffer_1[%d]:0x%x \n",i,buffer_1[i]);
      
    if(fResult) return TRUE;
    else return FALSE;
#endif

}

static unsigned int lcm_esd_recover(void)
{
    lcm_init();
    return 1;
}
#endif


LCM_DRIVER nt35517_xingliangda_XLD0500980L1_25_LG_dsi_2_vdo_qhd_lcm_drv = 
{
	.name		= "nt35517_xingliangda_XLD0500980L1_25_LG_dsi_2_vdo_qhd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id    = lcm_compare_id,
#if (LCM_DSI_CMD_MODE)
	.update         = lcm_update,
#endif
#if defined (BIRD_ESD_CHECK)//rm esd resolve LCD  flick
	//.esd_check = lcm_esd_check,
  //.esd_recover = lcm_esd_recover,
#endif

};

