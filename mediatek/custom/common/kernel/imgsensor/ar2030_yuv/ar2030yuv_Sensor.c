/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
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
 *   sensor.c
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Source code of Sensor driver
 *
 *

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
//#include <mach/mt6516_pll.h>

#include "kd_camera_hw.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"

#include "ar2030yuv_Sensor.h"
#include "ar2030yuv_Camera_Sensor_para.h"
#include "ar2030yuv_CameraCustomized.h"

#define AR2030YUV_DEBUG
#ifdef AR2030YUV_DEBUG
	#define SENSORDB printk
#else
	#define SENSORDB(x,...)
#endif


extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);

kal_uint16 AR2030_write_cmos_sensor(kal_uint32 addr, kal_uint32 para)
{
    char puSendCmd[4] = {(char)(addr >> 8) , (char)(addr & 0xFF) ,(char)(para >> 8),(char)(para & 0xFF)};
    
    iWriteRegI2C(puSendCmd , 4,AR2030_sensor.write_id);
}

kal_uint16 AR2030_read_cmos_sensor(kal_uint32 addr)
{
    kal_uint16 get_byte=0;
    char puSendCmd[2] = {(char)(addr >> 8) , (char)(addr & 0xFF) };
    iReadRegI2C(puSendCmd , 2, (u8*)&get_byte,2,AR2030_sensor.write_id);
        
    return ((get_byte<<8)&0xff00)|((get_byte>>8)&0x00ff);
}


/*******************************************************************************
* // Adapter for Winmo typedef 
********************************************************************************/

#define DRV_ISP_YUV_BURST_MODE_SUPPORT
#define Sleep(ms) mdelay(ms)

MSDK_SENSOR_CONFIG_STRUCT AR2030SensorConfigData;

/*********yuan add driver here****************/
UINT32 AR2030YUVSensorSetting(FEATURE_ID iCmd, UINT32 iPara);

struct AR2030_sensor_STRUCT AR2030_sensor;
static kal_uint32 AR2030_zoom_factor = 0; 
static MSDK_SCENARIO_ID_ENUM AR2030_currentScenarioId = MSDK_SCENARIO_ID_CAMERA_PREVIEW;

/*************************************************************************
* FUNCTION
*    AR2030_write_XDMA
*
* DESCRIPTION
*    This function write XMDA
*
* PARAMETERS
*    addr    : the 16bit address of XDMA
*    para    : the 16bit value of XDMA
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void AR2030_write_XDMA(kal_uint16 addr, kal_uint16 para)
{
	AR2030_write_cmos_sensor(0x098C, addr);
	AR2030_write_cmos_sensor(0x0990, para);
}

/*************************************************************************
* FUNCTION
*    AR2030_read_XDMA
*
* DESCRIPTION
*    This function read XDMA
*
* PARAMETERS
*    addr    : the 16bit address of XDMA
*
* RETURNS
*    The 16bit data frome XDMA
*
* LOCAL AFFECTED
*
*************************************************************************/
static kal_uint16 AR2030_read_XDMA(kal_uint16 addr)
{
	AR2030_write_cmos_sensor(0x098C, addr);
	return AR2030_read_cmos_sensor(0x0990);
}

/*************************************************************************
* FUNCTION
*    AR2030_burst_write_XDMA
*
* DESCRIPTION
*    This function burst wirte XDMA to CMOS sensor through I2C
*    AR2030_MAX_I2C_FAIL_TRY_NO is the max try number. when write fail, the function retry again until
*    try times reach AR2030_MAX_I2C_FAIL_TRY_NO.
*
* PARAMETERS
*    addr    : the 16bit address of register
*    data    : pointer of data
*    len      : length of data
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void AR2030_burst_write_XDMA(kal_uint16 addr, const kal_uint16 *data, kal_uint16 len)
{
  kal_uint16 i, j = 0;
  
  for (i = 0; i < len; i++, j += 2)
  {
    if (!(i&7))
    {
      j = 0;
      AR2030_write_cmos_sensor(0x098C, addr + i * 2);
    }
    AR2030_write_cmos_sensor(0x0990 + j, data[i]);
  }
}

/*************************************************************************
* FUNCTION
*    AR2030_half_adjust
*
* DESCRIPTION
*    This function dividend / divisor and use round-up.
*
* PARAMETERS
*    dividend
*    divisor
*
* RETURNS
*    [dividend / divisor]
*
* LOCAL AFFECTED
*
*************************************************************************/
__inline static kal_uint16 AR2030_half_adjust(kal_uint32 dividend, kal_uint32 divisor)
{
  return (dividend * 2 + divisor) / (divisor * 2); /* that is [dividend / divisor + 0.5]*/
}

/*************************************************************************
* FUNCTION
*    AR2030_set_cmd
*
* DESCRIPTION
*    This function set command to the sequencer to change the state.
*    when set cmd fail, the function retry again until try times reach 200.
*
* PARAMETERS
*    cmd    : command, MUST BE 1, 2, 5, 6 !!!
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void AR2030_set_cmd(kal_uint16 cmd)
{
  const kal_uint8 status = (cmd == 1 ? 3 : 7);
  kal_uint8 delay, i;
  
  if (AR2030_sensor.initial) return;
  if ((cmd == 1 && AR2030_sensor.pv_mode) || (cmd == 2 && !AR2030_sensor.pv_mode)) return;
    AR2030_write_XDMA(0xA103, cmd);
  for (delay = 5, i = 100; i > 0; i--) /* check if ready to set */
  {
    if (!AR2030_read_XDMA(0xA103)) break;
    printk("1111AR2030 CMD i=%d cmd=%d\n",i,cmd);
    Sleep(4*delay);
    delay = 2;
  }
#ifdef AR2030YUV_DEBUG
  if (!i) SENSORDB( "sequencer time out");
#endif

  if (cmd >= 5) return;
  AR2030_sensor.pv_mode = !AR2030_sensor.pv_mode;
  for (delay = 8, i = 100; i > 0; i--) /* wait for ready */
  {
    if (AR2030_read_XDMA(0xA104) == status) break;
    Sleep(4*delay);
    printk("2222AR2030 CMD i=%d,cmd=%d\n",i,cmd);
    delay = 2;
  }
#ifdef AR2030YUV_DEBUG
  if (!i) SENSORDB("set cmd: %d time out", cmd);
#endif
}


/*************************************************************************
* FUNCTION
*    AR2030_set_mirror
*
* DESCRIPTION
*    This function set the mirror to the CMOS sensor
*
* PARAMETERS
*    mirror
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void AR2030_set_mirror(kal_uint8 mirror)
{
  kal_uint8 sensor_mirror = 0;
  
 // if (AR2030_sensor.mirror == mirror) return;
  AR2030_sensor.mirror = mirror;
  switch (AR2030_sensor.mirror)
  {
  case IMAGE_NORMAL:
     sensor_mirror = 0;
    break;         
  case IMAGE_H_MIRROR:
    sensor_mirror = 1;
    break;
  case IMAGE_V_MIRROR:
    sensor_mirror = 2;
    break;
  case IMAGE_HV_MIRROR:
    sensor_mirror = 3;
    break;
  default:
    break;
  }
//  SET_CAMERA_INPUT_ORDER(INPUT_ORDER_CbYCrY1);  to check here
  AR2030_write_XDMA(0x2717, 0x046C|sensor_mirror); /* preview read mode */
  AR2030_write_XDMA(0x272D, 0x0024|sensor_mirror); /* snapshot/video read mode */
  AR2030_set_cmd(6); /* refresh mode */
}


/*************************************************************************
* FUNCTION
*    AR2030_config_context
*
* DESCRIPTION
*    This function config the context A and context B.
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
static void AR2030_config_context(void)
{
  //Register wizard
  AR2030_write_cmos_sensor(0x98C, 0x2703); //Output Width (A)
  AR2030_write_cmos_sensor(0x990, 0x0320); //  = 800
  AR2030_write_cmos_sensor(0x98C, 0x2705); //Output Height (A)
  AR2030_write_cmos_sensor(0x990, 0x0258); //  = 600
  AR2030_write_cmos_sensor(0x98C, 0x2707); //Output Width (B)
  AR2030_write_cmos_sensor(0x990, 0x0640); //  = 1600
  AR2030_write_cmos_sensor(0x98C, 0x2709); //Output Height (B)
  AR2030_write_cmos_sensor(0x990, 0x04B0); //  = 1200
  AR2030_write_cmos_sensor(0x98C, 0x270D); //Row Start (A)
  AR2030_write_cmos_sensor(0x990, 0x0000); //  = 0
  AR2030_write_cmos_sensor(0x98C, 0x270F); //Column Start (A)
  AR2030_write_cmos_sensor(0x990, 0x0000); //  = 0
  AR2030_write_cmos_sensor(0x98C, 0x2711); //Row End (A)
  AR2030_write_cmos_sensor(0x990, 0x04BD); //  = 1213
  AR2030_write_cmos_sensor(0x98C, 0x2713); //Column End (A)
  AR2030_write_cmos_sensor(0x990, 0x064D); //  = 1613
  AR2030_write_cmos_sensor(0x98C, 0x2715); //Row Speed (A)
  AR2030_write_cmos_sensor(0x990, 0x0111); //   = 273
  AR2030_write_cmos_sensor(0x98C, 0x2719); //sensor_fine_correction (A)
  AR2030_write_cmos_sensor(0x990, 0x005A); //   = 90
  AR2030_write_cmos_sensor(0x98C, 0x271B); //sensor_fine_IT_min (A)
  AR2030_write_cmos_sensor(0x990, 0x01BE); //   = 446
  AR2030_write_cmos_sensor(0x98C, 0x271D); //sensor_fine_IT_max_margin (A)
  AR2030_write_cmos_sensor(0x990, 0x0131); //   = 305
  AR2030_write_cmos_sensor(0x98C, 0x2723); //Row Start (B)
  AR2030_write_cmos_sensor(0x990, 0x0004); //  = 4
  AR2030_write_cmos_sensor(0x98C, 0x2725); //Column Start (B)
  AR2030_write_cmos_sensor(0x990, 0x0004); //  = 4
  AR2030_write_cmos_sensor(0x98C, 0x2727); //Row End (B)
  AR2030_write_cmos_sensor(0x990, 0x04BB); //  = 1211
  AR2030_write_cmos_sensor(0x98C, 0x2729); //Column End (B)
  AR2030_write_cmos_sensor(0x990, 0x064B); //  = 1611
  AR2030_write_cmos_sensor(0x98C, 0x272B); //Row Speed (B)
  AR2030_write_cmos_sensor(0x990, 0x0111); //   = 273
  AR2030_write_cmos_sensor(0x98C, 0x272F); //sensor_fine_correction (B)
  AR2030_write_cmos_sensor(0x990, 0x003A); //   = 58
  AR2030_write_cmos_sensor(0x98C, 0x2731); //sensor_fine_IT_min (B)
  AR2030_write_cmos_sensor(0x990, 0x00F6); //   = 246
  AR2030_write_cmos_sensor(0x98C, 0x2733); //sensor_fine_IT_max_margin (B)
  AR2030_write_cmos_sensor(0x990, 0x008B); //   = 139
  AR2030_write_cmos_sensor(0x98C, 0x2739); //Crop_X0 (A)
  AR2030_write_cmos_sensor(0x990, 0x0000); //   = 0
  AR2030_write_cmos_sensor(0x98C, 0x273B); //Crop_X1 (A)
  AR2030_write_cmos_sensor(0x990, 0x031F); //   = 799
  AR2030_write_cmos_sensor(0x98C, 0x273D); //Crop_Y0 (A)
  AR2030_write_cmos_sensor(0x990, 0x0000); //   = 0
  AR2030_write_cmos_sensor(0x98C, 0x273F); //Crop_Y1 (A)
  AR2030_write_cmos_sensor(0x990, 0x0257); //   = 599
  AR2030_write_cmos_sensor(0x98C, 0x2747); //Crop_X0 (B)
  AR2030_write_cmos_sensor(0x990, 0x0000); //   = 0
  AR2030_write_cmos_sensor(0x98C, 0x2749); //Crop_X1 (B)
  AR2030_write_cmos_sensor(0x990, 0x063F); //   = 1599
  AR2030_write_cmos_sensor(0x98C, 0x274B); //Crop_Y0 (B)
  AR2030_write_cmos_sensor(0x990, 0x0000); //   = 0
  AR2030_write_cmos_sensor(0x98C, 0x274D); //Crop_Y1 (B)
  AR2030_write_cmos_sensor(0x990, 0x04AF); //   = 1199
  /* no need config search_f1/f2_50/60, for they use in auto FD!!! */
  AR2030_write_cmos_sensor(0x98C, 0xA404); //FD Mode
  AR2030_write_cmos_sensor(0x990, 0x10  ); //    = 16
  AR2030_write_cmos_sensor(0x98C, 0xA40D); //Stat_min
  AR2030_write_cmos_sensor(0x990, 0x02  ); //    = 2
  AR2030_write_cmos_sensor(0x98C, 0xA40E); //Stat_max
  AR2030_write_cmos_sensor(0x990, 0x03  ); //    = 3
  AR2030_write_cmos_sensor(0x98C, 0xA410); //Min_amplitude
  AR2030_write_cmos_sensor(0x990, 0x0A  ); //    = 10
}

/*************************************************************************
* FUNCTION
*    AR2030_patch
*
* DESCRIPTION
*    This function Patch the AR2030 which provided by Micron
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
static void AR2030_patch(void)
{
  // SOC2030 Rev2 patch
  const static kal_uint16 patch_data[]=
  {
    0xF601,0x42C1,0x0226,0x11F6,0x0143,0xC102,0x260A,0xCC04,0x33BD,0xA365,0xBD04,0x3339,0xC6FF,0xF701,0x6439,0xFE02,
    0x5418,0xCE03,0x42CC,0x000B,0xBDC2,0xBFCC,0x04BF,0xFD03,0x4CCC,0x0342,0xFD02,0x545F,0x4FFD,0x025F,0xFE02,0xBD18,
    0xCE03,0x30C6,0x11BD,0xC2BF,0xCC05,0x03FD,0x033A,0xCC05,0xB5FD,0x033E,0xCC03,0x30FD,0x02BD,0xDE00,0x18CE,0x00C2,
    0xCC00,0x37BD,0xC2BF,0xCC06,0xAEDD,0xC4CC,0x06B4,0xDDD6,0xCC00,0xC2DD,0x00B6,0x02C3,0x810A,0x2407,0x1686,0x0A3D,
    0xF702,0xC3B6,0x02C4,0x810A,0x2407,0x1686,0x0A3D,0xF702,0xC4CC,0x02BD,0xFD03,0x4EFE,0x034E,0xCC4A,0x38ED,0x66C6,
    0x02F7,0x0164,0xC609,0xF701,0x6539,0x3C3C,0x34BD,0xCDCF,0x7D02,0x5626,0x05F6,0x025F,0x2003,0xF602,0x6030,0xE704,
    0xCC34,0x00BD,0xA55B,0xC43F,0x84FE,0x30ED,0x02E6,0x044F,0x0505,0x0505,0x0505,0xC4C0,0x8401,0xEA03,0xAA02,0xED02,
    0xCC34,0x00ED,0x00EC,0x02BD,0xA547,0x3838,0x3139,0x3CF6,0x02C3,0xF102,0xC423,0x08F0,0x02C4,0x30E7,0x0020,0x0330,
    0x6F00,0xF602,0xC3FB,0x02C4,0xE701,0x4FB3,0x02C8,0x2415,0xF602,0xC5F1,0x02C2,0x2308,0xF002,0xC2F7,0x02C5,0x2016,
    0x7F02,0xC520,0x11E6,0x004F,0xB302,0xC823,0x09F6,0x02C5,0xFB02,0xC2F7,0x02C5,0xF602,0xC5F1,0x02C1,0x2306,0xF602,
    0xC1F7,0x02C5,0x3839,0x3736,0x3C3C,0x3C3C,0x3C34,0x30EC,0x0FBD,0xA55B,0x30ED,0x0617,0x847F,0xA70A,0xEC06,0xC480,
    0x8401,0xED08,0x2709,0x8301,0x8026,0x0668,0x0A20,0x0264,0x0AEC,0x0BBD,0xA55B,0x30ED,0x06E6,0x0A4F,0xED02,0xEC06,
    0xED00,0xCC00,0x80BD,0xA409,0x30EC,0x02ED,0x046D,0x0427,0x05CC,0x00FF,0xED04,0xE605,0x308F,0xC300,0x0D8F,0x3539,
    0x308F,0xC3FF,0xF38F,0x35CC,0x3210,0xBDA5,0x5B30,0xED06,0xBDD6,0x54CC,0x3210,0x30ED,0x00EC,0x06BD,0xA547,0xCC30,
    0x5A30,0xED00,0xCC32,0xD4BD,0x055B,0x30E7,0x08CC,0x3056,0xED00,0xCC32,0xD6BD,0x055B,0x30E7,0x09CC,0x3058,0xED00,
    0xCC32,0xDABD,0x055B,0x30E7,0x0AE6,0x09E7,0x0B6F,0x0CE6,0x0C4F,0x05C3,0x33F6,0xED00,0xE60C,0x4FED,0x028F,0xC300,
    0x0830,0xE302,0x188F,0x3C18,0xE600,0x4F30,0xED00,0x3CF6,0x02F1,0x30ED,0x003C,0xF602,0xF230,0xED00,0xE612,0xC302,
    0xE98F,0x34E6,0x0030,0xE700,0xE613,0x4FC3,0x02ED,0x8FE6,0x00BD,0xA4EE,0x3838,0x3831,0x4FBD,0xA547,0x306C,0x0CE6,
    0x0CC1,0x0425,0xAACE,0x02BD,0x1F1F,0x8047,0xCC33,0xF4BD,0xA55B,0xC4BF,0x30ED,0x04FE,0x034E,0xFC02,0xD8A3,0x6625,
    0x0430,0x1C05,0x40CE,0x02BD,0x1F1F,0x401C,0xCC32,0x10BD,0xA55B,0xC4EF,0x30ED,0x061E,0x0540,0x031C,0x0710,0xCC32,
    0x10ED,0x00EC,0x06BD,0xA547,0xCC33,0xF430,0xED00,0xEC04,0xBDA5,0x4730,0xC60D,0x3A35,0x39BD,0xA705,0xBDA7,0x613C,
    0x3C3C,0x122F,0x0203,0x5F20,0x02C6,0x0130,0xE702,0xCC00,0x18BD,0xA55B,0x8580,0x2603,0x5F20,0x02C6,0x0130,0xE703,
    0xD604,0xF704,0x13CC,0x0016,0xBDA5,0x5BFD,0x040F,0xCC00,0x14BD,0xA55B,0xFD04,0x0DCC,0x001A,0xBDA5,0x5BFD,0x0411,
    0xDE00,0xEE24,0xAD00,0xCC00,0x1630,0xED00,0xDC30,0xBDA5,0x47CC,0x001A,0xBDA5,0x5B30,0xED04,0x1F04,0x020C,0xCC00,
    0x1AED,0x00EC,0x0484,0xFDBD,0xA547,0xDE00,0xEE36,0xC605,0xAD00,0xCC07,0xFFFD,0x1042,0xC603,0xF710,0x44F6,0x104D,
    0xC4F0,0xCA09,0xF710,0x4DCC,0x0020,0x30ED,0x00CC,0x0001,0xBDA5,0x47CC,0x0026,0x30ED,0x0034,0xBDA5,0x5BCA,0x0231,
    0xBDA5,0x47CC,0x0018,0x30ED,0x0034,0xBDA5,0x5B8A,0x4031,0xBDA5,0x4720,0x11C6,0xFFF7,0x1040,0xB610,0x40FE,0x0140,
    0xEE00,0xC620,0xAD00,0x306D,0x0227,0x0FCC,0x0018,0xBDA5,0x5BC4,0x0184,0x8083,0x0000,0x26DB,0x306D,0x0226,0x06D6,
    0x03C1,0x0126,0xD0CC,0x0016,0xED00,0xFC04,0x0FCA,0x20BD,0xA547,0xDE00,0xEE30,0xAD00,0xCC00,0x2630,0xED00,0x34BD,
    0xA55B,0xC4FD,0x31BD,0xA547,0xCC00,0x1A30,0xED00,0xEC04,0x8A08,0xBDA5,0x47CC,0x001A,0x30ED,0x00EC,0x04BD,0xA547,
    0xCC00,0x1630,0xED00,0xFC04,0x0FBD,0xA547,0xDE00,0xEE2E,0xAD00,0x306D,0x0226,0x06DE,0x00EE,0x2CAD,0x00F6,0x104D,
    0xC4F0,0xCA0A,0xF710,0x4D38,0x3838
  };
  
  AR2030_burst_write_XDMA(0x0415, patch_data, sizeof(patch_data) / sizeof(patch_data[0]));
  AR2030_write_XDMA(0x87FF, 0x0039);
  AR2030_write_XDMA(0x2006, 0x0415); /* MON_ARG1 */
  AR2030_write_XDMA(0xA005, 0x01); /* MON_CMD */
  Sleep(4); /* wait for the patch to complete initialization */
}

