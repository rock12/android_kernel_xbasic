/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/* 
 * 
 * Author: MingHsien Hsieh <minghsien.hsieh@mediatek.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/earlysuspend.h>
#include <linux/platform_device.h>
#include <asm/atomic.h>
#include <linux/kthread.h>
#include <linux/wait.h>

//#include <mach/mt_devs.c>
//#include <mach/mt_typedefs.h>
//#include <mach/mt_gpio.h>
////#include <mach/mt6575_pll.h>

#include <mach/devs.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_gpio.h>
//#include <mach/mt_pll.h>

#include <linux/hwmsensor.h>
#include <linux/hwmsen_dev.h>
#include <linux/sensors_io.h>
#include <linux/hwmsen_helper.h>
#include <asm/io.h>
#include <cust_eint.h>
#include <cust_alsps.h>
/******************************************************************************
 * configuration
*******************************************************************************/
/*----------------------------------------------------------------------------*/

#define SANLED_marquee_DEV_NAME     "SANLED"
#define SANLED_marquee_I2C_WRITE_ADDR     0xb6
struct timer_list SANLED_Timer;
#define SANLED_TIMER_COUNT    (1*HZ)

static int count = 0;int count_1 = 0;int count_2 = 0;int count_3 = 0;int count_4 = 0;int count_5 = 0;int count_6 = 0;int count_7 = 0;
static struct task_struct *thread = NULL;
wait_queue_head_t SANLED_waitMain;
int SANLED_Start_thread = 0;
static unsigned char G_SANLED_Ledctrl1=0,G_SANLED_Ledctrl2=0,G_SANLED_LedEffect = 0,G_SANLED_CFG=0;

int Mode1 = 0;

/*----------------------------------------------------------------------------*/
static struct i2c_client *SANLED_marquee_i2c_client = NULL;
/*----------------------------------------------------------------------------*/
static const struct i2c_device_id SANLED_marquee_i2c_id[] = {{SANLED_marquee_DEV_NAME,0},{}};
//static unsigned short SANLED_force[] = {0x00, 0xE8, I2C_CLIENT_END, I2C_CLIENT_END};
//static const unsigned short *const SANLED_forces[] = { SANLED_force, NULL };
////static struct i2c_client_address_data SANLED_marquee_addr_data = { .forces = SANLED_forces,};

static struct i2c_board_info __initdata i2c_SANLED={ I2C_BOARD_INFO("SANLED", (SANLED_marquee_I2C_WRITE_ADDR>>1))};

/*----------------------------------------------------------------------------*/
static struct platform_driver SANLED_marquee_driver;

#define marquee							0x99
#define MODEL1_OPEN_NOTICE		         _IO(marquee, 1)
#define MODEL2_OPEN_CHARGE	             _IO(marquee, 2)
#define MODEL3_OPEN_MUSIC				_IO(marquee,3)
#define MODEL4_OPEN_ALARM	             _IO(marquee, 4)
#define MODEL5_OPEN_LESSBATTERY          _IO(marquee, 5)
#define MODEL6_OPEN_UNREADSMS            _IO(marquee, 6)
#define MODEL7_OPEN_UNANSWERINCOMECAL	 _IO(marquee, 7)
#define MODEL8_OPEN_OUTCALL              _IO(marquee, 8)  ////no use
#define MODEL9_OPEN_COMINGCALL	         _IO(marquee, 9)
#define MODEL10_OPEN_SENDSMS	         _IO(marquee, 10)////no use
#define MODEL11_OPEN_COMINGSMS           _IO(marquee, 11)
#define MODEL12_OPEN_POWEROFF	         _IO(marquee, 12)////no use
#define MODEL13_OPEN_POWERON             _IO(marquee, 13)////no use
#define MODEL15_CLOSE_ALL             _IO(marquee, 14)
#define MODEL15_MUSIC_SPEED_HIGHT             _IO(marquee, 15)
#define MODEL16_MUSIC_SPEED_SLOW             _IO(marquee, 16)


