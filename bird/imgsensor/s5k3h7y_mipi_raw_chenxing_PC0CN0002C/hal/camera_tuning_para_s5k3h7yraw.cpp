#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_s5k3h7yraw.h"
#include "camera_info_s5k3h7yraw.h"
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
        70300,    // i4R_AVG
        8906,    // i4R_STD
        92267,    // i4B_AVG
        24788,    // i4B_STD
        {  // i4P00[9]
            5383333, -2373333, -453333, -790000, 3470000, -120000, 120000, -1833333, 4270000
        },
        {  // i4P10[9]
            639683, -691536, 50691, 25718, -78211, 52494, 39217, -43419, 3040
        },
        {  // i4P01[9]
            534236, -502884, -26376, -173683, -14681, 188364, -40808, -131880, 177664
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
            1195,    // u4MinGain, 1024 base = 1x
            16384,    // u4MaxGain, 16x
            55,    // u4MiniISOGain, ISOxx  
            128,    // u4GainStepUnit, 1x/8 
            33,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            33,    // u4VideoExpUnit  
            30,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            58,    // u4CapExpUnit 
            15,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            22,    // u4LensFno, Fno = 2.8
            385    // u4FocusLength_100x
        },
        // rHistConfig
        {
            2,    // u4HistHighThres
            40,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            160,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 220, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {86, 108, 128, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 26, 30, 34}    // u4BlackLightThres[AE_CCT_STRENGTH_NUM] 
        },
        // rCCTConfig
        {
            TRUE,    // bEnableBlackLight
            TRUE,    // bEnableHistStretch
            FALSE,    // bEnableAntiOverExposure
            TRUE,    // bEnableTimeLPF
            TRUE,    // bEnableCaptureThres
            TRUE,    // bEnableVideoThres
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
            10,    // i4BVOffset delta BV = value/10 
            32,                 // u4PreviewFlareOffset
            32,                 // u4CaptureFlareOffset
            2,                 // u4CaptureFlareThres
            32,                 // u4VideoFlareOffset
            2,                 // u4VideoFlareThres
            32,                 // u4StrobeFlareOffset
            2,                 // u4StrobeFlareThres
            50,                 // u4PrvMaxFlareThres
            0,                 // u4PrvMinFlareThres
            50,                 // u4VideoMaxFlareThres
            0,                 // u4VideoMinFlareThres     
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
                758,    // i4R
                512,    // i4G
                641    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                -91,    // i4X
                -269    // i4Y
            },
            // Horizon
            {
                -475,    // i4X
                -347    // i4Y
            },
            // A
            {
                -351,    // i4X
                -342    // i4Y
            },
            // TL84
            {
                -207,    // i4X
                -332    // i4Y
            },
            // CWF
            {
                -155,    // i4X
                -417    // i4Y
            },
            // DNP
            {
                -91,    // i4X
                -269    // i4Y
            },
            // D65
            {
                62,    // i4X
                -228    // i4Y
            },
            // DF
            {
                0,    // i4X
                0    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                -157,    // i4X
                -236    // i4Y
            },
            // Horizon
            {
                -548,    // i4X
                -212    // i4Y
            },
            // A
            {
                -427,    // i4X
                -239    // i4Y
            },
            // TL84
            {
                -285,    // i4X
                -267    // i4Y
            },
            // CWF
            {
                -257,    // i4X
                -362    // i4Y
            },
            // DNP
            {
                -157,    // i4X
                -236    // i4Y
            },
            // D65
            {
                1,    // i4X
                -236    // i4Y
            },
            // DF
            {
                -157,    // i4X
                -332    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                651,    // i4R
                512,    // i4G
                833    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                609,    // i4G
                1854    // i4B
            },
            // A 
            {
                512,    // i4R
                518,    // i4G
                1324    // i4B
            },
            // TL84 
            {
                606,    // i4R
                512,    // i4G
                1061    // i4B
            },
            // CWF 
            {
                730,    // i4R
                512,    // i4G
                1110    // i4B
            },
            // DNP 
            {
                651,    // i4R
                512,    // i4G
                833    // i4B
            },
            // D65 
            {
                758,    // i4R
                512,    // i4G
                641    // i4B
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
            15,    // i4RotationAngle
            247,    // i4Cos
            66    // i4Sin
        },
        // Daylight locus parameter
        {
            -217,    // i4SlopeNumerator
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
            -335,    // i4RightBound
            -985,    // i4LeftBound
            -175,    // i4UpperBound
            -275    // i4LowerBound
            },
            // Warm fluorescent
            {
            -335,    // i4RightBound
            -985,    // i4LeftBound
            -275,    // i4UpperBound
            -395    // i4LowerBound
            },
            // Fluorescent
            {
            -207,    // i4RightBound
            -335,    // i4LeftBound
            -165,    // i4UpperBound
            -314    // i4LowerBound
            },
            // CWF
            {
            -207,    // i4RightBound
            -335,    // i4LeftBound
            -314,    // i4UpperBound
            -412    // i4LowerBound
            },
            // Daylight
            {
            26,    // i4RightBound
            -207,    // i4LeftBound
            -156,    // i4UpperBound
            -316    // i4LowerBound
            },
            // Shade
            {
            386,    // i4RightBound
            26,    // i4LeftBound
            -156,    // i4UpperBound
            -316    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            26,    // i4RightBound
            -207,    // i4LeftBound
            -316,    // i4UpperBound
            -416    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            386,    // i4RightBound
            -985,    // i4LeftBound
            -131,    // i4UpperBound
            -412    // i4LowerBound
            },
            // Daylight
            {
            51,    // i4RightBound
            -207,    // i4LeftBound
            -156,    // i4UpperBound
            -316    // i4LowerBound
            },
            // Cloudy daylight
            {
            151,    // i4RightBound
            -24,    // i4LeftBound
            -156,    // i4UpperBound
            -316    // i4LowerBound
            },
            // Shade
            {
            251,    // i4RightBound
            -24,    // i4LeftBound
            -156,    // i4UpperBound
            -316    // i4LowerBound
            },
            // Twilight
            {
            -207,    // i4RightBound
            -367,    // i4LeftBound
            -156,    // i4UpperBound
            -316    // i4LowerBound
            },
            // Fluorescent
            {
            51,    // i4RightBound
            -385,    // i4LeftBound
            -186,    // i4UpperBound
            -412    // i4LowerBound
            },
            // Warm fluorescent
            {
            -327,    // i4RightBound
            -527,    // i4LeftBound
            -186,    // i4UpperBound
            -412    // i4LowerBound
            },
            // Incandescent
            {
            -327,    // i4RightBound
            -527,    // i4LeftBound
            -156,    // i4UpperBound
            -316    // i4LowerBound
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
            703,    // i4R
            512,    // i4G
            731    // i4B
            },
            // Cloudy daylight
            {
            805,    // i4R
            512,    // i4G
            578    // i4B
            },
            // Shade
            {
            845,    // i4R
            512,    // i4G
            532    // i4B
            },
            // Twilight
            {
            575,    // i4R
            512,    // i4G
            1034    // i4B
            },
            // Fluorescent
            {
            716,    // i4R
            512,    // i4G
            900    // i4B
            },
            // Warm fluorescent
            {
            558,    // i4R
            512,    // i4G
            1386    // i4B
            },
            // Incandescent
            {
            503,    // i4R
            512,    // i4G
            1304    // i4B
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
            6890    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            5731    // i4OffsetThr
            },
            // Shade
            {
            0,    // i4SliderValue
            1352    // i4OffsetThr
            },
            // Daylight WB gain
            {
            652,    // i4R
            512,    // i4G
            833    // i4B
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
            512,    // i4R
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
                -549,    // i4RotatedXCoordinate[0]
                -428,    // i4RotatedXCoordinate[1]
                -286,    // i4RotatedXCoordinate[2]
                -158,    // i4RotatedXCoordinate[3]
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


