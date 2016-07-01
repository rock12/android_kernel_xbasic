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

#include "HM5065yuv_Sensor.h"
#include "HM5065yuv_Camera_Sensor_para.h"
#include "HM5065yuv_CameraCustomized.h"

static MSDK_SENSOR_CONFIG_STRUCT HM5065SensorConfigData;
static struct HM5065_Sensor_Struct HM5065_Sensor_Driver;

static kal_uint32 AF_XS = 40;//version0.21, aug.2009
static kal_uint32 AF_YS = 30;//version0.21, aug.2009
static UINT16 XSTART,YSTART ;

//static kal_bool AF_INIT = FALSE;
static UINT8 ZONE[4] = {120, 90, 40, 30};////version0.21, aug.2009,center 4:3 window

#define HM5065YUV_DEBUG
#ifdef HM5065YUV_DEBUG
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

#define FACE_LC 			0x0714
#define FACE_START_XH 	0x0715
#define FACE_START_XL 	0x0716
#define FACE_SIZE_XH  	0x0717
#define FACE_SIZE_XL	 0x0718
#define FACE_START_YH	 0x0719
#define FACE_START_YL	 0x071A
#define FACE_SIZE_YH	 0x071B
#define FACE_SIZE_YL 	0x071C

#define AF_CANCEL	3
#define AF_INF 		2
#define AF_SINGLE   1
#define AF_CONSTANT 0
static kal_uint8 res=0,closed=0,info=0;
static kal_uint32 Capture_Shutter = 0;
static kal_uint32 Capture_Gain = 0;

static kal_uint8 gain[4] = {0};
static kal_uint8 shutter[4] = {0};
static kal_uint32 af_type = AF_CONSTANT;
static kal_uint8 af_pos_h = 0;
static kal_uint8 af_pos_l = 0;
static kal_uint8 af_error_flag = 0;
static struct timeval tv;


#if 0
extern int iReadReg(u16 a_u2Addr , u8 * a_puBuff , u16 i2cId);
extern int iWriteReg(u16 a_u2Addr , u32 a_u4Data , u32 a_u4Bytes , u16 i2cId);
static int sensor_id_fail = 0; 
#define HM5065_write_cmos_sensor(addr,para) iWriteReg((u16) addr , (u32) para ,1,HM5065_WRITE_ID)
//#define HM5065_write_cmos_sensor_2(addr, para, bytes) iWriteReg((u16) addr , (u32) para ,bytes,HM5065_WRITE_ID)
kal_uint16 HM5065_read_cmos_sensor(kal_uint32 addr)
{
kal_uint16 get_byte=0;
    iReadReg((u16) addr ,(u8*)&get_byte,HM5065_WRITE_ID);
    return get_byte;
}
#endif

extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);
/*************************************************************************
* FUNCTION
*    HM5065_write_cmos_sensor
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
static void HM5065_write_cmos_sensor(kal_uint32 addr, kal_uint32 para)
{
	char puSendCmd[3] = {(char)(addr >> 8) , (char)(addr & 0xFF) ,(char)(para & 0xFF)};
	   
	   iWriteRegI2C(puSendCmd , 3,HM5065_WRITE_ID);
	   //iWriteReg((u16) addr , (u32) para ,1,HM5065_WRITE_ID);
}

/*************************************************************************
* FUNCTION
*    HM5065_read_cmos_sensor
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
static kal_uint8 HM5065_read_cmos_sensor(kal_uint32 addr)
{
	kal_uint16 get_byte=0;
	char puSendCmd[2] = {(char)(addr >> 8) , (char)(addr & 0xFF) };
	iReadRegI2C(puSendCmd , 2, (u8*)&get_byte,1,HM5065_WRITE_ID);
	return get_byte;
}



 static void HM5065_Set_Dummy(const kal_uint16 iPixels, const kal_uint16 iLines)
{

}   /*  HM5065_Set_Dummy    */


/*************************************************************************
* FUNCTION
*	HM5065_write_reg
*
* DESCRIPTION
*	This function set the register of HM5065.
*
* PARAMETERS
*	addr : the register index of 		hm5065
*  para : setting parameter of the specified register of hm5065
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/

//static void HM5065_write_reg(kal_uint32 addr, kal_uint32 para)
//{
//	HM5065_write_cmos_sensor(addr,para);
//}	/* HM5065_write_reg() */

/*************************************************************************
* FUNCTION
*hm5065_read_cmos_sensor
*
* DESCRIPTION
*	This function read parameter of specified register from hm5065.
*
* PARAMETERS
*	addr : the register index of hm5065
*
* RETURNS
*	the data that read from hm5065
*
* GLOBALS AFFECTED
*
*************************************************************************/
//static kal_uint32 HM5065_read_reg(kal_uint32 addr)
//{
//	return (HM5065_read_cmos_sensor(addr));
//}	/* HM5065_read_reg() */


//add by lingnan for af status
static UINT8 STA_FOCUS = 0x8F; 
//static kal_uint32 MAC = 255;
//static kal_uint32 INF = 0;
//static kal_uint32 AF_XS = 40;//version2.01, aug.2009
//static kal_uint32 AF_YS = 30;//version2.01, aug.2009
static kal_bool AF_INIT = FALSE;
//static UINT8 ZONE[4] = {16, 11, 24, 19};////version0.21, aug.2009,center 4:3 window
//static UINT8 af_pos_h = 0 ;
//static UINT8 af_pos_l = 0 ;

//SENSOR_REG_STRUCT HM5065SensorCCT[FACTORY_END_ADDR]=CAMERA_SENSOR_CCT_DEFAULT_VALUE;
//SENSOR_REG_STRUCT HM5065SensorReg[ENGINEER_END]=CAMERA_SENSOR_REG_DEFAULT_VALUE;
//	camera_para.SENSOR.cct	SensorCCT	=> SensorCCT
//	camera_para.SENSOR.reg	SensorReg
//MSDK_SENSOR_CONFIG_STRUCT HM5065SensorConfigData;


//=====================touch AE begin==========================//
typedef enum
{
    AE_SECTION_INDEX_BEGIN=0, 
    AE_SECTION_INDEX_1=AE_SECTION_INDEX_BEGIN, 
    AE_SECTION_INDEX_2, 
    AE_SECTION_INDEX_3, 
    AE_SECTION_INDEX_4, 
    AE_SECTION_INDEX_5, 
    AE_SECTION_INDEX_6, 
    AE_SECTION_INDEX_7, 
    AE_SECTION_INDEX_8, 
    AE_SECTION_INDEX_9, 
    AE_SECTION_INDEX_10, 
    AE_SECTION_INDEX_11, 
    AE_SECTION_INDEX_12, 
    AE_SECTION_INDEX_13, 
    AE_SECTION_INDEX_14, 
    AE_SECTION_INDEX_15, 
    AE_SECTION_INDEX_16,  
    AE_SECTION_INDEX_MAX
}AE_SECTION_INDEX;

typedef enum
{
    AE_VERTICAL_BLOCKS=4,
    AE_VERTICAL_BLOCKS_MAX,
    AE_HORIZONTAL_BLOCKS=4,
    AE_HORIZONTAL_BLOCKS_MAX
}AE_VERTICAL_HORIZONTAL_BLOCKS;

typedef enum
{
    PRV_W=640,
    PRV_H=480
}PREVIEW_VIEW_SIZE;

static UINT32 line_coordinate[AE_VERTICAL_BLOCKS_MAX] = {0};//line[0]=0      line[1]=160     line[2]=320     line[3]=480     line[4]=640
static UINT32 row_coordinate[AE_HORIZONTAL_BLOCKS_MAX] = {0};//line[0]=0       line[1]=120     line[2]=240     line[3]=360     line[4]=480
static BOOL AE_1_ARRAY[AE_SECTION_INDEX_MAX] = {FALSE};
static BOOL AE_2_ARRAY[AE_HORIZONTAL_BLOCKS][AE_VERTICAL_BLOCKS] = {{FALSE},{FALSE},{FALSE},{FALSE}};//how to ....

void readAEReg(void)
{
	return 0 ;

}
void writeAEReg(void)
{	
	return 0 ;  
}


void printAE_1_ARRAY(void)
{
    UINT32 i;
    for(i=0; i<AE_SECTION_INDEX_MAX; i++)
    {
        SENSORDB("AE_1_ARRAY[%2d]=%d\n", i, AE_1_ARRAY[i]);
    }
}

void printAE_2_ARRAY(void)
{
    UINT32 i, j;
    SENSORDB("\t\t");
    for(i=0; i<AE_VERTICAL_BLOCKS; i++)
    {
        printk("      line[%2d]", i);
    }
    printk("\n");
    for(j=0; j<AE_HORIZONTAL_BLOCKS; j++)
    {
        SENSORDB("\trow[%2d]", j);
        for(i=0; i<AE_VERTICAL_BLOCKS; i++)
        {
            //SENSORDB("AE_2_ARRAY[%2d][%2d]=%d\n", j,i,AE_2_ARRAY[j][i]);
            printk("  %7d", AE_2_ARRAY[j][i]);
        }
        printk("\n");
    }
}

void clearAE_2_ARRAY(void)
{
    UINT32 i, j;
    for(j=0; j<AE_HORIZONTAL_BLOCKS; j++)
    {
        for(i=0; i<AE_VERTICAL_BLOCKS; i++)
        {AE_2_ARRAY[j][i]=FALSE;}
    }
}

void mapAE_2_ARRAY_To_AE_1_ARRAY(void)
{
    UINT32 i, j;
    for(j=0; j<AE_HORIZONTAL_BLOCKS; j++)
    {
        for(i=0; i<AE_VERTICAL_BLOCKS; i++)
        { AE_1_ARRAY[j*AE_VERTICAL_BLOCKS+i] = AE_2_ARRAY[j][i];}
    }
}

void mapMiddlewaresizePointToPreviewsizePoint(
    UINT32 mx,
    UINT32 my,
    UINT32 mw,
    UINT32 mh,
    UINT32 * pvx,
    UINT32 * pvy,
    UINT32 pvw,
    UINT32 pvh
)
{
    *pvx = pvw * mx / mw;
    *pvy = pvh * my / mh;
    SENSORDB("mapping middlware x[%d],y[%d], [%d X %d]\n\t\tto x[%d],y[%d],[%d X %d]\n ",
        mx, my, mw, mh, *pvx, *pvy, pvw, pvh);
}


void calcLine(void)
{//line[5]
    UINT32 i;
    UINT32 step = PRV_W / AE_VERTICAL_BLOCKS;
    for(i=0; i<=AE_VERTICAL_BLOCKS; i++)
    {
        *(&line_coordinate[0]+i) = step*i;
        SENSORDB("line[%d]=%d\t",i, *(&line_coordinate[0]+i));
    }
    SENSORDB("\n");
}

void calcRow(void)
{//row[5]
    UINT32 i;
    UINT32 step = PRV_H / AE_HORIZONTAL_BLOCKS;
    for(i=0; i<=AE_HORIZONTAL_BLOCKS; i++)
    {
        *(&row_coordinate[0]+i) = step*i;
        SENSORDB("row[%d]=%d\t",i,*(&row_coordinate[0]+i));
    }
    SENSORDB("\n");
}

void calcPointsAELineRowCoordinate(UINT32 x, UINT32 y, UINT32 * linenum, UINT32 * rownum)
{
    UINT32 i;
    i = 1;
    while(i<=AE_VERTICAL_BLOCKS)
    {
        if(x<line_coordinate[i])
        {
            *linenum = i;
            break;
        }
        *linenum = i++;
    }
    
    i = 1;
    while(i<=AE_HORIZONTAL_BLOCKS)
    {
        if(y<row_coordinate[i])
        {
            *rownum = i;
            break;
        }
        *rownum = i++;
    }
    SENSORDB("PV point [%d, %d] to section line coordinate[%d] row[%d]\n",x,y,*linenum,*rownum);
}



MINT32 clampSection(UINT32 x, UINT32 min, UINT32 max)
{
    if (x > max) return max;
    if (x < min) return min;
    return x;
}

void mapCoordinate(UINT32 linenum, UINT32 rownum, UINT32 * sectionlinenum, UINT32 * sectionrownum)
{
    *sectionlinenum = clampSection(linenum-1,0,AE_VERTICAL_BLOCKS-1);
    *sectionrownum = clampSection(rownum-1,0,AE_HORIZONTAL_BLOCKS-1);	
    SENSORDB("mapCoordinate from[%d][%d] to[%d][%d]\n",
		linenum, rownum,*sectionlinenum,*sectionrownum);
}

void mapRectToAE_2_ARRAY(UINT32 x0, UINT32 y0, UINT32 x1, UINT32 y1)
{
    UINT32 i, j;
    SENSORDB("([%d][%d]),([%d][%d])\n", x0,y0,x1,y1);
    clearAE_2_ARRAY();
    x0=clampSection(x0,0,AE_VERTICAL_BLOCKS-1);
    y0=clampSection(y0,0,AE_HORIZONTAL_BLOCKS-1);
    x1=clampSection(x1,0,AE_VERTICAL_BLOCKS-1);
    y1=clampSection(y1,0,AE_HORIZONTAL_BLOCKS-1);

    for(j=y0; j<=y1; j++)
    {
        for(i=x0; i<=x1; i++)
        {
            AE_2_ARRAY[j][i]=TRUE;
        }
    }
}

void resetPVAE_2_ARRAY(void)
{
    mapRectToAE_2_ARRAY(1,1,2,2);
}

//update ae window
//@input zone[] addr
//static void HM5065_FOCUS_Single_Focus();
static void HM5065_FOCUS_Set_AF_Window_to_IC(void);
static void HM5065_FOCUS_Single_Focus();

void HM5065_FOCUS_AD5820_Set_AE_Window(UINT32 zone_addr)
{//update global zone
    
    //input:
    UINT32 FD_XS;
    UINT32 FD_YS;	
    UINT32 x0, y0, x1, y1;
    UINT32 pvx0, pvy0, pvx1, pvy1;
    UINT32 linenum, rownum;
    UINT32 rightbottomlinenum,rightbottomrownum;
    UINT32 leftuplinenum,leftuprownum;
    UINT32* zone = (UINT32*)zone_addr;
    x0 = *zone;
    y0 = *(zone + 1);
    x1 = *(zone + 2);
    y1 = *(zone + 3);	
    FD_XS = *(zone + 4);
    FD_YS = *(zone + 5);

    SENSORDB("AE x0=%d,y0=%d,x1=%d,y1=%d,FD_XS=%d,FD_YS=%d\n",
    x0, y0, x1, y1, FD_XS, FD_YS);	
	XSTART=x0*4;
	YSTART=y0*4;
	
	 HM5065_FOCUS_Set_AF_Window_to_IC();
   // HM5065_FOCUS_Single_Focus();
		
	/*
    //print_sensor_ae_section();
    //print_AE_section();	

    //1.transfer points to preview size
    //UINT32 pvx0, pvy0, pvx1, pvy1;
    mapMiddlewaresizePointToPreviewsizePoint(x0,y0,FD_XS,FD_YS,&pvx0, &pvy0, PRV_W, PRV_H);
    mapMiddlewaresizePointToPreviewsizePoint(x1,y1,FD_XS,FD_YS,&pvx1, &pvy1, PRV_W, PRV_H);
    
    //2.sensor AE line and row coordinate
    calcLine();
    calcRow();

    //3.calc left up point to section
    //UINT32 linenum, rownum;
    calcPointsAELineRowCoordinate(pvx0,pvy0,&linenum,&rownum);    
    //UINT32 leftuplinenum,leftuprownum;
    mapCoordinate(linenum, rownum, &leftuplinenum, &leftuprownum);
    //SENSORDB("leftuplinenum=%d,leftuprownum=%d\n",leftuplinenum,leftuprownum);

    //4.calc right bottom point to section
    calcPointsAELineRowCoordinate(pvx1,pvy1,&linenum,&rownum);    
    //UINT32 rightbottomlinenum,rightbottomrownum;
    mapCoordinate(linenum, rownum, &rightbottomlinenum, &rightbottomrownum);
    //SENSORDB("rightbottomlinenum=%d,rightbottomrownum=%d\n",rightbottomlinenum,rightbottomrownum);

    //5.update global section array
    mapRectToAE_2_ARRAY(leftuplinenum, leftuprownum, rightbottomlinenum, rightbottomrownum);
    //print_AE_section();

    //6.write to reg
    mapAE_2_ARRAY_To_AE_1_ARRAY();
    //printAE_1_ARRAY();
    printAE_2_ARRAY();
    writeAEReg();
*/
}
//=====================touch AE end==========================//

static void HM5065_FOCUS_Befor_Cap(void) 
{
kal_uint32 focused_cnt = 0;
	
	if(af_type!=AF_INF)
	{
	    HM5065_FOCUS_Single_Focus();
	do
		{
	if (0 ==  HM5065_read_cmos_sensor(0x07AE))
		{
		focused_cnt++;		
		mdelay(30);
		}
	else if (1 ==  HM5065_read_cmos_sensor(0x07AE))
		{
		af_pos_h = HM5065_read_cmos_sensor(0x06F0);
		af_pos_l = HM5065_read_cmos_sensor(0x06F1);
		break;
		}
		}while(focused_cnt<=9);
//HM5065_FOCUS_Get_AF_Status(ppp);
	}
	

}


/*************************************************************************
* FUNCTION
*	HM5065_NightMode
*
* DESCRIPTION
*	This function night mode of HM5065.
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
static void HM5065_night_mode(kal_bool bEnable)
{
 //kal_uint8 temp = HM5065_read_cmos_sensor(0x3B);


  if (!HM5065_Sensor_Driver.MODE_CAPTURE) { 
	if(bEnable)
	{ 
		   HM5065_Sensor_Driver.bNight_mode = KAL_TRUE;
	   if(HM5065_Sensor_Driver.MPEG4_encode_mode == KAL_TRUE)
		{		

				/* MJPEG or MPEG4 Apps */
		    HM5065_write_cmos_sensor(0x00E8, 0x00);//Static Framerate
				HM5065_write_cmos_sensor(0x00C8, 0x00);
				HM5065_write_cmos_sensor(0x00C9, 0x0F);//15fps
				HM5065_write_cmos_sensor(0x00CA, 0x01);

				HM5065_write_cmos_sensor(0x015E, 0x42);//Max Dgain 4100=3x ,4000=2x, 3E00=1x 42:4x
				HM5065_write_cmos_sensor(0x015F, 0x00);
				HM5065_write_cmos_sensor(0x0082, 0x7A);
	    }
	    else
	    {
				/* DSC Apps */
		    HM5065_write_cmos_sensor(0x00E8, 0x00);//Static Framerate
				HM5065_write_cmos_sensor(0x00C8, 0x00);
				HM5065_write_cmos_sensor(0x00C9, 0x05);//5fps
				HM5065_write_cmos_sensor(0x00CA, 0x01);

				HM5065_write_cmos_sensor(0x015E, 0x42);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
				HM5065_write_cmos_sensor(0x015F, 0x00);
				HM5065_write_cmos_sensor(0x0082, 0x7A);
		}
	    
	}
	else
	{
		HM5065_Sensor_Driver.bNight_mode = KAL_FALSE;
	    if(HM5065_Sensor_Driver.MPEG4_encode_mode == KAL_TRUE)
	    {
	   
				 /* MJPEG or MPEG4 Apps */
				HM5065_write_cmos_sensor(0x00E8, 0x00);//Static Framerate
				HM5065_write_cmos_sensor(0x00C8, 0x00);
				HM5065_write_cmos_sensor(0x00C9, 0x1E);//30fps
				HM5065_write_cmos_sensor(0x00CA, 0x01);

				HM5065_write_cmos_sensor(0x015E, 0x40);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
				HM5065_write_cmos_sensor(0x015F, 0x00);
				HM5065_write_cmos_sensor(0x0082, 0x5A);
		}

	 	else
	    {
				/* DSC Apps */
		        HM5065_write_cmos_sensor(0x00E8, 0x01);//Static Framerate 20130115 00->01
				HM5065_write_cmos_sensor(0x00C8, 0x00);
				HM5065_write_cmos_sensor(0x00C9, 0x0F);//15fps
				HM5065_write_cmos_sensor(0x00CA, 0x01);

				HM5065_write_cmos_sensor(0x015E, 0x40);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
				HM5065_write_cmos_sensor(0x015F, 0x00);
				HM5065_write_cmos_sensor(0x0082, 0x5A);
		}
		
	   
	}  
	}
}	/*	HM5065_NightMode	*/

