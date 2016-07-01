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
#include "HM5065yuv_Sensor.h"
#include "HM5065yuv_Camera_Sensor_para.h"
#include "HM5065yuv_CameraCustomized.h" 
#define HM5065YUV_DEBUG
#ifdef HM5065YUV_DEBUG
#define HM5065SENSORDB printk
#else
#define HM5065SENSORDB(x,...)
#endif
#define AE_and_AF_FACE
static DEFINE_SPINLOCK(HM5065_drv_lock);
static MSDK_SCENARIO_ID_ENUM CurrentScenarioId = MSDK_SCENARIO_ID_CAMERA_PREVIEW;
static kal_uint8 af_pos_h = 0;
static kal_uint8 af_pos_l = 0;


extern int iReadReg(u16 a_u2Addr , u8 * a_puBuff , u16 i2cId);
extern int iWriteReg(u16 a_u2Addr , u32 a_u4Data , u32 a_u4Bytes , u16 i2cId);
extern int iBurstWriteReg(u8 *pData, u32 bytes, u16 i2cId);
#define HM5065_write_cmos_sensor(addr, para) iWriteReg((u16) addr , (u32) para ,1,HM5065_WRITE_ID)
#define mDELAY(ms)  mdelay(ms)
#define HM5065_burst_write_cmos_sensor(pData, bytes)  iBurstWriteReg(pData, bytes, HM5065_WRITE_ID)

typedef enum
{
    PRV_W=1280,
    PRV_H=960,
		
	ZSD_PRV_W=2592,
	ZSD_PRV_H=1944
}PREVIEW_VIEW_SIZE;
kal_uint16 HM5065_read_cmos_sensor(kal_uint32 addr)
{
	kal_uint16 get_byte=0;
    iReadReg((u16) addr ,(u8*)&get_byte,HM5065_WRITE_ID);
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
	kal_uint32  PreviewExtraShutter;
	kal_uint32  SensorGain;
	kal_bool    	manualAEStart;
	kal_bool    	userAskAeLock;
    kal_bool    	userAskAwbLock;
	kal_uint32      currentExposureTime;
    kal_uint32      currentShutter;
	kal_uint32      currentextshutter;
    kal_uint32      currentAxDGain;
	kal_uint32  	sceneMode;
    unsigned char isoSpeed;
	unsigned char zsd_flag;
	HM5065_SENSOR_MODE SensorMode;

	UINT16 wb;
} HM5065Sensor;
/* Global Valuable */
static kal_uint32 zoom_factor = 0; 
static kal_int8 HM5065_DELAY_AFTER_PREVIEW = -1;
static kal_uint8 HM5065_Banding_setting = AE_FLICKER_MODE_50HZ; 
static kal_bool HM5065_AWB_ENABLE = KAL_TRUE; 
static kal_bool HM5065_AE_ENABLE = KAL_TRUE; 
MSDK_SENSOR_CONFIG_STRUCT HM5065SensorConfigData;
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
////burst mode 由于是16bit addr 和 8bit data，可以参考如下
#define USE_I2C_BURST_WRITE_HM5065
#ifdef USE_I2C_BURST_WRITE_HM5065
#define I2C_BUFFER_LEN_HM5065 254 //MAX data to send by MT6572 i2c dma mode is 255 bytes
#define BLOCK_I2C_DATA_WRITE_HM5065 iBurstWriteReg
#else
#define I2C_BUFFER_LEN_HM5065 8 // MT6572 i2s bus master fifo length is 8 bytes
#define BLOCK_I2C_DATA_WRITE_HM5065 iWriteRegI2C
#endif

// {addr, data} pair in para
// len is the total length of addr+data
// Using I2C multiple/burst write if the addr increase by 1
static kal_uint16 HM5065_table_write_cmos_sensor(kal_uint8* para, kal_uint32 len)
{
char puSendCmd[I2C_BUFFER_LEN_HM5065]; //at most 2 bytes address and 6 bytes data for multiple write. MTK i2c master has only 8bytes fifo.
kal_uint32 tosend, IDX;
kal_uint8 data;
kal_uint16 addr, addr_last;

tosend = 0;
IDX = 0;
while(IDX < len)
{
addr = ((para[IDX]<<8)&0xff00) | ((para[IDX+1]<<0)&0x00ff);//16bit addr

if (tosend == 0) // new (addr, data) to send
{
puSendCmd[tosend++] = (char)para[IDX];//addr
puSendCmd[tosend++] = (char)para[IDX+1];//addr
puSendCmd[tosend++] = (char)para[IDX+2];//data
IDX += 3;
addr_last = addr;
}
else if (addr == addr_last+1) // to multiple write the data to the incremental address
{
data = para[IDX+2];
puSendCmd[tosend++] = (char)data;
IDX += 3;
addr_last ++;
}
// to send out the data if the sen buffer is full or last data or to program to the address not incremental.
if (tosend == I2C_BUFFER_LEN_HM5065 || IDX == len || addr != addr_last)
{
BLOCK_I2C_DATA_WRITE_HM5065(puSendCmd , tosend, HM5065_WRITE_ID);
tosend = 0;
}
}
return 0;
}


void HM5065_printAE_1_ARRAY(void)
{
    UINT32 i;
    for(i=0; i<AE_SECTION_INDEX_MAX; i++)
    {
        HM5065SENSORDB("AE_1_ARRAY[%2d]=%d\n", i, AE_1_ARRAY[i]);
    }
}

void HM5065_printAE_2_ARRAY(void)
{
    UINT32 i, j;
    HM5065SENSORDB("\t\t");
    for(i=0; i<AE_VERTICAL_BLOCKS; i++)
    {
        HM5065SENSORDB("      line[%2d]", i);
    }
    printk("\n");
    for(j=0; j<AE_HORIZONTAL_BLOCKS; j++)
    {
        HM5065SENSORDB("\trow[%2d]", j);
        for(i=0; i<AE_VERTICAL_BLOCKS; i++)
        {
            //SENSORDB("AE_2_ARRAY[%2d][%2d]=%d\n", j,i,AE_2_ARRAY[j][i]);
            HM5065SENSORDB("  %7d", AE_2_ARRAY[j][i]);
        }
        HM5065SENSORDB("\n");
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
    HM5065SENSORDB("mapping middlware x[%d],y[%d], [%d X %d]\n\t\tto x[%d],y[%d],[%d X %d]\n ",
        mx, my, mw, mh, *pvx, *pvy, pvw, pvh);
}


void HM5065_calcLine(void)
{//line[5]
    UINT32 i;
    UINT32 step = PRV_W / AE_VERTICAL_BLOCKS;
    for(i=0; i<=AE_VERTICAL_BLOCKS; i++)
    {
        *(&line_coordinate[0]+i) = step*i;
        HM5065SENSORDB("line[%d]=%d\t",i, *(&line_coordinate[0]+i));
    }
    HM5065SENSORDB("\n");
}

void HM5065_vcalcRow(void)
{//row[5]
    UINT32 i;
    UINT32 step = PRV_H / AE_HORIZONTAL_BLOCKS;
    for(i=0; i<=AE_HORIZONTAL_BLOCKS; i++)
    {
        *(&row_coordinate[0]+i) = step*i;
        HM5065SENSORDB("row[%d]=%d\t",i,*(&row_coordinate[0]+i));
    }
    HM5065SENSORDB("\n");
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
    HM5065SENSORDB("PV point [%d, %d] to section line coordinate[%d] row[%d]\n",x,y,*linenum,*rownum);
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
    HM5065SENSORDB("mapCoordinate from[%d][%d] to[%d][%d]\n",
		linenum, rownum,*sectionlinenum,*sectionrownum);
}

void HM5065_mapRectToAE_2_ARRAY(UINT32 x0, UINT32 y0, UINT32 x1, UINT32 y1)
{
    UINT32 i, j;
    HM5065SENSORDB("([%d][%d]),([%d][%d])\n", x0,y0,x1,y1);
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
  	  HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_FOCUS_Set_AE_Window function:\n ");
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
	  HM5065SENSORDB("Set_AE_Window x0=%d,y0=%d,x1=%d,y1=%d,FD_XS=%d,FD_YS=%d\n",
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
		  
	  HM5065SENSORDB("[HM5065]AE pvx0=%d,pvy0=%d\n",pvx0, pvy0);
	  HM5065SENSORDB("[HM5065]AE pvx1=%d,pvy1=%d\n",pvx1, pvy1);
  //ndef AE_and_AF_FACE
	  if((pvx0==pvx1)&&(pvy0==pvy1))
	  {
	   if(CurrentScenarioId == MSDK_SCENARIO_ID_CAMERA_ZSD )
	   	{
	   	  HM5065_write_cmos_sensor(0x0120,0x01);
		  HM5065_write_cmos_sensor(0x0121,0x04); 
		  HM5065_write_cmos_sensor(0x0122,0x00);
		  HM5065_write_cmos_sensor(0x0123,0xC4);  
		  HM5065_write_cmos_sensor(0x0124,0x02);
		  HM5065_write_cmos_sensor(0x0125,0x09);
		  HM5065_write_cmos_sensor(0x0126,0x01); 
		  HM5065_write_cmos_sensor(0x0127,0x88);
		  
		  HM5065SENSORDB("HM5065_FOCUS_Set_ZSDAE_defalt_Window_to_IC\n");
	   	 }
	   else
	   	{
		  HM5065_write_cmos_sensor(0x0120,0x00);
		  HM5065_write_cmos_sensor(0x0121,0x82); 
		  HM5065_write_cmos_sensor(0x0122,0x00);
		  HM5065_write_cmos_sensor(0x0123,0x62);  
		  HM5065_write_cmos_sensor(0x0124,0x01);
		  HM5065_write_cmos_sensor(0x0125,0x04);
		  HM5065_write_cmos_sensor(0x0126,0x00); 		
		  HM5065_write_cmos_sensor(0x0127,0xC4);
		  
		  HM5065SENSORDB("HM5065_FOCUS_Set_preview AE_defalt_Window_to_IC\n");
	   	}
	  }
	  else
	  {
		  HM5065_write_cmos_sensor(0x0120,pvx0>>8);
		  HM5065_write_cmos_sensor(0x0121,pvx0&0xff); 
		  HM5065_write_cmos_sensor(0x0122,pvy0>>8);
		  HM5065_write_cmos_sensor(0x0123,pvy0&0xff);  
		  HM5065_write_cmos_sensor(0x0124,((pvx1-pvx0)/4)>>8);
		  HM5065_write_cmos_sensor(0x0125,((pvx1-pvx0)/4)&0xff);
		  HM5065_write_cmos_sensor(0x0126,((pvy1-pvy0)/4)>>8);		
		  HM5065_write_cmos_sensor(0x0127,((pvy1-pvy0)/4)&0xff);
  
	 	  HM5065SENSORDB("HM5065_FOCUS_Set_AE_Touch Window_to_IC\n");
   
	 	  HM5065SENSORDB("[HM5065]exit HM5065_FOCUS_Set_AE_Window function:\n ");
	  }  
#endif
}
//=====================touch AE end==========================//
/*************************************************************************
* FUNCTION
*	HM5065_set_dummy
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
static void HM5065initalvariable()
{
	spin_lock(&HM5065_drv_lock);
	HM5065Sensor.VideoMode = KAL_FALSE;
	HM5065Sensor.NightMode = KAL_FALSE;
	HM5065Sensor.Fps = 100;
	HM5065Sensor.ShutterStep= 0xde;
	HM5065Sensor.CaptureDummyPixels = 0;
	HM5065Sensor.CaptureDummyLines = 0;
	HM5065Sensor.PreviewDummyPixels = 0;
	HM5065Sensor.PreviewDummyLines = 0;
	HM5065Sensor.SensorMode= SENSOR_MODE_INIT;
	HM5065Sensor.IsPVmode= KAL_TRUE;	
	HM5065Sensor.PreviewPclk= 560;
	HM5065Sensor.CapturePclk= 800;
	HM5065Sensor.ZsdturePclk= 800;
	HM5065Sensor.PreviewShutter=0x0375; //0375
	HM5065Sensor.PreviewExtraShutter=0x00; 
	HM5065Sensor.SensorGain=0x10;
	HM5065Sensor.manualAEStart=0;
	HM5065Sensor.isoSpeed=AE_ISO_100;
	HM5065Sensor.userAskAeLock=KAL_FALSE;
    HM5065Sensor.userAskAwbLock=KAL_FALSE;
	HM5065Sensor.currentExposureTime=0;
    HM5065Sensor.currentShutter=0;
	HM5065Sensor.zsd_flag=0;
	HM5065Sensor.currentextshutter=0;
	spin_unlock(&HM5065_drv_lock);
}
void HM5065GetExifInfo(UINT32 exifAddr)
{
    SENSOR_EXIF_INFO_STRUCT* pExifInfo = (SENSOR_EXIF_INFO_STRUCT*)exifAddr;
    pExifInfo->FNumber = 20;
    pExifInfo->AEISOSpeed = HM5065Sensor.isoSpeed;
    pExifInfo->FlashLightTimeus = 0;
    pExifInfo->RealISOValue = HM5065Sensor.isoSpeed;
}
static void HM5065SetDummy(kal_uint32 dummy_pixels, kal_uint32 dummy_lines)
{
		HM5065SENSORDB("[HM5065]enter HM5065SetDummy function:\n ");
		if (HM5065Sensor.IsPVmode)  
        {
            dummy_pixels = dummy_pixels+HM5065_PV_PERIOD_PIXEL_NUMS; 
            //HM5065_write_cmos_sensor(0x380D,( dummy_pixels&0xFF));         
            //HM5065_write_cmos_sensor(0x380C,(( dummy_pixels&0xFF00)>>8)); 
      
            dummy_lines= dummy_lines+HM5065_PV_PERIOD_LINE_NUMS; 
            //HM5065_write_cmos_sensor(0x380F,(dummy_lines&0xFF));       
            //HM5065_write_cmos_sensor(0x380E,((dummy_lines&0xFF00)>>8));  
        } 
        else
        {
            dummy_pixels = dummy_pixels+HM5065_FULL_PERIOD_PIXEL_NUMS; 
            //HM5065_write_cmos_sensor(0x380D,( dummy_pixels&0xFF));         
            //HM5065_write_cmos_sensor(0x380C,(( dummy_pixels&0xFF00)>>8)); 
      
            dummy_lines= dummy_lines+HM5065_FULL_PERIOD_LINE_NUMS; 
            //HM5065_write_cmos_sensor(0x380F,(dummy_lines&0xFF));       
            //HM5065_write_cmos_sensor(0x380E,((dummy_lines&0xFF00)>>8));  
        } 
		HM5065SENSORDB("[HM5065]exit HM5065SetDummy function:\n ");
}    /* HM5065_set_dummy */

/*************************************************************************
* FUNCTION
*	HM5065WriteShutter
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
static void HM5065WriteShutter(kal_uint32 shutter)
{
	kal_uint32 extra_exposure_vts = 0;
	kal_uint16 temp_reg1, temp_reg2 ,temp_reg3;
	
	HM5065_write_cmos_sensor(0x17c, ((shutter<<8)&0xff00) );    // shutter h 8bit
	HM5065_write_cmos_sensor(0x17d, ((shutter)&0xff) );    //  shutter l 8bit

	//SENSORDB("[HM5065]exit HM5065WriteShutter function:\n ");

	HM5065SENSORDB("[HM5065]exit HM5065WriteShutter function:\n ");
}    /* HM5065_write_shutter */

/*************************************************************************
* FUNCTION
*	HM5065ExpWriteShutter
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

static void HM5065WriteExpShutter(kal_uint32 shutter)
{
#if 0
	shutter*=16;
	HM5065SENSORDB("[HM5065]enter HM5065WriteExpShutter function:\n ");
	//HM5065_write_cmos_sensor(0x3502, (shutter & 0x00FF));           //AEC[7:0]
	//HM5065_write_cmos_sensor(0x3501, ((shutter & 0x0FF00) >>8));  //AEC[15:8]
	//HM5065_write_cmos_sensor(0x3500, ((shutter & 0xFF0000) >> 16));	
	HM5065SENSORDB("[HM5065]exit HM5065WriteExpShutter function:\n ");
#endif
}    /* HM5065_write_shutter */

/*************************************************************************
* FUNCTION
*	HM5065ExtraWriteShutter
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
static void HM5065WriteExtraShutter(kal_uint32 shutter)
{
	HM5065SENSORDB("[HM5065]enter HM5065WriteExtraShutter function:\n ");
	//HM5065_write_cmos_sensor(0x350D, shutter & 0xFF);          // EXVTS[b7~b0]
	//HM5065_write_cmos_sensor(0x350C, (shutter & 0xFF00) >> 8); // EXVTS[b15~b8]
	HM5065SENSORDB("[HM5065]exit HM5065WriteExtraShutter function:\n ");
}    /* HM5065_write_shutter */

/*************************************************************************
* FUNCTION
*	HM5065WriteSensorGain
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
static void HM5065WriteSensorGain(kal_uint32 gain)
{
	kal_uint16 temp_reg ;
	HM5065SENSORDB("[HM5065]enter HM5065WriteSensorGain function:\n ");
	/*if(gain > 1024)  ASSERT(0);
	temp_reg = 0;
	temp_reg=gain&0x0FF;	*/
	HM5065_write_cmos_sensor(0x180, ((gain<<8)&0xff00) );    // shutter h 8bit
	HM5065_write_cmos_sensor(0x181, ((gain)&0xff) );    //  shutter l 8bit

	HM5065SENSORDB("[HM5065]exit HM5065WriteSensorGain function:\n ");
}  /* HM5065_write_sensor_gain */

/*************************************************************************
* FUNCTION
*	HM5065ReadShutter
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
static kal_uint32 HM5065ReadShutter(void)
{
	kal_uint16 temp_reg1, temp_reg2 ,temp_reg3;
	HM5065SENSORDB("[HM5065]enter HM5065ReadShutter function:\n ");
	temp_reg1 = HM5065_read_cmos_sensor(0x17c);    // shutter h 8bit
	temp_reg2 = HM5065_read_cmos_sensor(0x17d);    //  shutter l 8bit

	HM5065SENSORDB("[HM5065]exit HM5065ReadShutter function:\n ");	

	return ( ((temp_reg1&0xff)<<8) | (temp_reg2&0xff) );

} /* HM5065_read_shutter */

/*************************************************************************
* FUNCTION
*	HM5065ReadExtraShutter
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
static kal_uint32 HM5065ReadExtraShutter(void)
{

} /* HM5065_read_shutter */
/*************************************************************************
* FUNCTION
*	HM5065ReadSensorGain
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
static kal_uint32 HM5065ReadSensorGain(void)
{
	kal_uint16 temp_reg1, temp_reg2 ,temp_reg3;
	//kal_uint32 sensor_gain = 0;
	HM5065SENSORDB("[HM5065]enter HM5065ReadSensorGain function:\n ");
 
	temp_reg1 = HM5065_read_cmos_sensor(0x180);    // a gain h 8bit = 0
	temp_reg2 = HM5065_read_cmos_sensor(0x181);    //  a gain  l 8bit

	HM5065SENSORDB("[HM5065]exit HM5065ReadSensorGain function:\n ");
	return ( ((temp_reg1&0xff)<<8) | (temp_reg2&0xff) );
}  /* HM5065ReadSensorGain */
/*************************************************************************
* FUNCTION
*	HM5065_set_AE_mode
*
* DESCRIPTION
*	This function HM5065_set_AE_mode.
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
static void HM5065_set_AE_mode(kal_bool AE_enable)
{
    kal_uint8 AeTemp;
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_set_AE_mode function:%d\n ", AE_enable);
    //
    if (AE_enable == KAL_TRUE)
	
		   HM5065_write_cmos_sensor(0x0142,0x00);//enable AE
		else	   
		   HM5065_write_cmos_sensor(0x0142,0x01);//disable AE
		
	HM5065SENSORDB("[HM5065]exit HM5065_set_AE_mode function:\n ");
}

/*************************************************************************
* FUNCTION
*	HM5065_set_AWB_mode
*
* DESCRIPTION
*	This function HM5065_set_AWB_mode.
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
static void HM5065_set_AWB_mode(kal_bool AWB_enable)
{
    kal_uint8 AwbTemp;
	HM5065SENSORDB("[HM5065]enter HM5065_set_AWB_mode function:\n ");
//	  AwbTemp = HM5065_read_cmos_sensor(0x3406);   


	//	if(AWB_enable)
		//	HM5065_write_cmos_sensor(0x01A4,0x00);//open awb
//	else
		//HM5065_write_cmos_sensor(0x01A4,0x04);//freeze
	HM5065SENSORDB("[HM5065]exit HM5065_set_AWB_mode function:\n ");
}


/*************************************************************************
* FUNCTION
*	HM5065_night_mode
*
* DESCRIPTION
*	This function night mode of HM5065.
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
void HM5065_night_mode(kal_bool enable)
{
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_night_mode function:\n ");
	//kal_uint16 night = HM5065_read_cmos_sensor(0x3A00); 
	spin_lock(&HM5065_drv_lock);
	HM5065Sensor.NightMode=enable;
	spin_unlock(&HM5065_drv_lock);
	#if 0
	if (enable)
	{ 

		HM5065Sensor.VideoMode
            /* camera night mode */
     	//HM5065_write_cmos_sensor(0x3A00,night|0x04); // 30fps-5fps
        //HM5065_write_cmos_sensor(0x3a02,0x17); 
        //HM5065_write_cmos_sensor(0x3a03,0x10);                         
        //HM5065_write_cmos_sensor(0x3a14,0x17); 
        //HM5065_write_cmos_sensor(0x3a15,0x10);
        //HM5065_write_cmos_sensor(0x3a19,0xc8);       
	}
	else
	{             /* camera normal mode */                
		//HM5065_write_cmos_sensor(0x3A00,night|0x04); //30fps-10fps               
		//HM5065_write_cmos_sensor(0x3a02,0x0b);
		//HM5065_write_cmos_sensor(0x3a03,0x88);
		//HM5065_write_cmos_sensor(0x3a14,0x0b); 
		//HM5065_write_cmos_sensor(0x3a15,0x88);								
								//HM5065_write_cmos_sensor(0x3a19,0x60);      
	} 
