#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include "kd_camera_hw.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"

#include "hm2057yuv_Sensor.h"
#include "hm2057yuv_Camera_Sensor_para.h"
#include "hm2057yuv_CameraCustomized.h"

#define HM2057YUV_DEBUG
#ifdef HM2057YUV_DEBUG
#define SENSORDB printk
#else
#define SENSORDB(x,...)
#endif

static HM2057_SENSOR_INFO_ST HM2057_sensor;

extern int iReadReg(u16 a_u2Addr , u8 * a_puBuff , u16 i2cId, bool isRegAddr16bit);
extern int iWriteReg(u16 a_u2Addr , u32 a_u4Data , u32 a_u4Bytes , u16 i2cId, bool isRegAddr16bit);
#define HM2057_write_cmos_sensor(addr, para) iWriteReg((u16) addr , (u32) para ,1,HM2057_WRITE_ID, true)
#define HM2057_write_cmos_sensor_2(addr, para, bytes) iWriteReg((u16) addr , (u32) para ,bytes,HM2057_WRITE_ID, true)

kal_uint16 HM2057_read_cmos_sensor(kal_uint32 addr)
{
kal_uint16 get_byte=0;
    iReadReg((u16) addr ,(u8*)&get_byte,HM2057_WRITE_ID, true);
    return get_byte;
}
/*
extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);

#define Sleep(ms) mdelay(ms)

kal_uint16 HM2057_write_cmos_sensor(kal_uint32 Addr, kal_uint32 Para)
{
  char pSendCmd[2] = {(char)(Addr & 0xFF) ,(char)(Para & 0xFF)};
  
  //SENSORDB("[HI342]HI342WriteCmosSensor,Addr:%x;Para:%x \n",Addr,Para);
  iWriteRegI2C(pSendCmd , 2,HM2057_WRITE_ID);
}

kal_uint16 HM2057_read_cmos_sensor(kal_uint32 Addr)
{
  char pGetByte=0;
  char pSendCmd = (char)(Addr & 0xFF);
  iReadRegI2C(&pSendCmd , 1, &pGetByte,1,HM2057_WRITE_ID);
  return pGetByte;
}
*/
/*******************************************************************************
* // Adapter for Winmo typedef 
********************************************************************************/
#define WINMO_USE 0

#define Sleep(ms) mdelay(ms)
#define RETAILMSG(x,...)
#define TEXT


/*******************************************************************************
* // End Adapter for Winmo typedef 
********************************************************************************/


#define	HM2057_LIMIT_EXPOSURE_LINES				(1253)
#define	HM2057_VIDEO_NORMALMODE_30FRAME_RATE       (30)
#define	HM2057_VIDEO_NORMALMODE_FRAME_RATE         (15)
#define	HM2057_VIDEO_NIGHTMODE_FRAME_RATE          (7.5)
#define BANDING50_30HZ
/* Global Valuable */

static kal_uint32 zoom_factor = 0; 
static kal_uint8 HM2057_exposure_line_h = 0, HM2057_exposure_line_l = 0,HM2057_extra_exposure_line_h = 0, HM2057_extra_exposure_line_l = 0;

static kal_bool HM2057_gPVmode = KAL_TRUE; //PV size or Full size
static kal_bool HM2057_VEDIO_encode_mode = KAL_FALSE; //Picture(Jpeg) or Video(Mpeg4)
static kal_bool HM2057_sensor_cap_state = KAL_FALSE; //Preview or Capture

static kal_uint16 HM2057_dummy_pixels=0, HM2057_dummy_lines=0;

static kal_uint16 HM2057_exposure_lines=0, HM2057_extra_exposure_lines = 0;


static kal_int8 HM2057_DELAY_AFTER_PREVIEW = -1;

static kal_uint8 HM2057_Banding_setting = AE_FLICKER_MODE_50HZ;  //Wonder add

/****** OVT 6-18******/
static kal_uint16 HM2057_Capture_Max_Gain16= 6*16;
static kal_uint16 HM2057_Capture_Gain16=0 ;    
static kal_uint16 HM2057_Capture_Shutter=0;
static kal_uint16 HM2057_Capture_Extra_Lines=0;

static kal_uint16  HM2057_PV_Dummy_Pixels =0, HM2057_Capture_Dummy_Pixels =0, HM2057_Capture_Dummy_Lines =0;
static kal_uint16  HM2057_PV_Gain16 = 0;
static kal_uint16  HM2057_PV_Shutter = 0;
static kal_uint16  HM2057_PV_Extra_Lines = 0;

kal_uint16 HM2057_sensor_gain_base=0,HM2057_FAC_SENSOR_REG=0,HM2057_iHM2057_Mode=0,HM2057_max_exposure_lines=0;
kal_uint32 HM2057_capture_pclk_in_M=520,HM2057_preview_pclk_in_M=390,HM2057_PV_dummy_pixels=0,HM2057_PV_dummy_lines=0,HM2057_isp_master_clock=0;


static kal_uint32  HM2057_sensor_pclk=390;
static kal_bool HM2057_AWB_ENABLE = KAL_TRUE; 
static kal_bool HM2057_AE_ENABLE = KAL_TRUE; 
kal_bool CapState = KAL_FALSE; 
kal_uint32  Shutter = 1;

static kal_uint32 Capture_Shutter = 0; 
static kal_uint32 Capture_Gain = 0; 
//static kal_uint32 HM2057_sensor.pv_shutter = 0;
//static kal_uint32 HM2057_sensor.pv_extra_shutter = 0;

UINT8 HM2057_PixelClockDivider=0;

//SENSOR_REG_STRUCT HM2057SensorCCT[FACTORY_END_ADDR]=CAMERA_SENSOR_CCT_DEFAULT_VALUE;
//SENSOR_REG_STRUCT HM2057SensorReg[ENGINEER_END]=CAMERA_SENSOR_REG_DEFAULT_VALUE;
//	camera_para.SENSOR.cct	SensorCCT	=> SensorCCT
//	camera_para.SENSOR.reg	SensorReg
MSDK_SENSOR_CONFIG_STRUCT HM2057SensorConfigData;



kal_uint16 HM2057_read_HM2057_gain(void)
{
	printk("%s\n", __func__);
	return 0;//liuhui add
}  /* HM2057_read_HM2057_gain */


kal_uint16 HM2057_read_shutter(void)
{kal_uint16 temp_reg1, temp_reg2;
	
	/* Backup the preview mode last shutter & sensor gain. */
	temp_reg1 = HM2057_read_cmos_sensor(0x0015);
	temp_reg2 = HM2057_read_cmos_sensor(0x0016);
	HM2057_sensor.pv_shutter = (temp_reg1 << 8) | (temp_reg2 & 0xFF);
	
	/* Backup the preview mode last shutter & sensor gain. */
	HM2057_sensor.pv_extra_shutter = 0;
	
	return HM2057_sensor.pv_shutter + HM2057_sensor.pv_extra_shutter;
}    /* HM2057_read_shutter */

void HM2057_write_HM2057_gain(kal_uint16 gain)
{    
	printk("%s\n", __func__);
	
}  /* HM2057_write_HM2057_gain */

static void HM2057_write_shutter(kal_uint16 shutter)
{
		kal_uint16 AGain_shutter,DGain_shutter;
	kal_uint16 temp_reg1, temp_reg2;	
		temp_reg1=(shutter>>8);
		temp_reg2=(shutter&0X00FF);
HM2057_write_cmos_sensor(0x0015,temp_reg1);
HM2057_write_cmos_sensor(0x0016,temp_reg2);
//	AGain_shutter = (HM2057_read_cmos_sensor(0x0014)<<8 )|( HM2057_read_cmos_sensor(0x0015));
//	DGain_shutter = (HM2057_read_cmos_sensor(0x0016)<<8 )|( HM2057_read_cmos_sensor(0x0017));
//
//	HM2057_write_cmos_sensor(0x0305 , shutter&0xff);           
//	HM2057_write_cmos_sensor(0x0304 , (shutter>>8)&0xff); 
//
//	HM2057_write_cmos_sensor(0x0307 , AGain_shutter&0xff);      
//	HM2057_write_cmos_sensor(0x0306 , (AGain_shutter>>8)&0xff); //AG
//
//	HM2057_write_cmos_sensor(0x0308,  (DGain_shutter>>2)&0xff);   //DG
//
//	HM2057_write_cmos_sensor(0x0300 , 0x01); //close ALC
}


void HM2057_Computer_AECAGC(kal_uint16 preview_clk_in_M, kal_uint16 capture_clk_in_M)
{
	printk("%s\n", __func__);
    kal_uint16 PV_Line_Width;
    kal_uint16 Capture_Line_Width;
    kal_uint16 Capture_Maximum_Shutter;
    kal_uint16 Capture_Exposure;
    kal_uint16 Capture_Gain16;
    kal_uint32 Capture_Banding_Filter;
    kal_uint32 Gain_Exposure=0;

    PV_Line_Width = HM2057_PV_PERIOD_PIXEL_NUMS + HM2057_PV_Dummy_Pixels;   

    Capture_Line_Width = HM2057_FULL_PERIOD_PIXEL_NUMS + HM2057_Capture_Dummy_Pixels;
    Capture_Maximum_Shutter = HM2057_FULL_EXPOSURE_LIMITATION + HM2057_Capture_Dummy_Lines;

    if (HM2057_Banding_setting == AE_FLICKER_MODE_50HZ)
#if WINMO_USE        
        Capture_Banding_Filter = (kal_uint32)(capture_clk_in_M*1000000/100/(2*Capture_Line_Width)+0.5);
#else 
        Capture_Banding_Filter = (kal_uint32)(capture_clk_in_M*100000/100/(2*Capture_Line_Width));
#endif 
    else
#if WINMO_USE
        Capture_Banding_Filter = (kal_uint16)(capture_clk_in_M*1000000/120/(2*Capture_Line_Width)+0.5);
#else 
        Capture_Banding_Filter = (kal_uint32)(capture_clk_in_M*100000/120/(2*Capture_Line_Width) );
#endif 

    /*   Gain_Exposure = HM2057_PV_Gain16*(HM2057_PV_Shutter+HM2057_PV_Extra_Lines)*PV_Line_Width/g_Preview_PCLK_Frequency/Capture_Line_Width*g_Capture_PCLK_Frequency
    ;*/
    HM2057_PV_Gain16 = HM2057_read_HM2057_gain();
    Gain_Exposure = 1 * HM2057_PV_Gain16;  //For HM2057

    Gain_Exposure *=(HM2057_PV_Shutter+HM2057_PV_Extra_Lines);
    Gain_Exposure *=PV_Line_Width;  //970
    //   Gain_Exposure /=g_Preview_PCLK_Frequency;
    Gain_Exposure /=Capture_Line_Width;//1940
    Gain_Exposure = Gain_Exposure*capture_clk_in_M/preview_clk_in_M;// for clock   

    //redistribute gain and exposure
    if (Gain_Exposure < (kal_uint32)(Capture_Banding_Filter * 16))     // Exposure < 1/100/120
    {
       if(Gain_Exposure<16){//exposure line smaller than 2 lines and gain smaller than 0x08 
            Gain_Exposure = Gain_Exposure*4;     
            Capture_Exposure = 1;
            Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2/4;
        }
        else
        {
            Capture_Exposure = Gain_Exposure /16;
            Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2;
        }
    }
    else 
    {
        if (Gain_Exposure >(kal_uint32)( Capture_Maximum_Shutter * 16)) // Exposure > Capture_Maximum_Shutter
        {
           
            Capture_Exposure = Capture_Maximum_Shutter/Capture_Banding_Filter*Capture_Banding_Filter;
            Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2;
            if (Capture_Gain16 > HM2057_Capture_Max_Gain16) 
            {
                // gain reach maximum, insert extra line
                Capture_Exposure = (kal_uint16)(Gain_Exposure*11 /10 /HM2057_Capture_Max_Gain16);
                
                // Exposure = n/100/120
                Capture_Exposure = Capture_Exposure/Capture_Banding_Filter * Capture_Banding_Filter;
                Capture_Gain16 = ((Gain_Exposure *4)/ Capture_Exposure+3)/4;
            }
        }
        else  // 1/100 < Exposure < Capture_Maximum_Shutter, Exposure = n/100/120
        {
            Capture_Exposure = Gain_Exposure/16/Capture_Banding_Filter;
            Capture_Exposure = Capture_Exposure * Capture_Banding_Filter;
            Capture_Gain16 = (Gain_Exposure*2 +1) / Capture_Exposure/2;
        }
    }
    
    HM2057_Capture_Gain16 = Capture_Gain16;
    HM2057_Capture_Extra_Lines = (Capture_Exposure > Capture_Maximum_Shutter)?
            (Capture_Exposure - Capture_Maximum_Shutter/Capture_Banding_Filter*Capture_Banding_Filter):0;     
    
    HM2057_Capture_Shutter = Capture_Exposure - HM2057_Capture_Extra_Lines;
}