/*
static void HM5065_set_isp_driving_current(kal_uint8 current)
{
    //#define CONFIG_BASE      	(0xF0001000)     
//  iowrite32((0xE << 12)|(0 << 28)|0x8880888, 0xF0001500);
}
*/
//EMAIL TO FAE
static void HM5065_Sensor_Driver_Init(void)
	{
	HM5065_write_cmos_sensor(0xffff,0x01);
	HM5065_write_cmos_sensor(0x9000,0x03);
	HM5065_write_cmos_sensor(0xA000,0x90);
	HM5065_write_cmos_sensor(0xA001,0x0C);
	HM5065_write_cmos_sensor(0xA002,0x56);
	HM5065_write_cmos_sensor(0xA003,0xE0);
	HM5065_write_cmos_sensor(0xA004,0xFE);
	HM5065_write_cmos_sensor(0xA005,0xA3);
	HM5065_write_cmos_sensor(0xA006,0xE0);
	HM5065_write_cmos_sensor(0xA007,0xFF);
	HM5065_write_cmos_sensor(0xA008,0x12);
	HM5065_write_cmos_sensor(0xA009,0x42);
	HM5065_write_cmos_sensor(0xA00A,0x85);
	HM5065_write_cmos_sensor(0xA00B,0x90);
	HM5065_write_cmos_sensor(0xA00C,0x01);
	HM5065_write_cmos_sensor(0xA00D,0xB7);
	HM5065_write_cmos_sensor(0xA00E,0xEE);
	HM5065_write_cmos_sensor(0xA00F,0xF0);
	HM5065_write_cmos_sensor(0xA010,0xFC);
	HM5065_write_cmos_sensor(0xA011,0xA3);
	HM5065_write_cmos_sensor(0xA012,0xEF);
	HM5065_write_cmos_sensor(0xA013,0xF0);
	HM5065_write_cmos_sensor(0xA014,0xFD);
	HM5065_write_cmos_sensor(0xA015,0x90);
	HM5065_write_cmos_sensor(0xA016,0x06);
	HM5065_write_cmos_sensor(0xA017,0x05);
	HM5065_write_cmos_sensor(0xA018,0xE0);
	HM5065_write_cmos_sensor(0xA019,0x75);
	HM5065_write_cmos_sensor(0xA01A,0xF0);
	HM5065_write_cmos_sensor(0xA01B,0x02);
	HM5065_write_cmos_sensor(0xA01C,0xA4);
	HM5065_write_cmos_sensor(0xA01D,0x2D);
	HM5065_write_cmos_sensor(0xA01E,0xFF);
	HM5065_write_cmos_sensor(0xA01F,0xE5);
	HM5065_write_cmos_sensor(0xA020,0xF0);
	HM5065_write_cmos_sensor(0xA021,0x3C);
	HM5065_write_cmos_sensor(0xA022,0xFE);
	HM5065_write_cmos_sensor(0xA023,0xAB);
	HM5065_write_cmos_sensor(0xA024,0x07);
	HM5065_write_cmos_sensor(0xA025,0xFA);
	HM5065_write_cmos_sensor(0xA026,0x33);
	HM5065_write_cmos_sensor(0xA027,0x95);
	HM5065_write_cmos_sensor(0xA028,0xE0);
	HM5065_write_cmos_sensor(0xA029,0xF9);
	HM5065_write_cmos_sensor(0xA02A,0xF8);
	HM5065_write_cmos_sensor(0xA02B,0x90);
	HM5065_write_cmos_sensor(0xA02C,0x0B);
	HM5065_write_cmos_sensor(0xA02D,0x4B);
	HM5065_write_cmos_sensor(0xA02E,0xE0);
	HM5065_write_cmos_sensor(0xA02F,0xFE);
	HM5065_write_cmos_sensor(0xA030,0xA3);
	HM5065_write_cmos_sensor(0xA031,0xE0);
	HM5065_write_cmos_sensor(0xA032,0xFF);
	HM5065_write_cmos_sensor(0xA033,0xEE);
	HM5065_write_cmos_sensor(0xA034,0x33);
	HM5065_write_cmos_sensor(0xA035,0x95);
	HM5065_write_cmos_sensor(0xA036,0xE0);
	HM5065_write_cmos_sensor(0xA037,0xFD);
	HM5065_write_cmos_sensor(0xA038,0xFC);
	HM5065_write_cmos_sensor(0xA039,0x12);
	HM5065_write_cmos_sensor(0xA03A,0x0C);
	HM5065_write_cmos_sensor(0xA03B,0x7B);
	HM5065_write_cmos_sensor(0xA03C,0x90);
	HM5065_write_cmos_sensor(0xA03D,0x01);
	HM5065_write_cmos_sensor(0xA03E,0xB9);
	HM5065_write_cmos_sensor(0xA03F,0x12);
	HM5065_write_cmos_sensor(0xA040,0x0E);
	HM5065_write_cmos_sensor(0xA041,0x05);
	HM5065_write_cmos_sensor(0xA042,0x90);
	HM5065_write_cmos_sensor(0xA043,0x01);
	HM5065_write_cmos_sensor(0xA044,0xB9);
	HM5065_write_cmos_sensor(0xA045,0xE0);
	HM5065_write_cmos_sensor(0xA046,0xFC);
	HM5065_write_cmos_sensor(0xA047,0xA3);
	HM5065_write_cmos_sensor(0xA048,0xE0);
	HM5065_write_cmos_sensor(0xA049,0xFD);
	HM5065_write_cmos_sensor(0xA04A,0xA3);
	HM5065_write_cmos_sensor(0xA04B,0xE0);
	HM5065_write_cmos_sensor(0xA04C,0xFE);
	HM5065_write_cmos_sensor(0xA04D,0xA3);
	HM5065_write_cmos_sensor(0xA04E,0xE0);
	HM5065_write_cmos_sensor(0xA04F,0xFF);
	HM5065_write_cmos_sensor(0xA050,0x78);
	HM5065_write_cmos_sensor(0xA051,0x08);
	HM5065_write_cmos_sensor(0xA052,0x12);
	HM5065_write_cmos_sensor(0xA053,0x0D);
	HM5065_write_cmos_sensor(0xA054,0xBF);
	HM5065_write_cmos_sensor(0xA055,0xA8);
	HM5065_write_cmos_sensor(0xA056,0x04);
	HM5065_write_cmos_sensor(0xA057,0xA9);
	HM5065_write_cmos_sensor(0xA058,0x05);
	HM5065_write_cmos_sensor(0xA059,0xAA);
	HM5065_write_cmos_sensor(0xA05A,0x06);
	HM5065_write_cmos_sensor(0xA05B,0xAB);
	HM5065_write_cmos_sensor(0xA05C,0x07);
	HM5065_write_cmos_sensor(0xA05D,0x90);
	HM5065_write_cmos_sensor(0xA05E,0x0B);
	HM5065_write_cmos_sensor(0xA05F,0x49);
	HM5065_write_cmos_sensor(0xA060,0xE0);
	HM5065_write_cmos_sensor(0xA061,0xFE);
	HM5065_write_cmos_sensor(0xA062,0xA3);
	HM5065_write_cmos_sensor(0xA063,0xE0);
	HM5065_write_cmos_sensor(0xA064,0xFF);
	HM5065_write_cmos_sensor(0xA065,0xEE);
	HM5065_write_cmos_sensor(0xA066,0x33);
	HM5065_write_cmos_sensor(0xA067,0x95);
	HM5065_write_cmos_sensor(0xA068,0xE0);
	HM5065_write_cmos_sensor(0xA069,0xFD);
	HM5065_write_cmos_sensor(0xA06A,0xFC);
	HM5065_write_cmos_sensor(0xA06B,0xC3);
	HM5065_write_cmos_sensor(0xA06C,0xEF);
	HM5065_write_cmos_sensor(0xA06D,0x9B);
	HM5065_write_cmos_sensor(0xA06E,0xFF);
	HM5065_write_cmos_sensor(0xA06F,0xEE);
	HM5065_write_cmos_sensor(0xA070,0x9A);
	HM5065_write_cmos_sensor(0xA071,0xFE);
	HM5065_write_cmos_sensor(0xA072,0xED);
	HM5065_write_cmos_sensor(0xA073,0x99);
	HM5065_write_cmos_sensor(0xA074,0xFD);
	HM5065_write_cmos_sensor(0xA075,0xEC);
	HM5065_write_cmos_sensor(0xA076,0x98);
	HM5065_write_cmos_sensor(0xA077,0xFC);
	HM5065_write_cmos_sensor(0xA078,0x78);
	HM5065_write_cmos_sensor(0xA079,0x01);
	HM5065_write_cmos_sensor(0xA07A,0x12);
	HM5065_write_cmos_sensor(0xA07B,0x0D);
	HM5065_write_cmos_sensor(0xA07C,0xBF);
	HM5065_write_cmos_sensor(0xA07D,0x90);
	HM5065_write_cmos_sensor(0xA07E,0x0C);
	HM5065_write_cmos_sensor(0xA07F,0x4A);
	HM5065_write_cmos_sensor(0xA080,0xE0);
	HM5065_write_cmos_sensor(0xA081,0xFC);
	HM5065_write_cmos_sensor(0xA082,0xA3);
	HM5065_write_cmos_sensor(0xA083,0xE0);
	HM5065_write_cmos_sensor(0xA084,0xF5);
	HM5065_write_cmos_sensor(0xA085,0x82);
	HM5065_write_cmos_sensor(0xA086,0x8C);
	HM5065_write_cmos_sensor(0xA087,0x83);
	HM5065_write_cmos_sensor(0xA088,0xC0);
	HM5065_write_cmos_sensor(0xA089,0x83);
	HM5065_write_cmos_sensor(0xA08A,0xC0);
	HM5065_write_cmos_sensor(0xA08B,0x82);
	HM5065_write_cmos_sensor(0xA08C,0x90);
	HM5065_write_cmos_sensor(0xA08D,0x0B);
	HM5065_write_cmos_sensor(0xA08E,0x48);
	HM5065_write_cmos_sensor(0xA08F,0xE0);
	HM5065_write_cmos_sensor(0xA090,0xD0);
	HM5065_write_cmos_sensor(0xA091,0x82);
	HM5065_write_cmos_sensor(0xA092,0xD0);
	HM5065_write_cmos_sensor(0xA093,0x83);
	HM5065_write_cmos_sensor(0xA094,0x75);
	HM5065_write_cmos_sensor(0xA095,0xF0);
	HM5065_write_cmos_sensor(0xA096,0x02);
	HM5065_write_cmos_sensor(0xA097,0x12);
	HM5065_write_cmos_sensor(0xA098,0x0E);
	HM5065_write_cmos_sensor(0xA099,0x45);
	HM5065_write_cmos_sensor(0xA09A,0xEE);
	HM5065_write_cmos_sensor(0xA09B,0xF0);
	HM5065_write_cmos_sensor(0xA09C,0xA3);
	HM5065_write_cmos_sensor(0xA09D,0xEF);
	HM5065_write_cmos_sensor(0xA09E,0xF0);
	HM5065_write_cmos_sensor(0xA09F,0x02);
	HM5065_write_cmos_sensor(0xA0A0,0xBA);
	HM5065_write_cmos_sensor(0xA0A1,0xD8);
	HM5065_write_cmos_sensor(0xA0A2,0x90);
	HM5065_write_cmos_sensor(0xA0A3,0x30);
	HM5065_write_cmos_sensor(0xA0A4,0x18);
	HM5065_write_cmos_sensor(0xA0A5,0xe4);
	HM5065_write_cmos_sensor(0xA0A6,0xf0);
	HM5065_write_cmos_sensor(0xA0A7,0x74);
	HM5065_write_cmos_sensor(0xA0A8,0x3f);
	HM5065_write_cmos_sensor(0xA0A9,0xf0);
	HM5065_write_cmos_sensor(0xA0AA,0x22);
	HM5065_write_cmos_sensor(0xA0BF,0x90);
	HM5065_write_cmos_sensor(0xA0C0,0x00);
	HM5065_write_cmos_sensor(0xA0C1,0x5E);
	HM5065_write_cmos_sensor(0xA0C2,0xE0);
	HM5065_write_cmos_sensor(0xA0C3,0xFF);
	HM5065_write_cmos_sensor(0xA0C4,0x70);
	HM5065_write_cmos_sensor(0xA0C5,0x20);
	HM5065_write_cmos_sensor(0xA0C6,0x90);
	HM5065_write_cmos_sensor(0xA0C7,0x47);
	HM5065_write_cmos_sensor(0xA0C8,0x04);
	HM5065_write_cmos_sensor(0xA0C9,0x74);
	HM5065_write_cmos_sensor(0xA0CA,0x0A);
	HM5065_write_cmos_sensor(0xA0CB,0xF0);
	HM5065_write_cmos_sensor(0xA0CC,0xA3);
	HM5065_write_cmos_sensor(0xA0CD,0x74);
	HM5065_write_cmos_sensor(0xA0CE,0x30);
	HM5065_write_cmos_sensor(0xA0CF,0xF0);
	HM5065_write_cmos_sensor(0xA0D0,0x90);
	HM5065_write_cmos_sensor(0xA0D1,0x47);
	HM5065_write_cmos_sensor(0xA0D2,0x0C);
	HM5065_write_cmos_sensor(0xA0D3,0x74);
	HM5065_write_cmos_sensor(0xA0D4,0x07);
	HM5065_write_cmos_sensor(0xA0D5,0xF0);
	HM5065_write_cmos_sensor(0xA0D6,0xA3);
	HM5065_write_cmos_sensor(0xA0D7,0x74);
	HM5065_write_cmos_sensor(0xA0D8,0xA8);
	HM5065_write_cmos_sensor(0xA0D9,0xF0);
	HM5065_write_cmos_sensor(0xA0DA,0x90);
	HM5065_write_cmos_sensor(0xA0DB,0x47);
	HM5065_write_cmos_sensor(0xA0DC,0xA4);
	HM5065_write_cmos_sensor(0xA0DD,0x74);
	HM5065_write_cmos_sensor(0xA0DE,0x01);
	HM5065_write_cmos_sensor(0xA0DF,0xF0);
	HM5065_write_cmos_sensor(0xA0E0,0x90);
	HM5065_write_cmos_sensor(0xA0E1,0x47);
	HM5065_write_cmos_sensor(0xA0E2,0xA8);
	HM5065_write_cmos_sensor(0xA0E3,0xF0);
	HM5065_write_cmos_sensor(0xA0E4,0x80);
	HM5065_write_cmos_sensor(0xA0E5,0x50);
	HM5065_write_cmos_sensor(0xA0E6,0xEF);
	HM5065_write_cmos_sensor(0xA0E7,0x64);
	HM5065_write_cmos_sensor(0xA0E8,0x01);
	HM5065_write_cmos_sensor(0xA0E9,0x60);
	HM5065_write_cmos_sensor(0xA0EA,0x04);
	HM5065_write_cmos_sensor(0xA0EB,0xEF);
	HM5065_write_cmos_sensor(0xA0EC,0xB4);
	HM5065_write_cmos_sensor(0xA0ED,0x03);
	HM5065_write_cmos_sensor(0xA0EE,0x20);
	HM5065_write_cmos_sensor(0xA0EF,0x90);
	HM5065_write_cmos_sensor(0xA0F0,0x47);
	HM5065_write_cmos_sensor(0xA0F1,0x04);
	HM5065_write_cmos_sensor(0xA0F2,0x74);
	HM5065_write_cmos_sensor(0xA0F3,0x05);
	HM5065_write_cmos_sensor(0xA0F4,0xF0);
	HM5065_write_cmos_sensor(0xA0F5,0xA3);
	HM5065_write_cmos_sensor(0xA0F6,0x74);
	HM5065_write_cmos_sensor(0xA0F7,0x18);
	HM5065_write_cmos_sensor(0xA0F8,0xF0);
	HM5065_write_cmos_sensor(0xA0F9,0x90);
	HM5065_write_cmos_sensor(0xA0FA,0x47);
	HM5065_write_cmos_sensor(0xA0FB,0x0C);
	HM5065_write_cmos_sensor(0xA0FC,0x74);
	HM5065_write_cmos_sensor(0xA0FD,0x03);
	HM5065_write_cmos_sensor(0xA0FE,0xF0);
	HM5065_write_cmos_sensor(0xA0FF,0xA3);
	HM5065_write_cmos_sensor(0xA100,0x74);
	HM5065_write_cmos_sensor(0xA101,0xD4);
	HM5065_write_cmos_sensor(0xA102,0xF0);
	HM5065_write_cmos_sensor(0xA103,0x90);
	HM5065_write_cmos_sensor(0xA104,0x47);
	HM5065_write_cmos_sensor(0xA105,0xA4);
	HM5065_write_cmos_sensor(0xA106,0x74);
	HM5065_write_cmos_sensor(0xA107,0x02);
	HM5065_write_cmos_sensor(0xA108,0xF0);
	HM5065_write_cmos_sensor(0xA109,0x90);
	HM5065_write_cmos_sensor(0xA10A,0x47);
	HM5065_write_cmos_sensor(0xA10B,0xA8);
	HM5065_write_cmos_sensor(0xA10C,0xF0);
	HM5065_write_cmos_sensor(0xA10D,0x80);
	HM5065_write_cmos_sensor(0xA10E,0x27);
	HM5065_write_cmos_sensor(0xA10F,0xEF);
	HM5065_write_cmos_sensor(0xA110,0x64);
	HM5065_write_cmos_sensor(0xA111,0x02);
	HM5065_write_cmos_sensor(0xA112,0x60);
	HM5065_write_cmos_sensor(0xA113,0x04);
	HM5065_write_cmos_sensor(0xA114,0xEF);
	HM5065_write_cmos_sensor(0xA115,0xB4);
	HM5065_write_cmos_sensor(0xA116,0x04);
	HM5065_write_cmos_sensor(0xA117,0x1E);
	HM5065_write_cmos_sensor(0xA118,0x90);
	HM5065_write_cmos_sensor(0xA119,0x47);
	HM5065_write_cmos_sensor(0xA11A,0x04);
	HM5065_write_cmos_sensor(0xA11B,0x74);
	HM5065_write_cmos_sensor(0xA11C,0x02);
	HM5065_write_cmos_sensor(0xA11D,0xF0);
	HM5065_write_cmos_sensor(0xA11E,0xA3);
	HM5065_write_cmos_sensor(0xA11F,0x74);
	HM5065_write_cmos_sensor(0xA120,0x8C);
	HM5065_write_cmos_sensor(0xA121,0xF0);
	HM5065_write_cmos_sensor(0xA122,0x90);
	HM5065_write_cmos_sensor(0xA123,0x47);
	HM5065_write_cmos_sensor(0xA124,0x0C);
	HM5065_write_cmos_sensor(0xA125,0x74);
	HM5065_write_cmos_sensor(0xA126,0x01);
	HM5065_write_cmos_sensor(0xA127,0xF0);
	HM5065_write_cmos_sensor(0xA128,0xA3);
	HM5065_write_cmos_sensor(0xA129,0x74);
	HM5065_write_cmos_sensor(0xA12A,0xEA);
	HM5065_write_cmos_sensor(0xA12B,0xF0);
	HM5065_write_cmos_sensor(0xA12C,0x90);
	HM5065_write_cmos_sensor(0xA12D,0x47);
	HM5065_write_cmos_sensor(0xA12E,0xA4);
	HM5065_write_cmos_sensor(0xA12F,0x74);
	HM5065_write_cmos_sensor(0xA130,0x04);
	HM5065_write_cmos_sensor(0xA131,0xF0);
	HM5065_write_cmos_sensor(0xA132,0x90);
	HM5065_write_cmos_sensor(0xA133,0x47);
	HM5065_write_cmos_sensor(0xA134,0xA8);
	HM5065_write_cmos_sensor(0xA135,0xF0);
	HM5065_write_cmos_sensor(0xA136,0x22);
	HM5065_write_cmos_sensor(0xA137,0x74);
	HM5065_write_cmos_sensor(0xA138,0x04);
	HM5065_write_cmos_sensor(0xA139,0xF0);
	HM5065_write_cmos_sensor(0xA13A,0xA3);
	HM5065_write_cmos_sensor(0xA13B,0x74);
	HM5065_write_cmos_sensor(0xA13C,0x20);
	HM5065_write_cmos_sensor(0xA13D,0xF0);
	HM5065_write_cmos_sensor(0xA13E,0xE4);
	HM5065_write_cmos_sensor(0xA13F,0xF5);
	HM5065_write_cmos_sensor(0xA140,0x22);
	HM5065_write_cmos_sensor(0xA141,0xE5);
	HM5065_write_cmos_sensor(0xA142,0x22);
	HM5065_write_cmos_sensor(0xA143,0xC3);
	HM5065_write_cmos_sensor(0xA144,0x94);
	HM5065_write_cmos_sensor(0xA145,0x40);
	HM5065_write_cmos_sensor(0xA146,0x40);
	HM5065_write_cmos_sensor(0xA147,0x03);
	HM5065_write_cmos_sensor(0xA148,0x02);
	HM5065_write_cmos_sensor(0xA149,0xF1);
	HM5065_write_cmos_sensor(0xA14A,0xFD);
	HM5065_write_cmos_sensor(0xA14B,0x90);
	HM5065_write_cmos_sensor(0xA14C,0x0A);
	HM5065_write_cmos_sensor(0xA14D,0xBA);
	HM5065_write_cmos_sensor(0xA14E,0xE0);
	HM5065_write_cmos_sensor(0xA14F,0xFE);
	HM5065_write_cmos_sensor(0xA150,0xA3);
	HM5065_write_cmos_sensor(0xA151,0xE0);
	HM5065_write_cmos_sensor(0xA152,0xFF);
	HM5065_write_cmos_sensor(0xA153,0xF5);
	HM5065_write_cmos_sensor(0xA154,0x82);
	HM5065_write_cmos_sensor(0xA155,0x8E);
	HM5065_write_cmos_sensor(0xA156,0x83);
	HM5065_write_cmos_sensor(0xA157,0xE0);
	HM5065_write_cmos_sensor(0xA158,0x54);
	HM5065_write_cmos_sensor(0xA159,0x70);
	HM5065_write_cmos_sensor(0xA15A,0xFD);
	HM5065_write_cmos_sensor(0xA15B,0xC4);
	HM5065_write_cmos_sensor(0xA15C,0x54);
	HM5065_write_cmos_sensor(0xA15D,0x0F);
	HM5065_write_cmos_sensor(0xA15E,0xFD);
	HM5065_write_cmos_sensor(0xA15F,0x90);
	HM5065_write_cmos_sensor(0xA160,0x0A);
	HM5065_write_cmos_sensor(0xA161,0xBC);
	HM5065_write_cmos_sensor(0xA162,0xE0);
	HM5065_write_cmos_sensor(0xA163,0xFA);
	HM5065_write_cmos_sensor(0xA164,0xA3);
	HM5065_write_cmos_sensor(0xA165,0xE0);
	HM5065_write_cmos_sensor(0xA166,0xF5);
	HM5065_write_cmos_sensor(0xA167,0x82);
	HM5065_write_cmos_sensor(0xA168,0x8A);
	HM5065_write_cmos_sensor(0xA169,0x83);
	HM5065_write_cmos_sensor(0xA16A,0xED);
	HM5065_write_cmos_sensor(0xA16B,0xF0);
	HM5065_write_cmos_sensor(0xA16C,0x90);
	HM5065_write_cmos_sensor(0xA16D,0x0A);
	HM5065_write_cmos_sensor(0xA16E,0xBD);
	HM5065_write_cmos_sensor(0xA16F,0xE0);
	HM5065_write_cmos_sensor(0xA170,0x04);
	HM5065_write_cmos_sensor(0xA171,0xF0);
	HM5065_write_cmos_sensor(0xA172,0x70);
	HM5065_write_cmos_sensor(0xA173,0x06);
	HM5065_write_cmos_sensor(0xA174,0x90);
	HM5065_write_cmos_sensor(0xA175,0x0A);
	HM5065_write_cmos_sensor(0xA176,0xBC);
	HM5065_write_cmos_sensor(0xA177,0xE0);
	HM5065_write_cmos_sensor(0xA178,0x04);
	HM5065_write_cmos_sensor(0xA179,0xF0);
	HM5065_write_cmos_sensor(0xA17A,0x8F);
	HM5065_write_cmos_sensor(0xA17B,0x82);
	HM5065_write_cmos_sensor(0xA17C,0x8E);
	HM5065_write_cmos_sensor(0xA17D,0x83);
	HM5065_write_cmos_sensor(0xA17E,0xA3);
	HM5065_write_cmos_sensor(0xA17F,0xE0);
	HM5065_write_cmos_sensor(0xA180,0xFF);
	HM5065_write_cmos_sensor(0xA181,0x90);
	HM5065_write_cmos_sensor(0xA182,0x0A);
	HM5065_write_cmos_sensor(0xA183,0xBC);
	HM5065_write_cmos_sensor(0xA184,0xE0);
	HM5065_write_cmos_sensor(0xA185,0xFC);
	HM5065_write_cmos_sensor(0xA186,0xA3);
	HM5065_write_cmos_sensor(0xA187,0xE0);
	HM5065_write_cmos_sensor(0xA188,0xF5);
	HM5065_write_cmos_sensor(0xA189,0x82);
	HM5065_write_cmos_sensor(0xA18A,0x8C);
	HM5065_write_cmos_sensor(0xA18B,0x83);
	HM5065_write_cmos_sensor(0xA18C,0xEF);
	HM5065_write_cmos_sensor(0xA18D,0xF0);
	HM5065_write_cmos_sensor(0xA18E,0x90);
	HM5065_write_cmos_sensor(0xA18F,0x0A);
	HM5065_write_cmos_sensor(0xA190,0xBD);
	HM5065_write_cmos_sensor(0xA191,0xE0);
	HM5065_write_cmos_sensor(0xA192,0x04);
	HM5065_write_cmos_sensor(0xA193,0xF0);
	HM5065_write_cmos_sensor(0xA194,0x70);
	HM5065_write_cmos_sensor(0xA195,0x06);
	HM5065_write_cmos_sensor(0xA196,0x90);
	HM5065_write_cmos_sensor(0xA197,0x0A);
	HM5065_write_cmos_sensor(0xA198,0xBC);
	HM5065_write_cmos_sensor(0xA199,0xE0);
	HM5065_write_cmos_sensor(0xA19A,0x04);
	HM5065_write_cmos_sensor(0xA19B,0xF0);
	HM5065_write_cmos_sensor(0xA19C,0x90);
	HM5065_write_cmos_sensor(0xA19D,0x0A);
	HM5065_write_cmos_sensor(0xA19E,0xBA);
	HM5065_write_cmos_sensor(0xA19F,0xE0);
	HM5065_write_cmos_sensor(0xA1A0,0xFE);
	HM5065_write_cmos_sensor(0xA1A1,0xA3);
	HM5065_write_cmos_sensor(0xA1A2,0xE0);
	HM5065_write_cmos_sensor(0xA1A3,0xFF);
	HM5065_write_cmos_sensor(0xA1A4,0xF5);
	HM5065_write_cmos_sensor(0xA1A5,0x82);
	HM5065_write_cmos_sensor(0xA1A6,0x8E);
	HM5065_write_cmos_sensor(0xA1A7,0x83);
	HM5065_write_cmos_sensor(0xA1A8,0xE0);
	HM5065_write_cmos_sensor(0xA1A9,0x54);
	HM5065_write_cmos_sensor(0xA1AA,0x07);
	HM5065_write_cmos_sensor(0xA1AB,0xFD);
	HM5065_write_cmos_sensor(0xA1AC,0x90);
	HM5065_write_cmos_sensor(0xA1AD,0x0A);
	HM5065_write_cmos_sensor(0xA1AE,0xBC);
	HM5065_write_cmos_sensor(0xA1AF,0xE0);
	HM5065_write_cmos_sensor(0xA1B0,0xFA);
	HM5065_write_cmos_sensor(0xA1B1,0xA3);
	HM5065_write_cmos_sensor(0xA1B2,0xE0);
	HM5065_write_cmos_sensor(0xA1B3,0xF5);
	HM5065_write_cmos_sensor(0xA1B4,0x82);
	HM5065_write_cmos_sensor(0xA1B5,0x8A);
	HM5065_write_cmos_sensor(0xA1B6,0x83);
	HM5065_write_cmos_sensor(0xA1B7,0xED);
	HM5065_write_cmos_sensor(0xA1B8,0xF0);
	HM5065_write_cmos_sensor(0xA1B9,0x90);
	HM5065_write_cmos_sensor(0xA1BA,0x0A);
	HM5065_write_cmos_sensor(0xA1BB,0xBD);
	HM5065_write_cmos_sensor(0xA1BC,0xE0);
	HM5065_write_cmos_sensor(0xA1BD,0x04);
	HM5065_write_cmos_sensor(0xA1BE,0xF0);
	HM5065_write_cmos_sensor(0xA1BF,0x70);
	HM5065_write_cmos_sensor(0xA1C0,0x06);
	HM5065_write_cmos_sensor(0xA1C1,0x90);
	HM5065_write_cmos_sensor(0xA1C2,0x0A);
	HM5065_write_cmos_sensor(0xA1C3,0xBC);
	HM5065_write_cmos_sensor(0xA1C4,0xE0);
	HM5065_write_cmos_sensor(0xA1C5,0x04);
	HM5065_write_cmos_sensor(0xA1C6,0xF0);
	HM5065_write_cmos_sensor(0xA1C7,0x8F);
	HM5065_write_cmos_sensor(0xA1C8,0x82);
	HM5065_write_cmos_sensor(0xA1C9,0x8E);
	HM5065_write_cmos_sensor(0xA1CA,0x83);
	HM5065_write_cmos_sensor(0xA1CB,0xA3);
	HM5065_write_cmos_sensor(0xA1CC,0xA3);
	HM5065_write_cmos_sensor(0xA1CD,0xE0);
	HM5065_write_cmos_sensor(0xA1CE,0xFF);
	HM5065_write_cmos_sensor(0xA1CF,0x90);
	HM5065_write_cmos_sensor(0xA1D0,0x0A);
	HM5065_write_cmos_sensor(0xA1D1,0xBC);
	HM5065_write_cmos_sensor(0xA1D2,0xE0);
	HM5065_write_cmos_sensor(0xA1D3,0xFC);
	HM5065_write_cmos_sensor(0xA1D4,0xA3);
	HM5065_write_cmos_sensor(0xA1D5,0xE0);
	HM5065_write_cmos_sensor(0xA1D6,0xF5);
	HM5065_write_cmos_sensor(0xA1D7,0x82);
	HM5065_write_cmos_sensor(0xA1D8,0x8C);
	HM5065_write_cmos_sensor(0xA1D9,0x83);
	HM5065_write_cmos_sensor(0xA1DA,0xEF);
	HM5065_write_cmos_sensor(0xA1DB,0xF0);
	HM5065_write_cmos_sensor(0xA1DC,0x90);
	HM5065_write_cmos_sensor(0xA1DD,0x0A);
	HM5065_write_cmos_sensor(0xA1DE,0xBD);
	HM5065_write_cmos_sensor(0xA1DF,0xE0);
	HM5065_write_cmos_sensor(0xA1E0,0x04);
	HM5065_write_cmos_sensor(0xA1E1,0xF0);
	HM5065_write_cmos_sensor(0xA1E2,0x70);
	HM5065_write_cmos_sensor(0xA1E3,0x06);
	HM5065_write_cmos_sensor(0xA1E4,0x90);
	HM5065_write_cmos_sensor(0xA1E5,0x0A);
	HM5065_write_cmos_sensor(0xA1E6,0xBC);
	HM5065_write_cmos_sensor(0xA1E7,0xE0);
	HM5065_write_cmos_sensor(0xA1E8,0x04);
	HM5065_write_cmos_sensor(0xA1E9,0xF0);
	HM5065_write_cmos_sensor(0xA1EA,0x90);
	HM5065_write_cmos_sensor(0xA1EB,0x0A);
	HM5065_write_cmos_sensor(0xA1EC,0xBB);
	HM5065_write_cmos_sensor(0xA1ED,0xE0);
	HM5065_write_cmos_sensor(0xA1EE,0x24);
	HM5065_write_cmos_sensor(0xA1EF,0x03);
	HM5065_write_cmos_sensor(0xA1F0,0xF0);
	HM5065_write_cmos_sensor(0xA1F1,0x90);
	HM5065_write_cmos_sensor(0xA1F2,0x0A);
	HM5065_write_cmos_sensor(0xA1F3,0xBA);
	HM5065_write_cmos_sensor(0xA1F4,0xE0);
	HM5065_write_cmos_sensor(0xA1F5,0x34);
	HM5065_write_cmos_sensor(0xA1F6,0x00);
	HM5065_write_cmos_sensor(0xA1F7,0xF0);
	HM5065_write_cmos_sensor(0xA1F8,0x05);
	HM5065_write_cmos_sensor(0xA1F9,0x22);
	HM5065_write_cmos_sensor(0xA1FA,0x02);
	HM5065_write_cmos_sensor(0xA1FB,0xF1);
	HM5065_write_cmos_sensor(0xA1FC,0x41);
	HM5065_write_cmos_sensor(0xA1FD,0x90);
	HM5065_write_cmos_sensor(0xA1FE,0x0A);
	HM5065_write_cmos_sensor(0xA1FF,0xBA);
	HM5065_write_cmos_sensor(0xA200,0x74);
	HM5065_write_cmos_sensor(0xA201,0x0E);
	HM5065_write_cmos_sensor(0xA202,0xF0);
	HM5065_write_cmos_sensor(0xA203,0xA3);
	HM5065_write_cmos_sensor(0xA204,0x74);
	HM5065_write_cmos_sensor(0xA205,0xDC);
	HM5065_write_cmos_sensor(0xA206,0xF0);
	HM5065_write_cmos_sensor(0xA207,0xA3);
	HM5065_write_cmos_sensor(0xA208,0x74);
	HM5065_write_cmos_sensor(0xA209,0x05);
	HM5065_write_cmos_sensor(0xA20A,0xF0);
	HM5065_write_cmos_sensor(0xA20B,0xA3);
	HM5065_write_cmos_sensor(0xA20C,0x74);
	HM5065_write_cmos_sensor(0xA20D,0x61);
	HM5065_write_cmos_sensor(0xA20E,0xF0);
	HM5065_write_cmos_sensor(0xA20F,0x90);
	HM5065_write_cmos_sensor(0xA210,0x0A);
	HM5065_write_cmos_sensor(0xA211,0xBA);
	HM5065_write_cmos_sensor(0xA212,0xE0);
	HM5065_write_cmos_sensor(0xA213,0xFE);
	HM5065_write_cmos_sensor(0xA214,0xA3);
	HM5065_write_cmos_sensor(0xA215,0xE0);
	HM5065_write_cmos_sensor(0xA216,0xAA);
	HM5065_write_cmos_sensor(0xA217,0x06);
	HM5065_write_cmos_sensor(0xA218,0xF9);
	HM5065_write_cmos_sensor(0xA219,0x7B);
	HM5065_write_cmos_sensor(0xA21A,0x01);
	HM5065_write_cmos_sensor(0xA21B,0xC0);
	HM5065_write_cmos_sensor(0xA21C,0x02);
	HM5065_write_cmos_sensor(0xA21D,0xA3);
	HM5065_write_cmos_sensor(0xA21E,0xE0);
	HM5065_write_cmos_sensor(0xA21F,0xFE);
	HM5065_write_cmos_sensor(0xA220,0xA3);
	HM5065_write_cmos_sensor(0xA221,0xE0);
	HM5065_write_cmos_sensor(0xA222,0xAA);
	HM5065_write_cmos_sensor(0xA223,0x06);
	HM5065_write_cmos_sensor(0xA224,0xF8);
	HM5065_write_cmos_sensor(0xA225,0xAC);
	HM5065_write_cmos_sensor(0xA226,0x02);
	HM5065_write_cmos_sensor(0xA227,0x7D);
	HM5065_write_cmos_sensor(0xA228,0x01);
	HM5065_write_cmos_sensor(0xA229,0xD0);
	HM5065_write_cmos_sensor(0xA22A,0x02);
	HM5065_write_cmos_sensor(0xA22B,0x7E);
	HM5065_write_cmos_sensor(0xA22C,0x00);
	HM5065_write_cmos_sensor(0xA22D,0x7F);
	HM5065_write_cmos_sensor(0xA22E,0x04);
	HM5065_write_cmos_sensor(0xA22F,0x12);
	HM5065_write_cmos_sensor(0xA230,0x0F);
	HM5065_write_cmos_sensor(0xA231,0x6F);
	HM5065_write_cmos_sensor(0xA232,0x02);
	HM5065_write_cmos_sensor(0xA233,0x66);
	HM5065_write_cmos_sensor(0xA234,0xD9);
	HM5065_write_cmos_sensor(0xA235,0x90);
	HM5065_write_cmos_sensor(0xA236,0x07);
	HM5065_write_cmos_sensor(0xA237,0xD0);
	HM5065_write_cmos_sensor(0xA238,0x02);
	HM5065_write_cmos_sensor(0xA239,0xA2);
	HM5065_write_cmos_sensor(0xA23A,0x69);
	HM5065_write_cmos_sensor(0xA240,0x02);
	HM5065_write_cmos_sensor(0xA241,0x21);
	HM5065_write_cmos_sensor(0xA242,0x7F);
	HM5065_write_cmos_sensor(0xA243,0x02);
	HM5065_write_cmos_sensor(0xA244,0x21);
	HM5065_write_cmos_sensor(0xA245,0xF4);
	HM5065_write_cmos_sensor(0xA246,0x02);
	HM5065_write_cmos_sensor(0xA247,0xA6);
	HM5065_write_cmos_sensor(0xA248,0x15);
	HM5065_write_cmos_sensor(0xA249,0x60);
	HM5065_write_cmos_sensor(0xA24A,0x0A);
	HM5065_write_cmos_sensor(0xA24B,0xEF);
	HM5065_write_cmos_sensor(0xA24C,0xB4);
	HM5065_write_cmos_sensor(0xA24D,0x01);
	HM5065_write_cmos_sensor(0xA24E,0x16);
	HM5065_write_cmos_sensor(0xA24F,0x90);
	HM5065_write_cmos_sensor(0xA250,0x00);
	HM5065_write_cmos_sensor(0xA251,0x5D);
	HM5065_write_cmos_sensor(0xA252,0xE0);
	HM5065_write_cmos_sensor(0xA253,0x70);
	HM5065_write_cmos_sensor(0xA254,0x10);
	HM5065_write_cmos_sensor(0xA255,0x12);
	HM5065_write_cmos_sensor(0xA256,0x26);
	HM5065_write_cmos_sensor(0xA257,0xC8);
	HM5065_write_cmos_sensor(0xA258,0x90);
	HM5065_write_cmos_sensor(0xA259,0x00);
	HM5065_write_cmos_sensor(0xA25A,0x11);
	HM5065_write_cmos_sensor(0xA25B,0x74);
	HM5065_write_cmos_sensor(0xA25C,0x30);
	HM5065_write_cmos_sensor(0xA25D,0xF0);
	HM5065_write_cmos_sensor(0xA25E,0x90);
	HM5065_write_cmos_sensor(0xA25F,0x00);
	HM5065_write_cmos_sensor(0xA260,0x10);
	HM5065_write_cmos_sensor(0xA261,0x74);
	HM5065_write_cmos_sensor(0xA262,0x01);
	HM5065_write_cmos_sensor(0xA263,0xF0);
	HM5065_write_cmos_sensor(0xA264,0x22);
	HM5065_write_cmos_sensor(0xA265,0x12);
	HM5065_write_cmos_sensor(0xA266,0x25);
	HM5065_write_cmos_sensor(0xA267,0xA8);
	HM5065_write_cmos_sensor(0xA268,0x02);
	HM5065_write_cmos_sensor(0xA269,0x29);
	HM5065_write_cmos_sensor(0xA26A,0xFC);
	HM5065_write_cmos_sensor(0xA26B,0x44);
	HM5065_write_cmos_sensor(0xA26C,0x18);
	HM5065_write_cmos_sensor(0xA26D,0xF0);
	HM5065_write_cmos_sensor(0xA26E,0x90);
	HM5065_write_cmos_sensor(0xA26F,0x72);
	HM5065_write_cmos_sensor(0xA270,0x18);
	HM5065_write_cmos_sensor(0xA271,0xE0);
	HM5065_write_cmos_sensor(0xA272,0x44);
	HM5065_write_cmos_sensor(0xA273,0x18);
	HM5065_write_cmos_sensor(0xA274,0xF0);
	HM5065_write_cmos_sensor(0xA275,0x00);
	HM5065_write_cmos_sensor(0xA276,0x00);
	HM5065_write_cmos_sensor(0xA277,0x00);
	HM5065_write_cmos_sensor(0xA278,0x00);
	HM5065_write_cmos_sensor(0xA279,0x00);
	HM5065_write_cmos_sensor(0xA27A,0x00);
	HM5065_write_cmos_sensor(0xA27B,0x90);
	HM5065_write_cmos_sensor(0xA27C,0x72);
	HM5065_write_cmos_sensor(0xA27D,0x08);
	HM5065_write_cmos_sensor(0xA27E,0xE0);
	HM5065_write_cmos_sensor(0xA27F,0x44);
	HM5065_write_cmos_sensor(0xA280,0x10);
	HM5065_write_cmos_sensor(0xA281,0xF0);
	HM5065_write_cmos_sensor(0xA282,0x90);
	HM5065_write_cmos_sensor(0xA283,0x72);
	HM5065_write_cmos_sensor(0xA284,0x14);
	HM5065_write_cmos_sensor(0xA285,0xE0);
	HM5065_write_cmos_sensor(0xA286,0x54);
	HM5065_write_cmos_sensor(0xA287,0xFD);
	HM5065_write_cmos_sensor(0xA288,0xF0);
	HM5065_write_cmos_sensor(0xA289,0x22);
	HM5065_write_cmos_sensor(0xA29B,0xF0);
	HM5065_write_cmos_sensor(0xA29C,0xD3);
	HM5065_write_cmos_sensor(0xA29D,0x90);
	HM5065_write_cmos_sensor(0xA29E,0x07);
	HM5065_write_cmos_sensor(0xA29F,0x91);
	HM5065_write_cmos_sensor(0xA2A0,0xE0);
	HM5065_write_cmos_sensor(0xA2A1,0x94);
	HM5065_write_cmos_sensor(0xA2A2,0x21);
	HM5065_write_cmos_sensor(0xA2A3,0x90);
	HM5065_write_cmos_sensor(0xA2A4,0x07);
	HM5065_write_cmos_sensor(0xA2A5,0x90);
	HM5065_write_cmos_sensor(0xA2A6,0xE0);
	HM5065_write_cmos_sensor(0xA2A7,0x64);
	HM5065_write_cmos_sensor(0xA2A8,0x80);
	HM5065_write_cmos_sensor(0xA2A9,0x94);
	HM5065_write_cmos_sensor(0xA2AA,0x81);
	HM5065_write_cmos_sensor(0xA2AB,0x40);
	HM5065_write_cmos_sensor(0xA2AC,0x08);
	HM5065_write_cmos_sensor(0xA2AD,0x90);
	HM5065_write_cmos_sensor(0xA2AE,0x07);
	HM5065_write_cmos_sensor(0xA2AF,0xCB);
	HM5065_write_cmos_sensor(0xA2B0,0x74);
	HM5065_write_cmos_sensor(0xA2B1,0xFF);
	HM5065_write_cmos_sensor(0xA2B2,0xF0);
	HM5065_write_cmos_sensor(0xA2B3,0x80);
	HM5065_write_cmos_sensor(0xA2B4,0x06);
	HM5065_write_cmos_sensor(0xA2B5,0x90);
	HM5065_write_cmos_sensor(0xA2B6,0x07);
	HM5065_write_cmos_sensor(0xA2B7,0xCB);
	HM5065_write_cmos_sensor(0xA2B8,0x74);
	HM5065_write_cmos_sensor(0xA2B9,0x01);
	HM5065_write_cmos_sensor(0xA2BA,0xF0);
	HM5065_write_cmos_sensor(0xA2BB,0x02);
	HM5065_write_cmos_sensor(0xA2BC,0xB5);
	HM5065_write_cmos_sensor(0xA2BD,0xC3);
	HM5065_write_cmos_sensor(0xA2BE,0x90);
	HM5065_write_cmos_sensor(0xA2BF,0x08);
	HM5065_write_cmos_sensor(0xA2C0,0x34);
	HM5065_write_cmos_sensor(0xA2C1,0xE0);
	HM5065_write_cmos_sensor(0xA2C2,0xFC);
	HM5065_write_cmos_sensor(0xA2C3,0xA3);
	HM5065_write_cmos_sensor(0xA2C4,0xE0);
	HM5065_write_cmos_sensor(0xA2C5,0xFD);
	HM5065_write_cmos_sensor(0xA2C6,0xA3);
	HM5065_write_cmos_sensor(0xA2C7,0xE0);
	HM5065_write_cmos_sensor(0xA2C8,0xFE);
	HM5065_write_cmos_sensor(0xA2C9,0xA3);
	HM5065_write_cmos_sensor(0xA2CA,0xE0);
	HM5065_write_cmos_sensor(0xA2CB,0xFF);
	HM5065_write_cmos_sensor(0xA2CC,0x90);
	HM5065_write_cmos_sensor(0xA2CD,0x07);
	HM5065_write_cmos_sensor(0xA2CE,0xD0);
	HM5065_write_cmos_sensor(0xA2CF,0xE0);
	HM5065_write_cmos_sensor(0xA2D0,0xF8);
	HM5065_write_cmos_sensor(0xA2D1,0xA3);
	HM5065_write_cmos_sensor(0xA2D2,0xE0);
	HM5065_write_cmos_sensor(0xA2D3,0xF9);
	HM5065_write_cmos_sensor(0xA2D4,0xA3);
	HM5065_write_cmos_sensor(0xA2D5,0xE0);
	HM5065_write_cmos_sensor(0xA2D6,0xFA);
	HM5065_write_cmos_sensor(0xA2D7,0xA3);
	HM5065_write_cmos_sensor(0xA2D8,0xE0);
	HM5065_write_cmos_sensor(0xA2D9,0xFB);
	HM5065_write_cmos_sensor(0xA2DA,0xD3);
	HM5065_write_cmos_sensor(0xA2DB,0x12);
	HM5065_write_cmos_sensor(0xA2DC,0x0D);
	HM5065_write_cmos_sensor(0xA2DD,0xAE);
	HM5065_write_cmos_sensor(0xA2DE,0x40);
	HM5065_write_cmos_sensor(0xA2DF,0x0B);
	HM5065_write_cmos_sensor(0xA2E0,0x12);
	HM5065_write_cmos_sensor(0xA2E1,0xB5);
	HM5065_write_cmos_sensor(0xA2E2,0x49);
	HM5065_write_cmos_sensor(0xA2E3,0x90);
	HM5065_write_cmos_sensor(0xA2E4,0x07);
	HM5065_write_cmos_sensor(0xA2E5,0xA4);
	HM5065_write_cmos_sensor(0xA2E6,0x74);
	HM5065_write_cmos_sensor(0xA2E7,0x02);
	HM5065_write_cmos_sensor(0xA2E8,0xF0);
	HM5065_write_cmos_sensor(0xA2E9,0x80);
	HM5065_write_cmos_sensor(0xA2EA,0x09);
	HM5065_write_cmos_sensor(0xA2EB,0x12);
	HM5065_write_cmos_sensor(0xA2EC,0xB7);
	HM5065_write_cmos_sensor(0xA2ED,0x51);
	HM5065_write_cmos_sensor(0xA2EE,0x90);
	HM5065_write_cmos_sensor(0xA2EF,0x07);
	HM5065_write_cmos_sensor(0xA2F0,0xA4);
	HM5065_write_cmos_sensor(0xA2F1,0x74);
	HM5065_write_cmos_sensor(0xA2F2,0x05);
	HM5065_write_cmos_sensor(0xA2F3,0xF0);
	HM5065_write_cmos_sensor(0xA2F4,0x02);
	HM5065_write_cmos_sensor(0xA2F5,0xA2);
	HM5065_write_cmos_sensor(0xA2F6,0xDA);
	HM5065_write_cmos_sensor(0xA2F7,0x90);
	HM5065_write_cmos_sensor(0xA2F8,0x0E);
	HM5065_write_cmos_sensor(0xA2F9,0xE0);
	HM5065_write_cmos_sensor(0xA2FA,0xE0);
	HM5065_write_cmos_sensor(0xA2FB,0xFD);
	HM5065_write_cmos_sensor(0xA2FC,0xA3);
	HM5065_write_cmos_sensor(0xA2FD,0xE0);
	HM5065_write_cmos_sensor(0xA2FE,0x90);
	HM5065_write_cmos_sensor(0xA2FF,0x02);
	HM5065_write_cmos_sensor(0xA300,0xA2);
	HM5065_write_cmos_sensor(0xA301,0xCD);
	HM5065_write_cmos_sensor(0xA302,0xF0);
	HM5065_write_cmos_sensor(0xA303,0xA3);
	HM5065_write_cmos_sensor(0xA304,0xED);
	HM5065_write_cmos_sensor(0xA305,0xF0);
	HM5065_write_cmos_sensor(0xA306,0x90);
	HM5065_write_cmos_sensor(0xA307,0x0E);
	HM5065_write_cmos_sensor(0xA308,0xE2);
	HM5065_write_cmos_sensor(0xA309,0xE0);
	HM5065_write_cmos_sensor(0xA30A,0xFD);
	HM5065_write_cmos_sensor(0xA30B,0xA3);
	HM5065_write_cmos_sensor(0xA30C,0xE0);
	HM5065_write_cmos_sensor(0xA30D,0x90);
	HM5065_write_cmos_sensor(0xA30E,0x02);
	HM5065_write_cmos_sensor(0xA30F,0xA8);
	HM5065_write_cmos_sensor(0xA310,0xCD);
	HM5065_write_cmos_sensor(0xA311,0xF0);
	HM5065_write_cmos_sensor(0xA312,0xA3);
	HM5065_write_cmos_sensor(0xA313,0xED);
	HM5065_write_cmos_sensor(0xA314,0xF0);
	HM5065_write_cmos_sensor(0xA315,0xE4);
	HM5065_write_cmos_sensor(0xA316,0x90);
	HM5065_write_cmos_sensor(0xA317,0x06);
	HM5065_write_cmos_sensor(0xA318,0x38);
	HM5065_write_cmos_sensor(0xA319,0xF0);
	HM5065_write_cmos_sensor(0xA31A,0x02);
	HM5065_write_cmos_sensor(0xA31B,0x67);
	HM5065_write_cmos_sensor(0xA31C,0x63);
	HM5065_write_cmos_sensor(0xA31D,0x90);
	HM5065_write_cmos_sensor(0xA31E,0x0E);
	HM5065_write_cmos_sensor(0xA31F,0xE8);
	HM5065_write_cmos_sensor(0xA320,0xE0);
	HM5065_write_cmos_sensor(0xA321,0x90);
	HM5065_write_cmos_sensor(0xA322,0x02);
	HM5065_write_cmos_sensor(0xA323,0x62);
	HM5065_write_cmos_sensor(0xA324,0xF0);
	HM5065_write_cmos_sensor(0xA325,0x90);
	HM5065_write_cmos_sensor(0xA326,0x0E);
	HM5065_write_cmos_sensor(0xA327,0xE9);
	HM5065_write_cmos_sensor(0xA328,0xE0);
	HM5065_write_cmos_sensor(0xA329,0x90);
	HM5065_write_cmos_sensor(0xA32A,0x02);
	HM5065_write_cmos_sensor(0xA32B,0x63);
	HM5065_write_cmos_sensor(0xA32C,0xF0);
	HM5065_write_cmos_sensor(0xA32D,0x02);
	HM5065_write_cmos_sensor(0xA32E,0x67);
	HM5065_write_cmos_sensor(0xA32F,0x1F);
	HM5065_write_cmos_sensor(0xA33B,0x90);
	HM5065_write_cmos_sensor(0xA33C,0x0E);
	HM5065_write_cmos_sensor(0xA33D,0x14);
	HM5065_write_cmos_sensor(0xA33E,0xE0);
	HM5065_write_cmos_sensor(0xA33F,0xFE);
	HM5065_write_cmos_sensor(0xA340,0xA3);
	HM5065_write_cmos_sensor(0xA341,0xE0);
	HM5065_write_cmos_sensor(0xA342,0xFF);
	HM5065_write_cmos_sensor(0xA343,0x90);
	HM5065_write_cmos_sensor(0xA344,0x06);
	HM5065_write_cmos_sensor(0xA345,0xD9);
	HM5065_write_cmos_sensor(0xA346,0xEE);
	HM5065_write_cmos_sensor(0xA347,0xF0);
	HM5065_write_cmos_sensor(0xA348,0xA3);
	HM5065_write_cmos_sensor(0xA349,0xEF);
	HM5065_write_cmos_sensor(0xA34A,0xF0);
	HM5065_write_cmos_sensor(0xA34B,0x90);
	HM5065_write_cmos_sensor(0xA34C,0x0E);
	HM5065_write_cmos_sensor(0xA34D,0x18);
	HM5065_write_cmos_sensor(0xA34E,0xE0);
	HM5065_write_cmos_sensor(0xA34F,0xFD);
	HM5065_write_cmos_sensor(0xA350,0x7C);
	HM5065_write_cmos_sensor(0xA351,0x00);
	HM5065_write_cmos_sensor(0xA352,0xC3);
	HM5065_write_cmos_sensor(0xA353,0xEF);
	HM5065_write_cmos_sensor(0xA354,0x9D);
	HM5065_write_cmos_sensor(0xA355,0xEE);
	HM5065_write_cmos_sensor(0xA356,0x9C);
	HM5065_write_cmos_sensor(0xA357,0x50);
	HM5065_write_cmos_sensor(0xA358,0x09);
	HM5065_write_cmos_sensor(0xA359,0xE4);
	HM5065_write_cmos_sensor(0xA35A,0x90);
	HM5065_write_cmos_sensor(0xA35B,0x06);
	HM5065_write_cmos_sensor(0xA35C,0xD7);
	HM5065_write_cmos_sensor(0xA35D,0xF0);
	HM5065_write_cmos_sensor(0xA35E,0xA3);
	HM5065_write_cmos_sensor(0xA35F,0xF0);
	HM5065_write_cmos_sensor(0xA360,0x80);
	HM5065_write_cmos_sensor(0xA361,0x13);
	HM5065_write_cmos_sensor(0xA362,0xC3);
	HM5065_write_cmos_sensor(0xA363,0x90);
	HM5065_write_cmos_sensor(0xA364,0x06);
	HM5065_write_cmos_sensor(0xA365,0xDA);
	HM5065_write_cmos_sensor(0xA366,0xE0);
	HM5065_write_cmos_sensor(0xA367,0x9D);
	HM5065_write_cmos_sensor(0xA368,0xFE);
	HM5065_write_cmos_sensor(0xA369,0x90);
	HM5065_write_cmos_sensor(0xA36A,0x06);
	HM5065_write_cmos_sensor(0xA36B,0xD9);
	HM5065_write_cmos_sensor(0xA36C,0xE0);
	HM5065_write_cmos_sensor(0xA36D,0x9C);
	HM5065_write_cmos_sensor(0xA36E,0x90);
	HM5065_write_cmos_sensor(0xA36F,0x06);
	HM5065_write_cmos_sensor(0xA370,0xD7);
	HM5065_write_cmos_sensor(0xA371,0xF0);
	HM5065_write_cmos_sensor(0xA372,0xA3);
	HM5065_write_cmos_sensor(0xA373,0xCE);
	HM5065_write_cmos_sensor(0xA374,0xF0);
	HM5065_write_cmos_sensor(0xA375,0x90);
	HM5065_write_cmos_sensor(0xA376,0x0E);
	HM5065_write_cmos_sensor(0xA377,0x18);
	HM5065_write_cmos_sensor(0xA378,0xE0);
	HM5065_write_cmos_sensor(0xA379,0xF9);
	HM5065_write_cmos_sensor(0xA37A,0xFF);
	HM5065_write_cmos_sensor(0xA37B,0x90);
	HM5065_write_cmos_sensor(0xA37C,0x06);
	HM5065_write_cmos_sensor(0xA37D,0xC2);
	HM5065_write_cmos_sensor(0xA37E,0xE0);
	HM5065_write_cmos_sensor(0xA37F,0xFC);
	HM5065_write_cmos_sensor(0xA380,0xA3);
	HM5065_write_cmos_sensor(0xA381,0xE0);
	HM5065_write_cmos_sensor(0xA382,0xFD);
	HM5065_write_cmos_sensor(0xA383,0xC3);
	HM5065_write_cmos_sensor(0xA384,0x9F);
	HM5065_write_cmos_sensor(0xA385,0xFF);
	HM5065_write_cmos_sensor(0xA386,0xEC);
	HM5065_write_cmos_sensor(0xA387,0x94);
	HM5065_write_cmos_sensor(0xA388,0x00);
	HM5065_write_cmos_sensor(0xA389,0xFE);
	HM5065_write_cmos_sensor(0xA38A,0x90);
	HM5065_write_cmos_sensor(0xA38B,0x0E);
	HM5065_write_cmos_sensor(0xA38C,0x16);
	HM5065_write_cmos_sensor(0xA38D,0xE0);
	HM5065_write_cmos_sensor(0xA38E,0xFA);
	HM5065_write_cmos_sensor(0xA38F,0xA3);
	HM5065_write_cmos_sensor(0xA390,0xE0);
	HM5065_write_cmos_sensor(0xA391,0xFB);
	HM5065_write_cmos_sensor(0xA392,0xD3);
	HM5065_write_cmos_sensor(0xA393,0x9F);
	HM5065_write_cmos_sensor(0xA394,0xEA);
	HM5065_write_cmos_sensor(0xA395,0x9E);
	HM5065_write_cmos_sensor(0xA396,0x40);
	HM5065_write_cmos_sensor(0xA397,0x0A);
	HM5065_write_cmos_sensor(0xA398,0x90);
	HM5065_write_cmos_sensor(0xA399,0x06);
	HM5065_write_cmos_sensor(0xA39A,0xD5);
	HM5065_write_cmos_sensor(0xA39B,0xEC);
	HM5065_write_cmos_sensor(0xA39C,0xF0);
	HM5065_write_cmos_sensor(0xA39D,0xA3);
	HM5065_write_cmos_sensor(0xA39E,0xED);
	HM5065_write_cmos_sensor(0xA39F,0xF0);
	HM5065_write_cmos_sensor(0xA3A0,0x80);
	HM5065_write_cmos_sensor(0xA3A1,0x0E);
	HM5065_write_cmos_sensor(0xA3A2,0xE9);
	HM5065_write_cmos_sensor(0xA3A3,0x7E);
	HM5065_write_cmos_sensor(0xA3A4,0x00);
	HM5065_write_cmos_sensor(0xA3A5,0x2B);
	HM5065_write_cmos_sensor(0xA3A6,0xFF);
	HM5065_write_cmos_sensor(0xA3A7,0xEE);
	HM5065_write_cmos_sensor(0xA3A8,0x3A);
	HM5065_write_cmos_sensor(0xA3A9,0x90);
	HM5065_write_cmos_sensor(0xA3AA,0x06);
	HM5065_write_cmos_sensor(0xA3AB,0xD5);
	HM5065_write_cmos_sensor(0xA3AC,0xF0);
	HM5065_write_cmos_sensor(0xA3AD,0xA3);
	HM5065_write_cmos_sensor(0xA3AE,0xEF);
	HM5065_write_cmos_sensor(0xA3AF,0xF0);
	HM5065_write_cmos_sensor(0xA3B0,0xE9);
	HM5065_write_cmos_sensor(0xA3B1,0xFB);
	HM5065_write_cmos_sensor(0xA3B2,0x7A);
	HM5065_write_cmos_sensor(0xA3B3,0x00);
	HM5065_write_cmos_sensor(0xA3B4,0x90);
	HM5065_write_cmos_sensor(0xA3B5,0x0E);
	HM5065_write_cmos_sensor(0xA3B6,0x15);
	HM5065_write_cmos_sensor(0xA3B7,0xE0);
	HM5065_write_cmos_sensor(0xA3B8,0x2B);
	HM5065_write_cmos_sensor(0xA3B9,0xFE);
	HM5065_write_cmos_sensor(0xA3BA,0x90);
	HM5065_write_cmos_sensor(0xA3BB,0x0E);
	HM5065_write_cmos_sensor(0xA3BC,0x14);
	HM5065_write_cmos_sensor(0xA3BD,0xE0);
	HM5065_write_cmos_sensor(0xA3BE,0x3A);
	HM5065_write_cmos_sensor(0xA3BF,0x90);
	HM5065_write_cmos_sensor(0xA3C0,0x06);
	HM5065_write_cmos_sensor(0xA3C1,0xE1);
	HM5065_write_cmos_sensor(0xA3C2,0xF0);
	HM5065_write_cmos_sensor(0xA3C3,0xA3);
	HM5065_write_cmos_sensor(0xA3C4,0xCE);
	HM5065_write_cmos_sensor(0xA3C5,0xF0);
	HM5065_write_cmos_sensor(0xA3C6,0xC3);
	HM5065_write_cmos_sensor(0xA3C7,0x90);
	HM5065_write_cmos_sensor(0xA3C8,0x0E);
	HM5065_write_cmos_sensor(0xA3C9,0x17);
	HM5065_write_cmos_sensor(0xA3CA,0xE0);
	HM5065_write_cmos_sensor(0xA3CB,0x9B);
	HM5065_write_cmos_sensor(0xA3CC,0xFE);
	HM5065_write_cmos_sensor(0xA3CD,0x90);
	HM5065_write_cmos_sensor(0xA3CE,0x0E);
	HM5065_write_cmos_sensor(0xA3CF,0x16);
	HM5065_write_cmos_sensor(0xA3D0,0x02);
	HM5065_write_cmos_sensor(0xA3D1,0x20);
	HM5065_write_cmos_sensor(0xA3D2,0xD5);
	HM5065_write_cmos_sensor(0xA3D3,0x90);
	HM5065_write_cmos_sensor(0xA3d4,0x0E);
	HM5065_write_cmos_sensor(0xA3d5,0xE4);
	HM5065_write_cmos_sensor(0xA3d6,0xE0);
	HM5065_write_cmos_sensor(0xA3d7,0x90);
	HM5065_write_cmos_sensor(0xA3d8,0x02);
	HM5065_write_cmos_sensor(0xA3d9,0x66);
	HM5065_write_cmos_sensor(0xA3da,0xF0);
	HM5065_write_cmos_sensor(0xA3DB,0x90);
	HM5065_write_cmos_sensor(0xA3dc,0x0E);
	HM5065_write_cmos_sensor(0xA3dd,0xE5);
	HM5065_write_cmos_sensor(0xA3de,0xE0);
	HM5065_write_cmos_sensor(0xA3df,0x90);
	HM5065_write_cmos_sensor(0xA3e0,0x02);
	HM5065_write_cmos_sensor(0xA3e1,0x64);
	HM5065_write_cmos_sensor(0xA3e2,0xF0);
	HM5065_write_cmos_sensor(0xA3e3,0x90);
	HM5065_write_cmos_sensor(0xA3e4,0x0E);
	HM5065_write_cmos_sensor(0xA3e5,0xE6);
	HM5065_write_cmos_sensor(0xA3e6,0xE0);
	HM5065_write_cmos_sensor(0xA3e7,0x90);
	HM5065_write_cmos_sensor(0xA3e8,0x02);
	HM5065_write_cmos_sensor(0xA3e9,0x65);
	HM5065_write_cmos_sensor(0xA3ea,0xF0);
	HM5065_write_cmos_sensor(0xA3eb,0x02);
	HM5065_write_cmos_sensor(0xA3ec,0x67);
	HM5065_write_cmos_sensor(0xA3ed,0xA5);
	HM5065_write_cmos_sensor(0xA3f0,0x12);
	HM5065_write_cmos_sensor(0xA3f1,0x47);
	HM5065_write_cmos_sensor(0xA3f2,0x59);
	HM5065_write_cmos_sensor(0xA3f3,0x90);
	HM5065_write_cmos_sensor(0xA3f4,0x00);
	HM5065_write_cmos_sensor(0xA3f5,0xB5);
	HM5065_write_cmos_sensor(0xA3f6,0xE0);
	HM5065_write_cmos_sensor(0xA3f7,0xB4);
	HM5065_write_cmos_sensor(0xA3f8,0x02);
	HM5065_write_cmos_sensor(0xA3f9,0x03);
	HM5065_write_cmos_sensor(0xA3fa,0x12);
	HM5065_write_cmos_sensor(0xA3fb,0x47);
	HM5065_write_cmos_sensor(0xA3fc,0x59);
	HM5065_write_cmos_sensor(0xA3fd,0x02);
	HM5065_write_cmos_sensor(0xA3fe,0xC5);
	HM5065_write_cmos_sensor(0xA3ff,0xC3);
	HM5065_write_cmos_sensor(0xA400,0x90);
	HM5065_write_cmos_sensor(0xA401,0x00);
	HM5065_write_cmos_sensor(0xA402,0x3D);
	HM5065_write_cmos_sensor(0xA403,0xF0);
	HM5065_write_cmos_sensor(0xA404,0x90);
	HM5065_write_cmos_sensor(0xA405,0x00);
	HM5065_write_cmos_sensor(0xA406,0x84);
	HM5065_write_cmos_sensor(0xA407,0xE0);
	HM5065_write_cmos_sensor(0xA408,0xFE);
	HM5065_write_cmos_sensor(0xA409,0x90);
	HM5065_write_cmos_sensor(0xA40A,0x00);
	HM5065_write_cmos_sensor(0xA40B,0x3E);
	HM5065_write_cmos_sensor(0xA40C,0xF0);
	HM5065_write_cmos_sensor(0xA40D,0xEF);
	HM5065_write_cmos_sensor(0xA40E,0x70);
	HM5065_write_cmos_sensor(0xA40F,0x03);
	HM5065_write_cmos_sensor(0xA410,0xEE);
	HM5065_write_cmos_sensor(0xA411,0x60);
	HM5065_write_cmos_sensor(0xA412,0x04);
	HM5065_write_cmos_sensor(0xA413,0x7F);
	HM5065_write_cmos_sensor(0xA414,0x01);
	HM5065_write_cmos_sensor(0xA415,0x80);
	HM5065_write_cmos_sensor(0xA416,0x02);
	HM5065_write_cmos_sensor(0xA417,0x7F);
	HM5065_write_cmos_sensor(0xA418,0x00);
	HM5065_write_cmos_sensor(0xA419,0x90);
	HM5065_write_cmos_sensor(0xA41A,0x00);
	HM5065_write_cmos_sensor(0xA41B,0x3F);
	HM5065_write_cmos_sensor(0xA41C,0xEF);
	HM5065_write_cmos_sensor(0xA41D,0xF0);
	HM5065_write_cmos_sensor(0xA41E,0x02);
	HM5065_write_cmos_sensor(0xA41F,0x89);
	HM5065_write_cmos_sensor(0xA420,0xD3);
	HM5065_write_cmos_sensor(0xA421,0x90);
	HM5065_write_cmos_sensor(0xA422,0x00);
	HM5065_write_cmos_sensor(0xA423,0x12);
	HM5065_write_cmos_sensor(0xA424,0xE0);
	HM5065_write_cmos_sensor(0xA425,0xFF);
	HM5065_write_cmos_sensor(0xA426,0x70);
	HM5065_write_cmos_sensor(0xA427,0x0C);
	HM5065_write_cmos_sensor(0xA428,0x90);
	HM5065_write_cmos_sensor(0xA429,0x00);
	HM5065_write_cmos_sensor(0xA42A,0x46);
	HM5065_write_cmos_sensor(0xA42B,0xE0);
	HM5065_write_cmos_sensor(0xA42C,0xC3);
	HM5065_write_cmos_sensor(0xA42D,0x94);
	HM5065_write_cmos_sensor(0xA42E,0x07);
	HM5065_write_cmos_sensor(0xA42F,0x40);
	HM5065_write_cmos_sensor(0xA430,0x03);
	HM5065_write_cmos_sensor(0xA431,0x75);
	HM5065_write_cmos_sensor(0xA432,0x2E);
	HM5065_write_cmos_sensor(0xA433,0x02);
	HM5065_write_cmos_sensor(0xA434,0xEF);
	HM5065_write_cmos_sensor(0xA435,0xB4);
	HM5065_write_cmos_sensor(0xA436,0x01);
	HM5065_write_cmos_sensor(0xA437,0x0C);
	HM5065_write_cmos_sensor(0xA438,0x90);
	HM5065_write_cmos_sensor(0xA439,0x00);
	HM5065_write_cmos_sensor(0xA43A,0x66);
	HM5065_write_cmos_sensor(0xA43B,0xE0);
	HM5065_write_cmos_sensor(0xA43C,0xC3);
	HM5065_write_cmos_sensor(0xA43D,0x94);
	HM5065_write_cmos_sensor(0xA43E,0x07);
	HM5065_write_cmos_sensor(0xA43F,0x40);
	HM5065_write_cmos_sensor(0xA440,0x03); 
	HM5065_write_cmos_sensor(0xA441,0x75);
	HM5065_write_cmos_sensor(0xA442,0x2E);
	HM5065_write_cmos_sensor(0xA443,0x02);
	HM5065_write_cmos_sensor(0xA444,0x02);
	HM5065_write_cmos_sensor(0xA445,0xA7);
	HM5065_write_cmos_sensor(0xA446,0x9E);
	HM5065_write_cmos_sensor(0xA447,0xC3);
	HM5065_write_cmos_sensor(0xA448,0x90);
	HM5065_write_cmos_sensor(0xA449,0x0B);
	HM5065_write_cmos_sensor(0xA44A,0x8F);
	HM5065_write_cmos_sensor(0xA44B,0xE0);
	HM5065_write_cmos_sensor(0xA44C,0x94);
	HM5065_write_cmos_sensor(0xA44D,0x40);//derek 80->40
	HM5065_write_cmos_sensor(0xA44E,0x90);
	HM5065_write_cmos_sensor(0xA44F,0x0B);
	HM5065_write_cmos_sensor(0xA450,0x8E);
	HM5065_write_cmos_sensor(0xA451,0xE0);
	HM5065_write_cmos_sensor(0xA452,0x94);
	HM5065_write_cmos_sensor(0xA453,0x42);//derek 44->42
	HM5065_write_cmos_sensor(0xA454,0x40);
	HM5065_write_cmos_sensor(0xA455,0x22);
	HM5065_write_cmos_sensor(0xA456,0x90);
	HM5065_write_cmos_sensor(0xA457,0x0B);
	HM5065_write_cmos_sensor(0xA458,0x91);
	HM5065_write_cmos_sensor(0xA459,0xE0);
	HM5065_write_cmos_sensor(0xA45A,0x94);
	HM5065_write_cmos_sensor(0xA45B,0x40);//derek 80->40
	HM5065_write_cmos_sensor(0xA45C,0x90);
	HM5065_write_cmos_sensor(0xA45D,0x0B);
	HM5065_write_cmos_sensor(0xA45E,0x90);
	HM5065_write_cmos_sensor(0xA45F,0xE0);
	HM5065_write_cmos_sensor(0xA460,0x94);
	HM5065_write_cmos_sensor(0xA461,0x42);//derek 44->42
	HM5065_write_cmos_sensor(0xA462,0x40);
	HM5065_write_cmos_sensor(0xA463,0x14);
	HM5065_write_cmos_sensor(0xA464,0x90);
	HM5065_write_cmos_sensor(0xA465,0x0B);
	HM5065_write_cmos_sensor(0xA466,0x93);
	HM5065_write_cmos_sensor(0xA467,0xE0);
	HM5065_write_cmos_sensor(0xA468,0x94);
	HM5065_write_cmos_sensor(0xA469,0x40);//derek 80->40
	HM5065_write_cmos_sensor(0xA46A,0x90);
	HM5065_write_cmos_sensor(0xA46B,0x0B);
	HM5065_write_cmos_sensor(0xA46C,0x92);
	HM5065_write_cmos_sensor(0xA46D,0xE0);
	HM5065_write_cmos_sensor(0xA46E,0x94);
	HM5065_write_cmos_sensor(0xA46F,0x42);//derek 44->42
	HM5065_write_cmos_sensor(0xA470,0x40);
	HM5065_write_cmos_sensor(0xA471,0x06);
	HM5065_write_cmos_sensor(0xA472,0x90);
	HM5065_write_cmos_sensor(0xA473,0x01);
	HM5065_write_cmos_sensor(0xA474,0xA4);
	HM5065_write_cmos_sensor(0xA475,0x02);
	HM5065_write_cmos_sensor(0xA476,0x86);
	HM5065_write_cmos_sensor(0xA477,0x57);
	HM5065_write_cmos_sensor(0xA478,0x02);
	HM5065_write_cmos_sensor(0xA479,0x86);
	HM5065_write_cmos_sensor(0xA47A,0x5C);
	HM5065_write_cmos_sensor(0xA500,0xF5);
	HM5065_write_cmos_sensor(0xA501,0x3B);
	HM5065_write_cmos_sensor(0xA502,0x90);
	HM5065_write_cmos_sensor(0xA503,0x06);
	HM5065_write_cmos_sensor(0xA504,0x6C);
	HM5065_write_cmos_sensor(0xA505,0xE0);
	HM5065_write_cmos_sensor(0xA506,0xFF);
	HM5065_write_cmos_sensor(0xA507,0xE5);
	HM5065_write_cmos_sensor(0xA508,0x3B);
	HM5065_write_cmos_sensor(0xA509,0xC3);
	HM5065_write_cmos_sensor(0xA50A,0x9F);
	HM5065_write_cmos_sensor(0xA50B,0x40);
	HM5065_write_cmos_sensor(0xA50C,0x03);
	HM5065_write_cmos_sensor(0xA50D,0x02);
	HM5065_write_cmos_sensor(0xA50E,0xF6);
	HM5065_write_cmos_sensor(0xA50F,0x0E);
	HM5065_write_cmos_sensor(0xA510,0x90);
	HM5065_write_cmos_sensor(0xA511,0x0B);
	HM5065_write_cmos_sensor(0xA512,0xC6);
	HM5065_write_cmos_sensor(0xA513,0xE0);
	HM5065_write_cmos_sensor(0xA514,0x14);
	HM5065_write_cmos_sensor(0xA515,0x60);
	HM5065_write_cmos_sensor(0xA516,0x3C);
	HM5065_write_cmos_sensor(0xA517,0x14);
	HM5065_write_cmos_sensor(0xA518,0x60);
	HM5065_write_cmos_sensor(0xA519,0x6B);
	HM5065_write_cmos_sensor(0xA51A,0x24);
	HM5065_write_cmos_sensor(0xA51B,0x02);
	HM5065_write_cmos_sensor(0xA51C,0x60);
	HM5065_write_cmos_sensor(0xA51D,0x03);
	HM5065_write_cmos_sensor(0xA51E,0x02);
	HM5065_write_cmos_sensor(0xA51F,0xF5);
	HM5065_write_cmos_sensor(0xA520,0xB5);
	HM5065_write_cmos_sensor(0xA521,0x90);
	HM5065_write_cmos_sensor(0xA522,0x0A);
	HM5065_write_cmos_sensor(0xA523,0x9A);
	HM5065_write_cmos_sensor(0xA524,0xE0);
	HM5065_write_cmos_sensor(0xA525,0xFB);
	HM5065_write_cmos_sensor(0xA526,0xA3);
	HM5065_write_cmos_sensor(0xA527,0xE0);
	HM5065_write_cmos_sensor(0xA528,0xFA);
	HM5065_write_cmos_sensor(0xA529,0xA3);
	HM5065_write_cmos_sensor(0xA52A,0xE0);
	HM5065_write_cmos_sensor(0xA52B,0xF9);
	HM5065_write_cmos_sensor(0xA52C,0x85);
	HM5065_write_cmos_sensor(0xA52D,0x3B);
	HM5065_write_cmos_sensor(0xA52E,0x82);
	HM5065_write_cmos_sensor(0xA52F,0x75);
	HM5065_write_cmos_sensor(0xA530,0x83);
	HM5065_write_cmos_sensor(0xA531,0x00);
	HM5065_write_cmos_sensor(0xA532,0x12);
	HM5065_write_cmos_sensor(0xA533,0x0A);
	HM5065_write_cmos_sensor(0xA534,0xB8);
	HM5065_write_cmos_sensor(0xA535,0xFF);
	HM5065_write_cmos_sensor(0xA536,0x74);
	HM5065_write_cmos_sensor(0xA537,0xAB);
	HM5065_write_cmos_sensor(0xA538,0x25);
	HM5065_write_cmos_sensor(0xA539,0x3B);
	HM5065_write_cmos_sensor(0xA53A,0xF5);
	HM5065_write_cmos_sensor(0xA53B,0x82);
	HM5065_write_cmos_sensor(0xA53C,0xE4);
	HM5065_write_cmos_sensor(0xA53D,0x34);
	HM5065_write_cmos_sensor(0xA53E,0x0A);
	HM5065_write_cmos_sensor(0xA53F,0xF5);
	HM5065_write_cmos_sensor(0xA540,0x83);
	HM5065_write_cmos_sensor(0xA541,0xE0);
	HM5065_write_cmos_sensor(0xA542,0xFD);
	HM5065_write_cmos_sensor(0xA543,0xC3);
	HM5065_write_cmos_sensor(0xA544,0xEF);
	HM5065_write_cmos_sensor(0xA545,0x9D);
	HM5065_write_cmos_sensor(0xA546,0xFE);
	HM5065_write_cmos_sensor(0xA547,0xE4);
	HM5065_write_cmos_sensor(0xA548,0x94);
	HM5065_write_cmos_sensor(0xA549,0x00);
	HM5065_write_cmos_sensor(0xA54A,0x90);
	HM5065_write_cmos_sensor(0xA54B,0x0B);
	HM5065_write_cmos_sensor(0xA54C,0xCA);
	HM5065_write_cmos_sensor(0xA54D,0xF0);
	HM5065_write_cmos_sensor(0xA54E,0xA3);
	HM5065_write_cmos_sensor(0xA54F,0xCE);
	HM5065_write_cmos_sensor(0xA550,0xF0);
	HM5065_write_cmos_sensor(0xA551,0x80);
	HM5065_write_cmos_sensor(0xA552,0x62);
	HM5065_write_cmos_sensor(0xA553,0x90);
	HM5065_write_cmos_sensor(0xA554,0x0A);
	HM5065_write_cmos_sensor(0xA555,0x9A);
	HM5065_write_cmos_sensor(0xA556,0xE0);
	HM5065_write_cmos_sensor(0xA557,0xFB);
	HM5065_write_cmos_sensor(0xA558,0xA3);
	HM5065_write_cmos_sensor(0xA559,0xE0);
	HM5065_write_cmos_sensor(0xA55A,0xFA);
	HM5065_write_cmos_sensor(0xA55B,0xA3);
	HM5065_write_cmos_sensor(0xA55C,0xE0);
	HM5065_write_cmos_sensor(0xA55D,0xF9);
	HM5065_write_cmos_sensor(0xA55E,0x85);
	HM5065_write_cmos_sensor(0xA55F,0x3B);
	HM5065_write_cmos_sensor(0xA560,0x82);
	HM5065_write_cmos_sensor(0xA561,0x75);
	HM5065_write_cmos_sensor(0xA562,0x83);
	HM5065_write_cmos_sensor(0xA563,0x00);
	HM5065_write_cmos_sensor(0xA564,0x12);
	HM5065_write_cmos_sensor(0xA565,0x0A);
	HM5065_write_cmos_sensor(0xA566,0xB8);
	HM5065_write_cmos_sensor(0xA567,0xFF);
	HM5065_write_cmos_sensor(0xA568,0x74);
	HM5065_write_cmos_sensor(0xA569,0x9D);
	HM5065_write_cmos_sensor(0xA56A,0x25);
	HM5065_write_cmos_sensor(0xA56B,0x3B);
	HM5065_write_cmos_sensor(0xA56C,0xF5);
	HM5065_write_cmos_sensor(0xA56D,0x82);
	HM5065_write_cmos_sensor(0xA56E,0xE4);
	HM5065_write_cmos_sensor(0xA56F,0x34);
	HM5065_write_cmos_sensor(0xA570,0x0A);
	HM5065_write_cmos_sensor(0xA571,0xF5);
	HM5065_write_cmos_sensor(0xA572,0x83);
	HM5065_write_cmos_sensor(0xA573,0xE0);
	HM5065_write_cmos_sensor(0xA574,0xFD);
	HM5065_write_cmos_sensor(0xA575,0xC3);
	HM5065_write_cmos_sensor(0xA576,0xEF);
	HM5065_write_cmos_sensor(0xA577,0x9D);
	HM5065_write_cmos_sensor(0xA578,0xFE);
	HM5065_write_cmos_sensor(0xA579,0xE4);
	HM5065_write_cmos_sensor(0xA57A,0x94);
	HM5065_write_cmos_sensor(0xA57B,0x00);
	HM5065_write_cmos_sensor(0xA57C,0x90);
	HM5065_write_cmos_sensor(0xA57D,0x0B);
	HM5065_write_cmos_sensor(0xA57E,0xCA);
	HM5065_write_cmos_sensor(0xA57F,0xF0);
	HM5065_write_cmos_sensor(0xA580,0xA3);
	HM5065_write_cmos_sensor(0xA581,0xCE);
	HM5065_write_cmos_sensor(0xA582,0xF0);
	HM5065_write_cmos_sensor(0xA583,0x80);
	HM5065_write_cmos_sensor(0xA584,0x30);
	HM5065_write_cmos_sensor(0xA585,0x90);
	HM5065_write_cmos_sensor(0xA586,0x0A);
	HM5065_write_cmos_sensor(0xA587,0x9A);
	HM5065_write_cmos_sensor(0xA588,0xE0);
	HM5065_write_cmos_sensor(0xA589,0xFB);
	HM5065_write_cmos_sensor(0xA58A,0xA3);
	HM5065_write_cmos_sensor(0xA58B,0xE0);
	HM5065_write_cmos_sensor(0xA58C,0xFA);
	HM5065_write_cmos_sensor(0xA58D,0xA3);
	HM5065_write_cmos_sensor(0xA58E,0xE0);
	HM5065_write_cmos_sensor(0xA58F,0xF9);
	HM5065_write_cmos_sensor(0xA590,0x85);
	HM5065_write_cmos_sensor(0xA591,0x3B);
	HM5065_write_cmos_sensor(0xA592,0x82);
	HM5065_write_cmos_sensor(0xA593,0x75);
	HM5065_write_cmos_sensor(0xA594,0x83);
	HM5065_write_cmos_sensor(0xA595,0x00);
	HM5065_write_cmos_sensor(0xA596,0x12);
	HM5065_write_cmos_sensor(0xA597,0x0A);
	HM5065_write_cmos_sensor(0xA598,0xB8);
	HM5065_write_cmos_sensor(0xA599,0xFF);
	HM5065_write_cmos_sensor(0xA59A,0x74);
	HM5065_write_cmos_sensor(0xA59B,0xA4);
	HM5065_write_cmos_sensor(0xA59C,0x25);
	HM5065_write_cmos_sensor(0xA59D,0x3B);
	HM5065_write_cmos_sensor(0xA59E,0xF5);
	HM5065_write_cmos_sensor(0xA59F,0x82);
	HM5065_write_cmos_sensor(0xA5A0,0xE4);
	HM5065_write_cmos_sensor(0xA5A1,0x34);
	HM5065_write_cmos_sensor(0xA5A2,0x0A);
	HM5065_write_cmos_sensor(0xA5A3,0xF5);
	HM5065_write_cmos_sensor(0xA5A4,0x83);
	HM5065_write_cmos_sensor(0xA5A5,0xE0);
	HM5065_write_cmos_sensor(0xA5A6,0xFD);
	HM5065_write_cmos_sensor(0xA5A7,0xC3);
	HM5065_write_cmos_sensor(0xA5A8,0xEF);
	HM5065_write_cmos_sensor(0xA5A9,0x9D);
	HM5065_write_cmos_sensor(0xA5AA,0xFE);
	HM5065_write_cmos_sensor(0xA5AB,0xE4);
	HM5065_write_cmos_sensor(0xA5AC,0x94);
	HM5065_write_cmos_sensor(0xA5AD,0x00);
	HM5065_write_cmos_sensor(0xA5AE,0x90);
	HM5065_write_cmos_sensor(0xA5AF,0x0B);
	HM5065_write_cmos_sensor(0xA5B0,0xCA);
	HM5065_write_cmos_sensor(0xA5B1,0xF0);
	HM5065_write_cmos_sensor(0xA5B2,0xA3);
	HM5065_write_cmos_sensor(0xA5B3,0xCE);
	HM5065_write_cmos_sensor(0xA5B4,0xF0);
	HM5065_write_cmos_sensor(0xA5B5,0x90);
	HM5065_write_cmos_sensor(0xA5B6,0x07);
	HM5065_write_cmos_sensor(0xA5B7,0x83);
	HM5065_write_cmos_sensor(0xA5B8,0xE0);
	HM5065_write_cmos_sensor(0xA5B9,0xFF);
	HM5065_write_cmos_sensor(0xA5BA,0x7E);
	HM5065_write_cmos_sensor(0xA5BB,0x00);
	HM5065_write_cmos_sensor(0xA5BC,0x90);
	HM5065_write_cmos_sensor(0xA5BD,0x0D);
	HM5065_write_cmos_sensor(0xA5BE,0xF6);
	HM5065_write_cmos_sensor(0xA5BF,0xEE);
	HM5065_write_cmos_sensor(0xA5C0,0xF0);
	HM5065_write_cmos_sensor(0xA5C1,0xA3);
	HM5065_write_cmos_sensor(0xA5C2,0xEF);
	HM5065_write_cmos_sensor(0xA5C3,0xF0);
	HM5065_write_cmos_sensor(0xA5C4,0x90);
	HM5065_write_cmos_sensor(0xA5C5,0x0B);
	HM5065_write_cmos_sensor(0xA5C6,0xCA);
	HM5065_write_cmos_sensor(0xA5C7,0xE0);
	HM5065_write_cmos_sensor(0xA5C8,0xFC);
	HM5065_write_cmos_sensor(0xA5C9,0xA3);
	HM5065_write_cmos_sensor(0xA5CA,0xE0);
	HM5065_write_cmos_sensor(0xA5CB,0xFD);
	HM5065_write_cmos_sensor(0xA5CC,0xD3);
	HM5065_write_cmos_sensor(0xA5CD,0x9F);
	HM5065_write_cmos_sensor(0xA5CE,0x74);
	HM5065_write_cmos_sensor(0xA5CF,0x80);
	HM5065_write_cmos_sensor(0xA5D0,0xF8);
	HM5065_write_cmos_sensor(0xA5D1,0xEC);
	HM5065_write_cmos_sensor(0xA5D2,0x64);
	HM5065_write_cmos_sensor(0xA5D3,0x80);
	HM5065_write_cmos_sensor(0xA5D4,0x98);
	HM5065_write_cmos_sensor(0xA5D5,0x40);
	HM5065_write_cmos_sensor(0xA5D6,0x0C);
	HM5065_write_cmos_sensor(0xA5D7,0x90);
	HM5065_write_cmos_sensor(0xA5D8,0x0B);
	HM5065_write_cmos_sensor(0xA5D9,0xC8);
	HM5065_write_cmos_sensor(0xA5DA,0xE0);
	HM5065_write_cmos_sensor(0xA5DB,0x04);
	HM5065_write_cmos_sensor(0xA5DC,0xF0);
	HM5065_write_cmos_sensor(0xA5DD,0xA3);
	HM5065_write_cmos_sensor(0xA5DE,0xE0);
	HM5065_write_cmos_sensor(0xA5DF,0x04);
	HM5065_write_cmos_sensor(0xA5E0,0xF0);
	HM5065_write_cmos_sensor(0xA5E1,0x80);
	HM5065_write_cmos_sensor(0xA5E2,0x26);
	HM5065_write_cmos_sensor(0xA5E3,0x90);
	HM5065_write_cmos_sensor(0xA5E4,0x0D);
	HM5065_write_cmos_sensor(0xA5E5,0xF6);
	HM5065_write_cmos_sensor(0xA5E6,0xE0);
	HM5065_write_cmos_sensor(0xA5E7,0xFE);
	HM5065_write_cmos_sensor(0xA5E8,0xA3);
	HM5065_write_cmos_sensor(0xA5E9,0xE0);
	HM5065_write_cmos_sensor(0xA5EA,0xFF);
	HM5065_write_cmos_sensor(0xA5EB,0xC3);
	HM5065_write_cmos_sensor(0xA5EC,0xE4);
	HM5065_write_cmos_sensor(0xA5ED,0x9F);
	HM5065_write_cmos_sensor(0xA5EE,0xFF);
	HM5065_write_cmos_sensor(0xA5EF,0xE4);
	HM5065_write_cmos_sensor(0xA5F0,0x9E);
	HM5065_write_cmos_sensor(0xA5F1,0xFE);
	HM5065_write_cmos_sensor(0xA5F2,0xC3);
	HM5065_write_cmos_sensor(0xA5F3,0xED);
	HM5065_write_cmos_sensor(0xA5F4,0x9F);
	HM5065_write_cmos_sensor(0xA5F5,0xEE);
	HM5065_write_cmos_sensor(0xA5F6,0x64);
	HM5065_write_cmos_sensor(0xA5F7,0x80);
	HM5065_write_cmos_sensor(0xA5F8,0xF8);
	HM5065_write_cmos_sensor(0xA5F9,0xEC);
	HM5065_write_cmos_sensor(0xA5FA,0x64);
	HM5065_write_cmos_sensor(0xA5FB,0x80);
	HM5065_write_cmos_sensor(0xA5FC,0x98);
	HM5065_write_cmos_sensor(0xA5FD,0x50);
	HM5065_write_cmos_sensor(0xA5FE,0x0A);
	HM5065_write_cmos_sensor(0xA5FF,0x90);
	HM5065_write_cmos_sensor(0xA600,0x0B);
	HM5065_write_cmos_sensor(0xA601,0xC8);
	HM5065_write_cmos_sensor(0xA602,0xE0);
	HM5065_write_cmos_sensor(0xA603,0x14);
	HM5065_write_cmos_sensor(0xA604,0xF0);
	HM5065_write_cmos_sensor(0xA605,0xA3);
	HM5065_write_cmos_sensor(0xA606,0xE0);
	HM5065_write_cmos_sensor(0xA607,0x04);
	HM5065_write_cmos_sensor(0xA608,0xF0);
	HM5065_write_cmos_sensor(0xA609,0x05);
	HM5065_write_cmos_sensor(0xA60A,0x3B);
	HM5065_write_cmos_sensor(0xA60B,0x02);
	HM5065_write_cmos_sensor(0xA60C,0xF5);
	HM5065_write_cmos_sensor(0xA60D,0x02);
	HM5065_write_cmos_sensor(0xA60E,0x90);
	HM5065_write_cmos_sensor(0xA60F,0x08);
	HM5065_write_cmos_sensor(0xA610,0x58);
	HM5065_write_cmos_sensor(0xA611,0x02);
	HM5065_write_cmos_sensor(0xA612,0x9D);
	HM5065_write_cmos_sensor(0xA613,0x50);
	HM5065_write_cmos_sensor(0x9006,0xBA);
	HM5065_write_cmos_sensor(0x9007,0x75);
	HM5065_write_cmos_sensor(0x9008,0x00);
	HM5065_write_cmos_sensor(0x9009,0x00);
	HM5065_write_cmos_sensor(0x900A,0x02);
	HM5065_write_cmos_sensor(0x900D,0x01);
	HM5065_write_cmos_sensor(0x900E,0xA2);
	HM5065_write_cmos_sensor(0x900F,0x8F);
	HM5065_write_cmos_sensor(0x9010,0x00);
	HM5065_write_cmos_sensor(0x9011,0xCB);
	HM5065_write_cmos_sensor(0x9012,0x03);
	HM5065_write_cmos_sensor(0x9016,0xE6);
	HM5065_write_cmos_sensor(0x9017,0x6B);
	HM5065_write_cmos_sensor(0x9018,0x02);
	HM5065_write_cmos_sensor(0x9019,0x6B);
	HM5065_write_cmos_sensor(0x901A,0x02);
	HM5065_write_cmos_sensor(0x901D,0x01);
	HM5065_write_cmos_sensor(0x901E,0xAC);
	HM5065_write_cmos_sensor(0x901F,0x70);
	HM5065_write_cmos_sensor(0x9020,0x00);
	HM5065_write_cmos_sensor(0x9021,0xC5);
	HM5065_write_cmos_sensor(0x9022,0x03);
	HM5065_write_cmos_sensor(0x9026,0x9C);
	HM5065_write_cmos_sensor(0x9027,0x5B);
	HM5065_write_cmos_sensor(0x9028,0x00);
	HM5065_write_cmos_sensor(0x9029,0xBF);
	HM5065_write_cmos_sensor(0x902A,0x02);
	HM5065_write_cmos_sensor(0x902E,0x60);
	HM5065_write_cmos_sensor(0x902F,0x1C);
	HM5065_write_cmos_sensor(0x9030,0x01);
	HM5065_write_cmos_sensor(0x9031,0x37);
	HM5065_write_cmos_sensor(0x9032,0x02);
	HM5065_write_cmos_sensor(0x9035,0x01);
	HM5065_write_cmos_sensor(0x9036,0xBA);
	HM5065_write_cmos_sensor(0x9037,0x70);
	HM5065_write_cmos_sensor(0x9038,0x00);
	HM5065_write_cmos_sensor(0x9039,0x00);
	HM5065_write_cmos_sensor(0x903A,0x03);
	HM5065_write_cmos_sensor(0x903E,0x21);
	HM5065_write_cmos_sensor(0x903F,0x3F);
	HM5065_write_cmos_sensor(0x9040,0x02);
	HM5065_write_cmos_sensor(0x9041,0x40);
	HM5065_write_cmos_sensor(0x9042,0x02);
	HM5065_write_cmos_sensor(0x9046,0x21);
	HM5065_write_cmos_sensor(0x9047,0xEA);
	HM5065_write_cmos_sensor(0x9048,0x02);
	HM5065_write_cmos_sensor(0x9049,0x43);
	HM5065_write_cmos_sensor(0x904A,0x02);
	HM5065_write_cmos_sensor(0x904E,0xA6);
	HM5065_write_cmos_sensor(0x904F,0x12);
	HM5065_write_cmos_sensor(0x9050,0x02);
	HM5065_write_cmos_sensor(0x9051,0x46);
	HM5065_write_cmos_sensor(0x9052,0x02);
	HM5065_write_cmos_sensor(0x9056,0x29);
	HM5065_write_cmos_sensor(0x9057,0xE3);
	HM5065_write_cmos_sensor(0x9058,0x02);
	HM5065_write_cmos_sensor(0x9059,0x49);
	HM5065_write_cmos_sensor(0x905A,0x02);
	HM5065_write_cmos_sensor(0x905D,0x01);
	HM5065_write_cmos_sensor(0x905E,0x9C);
	HM5065_write_cmos_sensor(0x905F,0x6E);
	HM5065_write_cmos_sensor(0x9060,0x05);
	HM5065_write_cmos_sensor(0x9061,0x00);
	HM5065_write_cmos_sensor(0x9062,0x02);
	HM5065_write_cmos_sensor(0x9065,0x01);
	HM5065_write_cmos_sensor(0x9066,0xA2);
	HM5065_write_cmos_sensor(0x9067,0x66);
	HM5065_write_cmos_sensor(0x9068,0x02);
	HM5065_write_cmos_sensor(0x9069,0x35);
	HM5065_write_cmos_sensor(0x906A,0x02);
	HM5065_write_cmos_sensor(0x906D,0x01);
	HM5065_write_cmos_sensor(0x906E,0xB5);
	HM5065_write_cmos_sensor(0x906F,0xC2);
	HM5065_write_cmos_sensor(0x9070,0x02);
	HM5065_write_cmos_sensor(0x9071,0x9B);
	HM5065_write_cmos_sensor(0x9072,0x02);
	HM5065_write_cmos_sensor(0x9075,0x01);
	HM5065_write_cmos_sensor(0x9076,0xA2);
	HM5065_write_cmos_sensor(0x9077,0xD4);
	HM5065_write_cmos_sensor(0x9078,0x02);
	HM5065_write_cmos_sensor(0x9079,0xBE);
	HM5065_write_cmos_sensor(0x907A,0x02);
	HM5065_write_cmos_sensor(0x907D,0x01);
	HM5065_write_cmos_sensor(0x907E,0xB7);
	HM5065_write_cmos_sensor(0x907F,0xEA);
	HM5065_write_cmos_sensor(0x9080,0x00);
	HM5065_write_cmos_sensor(0x9081,0x02);
	HM5065_write_cmos_sensor(0x9082,0x03);
	HM5065_write_cmos_sensor(0x9086,0x67);
	HM5065_write_cmos_sensor(0x9087,0x31);
	HM5065_write_cmos_sensor(0x9088,0x02);
	HM5065_write_cmos_sensor(0x9089,0xF7);
	HM5065_write_cmos_sensor(0x908A,0x02);
	HM5065_write_cmos_sensor(0x908E,0x66);
	HM5065_write_cmos_sensor(0x908F,0xED);
	HM5065_write_cmos_sensor(0x9090,0x03);
	HM5065_write_cmos_sensor(0x9091,0x1D);
	HM5065_write_cmos_sensor(0x9092,0x02);
	HM5065_write_cmos_sensor(0x9096,0x67);
	HM5065_write_cmos_sensor(0x9097,0x73);
	HM5065_write_cmos_sensor(0x9098,0x03);
	HM5065_write_cmos_sensor(0x9099,0xD3);
	HM5065_write_cmos_sensor(0x909A,0x02);
	HM5065_write_cmos_sensor(0x909E,0x20);
	HM5065_write_cmos_sensor(0x909F,0x40);
	HM5065_write_cmos_sensor(0x90A0,0x03);
	HM5065_write_cmos_sensor(0x90A1,0x3B);
	HM5065_write_cmos_sensor(0x90A2,0x02);
	HM5065_write_cmos_sensor(0x90A6,0xC5);
	HM5065_write_cmos_sensor(0x90A7,0xC0);
	HM5065_write_cmos_sensor(0x90A8,0x03);
	HM5065_write_cmos_sensor(0x90A9,0xF0);
	HM5065_write_cmos_sensor(0x90AA,0x02);
	HM5065_write_cmos_sensor(0x90AE,0x41);
	HM5065_write_cmos_sensor(0x90AF,0xB3);
	HM5065_write_cmos_sensor(0x90B0,0x00);
	HM5065_write_cmos_sensor(0x90B1,0xA2);
	HM5065_write_cmos_sensor(0x90B2,0x02);
	HM5065_write_cmos_sensor(0x90B6,0x44);
	HM5065_write_cmos_sensor(0x90B7,0xBA);
	HM5065_write_cmos_sensor(0x90B8,0x00);
	HM5065_write_cmos_sensor(0x90B9,0xF0);
	HM5065_write_cmos_sensor(0x90BA,0x03);
	HM5065_write_cmos_sensor(0x90BE,0x89);
	HM5065_write_cmos_sensor(0x90BF,0x99);
	HM5065_write_cmos_sensor(0x90C0,0x04);
	HM5065_write_cmos_sensor(0x90C1,0x00);
	HM5065_write_cmos_sensor(0x90C2,0x02);
	HM5065_write_cmos_sensor(0x90C6,0xA7);
	HM5065_write_cmos_sensor(0x90C7,0x91);
	HM5065_write_cmos_sensor(0x90C8,0x04);
	HM5065_write_cmos_sensor(0x90C9,0x21);
	HM5065_write_cmos_sensor(0x90CA,0x02);
	HM5065_write_cmos_sensor(0x90CE,0x3A);
	HM5065_write_cmos_sensor(0x90CF,0x51);
	HM5065_write_cmos_sensor(0x90D0,0x00);
	HM5065_write_cmos_sensor(0x90D1,0xA2);
	HM5065_write_cmos_sensor(0x90D2,0x02);
	HM5065_write_cmos_sensor(0x90D6,0x86);
	HM5065_write_cmos_sensor(0x90D7,0x54);
	HM5065_write_cmos_sensor(0x90D8,0x04);
	HM5065_write_cmos_sensor(0x90D9,0x6A); //20130115 derek 47->6A
	HM5065_write_cmos_sensor(0x90DA,0x02);
	HM5065_write_cmos_sensor(0x9000,0x01);
	HM5065_write_cmos_sensor(0xffff,0x00);
	mdelay(200);
	HM5065_write_cmos_sensor(0x0009,0x17);//MCLK=26Mhz ,
	HM5065_write_cmos_sensor(0x0012,0x00);
	HM5065_write_cmos_sensor(0x0013,0x00);
	HM5065_write_cmos_sensor(0x0016,0x00);
	HM5065_write_cmos_sensor(0x0021,0x00);
	HM5065_write_cmos_sensor(0x0022,0x01);
	HM5065_write_cmos_sensor(0x0040,0x01);
	HM5065_write_cmos_sensor(0x0041,0x0a);
	HM5065_write_cmos_sensor(0x0042,0x05);
	HM5065_write_cmos_sensor(0x0043,0x00);
	HM5065_write_cmos_sensor(0x0044,0x03);
	HM5065_write_cmos_sensor(0x0045,0xC0);
	HM5065_write_cmos_sensor(0x0046,0x02);
	HM5065_write_cmos_sensor(0x0060,0x00);
	HM5065_write_cmos_sensor(0x0061,0x00);
	HM5065_write_cmos_sensor(0x0066,0x02);
	HM5065_write_cmos_sensor(0x0083,0x01);
	HM5065_write_cmos_sensor(0x0084,0x01);
	HM5065_write_cmos_sensor(0x0085,0x02);
	HM5065_write_cmos_sensor(0x00B2,0x50);
	HM5065_write_cmos_sensor(0x00B3,0x80); //0x80 20130123 ca->80
	HM5065_write_cmos_sensor(0x00B4,0x01);
	HM5065_write_cmos_sensor(0x00B5,0x01);
	HM5065_write_cmos_sensor(0x00E8,0x01);
	HM5065_write_cmos_sensor(0x00ED,0x05);
	HM5065_write_cmos_sensor(0x00EE,0x1E);
	HM5065_write_cmos_sensor(0x0129,0x00);
	HM5065_write_cmos_sensor(0x0130,0x01);//00
	HM5065_write_cmos_sensor(0x019C,0x4B);//50hz
	HM5065_write_cmos_sensor(0x019D,0x20);
	HM5065_write_cmos_sensor(0x01A0,0x01);
	HM5065_write_cmos_sensor(0x01A1,0x80);
	HM5065_write_cmos_sensor(0x01A2,0x80);
	HM5065_write_cmos_sensor(0x01A3,0x80);
	HM5065_write_cmos_sensor(0x5200,0x01);
	HM5065_write_cmos_sensor(0x7000,0x0C);
	HM5065_write_cmos_sensor(0x7101,0xC4);
	HM5065_write_cmos_sensor(0x7102,0x01);
	HM5065_write_cmos_sensor(0x7103,0x00);
	HM5065_write_cmos_sensor(0x7104,0x00);
	HM5065_write_cmos_sensor(0x7105,0x80);
	HM5065_write_cmos_sensor(0x7158,0x00);
	HM5065_write_cmos_sensor(0x0143,0x5F);
	HM5065_write_cmos_sensor(0x0144,0x0D);
	HM5065_write_cmos_sensor(0x02C2,0x00);
	HM5065_write_cmos_sensor(0x02C3,0xE0);	//c0:4x gain E0:8x	
	HM5065_write_cmos_sensor(0x015E,0x40);
	HM5065_write_cmos_sensor(0x015F,0x00);
	HM5065_write_cmos_sensor(0x0390,0x01);
	HM5065_write_cmos_sensor(0x0391,0x00);
	HM5065_write_cmos_sensor(0x0392,0x00);
	HM5065_write_cmos_sensor(0x03A0,0x14);
	HM5065_write_cmos_sensor(0x03A1,0x00);
	HM5065_write_cmos_sensor(0x03A2,0x5A);
	HM5065_write_cmos_sensor(0x03A3,0xEE);
	HM5065_write_cmos_sensor(0x03A4,0x69);
	HM5065_write_cmos_sensor(0x03A5,0x49);
	HM5065_write_cmos_sensor(0x03A6,0x3E);
	HM5065_write_cmos_sensor(0x03A7,0x00);
	HM5065_write_cmos_sensor(0x03A8,0x39);
	HM5065_write_cmos_sensor(0x03A9,0x33);
	HM5065_write_cmos_sensor(0x03B0,0x60);
	HM5065_write_cmos_sensor(0x03B1,0x00);
	HM5065_write_cmos_sensor(0x03B2,0x5A);
	HM5065_write_cmos_sensor(0x03B3,0xEE);
	HM5065_write_cmos_sensor(0x03B4,0x69);
	HM5065_write_cmos_sensor(0x03B5,0x49);
	HM5065_write_cmos_sensor(0x03B6,0x3E);
	HM5065_write_cmos_sensor(0x03B7,0x00);
	HM5065_write_cmos_sensor(0x03B8,0x3D);
	HM5065_write_cmos_sensor(0x03B9,0x20);
	HM5065_write_cmos_sensor(0x03C0,0x10);
	HM5065_write_cmos_sensor(0x03C1,0x00);
	HM5065_write_cmos_sensor(0x03C2,0x5A);
	HM5065_write_cmos_sensor(0x03C3,0xEE);
	HM5065_write_cmos_sensor(0x03C4,0x69);
	HM5065_write_cmos_sensor(0x03C5,0x49);
	HM5065_write_cmos_sensor(0x03C6,0x3A);
	HM5065_write_cmos_sensor(0x03C7,0x80);
	HM5065_write_cmos_sensor(0x03D0,0x64);
	HM5065_write_cmos_sensor(0x03D1,0x00);
	HM5065_write_cmos_sensor(0x03D2,0x5A);
	HM5065_write_cmos_sensor(0x03D3,0xEE);
	HM5065_write_cmos_sensor(0x03D4,0x69);
	HM5065_write_cmos_sensor(0x03D5,0x49);
	HM5065_write_cmos_sensor(0x03D6,0x34);
	HM5065_write_cmos_sensor(0x03D7,0xD1);
	HM5065_write_cmos_sensor(0x004C,0x08);
	HM5065_write_cmos_sensor(0x006C,0x08);
	HM5065_write_cmos_sensor(0x0350,0x00);
	HM5065_write_cmos_sensor(0x0351,0x5A);
	HM5065_write_cmos_sensor(0x0352,0xEE);
	HM5065_write_cmos_sensor(0x0353,0x69);
	HM5065_write_cmos_sensor(0x0354,0x49);
	HM5065_write_cmos_sensor(0x0355,0x39);
	HM5065_write_cmos_sensor(0x0356,0x6D);
	HM5065_write_cmos_sensor(0x0357,0x19);
	HM5065_write_cmos_sensor(0x0358,0x00);
	HM5065_write_cmos_sensor(0x0359,0x3C);
	HM5065_write_cmos_sensor(0x035A,0x5A);
	HM5065_write_cmos_sensor(0x035B,0xEE);
	HM5065_write_cmos_sensor(0x035C,0x69);
	HM5065_write_cmos_sensor(0x035D,0x49);
	HM5065_write_cmos_sensor(0x035E,0x39);
	HM5065_write_cmos_sensor(0x035F,0x85);
	HM5065_write_cmos_sensor(0x0049,0x14);
	HM5065_write_cmos_sensor(0x004A,0x0D);
	HM5065_write_cmos_sensor(0x0069,0x14);
	HM5065_write_cmos_sensor(0x006A,0x0D);
	HM5065_write_cmos_sensor(0x0090,0x5A);
	HM5065_write_cmos_sensor(0x0091,0xEE);
	HM5065_write_cmos_sensor(0x0092,0x3E);
	HM5065_write_cmos_sensor(0x0093,0x00);
	HM5065_write_cmos_sensor(0x0094,0x69);
	HM5065_write_cmos_sensor(0x0095,0x49);
	HM5065_write_cmos_sensor(0x0096,0x39);
	HM5065_write_cmos_sensor(0x0097,0xCF);
	HM5065_write_cmos_sensor(0x0098,0x00);//20130118 00->01->00
	HM5065_write_cmos_sensor(0x00A0,0x5A);
	HM5065_write_cmos_sensor(0x00A1,0xEE);
	HM5065_write_cmos_sensor(0x00A2,0x3E);
	HM5065_write_cmos_sensor(0x00A3,0x00);
	HM5065_write_cmos_sensor(0x00A4,0x69);
	HM5065_write_cmos_sensor(0x00A5,0x49);
	HM5065_write_cmos_sensor(0x00A6,0x3B);
	HM5065_write_cmos_sensor(0x00A7,0x80);
	HM5065_write_cmos_sensor(0x00A8,0x00);//20130118 00->01->00

	HM5065_write_cmos_sensor(0x0320,0x01);
	HM5065_write_cmos_sensor(0x0321,0x04);
	HM5065_write_cmos_sensor(0x0322,0x01);
	HM5065_write_cmos_sensor(0x0323,0x01);
	HM5065_write_cmos_sensor(0x0330,0x00);//20130117 01->00
	HM5065_write_cmos_sensor(0x0335,0x3C);//20130117
	HM5065_write_cmos_sensor(0x0336,0x00);//20130117	
	HM5065_write_cmos_sensor(0x0384,0x00);
	HM5065_write_cmos_sensor(0x0337,0x01);
	
	//20130116 Brandon update CCM
	HM5065_write_cmos_sensor(0x03EC,0x39);//39
	HM5065_write_cmos_sensor(0x03ED,0x86);//85
	HM5065_write_cmos_sensor(0x03FC,0x3A);//3A
	HM5065_write_cmos_sensor(0x03FD,0x29);//14
	HM5065_write_cmos_sensor(0x040C,0x3A);//3A
	HM5065_write_cmos_sensor(0x040D,0xA4);//F6
	HM5065_write_cmos_sensor(0x041C,0x3B);//3B
	HM5065_write_cmos_sensor(0x041D,0x5D);//9A
	HM5065_write_cmos_sensor(0x03E0,0xB6);//B6
	HM5065_write_cmos_sensor(0x03E1,0x04);//04
	HM5065_write_cmos_sensor(0x03E2,0xBB);//BB
	HM5065_write_cmos_sensor(0x03E3,0xE9);//E9
	HM5065_write_cmos_sensor(0x03E4,0xBC);//BC
	HM5065_write_cmos_sensor(0x03E5,0x70);//70
	HM5065_write_cmos_sensor(0x03E6,0x39);//37
	HM5065_write_cmos_sensor(0x03E7,0x95);//02
	HM5065_write_cmos_sensor(0x03E8,0xBC);//BC
	HM5065_write_cmos_sensor(0x03E9,0x00);//00
	HM5065_write_cmos_sensor(0x03EA,0xBF);//BF
	HM5065_write_cmos_sensor(0x03EB,0x12);//12
	HM5065_write_cmos_sensor(0x03F0,0xBA);//BA
	HM5065_write_cmos_sensor(0x03F1,0x7B);//7B
	HM5065_write_cmos_sensor(0x03F2,0xBA);//BA
	HM5065_write_cmos_sensor(0x03F3,0x83);//83
	HM5065_write_cmos_sensor(0x03F4,0xBB);//BB
	HM5065_write_cmos_sensor(0x03F5,0xBC);//BC
	HM5065_write_cmos_sensor(0x03F6,0x39);//38
	HM5065_write_cmos_sensor(0x03F7,0x95);//2D
	HM5065_write_cmos_sensor(0x03F8,0xBB);//BB
	HM5065_write_cmos_sensor(0x03F9,0x23);//23
	HM5065_write_cmos_sensor(0x03FA,0xBD);//BD
	HM5065_write_cmos_sensor(0x03FB,0xAC);//AC
	HM5065_write_cmos_sensor(0x0400,0xBD);//BE
	HM5065_write_cmos_sensor(0x0401,0xF9);//96
	HM5065_write_cmos_sensor(0x0402,0xB9);//B9
	HM5065_write_cmos_sensor(0x0403,0xBE);//BE
	HM5065_write_cmos_sensor(0x0404,0xBB);//BB
	HM5065_write_cmos_sensor(0x0405,0x56);//57
	HM5065_write_cmos_sensor(0x0406,0x3A);//3A
	HM5065_write_cmos_sensor(0x0407,0xBA);//BB
	HM5065_write_cmos_sensor(0x0408,0xB3);//B3
	HM5065_write_cmos_sensor(0x0409,0x12);//17
	HM5065_write_cmos_sensor(0x040A,0xBE);//BE
	HM5065_write_cmos_sensor(0x040B,0x66);//66
	HM5065_write_cmos_sensor(0x0410,0xBB);//BB
	HM5065_write_cmos_sensor(0x0411,0x3E);//2A
	HM5065_write_cmos_sensor(0x0412,0xBA);//BA
	HM5065_write_cmos_sensor(0x0413,0x14);//00
	HM5065_write_cmos_sensor(0x0414,0xBA);//BB
	HM5065_write_cmos_sensor(0x0415,0x6C);//10
	HM5065_write_cmos_sensor(0x0416,0xB9);//B8
	HM5065_write_cmos_sensor(0x0417,0x47);//CD
	HM5065_write_cmos_sensor(0x0418,0xB7);//B7
	HM5065_write_cmos_sensor(0x0419,0x5C);//5C
	HM5065_write_cmos_sensor(0x041A,0xBB);//BB
	HM5065_write_cmos_sensor(0x041B,0x6C);//6C
	
	HM5065_write_cmos_sensor(0x01f8,0x3c);
	HM5065_write_cmos_sensor(0x01f9,0x00);
	HM5065_write_cmos_sensor(0x01fa,0x00);
	HM5065_write_cmos_sensor(0x02a2,0x3e);
	HM5065_write_cmos_sensor(0x02a3,0x00);
	HM5065_write_cmos_sensor(0x02a4,0x3e);
	HM5065_write_cmos_sensor(0x02a5,0x00);
	HM5065_write_cmos_sensor(0x02a6,0x3e);
	HM5065_write_cmos_sensor(0x02a7,0x00);
	HM5065_write_cmos_sensor(0x02a8,0x3e);
	HM5065_write_cmos_sensor(0x02a9,0x00);
	HM5065_write_cmos_sensor(0x056c,0x42);
	HM5065_write_cmos_sensor(0x056d,0x00);
	HM5065_write_cmos_sensor(0x056e,0x42);
	HM5065_write_cmos_sensor(0x056f,0x00);
	HM5065_write_cmos_sensor(0x0570,0x42);
	HM5065_write_cmos_sensor(0x0571,0x00);
	HM5065_write_cmos_sensor(0x0572,0x42);
	HM5065_write_cmos_sensor(0x0573,0x00);
	HM5065_write_cmos_sensor(0x0081,0x68); //20130118 6E->58->60->5c->64->60->62->64->66->68
	HM5065_write_cmos_sensor(0x0588,0x00);
	HM5065_write_cmos_sensor(0x0589,0x5A);
	HM5065_write_cmos_sensor(0x058A,0xEE);
	HM5065_write_cmos_sensor(0x058B,0x69);
	HM5065_write_cmos_sensor(0x058C,0x49);
	HM5065_write_cmos_sensor(0x058D,0x3D);
	HM5065_write_cmos_sensor(0x058E,0x3D);
	HM5065_write_cmos_sensor(0x0080,0x6C);
	HM5065_write_cmos_sensor(0x0082,0x5A);
	
	//DEREK
	
		// AF register optimuzation 
	HM5065_write_cmos_sensor(0x0658,0x00); // 2013-01-04
	HM5065_write_cmos_sensor(0x0659,0x01);
	HM5065_write_cmos_sensor(0x075A,0x00);//00 ->01
	HM5065_write_cmos_sensor(0x0756,0x00); //03->00,
	HM5065_write_cmos_sensor(0x06D5,0x01);
	HM5065_write_cmos_sensor(0x06D6,0x60);
	HM5065_write_cmos_sensor(0x06D7,0x00);
	HM5065_write_cmos_sensor(0x06D8,0x93);
	HM5065_write_cmos_sensor(0x06D9,0x00);
	HM5065_write_cmos_sensor(0x06DA,0x93);
	HM5065_write_cmos_sensor(0x0659,0x00);
	HM5065_write_cmos_sensor(0x065A,0x00);
	HM5065_write_cmos_sensor(0x06C9,0x01);
	HM5065_write_cmos_sensor(0x06CD,0x01);
	HM5065_write_cmos_sensor(0x06CE,0xBD);
	HM5065_write_cmos_sensor(0x06CF,0x00);
	HM5065_write_cmos_sensor(0x06D0,0x93);
	HM5065_write_cmos_sensor(0x06D1,0x02);
	HM5065_write_cmos_sensor(0x06D2,0x30);
	HM5065_write_cmos_sensor(0x06D3,0xD4);
	HM5065_write_cmos_sensor(0x06D4,0x01);
	HM5065_write_cmos_sensor(0x06DB,0x59);
	HM5065_write_cmos_sensor(0x06DC,0x0d);
	HM5065_write_cmos_sensor(0x0730,0x00);
	HM5065_write_cmos_sensor(0x0731,0x00);
	HM5065_write_cmos_sensor(0x0732,0x03);
	HM5065_write_cmos_sensor(0x0733,0xFF);
	HM5065_write_cmos_sensor(0x0734,0x03);
	HM5065_write_cmos_sensor(0x0735,0x70);
	HM5065_write_cmos_sensor(0x0755,0x01);		
	HM5065_write_cmos_sensor(0x075B,0x01);
	HM5065_write_cmos_sensor(0x075E,0x00);
	HM5065_write_cmos_sensor(0x0764,0x01);
	HM5065_write_cmos_sensor(0x0766,0x01);
	HM5065_write_cmos_sensor(0x0768,0x01);
	HM5065_write_cmos_sensor(0x076A,0x00);
	HM5065_write_cmos_sensor(0x0758,0x01);
	HM5065_write_cmos_sensor(0x075C,0x01);
	HM5065_write_cmos_sensor(0x0770,0x98); // 98->b5
 	HM5065_write_cmos_sensor(0x0771,0x19);
	HM5065_write_cmos_sensor(0x0772,0x1B);
	HM5065_write_cmos_sensor(0x0774,0x01);
	HM5065_write_cmos_sensor(0x0775,0x4a);
	HM5065_write_cmos_sensor(0x0777,0x00);
	HM5065_write_cmos_sensor(0x0778,0x45);
	HM5065_write_cmos_sensor(0x0779,0x00);
	HM5065_write_cmos_sensor(0x077A,0x02);
	HM5065_write_cmos_sensor(0x077D,0x01);
	HM5065_write_cmos_sensor(0x077E,0x03);
	HM5065_write_cmos_sensor(0x0783,0x10);
	HM5065_write_cmos_sensor(0x0785,0x14);
	HM5065_write_cmos_sensor(0x0788,0x04);
	HM5065_write_cmos_sensor(0x0846,0x06);
	HM5065_write_cmos_sensor(0x0847,0x05);
	//END
	
	HM5065_write_cmos_sensor(0xC41A,0x05);
	HM5065_write_cmos_sensor(0xC423,0x11);
	HM5065_write_cmos_sensor(0xC427,0x11);
	HM5065_write_cmos_sensor(0x300B,0x09);
	HM5065_write_cmos_sensor(0x4708,0x00);
	HM5065_write_cmos_sensor(0x4709,0x00);
	HM5065_write_cmos_sensor(0x4710,0x00);
	HM5065_write_cmos_sensor(0x4711,0x00);
	HM5065_write_cmos_sensor(0x0009, 0x17);//MCLK=26Mhz ,
	HM5065_write_cmos_sensor(0x0085, 0x00);//Reg0x0085=0x00(CbYCrY - sequence)for MTK
	//20121101 LSC
	HM5065_write_cmos_sensor(0x04b0,0xff);  
	HM5065_write_cmos_sensor(0x04b1,0x6a); //bc 
	HM5065_write_cmos_sensor(0x04b5,0x85);  //90->60->7a->85
	HM5065_write_cmos_sensor(0x04b7,0x90);  //c7->86->a0->90
	HM5065_write_cmos_sensor(0x04af,0x48);  //65->48
	HM5065_write_cmos_sensor(0x04cf,0x4b);//63->4b  
	HM5065_write_cmos_sensor(0x04b8,0x00);  //ff->00
	HM5065_write_cmos_sensor(0x04b9,0xa0);  //fb->a0

   //20121120 LSC
  HM5065_write_cmos_sensor(0x0420,0x00);
	HM5065_write_cmos_sensor(0x0421,0x1f);
	HM5065_write_cmos_sensor(0x0422,0xff);
	HM5065_write_cmos_sensor(0x0423,0x86);
	HM5065_write_cmos_sensor(0x0424,0x00);
	HM5065_write_cmos_sensor(0x0425,0x6b);
	HM5065_write_cmos_sensor(0x0426,0x00);
	HM5065_write_cmos_sensor(0x0427,0x86);
	HM5065_write_cmos_sensor(0x0428,0x00);
	HM5065_write_cmos_sensor(0x0429,0x3c);
	HM5065_write_cmos_sensor(0x042a,0xfe);
	HM5065_write_cmos_sensor(0x042b,0x77);
	HM5065_write_cmos_sensor(0x042c,0x00);
	HM5065_write_cmos_sensor(0x042d,0x49);
	HM5065_write_cmos_sensor(0x042e,0xff);
	HM5065_write_cmos_sensor(0x042f,0x8c);
	HM5065_write_cmos_sensor(0x0430,0xff);
	HM5065_write_cmos_sensor(0x0431,0xd7);
	HM5065_write_cmos_sensor(0x0432,0xfe);
	HM5065_write_cmos_sensor(0x0433,0xa4);
	HM5065_write_cmos_sensor(0x0434,0x00);
	HM5065_write_cmos_sensor(0x0435,0xa2); //20130117//ba->9a->a2
	HM5065_write_cmos_sensor(0x0436,0x00);
	HM5065_write_cmos_sensor(0x0437,0xc5); //20130117//dd->bd->c5
	HM5065_write_cmos_sensor(0x0438,0x00);
	HM5065_write_cmos_sensor(0x0439,0x0d);
	HM5065_write_cmos_sensor(0x043a,0xfe);
	HM5065_write_cmos_sensor(0x043b,0x9b);
	HM5065_write_cmos_sensor(0x043c,0x00);
	HM5065_write_cmos_sensor(0x043d,0xa2);
	HM5065_write_cmos_sensor(0x043e,0xff);
	HM5065_write_cmos_sensor(0x043f,0x3c);
	HM5065_write_cmos_sensor(0x0450,0x00);
	HM5065_write_cmos_sensor(0x0451,0x20);
	HM5065_write_cmos_sensor(0x0452,0xfe);
	HM5065_write_cmos_sensor(0x0453,0xe7);
	HM5065_write_cmos_sensor(0x0454,0x00);
	HM5065_write_cmos_sensor(0x0455,0x62); //20130117//52->62
	HM5065_write_cmos_sensor(0x0456,0x00);
	HM5065_write_cmos_sensor(0x0457,0x76); //20130117//66->76
	HM5065_write_cmos_sensor(0x0458,0xff);
	HM5065_write_cmos_sensor(0x0459,0xca);
	HM5065_write_cmos_sensor(0x045a,0x00);
	HM5065_write_cmos_sensor(0x045b,0x0e);
	HM5065_write_cmos_sensor(0x045c,0xff);
	HM5065_write_cmos_sensor(0x045d,0x56);
	HM5065_write_cmos_sensor(0x045e,0xff);
	HM5065_write_cmos_sensor(0x045f,0xe5);
	HM5065_write_cmos_sensor(0x0440,0x00);
	HM5065_write_cmos_sensor(0x0441,0x20);
	HM5065_write_cmos_sensor(0x0442,0xff);
	HM5065_write_cmos_sensor(0x0443,0x88);
	HM5065_write_cmos_sensor(0x0444,0x00);
	HM5065_write_cmos_sensor(0x0445,0x6b);
	HM5065_write_cmos_sensor(0x0446,0x00);
	HM5065_write_cmos_sensor(0x0447,0x87);
	HM5065_write_cmos_sensor(0x0448,0x00);
	HM5065_write_cmos_sensor(0x0449,0x3c);
	HM5065_write_cmos_sensor(0x044a,0xfe);
	HM5065_write_cmos_sensor(0x044b,0x78);
	HM5065_write_cmos_sensor(0x044c,0x00);
	HM5065_write_cmos_sensor(0x044d,0x52);
	HM5065_write_cmos_sensor(0x044e,0xff);
	HM5065_write_cmos_sensor(0x044f,0x8c);
	HM5065_write_cmos_sensor(0x0460,0x00);
	HM5065_write_cmos_sensor(0x0461,0x27);
	HM5065_write_cmos_sensor(0x0462,0xff);
	HM5065_write_cmos_sensor(0x0463,0xaa);
	HM5065_write_cmos_sensor(0x0464,0x00);
	HM5065_write_cmos_sensor(0x0465,0x65);
	HM5065_write_cmos_sensor(0x0466,0x00);
	HM5065_write_cmos_sensor(0x0467,0x7f);
	HM5065_write_cmos_sensor(0x0468,0x00);
	HM5065_write_cmos_sensor(0x0469,0x45);
	HM5065_write_cmos_sensor(0x046a,0xfe);
	HM5065_write_cmos_sensor(0x046b,0x21);
	HM5065_write_cmos_sensor(0x046c,0x00);
	HM5065_write_cmos_sensor(0x046d,0x58);
	HM5065_write_cmos_sensor(0x046e,0xff);
	HM5065_write_cmos_sensor(0x046f,0x9b);
	HM5065_write_cmos_sensor(0x0470,0xff);
	HM5065_write_cmos_sensor(0x0471,0xf2);
	HM5065_write_cmos_sensor(0x0472,0xfe);
	HM5065_write_cmos_sensor(0x0473,0xf1);
	HM5065_write_cmos_sensor(0x0474,0x00);
	HM5065_write_cmos_sensor(0x0475,0x65); //85
	HM5065_write_cmos_sensor(0x0476,0x00);
	HM5065_write_cmos_sensor(0x0477,0x85); //a5
	HM5065_write_cmos_sensor(0x0478,0x00);
	HM5065_write_cmos_sensor(0x0479,0x10);
	HM5065_write_cmos_sensor(0x047a,0xfe);
	HM5065_write_cmos_sensor(0x047b,0xb0);
	HM5065_write_cmos_sensor(0x047c,0x00);
	HM5065_write_cmos_sensor(0x047d,0x26);
	HM5065_write_cmos_sensor(0x047e,0xff);
	HM5065_write_cmos_sensor(0x047f,0x8f);
	HM5065_write_cmos_sensor(0x0490,0x00);
	HM5065_write_cmos_sensor(0x0491,0x21);
	HM5065_write_cmos_sensor(0x0492,0xff);
	HM5065_write_cmos_sensor(0x0493,0x1c);
	HM5065_write_cmos_sensor(0x0494,0x00);
	HM5065_write_cmos_sensor(0x0495,0x4b);
	HM5065_write_cmos_sensor(0x0496,0x00);
	HM5065_write_cmos_sensor(0x0497,0x5c);
	HM5065_write_cmos_sensor(0x0498,0xff);
	HM5065_write_cmos_sensor(0x0499,0xd4);
	HM5065_write_cmos_sensor(0x049a,0xff);
	HM5065_write_cmos_sensor(0x049b,0x5d);
	HM5065_write_cmos_sensor(0x049c,0xff);
	HM5065_write_cmos_sensor(0x049d,0x88);
	HM5065_write_cmos_sensor(0x049e,0xff);
	HM5065_write_cmos_sensor(0x049f,0xe4);
	HM5065_write_cmos_sensor(0x0480,0x00);
	HM5065_write_cmos_sensor(0x0481,0x28);
	HM5065_write_cmos_sensor(0x0482,0xff);
	HM5065_write_cmos_sensor(0x0483,0xac);
	HM5065_write_cmos_sensor(0x0484,0x00);
	HM5065_write_cmos_sensor(0x0485,0x65);
	HM5065_write_cmos_sensor(0x0486,0x00);
	HM5065_write_cmos_sensor(0x0487,0x7f);
	HM5065_write_cmos_sensor(0x0488,0x00);
	HM5065_write_cmos_sensor(0x0489,0x44);
	HM5065_write_cmos_sensor(0x048a,0xfe);
	HM5065_write_cmos_sensor(0x048b,0x22);
	HM5065_write_cmos_sensor(0x048c,0x00);
	HM5065_write_cmos_sensor(0x048d,0x55);
	HM5065_write_cmos_sensor(0x048e,0xff);
	HM5065_write_cmos_sensor(0x048f,0x9a);
	HM5065_write_cmos_sensor(0x04a0,0x00);
	HM5065_write_cmos_sensor(0x04a1,0x06);
	HM5065_write_cmos_sensor(0x04a2,0xff);
	HM5065_write_cmos_sensor(0x04a3,0x87);
	HM5065_write_cmos_sensor(0x04a4,0x00);
	HM5065_write_cmos_sensor(0x04a5,0x67);
	HM5065_write_cmos_sensor(0x04a6,0x00);
	HM5065_write_cmos_sensor(0x04a7,0x81);
	HM5065_write_cmos_sensor(0x04a8,0x00);
	HM5065_write_cmos_sensor(0x04a9,0x39);
	HM5065_write_cmos_sensor(0x04aa,0xfe);
	HM5065_write_cmos_sensor(0x04ab,0x72);
	HM5065_write_cmos_sensor(0x04ac,0x00);
	HM5065_write_cmos_sensor(0x04ad,0x3d);
	HM5065_write_cmos_sensor(0x04ae,0xff);
	HM5065_write_cmos_sensor(0x04af,0xb7);
	HM5065_write_cmos_sensor(0x04b0,0xff);
	HM5065_write_cmos_sensor(0x04b1,0xd6);
	HM5065_write_cmos_sensor(0x04b2,0xfe);
	HM5065_write_cmos_sensor(0x04b3,0xb4);
	HM5065_write_cmos_sensor(0x04b4,0x00);
	HM5065_write_cmos_sensor(0x04b5,0x9f);
	HM5065_write_cmos_sensor(0x04b6,0x00);
	HM5065_write_cmos_sensor(0x04b7,0xc3);
	HM5065_write_cmos_sensor(0x04b8,0xff);
	HM5065_write_cmos_sensor(0x04b9,0xe2);
	HM5065_write_cmos_sensor(0x04ba,0xfe);
	HM5065_write_cmos_sensor(0x04bb,0xf2);
	HM5065_write_cmos_sensor(0x04bc,0x00);
	HM5065_write_cmos_sensor(0x04bd,0x45);
	HM5065_write_cmos_sensor(0x04be,0xff);
	HM5065_write_cmos_sensor(0x04bf,0x90);
	HM5065_write_cmos_sensor(0x04d0,0xff);
	HM5065_write_cmos_sensor(0x04d1,0xfb);
	HM5065_write_cmos_sensor(0x04d2,0xff);
	HM5065_write_cmos_sensor(0x04d3,0x50);
	HM5065_write_cmos_sensor(0x04d4,0x00);
	HM5065_write_cmos_sensor(0x04d5,0x63); //20130118 53->63
	HM5065_write_cmos_sensor(0x04d6,0x00);
	HM5065_write_cmos_sensor(0x04d7,0x77); //20130118 67->77
	HM5065_write_cmos_sensor(0x04d8,0xff);
	HM5065_write_cmos_sensor(0x04d9,0xe9);
	HM5065_write_cmos_sensor(0x04da,0xfe);
	HM5065_write_cmos_sensor(0x04db,0x9a);
	HM5065_write_cmos_sensor(0x04dc,0x00);
	HM5065_write_cmos_sensor(0x04dd,0x1c);
	HM5065_write_cmos_sensor(0x04de,0xff);
	HM5065_write_cmos_sensor(0x04df,0xcb);
	HM5065_write_cmos_sensor(0x04c0,0x00);
	HM5065_write_cmos_sensor(0x04c1,0x07);
	HM5065_write_cmos_sensor(0x04c2,0xff);
	HM5065_write_cmos_sensor(0x04c3,0x89);
	HM5065_write_cmos_sensor(0x04c4,0x00);
	HM5065_write_cmos_sensor(0x04c5,0x67);
	HM5065_write_cmos_sensor(0x04c6,0x00);
	HM5065_write_cmos_sensor(0x04c7,0x82);
	HM5065_write_cmos_sensor(0x04c8,0x00);
	HM5065_write_cmos_sensor(0x04c9,0x3a);
	HM5065_write_cmos_sensor(0x04ca,0xfe);
	HM5065_write_cmos_sensor(0x04cb,0x72);
	HM5065_write_cmos_sensor(0x04cc,0x00);
	HM5065_write_cmos_sensor(0x04cd,0x46);
	HM5065_write_cmos_sensor(0x04ce,0xff);
	HM5065_write_cmos_sensor(0x04cf,0xb5);
	HM5065_write_cmos_sensor(0x04e0,0x00);
	HM5065_write_cmos_sensor(0x04e1,0x11);
	HM5065_write_cmos_sensor(0x04e2,0xff);
	HM5065_write_cmos_sensor(0x04e3,0x8f);
	HM5065_write_cmos_sensor(0x04e4,0x00);
	HM5065_write_cmos_sensor(0x04e5,0x62);
	HM5065_write_cmos_sensor(0x04e6,0x00);
	HM5065_write_cmos_sensor(0x04e7,0x7d);
	HM5065_write_cmos_sensor(0x04e8,0x00);
	HM5065_write_cmos_sensor(0x04e9,0x56);
	HM5065_write_cmos_sensor(0x04ea,0xfe);
	HM5065_write_cmos_sensor(0x04eb,0x75);
	HM5065_write_cmos_sensor(0x04ec,0x00);
	HM5065_write_cmos_sensor(0x04ed,0x1d);
	HM5065_write_cmos_sensor(0x04ee,0xff);
	HM5065_write_cmos_sensor(0x04ef,0xa0);
	HM5065_write_cmos_sensor(0x04f0,0xff);
	HM5065_write_cmos_sensor(0x04f1,0xea);
	HM5065_write_cmos_sensor(0x04f2,0xfe);
	HM5065_write_cmos_sensor(0x04f3,0xc8);
	HM5065_write_cmos_sensor(0x04f4,0x00);
	HM5065_write_cmos_sensor(0x04f5,0x85);//91
	HM5065_write_cmos_sensor(0x04f6,0x00);
	HM5065_write_cmos_sensor(0x04f7,0xb0);//b5
	HM5065_write_cmos_sensor(0x04f8,0xff);
	HM5065_write_cmos_sensor(0x04f9,0xec);
	HM5065_write_cmos_sensor(0x04fa,0xff);
	HM5065_write_cmos_sensor(0x04fb,0x19);
	HM5065_write_cmos_sensor(0x04fc,0xff);
	HM5065_write_cmos_sensor(0x04fd,0xe7);
	HM5065_write_cmos_sensor(0x04fe,0xff);
	HM5065_write_cmos_sensor(0x04ff,0x76);
	HM5065_write_cmos_sensor(0x0510,0x00);
	HM5065_write_cmos_sensor(0x0511,0x06);
	HM5065_write_cmos_sensor(0x0512,0xff);
	HM5065_write_cmos_sensor(0x0513,0x57);
	HM5065_write_cmos_sensor(0x0514,0x00);
	HM5065_write_cmos_sensor(0x0515,0x5d); //20130118 4d->5d
	HM5065_write_cmos_sensor(0x0516,0x00);
	HM5065_write_cmos_sensor(0x0517,0x71); //20130118 61->71
	HM5065_write_cmos_sensor(0x0518,0x00);
	HM5065_write_cmos_sensor(0x0519,0x01);
	HM5065_write_cmos_sensor(0x051a,0xfe);
	HM5065_write_cmos_sensor(0x051b,0xd6);
	HM5065_write_cmos_sensor(0x051c,0xff);
	HM5065_write_cmos_sensor(0x051d,0xfc);
	HM5065_write_cmos_sensor(0x051e,0xff);
	HM5065_write_cmos_sensor(0x051f,0xc5);
	HM5065_write_cmos_sensor(0x0500,0x00);
	HM5065_write_cmos_sensor(0x0501,0x12);
	HM5065_write_cmos_sensor(0x0502,0xff);
	HM5065_write_cmos_sensor(0x0503,0x90);
	HM5065_write_cmos_sensor(0x0504,0x00);
	HM5065_write_cmos_sensor(0x0505,0x62);
	HM5065_write_cmos_sensor(0x0506,0x00);
	HM5065_write_cmos_sensor(0x0507,0x7d);
	HM5065_write_cmos_sensor(0x0508,0x00);
	HM5065_write_cmos_sensor(0x0509,0x55);
	HM5065_write_cmos_sensor(0x050a,0xfe);
	HM5065_write_cmos_sensor(0x050b,0x7a);
	HM5065_write_cmos_sensor(0x050c,0x00);
	HM5065_write_cmos_sensor(0x050d,0x24);
	HM5065_write_cmos_sensor(0x050e,0xff);
	HM5065_write_cmos_sensor(0x050f,0xa0);
	HM5065_write_cmos_sensor(0x0561,0x00);
	HM5065_write_cmos_sensor(0x0562,0x00);
	HM5065_write_cmos_sensor(0x0563,0x00);
	HM5065_write_cmos_sensor(0x0564,0x00);
	HM5065_write_cmos_sensor(0x0324,0x39);
	HM5065_write_cmos_sensor(0x0325,0x85);
	HM5065_write_cmos_sensor(0x0326,0x3A);
	HM5065_write_cmos_sensor(0x0327,0x66);
	HM5065_write_cmos_sensor(0x0328,0x3A);
	HM5065_write_cmos_sensor(0x0329,0xF6);
	HM5065_write_cmos_sensor(0x032A,0x3B);
	HM5065_write_cmos_sensor(0x032B,0x1F);

/*
//20130115 black phone start
	HM5065_write_cmos_sensor(0x0420,0xFF);
	HM5065_write_cmos_sensor(0x0421,0xC6);
	HM5065_write_cmos_sensor(0x0422,0xFF);
	HM5065_write_cmos_sensor(0x0423,0x8E);
	HM5065_write_cmos_sensor(0x0424,0x00);
	HM5065_write_cmos_sensor(0x0425,0x4B);
	HM5065_write_cmos_sensor(0x0426,0x00);
	HM5065_write_cmos_sensor(0x0427,0x58);
	HM5065_write_cmos_sensor(0x0428,0x00);
	HM5065_write_cmos_sensor(0x0429,0x12);
	HM5065_write_cmos_sensor(0x042A,0x00);
	HM5065_write_cmos_sensor(0x042B,0x1F);
	HM5065_write_cmos_sensor(0x042C,0xFF);
	HM5065_write_cmos_sensor(0x042D,0xD1);
	HM5065_write_cmos_sensor(0x042E,0xFF);
	HM5065_write_cmos_sensor(0x042F,0xC1);
	HM5065_write_cmos_sensor(0x0430,0xFF);
	HM5065_write_cmos_sensor(0x0431,0xB5);
	HM5065_write_cmos_sensor(0x0432,0xFF);
	HM5065_write_cmos_sensor(0x0433,0x16);
	HM5065_write_cmos_sensor(0x0434,0x00);
	HM5065_write_cmos_sensor(0x0435,0x5D);
	HM5065_write_cmos_sensor(0x0436,0x00);
	HM5065_write_cmos_sensor(0x0437,0x6D);
	HM5065_write_cmos_sensor(0x0438,0xFF);
	HM5065_write_cmos_sensor(0x0439,0xDB);
	HM5065_write_cmos_sensor(0x043A,0xFF);
	HM5065_write_cmos_sensor(0x043B,0xC3);
	HM5065_write_cmos_sensor(0x043C,0x00);
	HM5065_write_cmos_sensor(0x043D,0x2A);
	HM5065_write_cmos_sensor(0x043E,0xFF);
	HM5065_write_cmos_sensor(0x043F,0xD7);
	HM5065_write_cmos_sensor(0x0450,0xFF);
	HM5065_write_cmos_sensor(0x0451,0xC3);
	HM5065_write_cmos_sensor(0x0452,0xFF);
	HM5065_write_cmos_sensor(0x0453,0x42);
	HM5065_write_cmos_sensor(0x0454,0x00);
	HM5065_write_cmos_sensor(0x0455,0x3A);
	HM5065_write_cmos_sensor(0x0456,0x00);
	HM5065_write_cmos_sensor(0x0457,0x4D);
	HM5065_write_cmos_sensor(0x0458,0xFF);
	HM5065_write_cmos_sensor(0x0459,0xE0);
	HM5065_write_cmos_sensor(0x045A,0x00);
	HM5065_write_cmos_sensor(0x045B,0x45);
	HM5065_write_cmos_sensor(0x045C,0xFF);
	HM5065_write_cmos_sensor(0x045D,0xFF);
	HM5065_write_cmos_sensor(0x045E,0xFF);
	HM5065_write_cmos_sensor(0x045F,0xB3);
	HM5065_write_cmos_sensor(0x0440,0xFF);
	HM5065_write_cmos_sensor(0x0441,0xC8);
	HM5065_write_cmos_sensor(0x0442,0xFF);
	HM5065_write_cmos_sensor(0x0443,0x8F);
	HM5065_write_cmos_sensor(0x0444,0x00);
	HM5065_write_cmos_sensor(0x0445,0x4B);
	HM5065_write_cmos_sensor(0x0446,0x00);
	HM5065_write_cmos_sensor(0x0447,0x58);
	HM5065_write_cmos_sensor(0x0448,0x00);
	HM5065_write_cmos_sensor(0x0449,0x13);
	HM5065_write_cmos_sensor(0x044A,0x00);
	HM5065_write_cmos_sensor(0x044B,0x1B);
	HM5065_write_cmos_sensor(0x044C,0xFF);
	HM5065_write_cmos_sensor(0x044D,0xCF);
	HM5065_write_cmos_sensor(0x044E,0xFF);
	HM5065_write_cmos_sensor(0x044F,0xC2);
	HM5065_write_cmos_sensor(0x0460,0xFF);
	HM5065_write_cmos_sensor(0x0461,0xCB);
	HM5065_write_cmos_sensor(0x0462,0xFF);
	HM5065_write_cmos_sensor(0x0463,0x91);
	HM5065_write_cmos_sensor(0x0464,0x00);
	HM5065_write_cmos_sensor(0x0465,0x48);
	HM5065_write_cmos_sensor(0x0466,0x00);
	HM5065_write_cmos_sensor(0x0467,0x57);
	HM5065_write_cmos_sensor(0x0468,0x00);
	HM5065_write_cmos_sensor(0x0469,0x1F);
	HM5065_write_cmos_sensor(0x046A,0x00);
	HM5065_write_cmos_sensor(0x046B,0x1D);
	HM5065_write_cmos_sensor(0x046C,0xFF);
	HM5065_write_cmos_sensor(0x046D,0xF2);
	HM5065_write_cmos_sensor(0x046E,0xFF);
	HM5065_write_cmos_sensor(0x046F,0xBC);
	HM5065_write_cmos_sensor(0x0470,0xFF);
	HM5065_write_cmos_sensor(0x0471,0xB7);
	HM5065_write_cmos_sensor(0x0472,0xFF);
	HM5065_write_cmos_sensor(0x0473,0x17);
	HM5065_write_cmos_sensor(0x0474,0x00);
	HM5065_write_cmos_sensor(0x0475,0x57);
	HM5065_write_cmos_sensor(0x0476,0x00);
	HM5065_write_cmos_sensor(0x0477,0x69);
	HM5065_write_cmos_sensor(0x0478,0xFF);
	HM5065_write_cmos_sensor(0x0479,0xE6);
	HM5065_write_cmos_sensor(0x047A,0xFF);
	HM5065_write_cmos_sensor(0x047B,0xCD);
	HM5065_write_cmos_sensor(0x047C,0x00);
	HM5065_write_cmos_sensor(0x047D,0x2F);
	HM5065_write_cmos_sensor(0x047E,0xFF);
	HM5065_write_cmos_sensor(0x047F,0xD2);
	HM5065_write_cmos_sensor(0x0490,0xFF);
	HM5065_write_cmos_sensor(0x0491,0xD1);
	HM5065_write_cmos_sensor(0x0492,0xFF);
	HM5065_write_cmos_sensor(0x0493,0x4D);
	HM5065_write_cmos_sensor(0x0494,0x00);
	HM5065_write_cmos_sensor(0x0495,0x38);
	HM5065_write_cmos_sensor(0x0496,0x00);
	HM5065_write_cmos_sensor(0x0497,0x4C);
	HM5065_write_cmos_sensor(0x0498,0xFF);
	HM5065_write_cmos_sensor(0x0499,0xE4);
	HM5065_write_cmos_sensor(0x049A,0x00);
	HM5065_write_cmos_sensor(0x049B,0x36);
	HM5065_write_cmos_sensor(0x049C,0x00);
	HM5065_write_cmos_sensor(0x049D,0x22);
	HM5065_write_cmos_sensor(0x049E,0xFF);
	HM5065_write_cmos_sensor(0x049F,0xB2);
	HM5065_write_cmos_sensor(0x0480,0xFF);
	HM5065_write_cmos_sensor(0x0481,0xCD);
	HM5065_write_cmos_sensor(0x0482,0xFF);
	HM5065_write_cmos_sensor(0x0483,0x93);
	HM5065_write_cmos_sensor(0x0484,0x00);
	HM5065_write_cmos_sensor(0x0485,0x48);
	HM5065_write_cmos_sensor(0x0486,0x00);
	HM5065_write_cmos_sensor(0x0487,0x57);
	HM5065_write_cmos_sensor(0x0488,0x00);
	HM5065_write_cmos_sensor(0x0489,0x20);
	HM5065_write_cmos_sensor(0x048A,0x00);
	HM5065_write_cmos_sensor(0x048B,0x1D);
	HM5065_write_cmos_sensor(0x048C,0xFF);
	HM5065_write_cmos_sensor(0x048D,0xE5);
	HM5065_write_cmos_sensor(0x048E,0xFF);
	HM5065_write_cmos_sensor(0x048F,0xBC);
	HM5065_write_cmos_sensor(0x04A0,0xFF);
	HM5065_write_cmos_sensor(0x04A1,0xD9);
	HM5065_write_cmos_sensor(0x04A2,0xFF);
	HM5065_write_cmos_sensor(0x04A3,0xB9);
	HM5065_write_cmos_sensor(0x04A4,0x00);
	HM5065_write_cmos_sensor(0x04A5,0x42);
	HM5065_write_cmos_sensor(0x04A6,0x00);
	HM5065_write_cmos_sensor(0x04A7,0x51);
	HM5065_write_cmos_sensor(0x04A8,0x00);
	HM5065_write_cmos_sensor(0x04A9,0x27);
	HM5065_write_cmos_sensor(0x04AA,0x00);
	HM5065_write_cmos_sensor(0x04AB,0x1A);
	HM5065_write_cmos_sensor(0x04AC,0xFF);
	HM5065_write_cmos_sensor(0x04AD,0xE9);
	HM5065_write_cmos_sensor(0x04AE,0xFF);
	HM5065_write_cmos_sensor(0x04AF,0xAD);
	HM5065_write_cmos_sensor(0x04B0,0xFF);
	HM5065_write_cmos_sensor(0x04B1,0xC3);
	HM5065_write_cmos_sensor(0x04B2,0xFF);
	HM5065_write_cmos_sensor(0x04B3,0x34);
	HM5065_write_cmos_sensor(0x04B4,0x00);
	HM5065_write_cmos_sensor(0x04B5,0x4F);
	HM5065_write_cmos_sensor(0x04B6,0x00);
	HM5065_write_cmos_sensor(0x04B7,0x61);
	HM5065_write_cmos_sensor(0x04B8,0xFF);
	HM5065_write_cmos_sensor(0x04B9,0xEC);
	HM5065_write_cmos_sensor(0x04BA,0x00);
	HM5065_write_cmos_sensor(0x04BB,0x09);
	HM5065_write_cmos_sensor(0x04BC,0x00);
	HM5065_write_cmos_sensor(0x04BD,0x34);
	HM5065_write_cmos_sensor(0x04BE,0xFF);
	HM5065_write_cmos_sensor(0x04BF,0xC3);
	HM5065_write_cmos_sensor(0x04D0,0xFF);
	HM5065_write_cmos_sensor(0x04D1,0xDB);
	HM5065_write_cmos_sensor(0x04D2,0xFF);
	HM5065_write_cmos_sensor(0x04D3,0x6E);
	HM5065_write_cmos_sensor(0x04D4,0x00);
	HM5065_write_cmos_sensor(0x04D5,0x35);
	HM5065_write_cmos_sensor(0x04D6,0x00);
	HM5065_write_cmos_sensor(0x04D7,0x48);
	HM5065_write_cmos_sensor(0x04D8,0xFF);
	HM5065_write_cmos_sensor(0x04D9,0xE8);
	HM5065_write_cmos_sensor(0x04DA,0x00);
	HM5065_write_cmos_sensor(0x04DB,0x50);
	HM5065_write_cmos_sensor(0x04DC,0x00);
	HM5065_write_cmos_sensor(0x04DD,0x26);
	HM5065_write_cmos_sensor(0x04DE,0xFF);
	HM5065_write_cmos_sensor(0x04DF,0xA7);
	HM5065_write_cmos_sensor(0x04C0,0xFF);
	HM5065_write_cmos_sensor(0x04C1,0xD9);
	HM5065_write_cmos_sensor(0x04C2,0xFF);
	HM5065_write_cmos_sensor(0x04C3,0xBB);
	HM5065_write_cmos_sensor(0x04C4,0x00);
	HM5065_write_cmos_sensor(0x04C5,0x42);
	HM5065_write_cmos_sensor(0x04C6,0x00);
	HM5065_write_cmos_sensor(0x04C7,0x52);
	HM5065_write_cmos_sensor(0x04C8,0x00);
	HM5065_write_cmos_sensor(0x04C9,0x26);
	HM5065_write_cmos_sensor(0x04CA,0x00);
	HM5065_write_cmos_sensor(0x04CB,0x1D);
	HM5065_write_cmos_sensor(0x04CC,0xFF);
	HM5065_write_cmos_sensor(0x04CD,0xEF);
	HM5065_write_cmos_sensor(0x04CE,0xFF);
	HM5065_write_cmos_sensor(0x04CF,0xAC);
	HM5065_write_cmos_sensor(0x04E0,0xFF);
	HM5065_write_cmos_sensor(0x04E1,0xC7);
	HM5065_write_cmos_sensor(0x04E2,0xFF);
	HM5065_write_cmos_sensor(0x04E3,0x8A);
	HM5065_write_cmos_sensor(0x04E4,0x00);
	HM5065_write_cmos_sensor(0x04E5,0x45);
	HM5065_write_cmos_sensor(0x04E6,0x00);
	HM5065_write_cmos_sensor(0x04E7,0x54);
	HM5065_write_cmos_sensor(0x04E8,0x00);
	HM5065_write_cmos_sensor(0x04E9,0x24);
	HM5065_write_cmos_sensor(0x04EA,0x00);
	HM5065_write_cmos_sensor(0x04EB,0x0F);
	HM5065_write_cmos_sensor(0x04EC,0xFF);
	HM5065_write_cmos_sensor(0x04ED,0xD9);
	HM5065_write_cmos_sensor(0x04EE,0xFF);
	HM5065_write_cmos_sensor(0x04EF,0xBC);
	HM5065_write_cmos_sensor(0x04F0,0xFF);
	HM5065_write_cmos_sensor(0x04F1,0xB0);
	HM5065_write_cmos_sensor(0x04F2,0xFF);
	HM5065_write_cmos_sensor(0x04F3,0x0D);
	HM5065_write_cmos_sensor(0x04F4,0x00);
	HM5065_write_cmos_sensor(0x04F5,0x52);
	HM5065_write_cmos_sensor(0x04F6,0x00);
	HM5065_write_cmos_sensor(0x04F7,0x66);
	HM5065_write_cmos_sensor(0x04F8,0xFF);
	HM5065_write_cmos_sensor(0x04F9,0xE7);
	HM5065_write_cmos_sensor(0x04FA,0xFF);
	HM5065_write_cmos_sensor(0x04FB,0xE1);
	HM5065_write_cmos_sensor(0x04FC,0xFF);
	HM5065_write_cmos_sensor(0x04FD,0xFD);
	HM5065_write_cmos_sensor(0x04FE,0xFF);
	HM5065_write_cmos_sensor(0x04FF,0xD2);
	HM5065_write_cmos_sensor(0x0510,0xFF);
	HM5065_write_cmos_sensor(0x0511,0xD7);
	HM5065_write_cmos_sensor(0x0512,0xFF);
	HM5065_write_cmos_sensor(0x0513,0x5D);
	HM5065_write_cmos_sensor(0x0514,0x00);
	HM5065_write_cmos_sensor(0x0515,0x39);
	HM5065_write_cmos_sensor(0x0516,0x00);
	HM5065_write_cmos_sensor(0x0517,0x4B);
	HM5065_write_cmos_sensor(0x0518,0xFF);
	HM5065_write_cmos_sensor(0x0519,0xE8);
	HM5065_write_cmos_sensor(0x051A,0x00);
	HM5065_write_cmos_sensor(0x051B,0x2A);
	HM5065_write_cmos_sensor(0x051C,0x00);
	HM5065_write_cmos_sensor(0x051D,0x1F);
	HM5065_write_cmos_sensor(0x051E,0xFF);
	HM5065_write_cmos_sensor(0x051F,0xB3);
	HM5065_write_cmos_sensor(0x0500,0xFF);
	HM5065_write_cmos_sensor(0x0501,0xC8);
	HM5065_write_cmos_sensor(0x0502,0xFF);
	HM5065_write_cmos_sensor(0x0503,0x8B);
	HM5065_write_cmos_sensor(0x0504,0x00);
	HM5065_write_cmos_sensor(0x0505,0x45);
	HM5065_write_cmos_sensor(0x0506,0x00);
	HM5065_write_cmos_sensor(0x0507,0x55);
	HM5065_write_cmos_sensor(0x0508,0x00);
	HM5065_write_cmos_sensor(0x0509,0x25);
	HM5065_write_cmos_sensor(0x050A,0x00);
	HM5065_write_cmos_sensor(0x050B,0x0F);
	HM5065_write_cmos_sensor(0x050C,0xFF);
	HM5065_write_cmos_sensor(0x050D,0xD5);
	HM5065_write_cmos_sensor(0x050E,0xFF);
	HM5065_write_cmos_sensor(0x050F,0xBC);
	
	HM5065_write_cmos_sensor(0x0561,0x01);
	HM5065_write_cmos_sensor(0x0562,0x01);
	HM5065_write_cmos_sensor(0x0563,0x01);
	HM5065_write_cmos_sensor(0x0564,0x01);
	
	HM5065_write_cmos_sensor(0x0324,0x39);
	HM5065_write_cmos_sensor(0x0325,0x86);
	HM5065_write_cmos_sensor(0x0326,0x3A);
	HM5065_write_cmos_sensor(0x0327,0x90);
	HM5065_write_cmos_sensor(0x0328,0x3A);
	HM5065_write_cmos_sensor(0x0329,0xF6);
	HM5065_write_cmos_sensor(0x032A,0x3B);
	HM5065_write_cmos_sensor(0x032B,0x5D);
//END
*/
/*
	HM5065_write_cmos_sensor(0x0420,0xFF);
	HM5065_write_cmos_sensor(0x0421,0xB8);
	HM5065_write_cmos_sensor(0x0422,0xFF);
	HM5065_write_cmos_sensor(0x0423,0xFE);
	HM5065_write_cmos_sensor(0x0424,0x00);
	HM5065_write_cmos_sensor(0x0425,0x99);
	HM5065_write_cmos_sensor(0x0426,0x00);
	HM5065_write_cmos_sensor(0x0427,0xBA);
	HM5065_write_cmos_sensor(0x0428,0x00);
	HM5065_write_cmos_sensor(0x0429,0x6D);
	HM5065_write_cmos_sensor(0x042A,0xFE);
	HM5065_write_cmos_sensor(0x042B,0x4E);
	HM5065_write_cmos_sensor(0x042C,0xFF);
	HM5065_write_cmos_sensor(0x042D,0x26);
	HM5065_write_cmos_sensor(0x042E,0xFF);
	HM5065_write_cmos_sensor(0x042F,0xE1);
	HM5065_write_cmos_sensor(0x0430,0xFF);
	HM5065_write_cmos_sensor(0x0431,0xAF);
	HM5065_write_cmos_sensor(0x0432,0xFE);
	HM5065_write_cmos_sensor(0x0433,0xB6);
	HM5065_write_cmos_sensor(0x0434,0x00);
	HM5065_write_cmos_sensor(0x0435,0xBE);
	HM5065_write_cmos_sensor(0x0436,0x00);
	HM5065_write_cmos_sensor(0x0437,0xE1);
	HM5065_write_cmos_sensor(0x0438,0xFF);
	HM5065_write_cmos_sensor(0x0439,0xAC);
	HM5065_write_cmos_sensor(0x043A,0xFC);
	HM5065_write_cmos_sensor(0x043B,0xA9);
	HM5065_write_cmos_sensor(0x043C,0xFE);
	HM5065_write_cmos_sensor(0x043D,0xDB);
	HM5065_write_cmos_sensor(0x043E,0x00);
	HM5065_write_cmos_sensor(0x043F,0x42);
	HM5065_write_cmos_sensor(0x0450,0xFF);
	HM5065_write_cmos_sensor(0x0451,0x95);
	HM5065_write_cmos_sensor(0x0452,0xFF);
	HM5065_write_cmos_sensor(0x0453,0x32);
	HM5065_write_cmos_sensor(0x0454,0x00);
	HM5065_write_cmos_sensor(0x0455,0x78);
	HM5065_write_cmos_sensor(0x0456,0x00);
	HM5065_write_cmos_sensor(0x0457,0x9F);
	HM5065_write_cmos_sensor(0x0458,0x00);
	HM5065_write_cmos_sensor(0x0459,0x15);
	HM5065_write_cmos_sensor(0x045A,0xFE);
	HM5065_write_cmos_sensor(0x045B,0xA4);
	HM5065_write_cmos_sensor(0x045C,0xFE);
	HM5065_write_cmos_sensor(0x045D,0xE3);
	HM5065_write_cmos_sensor(0x045E,0xFF);
	HM5065_write_cmos_sensor(0x045F,0xD9);
	HM5065_write_cmos_sensor(0x0440,0xFF);
	HM5065_write_cmos_sensor(0x0441,0xB9);
	HM5065_write_cmos_sensor(0x0442,0x00);
	HM5065_write_cmos_sensor(0x0443,0x02);
	HM5065_write_cmos_sensor(0x0444,0x00);
	HM5065_write_cmos_sensor(0x0445,0x99);
	HM5065_write_cmos_sensor(0x0446,0x00);
	HM5065_write_cmos_sensor(0x0447,0xBB);
	HM5065_write_cmos_sensor(0x0448,0x00);
	HM5065_write_cmos_sensor(0x0449,0x69);
	HM5065_write_cmos_sensor(0x044A,0xFE);
	HM5065_write_cmos_sensor(0x044B,0x4D);
	HM5065_write_cmos_sensor(0x044C,0xFF);
	HM5065_write_cmos_sensor(0x044D,0x39);
	HM5065_write_cmos_sensor(0x044E,0xFF);
	HM5065_write_cmos_sensor(0x044F,0xE2);
	HM5065_write_cmos_sensor(0x0460,0xFF);
	HM5065_write_cmos_sensor(0x0461,0xBC);
	HM5065_write_cmos_sensor(0x0462,0xFF);
	HM5065_write_cmos_sensor(0x0463,0xFD);
	HM5065_write_cmos_sensor(0x0464,0x00);
	HM5065_write_cmos_sensor(0x0465,0x99);
	HM5065_write_cmos_sensor(0x0466,0x00);
	HM5065_write_cmos_sensor(0x0467,0xB9);
	HM5065_write_cmos_sensor(0x0468,0x00);
	HM5065_write_cmos_sensor(0x0469,0x7D);
	HM5065_write_cmos_sensor(0x046A,0xFE);
	HM5065_write_cmos_sensor(0x046B,0x07);
	HM5065_write_cmos_sensor(0x046C,0xFF);
	HM5065_write_cmos_sensor(0x046D,0x24);
	HM5065_write_cmos_sensor(0x046E,0xFF);
	HM5065_write_cmos_sensor(0x046F,0xFE);
	HM5065_write_cmos_sensor(0x0470,0xFF);
	HM5065_write_cmos_sensor(0x0471,0xAF);
	HM5065_write_cmos_sensor(0x0472,0xFE);
	HM5065_write_cmos_sensor(0x0473,0xBD);
	HM5065_write_cmos_sensor(0x0474,0x00);
	HM5065_write_cmos_sensor(0x0475,0xBA);
	HM5065_write_cmos_sensor(0x0476,0x00);
	HM5065_write_cmos_sensor(0x0477,0xDC);
	HM5065_write_cmos_sensor(0x0478,0xFF);
	HM5065_write_cmos_sensor(0x0479,0xC1);
	HM5065_write_cmos_sensor(0x047A,0xFC);
	HM5065_write_cmos_sensor(0x047B,0xAB);
	HM5065_write_cmos_sensor(0x047C,0xFE);
	HM5065_write_cmos_sensor(0x047D,0xB9);
	HM5065_write_cmos_sensor(0x047E,0x00);
	HM5065_write_cmos_sensor(0x047F,0x5C);
	HM5065_write_cmos_sensor(0x0490,0xFF);
	HM5065_write_cmos_sensor(0x0491,0xA3);
	HM5065_write_cmos_sensor(0x0492,0xFF);
	HM5065_write_cmos_sensor(0x0493,0x2A);
	HM5065_write_cmos_sensor(0x0494,0x00);
	HM5065_write_cmos_sensor(0x0495,0x7B);
	HM5065_write_cmos_sensor(0x0496,0x00);
	HM5065_write_cmos_sensor(0x0497,0xA1);
	HM5065_write_cmos_sensor(0x0498,0x00);
	HM5065_write_cmos_sensor(0x0499,0x0C);
	HM5065_write_cmos_sensor(0x049A,0xFE);
	HM5065_write_cmos_sensor(0x049B,0x40);
	HM5065_write_cmos_sensor(0x049C,0xFE);
	HM5065_write_cmos_sensor(0x049D,0xF3);
	HM5065_write_cmos_sensor(0x049E,0xFF);
	HM5065_write_cmos_sensor(0x049F,0xEB);
	HM5065_write_cmos_sensor(0x0480,0xFF);
	HM5065_write_cmos_sensor(0x0481,0xBD);
	HM5065_write_cmos_sensor(0x0482,0xFF);
	HM5065_write_cmos_sensor(0x0483,0xFE);
	HM5065_write_cmos_sensor(0x0484,0x00);
	HM5065_write_cmos_sensor(0x0485,0x99);
	HM5065_write_cmos_sensor(0x0486,0x00);
	HM5065_write_cmos_sensor(0x0487,0xB9);
	HM5065_write_cmos_sensor(0x0488,0x00);
	HM5065_write_cmos_sensor(0x0489,0x77);
	HM5065_write_cmos_sensor(0x048A,0xFE);
	HM5065_write_cmos_sensor(0x048B,0x25);
	HM5065_write_cmos_sensor(0x048C,0xFF);
	HM5065_write_cmos_sensor(0x048D,0x36);
	HM5065_write_cmos_sensor(0x048E,0xFF);
	HM5065_write_cmos_sensor(0x048F,0xFC);
	HM5065_write_cmos_sensor(0x04A0,0xFF);
	HM5065_write_cmos_sensor(0x04A1,0xC1);
	HM5065_write_cmos_sensor(0x04A2,0xFF);
	HM5065_write_cmos_sensor(0x04A3,0xFF);
	HM5065_write_cmos_sensor(0x04A4,0x00);
	HM5065_write_cmos_sensor(0x04A5,0x97);
	HM5065_write_cmos_sensor(0x04A6,0x00);
	HM5065_write_cmos_sensor(0x04A7,0xB7);
	HM5065_write_cmos_sensor(0x04A8,0x00);
	HM5065_write_cmos_sensor(0x04A9,0x80);
	HM5065_write_cmos_sensor(0x04AA,0xFE);
	HM5065_write_cmos_sensor(0x04AB,0x33);
	HM5065_write_cmos_sensor(0x04AC,0xFF);
	HM5065_write_cmos_sensor(0x04AD,0x60);
	HM5065_write_cmos_sensor(0x04AE,0xFF);
	HM5065_write_cmos_sensor(0x04AF,0xFA);
	HM5065_write_cmos_sensor(0x04B0,0xFF);
	HM5065_write_cmos_sensor(0x04B1,0xB6);
	HM5065_write_cmos_sensor(0x04B2,0xFE);
	HM5065_write_cmos_sensor(0x04B3,0xC9);
	HM5065_write_cmos_sensor(0x04B4,0x00);
	HM5065_write_cmos_sensor(0x04B5,0xB4);
	HM5065_write_cmos_sensor(0x04B6,0x00);
	HM5065_write_cmos_sensor(0x04B7,0xD8);
	HM5065_write_cmos_sensor(0x04B8,0xFF);
	HM5065_write_cmos_sensor(0x04B9,0xD1);
	HM5065_write_cmos_sensor(0x04BA,0xFC);
	HM5065_write_cmos_sensor(0x04BB,0xFE);
	HM5065_write_cmos_sensor(0x04BC,0xFF);
	HM5065_write_cmos_sensor(0x04BD,0x06);
	HM5065_write_cmos_sensor(0x04BE,0x00);
	HM5065_write_cmos_sensor(0x04BF,0x4B);
	HM5065_write_cmos_sensor(0x04D0,0xFF);
	HM5065_write_cmos_sensor(0x04D1,0xAC);
	HM5065_write_cmos_sensor(0x04D2,0xFF);
	HM5065_write_cmos_sensor(0x04D3,0x29);
	HM5065_write_cmos_sensor(0x04D4,0x00);
	HM5065_write_cmos_sensor(0x04D5,0x79);
	HM5065_write_cmos_sensor(0x04D6,0x00);
	HM5065_write_cmos_sensor(0x04D7,0x9F);
	HM5065_write_cmos_sensor(0x04D8,0x00);
	HM5065_write_cmos_sensor(0x04D9,0x0B);
	HM5065_write_cmos_sensor(0x04DA,0xFE);
	HM5065_write_cmos_sensor(0x04DB,0x55);
	HM5065_write_cmos_sensor(0x04DC,0xFF);
	HM5065_write_cmos_sensor(0x04DD,0x1F);
	HM5065_write_cmos_sensor(0x04DE,0xFF);
	HM5065_write_cmos_sensor(0x04DF,0xEB);
	HM5065_write_cmos_sensor(0x04C0,0xFF);
	HM5065_write_cmos_sensor(0x04C1,0xC4);
	HM5065_write_cmos_sensor(0x04C2,0x00);
	HM5065_write_cmos_sensor(0x04C3,0x03);
	HM5065_write_cmos_sensor(0x04C4,0x00);
	HM5065_write_cmos_sensor(0x04C5,0x97);
	HM5065_write_cmos_sensor(0x04C6,0x00);
	HM5065_write_cmos_sensor(0x04C7,0xB7);
	HM5065_write_cmos_sensor(0x04C8,0x00);
	HM5065_write_cmos_sensor(0x04C9,0x7D);
	HM5065_write_cmos_sensor(0x04CA,0xFE);
	HM5065_write_cmos_sensor(0x04CB,0x2D);
	HM5065_write_cmos_sensor(0x04CC,0xFF);
	HM5065_write_cmos_sensor(0x04CD,0x70);
	HM5065_write_cmos_sensor(0x04CE,0xFF);
	HM5065_write_cmos_sensor(0x04CF,0xF8);
	HM5065_write_cmos_sensor(0x04E0,0xFF);
	HM5065_write_cmos_sensor(0x04E1,0x96);
	HM5065_write_cmos_sensor(0x04E2,0x00);
	HM5065_write_cmos_sensor(0x04E3,0x1C);
	HM5065_write_cmos_sensor(0x04E4,0x00);
	HM5065_write_cmos_sensor(0x04E5,0x91);
	HM5065_write_cmos_sensor(0x04E6,0x00);
	HM5065_write_cmos_sensor(0x04E7,0xB4);
	HM5065_write_cmos_sensor(0x04E8,0x00);
	HM5065_write_cmos_sensor(0x04E9,0x9A);
	HM5065_write_cmos_sensor(0x04EA,0xFE);
	HM5065_write_cmos_sensor(0x04EB,0x62);
	HM5065_write_cmos_sensor(0x04EC,0xFF);
	HM5065_write_cmos_sensor(0x04ED,0x26);
	HM5065_write_cmos_sensor(0x04EE,0xFF);
	HM5065_write_cmos_sensor(0x04EF,0xD7);
	HM5065_write_cmos_sensor(0x04F0,0xFF);
	HM5065_write_cmos_sensor(0x04F1,0x89);
	HM5065_write_cmos_sensor(0x04F2,0xFE);
	HM5065_write_cmos_sensor(0x04F3,0xEB);
	HM5065_write_cmos_sensor(0x04F4,0x00);
	HM5065_write_cmos_sensor(0x04F5,0xAA);
	HM5065_write_cmos_sensor(0x04F6,0x00);
	HM5065_write_cmos_sensor(0x04F7,0xD2);
	HM5065_write_cmos_sensor(0x04F8,0xFF);
	HM5065_write_cmos_sensor(0x04F9,0xF4);
	HM5065_write_cmos_sensor(0x04FA,0xFD);
	HM5065_write_cmos_sensor(0x04FB,0x9A);
	HM5065_write_cmos_sensor(0x04FC,0xFE);
	HM5065_write_cmos_sensor(0x04FD,0xB1);
	HM5065_write_cmos_sensor(0x04FE,0x00);
	HM5065_write_cmos_sensor(0x04FF,0x2E);
	HM5065_write_cmos_sensor(0x0510,0xFF);
	HM5065_write_cmos_sensor(0x0511,0x94);
	HM5065_write_cmos_sensor(0x0512,0xFF);
	HM5065_write_cmos_sensor(0x0513,0x41);
	HM5065_write_cmos_sensor(0x0514,0x00);
	HM5065_write_cmos_sensor(0x0515,0x77);
	HM5065_write_cmos_sensor(0x0516,0x00);
	HM5065_write_cmos_sensor(0x0517,0x9F);
	HM5065_write_cmos_sensor(0x0518,0xFF);
	HM5065_write_cmos_sensor(0x0519,0xFE);
	HM5065_write_cmos_sensor(0x051A,0xFE);
	HM5065_write_cmos_sensor(0x051B,0x22);
	HM5065_write_cmos_sensor(0x051C,0xFE);
	HM5065_write_cmos_sensor(0x051D,0xFC);
	HM5065_write_cmos_sensor(0x051E,0xFF);
	HM5065_write_cmos_sensor(0x051F,0xD6);
	HM5065_write_cmos_sensor(0x0500,0xFF);
	HM5065_write_cmos_sensor(0x0501,0x99);
	HM5065_write_cmos_sensor(0x0502,0x00);
	HM5065_write_cmos_sensor(0x0503,0x20);
	HM5065_write_cmos_sensor(0x0504,0x00);
	HM5065_write_cmos_sensor(0x0505,0x91);
	HM5065_write_cmos_sensor(0x0506,0x00);
	HM5065_write_cmos_sensor(0x0507,0xB5);
	HM5065_write_cmos_sensor(0x0508,0x00);
	HM5065_write_cmos_sensor(0x0509,0x99);
	HM5065_write_cmos_sensor(0x050A,0xFE);
	HM5065_write_cmos_sensor(0x050B,0x5A);
	HM5065_write_cmos_sensor(0x050C,0xFF);
	HM5065_write_cmos_sensor(0x050D,0x1B);
	HM5065_write_cmos_sensor(0x050E,0xFF);
	HM5065_write_cmos_sensor(0x050F,0xD5);
	
	HM5065_write_cmos_sensor(0x0561,0x01);
	HM5065_write_cmos_sensor(0x0562,0x00);
	HM5065_write_cmos_sensor(0x0563,0x01);
	HM5065_write_cmos_sensor(0x0564,0x00);
	
	HM5065_write_cmos_sensor(0x0324,0x39);
	HM5065_write_cmos_sensor(0x0325,0x86);
	HM5065_write_cmos_sensor(0x0326,0x3A);
	HM5065_write_cmos_sensor(0x0327,0x90);
	HM5065_write_cmos_sensor(0x0328,0x3A);
	HM5065_write_cmos_sensor(0x0329,0xF6);
	HM5065_write_cmos_sensor(0x032A,0x3B);
	HM5065_write_cmos_sensor(0x032B,0x5D);
*/
  //////////////////////////////////////
  
	HM5065_write_cmos_sensor(0x0754,0x01);
	HM5065_write_cmos_sensor(0x0773,0x08);
	HM5065_write_cmos_sensor(0x0779,0x00);
	HM5065_write_cmos_sensor(0x077a,0x03);
	HM5065_write_cmos_sensor(0x0846,0x0a);

	HM5065_write_cmos_sensor(0x016C,0x00);
		//AE
	HM5065_write_cmos_sensor(0x0143, 0x5D);//max intg 5 fps
	HM5065_write_cmos_sensor(0x0144, 0x0D);	
	//AWB SPEED
	/*
	HM5065_write_cmos_sensor(0x01D2, 0x37);
	HM5065_write_cmos_sensor(0x01D3, 0x33);		
	HM5065_write_cmos_sensor(0x01D4, 0x3A);
	HM5065_write_cmos_sensor(0x01D5, 0x00);		
	HM5065_write_cmos_sensor(0x01D6, 0x3A);
	HM5065_write_cmos_sensor(0x01D7, 0x00);	
	*/
	
	HM5065_write_cmos_sensor(0x0257, 0x02);//20130117, Blue offset
	
	return ERROR_NONE;
	
	}