#endif
	//if (!HM5065Sensor.MODE_CAPTURE) 
	if((CurrentScenarioId == MSDK_SCENARIO_ID_CAMERA_PREVIEW) || (CurrentScenarioId == MSDK_SCENARIO_ID_VIDEO_PREVIEW) )

	{ 
	  if(enable)
	  { 
		//	 HM5065Sensor.NightMode = KAL_TRUE;
		 if(HM5065Sensor.VideoMode == KAL_TRUE)
		  { 	  
	
				  /* MJPEG or MPEG4 Apps */
			  printk("HM5065 night mode ture MPEG4_encode--------------\r\n");	
				  				   #if 0
				  HM5065_write_cmos_sensor(0x0040,0x01);  //  binning mode and subsampling mode for frame rate
				  HM5065_write_cmos_sensor(0x0041,0x0A);  //  04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
				  HM5065_write_cmos_sensor(0x0042,0x05);  //05	  X:800 0x500=1280,0x0320=800
				  HM5065_write_cmos_sensor(0x0043,0x00);  //00
				  HM5065_write_cmos_sensor(0x0044,0x03);  //03	  Y:600 0x03c0=960,0x0258=600
				  HM5065_write_cmos_sensor(0x0045, 0xC0);	  //
	#endif
	
				  HM5065_write_cmos_sensor(0x00E8,0x00);//Static Framerate
				  HM5065_write_cmos_sensor(0x00C8,0x00);
				  HM5065_write_cmos_sensor(0x00C9,0x0F);//15fps
				  HM5065_write_cmos_sensor(0x00CA,0x01);
				  //HM5065_write_cmos_sensor(0x0330,0x00);
		#if 0
				  //HM5065_write_cmos_sensor(0x0082,0x74);//Brightness
				  HM5065_write_cmos_sensor(0x015E,0x41);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
				  HM5065_write_cmos_sensor(0x015F,0x00);
		#else
		          HM5065_write_cmos_sensor(0x0143,0x5E);	//max int 1/15s
		          HM5065_write_cmos_sensor(0x0144,0x09);
				  #endif
		 }	  
		  else 
		 {
				printk("HM5065 night mode ture camera preview\r\n");
	#if 0
				  //HM5065_write_cmos_sensor(0x0030,0x14);	  //  Max.Derate=4
				  HM5065_write_cmos_sensor(0x0040,0x01);  //  binning mode and subsampling mode for frame rate
				  HM5065_write_cmos_sensor(0x0041,0x0A);  //  04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
				  HM5065_write_cmos_sensor(0x0042,0x05);  //05	  X:800 0x500=1280,0x0320=800
				  HM5065_write_cmos_sensor(0x0043,0x00);  //00
				  HM5065_write_cmos_sensor(0x0044,0x03);  //03	  Y:600 0x03c0=960,0x0258=600
				  HM5065_write_cmos_sensor(0x0045, 0xC0);	  //
	#endif
			  #if 0
			  HM5065_write_cmos_sensor(0x00E8,0x00);//Static Framerate
				  HM5065_write_cmos_sensor(0x00C8,0x00);
				  HM5065_write_cmos_sensor(0x00C9,0x05);//5fps
				  HM5065_write_cmos_sensor(0x00CA,0x01);
				  //HM5065_write_cmos_sensor(0x0330,0x00);
			  #else
				  HM5065_write_cmos_sensor(0x00E8,0x01);//AFR
				  HM5065_write_cmos_sensor(0x00ED,0x05);//Min=5fps
				  HM5065_write_cmos_sensor(0x00EE,0x1E);//Max=30fps
			  #endif

			  			#if 0
				  //HM5065_write_cmos_sensor(0x0082,0x64);//Brightness
				  HM5065_write_cmos_sensor(0x0143,0x64);			  
				  HM5065_write_cmos_sensor(0x02C3,0xC0);			  
				  HM5065_write_cmos_sensor(0x015E,0x41);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
				  HM5065_write_cmos_sensor(0x015F,0x00);				
				  #else
				  HM5065_write_cmos_sensor(0x0143,0x61);	//max int = 1/5s		  
				  HM5065_write_cmos_sensor(0x0144,0x0D);
				  //HM5065_write_cmos_sensor(0x02C3,0xC0);			  
				  //HM5065_write_cmos_sensor(0x0130,0x04);//brightness
				  #endif
		  } 	  
	  }
	  else
		  {
		 // HM5065Sensor.bNight_mode = KAL_FALSE;
		  if(HM5065Sensor.VideoMode == KAL_TRUE)
		  {
		 
				   /* MJPEG or MPEG4 Apps */
				  //HM5065_write_cmos_sensor(0x0030,0x14);	  //  Max.Derate=4
			  printk("HM5065 night_auto mode ture MPEG4_encode--------------\r\n"); 
				   #if 0
				  HM5065_write_cmos_sensor(0x0040,0x01);  //  binning mode and subsampling mode for frame rate
				  HM5065_write_cmos_sensor(0x0041,0x0A);  //  04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
				  HM5065_write_cmos_sensor(0x0042,0x05);  //05	  X:800 0x500=1280,0x0320=800
				  HM5065_write_cmos_sensor(0x0043,0x00);  //00
				  HM5065_write_cmos_sensor(0x0044,0x03);  //03	  Y:600 0x03c0=960,0x0258=600
				  HM5065_write_cmos_sensor(0x0045, 0xC0);	  //
	#endif
				  HM5065_write_cmos_sensor(0x00E8,0x00);//Static Framerate
				  HM5065_write_cmos_sensor(0x00C8,0x00);
				  HM5065_write_cmos_sensor(0x00C9,0x1F);//30fps
				  HM5065_write_cmos_sensor(0x00CA,0x01);
				  //HM5065_write_cmos_sensor(0x0330,0x01);
	#if 0
				  //HM5065_write_cmos_sensor(0x0082,0x64);//Brightness
				  HM5065_write_cmos_sensor(0x015E,0x40);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
				  HM5065_write_cmos_sensor(0x015F,0x00);
		#else
		       // HM5065_write_cmos_sensor(0x0143,0x5C);	//max int 1/30s mark by derek
		        //HM5065_write_cmos_sensor(0x0144,0x09);				  
				  #endif
			  }
		  else 
			  {   
			  printk("HM5065 night_auto mode ture camera preview--------------\r\n");
		#if 0
				//HM5065_write_cmos_sensor(0x0030,0x14);  //  Max.Derate=4
				  HM5065_write_cmos_sensor(0x0040,0x01); //   binning mode and subsampling mode for frame rate
				  HM5065_write_cmos_sensor(0x0041,0x0A); //   04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
				  HM5065_write_cmos_sensor(0x0042,0x05);  //05	  X:800 0x500=1280,0x0320=800
				  HM5065_write_cmos_sensor(0x0043,0x00);  //00
				  HM5065_write_cmos_sensor(0x0044,0x03);  //03	  Y:600 0x03c0=960,0x0258=600
				  HM5065_write_cmos_sensor(0x0045, 0xC0); //
	#endif
				  HM5065_write_cmos_sensor(0x00E8,0x01);//AFR
				  HM5065_write_cmos_sensor(0x00ED,0x0a);//Min=12fps
				  HM5065_write_cmos_sensor(0x00EE,0x1E);//Max=30fps
				  //HM5065_write_cmos_sensor(0x0330,0x01);
			#if 0
				  //HM5065_write_cmos_sensor(0x0082,0x5A);//Brightness
				  HM5065_write_cmos_sensor(0x0143,0x5F);			  
				  HM5065_write_cmos_sensor(0x02C3,0xA0);			  
				  HM5065_write_cmos_sensor(0x015E,0x40);//Max Dgain 4100=3x ,4000=2x, 3E00=1x
				  HM5065_write_cmos_sensor(0x015F,0x00);
			#else
				  HM5065_write_cmos_sensor(0x0143,0x5F);	//max int 1/10s
				  HM5065_write_cmos_sensor(0x0144,0x0D);
				  //HM5065_write_cmos_sensor(0x02C3,0xA0);			  
				  //HM5065_write_cmos_sensor(0x0130,0x00);
				  #endif
			  } 	 
		  }  
	  }
	if((CurrentScenarioId == MSDK_SCENARIO_ID_CAMERA_ZSD) )

	{ 
		  if(enable)
		  { 
					printk("HM5065 night mode ture camera ZSD preview \r\n");
					  HM5065_write_cmos_sensor(0x00E8,0x01);//AFR
					  HM5065_write_cmos_sensor(0x00ED,0x05);//Min=5fps
					  HM5065_write_cmos_sensor(0x00EE,0x0f);//Max=30fps

					  HM5065_write_cmos_sensor(0x0143,0x61);	//max int = 1/5s		  
					  HM5065_write_cmos_sensor(0x0144,0x34);
		  }
		  else
			  {
				  printk("HM5065 night_auto mode ture camera ZSD preview--------------\r\n");
					  HM5065_write_cmos_sensor(0x00E8,0x01);//AFR
					  HM5065_write_cmos_sensor(0x00ED,0x0a);//Min=12fps
					  HM5065_write_cmos_sensor(0x00EE,0x0f);//Max=30fps
					  //HM5065_write_cmos_sensor(0x0330,0x01);
					  HM5065_write_cmos_sensor(0x0143,0x5F);	//max int 1/10s
					  HM5065_write_cmos_sensor(0x0144,0x0D);
			  }  
	  }

	
	HM5065SENSORDB("[HM5065]exit HM5065_night_mode function:\n ");
}	/* HM5065_night_mode */
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
//static 
kal_uint32 HM5065_GetSensorID(kal_uint32 *sensorID)
{
    volatile signed char i;
	kal_uint32 sensor_id=0;
	kal_uint8 temp_sccb_addr = 0;
	HM5065SENSORDB("[HM5065]enter HM5065_GetSensorID function:\n ");
	//HM5065_write_cmos_sensor(0x3008,0x82);// Reset sensor
	mDELAY(10);
	for(i=0;i<3;i++)
	{
		sensor_id = (HM5065_read_cmos_sensor(0x0000) << 8) | HM5065_read_cmos_sensor(0x0001);
		HM5065SENSORDB("HM5065 READ ID: %x",sensor_id);
		if(sensor_id != HM5065_SENSOR_ID)
		{	
			*sensorID =0xffffffff;
			return ERROR_SENSOR_CONNECT_FAIL;
		}
		else
			{
			*sensorID=HM5065_SENSOR_ID;
		        break;
			}
	}
	HM5065SENSORDB("[HM5065]exit HM5065_GetSensorID function:\n ");
   return ERROR_NONE;    

}   
UINT32 HM5065SetTestPatternMode(kal_bool bEnable)
{
	HM5065SENSORDB("[HM5065_HM5065SetTestPatternMode]test pattern bEnable:=%d\n",bEnable);
	if(bEnable)
	{
		////HM5065_write_cmos_sensor(0x503d,0x80);
		HM5065_run_test_pattern=1;
	}
	else
	{
		////HM5065_write_cmos_sensor(0x503d,0x00);
		HM5065_run_test_pattern=0;
	}
	return ERROR_NONE;
}
#if 0
////burst mode 由于是16bit addr 和 8bit data，可以参考如下
#define USE_I2C_BURST_WRITE_HM5065
#ifdef USE_I2C_BURST_WRITE_HM5065
#define I2C_BUFFER_LEN_HM5065 254 //MAX data to send by MT6572 i2c dma mode is 255 bytes
#define BLOCK_I2C_DATA_WRITE_HM5065 iBurstWriteReg
#else
#define I2C_BUFFER_LEN_HM5065 8 // MT6572 i2s bus master fifo length is 8 bytes
#define BLOCK_I2C_DATA_WRITE_HM5065 iWriteRegI2C
#endif

// {addr, data} pair in para
// len is the total length of addr+data
// Using I2C multiple/burst write if the addr increase by 1
static kal_uint16 HM5065_table_write_cmos_sensor(kal_uint8* para, kal_uint32 len)
{
char puSendCmd[I2C_BUFFER_LEN_HM5065]; //at most 2 bytes address and 6 bytes data for multiple write. MTK i2c master has only 8bytes fifo.
kal_uint32 tosend, IDX;
kal_uint8 data;
kal_uint16 addr, addr_last;

tosend = 0;
IDX = 0;
while(IDX < len)
{
addr = ((para[IDX]<<8)&0xff00) | ((para[IDX+1]<<0)&0x00ff);//16bit addr

if (tosend == 0) // new (addr, data) to send
{
puSendCmd[tosend++] = (char)para[IDX];//addr
puSendCmd[tosend++] = (char)para[IDX+1];//addr
puSendCmd[tosend++] = (char)para[IDX+2];//data
IDX += 3;
addr_last = addr;
}
else if (addr == addr_last+1) // to multiple write the data to the incremental address
{
data = para[IDX+2];
puSendCmd[tosend++] = (char)data;
IDX += 3;
addr_last ++;
}
// to send out the data if the sen buffer is full or last data or to program to the address not incremental.
if (tosend == I2C_BUFFER_LEN_HM5065 || IDX == len || addr != addr_last)
{
BLOCK_I2C_DATA_WRITE_HM5065(puSendCmd , tosend, HM5065_WRITE_ID);
tosend = 0;
}
}
return 0;
}


#endif


