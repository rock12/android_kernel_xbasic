
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
 
#define LCM_DSI_CMD_MODE                                   0
#define LCM_ID                                      0x8009
#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN 

static unsigned int lcm_esd_test = 0;      ///only for ESD test
//#define BIRD_ESD_CHECK

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
{0x00,1,{0x00}},
{0xFF,3,{0x80,0x09,0x01}},

{0x00,1,{0x80}},
{0xFF,2,{0x80,0x09}},

{0x00,1,{0x03}},
{0xFF,1,{0x01}},

{0x00,1,{0x8B}},
{0xB0,1,{0x40}},

{0x00,1,{0xC6}},
{0xB0,1,{0x03}},

{0x00,1,{0xA6}},
{0xB3,2,{0x20,0x01}},

{0x00,1,{0x91}},
{0xB3,1,{0x00}}, // 0xC0 为旋转 180 度. 0x00 为正常方向.   00

{0x00,1,{0x90}},
{0xC0,6,{0x00,0x44,0x00,0x00,0x00,0x03}},

{0x00,1,{0xA3}},
{0xC0,1,{0x00}},

{0x00,1,{0xB4}},
{0xC0,1,{0x50}},

{0x00,1,{0x81}},
{0xC1,1,{0x66}},

{0x00,1,{0xA0}},
{0xC1,1,{0xEA}},

{0x00,1,{0xA6}},
{0xC1,3,{0x01,0x00,0x00}},

{0x00,1,{0x81}},
{0xC4,1,{0x81}},

{0x00,1,{0x89}},
{0xC4,1,{0x08}},

{0x00,1,{0x90}},
{0xC5,7,{0x96,0x3C,0x01,0x03,0x33,0x34,0x23}},

{0x00,1,{0x82}},
{0xC5,1,{0x83}},

{0x00,1,{0xB1}},
{0xC5,1,{0xA9}},

{0x00,1,{0xC0}},
{0xC5,1,{0x00}},

{0x00,1,{0xB2}},
{0xF5,4,{0x15,0x00,0x15,0x00}},