const unsigned char PWMup_PowerOn[]=
{
	0,	0,	0,
16,	0,	0,
32,	0,	0,
48,	0,	0,
64,	0,	0,
80,	0,	0,
96,	0,	0,
112,	0,	0,
128,	0,	0,
144,	0,	0,
160,	0,	0,
176,	0,	0,
192,	0,	0,
208,	0,	0,
224,	0,	0,
240,	0,	0,
255,	0,	0,
240,	0,	0,
224,	0,	0,
208,	0,	0,
192,	0,	0,
176,	0,	0,
160,	0,	0,
144,	0,	0,
128,	0,	0,
112,	0,	0,
96,	0,	0,
80,	0,	0,
64,	0,	0,
48,	0,	0,
32,	0,	0,
16,	0,	0,
0,	0,	0,
0,	0,	0,
0,	16,	0,
0,	32,	0,
0,	48,	0,
0,	64,	0,
0,	80,	0,
0,	96,	0,
0,	112,	0,
0,	128,	0,
0,	144,	0,
0,	160,	0,
0,	176,	0,
0,	192,	0,
0,	208,	0,
0,	224,	0,
0,	240,	0,
0,	255,	0,
0,	240,	0,
0,	224,	0,
0,	208,	0,
0,	192,	0,
0,	176,	0,
0,	160,	0,
0,	144,	0,
0,	128,	0,
0,	112,	0,
0,	96,	0,
0,	80,	0,
0,	64,	0,
0,	48,	0,
0,	32,	0,
0,	16,	0,
0,	0,	0,
0,	0,	0,
0,	0,	16,
0,	0,	32,
0,	0,	48,
0,	0,	64,
0,	0,	80,
0,	0,	96,
0,	0,	112,
0,	0,	128,
0,	0,	144,
0,	0,	160,
0,	0,	176,
0,	0,	192,
0,	0,	208,
0,	0,	224,
0,	0,	240,
0,	0,	255,
0,	0,	240,
0,	0,	224,
0,	0,	208,
0,	0,	192,
0,	0,	176,
0,	0,	160,
0,	0,	144,
0,	0,	128,
0,	0,	112,
0,	0,	96,
0,	0,	80,
0,	0,	64,
0,	0,	48,
0,	0,	32,
0,	0,	16,
0,	0,	0,
0,	0,	0,
16,	0,	16,
32,	0,	32,
48,	0,	48,
64,	0,	64,
80,	0,	80,
96,	0,	96,
112,	0,	112,
128,	0,	128,
144,	0,	144,
160,	0,	160,
176,	0,	176,
192,	0,	192,
208,	0,	208,
224,	0,	224,
240,	0,	240,
255,	0,	255,
240,	0,	240,
224,	0,	224,
208,	0,	208,
192,	0,	192,
176,	0,	176,
160,	0,	160,
144,	0,	144,
128,	0,	128,
112,	0,	112,
96,	0,	96,
80,	0,	80,
64,	0,	64,
48,	0,	48,
32,	0,	32,
16,	0,	16,
0,	0,	0,
0,	0,	0,
0,	16,	16,
0,	32,	32,
0,	48,	48,
0,	64,	64,
0,	80,	80,
0,	96,	96,
0,	112,	112,
0,	128,	128,
0,	144,	144,
0,	160,	160,
0,	176,	176,
0,	192,	192,
0,	208,	208,
0,	224,	224,
0,	240,	240,
0,	255,	255,
0,	240,	240,
0,	224,	224,
0,	208,	208,
0,	192,	192,
0,	176,	176,
0,	160,	160,
0,	144,	144,
0,	128,	128,
0,	112,	112,
0,	96,	96,
0,	80,	80,
0,	64,	64,
0,	48,	48,
0,	32,	32,
0,	16,	16,
0,	0,	0,
0,	0,	0,
16,	16,	0,
32,	32,	0,
48,	48,	0,
64,	64,	0,
80,	80,	0,
96,	96,	0,
112,	112,	0,
128,	128,	0,
144,	144,	0,
160,	160,	0,
176,	176,	0,
192,	192,	0,
208,	208,	0,
224,	224,	0,
240,	240,	0,
255,	255,	0,
240,	240,	0,
224,	224,	0,
208,	208,	0,
192,	192,	0,
176,	176,	0,
160,	160,	0,
144,	144,	0,
128,	128,	0,
112,	112,	0,
96,	96,	0,
80,	80,	0,
64,	64,	0,
48,	48,	0,
32,	32,	0,
16,	16,	0,
0,	0,	0,
0,	0,	0,
16,	16,	16,
32,	32,	32,
48,	48,	48,
64,	64,	64,
80,	80,	80,
96,	96,	96,
112,	112,	112,
128,	128,	128,
144,	144,	144,
160,	160,	160,
176,	176,	176,
192,	192,	192,
208,	208,	208,
224,	224,	224,
240,	240,	240,
255,	255,	255,
240,	240,	240,
224,	224,	224,
208,	208,	208,
192,	192,	192,
176,	176,	176,
160,	160,	160,
144,	144,	144,
128,	128,	128,
112,	112,	112,
96,	96,	96,
80,	80,	80,
64,	64,	64,
48,	48,	48,
32,	32,	32,
16,	16,	16,
0,	0,	0,

};
const unsigned char PWMup_PowerOff[]=
{
	0,	0,	0,
16,	0,	0,
32,	0,	0,
48,	0,	0,
64,	0,	0,
80,	0,	0,
96,	0,	0,
112,	0,	0,
128,	0,	0,
144,	0,	0,
160,	0,	0,
176,	0,	0,
192,	0,	0,
208,	0,	0,
224,	0,	0,
240,	0,	0,
246,	0,	0,
250,	0,	0,
255,	0,	0,
0,	0,	0,
0,	0,	0,
0,	0,	0,
0,	0,	0,
192,	0,	0,
192,	0,	0,
192,	0,	0,
0,	0,	0,
0,	0,	0,
0,	0,	0,
0,	0,	0,
96,	0,	0,
96,	0,	0,
96,	0,	0,
0,	0,	0,
0,	0,	0,
0,	0,	0,
0,	0,	0,
16,	0,	0,
16,	0,	0,
16,	0,	0,
0,	0,	0,
0,	0,	0,
0,	0,	0,
0,	0,	0,


	};