/*************************************************************************
* FUNCTION
*    HM5065InitialSetting
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
static u8 MCU_Setting1[] = 
{ 
	0x90, 
0x0C, 
0x56, 
0xE0, 
0xFE, 
0xA3, 
0xE0, 
0xFF, 
0x12, 
0x42, 
0x85, 
0x90, 
0x01, 
0xB7, 
0xEE, 
0xF0, 
0xFC, 
0xA3, 
0xEF, 
0xF0, 
0xFD, 
0x90, 
0x06, 
0x05, 
0xE0, 
0x75, 
0xF0, 
0x02, 
0xA4, 
0x2D, 
0xFF, 
0xE5, 
0xF0, 
0x3C, 
0xFE, 
0xAB, 
0x07, 
0xFA, 
0x33, 
0x95, 
0xE0, 
0xF9, 
0xF8, 
0x90, 
0x0B, 
0x4B, 
0xE0, 
0xFE, 
0xA3, 
0xE0, 
0xFF, 
0xEE, 
0x33, 
0x95, 
0xE0, 
0xFD, 
0xFC, 
0x12, 
0x0C, 
0x7B, 
0x90, 
0x01, 
0xB9, 
0x12, 
0x0E, 
0x05, 
0x90, 
0x01, 
0xB9, 
0xE0, 
0xFC, 
0xA3, 
0xE0, 
0xFD, 
0xA3, 
0xE0, 
0xFE, 
0xA3, 
0xE0, 
0xFF, 
0x78, 
0x08, 
0x12, 
0x0D, 
0xBF, 
0xA8, 
0x04, 
0xA9, 
0x05, 
0xAA, 
0x06, 
0xAB, 
0x07, 
0x90, 
0x0B, 
0x49, 
0xE0, 
0xFE, 
0xA3, 
0xE0, 
0xFF, 
0xEE, 
0x33, 
0x95, 
0xE0, 
0xFD, 
0xFC, 
0xC3, 
0xEF, 
0x9B, 
0xFF, 
0xEE, 
0x9A, 
0xFE, 
0xED, 
0x99, 
0xFD, 
0xEC, 
0x98, 
0xFC, 
0x78, 
0x01, 
0x12, 
0x0D, 
0xBF, 
0x90, 
0x0C, 
0x4A, 
0xE0, 
0xFC, 
0xA3, 
0xE0, 
0xF5, 
0x82, 
0x8C, 
0x83, 
0xC0, 
0x83, 
0xC0, 
0x82, 
0x90, 
0x0B, 
0x48, 
0xE0, 
0xD0, 
0x82, 
0xD0, 
0x83, 
0x75, 
0xF0, 
0x02, 
0x12, 
0x0E, 
0x45, 
0xEE, 
0xF0, 
0xA3, 
0xEF, 
0xF0, 
0x02, 
0xBA, 
0xD8, 
0x90, 
0x30, 
0x18, 
0xE4, 
0xF0, 
0x74, 
0x3F, 
0xF0, 
0x22, 
0xC4, 
0x2F, 
0x08, 
0x61, 
0x43, 
0xDE, 
0xB1, 
0x4B, 
0xBC, 
0xBF, 
0x43, 
0x7A, 
0xB4, 
0x8B, 
0x7A, 
0xD9, 
0xB5, 
0x8E, 
0x0A, 
0x83, 
0x90, 
0x00, 
0x5E, 
0xE0, 
0xFF, 
0x70, 
0x20, 
0x90, 
0x47, 
0x04, 
0x74, 
0x0A, 
0xF0, 
0xA3, 
0x74, 
0x30, 
0xF0, 
0x90, 
0x47, 
0x0C, 
0x74, 
0x07, 
0xF0, 
0xA3, 
0x74, 
0xA8, 
0xF0, 
0x90, 
0x47, 
0xA4, 
0x74, 
0x01, 
0xF0, 
0x90, 
0x47, 
0xA8, 
0xF0, 
0x80, 
0x50, 
0xEF, 
0x64, 
0x01, 
0x60, 
0x04, 
0xEF, 
0xB4, 
0x03, 
0x20, 
0x90, 
0x47, 
0x04, 
0x74, 
0x05, 
0xF0, 
0xA3, 
0x74, 
0x18, 
0xF0, 
0x90, 
0x47, 
0x0C, 
0x74, 
0x03, 
0xF0, 
0xA3, 
0x74, 
0xD4, 
0xF0, 
0x90, 
0x47, 
0xA4, 
0x74, 
0x02, 
0xF0, 
0x90, 
0x47, 
0xA8, 
0xF0, 
0x80, 
0x27, 
0xEF, 
0x64, 
0x02, 
0x60, 
0x04, 
0xEF, 
0xB4, 
0x04, 
0x1E, 
0x90, 
0x47, 
0x04, 
0x74, 
0x02, 
0xF0, 
0xA3, 
0x74, 
0x8C, 
0xF0, 
0x90, 
0x47, 
0x0C, 
0x74, 
0x01, 
0xF0, 
0xA3, 
0x74, 
0xEA, 
0xF0, 
0x90, 
0x47, 
0xA4, 
0x74, 
0x04, 
0xF0, 
0x90, 
0x47, 
0xA8, 
0xF0, 
0x22, 
0x74, 
0x04, 
0xF0, 
0xA3, 
0x74, 
0x20, 
0xF0, 
0xE4, 
0xF5, 
0x22, 
0xE5, 
0x22, 
0xC3, 
0x94, 
0x40, 
0x40, 
0x03, 
0x02, 
0xF1, 
0xFD, 
0x90, 
0x0A, 
0xBA, 
0xE0, 
0xFE, 
0xA3, 
0xE0, 
0xFF, 
0xF5, 
0x82, 
0x8E, 
0x83, 
0xE0, 
0x54, 
0x70, 
0xFD, 
0xC4, 
0x54, 
0x0F, 
0xFD, 
0x90, 
0x0A, 
0xBC, 
0xE0, 
0xFA, 
0xA3, 
0xE0, 
0xF5, 
0x82, 
0x8A, 
0x83, 
0xED, 
0xF0, 
0x90, 
0x0A, 
0xBD, 
0xE0, 
0x04, 
0xF0, 
0x70, 
0x06, 
0x90, 
0x0A, 
0xBC, 
0xE0, 
0x04, 
0xF0, 
0x8F, 
0x82, 
0x8E, 
0x83, 
0xA3, 
0xE0, 
0xFF, 
0x90, 
0x0A, 
0xBC, 
0xE0, 
0xFC, 
0xA3, 
0xE0, 
0xF5, 
0x82, 
0x8C, 
0x83, 
0xEF, 
0xF0, 
0x90, 
0x0A, 
0xBD, 
0xE0, 
0x04, 
0xF0, 
0x70, 
0x06, 
0x90, 
0x0A, 
0xBC, 
0xE0, 
0x04, 
0xF0, 
0x90, 
0x0A, 
0xBA, 
0xE0, 
0xFE, 
0xA3, 
0xE0, 
0xFF, 
0xF5, 
0x82, 
0x8E, 
0x83, 
0xE0, 
0x54, 
0x07, 
0xFD, 
0x90, 
0x0A, 
0xBC, 
0xE0, 
0xFA, 
0xA3, 
0xE0, 
0xF5, 
0x82, 
0x8A, 
0x83, 
0xED, 
0xF0, 
0x90, 
0x0A, 
0xBD, 
0xE0, 
0x04, 
0xF0, 
0x70, 
0x06, 
0x90, 
0x0A, 
0xBC, 
0xE0, 
0x04, 
0xF0, 
0x8F, 
0x82, 
0x8E, 
0x83, 
0xA3, 
0xA3, 
0xE0, 
0xFF, 
0x90, 
0x0A, 
0xBC, 
0xE0, 
0xFC, 
0xA3, 
0xE0, 
0xF5, 
0x82, 
0x8C, 
0x83, 
0xEF, 
0xF0, 
0x90, 
0x0A, 
0xBD, 
0xE0, 
0x04, 
0xF0, 
0x70, 
0x06, 
0x90, 
0x0A, 
0xBC, 
0xE0, 
0x04, 
0xF0, 
0x90, 
0x0A, 
0xBB, 
0xE0, 
0x24, 
0x03, 
0xF0, 
0x90, 
0x0A, 
0xBA, 
0xE0, 
0x34, 
0x00, 
0xF0, 
0x05, 
0x22, 
0x02, 
0xF1, 
0x41, 
0x90, 
0x0A, 
0xBA, 
0x74, 
0x0E, 
0xF0, 
0xA3, 
0x74, 
0xDC, 
0xF0, 
0xA3, 
0x74, 
0x05, 
0xF0, 
0xA3, 
0x74, 
0x61, 
0xF0, 
0x90, 
0x0A, 
0xBA, 
0xE0, 
0xFE, 
0xA3, 
0xE0, 
0xAA, 
0x06, 
0xF9, 
0x7B, 
0x01, 
0xC0, 
0x02, 
0xA3, 
0xE0, 
0xFE, 
0xA3, 
0xE0, 
0xAA, 
0x06, 
0xF8, 
0xAC, 
0x02, 
0x7D, 
0x01, 
0xD0, 
0x02, 
0x7E, 
0x00, 
0x7F, 
0x04, 
0x12, 
0x0F, 
0x6F, 
0x02, 
0x66, 
0xD9, 
0x90, 
0x07, 
0xD0, 
0x02, 
0xA2, 
0x69, 
0x52, 
0x69, 
0x56, 
0x80, 
0x92, 
0x02, 
0x21, 
0x7F, 
0x02, 
0x21, 
0xF4, 
0x02, 
0xA6, 
0x15, 
0x60, 
0x0A, 
0xEF, 
0xB4, 
0x01, 
0x16, 
0x90, 
0x00, 
0x5D, 
0xE0, 
0x70, 
0x10, 
0x12, 
0x26, 
0xC8, 
0x90, 
0x00, 
0x11, 
0x74, 
0x30, 
0xF0, 
0x90, 
0x00, 
0x10, 
0x74, 
0x01, 
0xF0, 
0x22, 
0x12, 
0x25, 
0xA8, 
0x02, 
0x29, 
0xFC, 
0x44, 
0x18, 
0xF0, 
0x90, 
0x72, 
0x18, 
0xE0, 
0x44, 
0x18, 
0xF0, 
0x00, 
0x00, 
0x00, 
0x00, 
0x00, 
0x00, 
0x90, 
0x72, 
0x08, 
0xE0, 
0x44, 
0x10, 
0xF0, 
0x90, 
0x72, 
0x14, 
0xE0, 
0x54, 
0xFD, 
0xF0, 
0x22, 
0xB6, 
0x91, 
0x7F, 
0x68, 
0x2D, 
0x80, 
0xD0, 
0x1F, 
0x15, 
0xF1, 
0x8D, 
0x94, 
0x10, 
0x73, 
0x53, 
0xB6, 
0xD2, 
0xF0, 
0xD3, 
0x90, 
0x07, 
0x91, 
0xE0, 
0x94, 
0x21, 
0x90, 
0x07, 
0x90, 
0xE0, 
0x64, 
0x80, 
0x94, 
0x81, 
0x40, 
0x08, 
0x90, 
0x07, 
0xCB, 
0x74, 
0xFF, 
0xF0, 
0x80, 
0x06, 
0x90, 
0x07, 
0xCB, 
0x74, 
0x01, 
0xF0, 
0x02, 
0xB5, 
0xC3, 
0x90, 
0x08, 
0x34, 
0xE0, 
0xFC, 
0xA3, 
0xE0, 
0xFD, 
0xA3, 
0xE0, 
0xFE, 
0xA3, 
0xE0, 
0xFF, 
0x90, 
0x07, 
0xD0, 
0xE0, 
0xF8, 
0xA3, 
0xE0, 
0xF9, 
0xA3, 
0xE0, 
0xFA, 
0xA3, 
0xE0, 
0xFB, 
0xD3, 
0x12, 
0x0D, 
0xAE, 
0x40, 
0x0B, 
0x12, 
0xB5, 
0x49, 
0x90, 
0x07, 
0xA4, 
0x74, 
0x02, 
0xF0, 
0x80, 
0x09, 
0x12, 
0xB7, 
0x51, 
0x90, 
0x07, 
0xA4, 
0x74, 
0x05, 
0xF0, 
0x02, 
0xA2, 
0xDA, 
0x90, 
0x0E, 
0xE0, 
0xE0, 
0xFD, 
0xA3, 
0xE0, 
0x90, 
0x02, 
0xA2, 
0xCD, 
0xF0, 
0xA3, 
0xED, 
0xF0, 
0x90, 
0x0E, 
0xE2, 
0xE0, 
0xFD, 
0xA3, 
0xE0, 
0x90, 
0x02, 
0xA8, 
0xCD, 
0xF0, 
0xA3, 
0xED, 
0xF0, 
0xE4, 
0x90, 
0x06, 
0x38, 
0xF0, 
0x02, 
0x67, 
0x63, 
0x90, 
0x0E, 
0xE8, 
0xE0, 
0x90, 
0x02, 
0x62, 
0xF0, 
0x90, 
0x0E, 
0xE9, 
0xE0, 
0x90, 
0x02, 
0x63, 
0xF0, 
0x02, 
0x67, 
0x1F, 
0xD1, 
0x01, 
0x2D, 
0x40, 
0x9F, 
0x15, 
0x33, 
0x43, 
0x3B, 
0x09, 
0x1D, 
0x90, 
0x0E, 
0x14, 
0xE0, 
0xFE, 
0xA3, 
0xE0, 
0xFF, 
0x90, 
0x06, 
0xD9, 
0xEE, 
0xF0, 
0xA3, 
0xEF, 
0xF0, 
0x90, 
0x0E, 
0x18, 
0xE0, 
0xFD, 
0x7C, 
0x00, 
0xC3, 
0xEF, 
0x9D, 
0xEE, 
0x9C, 
0x50, 
0x09, 
0xE4, 
0x90, 
0x06, 
0xD7, 
0xF0, 
0xA3, 
0xF0, 
0x80, 
0x13, 
0xC3, 
0x90, 
0x06, 
0xDA, 
0xE0, 
0x9D, 
0xFE, 
0x90, 
0x06, 
0xD9, 
0xE0, 
0x9C, 
0x90, 
0x06, 
0xD7, 
0xF0, 
0xA3, 
0xCE, 
0xF0, 
0x90, 
0x0E, 
0x18, 
0xE0, 
0xF9, 
0xFF, 
0x90, 
0x06, 
0xC2, 
0xE0, 
0xFC, 
0xA3, 
0xE0, 
0xFD, 
0xC3, 
0x9F, 
0xFF, 
0xEC, 
0x94, 
0x00, 
0xFE, 
0x90, 
0x0E, 
0x16, 
0xE0, 
0xFA, 
0xA3, 
0xE0, 
0xFB, 
0xD3, 
0x9F, 
0xEA, 
0x9E, 
0x40, 
0x0A, 
0x90, 
0x06, 
0xD5, 
0xEC, 
0xF0, 
0xA3, 
0xED, 
0xF0, 
0x80, 
0x0E, 
0xE9, 
0x7E, 
0x00, 
0x2B, 
0xFF, 
0xEE, 
0x3A, 
0x90, 
0x06, 
0xD5, 
0xF0, 
0xA3, 
0xEF, 
0xF0, 
0xE9, 
0xFB, 
0x7A, 
0x00, 
0x90, 
0x0E, 
0x15, 
0xE0, 
0x2B, 
0xFE, 
0x90, 
0x0E, 
0x14, 
0xE0, 
0x3A, 
0x90, 
0x06, 
0xE1, 
0xF0, 
0xA3, 
0xCE, 
0xF0, 
0xC3, 
0x90, 
0x0E, 
0x17, 
0xE0, 
0x9B, 
0xFE, 
0x90, 
0x0E, 
0x16, 
0x02, 
0x20, 
0xD5, 
0x90, 
0x0E, 
0xE4, 
0xE0, 
0x90, 
0x02, 
0x66, 
0xF0, 
0x90, 
0x0E, 
0xE5, 
0xE0, 
0x90, 
0x02, 
0x64, 
0xF0, 
0x90, 
0x0E, 
0xE6, 
0xE0, 
0x90, 
0x02, 
0x65, 
0xF0, 
0x02, 
0x67, 
0xA5, 
0x3B, 
0x93, 
0x12, 
0x47, 
0x59, 
0x90, 
0x00, 
0xB5, 
0xE0, 
0xB4, 
0x02, 
0x03, 
0x12, 
0x47, 
0x59, 
0x02, 
0xC5, 
0xC3, 
0x90, 
0x00, 
0x3D, 
0xF0, 
0x90, 
0x00, 
0x84, 
0xE0, 
0xFE, 
0x90, 
0x00, 
0x3E, 
0xF0, 
0xEF, 
0x70, 
0x03, 
0xEE, 
0x60, 
0x04, 
0x7F, 
0x01, 
0x80, 
0x02, 
0x7F, 
0x00, 
0x90, 
0x00, 
0x3F, 
0xEF, 
0xF0, 
0x02, 
0x89, 
0xD3, 
0x90, 
0x00, 
0x12, 
0xE0, 
0xFF, 
0x70, 
0x0C, 
0x90, 
0x00, 
0x46, 
0xE0, 
0xC3, 
0x94, 
0x07, 
0x40, 
0x03, 
0x75, 
0x2E, 
0x02, 
0xEF, 
0xB4, 
0x01, 
0x0C, 
0x90, 
0x00, 
0x66, 
0xE0, 
0xC3, 
0x94, 
0x07, 
0x40, 
0x03, 
0x75, 
0x2E, 
0x02, 
0x02, 
0xA7, 
0x9E, 
0xC3, 
0x90, 
0x0B, 
0x8F, 
0xE0, 
0x94, 
0x80, 
0x90, 
0x0B, 
0x8E, 
0xE0, 
0x94, 
0x44, 
0x40, 
0x22, 
0x90, 
0x0B, 
0x91, 
0xE0, 
0x94, 
0x80, 
0x90, 
0x0B, 
0x90, 
0xE0, 
0x94, 
0x44, 
0x40, 
0x14, 
0x90, 
0x0B, 
0x93, 
0xE0, 
0x94, 
0x80, 
0x90, 
0x0B, 
0x92, 
0xE0, 
0x94, 
0x44, 
0x40, 
0x06, 
0x90, 
0x01, 
0xA4, 
0x02, 
0x86, 
0x57, 
0x02, 
0x86, 
0x5C, 
0xA3, 
0x08, 
0xF0, 
0x98, 
0xC9, 
0x33, 
0x75, 
0xC0, 
0xB4, 
0x3E, 
0x25, 
0xAB, 
0x15, 
0x40, 
0x5F, 
0x6B, 
0x38, 
0x6D, 
0xC8, 
0x60, 
0xAC, 
0x22, 
0xC1, 
0x34, 
0x40, 
0x87, 
0x8F, 
0x8D, 
0x21, 
0xCC, 
0x86, 
0xF0, 
0x69, 
0x36, 
0x36, 
0x33, 
0x64, 
0x90, 
0xD4, 
0x08, 
0x2F, 
0xBE, 
0x54, 
0x7D, 
0x80, 
0x40, 
0x8B, 
0x74, 
0x06, 
0xBA, 
0x11, 
0xB7, 
0x1C, 
0x9E, 
0xF5, 
0x83, 
0xFB, 
0xBB, 
0xA8, 
0xA5, 
0x8D, 
0xA8, 
0x4E, 
0x7F, 
0x24, 
0x92, 
0x96, 
0x5C, 
0x5F, 
0x8F, 
0x38, 
0xB1, 
0x24, 
0x89, 
0x93, 
0x8F, 
0x01, 
0xDA, 
0x3B, 
0x20, 
0x74, 
0x69, 
0x8E, 
0xC8, 
0xDE, 
0xFE, 
0x2C, 
0xCB, 
0x08, 
0xBD, 
0x53, 
0x3D, 
0x01, 
0x8C, 
0x80, 
0xDF, 
0xF8, 
0x48, 
0x44, 
0x33, 
0x77, 
0x6E, 
0xF0, 
0xBE, 
0xAE, 
0x54, 
0x3E, 
0x31, 
0x84, 
0x7D, 
0x99, 
0x9C, 
0x41, 
0x26, 
0x3A, 
0xF1, 
0xE0, 
0xBB, 
0x75, 
0x0B, 
0x72, 
0xAC, 
0x51, 
0xC3, 
0xF0, 
0x52, 
0x55, 
0xEA, 
0x01, 
0xEA, 
0x6F, 
0xB7, 
0xE5, 
0xF5, 
0x3B, 
0x90, 
0x06, 
0x6C, 
0xE0, 
0xFF, 
0xE5, 
0x3B, 
0xC3, 
0x9F, 
0x40, 
0x03, 
0x02, 
0xF6, 
0x0E, 
0x90, 
0x0B, 
0xC6, 
0xE0, 
0x14, 
0x60, 
0x3C, 
0x14, 
0x60, 
0x6B, 
0x24, 
0x02, 
0x60, 
0x03, 
0x02, 
0xF5, 
0xB5, 
0x90, 
0x0A, 
0x9A, 
0xE0, 
0xFB, 
0xA3, 
0xE0, 
0xFA, 
0xA3, 
0xE0, 
0xF9, 
0x85, 
0x3B, 
0x82, 
0x75, 
0x83, 
0x00, 
0x12, 
0x0A, 
0xB8, 
0xFF, 
0x74, 
0xAB, 
0x25, 
0x3B, 
0xF5, 
0x82, 
0xE4, 
0x34, 
0x0A, 
0xF5, 
0x83, 
0xE0, 
0xFD, 
0xC3, 
0xEF, 
0x9D, 
0xFE, 
0xE4, 
0x94, 
0x00, 
0x90, 
0x0B, 
0xCA, 
0xF0, 
0xA3, 
0xCE, 
0xF0, 
0x80, 
0x62, 
0x90, 
0x0A, 
0x9A, 
0xE0, 
0xFB, 
0xA3, 
0xE0, 
0xFA, 
0xA3, 
0xE0, 
0xF9, 
0x85, 
0x3B, 
0x82, 
0x75, 
0x83, 
0x00, 
0x12, 
0x0A, 
0xB8, 
0xFF, 
0x74, 
0x9D, 
0x25, 
0x3B, 
0xF5, 
0x82, 
0xE4, 
0x34, 
0x0A, 
0xF5, 
0x83, 
0xE0, 
0xFD, 
0xC3, 
0xEF, 
0x9D, 
0xFE, 
0xE4, 
0x94, 
0x00, 
0x90, 
0x0B, 
0xCA, 
0xF0, 
0xA3, 
0xCE, 
0xF0, 
0x80, 
0x30, 
0x90, 
0x0A, 
0x9A, 
0xE0, 
0xFB, 
0xA3, 
0xE0, 
0xFA, 
0xA3, 
0xE0, 
0xF9, 
0x85, 
0x3B, 
0x82, 
0x75, 
0x83, 
0x00, 
0x12, 
0x0A, 
0xB8, 
0xFF, 
0x74, 
0xA4, 
0x25, 
0x3B, 
0xF5, 
0x82, 
0xE4, 
0x34, 
0x0A, 
0xF5, 
0x83, 
0xE0, 
0xFD, 
0xC3, 
0xEF, 
0x9D, 
0xFE, 
0xE4, 
0x94, 
0x00, 
0x90, 
0x0B, 
0xCA, 
0xF0, 
0xA3, 
0xCE, 
0xF0, 
0x90, 
0x07, 
0x83, 
0xE0, 
0xFF, 
0x7E, 
0x00, 
0x90, 
0x0D, 
0xF6, 
0xEE, 
0xF0, 
0xA3, 
0xEF, 
0xF0, 
0x90, 
0x0B, 
0xCA, 
0xE0, 
0xFC, 
0xA3, 
0xE0, 
0xFD, 
0xD3, 
0x9F, 
0x74, 
0x80, 
0xF8, 
0xEC, 
0x64, 
0x80, 
0x98, 
0x40, 
0x0C, 
0x90, 
0x0B, 
0xC8, 
0xE0, 
0x04, 
0xF0, 
0xA3, 
0xE0, 
0x04, 
0xF0, 
0x80, 
0x26, 
0x90, 
0x0D, 
0xF6, 
0xE0, 
0xFE, 
0xA3, 
0xE0, 
0xFF, 
0xC3, 
0xE4, 
0x9F, 
0xFF, 
0xE4, 
0x9E, 
0xFE, 
0xC3, 
0xED, 
0x9F, 
0xEE, 
0x64, 
0x80, 
0xF8, 
0xEC, 
0x64, 
0x80, 
0x98, 
0x50, 
0x0A, 
0x90, 
0x0B, 
0xC8, 
0xE0, 
0x14, 
0xF0, 
0xA3, 
0xE0, 
0x04, 
0xF0, 
0x05, 
0x3B, 
0x02, 
0xF5, 
0x02, 
0x90, 
0x08, 
0x58, 
0x02, 
0x9D, 
0x50, 

};
static u8 MCU_Setting2[] = 
{ 
0xBA,
0x75,
0x00,
0x00,
0x02,
0x00,
0x00,
0x01,
0xA2,
0x8F,
0x00,
0xCB,
0x03,
0x00,
0x00,
0x00,
0xE6,
0x6B,
0x02,
0x6B,
0x02,
0x00,
0x00,
0x01,
0xAC,
0x70,
0x00,
0xC5,
0x03,
0x00,
0x00,
0x00,
0x9C,
0x5B,
0x00,
0xBF,
0x02,
0x00,
0x00,
0x00,
0x60,
0x1C,
0x01,
0x37,
0x02,
0x00,
0x00,
0x01,
0xBA,
0x70,
0x00,
0x00,
0x03,
0x00,
0x00,
0x00,
0x21,
0x3F,
0x02,
0x40,
0x02,
0x00,
0x00,
0x00,
0x21,
0xEA,
0x02,
0x43,
0x02,
0x00,
0x00,
0x00,
0xA6,
0x12,
0x02,
0x46,
0x02,
0x00,
0x00,
0x00,
0x29,
0xE3,
0x02,
0x49,
0x02,
0x00,
0x00,
0x01,
0x9C,
0x6E,
0x05,
0x00,
0x02,
0x00,
0x00,
0x01,
0xA2,
0x66,
0x02,
0x35,
0x02,
0x00,
0x00,
0x01,
0xB5,
0xC2,
0x02,
0x9B,
0x02,
0x00,
0x00,
0x01,
0xA2,
0xD4,
0x02,
0xBE,
0x02,
0x00,
0x00,
0x01,
0xB7,
0xEA,
0x00,
0x02,
0x03,
0x00,
0x00,
0x00,
0x67,
0x31,
0x02,
0xF7,
0x02,
0x00,
0x00,
0x00,
0x66,
0xED,
0x03,
0x1D,
0x02,
0x00,
0x00,
0x00,
0x67,
0x73,
0x03,
0xD3,
0x02,
0x00,
0x00,
0x00,
0x20,
0x40,
0x03,
0x3B,
0x02,
0x00,
0x00,
0x00,
0xC5,
0xC0,
0x03,
0xF0,
0x02,
0x00,
0x00,
0x00,
0x41,
0xB3,
0x00,
0xA2,
0x02,
0x00,
0x00,
0x00,
0x44,
0xBA,
0x00,
0xF0,
0x03,
0x00,
0x00,
0x00,
0x89,
0x99,
0x04,
0x00,
0x02,
0x00,
0x00,
0x00,
0xA7,
0x91,
0x04,
0x21,
0x02,
0x00,
0x00,
0x00,
0x3A,
0x51,
0x00,
0xA2,
0x02,
0x00,
0x00,
0x00,
0x86,
0x54,
0x04,
0x47,
0x02,

};

//static 
#if 1
void HM5065InitialSetting(void)
{
	

    UINT8 state=0x8F;
    UINT32 iteration = 100;
    int totalCnt = 0; 
    int sentCnt = 0; 
    int index=0; 
    u16 addr = 0xA000; 
    u8 buf[256]; 
    int len = 128; 
    
		HM5065_write_cmos_sensor(0xffff,0x01);    
    HM5065_write_cmos_sensor(0x9000,0x03); 
    
		totalCnt = ARRAY_SIZE(MCU_Setting1); 
    while (index < totalCnt) {        
        sentCnt = totalCnt - index > len  ? len  : totalCnt - index; 
        //SENSORDB("Index = %d, sent Cnt = %d, addr = 0x%x\n", index, sentCnt, addr); 
        buf[0] = addr >> 8; 
        buf[1] = addr & 0xff; 
        memcpy(&buf[2], &MCU_Setting1[index], len );
        HM5065_burst_write_cmos_sensor(buf, sentCnt + 2); 
        addr += len ; 
        index += len ;
    }	
#if 0
	int i=0;
    for (i = 0xA000; i < 0xA000 + totalCnt; i++) {
        HM5065SENSORDB("Addr = 0x%x, data = 0x%02x\n", i, HM5065_read_cmos_sensor(i)); 
    }
#endif     
    
    
    addr = 0x9006; 
    index=0;
 		totalCnt = ARRAY_SIZE(MCU_Setting2); 
    while (index < totalCnt) {        
        sentCnt = totalCnt - index > len  ? len  : totalCnt - index; 
        //SENSORDB("Index = %d, sent Cnt = %d, addr = 0x%x\n", index, sentCnt, addr); 
        buf[0] = addr >> 8; 
        buf[1] = addr & 0xff; 
        memcpy(&buf[2], &MCU_Setting2[index], len );
        HM5065_burst_write_cmos_sensor(buf, sentCnt + 2); 
        addr += len ; 
        index += len ;
    }	   
#if 0
    for (i = 0x9006; i < 0x9006 + totalCnt; i++) {
        HM5065SENSORDB("Addr = 0x%x, data = 0x%02x\n", i, HM5065_read_cmos_sensor(i)); 
    }
#endif         
    
 	HM5065_write_cmos_sensor(0x9000,0x01);
	HM5065_write_cmos_sensor(0xffff,0x00);
	mdelay(200);   
	HM5065_write_cmos_sensor(0x0009,0x17);
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
	HM5065_write_cmos_sensor(0x0083,0x01);//00 by jink 20131104
	HM5065_write_cmos_sensor(0x0084,0x01);//00 by jink 20131104
	HM5065_write_cmos_sensor(0x0085,0x02);
	HM5065_write_cmos_sensor(0x00B2,0x50);
	HM5065_write_cmos_sensor(0x00B3,0x80);
	HM5065_write_cmos_sensor(0x00B4,0x01);
	HM5065_write_cmos_sensor(0x00B5,0x02);
	HM5065_write_cmos_sensor(0x00E8,0x01);
	HM5065_write_cmos_sensor(0x00ED,0x05);
	HM5065_write_cmos_sensor(0x00EE,0x1E);
	HM5065_write_cmos_sensor(0x0129,0x00);
	HM5065_write_cmos_sensor(0x0130,0xfd);//00->fe->f9
	HM5065_write_cmos_sensor(0x019C,0x4B);
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
	HM5065_write_cmos_sensor(0x7104,0x02);
	HM5065_write_cmos_sensor(0x7105,0x00);
	HM5065_write_cmos_sensor(0x7158,0x00);
	HM5065_write_cmos_sensor(0x0143,0x5F);
	HM5065_write_cmos_sensor(0x0144,0x0D);
	HM5065_write_cmos_sensor(0x02C2,0x00);
	HM5065_write_cmos_sensor(0x02C3,0xc0);
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
	HM5065_write_cmos_sensor(0x004C,0x10);
	HM5065_write_cmos_sensor(0x006C,0x08);
	HM5065_write_cmos_sensor(0x0350,0x00);
	HM5065_write_cmos_sensor(0x0351,0x5A);
	HM5065_write_cmos_sensor(0x0352,0xEE);
	HM5065_write_cmos_sensor(0x0353,0x69);
	HM5065_write_cmos_sensor(0x0354,0x49);
	HM5065_write_cmos_sensor(0x0355,0x39);
	HM5065_write_cmos_sensor(0x0356,0x6D);
	HM5065_write_cmos_sensor(0x0357,0x29); //19
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
	HM5065_write_cmos_sensor(0x0098,0x00);
	HM5065_write_cmos_sensor(0x00A0,0x5A);
	HM5065_write_cmos_sensor(0x00A1,0xEE);
	HM5065_write_cmos_sensor(0x00A2,0x3E);
	HM5065_write_cmos_sensor(0x00A3,0x00);
	HM5065_write_cmos_sensor(0x00A4,0x69);
	HM5065_write_cmos_sensor(0x00A5,0x49);
	HM5065_write_cmos_sensor(0x00A6,0x3B);
	HM5065_write_cmos_sensor(0x00A7,0x80);
	HM5065_write_cmos_sensor(0x00A8,0x00);
	HM5065_write_cmos_sensor(0x03EC,0x39);
	HM5065_write_cmos_sensor(0x03ED,0x85);
	HM5065_write_cmos_sensor(0x03FC,0x3A);
	HM5065_write_cmos_sensor(0x03FD,0x14);
	HM5065_write_cmos_sensor(0x040C,0x3A);
	HM5065_write_cmos_sensor(0x040D,0xF6);
	HM5065_write_cmos_sensor(0x041C,0x3B);
	HM5065_write_cmos_sensor(0x041D,0x9A);
	HM5065_write_cmos_sensor(0x03E0,0xB6);
	HM5065_write_cmos_sensor(0x03E1,0x04);
	HM5065_write_cmos_sensor(0x03E2,0xBB);
	HM5065_write_cmos_sensor(0x03E3,0xE9);
	HM5065_write_cmos_sensor(0x03E4,0xBC);
	HM5065_write_cmos_sensor(0x03E5,0x70);
	HM5065_write_cmos_sensor(0x03E6,0x37);
	HM5065_write_cmos_sensor(0x03E7,0x02);
	HM5065_write_cmos_sensor(0x03E8,0xBC);
	HM5065_write_cmos_sensor(0x03E9,0x00);
	HM5065_write_cmos_sensor(0x03EA,0xBF);
	HM5065_write_cmos_sensor(0x03EB,0x12);
	HM5065_write_cmos_sensor(0x03F0,0xBA);
	HM5065_write_cmos_sensor(0x03F1,0x7B);
	HM5065_write_cmos_sensor(0x03F2,0xBA);
	HM5065_write_cmos_sensor(0x03F3,0x83);
	HM5065_write_cmos_sensor(0x03F4,0xBB);
	HM5065_write_cmos_sensor(0x03F5,0xBC);
	HM5065_write_cmos_sensor(0x03F6,0x38);
	HM5065_write_cmos_sensor(0x03F7,0x2D);
	HM5065_write_cmos_sensor(0x03F8,0xBB);
	HM5065_write_cmos_sensor(0x03F9,0x23);
	HM5065_write_cmos_sensor(0x03FA,0xBD);
	HM5065_write_cmos_sensor(0x0400,0xBE);
	HM5065_write_cmos_sensor(0x0401,0x96);
	HM5065_write_cmos_sensor(0x0402,0xB9);
	HM5065_write_cmos_sensor(0x0403,0xBE);
	HM5065_write_cmos_sensor(0x0404,0xBB);
	HM5065_write_cmos_sensor(0x0405,0x57);
	HM5065_write_cmos_sensor(0x0406,0x3A);
	HM5065_write_cmos_sensor(0x0407,0xBB);
	HM5065_write_cmos_sensor(0x0408,0xB3);
	HM5065_write_cmos_sensor(0x0409,0x17);
	HM5065_write_cmos_sensor(0x040A,0xBE);
	HM5065_write_cmos_sensor(0x040B,0x66);
	HM5065_write_cmos_sensor(0x0410,0xBB);
	HM5065_write_cmos_sensor(0x0411,0x2A);
	HM5065_write_cmos_sensor(0x0412,0xBA);
	HM5065_write_cmos_sensor(0x0413,0x00);
	HM5065_write_cmos_sensor(0x0414,0xBB);
	HM5065_write_cmos_sensor(0x0415,0x10);
	HM5065_write_cmos_sensor(0x0416,0xB8);
	HM5065_write_cmos_sensor(0x0417,0xCD);
	HM5065_write_cmos_sensor(0x0418,0xB7);
	HM5065_write_cmos_sensor(0x0419,0x5C);
	HM5065_write_cmos_sensor(0x041A,0xBB);
	HM5065_write_cmos_sensor(0x041B,0x6C);
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
	HM5065_write_cmos_sensor(0x0081,0x60); //6E
	HM5065_write_cmos_sensor(0x0588,0x00);
	HM5065_write_cmos_sensor(0x0589,0x5A);
	HM5065_write_cmos_sensor(0x058A,0xEE);
	HM5065_write_cmos_sensor(0x058B,0x69);
	HM5065_write_cmos_sensor(0x058C,0x49);
	HM5065_write_cmos_sensor(0x058D,0x3D);
	HM5065_write_cmos_sensor(0x058E,0x3D);
	HM5065_write_cmos_sensor(0x0080,0x64);
	HM5065_write_cmos_sensor(0x0082,0x5A);
// AF register optimuzation 
		HM5065_write_cmos_sensor(0x0658,0x00); // 2013-01-04
		HM5065_write_cmos_sensor(0x0659,0x01);//01
		HM5065_write_cmos_sensor(0x075A,0x00);//00 ->01
		HM5065_write_cmos_sensor(0x0756,0x00); //03->00,
		HM5065_write_cmos_sensor(0x06D5,0x01);
		HM5065_write_cmos_sensor(0x06D6,0x60);//60->BC->66
		HM5065_write_cmos_sensor(0x06D7,0x00);
		HM5065_write_cmos_sensor(0x06D8,0x50);//93
		HM5065_write_cmos_sensor(0x06D9,0x00);
		HM5065_write_cmos_sensor(0x06DA,0x50);//93
		HM5065_write_cmos_sensor(0x065A,0x00);
		HM5065_write_cmos_sensor(0x06C9,0x01);
		HM5065_write_cmos_sensor(0x06CD,0x01);
		HM5065_write_cmos_sensor(0x06CE,0x63);
		HM5065_write_cmos_sensor(0x06CF,0x00);
		HM5065_write_cmos_sensor(0x06D0,0x74);
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
		HM5065_write_cmos_sensor(0x0770,0xC8); // 98->b5
	 	HM5065_write_cmos_sensor(0x0771,0x21);
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
	HM5065_write_cmos_sensor(0x0010,0x02);
	mdelay(100);

	HM5065_write_cmos_sensor(0x4708,0x00);
	HM5065_write_cmos_sensor(0x4709,0x00);
	HM5065_write_cmos_sensor(0x4710,0x00);
	HM5065_write_cmos_sensor(0x4711,0x00);

	//HM5065_write_cmos_sensor(0x0009,0x17);//MCLK=26Mhz ,

HM5065_write_cmos_sensor(0x0420, 0xff);
HM5065_write_cmos_sensor(0x0421, 0x77);
HM5065_write_cmos_sensor(0x0422, 0x00);
HM5065_write_cmos_sensor(0x0423, 0x53);
HM5065_write_cmos_sensor(0x0424, 0x00);
HM5065_write_cmos_sensor(0x0425, 0x8f);
HM5065_write_cmos_sensor(0x0426, 0x00);
HM5065_write_cmos_sensor(0x0427, 0xa9);
HM5065_write_cmos_sensor(0x0428, 0x00);
HM5065_write_cmos_sensor(0x0429, 0x9f);
HM5065_write_cmos_sensor(0x042a, 0x01);
HM5065_write_cmos_sensor(0x042b, 0x0b);
HM5065_write_cmos_sensor(0x042c, 0x00);
HM5065_write_cmos_sensor(0x042d, 0x46);
HM5065_write_cmos_sensor(0x042e, 0x00);
HM5065_write_cmos_sensor(0x042f, 0x0f);
HM5065_write_cmos_sensor(0x0430, 0xff);
HM5065_write_cmos_sensor(0x0431, 0x63);
HM5065_write_cmos_sensor(0x0432, 0x00);
HM5065_write_cmos_sensor(0x0433, 0x1c);
HM5065_write_cmos_sensor(0x0434, 0x00);
HM5065_write_cmos_sensor(0x0435, 0xc8);
HM5065_write_cmos_sensor(0x0436, 0x00);
HM5065_write_cmos_sensor(0x0437, 0xde);
HM5065_write_cmos_sensor(0x0438, 0xff);
HM5065_write_cmos_sensor(0x0439, 0xe4);
HM5065_write_cmos_sensor(0x043a, 0x00);
HM5065_write_cmos_sensor(0x043b, 0x9d);
HM5065_write_cmos_sensor(0x043c, 0xff);
HM5065_write_cmos_sensor(0x043d, 0x05);
HM5065_write_cmos_sensor(0x043e, 0x00);
HM5065_write_cmos_sensor(0x043f, 0x19);
HM5065_write_cmos_sensor(0x0450, 0xff);
HM5065_write_cmos_sensor(0x0451, 0xd6);
HM5065_write_cmos_sensor(0x0452, 0x00);
HM5065_write_cmos_sensor(0x0453, 0x1e);
HM5065_write_cmos_sensor(0x0454, 0x00);
HM5065_write_cmos_sensor(0x0455, 0x77);
HM5065_write_cmos_sensor(0x0456, 0x00);
HM5065_write_cmos_sensor(0x0457, 0x90);
HM5065_write_cmos_sensor(0x0458, 0xff);
HM5065_write_cmos_sensor(0x0459, 0x84);
HM5065_write_cmos_sensor(0x045a, 0x00);
HM5065_write_cmos_sensor(0x045b, 0x02);
HM5065_write_cmos_sensor(0x045c, 0x00);
HM5065_write_cmos_sensor(0x045d, 0x01);
HM5065_write_cmos_sensor(0x045e, 0x00);
HM5065_write_cmos_sensor(0x045f, 0x32);
HM5065_write_cmos_sensor(0x0440, 0xff);
HM5065_write_cmos_sensor(0x0441, 0x30);
HM5065_write_cmos_sensor(0x0442, 0x00);
HM5065_write_cmos_sensor(0x0443, 0x58);
HM5065_write_cmos_sensor(0x0444, 0x00);
HM5065_write_cmos_sensor(0x0445, 0x99);
HM5065_write_cmos_sensor(0x0446, 0x00);
HM5065_write_cmos_sensor(0x0447, 0xa6);
HM5065_write_cmos_sensor(0x0448, 0x00);
HM5065_write_cmos_sensor(0x0449, 0xe3);
HM5065_write_cmos_sensor(0x044a, 0x00);
HM5065_write_cmos_sensor(0x044b, 0x97);
HM5065_write_cmos_sensor(0x044c, 0xff);
HM5065_write_cmos_sensor(0x044d, 0x5f);
HM5065_write_cmos_sensor(0x044e, 0x00);
HM5065_write_cmos_sensor(0x044f, 0x02);
HM5065_write_cmos_sensor(0x0460, 0xff);
HM5065_write_cmos_sensor(0x0461, 0x77);
HM5065_write_cmos_sensor(0x0462, 0x00);
HM5065_write_cmos_sensor(0x0463, 0x53);
HM5065_write_cmos_sensor(0x0464, 0x00);
HM5065_write_cmos_sensor(0x0465, 0x8f);
HM5065_write_cmos_sensor(0x0466, 0x00);
HM5065_write_cmos_sensor(0x0467, 0xa9);
HM5065_write_cmos_sensor(0x0468, 0x00);
HM5065_write_cmos_sensor(0x0469, 0x9f);
HM5065_write_cmos_sensor(0x046a, 0x01);
HM5065_write_cmos_sensor(0x046b, 0x0b);
HM5065_write_cmos_sensor(0x046c, 0x00);
HM5065_write_cmos_sensor(0x046d, 0x46);
HM5065_write_cmos_sensor(0x046e, 0x00);
HM5065_write_cmos_sensor(0x046f, 0x0f);
HM5065_write_cmos_sensor(0x0470, 0xff);
HM5065_write_cmos_sensor(0x0471, 0x63);
HM5065_write_cmos_sensor(0x0472, 0x00);
HM5065_write_cmos_sensor(0x0473, 0x1c);
HM5065_write_cmos_sensor(0x0474, 0x00);
HM5065_write_cmos_sensor(0x0475, 0xc8);
HM5065_write_cmos_sensor(0x0476, 0x00);
HM5065_write_cmos_sensor(0x0477, 0xde);
HM5065_write_cmos_sensor(0x0478, 0xff);
HM5065_write_cmos_sensor(0x0479, 0xe4);
HM5065_write_cmos_sensor(0x047a, 0x00);
HM5065_write_cmos_sensor(0x047b, 0x9d);
HM5065_write_cmos_sensor(0x047c, 0xff);
HM5065_write_cmos_sensor(0x047d, 0x05);
HM5065_write_cmos_sensor(0x047e, 0x00);
HM5065_write_cmos_sensor(0x047f, 0x19);
HM5065_write_cmos_sensor(0x0490, 0xff);
HM5065_write_cmos_sensor(0x0491, 0xd6);
HM5065_write_cmos_sensor(0x0492, 0x00);
HM5065_write_cmos_sensor(0x0493, 0x1e);
HM5065_write_cmos_sensor(0x0494, 0x00);
HM5065_write_cmos_sensor(0x0495, 0x77);
HM5065_write_cmos_sensor(0x0496, 0x00);
HM5065_write_cmos_sensor(0x0497, 0x90);
HM5065_write_cmos_sensor(0x0498, 0xff);
HM5065_write_cmos_sensor(0x0499, 0x84);
HM5065_write_cmos_sensor(0x049a, 0x00);
HM5065_write_cmos_sensor(0x049b, 0x02);
HM5065_write_cmos_sensor(0x049c, 0x00);
HM5065_write_cmos_sensor(0x049d, 0x01);
HM5065_write_cmos_sensor(0x049e, 0x00);
HM5065_write_cmos_sensor(0x049f, 0x32);
HM5065_write_cmos_sensor(0x0480, 0xff);
HM5065_write_cmos_sensor(0x0481, 0x30);
HM5065_write_cmos_sensor(0x0482, 0x00);
HM5065_write_cmos_sensor(0x0483, 0x58);
HM5065_write_cmos_sensor(0x0484, 0x00);
HM5065_write_cmos_sensor(0x0485, 0x99);
HM5065_write_cmos_sensor(0x0486, 0x00);
HM5065_write_cmos_sensor(0x0487, 0xa6);
HM5065_write_cmos_sensor(0x0488, 0x00);
HM5065_write_cmos_sensor(0x0489, 0xe3);
HM5065_write_cmos_sensor(0x048a, 0x00);
HM5065_write_cmos_sensor(0x048b, 0x97);
HM5065_write_cmos_sensor(0x048c, 0xff);
HM5065_write_cmos_sensor(0x048d, 0x5f);
HM5065_write_cmos_sensor(0x048e, 0x00);
HM5065_write_cmos_sensor(0x048f, 0x02);
HM5065_write_cmos_sensor(0x04a0, 0xff);
HM5065_write_cmos_sensor(0x04a1, 0x77);
HM5065_write_cmos_sensor(0x04a2, 0x00);
HM5065_write_cmos_sensor(0x04a3, 0x53);
HM5065_write_cmos_sensor(0x04a4, 0x00);
HM5065_write_cmos_sensor(0x04a5, 0x8f);
HM5065_write_cmos_sensor(0x04a6, 0x00);
HM5065_write_cmos_sensor(0x04a7, 0xa9);
HM5065_write_cmos_sensor(0x04a8, 0x00);
HM5065_write_cmos_sensor(0x04a9, 0x9f);
HM5065_write_cmos_sensor(0x04aa, 0x01);
HM5065_write_cmos_sensor(0x04ab, 0x0b);
HM5065_write_cmos_sensor(0x04ac, 0x00);
HM5065_write_cmos_sensor(0x04ad, 0x46);
HM5065_write_cmos_sensor(0x04ae, 0x00);
HM5065_write_cmos_sensor(0x04af, 0x0f);
HM5065_write_cmos_sensor(0x04b0, 0xff);
HM5065_write_cmos_sensor(0x04b1, 0x63);
HM5065_write_cmos_sensor(0x04b2, 0x00);
HM5065_write_cmos_sensor(0x04b3, 0x1c);
HM5065_write_cmos_sensor(0x04b4, 0x00);
HM5065_write_cmos_sensor(0x04b5, 0xc8);
HM5065_write_cmos_sensor(0x04b6, 0x00);
HM5065_write_cmos_sensor(0x04b7, 0xde);
HM5065_write_cmos_sensor(0x04b8, 0xff);
HM5065_write_cmos_sensor(0x04b9, 0xe4);
HM5065_write_cmos_sensor(0x04ba, 0x00);
HM5065_write_cmos_sensor(0x04bb, 0x9d);
HM5065_write_cmos_sensor(0x04bc, 0xff);
HM5065_write_cmos_sensor(0x04bd, 0x05);
HM5065_write_cmos_sensor(0x04be, 0x00);
HM5065_write_cmos_sensor(0x04bf, 0x19);
HM5065_write_cmos_sensor(0x04d0, 0xff);
HM5065_write_cmos_sensor(0x04d1, 0xd6);
HM5065_write_cmos_sensor(0x04d2, 0x00);
HM5065_write_cmos_sensor(0x04d3, 0x1e);
HM5065_write_cmos_sensor(0x04d4, 0x00);
HM5065_write_cmos_sensor(0x04d5, 0x77);
HM5065_write_cmos_sensor(0x04d6, 0x00);
HM5065_write_cmos_sensor(0x04d7, 0x90);
HM5065_write_cmos_sensor(0x04d8, 0xff);
HM5065_write_cmos_sensor(0x04d9, 0x84);
HM5065_write_cmos_sensor(0x04da, 0x00);
HM5065_write_cmos_sensor(0x04db, 0x02);
HM5065_write_cmos_sensor(0x04dc, 0x00);
HM5065_write_cmos_sensor(0x04dd, 0x01);
HM5065_write_cmos_sensor(0x04de, 0x00);
HM5065_write_cmos_sensor(0x04df, 0x32);
HM5065_write_cmos_sensor(0x04c0, 0xff);
HM5065_write_cmos_sensor(0x04c1, 0x30);
HM5065_write_cmos_sensor(0x04c2, 0x00);
HM5065_write_cmos_sensor(0x04c3, 0x58);
HM5065_write_cmos_sensor(0x04c4, 0x00);
HM5065_write_cmos_sensor(0x04c5, 0x99);
HM5065_write_cmos_sensor(0x04c6, 0x00);
HM5065_write_cmos_sensor(0x04c7, 0xa6);
HM5065_write_cmos_sensor(0x04c8, 0x00);
HM5065_write_cmos_sensor(0x04c9, 0xe3);
HM5065_write_cmos_sensor(0x04ca, 0x00);
HM5065_write_cmos_sensor(0x04cb, 0x97);
HM5065_write_cmos_sensor(0x04cc, 0xff);
HM5065_write_cmos_sensor(0x04cd, 0x5f);
HM5065_write_cmos_sensor(0x04ce, 0x00);
HM5065_write_cmos_sensor(0x04cf, 0x02);
HM5065_write_cmos_sensor(0x04e0, 0xff);
HM5065_write_cmos_sensor(0x04e1, 0x77);
HM5065_write_cmos_sensor(0x04e2, 0x00);
HM5065_write_cmos_sensor(0x04e3, 0x53);
HM5065_write_cmos_sensor(0x04e4, 0x00);
HM5065_write_cmos_sensor(0x04e5, 0x8f);
HM5065_write_cmos_sensor(0x04e6, 0x00);
HM5065_write_cmos_sensor(0x04e7, 0xa9);
HM5065_write_cmos_sensor(0x04e8, 0x00);
HM5065_write_cmos_sensor(0x04e9, 0x9f);
HM5065_write_cmos_sensor(0x04ea, 0x01);
HM5065_write_cmos_sensor(0x04eb, 0x0b);
HM5065_write_cmos_sensor(0x04ec, 0x00);
HM5065_write_cmos_sensor(0x04ed, 0x46);
HM5065_write_cmos_sensor(0x04ee, 0x00);
HM5065_write_cmos_sensor(0x04ef, 0x0f);
HM5065_write_cmos_sensor(0x04f0, 0xff);
HM5065_write_cmos_sensor(0x04f1, 0x63);
HM5065_write_cmos_sensor(0x04f2, 0x00);
HM5065_write_cmos_sensor(0x04f3, 0x1c);
HM5065_write_cmos_sensor(0x04f4, 0x00);
HM5065_write_cmos_sensor(0x04f5, 0xc8);
HM5065_write_cmos_sensor(0x04f6, 0x00);
HM5065_write_cmos_sensor(0x04f7, 0xde);
HM5065_write_cmos_sensor(0x04f8, 0xff);
HM5065_write_cmos_sensor(0x04f9, 0xe4);
HM5065_write_cmos_sensor(0x04fa, 0x00);
HM5065_write_cmos_sensor(0x04fb, 0x9d);
HM5065_write_cmos_sensor(0x04fc, 0xff);
HM5065_write_cmos_sensor(0x04fd, 0x05);
HM5065_write_cmos_sensor(0x04fe, 0xff);
HM5065_write_cmos_sensor(0x04ff, 0x8b);
HM5065_write_cmos_sensor(0x0510, 0xff);
HM5065_write_cmos_sensor(0x0511, 0xd6);
HM5065_write_cmos_sensor(0x0512, 0x00);
HM5065_write_cmos_sensor(0x0513, 0x1e);
HM5065_write_cmos_sensor(0x0514, 0x00);
HM5065_write_cmos_sensor(0x0515, 0x77);
HM5065_write_cmos_sensor(0x0516, 0x00);
HM5065_write_cmos_sensor(0x0517, 0x90);
HM5065_write_cmos_sensor(0x0518, 0xff);
HM5065_write_cmos_sensor(0x0519, 0x84);
HM5065_write_cmos_sensor(0x051a, 0x00);
HM5065_write_cmos_sensor(0x051b, 0x02);
HM5065_write_cmos_sensor(0x051c, 0x00);
HM5065_write_cmos_sensor(0x051d, 0x01);
HM5065_write_cmos_sensor(0x051e, 0x00);
HM5065_write_cmos_sensor(0x051f, 0x32);
HM5065_write_cmos_sensor(0x0500, 0xff);
HM5065_write_cmos_sensor(0x0501, 0x30);
HM5065_write_cmos_sensor(0x0502, 0x00);
HM5065_write_cmos_sensor(0x0503, 0x58);
HM5065_write_cmos_sensor(0x0504, 0x00);
HM5065_write_cmos_sensor(0x0505, 0x99);
HM5065_write_cmos_sensor(0x0506, 0x00);
HM5065_write_cmos_sensor(0x0507, 0xa6);
HM5065_write_cmos_sensor(0x0508, 0x00);
HM5065_write_cmos_sensor(0x0509, 0xe3);
HM5065_write_cmos_sensor(0x050a, 0x00);
HM5065_write_cmos_sensor(0x050b, 0x97);
HM5065_write_cmos_sensor(0x050c, 0xff);
HM5065_write_cmos_sensor(0x050d, 0x5f);
HM5065_write_cmos_sensor(0x050e, 0x00);
HM5065_write_cmos_sensor(0x050f, 0x02);
HM5065_write_cmos_sensor(0x0561, 0x18);
HM5065_write_cmos_sensor(0x0562, 0x18);
HM5065_write_cmos_sensor(0x0563, 0x18);
HM5065_write_cmos_sensor(0x0564, 0x18);

HM5065_write_cmos_sensor(0x0324,0x39);
HM5065_write_cmos_sensor(0x0325,0xAE);
HM5065_write_cmos_sensor(0x0326,0x3A);
HM5065_write_cmos_sensor(0x0327,0x29);
HM5065_write_cmos_sensor(0x0328,0x3B);
HM5065_write_cmos_sensor(0x0329,0x0A);
HM5065_write_cmos_sensor(0x032A,0x3B);
HM5065_write_cmos_sensor(0x032B,0x62);
HM5065_write_cmos_sensor(0x0320,0x01);
HM5065_write_cmos_sensor(0x0321,0x04);
HM5065_write_cmos_sensor(0x0322,0x01);
HM5065_write_cmos_sensor(0x0323,0x01);
HM5065_write_cmos_sensor(0x0330, 0x01);//Adaptive Colour Matrices
HM5065_write_cmos_sensor(0x0384, 0x00);
HM5065_write_cmos_sensor(0x0337, 0x01);
	HM5065_write_cmos_sensor(0x0754,0x01);
	HM5065_write_cmos_sensor(0x0773,0x08);
	HM5065_write_cmos_sensor(0x0779,0x00);
	HM5065_write_cmos_sensor(0x077a,0x03);
	HM5065_write_cmos_sensor(0x0846,0x0a);
//20130702
#if 1
HM5065_write_cmos_sensor(0x03E0,0xB6);
HM5065_write_cmos_sensor(0x03E1,0x04);
HM5065_write_cmos_sensor(0x03E2,0xBB);
HM5065_write_cmos_sensor(0x03E3,0xE9);
HM5065_write_cmos_sensor(0x03E4,0xBC);
HM5065_write_cmos_sensor(0x03E5,0x70);
HM5065_write_cmos_sensor(0x03E6,0x39);
HM5065_write_cmos_sensor(0x03E7,0x95);
HM5065_write_cmos_sensor(0x03E8,0xBC);
HM5065_write_cmos_sensor(0x03E9,0x00);
HM5065_write_cmos_sensor(0x03EA,0xBF);
HM5065_write_cmos_sensor(0x03EB,0x12);
HM5065_write_cmos_sensor(0x03F0,0xBA);
HM5065_write_cmos_sensor(0x03F1,0x7B);
HM5065_write_cmos_sensor(0x03F2,0xBA);
HM5065_write_cmos_sensor(0x03F3,0x83);
HM5065_write_cmos_sensor(0x03F4,0xBB);
HM5065_write_cmos_sensor(0x03F5,0xBC);
HM5065_write_cmos_sensor(0x03F6,0x39);
HM5065_write_cmos_sensor(0x03F7,0x95);
HM5065_write_cmos_sensor(0x03F8,0xBB);
HM5065_write_cmos_sensor(0x03F9,0x23);
HM5065_write_cmos_sensor(0x03FA,0xBD);
HM5065_write_cmos_sensor(0x03FB,0xAC);
HM5065_write_cmos_sensor(0x0400,0xBD);
HM5065_write_cmos_sensor(0x0401,0xF9);
HM5065_write_cmos_sensor(0x0402,0xB9);
HM5065_write_cmos_sensor(0x0403,0xBE);
HM5065_write_cmos_sensor(0x0404,0xBB);
HM5065_write_cmos_sensor(0x0405,0x56);
HM5065_write_cmos_sensor(0x0406,0x3A);
HM5065_write_cmos_sensor(0x0407,0xBA);
HM5065_write_cmos_sensor(0x0408,0xB3);
HM5065_write_cmos_sensor(0x0409,0x12);
HM5065_write_cmos_sensor(0x040A,0xBE);
HM5065_write_cmos_sensor(0x040B,0x66);
HM5065_write_cmos_sensor(0x0410,0xBB);
HM5065_write_cmos_sensor(0x0411,0x3E);
HM5065_write_cmos_sensor(0x0412,0x3A);
HM5065_write_cmos_sensor(0x0413,0x14);
HM5065_write_cmos_sensor(0x0414,0xBA);
HM5065_write_cmos_sensor(0x0415,0x6C);
HM5065_write_cmos_sensor(0x0416,0x39);
HM5065_write_cmos_sensor(0x0417,0x47);
HM5065_write_cmos_sensor(0x0418,0xB7);
HM5065_write_cmos_sensor(0x0419,0x5C);
HM5065_write_cmos_sensor(0x041A,0xBB);
HM5065_write_cmos_sensor(0x041B,0x6C);
#endif	
	HM5065_write_cmos_sensor(0x016c,0x00);	
	HM5065_write_cmos_sensor(0x0010,0x01);
	mdelay(100);								

		HM5065SENSORDB("[HM5065]exit HM5065InitialSetting function:\n ");
		return ERROR_NONE;
		
} 
#endif
/*****************************************************************
* FUNCTION
*    HM5065PreviewSetting
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
static void HM5065PreviewSetting_SVGA(void)
{
	//;HM5065 1280x960,30fps
	HM5065SENSORDB("[HM5065]enter HM5065PreviewSetting_SVGA function:\n ");
	
	HM5065_write_cmos_sensor(0x0142,0x00);//open AE
	//HM5065_write_cmos_sensor(0x01A4,0x00);//open awb
	
	//HM5065_write_cmos_sensor(0x0010,0x02);//sw reset
	//mdelay(200);
	//HM5065_write_cmos_sensor(0x00B5,0x01);//  01=>02	35Mhz  ;01: 70MHz;02:35MHz
	//HM5065_write_cmos_sensor(0x0030,0x11);//14

	HM5065_write_cmos_sensor(0x0040,0x01); 	//	binning mode and subsampling mode for frame rate 2*2 add 2? 4?
	HM5065_write_cmos_sensor(0x0041,0x0A); 	//	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
	HM5065_write_cmos_sensor(0x0042,0x05); 	//05	X:800 0x500=1280,0x0320=800
	HM5065_write_cmos_sensor(0x0043,0x00); 	//00
	HM5065_write_cmos_sensor(0x0044,0x03); 	//03	Y:600 0x03c0=960,0x0258=600
	HM5065_write_cmos_sensor(0x0045,0xc0); 	//c0
	HM5065_write_cmos_sensor(0x070A,0x01); //add by jink 20131104
#if 1
	HM5065_write_cmos_sensor(0x01A5, 0x3E);//Hue
	HM5065_write_cmos_sensor(0x01A6, 0x00);//HueRBias

#else
HM5065_write_cmos_sensor(0x01fa, 0x01);
	HM5065_write_cmos_sensor(0x01A5, 0x3d);//Hue
	HM5065_write_cmos_sensor(0x01A6, 0x71);//HueRBias 66->9a->71
	HM5065_write_cmos_sensor(0x01A7, 0x3e);
	HM5065_write_cmos_sensor(0x01A8, 0x00);
#endif	
	HM5065_write_cmos_sensor(0x0143, 0x5F);
	HM5065_write_cmos_sensor(0x0144, 0x0D);
	HM5065_write_cmos_sensor(0x02C2, 0x00);
	HM5065_write_cmos_sensor(0x02C3, 0xc0);
	HM5065_write_cmos_sensor(0x015E, 0x40);
	HM5065_write_cmos_sensor(0x015F, 0x00);
	HM5065_write_cmos_sensor(0x0251, 0x02);//BLC ON	
	HM5065_write_cmos_sensor(0x004c, 0x10);
	//HM5065_write_cmos_sensor(0x0010,0x01);
	//mdelay(200);
	
	if(HM5065_run_test_pattern)
	{
		HM5065_run_test_pattern=0;
		HM5065SetTestPatternMode(1);
	}
	//HM5065_write_cmos_sensor(0x4202, 0x00);//	; open mipi stream
	HM5065WriteExtraShutter(HM5065Sensor.PreviewExtraShutter);
	spin_lock(&HM5065_drv_lock);
	HM5065Sensor.SensorMode= SENSOR_MODE_PREVIEW;
	HM5065Sensor.IsPVmode = KAL_TRUE;
	HM5065Sensor.PreviewPclk= 560;	
	spin_unlock(&HM5065_drv_lock);
	HM5065SENSORDB("[HM5065]exit HM5065PreviewSetting_SVGA function:\n ");
}
static void HM5065ZSDPreviewSetting_QVGA(void)
{

}


/*************************************************************************
* FUNCTION
*     HM5065FullSizeCaptureSetting
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
static void HM5065FullSizeCaptureSetting(void)
{
HM5065_write_cmos_sensor(0x01fa, 0x01);
	
		HM5065_write_cmos_sensor(0x0142,0x01);//Freeze AE
		//HM5065_write_cmos_sensor(0x01A4,0x04);//Freeze AWB	
		//HM5065_write_cmos_sensor(0x01A4,0x04);//Freeze AWB	
	
		
		//HM5065_write_cmos_sensor(0x0010,0x02);
		//mdelay(200);
		//HM5065_write_cmos_sensor(0x00B5,0x01);//
		//HM5065_write_cmos_sensor(0x0030,0x11);	
		//mdelay(200);
		HM5065_write_cmos_sensor(0x0040,0x00); //Full size
		HM5065_write_cmos_sensor(0x0041,0x0A); //00:full size
		HM5065_write_cmos_sensor(0x0042,0x0A); //X:2592/2602
		HM5065_write_cmos_sensor(0x0043,0x20+0x8);
		HM5065_write_cmos_sensor(0x0044,0x07); //Y:1944/1954
		HM5065_write_cmos_sensor(0x0045,0x98+0x8);
		/*HM5065_write_cmos_sensor(0x0042,0x0A); //X:2592
		HM5065_write_cmos_sensor(0x0043,0x00);
		HM5065_write_cmos_sensor(0x0044,0x07); //Y:1944
		HM5065_write_cmos_sensor(0x0045,0x80);*/
		
		HM5065_write_cmos_sensor(0x0251,0x01);//BLC off 
		//HM5065_write_cmos_sensor(0x019C,0x4B);//50hz banding
		//HM5065_write_cmos_sensor(0x019D,0x20);
		//HM5065_write_cmos_sensor(0x0082,0x50);	//By Zeroy
	
		//HM5065_write_cmos_sensor(0x0010,0x02);
		// mdelay(200);
		//HM5065_write_cmos_sensor(0x0010,0x01);//Stream on
		//mdelay(200);


	
	if(HM5065_run_test_pattern)
	{
		HM5065_run_test_pattern=0;
		HM5065SetTestPatternMode(1);
	}
	//HM5065_write_cmos_sensor(0x4202, 0x00);//	; open mipi stream
	spin_lock(&HM5065_drv_lock);
	HM5065Sensor.IsPVmode = KAL_FALSE;
	HM5065Sensor.CapturePclk= 800;	
	//HM5065Sensor.SensorMode= SENSOR_MODE_CAPTURE;
	spin_unlock(&HM5065_drv_lock);
	HM5065SENSORDB("[HM5065]exit HM5065FullSizeCaptureSetting function:\n ");
}

