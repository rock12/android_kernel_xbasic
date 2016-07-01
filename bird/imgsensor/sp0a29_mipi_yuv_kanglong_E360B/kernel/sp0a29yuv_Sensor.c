/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   Sensor.c
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Image sensor driver function
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/



/*#####################################################


superpix    sensor   30m  SP0A29 .   sensorID = 0X0A       SLAVE ADDR= 0X42 



#####################################################*/

 
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include <asm/io.h>

#include "kd_camera_hw.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"

#include "sp0a29yuv_Sensor.h"
#include "sp0a29yuv_Camera_Sensor_para.h"
#include "sp0a29yuv_CameraCustomized.h"

static MSDK_SENSOR_CONFIG_STRUCT SP0A29SensorConfigData;
static struct SP0A29_Sensor_Struct SP0A29_Sensor_Driver;




#define SP0A29YUV_DEBUG
#ifdef SP0A29YUV_DEBUG
#define SENSORDB printk
#else
#define SENSORDB(x,...)
#endif


#define __SENSOR_CONTROL__
#ifdef __SENSOR_CONTROL__
#define CAMERA_CONTROL_FLOW(para1,para2) printk("[%s:%d]::para1=0x%x,para1=0x%x\n\n",__FUNCTION__,__LINE__,para1,para2)
#else
#define CAMERA_CONTROL_FLOW(para1, para2)
#endif


static kal_uint8 isBanding = 1; // 0: 50hz  1:60hz


#define DEBUG_SENSOR	//T-card for camera debug on/off
#ifdef DEBUG_SENSOR

kal_uint8 fromsd = 0;
kal_uint16 SP0A29_write_cmos_sensor(kal_uint8 addr, kal_uint8 para);

	#define SP0A29_OP_CODE_INI		0x00		/* Initial value. */
	#define SP0A29_OP_CODE_REG		0x01		/* Register */
	#define SP0A29_OP_CODE_DLY		0x02		/* Delay */
	#define SP0A29_OP_CODE_END		0x03		/* End of initial setting. */
	

	typedef struct
	{
		u16 init_reg;
		u16 init_val;	/* Save the register value and delay tick */
		u8 op_code;		/* 0 - Initial value, 1 - Register, 2 - Delay, 3 - End of setting. */
	} SP0A29_initial_set_struct;

	SP0A29_initial_set_struct SP0A29_Init_Reg[1000];
	
 u32 strtol(const char *nptr, u8 base)
{
	u8 ret;
	if(!nptr || (base!=16 && base!=10 && base!=8))
	{
		printk("%s(): NULL pointer input\n", __FUNCTION__);
		return -1;
	}
	for(ret=0; *nptr; nptr++)
	{
		if((base==16 && *nptr>='A' && *nptr<='F') || 
			(base==16 && *nptr>='a' && *nptr<='f') || 
			(base>=10 && *nptr>='0' && *nptr<='9') ||
			(base>=8 && *nptr>='0' && *nptr<='7') )
		{
			ret *= base;
			if(base==16 && *nptr>='A' && *nptr<='F')
				ret += *nptr-'A'+10;
			else if(base==16 && *nptr>='a' && *nptr<='f')
				ret += *nptr-'a'+10;
			else if(base>=10 && *nptr>='0' && *nptr<='9')
				ret += *nptr-'0';
			else if(base>=8 && *nptr>='0' && *nptr<='7')
				ret += *nptr-'0';
		}
		else
			return ret;
	}
	return ret;
}

 u8 SP0A29_Initialize_from_T_Flash()
{
	//FS_HANDLE fp = -1;				/* Default, no file opened. */
	//u8 *data_buff = NULL;
	u8 *curr_ptr = NULL;
	u32 file_size = 0;
	//u32 bytes_read = 0;
	u32 i = 0, j = 0;
	u8 func_ind[4] = {0};	/* REG or DLY */

	
	struct file *fp; 
    mm_segment_t fs; 
    loff_t pos = 0; 
	static u8 data_buff[10*1024] ;
 
    fp = filp_open("/storage/sdcard1/sp0a29_sd.dat", O_RDONLY , 0); 
    if (IS_ERR(fp)) { 
		printk("create file error 0\n");
		fp = filp_open("/storage/sdcard0/sp0a29_sd.dat", O_RDONLY , 0); 
    	if (IS_ERR(fp)) { 
	        printk("create file error 1\n"); 
	        return 2;//-1; 
    	}
    } 
    else
		printk("SP0A29_Initialize_from_T_Flash Open File Success\n");
	
    fs = get_fs(); 
    set_fs(KERNEL_DS); 

    file_size = vfs_llseek(fp, 0, SEEK_END);
    vfs_read(fp, data_buff, file_size, &pos); 
    filp_close(fp, NULL); 
    set_fs(fs);
	
	

printk("1\n");

	/* Start parse the setting witch read from t-flash. */
	curr_ptr = data_buff;
	while (curr_ptr < (data_buff + file_size))
	{
		while ((*curr_ptr == ' ') || (*curr_ptr == '\t'))/* Skip the Space & TAB */
			curr_ptr++;				

		if (((*curr_ptr) == '/') && ((*(curr_ptr + 1)) == '*'))
		{
			while (!(((*curr_ptr) == '*') && ((*(curr_ptr + 1)) == '/')))
			{
				curr_ptr++;		/* Skip block comment code. */
			}

			while (!((*curr_ptr == 0x0D) && (*(curr_ptr+1) == 0x0A)))
			{
				curr_ptr++;
			}

			curr_ptr += 2;						/* Skip the enter line */
			
			continue ;
		}
		
		if (((*curr_ptr) == '/') || ((*curr_ptr) == '{') || ((*curr_ptr) == '}'))		/* Comment line, skip it. */
		{
			while (!((*curr_ptr == 0x0D) && (*(curr_ptr+1) == 0x0A)))
			{
				curr_ptr++;
			}

			curr_ptr += 2;						/* Skip the enter line */

			continue ;
		}
		/* This just content one enter line. */
		if (((*curr_ptr) == 0x0D) && ((*(curr_ptr + 1)) == 0x0A))
		{
			curr_ptr += 2;
			continue ;
		}
		//printk(" curr_ptr1 = %s\n",curr_ptr);
		memcpy(func_ind, curr_ptr, 3);
	
						
		if (strcmp((const char *)func_ind, "REG") == 0)		/* REG */
		{
			curr_ptr += 6;				/* Skip "REG(0x" or "DLY(" */
			SP0A29_Init_Reg[i].op_code = SP0A29_OP_CODE_REG;
			
			SP0A29_Init_Reg[i].init_reg = strtol((const char *)curr_ptr, 16);
			curr_ptr += 5;	/* Skip "00, 0x" */
		
			SP0A29_Init_Reg[i].init_val = strtol((const char *)curr_ptr, 16);
			curr_ptr += 4;	/* Skip "00);" */
		
		}
		else									/* DLY */
		{
			/* Need add delay for this setting. */
			curr_ptr += 4;	
			SP0A29_Init_Reg[i].op_code = SP0A29_OP_CODE_DLY;
			
			SP0A29_Init_Reg[i].init_reg = 0xFF;
			SP0A29_Init_Reg[i].init_val = strtol((const char *)curr_ptr,  10);	/* Get the delay ticks, the delay should less then 50 */
		}
		i++;
		

		/* Skip to next line directly. */
		while (!((*curr_ptr == 0x0D) && (*(curr_ptr+1) == 0x0A)))
		{
			curr_ptr++;
		}
		curr_ptr += 2;
	}
printk("2\n");
	/* (0xFFFF, 0xFFFF) means the end of initial setting. */
	SP0A29_Init_Reg[i].op_code = SP0A29_OP_CODE_END;
	SP0A29_Init_Reg[i].init_reg = 0xFF;
	SP0A29_Init_Reg[i].init_val = 0xFF;
	i++;
	//for (j=0; j<i; j++)
		//printk(" %x  ==  %x\n",SP0A29_Init_Reg[j].init_reg, SP0A29_Init_Reg[j].init_val);

	/* Start apply the initial setting to sensor. */
	#if 1
	for (j=0; j<i; j++)
	{
		if (SP0A29_Init_Reg[j].op_code == SP0A29_OP_CODE_END)	/* End of the setting. */
		{
			break ;
		}
		else if (SP0A29_Init_Reg[j].op_code == SP0A29_OP_CODE_DLY)
		{
			msleep(SP0A29_Init_Reg[j].init_val);		/* Delay */
		}
		else if (SP0A29_Init_Reg[j].op_code == SP0A29_OP_CODE_REG)
		{
		
			SP0A29_write_cmos_sensor((kal_uint8)SP0A29_Init_Reg[j].init_reg, (kal_uint8)SP0A29_Init_Reg[j].init_val);
		}
		else
		{
			printk("REG ERROR!\n");
		}
	}
#endif

printk("3\n");
	return 1;	
}
#endif
#if 0
extern int iReadReg(u16 a_u2Addr , u8 * a_puBuff , u16 i2cId);
extern int iWriteReg(u16 a_u2Addr , u32 a_u4Data , u32 a_u4Bytes , u16 i2cId);
static int sensor_id_fail = 0; 
#define SP0A29_write_cmos_sensor(addr,para) iWriteReg((u16) addr , (u32) para ,1,SP0A29_WRITE_ID)
//#define SP0A29_write_cmos_sensor_2(addr, para, bytes) iWriteReg((u16) addr , (u32) para ,bytes,SP0A29_WRITE_ID)
kal_uint16 SP0A29_read_cmos_sensor(kal_uint32 addr)
{
kal_uint16 get_byte=0;
    iReadReg((u16) addr ,(u8*)&get_byte,SP0A29_WRITE_ID);
    return get_byte;
}
#endif

extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);
/*************************************************************************
* FUNCTION
*    SP0A29_write_cmos_sensor
*
* DESCRIPTION
*    This function wirte data to CMOS sensor through I2C
*
* PARAMETERS
*    addr: the 16bit address of register
*    para: the 8bit value of register
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
kal_uint16 SP0A29_write_cmos_sensor(kal_uint8 addr, kal_uint8 para)
{
    char puSendCmd[2] = {(char)(addr & 0xFF) , (char)(para & 0xFF)};
    iWriteRegI2C(puSendCmd , 2,SP0A29_WRITE_ID);
    return 0;
}

/*************************************************************************
* FUNCTION
*    SP0A29_read_cmos_sensor
*
* DESCRIPTION
*    This function read data from CMOS sensor through I2C.
*
* PARAMETERS
*    addr: the 16bit address of register
*
* RETURNS
*    8bit data read through I2C
*
* LOCAL AFFECTED
*
*************************************************************************/


kal_uint16 SP0A29_read_cmos_sensor(kal_uint8 addr)
{
    kal_uint16 get_byte=0;
    char puSendCmd = { (char)(addr & 0xFF) };
    iReadRegI2C(&puSendCmd , 1, (u8*)&get_byte,1,SP0A29_WRITE_ID);
    return get_byte;
}

 static void SP0A29_Set_Dummy(const kal_uint16 iPixels, const kal_uint16 iLines)
{
}   /*  SP0A29_Set_Dummy    */