{0x00,1,{0x80}},
{0xCE,12,{0x86,0x01,0x00,0x85,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00,1,{0xA0}},
{0xCE,14,{0x18,0x05,0x03,0x39,0x00,0x00,0x00,0x18,0x04,0x03,0x3A,0x00,0x00,0x00}},
{0x00,1,{0xB0}},
{0xCE,14,{0x18,0x03,0x03,0x3B,0x00,0x00,0x00,0x18,0x02,0x03,0x3C,0x00,0x00,0x00}},

{0x00,1,{0xC0}},
{0xCF,10,{0x01,0x01,0x20,0x20,0x00,0x00,0x01,0x00,0x00,0x00}},

{0x00,1,{0xC0}},
{0xCB,15,{0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00,1,{0xD0}},
{0xCB,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00}},
{0x00,1,{0xE0}},
{0xCB,10,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

{0x00,1,{0x80}},
{0xCC,10,{0x00,0x26,0x09,0x0B,0x01,0x25,0x00,0x00,0x00,0x00}},
{0x00,1,{0x90}},
{0xCC,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0x0A,0x0C,0x02}},
{0x00,1,{0xA0}},
{0xCC,15,{0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00,1,{0xB0}},
{0xCC,10,{0x00,0x25,0x0C,0x0A,0x02,0x26,0x00,0x00,0x00,0x00}},
{0x00,1,{0xC0}},
{0xCC,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x25,0x0B,0x09,0x01}},
{0x00,1,{0xD0}},
{0xCC,15,{0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

{0x00,1,{0x00}},
{0xD8,2,{0x6F,0x6F}},

{0x00,1,{0x00}},
{0xD9,1,{0x52}},//42

{0x00,1,{0x00}},
{0xE1,16,{0x00,0x09,0x0F,0x0E,0x07,0x10,0x0B,0x0A,0x04,0x07,0x0B,0x08,0x0F,0x10,0x0A,0x01}},

{0x00,1,{0x00}},
{0xE2,16,{0x00,0x09,0x0F,0x0E,0x07,0x10,0x0B,0x0A,0x04,0x07,0x0B,0x08,0x0F,0x10,0x0A,0x01}},

/////////////////////////////////////////////////
{0x00,1,{0xA0}},
{0xC1,1,{0xEA}},

{0x00,1,{0xA6}},
{0xC1,3,{0x01,0x00,0x00}},

{0x00,1,{0xC6}},
{0xB0,1,{0x03}},

{0x00,1,{0x00}},
{0xFF,3,{0xFF,0xFF,0xFF}},
#else
{0x00,1,{0x00}},
{0xFF,3,{0x80,0x09,0x01}},
      
{0x00,1,{0x80}},
{0xFF,2,{0x80,0x09}},

/////////////////////////////////
{0x00,1,{0x80}}, 
{0xC4,1,{0x30}}, //放电代码
{REGFLAG_DELAY, 10, {}},
{0x00,1,{0x8A}}, 
{0xC4,1,{0x40}}, //放电代码
{REGFLAG_DELAY, 10, {}},
/////////////////////////////////
{0x00,1,{0xB4}},
{0xC0,1,{0x50}},

{0x00,1,{0x90}},
{0xC0,6,{0x00,0x44,0x00,0x00,0x00,0x03}},

{0x00,1,{0xA6}},
{0xC1,2,{0x01,0x00}},

{0x00,1,{0xA8}},
{0xC0,1,{0x00}},

{0x00,1,{0x8B}},
{0xB0,1,{0x40}},

{0x00,1,{0xA0}},
{0xC1,1,{0xEA}},

{0x00,1,{0x82}},
{0xB2,1,{0x20}},

{0x00,1,{0x82}},
{0xC5,1,{0xA3}},

{0x00,1,{0x90}},
{0xC5,2,{0x96,0x96}},

{0x00,1,{0x00}},
{0xD8,2,{0x57,0x57}},

{0x00,1,{0x00}},
{0xD9,1,{0x61}},

{0x00,1,{0x81}},
{0xC1,1,{0x66}},

{0x00,1,{0xA1}},
{0xC1,1,{0x08}},

{0x00,1,{0x89}},
{0xC4,1,{0x08}},

{0x00,1,{0xA3}},
{0xC0,1,{0x00}},

{0x00,1,{0x81}},
{0xC4,1,{0x83}},

{0x00,1,{0x92}},
{0xC5,1,{0x01}},

{0x00,1,{0xB1}},
{0xC5,1,{0xA9}},

{0x00,1,{0xC7}},
{0xCF,1,{0x01}},

{0x00,1,{0x90}},
{0xB3,1,{0x00}},

{0x00,1,{0x92}},
{0xB3,1,{0x05}},

{0x00,1,{0x80}},
{0xC0,9,{0x00,0x5F,0x00,0x10,0x10,0x00,0x5F,0x10,0x10}},

{0x00,1,{0x90}},
{0xC0,6,{0x00,0x44,0x00,0x00,0x00,0x03}},

{0x00,1,{0xA6}},
{0xC1,3,{0x01,0x00,0x00}},

{0x00,1,{0x80}},
{0xCE,12,{0x87,0x03,0x00,0x86,0x03,0x00,0x85,0x03,0x00,0x84,0x03,0x00}},

{0x00,1,{0x90}},
{0xCE,14,{0x33,0x1C,0x00,0x33,0x1D,0x00,0x33,0x1E,0x00,0x33,0x1F,0x00,0x00,0x00}},

{0x00,1,{0xA0}},
{0xCE,14,{0x38,0x05,0x03,0x20,0x00,0x78,0x00,0x38,0x04,0x03,0x21,0x00,0x78,0x00}},

{0x00,1,{0xB0}},
{0xCE,14,{0x38,0x03,0x03,0x22,0x00,0x78,0x00,0x38,0x02,0x03,0x23,0x00,0x78,0x00}},

{0x00,1,{0xC0}},
{0xCE,14,{0x38,0x01,0x03,0x24,0x00,0x78,0x00,0x38,0x00,0x03,0x25,0x00,0x78,0x00}},

{0x00,1,{0xD0}},
{0xCE,14,{0x30,0x00,0x03,0x26,0x00,0x78,0x00,0x30,0x01,0x03,0x27,0x00,0x78,0x00}},

{0x00,1,{0xC3}},
{0xCB,8,{0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x54}},

{0x00,1,{0xD8}},
{0xCB,7,{0x54,0x54,0x54,0x54,0x54,0x54,0x54}},

{0x00,1,{0xE0}},
{0xCB,1,{0x54}},

{0x00,1,{0x83}},
{0xCC,7,{0x03,0x01,0x09,0x0B,0x0D,0x0F,0x05}},

{0x00,1,{0x90}},
{0xCC,1,{0x07}},

{0x00,1,{0x9D}},
{0xCC,2,{0x04,0x02}},

{0x00,1,{0xA0}},
{0xCC,6,{0x0A,0x0C,0x0E,0x10,0x06,0x08}},

{0x00,1,{0xB3}},
{0xCC,7,{0x06,0x08,0x0C,0x0A,0x10,0x0E,0x04}},

{0x00,1,{0xC0}},
{0xCC,1,{0x02}},

{0x00,1,{0xCD}},
{0xCC,1,{0x05,0x07}},

{0x00,1,{0xD0}},
{0xCC,6,{0x0B,0x09,0x0F,0x0D,0x03,0x01}},

{0x00,1,{0x00}},
{0xE1,16,{0x08,0x0A,0x11,0x0E,0x09,0x1F,0x0F,0x0F,0x00,0x04,0x01,0x05,0x0D,0x1B,0x18,0x12}},

{0x00,1,{0x00}},
{0xE2,16,{0x08,0x0A,0x11,0x0E,0x09,0x1F,0x0F,0x0F,0x00,0x04,0x02,0x05,0x0D,0x1C,0x19,0x12}},

/////////////////////////////////////////////////
{0x00,1,{0xA0}},
{0xC1,1,{0xEA}},

{0x00,1,{0xA6}},
{0xC1,2,{0x01,0x00,0x00}},

{0x00,1,{0xC6}},
{0xB0,1,{0x03}},

{0x00,1,{0x81}}, 
{0xC5,1,{0x66}}, //Auto pump 2x

{0x00,1,{0xB6}}, 
{0xF5,1,{0x06}}, //VGL_REG timing setting.

{0x00,1,{0x8B}}, 
{0xB0,1,{0x40}}, //ULPM
/////////////////////////////////////////////////

{0x00,1,{0x00}},
{0xFF,3,{0xFF,0xFF,0xFF}},
#endif

{0x3A,1,{0x77}}, 

{0x11, 1 ,{0x00}}, 
{REGFLAG_DELAY, 150, {}},

{0x29, 1 ,{0x00}}, 
{REGFLAG_DELAY, 200, {}},

{0x2C, 1 ,{0x00}}, 
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
 
static struct LCM_setting_table lcm_set_window[] = {
   {0x2A,  4,  {0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
   {0x2B,  4,  {0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
   {REGFLAG_END_OF_TABLE, 0x00, {}}
};
 
 
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
 
 
static struct LCM_setting_table lcm_backlight_level_setting[] = {
   {0x51, 1, {0xFF}},
   {REGFLAG_END_OF_TABLE, 0x00, {}}
};
 
 
static struct LCM_setting_table lcm_compare_id_setting[] = {
   {0x00,  1,  {0x50}},
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
 
      
        params->dsi.vertical_sync_active = 4;//--4
        params->dsi.vertical_backporch = 20;//--8
        params->dsi.vertical_frontporch = 16;//--8
        params->dsi.vertical_active_line = FRAME_HEIGHT; 
    /*
        params->dsi.horizontal_sync_active = 6;//---6
        params->dsi.horizontal_backporch =70;//--30
        params->dsi.horizontal_frontporch =70;//--30
    */  
      // 20120618 modify. for ID 01,8b,80,09
        params->dsi.horizontal_sync_active = 8;//---6
        params->dsi.horizontal_backporch =50;//--37
        params->dsi.horizontal_frontporch =60;//--37
    
        /*  // 20120618 modify. for ID 02,8b,80,09
        params->dsi.horizontal_sync_active = 6;//---6
        params->dsi.horizontal_backporch =23;//--30
        params->dsi.horizontal_frontporch =24;//--30
         */
        params->dsi.horizontal_active_pixel = FRAME_WIDTH;
 
            // Bit rate calculation
    params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4
    params->dsi.pll_div2=0;		// div2=0,1,2,3;div2_real=1,2,4,4    0
    params->dsi.fbk_div =16;//15;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)   18
 
      /* ESD or noise interference recovery For video mode LCM only. */ // Send TE packet to LCM in a period of n frames and check the response.
           params->dsi.lcm_int_te_monitor = 0;// FALSE;
           params->dsi.lcm_int_te_period = 1; // Unit : frames
 
           // Need longer FP for more opportunity to do int. TE monitor applicably.
           if(params->dsi.lcm_int_te_monitor)
           params->dsi.vertical_frontporch *= 2;
 
           // Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.)
           #ifdef BIRD_ESD_CHECK
          
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
 
    int   array[4];
   char  buffer[5];
   char  id_high=0;
   char  id_low=0;
   int   id=0;

#ifdef BUILD_LK
		  pmic_config_interface(0x0532,5,0x7,5);//add by libo for VGP2 POWER ON
		  pmic_config_interface(0x050C,1,0x1,15);
#else
		  hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
#endif
    MDELAY(100);

   
   SET_RESET_PIN(1);  //NOTE:should reset LCM firstly
   SET_RESET_PIN(0);
   MDELAY(10);
   SET_RESET_PIN(1);
   MDELAY(10);
 
   array[0] = 0x00053700;// read id return two byte,version and id
   dsi_set_cmdq(array, 1, 1);
   read_reg_v2(0xA1,buffer, 5);
 
   
   id_high = buffer[2];            //should be 0x80
   id_low  = buffer[3];            //should be 0x18
   id      = (id_high<<8)|id_low;  //should be 0x8018
 
   
 

	 Lcd_Log("%s, mycat otm8018b_suxian_S4013200A_BOE_dsi_2_vdo_wvga id = 0x%08x\n", __func__, id); 
 
//   return (LCM_ID == id&&get_lcd_id()==1)?1:0;
   return (LCM_ID == id)?1:0;
}
 
 
static void lcm_init(void)
{
	  int flag=0;
	  
#ifdef BUILD_LK
		  pmic_config_interface(0x0532,5,0x7,5);//add by libo for VGP2 POWER ON
		  pmic_config_interface(0x050C,1,0x1,15);
#else
		  hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM");
#endif
		  MDELAY(100);

    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(1);
    SET_RESET_PIN(1);
    MDELAY(10);
  
   Lcd_Log("mycat otm8018b_suxian_S4013200A_BOE_dsi_2_vdo_wvga flag=%d  \n",flag);

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

//lcm_initialization_setting1[69].para_list[0]+=1;
//#ifndef BUILD_LK
//printk("mycatx vcom=0x%x\n",lcm_initialization_setting1[69].para_list[0]);
//#endif
   lcm_init();
 
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
 
static void lcm_setbacklight(unsigned int level)
{
   unsigned int default_level = 145;
   unsigned int mapped_level = 0;
 
   //for LGE backlight IC mapping table
   if(level > 255) 
           level = 255;
 
   if(level >0) 
           mapped_level = default_level+(level)*(255-default_level)/(255);
   else
           mapped_level=0;
 
   // Refresh value of backlight level.
   lcm_backlight_level_setting[0].para_list[0] = mapped_level;
 
   push_table(lcm_backlight_level_setting, sizeof(lcm_backlight_level_setting) / sizeof(struct LCM_setting_table), 1);
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
	//MDELAY(20);
	read_reg_v2(0x0A, buffer, 1);

	printk("mycat$$ otm8018b_suxian_S4013200A_BOE_dsi_2_vdo_wvga ### esd check buffer[1] = 0x%x\r\n",buffer[0]);
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

LCM_DRIVER otm8018b_apex_A40305N01VO_IVO_dsi_2_vdo_wvga_lcm_drv = 
{
    .name			= "otm8018b_apex_A40305N01VO_IVO_dsi_2_vdo_wvga",
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
