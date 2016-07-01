# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.


#
# Copyright (C) 2007 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

PRODUCT_BRAND := generic
PRODUCT_DEVICE := generic
PRODUCT_NAME := core

ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),ccpo)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Ringtone01.mid \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Ringtone_1.mp3

else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),htl)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Heaven.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=FriendlyGhost.ogg


else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),w3b_in)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=OnTheHunt.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Ringtone_5.mp3

else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),new_103)

PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Heaven.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=FriendlyGhost.ogg

else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),z2)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Tinkerbell.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=CaribbeanIce.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),g18)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=OnTheHunt.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Childhood.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),hyundai)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=AMessage_default.mp3 \
    ro.config.alarm_alert=Alarm_default.mp3 \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=AKiss_Baby_default.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),malata)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Ep_Dolami.mp3 \
    ro.config.alarm_alert=Beep_Beep_Alarm.mp3 \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Malata_Voice.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),mobiwire)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=AMessage_default.mp3 \
    ro.config.alarm_alert=Alarm_default.mp3 \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=sun_default.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),doov)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=marimba.ogg \
    ro.config.alarm_alert=spring_morning.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=crystal.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),Z500_HINDI)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=SMS_1_Default.mp3 \
    ro.config.alarm_alert=Alarmtone.mp3 \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=iBall_Default_1.mp3

else ifeq ($(strip $(BIRD_CUSTOM_TIMEZONE_CFG)),Taipei)

PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=OnTheHunt.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Taipei \
    ro.config.ringtone=Backroad.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),BP603)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Weight.mp3 \
    ro.config.alarm_alert=NewDay.mp3 \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Windchimer.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),thl_w3)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Heaven.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=FriendlyGhost.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),thl)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=OnTheHunt.ogg \
    ro.config.alarm_alert=Ticktac.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=ThL_ringtone.mid
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),thl_151F)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Airport_57.mp3 \
    ro.config.alarm_alert=Ticktac.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=3d_Bass_Mix_Rington_37.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),thl_162C)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Airport.mp3 \
    ro.config.alarm_alert=Ticktac.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Rain_Drops.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),B15W)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=message06.mp3 \
    ro.config.alarm_alert=alarm05.mp3 \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=ringtone07.mp3 
     
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),X15)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Tinkerbell.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=X15_incomingCall.ogg

else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),X20)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Tinkerbell.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=X15_incomingCall.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),X20_VITA)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Tinkerbell.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=BeatPlucker.ogg

else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),X20_100CAN)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Tinkerbell.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=100Can_IncomingRing.mp3

else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),P3513_EKT)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=ekt.mid \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=ekt.mid
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),bdf)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Tinkerbell.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=bdf_ringtone.OGG
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),bdf_dt)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Tinkerbell.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Bollywood.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),ymx)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=OnTheHunt.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Ring_Classic_02.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),h22)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=OnTheHunt.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Terminated.wav
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),baike)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Tinkerbell.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=baike_ringtone.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),IGUO_I888)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=OnTheHunt.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Ring_Classic_02.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),bird)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Sms_0.mp3 \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=RingTone_0.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),N105)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Friendly.mp3 \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=2_Step.mp3
else ifeq ($(strip $(BIRD_CUSTOM_TIMEZONE_CFG)),INDIA)

PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=OnTheHunt.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Colombo \
    ro.config.ringtone=Backroad.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),DXUAN)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=OnTheHunt.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=FriendlyGhost.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),HOOW)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=HOOW_messanges_default.mp3 \
    ro.config.alarm_alert=HOOW_ALARM.mp3 \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=HOOW_INCOMING_CALL.mp3    
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),P3513D_YM)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=OnTheHunt.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=ringtone1.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),A6M)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=sms10_mp3.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=ring13_mp3.ogg

else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),D521P)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=xylophone.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=America/Chicago \
    ro.config.ringtone=piano.ogg
    
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),e39)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=arpeggio.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=celeste.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),s11m_ckk)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=arpeggio.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=S11M_CKK_boot.mp3
     
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),mobitel)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=arpeggio.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Africa/Cairo \
    ro.config.ringtone=celeste.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),s11m_pop)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=HappyMessage.mp3 \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=HappyRingtone.mp3