/*************************************************************************
* FUNCTION
*	SP0A29_write_reg
*
* DESCRIPTION
*	This function set the register of SP0A29.
*
* PARAMETERS
*	addr : the register index of OV76X0
*  para : setting parameter of the specified register of OV76X0
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/

//static void SP0A29_write_reg(kal_uint32 addr, kal_uint32 para)
//{
//	SP0A29_write_cmos_sensor(addr,para);
//}	/* SP0A29_write_reg() */

/*************************************************************************
* FUNCTION
*	ov7670_read_cmos_sensor
*
* DESCRIPTION
*	This function read parameter of specified register from OV76X0.
*
* PARAMETERS
*	addr : the register index of OV76X0
*
* RETURNS
*	the data that read from OV76X0
*
* GLOBALS AFFECTED
*
*************************************************************************/
//static kal_uint32 SP0A29_read_reg(kal_uint32 addr)
//{
//	return (SP0A29_read_cmos_sensor(addr));
//}	/* OV7670_read_reg() */


/*************************************************************************
* FUNCTION
*	SP0A29_NightMode
*
* DESCRIPTION
*	This function night mode of SP0A29.
*
* PARAMETERS
*	bEnable: KAL_TRUE -> enable night mode, otherwise, disable night mode
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static void SP0A29_night_mode(kal_bool bEnable)
{
// kal_uint8 temp = SP0A29_read_cmos_sensor(0x3B);

#if 1
  if (!SP0A29_Sensor_Driver.MODE_CAPTURE) { 
	if(bEnable)//night mode
	{ 
		   SP0A29_Sensor_Driver.bNight_mode = KAL_TRUE;
				SP0A29_write_cmos_sensor(0xfd,0x01);
				SP0A29_write_cmos_sensor(0xcd,0x25);
				SP0A29_write_cmos_sensor(0xce,0x1f);
	   if(SP0A29_Sensor_Driver.MPEG4_encode_mode == KAL_TRUE)
		{
				if(isBanding== 0)
				{
				printk("video 50Hz night\n");	
				//Video record night 24M 50hz 12-12FPS maxgain				                     
				SP0A29_write_cmos_sensor(0xfd , 0x00);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x04 , 0x68);
				SP0A29_write_cmos_sensor(0x06 , 0x00);
				SP0A29_write_cmos_sensor(0x09 , 0x04);
				SP0A29_write_cmos_sensor(0x0a , 0x7e);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0xf0 , 0x00);
				SP0A29_write_cmos_sensor(0xf7 , 0x3c);
				SP0A29_write_cmos_sensor(0x02 , 0x08);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x06 , 0x3c);
				SP0A29_write_cmos_sensor(0x07 , 0x00);
				SP0A29_write_cmos_sensor(0x08 , 0x01);
				SP0A29_write_cmos_sensor(0x09 , 0x00);
				SP0A29_write_cmos_sensor(0xfd , 0x02);
				SP0A29_write_cmos_sensor(0xbe , 0xe0);
				SP0A29_write_cmos_sensor(0xbf , 0x01);
				SP0A29_write_cmos_sensor(0xd0 , 0xe0);
				SP0A29_write_cmos_sensor(0xd1 , 0x01);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0x5b , 0x01);
				SP0A29_write_cmos_sensor(0x5c , 0xe0);

				//dbg_print(" video 50Hz night\r\n");
				}
				else if(isBanding == 1)
				{
				//Video record night 24M 60Hz 12-12FPS maxgain:
				SP0A29_write_cmos_sensor(0xfd , 0x00);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x04 , 0x2c);
				SP0A29_write_cmos_sensor(0x06 , 0x00);
				SP0A29_write_cmos_sensor(0x09 , 0x04);
				SP0A29_write_cmos_sensor(0x0a , 0x7e);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0xf0 , 0x00);
				SP0A29_write_cmos_sensor(0xf7 , 0x32);
				SP0A29_write_cmos_sensor(0x02 , 0x0a);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x06 , 0x32);
				SP0A29_write_cmos_sensor(0x07 , 0x00);
				SP0A29_write_cmos_sensor(0x08 , 0x01);
				SP0A29_write_cmos_sensor(0x09 , 0x00);
				SP0A29_write_cmos_sensor(0xfd , 0x02);
				SP0A29_write_cmos_sensor(0xbe , 0xf4);
				SP0A29_write_cmos_sensor(0xbf , 0x01);
				SP0A29_write_cmos_sensor(0xd0 , 0xf4);
				SP0A29_write_cmos_sensor(0xd1 , 0x01);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0x5b , 0x01);
				SP0A29_write_cmos_sensor(0x5c , 0xf4);
						
				printk(" video 60Hz night\r\n");
				}
   		  	}	
	    else 
	   {
			//	dbg_print(" SP0A29_banding=%x\r\n",SP0A29_banding);
			       if(isBanding== 0)
				{
				//capture preview night 24M 50hz 20-6FPS maxgain:	 
				SP0A29_write_cmos_sensor(0xfd , 0x00);
				SP0A29_write_cmos_sensor(0x03 , 0x02);
				SP0A29_write_cmos_sensor(0x04 , 0x58);
				SP0A29_write_cmos_sensor(0x06 , 0x00);
				SP0A29_write_cmos_sensor(0x09 , 0x01);
				SP0A29_write_cmos_sensor(0x0a , 0x5e);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0xf0 , 0x00);
				SP0A29_write_cmos_sensor(0xf7 , 0x64);
				SP0A29_write_cmos_sensor(0x02 , 0x10);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x06 , 0x64);
				SP0A29_write_cmos_sensor(0x07 , 0x00);
				SP0A29_write_cmos_sensor(0x08 , 0x01);
				SP0A29_write_cmos_sensor(0x09 , 0x00);
				SP0A29_write_cmos_sensor(0xfd , 0x02);
				SP0A29_write_cmos_sensor(0xbe , 0x40);
				SP0A29_write_cmos_sensor(0xbf , 0x06);
				SP0A29_write_cmos_sensor(0xd0 , 0x40);
				SP0A29_write_cmos_sensor(0xd1 , 0x06);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0x5b , 0x06);
				SP0A29_write_cmos_sensor(0x5c , 0x40);

				printk(" priview 50Hz night\r\n");	
				}  
				else if(isBanding== 1)
				{
				//capture preview night 24M 60hz 20-6FPS maxgain:
				SP0A29_write_cmos_sensor(0xfd , 0x00);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x04 , 0xf8);
				SP0A29_write_cmos_sensor(0x06 , 0x00);
				SP0A29_write_cmos_sensor(0x09 , 0x01);
				SP0A29_write_cmos_sensor(0x0a , 0x54);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0xf0 , 0x00);
				SP0A29_write_cmos_sensor(0xf7 , 0x54);
				SP0A29_write_cmos_sensor(0x02 , 0x14);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x06 , 0x54);
				SP0A29_write_cmos_sensor(0x07 , 0x00);
				SP0A29_write_cmos_sensor(0x08 , 0x01);
				SP0A29_write_cmos_sensor(0x09 , 0x00);
				SP0A29_write_cmos_sensor(0xfd , 0x02);
				SP0A29_write_cmos_sensor(0xbe , 0x90);
				SP0A29_write_cmos_sensor(0xbf , 0x06);
				SP0A29_write_cmos_sensor(0xd0 , 0x90);
				SP0A29_write_cmos_sensor(0xd1 , 0x06);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0x5b , 0x06);
				SP0A29_write_cmos_sensor(0x5c , 0x90);

				printk(" priview 60Hz night\r\n");	
				}
			       } 		
	}
	else    // daylight mode
	{
		SP0A29_Sensor_Driver.bNight_mode = KAL_FALSE;
				SP0A29_write_cmos_sensor(0xfd,0x01);
				SP0A29_write_cmos_sensor(0xcd,0x10);
				SP0A29_write_cmos_sensor(0xce,0x1f);                
	    if(SP0A29_Sensor_Driver.MPEG4_encode_mode == KAL_TRUE)
	    {
				SP0A29_write_cmos_sensor(0xfd,0x01);
				SP0A29_write_cmos_sensor(0xcd,0x20);
				SP0A29_write_cmos_sensor(0xce,0x1f);
				//dbg_print(" SP0A29_banding=%x\r\n",SP0A29_banding);
				if(isBanding== 0)
				{
				//Video record daylight 24M 50hz 20FPS maxgain:                     
				SP0A29_write_cmos_sensor(0xfd , 0x00);
				SP0A29_write_cmos_sensor(0x03 , 0x02);
				SP0A29_write_cmos_sensor(0x04 , 0x58);
				SP0A29_write_cmos_sensor(0x06 , 0x00);
				SP0A29_write_cmos_sensor(0x09 , 0x01);
				SP0A29_write_cmos_sensor(0x0a , 0x5e);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0xf0 , 0x00);
				SP0A29_write_cmos_sensor(0xf7 , 0x64);
				SP0A29_write_cmos_sensor(0x02 , 0x05);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x06 , 0x64);
				SP0A29_write_cmos_sensor(0x07 , 0x00);
				SP0A29_write_cmos_sensor(0x08 , 0x01);
				SP0A29_write_cmos_sensor(0x09 , 0x00);
				SP0A29_write_cmos_sensor(0xfd , 0x02);
				SP0A29_write_cmos_sensor(0xbe , 0xf4);
				SP0A29_write_cmos_sensor(0xbf , 0x01);
				SP0A29_write_cmos_sensor(0xd0 , 0xf4);
				SP0A29_write_cmos_sensor(0xd1 , 0x01);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0x5b , 0x01);
				SP0A29_write_cmos_sensor(0x5c , 0xf4);


				printk(" video 50Hz normal\r\n");				
				}
				else if(isBanding == 1)
				{
				//Video record daylight 24M 60Hz 20FPS maxgain:
				SP0A29_write_cmos_sensor(0xfd , 0x00);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x04 , 0xf8);
				SP0A29_write_cmos_sensor(0x06 , 0x00);
				SP0A29_write_cmos_sensor(0x09 , 0x01);
				SP0A29_write_cmos_sensor(0x0a , 0x54);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0xf0 , 0x00);
				SP0A29_write_cmos_sensor(0xf7 , 0x54);
				SP0A29_write_cmos_sensor(0x02 , 0x06);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x06 , 0x54);
				SP0A29_write_cmos_sensor(0x07 , 0x00);
				SP0A29_write_cmos_sensor(0x08 , 0x01);
				SP0A29_write_cmos_sensor(0x09 , 0x00);
				SP0A29_write_cmos_sensor(0xfd , 0x02);
				SP0A29_write_cmos_sensor(0xbe , 0xf8);
				SP0A29_write_cmos_sensor(0xbf , 0x01);
				SP0A29_write_cmos_sensor(0xd0 , 0xf8);
				SP0A29_write_cmos_sensor(0xd1 , 0x01);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0x5b , 0x01);
				SP0A29_write_cmos_sensor(0x5c , 0xf8);

				printk(" video 60Hz normal\r\n");	
				}
			   }
		else 
			{
			//	dbg_print(" SP0A29_banding=%x\r\n",SP0A29_banding);
			       if(isBanding== 0)
				{
				//capture preview daylight 24M 50hz 20-10FPS maxgain:   
				SP0A29_write_cmos_sensor(0xfd,0x00);
				SP0A29_write_cmos_sensor(0x03,0x02);
				SP0A29_write_cmos_sensor(0x04,0x58);
				SP0A29_write_cmos_sensor(0x06,0x00);
				SP0A29_write_cmos_sensor(0x09,0x01);
				SP0A29_write_cmos_sensor(0x0a,0x5e);
				SP0A29_write_cmos_sensor(0xfd,0x01);
				SP0A29_write_cmos_sensor(0xf0,0x00);
				SP0A29_write_cmos_sensor(0xf7,0x64);
				SP0A29_write_cmos_sensor(0x02,0x0a);
				SP0A29_write_cmos_sensor(0x03,0x01);
				SP0A29_write_cmos_sensor(0x06,0x64);
				SP0A29_write_cmos_sensor(0x07,0x00);
				SP0A29_write_cmos_sensor(0x08,0x01);
				SP0A29_write_cmos_sensor(0x09,0x00);
				SP0A29_write_cmos_sensor(0xfd,0x02);
				SP0A29_write_cmos_sensor(0xbe,0xe8);
				SP0A29_write_cmos_sensor(0xbf,0x03);
				SP0A29_write_cmos_sensor(0xd0,0xe8);
				SP0A29_write_cmos_sensor(0xd1,0x03);
				SP0A29_write_cmos_sensor(0xfd,0x01);
				SP0A29_write_cmos_sensor(0x5b,0x03);
				SP0A29_write_cmos_sensor(0x5c,0xe8);
			
				printk(" priview 50Hz normal\r\n");
				}
				else if(isBanding== 1)
				{			
				//capture preview daylight 24M 60hz 20-10FPS maxgain                         					  
				SP0A29_write_cmos_sensor(0xfd , 0x00);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x04 , 0xf8);
				SP0A29_write_cmos_sensor(0x06 , 0x00);
				SP0A29_write_cmos_sensor(0x09 , 0x01);
				SP0A29_write_cmos_sensor(0x0a , 0x54);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0xf0 , 0x00);
				SP0A29_write_cmos_sensor(0xf7 , 0x54);
				SP0A29_write_cmos_sensor(0x02 , 0x0c);
				SP0A29_write_cmos_sensor(0x03 , 0x01);
				SP0A29_write_cmos_sensor(0x06 , 0x54);
				SP0A29_write_cmos_sensor(0x07 , 0x00);
				SP0A29_write_cmos_sensor(0x08 , 0x01);
				SP0A29_write_cmos_sensor(0x09 , 0x00);
				SP0A29_write_cmos_sensor(0xfd , 0x02);
				SP0A29_write_cmos_sensor(0xbe , 0xf0);
				SP0A29_write_cmos_sensor(0xbf , 0x03);
				SP0A29_write_cmos_sensor(0xd0 , 0xf0);
				SP0A29_write_cmos_sensor(0xd1 , 0x03);
				SP0A29_write_cmos_sensor(0xfd , 0x01);
				SP0A29_write_cmos_sensor(0x5b , 0x03);
				SP0A29_write_cmos_sensor(0x5c , 0xf0);
	
				printk(" priview 60Hz normal\r\n");
				}
			       }
	   
	}  
	}
  #endif
}	/*	SP0A29_NightMode	*/

