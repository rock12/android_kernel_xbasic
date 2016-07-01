#ifndef _TL1D_CUSTOM_RF_H_
#define _TL1D_CUSTOM_RF_H_



typedef  struct {
kal_uint16 uwAction1;
kal_int16  swAction1TimingAdv;
kal_uint16 uwAction2;
kal_int16  swAction2TimingAdv;
kal_uint16 uwAction3;
kal_int16  swAction3TimingAdv;
kal_uint16 uwAction4;
kal_int16  swAction4TimingAdv;
kal_uint16 uwAction5;
kal_int16  swAction5TimingAdv;
kal_uint16 uwAction6;
kal_int16  swAction6TimingAdv;
kal_uint16 uwAction7;
kal_int16  swAction7TimingAdv;
kal_uint16 uwAction8;
kal_int16  swAction8TimingAdv;
kal_uint16 uwAction9;
kal_int16  swAction9TimingAdv;
kal_uint16 uwAction10;
kal_int16  swAction10TimingAdv;
kal_uint16 uwAction11;
kal_int16  swAction11TimingAdv;
kal_uint16 uwAction12;
kal_int16  swAction12TimingAdv;
kal_uint16 uwAction13;
kal_int16  swAction13TimingAdv;
kal_uint16 uwAction14;
kal_int16  swAction14TimingAdv;
kal_uint16 uwAction15;
kal_int16  swAction15TimingAdv;
kal_uint16 uwAction16;
kal_int16  swAction16TimingAdv;
kal_uint16 uwAction17;
kal_int16  swAction17TimingAdv;
kal_uint16 uwAction18;
kal_int16  swAction18TimingAdv;
kal_uint16 uwAction19;
kal_int16  swAction19TimingAdv;
kal_uint16 uwAction20;
kal_int16  swAction20TimingAdv;
kal_uint16 uwAction21;
kal_int16  swAction21TimingAdv;
kal_uint16 uwAction22;
kal_int16  swAction22TimingAdv;
kal_uint16 uwAction23;
kal_int16  swAction23TimingAdv;
kal_uint16 uwAction24;
kal_int16  swAction24TimingAdv;
kal_uint16 uwAction25;
kal_int16  swAction25TimingAdv;
/*kal_uint16 uwAction26;
kal_int16  swAction26TimingAdv;
kal_uint16 uwAction27;
kal_int16  swAction27TimingAdv;
kal_uint16 uwAction28;
kal_int16  swAction28TimingAdv;
kal_uint16 uwAction29;
kal_int16  swAction29TimingAdv;
kal_uint16 uwAction30;
kal_int16  swAction30TimingAdv;*/
} T_ACTION_TIMING_STRUCT;

typedef  struct {	
T_ACTION_TIMING_STRUCT tEnableDownlink;
T_ACTION_TIMING_STRUCT tDisableDownlink;
T_ACTION_TIMING_STRUCT tEnableUplink;
T_ACTION_TIMING_STRUCT tDisableUplink;
T_ACTION_TIMING_STRUCT tDlsGapCtrl;
T_ACTION_TIMING_STRUCT tUlsGapCtrl;
T_ACTION_TIMING_STRUCT tUlDlGapCtrl;
T_ACTION_TIMING_STRUCT tEnableUplinkF;
T_ACTION_TIMING_STRUCT tDisableUplinkF;
T_ACTION_TIMING_STRUCT tEnableUplinkE;
T_ACTION_TIMING_STRUCT tDisableUplinkE;

} T_RF_PROG_SEQ_STRUCT;



typedef  struct {
kal_uint32 uwTxCtrlUseBitsSet;
kal_uint32 uwTx2G34PaHModeSet;
kal_uint32 uwTx2G34PaMModeSet;
kal_uint32 uwTx2G34PaLModeSet;
kal_uint32 uwTx2G01PaHModeSet;
kal_uint32 uwTx2G01PaMModeSet;
kal_uint32 uwTx2G01PaLModeSet;
kal_uint32 uwTx1G90PaHModeSet;
kal_uint32 uwTx1G90PaMModeSet;
kal_uint32 uwTx1G90PaLModeSet;
kal_uint32 uwTxCtrlDisable;
} BPI_AREA1_STRUCT;