void HM2057_set_isp_driving_current(kal_uint8 current)
{
	printk("%s\n", __func__);
}


static void HM2057_set_AE_mode(kal_bool AE_enable)
{
	printk("%s\n", __func__);
	
kal_uint16 temp_AE_reg = 0;
	temp_AE_reg = HM2057_read_cmos_sensor(0x0380);
	
	if (AE_enable)	  
	{
		HM2057_write_cmos_sensor(0x0380, (temp_AE_reg| 0x01));	/* Turn ON AEC/AGC*/
	}
	else
	{
		HM2057_write_cmos_sensor(0x0380, (temp_AE_reg&(~0x01))); /* Turn OFF AEC/AGC*/
	}

	HM2057_write_cmos_sensor(0x0000,0xFF);	
	HM2057_write_cmos_sensor(0x0100,0xFF);
	HM2057_write_cmos_sensor(0x0101,0xFF);

	return KAL_TRUE;
	
}


static void HM2057_set_AWB_mode(kal_bool AWB_enable)
{
	printk("%s\n", __func__);
    kal_uint8 temp_AWB_reg = 0;
    temp_AWB_reg = HM2057_read_cmos_sensor(0x0380);
    //return ;

    if (AWB_enable == KAL_TRUE)
    {
        //enable Auto WB
		HM2057_write_cmos_sensor(0x0380, (temp_AWB_reg | 0x02));
	  }
	else
	{
		HM2057_write_cmos_sensor(0x0380, (temp_AWB_reg& ~0x02));		
	 }

	HM2057_write_cmos_sensor(0x0000,0xFF);	
	HM2057_write_cmos_sensor(0x0100,0xFF);
	HM2057_write_cmos_sensor(0x0101,0xFF);       
    return KAL_TRUE;
}


/*************************************************************************
* FUNCTION
*	HM2057_night_mode
*
* DESCRIPTION
*	This function night mode of HM2057.
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
bool HM2057_set_param_banding(UINT16 para)
{

    kal_uint8 banding;
    
    //banding = HM2057_read_cmos_sensor(0x0120);
printk("[HM2057] HM2057flicker. \n");
    switch (para)
    {
	case AE_FLICKER_MODE_50HZ:
		HM2057_Banding_setting = AE_FLICKER_MODE_50HZ;
		HM2057_write_cmos_sensor(0x0120, 0x36);	//[0]:0=>50Hz 
		HM2057_write_cmos_sensor(0x0000, 0xFF);	//AE CMU   
		HM2057_write_cmos_sensor(0x0100, 0xFF);	//AE CMU    
		HM2057_write_cmos_sensor(0x0101, 0xFF);	//AE CMU  
		break;
    	case AE_FLICKER_MODE_60HZ:			
        	HM2057_Banding_setting = AE_FLICKER_MODE_60HZ;
		HM2057_write_cmos_sensor(0x0120, 0x37);	//[0]:1=>60Hz
		HM2057_write_cmos_sensor(0x0000, 0xFF);	//AE CMU   
		HM2057_write_cmos_sensor(0x0100, 0xFF);	//AE CMU    
		HM2057_write_cmos_sensor(0x0101, 0xFF);	//AE CMU 		 
       		break;
      default:
          return FALSE;
    }

    return TRUE;
} /* HM2057_set_param_banding */


void HM2057_night_mode(kal_bool enable)
{
		printk("%s\n", __func__);
	if (enable) 		/* Night Mode */
	{
		/* HM2057 night mode enable. */
		if (HM2057_VEDIO_encode_mode == KAL_TRUE)	/* Video */
		{
			HM2057_write_cmos_sensor(0x038F,0x0A);	//Min 4 FPS.
			HM2057_write_cmos_sensor(0x0390,0x00);
		}
		else 										/* Camera */
		{
			HM2057_write_cmos_sensor(0x038F,0x0A);	//Min 4 FPS.
			HM2057_write_cmos_sensor(0x0390,0x00);
		}			
    
		HM2057_write_cmos_sensor(0x02E0,0x00);	// 00 for Night Mode, By Brandon/20110208
		HM2057_write_cmos_sensor(0x0481,0x06);	// 06 for Night Mode, By Brandon/20110208
		HM2057_write_cmos_sensor(0x04B1,0x88);	// 88 for Night Mode, By Brandon/20110208
		HM2057_write_cmos_sensor(0x04B4,0x20);	// 20 for Night Mode, By Brandon/20110208
		HM2057_write_cmos_sensor(0x0000,0xFF);
		HM2057_write_cmos_sensor(0x0100,0xFF);  
	}
	else  				/* Normal Mode */
	{
		/* HM2057 night mode disable. */
		if (HM2057_VEDIO_encode_mode == KAL_TRUE)	/* Video */
		{
			HM2057_write_cmos_sensor(0x038F,0x05);	//Min 8 FPS.
			HM2057_write_cmos_sensor(0x0390,0x04);
		}
		else										/* Camera */
		{
    		HM2057_write_cmos_sensor(0x038F,0x05);	//Min 8 FPS.
			HM2057_write_cmos_sensor(0x0390,0x04);
		}		
		
		HM2057_write_cmos_sensor(0x02E0,0x02);	//06->02, By Brandon/20110129
		HM2057_write_cmos_sensor(0x0481,0x08);	//06->08, By Brandon/20110129
		HM2057_write_cmos_sensor(0x04B1,0x00);
		HM2057_write_cmos_sensor(0x04B4,0x00);
		HM2057_write_cmos_sensor(0x0000,0xFF);
		HM2057_write_cmos_sensor(0x0100,0xFF);
	}
}	/* HM2057_night_mode */


