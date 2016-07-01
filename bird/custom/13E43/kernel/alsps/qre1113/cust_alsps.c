/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */


#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>
#include <linux/kernel.h>
static struct alsps_hw cust_alsps_hw = {
    .i2c_num    = 0,
	.polling_mode =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    .adc_channel = 0,
    .mros_en = 135,
    .mros_gpio = -1,    //set -1 if not used
    .ps_threshold = 0XFFFF,
};

struct alsps_hw *get_cust_alsps_hw(void) {
#ifdef BIRD_PS_THD_VAL
    cust_alsps_hw.ps_threshold = simple_strtol(BIRD_PS_THD_VAL, NULL, 0);
#endif
    return &cust_alsps_hw;
}