static void HM5065FullSizeCaptureSetting_ZSD(void)
{

	
		//HM5065_write_cmos_sensor(0x0142,0x01);//Freeze AE// zsd , need open ae
		//HM5065_write_cmos_sensor(0x01A4,0x04);//Freeze AWB	
		//HM5065_write_cmos_sensor(0x01A4,0x04);//Freeze AWB	
	
		
		HM5065_write_cmos_sensor(0x0142,0x00);//Freeze AE
		HM5065_write_cmos_sensor(0x0010,0x02);
		//mdelay(200);
		//HM5065_write_cmos_sensor(0x00B5,0x01);
		//HM5065_write_cmos_sensor(0x0030,0x11);	
		//mdelay(200);

		HM5065_write_cmos_sensor(0x0040,0x00); //Full size
		HM5065_write_cmos_sensor(0x0041,0x0A); //00:full size
		HM5065_write_cmos_sensor(0x0042,0x0A); //X:2592/2602
		HM5065_write_cmos_sensor(0x0043,0x20+0x8);
		HM5065_write_cmos_sensor(0x0044,0x07); //Y:1944/1954
		HM5065_write_cmos_sensor(0x0045,0x98+0x8);

		//HM5065_write_cmos_sensor(0x0251,0x01);//BLC off 
		
		HM5065_write_cmos_sensor(0x0251,0x02);//BLC off 
		//HM5065_write_cmos_sensor(0x019C,0x4B);//50hz banding
		//HM5065_write_cmos_sensor(0x019D,0x20);
		//HM5065_write_cmos_sensor(0x0082,0x50);	//By Zeroy
	
		// HM5065_write_cmos_sensor(0x0010,0x02);
		// mdelay(200);
		HM5065_write_cmos_sensor(0x0010,0x01);//Stream on
		//mdelay(200);


	
	if(HM5065_run_test_pattern)
	{
		HM5065_run_test_pattern=0;
		HM5065SetTestPatternMode(1);
	}
	//HM5065_write_cmos_sensor(0x4202, 0x00);//	; open mipi stream
	spin_lock(&HM5065_drv_lock);
	HM5065Sensor.IsPVmode = KAL_FALSE;
	HM5065Sensor.CapturePclk= 800;	
	//HM5065Sensor.SensorMode= SENSOR_MODE_CAPTURE;
	spin_unlock(&HM5065_drv_lock);
	HM5065SENSORDB("[HM5065]exit HM5065FullSizeCaptureSetting function:\n ");
}