typedef  struct {
kal_uint32 uwRxCtrlUseBitsSet;
kal_uint32 uwRxBand2G34Set;
kal_uint32 uwRxBand2G01Set;
kal_uint32 uwRxBand1G90Set;
kal_uint32 uwRxCtrlDisable;
} BPI_AREA2_STRUCT;


typedef  struct {
kal_uint32 uwPaEnableUseBitsSet;
kal_uint32 uwPaEnable2G34Set;
kal_uint32 uwPaEnable2G01Set;
kal_uint32 uwPaEnable1G90Set;
kal_uint32 uwPaDisable;
} BPI_AREA3_STRUCT;

typedef  struct {
kal_uint32 uwAreaUseBitsSet;
kal_uint32 uwAreaEnable;
kal_uint32 uwAreaDisable;
} BPI_AREANORMAL_STRUCT;

typedef  struct {
kal_uint32 uwPaStage;
BPI_AREA1_STRUCT tBpiArea1PaModeSet;
BPI_AREA2_STRUCT tBpiArea2BandModeSel;
BPI_AREA3_STRUCT tBpiArea3PaCtrlSet;
BPI_AREANORMAL_STRUCT  tBpiArea4Set;
BPI_AREANORMAL_STRUCT  tBpiArea5Set;
} T_BPI_SETTING_STRUCT;


typedef  struct {
kal_uint16 uwApcHighGainV;
kal_uint16 uwApcMidGainV;
kal_uint16 uwApcLowGainV;
kal_uint16 uwPadding;
} T_ABB_APC_VOLT_STRUCT;

typedef  struct {
kal_uint16 uwTx2G01SelRFport;  /*TX BAND34*/
kal_uint16 uwTx1G90SelRFport;  /*TX BAND39*/
kal_uint16 uwTx2G34SelRFport;  /*TX BAND40*/
kal_uint16 uwRx2G01SelRFport;  /*RX BAND34*/
kal_uint16 uwRx1G90SelRFport;  /*RX BAND39*/
kal_uint16 uwRx2G34SelRFport;  /*RX BAND40*/
} T_RF_PORT_SEL_STRUCT;


typedef  struct {
kal_uint16 uwRXIQSwap;  /*RX  0:  NO IQ SWAP     1: IQ SWAP */
kal_uint16 uwTXIQSwap;  /*TX  0:  NO IQ SWAP     1: IQ SWAP */
} T_IQ_SWAP_CFG_STRUCT;

typedef  struct {
T_BPI_SETTING_STRUCT   tBpiSetting;
T_ABB_APC_VOLT_STRUCT  tAbbApcVoltCfg;
T_RF_PORT_SEL_STRUCT  tOtPortSelCfg;
T_IQ_SWAP_CFG_STRUCT   tTrxIQswapCfg;
} T_TD_CUSTOMIZATION_STRUCT;


#if defined(__AST3001__) && defined(MT6589)
#define __AST_TL1_TDD_RF_PARAMETER_SUPPORT__
#endif


#if defined(_T12193_C_) || defined(__AST_TL1_TDD_RF_PARAMETER_DEFINE__)

#ifdef __UMTS_TDD128_BAND_E__
#define E_EN  1    //0   BAND_E  disable     1:    BAND_E    enable
#else
#define E_EN  0    //0   BAND_E  disable     1:    BAND_E    enable     
#endif

/*PlatForm : MT6572  + MT6166_RF*/
#if ( defined (MT6572) || defined (MT6582) ) && defined (MT6166_RF) 
#define SKY77590
#define INTERNAL_SW  1   //0 internal sw disable     1: internal sw enable
//#define RF9811
//#define INTERNAL_SW  0   //0 internal sw disable     1: internal sw enable
#endif