/*****************************************************************************/
/* Windows Mobile Sensor Interface */
/*****************************************************************************/
/*************************************************************************
* FUNCTION
*	HM5065Open
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
static kal_uint32 HM5065Open(void)

{
	kal_uint16 sensor_id=0; 
	kal_uint16 data=0;
	int retry = 10; 
	

    SENSORDB("HM5065Open_start \n");

//	HM5065_Sensor_Driver.i2c_clit.addr=HM5065_WRITE_ID;
//	HM5065_Sensor_Driver.i2c_clit = i2c_clit;
//    HM5065_Sensor_Driver.i2c_clit->addr = HM5065_WRITE_ID;

	// check if sensor ID correct
	do {
    	    sensor_id=((HM5065_read_cmos_sensor(0x0000)<< 8)|HM5065_read_cmos_sensor(0x0001));
    	    if (sensor_id == HM5065_SENSOR_ID) {
                 break; 
    	    }
            SENSORDB("Read Sensor ID Fail = 0x%04x\n", sensor_id); 
    	    
    	    retry--; 
	}while (retry > 0); 
	
	if (sensor_id != HM5065_SENSOR_ID) {
	    return ERROR_SENSOR_CONNECT_FAIL;
	}
	 
        memset(&HM5065_Sensor_Driver, 0, sizeof(struct HM5065_Sensor_Struct)); 
	HM5065_Sensor_Driver.MPEG4_encode_mode=KAL_FALSE;
	HM5065_Sensor_Driver.dummy_pixels=0;
	HM5065_Sensor_Driver.dummy_lines=0;
	HM5065_Sensor_Driver.extra_exposure_lines=0;
	HM5065_Sensor_Driver.exposure_lines=0;
	HM5065_Sensor_Driver.MODE_CAPTURE=KAL_FALSE;
		
	HM5065_Sensor_Driver.bNight_mode =KAL_FALSE; // to distinguish night mode or auto mode, default: auto mode setting
	HM5065_Sensor_Driver.bBanding_value = AE_FLICKER_MODE_50HZ; // to distinguish between 50HZ and 60HZ.
		
	HM5065_Sensor_Driver.fPV_PCLK = 26; //26000000;
	HM5065_Sensor_Driver.iPV_Pixels_Per_Line = 0;

//	HM5065_set_isp_driving_current(1);
	// initail sequence write in
    //HM5065_write_cmos_sensor(0x12, 0x80);
    mDELAY(10);
    HM5065_Sensor_Driver_Init();	
	data =HM5065_read_cmos_sensor(0x0010);
	SENSORDB("==================================0x0010= 0x%02x\n", data); 
	
    SENSORDB("HM5065Open_end \n");
    
	return ERROR_NONE;
}   /* HM5065Open  */


