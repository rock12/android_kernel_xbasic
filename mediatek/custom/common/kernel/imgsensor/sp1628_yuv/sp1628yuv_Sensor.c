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



the first version for sp1628 by zouyu 20130506

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

#include "sp1628yuv_Sensor.h"
#include "sp1628yuv_Camera_Sensor_para.h"
#include "sp1628yuv_CameraCustomized.h"

static MSDK_SENSOR_CONFIG_STRUCT SP1628SensorConfigData;
static struct SP1628_Sensor_Struct SP1628_Sensor_Driver;




//#define DEBUG_SENSOR_SP1628
#ifdef DEBUG_SENSOR_SP1628
kal_uint16 SP1628_write_cmos_sensor(kal_uint8 addr, kal_uint8 para);
kal_uint16 SP1628_read_cmos_sensor(kal_uint8 addr);
	#define SP1628_OP_CODE_INI		0x00		/* Initial value. */
	#define SP1628_OP_CODE_REG		0x01		/* Register */
	#define SP1628_OP_CODE_DLY		0x02		/* Delay */
	#define SP1628_OP_CODE_END		0x03		/* End of initial setting. */
	kal_uint16 fromsd1;

		typedef struct
	{
		u16 init_reg;
		u16 init_val;	/* Save the register value and delay tick */
		u8 op_code;		/* 0 - Initial value, 1 - Register, 2 - Delay, 3 - End of setting. */
	} SP1628_initial_set_struct;

	SP1628_initial_set_struct SP1628_Init_Reg[1000];
	
 u32 strtol1(const char *nptr, u8 base)
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

 u8 SP1628_Initialize_from_T_Flash1()
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
 
    fp = filp_open("/mnt/sdcard/sp1628_sd", O_RDONLY , 0); 
    if (IS_ERR(fp)) { 
        printk("create file error\n"); 
        return -1; 
    } 
    fs = get_fs(); 
    set_fs(KERNEL_DS); 

	file_size = vfs_llseek(fp, 0, SEEK_END);
    vfs_read(fp, data_buff, file_size, &pos); 
    //printk("%s %d %d\n", buf,iFileLen,pos); 
	filp_close(fp, NULL); 
    set_fs(fs);
	
	



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
			SP1628_Init_Reg[i].op_code = SP1628_OP_CODE_REG;
			
			SP1628_Init_Reg[i].init_reg = strtol1((const char *)curr_ptr, 16);
			curr_ptr += 5;	/* Skip "00, 0x" */
		
			SP1628_Init_Reg[i].init_val = strtol1((const char *)curr_ptr, 16);
			curr_ptr += 4;	/* Skip "00);" */
		
		}
		else									/* DLY */
		{
			/* Need add delay for this setting. */
			curr_ptr += 4;	
			SP1628_Init_Reg[i].op_code = SP1628_OP_CODE_DLY;
			
			SP1628_Init_Reg[i].init_reg = 0xFF;
			SP1628_Init_Reg[i].init_val = strtol1((const char *)curr_ptr,  10);	/* Get the delay ticks, the delay should less then 50 */
		}
		i++;
		

		/* Skip to next line directly. */
		while (!((*curr_ptr == 0x0D) && (*(curr_ptr+1) == 0x0A)))
		{
			curr_ptr++;
		}
		curr_ptr += 2;
	}

	/* (0xFFFF, 0xFFFF) means the end of initial setting. */
	SP1628_Init_Reg[i].op_code = SP1628_OP_CODE_END;
	SP1628_Init_Reg[i].init_reg = 0xFF;
	SP1628_Init_Reg[i].init_val = 0xFF;
	i++;
	//for (j=0; j<i; j++)
		//printk(" %x  ==  %x\n",SP1628_Init_Reg[j].init_reg, SP1628_Init_Reg[j].init_val);

	/* Start apply the initial setting to sensor. */
	#if 1
	for (j=0; j<i; j++)
	{
		if (SP1628_Init_Reg[j].op_code == SP1628_OP_CODE_END)	/* End of the setting. */
		{
			break ;
		}
		else if (SP1628_Init_Reg[j].op_code == SP1628_OP_CODE_DLY)
		{
			msleep(SP1628_Init_Reg[j].init_val);		/* Delay */
		}
		else if (SP1628_Init_Reg[j].op_code == SP1628_OP_CODE_REG)
		{
		
			SP1628_write_cmos_sensor(SP1628_Init_Reg[j].init_reg, SP1628_Init_Reg[j].init_val);
		}
		else
		{
			printk("REG ERROR!\n");
		}
	}
#endif


	return 1;	
}

#endif



#define SP1628YUV_DEBUG
#ifdef SP1628YUV_DEBUG
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

kal_uint8 isBanding = 0; // 0: 50hz  1:60hz

extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);
/*************************************************************************
* FUNCTION
*    SP1628_write_cmos_sensor
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
kal_uint16 SP1628_write_cmos_sensor(kal_uint8 addr, kal_uint8 para) 
{
    char puSendCmd[2] = {(char)(addr & 0xFF) , (char)(para & 0xFF)};
    iWriteRegI2C(puSendCmd , 2,SP1628_WRITE_ID);
}

kal_uint16 SP1628_read_cmos_sensor(kal_uint8 addr)
{
    kal_uint16 get_byte=0;
    char puSendCmd = { (char)(addr & 0xFF) };
    iReadRegI2C(&puSendCmd , 1, (u8*)&get_byte,1,SP1628_WRITE_ID);
    return get_byte;
}


 static void SP1628_Set_Dummy(const kal_uint16 iPixels, const kal_uint16 iLines)
{
}   /*  SP1628_Set_Dummy    */


/*************************************************************************
* FUNCTION
*	SP1628_write_reg
*
* DESCRIPTION
*	This function set the register of SP1628.
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

//static void SP1628_write_reg(kal_uint32 addr, kal_uint32 para)
//{
//	SP1628_write_cmos_sensor(addr,para);
//}	/* SP1628_write_reg() */

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
//static kal_uint32 SP1628_read_reg(kal_uint32 addr)
//{
//	return (SP1628_read_cmos_sensor(addr));
//}	/* OV7670_read_reg() */