/*************************************************************************
* FUNCTION
*	HM2057_YUV_sensor_initial_setting
*
* DESCRIPTION
*	This function apply all of the initial setting to sensor.
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
*************************************************************************/
static void HM2057_YUV_sensor_initial_setting(void)
{
		/* HM2057 initial setting date: 2010.12.28. */
HM2057_write_cmos_sensor(0x0022,0x00); //; Reset
HM2057_write_cmos_sensor(0x0004,0x10); //;
HM2057_write_cmos_sensor(0x0006,0x00); //; Flip/Mirror
HM2057_write_cmos_sensor(0x000D,0x11); //; 20120220 to fix morie
HM2057_write_cmos_sensor(0x000E,0x11); //; Binning ON
HM2057_write_cmos_sensor(0x000F,0x00); //; IMGCFG
HM2057_write_cmos_sensor(0x0011,0x02); //;
HM2057_write_cmos_sensor(0x0012,0x1C); //; 2012.02.08
HM2057_write_cmos_sensor(0x0013,0x01); //;
HM2057_write_cmos_sensor(0x0015,0x02); //;
HM2057_write_cmos_sensor(0x0016,0x80); //;
HM2057_write_cmos_sensor(0x0018,0x00); //;
HM2057_write_cmos_sensor(0x001D,0x40); //;
HM2057_write_cmos_sensor(0x0020,0x40); //;
HM2057_write_cmos_sensor(0x0025,0x80); //; CKCFG 80 from system clock, 00 from PLL
HM2057_write_cmos_sensor(0x0026,0x87); //; PLL1CFG should be 07 when system clock, should be 87 when PLL
HM2057_write_cmos_sensor(0x0027,0x18); //; YUV output
HM2057_write_cmos_sensor(0x002C,0x0A); //; Set default vaule for CP and resistance of LPF to 1010
HM2057_write_cmos_sensor(0x0040,0x20); //; 20120224 for BLC stable
HM2057_write_cmos_sensor(0x0053,0x0A); //;
HM2057_write_cmos_sensor(0x0044,0x06); //; enable BLC_phase2
HM2057_write_cmos_sensor(0x0046,0xD8); //; enable BLC_phase1, disable BLC_phase2 dithering
HM2057_write_cmos_sensor(0x004A,0x0A); //; disable BLC_phase2 hot pixel filter
HM2057_write_cmos_sensor(0x004B,0x72); //;
HM2057_write_cmos_sensor(0x0075,0x01); //; in OMUX data swap for debug usage
HM2057_write_cmos_sensor(0x002A,0x1F); //; Output=48MHz
HM2057_write_cmos_sensor(0x0070,0x5F); //;
HM2057_write_cmos_sensor(0x0071,0xFF); //;
HM2057_write_cmos_sensor(0x0072,0x55); //;
HM2057_write_cmos_sensor(0x0073,0x50); //;
HM2057_write_cmos_sensor(0x0080,0xC8); //; 2012.02.08
HM2057_write_cmos_sensor(0x0082,0xA2); //;
HM2057_write_cmos_sensor(0x0083,0xF0); //;
HM2057_write_cmos_sensor(0x0085,0x11); //; Enable Thin-Oxide Case (Kwangoh kim), Set ADC power to 100% Enable thermal sensor control bit[7] 0:on 1:off 2012 02 13 (YL)
HM2057_write_cmos_sensor(0x0086,0x02); //; K.Kim 2011.12.09
HM2057_write_cmos_sensor(0x0087,0x80); //; K.Kim 2011.12.09
HM2057_write_cmos_sensor(0x0088,0x6C); //;
HM2057_write_cmos_sensor(0x0089,0x2E); //;
HM2057_write_cmos_sensor(0x008A,0x7D); //; 20120224 for BLC stable
HM2057_write_cmos_sensor(0x008D,0x20); //;
HM2057_write_cmos_sensor(0x0090,0x00); //; 1.5x(Change Gain Table )
HM2057_write_cmos_sensor(0x0091,0x10); //; 3x  (3x CTIA)
HM2057_write_cmos_sensor(0x0092,0x11); //; 6x  (3x CTIA + 2x PGA)
HM2057_write_cmos_sensor(0x0093,0x12); //; 12x (3x CTIA + 4x PGA)
HM2057_write_cmos_sensor(0x0094,0x16); //; 24x (3x CTIA + 8x PGA)
HM2057_write_cmos_sensor(0x0095,0x08); //; 1.5x  20120217 for color shift
HM2057_write_cmos_sensor(0x0096,0x00); //; 3x    20120217 for color shift
HM2057_write_cmos_sensor(0x0097,0x10); //; 6x    20120217 for color shift
HM2057_write_cmos_sensor(0x0098,0x11); //; 12x   20120217 for color shift
HM2057_write_cmos_sensor(0x0099,0x12); //; 24x   20120217 for color shift
HM2057_write_cmos_sensor(0x009A,0x06); //; 24x  
HM2057_write_cmos_sensor(0x009B,0x34); //;
HM2057_write_cmos_sensor(0x00A0,0x00); //;
HM2057_write_cmos_sensor(0x00A1,0x04); //; 2012.02.06(for Ver.C)
HM2057_write_cmos_sensor(0x011F,0xF7); //; simple bpc P31 & P33[4] P40 P42 P44[5]
HM2057_write_cmos_sensor(0x0120,0x36); //; 36:50Hz, 37:60Hz, BV_Win_Weight_En=1
HM2057_write_cmos_sensor(0x0121,0x83); //; NSatScale_En=0, NSatScale=0
HM2057_write_cmos_sensor(0x0122,0x7B); //
HM2057_write_cmos_sensor(0x0123,0xC2); //
HM2057_write_cmos_sensor(0x0124,0xDE); //
HM2057_write_cmos_sensor(0x0125,0xFF); //
HM2057_write_cmos_sensor(0x0126,0x70); //
HM2057_write_cmos_sensor(0x0128,0x1F); //
HM2057_write_cmos_sensor(0x0132,0x10); //
HM2057_write_cmos_sensor(0x0131,0xBD); //; simle bpc enable[4]
HM2057_write_cmos_sensor(0x0140,0x14); //
HM2057_write_cmos_sensor(0x0141,0x0A); //
HM2057_write_cmos_sensor(0x0142,0x14); //
HM2057_write_cmos_sensor(0x0143,0x0A); //
HM2057_write_cmos_sensor(0x0144,0x06); //; Sort bpc hot pixel ratio
HM2057_write_cmos_sensor(0x0145,0x00); //
HM2057_write_cmos_sensor(0x0146,0x20); //
HM2057_write_cmos_sensor(0x0147,0x0A); //
HM2057_write_cmos_sensor(0x0148,0x10); //
HM2057_write_cmos_sensor(0x0149,0x0C); //
HM2057_write_cmos_sensor(0x014A,0x80); //
HM2057_write_cmos_sensor(0x014B,0x80); //
HM2057_write_cmos_sensor(0x014C,0x2E); //
HM2057_write_cmos_sensor(0x014D,0x2E); //
HM2057_write_cmos_sensor(0x014E,0x05); //
HM2057_write_cmos_sensor(0x014F,0x05); //
HM2057_write_cmos_sensor(0x0150,0x0D); //
HM2057_write_cmos_sensor(0x0155,0x00); //
HM2057_write_cmos_sensor(0x0156,0x10); //
HM2057_write_cmos_sensor(0x0157,0x0A); //
HM2057_write_cmos_sensor(0x0158,0x0A); //
HM2057_write_cmos_sensor(0x0159,0x0A); //
HM2057_write_cmos_sensor(0x015A,0x05); //
HM2057_write_cmos_sensor(0x015B,0x05); //
HM2057_write_cmos_sensor(0x015C,0x05); //
HM2057_write_cmos_sensor(0x015D,0x05); //
HM2057_write_cmos_sensor(0x015E,0x08); //
HM2057_write_cmos_sensor(0x015F,0xFF); //
HM2057_write_cmos_sensor(0x0160,0x50); //; OTP BPC 2line & 4line enable
HM2057_write_cmos_sensor(0x0161,0x20); //
HM2057_write_cmos_sensor(0x0162,0x14); //
HM2057_write_cmos_sensor(0x0163,0x0A); //
HM2057_write_cmos_sensor(0x0164,0x10); //; OTP 4line Strength
HM2057_write_cmos_sensor(0x0165,0x0A); //
HM2057_write_cmos_sensor(0x0166,0x0A); //
HM2057_write_cmos_sensor(0x018C,0x24); //
HM2057_write_cmos_sensor(0x018D,0x04); //; Cluster correction enable singal from thermal sensor (YL 2012 02 13)
HM2057_write_cmos_sensor(0x018E,0x00); //; Enable Thermal sensor control bit[7] (YL 2012 02 13)
HM2057_write_cmos_sensor(0x018F,0x11); //; Cluster Pulse enable T1[0] T2[1] T3[2] T4[3]
HM2057_write_cmos_sensor(0x0190,0x80); //; A11 BPC Strength[7:3], cluster correct P11[0]P12[1]P13[2]
HM2057_write_cmos_sensor(0x0191,0x47); //; A11[0],A7[1],Sort[3],A13 AVG[6]
HM2057_write_cmos_sensor(0x0192,0x48); //; A13 Strength[4:0],hot pixel detect for cluster[6]
HM2057_write_cmos_sensor(0x0193,0x64); //
HM2057_write_cmos_sensor(0x0194,0x32); //
HM2057_write_cmos_sensor(0x0195,0xc8); //
HM2057_write_cmos_sensor(0x0196,0x96); //
HM2057_write_cmos_sensor(0x0197,0x64); //
HM2057_write_cmos_sensor(0x0198,0x32); //
HM2057_write_cmos_sensor(0x0199,0x14); //; A13 hot pixel th
HM2057_write_cmos_sensor(0x019A,0x20); //; A13 edge detect th
HM2057_write_cmos_sensor(0x019B,0x14); //
HM2057_write_cmos_sensor(0x01B0,0x55); //; G1G2 Balance
HM2057_write_cmos_sensor(0x01B1,0x0C); //
HM2057_write_cmos_sensor(0x01B2,0x0A); //
HM2057_write_cmos_sensor(0x01B3,0x10); //
HM2057_write_cmos_sensor(0x01B4,0x0E); //
HM2057_write_cmos_sensor(0x01BA,0x10); //; BD
HM2057_write_cmos_sensor(0x01BB,0x04); //
HM2057_write_cmos_sensor(0x01D8,0x40); //
HM2057_write_cmos_sensor(0x01DE,0x60); //
HM2057_write_cmos_sensor(0x01E4,0x10); //
HM2057_write_cmos_sensor(0x01E5,0x10); //
HM2057_write_cmos_sensor(0x01F2,0x0C); //
HM2057_write_cmos_sensor(0x01F3,0x14); //
HM2057_write_cmos_sensor(0x01F8,0x04); //
HM2057_write_cmos_sensor(0x01F9,0x0C); //
HM2057_write_cmos_sensor(0x01FE,0x02); //
HM2057_write_cmos_sensor(0x01FF,0x04); //
HM2057_write_cmos_sensor(0x0220,0x00); //; LSC
HM2057_write_cmos_sensor(0x0221,0xB0); //
HM2057_write_cmos_sensor(0x0222,0x00); //
HM2057_write_cmos_sensor(0x0223,0x80); //
HM2057_write_cmos_sensor(0x0224,0x8E); //
HM2057_write_cmos_sensor(0x0225,0x00); //
HM2057_write_cmos_sensor(0x0226,0x88); //
HM2057_write_cmos_sensor(0x022A,0x88); //
HM2057_write_cmos_sensor(0x022B,0x00); //
HM2057_write_cmos_sensor(0x022C,0x88); //
HM2057_write_cmos_sensor(0x022D,0x13); //
HM2057_write_cmos_sensor(0x022E,0x0B); //
HM2057_write_cmos_sensor(0x022F,0x13); //
HM2057_write_cmos_sensor(0x0230,0x0B); //
HM2057_write_cmos_sensor(0x0233,0x13); //
HM2057_write_cmos_sensor(0x0234,0x0B); //
HM2057_write_cmos_sensor(0x0235,0x28); //
HM2057_write_cmos_sensor(0x0236,0x03); //
HM2057_write_cmos_sensor(0x0237,0x28); //
HM2057_write_cmos_sensor(0x0238,0x03); //
HM2057_write_cmos_sensor(0x023B,0x28); //
HM2057_write_cmos_sensor(0x023C,0x03); //
HM2057_write_cmos_sensor(0x023D,0x5C); //
HM2057_write_cmos_sensor(0x023E,0x02); //
HM2057_write_cmos_sensor(0x023F,0x5C); //
HM2057_write_cmos_sensor(0x0240,0x02); //
HM2057_write_cmos_sensor(0x0243,0x5C); //
HM2057_write_cmos_sensor(0x0244,0x02); //
HM2057_write_cmos_sensor(0x0251,0x0E); //
HM2057_write_cmos_sensor(0x0252,0x00); //
///
HM2057_write_cmos_sensor(0x0280,0x08); //; Gamma
HM2057_write_cmos_sensor(0x0281,0xFF);
HM2057_write_cmos_sensor(0x0282,0x0E); //
HM2057_write_cmos_sensor(0x0283,0xFF);
HM2057_write_cmos_sensor(0x0284,0x20); //
HM2057_write_cmos_sensor(0x0285,0xFF);
HM2057_write_cmos_sensor(0x0286,0x4B); //
HM2057_write_cmos_sensor(0x0288,0x60); //
HM2057_write_cmos_sensor(0x028A,0x6D); //
HM2057_write_cmos_sensor(0x028C,0x79); //
HM2057_write_cmos_sensor(0x028E,0x82); //
HM2057_write_cmos_sensor(0x0290,0x8A); //
HM2057_write_cmos_sensor(0x0292,0x91); //
HM2057_write_cmos_sensor(0x0294,0x9C); //
HM2057_write_cmos_sensor(0x0296,0xA6); //
HM2057_write_cmos_sensor(0x0298,0xB9); //
HM2057_write_cmos_sensor(0x029A,0xCD); //
HM2057_write_cmos_sensor(0x029C,0xE0); //
HM2057_write_cmos_sensor(0x029E,0x2D); //
//

#if 0
HM2057_write_cmos_sensor(0x0280,0x0A); //; Gamma
HM2057_write_cmos_sensor(0x0282,0x14); //
HM2057_write_cmos_sensor(0x0284,0x2A); //
HM2057_write_cmos_sensor(0x0286,0x50); //
HM2057_write_cmos_sensor(0x0288,0x60); //
HM2057_write_cmos_sensor(0x028A,0x6D); //
HM2057_write_cmos_sensor(0x028C,0x79); //
HM2057_write_cmos_sensor(0x028E,0x82); //
HM2057_write_cmos_sensor(0x0290,0x8A); //
HM2057_write_cmos_sensor(0x0292,0x91); //
HM2057_write_cmos_sensor(0x0294,0x9C); //
HM2057_write_cmos_sensor(0x0296,0xA7); //
HM2057_write_cmos_sensor(0x0298,0xBA); //
HM2057_write_cmos_sensor(0x029A,0xCD); //
HM2057_write_cmos_sensor(0x029C,0xE0); //
HM2057_write_cmos_sensor(0x029E,0x2D); //
#endif

HM2057_write_cmos_sensor(0x02A0,0x06); //; Gamma by Alpha
HM2057_write_cmos_sensor(0x02E0,0x04); //; CCM by Alpha
HM2057_write_cmos_sensor(0x02C0,0xB1); //; CCM
HM2057_write_cmos_sensor(0x02C1,0x01); //
HM2057_write_cmos_sensor(0x02C2,0x7D); //
HM2057_write_cmos_sensor(0x02C3,0x07); //
HM2057_write_cmos_sensor(0x02C4,0xD2); //
HM2057_write_cmos_sensor(0x02C5,0x07); //
HM2057_write_cmos_sensor(0x02C6,0xC4); //
HM2057_write_cmos_sensor(0x02C7,0x07); //
HM2057_write_cmos_sensor(0x02C8,0x79); //
HM2057_write_cmos_sensor(0x02C9,0x01); //
HM2057_write_cmos_sensor(0x02CA,0xC4); //
HM2057_write_cmos_sensor(0x02CB,0x07); //
HM2057_write_cmos_sensor(0x02CC,0xF7); //
HM2057_write_cmos_sensor(0x02CD,0x07); //
HM2057_write_cmos_sensor(0x02CE,0x3B); //
HM2057_write_cmos_sensor(0x02CF,0x07); //
HM2057_write_cmos_sensor(0x02D0,0xCF); //
HM2057_write_cmos_sensor(0x02D1,0x01); //
HM2057_write_cmos_sensor(0x0302,0x00); //
HM2057_write_cmos_sensor(0x0303,0x00); //
HM2057_write_cmos_sensor(0x0304,0x00); //
HM2057_write_cmos_sensor(0x02F0,0x5E); //
HM2057_write_cmos_sensor(0x02F1,0x07); //
HM2057_write_cmos_sensor(0x02F2,0xA0); //
HM2057_write_cmos_sensor(0x02F3,0x00); //
HM2057_write_cmos_sensor(0x02F4,0x02); //
HM2057_write_cmos_sensor(0x02F5,0x00); //
HM2057_write_cmos_sensor(0x02F6,0xC4); //
HM2057_write_cmos_sensor(0x02F7,0x07); //
HM2057_write_cmos_sensor(0x02F8,0x11); //
HM2057_write_cmos_sensor(0x02F9,0x00); //
HM2057_write_cmos_sensor(0x02FA,0x2A); //
HM2057_write_cmos_sensor(0x02FB,0x00); //
HM2057_write_cmos_sensor(0x02FC,0xA1); //
HM2057_write_cmos_sensor(0x02FD,0x07); //
HM2057_write_cmos_sensor(0x02FE,0xB8); //
HM2057_write_cmos_sensor(0x02FF,0x07); //
HM2057_write_cmos_sensor(0x0300,0xA7); //
HM2057_write_cmos_sensor(0x0301,0x00); //
HM2057_write_cmos_sensor(0x0305,0x00); //
HM2057_write_cmos_sensor(0x0306,0x00); //
HM2057_write_cmos_sensor(0x0307,0x7A); //
HM2057_write_cmos_sensor(0x032D,0x00); //
HM2057_write_cmos_sensor(0x032E,0x01); //
HM2057_write_cmos_sensor(0x032F,0x00); //
HM2057_write_cmos_sensor(0x0330,0x01); //
HM2057_write_cmos_sensor(0x0331,0x00); //
HM2057_write_cmos_sensor(0x0332,0x01); //
HM2057_write_cmos_sensor(0x0333,0x82); //; AWB channel offset
HM2057_write_cmos_sensor(0x0334,0x00); //
HM2057_write_cmos_sensor(0x0335,0x84); //
HM2057_write_cmos_sensor(0x0336,0x00); //	; LED AWB gain
HM2057_write_cmos_sensor(0x0337,0x01); //
HM2057_write_cmos_sensor(0x0338,0x00); //
HM2057_write_cmos_sensor(0x0339,0x01); //
HM2057_write_cmos_sensor(0x033A,0x00); //
HM2057_write_cmos_sensor(0x033B,0x01); //
HM2057_write_cmos_sensor(0x033E,0x04); //
HM2057_write_cmos_sensor(0x033F,0x86); //
HM2057_write_cmos_sensor(0x0340,0x30); //; AWB
HM2057_write_cmos_sensor(0x0341,0x44); //
HM2057_write_cmos_sensor(0x0342,0x4A); //
HM2057_write_cmos_sensor(0x0343,0x42); //; CT1
HM2057_write_cmos_sensor(0x0344,0x74); // 	;
HM2057_write_cmos_sensor(0x0345,0x4F); //; CT2
HM2057_write_cmos_sensor(0x0346,0x67); // 	;
HM2057_write_cmos_sensor(0x0347,0x5C); //; CT3
HM2057_write_cmos_sensor(0x0348,0x59); //;
HM2057_write_cmos_sensor(0x0349,0x67); //; CT4
HM2057_write_cmos_sensor(0x034A,0x4D); //;
HM2057_write_cmos_sensor(0x034B,0x6E); //; CT5
HM2057_write_cmos_sensor(0x034C,0x44); //;
HM2057_write_cmos_sensor(0x0350,0x80); //
HM2057_write_cmos_sensor(0x0351,0x80); //
HM2057_write_cmos_sensor(0x0352,0x18); //
HM2057_write_cmos_sensor(0x0353,0x18); //
HM2057_write_cmos_sensor(0x0354,0x6E); //
HM2057_write_cmos_sensor(0x0355,0x4A); //
HM2057_write_cmos_sensor(0x0356,0x7A); //
HM2057_write_cmos_sensor(0x0357,0xC6); //
HM2057_write_cmos_sensor(0x0358,0x06); //
HM2057_write_cmos_sensor(0x035A,0x06); //
HM2057_write_cmos_sensor(0x035B,0xA0); //
HM2057_write_cmos_sensor(0x035C,0x73); //
HM2057_write_cmos_sensor(0x035D,0x5A); //
HM2057_write_cmos_sensor(0x035E,0xC6); //
HM2057_write_cmos_sensor(0x035F,0xA0); //
HM2057_write_cmos_sensor(0x0360,0x02); //
HM2057_write_cmos_sensor(0x0361,0x18); //
HM2057_write_cmos_sensor(0x0362,0x80); //
HM2057_write_cmos_sensor(0x0363,0x6C); //
HM2057_write_cmos_sensor(0x0364,0x00); //
HM2057_write_cmos_sensor(0x0365,0xF0); //
HM2057_write_cmos_sensor(0x0366,0x20); //
HM2057_write_cmos_sensor(0x0367,0x0C); //
HM2057_write_cmos_sensor(0x0369,0x00); //
HM2057_write_cmos_sensor(0x036A,0x10); //
HM2057_write_cmos_sensor(0x036B,0x10); //
HM2057_write_cmos_sensor(0x036E,0x20); //
HM2057_write_cmos_sensor(0x036F,0x00); //
HM2057_write_cmos_sensor(0x0370,0x10); //
HM2057_write_cmos_sensor(0x0371,0x18); //
HM2057_write_cmos_sensor(0x0372,0x0C); //
HM2057_write_cmos_sensor(0x0373,0x38); //
HM2057_write_cmos_sensor(0x0374,0x3A); //
HM2057_write_cmos_sensor(0x0375,0x13); //
HM2057_write_cmos_sensor(0x0376,0x22); //
HM2057_write_cmos_sensor(0x0380,0xFF); //
HM2057_write_cmos_sensor(0x0381,0x4A); //
HM2057_write_cmos_sensor(0x0382,0x36); //
HM2057_write_cmos_sensor(0x038A,0x40); //
HM2057_write_cmos_sensor(0x038B,0x08); //
HM2057_write_cmos_sensor(0x038C,0xC1); //
HM2057_write_cmos_sensor(0x038E,0x40); //
HM2057_write_cmos_sensor(0x038F,0x04); // 0504 Min frame 8    0499 Min 9   0543??\A1\E4??\A1\C2?\A6̦\CC???\A8\BAy\A1\C0?
HM2057_write_cmos_sensor(0x0390,0x99); //
HM2057_write_cmos_sensor(0x0391,0x05); //05
HM2057_write_cmos_sensor(0x0392,0x02); //
HM2057_write_cmos_sensor(0x0393,0x80); //
HM2057_write_cmos_sensor(0x0395,0x21); //; AEAWB skip count
HM2057_write_cmos_sensor(0x0398,0x02); //; AE Frame Control
HM2057_write_cmos_sensor(0x0399,0x74); //
HM2057_write_cmos_sensor(0x039A,0x03); //
HM2057_write_cmos_sensor(0x039B,0x11); //
HM2057_write_cmos_sensor(0x039C,0x03); //
HM2057_write_cmos_sensor(0x039D,0xAE); //
HM2057_write_cmos_sensor(0x039E,0x04); //
HM2057_write_cmos_sensor(0x039F,0xE8); //
HM2057_write_cmos_sensor(0x03A0,0x06); //
HM2057_write_cmos_sensor(0x03A1,0x22); //
HM2057_write_cmos_sensor(0x03A2,0x07); //
HM2057_write_cmos_sensor(0x03A3,0x5C); //
HM2057_write_cmos_sensor(0x03A4,0x09); //
HM2057_write_cmos_sensor(0x03A5,0xD0); //
HM2057_write_cmos_sensor(0x03A6,0x0C); //
HM2057_write_cmos_sensor(0x03A7,0x0E); //
HM2057_write_cmos_sensor(0x03A8,0x10); //
HM2057_write_cmos_sensor(0x03A9,0x18); //
HM2057_write_cmos_sensor(0x03AA,0x20); //
HM2057_write_cmos_sensor(0x03AB,0x28); //
HM2057_write_cmos_sensor(0x03AC,0x1E); //
HM2057_write_cmos_sensor(0x03AD,0x1A); //
HM2057_write_cmos_sensor(0x03AE,0x13); //
HM2057_write_cmos_sensor(0x03AF,0x0C); //
HM2057_write_cmos_sensor(0x03B0,0x0B); //
HM2057_write_cmos_sensor(0x03B1,0x09); //
HM2057_write_cmos_sensor(0x03B3,0x10); //; AE window array
HM2057_write_cmos_sensor(0x03B4,0x00); //
HM2057_write_cmos_sensor(0x03B5,0x10); //
HM2057_write_cmos_sensor(0x03B6,0x00); //
HM2057_write_cmos_sensor(0x03B7,0xEA); //
HM2057_write_cmos_sensor(0x03B8,0x00); //
HM2057_write_cmos_sensor(0x03B9,0x3A); //
HM2057_write_cmos_sensor(0x03BA,0x01); //
HM2057_write_cmos_sensor(0x03BB,0x9F); //; enable 5x5 window
HM2057_write_cmos_sensor(0x03BC,0xCF); //
HM2057_write_cmos_sensor(0x03BD,0xE7); //
HM2057_write_cmos_sensor(0x03BE,0xF3); //
HM2057_write_cmos_sensor(0x03BF,0x01); //
HM2057_write_cmos_sensor(0x03D0,0xF8); //; AE NSMode YTh
HM2057_write_cmos_sensor(0x03E0,0x04); //; weight
HM2057_write_cmos_sensor(0x03E1,0x01); //
HM2057_write_cmos_sensor(0x03E2,0x04); //
HM2057_write_cmos_sensor(0x03E4,0x10); //
HM2057_write_cmos_sensor(0x03E5,0x12); //
HM2057_write_cmos_sensor(0x03E6,0x00); //
HM2057_write_cmos_sensor(0x03E8,0x21); //
HM2057_write_cmos_sensor(0x03E9,0x23); //
HM2057_write_cmos_sensor(0x03EA,0x01); //
HM2057_write_cmos_sensor(0x03EC,0x21); //
HM2057_write_cmos_sensor(0x03ED,0x23); //
HM2057_write_cmos_sensor(0x03EE,0x01); //
HM2057_write_cmos_sensor(0x03F0,0x20); //
HM2057_write_cmos_sensor(0x03F1,0x22); //
HM2057_write_cmos_sensor(0x03F2,0x00); //
HM2057_write_cmos_sensor(0x0420,0x84); //; Digital Gain offset
HM2057_write_cmos_sensor(0x0421,0x00); //
HM2057_write_cmos_sensor(0x0422,0x00); //
HM2057_write_cmos_sensor(0x0423,0x83); //
HM2057_write_cmos_sensor(0x0430,0x08); //; ABLC
HM2057_write_cmos_sensor(0x0431,0x28); //
HM2057_write_cmos_sensor(0x0432,0x10); //
HM2057_write_cmos_sensor(0x0433,0x08); //
HM2057_write_cmos_sensor(0x0435,0x0C); //
HM2057_write_cmos_sensor(0x0450,0xFF); //
HM2057_write_cmos_sensor(0x0451,0xE8); //
HM2057_write_cmos_sensor(0x0452,0xC4); //
HM2057_write_cmos_sensor(0x0453,0x88); //
HM2057_write_cmos_sensor(0x0454,0x00); //
HM2057_write_cmos_sensor(0x0458,0x98); //
HM2057_write_cmos_sensor(0x0459,0x03); //
HM2057_write_cmos_sensor(0x045A,0x00); //
HM2057_write_cmos_sensor(0x045B,0x28); //
HM2057_write_cmos_sensor(0x045C,0x00); //
HM2057_write_cmos_sensor(0x045D,0x68); //
HM2057_write_cmos_sensor(0x0466,0x14); //
HM2057_write_cmos_sensor(0x047A,0x00); //; ELOFFNRB
HM2057_write_cmos_sensor(0x047B,0x00); //; ELOFFNRY
HM2057_write_cmos_sensor(0x0480,0x58); //
HM2057_write_cmos_sensor(0x0481,0x06); //
HM2057_write_cmos_sensor(0x0482,0x0C); //
HM2057_write_cmos_sensor(0x04B0,0x50); //; Contrast
HM2057_write_cmos_sensor(0x04B6,0x30); //
HM2057_write_cmos_sensor(0x04B9,0x10); //
HM2057_write_cmos_sensor(0x04B3,0x10); //
HM2057_write_cmos_sensor(0x04B1,0x8E); //
HM2057_write_cmos_sensor(0x04B4,0x20); //
HM2057_write_cmos_sensor(0x0540,0x00); //;
HM2057_write_cmos_sensor(0x0541,0x9D); //; 60Hz Flicker
HM2057_write_cmos_sensor(0x0542,0x00); //;

HM2057_write_cmos_sensor(0x0580,0x01); //; Blur str sigma
HM2057_write_cmos_sensor(0x0581,0x0F); //; Blur str sigma ALPHA
HM2057_write_cmos_sensor(0x0582,0x04); //; Blur str sigma OD
HM2057_write_cmos_sensor(0x0594,0x00); //; UV Gray TH
HM2057_write_cmos_sensor(0x0595,0x04); //; UV Gray TH Alpha
HM2057_write_cmos_sensor(0x05A9,0x03); //
HM2057_write_cmos_sensor(0x05AA,0x40); //
HM2057_write_cmos_sensor(0x05AB,0x80); //
HM2057_write_cmos_sensor(0x05AC,0x0A); //
HM2057_write_cmos_sensor(0x05AD,0x10); //
HM2057_write_cmos_sensor(0x05AE,0x0C); //
HM2057_write_cmos_sensor(0x05AF,0x0C); //
HM2057_write_cmos_sensor(0x05B0,0x03); //
HM2057_write_cmos_sensor(0x05B1,0x03); //
HM2057_write_cmos_sensor(0x05B2,0x1C); //
HM2057_write_cmos_sensor(0x05B3,0x02); //
HM2057_write_cmos_sensor(0x05B4,0x00); //
HM2057_write_cmos_sensor(0x05B5,0x0C); //; BlurW
HM2057_write_cmos_sensor(0x05B8,0x80); //
HM2057_write_cmos_sensor(0x05B9,0x32); //
HM2057_write_cmos_sensor(0x05BA,0x00); //
HM2057_write_cmos_sensor(0x05BB,0x80); //
HM2057_write_cmos_sensor(0x05BC,0x03); //
HM2057_write_cmos_sensor(0x05BD,0x00); //
HM2057_write_cmos_sensor(0x05BF,0x05); //
HM2057_write_cmos_sensor(0x05C0,0x10); //; BlurW LowLight
HM2057_write_cmos_sensor(0x05C3,0x00); //
HM2057_write_cmos_sensor(0x05C4,0x0C); //; BlurW Outdoor
HM2057_write_cmos_sensor(0x05C5,0x20); //
HM2057_write_cmos_sensor(0x05C7,0x01); //
HM2057_write_cmos_sensor(0x05C8,0x14); //
HM2057_write_cmos_sensor(0x05C9,0x54); //
HM2057_write_cmos_sensor(0x05CA,0x14); //
HM2057_write_cmos_sensor(0x05CB,0xE0); //
HM2057_write_cmos_sensor(0x05CC,0x20); //
HM2057_write_cmos_sensor(0x05CD,0x00); //
HM2057_write_cmos_sensor(0x05CE,0x08); //
HM2057_write_cmos_sensor(0x05CF,0x60); //
HM2057_write_cmos_sensor(0x05D0,0x10); //
HM2057_write_cmos_sensor(0x05D1,0x05); //
HM2057_write_cmos_sensor(0x05D2,0x03); //
HM2057_write_cmos_sensor(0x05D4,0x00); //
HM2057_write_cmos_sensor(0x05D5,0x05); //
HM2057_write_cmos_sensor(0x05D6,0x05); //
HM2057_write_cmos_sensor(0x05D7,0x05); //
HM2057_write_cmos_sensor(0x05D8,0x08); //
HM2057_write_cmos_sensor(0x05DC,0x0C); //
HM2057_write_cmos_sensor(0x05D9,0x00); //
HM2057_write_cmos_sensor(0x05DB,0x00); //
HM2057_write_cmos_sensor(0x05DD,0x0F); //
HM2057_write_cmos_sensor(0x05DE,0x00); //
HM2057_write_cmos_sensor(0x05DF,0x0A); //
HM2057_write_cmos_sensor(0x05E0,0xA0); //; Scaler
HM2057_write_cmos_sensor(0x05E1,0x00); //
HM2057_write_cmos_sensor(0x05E2,0xA0); //
HM2057_write_cmos_sensor(0x05E3,0x00); //
HM2057_write_cmos_sensor(0x05E4,0x04); //; Windowing
HM2057_write_cmos_sensor(0x05E5,0x00); //
HM2057_write_cmos_sensor(0x05E6,0x83); //
HM2057_write_cmos_sensor(0x05E7,0x02); //
HM2057_write_cmos_sensor(0x05E8,0x06); //
HM2057_write_cmos_sensor(0x05E9,0x00); //
HM2057_write_cmos_sensor(0x05EA,0xE5); //
HM2057_write_cmos_sensor(0x05EB,0x01); //
HM2057_write_cmos_sensor(0x0660,0x04); //
HM2057_write_cmos_sensor(0x0661,0x16); //
HM2057_write_cmos_sensor(0x0662,0x04); //
HM2057_write_cmos_sensor(0x0663,0x28); //
HM2057_write_cmos_sensor(0x0664,0x04); //
HM2057_write_cmos_sensor(0x0665,0x18); //
HM2057_write_cmos_sensor(0x0666,0x04); //
HM2057_write_cmos_sensor(0x0667,0x21); //
HM2057_write_cmos_sensor(0x0668,0x04); //
HM2057_write_cmos_sensor(0x0669,0x0C); //
HM2057_write_cmos_sensor(0x066A,0x04); //
HM2057_write_cmos_sensor(0x066B,0x25); //
HM2057_write_cmos_sensor(0x066C,0x00); //
HM2057_write_cmos_sensor(0x066D,0x12); //
HM2057_write_cmos_sensor(0x066E,0x00); //
HM2057_write_cmos_sensor(0x066F,0x80); //
HM2057_write_cmos_sensor(0x0670,0x00); //
HM2057_write_cmos_sensor(0x0671,0x0A); //
HM2057_write_cmos_sensor(0x0672,0x04); //
HM2057_write_cmos_sensor(0x0673,0x1D); //
HM2057_write_cmos_sensor(0x0674,0x04); //
HM2057_write_cmos_sensor(0x0675,0x1D); //
HM2057_write_cmos_sensor(0x0676,0x00); //
HM2057_write_cmos_sensor(0x0677,0x7E); //
HM2057_write_cmos_sensor(0x0678,0x01); //
HM2057_write_cmos_sensor(0x0679,0x47); //
HM2057_write_cmos_sensor(0x067A,0x00); //
HM2057_write_cmos_sensor(0x067B,0x73); //
HM2057_write_cmos_sensor(0x067C,0x04); //
HM2057_write_cmos_sensor(0x067D,0x14); //
HM2057_write_cmos_sensor(0x067E,0x04); //
HM2057_write_cmos_sensor(0x067F,0x28); //
HM2057_write_cmos_sensor(0x0680,0x00); //
HM2057_write_cmos_sensor(0x0681,0x22); //
HM2057_write_cmos_sensor(0x0682,0x00); //
HM2057_write_cmos_sensor(0x0683,0xA5); //
HM2057_write_cmos_sensor(0x0684,0x00); //
HM2057_write_cmos_sensor(0x0685,0x1E); //
HM2057_write_cmos_sensor(0x0686,0x04); //
HM2057_write_cmos_sensor(0x0687,0x1D); //
HM2057_write_cmos_sensor(0x0688,0x04); //
HM2057_write_cmos_sensor(0x0689,0x19); //
HM2057_write_cmos_sensor(0x068A,0x04); //
HM2057_write_cmos_sensor(0x068B,0x21); //
HM2057_write_cmos_sensor(0x068C,0x04); //
HM2057_write_cmos_sensor(0x068D,0x0A); //
HM2057_write_cmos_sensor(0x068E,0x04); //
HM2057_write_cmos_sensor(0x068F,0x25); //
HM2057_write_cmos_sensor(0x0690,0x04); //
HM2057_write_cmos_sensor(0x0691,0x15); //
HM2057_write_cmos_sensor(0x0698,0x20); //
HM2057_write_cmos_sensor(0x0699,0x20); //
HM2057_write_cmos_sensor(0x069A,0x01); //
HM2057_write_cmos_sensor(0x069C,0x22); //
HM2057_write_cmos_sensor(0x069D,0x10); //
HM2057_write_cmos_sensor(0x069E,0x10); //
HM2057_write_cmos_sensor(0x069F,0x08); //
HM2057_write_cmos_sensor(0x0006,0x03); //;VGA
HM2057_write_cmos_sensor(0x000D,0x11); //;
HM2057_write_cmos_sensor(0x000E,0x11); //;
HM2057_write_cmos_sensor(0x0012,0x1C); //;
HM2057_write_cmos_sensor(0x0013,0x01); //;
HM2057_write_cmos_sensor(0x002A,0x1F); //; PLL Setting For 30fps under VGA @ 24MHz MCLK, MIPI 384bps
HM2057_write_cmos_sensor(0x0071,0xFF); //;
HM2057_write_cmos_sensor(0x0082,0xA2); //;
HM2057_write_cmos_sensor(0x011F,0xF7); //;
HM2057_write_cmos_sensor(0x0125,0xFF); //;
HM2057_write_cmos_sensor(0x0126,0x70); //;
HM2057_write_cmos_sensor(0x0131,0xBD); //;
HM2057_write_cmos_sensor(0x0144,0x06); //; Sort bpc hot pixel ratio
HM2057_write_cmos_sensor(0x0190,0x80); //; A11 BPC Strength[7:3], cluster correct P11[0]P12[1]P13[2]
HM2057_write_cmos_sensor(0x0192,0x48); //; A13 Strength[4:0],hot pixel detect for cluster[6]
HM2057_write_cmos_sensor(0x038F,0x09); //; Max INTG
HM2057_write_cmos_sensor(0x0390,0xD0); //;
HM2057_write_cmos_sensor(0x0541,0x9D); //; 60Hz Flicker

HM2057_write_cmos_sensor(0x05E0,0xA0); //; Scaler
HM2057_write_cmos_sensor(0x05E1,0x00); //;
HM2057_write_cmos_sensor(0x05E2,0xA0); //;
HM2057_write_cmos_sensor(0x05E3,0x00); //;
HM2057_write_cmos_sensor(0x05E4,0x04); //; Windowing
HM2057_write_cmos_sensor(0x05E5,0x00); //;
HM2057_write_cmos_sensor(0x05E6,0x83); //;
HM2057_write_cmos_sensor(0x05E7,0x02); //;
HM2057_write_cmos_sensor(0x05E8,0x06); //;
HM2057_write_cmos_sensor(0x05E9,0x00); //;
HM2057_write_cmos_sensor(0x05EA,0xE5); //;
HM2057_write_cmos_sensor(0x05EB,0x01); //;
HM2057_write_cmos_sensor(0x0006,0x03);//	;
HM2057_write_cmos_sensor(0x000D,0x01);//	;
HM2057_write_cmos_sensor(0x000E,0x11);//	;
HM2057_write_cmos_sensor(0x0012,0x08);//	;
HM2057_write_cmos_sensor(0x0013,0x00);//	;
HM2057_write_cmos_sensor(0x0025,0x80);//	; PLL off input=24M,outpu=24M
HM2057_write_cmos_sensor(0x0026,0x87);//	
HM2057_write_cmos_sensor(0x002A,0x1F);//	; If PLL on,input 24M,output 48M
HM2057_write_cmos_sensor(0x002B,0x00);//	
HM2057_write_cmos_sensor(0x0071,0x99);//	;
HM2057_write_cmos_sensor(0x0082,0xE2);//	;
HM2057_write_cmos_sensor(0x011F,0xF7);//	;
HM2057_write_cmos_sensor(0x0125,0xDF);//	;
HM2057_write_cmos_sensor(0x0126,0x70);//	;
HM2057_write_cmos_sensor(0x0131,0xBC);//	;
HM2057_write_cmos_sensor(0x0144,0x04);//	; Sort bpc hot pixel ratio
HM2057_write_cmos_sensor(0x0190,0x87);//	; A11 BPC Strength[7:3], cluster correct P11[0]P12[1]P13[2]
HM2057_write_cmos_sensor(0x0192,0x50);//	; A13 Strength[4:0],hot pixel detect for cluster[6]
HM2057_write_cmos_sensor(0x038F,0x04);//	; Max INTG
HM2057_write_cmos_sensor(0x0390,0xC0);//	;
HM2057_write_cmos_sensor(0x0541,0x58);//	; 60Hz Flicker
HM2057_write_cmos_sensor(0x0543,0x6b);//	; 50Hz Flicker 6c
HM2057_write_cmos_sensor(0x05E4,0x05);//	; Windowing
HM2057_write_cmos_sensor(0x05E5,0x00);//	;
HM2057_write_cmos_sensor(0x05E6,0x24);//	;
HM2057_write_cmos_sensor(0x05E7,0x03);//	;
HM2057_write_cmos_sensor(0x05E8,0x08);//	;
HM2057_write_cmos_sensor(0x05E9,0x00);//	;
HM2057_write_cmos_sensor(0x05EA,0x5F);//	;
HM2057_write_cmos_sensor(0x05EB,0x02);//	;


HM2057_write_cmos_sensor(0x0000,0x01); //

HM2057_write_cmos_sensor(0x0100,0x01); //
HM2057_write_cmos_sensor(0x0101,0x01); //
HM2057_write_cmos_sensor(0x0005,0x01); //; Turn on rolling shutter
	

} /* HM2057_YUV_sensor_initial_setting */