/*************************************************************************
* FUNCTION
*	HM5065_GetSensorID
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
static kal_uint32 HM5065_GetSensorID(kal_uint32 *sensorID)

{
    int  retry = 3; 
    // check if sensor ID correct
    do {
        *sensorID=((HM5065_read_cmos_sensor(0x0000)<< 8)|HM5065_read_cmos_sensor(0x0001));
        if (*sensorID == HM5065_SENSOR_ID)
            break; 
        SENSORDB("Read Sensor ID Fail = 0x%04x\n", *sensorID); 
        retry--; 
    } while (retry > 0);

		printk("[ysm]HM5065_GetSensorID >>> %x", *sensorID);

    if (*sensorID != HM5065_SENSOR_ID) {
        *sensorID = 0xFFFFFFFF; 
        return ERROR_SENSOR_CONNECT_FAIL;
    }
    return ERROR_NONE;    
}   /* HM5065Open  */

/*************************************************************************
* FUNCTION
*	HM5065Close
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
static kal_uint32 HM5065Close(void)
{
kal_uint8 tmp1;
    tmp1 = closed;
	CAMERA_CONTROL_FLOW(tmp1,closed++);

	return ERROR_NONE;
}   /* HM5065Close */


static void HM5065_HVMirror(ACDK_SENSOR_IMAGE_MIRROR_ENUM SensorImageMirror)
{
	switch (SensorImageMirror)
	{
		case IMAGE_NORMAL:	
			HM5065_write_cmos_sensor(0x0083, 0x01); 
			HM5065_write_cmos_sensor(0x0084, 0x01); 
			break;
		case IMAGE_H_MIRROR:			 
			HM5065_write_cmos_sensor(0x0083, 0x01); 
			HM5065_write_cmos_sensor(0x0084, 0x00); 
			break;
		case IMAGE_V_MIRROR:			 
			HM5065_write_cmos_sensor(0x0083, 0x00); 
			HM5065_write_cmos_sensor(0x0084, 0x01); 
			break;
		case IMAGE_HV_MIRROR:						 
			HM5065_write_cmos_sensor(0x0083, 0x00); 
			HM5065_write_cmos_sensor(0x0084, 0x00); 
			break;
	}
}
/*************************************************************************
* FUNCTION
* HM5065_Preview
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
static kal_uint32 HM5065_Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)

{
	SENSORDB("****HM5065_Preview****start***\n"); 
	//printk("[ysm]HM5065_Preview >>> start");
	
	HM5065_Sensor_Driver.fPV_PCLK=26000000;
	HM5065_Sensor_Driver.MODE_CAPTURE=KAL_FALSE;

	if(sensor_config_data->SensorOperationMode==MSDK_SENSOR_OPERATION_MODE_VIDEO){
		HM5065_Sensor_Driver.MPEG4_encode_mode = KAL_TRUE;  // MPEG4 Encode Mode
	}else{
		HM5065_Sensor_Driver.MPEG4_encode_mode = KAL_FALSE;  
	}

	//sensor_config_data->SensorImageMirror = IMAGE_V_MIRROR;
	printk("hwj HM5065_Preview mirror sensor_config_data->SensorImageMirror = %d", sensor_config_data->SensorImageMirror);
#if defined(BIRD_HM5065_HV_MIRROR)
	HM5065_HVMirror(IMAGE_HV_MIRROR);
#else
	HM5065_HVMirror(sensor_config_data->SensorImageMirror);
#endif

	HM5065_Sensor_Driver.dummy_pixels = 0;
	HM5065_Sensor_Driver.dummy_lines = 42;
	HM5065_Sensor_Driver.iPV_Pixels_Per_Line =VGA_PERIOD_PIXEL_NUMS+HM5065_Sensor_Driver.dummy_pixels;  
	HM5065_Set_Dummy(HM5065_Sensor_Driver.dummy_pixels, HM5065_Sensor_Driver.dummy_lines);

	HM5065_write_cmos_sensor(0x0142, 0x00);
	HM5065_write_cmos_sensor(0x01A4, 0x00);//AWB ON
	HM5065_write_cmos_sensor(0x0010, 0x02);//Stream off
	mdelay(200);
	HM5065_write_cmos_sensor(0x00B5, 0x02); //20130123 01->02
	HM5065_write_cmos_sensor(0x0030, 0x14);
	#if 1
	HM5065_write_cmos_sensor(0x0040, 0x01);//	binning mode and subsampling mode for frame rate
	HM5065_write_cmos_sensor(0x0041, 0x0A); //	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
	#else
	HM5065_write_cmos_sensor(0x0040, 0x01);//	binning mode and subsampling mode for frame rate
	HM5065_write_cmos_sensor(0x0041, 0x04); //	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
	#endif
	HM5065_write_cmos_sensor(0x0042, 0x05); //	X:800 0x500=1280,0x0320=800
	HM5065_write_cmos_sensor(0x0043, 0x10); //
	HM5065_write_cmos_sensor(0x0044, 0x03); //	Y:600 0x03c0=960,0x0258=600
	HM5065_write_cmos_sensor(0x0045, 0xcc); //
	HM5065_write_cmos_sensor(0x00E8, 0x01);//AFR
	HM5065_write_cmos_sensor(0x00ED, 0x0F);//min 15 fps
	HM5065_write_cmos_sensor(0x00EE, 0x1E);//30fps
	HM5065_write_cmos_sensor(0x0251, 0x02);//BLC ON
	HM5065_write_cmos_sensor(0x0261, 0x00);//20130117
	//20130115
	HM5065_write_cmos_sensor(0x01D2, 0x2F);
	HM5065_write_cmos_sensor(0x01D3, 0x00);		
	HM5065_write_cmos_sensor(0x01D4, 0x34);
	HM5065_write_cmos_sensor(0x01D5, 0x00);		
	HM5065_write_cmos_sensor(0x01D6, 0x38);
	HM5065_write_cmos_sensor(0x01D7, 0x00);			
	
	HM5065_write_cmos_sensor(0x0010, 0x01);//Stream on
	mdelay(200);
	//HM5065_write_cmos_sensor(0x070a, 0x01);
	
	image_window->GrabStartX= IMAGE_SENSOR_VGA_INSERTED_PIXELS;
	image_window->GrabStartY= IMAGE_SENSOR_VGA_INSERTED_LINES;
	image_window->ExposureWindowWidth = IMAGE_SENSOR_PV_WIDTH-image_window->GrabStartX;
	image_window->ExposureWindowHeight =IMAGE_SENSOR_PV_HEIGHT-image_window->GrabStartY;
	
	// copy sensor_config_data
	memcpy(&HM5065SensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
/*
	SENSORDB("****HM5065_Preview****end***\n"); 
	{
	    unsigned int i;

	    i = ioread32(0xF2090024);
	    printk("=================[%x]", i);
	    if(0==(0x800 & i))
	    {
       	    iowrite32((i | 0x800), 0xF2090024);
       	    i = ioread32(0xF2090024);
       	    printk("--[%x]===============\n", i);
	    }
	    printk("\n", i);
       }

    SENSORDB("preview set AE !!!!E \n");

    resetPVAE_2_ARRAY();
    mapAE_2_ARRAY_To_AE_1_ARRAY();
    writeAEReg();
    printAE_2_ARRAY();

    SENSORDB("preview set AE !!!!X \n");
	*/
	return ERROR_NONE;

}   /*  HM5065_Preview   */