/*************************************************************************
* FUNCTION
*    HM5065SetHVMirror
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
static void HM5065SetHVMirror(kal_uint8 Mirror)
{
  	kal_uint8 mirror= 0, flip=0;
    HM5065SENSORDB("[HM5065]enter HM5065SetHVMirror function:\n ");
	//	flip = HM5065_read_cmos_sensor(0x3820);
	//	mirror=HM5065_read_cmos_sensor(0x3821);
	
	//if (SENSOR_MODE_PREVIEW)
	{
		switch (Mirror)
		{
		case IMAGE_NORMAL:
			//HM5065_write_cmos_sensor(0x3820, flip&0xf9);     
			//HM5065_write_cmos_sensor(0x3821, mirror&0xf9);
			//HM5065_write_cmos_sensor(0x4514, 0x00);
			break;
		case IMAGE_H_MIRROR:
			//HM5065_write_cmos_sensor(0x3820, flip&0xf9);     
			//HM5065_write_cmos_sensor(0x3821, mirror|0x06);
			//HM5065_write_cmos_sensor(0x4514, 0x00);
			break;
		case IMAGE_V_MIRROR: 
			//HM5065_write_cmos_sensor(0x3820, flip|0x06);     
			//HM5065_write_cmos_sensor(0x3821, mirror&0xf9);
			//HM5065_write_cmos_sensor(0x4514, 0x00);
			break;		
		case IMAGE_HV_MIRROR:
			//HM5065_write_cmos_sensor(0x3820, flip|0x06);     
			//HM5065_write_cmos_sensor(0x3821, mirror|0x06);
			//HM5065_write_cmos_sensor(0x4514, 0x00);
			break; 		
		default:
			ASSERT(0);
		}
	}
/*	else if (Mode== SENSOR_MODE_CAPTURE)
	{
		switch (Mirror)
		{
		case IMAGE_NORMAL:
			//HM5065_write_cmos_sensor(0x3820, flip&0xf9);     
			//HM5065_write_cmos_sensor(0x3821, mirror&0xf9);
			//HM5065_write_cmos_sensor(0x4514, 0x00);
			break;
		case IMAGE_H_MIRROR:
			//HM5065_write_cmos_sensor(0x3820, flip&0xf9);     
			//HM5065_write_cmos_sensor(0x3821, mirror|0x06);
			//HM5065_write_cmos_sensor(0x4514, 0x00);
			break;
		case IMAGE_V_MIRROR: 
			//HM5065_write_cmos_sensor(0x3820, flip|0x06);     
			//HM5065_write_cmos_sensor(0x3821, mirror&0xf9);
			//HM5065_write_cmos_sensor(0x4514, 0xaa);
			break;		
		case IMAGE_HV_MIRROR:
			//HM5065_write_cmos_sensor(0x3820, flip|0x06);     
			//HM5065_write_cmos_sensor(0x3821, mirror|0x06);
			//HM5065_write_cmos_sensor(0x4514, 0xbb);
			break; 		
		default:
			ASSERT(0);
		}
	}*/
	HM5065SENSORDB("[HM5065]exit HM5065SetHVMirror function:\n ");
}

void HM5065_Standby(void)
{
	//HM5065_write_cmos_sensor(0x3008,0x42);
}

void HM5065_Wakeup(void)
{
	//HM5065_write_cmos_sensor(0x3008,0x02);
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
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_FOCUS_OVT_AFC_Constant_Focus function:\n ");
	HM5065_write_cmos_sensor(0x0751,0x00);	
	HM5065_write_cmos_sensor(0x070A,0x01);
	HM5065SENSORDB("[HM5065]exit HM5065_FOCUS_OVT_AFC_Constant_Focus function:\n ");
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
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_FOCUS_OVT_AFC_Single_Focus function:\n ");

    HM5065SENSORDB("HM5065_FOCUS_Single_Focus\n");
    HM5065_write_cmos_sensor(0x0751,0x00);	
   HM5065_write_cmos_sensor(0x070A,0x03);
	mDELAY(200);
    HM5065_write_cmos_sensor(0x070B,0x01);;
	mDELAY(200);
    HM5065_write_cmos_sensor(0x070B,0x02);
	mDELAY(200);
    HM5065SENSORDB("after single focus  \n");
	HM5065SENSORDB("[HM5065]exit HM5065_FOCUS_OVT_AFC_Single_Focus function:\n ");
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
    HM5065SENSORDB(" *pFeatureReturnPara32 = %d\n",  *pFeatureReturnPara32);	
}