/*don't modify the name and value*/
#if defined (MT6166_RF)
#define RF_TX_2GHB       (1<<0)  /*TX HB1*/
#define RF_TX_3GHB1      (1<<1)  /*TX HB2*/
#define RF_TX_3GHB2      (1<<2)  /*TX HB3*/
#define RF_TX_3GL5       (1<<3)  /*TX LB1*/
#define RF_TX_2GLB       (1<<4)  /*TX LB2*/ 
#define RF_RX_2GHB       1
#define RF_RX_2GLB       2
#define RF_RX_TDDB40     3
#define RF_RX_3GB1       4
#define RF_RX_3GB5       5
#define RF_RX_3GB2       6
#define RF_RX_3GB8       7
#elif defined (ORION_GT)
#define RF_TX_HB1       (4<<4)
#define RF_TX_HB2       (2<<4)
#define RF_TX_HB3       (1<<4)
#define RF_TX_LB3       (8<<4)
#define RF_RX_850       0
#define RF_RX_900       1
#define RF_RX_DCS       2
#define RF_RX_PCS       3
#define RF_RX_B39       5
#define RF_RX_B34       4
#define RF_RX_B40       6
#endif
/*don't modify the name and value*/

#if defined(_T12193_C_)
kal_int16 const TD_GSM_COCRYSTAL[]={
	1,
};
#endif
	
#if (defined(_T12193_C_) && !defined(__AST_TL1_TDD_RF_PARAMETER_SUPPORT__)) \
	|| (defined(__AST_TL1_TDD_RF_PARAMETER_DEFINE__) && defined(__AST_TL1_TDD_RF_PARAMETER_SUPPORT__))


