#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_s5k4h5yxraw.h"
#include "camera_info_s5k4h5yxraw.h"
#include "camera_custom_AEPlinetable.h"
const NVRAM_CAMERA_ISP_PARAM_STRUCT CAMERA_ISP_DEFAULT_VALUE =
{{
    //Version
    Version: NVRAM_CAMERA_PARA_FILE_VERSION,
    //SensorId
    SensorId: SENSOR_ID,
    ISPComm:{
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    },
    ISPPca:{
        #include INCLUDE_FILENAME_ISP_PCA_PARAM
    },
    ISPRegs:{
        #include INCLUDE_FILENAME_ISP_REGS_PARAM
        },
    ISPMfbMixer:{{
        {//00: MFB mixer for ISO 100
            0x00000000, 0x00000000
        },
        {//01: MFB mixer for ISO 200
            0x00000000, 0x00000000
        },
        {//02: MFB mixer for ISO 400
            0x00000000, 0x00000000
        },
        {//03: MFB mixer for ISO 800
            0x00000000, 0x00000000
        },
        {//04: MFB mixer for ISO 1600
            0x00000000, 0x00000000
        },
        {//05: MFB mixer for ISO 2400
            0x00000000, 0x00000000
        },
        {//06: MFB mixer for ISO 3200
            0x00000000, 0x00000000
        }
    }},
    ISPCcmPoly22:{
        74125,    // i4R_AVG
        15573,    // i4R_STD
        92925,    // i4B_AVG
        25511,    // i4B_STD
        {  // i4P00[9]
            4767500, -1717500, -490000, -675000, 3250000, -15000, 42500, -2067500, 4585000
        },
        {  // i4P10[9]
            -182396, 203165, -20768, 38604, 74648, -113252, -82655, 723950, -641295
        },
        {  // i4P01[9]
            -530158, 587665, -57506, -87925, 6140, 81785, -152115, 154175, -2060
        },
        {  // i4P20[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P11[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P02[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    }
}};

const NVRAM_CAMERA_3A_STRUCT CAMERA_3A_NVRAM_DEFAULT_VALUE =
{
    NVRAM_CAMERA_3A_FILE_VERSION, // u4Version
    SENSOR_ID, // SensorId

    // AE NVRAM
    {
        // rDevicesInfo
        {
            1152,    // u4MinGain, 1024 base = 1x
            16384,    // u4MaxGain, 16x
            70,    // u4MiniISOGain, ISOxx  
            256,    // u4GainStepUnit, 1x/8 
            14753,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            16177,    // u4VideoExpUnit  
            30,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            16177,    // u4CapExpUnit 
            24,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            24,    // u4LensFno, Fno = 2.8
            350    // u4FocusLength_100x
        },
        // rHistConfig
        {
            4,    // u4HistHighThres
            40,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            160,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 220, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {82, 108, 128, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 26, 30, 34}    // u4BlackLightThres[AE_CCT_STRENGTH_NUM] 
        },
        // rCCTConfig
        {
            TRUE,    // bEnableBlackLight
            TRUE,    // bEnableHistStretch
            FALSE,    // bEnableAntiOverExposure
            TRUE,    // bEnableTimeLPF
            FALSE,    // bEnableCaptureThres
            FALSE,    // bEnableVideoThres
            TRUE,    // bEnableStrobeThres
            47,    // u4AETarget
            47,    // u4StrobeAETarget
            50,    // u4InitIndex
            4,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            2,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            6,    // i4BVOffset delta BV = value/10 
            64,    // u4PreviewFlareOffset
            64,    // u4CaptureFlareOffset
            3,    // u4CaptureFlareThres
            64,    // u4VideoFlareOffset
            3,    // u4VideoFlareThres
            32,    // u4StrobeFlareOffset
            3,    // u4StrobeFlareThres
            160,    // u4PrvMaxFlareThres
            0,    // u4PrvMinFlareThres
            160,    // u4VideoMaxFlareThres
            0,    // u4VideoMinFlareThres
            18,    // u4FlatnessThres    // 10 base for flatness condition.
            75    // u4FlatnessStrength
        }
    },
    // AWB NVRAM
    {
        // AWB calibration data
        {
            // rUnitGain (unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rGoldenGain (golden sample gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rTuningUnitGain (Tuning sample unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rD65Gain (D65 WB gain: 1.0 = 512)
            {
                741,    // i4R
                512,    // i4G
                623    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                -80,    // i4X
                -252    // i4Y
            },
            // Horizon
            {
                -398,    // i4X
                -297    // i4Y
            },
            // A
            {
                -301,    // i4X
                -304    // i4Y
            },
            // TL84
            {
                -188,    // i4X
                -299    // i4Y
            },
            // CWF
            {
                -148,    // i4X
                -359    // i4Y
            },
            // DNP
            {
                -80,    // i4X
                -252    // i4Y
            },
            // D65
            {
                64,    // i4X
                -209    // i4Y
            },
            // DF
            {
                -80,    // i4X
                -352    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                -135,    // i4X
                -227    // i4Y
            },
            // Horizon
            {
                -454,    // i4X
                -199    // i4Y
            },
            // A
            {
                -362,    // i4X
                -227    // i4Y
            },
            // TL84
            {
                -251,    // i4X
                -248    // i4Y
            },
            // CWF
            {
                -225,    // i4X
                -316    // i4Y
            },
            // DNP
            {
                -135,    // i4X
                -227    // i4Y
            },
            // D65
            {
                15,    // i4X
                -218    // i4Y
            },
            // DF
            {
                0,    // i4X
                0    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                647,    // i4R
                512,    // i4G
                803    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                587,    // i4G
                1504    // i4B
            },
            // A 
            {
                513,    // i4R
                512,    // i4G
                1161    // i4B
            },
            // TL84 
            {
                595,    // i4R
                512,    // i4G
                991    // i4B
            },
            // CWF 
            {
                682,    // i4R
                512,    // i4G
                1018    // i4B
            },
            // DNP 
            {
                647,    // i4R
                512,    // i4G
                803    // i4B
            },
            // D65 
            {
                741,    // i4R
                512,    // i4G
                623    // i4B
            },
            // DF 
            {
                512,    // i4R
                512,    // i4G
                512    // i4B
            }
        },
        // Rotation matrix parameter
        {
            13,    // i4RotationAngle
            249,    // i4Cos
            58    // i4Sin
        },
        // Daylight locus parameter
        {
            -206,    // i4SlopeNumerator
            128    // i4SlopeDenominator
        },
        // AWB light area
        {
            // Strobe:FIXME
            {
            0,    // i4RightBound
            0,    // i4LeftBound
            0,    // i4UpperBound
            0    // i4LowerBound
            },
            // Tungsten
            {
            -301,    // i4RightBound
            -951,    // i4LeftBound
            -163,    // i4UpperBound
            -263    // i4LowerBound
            },
            // Warm fluorescent
            {
            -301,    // i4RightBound
            -951,    // i4LeftBound
            -263,    // i4UpperBound
            -383    // i4LowerBound
            },
            // Fluorescent
            {
            -185,    // i4RightBound
            -301,    // i4LeftBound
            -150,    // i4UpperBound
            -282    // i4LowerBound
            },
            // CWF
            {
            -185,    // i4RightBound
            -301,    // i4LeftBound
            -282,    // i4UpperBound
            -366    // i4LowerBound
            },
            // Daylight
            {
            40,    // i4RightBound
            -185,    // i4LeftBound
            -138,    // i4UpperBound
            -298    // i4LowerBound
            },
            // Shade
            {
            400,    // i4RightBound
            40,    // i4LeftBound
            -138,    // i4UpperBound
            -298    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            40,    // i4RightBound
            -185,    // i4LeftBound
            -298,    // i4UpperBound
            -400    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            400,    // i4RightBound
            -951,    // i4LeftBound
            0,    // i4UpperBound
            -400    // i4LowerBound
            },
            // Daylight
            {
            65,    // i4RightBound
            -185,    // i4LeftBound
            -138,    // i4UpperBound
            -298    // i4LowerBound
            },
            // Cloudy daylight
            {
            165,    // i4RightBound
            -10,    // i4LeftBound
            -138,    // i4UpperBound
            -298    // i4LowerBound
            },
            // Shade
            {
            265,    // i4RightBound
            -10,    // i4LeftBound
            -138,    // i4UpperBound
            -298    // i4LowerBound
            },
            // Twilight
            {
            -185,    // i4RightBound
            -345,    // i4LeftBound
            -138,    // i4UpperBound
            -298    // i4LowerBound
            },
            // Fluorescent
            {
            65,    // i4RightBound
            -351,    // i4LeftBound
            -168,    // i4UpperBound
            -366    // i4LowerBound
            },
            // Warm fluorescent
            {
            -262,    // i4RightBound
            -462,    // i4LeftBound
            -168,    // i4UpperBound
            -366    // i4LowerBound
            },
            // Incandescent
            {
            -262,    // i4RightBound
            -462,    // i4LeftBound
            -138,    // i4UpperBound
            -298    // i4LowerBound
            },
            // Gray World
            {
            5000,    // i4RightBound
            -5000,    // i4LeftBound
            5000,    // i4UpperBound
            -5000    // i4LowerBound
            }
        },
        // PWB default gain	
        {
            // Daylight
            {
            687,    // i4R
            512,    // i4G
            704    // i4B
            },
            // Cloudy daylight
            {
            790,    // i4R
            512,    // i4G
            563    // i4B
            },
            // Shade
            {
            831,    // i4R
            512,    // i4G
            519    // i4B
            },
            // Twilight
            {
            558,    // i4R
            512,    // i4G
            983    // i4B
            },
            // Fluorescent
            {
            684,    // i4R
            512,    // i4G
            847    // i4B
            },
            // Warm fluorescent
            {
            548,    // i4R
            512,    // i4G
            1210    // i4B
            },
            // Incandescent
            {
            506,    // i4R
            512,    // i4G
            1151    // i4B
            },
            // Gray World
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        // AWB preference color	
        {
            // Tungsten
            {
            0,    // i4SliderValue
            5850    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            5426    // i4OffsetThr
            },
            // Shade
            {
            0,    // i4SliderValue
            1350    // i4OffsetThr
            },
            // Daylight WB gain
            {
            637,    // i4R
            512,    // i4G
            795    // i4B
            },
            // Preference gain: strobe
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: tungsten
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: warm fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: CWF
            {
            525,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: shade
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        {// CCT estimation
            {// CCT
                2300,    // i4CCT[0]
                2850,    // i4CCT[1]
                4100,    // i4CCT[2]
                5100,    // i4CCT[3]
                6500    // i4CCT[4]
            },
            {// Rotated X coordinate
                -469,    // i4RotatedXCoordinate[0]
                -377,    // i4RotatedXCoordinate[1]
                -266,    // i4RotatedXCoordinate[2]
                -150,    // i4RotatedXCoordinate[3]
                0    // i4RotatedXCoordinate[4]
            }
        }
    },
    {0}
};

#include INCLUDE_FILENAME_ISP_LSC_PARAM
//};  //  namespace


typedef NSFeature::RAWSensorInfo<SENSOR_ID> SensorInfoSingleton_T;


namespace NSFeature {
template <>
UINT32
SensorInfoSingleton_T::
impGetDefaultData(CAMERA_DATA_TYPE_ENUM const CameraDataType, VOID*const pDataBuf, UINT32 const size) const
{
    UINT32 dataSize[CAMERA_DATA_TYPE_NUM] = {sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT),
                                             sizeof(NVRAM_CAMERA_3A_STRUCT),
                                             sizeof(NVRAM_CAMERA_SHADING_STRUCT),
                                             sizeof(NVRAM_LENS_PARA_STRUCT),
                                             sizeof(AE_PLINETABLE_T)};

    if (CameraDataType > CAMERA_DATA_AE_PLINETABLE || NULL == pDataBuf || (size < dataSize[CameraDataType]))
    {
        return 1;
    }

    switch(CameraDataType)
    {
        case CAMERA_NVRAM_DATA_ISP:
            memcpy(pDataBuf,&CAMERA_ISP_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_3A:
            memcpy(pDataBuf,&CAMERA_3A_NVRAM_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_3A_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_SHADING:
            memcpy(pDataBuf,&CAMERA_SHADING_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_SHADING_STRUCT));
            break;
        case CAMERA_DATA_AE_PLINETABLE:
            memcpy(pDataBuf,&g_PlineTableMapping,sizeof(AE_PLINETABLE_T));
            break;
        default:
            break;
    }
    return 0;
}}; // NSFeature