/*****************************************************************************/
/* Windows Mobile Sensor Interface */
/*****************************************************************************/
/*************************************************************************
* FUNCTION
*	HM2057Open
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
UINT32 HM2057Open(void)
{
	printk("%s\n", __func__);
	volatile signed char i;
	kal_uint16 sensor_id=0;

	zoom_factor = 0; 
	HM2057_write_cmos_sensor(0x0022, 0x01);
  Sleep(10);


	//  Read sensor ID to adjust I2C is OK?
	for(i=0;i<3;i++)
	{
		sensor_id = (HM2057_read_cmos_sensor(0x0001) << 8) | HM2057_read_cmos_sensor(0x0002);
		printk("%s sensor_id=%x\n", __func__, sensor_id);
		if(sensor_id == HM2057_SENSOR_ID)
		{
				HM2057_YUV_sensor_initial_setting();
	      printk("[HM2057] HM2057Open. \n");
	      return ERROR_NONE;
		}
	}
	
	return ERROR_SENSOR_CONNECT_FAIL;
}	

//==========================================================

void HM2057_H_V_Switch(kal_uint8 direction)
{
	printk("%s\n", __func__);
	switch(direction) 
	{
		case 1:  // normal
	    HM2057_write_cmos_sensor(0x0006, 0x02);
		  break;
		case 2:  // IMAGE_H_MIRROR
	    HM2057_write_cmos_sensor(0x0006, 0x03);
		  break;		  
		case 3:  // IMAGE_V_MIRROR
	    HM2057_write_cmos_sensor(0x0006, 0x00);
  	  break;	
		case 4:  // IMAGE_HV_MIRROR
	    HM2057_write_cmos_sensor(0x0006, 0x01);
		  break;		    
	} 
}








/*************************************************************************
* FUNCTION
*	HM2057_GetPeriod
*
* DESCRIPTION
*	This function return the image width and height of image sensor.
*
* PARAMETERS
*	*pixel_number : address pointer of pixel numbers in one period of HSYNC
*  *line_number : address pointer of line numbers in one period of VSYNC
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void HM2057_GetPeriod(kal_uint16 *pixel_number, kal_uint16 *line_number)
{
	printk("%s\n", __func__);
	*pixel_number=HM2057_PV_PERIOD_PIXEL_NUMS;	/* pixel numbers in one period of HSYNC */
	*line_number=HM2057_PV_PERIOD_LINE_NUMS;		/* line numbers in one period of VSYNC */
}	/* HM2057_GetPeriod */