/*************************************************************************
* FUNCTION
*	SP1628_NightMode
*
* DESCRIPTION
*	This function night mode of SP1628.
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
static void SP1628_night_mode(kal_bool bEnable)
{
// kal_uint8 temp = SP1628_read_cmos_sensor(0x3B);

  if (!SP1628_Sensor_Driver.MODE_CAPTURE) { 
	if(bEnable)//night mode
	{ 
		   SP1628_Sensor_Driver.bNight_mode = KAL_TRUE;
		                	//SP1628_write_cmos_sensor(0xfd,0x0 );  //?
					//SP1628_write_cmos_sensor(0xb2,0x25);
					//SP1628_write_cmos_sensor(0xb3,0x1f);
	   if(SP1628_Sensor_Driver.MPEG4_encode_mode == KAL_TRUE)
		{
				if(isBanding== 0)
				{
				printk("video 50Hz night\n");	
				//Video record night 24M 2 pll 50hz 12-12FPS maxgain				                     
				SP1628_write_cmos_sensor(0xfd,0x00);
				SP1628_write_cmos_sensor(0x03,0x01);
				SP1628_write_cmos_sensor(0x04,0x68);
				SP1628_write_cmos_sensor(0x05,0x00);
				SP1628_write_cmos_sensor(0x06,0x00);
				SP1628_write_cmos_sensor(0x09,0x04);
				SP1628_write_cmos_sensor(0x0a,0x7e);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0xf0,0x00);
				SP1628_write_cmos_sensor(0xf7,0x3c);
				SP1628_write_cmos_sensor(0xf8,0x32);
				SP1628_write_cmos_sensor(0x02,0x10);
				SP1628_write_cmos_sensor(0x03,0x01);
				SP1628_write_cmos_sensor(0x06,0x3c);
				SP1628_write_cmos_sensor(0x07,0x00);
				SP1628_write_cmos_sensor(0x08,0x01);
				SP1628_write_cmos_sensor(0x09,0x00);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0x40,0x14);
				SP1628_write_cmos_sensor(0x41,0x32);
				SP1628_write_cmos_sensor(0x42,0x00);
				SP1628_write_cmos_sensor(0x88,0x88);
				SP1628_write_cmos_sensor(0x89,0x3d);
				SP1628_write_cmos_sensor(0x8a,0xa8);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0xbe,0xc0);
				SP1628_write_cmos_sensor(0xbf,0x03);
				SP1628_write_cmos_sensor(0xd0,0xc0);
				SP1628_write_cmos_sensor(0xd1,0x03);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0x5b,0x03);
				SP1628_write_cmos_sensor(0x5c,0xc0);
				SP1628_write_cmos_sensor(0xfd,0x00);
				//dbg_print(" video 50Hz night\r\n");
				}
				else if(isBanding == 1)
				{
				//Video record night 24M3pll 60Hz 12-12FPS maxgain:
				SP1628_write_cmos_sensor(0xfd,0x00);
				SP1628_write_cmos_sensor(0x03,0x01);
				SP1628_write_cmos_sensor(0x04,0x2c);
				SP1628_write_cmos_sensor(0x05,0x00);
				SP1628_write_cmos_sensor(0x06,0x00);
				SP1628_write_cmos_sensor(0x09,0x04);
				SP1628_write_cmos_sensor(0x0a,0x7e);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0xf0,0x00);
				SP1628_write_cmos_sensor(0xf7,0x32);
				SP1628_write_cmos_sensor(0xf8,0x32);
				SP1628_write_cmos_sensor(0x02,0x14);
				SP1628_write_cmos_sensor(0x03,0x01);
				SP1628_write_cmos_sensor(0x06,0x32);
				SP1628_write_cmos_sensor(0x07,0x00);
				SP1628_write_cmos_sensor(0x08,0x01);
				SP1628_write_cmos_sensor(0x09,0x00);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0x40,0x14);
				SP1628_write_cmos_sensor(0x41,0x32);
				SP1628_write_cmos_sensor(0x42,0x00);
				SP1628_write_cmos_sensor(0x88,0x3d);
				SP1628_write_cmos_sensor(0x89,0x3d);
				SP1628_write_cmos_sensor(0x8a,0xaa);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0xbe,0xe8);
				SP1628_write_cmos_sensor(0xbf,0x03);
				SP1628_write_cmos_sensor(0xd0,0xe8);
				SP1628_write_cmos_sensor(0xd1,0x03);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0x5b,0x03);
				SP1628_write_cmos_sensor(0x5c,0xe8);
				SP1628_write_cmos_sensor(0xfd,0x00);
				printk(" video 60Hz night\r\n");
				}
   		  	}	
	    else 
	   {
			//	dbg_print(" SP1628_banding=%x\r\n",SP1628_banding);
			       if(isBanding== 0)
				{
				  //capture preview night 24M 3pll   50hz 6FPS maxgain:	 
				SP1628_write_cmos_sensor(0xfd,0x00);
				SP1628_write_cmos_sensor(0x03,0x01);
				SP1628_write_cmos_sensor(0x04,0x68);
				SP1628_write_cmos_sensor(0x05,0x00);
				SP1628_write_cmos_sensor(0x06,0x00);
				SP1628_write_cmos_sensor(0x09,0x04);
				SP1628_write_cmos_sensor(0x0a,0x7e);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0xf0,0x00);
				SP1628_write_cmos_sensor(0xf7,0x3c);
				SP1628_write_cmos_sensor(0xf8,0x32);
				SP1628_write_cmos_sensor(0x02,0x10);
				SP1628_write_cmos_sensor(0x03,0x01);
				SP1628_write_cmos_sensor(0x06,0x3c);
				SP1628_write_cmos_sensor(0x07,0x00);
				SP1628_write_cmos_sensor(0x08,0x01);
				SP1628_write_cmos_sensor(0x09,0x00);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0x40,0x14);
				SP1628_write_cmos_sensor(0x41,0x32);
				SP1628_write_cmos_sensor(0x42,0x00);
				SP1628_write_cmos_sensor(0x88,0x88);
				SP1628_write_cmos_sensor(0x89,0x3d);
				SP1628_write_cmos_sensor(0x8a,0xa8);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0xbe,0xc0);
				SP1628_write_cmos_sensor(0xbf,0x03);
				SP1628_write_cmos_sensor(0xd0,0xc0);
				SP1628_write_cmos_sensor(0xd1,0x03);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0x5b,0x03);
				SP1628_write_cmos_sensor(0x5c,0xc0);
				SP1628_write_cmos_sensor(0xfd,0x00);
				printk(" priview 50Hz night\r\n");	
				}  
				else if(isBanding== 1)
				{
				//capture preview night 24M 2pll 60hz 6FPS maxgain:
				SP1628_write_cmos_sensor(0xfd,0x00);
				SP1628_write_cmos_sensor(0x03,0x01);
				SP1628_write_cmos_sensor(0x04,0x2c);
				SP1628_write_cmos_sensor(0x05,0x00);
				SP1628_write_cmos_sensor(0x06,0x00);
				SP1628_write_cmos_sensor(0x09,0x04);
				SP1628_write_cmos_sensor(0x0a,0x7e);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0xf0,0x00);
				SP1628_write_cmos_sensor(0xf7,0x32);
				SP1628_write_cmos_sensor(0xf8,0x32);
				SP1628_write_cmos_sensor(0x02,0x14);
				SP1628_write_cmos_sensor(0x03,0x01);
				SP1628_write_cmos_sensor(0x06,0x32);
				SP1628_write_cmos_sensor(0x07,0x00);
				SP1628_write_cmos_sensor(0x08,0x01);
				SP1628_write_cmos_sensor(0x09,0x00);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0x40,0x14);
				SP1628_write_cmos_sensor(0x41,0x32);
				SP1628_write_cmos_sensor(0x42,0x00);
				SP1628_write_cmos_sensor(0x88,0x3d);
				SP1628_write_cmos_sensor(0x89,0x3d);
				SP1628_write_cmos_sensor(0x8a,0xaa);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0xbe,0xe8);
				SP1628_write_cmos_sensor(0xbf,0x03);
				SP1628_write_cmos_sensor(0xd0,0xe8);
				SP1628_write_cmos_sensor(0xd1,0x03);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0x5b,0x03);
				SP1628_write_cmos_sensor(0x5c,0xe8);
				SP1628_write_cmos_sensor(0xfd,0x00);
				printk(" priview 60Hz night\r\n");	
				}
			       } 		
	}
	else    // daylight mode
	{
		SP1628_Sensor_Driver.bNight_mode = KAL_FALSE;
		                   
	    if(SP1628_Sensor_Driver.MPEG4_encode_mode == KAL_TRUE)
	    {
				//dbg_print(" SP1628_banding=%x\r\n",SP1628_banding);
				if(isBanding== 0)
				{				
				SP1628_write_cmos_sensor(0xfd,0x00);//;ae setting						
				SP1628_write_cmos_sensor(0x03,0x02);//									
				SP1628_write_cmos_sensor(0x04,0x58);//									
				SP1628_write_cmos_sensor(0x05,0x00);//									
				SP1628_write_cmos_sensor(0x06,0x00);//									
				SP1628_write_cmos_sensor(0x09,0x01);//									
				SP1628_write_cmos_sensor(0x0a,0x5e);//									
				SP1628_write_cmos_sensor(0xfd,0x01);//									
				SP1628_write_cmos_sensor(0xf0,0x00);//									
				SP1628_write_cmos_sensor(0xf7,0x64);//									
				SP1628_write_cmos_sensor(0xf8,0x54);//									
				SP1628_write_cmos_sensor(0x02,0x0c);//									
				SP1628_write_cmos_sensor(0x03,0x01);//									
				SP1628_write_cmos_sensor(0x06,0x64);//									
				SP1628_write_cmos_sensor(0x07,0x00);//									
				SP1628_write_cmos_sensor(0x08,0x01);//									
				SP1628_write_cmos_sensor(0x09,0x00);//									
				SP1628_write_cmos_sensor(0xfd,0x02);//									
				SP1628_write_cmos_sensor(0x40,0x0f);//									
				SP1628_write_cmos_sensor(0x41,0x54);//									
				SP1628_write_cmos_sensor(0x42,0x00);//									
				SP1628_write_cmos_sensor(0x88,0x1e);//									
				SP1628_write_cmos_sensor(0x89,0x18);//									
				SP1628_write_cmos_sensor(0x8a,0x65);//									
				SP1628_write_cmos_sensor(0xfd,0x02);//;Status							
				SP1628_write_cmos_sensor(0xbe,0xb0);//									
				SP1628_write_cmos_sensor(0xbf,0x04);//									
				SP1628_write_cmos_sensor(0xd0,0xb0);//									
				SP1628_write_cmos_sensor(0xd1,0x04);//									
				SP1628_write_cmos_sensor(0xfd,0x01);//									
				SP1628_write_cmos_sensor(0x5b,0x04);//									
				SP1628_write_cmos_sensor(0x5c,0xb0);//									
				SP1628_write_cmos_sensor(0xfd,0x00);//									
				printk(" video 50Hz normal\r\n");				
				}
				else if(isBanding == 1)
				{
				//Video record daylight 24M3pll 60Hz 12FPS maxgain:				                       
				SP1628_write_cmos_sensor(0xfd,0x00);
				SP1628_write_cmos_sensor(0x03,0x02);
				SP1628_write_cmos_sensor(0x04,0x28);
				SP1628_write_cmos_sensor(0x05,0x00);
				SP1628_write_cmos_sensor(0x06,0x00);
				SP1628_write_cmos_sensor(0x09,0x00);
				SP1628_write_cmos_sensor(0x0a,0xec);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0xf0,0x00);
				SP1628_write_cmos_sensor(0xf7,0x5c);
				SP1628_write_cmos_sensor(0xf8,0x5c);
				SP1628_write_cmos_sensor(0x02,0x0a);
				SP1628_write_cmos_sensor(0x03,0x01);
				SP1628_write_cmos_sensor(0x06,0x5c);
				SP1628_write_cmos_sensor(0x07,0x00);
				SP1628_write_cmos_sensor(0x08,0x01);
				SP1628_write_cmos_sensor(0x09,0x00);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0x40,0x0a);
				SP1628_write_cmos_sensor(0x41,0x5c);
				SP1628_write_cmos_sensor(0x42,0x00);
				SP1628_write_cmos_sensor(0x88,0x90);
				SP1628_write_cmos_sensor(0x89,0x90);
				SP1628_write_cmos_sensor(0x8a,0x55);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0xbe,0x98);
				SP1628_write_cmos_sensor(0xbf,0x03);
				SP1628_write_cmos_sensor(0xd0,0x98);
				SP1628_write_cmos_sensor(0xd1,0x03);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0x5b,0x03);
				SP1628_write_cmos_sensor(0x5c,0x98);
				SP1628_write_cmos_sensor(0xfd,0x00);
				printk(" video 60Hz normal\r\n");	
				}
			   }
		else 
			{
			//	dbg_print(" SP1628_banding=%x\r\n",SP1628_banding);
			       if(isBanding== 0)
				{
					   // 50hz 24MHz2pll 11fps 50Hz  1280x960  
			
					   SP1628_write_cmos_sensor(0xfd,0x00);//;ae setting						
					   SP1628_write_cmos_sensor(0x03,0x02);//									
					   SP1628_write_cmos_sensor(0x04,0x58);//									
					   SP1628_write_cmos_sensor(0x05,0x00);//									
					   SP1628_write_cmos_sensor(0x06,0x00);//									
					   SP1628_write_cmos_sensor(0x09,0x01);//									
					   SP1628_write_cmos_sensor(0x0a,0x5e);//									
					   SP1628_write_cmos_sensor(0xfd,0x01);//									
					   SP1628_write_cmos_sensor(0xf0,0x00);//									
					   SP1628_write_cmos_sensor(0xf7,0x64);//									
					   SP1628_write_cmos_sensor(0xf8,0x54);//									
					   SP1628_write_cmos_sensor(0x02,0x0c);//									
					   SP1628_write_cmos_sensor(0x03,0x01);//									
					   SP1628_write_cmos_sensor(0x06,0x64);//									
					   SP1628_write_cmos_sensor(0x07,0x00);//									
					   SP1628_write_cmos_sensor(0x08,0x01);//									
					   SP1628_write_cmos_sensor(0x09,0x00);//									
					   SP1628_write_cmos_sensor(0xfd,0x02);//									
					   SP1628_write_cmos_sensor(0x40,0x0f);//									
					   SP1628_write_cmos_sensor(0x41,0x54);//									
					   SP1628_write_cmos_sensor(0x42,0x00);//									
					   SP1628_write_cmos_sensor(0x88,0x1e);//									
					   SP1628_write_cmos_sensor(0x89,0x18);//									
					   SP1628_write_cmos_sensor(0x8a,0x65);//									
					   SP1628_write_cmos_sensor(0xfd,0x02);//;Status							
					   SP1628_write_cmos_sensor(0xbe,0xb0);//									
					   SP1628_write_cmos_sensor(0xbf,0x04);//									
					   SP1628_write_cmos_sensor(0xd0,0xb0);//									
					   SP1628_write_cmos_sensor(0xd1,0x04);//									
					   SP1628_write_cmos_sensor(0xfd,0x01);//									
					   SP1628_write_cmos_sensor(0x5b,0x04);//									
					   SP1628_write_cmos_sensor(0x5c,0xb0);//									
					   SP1628_write_cmos_sensor(0xfd,0x00);//									
			
				printk(" priview 50Hz normal\r\n");
				}
				else if(isBanding== 1)
				{
				     // 60hz 24MHz 2pll  11fps 60Hz  1280x960  
				SP1628_write_cmos_sensor(0xfd,0x00);
				SP1628_write_cmos_sensor(0x03,0x02);
				SP1628_write_cmos_sensor(0x04,0x28);
				SP1628_write_cmos_sensor(0x05,0x00);
				SP1628_write_cmos_sensor(0x06,0x00);
				SP1628_write_cmos_sensor(0x09,0x00);
				SP1628_write_cmos_sensor(0x0a,0xec);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0xf0,0x00);
				SP1628_write_cmos_sensor(0xf7,0x5c);
				SP1628_write_cmos_sensor(0xf8,0x5c);
				SP1628_write_cmos_sensor(0x02,0x0a);
				SP1628_write_cmos_sensor(0x03,0x01);
				SP1628_write_cmos_sensor(0x06,0x5c);
				SP1628_write_cmos_sensor(0x07,0x00);
				SP1628_write_cmos_sensor(0x08,0x01);
				SP1628_write_cmos_sensor(0x09,0x00);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0x40,0x0a);
				SP1628_write_cmos_sensor(0x41,0x5c);
				SP1628_write_cmos_sensor(0x42,0x00);
				SP1628_write_cmos_sensor(0x88,0x90);
				SP1628_write_cmos_sensor(0x89,0x90);
				SP1628_write_cmos_sensor(0x8a,0x55);
				SP1628_write_cmos_sensor(0xfd,0x02);
				SP1628_write_cmos_sensor(0xbe,0x98);
				SP1628_write_cmos_sensor(0xbf,0x03);
				SP1628_write_cmos_sensor(0xd0,0x98);
				SP1628_write_cmos_sensor(0xd1,0x03);
				SP1628_write_cmos_sensor(0xfd,0x01);
				SP1628_write_cmos_sensor(0x5b,0x03);
				SP1628_write_cmos_sensor(0x5c,0x98);
				SP1628_write_cmos_sensor(0xfd,0x00);
			     }
	   
	}  
	}
  	}
}
/*************************************************************************
* FUNCTION
*	SP1628_NightMode
*
* DESCRIPTION
*	This function night mode of SP1628.
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
	/*	SP1628_NightMode	*/