/*************************************************************************
* FUNCTION
*	HM5065_Capture
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
static kal_uint32 HM5065_Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
						  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)

{
	volatile kal_uint32 shutter=HM5065_Sensor_Driver.exposure_lines;
	//tatic float fCP_PCLK = 0;
	kal_uint32 fCP_PCLK = 0;
	kal_uint16 iCP_Pixels_Per_Line = 0;

	HM5065_Sensor_Driver.MODE_CAPTURE=KAL_TRUE;

	SENSORDB("****HM5065_Capture****start***\n"); 

	HM5065_Sensor_Driver.dummy_pixels=0;
	HM5065_Sensor_Driver.dummy_lines=0;  
	fCP_PCLK= HM5065_Sensor_Driver.fPV_PCLK;
	iCP_Pixels_Per_Line =VGA_PERIOD_PIXEL_NUMS+HM5065_Sensor_Driver.dummy_pixels;
	shutter = shutter * (fCP_PCLK / HM5065_Sensor_Driver.fPV_PCLK) * HM5065_Sensor_Driver.iPV_Pixels_Per_Line / iCP_Pixels_Per_Line;
	if (shutter < 1) {
	  shutter = 1;
	}	  
	HM5065_Set_Dummy(HM5065_Sensor_Driver.dummy_pixels, HM5065_Sensor_Driver.dummy_lines);

	HM5065_write_cmos_sensor(0x0142, 0x01);
	//HM5065_write_cmos_sensor(0x01A4, 0x04);
  HM5065_write_cmos_sensor(0x0010, 0x02);
	mdelay(100);	
	HM5065_write_cmos_sensor(0x00B5, 0x02); //2012-10-20 , 02-->01
	HM5065_write_cmos_sensor(0x0030, 0x11); //2012-10-20	
	HM5065_write_cmos_sensor(0x0040, 0x00); //Full size
	HM5065_write_cmos_sensor(0x0041, 0x00); //00:full size
	HM5065_write_cmos_sensor(0x0042, 0x0A); //X:2592
	HM5065_write_cmos_sensor(0x0043, 0x00);
	HM5065_write_cmos_sensor(0x0044, 0x07); //Y:1944
	HM5065_write_cmos_sensor(0x0045, 0x80);	
	HM5065_write_cmos_sensor(0x0251, 0x01);//BLC off
	
	if ((HM5065_read_cmos_sensor(0x0180) >= 0x42) && (HM5065_read_cmos_sensor(0x0182) >= 0x40)) // AG>=4 && DG>=2
		HM5065_write_cmos_sensor(0x0261, 0x03);//20130117 00->04->02->03->04
	else
		HM5065_write_cmos_sensor(0x0261, 0x00);//20130117
	//20130115
/*
	HM5065_write_cmos_sensor(0x01D2, 0x37);
	HM5065_write_cmos_sensor(0x01D3, 0x33);		
	HM5065_write_cmos_sensor(0x01D4, 0x3A);
	HM5065_write_cmos_sensor(0x01D5, 0x00);		
	HM5065_write_cmos_sensor(0x01D6, 0x3A);
	HM5065_write_cmos_sensor(0x01D7, 0x00);		
	
	*/
	
	
	HM5065_write_cmos_sensor(0x0010, 0x01);//Stream on
	mdelay(200); 	
	//HM5065_FOCUS_Befor_Cap();