/*************************************************************************
* FUNCTION
*	HM2057Close
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
UINT32 HM2057GetSensorID(UINT32 *sensorID) 
{
	volatile signed char i;
	kal_uint16 sensor_id=0;
	SENSORDB("xieyang HM2057GetSensorID ");
	//  SENSORDB("xieyang in GPIO_CAMERA_CMPDN_PIN=%d,GPIO_CAMERA_CMPDN1_PIN=%d"
	//  	    , mt_get_gpio_out(GPIO_CAMERA_CMPDN_PIN),mt_get_gpio_out(GPIO_CAMERA_CMPDN1_PIN));
	for(i=0;i<3;i++)
	{
		sensor_id = (HM2057_read_cmos_sensor(0x0001) << 8) | HM2057_read_cmos_sensor(0x0002);
		printk("%s sensor_id=%x\n", __func__, sensor_id);
		if(sensor_id == HM2057_SENSOR_ID)
		{
			return ERROR_NONE;
		}
	}
	
	return ERROR_SENSOR_CONNECT_FAIL;
}

UINT32 HM2057Close(void)
{
	printk("%s\n", __func__);
//	CISModulePowerOn(FALSE);


	return ERROR_NONE;
}	/* HM2057Close() */

/*************************************************************************
* FUNCTION
*	HM2057Preview
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
UINT32 HM2057Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{

	printk("%s\n", __func__);
	kal_uint8 iTemp, temp_AE_reg, temp_AWB_reg;
    kal_uint16 iDummyPixels = 0, iDummyLines = 0, iStartX = 0, iStartY = 0;
     
    HM2057_sensor_cap_state = KAL_FALSE;
	HM2057_write_cmos_sensor(0x0380,0xFF);
	HM2057_write_cmos_sensor(0x000D,0x11);
	HM2057_write_cmos_sensor(0x000E,0x11);
	HM2057_write_cmos_sensor(0x011F,0xf7);   //80
	HM2057_write_cmos_sensor(0x0125,0xDF);
	HM2057_write_cmos_sensor(0x0126,0x70);
	HM2057_write_cmos_sensor(0x0131,0xbc);
	HM2057_write_cmos_sensor(0x0366,0x20);// MinCTFCount, 08=20/4
	HM2057_write_cmos_sensor(0x0433,0x08);// ABLC LPoint, 10=40/4
	HM2057_write_cmos_sensor(0x0435,0x0c);// ABLC UPoint, 14=50/4
	HM2057_write_cmos_sensor(0x05E4,0x05);// Windowing
	HM2057_write_cmos_sensor(0x05E5,0x00);
	HM2057_write_cmos_sensor(0x05E6,0x24);
	HM2057_write_cmos_sensor(0x05E7,0x03);
	HM2057_write_cmos_sensor(0x05E8,0x07);
	HM2057_write_cmos_sensor(0x05E9,0x00);
	HM2057_write_cmos_sensor(0x05EA,0x5E);
	HM2057_write_cmos_sensor(0x05EB,0x02);

	//Start
	HM2057_write_cmos_sensor(0x0000,0x01); 
	HM2057_write_cmos_sensor(0x0100,0x01); 
	HM2057_write_cmos_sensor(0x0101,0x01); 
	HM2057_write_cmos_sensor(0x0005,0x01); //Turn on rolling shutter

    HM2057_H_V_Switch(3);

	if (CapState)
    {
        HM2057_write_shutter(Shutter);
    }
	HM2057_set_AE_mode(KAL_TRUE);
	CapState = KAL_FALSE;
    image_window->GrabStartX = 1;//iStartX;
    image_window->GrabStartY = 1;//iStartY;
    image_window->ExposureWindowWidth = HM2057_IMAGE_SENSOR_PV_WIDTH ;
    image_window->ExposureWindowHeight = HM2057_IMAGE_SENSOR_PV_HEIGHT;
    
    HM2057_DELAY_AFTER_PREVIEW = 1;

	// copy sensor_config_data
	memcpy(&HM2057SensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
  	return ERROR_NONE;
}	/* HM2057Preview() */




