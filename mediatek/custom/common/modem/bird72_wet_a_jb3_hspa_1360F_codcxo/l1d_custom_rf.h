/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001
*
*******************************************************************************/

/*******************************************************************************
 *
 * Filename:
 * ---------
 *   l1d_custom_rf.h
 *
 * Project:
 * --------
 *   MT6166
 *
 * Description:
 * ------------
 *   MT6166 2G RF constance definition
 *
 * Author:
 * -------
 *
 *
 *==============================================================================
 *******************************************************************************/

#ifndef  _L1D_CUSTOM_RF_H_
#define  _L1D_CUSTOM_RF_H_
/* --------------------------------------------------------------------------- */

#if !defined(MT6166_2G_RF)
   #error "rf files mismatch with compile option!"
#endif

#if IS_BPI_V2_PT3A_SUPPORT
/*--------------------------------------------------------*/
/*   Event Timing Define                                  */
/*--------------------------------------------------------*/
/*MT6166*/
/*MT6166*/ #define  QB_SR1               166
/*MT6166*/ #define  QB_SR2               45   /* SR2 should be larger  than (QB_RX_FENA_2_FSYNC+8QB)*/
/*MT6166*/ #define  QB_SR2M              22
/*MT6166*/ #define  QB_SR3               3    /* SR3 should be larger than (QB_RX_FSYNC_2_FENA+2QB) */
/*MT6166*/ #define  QB_PR1               167  /* QB_PR1>QB_SR1 to prevent RF conflict among 2/3G    */
/*MT6166*/ #define  QB_PR2               50
/*MT6166*/ #define  QB_PR3               7
/*MT6166*/ #define  QB_ST1               280
/*MT6166*/ #define  QB_ST2B              33   /* ST2B should be larger  than (QB_TX_FENA_2_FSYNC+8QB)*/
/*MT6166*/ #define  QB_ST3               38   /* ST3  should be larger  than (QB_TX_FSYNC_2_FENA+7QB)*/
/*MT6166*/ #define  QB_PT1               282  /* QB_PT1>QB_ST1 to prevent RF conflict among 2/3G     */
/*MT6166*/ #define  QB_PT2               56
/*MT6166*/ #define  QB_PT2B              -6
/*MT6166*/ #define  QB_PT3               33
/*MT6166*/ #define  QB_PT3A              46
/*MT6166*/ #define  QB_ST2M_G8           10
/*MT6166*/ #define  QB_ST2M_8G           6
/*MT6166*/ #define  QB_PT2M1_G8          -1
/*MT6166*/ #define  QB_PT2M2_G8          -3
/*MT6166*/ #define  QB_PT2M1_8G          10
/*MT6166*/ #define  QB_PT2M2_8G          4
/*MT6166*/
/*MT6166*/
/*MT6166*/ #define  QB_APCON             14 //OH:11
/*MT6166*/ #define  QB_APCMID            20 //OH:18
/*MT6166*/ #define  QB_APCOFF            6  //56: 6
/*MT6166*/ #define  QB_APCDACON          18 //0
/*MT6166*/ #define  TX_PROPAGATION_DELAY 47 //56:47 / OH:46
/*MT6166*/
/*MT6166*/
/*MT6166*/ /*--------------------------------------------------*/
/*MT6166*/ /*   define  BPI data for MT6166                    */
/*MT6166*/ /*--------------------------------------------------*/
/*MT6166*/ /*  PRCB : bit   pin                                */
/*MT6166*/ /*          0    ASM_VCTRL_A                        */
/*MT6166*/ /*          1    ASM_VCTRL_B                        */
/*MT6166*/ /*          2    ASM_VCTRL_C                        */
/*MT6166*/ /*          3    ASM_VCTRL_D/WG_GGE_PA_EN           */
/*MT6166*/ /*         12    SP3T_A_V1                          */
/*MT6166*/ /*         13    SP3T_B_V2                          */
/*MT6166*/ /*         16    ANTENNA_CONFLICT_2G                */
/*MT6166*/ /*         17    SPI_SWITCH_TO_2G                   */
/*MT6166*/ /*         18    GSM_ERR_DET_ID                     */
/*MT6166*/ /*         20    M23G_PA_INDICATOR                  */
/*MT6166*/ /*--------------------------------------------------*/
/*MT6166*/
/*MT6166*/ /*------------------------------------------------------*/
/*MT6166*/ /*  ANTENNA_CONFLICT_2G(Pin:16) has no dedicate output  */
/*MT6166*/ /*  pin, and it is mapped to bit "16" for SW control.   */
/*MT6166*/ /*  For accurate RF conflict detection, this value must */
/*MT6166*/ /*  set "16" and is unchangable.                        */
/*MT6166*/ /*------------------------------------------------------*/
/*MT6166*/ #define  ANTENNA_CONFLICT_2G    (               16) /* For accurate RF conflict detection(2G/TDD), this value must set "16" */
/*MT6166*/                                                     /* and is unchangable.                                                  */
/*MT6166*/ #define  PDATA_GSM_ERR_DET      (1<<ANTENNA_CONFLICT_2G)
/*MT6166*/ #define  SPI_SWITCH_TO_2G       (1<<17)
/*MT6166*/
/*MT6166*/ #define  PDATA_GMSK              0x000002
/*MT6166*/ #define  PDATA_8PSK              0x000000
/*MT6166*/ 
/*MT6166*/ #define  PDATA_GSM850_PR1       (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PR2       (0x000002           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PR3       (0x000000                                              )
/*MT6166*/ #define  PDATA_GSM_PR1          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PR2          (0x000002           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PR3          (0x000000                                              )
/*MT6166*/ #define  PDATA_DCS_PR1          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PR2          (0x000003           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PR3          (0x000000                                              )
/*MT6166*/ #define  PDATA_PCS_PR1          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PR2          (0x000003           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PR3          (0x000000                                              )
/*MT6166*/ #define  PDATA_GSM850_PT1       (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT2       (0x000001           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT2B      (0x100008           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT3       (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT3A      (0x000000                                              )
/*MT6166*/ #define  PDATA_GSM850_PT2M1_G8  (0x100008|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT2M2_G8  (0x100008|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT2M1_8G  (0x100008           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT2M2_8G  (0x100008           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT1          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT2          (0x000001           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT2B         (0x100008          |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT3          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT3A         (0x000000                                              )
/*MT6166*/ #define  PDATA_GSM_PT2M1_G8     (0x100008|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT2M2_G8     (0x100008|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT2M1_8G     (0x100008           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT2M2_8G     (0x100008         |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT1          (0x000004           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT2          (0x000004           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT2B         (0x100009           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT3          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT3A         (0x000000                                              )
/*MT6166*/ #define  PDATA_DCS_PT2M1_G8     (0x100009|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT2M2_G8     (0x100009|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G) 
/*MT6166*/ #define  PDATA_DCS_PT2M1_8G     (0x100009           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT2M2_8G     (0x100009           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT1          (0x000004           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT2          (0x000004           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT2B         (0x100009           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT3          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT3A         (0x000000                                              )
/*MT6166*/ #define  PDATA_PCS_PT2M1_G8     (0x100009|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT2M2_G8     (0x100009|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT2M1_8G     (0x100009           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT2M2_8G     (0x100009           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_INIT             (0x000000                                              )
/*MT6166*/ #define  PDATA_IDLE             (0x000000                                              )
#else
/*--------------------------------------------------------*/
/*   Event Timing Define                                  */
/*--------------------------------------------------------*/
/*MT6166*/
/*MT6166*/ #define  QB_SR1               166
/*MT6166*/ #define  QB_SR2               45   /* SR2 should be larger  than (QB_RX_FENA_2_FSYNC+8QB)*/
/*MT6166*/ #define  QB_SR2M              22
/*MT6166*/ #define  QB_SR3               3    /* SR3 should be larger than (QB_RX_FSYNC_2_FENA+2QB) */
/*MT6166*/ #define  QB_PR1               167  /* QB_PR1>QB_SR1 to prevent RF conflict among 2/3G    */
/*MT6166*/ #define  QB_PR2               50
/*MT6166*/ #define  QB_PR3               7
/*MT6166*/ #define  QB_ST1               280
/*MT6166*/ #define  QB_ST2B              33   /* ST2B should be larger  than (QB_TX_FENA_2_FSYNC+8QB)*/
/*MT6166*/ #define  QB_ST3               38   /* ST3  should be larger  than (QB_TX_FSYNC_2_FENA+7QB)*/
/*MT6166*/ #define  QB_PT1               282  /* QB_PT1>QB_ST1 to prevent RF conflict among 2/3G     */
/*MT6166*/ #define  QB_PT2               56
/*MT6166*/ #define  QB_PT2B              6
/*MT6166*/ #define  QB_PT3               33
/*MT6166*/ #define  QB_ST2M_G8           10
/*MT6166*/ #define  QB_ST2M_8G           6
/*MT6166*/ #define  QB_PT2M1_G8          -1
/*MT6166*/ #define  QB_PT2M2_G8          -3
/*MT6166*/ #define  QB_PT2M1_8G          10
/*MT6166*/ #define  QB_PT2M2_8G          4
/*MT6166*/
/*MT6166*/
/*MT6166*/ #define  QB_APCON             14 //OH:11
/*MT6166*/ #define  QB_APCMID            20 //OH:18
/*MT6166*/ #define  QB_APCOFF            6  //56: 6
/*MT6166*/ #define  QB_APCDACON          18 //0
/*MT6166*/ #define  TX_PROPAGATION_DELAY 47 //56:47 / OH:46
/*MT6166*/
/*MT6166*/
/*MT6166*/ /*--------------------------------------------------*/
/*MT6166*/ /*   define  BPI data for MT6166                    */
/*MT6166*/ /*--------------------------------------------------*/
/*MT6166*/ /*  PRCB : bit   pin                                */
/*MT6166*/ /*          0    ASM_VCTRL_A                        */
/*MT6166*/ /*          1    ASM_VCTRL_B                        */
/*MT6166*/ /*          2    ASM_VCTRL_C                        */
/*MT6166*/ /*          3    ASM_VCTRL_D/WG_GGE_PA_EN           */
/*MT6166*/ /*         12    SP3T_A_V1                          */
/*MT6166*/ /*         13    SP3T_B_V2                          */
/*MT6166*/ /*         16    ANTENNA_CONFLICT_2G                */
/*MT6166*/ /*         17    SPI_SWITCH_TO_2G                   */
/*MT6166*/ /*         18    GSM_ERR_DET_ID                     */
/*MT6166*/ /*         20    M23G_PA_INDICATOR                  */
/*MT6166*/ /*--------------------------------------------------*/
/*MT6166*/
/*MT6166*/ /*------------------------------------------------------*/
/*MT6166*/ /*  ANTENNA_CONFLICT_2G(Pin:16) has no dedicate output  */
/*MT6166*/ /*  pin, and it is mapped to bit "16" for SW control.   */
/*MT6166*/ /*  For accurate RF conflict detection, this value must */
/*MT6166*/ /*  set "16" and is unchangable.                        */
/*MT6166*/ /*------------------------------------------------------*/
/*MT6166*/ #define  ANTENNA_CONFLICT_2G    (               16) /* For accurate RF conflict detection(2G/TDD), this value must set "16" */
/*MT6166*/                                                     /* and is unchangable.                                                  */
/*MT6166*/ #define  PDATA_GSM_ERR_DET      (1<<ANTENNA_CONFLICT_2G)
/*MT6166*/ #define  SPI_SWITCH_TO_2G       (1<<17)
/*MT6166*/
/*MT6166*/ #define  PDATA_GMSK              0x000000
/*MT6166*/ #define  PDATA_8PSK              0x000004
/*MT6166*/ 
/*MT6166*/ #define  PDATA_GSM850_PR1       (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PR2       (0x000006           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PR3       (0x000000                                              )
/*MT6166*/ #define  PDATA_GSM_PR1          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PR2          (0x000006           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PR3          (0x000000                                              )
/*MT6166*/ #define  PDATA_DCS_PR1          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PR2          (0x000004           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PR3          (0x000000                                              )
/*MT6166*/ #define  PDATA_PCS_PR1          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PR2          (0x000004           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PR3          (0x000000                                              )
/*MT6166*/ #define  PDATA_GSM850_PT1       (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT2       (0x000001           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT2B      (0x100009           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT3       (0x000000                                              )
/*MT6166*/ #define  PDATA_GSM850_PT2M1_G8  (0x100009|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT2M2_G8  (0x100009|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT2M1_8G  (0x000001           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM850_PT2M2_8G  (0x100009           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT1          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT2          (0x000001           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT2B         (0x100009           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT3          (0x000000                                              )
/*MT6166*/ #define  PDATA_GSM_PT2M1_G8     (0x100009|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT2M2_G8     (0x100009|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT2M1_8G     (0x000001           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_GSM_PT2M2_8G     (0x100009           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT1          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT2          (0x000003           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT2B         (0x10000B           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT3          (0x000000                                              )
/*MT6166*/ #define  PDATA_DCS_PT2M1_G8     (0x10000B|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT2M2_G8     (0x10000B|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G) 
/*MT6166*/ #define  PDATA_DCS_PT2M1_8G     (0x000003           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_DCS_PT2M2_8G     (0x10000B           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT1          (0x000000           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT2          (0x000003           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT2B         (0x10000B           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT3          (0x000000                                              )
/*MT6166*/ #define  PDATA_PCS_PT2M1_G8     (0x10000B|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT2M2_G8     (0x10000B|PDATA_8PSK|PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT2M1_8G     (0x000003           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_PCS_PT2M2_8G     (0x10000B           |PDATA_GSM_ERR_DET|SPI_SWITCH_TO_2G)
/*MT6166*/ #define  PDATA_INIT             (0x000000                                              )
/*MT6166*/ #define  PDATA_IDLE             (0x000000                                              )
#endif
/*MT6166*/
/*MT6166*//*----------------------------------------------*/
/*MT6166*//*   APC Compensate Thresholds                  */
/*MT6166*//*----------------------------------------------*/
/*MT6166*/
/*MT6166*/ #define    SECONDS2FRAME(n)                     ((int)((n)*1000/4.615))
/*MT6166*/ #define    VOLT2UVOLT(n)                        ((int)((n)*1000000))
/*MT6166*/ #define    TEMP2MTEMP(n)                        ((int)((n)*1000))
/*MT6166*/
/*MT6166*/ #define    BAT_VOLTAGE_SAMPLE_PERIOD_SECOND     180
/*MT6166*/ #define    BAT_VOLTAGE_SAMPLE_PERIOD            SECONDS2FRAME(BAT_VOLTAGE_SAMPLE_PERIOD_SECOND)
/*MT6166*/ #define    BAT_VOLTAGE_AVERAGE_COUNT            1
/*MT6166*/ #define    BAT_LOW_VOLTAGE_TRHESHOLD            (3.5)
/*MT6166*/ #define    BAT_HIGH_VOLTAGE_TRHESHOLD           (4.0)
/*MT6166*/ #define    BAT_LOW_VOLTAGE                      VOLT2UVOLT(BAT_LOW_VOLTAGE_TRHESHOLD)
/*MT6166*/ #define    BAT_HIGH_VOLTAGE                     VOLT2UVOLT(BAT_HIGH_VOLTAGE_TRHESHOLD)
/*MT6166*/
/*MT6166*/ #define    BAT_TEMPERATURE_SAMPLE_PERIOD_SECOND 180
/*MT6166*/ #define    BAT_TEMPERATURE_SAMPLE_PERIOD        SECONDS2FRAME(BAT_TEMPERATURE_SAMPLE_PERIOD_SECOND)
/*MT6166*/ #define    BAT_TEMPERATURE_AVERAGE_COUNT        1
/*MT6166*/ #define    BAT_LOW_TEMPERATURE_TRHESHOLD        (0)
/*MT6166*/ #define    BAT_HIGH_TEMPERATURE_TRHESHOLD       (50)
/*MT6166*/ #define    BAT_LOW_TEMPERATURE                  TEMP2MTEMP(BAT_LOW_TEMPERATURE_TRHESHOLD)
/*MT6166*/ #define    BAT_HIGH_TEMPERATURE                 TEMP2MTEMP(BAT_HIGH_TEMPERATURE_TRHESHOLD)
/*MT6166*/
/*MT6166*/ #define    RF_TEMPERATURE_SAMPLE_PERIOD_SECOND  1
/*MT6166*/ #define    RF_TEMPERATURE_SAMPLE_PERIOD         SECONDS2FRAME(RF_TEMPERATURE_SAMPLE_PERIOD_SECOND)
/*MT6166*/ #define    RF_TEMPERATURE_AVERAGE_COUNT         1
/*MT6166*/
/*MT6166*//*----------------------------------------------*/
/*MT6166*//*   Voltage Compensate Parameter               */
/*MT6166*//*----------------------------------------------*/
/*MT6166*/
/*MT6166*/ #define    MINUTES2FRAME(n)                     ((int)((n)*13000))
/*MT6166*/ #define    AP_UPDATE_VOLTINFO_PERIOD            MINUTES2FRAME(5)
/*MT6166*/
/*MT6166*//*----------------------------------------------*/
/*MT6166*//*   Crystal parameter                          */
/*MT6166*//*----------------------------------------------*/
/*MT6166*/ #if  IS_AFC_VCXO_SUPPORT
/*MT6166*/ #define Custom_RF_XO_CapID   145 /* RF SOP, Range:0~255 */
/*MT6166*/ #else
/*MT6166*/ #define Custom_RF_XO_CapID   0   /* For MT6166 with VCTCXO */
/*MT6166*/ #endif
/*MT6166*/
/*MT6166*/ /********************************************/
/*MT6166*/ /* Define your band mode selection on       */
/*MT6166*/ /* High Band and Low Band receivers         */
/*MT6166*/ /*        LNA_1 : 2GHB                      */
/*MT6166*/ /*        LNA_2 : 2GLB                      */
/*MT6166*/ /*        LNA_3 : B40                       */
/*MT6166*/ /*        LNA_4 : B1                        */
/*MT6166*/ /*        LNA_5 : B5                        */
/*MT6166*/ /*        LNA_6 : B2                        */
/*MT6166*/ /*        LNA_7 : B8                        */
/*MT6166*/ /********************************************/
/*MT6166*/
/*MT6166*/ #define GSM850_PATH_SEL LNA_2
/*MT6166*/ #define GSM_PATH_SEL    LNA_2
/*MT6166*/ #define DCS_PATH_SEL    LNA_1
/*MT6166*/ #define PCS_PATH_SEL    LNA_1
/*MT6166*/
/*MT6166*//*======================================================================================== */
/*MT6166*/
/*MT6166*/ /*----------------------------------------------*/
/*MT6166*/ /*   TX Power Control (TXPC) Support            */
/*MT6166*/ /*----------------------------------------------*/
/*MT6166*/
/*MT6166*/ #define IS_BSI_CLOSED_LOOP_TXPC_ON      0
/*MT6166*/
/*MT6166*/ #define TXPC_EPSK_TP_SLOPE_LB          ((26<<8)+22) /* Unit: degree/dB. Temperature increment that causes 1-dB EPSK TX power drop */
/*MT6166*/ #define TXPC_EPSK_TP_SLOPE_HB          ((27<<8)+22) /* Two slope method : [( temp<20:slpoe1)<<8 + (temp>=20:slpoe2)], slope must < 256 */
/*MT6166*/
/*MT6166*/ /*----------------------------------------------*/
/*MT6166*/ /*   DCXO LPM parameter                         */
/*MT6166*/ /*----------------------------------------------*/
/*MT6166*/ #define CUSTOM_CLOAD_FREQ_OFFSET   88940 /*in unit of Hz*/
/*MT6166*/
/*MT6166*/ /*----------------------------------------------------*/
/*MT6166*/ /*   Enable or disable the clock1, 2, 3, and 4 output */
/*MT6166*/ /*   1 : Enable                                       */
/*MT6166*/ /*   0 : Disable                                      */
/*MT6166*/ /*----------------------------------------------------*/
/*MT6166*/ #define CLK1_EN                         1 /* CLK1 is enabled for BB */
/*MT6166*/ #define CLK2_EN                         1
/*MT6166*/ #define CLK3_EN                         0
/*MT6166*/ #define CLK4_EN                         1
/*MT6166*/
/*MT6166*/ /*----------------------------------------------*/
/*MT6166*/ /*   TX power rollback parameter                */
/*MT6166*/ /*----------------------------------------------*/
/*MT6166*/ /*Unit: 1/8 dB*/
/*MT6166*/ /*GSM850 GMSK*/
/*MT6166*/ #define GSM850_TX_ROLLBACK_2T_GMSK      8
/*MT6166*/ #define GSM850_TX_ROLLBACK_3T_GMSK     24
/*MT6166*/ #define GSM850_TX_ROLLBACK_4T_GMSK     32
/*MT6166*/ #define GSM850_TX_ROLLBACK_5T_GMSK     40
/*MT6166*/ 
/*MT6166*/ /*GSM GMSK*/
/*MT6166*/ #define GSM_TX_ROLLBACK_2T_GMSK         8
/*MT6166*/ #define GSM_TX_ROLLBACK_3T_GMSK        24
/*MT6166*/ #define GSM_TX_ROLLBACK_4T_GMSK        32
/*MT6166*/ #define GSM_TX_ROLLBACK_5T_GMSK        40
/*MT6166*/ 
/*MT6166*/ /*DCS GMSK*/
/*MT6166*/ #define DCS_TX_ROLLBACK_2T_GMSK         8
/*MT6166*/ #define DCS_TX_ROLLBACK_3T_GMSK        24
/*MT6166*/ #define DCS_TX_ROLLBACK_4T_GMSK        32
/*MT6166*/ #define DCS_TX_ROLLBACK_5T_GMSK        40
/*MT6166*/ 
/*MT6166*/ /*PCS GMSK*/
/*MT6166*/ #define PCS_TX_ROLLBACK_2T_GMSK         8
/*MT6166*/ #define PCS_TX_ROLLBACK_3T_GMSK        24
/*MT6166*/ #define PCS_TX_ROLLBACK_4T_GMSK        32
/*MT6166*/ #define PCS_TX_ROLLBACK_5T_GMSK        40
/*MT6166*/ 
/*MT6166*/ /*GSM850 EPSK*/
/*MT6166*/ #define GSM850_TX_ROLLBACK_2T_EPSK      8
/*MT6166*/ #define GSM850_TX_ROLLBACK_3T_EPSK     24
/*MT6166*/ #define GSM850_TX_ROLLBACK_4T_EPSK     32
/*MT6166*/ #define GSM850_TX_ROLLBACK_5T_EPSK     40
/*MT6166*/ 
/*MT6166*/ /*GSM EPSK*/
/*MT6166*/ #define GSM_TX_ROLLBACK_2T_EPSK         8
/*MT6166*/ #define GSM_TX_ROLLBACK_3T_EPSK        24
/*MT6166*/ #define GSM_TX_ROLLBACK_4T_EPSK        32
/*MT6166*/ #define GSM_TX_ROLLBACK_5T_EPSK        40
/*MT6166*/ 
/*MT6166*/ /*DCS EPSK*/
/*MT6166*/ #define DCS_TX_ROLLBACK_2T_EPSK         8
/*MT6166*/ #define DCS_TX_ROLLBACK_3T_EPSK        24
/*MT6166*/ #define DCS_TX_ROLLBACK_4T_EPSK        32
/*MT6166*/ #define DCS_TX_ROLLBACK_5T_EPSK        40
/*MT6166*/ 
/*MT6166*/ /*PCS EPSK*/
/*MT6166*/ #define PCS_TX_ROLLBACK_2T_EPSK         8
/*MT6166*/ #define PCS_TX_ROLLBACK_3T_EPSK        24
/*MT6166*/ #define PCS_TX_ROLLBACK_4T_EPSK        32
/*MT6166*/ #define PCS_TX_ROLLBACK_5T_EPSK        40
/*MT6166*/ /*============================================================================== */
#endif