//if (af_type !=AF_INF)
	{
	HM5065_write_cmos_sensor(0x070A, 0x00);
	HM5065_write_cmos_sensor(0x0734, af_pos_h & 0xFF);
	HM5065_write_cmos_sensor(0x0735, af_pos_l & 0xFF);
	HM5065_write_cmos_sensor(0x070C, 0x00);
	mdelay(200);
	HM5065_write_cmos_sensor(0x070C, 0x05);
	mdelay(200);
 
 printk("HM5065_Capture read target af pos1: %02x %02x.\n", HM5065_read_cmos_sensor(0x0700), HM5065_read_cmos_sensor(0x0701));
	}

	// copy sensor_config_data
	memcpy(&HM5065SensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));

	image_window->GrabStartX = IMAGE_SENSOR_VGA_INSERTED_PIXELS*2;
	image_window->GrabStartY = IMAGE_SENSOR_VGA_INSERTED_LINES*2;
	image_window->ExposureWindowWidth= IMAGE_SENSOR_FULL_WIDTH-4*image_window->GrabStartX;
	image_window->ExposureWindowHeight = IMAGE_SENSOR_FULL_HEIGHT-4*image_window->GrabStartY;  

	//HM5065_DELAY_AFTER_PREVIEW = 2;

	return ERROR_NONE;
}   /* HM5065_Capture() */

