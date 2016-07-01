#include <cust_leds.h>
#include <mach/mt_pwm.h>
#include <mach/mt_gpio.h>

#include <linux/kernel.h>
#include <linux/delay.h>
#include <asm/delay.h>
#include <mach/pmic_mt6329_hw_bank1.h> 
#include <mach/pmic_mt6329_sw_bank1.h> 
#include <mach/pmic_mt6329_hw.h>
#include <mach/pmic_mt6329_sw.h>
#include <mach/upmu_common_sw.h>
#include <mach/upmu_hw.h>
#if defined(BIRD_CHR_BOOT_BK_CTL)
#include <mach/mt_boot.h>
#endif

unsigned int brightness_mapping(unsigned int level)
{
	unsigned int v_level = level;
	
	if(level>255)
		v_level = 255;
        if(level<=0)
		v_level = 0;
	//v_level = v_level/6+22;
	
#if defined(BIRD_LCD_BK_IC_COMPATIBLE)
	mt_set_gpio_mode(GPIO_CHG_ID_PIN, GPIO_PWM_2_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CHG_ID_PIN, GPIO_DIR_IN);
	if(mt_get_gpio_in(GPIO_CHG_ID_PIN))
	{
			if(level<40)
				v_level = (unsigned int)43*v_level/255;
			else if (level<60)
			  v_level = (unsigned int)43*v_level/255+10;
			else	
				v_level = (unsigned int)43*v_level/255 + 20;
	}
	else
	{
			v_level = (unsigned int)33*v_level/255 + 30;
	}
#else
  v_level = (unsigned int)43*v_level/255 + 20;
#endif
    
    printk("MYCAT high light level:v_level:[%d,%d]\n",level,v_level);
    return v_level;
}


static int custom_set_pwm(int pwm_num, u32 level, u32 div)//copy from leds.c
{
	//S32 result;
	struct pwm_spec_config pwm_setting;
	pwm_setting.pwm_no = pwm_num;
	pwm_setting.mode = PWM_MODE_FIFO; //new mode fifo and periodical mode
	pwm_setting.clk_div = div;
	pwm_setting.clk_src = PWM_CLK_NEW_MODE_BLOCK;//PWM_CLK_NEW_MODE_BLOCK;
	pwm_setting.pmic_pad = 0;
	
	pwm_setting.PWM_MODE_FIFO_REGS.IDLE_VALUE = 0;
	pwm_setting.PWM_MODE_FIFO_REGS.GUARD_VALUE = 0;
	pwm_setting.PWM_MODE_FIFO_REGS.STOP_BITPOS_VALUE = 63;
	//pwm_setting.PWM_MODE_FIFO_REGS.GDURATION = (pwm_setting.PWM_MODE_FIFO_REGS.HDURATION+1)*32 - 1;
	pwm_setting.PWM_MODE_FIFO_REGS.HDURATION = 40;
	pwm_setting.PWM_MODE_FIFO_REGS.LDURATION = 40;
	pwm_setting.PWM_MODE_FIFO_REGS.GDURATION = 0;
	pwm_setting.PWM_MODE_FIFO_REGS.WAVE_NUM = 0;
	//printk("MYCAT level:div= %d,%d\n", level,div);
  if(level <= 32)
	{
		pwm_setting.PWM_MODE_FIFO_REGS.SEND_DATA0 =  (1 << level) - 1 ;
		pwm_setting.PWM_MODE_FIFO_REGS.SEND_DATA1 = 0 ;
	}else if(level>32 && level <=64)
	{
		level -= 32;
		pwm_setting.PWM_MODE_FIFO_REGS.SEND_DATA0 =  0xFFFFFFFF ;
		pwm_setting.PWM_MODE_FIFO_REGS.SEND_DATA1 = (1 << level) - 1;
	}

	pwm_set_spec_config(&pwm_setting);
	//printk("MYCAT Result=%d\n",result);
	//printk("[LED]PWM con register is %x \n", INREG32(PWM_BASE + 0x0150));
	return 0;

}


static int BacklightLevel = 0;
int Cust_SetBacklight(int level, int div)
{
	if (level)
	{
#if 1	
		mt_set_gpio_mode(GPIO_PWM_2_PIN, GPIO_PWM_2_PIN_M_PWM);
		mt_set_gpio_dir(GPIO_PWM_2_PIN, GPIO_DIR_OUT);

		if(BacklightLevel == 0) 
			mdelay(200);
		level = brightness_mapping(level);

#if defined(BIRD_CHR_BOOT_BK_CTL)
		if(get_boot_mode() == KERNEL_POWER_OFF_CHARGING_BOOT)
			custom_set_pwm(PWM2, 30, div); //30
		else
			custom_set_pwm(PWM2, level, div);
#else
		custom_set_pwm(PWM2, level, div);
#endif
			
		if(BacklightLevel == 0) 
			mdelay(20);
#else
		//hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"leds");

		mt_set_gpio_mode(GPIO_PWM_2_PIN, GPIO_PWM_2_PIN_M_GPIO);
		mt_set_gpio_dir(GPIO_PWM_2_PIN, GPIO_DIR_OUT);
		mt_set_gpio_out(GPIO_PWM_2_PIN,GPIO_OUT_ONE);
#endif

		BacklightLevel = 1;
	}
	else
	{
		mt_set_gpio_mode(GPIO_PWM_2_PIN, GPIO_PWM_2_PIN_M_GPIO);
		mt_set_gpio_dir(GPIO_PWM_2_PIN, GPIO_DIR_OUT);
		mt_set_gpio_out(GPIO_PWM_2_PIN,GPIO_OUT_ZERO);

		//hwPowerDown(MT6323_POWER_LDO_VGP2);
	
		BacklightLevel = 0; 
	}
}

static struct cust_mt65xx_led cust_led_list[MT65XX_LED_TYPE_TOTAL] = {
	{"red",               MT65XX_LED_MODE_NONE, -1,{0}},
	{"green",             MT65XX_LED_MODE_NONE, -1,{0}},
	{"blue",              MT65XX_LED_MODE_NONE, -1,{0}},
	{"jogball-backlight", MT65XX_LED_MODE_NONE, -1,{0}},
	{"keyboard-backlight",MT65XX_LED_MODE_NONE, -1,{0}},
	{"button-backlight",  MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_NLED_ISINK0, {0}},
	{"lcd-backlight",     MT65XX_LED_MODE_CUST_LCM, (int)Cust_SetBacklight,{0}},
};

struct cust_mt65xx_led *get_cust_led_list(void)
{
	return cust_led_list;
}