/*
static void SP0A29_set_isp_driving_current(kal_uint8 current)
{
    //#define CONFIG_BASE      	(0xF0001000)     
//  iowrite32((0xE << 12)|(0 << 28)|0x8880888, 0xF0001500);
}
*/

static void SP0A29_Sensor_Driver_Init(void)
{ 
 SP0A29_write_cmos_sensor(0xfd,0x00);	
 SP0A29_write_cmos_sensor(0x1c,0x07);
 SP0A29_write_cmos_sensor(0x28,0x36);
 SP0A29_write_cmos_sensor(0x35,0x20);
 SP0A29_write_cmos_sensor(0xfb,0x31);
 SP0A29_write_cmos_sensor(0x03,0x04);
 SP0A29_write_cmos_sensor(0x04,0xb0);
 SP0A29_write_cmos_sensor(0x24,0x70);
 SP0A29_write_cmos_sensor(0x30,0x00);
 SP0A29_write_cmos_sensor(0x3a,0x06);
 
 SP0A29_write_cmos_sensor(0xfd,0x01);    
 SP0A29_write_cmos_sensor(0x5d,0x12);

 SP0A29_write_cmos_sensor(0xfd,0x00);   
 SP0A29_write_cmos_sensor(0x1b,0x17);
 SP0A29_write_cmos_sensor(0x38,0x5b);   
 SP0A29_write_cmos_sensor(0x0c,0x01);
 SP0A29_write_cmos_sensor(0xbc,0x6c);
 SP0A29_write_cmos_sensor(0x0c,0x00);
 SP0A29_write_cmos_sensor(0x0e,0x00);
 SP0A29_write_cmos_sensor(0x2f,0x01);
 SP0A29_write_cmos_sensor(0x27,0x2b);
 SP0A29_write_cmos_sensor(0x37,0x00);
 SP0A29_write_cmos_sensor(0xfd,0x01);    
 SP0A29_write_cmos_sensor(0x36,0x00);
 ////pre gain
  SP0A29_write_cmos_sensor(0xfd,0x02);    
  SP0A29_write_cmos_sensor(0x00,0x90);
  SP0A29_write_cmos_sensor(0x01,0x90);

 //ae 
  SP0A29_write_cmos_sensor(0xfd,0x02);
  SP0A29_write_cmos_sensor(0x52,0x04);
  SP0A29_write_cmos_sensor(0x53,0x01); //
  //SP0A29_write_cmos_sensor(0xf2,0x09); //sp0a29 0xf4//0x0b
  SP0A29_write_cmos_sensor(0x86,0x0c); //
  SP0A29_write_cmos_sensor(0x87,0x10); //
  SP0A29_write_cmos_sensor(0x8b,0x00);
  SP0A29_write_cmos_sensor(0xfd,0x01);
  SP0A29_write_cmos_sensor(0xf2,0x09); //sp0a29 0xf4//0x0b
  
#if 0
//sp0A29 24M 20-15fps 

  SP0A29_write_cmos_sensor(0xfd,0x00);
  SP0A29_write_cmos_sensor(0x03,0x02);
  SP0A29_write_cmos_sensor(0x04,0x58);
  SP0A29_write_cmos_sensor(0x06,0x00);
  SP0A29_write_cmos_sensor(0x09,0x01);
  SP0A29_write_cmos_sensor(0x0a,0x5e);
  SP0A29_write_cmos_sensor(0xfd,0x01);
  SP0A29_write_cmos_sensor(0xf0,0x00);
  SP0A29_write_cmos_sensor(0xf7,0x64);
  SP0A29_write_cmos_sensor(0x02,0x06);
  SP0A29_write_cmos_sensor(0x03,0x01); 
  SP0A29_write_cmos_sensor(0x06,0x64);
  SP0A29_write_cmos_sensor(0x07,0x00);
  SP0A29_write_cmos_sensor(0x08,0x01);
  SP0A29_write_cmos_sensor(0x09,0x00);
  SP0A29_write_cmos_sensor(0xfd,0x02);
  SP0A29_write_cmos_sensor(0xbe,0x58);
  SP0A29_write_cmos_sensor(0xbf,0x02);
  SP0A29_write_cmos_sensor(0xd0,0x58); 
  SP0A29_write_cmos_sensor(0xd1,0x02);
  SP0A29_write_cmos_sensor(0xfd,0x01);
  SP0A29_write_cmos_sensor(0x5b,0x02);
  SP0A29_write_cmos_sensor(0x5c,0x58);
 #else
 //sp0A29 24M 20-10fps  
   SP0A29_write_cmos_sensor(0xfd,0x00);
   SP0A29_write_cmos_sensor(0x03,0x02);
   SP0A29_write_cmos_sensor(0x04,0x58);
   SP0A29_write_cmos_sensor(0x06,0x00);
   SP0A29_write_cmos_sensor(0x09,0x01);
   SP0A29_write_cmos_sensor(0x0a,0x5e);
   SP0A29_write_cmos_sensor(0xfd,0x01);
   SP0A29_write_cmos_sensor(0xf0,0x00);
   SP0A29_write_cmos_sensor(0xf7,0x64);
   SP0A29_write_cmos_sensor(0x02,0x0a);
   SP0A29_write_cmos_sensor(0x03,0x01);
   SP0A29_write_cmos_sensor(0x06,0x64);
   SP0A29_write_cmos_sensor(0x07,0x00);
   SP0A29_write_cmos_sensor(0x08,0x01);
   SP0A29_write_cmos_sensor(0x09,0x00);
   SP0A29_write_cmos_sensor(0xfd,0x02);
   SP0A29_write_cmos_sensor(0xbe,0xe8);
   SP0A29_write_cmos_sensor(0xbf,0x03);
   SP0A29_write_cmos_sensor(0xd0,0xe8);
   SP0A29_write_cmos_sensor(0xd1,0x03);
   SP0A29_write_cmos_sensor(0xfd,0x01);
   SP0A29_write_cmos_sensor(0x5b,0x03);
   SP0A29_write_cmos_sensor(0x5c,0xe8);
 #endif
////fix status	
  SP0A29_write_cmos_sensor(0xfd,0x02);
  SP0A29_write_cmos_sensor(0xb8,0x6c);//mean_nr_dummy
  SP0A29_write_cmos_sensor(0xb9,0x7c);//mean_dummy_nr
  SP0A29_write_cmos_sensor(0xba,0x30);//mean_dummy_low
  SP0A29_write_cmos_sensor(0xbb,0x45);//mean_low_dummy
  SP0A29_write_cmos_sensor(0xbc,0x60);//rpc_heq_low
  SP0A29_write_cmos_sensor(0xbd,0x40);//rpc_heq_dummy
  
  
  ////rpc
  SP0A29_write_cmos_sensor(0xfd,0x01);
  SP0A29_write_cmos_sensor(0xe0,0x3e); 
  SP0A29_write_cmos_sensor(0xe1,0x30); 
  SP0A29_write_cmos_sensor(0xe2,0x2a); 
  SP0A29_write_cmos_sensor(0xe3,0x26);
  SP0A29_write_cmos_sensor(0xe4,0x26);
  SP0A29_write_cmos_sensor(0xe5,0x24);
  SP0A29_write_cmos_sensor(0xe6,0x24);
  SP0A29_write_cmos_sensor(0xe7,0x22);
  SP0A29_write_cmos_sensor(0xe8,0x22);
  SP0A29_write_cmos_sensor(0xe9,0x22);
  SP0A29_write_cmos_sensor(0xea,0x20);
  SP0A29_write_cmos_sensor(0xf3,0x20);
  SP0A29_write_cmos_sensor(0xf4,0x20);
  
  SP0A29_write_cmos_sensor(0x04,0xa0);//exp_max_indr
  SP0A29_write_cmos_sensor(0x05,0x20);//exp_min_indr
  SP0A29_write_cmos_sensor(0x0a,0xa0);//exp_max_outdr
  SP0A29_write_cmos_sensor(0x0b,0x20);//exp_min_outdr
  

////ae target
  SP0A29_write_cmos_sensor(0xfd,0x01); 
  SP0A29_write_cmos_sensor(0xeb,0x78); 
  SP0A29_write_cmos_sensor(0xec,0x74);
  SP0A29_write_cmos_sensor(0xed,0x0a);
  SP0A29_write_cmos_sensor(0xee,0x10);

////lens  shading
  SP0A29_write_cmos_sensor(0xfd,0x01);
  SP0A29_write_cmos_sensor(0x26,0x30);
  SP0A29_write_cmos_sensor(0x27,0xdc);
  SP0A29_write_cmos_sensor(0x28,0x05);
  SP0A29_write_cmos_sensor(0x29,0x08);
  SP0A29_write_cmos_sensor(0x2a,0x00);
  SP0A29_write_cmos_sensor(0x2b,0x03);
  SP0A29_write_cmos_sensor(0x2c,0x00);
  SP0A29_write_cmos_sensor(0x2d,0x2f);
  SP0A29_write_cmos_sensor(0x30,0x00);
  SP0A29_write_cmos_sensor(0x31,0x00);
  
////rgain
  SP0A29_write_cmos_sensor(0xfd,0x01);
  SP0A29_write_cmos_sensor(0xa1,0x40);//20//l
  SP0A29_write_cmos_sensor(0xa2,0x40);//20//r
  SP0A29_write_cmos_sensor(0xa3,0x2d);//1d//u
  SP0A29_write_cmos_sensor(0xa4,0x30);//20//d
  SP0A29_write_cmos_sensor(0xad,0x10);//lu
  SP0A29_write_cmos_sensor(0xae,0x0d);//ru
  SP0A29_write_cmos_sensor(0xaf,0x10);//ld
  SP0A29_write_cmos_sensor(0xb0,0x10);//rd
  
  SP0A29_write_cmos_sensor(0x18,0x05);//l2 
  SP0A29_write_cmos_sensor(0x19,0x05);//r2 
  SP0A29_write_cmos_sensor(0x1a,0x05);//u2 
  SP0A29_write_cmos_sensor(0x1b,0x05);//d2 
  SP0A29_write_cmos_sensor(0xbf,0x05);//lu2
  SP0A29_write_cmos_sensor(0xc0,0x05);//ru2
  SP0A29_write_cmos_sensor(0xc1,0x05);//ld2
  SP0A29_write_cmos_sensor(0xfa,0x00);//rd2
////ggain
  SP0A29_write_cmos_sensor(0xa5,0x40);//20//l   
  SP0A29_write_cmos_sensor(0xa6,0x40);//24//r   
  SP0A29_write_cmos_sensor(0xa7,0x30);//24//u   
  SP0A29_write_cmos_sensor(0xa8,0x28);//20//d   
  SP0A29_write_cmos_sensor(0xb1,0x12);//lu  
  SP0A29_write_cmos_sensor(0xb2,0x12);//ru  
  SP0A29_write_cmos_sensor(0xb3,0x10);//ld  
  SP0A29_write_cmos_sensor(0xb4,0x10);//rd  
                  
  SP0A29_write_cmos_sensor(0x1c,0x05);//l2  
  SP0A29_write_cmos_sensor(0x1d,0x05);//r2  
  SP0A29_write_cmos_sensor(0x1e,0x05);//u2  
  SP0A29_write_cmos_sensor(0xb9,0x05);//d2  
  SP0A29_write_cmos_sensor(0x21,0x05);//lu2 
  SP0A29_write_cmos_sensor(0x22,0x05);//ru2 
  SP0A29_write_cmos_sensor(0x23,0x05);//ld2 
  SP0A29_write_cmos_sensor(0x24,0x05);//rd2   
  
////bgain
  SP0A29_write_cmos_sensor(0xa9,0x40);//1d//l   
  SP0A29_write_cmos_sensor(0xaa,0x43);//20//r   
  SP0A29_write_cmos_sensor(0xab,0x2c);//1c//u   
  SP0A29_write_cmos_sensor(0xac,0x32);//20//d   
  SP0A29_write_cmos_sensor(0xb5,0x10);//lu  
  SP0A29_write_cmos_sensor(0xb6,0x12);//ru  
  SP0A29_write_cmos_sensor(0xb7,0x10);//ld  
  SP0A29_write_cmos_sensor(0xb8,0x10);//rd  
                  
  SP0A29_write_cmos_sensor(0xba,0x05);//l2  
  SP0A29_write_cmos_sensor(0xbc,0x05);//r2  
  SP0A29_write_cmos_sensor(0xbd,0x05);//u2  
  SP0A29_write_cmos_sensor(0xbe,0x05);//d2  
  SP0A29_write_cmos_sensor(0x25,0x05);//lu2 
  SP0A29_write_cmos_sensor(0x45,0x05);//ru2 
  SP0A29_write_cmos_sensor(0x46,0x05);//ld2 
  SP0A29_write_cmos_sensor(0x47,0x00);//rd2 
  
////awb           
  SP0A29_write_cmos_sensor(0xfd,0x01);  
  SP0A29_write_cmos_sensor(0x32,0x00);
  SP0A29_write_cmos_sensor(0xfd,0x02);
  SP0A29_write_cmos_sensor(0x26,0xa6);
  SP0A29_write_cmos_sensor(0x27,0x9a);
  SP0A29_write_cmos_sensor(0x1b,0x80);
  SP0A29_write_cmos_sensor(0x1a,0x80);
  SP0A29_write_cmos_sensor(0x18,0x27);
  SP0A29_write_cmos_sensor(0x19,0x26);
  SP0A29_write_cmos_sensor(0x2a,0x01);
  SP0A29_write_cmos_sensor(0x2b,0x10);
  SP0A29_write_cmos_sensor(0x28,0xf8);
  SP0A29_write_cmos_sensor(0x29,0x08);   
  SP0A29_write_cmos_sensor(0x10,0x20);//add b for !f
  SP0A29_write_cmos_sensor(0x11,0x09);//sub r for f
SP0A29_write_cmos_sensor(0x20,0xc8);
SP0A29_write_cmos_sensor(0x21,0x70);
SP0A29_write_cmos_sensor(0x22,0xef);
SP0A29_write_cmos_sensor(0x23,0x80);
             
 ////d65 10      
  SP0A29_write_cmos_sensor(0x66,0x3c);//35//
  SP0A29_write_cmos_sensor(0x67,0x65);//60//
  SP0A29_write_cmos_sensor(0x68,0xd0);//b0//
  SP0A29_write_cmos_sensor(0x69,0xec);//e0//
  SP0A29_write_cmos_sensor(0x6a,0xa5); 
 //indoor 11   
  SP0A29_write_cmos_sensor(0x7c,0x30);//1f 
  SP0A29_write_cmos_sensor(0x7d,0x58);//45 
  SP0A29_write_cmos_sensor(0x7e,0xf8);//fd 
  SP0A29_write_cmos_sensor(0x7f,0x20);//1b 
  SP0A29_write_cmos_sensor(0x80,0xa6);
 //cwf   12    
  SP0A29_write_cmos_sensor(0x70,0x1b);//18//2f//
  SP0A29_write_cmos_sensor(0x71,0x3b);//4a//
  SP0A29_write_cmos_sensor(0x72,0x18);//08//
  SP0A29_write_cmos_sensor(0x73,0x44);//32//24//
  SP0A29_write_cmos_sensor(0x74,0xaa); 
              
 //tl84  13    
  SP0A29_write_cmos_sensor(0x6b,0x10);//18 
  SP0A29_write_cmos_sensor(0x6c,0x2d);//4a//34//
  SP0A29_write_cmos_sensor(0x6d,0x20);//08//17//
  SP0A29_write_cmos_sensor(0x6e,0x40);//32//
  SP0A29_write_cmos_sensor(0x6f,0xaa); 
              
 //f    14     
  SP0A29_write_cmos_sensor(0x61,0xe0);//c5//10//
  SP0A29_write_cmos_sensor(0x62,0xff);//e5//38//
  SP0A29_write_cmos_sensor(0x63,0x48);//28//
  SP0A29_write_cmos_sensor(0x64,0x70);//50//
  SP0A29_write_cmos_sensor(0x65,0x6a);//
              
  SP0A29_write_cmos_sensor(0x75,0x80); 
  SP0A29_write_cmos_sensor(0x76,0x09); 
  SP0A29_write_cmos_sensor(0x77,0x02); 
  SP0A29_write_cmos_sensor(0x24,0x25); 
  SP0A29_write_cmos_sensor(0x0e,0x16); 
  SP0A29_write_cmos_sensor(0x3b,0x09);
  
 // sharp
  SP0A29_write_cmos_sensor(0xfd,0x02);  
  SP0A29_write_cmos_sensor(0xd7,0x0e);//0x182x gain 轮廓阈值
  SP0A29_write_cmos_sensor(0xd8,0x18);//0x25 4x gain
  SP0A29_write_cmos_sensor(0xd9,0x2f);//8x gain
  SP0A29_write_cmos_sensor(0xda,0x48);//16x gain
  SP0A29_write_cmos_sensor(0xdb,0x08);
  SP0A29_write_cmos_sensor(0xe8,0x48);//轮廓强度 白边锐化强度，越大锐化越强
  SP0A29_write_cmos_sensor(0xe9,0x44);
  SP0A29_write_cmos_sensor(0xea,0x30);
  SP0A29_write_cmos_sensor(0xeb,0x18);
  SP0A29_write_cmos_sensor(0xec,0x48);//轮廓强度 黑边锐化强度，越大锐化越强
  SP0A29_write_cmos_sensor(0xed,0x44);
  SP0A29_write_cmos_sensor(0xee,0x30);
  SP0A29_write_cmos_sensor(0xef,0x18);
                                    
  // sharpen
  SP0A29_write_cmos_sensor(0xfd,0x02);
  SP0A29_write_cmos_sensor(0xdc,0x04); //肤色锐化选择，值越大肤色上被锐化的区域越少
  SP0A29_write_cmos_sensor(0x05,0x6f); //
  SP0A29_write_cmos_sensor(0x09,0x10); //
  
  //dns
  SP0A29_write_cmos_sensor(0xfd,0x01);
  SP0A29_write_cmos_sensor(0x64,0x76); ////沿方向边缘平滑力度  //0-最强，8-最弱//44
  SP0A29_write_cmos_sensor(0x65,0x21);//22
  
  SP0A29_write_cmos_sensor(0x6d,0x08); //强平滑（平坦）区域平滑阈值
  SP0A29_write_cmos_sensor(0x6e,0x0c); 
  SP0A29_write_cmos_sensor(0x6f,0x14);  
  SP0A29_write_cmos_sensor(0x70,0x28); 
  SP0A29_write_cmos_sensor(0x71,0x06); //弱轮廓（非平坦）区域平滑阈值	
  SP0A29_write_cmos_sensor(0x72,0x09);
  SP0A29_write_cmos_sensor(0x73,0x12);
  SP0A29_write_cmos_sensor(0x74,0x24);

  SP0A29_write_cmos_sensor(0x75,0x44); ////[7:4]平坦区域强度，[3:0]非平坦区域强度；0-最强，8-最弱；
  SP0A29_write_cmos_sensor(0x76,0x02); 
  SP0A29_write_cmos_sensor(0x77,0x02); 
  SP0A29_write_cmos_sensor(0x78,0x02); 
  SP0A29_write_cmos_sensor(0x81,0x10); //2x根据增益判定区域阈值
  SP0A29_write_cmos_sensor(0x82,0x1c); //4x
  SP0A29_write_cmos_sensor(0x83,0x34); //8x
  SP0A29_write_cmos_sensor(0x84,0x48); //16x
  SP0A29_write_cmos_sensor(0x85,0x0a); // 12/8+reg0x81 
   
   
//gamma       
  SP0A29_write_cmos_sensor(0xfd,0x01);                
  SP0A29_write_cmos_sensor(0x8b,0x00);//00//00//00//00//00//00//  
  SP0A29_write_cmos_sensor(0x8c,0x0d);//07//02//11//0f//0f//11//  
  SP0A29_write_cmos_sensor(0x8d,0x18);//10//0a//24//1d//21//21//  
  SP0A29_write_cmos_sensor(0x8e,0x26);//1b//13//3f//36//3b//43//  
  SP0A29_write_cmos_sensor(0x8f,0x32);//27//1d//64//5a//60//67//  
  SP0A29_write_cmos_sensor(0x90,0x45);//3b//30//7f//75//7c//82//  
  SP0A29_write_cmos_sensor(0x91,0x57);//4b//40//93//8c//98//98//  
  SP0A29_write_cmos_sensor(0x92,0x67);//5b//4e//a4//9e//ae//a7//  
  SP0A29_write_cmos_sensor(0x93,0x77);//6a//5a//b2//ab//c3//b4//  
  SP0A29_write_cmos_sensor(0x94,0x8e);//83//71//bb//b8//d1//be//  
  SP0A29_write_cmos_sensor(0x95,0xa2);//95//85//c4//c2//da//c5//  
  SP0A29_write_cmos_sensor(0x96,0xb2);//a6//96//cb//ca//e0//cb//  
  SP0A29_write_cmos_sensor(0x97,0xbf);//b5//a6//d2//d0//e6//d1//  
  SP0A29_write_cmos_sensor(0x98,0xca);//c1//b3//d9//d5//ea//d5//  
  SP0A29_write_cmos_sensor(0x99,0xd2);//ca//c0//e0//dc//ef//dc//  
  SP0A29_write_cmos_sensor(0x9a,0xd9);//d2//cb//e8//e2//f4//e2//  
  SP0A29_write_cmos_sensor(0x9b,0xe1);//db//d5//ee//e9//f7//e9//  
  SP0A29_write_cmos_sensor(0x9c,0xe7);//e3//df//01//01//01//01//  
  SP0A29_write_cmos_sensor(0x9d,0xec);//eb//e9//f3//f2//fb//f2//  
  SP0A29_write_cmos_sensor(0x9e,0xf2);//f3//f2//f9//ff//ff//ff//  
  SP0A29_write_cmos_sensor(0x9f,0xf8);//fa//fa//      
  SP0A29_write_cmos_sensor(0xa0,0xff);//ff//ff//  
  //ccm
  SP0A29_write_cmos_sensor(0xfd,0x02);
  SP0A29_write_cmos_sensor(0x15,0xb8);//b>th  c1
  SP0A29_write_cmos_sensor(0x16,0x7c);//r<th  70
  //!f
  SP0A29_write_cmos_sensor(0xa0,0x79);//8c//99
  SP0A29_write_cmos_sensor(0xa1,0xfa);//fa//0c
  SP0A29_write_cmos_sensor(0xa2,0x0c);//fa//da
  SP0A29_write_cmos_sensor(0xa3,0x00);//f4//00
  SP0A29_write_cmos_sensor(0xa4,0x99);//99//8c
  SP0A29_write_cmos_sensor(0xa5,0xe7);//f4//f4
  SP0A29_write_cmos_sensor(0xa6,0x0c);//00//00
  SP0A29_write_cmos_sensor(0xa7,0xda);//b4//e7
  SP0A29_write_cmos_sensor(0xa8,0x99);//cc//99
  SP0A29_write_cmos_sensor(0xa9,0x0c);//3c//30
  SP0A29_write_cmos_sensor(0xaa,0x30);//33//30
  SP0A29_write_cmos_sensor(0xab,0x0c);//0c//0c
  //f    
  SP0A29_write_cmos_sensor(0xac,0x80);//80//99
  SP0A29_write_cmos_sensor(0xad,0x26);//00//26
  SP0A29_write_cmos_sensor(0xae,0xda);//00//c0
  SP0A29_write_cmos_sensor(0xaf,0xed);//e7//00
  SP0A29_write_cmos_sensor(0xb0,0xa6);//c0//a6
  SP0A29_write_cmos_sensor(0xb1,0xed);//da//da
  SP0A29_write_cmos_sensor(0xb2,0xe7);//e7//ed
  SP0A29_write_cmos_sensor(0xb3,0xcd);//b4//da
  SP0A29_write_cmos_sensor(0xb4,0xcc);//e6//b9
  SP0A29_write_cmos_sensor(0xb5,0x30);//00//30
  SP0A29_write_cmos_sensor(0xb6,0x33);//33//30
  SP0A29_write_cmos_sensor(0xb7,0x0f);//0f//0f
      
  //sat u 
  SP0A29_write_cmos_sensor(0xfd,0x01);
  SP0A29_write_cmos_sensor(0xd3,0xa0);
  SP0A29_write_cmos_sensor(0xd4,0x90);
  SP0A29_write_cmos_sensor(0xd5,0x68);
  SP0A29_write_cmos_sensor(0xd6,0x50);//78
  //sat v     
  SP0A29_write_cmos_sensor(0xd7,0xa4);
  SP0A29_write_cmos_sensor(0xd8,0x94);
  SP0A29_write_cmos_sensor(0xd9,0x6c);
  SP0A29_write_cmos_sensor(0xda,0x54);
  
  //auto_sat
  SP0A29_write_cmos_sensor(0xfd,0x01);
  SP0A29_write_cmos_sensor(0xd2,0x00);		//autosa_en
  
  //uv_th
  SP0A29_write_cmos_sensor(0xfd,0x01); //
  SP0A29_write_cmos_sensor(0xc2,0xee);
  SP0A29_write_cmos_sensor(0xc3,0xee);
  SP0A29_write_cmos_sensor(0xc4,0xdd);
  SP0A29_write_cmos_sensor(0xc5,0xbb);
   
  //low_lum_offset
  SP0A29_write_cmos_sensor(0xfd,0x01);
  SP0A29_write_cmos_sensor(0xcd,0x10);
  SP0A29_write_cmos_sensor(0xce,0x1f);
  SP0A29_write_cmos_sensor(0xcf,0x30);  
  SP0A29_write_cmos_sensor(0xd0,0x45);   

  
  //gw
  SP0A29_write_cmos_sensor(0xfd,0x02);
  SP0A29_write_cmos_sensor(0x30,0x40);//gw_mean_th
  SP0A29_write_cmos_sensor(0x31,0x70);//gw_offset
  SP0A29_write_cmos_sensor(0x32,0x60);//gw_y_bot
  SP0A29_write_cmos_sensor(0x33,0xef);//gw_y_top
  SP0A29_write_cmos_sensor(0x34,0x04);//gw_uv_radius
  SP0A29_write_cmos_sensor(0x35,0x4c);//40uv_fix_dat
  SP0A29_write_cmos_sensor(0x36,0x20);//y_offset_th
  SP0A29_write_cmos_sensor(0x37,0x13);//gw_en_sel
//heq
  SP0A29_write_cmos_sensor(0xfd,0x01);
  SP0A29_write_cmos_sensor(0x0e,0x80); //heq_mean
  SP0A29_write_cmos_sensor(0x10,0x80); //SP0A29_write_cmos_sensor(u outdoor
  SP0A29_write_cmos_sensor(0x11,0x80); //SP0A29_write_cmos_sensor(u nor
  SP0A29_write_cmos_sensor(0x12,0x80); //SP0A29_write_cmos_sensor(u dummy
  SP0A29_write_cmos_sensor(0x13,0x80); //SP0A29_write_cmos_sensor(u low
  SP0A29_write_cmos_sensor(0x14,0x9c); //SP0A29_write_cmos_sensor(l outdoor  
  SP0A29_write_cmos_sensor(0x15,0x98);//SP0A29_write_cmos_sensor(l nor      
  SP0A29_write_cmos_sensor(0x16,0x98); //SP0A29_write_cmos_sensor(l dummy    
  SP0A29_write_cmos_sensor(0x17,0x98); //SP0A29_write_cmos_sensor(l low      
  
  
  //ycnr
  SP0A29_write_cmos_sensor(0xfd,0x02);
  SP0A29_write_cmos_sensor(0x8c,0x80);
  SP0A29_write_cmos_sensor(0x8d,0x80);
  SP0A29_write_cmos_sensor(0x8e,0x80);
  SP0A29_write_cmos_sensor(0x8f,0x80);
  SP0A29_write_cmos_sensor(0x90,0x80);
  SP0A29_write_cmos_sensor(0x91,0x80);
  SP0A29_write_cmos_sensor(0x92,0x80);
  SP0A29_write_cmos_sensor(0x93,0x80);
  
  //auto
  SP0A29_write_cmos_sensor(0xfd,0x00);		  	
  SP0A29_write_cmos_sensor(0x31,0x10);
  SP0A29_write_cmos_sensor(0xfd,0x01);  
  SP0A29_write_cmos_sensor(0x00,0x00); 	//fix mode   
  SP0A29_write_cmos_sensor(0x32,0x15);//		//ae en
  SP0A29_write_cmos_sensor(0x33,0xef);		//lsc\bpc en
  SP0A29_write_cmos_sensor(0x34,0xc7);		//ynr\cnr\gamma\color en
  SP0A29_write_cmos_sensor(0x35,0x01);	//data format
  SP0A29_write_cmos_sensor(0xfd,0x00);	
}