static void HM5065_FOCUS_Get_AE_Max_Num_Metering_Areas(UINT32 *pFeatureReturnPara32)
{ 	
    HM5065SENSORDB("[HM5065]enter HM5065_FOCUS_Get_AE_Max_Num_Metering_Areas function:\n ");
    *pFeatureReturnPara32 = 1;    
    HM5065SENSORDB(" *pFeatureReturnPara32 = %d\n",  *pFeatureReturnPara32);
	HM5065SENSORDB("[HM5065]exit HM5065_FOCUS_Get_AE_Max_Num_Metering_Areas function:\n ");
}
static void HM5065_FOCUS_OVT_AFC_Touch_AF(UINT32 x,UINT32 y)
{
	 /*HM5065SENSORDB("[HM5065]enter HM5065_FOCUS_OVT_AFC_Touch_AF function:\n ");
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
	 HM5065SENSORDB("[HM5065]AF x_view=%d,y_view=%d\n",x_view, y_view);
     //HM5065_write_cmos_sensor(0x3024,x_view);
     //HM5065_write_cmos_sensor(0x3025,y_view);   
     x_tmp = HM5065_read_cmos_sensor(0x3024);
	 y_tmp = HM5065_read_cmos_sensor(0x3025);
	 HM5065SENSORDB("[HM5065]AF x_tmp1=%d,y_tmp1=%d\n",x_tmp, y_tmp);
     HM5065SENSORDB("[HM5065]exit HM5065_FOCUS_OVT_AFC_Touch_AF function:\n ");*/
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
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_FOCUS_Set_AF_Window function:\n ");
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

	HM5065SENSORDB("Set_AF_Window x0=%d,y0=%d,x1=%d,y1=%d,FD_XS=%d,FD_YS=%d\n",
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
		
	HM5065SENSORDB("[HM5065]AF pvx0=%d,pvy0=%d\n",pvx0, pvy0);
	HM5065SENSORDB("[HM5065]AF pvx1=%d,pvy1=%d\n",pvx1, pvy1);
/*
		HM5065_write_cmos_sensor(0x0808,0x01);
		HM5065_write_cmos_sensor(0x0809,0x00); 
		HM5065_write_cmos_sensor(0x080a,0x00);
		HM5065_write_cmos_sensor(0x080b,0x00);  
		HM5065_write_cmos_sensor(0x080c,0x00);
		HM5065_write_cmos_sensor(0x080d,0x00);
		HM5065_write_cmos_sensor(0x080e,0x00); 

		HM5065_write_cmos_sensor(0x0751,0x00);	  
		HM5065_write_cmos_sensor(FACE_LC,0x01);//add abc enable//bit 0: AF  bit1: touch AE
*/
#if 1//def	AE_and_AF_FACE
		//HM5065_write_cmos_sensor(FACE_LC,0x03);//add abc disable//bit 0: AF  bit1: touch AE
#endif

	if((pvx0==pvx1)&&(pvy0==pvy1))
	{
		HM5065_write_cmos_sensor(0x0808,0x01);
		HM5065_write_cmos_sensor(0x0809,0x01); 
		HM5065_write_cmos_sensor(0x080a,0x01);
		HM5065_write_cmos_sensor(0x080b,0x01);	
		HM5065_write_cmos_sensor(0x080c,0x01);
		HM5065_write_cmos_sensor(0x080d,0x01);
		HM5065_write_cmos_sensor(0x080e,0x01); 
		
		HM5065_write_cmos_sensor(0x0751,0x00);	  
		HM5065_write_cmos_sensor(FACE_LC,0x00);//add abc enable//bit 0: AF	bit1: touch AE
/*
       	HM5065_write_cmos_sensor(FACE_START_XH, (pvx0-160)>>8);
		HM5065_write_cmos_sensor(FACE_START_XL, (pvx0-160)&0xff);

		HM5065_write_cmos_sensor(FACE_START_YH, (pvy0-160)>>8);
		HM5065_write_cmos_sensor(FACE_START_YL, (pvy0-160)&0xff);		

		HM5065SENSORDB("HM5065_FOCUS_Set_AF_Center Window_to_IC\n");

		HM5065_write_cmos_sensor(FACE_SIZE_XH,0x01);
		HM5065_write_cmos_sensor(FACE_SIZE_XL,0x40);
		HM5065_write_cmos_sensor(FACE_SIZE_YH,0x01);
		HM5065_write_cmos_sensor(FACE_SIZE_YL,0x40);
*/
		HM5065SENSORDB("HM5065_FOCUS_Set_ normal AF_Center Window_to_IC\n");
		
    }
	else{
		HM5065_write_cmos_sensor(0x0808,0x01);
		HM5065_write_cmos_sensor(0x0809,0x00); 
		HM5065_write_cmos_sensor(0x080a,0x00);
		HM5065_write_cmos_sensor(0x080b,0x00);  
		HM5065_write_cmos_sensor(0x080c,0x00);
		HM5065_write_cmos_sensor(0x080d,0x00);
		HM5065_write_cmos_sensor(0x080e,0x00); 

		HM5065_write_cmos_sensor(0x0751,0x00);	  
		HM5065_write_cmos_sensor(FACE_LC,0x01);

#if 1
		HM5065_write_cmos_sensor(FACE_START_XH, pvx0>>8);
		HM5065_write_cmos_sensor(FACE_START_XL, pvx0&0xff);

		HM5065_write_cmos_sensor(FACE_START_YH, pvy0>>8);
		HM5065_write_cmos_sensor(FACE_START_YL, pvy0&0xff);		

		HM5065SENSORDB("HM5065_FOCUS_Set_AF_Touch Window_to_IC\n");

		HM5065_write_cmos_sensor(FACE_SIZE_XH,(pvx1-pvx0)>>8);
		HM5065_write_cmos_sensor(FACE_SIZE_XL,(pvx1-pvx0)&0xff);
		HM5065_write_cmos_sensor(FACE_SIZE_YH,(pvy1-pvy0)>>8);
		HM5065_write_cmos_sensor(FACE_SIZE_YL,(pvy1-pvy0)&0xff);
#endif
#if 0//test fix window
		HM5065_write_cmos_sensor(FACE_START_XH, 0x00);
		HM5065_write_cmos_sensor(FACE_START_XL, 0x10);

		HM5065_write_cmos_sensor(FACE_START_YH, 0x00);
		HM5065_write_cmos_sensor(FACE_START_YL, 0x10);		

		HM5065SENSORDB("HM5065_FOCUS_Set_AF_Touch Window_to_IC\n");

		HM5065_write_cmos_sensor(FACE_SIZE_XH,0x00);
		HM5065_write_cmos_sensor(FACE_SIZE_XL,0xA0);
		HM5065_write_cmos_sensor(FACE_SIZE_YH,0x00);
		HM5065_write_cmos_sensor(FACE_SIZE_YL,0xA0);

#endif}
	}

	HM5065SENSORDB("[HM5065]exit HM5065_FOCUS_Set_AF_Window function:\n ");
}
static void HM5065_FOCUS_Get_AF_Macro(UINT32 *pFeatureReturnPara32)
{
    HM5065SENSORDB("HM5065_FOCUS_Get_AF_Macro\n");
	//lens rang: 0--0x3ff
	HM5065_write_cmos_sensor(0x0751,0x00);	
	HM5065_write_cmos_sensor(0x070A,0x00);//disable af
    HM5065_write_cmos_sensor(0x0700,0x00);//position h
    HM5065_write_cmos_sensor(0x0701,0x05);//position l
	HM5065_write_cmos_sensor(0x070C,0x00);//fix len to 5
	mDELAY(100);
    HM5065_write_cmos_sensor(0x070C,0x07);//fix len to 5

    *pFeatureReturnPara32 = 0;
}
static void HM5065_FOCUS_Get_AF_Inf(UINT32 * pFeatureReturnPara32)
{

    HM5065SENSORDB("HM5065_FOCUS_Get_AF_Inf\n");
    HM5065_write_cmos_sensor(0x0751,0x00);	
	HM5065_write_cmos_sensor(0x070A,0x00);
    HM5065_write_cmos_sensor(0x0700,0x03);
    HM5065_write_cmos_sensor(0x0701,0xFF);
	HM5065_write_cmos_sensor(0x070C,0x00);
	mDELAY(100);
    HM5065_write_cmos_sensor(0x070C,0x07);
	printk("HM5065_FOCUS_Get_AF_Inf: AF POS %02x%02x.\n", HM5065_read_cmos_sensor(0x0700), HM5065_read_cmos_sensor
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
	//state_af = HM5065_read_cmos_sensor(0x07ae);
	//state_3029 = HM5065_read_cmos_sensor(0x3029);
	mDELAY(1);

	do
	{
	
		state_af = HM5065_read_cmos_sensor(0x07ae);
		//mDELAY(1);
		if (state_af==1)
		{
			*pFeatureReturnPara32 = SENSOR_AF_FOCUSED;  

			
			af_pos_h = HM5065_read_cmos_sensor(0x06F0);
			af_pos_l = HM5065_read_cmos_sensor(0x06F1);
			HM5065SENSORDB("[HM5065]brant read current af pos: %02x%02x.\n", af_pos_h, af_pos_l);

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
	
    HM5065SENSORDB("HM5065_AF_Status =%x\n",state_af);
	
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
	HM5065_write_cmos_sensor(0x0751,0x01);	

/*
	HM5065_write_cmos_sensor(0x070A,0x00);  
  	HM5065_write_cmos_sensor(0x0700,0x03);
	HM5065_write_cmos_sensor(0x0701,0xFF);
	HM5065_write_cmos_sensor(0x070C,0x00);
	HM5065_write_cmos_sensor(0x070C,0x07); 
 */
//	HM5065_write_cmos_sensor(FACE_LC,0x00);//disable
//	mDELAY(150);    
	//mDELAY(100);
#else
	HM5065_write_cmos_sensor(0x070A,0x00);

	HM5065_write_cmos_sensor(0x0734, af_pos_h & 0xFF);
	HM5065_write_cmos_sensor(0x0735, af_pos_l & 0xFF);

	printk("[HM5065]get af_status brant read target af pos1: %02x %02x.\n", HM5065_read_cmos_sensor(0x0700), HM5065_read_cmos_sensor(0x0701));

	HM5065_write_cmos_sensor(0x070C,0x00);
	HM5065_write_cmos_sensor(0x070C,0x05);
//	mdelay(200);
#endif
    HM5065SENSORDB("HM5065_Cancel FOCUS\n");

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
		HM5065SENSORDB("[HM5065]enter HM5065WBcalibattion function:\n ");
		kal_uint8 temp = HM5065_read_cmos_sensor(0x350b); 
		
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
		//HM5065_write_cmos_sensor(0x3400,(color_r_gain_w & 0xff00)>>8);																																														
		//HM5065_write_cmos_sensor(0x3401,color_r_gain_w & 0xff); 			
		//HM5065_write_cmos_sensor(0x3404,(color_b_gain_w & 0xff00)>>8);																																														
		//HM5065_write_cmos_sensor(0x3405,color_b_gain_w & 0xff); 
		HM5065SENSORDB("[HM5065]exit HM5065WBcalibattion function:\n ");*/
}	
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
UINT32 HM5065Open(void)
{
	volatile signed int i;
	kal_uint16 sensor_id = 0;
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065Open function:\n ");
	//HM5065_write_cmos_sensor(0x3103,0x11);
	//HM5065_write_cmos_sensor(0x3008,0x82);
    mDELAY(10);
	for(i=0;i<3;i++)
	{
		sensor_id = (HM5065_read_cmos_sensor(0x0000) << 8) | HM5065_read_cmos_sensor(0x0001);
		HM5065SENSORDB("HM5065 READ ID :%x",sensor_id);
		if(sensor_id != HM5065_SENSOR_ID)
		{
			return ERROR_SENSOR_CONNECT_FAIL;
		}
	}
	HM5065initalvariable();
	HM5065InitialSetting();
	HM5065SENSORDB("[HM5065]exit HM5065Open function:\n ");
	return ERROR_NONE;
}	/* HM5065Open() */

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
UINT32 HM5065Close(void)
{
	HM5065SENSORDB("[HM5065]CONTROLFLOW HM5065Close\n ");
  //CISModulePowerOn(FALSE);
	return ERROR_NONE;
}	/* HM5065Close() */
/*************************************************************************
* FUNCTION
*	HM5065Preview
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
UINT32 HM5065Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065Preview function:\n ");
	kal_uint32 zsdshutter = 0;
#if 0
	switch(CurrentScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			   HM5065Sensor.zsd_flag=1;
			   HM5065PreviewSetting_SVGA();
			   HM5065FullSizeCaptureSetting();	
			   zsdshutter=HM5065Sensor.PreviewShutter*2;
			//   HM5065WriteExpShutter(zsdshutter);
			   break;
		default:
			   HM5065PreviewSetting_SVGA();
			   HM5065Sensor.zsd_flag=0;
			//   HM5065WriteExpShutter(HM5065Sensor.PreviewShutter);
			   break;
	}
	#endif

	if(sensor_config_data->SensorOperationMode==MSDK_SENSOR_OPERATION_MODE_VIDEO){
		HM5065Sensor.VideoMode= KAL_TRUE;  // MPEG4 Encode Mode
	}else{
		HM5065Sensor.VideoMode = KAL_FALSE;  
	}
	HM5065PreviewSetting_SVGA();

	sensor_config_data->SensorImageMirror=IMAGE_HV_MIRROR;

	HM5065SetHVMirror(sensor_config_data->SensorImageMirror);

	
	//HM5065_FOCUS_OVT_AFC_Init();
	//HM5065_set_AE_mode(KAL_TRUE);
	//HM5065_set_AWB_mode(KAL_TRUE);
	//mDELAY(30);	
	HM5065_night_mode(HM5065Sensor.NightMode);
	mDELAY(30);	
	//HM5065_FOCUS_OVT_AFC_Constant_Focus();
	HM5065SENSORDB("[HM5065]exit HM5065Preview function:\n ");	
	return ERROR_NONE ;
	
}	/* HM5065Preview() */
BOOL HM5065_set_param_exposure_for_HDR(UINT16 para)
{
    kal_uint32 totalGain = 0, exposureTime = 0;
	
	HM5065SENSORDB("[HM5065]HM5065_set_param_exposure_for_HDR para=%d:\n ", para);
    if (0 == HM5065Sensor.manualAEStart)
    {       
        HM5065_set_AE_mode(KAL_FALSE);//Manual AE enable
        spin_lock(&HM5065_drv_lock);	
        HM5065Sensor.manualAEStart = 1;
		spin_unlock(&HM5065_drv_lock);
    }
	totalGain = HM5065Sensor.currentAxDGain;
    exposureTime = HM5065Sensor.currentExposureTime;
	switch (para)
	{
	   case AE_EV_COMP_20:	//+2 EV
       case AE_EV_COMP_10:	// +1 EV
		   totalGain = totalGain<<1;
           exposureTime = exposureTime<<1;
           HM5065SENSORDB("[4EC] HDR AE_EV_COMP_20\n");
		 break;
	   case AE_EV_COMP_00:	// +0 EV
           HM5065SENSORDB("[4EC] HDR AE_EV_COMP_00\n");
		 break;
	   case AE_EV_COMP_n10:  // -1 EV
	   case AE_EV_COMP_n20:  // -2 EV
		   totalGain = totalGain >> 1;
           exposureTime = exposureTime >> 1;
           HM5065SENSORDB("[4EC] HDR AE_EV_COMP_n20\n");
		 break;
	   default:
		 break;//return FALSE;
	}

	if(totalGain > 0xf0)
		totalGain = 0xf0;
    HM5065WriteShutter(totalGain);
	HM5065WriteShutter(exposureTime);
	return TRUE;
}
#if 0
UINT32 HM5065Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	kal_uint32 shutter = 0;	
	kal_uint32 extshutter = 0;
	kal_uint32 color_r_gain = 0;
	kal_uint32 color_b_gain = 0;
	kal_uint32 readgain=0;
	UINT32 AF_status;
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065Capture function:\n ");

	//if(SENSOR_MODE_PREVIEW == HM5065Sensor.SensorMode )
	//{		
	//shutter=HM5065ReadShutter();
	//extshutter=HM5065ReadExtraShutter();
	//readgain=HM5065ReadSensorGain();
	//spin_lock(&HM5065_drv_lock);
	//HM5065Sensor.PreviewShutter=shutter;
	//HM5065Sensor.PreviewExtraShutter=extshutter;	
	//HM5065Sensor.SensorGain=readgain;
	//spin_unlock(&HM5065_drv_lock);	
	//HM5065_set_AE_mode(KAL_FALSE);
	//HM5065_set_AWB_mode(KAL_FALSE);
	//color_r_gain=((HM5065_read_cmos_sensor(0x3401)&0xFF)+((HM5065_read_cmos_sensor(0x3400)&0xFF)*256));  
	//color_b_gain=((HM5065_read_cmos_sensor(0x3405)&0xFF)+((HM5065_read_cmos_sensor(0x3404)&0xFF)*256)); 

	spin_lock(&HM5065_drv_lock);	
	HM5065Sensor.SensorMode= SENSOR_MODE_CAPTURE;
	spin_unlock(&HM5065_drv_lock);

	HM5065FullSizeCaptureSetting();
    //HM5065WBcalibattion(color_r_gain,color_b_gain);      
    //HM5065WBcalibattion(color_r_gain,color_b_gain);      
	HM5065SENSORDB("[HM5065]Before shutter=%d:\n",shutter);
	/*if(HM5065Sensor.zsd_flag==0)
	{
		shutter = shutter*2;
	}*/
	if (SCENE_MODE_HDR == HM5065Sensor.sceneMode)
    {
		HM5065SENSORDB("[HM5065] HDR capture, record shutter gain\n");
        spin_lock(&HM5065_drv_lock);
        HM5065Sensor.currentExposureTime=HM5065ReadShutter();
		//HM5065Sensor.currentextshutter=extshutter;
		HM5065Sensor.currentAxDGain=HM5065ReadSensorGain();
		spin_unlock(&HM5065_drv_lock);
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
	HM5065SENSORDB("[HM5065]exit HM5065Capture function:\n ");
	return ERROR_NONE; 
}/* HM5065Capture() */
	#else
UINT32 HM5065Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	kal_uint32 shutter = 0;	
	kal_uint32 extshutter = 0;
	kal_uint32 color_r_gain = 0;
	kal_uint32 color_b_gain = 0;
	kal_uint32 readgain=0;
	UINT32 AF_status;
	UINT8 i=20;
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065Capture function:\n ");

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
	//if(SENSOR_MODE_PREVIEW == HM5065Sensor.SensorMode )
	//{		
	//shutter=HM5065ReadShutter();
	//extshutter=HM5065ReadExtraShutter();
	//readgain=HM5065ReadSensorGain();
	//spin_lock(&HM5065_drv_lock);
	//HM5065Sensor.PreviewShutter=shutter;
	//HM5065Sensor.PreviewExtraShutter=extshutter;	
	//HM5065Sensor.SensorGain=readgain;
	//spin_unlock(&HM5065_drv_lock);	
	//HM5065_set_AE_mode(KAL_FALSE);
	//HM5065_set_AWB_mode(KAL_FALSE);
	//color_r_gain=((HM5065_read_cmos_sensor(0x3401)&0xFF)+((HM5065_read_cmos_sensor(0x3400)&0xFF)*256));  
	//color_b_gain=((HM5065_read_cmos_sensor(0x3405)&0xFF)+((HM5065_read_cmos_sensor(0x3404)&0xFF)*256)); 

	spin_lock(&HM5065_drv_lock);	
	HM5065Sensor.SensorMode= SENSOR_MODE_CAPTURE;
	spin_unlock(&HM5065_drv_lock);

//
//
#if 1
//	HM5065_FOCUS_OVT_AFC_Single_Focus();
	
		do
		{			
			if(HM5065_read_cmos_sensor(0x07ae)==1)
			{	
				break;
			};
			mDELAY(10);	
		}
		while(i<20);
		af_pos_h = HM5065_read_cmos_sensor(0x06F0);
		af_pos_l = HM5065_read_cmos_sensor(0x06F1);	
//	
#endif
//HM5065_FOCUS_OVT_AFC_Cancel_Focus();
//213
///
	HM5065FullSizeCaptureSetting();
	mDELAY(100);
//

	#if 1
	HM5065_write_cmos_sensor(0x070A, 0x00);
	HM5065_write_cmos_sensor(0x0734, af_pos_h & 0xFF);
	HM5065_write_cmos_sensor(0x0735, af_pos_l & 0xFF);
	HM5065_write_cmos_sensor(0x070C, 0x00);
	mDELAY(200);
	HM5065_write_cmos_sensor(0x070C, 0x05);
	mDELAY(200);
	#endif
    //HM5065WBcalibattion(color_r_gain,color_b_gain);      
    //HM5065WBcalibattion(color_r_gain,color_b_gain);      
	HM5065SENSORDB("[HM5065]Before shutter=%d:\n",shutter);
	/*if(HM5065Sensor.zsd_flag==0)
	{
		shutter = shutter*2;
	}*/
	if (SCENE_MODE_HDR == HM5065Sensor.sceneMode)
    {
		HM5065SENSORDB("[HM5065] HDR capture, record shutter gain\n");
        spin_lock(&HM5065_drv_lock);
        HM5065Sensor.currentExposureTime=HM5065ReadShutter();
		//HM5065Sensor.currentextshutter=extshutter;
		HM5065Sensor.currentAxDGain=HM5065ReadSensorGain();
		spin_unlock(&HM5065_drv_lock);
    }

	//mDELAY(100);
	//}
	HM5065SENSORDB("[HM5065]exit HM5065Capture function:\n ");
	return ERROR_NONE; 
}/* HM5065Capture() */
#endif
UINT32 HM5065ZSDPreview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	kal_uint32 shutter = 0;	
	kal_uint32 extshutter = 0;
	kal_uint32 color_r_gain = 0;
	kal_uint32 color_b_gain = 0;
	kal_uint32 readgain=0;
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065Capture function:\n ");

	//if(SENSOR_MODE_PREVIEW == HM5065Sensor.SensorMode )
	//{		
	/*shutter=HM5065ReadShutter();
	extshutter=HM5065ReadExtraShutter();
	readgain=HM5065ReadSensorGain();
	spin_lock(&HM5065_drv_lock);
	HM5065Sensor.PreviewShutter=shutter;
	HM5065Sensor.PreviewExtraShutter=extshutter;	
	HM5065Sensor.SensorGain=readgain;
	spin_unlock(&HM5065_drv_lock);	*/
	//HM5065_set_AE_mode(KAL_FALSE);
	//HM5065_set_AWB_mode(KAL_FALSE);
	//color_r_gain=((HM5065_read_cmos_sensor(0x3401)&0xFF)+((HM5065_read_cmos_sensor(0x3400)&0xFF)*256));  
	//color_b_gain=((HM5065_read_cmos_sensor(0x3405)&0xFF)+((HM5065_read_cmos_sensor(0x3404)&0xFF)*256)); 
	HM5065FullSizeCaptureSetting_ZSD();
	spin_lock(&HM5065_drv_lock);	
	HM5065Sensor.SensorMode= SENSOR_MODE_CAPTURE;
	spin_unlock(&HM5065_drv_lock);
    //HM5065WBcalibattion(color_r_gain,color_b_gain);      
	HM5065SENSORDB("[HM5065]Before shutter=%d:\n",shutter);
	/*if(HM5065Sensor.zsd_flag==0)
	{
		shutter = shutter*2;
	}*/
	if (SCENE_MODE_HDR == HM5065Sensor.sceneMode)
    {

        spin_lock(&HM5065_drv_lock);
        HM5065Sensor.currentExposureTime=HM5065ReadShutter();
		//HM5065Sensor.currentextshutter=extshutter;
		HM5065Sensor.currentAxDGain=HM5065ReadSensorGain();
		spin_unlock(&HM5065_drv_lock);
    }

	mDELAY(100);
	//}
	HM5065SENSORDB("[HM5065]exit HM5065Capture function:\n ");
	return ERROR_NONE; 
}/* HM5065Capture() */


UINT32 HM5065GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
	HM5065SENSORDB("[HM5065]enter HM5065GetResolution function:\n ");
	pSensorResolution->SensorPreviewWidth= HM5065_IMAGE_SENSOR_SVGA_WIDTH;
	pSensorResolution->SensorPreviewHeight= HM5065_IMAGE_SENSOR_SVGA_HEIGHT;
	pSensorResolution->SensorFullWidth= HM5065_IMAGE_SENSOR_QSXGA_WITDH; 
	pSensorResolution->SensorFullHeight= HM5065_IMAGE_SENSOR_QSXGA_HEIGHT;
	pSensorResolution->SensorVideoWidth= HM5065_IMAGE_SENSOR_SVGA_WIDTH; 
	pSensorResolution->SensorVideoHeight= HM5065_IMAGE_SENSOR_SVGA_HEIGHT;
	HM5065SENSORDB("[HM5065]exit HM5065GetResolution function:\n ");
	return ERROR_NONE;
}	/* HM5065GetResolution() */

UINT32 HM5065GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,MSDK_SENSOR_INFO_STRUCT *pSensorInfo,MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	HM5065SENSORDB("[HM5065]enter HM5065GetInfo function:\n ");
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			pSensorInfo->SensorPreviewResolutionX=HM5065_IMAGE_SENSOR_QSXGA_WITDH;//HM5065_IMAGE_SENSOR_SVGA_WIDTH;//HM5065_IMAGE_SENSOR_QSXGA_WITDH ;
			pSensorInfo->SensorPreviewResolutionY=HM5065_IMAGE_SENSOR_QSXGA_HEIGHT;//HM5065_IMAGE_SENSOR_SVGA_HEIGHT;//HM5065_IMAGE_SENSOR_QSXGA_HEIGHT ;
			//pSensorInfo->SensorPreviewResolutionX=HM5065_IMAGE_SENSOR_SVGA_WIDTH;//HM5065_IMAGE_SENSOR_SVGA_WIDTH;//HM5065_IMAGE_SENSOR_QSXGA_WITDH ;
			//pSensorInfo->SensorPreviewResolutionY=HM5065_IMAGE_SENSOR_SVGA_HEIGHT;//HM5065_IMAGE_SENSOR_SVGA_HEIGHT;//HM5065_IMAGE_SENSOR_QSXGA_HEIGHT ;
			pSensorInfo->SensorCameraPreviewFrameRate=10;
			break;
		default:
			pSensorInfo->SensorPreviewResolutionX=HM5065_IMAGE_SENSOR_SVGA_WIDTH ;
			pSensorInfo->SensorPreviewResolutionY=HM5065_IMAGE_SENSOR_SVGA_HEIGHT ;
			pSensorInfo->SensorCameraPreviewFrameRate=30;
			break;
	}		 		
	pSensorInfo->SensorFullResolutionX= HM5065_IMAGE_SENSOR_QSXGA_WITDH;
	pSensorInfo->SensorFullResolutionY= HM5065_IMAGE_SENSOR_QSXGA_HEIGHT;
	//pSensorInfo->SensorCameraPreviewFrameRate=30;
	pSensorInfo->SensorVideoFrameRate=30;
	pSensorInfo->SensorStillCaptureFrameRate=5;
	pSensorInfo->SensorWebCamCaptureFrameRate=15;
	pSensorInfo->SensorResetActiveHigh=FALSE;
	pSensorInfo->SensorResetDelayCount=4;
	pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_YUYV;
	pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;	
	pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;  
	pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_HIGH;
	pSensorInfo->SensorInterruptDelayLines = 2;
	pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_PARALLEL;
	pSensorInfo->CaptureDelayFrame = 2;  //4
	pSensorInfo->PreviewDelayFrame = 1; //6
	pSensorInfo->VideoDelayFrame = 3; 		
	pSensorInfo->SensorMasterClockSwitch = 0; 
	pSensorInfo->YUVAwbDelayFrame = 2;
	pSensorInfo->YUVEffectDelayFrame= 3; 
	pSensorInfo->AEShutDelayFrame= 0;
 	pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;   		
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount=	3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
			pSensorInfo->SensorGrabStartX = HM5065_PV_GRAB_START_X; 
			pSensorInfo->SensorGrabStartY = HM5065_PV_GRAB_START_Y;   
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
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount= 3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
			pSensorInfo->SensorGrabStartX = HM5065_FULL_GRAB_START_X; 
			pSensorInfo->SensorGrabStartY = HM5065_FULL_GRAB_START_Y;             
			pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_2_LANE;			
			pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0; 
			pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount =4; 
			pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0; 
			pSensorInfo->SensorWidthSampling = 0; 
			pSensorInfo->SensorHightSampling = 0;
			pSensorInfo->SensorPacketECCOrder = 1;
			break;
		default:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount= 3;
			pSensorInfo->SensorClockRisingCount=0;
			pSensorInfo->SensorClockFallingCount=2;
			pSensorInfo->SensorPixelClockCount=3;
			pSensorInfo->SensorDataLatchCount=2;
			pSensorInfo->SensorGrabStartX = HM5065_PV_GRAB_START_X; 
			pSensorInfo->SensorGrabStartY = HM5065_PV_GRAB_START_Y; 			
			pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_2_LANE;			
			pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0; 
			pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount = 4; 
			pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
			pSensorInfo->SensorWidthSampling = 0;
			pSensorInfo->SensorHightSampling = 0;	
			pSensorInfo->SensorPacketECCOrder = 1;
		  break;
	}
	memcpy(pSensorConfigData, &HM5065SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));	
	HM5065SENSORDB("[HM5065]exit HM5065GetInfo function:\n ");	
	return ERROR_NONE;
}	/* HM5065GetInfo() */