/*************************************************************************
* FUNCTION
*    AR2030_LSC_setting
*
* DESCRIPTION
*    This function set Lens Shading Correction.
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

// < NOT USE >
static void AR2030_LSC_setting(void)
{
  //[Register Log 12/22/08 16:42:15]
  AR2030_write_cmos_sensor(0x364E, 0x05F0);    // P_GR_P0Q0
  AR2030_write_cmos_sensor(0x3650, 0xBC8B);    // P_GR_P0Q1
  AR2030_write_cmos_sensor(0x3652, 0x12D2);    // P_GR_P0Q2
  AR2030_write_cmos_sensor(0x3654, 0x284F);    // P_GR_P0Q3
  AR2030_write_cmos_sensor(0x3656, 0xCF13);    // P_GR_P0Q4
  AR2030_write_cmos_sensor(0x3658, 0x00D0);    // P_RD_P0Q0
  AR2030_write_cmos_sensor(0x365A, 0xB0EA);    // P_RD_P0Q1
  AR2030_write_cmos_sensor(0x365C, 0x39F2);    // P_RD_P0Q2
  AR2030_write_cmos_sensor(0x365E, 0x3C6F);    // P_RD_P0Q3
  AR2030_write_cmos_sensor(0x3660, 0xD173);    // P_RD_P0Q4
  AR2030_write_cmos_sensor(0x3662, 0x0130);    // P_BL_P0Q0
  AR2030_write_cmos_sensor(0x3664, 0x4FEA);    // P_BL_P0Q1
  AR2030_write_cmos_sensor(0x3666, 0x0472);    // P_BL_P0Q2
  AR2030_write_cmos_sensor(0x3668, 0x15CE);    // P_BL_P0Q3
  AR2030_write_cmos_sensor(0x366A, 0xA9F3);    // P_BL_P0Q4
  AR2030_write_cmos_sensor(0x366C, 0x00B0);    // P_GB_P0Q0
  AR2030_write_cmos_sensor(0x366E, 0x8FCC);    // P_GB_P0Q1
  AR2030_write_cmos_sensor(0x3670, 0x0F12);    // P_GB_P0Q2
  AR2030_write_cmos_sensor(0x3672, 0x552F);    // P_GB_P0Q3
  AR2030_write_cmos_sensor(0x3674, 0xCE93);    // P_GB_P0Q4
  AR2030_write_cmos_sensor(0x3676, 0x4B0C);    // P_GR_P1Q0
  AR2030_write_cmos_sensor(0x3678, 0xA9AF);    // P_GR_P1Q1
  AR2030_write_cmos_sensor(0x367A, 0x976E);    // P_GR_P1Q2
  AR2030_write_cmos_sensor(0x367C, 0x25F1);    // P_GR_P1Q3
  AR2030_write_cmos_sensor(0x367E, 0x582D);    // P_GR_P1Q4
  AR2030_write_cmos_sensor(0x3680, 0x126D);    // P_RD_P1Q0
  AR2030_write_cmos_sensor(0x3682, 0x68AF);    // P_RD_P1Q1
  AR2030_write_cmos_sensor(0x3684, 0x06AF);    // P_RD_P1Q2
  AR2030_write_cmos_sensor(0x3686, 0xCD71);    // P_RD_P1Q3
  AR2030_write_cmos_sensor(0x3688, 0xA0F1);    // P_RD_P1Q4
  AR2030_write_cmos_sensor(0x368A, 0x32AD);    // P_BL_P1Q0
  AR2030_write_cmos_sensor(0x368C, 0x942F);    // P_BL_P1Q1
  AR2030_write_cmos_sensor(0x368E, 0xC56E);    // P_BL_P1Q2
  AR2030_write_cmos_sensor(0x3690, 0x7511);    // P_BL_P1Q3
  AR2030_write_cmos_sensor(0x3692, 0x8F30);    // P_BL_P1Q4
  AR2030_write_cmos_sensor(0x3694, 0x394C);    // P_GB_P1Q0
  AR2030_write_cmos_sensor(0x3696, 0x59CF);    // P_GB_P1Q1
  AR2030_write_cmos_sensor(0x3698, 0xA1CD);    // P_GB_P1Q2
  AR2030_write_cmos_sensor(0x369A, 0xB9F1);    // P_GB_P1Q3
  AR2030_write_cmos_sensor(0x369C, 0x158F);    // P_GB_P1Q4
  AR2030_write_cmos_sensor(0x369E, 0x7212);    // P_GR_P2Q0
  AR2030_write_cmos_sensor(0x36A0, 0x3A90);    // P_GR_P2Q1
  AR2030_write_cmos_sensor(0x36A2, 0x8E75);    // P_GR_P2Q2
  AR2030_write_cmos_sensor(0x36A4, 0xC4F3);    // P_GR_P2Q3
  AR2030_write_cmos_sensor(0x36A6, 0x4AB6);    // P_GR_P2Q4
  AR2030_write_cmos_sensor(0x36A8, 0x0853);    // P_RD_P2Q0
  AR2030_write_cmos_sensor(0x36AA, 0x3B70);    // P_RD_P2Q1
  AR2030_write_cmos_sensor(0x36AC, 0x9BD4);    // P_RD_P2Q2
  AR2030_write_cmos_sensor(0x36AE, 0xD0B1);    // P_RD_P2Q3
  AR2030_write_cmos_sensor(0x36B0, 0x9636);    // P_RD_P2Q4
  AR2030_write_cmos_sensor(0x36B2, 0x5F32);    // P_BL_P2Q0
  AR2030_write_cmos_sensor(0x36B4, 0x3D4F);    // P_BL_P2Q1
  AR2030_write_cmos_sensor(0x36B6, 0xF7B4);    // P_BL_P2Q2
  AR2030_write_cmos_sensor(0x36B8, 0xA1F1);    // P_BL_P2Q3
  AR2030_write_cmos_sensor(0x36BA, 0x0C76);    // P_BL_P2Q4
  AR2030_write_cmos_sensor(0x36BC, 0x7EB2);    // P_GB_P2Q0
  AR2030_write_cmos_sensor(0x36BE, 0x33D0);    // P_GB_P2Q1
  AR2030_write_cmos_sensor(0x36C0, 0x97D5);    // P_GB_P2Q2
  AR2030_write_cmos_sensor(0x36C2, 0xA6F3);    // P_GB_P2Q3
  AR2030_write_cmos_sensor(0x36C4, 0x4216);    // P_GB_P2Q4
  AR2030_write_cmos_sensor(0x36C6, 0x266C);    // P_GR_P3Q0
  AR2030_write_cmos_sensor(0x36C8, 0xF770);    // P_GR_P3Q1
  AR2030_write_cmos_sensor(0x36CA, 0x3E31);    // P_GR_P3Q2
  AR2030_write_cmos_sensor(0x36CC, 0x53F4);    // P_GR_P3Q3
  AR2030_write_cmos_sensor(0x36CE, 0x02D1);    // P_GR_P3Q4
  AR2030_write_cmos_sensor(0x36D0, 0xA56F);    // P_RD_P3Q0
  AR2030_write_cmos_sensor(0x36D2, 0x8712);    // P_RD_P3Q1
  AR2030_write_cmos_sensor(0x36D4, 0xFC6F);    // P_RD_P3Q2
  AR2030_write_cmos_sensor(0x36D6, 0x2334);    // P_RD_P3Q3
  AR2030_write_cmos_sensor(0x36D8, 0xD6B2);    // P_RD_P3Q4
  AR2030_write_cmos_sensor(0x36DA, 0x85EE);    // P_BL_P3Q0
  AR2030_write_cmos_sensor(0x36DC, 0xD270);    // P_BL_P3Q1
  AR2030_write_cmos_sensor(0x36DE, 0xD591);    // P_BL_P3Q2
  AR2030_write_cmos_sensor(0x36E0, 0x7E33);    // P_BL_P3Q3
  AR2030_write_cmos_sensor(0x36E2, 0x33B4);    // P_BL_P3Q4
  AR2030_write_cmos_sensor(0x36E4, 0x23AE);    // P_GB_P3Q0
  AR2030_write_cmos_sensor(0x36E6, 0x8D31);    // P_GB_P3Q1
  AR2030_write_cmos_sensor(0x36E8, 0x0672);    // P_GB_P3Q2
  AR2030_write_cmos_sensor(0x36EA, 0x2C50);    // P_GB_P3Q3
  AR2030_write_cmos_sensor(0x36EC, 0x9794);    // P_GB_P3Q4
  AR2030_write_cmos_sensor(0x36EE, 0xE374);    // P_GR_P4Q0
  AR2030_write_cmos_sensor(0x36F0, 0xD892);    // P_GR_P4Q1
  AR2030_write_cmos_sensor(0x36F2, 0x9E34);    // P_GR_P4Q2
  AR2030_write_cmos_sensor(0x36F4, 0x44D6);    // P_GR_P4Q3
  AR2030_write_cmos_sensor(0x36F6, 0x17FA);    // P_GR_P4Q4
  AR2030_write_cmos_sensor(0x36F8, 0x8C94);    // P_RD_P4Q0
  AR2030_write_cmos_sensor(0x36FA, 0x9570);    // P_RD_P4Q1
  AR2030_write_cmos_sensor(0x36FC, 0x8198);    // P_RD_P4Q2
  AR2030_write_cmos_sensor(0x36FE, 0x1154);    // P_RD_P4Q3
  AR2030_write_cmos_sensor(0x3700, 0x3B9B);    // P_RD_P4Q4
  AR2030_write_cmos_sensor(0x3702, 0xA294);    // P_BL_P4Q0
  AR2030_write_cmos_sensor(0x3704, 0x2291);    // P_BL_P4Q1
  AR2030_write_cmos_sensor(0x3706, 0x8936);    // P_BL_P4Q2
  AR2030_write_cmos_sensor(0x3708, 0xBE33);    // P_BL_P4Q3
  AR2030_write_cmos_sensor(0x370A, 0x5C9A);    // P_BL_P4Q4
  AR2030_write_cmos_sensor(0x370C, 0xF5B4);    // P_GB_P4Q0
  AR2030_write_cmos_sensor(0x370E, 0xA892);    // P_GB_P4Q1
  AR2030_write_cmos_sensor(0x3710, 0xAA74);    // P_GB_P4Q2
  AR2030_write_cmos_sensor(0x3712, 0x1A76);    // P_GB_P4Q3
  AR2030_write_cmos_sensor(0x3714, 0x2DFA);    // P_GB_P4Q4
  AR2030_write_cmos_sensor(0x3644, 0x0308);    // POLY_ORIGIN_C
  AR2030_write_cmos_sensor(0x3642, 0x0258);    // POLY_ORIGIN_R
  AR2030_write_cmos_sensor(0x3210, 0x01B8); // COLOR_PIPELINE_CONTROL

}

/*************************************************************************
* FUNCTION
*    AR2030_gamma_setting
*
* DESCRIPTION
*    This function set two context of gamma.
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

// < NOT USE >
static void AR2030_gamma_setting(void)
{
  AR2030_write_cmos_sensor(0x098C, 0xAB3C); // MCU_ADDRESS [HG_GAMMA_TABLE_A_0] 
  AR2030_write_cmos_sensor(0x0990, 0x0000); // MCU_DATA_0 #24
  AR2030_write_cmos_sensor(0x098C, 0xAB3D); // MCU_ADDRESS [HG_GAMMA_TABLE_A_1] 
  AR2030_write_cmos_sensor(0x0990, 0x0010); // MCU_DATA_0 #23
  AR2030_write_cmos_sensor(0x098C, 0xAB3E); // MCU_ADDRESS [HG_GAMMA_TABLE_A_2] 
  AR2030_write_cmos_sensor(0x0990, 0x002E); // MCU_DATA_0 #22
  AR2030_write_cmos_sensor(0x098C, 0xAB3F); // MCU_ADDRESS [HG_GAMMA_TABLE_A_3] 
  AR2030_write_cmos_sensor(0x0990, 0x004C); // MCU_DATA_0 #21
  AR2030_write_cmos_sensor(0x098C, 0xAB40); // MCU_ADDRESS [HG_GAMMA_TABLE_A_4] 
  AR2030_write_cmos_sensor(0x0990, 0x0078); // MCU_DATA_0 #20
  AR2030_write_cmos_sensor(0x098C, 0xAB41); // MCU_ADDRESS [HG_GAMMA_TABLE_A_5] 
  AR2030_write_cmos_sensor(0x0990, 0x0098); // MCU_DATA_0 #19
  AR2030_write_cmos_sensor(0x098C, 0xAB42); // MCU_ADDRESS [HG_GAMMA_TABLE_A_6] 
  AR2030_write_cmos_sensor(0x0990, 0x00B0); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB43); // MCU_ADDRESS [HG_GAMMA_TABLE_A_7] 
  AR2030_write_cmos_sensor(0x0990, 0x00C1); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB44); // MCU_ADDRESS [HG_GAMMA_TABLE_A_8] 
  AR2030_write_cmos_sensor(0x0990, 0x00CF); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB45); // MCU_ADDRESS [HG_GAMMA_TABLE_A_9] 
  AR2030_write_cmos_sensor(0x0990, 0x00D9); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB46); // MCU_ADDRESS [HG_GAMMA_TABLE_A_10]
  AR2030_write_cmos_sensor(0x0990, 0x00E1); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB47); // MCU_ADDRESS [HG_GAMMA_TABLE_A_11]
  AR2030_write_cmos_sensor(0x0990, 0x00E8); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB48); // MCU_ADDRESS [HG_GAMMA_TABLE_A_12]
  AR2030_write_cmos_sensor(0x0990, 0x00EE); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB49); // MCU_ADDRESS [HG_GAMMA_TABLE_A_13]
  AR2030_write_cmos_sensor(0x0990, 0x00F2); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB4A); // MCU_ADDRESS [HG_GAMMA_TABLE_A_14]
  AR2030_write_cmos_sensor(0x0990, 0x00F6); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB4B); // MCU_ADDRESS [HG_GAMMA_TABLE_A_15]
  AR2030_write_cmos_sensor(0x0990, 0x00F9); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB4C); // MCU_ADDRESS [HG_GAMMA_TABLE_A_16]
  AR2030_write_cmos_sensor(0x0990, 0x00FB); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB4D); // MCU_ADDRESS [HG_GAMMA_TABLE_A_17]
  AR2030_write_cmos_sensor(0x0990, 0x00FD); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB4E); // MCU_ADDRESS [HG_GAMMA_TABLE_A_18]
  AR2030_write_cmos_sensor(0x0990, 0x00FF); // MCU_DATA_0 

  AR2030_write_cmos_sensor(0x098C, 0xAB4F); // MCU_ADDRESS [HG_GAMMA_TABLE_B_0]
  AR2030_write_cmos_sensor(0x0990, 0x0000); // MCU_DATA_0 #24
  AR2030_write_cmos_sensor(0x098C, 0xAB50); // MCU_ADDRESS [HG_GAMMA_TABLE_B_1]
  AR2030_write_cmos_sensor(0x0990, 0x0002); // MCU_DATA_0 #23
  AR2030_write_cmos_sensor(0x098C, 0xAB51); // MCU_ADDRESS [HG_GAMMA_TABLE_B_2]
  AR2030_write_cmos_sensor(0x0990, 0x0010); // MCU_DATA_0 #22
  AR2030_write_cmos_sensor(0x098C, 0xAB52); // MCU_ADDRESS [HG_GAMMA_TABLE_B_3]
  AR2030_write_cmos_sensor(0x0990, 0x002E); // MCU_DATA_0 #21
  AR2030_write_cmos_sensor(0x098C, 0xAB53); // MCU_ADDRESS [HG_GAMMA_TABLE_B_4]
  AR2030_write_cmos_sensor(0x0990, 0x005A); // MCU_DATA_0 #20
  AR2030_write_cmos_sensor(0x098C, 0xAB54); // MCU_ADDRESS [HG_GAMMA_TABLE_B_5]
  AR2030_write_cmos_sensor(0x0990, 0x006A); // MCU_DATA_0 #19
  AR2030_write_cmos_sensor(0x098C, 0xAB55); // MCU_ADDRESS [HG_GAMMA_TABLE_B_6]
  AR2030_write_cmos_sensor(0x0990, 0x0080); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB56); // MCU_ADDRESS [HG_GAMMA_TABLE_B_7]
  AR2030_write_cmos_sensor(0x0990, 0x0091); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB57); // MCU_ADDRESS [HG_GAMMA_TABLE_B_8]
  AR2030_write_cmos_sensor(0x0990, 0x00A1); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB58); // MCU_ADDRESS [HG_GAMMA_TABLE_B_9]
  AR2030_write_cmos_sensor(0x0990, 0x00AF); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB59); // MCU_ADDRESS [HG_GAMMA_TABLE_B_10]
  AR2030_write_cmos_sensor(0x0990, 0x00BB); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB5A); // MCU_ADDRESS [HG_GAMMA_TABLE_B_11]
  AR2030_write_cmos_sensor(0x0990, 0x00C6); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB5B); // MCU_ADDRESS [HG_GAMMA_TABLE_B_12]
  AR2030_write_cmos_sensor(0x0990, 0x00D0); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB5C); // MCU_ADDRESS [HG_GAMMA_TABLE_B_13]
  AR2030_write_cmos_sensor(0x0990, 0x00D9); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB5D); // MCU_ADDRESS [HG_GAMMA_TABLE_B_14]
  AR2030_write_cmos_sensor(0x0990, 0x00E2); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB5E); // MCU_ADDRESS [HG_GAMMA_TABLE_B_15]
  AR2030_write_cmos_sensor(0x0990, 0x00EA); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB5F); // MCU_ADDRESS [HG_GAMMA_TABLE_B_16]
  AR2030_write_cmos_sensor(0x0990, 0x00F1); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB60); // MCU_ADDRESS [HG_GAMMA_TABLE_B_17]
  AR2030_write_cmos_sensor(0x0990, 0x00F9); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB61); // MCU_ADDRESS [HG_GAMMA_TABLE_B_18]
  AR2030_write_cmos_sensor(0x0990, 0x00FF); // MCU_DATA_0
}

/*************************************************************************
* FUNCTION
*    AR2030_low_light
*
* DESCRIPTION
*    This function set low light performance.
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

// < EMPTY >
// < NOT USE >
static void AR2030_low_light(void)
{
    
    return;
    
  //Low light settings
  AR2030_write_cmos_sensor(0x098C, 0x2B1B); // MCU_ADDRESS [HG_BRIGHTNESSMETRIC]
  AR2030_write_cmos_sensor(0x0990, 0x392D); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2B1D); // MCU_ADDRESS [HG_LASTBRIGHTNESSMETRIC]
  AR2030_write_cmos_sensor(0x0990, 0x392D); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB1F); // MCU_ADDRESS [HG_LLMODE]
  AR2030_write_cmos_sensor(0x0990, 0x00C5); // MCU_DATA_0
  /* saturation setting: 0xAB20*/
  AR2030_write_cmos_sensor(0x098C, 0xAB20); // MCU_ADDRESS [HG_LL_SAT1] HIGH LIGHT SATURATION