/*
static void SP1628_set_isp_driving_current(kal_uint8 current)
{
    //#define CONFIG_BASE      	(0xF0001000)     
//  iowrite32((0xE << 12)|(0 << 28)|0x8880888, 0xF0001500);
}
*/



/*
static void SP1628_set_isp_driving_current(kal_uint8 current)
{
    //#define CONFIG_BASE      	(0xF0001000)     
//  iowrite32((0xE << 12)|(0 << 28)|0x8880888, 0xF0001500);
}
*/

static void SP1628_Sensor_Init(void)
{



#ifdef DEBUG_SENSOR_SP1628
			  if(fromsd1 == 1)//是否从SD读取//gepeiwei	120903
				  {
				  
			  printk("________________from t!\n");
			  SP1628_Initialize_from_T_Flash1();//从SD卡读取的主要函数
				  }
			  else
#endif

  SP1628_write_cmos_sensor(0xfd,0x00);//
  SP1628_write_cmos_sensor(0x1b,0x60);//
  SP1628_write_cmos_sensor(0x2f,0x10);//;61	;72M //0x20
  SP1628_write_cmos_sensor(0x1c,0x20);//0x10 //zouyu test
  SP1628_write_cmos_sensor(0x30,0x00);//;00		
  SP1628_write_cmos_sensor(0x0c,0x66);//;analog
  SP1628_write_cmos_sensor(0x0d,0x12);//
  SP1628_write_cmos_sensor(0x13,0x1d);//
  SP1628_write_cmos_sensor(0x6b,0x20);//
  SP1628_write_cmos_sensor(0x6d,0x20);//
  SP1628_write_cmos_sensor(0x6f,0x21);//
  SP1628_write_cmos_sensor(0x73,0x22);//
  SP1628_write_cmos_sensor(0x7a,0x20);//
  SP1628_write_cmos_sensor(0x15,0x30);//
  SP1628_write_cmos_sensor(0x71,0x32);//
  SP1628_write_cmos_sensor(0x76,0x34);//  
  SP1628_write_cmos_sensor(0x29,0x08);//
  SP1628_write_cmos_sensor(0x18,0x01);//
  SP1628_write_cmos_sensor(0x19,0x10);//
  SP1628_write_cmos_sensor(0x1a,0xc3);//;c1
  SP1628_write_cmos_sensor(0x1b,0x6f);//
  SP1628_write_cmos_sensor(0x1d,0x11);//;01
  SP1628_write_cmos_sensor(0x1e,0x00);//;1e
  SP1628_write_cmos_sensor(0x1f,0xa0);//0x80  zouyu test
  SP1628_write_cmos_sensor(0x20,0x7f);//
  SP1628_write_cmos_sensor(0x22,0x3c);//;1b
  SP1628_write_cmos_sensor(0x25,0xff);//
  SP1628_write_cmos_sensor(0x2b,0x88);//
  SP1628_write_cmos_sensor(0x2c,0x85);//
  SP1628_write_cmos_sensor(0x2d,0x00);//
  SP1628_write_cmos_sensor(0x2e,0x85);//0x80  //zouyu test
  SP1628_write_cmos_sensor(0x27,0x38);//
  SP1628_write_cmos_sensor(0x28,0x03);//
  SP1628_write_cmos_sensor(0x70,0x40);//
  SP1628_write_cmos_sensor(0x72,0x40);//    
  SP1628_write_cmos_sensor(0x74,0x38);//    
  SP1628_write_cmos_sensor(0x75,0x38);//
  SP1628_write_cmos_sensor(0x77,0x38);//  
  SP1628_write_cmos_sensor(0x7f,0x40);//       
  SP1628_write_cmos_sensor(0x31,0x70);//		;mirror/flip 960
  SP1628_write_cmos_sensor(0xfd,0x01);//
  SP1628_write_cmos_sensor(0x5d,0x11);//		;position
  SP1628_write_cmos_sensor(0x5f,0x00);//		;延长
  SP1628_write_cmos_sensor(0xfb,0x25);//		;blacklevl
  SP1628_write_cmos_sensor(0x48,0x00);//		;dp
  SP1628_write_cmos_sensor(0x49,0x99);// 
  SP1628_write_cmos_sensor(0xf2,0x0a);//		;同SP1628 0xf4     
  SP1628_write_cmos_sensor(0xfd,0x02);//;AE
  SP1628_write_cmos_sensor(0x52,0x34);//
  SP1628_write_cmos_sensor(0x53,0x02);//		;测试是否ae抖
  SP1628_write_cmos_sensor(0x54,0x0c);//
  SP1628_write_cmos_sensor(0x55,0x08);// 
  SP1628_write_cmos_sensor(0x86,0x0c);//		;其中满足条件帧数
  SP1628_write_cmos_sensor(0x87,0x10);//		;检测总帧数
  SP1628_write_cmos_sensor(0x8b,0x10);// 
  SP1628_write_cmos_sensor(0xfd,0x00);//;ae setting    50HZ，60Hz哪里设置？
  SP1628_write_cmos_sensor(0x03,0x05);//
  SP1628_write_cmos_sensor(0x04,0x40);// 
  SP1628_write_cmos_sensor(0x05,0x00);//
  SP1628_write_cmos_sensor(0x06,0x00);//  
  SP1628_write_cmos_sensor(0x09,0x00);//
  SP1628_write_cmos_sensor(0x0a,0x57);//
  SP1628_write_cmos_sensor(0xfd,0x01);//
  SP1628_write_cmos_sensor(0xf0,0x00);//		;base 5msb
  SP1628_write_cmos_sensor(0xf7,0xe0);//		;50Hz base 8lsb
  SP1628_write_cmos_sensor(0xf8,0xbb);//		;60Hz base 8lsb 
  SP1628_write_cmos_sensor(0x02,0x0c);//		;50Hz exp_max_indr
  SP1628_write_cmos_sensor(0x03,0x01);//		;exp_min_indr
  SP1628_write_cmos_sensor(0x06,0xe0);//		;50Hz exp_max_outdr[7:0]
  SP1628_write_cmos_sensor(0x07,0x00);//		;50Hz exp_max_outdr[12:8]
  SP1628_write_cmos_sensor(0x08,0x01);//		;50Hz exp_min_outdr[7:0]
  SP1628_write_cmos_sensor(0x09,0x00);//		;50Hz exp_min_outdr[12:8]
  SP1628_write_cmos_sensor(0xfd,0x02);//		
  SP1628_write_cmos_sensor(0x40,0x0f);//		;60Hz exp_max_indr
  SP1628_write_cmos_sensor(0x41,0xbb);//		;60Hz exp_max_outdr[7:0]   
  SP1628_write_cmos_sensor(0x42,0x00);//		;60Hz exp_max_outdr[12:8]  
  SP1628_write_cmos_sensor(0x88,0x49);//		;(2/Regf7)*256*256
  SP1628_write_cmos_sensor(0x89,0xbc);//		;(2/Regf8)*256*256
  SP1628_write_cmos_sensor(0x8a,0x22);//		;
  SP1628_write_cmos_sensor(0xfd,0x02);//;Status
  SP1628_write_cmos_sensor(0xbe,0x80);//		;DP_exp_5hsm
  SP1628_write_cmos_sensor(0xbf,0x0a);//		;DP_exp_8lsm
  SP1628_write_cmos_sensor(0xd0,0x80);//		
  SP1628_write_cmos_sensor(0xd1,0x0a);//		;exp_heq_dummy_8lsm
  SP1628_write_cmos_sensor(0xfd,0x01);//		;exp_heq_dummy_5hsm
  SP1628_write_cmos_sensor(0x5b,0x0a);//		;exp_heq_low_8lsm
  SP1628_write_cmos_sensor(0x5c,0x80);//		;exp_heq_low_5hsm  
  SP1628_write_cmos_sensor(0xfd,0x00);//
  SP1628_write_cmos_sensor(0xfd,0x01);//;fix status
  SP1628_write_cmos_sensor(0x5a,0x38);//		;DP_gain
  SP1628_write_cmos_sensor(0xfd,0x02);//
  SP1628_write_cmos_sensor(0xba,0x30);//		;mean_dummy_low
  SP1628_write_cmos_sensor(0xbb,0x50);//		;mean_low_dummy
  SP1628_write_cmos_sensor(0xbc,0xc0);//		;rpc_heq_low
  SP1628_write_cmos_sensor(0xbd,0xa0);//		;rpc_heq_dummy
  SP1628_write_cmos_sensor(0xb8,0x80);//		;mean_nr_dummy
  SP1628_write_cmos_sensor(0xb9,0x90);//		;mean_dummy_nr
  SP1628_write_cmos_sensor(0xfd,0x01);//;rpc
  SP1628_write_cmos_sensor(0xe0,0x54);//;6c 
  SP1628_write_cmos_sensor(0xe1,0x40);//;54 
  SP1628_write_cmos_sensor(0xe2,0x38);//;48 
  SP1628_write_cmos_sensor(0xe3,0x34);//;40
  SP1628_write_cmos_sensor(0xe4,0x34);//;40
  SP1628_write_cmos_sensor(0xe5,0x30);//;3e
  SP1628_write_cmos_sensor(0xe6,0x30);//;3e
  SP1628_write_cmos_sensor(0xe7,0x2e);//;3a
  SP1628_write_cmos_sensor(0xe8,0x2e);//;3a
  SP1628_write_cmos_sensor(0xe9,0x2e);//;3a
  SP1628_write_cmos_sensor(0xea,0x2c);//;38
  SP1628_write_cmos_sensor(0xf3,0x2c);//;38
  SP1628_write_cmos_sensor(0xf4,0x2c);//;38
  SP1628_write_cmos_sensor(0xfd,0x01);//;ae min gain 
  SP1628_write_cmos_sensor(0x04,0xc0);//		;rpc_max_indr
  SP1628_write_cmos_sensor(0x05,0x2c);//;38		;1e;rpc_min_indr 
  SP1628_write_cmos_sensor(0x0a,0xc0);//		;rpc_max_outdr
  SP1628_write_cmos_sensor(0x0b,0x2c);//;38		;rpc_min_outdr 
  SP1628_write_cmos_sensor(0xfd,0x01);//;ae target
  SP1628_write_cmos_sensor(0xeb,0x78);//		 
  SP1628_write_cmos_sensor(0xec,0x78);//		
  SP1628_write_cmos_sensor(0xed,0x05);//
  SP1628_write_cmos_sensor(0xee,0x0a);//
  SP1628_write_cmos_sensor(0xfd,0x01);//		;lsc
  SP1628_write_cmos_sensor(0x26,0x30);//
  SP1628_write_cmos_sensor(0x27,0xdc);//
  SP1628_write_cmos_sensor(0x28,0x05);//
  SP1628_write_cmos_sensor(0x29,0x08);//
  SP1628_write_cmos_sensor(0x2a,0x00);//
  SP1628_write_cmos_sensor(0x2b,0x03);//
  SP1628_write_cmos_sensor(0x2c,0x00);//
  SP1628_write_cmos_sensor(0x2d,0x2f);//
  SP1628_write_cmos_sensor(0xfd,0x01);//		;RGain
  SP1628_write_cmos_sensor(0xa1,0x37);//;48		;left
  SP1628_write_cmos_sensor(0xa2,0x30);//;58		;right
  SP1628_write_cmos_sensor(0xa3,0x32);//;58		;up
  SP1628_write_cmos_sensor(0xa4,0x2b);//;50		;down
  SP1628_write_cmos_sensor(0xad,0x0f);//;08		;lu
  SP1628_write_cmos_sensor(0xae,0x20);//;10		;ru
  SP1628_write_cmos_sensor(0xaf,0x0a);//;10		;ld
  SP1628_write_cmos_sensor(0xb0,0x0a);//;10		;rd
  SP1628_write_cmos_sensor(0x18,0x40);//;40		;left  
  SP1628_write_cmos_sensor(0x19,0x40);//;50		;right 
  SP1628_write_cmos_sensor(0x1a,0x88);//;32		;up    
  SP1628_write_cmos_sensor(0x1b,0x30);//;30		;down  
  SP1628_write_cmos_sensor(0xbf,0x0a);//;a5		;lu    
  SP1628_write_cmos_sensor(0xc0,0x20);//;a0		;ru    
  SP1628_write_cmos_sensor(0xc1,0x08);//;08		;ld    
  SP1628_write_cmos_sensor(0xfa,0x0a);//;00		;rd   
  SP1628_write_cmos_sensor(0xa5,0x35);//;38	;GGain
  SP1628_write_cmos_sensor(0xa6,0x24);//;48
  SP1628_write_cmos_sensor(0xa7,0x27);//;48
  SP1628_write_cmos_sensor(0xa8,0x25);//;40
  SP1628_write_cmos_sensor(0xb1,0x00);//;00
  SP1628_write_cmos_sensor(0xb2,0x04);//;00
  SP1628_write_cmos_sensor(0xb3,0x00);//;00
  SP1628_write_cmos_sensor(0xb4,0x00);//;00
  SP1628_write_cmos_sensor(0x1c,0x24);//;28
  SP1628_write_cmos_sensor(0x1d,0x23);//;40
  SP1628_write_cmos_sensor(0x1e,0x23);//;2c
  SP1628_write_cmos_sensor(0xb9,0x25);//;25 
  SP1628_write_cmos_sensor(0x21,0x00);//;b0
  SP1628_write_cmos_sensor(0x22,0x00);//;a0
  SP1628_write_cmos_sensor(0x23,0x10);//;50
  SP1628_write_cmos_sensor(0x24,0x00);//;0d  
  SP1628_write_cmos_sensor(0xa9,0x20);//;38		;BGain
  SP1628_write_cmos_sensor(0xaa,0x24);//;48
  SP1628_write_cmos_sensor(0xab,0x2d);//;46
  SP1628_write_cmos_sensor(0xac,0x24);//;46
  SP1628_write_cmos_sensor(0xb5,0x00);//;08
  SP1628_write_cmos_sensor(0xb6,0x00);//;08
  SP1628_write_cmos_sensor(0xb7,0x00);//;08
  SP1628_write_cmos_sensor(0xb8,0x08);//;08
  SP1628_write_cmos_sensor(0xba,0x22);//;12
  SP1628_write_cmos_sensor(0xbc,0x24);//;30
  SP1628_write_cmos_sensor(0xbd,0x2c);//;31
  SP1628_write_cmos_sensor(0xbe,0x24);//;1e
  SP1628_write_cmos_sensor(0x25,0x00);//;a0
  SP1628_write_cmos_sensor(0x45,0x00);//;a0
  SP1628_write_cmos_sensor(0x46,0x12);//;12
  SP1628_write_cmos_sensor(0x47,0x00);//;09    
  SP1628_write_cmos_sensor(0xfd,0x01);//		;awb
  SP1628_write_cmos_sensor(0x32,0x15);//
  SP1628_write_cmos_sensor(0xfd,0x02);//
  SP1628_write_cmos_sensor(0x26,0xc9);//
  SP1628_write_cmos_sensor(0x27,0x8b);//
  SP1628_write_cmos_sensor(0x1b,0x80);//
  SP1628_write_cmos_sensor(0x1a,0x80);//
  SP1628_write_cmos_sensor(0x18,0x27);//
  SP1628_write_cmos_sensor(0x19,0x26);//
  SP1628_write_cmos_sensor(0x2a,0x01);//
  SP1628_write_cmos_sensor(0x2b,0x10);//
  SP1628_write_cmos_sensor(0x28,0xf8);//		;0xa0
  SP1628_write_cmos_sensor(0x29,0x08);// ;d65 88
  SP1628_write_cmos_sensor(0x66,0x35);//;35		;0x48
  SP1628_write_cmos_sensor(0x67,0x60);//;60		;0x69
  SP1628_write_cmos_sensor(0x68,0xb0);//;b0		;c8;0xb5;0xaa
  SP1628_write_cmos_sensor(0x69,0xe0);//;e0		;f4;0xda;0xed
  SP1628_write_cmos_sensor(0x6a,0xa5);//;indoor 89
  SP1628_write_cmos_sensor(0x7c,0x28);//;43
  SP1628_write_cmos_sensor(0x7d,0x48);//
  SP1628_write_cmos_sensor(0x7e,0xe8);//
  SP1628_write_cmos_sensor(0x7f,0x08);//
  SP1628_write_cmos_sensor(0x80,0xa6);//;cwf   8a
  SP1628_write_cmos_sensor(0x70,0x15);//;2f		;0x3b
  SP1628_write_cmos_sensor(0x71,0x37);//;4a		;0x55
  SP1628_write_cmos_sensor(0x72,0x05);//		;0x28
  SP1628_write_cmos_sensor(0x73,0x21);//;24		;0x45
  SP1628_write_cmos_sensor(0x74,0xaA);//;tl84  8b
  SP1628_write_cmos_sensor(0x6b,0x07);//;18
  SP1628_write_cmos_sensor(0x6c,0x26);//;34		;0x25;0x2f
  SP1628_write_cmos_sensor(0x6d,0x06);//;17		;0x35
  SP1628_write_cmos_sensor(0x6e,0x24);//		;0x52
  SP1628_write_cmos_sensor(0x6f,0xaa);//;f    8c
  SP1628_write_cmos_sensor(0x61,0xf0);//;e0;10		;04;0xf4;0xed
  SP1628_write_cmos_sensor(0x62,0x1a);//;38		;22;0x14;0f
  SP1628_write_cmos_sensor(0x63,0x1c);//		;30;0x5d
  SP1628_write_cmos_sensor(0x64,0x3a);//		;55;0x75;0x8f
  SP1628_write_cmos_sensor(0x65,0x6a);//		;0x6a
  SP1628_write_cmos_sensor(0x75,0x80);//
  SP1628_write_cmos_sensor(0x76,0x09);//
  SP1628_write_cmos_sensor(0x77,0x02);//
  SP1628_write_cmos_sensor(0x24,0x25);//
  SP1628_write_cmos_sensor(0x0e,0x16);//
  SP1628_write_cmos_sensor(0x3b,0x09);//
  SP1628_write_cmos_sensor(0xfd,0x02);//		; sharp
  SP1628_write_cmos_sensor(0xde,0x0f);//
  SP1628_write_cmos_sensor(0xd2,0x0c);//		;控制黑白边；0-边粗，f-变细
  SP1628_write_cmos_sensor(0xd3,0x0a);//
  SP1628_write_cmos_sensor(0xd4,0x08);//
  SP1628_write_cmos_sensor(0xd5,0x08);//
  SP1628_write_cmos_sensor(0xd7,0x10);//		;轮廓判断
  SP1628_write_cmos_sensor(0xd8,0x1d);//
  SP1628_write_cmos_sensor(0xd9,0x32);//
  SP1628_write_cmos_sensor(0xda,0x48);//
  SP1628_write_cmos_sensor(0xdb,0x08);//
  SP1628_write_cmos_sensor(0xe8,0x38);//		;轮廓强度
  SP1628_write_cmos_sensor(0xe9,0x38);//
  SP1628_write_cmos_sensor(0xea,0x30);//
  SP1628_write_cmos_sensor(0xeb,0x20);//
  SP1628_write_cmos_sensor(0xec,0x60);//
  SP1628_write_cmos_sensor(0xed,0x40);//
  SP1628_write_cmos_sensor(0xee,0x30);//
  SP1628_write_cmos_sensor(0xef,0x20);//
  SP1628_write_cmos_sensor(0xf3,0x00);//		;平坦区域锐化力度
  SP1628_write_cmos_sensor(0xf4,0x00);//
  SP1628_write_cmos_sensor(0xf5,0x00);//
  SP1628_write_cmos_sensor(0xf6,0x00);//
  SP1628_write_cmos_sensor(0xfd,0x02);//		;skin sharpen
  SP1628_write_cmos_sensor(0xdc,0x04);//		;肤色降锐化
  SP1628_write_cmos_sensor(0x05,0x6f);//		;排除肤色降锐化对分辨率卡引起的干扰
  SP1628_write_cmos_sensor(0x09,0x10);//		;肤色排除白点区域
  SP1628_write_cmos_sensor(0xfd,0x01);//		;dns
  SP1628_write_cmos_sensor(0x64,0x22);//		;沿方向边缘平滑力度  ;0-最强，8-最弱
  SP1628_write_cmos_sensor(0x65,0x22);//		
  SP1628_write_cmos_sensor(0x86,0x20);//		;沿方向边缘平滑阈值，越小越弱
  SP1628_write_cmos_sensor(0x87,0x20);//		
  SP1628_write_cmos_sensor(0x88,0x20);//		
  SP1628_write_cmos_sensor(0x89,0x20);//		
  SP1628_write_cmos_sensor(0x6d,0x0f);//		;强平滑（平坦）区域平滑阈值
  SP1628_write_cmos_sensor(0x6e,0x0f);//		
  SP1628_write_cmos_sensor(0x6f,0x14);//		
  SP1628_write_cmos_sensor(0x70,0x14);//		
  SP1628_write_cmos_sensor(0x71,0x0d);//		;弱轮廓（非平坦）区域平滑阈值	
  SP1628_write_cmos_sensor(0x72,0x18);//0x23  //zouyu 20130506		
  SP1628_write_cmos_sensor(0x73,0x2f);//		
  SP1628_write_cmos_sensor(0x74,0x34);//		
  SP1628_write_cmos_sensor(0x75,0x26);//0x46 //zouyu 20130506		;[7:4]平坦区域强度，[3:0]非平坦区域强度；0-最强，8-最弱；
  SP1628_write_cmos_sensor(0x76,0x26);//	0x36	 //zouyu 20130506
  SP1628_write_cmos_sensor(0x77,0x24);//		
  SP1628_write_cmos_sensor(0x78,0x12);//		
  SP1628_write_cmos_sensor(0x81,0x1d);//		;2x;根据增益判定区域阈值
  SP1628_write_cmos_sensor(0x82,0x2b);//		;4x
  SP1628_write_cmos_sensor(0x83,0xff);//		;8x
  SP1628_write_cmos_sensor(0x84,0xff);//		;16x
  SP1628_write_cmos_sensor(0x85,0x0a);//		; 12/8+reg0x81 第二阈值，在平坦和非平坦区域做连接


  SP1628_write_cmos_sensor(0xfd,0x01);//		;gamma  
  SP1628_write_cmos_sensor(0x8b,0x00);//;00;00;00;     
  SP1628_write_cmos_sensor(0x8c,0x0b);//;02;0b;0b;     
  SP1628_write_cmos_sensor(0x8d,0x19);//;0a;19;17;     
  SP1628_write_cmos_sensor(0x8e,0x2a);//;13;2a;27;     
  SP1628_write_cmos_sensor(0x8f,0x37);//;1d;37;35;     
  SP1628_write_cmos_sensor(0x90,0x4b);//;30;4b;51;     
  SP1628_write_cmos_sensor(0x91,0x5e);//;40;5e;64;     
  SP1628_write_cmos_sensor(0x92,0x6c);//;4e;6c;74;     
  SP1628_write_cmos_sensor(0x93,0x78);//;5a;78;80;     
  SP1628_write_cmos_sensor(0x94,0x92);//;71;92;92;     
  SP1628_write_cmos_sensor(0x95,0xa6);//;85;a6;a2;     
  SP1628_write_cmos_sensor(0x96,0xb5);//;96;b5;af;     
  SP1628_write_cmos_sensor(0x97,0xbf);//;a6;bf;bb;     
  SP1628_write_cmos_sensor(0x98,0xca);//;b3;ca;c6;     
  SP1628_write_cmos_sensor(0x99,0xd2);//;c0;d2;d0;     
  SP1628_write_cmos_sensor(0x9a,0xd9);//;cb;d9;d9;     
  SP1628_write_cmos_sensor(0x9b,0xe1);//;d5;e1;e0;     
  SP1628_write_cmos_sensor(0x9c,0xe8);//;df;e8;e8;     
  SP1628_write_cmos_sensor(0x9d,0xee);//;e9;ee;ee;     
  SP1628_write_cmos_sensor(0x9e,0xf4);//;f2;f4;f4;     
  SP1628_write_cmos_sensor(0x9f,0xfa);//;fa;fa;fa;     
  SP1628_write_cmos_sensor(0xa0,0xff);//;ff;ff;ff;     

  //CCM
  SP1628_write_cmos_sensor(0xfd,0x02);//		;CCM
  SP1628_write_cmos_sensor(0x15,0xac);//		;b>th a4
  SP1628_write_cmos_sensor(0x16,0x90);//		;r<th 87
  SP1628_write_cmos_sensor(0xa0,0xa6);//;99;a6;a6;8c;80; 非F		
  SP1628_write_cmos_sensor(0xa1,0xda);//;0c;da;da;da;fa;00; 		
  SP1628_write_cmos_sensor(0xa2,0x00);//;da;00;00;00;fa;00; 		
  SP1628_write_cmos_sensor(0xa3,0xe7);//;00;e7;e7;da;da;e7; 		
  SP1628_write_cmos_sensor(0xa4,0xa6);//;99;c0;c0;c0;c0;a6; 		
  SP1628_write_cmos_sensor(0xa5,0xf4);//;e7;da;da;e7;e7;f4; 		
  SP1628_write_cmos_sensor(0xa6,0xf4);//;00;00;00;00;00;00; 		
  SP1628_write_cmos_sensor(0xa7,0xe7);//;e7;b4;b4;a7;cd;da; 		
  SP1628_write_cmos_sensor(0xa8,0xa6);//;99;cc;d9;b3;a6;			
  SP1628_write_cmos_sensor(0xa9,0x0c);//;30;0c;0c;0c;3c;00; 		
  SP1628_write_cmos_sensor(0xaa,0x33);//;30;33;33;33;33;33; 		
  SP1628_write_cmos_sensor(0xab,0x0f);//;0c;0c;0c;0c;0c;0c; 		
 //F														   
															   
   SP1628_write_cmos_sensor(0xac,0x80);//;80;a2;b3;8c;F 		 
   SP1628_write_cmos_sensor(0xad,0x00);//;00;04;0c;0c;			   
   SP1628_write_cmos_sensor(0xae,0x00);//;00;da;c0;e7;			   
   SP1628_write_cmos_sensor(0xaf,0xe7);//;e7;cd;cd;b4;			   
   SP1628_write_cmos_sensor(0xb0,0x99);//;c0;d9;e6;e6;			   
   SP1628_write_cmos_sensor(0xb1,0x00);//;da;da;cd;e7;			   
   SP1628_write_cmos_sensor(0xb2,0xda);//;e7;f6;e7;e7;			   
   SP1628_write_cmos_sensor(0xb3,0xc0);//;b4;98;9a;9a;			   
   SP1628_write_cmos_sensor(0xb4,0xe6);//;e6;f3;00;00;			   
   SP1628_write_cmos_sensor(0xb5,0x00);//;00;30;30;30;			   
   SP1628_write_cmos_sensor(0xb6,0x03);//;33;33;33;33;			   
   SP1628_write_cmos_sensor(0xb7,0x0f);//;0f;0f;1f;1f;			 

 
  SP1628_write_cmos_sensor(0xfd,0x01);//		;sat u 
  SP1628_write_cmos_sensor(0xd3,0x90);//	过标准105%
  SP1628_write_cmos_sensor(0xd4,0x84);//	
  SP1628_write_cmos_sensor(0xd5,0x78);//		
  SP1628_write_cmos_sensor(0xd6,0x58);//		
  SP1628_write_cmos_sensor(0xd7,0x90);// ;sat v 
  SP1628_write_cmos_sensor(0xd8,0x88);//	
  SP1628_write_cmos_sensor(0xd9,0x78);//		
  SP1628_write_cmos_sensor(0xda,0x58);//		
  SP1628_write_cmos_sensor(0xfd,0x01);//		;auto_sat
  SP1628_write_cmos_sensor(0xd2,0x00);//		;autosa_en
  SP1628_write_cmos_sensor(0xfd,0x01);//		;uv_th	
  SP1628_write_cmos_sensor(0xc2,0xee);//   ;白色物体表面有彩色噪声降低此值  
  SP1628_write_cmos_sensor(0xc3,0xee);//
  SP1628_write_cmos_sensor(0xc4,0xdd);//
  SP1628_write_cmos_sensor(0xc5,0xbb);//
  SP1628_write_cmos_sensor(0xfd,0x01);//		;low_lum_offset
  SP1628_write_cmos_sensor(0xcd,0x10);//
  SP1628_write_cmos_sensor(0xce,0x1f);//
  SP1628_write_cmos_sensor(0xfd,0x02);//		;gw
  SP1628_write_cmos_sensor(0x35,0x6f);//
  SP1628_write_cmos_sensor(0x37,0x13);//
  SP1628_write_cmos_sensor(0xfd,0x01);//		;heq
  SP1628_write_cmos_sensor(0xdb,0x00);//  
  SP1628_write_cmos_sensor(0x10,0x00);// 
  SP1628_write_cmos_sensor(0x14,0x20);// 0x15  //zouyu 20130506
  SP1628_write_cmos_sensor(0x11,0x00);//
  SP1628_write_cmos_sensor(0x15,0x18);//0x10  //zouyu 20130506
  SP1628_write_cmos_sensor(0x16,0x10);// 
  SP1628_write_cmos_sensor(0xfd,0x02);//		;cnr 找张国华解释  
  SP1628_write_cmos_sensor(0x8e,0x10);// 
  SP1628_write_cmos_sensor(0x90,0x20);//
  SP1628_write_cmos_sensor(0x91,0x20);//
  SP1628_write_cmos_sensor(0x92,0x60);//
  SP1628_write_cmos_sensor(0x93,0x80);//
  SP1628_write_cmos_sensor(0xfd,0x02);//		;auto 
  SP1628_write_cmos_sensor(0x85,0x00);//	;12 enable 50Hz/60Hz function
  SP1628_write_cmos_sensor(0xfd,0x01);// 
  SP1628_write_cmos_sensor(0x00,0x00);// 	;fix mode   
  SP1628_write_cmos_sensor(0x32,0x15);//;		;ae en
  SP1628_write_cmos_sensor(0x33,0xcf);//		;lsc\bpc en
  SP1628_write_cmos_sensor(0x34,0xc7);//		;ynr\cnr\gamma\color en
  SP1628_write_cmos_sensor(0x35,0x40);//		;YUYV
  SP1628_write_cmos_sensor(0xfd, 0x00);		  

  }




