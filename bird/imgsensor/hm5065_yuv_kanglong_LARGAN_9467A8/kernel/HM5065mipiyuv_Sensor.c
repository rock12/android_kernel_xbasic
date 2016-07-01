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
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   sensor.c
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Source code of Sensor driver
 * Author:
 * -------
 *   PC Huang (MTK02204)
 *
 *============================================================================
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
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
#include <asm/system.h>	 
#include "kd_camera_hw.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"
#include "HM5065mipiyuv_Sensor.h"
#include "HM5065mipiyuv_Camera_Sensor_para.h"
#include "HM5065mipiyuv_CameraCustomized.h" 
#define HM5065MIPIYUV_DEBUG
#ifdef HM5065MIPIYUV_DEBUG
#define HM5065MIPISENSORDB printk
#else
#define HM5065MIPISENSORDB(x,...)
#endif
#define AE_and_AF_FACE
static DEFINE_SPINLOCK(HM5065mipi_drv_lock);
static MSDK_SCENARIO_ID_ENUM CurrentScenarioId = MSDK_SCENARIO_ID_CAMERA_PREVIEW;
static kal_uint8 af_pos_h = 0;
static kal_uint8 af_pos_l = 0;


extern int iReadReg(u16 a_u2Addr , u8 * a_puBuff , u16 i2cId);
extern int iWriteReg(u16 a_u2Addr , u32 a_u4Data , u32 a_u4Bytes , u16 i2cId);
#define HM5065MIPI_write_cmos_sensor(addr, para) iWriteReg((u16) addr , (u32) para ,1,HM5065MIPI_WRITE_ID)
#define mDELAY(ms)  mdelay(ms)
typedef enum
{
    PRV_W=1280,
    PRV_H=960,
		
	ZSD_PRV_W=2592,
	ZSD_PRV_H=1944
}PREVIEW_VIEW_SIZE;
kal_uint16 HM5065MIPI_read_cmos_sensor(kal_uint32 addr)
{
	kal_uint16 get_byte=0;
    iReadReg((u16) addr ,(u8*)&get_byte,HM5065MIPI_WRITE_ID);
    return get_byte;
}

static struct
{
	//kal_uint8   Banding;
	kal_bool	  NightMode;
	kal_bool	  VideoMode;
	kal_uint16  Fps;
	kal_uint16  ShutterStep;
	kal_uint8   IsPVmode;
	kal_uint32  PreviewDummyPixels;
	kal_uint32  PreviewDummyLines;
	kal_uint32  CaptureDummyPixels;
	kal_uint32  CaptureDummyLines;
	kal_uint32  PreviewPclk;
	kal_uint32  CapturePclk;
	kal_uint32  ZsdturePclk;
	kal_uint32  PreviewShutter;
	kal_uint32  PreviewGain;
	
	kal_uint32  PreviewExtraShutter;
	kal_uint32  SensorGain;
	kal_bool    	manualAEStart;
	kal_bool    	userAskAeLock;
    kal_bool    	userAskAwbLock;
	kal_uint32      currentExposureTime;
    kal_uint32      currentShutter;
    kal_uint32      previewShutter;
	kal_uint32      currentextshutter;
    kal_uint32      currentAxDGain;
	kal_uint32  	sceneMode;
    unsigned char isoSpeed;
	unsigned char zsd_flag;
	HM5065MIPI_SENSOR_MODE SensorMode;

	UINT16 wb;
} HM5065MIPISensor;
/* Global Valuable */
static kal_uint32 zoom_factor = 0; 
static kal_int8 HM5065MIPI_DELAY_AFTER_PREVIEW = -1;
static kal_uint8 HM5065MIPI_Banding_setting = AE_FLICKER_MODE_50HZ; 
static kal_bool HM5065MIPI_AWB_ENABLE = KAL_TRUE; 
static kal_bool HM5065MIPI_AE_ENABLE = KAL_TRUE; 
MSDK_SENSOR_CONFIG_STRUCT HM5065MIPISensorConfigData;
#define HM5065_TEST_PATTERN_CHECKSUM (0x7ba87eae)
kal_bool HM5065_run_test_pattern=0;

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
static UINT32 line_coordinate[AE_VERTICAL_BLOCKS_MAX] = {0};//line[0]=0      line[1]=160     line[2]=320     line[3]=480     line[4]=640
static UINT32 row_coordinate[AE_HORIZONTAL_BLOCKS_MAX] = {0};//line[0]=0       line[1]=120     line[2]=240     line[3]=360     line[4]=480
static BOOL AE_1_ARRAY[AE_SECTION_INDEX_MAX] = {FALSE};
static BOOL AE_2_ARRAY[AE_HORIZONTAL_BLOCKS][AE_VERTICAL_BLOCKS] = {{FALSE},{FALSE},{FALSE},{FALSE}};//how to ....
//=====================touch AE begin==========================//
void HM5065_writeAEReg(void)
{	
}


void HM5065_printAE_1_ARRAY(void)
{
    UINT32 i;
    for(i=0; i<AE_SECTION_INDEX_MAX; i++)
    {
        HM5065MIPISENSORDB("AE_1_ARRAY[%2d]=%d\n", i, AE_1_ARRAY[i]);
    }
}

void HM5065_printAE_2_ARRAY(void)
{
    UINT32 i, j;
    HM5065MIPISENSORDB("\t\t");
    for(i=0; i<AE_VERTICAL_BLOCKS; i++)
    {
        HM5065MIPISENSORDB("      line[%2d]", i);
    }
    printk("\n");
    for(j=0; j<AE_HORIZONTAL_BLOCKS; j++)
    {
        HM5065MIPISENSORDB("\trow[%2d]", j);
        for(i=0; i<AE_VERTICAL_BLOCKS; i++)
        {
            //SENSORDB("AE_2_ARRAY[%2d][%2d]=%d\n", j,i,AE_2_ARRAY[j][i]);
            HM5065MIPISENSORDB("  %7d", AE_2_ARRAY[j][i]);
        }
        HM5065MIPISENSORDB("\n");
    }
}

void HM5065_clearAE_2_ARRAY(void)
{
    UINT32 i, j;
    for(j=0; j<AE_HORIZONTAL_BLOCKS; j++)
    {
        for(i=0; i<AE_VERTICAL_BLOCKS; i++)
        {AE_2_ARRAY[j][i]=FALSE;}
    }
}

void HM5065_mapAE_2_ARRAY_To_AE_1_ARRAY(void)
{
    UINT32 i, j;
    for(j=0; j<AE_HORIZONTAL_BLOCKS; j++)
    {
        for(i=0; i<AE_VERTICAL_BLOCKS; i++)
        { AE_1_ARRAY[j*AE_VERTICAL_BLOCKS+i] = AE_2_ARRAY[j][i];}
    }
}

void HM5065_mapMiddlewaresizePointToPreviewsizePoint(
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
    HM5065MIPISENSORDB("mapping middlware x[%d],y[%d], [%d X %d]\n\t\tto x[%d],y[%d],[%d X %d]\n ",
        mx, my, mw, mh, *pvx, *pvy, pvw, pvh);
}


void HM5065_calcLine(void)
{//line[5]
    UINT32 i;
    UINT32 step = PRV_W / AE_VERTICAL_BLOCKS;
    for(i=0; i<=AE_VERTICAL_BLOCKS; i++)
    {
        *(&line_coordinate[0]+i) = step*i;
        HM5065MIPISENSORDB("line[%d]=%d\t",i, *(&line_coordinate[0]+i));
    }
    HM5065MIPISENSORDB("\n");
}

void HM5065_vcalcRow(void)
{//row[5]
    UINT32 i;
    UINT32 step = PRV_H / AE_HORIZONTAL_BLOCKS;
    for(i=0; i<=AE_HORIZONTAL_BLOCKS; i++)
    {
        *(&row_coordinate[0]+i) = step*i;
        HM5065MIPISENSORDB("row[%d]=%d\t",i,*(&row_coordinate[0]+i));
    }
    HM5065MIPISENSORDB("\n");
}

void HM5065_calcPointsAELineRowCoordinate(UINT32 x, UINT32 y, UINT32 * linenum, UINT32 * rownum)
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
    HM5065MIPISENSORDB("PV point [%d, %d] to section line coordinate[%d] row[%d]\n",x,y,*linenum,*rownum);
}



MINT32 HM5065_clampSection(UINT32 x, UINT32 min, UINT32 max)
{
    if (x > max) return max;
    if (x < min) return min;
    return x;
}

void HM5065_mapCoordinate(UINT32 linenum, UINT32 rownum, UINT32 * sectionlinenum, UINT32 * sectionrownum)
{
    *sectionlinenum = HM5065_clampSection(linenum-1,0,AE_VERTICAL_BLOCKS-1);
    *sectionrownum = HM5065_clampSection(rownum-1,0,AE_HORIZONTAL_BLOCKS-1);	
    HM5065MIPISENSORDB("mapCoordinate from[%d][%d] to[%d][%d]\n",
		linenum, rownum,*sectionlinenum,*sectionrownum);
}

void HM5065_mapRectToAE_2_ARRAY(UINT32 x0, UINT32 y0, UINT32 x1, UINT32 y1)
{
    UINT32 i, j;
    HM5065MIPISENSORDB("([%d][%d]),([%d][%d])\n", x0,y0,x1,y1);
    HM5065_clearAE_2_ARRAY();
    x0=HM5065_clampSection(x0,0,AE_VERTICAL_BLOCKS-1);
    y0=HM5065_clampSection(y0,0,AE_HORIZONTAL_BLOCKS-1);
    x1=HM5065_clampSection(x1,0,AE_VERTICAL_BLOCKS-1);
    y1=HM5065_clampSection(y1,0,AE_HORIZONTAL_BLOCKS-1);

    for(j=y0; j<=y1; j++)
    {
        for(i=x0; i<=x1; i++)
        {
            AE_2_ARRAY[j][i]=TRUE;
        }
    }
}

void HM5065_resetPVAE_2_ARRAY(void)
{
    HM5065_mapRectToAE_2_ARRAY(1,1,2,2);
}

//update ae window
//@input zone[] addr
void HM5065_FOCUS_Set_AE_Window(UINT32 zone_addr)
{//update global zone
  #if 1
  	  HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065_FOCUS_Set_AE_Window function:\n ");
	  UINT32 FD_XS;
	  UINT32 FD_YS;   
	  UINT32 x0, y0, x1, y1;
	  UINT32 pvx0, pvy0, pvx1, pvy1;
	  UINT32 linenum, rownum;
	  UINT32 AF_pvx, AF_pvy;
	  UINT32* zone = (UINT32*)zone_addr;
	  x0 = *zone;
	  y0 = *(zone + 1);
	  x1 = *(zone + 2);
	  y1 = *(zone + 3);   
	  FD_XS = *(zone + 4);
	  FD_YS = *(zone + 5);
  //AE 0x714
	  HM5065MIPISENSORDB("Set_AE_Window x0=%d,y0=%d,x1=%d,y1=%d,FD_XS=%d,FD_YS=%d\n",
	  x0, y0, x1, y1, FD_XS, FD_YS);  
  
	  if(CurrentScenarioId == MSDK_SCENARIO_ID_CAMERA_ZSD )
	  {
		  HM5065_mapMiddlewaresizePointToPreviewsizePoint(x0,y0,FD_XS,FD_YS,&pvx0, &pvy0, ZSD_PRV_W, ZSD_PRV_H);
		  HM5065_mapMiddlewaresizePointToPreviewsizePoint(x1,y1,FD_XS,FD_YS,&pvx1, &pvy1, ZSD_PRV_W, ZSD_PRV_H);  
	  }  
	  else
	  {
		  HM5065_mapMiddlewaresizePointToPreviewsizePoint(x0,y0,FD_XS,FD_YS,&pvx0, &pvy0, PRV_W, PRV_H);
		  HM5065_mapMiddlewaresizePointToPreviewsizePoint(x1,y1,FD_XS,FD_YS,&pvx1, &pvy1, PRV_W, PRV_H);  
	  }
		  
	  HM5065MIPISENSORDB("[HM5065MIPI]AE pvx0=%d,pvy0=%d\n",pvx0, pvy0);
	  HM5065MIPISENSORDB("[HM5065MIPI]AE pvx1=%d,pvy1=%d\n",pvx1, pvy1);
  //ndef AE_and_AF_FACE
	  if((pvx0==pvx1)&&(pvy0==pvy1))
	  {
	   if(CurrentScenarioId == MSDK_SCENARIO_ID_CAMERA_ZSD )
	   	{
	   	  HM5065MIPI_write_cmos_sensor(0x0120,0x01);
		  HM5065MIPI_write_cmos_sensor(0x0121,0x04); 
		  HM5065MIPI_write_cmos_sensor(0x0122,0x00);
		  HM5065MIPI_write_cmos_sensor(0x0123,0xC4);  
		  HM5065MIPI_write_cmos_sensor(0x0124,0x02);
		  HM5065MIPI_write_cmos_sensor(0x0125,0x09);
		  HM5065MIPI_write_cmos_sensor(0x0126,0x01); 
		  HM5065MIPI_write_cmos_sensor(0x0127,0x88);
		  
		  HM5065MIPISENSORDB("HM5065_FOCUS_Set_ZSDAE_defalt_Window_to_IC\n");
	   	 }
	   else
	   	{
		  HM5065MIPI_write_cmos_sensor(0x0120,0x00);
		  HM5065MIPI_write_cmos_sensor(0x0121,0x82); 
		  HM5065MIPI_write_cmos_sensor(0x0122,0x00);
		  HM5065MIPI_write_cmos_sensor(0x0123,0x62);  
		  HM5065MIPI_write_cmos_sensor(0x0124,0x01);
		  HM5065MIPI_write_cmos_sensor(0x0125,0x04);
		  HM5065MIPI_write_cmos_sensor(0x0126,0x00); 		
		  HM5065MIPI_write_cmos_sensor(0x0127,0xC4);
		  
		  HM5065MIPISENSORDB("HM5065_FOCUS_Set_preview AE_defalt_Window_to_IC\n");
	   	}
	  }
	  else
	  {
		  HM5065MIPI_write_cmos_sensor(0x0120,pvx0>>8);
		  HM5065MIPI_write_cmos_sensor(0x0121,pvx0&0xff); 
		  HM5065MIPI_write_cmos_sensor(0x0122,pvy0>>8);
		  HM5065MIPI_write_cmos_sensor(0x0123,pvy0&0xff);  
		  HM5065MIPI_write_cmos_sensor(0x0124,((pvx1-pvx0)/4)>>8);
		  HM5065MIPI_write_cmos_sensor(0x0125,((pvx1-pvx0)/4)&0xff);
		  HM5065MIPI_write_cmos_sensor(0x0126,((pvy1-pvy0)/4)>>8);		
		  HM5065MIPI_write_cmos_sensor(0x0127,((pvy1-pvy0)/4)&0xff);
  
	 	  HM5065MIPISENSORDB("HM5065_FOCUS_Set_AE_Touch Window_to_IC\n");
   
	 	  HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065_FOCUS_Set_AE_Window function:\n ");
	  }  
#endif
}
//=====================touch AE end==========================//
/*************************************************************************
* FUNCTION
*	HM5065MIPI_set_dummy
*
* DESCRIPTION
*	This function set the dummy pixels(Horizontal Blanking) & dummy lines(Vertical Blanking), it can be
*	used to adjust the frame rate or gain more time for back-end process.
*	
*	IMPORTANT NOTICE: the base shutter need re-calculate for some sensor, or else flicker may occur.
*
* PARAMETERS
*	1. kal_uint32 : Dummy Pixels (Horizontal Blanking)
*	2. kal_uint32 : Dummy Lines (Vertical Blanking)
*
* RETURNS
*	None
*
*************************************************************************/
static void HM5065MIPIinitalvariable()
{
	spin_lock(&HM5065mipi_drv_lock);
	HM5065MIPISensor.VideoMode = KAL_FALSE;
	HM5065MIPISensor.NightMode = KAL_FALSE;
	HM5065MIPISensor.Fps = 100;
	HM5065MIPISensor.ShutterStep= 0xde;
	HM5065MIPISensor.CaptureDummyPixels = 0;
	HM5065MIPISensor.CaptureDummyLines = 0;
	HM5065MIPISensor.PreviewDummyPixels = 0;
	HM5065MIPISensor.PreviewDummyLines = 0;
	HM5065MIPISensor.SensorMode= SENSOR_MODE_INIT;
	HM5065MIPISensor.IsPVmode= KAL_TRUE;	
	HM5065MIPISensor.PreviewPclk= 560;
	HM5065MIPISensor.CapturePclk= 800;
	HM5065MIPISensor.ZsdturePclk= 800;
	
	HM5065MIPISensor.PreviewShutter=0x0375; //0375
	HM5065MIPISensor.PreviewGain = 0x20;
	
	HM5065MIPISensor.PreviewExtraShutter=0x00; 
	HM5065MIPISensor.SensorGain=0x10;
	HM5065MIPISensor.manualAEStart=0;
	HM5065MIPISensor.isoSpeed=AE_ISO_100;
	HM5065MIPISensor.userAskAeLock=KAL_FALSE;
    HM5065MIPISensor.userAskAwbLock=KAL_FALSE;
	HM5065MIPISensor.currentExposureTime=0;
    HM5065MIPISensor.currentShutter=0;
	HM5065MIPISensor.zsd_flag=0;
	HM5065MIPISensor.currentextshutter=0;

	spin_unlock(&HM5065mipi_drv_lock);
}
void HM5065MIPIGetExifInfo(UINT32 exifAddr)
{
    SENSOR_EXIF_INFO_STRUCT* pExifInfo = (SENSOR_EXIF_INFO_STRUCT*)exifAddr;
    pExifInfo->FNumber = 20;
    pExifInfo->AEISOSpeed = HM5065MIPISensor.isoSpeed;
    pExifInfo->FlashLightTimeus = 0;
    pExifInfo->RealISOValue = HM5065MIPISensor.isoSpeed;
}
static void HM5065MIPISetDummy(kal_uint32 dummy_pixels, kal_uint32 dummy_lines)
{
		HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPISetDummy function:\n ");
		if (HM5065MIPISensor.IsPVmode)  
        {
            dummy_pixels = dummy_pixels+HM5065MIPI_PV_PERIOD_PIXEL_NUMS; 
            //HM5065MIPI_write_cmos_sensor(0x380D,( dummy_pixels&0xFF));         
            //HM5065MIPI_write_cmos_sensor(0x380C,(( dummy_pixels&0xFF00)>>8)); 
      
            dummy_lines= dummy_lines+HM5065MIPI_PV_PERIOD_LINE_NUMS; 
            //HM5065MIPI_write_cmos_sensor(0x380F,(dummy_lines&0xFF));       
            //HM5065MIPI_write_cmos_sensor(0x380E,((dummy_lines&0xFF00)>>8));  
        } 
        else
        {
            dummy_pixels = dummy_pixels+HM5065MIPI_FULL_PERIOD_PIXEL_NUMS; 
            //HM5065MIPI_write_cmos_sensor(0x380D,( dummy_pixels&0xFF));         
            //HM5065MIPI_write_cmos_sensor(0x380C,(( dummy_pixels&0xFF00)>>8)); 
      
            dummy_lines= dummy_lines+HM5065MIPI_FULL_PERIOD_LINE_NUMS; 
            //HM5065MIPI_write_cmos_sensor(0x380F,(dummy_lines&0xFF));       
            //HM5065MIPI_write_cmos_sensor(0x380E,((dummy_lines&0xFF00)>>8));  
        } 
		HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPISetDummy function:\n ");
}    /* HM5065MIPI_set_dummy */

/*************************************************************************
* FUNCTION
*	HM5065MIPIWriteShutter
*
* DESCRIPTION
*	This function used to write the shutter.
*
* PARAMETERS
*	1. kal_uint32 : The shutter want to apply to sensor.
*
* RETURNS
*	None
*
*************************************************************************/
static void HM5065MIPIWriteShutter(kal_uint32 shutter)
{
	kal_uint32 extra_exposure_vts = 0;
	kal_uint16 temp_reg1, temp_reg2 ,temp_reg3;
	
	HM5065MIPI_write_cmos_sensor(0x132, ((shutter<<8)&0xff00) );    // shutter h 8bit
	HM5065MIPI_write_cmos_sensor(0x133, ((shutter)&0xff) );    //  shutter l 8bit

	//SENSORDB("[HM5065]exit HM5065WriteShutter function:\n ");

	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIWriteShutter function:\n ");
}    /* HM5065MIPI_write_shutter */

/*************************************************************************
* FUNCTION
*	HM5065MIPIExpWriteShutter
*
* DESCRIPTION
*	This function used to write the shutter.
*
* PARAMETERS
*	1. kal_uint32 : The shutter want to apply to sensor.
*
* RETURNS
*	None
*
*************************************************************************/

static void HM5065MIPIWriteExpShutter(kal_uint32 shutter)
{
#if 0
	shutter*=16;
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIWriteExpShutter function:\n ");
	//HM5065MIPI_write_cmos_sensor(0x3502, (shutter & 0x00FF));           //AEC[7:0]
	//HM5065MIPI_write_cmos_sensor(0x3501, ((shutter & 0x0FF00) >>8));  //AEC[15:8]
	//HM5065MIPI_write_cmos_sensor(0x3500, ((shutter & 0xFF0000) >> 16));	
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIWriteExpShutter function:\n ");
#endif
}    /* HM5065MIPI_write_shutter */

/*************************************************************************
* FUNCTION
*	HM5065MIPIExtraWriteShutter
*
* DESCRIPTION
*	This function used to write the shutter.
*
* PARAMETERS
*	1. kal_uint32 : The shutter want to apply to sensor.
*
* RETURNS
*	None
*
*************************************************************************/
static void HM5065MIPIWriteExtraShutter(kal_uint32 shutter)
{
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIWriteExtraShutter function:\n ");
	//HM5065MIPI_write_cmos_sensor(0x350D, shutter & 0xFF);          // EXVTS[b7~b0]
	//HM5065MIPI_write_cmos_sensor(0x350C, (shutter & 0xFF00) >> 8); // EXVTS[b15~b8]
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIWriteExtraShutter function:\n ");
}    /* HM5065MIPI_write_shutter */

/*************************************************************************
* FUNCTION
*	HM5065MIPIWriteSensorGain
*
* DESCRIPTION
*	This function used to write the sensor gain.
*
* PARAMETERS
*	1. kal_uint32 : The sensor gain want to apply to sensor.
*
* RETURNS
*	None
*
*************************************************************************/
static void HM5065MIPIWriteSensorGain(kal_uint32 gain)
{
	kal_uint16 temp_reg ;
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIWriteSensorGain function:\n ");
	/*if(gain > 1024)  ASSERT(0);
	temp_reg = 0;
	temp_reg=gain&0x0FF;	*/
	HM5065MIPI_write_cmos_sensor(0x136, ((gain<<8)&0xff00) );    // shutter h 8bit
	HM5065MIPI_write_cmos_sensor(0x137, ((gain)&0xff) );    //  shutter l 8bit

	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIWriteSensorGain function:\n ");
}  /* HM5065MIPI_write_sensor_gain */

/*************************************************************************
* FUNCTION
*	HM5065MIPIReadShutter
*
* DESCRIPTION
*	This function read current shutter for calculate the exposure.
*
* PARAMETERS
*	None
*
* RETURNS
*	kal_uint16 : The current shutter value.
*
*************************************************************************/
static kal_uint32 HM5065MIPIReadShutter(void)
{
	kal_uint16 temp_reg1, temp_reg2 ,temp_reg3;
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIReadShutter function:\n ");
	temp_reg1 = HM5065MIPI_read_cmos_sensor(0x17c);    // shutter h 8bit
	temp_reg2 = HM5065MIPI_read_cmos_sensor(0x17d);    //  shutter l 8bit

	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIReadShutter function:\n ");	

	return ( ((temp_reg1&0xff)<<8) | (temp_reg2&0xff) );

} /* HM5065MIPI_read_shutter */

/*************************************************************************
* FUNCTION
*	HM5065MIPIReadExtraShutter
*
* DESCRIPTION
*	This function read current shutter for calculate the exposure.
*
* PARAMETERS
*	None
*
* RETURNS
*	kal_uint16 : The current shutter value.
*
*************************************************************************/
static kal_uint32 HM5065MIPIReadExtraShutter(void)
{

} /* HM5065MIPI_read_shutter */
/*************************************************************************
* FUNCTION
*	HM5065MIPIReadSensorGain
*
* DESCRIPTION
*	This function read current sensor gain for calculate the exposure.
*
* PARAMETERS
*	None
*
* RETURNS
*	kal_uint16 : The current sensor gain value.
*
*************************************************************************/
static kal_uint32 HM5065MIPIReadSensorGain(void)
{
	kal_uint16 temp_reg1, temp_reg2 ,temp_reg3;
	//kal_uint32 sensor_gain = 0;
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIReadSensorGain function:\n ");
 
	temp_reg1 = HM5065MIPI_read_cmos_sensor(0x180);    // a gain h 8bit = 0
	temp_reg2 = HM5065MIPI_read_cmos_sensor(0x181);    //  a gain  l 8bit

	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIReadSensorGain function:\n ");
	return ( ((temp_reg1&0xff)<<8) | (temp_reg2&0xff) );
}  /* HM5065MIPIReadSensorGain */
/*************************************************************************
* FUNCTION
*	HM5065MIPI_set_AE_mode
*
* DESCRIPTION
*	This function HM5065MIPI_set_AE_mode.
*
* PARAMETERS
*	none
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static void HM5065MIPI_set_AE_mode(kal_bool AE_enable)
{
    kal_uint8 AeTemp;
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPI_set_AE_mode function:%d\n ", AE_enable);
    //
//    if(CurrentScenarioId == MSDK_SCENARIO_ID_CAMERA_PREVIEW) 
//		return;
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPI_set_AE_mode function:%d\,scenarioid=%dn ", AE_enable, CurrentScenarioId);
    if (AE_enable == KAL_TRUE)
	
		   HM5065MIPI_write_cmos_sensor(0x0142,0x00);//enable AE
		else	   
		   HM5065MIPI_write_cmos_sensor(0x0142,0x01);//disable AE
		
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_set_AE_mode function:\n ");
}

/*************************************************************************
* FUNCTION
*	HM5065MIPI_set_AWB_mode
*
* DESCRIPTION
*	This function HM5065MIPI_set_AWB_mode.
*
* PARAMETERS
*	none
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static void HM5065MIPI_set_AWB_mode(kal_bool AWB_enable)
{
    kal_uint8 AwbTemp;
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPI_set_AWB_mode function:\n ");
//	  AwbTemp = HM5065MIPI_read_cmos_sensor(0x3406);   


	//	if(AWB_enable)
		//	HM5065MIPI_write_cmos_sensor(0x01A4,0x00);//open awb
//	else
		//HM5065MIPI_write_cmos_sensor(0x01A4,0x04);//freeze
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_set_AWB_mode function:\n ");
}


/*************************************************************************
* FUNCTION
*	HM5065MIPI_night_mode
*
* DESCRIPTION
*	This function night mode of HM5065MIPI.
*
* PARAMETERS
*	none
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void HM5065MIPI_night_mode(kal_bool enable)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPI_night_mode function:\n ");
	//kal_uint16 night = HM5065MIPI_read_cmos_sensor(0x3A00); 
	spin_lock(&HM5065mipi_drv_lock);
	HM5065MIPISensor.NightMode=enable;
	spin_unlock(&HM5065mipi_drv_lock);
	#if 0
	if (enable)
	{ 

		HM5065MIPISensor.VideoMode
            /* camera night mode */
     	//HM5065MIPI_write_cmos_sensor(0x3A00,night|0x04); // 30fps-5fps
        //HM5065MIPI_write_cmos_sensor(0x3a02,0x17); 
        //HM5065MIPI_write_cmos_sensor(0x3a03,0x10);                         
        //HM5065MIPI_write_cmos_sensor(0x3a14,0x17); 
        //HM5065MIPI_write_cmos_sensor(0x3a15,0x10);
        //HM5065MIPI_write_cmos_sensor(0x3a19,0xc8);       
	}
	else
	{             /* camera normal mode */                
		//HM5065MIPI_write_cmos_sensor(0x3A00,night|0x04); //30fps-10fps               
		//HM5065MIPI_write_cmos_sensor(0x3a02,0x0b);
		//HM5065MIPI_write_cmos_sensor(0x3a03,0x88);
		//HM5065MIPI_write_cmos_sensor(0x3a14,0x0b); 
		//HM5065MIPI_write_cmos_sensor(0x3a15,0x88);								
								//HM5065MIPI_write_cmos_sensor(0x3a19,0x60);      
	} 
#endif
	//if (!HM5065MIPISensor.MODE_CAPTURE) 
	if((CurrentScenarioId == MSDK_SCENARIO_ID_CAMERA_PREVIEW) || (CurrentScenarioId == MSDK_SCENARIO_ID_VIDEO_PREVIEW) )
//	if((CurrentScenarioId == MSDK_SCENARIO_ID_CAMERA_ZSD) )

	{ 
	  if(enable)
	  { 
		//	 HM5065MIPISensor.NightMode = KAL_TRUE;
		 if(HM5065MIPISensor.VideoMode == KAL_TRUE)
		  { 	  
	
				  /* MJPEG or MPEG4 Apps */
			  printk("HM5065 night mode ture MPEG4_encode--------------\r\n");	
				  				   #if 0
				  HM5065MIPI_write_cmos_sensor(0x0040,0x01);  //  binning mode and subsampling mode for frame rate
				  HM5065MIPI_write_cmos_sensor(0x0041,0x0A);  //  04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
				  HM5065MIPI_write_cmos_sensor(0x0042,0x05);  //05	  X:800 0x500=1280,0x0320=800
				  HM5065MIPI_write_cmos_sensor(0x0043,0x00);  //00
				  HM5065MIPI_write_cmos_sensor(0x0044,0x03);  //03	  Y:600 0x03c0=960,0x0258=600
				  HM5065MIPI_write_cmos_sensor(0x0045, 0xC0);	  //
	#endif
	
				  HM5065MIPI_write_cmos_sensor(0x00E8,0x00);//Static Framerate
				  HM5065MIPI_write_cmos_sensor(0x00C8,0x00);
				  HM5065MIPI_write_cmos_sensor(0x00C9,0x0F);//15fps
				  HM5065MIPI_write_cmos_sensor(0x00CA,0x01);
				  //HM5065MIPI_write_cmos_sensor(0x0330,0x00);
		#if 0
				  //HM5065MIPI_write_cmos_sensor(0x0082,0x74);//Brightness
				  HM5065MIPI_write_cmos_sensor(0x015E,0x41);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
				  HM5065MIPI_write_cmos_sensor(0x015F,0x00);
		#else
		          HM5065MIPI_write_cmos_sensor(0x0143,0x5E);	//max int 1/15s
		          HM5065MIPI_write_cmos_sensor(0x0144,0x09);
				  #endif
		 }	  
		  else 
		 {
				printk("HM5065 night mode ture camera preview\r\n");
	#if 0
				  //HM5065MIPI_write_cmos_sensor(0x0030,0x14);	  //  Max.Derate=4
				  HM5065MIPI_write_cmos_sensor(0x0040,0x01);  //  binning mode and subsampling mode for frame rate
				  HM5065MIPI_write_cmos_sensor(0x0041,0x0A);  //  04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
				  HM5065MIPI_write_cmos_sensor(0x0042,0x05);  //05	  X:800 0x500=1280,0x0320=800
				  HM5065MIPI_write_cmos_sensor(0x0043,0x00);  //00
				  HM5065MIPI_write_cmos_sensor(0x0044,0x03);  //03	  Y:600 0x03c0=960,0x0258=600
				  HM5065MIPI_write_cmos_sensor(0x0045, 0xC0);	  //
	#endif
			  #if 0
			  HM5065MIPI_write_cmos_sensor(0x00E8,0x00);//Static Framerate
				  HM5065MIPI_write_cmos_sensor(0x00C8,0x00);
				  HM5065MIPI_write_cmos_sensor(0x00C9,0x05);//5fps
				  HM5065MIPI_write_cmos_sensor(0x00CA,0x01);
				  //HM5065MIPI_write_cmos_sensor(0x0330,0x00);
			  #else
				  HM5065MIPI_write_cmos_sensor(0x00E8,0x01);//AFR
				  HM5065MIPI_write_cmos_sensor(0x00ED,0x05);//Min=5fps
				  HM5065MIPI_write_cmos_sensor(0x00EE,0x1E);//Max=30fps
			  #endif

			  			#if 0
				  //HM5065MIPI_write_cmos_sensor(0x0082,0x64);//Brightness
				  HM5065MIPI_write_cmos_sensor(0x0143,0x64);			  
				  HM5065MIPI_write_cmos_sensor(0x02C3,0xC0);			  
				  HM5065MIPI_write_cmos_sensor(0x015E,0x41);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
				  HM5065MIPI_write_cmos_sensor(0x015F,0x00);				
				  #else
				  HM5065MIPI_write_cmos_sensor(0x0143,0x61);	//max int = 1/5s		  
				  HM5065MIPI_write_cmos_sensor(0x0144,0x0D);
				  //HM5065MIPI_write_cmos_sensor(0x02C3,0xC0);			  
				  //HM5065MIPI_write_cmos_sensor(0x0130,0x04);//brightness
				  #endif
		  } 	  
	  }
	  else
		  {
		 // HM5065MIPISensor.bNight_mode = KAL_FALSE;
		  if(HM5065MIPISensor.VideoMode == KAL_TRUE)
		  {
		 
				   /* MJPEG or MPEG4 Apps */
				  //HM5065MIPI_write_cmos_sensor(0x0030,0x14);	  //  Max.Derate=4
			  printk("HM5065 night_auto mode ture MPEG4_encode--------------\r\n"); 
				   #if 0
				  HM5065MIPI_write_cmos_sensor(0x0040,0x01);  //  binning mode and subsampling mode for frame rate
				  HM5065MIPI_write_cmos_sensor(0x0041,0x0A);  //  04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
				  HM5065MIPI_write_cmos_sensor(0x0042,0x05);  //05	  X:800 0x500=1280,0x0320=800
				  HM5065MIPI_write_cmos_sensor(0x0043,0x00);  //00
				  HM5065MIPI_write_cmos_sensor(0x0044,0x03);  //03	  Y:600 0x03c0=960,0x0258=600
				  HM5065MIPI_write_cmos_sensor(0x0045, 0xC0);	  //
	#endif
				  HM5065MIPI_write_cmos_sensor(0x00E8,0x00);//Static Framerate
				  HM5065MIPI_write_cmos_sensor(0x00C8,0x00);
				  HM5065MIPI_write_cmos_sensor(0x00C9,0x1F);//30fps
				  HM5065MIPI_write_cmos_sensor(0x00CA,0x01);
				  //HM5065MIPI_write_cmos_sensor(0x0330,0x01);
	#if 0
				  //HM5065MIPI_write_cmos_sensor(0x0082,0x64);//Brightness
				  HM5065MIPI_write_cmos_sensor(0x015E,0x40);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
				  HM5065MIPI_write_cmos_sensor(0x015F,0x00);
		#else
		          HM5065MIPI_write_cmos_sensor(0x0143,0x5C);	//max int 1/30s
		          HM5065MIPI_write_cmos_sensor(0x0144,0x09);				  
				  #endif
			  }
		  else 
			  {   
			  printk("HM5065 night_auto mode ture camera preview--------------\r\n");
		#if 0
				//HM5065MIPI_write_cmos_sensor(0x0030,0x14);  //  Max.Derate=4
				  HM5065MIPI_write_cmos_sensor(0x0040,0x01); //   binning mode and subsampling mode for frame rate
				  HM5065MIPI_write_cmos_sensor(0x0041,0x0A); //   04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
				  HM5065MIPI_write_cmos_sensor(0x0042,0x05);  //05	  X:800 0x500=1280,0x0320=800
				  HM5065MIPI_write_cmos_sensor(0x0043,0x00);  //00
				  HM5065MIPI_write_cmos_sensor(0x0044,0x03);  //03	  Y:600 0x03c0=960,0x0258=600
				  HM5065MIPI_write_cmos_sensor(0x0045, 0xC0); //
	#endif
				  HM5065MIPI_write_cmos_sensor(0x00E8,0x01);//AFR
				  HM5065MIPI_write_cmos_sensor(0x00ED,0x0a);//Min=12fps
				  HM5065MIPI_write_cmos_sensor(0x00EE,0x1E);//Max=30fps
				  //HM5065MIPI_write_cmos_sensor(0x0330,0x01);
			#if 0
				  //HM5065MIPI_write_cmos_sensor(0x0082,0x5A);//Brightness
				  HM5065MIPI_write_cmos_sensor(0x0143,0x5F);			  
				  HM5065MIPI_write_cmos_sensor(0x02C3,0xA0);			  
				  HM5065MIPI_write_cmos_sensor(0x015E,0x40);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
				  HM5065MIPI_write_cmos_sensor(0x015F,0x00);
			#else
				  HM5065MIPI_write_cmos_sensor(0x0143,0x5F);	//max int 1/10s
				  HM5065MIPI_write_cmos_sensor(0x0144,0x0D);
				  //HM5065MIPI_write_cmos_sensor(0x02C3,0xA0);			  
				  //HM5065MIPI_write_cmos_sensor(0x0130,0x00);
				  #endif
			  } 	 
		  }  
	  }
	if((CurrentScenarioId == MSDK_SCENARIO_ID_CAMERA_ZSD) )

	{ 
		  if(enable)
		  { 
					printk("HM5065 night mode ture camera ZSD preview \r\n");
					  HM5065MIPI_write_cmos_sensor(0x00E8,0x01);//AFR
					  HM5065MIPI_write_cmos_sensor(0x00ED,0x05);//Min=5fps
					  HM5065MIPI_write_cmos_sensor(0x00EE,0x0f);//Max=30fps

					  HM5065MIPI_write_cmos_sensor(0x0143,0x61);	//max int = 1/5s		  
					  HM5065MIPI_write_cmos_sensor(0x0144,0x34);
		  }
		  else
			  {
				  printk("HM5065 night_auto mode ture camera ZSD preview--------------\r\n");
					  HM5065MIPI_write_cmos_sensor(0x00E8,0x01);//AFR
					  HM5065MIPI_write_cmos_sensor(0x00ED,0x0a);//Min=12fps
					  HM5065MIPI_write_cmos_sensor(0x00EE,0x0f);//Max=30fps
					  //HM5065MIPI_write_cmos_sensor(0x0330,0x01);
					  HM5065MIPI_write_cmos_sensor(0x0143,0x5F);	//max int 1/10s
					  HM5065MIPI_write_cmos_sensor(0x0144,0x0D);
			  }  
	  }

	
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_night_mode function:\n ");
}	/* HM5065MIPI_night_mode */
/*************************************************************************
* FUNCTION
*	HM5065MIPI_GetSensorID
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
//static 
kal_uint32 HM5065MIPI_GetSensorID(kal_uint32 *sensorID)
{
    volatile signed char i;
	kal_uint32 sensor_id=0;
	kal_uint8 temp_sccb_addr = 0;
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPI_GetSensorID function:\n ");
	//HM5065MIPI_write_cmos_sensor(0x3008,0x82);// Reset sensor
	mDELAY(10);
	for(i=0;i<3;i++)
	{
		sensor_id = (HM5065MIPI_read_cmos_sensor(0x0000) << 8) | HM5065MIPI_read_cmos_sensor(0x0001);
		HM5065MIPISENSORDB("HM5065MIPI READ ID: %x",sensor_id);
		if(sensor_id != HM5065_YUV_SENSOR_ID)
		{	
			*sensorID =0xffffffff;
			return ERROR_SENSOR_CONNECT_FAIL;
		}
		else
			{
			*sensorID=HM5065_YUV_SENSOR_ID;
		        break;
			}
	}
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_GetSensorID function:\n ");
   return ERROR_NONE;    

}   
UINT32 HM5065SetTestPatternMode(kal_bool bEnable)
{
	HM5065MIPISENSORDB("[HM5065MIPI_HM5065SetTestPatternMode]test pattern bEnable:=%d\n",bEnable);
	if(bEnable)
	{
		////HM5065MIPI_write_cmos_sensor(0x503d,0x80);
		HM5065_run_test_pattern=1;
	}
	else
	{
		////HM5065MIPI_write_cmos_sensor(0x503d,0x00);
		HM5065_run_test_pattern=0;
	}
	return ERROR_NONE;
}

/*************************************************************************
* FUNCTION
*    HM5065MIPIInitialSetting
*
* DESCRIPTION
*    This function initialize the registers of CMOS sensor.
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
//static 
void HM5065MIPIInitialSetting(void)
{
	//;HM5065MIPI 1280x960,30fps
	//56Mhz, 224Mbps/Lane, 2 Lane
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIInitialSetting function:\n ");
#if 0
//Update MCU setting

		HM5065MIPI_write_cmos_sensor(0xffff,0x01);//  MCU bypass//
		HM5065MIPI_write_cmos_sensor(0x9000,0x03);//  Enable Ram and enable Write//
		
		///////////////////////////////////////////////////////////////////////////////
		//BP0 : Contrast  
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA000,0x90);//	 MOV	  DPTR,#fpInputRange(0x0C56);
		HM5065MIPI_write_cmos_sensor(0xA001,0x0C);//  
		HM5065MIPI_write_cmos_sensor(0xA002,0x56);//  
		HM5065MIPI_write_cmos_sensor(0xA003,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA004,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA005,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA006,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA007,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA008,0x12);//	 LCALL	  FPAlu_FloatToInt16(C:4285);
		HM5065MIPI_write_cmos_sensor(0xA009,0x42);//  
		HM5065MIPI_write_cmos_sensor(0xA00A,0x85);//  
		HM5065MIPI_write_cmos_sensor(0xA00B,0x90);//	 MOV	  DPTR,#0x01B7	(0x0B4D);
		HM5065MIPI_write_cmos_sensor(0xA00C,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA00D,0xB7);//  
		HM5065MIPI_write_cmos_sensor(0xA00E,0xEE);//	 MOV	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA00F,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA010,0xFC);//	 MOV	  R4,A
		HM5065MIPI_write_cmos_sensor(0xA011,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA012,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA013,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA014,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA015,0x90);//	 MOV	  DPTR,#0x0605
		HM5065MIPI_write_cmos_sensor(0xA016,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA017,0x05);//  
		HM5065MIPI_write_cmos_sensor(0xA018,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA019,0x75);//	 MOV	  B(0xF0);,#0x02
		HM5065MIPI_write_cmos_sensor(0xA01A,0xF0);//  
		HM5065MIPI_write_cmos_sensor(0xA01B,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA01C,0xA4);//	 MUL	  AB
		HM5065MIPI_write_cmos_sensor(0xA01D,0x2D);//	 ADD	  A,R5
		HM5065MIPI_write_cmos_sensor(0xA01E,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA01F,0xE5);//	 MOV	  A,B(0xF0);
		HM5065MIPI_write_cmos_sensor(0xA020,0xF0);//  
		HM5065MIPI_write_cmos_sensor(0xA021,0x3C);//	 ADDC	  A,R4
		HM5065MIPI_write_cmos_sensor(0xA022,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA023,0xAB);//	 MOV	  R3 07
		HM5065MIPI_write_cmos_sensor(0xA024,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA025,0xFA);//	 MOV	  R2,A
		HM5065MIPI_write_cmos_sensor(0xA026,0x33);//	 RLC	  A
		HM5065MIPI_write_cmos_sensor(0xA027,0x95);//	 SUBB	  A,ACC(0xE0);
		HM5065MIPI_write_cmos_sensor(0xA028,0xE0);//  
		HM5065MIPI_write_cmos_sensor(0xA029,0xF9);//	 MOV	  R1,A
		HM5065MIPI_write_cmos_sensor(0xA02A,0xF8);//	 MOV	  R0,A
		HM5065MIPI_write_cmos_sensor(0xA02B,0x90);//	 MOV	  DPTR,#0x0B4B
		HM5065MIPI_write_cmos_sensor(0xA02C,0x0B);//  
		HM5065MIPI_write_cmos_sensor(0xA02D,0x4B);//  
		HM5065MIPI_write_cmos_sensor(0xA02E,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA02F,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA030,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA031,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA032,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA033,0xEE);//	 MOV	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA034,0x33);//	 RLC	  A
		HM5065MIPI_write_cmos_sensor(0xA035,0x95);//	 SUBB	  A,ACC(0xE0);
		HM5065MIPI_write_cmos_sensor(0xA036,0xE0);//  
		HM5065MIPI_write_cmos_sensor(0xA037,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA038,0xFC);//	 MOV	  R4,A
		HM5065MIPI_write_cmos_sensor(0xA039,0x12);//	 LCALL	  C?LMUL(C:0C7B);
		HM5065MIPI_write_cmos_sensor(0xA03A,0x0C);//  
		HM5065MIPI_write_cmos_sensor(0xA03B,0x7B);//  
		HM5065MIPI_write_cmos_sensor(0xA03C,0x90);//	 MOV	  DPTR,#0x01B9(0x0B4F);
		HM5065MIPI_write_cmos_sensor(0xA03D,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA03E,0xB9);//  
		HM5065MIPI_write_cmos_sensor(0xA03F,0x12);//	 LCALL	  C?LSTXDATA(C:0E05);
		HM5065MIPI_write_cmos_sensor(0xA040,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA041,0x05);//  
		HM5065MIPI_write_cmos_sensor(0xA042,0x90);//	 MOV	  DPTR,#0x01B9(0x0B4F);
		HM5065MIPI_write_cmos_sensor(0xA043,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA044,0xB9);//  
		HM5065MIPI_write_cmos_sensor(0xA045,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA046,0xFC);//	 MOV	  R4,A
		HM5065MIPI_write_cmos_sensor(0xA047,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA048,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA049,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA04A,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA04B,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA04C,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA04D,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA04E,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA04F,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA050,0x78);//	 MOV	  R0,#g_fTimer0TimeOut(0x08);
		HM5065MIPI_write_cmos_sensor(0xA051,0x08);//  
		HM5065MIPI_write_cmos_sensor(0xA052,0x12);//	 LCALL	  C?ULSHR(C:0DBF);
		HM5065MIPI_write_cmos_sensor(0xA053,0x0D);//  
		HM5065MIPI_write_cmos_sensor(0xA054,0xBF);//  
		HM5065MIPI_write_cmos_sensor(0xA055,0xA8);//	 MOV	  R0,uwDelay1000(0x04);
		HM5065MIPI_write_cmos_sensor(0xA056,0x04);//  
		HM5065MIPI_write_cmos_sensor(0xA057,0xA9);//	 MOV	  R1 05
		HM5065MIPI_write_cmos_sensor(0xA058,0x05);//  
		HM5065MIPI_write_cmos_sensor(0xA059,0xAA);//	 MOV	  R2,uwDelay100(0x06);
		HM5065MIPI_write_cmos_sensor(0xA05A,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA05B,0xAB);//	 MOV	  R3 07
		HM5065MIPI_write_cmos_sensor(0xA05C,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA05D,0x90);//	 MOV	  DPTR,#0x0B49
		HM5065MIPI_write_cmos_sensor(0xA05E,0x0B);//  
		HM5065MIPI_write_cmos_sensor(0xA05F,0x49);//  
		HM5065MIPI_write_cmos_sensor(0xA060,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA061,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA062,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA063,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA064,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA065,0xEE);//	 MOV	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA066,0x33);//	 RLC	  A
		HM5065MIPI_write_cmos_sensor(0xA067,0x95);//	 SUBB	  A,ACC(0xE0);
		HM5065MIPI_write_cmos_sensor(0xA068,0xE0);//  
		HM5065MIPI_write_cmos_sensor(0xA069,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA06A,0xFC);//	 MOV	  R4,A
		HM5065MIPI_write_cmos_sensor(0xA06B,0xC3);//	 CLR	  C
		HM5065MIPI_write_cmos_sensor(0xA06C,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA06D,0x9B);//	 SUBB	  A,R3
		HM5065MIPI_write_cmos_sensor(0xA06E,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA06F,0xEE);//	 MOV	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA070,0x9A);//	 SUBB	  A,R2
		HM5065MIPI_write_cmos_sensor(0xA071,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA072,0xED);//	 MOV	  A,R5
		HM5065MIPI_write_cmos_sensor(0xA073,0x99);//	 SUBB	  A,R1
		HM5065MIPI_write_cmos_sensor(0xA074,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA075,0xEC);//	 MOV	  A,R4
		HM5065MIPI_write_cmos_sensor(0xA076,0x98);//	 SUBB	  A,R0
		HM5065MIPI_write_cmos_sensor(0xA077,0xFC);//	 MOV	  R4,A
		HM5065MIPI_write_cmos_sensor(0xA078,0x78);//	 MOV	  R0,#0x01
		HM5065MIPI_write_cmos_sensor(0xA079,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA07A,0x12);//	 LCALL	  C?ULSHR(C:0DBF);
		HM5065MIPI_write_cmos_sensor(0xA07B,0x0D);//  
		HM5065MIPI_write_cmos_sensor(0xA07C,0xBF);//  
		HM5065MIPI_write_cmos_sensor(0xA07D,0x90);//	 MOV	  DPTR,#m_pxwOffsetVector(0x0C4A);
		HM5065MIPI_write_cmos_sensor(0xA07E,0x0C);//  
		HM5065MIPI_write_cmos_sensor(0xA07F,0x4A);//  
		HM5065MIPI_write_cmos_sensor(0xA080,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA081,0xFC);//	 MOV	  R4,A
		HM5065MIPI_write_cmos_sensor(0xA082,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA083,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA084,0xF5);//	 MOV	  DPL(0x82);,A
		HM5065MIPI_write_cmos_sensor(0xA085,0x82);//  
		HM5065MIPI_write_cmos_sensor(0xA086,0x8C);//	 MOV	  DPH(0x83);,R4
		HM5065MIPI_write_cmos_sensor(0xA087,0x83);//  
		HM5065MIPI_write_cmos_sensor(0xA088,0xC0);//	 PUSH	  DPH(0x83);
		HM5065MIPI_write_cmos_sensor(0xA089,0x83);//  
		HM5065MIPI_write_cmos_sensor(0xA08A,0xC0);//	 PUSH	  DPL(0x82);
		HM5065MIPI_write_cmos_sensor(0xA08B,0x82);//  
		HM5065MIPI_write_cmos_sensor(0xA08C,0x90);//	 MOV	  DPTR,#0x0B48
		HM5065MIPI_write_cmos_sensor(0xA08D,0x0B);//  
		HM5065MIPI_write_cmos_sensor(0xA08E,0x48);//  
		HM5065MIPI_write_cmos_sensor(0xA08F,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA090,0xD0);//	 POP	  DPL(0x82);
		HM5065MIPI_write_cmos_sensor(0xA091,0x82);//  
		HM5065MIPI_write_cmos_sensor(0xA092,0xD0);//	 POP	  DPH(0x83);
		HM5065MIPI_write_cmos_sensor(0xA093,0x83);//  
		HM5065MIPI_write_cmos_sensor(0xA094,0x75);//	 MOV	  B(0xF0);,#0x02
		HM5065MIPI_write_cmos_sensor(0xA095,0xF0);//  
		HM5065MIPI_write_cmos_sensor(0xA096,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA097,0x12);//	 LCALL	  C?OFFXADD(C:0E45);
		HM5065MIPI_write_cmos_sensor(0xA098,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA099,0x45);//  
		HM5065MIPI_write_cmos_sensor(0xA09A,0xEE);//	 MOV	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA09B,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA09C,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA09D,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA09E,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA09F,0x02);//	 LJMP	  C:BAD8
		HM5065MIPI_write_cmos_sensor(0xA0A0,0xBA);//  
		HM5065MIPI_write_cmos_sensor(0xA0A1,0xD8);//  
		
		
		///////////////////////////////////////////////////////////////////////////////
		//BP 1: CSI reset
		///////////////////////////////////////////////////////////////////////////////
		//HM5065MIPI_write_cmos_sensor(0xA0A2,0x90);//	   MOV		DPTR,#0x0036
		//HM5065MIPI_write_cmos_sensor(0xA0A3,0x72);//	
		//HM5065MIPI_write_cmos_sensor(0xA0A4,0x00);//	
		//HM5065MIPI_write_cmos_sensor(0xA0A5,0x74);//	   MOV		A,#0x00
		//HM5065MIPI_write_cmos_sensor(0xA0A6,0x01);//	
		//HM5065MIPI_write_cmos_sensor(0xA0A7,0xF0);//		MOVX	 @DPTR,A
		//HM5065MIPI_write_cmos_sensor(0xA0A8,0x74);//	   INC		DPTR
		//HM5065MIPI_write_cmos_sensor(0xA0A9,0x00);//		MOV 	 A,#0x96
		//HM5065MIPI_write_cmos_sensor(0xA0AA,0xF0);//	
		//HM5065MIPI_write_cmos_sensor(0xA0AB,0x90);//	   MOVX 	@DPTR,A
		//HM5065MIPI_write_cmos_sensor(0xA0AC,0xC3);//		MOV 	 DPTR,#0x003D
		//HM5065MIPI_write_cmos_sensor(0xA0AD,0x88);//	
		//HM5065MIPI_write_cmos_sensor(0xA0AE,0x02);//	   
		//HM5065MIPI_write_cmos_sensor(0xA0AF,0xE8);//		LJMP	 C:9DA8
		//HM5065MIPI_write_cmos_sensor(0xA0B0,0x61);//	
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 21: Reset pipe at stop stage 
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA0A2,0x90);//		 
		HM5065MIPI_write_cmos_sensor(0xA0A3,0x30);//		MOV 	 DPTR,#0x0036
		HM5065MIPI_write_cmos_sensor(0xA0A4,0x18);//		
		HM5065MIPI_write_cmos_sensor(0xA0A5,0xe4);//		
		HM5065MIPI_write_cmos_sensor(0xA0A6,0xf0);//		MOV 	 A,#0x00
		HM5065MIPI_write_cmos_sensor(0xA0A7,0x74);//		
		HM5065MIPI_write_cmos_sensor(0xA0A8,0x3f);//		MOVX	 @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA0A9,0xf0);//		INC 	 DPTR
		HM5065MIPI_write_cmos_sensor(0xA0AA,0x22);//		INC 	 DPTR
		
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 2: CSI ULP enter 1
		///////////////////////////////////////////////////////////////////////////////
		
		//HM5065MIPI_write_cmos_sensor(0xA0B1,0x02);//	   
		//HM5065MIPI_write_cmos_sensor(0xA0B2,0xE6);//		LJMP	 C:9DA8
		//HM5065MIPI_write_cmos_sensor(0xA0B3,0x7C);//	
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 3: CSI ULP enter 2
		///////////////////////////////////////////////////////////////////////////////
		
		//HM5065MIPI_write_cmos_sensor(0xA0B4,0xF0);//	   
		//HM5065MIPI_write_cmos_sensor(0xA0B5,0x90);//	   
		//HM5065MIPI_write_cmos_sensor(0xA0B6,0x72);//		LJMP	 C:9DA8
		//HM5065MIPI_write_cmos_sensor(0xA0B7,0x08);//	
		//HM5065MIPI_write_cmos_sensor(0xA0B8,0xE0);//	   
		//HM5065MIPI_write_cmos_sensor(0xA0B9,0x44);//		LJMP	 C:9DA8
		//HM5065MIPI_write_cmos_sensor(0xA0BA,0x10);//	
		//HM5065MIPI_write_cmos_sensor(0xA0BB,0xF0);//	   
		//HM5065MIPI_write_cmos_sensor(0xA0BC,0x02);//		LJMP	 C:9DA8
		//HM5065MIPI_write_cmos_sensor(0xA0BD,0xE6);//	
		//HM5065MIPI_write_cmos_sensor(0xA0BE,0x9A);//	  
		
		///////////////////////////////////////////////////////////////////////////////
		//BP 4: AV
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA0BF,0x90);//	 MOV	  DPTR,#0x005E
		HM5065MIPI_write_cmos_sensor(0xA0C0,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA0C1,0x5E);//  
		HM5065MIPI_write_cmos_sensor(0xA0C2,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA0C3,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA0C4,0x70);//	 JNZ	  B00:A9AF
		HM5065MIPI_write_cmos_sensor(0xA0C5,0x20);//  
		HM5065MIPI_write_cmos_sensor(0xA0C6,0x90);//	 MOV	  DPTR,#Av2x2_H_Size(0x4704);
		HM5065MIPI_write_cmos_sensor(0xA0C7,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA0C8,0x04);//  
		HM5065MIPI_write_cmos_sensor(0xA0C9,0x74);//	 MOV	  A,#bInt_Event_Status(0x0A);
		HM5065MIPI_write_cmos_sensor(0xA0CA,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA0CB,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA0CC,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA0CD,0x74);//	 MOV	  A,#0x30
		HM5065MIPI_write_cmos_sensor(0xA0CE,0x30);//  
		HM5065MIPI_write_cmos_sensor(0xA0CF,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA0D0,0x90);//	 MOV	  DPTR,#Av2x2_V_Size(0x470C);
		HM5065MIPI_write_cmos_sensor(0xA0D1,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA0D2,0x0C);//  
		HM5065MIPI_write_cmos_sensor(0xA0D3,0x74);//	 MOV	  A,#0x07
		HM5065MIPI_write_cmos_sensor(0xA0D4,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA0D5,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA0D6,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA0D7,0x74);//	 MOV	  A,#IE(0xA8);
		HM5065MIPI_write_cmos_sensor(0xA0D8,0xA8);//  
		HM5065MIPI_write_cmos_sensor(0xA0D9,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA0DA,0x90);//	 MOV	  DPTR,#Av2x2_Xscale(0x47A4);
		HM5065MIPI_write_cmos_sensor(0xA0DB,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA0DC,0xA4);//  
		HM5065MIPI_write_cmos_sensor(0xA0DD,0x74);//	 MOV	  A,#0x01
		HM5065MIPI_write_cmos_sensor(0xA0DE,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA0DF,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA0E0,0x90);//	 MOV	  DPTR,#Av2x2_Yscale(0x47A8);
		HM5065MIPI_write_cmos_sensor(0xA0E1,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA0E2,0xA8);//  
		HM5065MIPI_write_cmos_sensor(0xA0E3,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA0E4,0x80);//	 SJMP	  B00:A9FF
		HM5065MIPI_write_cmos_sensor(0xA0E5,0x50);//  
		HM5065MIPI_write_cmos_sensor(0xA0E6,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA0E7,0x64);//	 XRL	  A,#0x01
		HM5065MIPI_write_cmos_sensor(0xA0E8,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA0E9,0x60);//	 JZ 	  B00:A9B8
		HM5065MIPI_write_cmos_sensor(0xA0EA,0x04);//  
		HM5065MIPI_write_cmos_sensor(0xA0EB,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA0EC,0xB4);//	 CJNE	  A,#0x03,B00:A9D8
		HM5065MIPI_write_cmos_sensor(0xA0ED,0x03);//  
		HM5065MIPI_write_cmos_sensor(0xA0EE,0x20);//  
		HM5065MIPI_write_cmos_sensor(0xA0EF,0x90);//	 MOV	  DPTR,#Av2x2_H_Size(0x4704);
		HM5065MIPI_write_cmos_sensor(0xA0F0,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA0F1,0x04);//  
		HM5065MIPI_write_cmos_sensor(0xA0F2,0x74);//	 MOV	  A,#0x05
		HM5065MIPI_write_cmos_sensor(0xA0F3,0x05);//  
		HM5065MIPI_write_cmos_sensor(0xA0F4,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA0F5,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA0F6,0x74);//	 MOV	  A,#0x18
		HM5065MIPI_write_cmos_sensor(0xA0F7,0x18);//  
		HM5065MIPI_write_cmos_sensor(0xA0F8,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA0F9,0x90);//	 MOV	  DPTR,#Av2x2_V_Size(0x470C);
		HM5065MIPI_write_cmos_sensor(0xA0FA,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA0FB,0x0C);//  
		HM5065MIPI_write_cmos_sensor(0xA0FC,0x74);//	 MOV	  A,#0x03
		HM5065MIPI_write_cmos_sensor(0xA0FD,0x03);//  
		HM5065MIPI_write_cmos_sensor(0xA0FE,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA0FF,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA100,0x74);//	 MOV	  A,#m_fDitherBitFormat(0xD4);
		HM5065MIPI_write_cmos_sensor(0xA101,0xD4);//  
		HM5065MIPI_write_cmos_sensor(0xA102,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA103,0x90);//	 MOV	  DPTR,#Av2x2_Xscale(0x47A4);
		HM5065MIPI_write_cmos_sensor(0xA104,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA105,0xA4);//  
		HM5065MIPI_write_cmos_sensor(0xA106,0x74);//	 MOV	  A,#0x02
		HM5065MIPI_write_cmos_sensor(0xA107,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA108,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA109,0x90);//	 MOV	  DPTR,#Av2x2_Yscale(0x47A8);
		HM5065MIPI_write_cmos_sensor(0xA10A,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA10B,0xA8);//  
		HM5065MIPI_write_cmos_sensor(0xA10C,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA10D,0x80);//	 SJMP	  B00:A9FF
		HM5065MIPI_write_cmos_sensor(0xA10E,0x27);//  
		HM5065MIPI_write_cmos_sensor(0xA10F,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA110,0x64);//	 XRL	  A,#0x02
		HM5065MIPI_write_cmos_sensor(0xA111,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA112,0x60);//	 JZ 	  B00:A9E1
		HM5065MIPI_write_cmos_sensor(0xA113,0x04);//  
		HM5065MIPI_write_cmos_sensor(0xA114,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA115,0xB4);//	 CJNE	  A,#uwDelay1000(0x04);,B00:A9FF
		HM5065MIPI_write_cmos_sensor(0xA116,0x04);//  
		HM5065MIPI_write_cmos_sensor(0xA117,0x1E);//  
		HM5065MIPI_write_cmos_sensor(0xA118,0x90);//	 MOV	  DPTR,#Av2x2_H_Size(0x4704);
		HM5065MIPI_write_cmos_sensor(0xA119,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA11A,0x04);//  
		HM5065MIPI_write_cmos_sensor(0xA11B,0x74);//	 MOV	  A,#0x02
		HM5065MIPI_write_cmos_sensor(0xA11C,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA11D,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA11E,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA11F,0x74);//	 MOV	  A,#TH0(0x8C);
		HM5065MIPI_write_cmos_sensor(0xA120,0x8C);//  
		HM5065MIPI_write_cmos_sensor(0xA121,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA122,0x90);//	 MOV	  DPTR,#Av2x2_V_Size(0x470C);
		HM5065MIPI_write_cmos_sensor(0xA123,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA124,0x0C);//  
		HM5065MIPI_write_cmos_sensor(0xA125,0x74);//	 MOV	  A,#0x01
		HM5065MIPI_write_cmos_sensor(0xA126,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA127,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA128,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA129,0x74);//	 MOV	  A,#0xEA
		HM5065MIPI_write_cmos_sensor(0xA12A,0xEA);//  
		HM5065MIPI_write_cmos_sensor(0xA12B,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA12C,0x90);//	 MOV	  DPTR,#Av2x2_Xscale(0x47A4);
		HM5065MIPI_write_cmos_sensor(0xA12D,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA12E,0xA4);//  
		HM5065MIPI_write_cmos_sensor(0xA12F,0x74);//	 MOV	  A,#uwDelay1000(0x04);
		HM5065MIPI_write_cmos_sensor(0xA130,0x04);//  
		HM5065MIPI_write_cmos_sensor(0xA131,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA132,0x90);//	 MOV	  DPTR,#Av2x2_Yscale(0x47A8);
		HM5065MIPI_write_cmos_sensor(0xA133,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA134,0xA8);//  
		HM5065MIPI_write_cmos_sensor(0xA135,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA136,0x22);//	 RTN
		
		///////////////////////////////////////////////////////////////////////////////
		//BP 5: AV OTP
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA137,0x74);//	 MOV	  A,#uwDelay1000(0x04);
		HM5065MIPI_write_cmos_sensor(0xA138,0x04);//  
		HM5065MIPI_write_cmos_sensor(0xA139,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA13A,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA13B,0x74);//	 MOV	  A,#ZoomPanControl(0x20);
		HM5065MIPI_write_cmos_sensor(0xA13C,0x20);//  
		HM5065MIPI_write_cmos_sensor(0xA13D,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA13E,0xE4);//	 CLR	  A
		HM5065MIPI_write_cmos_sensor(0xA13F,0xF5);//	 MOV	  0x22,A
		HM5065MIPI_write_cmos_sensor(0xA140,0x22);//  
		HM5065MIPI_write_cmos_sensor(0xA141,0xE5);//	 MOV	  A 22
		HM5065MIPI_write_cmos_sensor(0xA142,0x22);//  
		HM5065MIPI_write_cmos_sensor(0xA143,0xC3);//	 CLR	  C
		HM5065MIPI_write_cmos_sensor(0xA144,0x94);//	 SUBB	  A,#PipeSetupBank0(0x40);
		HM5065MIPI_write_cmos_sensor(0xA145,0x40);//  
		HM5065MIPI_write_cmos_sensor(0xA146,0x40);//	 JC 	  B00:AB81
		HM5065MIPI_write_cmos_sensor(0xA147,0x03);//  
		HM5065MIPI_write_cmos_sensor(0xA148,0x02);//	 LJMP	  B00:AC33
		HM5065MIPI_write_cmos_sensor(0xA149,0xF1);//  
		HM5065MIPI_write_cmos_sensor(0xA14A,0xFD);//  
		HM5065MIPI_write_cmos_sensor(0xA14B,0x90);//	 MOV	  DPTR,#0x0ABA
		HM5065MIPI_write_cmos_sensor(0xA14C,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA14D,0xBA);//  
		HM5065MIPI_write_cmos_sensor(0xA14E,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA14F,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA150,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA151,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA152,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA153,0xF5);//	 MOV	  DPL(0x82);,A
		HM5065MIPI_write_cmos_sensor(0xA154,0x82);//  
		HM5065MIPI_write_cmos_sensor(0xA155,0x8E);//	 MOV	  DPH(0x83);,R6
		HM5065MIPI_write_cmos_sensor(0xA156,0x83);//  
		HM5065MIPI_write_cmos_sensor(0xA157,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA158,0x54);//	 ANL	  A,#0x70
		HM5065MIPI_write_cmos_sensor(0xA159,0x70);//  
		HM5065MIPI_write_cmos_sensor(0xA15A,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA15B,0xC4);//	 SWAP	  A
		HM5065MIPI_write_cmos_sensor(0xA15C,0x54);//	 ANL	  A,#0x0F
		HM5065MIPI_write_cmos_sensor(0xA15D,0x0F);//  
		HM5065MIPI_write_cmos_sensor(0xA15E,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA15F,0x90);//	 MOV	  DPTR,#0x0ABC
		HM5065MIPI_write_cmos_sensor(0xA160,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA161,0xBC);//  
		HM5065MIPI_write_cmos_sensor(0xA162,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA163,0xFA);//	 MOV	  R2,A
		HM5065MIPI_write_cmos_sensor(0xA164,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA165,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA166,0xF5);//	 MOV	  DPL(0x82);,A
		HM5065MIPI_write_cmos_sensor(0xA167,0x82);//  
		HM5065MIPI_write_cmos_sensor(0xA168,0x8A);//	 MOV	  DPH(0x83);,R2
		HM5065MIPI_write_cmos_sensor(0xA169,0x83);//  
		HM5065MIPI_write_cmos_sensor(0xA16A,0xED);//	 MOV	  A,R5
		HM5065MIPI_write_cmos_sensor(0xA16B,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA16C,0x90);//	 MOV	  DPTR,#0x0ABD
		HM5065MIPI_write_cmos_sensor(0xA16D,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA16E,0xBD);//  
		HM5065MIPI_write_cmos_sensor(0xA16F,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA170,0x04);//	 INC	  A
		HM5065MIPI_write_cmos_sensor(0xA171,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA172,0x70);//	 JNZ	  B00:ABB0
		HM5065MIPI_write_cmos_sensor(0xA173,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA174,0x90);//	 MOV	  DPTR,#0x0ABC
		HM5065MIPI_write_cmos_sensor(0xA175,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA176,0xBC);//  
		HM5065MIPI_write_cmos_sensor(0xA177,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA178,0x04);//	 INC	  A
		HM5065MIPI_write_cmos_sensor(0xA179,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA17A,0x8F);//	 MOV	  DPL(0x82);,R7
		HM5065MIPI_write_cmos_sensor(0xA17B,0x82);//  
		HM5065MIPI_write_cmos_sensor(0xA17C,0x8E);//	 MOV	  DPH(0x83);,R6
		HM5065MIPI_write_cmos_sensor(0xA17D,0x83);//  
		HM5065MIPI_write_cmos_sensor(0xA17E,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA17F,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA180,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA181,0x90);//	 MOV	  DPTR,#0x0ABC
		HM5065MIPI_write_cmos_sensor(0xA182,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA183,0xBC);//  
		HM5065MIPI_write_cmos_sensor(0xA184,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA185,0xFC);//	 MOV	  R4,A
		HM5065MIPI_write_cmos_sensor(0xA186,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA187,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA188,0xF5);//	 MOV	  DPL(0x82);,A
		HM5065MIPI_write_cmos_sensor(0xA189,0x82);//  
		HM5065MIPI_write_cmos_sensor(0xA18A,0x8C);//	 MOV	  DPH(0x83);,R4
		HM5065MIPI_write_cmos_sensor(0xA18B,0x83);//  
		HM5065MIPI_write_cmos_sensor(0xA18C,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA18D,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA18E,0x90);//	 MOV	  DPTR,#0x0ABD
		HM5065MIPI_write_cmos_sensor(0xA18F,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA190,0xBD);//  
		HM5065MIPI_write_cmos_sensor(0xA191,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA192,0x04);//	 INC	  A
		HM5065MIPI_write_cmos_sensor(0xA193,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA194,0x70);//	 JNZ	  B00:ABD2
		HM5065MIPI_write_cmos_sensor(0xA195,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA196,0x90);//	 MOV	  DPTR,#0x0ABC
		HM5065MIPI_write_cmos_sensor(0xA197,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA198,0xBC);//  
		HM5065MIPI_write_cmos_sensor(0xA199,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA19A,0x04);//	 INC	  A
		HM5065MIPI_write_cmos_sensor(0xA19B,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA19C,0x90);//	 MOV	  DPTR,#0x0ABA
		HM5065MIPI_write_cmos_sensor(0xA19D,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA19E,0xBA);//  
		HM5065MIPI_write_cmos_sensor(0xA19F,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1A0,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA1A1,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA1A2,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1A3,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA1A4,0xF5);//	 MOV	  DPL(0x82);,A
		HM5065MIPI_write_cmos_sensor(0xA1A5,0x82);//  
		HM5065MIPI_write_cmos_sensor(0xA1A6,0x8E);//	 MOV	  DPH(0x83);,R6
		HM5065MIPI_write_cmos_sensor(0xA1A7,0x83);//  
		HM5065MIPI_write_cmos_sensor(0xA1A8,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1A9,0x54);//	 ANL	  A,#0x07
		HM5065MIPI_write_cmos_sensor(0xA1AA,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA1AB,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA1AC,0x90);//	 MOV	  DPTR,#0x0ABC
		HM5065MIPI_write_cmos_sensor(0xA1AD,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA1AE,0xBC);//  
		HM5065MIPI_write_cmos_sensor(0xA1AF,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1B0,0xFA);//	 MOV	  R2,A
		HM5065MIPI_write_cmos_sensor(0xA1B1,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA1B2,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1B3,0xF5);//	 MOV	  DPL(0x82);,A
		HM5065MIPI_write_cmos_sensor(0xA1B4,0x82);//  
		HM5065MIPI_write_cmos_sensor(0xA1B5,0x8A);//	 MOV	  DPH(0x83);,R2
		HM5065MIPI_write_cmos_sensor(0xA1B6,0x83);//  
		HM5065MIPI_write_cmos_sensor(0xA1B7,0xED);//	 MOV	  A,R5
		HM5065MIPI_write_cmos_sensor(0xA1B8,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA1B9,0x90);//	 MOV	  DPTR,#0x0ABD
		HM5065MIPI_write_cmos_sensor(0xA1BA,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA1BB,0xBD);//  
		HM5065MIPI_write_cmos_sensor(0xA1BC,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1BD,0x04);//	 INC	  A
		HM5065MIPI_write_cmos_sensor(0xA1BE,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA1BF,0x70);//	 JNZ	  B00:ABFD
		HM5065MIPI_write_cmos_sensor(0xA1C0,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA1C1,0x90);//	 MOV	  DPTR,#0x0ABC
		HM5065MIPI_write_cmos_sensor(0xA1C2,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA1C3,0xBC);//  
		HM5065MIPI_write_cmos_sensor(0xA1C4,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1C5,0x04);//	 INC	  A
		HM5065MIPI_write_cmos_sensor(0xA1C6,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA1C7,0x8F);//	 MOV	  DPL(0x82);,R7
		HM5065MIPI_write_cmos_sensor(0xA1C8,0x82);//  
		HM5065MIPI_write_cmos_sensor(0xA1C9,0x8E);//	 MOV	  DPH(0x83);,R6
		HM5065MIPI_write_cmos_sensor(0xA1CA,0x83);//  
		HM5065MIPI_write_cmos_sensor(0xA1CB,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA1CC,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA1CD,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1CE,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA1CF,0x90);//	 MOV	  DPTR,#0x0ABC
		HM5065MIPI_write_cmos_sensor(0xA1D0,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA1D1,0xBC);//  
		HM5065MIPI_write_cmos_sensor(0xA1D2,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1D3,0xFC);//	 MOV	  R4,A
		HM5065MIPI_write_cmos_sensor(0xA1D4,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA1D5,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1D6,0xF5);//	 MOV	  DPL(0x82);,A
		HM5065MIPI_write_cmos_sensor(0xA1D7,0x82);//  
		HM5065MIPI_write_cmos_sensor(0xA1D8,0x8C);//	 MOV	  DPH(0x83);,R4
		HM5065MIPI_write_cmos_sensor(0xA1D9,0x83);//  
		HM5065MIPI_write_cmos_sensor(0xA1DA,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA1DB,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA1DC,0x90);//	 MOV	  DPTR,#0x0ABD
		HM5065MIPI_write_cmos_sensor(0xA1DD,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA1DE,0xBD);//  
		HM5065MIPI_write_cmos_sensor(0xA1DF,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1E0,0x04);//	 INC	  A
		HM5065MIPI_write_cmos_sensor(0xA1E1,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA1E2,0x70);//	 JNZ	  B00:AC20
		HM5065MIPI_write_cmos_sensor(0xA1E3,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA1E4,0x90);//	 MOV	  DPTR,#0x0ABC
		HM5065MIPI_write_cmos_sensor(0xA1E5,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA1E6,0xBC);//  
		HM5065MIPI_write_cmos_sensor(0xA1E7,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1E8,0x04);//	 INC	  A
		HM5065MIPI_write_cmos_sensor(0xA1E9,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA1EA,0x90);//	 MOV	  DPTR,#0x0ABB
		HM5065MIPI_write_cmos_sensor(0xA1EB,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA1EC,0xBB);//  
		HM5065MIPI_write_cmos_sensor(0xA1ED,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1EE,0x24);//	 ADD	  A,#0x03
		HM5065MIPI_write_cmos_sensor(0xA1EF,0x03);//  
		HM5065MIPI_write_cmos_sensor(0xA1F0,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA1F1,0x90);//	 MOV	  DPTR,#0x0ABA
		HM5065MIPI_write_cmos_sensor(0xA1F2,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA1F3,0xBA);//  
		HM5065MIPI_write_cmos_sensor(0xA1F4,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA1F5,0x34);//	 ADDC	  A,#DeviceParameters(0x00);
		HM5065MIPI_write_cmos_sensor(0xA1F6,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA1F7,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA1F8,0x05);//	 INC	  0x22
		HM5065MIPI_write_cmos_sensor(0xA1F9,0x22);//  
		HM5065MIPI_write_cmos_sensor(0xA1FA,0x02);//	 LJMP	  B00:A409
		HM5065MIPI_write_cmos_sensor(0xA1FB,0xF1);//  
		HM5065MIPI_write_cmos_sensor(0xA1FC,0x41);//  
		HM5065MIPI_write_cmos_sensor(0xA1FD,0x90);//	 MOV	  DPTR,#0x0ABA
		HM5065MIPI_write_cmos_sensor(0xA1FE,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA1FF,0xBA);//  
		HM5065MIPI_write_cmos_sensor(0xA200,0x74);//	 MOV	  A,#g_fpPixelCount(0x0E);
		HM5065MIPI_write_cmos_sensor(0xA201,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA202,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA203,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA204,0x74);//	 MOV	  A,#0xDC
		HM5065MIPI_write_cmos_sensor(0xA205,0xDC);//  
		HM5065MIPI_write_cmos_sensor(0xA206,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA207,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA208,0x74);//	 MOV	  A,#0x05
		HM5065MIPI_write_cmos_sensor(0xA209,0x05);//  
		HM5065MIPI_write_cmos_sensor(0xA20A,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA20B,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA20C,0x74);//	 MOV	  A,#0x61
		HM5065MIPI_write_cmos_sensor(0xA20D,0x61);//  
		HM5065MIPI_write_cmos_sensor(0xA20E,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA20F,0x90);//	 MOV	  DPTR,#0x0ABA
		HM5065MIPI_write_cmos_sensor(0xA210,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA211,0xBA);//  
		HM5065MIPI_write_cmos_sensor(0xA212,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA213,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA214,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA215,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA216,0xAA);//	 MOV	  R2,uwDelay100(0x06);
		HM5065MIPI_write_cmos_sensor(0xA217,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA218,0xF9);//	 MOV	  R1,A
		HM5065MIPI_write_cmos_sensor(0xA219,0x7B);//	 MOV	  R3,#0x01
		HM5065MIPI_write_cmos_sensor(0xA21A,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA21B,0xC0);//	 PUSH	  0x02
		HM5065MIPI_write_cmos_sensor(0xA21C,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA21D,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA21E,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA21F,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA220,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA221,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA222,0xAA);//	 MOV	  R2,uwDelay100(0x06);
		HM5065MIPI_write_cmos_sensor(0xA223,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA224,0xF8);//	 MOV	  R0,A
		HM5065MIPI_write_cmos_sensor(0xA225,0xAC);//	 MOV	  R4 02
		HM5065MIPI_write_cmos_sensor(0xA226,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA227,0x7D);//	 MOV	  R5,#0x01
		HM5065MIPI_write_cmos_sensor(0xA228,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA229,0xD0);//	 POP	  0x02
		HM5065MIPI_write_cmos_sensor(0xA22A,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA22B,0x7E);//	 MOV	  R6,#DeviceParameters(0x00);
		HM5065MIPI_write_cmos_sensor(0xA22C,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA22D,0x7F);//	 MOV	  R7,#uwDelay1000(0x04);
		HM5065MIPI_write_cmos_sensor(0xA22E,0x04);//  
		HM5065MIPI_write_cmos_sensor(0xA22F,0x12);//	 LCALL	  C?COPY517(C:0F6F);
		HM5065MIPI_write_cmos_sensor(0xA230,0x0F);//  
		HM5065MIPI_write_cmos_sensor(0xA231,0x6F);//  
		HM5065MIPI_write_cmos_sensor(0xA232,0x02);//	 JUMP	 
		HM5065MIPI_write_cmos_sensor(0xA233,0x66);//  
		HM5065MIPI_write_cmos_sensor(0xA234,0xD9);//  
		
		///////////////////////////////////////////////////////////////////////////////
		//BP 12: AF p74 -04 part2
		///////////////////////////////////////////////////////////////////////////////// 
		HM5065MIPI_write_cmos_sensor(0xA235,0x90);//	 MOV	  R4 02
		HM5065MIPI_write_cmos_sensor(0xA236,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA237,0xD0);//	 MOV	  R5,#0x01
		HM5065MIPI_write_cmos_sensor(0xA238,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA239,0xA2);//	 POP	  0x02
		HM5065MIPI_write_cmos_sensor(0xA23A,0x69);//  
		
		///////////////////////////////////////////////////////////////////////////////
		//BP 7: skip AF init part1
		///////////////////////////////////////////////////////////////////////////////// 
		HM5065MIPI_write_cmos_sensor(0xA240,0x02);//	 ANL	  A,#0xFD
		HM5065MIPI_write_cmos_sensor(0xA241,0x21);//  
		HM5065MIPI_write_cmos_sensor(0xA242,0x7F);//	 MOVX	  @DPTR,A
		
		///////////////////////////////////////////////////////////////////////////////
		//BP 8: skip AF init part 2
		///////////////////////////////////////////////////////////////////////////////// 
		HM5065MIPI_write_cmos_sensor(0xA243,0x02);//	 ANL	  A,#0xFD
		HM5065MIPI_write_cmos_sensor(0xA244,0x21);//  
		HM5065MIPI_write_cmos_sensor(0xA245,0xF4);//	 MOVX	  @DPTR,A
		
		///////////////////////////////////////////////////////////////////////////////
		//BP 9:  Stop/Start part1  skip while loop in scaler update
		///////////////////////////////////////////////////////////////////////////////// 
		HM5065MIPI_write_cmos_sensor(0xA246,0x02);//	 ANL	  A,#0xFD
		HM5065MIPI_write_cmos_sensor(0xA247,0xA6);//  
		HM5065MIPI_write_cmos_sensor(0xA248,0x15);//	 MOVX	  @DPTR,A
		
		///////////////////////////////////////////////////////////////////////////////
		//BP 10:  Stop/Start part2	mode maganer
		///////////////////////////////////////////////////////////////////////////////// 
		HM5065MIPI_write_cmos_sensor(0xA249,0x60);//	 JZ 	  C:29EF
		HM5065MIPI_write_cmos_sensor(0xA24A,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA24B,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA24C,0xB4);//	 CJNE	  A,#0x01,C:29FF
		HM5065MIPI_write_cmos_sensor(0xA24D,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA24E,0x16);//  
		HM5065MIPI_write_cmos_sensor(0xA24F,0x90);//	 MOV	  DPTR,#ModeSetup(0x005D);
		HM5065MIPI_write_cmos_sensor(0xA250,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA251,0x5D);//  
		HM5065MIPI_write_cmos_sensor(0xA252,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA253,0x70);//	 JNZ	  C:29FF
		HM5065MIPI_write_cmos_sensor(0xA254,0x10);//  
		HM5065MIPI_write_cmos_sensor(0xA255,0x12);//	 LCALL	  StreamManager_ResumeStreaming(C:26C8);
		HM5065MIPI_write_cmos_sensor(0xA256,0x26);//  
		HM5065MIPI_write_cmos_sensor(0xA257,0xC8);//  
		HM5065MIPI_write_cmos_sensor(0xA258,0x90);//	 MOV	  DPTR,#0x0011
		HM5065MIPI_write_cmos_sensor(0xA259,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA25A,0x11);//  
		HM5065MIPI_write_cmos_sensor(0xA25B,0x74);//	 MOV	  A,#0x30
		HM5065MIPI_write_cmos_sensor(0xA25C,0x30);//  
		HM5065MIPI_write_cmos_sensor(0xA25D,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA25E,0x90);//	 MOV	  DPTR,#fpHighClipForDesiredExposure(0x0010);
		HM5065MIPI_write_cmos_sensor(0xA25F,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA260,0x10);//  
		HM5065MIPI_write_cmos_sensor(0xA261,0x74);//	 MOV	  A,#0x01
		HM5065MIPI_write_cmos_sensor(0xA262,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA263,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA264,0x22);//	 RET	  
		HM5065MIPI_write_cmos_sensor(0xA265,0x12);//	 LCALL	  C:25A8
		HM5065MIPI_write_cmos_sensor(0xA266,0x25);//  
		HM5065MIPI_write_cmos_sensor(0xA267,0xA8);//  
		HM5065MIPI_write_cmos_sensor(0xA268,0x02);//	   RET	   
		HM5065MIPI_write_cmos_sensor(0xA269,0x29);//  
		HM5065MIPI_write_cmos_sensor(0xA26A,0xFC);//  
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 2 	CSI ULP enter 
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA26B,0x44);//	 ORL	  A,#fpHighClipForDesiredExposure(0x10);
		HM5065MIPI_write_cmos_sensor(0xA26C,0x18);//  
		HM5065MIPI_write_cmos_sensor(0xA26D,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA26E,0x90);//	 MOV	  DPTR,#Tx_Csi2_Dphy_Datalane2_Pwr_Ctrl(0x7218);
		HM5065MIPI_write_cmos_sensor(0xA26F,0x72);//  
		HM5065MIPI_write_cmos_sensor(0xA270,0x18);//  
		HM5065MIPI_write_cmos_sensor(0xA271,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA272,0x44);//	 ORL	  A,#fpHighClipForDesiredExposure(0x10);
		HM5065MIPI_write_cmos_sensor(0xA273,0x18);//  
		HM5065MIPI_write_cmos_sensor(0xA274,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA275,0x00);//	 MOV	  DPTR,#Tx_Csi2_Dphy_Clklane_Pwr_Ctrl(0x7208);
		HM5065MIPI_write_cmos_sensor(0xA276,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA277,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA278,0x00);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA279,0x00);//	 ORL	  A,#fpHighClipForDesiredExposure(0x10);
		HM5065MIPI_write_cmos_sensor(0xA27A,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA27B,0x90);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA27C,0x72);//	 MOV	  DPTR,#Tx_Csi2_Pwr_Ctrl(0x7214);
		HM5065MIPI_write_cmos_sensor(0xA27D,0x08);//  
		HM5065MIPI_write_cmos_sensor(0xA27E,0xE0);//  
		HM5065MIPI_write_cmos_sensor(0xA27F,0x44);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA280,0x10);//	 ANL	  A,#0xFD
		HM5065MIPI_write_cmos_sensor(0xA281,0xF0);//  
		HM5065MIPI_write_cmos_sensor(0xA282,0x90);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA283,0x72);//	 MOV	  DPTR,#Tx_Csi2_Dphy_Pwr_Ctrl(0x7204);
		HM5065MIPI_write_cmos_sensor(0xA284,0x14);//  
		HM5065MIPI_write_cmos_sensor(0xA285,0xE0);//  
		HM5065MIPI_write_cmos_sensor(0xA286,0x54);//	 MOV	  A,#0x1F
		HM5065MIPI_write_cmos_sensor(0xA287,0xFD);//  
		HM5065MIPI_write_cmos_sensor(0xA288,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA289,0x22);//	 RET	 
		
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 13	 AF P74 05	 
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA29B,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA29C,0xD3);//	 SETB	  C
		HM5065MIPI_write_cmos_sensor(0xA29D,0x90);//	 MOV	  DPTR,#0x0791
		HM5065MIPI_write_cmos_sensor(0xA29E,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA29F,0x91);//  
		HM5065MIPI_write_cmos_sensor(0xA2A0,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2A1,0x94);//	 SUBB	  A,#0x21
		HM5065MIPI_write_cmos_sensor(0xA2A2,0x21);//  
		HM5065MIPI_write_cmos_sensor(0xA2A3,0x90);//	 MOV	  DPTR,#AutoFocusInput(0x0790);
		HM5065MIPI_write_cmos_sensor(0xA2A4,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA2A5,0x90);//  
		HM5065MIPI_write_cmos_sensor(0xA2A6,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2A7,0x64);//	 XRL	  A,#PipeSetupCommon(0x80);
		HM5065MIPI_write_cmos_sensor(0xA2A8,0x80);//  
		HM5065MIPI_write_cmos_sensor(0xA2A9,0x94);//	 SUBB	  A,#SP(0x81);
		HM5065MIPI_write_cmos_sensor(0xA2AA,0x81);//  
		HM5065MIPI_write_cmos_sensor(0xA2AB,0x40);//	 JC 	  B01:B152
		HM5065MIPI_write_cmos_sensor(0xA2AC,0x08);//  
		HM5065MIPI_write_cmos_sensor(0xA2AD,0x90);//	 MOV	  DPTR,#0x07CB
		HM5065MIPI_write_cmos_sensor(0xA2AE,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA2AF,0xCB);//  
		HM5065MIPI_write_cmos_sensor(0xA2B0,0x74);//	 MOV	  A,#0xFF
		HM5065MIPI_write_cmos_sensor(0xA2B1,0xFF);//  
		HM5065MIPI_write_cmos_sensor(0xA2B2,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA2B3,0x80);//	 SJMP	  B01:B158
		HM5065MIPI_write_cmos_sensor(0xA2B4,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA2B5,0x90);//	 MOV	  DPTR,#0x07CB
		HM5065MIPI_write_cmos_sensor(0xA2B6,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA2B7,0xCB);//  
		HM5065MIPI_write_cmos_sensor(0xA2B8,0x74);//	 MOV	  A,#0x01
		HM5065MIPI_write_cmos_sensor(0xA2B9,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA2BA,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA2BB,0x02);//	 JUMP
		HM5065MIPI_write_cmos_sensor(0xA2BC,0xB5);//  
		HM5065MIPI_write_cmos_sensor(0xA2BD,0xC3);//  
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 14	 AF P74 06	 
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA2BE,0x90);//	 MOV	  DPTR,#0x0834
		HM5065MIPI_write_cmos_sensor(0xA2BF,0x08);//  
		HM5065MIPI_write_cmos_sensor(0xA2C0,0x34);//  
		HM5065MIPI_write_cmos_sensor(0xA2C1,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2C2,0xFC);//	 MOV	  R4,A
		HM5065MIPI_write_cmos_sensor(0xA2C3,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA2C4,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2C5,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA2C6,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA2C7,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2C8,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA2C9,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA2CA,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2CB,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA2CC,0x90);//	 MOV	  DPTR,#AutoFocusMeasureData(0x07D0);
		HM5065MIPI_write_cmos_sensor(0xA2CD,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA2CE,0xD0);//  
		HM5065MIPI_write_cmos_sensor(0xA2CF,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2D0,0xF8);//	 MOV	  R0,A
		HM5065MIPI_write_cmos_sensor(0xA2D1,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA2D2,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2D3,0xF9);//	 MOV	  R1,A
		HM5065MIPI_write_cmos_sensor(0xA2D4,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA2D5,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2D6,0xFA);//	 MOV	  R2,A
		HM5065MIPI_write_cmos_sensor(0xA2D7,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA2D8,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2D9,0xFB);//	 MOV	  R3,A
		HM5065MIPI_write_cmos_sensor(0xA2DA,0xD3);//	 SETB	  C
		HM5065MIPI_write_cmos_sensor(0xA2DB,0x12);//	 LCALL	  C?ULCMP(C:0DAE);
		HM5065MIPI_write_cmos_sensor(0xA2DC,0x0D);//  
		HM5065MIPI_write_cmos_sensor(0xA2DD,0xAE);//  
		HM5065MIPI_write_cmos_sensor(0xA2DE,0x40);//	 JC 	  B01:9FDA
		HM5065MIPI_write_cmos_sensor(0xA2DF,0x0B);//  
		HM5065MIPI_write_cmos_sensor(0xA2E0,0x12);//	 LCALL	  HCS_Initialization(B01:B0EF);
		HM5065MIPI_write_cmos_sensor(0xA2E1,0xB5);//  
		HM5065MIPI_write_cmos_sensor(0xA2E2,0x49);//  
		HM5065MIPI_write_cmos_sensor(0xA2E3,0x90);//	 MOV	  DPTR,#0x07A4
		HM5065MIPI_write_cmos_sensor(0xA2E4,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA2E5,0xA4);//  
		HM5065MIPI_write_cmos_sensor(0xA2E6,0x74);//	 MOV	  A,#0x02
		HM5065MIPI_write_cmos_sensor(0xA2E7,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA2E8,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA2E9,0x80);//	 SJMP	  B01:9FE3
		HM5065MIPI_write_cmos_sensor(0xA2EA,0x09);//  
		HM5065MIPI_write_cmos_sensor(0xA2EB,0x12);//	 LCALL	  LowFocusMeasureFullSearchInit(B01:B7AE);
		HM5065MIPI_write_cmos_sensor(0xA2EC,0xB7);//  
		HM5065MIPI_write_cmos_sensor(0xA2ED,0x51);//  
		HM5065MIPI_write_cmos_sensor(0xA2EE,0x90);//	 MOV	  DPTR,#0x07A4
		HM5065MIPI_write_cmos_sensor(0xA2EF,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA2F0,0xA4);//  
		HM5065MIPI_write_cmos_sensor(0xA2F1,0x74);//	 MOV	  A,#0x05
		HM5065MIPI_write_cmos_sensor(0xA2F2,0x05);//  
		HM5065MIPI_write_cmos_sensor(0xA2F3,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA2F4,0x02);//	 JUM
		HM5065MIPI_write_cmos_sensor(0xA2F5,0xA2);//  
		HM5065MIPI_write_cmos_sensor(0xA2F6,0xDA);//  
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 16	 OTP WB   
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA2F7,0x90);//	 MOV	  DPTR,#fOTPRed(0x0EE0);
		HM5065MIPI_write_cmos_sensor(0xA2F8,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA2F9,0xE0);//  
		HM5065MIPI_write_cmos_sensor(0xA2FA,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2FB,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA2FC,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA2FD,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA2FE,0x90);//	 MOV	  DPTR,#0x02A2
		HM5065MIPI_write_cmos_sensor(0xA2FF,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA300,0xA2);//  
		HM5065MIPI_write_cmos_sensor(0xA301,0xCD);//	 XCH	  A,R5
		HM5065MIPI_write_cmos_sensor(0xA302,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA303,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA304,0xED);//	 MOV	  A,R5
		HM5065MIPI_write_cmos_sensor(0xA305,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA306,0x90);//	 MOV	  DPTR,#fOTPBlue(0x0EE2);
		HM5065MIPI_write_cmos_sensor(0xA307,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA308,0xE2);//  
		HM5065MIPI_write_cmos_sensor(0xA309,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA30A,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA30B,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA30C,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA30D,0x90);//	 MOV	  DPTR,#0x02A8
		HM5065MIPI_write_cmos_sensor(0xA30E,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA30F,0xA8);//  
		HM5065MIPI_write_cmos_sensor(0xA310,0xCD);//	 XCH	  A,R5
		HM5065MIPI_write_cmos_sensor(0xA311,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA312,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA313,0xED);//	 MOV	  A,R5
		HM5065MIPI_write_cmos_sensor(0xA314,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA315,0xE4);//	 CLR	  A
		HM5065MIPI_write_cmos_sensor(0xA316,0x90);//	 MOV	  DPTR,#PresetControl(0x0638);
		HM5065MIPI_write_cmos_sensor(0xA317,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA318,0x38);//  
		HM5065MIPI_write_cmos_sensor(0xA319,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA31A,0x02);//	 JUMP	  #676
		HM5065MIPI_write_cmos_sensor(0xA31B,0x67);//  
		HM5065MIPI_write_cmos_sensor(0xA31C,0x63);//  
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 17	 OTP DarkCal scaling   
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA31D,0x90);//	 MOV	  DPTR,#bDarkCalSR(0x0EE8);
		HM5065MIPI_write_cmos_sensor(0xA31E,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA31F,0xE8);//  
		HM5065MIPI_write_cmos_sensor(0xA320,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA321,0x90);//	 MOV	  DPTR,#0x0262
		HM5065MIPI_write_cmos_sensor(0xA322,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA323,0x62);//  
		HM5065MIPI_write_cmos_sensor(0xA324,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA325,0x90);//	 MOV	  DPTR,#bDarkCalAB4(0x0EE9);
		HM5065MIPI_write_cmos_sensor(0xA326,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA327,0xE9);//  
		HM5065MIPI_write_cmos_sensor(0xA328,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA329,0x90);//	 MOV	  DPTR,#0x0263
		HM5065MIPI_write_cmos_sensor(0xA32A,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA32B,0x63);//  
		HM5065MIPI_write_cmos_sensor(0xA32C,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA32D,0x02);//	 JUMP	  #676
		HM5065MIPI_write_cmos_sensor(0xA32E,0x67);//  
		HM5065MIPI_write_cmos_sensor(0xA32F,0x1F);//  
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 18	 OTP DarkCal HFPN scaling	Replaced
		///////////////////////////////////////////////////////////////////////////////
		//HM5065MIPI_write_cmos_sensor(0xA330,0x90);//	   MOV		DPTR,#bDarkCalHFPN(0x0EE4);
		//HM5065MIPI_write_cmos_sensor(0xA331,0x0E);//	
		//HM5065MIPI_write_cmos_sensor(0xA332,0xE4);//	
		//HM5065MIPI_write_cmos_sensor(0xA333,0xE0);//	   MOVX 	A,@DPTR
		//HM5065MIPI_write_cmos_sensor(0xA334,0x90);//	   MOV		DPTR,#0x0266
		//HM5065MIPI_write_cmos_sensor(0xA335,0x02);//	
		//HM5065MIPI_write_cmos_sensor(0xA336,0x66);//	
		//HM5065MIPI_write_cmos_sensor(0xA337,0xF0);//	   MOVX 	@DPTR,A
		//HM5065MIPI_write_cmos_sensor(0xA338,0x02);//	   JUMP    
		//HM5065MIPI_write_cmos_sensor(0xA339,0x67);//	
		//HM5065MIPI_write_cmos_sensor(0xA33A,0xA5);//	
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 19	 OTP VCM	
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA33B,0x90);//	 MOV	  DPTR,#INFINITY_OTP(0xE014);
		HM5065MIPI_write_cmos_sensor(0xA33C,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA33D,0x14);//  
		HM5065MIPI_write_cmos_sensor(0xA33E,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA33F,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA340,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA341,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA342,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA343,0x90);//	 MOV	  DPTR,#0x06D9
		HM5065MIPI_write_cmos_sensor(0xA344,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA345,0xD9);//  
		HM5065MIPI_write_cmos_sensor(0xA346,0xEE);//	 MOV	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA347,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA348,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA349,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA34A,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA34B,0x90);//	 MOV	  DPTR,#DELTA_UP_OTP(0xE018);
		HM5065MIPI_write_cmos_sensor(0xA34C,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA34D,0x18);//  
		HM5065MIPI_write_cmos_sensor(0xA34E,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA34F,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA350,0x7C);//	 MOV	  R4,#DeviceParameters(0x00);
		HM5065MIPI_write_cmos_sensor(0xA351,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA352,0xC3);//	 CLR	  C
		HM5065MIPI_write_cmos_sensor(0xA353,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA354,0x9D);//	 SUBB	  A,R5
		HM5065MIPI_write_cmos_sensor(0xA355,0xEE);//	 MOV	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA356,0x9C);//	 SUBB	  A,R4
		HM5065MIPI_write_cmos_sensor(0xA357,0x50);//	 JNC	  C:2067
		HM5065MIPI_write_cmos_sensor(0xA358,0x09);//  
		HM5065MIPI_write_cmos_sensor(0xA359,0xE4);//	 CLR	  A
		HM5065MIPI_write_cmos_sensor(0xA35A,0x90);//	 MOV	  DPTR,#0x06D7
		HM5065MIPI_write_cmos_sensor(0xA35B,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA35C,0xD7);//  
		HM5065MIPI_write_cmos_sensor(0xA35D,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA35E,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA35F,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA360,0x80);//	 SJMP	  C:207A
		HM5065MIPI_write_cmos_sensor(0xA361,0x13);//  
		HM5065MIPI_write_cmos_sensor(0xA362,0xC3);//	 CLR	  C
		HM5065MIPI_write_cmos_sensor(0xA363,0x90);//	 MOV	  DPTR,#0x06DA
		HM5065MIPI_write_cmos_sensor(0xA364,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA365,0xDA);//  
		HM5065MIPI_write_cmos_sensor(0xA366,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA367,0x9D);//	 SUBB	  A,R5
		HM5065MIPI_write_cmos_sensor(0xA368,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA369,0x90);//	 MOV	  DPTR,#0x06D9
		HM5065MIPI_write_cmos_sensor(0xA36A,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA36B,0xD9);//  
		HM5065MIPI_write_cmos_sensor(0xA36C,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA36D,0x9C);//	 SUBB	  A,R4
		HM5065MIPI_write_cmos_sensor(0xA36E,0x90);//	 MOV	  DPTR,#0x06D7
		HM5065MIPI_write_cmos_sensor(0xA36F,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA370,0xD7);//  
		HM5065MIPI_write_cmos_sensor(0xA371,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA372,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA373,0xCE);//	 XCH	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA374,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA375,0x90);//	 MOV	  DPTR,#DELTA_UP_OTP(0xE018);
		HM5065MIPI_write_cmos_sensor(0xA376,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA377,0x18);//  
		HM5065MIPI_write_cmos_sensor(0xA378,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA379,0xF9);//	 MOV	  R1,A
		HM5065MIPI_write_cmos_sensor(0xA37A,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA37B,0x90);//	 MOV	  DPTR,#0x06C2
		HM5065MIPI_write_cmos_sensor(0xA37C,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA37D,0xC2);//  
		HM5065MIPI_write_cmos_sensor(0xA37E,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA37F,0xFC);//	 MOV	  R4,A
		HM5065MIPI_write_cmos_sensor(0xA380,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA381,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA382,0xFD);//	 MOV	  R5,A
		HM5065MIPI_write_cmos_sensor(0xA383,0xC3);//	 CLR	  C
		HM5065MIPI_write_cmos_sensor(0xA384,0x9F);//	 SUBB	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA385,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA386,0xEC);//	 MOV	  A,R4
		HM5065MIPI_write_cmos_sensor(0xA387,0x94);//	 SUBB	  A,#DeviceParameters(0x00);
		HM5065MIPI_write_cmos_sensor(0xA388,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA389,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA38A,0x90);//	 MOV	  DPTR,#MACRO_OTP(0xE016);
		HM5065MIPI_write_cmos_sensor(0xA38B,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA38C,0x16);//  
		HM5065MIPI_write_cmos_sensor(0xA38D,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA38E,0xFA);//	 MOV	  R2,A
		HM5065MIPI_write_cmos_sensor(0xA38F,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA390,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA391,0xFB);//	 MOV	  R3,A
		HM5065MIPI_write_cmos_sensor(0xA392,0xD3);//	 SETB	  C
		HM5065MIPI_write_cmos_sensor(0xA393,0x9F);//	 SUBB	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA394,0xEA);//	 MOV	  A,R2
		HM5065MIPI_write_cmos_sensor(0xA395,0x9E);//	 SUBB	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA396,0x40);//	 JC 	  C:20A7
		HM5065MIPI_write_cmos_sensor(0xA397,0x0A);//  
		HM5065MIPI_write_cmos_sensor(0xA398,0x90);//	 MOV	  DPTR,#0x06D5
		HM5065MIPI_write_cmos_sensor(0xA399,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA39A,0xD5);//  
		HM5065MIPI_write_cmos_sensor(0xA39B,0xEC);//	 MOV	  A,R4
		HM5065MIPI_write_cmos_sensor(0xA39C,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA39D,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA39E,0xED);//	 MOV	  A,R5
		HM5065MIPI_write_cmos_sensor(0xA39F,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA3A0,0x80);//	 SJMP	  C:20B5
		HM5065MIPI_write_cmos_sensor(0xA3A1,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA3A2,0xE9);//	 MOV	  A,R1
		HM5065MIPI_write_cmos_sensor(0xA3A3,0x7E);//	 MOV	  R6,#DeviceParameters(0x00);
		HM5065MIPI_write_cmos_sensor(0xA3A4,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA3A5,0x2B);//	 ADD	  A,R3
		HM5065MIPI_write_cmos_sensor(0xA3A6,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA3A7,0xEE);//	 MOV	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA3A8,0x3A);//	 ADDC	  A,R2
		HM5065MIPI_write_cmos_sensor(0xA3A9,0x90);//	 MOV	  DPTR,#0x06D5
		HM5065MIPI_write_cmos_sensor(0xA3AA,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA3AB,0xD5);//  
		HM5065MIPI_write_cmos_sensor(0xA3AC,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA3AD,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA3AE,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA3AF,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA3B0,0xE9);//	 MOV	  A,R1
		HM5065MIPI_write_cmos_sensor(0xA3B1,0xFB);//	 MOV	  R3,A
		HM5065MIPI_write_cmos_sensor(0xA3B2,0x7A);//	 MOV	  R2,#DeviceParameters(0x00);
		HM5065MIPI_write_cmos_sensor(0xA3B3,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA3B4,0x90);//	 MOV	  DPTR,#0xE015
		HM5065MIPI_write_cmos_sensor(0xA3B5,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA3B6,0x15);//  
		HM5065MIPI_write_cmos_sensor(0xA3B7,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA3B8,0x2B);//	 ADD	  A,R3
		HM5065MIPI_write_cmos_sensor(0xA3B9,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA3BA,0x90);//	 MOV	  DPTR,#INFINITY_OTP(0xE014);
		HM5065MIPI_write_cmos_sensor(0xA3BB,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA3BC,0x14);//  
		HM5065MIPI_write_cmos_sensor(0xA3BD,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA3BE,0x3A);//	 ADDC	  A,R2
		HM5065MIPI_write_cmos_sensor(0xA3BF,0x90);//	 MOV	  DPTR,#0x06E1
		HM5065MIPI_write_cmos_sensor(0xA3C0,0x06);//  
		HM5065MIPI_write_cmos_sensor(0xA3C1,0xE1);//  
		HM5065MIPI_write_cmos_sensor(0xA3C2,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA3C3,0xA3);//	 INC	  DPTR
		HM5065MIPI_write_cmos_sensor(0xA3C4,0xCE);//	 XCH	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA3C5,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA3C6,0xC3);//	 CLR	  C
		HM5065MIPI_write_cmos_sensor(0xA3C7,0x90);//	 MOV	  DPTR,#0xE017
		HM5065MIPI_write_cmos_sensor(0xA3C8,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA3C9,0x17);//  
		HM5065MIPI_write_cmos_sensor(0xA3CA,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA3CB,0x9B);//	 SUBB	  A,R3
		HM5065MIPI_write_cmos_sensor(0xA3CC,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA3CD,0x90);//	 MOV	  DPTR,#MACRO_OTP(0xE016);
		HM5065MIPI_write_cmos_sensor(0xA3CE,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA3CF,0x16);//  
		HM5065MIPI_write_cmos_sensor(0xA3D0,0x02);//	 JUMP	 
		HM5065MIPI_write_cmos_sensor(0xA3D1,0x20);//  
		HM5065MIPI_write_cmos_sensor(0xA3D2,0xD5);//  
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 18	 OTP DarkCal HFPN scaling	Replaced
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA3D3,0x90);//	 MOV	  DPTR,#bDarkCalHFPN(0x0EE4);
		HM5065MIPI_write_cmos_sensor(0xA3d4,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA3d5,0xE4);//  
		HM5065MIPI_write_cmos_sensor(0xA3d6,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA3d7,0x90);//	 MOV	  DPTR,#0x0266
		HM5065MIPI_write_cmos_sensor(0xA3d8,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA3d9,0x66);//  
		HM5065MIPI_write_cmos_sensor(0xA3da,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA3DB,0x90);//	 MOV	  DPTR,#bDarkCalHFPN(0x0EE4);
		HM5065MIPI_write_cmos_sensor(0xA3dc,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA3dd,0xE5);//  
		HM5065MIPI_write_cmos_sensor(0xA3de,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA3df,0x90);//	 MOV	  DPTR,#0x0266
		HM5065MIPI_write_cmos_sensor(0xA3e0,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA3e1,0x64);//  
		HM5065MIPI_write_cmos_sensor(0xA3e2,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA3e3,0x90);//	 MOV	  DPTR,#bDarkCalHFPN(0x0EE4);
		HM5065MIPI_write_cmos_sensor(0xA3e4,0x0E);//  
		HM5065MIPI_write_cmos_sensor(0xA3e5,0xE6);//  
		HM5065MIPI_write_cmos_sensor(0xA3e6,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA3e7,0x90);//	 MOV	  DPTR,#0x0266
		HM5065MIPI_write_cmos_sensor(0xA3e8,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA3e9,0x65);//  
		HM5065MIPI_write_cmos_sensor(0xA3ea,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA3eb,0x02);//	 JUMP	 
		HM5065MIPI_write_cmos_sensor(0xA3ec,0x67);//  
		HM5065MIPI_write_cmos_sensor(0xA3ed,0xA5);//  
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 20	 TWakeup work around for PLL3 div
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA3f0,0x12);//  
		HM5065MIPI_write_cmos_sensor(0xA3f1,0x47);//  
		HM5065MIPI_write_cmos_sensor(0xA3f2,0x59);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA3f3,0x90);//	 MOV	  DPTR,#bDarkCalHFPN(0x0EE4);
		HM5065MIPI_write_cmos_sensor(0xA3f4,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA3f5,0xB5);//  
		HM5065MIPI_write_cmos_sensor(0xA3f6,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA3f7,0xB4);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA3f8,0x02);//	 MOV	  DPTR,#0x0266
		HM5065MIPI_write_cmos_sensor(0xA3f9,0x03);//  
		HM5065MIPI_write_cmos_sensor(0xA3fa,0x12);//  
		HM5065MIPI_write_cmos_sensor(0xA3fb,0x47);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA3fc,0x59);//	 JUMP	 
		HM5065MIPI_write_cmos_sensor(0xA3fd,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA3fe,0xC5);//  
		HM5065MIPI_write_cmos_sensor(0xA3ff,0xC3);//  
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 23	 AV 2x2
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA400,0x90);//	 MOV	  DPTR,#c_HFlip(0x003D);
		HM5065MIPI_write_cmos_sensor(0xA401,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA402,0x3D);//  
		HM5065MIPI_write_cmos_sensor(0xA403,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA404,0x90);//	 MOV	  DPTR,#0x0084
		HM5065MIPI_write_cmos_sensor(0xA405,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA406,0x84);//  
		HM5065MIPI_write_cmos_sensor(0xA407,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA408,0xFE);//	 MOV	  R6,A
		HM5065MIPI_write_cmos_sensor(0xA409,0x90);//	 MOV	  DPTR,#c_VFlip(0x003E);
		HM5065MIPI_write_cmos_sensor(0xA40A,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA40B,0x3E);//  
		HM5065MIPI_write_cmos_sensor(0xA40C,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA40D,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA40E,0x70);//	 JNZ	  B00:8201
		HM5065MIPI_write_cmos_sensor(0xA40F,0x03);//  
		HM5065MIPI_write_cmos_sensor(0xA410,0xEE);//	 MOV	  A,R6
		HM5065MIPI_write_cmos_sensor(0xA411,0x60);//	 JZ 	  B00:8205
		HM5065MIPI_write_cmos_sensor(0xA412,0x04);//  
		HM5065MIPI_write_cmos_sensor(0xA413,0x7F);//	 MOV	  R7,#0x01
		HM5065MIPI_write_cmos_sensor(0xA414,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA415,0x80);//	 SJMP	  B00:8207
		HM5065MIPI_write_cmos_sensor(0xA416,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA417,0x7F);//	 MOV	  R7,#DeviceParameters(0x00);
		HM5065MIPI_write_cmos_sensor(0xA418,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA419,0x90);//	 MOV	  DPTR,#c_HVFlip(0x003F);
		HM5065MIPI_write_cmos_sensor(0xA41A,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA41B,0x3F);//  
		HM5065MIPI_write_cmos_sensor(0xA41C,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA41D,0xF0);//	 MOVX	  @DPTR,A
		HM5065MIPI_write_cmos_sensor(0xA41E,0x02);//	 JUMP bac
		HM5065MIPI_write_cmos_sensor(0xA41F,0x89);//  
		HM5065MIPI_write_cmos_sensor(0xA420,0xD3);//  
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 24	CSI DL JPEG clk /2
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA421,0x90);//	 MOV	  DPTR,#uwI2CSIndex(0x0012);
		HM5065MIPI_write_cmos_sensor(0xA422,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA423,0x12);//  
		HM5065MIPI_write_cmos_sensor(0xA424,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA425,0xFF);//	 MOV	  R7,A
		HM5065MIPI_write_cmos_sensor(0xA426,0x70);//	 JNZ	  B00:9AC3
		HM5065MIPI_write_cmos_sensor(0xA427,0x0C);//  
		HM5065MIPI_write_cmos_sensor(0xA428,0x90);//	 MOV	  DPTR,#0x0046
		HM5065MIPI_write_cmos_sensor(0xA429,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA42A,0x46);//  
		HM5065MIPI_write_cmos_sensor(0xA42B,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA42C,0xC3);//	 CLR	  C
		HM5065MIPI_write_cmos_sensor(0xA42D,0x94);//	 SUBB	  A,#0x07
		HM5065MIPI_write_cmos_sensor(0xA42E,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA42F,0x40);//	 JC 	  B00:9AC3
		HM5065MIPI_write_cmos_sensor(0xA430,0x03);//  
		HM5065MIPI_write_cmos_sensor(0xA431,0x75);//	 MOV	  0x2E,#0x02
		HM5065MIPI_write_cmos_sensor(0xA432,0x2E);//  
		HM5065MIPI_write_cmos_sensor(0xA433,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA434,0xEF);//	 MOV	  A,R7
		HM5065MIPI_write_cmos_sensor(0xA435,0xB4);//	 CJNE	  A,#0x01,B00:9AD3
		HM5065MIPI_write_cmos_sensor(0xA436,0x01);//  
		HM5065MIPI_write_cmos_sensor(0xA437,0x0C);//  
		HM5065MIPI_write_cmos_sensor(0xA438,0x90);//	 MOV	  DPTR,#XDroop_Reverse_Croping(0x0066);
		HM5065MIPI_write_cmos_sensor(0xA439,0x00);//  
		HM5065MIPI_write_cmos_sensor(0xA43A,0x66);//  
		HM5065MIPI_write_cmos_sensor(0xA43B,0xE0);//	 MOVX	  A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA43C,0xC3);//	 CLR	  C
		HM5065MIPI_write_cmos_sensor(0xA43D,0x94);//	 SUBB	  A,#0x07
		HM5065MIPI_write_cmos_sensor(0xA43E,0x07);//  
		HM5065MIPI_write_cmos_sensor(0xA43F,0x40);//	 JC 	  B00:9AD3
		HM5065MIPI_write_cmos_sensor(0xA440,0x03);//  
		HM5065MIPI_write_cmos_sensor(0xA441,0x75);//	 MOV	  0x2E,#0x02
		HM5065MIPI_write_cmos_sensor(0xA442,0x2E);//  
		HM5065MIPI_write_cmos_sensor(0xA443,0x02);//  
		HM5065MIPI_write_cmos_sensor(0xA444,0x02);//	 JUMP	
		HM5065MIPI_write_cmos_sensor(0xA445,0xA7);//  
		HM5065MIPI_write_cmos_sensor(0xA446,0x9E);//  
		
		///////////////////////////////////////////////////////////////////////////////
		// BP  26	 freeze white balance at low stat <0x4480 10
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA447,0xC3);//	CLR 	 C
		HM5065MIPI_write_cmos_sensor(0xA448,0x90);//	MOV 	 DPTR,#0x0B8F
		HM5065MIPI_write_cmos_sensor(0xA449,0x0B);// 
		HM5065MIPI_write_cmos_sensor(0xA44A,0x8F);// 
		HM5065MIPI_write_cmos_sensor(0xA44B,0xE0);//	MOVX	 A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA44C,0x94);//	SUBB	 A,#PipeSetupCommon(0x80);
		HM5065MIPI_write_cmos_sensor(0xA44D,0x00);// 
		HM5065MIPI_write_cmos_sensor(0xA44E,0x90);//	MOV 	 DPTR,#CalculateNormalisedStatistics?BYTE(0x0B8E);
		HM5065MIPI_write_cmos_sensor(0xA44F,0x0B);// 
		HM5065MIPI_write_cmos_sensor(0xA450,0x8E);// 
		HM5065MIPI_write_cmos_sensor(0xA451,0xE0);//	MOVX	 A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA452,0x94);//	SUBB	 A,#0x44
		HM5065MIPI_write_cmos_sensor(0xA453,0x41);// 
		HM5065MIPI_write_cmos_sensor(0xA454,0x40);//	JC		 B00:827D
		HM5065MIPI_write_cmos_sensor(0xA455,0x22);// 
		HM5065MIPI_write_cmos_sensor(0xA456,0x90);//	MOV 	 DPTR,#0x0B91
		HM5065MIPI_write_cmos_sensor(0xA457,0x0B);// 
		HM5065MIPI_write_cmos_sensor(0xA458,0x91);// 
		HM5065MIPI_write_cmos_sensor(0xA459,0xE0);//	MOVX	 A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA45A,0x94);//	SUBB	 A,#PipeSetupCommon(0x80);
		HM5065MIPI_write_cmos_sensor(0xA45B,0x00);// 
		HM5065MIPI_write_cmos_sensor(0xA45C,0x90);//	MOV 	 DPTR,#0x0B90
		HM5065MIPI_write_cmos_sensor(0xA45D,0x0B);// 
		HM5065MIPI_write_cmos_sensor(0xA45E,0x90);// 
		HM5065MIPI_write_cmos_sensor(0xA45F,0xE0);//	MOVX	 A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA460,0x94);//	SUBB	 A,#0x44
		HM5065MIPI_write_cmos_sensor(0xA461,0x41);// 
		HM5065MIPI_write_cmos_sensor(0xA462,0x40);//	JC		 B00:827D
		HM5065MIPI_write_cmos_sensor(0xA463,0x14);// 
		HM5065MIPI_write_cmos_sensor(0xA464,0x90);//	MOV 	 DPTR,#0x0B93
		HM5065MIPI_write_cmos_sensor(0xA465,0x0B);// 
		HM5065MIPI_write_cmos_sensor(0xA466,0x93);// 
		HM5065MIPI_write_cmos_sensor(0xA467,0xE0);//	MOVX	 A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA468,0x94);//	SUBB	 A,#PipeSetupCommon(0x80);
		HM5065MIPI_write_cmos_sensor(0xA469,0x00);// 
		HM5065MIPI_write_cmos_sensor(0xA46A,0x90);//	MOV 	 DPTR,#0x0B92
		HM5065MIPI_write_cmos_sensor(0xA46B,0x0B);// 
		HM5065MIPI_write_cmos_sensor(0xA46C,0x92);// 
		HM5065MIPI_write_cmos_sensor(0xA46D,0xE0);//	MOVX	 A,@DPTR
		HM5065MIPI_write_cmos_sensor(0xA46E,0x94);//	SUBB	 A,#0x44
		HM5065MIPI_write_cmos_sensor(0xA46F,0x41);// 
		HM5065MIPI_write_cmos_sensor(0xA470,0x40);//	JC		 B00:827D
		HM5065MIPI_write_cmos_sensor(0xA471,0x06);// 
		HM5065MIPI_write_cmos_sensor(0xA472,0x90);//	MOV 	 DPTR,#0x01A4
		HM5065MIPI_write_cmos_sensor(0xA473,0x01);// 
		HM5065MIPI_write_cmos_sensor(0xA474,0xA4);// 
		HM5065MIPI_write_cmos_sensor(0xA475,0x02);//	LJMP	 back
		HM5065MIPI_write_cmos_sensor(0xA476,0x86);// 
		HM5065MIPI_write_cmos_sensor(0xA477,0x57);// 
		HM5065MIPI_write_cmos_sensor(0xA478,0x02);//	LJMP	 back
		HM5065MIPI_write_cmos_sensor(0xA479,0x86);// 
		HM5065MIPI_write_cmos_sensor(0xA47A,0x5C);//																
		
		///////////////////////////////////////////////////////////////////////////////
		// BP 11	 AF light detector	 using Xdata 0x0DF6/7  
		///////////////////////////////////////////////////////////////////////////////
		HM5065MIPI_write_cmos_sensor(0xA500,0xF5);//	  //	MOV 	 c_HeightScale(0x3B);,A 							 
		HM5065MIPI_write_cmos_sensor(0xA501,0x3B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA502,0x90);//	  //	MOV 	 DPTR,#0x066C										
		HM5065MIPI_write_cmos_sensor(0xA503,0x06);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA504,0x6C);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA505,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA506,0xFF);//	  //	MOV 	 R7,A												
		HM5065MIPI_write_cmos_sensor(0xA507,0xE5);//	  //	MOV 	 A,c_HeightScale(0x3B); 							 
		HM5065MIPI_write_cmos_sensor(0xA508,0x3B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA509,0xC3);//	  //	CLR 	 C													
		HM5065MIPI_write_cmos_sensor(0xA50A,0x9F);//	  //	SUBB	 A,R7												
		HM5065MIPI_write_cmos_sensor(0xA50B,0x40);//	  //	JC		 B01:98E8											
		HM5065MIPI_write_cmos_sensor(0xA50C,0x03);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA50D,0x02);//	  //	LJMP	 B01:99E6											
		HM5065MIPI_write_cmos_sensor(0xA50E,0xF6);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA50F,0x0E);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA510,0x90);//	  //	MOV 	 DPTR,#0x0BC6										
		HM5065MIPI_write_cmos_sensor(0xA511,0x0B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA512,0xC6);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA513,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA514,0x14);//	  //	DEC 	 A													
		HM5065MIPI_write_cmos_sensor(0xA515,0x60);//	  //	JZ		 B01:992B											
		HM5065MIPI_write_cmos_sensor(0xA516,0x3C);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA517,0x14);//	  //	DEC 	 A													
		HM5065MIPI_write_cmos_sensor(0xA518,0x60);//	  //	JZ		 B01:995D											
		HM5065MIPI_write_cmos_sensor(0xA519,0x6B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA51A,0x24);//	  //	ADD 	 A,#0x02											
		HM5065MIPI_write_cmos_sensor(0xA51B,0x02);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA51C,0x60);//	  //	JZ		 B01:98F9											
		HM5065MIPI_write_cmos_sensor(0xA51D,0x03);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA51E,0x02);//	  //	LJMP	 B01:998D											
		HM5065MIPI_write_cmos_sensor(0xA51F,0xF5);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA520,0xB5);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA521,0x90);//	  //	MOV 	 DPTR,#AutoFocusInstableFocusMeasureValues(0x0A9A);  
		HM5065MIPI_write_cmos_sensor(0xA522,0x0A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA523,0x9A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA524,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA525,0xFB);//	  //	MOV 	 R3,A												
		HM5065MIPI_write_cmos_sensor(0xA526,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA527,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA528,0xFA);//	  //	MOV 	 R2,A												
		HM5065MIPI_write_cmos_sensor(0xA529,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA52A,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA52B,0xF9);//	  //	MOV 	 R1,A												
		HM5065MIPI_write_cmos_sensor(0xA52C,0x85);//	  //	MOV 	 DPL(0x82);,c_HeightScale(0x3B);					  
		HM5065MIPI_write_cmos_sensor(0xA52D,0x3B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA52E,0x82);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA52F,0x75);//	  //	MOV 	 DPH(0x83);,#DeviceParameters(0x00);				  
		HM5065MIPI_write_cmos_sensor(0xA530,0x83);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA531,0x00);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA532,0x12);//	  //	LCALL	 C?CLDOPTR(C:0AB8); 								 
		HM5065MIPI_write_cmos_sensor(0xA533,0x0A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA534,0xB8);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA535,0xFF);//	  //	MOV 	 R7,A												
		HM5065MIPI_write_cmos_sensor(0xA536,0x74);//	  //	MOV 	 A,#0xAB											
		HM5065MIPI_write_cmos_sensor(0xA537,0xAB);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA538,0x25);//	  //	ADD 	 A,c_HeightScale(0x3B); 							 
		HM5065MIPI_write_cmos_sensor(0xA539,0x3B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA53A,0xF5);//	  //	MOV 	 DPL(0x82);,A										 
		HM5065MIPI_write_cmos_sensor(0xA53B,0x82);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA53C,0xE4);//	  //	CLR 	 A													
		HM5065MIPI_write_cmos_sensor(0xA53D,0x34);//	  //	ADDC	 A,#bInt_Event_Status(0x0A);						 
		HM5065MIPI_write_cmos_sensor(0xA53E,0x0A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA53F,0xF5);//	  //	MOV 	 DPH(0x83);,A										 
		HM5065MIPI_write_cmos_sensor(0xA540,0x83);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA541,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA542,0xFD);//	  //	MOV 	 R5,A												
		HM5065MIPI_write_cmos_sensor(0xA543,0xC3);//	  //	CLR 	 C													
		HM5065MIPI_write_cmos_sensor(0xA544,0xEF);//	  //	MOV 	 A,R7												
		HM5065MIPI_write_cmos_sensor(0xA545,0x9D);//	  //	SUBB	 A,R5												
		HM5065MIPI_write_cmos_sensor(0xA546,0xFE);//	  //	MOV 	 R6,A												
		HM5065MIPI_write_cmos_sensor(0xA547,0xE4);//	  //	CLR 	 A													
		HM5065MIPI_write_cmos_sensor(0xA548,0x94);//	  //	SUBB	 A,#DeviceParameters(0x00); 						 
		HM5065MIPI_write_cmos_sensor(0xA549,0x00);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA54A,0x90);//	  //	MOV 	 DPTR,#0x0BCA										
		HM5065MIPI_write_cmos_sensor(0xA54B,0x0B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA54C,0xCA);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA54D,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA54E,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA54F,0xCE);//	  //	XCH 	 A,R6												
		HM5065MIPI_write_cmos_sensor(0xA550,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA551,0x80);//	  //	SJMP	 B01:998D											
		HM5065MIPI_write_cmos_sensor(0xA552,0x62);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA553,0x90);//	  //	MOV 	 DPTR,#AutoFocusInstableFocusMeasureValues(0x0A9A);  
		HM5065MIPI_write_cmos_sensor(0xA554,0x0A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA555,0x9A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA556,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA557,0xFB);//	  //	MOV 	 R3,A												
		HM5065MIPI_write_cmos_sensor(0xA558,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA559,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA55A,0xFA);//	  //	MOV 	 R2,A												
		HM5065MIPI_write_cmos_sensor(0xA55B,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA55C,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA55D,0xF9);//	  //	MOV 	 R1,A												
		HM5065MIPI_write_cmos_sensor(0xA55E,0x85);//	  //	MOV 	 DPL(0x82);,c_HeightScale(0x3B);					  
		HM5065MIPI_write_cmos_sensor(0xA55F,0x3B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA560,0x82);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA561,0x75);//	  //	MOV 	 DPH(0x83);,#DeviceParameters(0x00);				  
		HM5065MIPI_write_cmos_sensor(0xA562,0x83);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA563,0x00);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA564,0x12);//	  //	LCALL	 C?CLDOPTR(C:0AB8); 								 
		HM5065MIPI_write_cmos_sensor(0xA565,0x0A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA566,0xB8);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA567,0xFF);//	  //	MOV 	 R7,A												
		HM5065MIPI_write_cmos_sensor(0xA568,0x74);//	  //	MOV 	 A,#0x9D											
		HM5065MIPI_write_cmos_sensor(0xA569,0x9D);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA56A,0x25);//	  //	ADD 	 A,c_HeightScale(0x3B); 							 
		HM5065MIPI_write_cmos_sensor(0xA56B,0x3B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA56C,0xF5);//	  //	MOV 	 DPL(0x82);,A										 
		HM5065MIPI_write_cmos_sensor(0xA56D,0x82);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA56E,0xE4);//	  //	CLR 	 A													
		HM5065MIPI_write_cmos_sensor(0xA56F,0x34);//	  //	ADDC	 A,#bInt_Event_Status(0x0A);						 
		HM5065MIPI_write_cmos_sensor(0xA570,0x0A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA571,0xF5);//	  //	MOV 	 DPH(0x83);,A										 
		HM5065MIPI_write_cmos_sensor(0xA572,0x83);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA573,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA574,0xFD);//	  //	MOV 	 R5,A												
		HM5065MIPI_write_cmos_sensor(0xA575,0xC3);//	  //	CLR 	 C													
		HM5065MIPI_write_cmos_sensor(0xA576,0xEF);//	  //	MOV 	 A,R7												
		HM5065MIPI_write_cmos_sensor(0xA577,0x9D);//	  //	SUBB	 A,R5												
		HM5065MIPI_write_cmos_sensor(0xA578,0xFE);//	  //	MOV 	 R6,A												
		HM5065MIPI_write_cmos_sensor(0xA579,0xE4);//	  //	CLR 	 A													
		HM5065MIPI_write_cmos_sensor(0xA57A,0x94);//	  //	SUBB	 A,#DeviceParameters(0x00); 						 
		HM5065MIPI_write_cmos_sensor(0xA57B,0x00);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA57C,0x90);//	  //	MOV 	 DPTR,#0x0BCA										
		HM5065MIPI_write_cmos_sensor(0xA57D,0x0B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA57E,0xCA);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA57F,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA580,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA581,0xCE);//	  //	XCH 	 A,R6												
		HM5065MIPI_write_cmos_sensor(0xA582,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA583,0x80);//	  //	SJMP	 B01:998D											
		HM5065MIPI_write_cmos_sensor(0xA584,0x30);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA585,0x90);//	  //	MOV 	 DPTR,#AutoFocusInstableFocusMeasureValues(0x0A9A);  
		HM5065MIPI_write_cmos_sensor(0xA586,0x0A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA587,0x9A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA588,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA589,0xFB);//	  //	MOV 	 R3,A												
		HM5065MIPI_write_cmos_sensor(0xA58A,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA58B,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA58C,0xFA);//	  //	MOV 	 R2,A												
		HM5065MIPI_write_cmos_sensor(0xA58D,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA58E,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA58F,0xF9);//	  //	MOV 	 R1,A												
		HM5065MIPI_write_cmos_sensor(0xA590,0x85);//	  //	MOV 	 DPL(0x82);,c_HeightScale(0x3B);					  
		HM5065MIPI_write_cmos_sensor(0xA591,0x3B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA592,0x82);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA593,0x75);//	  //	MOV 	 DPH(0x83);,#DeviceParameters(0x00);				  
		HM5065MIPI_write_cmos_sensor(0xA594,0x83);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA595,0x00);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA596,0x12);//	  //	LCALL	 C?CLDOPTR(C:0AB8); 								 
		HM5065MIPI_write_cmos_sensor(0xA597,0x0A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA598,0xB8);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA599,0xFF);//	  //	MOV 	 R7,A												
		HM5065MIPI_write_cmos_sensor(0xA59A,0x74);//	  //	MOV 	 A,#0xA4											
		HM5065MIPI_write_cmos_sensor(0xA59B,0xA4);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA59C,0x25);//	  //	ADD 	 A,c_HeightScale(0x3B); 							 
		HM5065MIPI_write_cmos_sensor(0xA59D,0x3B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA59E,0xF5);//	  //	MOV 	 DPL(0x82);,A										 
		HM5065MIPI_write_cmos_sensor(0xA59F,0x82);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5A0,0xE4);//	  //	CLR 	 A													
		HM5065MIPI_write_cmos_sensor(0xA5A1,0x34);//	  //	ADDC	 A,#bInt_Event_Status(0x0A);						 
		HM5065MIPI_write_cmos_sensor(0xA5A2,0x0A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5A3,0xF5);//	  //	MOV 	 DPH(0x83);,A										 
		HM5065MIPI_write_cmos_sensor(0xA5A4,0x83);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5A5,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA5A6,0xFD);//	  //	MOV 	 R5,A												
		HM5065MIPI_write_cmos_sensor(0xA5A7,0xC3);//	  //	CLR 	 C													
		HM5065MIPI_write_cmos_sensor(0xA5A8,0xEF);//	  //	MOV 	 A,R7												
		HM5065MIPI_write_cmos_sensor(0xA5A9,0x9D);//	  //	SUBB	 A,R5												
		HM5065MIPI_write_cmos_sensor(0xA5AA,0xFE);//	  //	MOV 	 R6,A												
		HM5065MIPI_write_cmos_sensor(0xA5AB,0xE4);//	  //	CLR 	 A													
		HM5065MIPI_write_cmos_sensor(0xA5AC,0x94);//	  //	SUBB	 A,#DeviceParameters(0x00); 						 
		HM5065MIPI_write_cmos_sensor(0xA5AD,0x00);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5AE,0x90);//	  //	MOV 	 DPTR,#0x0BCA										
		HM5065MIPI_write_cmos_sensor(0xA5AF,0x0B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5B0,0xCA);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5B1,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA5B2,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA5B3,0xCE);//	  //	XCH 	 A,R6												
		HM5065MIPI_write_cmos_sensor(0xA5B4,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA5B5,0x90);//	  //	MOV 	 DPTR,#0x0783										
		HM5065MIPI_write_cmos_sensor(0xA5B6,0x07);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5B7,0x83);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5B8,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA5B9,0xFF);//	  //	MOV 	 R7,A												
		HM5065MIPI_write_cmos_sensor(0xA5BA,0x7E);//	  //	MOV 	 R6,#DeviceParameters(0x00);						 
		HM5065MIPI_write_cmos_sensor(0xA5BB,0x00);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5BC,0x90);//	  //	MOV 	 DPTR,#patch_wLightGap(0x0DF6); 					 
		HM5065MIPI_write_cmos_sensor(0xA5BD,0x0D);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5BE,0xF6);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5BF,0xEE);//	  //	MOV 	 A,R6												
		HM5065MIPI_write_cmos_sensor(0xA5C0,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA5C1,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA5C2,0xEF);//	  //	MOV 	 A,R7												
		HM5065MIPI_write_cmos_sensor(0xA5C3,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA5C4,0x90);//	  //	MOV 	 DPTR,#0x0BCA										
		HM5065MIPI_write_cmos_sensor(0xA5C5,0x0B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5C6,0xCA);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5C7,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA5C8,0xFC);//	  //	MOV 	 R4,A												
		HM5065MIPI_write_cmos_sensor(0xA5C9,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA5CA,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA5CB,0xFD);//	  //	MOV 	 R5,A												
		HM5065MIPI_write_cmos_sensor(0xA5CC,0xD3);//	  //	SETB	 C													
		HM5065MIPI_write_cmos_sensor(0xA5CD,0x9F);//	  //	SUBB	 A,R7												
		HM5065MIPI_write_cmos_sensor(0xA5CE,0x74);//	  //	MOV 	 A,#PipeSetupCommon(0x80);							 
		HM5065MIPI_write_cmos_sensor(0xA5CF,0x80);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5D0,0xF8);//	  //	MOV 	 R0,A												
		HM5065MIPI_write_cmos_sensor(0xA5D1,0xEC);//	  //	MOV 	 A,R4												
		HM5065MIPI_write_cmos_sensor(0xA5D2,0x64);//	  //	XRL 	 A,#PipeSetupCommon(0x80);							 
		HM5065MIPI_write_cmos_sensor(0xA5D3,0x80);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5D4,0x98);//	  //	SUBB	 A,R0												
		HM5065MIPI_write_cmos_sensor(0xA5D5,0x40);//	  //	JC		 B01:99BB											
		HM5065MIPI_write_cmos_sensor(0xA5D6,0x0C);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5D7,0x90);//	  //	MOV 	 DPTR,#0x0BC8										
		HM5065MIPI_write_cmos_sensor(0xA5D8,0x0B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5D9,0xC8);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5DA,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA5DB,0x04);//	  //	INC 	 A													
		HM5065MIPI_write_cmos_sensor(0xA5DC,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA5DD,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA5DE,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA5DF,0x04);//	  //	INC 	 A													
		HM5065MIPI_write_cmos_sensor(0xA5E0,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA5E1,0x80);//	  //	SJMP	 B01:99E1											
		HM5065MIPI_write_cmos_sensor(0xA5E2,0x26);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5E3,0x90);//	  //	MOV 	 DPTR,#patch_wLightGap(0x0DF6); 					 
		HM5065MIPI_write_cmos_sensor(0xA5E4,0x0D);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5E5,0xF6);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5E6,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA5E7,0xFE);//	  //	MOV 	 R6,A												
		HM5065MIPI_write_cmos_sensor(0xA5E8,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA5E9,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA5EA,0xFF);//	  //	MOV 	 R7,A												
		HM5065MIPI_write_cmos_sensor(0xA5EB,0xC3);//	  //	CLR 	 C													
		HM5065MIPI_write_cmos_sensor(0xA5EC,0xE4);//	  //	CLR 	 A													
		HM5065MIPI_write_cmos_sensor(0xA5ED,0x9F);//	  //	SUBB	 A,R7												
		HM5065MIPI_write_cmos_sensor(0xA5EE,0xFF);//	  //	MOV 	 R7,A												
		HM5065MIPI_write_cmos_sensor(0xA5EF,0xE4);//	  //	CLR 	 A													
		HM5065MIPI_write_cmos_sensor(0xA5F0,0x9E);//	  //	SUBB	 A,R6												
		HM5065MIPI_write_cmos_sensor(0xA5F1,0xFE);//	  //	MOV 	 R6,A												
		HM5065MIPI_write_cmos_sensor(0xA5F2,0xC3);//	  //	CLR 	 C													
		HM5065MIPI_write_cmos_sensor(0xA5F3,0xED);//	  //	MOV 	 A,R5												
		HM5065MIPI_write_cmos_sensor(0xA5F4,0x9F);//	  //	SUBB	 A,R7												
		HM5065MIPI_write_cmos_sensor(0xA5F5,0xEE);//	  //	MOV 	 A,R6												
		HM5065MIPI_write_cmos_sensor(0xA5F6,0x64);//	  //	XRL 	 A,#PipeSetupCommon(0x80);							 
		HM5065MIPI_write_cmos_sensor(0xA5F7,0x80);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5F8,0xF8);//	  //	MOV 	 R0,A												
		HM5065MIPI_write_cmos_sensor(0xA5F9,0xEC);//	  //	MOV 	 A,R4												
		HM5065MIPI_write_cmos_sensor(0xA5FA,0x64);//	  //	XRL 	 A,#PipeSetupCommon(0x80);							 
		HM5065MIPI_write_cmos_sensor(0xA5FB,0x80);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5FC,0x98);//	  //	SUBB	 A,R0												
		HM5065MIPI_write_cmos_sensor(0xA5FD,0x50);//	  //	JNC 	 B01:99E1											
		HM5065MIPI_write_cmos_sensor(0xA5FE,0x0A);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA5FF,0x90);//	  //	MOV 	 DPTR,#0x0BC8										
		HM5065MIPI_write_cmos_sensor(0xA600,0x0B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA601,0xC8);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA602,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA603,0x14);//	  //	DEC 	 A													
		HM5065MIPI_write_cmos_sensor(0xA604,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA605,0xA3);//	  //	INC 	 DPTR												
		HM5065MIPI_write_cmos_sensor(0xA606,0xE0);//	  //	MOVX	 A,@DPTR											
		HM5065MIPI_write_cmos_sensor(0xA607,0x04);//	  //	INC 	 A													
		HM5065MIPI_write_cmos_sensor(0xA608,0xF0);//	  //	MOVX	 @DPTR,A											
		HM5065MIPI_write_cmos_sensor(0xA609,0x05);//	  //	INC 	 c_HeightScale(0x3B);								 
		HM5065MIPI_write_cmos_sensor(0xA60A,0x3B);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA60B,0x02);//	  //	LJMP	 B01:98DA											
		HM5065MIPI_write_cmos_sensor(0xA60C,0xF5);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA60D,0x02);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA60E,0x90);//	  //	MOV 	 DPTR,#AutoFocusInstableFocusMeasureStatus(0x0858);  
		HM5065MIPI_write_cmos_sensor(0xA60F,0x08);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA610,0x58);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA611,0x02);//	  //	LJMP														
		HM5065MIPI_write_cmos_sensor(0xA612,0x9D);//	  //																
		HM5065MIPI_write_cmos_sensor(0xA613,0x50);//	  //																
		   
		
		///////////////////////////////////////////////////////////////////////////////
		//Patch Entries
		///////////////////////////////////////////////////////////////////////////////
		// Breakpoint table entry 0 	Contrast
		HM5065MIPI_write_cmos_sensor(0x9006,0xBA);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x9007,0x75);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9008,0x00);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9009,0x00);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x900A,0x02);//  Enable BP 0//
		
		// Breakpoint table entry 1 	AF p74_04  part 1 
		HM5065MIPI_write_cmos_sensor(0x900D,0x01);//  Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x900E,0xA2);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x900F,0x8F);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9010,0x00);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9011,0xCB);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x9012,0x03);//  Enable BP 1//
		
		// Breakpoint table entry 2 	CSI ULP enter
		HM5065MIPI_write_cmos_sensor(0x9016,0xE6);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x9017,0x6B);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9018,0x02);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9019,0x6B);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x901A,0x02);//  Enable BP 2//
		
		// Breakpoint table entry 3 	 AF p74_03 udwFocusMeasure -> udwPrevFocusMeasureAF 					  
		HM5065MIPI_write_cmos_sensor(0x901D,0x01);//	 // 
		HM5065MIPI_write_cmos_sensor(0x901E,0xAC);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x901F,0x70);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9020,0x00);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9021,0xC5);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x9022,0x03);//  Enable BP 3//
		
		// Breakpoint table entry 4 	AV
		HM5065MIPI_write_cmos_sensor(0x9026,0x9C);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x9027,0x5B);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9028,0x00);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9029,0xBF);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x902A,0x02);//  Enable BP 4//
		
		// Breakpoint table entry 5 	AV OTP
		HM5065MIPI_write_cmos_sensor(0x902E,0x60);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x902F,0x1C);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9030,0x01);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9031,0x37);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x9032,0x02);//  Enable BP 3//
		
		// Breakpoint table entry 6 	AF stat
		HM5065MIPI_write_cmos_sensor(0x9035,0x01);//  Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x9036,0xBA);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x9037,0x70);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9038,0x00);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9039,0x00);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x903A,0x03);//  Enable BP 6//
		
		// Breakpoint table entry 7 	AF Init  part 1
		HM5065MIPI_write_cmos_sensor(0x903E,0x21);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x903F,0x3F);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9040,0x02);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9041,0x40);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x9042,0x02);//  Enable BP 7//
		
		// Breakpoint table entry 8 	AF Init part 2
		HM5065MIPI_write_cmos_sensor(0x9046,0x21);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x9047,0xEA);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9048,0x02);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9049,0x43);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x904A,0x02);//  Enable BP 8//
		
		// Breakpoint table entry 9 	Stop/Start	part 1
		HM5065MIPI_write_cmos_sensor(0x904E,0xA6);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x904F,0x12);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9050,0x02);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9051,0x46);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x9052,0x02);//  Enable BP 9//
		
		// Breakpoint table entry 10	 Stop/Start part 2
		HM5065MIPI_write_cmos_sensor(0x9056,0x29);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x9057,0xE3);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9058,0x02);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9059,0x49);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x905A,0x02);//  Enable BP 10//
		
		// Breakpoint table entry 11	 AF light 
		HM5065MIPI_write_cmos_sensor(0x905D,0x01);//  Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x905E,0x9C);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x905F,0x6E);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9060,0x05);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9061,0x00);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x9062,0x02);//  Enable BP 11//
		
		// Breakpoint table entry 12	 //   AF p74_04  part 2
		HM5065MIPI_write_cmos_sensor(0x9065,0x01);//  Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x9066,0xA2);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x9067,0x66);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9068,0x02);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9069,0x35);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x906A,0x02);//  Enable BP 12//
		
		// Breakpoint table entry 13	 AF p74-05	
		HM5065MIPI_write_cmos_sensor(0x906D,0x01);//  Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x906E,0xB5);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x906F,0xC2);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9070,0x02);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9071,0x9B);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x9072,0x02);//  Enable BP 13//
		
		// Breakpoint table entry 14	 //   AF p74_06  
		HM5065MIPI_write_cmos_sensor(0x9075,0x01);//  Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x9076,0xA2);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x9077,0xD4);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9078,0x02);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9079,0xBE);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x907A,0x02);//  Enable BP 14//
		
		// Breakpoint table entry 15	 bAF_NextState = AF_STATE_COARSE  
		HM5065MIPI_write_cmos_sensor(0x907D,0x01);//  Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x907E,0xB7);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x907F,0xEA);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9080,0x00);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9081,0x02);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x9082,0x03);//  Enable BP 15//
		
		// Breakpoint table entry 16	 //   OTP WB  
		//HM5065MIPI_write_cmos_sensor(0x9085,0x00);//	Patch break point address bank//
		
		HM5065MIPI_write_cmos_sensor(0x9086,0x67);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x9087,0x31);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9088,0x02);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9089,0xF7);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x908A,0x02);//  Enable BP 16//
		
		// Breakpoint table entry 17	  OTP darkcal	
		//HM5065MIPI_write_cmos_sensor(0x908D,0x00);//	Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x908E,0x66);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x908F,0xED);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9090,0x03);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9091,0x1D);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x9092,0x02);//  Enable BP 17//
		
		// Breakpoint table entry 18	 //   OTP HFPN	
		//HM5065MIPI_write_cmos_sensor(0x9095,0x00);//	Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x9096,0x67);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x9097,0x73);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x9098,0x03);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x9099,0xD3);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x909A,0x02);//  Enable BP 18//
		
		// Breakpoint table entry 19	  OTP VCM	
		//HM5065MIPI_write_cmos_sensor(0x909D,0x00);//	Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x909E,0x20);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x909F,0x40);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x90A0,0x03);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x90A1,0x3B);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x90A2,0x02);//  Enable BP 19//
		
		// Breakpoint table entry 20	 //   TWakeup work around for PLL3 div	 
		//HM5065MIPI_write_cmos_sensor(0x90A5,0x00);//	Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x90A6,0xC5);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x90A7,0xC0);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x90A8,0x03);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x90A9,0xF0);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x90AA,0x02);//  Enable BP 20//
		
		// Breakpoint table entry 21   // reset the pipe at stop stage
		//HM5065MIPI_write_cmos_sensor(0x90AD,0x00);//		//Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x90AE,0x41);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x90AF,0xB3);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x90B0,0x00);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x90B1,0xA2);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x90B2,0x02);//  Enable BP 21//
		
		// Breakpoint table entry 22	 //   CSI Tinit   
		//HM5065MIPI_write_cmos_sensor(0x90B5,0x00);//	Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x90B6,0x44);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x90B7,0xBA);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x90B8,0x00);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x90B9,0xF0);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x90BA,0x03);//  Enable BP 22//
		
		// Breakpoint table entry 23	  AV2x2    
		//HM5065MIPI_write_cmos_sensor(0x90BD,0x00);//		//Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x90BE,0x89);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x90BF,0x99);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x90C0,0x04);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x90C1,0x00);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x90C2,0x02);//  Enable BP 23//
		
		// Breakpoint table entry 24	 //   DL JPEG CSI clk /2   
		//HM5065MIPI_write_cmos_sensor(0x90C5,0x00);//	Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x90C6,0xA7);//  Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x90C7,0x91);//  Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x90C8,0x04);//  Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x90C9,0x21);//  Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x90CA,0x02);//  Enable BP 24//
		
		// Breakpoint table entry 25	  pipe reset at pause stage    
		//HM5065MIPI_write_cmos_sensor(0x90CD,0x00);//	  // Patch break point address bank//
		HM5065MIPI_write_cmos_sensor(0x90CE,0x3A);//	// Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x90CF,0x51);//	// Patch break point address low byte//
		HM5065MIPI_write_cmos_sensor(0x90D0,0x00);//	// Offset High byte//
		HM5065MIPI_write_cmos_sensor(0x90D1,0xA2);//	// Offset Low byte//
		HM5065MIPI_write_cmos_sensor(0x90D2,0x02);//	// Enable BP 25//
		
		// Breakpoint table entry 26	 Freeze WB at low stat
		//HM5065MIPI_write_cmos_sensor(0x90D5,0x00);//	   // Patch break point address bank//	  
		HM5065MIPI_write_cmos_sensor(0x90D6,0x86);//	 // Patch break point address high byte//
		HM5065MIPI_write_cmos_sensor(0x90D7,0x54);//	 // Patch break point address low byte// 
		HM5065MIPI_write_cmos_sensor(0x90D8,0x04);//	 // Offset High byte//					 
		HM5065MIPI_write_cmos_sensor(0x90D9,0x47);//	 // Offset Low byte//					 
		HM5065MIPI_write_cmos_sensor(0x90DA,0x02);//	 // Enable BP 26//						 
		
		
		HM5065MIPI_write_cmos_sensor(0x9000,0x01);//  Enable patch//
		HM5065MIPI_write_cmos_sensor(0xffff,0x00);//  MCU release//
		
		mDELAY(200);

		HM5065MIPI_write_cmos_sensor(0x0009,0x17); 	//	MCLK=26Mhz ,
		HM5065MIPI_write_cmos_sensor(0x0085,0x02); 	//	Reg0x0085=0x00(CbYCrY - sequence)for MTK 0:uyvy 2: yuyv
		HM5065MIPI_write_cmos_sensor(0x0040,0x00); 	//	binning mode and subsampling mode for frame rate
#ifdef BIRD_HM6065_MIPI_MODE			
		HM5065MIPI_write_cmos_sensor(0x0016,0x02); 	//	Parallel(00) mode or mipi/1 lane(01) /2 lanes(02)
#else
		HM5065MIPI_write_cmos_sensor(0x0016,0x00);	//	Parallel(00) mode or mipi/1 lane(01) /2 lanes(02)
#endif		
		HM5065MIPI_write_cmos_sensor(0x0046,0x00); 	//	data format (RGB/YUV/JPEG)
		HM5065MIPI_write_cmos_sensor(0x0041,0x00); 	//	04 : VGA mode : 0A : self define ; 00 : 5M
		HM5065MIPI_write_cmos_sensor(0x0042,0x0A); 	//	X:1298 --> 800 ->2592
		HM5065MIPI_write_cmos_sensor(0x0043,0x20); 	//
		HM5065MIPI_write_cmos_sensor(0x0044,0x07); 	//	Y:972 -->	600->1944
		HM5065MIPI_write_cmos_sensor(0x0045,0x98); 	//
		HM5065MIPI_write_cmos_sensor(0x00B5,0x01); 	//PLL Divider3 //02kai
		
		HM5065MIPI_write_cmos_sensor(0x0030,0x11);
		
		HM5065MIPI_write_cmos_sensor(0x7101,0xC4);
		HM5065MIPI_write_cmos_sensor(0x00E8,0x01);//AFR
		HM5065MIPI_write_cmos_sensor(0x00ED,0x0c);//Min.Frame Rate
		HM5065MIPI_write_cmos_sensor(0x00EE,0x1E);//Max.Framte Rate=30fps
		HM5065MIPI_write_cmos_sensor(0x00B2,0x50);//4e   //50//set PLL output 560MHz
		HM5065MIPI_write_cmos_sensor(0x00B3,0xC9);//CA
		HM5065MIPI_write_cmos_sensor(0x7104,0x02);
		HM5065MIPI_write_cmos_sensor(0x7105,0x00);
		HM5065MIPI_write_cmos_sensor(0x019C,0x4B);
		HM5065MIPI_write_cmos_sensor(0x019D,0xC0);
		HM5065MIPI_write_cmos_sensor(0x0129,0x00);//00:Flat  meter 02:center meter
		HM5065MIPI_write_cmos_sensor(0x0130,0x00);//00(0EV)->FF(-0.2EV)
		HM5065MIPI_write_cmos_sensor(0x0083,0x00);//0x01
		HM5065MIPI_write_cmos_sensor(0x0084,0x00);//0x01
		HM5065MIPI_write_cmos_sensor(0x01A1,0x80);
		HM5065MIPI_write_cmos_sensor(0x01A2,0x80);
		HM5065MIPI_write_cmos_sensor(0x01A3,0x80);
		HM5065MIPI_write_cmos_sensor(0x01A0,0x01);
		HM5065MIPI_write_cmos_sensor(0x0021,0x00);
		HM5065MIPI_write_cmos_sensor(0x0022,0x01);
		HM5065MIPI_write_cmos_sensor(0x0060,0x00);
		HM5065MIPI_write_cmos_sensor(0x0013,0x00);
		HM5065MIPI_write_cmos_sensor(0x0061,0x00);
		HM5065MIPI_write_cmos_sensor(0x0066,0x02);
		HM5065MIPI_write_cmos_sensor(0x0012,0x00);
		HM5065MIPI_write_cmos_sensor(0x7102,0x09);  //default 01 ; 09 : for clear Hsync gating
		HM5065MIPI_write_cmos_sensor(0x7103,0x00);
		HM5065MIPI_write_cmos_sensor(0x7158,0x00);
		HM5065MIPI_write_cmos_sensor(0x7000,0x2C);
		HM5065MIPI_write_cmos_sensor(0x5200,0x01);
		HM5065MIPI_write_cmos_sensor(0x7000,0x0C);
		HM5065MIPI_write_cmos_sensor(0x02C2,0x00);//Max Again 00E0=8x,0080=2x,  00C0=4x
		HM5065MIPI_write_cmos_sensor(0x02C3,0xA0);//C0 By Zeroy
		HM5065MIPI_write_cmos_sensor(0x015E,0x40);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
		HM5065MIPI_write_cmos_sensor(0x015F,0x00);
		HM5065MIPI_write_cmos_sensor(0x0390,0x01);
		HM5065MIPI_write_cmos_sensor(0x0391,0x00);
		HM5065MIPI_write_cmos_sensor(0x0392,0x00);
		HM5065MIPI_write_cmos_sensor(0x03A0,0x14);
		HM5065MIPI_write_cmos_sensor(0x03A1,0x00);
		HM5065MIPI_write_cmos_sensor(0x03A2,0x5A);
		HM5065MIPI_write_cmos_sensor(0x03A3,0xEE);
		HM5065MIPI_write_cmos_sensor(0x03A4,0x69);
		HM5065MIPI_write_cmos_sensor(0x03A5,0x49);
		HM5065MIPI_write_cmos_sensor(0x03A6,0x3E);
		HM5065MIPI_write_cmos_sensor(0x03A7,0x00);
		HM5065MIPI_write_cmos_sensor(0x03A8,0x39);
		HM5065MIPI_write_cmos_sensor(0x03A9,0x33);
		HM5065MIPI_write_cmos_sensor(0x03B0,0x60);
		HM5065MIPI_write_cmos_sensor(0x03B1,0x00);
		HM5065MIPI_write_cmos_sensor(0x03B2,0x5A);
		HM5065MIPI_write_cmos_sensor(0x03B3,0xEE);
		HM5065MIPI_write_cmos_sensor(0x03B4,0x69);
		HM5065MIPI_write_cmos_sensor(0x03B5,0x49);
		HM5065MIPI_write_cmos_sensor(0x03B6,0x3E);
		HM5065MIPI_write_cmos_sensor(0x03B7,0x00);
		HM5065MIPI_write_cmos_sensor(0x03B8,0x3D);
		HM5065MIPI_write_cmos_sensor(0x03B9,0x20);
		HM5065MIPI_write_cmos_sensor(0x03C0,0x10);
		HM5065MIPI_write_cmos_sensor(0x03C1,0x00);
		HM5065MIPI_write_cmos_sensor(0x03C2,0x5A);
		HM5065MIPI_write_cmos_sensor(0x03C3,0xEE);
		HM5065MIPI_write_cmos_sensor(0x03C4,0x69);
		HM5065MIPI_write_cmos_sensor(0x03C5,0x49);
		HM5065MIPI_write_cmos_sensor(0x03C6,0x3A);
		HM5065MIPI_write_cmos_sensor(0x03C7,0x80);
		HM5065MIPI_write_cmos_sensor(0x03D0,0x64);
		HM5065MIPI_write_cmos_sensor(0x03D1,0x00);
		HM5065MIPI_write_cmos_sensor(0x03D2,0x5A);
		HM5065MIPI_write_cmos_sensor(0x03D3,0xEE);
		HM5065MIPI_write_cmos_sensor(0x03D4,0x69);
		HM5065MIPI_write_cmos_sensor(0x03D5,0x49);
		HM5065MIPI_write_cmos_sensor(0x03D6,0x34);
		HM5065MIPI_write_cmos_sensor(0x03D7,0xD1);
		HM5065MIPI_write_cmos_sensor(0x004C,0x08);
		HM5065MIPI_write_cmos_sensor(0x006C,0x08);
		HM5065MIPI_write_cmos_sensor(0x0350,0x00);
		HM5065MIPI_write_cmos_sensor(0x0351,0x5A);
		HM5065MIPI_write_cmos_sensor(0x0352,0xEE);
		HM5065MIPI_write_cmos_sensor(0x0353,0x69);
		HM5065MIPI_write_cmos_sensor(0x0354,0x49);
		HM5065MIPI_write_cmos_sensor(0x0355,0x39);
		HM5065MIPI_write_cmos_sensor(0x0356,0x6D);
		HM5065MIPI_write_cmos_sensor(0x0357,0x19);
		HM5065MIPI_write_cmos_sensor(0x0358,0x00);
		HM5065MIPI_write_cmos_sensor(0x0359,0x3C);
		HM5065MIPI_write_cmos_sensor(0x035A,0x5A);
		HM5065MIPI_write_cmos_sensor(0x035B,0xEE);
		HM5065MIPI_write_cmos_sensor(0x035C,0x69);
		HM5065MIPI_write_cmos_sensor(0x035D,0x49);
		HM5065MIPI_write_cmos_sensor(0x035E,0x39);
		HM5065MIPI_write_cmos_sensor(0x035F,0x85);
		HM5065MIPI_write_cmos_sensor(0x0049,0x14);
		HM5065MIPI_write_cmos_sensor(0x004A,0x0D);
		HM5065MIPI_write_cmos_sensor(0x0069,0x14);
		HM5065MIPI_write_cmos_sensor(0x006A,0x0D);
		HM5065MIPI_write_cmos_sensor(0x0090,0x5A);
		HM5065MIPI_write_cmos_sensor(0x0091,0xEE);
		HM5065MIPI_write_cmos_sensor(0x0092,0x3E);
		HM5065MIPI_write_cmos_sensor(0x0093,0x00);
		HM5065MIPI_write_cmos_sensor(0x0094,0x69);
		HM5065MIPI_write_cmos_sensor(0x0095,0x49);
		HM5065MIPI_write_cmos_sensor(0x0096,0x39);
		HM5065MIPI_write_cmos_sensor(0x0097,0xCF);
		HM5065MIPI_write_cmos_sensor(0x0098,0x00);	//CF BY Zeroy
		HM5065MIPI_write_cmos_sensor(0x00A0,0x5A);
		HM5065MIPI_write_cmos_sensor(0x00A1,0xEE);
		HM5065MIPI_write_cmos_sensor(0x00A2,0x3E);
		HM5065MIPI_write_cmos_sensor(0x00A3,0x00);
		HM5065MIPI_write_cmos_sensor(0x00A4,0x69);
		HM5065MIPI_write_cmos_sensor(0x00A5,0x49);
		HM5065MIPI_write_cmos_sensor(0x00A6,0x3B);
		HM5065MIPI_write_cmos_sensor(0x00A7,0x80);
		HM5065MIPI_write_cmos_sensor(0x00A8,0x00);		
		HM5065MIPI_write_cmos_sensor(0x0420,0x00);
		HM5065MIPI_write_cmos_sensor(0x0421,0x26);
		HM5065MIPI_write_cmos_sensor(0x0422,0xff);
		HM5065MIPI_write_cmos_sensor(0x0423,0x03);
		HM5065MIPI_write_cmos_sensor(0x0424,0x00);
		HM5065MIPI_write_cmos_sensor(0x0425,0x63);
		HM5065MIPI_write_cmos_sensor(0x0426,0x00);
		HM5065MIPI_write_cmos_sensor(0x0427,0x8e);
		HM5065MIPI_write_cmos_sensor(0x0428,0xff);
		HM5065MIPI_write_cmos_sensor(0x0429,0xf3);
		HM5065MIPI_write_cmos_sensor(0x042A,0x00);
		HM5065MIPI_write_cmos_sensor(0x042B,0x26);
		HM5065MIPI_write_cmos_sensor(0x042C,0xff);
		HM5065MIPI_write_cmos_sensor(0x042D,0xc8);
		HM5065MIPI_write_cmos_sensor(0x042E,0xff);
		HM5065MIPI_write_cmos_sensor(0x042F,0x5f);
		HM5065MIPI_write_cmos_sensor(0x0430,0x00);
		HM5065MIPI_write_cmos_sensor(0x0431,0x10);
		HM5065MIPI_write_cmos_sensor(0x0432,0xfe);
		HM5065MIPI_write_cmos_sensor(0x0433,0xcf);
		HM5065MIPI_write_cmos_sensor(0x0434,0x00);
		HM5065MIPI_write_cmos_sensor(0x0435,0xc2);
		HM5065MIPI_write_cmos_sensor(0x0436,0x00);
		HM5065MIPI_write_cmos_sensor(0x0437,0xf2);
		HM5065MIPI_write_cmos_sensor(0x0438,0xff);
		HM5065MIPI_write_cmos_sensor(0x0439,0xd0);
		HM5065MIPI_write_cmos_sensor(0x043A,0x00);
		HM5065MIPI_write_cmos_sensor(0x043B,0xa1);
		HM5065MIPI_write_cmos_sensor(0x043C,0x00);
		HM5065MIPI_write_cmos_sensor(0x043D,0x2f);
		HM5065MIPI_write_cmos_sensor(0x043E,0xfe);
		HM5065MIPI_write_cmos_sensor(0x043F,0xdc);
		HM5065MIPI_write_cmos_sensor(0x0450,0x00);
		HM5065MIPI_write_cmos_sensor(0x0451,0x36);
		HM5065MIPI_write_cmos_sensor(0x0452,0xff);
		HM5065MIPI_write_cmos_sensor(0x0453,0x60);
		HM5065MIPI_write_cmos_sensor(0x0454,0x00);
		HM5065MIPI_write_cmos_sensor(0x0455,0x43);
		HM5065MIPI_write_cmos_sensor(0x0456,0x00);
		HM5065MIPI_write_cmos_sensor(0x0457,0x6b);
		HM5065MIPI_write_cmos_sensor(0x0458,0xff);
		HM5065MIPI_write_cmos_sensor(0x0459,0xdf);
		HM5065MIPI_write_cmos_sensor(0x045A,0x00);
		HM5065MIPI_write_cmos_sensor(0x045B,0x83);
		HM5065MIPI_write_cmos_sensor(0x045C,0xff);
		HM5065MIPI_write_cmos_sensor(0x045D,0xeb);
		HM5065MIPI_write_cmos_sensor(0x045E,0xff);
		HM5065MIPI_write_cmos_sensor(0x045F,0x5f);
		HM5065MIPI_write_cmos_sensor(0x0440,0x00);
		HM5065MIPI_write_cmos_sensor(0x0441,0x36);
		HM5065MIPI_write_cmos_sensor(0x0442,0xff);
		HM5065MIPI_write_cmos_sensor(0x0443,0x3e);
		HM5065MIPI_write_cmos_sensor(0x0444,0x00);
		HM5065MIPI_write_cmos_sensor(0x0445,0x6d);
		HM5065MIPI_write_cmos_sensor(0x0446,0x00);
		HM5065MIPI_write_cmos_sensor(0x0447,0x87);
		HM5065MIPI_write_cmos_sensor(0x0448,0x00);
		HM5065MIPI_write_cmos_sensor(0x0449,0x0f);
		HM5065MIPI_write_cmos_sensor(0x044A,0xff);
		HM5065MIPI_write_cmos_sensor(0x044B,0xd3);
		HM5065MIPI_write_cmos_sensor(0x044C,0x00);
		HM5065MIPI_write_cmos_sensor(0x044D,0x18);
		HM5065MIPI_write_cmos_sensor(0x044E,0xff);
		HM5065MIPI_write_cmos_sensor(0x044F,0x4f);
		HM5065MIPI_write_cmos_sensor(0x0561,0x08);
		HM5065MIPI_write_cmos_sensor(0x0460,0x00);
		HM5065MIPI_write_cmos_sensor(0x0461,0x22);
		HM5065MIPI_write_cmos_sensor(0x0462,0xff);
		HM5065MIPI_write_cmos_sensor(0x0463,0x0a);
		HM5065MIPI_write_cmos_sensor(0x0464,0x00);
		HM5065MIPI_write_cmos_sensor(0x0465,0x63);
		HM5065MIPI_write_cmos_sensor(0x0466,0x00);
		HM5065MIPI_write_cmos_sensor(0x0467,0x8b);
		HM5065MIPI_write_cmos_sensor(0x0468,0xff);
		HM5065MIPI_write_cmos_sensor(0x0469,0xf9);
		HM5065MIPI_write_cmos_sensor(0x046A,0x00);
		HM5065MIPI_write_cmos_sensor(0x046B,0x17);
		HM5065MIPI_write_cmos_sensor(0x046C,0xff);
		HM5065MIPI_write_cmos_sensor(0x046D,0xbd);
		HM5065MIPI_write_cmos_sensor(0x046E,0xff);
		HM5065MIPI_write_cmos_sensor(0x046F,0x65);
		HM5065MIPI_write_cmos_sensor(0x0470,0x00);
		HM5065MIPI_write_cmos_sensor(0x0471,0x15);
		HM5065MIPI_write_cmos_sensor(0x0472,0xfe);
		HM5065MIPI_write_cmos_sensor(0x0473,0xd7);
		HM5065MIPI_write_cmos_sensor(0x0474,0x00);
		HM5065MIPI_write_cmos_sensor(0x0475,0xb6);
		HM5065MIPI_write_cmos_sensor(0x0476,0x00);
		HM5065MIPI_write_cmos_sensor(0x0477,0xe5);
		HM5065MIPI_write_cmos_sensor(0x0478,0xff);
		HM5065MIPI_write_cmos_sensor(0x0479,0xd0);
		HM5065MIPI_write_cmos_sensor(0x047A,0x00);
		HM5065MIPI_write_cmos_sensor(0x047B,0xb2);
		HM5065MIPI_write_cmos_sensor(0x047C,0x00);
		HM5065MIPI_write_cmos_sensor(0x047D,0x2c);
		HM5065MIPI_write_cmos_sensor(0x047E,0xfe);
		HM5065MIPI_write_cmos_sensor(0x047F,0xe6);
		HM5065MIPI_write_cmos_sensor(0x0490,0x00);
		HM5065MIPI_write_cmos_sensor(0x0491,0x2b);
		HM5065MIPI_write_cmos_sensor(0x0492,0xff);
		HM5065MIPI_write_cmos_sensor(0x0493,0x71);
		HM5065MIPI_write_cmos_sensor(0x0494,0x00);
		HM5065MIPI_write_cmos_sensor(0x0495,0x43);
		HM5065MIPI_write_cmos_sensor(0x0496,0x00);
		HM5065MIPI_write_cmos_sensor(0x0497,0x69);
		HM5065MIPI_write_cmos_sensor(0x0498,0xff);
		HM5065MIPI_write_cmos_sensor(0x0499,0xd7);
		HM5065MIPI_write_cmos_sensor(0x049A,0x00);
		HM5065MIPI_write_cmos_sensor(0x049B,0x59);
		HM5065MIPI_write_cmos_sensor(0x049C,0xff);
		HM5065MIPI_write_cmos_sensor(0x049D,0xde);
		HM5065MIPI_write_cmos_sensor(0x049E,0xff);
		HM5065MIPI_write_cmos_sensor(0x049F,0x76);
		HM5065MIPI_write_cmos_sensor(0x0480,0x00);
		HM5065MIPI_write_cmos_sensor(0x0481,0x38);
		HM5065MIPI_write_cmos_sensor(0x0482,0xff);
		HM5065MIPI_write_cmos_sensor(0x0483,0x3a);
		HM5065MIPI_write_cmos_sensor(0x0484,0x00);
		HM5065MIPI_write_cmos_sensor(0x0485,0x6b);
		HM5065MIPI_write_cmos_sensor(0x0486,0x00);
		HM5065MIPI_write_cmos_sensor(0x0487,0x86);
		HM5065MIPI_write_cmos_sensor(0x0488,0x00);
		HM5065MIPI_write_cmos_sensor(0x0489,0x14);
		HM5065MIPI_write_cmos_sensor(0x048A,0xff);
		HM5065MIPI_write_cmos_sensor(0x048B,0xd7);
		HM5065MIPI_write_cmos_sensor(0x048C,0x00);
		HM5065MIPI_write_cmos_sensor(0x048D,0x13);
		HM5065MIPI_write_cmos_sensor(0x048E,0xff);
		HM5065MIPI_write_cmos_sensor(0x048F,0x58);
		HM5065MIPI_write_cmos_sensor(0x0562,0x08);
		HM5065MIPI_write_cmos_sensor(0x04A0,0x00);
		HM5065MIPI_write_cmos_sensor(0x04A1,0x17);
		HM5065MIPI_write_cmos_sensor(0x04A2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04A3,0x2e);
		HM5065MIPI_write_cmos_sensor(0x04A4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04A5,0x5d);
		HM5065MIPI_write_cmos_sensor(0x04A6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04A7,0x7a);
		HM5065MIPI_write_cmos_sensor(0x04A8,0xff);
		HM5065MIPI_write_cmos_sensor(0x04A9,0xf9);
		HM5065MIPI_write_cmos_sensor(0x04AA,0xff);
		HM5065MIPI_write_cmos_sensor(0x04AB,0xc0);
		HM5065MIPI_write_cmos_sensor(0x04AC,0xff);
		HM5065MIPI_write_cmos_sensor(0x04AD,0xc7);
		HM5065MIPI_write_cmos_sensor(0x04AE,0xff);
		HM5065MIPI_write_cmos_sensor(0x04AF,0x7d);
		HM5065MIPI_write_cmos_sensor(0x04B0,0x00);
		HM5065MIPI_write_cmos_sensor(0x04B1,0x13);
		HM5065MIPI_write_cmos_sensor(0x04B2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04B3,0x45);
		HM5065MIPI_write_cmos_sensor(0x04B4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04B5,0x70);
		HM5065MIPI_write_cmos_sensor(0x04B6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04B7,0x96);
		HM5065MIPI_write_cmos_sensor(0x04B8,0xff);
		HM5065MIPI_write_cmos_sensor(0x04B9,0xde);
		HM5065MIPI_write_cmos_sensor(0x04BA,0x00);
		HM5065MIPI_write_cmos_sensor(0x04BB,0x17);
		HM5065MIPI_write_cmos_sensor(0x04BC,0x00);
		HM5065MIPI_write_cmos_sensor(0x04BD,0x23);
		HM5065MIPI_write_cmos_sensor(0x04BE,0xff);
		HM5065MIPI_write_cmos_sensor(0x04BF,0x46);
		HM5065MIPI_write_cmos_sensor(0x04D0,0x00);
		HM5065MIPI_write_cmos_sensor(0x04D1,0x17);
		HM5065MIPI_write_cmos_sensor(0x04D2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04D3,0x99);
		HM5065MIPI_write_cmos_sensor(0x04D4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04D5,0x3d);
		HM5065MIPI_write_cmos_sensor(0x04D6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04D7,0x5f);
		HM5065MIPI_write_cmos_sensor(0x04D8,0xff);
		HM5065MIPI_write_cmos_sensor(0x04D9,0xd1);
		HM5065MIPI_write_cmos_sensor(0x04DA,0xff);
		HM5065MIPI_write_cmos_sensor(0x04DB,0xe2);
		HM5065MIPI_write_cmos_sensor(0x04DC,0xff);
		HM5065MIPI_write_cmos_sensor(0x04DD,0xc6);
		HM5065MIPI_write_cmos_sensor(0x04DE,0xff);
		HM5065MIPI_write_cmos_sensor(0x04DF,0x9c);
		HM5065MIPI_write_cmos_sensor(0x04C0,0x00);
		HM5065MIPI_write_cmos_sensor(0x04C1,0x32);
		HM5065MIPI_write_cmos_sensor(0x04C2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04C3,0x4e);
		HM5065MIPI_write_cmos_sensor(0x04C4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04C5,0x5e);
		HM5065MIPI_write_cmos_sensor(0x04C6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04C7,0x7c);
		HM5065MIPI_write_cmos_sensor(0x04C8,0x00);
		HM5065MIPI_write_cmos_sensor(0x04C9,0x17);
		HM5065MIPI_write_cmos_sensor(0x04CA,0xff);
		HM5065MIPI_write_cmos_sensor(0x04CB,0x9b);
		HM5065MIPI_write_cmos_sensor(0x04CC,0x00);
		HM5065MIPI_write_cmos_sensor(0x04CD,0x06);
		HM5065MIPI_write_cmos_sensor(0x04CE,0xff);
		HM5065MIPI_write_cmos_sensor(0x04CF,0x70);
		HM5065MIPI_write_cmos_sensor(0x0563,0x05);
		HM5065MIPI_write_cmos_sensor(0x04E0,0x00);
		HM5065MIPI_write_cmos_sensor(0x04E1,0x11);
		HM5065MIPI_write_cmos_sensor(0x04E2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04E3,0x20);
		HM5065MIPI_write_cmos_sensor(0x04E4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04E5,0x5f);
		HM5065MIPI_write_cmos_sensor(0x04E6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04E7,0x7b);
		HM5065MIPI_write_cmos_sensor(0x04E8,0x00);
		HM5065MIPI_write_cmos_sensor(0x04E9,0x02);
		HM5065MIPI_write_cmos_sensor(0x04EA,0xff);
		HM5065MIPI_write_cmos_sensor(0x04EB,0xf1);
		HM5065MIPI_write_cmos_sensor(0x04EC,0xff);
		HM5065MIPI_write_cmos_sensor(0x04ED,0xb7);
		HM5065MIPI_write_cmos_sensor(0x04EE,0xff);
		HM5065MIPI_write_cmos_sensor(0x04EF,0x7b);
		HM5065MIPI_write_cmos_sensor(0x04F0,0x00);
		HM5065MIPI_write_cmos_sensor(0x04F1,0x1c);
		HM5065MIPI_write_cmos_sensor(0x04F2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04F3,0x08);
		HM5065MIPI_write_cmos_sensor(0x04F4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04F5,0x7e);
		HM5065MIPI_write_cmos_sensor(0x04F6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04F7,0xac);
		HM5065MIPI_write_cmos_sensor(0x04F8,0xff);
		HM5065MIPI_write_cmos_sensor(0x04F9,0xeb);
		HM5065MIPI_write_cmos_sensor(0x04FA,0x00);
		HM5065MIPI_write_cmos_sensor(0x04FB,0x93);
		HM5065MIPI_write_cmos_sensor(0x04FC,0x00);
		HM5065MIPI_write_cmos_sensor(0x04FD,0x1b);
		HM5065MIPI_write_cmos_sensor(0x04FE,0xff);
		HM5065MIPI_write_cmos_sensor(0x04FF,0x11);
		HM5065MIPI_write_cmos_sensor(0x0510,0xff);
		HM5065MIPI_write_cmos_sensor(0x0511,0xfe);
		HM5065MIPI_write_cmos_sensor(0x0512,0xff);
		HM5065MIPI_write_cmos_sensor(0x0513,0x8e);
		HM5065MIPI_write_cmos_sensor(0x0514,0x00);
		HM5065MIPI_write_cmos_sensor(0x0515,0x4a);
		HM5065MIPI_write_cmos_sensor(0x0516,0x00);
		HM5065MIPI_write_cmos_sensor(0x0517,0x6e);
		HM5065MIPI_write_cmos_sensor(0x0518,0xff);
		HM5065MIPI_write_cmos_sensor(0x0519,0xbb);
		HM5065MIPI_write_cmos_sensor(0x051A,0xff);
		HM5065MIPI_write_cmos_sensor(0x051B,0xcc);
		HM5065MIPI_write_cmos_sensor(0x051C,0xff);
		HM5065MIPI_write_cmos_sensor(0x051D,0x9a);
		HM5065MIPI_write_cmos_sensor(0x051E,0xff);
		HM5065MIPI_write_cmos_sensor(0x051F,0xa2);
		HM5065MIPI_write_cmos_sensor(0x0500,0x00);
		HM5065MIPI_write_cmos_sensor(0x0501,0x30);
		HM5065MIPI_write_cmos_sensor(0x0502,0xff);
		HM5065MIPI_write_cmos_sensor(0x0503,0x36);
		HM5065MIPI_write_cmos_sensor(0x0504,0x00);
		HM5065MIPI_write_cmos_sensor(0x0505,0x5d);
		HM5065MIPI_write_cmos_sensor(0x0506,0x00);
		HM5065MIPI_write_cmos_sensor(0x0507,0x81);
		HM5065MIPI_write_cmos_sensor(0x0508,0x00);
		HM5065MIPI_write_cmos_sensor(0x0509,0x1f);
		HM5065MIPI_write_cmos_sensor(0x050A,0xff);
		HM5065MIPI_write_cmos_sensor(0x050B,0xd4);
		HM5065MIPI_write_cmos_sensor(0x050C,0xff);
		HM5065MIPI_write_cmos_sensor(0x050D,0xfe);
		HM5065MIPI_write_cmos_sensor(0x050E,0xff);
		HM5065MIPI_write_cmos_sensor(0x050F,0x75);
		HM5065MIPI_write_cmos_sensor(0x0564,0x00);
		HM5065MIPI_write_cmos_sensor(0x0324,0x39);
		HM5065MIPI_write_cmos_sensor(0x0325,0xAE);
		HM5065MIPI_write_cmos_sensor(0x0326,0x3A);
		HM5065MIPI_write_cmos_sensor(0x0327,0x29);
		HM5065MIPI_write_cmos_sensor(0x0328,0x3B);
		HM5065MIPI_write_cmos_sensor(0x0329,0x0A);
		HM5065MIPI_write_cmos_sensor(0x032A,0x3B);
		HM5065MIPI_write_cmos_sensor(0x032B,0xAE);
		HM5065MIPI_write_cmos_sensor(0x0320,0x01);
		HM5065MIPI_write_cmos_sensor(0x0321,0x04);
		HM5065MIPI_write_cmos_sensor(0x0322,0x01);
		HM5065MIPI_write_cmos_sensor(0x0323,0x01);
		HM5065MIPI_write_cmos_sensor(0x0330,0xCF);
		HM5065MIPI_write_cmos_sensor(0x0384,0x00);
		HM5065MIPI_write_cmos_sensor(0x0337,0x01);
		HM5065MIPI_write_cmos_sensor(0x03EC,0x39);
		HM5065MIPI_write_cmos_sensor(0x03ED,0x85);
		HM5065MIPI_write_cmos_sensor(0x03FC,0x3A);
		HM5065MIPI_write_cmos_sensor(0x03FD,0x14);
		HM5065MIPI_write_cmos_sensor(0x040C,0x3A);
		HM5065MIPI_write_cmos_sensor(0x040D,0xF6);
		HM5065MIPI_write_cmos_sensor(0x041C,0x3B);
		HM5065MIPI_write_cmos_sensor(0x041D,0x9A);
		HM5065MIPI_write_cmos_sensor(0x03E0,0xB6);
		HM5065MIPI_write_cmos_sensor(0x03E1,0x04);
		HM5065MIPI_write_cmos_sensor(0x03E2,0xBB);
		HM5065MIPI_write_cmos_sensor(0x03E3,0xE9);
		HM5065MIPI_write_cmos_sensor(0x03E4,0xBC);
		HM5065MIPI_write_cmos_sensor(0x03E5,0x70);
		HM5065MIPI_write_cmos_sensor(0x03E6,0x37);
		HM5065MIPI_write_cmos_sensor(0x03E7,0x02);
		HM5065MIPI_write_cmos_sensor(0x03E8,0xBC);
		HM5065MIPI_write_cmos_sensor(0x03E9,0x00);
		HM5065MIPI_write_cmos_sensor(0x03EA,0xBF);
		HM5065MIPI_write_cmos_sensor(0x03EB,0x12);
		HM5065MIPI_write_cmos_sensor(0x03F0,0xBA);
		HM5065MIPI_write_cmos_sensor(0x03F1,0x7B);
		HM5065MIPI_write_cmos_sensor(0x03F2,0xBA);
		HM5065MIPI_write_cmos_sensor(0x03F3,0x83);
		HM5065MIPI_write_cmos_sensor(0x03F4,0xBB);
		HM5065MIPI_write_cmos_sensor(0x03F5,0xBC);
		HM5065MIPI_write_cmos_sensor(0x03F6,0x38);
		HM5065MIPI_write_cmos_sensor(0x03F7,0x2D);
		HM5065MIPI_write_cmos_sensor(0x03F8,0xBB);
		HM5065MIPI_write_cmos_sensor(0x03F9,0x23);
		HM5065MIPI_write_cmos_sensor(0x03FA,0xBD);
		HM5065MIPI_write_cmos_sensor(0x03FB,0xAC);
		HM5065MIPI_write_cmos_sensor(0x0400,0xBE);
		HM5065MIPI_write_cmos_sensor(0x0401,0x96);
		HM5065MIPI_write_cmos_sensor(0x0402,0xB9);
		HM5065MIPI_write_cmos_sensor(0x0403,0xBE);
		HM5065MIPI_write_cmos_sensor(0x0404,0xBB);
		HM5065MIPI_write_cmos_sensor(0x0405,0x57);
		HM5065MIPI_write_cmos_sensor(0x0406,0x3A);
		HM5065MIPI_write_cmos_sensor(0x0407,0xBB);
		HM5065MIPI_write_cmos_sensor(0x0408,0xB3);
		HM5065MIPI_write_cmos_sensor(0x0409,0x17);
		HM5065MIPI_write_cmos_sensor(0x040A,0xBE);
		HM5065MIPI_write_cmos_sensor(0x040B,0x66);
		HM5065MIPI_write_cmos_sensor(0x0410,0xBB);
		HM5065MIPI_write_cmos_sensor(0x0411,0x2A);
		HM5065MIPI_write_cmos_sensor(0x0412,0xBA);
		HM5065MIPI_write_cmos_sensor(0x0413,0x00);
		HM5065MIPI_write_cmos_sensor(0x0414,0xBB);
		HM5065MIPI_write_cmos_sensor(0x0415,0x10);
		HM5065MIPI_write_cmos_sensor(0x0416,0xB8);
		HM5065MIPI_write_cmos_sensor(0x0417,0xCD);
		HM5065MIPI_write_cmos_sensor(0x0418,0xB7);
		HM5065MIPI_write_cmos_sensor(0x0419,0x5C);
		HM5065MIPI_write_cmos_sensor(0x041A,0xBB);
		HM5065MIPI_write_cmos_sensor(0x041B,0x6C);
		HM5065MIPI_write_cmos_sensor(0x01f8,0x3c);
		HM5065MIPI_write_cmos_sensor(0x01f9,0x00);
		HM5065MIPI_write_cmos_sensor(0x01fa,0x00);
		HM5065MIPI_write_cmos_sensor(0x02a2,0x3e);
		HM5065MIPI_write_cmos_sensor(0x02a3,0x00);
		HM5065MIPI_write_cmos_sensor(0x02a4,0x3e);
		HM5065MIPI_write_cmos_sensor(0x02a5,0x00);
		HM5065MIPI_write_cmos_sensor(0x02a6,0x3e);
		HM5065MIPI_write_cmos_sensor(0x02a7,0x00);
		HM5065MIPI_write_cmos_sensor(0x02a8,0x3f);
		HM5065MIPI_write_cmos_sensor(0x02a9,0x66);
		HM5065MIPI_write_cmos_sensor(0x056c,0x42);
		HM5065MIPI_write_cmos_sensor(0x056d,0x00);
		HM5065MIPI_write_cmos_sensor(0x056e,0x42);
		HM5065MIPI_write_cmos_sensor(0x056f,0x00);
		HM5065MIPI_write_cmos_sensor(0x0570,0x42);
		HM5065MIPI_write_cmos_sensor(0x0571,0x00);
		HM5065MIPI_write_cmos_sensor(0x0572,0x42);
		HM5065MIPI_write_cmos_sensor(0x0573,0x00);
		HM5065MIPI_write_cmos_sensor(0x0081,0x7A); //	;Saturation 0x75->0x7A
		HM5065MIPI_write_cmos_sensor(0x0588,0x00); //	;ColourSaturationDamper fDisable {CompiledExposureTime}
		HM5065MIPI_write_cmos_sensor(0x0589,0x5A); //	;ColourSaturationDamper fpLowThreshold {MSB}
		HM5065MIPI_write_cmos_sensor(0x058A,0xEE); //	;ColourSaturationDamper fpLowThreshold {LSB}
		HM5065MIPI_write_cmos_sensor(0x058B,0x69); //	;ColourSaturationDamper fpHighThreshold {MSB}
		HM5065MIPI_write_cmos_sensor(0x058C,0x49); //	;ColourSaturationDamper fpHighThreshold {LSB}
		HM5065MIPI_write_cmos_sensor(0x058D,0x3D); //	;ColourSaturationDamper fpMinimumOutput {MSB}
		HM5065MIPI_write_cmos_sensor(0x058E,0x3D); //	;ColourSaturationDamper fpMinimumOutput {LSB}
		HM5065MIPI_write_cmos_sensor(0x0080,0x6C); //	;Set Contrast
		HM5065MIPI_write_cmos_sensor(0x0082,0x5A); //	;Set Brightness
		//HM5065MIPI_write_cmos_sensor(0x0010,0x01);

		//mdelay(200); // Sleep

		HM5065MIPI_write_cmos_sensor(0x4708,0x00); ;//Set horizontal and vertical offsets to zero
		HM5065MIPI_write_cmos_sensor(0x4709,0x00);
		HM5065MIPI_write_cmos_sensor(0x4710,0x00);
		HM5065MIPI_write_cmos_sensor(0x4711,0x00);

		// AF register optimuzation 
		HM5065MIPI_write_cmos_sensor(0x0658,0x00); // 2013-01-04
		HM5065MIPI_write_cmos_sensor(0x0659,0x00);//01
		HM5065MIPI_write_cmos_sensor(0x075A,0x00);//00 ->01
		HM5065MIPI_write_cmos_sensor(0x0756,0x00); //03->00,
		HM5065MIPI_write_cmos_sensor(0x06D5,0x01);
		HM5065MIPI_write_cmos_sensor(0x06D6,0x66);//60->BC->66
		HM5065MIPI_write_cmos_sensor(0x06D7,0x00);
		HM5065MIPI_write_cmos_sensor(0x06D8,0x50);//93
		HM5065MIPI_write_cmos_sensor(0x06D9,0x00);
		HM5065MIPI_write_cmos_sensor(0x06DA,0x50);//93
		HM5065MIPI_write_cmos_sensor(0x065A,0x00);
		HM5065MIPI_write_cmos_sensor(0x06C9,0x01);
		HM5065MIPI_write_cmos_sensor(0x06CD,0x01);
		HM5065MIPI_write_cmos_sensor(0x06CE,0xBD);
		HM5065MIPI_write_cmos_sensor(0x06CF,0x00);
		HM5065MIPI_write_cmos_sensor(0x06D0,0x93);
		HM5065MIPI_write_cmos_sensor(0x06D1,0x02);
		HM5065MIPI_write_cmos_sensor(0x06D2,0x30);
		HM5065MIPI_write_cmos_sensor(0x06D3,0xD4);
		HM5065MIPI_write_cmos_sensor(0x06D4,0x01);
		HM5065MIPI_write_cmos_sensor(0x06DB,0x59);
		HM5065MIPI_write_cmos_sensor(0x06DC,0x0d);
		HM5065MIPI_write_cmos_sensor(0x0730,0x00);
		HM5065MIPI_write_cmos_sensor(0x0731,0x00);
		HM5065MIPI_write_cmos_sensor(0x0732,0x03);
		HM5065MIPI_write_cmos_sensor(0x0733,0xFF);
		HM5065MIPI_write_cmos_sensor(0x0734,0x03);
		HM5065MIPI_write_cmos_sensor(0x0735,0x70);
		HM5065MIPI_write_cmos_sensor(0x0755,0x01);		
		HM5065MIPI_write_cmos_sensor(0x075B,0x01);
		HM5065MIPI_write_cmos_sensor(0x075E,0x00);
		HM5065MIPI_write_cmos_sensor(0x0764,0x01);
		HM5065MIPI_write_cmos_sensor(0x0766,0x01);
		HM5065MIPI_write_cmos_sensor(0x0768,0x01);
		HM5065MIPI_write_cmos_sensor(0x076A,0x00);
		HM5065MIPI_write_cmos_sensor(0x0758,0x01);
		HM5065MIPI_write_cmos_sensor(0x075C,0x01);
		HM5065MIPI_write_cmos_sensor(0x0770,0x98); // 98->b5
	 	HM5065MIPI_write_cmos_sensor(0x0771,0x19);
		HM5065MIPI_write_cmos_sensor(0x0772,0x1B);
		HM5065MIPI_write_cmos_sensor(0x0774,0x01);
		HM5065MIPI_write_cmos_sensor(0x0775,0x4a);
		HM5065MIPI_write_cmos_sensor(0x0777,0x00);
		HM5065MIPI_write_cmos_sensor(0x0778,0x45);
		HM5065MIPI_write_cmos_sensor(0x0779,0x00);
		HM5065MIPI_write_cmos_sensor(0x077A,0x02);
		HM5065MIPI_write_cmos_sensor(0x077D,0x01);
		HM5065MIPI_write_cmos_sensor(0x077E,0x03);
		HM5065MIPI_write_cmos_sensor(0x0783,0x10);
		HM5065MIPI_write_cmos_sensor(0x0785,0x14);
		HM5065MIPI_write_cmos_sensor(0x0788,0x04);
		HM5065MIPI_write_cmos_sensor(0x0846,0x06);
		HM5065MIPI_write_cmos_sensor(0x0847,0x05);
		//END

		HM5065MIPI_write_cmos_sensor(0x0420,0x00);//LSC_20120612
		HM5065MIPI_write_cmos_sensor(0x0421,0x09);
		HM5065MIPI_write_cmos_sensor(0x0422,0xff);
		HM5065MIPI_write_cmos_sensor(0x0423,0x9e);
		HM5065MIPI_write_cmos_sensor(0x0424,0x00);
		HM5065MIPI_write_cmos_sensor(0x0425,0x89);
		HM5065MIPI_write_cmos_sensor(0x0426,0x00);
		HM5065MIPI_write_cmos_sensor(0x0427,0xab);
		HM5065MIPI_write_cmos_sensor(0x0428,0xff);
		HM5065MIPI_write_cmos_sensor(0x0429,0xe9);
		HM5065MIPI_write_cmos_sensor(0x042a,0xff);
		HM5065MIPI_write_cmos_sensor(0x042b,0x8b);
		HM5065MIPI_write_cmos_sensor(0x042c,0x00);
		HM5065MIPI_write_cmos_sensor(0x042d,0x73);
		HM5065MIPI_write_cmos_sensor(0x042e,0xff);
		HM5065MIPI_write_cmos_sensor(0x042f,0xb6);
		HM5065MIPI_write_cmos_sensor(0x0430,0x00);
		HM5065MIPI_write_cmos_sensor(0x0431,0x54);
		HM5065MIPI_write_cmos_sensor(0x0432,0xff);
		HM5065MIPI_write_cmos_sensor(0x0433,0x43);
		HM5065MIPI_write_cmos_sensor(0x0434,0x01);
		HM5065MIPI_write_cmos_sensor(0x0435,0x04);
		HM5065MIPI_write_cmos_sensor(0x0436,0x01);
		HM5065MIPI_write_cmos_sensor(0x0437,0x34);
		HM5065MIPI_write_cmos_sensor(0x0438,0xff);
		HM5065MIPI_write_cmos_sensor(0x0439,0x7c);
		HM5065MIPI_write_cmos_sensor(0x043a,0xfe);
		HM5065MIPI_write_cmos_sensor(0x043b,0xd2);
		HM5065MIPI_write_cmos_sensor(0x043c,0x00);
		HM5065MIPI_write_cmos_sensor(0x043d,0x63);
		HM5065MIPI_write_cmos_sensor(0x043e,0xff);
		HM5065MIPI_write_cmos_sensor(0x043f,0x15);
		HM5065MIPI_write_cmos_sensor(0x0450,0x00);
		HM5065MIPI_write_cmos_sensor(0x0451,0x3b);
		HM5065MIPI_write_cmos_sensor(0x0452,0xff);
		HM5065MIPI_write_cmos_sensor(0x0453,0x98);
		HM5065MIPI_write_cmos_sensor(0x0454,0x00);
		HM5065MIPI_write_cmos_sensor(0x0455,0x6f);
		HM5065MIPI_write_cmos_sensor(0x0456,0x00);
		HM5065MIPI_write_cmos_sensor(0x0457,0x93);
		HM5065MIPI_write_cmos_sensor(0x0458,0xff);
		HM5065MIPI_write_cmos_sensor(0x0459,0xad);
		HM5065MIPI_write_cmos_sensor(0x045a,0xff);
		HM5065MIPI_write_cmos_sensor(0x045b,0x87);
		HM5065MIPI_write_cmos_sensor(0x045c,0x00);
		HM5065MIPI_write_cmos_sensor(0x045d,0x52);
		HM5065MIPI_write_cmos_sensor(0x045e,0xff);
		HM5065MIPI_write_cmos_sensor(0x045f,0xa7);
		HM5065MIPI_write_cmos_sensor(0x0440,0xff);
		HM5065MIPI_write_cmos_sensor(0x0441,0xfd);
		HM5065MIPI_write_cmos_sensor(0x0442,0xff);
		HM5065MIPI_write_cmos_sensor(0x0443,0x6c);
		HM5065MIPI_write_cmos_sensor(0x0444,0x00);
		HM5065MIPI_write_cmos_sensor(0x0445,0x90);
		HM5065MIPI_write_cmos_sensor(0x0446,0x00);
		HM5065MIPI_write_cmos_sensor(0x0447,0xa1);
		HM5065MIPI_write_cmos_sensor(0x0448,0x00);
		HM5065MIPI_write_cmos_sensor(0x0449,0x02);
		HM5065MIPI_write_cmos_sensor(0x044a,0xff);
		HM5065MIPI_write_cmos_sensor(0x044b,0x48);
		HM5065MIPI_write_cmos_sensor(0x044c,0x00);
		HM5065MIPI_write_cmos_sensor(0x044d,0x5b);
		HM5065MIPI_write_cmos_sensor(0x044e,0xff);
		HM5065MIPI_write_cmos_sensor(0x044f,0xb4);
		HM5065MIPI_write_cmos_sensor(0x0460,0xff);
		HM5065MIPI_write_cmos_sensor(0x0461,0x69);
		HM5065MIPI_write_cmos_sensor(0x0462,0xff);
		HM5065MIPI_write_cmos_sensor(0x0463,0xbb);
		HM5065MIPI_write_cmos_sensor(0x0464,0x00);
		HM5065MIPI_write_cmos_sensor(0x0465,0x84);
		HM5065MIPI_write_cmos_sensor(0x0466,0x00);
		HM5065MIPI_write_cmos_sensor(0x0467,0xa3);
		HM5065MIPI_write_cmos_sensor(0x0468,0x00);
		HM5065MIPI_write_cmos_sensor(0x0469,0x0e);
		HM5065MIPI_write_cmos_sensor(0x046a,0x00);
		HM5065MIPI_write_cmos_sensor(0x046b,0x76);
		HM5065MIPI_write_cmos_sensor(0x046c,0xff);
		HM5065MIPI_write_cmos_sensor(0x046d,0xaf);
		HM5065MIPI_write_cmos_sensor(0x046e,0xff);
		HM5065MIPI_write_cmos_sensor(0x046f,0xf5);
		HM5065MIPI_write_cmos_sensor(0x0470,0xff);
		HM5065MIPI_write_cmos_sensor(0x0471,0x8a);
		HM5065MIPI_write_cmos_sensor(0x0472,0xff);
		HM5065MIPI_write_cmos_sensor(0x0473,0x5a);
		HM5065MIPI_write_cmos_sensor(0x0474,0x00);
		HM5065MIPI_write_cmos_sensor(0x0475,0xef);
		HM5065MIPI_write_cmos_sensor(0x0476,0x01);
		HM5065MIPI_write_cmos_sensor(0x0477,0x16);
		HM5065MIPI_write_cmos_sensor(0x0478,0xff);
		HM5065MIPI_write_cmos_sensor(0x0479,0xd4);
		HM5065MIPI_write_cmos_sensor(0x047a,0x00);
		HM5065MIPI_write_cmos_sensor(0x047b,0x02);
		HM5065MIPI_write_cmos_sensor(0x047c,0x00);
		HM5065MIPI_write_cmos_sensor(0x047d,0x2c);
		HM5065MIPI_write_cmos_sensor(0x047e,0xff);
		HM5065MIPI_write_cmos_sensor(0x047f,0x95);
		HM5065MIPI_write_cmos_sensor(0x0490,0xff);
		HM5065MIPI_write_cmos_sensor(0x0491,0x9b);
		HM5065MIPI_write_cmos_sensor(0x0492,0xff);
		HM5065MIPI_write_cmos_sensor(0x0493,0x91);
		HM5065MIPI_write_cmos_sensor(0x0494,0x00);
		HM5065MIPI_write_cmos_sensor(0x0495,0x6f);
		HM5065MIPI_write_cmos_sensor(0x0496,0x00);
		HM5065MIPI_write_cmos_sensor(0x0497,0x95);
		HM5065MIPI_write_cmos_sensor(0x0498,0xff);
		HM5065MIPI_write_cmos_sensor(0x0499,0xd5);
		HM5065MIPI_write_cmos_sensor(0x049a,0x01);
		HM5065MIPI_write_cmos_sensor(0x049b,0x20);
		HM5065MIPI_write_cmos_sensor(0x049c,0xff);
		HM5065MIPI_write_cmos_sensor(0x049d,0xfb);
		HM5065MIPI_write_cmos_sensor(0x049e,0xff);
		HM5065MIPI_write_cmos_sensor(0x049f,0xe1);
		HM5065MIPI_write_cmos_sensor(0x0480,0xff);
		HM5065MIPI_write_cmos_sensor(0x0481,0x5a);
		HM5065MIPI_write_cmos_sensor(0x0482,0xff);
		HM5065MIPI_write_cmos_sensor(0x0483,0x91);
		HM5065MIPI_write_cmos_sensor(0x0484,0x00);
		HM5065MIPI_write_cmos_sensor(0x0485,0x8c);
		HM5065MIPI_write_cmos_sensor(0x0486,0x00);
		HM5065MIPI_write_cmos_sensor(0x0487,0x9f);
		HM5065MIPI_write_cmos_sensor(0x0488,0x00);
		HM5065MIPI_write_cmos_sensor(0x0489,0x29);
		HM5065MIPI_write_cmos_sensor(0x048a,0x00);
		HM5065MIPI_write_cmos_sensor(0x048b,0x53);
		HM5065MIPI_write_cmos_sensor(0x048c,0xff);
		HM5065MIPI_write_cmos_sensor(0x048d,0x80);
		HM5065MIPI_write_cmos_sensor(0x048e,0xff);
		HM5065MIPI_write_cmos_sensor(0x048f,0xf7);
		HM5065MIPI_write_cmos_sensor(0x04a0,0xff);
		HM5065MIPI_write_cmos_sensor(0x04a1,0x6c);
		HM5065MIPI_write_cmos_sensor(0x04a2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04a3,0xb9);
		HM5065MIPI_write_cmos_sensor(0x04a4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04a5,0x81);
		HM5065MIPI_write_cmos_sensor(0x04a6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04a7,0x93);
		HM5065MIPI_write_cmos_sensor(0x04a8,0x00);
		HM5065MIPI_write_cmos_sensor(0x04a9,0x1c);
		HM5065MIPI_write_cmos_sensor(0x04aa,0x00);
		HM5065MIPI_write_cmos_sensor(0x04ab,0x39);
		HM5065MIPI_write_cmos_sensor(0x04ac,0xff);
		HM5065MIPI_write_cmos_sensor(0x04ad,0x9f);
		HM5065MIPI_write_cmos_sensor(0x04ae,0x00);
		HM5065MIPI_write_cmos_sensor(0x04af,0x0e);
		HM5065MIPI_write_cmos_sensor(0x04b0,0xff);
		HM5065MIPI_write_cmos_sensor(0x04b1,0x96);
		HM5065MIPI_write_cmos_sensor(0x04b2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04b3,0x7b);
		HM5065MIPI_write_cmos_sensor(0x04b4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04b5,0xaa);
		HM5065MIPI_write_cmos_sensor(0x04b6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04b7,0xc8);
		HM5065MIPI_write_cmos_sensor(0x04b8,0xff);
		HM5065MIPI_write_cmos_sensor(0x04b9,0xe1);
		HM5065MIPI_write_cmos_sensor(0x04ba,0x00);
		HM5065MIPI_write_cmos_sensor(0x04bb,0x0e);
		HM5065MIPI_write_cmos_sensor(0x04bc,0x00);
		HM5065MIPI_write_cmos_sensor(0x04bd,0x0b);
		HM5065MIPI_write_cmos_sensor(0x04be,0xff);
		HM5065MIPI_write_cmos_sensor(0x04bf,0xff);
		HM5065MIPI_write_cmos_sensor(0x04d0,0xff);
		HM5065MIPI_write_cmos_sensor(0x04d1,0xac);
		HM5065MIPI_write_cmos_sensor(0x04d2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04d3,0x93);
		HM5065MIPI_write_cmos_sensor(0x04d4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04d5,0x64);
		HM5065MIPI_write_cmos_sensor(0x04d6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04d7,0x83);
		HM5065MIPI_write_cmos_sensor(0x04d8,0xff);
		HM5065MIPI_write_cmos_sensor(0x04d9,0xdb);
		HM5065MIPI_write_cmos_sensor(0x04da,0x00);
		HM5065MIPI_write_cmos_sensor(0x04db,0xa8);
		HM5065MIPI_write_cmos_sensor(0x04dc,0xff);
		HM5065MIPI_write_cmos_sensor(0x04dd,0xf5);
		HM5065MIPI_write_cmos_sensor(0x04de,0x00);
		HM5065MIPI_write_cmos_sensor(0x04df,0x15);
		HM5065MIPI_write_cmos_sensor(0x04c0,0xff);
		HM5065MIPI_write_cmos_sensor(0x04c1,0x5d);
		HM5065MIPI_write_cmos_sensor(0x04c2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04c3,0x9c);
		HM5065MIPI_write_cmos_sensor(0x04c4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04c5,0x82);
		HM5065MIPI_write_cmos_sensor(0x04c6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04c7,0x96);
		HM5065MIPI_write_cmos_sensor(0x04c8,0x00);
		HM5065MIPI_write_cmos_sensor(0x04c9,0x33);
		HM5065MIPI_write_cmos_sensor(0x04ca,0x00);
		HM5065MIPI_write_cmos_sensor(0x04cb,0x07);
		HM5065MIPI_write_cmos_sensor(0x04cc,0xff);
		HM5065MIPI_write_cmos_sensor(0x04cd,0x71);
		HM5065MIPI_write_cmos_sensor(0x04ce,0x00);
		HM5065MIPI_write_cmos_sensor(0x04cf,0x11);
		HM5065MIPI_write_cmos_sensor(0x04e0,0xff);
		HM5065MIPI_write_cmos_sensor(0x04e1,0x6D);//76->6D
		HM5065MIPI_write_cmos_sensor(0x04e2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04e3,0xb8);
		HM5065MIPI_write_cmos_sensor(0x04e4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04e5,0x84);
		HM5065MIPI_write_cmos_sensor(0x04e6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04e7,0x96);
		HM5065MIPI_write_cmos_sensor(0x04e8,0xFF);//00->FF
		HM5065MIPI_write_cmos_sensor(0x04e9,0xC0);//21->C0
		HM5065MIPI_write_cmos_sensor(0x04ea,0x00);
		HM5065MIPI_write_cmos_sensor(0x04eb,0x6d);
		HM5065MIPI_write_cmos_sensor(0x04ec,0xff);
		HM5065MIPI_write_cmos_sensor(0x04ed,0xbb);
		HM5065MIPI_write_cmos_sensor(0x04ee,0x00);
		HM5065MIPI_write_cmos_sensor(0x04ef,0x00);
		HM5065MIPI_write_cmos_sensor(0x04f0,0xff);
		HM5065MIPI_write_cmos_sensor(0x04f1,0xA0);//90->A0
		HM5065MIPI_write_cmos_sensor(0x04f2,0xff);
		HM5065MIPI_write_cmos_sensor(0x04f3,0x95);//6D->95
		HM5065MIPI_write_cmos_sensor(0x04f4,0x00);
		HM5065MIPI_write_cmos_sensor(0x04f5,0xA7);//D2->A7
		HM5065MIPI_write_cmos_sensor(0x04f6,0x00);
		HM5065MIPI_write_cmos_sensor(0x04f7,0xC8);//FC->C8
		HM5065MIPI_write_cmos_sensor(0x04f8,0xff);
		HM5065MIPI_write_cmos_sensor(0x04f9,0xde);
		HM5065MIPI_write_cmos_sensor(0x04fa,0x00);
		HM5065MIPI_write_cmos_sensor(0x04fb,0x7e);
		HM5065MIPI_write_cmos_sensor(0x04fc,0x00);
		HM5065MIPI_write_cmos_sensor(0x04fd,0x36);
		HM5065MIPI_write_cmos_sensor(0x04fe,0x00);//FF->00
		HM5065MIPI_write_cmos_sensor(0x04ff,0x10);//AE->10
		HM5065MIPI_write_cmos_sensor(0x0510,0xff);
		HM5065MIPI_write_cmos_sensor(0x0511,0xc1);
		HM5065MIPI_write_cmos_sensor(0x0512,0xff);
		HM5065MIPI_write_cmos_sensor(0x0513,0x9f);
		HM5065MIPI_write_cmos_sensor(0x0514,0x00);
		HM5065MIPI_write_cmos_sensor(0x0515,0x6a);
		HM5065MIPI_write_cmos_sensor(0x0516,0x00);
		HM5065MIPI_write_cmos_sensor(0x0517,0x89);
		HM5065MIPI_write_cmos_sensor(0x0518,0xff);
		HM5065MIPI_write_cmos_sensor(0x0519,0xdc);
		HM5065MIPI_write_cmos_sensor(0x051a,0x00);
		HM5065MIPI_write_cmos_sensor(0x051b,0x55);
		HM5065MIPI_write_cmos_sensor(0x051c,0x00);
		HM5065MIPI_write_cmos_sensor(0x051d,0x09);
		HM5065MIPI_write_cmos_sensor(0x051e,0x00);
		HM5065MIPI_write_cmos_sensor(0x051f,0x0d);
		HM5065MIPI_write_cmos_sensor(0x0500,0xff);
		HM5065MIPI_write_cmos_sensor(0x0501,0x60);
		HM5065MIPI_write_cmos_sensor(0x0502,0xff);
		HM5065MIPI_write_cmos_sensor(0x0503,0x9e);
		HM5065MIPI_write_cmos_sensor(0x0504,0x00);
		HM5065MIPI_write_cmos_sensor(0x0505,0x81);
		HM5065MIPI_write_cmos_sensor(0x0506,0x00);
		HM5065MIPI_write_cmos_sensor(0x0507,0x9c);
		HM5065MIPI_write_cmos_sensor(0x0508,0xFF);//00->FF
		HM5065MIPI_write_cmos_sensor(0x0509,0xC0);//36->C0
		HM5065MIPI_write_cmos_sensor(0x050a,0x00);
		HM5065MIPI_write_cmos_sensor(0x050b,0x40);
		HM5065MIPI_write_cmos_sensor(0x050c,0xff);
		HM5065MIPI_write_cmos_sensor(0x050d,0x8e);
		HM5065MIPI_write_cmos_sensor(0x050e,0x00);
		HM5065MIPI_write_cmos_sensor(0x050f,0x00);
		HM5065MIPI_write_cmos_sensor(0x0561,0x0e);
		HM5065MIPI_write_cmos_sensor(0x0562,0x01);
		HM5065MIPI_write_cmos_sensor(0x0563,0x01);
		HM5065MIPI_write_cmos_sensor(0x0564,0x06);
		
		//MIPI CSI setting
		HM5065MIPI_write_cmos_sensor(0xC41A,0x05);
		HM5065MIPI_write_cmos_sensor(0xC423,0x11);
		HM5065MIPI_write_cmos_sensor(0xC427,0x11);
		HM5065MIPI_write_cmos_sensor(0x300B,0x08);
	
		//80M  hkm test 
//		HM5065MIPI_write_cmos_sensor(0x00B2,0x50);//4e   //50//set PLL output 560MHz
//		HM5065MIPI_write_cmos_sensor(0x00B3,0xC9);//CA
//		HM5065MIPI_write_cmos_sensor(0x00B5,0x01);//02
//		HM5065MIPI_write_cmos_sensor(0x0030,0x11);//Max.Derate//14kai
//		HM5065MIPI_write_cmos_sensor(0x0143,0x5F);//Max.Int time_H	0x64->5F
//		HM5065MIPI_write_cmos_sensor(0x0144,0x0D);//Max.Int time_L	0x7F->0D		

//		HM5065MIPI_write_cmos_sensor(0x01D4,0x3C);//AWB step
//		HM5065MIPI_write_cmos_sensor(0x01D5,0x00);

//		HM5065MIPI_write_cmos_sensor(0x0010,0x02);
//		HM5065MIPI_write_cmos_sensor(0x00B2,0x50);//4e   //50//set PLL output 560MHz
//		HM5065MIPI_write_cmos_sensor(0x00B3,0xC9);//CA
//		mdelay(200);
		HM5065MIPI_write_cmos_sensor(0x0010,0x01);
		mdelay(200);
#else
//#endif
//new setting
HM5065MIPI_write_cmos_sensor(0xffff,0x01);//  MCU bypass//
HM5065MIPI_write_cmos_sensor(0x9000,0x03);//  Enable Ram and enable Write//

///////////////////////////////////////////////////////////////////////////////
//BP0 : Contrast  
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA000,0x90);//     MOV      DPTR,#fpInputRange(0x0C56);
HM5065MIPI_write_cmos_sensor(0xA001,0x0C);//  
HM5065MIPI_write_cmos_sensor(0xA002,0x56);//  
HM5065MIPI_write_cmos_sensor(0xA003,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA004,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA005,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA006,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA007,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA008,0x12);//     LCALL    FPAlu_FloatToInt16(C:4285);
HM5065MIPI_write_cmos_sensor(0xA009,0x42);//  
HM5065MIPI_write_cmos_sensor(0xA00A,0x85);//  
HM5065MIPI_write_cmos_sensor(0xA00B,0x90);//     MOV      DPTR,#0x01B7  (0x0B4D);
HM5065MIPI_write_cmos_sensor(0xA00C,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA00D,0xB7);//  
HM5065MIPI_write_cmos_sensor(0xA00E,0xEE);//     MOV      A,R6
HM5065MIPI_write_cmos_sensor(0xA00F,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA010,0xFC);//     MOV      R4,A
HM5065MIPI_write_cmos_sensor(0xA011,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA012,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA013,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA014,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA015,0x90);//     MOV      DPTR,#0x0605
HM5065MIPI_write_cmos_sensor(0xA016,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA017,0x05);//  
HM5065MIPI_write_cmos_sensor(0xA018,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA019,0x75);//     MOV      B(0xF0);,#0x02
HM5065MIPI_write_cmos_sensor(0xA01A,0xF0);//  
HM5065MIPI_write_cmos_sensor(0xA01B,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA01C,0xA4);//     MUL      AB
HM5065MIPI_write_cmos_sensor(0xA01D,0x2D);//     ADD      A,R5
HM5065MIPI_write_cmos_sensor(0xA01E,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA01F,0xE5);//     MOV      A,B(0xF0);
HM5065MIPI_write_cmos_sensor(0xA020,0xF0);//  
HM5065MIPI_write_cmos_sensor(0xA021,0x3C);//     ADDC     A,R4
HM5065MIPI_write_cmos_sensor(0xA022,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA023,0xAB);//     MOV      R3 07
HM5065MIPI_write_cmos_sensor(0xA024,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA025,0xFA);//     MOV      R2,A
HM5065MIPI_write_cmos_sensor(0xA026,0x33);//     RLC      A
HM5065MIPI_write_cmos_sensor(0xA027,0x95);//     SUBB     A,ACC(0xE0);
HM5065MIPI_write_cmos_sensor(0xA028,0xE0);//  
HM5065MIPI_write_cmos_sensor(0xA029,0xF9);//     MOV      R1,A
HM5065MIPI_write_cmos_sensor(0xA02A,0xF8);//     MOV      R0,A
HM5065MIPI_write_cmos_sensor(0xA02B,0x90);//     MOV      DPTR,#0x0B4B
HM5065MIPI_write_cmos_sensor(0xA02C,0x0B);//  
HM5065MIPI_write_cmos_sensor(0xA02D,0x4B);//  
HM5065MIPI_write_cmos_sensor(0xA02E,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA02F,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA030,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA031,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA032,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA033,0xEE);//     MOV      A,R6
HM5065MIPI_write_cmos_sensor(0xA034,0x33);//     RLC      A
HM5065MIPI_write_cmos_sensor(0xA035,0x95);//     SUBB     A,ACC(0xE0);
HM5065MIPI_write_cmos_sensor(0xA036,0xE0);//  
HM5065MIPI_write_cmos_sensor(0xA037,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA038,0xFC);//     MOV      R4,A
HM5065MIPI_write_cmos_sensor(0xA039,0x12);//     LCALL    C?LMUL(C:0C7B);
HM5065MIPI_write_cmos_sensor(0xA03A,0x0C);//  
HM5065MIPI_write_cmos_sensor(0xA03B,0x7B);//  
HM5065MIPI_write_cmos_sensor(0xA03C,0x90);//     MOV      DPTR,#0x01B9(0x0B4F);
HM5065MIPI_write_cmos_sensor(0xA03D,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA03E,0xB9);//  
HM5065MIPI_write_cmos_sensor(0xA03F,0x12);//     LCALL    C?LSTXDATA(C:0E05);
HM5065MIPI_write_cmos_sensor(0xA040,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA041,0x05);//  
HM5065MIPI_write_cmos_sensor(0xA042,0x90);//     MOV      DPTR,#0x01B9(0x0B4F);
HM5065MIPI_write_cmos_sensor(0xA043,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA044,0xB9);//  
HM5065MIPI_write_cmos_sensor(0xA045,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA046,0xFC);//     MOV      R4,A
HM5065MIPI_write_cmos_sensor(0xA047,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA048,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA049,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA04A,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA04B,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA04C,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA04D,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA04E,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA04F,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA050,0x78);//     MOV      R0,#g_fTimer0TimeOut(0x08);
HM5065MIPI_write_cmos_sensor(0xA051,0x08);//  
HM5065MIPI_write_cmos_sensor(0xA052,0x12);//     LCALL    C?ULSHR(C:0DBF);
HM5065MIPI_write_cmos_sensor(0xA053,0x0D);//  
HM5065MIPI_write_cmos_sensor(0xA054,0xBF);//  
HM5065MIPI_write_cmos_sensor(0xA055,0xA8);//     MOV      R0,uwDelay1000(0x04);
HM5065MIPI_write_cmos_sensor(0xA056,0x04);//  
HM5065MIPI_write_cmos_sensor(0xA057,0xA9);//     MOV      R1 05
HM5065MIPI_write_cmos_sensor(0xA058,0x05);//  
HM5065MIPI_write_cmos_sensor(0xA059,0xAA);//     MOV      R2,uwDelay100(0x06);
HM5065MIPI_write_cmos_sensor(0xA05A,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA05B,0xAB);//     MOV      R3 07
HM5065MIPI_write_cmos_sensor(0xA05C,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA05D,0x90);//     MOV      DPTR,#0x0B49
HM5065MIPI_write_cmos_sensor(0xA05E,0x0B);//  
HM5065MIPI_write_cmos_sensor(0xA05F,0x49);//  
HM5065MIPI_write_cmos_sensor(0xA060,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA061,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA062,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA063,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA064,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA065,0xEE);//     MOV      A,R6
HM5065MIPI_write_cmos_sensor(0xA066,0x33);//     RLC      A
HM5065MIPI_write_cmos_sensor(0xA067,0x95);//     SUBB     A,ACC(0xE0);
HM5065MIPI_write_cmos_sensor(0xA068,0xE0);//  
HM5065MIPI_write_cmos_sensor(0xA069,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA06A,0xFC);//     MOV      R4,A
HM5065MIPI_write_cmos_sensor(0xA06B,0xC3);//     CLR      C
HM5065MIPI_write_cmos_sensor(0xA06C,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA06D,0x9B);//     SUBB     A,R3
HM5065MIPI_write_cmos_sensor(0xA06E,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA06F,0xEE);//     MOV      A,R6
HM5065MIPI_write_cmos_sensor(0xA070,0x9A);//     SUBB     A,R2
HM5065MIPI_write_cmos_sensor(0xA071,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA072,0xED);//     MOV      A,R5
HM5065MIPI_write_cmos_sensor(0xA073,0x99);//     SUBB     A,R1
HM5065MIPI_write_cmos_sensor(0xA074,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA075,0xEC);//     MOV      A,R4
HM5065MIPI_write_cmos_sensor(0xA076,0x98);//     SUBB     A,R0
HM5065MIPI_write_cmos_sensor(0xA077,0xFC);//     MOV      R4,A
HM5065MIPI_write_cmos_sensor(0xA078,0x78);//     MOV      R0,#0x01
HM5065MIPI_write_cmos_sensor(0xA079,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA07A,0x12);//     LCALL    C?ULSHR(C:0DBF);
HM5065MIPI_write_cmos_sensor(0xA07B,0x0D);//  
HM5065MIPI_write_cmos_sensor(0xA07C,0xBF);//  
HM5065MIPI_write_cmos_sensor(0xA07D,0x90);//     MOV      DPTR,#m_pxwOffsetVector(0x0C4A);
HM5065MIPI_write_cmos_sensor(0xA07E,0x0C);//  
HM5065MIPI_write_cmos_sensor(0xA07F,0x4A);//  
HM5065MIPI_write_cmos_sensor(0xA080,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA081,0xFC);//     MOV      R4,A
HM5065MIPI_write_cmos_sensor(0xA082,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA083,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA084,0xF5);//     MOV      DPL(0x82);,A
HM5065MIPI_write_cmos_sensor(0xA085,0x82);//  
HM5065MIPI_write_cmos_sensor(0xA086,0x8C);//     MOV      DPH(0x83);,R4
HM5065MIPI_write_cmos_sensor(0xA087,0x83);//  
HM5065MIPI_write_cmos_sensor(0xA088,0xC0);//     PUSH     DPH(0x83);
HM5065MIPI_write_cmos_sensor(0xA089,0x83);//  
HM5065MIPI_write_cmos_sensor(0xA08A,0xC0);//     PUSH     DPL(0x82);
HM5065MIPI_write_cmos_sensor(0xA08B,0x82);//  
HM5065MIPI_write_cmos_sensor(0xA08C,0x90);//     MOV      DPTR,#0x0B48
HM5065MIPI_write_cmos_sensor(0xA08D,0x0B);//  
HM5065MIPI_write_cmos_sensor(0xA08E,0x48);//  
HM5065MIPI_write_cmos_sensor(0xA08F,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA090,0xD0);//     POP      DPL(0x82);
HM5065MIPI_write_cmos_sensor(0xA091,0x82);//  
HM5065MIPI_write_cmos_sensor(0xA092,0xD0);//     POP      DPH(0x83);
HM5065MIPI_write_cmos_sensor(0xA093,0x83);//  
HM5065MIPI_write_cmos_sensor(0xA094,0x75);//     MOV      B(0xF0);,#0x02
HM5065MIPI_write_cmos_sensor(0xA095,0xF0);//  
HM5065MIPI_write_cmos_sensor(0xA096,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA097,0x12);//     LCALL    C?OFFXADD(C:0E45);
HM5065MIPI_write_cmos_sensor(0xA098,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA099,0x45);//  
HM5065MIPI_write_cmos_sensor(0xA09A,0xEE);//     MOV      A,R6
HM5065MIPI_write_cmos_sensor(0xA09B,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA09C,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA09D,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA09E,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA09F,0x02);//     LJMP     C:BAD8
HM5065MIPI_write_cmos_sensor(0xA0A0,0xBA);//  
HM5065MIPI_write_cmos_sensor(0xA0A1,0xD8);//  


///////////////////////////////////////////////////////////////////////////////
//BP 1: CSI reset
///////////////////////////////////////////////////////////////////////////////
//HM5065MIPI_write_cmos_sensor(0xA0A2,0x90);//     MOV      DPTR,#0x0036
//HM5065MIPI_write_cmos_sensor(0xA0A3,0x72);//  
//HM5065MIPI_write_cmos_sensor(0xA0A4,0x00);//  
//HM5065MIPI_write_cmos_sensor(0xA0A5,0x74);//     MOV      A,#0x00
//HM5065MIPI_write_cmos_sensor(0xA0A6,0x01);//  
//HM5065MIPI_write_cmos_sensor(0xA0A7,0xF0);//  	MOVX     @DPTR,A
//HM5065MIPI_write_cmos_sensor(0xA0A8,0x74);//     INC      DPTR
//HM5065MIPI_write_cmos_sensor(0xA0A9,0x00);//  	MOV      A,#0x96
//HM5065MIPI_write_cmos_sensor(0xA0AA,0xF0);//  
//HM5065MIPI_write_cmos_sensor(0xA0AB,0x90);//     MOVX     @DPTR,A
//HM5065MIPI_write_cmos_sensor(0xA0AC,0xC3);//  	MOV      DPTR,#0x003D
//HM5065MIPI_write_cmos_sensor(0xA0AD,0x88);//  
//HM5065MIPI_write_cmos_sensor(0xA0AE,0x02);//     
//HM5065MIPI_write_cmos_sensor(0xA0AF,0xE8);//  	LJMP     C:9DA8
//HM5065MIPI_write_cmos_sensor(0xA0B0,0x61);//  

///////////////////////////////////////////////////////////////////////////////
// BP 21: Reset pipe at stop stage 
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA0A2,0x90);//         
HM5065MIPI_write_cmos_sensor(0xA0A3,0x30);//        MOV      DPTR,#0x0036
HM5065MIPI_write_cmos_sensor(0xA0A4,0x18);//        
HM5065MIPI_write_cmos_sensor(0xA0A5,0xe4);//        
HM5065MIPI_write_cmos_sensor(0xA0A6,0xf0);//        MOV      A,#0x00
HM5065MIPI_write_cmos_sensor(0xA0A7,0x74);//        
HM5065MIPI_write_cmos_sensor(0xA0A8,0x3f);//        MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA0A9,0xf0);//        INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA0AA,0x22);//        INC      DPTR


///////////////////////////////////////////////////////////////////////////////
// BP 2: CSI ULP enter 1
///////////////////////////////////////////////////////////////////////////////

//HM5065MIPI_write_cmos_sensor(0xA0B1,0x02);//     
//HM5065MIPI_write_cmos_sensor(0xA0B2,0xE6);//  	LJMP     C:9DA8
//HM5065MIPI_write_cmos_sensor(0xA0B3,0x7C);//  

///////////////////////////////////////////////////////////////////////////////
// BP 3: CSI ULP enter 2
///////////////////////////////////////////////////////////////////////////////

//HM5065MIPI_write_cmos_sensor(0xA0B4,0xF0);//     
//HM5065MIPI_write_cmos_sensor(0xA0B5,0x90);//     
//HM5065MIPI_write_cmos_sensor(0xA0B6,0x72);//  	LJMP     C:9DA8
//HM5065MIPI_write_cmos_sensor(0xA0B7,0x08);//  
//HM5065MIPI_write_cmos_sensor(0xA0B8,0xE0);//     
//HM5065MIPI_write_cmos_sensor(0xA0B9,0x44);//  	LJMP     C:9DA8
//HM5065MIPI_write_cmos_sensor(0xA0BA,0x10);//  
//HM5065MIPI_write_cmos_sensor(0xA0BB,0xF0);//     
//HM5065MIPI_write_cmos_sensor(0xA0BC,0x02);//  	LJMP     C:9DA8
//HM5065MIPI_write_cmos_sensor(0xA0BD,0xE6);//  
//HM5065MIPI_write_cmos_sensor(0xA0BE,0x9A);//    

///////////////////////////////////////////////////////////////////////////////
//BP 4: AV
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA0BF,0x90);//     MOV      DPTR,#0x005E
HM5065MIPI_write_cmos_sensor(0xA0C0,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA0C1,0x5E);//  
HM5065MIPI_write_cmos_sensor(0xA0C2,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA0C3,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA0C4,0x70);//     JNZ      B00:A9AF
HM5065MIPI_write_cmos_sensor(0xA0C5,0x20);//  
HM5065MIPI_write_cmos_sensor(0xA0C6,0x90);//     MOV      DPTR,#Av2x2_H_Size(0x4704);
HM5065MIPI_write_cmos_sensor(0xA0C7,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA0C8,0x04);//  
HM5065MIPI_write_cmos_sensor(0xA0C9,0x74);//     MOV      A,#bInt_Event_Status(0x0A);
HM5065MIPI_write_cmos_sensor(0xA0CA,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA0CB,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA0CC,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA0CD,0x74);//     MOV      A,#0x30
HM5065MIPI_write_cmos_sensor(0xA0CE,0x30);//  
HM5065MIPI_write_cmos_sensor(0xA0CF,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA0D0,0x90);//     MOV      DPTR,#Av2x2_V_Size(0x470C);
HM5065MIPI_write_cmos_sensor(0xA0D1,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA0D2,0x0C);//  
HM5065MIPI_write_cmos_sensor(0xA0D3,0x74);//     MOV      A,#0x07
HM5065MIPI_write_cmos_sensor(0xA0D4,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA0D5,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA0D6,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA0D7,0x74);//     MOV      A,#IE(0xA8);
HM5065MIPI_write_cmos_sensor(0xA0D8,0xA8);//  
HM5065MIPI_write_cmos_sensor(0xA0D9,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA0DA,0x90);//     MOV      DPTR,#Av2x2_Xscale(0x47A4);
HM5065MIPI_write_cmos_sensor(0xA0DB,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA0DC,0xA4);//  
HM5065MIPI_write_cmos_sensor(0xA0DD,0x74);//     MOV      A,#0x01
HM5065MIPI_write_cmos_sensor(0xA0DE,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA0DF,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA0E0,0x90);//     MOV      DPTR,#Av2x2_Yscale(0x47A8);
HM5065MIPI_write_cmos_sensor(0xA0E1,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA0E2,0xA8);//  
HM5065MIPI_write_cmos_sensor(0xA0E3,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA0E4,0x80);//     SJMP     B00:A9FF
HM5065MIPI_write_cmos_sensor(0xA0E5,0x50);//  
HM5065MIPI_write_cmos_sensor(0xA0E6,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA0E7,0x64);//     XRL      A,#0x01
HM5065MIPI_write_cmos_sensor(0xA0E8,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA0E9,0x60);//     JZ       B00:A9B8
HM5065MIPI_write_cmos_sensor(0xA0EA,0x04);//  
HM5065MIPI_write_cmos_sensor(0xA0EB,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA0EC,0xB4);//     CJNE     A,#0x03,B00:A9D8
HM5065MIPI_write_cmos_sensor(0xA0ED,0x03);//  
HM5065MIPI_write_cmos_sensor(0xA0EE,0x20);//  
HM5065MIPI_write_cmos_sensor(0xA0EF,0x90);//     MOV      DPTR,#Av2x2_H_Size(0x4704);
HM5065MIPI_write_cmos_sensor(0xA0F0,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA0F1,0x04);//  
HM5065MIPI_write_cmos_sensor(0xA0F2,0x74);//     MOV      A,#0x05
HM5065MIPI_write_cmos_sensor(0xA0F3,0x05);//  
HM5065MIPI_write_cmos_sensor(0xA0F4,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA0F5,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA0F6,0x74);//     MOV      A,#0x18
HM5065MIPI_write_cmos_sensor(0xA0F7,0x18);//  
HM5065MIPI_write_cmos_sensor(0xA0F8,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA0F9,0x90);//     MOV      DPTR,#Av2x2_V_Size(0x470C);
HM5065MIPI_write_cmos_sensor(0xA0FA,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA0FB,0x0C);//  
HM5065MIPI_write_cmos_sensor(0xA0FC,0x74);//     MOV      A,#0x03
HM5065MIPI_write_cmos_sensor(0xA0FD,0x03);//  
HM5065MIPI_write_cmos_sensor(0xA0FE,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA0FF,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA100,0x74);//     MOV      A,#m_fDitherBitFormat(0xD4);
HM5065MIPI_write_cmos_sensor(0xA101,0xD4);//  
HM5065MIPI_write_cmos_sensor(0xA102,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA103,0x90);//     MOV      DPTR,#Av2x2_Xscale(0x47A4);
HM5065MIPI_write_cmos_sensor(0xA104,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA105,0xA4);//  
HM5065MIPI_write_cmos_sensor(0xA106,0x74);//     MOV      A,#0x02
HM5065MIPI_write_cmos_sensor(0xA107,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA108,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA109,0x90);//     MOV      DPTR,#Av2x2_Yscale(0x47A8);
HM5065MIPI_write_cmos_sensor(0xA10A,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA10B,0xA8);//  
HM5065MIPI_write_cmos_sensor(0xA10C,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA10D,0x80);//     SJMP     B00:A9FF
HM5065MIPI_write_cmos_sensor(0xA10E,0x27);//  
HM5065MIPI_write_cmos_sensor(0xA10F,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA110,0x64);//     XRL      A,#0x02
HM5065MIPI_write_cmos_sensor(0xA111,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA112,0x60);//     JZ       B00:A9E1
HM5065MIPI_write_cmos_sensor(0xA113,0x04);//  
HM5065MIPI_write_cmos_sensor(0xA114,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA115,0xB4);//     CJNE     A,#uwDelay1000(0x04);,B00:A9FF
HM5065MIPI_write_cmos_sensor(0xA116,0x04);//  
HM5065MIPI_write_cmos_sensor(0xA117,0x1E);//  
HM5065MIPI_write_cmos_sensor(0xA118,0x90);//     MOV      DPTR,#Av2x2_H_Size(0x4704);
HM5065MIPI_write_cmos_sensor(0xA119,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA11A,0x04);//  
HM5065MIPI_write_cmos_sensor(0xA11B,0x74);//     MOV      A,#0x02
HM5065MIPI_write_cmos_sensor(0xA11C,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA11D,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA11E,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA11F,0x74);//     MOV      A,#TH0(0x8C);
HM5065MIPI_write_cmos_sensor(0xA120,0x8C);//  
HM5065MIPI_write_cmos_sensor(0xA121,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA122,0x90);//     MOV      DPTR,#Av2x2_V_Size(0x470C);
HM5065MIPI_write_cmos_sensor(0xA123,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA124,0x0C);//  
HM5065MIPI_write_cmos_sensor(0xA125,0x74);//     MOV      A,#0x01
HM5065MIPI_write_cmos_sensor(0xA126,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA127,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA128,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA129,0x74);//     MOV      A,#0xEA
HM5065MIPI_write_cmos_sensor(0xA12A,0xEA);//  
HM5065MIPI_write_cmos_sensor(0xA12B,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA12C,0x90);//     MOV      DPTR,#Av2x2_Xscale(0x47A4);
HM5065MIPI_write_cmos_sensor(0xA12D,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA12E,0xA4);//  
HM5065MIPI_write_cmos_sensor(0xA12F,0x74);//     MOV      A,#uwDelay1000(0x04);
HM5065MIPI_write_cmos_sensor(0xA130,0x04);//  
HM5065MIPI_write_cmos_sensor(0xA131,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA132,0x90);//     MOV      DPTR,#Av2x2_Yscale(0x47A8);
HM5065MIPI_write_cmos_sensor(0xA133,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA134,0xA8);//  
HM5065MIPI_write_cmos_sensor(0xA135,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA136,0x22);//     RTN

///////////////////////////////////////////////////////////////////////////////
//BP 5: AV OTP
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA137,0x74);//     MOV      A,#uwDelay1000(0x04);
HM5065MIPI_write_cmos_sensor(0xA138,0x04);//  
HM5065MIPI_write_cmos_sensor(0xA139,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA13A,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA13B,0x74);//     MOV      A,#ZoomPanControl(0x20);
HM5065MIPI_write_cmos_sensor(0xA13C,0x20);//  
HM5065MIPI_write_cmos_sensor(0xA13D,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA13E,0xE4);//     CLR      A
HM5065MIPI_write_cmos_sensor(0xA13F,0xF5);//     MOV      0x22,A
HM5065MIPI_write_cmos_sensor(0xA140,0x22);//  
HM5065MIPI_write_cmos_sensor(0xA141,0xE5);//     MOV      A 22
HM5065MIPI_write_cmos_sensor(0xA142,0x22);//  
HM5065MIPI_write_cmos_sensor(0xA143,0xC3);//     CLR      C
HM5065MIPI_write_cmos_sensor(0xA144,0x94);//     SUBB     A,#PipeSetupBank0(0x40);
HM5065MIPI_write_cmos_sensor(0xA145,0x40);//  
HM5065MIPI_write_cmos_sensor(0xA146,0x40);//     JC       B00:AB81
HM5065MIPI_write_cmos_sensor(0xA147,0x03);//  
HM5065MIPI_write_cmos_sensor(0xA148,0x02);//     LJMP     B00:AC33
HM5065MIPI_write_cmos_sensor(0xA149,0xF1);//  
HM5065MIPI_write_cmos_sensor(0xA14A,0xFD);//  
HM5065MIPI_write_cmos_sensor(0xA14B,0x90);//     MOV      DPTR,#0x0ABA
HM5065MIPI_write_cmos_sensor(0xA14C,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA14D,0xBA);//  
HM5065MIPI_write_cmos_sensor(0xA14E,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA14F,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA150,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA151,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA152,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA153,0xF5);//     MOV      DPL(0x82);,A
HM5065MIPI_write_cmos_sensor(0xA154,0x82);//  
HM5065MIPI_write_cmos_sensor(0xA155,0x8E);//     MOV      DPH(0x83);,R6
HM5065MIPI_write_cmos_sensor(0xA156,0x83);//  
HM5065MIPI_write_cmos_sensor(0xA157,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA158,0x54);//     ANL      A,#0x70
HM5065MIPI_write_cmos_sensor(0xA159,0x70);//  
HM5065MIPI_write_cmos_sensor(0xA15A,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA15B,0xC4);//     SWAP     A
HM5065MIPI_write_cmos_sensor(0xA15C,0x54);//     ANL      A,#0x0F
HM5065MIPI_write_cmos_sensor(0xA15D,0x0F);//  
HM5065MIPI_write_cmos_sensor(0xA15E,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA15F,0x90);//     MOV      DPTR,#0x0ABC
HM5065MIPI_write_cmos_sensor(0xA160,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA161,0xBC);//  
HM5065MIPI_write_cmos_sensor(0xA162,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA163,0xFA);//     MOV      R2,A
HM5065MIPI_write_cmos_sensor(0xA164,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA165,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA166,0xF5);//     MOV      DPL(0x82);,A
HM5065MIPI_write_cmos_sensor(0xA167,0x82);//  
HM5065MIPI_write_cmos_sensor(0xA168,0x8A);//     MOV      DPH(0x83);,R2
HM5065MIPI_write_cmos_sensor(0xA169,0x83);//  
HM5065MIPI_write_cmos_sensor(0xA16A,0xED);//     MOV      A,R5
HM5065MIPI_write_cmos_sensor(0xA16B,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA16C,0x90);//     MOV      DPTR,#0x0ABD
HM5065MIPI_write_cmos_sensor(0xA16D,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA16E,0xBD);//  
HM5065MIPI_write_cmos_sensor(0xA16F,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA170,0x04);//     INC      A
HM5065MIPI_write_cmos_sensor(0xA171,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA172,0x70);//     JNZ      B00:ABB0
HM5065MIPI_write_cmos_sensor(0xA173,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA174,0x90);//     MOV      DPTR,#0x0ABC
HM5065MIPI_write_cmos_sensor(0xA175,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA176,0xBC);//  
HM5065MIPI_write_cmos_sensor(0xA177,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA178,0x04);//     INC      A
HM5065MIPI_write_cmos_sensor(0xA179,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA17A,0x8F);//     MOV      DPL(0x82);,R7
HM5065MIPI_write_cmos_sensor(0xA17B,0x82);//  
HM5065MIPI_write_cmos_sensor(0xA17C,0x8E);//     MOV      DPH(0x83);,R6
HM5065MIPI_write_cmos_sensor(0xA17D,0x83);//  
HM5065MIPI_write_cmos_sensor(0xA17E,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA17F,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA180,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA181,0x90);//     MOV      DPTR,#0x0ABC
HM5065MIPI_write_cmos_sensor(0xA182,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA183,0xBC);//  
HM5065MIPI_write_cmos_sensor(0xA184,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA185,0xFC);//     MOV      R4,A
HM5065MIPI_write_cmos_sensor(0xA186,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA187,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA188,0xF5);//     MOV      DPL(0x82);,A
HM5065MIPI_write_cmos_sensor(0xA189,0x82);//  
HM5065MIPI_write_cmos_sensor(0xA18A,0x8C);//     MOV      DPH(0x83);,R4
HM5065MIPI_write_cmos_sensor(0xA18B,0x83);//  
HM5065MIPI_write_cmos_sensor(0xA18C,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA18D,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA18E,0x90);//     MOV      DPTR,#0x0ABD
HM5065MIPI_write_cmos_sensor(0xA18F,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA190,0xBD);//  
HM5065MIPI_write_cmos_sensor(0xA191,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA192,0x04);//     INC      A
HM5065MIPI_write_cmos_sensor(0xA193,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA194,0x70);//     JNZ      B00:ABD2
HM5065MIPI_write_cmos_sensor(0xA195,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA196,0x90);//     MOV      DPTR,#0x0ABC
HM5065MIPI_write_cmos_sensor(0xA197,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA198,0xBC);//  
HM5065MIPI_write_cmos_sensor(0xA199,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA19A,0x04);//     INC      A
HM5065MIPI_write_cmos_sensor(0xA19B,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA19C,0x90);//     MOV      DPTR,#0x0ABA
HM5065MIPI_write_cmos_sensor(0xA19D,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA19E,0xBA);//  
HM5065MIPI_write_cmos_sensor(0xA19F,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1A0,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA1A1,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA1A2,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1A3,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA1A4,0xF5);//     MOV      DPL(0x82);,A
HM5065MIPI_write_cmos_sensor(0xA1A5,0x82);//  
HM5065MIPI_write_cmos_sensor(0xA1A6,0x8E);//     MOV      DPH(0x83);,R6
HM5065MIPI_write_cmos_sensor(0xA1A7,0x83);//  
HM5065MIPI_write_cmos_sensor(0xA1A8,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1A9,0x54);//     ANL      A,#0x07
HM5065MIPI_write_cmos_sensor(0xA1AA,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA1AB,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA1AC,0x90);//     MOV      DPTR,#0x0ABC
HM5065MIPI_write_cmos_sensor(0xA1AD,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA1AE,0xBC);//  
HM5065MIPI_write_cmos_sensor(0xA1AF,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1B0,0xFA);//     MOV      R2,A
HM5065MIPI_write_cmos_sensor(0xA1B1,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA1B2,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1B3,0xF5);//     MOV      DPL(0x82);,A
HM5065MIPI_write_cmos_sensor(0xA1B4,0x82);//  
HM5065MIPI_write_cmos_sensor(0xA1B5,0x8A);//     MOV      DPH(0x83);,R2
HM5065MIPI_write_cmos_sensor(0xA1B6,0x83);//  
HM5065MIPI_write_cmos_sensor(0xA1B7,0xED);//     MOV      A,R5
HM5065MIPI_write_cmos_sensor(0xA1B8,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA1B9,0x90);//     MOV      DPTR,#0x0ABD
HM5065MIPI_write_cmos_sensor(0xA1BA,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA1BB,0xBD);//  
HM5065MIPI_write_cmos_sensor(0xA1BC,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1BD,0x04);//     INC      A
HM5065MIPI_write_cmos_sensor(0xA1BE,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA1BF,0x70);//     JNZ      B00:ABFD
HM5065MIPI_write_cmos_sensor(0xA1C0,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA1C1,0x90);//     MOV      DPTR,#0x0ABC
HM5065MIPI_write_cmos_sensor(0xA1C2,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA1C3,0xBC);//  
HM5065MIPI_write_cmos_sensor(0xA1C4,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1C5,0x04);//     INC      A
HM5065MIPI_write_cmos_sensor(0xA1C6,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA1C7,0x8F);//     MOV      DPL(0x82);,R7
HM5065MIPI_write_cmos_sensor(0xA1C8,0x82);//  
HM5065MIPI_write_cmos_sensor(0xA1C9,0x8E);//     MOV      DPH(0x83);,R6
HM5065MIPI_write_cmos_sensor(0xA1CA,0x83);//  
HM5065MIPI_write_cmos_sensor(0xA1CB,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA1CC,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA1CD,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1CE,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA1CF,0x90);//     MOV      DPTR,#0x0ABC
HM5065MIPI_write_cmos_sensor(0xA1D0,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA1D1,0xBC);//  
HM5065MIPI_write_cmos_sensor(0xA1D2,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1D3,0xFC);//     MOV      R4,A
HM5065MIPI_write_cmos_sensor(0xA1D4,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA1D5,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1D6,0xF5);//     MOV      DPL(0x82);,A
HM5065MIPI_write_cmos_sensor(0xA1D7,0x82);//  
HM5065MIPI_write_cmos_sensor(0xA1D8,0x8C);//     MOV      DPH(0x83);,R4
HM5065MIPI_write_cmos_sensor(0xA1D9,0x83);//  
HM5065MIPI_write_cmos_sensor(0xA1DA,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA1DB,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA1DC,0x90);//     MOV      DPTR,#0x0ABD
HM5065MIPI_write_cmos_sensor(0xA1DD,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA1DE,0xBD);//  
HM5065MIPI_write_cmos_sensor(0xA1DF,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1E0,0x04);//     INC      A
HM5065MIPI_write_cmos_sensor(0xA1E1,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA1E2,0x70);//     JNZ      B00:AC20
HM5065MIPI_write_cmos_sensor(0xA1E3,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA1E4,0x90);//     MOV      DPTR,#0x0ABC
HM5065MIPI_write_cmos_sensor(0xA1E5,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA1E6,0xBC);//  
HM5065MIPI_write_cmos_sensor(0xA1E7,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1E8,0x04);//     INC      A
HM5065MIPI_write_cmos_sensor(0xA1E9,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA1EA,0x90);//     MOV      DPTR,#0x0ABB
HM5065MIPI_write_cmos_sensor(0xA1EB,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA1EC,0xBB);//  
HM5065MIPI_write_cmos_sensor(0xA1ED,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1EE,0x24);//     ADD      A,#0x03
HM5065MIPI_write_cmos_sensor(0xA1EF,0x03);//  
HM5065MIPI_write_cmos_sensor(0xA1F0,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA1F1,0x90);//     MOV      DPTR,#0x0ABA
HM5065MIPI_write_cmos_sensor(0xA1F2,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA1F3,0xBA);//  
HM5065MIPI_write_cmos_sensor(0xA1F4,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA1F5,0x34);//     ADDC     A,#DeviceParameters(0x00);
HM5065MIPI_write_cmos_sensor(0xA1F6,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA1F7,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA1F8,0x05);//     INC      0x22
HM5065MIPI_write_cmos_sensor(0xA1F9,0x22);//  
HM5065MIPI_write_cmos_sensor(0xA1FA,0x02);//     LJMP     B00:A409
HM5065MIPI_write_cmos_sensor(0xA1FB,0xF1);//  
HM5065MIPI_write_cmos_sensor(0xA1FC,0x41);//  
HM5065MIPI_write_cmos_sensor(0xA1FD,0x90);//     MOV      DPTR,#0x0ABA
HM5065MIPI_write_cmos_sensor(0xA1FE,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA1FF,0xBA);//  
HM5065MIPI_write_cmos_sensor(0xA200,0x74);//     MOV      A,#g_fpPixelCount(0x0E);
HM5065MIPI_write_cmos_sensor(0xA201,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA202,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA203,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA204,0x74);//     MOV      A,#0xDC
HM5065MIPI_write_cmos_sensor(0xA205,0xDC);//  
HM5065MIPI_write_cmos_sensor(0xA206,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA207,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA208,0x74);//     MOV      A,#0x05
HM5065MIPI_write_cmos_sensor(0xA209,0x05);//  
HM5065MIPI_write_cmos_sensor(0xA20A,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA20B,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA20C,0x74);//     MOV      A,#0x61
HM5065MIPI_write_cmos_sensor(0xA20D,0x61);//  
HM5065MIPI_write_cmos_sensor(0xA20E,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA20F,0x90);//     MOV      DPTR,#0x0ABA
HM5065MIPI_write_cmos_sensor(0xA210,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA211,0xBA);//  
HM5065MIPI_write_cmos_sensor(0xA212,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA213,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA214,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA215,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA216,0xAA);//     MOV      R2,uwDelay100(0x06);
HM5065MIPI_write_cmos_sensor(0xA217,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA218,0xF9);//     MOV      R1,A
HM5065MIPI_write_cmos_sensor(0xA219,0x7B);//     MOV      R3,#0x01
HM5065MIPI_write_cmos_sensor(0xA21A,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA21B,0xC0);//     PUSH     0x02
HM5065MIPI_write_cmos_sensor(0xA21C,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA21D,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA21E,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA21F,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA220,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA221,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA222,0xAA);//     MOV      R2,uwDelay100(0x06);
HM5065MIPI_write_cmos_sensor(0xA223,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA224,0xF8);//     MOV      R0,A
HM5065MIPI_write_cmos_sensor(0xA225,0xAC);//     MOV      R4 02
HM5065MIPI_write_cmos_sensor(0xA226,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA227,0x7D);//     MOV      R5,#0x01
HM5065MIPI_write_cmos_sensor(0xA228,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA229,0xD0);//     POP      0x02
HM5065MIPI_write_cmos_sensor(0xA22A,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA22B,0x7E);//     MOV      R6,#DeviceParameters(0x00);
HM5065MIPI_write_cmos_sensor(0xA22C,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA22D,0x7F);//     MOV      R7,#uwDelay1000(0x04);
HM5065MIPI_write_cmos_sensor(0xA22E,0x04);//  
HM5065MIPI_write_cmos_sensor(0xA22F,0x12);//     LCALL    C?COPY517(C:0F6F);
HM5065MIPI_write_cmos_sensor(0xA230,0x0F);//  
HM5065MIPI_write_cmos_sensor(0xA231,0x6F);//  
HM5065MIPI_write_cmos_sensor(0xA232,0x02);//     JUMP    
HM5065MIPI_write_cmos_sensor(0xA233,0x66);//  
HM5065MIPI_write_cmos_sensor(0xA234,0xD9);//  

///////////////////////////////////////////////////////////////////////////////
//BP 12: AF p74 -04 part2
///////////////////////////////////////////////////////////////////////////////// 
HM5065MIPI_write_cmos_sensor(0xA235,0x90);//     MOV      R4 02
HM5065MIPI_write_cmos_sensor(0xA236,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA237,0xD0);//     MOV      R5,#0x01
HM5065MIPI_write_cmos_sensor(0xA238,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA239,0xA2);//     POP      0x02
HM5065MIPI_write_cmos_sensor(0xA23A,0x69);//  

///////////////////////////////////////////////////////////////////////////////
//BP 7: skip AF init part1
///////////////////////////////////////////////////////////////////////////////// 
HM5065MIPI_write_cmos_sensor(0xA240,0x02);//     ANL      A,#0xFD
HM5065MIPI_write_cmos_sensor(0xA241,0x21);//  
HM5065MIPI_write_cmos_sensor(0xA242,0x7F);//     MOVX     @DPTR,A

///////////////////////////////////////////////////////////////////////////////
//BP 8: skip AF init part 2
///////////////////////////////////////////////////////////////////////////////// 
HM5065MIPI_write_cmos_sensor(0xA243,0x02);//     ANL      A,#0xFD
HM5065MIPI_write_cmos_sensor(0xA244,0x21);//  
HM5065MIPI_write_cmos_sensor(0xA245,0xF4);//     MOVX     @DPTR,A

///////////////////////////////////////////////////////////////////////////////
//BP 9:  Stop/Start part1  skip while loop in scaler update
///////////////////////////////////////////////////////////////////////////////// 
HM5065MIPI_write_cmos_sensor(0xA246,0x02);//     ANL      A,#0xFD
HM5065MIPI_write_cmos_sensor(0xA247,0xA6);//  
HM5065MIPI_write_cmos_sensor(0xA248,0x15);//     MOVX     @DPTR,A

///////////////////////////////////////////////////////////////////////////////
//BP 10:  Stop/Start part2  mode maganer
///////////////////////////////////////////////////////////////////////////////// 
HM5065MIPI_write_cmos_sensor(0xA249,0x60);//     JZ       C:29EF
HM5065MIPI_write_cmos_sensor(0xA24A,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA24B,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA24C,0xB4);//     CJNE     A,#0x01,C:29FF
HM5065MIPI_write_cmos_sensor(0xA24D,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA24E,0x16);//  
HM5065MIPI_write_cmos_sensor(0xA24F,0x90);//     MOV      DPTR,#ModeSetup(0x005D);
HM5065MIPI_write_cmos_sensor(0xA250,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA251,0x5D);//  
HM5065MIPI_write_cmos_sensor(0xA252,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA253,0x70);//     JNZ      C:29FF
HM5065MIPI_write_cmos_sensor(0xA254,0x10);//  
HM5065MIPI_write_cmos_sensor(0xA255,0x12);//     LCALL    StreamManager_ResumeStreaming(C:26C8);
HM5065MIPI_write_cmos_sensor(0xA256,0x26);//  
HM5065MIPI_write_cmos_sensor(0xA257,0xC8);//  
HM5065MIPI_write_cmos_sensor(0xA258,0x90);//     MOV      DPTR,#0x0011
HM5065MIPI_write_cmos_sensor(0xA259,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA25A,0x11);//  
HM5065MIPI_write_cmos_sensor(0xA25B,0x74);//     MOV      A,#0x30
HM5065MIPI_write_cmos_sensor(0xA25C,0x30);//  
HM5065MIPI_write_cmos_sensor(0xA25D,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA25E,0x90);//     MOV      DPTR,#fpHighClipForDesiredExposure(0x0010);
HM5065MIPI_write_cmos_sensor(0xA25F,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA260,0x10);//  
HM5065MIPI_write_cmos_sensor(0xA261,0x74);//     MOV      A,#0x01
HM5065MIPI_write_cmos_sensor(0xA262,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA263,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA264,0x22);//     RET      
HM5065MIPI_write_cmos_sensor(0xA265,0x12);//     LCALL    C:25A8
HM5065MIPI_write_cmos_sensor(0xA266,0x25);//  
HM5065MIPI_write_cmos_sensor(0xA267,0xA8);//  
HM5065MIPI_write_cmos_sensor(0xA268,0x02);//       RET     
HM5065MIPI_write_cmos_sensor(0xA269,0x29);//  
HM5065MIPI_write_cmos_sensor(0xA26A,0xFC);//  

///////////////////////////////////////////////////////////////////////////////
// BP 2     CSI ULP enter 
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA26B,0x44);//     ORL      A,#fpHighClipForDesiredExposure(0x10);
HM5065MIPI_write_cmos_sensor(0xA26C,0x18);//  
HM5065MIPI_write_cmos_sensor(0xA26D,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA26E,0x90);//     MOV      DPTR,#Tx_Csi2_Dphy_Datalane2_Pwr_Ctrl(0x7218);
HM5065MIPI_write_cmos_sensor(0xA26F,0x72);//  
HM5065MIPI_write_cmos_sensor(0xA270,0x18);//  
HM5065MIPI_write_cmos_sensor(0xA271,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA272,0x44);//     ORL      A,#fpHighClipForDesiredExposure(0x10);
HM5065MIPI_write_cmos_sensor(0xA273,0x18);//  
HM5065MIPI_write_cmos_sensor(0xA274,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA275,0x00);//     MOV      DPTR,#Tx_Csi2_Dphy_Clklane_Pwr_Ctrl(0x7208);
HM5065MIPI_write_cmos_sensor(0xA276,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA277,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA278,0x00);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA279,0x00);//     ORL      A,#fpHighClipForDesiredExposure(0x10);
HM5065MIPI_write_cmos_sensor(0xA27A,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA27B,0x90);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA27C,0x72);//     MOV      DPTR,#Tx_Csi2_Pwr_Ctrl(0x7214);
HM5065MIPI_write_cmos_sensor(0xA27D,0x08);//  
HM5065MIPI_write_cmos_sensor(0xA27E,0xE0);//  
HM5065MIPI_write_cmos_sensor(0xA27F,0x44);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA280,0x10);//     ANL      A,#0xFD
HM5065MIPI_write_cmos_sensor(0xA281,0xF0);//  
HM5065MIPI_write_cmos_sensor(0xA282,0x90);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA283,0x72);//     MOV      DPTR,#Tx_Csi2_Dphy_Pwr_Ctrl(0x7204);
HM5065MIPI_write_cmos_sensor(0xA284,0x14);//  
HM5065MIPI_write_cmos_sensor(0xA285,0xE0);//  
HM5065MIPI_write_cmos_sensor(0xA286,0x54);//     MOV      A,#0x1F
HM5065MIPI_write_cmos_sensor(0xA287,0xFD);//  
HM5065MIPI_write_cmos_sensor(0xA288,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA289,0x22);//     RET     


///////////////////////////////////////////////////////////////////////////////
// BP 13     AF P74 05   
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA29B,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA29C,0xD3);//     SETB     C
HM5065MIPI_write_cmos_sensor(0xA29D,0x90);//     MOV      DPTR,#0x0791
HM5065MIPI_write_cmos_sensor(0xA29E,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA29F,0x91);//  
HM5065MIPI_write_cmos_sensor(0xA2A0,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2A1,0x94);//     SUBB     A,#0x21
HM5065MIPI_write_cmos_sensor(0xA2A2,0x21);//  
HM5065MIPI_write_cmos_sensor(0xA2A3,0x90);//     MOV      DPTR,#AutoFocusInput(0x0790);
HM5065MIPI_write_cmos_sensor(0xA2A4,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA2A5,0x90);//  
HM5065MIPI_write_cmos_sensor(0xA2A6,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2A7,0x64);//     XRL      A,#PipeSetupCommon(0x80);
HM5065MIPI_write_cmos_sensor(0xA2A8,0x80);//  
HM5065MIPI_write_cmos_sensor(0xA2A9,0x94);//     SUBB     A,#SP(0x81);
HM5065MIPI_write_cmos_sensor(0xA2AA,0x81);//  
HM5065MIPI_write_cmos_sensor(0xA2AB,0x40);//     JC       B01:B152
HM5065MIPI_write_cmos_sensor(0xA2AC,0x08);//  
HM5065MIPI_write_cmos_sensor(0xA2AD,0x90);//     MOV      DPTR,#0x07CB
HM5065MIPI_write_cmos_sensor(0xA2AE,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA2AF,0xCB);//  
HM5065MIPI_write_cmos_sensor(0xA2B0,0x74);//     MOV      A,#0xFF
HM5065MIPI_write_cmos_sensor(0xA2B1,0xFF);//  
HM5065MIPI_write_cmos_sensor(0xA2B2,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA2B3,0x80);//     SJMP     B01:B158
HM5065MIPI_write_cmos_sensor(0xA2B4,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA2B5,0x90);//     MOV      DPTR,#0x07CB
HM5065MIPI_write_cmos_sensor(0xA2B6,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA2B7,0xCB);//  
HM5065MIPI_write_cmos_sensor(0xA2B8,0x74);//     MOV      A,#0x01
HM5065MIPI_write_cmos_sensor(0xA2B9,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA2BA,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA2BB,0x02);//     JUMP
HM5065MIPI_write_cmos_sensor(0xA2BC,0xB5);//  
HM5065MIPI_write_cmos_sensor(0xA2BD,0xC3);//  

///////////////////////////////////////////////////////////////////////////////
// BP 14     AF P74 06   
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA2BE,0x90);//     MOV      DPTR,#0x0834
HM5065MIPI_write_cmos_sensor(0xA2BF,0x08);//  
HM5065MIPI_write_cmos_sensor(0xA2C0,0x34);//  
HM5065MIPI_write_cmos_sensor(0xA2C1,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2C2,0xFC);//     MOV      R4,A
HM5065MIPI_write_cmos_sensor(0xA2C3,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA2C4,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2C5,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA2C6,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA2C7,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2C8,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA2C9,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA2CA,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2CB,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA2CC,0x90);//     MOV      DPTR,#AutoFocusMeasureData(0x07D0);
HM5065MIPI_write_cmos_sensor(0xA2CD,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA2CE,0xD0);//  
HM5065MIPI_write_cmos_sensor(0xA2CF,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2D0,0xF8);//     MOV      R0,A
HM5065MIPI_write_cmos_sensor(0xA2D1,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA2D2,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2D3,0xF9);//     MOV      R1,A
HM5065MIPI_write_cmos_sensor(0xA2D4,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA2D5,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2D6,0xFA);//     MOV      R2,A
HM5065MIPI_write_cmos_sensor(0xA2D7,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA2D8,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2D9,0xFB);//     MOV      R3,A
HM5065MIPI_write_cmos_sensor(0xA2DA,0xD3);//     SETB     C
HM5065MIPI_write_cmos_sensor(0xA2DB,0x12);//     LCALL    C?ULCMP(C:0DAE);
HM5065MIPI_write_cmos_sensor(0xA2DC,0x0D);//  
HM5065MIPI_write_cmos_sensor(0xA2DD,0xAE);//  
HM5065MIPI_write_cmos_sensor(0xA2DE,0x40);//     JC       B01:9FDA
HM5065MIPI_write_cmos_sensor(0xA2DF,0x0B);//  
HM5065MIPI_write_cmos_sensor(0xA2E0,0x12);//     LCALL    HCS_Initialization(B01:B0EF);
HM5065MIPI_write_cmos_sensor(0xA2E1,0xB5);//  
HM5065MIPI_write_cmos_sensor(0xA2E2,0x49);//  
HM5065MIPI_write_cmos_sensor(0xA2E3,0x90);//     MOV      DPTR,#0x07A4
HM5065MIPI_write_cmos_sensor(0xA2E4,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA2E5,0xA4);//  
HM5065MIPI_write_cmos_sensor(0xA2E6,0x74);//     MOV      A,#0x02
HM5065MIPI_write_cmos_sensor(0xA2E7,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA2E8,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA2E9,0x80);//     SJMP     B01:9FE3
HM5065MIPI_write_cmos_sensor(0xA2EA,0x09);//  
HM5065MIPI_write_cmos_sensor(0xA2EB,0x12);//     LCALL    LowFocusMeasureFullSearchInit(B01:B7AE);
HM5065MIPI_write_cmos_sensor(0xA2EC,0xB7);//  
HM5065MIPI_write_cmos_sensor(0xA2ED,0x51);//  
HM5065MIPI_write_cmos_sensor(0xA2EE,0x90);//     MOV      DPTR,#0x07A4
HM5065MIPI_write_cmos_sensor(0xA2EF,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA2F0,0xA4);//  
HM5065MIPI_write_cmos_sensor(0xA2F1,0x74);//     MOV      A,#0x05
HM5065MIPI_write_cmos_sensor(0xA2F2,0x05);//  
HM5065MIPI_write_cmos_sensor(0xA2F3,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA2F4,0x02);//     JUM
HM5065MIPI_write_cmos_sensor(0xA2F5,0xA2);//  
HM5065MIPI_write_cmos_sensor(0xA2F6,0xDA);//  

///////////////////////////////////////////////////////////////////////////////
// BP 16     OTP WB   
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA2F7,0x90);//     MOV      DPTR,#fOTPRed(0x0EE0);
HM5065MIPI_write_cmos_sensor(0xA2F8,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA2F9,0xE0);//  
HM5065MIPI_write_cmos_sensor(0xA2FA,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2FB,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA2FC,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA2FD,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA2FE,0x90);//     MOV      DPTR,#0x02A2
HM5065MIPI_write_cmos_sensor(0xA2FF,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA300,0xA2);//  
HM5065MIPI_write_cmos_sensor(0xA301,0xCD);//     XCH      A,R5
HM5065MIPI_write_cmos_sensor(0xA302,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA303,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA304,0xED);//     MOV      A,R5
HM5065MIPI_write_cmos_sensor(0xA305,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA306,0x90);//     MOV      DPTR,#fOTPBlue(0x0EE2);
HM5065MIPI_write_cmos_sensor(0xA307,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA308,0xE2);//  
HM5065MIPI_write_cmos_sensor(0xA309,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA30A,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA30B,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA30C,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA30D,0x90);//     MOV      DPTR,#0x02A8
HM5065MIPI_write_cmos_sensor(0xA30E,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA30F,0xA8);//  
HM5065MIPI_write_cmos_sensor(0xA310,0xCD);//     XCH      A,R5
HM5065MIPI_write_cmos_sensor(0xA311,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA312,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA313,0xED);//     MOV      A,R5
HM5065MIPI_write_cmos_sensor(0xA314,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA315,0xE4);//     CLR      A
HM5065MIPI_write_cmos_sensor(0xA316,0x90);//     MOV      DPTR,#PresetControl(0x0638);
HM5065MIPI_write_cmos_sensor(0xA317,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA318,0x38);//  
HM5065MIPI_write_cmos_sensor(0xA319,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA31A,0x02);//     JUMP     #676
HM5065MIPI_write_cmos_sensor(0xA31B,0x67);//  
HM5065MIPI_write_cmos_sensor(0xA31C,0x63);//  

///////////////////////////////////////////////////////////////////////////////
// BP 17     OTP DarkCal scaling   
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA31D,0x90);//     MOV      DPTR,#bDarkCalSR(0x0EE8);
HM5065MIPI_write_cmos_sensor(0xA31E,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA31F,0xE8);//  
HM5065MIPI_write_cmos_sensor(0xA320,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA321,0x90);//     MOV      DPTR,#0x0262
HM5065MIPI_write_cmos_sensor(0xA322,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA323,0x62);//  
HM5065MIPI_write_cmos_sensor(0xA324,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA325,0x90);//     MOV      DPTR,#bDarkCalAB4(0x0EE9);
HM5065MIPI_write_cmos_sensor(0xA326,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA327,0xE9);//  
HM5065MIPI_write_cmos_sensor(0xA328,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA329,0x90);//     MOV      DPTR,#0x0263
HM5065MIPI_write_cmos_sensor(0xA32A,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA32B,0x63);//  
HM5065MIPI_write_cmos_sensor(0xA32C,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA32D,0x02);//     JUMP     #676
HM5065MIPI_write_cmos_sensor(0xA32E,0x67);//  
HM5065MIPI_write_cmos_sensor(0xA32F,0x1F);//  

///////////////////////////////////////////////////////////////////////////////
// BP 18     OTP DarkCal HFPN scaling   Replaced
///////////////////////////////////////////////////////////////////////////////
//HM5065MIPI_write_cmos_sensor(0xA330,0x90);//     MOV      DPTR,#bDarkCalHFPN(0x0EE4);
//HM5065MIPI_write_cmos_sensor(0xA331,0x0E);//  
//HM5065MIPI_write_cmos_sensor(0xA332,0xE4);//  
//HM5065MIPI_write_cmos_sensor(0xA333,0xE0);//     MOVX     A,@DPTR
//HM5065MIPI_write_cmos_sensor(0xA334,0x90);//     MOV      DPTR,#0x0266
//HM5065MIPI_write_cmos_sensor(0xA335,0x02);//  
//HM5065MIPI_write_cmos_sensor(0xA336,0x66);//  
//HM5065MIPI_write_cmos_sensor(0xA337,0xF0);//     MOVX     @DPTR,A
//HM5065MIPI_write_cmos_sensor(0xA338,0x02);//     JUMP    
//HM5065MIPI_write_cmos_sensor(0xA339,0x67);//  
//HM5065MIPI_write_cmos_sensor(0xA33A,0xA5);//  

///////////////////////////////////////////////////////////////////////////////
// BP 19     OTP VCM    
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA33B,0x90);//     MOV      DPTR,#INFINITY_OTP(0xE014);
HM5065MIPI_write_cmos_sensor(0xA33C,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA33D,0x14);//  
HM5065MIPI_write_cmos_sensor(0xA33E,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA33F,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA340,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA341,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA342,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA343,0x90);//     MOV      DPTR,#0x06D9
HM5065MIPI_write_cmos_sensor(0xA344,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA345,0xD9);//  
HM5065MIPI_write_cmos_sensor(0xA346,0xEE);//     MOV      A,R6
HM5065MIPI_write_cmos_sensor(0xA347,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA348,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA349,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA34A,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA34B,0x90);//     MOV      DPTR,#DELTA_UP_OTP(0xE018);
HM5065MIPI_write_cmos_sensor(0xA34C,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA34D,0x18);//  
HM5065MIPI_write_cmos_sensor(0xA34E,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA34F,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA350,0x7C);//     MOV      R4,#DeviceParameters(0x00);
HM5065MIPI_write_cmos_sensor(0xA351,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA352,0xC3);//     CLR      C
HM5065MIPI_write_cmos_sensor(0xA353,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA354,0x9D);//     SUBB     A,R5
HM5065MIPI_write_cmos_sensor(0xA355,0xEE);//     MOV      A,R6
HM5065MIPI_write_cmos_sensor(0xA356,0x9C);//     SUBB     A,R4
HM5065MIPI_write_cmos_sensor(0xA357,0x50);//     JNC      C:2067
HM5065MIPI_write_cmos_sensor(0xA358,0x09);//  
HM5065MIPI_write_cmos_sensor(0xA359,0xE4);//     CLR      A
HM5065MIPI_write_cmos_sensor(0xA35A,0x90);//     MOV      DPTR,#0x06D7
HM5065MIPI_write_cmos_sensor(0xA35B,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA35C,0xD7);//  
HM5065MIPI_write_cmos_sensor(0xA35D,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA35E,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA35F,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA360,0x80);//     SJMP     C:207A
HM5065MIPI_write_cmos_sensor(0xA361,0x13);//  
HM5065MIPI_write_cmos_sensor(0xA362,0xC3);//     CLR      C
HM5065MIPI_write_cmos_sensor(0xA363,0x90);//     MOV      DPTR,#0x06DA
HM5065MIPI_write_cmos_sensor(0xA364,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA365,0xDA);//  
HM5065MIPI_write_cmos_sensor(0xA366,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA367,0x9D);//     SUBB     A,R5
HM5065MIPI_write_cmos_sensor(0xA368,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA369,0x90);//     MOV      DPTR,#0x06D9
HM5065MIPI_write_cmos_sensor(0xA36A,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA36B,0xD9);//  
HM5065MIPI_write_cmos_sensor(0xA36C,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA36D,0x9C);//     SUBB     A,R4
HM5065MIPI_write_cmos_sensor(0xA36E,0x90);//     MOV      DPTR,#0x06D7
HM5065MIPI_write_cmos_sensor(0xA36F,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA370,0xD7);//  
HM5065MIPI_write_cmos_sensor(0xA371,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA372,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA373,0xCE);//     XCH      A,R6
HM5065MIPI_write_cmos_sensor(0xA374,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA375,0x90);//     MOV      DPTR,#DELTA_UP_OTP(0xE018);
HM5065MIPI_write_cmos_sensor(0xA376,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA377,0x18);//  
HM5065MIPI_write_cmos_sensor(0xA378,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA379,0xF9);//     MOV      R1,A
HM5065MIPI_write_cmos_sensor(0xA37A,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA37B,0x90);//     MOV      DPTR,#0x06C2
HM5065MIPI_write_cmos_sensor(0xA37C,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA37D,0xC2);//  
HM5065MIPI_write_cmos_sensor(0xA37E,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA37F,0xFC);//     MOV      R4,A
HM5065MIPI_write_cmos_sensor(0xA380,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA381,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA382,0xFD);//     MOV      R5,A
HM5065MIPI_write_cmos_sensor(0xA383,0xC3);//     CLR      C
HM5065MIPI_write_cmos_sensor(0xA384,0x9F);//     SUBB     A,R7
HM5065MIPI_write_cmos_sensor(0xA385,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA386,0xEC);//     MOV      A,R4
HM5065MIPI_write_cmos_sensor(0xA387,0x94);//     SUBB     A,#DeviceParameters(0x00);
HM5065MIPI_write_cmos_sensor(0xA388,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA389,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA38A,0x90);//     MOV      DPTR,#MACRO_OTP(0xE016);
HM5065MIPI_write_cmos_sensor(0xA38B,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA38C,0x16);//  
HM5065MIPI_write_cmos_sensor(0xA38D,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA38E,0xFA);//     MOV      R2,A
HM5065MIPI_write_cmos_sensor(0xA38F,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA390,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA391,0xFB);//     MOV      R3,A
HM5065MIPI_write_cmos_sensor(0xA392,0xD3);//     SETB     C
HM5065MIPI_write_cmos_sensor(0xA393,0x9F);//     SUBB     A,R7
HM5065MIPI_write_cmos_sensor(0xA394,0xEA);//     MOV      A,R2
HM5065MIPI_write_cmos_sensor(0xA395,0x9E);//     SUBB     A,R6
HM5065MIPI_write_cmos_sensor(0xA396,0x40);//     JC       C:20A7
HM5065MIPI_write_cmos_sensor(0xA397,0x0A);//  
HM5065MIPI_write_cmos_sensor(0xA398,0x90);//     MOV      DPTR,#0x06D5
HM5065MIPI_write_cmos_sensor(0xA399,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA39A,0xD5);//  
HM5065MIPI_write_cmos_sensor(0xA39B,0xEC);//     MOV      A,R4
HM5065MIPI_write_cmos_sensor(0xA39C,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA39D,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA39E,0xED);//     MOV      A,R5
HM5065MIPI_write_cmos_sensor(0xA39F,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA3A0,0x80);//     SJMP     C:20B5
HM5065MIPI_write_cmos_sensor(0xA3A1,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA3A2,0xE9);//     MOV      A,R1
HM5065MIPI_write_cmos_sensor(0xA3A3,0x7E);//     MOV      R6,#DeviceParameters(0x00);
HM5065MIPI_write_cmos_sensor(0xA3A4,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA3A5,0x2B);//     ADD      A,R3
HM5065MIPI_write_cmos_sensor(0xA3A6,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA3A7,0xEE);//     MOV      A,R6
HM5065MIPI_write_cmos_sensor(0xA3A8,0x3A);//     ADDC     A,R2
HM5065MIPI_write_cmos_sensor(0xA3A9,0x90);//     MOV      DPTR,#0x06D5
HM5065MIPI_write_cmos_sensor(0xA3AA,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA3AB,0xD5);//  
HM5065MIPI_write_cmos_sensor(0xA3AC,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA3AD,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA3AE,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA3AF,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA3B0,0xE9);//     MOV      A,R1
HM5065MIPI_write_cmos_sensor(0xA3B1,0xFB);//     MOV      R3,A
HM5065MIPI_write_cmos_sensor(0xA3B2,0x7A);//     MOV      R2,#DeviceParameters(0x00);
HM5065MIPI_write_cmos_sensor(0xA3B3,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA3B4,0x90);//     MOV      DPTR,#0xE015
HM5065MIPI_write_cmos_sensor(0xA3B5,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA3B6,0x15);//  
HM5065MIPI_write_cmos_sensor(0xA3B7,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA3B8,0x2B);//     ADD      A,R3
HM5065MIPI_write_cmos_sensor(0xA3B9,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA3BA,0x90);//     MOV      DPTR,#INFINITY_OTP(0xE014);
HM5065MIPI_write_cmos_sensor(0xA3BB,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA3BC,0x14);//  
HM5065MIPI_write_cmos_sensor(0xA3BD,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA3BE,0x3A);//     ADDC     A,R2
HM5065MIPI_write_cmos_sensor(0xA3BF,0x90);//     MOV      DPTR,#0x06E1
HM5065MIPI_write_cmos_sensor(0xA3C0,0x06);//  
HM5065MIPI_write_cmos_sensor(0xA3C1,0xE1);//  
HM5065MIPI_write_cmos_sensor(0xA3C2,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA3C3,0xA3);//     INC      DPTR
HM5065MIPI_write_cmos_sensor(0xA3C4,0xCE);//     XCH      A,R6
HM5065MIPI_write_cmos_sensor(0xA3C5,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA3C6,0xC3);//     CLR      C
HM5065MIPI_write_cmos_sensor(0xA3C7,0x90);//     MOV      DPTR,#0xE017
HM5065MIPI_write_cmos_sensor(0xA3C8,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA3C9,0x17);//  
HM5065MIPI_write_cmos_sensor(0xA3CA,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA3CB,0x9B);//     SUBB     A,R3
HM5065MIPI_write_cmos_sensor(0xA3CC,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA3CD,0x90);//     MOV      DPTR,#MACRO_OTP(0xE016);
HM5065MIPI_write_cmos_sensor(0xA3CE,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA3CF,0x16);//  
HM5065MIPI_write_cmos_sensor(0xA3D0,0x02);//     JUMP    
HM5065MIPI_write_cmos_sensor(0xA3D1,0x20);//  
HM5065MIPI_write_cmos_sensor(0xA3D2,0xD5);//  

///////////////////////////////////////////////////////////////////////////////
// BP 18     OTP DarkCal HFPN scaling   Replaced
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA3D3,0x90);//     MOV      DPTR,#bDarkCalHFPN(0x0EE4);
HM5065MIPI_write_cmos_sensor(0xA3d4,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA3d5,0xE4);//  
HM5065MIPI_write_cmos_sensor(0xA3d6,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA3d7,0x90);//     MOV      DPTR,#0x0266
HM5065MIPI_write_cmos_sensor(0xA3d8,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA3d9,0x66);//  
HM5065MIPI_write_cmos_sensor(0xA3da,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA3DB,0x90);//     MOV      DPTR,#bDarkCalHFPN(0x0EE4);
HM5065MIPI_write_cmos_sensor(0xA3dc,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA3dd,0xE5);//  
HM5065MIPI_write_cmos_sensor(0xA3de,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA3df,0x90);//     MOV      DPTR,#0x0266
HM5065MIPI_write_cmos_sensor(0xA3e0,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA3e1,0x64);//  
HM5065MIPI_write_cmos_sensor(0xA3e2,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA3e3,0x90);//     MOV      DPTR,#bDarkCalHFPN(0x0EE4);
HM5065MIPI_write_cmos_sensor(0xA3e4,0x0E);//  
HM5065MIPI_write_cmos_sensor(0xA3e5,0xE6);//  
HM5065MIPI_write_cmos_sensor(0xA3e6,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA3e7,0x90);//     MOV      DPTR,#0x0266
HM5065MIPI_write_cmos_sensor(0xA3e8,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA3e9,0x65);//  
HM5065MIPI_write_cmos_sensor(0xA3ea,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA3eb,0x02);//     JUMP    
HM5065MIPI_write_cmos_sensor(0xA3ec,0x67);//  
HM5065MIPI_write_cmos_sensor(0xA3ed,0xA5);//  

///////////////////////////////////////////////////////////////////////////////
// BP 20     TWakeup work around for PLL3 div
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA3f0,0x12);//  
HM5065MIPI_write_cmos_sensor(0xA3f1,0x47);//  
HM5065MIPI_write_cmos_sensor(0xA3f2,0x59);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA3f3,0x90);//     MOV      DPTR,#bDarkCalHFPN(0x0EE4);
HM5065MIPI_write_cmos_sensor(0xA3f4,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA3f5,0xB5);//  
HM5065MIPI_write_cmos_sensor(0xA3f6,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA3f7,0xB4);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA3f8,0x02);//     MOV      DPTR,#0x0266
HM5065MIPI_write_cmos_sensor(0xA3f9,0x03);//  
HM5065MIPI_write_cmos_sensor(0xA3fa,0x12);//  
HM5065MIPI_write_cmos_sensor(0xA3fb,0x47);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA3fc,0x59);//     JUMP    
HM5065MIPI_write_cmos_sensor(0xA3fd,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA3fe,0xC5);//  
HM5065MIPI_write_cmos_sensor(0xA3ff,0xC3);//  

///////////////////////////////////////////////////////////////////////////////
// BP 23     AV 2x2
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA400,0x90);//     MOV      DPTR,#c_HFlip(0x003D);
HM5065MIPI_write_cmos_sensor(0xA401,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA402,0x3D);//  
HM5065MIPI_write_cmos_sensor(0xA403,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA404,0x90);//     MOV      DPTR,#0x0084
HM5065MIPI_write_cmos_sensor(0xA405,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA406,0x84);//  
HM5065MIPI_write_cmos_sensor(0xA407,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA408,0xFE);//     MOV      R6,A
HM5065MIPI_write_cmos_sensor(0xA409,0x90);//     MOV      DPTR,#c_VFlip(0x003E);
HM5065MIPI_write_cmos_sensor(0xA40A,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA40B,0x3E);//  
HM5065MIPI_write_cmos_sensor(0xA40C,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA40D,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA40E,0x70);//     JNZ      B00:8201
HM5065MIPI_write_cmos_sensor(0xA40F,0x03);//  
HM5065MIPI_write_cmos_sensor(0xA410,0xEE);//     MOV      A,R6
HM5065MIPI_write_cmos_sensor(0xA411,0x60);//     JZ       B00:8205
HM5065MIPI_write_cmos_sensor(0xA412,0x04);//  
HM5065MIPI_write_cmos_sensor(0xA413,0x7F);//     MOV      R7,#0x01
HM5065MIPI_write_cmos_sensor(0xA414,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA415,0x80);//     SJMP     B00:8207
HM5065MIPI_write_cmos_sensor(0xA416,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA417,0x7F);//     MOV      R7,#DeviceParameters(0x00);
HM5065MIPI_write_cmos_sensor(0xA418,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA419,0x90);//     MOV      DPTR,#c_HVFlip(0x003F);
HM5065MIPI_write_cmos_sensor(0xA41A,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA41B,0x3F);//  
HM5065MIPI_write_cmos_sensor(0xA41C,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA41D,0xF0);//     MOVX     @DPTR,A
HM5065MIPI_write_cmos_sensor(0xA41E,0x02);//     JUMP bac
HM5065MIPI_write_cmos_sensor(0xA41F,0x89);//  
HM5065MIPI_write_cmos_sensor(0xA420,0xD3);//  

///////////////////////////////////////////////////////////////////////////////
// BP 24    CSI DL JPEG clk /2
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA421,0x90);//     MOV      DPTR,#uwI2CSIndex(0x0012);
HM5065MIPI_write_cmos_sensor(0xA422,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA423,0x12);//  
HM5065MIPI_write_cmos_sensor(0xA424,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA425,0xFF);//     MOV      R7,A
HM5065MIPI_write_cmos_sensor(0xA426,0x70);//     JNZ      B00:9AC3
HM5065MIPI_write_cmos_sensor(0xA427,0x0C);//  
HM5065MIPI_write_cmos_sensor(0xA428,0x90);//     MOV      DPTR,#0x0046
HM5065MIPI_write_cmos_sensor(0xA429,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA42A,0x46);//  
HM5065MIPI_write_cmos_sensor(0xA42B,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA42C,0xC3);//     CLR      C
HM5065MIPI_write_cmos_sensor(0xA42D,0x94);//     SUBB     A,#0x07
HM5065MIPI_write_cmos_sensor(0xA42E,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA42F,0x40);//     JC       B00:9AC3
HM5065MIPI_write_cmos_sensor(0xA430,0x03);//  
HM5065MIPI_write_cmos_sensor(0xA431,0x75);//     MOV      0x2E,#0x02
HM5065MIPI_write_cmos_sensor(0xA432,0x2E);//  
HM5065MIPI_write_cmos_sensor(0xA433,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA434,0xEF);//     MOV      A,R7
HM5065MIPI_write_cmos_sensor(0xA435,0xB4);//     CJNE     A,#0x01,B00:9AD3
HM5065MIPI_write_cmos_sensor(0xA436,0x01);//  
HM5065MIPI_write_cmos_sensor(0xA437,0x0C);//  
HM5065MIPI_write_cmos_sensor(0xA438,0x90);//     MOV      DPTR,#XDroop_Reverse_Croping(0x0066);
HM5065MIPI_write_cmos_sensor(0xA439,0x00);//  
HM5065MIPI_write_cmos_sensor(0xA43A,0x66);//  
HM5065MIPI_write_cmos_sensor(0xA43B,0xE0);//     MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA43C,0xC3);//     CLR      C
HM5065MIPI_write_cmos_sensor(0xA43D,0x94);//     SUBB     A,#0x07
HM5065MIPI_write_cmos_sensor(0xA43E,0x07);//  
HM5065MIPI_write_cmos_sensor(0xA43F,0x40);//     JC       B00:9AD3
HM5065MIPI_write_cmos_sensor(0xA440,0x03);//  
HM5065MIPI_write_cmos_sensor(0xA441,0x75);//     MOV      0x2E,#0x02
HM5065MIPI_write_cmos_sensor(0xA442,0x2E);//  
HM5065MIPI_write_cmos_sensor(0xA443,0x02);//  
HM5065MIPI_write_cmos_sensor(0xA444,0x02);//     JUMP   
HM5065MIPI_write_cmos_sensor(0xA445,0xA7);//  
HM5065MIPI_write_cmos_sensor(0xA446,0x9E);//  

///////////////////////////////////////////////////////////////////////////////
// BP  26    freeze white balance at low stat <0x4480 10
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA447,0xC3);//    CLR      C
HM5065MIPI_write_cmos_sensor(0xA448,0x90);//    MOV      DPTR,#0x0B8F
HM5065MIPI_write_cmos_sensor(0xA449,0x0B);// 
HM5065MIPI_write_cmos_sensor(0xA44A,0x8F);// 
HM5065MIPI_write_cmos_sensor(0xA44B,0xE0);//    MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA44C,0x94);//    SUBB     A,#PipeSetupCommon(0x80);
HM5065MIPI_write_cmos_sensor(0xA44D,0x00);// 
HM5065MIPI_write_cmos_sensor(0xA44E,0x90);//    MOV      DPTR,#CalculateNormalisedStatistics?BYTE(0x0B8E);
HM5065MIPI_write_cmos_sensor(0xA44F,0x0B);// 
HM5065MIPI_write_cmos_sensor(0xA450,0x8E);// 
HM5065MIPI_write_cmos_sensor(0xA451,0xE0);//    MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA452,0x94);//    SUBB     A,#0x44
HM5065MIPI_write_cmos_sensor(0xA453,0x41);// 
HM5065MIPI_write_cmos_sensor(0xA454,0x40);//    JC       B00:827D
HM5065MIPI_write_cmos_sensor(0xA455,0x22);// 
HM5065MIPI_write_cmos_sensor(0xA456,0x90);//    MOV      DPTR,#0x0B91
HM5065MIPI_write_cmos_sensor(0xA457,0x0B);// 
HM5065MIPI_write_cmos_sensor(0xA458,0x91);// 
HM5065MIPI_write_cmos_sensor(0xA459,0xE0);//    MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA45A,0x94);//    SUBB     A,#PipeSetupCommon(0x80);
HM5065MIPI_write_cmos_sensor(0xA45B,0x00);// 
HM5065MIPI_write_cmos_sensor(0xA45C,0x90);//    MOV      DPTR,#0x0B90
HM5065MIPI_write_cmos_sensor(0xA45D,0x0B);// 
HM5065MIPI_write_cmos_sensor(0xA45E,0x90);// 
HM5065MIPI_write_cmos_sensor(0xA45F,0xE0);//    MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA460,0x94);//    SUBB     A,#0x44
HM5065MIPI_write_cmos_sensor(0xA461,0x41);// 
HM5065MIPI_write_cmos_sensor(0xA462,0x40);//    JC       B00:827D
HM5065MIPI_write_cmos_sensor(0xA463,0x14);// 
HM5065MIPI_write_cmos_sensor(0xA464,0x90);//    MOV      DPTR,#0x0B93
HM5065MIPI_write_cmos_sensor(0xA465,0x0B);// 
HM5065MIPI_write_cmos_sensor(0xA466,0x93);// 
HM5065MIPI_write_cmos_sensor(0xA467,0xE0);//    MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA468,0x94);//    SUBB     A,#PipeSetupCommon(0x80);
HM5065MIPI_write_cmos_sensor(0xA469,0x00);// 
HM5065MIPI_write_cmos_sensor(0xA46A,0x90);//    MOV      DPTR,#0x0B92
HM5065MIPI_write_cmos_sensor(0xA46B,0x0B);// 
HM5065MIPI_write_cmos_sensor(0xA46C,0x92);// 
HM5065MIPI_write_cmos_sensor(0xA46D,0xE0);//    MOVX     A,@DPTR
HM5065MIPI_write_cmos_sensor(0xA46E,0x94);//    SUBB     A,#0x44
HM5065MIPI_write_cmos_sensor(0xA46F,0x41);// 
HM5065MIPI_write_cmos_sensor(0xA470,0x40);//    JC       B00:827D
HM5065MIPI_write_cmos_sensor(0xA471,0x06);// 
HM5065MIPI_write_cmos_sensor(0xA472,0x90);//    MOV      DPTR,#0x01A4
HM5065MIPI_write_cmos_sensor(0xA473,0x01);// 
HM5065MIPI_write_cmos_sensor(0xA474,0xA4);// 
HM5065MIPI_write_cmos_sensor(0xA475,0x02);//    LJMP     back
HM5065MIPI_write_cmos_sensor(0xA476,0x86);// 
HM5065MIPI_write_cmos_sensor(0xA477,0x57);// 
HM5065MIPI_write_cmos_sensor(0xA478,0x02);//    LJMP     back
HM5065MIPI_write_cmos_sensor(0xA479,0x86);// 
HM5065MIPI_write_cmos_sensor(0xA47A,0x5C);//                                                                

///////////////////////////////////////////////////////////////////////////////
// BP 11     AF light detector   using Xdata 0x0DF6/7  
///////////////////////////////////////////////////////////////////////////////
HM5065MIPI_write_cmos_sensor(0xA500,0xF5);//      //    MOV      c_HeightScale(0x3B);,A                              
HM5065MIPI_write_cmos_sensor(0xA501,0x3B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA502,0x90);//      //    MOV      DPTR,#0x066C                                       
HM5065MIPI_write_cmos_sensor(0xA503,0x06);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA504,0x6C);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA505,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA506,0xFF);//      //    MOV      R7,A                                               
HM5065MIPI_write_cmos_sensor(0xA507,0xE5);//      //    MOV      A,c_HeightScale(0x3B);                              
HM5065MIPI_write_cmos_sensor(0xA508,0x3B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA509,0xC3);//      //    CLR      C                                                  
HM5065MIPI_write_cmos_sensor(0xA50A,0x9F);//      //    SUBB     A,R7                                               
HM5065MIPI_write_cmos_sensor(0xA50B,0x40);//      //    JC       B01:98E8                                           
HM5065MIPI_write_cmos_sensor(0xA50C,0x03);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA50D,0x02);//      //    LJMP     B01:99E6                                           
HM5065MIPI_write_cmos_sensor(0xA50E,0xF6);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA50F,0x0E);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA510,0x90);//      //    MOV      DPTR,#0x0BC6                                       
HM5065MIPI_write_cmos_sensor(0xA511,0x0B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA512,0xC6);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA513,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA514,0x14);//      //    DEC      A                                                  
HM5065MIPI_write_cmos_sensor(0xA515,0x60);//      //    JZ       B01:992B                                           
HM5065MIPI_write_cmos_sensor(0xA516,0x3C);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA517,0x14);//      //    DEC      A                                                  
HM5065MIPI_write_cmos_sensor(0xA518,0x60);//      //    JZ       B01:995D                                           
HM5065MIPI_write_cmos_sensor(0xA519,0x6B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA51A,0x24);//      //    ADD      A,#0x02                                            
HM5065MIPI_write_cmos_sensor(0xA51B,0x02);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA51C,0x60);//      //    JZ       B01:98F9                                           
HM5065MIPI_write_cmos_sensor(0xA51D,0x03);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA51E,0x02);//      //    LJMP     B01:998D                                           
HM5065MIPI_write_cmos_sensor(0xA51F,0xF5);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA520,0xB5);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA521,0x90);//      //    MOV      DPTR,#AutoFocusInstableFocusMeasureValues(0x0A9A);  
HM5065MIPI_write_cmos_sensor(0xA522,0x0A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA523,0x9A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA524,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA525,0xFB);//      //    MOV      R3,A                                               
HM5065MIPI_write_cmos_sensor(0xA526,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA527,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA528,0xFA);//      //    MOV      R2,A                                               
HM5065MIPI_write_cmos_sensor(0xA529,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA52A,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA52B,0xF9);//      //    MOV      R1,A                                               
HM5065MIPI_write_cmos_sensor(0xA52C,0x85);//      //    MOV      DPL(0x82);,c_HeightScale(0x3B);                      
HM5065MIPI_write_cmos_sensor(0xA52D,0x3B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA52E,0x82);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA52F,0x75);//      //    MOV      DPH(0x83);,#DeviceParameters(0x00);                  
HM5065MIPI_write_cmos_sensor(0xA530,0x83);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA531,0x00);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA532,0x12);//      //    LCALL    C?CLDOPTR(C:0AB8);                                  
HM5065MIPI_write_cmos_sensor(0xA533,0x0A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA534,0xB8);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA535,0xFF);//      //    MOV      R7,A                                               
HM5065MIPI_write_cmos_sensor(0xA536,0x74);//      //    MOV      A,#0xAB                                            
HM5065MIPI_write_cmos_sensor(0xA537,0xAB);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA538,0x25);//      //    ADD      A,c_HeightScale(0x3B);                              
HM5065MIPI_write_cmos_sensor(0xA539,0x3B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA53A,0xF5);//      //    MOV      DPL(0x82);,A                                        
HM5065MIPI_write_cmos_sensor(0xA53B,0x82);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA53C,0xE4);//      //    CLR      A                                                  
HM5065MIPI_write_cmos_sensor(0xA53D,0x34);//      //    ADDC     A,#bInt_Event_Status(0x0A);                         
HM5065MIPI_write_cmos_sensor(0xA53E,0x0A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA53F,0xF5);//      //    MOV      DPH(0x83);,A                                        
HM5065MIPI_write_cmos_sensor(0xA540,0x83);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA541,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA542,0xFD);//      //    MOV      R5,A                                               
HM5065MIPI_write_cmos_sensor(0xA543,0xC3);//      //    CLR      C                                                  
HM5065MIPI_write_cmos_sensor(0xA544,0xEF);//      //    MOV      A,R7                                               
HM5065MIPI_write_cmos_sensor(0xA545,0x9D);//      //    SUBB     A,R5                                               
HM5065MIPI_write_cmos_sensor(0xA546,0xFE);//      //    MOV      R6,A                                               
HM5065MIPI_write_cmos_sensor(0xA547,0xE4);//      //    CLR      A                                                  
HM5065MIPI_write_cmos_sensor(0xA548,0x94);//      //    SUBB     A,#DeviceParameters(0x00);                          
HM5065MIPI_write_cmos_sensor(0xA549,0x00);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA54A,0x90);//      //    MOV      DPTR,#0x0BCA                                       
HM5065MIPI_write_cmos_sensor(0xA54B,0x0B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA54C,0xCA);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA54D,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA54E,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA54F,0xCE);//      //    XCH      A,R6                                               
HM5065MIPI_write_cmos_sensor(0xA550,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA551,0x80);//      //    SJMP     B01:998D                                           
HM5065MIPI_write_cmos_sensor(0xA552,0x62);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA553,0x90);//      //    MOV      DPTR,#AutoFocusInstableFocusMeasureValues(0x0A9A);  
HM5065MIPI_write_cmos_sensor(0xA554,0x0A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA555,0x9A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA556,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA557,0xFB);//      //    MOV      R3,A                                               
HM5065MIPI_write_cmos_sensor(0xA558,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA559,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA55A,0xFA);//      //    MOV      R2,A                                               
HM5065MIPI_write_cmos_sensor(0xA55B,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA55C,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA55D,0xF9);//      //    MOV      R1,A                                               
HM5065MIPI_write_cmos_sensor(0xA55E,0x85);//      //    MOV      DPL(0x82);,c_HeightScale(0x3B);                      
HM5065MIPI_write_cmos_sensor(0xA55F,0x3B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA560,0x82);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA561,0x75);//      //    MOV      DPH(0x83);,#DeviceParameters(0x00);                  
HM5065MIPI_write_cmos_sensor(0xA562,0x83);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA563,0x00);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA564,0x12);//      //    LCALL    C?CLDOPTR(C:0AB8);                                  
HM5065MIPI_write_cmos_sensor(0xA565,0x0A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA566,0xB8);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA567,0xFF);//      //    MOV      R7,A                                               
HM5065MIPI_write_cmos_sensor(0xA568,0x74);//      //    MOV      A,#0x9D                                            
HM5065MIPI_write_cmos_sensor(0xA569,0x9D);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA56A,0x25);//      //    ADD      A,c_HeightScale(0x3B);                              
HM5065MIPI_write_cmos_sensor(0xA56B,0x3B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA56C,0xF5);//      //    MOV      DPL(0x82);,A                                        
HM5065MIPI_write_cmos_sensor(0xA56D,0x82);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA56E,0xE4);//      //    CLR      A                                                  
HM5065MIPI_write_cmos_sensor(0xA56F,0x34);//      //    ADDC     A,#bInt_Event_Status(0x0A);                         
HM5065MIPI_write_cmos_sensor(0xA570,0x0A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA571,0xF5);//      //    MOV      DPH(0x83);,A                                        
HM5065MIPI_write_cmos_sensor(0xA572,0x83);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA573,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA574,0xFD);//      //    MOV      R5,A                                               
HM5065MIPI_write_cmos_sensor(0xA575,0xC3);//      //    CLR      C                                                  
HM5065MIPI_write_cmos_sensor(0xA576,0xEF);//      //    MOV      A,R7                                               
HM5065MIPI_write_cmos_sensor(0xA577,0x9D);//      //    SUBB     A,R5                                               
HM5065MIPI_write_cmos_sensor(0xA578,0xFE);//      //    MOV      R6,A                                               
HM5065MIPI_write_cmos_sensor(0xA579,0xE4);//      //    CLR      A                                                  
HM5065MIPI_write_cmos_sensor(0xA57A,0x94);//      //    SUBB     A,#DeviceParameters(0x00);                          
HM5065MIPI_write_cmos_sensor(0xA57B,0x00);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA57C,0x90);//      //    MOV      DPTR,#0x0BCA                                       
HM5065MIPI_write_cmos_sensor(0xA57D,0x0B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA57E,0xCA);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA57F,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA580,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA581,0xCE);//      //    XCH      A,R6                                               
HM5065MIPI_write_cmos_sensor(0xA582,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA583,0x80);//      //    SJMP     B01:998D                                           
HM5065MIPI_write_cmos_sensor(0xA584,0x30);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA585,0x90);//      //    MOV      DPTR,#AutoFocusInstableFocusMeasureValues(0x0A9A);  
HM5065MIPI_write_cmos_sensor(0xA586,0x0A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA587,0x9A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA588,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA589,0xFB);//      //    MOV      R3,A                                               
HM5065MIPI_write_cmos_sensor(0xA58A,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA58B,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA58C,0xFA);//      //    MOV      R2,A                                               
HM5065MIPI_write_cmos_sensor(0xA58D,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA58E,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA58F,0xF9);//      //    MOV      R1,A                                               
HM5065MIPI_write_cmos_sensor(0xA590,0x85);//      //    MOV      DPL(0x82);,c_HeightScale(0x3B);                      
HM5065MIPI_write_cmos_sensor(0xA591,0x3B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA592,0x82);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA593,0x75);//      //    MOV      DPH(0x83);,#DeviceParameters(0x00);                  
HM5065MIPI_write_cmos_sensor(0xA594,0x83);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA595,0x00);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA596,0x12);//      //    LCALL    C?CLDOPTR(C:0AB8);                                  
HM5065MIPI_write_cmos_sensor(0xA597,0x0A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA598,0xB8);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA599,0xFF);//      //    MOV      R7,A                                               
HM5065MIPI_write_cmos_sensor(0xA59A,0x74);//      //    MOV      A,#0xA4                                            
HM5065MIPI_write_cmos_sensor(0xA59B,0xA4);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA59C,0x25);//      //    ADD      A,c_HeightScale(0x3B);                              
HM5065MIPI_write_cmos_sensor(0xA59D,0x3B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA59E,0xF5);//      //    MOV      DPL(0x82);,A                                        
HM5065MIPI_write_cmos_sensor(0xA59F,0x82);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5A0,0xE4);//      //    CLR      A                                                  
HM5065MIPI_write_cmos_sensor(0xA5A1,0x34);//      //    ADDC     A,#bInt_Event_Status(0x0A);                         
HM5065MIPI_write_cmos_sensor(0xA5A2,0x0A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5A3,0xF5);//      //    MOV      DPH(0x83);,A                                        
HM5065MIPI_write_cmos_sensor(0xA5A4,0x83);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5A5,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA5A6,0xFD);//      //    MOV      R5,A                                               
HM5065MIPI_write_cmos_sensor(0xA5A7,0xC3);//      //    CLR      C                                                  
HM5065MIPI_write_cmos_sensor(0xA5A8,0xEF);//      //    MOV      A,R7                                               
HM5065MIPI_write_cmos_sensor(0xA5A9,0x9D);//      //    SUBB     A,R5                                               
HM5065MIPI_write_cmos_sensor(0xA5AA,0xFE);//      //    MOV      R6,A                                               
HM5065MIPI_write_cmos_sensor(0xA5AB,0xE4);//      //    CLR      A                                                  
HM5065MIPI_write_cmos_sensor(0xA5AC,0x94);//      //    SUBB     A,#DeviceParameters(0x00);                          
HM5065MIPI_write_cmos_sensor(0xA5AD,0x00);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5AE,0x90);//      //    MOV      DPTR,#0x0BCA                                       
HM5065MIPI_write_cmos_sensor(0xA5AF,0x0B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5B0,0xCA);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5B1,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA5B2,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA5B3,0xCE);//      //    XCH      A,R6                                               
HM5065MIPI_write_cmos_sensor(0xA5B4,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA5B5,0x90);//      //    MOV      DPTR,#0x0783                                       
HM5065MIPI_write_cmos_sensor(0xA5B6,0x07);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5B7,0x83);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5B8,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA5B9,0xFF);//      //    MOV      R7,A                                               
HM5065MIPI_write_cmos_sensor(0xA5BA,0x7E);//      //    MOV      R6,#DeviceParameters(0x00);                         
HM5065MIPI_write_cmos_sensor(0xA5BB,0x00);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5BC,0x90);//      //    MOV      DPTR,#patch_wLightGap(0x0DF6);                      
HM5065MIPI_write_cmos_sensor(0xA5BD,0x0D);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5BE,0xF6);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5BF,0xEE);//      //    MOV      A,R6                                               
HM5065MIPI_write_cmos_sensor(0xA5C0,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA5C1,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA5C2,0xEF);//      //    MOV      A,R7                                               
HM5065MIPI_write_cmos_sensor(0xA5C3,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA5C4,0x90);//      //    MOV      DPTR,#0x0BCA                                       
HM5065MIPI_write_cmos_sensor(0xA5C5,0x0B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5C6,0xCA);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5C7,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA5C8,0xFC);//      //    MOV      R4,A                                               
HM5065MIPI_write_cmos_sensor(0xA5C9,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA5CA,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA5CB,0xFD);//      //    MOV      R5,A                                               
HM5065MIPI_write_cmos_sensor(0xA5CC,0xD3);//      //    SETB     C                                                  
HM5065MIPI_write_cmos_sensor(0xA5CD,0x9F);//      //    SUBB     A,R7                                               
HM5065MIPI_write_cmos_sensor(0xA5CE,0x74);//      //    MOV      A,#PipeSetupCommon(0x80);                           
HM5065MIPI_write_cmos_sensor(0xA5CF,0x80);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5D0,0xF8);//      //    MOV      R0,A                                               
HM5065MIPI_write_cmos_sensor(0xA5D1,0xEC);//      //    MOV      A,R4                                               
HM5065MIPI_write_cmos_sensor(0xA5D2,0x64);//      //    XRL      A,#PipeSetupCommon(0x80);                           
HM5065MIPI_write_cmos_sensor(0xA5D3,0x80);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5D4,0x98);//      //    SUBB     A,R0                                               
HM5065MIPI_write_cmos_sensor(0xA5D5,0x40);//      //    JC       B01:99BB                                           
HM5065MIPI_write_cmos_sensor(0xA5D6,0x0C);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5D7,0x90);//      //    MOV      DPTR,#0x0BC8                                       
HM5065MIPI_write_cmos_sensor(0xA5D8,0x0B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5D9,0xC8);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5DA,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA5DB,0x04);//      //    INC      A                                                  
HM5065MIPI_write_cmos_sensor(0xA5DC,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA5DD,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA5DE,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA5DF,0x04);//      //    INC      A                                                  
HM5065MIPI_write_cmos_sensor(0xA5E0,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA5E1,0x80);//      //    SJMP     B01:99E1                                           
HM5065MIPI_write_cmos_sensor(0xA5E2,0x26);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5E3,0x90);//      //    MOV      DPTR,#patch_wLightGap(0x0DF6);                      
HM5065MIPI_write_cmos_sensor(0xA5E4,0x0D);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5E5,0xF6);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5E6,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA5E7,0xFE);//      //    MOV      R6,A                                               
HM5065MIPI_write_cmos_sensor(0xA5E8,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA5E9,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA5EA,0xFF);//      //    MOV      R7,A                                               
HM5065MIPI_write_cmos_sensor(0xA5EB,0xC3);//      //    CLR      C                                                  
HM5065MIPI_write_cmos_sensor(0xA5EC,0xE4);//      //    CLR      A                                                  
HM5065MIPI_write_cmos_sensor(0xA5ED,0x9F);//      //    SUBB     A,R7                                               
HM5065MIPI_write_cmos_sensor(0xA5EE,0xFF);//      //    MOV      R7,A                                               
HM5065MIPI_write_cmos_sensor(0xA5EF,0xE4);//      //    CLR      A                                                  
HM5065MIPI_write_cmos_sensor(0xA5F0,0x9E);//      //    SUBB     A,R6                                               
HM5065MIPI_write_cmos_sensor(0xA5F1,0xFE);//      //    MOV      R6,A                                               
HM5065MIPI_write_cmos_sensor(0xA5F2,0xC3);//      //    CLR      C                                                  
HM5065MIPI_write_cmos_sensor(0xA5F3,0xED);//      //    MOV      A,R5                                               
HM5065MIPI_write_cmos_sensor(0xA5F4,0x9F);//      //    SUBB     A,R7                                               
HM5065MIPI_write_cmos_sensor(0xA5F5,0xEE);//      //    MOV      A,R6                                               
HM5065MIPI_write_cmos_sensor(0xA5F6,0x64);//      //    XRL      A,#PipeSetupCommon(0x80);                           
HM5065MIPI_write_cmos_sensor(0xA5F7,0x80);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5F8,0xF8);//      //    MOV      R0,A                                               
HM5065MIPI_write_cmos_sensor(0xA5F9,0xEC);//      //    MOV      A,R4                                               
HM5065MIPI_write_cmos_sensor(0xA5FA,0x64);//      //    XRL      A,#PipeSetupCommon(0x80);                           
HM5065MIPI_write_cmos_sensor(0xA5FB,0x80);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5FC,0x98);//      //    SUBB     A,R0                                               
HM5065MIPI_write_cmos_sensor(0xA5FD,0x50);//      //    JNC      B01:99E1                                           
HM5065MIPI_write_cmos_sensor(0xA5FE,0x0A);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA5FF,0x90);//      //    MOV      DPTR,#0x0BC8                                       
HM5065MIPI_write_cmos_sensor(0xA600,0x0B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA601,0xC8);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA602,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA603,0x14);//      //    DEC      A                                                  
HM5065MIPI_write_cmos_sensor(0xA604,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA605,0xA3);//      //    INC      DPTR                                               
HM5065MIPI_write_cmos_sensor(0xA606,0xE0);//      //    MOVX     A,@DPTR                                            
HM5065MIPI_write_cmos_sensor(0xA607,0x04);//      //    INC      A                                                  
HM5065MIPI_write_cmos_sensor(0xA608,0xF0);//      //    MOVX     @DPTR,A                                            
HM5065MIPI_write_cmos_sensor(0xA609,0x05);//      //    INC      c_HeightScale(0x3B);                                
HM5065MIPI_write_cmos_sensor(0xA60A,0x3B);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA60B,0x02);//      //    LJMP     B01:98DA                                           
HM5065MIPI_write_cmos_sensor(0xA60C,0xF5);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA60D,0x02);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA60E,0x90);//      //    MOV      DPTR,#AutoFocusInstableFocusMeasureStatus(0x0858);  
HM5065MIPI_write_cmos_sensor(0xA60F,0x08);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA610,0x58);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA611,0x02);//      //    LJMP                                                        
HM5065MIPI_write_cmos_sensor(0xA612,0x9D);//      //                                                                
HM5065MIPI_write_cmos_sensor(0xA613,0x50);//      //                                                                
   

///////////////////////////////////////////////////////////////////////////////
//Patch Entries
///////////////////////////////////////////////////////////////////////////////
// Breakpoint table entry 0     Contrast
HM5065MIPI_write_cmos_sensor(0x9006,0xBA);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x9007,0x75);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9008,0x00);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9009,0x00);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x900A,0x02);//  Enable BP 0//

// Breakpoint table entry 1     AF p74_04  part 1 
HM5065MIPI_write_cmos_sensor(0x900D,0x01);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x900E,0xA2);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x900F,0x8F);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9010,0x00);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9011,0xCB);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x9012,0x03);//  Enable BP 1//

// Breakpoint table entry 2     CSI ULP enter
HM5065MIPI_write_cmos_sensor(0x9016,0xE6);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x9017,0x6B);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9018,0x02);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9019,0x6B);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x901A,0x02);//  Enable BP 2//

// Breakpoint table entry 3      AF p74_03 udwFocusMeasure -> udwPrevFocusMeasureAF                       
HM5065MIPI_write_cmos_sensor(0x901D,0x01);//     //	
HM5065MIPI_write_cmos_sensor(0x901E,0xAC);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x901F,0x70);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9020,0x00);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9021,0xC5);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x9022,0x03);//  Enable BP 3//

// Breakpoint table entry 4     AV
HM5065MIPI_write_cmos_sensor(0x9026,0x9C);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x9027,0x5B);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9028,0x00);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9029,0xBF);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x902A,0x02);//  Enable BP 4//

// Breakpoint table entry 5     AV OTP
HM5065MIPI_write_cmos_sensor(0x902E,0x60);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x902F,0x1C);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9030,0x01);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9031,0x37);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x9032,0x02);//  Enable BP 3//

// Breakpoint table entry 6     AF stat
HM5065MIPI_write_cmos_sensor(0x9035,0x01);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x9036,0xBA);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x9037,0x70);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9038,0x00);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9039,0x00);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x903A,0x03);//  Enable BP 6//

// Breakpoint table entry 7     AF Init  part 1
HM5065MIPI_write_cmos_sensor(0x903E,0x21);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x903F,0x3F);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9040,0x02);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9041,0x40);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x9042,0x02);//  Enable BP 7//

// Breakpoint table entry 8     AF Init part 2
HM5065MIPI_write_cmos_sensor(0x9046,0x21);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x9047,0xEA);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9048,0x02);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9049,0x43);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x904A,0x02);//  Enable BP 8//

// Breakpoint table entry 9     Stop/Start  part 1
HM5065MIPI_write_cmos_sensor(0x904E,0xA6);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x904F,0x12);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9050,0x02);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9051,0x46);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x9052,0x02);//  Enable BP 9//

// Breakpoint table entry 10     Stop/Start part 2
HM5065MIPI_write_cmos_sensor(0x9056,0x29);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x9057,0xE3);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9058,0x02);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9059,0x49);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x905A,0x02);//  Enable BP 10//

// Breakpoint table entry 11     AF light 
HM5065MIPI_write_cmos_sensor(0x905D,0x01);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x905E,0x9C);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x905F,0x6E);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9060,0x05);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9061,0x00);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x9062,0x02);//  Enable BP 11//

// Breakpoint table entry 12     //   AF p74_04  part 2
HM5065MIPI_write_cmos_sensor(0x9065,0x01);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x9066,0xA2);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x9067,0x66);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9068,0x02);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9069,0x35);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x906A,0x02);//  Enable BP 12//

// Breakpoint table entry 13     AF p74-05  
HM5065MIPI_write_cmos_sensor(0x906D,0x01);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x906E,0xB5);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x906F,0xC2);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9070,0x02);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9071,0x9B);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x9072,0x02);//  Enable BP 13//

// Breakpoint table entry 14     //   AF p74_06  
HM5065MIPI_write_cmos_sensor(0x9075,0x01);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x9076,0xA2);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x9077,0xD4);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9078,0x02);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9079,0xBE);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x907A,0x02);//  Enable BP 14//

// Breakpoint table entry 15     bAF_NextState = AF_STATE_COARSE  
HM5065MIPI_write_cmos_sensor(0x907D,0x01);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x907E,0xB7);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x907F,0xEA);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9080,0x00);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9081,0x02);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x9082,0x03);//  Enable BP 15//

// Breakpoint table entry 16     //   OTP WB  
//HM5065MIPI_write_cmos_sensor(0x9085,0x00);//  Patch break point address bank//

HM5065MIPI_write_cmos_sensor(0x9086,0x67);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x9087,0x31);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9088,0x02);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9089,0xF7);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x908A,0x02);//  Enable BP 16//

// Breakpoint table entry 17      OTP darkcal   
//HM5065MIPI_write_cmos_sensor(0x908D,0x00);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x908E,0x66);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x908F,0xED);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9090,0x03);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9091,0x1D);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x9092,0x02);//  Enable BP 17//

// Breakpoint table entry 18     //   OTP HFPN  
//HM5065MIPI_write_cmos_sensor(0x9095,0x00);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x9096,0x67);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x9097,0x73);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x9098,0x03);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x9099,0xD3);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x909A,0x02);//  Enable BP 18//

// Breakpoint table entry 19      OTP VCM   
//HM5065MIPI_write_cmos_sensor(0x909D,0x00);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x909E,0x20);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x909F,0x40);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x90A0,0x03);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x90A1,0x3B);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x90A2,0x02);//  Enable BP 19//

// Breakpoint table entry 20     //   TWakeup work around for PLL3 div   
//HM5065MIPI_write_cmos_sensor(0x90A5,0x00);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x90A6,0xC5);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x90A7,0xC0);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x90A8,0x03);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x90A9,0xF0);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x90AA,0x02);//  Enable BP 20//

// Breakpoint table entry 21   // reset the pipe at stop stage
//HM5065MIPI_write_cmos_sensor(0x90AD,0x00);// 		//Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x90AE,0x41);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x90AF,0xB3);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x90B0,0x00);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x90B1,0xA2);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x90B2,0x02);//  Enable BP 21//

// Breakpoint table entry 22     //   CSI Tinit   
//HM5065MIPI_write_cmos_sensor(0x90B5,0x00);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x90B6,0x44);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x90B7,0xBA);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x90B8,0x00);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x90B9,0xF0);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x90BA,0x03);//  Enable BP 22//

// Breakpoint table entry 23      AV2x2    
//HM5065MIPI_write_cmos_sensor(0x90BD,0x00);// 		//Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x90BE,0x89);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x90BF,0x99);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x90C0,0x04);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x90C1,0x00);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x90C2,0x02);//  Enable BP 23//

// Breakpoint table entry 24     //   DL JPEG CSI clk /2   
//HM5065MIPI_write_cmos_sensor(0x90C5,0x00);//  Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x90C6,0xA7);//  Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x90C7,0x91);//  Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x90C8,0x04);//  Offset High byte//
HM5065MIPI_write_cmos_sensor(0x90C9,0x21);//  Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x90CA,0x02);//  Enable BP 24//

// Breakpoint table entry 25      pipe reset at pause stage    
//HM5065MIPI_write_cmos_sensor(0x90CD,0x00);//    // Patch break point address bank//
HM5065MIPI_write_cmos_sensor(0x90CE,0x3A);//    // Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x90CF,0x51);//    // Patch break point address low byte//
HM5065MIPI_write_cmos_sensor(0x90D0,0x00);//    // Offset High byte//
HM5065MIPI_write_cmos_sensor(0x90D1,0xA2);//    // Offset Low byte//
HM5065MIPI_write_cmos_sensor(0x90D2,0x02);//    // Enable BP 25//

// Breakpoint table entry 26     Freeze WB at low stat
//HM5065MIPI_write_cmos_sensor(0x90D5,0x00);//     // Patch break point address bank//    
HM5065MIPI_write_cmos_sensor(0x90D6,0x86);//     // Patch break point address high byte//
HM5065MIPI_write_cmos_sensor(0x90D7,0x54);//     // Patch break point address low byte// 
HM5065MIPI_write_cmos_sensor(0x90D8,0x04);//     // Offset High byte//                   
HM5065MIPI_write_cmos_sensor(0x90D9,0x47);//     // Offset Low byte//                    
HM5065MIPI_write_cmos_sensor(0x90DA,0x02);//     // Enable BP 26//                       


HM5065MIPI_write_cmos_sensor(0x9000,0x01);//  Enable patch//
HM5065MIPI_write_cmos_sensor(0xffff,0x00);//  MCU release//

mDELAY(200);

//////////////////////////////////////////

HM5065MIPI_write_cmos_sensor(0x0009,0x17);// 
HM5065MIPI_write_cmos_sensor(0x0012,0x00);// 
HM5065MIPI_write_cmos_sensor(0x0013,0x00);// 
#ifdef BIRD_HM6065_MIPI_MODE			
		HM5065MIPI_write_cmos_sensor(0x0016,0x02); 	//	Parallel(00) mode or mipi/1 lane(01) /2 lanes(02)
#else
		HM5065MIPI_write_cmos_sensor(0x0016,0x00);	//	Parallel(00) mode or mipi/1 lane(01) /2 lanes(02)
#endif		
HM5065MIPI_write_cmos_sensor(0x0021,0x00);// 
HM5065MIPI_write_cmos_sensor(0x0022,0x01);// 

HM5065MIPI_write_cmos_sensor(0x0040,0x01);//  	// AB2
HM5065MIPI_write_cmos_sensor(0x0041,0x0a);//  	// Image Size Manual
HM5065MIPI_write_cmos_sensor(0x0042,0x05);//  	// 1280
HM5065MIPI_write_cmos_sensor(0x0043,0x00);//  
HM5065MIPI_write_cmos_sensor(0x0044,0x03);//  	// 960
HM5065MIPI_write_cmos_sensor(0x0045,0xC0);//  
HM5065MIPI_write_cmos_sensor(0x0046,0x02);//  	// DataFormat_YCbCr_Custom

HM5065MIPI_write_cmos_sensor(0x0060,0x00);// 
HM5065MIPI_write_cmos_sensor(0x0061,0x00);// 
HM5065MIPI_write_cmos_sensor(0x0066,0x02);// 
                                  
HM5065MIPI_write_cmos_sensor(0x0083,0x01);//         // Horizontal Mirror Enable
HM5065MIPI_write_cmos_sensor(0x0084,0x01);//         // Vertical Flip Enable
HM5065MIPI_write_cmos_sensor(0x0085,0x02);//  YCbYCr Order

HM5065MIPI_write_cmos_sensor(0x00B2,0x4E);// 50       // set PLL output 713MHz
HM5065MIPI_write_cmos_sensor(0x00B3,0xCA);// c9
HM5065MIPI_write_cmos_sensor(0x00B4,0x01);//         // E_div
HM5065MIPI_write_cmos_sensor(0x00B5,0x01);//         // PLL3_div 2
HM5065MIPI_write_cmos_sensor(0x0030,0x11);

HM5065MIPI_write_cmos_sensor(0x00E8,0x01);// 
HM5065MIPI_write_cmos_sensor(0x00ED,0x05);//  Min Framerate
HM5065MIPI_write_cmos_sensor(0x00EE,0x1E);//  Max Framerate

HM5065MIPI_write_cmos_sensor(0x0129,0x00);// 
HM5065MIPI_write_cmos_sensor(0x0130,0x00);// 

HM5065MIPI_write_cmos_sensor(0x019C,0x4B);// 
HM5065MIPI_write_cmos_sensor(0x019D,0xC0);// 

HM5065MIPI_write_cmos_sensor(0x01A0,0x01);// 
HM5065MIPI_write_cmos_sensor(0x01A1,0x80);// 
HM5065MIPI_write_cmos_sensor(0x01A2,0x80);// 
HM5065MIPI_write_cmos_sensor(0x01A3,0x80);// 

HM5065MIPI_write_cmos_sensor(0x5200,0x01);// 

//HM5065MIPI_write_cmos_sensor(0x7000,0x2C);// 
HM5065MIPI_write_cmos_sensor(0x7000,0x0C);// 

HM5065MIPI_write_cmos_sensor(0x7101,0xC4);// 
HM5065MIPI_write_cmos_sensor(0x7102,0x01);// 
HM5065MIPI_write_cmos_sensor(0x7103,0x00);// 
HM5065MIPI_write_cmos_sensor(0x7104,0x02);//  OIF threshold = 512
HM5065MIPI_write_cmos_sensor(0x7105,0x00);// 
HM5065MIPI_write_cmos_sensor(0x7158,0x00);// 

//**********************************************
// System Max Integration Time limits
//**********************************************

// Set UserMaximumIntegratonTime_us = 100000
HM5065MIPI_write_cmos_sensor(0x0143,0x5F);// 
HM5065MIPI_write_cmos_sensor(0x0144,0x0D);// 

//**********************************************
// System Gain limits
//**********************************************

// Set SensorAnalogGainCeiling = 4.0
HM5065MIPI_write_cmos_sensor(0x02C2,0x00);// 
HM5065MIPI_write_cmos_sensor(0x02C3,0xC0);// 

// Set DigitalGainCeiling = 2.0
HM5065MIPI_write_cmos_sensor(0x015E,0x40);// 
HM5065MIPI_write_cmos_sensor(0x015F,0x00);// 


//**********************************************
// Defects correction and noise reduction
//**********************************************

// Arctic controls
HM5065MIPI_write_cmos_sensor(0x0390,0x01);//  ArcticControl fArcticEnable
HM5065MIPI_write_cmos_sensor(0x0391,0x00);//  ArcticControl fArcticConfig DEFAULT CONFIG
HM5065MIPI_write_cmos_sensor(0x0392,0x00);//  ArcticControl fGNFConfig    DEFAULT CONFIG

// SetArcticCCSigmaControl tuned settings
// Value LL = 4 (3444736us);
// Value HL = 20 (24000us);
HM5065MIPI_write_cmos_sensor(0x03A0,0x14);//  ArcticCCSigmaControl fMaximumCCSigma 
HM5065MIPI_write_cmos_sensor(0x03A1,0x00);//  ArcticCCSigmaControl fDisablePromotion {CompiledExposureTime}
HM5065MIPI_write_cmos_sensor(0x03A2,0x5A);//  ArcticCCSigmaControl fDamperLowThreshold {MSB}   2400
HM5065MIPI_write_cmos_sensor(0x03A3,0xEE);//  ArcticCCSigmaControl fDamperLowThreshold {LSB}   
HM5065MIPI_write_cmos_sensor(0x03A4,0x69);//  ArcticCCSigmaControl fDamperHighThreshold {MSB}   3444736
HM5065MIPI_write_cmos_sensor(0x03A5,0x49);//  ArcticCCSigmaControl fDamperHighThreshold {LSB}
HM5065MIPI_write_cmos_sensor(0x03A6,0x3E);//  ArcticCCSigmaControl fY1 {MSB}   Low threshold
HM5065MIPI_write_cmos_sensor(0x03A7,0x00);//  ArcticCCSigmaControl fY1 {LSB} 
HM5065MIPI_write_cmos_sensor(0x03A8,0x39);//  ArcticCCSigmaControl fY2 {MSB}   High threshold
HM5065MIPI_write_cmos_sensor(0x03A9,0x33);//  ArcticCCSigmaControl fY2 {LSB} 

// SetArcticRingControl  tuned settings
// Value LL = 75 (3444736us);
// Value HL = 96 (24000us);
HM5065MIPI_write_cmos_sensor(0x03B0,0x60);//  ArcticCCSigmaControl fMaximumRing 
HM5065MIPI_write_cmos_sensor(0x03B1,0x00);//  ArcticCCSigmaControl fDisablePromotion {CompiledExposureTime}
HM5065MIPI_write_cmos_sensor(0x03B2,0x5A);//  ArcticCCSigmaControl fDamperLowThreshold {MSB}    24000
HM5065MIPI_write_cmos_sensor(0x03B3,0xEE);//  ArcticCCSigmaControl fDamperLowThreshold {LSB}
HM5065MIPI_write_cmos_sensor(0x03B4,0x69);//  ArcticCCSigmaControl DamperHighThreshold {MSB}    3444736
HM5065MIPI_write_cmos_sensor(0x03B5,0x49);//  ArcticCCSigmaControl DamperHighThreshold {LSB}
HM5065MIPI_write_cmos_sensor(0x03B6,0x3E);//  ArcticCCSigmaControl fY1 {MSB}  Low threshold
HM5065MIPI_write_cmos_sensor(0x03B7,0x00);//  ArcticCCSigmaControl fY1 {LSB} 
HM5065MIPI_write_cmos_sensor(0x03B8,0x3D);//  ArcticCCSigmaControl fY2 {MSB}  High threshold
HM5065MIPI_write_cmos_sensor(0x03B9,0x20);//  ArcticCCSigmaControl fY2 {LSB}  

// SetArcticScoringControl  default settings
// Value LL = 16 (3444736us);
// Value HL = 5 (24000us);
HM5065MIPI_write_cmos_sensor(0x03C0,0x10);//  ArcticCCSigmaControl fMaximumScoring 
HM5065MIPI_write_cmos_sensor(0x03C1,0x00);//  ArcticCCSigmaControl fDisablePromotion {CompiledExposureTime}
HM5065MIPI_write_cmos_sensor(0x03C2,0x5A);//  ArcticCCSigmaControl fDamperLowThreshold {MSB}    24000
HM5065MIPI_write_cmos_sensor(0x03C3,0xEE);//  ArcticCCSigmaControl fDamperLowThreshold {LSB}
HM5065MIPI_write_cmos_sensor(0x03C4,0x69);//  ArcticCCSigmaControl DamperHighThreshold {MSB}    3444736
HM5065MIPI_write_cmos_sensor(0x03C5,0x49);//  ArcticCCSigmaControl DamperHighThreshold {LSB}
HM5065MIPI_write_cmos_sensor(0x03C6,0x3A);//  ArcticCCSigmaControl fMinimumDamperOutput {MSB}
HM5065MIPI_write_cmos_sensor(0x03C7,0x80);//  ArcticCCSigmaControl fMinimumDamperOutput {LSB} 

// SetArcticGNFTh1Control  default settings
// Value HW GNFTH1 LL = 100 (3444736us);
// Value HW GNFTH1 HL = 4 (24000us);
HM5065MIPI_write_cmos_sensor(0x03D0,0x64);//  ArcticCCSigmaControl fMaximumScoring 
HM5065MIPI_write_cmos_sensor(0x03D1,0x00);//  ArcticCCSigmaControl fDisablePromotion {CompiledExposureTime}
HM5065MIPI_write_cmos_sensor(0x03D2,0x5A);//  ArcticCCSigmaControl fDamperLowThreshold {MSB}   24000
HM5065MIPI_write_cmos_sensor(0x03D3,0xEE);//  ArcticCCSigmaControl fDamperLowThreshold {LSB}
HM5065MIPI_write_cmos_sensor(0x03D4,0x69);//  ArcticCCSigmaControl DamperHighThreshold {MSB}   3444736
HM5065MIPI_write_cmos_sensor(0x03D5,0x49);//  ArcticCCSigmaControl DamperHighThreshold {LSB}
HM5065MIPI_write_cmos_sensor(0x03D6,0x34);//  ArcticCCSigmaControl fMinimumDamperOutput {MSB}
HM5065MIPI_write_cmos_sensor(0x03D7,0xD1);//  ArcticCCSigmaControl fMinimumDamperOutput {LSB} 


//**********************************************
// Sharpness
//**********************************************

// SetPeakingControl
HM5065MIPI_write_cmos_sensor(0x004C,0x08);//  PipeSetupBank0 fPeakingGain
HM5065MIPI_write_cmos_sensor(0x006C,0x08);//  PipeSetupBank1 fPeakingGain

// Peaking LL value = 3 (3444736us);
// peaking HL value = 14 (24000us);
HM5065MIPI_write_cmos_sensor(0x0350,0x00);//  PeakingControl fDisableGainDamping  {CompiledExposureTime}
HM5065MIPI_write_cmos_sensor(0x0351,0x5A);//  PeakingControl fDamperLowThreshold_Gain  {MSB}   24000
HM5065MIPI_write_cmos_sensor(0x0352,0xEE);//  PeakingControl fDamperLowThreshold_Gain  {LSB}
HM5065MIPI_write_cmos_sensor(0x0353,0x69);//  PeakingControl fDamperHighThreshold_Gain  {MSB}  3444736
HM5065MIPI_write_cmos_sensor(0x0354,0x49);//  PeakingControl fDamperHighThreshold_Gain  {LSB}
HM5065MIPI_write_cmos_sensor(0x0355,0x39);//  PeakingControl fMinimumDamperOutput_Gain  {MSB}
HM5065MIPI_write_cmos_sensor(0x0356,0x6D);//  PeakingControl fMinimumDamperOutput_Gain  {LSB}

// Coring LL Value = 25 (3444736us);
// Coring HL Value = 5 (24000us);
HM5065MIPI_write_cmos_sensor(0x0357,0x19);//  PeakingControl fUserPeakLoThresh 
HM5065MIPI_write_cmos_sensor(0x0358,0x00);//  PeakingControl fDisableCoringDamping  {CompiledExposureTime}
HM5065MIPI_write_cmos_sensor(0x0359,0x3C);//  PeakingControl fUserPeakHiThresh
 
HM5065MIPI_write_cmos_sensor(0x035A,0x5A);//  PeakingControl fDamperLowThreshold_Coring  {MSB}  24000
HM5065MIPI_write_cmos_sensor(0x035B,0xEE);//  PeakingControl fDamperLowThreshold_Coring  {LSB}
HM5065MIPI_write_cmos_sensor(0x035C,0x69);//  PeakingControl fDamperHighThreshold_Coring {MSB}  3444736
HM5065MIPI_write_cmos_sensor(0x035D,0x49);//  PeakingControl fDamperHighThreshold_Coring  {LSB}
HM5065MIPI_write_cmos_sensor(0x035E,0x39);//  PeakingControl fMinimumDamperOutput_Coring  {MSB}
HM5065MIPI_write_cmos_sensor(0x035F,0x85);//  PeakingControl fMinimumDamperOutput_Coring  {LSB}


//**********************************************
// Gamma
//**********************************************

HM5065MIPI_write_cmos_sensor(0x0049,0x14);//  PipeSetupBank0 bGammaGain 
HM5065MIPI_write_cmos_sensor(0x004A,0x0D);//  PipeSetupBank0 bGammaInterpolationGain
HM5065MIPI_write_cmos_sensor(0x0069,0x14);//  PipeSetupBank1 bGammaGain 
HM5065MIPI_write_cmos_sensor(0x006A,0x0D);//  PipeSetupBank1 bGammaInterpolationGain

// SetGammaGainDamperControl
// Value LL = 5 (3444736us);
// Value HL = 20 (24000us);
HM5065MIPI_write_cmos_sensor(0x0090,0x5A);//  GammaGainDamperControl fpX1 {MSB}   24000
HM5065MIPI_write_cmos_sensor(0x0091,0xEE);//  GammaGainDamperControl fpX1 {LSB}
HM5065MIPI_write_cmos_sensor(0x0092,0x3E);//  GammaGainDamperControl fpY1 {MSB}   1
HM5065MIPI_write_cmos_sensor(0x0093,0x00);//  GammaGainDamperControl fpY1 {LSB}
HM5065MIPI_write_cmos_sensor(0x0094,0x69);//  GammaGainDamperControl fpX2 {MSB}   3444736
HM5065MIPI_write_cmos_sensor(0x0095,0x49);//  GammaGainDamperControl fpX2 {LSB}
HM5065MIPI_write_cmos_sensor(0x0096,0x39);//  GammaGainDamperControl fpY2 {MSB}   0.238
HM5065MIPI_write_cmos_sensor(0x0097,0xCF);//  GammaGainDamperControl fpY2 {LSB}
HM5065MIPI_write_cmos_sensor(0x0098,0x00);//   // GammaGainDamperControl fDisable {CompiledExposureTime}

// SetGammaInterpolationDamperControl
// Value LL = 6 (3444736us);
// Value HL = 13 (24000us);
HM5065MIPI_write_cmos_sensor(0x00A0,0x5A);//  GammaInterpolationDamperControl fpX1 {MSB}   24000 
HM5065MIPI_write_cmos_sensor(0x00A1,0xEE);//  GammaInterpolationDamperControl fpX1 {LSB} 
HM5065MIPI_write_cmos_sensor(0x00A2,0x3E);//  GammaInterpolationDamperControl fpY1 {MSB}   1 
HM5065MIPI_write_cmos_sensor(0x00A3,0x00);//  GammaInterpolationDamperControl fpY1 {LSB} 
HM5065MIPI_write_cmos_sensor(0x00A4,0x69);//  GammaInterpolationDamperControl fpX2 {MSB}   3444736 
HM5065MIPI_write_cmos_sensor(0x00A5,0x49);//  GammaInterpolationDamperControl fpX2 {LSB} 
HM5065MIPI_write_cmos_sensor(0x00A6,0x3B);//  GammaInterpolationDamperControl fpY2 {MSB}   0.4375 
HM5065MIPI_write_cmos_sensor(0x00A7,0x80);//  GammaInterpolationDamperControl fpY2 {LSB} 
HM5065MIPI_write_cmos_sensor(0x00A8,0x00);//  GammaInterpolationDamperControl fDisable {CompiledExposureTime} 

//**********************************************
// Lens shading based on LGA - Adaptive Lens Shading Parameters
//**********************************************

HM5065MIPI_write_cmos_sensor(0x0420,0xFF);//     // C0_GreenRed_X
HM5065MIPI_write_cmos_sensor(0x0421,0xCC);//     // C0_GreenRed_X LSB
HM5065MIPI_write_cmos_sensor(0x0422,0x00);//     // C0_GreenRed_Y
HM5065MIPI_write_cmos_sensor(0x0423,0x3B);//     // C0_GreenRed_Y LSB
HM5065MIPI_write_cmos_sensor(0x0424,0x00);//     // C0_GreenRed_X2
HM5065MIPI_write_cmos_sensor(0x0425,0x94);//     // C0_GreenRed_X2 LSB
HM5065MIPI_write_cmos_sensor(0x0426,0x00);//     // C0_GreenRed_Y2
HM5065MIPI_write_cmos_sensor(0x0427,0xAD);//     // C0_GreenRed_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x0428,0x00);//     // C0_GreenRed_XY
HM5065MIPI_write_cmos_sensor(0x0429,0x92);//     // C0_GreenRed_XY LSB
HM5065MIPI_write_cmos_sensor(0x042A,0xFF);//     // C0_GreenRed_X2Y
HM5065MIPI_write_cmos_sensor(0x042B,0x89);//     // C0_GreenRed_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x042C,0xFF);//     // C0_GreenRed_XY2
HM5065MIPI_write_cmos_sensor(0x042D,0x9A);//     // C0_GreenRed_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x042E,0xFF);//     // C0_GreenRed_X2Y2
HM5065MIPI_write_cmos_sensor(0x042F,0xC3);//     // C0_GreenRed_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x0430,0x00);//     // C0_Red_X
HM5065MIPI_write_cmos_sensor(0x0431,0x20);//     // C0_Red_X LSB
HM5065MIPI_write_cmos_sensor(0x0432,0x00);//     // C0_Red_Y
HM5065MIPI_write_cmos_sensor(0x0433,0x2F);//     // C0_Red_Y LSB
HM5065MIPI_write_cmos_sensor(0x0434,0x01);//     // C0_Red_X2
HM5065MIPI_write_cmos_sensor(0x0435,0x0D);//     // C0_Red_X2 LSB
HM5065MIPI_write_cmos_sensor(0x0436,0x01);//     // C0_Red_Y2
HM5065MIPI_write_cmos_sensor(0x0437,0x32);//     // C0_Red_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x0438,0x00);//     // C0_Red_XY
HM5065MIPI_write_cmos_sensor(0x0439,0x0C);//     // C0_Red_XY LSB
HM5065MIPI_write_cmos_sensor(0x043A,0xFF);//     // C0_Red_X2Y
HM5065MIPI_write_cmos_sensor(0x043B,0x30);//     // C0_Red_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x043C,0x00);//     // C0_Red_XY2
HM5065MIPI_write_cmos_sensor(0x043D,0x03);//     // C0_Red_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x043E,0xFF);//     // C0_Red_X2Y2
HM5065MIPI_write_cmos_sensor(0x043F,0x5C);//     // C0_Red_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x0450,0x00);//     // C0_Blue_X
HM5065MIPI_write_cmos_sensor(0x0451,0x27);//     // C0_Blue_X LSB
HM5065MIPI_write_cmos_sensor(0x0452,0xFF);//     // C0_Blue_Y
HM5065MIPI_write_cmos_sensor(0x0453,0xE9);//     // C0_Blue_Y LSB
HM5065MIPI_write_cmos_sensor(0x0454,0x00);//     // C0_Blue_X2
HM5065MIPI_write_cmos_sensor(0x0455,0x74);//     // C0_Blue_X2 LSB
HM5065MIPI_write_cmos_sensor(0x0456,0x00);//     // C0_Blue_Y2
HM5065MIPI_write_cmos_sensor(0x0457,0x9C);//     // C0_Blue_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x0458,0xFF);//     // C0_Blue_XY
HM5065MIPI_write_cmos_sensor(0x0459,0xF2);//     // C0_Blue_XY LSB
HM5065MIPI_write_cmos_sensor(0x045A,0xFF);//     // C0_Blue_X2Y
HM5065MIPI_write_cmos_sensor(0x045B,0x8E);//     // C0_Blue_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x045C,0xFE);//     // C0_Blue_XY2
HM5065MIPI_write_cmos_sensor(0x045D,0x53);//     // C0_Blue_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x045E,0xFF);//     // C0_Blue_X2Y2
HM5065MIPI_write_cmos_sensor(0x045F,0xFB);//     // C0_Blue_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x0440,0xFF);//     // C0_GreenBlue_X
HM5065MIPI_write_cmos_sensor(0x0441,0xCD);//     // C0_GreenBlue_X LSB
HM5065MIPI_write_cmos_sensor(0x0442,0x00);//     // C0_GreenBlue_Y
HM5065MIPI_write_cmos_sensor(0x0443,0x3E);//     // C0_GreenBlue_Y LSB
HM5065MIPI_write_cmos_sensor(0x0444,0x00);//     // C0_GreenBlue_X2
HM5065MIPI_write_cmos_sensor(0x0445,0x94);//     // C0_GreenBlue_X2 LSB
HM5065MIPI_write_cmos_sensor(0x0446,0x00);//     // C0_GreenBlue_Y2
HM5065MIPI_write_cmos_sensor(0x0447,0xAD);//     // C0_GreenBlue_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x0448,0x00);//     // C0_GreenBlue_XY
HM5065MIPI_write_cmos_sensor(0x0449,0x96);//     // C0_GreenBlue_XY LSB
HM5065MIPI_write_cmos_sensor(0x044A,0xFF);//     // C0_GreenBlue_X2Y
HM5065MIPI_write_cmos_sensor(0x044B,0x81);//     // C0_GreenBlue_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x044C,0xFF);//     // C0_GreenBlue_XY2
HM5065MIPI_write_cmos_sensor(0x044D,0x99);//     // C0_GreenBlue_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x044E,0xFF);//     // C0_GreenBlue_X2Y2
HM5065MIPI_write_cmos_sensor(0x044F,0xC5);//     // C0_GreenBlue_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x0460,0xFF);//     // C1_GreenRed_X
HM5065MIPI_write_cmos_sensor(0x0461,0xC2);//     // C1_GreenRed_X LSB
HM5065MIPI_write_cmos_sensor(0x0462,0x00);//     // C1_GreenRed_Y
HM5065MIPI_write_cmos_sensor(0x0463,0x3E);//     // C1_GreenRed_Y LSB
HM5065MIPI_write_cmos_sensor(0x0464,0x00);//     // C1_GreenRed_X2
HM5065MIPI_write_cmos_sensor(0x0465,0x89);//     // C1_GreenRed_X2 LSB
HM5065MIPI_write_cmos_sensor(0x0466,0x00);//     // C1_GreenRed_Y2
HM5065MIPI_write_cmos_sensor(0x0467,0x9E);//     // C1_GreenRed_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x0468,0x00);//     // C1_GreenRed_XY
HM5065MIPI_write_cmos_sensor(0x0469,0x95);//     // C1_GreenRed_XY LSB
HM5065MIPI_write_cmos_sensor(0x046A,0xFF);//     // C1_GreenRed_X2Y
HM5065MIPI_write_cmos_sensor(0x046B,0x89);//     // C1_GreenRed_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x046C,0xFF);//     // C1_GreenRed_XY2
HM5065MIPI_write_cmos_sensor(0x046D,0x93);//     // C1_GreenRed_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x046E,0xFF);//     // C1_GreenRed_X2Y2
HM5065MIPI_write_cmos_sensor(0x046F,0xE8);//     // C1_GreenRed_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x0470,0x00);//     // C1_Red_X
HM5065MIPI_write_cmos_sensor(0x0471,0x20);//     // C1_Red_X LSB
HM5065MIPI_write_cmos_sensor(0x0472,0x00);//     // C1_Red_Y
HM5065MIPI_write_cmos_sensor(0x0473,0x20);//     // C1_Red_Y LSB
HM5065MIPI_write_cmos_sensor(0x0474,0x00);//     // C1_Red_X2
HM5065MIPI_write_cmos_sensor(0x0475,0xC2);//     // C1_Red_X2 LSB
HM5065MIPI_write_cmos_sensor(0x0476,0x00);//     // C1_Red_Y2
HM5065MIPI_write_cmos_sensor(0x0477,0xDB);//     // C1_Red_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x0478,0x00);//     // C1_Red_XY
HM5065MIPI_write_cmos_sensor(0x0479,0x02);//     // C1_Red_XY LSB
HM5065MIPI_write_cmos_sensor(0x047A,0xFF);//     // C1_Red_X2Y
HM5065MIPI_write_cmos_sensor(0x047B,0x4D);//     // C1_Red_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x047C,0xFF);//     // C1_Red_XY2
HM5065MIPI_write_cmos_sensor(0x047D,0x46);//     // C1_Red_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x047E,0xFF);//     // C1_Red_X2Y2
HM5065MIPI_write_cmos_sensor(0x047F,0xD2);//     // C1_Red_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x0490,0x00);//     // C1_Blue_X
HM5065MIPI_write_cmos_sensor(0x0491,0x1C);//     // C1_Blue_X LSB
HM5065MIPI_write_cmos_sensor(0x0492,0xFF);//     // C1_Blue_Y
HM5065MIPI_write_cmos_sensor(0x0493,0xF6);//     // C1_Blue_Y LSB
HM5065MIPI_write_cmos_sensor(0x0494,0x00);//     // C1_Blue_X2
HM5065MIPI_write_cmos_sensor(0x0495,0x74);//     // C1_Blue_X2 LSB
HM5065MIPI_write_cmos_sensor(0x0496,0x00);//     // C1_Blue_Y2
HM5065MIPI_write_cmos_sensor(0x0497,0x8F);//     // C1_Blue_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x0498,0xFF);//     // C1_Blue_XY
HM5065MIPI_write_cmos_sensor(0x0499,0xFD);//     // C1_Blue_XY LSB
HM5065MIPI_write_cmos_sensor(0x049A,0xFF);//     // C1_Blue_X2Y
HM5065MIPI_write_cmos_sensor(0x049B,0x5B);//     // C1_Blue_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x049C,0xFF);//     // C1_Blue_XY2
HM5065MIPI_write_cmos_sensor(0x049D,0x00);//     // C1_Blue_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x049E,0x00);//     // C1_Blue_X2Y2
HM5065MIPI_write_cmos_sensor(0x049F,0x0B);//     // C1_Blue_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x0480,0xFF);//     // C1_GreenBlue_X
HM5065MIPI_write_cmos_sensor(0x0481,0xC4);//     // C1_GreenBlue_X LSB
HM5065MIPI_write_cmos_sensor(0x0482,0x00);//     // C1_GreenBlue_Y
HM5065MIPI_write_cmos_sensor(0x0483,0x40);//     // C1_GreenBlue_Y LSB
HM5065MIPI_write_cmos_sensor(0x0484,0x00);//     // C1_GreenBlue_X2
HM5065MIPI_write_cmos_sensor(0x0485,0x89);//     // C1_GreenBlue_X2 LSB
HM5065MIPI_write_cmos_sensor(0x0486,0x00);//     // C1_GreenBlue_Y2
HM5065MIPI_write_cmos_sensor(0x0487,0x9E);//     // C1_GreenBlue_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x0488,0x00);//     // C1_GreenBlue_XY
HM5065MIPI_write_cmos_sensor(0x0489,0x9A);//     // C1_GreenBlue_XY LSB
HM5065MIPI_write_cmos_sensor(0x048A,0xFF);//     // C1_GreenBlue_X2Y
HM5065MIPI_write_cmos_sensor(0x048B,0x7D);//     // C1_GreenBlue_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x048C,0xFF);//     // C1_GreenBlue_XY2
HM5065MIPI_write_cmos_sensor(0x048D,0x90);//     // C1_GreenBlue_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x048E,0xFF);//     // C1_GreenBlue_X2Y2
HM5065MIPI_write_cmos_sensor(0x048F,0xEA);//     // C1_GreenBlue_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x04A0,0xFF);//     // C2_GreenRed_X
HM5065MIPI_write_cmos_sensor(0x04A1,0xD0);//     // C2_GreenRed_X LSB
HM5065MIPI_write_cmos_sensor(0x04A2,0x00);//     // C2_GreenRed_Y
HM5065MIPI_write_cmos_sensor(0x04A3,0x49);//     // C2_GreenRed_Y LSB
HM5065MIPI_write_cmos_sensor(0x04A4,0x00);//     // C2_GreenRed_X2
HM5065MIPI_write_cmos_sensor(0x04A5,0x72);//     // C2_GreenRed_X2 LSB
HM5065MIPI_write_cmos_sensor(0x04A6,0x00);//     // C2_GreenRed_Y2
HM5065MIPI_write_cmos_sensor(0x04A7,0x89);//     // C2_GreenRed_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x04A8,0x00);//     // C2_GreenRed_XY
HM5065MIPI_write_cmos_sensor(0x04A9,0x8B);//     // C2_GreenRed_XY LSB
HM5065MIPI_write_cmos_sensor(0x04AA,0xFF);//     // C2_GreenRed_X2Y
HM5065MIPI_write_cmos_sensor(0x04AB,0x7D);//     // C2_GreenRed_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x04AC,0xFF);//     // C2_GreenRed_XY2
HM5065MIPI_write_cmos_sensor(0x04AD,0x79);//     // C2_GreenRed_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x04AE,0xFF);//     // C2_GreenRed_X2Y2
HM5065MIPI_write_cmos_sensor(0x04AF,0xB5);//     // C2_GreenRed_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x04B0,0x00);//     // C2_Red_X
HM5065MIPI_write_cmos_sensor(0x04B1,0x20);//     // C2_Red_X LSB
HM5065MIPI_write_cmos_sensor(0x04B2,0x00);//     // C2_Red_Y
HM5065MIPI_write_cmos_sensor(0x04B3,0x28);//     // C2_Red_Y LSB
HM5065MIPI_write_cmos_sensor(0x04B4,0x00);//     // C2_Red_X2
HM5065MIPI_write_cmos_sensor(0x04B5,0x7C);//     // C2_Red_X2 LSB
HM5065MIPI_write_cmos_sensor(0x04B6,0x00);//     // C2_Red_Y2
HM5065MIPI_write_cmos_sensor(0x04B7,0xA2);//     // C2_Red_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x04B8,0xFF);//     // C2_Red_XY
HM5065MIPI_write_cmos_sensor(0x04B9,0xFE);//     // C2_Red_XY LSB
HM5065MIPI_write_cmos_sensor(0x04BA,0xFF);//     // C2_Red_X2Y
HM5065MIPI_write_cmos_sensor(0x04BB,0x6B);//     // C2_Red_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x04BC,0xFF);//     // C2_Red_XY2
HM5065MIPI_write_cmos_sensor(0x04BD,0x40);//     // C2_Red_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x04BE,0xFF);//     // C2_Red_X2Y2
HM5065MIPI_write_cmos_sensor(0x04BF,0xAE);//     // C2_Red_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x04D0,0x00);//     // C2_Blue_X
HM5065MIPI_write_cmos_sensor(0x04D1,0x24);//     // C2_Blue_X LSB
HM5065MIPI_write_cmos_sensor(0x04D2,0x00);//     // C2_Blue_Y
HM5065MIPI_write_cmos_sensor(0x04D3,0x02);//     // C2_Blue_Y LSB
HM5065MIPI_write_cmos_sensor(0x04D4,0x00);//     // C2_Blue_X2
HM5065MIPI_write_cmos_sensor(0x04D5,0x60);//     // C2_Blue_X2 LSB
HM5065MIPI_write_cmos_sensor(0x04D6,0x00);//     // C2_Blue_Y2
HM5065MIPI_write_cmos_sensor(0x04D7,0x7D);//     // C2_Blue_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x04D8,0xFF);//     // C2_Blue_XY
HM5065MIPI_write_cmos_sensor(0x04D9,0xF7);//     // C2_Blue_XY LSB
HM5065MIPI_write_cmos_sensor(0x04DA,0xFF);//     // C2_Blue_X2Y
HM5065MIPI_write_cmos_sensor(0x04DB,0x5F);//     // C2_Blue_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x04DC,0xFF);//     // C2_Blue_XY2
HM5065MIPI_write_cmos_sensor(0x04DD,0x57);//     // C2_Blue_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x04DE,0xFF);//     // C2_Blue_X2Y2
HM5065MIPI_write_cmos_sensor(0x04DF,0xD1);//     // C2_Blue_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x04C0,0xFF);//     // C2_GreenBlue_X
HM5065MIPI_write_cmos_sensor(0x04C1,0xD1);//     // C2_GreenBlue_X LSB
HM5065MIPI_write_cmos_sensor(0x04C2,0x00);//     // C2_GreenBlue_Y
HM5065MIPI_write_cmos_sensor(0x04C3,0x4B);//     // C2_GreenBlue_Y LSB
HM5065MIPI_write_cmos_sensor(0x04C4,0x00);//     // C2_GreenBlue_X2
HM5065MIPI_write_cmos_sensor(0x04C5,0x72);//     // C2_GreenBlue_X2 LSB
HM5065MIPI_write_cmos_sensor(0x04C6,0x00);//     // C2_GreenBlue_Y2
HM5065MIPI_write_cmos_sensor(0x04C7,0x89);//     // C2_GreenBlue_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x04C8,0x00);//     // C2_GreenBlue_XY
HM5065MIPI_write_cmos_sensor(0x04C9,0x8F);//     // C2_GreenBlue_XY LSB
HM5065MIPI_write_cmos_sensor(0x04CA,0xFF);//     // C2_GreenBlue_X2Y
HM5065MIPI_write_cmos_sensor(0x04CB,0x7B);//     // C2_GreenBlue_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x04CC,0xFF);//     // C2_GreenBlue_XY2
HM5065MIPI_write_cmos_sensor(0x04CD,0x7B);//     // C2_GreenBlue_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x04CE,0xFF);//     // C2_GreenBlue_X2Y2
HM5065MIPI_write_cmos_sensor(0x04CF,0xB4);//     // C2_GreenBlue_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x04E0,0xFF);//     // C3_GreenRed_X
HM5065MIPI_write_cmos_sensor(0x04E1,0xF0);//     // C3_GreenRed_X LSB
HM5065MIPI_write_cmos_sensor(0x04E2,0x00);//     // C3_GreenRed_Y
HM5065MIPI_write_cmos_sensor(0x04E3,0x44);//     // C3_GreenRed_Y LSB
HM5065MIPI_write_cmos_sensor(0x04E4,0x00);//     // C3_GreenRed_X2
HM5065MIPI_write_cmos_sensor(0x04E5,0x8B);//     // C3_GreenRed_X2 LSB
HM5065MIPI_write_cmos_sensor(0x04E6,0x00);//     // C3_GreenRed_Y2
HM5065MIPI_write_cmos_sensor(0x04E7,0xA1);//     // C3_GreenRed_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x04E8,0x00);//     // C3_GreenRed_XY
HM5065MIPI_write_cmos_sensor(0x04E9,0xA7);//     // C3_GreenRed_XY LSB
HM5065MIPI_write_cmos_sensor(0x04EA,0xFF);//     // C3_GreenRed_X2Y
HM5065MIPI_write_cmos_sensor(0x04EB,0x6F);//     // C3_GreenRed_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x04EC,0xFF);//     // C3_GreenRed_XY2
HM5065MIPI_write_cmos_sensor(0x04ED,0x86);//     // C3_GreenRed_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x04EE,0xFF);//     // C3_GreenRed_X2Y2
HM5065MIPI_write_cmos_sensor(0x04EF,0xD9);//     // C3_GreenRed_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x04F0,0x00);//     // C3_Red_X
HM5065MIPI_write_cmos_sensor(0x04F1,0x30);//     // C3_Red_X LSB
HM5065MIPI_write_cmos_sensor(0x04F2,0x00);//     // C3_Red_Y
HM5065MIPI_write_cmos_sensor(0x04F3,0x31);//     // C3_Red_Y LSB
HM5065MIPI_write_cmos_sensor(0x04F4,0x00);//     // C3_Red_X2
HM5065MIPI_write_cmos_sensor(0x04F5,0xC8);//     // C3_Red_X2 LSB
HM5065MIPI_write_cmos_sensor(0x04F6,0x00);//     // C3_Red_Y2
HM5065MIPI_write_cmos_sensor(0x04F7,0xFA);//     // C3_Red_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x04F8,0x00);//     // C3_Red_XY
HM5065MIPI_write_cmos_sensor(0x04F9,0x04);//     // C3_Red_XY LSB
HM5065MIPI_write_cmos_sensor(0x04FA,0xFE);//     // C3_Red_X2Y
HM5065MIPI_write_cmos_sensor(0x04FB,0xC0);//     // C3_Red_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x04FC,0x01);//     // C3_Red_XY2
HM5065MIPI_write_cmos_sensor(0x04FD,0x40);//     // C3_Red_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x04FE,0xFF);//     // C3_Red_X2Y2
HM5065MIPI_write_cmos_sensor(0x04FF,0x68);//     // C3_Red_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x0510,0x00);//     // C3_Blue_X
HM5065MIPI_write_cmos_sensor(0x0511,0x36);//     // C3_Blue_X LSB
HM5065MIPI_write_cmos_sensor(0x0512,0xFF);//     // C3_Blue_Y
HM5065MIPI_write_cmos_sensor(0x0513,0xFA);//     // C3_Blue_Y LSB
HM5065MIPI_write_cmos_sensor(0x0514,0x00);//     // C3_Blue_X2
HM5065MIPI_write_cmos_sensor(0x0515,0x78);//     // C3_Blue_X2 LSB
HM5065MIPI_write_cmos_sensor(0x0516,0x00);//     // C3_Blue_Y2
HM5065MIPI_write_cmos_sensor(0x0517,0x94);//     // C3_Blue_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x0518,0x00);//     // C3_Blue_XY
HM5065MIPI_write_cmos_sensor(0x0519,0x01);//     // C3_Blue_XY LSB
HM5065MIPI_write_cmos_sensor(0x051A,0xFF);//     // C3_Blue_X2Y
HM5065MIPI_write_cmos_sensor(0x051B,0x4D);//     // C3_Blue_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x051C,0xFF);//     // C3_Blue_XY2
HM5065MIPI_write_cmos_sensor(0x051D,0x66);//     // C3_Blue_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x051E,0xFF);//     // C3_Blue_X2Y2
HM5065MIPI_write_cmos_sensor(0x051F,0xFC);//     // C3_Blue_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x0500,0xFF);//     // C3_GreenBlue_X
HM5065MIPI_write_cmos_sensor(0x0501,0xF0);//     // C3_GreenBlue_X LSB
HM5065MIPI_write_cmos_sensor(0x0502,0x00);//     // C3_GreenBlue_Y
HM5065MIPI_write_cmos_sensor(0x0503,0x48);//     // C3_GreenBlue_Y LSB
HM5065MIPI_write_cmos_sensor(0x0504,0x00);//     // C3_GreenBlue_X2
HM5065MIPI_write_cmos_sensor(0x0505,0x8B);//     // C3_GreenBlue_X2 LSB
HM5065MIPI_write_cmos_sensor(0x0506,0x00);//     // C3_GreenBlue_Y2
HM5065MIPI_write_cmos_sensor(0x0507,0xA2);//     // C3_GreenBlue_Y2 LSB
HM5065MIPI_write_cmos_sensor(0x0508,0x00);//     // C3_GreenBlue_XY
HM5065MIPI_write_cmos_sensor(0x0509,0xA9);//     // C3_GreenBlue_XY LSB
HM5065MIPI_write_cmos_sensor(0x050A,0xFF);//     // C3_GreenBlue_X2Y
HM5065MIPI_write_cmos_sensor(0x050B,0x64);//     // C3_GreenBlue_X2Y LSB
HM5065MIPI_write_cmos_sensor(0x050C,0xFF);//     // C3_GreenBlue_XY2
HM5065MIPI_write_cmos_sensor(0x050D,0x8A);//     // C3_GreenBlue_XY2 LSB
HM5065MIPI_write_cmos_sensor(0x050E,0xFF);//     // C3_GreenBlue_X2Y2
HM5065MIPI_write_cmos_sensor(0x050F,0xDA);//     // C3_GreenBlue_X2Y2 LSB

HM5065MIPI_write_cmos_sensor(0x0561,0x00);//     // C0 Unity
HM5065MIPI_write_cmos_sensor(0x0562,0x00);//     // C1 Unity
HM5065MIPI_write_cmos_sensor(0x0563,0x00);//     // C2 Unity
HM5065MIPI_write_cmos_sensor(0x0564,0x01);//     // C3 Unity

// Casts      
HM5065MIPI_write_cmos_sensor(0x0324,0x39);//     // NormRedGain_Cast0 Hor
HM5065MIPI_write_cmos_sensor(0x0325,0x86);//     // NormRedGain_Cast0_LSB 
HM5065MIPI_write_cmos_sensor(0x0326,0x3A);//     // NormRedGain_Cast1 IncA
HM5065MIPI_write_cmos_sensor(0x0327,0x90);//     // NormRedGain_Cast1_LSB 
HM5065MIPI_write_cmos_sensor(0x0328,0x3A);//     // NormRedGain_Cast2 CWF
HM5065MIPI_write_cmos_sensor(0x0329,0xF6);//     // NormRedGain_Cast2_LSB 
HM5065MIPI_write_cmos_sensor(0x032A,0x3B);//     // NormRedGain_Cast3 D65
HM5065MIPI_write_cmos_sensor(0x032B,0x5D);//     // NormRedGain_Cast3_LSB 

// Antivignette Control
HM5065MIPI_write_cmos_sensor(0x0320,0x01);//     // AntiVignetteControl - Enable
HM5065MIPI_write_cmos_sensor(0x0321,0x04);//     // NbOfPresets
HM5065MIPI_write_cmos_sensor(0x0322,0x01);//     // AdaptiveAntiVignetteControlEnable - Enable
HM5065MIPI_write_cmos_sensor(0x0323,0x01);//     // LoLightAntiVignetteControlDisable - Damper Off


//**********************************************
// Adaptive Colour Matrices
//**********************************************

// Adaptive Colour Matrices
HM5065MIPI_write_cmos_sensor(0x0330,0x01);//  Turn off colour matrix damper
HM5065MIPI_write_cmos_sensor(0x0384,0x00);//  Turn off colour effects
HM5065MIPI_write_cmos_sensor(0x0337,0x01);//  Turn on adaptive colour matrix

// Normalised red gain presets for Colour Matrices
HM5065MIPI_write_cmos_sensor(0x03EC,0x39);//  Matrix 0
HM5065MIPI_write_cmos_sensor(0x03ED,0x86);//  LSB
HM5065MIPI_write_cmos_sensor(0x03FC,0x3A);//  Matrix 1
HM5065MIPI_write_cmos_sensor(0x03FD,0x29);//  LSB
HM5065MIPI_write_cmos_sensor(0x040C,0x3A);//  Matrix 2
HM5065MIPI_write_cmos_sensor(0x040D,0xA4);//  LSB
HM5065MIPI_write_cmos_sensor(0x041C,0x3B);//  Matrix 3
HM5065MIPI_write_cmos_sensor(0x041D,0x5D);//  LSB

// Colour Matrices based on 953 cut 3.0 measured matrices

// Colour Matrix 0 - IncA
// 	1.552	-0.063	-0.489
// 	-0.609	1.385	0.224
// 	-0.500	-1.535	3.035

HM5065MIPI_write_cmos_sensor(0x03E0,0xB6);//  GInR
HM5065MIPI_write_cmos_sensor(0x03E1,0x04);// 
HM5065MIPI_write_cmos_sensor(0x03E2,0xBB);//  BInR
HM5065MIPI_write_cmos_sensor(0x03E3,0xE9);// 
HM5065MIPI_write_cmos_sensor(0x03E4,0xBC);//  RInG
HM5065MIPI_write_cmos_sensor(0x03E5,0x70);// 
HM5065MIPI_write_cmos_sensor(0x03E6,0x39);//  BInG
HM5065MIPI_write_cmos_sensor(0x03E7,0x95);// 
HM5065MIPI_write_cmos_sensor(0x03E8,0xBC);//  RInB
HM5065MIPI_write_cmos_sensor(0x03E9,0x00);// 
HM5065MIPI_write_cmos_sensor(0x03EA,0xBF);//  GInB
HM5065MIPI_write_cmos_sensor(0x03EB,0x12);// 

// Colour Matrix 1 - 3500K
// 	1.624	-0.310	-0.314
// 	-0.467	1.243	0.224
// 	-0.392	-0.918	2.310

HM5065MIPI_write_cmos_sensor(0x03F0,0xBA);//  GInR
HM5065MIPI_write_cmos_sensor(0x03F1,0x7B);// 
HM5065MIPI_write_cmos_sensor(0x03F2,0xBA);//  BInR
HM5065MIPI_write_cmos_sensor(0x03F3,0x83);// 
HM5065MIPI_write_cmos_sensor(0x03F4,0xBB);//  RInG
HM5065MIPI_write_cmos_sensor(0x03F5,0xBC);// 
HM5065MIPI_write_cmos_sensor(0x03F6,0x39);//  BInG
HM5065MIPI_write_cmos_sensor(0x03F7,0x95);// 
HM5065MIPI_write_cmos_sensor(0x03F8,0xBB);//  RInB
HM5065MIPI_write_cmos_sensor(0x03F9,0x23);// 
HM5065MIPI_write_cmos_sensor(0x03FA,0xBD);//  GInB
HM5065MIPI_write_cmos_sensor(0x03FB,0xAC);// 

// Colour Matrix 2 - CWF
// 	1.826	-0.592	-0.234
// 	-0.417	1.076	0.341
// 	-0.024	-1.199	2.223

HM5065MIPI_write_cmos_sensor(0x0400,0xBD);//  GInR
HM5065MIPI_write_cmos_sensor(0x0401,0xF9);// 
HM5065MIPI_write_cmos_sensor(0x0402,0xB9);//  BInR
HM5065MIPI_write_cmos_sensor(0x0403,0xBE);// 
HM5065MIPI_write_cmos_sensor(0x0404,0xBB);//  RInG
HM5065MIPI_write_cmos_sensor(0x0405,0x56);// 
HM5065MIPI_write_cmos_sensor(0x0406,0x3A);//  BInG
HM5065MIPI_write_cmos_sensor(0x0407,0xBA);// 
HM5065MIPI_write_cmos_sensor(0x0408,0xB3);//  RInB
HM5065MIPI_write_cmos_sensor(0x0409,0x12);// 
HM5065MIPI_write_cmos_sensor(0x040A,0xBE);//  GInB
HM5065MIPI_write_cmos_sensor(0x040B,0x66);// 

// Colour Matrix 3 - D65
// 	1.665	-0.405	-0.260
// 	-0.303	1.508	-0.205
// 	-0.105	-0.428	1.533

HM5065MIPI_write_cmos_sensor(0x0410,0xBB);//  GInR
HM5065MIPI_write_cmos_sensor(0x0411,0x3E);// 
HM5065MIPI_write_cmos_sensor(0x0412,0xBA);//  BInR
HM5065MIPI_write_cmos_sensor(0x0413,0x14);// 
HM5065MIPI_write_cmos_sensor(0x0414,0xBA);//  RInG
HM5065MIPI_write_cmos_sensor(0x0415,0x6C);// 
HM5065MIPI_write_cmos_sensor(0x0416,0xB9);//  BInG
HM5065MIPI_write_cmos_sensor(0x0417,0x47);// 
HM5065MIPI_write_cmos_sensor(0x0418,0xB7);//  RInB
HM5065MIPI_write_cmos_sensor(0x0419,0x5C);// 
HM5065MIPI_write_cmos_sensor(0x041A,0xBB);//  GInB
HM5065MIPI_write_cmos_sensor(0x041B,0x6C);// 


//**********************************************
// White Balance Setup - Saturation, MWWB tilt gains, Tilts, Gain Clip, Constrainer
//**********************************************

HM5065MIPI_write_cmos_sensor(0x01A5,0x3E);//  Hue Red Bias
HM5065MIPI_write_cmos_sensor(0x01A6,0x00);// 
HM5065MIPI_write_cmos_sensor(0x01A7,0x3E);//  Hue Blue Bias
HM5065MIPI_write_cmos_sensor(0x01A8,0x00);// 

HM5065MIPI_write_cmos_sensor(0x01f8,0x3c);//     //fpMaximumDistanceAllowedFromLocus
HM5065MIPI_write_cmos_sensor(0x01f9,0x00);//     //=0.5
HM5065MIPI_write_cmos_sensor(0x01fa,0x00);//     //fEnableConstrainedWhiteBalance = false

HM5065MIPI_write_cmos_sensor(0x02a2,0x3e);//     //fpRedTilt
HM5065MIPI_write_cmos_sensor(0x02a3,0x00);//     //= 1.00
HM5065MIPI_write_cmos_sensor(0x02a4,0x3e);//     //fpGreenTilt1
HM5065MIPI_write_cmos_sensor(0x02a5,0x00);//     //= 1.00
HM5065MIPI_write_cmos_sensor(0x02a6,0x3e);//     //fpGreenTilt2
HM5065MIPI_write_cmos_sensor(0x02a7,0x00);//     //= 1.00
HM5065MIPI_write_cmos_sensor(0x02a8,0x3e);//     //fpBlueTilt
HM5065MIPI_write_cmos_sensor(0x02a9,0x00);//     //= 1.00

//MWWB Tilts need set high to avoid constrainer bug
HM5065MIPI_write_cmos_sensor(0x056c,0x42);//     //fpRedTilt
HM5065MIPI_write_cmos_sensor(0x056d,0x00);//     //= 4.00
HM5065MIPI_write_cmos_sensor(0x056e,0x42);//     //fpGreenTilt1
HM5065MIPI_write_cmos_sensor(0x056f,0x00);//     //= 4.00
HM5065MIPI_write_cmos_sensor(0x0570,0x42);//     //fpGreenTilt2
HM5065MIPI_write_cmos_sensor(0x0571,0x00);//     //= 4.00
HM5065MIPI_write_cmos_sensor(0x0572,0x42);//     //fpBlueTilt
HM5065MIPI_write_cmos_sensor(0x0573,0x00);//     //= 4.00


//**********************************************
// Colour Saturation
//**********************************************

// Set Saturation 
HM5065MIPI_write_cmos_sensor(0x0081,0x60);//  PipeSetupCommon bColourSaturation

// Set Colour Saturation Damper
// Value LL = 85
// Value HL = 105
HM5065MIPI_write_cmos_sensor(0x0588,0x00);//  ColourSaturationDamper fDisable {CompiledExposureTime}
HM5065MIPI_write_cmos_sensor(0x0589,0x5A);//  ColourSaturationDamper fpLowThreshold {MSB}
HM5065MIPI_write_cmos_sensor(0x058A,0xEE);//  ColourSaturationDamper fpLowThreshold {LSB}
HM5065MIPI_write_cmos_sensor(0x058B,0x69);//  ColourSaturationDamper fpHighThreshold {MSB}
HM5065MIPI_write_cmos_sensor(0x058C,0x49);//  ColourSaturationDamper fpHighThreshold {LSB}
HM5065MIPI_write_cmos_sensor(0x058D,0x3D);//  ColourSaturationDamper fpMinimumOutput {MSB}
HM5065MIPI_write_cmos_sensor(0x058E,0x3D);//  ColourSaturationDamper fpMinimumOutput {LSB}


//**********************************************
// Others
//**********************************************

// Set Contrast
HM5065MIPI_write_cmos_sensor(0x0080,0x6C);//  PipeSetupCommon bContrast

// Set Brightness
HM5065MIPI_write_cmos_sensor(0x0082,0x5A);//  PipeSetupCommon bBrightness

// Set Exposure Compensation
// WriteByte(0x0130, 0xff);//     iExposureCompensation = -1


//**********************************************
// AF and VCM settings - 29Aug2012
//**********************************************
HM5065MIPI_write_cmos_sensor(0x0659,0x01);//     // AFStatsControls->bCoringValue = 1 Coring Value  Midified by HP 2013/01/08 
HM5065MIPI_write_cmos_sensor(0x065A,0x00);//     // AFStatsControls->bWindowsSystem = 7 zone AF system 

HM5065MIPI_write_cmos_sensor(0x06C9,0x01);//     // FLADriverLowLevelParameters->AutoSkipNextFrame = ENABLED
HM5065MIPI_write_cmos_sensor(0x06CD,0x01);//     // FLADriverLowLevelParameters->AF_OTP_uwHostDefMacro MSB = 445
HM5065MIPI_write_cmos_sensor(0x06CE,0xBD);//     // FLADriverLowLevelParameters->AF_OTP_uwHostDefMacro LSB
HM5065MIPI_write_cmos_sensor(0x06CF,0x00);//     // FLADriverLowLevelParameters->AF_OTP_uwHostDefInfinity MSB = 147
HM5065MIPI_write_cmos_sensor(0x06D0,0x93);//     // FLADriverLowLevelParameters->AF_OTP_uwHostDefInfinity LSB
HM5065MIPI_write_cmos_sensor(0x06D1,0x02);//     // FLADriverLowLevelParameters->AF_OTP_bStepsMultiStepDriver = 2 step driver
HM5065MIPI_write_cmos_sensor(0x06D2,0x30);//     // FLADriverLowLevelParameters->AF_OTP_uwMultiStepTimeDelay MSB = 12.5ms
HM5065MIPI_write_cmos_sensor(0x06D3,0xD4);//     // FLADriverLowLevelParameters->AF_OTP_uwMultiStepTimeDelay LSB
HM5065MIPI_write_cmos_sensor(0x06D4,0x01);//     // FLADriverLowLevelParameters->AF_OTP_fHostEnableOTPRead (1 = disabled);
HM5065MIPI_write_cmos_sensor(0x06DB,0x59);//     // FLADriverLowLevelParameters->fpActuatorResponseTime MSB 12.5ms (FP900); 
HM5065MIPI_write_cmos_sensor(0x06DC,0x0d);//     // FLADriverLowLevelParameters->fpActuatorResponseTime LSB

HM5065MIPI_write_cmos_sensor(0x0730,0x00);//     // FocusRangeConstants->wFullRange_LensMinPosition MSB = 0
HM5065MIPI_write_cmos_sensor(0x0731,0x00);//     // FocusRangeConstants->wFullRange_LensMinPosition LSB
HM5065MIPI_write_cmos_sensor(0x0732,0x03);//     // FocusRangeConstants->wFullRange_LensMaxPosition MSB = 1023
HM5065MIPI_write_cmos_sensor(0x0733,0xFF);//     // FocusRangeConstants->wFullRange_LensMaxPosition LSB
HM5065MIPI_write_cmos_sensor(0x0734,0x03);//     // FocusRangeConstants->wFullRange_LensRecoveryPosition MSB = 880
HM5065MIPI_write_cmos_sensor(0x0735,0x70);//     // FocusRangeConstants->wFullRange_LensRecoveryPosition LSB

HM5065MIPI_write_cmos_sensor(0x0755,0x01);//     // AutoFocusControls->fEnableSimpleCoarseThEvaluation = ENABLED
HM5065MIPI_write_cmos_sensor(0x0756,0x00);//     // AutoFocusControls->bSelectedMultizoneBehavior = REGIONSELECTIONMETHOD_AVERAGE Modified by HP
HM5065MIPI_write_cmos_sensor(0x075A,0x00);//     // AutoFocusControls-fLightVariationEnable = FALSE = do not enable with bugs Modified by HP
HM5065MIPI_write_cmos_sensor(0x075B,0x01);//     // AutoFocusControls->fEnableTrackingThresholdEvaluation = DISABLED MK change to 0x1
HM5065MIPI_write_cmos_sensor(0x075E,0x00);//     // AutoFocusControls->fFineToCoarseAutoTransitionEnable = DISABLED
//HM5065MIPI_write_cmos_sensor(0x075F,0x00);//     // AutoFocusControls-fEnableTimedFineExecution = FALSE  Disable   Modified by HP
HM5065MIPI_write_cmos_sensor(0x0764,0x01);//     // AutoFocusControls->fResetHCSPos = TRUE = Start from Recovery Position for every HCS
HM5065MIPI_write_cmos_sensor(0x0766,0x01);//     // AutoFocusControls->fEnablePrioritiesMacro = FALSE = Do not prioritise Macro mk change to 0x01
HM5065MIPI_write_cmos_sensor(0x0768,0x01);//     // AutoFocusControls->fEnableInterpolationAfterFineSearch = TRUE
HM5065MIPI_write_cmos_sensor(0x076A,0x00);//     // AutoFocusControls->fReducedZoneSetup = TRUE mk change to 0x0


HM5065MIPI_write_cmos_sensor(0x0758,0x01);//     // AutoFocusControls->bWeighedFunctionSelected = TRAPEZIUM
HM5065MIPI_write_cmos_sensor(0x075C,0x01);//     // AutoFocusControls->fEnableHeuristicMethod = FALSE

HM5065MIPI_write_cmos_sensor(0x0770,0x98);//     // AutoFocusConstants->bCoarseStep = 95
HM5065MIPI_write_cmos_sensor(0x0771,0x19);//     // AutoFocusConstants->bFineStep = 16
HM5065MIPI_write_cmos_sensor(0x0772,0x1B);//     // AutoFocusConstants->bFullSearchStep = 27
HM5065MIPI_write_cmos_sensor(0x0774,0x01);//     // AutoFocusConstants->uwFineThreshold MSB = 330 
HM5065MIPI_write_cmos_sensor(0x0775,0x4a);//     // AutoFocusConstants->uwFineThreshold LSB 
HM5065MIPI_write_cmos_sensor(0x0777,0x00);//     // AutoFocusConstants->uwBacklightThreshold MSB = 69
HM5065MIPI_write_cmos_sensor(0x0778,0x45);//     // AutoFocusConstants->uwBacklightThreshold LSB 
HM5065MIPI_write_cmos_sensor(0x0779,0x00);//     // AutoFocusConstants->uwMotionBlurInRatio MSB = 2
HM5065MIPI_write_cmos_sensor(0x077A,0x02);//     // AutoFocusConstants->uwMotionBlurInRatio LSB
HM5065MIPI_write_cmos_sensor(0x077D,0x01);//     // AutoFocusConstants->bMaxNumberContinuouslyInstableTime = 1
HM5065MIPI_write_cmos_sensor(0x077E,0x03);//     // AutoFocusConstants->bMaxNumberContinuouslyStableFrame = 3
HM5065MIPI_write_cmos_sensor(0x0783,0x10);//     // AutoFocusConstants->bLightGap = 10 
HM5065MIPI_write_cmos_sensor(0x0785,0x14);//     // AutoFocusConstants->uwDeltaValue = 20
HM5065MIPI_write_cmos_sensor(0x0788,0x04);//     // AutoFocusConstants->bMinNumberMacroRegion = 4 mk add

HM5065MIPI_write_cmos_sensor(0x0846,0x06);//     // AutoFocusHeuristicConstants->bHighToMaxFMShiftFactor = 6
HM5065MIPI_write_cmos_sensor(0x0847,0x05);//     // AutoFocusHeuristicConstants->bLowToHighFMShiftFactor = 5


//**********************************************
// MIPI CSI settings - 14May2012
//**********************************************
HM5065MIPI_write_cmos_sensor(0xC41A,0x05);//     // TEST_LP_TX (clock slew rate);
HM5065MIPI_write_cmos_sensor(0xC423,0x11);//     // TEST_LP_TX_SLEW_RATE_DL1
HM5065MIPI_write_cmos_sensor(0xC427,0x11);//     // TEST_LP_TX_SLEW_RATE_DL2
HM5065MIPI_write_cmos_sensor(0x300B,0x09);//     // esc_clk_div (clk_sys div by 10);

//**********************************************
// CMD_RUN
//**********************************************

HM5065MIPI_write_cmos_sensor(0x0010,0x01);//     // CMD_RUN

//**********************************************
// AV
//**********************************************
mDELAY(200);		// Sleep... Important !

// Set horizontal and vertical offsets to zero
HM5065MIPI_write_cmos_sensor(0x4708,0x00);//  av2x2_h_offset
HM5065MIPI_write_cmos_sensor(0x4709,0x00);//  LSB
HM5065MIPI_write_cmos_sensor(0x4710,0x00);//  av2x2_v_offset {0x4710 & 11} are correct!
HM5065MIPI_write_cmos_sensor(0x4711,0x00);//  LSB

#endif
		
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIInitialSetting function:\n ");
		
} 
/*****************************************************************
* FUNCTION
*    HM5065MIPIPreviewSetting
*
* DESCRIPTION
*    This function config Preview setting related registers of CMOS sensor.
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void HM5065MIPIPreviewSetting_SVGA(void)
{
	//;HM5065MIPI 1280x960,30fps
	//56Mhz, 224Mbps/Lane, 2Lane.
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIPreviewSetting_SVGA function:\n ");
	
	HM5065MIPI_write_cmos_sensor(0x0142,0x00);//open AE
	//HM5065MIPI_write_cmos_sensor(0x01A4,0x00);//open awb
	
	//HM5065MIPI_write_cmos_sensor(0x0010,0x02);//sw reset
	//mdelay(200);
	//HM5065MIPI_write_cmos_sensor(0x00B5,0x01);//  01=>02	35Mhz  ;01: 70MHz;02:35MHz
	//HM5065MIPI_write_cmos_sensor(0x0030,0x11);//14


	HM5065MIPI_write_cmos_sensor(0x0040,0x01); 	//	binning mode and subsampling mode for frame rate 2*2 add 2? 4?
	HM5065MIPI_write_cmos_sensor(0x0041,0x0A); 	//	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
	HM5065MIPI_write_cmos_sensor(0x0042,0x05); 	//05	X:800 0x500=1280,0x0320=800
	HM5065MIPI_write_cmos_sensor(0x0043,0x00+0x10+0x8/2); 	//00
	HM5065MIPI_write_cmos_sensor(0x0044,0x03); 	//03	Y:600 0x03c0=960,0x0258=600
	HM5065MIPI_write_cmos_sensor(0x0045,0xc0+0xc+0x8/2); 	//c0
	HM5065MIPI_write_cmos_sensor(0x0251,0x02);//BLC ON	
	//HM5065MIPI_write_cmos_sensor(0x0010,0x01);
	//mdelay(200);
	
	if(HM5065_run_test_pattern)
	{
		HM5065_run_test_pattern=0;
		HM5065SetTestPatternMode(1);
	}
	//HM5065MIPI_write_cmos_sensor(0x4202, 0x00);//	; open mipi stream
	HM5065MIPIWriteExtraShutter(HM5065MIPISensor.PreviewExtraShutter);
	spin_lock(&HM5065mipi_drv_lock);
	HM5065MIPISensor.SensorMode= SENSOR_MODE_PREVIEW;
	HM5065MIPISensor.IsPVmode = KAL_TRUE;
	HM5065MIPISensor.PreviewPclk= 560;	
	spin_unlock(&HM5065mipi_drv_lock);
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIPreviewSetting_SVGA function:\n ");
}
static void HM5065MIPIZSDPreviewSetting_QVGA(void)
{

}


/*************************************************************************
* FUNCTION
*     HM5065MIPIFullSizeCaptureSetting
*
* DESCRIPTION
*    This function config full size capture setting related registers of CMOS sensor.
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void HM5065MIPIFullSizeCaptureSetting(void)
{

	
		HM5065MIPI_write_cmos_sensor(0x0142,0x01);//Freeze AE
		//HM5065MIPI_write_cmos_sensor(0x01A4,0x04);//Freeze AWB	
		//HM5065MIPI_write_cmos_sensor(0x01A4,0x04);//Freeze AWB	
	
	HM5065MIPISensor.previewShutter = HM5065MIPIReadShutter();
		
		//HM5065MIPI_write_cmos_sensor(0x0010,0x02);
		//mdelay(200);
		//HM5065MIPI_write_cmos_sensor(0x00B5,0x01);//
		//HM5065MIPI_write_cmos_sensor(0x0030,0x11);	
		//mdelay(200);
		
		HM5065MIPI_write_cmos_sensor(0x0040,0x00); //Full size
		HM5065MIPI_write_cmos_sensor(0x0041,0x0A); //00:full size
		HM5065MIPI_write_cmos_sensor(0x0042,0x0A); //X:2592/2602
		HM5065MIPI_write_cmos_sensor(0x0043,0x20+0x8);
		HM5065MIPI_write_cmos_sensor(0x0044,0x07); //Y:1944/1954
		HM5065MIPI_write_cmos_sensor(0x0045,0x98+0x8);
		/*HM5065MIPI_write_cmos_sensor(0x0042,0x0A); //X:2592
		HM5065MIPI_write_cmos_sensor(0x0043,0x00);
		HM5065MIPI_write_cmos_sensor(0x0044,0x07); //Y:1944
		HM5065MIPI_write_cmos_sensor(0x0045,0x80);*/
		
		HM5065MIPI_write_cmos_sensor(0x0251,0x01);//BLC off 
		//HM5065MIPI_write_cmos_sensor(0x019C,0x4B);//50hz banding
		//HM5065MIPI_write_cmos_sensor(0x019D,0x20);
		//HM5065MIPI_write_cmos_sensor(0x0082,0x50);	//By Zeroy
	
		//HM5065MIPI_write_cmos_sensor(0x0010,0x02);
		// mdelay(200);
		//HM5065MIPI_write_cmos_sensor(0x0010,0x01);//Stream on
		//mdelay(200);


	
	if(HM5065_run_test_pattern)
	{
		HM5065_run_test_pattern=0;
		HM5065SetTestPatternMode(1);
	}
	//HM5065MIPI_write_cmos_sensor(0x4202, 0x00);//	; open mipi stream
	spin_lock(&HM5065mipi_drv_lock);
	HM5065MIPISensor.IsPVmode = KAL_FALSE;
	HM5065MIPISensor.CapturePclk= 800;	
	//HM5065MIPISensor.SensorMode= SENSOR_MODE_CAPTURE;
	spin_unlock(&HM5065mipi_drv_lock);
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIFullSizeCaptureSetting function:\n ");
}

static void HM5065MIPIFullSizeCaptureSetting_ZSD(void)
{

	
		//HM5065MIPI_write_cmos_sensor(0x0142,0x01);//Freeze AE// zsd , need open ae
		//HM5065MIPI_write_cmos_sensor(0x01A4,0x04);//Freeze AWB	
		//HM5065MIPI_write_cmos_sensor(0x01A4,0x04);//Freeze AWB	
	
		
		HM5065MIPI_write_cmos_sensor(0x0142,0x00);//Freeze AE
		HM5065MIPI_write_cmos_sensor(0x0010,0x02);
		//mdelay(200);
		//HM5065MIPI_write_cmos_sensor(0x00B5,0x01);
		//HM5065MIPI_write_cmos_sensor(0x0030,0x11);	
		//mdelay(200);

		HM5065MIPI_write_cmos_sensor(0x0040,0x00); //Full size
		HM5065MIPI_write_cmos_sensor(0x0041,0x0A); //00:full size
		HM5065MIPI_write_cmos_sensor(0x0042,0x0A); //X:2592/2602
		HM5065MIPI_write_cmos_sensor(0x0043,0x20+0x8);
		HM5065MIPI_write_cmos_sensor(0x0044,0x07); //Y:1944/1954
		HM5065MIPI_write_cmos_sensor(0x0045,0x98+0x8);

		//HM5065MIPI_write_cmos_sensor(0x0251,0x01);//BLC off 
		
		HM5065MIPI_write_cmos_sensor(0x0251,0x02);//BLC off 
		//HM5065MIPI_write_cmos_sensor(0x019C,0x4B);//50hz banding
		//HM5065MIPI_write_cmos_sensor(0x019D,0x20);
		//HM5065MIPI_write_cmos_sensor(0x0082,0x50);	//By Zeroy
	
		// HM5065MIPI_write_cmos_sensor(0x0010,0x02);
		// mdelay(200);
		HM5065MIPI_write_cmos_sensor(0x0010,0x01);//Stream on
		//mdelay(200);


	
	if(HM5065_run_test_pattern)
	{
		HM5065_run_test_pattern=0;
		HM5065SetTestPatternMode(1);
	}
	//HM5065MIPI_write_cmos_sensor(0x4202, 0x00);//	; open mipi stream
	spin_lock(&HM5065mipi_drv_lock);
	HM5065MIPISensor.IsPVmode = KAL_FALSE;
	HM5065MIPISensor.CapturePclk= 800;	
	//HM5065MIPISensor.SensorMode= SENSOR_MODE_CAPTURE;
	spin_unlock(&HM5065mipi_drv_lock);
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIFullSizeCaptureSetting function:\n ");
}

/*************************************************************************
* FUNCTION
*    HM5065MIPISetHVMirror
*
* DESCRIPTION
*    This function set sensor Mirror
*
* PARAMETERS
*    Mirror
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void HM5065MIPISetHVMirror(kal_uint8 Mirror)
{
    HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPISetHVMirror function:\n ");

	switch (Mirror)
	{
		case IMAGE_NORMAL:	
			HM5065MIPI_write_cmos_sensor(0x0083, 0x01); 
			HM5065MIPI_write_cmos_sensor(0x0084, 0x01); 
			break;
		case IMAGE_H_MIRROR:			 
			HM5065MIPI_write_cmos_sensor(0x0083, 0x01); 
			HM5065MIPI_write_cmos_sensor(0x0084, 0x00); 
			break;
		case IMAGE_V_MIRROR:			 
			HM5065MIPI_write_cmos_sensor(0x0083, 0x00); 
			HM5065MIPI_write_cmos_sensor(0x0084, 0x01); 
			break;
		case IMAGE_HV_MIRROR:						 
			HM5065MIPI_write_cmos_sensor(0x0083, 0x00); 
			HM5065MIPI_write_cmos_sensor(0x0084, 0x00); 
			break;
	}
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPISetHVMirror function:\n ");
	
}

void HM5065MIPI_Standby(void)
{
	//HM5065MIPI_write_cmos_sensor(0x3008,0x42);
}

void HM5065MIPI_Wakeup(void)
{
	//HM5065MIPI_write_cmos_sensor(0x3008,0x02);
}
/*************************************************************************
* FUNCTION
*   HM5065_FOCUS_OVT_AFC_Init
* DESCRIPTION
*   This function is to load micro code for AF function
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*************************************************************************/
static void HM5065_FOCUS_OVT_AFC_Init(void)
{}
/*************************************************************************
* FUNCTION
*   OV5640_FOCUS_OVT_AFC_Constant_Focus
* DESCRIPTION
*   GET af stauts
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*************************************************************************/	
static void HM5065_FOCUS_OVT_AFC_Constant_Focus(void)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065_FOCUS_OVT_AFC_Constant_Focus function:\n ");
	HM5065MIPI_write_cmos_sensor(0x0751,0x00);	
	HM5065MIPI_write_cmos_sensor(0x070A,0x01);
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065_FOCUS_OVT_AFC_Constant_Focus function:\n ");
}   
/*************************************************************************
* FUNCTION
*   OV5640_FOCUS_OVT_AFC_Single_Focus
* DESCRIPTION
*   GET af stauts
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*************************************************************************/	
static void HM5065_FOCUS_OVT_AFC_Single_Focus()
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065_FOCUS_OVT_AFC_Single_Focus function:\n ");

    //HM5065MIPISENSORDB("HM5065_FOCUS_Single_Focus\n");
    HM5065MIPI_write_cmos_sensor(0x0751,0x00);	
    HM5065MIPI_write_cmos_sensor(0x070A,0x03);
	mDELAY(200);
    HM5065MIPI_write_cmos_sensor(0x070B,0x01);;
	mDELAY(200);
    HM5065MIPI_write_cmos_sensor(0x070B,0x02);
	mDELAY(200);
    //HM5065MIPISENSORDB("after single focus  \n");
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065_FOCUS_OVT_AFC_Single_Focus function:\n ");
}
/*************************************************************************
* FUNCTION
*   OV5640_FOCUS_OVT_AFC_Pause_Focus
* DESCRIPTION
*   GET af stauts
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*************************************************************************/	
static void HM5065_FOCUS_OVT_AFC_Pause_Focus()
{

}
static void HM5065_FOCUS_Get_AF_Max_Num_Focus_Areas(UINT32 *pFeatureReturnPara32)
{ 	  
    *pFeatureReturnPara32 = 1;    
    HM5065MIPISENSORDB(" *pFeatureReturnPara32 = %d\n",  *pFeatureReturnPara32);	
}

static void HM5065_FOCUS_Get_AE_Max_Num_Metering_Areas(UINT32 *pFeatureReturnPara32)
{ 	
    HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065_FOCUS_Get_AE_Max_Num_Metering_Areas function:\n ");
    *pFeatureReturnPara32 = 1;    
    HM5065MIPISENSORDB(" *pFeatureReturnPara32 = %d\n",  *pFeatureReturnPara32);
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065_FOCUS_Get_AE_Max_Num_Metering_Areas function:\n ");
}
static void HM5065_FOCUS_OVT_AFC_Touch_AF(UINT32 x,UINT32 y)
{
	 /*HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065_FOCUS_OVT_AFC_Touch_AF function:\n ");
	 int x_view,y_view;
     int x_tmp,y_tmp;
     if(x<1)
     {
               x_view=1;
     }
     else if(x>79)
     {
               x_view=79;
     }
     else
     {
               x_view= x;
     }
     
     if(y<1)
     {
               y_view=1;
     }
     else if(y>59)
     {
               y_view=59;
     }
     else
     {
               y_view= y;
     }
	 HM5065MIPISENSORDB("[HM5065MIPI]AF x_view=%d,y_view=%d\n",x_view, y_view);
     //HM5065MIPI_write_cmos_sensor(0x3024,x_view);
     //HM5065MIPI_write_cmos_sensor(0x3025,y_view);   
     x_tmp = HM5065MIPI_read_cmos_sensor(0x3024);
	 y_tmp = HM5065MIPI_read_cmos_sensor(0x3025);
	 HM5065MIPISENSORDB("[HM5065MIPI]AF x_tmp1=%d,y_tmp1=%d\n",x_tmp, y_tmp);
     HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065_FOCUS_OVT_AFC_Touch_AF function:\n ");*/
}

#define FACE_LC 			0x0714
#define FACE_START_XH 	0x0715
#define FACE_START_XL 	0x0716
#define FACE_SIZE_XH  	0x0717
#define FACE_SIZE_XL	 0x0718
#define FACE_START_YH	 0x0719
#define FACE_START_YL	 0x071A
#define FACE_SIZE_YH	 0x071B
#define FACE_SIZE_YL 	0x071C

static void HM5065_FOCUS_Set_AF_Window(UINT32 zone_addr)
{//update global zone
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065_FOCUS_Set_AF_Window function:\n ");
	UINT32 FD_XS;
	UINT32 FD_YS;   
	UINT32 x0, y0, x1, y1;
	UINT32 pvx0, pvy0, pvx1, pvy1;
	UINT32 linenum, rownum;
	UINT32 AF_pvx, AF_pvy;
	UINT32* zone = (UINT32*)zone_addr;
	x0 = *zone;
	y0 = *(zone + 1);
	x1 = *(zone + 2);
	y1 = *(zone + 3);   
	FD_XS = *(zone + 4);
	FD_YS = *(zone + 5);

	HM5065MIPISENSORDB("Set_AF_Window x0=%d,y0=%d,x1=%d,y1=%d,FD_XS=%d,FD_YS=%d\n",
	x0, y0, x1, y1, FD_XS, FD_YS);  

	if(CurrentScenarioId == MSDK_SCENARIO_ID_CAMERA_ZSD )
	{
		HM5065_mapMiddlewaresizePointToPreviewsizePoint(x0,y0,FD_XS,FD_YS,&pvx0, &pvy0, ZSD_PRV_W, ZSD_PRV_H);
		HM5065_mapMiddlewaresizePointToPreviewsizePoint(x1,y1,FD_XS,FD_YS,&pvx1, &pvy1, ZSD_PRV_W, ZSD_PRV_H);  
	}  
	else
	{
		HM5065_mapMiddlewaresizePointToPreviewsizePoint(x0,y0,FD_XS,FD_YS,&pvx0, &pvy0, PRV_W, PRV_H);
		HM5065_mapMiddlewaresizePointToPreviewsizePoint(x1,y1,FD_XS,FD_YS,&pvx1, &pvy1, PRV_W, PRV_H);  
	}
		
	HM5065MIPISENSORDB("[HM5065MIPI]AF pvx0=%d,pvy0=%d\n",pvx0, pvy0);
	HM5065MIPISENSORDB("[HM5065MIPI]AF pvx1=%d,pvy1=%d\n",pvx1, pvy1);
/*
		HM5065MIPI_write_cmos_sensor(0x0808,0x01);
		HM5065MIPI_write_cmos_sensor(0x0809,0x00); 
		HM5065MIPI_write_cmos_sensor(0x080a,0x00);
		HM5065MIPI_write_cmos_sensor(0x080b,0x00);  
		HM5065MIPI_write_cmos_sensor(0x080c,0x00);
		HM5065MIPI_write_cmos_sensor(0x080d,0x00);
		HM5065MIPI_write_cmos_sensor(0x080e,0x00); 

		HM5065MIPI_write_cmos_sensor(0x0751,0x00);	  
		HM5065MIPI_write_cmos_sensor(FACE_LC,0x01);//add abc enable//bit 0: AF  bit1: touch AE
*/
#if 1//def	AE_and_AF_FACE
		//HM5065MIPI_write_cmos_sensor(FACE_LC,0x03);//add abc disable//bit 0: AF  bit1: touch AE
#endif

	if((pvx0==pvx1)&&(pvy0==pvy1))
	{
		HM5065MIPI_write_cmos_sensor(0x0808,0x01);
		HM5065MIPI_write_cmos_sensor(0x0809,0x01); 
		HM5065MIPI_write_cmos_sensor(0x080a,0x01);
		HM5065MIPI_write_cmos_sensor(0x080b,0x01);	
		HM5065MIPI_write_cmos_sensor(0x080c,0x01);
		HM5065MIPI_write_cmos_sensor(0x080d,0x01);
		HM5065MIPI_write_cmos_sensor(0x080e,0x01); 
		
		HM5065MIPI_write_cmos_sensor(0x0751,0x00);	  
		HM5065MIPI_write_cmos_sensor(FACE_LC,0x00);//add abc enable//bit 0: AF	bit1: touch AE
/*
       	HM5065MIPI_write_cmos_sensor(FACE_START_XH, (pvx0-160)>>8);
		HM5065MIPI_write_cmos_sensor(FACE_START_XL, (pvx0-160)&0xff);

		HM5065MIPI_write_cmos_sensor(FACE_START_YH, (pvy0-160)>>8);
		HM5065MIPI_write_cmos_sensor(FACE_START_YL, (pvy0-160)&0xff);		

		HM5065MIPISENSORDB("HM5065_FOCUS_Set_AF_Center Window_to_IC\n");

		HM5065MIPI_write_cmos_sensor(FACE_SIZE_XH,0x01);
		HM5065MIPI_write_cmos_sensor(FACE_SIZE_XL,0x40);
		HM5065MIPI_write_cmos_sensor(FACE_SIZE_YH,0x01);
		HM5065MIPI_write_cmos_sensor(FACE_SIZE_YL,0x40);
*/
		HM5065MIPISENSORDB("HM5065_FOCUS_Set_ normal AF_Center Window_to_IC\n");
		
    }
	else{
		HM5065MIPI_write_cmos_sensor(0x0808,0x01);
		HM5065MIPI_write_cmos_sensor(0x0809,0x00); 
		HM5065MIPI_write_cmos_sensor(0x080a,0x00);
		HM5065MIPI_write_cmos_sensor(0x080b,0x00);  
		HM5065MIPI_write_cmos_sensor(0x080c,0x00);
		HM5065MIPI_write_cmos_sensor(0x080d,0x00);
		HM5065MIPI_write_cmos_sensor(0x080e,0x00); 

		HM5065MIPI_write_cmos_sensor(0x0751,0x00);	  
		HM5065MIPI_write_cmos_sensor(FACE_LC,0x01);

#if 1
		HM5065MIPI_write_cmos_sensor(FACE_START_XH, pvx0>>8);
		HM5065MIPI_write_cmos_sensor(FACE_START_XL, pvx0&0xff);

		HM5065MIPI_write_cmos_sensor(FACE_START_YH, pvy0>>8);
		HM5065MIPI_write_cmos_sensor(FACE_START_YL, pvy0&0xff);		

		HM5065MIPISENSORDB("HM5065_FOCUS_Set_AF_Touch Window_to_IC\n");

		HM5065MIPI_write_cmos_sensor(FACE_SIZE_XH,(pvx1-pvx0)>>8);
		HM5065MIPI_write_cmos_sensor(FACE_SIZE_XL,(pvx1-pvx0)&0xff);
		HM5065MIPI_write_cmos_sensor(FACE_SIZE_YH,(pvy1-pvy0)>>8);
		HM5065MIPI_write_cmos_sensor(FACE_SIZE_YL,(pvy1-pvy0)&0xff);
#endif
#if 0//test fix window
		HM5065MIPI_write_cmos_sensor(FACE_START_XH, 0x00);
		HM5065MIPI_write_cmos_sensor(FACE_START_XL, 0x10);

		HM5065MIPI_write_cmos_sensor(FACE_START_YH, 0x00);
		HM5065MIPI_write_cmos_sensor(FACE_START_YL, 0x10);		

		HM5065MIPISENSORDB("HM5065_FOCUS_Set_AF_Touch Window_to_IC\n");

		HM5065MIPI_write_cmos_sensor(FACE_SIZE_XH,0x00);
		HM5065MIPI_write_cmos_sensor(FACE_SIZE_XL,0xA0);
		HM5065MIPI_write_cmos_sensor(FACE_SIZE_YH,0x00);
		HM5065MIPI_write_cmos_sensor(FACE_SIZE_YL,0xA0);

#endif}
	}

	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065_FOCUS_Set_AF_Window function:\n ");
}
static void HM5065_FOCUS_Get_AF_Macro(UINT32 *pFeatureReturnPara32)
{

    HM5065MIPISENSORDB("HM5065_FOCUS_Get_AF_Macro\n");
	//lens rang: 0--0x3ff
	HM5065MIPI_write_cmos_sensor(0x0751,0x00);	
	HM5065MIPI_write_cmos_sensor(0x070A,0x00);//disable af
    HM5065MIPI_write_cmos_sensor(0x0700,0x00);//position h
    HM5065MIPI_write_cmos_sensor(0x0701,0x05);//position l
	HM5065MIPI_write_cmos_sensor(0x070C,0x00);//fix len to 5
	mDELAY(100);
    HM5065MIPI_write_cmos_sensor(0x070C,0x07);//fix len to 5

    *pFeatureReturnPara32 = 0;
}
static void HM5065_FOCUS_Get_AF_Inf(UINT32 * pFeatureReturnPara32)
{

    HM5065MIPISENSORDB("HM5065_FOCUS_Get_AF_Inf\n");
    HM5065MIPI_write_cmos_sensor(0x0751,0x00);	
	HM5065MIPI_write_cmos_sensor(0x070A,0x00);
    HM5065MIPI_write_cmos_sensor(0x0700,0x03);
    HM5065MIPI_write_cmos_sensor(0x0701,0xFF);
	HM5065MIPI_write_cmos_sensor(0x070C,0x00);
	mDELAY(100);
    HM5065MIPI_write_cmos_sensor(0x070C,0x07);
	printk("HM5065_FOCUS_Get_AF_Inf: AF POS %02x%02x.\n", HM5065MIPI_read_cmos_sensor(0x0700), HM5065MIPI_read_cmos_sensor
			(0x0701));

    *pFeatureReturnPara32 = 0;
}
/*************************************************************************
//此函数变量未定义,请根据编译定义数据变量.
//prview 1280*960 
//16的整数倍 ; n*16*80/1280
//16的整数倍 ; n*16*60/960
//touch_x  为对应preview的横坐标[0-1280]
//touch_y  为对应preview的纵坐标[0-960]

*************************************************************************/ 
static UINT32 HM5065_FOCUS_Move_to(UINT32 a_u2MovePosition)//??how many bits for ov3640??
{
}
/*************************************************************************
* FUNCTION
*   OV5640_FOCUS_OVT_AFC_Get_AF_Status
* DESCRIPTION
*   GET af stauts
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*************************************************************************/                        
static void HM5065_FOCUS_OVT_AFC_Get_AF_Status(UINT32 *pFeatureReturnPara32)
{
	UINT32 state_af=0;
	UINT32 i=0;
	*pFeatureReturnPara32 = SENSOR_AF_IDLE;
	//state_af = HM5065MIPI_read_cmos_sensor(0x07ae);
	//state_3029 = HM5065MIPI_read_cmos_sensor(0x3029);
	mDELAY(1);

	do
	{
	
		state_af = HM5065MIPI_read_cmos_sensor(0x07ae);
		//mDELAY(1);
		if (state_af==1)
		{
			*pFeatureReturnPara32 = SENSOR_AF_FOCUSED;  

			
			af_pos_h = HM5065MIPI_read_cmos_sensor(0x06F0);
			af_pos_l = HM5065MIPI_read_cmos_sensor(0x06F1);
			HM5065MIPISENSORDB("[HM5065]brant read current af pos: %02x%02x.\n", af_pos_h, af_pos_l);

			break;
		}
		else if(state_af==0)
		{
		
					*pFeatureReturnPara32 = SENSOR_AF_FOCUSING;
/*			switch (state_af)
			{
				case 0x70:
					*pFeatureReturnPara32 = SENSOR_AF_IDLE;
					break;
				case 0x00:
					*pFeatureReturnPara32 = SENSOR_AF_FOCUSING;
					break;
				case 0x10:
					*pFeatureReturnPara32 = SENSOR_AF_FOCUSED;
					break;
				case 0x20:
					*pFeatureReturnPara32 = SENSOR_AF_FOCUSED;
					break;
				default:
					*pFeatureReturnPara32 = SENSOR_AF_SCENE_DETECTING; 
					break;
			}                                 */ 
		} 
		else
		{
			*pFeatureReturnPara32 = SENSOR_AF_ERROR;
		}
		i++;
		if(i>10)
			break;
	}while(1);
	
    HM5065MIPISENSORDB("HM5065_AF_Status =%x\n",state_af);
	
}

/*************************************************************************
* FUNCTION
*   OV5640_FOCUS_OVT_AFC_Cancel_Focus
* DESCRIPTION
*   cancel af 
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*************************************************************************/     
static void HM5065_FOCUS_OVT_AFC_Cancel_Focus()
{
#if 1
	HM5065MIPI_write_cmos_sensor(0x0751,0x01);	

/*
	HM5065MIPI_write_cmos_sensor(0x070A,0x00);  
  	HM5065MIPI_write_cmos_sensor(0x0700,0x03);
	HM5065MIPI_write_cmos_sensor(0x0701,0xFF);
	HM5065MIPI_write_cmos_sensor(0x070C,0x00);
	HM5065MIPI_write_cmos_sensor(0x070C,0x07); 
 */
//	HM5065MIPI_write_cmos_sensor(FACE_LC,0x00);//disable
//	mDELAY(150);    
	//mDELAY(100);
#else
	HM5065MIPI_write_cmos_sensor(0x070A,0x00);

	HM5065MIPI_write_cmos_sensor(0x0734, af_pos_h & 0xFF);
	HM5065MIPI_write_cmos_sensor(0x0735, af_pos_l & 0xFF);

	printk("[HM5065]get af_status brant read target af pos1: %02x %02x.\n", HM5065MIPI_read_cmos_sensor(0x0700), HM5065MIPI_read_cmos_sensor(0x0701));

	HM5065MIPI_write_cmos_sensor(0x070C,0x00);
	HM5065MIPI_write_cmos_sensor(0x070C,0x05);
//	mdelay(200);
#endif
    HM5065MIPISENSORDB("HM5065_Cancel FOCUS\n");

}

/*************************************************************************
* FUNCTION
*   HM5065WBcalibattion
* DESCRIPTION
*   color calibration
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*************************************************************************/	
static void HM5065WBcalibattion(kal_uint32 color_r_gain,kal_uint32 color_b_gain)
{
/*		kal_uint32 color_r_gain_w = 0;
		kal_uint32 color_b_gain_w = 0;
		HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065WBcalibattion function:\n ");
		kal_uint8 temp = HM5065MIPI_read_cmos_sensor(0x350b); 
		
		if(temp>=0xb0)
		{	
			color_r_gain_w=color_r_gain*97/100;																																														
			color_b_gain_w=color_b_gain*99/100;  
		}
		else if (temp>=0x70)
		{
			color_r_gain_w=color_r_gain *97/100;																																														
			color_b_gain_w=color_b_gain*99/100;
		}
		else if (temp>=0x30)
		{
			color_r_gain_w=color_r_gain*98/100;																																														
			color_b_gain_w=color_b_gain*99/100;
		}
		else
		{
			color_r_gain_w=color_r_gain*98/100;																																														
			color_b_gain_w=color_b_gain*99/100; 
		}																																																																						
		//HM5065MIPI_write_cmos_sensor(0x3400,(color_r_gain_w & 0xff00)>>8);																																														
		//HM5065MIPI_write_cmos_sensor(0x3401,color_r_gain_w & 0xff); 			
		//HM5065MIPI_write_cmos_sensor(0x3404,(color_b_gain_w & 0xff00)>>8);																																														
		//HM5065MIPI_write_cmos_sensor(0x3405,color_b_gain_w & 0xff); 
		HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065WBcalibattion function:\n ");*/
}	
/*************************************************************************
* FUNCTION
*	HM5065MIPIOpen
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
UINT32 HM5065MIPIOpen(void)
{
	volatile signed int i;
	kal_uint16 sensor_id = 0;
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPIOpen function:\n ");
	//HM5065MIPI_write_cmos_sensor(0x3103,0x11);
	//HM5065MIPI_write_cmos_sensor(0x3008,0x82);
    mDELAY(10);
	for(i=0;i<3;i++)
	{
		sensor_id = (HM5065MIPI_read_cmos_sensor(0x0000) << 8) | HM5065MIPI_read_cmos_sensor(0x0001);
		HM5065MIPISENSORDB("HM5065MIPI READ ID :%x",sensor_id);
		if(sensor_id != HM5065_YUV_SENSOR_ID)
		{
			return ERROR_SENSOR_CONNECT_FAIL;
		}
	}
	HM5065MIPIinitalvariable();
	HM5065MIPIInitialSetting();
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIOpen function:\n ");
	return ERROR_NONE;
}	/* HM5065MIPIOpen() */

/*************************************************************************
* FUNCTION
*	HM5065MIPIClose
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
UINT32 HM5065MIPIClose(void)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW HM5065MIPIClose\n ");
  //CISModulePowerOn(FALSE);
	return ERROR_NONE;
}	/* HM5065MIPIClose() */
/*************************************************************************
* FUNCTION
*	HM5065MIPIPreview
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
UINT32 HM5065MIPIPreview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPIPreview function:\n ");
	kal_uint32 zsdshutter = 0;
#if 0
	switch(CurrentScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			   HM5065MIPISensor.zsd_flag=1;
			   HM5065MIPIPreviewSetting_SVGA();
			   HM5065MIPIFullSizeCaptureSetting();	
			   zsdshutter=HM5065MIPISensor.PreviewShutter*2;
			//   HM5065MIPIWriteExpShutter(zsdshutter);
			   break;
		default:
			   HM5065MIPIPreviewSetting_SVGA();
			   HM5065MIPISensor.zsd_flag=0;
			//   HM5065MIPIWriteExpShutter(HM5065MIPISensor.PreviewShutter);
			   break;
	}
	#endif

	if(sensor_config_data->SensorOperationMode==MSDK_SENSOR_OPERATION_MODE_VIDEO){
		HM5065MIPISensor.VideoMode= KAL_TRUE;  // MPEG4 Encode Mode
	}else{
		HM5065MIPISensor.VideoMode = KAL_FALSE;  
	}
	
	//HM5065MIPI_write_cmos_sensor(0x0010,0x02);
	HM5065MIPIPreviewSetting_SVGA();

	//HM5065MIPI_write_cmos_sensor(0x0128,0x02);//close AE
	//HM5065MIPIWriteShutter(HM5065MIPISensor.PreviewShutter);
	//HM5065MIPIWriteSensorGain(HM5065MIPISensor.PreviewGain);
	HM5065MIPISENSORDB("[HM5065MIPI]HM5065MIPIPreview previewShutter=%x:\n ", HM5065MIPISensor.PreviewShutter);	
	//HM5065MIPI_write_cmos_sensor(0x0128,0x00);//open AE
	
	//HM5065MIPI_write_cmos_sensor(0x0142,0x00);//open AE
    //mDELAY(200);
    //HM5065MIPI_write_cmos_sensor(0x0010,0x01);
	
//#ifdef BIRD_HM5065_HV_MIRROR
	sensor_config_data->SensorImageMirror=IMAGE_HV_MIRROR;
//#else
	//sensor_config_data->SensorImageMirror=IMAGE_NORMAL;
//#endif

	HM5065MIPISetHVMirror(sensor_config_data->SensorImageMirror);

	
	//HM5065_FOCUS_OVT_AFC_Init();
	//HM5065MIPI_set_AE_mode(KAL_TRUE);
	//HM5065MIPI_set_AWB_mode(KAL_TRUE);
	//mDELAY(30);	
	HM5065MIPI_night_mode(HM5065MIPISensor.NightMode);
	//if(HM5065MIPISensor.NightMode)
	//	mDELAY(1000);
	//else
	if( (HM5065MIPISensor.PreviewShutter==0x191e) && (HM5065MIPISensor.PreviewGain==0x4200))//max shutter and gain
		{
			mDELAY(1000);
			HM5065MIPISENSORDB("[HM5065MIPI]HM5065MIPIPreview previewShutter=%d:\n ", HM5065MIPISensor.PreviewShutter); 
		}
	else
		mDELAY(200);
		
	//if(HM5065MIPISensor.NightMode)
		
	//HM5065_FOCUS_OVT_AFC_Constant_Focus();
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIPreview function:\n ");	
	return ERROR_NONE ;
	
}	/* HM5065MIPIPreview() */
BOOL HM5065MIPI_set_param_exposure_for_HDR(UINT16 para)
{
    kal_uint32 totalGain = 0, exposureTime = 0;
	
	HM5065MIPISENSORDB("[HM5065MIPI]HM5065MIPI_set_param_exposure_for_HDR para=%d:\n ", para);
    if (0 == HM5065MIPISensor.manualAEStart)
    {       
        HM5065MIPI_set_AE_mode(KAL_FALSE);//Manual AE enable
        spin_lock(&HM5065mipi_drv_lock);	
        HM5065MIPISensor.manualAEStart = 1;
		spin_unlock(&HM5065mipi_drv_lock);
    }
	totalGain = HM5065MIPISensor.currentAxDGain;
    exposureTime = HM5065MIPISensor.currentExposureTime;
	switch (para)
	{
	   case AE_EV_COMP_20:	//+2 EV
       case AE_EV_COMP_10:	// +1 EV
		   totalGain = totalGain<<1;
           exposureTime = exposureTime<<1;
           HM5065MIPISENSORDB("[4EC] HDR AE_EV_COMP_20\n");
		 break;
	   case AE_EV_COMP_00:	// +0 EV
           HM5065MIPISENSORDB("[4EC] HDR AE_EV_COMP_00\n");
		 break;
	   case AE_EV_COMP_n10:  // -1 EV
	   case AE_EV_COMP_n20:  // -2 EV
		   totalGain = totalGain >> 1;
           exposureTime = exposureTime >> 1;
           HM5065MIPISENSORDB("[4EC] HDR AE_EV_COMP_n20\n");
		 break;
	   default:
		 break;//return FALSE;
	}

	if(totalGain > 0xf0)
		totalGain = 0xf0;
    HM5065MIPIWriteShutter(totalGain);
	HM5065MIPIWriteShutter(exposureTime);
	return TRUE;
}
#if 0
UINT32 HM5065MIPICapture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	kal_uint32 shutter = 0;	
	kal_uint32 extshutter = 0;
	kal_uint32 color_r_gain = 0;
	kal_uint32 color_b_gain = 0;
	kal_uint32 readgain=0;
	UINT32 AF_status;
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPICapture function:\n ");

	//if(SENSOR_MODE_PREVIEW == HM5065MIPISensor.SensorMode )
	//{		
	//shutter=HM5065MIPIReadShutter();
	//extshutter=HM5065MIPIReadExtraShutter();
	//readgain=HM5065MIPIReadSensorGain();
	//spin_lock(&HM5065mipi_drv_lock);
	//HM5065MIPISensor.PreviewShutter=shutter;
	//HM5065MIPISensor.PreviewExtraShutter=extshutter;	
	//HM5065MIPISensor.SensorGain=readgain;
	//spin_unlock(&HM5065mipi_drv_lock);	
	//HM5065MIPI_set_AE_mode(KAL_FALSE);
	//HM5065MIPI_set_AWB_mode(KAL_FALSE);
	//color_r_gain=((HM5065MIPI_read_cmos_sensor(0x3401)&0xFF)+((HM5065MIPI_read_cmos_sensor(0x3400)&0xFF)*256));  
	//color_b_gain=((HM5065MIPI_read_cmos_sensor(0x3405)&0xFF)+((HM5065MIPI_read_cmos_sensor(0x3404)&0xFF)*256)); 

	spin_lock(&HM5065mipi_drv_lock);	
	HM5065MIPISensor.SensorMode= SENSOR_MODE_CAPTURE;
	spin_unlock(&HM5065mipi_drv_lock);

	HM5065MIPIFullSizeCaptureSetting();
    //HM5065WBcalibattion(color_r_gain,color_b_gain);      
    //HM5065WBcalibattion(color_r_gain,color_b_gain);      
	HM5065MIPISENSORDB("[HM5065MIPI]Before shutter=%d:\n",shutter);
	/*if(HM5065MIPISensor.zsd_flag==0)
	{
		shutter = shutter*2;
	}*/
	if (SCENE_MODE_HDR == HM5065MIPISensor.sceneMode)
    {
		HM5065MIPISENSORDB("[HM5065MIPI] HDR capture, record shutter gain\n");
        spin_lock(&HM5065mipi_drv_lock);
        HM5065MIPISensor.currentExposureTime=HM5065MIPIReadShutter();
		//HM5065MIPISensor.currentextshutter=extshutter;
		HM5065MIPISensor.currentAxDGain=HM5065MIPIReadSensorGain();
		spin_unlock(&HM5065mipi_drv_lock);
    }
#if 0
		HM5065_FOCUS_OVT_AFC_Get_AF_Status(&AF_status);
		if(AF_status==SENSOR_AF_FOCUSED)
			HM5065_FOCUS_OVT_AFC_Cancel_Focus();
		else
			{
				msleep(60); //msleep(30);
				HM5065_FOCUS_OVT_AFC_Get_AF_Status(&AF_status);
				if(AF_status==SENSOR_AF_FOCUSED)
					HM5065_FOCUS_OVT_AFC_Cancel_Focus();
			}
	#else
	
		
	mDELAY(100);
			HM5065_FOCUS_OVT_AFC_Cancel_Focus();
	#endif

	mDELAY(100);
	//}
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPICapture function:\n ");
	return ERROR_NONE; 
}/* HM5065MIPICapture() */
	#else
UINT32 HM5065MIPICapture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	kal_uint32 shutter = 0;	
	kal_uint32 extshutter = 0;
	kal_uint32 color_r_gain = 0;
	kal_uint32 color_b_gain = 0;
	kal_uint32 readgain=0;
	UINT32 AF_status;
	UINT8 i=20;
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPICapture function:\n ");

/*	
	#if 1
	HM5065_FOCUS_OVT_AFC_Get_AF_Status(&AF_status);
	if(AF_status==SENSOR_AF_FOCUSED)
    	HM5065_FOCUS_OVT_AFC_Cancel_Focus();
	else
		{
			msleep(60);	//msleep(30);
			HM5065_FOCUS_OVT_AFC_Get_AF_Status(&AF_status);
			if(AF_status==SENSOR_AF_FOCUSED)
	    		HM5065_FOCUS_OVT_AFC_Cancel_Focus();
		}
	#endif
*/
	//if(SENSOR_MODE_PREVIEW == HM5065MIPISensor.SensorMode )
	//{		
	//shutter=HM5065MIPIReadShutter();
	//extshutter=HM5065MIPIReadExtraShutter();
	//readgain=HM5065MIPIReadSensorGain();
	//spin_lock(&HM5065mipi_drv_lock);
	//HM5065MIPISensor.PreviewShutter=shutter;
	//HM5065MIPISensor.PreviewExtraShutter=extshutter;	
	//HM5065MIPISensor.SensorGain=readgain;
	//spin_unlock(&HM5065mipi_drv_lock);	
	//HM5065MIPI_set_AE_mode(KAL_FALSE);
	//HM5065MIPI_set_AWB_mode(KAL_FALSE);
	//color_r_gain=((HM5065MIPI_read_cmos_sensor(0x3401)&0xFF)+((HM5065MIPI_read_cmos_sensor(0x3400)&0xFF)*256));  
	//color_b_gain=((HM5065MIPI_read_cmos_sensor(0x3405)&0xFF)+((HM5065MIPI_read_cmos_sensor(0x3404)&0xFF)*256)); 

	spin_lock(&HM5065mipi_drv_lock);	
	HM5065MIPISensor.SensorMode= SENSOR_MODE_CAPTURE;
	HM5065MIPISensor.PreviewShutter = HM5065MIPIReadShutter();
	HM5065MIPISensor.PreviewGain = HM5065MIPIReadSensorGain();
	spin_unlock(&HM5065mipi_drv_lock);
	HM5065MIPISENSORDB("[HM5065MIPI]HM5065MIPICapture previewShutter=%x, gain=%x \n ", HM5065MIPISensor.PreviewShutter, HM5065MIPISensor.PreviewGain);	

//
//
#if 1
//	HM5065_FOCUS_OVT_AFC_Single_Focus();
	
		do
		{			
			if(HM5065MIPI_read_cmos_sensor(0x07ae)==1)
			{	
				break;
			};
			mDELAY(10);	
		}
		while(i<20);
		af_pos_h = HM5065MIPI_read_cmos_sensor(0x06F0);
		af_pos_l = HM5065MIPI_read_cmos_sensor(0x06F1);	
//	
#endif
//HM5065_FOCUS_OVT_AFC_Cancel_Focus();
//213
///
	HM5065MIPIFullSizeCaptureSetting();
	mDELAY(100);
//

	#if 1
	HM5065MIPI_write_cmos_sensor(0x070A, 0x00);
	HM5065MIPI_write_cmos_sensor(0x0734, af_pos_h & 0xFF);
	HM5065MIPI_write_cmos_sensor(0x0735, af_pos_l & 0xFF);
	HM5065MIPI_write_cmos_sensor(0x070C, 0x00);
	mDELAY(200);
	HM5065MIPI_write_cmos_sensor(0x070C, 0x05);
	mDELAY(200);
	#endif
    //HM5065WBcalibattion(color_r_gain,color_b_gain);      
    //HM5065WBcalibattion(color_r_gain,color_b_gain);      
	HM5065MIPISENSORDB("[HM5065MIPI]Before shutter=%x:\n",shutter);
	/*if(HM5065MIPISensor.zsd_flag==0)
	{
		shutter = shutter*2;
	}*/
        HM5065MIPISensor.currentExposureTime=HM5065MIPIReadShutter();
		HM5065MIPISENSORDB("[HM5065MIPI]CaptureShutter=%d:\n ", HM5065MIPISensor.currentExposureTime);	
	if (SCENE_MODE_HDR == HM5065MIPISensor.sceneMode)
    {
		HM5065MIPISENSORDB("[HM5065MIPI] HDR capture, record shutter gain\n");
        spin_lock(&HM5065mipi_drv_lock);
        HM5065MIPISensor.currentExposureTime=HM5065MIPIReadShutter();
		//HM5065MIPISensor.currentextshutter=extshutter;
		HM5065MIPISensor.currentAxDGain=HM5065MIPIReadSensorGain();
		spin_unlock(&HM5065mipi_drv_lock);
		HM5065MIPISENSORDB("[HM5065MIPI]CaptureShutter=%d:\n ", HM5065MIPISensor.currentExposureTime);	
    }

	//mDELAY(100);
	//}
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPICapture function:\n ");
	return ERROR_NONE; 
}/* HM5065MIPICapture() */
#endif
UINT32 HM5065MIPIZSDPreview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	kal_uint32 shutter = 0;	
	kal_uint32 extshutter = 0;
	kal_uint32 color_r_gain = 0;
	kal_uint32 color_b_gain = 0;
	kal_uint32 readgain=0;
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPICapture function:\n ");

	//if(SENSOR_MODE_PREVIEW == HM5065MIPISensor.SensorMode )
	//{		
	/*shutter=HM5065MIPIReadShutter();
	extshutter=HM5065MIPIReadExtraShutter();
	readgain=HM5065MIPIReadSensorGain();
	spin_lock(&HM5065mipi_drv_lock);
	HM5065MIPISensor.PreviewShutter=shutter;
	HM5065MIPISensor.PreviewExtraShutter=extshutter;	
	HM5065MIPISensor.SensorGain=readgain;
	spin_unlock(&HM5065mipi_drv_lock);	*/
	//HM5065MIPI_set_AE_mode(KAL_FALSE);
	//HM5065MIPI_set_AWB_mode(KAL_FALSE);
	//color_r_gain=((HM5065MIPI_read_cmos_sensor(0x3401)&0xFF)+((HM5065MIPI_read_cmos_sensor(0x3400)&0xFF)*256));  
	//color_b_gain=((HM5065MIPI_read_cmos_sensor(0x3405)&0xFF)+((HM5065MIPI_read_cmos_sensor(0x3404)&0xFF)*256)); 
	HM5065MIPIFullSizeCaptureSetting_ZSD();
	spin_lock(&HM5065mipi_drv_lock);	
	HM5065MIPISensor.SensorMode= SENSOR_MODE_CAPTURE;
	spin_unlock(&HM5065mipi_drv_lock);
    //HM5065WBcalibattion(color_r_gain,color_b_gain);      
	HM5065MIPISENSORDB("[HM5065MIPI]Before shutter=%d:\n",shutter);
	/*if(HM5065MIPISensor.zsd_flag==0)
	{
		shutter = shutter*2;
	}*/
	if (SCENE_MODE_HDR == HM5065MIPISensor.sceneMode)
    {

        spin_lock(&HM5065mipi_drv_lock);
        HM5065MIPISensor.currentExposureTime=HM5065MIPIReadShutter();
		//HM5065MIPISensor.currentextshutter=extshutter;
		HM5065MIPISensor.currentAxDGain=HM5065MIPIReadSensorGain();
		spin_unlock(&HM5065mipi_drv_lock);
    }

	mDELAY(150);
	//}
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPICapture function:\n ");
	return ERROR_NONE; 
}/* HM5065MIPICapture() */


UINT32 HM5065MIPIGetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIGetResolution function:\n ");
	pSensorResolution->SensorPreviewWidth= HM5065MIPI_IMAGE_SENSOR_SVGA_WIDTH;
	pSensorResolution->SensorPreviewHeight= HM5065MIPI_IMAGE_SENSOR_SVGA_HEIGHT;
	pSensorResolution->SensorFullWidth= HM5065MIPI_IMAGE_SENSOR_QSXGA_WITDH; 
	pSensorResolution->SensorFullHeight= HM5065MIPI_IMAGE_SENSOR_QSXGA_HEIGHT;
	pSensorResolution->SensorVideoWidth= HM5065MIPI_IMAGE_SENSOR_SVGA_WIDTH; 
	pSensorResolution->SensorVideoHeight= HM5065MIPI_IMAGE_SENSOR_SVGA_HEIGHT;
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIGetResolution function:\n ");
	return ERROR_NONE;
}	/* HM5065MIPIGetResolution() */

UINT32 HM5065MIPIGetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,MSDK_SENSOR_INFO_STRUCT *pSensorInfo,MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIGetInfo function:\n ");
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			pSensorInfo->SensorPreviewResolutionX=HM5065MIPI_IMAGE_SENSOR_QSXGA_WITDH;//HM5065MIPI_IMAGE_SENSOR_SVGA_WIDTH;//HM5065MIPI_IMAGE_SENSOR_QSXGA_WITDH ;
			pSensorInfo->SensorPreviewResolutionY=HM5065MIPI_IMAGE_SENSOR_QSXGA_HEIGHT;//HM5065MIPI_IMAGE_SENSOR_SVGA_HEIGHT;//HM5065MIPI_IMAGE_SENSOR_QSXGA_HEIGHT ;
			//pSensorInfo->SensorPreviewResolutionX=HM5065MIPI_IMAGE_SENSOR_SVGA_WIDTH;//HM5065MIPI_IMAGE_SENSOR_SVGA_WIDTH;//HM5065MIPI_IMAGE_SENSOR_QSXGA_WITDH ;
			//pSensorInfo->SensorPreviewResolutionY=HM5065MIPI_IMAGE_SENSOR_SVGA_HEIGHT;//HM5065MIPI_IMAGE_SENSOR_SVGA_HEIGHT;//HM5065MIPI_IMAGE_SENSOR_QSXGA_HEIGHT ;
			pSensorInfo->SensorCameraPreviewFrameRate=10;
			break;
		default:
			pSensorInfo->SensorPreviewResolutionX=HM5065MIPI_IMAGE_SENSOR_SVGA_WIDTH ;
			pSensorInfo->SensorPreviewResolutionY=HM5065MIPI_IMAGE_SENSOR_SVGA_HEIGHT ;
			pSensorInfo->SensorCameraPreviewFrameRate=30;
			break;
	}		 		
	pSensorInfo->SensorFullResolutionX= HM5065MIPI_IMAGE_SENSOR_QSXGA_WITDH;
	pSensorInfo->SensorFullResolutionY= HM5065MIPI_IMAGE_SENSOR_QSXGA_HEIGHT;
	//pSensorInfo->SensorCameraPreviewFrameRate=30;
	pSensorInfo->SensorVideoFrameRate=30;
	pSensorInfo->SensorStillCaptureFrameRate=5;
	pSensorInfo->SensorWebCamCaptureFrameRate=15;
	pSensorInfo->SensorResetActiveHigh=FALSE;
	pSensorInfo->SensorResetDelayCount=4;
	pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_YUYV;

#ifdef BIRD_HM6065_MIPI_MODE		
	pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;	
	pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_HIGH;  
	pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
#else
	pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW; 
	pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;	
	pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;	
	pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_HIGH;
#endif
	////
	pSensorInfo->SensorInterruptDelayLines = 2;
#ifdef BIRD_HM6065_MIPI_MODE		
	pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_MIPI;
#else
	pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_PARALLEL;
#endif
	pSensorInfo->CaptureDelayFrame = 4;
	pSensorInfo->PreviewDelayFrame = 6; 
	pSensorInfo->VideoDelayFrame = 3; 		
	pSensorInfo->SensorMasterClockSwitch = 0; 
	pSensorInfo->YUVAwbDelayFrame = 3;
	pSensorInfo->YUVEffectDelayFrame= 3; 
	pSensorInfo->AEShutDelayFrame= 0;
 	pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;   		
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			pSensorInfo->SensorClockFreq=24;
			pSensorInfo->SensorClockDividCount=	3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
			pSensorInfo->SensorGrabStartX = HM5065MIPI_PV_GRAB_START_X; 
			pSensorInfo->SensorGrabStartY = HM5065MIPI_PV_GRAB_START_Y;   
			pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_2_LANE;			
			pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0; 
			pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount = 4; 
			pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
			pSensorInfo->SensorWidthSampling = 0; 
			pSensorInfo->SensorHightSampling = 0;  	
			pSensorInfo->SensorPacketECCOrder = 1;		
			break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			pSensorInfo->SensorClockFreq=24;
			pSensorInfo->SensorClockDividCount= 3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
			pSensorInfo->SensorGrabStartX = HM5065MIPI_FULL_GRAB_START_X; 
			pSensorInfo->SensorGrabStartY = HM5065MIPI_FULL_GRAB_START_Y;             
			pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_2_LANE;			
			pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0; 
			pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount =4; 
			pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0; 
			pSensorInfo->SensorWidthSampling = 0; 
			pSensorInfo->SensorHightSampling = 0;
			pSensorInfo->SensorPacketECCOrder = 1;
			break;
		default:
			pSensorInfo->SensorClockFreq=24;
			pSensorInfo->SensorClockDividCount= 3;
			pSensorInfo->SensorClockRisingCount=0;
			pSensorInfo->SensorClockFallingCount=2;
			pSensorInfo->SensorPixelClockCount=3;
			pSensorInfo->SensorDataLatchCount=2;
			pSensorInfo->SensorGrabStartX = HM5065MIPI_PV_GRAB_START_X; 
			pSensorInfo->SensorGrabStartY = HM5065MIPI_PV_GRAB_START_Y; 			
			pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_2_LANE;			
			pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0; 
			pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount = 4; 
			pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
			pSensorInfo->SensorWidthSampling = 0;
			pSensorInfo->SensorHightSampling = 0;	
			pSensorInfo->SensorPacketECCOrder = 1;
		  break;
	}
	memcpy(pSensorConfigData, &HM5065MIPISensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));	
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIGetInfo function:\n ");	
	return ERROR_NONE;
}	/* HM5065MIPIGetInfo() */

UINT32 HM5065MIPIControl(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	  HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPIControl function:%d \n ",ScenarioId);
	  spin_lock(&HM5065mipi_drv_lock);
	  CurrentScenarioId = ScenarioId;
	  spin_unlock(&HM5065mipi_drv_lock);
	  switch (ScenarioId)
	  {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			
			 HM5065MIPIPreview(pImageWindow, pSensorConfigData);
			 break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		//case MSDK_SCENARIO_ID_CAMERA_ZSD:
			 HM5065MIPICapture(pImageWindow, pSensorConfigData);
	  	     break;
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			 HM5065MIPIZSDPreview(pImageWindow, pSensorConfigData);
			break;
		default:
			return ERROR_INVALID_SCENARIO_ID;
	}
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIControl function:\n ");
	return ERROR_NONE;
}	/* HM5065MIPIControl() */

/* [TC] YUV sensor */	

BOOL HM5065MIPI_set_param_wb(UINT16 para)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPI_set_param_wb function:\n ");

	
	spin_lock(&HM5065mipi_drv_lock);
	HM5065MIPISensor.wb = para;
	spin_unlock(&HM5065mipi_drv_lock);

    switch (para)
    {
      //  case AWB_MODE_OFF:
			//HM5065MIPI_write_cmos_sensor(0x01A4,0x04);
        //    break;

        case AWB_MODE_AUTO:
			HM5065MIPI_write_cmos_sensor(0x01A0,0x01);//awb enable
            break;

        case AWB_MODE_CLOUDY_DAYLIGHT: //cloudy(13000K)
			HM5065MIPI_write_cmos_sensor(0x01A0,0x03);//MWB awb disable
			HM5065MIPI_write_cmos_sensor(0x01A1,0xef);//Rgain ff
			HM5065MIPI_write_cmos_sensor(0x01A2,0x60);//Ggain 40
			HM5065MIPI_write_cmos_sensor(0x01A3,0x00);//Bgain
            break;

        case AWB_MODE_DAYLIGHT: //sunny(7000K)
			HM5065MIPI_write_cmos_sensor(0x01A0,0x03);//MWB
			HM5065MIPI_write_cmos_sensor(0x01A1,0xC0);//Rgain
			HM5065MIPI_write_cmos_sensor(0x01A2,0x40);//Ggain
			HM5065MIPI_write_cmos_sensor(0x01A3,0x00);//Bgain
            break;

        case AWB_MODE_INCANDESCENT: //office(5000K)
			HM5065MIPI_write_cmos_sensor(0x01A0,0x03);//MWB
			HM5065MIPI_write_cmos_sensor(0x01A1,0xB0);//Rgain
			HM5065MIPI_write_cmos_sensor(0x01A2,0x40);//Ggain
			HM5065MIPI_write_cmos_sensor(0x01A3,0x20);//Bgain
            break;

        case AWB_MODE_TUNGSTEN: //home(2800K)
			HM5065MIPI_write_cmos_sensor(0x01A0,0x03);//MWB
			HM5065MIPI_write_cmos_sensor(0x01A1,0x90);//Rgain
			HM5065MIPI_write_cmos_sensor(0x01A2,0x40);//Ggain
			HM5065MIPI_write_cmos_sensor(0x01A3,0x40);//Bgain
            break;

        case AWB_MODE_FLUORESCENT://(4000K)
			HM5065MIPI_write_cmos_sensor(0x01A0,0x03);//MWB
			HM5065MIPI_write_cmos_sensor(0x01A1,0xA0);//Rgain
			HM5065MIPI_write_cmos_sensor(0x01A2,0x40);//Ggain
			HM5065MIPI_write_cmos_sensor(0x01A3,0x30);//Bgain
            break;

        default:
            break;
           // return FALSE;
    }
  //  return TRUE;


	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_set_param_wb function:\n ");
       return TRUE;
} /* HM5065MIPI_set_param_wb */
void HM5065MIPI_set_contrast(UINT16 para)
{   
    HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPI_set_contrast function:\n ");
    switch (para)
    {
        case ISP_CONTRAST_LOW:
             HM5065MIPI_write_cmos_sensor(0x0080,0x5c);
             break; 
        case ISP_CONTRAST_HIGH:
             HM5065MIPI_write_cmos_sensor(0x0080,0x7c);
             break; 
        case ISP_CONTRAST_MIDDLE:
             HM5065MIPI_write_cmos_sensor(0x0080,0x6c);
        default:
             break; 

    }
    HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_set_contrast function:\n ");
    return;
}

void HM5065MIPI_set_brightness(UINT16 para)
{
    HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPI_set_brightness function:\n ");

		switch (para)
		{
			case ISP_BRIGHT_LOW:
				  HM5065MIPI_write_cmos_sensor(0x0130,0xf7);
				 break; 
			case ISP_BRIGHT_HIGH:  
				  HM5065MIPI_write_cmos_sensor(0x0130,0x08);
				 break; 
			case ISP_BRIGHT_MIDDLE:
				  HM5065MIPI_write_cmos_sensor(0x0130,0x00);
			default:	
				 break; 
		}


    HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_set_brightness function:\n ");
    return;
}
void HM5065MIPI_set_saturation(UINT16 para)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPI_set_saturation function:\n ");
    switch (para)
    {
        case ISP_SAT_HIGH:
             HM5065MIPI_write_cmos_sensor(0x0081 ,0x78);

             break; 
        case ISP_SAT_LOW:
             HM5065MIPI_write_cmos_sensor(0x0081 ,0x38);
             break; 
        case ISP_SAT_MIDDLE:
        default:
             HM5065MIPI_write_cmos_sensor(0x0081 ,0x58);
             break; 
    }	

	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_set_saturation function:\n ");
     return;
}
void HM5065MIPI_set_scene_mode(UINT16 para)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPI_set_scene_mode function:%d\n ",para);
	spin_lock(&HM5065mipi_drv_lock);
	HM5065MIPISensor.sceneMode=para;
	spin_unlock(&HM5065mipi_drv_lock);
	

    switch (para)
    { 

		case SCENE_MODE_NIGHTSCENE:
				
          	HM5065MIPI_night_mode(KAL_TRUE); 
			HM5065MIPI_set_param_wb(HM5065MIPISensor.wb);
			break;
        case SCENE_MODE_PORTRAIT:
			HM5065MIPI_night_mode(KAL_FALSE);
			//HM5065MIPI_write_cmos_sensor(0x004C, 0x08);
			//HM5065MIPI_write_cmos_sensor(0x006C, 0x08);
			//HM5065MIPI_write_cmos_sensor(0x0080, 0x6C);
			//HM5065MIPI_write_cmos_sensor(0x0081, 0x58);
			HM5065MIPI_write_cmos_sensor(0x00E8, 0x01);
			HM5065MIPI_write_cmos_sensor(0x00ED, 0x0c);
			HM5065MIPI_write_cmos_sensor(0x0128, 0x00);
			//HM5065MIPI_write_cmos_sensor(0x0130, 0x00);
			//HM5065MIPI_write_cmos_sensor(0x0143, 0x5F);
			//HM5065MIPI_write_cmos_sensor(0x0144, 0x0D);
			//HM5065MIPI_write_cmos_sensor(0x015E, 0x40);
			//HM5065MIPI_write_cmos_sensor(0x015F, 0x00);
			HM5065MIPI_write_cmos_sensor(0x01A0, 0x01);
			//HM5065MIPI_write_cmos_sensor(0x02C2, 0x00);
			//HM5065MIPI_write_cmos_sensor(0x02C3, 0xC0);
			
			 		 
            break;
        case SCENE_MODE_LANDSCAPE:
		
			HM5065MIPI_night_mode(KAL_FALSE);
			//HM5065MIPI_write_cmos_sensor(0x004C, 0x10);
			//HM5065MIPI_write_cmos_sensor(0x006C, 0x10);
			//HM5065MIPI_write_cmos_sensor(0x0080, 0x78);
			//HM5065MIPI_write_cmos_sensor(0x0081, 0x68);
			HM5065MIPI_write_cmos_sensor(0x00E8, 0x01);
			HM5065MIPI_write_cmos_sensor(0x00ED, 0x0c);
			HM5065MIPI_write_cmos_sensor(0x0128, 0x00);
			//HM5065MIPI_write_cmos_sensor(0x0130, 0x00);
			//HM5065MIPI_write_cmos_sensor(0x0143, 0x5F);
			//HM5065MIPI_write_cmos_sensor(0x0144, 0x0D);
			//HM5065MIPI_write_cmos_sensor(0x015E, 0x40);
			//HM5065MIPI_write_cmos_sensor(0x015F, 0x00);
			HM5065MIPI_write_cmos_sensor(0x01A0, 0x01);
			//HM5065MIPI_write_cmos_sensor(0x02C2, 0x00);
			//HM5065MIPI_write_cmos_sensor(0x02C3, 0xC0);			 
             break;
        case SCENE_MODE_SUNSET:
			HM5065MIPI_night_mode(KAL_FALSE);
			//HM5065MIPI_write_cmos_sensor(0x004C, 0x05);
			//HM5065MIPI_write_cmos_sensor(0x006C, 0x05);
			//HM5065MIPI_write_cmos_sensor(0x0080, 0x6C);
			//HM5065MIPI_write_cmos_sensor(0x0081, 0x58);
			HM5065MIPI_write_cmos_sensor(0x00E8, 0x01);
			HM5065MIPI_write_cmos_sensor(0x00ED, 0x0c);
			HM5065MIPI_write_cmos_sensor(0x0128, 0x00);
			//HM5065MIPI_write_cmos_sensor(0x0130, 0xFC);
			//HM5065MIPI_write_cmos_sensor(0x0143, 0x5F);
			//HM5065MIPI_write_cmos_sensor(0x0144, 0x0D);
			//HM5065MIPI_write_cmos_sensor(0x015E, 0x40);
			//HM5065MIPI_write_cmos_sensor(0x015F, 0x00); 		  
			HM5065MIPI_write_cmos_sensor(0x01A0, 0x03);//;WB_Cloudy	 
			HM5065MIPI_write_cmos_sensor(0x01A1, 0xE0);//;	
			HM5065MIPI_write_cmos_sensor(0x01A2, 0x40);//;	
			HM5065MIPI_write_cmos_sensor(0x01A3, 0x00);//;	
			//HM5065MIPI_write_cmos_sensor(0x02C2, 0x00);
			//HM5065MIPI_write_cmos_sensor(0x02C3, 0xC0);					 
            break;
        case SCENE_MODE_SPORTS:
  
			HM5065MIPI_night_mode(KAL_FALSE);

			//HM5065MIPI_write_cmos_sensor(0x004C, 0x08);
			//HM5065MIPI_write_cmos_sensor(0x006C, 0x08);
			//HM5065MIPI_write_cmos_sensor(0x0080, 0x6C);
			//HM5065MIPI_write_cmos_sensor(0x0081, 0x58);
			HM5065MIPI_write_cmos_sensor(0x00E8, 0x01);
			HM5065MIPI_write_cmos_sensor(0x00ED, 0x0f);//0x1e
			HM5065MIPI_write_cmos_sensor(0x0128, 0x00);
			//HM5065MIPI_write_cmos_sensor(0x0130, 0x00);
			//HM5065MIPI_write_cmos_sensor(0x0143, 0x5C);
			//HM5065MIPI_write_cmos_sensor(0x0144, 0x09);
			//HM5065MIPI_write_cmos_sensor(0x015E, 0x40);
			//HM5065MIPI_write_cmos_sensor(0x015F, 0x00);
			HM5065MIPI_write_cmos_sensor(0x01A0, 0x01);
			//HM5065MIPI_write_cmos_sensor(0x02C2, 0x00);
			//HM5065MIPI_write_cmos_sensor(0x02C3, 0xC0);           		 
            break;
      case SCENE_MODE_HDR:
	  	
			HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW SCENE_MODE_HDR\n ");
        /*      if (1 == HM5065MIPISensor.manualAEStart)
            {
                HM5065MIPI_set_AE_mode(KAL_TRUE);//Manual AE disable
                spin_lock(&HM5065mipi_drv_lock);
            	HM5065MIPISensor.manualAEStart = 0;
                HM5065MIPISensor.currentExposureTime = 0;
                HM5065MIPISensor.currentAxDGain = 0;
				spin_unlock(&HM5065mipi_drv_lock);
            }
            */
            break;
        case SCENE_MODE_OFF:
        default:
	
			HM5065MIPI_set_param_wb(HM5065MIPISensor.wb);
          	HM5065MIPI_night_mode(KAL_FALSE); 		 	   
			break;
        //default:
		//	return KAL_FALSE;
         //   break;
    }

/*	if ((
		(SCENE_MODE_OFF == HM5065MIPISensor.sceneMode) 
		|| (SCENE_MODE_NORMAL == HM5065MIPISensor.sceneMode)
		|| (SCENE_MODE_NIGHTSCENE == HM5065MIPISensor.sceneMode)
		//|| (SCENE_MODE_AUTO == HM5065MIPISensor.sceneMode)

		
		))
		HM5065MIPI_set_param_wb(HM5065MIPISensor.wb);*/


	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_set_scene_mode function:\n ");
	return;
}
void HM5065MIPI_set_iso(UINT16 para)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW HM5065MIPI_set_iso:%d\n ",para);

    spin_lock(&HM5065mipi_drv_lock);
    HM5065MIPISensor.isoSpeed = para;
    spin_unlock(&HM5065mipi_drv_lock);   
	 switch (para)
		{
			case AE_ISO_100:
				 //ISO 100
	HM5065MIPI_write_cmos_sensor(0x015C, 0x3e);
	HM5065MIPI_write_cmos_sensor(0x015D, 0x00);
	HM5065MIPI_write_cmos_sensor(0x015E, 0x3e);
	HM5065MIPI_write_cmos_sensor(0x015F, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C0, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C1, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C2, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C3, 0x00);
				 break; 
			case AE_ISO_200:
				 //ISO 200
	HM5065MIPI_write_cmos_sensor(0x015C, 0x3e);
	HM5065MIPI_write_cmos_sensor(0x015D, 0x00);
	HM5065MIPI_write_cmos_sensor(0x015E, 0x3e);
	HM5065MIPI_write_cmos_sensor(0x015F, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C0, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C1, 0x80);
	HM5065MIPI_write_cmos_sensor(0x02C2, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C3, 0x80);
				 break; 
			case AE_ISO_400:
				 //ISO 400
	HM5065MIPI_write_cmos_sensor(0x015C, 0x40);
	HM5065MIPI_write_cmos_sensor(0x015D, 0x00);
	HM5065MIPI_write_cmos_sensor(0x015E, 0x40);
	HM5065MIPI_write_cmos_sensor(0x015F, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C0, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C1, 0x80);
	HM5065MIPI_write_cmos_sensor(0x02C2, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C3, 0x80);
				 break; 
			default:
			case AE_ISO_AUTO:
				 //ISO Auto
	HM5065MIPI_write_cmos_sensor(0x015C, 0x40);
	HM5065MIPI_write_cmos_sensor(0x015D, 0x00);
	HM5065MIPI_write_cmos_sensor(0x015E, 0x40);
	HM5065MIPI_write_cmos_sensor(0x015F, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C0, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C1, 0xC0);
	HM5065MIPI_write_cmos_sensor(0x02C2, 0x00);
	HM5065MIPI_write_cmos_sensor(0x02C3, 0xC0);
				 break; 
		}	

    return;
}

BOOL HM5065MIPI_set_param_effect(UINT16 para)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPI_set_param_effect function:\n ");
	 

	    switch (para)
    {
        case MEFFECT_OFF:
			HM5065MIPI_write_cmos_sensor(0x0380,0x00);
			HM5065MIPI_write_cmos_sensor(0x0381,0x00);
			HM5065MIPI_write_cmos_sensor(0x0382,0x00);
			HM5065MIPI_write_cmos_sensor(0x0384,0x00);
#if 0
			HM5065MIPI_write_cmos_sensor(0x01A0,0x01);
			HM5065MIPI_write_cmos_sensor(0x01A1,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A2,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A3,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A5,0x3e);
			HM5065MIPI_write_cmos_sensor(0x01A6,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A7,0x3e);
			HM5065MIPI_write_cmos_sensor(0x01A8,0x00);	
			HM5065MIPI_write_cmos_sensor(0x0049,0x14);	
#endif
		//HM5065MIPI_set_scene_mode(HM5065MIPISensor.sceneMode);

		break;

        case MEFFECT_SEPIA:
			HM5065MIPI_write_cmos_sensor(0x0380,0x00);
			HM5065MIPI_write_cmos_sensor(0x0381,0x00);
			HM5065MIPI_write_cmos_sensor(0x0382,0x00);
			HM5065MIPI_write_cmos_sensor(0x0384,0x06);
#if 0
			HM5065MIPI_write_cmos_sensor(0x01A0,0x03);
			HM5065MIPI_write_cmos_sensor(0x01A1,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A2,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A3,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A5,0x3e);
			HM5065MIPI_write_cmos_sensor(0x01A6,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A7,0x3e);
			HM5065MIPI_write_cmos_sensor(0x01A8,0x00);
			HM5065MIPI_write_cmos_sensor(0x0049,0x14);
#endif			
            break;

        case MEFFECT_NEGATIVE:
			HM5065MIPI_write_cmos_sensor(0x0380,0x01);
			HM5065MIPI_write_cmos_sensor(0x0381,0x00);
			HM5065MIPI_write_cmos_sensor(0x0382,0x00);
			HM5065MIPI_write_cmos_sensor(0x0384,0x00);
#if 0
			HM5065MIPI_write_cmos_sensor(0x01A0,0x01);
			HM5065MIPI_write_cmos_sensor(0x01A1,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A2,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A3,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A5,0x3e);
			HM5065MIPI_write_cmos_sensor(0x01A6,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A7,0x3e);
			HM5065MIPI_write_cmos_sensor(0x01A8,0x00);	
			HM5065MIPI_write_cmos_sensor(0x0049,0x08);	
#endif
            break;

        case MEFFECT_SEPIAGREEN:
			HM5065MIPI_write_cmos_sensor(0x0380,0x00);
			HM5065MIPI_write_cmos_sensor(0x0381,0x00);
			HM5065MIPI_write_cmos_sensor(0x0382,0x00);
			HM5065MIPI_write_cmos_sensor(0x0384,0x03);
#if 0
			HM5065MIPI_write_cmos_sensor(0x01A0,0x03);
			HM5065MIPI_write_cmos_sensor(0x01A1,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A2,0xCF);
			HM5065MIPI_write_cmos_sensor(0x01A3,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A5,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A6,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A7,0x45);
			HM5065MIPI_write_cmos_sensor(0x01A8,0x00);	
			HM5065MIPI_write_cmos_sensor(0x0049,0x14);	
#endif
            break;

        case MEFFECT_SEPIABLUE:

			HM5065MIPI_write_cmos_sensor(0x0380,0x00);
			HM5065MIPI_write_cmos_sensor(0x0381,0x00);
			HM5065MIPI_write_cmos_sensor(0x0382,0x00);
			HM5065MIPI_write_cmos_sensor(0x0384,0x08);
#if 0
			HM5065MIPI_write_cmos_sensor(0x01A0,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A1,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A2,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A3,0x3F);
			HM5065MIPI_write_cmos_sensor(0x01A5,0x1E);
			HM5065MIPI_write_cmos_sensor(0x01A6,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A7,0x3e);
			HM5065MIPI_write_cmos_sensor(0x01A8,0x00);
			HM5065MIPI_write_cmos_sensor(0x0049,0x14);	
#endif		
            break;
	case MEFFECT_MONO: //B&W
			HM5065MIPI_write_cmos_sensor(0x0380,0x00);
			HM5065MIPI_write_cmos_sensor(0x0381,0x00);
			HM5065MIPI_write_cmos_sensor(0x0382,0x00);
			HM5065MIPI_write_cmos_sensor(0x0384,0x05);
#if 0
			HM5065MIPI_write_cmos_sensor(0x01A0,0x03);
			HM5065MIPI_write_cmos_sensor(0x01A1,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A2,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A3,0x80);
			HM5065MIPI_write_cmos_sensor(0x01A5,0x3e);
			HM5065MIPI_write_cmos_sensor(0x01A6,0x00);
			HM5065MIPI_write_cmos_sensor(0x01A7,0x3e);
			HM5065MIPI_write_cmos_sensor(0x01A8,0x00);
			HM5065MIPI_write_cmos_sensor(0x0049,0x14);	
#endif
		break;

        default:
             return KAL_FALSE;
    }
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_set_param_effect function:\n ");
    return KAL_TRUE;
} /* HM5065MIPI_set_param_effect */

BOOL HM5065MIPI_set_param_banding(UINT16 para)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPI_set_param_banding function:\n ");
	switch (para)
    {
        case AE_FLICKER_MODE_50HZ:
        case AE_FLICKER_MODE_AUTO:
            default:
						spin_lock(&HM5065mipi_drv_lock);
						HM5065MIPI_Banding_setting = AE_FLICKER_MODE_50HZ;
						spin_unlock(&HM5065mipi_drv_lock);
						//HM5065MIPI_write_cmos_sensor(0x3c00,0x04);
						//HM5065MIPI_write_cmos_sensor(0x3c01,0x80);
		  	HM5065MIPI_write_cmos_sensor(0x0190,0x00);
			HM5065MIPI_write_cmos_sensor(0x019C,0x4B);
			HM5065MIPI_write_cmos_sensor(0x019D,0x20);
            break;
        case AE_FLICKER_MODE_60HZ:			
						spin_lock(&HM5065mipi_drv_lock);
						HM5065MIPI_Banding_setting = AE_FLICKER_MODE_60HZ;
						spin_unlock(&HM5065mipi_drv_lock);
		  	HM5065MIPI_write_cmos_sensor(0x0190,0x00);
			HM5065MIPI_write_cmos_sensor(0x019C,0x4B);
			HM5065MIPI_write_cmos_sensor(0x019D,0xC0);
						//HM5065MIPI_write_cmos_sensor(0x3c00,0x00);
						//HM5065MIPI_write_cmos_sensor(0x3c01,0x80);
            break;
                 return FALSE;
    }
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_set_param_banding function:\n ");
        return TRUE;
} /* HM5065MIPI_set_param_banding */

BOOL HM5065MIPI_set_param_exposure(UINT16 para)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPI_set_param_exposure function:\n ");
	HM5065MIPISENSORDB("[HM5065MIPI]para=%d:\n",para);
	//spin_lock(&HM5065mipi_drv_lock);
   if (SCENE_MODE_HDR == HM5065MIPISensor.sceneMode && 
    SENSOR_MODE_CAPTURE == HM5065MIPISensor.SensorMode)
   {
   	   //spin_unlock(&HM5065mipi_drv_lock);
    //   HM5065MIPI_set_param_exposure_for_HDR(para);
    //   return TRUE;
    
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW HDR enter HM5065MIPI_set_param_exposure function:\n ");
   }
   //spin_unlock(&HM5065mipi_drv_lock);
	switch (para)
    {	
       case AE_EV_COMP_20:	                   
				
				HM5065MIPI_write_cmos_sensor(0x0082,0x9A);
				HM5065MIPISENSORDB("[HM5065MIPI]EV +2 \n ");

				break;
		case AE_EV_COMP_10:	                   
		
				HM5065MIPI_write_cmos_sensor(0x0082,0x8A);

			  break;
		case AE_EV_COMP_00:

           	HM5065MIPI_write_cmos_sensor(0x0082,0x5A);
				HM5065MIPISENSORDB("[HM5065MIPI]EV 0 \n ");
				//HM5065MIPI_write_cmos_sensor(0x3a1f, 0x18);//	; control zone L  
			  break;
   		 case AE_EV_COMP_n10:
			 
			 HM5065MIPI_write_cmos_sensor(0x0082,0x2A);
			  break;
      	case AE_EV_COMP_n20:  // -2 EV

           	HM5065MIPI_write_cmos_sensor(0x0082,0x10);
				HM5065MIPISENSORDB("[HM5065MIPI]EV -2 \n ");
        	 break;
		default:
           	HM5065MIPI_write_cmos_sensor(0x0082,0x5A);
			break;//
						// return FALSE;
    }
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_set_param_exposure function:\n ");
    return TRUE;
} /* HM5065MIPI_set_param_exposure */
#if 0//afc
BOOL HM5065MIPI_set_param_afmode(UINT16 para)
{
    switch (para)
    {
			case AF_MODE_AFS:
				OV5640_FOCUS_OVT_AFC_Single_Focus();
			break;
			case AF_MODE_AFC:
				OV5640_FOCUS_OVT_AFC_Constant_Focus();
			break;            
      	default:
      	return FALSE;
    }
        return TRUE;
} /* HM5065MIPI_set_param_banding */
#endif
UINT32 HM5065MIPIYUVSensorSetting(FEATURE_ID iCmd, UINT32 iPara)
{
	HM5065MIPISENSORDB("HM5065MIPIYUVSensorSetting:iCmd=%d,iPara=%d, %d \n",iCmd, iPara);
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIYUVSensorSetting function:\n ");
	switch (iCmd) {
		case FID_SCENE_MODE:
			HM5065MIPI_set_scene_mode(iPara);
	    	break; 	    
		case FID_AWB_MODE:
				HM5065MIPI_set_param_wb(iPara);
			  break;
		case FID_COLOR_EFFECT:	    	    
				HM5065MIPI_set_param_effect(iPara);
		 	  break;
		case FID_AE_EV:   
				HM5065MIPI_set_param_exposure(iPara);
		    break;
		case FID_AE_FLICKER:    	    	    
				HM5065MIPI_set_param_banding(iPara);
		 	  break;
		case FID_AE_SCENE_MODE: 
				if (iPara == AE_MODE_OFF) 
				{
					spin_lock(&HM5065mipi_drv_lock);
		 			HM5065MIPI_AE_ENABLE = KAL_FALSE; 
					spin_unlock(&HM5065mipi_drv_lock);
        }
        else 
        {
					spin_lock(&HM5065mipi_drv_lock);
		 			HM5065MIPI_AE_ENABLE = KAL_TRUE; 
					spin_unlock(&HM5065mipi_drv_lock);
	     	}
				HM5065MIPI_set_AE_mode(HM5065MIPI_AE_ENABLE);
        break; 
		case FID_ISP_CONTRAST:
            HM5065MIPI_set_contrast(iPara);
            break;
        case FID_ISP_BRIGHT:
            HM5065MIPI_set_brightness(iPara);
            break;
        case FID_ISP_SAT:
            HM5065MIPI_set_saturation(iPara);
        break; 
    case FID_ZOOM_FACTOR:
   		    HM5065MIPISENSORDB("FID_ZOOM_FACTOR:%d\n", iPara); 	    
					spin_lock(&HM5065mipi_drv_lock);
	        zoom_factor = iPara; 
					spin_unlock(&HM5065mipi_drv_lock);
            break; 
		case FID_AE_ISO:
            HM5065MIPI_set_iso(iPara);
            break;
#if 0 //afc
		case FID_AF_MODE:
	    	 HM5065MIPI_set_param_afmode(iPara);
					break;     
#endif            
	  default:
		 	      break;
	}
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIYUVSensorSetting function:\n ");
	  return TRUE;
}   /* HM5065MIPIYUVSensorSetting */

UINT32 HM5065MIPIYUVSetVideoMode(UINT16 u2FrameRate)
{
	HM5065MIPISENSORDB("[HM5065MIPI]CONTROLFLOW enter HM5065MIPIYUVSetVideoMode function:\n ");
	//sHM5065MIPISensor.VideoMode == KAL_TRUE;
	spin_lock(&HM5065mipi_drv_lock);	
	HM5065MIPISensor.VideoMode= KAL_TRUE;
	spin_unlock(&HM5065mipi_drv_lock);

	if (u2FrameRate == 30)
	{
		//;HM5065MIPI 1280x960,30fps
		//56Mhz, 224Mbps/Lane, 2Lane.
		HM5065MIPISENSORDB("[HM5065MIPI]HM5065MIPIYUVSetVideoMode enter u2FrameRate == 30 setting  :\n ");	

		//HM5065MIPI_write_cmos_sensor(0x0040,0x01);	//	binning mode and subsampling mode for frame rate
		//		HM5065MIPI_write_cmos_sensor(0x0041,0x0A);	//	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
		//		HM5065MIPI_write_cmos_sensor(0x0042,0x05);	//05	X:800 0x500=1280,0x0320=800
		//			HM5065MIPI_write_cmos_sensor(0x0043,0x00);	//00
		//			HM5065MIPI_write_cmos_sensor(0x0044,0x03);	//03	Y:600 0x03c0=960,0x0258=600
		//			HM5065MIPI_write_cmos_sensor(0x0045,0xc0);	//c0
		
		
				HM5065MIPI_write_cmos_sensor(0x00E8,0x00);
				HM5065MIPI_write_cmos_sensor(0x00C8,0x00);
				HM5065MIPI_write_cmos_sensor(0x00C9,0x1F);//30fps
				HM5065MIPI_write_cmos_sensor(0x00CA,0x01);
		

		if(HM5065_run_test_pattern)
		{
			HM5065_run_test_pattern=0;
			HM5065SetTestPatternMode(1);
		}
		HM5065MIPISENSORDB("[HM5065MIPI]HM5065MIPIYUVSetVideoMode exit u2FrameRate == 30 setting  :\n ");
		}
    else if (u2FrameRate == 15)   
	{
		//;HM5065MIPI 1280x960,15fps
		//28Mhz, 112Mbps/Lane, 2Lane.
		HM5065MIPISENSORDB("[HM5065MIPI]HM5065MIPIYUVSetVideoMode enter u2FrameRate == 15 setting  :\n ");	
		//HM5065MIPI_write_cmos_sensor(0x0040,0x01); 	//	binning mode and subsampling mode for frame rate
		//	HM5065MIPI_write_cmos_sensor(0x0041,0x0A); 	//	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
		//		HM5065MIPI_write_cmos_sensor(0x0042,0x05); 	//05	X:800 0x500=1280,0x0320=800
		//		HM5065MIPI_write_cmos_sensor(0x0043,0x00); 	//00
		//		HM5065MIPI_write_cmos_sensor(0x0044,0x03); 	//03	Y:600 0x03c0=960,0x0258=600
		//	HM5065MIPI_write_cmos_sensor(0x0045, 0xC0); 	//

	
			HM5065MIPI_write_cmos_sensor(0x00E8,0x00);
			HM5065MIPI_write_cmos_sensor(0x00C8,0x00);
			HM5065MIPI_write_cmos_sensor(0x00C9,0x0F);//15fps
			HM5065MIPI_write_cmos_sensor(0x00CA,0x01);


		if(HM5065_run_test_pattern)
		{
			HM5065_run_test_pattern=0;
			HM5065SetTestPatternMode(1);
		}
		HM5065MIPISENSORDB("[HM5065MIPI]HM5065MIPIYUVSetVideoMode exit u2FrameRate == 15 setting  :\n ");
	}   
    else 
    {
        HM5065MIPISENSORDB("Wrong frame rate setting \n");
    } 
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIYUVSetVideoMode function:\n ");
    return TRUE; 
}

/**************************/
static void HM5065MIPIGetEvAwbRef(UINT32 pSensorAEAWBRefStruct)
{

	PSENSOR_AE_AWB_REF_STRUCT Ref = (PSENSOR_AE_AWB_REF_STRUCT)pSensorAEAWBRefStruct;
	Ref->SensorAERef.AeRefLV05Shutter=0x170c;
	Ref->SensorAERef.AeRefLV05Gain=0x30;
	Ref->SensorAERef.AeRefLV13Shutter=0x24e;
	Ref->SensorAERef.AeRefLV13Gain=0x10;
	Ref->SensorAwbGainRef.AwbRefD65Rgain=0x610;
	Ref->SensorAwbGainRef.AwbRefD65Bgain=0x448;
	Ref->SensorAwbGainRef.AwbRefCWFRgain=0x4e0;
	Ref->SensorAwbGainRef.AwbRefCWFBgain=0x5a0;
	
}

static void HM5065MIPIGetCurAeAwbInfo(UINT32 pSensorAEAWBCurStruct)
{

	PSENSOR_AE_AWB_CUR_STRUCT Info = (PSENSOR_AE_AWB_CUR_STRUCT)pSensorAEAWBCurStruct;
	Info->SensorAECur.AeCurShutter=HM5065MIPIReadShutter();
	Info->SensorAECur.AeCurGain=HM5065MIPIReadSensorGain() ;
	Info->SensorAwbGainCur.AwbCurRgain=((HM5065MIPI_read_cmos_sensor(0x3401)&&0xff)+((HM5065MIPI_read_cmos_sensor(0x3400)&&0xff)*256));
	Info->SensorAwbGainCur.AwbCurBgain=((HM5065MIPI_read_cmos_sensor(0x3405)&&0xff)+((HM5065MIPI_read_cmos_sensor(0x3404)&&0xff)*256));

}
UINT32 HM5065MIPIMaxFramerateByScenario(MSDK_SCENARIO_ID_ENUM scenarioId, MUINT32 frameRate) 
	{
		kal_uint32 pclk;
		kal_int16 dummyLine;
		kal_uint16 lineLength,frameHeight;
		HM5065MIPISENSORDB("HM5065MIPIMaxFramerateByScenario: scenarioId = %d, frame rate = %d\n",scenarioId,frameRate);
		HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIMaxFramerateByScenario function:\n ");
		switch (scenarioId) {
			case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
				pclk = 56000000;
				lineLength = HM5065MIPI_IMAGE_SENSOR_SVGA_WIDTH;
				frameHeight = (10 * pclk)/frameRate/lineLength;
				dummyLine = frameHeight - HM5065MIPI_IMAGE_SENSOR_SVGA_HEIGHT;
				if(dummyLine<0)
					dummyLine = 0;
				spin_lock(&HM5065mipi_drv_lock);
				HM5065MIPISensor.SensorMode= SENSOR_MODE_PREVIEW;
				HM5065MIPISensor.PreviewDummyLines = dummyLine;
				spin_unlock(&HM5065mipi_drv_lock);
				HM5065MIPISetDummy(HM5065MIPISensor.PreviewDummyPixels, dummyLine);			
				break;			
			case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
				pclk = 56000000;
				lineLength = HM5065MIPI_IMAGE_SENSOR_VIDEO_WITDH;
				frameHeight = (10 * pclk)/frameRate/lineLength;
				dummyLine = frameHeight - HM5065MIPI_IMAGE_SENSOR_VIDEO_HEIGHT;
				if(dummyLine<0)
					dummyLine = 0;
				//spin_lock(&HM5065mipi_drv_lock);
				//ov8825.sensorMode = SENSOR_MODE_VIDEO;
				//spin_unlock(&HM5065mipi_drv_lock);
				HM5065MIPISetDummy(0, dummyLine);			
				break;			
			case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			case MSDK_SCENARIO_ID_CAMERA_ZSD:			
				pclk = 90000000;
				lineLength = HM5065MIPI_IMAGE_SENSOR_QSXGA_WITDH;
				frameHeight = (10 * pclk)/frameRate/lineLength;
				dummyLine = frameHeight - HM5065MIPI_IMAGE_SENSOR_QSXGA_HEIGHT;
				if(dummyLine<0)
					dummyLine = 0;
				spin_lock(&HM5065mipi_drv_lock);
				HM5065MIPISensor.CaptureDummyLines = dummyLine;
				HM5065MIPISensor.SensorMode= SENSOR_MODE_CAPTURE;
				spin_unlock(&HM5065mipi_drv_lock);
				HM5065MIPISetDummy(HM5065MIPISensor.CaptureDummyPixels, dummyLine);			
				break;		
			case MSDK_SCENARIO_ID_CAMERA_3D_PREVIEW: //added
				break;
			case MSDK_SCENARIO_ID_CAMERA_3D_VIDEO:
				break;
			case MSDK_SCENARIO_ID_CAMERA_3D_CAPTURE: //added   
				break;		
			default:
				break;
		}	
		HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIMaxFramerateByScenario function:\n ");
		return ERROR_NONE;
	}
UINT32 HM5065MIPIGetDefaultFramerateByScenario(MSDK_SCENARIO_ID_ENUM scenarioId, MUINT32 *pframeRate) 
{
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPIGetDefaultFramerateByScenario function:\n ");
	switch (scenarioId) {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			 *pframeRate = 300;
			 break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			 *pframeRate = 150;
			break;		
        case MSDK_SCENARIO_ID_CAMERA_3D_PREVIEW: //added
        case MSDK_SCENARIO_ID_CAMERA_3D_VIDEO:
        case MSDK_SCENARIO_ID_CAMERA_3D_CAPTURE: //added   
			 *pframeRate = 300;
			break;		
		default:
			break;
	}
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIGetDefaultFramerateByScenario function:\n ");
	return ERROR_NONE;
}
void HM5065MIPI_get_AEAWB_lock(UINT32 *pAElockRet32, UINT32 *pAWBlockRet32)
{
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPI_get_AEAWB_lock function:\n ");
	*pAElockRet32 =1;
	*pAWBlockRet32=1;
	HM5065MIPISENSORDB("[HM5065MIPI]HM5065MIPI_get_AEAWB_lock,AE=%d,AWB=%d\n",*pAElockRet32,*pAWBlockRet32);
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_get_AEAWB_lock function:\n ");
}
void HM5065MIPI_GetDelayInfo(UINT32 delayAddr)
{
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPI_GetDelayInfo function:\n ");
	SENSOR_DELAY_INFO_STRUCT *pDelayInfo=(SENSOR_DELAY_INFO_STRUCT*)delayAddr;
	pDelayInfo->InitDelay=1;
	pDelayInfo->EffectDelay=3;
	pDelayInfo->AwbDelay=3;
	pDelayInfo->AFSwitchDelayFrame=50;
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_GetDelayInfo function:\n ");
}
void HM5065MIPI_AutoTestCmd(UINT32 *cmd,UINT32 *para)
{
	HM5065MIPISENSORDB("[HM5065MIPI]enter HM5065MIPI_AutoTestCmd function:\n ");
	switch(*cmd)
	{
		case YUV_AUTOTEST_SET_SHADDING:
			HM5065MIPISENSORDB("YUV_AUTOTEST_SET_SHADDING:para=%d\n",*para);
			break;
		case YUV_AUTOTEST_SET_GAMMA:
			HM5065MIPISENSORDB("YUV_AUTOTEST_SET_GAMMA:para=%d\n",*para);
			break;
		case YUV_AUTOTEST_SET_AE:
			HM5065MIPISENSORDB("YUV_AUTOTEST_SET_AE:para=%d\n",*para);
			break;
		case YUV_AUTOTEST_SET_SHUTTER:
			HM5065MIPISENSORDB("YUV_AUTOTEST_SET_SHUTTER:para=%d\n",*para);
			break;
		case YUV_AUTOTEST_SET_GAIN:
			HM5065MIPISENSORDB("YUV_AUTOTEST_SET_GAIN:para=%d\n",*para);
			break;
		case YUV_AUTOTEST_GET_SHUTTER_RANGE:
			*para=8228;
			break;
		default:
			HM5065MIPISENSORDB("YUV AUTOTEST NOT SUPPORT CMD:%d\n",*cmd);
			break;	
	}
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPI_AutoTestCmd function:\n ");
}
void HM5065MIPI_3ACtrl(ACDK_SENSOR_3A_LOCK_ENUM action)
{
   switch (action)
   {
      case SENSOR_3A_AE_LOCK:
          spin_lock(&HM5065mipi_drv_lock);
          HM5065MIPISensor.userAskAeLock = TRUE;
          spin_unlock(&HM5065mipi_drv_lock);
          HM5065MIPI_set_AE_mode(KAL_FALSE);
      break;
      case SENSOR_3A_AE_UNLOCK:
          spin_lock(&HM5065mipi_drv_lock);
          HM5065MIPISensor.userAskAeLock = FALSE;
          spin_unlock(&HM5065mipi_drv_lock);
          HM5065MIPI_set_AE_mode(KAL_TRUE);
      break;

      case SENSOR_3A_AWB_LOCK:
          spin_lock(&HM5065mipi_drv_lock);
          HM5065MIPISensor.userAskAwbLock = TRUE;
          spin_unlock(&HM5065mipi_drv_lock);
      //    HM5065MIPI_set_AWB_mode(KAL_FALSE);
      break;

      case SENSOR_3A_AWB_UNLOCK:
          spin_lock(&HM5065mipi_drv_lock);
          HM5065MIPISensor.userAskAwbLock = FALSE;
          spin_unlock(&HM5065mipi_drv_lock);
      //    HM5065MIPI_set_AWB_mode(KAL_TRUE);
      break;
      default:
      	break;
   }
   return;
}

UINT32 HM5065MIPIFeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
{
	UINT16 *pFeatureReturnPara16=(UINT16 *) pFeaturePara;
	UINT16 *pFeatureData16=(UINT16 *) pFeaturePara;
	UINT32 *pFeatureReturnPara32=(UINT32 *) pFeaturePara;
	UINT32 *pFeatureData32=(UINT32 *) pFeaturePara;
	MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData=(MSDK_SENSOR_CONFIG_STRUCT *) pFeaturePara;
	MSDK_SENSOR_REG_INFO_STRUCT *pSensorRegData=(MSDK_SENSOR_REG_INFO_STRUCT *) pFeaturePara;
	UINT32 Tony_Temp1 = 0;
	UINT32 Tony_Temp2 = 0;
	Tony_Temp1 = pFeaturePara[0];
	Tony_Temp2 = pFeaturePara[1];
	HM5065MIPISENSORDB("[HM5065MIPI][HM5065MIPIFeatureControl]feature id=%d \n",FeatureId);
	switch (FeatureId)
	{
		case SENSOR_FEATURE_GET_RESOLUTION:
			*pFeatureReturnPara16++=HM5065MIPI_IMAGE_SENSOR_QSXGA_WITDH;
			*pFeatureReturnPara16=HM5065MIPI_IMAGE_SENSOR_QSXGA_HEIGHT;
			*pFeatureParaLen=4;
			break;
		case SENSOR_FEATURE_GET_PERIOD:
			switch(CurrentScenarioId)
			{
				case MSDK_SCENARIO_ID_CAMERA_ZSD:
					*pFeatureReturnPara16++=HM5065MIPI_FULL_PERIOD_PIXEL_NUMS + HM5065MIPISensor.CaptureDummyPixels;
					*pFeatureReturnPara16=HM5065MIPI_FULL_PERIOD_LINE_NUMS + HM5065MIPISensor.CaptureDummyLines;
					*pFeatureParaLen=4;
					break;
				default:
					*pFeatureReturnPara16++=HM5065MIPI_PV_PERIOD_PIXEL_NUMS + HM5065MIPISensor.PreviewDummyPixels;
					*pFeatureReturnPara16=HM5065MIPI_PV_PERIOD_LINE_NUMS + HM5065MIPISensor.PreviewDummyLines;
					*pFeatureParaLen=4;
					break;
			}
			break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			switch(CurrentScenarioId)
			{
				case MSDK_SCENARIO_ID_CAMERA_ZSD:
					*pFeatureReturnPara32 = HM5065MIPISensor.ZsdturePclk * 1000 *100;	 //unit: Hz				
					*pFeatureParaLen=4;
					break;
				default:
					*pFeatureReturnPara32 = HM5065MIPISensor.PreviewPclk * 1000 *100;	 //unit: Hz
					*pFeatureParaLen=4;
					break;
			}
			break;
		case SENSOR_FEATURE_SET_ESHUTTER:
			break;
		case SENSOR_FEATURE_GET_EXIF_INFO:
            HM5065MIPIGetExifInfo(*pFeatureData32);
            break;
		case SENSOR_FEATURE_SET_NIGHTMODE:
			HM5065MIPI_night_mode((BOOL) *pFeatureData16);
			break;
		case SENSOR_FEATURE_SET_GAIN:
			break;
		case SENSOR_FEATURE_SET_FLASHLIGHT:
			break;
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			break;
		case SENSOR_FEATURE_SET_REGISTER:
			HM5065MIPI_write_cmos_sensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
			break;
		case SENSOR_FEATURE_GET_REGISTER:
			pSensorRegData->RegData = HM5065MIPI_read_cmos_sensor(pSensorRegData->RegAddr);
			break;
		case SENSOR_FEATURE_GET_CONFIG_PARA:
			memcpy(pSensorConfigData, &HM5065MIPISensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
			*pFeatureParaLen=sizeof(MSDK_SENSOR_CONFIG_STRUCT);
			break;
		case SENSOR_FEATURE_SET_CCT_REGISTER:
		case SENSOR_FEATURE_GET_CCT_REGISTER:
		case SENSOR_FEATURE_SET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_REGISTER_DEFAULT:
		case SENSOR_FEATURE_CAMERA_PARA_TO_SENSOR:
		case SENSOR_FEATURE_SENSOR_TO_CAMERA_PARA:
		case SENSOR_FEATURE_GET_GROUP_INFO:
		case SENSOR_FEATURE_GET_ITEM_INFO:
		case SENSOR_FEATURE_SET_ITEM_INFO:
		case SENSOR_FEATURE_GET_ENG_INFO:
			break;
		case SENSOR_FEATURE_GET_GROUP_COUNT:
            *pFeatureReturnPara32++=0;
            *pFeatureParaLen=4;	   
		    break; 
		case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
			*pFeatureReturnPara32=LENS_DRIVER_ID_DO_NOT_CARE;
			*pFeatureParaLen=4;
			break;
		case SENSOR_FEATURE_SET_TEST_PATTERN:            
			HM5065SetTestPatternMode((BOOL)*pFeatureData16);            
			break;
		case SENSOR_FEATURE_CHECK_SENSOR_ID:
			HM5065MIPI_GetSensorID(pFeatureData32);
			break;
		case SENSOR_FEATURE_GET_TEST_PATTERN_CHECKSUM_VALUE:
			*pFeatureReturnPara32=HM5065_TEST_PATTERN_CHECKSUM;
			*pFeatureParaLen=4;
			break;
		case SENSOR_FEATURE_SET_YUV_CMD:
			HM5065MIPIYUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_SET_YUV_3A_CMD:
            HM5065MIPI_3ACtrl((ACDK_SENSOR_3A_LOCK_ENUM)*pFeatureData32);
            break;
		case SENSOR_FEATURE_SET_VIDEO_MODE:
		    HM5065MIPIYUVSetVideoMode(*pFeatureData16);
		    break;
		case SENSOR_FEATURE_GET_EV_AWB_REF:
			HM5065MIPIGetEvAwbRef(*pFeatureData32);
			break;
		case SENSOR_FEATURE_GET_SHUTTER_GAIN_AWB_GAIN:
			HM5065MIPIGetCurAeAwbInfo(*pFeatureData32);			
			break;
		case SENSOR_FEATURE_SET_MAX_FRAME_RATE_BY_SCENARIO:
			HM5065MIPIMaxFramerateByScenario((MSDK_SCENARIO_ID_ENUM)*pFeatureData32,*(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_GET_DEFAULT_FRAME_RATE_BY_SCENARIO:
			HM5065MIPIGetDefaultFramerateByScenario((MSDK_SCENARIO_ID_ENUM)*pFeatureData32,(MUINT32 *)*(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_GET_AE_AWB_LOCK_INFO:
			HM5065MIPI_get_AEAWB_lock(*pFeatureData32, *(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_GET_DELAY_INFO:
			HM5065MIPISENSORDB("SENSOR_FEATURE_GET_DELAY_INFO\n");
			HM5065MIPI_GetDelayInfo(*pFeatureData32);
			break;
		case SENSOR_FEATURE_AUTOTEST_CMD:
			HM5065MIPISENSORDB("SENSOR_FEATURE_AUTOTEST_CMD\n");
			HM5065MIPI_AutoTestCmd(*pFeatureData32,*(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_INITIALIZE_AF:           
             break;
        case SENSOR_FEATURE_MOVE_FOCUS_LENS:
            HM5065_FOCUS_Move_to(*pFeatureData16);
            break;
        case SENSOR_FEATURE_GET_AF_STATUS:
            HM5065_FOCUS_OVT_AFC_Get_AF_Status(pFeatureReturnPara32);            
            *pFeatureParaLen=4;
            break;
        case SENSOR_FEATURE_GET_AF_INF:
            HM5065_FOCUS_Get_AF_Inf(pFeatureReturnPara32);
            *pFeatureParaLen=4;            
            break;
        case SENSOR_FEATURE_GET_AF_MACRO:
            HM5065_FOCUS_Get_AF_Macro(pFeatureReturnPara32);
            *pFeatureParaLen=4;            
            break;
		case SENSOR_FEATURE_CONSTANT_AF:
			HM5065_FOCUS_OVT_AFC_Constant_Focus();
			 break;
        case SENSOR_FEATURE_SET_AF_WINDOW:       
			HM5065_FOCUS_Set_AF_Window(*pFeatureData32);
            break;
        case SENSOR_FEATURE_SINGLE_FOCUS_MODE:
			HM5065_FOCUS_OVT_AFC_Single_Focus();
            break;	
        case SENSOR_FEATURE_CANCEL_AF:
            HM5065_FOCUS_OVT_AFC_Cancel_Focus();
            break;					
        case SENSOR_FEATURE_GET_AF_MAX_NUM_FOCUS_AREAS:
            HM5065_FOCUS_Get_AF_Max_Num_Focus_Areas(pFeatureReturnPara32);            
            *pFeatureParaLen=4;
            break;        
        case SENSOR_FEATURE_GET_AE_MAX_NUM_METERING_AREAS:
            HM5065_FOCUS_Get_AE_Max_Num_Metering_Areas(pFeatureReturnPara32);            
            *pFeatureParaLen=4;
            break;        
        case SENSOR_FEATURE_SET_AE_WINDOW:
            HM5065MIPISENSORDB("AE zone addr = 0x%x\n",*pFeatureData32);			
            HM5065_FOCUS_Set_AE_Window(*pFeatureData32);
            break; 
		default:
			HM5065MIPISENSORDB("HM5065MIPIFeatureControl:default \n");
			break;			
	}
	HM5065MIPISENSORDB("[HM5065MIPI]exit HM5065MIPIFeatureControl function:\n ");
	return ERROR_NONE;
}	/* HM5065MIPIFeatureControl() */

SENSOR_FUNCTION_STRUCT	SensorFuncHM5065MIPI=
{
	HM5065MIPIOpen,
	HM5065MIPIGetInfo,
	HM5065MIPIGetResolution,
	HM5065MIPIFeatureControl,
	HM5065MIPIControl,
	HM5065MIPIClose
};

UINT32 HM5065_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&SensorFuncHM5065MIPI;
	return ERROR_NONE;
}	/* SensorInit() */