/*****************************************************************************/
/* Windows Mobile Sensor Interface */
/*****************************************************************************/
/*************************************************************************
* FUNCTION
*	SP0A29Open
*
* DESCRIPTION
*	This function initialize the registers of CMOS sensor
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 SP0A29Open(void)

{
	kal_uint16 sensor_id=0; 
	int retry = 10; 

    SENSORDB("SP0A29Open_start \n");

//	SP0A29_Sensor_Driver.i2c_clit.addr=SP0A29_WRITE_ID;
//	SP0A29_Sensor_Driver.i2c_clit = i2c_clit;
//    SP0A29_Sensor_Driver.i2c_clit->addr = SP0A29_WRITE_ID;

#if 0 
	SP0A29_write_cmos_sensor(0x12, 0x80);
	mDELAY(10);
#endif 

	// check if sensor ID correct
	do {
		
		SP0A29_write_cmos_sensor(0xfd,0x00);
	    sensor_id=SP0A29_read_cmos_sensor(0x02);
    	    if (sensor_id == SP0A29_SENSOR_ID) {
                 break; 
    	    }
        
    	    
    	    retry--; 
	}while (retry > 0); 
	
	if (sensor_id != SP0A29_SENSOR_ID) {
         SENSORDB("SP0A29 Read Sensor ID Fail = 0x%x\n", sensor_id); 
	    return ERROR_SENSOR_CONNECT_FAIL;
	}

   SENSORDB("SP0A29 Read Sensor ID OK \n"); 

  memset(&SP0A29_Sensor_Driver, 0, sizeof(struct SP0A29_Sensor_Struct)); 
	SP0A29_Sensor_Driver.MPEG4_encode_mode=KAL_FALSE;
	SP0A29_Sensor_Driver.dummy_pixels=0;
	SP0A29_Sensor_Driver.dummy_lines=0;
	SP0A29_Sensor_Driver.extra_exposure_lines=0;
	SP0A29_Sensor_Driver.exposure_lines=0;
	SP0A29_Sensor_Driver.MODE_CAPTURE=KAL_FALSE;
		
	SP0A29_Sensor_Driver.bNight_mode =KAL_FALSE; // to distinguish night mode or auto mode, default: auto mode setting
	SP0A29_Sensor_Driver.bBanding_value = AE_FLICKER_MODE_50HZ; // to distinguish between 50HZ and 60HZ.
		
	SP0A29_Sensor_Driver.fPV_PCLK = 24; //26;
	SP0A29_Sensor_Driver.iPV_Pixels_Per_Line = 0;

//	SP0A29_set_isp_driving_current(1);
	// initail sequence write in

    mDELAY(10);


#ifdef DEBUG_SENSOR  //gepeiwei   120903
			//判断手机对应目录下是否有名为sp2528_sd 的文件,没有默认参数
	
			//介于各种原因，本版本初始化参数在_s_fmt中。
			struct file *fp; 
			mm_segment_t fs; 
			loff_t pos = 0; 
			//static char buf[10*1024] ;
	
			printk("SP0A29 Open File Start\n");
			fp = filp_open("/storage/sdcard1/sp0a29_sd.dat", O_RDONLY , 0); 
			if (IS_ERR(fp)) 
			{ 
				printk("open file error 0\n");
				fp = filp_open("/storage/sdcard0/sp0a29_sd.dat", O_RDONLY , 0); 
				if (IS_ERR(fp)) { 
					fromsd = 0;   
					printk("open file error 1\n");
				}
				else{
					printk("open file success 1\n");
					fromsd = 1;
					filp_close(fp, NULL); 
				}
			} 
			else 
			{
				printk("open file success 0\n");
				fromsd = 1;
				//SP0A29_Initialize_from_T_Flash();
				filp_close(fp, NULL); 
			}
			//set_fs(fs);
			if(fromsd == 1)//是否从SD读取//gepeiwei   120903
				SP0A29_Initialize_from_T_Flash();//从SD卡读取的主要函数
			else
				SP0A29_Sensor_Driver_Init();
#else
	SP0A29_Sensor_Driver_Init();
#endif
    return ERROR_NONE;
}   /* SP0A29Open  */