#if defined(DRV_ISP_YUV_BURST_MODE_SUPPORT)
  AR2030_write_cmos_sensor(0x0990, 0x003B); // MCU_DATA_0
#else
  AR2030_write_cmos_sensor(0x0990, 0x0043); // MCU_DATA_0
#endif
  AR2030_write_cmos_sensor(0x098C, 0xAB21); // MCU_ADDRESS [HG_LL_INTERPTHRESH1]
  AR2030_write_cmos_sensor(0x0990, 0x0010); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB22); // MCU_ADDRESS [HG_LL_APCORR1]
  AR2030_write_cmos_sensor(0x0990, 0x0004); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB23); // MCU_ADDRESS [HG_LL_APTHRESH1]
  AR2030_write_cmos_sensor(0x0990, 0x0004); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB24); // MCU_ADDRESS [HG_LL_SAT2] LOW LIGHT SATURATION
  AR2030_write_cmos_sensor(0x0990, 0x0020); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB25); // MCU_ADDRESS [HG_LL_INTERPTHRESH2]
  AR2030_write_cmos_sensor(0x0990, 0x0032); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB26); // MCU_ADDRESS [HG_LL_APCORR2]
  AR2030_write_cmos_sensor(0x0990, 0x0000); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB27); // MCU_ADDRESS [HG_LL_APTHRESH2]
  AR2030_write_cmos_sensor(0x0990, 0x0008); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2B28); // MCU_ADDRESS [HG_LL_BRIGHTNESSSTART]
  AR2030_write_cmos_sensor(0x0990, 0x1E14); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2B2A); // MCU_ADDRESS [HG_LL_BRIGHTNESSSTOP]
  AR2030_write_cmos_sensor(0x0990, 0x3A98); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB2C); // MCU_ADDRESS [HG_NR_START_R]
  AR2030_write_cmos_sensor(0x0990, 0x0001); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB2D); // MCU_ADDRESS [HG_NR_START_G]
  AR2030_write_cmos_sensor(0x0990, 0x0001); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB2E); // MCU_ADDRESS [HG_NR_START_B]
  AR2030_write_cmos_sensor(0x0990, 0x0001); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB2F); // MCU_ADDRESS [HG_NR_START_OL]
  AR2030_write_cmos_sensor(0x0990, 0x0001); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB30); // MCU_ADDRESS [HG_NR_STOP_R]
  AR2030_write_cmos_sensor(0x0990, 0x00F0); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB31); // MCU_ADDRESS [HG_NR_STOP_G]
  AR2030_write_cmos_sensor(0x0990, 0x00F0); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB32); // MCU_ADDRESS [HG_NR_STOP_B]
  AR2030_write_cmos_sensor(0x0990, 0x00F0); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB33); // MCU_ADDRESS [HG_NR_STOP_OL]
  AR2030_write_cmos_sensor(0x0990, 0x00F0); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB36); // MCU_ADDRESS [HG_CLUSTERDC_TH]
  AR2030_write_cmos_sensor(0x0990, 0x002D); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2B62); // MCU_ADDRESS [HG_FTB_START_BM]
  AR2030_write_cmos_sensor(0x0990, 0xFFFF); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2B64); // MCU_ADDRESS [HG_FTB_STOP_BM]
  AR2030_write_cmos_sensor(0x0990, 0xFFFF); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2B66); // MCU_ADDRESS [HG_CLUSTER_DC_BM]
  AR2030_write_cmos_sensor(0x0990, 0x3A98); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x275F); // MCU_ADDRESS [MODE_COMMONMODESETTINGS_BRIGHT_COLOR_KILL]
#if defined(DRV_ISP_YUV_BURST_MODE_SUPPORT)
  AR2030_write_cmos_sensor(0x0990, 0x0595); // MCU_DATA_0
#else
  AR2030_write_cmos_sensor(0x0990, 0x0596); // MCU_DATA_0
#endif
  AR2030_write_cmos_sensor(0x098C, 0x2761); // MCU_ADDRESS [MODE_COMMONMODESETTINGS_DARK_COLOR_KILL]
  AR2030_write_cmos_sensor(0x0990, 0x0080); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xA765); // MCU_ADDRESS [MODE_COMMONMODESETTINGS_FILTER_MODE]
  AR2030_write_cmos_sensor(0x0990, 0x00A4); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB34); // MCU_ADDRESS [HG_NR_GAINSTART]
  AR2030_write_cmos_sensor(0x0990, 0x0008); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xAB35); // MCU_ADDRESS [HG_NR_GAINSTOP]
  AR2030_write_cmos_sensor(0x0990, 0x0078); // MCU_DATA_0
}

/*************************************************************************
* FUNCTION
*    AR2030_awb_ccms
*
* DESCRIPTION
*    This function set awb and ccms.
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

// < NOT USE >
static void AR2030_awb_ccms(void)
{
  //AWB and CCMS
  AR2030_write_cmos_sensor(0x098C, 0x2306); // MCU_ADDRESS [AWB_CCM_L_0]
  AR2030_write_cmos_sensor(0x0990, 0x0180); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2308); // MCU_ADDRESS [AWB_CCM_L_1]
  AR2030_write_cmos_sensor(0x0990, 0xFF00); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x230A); // MCU_ADDRESS [AWB_CCM_L_2]
  AR2030_write_cmos_sensor(0x0990, 0x0080); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x230C); // MCU_ADDRESS [AWB_CCM_L_3]
  AR2030_write_cmos_sensor(0x0990, 0xFF66); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x230E); // MCU_ADDRESS [AWB_CCM_L_4]
  AR2030_write_cmos_sensor(0x0990, 0x0180); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2310); // MCU_ADDRESS [AWB_CCM_L_5]
  AR2030_write_cmos_sensor(0x0990, 0xFFEE); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2312); // MCU_ADDRESS [AWB_CCM_L_6]
  AR2030_write_cmos_sensor(0x0990, 0xFFCD); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2314); // MCU_ADDRESS [AWB_CCM_L_7]
  AR2030_write_cmos_sensor(0x0990, 0xFECD); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2316); // MCU_ADDRESS [AWB_CCM_L_8]
  AR2030_write_cmos_sensor(0x0990, 0x019A); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2318); // MCU_ADDRESS [AWB_CCM_L_9]
  AR2030_write_cmos_sensor(0x0990, 0x0020); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x231A); // MCU_ADDRESS [AWB_CCM_L_10]
  AR2030_write_cmos_sensor(0x0990, 0x0033); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x231C); // MCU_ADDRESS [AWB_CCM_RL_0]
  AR2030_write_cmos_sensor(0x0990, 0x0100); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x231E); // MCU_ADDRESS [AWB_CCM_RL_1]
  AR2030_write_cmos_sensor(0x0990, 0xFF9A); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2320); // MCU_ADDRESS [AWB_CCM_RL_2]
  AR2030_write_cmos_sensor(0x0990, 0x0000); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2322); // MCU_ADDRESS [AWB_CCM_RL_3]
  AR2030_write_cmos_sensor(0x0990, 0x004D); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2324); // MCU_ADDRESS [AWB_CCM_RL_4]
  AR2030_write_cmos_sensor(0x0990, 0xFFCD); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2326); // MCU_ADDRESS [AWB_CCM_RL_5]
  AR2030_write_cmos_sensor(0x0990, 0xFFB8); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2328); // MCU_ADDRESS [AWB_CCM_RL_6]
  AR2030_write_cmos_sensor(0x0990, 0x004D); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x232A); // MCU_ADDRESS [AWB_CCM_RL_7]
  AR2030_write_cmos_sensor(0x0990, 0x0080); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x232C); // MCU_ADDRESS [AWB_CCM_RL_8]
  AR2030_write_cmos_sensor(0x0990, 0xFF66); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x232E); // MCU_ADDRESS [AWB_CCM_RL_9]
  AR2030_write_cmos_sensor(0x0990, 0x0010); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0x2330); // MCU_ADDRESS [AWB_CCM_RL_10]
  AR2030_write_cmos_sensor(0x0990, 0xFFF7); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xA363); // MCU_ADDRESS [AWB_TG_MIN0]
  AR2030_write_cmos_sensor(0x0990, 0x00D2); // MCU_DATA_0
  AR2030_write_cmos_sensor(0x098C, 0xA364); // MCU_ADDRESS [AWB_TG_MAX0]
  AR2030_write_cmos_sensor(0x0990, 0x00EE); // MCU_DATA_0
  
  AR2030_write_cmos_sensor(0x3244, 0x0302); /* AWB_CONFIG4: change AWB: 0x0328 */
  AR2030_write_cmos_sensor(0x323E, 0xC22C); /* AWB_CONFIG1 */
}

/*************************************************************************
* FUNCTION
*    AR2030_set_pv_dummy
*
* DESCRIPTION
*    This function add dummy_pixel and dummy_line in preview state.
*    MUST BE INVOKED BEFORE AR2030_set_cmd(1) !!!
*
* PARAMETERS
*    dummy_pixel
*    dummy_line
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void AR2030_set_pv_dummy(kal_uint16 dummy_pixel, kal_uint16 dummy_line)
{
  const kal_uint16 frame_height = AR2030_PV_PERIOD_LINE_NUMS + dummy_line;
  const kal_uint16 line_length = AR2030_PV_PERIOD_PIXEL_NUMS + dummy_pixel;
  
  if (AR2030_sensor.pv_frame_height == frame_height && AR2030_sensor.pv_line_length == line_length) return;
  AR2030_sensor.pv_frame_height = frame_height;
  AR2030_sensor.pv_line_length = line_length;
  AR2030_write_XDMA(0x271F, frame_height); /* preview frame length */
  AR2030_write_XDMA(0x2721, line_length); /* preview line length */
  AR2030_write_XDMA(0x2411, AR2030_half_adjust(AR2030_sensor.pclk / 2, line_length * AR2030_NUM_60HZ * 2));/* preview fd_r9_step_f60 */
  AR2030_write_XDMA(0x2413, AR2030_half_adjust(AR2030_sensor.pclk / 2, line_length * AR2030_NUM_50HZ * 2));/* preview fd_r9_step_f50 */
  AR2030_set_cmd(6); /* refresh mode */
}

/*************************************************************************
* FUNCTION
*    AR2030_set_cap_dummy
*
* DESCRIPTION
*    This function add dummy_pixel and dummy_line in capture state.
*    MUST BE INVOKED BEFORE AR2030_set_cmd(2) !!!
*
* PARAMETERS
*    dummy_pixel
*    dummy_line
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/

// < NOT USE >
static void AR2030_set_cap_dummy(kal_uint16 dummy_pixel, kal_uint16 dummy_line)
{
  const kal_uint16 frame_height = AR2030_FULL_PERIOD_LINE_NUMS + dummy_line;
  const kal_uint16 line_length = AR2030_FULL_PERIOD_PIXEL_NUMS + dummy_pixel;
  
  if (AR2030_sensor.cap_frame_height == frame_height && AR2030_sensor.cap_line_length == line_length) return;
  AR2030_sensor.cap_frame_height = frame_height;
  AR2030_sensor.cap_line_length = line_length;
  AR2030_write_XDMA(0x2735, frame_height); /* snapshot/video frame length */
  AR2030_write_XDMA(0x2737, line_length); /* snapshot/video line length */
  AR2030_write_XDMA(0x2415, AR2030_half_adjust(AR2030_sensor.pclk / 2, line_length * AR2030_NUM_60HZ * 2));/* snapshot/video fd_r9_step_f60 */
  AR2030_write_XDMA(0x2417, AR2030_half_adjust(AR2030_sensor.pclk / 2, line_length * AR2030_NUM_50HZ * 2));/* snapshot/video fd_r9_step_f50 */
  AR2030_set_cmd(6); /* refresh mode */
}

/*************************************************************************
* FUNCTION
*    AR2030_cal_fps
*
* DESCRIPTION
*    This function calculate & set frame rate and fix frame rate when video mode
*    MUST BE INVOKED AFTER AR2030_preview() !!!
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
static void AR2030_cal_fps(void)
{
  const kal_uint16 dummy_pixel = AR2030_sensor.pv_line_length - AR2030_PV_PERIOD_PIXEL_NUMS;
  const kal_uint8 banding = AR2030_sensor.banding == AE_FLICKER_MODE_50HZ ? AR2030_NUM_50HZ : AR2030_NUM_60HZ;
  const kal_uint16 pv_max_fps = AR2030_sensor.pclk * AR2030_FRAME_RATE_UNIT / (2 * AR2030_sensor.pv_line_length * AR2030_sensor.pv_frame_height);
  kal_uint16 pv_min_fps =  AR2030_sensor.night_mode ? AR2030_sensor.night_fps : AR2030_sensor.normal_fps;
  kal_uint16 max_exposure_lines;
  
  SENSORDB("AR2030_cal_fps AR2030_sensor.video_mode %d\n",AR2030_sensor.video_mode);
  
  if (pv_min_fps > pv_max_fps) pv_min_fps = pv_max_fps;
  if (AR2030_sensor.video_mode) /* fix frame rate when video mode */
  {
    SENSORDB("AR2030_cal_fps AR2030_sensor.video_frame %d\n",AR2030_sensor.video_frame);
      pv_min_fps =  AR2030_sensor.video_frame ? AR2030_sensor.night_fps : AR2030_sensor.normal_fps;
    
    SENSORDB("AR2030_cal_fps pv_min_fps %d\n",pv_min_fps);
    max_exposure_lines = AR2030_sensor.pclk * AR2030_FRAME_RATE_UNIT / (2 * pv_min_fps * AR2030_sensor.pv_line_length);
    AR2030_set_pv_dummy(dummy_pixel, max_exposure_lines - AR2030_PV_PERIOD_LINE_NUMS);
  }
  AR2030_write_XDMA(0xA20C, banding * 2 * AR2030_FRAME_RATE_UNIT / pv_min_fps);
  AR2030_set_cmd(5); /* refresh */
}