const unsigned char PWMup_Charge[]=
{
	0,	0,	0,
16,	0,	0,
32,	0,	0,
48,	0,	0,
64,	0,	0,
80,	0,	0,
96,	0,	0,
112,	0,	0,
128,	0,	0,
144,	0,	0,
160,	0,	0,
176,	0,	0,
192,	0,	0,
200,	0,	0,
208,	0,	0,
216,	0,	0,
224,	0,	0,
232,	0,	0,
240,	0,	0,
246,	0,	0,
250,	0,	0,
255,	0,	0,
0,	0,	0,
0,	0,	0,
0,	0,	0,
192,	0,	0,
192,	0,	0,
192,	0,	0,
192,	0,	0,
0,	0,	0,
0,	0,	0,
0,	0,	0,
96,	0,	0,
96,	0,	0,
96,	0,	0,
96,	0,	0,
0,	0,	0,
0,	0,	0,
0,	0,	0,
48,	0,	0,
48,	0,	0,
48,	0,	0,
48,	0,	0,
0,	0,	0,
0,	0,	0,

};
const unsigned char PWMup_Music[]=
{
	0,	0,	0,
48,	0,	0,
48,	0,	0,
48,	0,	0,
48,	0,	0,
48,	0,	0,
48,	0,	0,
0,	0,	0,
156,	0,	0,
156,	0,	0,
156,	0,	0,
156,	0,	0,
156,	0,	0,
156,	0,	0,
0,	0,	0,
255,	0,	0,
255,	0,	0,
255,	0,	0,
255,	0,	0,
255,	0,	0,
255,	0,	0,
255,	0,	0,
255,	0,	0,
0,	0,	0,
	 
};

const unsigned char PWMup_ComingCall[]=
{
	48,	0,	0,
48,	0,	0,
48,	0,	0,
48,	0,	0,
150,	0,	0,
150,	0,	0,
150,	0,	0,
150,	0,	0,
255,	0,	0,
255,	0,	0,
255,	0,	0,
255,	0,	0,
255,	0,	0,
255,	0,	0,
	  

};

void SANLED_Chip_Pin_Off()
{
		
	if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x20,0x00))//OUT0口调光,调光等级为0-255。OUT0~OUT9的调光指令依次为0x20~0x29. 写0关闭
	{
	  return;
	}
	
	if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,0x00))//OUT0口调光,调光等级为0-255。OUT0~OUT9的调光指令依次为0x20~0x29. 写0关闭
	{
	  return;
	}
	
	if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,0x00))//OUT0口调光,调光等级为0-255。OUT0~OUT9的调光指令依次为0x20~0x29. 写0关闭
	{
	  return;
	}
}


void  Mode_Close_all()
{
		SANLED_Chip_Pin_Off();
}

static  void led_share_red_grean_blue() 
{ 
        static int led_red_grean_blue; 
        if(led_red_grean_blue/2==0) 
        { 
            if ((led_red_grean_blue % 2) == 0){ 

                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,0x3F))
								{
								  	return;
								}
	
            }else{ 
               //mdelay(100); 
               if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,0x00))
							 {
								  	return;
							 }
            } 
            
        } 
        else if(led_red_grean_blue/2==1) 
        { 
             if ((led_red_grean_blue % 2) == 0){ 

                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,0x3F))
								{
								  	return;
								}
               //mdelay(100); 
               } 
               else{ 
                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,0x00))
							 	{
								  	return;
							 	}
               } 
        } 
        else if(led_red_grean_blue/2==2) 
        { 
            if ((led_red_grean_blue % 2) == 0){ 

                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x20,0x3F))
								{
								  	return;
								}
             }else{ 
          // mdelay(100); 
                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x20,0x00))
							 	{
								  	return;
							 	}
             } 
        } 

             if (led_red_grean_blue >= 5) 
                led_red_grean_blue=0; 
             else 
                 led_red_grean_blue+=1; 
        
} 

static  void led_share_red_red()
{
	static int led_red_red;
	if(led_red_red/2==0) 
        { 
            if ((led_red_red % 2) == 0){ 

                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,0x3F))
								{
								  	return;
								}
            }else{ 
           //    schedule_timeout(100); 
                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,0x00))
								{
								  	return;
								}
            } 
            
        } 
        else if(led_red_red/2==1) 
        { 
             if ((led_red_red % 2) == 0){ 

                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,0x3F))
								{
								  	return;
								}
           //    schedule_timeout(100); 
               } 
               else{ 
                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,0x00))
								{
								  	return;
								}
                } 
        } 
         
             if (led_red_red >= 3) 
                led_red_red=0; 
             else 
                 led_red_red+=1; 

}

static  void led_share_grean_grean()
{
	static int led_grean_grean;
	if(led_grean_grean/2==0) 
        { 
            if ((led_grean_grean % 2) == 0){ 

                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,0x3F))
								{
								  	return;
								}
            }else{ 
               //mdelay(100); 
                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,0x00))
								{
								  	return;
								}
            } 
            
        } 
        else if(led_grean_grean/2==1) 
        { 
             if ((led_grean_grean % 2) == 0){ 
               //mdelay(100); 
               if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,0x3F))
							 {
								  	return;
							 }
               } 
               else{ 
               if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,0x00))
							 {
								  	return;
							 }
                } 
        } 
         
             if (led_grean_grean >= 3) 
                led_grean_grean=0; 
             else 
                 led_grean_grean+=1; 


}

