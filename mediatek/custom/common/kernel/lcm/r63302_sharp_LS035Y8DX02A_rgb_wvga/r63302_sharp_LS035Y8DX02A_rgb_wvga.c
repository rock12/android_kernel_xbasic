#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#include <debug.h>
#else
#include <linux/string.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#include <mach/pmic_mt6323_sw.h>
#include <mach/upmu_common.h>
#include <mach/upmu_hw.h>
#endif
#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define LSA0_GPIO_PIN (GPIO_DISP_LSA0_PIN)
#define LSCE_GPIO_PIN (GPIO_DISP_LSCE_PIN)
#define LSCK_GPIO_PIN (GPIO_DISP_LSCK_PIN)
#define LSDA_GPIO_PIN (GPIO_DISP_LSDA_PIN)

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (800)

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))
#define SET_GPIO_OUT(n, v)  (lcm_util.set_gpio_out((n), (v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define SET_LSCE_LOW   SET_GPIO_OUT(LSCE_GPIO_PIN, 0)
#define SET_LSCE_HIGH  SET_GPIO_OUT(LSCE_GPIO_PIN, 1)
#define SET_LSCK_LOW   SET_GPIO_OUT(LSCK_GPIO_PIN, 0)
#define SET_LSCK_HIGH  SET_GPIO_OUT(LSCK_GPIO_PIN, 1)
#define SET_LSDA_LOW   SET_GPIO_OUT(LSDA_GPIO_PIN, 0)
#define SET_LSDA_HIGH  SET_GPIO_OUT(LSDA_GPIO_PIN, 1)

static __inline void spi_send_data(unsigned int data)
{
    unsigned int i;

    for (i = 0; i < 8; ++ i)
    {
        SET_LSCK_LOW;
        if (data & 0x80) {
            SET_LSDA_HIGH;
        } else {
            SET_LSDA_LOW;
        }
        UDELAY(1);
        SET_LSCK_HIGH;
        UDELAY(1);
        data <<= 1;
    }
}

static __inline void send_ctrl_cmd(unsigned int cmd)
{
    SET_LSCE_HIGH;
    SET_LSDA_HIGH;
    SET_LSCK_HIGH;

    SET_LSCE_LOW;
    UDELAY(1);

    SET_LSCK_LOW;
    SET_LSDA_LOW;
    UDELAY(1);

    SET_LSCK_HIGH;
    UDELAY(1);

    spi_send_data(cmd);

    SET_LSCK_HIGH;
    SET_LSDA_HIGH;
    SET_LSCE_HIGH;
    UDELAY(1);
}

static __inline void send_data_cmd(unsigned int data)
{
    SET_LSCE_HIGH;
    SET_LSDA_HIGH;
    SET_LSCK_HIGH;

    SET_LSCE_LOW;
    UDELAY(1);

    SET_LSCK_LOW;
    SET_LSDA_HIGH;
    UDELAY(1);

    SET_LSCK_HIGH;
    UDELAY(1);

    spi_send_data(data);

    SET_LSCK_HIGH;
    SET_LSDA_HIGH;
    SET_LSCE_HIGH;
    UDELAY(1);
}

static void init_lcm_registers(void)
{
    send_ctrl_cmd(0x29);
    send_ctrl_cmd(0x11);
    MDELAY(120);
    send_ctrl_cmd(0x36);
    send_data_cmd(0x00); // 0x1010     
    MDELAY(4);
    send_ctrl_cmd(0x3A);    
    send_data_cmd(0x70); // 0x1C1E 123e
    MDELAY(4);
    send_ctrl_cmd(0xB0);    
    send_data_cmd(0x00); // 0x0C0E    
    MDELAY(4);
    send_ctrl_cmd(0xB8);    
    send_data_cmd(0x00); // 0x0280
    //send_data_cmd(0x01); // 0x0280
    MDELAY(4);
    send_ctrl_cmd(0xB9);    
    send_data_cmd(0x01); // 0x03C0     
    send_data_cmd(0xFF); // 0x03C0     
    MDELAY(4);
    send_ctrl_cmd(0xB0);    
    send_data_cmd(0x03); // 0x0C0E
    MDELAY(4);
}