else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),I580_T8)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=pixiedust.mp3 \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Africa/Nairobi \
    ro.config.ringtone=digitalphone.mp3 \
    ro.config.ringtone2=digitalphone.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),mg009)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=arpeggio.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=MG009_RING.mp3     
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),X5W)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=DTC_SMS.mp3 \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=DTC_Bell.mp3 
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),t56a)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=arpeggio.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=GuXiangDeYun.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),T9600C)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=TwirlAway.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=TwirlAway.ogg   
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),SIWER_A6M)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=arpeggio.ogg \
    ro.config.alarm_alert=Ticktac.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=siwer_ringtone.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),Y05C)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=message2.aac \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=ringtone1.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),V26E_DTC)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=dtc_sms.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Singapore \
    ro.config.ringtone=dtc_bell.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),dtc)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=dtc_sms.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Singapore \
    ro.config.ringtone=dtc_bell.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),KUBAO)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=arpeggio.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=ringtone01.mp3
    else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),I599)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Cloud.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Over_the_horizon.ogg

 
 else ifeq ($(strip $(BIRD_CUSTOM_TIMEZONE_CFG)),F7)
    PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=arpeggio.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Europe/Moscow \
    ro.config.ringtone=celeste.ogg  

else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),BIRD_E700S)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=arpeggio.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=spring_morning.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),S8_JT)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=pixiedust.ogg \
    ro.config.alarm_alert=Alarm_Classic.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Ring_Synth.ogg
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),v3493)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=VIMOOSMS.mp3 \
    ro.config.alarm_alert=Walk_in_the_forest.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=VimmoRing.ogg     
    
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),k3492)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=VIMOOSMS.mp3 \
    ro.config.alarm_alert=Walk_in_the_forest.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=VimmoRing.ogg     
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),m3491)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=mofutN.ogg \
    ro.config.alarm_alert=A_toy_watch.mp3 \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=melodioustone.mp3    
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),NEW_UI_1)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Alpha.ogg \
    ro.config.alarm_alert=Beat.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=Atria.ogg    
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),GREENTEL)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=arpeggio.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Colombo \
    ro.config.ringtone=Sequence.mp3
else ifeq ($(strip $(BIRD_CUSTOM_RING_CFG)),W101_IT)
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=Proxima.ogg \
    ro.config.alarm_alert=Alarm_Beep_01.ogg \
    persist.sys.timezone=Europe/Brussels \
    ro.config.ringtone=Backroad.ogg \
    ro.config.ringtone2=Paradise_Island.ogg
else
PRODUCT_PROPERTY_OVERRIDES := \
    ro.config.notification_sound=arpeggio.ogg \
    ro.config.alarm_alert=Dawn_chorus.ogg \
    persist.sys.timezone=Asia/Shanghai \
    ro.config.ringtone=celeste.ogg    
endif