/*****************************************************************************/
/* Windows Mobile Sensor Interface */
/*****************************************************************************/
/*************************************************************************
* FUNCTION
*	SP1628Open
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
static kal_uint32 SP1628Open(void)

{
    kal_uint16 sensor_id=0;
    kal_uint8 id_h8bit=0;
    kal_uint8 id_l8bit=0;
    int i;
	SENSORDB("Ronlus SP1628Open\r\n");
	// check if sensor ID correct
	for(i = 0; i < 3; i++)
	{
		SP1628_write_cmos_sensor(0xfd,0x00);
		id_h8bit = SP1628_read_cmos_sensor(0x02);
		id_l8bit = SP1628_read_cmos_sensor(0xa0);
		sensor_id=(id_h8bit<<8)|id_l8bit;
		SENSORDB("Ronlus SP1628 Sensor id = %x\n", sensor_id);
		if (sensor_id == SP1628_SENSOR_ID)
		{
			break;
		}
	}
	mdelay(50);
    if(sensor_id != SP1628_SENSOR_ID)
    {
        SENSORDB("SP1628 Sensor id read failed, ID = %x\n", sensor_id);
        return ERROR_SENSOR_CONNECT_FAIL;
    }

	
#ifdef DEBUG_SENSOR_SP1628  //gepeiwei   120903
							//判断手机对应目录下是否有名为sp2528_sd 的文件,没有默认参数
		
							//介于各种原因，本版本初始化参数在_s_fmt中。
		 struct file *fp; 
			mm_segment_t fs; 
			loff_t pos = 0; 
			static char buf[10*1024] ;
		 
			fp = filp_open("/mnt/sdcard/sp1628_sd", O_RDONLY , 0); 
			if (IS_ERR(fp)) { 
				fromsd1 = 0;   
				printk("open file error\n");
				
			} 
			else 
				{
				fromsd1 = 1;
			printk("open file ok\n");
				
			//SP1628_Initialize_from_T_Flash1();
		
		
			filp_close(fp, NULL); 
			set_fs(fs);
			}
#endif  
    //RETAILMSG(1, (TEXT("Sensor Read ID OK \r\n")));
    // initail sequence write in
    SP1628_Sensor_Init();
    return ERROR_NONE;
}   /* SP1628Open  */