static  void led_red_grean_blue() 
{ 
        static int led_red_grean_blue; 
        if(led_red_grean_blue==0) 
        { 
               if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,0x3F))
							 {
								  	;
							 }
                 led_red_grean_blue+=1; 
        } 
        else if(led_red_grean_blue==1) 
        { 
                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,0x3F))
							  {
								  	;
							  }
                 led_red_grean_blue+=1; 
        } 
        else if(led_red_grean_blue==2) 
        { 
                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x20,0x3F))
							  {
								  	;
							  }
                led_red_grean_blue=0; 
        } 
} 
static  void led_share_grean()
{
                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,0x3F))
							  {
								  	;
							  }
}

static  void led_share_red()
{
                if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x20,0x3F))
							  {
								  	;
							  }
}

void Mode16_open_Timer_Music_slow(void)
{
		/*static int sizeof_on_Close_all;
		static int count;
		sizeof_on_Close_all=2;
		 SANLED_Chip_Pin_Off();
		count++;
		if(sizeof_on_Close_all<=count)
			{
			  Mode_Close_all();
			  count=0;
			  return;
			}
   		SANLED_Timer.data = (unsigned long)15;
		mod_timer(&SANLED_Timer,(jiffies + 10));*/
}

void Mode15_open_Timer_Music_high(void)
{
		/*static int sizeof_on_Close_all;
		static int count;
		sizeof_on_Close_all=2;
		 SANLED_Chip_Pin_Off();
		count++;
		if(sizeof_on_Close_all<=count)
			{
			  Mode_Close_all();
			  count=0;
			  return;
			}
   		SANLED_Timer.data = (unsigned long)15;
		mod_timer(&SANLED_Timer,(jiffies + 10));*/
}


void Mode14_open_Timer_Close_all()
{
		static int sizeof_on_Close_all;
		static int count;
		sizeof_on_Close_all=2;
		 SANLED_Chip_Pin_Off();
		count++;
		if(sizeof_on_Close_all<=count)
			{
			  Mode_Close_all();
			  count=0;
			  return;
			}
   		SANLED_Timer.data = (unsigned long)15;
		mod_timer(&SANLED_Timer,(jiffies + 10));
}

void Mode13_open_Timer_ON_PowerOn(void)
{
		static int sizeof_on_PowerOn;
		static int count;
		//sizeof_on_PowerOn=2;
		sizeof_on_PowerOn=(sizeof(PWMup_PowerOn))/sizeof(PWMup_PowerOn[0]);
		//Mode_Close_all();
		//led_share_red_grean_blue();	
		//count++;
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,PWMup_PowerOn[count++]))
			{
		  return;
		}
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,PWMup_PowerOn[count++]))
			{
		  return;
		}
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x20,PWMup_PowerOn[count++]))
			{
		  return;
		}
		if(sizeof_on_PowerOn<=count)
			{
				Mode_Close_all();
			  //led_red_grean_blue();
			  count=0;
			  return;
			}
		SANLED_Timer.data = (unsigned long)13;
		mod_timer(&SANLED_Timer,(jiffies + 20));
}
void Mode12_open_Timer_ON_PowerOff(void)
{
		static int sizeof_on_PowerOff;
		static int count;
		//sizeof_on_PowerOff=2;
		sizeof_on_PowerOff=(sizeof(PWMup_PowerOff))/sizeof(PWMup_PowerOff[0]);
		//Mode_Close_all();
		//led_share_grean_grean();	
		//count++;
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,PWMup_PowerOff[count++]))
			{
		  return;
		}
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,PWMup_PowerOff[count++]))
			{
		  return;
		}
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x20,PWMup_PowerOff[count++]))
			{
		  return;
		}
		if(sizeof_on_PowerOff<=count)
			{
				Mode_Close_all();
			  //led_share_grean();
			  count=0;
			  return;
			}
		SANLED_Timer.data = (unsigned long)12;
		mod_timer(&SANLED_Timer,(jiffies + 10));
}
void Mode11_open_Timer_ON_ComingSms(void)
{
		static int sizeof_on_ComingSms;
		static int count;
		sizeof_on_ComingSms=10;
		Mode_Close_all();
		//led_share_grean_grean();	
		count++;
		if(sizeof_on_ComingSms<=count)
			{
			  led_share_grean();
			  count=0;
			//  return;
			}
		SANLED_Timer.data = (unsigned long)11;
		mod_timer(&SANLED_Timer,(jiffies + 60));
}
void Mode10_open_Timer_ON_SendSms(void)
{
		static int sizeof_on_SendSms;
		static int count;
		sizeof_on_SendSms=2;
		Mode_Close_all();
  		//led_share_grean_grean();	   
		count++;
		if(sizeof_on_SendSms<=count)
			{
			  led_share_grean();
			  count=0;
		//	  return;
			}
		SANLED_Timer.data = (unsigned long)10;
		mod_timer(&SANLED_Timer,(jiffies + 10));
}
void Mode9_open_Timer_ON_ComingCall(void)
{
		static int sizeof_on_ComingCall;
		static int count;
		//sizeof_on_ComingCall=2;
		sizeof_on_ComingCall=(sizeof(PWMup_ComingCall))/sizeof(PWMup_ComingCall[0]);
		//Mode_Close_all();
  		//led_share_red_grean_blue();	    
		//count++;
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,PWMup_ComingCall[count++]))
			{
		  return;
		}
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,PWMup_ComingCall[count++]))
			{
		  return;
		}
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x20,PWMup_ComingCall[count++]))
			{
		  return;
		}
		if(sizeof_on_ComingCall<=count)
			{
				Mode_Close_all();
			  //led_red_grean_blue();
			  count=0;
			  return;
			}
		SANLED_Timer.data = (unsigned long)9;
		mod_timer(&SANLED_Timer,(jiffies + 5));     //20
}