#if ( defined (MT6572) || defined (MT6582) ) && defined (MT6166_RF) 
#if defined (RF9810)
 /*--------------------------------------------------------*/
 /*           bit   pin                                    */
 /*            0    GPCTRL0                                */                                     
 /*            1    GPCTRL1                            */                                     
 /*            2    GPCTRL2                             */                                     
 /*            3    TX_EN                                  */                                     
 /*            4    not used                                    */                                     
 /*            5    not used                               */                                     
 /*            6    not used                                    */                                     
 /*            7    not used                                */                                     
 /*            8    not used                                */                                     
 /*            9    not used                                 */  
 /*            10   not used                               */ 
 /*            11   not used                               */ 
 /*            12   not used                                  */       
 /*            13   not used                               */   
 /*            14   not used                               */                         
 /*--------------------------------------------------------*/ 
 T_TD_CUSTOMIZATION_STRUCT    AST_TL1_RF_PARAMETER_DEFAULT[] = {{ 
 //T_BPI_SETTING 
 {
  //TX_2G34   TX_2G01/TX_1G90 
 // [7:4]     [3:0] 	
 	 (1<<4)     +   1,                                 //PA_STAGE          data[0]
 	 																						       //3:PA_HGAIN PA_MGAIN PA_LGAIN
 	 																						       //2:PA_HGAIN PA_MGAIN
 	 																						       //1:PA_HGAIN
 

   {	
  /////////////////////DBB_BPI_AREA1 begin (fix for TX Control )/////////////////////////////
  //GPTRL2  GPTRL1  GPTRL0            RFconflict
   (1<<2) +(1<<1)  +(1<<0)         +(1<<31),     //TX_CTRL_BITMASK   data[1]
   (0<<2) +(0<<1)  +(0<<0)         +(1<<31),     //TX_2G34 PA_HGAIN  data[2]
   (0<<2) +(0<<1)  +(0<<0)         +(1<<31),     //TX_2G34 PA_MGAIN  data[3]
   (0<<2) +(0<<1)  +(0<<0)         +(1<<31),     //TX_2G34 PA_LGAIN  data[4]
   (0<<2) +(0<<1)  +(1<<0)         +(1<<31),     //TX_2G01 PA_HGAIN  data[5]
   (0<<2) +(0<<1)  +(1<<0)         +(1<<31),     //TX_2G01 PA_MGAIN  data[6]
   (0<<2) +(0<<1)  +(1<<0)         +(1<<31),     //TX_2G01 PA_LGAIN  data[7]
   (0<<2) +(0<<1)  +(1<<0)         +(1<<31),     //TX_1G90 PA_HGAIN  data[8]
   (0<<2) +(0<<1)  +(1<<0)         +(1<<31),     //TX_1G90 PA_MGAIN  data[9]
   (0<<2) +(0<<1)  +(1<<0)         +(1<<31),     //TX_1G90 PA_LGAIN  data[10]
   (0<<2) +(0<<1)  +(0<<0)         +(0<<31)      //TX_DISABLE        data[11]
 /////////////////////DBB_BPI_AREA1 end///////////////////////////////////////////////////
    },

    {
 /////////////////////DBB_BPI_AREA2 begin (fix for RX Control)///////////////////////////
   //GPTRL2  GPTRL1  GPTRL0       RFconflict    TXEN     
   (1<<2) +(1<<1)  +(1<<0)        +(1<<31)   + (1<<3),      //RX_CTRL_BITMASK   data[12]
   (0<<2) +(0<<1)  +(0<<0)        +(1<<31)   + (0<<3),      //RX_2G34           data[13]
   (0<<2) +(1<<1)  +(1<<0)        +(1<<31)   + (0<<3),      //RX_2G01           data[14]
   (0<<2) +(1<<1)  +(1<<0)        +(1<<31)   + (0<<3),      //RX_1G90           data[15]
   (0<<2) +(0<<1)  +(0<<0)        +(0<<31)   + (0<<3)       //RX_DISABLE        data[16]
  /////////////////////DBB_BPI_AREA2 end///////////////////////////////////////////////////
    },

    {
  /////////////////////DBB_BPI_AREA3 begin/////////////////////////////////////////////////   
  //TXEN    VEN
    (1<<3),                                      //PA_BITMASK       data[17]
    (0<<3),                                         //PA_ENABLE_2G34   data[18]    
    (1<<3),                                         //PA_ENABLE_2G01   data[19] 
    (1<<3),                                         //PA_ENABLE_1G90   data[20] 
    (0<<3)                                         //PA_DISABLE       data[21]  
  /////////////////////DBB_BPI_AREA3 end///////////////////////////////////////////////////
    },

    {
  /////////////////////DBB_BPI_AREA4 begin/////////////////////////////////////////////////   
 //enable/disable Other fuction 
    (0<<0),                                             //?_BITMASK        data[22]
    (0<<0),                                             //?_ENABLE         data[23]    
    (0<<0)                                              //?_DISABLE        data[24]   
  /////////////////////DBB_BPI_AREA4 end///////////////////////////////////////////////////                                                                                     
    },

    {
  /////////////////////DBB_BPI_AREA5 begin/////////////////////////////////////////////////   
  //enable/disable Other fuction 
    (0<<0),                                             //?_BITMASK        data[25]
    (0<<0),                                             //?_ENABLE         data[26]    
    (0<<0)                                             //?_DISABLE        data[27]   
  /////////////////////DBB_BPI_AREA5 end///////////////////////////////////////////////////
    }
 },

//T_ABB_APC_VOLT 
 {
     0x25,//HGv = 1.6v
     0x19,//MGv = 1.1v
     0x10,//LGv = 0.7v
     0x1e //UPAV = 2.0v not use
 },

//T_RF_PORT_SEL
 {
     RF_TX_2GHB,  //Tx2G01
     RF_TX_2GHB,  //Tx1G90
     RF_TX_3GHB1, //Tx2G34
     RF_RX_2GHB,  //Rx2G01
     RF_RX_2GHB,  //Rx1G90
     RF_RX_TDDB40 //Rx2G34
 },

//T_IQ_SWAP_CFG_STRUCT
 {
     0,//RX :0    IQswap diable     1:    IQswap enable
     0 //TX :0    IQswap diable     1:    IQswap enable
 }
}};
 #endif
 #endif