/*************************************************************************
* FUNCTION
*	SP0A29_GetSensorID
*
* DESCRIPTION
*	This function get the sensor ID
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 SP0A29_GetSensorID(UINT32 *sensorID)

{
	
	SENSORDB("SP0A29GetSensorID\n");	
	//	Read sensor ID to adjust I2C is OK?
    	SP0A29_write_cmos_sensor(0xfd,0x00);
	*sensorID = SP0A29_read_cmos_sensor(0x02);
	SENSORDB("SP0A29 Sensor Read IDtangzibo= %x\n",*sensorID);
	SENSORDB("SP0A29 Sensor Read IDtangzibo SP0A29_SENSOR_ID= %x\n",SP0A29_SENSOR_ID);
	if (*sensorID != SP0A29_SENSOR_ID) 
	{
	  return ERROR_SENSOR_CONNECT_FAIL;
	}
	
	return ERROR_NONE;
}


/*************************************************************************
* FUNCTION
*	SP0A29Close
*
* DESCRIPTION
*	This function is to turn off sensor module power.
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 SP0A29Close(void)
{
	kal_uint8 tmp1;
   // tmp1 = closed;
	//CAMERA_CONTROL_FLOW(tmp1,closed++);
   SENSORDB("SP0A29Close\n");
	return ERROR_NONE;
}   /* SP0A29Close */