void Mode8_open_Timer_ON_OutCall(void)
{
		static int sizeof_on_OutCall;
		static int count;
		sizeof_on_OutCall=2;
		Mode_Close_all();
  		//led_share_grean_grean();	
		count++;
		if(sizeof_on_OutCall<=count)
			{
			  led_share_grean();
			  count=0;
			//  return;
			}
		SANLED_Timer.data = (unsigned long)8;
		mod_timer(&SANLED_Timer,(jiffies + 10));
}


void Mode7_open_Timer_ON_UnAnswerInComeCall( void )
{
		static int sizeof_on_UnAnswerInComeCall;
		static int count;
		sizeof_on_UnAnswerInComeCall=10;
		Mode_Close_all();
  		//led_share_grean_grean();	    
		count++;
		if(sizeof_on_UnAnswerInComeCall<=count)
			{
			  led_share_grean();
			  count=0;
			//  return;
			}
	   	SANLED_Timer.data = (unsigned long)7;
		mod_timer(&SANLED_Timer,(jiffies + 60));
}

void Mode6_open_Timer_ON_UnReadSms(void)
{
		static int sizeof_on_UnReadSms;
		static int count;
		sizeof_on_UnReadSms=10;
		Mode_Close_all();
		//led_share_grean_grean();
		count++;
		if(sizeof_on_UnReadSms<=count)
			{
			  led_share_grean();
			  count=0;
			 // return;
			}
		SANLED_Timer.data = (unsigned long)6;
		mod_timer(&SANLED_Timer,(jiffies + 60));
}


void Mode5_open_Timer_ON_LessBattery( void )
{
		static int sizeof_on_lessbattery;
		static int count;
		sizeof_on_lessbattery=10;
		Mode_Close_all();
 		//led_share_red_red();
		count++;
		if(sizeof_on_lessbattery<=count)
			{
			  led_share_red();
			  count=0;
			//  return;
			}
   		SANLED_Timer.data = (unsigned long)5;
		mod_timer(&SANLED_Timer,(jiffies + 60));
}

void Mode4_open_Timer_ON_Alarm()
{
		static int sizeof_on_alarm;
		static int count;
		sizeof_on_alarm=2;
		led_red_grean_blue();
		
  		//led_share_red_grean_blue();	   
		count++;
		if(sizeof_on_alarm<=count)
			{
				count=0;
			  Mode_Close_all();
			  return;
			}
		SANLED_Timer.data = (unsigned long)4;
		mod_timer(&SANLED_Timer,(jiffies + 20));
}

void Mode3_open_Timer_ON_Music()
{
		static int sizeof_on_music;
		static int count;
		sizeof_on_music=2;
		//sizeof_on_music=(sizeof(PWMup_Music))/sizeof(PWMup_Music[0]);
		//Mode_Close_all();
		//led_share_red_grean_blue();
		//count++;
		/*if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,PWMup_Music[count++]))
			{
		  return;
		}
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,PWMup_Music[count++]))
			{
		  return;
		}
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x20,PWMup_Music[count++]))
			{
		  return;
		}*/
		
		led_share_red_grean_blue();
		
		if(sizeof_on_music<=count)
			{
				Mode_Close_all();
			  //led_red_grean_blue();
			  count=0;
			  //return;
			}
		SANLED_Timer.data = (unsigned long)3;
		mod_timer(&SANLED_Timer,(jiffies + 20));   //20
}

void Mode2_open_Timer_ON_Charge()
{
		static int sizeof_on_charge;
		static int count;
		//sizeof_on_charge=10;
		sizeof_on_charge=(sizeof(PWMup_Charge))/sizeof(PWMup_Charge[0]);
		//Mode_Close_all();
   		//led_share_red_red();
   // printk("Mode2_open_Timer_ON_Charge  \n");
		//count++;
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x22,PWMup_Charge[count++]))
			{
		  return;
		}
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x21,PWMup_Charge[count++]))
			{
		  return;
		}
		if(hwmsen_write_byte(SANLED_marquee_i2c_client,0x20,PWMup_Charge[count++]))
			{
		  return;
		}
		if(sizeof_on_charge<=count)
			{
				Mode_Close_all();
			  //led_share_red();
			  count=0;
			  return;
			}
		SANLED_Timer.data = (unsigned long)2;
		mod_timer(&SANLED_Timer,(jiffies + 20));     //60
}

void Mode1_open_Timer_ON_NOTICE()
{
		static int sizeof_on_notice;
		static int count;
		sizeof_on_notice=10;
		Mode_Close_all();
   		//led_share_grean_grean();
		count++;
		if(sizeof_on_notice<=count)
			{
			  led_share_grean();
			  count=0;
			//  return;
			}
		SANLED_Timer.data = (unsigned long)1;
		mod_timer(&SANLED_Timer,(jiffies + 60));
}


void Marquee_open_mod(int mod,int timer_delay)
{
	count = 0;
	SANLED_Timer.data = (unsigned long)mod;
	SANLED_Timer.expires = jiffies + timer_delay;
//	add_timer(&(SANLED_Timer));
	mod_timer(&SANLED_Timer,(jiffies + timer_delay));
}

