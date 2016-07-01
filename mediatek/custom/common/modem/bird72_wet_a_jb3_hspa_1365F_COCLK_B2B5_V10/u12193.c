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
 *	u12193.c
 *
 * Project:
 * --------
 *   MT6582
 *
 * Description:
 * ------------
 *   MT6582 Power Ramp Table
 *
 * Author:
 * -------
 * -------
 *
 *******************************************************************************/
#if !defined(MT6166_RF)
   #error "rf files mismatch with compile option!"
#endif

/*===============================================================================================*/
#include "kal_public_api.h"
#include "ul1d_rf_public.h"

U_sTEMPAGCOFFSET AGC_PATHLOSS_BANDANY[CAL_TEMP_SECTION] =
{
   /*temp1*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp2*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp3*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp4*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp5*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp6*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp7*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp8*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
};

U_sTEMPAGCOFFSET AGC_PATHLOSS_BAND1[CAL_TEMP_SECTION] = 
{
   /*temp1*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 10562,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10631,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10700,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10769,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10838,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp2*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 10562,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10631,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10700,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10769,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10838,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp3*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 10562,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10631,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10700,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10769,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10838,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp4*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 10562,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10631,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10700,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10769,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10838,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp5*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 10562,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10631,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10700,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10769,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10838,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp6*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 10562,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10631,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10700,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10769,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10838,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp7*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 10562,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10631,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10700,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10769,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10838,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp8*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     { 10562,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10631,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10700,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10769,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 10838,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
};

U_sTEMPAGCOFFSET AGC_PATHLOSS_BAND2[CAL_TEMP_SECTION] = 
{
   /*temp1*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  9662,  U_GAINLOSS( -1.25 ), U_GAINLOSS( -1.25 ), U_GAINLOSS( -0.25 ) },
     {  9700,  U_GAINLOSS( -1.25 ), U_GAINLOSS( -1.50 ), U_GAINLOSS( -0.50 ) },
     {  9800,  U_GAINLOSS( -2.75 ), U_GAINLOSS( -3.25 ), U_GAINLOSS( -2.25 ) },
     {  9869,  U_GAINLOSS( -0.75 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( 0.000 ) },
     {  9938,  U_GAINLOSS( -1.75 ), U_GAINLOSS( -2.00 ), U_GAINLOSS( -1.25 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
   }},
   /*temp2*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  9662,  U_GAINLOSS( -1.25 ), U_GAINLOSS( -1.25 ), U_GAINLOSS( -0.25 ) },
     {  9700,  U_GAINLOSS( -1.25 ), U_GAINLOSS( -1.50 ), U_GAINLOSS( -0.50 ) },
     {  9800,  U_GAINLOSS( -2.75 ), U_GAINLOSS( -3.25 ), U_GAINLOSS( -2.25 ) },
     {  9869,  U_GAINLOSS( -0.75 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( 0.000 ) },
     {  9938,  U_GAINLOSS( -1.75 ), U_GAINLOSS( -2.00 ), U_GAINLOSS( -1.25 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
     { 0,  U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -1.00 ) },
   }},
    /*temp3*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  9662,  U_GAINLOSS( -0.25 ), U_GAINLOSS( -0.25 ), U_GAINLOSS( 0.750 ) },
     {  9700,  U_GAINLOSS( -0.25 ), U_GAINLOSS( -0.50 ), U_GAINLOSS( 0.500 ) },
     {  9800,  U_GAINLOSS( -1.75 ), U_GAINLOSS( -2.25 ), U_GAINLOSS( -1.25 ) },
     {  9869,  U_GAINLOSS( 0.250 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 1.000 ) },
     {  9938,  U_GAINLOSS( -0.75 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -0.25 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp4*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
      {  9662,  U_GAINLOSS( -0.25 ), U_GAINLOSS( -0.25 ), U_GAINLOSS( 0.750 ) },
     {  9700,  U_GAINLOSS( -0.25 ), U_GAINLOSS( -0.50 ), U_GAINLOSS( 0.500 ) },
     {  9800,  U_GAINLOSS( -1.75 ), U_GAINLOSS( -2.25 ), U_GAINLOSS( -1.25 ) },
     {  9869,  U_GAINLOSS( 0.250 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 1.000 ) },
     {  9938,  U_GAINLOSS( -0.75 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -0.25 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp5*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
      {  9662,  U_GAINLOSS( -0.25 ), U_GAINLOSS( -0.25 ), U_GAINLOSS( 0.750 ) },
     {  9700,  U_GAINLOSS( -0.25 ), U_GAINLOSS( -0.50 ), U_GAINLOSS( 0.500 ) },
     {  9800,  U_GAINLOSS( -1.75 ), U_GAINLOSS( -2.25 ), U_GAINLOSS( -1.25 ) },
     {  9869,  U_GAINLOSS( 0.250 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 1.000 ) },
     {  9938,  U_GAINLOSS( -0.75 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -0.25 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp6*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
      {  9662,  U_GAINLOSS( -0.25 ), U_GAINLOSS( -0.25 ), U_GAINLOSS( 0.750 ) },
     {  9700,  U_GAINLOSS( -0.25 ), U_GAINLOSS( -0.50 ), U_GAINLOSS( 0.500 ) },
     {  9800,  U_GAINLOSS( -1.75 ), U_GAINLOSS( -2.25 ), U_GAINLOSS( -1.25 ) },
     {  9869,  U_GAINLOSS( 0.250 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 1.000 ) },
     {  9938,  U_GAINLOSS( -0.75 ), U_GAINLOSS( -1.00 ), U_GAINLOSS( -0.25 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp7*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  9662,  U_GAINLOSS( 0.750 ), U_GAINLOSS( 0.750 ), U_GAINLOSS( 1.750 ) },
     {  9700,  U_GAINLOSS( 0.750 ), U_GAINLOSS( 0.500 ), U_GAINLOSS( 1.500 ) },
     {  9800,  U_GAINLOSS( -0.75 ), U_GAINLOSS( -1.25 ), U_GAINLOSS( -0.25 ) },
     {  9869,  U_GAINLOSS( 1.250 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 2.000 ) },
     {  9938,  U_GAINLOSS( 0.250 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.750 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
   }},
   /*temp8*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  9662,  U_GAINLOSS( 0.750 ), U_GAINLOSS( 0.750 ), U_GAINLOSS( 1.750 ) },
     {  9700,  U_GAINLOSS( 0.750 ), U_GAINLOSS( 0.500 ), U_GAINLOSS( 1.500 ) },
     {  9800,  U_GAINLOSS( -0.75 ), U_GAINLOSS( -1.25 ), U_GAINLOSS( -0.25 ) },
     {  9869,  U_GAINLOSS( 1.250 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 2.000 ) },
     {  9938,  U_GAINLOSS( 0.250 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.750 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
     { 0,  U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ), U_GAINLOSS( 1.000 ) },
   }},
};

U_sTEMPAGCOFFSET AGC_PATHLOSS_BAND3[CAL_TEMP_SECTION] = 
{
   /*temp1*/
   {{/* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  1162,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1187,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1212,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1262,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1287,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1338,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1363,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1388,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1413,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1438,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1463,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1488,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1513,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp2*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  1162,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1187,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1212,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1262,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1287,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1338,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1363,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1388,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1413,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1438,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1463,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1488,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1513,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
    /*temp3*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  1162,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1187,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1212,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1262,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1287,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1338,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1363,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1388,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1413,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1438,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1463,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1488,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1513,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp4*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  1162,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1187,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1212,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1262,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1287,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1338,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1363,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1388,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1413,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1438,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1463,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1488,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1513,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp5*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  1162,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1187,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1212,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1262,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1287,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1338,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1363,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1388,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1413,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1438,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1463,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1488,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1513,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp6*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  1162,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1187,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1212,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1262,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1287,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1338,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1363,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1388,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1413,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1438,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1463,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1488,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1513,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp7*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  1162,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1187,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1212,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1262,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1287,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1338,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1363,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1388,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1413,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1438,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1463,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1488,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1513,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp8*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  1162,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1187,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1212,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1262,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1287,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1338,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1363,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1388,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1413,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1438,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1463,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1488,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  1513,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
};

U_sTEMPAGCOFFSET AGC_PATHLOSS_BAND4[CAL_TEMP_SECTION] = 
{
   /*temp1*/
   {{/* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     { 1537,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1552,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1567,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1582,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1597,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1612,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1627,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1642,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1657,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1675,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1687,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1702,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1717,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1732,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1738,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp2*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     { 1537,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1552,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1567,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1582,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1597,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1612,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1627,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1642,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1657,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1675,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1687,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1702,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1717,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1732,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1738,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp3*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     { 1537,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1552,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1567,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1582,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1597,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1612,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1627,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1642,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1657,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1675,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1687,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1702,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1717,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1732,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1738,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp4*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     { 1537,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1552,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1567,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1582,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1597,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1612,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1627,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1642,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1657,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1675,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1687,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1702,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1717,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1732,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1738,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp5*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     { 1537,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1552,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1567,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1582,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1597,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1612,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1627,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1642,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1657,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1675,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1687,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1702,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1717,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1732,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1738,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp6*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     { 1537,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1552,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1567,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1582,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1597,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1612,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1627,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1642,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1657,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1675,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1687,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1702,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1717,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1732,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1738,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp7*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     { 1537,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1552,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1567,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1582,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1597,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1612,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1627,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1642,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1657,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1675,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1687,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1702,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1717,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1732,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1738,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp8*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     { 1537,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1552,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1567,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1582,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1597,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1612,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1627,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1642,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1657,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1675,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1687,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1702,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1717,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1732,  U_GAINLOSS( 0.000 ), 0, 0 },
     { 1738,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
};

U_sTEMPAGCOFFSET AGC_PATHLOSS_BAND5[CAL_TEMP_SECTION] = 
{
   /*temp1*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  4357,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4382,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4408,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4433,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4458,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp2*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  4357,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4382,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4408,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4433,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4458,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp3*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  4357,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4382,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4408,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4433,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4458,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp4*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  4357,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4382,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4408,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4433,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4458,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp5*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  4357,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4382,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4408,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4433,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4458,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp6*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  4357,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4382,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4408,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4433,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4458,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp7*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  4357,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4382,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4408,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4433,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4458,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
   /*temp8*/
   {{/* uarfcn,    LNA_H_loss,    LNA_M_loss,    LNA_L_loss */
     {  4357,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4382,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4408,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4433,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     {  4458,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
     { 0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }},
};

U_sTEMPAGCOFFSET AGC_PATHLOSS_BAND6[CAL_TEMP_SECTION] = 
{
   /*temp1*/
   {{/* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  4387,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4394,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4400,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4406,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4413,  U_GAINLOSS( 0.000 ), 0, 0 },
     { UMTS_TABLE_END }
   }},
   /*temp2*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  4387,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4394,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4400,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4406,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4413,  U_GAINLOSS( 0.000 ), 0, 0 },
     { UMTS_TABLE_END }
   }},
   /*temp3*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  4387,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4394,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4400,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4406,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4413,  U_GAINLOSS( 0.000 ), 0, 0 },
     { UMTS_TABLE_END }
   }},
   /*temp4*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  4387,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4394,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4400,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4406,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4413,  U_GAINLOSS( 0.000 ), 0, 0 },
     { UMTS_TABLE_END }
   }},
   /*temp5*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  4387,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4394,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4400,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4406,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4413,  U_GAINLOSS( 0.000 ), 0, 0 },
     { UMTS_TABLE_END }
   }},
   /*temp6*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  4387,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4394,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4400,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4406,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4413,  U_GAINLOSS( 0.000 ), 0, 0 },
     { UMTS_TABLE_END }
   }},
   /*temp7*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  4387,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4394,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4400,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4406,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4413,  U_GAINLOSS( 0.000 ), 0, 0 },
     { UMTS_TABLE_END }
   }},
   /*temp8*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  4387,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4394,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4400,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4406,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  4413,  U_GAINLOSS( 0.000 ), 0, 0 },
     { UMTS_TABLE_END }
   }},
};

U_sTEMPAGCOFFSET AGC_PATHLOSS_BAND8[CAL_TEMP_SECTION] = 
{
   /*temp1*/
   {{/* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  2937,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2948,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2959,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2970,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2981,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2992,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3003,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3014,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3025,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3036,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3047,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3058,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3069,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3080,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3088,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp2*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  2937,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2948,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2959,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2970,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2981,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2992,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3003,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3014,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3025,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3036,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3047,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3058,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3069,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3080,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3088,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp3*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  2937,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2948,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2959,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2970,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2981,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2992,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3003,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3014,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3025,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3036,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3047,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3058,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3069,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3080,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3088,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp4*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  2937,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2948,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2959,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2970,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2981,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2992,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3003,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3014,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3025,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3036,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3047,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3058,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3069,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3080,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3088,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp5*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  2937,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2948,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2959,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2970,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2981,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2992,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3003,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3014,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3025,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3036,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3047,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3058,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3069,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3080,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3088,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp6*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  2937,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2948,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2959,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2970,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2981,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2992,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3003,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3014,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3025,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3036,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3047,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3058,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3069,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3080,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3088,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp7*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  2937,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2948,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2959,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2970,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2981,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2992,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3003,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3014,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3025,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3036,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3047,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3058,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3069,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3080,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3088,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp8*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  2937,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2948,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2959,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2970,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2981,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  2992,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3003,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3014,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3025,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3036,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3047,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3058,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3069,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3080,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3088,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
};

U_sTEMPAGCOFFSET AGC_PATHLOSS_BAND9[CAL_TEMP_SECTION] = 
{
   /*temp1*/
   {{/* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  9237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9248,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9259,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9281,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9292,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9303,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9323,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9334,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9345,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9356,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9367,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9378,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp2*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  9237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9248,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9259,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9281,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9292,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9303,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9323,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9334,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9345,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9356,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9367,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9378,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp3*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  9237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9248,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9259,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9281,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9292,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9303,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9323,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9334,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9345,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9356,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9367,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9378,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp4*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  9237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9248,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9259,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9281,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9292,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9303,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9323,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9334,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9345,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9356,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9367,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9378,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp5*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  9237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9248,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9259,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9281,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9292,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9303,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9323,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9334,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9345,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9356,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9367,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9378,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp6*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  9237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9248,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9259,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9281,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9292,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9303,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9323,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9334,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9345,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9356,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9367,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9378,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp7*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  9237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9248,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9259,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9281,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9292,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9303,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9323,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9334,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9345,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9356,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9367,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9378,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp8*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  9237,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9248,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9259,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9281,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9292,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9303,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9312,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9323,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9334,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9345,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9356,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9367,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9378,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  9387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
};

U_sTEMPAGCOFFSET AGC_PATHLOSS_BAND10[CAL_TEMP_SECTION] = 
{
   /*temp1*/
   {{/* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  3113,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3133,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3153,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3173,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3193,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3213,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3233,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3250,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3290,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3310,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3330,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3350,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3370,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp2*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  3113,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3133,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3153,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3173,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3193,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3213,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3233,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3250,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3290,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3310,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3330,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3350,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3370,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp3*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  3113,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3133,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3153,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3173,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3193,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3213,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3233,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3250,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3290,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3310,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3330,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3350,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3370,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp4*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  3113,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3133,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3153,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3173,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3193,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3213,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3233,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3250,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3290,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3310,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3330,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3350,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3370,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp5*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  3113,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3133,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3153,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3173,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3193,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3213,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3233,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3250,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3290,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3310,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3330,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3350,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3370,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp6*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  3113,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3133,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3153,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3173,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3193,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3213,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3233,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3250,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3290,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3310,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3330,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3350,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3370,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp7*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  3113,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3133,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3153,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3173,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3193,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3213,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3233,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3250,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3290,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3310,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3330,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3350,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3370,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
   /*temp8*/
   {{  /* uarfcn,    LNA_H_loss,    diff_HM,    diff_HL */
     {  3113,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3133,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3153,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3173,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3193,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3213,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3233,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3250,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3270,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3290,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3310,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3330,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3350,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3370,  U_GAINLOSS( 0.000 ), 0, 0 },
     {  3387,  U_GAINLOSS( 0.000 ), 0, 0 },
   }},
};

U_sTEMPAGCOFFSET*  U_AGC_PATHLOSS_TABLE[] =
{
   &AGC_PATHLOSS_BANDANY[0],
   &AGC_PATHLOSS_BAND1[0],
   &AGC_PATHLOSS_BAND2[0],
   &AGC_PATHLOSS_BAND4[0],
   &AGC_PATHLOSS_BAND5[0],
   &AGC_PATHLOSS_BAND8[0]
};

//temperature dac
kal_uint16 U_TEMP_DAC[CAL_TEMP_SECTION] =
{
   334, 378, 424, 467, 501, 531, 546, 560
};

/*------------------------------*/
/* RX Diversity Path loss table */
/*------------------------------*/
U_sAGCOFFSETRXD AGC_PATHLOSS_RXD_BAND_DUMMY =
{
   //8 temperature section compensated offset
   {0, 0, 0, 0, 0, 0, 0, 0},

   {//  uarfcn,       LNA_High_loss,     LNA_Middle_loss,       LNA_Low_loss
      {  0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  0,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  UMTS_TABLE_END }// Max. to 15 path loss elements
   }
};

U_sAGCOFFSETRXD AGC_PATHLOSS_RXD_BAND1 = 
{
   //8 temperature section compensated offset
   {0, 0, 0, 0, 0, 0, 0, 0},

   {//  uarfcn,       LNA_High_loss,     LNA_Middle_loss,       LNA_Low_loss
      {  10562,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  10631,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  10700,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  10769,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  10838,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  UMTS_TABLE_END }// Max. to 15 path loss elements
   }
};

U_sAGCOFFSETRXD AGC_PATHLOSS_RXD_BAND2 = 
{
   //8 temperature section compensated offset
   {0, 0, 0, 0, 0, 0, 0, 0},

   {//  uarfcn,       LNA_High_loss,     LNA_Middle_loss,       LNA_Low_loss
      {   9662,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   9700,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   9800,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   9869,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   9938,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  UMTS_TABLE_END }// Max. to 15 path loss elements
   }
};

U_sAGCOFFSETRXD AGC_PATHLOSS_RXD_BAND4 = 
{
   //8 temperature section compensated offset
   {0, 0, 0, 0, 0, 0, 0, 0},

   {//  uarfcn,       LNA_High_loss,     LNA_Middle_loss,       LNA_Low_loss
      {   1537,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1552,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1567,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1582,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1597,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1612,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1627,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1642,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1657,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1675,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1687,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1702,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1717,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   1732,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
   }
};

U_sAGCOFFSETRXD AGC_PATHLOSS_RXD_BAND5 = 
{
   //8 temperature section compensated offset
   {0, 0, 0, 0, 0, 0, 0, 0},

   {//  uarfcn,       LNA_High_loss,     LNA_Middle_loss,       LNA_Low_loss
      {   4357,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   4382,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   4408,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   4433,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   4458,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  UMTS_TABLE_END }// Max. to 15 path loss elements
   }
};

U_sAGCOFFSETRXD AGC_PATHLOSS_RXD_BAND8 = 
{
   //8 temperature section compensated offset
   {0, 0, 0, 0, 0, 0, 0, 0},

   {//  uarfcn,       LNA_High_loss,     LNA_Middle_loss,       LNA_Low_loss
      {   2937,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   2975,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   3013,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   3051,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {   3088,  U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ), U_GAINLOSS( 0.000 ) },
      {  UMTS_TABLE_END }// Max. to 15 path loss elements
   }
};

U_sAGCOFFSETRXD*  U_AGC_PATHLOSS_RXD_TABLE[] =
{
   &AGC_PATHLOSS_RXD_BAND_DUMMY,
   &AGC_PATHLOSS_RXD_BAND1,
   &AGC_PATHLOSS_RXD_BAND2,
   &AGC_PATHLOSS_RXD_BAND4,
   &AGC_PATHLOSS_RXD_BAND5,
   &AGC_PATHLOSS_RXD_BAND8
};

/* BAND ANY...................................................................*/
U_sRAMPDATA  BANDANY_RampData =
{
   /* DC2DC data by BAND */
   { 2, 
     31 
   },
   {
      /* PA high mode */
      {  
         /* PA DAC info */
         { 1, 660, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  296,  198 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {    0x0000    ,     5      ,     0     },
            {    0x0000    ,     1      ,     0     },
            {    0x0000    ,    -2      ,     0     },
            {    0x0000    ,    -2      ,     0     },
            {    0x0000    ,    -1      ,     0     },
            {    0x0000    ,     0      ,     0     },
            {    0x0000    ,    -2      ,     0     },
            {    0x0000    ,     0      ,     0     },
            {    0x0000    ,     3      ,     0     },
            {    0x0000    ,     7      ,     0     },
            {    0x0000    ,    12      ,     0     },
            {    0x0000    ,    20      ,     0     },
            {    0x0000    ,    25      ,     0     },
            {    0x0000    ,    33      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA mid mode */
      {  
         /* PA DAC info */
         { 0, 550, 1, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  238,   198 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {    0x0000    ,     5      ,     0     },
            {    0x0000    ,     1      ,     0     },
            {    0x0000    ,    -2      ,     0     },
            {    0x0000    ,    -2      ,     0     },
            {    0x0000    ,    -1      ,     0     },
            {    0x0000    ,     0      ,     0     },
            {    0x0000    ,    -2      ,     0     },
            {    0x0000    ,     0      ,     0     },
            {    0x0000    ,     3      ,     0     },
            {    0x0000    ,     6      ,     0     },
            {    0x0000    ,    11      ,     0     },
            {    0x0000    ,    19      ,     0     },
            {    0x0000    ,    25      ,     0     },
            {    0x0000    ,    32      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA low DAC */
      {  
         /* PA DAC info */
         { 0, 490, 1, 1},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  161,   198 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {    0x0000    ,     2      ,     0     },
            {    0x0000    ,    -1      ,     0     },
            {    0x0000    ,    -3      ,     0     },
            {    0x0000    ,    -4      ,     0     },
            {    0x0000    ,    -3      ,     0     },
            {    0x0000    ,     0      ,     0     },
            {    0x0000    ,    -3      ,     0     },
            {    0x0000    ,     0      ,     0     },
            {    0x0000    ,     3      ,     0     },
            {    0x0000    ,     6      ,     0     },
            {    0x0000    ,    11      ,     0     },
            {    0x0000    ,    19      ,     0     },
            {    0x0000    ,    25      ,     0     },
            {    0x0000    ,    33      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      }     
   },
   
   /* hysteresis start and end idx */
   {
     { 10, 14 },   
     { 20, 24 },
   },
   /*-------------------------------------------------------------------------------------------*/
   /* VGA comp by power detector */
   { 
     /* pwr dt start threshold: dBm */
     16, 
     /* pwr dt used section */ 
     32,
     /* pwr dt dac */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
     /* pwr dt value */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },  
     /* pwr dt compensate by subband */
     {
        /* max arfcn , pwr_offset_db */
        {    0x0000    ,    10      ,     0     },
        {    0x0000    ,    14      ,     0     },
        {    0x0000    ,    17      ,     0     },
        {    0x0000    ,    13      ,     0     },
        {    0x0000    ,    11      ,     0     },
        {    0x0000    ,     8      ,     0     },
        {    0x0000    ,     6      ,     0     },
        {    0x0000    ,     0      ,     0     },
        {    0x0000    ,    -2      ,     0     },
        {    0x0000    ,    -3      ,     0     },
        {    0x0000    ,    -7      ,     0     },
        {    0x0000    ,    -14     ,     0     },
        {    0x0000    ,    -18     ,     0     },
        {    0x0000    ,    -15     ,     0     },
        { UMTS_TABLE_END }
     },
     /* pwr dt compensate by temperature */
     { { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
     }  
   },
   
};

/* BAND1...................................................................*/
U_sRAMPDATA  BAND1_RampData = 
{
   /* DC2DC data by BAND */
   { 2, 
     31 
   },
   {
      /* PA high mode */
      {  
         /* PA DAC info */
         { 1, 660, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  296,  317 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {    9612    ,     -1      ,     0     },
            {    9634    ,     -6      ,     0     },
            {    9656    ,    -4      ,     0     },
            {    9678    ,    -5      ,     0     },
            {    9700    ,    -1      ,     0     },
            {    9722    ,     0      ,     0     },
            {    9750    ,     0     ,     0     },
            {    9766    ,     2      ,     0     },
            {    9788    ,     0      ,     0     },
            {    9810    ,     -1      ,     0     },
            {    9832    ,    -3      ,     0     },
            {    9854    ,    -3      ,     0     },
            {    9876    ,    -4      ,     0     },
            {    9888    ,    -3      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA mid mode */
      {  
         /* PA DAC info */
         { 0, 550, 1, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  238,   322 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {    9612    ,     1      ,     0     },
            {    9634    ,     -4      ,     0     },
            {    9656    ,    -5      ,     0     },
            {    9678    ,    -6      ,     0     },
            {    9700    ,    -3      ,     0     },
            {    9722    ,     -3      ,     0     },
            {    9750    ,     0     ,     0     },
            {    9766    ,     -3      ,     0     },
            {    9788    ,     -8      ,     0     },
            {    9810    ,     -14      ,     0     },
            {    9832    ,    -21      ,     0     },
            {    9854    ,    -25      ,     0     },
            {    9876    ,    -30      ,     0     },
            {    9888    ,    -31      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 4, 0 },
           { 4, 0 },
           { 4, 0 },
           { 4, 0 },
           { 4, 0 },
           { 4, 0 },
           { 4, 0 },
           { 4, 0 },
         }
      },
      
      /* PA low DAC */
      {  
         /* PA DAC info */
         { 0, 490, 1, 1},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  161,   324 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {    9612    ,     3      ,     0     },
            {    9634    ,    -2      ,     0     },
            {    9656    ,    -2      ,     0     },
            {    9678    ,    -4      ,     0     },
            {    9700    ,     0      ,     0     },
            {    9722    ,     0      ,     0     },
            {    9750    ,     0      ,     0     },
            {    9766    ,     -1      ,     0     },
            {    9788    ,     -7      ,     0     },
            {    9810    ,     -12      ,     0     },
            {    9832    ,    -19      ,     0     },
            {    9854    ,    -24      ,     0     },
            {    9876    ,    -29      ,     0     },
            {    9888    ,    -30      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 6, 0 },
           { 6, 0 },
           { 6, 0 },
           { 6, 0 },
           { 6, 0 },
           { 6, 0 },
           { 6, 0 },
           { 6, 0 },
         }
      }     
   },
   
   /* hysteresis start and end idx */
   {
     { 12, 14 },   
     { 22, 24 },
   },
   /*-------------------------------------------------------------------------------------------*/
   /* VGA comp by power detector */
   { 
     /* pwr dt start threshold: dBm */
     16, 
     /* pwr dt used section */ 
     32,
     /* pwr dt dac */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
     /* pwr dt value */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },  
     /* pwr dt compensate by subband */
     {
        /* max arfcn , pwr_offset_db */
        {    9612    ,    2      ,     0     },
        {    9634    ,    3      ,     0     },
        {    9656    ,    6      ,     0     },
        {    9678    ,    8      ,     0     },
        {    9700    ,    8      ,     0     },
        {    9722    ,    6      ,     0     },
        {    9750    ,    0      ,     0     },
        {    9766    ,    4      ,     0     },
        {    9788    ,    3      ,     0     },
        {    9810    ,    4      ,     0     },
        {    9832    ,    0      ,     0     },
        {    9854    ,    -2     ,     0     },
        {    9876    ,    -5     ,     0     },
        {    9888    ,    -5     ,     0     },
        { UMTS_TABLE_END }
     },
     /* pwr dt compensate by temperature */
     { { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
     }  
   },
   
};

/* BAND2...................................................................*/
U_sRAMPDATA  BAND2_RampData = 
{
   /* DC2DC data by BAND */
   { 2, 
     31 
   },
   {
      /* PA high mode */
      {  
         /* PA DAC info */
         { 1, 660, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  296,  294 },
              
         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {      12    ,    -5      ,     0     },
            {     162    ,    -1      ,     0     },
            {     287    ,    35      ,     0     },
            {    9262    ,    -4      ,     0     },
            {    9290    ,   -13      ,     0     },
            {    9318    ,   -12      ,     0     },
            {    9346    ,    -5      ,     0     },
            {    9374    ,     0      ,     0     },
            {    9400    ,     0      ,     0     },
            {    9428    ,    -3      ,     0     },
            {    9456    ,    -5      ,     0     },
            {    9484    ,     1      ,     0     },
            {    9512    ,    22      ,     0     },
            {    9538    ,    36      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA mid mode */
      {  
         /* PA DAC info */
         { 0, 550, 1, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  238,   291 },
 
         /* VGA offset by subband */
         {
            /* max arfcn ,   pwr_offset_db */
            {      12    ,   -16      ,     0     },
            {     162    ,    -5      ,     0     },
            {     287    ,    26      ,     0     },
            {    9262    ,   -17      ,     0     },
            {    9290    ,   -27      ,     0     },
            {    9318    ,   -23      ,     0     },
            {    9346    ,   -11      ,     0     },
            {    9374    ,    -2      ,     0     },
            {    9400    ,     0      ,     0     },
            {    9428    ,   -12      ,     0     },
            {    9456    ,   -20      ,     0     },
            {    9484    ,   -13      ,     0     },
            {    9512    ,    11      ,     0     },
            {    9538    ,    25      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA low DAC */
      {  
         /* PA DAC info */
         { 0, 490, 1, 1},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  161,   292 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {      12    ,   -17      ,     0     },
            {     162    ,    -3      ,     0     },
            {     287    ,    28      ,     0     },
            {    9262    ,   -17      ,     0     },
            {    9290    ,   -26      ,     0     },
            {    9318    ,   -21      ,     0     },
            {    9346    ,    -8      ,     0     },
            {    9374    ,     0      ,     0     },
            {    9400    ,     0      ,     0     },
            {    9428    ,   -10      ,     0     },
            {    9456    ,   -19      ,     0     },
            {    9484    ,   -11      ,     0     },
            {    9512    ,    13      ,     0     },
            {    9538    ,    27      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      }     
   },
   /*-------------------------------------------------------------------------------------------*/
   /* hysteresis start and end idx */
   {
     { 16, 18 },   
     { 22, 24 },
   },
   /*-------------------------------------------------------------------------------------------*/
   /* VGA comp by power detector */
   { 
     /* pwr dt start threshold: dBm */
     16, 
     /* pwr dt used section */ 
     32,
     /* pwr dt dac */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
     /* pwr dt value */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },  
     /* pwr dt compensate by subband */
     {
        /* max arfcn ,   pwr_offset_db */
        {      12    ,    -15     ,     0     },
        {     162    ,      6     ,     0     },
        {     287    ,    -28     ,     0     },
        {    9262    ,    -14     ,     0     },
        {    9290    ,     -6     ,     0     },
        {    9318    ,     -3     ,     0     },
        {    9346    ,     -4     ,     0     },
        {    9374    ,      2     ,     0     },
        {    9400    ,      0     ,     0     },
        {    9428    ,      6     ,     0     },
        {    9456    ,      0     ,     0     },
        {    9484    ,    -11     ,     0     },
        {    9512    ,    -23     ,     0     },
        {    9538    ,    -31     ,     0     },
        { UMTS_TABLE_END }
     },
     /* pwr dt compensate by temperature */
     { { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
     }  
   },
   
};

/* BAND3...................................................................*/
U_sRAMPDATA  BAND3_RampData = 
{
   /* DC2DC data by BAND */
   { 7, 
     31 
   },
   {
      /* PA high mode */
      {  
         /* PA DAC info */
         { 1, 660, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  225,   176 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    937     ,     0      ,     0     },
            {    962     ,     0      ,     0     },
            {    987     ,     0      ,     0     },
            {    1012    ,     0      ,     0     },
            {    1037    ,     0      ,     0     },
            {    1062    ,     0      ,     0     },
            {    1087    ,     0      ,     0     },
            {    1113    ,     0      ,     0     },
            {    1138    ,     0      ,     0     },
            {    1163    ,     0      ,     0     },
            {    1188    ,     0      ,     0     },
            {    1213    ,     0      ,     0     },
            {    1238    ,     0      ,     0     },
            {    1263    ,     0      ,     0     },
            {    1288    ,     0      ,     0     },
         },      
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA mid mode */
      {  
         /* PA DAC info */
         { 0, 580, 0, 0},
         /* VGA DAC of PA mid  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  191,   172 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    937     ,     0      ,     0     },
            {    962     ,     0      ,     0     },
            {    987     ,     0      ,     0     },
            {    1012    ,     0      ,     0     },
            {    1037    ,     0      ,     0     },
            {    1062    ,     0      ,     0     },
            {    1087    ,     0      ,     0     },
            {    1113    ,     0      ,     0     },
            {    1138    ,     0      ,     0     },
            {    1163    ,     0      ,     0     },
            {    1188    ,     0      ,     0     },
            {    1213    ,     0      ,     0     },
            {    1238    ,     0      ,     0     },
            {    1263    ,     0      ,     0     },
            {    1288    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA low DAC */
      {  
         /* PA DAC info */
         { 0, 520, 0, 0},
         /* VGA DAC of PA low  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  144,   173 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    937     ,     0      ,     0     },
            {    962     ,     0      ,     0     },
            {    987     ,     0      ,     0     },
            {    1012    ,     0      ,     0     },
            {    1037    ,     0      ,     0     },
            {    1062    ,     0      ,     0     },
            {    1087    ,     0      ,     0     },
            {    1113    ,     0      ,     0     },
            {    1138    ,     0      ,     0     },
            {    1163    ,     0      ,     0     },
            {    1188    ,     0      ,     0     },
            {    1213    ,     0      ,     0     },
            {    1238    ,     0      ,     0     },
            {    1263    ,     0      ,     0     },
            {    1288    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      }     
   },
   
   /* hysteresis start and end idx */
   {
     { 10, 14 },   
     { 20, 24 },
   },
   /*-------------------------------------------------------------------------------------------*/
   /* VGA comp by power detector */
   { 
     /* pwr dt start threshold: dBm */
     16, 
     /* pwr dt used section */ 
     32,
     /* pwr dt dac */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
     /* pwr dt value */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },  
     /* pwr dt compensate by subband */
     {
        /* max arfcn , pwr_slope  , pwr_offset*/
        {    937     ,     0      ,     0     },
        {    962     ,     0      ,     0     },
        {    987     ,     0      ,     0     },
        {    1012    ,     0      ,     0     },
        {    1037    ,     0      ,     0     },
        {    1062    ,     0      ,     0     },
        {    1087    ,     0      ,     0     },
        {    1113    ,     0      ,     0     },
        {    1138    ,     0      ,     0     },
        {    1163    ,     0      ,     0     },
        {    1188    ,     0      ,     0     },
        {    1213    ,     0      ,     0     },
        {    1238    ,     0      ,     0     },
        {    1263    ,     0      ,     0     },
        {    1288    ,     0      ,     0     },
     },
     /* pwr dt compensate by temperature */
     { { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
     }  
   },
   
};

/* BAND4...................................................................*/
U_sRAMPDATA  BAND4_RampData = 
{
   /* DC2DC data by BAND */
   { 7, 
     31 
   },
   {
      /* PA high mode */
      {  
         /* PA DAC info */
         { 1, 660, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  246,   176 },
              
         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    1312    ,     0      ,     0     },
            {    1327    ,     0      ,     0     },
            {    1342    ,     0      ,     0     },
            {    1357    ,     0      ,     0     },
            {    1372    ,     0      ,     0     },
            {    1387    ,     0      ,     0     },
            {    1402    ,     0      ,     0     },
            {    1417    ,     0      ,     0     },
            {    1432    ,     0      ,     0     },
            {    1450    ,     0      ,     0     },
            {    1465    ,     0      ,     0     },
            {    1480    ,     0      ,     0     },
            {    1495    ,     0      ,     0     },
            {    1510    ,     0      ,     0     },
            {    1513    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA mid mode */
      {  
         /* PA DAC info */
         { 0, 580, 0, 0},
         /* VGA DAC of PA mid  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  212,   172 },
 
         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    1312    ,     0      ,     0     },
            {    1327    ,     0      ,     0     },
            {    1342    ,     0      ,     0     },
            {    1357    ,     0      ,     0     },
            {    1372    ,     0      ,     0     },
            {    1387    ,     0      ,     0     },
            {    1402    ,     0      ,     0     },
            {    1417    ,     0      ,     0     },
            {    1432    ,     0      ,     0     },
            {    1450    ,     0      ,     0     },
            {    1465    ,     0      ,     0     },
            {    1480    ,     0      ,     0     },
            {    1495    ,     0      ,     0     },
            {    1510    ,     0      ,     0     },
            {    1513    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA low DAC */
      {  
         /* PA DAC info */
         { 0, 520, 0, 0},
         /* VGA DAC of PA low  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  166,   173 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    1312    ,     0      ,     0     },
            {    1327    ,     0      ,     0     },
            {    1342    ,     0      ,     0     },
            {    1357    ,     0      ,     0     },
            {    1372    ,     0      ,     0     },
            {    1387    ,     0      ,     0     },
            {    1402    ,     0      ,     0     },
            {    1417    ,     0      ,     0     },
            {    1432    ,     0      ,     0     },
            {    1450    ,     0      ,     0     },
            {    1465    ,     0      ,     0     },
            {    1480    ,     0      ,     0     },
            {    1495    ,     0      ,     0     },
            {    1510    ,     0      ,     0     },
            {    1513    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      }     
   },
   /*-------------------------------------------------------------------------------------------*/
   /* hysteresis start and end idx */
   {
     { 10, 14 },   
     { 20, 24 },
   },
   /*-------------------------------------------------------------------------------------------*/
   /* VGA comp by power detector */
   { 
     /* pwr dt start threshold: dBm */
     16, 
     /* pwr dt used section */ 
     32,
     /* pwr dt dac */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
     /* pwr dt value */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },  
     /* pwr dt compensate by subband */
     {
        /* max arfcn , pwr_slope  , pwr_offset*/
        {    1312    ,     0      ,     0     },
        {    1327    ,     0      ,     0     },
        {    1342    ,     0      ,     0     },
        {    1357    ,     0      ,     0     },
        {    1372    ,     0      ,     0     },
        {    1387    ,     0      ,     0     },
        {    1402    ,     0      ,     0     },
        {    1417    ,     0      ,     0     },
        {    1432    ,     0      ,     0     },
        {    1450    ,     0      ,     0     },
        {    1465    ,     0      ,     0     },
        {    1480    ,     0      ,     0     },
        {    1495    ,     0      ,     0     },
        {    1510    ,     0      ,     0     },
        {    1513    ,     0      ,     0     },
     },
     /* pwr dt compensate by temperature */
     { { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
     }  
   },
   
};

/* BAND5...................................................................*/
U_sRAMPDATA  BAND5_RampData = 
{
   /* DC2DC data by BAND */
   { 2, 
     31 
   },
   {
      /* PA high mode */
      {  
         /* PA DAC info */
         { 1, 660, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  296,  272 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {    782    ,     -12      ,     0     },
            {    812    ,       1      ,     0     },
            {    862    ,      -6      ,     0     },
            {    4132    ,    -12      ,     0     },
            {    4142    ,     -9      ,     0     },
            {    4152    ,     -3      ,     0     },
            {    4162    ,      1      ,     0     },
            {    4172    ,      2      ,     0     },
            {    4183    ,      0      ,     0     },
            {    4193    ,     -3      ,     0     },
            {    4203    ,     -6      ,     0     },
            {    4213    ,     -5      ,     0     },
            {    4223    ,      1      ,     0     },
            {    4233    ,     10      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA mid mode */
      {  
         /* PA DAC info */
         { 0, 550, 1, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  238,    274 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {    782    ,      -25      ,     0     },
            {    812    ,       -6      ,     0     },
            {    862    ,      -10      ,     0     },
            {    4132    ,     -26      ,     0     },
            {    4142    ,     -23      ,     0     },
            {    4152    ,     -16      ,     0     },
            {    4162    ,      -7      ,     0     },
            {    4172    ,      -3      ,     0     },
            {    4183    ,       0      ,     0     },
            {    4193    ,      -6      ,     0     },
            {    4203    ,     -10      ,     0     },
            {    4213    ,     -11      ,     0     },
            {    4223    ,      -7      ,     0     },
            {    4233    ,       0      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA low DAC */
      {  
         /* PA DAC info */
         { 0, 490, 1, 1},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  161,    273 },
         
         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {    782    ,      -23      ,     0     },
            {    812    ,       -4      ,     0     },
            {    862    ,       -9      ,     0     },
            {    4132    ,     -23      ,     0     },
            {    4142    ,     -21      ,     0     },
            {    4152    ,     -14      ,     0     },
            {    4162    ,      -5      ,     0     },
            {    4172    ,       0      ,     0     },
            {    4183    ,       0      ,     0     },
            {    4193    ,      -4      ,     0     },
            {    4203    ,      -8      ,     0     },
            {    4213    ,      -9      ,     0     },
            {    4223    ,      -5      ,     0     },
            {    4233    ,       2      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { -3, 0 },
           { -3, 0 },
           { -3, 0 },
           { -3, 0 },
           { -3, 0 },
           { -3, 0 },
           { -3, 0 },
           { -3, 0 },
         }
      }     
   },
   /*-------------------------------------------------------------------------------------------*/
   /* hysteresis start and end idx */
   {
     { 12, 14 },   
     { 22, 24 },
   },
   /*-------------------------------------------------------------------------------------------*/
   /* VGA comp by power detector */
   { 
     /* pwr dt start threshold: dBm */
     16, 
     /* pwr dt used section */ 
      0,
     /* pwr dt dac */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
     /* pwr dt value */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },  
     /* pwr dt compensate by subband */
     {
        /* max arfcn , pwr_offset_db */
            {    782    ,     -22      ,     0     },
            {    812    ,       1      ,     0     },
            {    862    ,     -12      ,     0     },
            {    4132    ,    -23      ,     0     },
            {    4142    ,    -14      ,     0     },
            {    4152    ,     -5      ,     0     },
            {    4162    ,      1      ,     0     },
            {    4172    ,      5      ,     0     },
            {    4183    ,      0      ,     0     },
            {    4193    ,     -5      ,     0     },
            {    4203    ,     -8      ,     0     },
            {    4213    ,    -15      ,     0     },
            {    4223    ,    -16      ,     0     },
            {    4233    ,    -14      ,     0     },
        { UMTS_TABLE_END }
     },
     /* pwr dt compensate by temperature */
     { { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
     }  
   },
   
};

/* BAND6...................................................................*/
/* Band6 is overlap with Band5 */
U_sRAMPDATA  BAND6_RampData = 
{
   /* DC2DC data by BAND */
   { 0, 
     0 
   },
   {
      /* PA high mode */
      {  
         /* PA DAC info */
         { 0, 0, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset_db */
            {    4162    ,     0      ,     0     },
            {    4168    ,     0      ,     0     },
            {    4175    ,     0      ,     0     },
            {    4182    ,     0      ,     0     },
            {    4188    ,     0      ,     0     },
            { UMTS_TABLE_END }
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA mid mode */
      {  
         /* PA DAC info */
         { 0, 0, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            /* max arfcn , pwr_offset */
            {    4162    ,     0      ,     0     },
            {    4168    ,     0      ,     0     },
            {    4175    ,     0      ,     0     },
            {    4182    ,     0      ,     0     },
            {    4188    ,     0      ,     0     },
            { UMTS_TABLE_END }

         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA low DAC */
      {  
         /* PA DAC info */
         { 0, 0, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
         
         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            /* max arfcn , pwr_offset */
            {    4162    ,     0      ,     0     },
            {    4168    ,     0      ,     0     },
            {    4175    ,     0      ,     0     },
            {    4182    ,     0      ,     0     },
            {    4188    ,     0      ,     0     },
            { UMTS_TABLE_END }

         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      }     
   },
   /*-------------------------------------------------------------------------------------------*/
   /* hysteresis start and end idx */
   {
     { 0, 0 },   
     { 0, 0 },
   },
   /*-------------------------------------------------------------------------------------------*/
   /* VGA comp by power detector */
   { 
     /* pwr dt start threshold: dBm */
     0, 
     /* pwr dt used section */ 
     0,
     /* pwr dt dac */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
     /* pwr dt value */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },  
     /* pwr dt compensate by subband */
     {
        /* max arfcn , pwr_slope  , pwr_offset*/
        {    4132    ,     0      ,     0     },
        {    4140    ,     0      ,     0     },
        {    4148    ,     0      ,     0     },
        {    4156    ,     0      ,     0     },
        {    4164    ,     0      ,     0     },
        {    4172    ,     0      ,     0     },
        {    4180    ,     0      ,     0     },
        {    4188    ,     0      ,     0     },
        {    4196    ,     0      ,     0     },
        {    4204    ,     0      ,     0     },
        {    4212    ,     0      ,     0     },
        {    4220    ,     0      ,     0     },
        {    4228    ,     0      ,     0     },
        {    4233    ,     0      ,     0     },
        { UMTS_TABLE_END }
     },
     /* pwr dt compensate by temperature */
     { { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
     }  
   },
   
};

/* BAND8...................................................................*/
U_sRAMPDATA  BAND8_RampData = 
{
   /* DC2DC data by BAND */
   { 7, 
     31 
   },
   {
      /* PA high mode */
      {  
         /* PA DAC info */
         { 1, 660, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  292,   162 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    2712    ,     0      ,     0     },
            {    2723    ,     0      ,     0     },
            {    2734    ,     0      ,     0     },
            {    2745    ,     0      ,     0     },
            {    2756    ,     0      ,     0     },
            {    2767    ,     0      ,     0     },
            {    2778    ,     0      ,     0     },
            {    2788    ,     0      ,     0     },
            {    2799    ,     0      ,     0     },
            {    2810    ,     0      ,     0     },
            {    2821    ,     0      ,     0     },
            {    2832    ,     0      ,     0     },
            {    2843    ,     0      ,     0     },
            {    2854    ,     0      ,     0     },
            {    2863    ,     0      ,     0     },
         },      
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA mid mode */
      {  
         /* PA DAC info */
         { 0, 550, 1, 0},
         /* VGA DAC of PA mid  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  238,   162 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    2712    ,     0      ,     0     },
            {    2723    ,     0      ,     0     },
            {    2734    ,     0      ,     0     },
            {    2745    ,     0      ,     0     },
            {    2756    ,     0      ,     0     },
            {    2767    ,     0      ,     0     },
            {    2778    ,     0      ,     0     },
            {    2788    ,     0      ,     0     },
            {    2799    ,     0      ,     0     },
            {    2810    ,     0      ,     0     },
            {    2821    ,     0      ,     0     },
            {    2832    ,     0      ,     0     },
            {    2843    ,     0      ,     0     },
            {    2854    ,     0      ,     0     },
            {    2863    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA low DAC */
      {  
         /* PA DAC info */
         { 0, 490, 1, 1},
         /* VGA DAC of PA low  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  161,   162 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    2712    ,     0      ,     0     },
            {    2723    ,     0      ,     0     },
            {    2734    ,     0      ,     0     },
            {    2745    ,     0      ,     0     },
            {    2756    ,     0      ,     0     },
            {    2767    ,     0      ,     0     },
            {    2778    ,     0      ,     0     },
            {    2788    ,     0      ,     0     },
            {    2799    ,     0      ,     0     },
            {    2810    ,     0      ,     0     },
            {    2821    ,     0      ,     0     },
            {    2832    ,     0      ,     0     },
            {    2843    ,     0      ,     0     },
            {    2854    ,     0      ,     0     },
            {    2863    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      }     
   },
   
   /* hysteresis start and end idx */
   {
     { 10, 14 },   
     { 20, 24 },
   },
   /*-------------------------------------------------------------------------------------------*/
   /* VGA comp by power detector */
   { 
     /* pwr dt start threshold: dBm */
     16, 
     /* pwr dt used section */ 
     32,
     /* pwr dt dac */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
     /* pwr dt value */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },  
     /* pwr dt compensate by subband */
     {
        /* max arfcn , pwr_offset */
        {    2712    ,     0      ,     0     },
        {    2723    ,     0      ,     0     },
        {    2734    ,     0      ,     0     },
        {    2745    ,     0      ,     0     },
        {    2756    ,     0      ,     0     },
        {    2767    ,     0      ,     0     },
        {    2778    ,     0      ,     0     },
        {    2788    ,     0      ,     0     },
        {    2799    ,     0      ,     0     },
        {    2810    ,     0      ,     0     },
        {    2821    ,     0      ,     0     },
        {    2832    ,     0      ,     0     },
        {    2843    ,     0      ,     0     },
        {    2854    ,     0      ,     0     },
        {    2863    ,     0      ,     0     },
     },
     /* pwr dt compensate by temperature */
     { { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
     }  
   },
   
};

/* BAND9...................................................................*/
U_sRAMPDATA  BAND9_RampData = 
{
   /* DC2DC data by BAND */
   { 7, 
     31 
   },
   {
      /* PA high mode */
      {  
         /* PA DAC info */
         { 1, 660, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  246,   176 },
              
         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    8762    ,     0      ,     0     },
            {    8773    ,     0      ,     0     },
            {    8784    ,     0      ,     0     },
            {    8795    ,     0      ,     0     },
            {    8806    ,     0      ,     0     },
            {    8817    ,     0      ,     0     },
            {    8828    ,     0      ,     0     },
            {    8837    ,     0      ,     0     },
            {    8848    ,     0      ,     0     },
            {    8859    ,     0      ,     0     },
            {    8870    ,     0      ,     0     },
            {    8881    ,     0      ,     0     },
            {    8892    ,     0      ,     0     },
            {    8903    ,     0      ,     0     },
            {    8912    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA mid mode */
      {  
         /* PA DAC info */
         { 0, 580, 0, 0},
         /* VGA DAC of PA mid  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  212,   172 },
 
         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    8762    ,     0      ,     0     },
            {    8773    ,     0      ,     0     },
            {    8784    ,     0      ,     0     },
            {    8795    ,     0      ,     0     },
            {    8806    ,     0      ,     0     },
            {    8817    ,     0      ,     0     },
            {    8828    ,     0      ,     0     },
            {    8837    ,     0      ,     0     },
            {    8848    ,     0      ,     0     },
            {    8859    ,     0      ,     0     },
            {    8870    ,     0      ,     0     },
            {    8881    ,     0      ,     0     },
            {    8892    ,     0      ,     0     },
            {    8903    ,     0      ,     0     },
            {    8912    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA low DAC */
      {  
         /* PA DAC info */
         { 0, 520, 0, 0},
         /* VGA DAC of PA low  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  166,   173 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    8762    ,     0      ,     0     },
            {    8773    ,     0      ,     0     },
            {    8784    ,     0      ,     0     },
            {    8795    ,     0      ,     0     },
            {    8806    ,     0      ,     0     },
            {    8817    ,     0      ,     0     },
            {    8828    ,     0      ,     0     },
            {    8837    ,     0      ,     0     },
            {    8848    ,     0      ,     0     },
            {    8859    ,     0      ,     0     },
            {    8870    ,     0      ,     0     },
            {    8881    ,     0      ,     0     },
            {    8892    ,     0      ,     0     },
            {    8903    ,     0      ,     0     },
            {    8912    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      }     
   },
   /*-------------------------------------------------------------------------------------------*/
   /* hysteresis start and end idx */
   {
     { 10, 14 },   
     { 20, 24 },
   },
   /*-------------------------------------------------------------------------------------------*/
   /* VGA comp by power detector */
   { 
     /* pwr dt start threshold: dBm */
     16, 
     /* pwr dt used section */ 
     32,
     /* pwr dt dac */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
     /* pwr dt value */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },  
     /* pwr dt compensate by subband */
     {
        /* max arfcn , pwr_offset */
        {    8762    ,     0      ,     0     },
        {    8773    ,     0      ,     0     },
        {    8784    ,     0      ,     0     },
        {    8795    ,     0      ,     0     },
        {    8806    ,     0      ,     0     },
        {    8817    ,     0      ,     0     },
        {    8828    ,     0      ,     0     },
        {    8837    ,     0      ,     0     },
        {    8848    ,     0      ,     0     },
        {    8859    ,     0      ,     0     },
        {    8870    ,     0      ,     0     },
        {    8881    ,     0      ,     0     },
        {    8892    ,     0      ,     0     },
        {    8903    ,     0      ,     0     },
        {    8912    ,     0      ,     0     },
     },
     /* pwr dt compensate by temperature */
     { { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
     }  
   },
   
};

/* BAND10...................................................................*/
U_sRAMPDATA  BAND10_RampData = 
{
   /* DC2DC data by BAND */
   { 7, 
     31 
   },
   {
      /* PA high mode */
      {  
         /* PA DAC info */
         { 1, 660, 0, 0},
         /* VGA DAC of PA high  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  276,   176 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    2888    ,     0      ,     0     },
            {    2908    ,     0      ,     0     },
            {    2928    ,     0      ,     0     },
            {    2948    ,     0      ,     0     },
            {    2968    ,     0      ,     0     },
            {    2988    ,     0      ,     0     },
            {    3008    ,     0      ,     0     },
            {    3025    ,     0      ,     0     },
            {    3045    ,     0      ,     0     },
            {    3065    ,     0      ,     0     },
            {    3085    ,     0      ,     0     },
            {    3105    ,     0      ,     0     },
            {    3125    ,     0      ,     0     },
            {    3145    ,     0      ,     0     },
            {    3162    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA mid mode */
      {  
         /* PA DAC info */
         { 0, 580, 0, 0},
         /* VGA DAC of PA mid  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  242,   172 },

         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    2888    ,     0      ,     0     },
            {    2908    ,     0      ,     0     },
            {    2928    ,     0      ,     0     },
            {    2948    ,     0      ,     0     },
            {    2968    ,     0      ,     0     },
            {    2988    ,     0      ,     0     },
            {    3008    ,     0      ,     0     },
            {    3025    ,     0      ,     0     },
            {    3045    ,     0      ,     0     },
            {    3065    ,     0      ,     0     },
            {    3085    ,     0      ,     0     },
            {    3105    ,     0      ,     0     },
            {    3125    ,     0      ,     0     },
            {    3145    ,     0      ,     0     },
            {    3162    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      },
      
      /* PA low DAC */
      {  
         /* PA DAC info */
         { 0, 520, 0, 0},
         /* VGA DAC of PA low  */
         /* 32,   32,   31,   30,   29,   28,   27,   26,   25,   24 */
         {   0,    0,    0,    0,    0,    0,    0,    0,  183,   173 },
         
         /* VGA offset by subband */
         {
            /* max arfcn , pwr_offset */
            {    2888    ,     0      ,     0     },
            {    2908    ,     0      ,     0     },
            {    2928    ,     0      ,     0     },
            {    2948    ,     0      ,     0     },
            {    2968    ,     0      ,     0     },
            {    2988    ,     0      ,     0     },
            {    3008    ,     0      ,     0     },
            {    3025    ,     0      ,     0     },
            {    3045    ,     0      ,     0     },
            {    3065    ,     0      ,     0     },
            {    3085    ,     0      ,     0     },
            {    3105    ,     0      ,     0     },
            {    3125    ,     0      ,     0     },
            {    3145    ,     0      ,     0     },
            {    3162    ,     0      ,     0     },
         },
         /* VGA comp by temperature */
         { { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
           { 0, 0 },
         }
      }     
   },
   /*-------------------------------------------------------------------------------------------*/
   /* hysteresis start and end idx */
   {
     { 10, 14 },   
     { 20, 24 },
   },
   /*-------------------------------------------------------------------------------------------*/
   /* VGA comp by power detector */
   { 
     /* pwr dt start threshold: dBm */
     16, 
     /* pwr dt used section */ 
     32,
     /* pwr dt dac */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
     /* pwr dt value */
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },  
     /* pwr dt compensate by subband */
     {
        /* max arfcn , pwr_slope  , pwr_offset*/
        {    2888    ,     0      ,     0     },
        {    2908    ,     0      ,     0     },
        {    2928    ,     0      ,     0     },
        {    2948    ,     0      ,     0     },
        {    2968    ,     0      ,     0     },
        {    2988    ,     0      ,     0     },
        {    3008    ,     0      ,     0     },
        {    3025    ,     0      ,     0     },
        {    3045    ,     0      ,     0     },
        {    3065    ,     0      ,     0     },
        {    3085    ,     0      ,     0     },
        {    3105    ,     0      ,     0     },
        {    3125    ,     0      ,     0     },
        {    3145    ,     0      ,     0     },
        {    3162    ,     0      ,     0     },
     },
     /* pwr dt compensate by temperature */
     { { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
       { 0, 0 },
     }  
   },
   
};

U_sRAMPDATA*  UMTS_RampData[] =
{
   &BANDANY_RampData,
   &BAND1_RampData,
   &BAND2_RampData,
   &BAND4_RampData,
   &BAND5_RampData,  
   &BAND8_RampData,
};

/* PA 8-level control (for MT6276, MT6573) */
U_sPAOCTLVLSETTING  BANDANY_PaOctLevData =
{
   /* Available PA 8-level sections set by user */
     8
   ,
   /* Reserved byte */
     0
   ,
   /* PA phase compensation */
   {
     0x0,
     0x0,
     0x0
   },
   /* PMU OCT level control table */
   {
      {
          2, // PA Mode
          0, // Prf
          4, // DC2DC level
          1, // Vm0
          1, // Vm1
        490, // Vbias
        190 // PA gain
      },
      {
          1, // PA Mode
          6, // Prf
         14, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        296 // PA gain
      },
      {
          1, // PA Mode
         10, // Prf
         20, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        298 // PA gain
      },
      {
          0, // PA Mode
         15, // Prf
         32, // DC2DC level
          0, // Vm0
          0, // Vm1
        580, // Vbias
        642 // PA gain
      },
      {
          0, // PA Mode
         18, // Prf
         36, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        639 // PA gain
      },
      {
          0, // PA Mode
         20, // Prf
         46, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        648 // PA gain
      },
      {
          0, // PA Mode
         22, // Prf
         50, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        663 // PA gain
      },
      {
          0, // PA Mode
         24, // Prf
         58, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        680 // PA gain
      },
   }
};

/* BAND1...................................................................*/
U_sPAOCTLVLSETTING  BAND1_PaOctLevData =
{
   /* Available PA 8-level sections set by user */
     8
   ,
   /* Reserved byte */
     0
   ,
   /* PA phase compensation */
   {
     0x0,
     0x0,
     0x0
   },
   /* PMU OCT level control table */
   {
      {
          2, // PA Mode
          0, // Prf
          0, // DC2DC level
          1, // Vm0
          1, // Vm1
          0, // Vbias
        686 // PA gain
      },
      {
          1, // PA Mode
          6, // Prf
          0, // DC2DC level
          1, // Vm0
          0, // Vm1
          0, // Vbias
        693 // PA gain
      },
      {
          1, // PA Mode
         11, // Prf
          0, // DC2DC level
          1, // Vm0
          0, // Vm1
          0, // Vbias
        692 // PA gain
      },
      {
          0, // PA Mode
         15, // Prf
          0, // DC2DC level
          0, // Vm0
          0, // Vm1
          0, // Vbias
        1067 // PA gain
      },
      {
          0, // PA Mode
         18, // Prf
          0, // DC2DC level
          0, // Vm0
          0, // Vm1
          0, // Vbias
        1066 // PA gain
      },
      {
          0, // PA Mode
         20, // Prf
          0, // DC2DC level
          0, // Vm0
          0, // Vm1
          0, // Vbias
        1060 // PA gain
      },
      {
          0, // PA Mode
         22, // Prf
          0, // DC2DC level
          0, // Vm0
          0, // Vm1
          0, // Vbias
        1054 // PA gain
      },
      {
          0, // PA Mode
         24, // Prf
          0, // DC2DC level
          0, // Vm0
          0, // Vm1
          0, // Vbias
        1059 // PA gain
      },
   }
};

/* BAND2...................................................................*/
U_sPAOCTLVLSETTING  BAND2_PaOctLevData =
{
   /* Available PA 8-level sections set by user */
     8
   ,
   /* Reserved byte */
     0
   ,
   /* PA phase compensation */
   {
     0x0,
     0x0,
     0x0
   },
   /* PMU OCT level control table */
   {
      {
          2, // PA Mode
          0, // Prf
          4, // DC2DC level
          1, // Vm0
          1, // Vm1
        490, // Vbias
        721 // PA gain
      },
      {
          1, // PA Mode
          6, // Prf
          6, // DC2DC level
          1, // Vm0
          1, // Vm1
        580, // Vbias
        718 // PA gain
      },
      {
          1, // PA Mode
          8, // Prf
         11, // DC2DC level
          1, // Vm0
          1, // Vm1
        580, // Vbias
        715 // PA gain
      },
      {
          0, // PA Mode
         15, // Prf
         20, // DC2DC level
          0, // Vm0
          0, // Vm1
        580, // Vbias
       1125 // PA gain
      },
      {
          0, // PA Mode
         18, // Prf
         28, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
       1126 // PA gain
      },
      {
          0, // PA Mode
         20, // Prf
         34, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
       1127 // PA gain
      },
      {
          0, // PA Mode
         22, // Prf
         42, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
       1127 // PA gain
      },
      {
          0, // PA Mode
         24, // Prf
         58, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
       1130 // PA gain
      },
   }
};

/* BAND3...................................................................*/
U_sPAOCTLVLSETTING  BAND3_PaOctLevData =
{
   /* Available PA 8-level sections set by user */
     8
   ,
   /* Reserved byte */
     0
   ,
   /* PA phase compensation */
   {
     0x0,
     0x0,
     0x0
   },
   /* PMU OCT level control table */
   {
      {
          2, // PA Mode
          0, // Prf
          4, // DC2DC level
          1, // Vm0
          1, // Vm1
        490, // Vbias
        172 // PA gain
      },
      {
          2, // PA Mode
          4, // Prf
         14, // DC2DC level
          1, // Vm0
          1, // Vm1
        580, // Vbias
        227 // PA gain
      },
      {
          1, // PA Mode
          4, // Prf
         20, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        403 // PA gain
      },
      {
          1, // PA Mode
          8, // Prf
         32, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        407 // PA gain
      },
      {
          1, // PA Mode
         10, // Prf
         36, // DC2DC level
          1, // Vm0
          0, // Vm1
        660, // Vbias
        417 // PA gain
      },
      {
          1, // PA Mode
         12, // Prf
         46, // DC2DC level
          1, // Vm0
          0, // Vm1
        660, // Vbias
        418 // PA gain
      },
      {
          0, // PA Mode
         12, // Prf
         50, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        708 // PA gain
      },
      {
          0, // PA Mode
         24, // Prf
         58, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        725 // PA gain
      },
   }
};

/* BAND4...................................................................*/
U_sPAOCTLVLSETTING  BAND4_PaOctLevData =
{
   /* Available PA 8-level sections set by user */
     8
   ,
   /* Reserved byte */
     0
   ,
   /* PA phase compensation */
   {
     0x0,
     0x0,
     0x0
   },
   /* PMU OCT level control table */
   {
      {
          2, // PA Mode
          0, // Prf
          4, // DC2DC level
          1, // Vm0
          1, // Vm1
        490, // Vbias
        172 // PA gain
      },
      {
          2, // PA Mode
          4, // Prf
         14, // DC2DC level
          1, // Vm0
          1, // Vm1
        580, // Vbias
        227 // PA gain
      },
      {
          1, // PA Mode
          4, // Prf
         20, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        403 // PA gain
      },
      {
          1, // PA Mode
          8, // Prf
         32, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        407 // PA gain
      },
      {
          1, // PA Mode
         10, // Prf
         36, // DC2DC level
          1, // Vm0
          0, // Vm1
        660, // Vbias
        417 // PA gain
      },
      {
          1, // PA Mode
         12, // Prf
         46, // DC2DC level
          1, // Vm0
          0, // Vm1
        660, // Vbias
        418 // PA gain
      },
      {
          0, // PA Mode
         12, // Prf
         50, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        708 // PA gain
      },
      {
          0, // PA Mode
         24, // Prf
         58, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        725 // PA gain
      },
   }
};

/* BAND5...................................................................*/
U_sPAOCTLVLSETTING  BAND5_PaOctLevData =
{
   /* Available PA 8-level sections set by user */
     8
   ,
   /* Reserved byte */
     0
   ,
   /* PA phase compensation */
   {
     0x0,
     0x0,
     0x0
   },
   /* PMU OCT level control table */
   {
      {
          2, // PA Mode
          0, // Prf
          2, // DC2DC level
          1, // Vm0
          1, // Vm1
        490, // Vbias
        820 // PA gain
      },
      {
          1, // PA Mode
          6, // Prf
          4, // DC2DC level
          1, // Vm0
          1, // Vm1
        580, // Vbias
        818 // PA gain
      },
      {
          1, // PA Mode
         11, // Prf
         12, // DC2DC level
          1, // Vm0
          1, // Vm1
        580, // Vbias
        824 // PA gain
      },
      {
          0, // PA Mode
         15, // Prf
         20, // DC2DC level
          0, // Vm0
          0, // Vm1
        580, // Vbias
        1135 // PA gain
      },
      {
          0, // PA Mode
         18, // Prf
         28, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        1134 // PA gain
      },
      {
          0, // PA Mode
         20, // Prf
         34, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        1137 // PA gain
      },
      {
          0, // PA Mode
         22, // Prf
         42, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        1143 // PA gain
      },
      {
          0, // PA Mode
         24, // Prf
         58, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        1140 // PA gain
      },
   }
};

/* BAND6...................................................................*/
U_sPAOCTLVLSETTING  BAND6_PaOctLevData =
{
   /* Available PA 8-level sections set by user */
     8
   ,
   /* Reserved byte */
     0
   ,
   /* PA phase compensation */
   {
     0x0,
     0x0,
     0x0
   },
   /* PMU OCT level control table */
   {
      {
          2, // PA Mode
          0, // Prf
          4, // DC2DC level
          1, // Vm0
          1, // Vm1
        490, // Vbias
        296 // PA gain
      },
      {
          1, // PA Mode
          6, // Prf
         14, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        399 // PA gain
      },
      {
          1, // PA Mode
         11, // Prf
         20, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        407 // PA gain
      },
      {
          0, // PA Mode
         15, // Prf
         32, // DC2DC level
          0, // Vm0
          0, // Vm1
        580, // Vbias
        724 // PA gain
      },
      {
          0, // PA Mode
         18, // Prf
         36, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        727 // PA gain
      },
      {
          0, // PA Mode
         20, // Prf
         46, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        742 // PA gain
      },
      {
          0, // PA Mode
         22, // Prf
         50, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        759 // PA gain
      },
      {
          0, // PA Mode
         24, // Prf
         58, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        774 // PA gain
      },
   }
};

/* BAND8...................................................................*/
U_sPAOCTLVLSETTING  BAND8_PaOctLevData =
{
   /* Available PA 8-level sections set by user */
     8
   ,
   /* Reserved byte */
     0
   ,
   /* PA phase compensation */
   {
     0x0,
     0x0,
     0x0
   },
   /* PMU OCT level control table */
   {
      {
          2, // PA Mode
          0, // Prf
          4, // DC2DC level
          1, // Vm0
          1, // Vm1
        490, // Vbias
        221 // PA gain
      },
      {
          1, // PA Mode
          6, // Prf
         14, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        427 // PA gain
      },
      {
          1, // PA Mode
         10, // Prf
         20, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        432 // PA gain
      },
      {
          0, // PA Mode
         15, // Prf
         32, // DC2DC level
          0, // Vm0
          0, // Vm1
        580, // Vbias
        701 // PA gain
      },
      {
          0, // PA Mode
         18, // Prf
         36, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        720 // PA gain
      },
      {
          0, // PA Mode
         20, // Prf
         46, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        731 // PA gain
      },
      {
          0, // PA Mode
         22, // Prf
         50, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        734 // PA gain
      },
      {
          0, // PA Mode
         24, // Prf
         58, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        740 // PA gain
      },
   }
};

/* BAND9...................................................................*/
U_sPAOCTLVLSETTING  BAND9_PaOctLevData =
{
   /* Available PA 8-level sections set by user */
     8
   ,
   /* Reserved byte */
     0
   ,
   /* PA phase compensation */
   {
     0x0,
     0x0,
     0x0
   },
   /* PMU OCT level control table */
   {
      {
          2, // PA Mode
          0, // Prf
          4, // DC2DC level
          1, // Vm0
          1, // Vm1
        490, // Vbias
        172 // PA gain
      },
      {
          2, // PA Mode
          4, // Prf
         14, // DC2DC level
          1, // Vm0
          1, // Vm1
        580, // Vbias
        227 // PA gain
      },
      {
          1, // PA Mode
          4, // Prf
         20, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        403 // PA gain
      },
      {
          1, // PA Mode
          8, // Prf
         32, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        407 // PA gain
      },
      {
          1, // PA Mode
         10, // Prf
         36, // DC2DC level
          1, // Vm0
          0, // Vm1
        660, // Vbias
        417 // PA gain
      },
      {
          1, // PA Mode
         12, // Prf
         46, // DC2DC level
          1, // Vm0
          0, // Vm1
        660, // Vbias
        418 // PA gain
      },
      {
          0, // PA Mode
         12, // Prf
         50, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        708 // PA gain
      },
      {
          0, // PA Mode
         24, // Prf
         58, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        725 // PA gain
      },
   }
};

/* BAND10...................................................................*/
U_sPAOCTLVLSETTING  BAND10_PaOctLevData =
{
   /* Available PA 8-level sections set by user */
     8
   ,
   /* Reserved byte */
     0
   ,
   /* PA phase compensation */
   {
     0x0,
     0x0,
     0x0
   },
   /* PMU OCT level control table */
   {
      {
          2, // PA Mode
          0, // Prf
          4, // DC2DC level
          1, // Vm0
          1, // Vm1
        490, // Vbias
        172 // PA gain
      },
      {
          2, // PA Mode
          4, // Prf
         14, // DC2DC level
          1, // Vm0
          1, // Vm1
        580, // Vbias
        227 // PA gain
      },
      {
          1, // PA Mode
          4, // Prf
         20, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        403 // PA gain
      },
      {
          1, // PA Mode
          8, // Prf
         32, // DC2DC level
          1, // Vm0
          0, // Vm1
        580, // Vbias
        407 // PA gain
      },
      {
          1, // PA Mode
         10, // Prf
         36, // DC2DC level
          1, // Vm0
          0, // Vm1
        660, // Vbias
        417 // PA gain
      },
      {
          1, // PA Mode
         12, // Prf
         46, // DC2DC level
          1, // Vm0
          0, // Vm1
        660, // Vbias
        418 // PA gain
      },
      {
          0, // PA Mode
         12, // Prf
         50, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        708 // PA gain
      },
      {
          0, // PA Mode
         24, // Prf
         58, // DC2DC level
          0, // Vm0
          0, // Vm1
        660, // Vbias
        725 // PA gain
      },
   }
};

/* PA 8-level control */
U_sPAOCTLVLSETTING*  U_PA_OCTLEV_TABLE[] =
{
   &BANDANY_PaOctLevData,
   &BAND1_PaOctLevData,
   &BAND2_PaOctLevData,
   &BAND4_PaOctLevData,
   &BAND5_PaOctLevData,  
   &BAND8_PaOctLevData,
};
/*===============================================================================================*/
U_sPADRIFTSETTING  BANDANY_PaDriftCompData =
{
   //pa_drift_comp_w_table
   {    /* (Slot0, Slot1) */
      {  0, 0 } , //  L=>M
      {  0, 0 } , //  L=>H
      {  0, 0 } , //  M=>L
      {  0, 0 } , //  M=>H
      {  0, 0 } , //  H=>L
      {  0, 0 } , //  H=>M
   },

   //pa_drift_comp_h_table
   {    /* (Slot00, Slot01, slot10, slot11) */
      {  0, 0, 0, 0 } , //  L=>M
      {  0, 0, 0, 0 } , //  L=>H
      {  0, 0, 0, 0 } , //  M=>L
      {  0, 0, 0, 0 } , //  M=>H
      {  0, 0, 0, 0 } , //  H=>L
      {  0, 0, 0, 0 } , //  H=>M
  }
};

U_sPADRIFTSETTING  BAND1_PaDriftCompData =
{
   //pa_drift_comp_w_table
   {    /* (Slot0, Slot1) */
      {  0, 0 } , //  L=>M
      {  0, 0 } , //  L=>H
      {  0, 0 } , //  M=>L
      {  0, 0 } , //  M=>H
      {  0, 0 } , //  H=>L
      {  0, 0 } , //  H=>M
   },

   //pa_drift_comp_h_table
   {    /* (Slot00, Slot01, slot10, slot11) */
      {  0, 0, 0, 0 } , //  L=>M
      {  0, 0, 0, 0 } , //  L=>H
      {  0, 0, 0, 0 } , //  M=>L
      {  0, 0, 0, 0 } , //  M=>H
      {  0, 0, 0, 0 } , //  H=>L
      {  0, 0, 0, 0 } , //  H=>M
  }
};

U_sPADRIFTSETTING  BAND2_PaDriftCompData =
{
   //pa_drift_comp_w_table
   {    /* (Slot0, Slot1) */
      {  0, 0 } , //  L=>M
      {  0, 0 } , //  L=>H
      {  0, 0 } , //  M=>L
      {  0, 0 } , //  M=>H
      {  0, 0 } , //  H=>L
      {  0, 0 } , //  H=>M
   },

   //pa_drift_comp_h_table
   {    /* (Slot00, Slot01, slot10, slot11) */
      {  0, 0, 0, 0 } , //  L=>M
      {  0, 0, 0, 0 } , //  L=>H
      {  0, 0, 0, 0 } , //  M=>L
      {  0, 0, 0, 0 } , //  M=>H
      {  0, 0, 0, 0 } , //  H=>L
      {  0, 0, 0, 0 } , //  H=>M
  }
};

U_sPADRIFTSETTING  BAND3_PaDriftCompData =
{
   //pa_drift_comp_w_table
   {    /* (Slot0, Slot1) */
      {  0, 0 } , //  L=>M
      {  0, 0 } , //  L=>H
      {  0, 0 } , //  M=>L
      {  0, 0 } , //  M=>H
      {  0, 0 } , //  H=>L
      {  0, 0 } , //  H=>M
   },

   //pa_drift_comp_h_table
   {    /* (Slot00, Slot01, slot10, slot11) */
      {  0, 0, 0, 0 } , //  L=>M
      {  0, 0, 0, 0 } , //  L=>H
      {  0, 0, 0, 0 } , //  M=>L
      {  0, 0, 0, 0 } , //  M=>H
      {  0, 0, 0, 0 } , //  H=>L
      {  0, 0, 0, 0 } , //  H=>M
  }
};

U_sPADRIFTSETTING  BAND4_PaDriftCompData =
{
   //pa_drift_comp_w_table
   {    /* (Slot0, Slot1) */
      {  0, 0 } , //  L=>M
      {  0, 0 } , //  L=>H
      {  0, 0 } , //  M=>L
      {  0, 0 } , //  M=>H
      {  0, 0 } , //  H=>L
      {  0, 0 } , //  H=>M
   },

   //pa_drift_comp_h_table
   {    /* (Slot00, Slot01, slot10, slot11) */
      {  0, 0, 0, 0 } , //  L=>M
      {  0, 0, 0, 0 } , //  L=>H
      {  0, 0, 0, 0 } , //  M=>L
      {  0, 0, 0, 0 } , //  M=>H
      {  0, 0, 0, 0 } , //  H=>L
      {  0, 0, 0, 0 } , //  H=>M
  }
};

U_sPADRIFTSETTING  BAND5_PaDriftCompData =
{
   //pa_drift_comp_w_table
   {    /* (Slot0, Slot1) */
      {  0, 0 } , //  L=>M
      {  0, 0 } , //  L=>H
      {  0, 0 } , //  M=>L
      {  0, 0 } , //  M=>H
      {  0, 0 } , //  H=>L
      {  0, 0 } , //  H=>M
   },

   //pa_drift_comp_h_table
   {    /* (Slot00, Slot01, slot10, slot11) */
      {  0, 0, 0, 0 } , //  L=>M
      {  0, 0, 0, 0 } , //  L=>H
      {  0, 0, 0, 0 } , //  M=>L
      {  0, 0, 0, 0 } , //  M=>H
      {  0, 0, 0, 0 } , //  H=>L
      {  0, 0, 0, 0 } , //  H=>M
  }
};

U_sPADRIFTSETTING  BAND6_PaDriftCompData =
{
   //pa_drift_comp_w_table
   {    /* (Slot0, Slot1) */
      {  0, 0 } , //  L=>M
      {  0, 0 } , //  L=>H
      {  0, 0 } , //  M=>L
      {  0, 0 } , //  M=>H
      {  0, 0 } , //  H=>L
      {  0, 0 } , //  H=>M
   },

   //pa_drift_comp_h_table
   {    /* (Slot00, Slot01, slot10, slot11) */
      {  0, 0, 0, 0 } , //  L=>M
      {  0, 0, 0, 0 } , //  L=>H
      {  0, 0, 0, 0 } , //  M=>L
      {  0, 0, 0, 0 } , //  M=>H
      {  0, 0, 0, 0 } , //  H=>L
      {  0, 0, 0, 0 } , //  H=>M
  }
};

U_sPADRIFTSETTING  BAND8_PaDriftCompData =
{
   //pa_drift_comp_w_table
   {    /* (Slot0, Slot1) */
      {  0, 0 } , //  L=>M
      {  0, 0 } , //  L=>H
      {  0, 0 } , //  M=>L
      {  0, 0 } , //  M=>H
      {  0, 0 } , //  H=>L
      {  0, 0 } , //  H=>M
   },

   //pa_drift_comp_h_table
   {    /* (Slot00, Slot01, slot10, slot11) */
      {  0, 0, 0, 0 } , //  L=>M
      {  0, 0, 0, 0 } , //  L=>H
      {  0, 0, 0, 0 } , //  M=>L
      {  0, 0, 0, 0 } , //  M=>H
      {  0, 0, 0, 0 } , //  H=>L
      {  0, 0, 0, 0 } , //  H=>M
  }
};

U_sPADRIFTSETTING  BAND9_PaDriftCompData =
{
   //pa_drift_comp_w_table
   {    /* (Slot0, Slot1) */
      {  0, 0 } , //  L=>M
      {  0, 0 } , //  L=>H
      {  0, 0 } , //  M=>L
      {  0, 0 } , //  M=>H
      {  0, 0 } , //  H=>L
      {  0, 0 } , //  H=>M
   },

   //pa_drift_comp_h_table
   {    /* (Slot00, Slot01, slot10, slot11) */
      {  0, 0, 0, 0 } , //  L=>M
      {  0, 0, 0, 0 } , //  L=>H
      {  0, 0, 0, 0 } , //  M=>L
      {  0, 0, 0, 0 } , //  M=>H
      {  0, 0, 0, 0 } , //  H=>L
      {  0, 0, 0, 0 } , //  H=>M
  }
};

U_sPADRIFTSETTING  BAND10_PaDriftCompData =
{
   //pa_drift_comp_w_table
   {    /* (Slot0, Slot1) */
      {  0, 0 } , //  L=>M
      {  0, 0 } , //  L=>H
      {  0, 0 } , //  M=>L
      {  0, 0 } , //  M=>H
      {  0, 0 } , //  H=>L
      {  0, 0 } , //  H=>M
   },

   //pa_drift_comp_h_table
   {    /* (Slot00, Slot01, slot10, slot11) */
      {  0, 0, 0, 0 } , //  L=>M
      {  0, 0, 0, 0 } , //  L=>H
      {  0, 0, 0, 0 } , //  M=>L
      {  0, 0, 0, 0 } , //  M=>H
      {  0, 0, 0, 0 } , //  H=>L
      {  0, 0, 0, 0 } , //  H=>M
  }
};

U_sPADRIFTSETTING*  U_PA_DRIFT_TABLE[] =
{
   &BANDANY_PaDriftCompData,
   &BAND1_PaDriftCompData,
   &BAND2_PaDriftCompData,
   &BAND4_PaDriftCompData,
   &BAND5_PaDriftCompData,  
   &BAND8_PaDriftCompData,
};


U_sPARACHTMCOMPDATA BANDANY_RACH_temperature_compensation =
{
  {
    {
      {
        { -64, 0 } ,
        { -64, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
     }
   },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    }
  }
};

U_sPARACHTMCOMPDATA BAND1_RACH_temperature_compensation =
{
  {
    {
      {
        { -64, 0 } ,
        { -64, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
     }
   },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    }
  }
};

U_sPARACHTMCOMPDATA BAND2_RACH_temperature_compensation =
{
  {
    {
      {
        { -64, 0 } ,
        { -64, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
     }
   },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    }
  }
};

U_sPARACHTMCOMPDATA BAND3_RACH_temperature_compensation =
{
  {
    {
      {
        { -64, 0 } ,
        { -64, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
     }
   },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    }
  }
};

U_sPARACHTMCOMPDATA BAND4_RACH_temperature_compensation =
{
  {
    {
      {
        { -64, 0 } ,
        { -64, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
     }
   },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    }
  }
};

U_sPARACHTMCOMPDATA BAND5_RACH_temperature_compensation =
{
  {
    {
      {
        { -64, 0 } ,
        { -64, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
     }
   },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    }
  }
};

U_sPARACHTMCOMPDATA BAND6_RACH_temperature_compensation =
{
  {
    {
      {
        { -64, 0 } ,
        { -64, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
     }
   },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    }
  }
};

U_sPARACHTMCOMPDATA BAND7_RACH_temperature_compensation =
{
  {
    {
      {
        { -64, 0 } ,
        { -64, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
     }
   },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    }
  }
};

U_sPARACHTMCOMPDATA BAND8_RACH_temperature_compensation =
{
  {
    {
      {
        { -64, 0 } ,
        { -64, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
     }
   },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    }
  }
};

U_sPARACHTMCOMPDATA BAND9_RACH_temperature_compensation =
{
  {
    {
      {
        { -64, 0 } ,
        { -64, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
     }
   },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    }
  }
};

U_sPARACHTMCOMPDATA BAND10_RACH_temperature_compensation =
{
  {
    {
      {
        { -64, 0 } ,
        { -64, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
     }
   },
   {  
     {
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
        { 0, 0 } ,
      }
    }
  }
};



U_sPARACHTMCOMPDATA*  U_PA_RACH_COMP_TABLE[] =
{
   &BANDANY_RACH_temperature_compensation,
   &BAND1_RACH_temperature_compensation,
   &BAND2_RACH_temperature_compensation,
   &BAND4_RACH_temperature_compensation,
   &BAND5_RACH_temperature_compensation,  
   &BAND8_RACH_temperature_compensation,
};