static void SP0A29_HVMirror(ACDK_SENSOR_IMAGE_MIRROR_ENUM SensorImageMirror)
{
	//volatile kal_uint32 temp_reg2=SP0A29_read_cmos_sensor(0x1E), temp_reg1=(temp_reg2&0x0F);
	kal_uint32 iTemp;
	
	kal_uint32 iTemp2;
		
		SP0A29_write_cmos_sensor(0xfd,0x00);
	iTemp =  SP0A29_read_cmos_sensor(0x31);
	#if 0
#if defined(AGOLD_SP0A29_YUV_HV_MIRROR) //[Agold][xxd]
	SensorImageMirror  = IMAGE_HV_MIRROR;
#elif defined(AGOLD_SP0A29_YUV_H_MIRROR)
	SensorImageMirror   = IMAGE_H_MIRROR;
#elif defined(AGOLD_SP0A29_YUV_V_MIRROR)
	SensorImageMirror   = IMAGE_V_MIRROR;
#endif 

        SensorImageMirror ^= IMAGE_HV_MIRROR; //[Agold][xxd][add for qq video rotate 180]   
      #endif
	iTemp2= iTemp;
	switch (SensorImageMirror)
	{
		case IMAGE_NORMAL:
			SP0A29_write_cmos_sensor(0xfd,0x00);
			SP0A29_write_cmos_sensor(0x31,iTemp2);

			
			break;
		case IMAGE_H_MIRROR:			 

			if((iTemp2 & 0x20)==0x20)
				iTemp2 &= (~0x20);
			else
				iTemp2 |= 0x20;
			
			SP0A29_write_cmos_sensor(0xfd,0x00);
			SP0A29_write_cmos_sensor(0x31,iTemp2);
			break;
		case IMAGE_V_MIRROR:			 
			if((iTemp2 & 0x40)==0x40)
				iTemp2 &= (~0x40);
			else
				iTemp2 |= 0x40;
			
			SP0A29_write_cmos_sensor(0xfd,0x00);
			SP0A29_write_cmos_sensor(0x31,iTemp2);
			break;
		case IMAGE_HV_MIRROR:						 

			if((iTemp2 & 0x20)==0x20)
				iTemp2 &= (~0x20);
			else
				iTemp2 |= 0x20;

			if((iTemp2 & 0x40)==0x40)
				iTemp2 &= (~0x40);
			else
				iTemp2 |= 0x40;

			SP0A29_write_cmos_sensor(0xfd,0x00);
			SP0A29_write_cmos_sensor(0x31,iTemp2);
			break;
	}


}
/*************************************************************************
* FUNCTION
* SP0A29_Preview
*
* DESCRIPTION
*	This function start the sensor preview.
*
* PARAMETERS
*	*image_window : address pointer of pixel numbers in one period of HSYNC
*  *sensor_config_data : address pointer of line numbers in one period of VSYNC
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 SP0A29_Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)

{
	SP0A29_Sensor_Driver.fPV_PCLK=24000000;//26000000
	SP0A29_Sensor_Driver.MODE_CAPTURE=KAL_FALSE;

	if(sensor_config_data->SensorOperationMode==MSDK_SENSOR_OPERATION_MODE_VIDEO){
		SP0A29_Sensor_Driver.MPEG4_encode_mode = KAL_TRUE;  // MPEG4 Encode Mode
	}else{
		SP0A29_Sensor_Driver.MPEG4_encode_mode = KAL_FALSE;  
	}


	// SP0A29_HVMirror(sensor_config_data->SensorImageMirror);

//	SP0A29_Sensor_Driver.dummy_pixels = 0;
//	SP0A29_Sensor_Driver.dummy_lines = 42;
//	SP0A29_Sensor_Driver.iPV_Pixels_Per_Line =VGA_PERIOD_PIXEL_NUMS+SP0A29_Sensor_Driver.dummy_pixels;  
//	SP0A29_Set_Dummy(SP0A29_Sensor_Driver.dummy_pixels, SP0A28_Sensor_Driver.dummy_lines);

	
	image_window->GrabStartX= IMAGE_SENSOR_VGA_INSERTED_PIXELS;
	image_window->GrabStartY= IMAGE_SENSOR_VGA_INSERTED_LINES;
	image_window->ExposureWindowWidth = IMAGE_SENSOR_PV_WIDTH;
	image_window->ExposureWindowHeight =IMAGE_SENSOR_PV_HEIGHT;

	if(KAL_TRUE == SP0A29_Sensor_Driver.bNight_mode) // for nd 128 noise,decrease color matrix
	{
	}

	// copy sensor_config_data
	memcpy(&SP0A29SensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	return ERROR_NONE;

}   /*  SP0A29_Preview   */

/*************************************************************************
* FUNCTION
*	SP0A29_Capture
*
* DESCRIPTION
*	This function setup the CMOS sensor in capture MY_OUTPUT mode
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
#if 0
static kal_uint32 SP0A29_Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
						  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)

{
}   /* OV7576_Capture() */
#endif

static kal_uint32 SP0A29GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
kal_uint8 tmp1;
//    tmp1 = res;
//	CAMERA_CONTROL_FLOW(tmp1,res++);

	pSensorResolution->SensorFullWidth=IMAGE_SENSOR_FULL_WIDTH;
	pSensorResolution->SensorFullHeight=IMAGE_SENSOR_FULL_HEIGHT;
    pSensorResolution->SensorPreviewWidth=IMAGE_SENSOR_PV_WIDTH;
	pSensorResolution->SensorPreviewHeight=IMAGE_SENSOR_PV_HEIGHT;
	pSensorResolution->SensorVideoWidth=IMAGE_SENSOR_FULL_WIDTH;
	pSensorResolution->SensorVideoHeight=IMAGE_SENSOR_FULL_HEIGHT;

	return ERROR_NONE;
}	/* SP0A29GetResolution() */