UINT32 HM2057Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{

	printk("%s\n", __func__);
    volatile kal_uint32 shutter = HM2057_exposure_lines, temp_reg;
    kal_uint8 temp_AE_reg, temp;
    kal_uint16 AE_setting_delay = 0;

    HM2057_sensor_cap_state = KAL_TRUE;
 
	HM2057_set_AE_mode(KAL_FALSE);

    Shutter = HM2057_read_shutter();
	
    if ((image_window->ImageTargetWidth<=HM2057_IMAGE_SENSOR_PV_WIDTH)&&
        (image_window->ImageTargetHeight<=HM2057_IMAGE_SENSOR_PV_HEIGHT))
    {    /* Less than PV Mode */


        HM2057_gPVmode=KAL_TRUE;

        HM2057_dummy_pixels = 0;
        HM2057_dummy_lines = 0;

        HM2057_capture_pclk_in_M = HM2057_preview_pclk_in_M;   //Don't change the clk
   
        
        
    image_window->GrabStartX = HM2057_PV_GRAB_START_X;//iStartX;
    image_window->GrabStartY = HM2057_PV_GRAB_START_Y;//iStartY;
    image_window->ExposureWindowWidth = HM2057_IMAGE_SENSOR_PV_WIDTH ;
    image_window->ExposureWindowHeight = HM2057_IMAGE_SENSOR_PV_HEIGHT;
		

    }
    else 
    {    
		if (CapState)
		{
			HM2057_write_shutter(Shutter);
		}
		else
		{
	HM2057_write_cmos_sensor(0x0380,0xfe);
			shutter = Shutter / 2;
			HM2057_write_shutter(shutter);	
		}
    	 /* Capature mode parameters */
	HM2057_write_cmos_sensor(0x000D,0x00);
	HM2057_write_cmos_sensor(0x000E,0x00);
	HM2057_write_cmos_sensor(0x011F,0x88);
	HM2057_write_cmos_sensor(0x0125,0xDF);
	HM2057_write_cmos_sensor(0x0126,0x70);
	HM2057_write_cmos_sensor(0x0131,0xAC);
	HM2057_write_cmos_sensor(0x0366,0x20);// MinCTFCount, 08=20/4
	HM2057_write_cmos_sensor(0x0433,0x40);// ABLC LPoint, 10=40/4
	HM2057_write_cmos_sensor(0x0435,0x50);// ABLC UPoint, 14=50/4
	
	HM2057_write_cmos_sensor(0x05E4,0x0a);// Windowing
	HM2057_write_cmos_sensor(0x05E5,0x00);
	HM2057_write_cmos_sensor(0x05E6,0x49);
	HM2057_write_cmos_sensor(0x05E7,0x06);
	HM2057_write_cmos_sensor(0x05E8,0x0a);
	HM2057_write_cmos_sensor(0x05E9,0x00);
	HM2057_write_cmos_sensor(0x05EA,0xb9);
	HM2057_write_cmos_sensor(0x05EB,0x04);
	HM2057_write_cmos_sensor(0x0000,0x01);
	HM2057_write_cmos_sensor(0x0100,0x01);
	HM2057_write_cmos_sensor(0x0101,0x01);
	
		CapState = KAL_TRUE;
		HM2057_gPVmode = KAL_FALSE;

		HM2057_Capture_Dummy_Pixels = HM2057_dummy_pixels ;
		HM2057_Capture_Dummy_Lines = HM2057_dummy_lines;


		image_window->GrabStartX = 1;//iStartX;
		image_window->GrabStartY = 1;//iStartY;
		image_window->ExposureWindowWidth=HM2057_IMAGE_SENSOR_FULL_HEIGHT ;
		image_window->ExposureWindowHeight=HM2057_IMAGE_SENSOR_FULL_HEIGHT;
	}

  

    HM2057_DELAY_AFTER_PREVIEW = 4;  // 2

	// copy sensor_config_data
	memcpy(&HM2057SensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
       
	mdelay(500);
	return ERROR_NONE;
}	/* HM2057Capture() */

UINT32 HM2057GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
	printk("%s\n", __func__);
	pSensorResolution->SensorFullWidth=HM2057_FULL_GRAB_WIDTH;  //modify by yanxu
	pSensorResolution->SensorFullHeight=HM2057_FULL_GRAB_HEIGHT;
	pSensorResolution->SensorPreviewWidth=HM2057_PV_GRAB_WIDTH ;
	pSensorResolution->SensorPreviewHeight=HM2057_PV_GRAB_HEIGHT;
        pSensorResolution->SensorVideoWidth=HM2057_VIDEO_GRAB_WIDTH ;
	pSensorResolution->SensorVideoHeight=HM2057_VIDEO_GRAB_HEIGHT;

	return ERROR_NONE;
}	/* HM2057GetResolution() */

UINT32 HM2057GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
					  MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
					  
{
	pSensorInfo->SensorPreviewResolutionX=HM2057_PV_GRAB_WIDTH;
	pSensorInfo->SensorPreviewResolutionY=HM2057_PV_GRAB_HEIGHT;
	pSensorInfo->SensorFullResolutionX=HM2057_FULL_GRAB_WIDTH;
	pSensorInfo->SensorFullResolutionY=HM2057_FULL_GRAB_HEIGHT;

	pSensorInfo->SensorCameraPreviewFrameRate=30;
	pSensorInfo->SensorVideoFrameRate=30;
	pSensorInfo->SensorStillCaptureFrameRate=10;
	pSensorInfo->SensorWebCamCaptureFrameRate=15;
	pSensorInfo->SensorResetActiveHigh=FALSE;
	pSensorInfo->SensorResetDelayCount=1;

	pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_VYUY;//SENSOR_OUTPUT_FORMAT_YUYV;//SENSOR_OUTPUT_FORMAT_YUYV;
	pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;	
	pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;

	pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorInterruptDelayLines = 1;
	pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_PARALLEL;

	pSensorInfo->CaptureDelayFrame = 1; //2
	pSensorInfo->PreviewDelayFrame = 3; //3
	pSensorInfo->VideoDelayFrame = 20; 
	pSensorInfo->SensorMasterClockSwitch = 0; 
    pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;   	
	
	switch (ScenarioId)
	{
	
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		//case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
		pSensorInfo->SensorClockFreq=24;
		pSensorInfo->SensorClockDividCount=	3;
		pSensorInfo->SensorClockRisingCount= 0;
		pSensorInfo->SensorClockFallingCount= 2;
		pSensorInfo->SensorPixelClockCount= 3;
		pSensorInfo->SensorDataLatchCount= 2;
                /*ergate-004*/
		pSensorInfo->SensorGrabStartX = HM2057_PV_GRAB_START_X;//0; 
		pSensorInfo->SensorGrabStartY = HM2057_PV_GRAB_START_Y;//0; 
		printk("MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4");
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		//case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
		pSensorInfo->SensorClockFreq=24;
		pSensorInfo->SensorClockDividCount=	3;
		pSensorInfo->SensorClockRisingCount= 0;
		pSensorInfo->SensorClockFallingCount= 2;
		pSensorInfo->SensorPixelClockCount= 3;
		pSensorInfo->SensorDataLatchCount= 2;
                /*ergate-004*/
		pSensorInfo->SensorGrabStartX = HM2057_FULL_GRAB_START_X;//0; 
		pSensorInfo->SensorGrabStartY = HM2057_FULL_GRAB_START_Y;//0;
		printk("MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM");
		break;
		default:
		pSensorInfo->SensorClockFreq=24;
		pSensorInfo->SensorClockDividCount=3;
		pSensorInfo->SensorClockRisingCount=0;
		pSensorInfo->SensorClockFallingCount=2;
		pSensorInfo->SensorPixelClockCount=3;
		pSensorInfo->SensorDataLatchCount=2;
                /*ergate-004*/
		pSensorInfo->SensorGrabStartX = HM2057_PV_GRAB_START_X;//0; 
		pSensorInfo->SensorGrabStartY = HM2057_PV_GRAB_START_Y;//0;
		printk("jinyunshan default");
		break;
	}
	//HM2057_PixelClockDivider=pSensorInfo->SensorPixelClockCount;
	memcpy(pSensorConfigData, &HM2057SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	return ERROR_NONE;
}	/* HM2057GetInfo() */


UINT32 HM2057Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	printk("%s\n", __func__);
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		//case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
			HM2057Preview(pImageWindow, pSensorConfigData);
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		//case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
			HM2057Capture(pImageWindow, pSensorConfigData);
		break;
		default:
		    break; 
	}
	return TRUE;
}	/* HM2057Control() */