/*************************************************************************
* FUNCTION
*	SP1628_GetSensorID
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
static kal_uint32 SP1628_GetSensorID(kal_uint32 *sensorID)

{
		   volatile signed char i;
		kal_uint16 sensor_id=0;
		kal_uint8 id_h8bit=0;
		kal_uint8 id_l8bit=0;
		  SENSORDB("xieyang SP1628GetSensorID ");
	
		  for(i=0;i<3;i++)
		{
			SP1628_write_cmos_sensor(0xfd,0x00);
			id_h8bit = SP1628_read_cmos_sensor(0x02);
			id_l8bit = SP1628_read_cmos_sensor(0xa0);
			sensor_id=(id_h8bit<<8)|id_l8bit;
			SENSORDB("Ronlus SP1628 Sensor id = 0x%x\n", sensor_id);
			if(sensor_id==SP1628_SENSOR_ID)
			{
				*sensorID=SP1628_SENSOR_ID;
				break;
			}
		
	        }
		  if(sensor_id!=SP1628_SENSOR_ID)
		 {
			 *sensorID = 0xFFFFFFFF;
			 return ERROR_SENSOR_CONNECT_FAIL;
		 }
	
		return ERROR_NONE;
}


/*************************************************************************
* FUNCTION
*	SP1628Close
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
static kal_uint32 SP1628Close(void)
{
	kal_uint8 tmp1;
   // tmp1 = closed;
	//CAMERA_CONTROL_FLOW(tmp1,closed++);
   SENSORDB("SP1628Close\n");
	return ERROR_NONE;
}   /* SP1628Close */