/*************************************************************************
* FUNCTION
*    AR2030_initial_setting
*
* DESCRIPTION
*    This function initialize the registers of CMOS sensor
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
static void AR2030_initial_setting(void)
{
	
	//[step1 - PLL]
	AR2030_write_cmos_sensor(0x001A, 0x0051);
	AR2030_write_cmos_sensor(0x001A, 0x0050);
	mdelay(10);
	AR2030_write_cmos_sensor(0x001A, 0x0050);
	AR2030_write_cmos_sensor(0x001A, 0x0058);

	AR2030_write_cmos_sensor(0x0014, 0x2545);    //PLL Control: BYPASS PLL = 9541
	AR2030_write_cmos_sensor(0x0010, 0x0115);    //PLL Dividers = 277
	AR2030_write_cmos_sensor(0x0012, 0x00F5);    //PLL P Dividers = 245
	AR2030_write_cmos_sensor(0x0014, 0x2547);    //PLL Control: PLL_ENABLE on = 9543
	AR2030_write_cmos_sensor(0x0014, 0x2447);    //PLL Control: SEL_LOCK_DET on = 9287
	
	mdelay(10);                 // Allow PLL to lock
	AR2030_write_cmos_sensor(0x0014, 0x2047);    //PLL Control: PLL_BYPASS off = 8263
	AR2030_write_cmos_sensor(0x0014, 0x2046);    //PLL Control: = 8262
	AR2030_write_cmos_sensor(0x0018, 0x402C);    //
	mdelay(10);   
	//[step2 - Timing]
	AR2030_write_cmos_sensor(0x321C, 0x0000);  //By Pass TxFIFO = 0
	AR2030_write_cmos_sensor(0x98C, 0x2703);  //Output Width (A)
	AR2030_write_cmos_sensor(0x990, 0x0320);  //      = 800
	AR2030_write_cmos_sensor(0x98C, 0x2705);  //Output Height (A)
	AR2030_write_cmos_sensor(0x990, 0x0258);  //      = 600
	AR2030_write_cmos_sensor(0x98C, 0x2707);  //Output Width (B)
	AR2030_write_cmos_sensor(0x990, 0x0640);  //      = 1600
	AR2030_write_cmos_sensor(0x98C, 0x2709);  //Output Height (B)
	AR2030_write_cmos_sensor(0x990, 0x04B0);  //      = 1200
	AR2030_write_cmos_sensor(0x98C, 0x270D);  //Row Start (A)
	AR2030_write_cmos_sensor(0x990, 0x000);  //      = 0
	AR2030_write_cmos_sensor(0x98C, 0x270F);  //Column Start (A)
	AR2030_write_cmos_sensor(0x990, 0x000);  //      = 0
	AR2030_write_cmos_sensor(0x98C, 0x2711);  //Row End (A)
	AR2030_write_cmos_sensor(0x990, 0x4BD);  //      = 1213
	AR2030_write_cmos_sensor(0x98C, 0x2713);  //Column End (A)
	AR2030_write_cmos_sensor(0x990, 0x64D);  //      = 1613
	AR2030_write_cmos_sensor(0x98C, 0x2715);  //Row Speed (A)
	AR2030_write_cmos_sensor(0x990, 0x0111);  //      = 273
	AR2030_write_cmos_sensor(0x98C, 0x2717);  //Read Mode (A)
	AR2030_write_cmos_sensor(0x990, 0x046C);  //      = 1132
	AR2030_write_cmos_sensor(0x98C, 0x2719);  //sensor_fine_correction (A)
	AR2030_write_cmos_sensor(0x990, 0x005A);  //      = 90
	AR2030_write_cmos_sensor(0x98C, 0x271B);  //sensor_fine_IT_min (A)
	AR2030_write_cmos_sensor(0x990, 0x01BE);  //      = 446
	AR2030_write_cmos_sensor(0x98C, 0x271D);  //sensor_fine_IT_max_margin (A)
	AR2030_write_cmos_sensor(0x990, 0x0131);  //      = 305
	AR2030_write_cmos_sensor(0x98C, 0x271F);  //Frame Lines (A)
	AR2030_write_cmos_sensor(0x990, 0x02B3);  //      = 691
	AR2030_write_cmos_sensor(0x98C, 0x2721);  //Line Length (A)
	AR2030_write_cmos_sensor(0x990, 0x07EA);  //      = 2026
	AR2030_write_cmos_sensor(0x98C, 0x2723);  //Row Start (B)
	AR2030_write_cmos_sensor(0x990, 0x004);  //      = 4
	AR2030_write_cmos_sensor(0x98C, 0x2725);  //Column Start (B)
	AR2030_write_cmos_sensor(0x990, 0x004);  //      = 4
	AR2030_write_cmos_sensor(0x98C, 0x2727);  //Row End (B)
	AR2030_write_cmos_sensor(0x990, 0x4BB);  //      = 1211
	AR2030_write_cmos_sensor(0x98C, 0x2729);  //Column End (B)
	AR2030_write_cmos_sensor(0x990, 0x64B);  //      = 1611
	AR2030_write_cmos_sensor(0x98C, 0x272B);  //Row Speed (B)
	AR2030_write_cmos_sensor(0x990, 0x0111);  //      = 273
	AR2030_write_cmos_sensor(0x98C, 0x272D);  //Read Mode (B)
	AR2030_write_cmos_sensor(0x990, 0x0024);  //      = 36
	AR2030_write_cmos_sensor(0x98C, 0x272F);  //sensor_fine_correction (B)
	AR2030_write_cmos_sensor(0x990, 0x003A);  //      = 58
	AR2030_write_cmos_sensor(0x98C, 0x2731);  //sensor_fine_IT_min (B)
	AR2030_write_cmos_sensor(0x990, 0x00F6);  //      = 246
	AR2030_write_cmos_sensor(0x98C, 0x2733);  //sensor_fine_IT_max_margin (B)
	AR2030_write_cmos_sensor(0x990, 0x008B);  //      = 139
	AR2030_write_cmos_sensor(0x98C, 0x2735);  //Frame Lines (B)
	AR2030_write_cmos_sensor(0x990, 0x050D);  //      = 1293
	AR2030_write_cmos_sensor(0x98C, 0x2737);  //Line Length (B)
	AR2030_write_cmos_sensor(0x990, 0x0875);  //      = 2165
	AR2030_write_cmos_sensor(0x98C, 0x2739);  //Crop_X0 (A)
	AR2030_write_cmos_sensor(0x990, 0x0000);  //      = 0
	AR2030_write_cmos_sensor(0x98C, 0x273B);  //Crop_X1 (A)
	AR2030_write_cmos_sensor(0x990, 0x031F);  //      = 799
	AR2030_write_cmos_sensor(0x98C, 0x273D);  //Crop_Y0 (A)
	AR2030_write_cmos_sensor(0x990, 0x0000);  //      = 0
	AR2030_write_cmos_sensor(0x98C, 0x273F);  //Crop_Y1 (A)
	AR2030_write_cmos_sensor(0x990, 0x0257);  //      = 599
	AR2030_write_cmos_sensor(0x98C, 0x2747);  //Crop_X0 (B)
	AR2030_write_cmos_sensor(0x990, 0x0000);  //      = 0
	AR2030_write_cmos_sensor(0x98C, 0x2749);  //Crop_X1 (B)
	AR2030_write_cmos_sensor(0x990, 0x063F);  //      = 1599
	AR2030_write_cmos_sensor(0x98C, 0x274B);  //Crop_Y0 (B)
	AR2030_write_cmos_sensor(0x990, 0x0000);  //      = 0
	AR2030_write_cmos_sensor(0x98C, 0x274D);  //Crop_Y1 (B)
	AR2030_write_cmos_sensor(0x990, 0x04AF);  //      = 1199
	AR2030_write_cmos_sensor(0x98C, 0x222D);  //R9 Step
	AR2030_write_cmos_sensor(0x990, 0x00AD);  //      = 173
	AR2030_write_cmos_sensor(0x98C, 0xA408);  //search_f1_50
	AR2030_write_cmos_sensor(0x990, 0x2A);  //      = 42
	AR2030_write_cmos_sensor(0x98C, 0xA409);  //search_f2_50
	AR2030_write_cmos_sensor(0x990, 0x2C);  //      = 44
	AR2030_write_cmos_sensor(0x98C, 0xA40A);  //search_f1_60
	AR2030_write_cmos_sensor(0x990, 0x32);  //      = 50
	AR2030_write_cmos_sensor(0x98C, 0xA40B);  //search_f2_60
	AR2030_write_cmos_sensor(0x990, 0x34);  //      = 52
	AR2030_write_cmos_sensor(0x98C, 0x2411);  //R9_Step_60 (A)
	AR2030_write_cmos_sensor(0x990, 0x00AD);  //      = 173
	AR2030_write_cmos_sensor(0x98C, 0x2413);  //R9_Step_50 (A)
	AR2030_write_cmos_sensor(0x990, 0x00CF);  //      = 207
	AR2030_write_cmos_sensor(0x98C, 0x2415);  //R9_Step_60 (B)
	AR2030_write_cmos_sensor(0x990, 0x00A2);  //      = 162
	AR2030_write_cmos_sensor(0x98C, 0x2417);  //R9_Step_50 (B)
	AR2030_write_cmos_sensor(0x990, 0x00C2);  //      = 194
	AR2030_write_cmos_sensor(0x98C, 0xA404);  //FD Mode
	AR2030_write_cmos_sensor(0x990, 0x10);  //      = 16
	AR2030_write_cmos_sensor(0x98C, 0xA40D);  //Stat_min
	AR2030_write_cmos_sensor(0x990, 0x02);  //      = 2
	AR2030_write_cmos_sensor(0x98C, 0xA40E);  //Stat_max
	AR2030_write_cmos_sensor(0x990, 0x03);  //      = 3
	AR2030_write_cmos_sensor(0x98C, 0xA410);  //Min_amplitude
	AR2030_write_cmos_sensor(0x990, 0x0A);  //      = 10
	AR2030_write_cmos_sensor(0x98C, 0xA103);  //Refresh Sequencer Mode
	AR2030_write_cmos_sensor(0x990, 0x06);  //      = 6

	//[step3 - LSC]
	//lens shading
	AR2030_write_cmos_sensor(0x3210, 0x01B0);    // COLOR_PIPELINE_CONTROL
  AR2030_write_cmos_sensor(0x364E, 0x05F0);    // P_GR_P0Q0
  AR2030_write_cmos_sensor(0x3650, 0xBC8B);    // P_GR_P0Q1
  AR2030_write_cmos_sensor(0x3652, 0x12D2);    // P_GR_P0Q2
  AR2030_write_cmos_sensor(0x3654, 0x284F);    // P_GR_P0Q3
  AR2030_write_cmos_sensor(0x3656, 0xCF13);    // P_GR_P0Q4
  AR2030_write_cmos_sensor(0x3658, 0x00D0);    // P_RD_P0Q0
  AR2030_write_cmos_sensor(0x365A, 0xB0EA);    // P_RD_P0Q1
  AR2030_write_cmos_sensor(0x365C, 0x39F2);    // P_RD_P0Q2
  AR2030_write_cmos_sensor(0x365E, 0x3C6F);    // P_RD_P0Q3
  AR2030_write_cmos_sensor(0x3660, 0xD173);    // P_RD_P0Q4
  AR2030_write_cmos_sensor(0x3662, 0x0130);    // P_BL_P0Q0
  AR2030_write_cmos_sensor(0x3664, 0x4FEA);    // P_BL_P0Q1
  AR2030_write_cmos_sensor(0x3666, 0x0472);    // P_BL_P0Q2
  AR2030_write_cmos_sensor(0x3668, 0x15CE);    // P_BL_P0Q3
  AR2030_write_cmos_sensor(0x366A, 0xA9F3);    // P_BL_P0Q4
  AR2030_write_cmos_sensor(0x366C, 0x00B0);    // P_GB_P0Q0
  AR2030_write_cmos_sensor(0x366E, 0x8FCC);    // P_GB_P0Q1
  AR2030_write_cmos_sensor(0x3670, 0x0F12);    // P_GB_P0Q2
  AR2030_write_cmos_sensor(0x3672, 0x552F);    // P_GB_P0Q3
  AR2030_write_cmos_sensor(0x3674, 0xCE93);    // P_GB_P0Q4
  AR2030_write_cmos_sensor(0x3676, 0x4B0C);    // P_GR_P1Q0
  AR2030_write_cmos_sensor(0x3678, 0xA9AF);    // P_GR_P1Q1
  AR2030_write_cmos_sensor(0x367A, 0x976E);    // P_GR_P1Q2
  AR2030_write_cmos_sensor(0x367C, 0x25F1);    // P_GR_P1Q3
  AR2030_write_cmos_sensor(0x367E, 0x582D);    // P_GR_P1Q4
  AR2030_write_cmos_sensor(0x3680, 0x126D);    // P_RD_P1Q0
  AR2030_write_cmos_sensor(0x3682, 0x68AF);    // P_RD_P1Q1
  AR2030_write_cmos_sensor(0x3684, 0x06AF);    // P_RD_P1Q2
  AR2030_write_cmos_sensor(0x3686, 0xCD71);    // P_RD_P1Q3
  AR2030_write_cmos_sensor(0x3688, 0xA0F1);    // P_RD_P1Q4
  AR2030_write_cmos_sensor(0x368A, 0x32AD);    // P_BL_P1Q0
  AR2030_write_cmos_sensor(0x368C, 0x942F);    // P_BL_P1Q1
  AR2030_write_cmos_sensor(0x368E, 0xC56E);    // P_BL_P1Q2
  AR2030_write_cmos_sensor(0x3690, 0x7511);    // P_BL_P1Q3
  AR2030_write_cmos_sensor(0x3692, 0x8F30);    // P_BL_P1Q4
  AR2030_write_cmos_sensor(0x3694, 0x394C);    // P_GB_P1Q0
  AR2030_write_cmos_sensor(0x3696, 0x59CF);    // P_GB_P1Q1
  AR2030_write_cmos_sensor(0x3698, 0xA1CD);    // P_GB_P1Q2
  AR2030_write_cmos_sensor(0x369A, 0xB9F1);    // P_GB_P1Q3
  AR2030_write_cmos_sensor(0x369C, 0x158F);    // P_GB_P1Q4
  AR2030_write_cmos_sensor(0x369E, 0x7212);    // P_GR_P2Q0
  AR2030_write_cmos_sensor(0x36A0, 0x3A90);    // P_GR_P2Q1
  AR2030_write_cmos_sensor(0x36A2, 0x8E75);    // P_GR_P2Q2
  AR2030_write_cmos_sensor(0x36A4, 0xC4F3);    // P_GR_P2Q3
  AR2030_write_cmos_sensor(0x36A6, 0x4AB6);    // P_GR_P2Q4
  AR2030_write_cmos_sensor(0x36A8, 0x0853);    // P_RD_P2Q0
  AR2030_write_cmos_sensor(0x36AA, 0x3B70);    // P_RD_P2Q1
  AR2030_write_cmos_sensor(0x36AC, 0x9BD4);    // P_RD_P2Q2
  AR2030_write_cmos_sensor(0x36AE, 0xD0B1);    // P_RD_P2Q3
  AR2030_write_cmos_sensor(0x36B0, 0x9636);    // P_RD_P2Q4
  AR2030_write_cmos_sensor(0x36B2, 0x5F32);    // P_BL_P2Q0
  AR2030_write_cmos_sensor(0x36B4, 0x3D4F);    // P_BL_P2Q1
  AR2030_write_cmos_sensor(0x36B6, 0xF7B4);    // P_BL_P2Q2
  AR2030_write_cmos_sensor(0x36B8, 0xA1F1);    // P_BL_P2Q3
  AR2030_write_cmos_sensor(0x36BA, 0x0C76);    // P_BL_P2Q4
  AR2030_write_cmos_sensor(0x36BC, 0x7EB2);    // P_GB_P2Q0
  AR2030_write_cmos_sensor(0x36BE, 0x33D0);    // P_GB_P2Q1
  AR2030_write_cmos_sensor(0x36C0, 0x97D5);    // P_GB_P2Q2
  AR2030_write_cmos_sensor(0x36C2, 0xA6F3);    // P_GB_P2Q3
  AR2030_write_cmos_sensor(0x36C4, 0x4216);    // P_GB_P2Q4
  AR2030_write_cmos_sensor(0x36C6, 0x266C);    // P_GR_P3Q0
  AR2030_write_cmos_sensor(0x36C8, 0xF770);    // P_GR_P3Q1
  AR2030_write_cmos_sensor(0x36CA, 0x3E31);    // P_GR_P3Q2
  AR2030_write_cmos_sensor(0x36CC, 0x53F4);    // P_GR_P3Q3
  AR2030_write_cmos_sensor(0x36CE, 0x02D1);    // P_GR_P3Q4
  AR2030_write_cmos_sensor(0x36D0, 0xA56F);    // P_RD_P3Q0
  AR2030_write_cmos_sensor(0x36D2, 0x8712);    // P_RD_P3Q1
  AR2030_write_cmos_sensor(0x36D4, 0xFC6F);    // P_RD_P3Q2
  AR2030_write_cmos_sensor(0x36D6, 0x2334);    // P_RD_P3Q3
  AR2030_write_cmos_sensor(0x36D8, 0xD6B2);    // P_RD_P3Q4
  AR2030_write_cmos_sensor(0x36DA, 0x85EE);    // P_BL_P3Q0
  AR2030_write_cmos_sensor(0x36DC, 0xD270);    // P_BL_P3Q1
  AR2030_write_cmos_sensor(0x36DE, 0xD591);    // P_BL_P3Q2
  AR2030_write_cmos_sensor(0x36E0, 0x7E33);    // P_BL_P3Q3
  AR2030_write_cmos_sensor(0x36E2, 0x33B4);    // P_BL_P3Q4
  AR2030_write_cmos_sensor(0x36E4, 0x23AE);    // P_GB_P3Q0
  AR2030_write_cmos_sensor(0x36E6, 0x8D31);    // P_GB_P3Q1
  AR2030_write_cmos_sensor(0x36E8, 0x0672);    // P_GB_P3Q2
  AR2030_write_cmos_sensor(0x36EA, 0x2C50);    // P_GB_P3Q3
  AR2030_write_cmos_sensor(0x36EC, 0x9794);    // P_GB_P3Q4
  AR2030_write_cmos_sensor(0x36EE, 0xE374);    // P_GR_P4Q0
  AR2030_write_cmos_sensor(0x36F0, 0xD892);    // P_GR_P4Q1
  AR2030_write_cmos_sensor(0x36F2, 0x9E34);    // P_GR_P4Q2
  AR2030_write_cmos_sensor(0x36F4, 0x44D6);    // P_GR_P4Q3
  AR2030_write_cmos_sensor(0x36F6, 0x17FA);    // P_GR_P4Q4
  AR2030_write_cmos_sensor(0x36F8, 0x8C94);    // P_RD_P4Q0
  AR2030_write_cmos_sensor(0x36FA, 0x9570);    // P_RD_P4Q1
  AR2030_write_cmos_sensor(0x36FC, 0x8198);    // P_RD_P4Q2
  AR2030_write_cmos_sensor(0x36FE, 0x1154);    // P_RD_P4Q3
  AR2030_write_cmos_sensor(0x3700, 0x3B9B);    // P_RD_P4Q4
  AR2030_write_cmos_sensor(0x3702, 0xA294);    // P_BL_P4Q0
  AR2030_write_cmos_sensor(0x3704, 0x2291);    // P_BL_P4Q1
  AR2030_write_cmos_sensor(0x3706, 0x8936);    // P_BL_P4Q2
  AR2030_write_cmos_sensor(0x3708, 0xBE33);    // P_BL_P4Q3
  AR2030_write_cmos_sensor(0x370A, 0x5C9A);    // P_BL_P4Q4
  AR2030_write_cmos_sensor(0x370C, 0xF5B4);    // P_GB_P4Q0
  AR2030_write_cmos_sensor(0x370E, 0xA892);    // P_GB_P4Q1
  AR2030_write_cmos_sensor(0x3710, 0xAA74);    // P_GB_P4Q2
  AR2030_write_cmos_sensor(0x3712, 0x1A76);    // P_GB_P4Q3
  AR2030_write_cmos_sensor(0x3714, 0x2DFA);    // P_GB_P4Q4
  AR2030_write_cmos_sensor(0x3644, 0x0308);    // POLY_ORIGIN_C
  AR2030_write_cmos_sensor(0x3642, 0x0258);    // POLY_ORIGIN_R
	AR2030_write_cmos_sensor(0x3210, 0x01B8);    // COLOR_PIPELINE_CONTROL

	//[step4 - low light setting]
	AR2030_write_cmos_sensor(0x098C, 0xA117);    // MCU_ADDRESS [SEQ_PREVIEW_0_AE]
	AR2030_write_cmos_sensor(0x0990, 0x0001);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xA11D);    // MCU_ADDRESS [SEQ_PREVIEW_1_AE]
	AR2030_write_cmos_sensor(0x0990, 0x0001);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xA129);    // MCU_ADDRESS [SEQ_PREVIEW_3_AE]
	AR2030_write_cmos_sensor(0x0990, 0x0001);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xA20C);    // MCU_ADDRESS [AE_MAX_INDEX]
	AR2030_write_cmos_sensor(0x0990, 0x0012);    // MCU_DATA_0  0E
	AR2030_write_cmos_sensor(0x098C, 0xA20E);    // MCU_ADDRESS [AE_MAX_VIRTGAIN]
	AR2030_write_cmos_sensor(0x0990, 0x0091);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2212);    // MCU_ADDRESS [AE_MAX_DGAIN_AE1]
	AR2030_write_cmos_sensor(0x0990, 0x00E0);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xA24F);    // MCU_ADDRESS [AE_MAX_DGAIN_AE1]
	AR2030_write_cmos_sensor(0x0990, 0x003C);    // MCU_DATA_0  60

	//AE Window [full size]
	AR2030_write_cmos_sensor(0x098C, 0xA202);   // MCU_ADDRESS [AE_WINDOW_POS]
	AR2030_write_cmos_sensor(0x0990, 0x0044);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xA203);   // MCU_ADDRESS [AE_WINDOW_SIZE]
	AR2030_write_cmos_sensor(0x0990, 0x0077);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xA103);   // MCU_ADDRESS [SEQ_CMD]
	AR2030_write_cmos_sensor(0x0990, 0x0006);       // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB24); // MCU_ADDRESS [HG_LL_SAT2] LOW LIGHT SATURATION
	AR2030_write_cmos_sensor(0x0990, 0x0034); // MCU_DATA_0  30 /38
	AR2030_write_cmos_sensor(0x098C, 0xAB36);    // MCU_ADDRESS [RESERVED_HG_36]
	AR2030_write_cmos_sensor(0x0990, 0x0014);    // MCU_DATA_0 

	//Errata for Rev2
	AR2030_write_cmos_sensor(0x3084, 0x240C);  
	AR2030_write_cmos_sensor(0x3092, 0x0A4C);  
	AR2030_write_cmos_sensor(0x3094, 0x4C4C);  
	AR2030_write_cmos_sensor(0x3096, 0x4C54);  

	//[AE Setting]     
	AR2030_write_cmos_sensor(0x098C, 0xAB22);    // MCU_ADDRESS [HG_LL_APCORR1]
	AR2030_write_cmos_sensor(0x0990, 0x0007);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB20);    // MCU_ADDRESS [HG_LL_APCORR1]///////////
	AR2030_write_cmos_sensor(0x0990, 0x0052);    // MCU_DATA_0  4A
	AR2030_write_cmos_sensor(0x098C, 0xAB26);    // MCU_ADDRESS [HG_LL_APCORR2]
	AR2030_write_cmos_sensor(0x0990, 0x0004);    // MCU_DATA_0  0x0007

	AR2030_write_cmos_sensor(0x098C, 0xA208);    // MCU_ADDRESS [SEQ_CMD]
	AR2030_write_cmos_sensor(0x0990, 0x0002);    // MCU_DATA_0  0x0002
	AR2030_write_cmos_sensor(0x098C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
	AR2030_write_cmos_sensor(0x0990, 0x0006);    // MCU_DATA_0
	mdelay(10);                          
	AR2030_write_cmos_sensor(0x098C, 0xA103);    // MCU_ADDRESS [SEQ_CMD]
	AR2030_write_cmos_sensor(0x0990, 0x0005);    // MCU_DATA_0
	mdelay(10);

	//NR
	AR2030_write_cmos_sensor(0x098C, 0xAB2C);    // MCU_ADDRESS [HG_NR_START_R]
	AR2030_write_cmos_sensor(0x0990, 0x00FF);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB30);    // MCU_ADDRESS [HG_NR_STOP_R]
	AR2030_write_cmos_sensor(0x0990, 0x00FF);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB2D);    // MCU_ADDRESS [HG_NR_START_G]
	AR2030_write_cmos_sensor(0x0990, 0x00FF);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB31);    // MCU_ADDRESS [HG_NR_STOP_G]
	AR2030_write_cmos_sensor(0x0990, 0x00FF);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB2E);    // MCU_ADDRESS [HG_NR_START_B]
	AR2030_write_cmos_sensor(0x0990, 0x00FF);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB32);    // MCU_ADDRESS [HG_NR_STOP_B]
	AR2030_write_cmos_sensor(0x0990, 0x00FF);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB2F);    // MCU_ADDRESS [HG_NR_START_OL]
	AR2030_write_cmos_sensor(0x0990, 0x000A);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB33);    // MCU_ADDRESS [HG_NR_STOP_OL]
	AR2030_write_cmos_sensor(0x0990, 0x0006);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB34);    // MCU_ADDRESS [HG_NR_GAINSTART]
	AR2030_write_cmos_sensor(0x0990, 0x0020);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB35);    // MCU_ADDRESS [HG_NR_GAINSTOP]
	AR2030_write_cmos_sensor(0x0990, 0x0091);    // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xA765);    // MCU_ADDRESS [MODE_COMMONMODESETTINGS_FILTER_MODE]
	AR2030_write_cmos_sensor(0x0990, 0x0006);    // MCU_DATA_0
	//[gamma_1]
	/*
	AR2030_write_cmos_sensor(0x098C, 0xAB3C); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_0]
	AR2030_write_cmos_sensor(0x0990, 0x0000); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB3D); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_1]
	AR2030_write_cmos_sensor(0x0990, 0x0007); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB3E); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_2]
	AR2030_write_cmos_sensor(0x0990, 0x0016); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB3F); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_3]
	AR2030_write_cmos_sensor(0x0990, 0x0039); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB40); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_4]
	AR2030_write_cmos_sensor(0x0990, 0x005F); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB41); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_5]
	AR2030_write_cmos_sensor(0x0990, 0x007A); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB42); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_6]
	AR2030_write_cmos_sensor(0x0990, 0x008F); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB43); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_7]
	AR2030_write_cmos_sensor(0x0990, 0x00A1); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB44); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_8]
	AR2030_write_cmos_sensor(0x0990, 0x00AF); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB45); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_9]
	AR2030_write_cmos_sensor(0x0990, 0x00BB); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB46); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_10]
	AR2030_write_cmos_sensor(0x0990, 0x00C6); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB47); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_11]
	AR2030_write_cmos_sensor(0x0990, 0x00CF); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB48); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_12]
	AR2030_write_cmos_sensor(0x0990, 0x00D8); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB49); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_13]
	AR2030_write_cmos_sensor(0x0990, 0x00E0); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB4A); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_14]
	AR2030_write_cmos_sensor(0x0990, 0x00E7); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB4B); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_15]
	AR2030_write_cmos_sensor(0x0990, 0x00EE); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB4C); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_16]
	AR2030_write_cmos_sensor(0x0990, 0x00F4); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB4D); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_17]
	AR2030_write_cmos_sensor(0x0990, 0x00FA); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB4E); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_18]
	AR2030_write_cmos_sensor(0x0990, 0x00FF); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xA103); 	// MCU_ADDRESS [SEQ_CMD]
	AR2030_write_cmos_sensor(0x0990, 0x0005); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB4F); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_0]
	AR2030_write_cmos_sensor(0x0990, 0x0000); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB50); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_1]
	AR2030_write_cmos_sensor(0x0990, 0x0007); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB51); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_2]
	AR2030_write_cmos_sensor(0x0990, 0x0016); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB52); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_3]
	AR2030_write_cmos_sensor(0x0990, 0x0039); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB53); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_4]
	AR2030_write_cmos_sensor(0x0990, 0x005F); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB54); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_5]
	AR2030_write_cmos_sensor(0x0990, 0x007A); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB55); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_6]
	AR2030_write_cmos_sensor(0x0990, 0x008F); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB56); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_7]
	AR2030_write_cmos_sensor(0x0990, 0x00A1); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB57); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_8]
	AR2030_write_cmos_sensor(0x0990, 0x00AF); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB58); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_9]
	AR2030_write_cmos_sensor(0x0990, 0x00BB); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB59); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_10]
	AR2030_write_cmos_sensor(0x0990, 0x00C6); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB5A); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_11]
	AR2030_write_cmos_sensor(0x0990, 0x00CF); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB5B); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_12]
	AR2030_write_cmos_sensor(0x0990, 0x00D8); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB5C); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_13]
	AR2030_write_cmos_sensor(0x0990, 0x00E0); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB5D); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_14]
	AR2030_write_cmos_sensor(0x0990, 0x00E7); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB5E); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_15]
	AR2030_write_cmos_sensor(0x0990, 0x00EE); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB5F); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_16]
	AR2030_write_cmos_sensor(0x0990, 0x00F4); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB60); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_17]
	AR2030_write_cmos_sensor(0x0990, 0x00FA); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xAB61); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_18]
	AR2030_write_cmos_sensor(0x0990, 0x00FF); 	// MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xA103); 	// MCU_ADDRESS [SEQ_CMD]
	AR2030_write_cmos_sensor(0x0990, 0x0005); 	// MCU_DATA_0
	*/
	//gamma 2
	AR2030_write_cmos_sensor(0x098C, 0xAB3C); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_0] 
	AR2030_write_cmos_sensor(0x0990, 0x0000); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB3D); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_1] 
	AR2030_write_cmos_sensor(0x0990, 0x0005); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB3E); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_2] 
	AR2030_write_cmos_sensor(0x0990, 0x000F); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB3F); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_3] 
	AR2030_write_cmos_sensor(0x0990, 0x0027); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB40); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_4] 
	AR2030_write_cmos_sensor(0x0990, 0x0048); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB41); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_5] 
	AR2030_write_cmos_sensor(0x0990, 0x0062); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB42); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_6] 
	AR2030_write_cmos_sensor(0x0990, 0x0078); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB43); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_7] 
	AR2030_write_cmos_sensor(0x0990, 0x008C); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB44); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_8] 
	AR2030_write_cmos_sensor(0x0990, 0x009D); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB45); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_9] 
	AR2030_write_cmos_sensor(0x0990, 0x00AB); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB46); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_10]
	AR2030_write_cmos_sensor(0x0990, 0x00B8); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB47); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_11]
	AR2030_write_cmos_sensor(0x0990, 0x00C4); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB48); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_12]
	AR2030_write_cmos_sensor(0x0990, 0x00CF); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB49); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_13]
	AR2030_write_cmos_sensor(0x0990, 0x00D8); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB4A); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_14]
	AR2030_write_cmos_sensor(0x0990, 0x00E1); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB4B); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_15]
	AR2030_write_cmos_sensor(0x0990, 0x00E9); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB4C); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_16]
	AR2030_write_cmos_sensor(0x0990, 0x00F1); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB4D); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_17]
	AR2030_write_cmos_sensor(0x0990, 0x00F8); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB4E); 	// MCU_ADDRESS [HG_GAMMA_TABLE_A_18]
	AR2030_write_cmos_sensor(0x0990, 0x00FF); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xA103); 	// MCU_ADDRESS [SEQ_CMD]            
	AR2030_write_cmos_sensor(0x0990, 0x0005); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB4F); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_0] 
	AR2030_write_cmos_sensor(0x0990, 0x0000); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB50); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_1] 
	AR2030_write_cmos_sensor(0x0990, 0x0005); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB51); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_2] 
	AR2030_write_cmos_sensor(0x0990, 0x000F); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB52); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_3] 
	AR2030_write_cmos_sensor(0x0990, 0x0027); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB53); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_4] 
	AR2030_write_cmos_sensor(0x0990, 0x0048); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB54); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_5] 
	AR2030_write_cmos_sensor(0x0990, 0x0062); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB55); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_6] 
	AR2030_write_cmos_sensor(0x0990, 0x0078); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB56); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_7] 
	AR2030_write_cmos_sensor(0x0990, 0x008C); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB57); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_8] 
	AR2030_write_cmos_sensor(0x0990, 0x009D); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB58); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_9] 
	AR2030_write_cmos_sensor(0x0990, 0x00AB); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB59); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_10]
	AR2030_write_cmos_sensor(0x0990, 0x00B8); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB5A); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_11]
	AR2030_write_cmos_sensor(0x0990, 0x00C4); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB5B); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_12]
	AR2030_write_cmos_sensor(0x0990, 0x00CF); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB5C); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_13]
	AR2030_write_cmos_sensor(0x0990, 0x00D8); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB5D); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_14]
	AR2030_write_cmos_sensor(0x0990, 0x00E1); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB5E); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_15]
	AR2030_write_cmos_sensor(0x0990, 0x00E9); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB5F); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_16]
	AR2030_write_cmos_sensor(0x0990, 0x00F1); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB60); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_17]
	AR2030_write_cmos_sensor(0x0990, 0x00F8); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xAB61); 	// MCU_ADDRESS [HG_GAMMA_TABLE_B_18]
	AR2030_write_cmos_sensor(0x0990, 0x00FF); 	// MCU_DATA_0                       
	AR2030_write_cmos_sensor(0x098C, 0xA103); 	// MCU_ADDRESS [SEQ_CMD]            
	AR2030_write_cmos_sensor(0x0990, 0x0005); 	// MCU_DATA_0        
	
	//[step5 - AWB AND CCM]
	//[tint]            
	AR2030_write_cmos_sensor(0x098C, 0xA366);     //     // MCU_ADDRESS [AWB_KR_L]
	AR2030_write_cmos_sensor(0x0990, 0x007A);     //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xA36B);     //     // MCU_ADDRESS [AWB_KB_R]
	AR2030_write_cmos_sensor(0x0990, 0x0074);     //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0xA103);     //     // MCU_ADDRESS [SEQ_CMD]
	AR2030_write_cmos_sensor(0x0990, 0x0005);     //     // MCU_DATA_0

	// //Color Correction Matrices
	AR2030_write_cmos_sensor(0x098C, 0x2306);   //     // MCU_ADDRESS [AWB_CCM_L_0]
	AR2030_write_cmos_sensor(0x0990, 0x01D6);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2308);   //     // MCU_ADDRESS [AWB_CCM_L_1]
	AR2030_write_cmos_sensor(0x0990, 0xFF89);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x230A);   //     // MCU_ADDRESS [AWB_CCM_L_2]
	AR2030_write_cmos_sensor(0x0990, 0xFFA1);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x230C);   //     // MCU_ADDRESS [AWB_CCM_L_3]
	AR2030_write_cmos_sensor(0x0990, 0xFF73);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x230E);   //     // MCU_ADDRESS [AWB_CCM_L_4]
	AR2030_write_cmos_sensor(0x0990, 0x019C);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2310);   //     // MCU_ADDRESS [AWB_CCM_L_5]
	AR2030_write_cmos_sensor(0x0990, 0xFFF1);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2312);   //     // MCU_ADDRESS [AWB_CCM_L_6]
	AR2030_write_cmos_sensor(0x0990, 0xFFB0);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2314);   //     // MCU_ADDRESS [AWB_CCM_L_7]
	AR2030_write_cmos_sensor(0x0990, 0xFF2D);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2316);   //     // MCU_ADDRESS [AWB_CCM_L_8]
	AR2030_write_cmos_sensor(0x0990, 0x0223);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2318);   //     // MCU_ADDRESS [AWB_CCM_L_9]
	AR2030_write_cmos_sensor(0x0990, 0x001C);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x231A);   //     // MCU_ADDRESS [AWB_CCM_L_10]
	AR2030_write_cmos_sensor(0x0990, 0x0048);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2318);   //     // MCU_ADDRESS [AWB_CCM_L_9]
	AR2030_write_cmos_sensor(0x0990, 0x001C);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x231A);   //     // MCU_ADDRESS [AWB_CCM_L_10]
	AR2030_write_cmos_sensor(0x0990, 0x0038);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2318);   //     // MCU_ADDRESS [AWB_CCM_L_9]
	AR2030_write_cmos_sensor(0x0990, 0x001E);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x231A);   //     // MCU_ADDRESS [AWB_CCM_L_10]
	AR2030_write_cmos_sensor(0x0990, 0x0038);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2318);   //     // MCU_ADDRESS [AWB_CCM_L_9]
	AR2030_write_cmos_sensor(0x0990, 0x0022);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x231A);   //     // MCU_ADDRESS [AWB_CCM_L_10]
	AR2030_write_cmos_sensor(0x0990, 0x0038);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2318);   //     // MCU_ADDRESS [AWB_CCM_L_9]
	AR2030_write_cmos_sensor(0x0990, 0x002C);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x231A);   //     // MCU_ADDRESS [AWB_CCM_L_10]
	AR2030_write_cmos_sensor(0x0990, 0x0038);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2318);   //     // MCU_ADDRESS [AWB_CCM_L_9]
	AR2030_write_cmos_sensor(0x0990, 0x0024);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x231A);   //     // MCU_ADDRESS [AWB_CCM_L_10]
	AR2030_write_cmos_sensor(0x0990, 0x0038);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x231C);   //     // MCU_ADDRESS [AWB_CCM_RL_0]
	AR2030_write_cmos_sensor(0x0990, 0xFFCD);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x231E);   //     // MCU_ADDRESS [AWB_CCM_RL_1]
	AR2030_write_cmos_sensor(0x0990, 0x0023);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2320);   //     // MCU_ADDRESS [AWB_CCM_RL_2]
	AR2030_write_cmos_sensor(0x0990, 0x0010);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2322);   //     // MCU_ADDRESS [AWB_CCM_RL_3]
	AR2030_write_cmos_sensor(0x0990, 0x0026);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2324);   //     // MCU_ADDRESS [AWB_CCM_RL_4]
	AR2030_write_cmos_sensor(0x0990, 0xFFE9);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2326);   //     // MCU_ADDRESS [AWB_CCM_RL_5]
	AR2030_write_cmos_sensor(0x0990, 0xFFF1);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2328);   //     // MCU_ADDRESS [AWB_CCM_RL_6]
	AR2030_write_cmos_sensor(0x0990, 0x003A);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x232A);   //     // MCU_ADDRESS [AWB_CCM_RL_7]
	AR2030_write_cmos_sensor(0x0990, 0x005D);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x232C);   //     // MCU_ADDRESS [AWB_CCM_RL_8]
	AR2030_write_cmos_sensor(0x0990, 0xFF69);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x232E);   //     // MCU_ADDRESS [AWB_CCM_RL_9]
	AR2030_write_cmos_sensor(0x0990, 0x000C);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2330);   //     // MCU_ADDRESS [AWB_CCM_RL_10]
	AR2030_write_cmos_sensor(0x0990, 0xFFE4);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x232E);   //     // MCU_ADDRESS [AWB_CCM_RL_9]
	AR2030_write_cmos_sensor(0x0990, 0x000C);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2330);   //     // MCU_ADDRESS [AWB_CCM_RL_10]
	AR2030_write_cmos_sensor(0x0990, 0xFFF4);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x232E);   //     // MCU_ADDRESS [AWB_CCM_RL_9]
	AR2030_write_cmos_sensor(0x0990, 0x000A);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2330);   //     // MCU_ADDRESS [AWB_CCM_RL_10]
	AR2030_write_cmos_sensor(0x0990, 0xFFF4);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x232E);   //     // MCU_ADDRESS [AWB_CCM_RL_9]
	AR2030_write_cmos_sensor(0x0990, 0x0006);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2330);   //     // MCU_ADDRESS [AWB_CCM_RL_10]
	AR2030_write_cmos_sensor(0x0990, 0xFFF4);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x232E);   //     // MCU_ADDRESS [AWB_CCM_RL_9]
	AR2030_write_cmos_sensor(0x0990, 0xFFFC);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2330);   //     // MCU_ADDRESS [AWB_CCM_RL_10]
	AR2030_write_cmos_sensor(0x0990, 0xFFF4);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x232E);   //     // MCU_ADDRESS [AWB_CCM_RL_9]
	AR2030_write_cmos_sensor(0x0990, 0x0004);   //     // MCU_DATA_0
	AR2030_write_cmos_sensor(0x098C, 0x2330);   //     // MCU_ADDRESS [AWB_CCM_RL_10]
	AR2030_write_cmos_sensor(0x0990, 0xFFF4);   //     // MCU_DATA_0

	//[step6 - patch_rev3]
	AR2030_write_cmos_sensor( 0x098C, 0x0415  ); // AR2030_write_cmos_sensor( 0x098C, 0x0415
	AR2030_write_cmos_sensor( 0x0990, 0xF601  );
	AR2030_write_cmos_sensor( 0x0992, 0x42C1  );
	AR2030_write_cmos_sensor( 0x0994, 0x0326  );
	AR2030_write_cmos_sensor( 0x0996, 0x11F6  );
	AR2030_write_cmos_sensor( 0x0998, 0x0143  );
	AR2030_write_cmos_sensor( 0x099A, 0xC104  );
	AR2030_write_cmos_sensor( 0x099C, 0x260A  );
	AR2030_write_cmos_sensor( 0x099E, 0xCC04  );
	AR2030_write_cmos_sensor( 0x098C, 0x0425  ); // AR2030_write_cmos_sensor( 0x098C, 0x0425
	AR2030_write_cmos_sensor( 0x0990, 0x33BD  );
	AR2030_write_cmos_sensor( 0x0992, 0xA362  );
	AR2030_write_cmos_sensor( 0x0994, 0xBD04  );
	AR2030_write_cmos_sensor( 0x0996, 0x3339  );
	AR2030_write_cmos_sensor( 0x0998, 0xC6FF  );
	AR2030_write_cmos_sensor( 0x099A, 0xF701  );
	AR2030_write_cmos_sensor( 0x099C, 0x6439  );
	AR2030_write_cmos_sensor( 0x099E, 0xDE5D   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0435  ); // AR2030_write_cmos_sensor( 0x098C, 0x0435
	AR2030_write_cmos_sensor( 0x0990, 0x18CE   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x0325   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xCC00   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x27BD   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xC2B8   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xCC04   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xBDFD   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x033B   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0445  ); // AR2030_write_cmos_sensor( 0x098C, 0x0445
	AR2030_write_cmos_sensor( 0x0990, 0xCC06   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x6BFD   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x032F   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xCC03   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x25DD   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x5DC6   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x1ED7   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x6CD7   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0455  ); // AR2030_write_cmos_sensor( 0x098C, 0x0455
	AR2030_write_cmos_sensor( 0x0990, 0x6D5F   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xD76E   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xD78D   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x8620   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x977A   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xD77B   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x979A   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xC621   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0465  ); // AR2030_write_cmos_sensor( 0x098C, 0x0465
	AR2030_write_cmos_sensor( 0x0990, 0xD79B   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xFE01   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x6918   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xCE03   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x4DCC   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x0013   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xBDC2   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xB8CC   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0475  ); // AR2030_write_cmos_sensor( 0x098C, 0x0475
	AR2030_write_cmos_sensor( 0x0990, 0x05E9   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xFD03   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x4FCC   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x034D   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xFD01   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x69FE   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x02BD   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x18CE   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0485  ); // AR2030_write_cmos_sensor( 0x098C, 0x0485
	AR2030_write_cmos_sensor( 0x0990, 0x0361   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xCC00   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x11BD   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xC2B8   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xCC06   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x28FD   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x036F   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xCC03   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0495  ); // AR2030_write_cmos_sensor( 0x098C, 0x0495
	AR2030_write_cmos_sensor( 0x0990, 0x61FD   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x02BD   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xDE00   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x18CE   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x00C2   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xCC00   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x37BD   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xC2B8   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x04A5  ); // AR2030_write_cmos_sensor( 0x098C, 0x04A5
	AR2030_write_cmos_sensor( 0x0990, 0xCC06   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x4FDD   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xE6CC   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x00C2   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xDD00   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xC601   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xF701   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x64C6   ); 

	AR2030_write_cmos_sensor( 0x098C, 0x04B5  ); // AR2030_write_cmos_sensor( 0x098C, 0x04B5
	AR2030_write_cmos_sensor( 0x0990, 0x05F7   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x0165   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x7F01   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x6639   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x373C   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x3C3C   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x3C3C   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x30EC   ); 

	AR2030_write_cmos_sensor( 0x098C, 0x04C5  ); // AR2030_write_cmos_sensor( 0x098C, 0x04C5
	AR2030_write_cmos_sensor( 0x0990, 0x11ED   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x02EC   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x0FED   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x008F   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x30ED   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x04EC   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x0DEE   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x04BD   );

	AR2030_write_cmos_sensor( 0x098C, 0x04D5  ); // AR2030_write_cmos_sensor( 0x098C, 0x04D5
	AR2030_write_cmos_sensor( 0x0990, 0xA406   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x30EC   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x02ED   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x06FC   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x10C0   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x2705   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xCCFF   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xFFED   ); 

	AR2030_write_cmos_sensor( 0x098C, 0x04E5  ); // AR2030_write_cmos_sensor( 0x098C, 0x04E5
	AR2030_write_cmos_sensor( 0x0990, 0x06F6    ); 
	AR2030_write_cmos_sensor( 0x0992, 0x0256    ); 
	AR2030_write_cmos_sensor( 0x0994, 0x8616    ); 
	AR2030_write_cmos_sensor( 0x0996, 0x3DC3    ); 
	AR2030_write_cmos_sensor( 0x0998, 0x0261    ); 
	AR2030_write_cmos_sensor( 0x099A, 0x8FE6    ); 
	AR2030_write_cmos_sensor( 0x099C, 0x09C4    ); 
	AR2030_write_cmos_sensor( 0x099E, 0x07C1    ); 

	AR2030_write_cmos_sensor( 0x098C, 0x04F5  ); // AR2030_write_cmos_sensor( 0x098C, 0x04F5
	AR2030_write_cmos_sensor( 0x0990, 0x0226   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x1DFC   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x10C2   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x30ED   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x02FC   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x10C0   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xED00   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xC602   ); 

	AR2030_write_cmos_sensor( 0x098C, 0x0505   );    // AR2030_write_cmos_sensor( 0x098C, 0x0505
	AR2030_write_cmos_sensor( 0x0990, 0xBDC2   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x5330   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xEC00   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xFD10   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xC0EC   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x02FD   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x10C2   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x201B   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0515   );     // AR2030_write_cmos_sensor( 0x098C, 0x0515
	AR2030_write_cmos_sensor( 0x0990, 0xFC10   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xC230   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xED02   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xFC10   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xC0ED   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x00C6   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x01BD   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xC253   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0525   );     // AR2030_write_cmos_sensor( 0x098C, 0x0525
	AR2030_write_cmos_sensor( 0x0990, 0x30EC   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x00FD   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x10C0   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xEC02   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xFD10   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xC2C6   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x80D7   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x85C6   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0535   );     // AR2030_write_cmos_sensor( 0x098C, 0x0535
	AR2030_write_cmos_sensor( 0x0990, 0x40F7   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x10C4   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xF602   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x5686   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x163D   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xC302   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x618F   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xEC14   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0545   );     // AR2030_write_cmos_sensor( 0x098C, 0x0545
	AR2030_write_cmos_sensor( 0x0990, 0xFD10   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xC501   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x0101   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x0101   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xFC10   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xC2DD   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x7FFC   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x10C7   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0555   );     // AR2030_write_cmos_sensor( 0x098C, 0x0555
	AR2030_write_cmos_sensor( 0x0990, 0xDD76   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xF602   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x5686   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x163D   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xC302   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x618F   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xEC14   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x939F   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0565   );     // AR2030_write_cmos_sensor( 0x098C, 0x0565
	AR2030_write_cmos_sensor( 0x0990, 0x30ED   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x08DC   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x7693   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x9D25   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x08F6   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x02BC   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x4F93   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x7F23   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0575   );     // AR2030_write_cmos_sensor( 0x098C, 0x0575
	AR2030_write_cmos_sensor( 0x0990, 0x3DF6   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x02BC   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x4F93   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x7F23   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x06F6   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x02BC   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x4FDD   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x7FDC   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0585   );     // AR2030_write_cmos_sensor( 0x098C, 0x0585
	AR2030_write_cmos_sensor( 0x0990, 0x9DDD   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x76F6   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x02BC   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x4F93   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x7F26   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x0FE6   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x0AC1   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x0226   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0595   );     // AR2030_write_cmos_sensor( 0x098C, 0x0595
	AR2030_write_cmos_sensor( 0x0990, 0x09D6   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x85C1   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x8026   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x0314   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x7401   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xF602   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xBC4F   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x937F   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x05A5   );     // AR2030_write_cmos_sensor( 0x098C, 0x05A5
	AR2030_write_cmos_sensor( 0x0990, 0x2416   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xDE7F   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x09DF   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x7F30   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xEC08   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xDD76   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x200A   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xDC76   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x05B5   );     // AR2030_write_cmos_sensor( 0x098C, 0x05B5
	AR2030_write_cmos_sensor( 0x0990, 0xA308   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x2304   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xEC08   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xDD76   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x1274   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x0122   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xDE5D   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xEE14   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x05C5   );     // AR2030_write_cmos_sensor( 0x098C, 0x05C5
	AR2030_write_cmos_sensor( 0x0990, 0xAD00   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x30ED   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x11EC   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x06ED   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x02CC   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x0080   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xED00   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x8F30   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x05D5   );     // AR2030_write_cmos_sensor( 0x098C, 0x05D5
	AR2030_write_cmos_sensor( 0x0990, 0xED04   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xEC11   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xEE04   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xBDA4   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x0630   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xE603   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xD785   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x30C6   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x05E5   );     // AR2030_write_cmos_sensor( 0x098C, 0x05E5
	AR2030_write_cmos_sensor( 0x0990, 0x0B3A   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x3539   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x3C3C   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x3C34   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xCC32   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x3EBD   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xA558   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x30ED   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x05F5   );     // AR2030_write_cmos_sensor( 0x098C, 0x05F5
	AR2030_write_cmos_sensor( 0x0990, 0x04BD   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xB2D7   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x30E7   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x06CC   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x323E   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xED00   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xEC04   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xBDA5   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0605   );     // AR2030_write_cmos_sensor( 0x098C, 0x0605
	AR2030_write_cmos_sensor( 0x0990, 0x44CC   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x3244   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xBDA5   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x585F   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x30ED   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x02CC   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x3244   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xED00   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0615   );     // AR2030_write_cmos_sensor( 0x098C, 0x0615
	AR2030_write_cmos_sensor( 0x0990, 0xF601   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xD54F   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xEA03   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xAA02   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xBDA5   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x4430   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xE606   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x3838   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0625   );     // AR2030_write_cmos_sensor( 0x098C, 0x0625
	AR2030_write_cmos_sensor( 0x0990, 0x3831   ); 
	AR2030_write_cmos_sensor( 0x0992, 0x39BD   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xD661   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xF602   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xF4C1   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x0126   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x0BFE   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x02BD   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0635   );     // AR2030_write_cmos_sensor( 0x098C, 0x0635
	AR2030_write_cmos_sensor( 0x0990, 0xEE10   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xFC02   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xF5AD   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x0039   ); 
	AR2030_write_cmos_sensor( 0x0998, 0xF602   ); 
	AR2030_write_cmos_sensor( 0x099A, 0xF4C1   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x0226   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x0AFE   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0645   );     // AR2030_write_cmos_sensor( 0x098C, 0x0645
	AR2030_write_cmos_sensor( 0x0990, 0x02BD   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xEE10   ); 
	AR2030_write_cmos_sensor( 0x0994, 0xFC02   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xF7AD   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x0039   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x3CBD   ); 
	AR2030_write_cmos_sensor( 0x099C, 0xB059   ); 
	AR2030_write_cmos_sensor( 0x099E, 0xCC00   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0655   );     // AR2030_write_cmos_sensor( 0x098C, 0x0655
	AR2030_write_cmos_sensor( 0x0990, 0x28BD   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xA558   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x8300   ); 
	AR2030_write_cmos_sensor( 0x0996, 0x0027   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x0BCC   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x0026   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x30ED   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x00C6   ); 
	AR2030_write_cmos_sensor( 0x098C, 0x0665   );     // AR2030_write_cmos_sensor( 0x098C, 0x0665
	AR2030_write_cmos_sensor( 0x0990, 0x03BD   ); 
	AR2030_write_cmos_sensor( 0x0992, 0xA544   ); 
	AR2030_write_cmos_sensor( 0x0994, 0x3839   ); 
	AR2030_write_cmos_sensor( 0x0996, 0xBDD9   ); 
	AR2030_write_cmos_sensor( 0x0998, 0x42D6   ); 
	AR2030_write_cmos_sensor( 0x099A, 0x9ACB   ); 
	AR2030_write_cmos_sensor( 0x099C, 0x01D7   ); 
	AR2030_write_cmos_sensor( 0x099E, 0x9B39   ); 


	AR2030_write_cmos_sensor( 0x098C, 0x2006  ); // MCU_ADDRESS [MON_ARG1]
	AR2030_write_cmos_sensor( 0x0990, 0x0415  ); // MCU_DATA_0
	AR2030_write_cmos_sensor( 0x098C, 0xA005  ); // MCU_ADDRESS [MON_CMD]
	AR2030_write_cmos_sensor( 0x0990, 0x0001  ); // MCU_DATA_0
	mdelay(10);//delay=10
	//[step7 - streaming on]
	AR2030_write_cmos_sensor(0x0018, 0x0028);    // STANDBY_CONTROL
	mdelay(20);

	// mdelay(100);//delay=300  
	//  **POLL_FIELD=SEQ_STATE,!=3,DELAY=10,TIMEOUT=100
	//  **polling \u00CF\u00C8\u00D0\u00B40x98C, 0xA104,read 0x0990\u00B5\u00C4\u00D6\u00B5\u00B5\u00B1\u00D6\u00B5\u00CE\u00AA0x0003\u00CA\u00B1\u00A3\u00AC\u00BF\u00C9\u00D2\u00D4\u00BD\u00F8\u00D0\u00D0\u00CF\u00C2\u00D2\u00BB\u00B2\u00BD\u00B2\u00D9\u00D7\u00F7\u00A1\u00A3\u00B2\u00BB\u00D0\u00D0\u00BE\u00CDdelay10ms,\u00BD\u00F8\u00D0\u00D010\u00B4\u00CE
	AR2030_set_cmd(6); /* refresh mode */
	AR2030_set_cmd(5); /* refresh */
}

 