#if ( defined (MT6572) || defined (MT6582) ) && defined (MT6166_RF) 
#if defined (SKY77590)
 /*--------------------------------------------------------*/
 /*           bit   pin                                    */
 /*            0    GPCTRL0                                */                                     
 /*            1    GPCTRL1                            */                                     
 /*            2    GPCTRL2                             */                                     
 /*            3    TX_EN                                  */                                     
 /*            4    not used                                    */                                     
 /*            5    not used                               */                                     
 /*            6    not used                                    */                                     
 /*            7    not used                                */                                     
 /*            8    not used                                */                                     
 /*            9    not used                                 */  
 /*            10   not used                               */ 
 /*            11   not used                                */ 
 /*            12   not used                                 */       
 /*            13   not used                              */   
 /*            14   not used                               */                         
 /*--------------------------------------------------------*/ 
 T_TD_CUSTOMIZATION_STRUCT    AST_TL1_RF_PARAMETER_DEFAULT[] = {{ 
 //T_BPI_SETTING 
 {
  //TX_2G34   TX_2G01/TX_1G90 
 // [7:4]     [3:0] 	
 	 (1<<4)     +   1,                                 //PA_STAGE          data[0]
 	 																						       //3:PA_HGAIN PA_MGAIN PA_LGAIN
 	 																						       //2:PA_HGAIN PA_MGAIN
 	 																						       //1:PA_HGAIN
 

   {	
  /////////////////////DBB_BPI_AREA1 begin (fix for TX Control )/////////////////////////////
  //GPTRL2  GPTRL1  GPTRL0           RFconflict
   (1<<2) +(1<<1)  +(1<<0)         +(1<<31),     //TX_CTRL_BITMASK   data[1]
   (0<<2) +(1<<1)  +(0<<0)         +(1<<31),     //TX_2G34 PA_HGAIN  data[2]
   (0<<2) +(1<<1)  +(0<<0)         +(1<<31),     //TX_2G34 PA_MGAIN  data[3]
   (0<<2) +(1<<1)  +(0<<0)         +(1<<31),     //TX_2G34 PA_LGAIN  data[4]
   (1<<2) +(1<<1)  +(1<<0)         +(1<<31),     //TX_2G01 PA_HGAIN  data[5]
   (1<<2) +(1<<1)  +(1<<0)         +(1<<31),     //TX_2G01 PA_MGAIN  data[6]
   (0<<2) +(0<<1)  +(0<<0)         +(1<<31),     //TX_2G01 PA_LGAIN  data[7]
   (1<<2) +(1<<1)  +(1<<0)         +(1<<31),     //TX_1G90 PA_HGAIN  data[8]
   (1<<2) +(1<<1)  +(1<<0)         +(1<<31),     //TX_1G90 PA_MGAIN  data[9]
   (0<<2) +(0<<1)  +(0<<0)         +(1<<31),     //TX_1G90 PA_LGAIN  data[10]
   (0<<2) +(0<<1)  +(0<<0)         +(0<<31)      //TX_DISABLE        data[11]
 /////////////////////DBB_BPI_AREA1 end///////////////////////////////////////////////////
    },

    {
 /////////////////////DBB_BPI_AREA2 begin (fix for RX Control)///////////////////////////
   //GPTRL2  GPTRL1  GPTRL0        RFconflict    TXEN      
   (1<<2) +(1<<1)  +(1<<0)        +(1<<31)   + (1<<3),     //RX_CTRL_BITMASK   data[12]
   (1<<2) +(0<<1)  +(1<<0)            +(1<<31)   + (0<<3) ,     //RX_2G34           data[13]
   (1<<2) +(0<<1)  +(0<<0)        +(1<<31)   + (0<<3),     //RX_2G01           data[14]
   (1<<2) +(0<<1)  +(0<<0)        +(1<<31)   + (0<<3),     //RX_1G90           data[15]
   (0<<2) +(0<<1)  +(0<<0)        +(0<<31)   + (0<<3)      //RX_DISABLE        data[16]
  /////////////////////DBB_BPI_AREA2 end///////////////////////////////////////////////////
    },

    {
  /////////////////////DBB_BPI_AREA3 begin/////////////////////////////////////////////////   
  //TXEN    
    (1<<3),                                      //PA_BITMASK       data[17]
    (0<<3),                                         //PA_ENABLE_2G34   data[18]    
    (1<<3),                                         //PA_ENABLE_2G01   data[19] 
    (1<<3),                                         //PA_ENABLE_1G90   data[20] 
    (0<<3)                                          //PA_DISABLE       data[21]  
  /////////////////////DBB_BPI_AREA3 end///////////////////////////////////////////////////
    },

    {
  /////////////////////DBB_BPI_AREA4 begin/////////////////////////////////////////////////   
 //enable/disable Other fuction 
    (0<<0),                                             //?_BITMASK        data[22]
    (0<<0),                                             //?_ENABLE         data[23]    
    (0<<0)                                              //?_DISABLE        data[24]   
  /////////////////////DBB_BPI_AREA4 end///////////////////////////////////////////////////                                                                                     
    },

    {
  /////////////////////DBB_BPI_AREA5 begin/////////////////////////////////////////////////   
  //enable/disable Other fuction 
    (0<<0),                                             //?_BITMASK        data[25]
    (0<<0),                                             //?_ENABLE         data[26]    
    (0<<0)                                             //?_DISABLE        data[27]   
  /////////////////////DBB_BPI_AREA5 end///////////////////////////////////////////////////
    }
 },

//T_ABB_APC_VOLT 
 {
     0x33, //0x25,//HGv = 1.6v
     0x18, //0x10,//MGv = 0.7V
     0x18,//0x10,//LGv = 0.7v
     0x33, //0x1e //UPAV = 2.0v not use
 },

//T_RF_PORT_SEL
 {
     RF_TX_2GHB,  //Tx2G01
     RF_TX_2GHB,  //Tx1G90
     RF_TX_3GHB1, //Tx2G34
     RF_RX_2GHB,  //Rx2G01
     RF_RX_2GHB,  //Rx1G90
     RF_RX_TDDB40 //Rx2G34
 },

//T_IQ_SWAP_CFG_STRUCT
 {
     0,//RX :0    IQswap diable     1:    IQswap enable
     0 //TX :0    IQswap diable     1:    IQswap enable
 }
}};
 #endif
 #endif