UINT32 HM5065Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	  HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065Control function:%d \n ",ScenarioId);
	  spin_lock(&HM5065_drv_lock);
	  CurrentScenarioId = ScenarioId;
	  spin_unlock(&HM5065_drv_lock);
	  switch (ScenarioId)
	  {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			
			 HM5065Preview(pImageWindow, pSensorConfigData);
			 break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		//case MSDK_SCENARIO_ID_CAMERA_ZSD:
			 HM5065Capture(pImageWindow, pSensorConfigData);
	  	     break;
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			 HM5065ZSDPreview(pImageWindow, pSensorConfigData);
			break;
		default:
			return ERROR_INVALID_SCENARIO_ID;
	}
	HM5065SENSORDB("[HM5065]exit HM5065Control function:\n ");
	return ERROR_NONE;
}	/* HM5065Control() */

/* [TC] YUV sensor */	

BOOL HM5065_set_param_wb(UINT16 para)
{
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_set_param_wb function:\n ");

	
	spin_lock(&HM5065_drv_lock);
	HM5065Sensor.wb = para;
	spin_unlock(&HM5065_drv_lock);

    switch (para)
    {
      //  case AWB_MODE_OFF:
			//HM5065_write_cmos_sensor(0x01A4,0x04);
        //    break;

        case AWB_MODE_AUTO:
			HM5065_write_cmos_sensor(0x01A0,0x01);//awb enable
            break;

        case AWB_MODE_CLOUDY_DAYLIGHT: //cloudy(13000K)
			HM5065_write_cmos_sensor(0x01A0,0x03);//MWB awb disable
			HM5065_write_cmos_sensor(0x01A1,0xef);//Rgain ff
			HM5065_write_cmos_sensor(0x01A2,0x60);//Ggain 40
			HM5065_write_cmos_sensor(0x01A3,0x00);//Bgain
            break;

        case AWB_MODE_DAYLIGHT: //sunny(7000K)
			HM5065_write_cmos_sensor(0x01A0,0x03);//MWB
			HM5065_write_cmos_sensor(0x01A1,0xC0);//Rgain
			HM5065_write_cmos_sensor(0x01A2,0x40);//Ggain
			HM5065_write_cmos_sensor(0x01A3,0x00);//Bgain
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
            break;
           // return FALSE;
    }
    return TRUE;


	HM5065SENSORDB("[HM5065]exit HM5065_set_param_wb function:\n ");
       return TRUE;
} /* HM5065_set_param_wb */
void HM5065_set_contrast(UINT16 para)
{   
    HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_set_contrast function:\n ");
    switch (para)
    {
        case ISP_CONTRAST_LOW:
             //HM5065_write_cmos_sensor(0x0080,0x5c);//mark by jink 20131104
             break; 
        case ISP_CONTRAST_HIGH:
             //HM5065_write_cmos_sensor(0x0080,0x7c);
             break; 
        case ISP_CONTRAST_MIDDLE:
             //HM5065_write_cmos_sensor(0x0080,0x64);
        default:
             break; 

    }
    HM5065SENSORDB("[HM5065]exit HM5065_set_contrast function:\n ");
    return;
}

void HM5065_set_brightness(UINT16 para)
{
    HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_set_brightness function:\n ");

		switch (para)
		{
			case ISP_BRIGHT_LOW:
				  //HM5065_write_cmos_sensor(0x0130,0xfb);//mark by jink 20131104
				 break; 
			case ISP_BRIGHT_HIGH:  
				 // HM5065_write_cmos_sensor(0x0130,0xfe);
				 break; 
			case ISP_BRIGHT_MIDDLE:
				 // HM5065_write_cmos_sensor(0x0130,0xfd);//00->fe
			default:	
				 break; 
		}


    HM5065SENSORDB("[HM5065]exit HM5065_set_brightness function:\n ");
    return;
}
void HM5065_set_saturation(UINT16 para)
{
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_set_saturation function:\n ");
    switch (para)
    {
        case ISP_SAT_HIGH:
             //HM5065_write_cmos_sensor(0x0081 ,0x78);//mark by jink 20131104

             break; 
        case ISP_SAT_LOW:
            // HM5065_write_cmos_sensor(0x0081 ,0x38);
             break; 
        case ISP_SAT_MIDDLE:
        default:
            // HM5065_write_cmos_sensor(0x0081 ,0x60); //54->60
             break; 
    }	

	HM5065SENSORDB("[HM5065]exit HM5065_set_saturation function:\n ");
     return;
}
void HM5065_set_scene_mode(UINT16 para)
{
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_set_scene_mode function:%d\n ",para);
	spin_lock(&HM5065_drv_lock);
	HM5065Sensor.sceneMode=para;
	spin_unlock(&HM5065_drv_lock);
	
	HM5065_set_param_wb(HM5065Sensor.wb);

    switch (para)
    { 

		case SCENE_MODE_NIGHTSCENE:
				
          	HM5065_night_mode(KAL_TRUE); 
			break;
        case SCENE_MODE_PORTRAIT:
			HM5065_night_mode(KAL_FALSE);
			//HM5065_write_cmos_sensor(0x004C, 0x08);
			//HM5065_write_cmos_sensor(0x006C, 0x08);
			//HM5065_write_cmos_sensor(0x0080, 0x6C);
			//HM5065_write_cmos_sensor(0x0081, 0x58);
			HM5065_write_cmos_sensor(0x00E8, 0x01);
			HM5065_write_cmos_sensor(0x00ED, 0x05);
			HM5065_write_cmos_sensor(0x0128, 0x00);
			//HM5065_write_cmos_sensor(0x0130, 0x00);
			//HM5065_write_cmos_sensor(0x0143, 0x5F);
			//HM5065_write_cmos_sensor(0x0144, 0x0D);
			//HM5065_write_cmos_sensor(0x015E, 0x40);
			//HM5065_write_cmos_sensor(0x015F, 0x00);
			HM5065_write_cmos_sensor(0x01A0, 0x01);
			//HM5065_write_cmos_sensor(0x02C2, 0x00);
			//HM5065_write_cmos_sensor(0x02C3, 0xC0);
			
			 		 
            break;
        case SCENE_MODE_LANDSCAPE:
		
			HM5065_night_mode(KAL_FALSE);
			//HM5065_write_cmos_sensor(0x004C, 0x10);
			//HM5065_write_cmos_sensor(0x006C, 0x10);
			//HM5065_write_cmos_sensor(0x0080, 0x78);
			//HM5065_write_cmos_sensor(0x0081, 0x68);
			HM5065_write_cmos_sensor(0x00E8, 0x01);
			HM5065_write_cmos_sensor(0x00ED, 0x05);
			HM5065_write_cmos_sensor(0x0128, 0x00);
			//HM5065_write_cmos_sensor(0x0130, 0x00);
			//HM5065_write_cmos_sensor(0x0143, 0x5F);
			//HM5065_write_cmos_sensor(0x0144, 0x0D);
			//HM5065_write_cmos_sensor(0x015E, 0x40);
			//HM5065_write_cmos_sensor(0x015F, 0x00);
			HM5065_write_cmos_sensor(0x01A0, 0x01);
			//HM5065_write_cmos_sensor(0x02C2, 0x00);
			//HM5065_write_cmos_sensor(0x02C3, 0xC0);			 
             break;
        case SCENE_MODE_SUNSET:
			HM5065_night_mode(KAL_FALSE);
			//HM5065_write_cmos_sensor(0x004C, 0x05);
			//HM5065_write_cmos_sensor(0x006C, 0x05);
			//HM5065_write_cmos_sensor(0x0080, 0x6C);
			//HM5065_write_cmos_sensor(0x0081, 0x58);
			HM5065_write_cmos_sensor(0x00E8, 0x01);
			HM5065_write_cmos_sensor(0x00ED, 0x05);
			HM5065_write_cmos_sensor(0x0128, 0x00);
			//HM5065_write_cmos_sensor(0x0130, 0xFC);
			//HM5065_write_cmos_sensor(0x0143, 0x5F);
			//HM5065_write_cmos_sensor(0x0144, 0x0D);
			//HM5065_write_cmos_sensor(0x015E, 0x40);
			//HM5065_write_cmos_sensor(0x015F, 0x00); 		  
			HM5065_write_cmos_sensor(0x01A0, 0x03);//;WB_Cloudy	 
			HM5065_write_cmos_sensor(0x01A1, 0xE0);//;	
			HM5065_write_cmos_sensor(0x01A2, 0x40);//;	
			HM5065_write_cmos_sensor(0x01A3, 0x00);//;	
			//HM5065_write_cmos_sensor(0x02C2, 0x00);
			//HM5065_write_cmos_sensor(0x02C3, 0xC0);					 
            break;
        case SCENE_MODE_SPORTS:
  
			HM5065_night_mode(KAL_FALSE);

			//HM5065_write_cmos_sensor(0x004C, 0x08);
			//HM5065_write_cmos_sensor(0x006C, 0x08);
			//HM5065_write_cmos_sensor(0x0080, 0x6C);
			//HM5065_write_cmos_sensor(0x0081, 0x58);
			HM5065_write_cmos_sensor(0x00E8, 0x01);
			HM5065_write_cmos_sensor(0x00ED, 0x0f);//0x1e
			HM5065_write_cmos_sensor(0x0128, 0x00);
			//HM5065_write_cmos_sensor(0x0130, 0x00);
			//HM5065_write_cmos_sensor(0x0143, 0x5C);
			//HM5065_write_cmos_sensor(0x0144, 0x09);
			//HM5065_write_cmos_sensor(0x015E, 0x40);
			//HM5065_write_cmos_sensor(0x015F, 0x00);
			HM5065_write_cmos_sensor(0x01A0, 0x01);
			//HM5065_write_cmos_sensor(0x02C2, 0x00);
			//HM5065_write_cmos_sensor(0x02C3, 0xC0);           		 
            break;
      case SCENE_MODE_HDR:
	  	
			HM5065SENSORDB("[HM5065]CONTROLFLOW SCENE_MODE_HDR\n ");
        /*      if (1 == HM5065Sensor.manualAEStart)
            {
                HM5065_set_AE_mode(KAL_TRUE);//Manual AE disable
                spin_lock(&HM5065_drv_lock);
            	HM5065Sensor.manualAEStart = 0;
                HM5065Sensor.currentExposureTime = 0;
                HM5065Sensor.currentAxDGain = 0;
				spin_unlock(&HM5065_drv_lock);
            }
            */
            break;
        case SCENE_MODE_OFF:
        default:
	
          	HM5065_night_mode(KAL_FALSE); 		 	   
			break;
        //default:
		//	return KAL_FALSE;
         //   break;
    }

/*	if ((
		(SCENE_MODE_OFF == HM5065Sensor.sceneMode) 
		|| (SCENE_MODE_NORMAL == HM5065Sensor.sceneMode)
		|| (SCENE_MODE_NIGHTSCENE == HM5065Sensor.sceneMode)
		//|| (SCENE_MODE_AUTO == HM5065Sensor.sceneMode)

		
		))
		HM5065_set_param_wb(HM5065Sensor.wb);*/


	HM5065SENSORDB("[HM5065]exit HM5065_set_scene_mode function:\n ");
	return;
}
void HM5065_set_iso(UINT16 para)
{
	HM5065SENSORDB("[HM5065]CONTROLFLOW HM5065_set_iso:%d\n ",para);

    spin_lock(&HM5065_drv_lock);
    HM5065Sensor.isoSpeed = para;
    spin_unlock(&HM5065_drv_lock);   
	 switch (para)
		{
			case AE_ISO_100:
				 //ISO 100
	HM5065_write_cmos_sensor(0x015C, 0x3e);
	HM5065_write_cmos_sensor(0x015D, 0x00);
	HM5065_write_cmos_sensor(0x015E, 0x3e);
	HM5065_write_cmos_sensor(0x015F, 0x00);
	HM5065_write_cmos_sensor(0x02C0, 0x00);
	HM5065_write_cmos_sensor(0x02C1, 0x00);
	HM5065_write_cmos_sensor(0x02C2, 0x00);
	HM5065_write_cmos_sensor(0x02C3, 0x00);
				 break; 
			case AE_ISO_200:
				 //ISO 200
	HM5065_write_cmos_sensor(0x015C, 0x3e);
	HM5065_write_cmos_sensor(0x015D, 0x00);
	HM5065_write_cmos_sensor(0x015E, 0x3e);
	HM5065_write_cmos_sensor(0x015F, 0x00);
	HM5065_write_cmos_sensor(0x02C0, 0x00);
	HM5065_write_cmos_sensor(0x02C1, 0x80);
	HM5065_write_cmos_sensor(0x02C2, 0x00);
	HM5065_write_cmos_sensor(0x02C3, 0x80);
				 break; 
			case AE_ISO_400:
				 //ISO 400
	HM5065_write_cmos_sensor(0x015C, 0x40);
	HM5065_write_cmos_sensor(0x015D, 0x00);
	HM5065_write_cmos_sensor(0x015E, 0x40);
	HM5065_write_cmos_sensor(0x015F, 0x00);
	HM5065_write_cmos_sensor(0x02C0, 0x00);
	HM5065_write_cmos_sensor(0x02C1, 0x80);
	HM5065_write_cmos_sensor(0x02C2, 0x00);
	HM5065_write_cmos_sensor(0x02C3, 0x80);
				 break; 
			default:
			case AE_ISO_AUTO:
				 //ISO Auto
	HM5065_write_cmos_sensor(0x015C, 0x40);
	HM5065_write_cmos_sensor(0x015D, 0x00);
	HM5065_write_cmos_sensor(0x015E, 0x40);
	HM5065_write_cmos_sensor(0x015F, 0x00);
	HM5065_write_cmos_sensor(0x02C0, 0x00);
	HM5065_write_cmos_sensor(0x02C1, 0x00);
	HM5065_write_cmos_sensor(0x02C2, 0x00);
	HM5065_write_cmos_sensor(0x02C3, 0xa0);
				 break; 
		}	

    return;
}

BOOL HM5065_set_param_effect(UINT16 para)
{
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_set_param_effect function:\n ");
	 

	    switch (para)
    {
        case MEFFECT_OFF:
			HM5065_write_cmos_sensor(0x0380,0x00);
			HM5065_write_cmos_sensor(0x0381,0x00);
			HM5065_write_cmos_sensor(0x0382,0x00);
			HM5065_write_cmos_sensor(0x0384,0x00);
#if 0
			HM5065_write_cmos_sensor(0x01A0,0x01);
			HM5065_write_cmos_sensor(0x01A1,0x80);
			HM5065_write_cmos_sensor(0x01A2,0x80);
			HM5065_write_cmos_sensor(0x01A3,0x80);
			HM5065_write_cmos_sensor(0x01A5,0x3e);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x3e);
			HM5065_write_cmos_sensor(0x01A8,0x00);	
			HM5065_write_cmos_sensor(0x0049,0x14);	
#endif
		//HM5065_set_scene_mode(HM5065Sensor.sceneMode);

		break;

        case MEFFECT_SEPIA:
			HM5065_write_cmos_sensor(0x0380,0x00);
			HM5065_write_cmos_sensor(0x0381,0x00);
			HM5065_write_cmos_sensor(0x0382,0x00);
			HM5065_write_cmos_sensor(0x0384,0x06);
#if 0
			HM5065_write_cmos_sensor(0x01A0,0x03);
			HM5065_write_cmos_sensor(0x01A1,0x80);
			HM5065_write_cmos_sensor(0x01A2,0x80);
			HM5065_write_cmos_sensor(0x01A3,0x80);
			HM5065_write_cmos_sensor(0x01A5,0x3e);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x3e);
			HM5065_write_cmos_sensor(0x01A8,0x00);
			HM5065_write_cmos_sensor(0x0049,0x14);
#endif			
            break;

        case MEFFECT_NEGATIVE:
			HM5065_write_cmos_sensor(0x0380,0x01);
			HM5065_write_cmos_sensor(0x0381,0x00);
			HM5065_write_cmos_sensor(0x0382,0x00);
			HM5065_write_cmos_sensor(0x0384,0x00);
#if 0
			HM5065_write_cmos_sensor(0x01A0,0x01);
			HM5065_write_cmos_sensor(0x01A1,0x80);
			HM5065_write_cmos_sensor(0x01A2,0x80);
			HM5065_write_cmos_sensor(0x01A3,0x80);
			HM5065_write_cmos_sensor(0x01A5,0x3e);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x3e);
			HM5065_write_cmos_sensor(0x01A8,0x00);	
			HM5065_write_cmos_sensor(0x0049,0x08);	
#endif
            break;

        case MEFFECT_SEPIAGREEN:
			HM5065_write_cmos_sensor(0x0380,0x00);
			HM5065_write_cmos_sensor(0x0381,0x00);
			HM5065_write_cmos_sensor(0x0382,0x00);
			HM5065_write_cmos_sensor(0x0384,0x03);
#if 0
			HM5065_write_cmos_sensor(0x01A0,0x03);
			HM5065_write_cmos_sensor(0x01A1,0x00);
			HM5065_write_cmos_sensor(0x01A2,0xCF);
			HM5065_write_cmos_sensor(0x01A3,0x00);
			HM5065_write_cmos_sensor(0x01A5,0x00);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x45);
			HM5065_write_cmos_sensor(0x01A8,0x00);	
			HM5065_write_cmos_sensor(0x0049,0x14);	
#endif
            break;

        case MEFFECT_SEPIABLUE:

			HM5065_write_cmos_sensor(0x0380,0x00);
			HM5065_write_cmos_sensor(0x0381,0x00);
			HM5065_write_cmos_sensor(0x0382,0x00);
			HM5065_write_cmos_sensor(0x0384,0x08);
#if 0
			HM5065_write_cmos_sensor(0x01A0,0x00);
			HM5065_write_cmos_sensor(0x01A1,0x00);
			HM5065_write_cmos_sensor(0x01A2,0x00);
			HM5065_write_cmos_sensor(0x01A3,0x3F);
			HM5065_write_cmos_sensor(0x01A5,0x1E);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x3e);
			HM5065_write_cmos_sensor(0x01A8,0x00);
			HM5065_write_cmos_sensor(0x0049,0x14);	
#endif		
            break;
	case MEFFECT_MONO: //B&W
			HM5065_write_cmos_sensor(0x0380,0x00);
			HM5065_write_cmos_sensor(0x0381,0x00);
			HM5065_write_cmos_sensor(0x0382,0x00);
			HM5065_write_cmos_sensor(0x0384,0x05);
#if 0
			HM5065_write_cmos_sensor(0x01A0,0x03);
			HM5065_write_cmos_sensor(0x01A1,0x80);
			HM5065_write_cmos_sensor(0x01A2,0x80);
			HM5065_write_cmos_sensor(0x01A3,0x80);
			HM5065_write_cmos_sensor(0x01A5,0x3e);
			HM5065_write_cmos_sensor(0x01A6,0x00);
			HM5065_write_cmos_sensor(0x01A7,0x3e);
			HM5065_write_cmos_sensor(0x01A8,0x00);
			HM5065_write_cmos_sensor(0x0049,0x14);	
#endif
		break;

        default:
             return KAL_FALSE;
    }
	HM5065SENSORDB("[HM5065]exit HM5065_set_param_effect function:\n ");
    return KAL_TRUE;
} /* HM5065_set_param_effect */