static kal_uint32 HM5065GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{

kal_uint8 tmp1;
    tmp1 = res;
	CAMERA_CONTROL_FLOW(tmp1,res++);
#if 0
	pSensorResolution->SensorFullWidth=IMAGE_SENSOR_FULL_WIDTH ;
	pSensorResolution->SensorFullHeight=IMAGE_SENSOR_FULL_HEIGHT;
    pSensorResolution->SensorPreviewWidth=IMAGE_SENSOR_PV_WIDTH;
	pSensorResolution->SensorPreviewHeight=IMAGE_SENSOR_PV_HEIGHT;
#else
	pSensorResolution->SensorFullWidth=IMAGE_SENSOR_FULL_WIDTH - 2*HM5065_IMAGE_SENSOR_PV_STARTX;
	pSensorResolution->SensorFullHeight=IMAGE_SENSOR_FULL_HEIGHT- 2*HM5065_IMAGE_SENSOR_PV_STARTY;
    pSensorResolution->SensorPreviewWidth=IMAGE_SENSOR_PV_WIDTH-HM5065_IMAGE_SENSOR_PV_STARTX;
	pSensorResolution->SensorPreviewHeight=IMAGE_SENSOR_PV_HEIGHT-HM5065_IMAGE_SENSOR_PV_STARTY;
#endif	
	return ERROR_NONE;
}	/* HM5065GetResolution() */

static kal_uint32 HM5065GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
					  MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{

#if 1
	pSensorInfo->SensorPreviewResolutionX=IMAGE_SENSOR_PV_WIDTH;
	pSensorInfo->SensorPreviewResolutionY=IMAGE_SENSOR_PV_HEIGHT;
	pSensorInfo->SensorFullResolutionX=IMAGE_SENSOR_FULL_WIDTH;
	pSensorInfo->SensorFullResolutionY=IMAGE_SENSOR_FULL_WIDTH;
	pSensorInfo->SensorCameraPreviewFrameRate=30;
	pSensorInfo->SensorVideoFrameRate=30;
	pSensorInfo->SensorStillCaptureFrameRate=10;
	pSensorInfo->SensorWebCamCaptureFrameRate=15;
	pSensorInfo->SensorResetActiveHigh=FALSE; //low active
	pSensorInfo->SensorResetDelayCount=5; 
#endif

	pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_UYVY;
	//pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_YUYV;
	pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
 	pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_HIGH;
       //pSensorInfo->SensorDriver3D = 0;   // the sensor driver is 2D

	
	pSensorInfo->SensorMasterClockSwitch = 0; 
      pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_2MA;   		


	pSensorInfo->SensorInterruptDelayLines = 1;
	pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_PARALLEL;
#if 0	
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_100_MODE].MaxWidth=CAM_SIZE_5M_WIDTH;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_100_MODE].MaxHeight=CAM_SIZE_5M_HEIGHT;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_100_MODE].ISOSupported=TRUE;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_100_MODE].BinningEnable=FALSE;

	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_200_MODE].MaxWidth=CAM_SIZE_5M_WIDTH;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_200_MODE].MaxHeight=CAM_SIZE_5M_HEIGHT;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_200_MODE].ISOSupported=TRUE;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_200_MODE].BinningEnable=FALSE;

	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_400_MODE].MaxWidth=CAM_SIZE_5M_WIDTH;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_400_MODE].MaxHeight=CAM_SIZE_5M_HEIGHT;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_400_MODE].ISOSupported=TRUE;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_400_MODE].BinningEnable=FALSE;

	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_800_MODE].MaxWidth=CAM_SIZE_1M_WIDTH;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_800_MODE].MaxHeight=CAM_SIZE_1M_HEIGHT;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_800_MODE].ISOSupported=TRUE;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_800_MODE].BinningEnable=TRUE;

	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].MaxWidth=CAM_SIZE_1M_WIDTH;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].MaxHeight=CAM_SIZE_1M_HEIGHT;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].ISOSupported=TRUE;
	pSensorInfo->SensorISOBinningInfo.ISOBinningInfo[ISO_1600_MODE].BinningEnable=TRUE;
#endif

	//CAMERA_CONTROL_FLOW(ScenarioId,ScenarioId);
      pSensorInfo->PreviewDelayFrame = 10; 
      pSensorInfo->VideoDelayFrame = 5; 	

	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		//case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:		
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount=	3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
			
			pSensorInfo->SensorGrabStartX = IMAGE_SENSOR_VGA_INSERTED_PIXELS; 
			pSensorInfo->SensorGrabStartY = IMAGE_SENSOR_VGA_INSERTED_LINES;			   
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		//case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:		
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount= 3;
			pSensorInfo->SensorClockRisingCount=0;
			pSensorInfo->SensorClockFallingCount=2;
			pSensorInfo->SensorPixelClockCount=3;
			pSensorInfo->SensorDataLatchCount=2;	
			
			pSensorInfo->SensorGrabStartX = IMAGE_SENSOR_VGA_INSERTED_PIXELS; 
			pSensorInfo->SensorGrabStartY = IMAGE_SENSOR_VGA_INSERTED_LINES;			   
		break;
		default:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount= 3;
			pSensorInfo->SensorClockRisingCount=0;
			pSensorInfo->SensorClockFallingCount=2;
			pSensorInfo->SensorPixelClockCount=3;
			pSensorInfo->SensorDataLatchCount=2;
			
			pSensorInfo->SensorGrabStartX =1;// IMAGE_SENSOR_VGA_INSERTED_PIXELS; 
			pSensorInfo->SensorGrabStartY =1;// IMAGE_SENSOR_VGA_INSERTED_LINES;			   
		break;
	}

	memcpy(pSensorConfigData, &HM5065SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));


	return ERROR_NONE;
}	/* HM5065GetInfo() */

static kal_uint32 HM5065Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	CAMERA_CONTROL_FLOW(ScenarioId,ScenarioId);

	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		//case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
			 HM5065_Preview(pImageWindow, pSensorConfigData);
			
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		//case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
		     //HM5065_FOCUS_Single_Focus();
		     //HM5065_FOCUS_Befor_Cap();
			 HM5065_Capture(pImageWindow, pSensorConfigData);
		break;
		default:
			return ERROR_INVALID_SCENARIO_ID;
	}
	return TRUE;
}	/* HM5065Control() */

static void HM5065_AV_Calibration(void)
{
	HM5065_write_cmos_sensor(0x0049,0x00); // Gamma gain
	HM5065_write_cmos_sensor(0x004a,0x00); // Gamma interpolation
	HM5065_write_cmos_sensor(0x0080,0x64); // Contrast	100
	HM5065_write_cmos_sensor(0x0081,0x64); // Saturation	100
	HM5065_write_cmos_sensor(0x0082,0x64); // Brightness	100
	HM5065_write_cmos_sensor(0x0083,0x00); // HorizontalMirror	FALSE
	HM5065_write_cmos_sensor(0x0084,0x00); // VerticalFlip	FALSE
	HM5065_write_cmos_sensor(0x00c8,0x00); // Static Frame Rate_Num MSB	0
	HM5065_write_cmos_sensor(0x00c9,0x01); // Static Frame Rate_Num LSB	1
	HM5065_write_cmos_sensor(0x00ca,0x01); // Static Frame Rate_Den	1
	HM5065_write_cmos_sensor(0x00e8,0x00); // Frame Rate	MANUAL
	HM5065_write_cmos_sensor(0x0130,0x06); // Exposure Compensation	6
	HM5065_write_cmos_sensor(0x0143,0x65); // MaxIntegrationTime
	HM5065_write_cmos_sensor(0x0144,0xCF);
	HM5065_write_cmos_sensor(0x01a0,0x03); // White Balance Mode	MANUAL
	HM5065_write_cmos_sensor(0x01a1,0x00); // ManualRedGain	0
	HM5065_write_cmos_sensor(0x01a2,0x00); // ManualGreenGain	0
	HM5065_write_cmos_sensor(0x01a3,0x00); // ManualBlueGain	0
	HM5065_write_cmos_sensor(0x01fa,0x00); // WhiteBalanceConstrainer	FALSE
	HM5065_write_cmos_sensor(0x015e,0x3e); // DigitalGainCeiling MSB	1.000
	HM5065_write_cmos_sensor(0x015f,0x00); // DigitalGainCeiling LSB
	HM5065_write_cmos_sensor(0x02a2,0x3e); // RedTilt MSB	1.000
	HM5065_write_cmos_sensor(0x02a3,0x00); // RedTilt LSB
	HM5065_write_cmos_sensor(0x02a4,0x3e); // Green1Tilt MSB	1.000
	HM5065_write_cmos_sensor(0x02a5,0x00); // Green1Tilt LSB
	HM5065_write_cmos_sensor(0x02a6,0x3e); // Green2Tilt MSB	1.000
	HM5065_write_cmos_sensor(0x02a7,0x00); // Green2Tilt LSB
	HM5065_write_cmos_sensor(0x02a8,0x3e); // BlueTilt MSB	1.000
	HM5065_write_cmos_sensor(0x02a9,0x00); // BlueTilt LSB
	HM5065_write_cmos_sensor(0x02aa,0x00); // BlackCorrectionOffset	0
	HM5065_write_cmos_sensor(0x02c2,0x00); // Analogue Gain Ceiling MSB	0
	HM5065_write_cmos_sensor(0x02c3,0x00); // Analogue Gain Ceiling LSB	0
	HM5065_write_cmos_sensor(0x0320,0x00); // fEnable	FALSE
	HM5065_write_cmos_sensor(0x0322,0x00); // AdaptiveAVEnable	FALSE
	HM5065_write_cmos_sensor(0x0323,0x01); // LoLightAVControlDisable	DamperOff
	HM5065_write_cmos_sensor(0x03E0,0x00); // Unity CCM
	HM5065_write_cmos_sensor(0x03E1,0x00);
	HM5065_write_cmos_sensor(0x03E2,0x00);
	HM5065_write_cmos_sensor(0x03E3,0x00);
	HM5065_write_cmos_sensor(0x03E4,0x00);
	HM5065_write_cmos_sensor(0x03E5,0x00);
	HM5065_write_cmos_sensor(0x03E6,0x00);
	HM5065_write_cmos_sensor(0x03E7,0x00);
	HM5065_write_cmos_sensor(0x03E8,0x00);
	HM5065_write_cmos_sensor(0x03E9,0x00);
	HM5065_write_cmos_sensor(0x03EA,0x00);
	HM5065_write_cmos_sensor(0x03EB,0x00);
	HM5065_write_cmos_sensor(0x03F0,0x00);
	HM5065_write_cmos_sensor(0x03F1,0x00);
	HM5065_write_cmos_sensor(0x03F2,0x00);
	HM5065_write_cmos_sensor(0x03F3,0x00);
	HM5065_write_cmos_sensor(0x03F4,0x00);
	HM5065_write_cmos_sensor(0x03F5,0x00);
	HM5065_write_cmos_sensor(0x03F6,0x00);
	HM5065_write_cmos_sensor(0x03F7,0x00);
	HM5065_write_cmos_sensor(0x03F8,0x00);
	HM5065_write_cmos_sensor(0x03F9,0x00);
	HM5065_write_cmos_sensor(0x03FA,0x00);
	HM5065_write_cmos_sensor(0x03FB,0x00);
	HM5065_write_cmos_sensor(0x0400,0x00);
	HM5065_write_cmos_sensor(0x0401,0x00);
	HM5065_write_cmos_sensor(0x0402,0x00);
	HM5065_write_cmos_sensor(0x0403,0x00);
	HM5065_write_cmos_sensor(0x0404,0x00);
	HM5065_write_cmos_sensor(0x0405,0x00);
	HM5065_write_cmos_sensor(0x0406,0x00);
	HM5065_write_cmos_sensor(0x0407,0x00);
	HM5065_write_cmos_sensor(0x0408,0x00);
	HM5065_write_cmos_sensor(0x0409,0x00);
	HM5065_write_cmos_sensor(0x040A,0x00);
	HM5065_write_cmos_sensor(0x040B,0x00);
	HM5065_write_cmos_sensor(0x0410,0x00);
	HM5065_write_cmos_sensor(0x0411,0x00);
	HM5065_write_cmos_sensor(0x0412,0x00);
	HM5065_write_cmos_sensor(0x0413,0x00);
	HM5065_write_cmos_sensor(0x0414,0x00);
	HM5065_write_cmos_sensor(0x0415,0x00);
	HM5065_write_cmos_sensor(0x0416,0x00);
	HM5065_write_cmos_sensor(0x0417,0x00);
	HM5065_write_cmos_sensor(0x0418,0x00);
	HM5065_write_cmos_sensor(0x0419,0x00);
	HM5065_write_cmos_sensor(0x041A,0x00);
	HM5065_write_cmos_sensor(0x041B,0x00);
//	HM5065_write_cmos_sensor(0x331E,0x00); // disable  VCM
}

static UINT16 wbsave = AWB_MODE_AUTO;
static UINT16 effectsave = MEFFECT_OFF;

static BOOL HM5065_set_param_wb(UINT16 para)
{
    kal_uint8  temp_reg;

    //temp_reg=HM5065_read_cmos_sensor(0x01A0);

		wbsave = para;
		
    switch (para)
    {
    //    case AWB_MODE_OFF:
		//	HM5065_write_cmos_sensor(0x01A4,0x04);
		//		HM5065_write_cmos_sensor(0x01A0,0x00);//20130114
     //       break;

        case AWB_MODE_AUTO:
			HM5065_write_cmos_sensor(0x01A0,0x01);					
            break;

        case AWB_MODE_CLOUDY_DAYLIGHT: //cloudy(13000K)
#if 0        	
			HM5065_write_cmos_sensor(0x01A0,0x03);//MWB
			HM5065_write_cmos_sensor(0x01A1,0xff);//Rgain
			HM5065_write_cmos_sensor(0x01A2,0x40);//Ggain
			HM5065_write_cmos_sensor(0x01A3,0x00);//Bgain
#else
			HM5065_write_cmos_sensor(0x01A0,0x03);//MWB
			HM5065_write_cmos_sensor(0x01A1,0xe0);//Rgain
			HM5065_write_cmos_sensor(0x01A2,0x40);//Ggain
			HM5065_write_cmos_sensor(0x01A3,0x00);//Bgain
#endif			
            break;

        case AWB_MODE_DAYLIGHT: //sunny(7000K)   
	#if 1  	
			HM5065_write_cmos_sensor(0x01A0,0x03);//MWB
			HM5065_write_cmos_sensor(0x01A1,0xC0);//Rgain
			HM5065_write_cmos_sensor(0x01A2,0x40);//Ggain
			HM5065_write_cmos_sensor(0x01A3,0x00);//Bgain
 #else
 			HM5065_AV_Calibration();
 #endif			
            break;

        case AWB_MODE_INCANDESCENT: //office(5000K)
			HM5065_write_cmos_sensor(0x01A0,0x03);//MWB
			HM5065_write_cmos_sensor(0x01A1,0xB0);//Rgain
			HM5065_write_cmos_sensor(0x01A2,0x40);//Ggain
			HM5065_write_cmos_sensor(0x01A3,0x20);//Bgain
            break;

        case AWB_MODE_TUNGSTEN: //home(2800K)
			HM5065_write_cmos_sensor(0x01A0,0x03);//MWB
			HM5065_write_cmos_sensor(0x01A1,0x90);//Rgain
			HM5065_write_cmos_sensor(0x01A2,0x40);//Ggain
			HM5065_write_cmos_sensor(0x01A3,0x40);//Bgain
            break;

        case AWB_MODE_FLUORESCENT://(4000K)
			HM5065_write_cmos_sensor(0x01A0,0x03);//MWB
			HM5065_write_cmos_sensor(0x01A1,0xA0);//Rgain
			HM5065_write_cmos_sensor(0x01A2,0x40);//Ggain
			HM5065_write_cmos_sensor(0x01A3,0x30);//Bgain
            break;

        default:
            return FALSE;
    }
    return TRUE;

} /* HM5065_set_param_wb */