#endif// __AST_TL1_TDD_RF_PARAMETER__

#else  //defined(_T12193_C_) || defined(__AST_TL1_TDD_RF_PARAMETER__)
extern const kal_uint16 NVRAM_EF_AST_TL1_AFC_DATA_DEFAULT[];
extern const kal_int16  NVRAM_EF_AST_TL1_PATHLOSS_33_35_37_39_DEFAULT[];

extern const kal_int16  NVRAM_EF_AST_TL1_PATHLOSS_34_DEFAULT[];
extern const kal_int16  NVRAM_EF_AST_TL1_PATHLOSS_40_DEFAULT[];

extern const kal_int16  NVRAM_EF_AST_TL1_TXDAC_33_35_37_39_DEFAULT[];

extern const kal_int16  NVRAM_EF_AST_TL1_TXDAC_34_DEFAULT[];
extern const kal_int16  NVRAM_EF_AST_TL1_TXDAC_40_DEFAULT[];

extern const kal_int16  NVRAM_EF_AST_TL1_ABB_CAL_DEFAULT[];
extern const kal_uint32  NVRAM_EF_AST_TL1_CAP_DATA_DEFAULT[];
extern const T_TD_CUSTOMIZATION_STRUCT    AST_TL1_RF_PARAMETER_DEFAULT[];
extern const kal_int16 TD_GSM_COCRYSTAL[];
#endif //defined(_T12193_C_) || defined(__AST_TL1_TDD_RF_PARAMETER__)

#endif
