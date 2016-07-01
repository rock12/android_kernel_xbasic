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
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/* linux/drivers/hwmon/adxl345.c
 *
 * (C) Copyright 2008 
 * MediaTek <www.mediatek.com>
 *
 * BMA220 driver for MT6516
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  BMA220
 */
#ifndef BMA220_H
#define BMA220_H
	 
#include <linux/ioctl.h>
	 
	#define BMA220_I2C_SLAVE_WRITE_ADDR		0x14  //0x30  //0x16  //0x30   //0X32 //0X12   //0x14
	 
	 /* BMA220 Register Map  (Please refer to BMA220 Specifications) */
	#define BMA220_REG_DEVID				(0x00<<1)
	#define BMA220_FIXED_DEVID				0xDD
	#define BMA220_REG_OFSX				(0x14<<1)
	#define	BMA220_REG_BW_RATE			(0x10<<1)
	#define BMA220_BW_MASK				0x0F
	#define BMA220_BW_32HZ				0x05
	#define BMA220_BW_64HZ				0x04	
	#define BMA220_BW_125HZ				0x03
	#define BMA220_BW_250HZ				0x02
	#define BMA220_BW_500HZ				0x01
	#define BMA220_BW_1000HZ				0x00
	#define BMA220_REG_POWER_CTL			(0x0F<<1)	
	#define BMA220_SLEEP_EN_MODE			0x40
	#define BMA220_CHANNEL_MASK			0x07	
	#define BMA220_EN_CHANNEL_X			0x04
	#define BMA220_EN_CHANNEL_Y			0x02
	#define BMA220_EN_CHANNEL_Z			0x01	
	#define BMA220_REG_DATA_FORMAT		(0x11<<1)
	#define BMA220_RANGE_MASK				0x03
	#define BMA220_RANGE_2G				0x00
	#define BMA220_RANGE_4G				0x01
	#define BMA220_RANGE_8G				0x02
	#define BMA220_RANGE_16G				0x03	
	#define BMA220_REG_DATAXLOW			(0x02<<1)	
	#define BMA220_INT_REG           			(0x0D<<1)
	#define BMA220_INT1_REG           			(0x0E<<1)	

	
#define BMA220_SUCCESS						0
#define BMA220_ERR_I2C						-1
#define BMA220_ERR_STATUS					-3
#define BMA220_ERR_SETUP_FAILURE			-4
#define BMA220_ERR_GETGSENSORDATA			-5
#define BMA220_ERR_IDENTIFICATION			-6
	 
	 
	 
#define BMA220_BUFSIZE				256
	 
#endif

