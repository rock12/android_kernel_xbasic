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
 *   camera_sensor_para.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Header file of Sensor tuning parameters that should be generated by CCT
 *
 *
 * Author:
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: 1.1 $
 * $Modtime:$
 * $Log: ov5647_Camera_Sensor_para.h,v $
 * Revision 1.1  2013/05/23 03:38:40  ningzhiyu
 * Editor: ningzhiyu
 * Inspector:ningzhiyu
 * Reason: create 72 JB3 module
 * Reference:none
 *
 * Revision 1.1  2013/04/18 12:40:02  huangwenjian
 * Bug[none]
 *
 * Reason:12Y36 T1000  ov8825 & ov5647
 * Author:Huangwenjian
 *
 * Revision 1.1  2013/04/13 03:49:15  huangwenjian
 * Bug[none]
 *
 * Reason:12Y36 T1000 sub ov5647 AR0833
 * Author:Huangwenjian
 *
 *
 * 12 16 2011 koli.lin
 * [ALPS00030473] [Camera]
 * [Camera] Add the OV5647 sensor driver to perforce.
 *
 * 
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef _OV5647_CAMERA_SENSOR_PARA_H
#define _OV5647_CAMERA_SENSOR_PARA_H

#define OV5647_CAMERA_SENSOR_REG_DEFAULT_VALUE \
{ \
  {0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF}, \
  {0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF}, \
  {0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF}, \
  {0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF}, \
  {0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF}, \
  {0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF}, \
  {0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF}, \
  {0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF}, \
  {0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF}, \
  {0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,0xFFFFFFFF},{0xFFFFFFFF,ISP_DRIVING_6MA}, \
}

#define OV5647_CAMERA_SENSOR_CCT_DEFAULT_VALUE \
{ \
  {0xFFFFFFFF,0x40},{0xFFFFFFFF,0x40},{0xFFFFFFFF,0x40},{0xFFFFFFFF,0x40},{0xFFFFFFFF,0x40}, \
}

#endif /* __CAMERA_SENSOR_PARA_H */
/* SENSOR FULL SIZE */