PRODUCT_PACKAGES += \
    ApplicationsProvider \
    BackupRestoreConfirmation \
    Browser \
    Contacts \
    ContactsProvider \
    DefaultContainerService \
    DownloadProvider \
    DownloadProviderUi \
    HTMLViewer \
    Home \
    KeyChain \
    MediaProvider \
    PackageInstaller \
    PicoTts \
    SettingsProvider \
    SharedStorageBackup \
    TelephonyProvider \
    UserDictionaryProvider \
    VpnDialogs \
    abcc \
    apache-xml \
    atrace \
    bouncycastle \
    bu \
    cacerts \
    com.android.location.provider \
    com.android.location.provider.xml \
    core \
    core-junit \
    dalvikvm \
    dexdeps \
    dexdump \
    dexlist \
    dexopt \
    dmtracedump \
    drmserver \
    dx \
    ext \
    framework-res \
    hprof-conv \
    icu.dat \
    installd \
    ip \
    ip-up-vpn \
    ip6tables \
    iptables \
    keystore \
    keystore.default \
    libandroidfw \
    libOpenMAXAL \
    libOpenSLES \
    libaudiopreprocessing \
    libaudioutils \
    libbcc \
    libcrypto \
    libdownmix \
    libdvm \
    libdrmframework \
    libdrmframework_jni \
    libexpat \
    libfilterfw \
    libfilterpack_imageproc \
    libgabi++ \
    libicui18n \
    libicuuc \
    libjavacore \
    libkeystore \
    libmdnssd \
    libnativehelper \
    libnfc_ndef \
    libportable \
    libpowermanager \
    libspeexresampler \
    libsqlite_jni \
    libssl \
    libstagefright \
    libstagefright_chromium_http \
    libstagefright_soft_aacdec \
    libstagefright_soft_aacenc \
    libstagefright_soft_amrdec \
    libstagefright_soft_amrnbenc \
    libstagefright_soft_amrwbenc \
    libstagefright_soft_flacenc \
    libstagefright_soft_g711dec \
    libstagefright_soft_h264dec \
    libstagefright_soft_h264enc \
    libstagefright_soft_mp3dec \
    libstagefright_soft_mpeg4dec \
    libstagefright_soft_mpeg4enc \
    libstagefright_soft_vorbisdec \
    libstagefright_soft_vpxdec \
    libstagefright_soft_rawdec \
    libvariablespeed \
    libwebrtc_audio_preprocessing \
    libwilhelm \
    libz \
    make_ext4fs \
    mdnsd \
    requestsync \
    screencap \
    jpe_tool \
    libnativecheck-jni \
    sensorservice \
    lint \
    uiautomator \
    telephony-common \
    mms-common \
    zoneinfo.dat \
    zoneinfo.idx \
    zoneinfo.version \
    wpa_supplicant \
    wpa_supplicant.conf \
    p2p_supplicant.conf \
    hald \
    hdc \
    mediatek-res \
    mediatek-common \
    mediatek-framework \
    mediatek-op \
    CustomProperties \
    mediatek-telephony-common

ifneq ($(strip $(MTK_LCA_RAM_OPTIMIZE)), yes)
  PRODUCT_PACKAGES += BasicDreams \
    PhotoTable
endif

ifeq ($(strip $(MTK_AGPS_APP)), yes)
  PRODUCT_PACKAGES += mtk_agpsd \
    libssladp \
    libagpssupl
endif    

#ifneq ($(filter secondary-framework,$(ALL_MODULES)),)
  PRODUCT_PACKAGES += secondary-framework
#endif

PRODUCT_COPY_FILES += \
    system/core/rootdir/init.trace.rc:root/init.trace.rc \
#    system/core/rootdir/init.usb.rc:root/init.usb.rc \

# host-only dependencies
ifeq ($(WITH_HOST_DALVIK),true)
    PRODUCT_PACKAGES += \
        apache-xml-hostdex \
        bouncycastle-hostdex \
        core-hostdex \
        libcrypto \
        libexpat \
        libicui18n \
        libicuuc \
        libjavacore \
        libssl \
        libz-host \
        dalvik \
        zoneinfo-host.dat \
        zoneinfo-host.idx \
        zoneinfo-host.version
endif

ifeq ($(strip $(MTK_FM_SUPPORT)), yes)
  PRODUCT_PACKAGES += FMRadio
endif

ifeq ($(HAVE_SELINUX),true)
    PRODUCT_PACKAGES += \
        sepolicy \
        file_contexts \
        seapp_contexts \
        property_contexts \
        mac_permissions.xml
endif

ifeq (MT6620_FM,$(strip $(MTK_FM_CHIP)))
    PRODUCT_PROPERTY_OVERRIDES += \
        fmradio.driver.chip=1
endif

$(call inherit-product, $(SRC_TARGET_DIR)/product/base.mk)

ifeq (MT6626_FM,$(strip $(MTK_FM_CHIP)))
    PRODUCT_PROPERTY_OVERRIDES += \
        fmradio.driver.chip=2
endif

ifeq (MT6628_FM,$(strip $(MTK_FM_CHIP)))
    PRODUCT_PROPERTY_OVERRIDES += \
        fmradio.driver.chip=3
endif


ifeq ($(GMT_FOTA_SUPPORT),yes)
PRODUCT_PACKAGES += \
        uafs \
        libdmc \
		DMClient \
		flash_image_gmobi 
endif