static void SP1628_HVMirror(ACDK_SENSOR_IMAGE_MIRROR_ENUM SensorImageMirror)
{
	//volatile kal_uint32 temp_reg2=SP1628_read_cmos_sensor(0x1E), temp_reg1=(temp_reg2&0x0F);
	kal_uint32 iTemp;
	
	kal_uint32 iTemp2;
		return;
		SP1628_write_cmos_sensor(0xfd,0x00);
	iTemp =  SP1628_read_cmos_sensor(0x31);
	#if 0
#if defined(AGOLD_SP1628_YUV_HV_MIRROR) //[Agold][xxd]
	SensorImageMirror  = IMAGE_HV_MIRROR;
#elif defined(AGOLD_SP1628_YUV_H_MIRROR)
	SensorImageMirror   = IMAGE_H_MIRROR;
#elif defined(AGOLD_SP1628_YUV_V_MIRROR)
	SensorImageMirror   = IMAGE_V_MIRROR;
#endif 

        SensorImageMirror ^= IMAGE_HV_MIRROR; //[Agold][xxd][add for qq video rotate 180]   
      #endif
	iTemp2= iTemp;
	switch (SensorImageMirror)
	{
		case IMAGE_NORMAL:
			SP1628_write_cmos_sensor(0xfd,0x00);
			SP1628_write_cmos_sensor(0x31,iTemp2);

			
			break;
		case IMAGE_H_MIRROR:			 

			if((iTemp2 & 0x20)==0x20)
				iTemp2 &= (~0x20);
			else
				iTemp2 |= 0x20;
			
			SP1628_write_cmos_sensor(0xfd,0x00);
			SP1628_write_cmos_sensor(0x31,iTemp2);
			break;
		case IMAGE_V_MIRROR:			 
			if((iTemp2 & 0x40)==0x40)
				iTemp2 &= (~0x40);
			else
				iTemp2 |= 0x40;
			
			SP1628_write_cmos_sensor(0xfd,0x00);
			SP1628_write_cmos_sensor(0x31,iTemp2);
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

			SP1628_write_cmos_sensor(0xfd,0x00);
			SP1628_write_cmos_sensor(0x31,iTemp2);
			break;
	}


}
/*************************************************************************
* FUNCTION
* SP1628_Preview
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
static kal_uint32 SP1628_Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)

{
	SP1628_Sensor_Driver.fPV_PCLK=24000000;//26000000
	SP1628_Sensor_Driver.MODE_CAPTURE=KAL_FALSE;

	if(sensor_config_data->SensorOperationMode==MSDK_SENSOR_OPERATION_MODE_VIDEO){
		SP1628_Sensor_Driver.MPEG4_encode_mode = KAL_TRUE;  // MPEG4 Encode Mode
	}else{
		SP1628_Sensor_Driver.MPEG4_encode_mode = KAL_FALSE;  
	}


	// SP1628_HVMirror(sensor_config_data->SensorImageMirror);

//	SP1628_Sensor_Driver.dummy_pixels = 0;
//	SP1628_Sensor_Driver.dummy_lines = 42;
//	SP1628_Sensor_Driver.iPV_Pixels_Per_Line =VGA_PERIOD_PIXEL_NUMS+SP1628_Sensor_Driver.dummy_pixels;  
//	SP1628_Set_Dummy(SP1628_Sensor_Driver.dummy_pixels, SP1628_Sensor_Driver.dummy_lines);

	
	image_window->GrabStartX= IMAGE_SENSOR_VGA_INSERTED_PIXELS;
	image_window->GrabStartY= IMAGE_SENSOR_VGA_INSERTED_LINES;
	image_window->ExposureWindowWidth = IMAGE_SENSOR_PV_WIDTH;
	image_window->ExposureWindowHeight =IMAGE_SENSOR_PV_HEIGHT;

	if(KAL_TRUE == SP1628_Sensor_Driver.bNight_mode) // for nd 128 noise,decrease color matrix
	{
	}

	// copy sensor_config_data
	memcpy(&SP1628SensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	return ERROR_NONE;

}   /*  SP1628_Preview   */

/*************************************************************************
* FUNCTION
*	SP1628_Capture
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
static kal_uint32 SP1628_Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
						  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)

{
}   /* OV7576_Capture() */
#endif

static kal_uint32 SP1628GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
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
}	/* SP1628GetResolution() */

static kal_uint32 SP1628GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
					  MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	
		SENSORDB("SP1628GetInfo \n");
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
		//pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;
	
		pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_UYVY;//SENSOR_OUTPUT_FORMAT_YUYV;
		pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;
		pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
		pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
		pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_HIGH;
//		   pSensorInfo->SensorDriver3D = 0;   // the sensor driver is 2D
		pSensorInfo->SensorInterruptDelayLines = 1;
		pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_PARALLEL;//SENSOR_INTERFACE_TYPE_MIPI;
		pSensorInfo->CaptureDelayFrame = 1;
		pSensorInfo->PreviewDelayFrame = 0;
		pSensorInfo->VideoDelayFrame = 4;
		pSensorInfo->SensorMasterClockSwitch = 0; 
	       pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_2MA;

	
		switch (ScenarioId)
		{
			case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			//case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
				 
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
	memcpy(pSensorConfigData, &SP1628SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	
	return ERROR_NONE;
}	/* SP1628GetInfo() */


static kal_uint32 SP1628Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	CAMERA_CONTROL_FLOW(ScenarioId,ScenarioId);

	switch (ScenarioId)
	{
		//case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:// MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
			SP1628_Preview(pImageWindow, pSensorConfigData);
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		//case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
			//SP1628_Capture(pImageWindow, pSensorConfigData);
			SP1628_Preview(pImageWindow, pSensorConfigData);
		break;
		default:
			return ERROR_INVALID_SCENARIO_ID;
	}
	return TRUE;
}	/* MT9P012Control() */