/*************************************************************************
* FUNCTION
*    AR2030_gain2shutter
*
* DESCRIPTION
*    This function compensate gain to shutter to reduce the noise in capture mode.
*
* PARAMETERS
*    old shutter
*
* RETURNS
*    new shutter
*
* LOCAL AFFECTED
*
*************************************************************************/

// < NOT USE >
static kal_uint16 AR2030_gain2shutter(kal_bool enable)
{
  static kal_uint16 gain_r, gain_gr, gain_gb, gain_b;
  kal_uint8 mul = 2; /* multiple of gain */
  
  if (enable)
  {
    gain_gr = AR2030_read_cmos_sensor(0x32D6);
    /* convert digital gain(isp gain. sensor core digital gain no use) */
    while (gain_gr >= mul * 0x80) mul++;
    if (--mul > 1)
    {
      gain_r = AR2030_read_cmos_sensor(0x32D4);
      gain_gb = AR2030_read_cmos_sensor(0x32D8);
      gain_b = AR2030_read_cmos_sensor(0x32DA);
      AR2030_write_cmos_sensor(0x32D4, AR2030_half_adjust(gain_r, mul));
      AR2030_write_cmos_sensor(0x32D6, AR2030_half_adjust(gain_gr, mul));
      AR2030_write_cmos_sensor(0x32D8, AR2030_half_adjust(gain_gr, mul));
      AR2030_write_cmos_sensor(0x32DA, AR2030_half_adjust(gain_b * gain_gr, mul * gain_gb));
      
      return mul;
    }
    gain_gr = 0;
  }
  if (gain_r)  AR2030_write_cmos_sensor(0x32D4, gain_r);
  if (gain_gr) AR2030_write_cmos_sensor(0x32D6, gain_gr);
  if (gain_gb) AR2030_write_cmos_sensor(0x32D8, gain_gr);
  if (gain_b)  AR2030_write_cmos_sensor(0x32DA, gain_b);
  gain_r = gain_gr = gain_gb = gain_b = 0;
  
  return 1;
}