static kal_uint32 SP0A29GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
					  MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	
		SENSORDB("SP0A29GetInfo \n");
		pSensorInfo->SensorPreviewResolutionX = IMAGE_SENSOR_PV_WIDTH;
		pSensorInfo->SensorPreviewResolutionY = IMAGE_SENSOR_PV_HEIGHT;
		pSensorInfo->SensorFullResolutionX = IMAGE_SENSOR_PV_WIDTH;
		pSensorInfo->SensorFullResolutionY = IMAGE_SENSOR_PV_HEIGHT;
	
		pSensorInfo->SensorCameraPreviewFrameRate=30;
		pSensorInfo->SensorVideoFrameRate=30;
		pSensorInfo->SensorStillCaptureFrameRate=30;
		pSensorInfo->SensorWebCamCaptureFrameRate=30;
		pSensorInfo->SensorResetActiveHigh=FALSE;
		pSensorInfo->SensorResetDelayCount=1;
		pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;
	
		pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_YUYV;
		pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;
		pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
		pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
		pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_HIGH;
		 //  pSensorInfo->SensorDriver3D = 0;   // the sensor driver is 2D
		pSensorInfo->SensorInterruptDelayLines = 1;
		pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_MIPI;

		pSensorInfo->CaptureDelayFrame = 3;
		pSensorInfo->PreviewDelayFrame = 3; 
		pSensorInfo->VideoDelayFrame = 3; 	
		pSensorInfo->SensorMasterClockSwitch = 0; 
	
	
		switch (ScenarioId)
		{
			case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			//case MSDK_SCENARIO_ID_CAMERA_PREVIEW://MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
				 
			case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
				
			//case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
			default:			
				pSensorInfo->SensorClockFreq=24;//26;
				pSensorInfo->SensorClockDividCount= 3;
				pSensorInfo->SensorClockRisingCount= 0;
				pSensorInfo->SensorClockFallingCount= 2;
				pSensorInfo->SensorPixelClockCount= 3;
				pSensorInfo->SensorDataLatchCount= 2;
				pSensorInfo->SensorGrabStartX = 1; 
				pSensorInfo->SensorGrabStartY = 1;		   
				break;
		}

	memcpy(pSensorConfigData, &SP0A29SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	
	return ERROR_NONE;
}	/* SP0A29GetInfo() */


static kal_uint32 SP0A29Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	CAMERA_CONTROL_FLOW(ScenarioId,ScenarioId);

	switch (ScenarioId)
	{
		//case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW://MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
			SP0A29_Preview(pImageWindow, pSensorConfigData);
		break;
		//case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG://MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
			//SP0A29_Capture(pImageWindow, pSensorConfigData);
			SP0A29_Preview(pImageWindow, pSensorConfigData);
		break;
		default:
			return ERROR_INVALID_SCENARIO_ID;
	}
	return TRUE;
}	/* MT9P012Control() */



static BOOL SP0A29_set_param_wb(UINT16 para)
{
	kal_uint8  temp_reg;

	if(SP0A29_Sensor_Driver.u8Wb_value==para)
		return FALSE;

	
	SP0A29_Sensor_Driver.u8Wb_value = para;

	switch (para)
	 {
		 case AWB_MODE_OFF:
		 //SP0A29_write_cmos_sensor(0xfd,0x00);				   
		 //SP0A29_write_cmos_sensor(0x32,0x05);	   
		 break;
			 
		 case AWB_MODE_AUTO:
		 SP0A29_write_cmos_sensor(0xfd,0x02);															
		 SP0A29_write_cmos_sensor(0x26,0xa6);																
		 SP0A29_write_cmos_sensor(0x27,0x9a);
		 SP0A29_write_cmos_sensor(0xfd,0x01);	// AUTO 3000K~7000K 	   
		 SP0A29_write_cmos_sensor(0x32,0x15);
		 SP0A29_write_cmos_sensor(0xfd,0x00);
			 break;
	
		 case AWB_MODE_CLOUDY_DAYLIGHT: //cloudy
		 SP0A29_write_cmos_sensor(0xfd,0x01);	 //7000K									 
		 SP0A29_write_cmos_sensor(0x32,0x05);															
		 SP0A29_write_cmos_sensor(0xfd,0x02);															
		 SP0A29_write_cmos_sensor(0x26,0xdb);																
		 SP0A29_write_cmos_sensor(0x27,0x63);																
		 SP0A29_write_cmos_sensor(0xfd,0x00);											   
			 break;
	
		 case AWB_MODE_DAYLIGHT: //sunny
		SP0A29_write_cmos_sensor(0xfd,0x01);	//6500K 									
		 SP0A29_write_cmos_sensor(0x32,0x05);															
		 SP0A29_write_cmos_sensor(0xfd,0x02);															
		 SP0A29_write_cmos_sensor(0x26,0xca);																
		 SP0A29_write_cmos_sensor(0x27,0x73);																
		 SP0A29_write_cmos_sensor(0xfd,0x00);														   
			 break;
	
		 case AWB_MODE_INCANDESCENT: //office
			 // SP0A29_reg_WB_auto 
		 SP0A29_write_cmos_sensor(0xfd,0x01);	//2800K~3000K									  
		 SP0A29_write_cmos_sensor(0x32,0x05);															
		 SP0A29_write_cmos_sensor(0xfd,0x02);															
		 SP0A29_write_cmos_sensor(0x26,0x8c);																
		 SP0A29_write_cmos_sensor(0x27,0xb3);																
		 SP0A29_write_cmos_sensor(0xfd,0x00);															
			 break;
	
		 case AWB_MODE_TUNGSTEN: //home
		 // SP0A29_reg_WB_auto 
		 SP0A29_write_cmos_sensor(0xfd,0x01);	//4000K 								  
		 SP0A29_write_cmos_sensor(0x32,0x05);															
		 SP0A29_write_cmos_sensor(0xfd,0x02);															
		 SP0A29_write_cmos_sensor(0x26,0xb3);																
		 SP0A29_write_cmos_sensor(0x27,0x8e);																
		 SP0A29_write_cmos_sensor(0xfd,0x00);														   
			 break;
			 
		 case AWB_MODE_FLUORESCENT:
		 // SP0A29_reg_WB_auto 
		 SP0A29_write_cmos_sensor(0xfd,0x01);	//4000K 								  
		 SP0A29_write_cmos_sensor(0x32,0x05);															
		 SP0A29_write_cmos_sensor(0xfd,0x02);															
		 SP0A29_write_cmos_sensor(0x26,0x95);																
		 SP0A29_write_cmos_sensor(0x27,0x9c);																
		 SP0A29_write_cmos_sensor(0xfd,0x00);														   
			 break;
	
		 default:
			 return FALSE;
	 }


	return TRUE;
} /* SP0A29_set_param_wb */


static BOOL SP0A29_set_param_effect(UINT16 para)
{
	kal_uint32 ret = KAL_TRUE;

	if(para==SP0A29_Sensor_Driver.u8Effect_value)
		return FALSE;

	
	SP0A29_Sensor_Driver.u8Effect_value = para;
    switch (para)
    {
        case MEFFECT_OFF:  
			SP0A29_write_cmos_sensor(0xfd,0x01);
			SP0A29_write_cmos_sensor(0x66,0x00);
			SP0A29_write_cmos_sensor(0x67,0x80);
			SP0A29_write_cmos_sensor(0x68,0x80);
			SP0A29_write_cmos_sensor(0xdb,0x00);
			SP0A29_write_cmos_sensor(0x34,0xc7);
			SP0A29_write_cmos_sensor(0xfd,0x02);
			SP0A29_write_cmos_sensor(0x14,0x00);
            break;

        case MEFFECT_SEPIA:  
			SP0A29_write_cmos_sensor(0xfd,0x01);
			SP0A29_write_cmos_sensor(0x66,0x10);
			SP0A29_write_cmos_sensor(0x67,0x98);
			SP0A29_write_cmos_sensor(0x68,0x58);
			SP0A29_write_cmos_sensor(0xdb,0x00);
			SP0A29_write_cmos_sensor(0x34,0xc7);
			SP0A29_write_cmos_sensor(0xfd,0x02);
			SP0A29_write_cmos_sensor(0x14,0x00);

            break;

        case MEFFECT_NEGATIVE: 
			SP0A29_write_cmos_sensor(0xfd,0x01);
			SP0A29_write_cmos_sensor(0x66,0x08);
			SP0A29_write_cmos_sensor(0x67,0x80);
			SP0A29_write_cmos_sensor(0x68,0x80);
			SP0A29_write_cmos_sensor(0xdb,0x00);
			SP0A29_write_cmos_sensor(0x34,0xc7);
			SP0A29_write_cmos_sensor(0xfd,0x02);
			SP0A29_write_cmos_sensor(0x14,0x00);
            break;

        case MEFFECT_SEPIAGREEN:
			SP0A29_write_cmos_sensor(0xfd,0x01);
			SP0A29_write_cmos_sensor(0x66,0x10);
			SP0A29_write_cmos_sensor(0x67,0x50);
			SP0A29_write_cmos_sensor(0x68,0x50);
			SP0A29_write_cmos_sensor(0xdb,0x00);
			SP0A29_write_cmos_sensor(0x34,0xc7);
			SP0A29_write_cmos_sensor(0xfd,0x02);
			SP0A29_write_cmos_sensor(0x14,0x00);
            break;

        case MEFFECT_SEPIABLUE:
			SP0A29_write_cmos_sensor(0xfd,0x01);
			SP0A29_write_cmos_sensor(0x66,0x10);
			SP0A29_write_cmos_sensor(0x67,0x80);
			SP0A29_write_cmos_sensor(0x68,0xb0);
			SP0A29_write_cmos_sensor(0xdb,0x00);
			SP0A29_write_cmos_sensor(0x34,0xc7);
			SP0A29_write_cmos_sensor(0xfd,0x02);
			SP0A29_write_cmos_sensor(0x14,0x00);

            break;
			
		case MEFFECT_MONO: //B&W
			SP0A29_write_cmos_sensor(0xfd,0x01);
			SP0A29_write_cmos_sensor(0x66,0x20);
			SP0A29_write_cmos_sensor(0x67,0x80);
			SP0A29_write_cmos_sensor(0x68,0x80);
			SP0A29_write_cmos_sensor(0xdb,0x00);
			SP0A29_write_cmos_sensor(0x34,0xc7);
			SP0A29_write_cmos_sensor(0xfd,0x02);
			SP0A29_write_cmos_sensor(0x14,0x00);

			break;
        default:
            return FALSE;
    }

	return ret;

} /* SP0A29_set_param_effect */

static void SP0A29_set_banding_for_50Hz(void)
{
printk("SP0A29_set_banding_for_50Hz\n");

}


static void SP0A29_set_banding_for_60Hz(void)
{
printk("SP0A29_set_banding_for_60Hz\n");

}