static BOOL SP1628_set_param_wb(UINT16 para)
{
	kal_uint8  temp_reg;

	//if(SP1628_Sensor_Driver.u8Wb_value==para)
		//return FALSE;

	
	SP1628_Sensor_Driver.u8Wb_value = para;

	switch (para)
	 {
		 case AWB_MODE_OFF:
		 //SP1628_write_cmos_sensor(0xfd,0x00);				   
		 //SP1628_write_cmos_sensor(0x32,0x05);	   
		 break;
			 
		 case AWB_MODE_AUTO: // AUTO 3000K~7000K 
		
			 
			SP1628_write_cmos_sensor(0xfd,0x02);
			SP1628_write_cmos_sensor(0x26,0xc9);
			SP1628_write_cmos_sensor(0x27,0x8b);
			SP1628_write_cmos_sensor(0xfd,0x00);
			SP1628_write_cmos_sensor(0xe7,0x03);
			SP1628_write_cmos_sensor(0xe7,0x00);
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0x32,0x15);


			 break;
	
		 case AWB_MODE_CLOUDY_DAYLIGHT: //cloudy   //7000K
			 SP1628_write_cmos_sensor(0xfd,0x01);
			 SP1628_write_cmos_sensor(0x32,0x05);
											 
			 SP1628_write_cmos_sensor(0xfd,0x00);
			 SP1628_write_cmos_sensor(0xe7,0x03);
											 
			 SP1628_write_cmos_sensor(0xfd,0x02);
			 SP1628_write_cmos_sensor(0x26,0xdb);
			 SP1628_write_cmos_sensor(0x27,0x63);
											 
			 SP1628_write_cmos_sensor(0xfd,0x00);
			 SP1628_write_cmos_sensor(0xe7,0x00);
			 break;
	
		 case AWB_MODE_DAYLIGHT: //sunny   //6500K 	
		 // SP1628_reg_WB_auto	
		 SP1628_write_cmos_sensor(0xfd,0x01);
		 SP1628_write_cmos_sensor(0x32,0x05);
										 
		 SP1628_write_cmos_sensor(0xfd,0x00);
		 SP1628_write_cmos_sensor(0xe7,0x03);
										 
		 SP1628_write_cmos_sensor(0xfd,0x02);
		 SP1628_write_cmos_sensor(0x26,0xca);
		 SP1628_write_cmos_sensor(0x27,0x73);
										 
		 SP1628_write_cmos_sensor(0xfd,0x00);
		 SP1628_write_cmos_sensor(0xe7,0x00);

			 break;
	
		 case AWB_MODE_INCANDESCENT: //office   //2800K~3000K	
			 // SP1628_reg_WB_auto 
			 SP1628_write_cmos_sensor(0xfd,0x01);
			 SP1628_write_cmos_sensor(0x32,0x05);
												 
			 SP1628_write_cmos_sensor(0xfd,0x00);
			 SP1628_write_cmos_sensor(0xe7,0x03);
												 
			 SP1628_write_cmos_sensor(0xfd,0x02);
			 SP1628_write_cmos_sensor(0x26,0x8c);
			 SP1628_write_cmos_sensor(0x27,0xb3);
												 
			 SP1628_write_cmos_sensor(0xfd,0x00);
			 SP1628_write_cmos_sensor(0xe7,0x00);
			 break;
	
		 case AWB_MODE_TUNGSTEN: //home  //4000K 
		 // SP1628_reg_WB_auto 
		 SP1628_write_cmos_sensor(0xfd,0x01);
		 SP1628_write_cmos_sensor(0x32,0x05);
											 
		 SP1628_write_cmos_sensor(0xfd,0x00);
		 SP1628_write_cmos_sensor(0xe7,0x03);
											 
		 SP1628_write_cmos_sensor(0xfd,0x02);
		 SP1628_write_cmos_sensor(0x26,0x90);
		 SP1628_write_cmos_sensor(0x27,0xa5);
											 
		 SP1628_write_cmos_sensor(0xfd,0x00);
		 SP1628_write_cmos_sensor(0xe7,0x00);
			 break;
			 
		 case AWB_MODE_FLUORESCENT: //4200K 	~ 5000K
		 // SP1628_reg_WB_auto 
		 SP1628_write_cmos_sensor(0xfd,0x01);
		 SP1628_write_cmos_sensor(0x32,0x05);
											 
		 SP1628_write_cmos_sensor(0xfd,0x00);
		 SP1628_write_cmos_sensor(0xe7,0x03);
											 
		 SP1628_write_cmos_sensor(0xfd,0x02);
		 SP1628_write_cmos_sensor(0x26,0x95);
		 SP1628_write_cmos_sensor(0x27,0x9c);
											 
		 SP1628_write_cmos_sensor(0xfd,0x00);
		 SP1628_write_cmos_sensor(0xe7,0x00);
			 break;
	
		 default:
			 return FALSE;
	 }


	return TRUE;
} /* SP1628_set_param_wb */


static BOOL SP1628_set_param_effect(UINT16 para)
{
	kal_uint32 ret = KAL_TRUE;

	//if(para==SP1628_Sensor_Driver.u8Effect_value)
		//return FALSE;

	
	SP1628_Sensor_Driver.u8Effect_value = para;
    switch (para)
    {
        case MEFFECT_OFF:  
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0x66,0x00);
			SP1628_write_cmos_sensor(0x67,0x80);
			SP1628_write_cmos_sensor(0x68,0x80);
			SP1628_write_cmos_sensor(0xdb,0x00);
			SP1628_write_cmos_sensor(0x34,0xc7);
			SP1628_write_cmos_sensor(0xfd,0x02);
			SP1628_write_cmos_sensor(0x14,0x00);
            break;

        case MEFFECT_SEPIA:  
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0x66,0x10);
			SP1628_write_cmos_sensor(0x67,0x98);
			SP1628_write_cmos_sensor(0x68,0x58);
			SP1628_write_cmos_sensor(0xdb,0x00);
			SP1628_write_cmos_sensor(0x34,0xc7);
			SP1628_write_cmos_sensor(0xfd,0x02);
			SP1628_write_cmos_sensor(0x14,0x00);

            break;

        case MEFFECT_NEGATIVE: 
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0x66,0x08);
			SP1628_write_cmos_sensor(0x67,0x80);
			SP1628_write_cmos_sensor(0x68,0x80);
			SP1628_write_cmos_sensor(0xdb,0x00);
			SP1628_write_cmos_sensor(0x34,0xc7);
			SP1628_write_cmos_sensor(0xfd,0x02);
			SP1628_write_cmos_sensor(0x14,0x00);
            break;

        case MEFFECT_SEPIAGREEN:
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0x66,0x10);
			SP1628_write_cmos_sensor(0x67,0x50);
			SP1628_write_cmos_sensor(0x68,0x50);
			SP1628_write_cmos_sensor(0xdb,0x00);
			SP1628_write_cmos_sensor(0x34,0xc7);
			SP1628_write_cmos_sensor(0xfd,0x02);
			SP1628_write_cmos_sensor(0x14,0x00);
            break;

        case MEFFECT_SEPIABLUE:
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0x66,0x10);
			SP1628_write_cmos_sensor(0x67,0x80);
			SP1628_write_cmos_sensor(0x68,0xb0);
			SP1628_write_cmos_sensor(0xdb,0x00);
			SP1628_write_cmos_sensor(0x34,0xc7);
			SP1628_write_cmos_sensor(0xfd,0x02);
			SP1628_write_cmos_sensor(0x14,0x00);

            break;
			
		case MEFFECT_MONO: //B&W
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0x66,0x20);
			SP1628_write_cmos_sensor(0x67,0x80);
			SP1628_write_cmos_sensor(0x68,0x80);
			SP1628_write_cmos_sensor(0xdb,0x00);
			SP1628_write_cmos_sensor(0x34,0xc7);
			SP1628_write_cmos_sensor(0xfd,0x02);
			SP1628_write_cmos_sensor(0x14,0x00);

			break;
        default:
            return FALSE;
    }

	return ret;

} /* SP1628_set_param_effect */

static void SP1628_set_banding_for_50Hz(void)
{
   printk("SP1628_set_banding_for_50Hz\n");
    // 48MHz 9~15fps 50Hz  1280x960  
		SP1628_write_cmos_sensor(0xfd,0x00);//ae setting
		SP1628_write_cmos_sensor(0x03,0x03);
		SP1628_write_cmos_sensor(0x04,0x90);
		SP1628_write_cmos_sensor(0x05,0x00);
		SP1628_write_cmos_sensor(0x06,0x00);
		SP1628_write_cmos_sensor(0x09,0x00);
		SP1628_write_cmos_sensor(0x0a,0x09);
		SP1628_write_cmos_sensor(0xfd,0x01);
		SP1628_write_cmos_sensor(0xf0,0x00);
		SP1628_write_cmos_sensor(0xf7,0x98);
		SP1628_write_cmos_sensor(0xf8,0x80);
		SP1628_write_cmos_sensor(0x02,0x0b);
		SP1628_write_cmos_sensor(0x03,0x01);
		SP1628_write_cmos_sensor(0x06,0x98);
		SP1628_write_cmos_sensor(0x07,0x00);
		SP1628_write_cmos_sensor(0x08,0x01);
		SP1628_write_cmos_sensor(0x09,0x00);
		SP1628_write_cmos_sensor(0xfd,0x02);
		SP1628_write_cmos_sensor(0x40,0x0d);
		SP1628_write_cmos_sensor(0x41,0x80);
		SP1628_write_cmos_sensor(0x42,0x00);
		SP1628_write_cmos_sensor(0x88,0x5e);
		SP1628_write_cmos_sensor(0x89,0x00);
		SP1628_write_cmos_sensor(0x8a,0x43);
		SP1628_write_cmos_sensor(0xfd,0x02);//Status
		SP1628_write_cmos_sensor(0xbe,0x88);
		SP1628_write_cmos_sensor(0xbf,0x06);
		SP1628_write_cmos_sensor(0xd0,0x88);
		SP1628_write_cmos_sensor(0xd1,0x06);
		SP1628_write_cmos_sensor(0xfd,0x01);
		SP1628_write_cmos_sensor(0x5b,0x06);
		SP1628_write_cmos_sensor(0x5c,0x88);
		SP1628_write_cmos_sensor(0xfd,0x00);
}