//////////////////////////////////////////////////////////////////////////////////////////

void Mode1_open_NOTICE()
{
	Marquee_open_mod(1,10);
}

void Mode2_open_Charge() /////  
{
	Marquee_open_mod(2,50);
}

void Mode3_open_Music()
{
	Marquee_open_mod(3,10);
}

void Mode4_open_ON_Alarm(void) 
{
	Marquee_open_mod(4,10);
}
void Mode5_open_ON_LessBattery(void) 
{
	Marquee_open_mod(5,10);
}

void Mode6_open_ON_UnReadSms(void) 
{
	Marquee_open_mod(6,10);
}

void Mode7_open_ON_UnAnswerInComeCal(void) 
{
	Marquee_open_mod(7,10);
}

void Mode8_open_ON_OutCall(void) 
{
	Marquee_open_mod(8,10);
}
void Mode9_open_ON_ComingCall(void) 
{
	Marquee_open_mod(9,10);
}
void Mode10_open_ON_SendSms(void)
{
	Marquee_open_mod(10,10);
}
void Mode11_open_ON_ComingSms(void)
{
	Marquee_open_mod(11,10);
}
void Mode12_open_ON_PowerOff(void)
{
	Marquee_open_mod(12,10);
}
void Mode13_open_ON_PowerOn(void)
{
	Marquee_open_mod(13,10);
}

void Mode14_CLOSE_ALL() /////  
{
	Marquee_open_mod(14,10);
	del_timer(&SANLED_Timer);
	
	SANLED_Chip_Pin_Off();
}

void Mode15_MUSIC_SPEED_HIGH()
{
	Marquee_open_mod(15,10);
}

void Mode16_MUSIC_SPEED_SLOW()
{
	Marquee_open_mod(16,10);
}

/*void SANLED_ChipOff()
{
	Marquee_open_mod(15,10);
	
	del_timer(&SANLED_Timer);
	
	SANLED_Chip_Pin_Off();	

}*/

void SANLED_timer_fn(unsigned long arg)
{
    SANLED_Start_thread = 1;
	
	if(arg == 1)
	{
        Mode1 = 1;
	}
	else if(arg == 2)
	{
         Mode1 = 2;
	}
    else if(arg == 3)
	{
         Mode1 = 3;
	}
    else if(arg == 4)
	{
         Mode1 = 4;
	}else if(arg ==5)
	{
		Mode1 =5;
	}
	else if(arg ==6)
	{
		Mode1 =6;

	}else if(arg == 7)
	{
	   Mode1 =7;		
	}else if (arg ==8)
	{
		Mode1 =8;		
	}else if(arg ==9)
	{
		Mode1 =9;
	}
	else if(arg ==10)
	{
		Mode1 =10;

	}else if(arg == 11)
	{
	   Mode1 =11;		
	}else if (arg ==12)
	{
		Mode1 =12;		
	}else if (arg ==13)
	{
		Mode1 =13;		
	}
	else if (arg ==14)
	{
		Mode1 =14;		
	}
	else if (arg ==15)
	{
		Mode1 =15;		
	}
	else if (arg ==16)
	{
		Mode1 =16;		
	}
	wake_up_interruptible_sync(&SANLED_waitMain);
}

/****************************************************************************** 
 * Function Configuration
******************************************************************************/
////////////////////////////////////////////////////////////////////////////


static int SANLED_event_handler(void *unused) 
{
       do {
		wait_event_interruptible(SANLED_waitMain,SANLED_Start_thread != 0);
             SANLED_Start_thread = 0;


			switch (Mode1)
				{
					case 1:
						Mode1_open_Timer_ON_NOTICE();
						break;
					case 2:
						Mode2_open_Timer_ON_Charge();
						break;
					case 3:
						Mode3_open_Timer_ON_Music();
						break;
					case 4:
						Mode4_open_Timer_ON_Alarm();
						break;
					case 5:
						Mode5_open_Timer_ON_LessBattery();
						break;
					case 6:
						Mode6_open_Timer_ON_UnReadSms();
						break;
					case 7:
						Mode7_open_Timer_ON_UnAnswerInComeCall();
						break;
					case 8:
						Mode8_open_Timer_ON_OutCall();
						break;
					case 9:
						Mode9_open_Timer_ON_ComingCall();
						break;
					case 10:
						Mode10_open_Timer_ON_SendSms();	
						break;
					case 11:
						Mode11_open_Timer_ON_ComingSms();
						break;
					case 12:
						Mode12_open_Timer_ON_PowerOff();
						break;
					case 13:
						Mode13_open_Timer_ON_PowerOn();
						break;
					case 14:
						Mode14_open_Timer_Close_all();
						break;
					case 15:
						Mode15_open_Timer_Music_high();
						break;
					case 16:
						Mode16_open_Timer_Music_slow();
						break;
					default :

						break;
				}

         }while (!kthread_should_stop()); 
}