/* [TC] YUV sensor */	
#if WINMO_USE
void HM2057Query(PMSDK_FEATURE_INFO_STRUCT pSensorFeatureInfo)
{
	MSDK_FEATURE_TYPE_RANGE_STRUCT *pFeatureRange;
	MSDK_FEATURE_TYPE_MULTI_SELECTION_STRUCT *pFeatureMultiSelection;
	switch (pSensorFeatureInfo->FeatureId)
	{
		case ISP_FEATURE_DSC_MODE:
			pSensorFeatureInfo->FeatureType = MSDK_FEATURE_TYPE_MULTI_SELECTION;
			pSensorFeatureInfo->FeatureSupported = (UINT8)(MSDK_SET_GET_FEATURE_SUPPORTED|MSDK_QUERY_CAMERA_SUPPORTED);
			pFeatureMultiSelection = (PMSDK_FEATURE_TYPE_MULTI_SELECTION_STRUCT)(&pSensorFeatureInfo->FeatureInformation.FeatureMultiSelection);
			pFeatureMultiSelection->TotalSelection = CAM_NO_OF_SCENE_MODE_MAX;
			pFeatureMultiSelection->DefaultSelection = CAM_AUTO_DSC_MODE;
			pFeatureMultiSelection->SupportedSelection = 
				(CAMERA_FEATURE_SUPPORT(CAM_AUTO_DSC_MODE)|
				CAMERA_FEATURE_SUPPORT(CAM_NIGHTSCENE_MODE));			
		break;
		case ISP_FEATURE_WHITEBALANCE:
			pSensorFeatureInfo->FeatureType = MSDK_FEATURE_TYPE_MULTI_SELECTION;
			pSensorFeatureInfo->FeatureSupported = (UINT8)(MSDK_SET_GET_FEATURE_SUPPORTED|MSDK_QUERY_CAMERA_VIDEO_SUPPORTED);
			pFeatureMultiSelection = (PMSDK_FEATURE_TYPE_MULTI_SELECTION_STRUCT)(&pSensorFeatureInfo->FeatureInformation.FeatureMultiSelection);
			pFeatureMultiSelection->TotalSelection = CAM_NO_OF_WB;
			pFeatureMultiSelection->DefaultSelection = CAM_WB_AUTO;
			pFeatureMultiSelection->SupportedSelection = 
				(CAMERA_FEATURE_SUPPORT(CAM_WB_AUTO)|
				CAMERA_FEATURE_SUPPORT(CAM_WB_CLOUD)|
				CAMERA_FEATURE_SUPPORT(CAM_WB_DAYLIGHT)|
				CAMERA_FEATURE_SUPPORT(CAM_WB_INCANDESCENCE)|
				CAMERA_FEATURE_SUPPORT(CAM_WB_TUNGSTEN)|
				CAMERA_FEATURE_SUPPORT(CAM_WB_FLUORESCENT));
		break;
		case ISP_FEATURE_IMAGE_EFFECT:
			pSensorFeatureInfo->FeatureType = MSDK_FEATURE_TYPE_MULTI_SELECTION;
			pSensorFeatureInfo->FeatureSupported = (UINT8)(MSDK_SET_GET_FEATURE_SUPPORTED|MSDK_QUERY_CAMERA_VIDEO_SUPPORTED);
			pFeatureMultiSelection = (PMSDK_FEATURE_TYPE_MULTI_SELECTION_STRUCT)(&pSensorFeatureInfo->FeatureInformation.FeatureMultiSelection);
			pFeatureMultiSelection->TotalSelection = CAM_NO_OF_EFFECT_ENC;
			pFeatureMultiSelection->DefaultSelection = CAM_EFFECT_ENC_NORMAL;
			pFeatureMultiSelection->SupportedSelection = 
				(CAMERA_FEATURE_SUPPORT(CAM_EFFECT_ENC_NORMAL)|
				CAMERA_FEATURE_SUPPORT(CAM_EFFECT_ENC_GRAYSCALE)|
				CAMERA_FEATURE_SUPPORT(CAM_EFFECT_ENC_COLORINV)|
				CAMERA_FEATURE_SUPPORT(CAM_EFFECT_ENC_SEPIAGREEN)|
				CAMERA_FEATURE_SUPPORT(CAM_EFFECT_ENC_SEPIABLUE)|
				CAMERA_FEATURE_SUPPORT(CAM_EFFECT_ENC_SEPIA));	
		break;
		case ISP_FEATURE_AE_METERING_MODE:
			pSensorFeatureInfo->FeatureSupported = MSDK_FEATURE_NOT_SUPPORTED;
		break;
		case ISP_FEATURE_BRIGHTNESS:
			pSensorFeatureInfo->FeatureType = MSDK_FEATURE_TYPE_RANGE;
			pSensorFeatureInfo->FeatureSupported = (UINT8)(MSDK_SET_GET_FEATURE_SUPPORTED|MSDK_QUERY_CAMERA_VIDEO_SUPPORTED);
			pFeatureRange = (PMSDK_FEATURE_TYPE_RANGE_STRUCT)(&pSensorFeatureInfo->FeatureInformation.FeatureRange);
			pFeatureRange->MinValue = CAM_EV_NEG_4_3;
			pFeatureRange->MaxValue = CAM_EV_POS_4_3;
			pFeatureRange->StepValue = CAMERA_FEATURE_ID_EV_STEP;
			pFeatureRange->DefaultValue = CAM_EV_ZERO;
		break;
		case ISP_FEATURE_BANDING_FREQ:
			pSensorFeatureInfo->FeatureType = MSDK_FEATURE_TYPE_MULTI_SELECTION;
			pSensorFeatureInfo->FeatureSupported = (UINT8)(MSDK_SET_GET_FEATURE_SUPPORTED|MSDK_QUERY_CAMERA_VIDEO_SUPPORTED);
			pFeatureMultiSelection = (PMSDK_FEATURE_TYPE_MULTI_SELECTION_STRUCT)(&pSensorFeatureInfo->FeatureInformation.FeatureMultiSelection);
			pFeatureMultiSelection->TotalSelection = CAM_NO_OF_BANDING;
			pFeatureMultiSelection->DefaultSelection = CAM_BANDING_50HZ;
			pFeatureMultiSelection->SupportedSelection = 
				(CAMERA_FEATURE_SUPPORT(CAM_BANDING_50HZ)|
				CAMERA_FEATURE_SUPPORT(CAM_BANDING_60HZ));
		break;
		case ISP_FEATURE_AF_OPERATION:
			pSensorFeatureInfo->FeatureSupported = MSDK_FEATURE_NOT_SUPPORTED;
		break;
		case ISP_FEATURE_AF_RANGE_CONTROL:
			pSensorFeatureInfo->FeatureSupported = MSDK_FEATURE_NOT_SUPPORTED;
		break;
		case ISP_FEATURE_FLASH:
			pSensorFeatureInfo->FeatureSupported = MSDK_FEATURE_NOT_SUPPORTED;			
		break;
		case ISP_FEATURE_VIDEO_SCENE_MODE:
			pSensorFeatureInfo->FeatureType = MSDK_FEATURE_TYPE_MULTI_SELECTION;
			pSensorFeatureInfo->FeatureSupported = (UINT8)(MSDK_SET_GET_FEATURE_SUPPORTED|MSDK_QUERY_VIDEO_SUPPORTED);
			pFeatureMultiSelection = (PMSDK_FEATURE_TYPE_MULTI_SELECTION_STRUCT)(&pSensorFeatureInfo->FeatureInformation.FeatureMultiSelection);
			pFeatureMultiSelection->TotalSelection = CAM_NO_OF_SCENE_MODE_MAX;
			pFeatureMultiSelection->DefaultSelection = CAM_VIDEO_AUTO_MODE;
			pFeatureMultiSelection->SupportedSelection = 
				(CAMERA_FEATURE_SUPPORT(CAM_VIDEO_AUTO_MODE)|
				CAMERA_FEATURE_SUPPORT(CAM_VIDEO_NIGHT_MODE));
		break;
		case ISP_FEATURE_ISO:
			pSensorFeatureInfo->FeatureSupported = MSDK_FEATURE_NOT_SUPPORTED;			
		break;
		default:
			pSensorFeatureInfo->FeatureSupported = MSDK_FEATURE_NOT_SUPPORTED;			
		break;
	}
}
#endif 

BOOL HM2057_set_param_wb(UINT16 para)
{
	printk("%s\n", __func__);
    kal_uint8  temp_reg;

     switch (para)
    {
    case AWB_MODE_AUTO:
	HM2057_write_cmos_sensor(0x0380, 0xFF);   // select Auto WB
	HM2057_write_cmos_sensor(0x0101, 0xFF);  
        break;
    case AWB_MODE_CLOUDY_DAYLIGHT: //cloudy
	HM2057_write_cmos_sensor(0x0380, 0xFD);  	//Disable AWB
	HM2057_write_cmos_sensor(0x032D, 0x70);
	HM2057_write_cmos_sensor(0x032E, 0x01); 	//Red
	HM2057_write_cmos_sensor(0x032F, 0x00);
	HM2057_write_cmos_sensor(0x0330, 0x01);		//Green
	HM2057_write_cmos_sensor(0x0331, 0x08);
	HM2057_write_cmos_sensor(0x0332, 0x01);		//Blue
	HM2057_write_cmos_sensor(0x0101, 0xFF);
        break;
    case AWB_MODE_DAYLIGHT: //sunny
	HM2057_write_cmos_sensor(0x0380, 0xFD);  	//Disable AWB
	HM2057_write_cmos_sensor(0x032D, 0x60);
	HM2057_write_cmos_sensor(0x032E, 0x01); 	//Red
	HM2057_write_cmos_sensor(0x032F, 0x00);
	HM2057_write_cmos_sensor(0x0330, 0x01);		//Green
	HM2057_write_cmos_sensor(0x0331, 0x20);
	HM2057_write_cmos_sensor(0x0332, 0x01);		//Blue
	HM2057_write_cmos_sensor(0x0101, 0xFF);
        break;
    case AWB_MODE_INCANDESCENT: //office
	HM2057_write_cmos_sensor(0x0380, 0xFD);  	//Disable AWB
	HM2057_write_cmos_sensor(0x032D, 0x00);
	HM2057_write_cmos_sensor(0x032E, 0x01); 	//Red
	HM2057_write_cmos_sensor(0x032F, 0x14);
	HM2057_write_cmos_sensor(0x0330, 0x01);		//Green
	HM2057_write_cmos_sensor(0x0331, 0xD6);
	HM2057_write_cmos_sensor(0x0332, 0x01);		//Blue
	HM2057_write_cmos_sensor(0x0101, 0xFF); 
        break;
    case AWB_MODE_TUNGSTEN: //home
	HM2057_write_cmos_sensor(0x0380, 0xFD);  	//Disable AWB
	HM2057_write_cmos_sensor(0x032D, 0x10);
	HM2057_write_cmos_sensor(0x032E, 0x01); 	//Red
	HM2057_write_cmos_sensor(0x032F, 0x00);
	HM2057_write_cmos_sensor(0x0330, 0x01);		//Green
	HM2057_write_cmos_sensor(0x0331, 0xA0);
	HM2057_write_cmos_sensor(0x0332, 0x01);		//Blue
	HM2057_write_cmos_sensor(0x0101, 0xFF);
        break;
    case AWB_MODE_FLUORESCENT:
	HM2057_write_cmos_sensor(0x0380, 0xFD);  	//Disable AWB
	HM2057_write_cmos_sensor(0x032D, 0x34);
	HM2057_write_cmos_sensor(0x032E, 0x01); 	//Red
	HM2057_write_cmos_sensor(0x032F, 0x00);
	HM2057_write_cmos_sensor(0x0330, 0x01);		//Green
	HM2057_write_cmos_sensor(0x0331, 0x92);
	HM2057_write_cmos_sensor(0x0332, 0x01);		//Blue
	HM2057_write_cmos_sensor(0x0101, 0xFF); 
        break; 
    default:
        return FALSE;
    }
	return TRUE;
} /* HM2057_set_param_wb */



BOOL HM2057_set_param_effect(UINT16 para)
{
	printk("%s\n", __func__);
   BOOL  ret = TRUE;
   //UINT8  temp_reg;
   //temp_reg=HM2057_read_cmos_sensor(0x3391);
    switch (para)
    {
    case MEFFECT_OFF:
	HM2057_write_cmos_sensor(0x0005, 0x00);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
	HM2057_write_cmos_sensor(0x0488, 0x10); //[0]:Image scense, [1]:Image Xor
	HM2057_write_cmos_sensor(0x0486, 0x08); //Hue, sin                       
	HM2057_write_cmos_sensor(0x0487, 0xFF); //Hue, cos
	//HM2057_write_cmos_sensor(0x0120, 0x36);
	HM2057_write_cmos_sensor(0x0005, 0x01);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
        break;
    case MEFFECT_SEPIA:
	HM2057_write_cmos_sensor(0x0005, 0x00);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
	HM2057_write_cmos_sensor(0x0486, 0x40); //Hue, sin						 
	HM2057_write_cmos_sensor(0x0487, 0xA0); //Hue, cos
	HM2057_write_cmos_sensor(0x0488, 0x11); //[0]:Image scense, [1]:Image Xor
	//HM2057_write_cmos_sensor(0x0120, 0x27);
	HM2057_write_cmos_sensor(0x0005, 0x01);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
        break;
    case MEFFECT_NEGATIVE:
	HM2057_write_cmos_sensor(0x0005, 0x00);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
	HM2057_write_cmos_sensor(0x0488, 0x12); //[0]:Image scense, [1]:Image Xor
	HM2057_write_cmos_sensor(0x0486, 0x00); //Hue, sin						 
	HM2057_write_cmos_sensor(0x0487, 0xFF); //Hue, cos
	//HM2057_write_cmos_sensor(0x0120, 0x37);
	HM2057_write_cmos_sensor(0x0005, 0x01);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
        break;
    case MEFFECT_SEPIAGREEN:
	HM2057_write_cmos_sensor(0x0005, 0x00);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
	HM2057_write_cmos_sensor(0x0486, 0x60); //Hue, sin						 
	HM2057_write_cmos_sensor(0x0487, 0x60); //Hue, cos
	HM2057_write_cmos_sensor(0x0488, 0x11); //[0]:Image scense, [1]:Image Xor
	//HM2057_write_cmos_sensor(0x0120, 0x27);
	HM2057_write_cmos_sensor(0x0005, 0x01);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
        break;
    case MEFFECT_SEPIABLUE:
	HM2057_write_cmos_sensor(0x0005, 0x00);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
	HM2057_write_cmos_sensor(0x0486, 0xB0); //Hue, sin						 
	HM2057_write_cmos_sensor(0x0487, 0x80); //Hue, cos
	HM2057_write_cmos_sensor(0x0488, 0x11); //[0]:Image scense, [1]:Image Xor
	//HM2057_write_cmos_sensor(0x0120, 0x27);
	HM2057_write_cmos_sensor(0x0005, 0x01);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
        break;
	case MEFFECT_MONO: //B&W
	HM2057_write_cmos_sensor(0x0005, 0x00);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
	HM2057_write_cmos_sensor(0x0486, 0x80); //Hue, sin						 
	HM2057_write_cmos_sensor(0x0487, 0x80); //Hue, cos
	HM2057_write_cmos_sensor(0x0488, 0x11); //[0]:Image scense, [1]:Image Xor
	//HM2057_write_cmos_sensor(0x0120, 0x27);
	HM2057_write_cmos_sensor(0x0005, 0x01);
	HM2057_write_cmos_sensor(0x0000, 0x01);
	HM2057_write_cmos_sensor(0x0100, 0xFF);
	HM2057_write_cmos_sensor(0x0101, 0xFF);
		break; 
    default:
        ret = FALSE;
    }
   
    return ret;

} /* HM2057_set_param_effect */