static void SP1628_set_banding_for_60Hz(void)
{
  printk("SP1628_set_banding_for_60Hz\n");

    // 48MHz 9~15fps 60Hz  1280x960  
		SP1628_write_cmos_sensor(0xfd,0x00);//ae setting
		SP1628_write_cmos_sensor(0x03,0x03);
		SP1628_write_cmos_sensor(0x04,0x00);
		SP1628_write_cmos_sensor(0x05,0x00);
		SP1628_write_cmos_sensor(0x06,0x00);
		SP1628_write_cmos_sensor(0x09,0x00);
		SP1628_write_cmos_sensor(0x0a,0x01);
		SP1628_write_cmos_sensor(0xfd,0x01);
		SP1628_write_cmos_sensor(0xf0,0x00);
		SP1628_write_cmos_sensor(0xf7,0x80);
		SP1628_write_cmos_sensor(0xf8,0x80);
		SP1628_write_cmos_sensor(0x02,0x0d);
		SP1628_write_cmos_sensor(0x03,0x01);
		SP1628_write_cmos_sensor(0x06,0x80);
		SP1628_write_cmos_sensor(0x07,0x00);
		SP1628_write_cmos_sensor(0x08,0x01);
		SP1628_write_cmos_sensor(0x09,0x00);
		SP1628_write_cmos_sensor(0xfd,0x02);
		SP1628_write_cmos_sensor(0x40,0x0d);
		SP1628_write_cmos_sensor(0x41,0x80);
		SP1628_write_cmos_sensor(0x42,0x00);
		SP1628_write_cmos_sensor(0x88,0x00);
		SP1628_write_cmos_sensor(0x89,0x00);
		SP1628_write_cmos_sensor(0x8a,0x44);
		SP1628_write_cmos_sensor(0xfd,0x02);//Status
		SP1628_write_cmos_sensor(0xbe,0x80);
		SP1628_write_cmos_sensor(0xbf,0x06);
		SP1628_write_cmos_sensor(0xd0,0x80);
		SP1628_write_cmos_sensor(0xd1,0x06);
		SP1628_write_cmos_sensor(0xfd,0x01);
		SP1628_write_cmos_sensor(0x5b,0x06);
		SP1628_write_cmos_sensor(0x5c,0x80);
		SP1628_write_cmos_sensor(0xfd,0x00);
}

static BOOL SP1628_set_param_banding(UINT16 para)
{
	//if(SP1628_Sensor_Driver.bBanding_value == para)

	SP1628_Sensor_Driver.bBanding_value = para;
	
	switch (para)
	{
		case AE_FLICKER_MODE_50HZ:
			isBanding = 0;
			printk("SP1628_set_param_banding_50hz\n");
			SP1628_set_banding_for_50Hz();
			break;
		case AE_FLICKER_MODE_60HZ:
			isBanding = 1;
			printk("SP1628_set_param_banding_60hz\n");
			SP1628_set_banding_for_60Hz();
			break;
		default:
			return FALSE;
	}

	return TRUE;
} /* SP1628_set_param_banding */
static BOOL SP1628_set_param_exposure(UINT16 para)
{
	//if(para == SP1628_Sensor_Driver.u8Ev_value)
		//return FALSE;

	SP1628_Sensor_Driver.u8Ev_value = para;

    switch (para)
    {
        case AE_EV_COMP_n13:
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0xeb,0x38);
			SP1628_write_cmos_sensor(0xec,0x38);

            break;

        case AE_EV_COMP_n10:
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0xeb,0x48);
			SP1628_write_cmos_sensor(0xec,0x48);

            break;

        case AE_EV_COMP_n07:
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0xeb,0x58);
			SP1628_write_cmos_sensor(0xec,0x58);

            break;

        case AE_EV_COMP_n03:
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0xeb,0x68);
			SP1628_write_cmos_sensor(0xec,0x68);
            break;

        case AE_EV_COMP_00:
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0xeb,0x78);
			SP1628_write_cmos_sensor(0xec,0x78);
            break;

        case AE_EV_COMP_03:
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0xeb,0x88);
			SP1628_write_cmos_sensor(0xec,0x88);
            break;

        case AE_EV_COMP_07:
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0xeb,0x98);
			SP1628_write_cmos_sensor(0xec,0x98);

            break;

        case AE_EV_COMP_10:
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0xeb,0xa8);
			SP1628_write_cmos_sensor(0xec,0xa8);

            break;

        case AE_EV_COMP_13:
			SP1628_write_cmos_sensor(0xfd,0x01);
			SP1628_write_cmos_sensor(0xeb,0xb8);
			SP1628_write_cmos_sensor(0xec,0xb8);

            break;

        default:
            return FALSE;
    }


	return TRUE;
} /* SP1628_set_param_exposure */

static kal_uint32 SP1628_YUVSensorSetting(FEATURE_ID iCmd, UINT16 iPara)
{


#ifdef DEBUG_SENSOR_SP1628
	return TRUE;
#endif

	switch (iCmd) {
		case FID_SCENE_MODE:
		    if (iPara == SCENE_MODE_OFF){
		        SP1628_night_mode(FALSE); 
		    }else if (iPara == SCENE_MODE_NIGHTSCENE){
               SP1628_night_mode(TRUE); 
		    }	    
		  
		    break; 
		case FID_AWB_MODE:
			SP1628_set_param_wb(iPara);
		break;
		case FID_COLOR_EFFECT:
			SP1628_set_param_effect(iPara);
		break;
		case FID_AE_EV:	
			SP1628_set_param_exposure(iPara);
		break;
		case FID_AE_FLICKER:
			SP1628_set_param_banding(iPara);
			//whl120717 test
			 if (SP1628_Sensor_Driver.bNight_mode == KAL_FALSE){
		        SP1628_night_mode(FALSE); 
		    }else if (SP1628_Sensor_Driver.bNight_mode == KAL_TRUE){
               	SP1628_night_mode(TRUE); 
        
		    }	
		      
		break;
		default:
		break;
	}
	
	return TRUE;
}   /* SP1628_YUVSensorSetting */

static kal_uint32 SP1628_YUVSetVideoMode(UINT16 u2FrameRate)
{
    kal_uint8 temp ;//= SP1628_read_cmos_sensor(0x3B);
    SP1628_Sensor_Driver.MPEG4_encode_mode = KAL_TRUE; 

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
    
	printk("\n SP1628_YUVSetVideoMode:u2FrameRate=%d\n\n",u2FrameRate);
    return TRUE;
}

UINT32 SP1628SetSoftwarePWDNMode(kal_bool bEnable)
{
#if 0
    SENSORDB("[SP1628SetSoftwarePWDNMode] Software Power down enable:%d\n", bEnable);
    
    if(bEnable) {   // enable software sleep mode   
	 SP1628_write_cmos_sensor(0x09, 0x10);
    } else {
        SP1628_write_cmos_sensor(0x09, 0x03);  
    }
#endif
    return TRUE;
}

/*************************************************************************
* FUNCTION
*    SP1628_get_size
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
static void SP1628_get_size(kal_uint16 *sensor_width, kal_uint16 *sensor_height)
{
  *sensor_width = IMAGE_SENSOR_FULL_WIDTH; /* must be 4:3 */
  *sensor_height = IMAGE_SENSOR_FULL_HEIGHT;
}

/*************************************************************************
* FUNCTION
*    SP1628_get_period
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
static void SP1628_get_period(kal_uint16 *pixel_number, kal_uint16 *line_number)
{
  *pixel_number = VGA_PERIOD_PIXEL_NUMS+SP1628_Sensor_Driver.dummy_pixels;
  *line_number = VGA_PERIOD_LINE_NUMS+SP1628_Sensor_Driver.dummy_lines;
}

/*************************************************************************
* FUNCTION
*    SP1628_feature_control
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
static kal_uint32 SP1628FeatureControl(MSDK_SENSOR_FEATURE_ENUM id, kal_uint8 *para, kal_uint32 *len)
{
	UINT32 *pFeatureData32=(UINT32 *) para;

	switch (id)
	{
		case SENSOR_FEATURE_GET_RESOLUTION: /* no use */
			SP1628_get_size((kal_uint16 *)para, (kal_uint16 *)(para + sizeof(kal_uint16)));
			*len = sizeof(kal_uint32);
			break;
		case SENSOR_FEATURE_GET_PERIOD:
			SP1628_get_period((kal_uint16 *)para, (kal_uint16 *)(para + sizeof(kal_uint16)));
			*len = sizeof(kal_uint32);
			break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			*(kal_uint32 *)para = SP1628_Sensor_Driver.fPV_PCLK;
			*len = sizeof(kal_uint32);
			break;
		case SENSOR_FEATURE_SET_ESHUTTER:
			break;
		case SENSOR_FEATURE_SET_NIGHTMODE: 
#ifndef DEBUG_SENSOR_SP1628      
					SP1628_night_mode((kal_bool)*(kal_uint16 *)para);
#endif
			break;
		case SENSOR_FEATURE_SET_GAIN:
		case SENSOR_FEATURE_SET_FLASHLIGHT:
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			break;
		case SENSOR_FEATURE_SET_REGISTER:
			SP1628_write_cmos_sensor(((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegAddr, ((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegData);
			break;
		case SENSOR_FEATURE_GET_REGISTER: /* 10 */
			((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegData = SP1628_read_cmos_sensor(((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegAddr);
			break;
		case SENSOR_FEATURE_SET_CCT_REGISTER:
			memcpy(&SP1628_Sensor_Driver.eng.CCT, para, sizeof(SP1628_Sensor_Driver.eng.CCT));
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
	//		SP1628_YUVSensorSetting((FEATURE_ID)(UINT32 *)para, (UINT32 *)(para+1));
			
			SP1628_YUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
			break;
#if 0		    		
		case SENSOR_FEATURE_QUERY:
			SP1628_Query(pSensorFeatureInfo);
			*pFeatureParaLen = sizeof(MSDK_FEATURE_INFO_STRUCT);
			break;		
		case SENSOR_FEATURE_SET_YUV_CAPTURE_RAW_SUPPORT:
			/* update yuv capture raw support flag by *pFeatureData16 */
			break;		
#endif 			
		case SENSOR_FEATURE_SET_VIDEO_MODE:
			SP1628_YUVSetVideoMode(*para);
			break;
              case SENSOR_FEATURE_CHECK_SENSOR_ID:
                     SP1628_GetSensorID(pFeatureData32); 
                     break; 	
              case SENSOR_FEATURE_SET_SOFTWARE_PWDN:
                     SP1628SetSoftwarePWDNMode((BOOL)*pFeatureData32);        	        	
                     break;
		default:
			break;
	}
	return ERROR_NONE;
}





SENSOR_FUNCTION_STRUCT	SensorFuncSP1628YUV=
{
	SP1628Open,
	SP1628GetInfo,
	SP1628GetResolution,
	SP1628FeatureControl,
	SP1628Control,
	SP1628Close
};

UINT32 SP1628_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
	/* To Do : Check Sensor status here */
	SENSORDB("Ronlus SP1628_YUV_SensorInit\r\n");
	if (pfFunc!=NULL)
	{
		 SENSORDB("Ronlus SP1628_YUV_SensorInit fun_config success\r\n");
		*pfFunc=&SensorFuncSP1628YUV;
	}
	return ERROR_NONE;
}	/* SensorInit() */