static int SANLED_marquee_open(struct inode *inode, struct file *file)
{

printk("HMH marquee  SANLED_marquee_open SANLED_marquee_i2c_client =%d \n",SANLED_marquee_i2c_client);
	file->private_data = SANLED_marquee_i2c_client;
	if (!file->private_data)
	{
		return -EINVAL;
	}

	return nonseekable_open(inode, file);
}
/*----------------------------------------------------------------------------*/
static int SANLED_marquee_release(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	return 0;
}
/*----------------------------------------------------------------------------*/
static int SANLED_marquee_ioctl(struct file *file, unsigned int cmd,  unsigned long arg)
{
	int err = 0;
	int i = 0;

	printk("HMH marquee  SANLED_marquee_ioctl  cmd =%d arg=%ld \n", cmd,arg);

	del_timer(&SANLED_Timer);

	init_timer(&(SANLED_Timer));
       SANLED_Timer.function = SANLED_timer_fn;
	SANLED_Timer.expires = jiffies;
	
	switch (cmd) 
	{
		case MODEL1_OPEN_NOTICE:
			i = 1;
			printk("HMH marquee  MODEL1_OPEN_NOTICE\n");
			Mode1_open_NOTICE();
			break;
		case MODEL2_OPEN_CHARGE:
			i = 2;
			Mode2_open_Charge();
		      printk("Mode2_open_Charge\n");
			break;
		case MODEL3_OPEN_MUSIC:
			i = 3;
			Mode3_open_Music();
			break;
		case MODEL4_OPEN_ALARM:
			i = 4;
			Mode4_open_ON_Alarm();
			break;
		case MODEL5_OPEN_LESSBATTERY:
			i =5;
			Mode5_open_ON_LessBattery();
			break;
		case MODEL6_OPEN_UNREADSMS:
			i= 6;
			Mode6_open_ON_UnReadSms();
			break;
		case MODEL7_OPEN_UNANSWERINCOMECAL:
			i= 7;
			Mode7_open_ON_UnAnswerInComeCal();
			break;
		case MODEL8_OPEN_OUTCALL:
			i =8;
			Mode8_open_ON_OutCall();
			break;
		case MODEL9_OPEN_COMINGCALL:
			i =9;
			Mode9_open_ON_ComingCall();
			break;
		case MODEL10_OPEN_SENDSMS:
			i = 10;
			Mode10_open_ON_SendSms();
			break;
		case MODEL11_OPEN_COMINGSMS:
			i = 11;
			Mode11_open_ON_ComingSms();
			break;
		case MODEL12_OPEN_POWEROFF:
			i = 12;
			Mode12_open_ON_PowerOff();
			break;
		case MODEL13_OPEN_POWERON:
			i = 13;
			Mode13_open_ON_PowerOn();
			break;
		case MODEL15_CLOSE_ALL:
			i = 14;
			Mode14_CLOSE_ALL();
			break;
		case MODEL15_MUSIC_SPEED_HIGHT:
			i = 15;
			printk("HMH marquee  MODEL15_CLOSE_ALL\n");
			Mode15_MUSIC_SPEED_HIGH();
			break;
		case MODEL16_MUSIC_SPEED_SLOW:
			i = 16;
			printk("HMH marquee  MODEL15_CLOSE_ALL\n");
			Mode16_MUSIC_SPEED_SLOW();
			break;
	    default:
	    	i = 17;
	             printk("HMH marquee  SANLED_marquee_ioctl  NONE \n");
			return -EINVAL;
	}
	printk("mycat HMH marquee  return i = %d \n", i);
	return 0;   
}
/*----------------------------------------------------------------------------*/

static struct file_operations SANLED_fops = {
	.owner = THIS_MODULE,
	.open = SANLED_marquee_open,
	.release = SANLED_marquee_release,
	.unlocked_ioctl = SANLED_marquee_ioctl,
};

static struct miscdevice SANLED_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "marquee",
	.fops = &SANLED_fops,
};

/*----------------------------------------------------------------------------*/
static int SANLED_marquee_i2c_suspend(struct i2c_client *client, pm_message_t msg) 
{
    printk("HMH marquee  SANLED_marquee_i2c_suspend i2c addr=0x%x\n", client->addr);
	return 0;
}
/*----------------------------------------------------------------------------*/
static int SANLED_marquee_i2c_resume(struct i2c_client *client)
{
    printk("HMH marquee  SANLED_marquee_i2c_resume i2c addr=0x%x\n", client->addr);
	return 0;
}

/*----------------------------------------------------------------------------*/
static int SANLED_marquee_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info) 
{    
	strcpy(info->type, SANLED_marquee_DEV_NAME);
	return 0;
}