static BOOL SP0A29_set_param_banding(UINT16 para)
{
	//if(SP0A29_Sensor_Driver.bBanding_value == para)
	//	return TRUE;
	
	SP0A29_Sensor_Driver.bBanding_value = para;
	
	switch (para)
	{
		case AE_FLICKER_MODE_50HZ:
			isBanding = 0;
			printk("SP0A29_set_param_banding_50hz\n");
			//SP0A29_set_banding_for_50Hz();
			break;
		case AE_FLICKER_MODE_60HZ:
			isBanding = 1;
			printk("SP0A29_set_param_banding_60hz\n");
			//SP0A29_set_banding_for_60Hz();
			break;
		default:
			return FALSE;
	}

	return TRUE;
} /* SP0A29_set_param_banding */
static BOOL SP0A29_set_param_exposure(UINT16 para)
{
	if(para == SP0A29_Sensor_Driver.u8Ev_value)
		return FALSE;

	SP0A29_Sensor_Driver.u8Ev_value = para;

    switch (para)
    {
        case AE_EV_COMP_n13:
		SP0A29_write_cmos_sensor(0xfd, 0x01);
		SP0A29_write_cmos_sensor(0xdb, 0xc0);
            break;

        case AE_EV_COMP_n10:
		SP0A29_write_cmos_sensor(0xfd, 0x01);
		SP0A29_write_cmos_sensor(0xdb, 0xd0);
            break;

        case AE_EV_COMP_n07:
		SP0A29_write_cmos_sensor(0xfd, 0x01);
		SP0A29_write_cmos_sensor(0xdb, 0xe0);
            break;

        case AE_EV_COMP_n03:
		SP0A29_write_cmos_sensor(0xfd, 0x01);
		SP0A29_write_cmos_sensor(0xdb, 0xf0);
            break;

        case AE_EV_COMP_00:
		SP0A29_write_cmos_sensor(0xfd, 0x01);
		SP0A29_write_cmos_sensor(0xdb, 0x00);//0xfa before
            break;

        case AE_EV_COMP_03:
		SP0A29_write_cmos_sensor(0xfd, 0x01);
		SP0A29_write_cmos_sensor(0xdb, 0x10);
            break;

        case AE_EV_COMP_07:
		SP0A29_write_cmos_sensor(0xfd, 0x01);
		SP0A29_write_cmos_sensor(0xdb, 0x20);
            break;

        case AE_EV_COMP_10:
		SP0A29_write_cmos_sensor(0xfd, 0x01);
		SP0A29_write_cmos_sensor(0xdb, 0x30);
            break;

        case AE_EV_COMP_13:
		SP0A29_write_cmos_sensor(0xfd, 0x01);
		SP0A29_write_cmos_sensor(0xdb, 0x40);
            break;

        default:
            return FALSE;
    }


	return TRUE;
} /* SP0A29_set_param_exposure */

static kal_uint32 SP0A29_YUVSensorSetting(FEATURE_ID iCmd, UINT16 iPara)
{
#ifdef DEBUG_SENSOR
	return TRUE;
#endif
	switch (iCmd) {
		case FID_SCENE_MODE:
		    if (iPara == SCENE_MODE_OFF){
		        SP0A29_night_mode(FALSE); 
		    }else if (iPara == SCENE_MODE_NIGHTSCENE){
               SP0A29_night_mode(TRUE); 
		    }	    
		  
		    break; 
		case FID_AWB_MODE:
			SP0A29_set_param_wb(iPara);
		break;
		case FID_COLOR_EFFECT:
			SP0A29_set_param_effect(iPara);
		break;
		case FID_AE_EV:	
			SP0A29_set_param_exposure(iPara);
		break;
		case FID_AE_FLICKER:
			SP0A29_set_param_banding(iPara);
			//whl120717 test
			 if (SP0A29_Sensor_Driver.bNight_mode == KAL_FALSE){
		        SP0A29_night_mode(FALSE); 
		    }else if (SP0A29_Sensor_Driver.bNight_mode == KAL_TRUE){
               	SP0A29_night_mode(TRUE); 
        
		    }	
		      
		break;
		default:
		break;
	}
	
	return TRUE;
}   /* SP0A29_YUVSensorSetting */

static kal_uint32 SP0A29_YUVSetVideoMode(UINT16 u2FrameRate)
{
    kal_uint8 temp ;//= SP0A29_read_cmos_sensor(0x3B);
    SP0A29_Sensor_Driver.MPEG4_encode_mode = KAL_TRUE; 

    if (u2FrameRate == 30)
    {
    }
    else if (u2FrameRate == 15)       
    {
    }
    else 
    {
        printk("Wrong frame rate setting \n");
    }   
    
	printk("\n SP0A29_YUVSetVideoMode:u2FrameRate=%d\n\n",u2FrameRate);
    return TRUE;
}

UINT32 SP0A29SetSoftwarePWDNMode(kal_bool bEnable)
{
#if 0
    SENSORDB("[SP0A29SetSoftwarePWDNMode] Software Power down enable:%d\n", bEnable);
    
    if(bEnable) {   // enable software sleep mode   
	 SP0A29_write_cmos_sensor(0x09, 0x10);
    } else {
        SP0A29_write_cmos_sensor(0x09, 0x03);  
    }
#endif
    return TRUE;
}

/*************************************************************************
* FUNCTION
*    SP0A29_get_size
*
* DESCRIPTION
*    This function return the image width and height of image sensor.
*
* PARAMETERS
*    *sensor_width: address pointer of horizontal effect pixels of image sensor
*    *sensor_height: address pointer of vertical effect pixels of image sensor
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void SP0A29_get_size(kal_uint16 *sensor_width, kal_uint16 *sensor_height)
{
  *sensor_width = IMAGE_SENSOR_FULL_WIDTH; /* must be 4:3 */
  *sensor_height = IMAGE_SENSOR_FULL_HEIGHT;
}

/*************************************************************************
* FUNCTION
*    SP0A29_get_period
*
* DESCRIPTION
*    This function return the image width and height of image sensor.
*
* PARAMETERS
*    *pixel_number: address pointer of pixel numbers in one period of HSYNC
*    *line_number: address pointer of line numbers in one period of VSYNC
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void SP0A29_get_period(kal_uint16 *pixel_number, kal_uint16 *line_number)
{
  *pixel_number = VGA_PERIOD_PIXEL_NUMS+SP0A29_Sensor_Driver.dummy_pixels;
  *line_number = VGA_PERIOD_LINE_NUMS+SP0A29_Sensor_Driver.dummy_lines;
}

/*************************************************************************
* FUNCTION
*    SP0A29_feature_control
*
* DESCRIPTION
*    This function control sensor mode
*
* PARAMETERS
*    id: scenario id
*    image_window: image grab window
*    cfg_data: config data
*
* RETURNS
*    error code
*
* LOCAL AFFECTED
*
*************************************************************************/
static kal_uint32 SP0A29FeatureControl(MSDK_SENSOR_FEATURE_ENUM id, kal_uint8 *para, kal_uint32 *len)
{
	UINT32 *pFeatureData32=(UINT32 *) para;

	switch (id)
	{
		case SENSOR_FEATURE_GET_RESOLUTION: /* no use */
			SP0A29_get_size((kal_uint16 *)para, (kal_uint16 *)(para + sizeof(kal_uint16)));
			*len = sizeof(kal_uint32);
			break;
		case SENSOR_FEATURE_GET_PERIOD:
			SP0A29_get_period((kal_uint16 *)para, (kal_uint16 *)(para + sizeof(kal_uint16)));
			*len = sizeof(kal_uint32);
			break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			*(kal_uint32 *)para = SP0A29_Sensor_Driver.fPV_PCLK;
			*len = sizeof(kal_uint32);
			break;
		case SENSOR_FEATURE_SET_ESHUTTER:
			break;
		case SENSOR_FEATURE_SET_NIGHTMODE: 
#ifdef DEBUG_SENSOR
			SP0A29_night_mode((kal_bool)*(kal_uint16 *)para);
#endif
			break;
		case SENSOR_FEATURE_SET_GAIN:
		case SENSOR_FEATURE_SET_FLASHLIGHT:
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			break;
		case SENSOR_FEATURE_SET_REGISTER:
			SP0A29_write_cmos_sensor(((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegAddr, ((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegData);
			break;
		case SENSOR_FEATURE_GET_REGISTER: /* 10 */
			((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegData = SP0A29_read_cmos_sensor(((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegAddr);
			break;
		case SENSOR_FEATURE_SET_CCT_REGISTER:
			memcpy(&SP0A29_Sensor_Driver.eng.CCT, para, sizeof(SP0A29_Sensor_Driver.eng.CCT));
			break;
		case SENSOR_FEATURE_GET_CCT_REGISTER:
		case SENSOR_FEATURE_SET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_REGISTER_DEFAULT:
		case SENSOR_FEATURE_GET_CONFIG_PARA: /* no use */
			break;
		case SENSOR_FEATURE_CAMERA_PARA_TO_SENSOR:
			break;
		case SENSOR_FEATURE_SENSOR_TO_CAMERA_PARA:
			break;
		case SENSOR_FEATURE_GET_GROUP_COUNT:
		case SENSOR_FEATURE_GET_GROUP_INFO: /* 20 */
		case SENSOR_FEATURE_GET_ITEM_INFO:
		case SENSOR_FEATURE_SET_ITEM_INFO:
		case SENSOR_FEATURE_GET_ENG_INFO:
			break;
		case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
		/*
		* get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
		* if EEPROM does not exist in camera module.
		*/
			*(kal_uint32 *)para = LENS_DRIVER_ID_DO_NOT_CARE;
			*len = sizeof(kal_uint32);
			break;
		case SENSOR_FEATURE_SET_YUV_CMD:
	//		SP0A29_YUVSensorSetting((FEATURE_ID)(UINT32 *)para, (UINT32 *)(para+1));
			
			SP0A29_YUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
			break;
#if 0		    		
		case SENSOR_FEATURE_QUERY:
			SP0A29_Query(pSensorFeatureInfo);
			*pFeatureParaLen = sizeof(MSDK_FEATURE_INFO_STRUCT);
			break;		
		case SENSOR_FEATURE_SET_YUV_CAPTURE_RAW_SUPPORT:
			/* update yuv capture raw support flag by *pFeatureData16 */
			break;		
#endif 			
		case SENSOR_FEATURE_SET_VIDEO_MODE:
			SP0A29_YUVSetVideoMode(*para);
			break;
              case SENSOR_FEATURE_CHECK_SENSOR_ID:
                     SP0A29_GetSensorID(pFeatureData32); 
                     break; 	
              case SENSOR_FEATURE_SET_SOFTWARE_PWDN:
                     SP0A29SetSoftwarePWDNMode((BOOL)*pFeatureData32);        	        	
                     break;
		default:
			break;
	}
	return ERROR_NONE;
}




#if 0
image_sensor_func_struct image_sensor_driver_SP0A29=
{
	SP0A29Open,
	SP0A29Close,
	SP0A29GetResolution,
	SP0A29GetInfo,
	SP0A29Control,
	SP0A29FeatureControl
};
void image_sensor_func_config(void)
{
	extern image_sensor_func_struct *image_sensor_driver;

	image_sensor_driver = &image_sensor_driver_SP0A29;
}

#endif

SENSOR_FUNCTION_STRUCT	SensorFuncSP0A29=
{
	SP0A29Open,
	SP0A29GetInfo,
	SP0A29GetResolution,
	SP0A29FeatureControl,
	SP0A29Control,
	SP0A29Close
};

UINT32 SP0A29_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{

	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&SensorFuncSP0A29;

	return ERROR_NONE;
}	/* SensorInit() */