static void config_gpio(void)
{
    const unsigned int USED_GPIOS[] = 
    {
        LSCE_GPIO_PIN,
        LSCK_GPIO_PIN,
        LSDA_GPIO_PIN
    };

    unsigned int i;

    lcm_util.set_gpio_mode(LSA0_GPIO_PIN, GPIO_DISP_LSA0_PIN_M_GPIO);
    lcm_util.set_gpio_mode(LSCE_GPIO_PIN, GPIO_DISP_LSCE_PIN_M_GPIO);
    lcm_util.set_gpio_mode(LSCK_GPIO_PIN, GPIO_DISP_LSCK_PIN_M_GPIO);
    lcm_util.set_gpio_mode(LSDA_GPIO_PIN, GPIO_DISP_LSDA_PIN_M_GPIO);

    for (i = 0; i < ARY_SIZE(USED_GPIOS); ++ i)
    {
        lcm_util.set_gpio_dir(USED_GPIOS[i], 1);               // GPIO out
        lcm_util.set_gpio_pull_enable(USED_GPIOS[i], 0);
    }

    // Swithc LSA0 pin to GPIO mode to avoid data contention,
    // since A0 is connected to LCM's SPI SDO pin
    //
    lcm_util.set_gpio_dir(LSA0_GPIO_PIN, 0);                   // GPIO in
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

    params->type   = LCM_TYPE_DPI;
    params->ctrl   = LCM_CTRL_GPIO;
    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;
    
    params->dpi.mipi_pll_clk_ref  = 0;
    params->dpi.mipi_pll_clk_div1 = 33;
    params->dpi.mipi_pll_clk_div2 = 9;
    params->dpi.dpi_clk_div       = 2;
    params->dpi.dpi_clk_duty      = 1;

    params->dpi.clk_pol           = LCM_POLARITY_FALLING;
    params->dpi.de_pol            = LCM_POLARITY_RISING;
    params->dpi.vsync_pol         = LCM_POLARITY_FALLING;
    params->dpi.hsync_pol         = LCM_POLARITY_FALLING;
    params->dpi.hsync_pulse_width = 16;//16
    params->dpi.hsync_back_porch  = 24;//24
    params->dpi.hsync_front_porch = 16;//16
    params->dpi.vsync_pulse_width = 2;//2
    params->dpi.vsync_back_porch  = 2;//2
    params->dpi.vsync_front_porch = 4;//4
    
    params->dpi.format            = LCM_DPI_FORMAT_RGB888;
    params->dpi.rgb_order         = LCM_COLOR_ORDER_RGB;
    params->dpi.is_serial_output  = 0;

    params->dpi.intermediat_buffer_num = 2;

    params->dpi.io_driving_current = LCM_DRIVING_CURRENT_4MA; //8

    params->dpi.i2x_en = 0;
    params->dpi.i2x_edge = 1;
}


static void lcm_init(void)
{
#ifdef BUILD_LK
    upmu_set_rg_vgp2_vosel(5);
    upmu_set_rg_vgp2_en(1);
    MDELAY(50);
		
    upmu_set_rg_vgp3_vosel(3);
    upmu_set_rg_vgp3_en(1); 
#else
    hwPowerOn(MT6323_POWER_LDO_VGP2,VOL_2800,"LCM28");
    hwPowerOn(MT6323_POWER_LDO_VGP3,VOL_1800,"LCM18");
#endif
    MDELAY(100);

    config_gpio();
    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(50);

    init_lcm_registers();
}


static void lcm_suspend(void)
{
    send_ctrl_cmd(0x10);
    MDELAY(120);
    send_ctrl_cmd(0x28);
}


static void lcm_resume(void)
{
    //lcm_init();
    send_ctrl_cmd(0x29);
    send_ctrl_cmd(0x11);
    MDELAY(120);
}


// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER r63302_sharp_LS035Y8DX02A_rgb_wvga_lcm_drv = 
{
   .name = "r63302_sharp_LS035Y8DX02A_rgb_wvga",
   .set_util_funcs = lcm_set_util_funcs,
   .get_params     = lcm_get_params,
   .init           = lcm_init,
   .suspend        = lcm_suspend,
   .resume         = lcm_resume,
};