static int aw9523b_init_device(struct i2c_client *client)
{
	u8 buf =0;
	u8  dat;
	int err = 0;
	
	printk("aw9523b_init_device.........\r\n");
	/*if(hwmsen_write_byte(client,0x7F,0x00))
	{
	  return -EFAULT;
	}
	*/
	
	if(hwmsen_write_byte(client,0x11,0x02))
	{
	  return -EFAULT;
	}
	
	//refor to datasheet
	if(hwmsen_write_byte(client,0x12,0x00))//P1_0 P1_1 P1_2 配置为呼吸灯模式
	{
	  return -EFAULT;
	}

	if(hwmsen_write_byte(client,0x13,0x00))//P1_0 P1_1 P1_2 配置为呼吸灯模式
	{
	  return -EFAULT;
	}
	/*
	mdelay(10);
	if(hwmsen_write_byte(client,0x06,0xFF))
	{
	  return -EFAULT;
	}
	mdelay(10);
	if(hwmsen_write_byte(client,0x07,0xFF))
	{
	  return -EFAULT;
	}
	mdelay(10);
	if(hwmsen_write_byte(client,0x04,0x00))
	{
	  return -EFAULT;
	}
	mdelay(10);
	if(hwmsen_write_byte(client,0x05,0x00))
	{
	  return -EFAULT;
	}
	mdelay(10);
	if(hwmsen_write_byte(client,0x02,0x00))
	{
	  return -EFAULT;
	}
	mdelay(10);
	if(hwmsen_write_byte(client,0x03,0x00))
	{
	  return -EFAULT;
	}
	mdelay(10);
	*/
	
	
	if(hwmsen_write_byte(client,0x20,0x00))//OUT0口调光,调光等级为0-255。OUT0~OUT9的调光指令依次为0x20~0x29. 写0关闭
	{
	  return -EFAULT;
	}
	
	if(hwmsen_write_byte(client,0x21,0x00))//OUT0口调光,调光等级为0-255。OUT0~OUT9的调光指令依次为0x20~0x29. 写0关闭
	{
	  return -EFAULT;
	}
	
	if(hwmsen_write_byte(client,0x22,0x00))//OUT0口调光,调光等级为0-255。OUT0~OUT9的调光指令依次为0x20~0x29. 写0关闭
	{
	  return -EFAULT;
	}

	printk("aw9523b_init_device.........OK \r\n");
	return 0;
}

static int SANLED_marquee_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int err = 0;
		u8  dat;

    printk("HMH marquee i2c  probe  \n");
       SANLED_marquee_i2c_client = client;
       //SANLED_marquee_i2c_client->timing = 100;
       
  if((err = hwmsen_read_byte(SANLED_marquee_i2c_client, 0x10, &dat)))
	{
		printk("read data fail!!\n");
		return err;
	}
	
	printk("HMH aw9523b  aw9523b id = 0x%x \n",dat);
	
	if((err = aw9523b_init_device(SANLED_marquee_i2c_client)))
	{
		printk("init dev: %d\n", err);
		return err;
	}

	if(err = misc_register(&SANLED_device))
	{
		printk("HMH marquee  SANLED_marquee_i2c_probe register failed\n");
	}

	init_waitqueue_head(&SANLED_waitMain);

    thread = kthread_run(SANLED_event_handler, 0, SANLED_marquee_DEV_NAME);
    if (IS_ERR(thread)) { 
        err = PTR_ERR(thread);
        printk("HMH marquee failed to create kernel thread: %d\n", err);
    }
	
	return err;
}
/*----------------------------------------------------------------------------*/
static int SANLED_marquee_i2c_remove(struct i2c_client *client)
{
	int err;
	misc_deregister(&SANLED_device);
	return 0;
}

static struct i2c_driver SANLED_marquee_i2c_driver = {	
	.probe      = SANLED_marquee_i2c_probe,
	.remove     = SANLED_marquee_i2c_remove,
	.detect     = SANLED_marquee_i2c_detect,
	.suspend    = SANLED_marquee_i2c_suspend,
	.resume     = SANLED_marquee_i2c_resume,
	.id_table   = SANLED_marquee_i2c_id,
	.driver = {
		.owner          = THIS_MODULE,
		.name           = SANLED_marquee_DEV_NAME,
	},
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------*/
static int SANLED_marquee_probe(struct platform_device *pdev) 
{
	if(i2c_add_driver(&SANLED_marquee_i2c_driver))
	{
		printk("HMH marquee add driver error\n");
		return -1;
	} 
	return 0;
}
/*----------------------------------------------------------------------------*/
static int SANLED_marquee_remove(struct platform_device *pdev)
{
	i2c_del_driver(&SANLED_marquee_i2c_driver);
	return 0;
}
/*----------------------------------------------------------------------------*/
static struct platform_driver SANLED_marquee_driver = {
	.probe      = SANLED_marquee_probe,
	.remove     = SANLED_marquee_remove,    
	.driver     = {
		.name  = "marquee",
		.owner = THIS_MODULE,
	}
};
/*----------------------------------------------------------------------------*/
static int __init SANLED_init(void)
{
	mt_set_gpio_mode(GPIO_AW9523B_RST_PIN, GPIO_MODE_00); 
  mt_set_gpio_dir(GPIO_AW9523B_RST_PIN, GPIO_DIR_OUT); 
  mt_set_gpio_out(GPIO_AW9523B_RST_PIN, GPIO_OUT_ZERO); 
  mdelay(10);
  mt_set_gpio_out(GPIO_AW9523B_RST_PIN, GPIO_OUT_ONE); 
  mdelay(10);
  
       i2c_register_board_info(1, &i2c_SANLED, 1);
	if(platform_driver_register(&SANLED_marquee_driver))
	{
		printk("HMH marquee failed to register driver");
		return -ENODEV;
	}
	return 0;
}
/*----------------------------------------------------------------------------*/
static void __exit SANLED_exit(void)
{
	    platform_driver_unregister(&SANLED_marquee_driver);
}
/*----------------------------------------------------------------------------*/
module_init(SANLED_init);
module_exit(SANLED_exit);
/*----------------------------------------------------------------------------*/
MODULE_AUTHOR("ma liejun");
MODULE_DESCRIPTION("SANLED driver");
MODULE_LICENSE("GPL");