BOOL HM5065_set_param_banding(UINT16 para)
{
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_set_param_banding function:\n ");
	switch (para)
    {
        case AE_FLICKER_MODE_50HZ:
        case AE_FLICKER_MODE_AUTO:
            default:
						spin_lock(&HM5065_drv_lock);
						HM5065_Banding_setting = AE_FLICKER_MODE_50HZ;
						spin_unlock(&HM5065_drv_lock);
						//HM5065_write_cmos_sensor(0x3c00,0x04);
						//HM5065_write_cmos_sensor(0x3c01,0x80);
		  	HM5065_write_cmos_sensor(0x0190,0x00);
			HM5065_write_cmos_sensor(0x019C,0x4B);
			HM5065_write_cmos_sensor(0x019D,0x20);
            break;
        case AE_FLICKER_MODE_60HZ:			
						spin_lock(&HM5065_drv_lock);
						HM5065_Banding_setting = AE_FLICKER_MODE_60HZ;
						spin_unlock(&HM5065_drv_lock);
		  	HM5065_write_cmos_sensor(0x0190,0x00);
			HM5065_write_cmos_sensor(0x019C,0x4B);
			HM5065_write_cmos_sensor(0x019D,0xc0);
						//HM5065_write_cmos_sensor(0x3c00,0x00);
						//HM5065_write_cmos_sensor(0x3c01,0x80);
            break;
                 return FALSE;
    }
	HM5065SENSORDB("[HM5065]exit HM5065_set_param_banding function:\n ");
        return TRUE;
} /* HM5065_set_param_banding */

BOOL HM5065_set_param_exposure(UINT16 para)
{
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065_set_param_exposure function:\n ");
	HM5065SENSORDB("[HM5065]para=%d:\n",para);
	//spin_lock(&HM5065_drv_lock);
   if (SCENE_MODE_HDR == HM5065Sensor.sceneMode && 
    SENSOR_MODE_CAPTURE == HM5065Sensor.SensorMode)
   {
   	   //spin_unlock(&HM5065_drv_lock);
    //   HM5065_set_param_exposure_for_HDR(para);
    //   return TRUE;
    
	HM5065SENSORDB("[HM5065]CONTROLFLOW HDR enter HM5065_set_param_exposure function:\n ");
   }
   //spin_unlock(&HM5065_drv_lock);
	switch (para)
    {	
       case AE_EV_COMP_20:	                   
				
				HM5065_write_cmos_sensor(0x0082,0x9A);
				HM5065SENSORDB("[HM5065]EV +2 \n ");

				break;
		case AE_EV_COMP_10:	                   
		
				HM5065_write_cmos_sensor(0x0082,0x8A);

			  break;
		case AE_EV_COMP_00:

           	HM5065_write_cmos_sensor(0x0082,0x5A);
				HM5065SENSORDB("[HM5065]EV 0 \n ");
				//HM5065_write_cmos_sensor(0x3a1f, 0x18);//	; control zone L  
			  break;
   		 case AE_EV_COMP_n10:
			 
			 HM5065_write_cmos_sensor(0x0082,0x2A);
			  break;
      	case AE_EV_COMP_n20:  // -2 EV

           	HM5065_write_cmos_sensor(0x0082,0x10);
				HM5065SENSORDB("[HM5065]EV -2 \n ");
        	 break;
		default:
           	HM5065_write_cmos_sensor(0x0082,0x5A);
			break;//
						// return FALSE;
    }
	HM5065SENSORDB("[HM5065]exit HM5065_set_param_exposure function:\n ");
    return TRUE;
} /* HM5065_set_param_exposure */
#if 0//afc
BOOL HM5065_set_param_afmode(UINT16 para)
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
} /* HM5065_set_param_banding */
#endif
UINT32 HM5065YUVSensorSetting(FEATURE_ID iCmd, UINT32 iPara)
{
	HM5065SENSORDB("HM5065YUVSensorSetting:iCmd=%d,iPara=%d, %d \n",iCmd, iPara);
	HM5065SENSORDB("[HM5065]enter HM5065YUVSensorSetting function:\n ");
	switch (iCmd) {
		case FID_SCENE_MODE:
			HM5065_set_scene_mode(iPara);
	    	break; 	    
		case FID_AWB_MODE:
				HM5065_set_param_wb(iPara);
			  break;
		case FID_COLOR_EFFECT:	    	    
				HM5065_set_param_effect(iPara);
		 	  break;
		case FID_AE_EV:   
				HM5065_set_param_exposure(iPara);
		    break;
		case FID_AE_FLICKER:    	    	    
				HM5065_set_param_banding(iPara);
		 	  break;
		case FID_AE_SCENE_MODE: 
				if (iPara == AE_MODE_OFF) 
				{
					spin_lock(&HM5065_drv_lock);
		 			HM5065_AE_ENABLE = KAL_FALSE; 
					spin_unlock(&HM5065_drv_lock);
        }
        else 
        {
					spin_lock(&HM5065_drv_lock);
		 			HM5065_AE_ENABLE = KAL_TRUE; 
					spin_unlock(&HM5065_drv_lock);
	     	}
				HM5065_set_AE_mode(HM5065_AE_ENABLE);
        break; 
		case FID_ISP_CONTRAST:
            HM5065_set_contrast(iPara);
            break;
        case FID_ISP_BRIGHT:
            HM5065_set_brightness(iPara);
            break;
        case FID_ISP_SAT:
            HM5065_set_saturation(iPara);
        break; 
    case FID_ZOOM_FACTOR:
   		    HM5065SENSORDB("FID_ZOOM_FACTOR:%d\n", iPara); 	    
					spin_lock(&HM5065_drv_lock);
	        zoom_factor = iPara; 
					spin_unlock(&HM5065_drv_lock);
            break; 
		case FID_AE_ISO:
            HM5065_set_iso(iPara);
            break;
#if 0 //afc
		case FID_AF_MODE:
	    	 HM5065_set_param_afmode(iPara);
					break;     
#endif            
	  default:
		 	      break;
	}
	HM5065SENSORDB("[HM5065]exit HM5065YUVSensorSetting function:\n ");
	  return TRUE;
}   /* HM5065YUVSensorSetting */

UINT32 HM5065YUVSetVideoMode(UINT16 u2FrameRate)
{
	HM5065SENSORDB("[HM5065]CONTROLFLOW enter HM5065YUVSetVideoMode function:\n ");
	//sHM5065Sensor.VideoMode == KAL_TRUE;
	spin_lock(&HM5065_drv_lock);	
	HM5065Sensor.VideoMode= KAL_TRUE;
	spin_unlock(&HM5065_drv_lock);

	if (u2FrameRate == 30)
	{
		//;HM5065 1280x960,30fps
		//56Mhz, 224Mbps/Lane, 2Lane.
		HM5065SENSORDB("[HM5065]HM5065YUVSetVideoMode enter u2FrameRate == 30 setting  :\n ");	

		//HM5065_write_cmos_sensor(0x0040,0x01);	//	binning mode and subsampling mode for frame rate
		//		HM5065_write_cmos_sensor(0x0041,0x0A);	//	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
		//		HM5065_write_cmos_sensor(0x0042,0x05);	//05	X:800 0x500=1280,0x0320=800
		//			HM5065_write_cmos_sensor(0x0043,0x00);	//00
		//			HM5065_write_cmos_sensor(0x0044,0x03);	//03	Y:600 0x03c0=960,0x0258=600
		//			HM5065_write_cmos_sensor(0x0045,0xc0);	//c0
		
		#if 1
				HM5065_write_cmos_sensor(0x00E8,0x00);
				HM5065_write_cmos_sensor(0x00C8,0x00);
				HM5065_write_cmos_sensor(0x00C9,0x1F);//30fps
				HM5065_write_cmos_sensor(0x00CA,0x01);
		#endif

		if(HM5065_run_test_pattern)
		{
			HM5065_run_test_pattern=0;
			HM5065SetTestPatternMode(1);
		}
		HM5065SENSORDB("[HM5065]HM5065YUVSetVideoMode exit u2FrameRate == 30 setting  :\n ");
		}
    else if (u2FrameRate == 15)   
	{
		//;HM5065 1280x960,15fps
		//28Mhz, 112Mbps/Lane, 2Lane.
		HM5065SENSORDB("[HM5065]HM5065YUVSetVideoMode enter u2FrameRate == 15 setting  :\n ");	
		//HM5065_write_cmos_sensor(0x0040,0x01); 	//	binning mode and subsampling mode for frame rate
		//	HM5065_write_cmos_sensor(0x0041,0x0A); 	//	04 : VGA mode : 0A : self define ; 00 : 5M ;03:SVGA
		//		HM5065_write_cmos_sensor(0x0042,0x05); 	//05	X:800 0x500=1280,0x0320=800
		//		HM5065_write_cmos_sensor(0x0043,0x00); 	//00
		//		HM5065_write_cmos_sensor(0x0044,0x03); 	//03	Y:600 0x03c0=960,0x0258=600
		//	HM5065_write_cmos_sensor(0x0045, 0xC0); 	//

	
			HM5065_write_cmos_sensor(0x00E8,0x00);
			HM5065_write_cmos_sensor(0x00C8,0x00);
			HM5065_write_cmos_sensor(0x00C9,0x0F);//15fps
			HM5065_write_cmos_sensor(0x00CA,0x01);


		if(HM5065_run_test_pattern)
		{
			HM5065_run_test_pattern=0;
			HM5065SetTestPatternMode(1);
		}
		HM5065SENSORDB("[HM5065]HM5065YUVSetVideoMode exit u2FrameRate == 15 setting  :\n ");
	}   
    else 
    {
        HM5065SENSORDB("Wrong frame rate setting \n");
    } 
	HM5065SENSORDB("[HM5065]exit HM5065YUVSetVideoMode function:\n ");
    return TRUE; 
}

/**************************/
static void HM5065GetEvAwbRef(UINT32 pSensorAEAWBRefStruct)
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

static void HM5065GetCurAeAwbInfo(UINT32 pSensorAEAWBCurStruct)
{

	PSENSOR_AE_AWB_CUR_STRUCT Info = (PSENSOR_AE_AWB_CUR_STRUCT)pSensorAEAWBCurStruct;
	Info->SensorAECur.AeCurShutter=HM5065ReadShutter();
	Info->SensorAECur.AeCurGain=HM5065ReadSensorGain() ;
	Info->SensorAwbGainCur.AwbCurRgain=((HM5065_read_cmos_sensor(0x3401)&&0xff)+((HM5065_read_cmos_sensor(0x3400)&&0xff)*256));
	Info->SensorAwbGainCur.AwbCurBgain=((HM5065_read_cmos_sensor(0x3405)&&0xff)+((HM5065_read_cmos_sensor(0x3404)&&0xff)*256));

}
UINT32 HM5065MaxFramerateByScenario(MSDK_SCENARIO_ID_ENUM scenarioId, MUINT32 frameRate) 
	{
		kal_uint32 pclk;
		kal_int16 dummyLine;
		kal_uint16 lineLength,frameHeight;
		HM5065SENSORDB("HM5065MaxFramerateByScenario: scenarioId = %d, frame rate = %d\n",scenarioId,frameRate);
		HM5065SENSORDB("[HM5065]enter HM5065MaxFramerateByScenario function:\n ");
		switch (scenarioId) {
			case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
				pclk = 56000000;
				lineLength = HM5065_IMAGE_SENSOR_SVGA_WIDTH;
				frameHeight = (10 * pclk)/frameRate/lineLength;
				dummyLine = frameHeight - HM5065_IMAGE_SENSOR_SVGA_HEIGHT;
				if(dummyLine<0)
					dummyLine = 0;
				spin_lock(&HM5065_drv_lock);
				HM5065Sensor.SensorMode= SENSOR_MODE_PREVIEW;
				HM5065Sensor.PreviewDummyLines = dummyLine;
				spin_unlock(&HM5065_drv_lock);
				HM5065SetDummy(HM5065Sensor.PreviewDummyPixels, dummyLine);			
				break;			
			case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
				pclk = 56000000;
				lineLength = HM5065_IMAGE_SENSOR_VIDEO_WITDH;
				frameHeight = (10 * pclk)/frameRate/lineLength;
				dummyLine = frameHeight - HM5065_IMAGE_SENSOR_VIDEO_HEIGHT;
				if(dummyLine<0)
					dummyLine = 0;
				//spin_lock(&HM5065_drv_lock);
				//ov8825.sensorMode = SENSOR_MODE_VIDEO;
				//spin_unlock(&HM5065_drv_lock);
				HM5065SetDummy(0, dummyLine);			
				break;			
			case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			case MSDK_SCENARIO_ID_CAMERA_ZSD:			
				pclk = 90000000;
				lineLength = HM5065_IMAGE_SENSOR_QSXGA_WITDH;
				frameHeight = (10 * pclk)/frameRate/lineLength;
				dummyLine = frameHeight - HM5065_IMAGE_SENSOR_QSXGA_HEIGHT;
				if(dummyLine<0)
					dummyLine = 0;
				spin_lock(&HM5065_drv_lock);
				HM5065Sensor.CaptureDummyLines = dummyLine;
				HM5065Sensor.SensorMode= SENSOR_MODE_CAPTURE;
				spin_unlock(&HM5065_drv_lock);
				HM5065SetDummy(HM5065Sensor.CaptureDummyPixels, dummyLine);			
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
		HM5065SENSORDB("[HM5065]exit HM5065MaxFramerateByScenario function:\n ");
		return ERROR_NONE;
	}
UINT32 HM5065GetDefaultFramerateByScenario(MSDK_SCENARIO_ID_ENUM scenarioId, MUINT32 *pframeRate) 
{
	HM5065SENSORDB("[HM5065]enter HM5065GetDefaultFramerateByScenario function:\n ");
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
	HM5065SENSORDB("[HM5065]exit HM5065GetDefaultFramerateByScenario function:\n ");
	return ERROR_NONE;
}
void HM5065_get_AEAWB_lock(UINT32 *pAElockRet32, UINT32 *pAWBlockRet32)
{
	HM5065SENSORDB("[HM5065]enter HM5065_get_AEAWB_lock function:\n ");
	*pAElockRet32 =1;
	*pAWBlockRet32=1;
	HM5065SENSORDB("[HM5065]HM5065_get_AEAWB_lock,AE=%d,AWB=%d\n",*pAElockRet32,*pAWBlockRet32);
	HM5065SENSORDB("[HM5065]exit HM5065_get_AEAWB_lock function:\n ");
}
void HM5065_GetDelayInfo(UINT32 delayAddr)
{
	HM5065SENSORDB("[HM5065]enter HM5065_GetDelayInfo function:\n ");
	SENSOR_DELAY_INFO_STRUCT *pDelayInfo=(SENSOR_DELAY_INFO_STRUCT*)delayAddr;
	pDelayInfo->InitDelay=1;
	pDelayInfo->EffectDelay=3;
	pDelayInfo->AwbDelay=3;
	pDelayInfo->AFSwitchDelayFrame=50;
	HM5065SENSORDB("[HM5065]exit HM5065_GetDelayInfo function:\n ");
}
void HM5065_AutoTestCmd(UINT32 *cmd,UINT32 *para)
{
	HM5065SENSORDB("[HM5065]enter HM5065_AutoTestCmd function:\n ");
	switch(*cmd)
	{
		case YUV_AUTOTEST_SET_SHADDING:
			HM5065SENSORDB("YUV_AUTOTEST_SET_SHADDING:para=%d\n",*para);
			break;
		case YUV_AUTOTEST_SET_GAMMA:
			HM5065SENSORDB("YUV_AUTOTEST_SET_GAMMA:para=%d\n",*para);
			break;
		case YUV_AUTOTEST_SET_AE:
			HM5065SENSORDB("YUV_AUTOTEST_SET_AE:para=%d\n",*para);
			break;
		case YUV_AUTOTEST_SET_SHUTTER:
			HM5065SENSORDB("YUV_AUTOTEST_SET_SHUTTER:para=%d\n",*para);
			break;
		case YUV_AUTOTEST_SET_GAIN:
			HM5065SENSORDB("YUV_AUTOTEST_SET_GAIN:para=%d\n",*para);
			break;
		case YUV_AUTOTEST_GET_SHUTTER_RANGE:
			*para=8228;
			break;
		default:
			HM5065SENSORDB("YUV AUTOTEST NOT SUPPORT CMD:%d\n",*cmd);
			break;	
	}
	HM5065SENSORDB("[HM5065]exit HM5065_AutoTestCmd function:\n ");
}
void HM5065_3ACtrl(ACDK_SENSOR_3A_LOCK_ENUM action)
{
   switch (action)
   {
      case SENSOR_3A_AE_LOCK:
          spin_lock(&HM5065_drv_lock);
          HM5065Sensor.userAskAeLock = TRUE;
          spin_unlock(&HM5065_drv_lock);
          HM5065_set_AE_mode(KAL_FALSE);
      break;
      case SENSOR_3A_AE_UNLOCK:
          spin_lock(&HM5065_drv_lock);
          HM5065Sensor.userAskAeLock = FALSE;
          spin_unlock(&HM5065_drv_lock);
          HM5065_set_AE_mode(KAL_TRUE);
      break;

      case SENSOR_3A_AWB_LOCK:
          spin_lock(&HM5065_drv_lock);
          HM5065Sensor.userAskAwbLock = TRUE;
          spin_unlock(&HM5065_drv_lock);
      //    HM5065_set_AWB_mode(KAL_FALSE);
      break;

      case SENSOR_3A_AWB_UNLOCK:
          spin_lock(&HM5065_drv_lock);
          HM5065Sensor.userAskAwbLock = FALSE;
          spin_unlock(&HM5065_drv_lock);
      //    HM5065_set_AWB_mode(KAL_TRUE);
      break;
      default:
      	break;
   }
   return;
}

UINT32 HM5065FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
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
	HM5065SENSORDB("[HM5065][HM5065FeatureControl]feature id=%d \n",FeatureId);
	switch (FeatureId)
	{
		case SENSOR_FEATURE_GET_RESOLUTION:
			*pFeatureReturnPara16++=HM5065_IMAGE_SENSOR_QSXGA_WITDH;
			*pFeatureReturnPara16=HM5065_IMAGE_SENSOR_QSXGA_HEIGHT;
			*pFeatureParaLen=4;
			break;
		case SENSOR_FEATURE_GET_PERIOD:
			switch(CurrentScenarioId)
			{
				case MSDK_SCENARIO_ID_CAMERA_ZSD:
					*pFeatureReturnPara16++=HM5065_FULL_PERIOD_PIXEL_NUMS + HM5065Sensor.CaptureDummyPixels;
					*pFeatureReturnPara16=HM5065_FULL_PERIOD_LINE_NUMS + HM5065Sensor.CaptureDummyLines;
					*pFeatureParaLen=4;
					break;
				default:
					*pFeatureReturnPara16++=HM5065_PV_PERIOD_PIXEL_NUMS + HM5065Sensor.PreviewDummyPixels;
					*pFeatureReturnPara16=HM5065_PV_PERIOD_LINE_NUMS + HM5065Sensor.PreviewDummyLines;
					*pFeatureParaLen=4;
					break;
			}
			break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			switch(CurrentScenarioId)
			{
				case MSDK_SCENARIO_ID_CAMERA_ZSD:
					*pFeatureReturnPara32 = HM5065Sensor.ZsdturePclk * 1000 *100;	 //unit: Hz				
					*pFeatureParaLen=4;
					break;
				default:
					*pFeatureReturnPara32 = HM5065Sensor.PreviewPclk * 1000 *100;	 //unit: Hz
					*pFeatureParaLen=4;
					break;
			}
			break;
		case SENSOR_FEATURE_SET_ESHUTTER:
			break;
		case SENSOR_FEATURE_GET_EXIF_INFO:
            HM5065GetExifInfo(*pFeatureData32);
            break;
		case SENSOR_FEATURE_SET_NIGHTMODE:
			HM5065_night_mode((BOOL) *pFeatureData16);
			break;
		case SENSOR_FEATURE_SET_GAIN:
			break;
		case SENSOR_FEATURE_SET_FLASHLIGHT:
			break;
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			break;
		case SENSOR_FEATURE_SET_REGISTER:
			HM5065_write_cmos_sensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
			break;
		case SENSOR_FEATURE_GET_REGISTER:
			pSensorRegData->RegData = HM5065_read_cmos_sensor(pSensorRegData->RegAddr);
			break;
		case SENSOR_FEATURE_GET_CONFIG_PARA:
			memcpy(pSensorConfigData, &HM5065SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
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
			HM5065_GetSensorID(pFeatureData32);
			break;
		case SENSOR_FEATURE_GET_TEST_PATTERN_CHECKSUM_VALUE:
			*pFeatureReturnPara32=HM5065_TEST_PATTERN_CHECKSUM;
			*pFeatureParaLen=4;
			break;
		case SENSOR_FEATURE_SET_YUV_CMD:
			HM5065YUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_SET_YUV_3A_CMD:
            HM5065_3ACtrl((ACDK_SENSOR_3A_LOCK_ENUM)*pFeatureData32);
            break;
		case SENSOR_FEATURE_SET_VIDEO_MODE:
		    HM5065YUVSetVideoMode(*pFeatureData16);
		    break;
		case SENSOR_FEATURE_GET_EV_AWB_REF:
			HM5065GetEvAwbRef(*pFeatureData32);
			break;
		case SENSOR_FEATURE_GET_SHUTTER_GAIN_AWB_GAIN:
			HM5065GetCurAeAwbInfo(*pFeatureData32);			
			break;
		case SENSOR_FEATURE_SET_MAX_FRAME_RATE_BY_SCENARIO:
			HM5065MaxFramerateByScenario((MSDK_SCENARIO_ID_ENUM)*pFeatureData32,*(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_GET_DEFAULT_FRAME_RATE_BY_SCENARIO:
			HM5065GetDefaultFramerateByScenario((MSDK_SCENARIO_ID_ENUM)*pFeatureData32,(MUINT32 *)*(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_GET_AE_AWB_LOCK_INFO:
			HM5065_get_AEAWB_lock(*pFeatureData32, *(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_GET_DELAY_INFO:
			HM5065SENSORDB("SENSOR_FEATURE_GET_DELAY_INFO\n");
			HM5065_GetDelayInfo(*pFeatureData32);
			break;
		case SENSOR_FEATURE_AUTOTEST_CMD:
			HM5065SENSORDB("SENSOR_FEATURE_AUTOTEST_CMD\n");
			HM5065_AutoTestCmd(*pFeatureData32,*(pFeatureData32+1));
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
            HM5065SENSORDB("AE zone addr = 0x%x\n",*pFeatureData32);			
            HM5065_FOCUS_Set_AE_Window(*pFeatureData32);
            break; 
		default:
			HM5065SENSORDB("HM5065FeatureControl:default \n");
			break;			
	}
	HM5065SENSORDB("[HM5065]exit HM5065FeatureControl function:\n ");
	return ERROR_NONE;
}	/* HM5065FeatureControl() */

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



