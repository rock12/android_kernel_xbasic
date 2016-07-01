#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov5647raw.h"
#include "camera_info_ov5647raw.h"
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
        73925,    // i4R_AVG
        15195,    // i4R_STD
        91100,    // i4B_AVG
        23505,    // i4B_STD
        {  // i4P00[9]
            4600000, -1615000, -425000, -602500, 3127500, 35000, 145000, -1950000, 4365000
        },
        {  // i4P10[9]
            713554, -726527, 12972, -24002, -7872, 32365, 41627, 483184, -524811
        },
        {  // i4P01[9]
            103544, -290407, 186863, -49052, -253944, 308314, 37078, -81258, 44180
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
            1136,    // u4MinGain, 1024 base = 1x
            8192,    // u4MaxGain, 16x
            106,    // u4MiniISOGain, ISOxx  
            128,    // u4GainStepUnit, 1x/8 
            34,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            30,    // u4VideoExpUnit  
            30,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            35,    // u4CapExpUnit 
            14,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            28,    // u4LensFno, Fno = 2.8
            350    // u4FocusLength_100x
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
            FALSE,    // bEnableCaptureThres
            FALSE,    // bEnableVideoThres
            FALSE,    // bEnableStrobeThres
            52,                // u4AETarget // 47
            52,                // u4StrobeAETarget // 47

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
            -13,    // i4BVOffset delta BV = value/10 
            80,    // u4PreviewFlareOffset
            80,    // u4CaptureFlareOffset
            5,    // u4CaptureFlareThres
            80,    // u4VideoFlareOffset
            5,    // u4VideoFlareThres
            2,    // u4StrobeFlareOffset
            2,    // u4StrobeFlareThres
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
                819,    // i4R
                512,    // i4G
                564    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                -14,    // i4X
                -250    // i4Y
            },
            // Horizon
            {
                -377,    // i4X
                -287    // i4Y
            },
            // A
            {
                -267,    // i4X
                -294    // i4Y
            },
            // TL84
            {
                -134,    // i4X
                -299    // i4Y
            },
            // CWF
            {
                -100,    // i4X
                -370    // i4Y
            },
            // DNP
            {
                -14,    // i4X
                -250    // i4Y
            },
            // D65
            {
                138,    // i4X
                -209    // i4Y
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
                -62,    // i4X
                -242    // i4Y
            },
            // Horizon
            {
                -425,    // i4X
                -209    // i4Y
            },
            // A
            {
                -318,    // i4X
                -237    // i4Y
            },
            // TL84
            {
                -189,    // i4X
                -268    // i4Y
            },
            // CWF
            {
                -169,    // i4X
                -344    // i4Y
            },
            // DNP
            {
                -62,    // i4X
                -242    // i4Y
            },
            // D65
            {
                95,    // i4X
                -231    // i4Y
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
                705,    // i4R
                512,    // i4G
                732    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                578,    // i4G
                1419    // i4B
            },
            // A 
            {
                531,    // i4R
                512,    // i4G
                1095    // i4B
            },
            // TL84 
            {
                641,    // i4R
                512,    // i4G
                920    // i4B
            },
            // CWF 
            {
                739,    // i4R
                512,    // i4G
                967    // i4B
            },
            // DNP 
            {
                705,    // i4R
                512,    // i4G
                732    // i4B
            },
            // D65 
            {
                819,    // i4R
                512,    // i4G
                564    // i4B
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
            11,    // i4RotationAngle
            251,    // i4Cos
            49    // i4Sin
        },
        // Daylight locus parameter
        {
            -189,    // i4SlopeNumerator
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
            -239,    // i4RightBound
            -1589,    // i4LeftBound
            -170,    // i4UpperBound
            -273    // i4LowerBound
            },
            // Warm fluorescent
            {
            -239,    // i4RightBound
            -1589,    // i4LeftBound
            -273,    // i4UpperBound
            -393    // i4LowerBound
            },
            // Fluorescent
            {
            -112,    // i4RightBound
            -239,    // i4LeftBound
            -162,    // i4UpperBound
            -306    // i4LowerBound
            },
            // CWF
            {
            -112,    // i4RightBound
            -239,    // i4LeftBound
            -306,    // i4UpperBound
            -394    // i4LowerBound
            },
            // Daylight
            {
            120,    // i4RightBound
            -112,    // i4LeftBound
            -151,    // i4UpperBound
            -311    // i4LowerBound
            },
            // Shade
            {
            480,    // i4RightBound
            120,    // i4LeftBound
            -151,    // i4UpperBound
            -311    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            120,    // i4RightBound
            -112,    // i4LeftBound
            -311,    // i4UpperBound
            -394    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            480,    // i4RightBound
            -1589,    // i4LeftBound
            0,    // i4UpperBound
            -394    // i4LowerBound
            },
            // Daylight
            {
            145,    // i4RightBound
            -112,    // i4LeftBound
            -151,    // i4UpperBound
            -311    // i4LowerBound
            },
            // Cloudy daylight
            {
            245,    // i4RightBound
            70,    // i4LeftBound
            -151,    // i4UpperBound
            -311    // i4LowerBound
            },
            // Shade
            {
            345,    // i4RightBound
            70,    // i4LeftBound
            -151,    // i4UpperBound
            -311    // i4LowerBound
            },
            // Twilight
            {
            -112,    // i4RightBound
            -272,    // i4LeftBound
            -151,    // i4UpperBound
            -311    // i4LowerBound
            },
            // Fluorescent
            {
            145,    // i4RightBound
            -289,    // i4LeftBound
            -181,    // i4UpperBound
            -394    // i4LowerBound
            },
            // Warm fluorescent
            {
            -218,    // i4RightBound
            -418,    // i4LeftBound
            -181,    // i4UpperBound
            -394    // i4LowerBound
            },
            // Incandescent
            {
            -218,    // i4RightBound
            -418,    // i4LeftBound
            -151,    // i4UpperBound
            -311    // i4LowerBound
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
            752,    // i4R
            512,    // i4G
            639    // i4B
            },
            // Cloudy daylight
            {
            875,    // i4R
            512,    // i4G
            510    // i4B
            },
            // Shade
            {
            923,    // i4R
            512,    // i4G
            471    // i4B
            },
            // Twilight
            {
            602,    // i4R
            512,    // i4G
            890    // i4B
            },
            // Fluorescent
            {
            749,    // i4R
            512,    // i4G
            781    // i4B
            },
            // Warm fluorescent
            {
            575,    // i4R
            512,    // i4G
            1155    // i4B
            },
            // Incandescent
            {
            526,    // i4R
            512,    // i4G
            1087    // i4B
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
            6466    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            5637    // i4OffsetThr
            },
            // Shade
            {
            50,    // i4SliderValue
            347    // i4OffsetThr
            },
            // Daylight WB gain
            {
            692,    // i4R
            512,    // i4G
            724    // i4B
            },
            // Preference gain: strobe
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: tungsten
            {
            480,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: warm fluorescent
            {
            480,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: fluorescent
            {
            500,    // i4R
            512,    // i4G
            530    // i4B
            },
            // Preference gain: CWF
            {
            500,    // i4R
            512,    // i4G
            530    // i4B
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
                -520,    // i4RotatedXCoordinate[0]
                -413,    // i4RotatedXCoordinate[1]
                -284,    // i4RotatedXCoordinate[2]
                -157,    // i4RotatedXCoordinate[3]
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