/*************************************************************************
* FUNCTION
*    AR2030_power_on
*
* DESCRIPTION
*    This function power on CMOS sensor and check sensor id
*
* PARAMETERS
*    None
*
* RETURNS
*    sensor ID
*
* LOCAL AFFECTED
*
*************************************************************************/
static kal_uint16 AR2030_power_on(void)
{
	kal_uint16 sensor_id;

	// 26MHz CLKIN, 52 MHz PLL out
	AR2030_sensor.pclk = 52000000;
	AR2030_sensor.write_id = AR2030_SLV1_WRITE_ID;
	AR2030_sensor.read_id = AR2030_SLV1_READ_ID;

	sensor_id = AR2030_read_cmos_sensor(0x0000);
	SENSORDB( "ZLiao_SENSOR ID: %x", sensor_id);

	if (sensor_id != AR2030_SENSOR_ID) /* READ SENSOR ID */
	{
		AR2030_sensor.write_id = AR2030_SLV2_WRITE_ID;
		AR2030_sensor.read_id = AR2030_SLV2_READ_ID;

		sensor_id = AR2030_read_cmos_sensor(0x0000);
		if (sensor_id != AR2030_SENSOR_ID)
		{
		#ifdef AR2030YUV_DEBUG
			SENSORDB( "SENSOR ID: %x", sensor_id);
		#endif
		}
	}

	return sensor_id;
}