BOOL HM2057_set_param_exposure(UINT16 para)
{
	printk("%s\n", __func__);
  // HM2057_write_cmos_sensor(0x03,0x10);
	 //HM2057_write_cmos_sensor(0x12,(HM2057_read_cmos_sensor(0x12)|0x10));//make sure the Yoffset control is opened.

    switch (para)
    {
    case AE_EV_COMP_n13:
		HM2057_write_cmos_sensor(0x04C0,0xC0);
//		HM2057_write_cmos_sensor(0x038E,0x28);	// 20-28-30
//		HM2057_write_cmos_sensor(0x0381,0x30);
//		HM2057_write_cmos_sensor(0x0382,0x20);
		HM2057_write_cmos_sensor(0x0100,0xFF);
        break;
    case AE_EV_COMP_n10:
		HM2057_write_cmos_sensor(0x04C0,0xB0);
//		HM2057_write_cmos_sensor(0x038E,0x30);	// 28-30-38
//		HM2057_write_cmos_sensor(0x0381,0x38);
//		HM2057_write_cmos_sensor(0x0382,0x28);
		HM2057_write_cmos_sensor(0x0100,0xFF);
        break;
    case AE_EV_COMP_n07:
		HM2057_write_cmos_sensor(0x04C0,0xA0);
//		HM2057_write_cmos_sensor(0x038E,0x38);  // 30-38-40
//		HM2057_write_cmos_sensor(0x0381,0x40);
//		HM2057_write_cmos_sensor(0x0382,0x30);
		HM2057_write_cmos_sensor(0x0100,0xFF);
        break;
    case AE_EV_COMP_n03:
		HM2057_write_cmos_sensor(0x04C0,0x90);
//		HM2057_write_cmos_sensor(0x038E,0x40);	// 38-40-48
//		HM2057_write_cmos_sensor(0x0381,0x48);
//		HM2057_write_cmos_sensor(0x0382,0x38);
		HM2057_write_cmos_sensor(0x0100,0xFF); 
        break;
    case AE_EV_COMP_00:
		HM2057_write_cmos_sensor(0x04C0,0x00);
//		HM2057_write_cmos_sensor(0x038E,0x48);	// 40-48-50
//		HM2057_write_cmos_sensor(0x0381,0x50);
//		HM2057_write_cmos_sensor(0x0382,0x40);
		HM2057_write_cmos_sensor(0x0100,0xFF);
        break;
    case AE_EV_COMP_03:
		HM2057_write_cmos_sensor(0x04C0,0x10);	
//		HM2057_write_cmos_sensor(0x038E,0x50);	// 48-50-58
//		HM2057_write_cmos_sensor(0x0381,0x58);
//		HM2057_write_cmos_sensor(0x0382,0x48);
		HM2057_write_cmos_sensor(0x0100,0xFF); 
        break;
    case AE_EV_COMP_07:
		HM2057_write_cmos_sensor(0x04C0,0x20);	
//		HM2057_write_cmos_sensor(0x038E,0x58);	// 50-58-60
//		HM2057_write_cmos_sensor(0x0381,0x60);
//		HM2057_write_cmos_sensor(0x0382,0x50);
		HM2057_write_cmos_sensor(0x0100,0xFF); 
        break;
    case AE_EV_COMP_10:
		HM2057_write_cmos_sensor(0x04C0,0x30);	
//		HM2057_write_cmos_sensor(0x038E,0x60);	// 58-60-68
//		HM2057_write_cmos_sensor(0x0381,0x68);
//		HM2057_write_cmos_sensor(0x0382,0x58);
		HM2057_write_cmos_sensor(0x0100,0xFF);
        break;
    case AE_EV_COMP_13:
		HM2057_write_cmos_sensor(0x04C0,0x40);
//		HM2057_write_cmos_sensor(0x038E,0x68);	// 60-68-70
//		HM2057_write_cmos_sensor(0x0381,0x70);
//		HM2057_write_cmos_sensor(0x0382,0x60);
		HM2057_write_cmos_sensor(0x0100,0xFF);
        break;
    default:
        return FALSE;
    }

    return TRUE;
} /* HM2057_set_param_exposure */



UINT32 HM2057YUVSensorSetting(FEATURE_ID iCmd, UINT32 iPara)
{
	printk("%s\n", __func__);
//   if( HM2057_sensor_cap_state == KAL_TRUE)
//	   return TRUE;

	switch (iCmd) {
	case FID_SCENE_MODE:	    
//	    printk("Set Scene Mode:%d\n", iPara); 
	    if (iPara == SCENE_MODE_OFF)
	    {
	        HM2057_night_mode(0); 
	    }
	    else if (iPara == SCENE_MODE_NIGHTSCENE)
	    {
               HM2057_night_mode(1); 
	    }	    
	    break; 	    
	case FID_AWB_MODE:
//	    printk("Set AWB Mode:%d\n", iPara); 	    
           HM2057_set_param_wb(iPara);
	break;
	case FID_COLOR_EFFECT:
//	    printk("Set Color Effect:%d\n", iPara); 	    	    
           HM2057_set_param_effect(iPara);
	break;
	case FID_AE_EV:
#if WINMO_USE	    
	case ISP_FEATURE_EXPOSURE:
#endif 	    
//           printk("Set EV:%d\n", iPara); 	    	    
           HM2057_set_param_exposure(iPara);
	break;
	case FID_AE_FLICKER:
//           printk("Set Flicker:%d\n", iPara); 	    	    	    
           HM2057_set_param_banding(iPara);
	break;
        case FID_AE_SCENE_MODE: 
            if (iPara == AE_MODE_OFF) {
                HM2057_AE_ENABLE = KAL_FALSE; 
            }
            else {
                HM2057_AE_ENABLE = KAL_TRUE; 
	    }
            HM2057_set_AE_mode(HM2057_AE_ENABLE);
            break; 
	case FID_ZOOM_FACTOR:
	    zoom_factor = iPara; 
        break; 
	default:
	break;
	}
	return TRUE;
}   /* HM2057YUVSensorSetting */

UINT32 HM2057YUVSetVideoMode(UINT16 u2FrameRate)
{
	printk("%s\n", __func__);
    kal_uint8 iTemp;
    /* to fix VSYNC, to fix frame rate */
    //printk("Set YUV Video Mode \n");  
//    iTemp = HM2057_read_cmos_sensor(0x3014);
//    HM2057_write_cmos_sensor(0x3014, iTemp & 0xf7); //Disable night mode

    if (u2FrameRate == 30)
    {
 //       HM2057_write_cmos_sensor(0x302d, 0x00);
  //      HM2057_write_cmos_sensor(0x302e, 0x00);
    }
    else if (u2FrameRate == 15)       
    {
 //       HM2057_write_cmos_sensor(0x300e, 0x34);
//        HM2057_write_cmos_sensor(0x302A, HM2057_VIDEO_15FPS_FRAME_LENGTH>>8);  /*  15fps*/
//        HM2057_write_cmos_sensor(0x302B, HM2057_VIDEO_15FPS_FRAME_LENGTH&0xFF);
                
        // clear extra exposure line
 //       HM2057_write_cmos_sensor(0x302d, 0x00);
//        HM2057_write_cmos_sensor(0x302e, 0x00);   
    }
    else 
    {
        printk("Wrong frame rate setting \n");
    }
    HM2057_VEDIO_encode_mode = KAL_TRUE; 
        
    return TRUE;
}

UINT32 HM2057FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,
							 UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
{
	printk("%s\n", __func__);

	UINT16 *pFeatureReturnPara16=(UINT16 *) pFeaturePara;
	UINT16 *pFeatureData16=(UINT16 *) pFeaturePara;
	UINT32 *pFeatureReturnPara32=(UINT32 *) pFeaturePara;
	UINT32 *pFeatureData32=(UINT32 *) pFeaturePara;
	MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData=(MSDK_SENSOR_CONFIG_STRUCT *) pFeaturePara;
	MSDK_SENSOR_REG_INFO_STRUCT *pSensorRegData=(MSDK_SENSOR_REG_INFO_STRUCT *) pFeaturePara;

#if WINMO_USE	
	PMSDK_FEATURE_INFO_STRUCT pSensorFeatureInfo=(PMSDK_FEATURE_INFO_STRUCT) pFeaturePara;
#endif 

	switch (FeatureId)
	{
		case SENSOR_FEATURE_GET_RESOLUTION:
			*pFeatureReturnPara16++=HM2057_IMAGE_SENSOR_FULL_WIDTH;
			*pFeatureReturnPara16=HM2057_IMAGE_SENSOR_FULL_HEIGHT;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_GET_PERIOD:
			*pFeatureReturnPara16++=HM2057_PV_PERIOD_PIXEL_NUMS+HM2057_PV_dummy_pixels;
			*pFeatureReturnPara16=HM2057_PV_PERIOD_LINE_NUMS+HM2057_PV_dummy_lines;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			*pFeatureReturnPara32 = HM2057_sensor_pclk/10;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_SET_ESHUTTER:
		break;
		case SENSOR_FEATURE_SET_NIGHTMODE:
			HM2057_night_mode((BOOL) *pFeatureData16);
		break;
		case SENSOR_FEATURE_SET_GAIN:
		case SENSOR_FEATURE_SET_FLASHLIGHT:
		break;
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			HM2057_isp_master_clock=*pFeatureData32;
		break;
		case SENSOR_FEATURE_SET_REGISTER:
			HM2057_write_cmos_sensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
		break;
		case SENSOR_FEATURE_GET_REGISTER:
			pSensorRegData->RegData = HM2057_read_cmos_sensor(pSensorRegData->RegAddr);
		break;
		case SENSOR_FEATURE_GET_CONFIG_PARA:
			memcpy(pSensorConfigData, &HM2057SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
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
			// get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
			// if EEPROM does not exist in camera module.
			*pFeatureReturnPara32=LENS_DRIVER_ID_DO_NOT_CARE;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_SET_YUV_CMD:
//		       printk("HM2057 YUV sensor Setting:%d, %d \n", *pFeatureData32,  *(pFeatureData32+1));
			HM2057YUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
		break;
#if WINMO_USE		    		
		case SENSOR_FEATURE_QUERY:
			HM2057Query(pSensorFeatureInfo);
			*pFeatureParaLen = sizeof(MSDK_FEATURE_INFO_STRUCT);
		break;		
		case SENSOR_FEATURE_SET_YUV_CAPTURE_RAW_SUPPORT:
			/* update yuv capture raw support flag by *pFeatureData16 */
		break;		
#endif 			
		case SENSOR_FEATURE_SET_VIDEO_MODE:
		       HM2057YUVSetVideoMode(*pFeatureData16);
		       break; 
	case SENSOR_FEATURE_CHECK_SENSOR_ID:
		HM2057GetSensorID(pFeatureData32); 
	break; 				   
		default:
			break;			
	}

	return ERROR_NONE;
}	/* HM2057FeatureControl() */


SENSOR_FUNCTION_STRUCT	SensorFuncHM2057=
{
	HM2057Open,
	HM2057GetInfo,
	HM2057GetResolution,
	HM2057FeatureControl,
	HM2057Control,
	HM2057Close
};

UINT32 HM2057_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&SensorFuncHM2057;

	return ERROR_NONE;
}	/* SensorInit() */