static BOOL HM5065_set_param_effect(UINT16 para)
{
   	BOOL  ret = TRUE;
   	//UINT8  temp_reg;
   	//temp_reg=HM5065_read_cmos_sensor(0x3391);
   
   	effectsave = para;
   
    switch (para)
    {
        case MEFFECT_OFF:
			HM5065_write_cmos_sensor(0x0380,0x00);
			HM5065_write_cmos_sensor(0x0381,0x00);
			HM5065_write_cmos_sensor(0x0382,0x00);
			HM5065_write_cmos_sensor(0x0384,0x00);
				//add 20130114
			HM5065_write_cmos_sensor(0x01A0,0x01);
			HM5065_write_cmos_sensor(0x01A1,0x80);
			HM5065_write_cmos_sensor(0x01A2,0x80);
			HM5065_write_cmos_sensor(0x01A3,0x80);			
			HM5065_write_cmos_sensor(0x01A5,0x3E);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x3E);
			HM5065_write_cmos_sensor(0x01A8,0x00);								
            break;

        case MEFFECT_SEPIA:
			HM5065_write_cmos_sensor(0x0380,0x00);
			HM5065_write_cmos_sensor(0x0381,0x00);
			HM5065_write_cmos_sensor(0x0382,0x00);
			HM5065_write_cmos_sensor(0x0384,0x06);
					//add 20130114
			HM5065_write_cmos_sensor(0x01A0,0x01);
			HM5065_write_cmos_sensor(0x01A1,0x80);
			HM5065_write_cmos_sensor(0x01A2,0x80);
			HM5065_write_cmos_sensor(0x01A3,0x80);			
			HM5065_write_cmos_sensor(0x01A5,0x3E);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x3E);
			HM5065_write_cmos_sensor(0x01A8,0x00);		
            break;

        case MEFFECT_NEGATIVE:
			HM5065_write_cmos_sensor(0x0380,0x01);
			HM5065_write_cmos_sensor(0x0381,0x00);
			HM5065_write_cmos_sensor(0x0382,0x00);
			HM5065_write_cmos_sensor(0x0384,0x00);
			
				//add 20130114
			HM5065_write_cmos_sensor(0x01A0,0x01);
			HM5065_write_cmos_sensor(0x01A1,0x80);
			HM5065_write_cmos_sensor(0x01A2,0x80);
			HM5065_write_cmos_sensor(0x01A3,0x80);			
			HM5065_write_cmos_sensor(0x01A5,0x3E);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x3E);
			HM5065_write_cmos_sensor(0x01A8,0x00);			
            break;

        case MEFFECT_SEPIAGREEN:
			HM5065_write_cmos_sensor(0x0380,0x00);
			HM5065_write_cmos_sensor(0x0381,0x00);
			HM5065_write_cmos_sensor(0x0382,0x00);
			HM5065_write_cmos_sensor(0x0384,0x03);
			//add 20130114
			HM5065_write_cmos_sensor(0x01A0,0x00);
			HM5065_write_cmos_sensor(0x01A1,0x00);
			HM5065_write_cmos_sensor(0x01A2,0xFF);
			HM5065_write_cmos_sensor(0x01A3,0x00);			
			HM5065_write_cmos_sensor(0x01A5,0x00);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x42);
			HM5065_write_cmos_sensor(0x01A8,0x00);			
			
			
            break;

        case MEFFECT_SEPIABLUE:
			HM5065_write_cmos_sensor(0x0380,0x00);
			HM5065_write_cmos_sensor(0x0381,0x00);
			HM5065_write_cmos_sensor(0x0382,0x00);
			HM5065_write_cmos_sensor(0x0384,0x04);
			//add 20130114
			HM5065_write_cmos_sensor(0x01A0,0x00);
			HM5065_write_cmos_sensor(0x01A1,0x00);
			HM5065_write_cmos_sensor(0x01A2,0x00);
			HM5065_write_cmos_sensor(0x01A3,0xFF);			
			HM5065_write_cmos_sensor(0x01A5,0x3E);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x42);
			HM5065_write_cmos_sensor(0x01A8,0x00);			
			
			
            break;
	case MEFFECT_MONO: //B&W                                                                                                          
				HM5065_write_cmos_sensor(0x0380,0x00);																		  
				HM5065_write_cmos_sensor(0x0381,0x00);																		  
				HM5065_write_cmos_sensor(0x0382,0x00);																		  
				HM5065_write_cmos_sensor(0x0384,0x05);	
				//add 20130114
			HM5065_write_cmos_sensor(0x01A0,0x01);
			HM5065_write_cmos_sensor(0x01A1,0x80);
			HM5065_write_cmos_sensor(0x01A2,0x80);
			HM5065_write_cmos_sensor(0x01A3,0x80);			
			HM5065_write_cmos_sensor(0x01A5,0x3E);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x3E);
			HM5065_write_cmos_sensor(0x01A8,0x00);																					  
		break;

        default:
            ret = FALSE;
    }

    return ret;

} /* HM5065_set_param_effect */

static void HM5065_set_banding_for_50Hz(void)
{

		HM5065_write_cmos_sensor(0x019C, 0x4B);
		HM5065_write_cmos_sensor(0x019D, 0x20);
}


static void HM5065_set_banding_for_60Hz(void)
{
		HM5065_write_cmos_sensor(0x019C, 0x4B);
		HM5065_write_cmos_sensor(0x019D, 0xC0);

}

static BOOL HM5065_set_param_banding(UINT16 para)
{
    kal_uint8 banding;

    switch (para)
    {
        case AE_FLICKER_MODE_50HZ:

           HM5065_Sensor_Driver.bBanding_value = AE_FLICKER_MODE_50HZ;
           //HM5065_preview_pclk_in_M = 335;
		   
           if (HM5065_Sensor_Driver.MPEG4_encode_mode)
           {
			HM5065_write_cmos_sensor(0x019C, 0x4B);
			HM5065_write_cmos_sensor(0x019D, 0x20);
           }
           else
           {
		   HM5065_write_cmos_sensor(0x019C, 0x4B);
		   HM5065_write_cmos_sensor(0x019D, 0x20);
           }
            break;

        case AE_FLICKER_MODE_60HZ:
            //HM5065_preview_pclk_in_M = 390;
            HM5065_Sensor_Driver.bBanding_value = AE_FLICKER_MODE_60HZ;
			HM5065_write_cmos_sensor(0x019C, 0x4B);
			HM5065_write_cmos_sensor(0x019D, 0xC0);
            break;

          default:
		  	HM5065_write_cmos_sensor(0x019C, 0x4B);
			HM5065_write_cmos_sensor(0x019D, 0x20);
              return FALSE;
    }
    return TRUE;

} /* HM5065_set_param_banding */

static BOOL HM5065_set_param_exposure(UINT16 para)
{
    kal_uint8  temp_reg;

    //temp_reg=HM5065_read_cmos_sensor(0x0300);

    switch (para)
    {
        case AE_EV_COMP_n13:
           //	HM5065_write_cmos_sensor(0x0082, 0x1A);
           HM5065_write_cmos_sensor(0x0130, 0xF7);
            break;

        case AE_EV_COMP_n10:
           	//HM5065_write_cmos_sensor(0x0082, 0x2A);
           	HM5065_write_cmos_sensor(0x0130, 0xF8);
            break;

        case AE_EV_COMP_n07:
          // 	HM5065_write_cmos_sensor(0x0082, 0x3A);
          HM5065_write_cmos_sensor(0x0130, 0xF9);
		break;

        case AE_EV_COMP_n03:
           	//HM5065_write_cmos_sensor(0x0082, 0x4A);
           	HM5065_write_cmos_sensor(0x0130, 0xFD);
            break;

        case AE_EV_COMP_00:
           	//HM5065_write_cmos_sensor(0x0082, 0x5A);
           	HM5065_write_cmos_sensor(0x0130, 0x01);
            break;

        case AE_EV_COMP_03:
           //	HM5065_write_cmos_sensor(0x0082, 0x6A);
           HM5065_write_cmos_sensor(0x0130, 0x03);
            break;

        case AE_EV_COMP_07:
           	//HM5065_write_cmos_sensor(0x0082, 0x7A);
           	HM5065_write_cmos_sensor(0x0130, 0x05);
            break;

        case AE_EV_COMP_10:
          // 	HM5065_write_cmos_sensor(0x0082, 0x8A);
          HM5065_write_cmos_sensor(0x0130, 0x06);
            break;

        case AE_EV_COMP_13:
           //	HM5065_write_cmos_sensor(0x0082, 0x9A);
           HM5065_write_cmos_sensor(0x0130, 0x07);
            break;

        default:
            return FALSE;
    }
    return TRUE;

} /* HM5065_set_param_exposure */

static kal_uint32 HM5065_YUVSensorSetting(FEATURE_ID iCmd, UINT16 iPara)
{
/*
CMD: AE Strob(4)->AE Flick(0) -> AF Mode(6) -> AF Metring(7) -> AE metring(1)
-> EV(3) -> AWB(5) -> ISO(2) -> AE Scene Mode(13) ->Brightness(1) -> Hue(9)
-> Saturation(10) -> Edge(8) -> Contrast(12) -> Scene Mode(14) -> Effect(15)

For Current: Banding->EV->WB->Effect
*/
    //printk("[HM5065_YUVSensorSetting], Cmd = 0x%x, Para = 0x%x\n", iCmd, iPara); 
	//CAMERA_CONTROL_FLOW(iCmd,iPara);

	switch (iCmd) {
		case FID_SCENE_MODE:
                //printk("\n\nHM5065YUVSensorSetting:para=%d\n\n",iPara);
			
		    if (iPara == SCENE_MODE_OFF){
		        HM5065_night_mode(FALSE); 
		    }else if (iPara == SCENE_MODE_NIGHTSCENE){
               HM5065_night_mode(TRUE); 
		    }	    

		    break; 
		    		  		    
		case FID_AWB_MODE:
			HM5065_set_param_wb(iPara);
			if(AWB_MODE_AUTO==iPara) HM5065_set_param_effect(effectsave); // 20130116 Brandon // solve wb&effect conflict
		break;
		case FID_COLOR_EFFECT:
			HM5065_set_param_effect(iPara);
			if(MEFFECT_OFF==iPara) HM5065_set_param_wb(wbsave); // 20130116 Brandon // solve wb&effect conflict
		break;
		case FID_AE_EV:	
			HM5065_set_param_exposure(iPara);
		break;
		case FID_AE_FLICKER:
			HM5065_set_param_banding(iPara);
		break;
		default:
		break;
	}
	
	return TRUE;
}   /* HM5065_YUVSensorSetting */

static void HM5065_FOCUS_Get_AF_Inf(void);
static kal_uint32 HM5065_YUVSetVideoMode(UINT16 u2FrameRate)
{
		kal_uint8 iTemp;
		/* to fix VSYNC, to fix frame rate */
		//iTemp = HM5065_read_cmos_sensor(0x3014);
		//HM5065_write_cmos_sensor(0x3014, iTemp & 0xf7); //Disable night mode
		printk("HM5065YUVSetVideoMode.\n");
		HM5065_FOCUS_Get_AF_Inf();
		if (u2FrameRate == 30)
		{
		#if 1	
			HM5065_write_cmos_sensor(0x0040, 0x01); 	//	binning mode and subsampling mode for frame rate
			HM5065_write_cmos_sensor(0x0041, 0x0A); 	//	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
		#else
			HM5065_write_cmos_sensor(0x0040, 0x01); 	//	binning mode and subsampling mode for frame rate
			HM5065_write_cmos_sensor(0x0041, 0x04); 	//	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
		#endif	
			HM5065_write_cmos_sensor(0x0042, 0x05); 	//	X:800 0x500=1280,0x0320=800
			HM5065_write_cmos_sensor(0x0043, 0x00); 	//
			HM5065_write_cmos_sensor(0x0044, 0x03); 	//	Y:600 0x03c0=960,0x0258=600
			HM5065_write_cmos_sensor(0x0045, 0xC0); 	//
	
			HM5065_write_cmos_sensor(0x00E8, 0x00);
			HM5065_write_cmos_sensor(0x00C8, 0x00);
			HM5065_write_cmos_sensor(0x00C9, 0x1E);//30fps
			HM5065_write_cmos_sensor(0x00CA, 0x01);
	
	
		}
		else if (u2FrameRate == 15)
		{
		#if 1	
			HM5065_write_cmos_sensor(0x0040, 0x01); 	//	binning mode and subsampling mode for frame rate
			HM5065_write_cmos_sensor(0x0041, 0x0A); 	//	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
		#else
			HM5065_write_cmos_sensor(0x0040, 0x01); 	//	binning mode and subsampling mode for frame rate
			HM5065_write_cmos_sensor(0x0041, 0x04); 	//	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
		#endif	
			HM5065_write_cmos_sensor(0x0042, 0x05); 	//	X:800 0x500=1280,0x0320=800
			HM5065_write_cmos_sensor(0x0043, 0x00); 	//
			HM5065_write_cmos_sensor(0x0044, 0x03); 	//	Y:600 0x03c0=960,0x0258=600
			HM5065_write_cmos_sensor(0x0045, 0xC0); 	//
	
			HM5065_write_cmos_sensor(0x00E8, 0x00);
			HM5065_write_cmos_sensor(0x00C8, 0x00);
			HM5065_write_cmos_sensor(0x00C9, 0x0F);//15fps
			HM5065_write_cmos_sensor(0x00CA, 0x01);
	
		}
	
		HM5065_Sensor_Driver.MPEG4_encode_mode = KAL_TRUE;
    return TRUE;

}

static void HM5065_FOCUS_Constant_Focus()
{
    SENSORDB("MYCAT HM5065_FOCUS_Constant_Focus\n");
    //continue AF mode
 // mdelay(200);
 
	//HM5065_write_cmos_sensor(FACE_LC,0x00);//disable
	//mdelay(50);

    HM5065_write_cmos_sensor(0x070A,0x01);
	//mdelay(200);
   // SENSORDB("after constand focus  \n");

	//printk("[ysm]HM5065_FOCUS_Constant_Focus >>> read 0x070A : %x", HM5065_read_cmos_sensor(0x070A));
}

	
static void HM5065_FOCUS_Single_Focus()
{
	int i = 0;
	// Snapshot or Touch AF is trigger .
	HM5065_FOCUS_Set_AF_Window_to_IC() ;

    SENSORDB("HM5065_FOCUS_Single_Focus\n");
    HM5065_write_cmos_sensor(0x070B,0x01);
    HM5065_write_cmos_sensor(0x070A,0x03);
	  mdelay(200);
    HM5065_write_cmos_sensor(0x070B,0x02);
    SENSORDB("after single focus  \n");

  
	//else
   //HM5065_write_cmos_sensor(0x0714,0x00);	//disable zone sinagle 

 
}

static void HM5065_FOCUS_Cancel_Focus()
{
    SENSORDB("HM5065_FOCUS_Cancel_Focus\n");
	#if 1
	HM5065_write_cmos_sensor(0x070A,0x00);
	HM5065_write_cmos_sensor(0x0700,0x03);
	HM5065_write_cmos_sensor(0x0701,0xFF);
	HM5065_write_cmos_sensor(0x070C,0x00);
	HM5065_write_cmos_sensor(0x070C,0x07);
    #endif
	HM5065_write_cmos_sensor(FACE_LC,0x00);//disable
	mdelay(150);
}

// Step to Specified position
static UINT32 HM5065_FOCUS_AD5820_Move_to(UINT32 a_u2MovePosition)//??how many bits for ov3640??
{

    UINT8 state = 0x8F;
    UINT32 iteration = 100;
    UINT8 pos = (UINT8)a_u2MovePosition;
    UINT8 pos_temp=0x00;
	
    //3)checking status=S_IDLE?
    do{
        state = (UINT8)HM5065_read_cmos_sensor(0x07AE);
        printk("HM5065_FOCUS_AD5820_Move_to,CMD_TAG= %x\n",state);
        msleep(1);
        if(iteration-- == 0)
        {
             printk("HM5065_FOCUS_AD5820_Move_to,CMD_TAG check error!!!! %x\n",state);
		break;
        }

    }while(state!=0x00);

    af_pos_h= HM5065_read_cmos_sensor(0x06F0);
    af_pos_l= HM5065_read_cmos_sensor(0x06F1);
   printk(" Check current af position : %02x%02x. \n" , af_pos_h,af_pos_l);

    iteration = 100;
    do{
        state = (UINT8)HM5065_read_cmos_sensor(0x07AE);
        printk("HM5065_FOCUS_AD5820_Move_to,CMD_TAG= %x\n",state);
        msleep(1);
        if(iteration-- == 0)
        {
             printk("HM5065_FOCUS_AD5820_Move_to,CMD_TAG check error!!!! %x\n",state);
		break;
        }

    }while(state!=0x00);

    return 0;
}

static void HM5065_FOCUS_Get_AF_Status(UINT32 *para)
{
	kal_uint16 af_status=0;
	static kal_uint32 focused_cnt = 0;
	static kal_uint32 focusing_cnt = 0;
	struct timeval now;
	do_gettimeofday(&now);
	af_status = HM5065_read_cmos_sensor(0x07AE);//0x00:False 0x01:True

#if 1
	// if (af_error_flag == 1)
	// {
	// 	*para = SENSOR_AF_FOCUSED;
	// 	return;
	// }

	printk("HM5065 Get AF Status: %d.\n", af_status);
	if (0 == af_status)
	{
		focused_cnt = 0;
		if (af_type == AF_SINGLE)
		{
			//printk("brant single af mode read af status error.\n");
			if (now.tv_sec - tv.tv_sec > 2)
			{
				*para = SENSOR_AF_FOCUSED;
				// af_error_flag = 1;
			}
			else
			{
				// af_error_flag == 1;
				*para = SENSOR_AF_FOCUSING;
			}
		}
		else
		{
			// af_error_flag == 1;
			*para = SENSOR_AF_FOCUSING;
		}
	}
	else if (1 == af_status)
	{
		af_pos_h = HM5065_read_cmos_sensor(0x06F0);
		af_pos_l = HM5065_read_cmos_sensor(0x06F1);
		printk("HM5065 Get AF Statu read current af pos: %02x%02x.\n", af_pos_h, af_pos_l);

		focused_cnt++;
		if (focused_cnt > 10)
		{
			*para = SENSOR_AF_IDLE;
		}
		else
		{
			*para = SENSOR_AF_FOCUSED;
			printk("----HM5065 Get AF Status:SENSOR_AF_FOCUSED\n");
		}
	}
	else
	{
		*para = SENSOR_AF_ERROR;
	}
#else
	if (0 == af_status)
	{
		focusing_cnt++;
		if (focusing_cnt > 100)
		{
			*para = SENSOR_AF_FOCUSED;
			focused_cnt++;
		}
		else
		{
			*para = SENSOR_AF_FOCUSING;
		}
	}
	else if (1 == af_status)
	{
		*para = SENSOR_AF_FOCUSED;
	}
	else
	{
		*para = SENSOR_AF_ERROR;
	}
#endif
}
static void HM5065_FOCUS_Get_AF_Inf()
{
    SENSORDB("HM5065_FOCUS_Get_AF_Inf\n");
    // HM5065_write_cmos_sensor(0x0700,0x00);
    // HM5065_write_cmos_sensor(0x0701,0x05);
    // HM5065_write_cmos_sensor(0x070C,0x00);
	// HM5065_write_cmos_sensor(0x070C,0x07);
	HM5065_write_cmos_sensor(0x070A,0x00);
    HM5065_write_cmos_sensor(0x0700,0x03);
    HM5065_write_cmos_sensor(0x0701,0xFF);
	HM5065_write_cmos_sensor(0x070C,0x00);
    HM5065_write_cmos_sensor(0x070C,0x07);
	//printk("HM5065_FOCUS_Get_AF_Inf: AF POS %02x%02x.\n", HM5065_read_cmos_sensor(0x0700), HM5065_read_cmos_sensor
		//	(0x0701));

}

static void HM5065_FOCUS_Get_AF_Macro()
{
    SENSORDB("HM5065_FOCUS_Get_AF_Macro\n");
    // HM5065_write_cmos_sensor(0x0700,0x03);
    // HM5065_write_cmos_sensor(0x0701,0xF5);
    // HM5065_write_cmos_sensor(0x070C,0x00);
	// HM5065_write_cmos_sensor(0x070C,0x07);
	HM5065_write_cmos_sensor(0x070A,0x00);
    HM5065_write_cmos_sensor(0x0700,0x00);
    HM5065_write_cmos_sensor(0x0701,0x05);
		HM5065_write_cmos_sensor(0x070C,0x00);
    HM5065_write_cmos_sensor(0x070C,0x07);
}

static void HM5065_FOCUS_Set_AF_Window(UINT32 zone_addr)
{//update global zone

    //input:
    UINT16 times = 1;
    UINT16 FD_XS = 4;
    UINT16 FD_YS = 3;	
    UINT16 x0, y0, x1, y1;
    UINT32* zone = (UINT32*)zone_addr;
    x0 = *zone;
    y0 = *(zone + 1);
    x1 = *(zone + 2);
    y1 = *(zone + 3);
	
    FD_XS = *(zone + 4);
    FD_YS = *(zone + 5);
   times = FD_XS / AF_XS;
   
//printk("HM5065_FOCUS_Set_AF_Window\n");
   // SENSORDB("x0=%d,y0=%d,x1=%d,y1=%d,FD_XS=%d,FD_YS=%d,times=%d\n",
	//	x0, y0, x1, y1, FD_XS, FD_YS,times);	

//zone changed, update global zone.
//    ZONE[0] = x0;//(UINT8)(x0 / times);   
   // ZONE[1] = y0;//(UINT8)(y0 / times); 
   // ZONE[2]= (UINT8)(x1 / times);  
   // ZONE[3] = (UINT8)(y1 / times);  
    //SENSORDB("zone_x0=%d,zone_y0=%d,zone_x1=%d,zone_y1=%d,FD_XS=%d,FD_YS=%d\n",
       // ZONE[0], ZONE[1], ZONE[2], ZONE[3], FD_XS, FD_YS);  
XSTART=x0*4;
YSTART=y0*4;	
SENSORDB("HM5065_FOCUS_Set_AF_Window ----- XSTART=%d,YSTART=%d,\n",XSTART,YSTART);	

}

static void HM5065_FOCUS_Set_AF_Window_to_IC(void)
{
	
		UINT16 PBASEX=0,PBASEY=0,POFFSETX,POFFSETY;
    	   //return ;
    //define touch AF Zone 
    
	  HM5065_write_cmos_sensor(0x0808,0x01);
	  HM5065_write_cmos_sensor(0x0809,0x00); 
	  HM5065_write_cmos_sensor(0x080a,0x00);
	  HM5065_write_cmos_sensor(0x080b,0x00);  
 	  HM5065_write_cmos_sensor(0x080c,0x00);
	  HM5065_write_cmos_sensor(0x080d,0x00);
	  HM5065_write_cmos_sensor(0x080e,0x00); 
	  
	  HM5065_write_cmos_sensor(FACE_LC,0x01);//enable
	  
		//XSTART =PBASEX+POFFSETX*1.6 ;  //Mapping(FACE_START_XH ,FACE_START_XL)
	 HM5065_write_cmos_sensor(FACE_START_XH, XSTART>>8);
	 HM5065_write_cmos_sensor(FACE_START_XL, XSTART&0xff);
	 
	 //HM5065_write_cmos_sensor(FACE_START_XH, 0x02);
	// HM5065_write_cmos_sensor(FACE_START_XL, 0x80);
			
			
		//YSTART = PBASEY+POFFSETY*2 ;  //Mapping(FACE_START_XH ,FACE_START_XL)	
	HM5065_write_cmos_sensor(FACE_START_YH, YSTART>>8);
	HM5065_write_cmos_sensor(FACE_START_YL, YSTART&0xff);		
	
	//HM5065_write_cmos_sensor(FACE_START_YH, 0x01);
	//HM5065_write_cmos_sensor(FACE_START_YL, 0xe0);	
		
		SENSORDB("HM5065_FOCUS_Set_AF_Window_to_IC\n");

		HM5065_write_cmos_sensor(FACE_SIZE_XH, 0x01);
		HM5065_write_cmos_sensor(FACE_SIZE_XL, 0x40);
		HM5065_write_cmos_sensor(FACE_SIZE_YH, 0x01);
		HM5065_write_cmos_sensor(FACE_SIZE_YL, 0x40);
    return;

}
UINT32 HM5065SetSoftwarePWDNMode(kal_bool bEnable)
{
    return TRUE;

}

/*************************************************************************
* FUNCTION
*    HM5065_get_size
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
static void HM5065_get_size(kal_uint16 *sensor_width, kal_uint16 *sensor_height)
{
  *sensor_width = IMAGE_SENSOR_FULL_WIDTH; /* must be 4:3 */
  *sensor_height = IMAGE_SENSOR_FULL_HEIGHT;
}

/*************************************************************************
* FUNCTION
*    HM5065_get_period
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
static void HM5065_get_period(kal_uint16 *pixel_number, kal_uint16 *line_number)
{
  *pixel_number = VGA_PERIOD_PIXEL_NUMS+HM5065_Sensor_Driver.dummy_pixels;
  *line_number = VGA_PERIOD_LINE_NUMS+HM5065_Sensor_Driver.dummy_lines;
}

void HM5065YUVGetAFMaxNumFocusAreas(UINT32 *pFeatureData32)
{
	*pFeatureData32 = 5 ;
	printk(" HM5065YUVGetAFMaxNumFocusAreas , pFeatureReturnData32 = %d\n " ,  *pFeatureData32 );
}

void HM5065YUVGetAEMaxNumMeteringAreas(UINT32 *pFeatureData32)
{
	*pFeatureData32 = 1 ;
	printk("HM5065YUVGetAEMaxNumMeteringAreas ,   pFeatureReturnData32 = %d\n" ,  *pFeatureData32);
}
/*************************************************************************
* FUNCTION
*    HM5065_feature_control
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
static kal_uint32 HM5065FeatureControl(MSDK_SENSOR_FEATURE_ENUM id, kal_uint8 *para, kal_uint32 *len)
{

    UINT16 *pFeatureReturnPara16=(UINT16 *) para;
    UINT16 *pFeatureData16=(UINT16 *) para;
    UINT32 *pFeatureReturnPara32=(UINT32 *) para;
    UINT32 *pFeatureData32=(UINT32 *) para;

	
	if((id!=3000)&&(id!=3004)&&(id!=3006)){
	    //CAMERA_CONTROL_FLOW(id,id);
	}
		//SENSORDB("HM5065FeatureControl\n");
	switch (id)
	{
		case SENSOR_FEATURE_GET_RESOLUTION: /* no use */
			HM5065_get_size((kal_uint16 *)para, (kal_uint16 *)(para + sizeof(kal_uint16)));
			*len = sizeof(kal_uint32);
			break;
		case SENSOR_FEATURE_GET_PERIOD:
			HM5065_get_period((kal_uint16 *)para, (kal_uint16 *)(para + sizeof(kal_uint16)));
			*len = sizeof(kal_uint32);
			break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			*(kal_uint32 *)para = HM5065_Sensor_Driver.fPV_PCLK;
			*len = sizeof(kal_uint32);
			break;
		case SENSOR_FEATURE_SET_ESHUTTER:
			break;
		case SENSOR_FEATURE_SET_NIGHTMODE: 
			HM5065_night_mode((kal_bool)*(kal_uint16 *)para);
			break;
		case SENSOR_FEATURE_SET_GAIN:
		case SENSOR_FEATURE_SET_FLASHLIGHT:
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			break;
		case SENSOR_FEATURE_SET_REGISTER:
			HM5065_write_cmos_sensor(((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegAddr, ((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegData);
			break;
		case SENSOR_FEATURE_GET_REGISTER: /* 10 */
			((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegData = HM5065_read_cmos_sensor(((MSDK_SENSOR_REG_INFO_STRUCT *)para)->RegAddr);
			break;
		case SENSOR_FEATURE_SET_CCT_REGISTER:
			memcpy(&HM5065_Sensor_Driver.eng.CCT, para, sizeof(HM5065_Sensor_Driver.eng.CCT));
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
	//		HM5065_YUVSensorSetting((FEATURE_ID)(UINT32 *)para, (UINT32 *)(para+1));
			
			HM5065_YUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
			break;
#if 0		    		
		case SENSOR_FEATURE_QUERY:
			HM5065_Query(pSensorFeatureInfo);
			*pFeatureParaLen = sizeof(MSDK_FEATURE_INFO_STRUCT);
			break;		
		case SENSOR_FEATURE_SET_YUV_CAPTURE_RAW_SUPPORT:
			/* update yuv capture raw support flag by *pFeatureData16 */
			break;		
#endif 			
		case SENSOR_FEATURE_SET_VIDEO_MODE:
			HM5065_YUVSetVideoMode(*para);
			break;
              case SENSOR_FEATURE_CHECK_SENSOR_ID:
                     HM5065_GetSensorID(pFeatureData32); 
                     break; 	
              case SENSOR_FEATURE_SET_SOFTWARE_PWDN:
                     HM5065SetSoftwarePWDNMode((BOOL)*pFeatureData32);        	        	
                     break;

		case SENSOR_FEATURE_INITIALIZE_AF:
			printk("FCtrl : FEATURE_INITIALIZE_AF\n");
			break;
        case SENSOR_FEATURE_MOVE_FOCUS_LENS:
            HM5065_FOCUS_AD5820_Move_to(*pFeatureData16);
            break;
			
		case SENSOR_FEATURE_GET_AF_STATUS:
			// FUNC();
			HM5065_FOCUS_Get_AF_Status(pFeatureData32);
			*len = 4;
			break;
		case SENSOR_FEATURE_SINGLE_FOCUS_MODE:
			af_type = AF_SINGLE;
			// af_error_flag = 0;
			do_gettimeofday(&tv);
			HM5065_FOCUS_Single_Focus();
			break;
		case SENSOR_FEATURE_CONSTANT_AF:
			af_type = AF_CONSTANT;
			HM5065_FOCUS_Constant_Focus();
			break;
		case SENSOR_FEATURE_CANCEL_AF:
			printk("HM5065_FOCUS_CANCEL_AF\n");
			// af_error_flag = 0;
			af_type = AF_CANCEL;
			HM5065_FOCUS_Cancel_Focus();
			break;
		case SENSOR_FEATURE_GET_AF_INF:
			af_type = AF_INF;
			printk("HM5065_FOCUS_INF_AF\n");
			HM5065_FOCUS_Get_AF_Inf();
			*pFeatureData32 = 0;
			*len = 4;
			break;
		case SENSOR_FEATURE_GET_AF_MACRO:
			HM5065_FOCUS_Get_AF_Macro();
			*pFeatureData32 = 1023;
			*len = 4;
			break;
		case SENSOR_FEATURE_SET_AF_WINDOW:
			printk("FEATURE_SET_AF_WINDOW\n");			
			HM5065_FOCUS_Set_AF_Window(*pFeatureData32);
			*len = 4 ;
			break;
		case SENSOR_FEATURE_GET_AF_MAX_NUM_FOCUS_AREAS:
			printk("FEATURE_GET_AF_MAX_NUM_FOCUS_AREAS\n");						
			HM5065YUVGetAFMaxNumFocusAreas(pFeatureData32);
			*len = 4 ;
			break;
		case SENSOR_FEATURE_GET_AE_MAX_NUM_METERING_AREAS:
			printk("	FEATURE_GET_AE_MAX_NUM_METERING_AREAS\n");
			HM5065YUVGetAEMaxNumMeteringAreas(pFeatureData32);
			*len = 4 ;
			break;			
        case SENSOR_FEATURE_SET_AE_WINDOW:
            SENSORDB("SENSOR_FEATURE_SET_AE_WINDOW\n");
            SENSORDB("AE zone addr = 0x%x\n",*pFeatureData32);			
            HM5065_FOCUS_AD5820_Set_AE_Window(*pFeatureData32);
            break;  
			
		default:
			break;
	}
	return ERROR_NONE;
}




#if 0
image_sensor_func_struct image_sensor_driver_HM5065=
{
	HM5065Open,
	HM5065Close,
	HM5065GetResolution,
	HM5065GetInfo,
	HM5065Control,
	HM5065FeatureControl
};
void image_sensor_func_config(void)
{
	extern image_sensor_func_struct *image_sensor_driver;

	image_sensor_driver = &image_sensor_driver_HM5065;
}

#endif

SENSOR_FUNCTION_STRUCT	SensorFuncHM5065=
{
	HM5065Open,
	HM5065GetInfo,
	HM5065GetResolution,
	HM5065FeatureControl,
	HM5065Control,
	HM5065Close
};

UINT32 HM5065_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&SensorFuncHM5065;

	return ERROR_NONE;
}	/* SensorInit() */