/*************************************************************************
* FUNCTION
*   AR2030_NightMode
*
* DESCRIPTION
*   This function switch on/off night mode of AR2030.
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void AR2030_NightMode(kal_bool bEnable)
{    
	SENSORDB("zhijie AR2030_NightMode ");
	kal_uint8 y_rgb_offset, awb_gain_max_r;
	kal_uint16 limit_of_digital_gain;

	if (AR2030_sensor.night_mode == bEnable ) return;
	AR2030_sensor.night_mode = bEnable;
	if (AR2030_sensor.night_mode)
	{
		//AR2030_write_XDMA(0xAB37, 0x02); /* use GAMMA TABLE B */
		//y_rgb_offset = 20;
		//awb_gain_max_r = 0xA0;
		//limit_of_digital_gain = 0x0100;
		AR2030_write_XDMA(0xA20C, 0x0018); 	// MCU_ADDRESS [AE_MAX_INDEX]
		AR2030_write_XDMA(0xA215, 0x0018); 	// MCU_ADDRESS [AE_INDEX_TH23]
		AR2030_write_XDMA(0xA103, 0x0006); 	// MCU_ADDRESS [SEQ_CMD]
	}
	else
	{
		//AR2030_write_XDMA(0xAB37, 0x01); /* use GAMMA TABLE A */
		//y_rgb_offset = 0;
		//awb_gain_max_r = 0xC8;
		//limit_of_digital_gain = 0x00EE;
		AR2030_write_XDMA(0xA20C, 0x0008); 	// MCU_ADDRESS [AE_MAX_INDEX]
		AR2030_write_XDMA(0xA215, 0x0008); 	// MCU_ADDRESS [AE_INDEX_TH23]
		AR2030_write_XDMA(0xA103, 0x0006); 	// MCU_ADDRESS [SEQ_CMD]
	}
	//AR2030_write_XDMA(0x2212, limit_of_digital_gain);
	//AR2030_write_XDMA(0xA34B, awb_gain_max_r);
	//AR2030_write_XDMA(0xA75D, y_rgb_offset); /* mode_y_rgb_offset_a */
	//AR2030_write_XDMA(0xA75E, y_rgb_offset); /* mode_y_rgb_offset_b */

	//AR2030_cal_fps(); /* need cal new fps */
}   /* AR2030_NightMode */


/*************************************************************************
* FUNCTION
*    AR2030_GetSensorID
*
* DESCRIPTION
*    This function get the sensor ID
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 AR2030_GetSensorID(kal_uint32 *sensorID)

{
    SENSORDB("[Enter]:AR2030 Open func ");

    *sensorID = AR2030_power_on();
    SENSORDB("[AR2030]ZL AR2030_GetSensorID\n");

	if (AR2030_SENSOR_ID != *sensorID  ) 
	{
		SENSORDB("[AR2030]ZLZL_Error:read sensor ID fail\n");
		*sensorID = 0xFFFFFFFF; 
		return ERROR_SENSOR_CONNECT_FAIL;
	}

	return ERROR_NONE;    
}   /* AR2030Open  */


/*****************************************************************************/
/* Windows Mobile Sensor Interface */
/*****************************************************************************/
/*************************************************************************
* FUNCTION
*    AR2030Open
*
* DESCRIPTION
*    This function initialize the registers of CMOS sensor
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 AR2030Open(void)
{
    volatile signed char i;
    kal_uint16 sensor_id=0;
	AR2030_sensor.pv_mode=KAL_FALSE ;
    SENSORDB("zhijie AR2030Open ");
    Sleep(20);
    if (AR2030_SENSOR_ID != AR2030_power_on())
        return ERROR_SENSOR_CONNECT_FAIL;
    
    SENSORDB("[out]zhijie AR2030Open ");
	AR2030_initial_setting();
	//AR2030_initial_setting22(); //shere
    //AR2030_set_cmd(1); /* seq_cmd: goto preview mode */

    //iowrite32(0xe<<12, 0xF0001500);
    
    return ERROR_NONE;
}    /* AR2030Open() */

/*************************************************************************
* FUNCTION
*    AR2030Close
*
* DESCRIPTION
*    This function is to turn off sensor module power.
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 AR2030Close(void)
{
    return ERROR_NONE;
}    /* AR2030Close() */

/*************************************************************************
* FUNCTION
*    AR2030Preview
*
* DESCRIPTION
*    This function start the sensor preview.
*
* PARAMETERS
*    *image_window : address pointer of pixel numbers in one period of HSYNC
*  *sensor_config_data : address pointer of line numbers in one period of VSYNC
*
* RETURNS
*    None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 AR2030Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
                      MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
    SENSORDB("Camera AR2030Preview\n");
/*    
	AR2030_read_XDMA(0xA115);
    AR2030_read_XDMA(0xA103);
    printk("AR2030_read_cmos_sensor(0X3028)=%x\n",AR2030_read_cmos_sensor(0X3028));
    printk("AR2030_read_cmos_sensor(0X302a)=%x\n",AR2030_read_cmos_sensor(0X302a));
    printk("AR2030_read_cmos_sensor(0X302c)=%x\n",AR2030_read_cmos_sensor(0X302c));
    printk("AR2030_read_cmos_sensor(0X302e)=%x\n",AR2030_read_cmos_sensor(0X302e));
    printk("AR2030_read_cmos_sensor(0X3030)=%x\n",AR2030_read_cmos_sensor(0X3030));
    printk("captureAR2030_read_cmos_sensor(0X33f4)=%x\n",AR2030_read_cmos_sensor(0X33f4));
    {0x98C, 0xA115},
    {0x0990, 0x0000},
    {0x98C, 0xA103},    //Refresh Sequencer
    {0x0990, 0x0001},
*/
    AR2030_write_cmos_sensor(0x98C, 0xA115);           // MCU_ADDRESS [SEQ_CAP_MODE]
    AR2030_write_cmos_sensor(0x0990, 0x0000);            // MCU_DATA_0
    AR2030_write_cmos_sensor(0x98C, 0xA103);           // MCU_ADDRESS [SEQ_CMD]
    AR2030_write_cmos_sensor(0x0990, 0x0001);            // MCU_DATA_0    

    AR2030_write_cmos_sensor(0x301A, 0x12dc);//, MSM_CAMERA_I2C_SET_WORD_MASK},
    
	//    AR2030_set_cmd(1);
	AR2030_sensor.pv_mode=KAL_TRUE ;
	//    AR2030_write_cmos_sensor(0x33F4, 0x004B);           // 
	    
	return ERROR_NONE;
}    /* AR2030Preview() */

UINT32 AR2030Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
                      MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	SENSORDB("Camera Capture\r\n");
/*    
	SENSORDB("Camera Capture\r\n");
    AR2030_read_XDMA(0xA115);
    AR2030_read_XDMA(0xA103);
    printk("AR2030_read_cmos_sensor(0X3028)=%x\n",AR2030_read_cmos_sensor(0X3028));
    printk("AR2030_read_cmos_sensor(0X302a)=%x\n",AR2030_read_cmos_sensor(0X302a));
    printk("AR2030_read_cmos_sensor(0X302c)=%x\n",AR2030_read_cmos_sensor(0X302c));
    printk("AR2030_read_cmos_sensor(0X302e)=%x\n",AR2030_read_cmos_sensor(0X302e));
    printk("AR2030_read_cmos_sensor(0X3030)=%x\n",AR2030_read_cmos_sensor(0X3030));
    printk("captureAR2030_read_cmos_sensor(0X33f4)=%x\n",AR2030_read_cmos_sensor(0X33f4));
    {0x98C, 0xA115},
    {0x0990, 0x0002},
    {0x98C, 0xA103},    //Refresh Sequencer
    {0x0990, 0x0002},
*/
    
    AR2030_write_cmos_sensor(0x098C, 0xA115);           // MCU_ADDRESS [SEQ_CAP_MODE]
    AR2030_write_cmos_sensor(0x0990, 0x0002);            // MCU_DATA_0
    AR2030_write_cmos_sensor(0x098C, 0xA103);           // MCU_ADDRESS [SEQ_CMD]
    AR2030_write_cmos_sensor(0x0990, 0x0002);            // MCU_DATA_0    
    
	//AR2030_write_cmos_sensor(0x33F4, 0x004B);           // 
    
	return ERROR_NONE;
}    /* AR2030Capture() */

UINT32 AR2030GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
    SENSORDB("[AR2030YUV]AR2030GetResolution zhijie1111111\n");

    pSensorResolution->SensorFullWidth=AR2030_IMAGE_SENSOR_FULL_WIDTH;  //modify by yanxu
    pSensorResolution->SensorFullHeight=AR2030_IMAGE_SENSOR_FULL_HEIGHT;
    
    pSensorResolution->SensorVideoWidth = AR2030_IMAGE_SENSOR_FULL_WIDTH;
    pSensorResolution->SensorVideoHeight = AR2030_IMAGE_SENSOR_FULL_HEIGHT;

	switch(AR2030_currentScenarioId)
	{
	case MSDK_SCENARIO_ID_CAMERA_ZSD:
		pSensorResolution->SensorPreviewWidth=AR2030_IMAGE_SENSOR_FULL_WIDTH;
		pSensorResolution->SensorPreviewHeight=AR2030_IMAGE_SENSOR_FULL_HEIGHT;
		break;

	default:
		pSensorResolution->SensorPreviewWidth=AR2030_IMAGE_SENSOR_PV_WIDTH_DRV;
		pSensorResolution->SensorPreviewHeight=AR2030_IMAGE_SENSOR_PV_HEIGHT_DRV;
		break;
	}

	//SENSORDB("fullwidth=%d fullheight=%d,previewwidth=%d,previewheight=%d\n",pSensorResolution->SensorFullWidth,
	//pSensorResolution->SensorFullHeight,pSensorResolution->SensorPreviewWidth,pSensorResolution->SensorPreviewHeight    );

    return ERROR_NONE;
}    /* AR2030GetResolution() */

UINT32 AR2030GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
                      MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
                      MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
    SENSORDB("[AR2030YUV]AR2030GetInfo zhijie111111 ScenarioId=%d\n",ScenarioId);

    pSensorInfo->SensorPreviewResolutionX=AR2030_IMAGE_SENSOR_PV_WIDTH_DRV;//AR2030_IMAGE_SENSOR_FULL_WIDTH;
    pSensorInfo->SensorPreviewResolutionY=AR2030_IMAGE_SENSOR_PV_HEIGHT_DRV;//AR2030_IMAGE_SENSOR_FULL_HEIGHT;
    pSensorInfo->SensorFullResolutionX=AR2030_IMAGE_SENSOR_FULL_WIDTH;
    pSensorInfo->SensorFullResolutionY=AR2030_IMAGE_SENSOR_FULL_HEIGHT;

    pSensorInfo->SensorCameraPreviewFrameRate=30;
    pSensorInfo->SensorVideoFrameRate=30;
    pSensorInfo->SensorStillCaptureFrameRate=10;
    pSensorInfo->SensorWebCamCaptureFrameRate=15;
    pSensorInfo->SensorResetActiveHigh=FALSE;
    pSensorInfo->SensorResetDelayCount=1;
    pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_VYUY;//SENSOR_OUTPUT_FORMAT_YVYU;//SENSOR_OUTPUT_FORMAT_UYVY;// SENSOR_OUTPUT_FORMAT_YUYV  SENSOR_OUTPUT_FORMAT_UYVY  SENSOR_OUTPUT_FORMAT_YVYU SENSOR_OUTPUT_FORMAT_VYUY
    pSensorInfo->SensorClockPolarity = SENSOR_CLOCK_POLARITY_LOW;//SENSOR_CLOCK_POLARITY_HIGH;    /*??? */
    pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
    pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;      //HSYNC HIGH valid
    pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW; //SENSOR_CLOCK_POLARITY_HIGH;      //VSYNC LOW valid
    pSensorInfo->SensorInterruptDelayLines = 1;
    pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_MIPI;    

    pSensorInfo->CaptureDelayFrame = 2; 
    pSensorInfo->PreviewDelayFrame = 2; 
    pSensorInfo->VideoDelayFrame = 2;         
    pSensorInfo->SensorMasterClockSwitch = 0; 
    pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;         

    pSensorInfo->YUVAwbDelayFrame = 2;
    pSensorInfo->YUVEffectDelayFrame = 2;

    switch (ScenarioId)
    {
    case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
    case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
        //case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
        pSensorInfo->SensorClockFreq=24;        
        pSensorInfo->SensorClockDividCount=    3;
        pSensorInfo->SensorClockRisingCount= 0;
        pSensorInfo->SensorClockFallingCount= 2;
        pSensorInfo->SensorPixelClockCount= 3;
        pSensorInfo->SensorDataLatchCount= 2;
        pSensorInfo->SensorGrabStartX = 2; 
        pSensorInfo->SensorGrabStartY = 2;    

        pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_1_LANE;            
        pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0; 
        pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount = 14; 
        pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
        pSensorInfo->SensorWidthSampling = 0;  // 0 is default 1x
        pSensorInfo->SensorHightSampling = 0;   // 0 is default 1x 
        break;
	case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
        //case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
        pSensorInfo->SensorClockFreq=24;        
        pSensorInfo->SensorClockDividCount=  3;
        pSensorInfo->SensorClockRisingCount= 0;
        pSensorInfo->SensorClockFallingCount= 2;
        pSensorInfo->SensorPixelClockCount= 3;
        pSensorInfo->SensorDataLatchCount= 2;
        pSensorInfo->SensorGrabStartX = 2; 
        pSensorInfo->SensorGrabStartY = 2;    

		pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_1_LANE;            
		pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0; 
		pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount = 14;//0; 
		pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0; 
		pSensorInfo->SensorWidthSampling = 0;  // 0 is default 1x
		pSensorInfo->SensorHightSampling = 0;   // 0 is default 1x    
        break;
		
	default:
		pSensorInfo->SensorClockFreq=24;            
		pSensorInfo->SensorClockDividCount=    3;
		pSensorInfo->SensorClockRisingCount= 0;
		pSensorInfo->SensorClockFallingCount= 2;
		pSensorInfo->SensorPixelClockCount= 3;
		pSensorInfo->SensorDataLatchCount= 2;
		pSensorInfo->SensorGrabStartX = 2; 
		pSensorInfo->SensorGrabStartY = 2;    
        break;
    }
    //AR2030_PixelClockDivider=pSensorInfo->SensorPixelClockCount;
    memcpy(pSensorConfigData, &AR2030SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
    return ERROR_NONE;
}    /* AR2030GetInfo() */


UINT32 AR2030Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
                      MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
    SENSORDB("[AR2030YUV]AR2030Control zhijie111111,ScenarioId=%d\n",ScenarioId);

	AR2030_currentScenarioId = ScenarioId;
    switch (ScenarioId)
    {
    case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
    case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
    //case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
        AR2030Preview(pImageWindow, pSensorConfigData);
        break;
        
    case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
	case MSDK_SCENARIO_ID_CAMERA_ZSD:
    //case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
        AR2030Capture(pImageWindow, pSensorConfigData);
        break;
        
    default:
        break; 
    }

    return TRUE;
}    /* AR2030Control() */


/* [TC] YUV sensor */    
BOOL AR2030_set_param_wb(UINT16 para)
{
    SENSORDB("[AR2030YUV]AR2030_set_param_wb para=%d\n",para);
    
    kal_uint8 awb_ccm_position;
      
    if (AR2030_sensor.wb == para) return KAL_TRUE;
      AR2030_sensor.wb = para;

    switch (AR2030_sensor.wb)
    {
    case AWB_MODE_AUTO:   // enable AWB
        //AR2030_write_XDMA(0xA355, 0x02);
        AR2030_write_XDMA(0xA34A, 0x59); 	// MCU_ADDRESS [AWB_GAIN_MIN]
				AR2030_write_XDMA(0xA34B, 0xC8); 	// MCU_ADDRESS [AWB_GAIN_MAX]
				AR2030_write_XDMA(0xA34C, 0x59); 	// MCU_ADDRESS [AWB_GAINMIN_B]
				AR2030_write_XDMA(0xA34D, 0xA6); 	// MCU_ADDRESS [AWB_GAINMAX_B]
				AR2030_write_XDMA(0xA351, 0x00); 	// MCU_ADDRESS [AWB_CCM_POSITION_MIN]
				AR2030_write_XDMA(0xA352, 0x7F); 	// MCU_ADDRESS [AWB_CCM_POSITION_MAX]
				AR2030_write_XDMA(0xA103, 0x05); 	// MCU_ADDRESS [SEQ_CMD]
        break;

    case AWB_MODE_CLOUDY_DAYLIGHT:
        //awb_ccm_position = 0x6F;
        AR2030_write_XDMA(0xA34A, 0xD0); 	// MCU_ADDRESS [AWB_GAIN_MIN]
				AR2030_write_XDMA(0xA34B, 0xD0); 	// MCU_ADDRESS [AWB_GAIN_MAX]
				AR2030_write_XDMA(0xA34C, 0x56); 	// MCU_ADDRESS [AWB_GAINMIN_B]
				AR2030_write_XDMA(0xA34D, 0x56); 	// MCU_ADDRESS [AWB_GAINMAX_B]
				AR2030_write_XDMA(0xA351, 0x7f); 	// MCU_ADDRESS [AWB_CCM_POSITION_MIN]
				AR2030_write_XDMA(0xA352, 0x7F); 	// MCU_ADDRESS [AWB_CCM_POSITION_MAX]
				AR2030_write_XDMA(0xA103, 0x05); 	// MCU_ADDRESS [SEQ_CMD]
        break;
        
    case AWB_MODE_DAYLIGHT:
        //awb_ccm_position = 0x7F;
				AR2030_write_XDMA(0xA34A, 0xC2); 	// MCU_ADDRESS [AWB_GAIN_MIN]
				AR2030_write_XDMA(0xA34B, 0xc4); 	// MCU_ADDRESS [AWB_GAIN_MAX]
				AR2030_write_XDMA(0xA34C, 0x5d); 	// MCU_ADDRESS [AWB_GAINMIN_B]
				AR2030_write_XDMA(0xA34D, 0x5f); 	// MCU_ADDRESS [AWB_GAINMAX_B]
				AR2030_write_XDMA(0xA351, 0x7f); 	// MCU_ADDRESS [AWB_CCM_POSITION_MIN]
				AR2030_write_XDMA(0xA352, 0x7F); 	// MCU_ADDRESS [AWB_CCM_POSITION_MAX]
				AR2030_write_XDMA(0xA103, 0x05); 	// MCU_ADDRESS [SEQ_CMD]
        break;
    
    case AWB_MODE_INCANDESCENT:
        //awb_ccm_position = 0x10;
        AR2030_write_XDMA(0xA34A, 0x86); 	// MCU_ADDRESS [AWB_GAIN_MIN]
				AR2030_write_XDMA(0xA34B, 0x88); 	// MCU_ADDRESS [AWB_GAIN_MAX]
				AR2030_write_XDMA(0xA34C, 0x92); 	// MCU_ADDRESS [AWB_GAINMIN_B]
				AR2030_write_XDMA(0xA34D, 0x94); 	// MCU_ADDRESS [AWB_GAINMAX_B]
				AR2030_write_XDMA(0xA351, 0x10); 	// MCU_ADDRESS [AWB_CCM_POSITION_MIN]
				AR2030_write_XDMA(0xA352, 0x10); 	// MCU_ADDRESS [AWB_CCM_POSITION_MAX]
				AR2030_write_XDMA(0xA103, 0x05); 	// MCU_ADDRESS [SEQ_CMD]
        break;
    
    case AWB_MODE_FLUORESCENT:
        //awb_ccm_position = 0x79;
        AR2030_write_XDMA(0xA34A, 0xA0); 	// MCU_ADDRESS [AWB_GAIN_MIN]
				AR2030_write_XDMA(0xA34B, 0xA0); 	// MCU_ADDRESS [AWB_GAIN_MAX]
				AR2030_write_XDMA(0xA34C, 0x7F); 	// MCU_ADDRESS [AWB_GAINMIN_B]
				AR2030_write_XDMA(0xA34D, 0x7F); 	// MCU_ADDRESS [AWB_GAINMAX_B]
				AR2030_write_XDMA(0xA351, 0x42); 	// MCU_ADDRESS [AWB_CCM_POSITION_MIN]
				AR2030_write_XDMA(0xA352, 0x42); 	// MCU_ADDRESS [AWB_CCM_POSITION_MAX]
				AR2030_write_XDMA(0xA103, 0x05); 	// MCU_ADDRESS [SEQ_CMD]

        break;
    
    case AWB_MODE_TUNGSTEN:
        //awb_ccm_position = 0x00;
        AR2030_write_XDMA(0xA34A, 0x6E); 	// MCU_ADDRESS [AWB_GAIN_MIN]
				AR2030_write_XDMA(0xA34B, 0x6E); 	// MCU_ADDRESS [AWB_GAIN_MAX]
				AR2030_write_XDMA(0xA34C, 0xB3); 	// MCU_ADDRESS [AWB_GAINMIN_B]
				AR2030_write_XDMA(0xA34D, 0xB5); 	// MCU_ADDRESS [AWB_GAINMAX_B]
				AR2030_write_XDMA(0xA351, 0x00); 	// MCU_ADDRESS [AWB_CCM_POSITION_MIN]
				AR2030_write_XDMA(0xA352, 0x00); 	// MCU_ADDRESS [AWB_CCM_POSITION_MAX]
				AR2030_write_XDMA(0xA103, 0x05); 	// MCU_ADDRESS [SEQ_CMD]
        break;
    
    default:
        return FALSE;
    }
    
    return TRUE;
} /* AR2030_set_param_wb */

BOOL AR2030_set_param_effect(UINT16 para)
{
	kal_uint8 spec_effects;

	if (AR2030_sensor.effect == para) return KAL_TRUE;
	AR2030_sensor.effect = para;

    switch (AR2030_sensor.effect)
    {
    case MEFFECT_OFF:
        //spec_effects = 0;
        AR2030_write_XDMA(0x2759, 0x6440); 	// MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
				AR2030_write_XDMA(0x275B, 0x6440);  	// MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
				AR2030_write_XDMA(0xA103, 0x05); 	// MCU_ADDRESS [SEQ_CMD]
        break;

    case MEFFECT_MONO:
        //spec_effects = 1;
        AR2030_write_XDMA(0x2759, 0x6441);   	// MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
				AR2030_write_XDMA(0x275B, 0x6441); 	// MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
				AR2030_write_XDMA(0xA103, 0x05);  	// MCU_ADDRESS [SEQ_CMD]
        break;
        
    case MEFFECT_SEPIA:
        //spec_effects = 2;
        //AR2030_write_XDMA(0x2763, 0xB023); /* mode_common_mode_settings_fx_sepia_settings */
        AR2030_write_XDMA(0x2763, 0xB023);   	// mode_common_mode_settings_fx_sepia_settings
        AR2030_write_XDMA(0x2759, 0x6442);   	// MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
				AR2030_write_XDMA(0x275B, 0x6442);   	// MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
				AR2030_write_XDMA(0xA103, 0x05);   	// MCU_ADDRESS [SEQ_CMD]
        break;

    case MEFFECT_SEPIAGREEN:
        //spec_effects = 2;
        //AR2030_write_XDMA(0x2763, 0xB0CD);
        AR2030_write_XDMA(0x2763, 0xDBDB); 	// MCU_ADDRESS [MODE_COMMONMODESETTINGS_FX_SEPIA_SETTINGS]
				AR2030_write_XDMA(0x2759, 0x6442); 	// MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
				AR2030_write_XDMA(0x275B, 0x6442); 	// MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
				AR2030_write_XDMA(0xA103, 0x05); 	// MCU_ADDRESS [SEQ_CMD]
        break;

    case MEFFECT_SEPIABLUE:
        //spec_effects = 2;
        AR2030_write_XDMA(0x2763, 0x25F0);
        AR2030_write_XDMA(0x2759, 0x6442); 	// MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
				AR2030_write_XDMA(0x275B, 0x6442); 	// MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
				AR2030_write_XDMA(0xA103, 0x05); 	// MCU_ADDRESS [SEQ_CMD]
        break;
    
    case MEFFECT_NEGATIVE:
        //spec_effects = 3;
        AR2030_write_XDMA(0x2759, 0x6443);  	// MCU_ADDRESS [MODE_SPEC_EFFECTS_A]
				AR2030_write_XDMA(0x275B, 0x6443);  	// MCU_ADDRESS [MODE_SPEC_EFFECTS_B]
				AR2030_write_XDMA(0xA103, 0x05);   	// MCU_ADDRESS [SEQ_CMD]
        break;
  
    default:
        return FALSE;
    }
	
    return TRUE;
} /* AR2030_set_param_effect */

BOOL AR2030_set_param_banding(UINT16 para)
{
    if (AR2030_sensor.banding == para) return KAL_TRUE;
    AR2030_sensor.banding = para;
    
    switch (AR2030_sensor.banding)
    {
    case AE_FLICKER_MODE_50HZ:
	    AR2030_write_XDMA(0xA404, 0x80); /* fd_mode */
	    printk("liuxd: AE_FLICKER_MODE_50HZ\n");
	    break;
    
    case AE_FLICKER_MODE_60HZ:
	    AR2030_write_XDMA(0xA404, 0xA0); /* fd_mode */
	    printk("liuxd: AE_FLICKER_MODE_60HZ\n");
	    break;
    
    default:
    	return FALSE;
    }
    AR2030_set_cmd(5); /* refresh mode */
    //AR2030_cal_fps(); /* need cal new fps */

    return TRUE;
} /* AR2030_set_param_banding */


// < EMPTY >
BOOL AR2030_set_param_exposure(UINT16 para)
{
    kal_uint8 ae_base_target;
    
	if (AR2030_sensor.exposure == para) return KAL_TRUE;
	AR2030_sensor.exposure = para;
    switch (AR2030_sensor.exposure)
    {
    case AE_EV_COMP_n13:    // -4 EV            
        ae_base_target = AR2030_AE_TARGET_ZERO - 40;
        break;
    
    case AE_EV_COMP_n10:    // -3 EV
        ae_base_target = AR2030_AE_TARGET_ZERO - 30;
        break;
    
    case AE_EV_COMP_n07:    // -2 EV
        ae_base_target = AR2030_AE_TARGET_ZERO - 20;
        break;
    
    case AE_EV_COMP_n03:    // -1 EV
        ae_base_target = AR2030_AE_TARGET_ZERO - 10;
        break;
    
    case AE_EV_COMP_00:   // +0 EV
        ae_base_target = AR2030_AE_TARGET_ZERO;
        break;
    
    case AE_EV_COMP_03:    // +1 EV
        ae_base_target = AR2030_AE_TARGET_ZERO + 10;
        break;
    
    case AE_EV_COMP_07:    // +2 EV
        ae_base_target = AR2030_AE_TARGET_ZERO + 20;
        break;
    
    case AE_EV_COMP_10:    // +3 EV
        ae_base_target = AR2030_AE_TARGET_ZERO + 30;
        break;
    
    case AE_EV_COMP_13:    // +4 EV
        ae_base_target = AR2030_AE_TARGET_ZERO + 40;
        break;
    
    default:
        return FALSE;    
    }
    AR2030_write_XDMA(0xA24F, ae_base_target); /* ae_base_target */

    return TRUE;    
} /* AR2030_set_param_exposure */


// < EMPTY >
UINT32 AR2030YUVSensorSetting(FEATURE_ID iCmd, UINT32 iPara)
{
    SENSORDB("AR2030YUVSensorSetting \n");
    switch (iCmd) {
    case FID_SCENE_MODE:        
        if (iPara == SCENE_MODE_OFF)
        {
			SENSORDB("FID_SCENE_MODE = FALSE \n");
			AR2030_NightMode(FALSE); 
        }
        else if (iPara == SCENE_MODE_NIGHTSCENE)
        {
			SENSORDB("FID_SCENE_MODE = TRUE \n");
			AR2030_NightMode(TRUE); 
        }
        break;
		
    case FID_AWB_MODE:
		SENSORDB("FID_AWB_MODE\n");
		AR2030_set_param_wb(iPara);
    	break;
		
    case FID_COLOR_EFFECT:
		SENSORDB("FID_COLOR_EFFECT\n");
		AR2030_set_param_effect(iPara);
		break;
		
    case FID_AE_EV:
		SENSORDB("FID_AE_EV\n");
		AR2030_set_param_exposure(iPara);
		break;
		
    case FID_AE_FLICKER:
		SENSORDB("FID_AE_FLICKER\n");
		AR2030_set_param_banding(iPara);
		break;
    
    case FID_ZOOM_FACTOR:
        SENSORDB("FID_ZOOM_FACTOR = %d\n",iPara);
        AR2030_zoom_factor = iPara;
    	break;
		
    default:
    	break;
    }
	
    return TRUE;
}   /* AR2030YUVSensorSetting */


// < EMPTY >
UINT32 AR2030YUVSetVideoMode(UINT16 u2FrameRate)
{
    SENSORDB("SetVideoMode %d\n",u2FrameRate);
	
    AR2030_sensor.video_mode = TRUE;
    AR2030_sensor.normal_fps = AR2030_FRAME_RATE_UNIT * 30;
    AR2030_sensor.night_fps = AR2030_FRAME_RATE_UNIT * 15;
    if (u2FrameRate == 30)
    {
        AR2030_sensor.video_frame = FALSE;
    }
    else if (u2FrameRate == 15)       
    {
        AR2030_sensor.video_frame = TRUE;
    }
    else
    {
        printk("Wrong frame rate setting \n");
    }
	
    AR2030_cal_fps();
    
    return TRUE;
}


UINT32 AR2030FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,
                             UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
{
	UINT16 u2Temp = 0; 
    UINT16 *pFeatureReturnPara16=(UINT16 *) pFeaturePara;
    UINT16 *pFeatureData16=(UINT16 *) pFeaturePara;
    UINT32 *pFeatureReturnPara32=(UINT32 *) pFeaturePara;
    UINT32 *pFeatureData32=(UINT32 *) pFeaturePara;
    MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData=(MSDK_SENSOR_CONFIG_STRUCT *) pFeaturePara;
    MSDK_SENSOR_REG_INFO_STRUCT *pSensorRegData=(MSDK_SENSOR_REG_INFO_STRUCT *) pFeaturePara;

    SENSORDB("[AR2030YUV]AR2030FeatureControl =%d\n",FeatureId);

    switch (FeatureId)
    {
	case SENSOR_FEATURE_GET_RESOLUTION:
		*pFeatureReturnPara16++=AR2030_IMAGE_SENSOR_FULL_HACTIVE;
		*pFeatureReturnPara16=AR2030_IMAGE_SENSOR_FULL_VACTIVE;
		*pFeatureParaLen=4;
		break;
		
	case SENSOR_FEATURE_GET_PERIOD:
		switch(AR2030_currentScenarioId)
		{
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			*pFeatureReturnPara16++=AR2030_sensor.pv_frame_height; // ?
        	*pFeatureReturnPara16=AR2030_sensor.pv_line_length; // ?
			break;
			
		default:
			*pFeatureReturnPara16++=AR2030_sensor.pv_frame_height;//+AR2030_PV_dummy_pixels;
        	*pFeatureReturnPara16=AR2030_sensor.pv_line_length;//+AR2030_PV_dummy_lines;
			break;
		}
		*pFeatureParaLen=4;
		break;
		
	case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
		//*pFeatureReturnPara32 = AR2030_sensor_pclk/10;
		*pFeatureParaLen=4;
		break;
		
	case SENSOR_FEATURE_SET_ESHUTTER:
		//u2Temp = AR2030_read_shutter(); 
		//printk("Shutter:%d\n", u2Temp);             
		break;
		
	case SENSOR_FEATURE_SET_NIGHTMODE:
		AR2030_NightMode((BOOL) *pFeatureData16);
		break;
		
	case SENSOR_FEATURE_SET_GAIN:
		//u2Temp = AR2030_read_gain(); 
		//printk("Gain:%d\n", u2Temp); 
		//printk("y_val:%d\n", AR2030_read_cmos_sensor(0x301B));
		break;
		
	case SENSOR_FEATURE_SET_FLASHLIGHT:
		break;
		
	case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
		//AR2030_isp_master_clock=*pFeatureData32;
		break;
		
	case SENSOR_FEATURE_SET_REGISTER:
		AR2030_write_cmos_sensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
		break;
		
	case SENSOR_FEATURE_GET_REGISTER:
		pSensorRegData->RegData = AR2030_read_cmos_sensor(pSensorRegData->RegAddr);
		break;
		
	case SENSOR_FEATURE_GET_CONFIG_PARA:
		memcpy(pSensorConfigData, &AR2030SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
		*pFeatureParaLen=sizeof(MSDK_SENSOR_CONFIG_STRUCT);
		break;
		
	case SENSOR_FEATURE_SET_CCT_REGISTER:
	case SENSOR_FEATURE_GET_CCT_REGISTER:
	case SENSOR_FEATURE_SET_ENG_REGISTER:
	case SENSOR_FEATURE_GET_ENG_REGISTER:
	case SENSOR_FEATURE_GET_REGISTER_DEFAULT:

	case SENSOR_FEATURE_CAMERA_PARA_TO_SENSOR:
	case SENSOR_FEATURE_SENSOR_TO_CAMERA_PARA:
	case SENSOR_FEATURE_GET_GROUP_COUNT:
	case SENSOR_FEATURE_GET_GROUP_INFO:
	case SENSOR_FEATURE_GET_ITEM_INFO:
	case SENSOR_FEATURE_SET_ITEM_INFO:
	case SENSOR_FEATURE_GET_ENG_INFO:
		break;
		
	case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
		// get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
		// if EEPROM does not exist in camera module.
		*pFeatureReturnPara32=LENS_DRIVER_ID_DO_NOT_CARE;
		*pFeatureParaLen=4;
		break;
		
	case SENSOR_FEATURE_SET_YUV_CMD:
		printk("AR2030 YUV sensor Setting:%d, %d \n", *pFeatureData32,  *(pFeatureData32+1));
		AR2030YUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
		break;
		
	case SENSOR_FEATURE_SET_VIDEO_MODE:
		SENSORDB("zhijie SENSOR_FEATURE_SET_VIDEO_MODE ");
		AR2030YUVSetVideoMode(*pFeatureData16);
		break; 
		
	case SENSOR_FEATURE_CHECK_SENSOR_ID:
		AR2030_GetSensorID(pFeatureData32); 
		break;
		
	default:
		break;            
    }
	
    return ERROR_NONE;
}    /* AR2030FeatureControl() */


SENSOR_FUNCTION_STRUCT SensorFuncAR2030 =
{
    AR2030Open,
    AR2030GetInfo,
    AR2030GetResolution,
    AR2030FeatureControl,
    AR2030Control,
    AR2030Close
};

UINT32 AR2030_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
    /* To Do : Check Sensor status here */
    if (pfFunc!=NULL)
        *pfFunc=&SensorFuncAR2030;

    return ERROR_NONE;
}    /* SensorInit() */

