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
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.


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

# This is a generic product that isn't specialized for a specific device.
# It includes the base Android platform.

TARGET_ARCH := arm

PRODUCT_PACKAGES := \
    libI420colorconvert \
    libvcodec_utility \
    libvcodec_oal \
    libh264dec_xa.ca7 \
    libh264dec_xb.ca7 \
    libmp4dec_sa.ca7 \
    libmp4dec_sb.ca7 \
    libvp8dec_xa.ca7 \
    libmp4enc_xa.ca7 \
    libmp4enc_xb.ca7 \
    libh264enc_sa.ca7 \
    libh264enc_sb.ca7 \
    libvc1dec_sa.ca7 \
    libvideoeditorplayer \
    libvideoeditor_osal \
    libvideoeditor_3gpwriter \
    libvideoeditor_mcs \
    libvideoeditor_core \
    libvideoeditor_stagefrightshells \
    libvideoeditor_videofilters \
    libvideoeditor_jni \
    audio.primary.default \
    audio_policy.stub \
    local_time.default \
    libaudiocustparam \
    libh264dec_xa.ca9 \
    libh264dec_xb.ca9 \
    libh264dec_customize \
    libmp4dec_sa.ca9 \
    libmp4dec_sb.ca9 \
    libmp4dec_customize \
    libvp8dec_xa.ca9 \
    libmp4enc_xa.ca9 \
    libmp4enc_xb.ca9 \
    libh264enc_sa.ca9 \
    libh264enc_sb.ca9 \
    libvcodec_oal \
    libvc1dec_sa.ca9 \
    init.factory.rc \
    libaudio.primary.default \
    audio_policy.default \
    libaudio.a2dp.default \
    libMtkVideoTranscoder \
    libMtkOmxCore \
    libMtkOmxOsalUtils \
    libMtkOmxVdec \
    libMtkOmxVenc \
    libaudiodcrflt \
    libaudiosetting \
    librtp_jni \
    mfv_ut \
    libstagefrighthw \
    libstagefright_memutil \
    factory.ini \
    libmtdutil \
    libminiui \
    factory \
    libaudio.usb.default \
    AccountAndSyncSettings \
    DeskClock \
    AlarmProvider \
    Bluetooth \
    Calculator \
    Calendar \
    CertInstaller \
    DrmProvider \
    Email \
    FusedLocation \
    TelephonyProvider \
    Exchange2 \
    LatinIME \
    Music \
    MusicFX \
    Protips \
    QuickSearchBox \
    Settings \
    Sync \
    SystemUI \
    Updater \
    CalendarProvider \
    ccci_mdinit \
    ccci_fsd \
    permission_check \
    batterywarning \
    SyncProvider \
    Launcher2 \
    disableapplist.txt \
    resmonwhitelist.txt \
    MTKThermalManager \
    thermal_manager \
    thermald \
    thermal \
    ApplicationGuide \
    CellConnService \
    MTKAndroidSuiteDaemon \
    libfmjni \
    libfmmt6616 \
    libfmmt6626 \
    libfmmt6620 \
    libfmmt6628 \
    libfmmt6627 \
    libfmar1000 \
    libfmcust \
    fm_cust.cfg \
    mt6620_fm_cust.cfg \
    mt6627_fm_cust.cfg \
    mt6628_fm_rom.bin \
    mt6628_fm_v1_patch.bin \
    mt6628_fm_v1_coeff.bin \
    mt6628_fm_v2_patch.bin \
    mt6628_fm_v2_coeff.bin \
    mt6628_fm_v3_patch.bin \
    mt6628_fm_v3_coeff.bin \
    mt6628_fm_v4_patch.bin \
    mt6628_fm_v4_coeff.bin \
    mt6628_fm_v5_patch.bin \
    mt6628_fm_v5_coeff.bin \
    mt6627_fm_v1_patch.bin \
    mt6627_fm_v1_coeff.bin \
    mt6627_fm_v2_patch.bin \
    mt6627_fm_v2_coeff.bin \
    mt6627_fm_v3_patch.bin \
    mt6627_fm_v3_coeff.bin \
    mt6627_fm_v4_patch.bin \
    mt6627_fm_v4_coeff.bin \
    mt6627_fm_v5_patch.bin \
    mt6627_fm_v5_coeff.bin \
    ami304d \
    akmd8963 \
    akmd8975 \
    akmd09911 \
    st480d \
    geomagneticd \
    orientationd \
    memsicd \
    msensord \
    lsm303md \
    memsicd3416x \
    s62xd smartsensor \
    bmm050d \
    mc6420d \
    qmc5983d \
    magd \
    sensors.mt6577 \
    sensors.mt6589 \
    sensors.default\
    libhwm \
    lights.default \
    libft \
    meta_tst \
    GoogleOtaBinder \
    dm_agent_binder \
    libvdmengine.so \
    libvdmfumo.so \
    libvdmlawmo.so \
    libvdmscinv.so \
    libvdmscomo.so \
    dhcp6c \
    dhcp6ctl \
    dhcp6c.conf \
    dhcp6cDNS.conf \
    dhcp6s \
    dhcp6s.conf \
    dhcp6c.script \
    dhcp6cctlkey \
    libblisrc \
    libifaddrs \
    libbluetoothdrv \
    libbluetooth_mtk \
    libbluetoothem_mtk \
    libbluetooth_relayer \
    libmeta_bluetooth \
    mobile_log_d \
    libmobilelog_jni \
    libaudio.r_submix.default \
    libaudio.usb.default \
    libnbaio \
    libaudioflinger \
    libmeta_audio \
    sysctl \
    sysctld \
    liba3m \
    libja3m \
    libmmprofile \
    libmmprofile_jni \
    libtvoutjni \
    libtvoutpattern \
    libmtkhdmi_jni \
    aee \
    aee_aed \
    aee_core_forwarder \
    aee_dumpstate \
    rtt \
    libaed.so \
    libmediatek_exceptionlog\
    camera.default \
    xlog \
    liblog \
    shutdown \
    WIFI_RAM_CODE \
    WIFI_RAM_CODE_E6 \
    WIFI_RAM_CODE_MT6628 \
    muxreport \
    rild \
    mtk-ril \
    librilmtk \
    libutilrilmtk \
    gsm0710muxd \
    rildmd2 \
    mtk-rilmd2 \
    librilmtkmd2 \
    gsm0710muxdmd2 \
    md_minilog_util \
    wbxml \
    wappush \
    thememap.xml \
    libBLPP.so \
    rc.fac \
    mtkGD \
    pvrsrvctl \
    libEGL_mtk.so \
    libGLESv1_CM_mtk.so \
    libGLESv2_mtk.so \
    gralloc.mt6577.so \
    gralloc.mt6589.so \
    gralloc.mt8125.so \
    gralloc.mt8389.so \
    gralloc.mt6572.so \
    libusc.so \
    libglslcompiler.so \
    libIMGegl.so \
    libpvr2d.so \
    libsrv_um.so \
    libsrv_init.so \
    libPVRScopeServices.so \
    libpvrANDROID_WSEGL.so \
    libFraunhoferAAC \
    libMtkOmxAudioEncBase \
    libMtkOmxAmrEnc \
    libMtkOmxAwbEnc \
    libMtkOmxAacEnc \
    libMtkOmxVorbisEnc \
    libMtkOmxAdpcmEnc \
    libMtkOmxMp3Dec \
    libMtkOmxAacDec \
    libMtkOmxG711Dec \
    libMtkOmxVorbisDec \
    libMtkOmxAudioDecBase \
    libMtkOmxAdpcmDec \
    libMtkOmxWmaDec \
    libMtkOmxRawDec \
    libMtkOmxAMRNBDec \
    libMtkOmxAMRWBDec \
    libvoicerecognition_jni \
    libvoicerecognition \
    libphonemotiondetector_jni \
    libphonemotiondetector \
    libmotionrecognition \
    libasf \
    libasfextractor \
    audio.primary.default \
    audio_policy.stub \
    audio_policy.default \
    libaudio.primary.default \
    libaudio.a2dp.default \
    libaudio-resampler \
    local_time.default \
    libaudiocustparam \
    libaudiodcrflt \
    libaudiosetting \
    librtp_jni \
    libmatv_cust \
    libmtkplayer \
    libatvctrlservice \
    matv \
    libMtkOmxApeDec \
    libMtkOmxFlacDec \
    ppp_dt \
    power.default \
    libdiagnose \
    netdiag \
    mnld \
    libmnlp \
    libmnlp_mt6628 \
    libmnlp_mt6620 \
    libmnlp_mt3332 \
    libmnlp_mt6572 \
    gps.default\
    libmnl.a \
    libsupl.a \
    libhotstill.a \
    libagent.a \
    libsonivox \
    iAmCdRom.iso \
    libmemorydumper \
    memorydumper \
    libvt_custom \
    libamrvt \
    libvtmal \
    racoon \
    libipsec \
    libpcap \
    mtpd \
    netcfg \
    pppd \
    pppd_dt \
    dhcpcd \
    dhcpcd.conf \
    dhcpcd-run-hooks \
    20-dns.conf \
    95-configured \
    radvd \
    radvd.conf \
    dnsmasq \
    netd \
    ndc \
    libiprouteutil \
    libnetlink \
    tc \
    libext2_profile \
    e2fsck \
    libext2_blkid \
    libext2_e2p \
    libext2_com_err \
    libext2fs \
    libext2_uuid \
    mke2fs \
    tune2fs \
    badblocks \
    resize2fs \
    libnvram \
    libnvram_daemon_callback \
    libfile_op \
    nvram_agent_binder \
    nvram_daemon \
    make_ext4fs \
    sdcard \
    libext \
    libext \
    libext4 \
    libext6 \
    libxtables \
    libip4tc \
    libip6tc \
    ipod \
    libipod \
    ipohctl \
    boot_logo_updater\
    boot_logo\
    bootanimation\
    libtvoutjni \
    libtvoutpattern \
    libmtkhdmi_jni \
    libhissage.so \
    libhpe.so \
    sdiotool \
    superumount \
    libsched \
    fsck_msdos_mtk \
    cmmbsp \
    libcmmb_jni \
    robotium \
    libc_malloc_debug_mtk \
    dpfd \
    libaal \
    aal \
    SchedulePowerOnOff \
    BatteryWarning \
    pq \
    wlan_loader \
    showmap \
    tiny_mkswap \
    tiny_swapon \
    tiny_swapoff \
    dmlog \
    mtk_msr.ko \
    ext4_resize \
    mtop \
    send_bug \
    met-cmd \
    libmet-tag \
    libMtkOmxRawDec \
    libperfservice \
    libperfservice_test \
    VideoPlayer \
    sn \
    lcdc_screen_cap \
    libJniAtvService \
    smsSelfRegConfig.xml \
    terservice \
    libterservice
    
#bird framework lib
    PRODUCT_PACKAGES += libtorch_switch \
    libfactorytest_jni \
    libmmitest_jni \
    libbirdvalidate \
    libengineerproximityjni \
    libproximityjni \
    libgsensor_jni \
    libIncomingCallShow-jni
    
ifeq ($(strip $(MTK_FLV_PLAYBACK_SUPPORT)), yes)
    PRODUCT_PACKAGES += libflv \
    libflvextractor
endif
ifeq ($(strip $(MTK_CELL_BROADCAST_RECEIVER_SUPPORT)), yes) 
  PRODUCT_PACKAGES += CellBroadcastReceiver
endif

ifneq ($(strip $(foreach value,$(DFO_NVRAM_SET),$(filter yes,$($(value))))),)
  PRODUCT_PACKAGES += \
    featured \
    libdfo \
    libdfo_jni
endif
#For BBK add by zhouleigang 20131106 begin 
#weacher
ifeq ($(strip $(BIRD_WIDGET_WEATHER_SUPPORT)), yes)
  PRODUCT_PACKAGES += BirdWidgetWeather
endif
#cpu 2
ifeq ($(strip $(BIRD_CPUWIDGET)), yes)
ifeq ($(strip $(BIRD_LAUNCHER_FLYING_EFFECT)), yes)
  PRODUCT_PACKAGES += CpuWidgetNewVersion
endif
endif
#cpu 4 processor
ifeq ($(strip $(BIRD_CPUWIDGET_FOUR_PROCESSOR_NEW_STYLE)), yes)
ifeq ($(strip $(BIRD_LAUNCHER_FLYING_EFFECT)), yes)
  PRODUCT_PACKAGES += CpuWidgetFourProcessor
endif
endif

#For BBK DeskTop
ifeq ($(strip $(BIRD_SCENE_DESKTOP_SUPPORT)), yes)
  PRODUCT_PACKAGES += BirdSceneDesktop
endif
#For BBK digitalClock
ifeq ($(strip $(BIRD_DIGITAL_CLOCK_SUPPORT)), yes)
  PRODUCT_PACKAGES += BirdDigitalClock
endif
#For BBK caculator
ifeq ($(strip $(SMARTISAN_CALCULATOR_SUPPORT)), yes)
  PRODUCT_PACKAGES += SmartisanCalculator
endif
#for Ismart zone
ifeq ($(strip $(BIRD_ISMART_ZONE)), yes)
  PRODUCT_PACKAGES += ISmartZone
endif
#For prevenstolen
ifeq ($(strip $(BIRD_PREVENT_STOLEN)), yes)
  PRODUCT_PACKAGES += PreventionStolen
endif
#For BBK add by zhouleigang 20131106 end

ifeq ($(strip $(MTK_CMAS_SUPPORT)), yes)
  PRODUCT_PACKAGES += CellBroadcastReceiver \
                      CmasEM
endif

ifeq ($(strip $(MTK_CDS_EM_SUPPORT)), yes)
  PRODUCT_PACKAGES += CDS_INFO
endif

ifeq ($(strip $(MTK_WLAN_SUPPORT)), yes)
  PRODUCT_PACKAGES += WIFI_RAM_CODE_MT6582
endif
#
ifeq ($(strip $(MTK_QQBROWSER_SUPPORT)), yes)
  PRODUCT_PACKAGES += QQBrowser
endif

ifeq ($(strip $(MTK_TENCENT_MOBILE_MANAGER_NORMAL_SUPPORT)), yes)
  PRODUCT_PACKAGES += Tencent_Mobile_Manager_Normal
endif

ifeq ($(strip $(MTK_TENCENT_MOBILE_MANAGER_SLIM_SUPPORT)), yes)
  PRODUCT_PACKAGES += Tencent_Mobile_Manager_Slim
endif
#
ifeq ($(strip $(MTK_NFC_SUPPORT)), yes)
    PRODUCT_PACKAGES += nfcservice
endif
#
ifeq ($(strip $(MTK_BAIDU_LOCATION_SUPPORT)), yes)
  PRODUCT_PACKAGES += Baidu_Location
endif
#
PRODUCT_COPY_FILES += \
   system/extras/bugmailer/bugmailer.sh:system/bin/bugmailer.sh \
    system/extras/bugmailer/send_bug:system/bin/send_bug

ifeq ($(strip $(MTK_COMBO_SUPPORT)), yes)

BUILD_MT6620 := false
BUILD_MT6628 := false
BUILD_MT6582 := false

  PRODUCT_PACKAGES += WMT.cfg \
    6620_launcher \
    6620_wmt_concurrency \
    6620_wmt_lpbk \
    stp_dump3

ifeq ($(strip $(MTK_COMBO_CHIP)), MT6572_CONSYS)
  PRODUCT_PACKAGES += mt6572_82_patch_e1_0_hdr.bin \
                      mt6572_82_patch_e1_1_hdr.bin \
                      WMT_SOC.cfg
endif

ifneq ($(filter MT6620E3,$(MTK_COMBO_CHIP)),)
    BUILD_MT6620 := true
endif

ifneq ($(filter MT6620,$(MTK_COMBO_CHIP)),)
    BUILD_MT6620 := true
endif

ifeq ($(strip $(MTK_CMCC_MOBILEMARKET_SUPPORT)), yes)
      PRODUCT_PACKAGES += CMCC_MobileMarket
endif

ifneq ($(filter MT6628,$(MTK_COMBO_CHIP)),)
    BUILD_MT6628 := true
endif


ifeq ($(BUILD_MT6620), true)
  PRODUCT_PACKAGES += mt6620_patch_e3_hdr.bin \
    mt6620_patch_e3_0_hdr.bin \
    mt6620_patch_e3_1_hdr.bin \
    mt6620_patch_e3_2_hdr.bin \
    mt6620_patch_e3_3_hdr.bin \
    mt6620_patch_e6_hdr.bin

  ifneq ($(filter mt6620_ant_m1,$(CUSTOM_HAL_ANT)),)
    PRODUCT_PACKAGES += mt6620_ant_m1.cfg
  endif

  ifneq ($(filter mt6620_ant_m2,$(CUSTOM_HAL_ANT)),)
    PRODUCT_PACKAGES += mt6620_ant_m2.cfg
  endif

  ifneq ($(filter mt6620_ant_m3,$(CUSTOM_HAL_ANT)),)
    PRODUCT_PACKAGES += mt6620_ant_m3.cfg
  endif

  ifneq ($(filter mt6620_ant_m4,$(CUSTOM_HAL_ANT)),)
    PRODUCT_PACKAGES += mt6620_ant_m4.cfg
  endif

  ifneq ($(filter mt6620_ant_m5,$(CUSTOM_HAL_ANT)),)
    PRODUCT_PACKAGES += mt6620_ant_m5.cfg
  endif

  ifneq ($(filter mt6620_ant_m6,$(CUSTOM_HAL_ANT)),)
    PRODUCT_PACKAGES += mt6620_ant_m6.cfg
  endif

  ifneq ($(filter mt6620_ant_m7,$(CUSTOM_HAL_ANT)),)
    PRODUCT_PACKAGES += mt6620_ant_m7.cfg
  endif

endif


ifeq ($(BUILD_MT6628), true)
  PRODUCT_PACKAGES += mt6628_patch_e1_hdr.bin \
    mt6628_patch_e2_hdr.bin \
    mt6628_patch_e2_0_hdr.bin \
    mt6628_patch_e2_1_hdr.bin

  ifneq ($(filter mt6628_ant_m1,$(CUSTOM_HAL_ANT)),)
    PRODUCT_PACKAGES += mt6628_ant_m1.cfg
  endif

  ifneq ($(filter mt6628_ant_m2,$(CUSTOM_HAL_ANT)),)
    PRODUCT_PACKAGES += mt6628_ant_m2.cfg
  endif

  ifneq ($(filter mt6628_ant_m3,$(CUSTOM_HAL_ANT)),)
    PRODUCT_PACKAGES += mt6628_ant_m3.cfg
  endif

  ifneq ($(filter mt6628_ant_m4,$(CUSTOM_HAL_ANT)),)
    PRODUCT_PACKAGES += mt6628_ant_m4.cfg
  endif

endif


ifeq ($(strip $(MTK_VOICE_UNLOCK_SUPPORT)),yes)
  PRODUCT_PACKAGES += VoiceUnlock
endif


endif

ifeq ($(strip $(GEMINI)),yes)
  ifeq ($(strip $(MTK_GEMINI_3SIM_SUPPORT)),yes)
    PRODUCT_PROPERTY_OVERRIDES += \
      persist.gemini.sim_num=3
  else
    ifeq ($(strip $(MTK_GEMINI_4SIM_SUPPORT)),yes)
      PRODUCT_PROPERTY_OVERRIDES += \
         persist.gemini.sim_num=4
    else
      PRODUCT_PROPERTY_OVERRIDES += \
         persist.gemini.sim_num=2
    endif
  endif
else
  PRODUCT_PROPERTY_OVERRIDES += \
     persist.gemini.sim_num=1
endif
#
ifeq ($(strip $(MTK_GEMINI_SMART_SIM_SWITCH)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.gemini.smart_sim_switch=true
else
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.gemini.smart_sim_switch=false
endif
#
######################## MTK_GEMINI_SMART_3G_SWITCH
#ifeq ($(strip $(MTK_GEMINI_SMART_3G_SWITCH)),0)
#  PRODUCT_PROPERTY_OVERRIDES += \
#    ro.gemini.smart_3g_switch=0
#endif
#ifeq ($(strip $(MTK_GEMINI_SMART_3G_SWITCH)),1)
#  PRODUCT_PROPERTY_OVERRIDES += \
#    ro.gemini.smart_3g_switch=1
#endif
#ifeq ($(strip $(MTK_GEMINI_SMART_3G_SWITCH)),2)
#  PRODUCT_PROPERTY_OVERRIDES += \
#    ro.gemini.smart_3g_switch=2
#endif
ifeq ($(strip $(MTK_GEMINI_3G_SWITCH)),yes)
  ifeq ($(strip $(BIRD_WCDMA_SIGNAL)),yes)
    PRODUCT_PROPERTY_OVERRIDES += \
      ro.gemini.smart_3g_switch=1
  else
    PRODUCT_PROPERTY_OVERRIDES += \
      ro.gemini.smart_3g_switch=2
  endif
else
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.gemini.smart_3g_switch=0
endif
######################## MTK_GEMINI_SMART_3G_SWITCH

ifeq ($(strip $(MTK_EMMC_SUPPORT)), yes)
  PRODUCT_DEFAULT_PROPERTY_OVERRIDES += ro.mount.fs=EXT4
else
  ifeq ($(strip $(MTK_NAND_UBIFS_SUPPORT)), yes)
    PRODUCT_DEFAULT_PROPERTY_OVERRIDES += ro.mount.fs=UBIFS
  else
    PRODUCT_DEFAULT_PROPERTY_OVERRIDES += ro.mount.fs=YAFFS
  endif
endif

ifeq ($(strip $(MTK_DATAUSAGE_SUPPORT)), yes)
  ifeq ($(strip $(MTK_DATAUSAGELOCKSCREENCLIENT_SUPPORT)), yes)
    PRODUCT_PACKAGES += DataUsageLockScreenClient
  endif
endif

ifeq ($(strip $(MTK_ENABLE_MD1)),yes)
  ifneq ($(wildcard $(MTK_ROOT_CUSTOM_OUT))/modem/modem_1_2g_n.img),)
    PRODUCT_PACKAGES += modem_1_2g_n.img
    ifeq ($(MTK_MDLOGGER_SUPPORT),yes)
      PRODUCT_PACKAGES += catcher_filter_1_2g_n.bin
    endif
  endif
  ifneq ($(wildcard $(MTK_ROOT_CUSTOM_OUT))/modem/modem_1_wg_n.img),)
    PRODUCT_PACKAGES += modem_1_wg_n.img
    ifeq ($(MTK_MDLOGGER_SUPPORT),yes)
      PRODUCT_PACKAGES += catcher_filter_1_wg_n.bin
    endif
  endif
  ifneq ($(wildcard $(MTK_ROOT_CUSTOM_OUT))/modem/modem_1_tg_n.img),)
    PRODUCT_PACKAGES += modem_1_tg_n.img
    ifeq ($(MTK_MDLOGGER_SUPPORT),yes)
      PRODUCT_PACKAGES += catcher_filter_1_tg_n.bin
    endif
  endif
endif

ifeq ($(strip $(MTK_ENABLE_MD2)),yes)
  ifneq ($(wildcard $(MTK_ROOT_CUSTOM_OUT))/modem/modem_2_2g_n.img),)
    PRODUCT_PACKAGES += modem_2_2g_n.img
    ifeq ($(MTK_MDLOGGER_SUPPORT),yes)
      PRODUCT_PACKAGES += catcher_filter_2_2g_n.bin
    endif
  endif
  ifneq ($(wildcard $(MTK_ROOT_CUSTOM_OUT))/modem/modem_2_wg_n.img),)
    PRODUCT_PACKAGES += modem_2_wg_n.img
    ifeq ($(MTK_MDLOGGER_SUPPORT),yes)
      PRODUCT_PACKAGES += catcher_filter_2_wg_n.bin
    endif
  endif
  ifneq ($(wildcard $(MTK_ROOT_CUSTOM_OUT))/modem/modem_2_tg_n.img),)
    PRODUCT_PACKAGES += modem_2_tg_n.img
    ifeq ($(MTK_MDLOGGER_SUPPORT),yes)
      PRODUCT_PACKAGES += catcher_filter_2_tg_n.bin
    endif
  endif
endif

ifeq ($(strip $(MTK_ISMS_SUPPORT)), yes)
  PRODUCT_PACKAGES += ISmsService
endif


ifeq ($(strip $(MTK_NFC_SUPPORT)), yes)
  PRODUCT_PACKAGES += nfcstackp
  PRODUCT_PACKAGES += DeviceTestApp
  PRODUCT_PACKAGES += libmtknfc_dynamic_load_jni
  PRODUCT_PACKAGES += server_open_nfc
  PRODUCT_PACKAGES += libopen_nfc_client_jni
  PRODUCT_PACKAGES += libopen_nfc_server_jni
  PRODUCT_PACKAGES += libnfc_hal_msr3110
  PRODUCT_PACKAGES += libnfc_msr3110_jni
  PRODUCT_PACKAGES += libnfc_mt6605_jni
  PRODUCT_PACKAGES += libdta_dynamic_load_jni
  PRODUCT_PACKAGES += libdta_msr3110_jni
  PRODUCT_PACKAGES += libdta_mt6605_jni
endif

ifeq ($(strip $(MTK_MTKLOGGER_SUPPORT)), yes)
  PRODUCT_PACKAGES += MTKLogger
endif

ifeq ($(strip $(MTK_SPECIFIC_SM_CAUSE)), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
  ril.specific.sm_cause=1
else
  PRODUCT_PROPERTY_OVERRIDES += \
  ril.specific.sm_cause=0
endif

ifeq ($(strip $(MTK_EMULATOR_SUPPORT)),yes)
  PRODUCT_PACKAGES += SDKGallery
else
  PRODUCT_PACKAGES += Gallery2
endif

ifeq ($(strip $(MTK_BAIDU_MAP_SUPPORT)), yes)
      PRODUCT_PACKAGES += Baidu_Map
endif
ifeq ($(strip $(MTK_BAIDU_SEARCH_BAR_SUPPORT)), yes)
      PRODUCT_PACKAGES += Baidu_Search_Bar
endif

ifneq ($(strip $(MTK_EMULATOR_SUPPORT)),yes)
  PRODUCT_PACKAGES += Provision
endif

ifeq ($(strip $(HAVE_CMMB_FEATURE)), yes)
  PRODUCT_PACKAGES += CMMBPlayer
endif

ifeq ($(strip $(MTK_DATA_TRANSFER_APP)), yes)
  PRODUCT_PACKAGES += DataTransfer
endif

ifeq ($(strip $(MTK_MDM_APP)),yes)
  PRODUCT_PACKAGES += MediatekDM
endif

ifeq ($(strip $(MTK_VT3G324M_SUPPORT)),yes)
  PRODUCT_PACKAGES += libmtk_vt_client \
                      libmtk_vt_em \
                      libmtk_vt_utils \
                      libmtk_vt_service \
                      libmtk_vt_swip \
                      vtservice
endif

ifeq ($(strip $(MTK_OOBE_APP)),yes)
  PRODUCT_PACKAGES += OOBE
endif

ifeq ($(strip $(MTK_MEDIA3D_APP)), yes)
    PRODUCT_PACKAGES += Media3D
endif

ifdef MTK_WEATHER_PROVIDER_APP
  ifneq ($(strip $(MTK_WEATHER_PROVIDER_APP)), no)
    PRODUCT_PACKAGES += MtkWeatherProvider
  endif
endif

ifeq ($(strip $(MTK_VOICE_UNLOCK_SUPPORT)),yes)
    PRODUCT_PACKAGES += VoiceCommand
else
        ifeq ($(strip $(MTK_VOICE_UI_SUPPORT)),yes)
            PRODUCT_PACKAGES += VoiceCommand
        endif
endif

ifeq ($(strip $(MTK_ENABLE_VIDEO_EDITOR)),yes)
  PRODUCT_PACKAGES += VideoEditor
endif

ifeq ($(strip $(MTK_CALENDAR_IMPORTER_APP)), yes)
  PRODUCT_PACKAGES += CalendarImporter
endif

ifeq ($(strip $(MTK_THEMEMANAGER_APP)), yes)
  PRODUCT_PACKAGES += theme-res-mint \
                      theme-res-mocha \
                      theme-res-raspberry \
                      libtinyxml
endif

ifeq ($(strip $(MTK_GALLERY3D_APP)), yes)
  PRODUCT_PACKAGES += Gallery3D
endif

ifeq ($(strip $(MTK_LOG2SERVER_APP)), yes)
  PRODUCT_PACKAGES += Log2Server \
                      Excftpcommonlib \
                      Excactivationlib \
                      Excadditionnallib \
                      Excmaillib

endif

ifeq ($(strip $(MTK_GALLERY_APP)), yes)
  PRODUCT_PACKAGES += Gallery
endif

ifeq ($(strip $(MTK_INPUTMETHOD_PINYINIME_APP)), yes)
  PRODUCT_PACKAGES += PinyinIME
  PRODUCT_PACKAGES += libjni_pinyinime
endif

ifeq ($(strip $(BIRD_IPHONE_PINYINIME)), yes)
  PRODUCT_PACKAGES += PinyinIME
  PRODUCT_PACKAGES += libjni_pinyinime
endif

  PRODUCT_PACKAGES += Camera

ifeq ($(strip $(MTK_VIDEO_FAVORITES_WIDGET_APP)), yes)
  ifneq ($(strip $(MTK_TABLET_PLATFORM)), yes)
    ifneq (,$(filter hdpi xhdpi,$(MTK_PRODUCT_LOCALES)))
      PRODUCT_PACKAGES += VideoFavorites \
                          libjtranscode
    endif
  endif
endif

ifneq (,$(filter km_KH,$(MTK_PRODUCT_LOCALES)))
  PRODUCT_PACKAGES += Mondulkiri.ttf
endif
ifneq (,$(filter my_MM,$(MTK_PRODUCT_LOCALES)))
  PRODUCT_PACKAGES += Padauk.ttf
endif

ifeq ($(strip $(MTK_VIDEOWIDGET_APP)),yes)
  PRODUCT_PACKAGES += MtkVideoWidget
endif

ifeq ($(strip $(MTK_BSP_PACKAGE)),yes)
  PRODUCT_PACKAGES += Stk
else
  PRODUCT_PACKAGES += Stk1
endif

ifeq ($(strip $(MTK_ENGINEERMODE_APP)), yes)
  PRODUCT_PACKAGES += EngineerMode \
                      EngineerModeSim \
                      libem_bt_jni \
                      libem_support_jni \
                      libem_gpio_jni \
                      libem_modem_jni \
                      libem_usb_jni \
                      libem_wifi_jni
  ifeq ($(strip $(MTK_NFC_SUPPORT)), yes)
      PRODUCT_PACKAGES += libem_nfc_jni
  endif
endif

ifeq ($(strip $(MTK_RCSE_SUPPORT)), yes)
    PRODUCT_PACKAGES += Rcse
    PRODUCT_PACKAGES += Provisioning
endif

ifeq ($(strip $(MTK_GPS_SUPPORT)), yes)
  PRODUCT_PACKAGES += YGPS
  PRODUCT_PACKAGES += BGW
  PRODUCT_PROPERTY_OVERRIDES += \
    bgw.current3gband=0
endif

ifeq ($(strip $(MTK_STEREO3D_WALLPAPER_APP)), yes)
  PRODUCT_PACKAGES += Stereo3DWallpaper
endif

ifeq ($(strip $(MTK_DATAREG_APP)),yes)
  PRODUCT_PACKAGES += DataReg
  PRODUCT_PACKAGES += DataRegSecrets
  PRODUCT_PACKAGES += DataRegDefault.properties
endif

ifeq ($(strip $(MTK_GPS_SUPPORT)), yes)
  ifeq ($(strip $(MTK_GPS_CHIP)), MTK_GPS_MT6620)
    PRODUCT_PROPERTY_OVERRIDES += gps.solution.combo.chip=1
  endif
  ifeq ($(strip $(MTK_GPS_CHIP)), MTK_GPS_MT6628)
    PRODUCT_PROPERTY_OVERRIDES += gps.solution.combo.chip=1
  endif
  ifeq ($(strip $(MTK_GPS_CHIP)), MTK_GPS_MT3332)
    PRODUCT_PROPERTY_OVERRIDES += gps.solution.combo.chip=0
  endif
endif

ifeq ($(strip $(MTK_NAND_UBIFS_SUPPORT)),yes)
  PRODUCT_PACKAGES += mkfs_ubifs \
                      ubinize \
            mtdinfo \
         ubiupdatevol \
         ubirmvol \
         ubimkvol \
         ubidetach \
         ubiattach \
         ubinfo \
         ubiformat
endif

ifeq ($(strip $(MTK_EXTERNAL_MODEM_SLOT)),2)
  PRODUCT_PROPERTY_OVERRIDES += \
  ril.external.md=2
else
  ifeq ($(strip $(MTK_EXTERNAL_MODEM_SLOT)),1)
    PRODUCT_PROPERTY_OVERRIDES += \
    ril.external.md=1
  else
    PRODUCT_PROPERTY_OVERRIDES += \
    ril.external.md=0
  endif
endif

ifeq ($(strip $(MTK_LIVEWALLPAPER_APP)), yes)
ifeq ($(strip $(BIRD_LIVEWALLPAPER_APP_FOR_T56A)), yes) 

  PRODUCT_PACKAGES += LiveWallpapers \
                      Galaxy4
else ifeq ($(strip $(BIRD_LIVEWALLPAPER_APP_FOR_T9600C)), yes) 
PRODUCT_PACKAGES += LiveWallpapers \
                      LiveWallpapersPicker \
                      Galaxy4 \
                      HoloSpiralWallpaper
else ifeq ($(strip $(BIRD_LIVEWALLPAPER_APP_FOR_I600)), yes) 
PRODUCT_PACKAGES += LiveWallpapers \
                      LiveWallpapersPicker \
                      Galaxy4 \
                      HoloSpiralWallpaper		      
else ifeq ($(strip $(BIRD_LIVEWALLPAPER_APP_FOR_T9)), yes) 
  PRODUCT_PACKAGES += LiveWallpapers \
                      LiveWallpapersPicker \
                      VisualizationWallpapers \
                      Galaxy4 \
                      HoloSpiralWallpaper
else ifeq ($(strip $(BIRD_LIVEWALLPAPER_APP_FOR_Y05C)), yes) 
  PRODUCT_PACKAGES += LiveWallpapers \
                      LiveWallpapersPicker
else ifeq ($(strip $(BIRD_LIVEWALLPAPER_APP_FOR_K03)), yes) 
  PRODUCT_PACKAGES += LiveWallpapers \
                      LiveWallpapersPicker \
                      VisualizationWallpapers
else
  PRODUCT_PACKAGES += LiveWallpapers \
                      LiveWallpapersPicker \
                      MagicSmokeWallpapers \
                      VisualizationWallpapers \
                      Galaxy4 \
                      HoloSpiralWallpaper \
                      NoiseField \
                      PhaseBeam
endif
endif

ifeq ($(strip $(MTK_VLW_APP)), yes)
  PRODUCT_PACKAGES += MtkVideoLiveWallpaper
endif

ifeq ($(strip $(MTK_SINA_WEIBO_SUPPORT)), yes)
  PRODUCT_PACKAGES += Sina_Weibo
endif

ifeq ($(strip $(MTK_SYSTEM_UPDATE_SUPPORT)), yes)
  PRODUCT_PACKAGES += SystemUpdate \
                      SystemUpdateAssistant
endif

ifeq ($(strip $(MTK_DATADIALOG_APP)), yes)
  PRODUCT_PACKAGES += DataDialog
endif

ifeq ($(strip $(MTK_DATA_TRANSFER_APP)), yes)
  PRODUCT_PACKAGES += DataTransfer
endif

ifeq ($(strip $(MTK_FM_SUPPORT)), yes)
  PRODUCT_PACKAGES += FMRadio
endif

ifeq (MT6620_FM,$(strip $(MTK_FM_CHIP)))
    PRODUCT_PROPERTY_OVERRIDES += \
        fmradio.driver.chip=1
endif

ifeq (MT6626_FM,$(strip $(MTK_FM_CHIP)))
    PRODUCT_PROPERTY_OVERRIDES += \
        fmradio.driver.chip=2
endif

ifeq (MT6628_FM,$(strip $(MTK_FM_CHIP)))
    PRODUCT_PROPERTY_OVERRIDES += \
        fmradio.driver.chip=3
endif

ifeq ($(strip $(MTK_BT_SUPPORT)), yes)
  PRODUCT_PACKAGES += MtkBt \
        libbtcusttable \
        libbtcust \
        libmtkbtextadp \
        libextpbap \
        libextavrcp \
        libextopp \
        libextsys \
        libextftp \
        libmtkbtextadpa2dp \
        libmtka2dp \
        libextbip \
        libextbpp \
        libexthid \
        libextsimap \
        libextjsr82 \
        libmtkbtextpan \
        libextmap \
        libmtkbtextspp \
        libexttestmode \
        libpppbtdun \
        libextopp_jni \
        libexthid_jni \
        libextpan_jni \
        libextftp_jni \
        libextbpp_jni \
        libextbip_jni \
        libextpbap_jni \
        libextavrcp_jni \
        libextsimap_jni \
        libextdun_jni \
        libextmap_jni \
        libextsys_jni \
        btlogmask \
        btconfig \
        libbtpcm \
        libbtsniff \
        mtkbt
    ifeq ($(strip $(MTK_WLANBT_SINGLEANT)), yes)
        PRODUCT_PACKAGES += libwifiwmt
    endif
endif

ifeq ($(strip $(MTK_DT_SUPPORT)),yes)
    ifneq ($(strip $(EVDO_DT_SUPPORT)),yes)
        ifeq ($(strip $(MTK_MDLOGGER_SUPPORT)),yes)
            PRODUCT_PACKAGES += \
                ExtModemLog \
                libextmdlogger_ctrl_jni \
                libextmdlogger_ctrl \
                extmdlogger
        endif
    endif
endif

ifeq ($(strip $(MTK_ENGINEERMODE_APP)), yes)
  PRODUCT_PACKAGES += EngineerMode \
                      MobileLog
endif

ifeq ($(strip $(HAVE_MATV_FEATURE)),yes)
  PRODUCT_PACKAGES += MtvPlayer \
                      MATVEM    \
                      com.mediatek.atv.adapter
endif

ifneq ($(strip $(MTK_LCM_PHYSICAL_ROTATION)),)
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.sf.hwrotation=$(MTK_LCM_PHYSICAL_ROTATION)
endif

ifeq ($(strip $(MTK_SHARE_MODEM_CURRENT)),2)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.current.share_modem=2
else
  ifeq ($(strip $(MTK_SHARE_MODEM_CURRENT)),1)
    PRODUCT_PROPERTY_OVERRIDES += \
      ril.current.share_modem=1
  else
    PRODUCT_PROPERTY_OVERRIDES += \
      ril.current.share_modem=0
  endif
endif

ifeq ($(strip $(MTK_FM_TX_SUPPORT)), yes)
  PRODUCT_PACKAGES += FMTransmitter
endif

ifeq ($(strip $(MTK_SOUNDRECORDER_APP)),yes)
  PRODUCT_PACKAGES += SoundRecorder
endif

ifeq ($(strip $(MTK_DM_APP)),yes)
  PRODUCT_PACKAGES += dm
endif

ifeq ($(strip $(MTK_WEATHER3D_WIDGET)), yes)
  ifneq ($(strip $(MTK_TABLET_PLATFORM)), yes)
    ifneq (,$(filter hdpi xhdpi,$(MTK_PRODUCT_LOCALES)))
      PRODUCT_PACKAGES += Weather3DWidget
    endif
  endif
endif

ifeq ($(strip $(MTK_LAUNCHERPLUS_APP)),yes)
  PRODUCT_PACKAGES += LauncherPlus \
                      MoreApp
  PRODUCT_PROPERTY_OVERRIDES += \
    launcherplus.allappsgrid=2d
endif

ifeq ($(strip $(MTK_LAUNCHER_ALLAPPSGRID)), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
          launcher2.allappsgrid=3d_20
endif

ifeq ($(strip $(MTK_LOCKSCREEN_TYPE)),2)
  PRODUCT_PACKAGES += MtkWallPaper
endif

ifneq ($(strip $(MTK_LOCKSCREEN_TYPE)),)
  PRODUCT_PROPERTY_OVERRIDES += \
    curlockscreen=$(MTK_LOCKSCREEN_TYPE)
endif

ifeq ($(strip $(MTK_IME_SUPPORT)),yes)
  PRODUCT_PACKAGES += MediatekIME
endif

ifeq ($(strip $(MTK_ANDROIDFACTORYMODE_APP)),yes)
  PRODUCT_PACKAGES += AndroidFactoryMode
endif

ifeq ($(strip $(MTK_OMA_DOWNLOAD_SUPPORT)),yes)
  PRODUCT_PACKAGES += Browser \
                      DownloadProvider
endif

ifeq ($(strip $(MTK_OMACP_SUPPORT)),yes)
  PRODUCT_PACKAGES += Omacp
endif
ifeq ($(strip $(MTK_VIDEO_THUMBNAIL_PLAY_SUPPORT)),yes)
  PRODUCT_PACKAGES += libjtranscode
endif
ifeq ($(strip $(MTK_WIFI_P2P_SUPPORT)),yes)
  PRODUCT_PACKAGES += \
    WifiContactSync \
    WifiP2PWizardy \
    FileSharingServer \
    FileSharingClient \
    UPnPAV \
    WifiWsdsrv \
    bonjourExplorer
endif

ifeq ($(strip $(MTK_MDLOGGER_SUPPORT)),yes)
  PRODUCT_PACKAGES += \
    dualmdlogger \
    mdlogger
endif

ifeq ($(strip $(CUSTOM_KERNEL_TOUCHPANEL)),generic)
  PRODUCT_PACKAGES += Calibrator
endif

ifeq ($(strip $(MTK_FILEMANAGER_APP)), yes)
  PRODUCT_PACKAGES += FileManager
endif

ifeq ($(strip $(MTK_ENGINEERMODE_APP)), yes)
  PRODUCT_PACKAGES += ActivityNetwork
endif

ifneq ($(findstring OP03, $(strip $(OPTR_SPEC_SEG_DEF))),)
  PRODUCT_PACKAGES += SimCardAuthenticationService
endif

ifeq ($(strip $(MTK_NFC_SUPPORT)), yes)
  PRODUCT_PACKAGES += NxpSecureElement
endif

ifeq ($(strip $(MTK_NFC_OMAAC_SUPPORT)),yes)
  PRODUCT_PACKAGES += SmartcardService
  PRODUCT_PACKAGES += org.simalliance.openmobileapi
  PRODUCT_PACKAGES += org.simalliance.openmobileapi.xml
  PRODUCT_PACKAGES += libassd
endif

ifeq ($(strip $(MTK_APKINSTALLER_APP)), yes)
  PRODUCT_PACKAGES += APKInstaller
endif

ifeq ($(strip $(MTK_SMSREG_APP)), yes)
  PRODUCT_PACKAGES += SmsReg
endif

ifeq ($(strip $(GEMINI)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.mediatek.gemini_support=true
else
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.mediatek.gemini_support=false
endif

ifeq ($(strip $(MTK_ENGINEERMODE_INTERNAL_APP)), yes)
  PRODUCT_PACKAGES += InternalEngineerMode
endif

ifeq ($(strip $(MTK_STEREO3D_WALLPAPER_APP)), yes)
  PRODUCT_PACKAGES += Stereo3DWallpaper
endif

ifeq ($(strip $(MTK_WEATHER3D_WIDGET)), yes)
    PRODUCT_PACKAGES += Weather3DWidget
endif

ifeq ($(strip $(MTK_YMCAPROP_SUPPORT)),yes)
  PRODUCT_COPY_FILES += mediatek/packages/yahoo_tracking/ymca.properties:system/yahoo/com.yahoo.mobile.client.android.news/ymca.properties
endif

ifeq ($(MTK_BACKUPANDRESTORE_APP),yes)
  PRODUCT_PACKAGES += BackupAndRestore
endif

ifeq ($(strip $(MTK_NOTEBOOK_SUPPORT)),yes)
  PRODUCT_PACKAGES += NoteBook
endif

ifeq ($(strip $(MTK_BWC_SUPPORT)), yes)
    PRODUCT_PACKAGES += libbwc
endif

ifeq ($(strip $(MTK_GPU_SUPPORT)), yes)
   ifeq ($(MTK_PLATFORM),$(filter $(MTK_PLATFORM),MT6572))
       PRODUCT_PACKAGES +=       \
               gralloc.mt6572    \
               libMali           \
               libGLESv1_CM_mali \
               libGLESv2_mali    \
               libEGL_mali
   endif
endif

# Todos is a common feature on JB
PRODUCT_PACKAGES += Todos

ifeq ($(strip $(MTK_DT_SUPPORT)),yes)
  PRODUCT_PACKAGES += ip-up \
                      ip-down \
                      ppp_options \
                      chap-secrets \
                      init.gprs-pppd
endif

ifdef OPTR_SPEC_SEG_DEF
  ifneq ($(strip $(OPTR_SPEC_SEG_DEF)),NONE)
    OPTR := $(word 1,$(subst _,$(space),$(OPTR_SPEC_SEG_DEF)))
    SPEC := $(word 2,$(subst _,$(space),$(OPTR_SPEC_SEG_DEF)))
    SEG  := $(word 3,$(subst _,$(space),$(OPTR_SPEC_SEG_DEF)))
    $(call inherit-product-if-exists, mediatek/operator/$(OPTR)/$(SPEC)/$(SEG)/optr_apk_config.mk)

# Todo:
# obsolete this section's configuration for operator project resource overlay
# once all operator related overlay resource moved to custom folder
    PRODUCT_PACKAGE_OVERLAYS += mediatek/operator/$(OPTR)/$(SPEC)/$(SEG)/OverLayResource
# End

    PRODUCT_PROPERTY_OVERRIDES += \
      ro.operator.optr=$(OPTR) \
      ro.operator.spec=$(SPEC) \
      ro.operator.seg=$(SEG)
  endif
endif

ifeq ($(strip $(GEMINI)), yes)
  ifeq ($(OPTR_SPEC_SEG_DEF),NONE)
    PRODUCT_PACKAGES += StkSelection
  endif
  ifeq (OP01,$(word 1,$(subst _, ,$(OPTR_SPEC_SEG_DEF))))
    PRODUCT_PACKAGES += StkSelection
  endif
  ifndef OPTR_SPEC_SEG_DEF
    PRODUCT_PACKAGES += StkSelection
  endif
endif

ifeq ($(strip $(MTK_DATAREG_APP)),yes)
  PRODUCT_PACKAGES += DataReg
  PRODUCT_PACKAGES += DataRegSecrets
  PRODUCT_PACKAGES += DataRegDefault.properties
endif

ifeq (yes,$(strip $(MTK_FD_SUPPORT)))
# Only support the format: n.m (n:1 or 1+ digits, m:Only 1 digit) or n (n:integer)
    PRODUCT_PROPERTY_OVERRIDES += \
        persist.radio.fd.counter=15

    PRODUCT_PROPERTY_OVERRIDES += \
        persist.radio.fd.off.counter=5

    PRODUCT_PROPERTY_OVERRIDES += \
        persist.radio.fd.r8.counter=15

    PRODUCT_PROPERTY_OVERRIDES += \
        persist.radio.fd.off.r8.counter=5
endif

ifeq ($(strip $(MTK_COMBO_SUPPORT)), yes)
    PRODUCT_PROPERTY_OVERRIDES += persist.mtk.wcn.combo.chipid=-1
endif

ifeq ($(strip $(MTK_WVDRM_SUPPORT)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    drm.service.enabled=true
  PRODUCT_PACKAGES += \
    com.google.widevine.software.drm.xml \
    com.google.widevine.software.drm \
    libdrmwvmplugin \
    libwvm \
    libdrmdecrypt \
    libWVStreamControlAPI_L3 \
    libwvdrm_L3
else
  PRODUCT_PROPERTY_OVERRIDES += \
    drm.service.enabled=false
endif

ifeq ($(strip $(MTK_DRM_APP)),yes)
  PRODUCT_PACKAGES += \
    libdrmmtkplugin \
    drm_chmod \
    libdcfdecoderjni
endif

ifeq (yes,$(strip $(MTK_FM_SUPPORT)))
    PRODUCT_PROPERTY_OVERRIDES += \
        fmradio.driver.enable=1
else
    PRODUCT_PROPERTY_OVERRIDES += \
        fmradio.driver.enable=0
endif

#
# MediaTek resource overlay configuration
#
$(foreach cf,$(RESOURCE_OVERLAY_SUPPORT), \
  $(eval # do NOT modify the overlay resource paths order) \
  $(eval # 1. project level resource overlay) \
  $(eval _project_overlay_dir := $(MTK_ROOT_CUSTOM)/$(TARGET_PRODUCT)/resource_overlay/$(cf)) \
  $(if $(wildcard $(_project_overlay_dir)), \
    $(eval PRODUCT_PACKAGE_OVERLAYS += $(_project_overlay_dir)) \
    , \
   ) \
  $(eval # 2. operator spec. resource overlay) \
  $(eval _operator_overlay_dir := $(MTK_ROOT_CUSTOM)/$(word 1,$(subst _, ,$(OPTR_SPEC_SEG_DEF)))/resource_overlay/$(cf)) \
  $(if $(wildcard $(_operator_overlay_dir)), \
    $(eval PRODUCT_PACKAGE_OVERLAYS += $(_operator_overlay_dir)) \
    , \
   ) \
  $(eval # 3. product line level resource overlay) \
  $(eval _product_line_overlay_dir := $(MTK_ROOT_CUSTOM)/$(PRODUCT)/resource_overlay/$(cf)) \
  $(if $(wildcard $(_product_line_overlay_dir)), \
    $(eval PRODUCT_PACKAGE_OVERLAYS += $(_product_line_overlay_dir)) \
    , \
   ) \
  $(eval # 4. common level(v.s android default) resource overlay) \
  $(eval _common_overlay_dir := $(MTK_ROOT_CUSTOM)/common/resource_overlay/$(cf)) \
  $(if $(wildcard $(_common_overlay_dir)), \
    $(eval PRODUCT_PACKAGE_OVERLAYS += $(_common_overlay_dir)) \
    , \
   ) \
 )

ifeq (yes,$(strip $(MTK_NFC_SUPPORT)))
  PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml \
                        frameworks/base/nfc-extras/com.android.nfc_extras.xml:system/etc/permissions/com.android.nfc_extras.xml
  PRODUCT_PACKAGES += Nfc \
		      Tag \
                      nfcc.default
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.nfc.port=I2C
endif

ifeq ($(strip $(MTK_NFC_SUPPORT)), yes)
  ifeq ($(strip $(MTK_NFC_APP_SUPPORT)), yes)
    PRODUCT_PACKAGES += NFCTagMaster
  endif
endif

ifeq ($(strip $(HAVE_SRSAUDIOEFFECT_FEATURE)),yes)
  PRODUCT_PACKAGES += SRSTruMedia
  PRODUCT_PACKAGES += libsrsprocessing
endif

ifeq ($(strip $(MTK_WEATHER_WIDGET_APP)), yes)
    PRODUCT_PACKAGES += MtkWeatherWidget
endif

ifeq ($(strip $(MTK_WORLD_CLOCK_WIDGET_APP)), yes)
    PRODUCT_PACKAGES += MtkWorldClockWidget
endif

ifeq ($(strip $(MTK_FIRST_MD)),1)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.first.md=1
endif

#factory apk test huangrunming b
ifeq ($(BIRD_FACTORY_POWERON_TEST), yes)
  PRODUCT_PACKAGES += FactoryTest
endif
#factory apk test huangrunming d

ifeq ($(strip $(MTK_FIRST_MD)),2)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.first.md=2
endif

ifeq ($(strip $(MTK_FLIGHT_MODE_POWER_OFF_MD)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.flightmode.poweroffMD=1
else
    PRODUCT_PROPERTY_OVERRIDES += \
      ril.flightmode.poweroffMD=0
endif

ifeq ($(strip $(MTK_FIRST_MD)),1)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.first.md=1
endif
ifeq ($(strip $(MTK_FIRST_MD)),2)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.first.md=2
endif

ifeq ($(strip $(MTK_TELEPHONY_MODE)),0)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.telephony.mode=0
endif
ifeq ($(strip $(MTK_TELEPHONY_MODE)),1)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.telephony.mode=1
endif
ifeq ($(strip $(MTK_TELEPHONY_MODE)),2)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.telephony.mode=2
endif
ifeq ($(strip $(MTK_TELEPHONY_MODE)),3)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.telephony.mode=3
endif
ifeq ($(strip $(MTK_TELEPHONY_MODE)),4)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.telephony.mode=4
endif
ifeq ($(strip $(MTK_TELEPHONY_MODE)),5)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.telephony.mode=5
endif
ifeq ($(strip $(MTK_TELEPHONY_MODE)),6)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.telephony.mode=6
endif
ifeq ($(strip $(MTK_TELEPHONY_MODE)),7)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.telephony.mode=7
endif
ifeq ($(strip $(MTK_TELEPHONY_MODE)),8)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.telephony.mode=8
endif

ifeq ($(strip $(MTK_AGPS_APP)), yes)
  PRODUCT_PACKAGES += LocationEM
  PRODUCT_COPY_FILES += mediatek/frameworks/base/epo/etc/epo_conf.xml:system/etc/epo_conf.xml
  PRODUCT_COPY_FILES += mediatek/frameworks/base/agps/etc/agps_profiles_conf.xml:system/etc/agps_profiles_conf.xml
endif

ifeq (yes,$(strip $(FEATURE_FTM_AUDIO_TEST)))
  PRODUCT_COPY_FILES += mediatek/custom/common/factory/res/sound/testpattern1.wav:system/res/sound/testpattern1.wav
ifeq (yes,$(strip $(FEATURE_FTM_AUDIO_AUTOTEST)))
  PRODUCT_COPY_FILES += mediatek/custom/common/factory/res/sound/ringtone.wav:system/res/sound/ringtone.wav
endif
endif

#for Bird ROM Tool shenzhiwang
ifneq ($(strip $(BIRD_BT_STRING)), )
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.tool.btname=$(BIRD_BT_STRING) 
endif
#for Bird ROM Tool shenzhiwang

#custom camera shutter sound, shenzhiwang
ifdef BIRD_CUSTOM_CAMERA_SHUTTER_SOUND_DEFAULT
    PRODUCT_PROPERTY_OVERRIDES += ro.bird.shutter.sound.default = $(BIRD_CUSTOM_CAMERA_SHUTTER_SOUND_DEFAULT)
endif
#custom camera shutter sound, shenzhiwang

ifneq ($(strip $(BIRD_IPO_ANIM_TIME)), )
  PRODUCT_PROPERTY_OVERRIDES += \
    sys.ipod_3rd_dur=$(BIRD_IPO_ANIM_TIME) 
endif

ifdef BIRD_HARDWARE_VERSION
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.bird.hardware.version=$(BIRD_HARDWARE_VERSION)
endif

PRODUCT_PACKAGES += libsec
PRODUCT_PACKAGES += sbchk
PRODUCT_PACKAGES += S_ANDRO_SFL.ini
PRODUCT_PACKAGES += S_SECRO_SFL.ini

#adupsfota start
ifeq ($(strip $(ADUPS_FOTA_SUPPORT)), yes)
    PRODUCT_PACKAGES += AdupsFota \
                        AdupsFotaReboot
    PRODUCT_COPY_FILES += packages/apps/AdupsFotaApp/fotabinder:system/bin/fotabinder
endif
#adupsfota end
PRODUCT_BRAND := alps
PRODUCT_MANUFACTURER := alps

PRODUCT_COPY_FILES += mediatek/frameworks/base/telephony/etc/apns-conf.xml:system/etc/apns-conf.xml
PRODUCT_COPY_FILES += mediatek/frameworks/base/telephony/etc/spn-conf.xml:system/etc/spn-conf.xml

# for USB Accessory Library/permission
# Mark for early porting in JB
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml
PRODUCT_PACKAGES += com.android.future.usb.accessory

# System property for MediaTek ANR pre-dump.
PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.mtk-stack-trace-file=/data/anr/mtk_traces.txt

ifeq ($(strip $(MTK_WLAN_SUPPORT)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    mediatek.wlan.chip=$(MTK_WLAN_CHIP)

  PRODUCT_PROPERTY_OVERRIDES += \
    mediatek.wlan.module.postfix="_"$(shell echo $(strip $(MTK_WLAN_CHIP)) | tr A-Z a-z)
endif


ifeq ($(strip $(MTK_RADIOOFF_POWER_OFF_MD)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    ril.radiooff.poweroffMD=1
else
    PRODUCT_PROPERTY_OVERRIDES += \
      ril.radiooff.poweroffMD=0
endif

#MT6575/77 MDP Packages
ifeq ($(MTK_PLATFORM),$(filter $(MTK_PLATFORM),MT6575 MT6575T MT6577))
   PRODUCT_PACKAGES += \
      mdpd \
      mdpserver \
      libmhalmdp
endif

ifeq ($(strip $(MTK_TER_SERVICE)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    ter.service.enable=1
else
  PRODUCT_PROPERTY_OVERRIDES += \
    ter.service.enable=0
endif

######add by bird zhuyaozhong 20121207 #####
ifneq ($(strip $(BIRD_PRODUCT_MODEL)), ) 
  PRODUCT_MODEL := $(BIRD_PRODUCT_MODEL)
else
  PRODUCT_MODEL := Android
endif

ifneq ($(strip $(BIRD_PRODUCT_BRAND)), ) 
  PRODUCT_BRAND := $(BIRD_PRODUCT_BRAND)
else
  PRODUCT_BRAND := Android
endif

ifneq ($(strip $(BIRD_PRODUCT_NAME)), ) 
  PRODUCT_NAME := $(BIRD_PRODUCT_NAME)
else
  PRODUCT_NAME := Android
endif

ifneq ($(BIRD_PRODUCT_MANUFACTURER), ) 
  PRODUCT_MANUFACTURER := $(BIRD_PRODUCT_MANUFACTURER)
else
  PRODUCT_MANUFACTURER := BIRDA
endif

# For BIRD_FAKE_JB_VERSION BIRD Wang Lei 121128 ADD BEGIN
ifeq ($(BIRD_FAKE_JB_VERSION), yes)
	PLATFORM_VERSION := 4.2.6
endif
# For BIRD_FAKE_JB_VERSION BIRD Wang Lei 121128 ADD END

ifeq ($(BIRD_FAKE_S999_VERSION), yes)
	PLATFORM_VERSION := 4.2.1
endif

ifeq ($(BIRD_FAKE_S356_VERSION), yes)
	PLATFORM_VERSION := 4.3
endif

#bug[94192]add by meifangting 20130603 begin
ifdef BROWSER_FONTSIZE_SCALE
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.fontsize.scaled=$(BROWSER_FONTSIZE_SCALE)
endif
#add end

#shenzhiwang
ifdef BIRD_GPRS_CONNECTION_ICCID
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.gprs.connection.iccid=$(BIRD_GPRS_CONNECTION_ICCID)
endif
#shenzhiwang

# bug[99599] add by meifangting 20131225
ifeq ($(strip $(BIRD_FOLDER_FAIRYLAND)),yes)  
 PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.fairyland=true
  PRODUCT_PACKAGES += FairyLand
endif

#meifangting add 201201029 for Bug[87242] Intracytoplasmic and outside apk change 
ifeq ($(strip $(BIRD_DATA_APP_SETTING)),yes)  
  PRODUCT_PACKAGES += DataAppSetting \
          del_sysapps
endif
#add end 

#meifangting add @20140219 BIRD_UI_TYPE begin
ifdef BIRD_UI_TYPE
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.ui.type=$(BIRD_UI_TYPE)
endif
#meifangting add @20140219 BIRD_UI_TYPE end

#meifangting add @20120924 for transparent statusbar
ifeq ($(strip $(BIRD_STATUSBAR_TRANSPARENT)),yes)  
  PRODUCT_PROPERTY_OVERRIDES += \
				ro.bird.statusbar.transparent = true
ifdef BIRD_STATUSBAR_ALPHA
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.statusbar.alpha=$(BIRD_STATUSBAR_ALPHA)
endif
ifdef BIRD_STATUSBAR_LOCK0_ALPHA
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.statusbar_lock0.alpha=$(BIRD_STATUSBAR_LOCK0_ALPHA)
endif
endif
#add end

#meifangting add @20130424 for BIRD_MENU_SHOW_WALLPAPER alpha
ifdef BIRD_MENU_SHOW_WALLPAPER_ALPHA
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.menu.alpha=$(BIRD_MENU_SHOW_WALLPAPER_ALPHA)
endif
#add end
#sunjunwei add @20140115 for BIRD_LOCKSCREEN_ALPHA
ifdef BIRD_LOCKSCREEN_ALPHA
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.lockscreen.alpha=$(BIRD_LOCKSCREEN_ALPHA)
endif
#add end

#meifangting add @20120924 for miui statusbar
ifeq ($(strip $(BIRD_STATUSBAR_MIUI)),yes)  
  PRODUCT_PACKAGES += ToolbarSwitches
  PRODUCT_PROPERTY_OVERRIDES += \
				ro.bird.statusbar.miui = true
endif
#add end

#meifangting add @20130329 for doov calculator
ifeq ($(strip $(BIRD_DOOV_CALCULATOR)),yes)  
  PRODUCT_PROPERTY_OVERRIDES += \
				ro.bird.calculator.doovstyle = true
endif
#add end
#meifangting add @20130809 for BIRD_DEFAULT_INSTALL_LOCATION [0-auto,1-internal,2-external]
ifdef BIRD_DEFAULT_INSTALL_LOCATION
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.definstalloc=$(BIRD_DEFAULT_INSTALL_LOCATION)
endif
#add end
#chengting add for doov incall screen photo size @20130508
ifdef BIRD_CONTACT_DISPLAY_PHOTO_SIZE
  PRODUCT_PROPERTY_OVERRIDES += \
    contacts.display_photo_size=$(BIRD_CONTACT_DISPLAY_PHOTO_SIZE)
endif
#add end
#chengting add for doov contact thumbnail photo size @20130508
ifdef BIRD_CONTACT_THUMBNAIL_SIZE
  PRODUCT_PROPERTY_OVERRIDES += \
    contacts.thumbnail_size=$(BIRD_CONTACT_THUMBNAIL_SIZE)
endif
#add end

#bug[97190] add by meifangting 20130911 begin
ifeq ($(strip $(BIRD_10086CN)),yes)  
  PRODUCT_PACKAGES += 10086cn
endif
#bug[97190] add by meifangting 20130911 end

#bug[100372] add by meifangting 20130911 begin
ifeq ($(strip $(BIRD_ICHERRYNET_MENU)),yes)  
  PRODUCT_PACKAGES += ICherryNetMenu
endif
#bug[100372] add by meifangting 20130911 end

#meifangting add 20120910 for HW 12T20-H59
ifeq ($(strip $(BIRD_HOOWMENU)),yes)  
  PRODUCT_PACKAGES += HoowMenu
endif

#dongyaoping add 20140122 for LM
ifeq ($(strip $(BIRD_LOVMEMENU)),yes)  
  PRODUCT_PACKAGES += LovmeMenu
endif
#dongyaoping add 20140122 for DM
ifeq ($(strip $(BIRD_DOWEEMENU)),yes)  
  PRODUCT_PACKAGES += DoweeMenu
endif



ifeq ($(strip $(BIRD_SETTING_SHOW_BRANDWEBSITE)),yes)  
  PRODUCT_PACKAGES += Coca
endif

ifeq ($(strip $(BIRD_YUYI_MENU)),yes)  
  PRODUCT_PACKAGES += YuYiMenu
endif
ifeq ($(strip $(BIRD_LOOBEE_MENU)),yes)  
  PRODUCT_PACKAGES += LoobeeMenu
endif
#add end 

ifeq ($(strip $(BIRD_USERMANUAL)),yes)  
  PRODUCT_PACKAGES += UserManual
  PRODUCT_PROPERTY_OVERRIDES += \
				ro.bird.usermanual = true
endif
#add end 

#bug[06915] add by meifangting 20130910
ifeq ($(strip $(BIRD_HAS_FLIP_CLAM)),yes)  
  PRODUCT_PACKAGES += LeatherLockScreen
endif
#add end 

#meifangting add @20120830 for 240X320 lcd
ifeq ($(strip $(BIRD_240x320_LCD)),yes)  
  PRODUCT_PROPERTY_OVERRIDES += \
				ro.bird.240x320.lcd = true
endif
#add end

#shenyibin add  for BIRD_APPBOX 
ifeq ($(strip $(BIRD_APPBOX)),yes)  
 PRODUCT_PACKAGES += AppBox
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.bird.appbox = true
endif
#add end

#shenyibin add  for BIRD_FARIYLAND 
ifeq ($(strip $(BIRD_FARIYLAND)),yes)  
 PRODUCT_PACKAGES += FairyLand
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.bird.fairyland = true
endif
#add end


#chengting add @2012-07-24 for guest mode
ifeq ($(strip $(BIRD_GUESTMODE)),yes)
  PRODUCT_PACKAGES += GuestModeAPK
  PRODUCT_PROPERTY_OVERRIDES += \
				ro.settings.guestmode_support=true

# launcher  option under guest mode
# BIRD_GUESTMODE_LAUNCHER_SUPPORT = false   will remove the "launcher" option
ifeq ($(strip $(BIRD_GUESTMODE_LAUNCHER_SUPPORT)),false)
  PRODUCT_PROPERTY_OVERRIDES += \
				ro.settings.deskedit=false
endif				
endif
#add end


ifeq ($(strip $(ONEXA_EN_LIVEWALLPAPER_APP)), yes)
  PRODUCT_PACKAGES += LiveWallpapersPicker
endif


ifeq ($(strip $(YX6M_LIVEWALLPAPER_APP)), yes)
  PRODUCT_PACKAGES += LiveWallpapersPicker \
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/YX6M_APK/livewallpaper/LivingWallpaper.apk:data/app/LivingWallpaper.apk
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/YX6M_APK/livewallpaper/LivingWallpaper1.apk:data/app/LivingWallpaper1.apk
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/YX6M_APK/livewallpaper/LivingWallpaper2.apk:data/app/LivingWallpaper2.apk
endif

#  apk install onkey huangrunming 20120703
ifeq (yes,$(strip $(BIRD_APKINSTALL_ONKEY)))
    PRODUCT_PACKAGES += ApkInstallBird
endif

#  apk proximity calibrate huangrunming 20120717
ifeq (yes,$(strip $(BIRD_ENGINEER_PROXIMITY_CALIBRATION)))
    PRODUCT_PACKAGES += ProximityCalibrate
endif

# add by jiali 20120704, for BirdCustTool
ifdef MTK_SPECIAL_FACTORY_RESET
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.tool.specialfactoryreset=$(MTK_SPECIAL_FACTORY_RESET)
endif

#add by chengting 20120829 , for Bird ROM Tool
ifneq ($(strip $(BIRD_BT_STRING)), )
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.tool.btname=$(BIRD_BT_STRING) 
endif

#modify by zhouleigang 20131219begin
ifeq ($(strip $(BIRD_KOOBI_FILE_EXPLORER)),yes)	
    PRODUCT_PACKAGES += FileExplorer
else
ifeq ($(strip $(BIRD_FILE_EXPLORER)),yes)
    PRODUCT_PACKAGES += FileExplorer
endif
endif
#modify by zhouleigang 20131219end

#add by sizhixiang 20140314 begin
ifeq ($(strip $(HW_WIDGET_WEATHER_SUPPORT)),yes)
    PRODUCT_PACKAGES += HwWidgetWeather
endif
ifeq ($(strip $(HW_CONTENT_WIDGET_SUPPORT)),yes)
    PRODUCT_PACKAGES += HwContentWidget
endif
ifeq ($(strip $(HW_DIGITAL_CLOCK_SUPPORT)),yes)
    PRODUCT_PACKAGES += HwDigitalClock 
endif
ifeq ($(strip $(BIRD_HW_LAUNCHER)),yes)
  PRODUCT_PACKAGES := $(filter-out LauncherDoov%, $(PRODUCT_PACKAGES))
  PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))
  PRODUCT_COPY_FILES += \
  packages/apps/Bird_APK/BirdHwLauncher/hwapk/BirdHwLauncher.apk:system/app/BirdHwLauncher.apk
  ifeq ($(strip $(LCM_HEIGHT)),320)
	LCD_DENS=qvga
  endif
  ifeq ($(strip $(LCM_HEIGHT)),400)
	LCD_DENS=wqvga
  endif
  ifeq ($(strip $(LCM_HEIGHT)),480)
	LCD_DENS=hvga
  endif
  ifeq ($(strip $(LCM_HEIGHT)),800)
	LCD_DENS=wvga
  endif
  ifeq ($(strip $(LCM_HEIGHT)),854)
	LCD_DENS=fwvga
  endif
  ifeq ($(strip $(LCM_HEIGHT)),960)
	LCD_DENS=qhd
  endif
  ifeq ($(strip $(LCM_HEIGHT)),1024)
	LCD_DENS=wsvga
  endif
  ifeq ($(strip $(LCM_HEIGHT)),600)
	LCD_DENS=wsvga
  endif
  ifeq ($(strip $(LCM_HEIGHT)),1280)
	LCD_DENS=hd
  endif
  ifeq ($(strip $(LCM_HEIGHT)),1920)
	LCD_DENS=fhd
  endif
      src_files := $(shell ls  packages/apps/Bird_APK/BirdHwLauncher/hw_style_with_thememanage/$(LCD_DENS)/launchersetting/)
      PRODUCT_COPY_FILES += $(foreach file, $(src_files),packages/apps/Bird_APK/BirdHwLauncher/hw_style_with_thememanage/$(LCD_DENS)/launchersetting/$(file):system/launchersetting/$(file))
      src_files := $(shell ls  packages/apps/Bird_APK/BirdHwLauncher/hw_style_with_thememanage/wvga/launchersetting/menu/)
      PRODUCT_COPY_FILES += $(foreach file, $(src_files),packages/apps/Bird_APK/BirdHwLauncher/hw_style_with_thememanage/$(LCD_DENS)/launchersetting/menu/$(file):system/launchersetting/menu/$(file))
endif
#add by sizhixiang 20140314 end

#add by guoliangliang for KoobiWidgetWeather 20140528 begin
ifeq ($(strip $(BIRD_KOOBI_WIDGET_WEATHER)),yes)
    PRODUCT_PACKAGES += KoobiWidgetWeather
endif
#add by guoliangliang for KoobiWidgetWeather 20140528 end

#add by guoliangliang for KoobiWidgetMusic 20140528 begin
ifeq ($(strip $(BIRD_KOOBI_WIDGET_MUSIC)),yes)
    PRODUCT_PACKAGES += KoobiWidgetMusic
endif
#add by guoliangliang for KoobiWidgetMusic 20140528 end

#add by meifangting 20130613 
ifeq ($(strip $(BIRD_3D_LAUNCHER_MOONBASE)),yes)
  PRODUCT_COPY_FILES += \
  packages/apps/Bird_APK/3D_Launcher_MoonBase/lib/libgdx.so:system/lib/libgdx.so \
  packages/apps/Bird_APK/3D_Launcher_MoonBase/lib/libandroidgl20.so:system/lib/libandroidgl20.so \
  packages/apps/Bird_APK/3D_Launcher_MoonBase/lib/libcut.so:system/lib/libcut.so \
  packages/apps/Bird_APK/3D_Launcher_MoonBase/apk/3D_MoonBase.apk:system/app/3D_MoonBase.apk \
  packages/apps/Bird_APK/3D_Launcher_MoonBase/apk/SamWeatherClock_launcher.apk:data/app/SamWeatherClock_launcher.apk     
endif
#add by meifangting 20130724
ifeq ($(strip $(BIRD_3D_GTLAUNCHERGAOKE)),yes)
  PRODUCT_COPY_FILES += \
  packages/apps/Bird_APK/3D_GtLauncherGaoke/GtLauncherGaoke.apk:system/app/GtLauncherGaoke.apk
endif
ifeq ($(strip $(BIRD_3D_ILOONGLAUNCHERS3)),yes)
 PRODUCT_COPY_FILES += \
  packages/apps/Bird_APK/3D_iLoongLauncherS3/lib/libgdx.so:system/lib/libgdx.so \
  packages/apps/Bird_APK/3D_iLoongLauncherS3/lib/libandroidgl20.so:system/lib/libandroidgl20.so \
  packages/apps/Bird_APK/3D_iLoongLauncherS3/lib/libcut.so:system/lib/libcut.so \
  packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/SamWeatherClock.apk:system/app/SamWeatherClock.apk
  
ifeq ($(strip $(BIRD_ILOONGLAUNCHERS3_DEFAULTSTYLE)),miui)
    PRODUCT_COPY_FILES += \
     packages/apps/Bird_APK/3D_iLoongLauncherS3/miui_style/iLoongLauncherS3_BDA.apk:system/app/iLoongLauncherS3_BDA.apk 
     else
     PRODUCT_COPY_FILES += \
     packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/iLoongLauncherS3_BDA.apk:system/app/iLoongLauncherS3_BDA.apk 
endif
endif

ifeq ($(strip $(BIRD_THEMEM_ADDSCENE)),yes)
 PRODUCT_COPY_FILES += \
  packages/apps/Bird_APK/3D_iLoongLauncherS3/no_switch_menu/lib/libcoo.so:system/lib/libcoo.so \
  packages/apps/Bird_APK/3D_iLoongLauncherS3/no_switch_menu/lib/libcoogl20.so:system/lib/libcoogl20.so \
  packages/apps/Bird_APK/3D_iLoongLauncherS3/no_switch_menu/lib/libcut.so:system/lib/libcut.so \
  packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/SamWeatherClock.apk:system/app/SamWeatherClock.apk \
  packages/apps/Bird_APK/3D_iLoongLauncherS3/no_switch_menu/iLoongSceneEngine_BDA.apk:system/app/iLoongSceneEngine_BDA.apk
endif

ifeq ($(BIRD_SCENE_3DSCENE_APK),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/Cooee3DScene.apk:system/app/Cooee3DScene.apk
endif
ifeq ($(BIRD_SCENE_3DSCENE_APK),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/Cooee3DScene.apk:data/app/Cooee3DScene.apk
endif

ifeq ($(BIRD_SCENE_MYSCENE_APK),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeMyScene.apk:system/app/CooeeMyScene.apk
endif
ifeq ($(BIRD_SCENE_MYSCENE_APK),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeMyScene.apk:data/app/CooeeMyScene.apk
endif

ifeq ($(BIRD_SCENE_QIHUAN_APK),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeQiHuanScene.apk:system/app/CooeeQiHuanScene.apk
endif
ifeq ($(BIRD_SCENE_QIHUAN_APK),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeQiHuanScene.apk:data/app/CooeeQiHuanScene.apk
endif

ifeq ($(BIRD_SCENE_THEGILDED_APK),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeTheGilded.apk:system/app/CooeeTheGilded.apk
endif
ifeq ($(BIRD_SCENE_THEGILDED_APK),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeTheGilded.apk:data/app/CooeeTheGilded.apk
endif

ifeq ($(BIRD_SCENE_WIN8_APK),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeWin8.apk:system/app/CooeeWin8.apk
endif
ifeq ($(BIRD_SCENE_WIN8_APK),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeWin8.apk:data/app/CooeeWin8.apk
endif

ifeq ($(BIRD_SCENE_MYROOM_APK),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeMyRoom.apk:system/app/CooeeMyRoom.apk
endif
ifeq ($(BIRD_SCENE_MYROOM_APK),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeMyRoom.apk:data/app/CooeeMyRoom.apk
endif

ifeq ($(BIRD_SCENE_SWZH_APK),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeShouWangZhiHai.apk:system/app/CooeeShouWangZhiHai.apk
endif
ifeq ($(BIRD_SCENE_SWZH_APK),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeShouWangZhiHai.apk:data/app/CooeeShouWangZhiHai.apk
endif

ifeq ($(BIRD_SCENE_XDJJ_APK),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeXianDaiJiaJu.apk:system/app/CooeeXianDaiJiaJu.apk
endif
ifeq ($(BIRD_SCENE_XDJJ_APK),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/3D_iLoongLauncherS3/apk/CooeeXianDaiJiaJu.apk:data/app/CooeeXianDaiJiaJu.apk
endif

#add by dinghongzhen 20130803
ifeq ($(BIRD_APK_CUSTOM_V26E),yes)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/V26E/QQYingYongBao_V26E.apk:data/app/QQYingYongBao_V26E.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/V26E/QQManager_V26E.apk:data/app/QQManager_V26E.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/V26E/QQGmae_V26E.apk:data/app/QQGmae_V26E.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/V26E/QQBrower_V26E.apk:data/app/QQBrower_V26E.apk
endif

#add by meifangting for vivo Musicwidget 20131121
ifeq ($(strip $(BIRD_VIVO_MUSICWIDGET)),yes)
PRODUCT_PACKAGES += TranslucentMusicWidget
endif

#add by meifangting for birdmilauncher 20131201 
ifeq ($(strip $(BIRD_MI_LAUNCHER)),yes)
  PRODUCT_PACKAGES := $(filter-out LauncherDoov%, $(PRODUCT_PACKAGES))
  PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))
  PRODUCT_COPY_FILES += \
  packages/apps/Bird_APK/BirdMiLauncher/apk/BirdMiLauncher.apk:system/app/BirdMiLauncher.apk  
  ifeq ($(strip $(BIRD_MI_LAUNCHER_STYLE)),miui)
     ifdef BIRD_MI_LAUNCHER_LCDDENS
      lcddens=$(BIRD_MI_LAUNCHER_LCDDENS)
      src_files := $(shell ls  packages/apps/Bird_APK/BirdMiLauncher/miui_style_with_thememanage/$(lcddens)/launchersetting/)
      PRODUCT_COPY_FILES += $(foreach file, $(src_files),packages/apps/Bird_APK/BirdMiLauncher/miui_style_with_thememanage/$(lcddens)/launchersetting/$(file):system/launchersetting/$(file))
      src_files := $(shell ls  packages/apps/Bird_APK/BirdMiLauncher/miui_style_with_thememanage/$(lcddens)/launchersetting/menu/)
      PRODUCT_COPY_FILES += $(foreach file, $(src_files),packages/apps/Bird_APK/BirdMiLauncher/miui_style_with_thememanage/$(lcddens)/launchersetting/menu/$(file):system/launchersetting/menu/$(file))
     endif
  else ifeq ($(strip $(BIRD_MI_LAUNCHER_STYLE)),push)
      ifdef BIRD_MI_LAUNCHER_LCDDENS
      lcddens=$(BIRD_MI_LAUNCHER_LCDDENS)
      src_files := $(shell ls  packages/apps/Bird_APK/BirdMiLauncher/vivo_style_with_push/$(lcddens)/launchersetting/)
      PRODUCT_COPY_FILES += $(foreach file, $(src_files),packages/apps/Bird_APK/BirdMiLauncher/vivo_style_with_push/$(lcddens)/launchersetting/$(file):system/launchersetting/$(file))
      src_files := $(shell ls  packages/apps/Bird_APK/BirdMiLauncher/vivo_style_with_push/$(lcddens)/launchersetting/menu/)
      PRODUCT_COPY_FILES += $(foreach file, $(src_files),packages/apps/Bird_APK/BirdMiLauncher/vivo_style_with_push/$(lcddens)/launchersetting/menu/$(file):system/launchersetting/menu/$(file))
     endif
  else
     ifdef BIRD_MI_LAUNCHER_LCDDENS
      lcddens=$(BIRD_MI_LAUNCHER_LCDDENS)
      src_files := $(shell ls  packages/apps/Bird_APK/BirdMiLauncher/vivo_style_with_thememanage/$(lcddens)/launchersetting/)
      PRODUCT_COPY_FILES += $(foreach file, $(src_files),packages/apps/Bird_APK/BirdMiLauncher/vivo_style_with_thememanage/$(lcddens)/launchersetting/$(file):system/launchersetting/$(file))
      src_files := $(shell ls  packages/apps/Bird_APK/BirdMiLauncher/vivo_style_with_thememanage/wvga/launchersetting/menu/)
      PRODUCT_COPY_FILES += $(foreach file, $(src_files),packages/apps/Bird_APK/BirdMiLauncher/vivo_style_with_thememanage/$(lcddens)/launchersetting/menu/$(file):system/launchersetting/menu/$(file))
     endif   
  endif
    ifeq ($(strip $(BIRD_MI_LAUNCHER_MENU_ORIGIN)),yes)
    PRODUCT_COPY_FILES += \
      packages/apps/Bird_APK/BirdMiLauncher/without_thememanage_and_scenedesk/menu/menusetting_en.xml:system/launchersetting/menu/menusetting_en.xml \
      packages/apps/Bird_APK/BirdMiLauncher/without_thememanage_and_scenedesk/menu/menusetting_zh.xml:system/launchersetting/menu/menusetting_zh.xml 
    endif
endif


# facelock, unlock 
ifeq ($(strip $(GOOGLE_FACELOCK)), yes)
PRODUCT_PROPERTY_OVERRIDES += \
          ro.facelock.support=true
PRODUCT_COPY_FILES += \
	vendor/facelock/app/FaceLock.apk:system/app/FaceLock.apk \
	vendor/facelock/lib/libfacelock_jni.so:system/lib/libfacelock_jni.so \
	vendor/facelock/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/left_eye-y0-yi45-p0-pi45-r0-ri20.lg_32/full_model.bin:system/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/left_eye-y0-yi45-p0-pi45-r0-ri20.lg_32/full_model.bin \
	vendor/facelock/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/nose_base-y0-yi45-p0-pi45-r0-ri20.lg_32/full_model.bin:system/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/nose_base-y0-yi45-p0-pi45-r0-ri20.lg_32/full_model.bin \
	vendor/facelock/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/right_eye-y0-yi45-p0-pi45-r0-ri20.lg_32-2/full_model.bin:system/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/right_eye-y0-yi45-p0-pi45-r0-ri20.lg_32-2/full_model.bin \
	vendor/facelock/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-r0-ri30.4a-v24/full_model.bin:system/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-r0-ri30.4a-v24/full_model.bin \
	vendor/facelock/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-rn30-ri30.5-v24/full_model.bin:system/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-rn30-ri30.5-v24/full_model.bin \
	vendor/facelock/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-rp30-ri30.5-v24/full_model.bin:system/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-rp30-ri30.5-v24/full_model.bin \
	vendor/facelock/vendor/pittpatt/models/recognition/face.face.y0-y0-22-b-N/full_model.bin:system/vendor/pittpatt/models/recognition/face.face.y0-y0-22-b-N/full_model.bin       
endif

# --------------GMS4.2.2_NAND begin----------------

ifeq ($(GOOGLE_GMS422_NAND), yes)
PRODUCT_PACKAGES := $(filter-out QuickSearchBox, $(PRODUCT_PACKAGES))
PRODUCT_COPY_FILES += \
vendor/gms422/app/ChromeBookmarksSyncAdapter.apk:system/app/ChromeBookmarksSyncAdapter.apk \
vendor/gms422/app/ConfigUpdater.apk:system/app/ConfigUpdater.apk \
vendor/gms422/app/GmsCore.apk:system/app/GmsCore.apk \
vendor/gms422/app/GoogleBackupTransport.apk:system/app/GoogleBackupTransport.apk \
vendor/gms422/app/GoogleCalendarSyncAdapter.apk:system/app/GoogleCalendarSyncAdapter.apk \
vendor/gms422/app/GoogleContactsSyncAdapter.apk:system/app/GoogleContactsSyncAdapter.apk \
vendor/gms422/app/GoogleEars.apk:system/app/GoogleEars.apk \
vendor/gms422/app/GoogleFeedback.apk:system/app/GoogleFeedback.apk \
vendor/gms422/app/GoogleLoginService.apk:system/app/GoogleLoginService.apk \
vendor/gms422/app/GooglePartnerSetup.apk:system/app/GooglePartnerSetup.apk \
vendor/gms422/app/GoogleServicesFramework.apk:system/app/GoogleServicesFramework.apk \
vendor/gms422/app/LatinImeDictionaryPack.apk:system/app/LatinImeDictionaryPack.apk \
vendor/gms422/app/MediaUploader.apk:system/app/MediaUploader.apk \
vendor/gms422/app/NetworkLocation.apk:system/app/NetworkLocation.apk \
vendor/gms422/app/OneTimeInitializer.apk:system/app/OneTimeInitializer.apk \
vendor/gms422/app/Phonesky.apk:system/app/Phonesky.apk \
vendor/gms422/app/QuickSearchBox.apk:system/app/GoogleQuickSearchBox.apk \
vendor/gms422/app/VoiceSearchStub.apk:system/app/VoiceSearchStub.apk \
vendor/gms422/etc/permissions/com.google.android.maps.xml:system/etc/permissions/com.google.android.maps.xml \
vendor/gms422/etc/permissions/com.google.android.media.effects.xml:system/etc/permissions/com.google.android.media.effects.xml \
vendor/gms422/etc/permissions/com.google.widevine.software.drm.xml:system/etc/permissions/com.google.widevine.software.drm.xml \
vendor/gms422/etc/permissions/features.xml:system/etc/permissions/features.xml \
vendor/gms422/etc/preferred-apps/google.xml:system/etc/preferred-apps/google.xml \
vendor/gms422/etc/g.prop:system/etc/g.prop \
vendor/gms422/framework/com.google.android.maps.jar:system/framework/com.google.android.maps.jar \
vendor/gms422/framework/com.google.android.media.effects.jar:system/framework/com.google.android.media.effects.jar \
vendor/gms422/framework/com.google.widevine.software.drm.jar:system/framework/com.google.widevine.software.drm.jar \
vendor/gms422/lib/libAppDataSearch.so:system/lib/libAppDataSearch.so \
vendor/gms422/lib/libfilterpack_facedetect.so:system/lib/libfilterpack_facedetect.so \
vendor/gms422/lib/libfrsdk.so:system/lib/libfrsdk.so \
vendor/gms422/lib/libgames_rtmp_jni.so:system/lib/libgames_rtmp_jni.so \
vendor/gms422/lib/libgcomm_jni.so:system/lib/libgcomm_jni.so \
vendor/gms422/lib/libgoogle_recognizer_jni.so:system/lib/libgoogle_recognizer_jni.so \
vendor/gms422/lib/libgoogle_recognizer_jni_l.so:system/lib/libgoogle_recognizer_jni_l.so \
vendor/gms422/lib/libgtalk_jni.so:system/lib/libgtalk_jni.so \
vendor/gms422/lib/libgtalk_stabilize.so:system/lib/libgtalk_stabilize.so \
vendor/gms422/lib/libjni_latinime.so:system/lib/libjni_latinime.so \
vendor/gms422/lib/libpatts_engine_jni_api.so:system/lib/libpatts_engine_jni_api.so \
vendor/gms422/lib/libplus_jni_v8.so:system/lib/libplus_jni_v8.so \
vendor/gms422/lib/librs.antblur.so:system/lib/librs.antblur.so \
vendor/gms422/lib/librs.antblur_constant.so:system/lib/librs.antblur_constant.so \
vendor/gms422/lib/librs.antblur_drama.so:system/lib/librs.antblur_drama.so \
vendor/gms422/lib/librs.drama.so:system/lib/librs.drama.so \
vendor/gms422/lib/librs.film_base.so:system/lib/librs.film_base.so \
vendor/gms422/lib/librs.fixedframe.so:system/lib/librs.fixedframe.so \
vendor/gms422/lib/librs.grey.so:system/lib/librs.grey.so \
vendor/gms422/lib/librs.image_wrapper.so:system/lib/librs.image_wrapper.so \
vendor/gms422/lib/librs.retrolux.so:system/lib/librs.retrolux.so \
vendor/gms422/lib/librsjni.so:system/lib/librsjni.so \
vendor/gms422/lib/libRSSupport.so:system/lib/libRSSupport.so \
vendor/gms422/lib/libspeexwrapper.so:system/lib/libspeexwrapper.so \
vendor/gms422/lib/libvcdecoder_jni.so:system/lib/libvcdecoder_jni.so \
vendor/gms422/lib/libvorbisencoder.so:system/lib/libvorbisencoder.so \
vendor/gms422/lib/libwebp_android.so:system/lib/libwebp_android.so
endif


ifeq ($(GOOGLE_GMS422_MAPS), data)
PRODUCT_COPY_FILES += \
  vendor/gms422/app/Maps.apk:data/app/Maps.apk
endif
ifeq ($(GOOGLE_GMS422_YOUTUBE), data)
PRODUCT_COPY_FILES += \
        vendor/gms422/app/YouTube.apk:data/app/YouTube.apk
endif


# --------------GMS4.2.2_NAND end----------------





# --------------GMS4.2.2----------------
ifeq ($(GOOGLE_GMS422), yes)
PRODUCT_PACKAGES := $(filter-out QuickSearchBox, $(PRODUCT_PACKAGES))
PRODUCT_COPY_FILES += \
vendor/gms422/app/ChromeBookmarksSyncAdapter.apk:system/app/ChromeBookmarksSyncAdapter.apk \
vendor/gms422/app/ConfigUpdater.apk:system/app/ConfigUpdater.apk \
vendor/gms422/app/GmsCore.apk:system/app/GmsCore.apk \
vendor/gms422/app/GoogleBackupTransport.apk:system/app/GoogleBackupTransport.apk \
vendor/gms422/app/GoogleCalendarSyncAdapter.apk:system/app/GoogleCalendarSyncAdapter.apk \
vendor/gms422/app/GoogleContactsSyncAdapter.apk:system/app/GoogleContactsSyncAdapter.apk \
vendor/gms422/app/GoogleEars.apk:system/app/GoogleEars.apk \
vendor/gms422/app/GoogleFeedback.apk:system/app/GoogleFeedback.apk \
vendor/gms422/app/GoogleLoginService.apk:system/app/GoogleLoginService.apk \
vendor/gms422/app/GooglePartnerSetup.apk:system/app/GooglePartnerSetup.apk \
vendor/gms422/app/GoogleServicesFramework.apk:system/app/GoogleServicesFramework.apk \
vendor/gms422/app/GoogleTTS.apk:system/app/GoogleTTS.apk \
vendor/gms422/app/LatinImeDictionaryPack.apk:system/app/LatinImeDictionaryPack.apk \
vendor/gms422/app/MediaUploader.apk:system/app/MediaUploader.apk \
vendor/gms422/app/NetworkLocation.apk:system/app/NetworkLocation.apk \
vendor/gms422/app/OneTimeInitializer.apk:system/app/OneTimeInitializer.apk \
vendor/gms422/app/Phonesky.apk:system/app/Phonesky.apk \
vendor/gms422/app/QuickSearchBox.apk:system/app/GoogleQuickSearchBox.apk \
vendor/gms422/app/VoiceSearchStub.apk:system/app/VoiceSearchStub.apk \
vendor/gms422/etc/permissions/com.google.android.maps.xml:system/etc/permissions/com.google.android.maps.xml \
vendor/gms422/etc/permissions/com.google.android.media.effects.xml:system/etc/permissions/com.google.android.media.effects.xml \
vendor/gms422/etc/permissions/com.google.widevine.software.drm.xml:system/etc/permissions/com.google.widevine.software.drm.xml \
vendor/gms422/etc/permissions/features.xml:system/etc/permissions/features.xml \
vendor/gms422/etc/preferred-apps/google.xml:system/etc/preferred-apps/google.xml \
vendor/gms422/etc/g.prop:system/etc/g.prop \
vendor/gms422/framework/com.google.android.maps.jar:system/framework/com.google.android.maps.jar \
vendor/gms422/framework/com.google.android.media.effects.jar:system/framework/com.google.android.media.effects.jar \
vendor/gms422/framework/com.google.widevine.software.drm.jar:system/framework/com.google.widevine.software.drm.jar \
vendor/gms422/lib/libAppDataSearch.so:system/lib/libAppDataSearch.so \
vendor/gms422/lib/libfilterpack_facedetect.so:system/lib/libfilterpack_facedetect.so \
vendor/gms422/lib/libfrsdk.so:system/lib/libfrsdk.so \
vendor/gms422/lib/libgames_rtmp_jni.so:system/lib/libgames_rtmp_jni.so \
vendor/gms422/lib/libgcomm_jni.so:system/lib/libgcomm_jni.so \
vendor/gms422/lib/libgoogle_recognizer_jni.so:system/lib/libgoogle_recognizer_jni.so \
vendor/gms422/lib/libgoogle_recognizer_jni_l.so:system/lib/libgoogle_recognizer_jni_l.so \
vendor/gms422/lib/libgtalk_jni.so:system/lib/libgtalk_jni.so \
vendor/gms422/lib/libgtalk_stabilize.so:system/lib/libgtalk_stabilize.so \
vendor/gms422/lib/libjni_latinime.so:system/lib/libjni_latinime.so \
vendor/gms422/lib/libpatts_engine_jni_api.so:system/lib/libpatts_engine_jni_api.so \
vendor/gms422/lib/libplus_jni_v8.so:system/lib/libplus_jni_v8.so \
vendor/gms422/lib/librs.antblur.so:system/lib/librs.antblur.so \
vendor/gms422/lib/librs.antblur_constant.so:system/lib/librs.antblur_constant.so \
vendor/gms422/lib/librs.antblur_drama.so:system/lib/librs.antblur_drama.so \
vendor/gms422/lib/librs.drama.so:system/lib/librs.drama.so \
vendor/gms422/lib/librs.film_base.so:system/lib/librs.film_base.so \
vendor/gms422/lib/librs.fixedframe.so:system/lib/librs.fixedframe.so \
vendor/gms422/lib/librs.grey.so:system/lib/librs.grey.so \
vendor/gms422/lib/librs.image_wrapper.so:system/lib/librs.image_wrapper.so \
vendor/gms422/lib/librs.retrolux.so:system/lib/librs.retrolux.so \
vendor/gms422/lib/librsjni.so:system/lib/librsjni.so \
vendor/gms422/lib/libRSSupport.so:system/lib/libRSSupport.so \
vendor/gms422/lib/libspeexwrapper.so:system/lib/libspeexwrapper.so \
vendor/gms422/lib/libvcdecoder_jni.so:system/lib/libvcdecoder_jni.so \
vendor/gms422/lib/libvorbisencoder.so:system/lib/libvorbisencoder.so \
vendor/gms422/lib/libwebp_android.so:system/lib/libwebp_android.so \
vendor/gms422/tts/lang_pico/de-DE_gl0_sg.bin:system/tts/lang_pico/de-DE_gl0_sg.bin \
vendor/gms422/tts/lang_pico/de-DE_ta.bin:system/tts/lang_pico/de-DE_ta.bin \
vendor/gms422/tts/lang_pico/es-ES_ta.bin:system/tts/lang_pico/es-ES_ta.bin \
vendor/gms422/tts/lang_pico/es-ES_zl0_sg.bin:system/tts/lang_pico/es-ES_zl0_sg.bin \
vendor/gms422/tts/lang_pico/fr-FR_nk0_sg.bin:system/tts/lang_pico/fr-FR_nk0_sg.bin \
vendor/gms422/tts/lang_pico/fr-FR_ta.bin:system/tts/lang_pico/fr-FR_ta.bin \
vendor/gms422/tts/lang_pico/it-IT_cm0_sg.bin:system/tts/lang_pico/it-IT_cm0_sg.bin \
vendor/gms422/tts/lang_pico/it-IT_ta.bin:system/tts/lang_pico/it-IT_ta.bin \
vendor/gms422/usr/srec/en-US/acoustic_model:system/usr/srec/en-US/acoustic_model \
vendor/gms422/usr/srec/en-US/c_fst:system/usr/srec/en-US/c_fst \
vendor/gms422/usr/srec/en-US/clg:system/usr/srec/en-US/clg \
vendor/gms422/usr/srec/en-US/compile_grammar.config:system/usr/srec/en-US/compile_grammar.config \
vendor/gms422/usr/srec/en-US/contacts.abnf:system/usr/srec/en-US/contacts.abnf \
vendor/gms422/usr/srec/en-US/dict:system/usr/srec/en-US/dict \
vendor/gms422/usr/srec/en-US/dictation.config:system/usr/srec/en-US/dictation.config \
vendor/gms422/usr/srec/en-US/embed_phone_nn_model:system/usr/srec/en-US/embed_phone_nn_model \
vendor/gms422/usr/srec/en-US/embed_phone_nn_state_sym:system/usr/srec/en-US/embed_phone_nn_state_sym \
vendor/gms422/usr/srec/en-US/endpointer_dictation.config:system/usr/srec/en-US/endpointer_dictation.config \
vendor/gms422/usr/srec/en-US/endpointer_voicesearch.config:system/usr/srec/en-US/endpointer_voicesearch.config \
vendor/gms422/usr/srec/en-US/ep_acoustic_model:system/usr/srec/en-US/ep_acoustic_model \
vendor/gms422/usr/srec/en-US/g2p_fst:system/usr/srec/en-US/g2p_fst \
vendor/gms422/usr/srec/en-US/google_hotword.config:system/usr/srec/en-US/google_hotword.config \
vendor/gms422/usr/srec/en-US/google_hotword_clg:system/usr/srec/en-US/google_hotword_clg \
vendor/gms422/usr/srec/en-US/google_hotword_logistic:system/usr/srec/en-US/google_hotword_logistic \
vendor/gms422/usr/srec/en-US/grammar.config:system/usr/srec/en-US/grammar.config \
vendor/gms422/usr/srec/en-US/hmmsyms:system/usr/srec/en-US/hmmsyms \
vendor/gms422/usr/srec/en-US/hotword_symbols:system/usr/srec/en-US/hotword_symbols \
vendor/gms422/usr/srec/en-US/lintrans_model:system/usr/srec/en-US/lintrans_model \
vendor/gms422/usr/srec/en-US/metadata:system/usr/srec/en-US/metadata \
vendor/gms422/usr/srec/en-US/norm_fst:system/usr/srec/en-US/norm_fst \
vendor/gms422/usr/srec/en-US/normalizer:system/usr/srec/en-US/normalizer \
vendor/gms422/usr/srec/en-US/offensive_word_normalizer:system/usr/srec/en-US/offensive_word_normalizer \
vendor/gms422/usr/srec/en-US/phonelist:system/usr/srec/en-US/phonelist \
vendor/gms422/usr/srec/en-US/rescoring_lm:system/usr/srec/en-US/rescoring_lm \
vendor/gms422/usr/srec/en-US/symbols:system/usr/srec/en-US/symbols
endif
	
ifeq ($(GOOGLE_GMS422_GMAIL), yes)
PRODUCT_COPY_FILES += \
	vendor/gms422/app/Gmail.apk:system/app/Gmail.apk
endif

ifeq ($(GOOGLE_GMS422_TALK), yes)
PRODUCT_COPY_FILES += \
	vendor/gms422/app/Talk.apk:system/app/Talk.apk
endif

ifeq ($(GOOGLE_GMS422_MAPS), yes)
PRODUCT_COPY_FILES += \
  vendor/gms422/app/Maps.apk:system/app/Maps.apk
endif

ifeq ($(GOOGLE_GMS422_YOUTUBE), yes)
PRODUCT_COPY_FILES += \
        vendor/gms422/app/YouTube.apk:system/app/YouTube.apk
endif

ifeq ($(GOOGLE_GMS422_SETUPWIZED), yes)
PRODUCT_COPY_FILES += \
	vendor/gms422/app/SetupWizard.apk:system/app/SetupWizard.apk
endif

ifeq ($(GOOGLE_GMS422_GOOGLEPLUS), yes)
PRODUCT_COPY_FILES += \
  vendor/gms422/app/GooglePlus.apk:system/app/GooglePlus.apk
endif

# facelock, unlock 
ifeq ($(strip $(GOOGLE_FACELOCK422)), yes)
PRODUCT_PROPERTY_OVERRIDES += \
          ro.facelock.support=true
PRODUCT_COPY_FILES += \
	vendor/facelock422/app/FaceLock.apk:system/app/FaceLock.apk \
	vendor/facelock422/lib/libfacelock_jni.so:system/lib/libfacelock_jni.so \
	vendor/facelock422/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/left_eye-y0-yi45-p0-pi45-r0-ri20.lg_32/full_model.bin:system/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/left_eye-y0-yi45-p0-pi45-r0-ri20.lg_32/full_model.bin \
	vendor/facelock422/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/nose_base-y0-yi45-p0-pi45-r0-ri20.lg_32/full_model.bin:system/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/nose_base-y0-yi45-p0-pi45-r0-ri20.lg_32/full_model.bin \
	vendor/facelock422/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/right_eye-y0-yi45-p0-pi45-r0-ri20.lg_32-2/full_model.bin:system/vendor/pittpatt/models/detection/multi_pose_face_landmark_detectors.7/right_eye-y0-yi45-p0-pi45-r0-ri20.lg_32-2/full_model.bin \
	vendor/facelock422/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-r0-ri30.4a-v24/full_model.bin:system/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-r0-ri30.4a-v24/full_model.bin \
	vendor/facelock422/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-rn30-ri30.5-v24/full_model.bin:system/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-rn30-ri30.5-v24/full_model.bin \
	vendor/facelock422/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-rp30-ri30.5-v24/full_model.bin:system/vendor/pittpatt/models/detection/yaw_roll_face_detectors.6/head-y0-yi45-p0-pi45-rp30-ri30.5-v24/full_model.bin \
	vendor/facelock422/vendor/pittpatt/models/recognition/face.face.y0-y0-22-b-N/full_model.bin:system/vendor/pittpatt/models/recognition/face.face.y0-y0-22-b-N/full_model.bin       
endif

#-----------------------------------------------------------------------------------------------

ifeq ($(BIRD_APK_AGILELOCK), yes)
  PRODUCT_PACKAGES += AgileLock
endif
#-----------------------------------------------------------------------------------------------
#apk

ifeq ($(strip $(BIRD_TORCH)), yes)
  PRODUCT_PACKAGES += Torch
endif

ifeq ($(strip $(BIRD_DISPLAY_SAR_VALUE_FOR_MAXX)), yes)
  PRODUCT_PACKAGES += SAR_Test
endif

ifeq ($(BIRD_DEVICE_TEST), yes)
  PRODUCT_PACKAGES += DEVICE_TEST
endif


ifeq ($(BIRD_MMI_AUTO_TEST), yes)
  PRODUCT_PACKAGES += MMITest
endif

#ifeq ($(BIRD_WRITE_IMEI), yes)
  PRODUCT_PACKAGES += WriteIMEIApp
#endif

ifeq ($(BIRD_APK_FENGHUANGMICROBLOG), yes)
  PRODUCT_PACKAGES += apkFenghuangMicroblog
endif
ifeq ($(BIRD_APK_SINAMICROBLOG), yes)
  PRODUCT_PACKAGES += apkSinaMicroblog
  PRODUCT_COPY_FILES += vendor/apklib/libutility.so:system/lib/libutility.so
endif
ifeq ($(BIRD_APK_TENCENTMICROBLOG), yes)
  PRODUCT_PACKAGES += apkTencentMicroblog
  PRODUCT_COPY_FILES += vendor/apklib/libcodecwrapper.so:system/lib/libcodecwrapper.so
  PRODUCT_COPY_FILES += vendor/apklib/libmsfboot.so:system/lib/libmsfboot.so
  PRODUCT_COPY_FILES += vendor/apklib/libpingyin.so:system/lib/libpingyin.so
endif
ifeq ($(BIRD_APK_OTIUNLAUNCHER), yes)
  PRODUCT_PACKAGES += OtiumTaskManager
endif
ifeq ($(BIRD_APK_ABLACK), yes)
  PRODUCT_PACKAGES += Otium_Ablack
endif
ifeq ($(BIRD_APK_OTIUNLAUNCHER), yes)
  PRODUCT_PACKAGES +=OTIUNlancher
endif
# For BIRD_MOBILE_MANAGER BIRD Wang Lei 110826 ADD BEGIN
ifeq ($(BIRD_MOBILE_MANAGER), yes)
  PRODUCT_PACKAGES += MobileManager
endif
# For BIRD_MOBILE_MANAGER BIRD Wang Lei 110826 ADD END

# For BIRD_MOBILE_MANAGER_INCOMINGCALLSHOW BIRD Wang Lei 110826 ADD BEGIN
ifeq ($(BIRD_MOBILE_MANAGER_INCOMINGCALLSHOW), yes)
  PRODUCT_PACKAGES += IncomingCallShow
endif
# For BIRD_MOBILE_MANAGER_INCOMINGCALLSHOW BIRD Wang Lei 110826 ADD END

# For BIRD_MOBILE_MANAGER_INCOMINGCALLSHOW BIRD Wang Lei 110826 ADD BEGIN
ifeq ($(BIRD_MOBILE_MANAGER_VIP), yes)
  PRODUCT_PACKAGES += Vip
endif
# For BIRD_MOBILE_MANAGER_INCOMINGCALLSHOW BIRD Wang Lei 110826 ADD END

# For BIRD_MOBILE_MANAGER BIRD Wang Lei 111129 ADD BEGIN
ifeq ($(BIRD_MOBILE_MANAGER_BLOCKRECORD), yes)
  PRODUCT_PACKAGES += BlockRecord
endif

ifeq ($(BIRD_MOBILE_MANAGER_APPENCRYPTED), yes)
  PRODUCT_PACKAGES += AppEncrypted
endif

ifeq ($(BIRD_MOBILE_MANAGER_ICSAPP), yes)
  PRODUCT_PACKAGES += ICSApp
endif
# For BIRD_MOBILE_MANAGER BIRD Wang Lei 111129 ADD END

ifeq ($(BIRD_SAGETEL_APK), yes)
  PRODUCT_PACKAGES += Sagetel
endif

ifeq ($(BIRD_BIRDAPP), yes)
  PRODUCT_PACKAGES += BirdApp
endif

ifeq ($(BIRD_EWC), yes)
  PRODUCT_PACKAGES += EWC
endif

ifeq ($(BIRD_EWC_TIANYU), yes)
  PRODUCT_PACKAGES += EWC_TIANYU
endif

ifeq ($(BIRD_EWC_RUIJI), yes)
  PRODUCT_PACKAGES += EWC_RUIJI
endif
ifeq ($(BIRD_EWC_ISMAR), yes)
  PRODUCT_PACKAGES += EWC_ISMAR
endif

ifeq ($(BIRD_YINGMAI_APK), yes)
  PRODUCT_PACKAGES += YingmaiService
endif

ifeq ($(BIRD_APK_FANCYWIDGET), yes)
  PRODUCT_PACKAGES += FancyWidget
endif

ifeq ($(BIRD_EWC_XBF_X920), yes)
  PRODUCT_PACKAGES += EWC_XBF_X920
endif
ifeq ($(BIRD_EWC_A99), yes)
  PRODUCT_PACKAGES += EWC_A99
endif

ifeq ($(BIRD_EWC_SD), yes)
  PRODUCT_PACKAGES += EWC_SD
endif

ifeq ($(strip $(BIRD_EWC_IMEI)), yes)
  PRODUCT_PACKAGES += EWC_IMEI
endif

ifeq ($(BIRD_PORTABLE_HOTPOT), yes)
  PRODUCT_PACKAGES += wlan
endif

ifeq ($(BIRD_POWERMASTER), yes)
  PRODUCT_PACKAGES += PowerMaster
endif

ifeq ($(BIRD_IPHONE_WALLPAPER), yes)
  PRODUCT_PACKAGES += IPhoneWallpaper
endif

ifeq ($(BIRD_LAUNCHERCCPO_APP), yes)
  PRODUCT_PACKAGES += LauncherCCPO
  PRODUCT_PROPERTY_OVERRIDES += curlockscreen=1
endif

ifeq ($(strip $(BIRD_DISPLAY_VERSION_FOR_SOLONE)), yes)
  PRODUCT_PACKAGES += MobileVersion
endif

ifneq ($(strip $(BIRD_LAUNCHER2_ALLAPPSGRID_TYPE)), )
  PRODUCT_PROPERTY_OVERRIDES += \
          launcher2.allappsgrid=$(BIRD_LAUNCHER2_ALLAPPSGRID_TYPE)
endif

# For BIRD_DEFAULT_CONTACTS BIRD Wang Lei 120222 ADD BEGIN
ifeq ($(BIRD_DEFAULT_CONTACTS), yes)
  PRODUCT_PACKAGES += DefaultContacts
endif
# For BIRD_DEFAULT_CONTACTS BIRD Wang Lei 120222 ADD END

ifeq ($(strip $(BIRD_LAUNCHER_APP)), yes)
  PRODUCT_PACKAGES += LauncherBird
endif

ifeq ($(strip $(BIRD_LAUNCHER_OPEN)), yes)
  PRODUCT_PACKAGES += LauncherOpen \
                      MtkWorldClockWidget \
                      MtkWallPaper
endif

#lvhuaiyi add for hyundai launcher
ifeq ($(strip $(BIRD_LAUNCHER_HYUNDAI)),yes)
  PRODUCT_PACKAGES += LauncherHyundai
endif

ifeq ($(strip $(BIRD_IPHONE_LAUNCHER)), yes)
  PRODUCT_PACKAGES += LauncherIphone
endif

ifeq ($(strip $(BIRD_LAUNCHER_STYLE)), yes)
  PRODUCT_PACKAGES += LauncherStyle
endif

ifeq ($(strip $(BIRD_WLD_STYLE)), yes)
  PRODUCT_PACKAGES += LauncherWLD
  PRODUCT_PACKAGES += AlbumWidget
  PRODUCT_COPY_FILES += packages/apps/WanLiDa/AlbumWidget.apk:system/app/AlbumWidget.apk
  PRODUCT_COPY_FILES += packages/apps/WanLiDa/MessageWidget.apk:system/app/MessageWidget.apk
endif
ifeq ($(strip $(BIRD_WLD_STYLE_WEATHER)), yes)
  PRODUCT_PACKAGES += WeatherWidget
endif
ifeq ($(strip $(BIRD_WLD_SERVICE)), yes)
  PRODUCT_PACKAGES += WanLiDaService
endif
ifeq ($(strip $(BIRD_LAUNCHER_CJ)), yes)
  PRODUCT_PACKAGES += LauncherCJ
endif
ifeq ($(strip $(BIRD_DOOV_STYLE)),yes) 
  PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))
  ifneq ($(strip $(BIRD_MI_LAUNCHER)),yes)
    ifneq ($(strip $(BIRD_HW_LAUNCHER)),yes)
  PRODUCT_PACKAGES += LauncherDoov
    endif
  endif
  PRODUCT_PACKAGES += ThemeApkManager
  # PRODUCT_PACKAGES += ThemeElegant
  PRODUCT_PACKAGES += ThemeLovely
  PRODUCT_PACKAGES += ThemePaper
  PRODUCT_PACKAGES += ThemeBinfen
  PRODUCT_PACKAGES += ThemeDefault
  PRODUCT_PACKAGES += ThemeRice2
  PRODUCT_PACKAGES += ThemeVivo
  # PRODUCT_PACKAGES += ThemeBeautiful
  # PRODUCT_PACKAGES += ThemeBright
  PRODUCT_PACKAGES += QuickSearchBox
endif

# shenyibin add for LauncherDoov @20130903
ifeq ($(strip $(BIRD_LOGO_MYVIDEO)),yes) 
  PRODUCT_PACKAGES += MyVideo
  PRODUCT_COPY_FILES += \
	bird/sounds/media/my_video.mp4:system/media/my_video.mp4
endif
# shenyibin add for LauncherDoov @20130909
ifeq ($(strip $(BIRD_LOGO_MYVIDEO_APK)),yes) 
  PRODUCT_COPY_FILES += packages/apps/MyVideo/MyVideo.apk:system/app/MyVideo.apk
endif

# shenyibin add theme  @20130905
ifeq ($(strip $(BIRD_THEME_ELEGANT)),yes) 
  PRODUCT_PACKAGES += ThemeElegant
endif
# shenyibin add theme  @20130905
ifeq ($(strip $(BIRD_THEME_CRYSTAL)),yes) 
  PRODUCT_PACKAGES += ThemeCrystal
endif
# shenyibin add theme  @20130905
ifeq ($(strip $(BIRD_THEME_HALO)),yes) 
  PRODUCT_PACKAGES += ThemeHalo
endif
# shenyibin add theme  @20130905
ifeq ($(strip $(BIRD_THEME_PURE)),yes) 
  PRODUCT_PACKAGES += ThemePure
endif
# shenyibin add theme  @20130905
ifeq ($(strip $(BIRD_THEME_TASTE)),yes) 
  PRODUCT_PACKAGES += ThemeTaste
endif

# shenyibin add for LauncherDoov @20130822
ifeq ($(strip $(BIRD_LAUNCHER_DOOV)),yes) 
  PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))
  PRODUCT_PACKAGES += LauncherDoov
endif

# shenyibin add for ThemeApkManager @20130822
ifeq ($(strip $(BIRD_THEME_APK_MANAGER)),yes) 
  PRODUCT_PACKAGES += ThemeApkManager
endif

# shenyibin add for LauncherBerry @20130820
ifeq ($(strip $(BIRD_LUANCHER_BERRY)),yes) 
  PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))
  PRODUCT_PACKAGES += LauncherBerry
endif

# shenyibin add for Themerice2 @20130822
ifeq ($(strip $(BIRD_THEMERICE2)),yes) 
  PRODUCT_PACKAGES += ThemeRice2
endif

# shenyibin add for ThemeFate @20130829
ifeq ($(strip $(BIRD_THEME_FATE)),yes) 
  PRODUCT_PACKAGES += ThemeFate
endif

# shenyibin add for ThemeElegant @20130829
ifeq ($(strip $(BIRD_THEME_ELEGANT)),yes) 
  PRODUCT_PACKAGES += ThemeElegant
endif


# sunjunwei add for ThemeGFIVE @20140226
ifeq ($(strip $(BIRD_THEME_GFIVE)),yes) 
  PRODUCT_PACKAGES += ThemeGFIVE
endif

# shenyibin add for ThemeDoovDefault @20130822
ifeq ($(strip $(BIRD_THEMEDOOVDEFAULT)),yes) 
  PRODUCT_PACKAGES += ThemeDoovDefault
endif

# shenyibin add for ThemeDefault @20130822
ifeq ($(strip $(BIRD_THEMEDEFAULT)),yes) 
  PRODUCT_PACKAGES += ThemeDefault
endif

ifeq ($(strip $(BIRD_THEME_NOVELTY)),yes) 
	PRODUCT_PACKAGES += ThemeNovelty
endif
ifeq ($(strip $(BIRD_THEME_GOOGLE)),yes) 
	PRODUCT_PACKAGES += ThemeGoogle
endif
ifeq ($(strip $(BIRD_THEME_SKK)),yes) 
	PRODUCT_PACKAGES += ThemeSkk
endif
ifeq ($(strip $(BIRD_THEME_BEAUTIFUL)),yes) 
  PRODUCT_PACKAGES += ThemeBeautiful
endif
ifeq ($(strip $(BIRD_THEME_BRIGHT)),yes) 
  PRODUCT_PACKAGES += ThemeBright
endif
ifeq ($(strip $(BIRD_THEME_GXQ)),yes) 
  PRODUCT_PACKAGES += ThemeGXQ
endif
ifeq ($(strip $(BIRD_THEME_VIVO)),yes) 
  PRODUCT_PACKAGES += ThemeVivo
endif
ifeq ($(strip $(BIRD_THEME_SAMSUNG)),yes) 
  PRODUCT_PACKAGES += ThemeSAMSUNG
endif
ifeq ($(strip $(BIRD_THEME_PYJ)),yes) 
  PRODUCT_PACKAGES += ThemeIPH
endif
ifeq ($(strip $(BIRD_THEME_LJFH)),yes) 
  PRODUCT_PACKAGES += ThemeLJFH
endif

ifeq ($(strip $(BIRD_THEME_DANCAI)),yes) 
  PRODUCT_PACKAGES += ThemeDANCAI
endif

ifeq ($(strip $(BIRD_THEME_COLOROS)),yes) 
  PRODUCT_PACKAGES += ThemeColorOs
endif

ifeq ($(strip $(BIRD_THEME_ENERGY)),yes) 
  PRODUCT_PACKAGES += ThemeENERGY
endif

ifeq ($(strip $(BIRD_THEME_OPPO)),yes) 
  PRODUCT_PACKAGES += ThemeOPPO
endif
ifeq ($(strip $(BIRD_CUSTOM_BG)),yes) 
  PRODUCT_PACKAGES += bgconfig
endif

# shenyibin add for LauncherDoov init theme package name
ifneq ($(strip $(THEME_DEFAULT_PACKAGE_NAME)), )
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.theme_package_name=$(THEME_DEFAULT_PACKAGE_NAME)
endif

#[95927],Camera365,chengting,@20130615,begin
ifeq ($(strip $(BIRD_DOOV_Camera365)),yes) 
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/Camera365_for_Doov/apk/Camera365_for_Doov.apk:system/app/Camera365_for_Doov.apk
  
  	src_file := packages/apps/Bird_APK/DOOV_APK/Camera365_for_Doov/extra/2d_scene/095A4A2C-0031E4B3-00000137-E90179E7
	des_file := system/arcsoft/camerahawk/data/2d_scene/095A4A2C-0031E4B3-00000137-E90179E7
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/DOOV_APK/Camera365_for_Doov/extra/2d_scene/0B31A832-0031E4B3-00000137-E9BFF24C
	des_file := system/arcsoft/camerahawk/data/2d_scene/0B31A832-0031E4B3-00000137-E9BFF24C
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/DOOV_APK/Camera365_for_Doov/extra/2d_scene/21A3A1DC-0031E4B3-00000137-BBAB1C02
	des_file := system/arcsoft/camerahawk/data/2d_scene/21A3A1DC-0031E4B3-00000137-BBAB1C02
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/DOOV_APK/Camera365_for_Doov/extra/2d_scene/47665724-0031E4B3-00000137-E97B9094
	des_file := system/arcsoft/camerahawk/data/2d_scene/47665724-0031E4B3-00000137-E97B9094
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/DOOV_APK/Camera365_for_Doov/extra/2d_scene/6A5443B0-0031E4B3-00000137-C4FEF1CD
	des_file := system/arcsoft/camerahawk/data/2d_scene/6A5443B0-0031E4B3-00000137-C4FEF1CD
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/DOOV_APK/Camera365_for_Doov/extra/2d_scene/8CBABF1C-0031E4B3-00000137-BBBD86A4
	des_file := system/arcsoft/camerahawk/data/2d_scene/8CBABF1C-0031E4B3-00000137-BBBD86A4
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/DOOV_APK/Camera365_for_Doov/extra/2d_scene/98C0B868-0031E4B3-00000137-EDFCB86F
	des_file := system/arcsoft/camerahawk/data/2d_scene/98C0B868-0031E4B3-00000137-EDFCB86F
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/DOOV_APK/Camera365_for_Doov/extra/2d_scene/F7D1DC5C-0031E4B3-00000137-BBCB38D1
	des_file := system/arcsoft/camerahawk/data/2d_scene/F7D1DC5C-0031E4B3-00000137-BBCB38D1
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
		
	src_file := packages/apps/Bird_APK/DOOV_APK/Camera365_for_Doov/so
	des_file := system/arcsoft/camerahawk/libs
		src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
        $(src_file)/$(file):$(des_file)/$(file))
        
  	src_file := packages/apps/Bird_APK/DOOV_APK/Camera365_for_Doov/extra/2d_scene/F9BAB1EC-0031E4B3-00000137-EA096658
	des_file := system/arcsoft/camerahawk/data/2d_scene/F9BAB1EC-0031E4B3-00000137-EA096658
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
		
endif
#[95927],Camera365,chengting,@20130720,end

# For BIRD_PROCESS_MANAGER BIRD Wang Lei 120629 ADD BEGIN
ifeq ($(strip $(BIRD_PROCESS_MANAGER)), yes)
	PRODUCT_COPY_FILES += packages/apps/ProcessManager/ProcessManager.apk:system/app/ProcessManager.apk
endif


ifeq ($(strip $(BIRD_MEMORY_CLEAN_UP)), yes)
	PRODUCT_COPY_FILES += packages/apps/MemoryCleanUp/MemoryCleanUp.apk:system/app/MemoryCleanUp.apk
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.real_cpu_number = 1
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.cpu_number_times = 2
endif

ifeq ($(strip $(BIRD_MEMORY_CLEAN_UP_CHEAT_QUAD)), yes)
	PRODUCT_COPY_FILES += packages/apps/MemoryCleanUp/MemoryCleanUp.apk:system/app/MemoryCleanUp.apk
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.real_cpu_number = 2
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.cpu_number_times = 2
endif

ifeq ($(strip $(BIRD_MEMORY_CLEAN_UP_EIGHT_CPU)), yes)
	PRODUCT_COPY_FILES += packages/apps/MemoryCleanUp/MemoryCleanUpEightCpu.apk:system/app/MemoryCleanUp.apk
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.real_cpu_number = 2
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.cpu_number_times = 4
endif

#add start by shenzhiwang 
ifdef BIRD_FAKE_CPU_NUMBER
    PRODUCT_PROPERTY_OVERRIDES += ro.bird.fake.cpunumber = $(BIRD_FAKE_CPU_NUMBER)
endif


#add start by shenzhiwang 
#kernel version
ifeq ($(BIRD_KERNEL_VERSION), yes)
ifdef BIRD_KERNEL_VERSION_STR
    PRODUCT_PROPERTY_OVERRIDES += ro.bird.kernel.version = $(BIRD_KERNEL_VERSION_STR)
endif
endif

#baseband version
ifeq ($(BIRD_BASEBAND_VERSION), yes)
ifdef BIRD_BASEBAND_VERSION_STR
    PRODUCT_PROPERTY_OVERRIDES += ro.bird.baseband.version = $(BIRD_BASEBAND_VERSION_STR)
endif
endif
#add end by shenzhiwang

ifeq ($(BIRD_CPU_FREQUENCY1), yes)
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.cpu_frequency = 1.0GHz
endif

ifeq ($(BIRD_CPU_FREQUENCY12), yes)
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.cpu_frequency = 1.2GHz
endif

ifeq ($(BIRD_CPU_FREQUENCY13), yes)
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.cpu_frequency = 1.3GHz
endif

ifeq ($(BIRD_CPU_FREQUENCY15), yes)
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.cpu_frequency = 1.5GHz
endif

ifeq ($(BIRD_CPU_FREQUENCY17), yes)
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.cpu_frequency = 1.7GHz
endif

ifdef BIRD_REAL_CPU_NUMBER
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.real_cpu_number = $(BIRD_REAL_CPU_NUMBER)
endif

ifdef BIRD_CPU_NUMBER_TIMES
	PRODUCT_PROPERTY_OVERRIDES += ro.bird.cpu_number_times = $(BIRD_CPU_NUMBER_TIMES)
endif
# For BIRD_PROCESS_MANAGER BIRD Wang Lei 120629 ADD END

#add start by shenzhiwang 
PRODUCT_PROPERTY_OVERRIDES += ro.birda.tag = birda

ifeq ($(strip $(BIRD_IPHONE_MUSICPLAYER)), yes)
  PRODUCT_PACKAGES += MusicPlayer
endif

# For BIRD_ACCESS_CONTROL BIRD Wang Lei 120730 ADD BEGIN
ifeq ($(BIRD_ACCESS_CONTROL), yes)
  PRODUCT_PACKAGES += AccessControl
endif
# For BIRD_ACCESS_CONTROL BIRD Wang Lei 120730 ADD END

# For BIRD_ASSISTIVE_TOUCH BIRD Wang Lei 121211 ADD BEGIN
ifeq ($(BIRD_ASSISTIVE_TOUCH), yes)
  PRODUCT_PACKAGES += AssistiveTouch
endif
# For BIRD_ASSISTIVE_TOUCH BIRD Wang Lei 121211 ADD END

# For BIRD_GLOWPADVIEW BIRD Wang Lei 130321 ADD BEGIN
ifeq ($(BIRD_GLOWPADVIEW), yes)
  PRODUCT_PACKAGES += BirdGlowPadViewSettings
endif
# For BIRD_GLOWPADVIEW BIRD Wang Lei 130321 ADD END

# For BIRD_FESTIVAL_WALLPAPER BIRD Wang Lei 130401 ADD BEGIN
ifeq ($(BIRD_FESTIVAL_WALLPAPER), yes)
  PRODUCT_PACKAGES += FestivalWallpaper
endif
# For BIRD_FESTIVAL_WALLPAPER BIRD Wang Lei 130401 ADD END

# For BIRD_LAUNCHER_GETEK BIRD Wang Lei 130508 ADD BEGIN
ifeq ($(BIRD_LAUNCHER_GETEK), yes)
	PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))
	PRODUCT_PACKAGES += LauncherGetek
endif
# For BIRD_LAUNCHER_GETEK BIRD Wang Lei 130508 ADD END

# For BIRD_SMALL_SCREEN BIRD Wang Lei 130618 ADD BEGIN
ifeq ($(BIRD_SMALL_SCREEN), yes)
	PRODUCT_PACKAGES += SmallScreen
endif
# For BIRD_SMALL_SCREEN BIRD Wang Lei 130618 ADD END

# For BIRD_READING_MODE BIRD Wang Lei 130808 ADD BEGIN
ifeq ($(BIRD_READING_MODE), yes)
	PRODUCT_PACKAGES += ReadingMode
endif
# For BIRD_READING_MODE BIRD Wang Lei 130808 ADD END

# For BIRD_THEME_XPERIA BIRD Wang Lei 130820 ADD BEGIN
ifeq ($(BIRD_THEME_XPERIA), yes)
	PRODUCT_PACKAGES += ThemeXperia
endif
# For BIRD_THEME_XPERIA BIRD Wang Lei 130820 ADD END

# For BIRD_ANALOG_CLOCK BIRD Wang Lei 130911 ADD BEGIN
ifeq ($(BIRD_ANALOG_CLOCK), yes)
    ifdef BIRD_ANALOG_CLOCK_TYPE
      PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.analogclcok.type=$(BIRD_ANALOG_CLOCK_TYPE)
    endif
    PRODUCT_PACKAGES += BirdAnalogClock
endif
# For BIRD_ANALOG_CLOCK BIRD Wang Lei 130911 ADD END

# For BIRD_MUSIC_WIDGET BIRD Wang Lei 130917 ADD BEGIN
ifeq ($(BIRD_MUSIC_WIDGET), yes)
	PRODUCT_PACKAGES += MusicWidget
endif
# For BIRD_MUSIC_WIDGET BIRD Wang Lei 130917 ADD END

# For BIRD_ANALOG_CLOCK_3D BIRD Wang Lei 131111 ADD BEGIN
ifeq ($(BIRD_ANALOG_CLOCK_3D), yes)
	PRODUCT_PACKAGES += BirdAnalogClock3D
endif
# For BIRD_ANALOG_CLOCK_3D BIRD Wang Lei 131111 ADD END

# For BIRD_PHOTO_WIDGET BIRD Wang Lei 131210 ADD BEGIN
ifeq ($(BIRD_PHOTO_WIDGET), yes)
	PRODUCT_PACKAGES += PhotoWidget
endif
	PRODUCT_PACKAGES += libbox2d
# For BIRD_PHOTO_WIDGET BIRD Wang Lei 131210 ADD END

# For BIRD_LAUNCHER_EFFECTS BIRD Wang Lei 131220 ADD BEGIN
ifeq ($(BIRD_LAUNCHER_EFFECTS), yes)
	PRODUCT_PACKAGES += LauncherEffects
endif
# For BIRD_LAUNCHER_EFFECTS BIRD Wang Lei 131220 ADD END

# For BIRD_VISUALIZER BIRD Wang Lei 140121 ADD BEGIN
ifeq ($(BIRD_VISUALIZER), yes)
	PRODUCT_PACKAGES += Visualizer
endif
# For BIRD_VISUALIZER BIRD Wang Lei 140121 ADD END

# For BIRD_VIRTUAL_BOX_FOLDER BIRD Wang Lei 140215 ADD BEGIN
ifeq ($(BIRD_VIRTUAL_BOX_FOLDER), yes)
	PRODUCT_PACKAGES += VirtualBoxFolder
	PRODUCT_COPY_FILES += packages/apps/VirtualBoxFolder/libs/libtencent_loc.so:system/lib/libtencent_loc.so
endif
# For BIRD_VIRTUAL_BOX_FOLDER BIRD Wang Lei 140215 ADD END

# for D95 after service, pyj 121114 add begin
ifeq ($(BIRD_AFTER_SERVICE), yes)
  PRODUCT_PACKAGES += AfterService
endif
# for D95 after service, pyj 121114 add end

ifeq ($(strip $(BIRD_SALES_TRACKER_FOR_WINMAX)), yes)
  PRODUCT_PACKAGES += GMobiSalesTrackerForWinmax
else ifeq ($(strip $(BIRD_SALES_TRACKER_FOR_HSL)), yes)
  PRODUCT_PACKAGES += GMobiSalesTracker
endif

#-----------------------------------------------------------------------------------------------
#opt
ifeq ($(BIRD_SENSE_LAUNCHER), yes)
  PRODUCT_PACKAGES += SenseLauncher
  PRODUCT_PROPERTY_OVERRIDES += birdsenselauncher=true \
                                curlockscreen=7
endif
ifeq ($(BIRD_SENSE_LAUNCHERG11), yes)
  PRODUCT_PACKAGES += SenseLauncherG11
  PRODUCT_PROPERTY_OVERRIDES += birdsenselauncherg11=true \
                                curlockscreen=6
endif
ifeq ($(BIRD_SENSE_LAUNCHERG14), yes)
  PRODUCT_PACKAGES += SenseLauncherG14
  PRODUCT_PROPERTY_OVERRIDES += curlockscreen=10
endif
ifeq ($(BIRD_LOCKSCREEN_G11), yes)
  PRODUCT_PROPERTY_OVERRIDES += curlockscreen=6
endif
ifeq ($(BIRD_LOCKSCREEN_G14), yes)
  PRODUCT_PROPERTY_OVERRIDES += curlockscreen=10
endif
ifeq ($(BIRD_GLASS_LOCKSCREEN), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
                                curlockscreen=5
endif
ifeq ($(BIRD_G2_GLASS_LOCKSCREEN), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
                                curlockscreen=8
endif
ifeq ($(BIRD_IPHONE_LOCKSCREEN), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
                                ro.bird.lockscreen=5
endif
ifeq ($(BIRD_WAVE_LOCKSCREEN), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
                                curlockscreen=11
endif
ifeq ($(BIRD_ROLLER_LOCKSCREEN), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
                                ro.bird.lockscreen=4
endif
#lvhuaiyi add for HYDAI lockscreen
ifeq ($(BIRD_HYDAI_LOCKSCREEN), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
                                ro.bird.lockscreen=3
endif
ifneq ($(strip $(BIRD_DEFAULT_FONT_SIZE)), )
  PRODUCT_PROPERTY_OVERRIDES += \
                                bird_default_font_size=$(BIRD_DEFAULT_FONT_SIZE)
endif
ifneq ($(strip $(BIRD_DEFAULT_IME)), )
  PRODUCT_PROPERTY_OVERRIDES += bird_default_ime=$(BIRD_DEFAULT_IME)
endif

#[81471]verno huangrunming 20120612
ifneq ($(strip $(BIRD_EM_VERNO)), )
  PRODUCT_PROPERTY_OVERRIDES += bird_em_verno=$(BIRD_EM_VERNO)
endif

ifneq ($(strip $(BIRD_BASEBAND_VERSION)), )
  PRODUCT_PROPERTY_OVERRIDES += bird_baseband_version=$(BIRD_BASEBAND_VERSION)
endif

ifeq ($(BIRD_EWC_SETTING), yes)
  PRODUCT_PROPERTY_OVERRIDES += bird_ewc_customer_id=$(BIRD_EWC_CUSTOMER_ID)
  PRODUCT_PROPERTY_OVERRIDES += bird_ewc_production_id=$(BIRD_EWC_PRODUCTION_ID)
endif

ifneq ($(strip $(BIRD_DM_VERNO)), )
  PRODUCT_PROPERTY_OVERRIDES += bird_dm_verno=$(BIRD_DM_VERNO)
endif

ifdef BIRD_EMMC_FFU
PRODUCT_COPY_FILES += \
bird/ffu/emmc-ffu:system/etc/ffu/emmc-ffu \
bird/ffu/fw.bin:system/etc/ffu/fw.bin \
bird/ffu/swrm.ko:system/lib/modules/swrm.ko
endif

#add start by shenzhiwang
ifdef BIRD_CALL_VIBRATE
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.call_vibrate_mode=$(BIRD_CALL_VIBRATE_MODE)
endif

#add start by shenzhiwang
#ifeq ($(strip $(BIRD_WRITE_SUPPORT)),yes)  
    PRODUCT_PACKAGES += bwrite
#endif

#add by dongyaoping for bug[103090] begin
ifeq ($(strip $(BIRD_PDF_MANUAL)), yes)
  PRODUCT_PACKAGES += KingoPdf
  PRODUCT_COPY_FILES += packages/apps/KingoPdf/res/raw/usermanual.pdf:system/manualpdf/usermanual.pdf
endif
#add by dongyaoping for bug[103090] end




#add start by shenzhiwang
#ifeq ($(strip $(BIRD_COPY_SUPPORT)),yes)  
    PRODUCT_PACKAGES += bcopy
    PRODUCT_PROPERTY_OVERRIDES += \
        persist.copy.app=0
#endif

#bug[100464] add by meifangting 20140225 begin 
ifdef BIRD_GOOGLEPALY_INSTALL
  PRODUCT_PACKAGES += InstallApp  
  ifneq ($(strip $(BIRD_GOOGLEPALY_INSTALL)),yes)
    PRODUCT_PROPERTY_OVERRIDES += \
    ro.install.app.order=$(BIRD_GOOGLEPALY_INSTALL)
  endif
  PRODUCT_COPY_FILES += \
  bird/installapp/assets/install_app_list.xml:packages/apps/InstallApp/assets/install_app_list.xml
  src_files := $(shell ls bird/installapp/apk/)
  PRODUCT_COPY_FILES += $(foreach file, $(src_files),bird/installapp/apk/$(file):system/unstalled/$(file))
endif
#bug[100464] add by meifangting 20140225 end

ifdef BIRD_HIDE_APP_CONDITION
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.cooee.hide.app.condition=$(BIRD_HIDE_APP_CONDITION)
endif
#add start by shenyibin 20130903
ifdef BIRD_MEDIA_VIDEO
PRODUCT_COPY_FILES += \
   bird/sounds/media/$(BIRD_MEDIA_VIDEO).mp4:system/media/$(BIRD_MEDIA_VIDEO).mp4
endif

ifdef BIRD_HIDE_APP
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.cooee.hide.app=$(BIRD_HIDE_APP)
endif
ifdef BIRD_HIDE_APP_DURATION
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.cooee.hide.app.duration=$(BIRD_HIDE_APP_DURATION)
endif
#add end by shenzhiwang

ifeq ($(BIRD_INPUT_METHOD_BY_LANGUAGE), yes)
  PRODUCT_PROPERTY_OVERRIDES += bird_input_method_by_language=true
endif
ifeq ($(BIRD_BROWSER_WAP_UA_ON), yes)
  PRODUCT_PROPERTY_OVERRIDES += bird_browser_wap_ua=$(BIRD_BROWSER_WAP_UA)
endif
ifdef BIRD_UPDATEZIP_CODE
ADDITIONAL_BUILD_PROPERTIES += \
             bird.updatezip.code=$(BIRD_UPDATEZIP_CODE)
else
ADDITIONAL_BUILD_PROPERTIES += \
             bird.updatezip.code=BIRD
endif
ifneq ($(strip $(BIRD_DEFAULT_LIVE_WALLPAPER)), )
ADDITIONAL_BUILD_PROPERTIES += \
             bird_default_live_wallpaper=$(BIRD_DEFAULT_LIVE_WALLPAPER)
endif
ifneq ($(strip $(BIRD_THEME_APPICON)), )
PRODUCT_PROPERTY_OVERRIDES += \
             bird_theme_appicon=$(BIRD_THEME_APPICON)
endif

#[95838]shenyibin add  for BIRD_LAUNCHERDOOV_APPICON_STYLE
ifeq ($(strip $(BIRD_LAUNCHERDOOV_APPICON_STYLE)), yes)
		PRODUCT_PROPERTY_OVERRIDES += \
	             bird_ld_icon_style=true
    PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))
	PRODUCT_PACKAGES += LauncherDoov        
endif



#shenyibin add [95720] for customizing the WorkspaceScreenCount 
ifneq ($(strip $(BIRD_WORKSPACE_SCREEN_COUNT)), )
PRODUCT_PROPERTY_OVERRIDES += \
             workspace_screen_count=$(BIRD_WORKSPACE_SCREEN_COUNT)
endif

ifdef BIRD_FUNCTION_BY_CUSTOM
    ifdef BIRD_FUNCTION_SHORT_KEY
        ADDITIONAL_BUILD_PROPERTIES += function_key_short_press=$(BIRD_FUNCTION_SHORT_KEY)
    endif
    ifdef BIRD_FUNCTION_LONG_KEY
        ADDITIONAL_BUILD_PROPERTIES += function_key_long_press=$(BIRD_FUNCTION_LONG_KEY)
    endif
endif

ifneq ($(strip $(BIRD_EMERGENCY_CALL_KEY)), )
    ADDITIONAL_BUILD_PROPERTIES += bird_emergency_number=$(BIRD_EMERGENCY_NUMBER)
endif

ifeq ($(BIRD_APK_ADOBE_FLASH_PLAYER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AdobeFlashPlayer/AdobeFlashPlayer.apk:system/app/AdobeFlashPlayer.apk 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AdobeFlashPlayer/libflashplayer.so:system/lib/armeabi-v7a/libflashplayer.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AdobeFlashPlayer/libstagefright_froyo.so:system/lib/armeabi-v7a/libstagefright_froyo.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AdobeFlashPlayer/libstagefright_honeycomb.so:system/lib/armeabi-v7a/libstagefright_honeycomb.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AdobeFlashPlayer/libysshared.so:system/lib/armeabi-v7a/libysshared.so
endif

ifeq ($(BIRD_APK_ADOBE_FLASH_PLAYER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AdobeFlashPlayer/AdobeFlashPlayer.apk:data/app/AdobeFlashPlayer.apk
endif

ifeq ($(BIRD_APK_QQ_PLAYER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQPlayer/QQPlayer.apk:system/app/QQPlayer.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQPlayer/libexport.so:system/lib/libexport.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQPlayer/libexport_neon.so:system/lib/libexport_neon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQPlayer/libgetproperty.so:system/lib/libgetproperty.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQPlayer/libsemaphore.so:system/lib/libsemaphore.so
endif

ifeq ($(BIRD_APK_QQ_PLAYER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQPlayer/QQPlayer.apk:data/app/QQPlayer.apk
endif

ifeq ($(BIRD_APK_QQ),system) 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/QQ.apk:system/app/QQ.apk 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libamrnb.so:system/lib/libamrnb.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libaudiohelper.so:system/lib/libaudiohelper.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libcftutils.so:system/lib/libcftutils.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libmqq.so:system/lib/libmqq.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libNativeRQD.so:system/lib/libNativeRQD.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libns.so:system/lib/libns.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libqq_sharp.so:system/lib/libqq_sharp.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libsm_mq.so:system/lib/libsm_mq.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libTcVp8Dec.so:system/lib/libTcVp8Dec.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libTcVp8Enc.so:system/lib/libTcVp8Enc.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libTcVpxDec.so:system/lib/libTcVpxDec.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libTcVpxEnc.so:system/lib/libTcVpxEnc.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libtea.so:system/lib/libtea.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libtraeimp-armeabi-v7a.so:system/lib/libtraeimp-armeabi-v7a.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libVCodec_v5te.so:system/lib/libVCodec_v5te.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libVCodec_v6.so:system/lib/libVCodec_v6.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libVCodec_v7a.so:system/lib/libVCodec_v7a.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/libVideoCtrl.so:system/lib/libVideoCtrl.so 
endif 

ifeq ($(BIRD_APK_QQ),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ/QQ.apk:data/app/QQ.apk
endif

ifeq ($(BIRD_APK_1365E_M3), data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/1365E_M3/.keep_list:data/app/.keep_list
PRODUCT_COPY_FILES += packages/apps/Bird_APK/1365E_M3/.restore_list:data/app/.restore_list
endif

ifeq ($(BIRD_APK_A2),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/A2C_APK/.keep_list:data/app/.keep_list
PRODUCT_COPY_FILES += packages/apps/Bird_APK/A2C_APK/.restore_list:data/app/.restore_list
endif


ifeq ($(BIRD_APK_QQ_MANAGER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/QQManager.apk:system/app/QQManager.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libcryptor-1.0.2.so:system/lib/libcryptor-1.0.2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libams-1.1.3.so:system/lib/libams-1.1.3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libapollo-1.1.8.so:system/lib/libapollo-1.1.8.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/liblocation-1.1.2.so:system/lib/liblocation-1.1.2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/liblogin-1.0.1.so:system/lib/liblogin-1.0.1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libnative-1.0.1.so:system/lib/libnative-1.0.1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libNativeRQD.so:system/lib/libNativeRQD.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libnum_mark-1.0.1.so:system/lib/libnum_mark-1.0.1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libqrcode-1.0.0.so:system/lib/libqrcode-1.0.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libsmschecker-1.0.4.so:system/lib/libsmschecker-1.0.4.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libsmsparser-1.0.2.so:system/lib/libsmsparser-1.0.2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libSync.so:system/lib/libSync.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libTccDiff.so:system/lib/libTccDiff.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/libtmspinyin.so:system/lib/libtmspinyin.so
endif

ifeq ($(BIRD_APK_QQ_MANAGER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/QQManager.apk:data/app/QQManager.apk
endif

ifeq ($(BIRD_APK_QQ_MANAGER),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager/QQManager.apk:data/mediatek/kernel/bcopy/QQManager/data/app/QQManager.apk
endif

ifeq ($(BIRD_APK_QQ_MANAGER_4PLUS2),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/QQManager_4plus2.apk:system/app/QQManager_4plus2.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/libcryptor-1.0.0.so:system/lib/libcryptor-1.0.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/liblocation-1.0.0.so:system/lib/liblocation-1.0.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/libnative-1.0.0.so:system/lib/libnative-1.0.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/libsmschecker-1.0.1.so:system/lib/libsmschecker-1.0.1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/libsmsparser-1.0.0.so:system/lib/libsmsparser-1.0.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/libams-1.1.0.so:system/lib/libams-1.1.0.so
endif

ifeq ($(BIRD_APK_QQ_MANAGER_4PLUS2),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/QQManager_4plus2.apk:data/app/QQManager_4plus2.apk
endif

ifeq ($(BIRD_APK_QQ_MANAGER_4PLUS2),system_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/QQManager_4plus2.apk:data/mediatek/kernel/bcopy/QQManager_4plus2/system/app/QQManager_4plus2.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/libcryptor-1.0.0.so:data/mediatek/kernel/bcopy/QQManager_4plus2/system/lib/libcryptor-1.0.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/liblocation-1.0.0.so:data/mediatek/kernel/bcopy/QQManager_4plus2/system/lib/liblocation-1.0.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/libnative-1.0.0.so:data/mediatek/kernel/bcopy/QQManager_4plus2/system/lib/libnative-1.0.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/libsmschecker-1.0.1.so:data/mediatek/kernel/bcopy/QQManager_4plus2/system/lib/libsmschecker-1.0.1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/libsmsparser-1.0.0.so:data/mediatek/kernel/bcopy/QQManager_4plus2/system/lib/libsmsparser-1.0.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/libams-1.1.0.so:data/mediatek/kernel/bcopy/QQManager_4plus2/system/lib/libams-1.1.0.so
endif

ifeq ($(BIRD_APK_QQ_MANAGER_4PLUS2),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManager_4plus2/QQManager_4plus2.apk:data/mediatek/kernel/bcopy/QQManager_4plus2/data/app/QQManager_4plus2.apk
endif

ifeq ($(BIRD_APK_QQ_GAME),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TencentQQGame/TencentQQGame.apk:system/app/TencentQQGame.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TencentQQGame/libcom_tencent_qqgame_buildin_codecwrapper.so:system/lib/libcom_tencent_qqgame_buildin_codecwrapper.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TencentQQGame/libcom_tencent_qqgame_buildin_msfboot.so:system/lib/libcom_tencent_qqgame_buildin_msfboot.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TencentQQGame/libcom_tencent_qqgame_encrypt.so:system/lib/libcom_tencent_qqgame_encrypt.so
endif

ifeq ($(BIRD_APK_QQ_GAME),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TencentQQGame/TencentQQGame.apk:data/app/TencentQQGame.apk
endif

ifeq ($(BIRD_APK_QQ_BROWSER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/QQBrower.apk:system/app/QQBrower.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libchirp-jni.so:system/lib/libchirp-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libgif-jni.so:system/lib/libgif-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/liblbs.so:system/lib/liblbs.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libmttwebcore.so:system/lib/libmttwebcore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libNativeRQD.so:system/lib/libNativeRQD.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libqqpatch.so:system/lib/libqqpatch.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libQrcodeDecoder.so:system/lib/libQrcodeDecoder.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libspeex.so:system/lib/libspeex.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libvadLib-v2.so:system/lib/libvadLib-v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libwebp_base.so:system/lib/libwebp_base.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libwebp_base_neon.so:system/lib/libwebp_base_neon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libwonderplayer_init.so:system/lib/libwonderplayer_init.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libwtecdh.so:system/lib/libwtecdh.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libzipalign.so:system/lib/libzipalign.so
endif

ifeq ($(BIRD_APK_QQ_BROWSER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/QQBrower.apk:data/app/QQBrower.apk
endif

ifeq ($(BIRD_APK_QQ_BROWSER),system_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/QQBrower.apk:data/mediatek/kernel/bcopy/QQBrowser/system/app/QQBrower.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libchirp-jni.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libchirp-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libgif-jni.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libgif-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/liblbs.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/liblbs.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libmttwebcore.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libmttwebcore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libNativeRQD.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libNativeRQD.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libqqpatch.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libqqpatch.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libQrcodeDecoder.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libQrcodeDecoder.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libspeex.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libspeex.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libvadLib-v2.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libvadLib-v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libwebp_base.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libwebp_base.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libwebp_base_neon.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libwebp_base_neon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libwonderplayer_init.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libwonderplayer_init.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libwtecdh.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libwtecdh.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/libzipalign.so:data/mediatek/kernel/bcopy/QQBrowser/system/lib/libzipalign.so
endif

ifeq ($(BIRD_APK_QQ_BROWSER),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser/QQBrower.apk:data/mediatek/kernel/bcopy/QQBrowser/data/app/QQBrower.apk
endif

ifeq ($(BIRD_APK_QQ_BROWSER_LITE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/QQBrowser.apk:system/app/QQBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/libwebp-jni_eclair.so:system/lib/libwebp-jni_eclair.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/libwebp-mtt_0.2.0.so:system/lib/libwebp-mtt_0.2.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/qbchannel.ini:system/channel/qbchannel.ini
endif

ifeq ($(BIRD_APK_QQ_BROWSER_LITE),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/QQBrowser.apk:data/app/QQBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/qbchannel.ini:data/channel/qbchannel.ini
endif

ifeq ($(BIRD_APK_QQ_BROWSER_LITE),system_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/QQBrowser.apk:data/mediatek/kernel/bcopy/QQBrowser_lite/system/app/QQBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/libwebp-jni_eclair.so:data/mediatek/kernel/bcopy/QQBrowser_lite/system/lib/libwebp-jni_eclair.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/libwebp-mtt_0.2.0.so:data/mediatek/kernel/bcopy/QQBrowser_lite/system/lib/libwebp-mtt_0.2.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/qbchannel.ini:data/mediatek/kernel/bcopy/QQBrowser_lite/system/channel/qbchannel.ini
endif

ifeq ($(BIRD_APK_QQ_BROWSER_LITE),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/QQBrowser.apk:data/mediatek/kernel/bcopy/QQBrowser_lite/data/app/QQBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowser_lite/qbchannel.ini:data/mediatek/kernel/bcopy/QQBrowser_lite/data/channel/qbchannel.ini
endif


ifeq ($(BIRD_APK_TENCENT_NEWS),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_News/TencentNews.apk:system/app/TencentNews.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_News/libckey.so:system/lib/libckey.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_News/libwtecdh.so:system/lib/libwtecdh.so
endif

ifeq ($(BIRD_APK_TENCENT_NEWS),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_News/TencentNews.apk:data/app/TencentNews.apk
endif

ifeq ($(BIRD_APK_TENCENT_NEWS),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_News/TencentNews.apk:data/mediatek/kernel/bcopy/Tencent_News/data/app/TencentNews.apk
endif

ifeq ($(BIRD_APK_TENCENT_VIDEO),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/Tencent_Video.apk:system/app/Tencent_Video.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libgetproperty.so:system/lib/libgetproperty.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libhttpproxy.so:system/lib/libhttpproxy.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libmresearch.so:system/lib/libmresearch.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libMultiScreenNet.so:system/lib/libMultiScreenNet.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libNativeRQD.so:system/lib/libNativeRQD.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libPlayerCore.so:system/lib/libPlayerCore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libPlayerCore_neon.so:system/lib/libPlayerCore_neon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libsta_jni.so:system/lib/libsta_jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libTUpdateService.so:system/lib/libTUpdateService.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libTxCodec.so:system/lib/libTxCodec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libTxCodec_neon.so:system/lib/libTxCodec_neon.so
endif

ifeq ($(BIRD_APK_TENCENT_VIDEO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/Tencent_Video.apk:data/app/Tencent_Video.apk
endif

ifeq ($(BIRD_APK_TENCENT_VIDEO),system_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/Tencent_Video.apk:data/mediatek/kernel/bcopy/Tencent_Video/system/app/Tencent_Video.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libgetproperty.so:data/mediatek/kernel/bcopy/Tencent_Video/system/lib/libgetproperty.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libhttpproxy.so:data/mediatek/kernel/bcopy/Tencent_Video/system/lib/libhttpproxy.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libmresearch.so:data/mediatek/kernel/bcopy/Tencent_Video/system/lib/libmresearch.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libMultiScreenNet.so:data/mediatek/kernel/bcopy/Tencent_Video/system/lib/libMultiScreenNet.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libNativeRQD.so:data/mediatek/kernel/bcopy/Tencent_Video/system/lib/libNativeRQD.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libPlayerCore.so:data/mediatek/kernel/bcopy/Tencent_Video/system/lib/libPlayerCore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libPlayerCore_neon.so:data/mediatek/kernel/bcopy/Tencent_Video/system/lib/libPlayerCore_neon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libsta_jni.so:data/mediatek/kernel/bcopy/Tencent_Video/system/lib/libsta_jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libTUpdateService.so:data/mediatek/kernel/bcopy/Tencent_Video/system/lib/libTUpdateService.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libTxCodec.so:data/mediatek/kernel/bcopy/Tencent_Video/system/lib/libTxCodec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/libTxCodec_neon.so:data/mediatek/kernel/bcopy/Tencent_Video/system/lib/libTxCodec_neon.so
endif

ifeq ($(BIRD_APK_TENCENT_VIDEO),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Tencent_Video/Tencent_Video.apk:data/mediatek/kernel/bcopy/Tencent_Video/data/app/Tencent_Video.apk
endif

ifeq ($(BIRD_APK_DOUWAN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouWan/douwan.apk:system/app/douwan.apk
endif

ifeq ($(BIRD_APK_DOUWAN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouWan/douwan.apk:data/app/douwan.apk
endif

ifeq ($(BIRD_APK_MOBO_PLAYER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MoboPlayer/MoboPlayer.apk:system/app/MoboPlayer.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MoboPlayer/libpng.so:system/lib/libpng.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MoboPlayer/libffmpeg_armv7_neon.so:system/lib/libffmpeg_armv7_neon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MoboPlayer/libcmplayer_v7_14.so:system/lib/libcmplayer_v7_14.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MoboPlayer/libcmplayer_v7_8.so:system/lib/libcmplayer_v7_8.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MoboPlayer/libcmplayer_v7.so:system/lib/libcmplayer_v7.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MoboPlayer/libcmplayer_v5_14.so:system/lib/libcmplayer_v5_14.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MoboPlayer/libcmplayer_v5_8.so:system/lib/libcmplayer_v5_8.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MoboPlayer/libcmplayer_v5_4.so:system/lib/libcmplayer_v5_4.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MoboPlayer/libcmplayer_v5.so:system/lib/libcmplayer_v5.so
endif

ifeq ($(BIRD_APK_MOBO_PLAYER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MoboPlayer/MoboPlayer.apk:data/app/MoboPlayer.apk
endif

ifeq ($(BIRD_APK_APP_WEIXIN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/WeChat.apk:system/app/WeChat.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libAmmCommon.so:system/lib/libAmmCommon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libcdntran.so:system/lib/libcdntran.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libCrashMonitorForJni.so:system/lib/libCrashMonitorForJni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libImgProcessScan.so:system/lib/libImgProcessScan.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libImgTools.so:system/lib/libImgTools.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libmmdb.so:system/lib/libmmdb.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libMMProtocalJni.so:system/lib/libMMProtocalJni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libnetwork.so:system/lib/libnetwork.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libQMP.so:system/lib/libQMP.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libQrcodeDecoder.so:system/lib/libQrcodeDecoder.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libtenpay_utils.so:system/lib/libtenpay_utils.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libvoice.so:system/lib/libvoice.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libWXZBar.so:system/lib/libWXZBar.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/libxlog.so:system/lib/libxlog.so
endif

ifeq ($(BIRD_APK_APP_WEIXIN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WeiXin/WeChat.apk:data/app/WeChat.apk
endif

ifeq ($(BIRD_APK_QQ_ALIGNED),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ_aligned/QQ.apk:system/app/QQ.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ_aligned/libCommon.so:system/lib/libCommon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ_aligned/libVideoCtrl.so:system/lib/libVideoCtrl.so
endif

ifeq ($(BIRD_APK_QQ_ALIGNED),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQ_aligned/QQ.apk:data/app/QQ.apk
endif

ifeq ($(BIRD_APK_ZHANGHAOTONG),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhangHaoTong_Tencent/Zhanghaotong.apk:system/app/Zhanghaotong.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhangHaoTong_Tencent/libcom_tencent_qqsso_codecwrapper.so:system/lib/libcom_tencent_qqsso_codecwrapper.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhangHaoTong_Tencent/libcom_tencent_qqsso_encrypt.so:system/lib/libcom_tencent_qqsso_encrypt.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhangHaoTong_Tencent/libcom_tencent_qqsso_msfboot.so:system/lib/libcom_tencent_qqsso_msfboot.so
endif

ifeq ($(BIRD_APK_ZHANGHAOTONG),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhangHaoTong_Tencent/Zhanghaotong.apk:data/app/Zhanghaotong.apk
endif



ifeq ($(BIRD_APK_UC_BROWSER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UCBrowser/UCBrowser.apk:system/app/UCBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UCBrowser/libCTZip.so:system/lib/libCTZip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UCBrowser/libwebp.so:system/lib/libwebp.so
endif

ifeq ($(BIRD_APK_UC_BROWSER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UCBrowser/UCBrowser.apk:data/app/UCBrowser.apk
endif

ifeq ($(BIRD_APK_UCBROWSER_FOREIGN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UCBrowser_Foreign/UCBrowser_Foreign.apk:system/app/UCBrowser_Foreign.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UCBrowser_Foreign/libBrowserShell_UC.so:system/lib/libBrowserShell_UC.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UCBrowser_Foreign/libWebCore_UC.so:system/lib/libWebCore_UC.so
endif

ifeq ($(BIRD_APK_UCBROWSER_FOREIGN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UCBrowser_Foreign/UCBrowser_Foreign.apk:data/app/UCBrowser_Foreign.apk
endif

ifeq ($(BIRD_APK_UCBROWSER_FOREIGN),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UCBrowser_Foreign/UCBrowser_Foreign.apk:data/mediatek/kernel/bcopy/UCBrowser_Foreign/data/app/UCBrowser_Foreign.apk
endif

ifeq ($(BIRD_APK_ES_FILE_EXPLORER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ES_FileExplorer/ES_FileExplorer.apk:system/app/ES_FileExplorer.apk
endif

ifeq ($(BIRD_APK_ES_FILE_EXPLORER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ES_FileExplorer/ES_FileExplorer.apk:data/app/ES_FileExplorer.apk
endif

ifeq ($(BIRD_APK_ES_TASK_MANAGER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ES_TaskManager/ES_TaskManager.apk:system/app/ES_TaskManager.apk
endif

ifeq ($(BIRD_APK_ES_TASK_MANAGER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ES_TaskManager/ES_TaskManager.apk:data/app/ES_TaskManager.apk
endif

ifeq ($(BIRD_APK_LETV),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/LeTV.apk:system/app/LeTV.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/libletvplayer_neon.so:system/lib/libletvplayer_neon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/libletvplayer_vfp.so:system/lib/libletvplayer_vfp.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/libmsc.so:system/lib/libmsc.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/liblocSDK3.so:system/lib/liblocSDK3.so
endif

ifeq ($(BIRD_APK_LETV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/LeTV.apk:data/app/LeTV.apk
endif

ifeq ($(BIRD_APK_LETV),system_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/LeTV.apk:data/mediatek/kernel/bcopy/LeTV/system/app/LeTV.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/libletvplayer_neon.so:data/mediatek/kernel/bcopy/LeTV/system/lib/libletvplayer_neon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/libletvplayer_vfp.so:data/mediatek/kernel/bcopy/LeTV/system/lib/libletvplayer_vfp.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/libmsc.so:data/mediatek/kernel/bcopy/LeTV/system/lib/libmsc.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/liblocSDK3.so:data/mediatek/kernel/bcopy/LeTV/system/lib/liblocSDK3.so
endif

ifeq ($(BIRD_APK_LETV),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LeTV/LeTV.apk:data/mediatek/kernel/bcopy/LeTV/data/app/LeTV.apk
endif

ifeq ($(BIRD_APK_SINA_WEIBO),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SinaWeibo/SinaWeibo.apk:system/app/SinaWeibo.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SinaWeibo/libutility.so:system/lib/libutility.so
endif

ifeq ($(BIRD_APK_SINA_WEIBO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SinaWeibo/SinaWeibo.apk:data/app/SinaWeibo.apk
endif

ifeq ($(BIRD_APK_SINA_WEIBO_MEMBER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SinaWeiboMember/SinaWeibo.apk:system/app/SinaWeibo.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SinaWeiboMember/libutility.so:system/lib/libutility.so
endif

ifeq ($(BIRD_APK_SINA_WEIBO_MEMBER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SinaWeiboMember/SinaWeibo.apk:data/app/SinaWeibo.apk
endif

ifeq ($(BIRD_APK_SINA_NEWS),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SinaNews/SinaNews.apk:system/app/SinaNews.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SinaNews/liblocSDK_2.4.so:system/lib/liblocSDK_2.4.so
endif

ifeq ($(BIRD_APK_SINA_NEWS),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SinaNews/SinaNews.apk:data/app/SinaNews.apk
endif

ifeq ($(BIRD_APK_SHOUYOU_CENTER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhongguoShouyouCenter/ShouyouCenter.apk:system/app/ShouyouCenter.apk
endif

ifeq ($(BIRD_APK_SHOUYOU_CENTER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhongguoShouyouCenter/ShouyouCenter.apk:data/app/ShouyouCenter.apk
endif

ifeq ($(BIRD_APK_SHOUYOU_CENTER),system_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhongguoShouyouCenter/ShouyouCenter.apk:data/mediatek/kernel/bcopy/ZhongguoShouyouCenter/system/app/ShouyouCenter.apk
endif

ifeq ($(BIRD_APK_SHOUYOU_CENTER),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhongguoShouyouCenter/ShouyouCenter.apk:data/mediatek/kernel/bcopy/ZhongguoShouyouCenter/data/app/ShouyouCenter.apk
endif

ifeq ($(BIRD_APK_BAIDU_MAP),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap/BaiduMap.apk:system/app/BaiduMap.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap/libAudiofe_V1OEM401.so:system/lib/libAudiofe_V1OEM401.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap/libCNPackageOEM401.so:system/lib/libCNPackageOEM401.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap/libDMPackageOEM401.so:system/lib/libDMPackageOEM401.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap/libejTTSOEM401.so:system/lib/libejTTSOEM401.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap/liblocSDK_2_4.so:system/lib/lliblocSDK_2_4.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap/libMapEngineOEM401.so:system/lib/libMapEngineOEM401.so
endif

ifeq ($(BIRD_APK_BAIDU_MAP),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap/BaiduMap.apk:data/app/BaiduMap.apk
endif

ifeq ($(BIRD_APK_BAIDU_MAP),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap/BaiduMap.apk:data/mediatek/kernel/bcopy/BaiduMap/data/app/BaiduMap.apk
endif

ifeq ($(BIRD_APK_BAIDU_MAP_EMMC),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/BaiduMap_EMMC.apk:system/app/BaiduMap_EMMC.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/libAudiofe_V1OEM310.so:system/lib/libAudiofe_V1OEM310.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/libCNPackageOEM310.so:system/lib/libCNPackageOEM310.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/libDMPackageOEM310.so:system/lib/libDMPackageOEM310.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/libejTTSOEM310.so:system/lib/libejTTSOEM310.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/liblocSDK_2.2.2.so:system/lib/liblocSDK_2.2.2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/libMapEngineOEM310.so:system/lib/libMapEngineOEM310.so
endif

ifeq ($(BIRD_APK_BAIDU_MAP_EMMC),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/BaiduMap_EMMC.apk:data/app/BaiduMap_EMMC.apk
endif

ifeq ($(BIRD_APK_BAIDU_MAP_EMMC),system_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/BaiduMap_EMMC.apk:data/mediatek/kernel/bcopy/BaiduMap_EMMC/system/app/BaiduMap_EMMC.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/libAudiofe_V1OEM310.so:data/mediatek/kernel/bcopy/BaiduMap_EMMC/system/lib/libAudiofe_V1OEM310.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/libCNPackageOEM310.so:data/mediatek/kernel/bcopy/BaiduMap_EMMC/system/lib/libCNPackageOEM310.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/libDMPackageOEM310.so:data/mediatek/kernel/bcopy/BaiduMap_EMMC/system/lib/libDMPackageOEM310.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/libejTTSOEM310.so:data/mediatek/kernel/bcopy/BaiduMap_EMMC/system/lib/libejTTSOEM310.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/liblocSDK_2.2.2.so:data/mediatek/kernel/bcopy/BaiduMap_EMMC/system/lib/liblocSDK_2.2.2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/libMapEngineOEM310.so:data/mediatek/kernel/bcopy/BaiduMap_EMMC/system/lib/libMapEngineOEM310.so
endif

ifeq ($(BIRD_APK_BAIDU_MAP_EMMC),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduMap_EMMC/BaiduMap_EMMC.apk:data/mediatek/kernel/bcopy/BaiduMap_EMMC/data/app/BaiduMap_EMMC.apk
endif

ifeq ($(BIRD_APK_BAIDU_SEARCH),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/BaiduSearch.apk:system/app/BaiduSearch.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libAesArithmetic.so:system/lib/libAesArithmetic.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libbase64encoder_v1_3.so:system/lib/llibbase64encoder_v1_3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libBDVoiceSearchClient_V2.so:system/lib/libBDVoiceSearchClient_V2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libcyberplayer.so:system/lib/libcyberplayer.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libffmpeg.so:system/lib/libffmpeg.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libgetcpuspec.so:system/lib/libgetcpuspec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/liblocSDK3.so:system/lib/liblocSDK3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libMoplusMD5.so:system/lib/libMoplusMD5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libp2p-jni.so:system/lib/libp2p-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/librabjni.so:system/lib/librabjni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libsapi_V5_0.so:system/lib/libsapi_V5_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libstlport_shared.so:system/lib/libstlport_shared.so
endif

ifeq ($(BIRD_APK_BAIDU_SEARCH),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/BaiduSearch.apk:data/app/BaiduSearch.apk
endif

ifeq ($(BIRD_APK_BAIDU_SEARCH),system_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/BaiduSearch.apk:data/mediatek/kernel/bcopy/BaiduSearch/system/app/BaiduSearch.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libAesArithmetic.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/libAesArithmetic.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libbase64encoder_v1_3.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/llibbase64encoder_v1_3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libBDVoiceSearchClient_V2.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/libBDVoiceSearchClient_V2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libcyberplayer.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/libcyberplayer.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libffmpeg.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/libffmpeg.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libgetcpuspec.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/libgetcpuspec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/liblocSDK3.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/liblocSDK3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libMoplusMD5.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/libMoplusMD5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libp2p-jni.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/libp2p-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/librabjni.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/librabjni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libsapi_V5_0.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/libsapi_V5_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/libstlport_shared.so:data/mediatek/kernel/bcopy/BaiduSearch/system/lib/libstlport_shared.so
endif

ifeq ($(BIRD_APK_BAIDU_SEARCH),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduSearch/BaiduSearch.apk:data/mediatek/kernel/bcopy/BaiduSearch/data/app/BaiduSearch.apk
endif

ifeq ($(BIRD_APK_BAIDU_INPUT_CUSTOM),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduInputCustom/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduInputCustom/libkpencore.so:system/lib/libkpencore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduInputCustom/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduInputCustom/libshare_v2.so:system/lib/libshare_v2.so
endif

ifeq ($(BIRD_APK_BAIDU_INPUT_CUSTOM),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduInputCustom/BaiduInput.apk:data/app/BaiduInput.apk
endif

ifeq ($(BIRD_APK_BAIDU_BROWSER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/BaiduBrowser.apk:system/app/BaiduBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/libflyflowjni.so:system/lib/libflyflowjni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/liblocSDK_2.4.so:system/lib/liblocSDK_2.4.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/libloginshare.so:system/lib/libloginshare.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/libosruntime.so:system/lib/libosruntime.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/libshare.so:system/lib/libshare.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/libtmfe30.so:system/lib/libtmfe30.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/libzeus2.1.so:system/lib/libzeus2.1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/libzeus2.1_AR.so:system/lib/libzeus2.1_AR.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/libzeus.so:system/lib/libzeus.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/libZeusJNI.so:system/lib/libZeusJNI.so
endif

ifeq ($(BIRD_APK_BAIDU_BROWSER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduBrowser/BaiduBrowser.apk:data/app/BaiduBrowser.apk
endif

ifeq ($(BIRD_APK_BAIDU_INPUT),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduInput/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduInput/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduInput/libkpencore.so:system/lib/libkpencore.so
endif

ifeq ($(BIRD_APK_BAIDU_INPUT),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BaiduInput/BaiduInput.apk:data/app/BaiduInput.apk
endif

ifeq ($(BIRD_APK_APP_HYBaiduinput),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HYBaiduinput/HYBaiduinput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HYBaiduinput/libinputcoreoem-2.so:system/lib/libinputcoreoem-2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HYBaiduinput/libkpencoreoem.so:system/lib/libkpencoreoem.so
endif

ifeq ($(BIRD_APK_CJBAIDU_INPUT),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/CJBaiduIME/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/CJBaiduIME/libinputcore-2.so:system/lib/libinputcore-2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/CJBaiduIME/libkpen_api_so.so:system/lib/libkpen_api_so.so
endif


ifeq ($(BIRD_APK_SOGOU_MAP),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SogouMap/SogouMap.apk:system/app/SogouMap.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SogouMap/libCNPackage.so:system/lib/libCNPackage.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SogouMap/libdes.so:system/lib/libdes.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SogouMap/libDMPackage.so:system/lib/libDMPackage.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SogouMap/libejTTS4.2.so:system/lib/libejTTS4.2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SogouMap/libgnustl_shared.so:system/lib/libgnustl_shared.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SogouMap/libTileStorage.so:system/lib/libTileStorage.so
endif

ifeq ($(BIRD_APK_SOGOU_MAP),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SogouMap/SogouMap.apk:data/app/SogouMap.apk
endif


ifeq ($(BIRD_APK_SOGOU_INPUT),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SogouInput/SogouInput.apk:system/app/SogouInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SogouInput/libkpencore_v52.so:system/lib/libkpencore_v52.so
endif


ifeq ($(BIRD_APK_SOGOU_INPUT),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SogouInput/SogouInput.apk:data/app/SogouInput.apk
endif

ifeq ($(BIRD_APK_BOHP_SOGOU_INPUT),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BOHP_SogouInput/SogouInput.apk:system/app/SogouInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BOHP_SogouInput/libkpencore_v48.so:system/lib/libkpencore_v48.so
endif



ifeq ($(BIRD_APK_BOHP_LIVEWALLPAPER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BOHP_LIVEWALLPAPER/WaterRipples_V3.02.12500.apk:system/app/WaterRipples_V3.02.12500.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BOHP_LIVEWALLPAPER/libandroidgl20.so:system/lib/libandroidgl20.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BOHP_LIVEWALLPAPER/libgdx.so:system/lib/libgdx.so
endif

ifeq ($(BIRD_APK_BOHP_LIVEWALLPAPER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BOHP_LIVEWALLPAPER/WaterRipples_V3.02.12500.apk:data/app/WaterRipples_V3.02.12500.apk
endif

ifeq ($(BIRD_APK_BOHP_SOGOU_INPUT),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BOHP_SogouInput/SogouInput.apk:data/app/SogouInput.apk
endif

ifeq ($(BIRD_APK_Keyboard_TH_INPUT),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Keyboard_TH_INPUT/Keyboard.apk:system/app/Keyboard.apk
endif


ifeq ($(BIRD_APK_DUOMIMUSIC),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DuoMiMusic/DuoMiMusic.apk:system/app/DuoMiMusic.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DuoMiMusic/libauthorized.so:system/lib/libauthorized.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DuoMiMusic/libdmcplayer.so:system/lib/libdmcplayer.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DuoMiMusic/libhightrack.so:system/lib/libhightrack.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DuoMiMusic/liblowtrack.so:system/lib/liblowtrack.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DuoMiMusic/libscan-files.so:system/lib/libscan-files.so
endif

ifeq ($(BIRD_APK_DUOMIMUSIC),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DuoMiMusic/DuoMiMusic.apk:data/app/DuoMiMusic.apk
endif


ifeq ($(BIRD_APK_WANDOUJIA),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WanDouJia/Wandoujia.apk:system/app/Wandoujia.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WanDouJia/libwdj_udid.so:system/lib/libwdj_udid.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WanDouJia/libudid.so:system/lib/libudid.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WanDouJia/libphoenixqr.apk.so:system/lib/libphoenixqr.apk.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WanDouJia/libphoenixnetty.apk.so:system/lib/libphoenixnetty.apk.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WanDouJia/libfastmd5.so:system/lib/libfastmd5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WanDouJia/libentryex.so:system/lib/libentryex.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WanDouJia/libDiffDecoder.so:system/lib/libDiffDecoder.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WanDouJia/libcipher.so:system/lib/libcipher.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WanDouJia/libaccount.so:system/lib/libaccount.so
endif

ifeq ($(BIRD_APK_WANDOUJIA),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WanDouJia/Wandoujia.apk:data/app/Wandoujia.apk
endif


ifeq ($(BIRD_APK_SANGUOAGE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SanGuoAge/SanGuoAge.apk:system/app/SanGuoAge.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SanGuoAge/libUnionPay.so:system/lib/libUnionPay.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SanGuoAge/libusedes.so:system/lib/libusedes.so
endif

ifeq ($(BIRD_APK_SANGUOAGE),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SanGuoAge/SanGuoAge.apk:data/app/SanGuoAge.apk
endif


ifeq ($(BIRD_APK_SOHUNEWS),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SohuNews/SohuNews.apk:system/app/SohuNews.apk
endif

ifeq ($(BIRD_APK_SOHUNEWS),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SohuNews/SohuNews.apk:data/app/SohuNews.apk
endif


ifeq ($(BIRD_APK_KGEDAREN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KGeDaren/KGeDaren.apk:system/app/KGeDaren.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KGeDaren/libKsongJNI.so:system/lib/libKsongJNI.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KGeDaren/libMp3Code.so:system/lib/libMp3Code.so
endif

ifeq ($(BIRD_APK_KGEDAREN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KGeDaren/KGeDaren.apk:data/app/KGeDaren.apk
endif


ifeq ($(BIRD_APK_KUAISHOUKANPIAN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KuaiShouKanPian/KSVOD.apk:system/app/KSVOD.apk
endif

ifeq ($(BIRD_APK_KUAISHOUKANPIAN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KuaiShouKanPian/KSVOD.apk:data/app/KSVOD.apk
endif


ifeq ($(BIRD_APK_MINGZHUSANGUO),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MingZhuSanGuo/MingZhuSanGuo.apk:system/app/MingZhuSanGuo.apk
endif

ifeq ($(BIRD_APK_MINGZHUSANGUO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MingZhuSanGuo/MingZhuSanGuo.apk:data/app/MingZhuSanGuo.apk
endif


ifeq ($(BIRD_APK_MINGZHUXUANYUAN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MingZhuXuanYuan/MingZhuXuanYuan.apk:system/app/MingZhuXuanYuan.apk
endif

ifeq ($(BIRD_APK_MINGZHUXUANYUAN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MingZhuXuanYuan/MingZhuXuanYuan.apk:data/app/MingZhuXuanYuan.apk
endif


ifeq ($(BIRD_APK_SHIJIJIAYUAN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ShiJiJiaYuan/ShiJiJiaYuan.apk:system/app/ShiJiJiaYuan.apk
endif

ifeq ($(BIRD_APK_SHIJIJIAYUAN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ShiJiJiaYuan/ShiJiJiaYuan.apk:data/app/ShiJiJiaYuan.apk
endif

ifeq ($(BIRD_APK_SANWUMA_TONGHUASHUN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SanWuMa_TongHuaShun/StraightFlush.apk:system/app/StraightFlush.apk
endif

ifeq ($(BIRD_APK_SANWUMA_TONGHUASHUN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SanWuMa_TongHuaShun/StraightFlush.apk:data/app/StraightFlush.apk
endif 

ifeq ($(BIRD_APK_ANGRY_BIRDS),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AngryBirds/AngryBirds.apk:system/app/AngryBirds.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AngryBirds/libangrybirds.so:system/lib/libangrybirds.so
endif

ifeq ($(BIRD_APK_ANGRY_BIRDS),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AngryBirds/AngryBirds.apk:data/app/AngryBirds.apk
endif

ifeq ($(BIRD_APK_MOJI_WEATHER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MojiWeather/MojiWeather.apk:system/app/MojiWeather.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MojiWeather/MojiWeatherServices.apk:system/app/MojiWeatherServices.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MojiWeather/libsndattslib.so:system/lib/libsndattslib.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MojiWeather/liblocSDK_2.4.so:system/lib/liblocSDK_2.4.so
endif

ifeq ($(BIRD_APK_MOJI_WEATHER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MojiWeather/MojiWeather.apk:data/app/MojiWeather.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MojiWeather/MojiWeatherServices.apk:data/app/MojiWeatherServices.apk
endif


ifeq ($(BIRD_APK_YUNMIWEATHER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather/YunmiWeather.apk:system/app/YunmiWeather.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather/liblocSDK_2.4.so:system/lib/liblocSDK_2.4.so
endif

ifeq ($(BIRD_APK_YUNMIWEATHER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather/YunmiWeather.apk:data/app/YunmiWeather.apk
endif

ifeq ($(BIRD_APK_YUNMIWEATHER_LITE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather_Lite/YunmiWeather_lite.apk:system/app/YunmiWeather_lite.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather_Lite/liblocSDK_2.4.so:system/lib/liblocSDK_2.4.so
endif

ifeq ($(BIRD_APK_YUNMIWEATHER_LITE),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather_Lite/YunmiWeather_lite.apk:data/app/YunmiWeather_lite.apk
endif


ifeq ($(BIRD_APK_YUNMIWEATHER_WITHVIVOSTYLE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather_withVivoStyle/yunmiWeather_with_vivostyle.apk:system/app/yunmiWeather_with_vivostyle.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather_withVivoStyle/liblocSDK3.so:system/lib/liblocSDK3.so
endif

ifeq ($(BIRD_APK_YUNMIWEATHER_NOMENU),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather_NoMenu/YunmiWeather_NoMenu.apk:system/app/YunmiWeather_NoMenu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather_NoMenu/liblocSDK3.so:system/lib/liblocSDK3.so
endif

ifeq ($(BIRD_APK_YUNMIWEATHER_NOMENU),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather_NoMenu/YunmiWeather_NoMenu.apk:data/app/YunmiWeather_NoMenu.apk
endif

ifeq ($(BIRD_APK_YUNMIWEATHER_WIDGET),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather_Widget/YunmiWeather_Widget.apk:system/app/YunmiWeather_Widget.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather_Widget/liblocSDK3.so:system/lib/liblocSDK3.so
endif

ifeq ($(BIRD_APK_YUNMIWEATHER_WIDGET),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunMiWeather_Widget/YunmiWeather_Widget.apk:data/app/YunmiWeather_Widget.apk
endif

ifeq ($(BIRD_APK_ZHIQUWEATHER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhiQuWeather/ZhiQuWeather.apk:system/app/ZhiQuWeather.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhiQuWeather/liblocSDK_2.4.so:system/lib/liblocSDK_2.4.so
endif

ifeq ($(BIRD_APK_ZHIQUWEATHER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhiQuWeather/ZhiQuWeather.apk:data/app/ZhiQuWeather.apk
endif

ifeq ($(BIRD_APK_YUNYINGYONG),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunYingyong/YunYingyong.apk:system/app/YunYingyong.apk
endif

ifeq ($(BIRD_APK_YUNYINGYONG),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YunYingyong/YunYingyong.apk:data/app/YunYingyong.apk
endif

ifeq ($(BIRD_APK_YUYINWANG),yes)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YuYinWang/yuyinzhuli.apk:system/app/yuyinzhuli.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YuYinWang/libBMapApiEngine_v1_3_5.so:system/lib/libBMapApiEngine_v1_3_5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YuYinWang/liblocSDK3.so:system/lib/liblocSDK3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YuYinWang/libmsc.so:system/lib/libmsc.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/YuYinWang/tts-engine-pkg-4.0.apk:data/app/tts-engine-pkg-4.0.apk
endif



ifeq ($(BIRD_APK_ADOBE_READER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AdobeReader/AdobeReader.apk:system/app/AdobeReader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AdobeReader/libAdobeReader.so:system/lib/libAdobeReader.so
endif

ifeq ($(BIRD_APK_ADOBE_READER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AdobeReader/AdobeReader.apk:data/app/AdobeReader.apk
endif


ifeq ($(BIRD_APK_DOCUMENTS_TO_GO),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DocumentsToGo/DocumentsToGo.apk:system/app/DocumentsToGo.apk
endif

ifeq ($(BIRD_APK_DOCUMENTS_TO_GO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DocumentsToGo/DocumentsToGo.apk:data/app/DocumentsToGo.apk
endif

ifeq ($(CJ_APK_DOCUMENTS_TO_GO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/CJDocumentsToGo/CJDocumentsToGo.apk:data/app/CJDocumentsToGo.apk
endif

ifeq ($(BIRD_APK_CANGJIE_IME),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/CangjieIME/CangjieIME.apk:system/app/CangjieIME.apk
endif

ifeq ($(BIRD_APK_CANGJIE_IME),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/CangjieIME/CangjieIME.apk:data/app/CangjieIME.apk
endif

ifeq ($(BIRD_APK_CJCANGJIE_IME),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/CJZhuyincangjieIME/zycjsrf.apk:system/app/zycjsrf.apk
endif

ifeq ($(BIRD_APK_HI_MARKET),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HiMarket/HiMarket.apk:system/app/HiMarket.apk
endif

ifeq ($(BIRD_APK_HI_MARKET),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HiMarket/HiMarket.apk:data/app/HiMarket.apk
endif

ifeq ($(BIRD_APK_ZP500_SPEC),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZP500_SPEC/zp500.apk:system/app/zp500.apk
endif

ifeq ($(BIRD_APK_YOUKU),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Youku/Youku.apk:system/app/Youku.apk
endif

ifeq ($(BIRD_APK_YOUKU),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Youku/Youku.apk:data/app/Youku.apk
endif

ifeq ($(BIRD_APK_MSN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MSN/MSN.apk:system/app/MSN.apk
endif

ifeq ($(BIRD_APK_MSN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MSN/MSN.apk:data/app/MSN.apk
endif

ifeq ($(BIRD_APK_TTPOD),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/TTPod.apk:system/app/TTPod.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libaacdecplugin.so:system/lib/libaacdecplugin.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libalacdecplugin.so:system/lib/libalacdecplugin.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libapedecplugin.so:system/lib/libapedecplugin.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libaudiofx.so:system/lib/libaudiofx.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libaudiofxlvm.so:system/lib/libaudiofxlvm.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libflacdecplugin.so:system/lib/libflacdecplugin.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libmp3decplugin.so:system/lib/libmp3decplugin.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libosal.so:system/lib/libosal.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libttcrypto.so:system/lib/libttcrypto.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libttmedia_jni.so:system/lib/libttmedia_jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libtttag.so:system/lib/libtttag.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libwavdecplugin.so:system/lib/libwavdecplugin.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libwmadecplugin.so:system/lib/libwmadecplugin.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libwmafloatdecplugin.so:system/lib/libwmafloatdecplugin.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/libwmaprodecplugin.so:system/lib/libwmaprodecplugin.so
endif

ifeq ($(BIRD_APK_TTPOD),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TTPod/TTPod.apk:data/app/TTPod.apk
endif

ifeq ($(BIRD_APK_FACE_BOOK),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FaceBook/FaceBook.apk:system/app/FaceBook.apk
endif

ifeq ($(BIRD_APK_FACE_BOOK),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FaceBook/FaceBook.apk:data/app/FaceBook.apk
endif

ifeq ($(BIRD_APK_EBUDDY),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/eBuddy/eBuddy.apk:system/app/eBuddy.apk
endif

ifeq ($(BIRD_APK_EBUDDY),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/eBuddy/eBuddy.apk:data/app/eBuddy.apk
endif


ifeq ($(BIRD_APK_TWITTER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Twitter/Twitter.apk:system/app/Twitter.apk
endif

ifeq ($(BIRD_APK_TWITTER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Twitter/Twitter.apk:data/app/Twitter.apk
endif

ifeq ($(BIRD_APK_TWITTER_MOBITEL),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOBITEL_Twitter/Twitter.apk:data/app/Twitter.apk
endif


ifeq ($(BIRD_APK_365_RILI_PLUS),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/365RiLiPlus/365RiLiPlus.apk:system/app/365RiLiPlus.apk
endif

ifeq ($(BIRD_APK_365_RILI_PLUS),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/365RiLiPlus/365RiLiPlus.apk:data/app/365RiLiPlus.apk
endif

ifeq ($(BIRD_APK_DOPOOL_PLAYER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DoPoolPlayer/DoPoolPlayer.apk:system/app/DoPoolPlayer.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DoPoolPlayer/libDmplayer.so:system/lib/libDmplayer.so
endif

ifeq ($(BIRD_APK_DOPOOL_PLAYER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DoPoolPlayer/DoPoolPlayer.apk:data/app/DoPoolPlayer.apk
endif 

ifeq ($(BIRD_APK_HJ_IME),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HuajiIME/zhuyincangjiehuajiv1_2_1.apk:system/app/zhuyincangjiehuajiv1_2_1.apk
endif

ifeq ($(BIRD_APK_FRUIT_NINJA),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Fruitninja/Fruitninja.apk:system/app/Fruitninja.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Fruitninja/libfruitninja.so:system/lib/libfruitninja.so
endif

ifeq ($(BIRD_APK_FRUIT_NINJA),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Fruitninja/Fruitninja.apk:data/app/Fruitninja.apk
endif 


ifeq ($(BIRD_APK_FRUIT_NINJA_CN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FruitninjaCN/FruitninjaCN.apk:system/app/FruitninjaCN.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FruitninjaCN/libfruitninja.so:system/lib/libfruitninja.so
endif

ifeq ($(BIRD_APK_FRUIT_NINJA_CN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FruitninjaCN/FruitninjaCN.apk:data/app/FruitninjaCN.apk
endif 

ifeq ($(BIRD_APK_NINJA),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Ninja/Ninja.apk:system/app/Ninja.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Ninja/libfeelingtouchninjiarun.so:system/lib/libfeelingtouchninjiarun.so
endif

ifeq ($(BIRD_APK_NINJA),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Ninja/Ninja.apk:data/app/Ninja.apk
endif 

ifeq ($(BIRD_APK_NEED_FOR_SPEED_SHIFT),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/NeedForSpeedShift/NeedForSpeedShift.apk:system/app/NeedForSpeedShift.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/NeedForSpeedShift/libnfsshift_na_wf.so:system/lib/libnfsshift_na_wf.so
endif

ifeq ($(BIRD_APK_NEED_FOR_SPEED_SHIFT),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/NeedForSpeedShift/NeedForSpeedShift.apk:data/app/NeedForSpeedShift.apk
endif 

ifeq ($(BIRD_APK_STEREO_3D),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Stereo3D/Stereo3D.apk:system/app/Stereo3D.apk
endif

ifeq ($(BIRD_APK_STEREO_3D),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Stereo3D/Stereo3D.apk:data/app/Stereo3D.apk
endif 

ifeq ($(BIRD_APK_STEREO_3D_WALLPAPER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Stereo3DWallpaper/Stereo3DWallpaper.apk:system/app/Stereo3DWallpaper.apk
endif

ifeq ($(BIRD_APK_STEREO_3D_WALLPAPER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Stereo3DWallpaper/Stereo3DWallpaper.apk:data/app/Stereo3DWallpaper.apk
endif 

ifeq ($(BIRD_APK_IREADER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/iReader.apk:system/app/iReader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/libcasdkjni.so:system/lib/libcasdkjni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/libentryex.so:system/lib/libentryex.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/libft2_mini_nodep.android.so:system/lib/libft2_mini_nodep.android.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/libftftottf.android.so:system/lib/libftftottf.android.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/libidentifyapp.so:system/lib/libidentifyapp.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/liblocSDK4.so:system/lib/liblocSDK4.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/libUiControl.so:system/lib/libUiControl.so

PRODUCT_PROPERTY_OVERRIDES += \
				 ro.ireader.channel=108124
endif

ifeq ($(BIRD_APK_IREADER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/iReader.apk:data/app/iReader.apk


PRODUCT_PROPERTY_OVERRIDES += \
				 ro.ireader.channel=108124
endif

ifeq ($(BIRD_APK_IREADER),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader/iReader.apk:data/mediatek/kernel/bcopy/iReader/data/app/iReader.apk
endif  

ifeq ($(BIRD_APK_IREADER_T9),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader_T9/iReader_bodaoshuomingshu.apk:system/app/iReader_bodaoshuomingshu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader_T9/libiReader_pdf.so:system/lib/libiReader_pdf.so
endif

ifeq ($(BIRD_APK_IREADER_V9),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader_V9/iReader_bodaoshuomingshu.apk:system/app/iReader_bodaoshuomingshu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/iReader_V9/libiReader_pdf.so:system/lib/libiReader_pdf.so
endif
#taobao 
ifeq ($(BIRD_APK_TAOBAO_AUTOFOCUS),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/Taobao_AutoFocus.apk:system/app/Taobao_AutoFocus.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/libLwUtils.so:system/app/libLwUtils.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/libsdkencrypt.so:system/app/libsdkencrypt.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/libSSECeg-mini-1.2.3.so:system/app/libSSECeg-mini-1.2.3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/libwebp.so:system/app/libwebp.so

endif

ifeq ($(BIRD_APK_TAOBAO_AUTOFOCUS),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/Taobao_AutoFocus.apk:data/app/Taobao_AutoFocus.apk
endif 

ifeq ($(BIRD_APK_TAOBAO_AUTOFOCUS),system_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/Taobao_AutoFocus.apk:data/mediatek/kernel/bcopy/Taobao_AutoFocus/system/app/Taobao_AutoFocus.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/libLwUtils.so:data/mediatek/kernel/bcopy/Taobao_AutoFocus/system/lib/libLwUtils.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/libsdkencrypt.so:data/mediatek/kernel/bcopy/Taobao_AutoFocus/system/lib/libsdkencrypt.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/libSSECeg-mini-1.2.3.so:data/mediatek/kernel/bcopy/Taobao_AutoFocus/system/lib/libSSECeg-mini-1.2.3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/libwebp.so:data/mediatek/kernel/bcopy/Taobao_AutoFocus/system/lib/libwebp.so
endif

ifeq ($(BIRD_APK_TAOBAO_AUTOFOCUS),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_AutoFocus/Taobao_AutoFocus.apk:data/mediatek/kernel/bcopy/Taobao_AutoFocus/data/app/Taobao_AutoFocus.apk
endif

ifeq ($(BIRD_APK_TAOBAO_ANQUANZHIFU),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_Anquanzhifu/Alipay_Anquanzhifu.apk:system/app/Alipay_Anquanzhifu.apk
endif

ifeq ($(BIRD_APK_TAOBAO_ANQUANZHIFU),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_Anquanzhifu/Alipay_Anquanzhifu.apk:data/app/Alipay_Anquanzhifu.apk
endif 

ifeq ($(BIRD_APK_ZHIFUBAO),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Zhifubao/Alipay.apk:system/app/Alipay.apk
endif

ifeq ($(BIRD_APK_ZHIFUBAO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Zhifubao/Alipay.apk:data/app/Alipay.apk
endif 

#taobao trip
ifeq ($(BIRD_APK_TAOBAO_TRIP),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_Trip/TaobaoTrip.apk:system/app/TaobaoTrip.apk
endif

ifeq ($(BIRD_APK_TAOBAO_TRIP),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_Trip/TaobaoTrip.apk:data/app/TaobaoTrip.apk
endif 

ifeq ($(BIRD_APK_TAOBAO_SMART),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_Smart/Taobao_Smart.apk:system/app/Taobao_Smart.apk
endif

ifeq ($(BIRD_APK_TAOBAO_SMART),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Taobao_Smart/Taobao_Smart.apk:data/app/Taobao_Smart.apk
endif 


ifeq ($(BIRD_APK_TMALL),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TMall/TMall.apk:system/app/TMall.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TMall/libSSECeg-1.0.3.so:system/lib/libSSECeg-1.0.3.so
endif

ifeq ($(BIRD_APK_TMALL),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TMall/TMall.apk:data/app/TMall.apk
endif 


ifeq ($(BIRD_APK_YIPAI),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YiPai/SelfPortrait.apk:system/app/SelfPortrait.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YiPai/libstlport_shared.so:system/lib/libstlport_shared.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YiPai/libself_portrait_jni.so:system/lib/libself_portrait_jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YiPai/libmakeupengine.so:system/lib/libmakeupengine.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YiPai/libjni_utils.so:system/lib/libjni_utils.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YiPai/libgputool.so:system/lib/libgputool.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YiPai/libglrenderer_jni.so:system/lib/libglrenderer_jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YiPai/libdetectobject.so:system/lib/libdetectobject.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YiPai/libbspatch.so:system/lib/libbspatch.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YiPai/libvoicerecognize.so:system/lib/libvoicerecognize

PRODUCT_COPY_FILES += packages/apps/Bird_APK/Amazon/Amazon.apk:system/app/Amazon.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Amazon/libA9BarcodeDecoder.so:system/lib/libA9BarcodeDecoder.so

endif

ifeq ($(BIRD_APK_YIPAI),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YiPai/SelfPortrait.apk:data/app/SelfPortrait.apk

PRODUCT_COPY_FILES += packages/apps/Bird_APK/Amazon/Amazon.apk:data/app/Amazon.apk
endif 

ifeq ($(BIRD_APK_RU_ANEWS),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/RU_ANEWS/RU_ANEWS.apk:data/app/RU_ANEWS.apk
endif 

ifeq ($(BIRD_APK_RU_LOKATA),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/RU_LOKATA/RU_LOKATA.apk:data/app/RU_LOKATA.apk
endif 


ifeq ($(BIRD_APK_WATERDROP_SOUNDOFF),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WaterDrop_SoundOff/WaterDrop_SoundOff.apk:system/app/WaterDrop_SoundOff.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WaterDrop_SoundOff/libpuddle_drops.so:system/lib/libpuddle_drops.so
endif

ifeq ($(BIRD_APK_WATERDROP_SOUNDOFF),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WaterDrop_SoundOff/WaterDrop_SoundOff.apk:data/app/WaterDrop_SoundOff.apk
endif 


ifeq ($(BIRD_APK_WATERDROP_SOUNDON),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WaterDrop_SoundOn/WaterDrop_SoundOn.apk:system/app/WaterDrop_SoundOn.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WaterDrop_SoundOn/libpuddle_drops.so:system/lib/libpuddle_drops.so
endif

ifeq ($(BIRD_APK_WATERDROP_SOUNDON),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WaterDrop_SoundOn/WaterDrop_SoundOn.apk:data/app/WaterDrop_SoundOn.apk
endif 

ifeq ($(BIRD_APK_SIKAI_GAME),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SkyMobi_Game/skymobi_game.apk:system/app/skymobi_game.apk
endif

ifeq ($(BIRD_APK_SIKAI_GAME),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SkyMobi_Game/skymobi_game.apk:data/app/skymobi_game.apk
endif


ifeq ($(BIRD_APK_YY_ASSISTANT),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YY_Assistant/YY_Assistant.apk:system/app/YY_Assistant.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YY_Assistant/libBMapApiEngine_v1_3_5.so:system/lib/libBMapApiEngine_v1_3_5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YY_Assistant/libmsc.so:system/lib/libmsc.so
endif

ifeq ($(BIRD_APK_YY_ASSISTANT),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YY_Assistant/YY_Assistant.apk:data/app/YY_Assistant.apk
endif 

ifeq ($(BIRD_APK_YY_ASSISTANT),system_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YY_Assistant/YY_Assistant.apk:data/mediatek/kernel/bcopy/YY_Assistant/system/app/YY_Assistant.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YY_Assistant/libBMapApiEngine_v1_3_5.so:data/mediatek/kernel/bcopy/YY_Assistant/system/lib/libBMapApiEngine_v1_3_5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YY_Assistant/libmsc.so:data/mediatek/kernel/bcopy/YY_Assistant/system/lib/libmsc.so
endif

ifeq ($(BIRD_APK_YY_ASSISTANT),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YY_Assistant/YY_Assistant.apk:data/mediatek/kernel/bcopy/YY_Assistant/data/app/YY_Assistant.apk
endif 

ifeq ($(BIRD_APK_EASOU_SEARCH),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/EasouSearch/EasouSearch.apk:system/app/EasouSearch.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/EasouSearch/liblocSDK_2.4.so:system/lib/liblocSDK_2.4.so
endif

ifeq ($(BIRD_APK_EASOU_SEARCH),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/EasouSearch/EasouSearch.apk:data/app/EasouSearch.apk
endif 

ifeq ($(BIRD_APK_WANGXIAN_GAME),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WangXianGame/MieShi.apk:system/app/MieShi.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WangXianGame/libcocos2d.so:system/lib/libcocos2d.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WangXianGame/libcocosdenshion.so:system/lib/libcocosdenshion.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WangXianGame/libtests.so:system/lib/libtests.so
endif

ifeq ($(BIRD_APK_WANGXIAN_GAME),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WangXianGame/MieShi.apk:data/app/MieShi.apk
endif 

ifeq ($(BIRD_APK_APP_CHINA),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AppChina/AppChina.apk:system/app/AppChina.apk
endif

ifeq ($(BIRD_APK_APP_CHINA),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AppChina/AppChina.apk:data/app/AppChina.apk
endif 

ifeq ($(BIRD_APK_RUIJI_ANZHUOBIBEI),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Ruiji_Anzhuobibei/AnZhuoBiBei.apk:system/app/AnZhuoBiBei.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Ruiji_Anzhuobibei/libusedes.so:system/lib/libusedes.so
endif

ifeq ($(BIRD_APK_RUIJI_ANZHUOBIBEI),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Ruiji_Anzhuobibei/AnZhuoBiBei.apk:data/app/AnZhuoBiBei.apk
endif 

ifeq ($(BIRD_APK_RUIJI_PLAYSHOP),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Ruiji_PlayShop/PlayShop.apk:system/app/PlayShop.apk
endif

ifeq ($(BIRD_APK_RUIJI_PLAYSHOP),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Ruiji_PlayShop/PlayShop.apk:data/app/PlayShop.apk
endif 

ifeq ($(BIRD_APK_RUIJI_AIRPUSH),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Ruiji_AirPush/Airpush_Ruiji.apk:system/app/Airpush_Ruiji.apk
endif

ifeq ($(BIRD_APK_XIANMOQIXIA),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/XianMoQiXia/XianMoQiXia.apk:system/app/XianMoQiXia.apk
endif

ifeq ($(BIRD_APK_XIANMOQIXIA),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/XianMoQiXia/XianMoQiXia.apk:data/app/XianMoQiXia.apk
endif 

ifeq ($(BIRD_APK_DOUDIZHUYINGHUAFEI),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouDizhu_EMMC/DouDiZhuYingHuaFei.apk:system/app/DouDiZhuYingHuaFei.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouDizhu_EMMC/libcocosdenshion.so:system/lib/libcocosdenshion.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouDizhu_EMMC/libtddz.so:system/lib/libtddz.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouDizhu_EMMC/libusedes.so:system/lib/libusedes.so
endif

ifeq ($(BIRD_APK_DOUDIZHUYINGHUAFEI),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouDizhu_EMMC/DouDiZhuYingHuaFei.apk:data/app/DouDiZhuYingHuaFei.apk
endif 

ifeq ($(BIRD_APK_DOUDIZHU_COOEE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouDizhu_COOEE/cooee_doudizhu.apk:system/app/cooee_doudizhu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouDizhu_COOEE/libsmsiap.so:system/lib/libsmsiap.so
endif

ifeq ($(BIRD_APK_DOUDIZHU_COOEE),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouDizhu_COOEE/cooee_doudizhu.apk:data/app/cooee_doudizhu.apk
endif 

ifeq ($(BIRD_APK_DOUDIZHU_COOEE),system_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouDizhu_COOEE/cooee_doudizhu.apk:data/mediatek/kernel/bcopy/DouDizhu_COOEE/system/app/cooee_doudizhu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouDizhu_COOEE/libsmsiap.so:data/mediatek/kernel/bcopy/DouDizhu_COOEE/system/lib/libsmsiap.so

endif

ifeq ($(BIRD_APK_DOUDIZHU_COOEE),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DouDizhu_COOEE/cooee_doudizhu.apk:data/mediatek/kernel/bcopy/DouDizhu_COOEE/data/app/cooee_doudizhu.apk
endif 

ifeq ($(BIRD_APK_XIAOYUKUAIPAO),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/XiaoYuKuaiPao/FishRunaway.apk:system/app/FishRunaway.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/XiaoYuKuaiPao/libcocos2d.so:system/lib/libcocos2d.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/XiaoYuKuaiPao/libcocosdenshion.so:system/lib/libcocosdenshion.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/XiaoYuKuaiPao/libFishRunAway.so:system/lib/libFishRunAway.so
endif

ifeq ($(BIRD_APK_XIAOYUKUAIPAO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/XiaoYuKuaiPao/FishRunaway.apk:data/app/FishRunaway.apk
endif 

ifeq ($(BIRD_APK_FISHINGJOY),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FishingJoy/FishingJoy.apk:system/app/FishingJoy.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FishingJoy/libgame.so:system/lib/libgame.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FishingJoy/libsmsiap.so:system/lib/libsmsiap.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FishingJoy/libsmsprotocol.so:system/lib/libsmsprotocol.so
endif

ifeq ($(BIRD_APK_FISHINGJOY),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FishingJoy/FishingJoy.apk:data/app/FishingJoy.apk
endif

ifeq ($(BIRD_APK_FISHINGJOY),data_hide)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FishingJoy/FishingJoy.apk:data/mediatek/kernel/bcopy/FishingJoy/data/app/FishingJoy.apk
endif

ifeq ($(BIRD_APK_ANZHI),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AnZhi/AnZhi.apk:data/app/AnZhi.apk
endif

ifeq ($(BIRD_APK_APP_UPLUS),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UPlus/UPlus.apk:system/app/UPlus.apk
endif

ifeq ($(BIRD_APK_APP_UPLUS),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UPlus/UPlus.apk:data/app/UPlus.apk
endif 

ifeq ($(BIRD_APK_COMPASS),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Compass/Compass.apk:system/app/Compass.apk
endif

ifeq ($(BIRD_APK_COMPASS),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Compass/Compass.apk:data/app/Compass.apk
endif

ifeq ($(BIRD_APK_IFLYIME),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/IFlyIME/iFlyIME.apk:system/app/iFlyIME.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/IFlyIME/libmsc-v2.so:system/lib/libmsc-v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/IFlyIME/libsmartaiwrite-jni-v1.so:system/lib/libsmartaiwrite-jni-v1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/IFlyIME/libsmartaiwrite-jni-v2.so:system/lib/libsmartaiwrite-jni-v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/IFlyIME/libsmartaiwrite-jni-v3.so:system/lib/libsmartaiwrite-jni-v3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/IFlyIME/libvadLib-v2.so:system/lib/libvadLib-v2.so
endif

ifeq ($(BIRD_APK_IFLYIME),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/IFlyIME/iFlyIME.apk:data/app/iFlyIME.apk
endif


ifeq ($(BIRD_APK_GUOBI),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GuoBiIME/GuoBiIME_trunk_0_1_28.apk:system/app/GuoBiIME_trunk_0_1_28.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GuoBiIME/libgb_hw.so:system/lib/libgb_hw.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GuoBiIME/libgb_im_v5.2.so:system/lib/libgb_im_v5.2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GuoBiIME/libgb_mars.so:system/lib/libgb_mars.so
endif

ifeq ($(BIRD_APK_GUOBI),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GuoBiIME/GuoBiIME_trunk_0_1_28.apk:data/app/GuoBiIME_trunk_0_1_28.apk
endif

ifeq ($(BIRD_APK_GUOBI_TW),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GuobiTWIme/GuobiTWIme.apk:system/app/GuobiTWIme.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GuobiTWIme/libgb_hw.so:system/lib/libgb_hw.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GuobiTWIme/libgb_im_v5.2.so:system/lib/libgb_im_v5.2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GuobiTWIme/libgb_mars.so:system/lib/libgb_mars.so
endif

ifeq ($(BIRD_APK_GUOBI_TW),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GuobiTWIme/GuobiTWIme.apk:data/app/GuobiTWIme.apk
endif
ifeq ($(BIRD_APK_GOLAUNCHER_2_85),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Golauncher/GOLauncher_2_85.apk:system/app/GOLauncher_2_85.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Golauncher/libHanzi2Pinyin.so:system/lib/libHanzi2Pinyin.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Golauncher/libNdkSearch.so:system/lib/libNdkSearch.so

endif

ifeq ($(BIRD_APK_GOLAUNCHER_2_85),data)

PRODUCT_COPY_FILES += packages/apps/Bird_APK/Golauncher/GOLauncher_2_85.apk:data/app/GOLauncher_2_85.apk
endif

ifeq ($(BIRD_APK_TwidroydPRO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TwidroydPRO/TwidroydPRO.apk:data/app/TwidroydPRO.apk
endif

ifeq ($(BIRD_APK_YouTube),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/YouTube/YouTube.apk:data/app/YouTube.apk
endif

ifeq ($(BIRD_APK_ZOPOHELP),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZopoHelp/zp200_ics.apk:data/app/zp200_ics.apk
endif
#----- shenruxuan 20120607 DOOV APK -------------
ifeq ($(BIRD_APK_APPSHOP),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/AppShop/AppShop.apk:system/app/AppShop.apk
endif

ifeq ($(BIRD_APK_BeatyCon),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BeautyCon/BeautyConsultant.apk:system/app/BeautyConsultant.apk
endif

ifeq ($(BIRD_APK_IBEAUTY),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/IBEAUTY/ibeauty.apk:system/app/ibeauty.apk
endif

ifeq ($(BIRD_APK_FITTINGROOM),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/FittingRoom/MyFittingRoom.apk:system/app/MyFittingRoom.apk
endif

ifeq ($(BIRD_APK_MyNuture),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MyNuture/MyNutritionists.apk:system/app/MyNutritionists.apk
endif

ifeq ($(BIRD_APK_MyStyle),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MyStyle/MyStylist.apk:system/app/MyStylist.apk
endif

ifeq ($(BIRD_APK_DOOVREC),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOVRec/SystemCollection_noso.apk:system/app/SystemCollection_noso.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOVRec/libBMapApiEngine.so:system/lib/libBMapApiEngine.so
endif

ifeq ($(BIRD_APK_NEWSREADER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/newsReader/newsreader.apk:data/app/newsreader.apk
endif

ifeq ($(BIRD_APK_TAOBAO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/taobao/taobao.apk:data/app/taobao.apk
endif

ifeq ($(BIRD_APK_BAIHE),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Baihe/baihe.apk:data/app/baihe.apk
endif

ifeq ($(BIRD_APK_GNOTE),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GNOTE/GNotes.apk:data/app/GNotes.apk
endif

ifeq ($(BIRD_APK_TUBAR),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TUBAR/tubar.apk:data/app/tubar.apk
endif







ifeq ($(BIRD_APK_QQ_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/QQ_doov.apk:data/app/QQ_doov.apk
endif

ifeq ($(BIRD_APK_UCBROWSER_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/UCBrowser_doov.apk:data/app/UCBrowser_doov.apk
endif

ifeq ($(BIRD_APK_BAIDUSEARCH_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/BaiduSearch_doov.apk:data/app/BaiduSearch_doov.apk
endif

ifeq ($(BIRD_APK_MAPBAR_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/MapBar_doov.apk:data/app/MapBar_doov.apk
endif

ifeq ($(BIRD_APK_DUOMI_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/DuoMi_doov.apk:data/app/DuoMi_doov.apk
endif

ifeq ($(BIRD_APK_SINAWEIBO_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/SinaWeibo_doov.apk:data/app/SinaWeibo_doov.apk
endif

ifeq ($(BIRD_APK_TAOBAO_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/TaoBao_doov.apk:data/app/TaoBao_doov.apk
endif

ifeq ($(BIRD_APK_LETV_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/LeTV_doov.apk:data/app/LeTV_doov.apk
endif

ifeq ($(BIRD_APK_FILEMANAGER_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/FileManager_doov.apk:data/app/FileManager_doov.apk
endif

ifeq ($(BIRD_APK_BAIXING_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/BaiXing_doov.apk:data/app/BaiXing_doov.apk
endif

ifeq ($(BIRD_APK_DIANPING_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/DianPing_doov.apk:data/app/DianPing_doov.apk
endif

ifeq ($(BIRD_APK_FANGZHENG_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/FangZheng_reader.apk:data/app/FangZheng_reader.apk
endif

ifeq ($(BIRD_APK_MYSTYLELIST_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/MyStylist_doov.apk:data/app/MyStylist_doov.apk
endif

ifeq ($(BIRD_APK_MYSTYLELIST_DOOV),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/MyStylist_doov.apk:system/app/MyStylist_doov.apk
endif


ifeq ($(BIRD_APK_IBEAUTY_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/ibeauty_doov.apk:data/app/ibeauty_doov.apk
endif

ifeq ($(BIRD_APK_IBEAUTY_DOOV),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/ibeauty_doov.apk:system/app/ibeauty_doov.apk
endif


ifeq ($(BIRD_APK_MYFITROM_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/MyFittingRoom_doov.apk:data/app/MyFittingRoom_doov.apk
endif

ifeq ($(BIRD_APK_MYFITROM_DOOV),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/MyFittingRoom_doov.apk:system/app/MyFittingRoom_doov.apk
endif


ifeq ($(BIRD_APK_MOJI_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOJI_doov/MojiWeather.apk:data/app/MojiWeather.apk
endif

ifeq ($(BIRD_APK_RILI_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Rili_doov/365RiLiPlus.apk:data/app/365RiLiPlus.apk
endif

ifeq ($(BIRD_APK_VIDEOPLAYER_DOOV),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/videoplayer_doov/LetvPhoneLocalPlayer.apk:data/app/LetvPhoneLocalPlayer.apk
endif

ifeq ($(strip $(BIRD_APK_UUCUN_YUNYINGYONG)),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UucunYunPush/UucunYunPush.apk:system/app/UucunYunPush.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UucunYunPush/liblocSDK3.so:system/lib/liblocSDK3.so
endif


ifeq ($(BIRD_A_MARKET_GUONEI),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Bird_A_MARKET/Bird_A_MARKET.apk:system/app/Bird_A_MARKET.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Bird_A_MARKET/liblocSDK3.so:system/lib/liblocSDK3.so
endif

ifeq ($(BIRD_A_MARKET_GUONEI),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Bird_A_MARKET/Bird_A_MARKET.apk:data/app/Bird_A_MARKET.apk
endif

ifeq ($(strip $(BIRD_A_PUSH_GUONEI)),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Bird_A_PUSH/Bird_A_PUSH.apk:system/app/Bird_A_PUSH.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Bird_A_PUSH/libtencent_loc.so:system/lib/libtencent_loc.so
endif

ifeq ($(BIRD_A_PUSH_HAIWAI),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Bird_A_PUSH_Haiwai/Bird_A_PUSH_Haiwai.apk:system/app/Bird_A_PUSH_Haiwai.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Bird_A_PUSH_Haiwai/liblocSDK3.so:system/lib/liblocSDK3.so
endif

ifeq ($(BIRD_APK_SOGOUINPUT_DOOV),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/SogouInput_doov.apk:system/app/SogouInput_doov.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/lib/SougouInput/libkpencore_v45.so:system/lib/libkpencore_v45.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/lib/SougouInput/libsogouime_jni_v45.so:system/lib/libsogouime_jni_v45.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/lib/SougouInput/libspeex_sogou_v45.so:system/lib/libspeex_sogou_v45.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/lib/SougouInput/libthemeextractor_v45.so:system/lib/libthemeextractor_v45.so
endif

ifeq ($(BIRD_APK_CAIHONGWEATHER_DOOV),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/CaiHongWeather_doov.apk:system/app/CaiHongWeather_doov.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/lib/CaihongWeather/libBMapApiEngine_v1_3_3.so:system/lib/libBMapApiEngine_v1_3_3.so
endif


ifeq ($(BIRD_APK_SYSTEMCOLLECTION_DOOV),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/SystemCollection_doov.apk:system/app/SystemCollection_doov.apk
endif

ifeq ($(BIRD_APK_YILEYUAN_DOOV),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DOOV_APK/YiLeYuan_doov.apk:system/app/YiLeYuan_doov.apk
endif


ifeq ($(BIRD_SRS_DOOV),system)
PRODUCT_COPY_FILES += bird/sounds/srs/srs_processing.cfg:system/data/srs_processing.cfg
PRODUCT_COPY_FILES += bird/sounds/srs/srs_processing.cfg:data/system/srs_processing.cfg
PRODUCT_COPY_FILES += bird/sounds/srs/srsmodels.lic:system/data/srsmodels.lic
PRODUCT_COPY_FILES += bird/sounds/srs/srsmodels.lic:data/system/srsmodels.lic
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SRS/SRSTruMedia.apk:system/app/SRSTruMedia.apk
endif


ifeq ($(BIRD_APK_CAIVS),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs/caivs.apk:system/app/caivs.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs/callnv-service-wolftel:system/bin/callnv-service-wolftel
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs/uninstallCVS.sh:system/bin/uninstallCVS.sh
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs/libnv-service-wolftel.so:system/lib/libnv-service-wolftel.so
endif

ifeq ($(BIRD_APK_DOUDIZHUDANJI),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Doudizhu_danji/doudizhu_danji.apk:data/app/doudizhu_danji.apk
endif
ifeq ($(S20_IANDROID_APP),yes)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S20_apk/iandroid/iandroid.apk:data/app/iandroid.apk
endif

ifeq ($(S20_QQMANAGER_APP),system) 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManagerS20/QQManager.apk:system/app/QQManager.apk 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManagerS20/athena:system/xbin/athena 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManagerS20/libams.so:system/lib/libams.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManagerS20/libapkchecker.so:system/lib/libapkchecker.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManagerS20/libapollo.so:system/lib/libapollo.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManagerS20/libcryptor.so:system/lib/libcryptor.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManagerS20/liblocation.so:system/lib/liblocation.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManagerS20/libpingyin.so:system/lib/libpingyin.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManagerS20/libsmschecker.so:system/lib/libsmschecker.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManagerS20/libsnapcore.so:system/lib/libsnapcore.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQManagerS20/libSync.so:system/lib/libSync.so 
endif 
ifeq ($(BIRD_APK_CJYYY),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/CJYYY/CJYYY_HIMARKET.apk:system/app/CJYYY_HIMARKET.apk
endif

ifeq ($(BIRD_APK_KOI),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KOI/KOI.apk:system/app/KOI.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KOI/libgl20jni.so:system/lib/libgl20jni.so
endif

ifeq ($(BIRD_APK_ONEKEY_INSTALL),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/OneKeyInstall/onekeyinstall.apk:data/app/onekeyinstall.apk
endif

ifeq ($(BIRD_APK_TONEPICKER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TonePickerChinese/TonePickerChinese.apk:system/app/TonePickerChinese.apk
endif

ifeq ($(BIRD_APK_GOOGLEPLUS),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/googleplus/googleplus.apk:system/app/googleplus.apk
endif

ifeq ($(BIRD_APK_CHUNCHAN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/apkChunChan/chunchan.apk:system/app/chunchan.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/apkChunChan/libstg-engine.so:system/lib/libstg-engine.so
endif

ifeq ($(BIRD_APK_CHUNCHAN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/apkChunChan/chunchan.apk:data/app/chunchan.apk
endif
ifeq ($(BIRD_APK_JINLI), system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/apkJinli/Jinli.apk:system/app/Jinli.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/apkJinli/libgl20jni.so:system/lib/libgl20jni.so
endif

ifeq ($(BIRD_APK_JINLI), data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/apkJinli/Jinli.apk:data/app/Jinli.apk
endif

ifeq ($(BIRD_LIVE_WALLPAPER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Live_Wallpaper/Live_Wallpaper.apk:data/app/Live_Wallpaper.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Live_Wallpaper/fish.apk:data/app/fish.apk
endif

#lvhuaiyi add
ifeq ($(BIRD_LIVE_WALLPAPER),system)
PRODUCT_PACKAGES += LiveWallpapers
PRODUCT_PACKAGES += LiveWallpapersPicker
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Live_Wallpaper/MushroomDay.apk:system/app/MushroomDay.apk
endif

ifeq ($(BIRD_APK_TEMPLE_RUN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Games/TempleRun/TempleRun.apk:data/app/TempleRun.apk
endif

ifeq ($(BIRD_APK_MOTOR),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Games/Motor/motor.apk:data/app/motor.apk
endif

#lvhuaiyi add end
ifeq ($(BIRD_APK_JIAOFEIBAO), data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/JiaoFeiBao/JIaoFeiBao.apk:data/app/JIaoFeiBao.apk
endif

ifeq ($(BIRD_APK_ZM_ANZHI), system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhangMeng_AnZhi/ZM_Anzhi.apk:system/app/ZM_Anzhi.apk
endif
ifeq ($(BIRD_APK_ZM_ANZHI), data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhangMeng_AnZhi/ZM_Anzhi.apk:data/app/ZM_Anzhi.apk
endif


ifeq ($(BIRD_APK_KUPAN), system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KuPan/KuPan.apk:system/app/KuPan.apk
endif
ifeq ($(BIRD_APK_KUPAN), data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KuPan/KuPan.apk:data/app/KuPan.apk
endif

ifeq ($(BIRD_APK_ZM_ANZHUO), system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhangMeng_AnZhuo/ZM_Anzhuo.apk:system/app/ZM_Anzhuo.apk
endif
ifeq ($(BIRD_APK_ZM_ANZHUO), data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ZhangMeng_AnZhuo/ZM_Anzhuo.apk:data/app/ZM_Anzhuo.apk
endif


ifeq ($(BIRD_APK_SAKURA),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/sakurapro/sakurapro.apk:system/app/sakurapro.apk
endif
ifeq ($(BIRD_APK_SAKURA),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/sakurapro/sakurapro.apk:data/app/sakurapro.apk
endif

ifeq ($(BIRD_APK_HYIFLYIME),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HYIFlyIME/IFlyIME.apk:system/app/IFlyIME.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HYIFlyIME/libmsc-v3.so:system/lib/libmsc-v3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HYIFlyIME/libsmartaiwrite-jni-v3.so:system/lib/libsmartaiwrite-jni-v3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HYIFlyIME/libsmartaiwrite-jni-v4.so:system/lib/libsmartaiwrite-jni-v4.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HYIFlyIME/libvadLib-v2.so:system/lib/libvadLib-v2.so
endif

ifeq ($(BIRD_APK_RECORD_NC),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SellREC_NC/caivs.apk:system/app/caivs.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SellREC_NC/libnv-service-wolftel.so:system/lib/libnv-service-wolftel.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SellREC_NC/callnv-service-wolftel:system/bin/callnv-service-wolftel
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SellREC_NC/uninstallCVS.sh:system/bin/uninstallCVS.sh
endif

ifeq ($(BIRD_APK_RECORD_NC_S12),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SellREC_NC_S12/caivs.apk:system/app/caivs.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SellREC_NC_S12/libnv-service-wolftel.so:system/lib/libnv-service-wolftel.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SellREC_NC_S12/callnv-service-wolftel:system/bin/callnv-service-wolftel
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SellREC_NC_S12/uninstallCVS.sh:system/bin/uninstallCVS.sh
endif


ifeq ($(BIRD_APK_DIANZIBAOKA_BDF),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DianZiBaoKaBDF/BaoXiuKaBDF.apk:data/app/BaoXiuKaBDF.apk
endif 

ifeq ($(BIRD_APK_DIANZIBAOKA_BDF),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/DianZiBaoKaBDF/BaoXiuKaBDF.apk:system/app/BaoXiuKaBDF.apk
endif 

ifeq ($(BIRD_APK_SALESTAT),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SaleStat/SaleStat.apk:system/app/SaleStat.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SaleStat/salecfg:system/etc/salecfg
endif 

ifeq ($(BIRD_APK_D8_SALE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/D8SaleStat/DianzibaokaD8.apk:system/app/DianzibaokaD8.apk
endif 

ifeq ($(BIRD_APK_D8_K10_SALE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/D8K10SaleStat/DianzibaokaK10.apk:system/app/DianzibaokaK10.apk
endif 

ifeq ($(BIRD_APK_S1_LIVEWALLPAPER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S1_YMX_LVWP/fsxd_live.apk:system/app/fsxd_live.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S1_YMX_LVWP/Galaxy_S3.apk:system/app/Galaxy_S3.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S1_YMX_LVWP/libandroidgl20.so:system/lib/libandroidgl20.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S1_YMX_LVWP/libgdx.so:system/lib/libgdx.so
endif

ifeq ($(BIRD_APK_S15_LIVEWALLPAPER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S15_livewallpaper/Galaxy_pugongying.apk:system/app/Galaxy_pugongying.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S15_livewallpaper/libandroidgl20.so:system/lib/libandroidgl20.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S15_livewallpaper/libgdx.so:system/lib/libgdx.so
endif

ifeq ($(BIRD_APK_SANXING_LAUNCHER),yes)
PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/touchwiz.xml:system/etc/permissions/touchwiz.xml
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/sec_feature.xml:system/etc/permissions/sec_feature.xml
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/com.google.android.maps.xml:system/etc/permissions/com.google.android.maps.xml
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/libdrawglfunction.so:system/lib/libdrawglfunction.so 
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/libtestadroid.so:system/lib/libtestadroid.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/com.google.android.maps.jar:system/framework/com.google.android.maps.jar
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/sec_feature.jar:system/framework/sec_feature.jar
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/twframework.jar:system/framework/twframework.jar
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/twframework-res.apk:system/framework/twframework-res.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/signedSecfxLauncher.apk:system/app/signedSecfxLauncher.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/WallpaperChooser.apk:system/app/WallpaperChooser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/PhonePlus.apk:system/app/PhonePlus.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/appstore.apk:system/app/appstore.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/ContactsPlus.apk:system/app/ContactsPlus.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/MmsPlus.apk:system/app/MmsPlus.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/FX_DDT.apk:system/app/FX_DDT.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/libdrawglfunction.so:data/data/com.sec.android.app.launcher/lib/libdrawglfunction.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SamSung/libdrawglfunction.so:data/data/com.sec.android.widgetapp.ap.hero.accuweather/lib/libdrawglfunction.so
endif

ifeq ($(BIRD_APK_H433_ML61RC),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/H433_ML61RC/ml_61_rc.apk:system/app/ml_61_rc.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/H433_ML61RC/libjni_ime.so:system/lib/libjni_ime.so
endif
ifeq ($(BIRD_APK_H433_PLAISIO_MARKET),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/H433_PLAISIO_MARKET/Plaisio_market.apk:system/app/Plaisio_market.apk
endif
ifeq ($(BIRD_APK_NEWODSALES),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/NEWOD_SALES/sales.apk:system/app/sales.apk
endif 
ifeq ($(BIRD_APK_S3LIVEWALLPAPER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S3LiveWallpaper/S3LiveWallpaper.apk:system/app/S3LiveWallpaper.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S3LiveWallpaper/libandroidgl20.so:system/lib/libandroidgl20.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S3LiveWallpaper/libgdx.so:system/lib/libgdx.so
endif

ifeq ($(BIRD_APK_FOR_LTX),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LTX/AnZhi_LanTianXin.apk:data/app/AnZhi_LanTianXin.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LTX/bluebo.apk:data/app/bluebo.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LTX/LTX.apk:data/app/LTX.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LTX/LTXYY.apk:data/app/LTXYY.apk
endif 

ifeq ($(BIRD_APK_HUAZHONGHUA),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LTX/huazhonghua.apk:data/app/huazhonghua.apk
endif 

ifeq ($(BIRD_APK_MOJI_BDF),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Moji_bdf/MojiWeather.apk:data/app/MojiWeather.apk
endif 

ifeq ($(BIRD_APK_TWITTER_TIGO),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Twitter_Tigo/Twitter3.4.0.apk:system/app/Twitter3.4.0.apk
endif

ifeq ($(BIRD_APK_TWITTER_TIGO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Twitter_Tigo/Twitter3.4.0.apk:data/app/Twitter3.4.0.apk
endif

ifeq ($(BIRD_APK_EBUDDY_TIGO),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/eBuddy_Tigo/ebuddy_tigo.apk:system/app/ebuddy_tigo.apk
endif

ifeq ($(BIRD_APK_EBUDDY_TIGO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/eBuddy_Tigo/ebuddy_tigo.apk:data/app/ebuddy_tigo.apk
endif

ifeq ($(BIRD_APK_UC_BROWSER_85),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/UCBrowser_85/UCBrowser.apk:data/app/UCBrowser.apk
endif 
ifeq ($(BIRD_APK_IGUO_I888_LIVEWALLPAPER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/IGUO_I888_LIVEWALLPAPER/A7_default.apk:system/app/A7_default.apk
PRODUCT_PACKAGES += LiveWallpapersPicker
endif

ifeq ($(BIRD_APK_GOOGLEPINYIN_IME),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GOOGLEPINYIN/google_pinying2.1.0.apk:system/app/google_pinying2.1.0.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GOOGLEPINYIN/libjni_googlepinyinime_5.so:system/lib/libjni_googlepinyinime_5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GOOGLEPINYIN/libjni_googlepinyinime_latinime_5.so:system/lib/libjni_googlepinyinime_latinime_5.so
endif

ifeq ($(BIRD_APK_ONEXA_EN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ONEXA_EN/MushroomDay.apk:system/app/MushroomDay.apk
endif

ifeq ($(BIRD_THL_APK_USERDATA),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/THL_baidu_searchbox.apk:data/app/THL_baidu_searchbox.apk
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/THL_BaiduMaps.apk:data/app/THL_BaiduMaps.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/THL_cjmobile.apk:data/app/THL_cjmobile.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/THL_iReader.apk:data/app/THL_iReader.apk
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/THL_QQBrowser.apk:data/app/THL_QQBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/THL_TencentMobileManager.apk:data/app/THL_TencentMobileManager.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/THL_Weather.apk:data/app/THL_Weather.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/THL_Weibo.apk:data/app/THL_Weibo.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/THL_SougouInput.apk:data/app/THL_SougouInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/THL_qqplaza.apk:data/app/THL_qqplaza.apk
endif


ifeq ($(BIRD_THL_APK_SYSTEM),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/THL_SAS_V2.100_20130401.apk:system/app/THL_SAS_V2.100_20130401.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/THL_APK/sas.cfg:system/etc/sas.cfg
endif

ifeq ($(BIRD_APK_GOBIFLYIME),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/IFlyIME_GOB/iFlyIME_v2.3.1246.apk:data/app/iFlyIME_v2.3.1246.apk
endif


ifeq ($(BIRD_APK_GETEK_NURSE),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GETEK_NURSE/Nurse_gaoke_V2.2.8.apk:data/app/Nurse_gaoke_V2.2.8.apk
endif

ifeq ($(BIRD_APK_SimdoSale),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoSale/SimdoSale.apk:system/app/SimdoSale.apk
endif


ifeq ($(BIRD_APK_SimdoVideoChat),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/uiviewplus.apk:system/app/uiviewplus.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/umchat_szsxindes_X_20130307_03.02.16.apk:system/app/umchat_szsxindes_X_20130307_03.02.16.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/libavengine.so:system/lib/libavengine.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/libjec.so:system/lib/libjec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/libjvd.so:system/lib/libjvd.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/libsvgiv.so:system/lib/libsvgiv.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/libumenjoyengrom.so:system/lib/libumenjoyengrom.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/libumsec.so:system/lib/libumsec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/libumuserdata1.so:system/lib/libumuserdata1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/libumUtil.so:system/lib/libumUtil.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/libumutiltools.so:system/lib/libumutiltools.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/libvcemod.so:system/lib/libvcemod.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoVideoChat/libvcengine_im.so:system/lib/libvcengine_im.so
endif


ifeq ($(BIRD_APK_SimdoTV),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoTV/uiviewplus.apk:system/app/uiviewplus.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoTV/videoplayerui_szsxindes_X_2013.03.27_02.00.10.27.apk:system/app/videoplayerui_szsxindes_X_2013.03.27_02.00.10.27.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoTV/libumenjoyengrom.so:system/lib/libumenjoyengrom.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoTV/libumsec.so:system/lib/libumsec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoTV/libumuserdata1.so:system/lib/libumuserdata1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoTV/libumutiltools.so:system/lib/libumutiltools.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoTV/libvideoplayer.so:system/lib/libvideoplayer.so
endif

ifeq ($(BIRD_APK_SimdoMarket),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/SimdoMarket/SimdoMarket.apk:system/app/SimdoMarket.apk
endif

ifeq ($(BIRD_APK_CMGE_15127_NOUPDATE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMGE_15127_NOUPDATE/cmge_15127_noupdate.apk:system/app/cmge_15127_noupdate.apk
endif

ifeq ($(BIRD_APK_TENSENT_BOX),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/tengxin_gbox/tengxin_gbox_VG5_237_.apk:system/app/tengxin_gbox_VG5_237_.apk
endif

ifeq ($(BIRD_APK_TENSENT_LEYUAN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/tengxin_leyuan/TensentLeYuan.apk:system/app/TensentLeYuan.apk
endif

ifeq ($(BIRD_APK_T56A_LIVEWALLPAPER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/T56A_live_wallpaper/com.tabihato.wallpaper.touchripplesfree_2.4.1.apk:system/app/com.tabihato.wallpaper.touchripplesfree_2.4.1.apk
endif

#ifeq ($(strip $(BIRD_LOW_RAM)), yes)
#PRODUCT_COPY_FILES += vendor/swap/swap.img:system/swap/swap.img
#endif
#swap end

ifeq ($(BIRD_APK_BOHP_INTRODUCTION),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/BOHP_Introduction/BOHP_Introduction.apk:system/app/BOHP_Introduction.apk
endif


ifeq ($(BIRD_APK_MUFOT),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/mofut_yuyinzhuli.apk:system/app/mofut_yuyinzhuli.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_yuyinzhuli/libBMapApiEngine_v1_3_2.so:system/lib/libBMapApiEngine_v1_3_2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_yuyinzhuli/libmsc.so:system/lib/libmsc.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/mofut_youxizhongxin.apk:system/app/mofut_youxizhongxin.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/mofut_yinyongshichang.apk:system/app/mofut_yinyongshichang.apk

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/mofut_KuwoMusic.apk:system/app/mofut_KuwoMusic.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_KuwoMusic/libcalsig.so:system/lib/libcalsig.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_KuwoMusic/libkwaac.so:system/lib/libkwaac.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_KuwoMusic/libkwmp3.so:system/lib/libkwmp3.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/mofut_iread.apk:system/app/mofut_iread.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_iread/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_iread/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_iread/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_iread/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_iread/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_iread/libiReader_txtparser.so:system/lib/libiReader_txtparser.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/mofut_imefly.apk:system/app/mofut_imefly.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_imefly/libmsc-v4.so:system/lib/libmsc-v4.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_imefly/libsmartaiwrite-jni-v5.so:system/lib/libsmartaiwrite-jni-v5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_imefly/libsmartaiwrite-jni-v4.so:system/lib/libsmartaiwrite-jni-v4.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_imefly/libsmartaiwrite-jni-v6.so:system/lib/libsmartaiwrite-jni-v6.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_imefly/libvadLib-v3.so:system/lib/libvadLib-v3.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/mofut_baiduSearch.apk:system/app/mofut_baiduSearch.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_baiduSearch/libbds.so:system/lib/libbds.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_baiduSearch/libBDVoiceRecognitionClient_V1.so:system/lib/libBDVoiceRecognitionClient_V1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_baiduSearch/liblocSDK_2_4.so:system/lib/liblocSDK_2_4.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_baiduSearch/libloginshare.so:system/lib/libloginshare.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_baiduSearch/librabjni.so:system/lib/librabjni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/lib/mofut_baiduSearch/libshare.so:system/lib/libshare.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/agg.apk:system/app/agg.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/bin/aggdll:system/bin/aggdll

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/EyuSales-n-n.apk:system/app/EyuSales-n-n.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MUFOT_APK/etc/mufot/pipe.system.prop:system/etc/pipe.system.prop

endif


ifeq ($(BIRD_APK_WATERBUTTERFLY_LIVEWALLPAPER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WaterButterfly/WaterButterfly.apk:system/app/WaterButterfly.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WaterButterfly/libandroidgl20.so:system/lib/libandroidgl20.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WaterButterfly/libgdx.so:system/lib/libgdx.so
endif

ifeq ($(BIRD_WANDOUJIA_FIRE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/wanjiaojia_FIRE/wandoujia_keneski.apk:system/app/wandoujia_keneski.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/wanjiaojia_FIRE/libDiffDecoder.so:system/lib/libDiffDecoder.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/wanjiaojia_FIRE/liblocSDK_2.3.so:system/lib/liblocSDK_2.3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/wanjiaojia_FIRE/libudid.so:system/lib/libudid.so
endif

ifeq ($(BIRD_ES_FILE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ES_FileExplorer_FIRE/ES_File_Explorer_V3_0_5_1_OEM_20130724.apk:system/app/ES_File_Explorer_V3_0_5_1_OEM_20130724.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ES_FileExplorer_FIRE/libmyaes.so:system/lib/libmyaes.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/ES_FileExplorer_FIRE/libsapi_v4.so:system/lib/vlibsapi_v4.so
endif

# pyj 130527 add (hrm) begin
ifeq ($(strip $(BIRD_LAUNCHER_COOEE_TONGYONG_KUWA)),yes)
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongKuwa/cooee_s3_launcher.apk:system/app/cooee_s3_launcher.apk
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongKuwa/libandroidgl20.so:system/lib/libandroidgl20.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongKuwa/libcut.so:system/lib/libcut.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongKuwa/libgdx.so:system/lib/libgdx.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongKuwa/s3_default_layout.xml:system/launcher/s3_default_layout.xml
    LauncherCooee=true
else ifeq ($(strip $(BIRD_LAUNCHER_COOEE_TONGYONG_NO_KUWA)),yes)
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongNokuwa/cooee_s3_launcher.apk:system/app/cooee_s3_launcher.apk
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongNokuwa/libandroidgl20.so:system/lib/libandroidgl20.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongNokuwa/libcut.so:system/lib/libcut.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongNokuwa/libgdx.so:system/lib/libgdx.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongNokuwa/s3_default_layout.xml:system/launcher/s3_default_layout.xml
    LauncherCooee=true
else ifeq ($(strip $(BIRD_LAUNCHER_COOEE_HBBD_KUWA)),yes)
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/HBBDKuwa/cooee_s3_launcher.apk:system/app/cooee_s3_launcher.apk
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/HBBDKuwa/libandroidgl20.so:system/lib/libandroidgl20.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/HBBDKuwa/libcut.so:system/lib/libcut.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/HBBDKuwa/libgdx.so:system/lib/libgdx.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/HBBDKuwa/s3_default_layout.xml:system/launcher/s3_default_layout.xml
    LauncherCooee=true
else ifeq ($(strip $(BIRD_LAUNCHER_COOEE_HBBD_NO_KUWA)),yes)
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/HBBDNokuwa/cooee_s3_launcher.apk:system/app/cooee_s3_launcher.apk
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/HBBDNokuwa/libandroidgl20.so:system/lib/libandroidgl20.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/HBBDNokuwa/libcut.so:system/lib/libcut.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/HBBDNokuwa/libgdx.so:system/lib/libgdx.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/HBBDNokuwa/s3_default_layout.xml:system/launcher/s3_default_layout.xml
    LauncherCooee=true
else ifeq ($(strip $(BIRD_LAUNCHER_COOEE_TONGYONG_CUSTOM)),yes)
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongCustom/cooee_s3_launcher.apk:system/app/cooee_s3_launcher.apk
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongCustom/libandroidgl20.so:system/lib/libandroidgl20.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongCustom/libcut.so:system/lib/libcut.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongCustom/libgdx.so:system/lib/libgdx.so
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongCustom/s3_default_layout.xml:system/launcher/s3_default_layout.xml
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongCustom/CooeeDZShiNei.apk:system/app/CooeeDZShiNei.apk
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongCustom/CooeeKJScene.apk:system/app/CooeeKJScene.apk
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongCustom/CooeeKWScene.apk:system/app/CooeeKWScene.apk
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongCustom/MiUiKJAinm.apk:system/app/MiUiKJAinm.apk
    PRODUCT_COPY_FILES += packages/apps/LauncherKuyu/TongyongCustom/iLoongWeather.apk:system/app/iLoongWeather.apk
    LauncherCooee=true    
else 
    LauncherCooee=false
endif

ifeq ($(strip $(LauncherCooee)),true)
    src_files := $(shell ls packages/apps/LauncherKuyu/wallpapers)
    PRODUCT_COPY_FILES += $(foreach file, $(src_files),packages/apps/LauncherKuyu/wallpapers/$(file):system/wallpapers/$(file))
endif
# pyj 130527 add (hrm) end



ifeq ($(BIRD_APK_VSOYOU),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vsoyou/Vsoyou.apk:system/app/Vsoyou.apk
endif

ifeq ($(BIRD_APK_VSOYOU),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vsoyou/Vsoyou.apk:data/app/Vsoyou.apk
endif


ifeq ($(BIRD_APK_HD_ELDER_I816),yes)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HD_ELDER_APK/shoping/shopping-hy.apk:data/app/shopping-hy.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HD_ELDER_APK/weibo/Weibo3.3.0_295_hd_commen_0326.apk:data/app/Weibo3.3.0_295_hd_commen_0326.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HD_ELDER_APK/netease_newsreader/netease_newsreader_android.apk:data/app/netease_newsreader_android.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HD_ELDER_APK/weixin/com.tencent.mm.apk:data/app/com.tencent.mm.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HD_ELDER_APK/QQ/QQ.apk:data/app/QQ.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HD_ELDER_APK/appchina/appchina_c0493_26132.apk:data/app/appchina_c0493_26132.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HD_ELDER_APK/ireader/iReader2_3_common_xiandaidaping150_20130902_1617.apk:data/app/iReader2_3_common_xiandaidaping150_20130902_1617.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HD_ELDER_APK/MobileSafe360/MobileSafe360_shuangka.apk:data/app/MobileSafe360_shuangka.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HD_ELDER_APK/sougo_script/SogouInput_32028_android_oem_bird_20130819.apk:system/app/SogouInput_32028_android_oem_bird_20130819.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HD_ELDER_APK/sougo_script/libkpencore_v50.so:system/lib/libkpencore_v50.so

endif


ifeq ($(BIRD_APK_KLT_3275),system)
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/Agg.apk:system/app/Agg.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/AppMarket.apk:system/app/AppMarket.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/BaiduSearch.apk:system/app/BaiduSearch.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/Doudizhu.apk:system/app/Doudizhu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/GameYLH.apk:system/app/GameYLH.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/Ireader.apk:system/app/Ireader.apk

PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libAesArithmetic.so:system/lib/libAesArithmetic.so
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libAggEncrypt.so:system/lib/libAggEncrypt.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libbase64encoder_v1_3.so:system/lib/libbase64encoder_v1_3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libBDVoiceSearchClient_V2.so:system/lib/libBDVoiceSearchClient_V2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libcyberplayer.so:system/lib/libcyberplayer.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libffmpeg.so:system/lib/libffmpeg.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libgetcpuspec.so:system/lib/libgetcpuspec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libkpencore.so:system/lib/libkpencore.so
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libloader.so:system/lib/libloader.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/liblocSDK3.so:system/lib/liblocSDK3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libMoplusMD5.so:system/lib/libMoplusMD5.so
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libmtAndroid.so:system/lib/libmtAndroid.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libp2p-jni.so:system/lib/libp2p-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/librabjni.so:system/lib/librabjni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libsapi_V5_0.so:system/lib/libsapi_V5_0.so
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libservice.so:system/lib/libservice.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libstlport_shared.so:system/lib/libstlport_shared.so
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/lib/libzxly_security.so:system/lib/libzxly_security.so
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3275/bin/aggdll:system/bin/aggdll
endif

ifeq ($(BIRD_APK_MOFUT_3275),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/AppMarket.apk:system/app/AppMarket.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/BaiduSearch.apk:system/app/BaiduSearch.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/Doudizhu.apk:system/app/Doudizhu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/GameMYJ.apk:system/app/GameMYJ.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/Ireader.apk:system/app/Ireader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/WaterRipples.apk:data/app/WaterRipples.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/.keep_list:data/app/.keep_list

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libAesArithmetic.so:system/lib/libAesArithmetic.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libbase64encoder_v1_3.so:system/lib/libbase64encoder_v1_3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libBDVoiceSearchClient_V2.so:system/lib/libBDVoiceSearchClient_V2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libcyberplayer.so:system/lib/libcyberplayer.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libffmpeg.so:system/lib/libffmpeg.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libgetcpuspec.so:system/lib/libgetcpuspec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libkpencore.so:system/lib/libkpencore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/liblocSDK3.so:system/lib/liblocSDK3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libMoplusMD5.so:system/lib/libMoplusMD5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libp2p-jni.so:system/lib/libp2p-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/librabjni.so:system/liblibrabjni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libsapi_V5_0.so:system/lib/libsapi_V5_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3275/lib/libstlport_shared.so:system/lib/libstlport_shared.so
endif

ifeq ($(BIRD_APK_VIMOO_3275),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/AppMarket.apk:system/app/AppMarket.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/BaiduSearch.apk:system/app/BaiduSearch.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/Doudizhu.apk:system/app/Doudizhu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/GameVIMOO.apk:system/app/GameVIMOO.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/Ireader.apk:system/app/Ireader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/WaterRipples.apk:data/app/WaterRipples.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/.keep_list:data/app/.keep_list

PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libAesArithmetic.so:system/lib/libAesArithmetic.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libbase64encoder_v1_3.so:system/lib/libbase64encoder_v1_3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libBDVoiceSearchClient_V2.so:system/lib/libBDVoiceSearchClient_V2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libcyberplayer.so:system/lib/libcyberplayer.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libffmpeg.so:system/lib/libffmpeg.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libgetcpuspec.so:system/lib/libgetcpuspec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libkpencore.so:system/lib/libkpencore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/liblocSDK3.so:system/lib/liblocSDK3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libMoplusMD5.so:system/lib/libMoplusMD5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libp2p-jni.so:system/lib/libp2p-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/librabjni.so:system/lib/librabjni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libsapi_V5_0.so:system/lib/libsapi_V5_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/VIMOO_3275/lib/libstlport_shared.so:system/lib/libstlport_shared.so
endif

ifeq ($(BIRD_APK_CUSTOM_KLT_3471),yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/KLT_baiduinput.apk:system/app/KLT_baiduinput.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduinput_lib/libkpencore.so:system/lib/libkpencore.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduinput_lib/librabjni-1.so:system/lib/librabjni-1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduinput_lib/libshare_v2.so:system/lib/libshare_v2.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/KLT_iread.apk:system/app/KLT_iread.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/iread_lib/libiReader_common.so:system/lib/libiReader_common.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/iread_lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/iread_lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/iread_lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/iread_lib/libiReader_icu.so:system/lib/libiReader_icu.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/iread_lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/KLT_baidusearch.apk:system/app/KLT_baidusearch.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/libAesArithmetic.so:system/lib/libAesArithmetic.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/libbase64encoder_v1_3.so:system/lib/libbase64encoder_v1_3.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/libBDVoiceSearchClient_V2.so:system/lib/libBDVoiceSearchClient_V2.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/libcyberplayer.so:system/lib/libcyberplayer.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/libffmpeg.so:system/lib/libffmpeg.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/libgetcpuspec.so:system/lib/libgetcpuspec.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/liblocSDK3.so:system/lib/liblocSDK3.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/libMoplusMD5.so:system/lib/libMoplusMD5.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/libp2p-jni.so:system/lib/libp2p-jni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/librabjni.so:system/lib/librabjni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/libsapi_V5_0.so:system/lib/libsapi_V5_0.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/baiduSearch_lib/libstlport_shared.so:system/lib/libstlport_shared.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/wallpaper_WaterRipples.apk:system/app/wallpaper_WaterRipples.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/wallpaper_lib/libandroidgl20.so:system/lib/libandroidgl20.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/wallpaper_lib/libgdx.so:system/lib/libgdx.so

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/KLT_market.apk:system/app/KLT_market.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/KLT_youlehui.apk:system/app/KLT_youlehui.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3471/KLT_kuwadoudizhu.apk:system/app/KLT_kuwadoudizhu.apk
endif

ifeq ($(BIRD_APK_CUSTOM_MFT_3471),yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/MFT_baiduinput.apk:system/app/MFT_baiduinput.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduinput_lib/libkpencore.so:system/lib/libkpencore.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduinput_lib/librabjni-1.so:system/lib/librabjni-1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduinput_lib/libshare_v2.so:system/lib/libshare_v2.so
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/MFT_iread.apk:system/app/MFT_iread.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/iread_lib/libiReader_common.so:system/lib/libiReader_common.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/iread_lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/iread_lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/iread_lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/iread_lib/libiReader_icu.so:system/lib/libiReader_icu.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/iread_lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/MFT_baidusearch.apk:system/app/MFT_baidusearch.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/libAesArithmetic.so:system/lib/libAesArithmetic.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/libbase64encoder_v1_3.so:system/lib/libbase64encoder_v1_3.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/libBDVoiceSearchClient_V2.so:system/lib/libBDVoiceSearchClient_V2.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/libcyberplayer.so:system/lib/libcyberplayer.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/libffmpeg.so:system/lib/libffmpeg.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/libgetcpuspec.so:system/lib/libgetcpuspec.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/liblocSDK3.so:system/lib/liblocSDK3.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/libMoplusMD5.so:system/lib/libMoplusMD5.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/libp2p-jni.so:system/lib/libp2p-jni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/librabjni.so:system/lib/librabjni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/libsapi_V5_0.so:system/lib/libsapi_V5_0.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/baiduSearch_lib/libstlport_shared.so:system/lib/libstlport_shared.so
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/wallpaper_WaterRipples.apk:system/app/wallpaper_WaterRipples.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/wallpaper_lib/libandroidgl20.so:system/lib/libandroidgl20.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/wallpaper_lib/libgdx.so:system/lib/libgdx.so

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/MFT_market.apk:system/app/MFT_market.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/MFT_meiyouji.apk:system/app/MFT_meiyouji.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3471/MFT_kuwadoudizhu.apk:system/app/MFT_kuwadoudizhu.apk
endif

ifeq ($(BIRD_APK_CUSTOM_VM_3471),yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/VM_baiduinput.apk:system/app/VM_baiduinput.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduinput_lib/libkpencore.so:system/lib/libkpencore.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduinput_lib/librabjni-1.so:system/lib/librabjni-1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduinput_lib/libshare_v2.so:system/lib/libshare_v2.so
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/VM_iread.apk:system/app/VM_iread.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/iread_lib/libiReader_common.so:system/lib/libiReader_common.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/iread_lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/iread_lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/iread_lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/iread_lib/libiReader_icu.so:system/lib/libiReader_icu.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/iread_lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/VM_baidusearch.apk:system/app/VM_baidusearch.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/libAesArithmetic.so:system/lib/libAesArithmetic.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/libbase64encoder_v1_3.so:system/lib/libbase64encoder_v1_3.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/libBDVoiceSearchClient_V2.so:system/lib/libBDVoiceSearchClient_V2.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/libcyberplayer.so:system/lib/libcyberplayer.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/libffmpeg.so:system/lib/libffmpeg.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/libgetcpuspec.so:system/lib/libgetcpuspec.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/liblocSDK3.so:system/lib/liblocSDK3.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/libMoplusMD5.so:system/lib/libMoplusMD5.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/libp2p-jni.so:system/lib/libp2p-jni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/librabjni.so:system/lib/librabjni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/libsapi_V5_0.so:system/lib/libsapi_V5_0.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/baiduSearch_lib/libstlport_shared.so:system/lib/libstlport_shared.so
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/wallpaper_WaterRipples.apk:system/app/wallpaper_WaterRipples.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/wallpaper_lib/libandroidgl20.so:system/lib/libandroidgl20.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/wallpaper_lib/libgdx.so:system/lib/libgdx.so

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/VM_game.apk:system/app/VM_game.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/VM_market.apk:system/app/VM_market.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/VM_3471/VM_kuwadoudizhu.apk:system/app/VM_kuwadoudizhu.apk
endif

ifeq ($(BIRD_APK_KLT_3401),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/AppMarket.apk:system/app/AppMarket.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/BaiduBrowser.apk:system/app/BaiduBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/BaiduMap.apk:system/app/BaiduMap.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/BaiduSearch.apk:system/app/BaiduSearch.apk
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/Doudizhu.apk:system/app/Doudizhu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/DoudizhuBird.apk:system/app/DoudizhuBird.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/GameYLH.apk:system/app/GameYLH.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/Ireader.apk:system/app/Ireader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/WaterRipples_LivingWallpaper.apk:system/app/WaterRipples_LivingWallpaper.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/.keep_list:data/app/.keep_list

PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libAesArithmetic.so:system/lib/libAesArithmetic.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libbase64encoder_v1_3.so:system/lib/libbase64encoder_v1_3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libBDVoiceSearchClient_V2.so:system/lib/libBDVoiceSearchClient_V2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libcyberplayer.so:system/lib/libcyberplayer.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libffmpeg.so:system/lib/libffmpeg.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libgetcpuspec.so:system/lib/libgetcpuspec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libkpencore.so:system/lib/libkpencore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/liblocSDK3.so:system/lib/liblocSDK3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libMoplusMD5.so:system/lib/libMoplusMD5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libp2p-jni.so:system/lib/libp2p-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/librabjni.so:system/lib/librabjni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libsapi_V5_0.so:system/lib/libsapi_V5_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libstlport_shared.so:system/lib/libstlport_shared.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libapp_BaiduMapApplib_OEM500.so:system/lib/libapp_BaiduMapApplib_OEM500.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libAudiofe_V1_OEM500.so:system/lib/libAudiofe_V1_OEM500.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libbase64encoder_v1_2.so:system/lib/libbase64encoder_v1_2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libchiperencoder_v1_1.so:system/lib/libchiperencoder_v1_1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libejTTS_OEM500.so:system/lib/libejTTS_OEM500.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libflyflowjni_V1_0_0_0.so:system/lib/libflyflowjni_V1_0_0_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libosruntime.so:system/lib/libosruntime.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/librabjni_baidubrowser.so:system/lib/librabjni_baidubrowser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libtmfe30.so:system/lib/libtmfe30.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libtmfe30_baidubrowser.so:system/lib/libtmfe30_baidubrowser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libzeus.so:system/lib/libzeus.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libzeus2.1.so:system/lib/libzeus2.1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libzeus2.1_AR.so:system/lib/libzeus2.1_AR.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libZeusJNI.so:system/lib/libZeusJNI.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libgdx.so:system/lib/libgdx.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/lib/libandroidgl20.so:system/lib/libandroidgl20.so
endif

ifeq ($(BIRD_APK_KLT_3401),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/AppMarket.apk:data/app/AppMarket.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/BaiduBrowser.apk:data/app/BaiduBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/BaiduInput.apk:data/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/BaiduMap.apk:data/app/BaiduMap.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/BaiduSearch.apk:data/app/BaiduSearch.apk
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/Doudizhu.apk:data/app/Doudizhu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/DoudizhuBird.apk:data/app/DoudizhuBird.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/GameYLH.apk:data/app/GameYLH.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/Ireader.apk:data/app/Ireader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/WaterRipples_LivingWallpaper.apk:data/app/WaterRipples_LivingWallpaper.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401/.keep_list:data/app/.keep_list
endif

ifeq ($(BIRD_APK_MFT_3401),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/BaiduBrowser.apk:data/app/BaiduBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/Ireader.apk:system/app/Ireader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/Lianyi_LivingWallpaper.apk:data/app/Lianyi_LivingWallpaper.apk

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libchiperencoder_v1_2_1.so:system/lib/libchiperencoder_v1_2_1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libkpencore.so:system/lib/libkpencore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libtmfe30.so:system/lib/libtmfe30.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/.keep_list:data/app/.keep_list
endif

ifeq ($(BIRD_APK_KLT_3401ATM),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/BaiduBrowser.apk:data/app/BaiduBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/Ireader.apk:system/app/Ireader.apk
#PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/Lianyi_LivingWallpaper.apk:data/app/Lianyi_LivingWallpaper.apk

PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/lib/libchiperencoder_v1_2_1.so:system/lib/libchiperencoder_v1_2_1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/lib/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/lib/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/lib/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/lib/libtmfe30.so:system/lib/libtmfe30.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3401ATM/.keep_list:data/app/.keep_list
endif



ifeq ($(BIRD_APK_KLT_P3401ATM),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/BaiduBrowser.apk:data/app/BaiduBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/Ireader.apk:system/app/Ireader.apk
#PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/Lianyi_LivingWallpaper.apk:data/app/Lianyi_LivingWallpaper.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/WaterRipples_LivingWallpaper.apk:system/app/WaterRipples_LivingWallpaper.apk

PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/libchiperencoder_v1_2_1.so:system/lib/libchiperencoder_v1_2_1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/libtmfe30.so:system/lib/libtmfe30.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/libandroidgl20.so:system/lib/libandroidgl20.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/lib/libgdx.so:system/lib/libgdx.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_P3401ATM/.keep_list:data/app/.keep_list
endif





ifeq ($(BIRD_APK_KLT_3571),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/AppMarket.apk:system/app/AppMarket.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/BaiduInput.apk:system/app/BaiduInput.apk
# PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/Doudizhu.apk:system/app/Doudizhu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/DoudizhuBird.apk:system/app/DoudizhuBird.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/GameYLH.apk:system/app/GameYLH.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/Ireader.apk:system/app/Ireader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/WaterRipples_LivingWallpaper.apk:system/app/WaterRipples_LivingWallpaper.apk

PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/BaiduBrowser.apk:data/app/BaiduBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/BaiduMap.apk:data/app/BaiduMap.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/BaiduSearch.apk:data/app/BaiduSearch.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/Jinshanclear.apk:data/app/Jinshanclear.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/.keep_list:data/app/.keep_list

PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libandroidgl20.so:system/lib/libandroidgl20.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libchiperencoder_v1_1.so:system/lib/libchiperencoder_v1_1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libgdx.so:system/lib/libgdx.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libkpencore.so:system/lib/libkpencore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3571/lib/libtmfe30.so:system/lib/libtmfe30.so
endif


ifeq ($(BIRD_APK_KLT_3405),data)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/Lingxi.apk:data/app/Lingxi.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/Migumusic.apk:data/app/Migumusic.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/Shoujidaohang.apk:data/app/Shoujidaohang.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/Shoujiread.apk:data/app/Shoujiread.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/Shoujiyouxi.apk:data/app/Shoujiyouxi.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/Suiyixing.apk:data/app/Suiyixing.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/Yidongmm.apk:data/app/Yidongmm.apk
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/WaterRipples_LivingWallpaper.apk:system/app/WaterRipples_LivingWallpaper.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/baiduinput.apk:system/app/baiduinput.apk
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/.keep_list:data/app/.keep_list
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/.restore_list:data/app/.restore_list
	
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/lib/libgdx.so:system/lib/libgdx.so
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/lib/libandroidgl20.so:system/lib/libandroidgl20.so
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/lib/libkpencore.so:system/lib/libkpencore.so
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/lib/librabjni-1.so:system/lib/librabjni-1.so
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405/lib/libshare_v2.so:system/lib/libshare_v2.so
endif


ifeq ($(BIRD_APK_KLT_3405ATM),data)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/Lingxi.apk:data/app/Lingxi.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/Migumusic.apk:data/app/Migumusic.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/Shoujidaohang.apk:data/app/Shoujidaohang.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/Shoujiread.apk:data/app/Shoujiread.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/Shoujiyouxi.apk:data/app/Shoujiyouxi.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/Suiyixing.apk:data/app/Suiyixing.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/Yidongmm.apk:data/app/Yidongmm.apk
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/WaterRipples_LivingWallpaper.apk:system/app/WaterRipples_LivingWallpaper.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/baiduinput.apk:system/app/baiduinput.apk
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/.keep_list:data/app/.keep_list
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/.restore_list:data/app/.restore_list
	
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/lib/libgdx.so:system/lib/libgdx.so
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/lib/libandroidgl20.so:system/lib/libandroidgl20.so
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/lib/libkpencore.so:system/lib/libkpencore.so
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/KLT_3405ATM/lib/librabjni-1.so:system/lib/librabjni-1.so
endif



ifeq ($(BIRD_APK_MFT_3401),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/BaiduBrowser.apk:system/app/BaiduBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/Ireader.apk:system/app/Ireader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/Lianyi_LivingWallpaper.apk:data/app/Lianyi_LivingWallpaper.apk

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libchiperencoder_v1_1.so:system/lib/libchiperencoder_v1_1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libkpencore.so:system/lib/libkpencore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/libtmfe30.so:system/lib/libtmfe30.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/BaiduBrowser/libbase64encoder_v1_2.so:system/lib/libbase64encoder_v1_2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/BaiduBrowser/libflyflowjni_V1_0_0_0.so:system/lib/libflyflowjni_V1_0_0_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/BaiduBrowser/libosruntime.so:system/lib/libosruntime.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/BaiduBrowser/librabjni_baidubrowser.so:system/lib/librabjni_baidubrowser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/BaiduBrowser/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/BaiduBrowser/libtmfe30_baidubrowser.so:system/lib/libtmfe30_baidubrowser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/BaiduBrowser/libzeus2.1.so:system/lib/libzeus2.1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/BaiduBrowser/libzeus2.1_AR.so:system/lib/libzeus2.1_AR.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/BaiduBrowser/libzeus.so:system/lib/libzeus.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/lib/BaiduBrowser/libZeusJNI.so:system/lib/libZeusJNI.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MFT_3401/.keep_list:data/app/.keep_list
endif


ifeq ($(BIRD_APK_GS_E43),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/GS_E43/.keep_list:data/app/.keep_list
endif



ifeq ($(BIRD_APK_CMCC_3405),data)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/CMCCWifi_KliTON_I828.apk:data/app/CMCCWifi_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/CMREAD_KliTON_I828.apk:data/app/CMREAD_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/Fetion_KliTON_I828.apk:data/app/Fetion_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/GameHall_KliTON_I828.apk:data/app/GameHall_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/HaopuManager_KliTON_I828.apk:data/app/HaopuManager_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/MobileMusic_KliTON_I828.apk:data/app/MobileMusic_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/MobileVideo_KliTON_I828.apk:data/app/MobileVideo_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/MM_FLASH_KliTON_I828.apk:data/app/MM_FLASH_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/MM_KliTON_I828.apk:data/app/MM_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/MM_MANHUA_KliTON_I828.apk:data/app/MM_MANHUA_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/MM_MUSIC_KliTON_I828.apk:data/app/MM_MUSIC_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/MM_VIDEO_KliTON_I828.apk:data/app/MM_VIDEO_KliTON_I828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/WaterRipples_LivingWallpaper.apk:data/app/WaterRipples_LivingWallpaper.apk
     
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/.keep_list:data/app/.keep_list
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/.restore_list:data/app/.restore_list
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/xunfei/xunfei.apk:system/app/xunfei.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/xunfei/lib/libmsc-v4.so:system/lib/libmsc-v4.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/xunfei/lib/libsmartaiwrite-jni-v4.so:system/lib/libsmartaiwrite-jni-v4.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/xunfei/lib/libsmartaiwrite-jni-v5.so:system/lib/libsmartaiwrite-jni-v5.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/xunfei/lib/libsmartaiwrite-jni-v6.so:system/lib/libsmartaiwrite-jni-v6.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/CMCC/xunfei/lib/libvadLib-v3.so:system/lib/libvadLib-v3.so
endif

ifeq ($(BIRD_APK_MOFUT_3405),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/AppMarket.apk:system/app/AppMarket.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/GameMYJ.apk:system/app/GameMYJ.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/DoudizhuBird.apk:system/app/DoudizhuBird.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/WaterRipples_LivingWallpaper.apk:system/app/WaterRipples_LivingWallpaper.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/Ireader.apk:system/app/Ireader.apk

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/BaiduBrowser.apk:data/app/BaiduBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/BaiduMap.apk:data/app/BaiduMap.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/BaiduSearch.apk:data/app/BaiduSearch.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/.keep_list:data/app/.keep_list

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libandroidgl20.so:system/lib/libandroidgl20.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libchiperencoder_v1_1.so:system/lib/libchiperencoder_v1_1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libgdx.so:system/lib/libgdx.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libkpencore.so:system/lib/libkpencore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libtmfe30.so:system/lib/libtmfe30.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3405/lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
endif

ifeq ($(BIRD_APK_CUSTOM_GXQ_T3),yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/GXQ_T3/iReader_V2300_bodaoshuomingshu.apk:system/app/iReader_V2300_bodaoshuomingshu.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/GXQ_T3/libiReader_common.so:system/lib/libiReader_common.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/GXQ_T3/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/GXQ_T3/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/GXQ_T3/libiReader_filezip.so:system/lib/libiReader_filezip.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/GXQ_T3/libiReader_icu.so:system/lib/libiReader_icu.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/GXQ_T3/libiReader_pdf.so:system/lib/libiReader_pdf.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/GXQ_T3/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
endif


ifeq ($(BIRD_APK_B010),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/B010_APK/.keep_list:data/app/.keep_list
endif


ifeq ($(BIRD_APK_E43_T19),data)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/E43_T19_APK/tianqi.apk:data/app/tianqi.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/E43_T19_APK/.keep_list:data/app/.keep_list
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/E43_T19_APK/.restore_list:data/app/.restore_list
endif





ifeq ($(BIRD_502C_CUSTOM_APK),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/502C_APK/angry_birds.apk:system/app/angry_birds.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/502C_APK/libAngryBirdsClassic.so:system/lib/libAngryBirdsClassic.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/502C_APK/candy_crush.apk:system/app/candy_crush.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/502C_APK/libcandycrushsaga.so:system/lib/libcandycrushsaga.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/502C_APK/twitter.apk:system/app/twitter.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/502C_APK/libfilters.so:system/lib/libfilters.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/502C_APK/youtube.apk:system/app/youtube.apk
endif

ifeq ($(BIRD_APK_BXC_A308T),yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/360dashi.apk:system/app/360dashi.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libBench360_3.so:system/lib/libBench360_3.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libBench360_v7_3.so:system/lib/libBench360_v7_3.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libmobilesafe360-jni.so:system/lib/libmobilesafe360-jni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libNativeUtils.so:system/lib/libNativeUtils.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libsu.so:system/lib/libsu.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/baidubrowser.apk:system/app/baidubrowser.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libbase64encoder_v1_2.so:system/lib/libbase64encoder_v1_2.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libflyflowjni_V1_0_0_0.so:system/lib/libflyflowjni_V1_0_0_0.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libosruntime.so:system/lib/libosruntime.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/librabjni_baidubrowser.so:system/lib/librabjni_baidubrowser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libshare_v2.so:system/lib/libshare_v2.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libtmfe30_baidubrowser.so:system/lib/libtmfe30_baidubrowser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libzeus2.1.so:system/lib/libzeus2.1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libzeus2.1_AR.so:system/lib/libzeus2.1_AR.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libzeus.so:system/lib/libzeus.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libZeusJNI.so:system/lib/libZeusJNI.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/baiduinput.apk:system/app/baiduinput.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libkpencore.so:system/lib/libkpencore.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/librabjni-1.so:system/lib/librabjni-1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libshare_v2.so:system/lib/libshare_v2.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/baidushoujizhushou.apk:system/app/baidushoujizhushou.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libasAES_v1.so:system/lib/libasAES_v1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libbase64encoder_v1_4.so:system/lib/libbase64encoder_v1_4.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libBDVoiceSearchClient_V1.so:system/lib/libBDVoiceSearchClient_V1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/liblocSDK3.so:system/lib/liblocSDK3.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libMD5_v1.so:system/lib/libMD5_v1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libminigzip_v1.so:system/lib/libminigzip_v1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libMoplusMD5.so:system/lib/libMoplusMD5.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libprocmoi_v1_3.so:system/lib/libprocmoi_v1_3.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libprocmox_v1_3.so:system/lib/libprocmox_v1_3.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libpush-socket.so:system/lib/libpush-socket.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/librabjni_v1_0.so:system/lib/librabjni_v1_0.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libshare_v2.so:system/lib/libshare_v2.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/game_fuhaozhajinhua.apk:system/app/game_fuhaozhajinhua.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/NewsArticle.apk:system/app/NewsArticle.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/shuqi.apk:system/app/shuqi.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/yingyonghui.apk:system/app/yingyonghui.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libdisk-info-checker.so:system/lib/libdisk-info-checker.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/Antutu4_1.apk:system/app/Antutu4_1.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/libabenchmark.so:system/lib/libabenchmark.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308T_APK/.keep_list:data/app/.keep_list
endif
ifeq ($(BIRD_WATERDROPLIVEWALLPAPER),system)

PRODUCT_COPY_FILES += packages/apps/Bird_APK/WaterDropLiveWallPaper/PuddleDrops_svn22607.apk:system/app/PuddleDrops_svn22607.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/WaterDropLiveWallPaper/libpuddle_drops.so:system/lib/libpuddle_drops.so
endif

ifeq ($(BIRD_A308_IVEWALLPAPER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308_LiveWallpaper/maozi_livewallpaper.apk:system/app/maozi_livewallpaper.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308_LiveWallpaper/Lotus_3Dhehua.apk:system/app/Lotus_3Dhehua.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/A308_LiveWallpaper/libpuddle_drops.so:system/lib/libpuddle_drops.so
endif

ifeq ($(BIRD_APK_T02_CUSTOM),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/T02_APK/guide.apk:system/app/guide.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/T02_APK/.keep_list:data/app/.keep_list
endif

ifeq ($(BIRD_APK_V39_CUSTOM),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/V39_APK/preinstaller.apk:system/app/preinstaller.apk
endif
ifeq ($(BIRD_APK_V39_SUN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/V39_SUN/gal.apk:system/app/gal.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/V39_SUN/postpaid.apk:system/app/postpaid.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/V39_SUN/suntap.apk:system/app/suntap.apk
endif

ifeq ($(BIRD_APK_MOFUT_3476),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/AppMarket.apk:system/app/AppMarket.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/BaiduInput.apk:system/app/BaiduInput.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/BaiduSearch.apk:system/app/BaiduSearch.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/Doudizhu.apk:system/app/Doudizhu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/GameMYJ.apk:system/app/GameMYJ.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/Ireader.apk:system/app/Ireader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/BaiduBrowse.apk:system/app/BaiduBrowse.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/WaterRipples.apk:system/app/WaterRipples.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/.keep_list:data/app/.keep_list

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libAesArithmetic.so:system/lib/libAesArithmetic.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libbase64encoder_v1_3.so:system/lib/libbase64encoder_v1_3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libBDVoiceSearchClient_V2.so:system/lib/libBDVoiceSearchClient_V2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libcyberplayer.so:system/lib/libcyberplayer.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libffmpeg.so:system/lib/libffmpeg.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libgetcpuspec.so:system/lib/libgetcpuspec.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libkpencore.so:system/lib/libkpencore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/liblocSDK3.so:system/lib/liblocSDK3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libMoplusMD5.so:system/lib/libMoplusMD5.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libp2p-jni.so:system/lib/libp2p-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/librabjni.so:system/liblibrabjni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libsapi_V5_0.so:system/lib/libsapi_V5_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libshare_v2.so:system/lib/libshare_v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libstlport_shared.so:system/lib/libstlport_shared.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libandroidgl20.so:system/lib/libandroidgl20.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libbase64encoder_v1_2.so:system/lib/libbase64encoder_v1_2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libflyflowjni_V1_0_0_0.so:system/lib/libflyflowjni_V1_0_0_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libgdx.so:system/lib/libgdx.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libosruntime.so:system/lib/libosruntime.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/librabjni_baidubrowser.so:system/lib/librabjni_baidubrowser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libtmfe30_baidubrowser.so:system/lib/libtmfe30_baidubrowser.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libzeus2.1.so:system/lib/libzeus2.1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libzeus2.1_AR.so:system/lib/libzeus2.1_AR.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libzeus.so:system/lib/libzeus.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libZeusJNI.so:system/lib/libZeusJNI.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libtmfe30.so:system/lib/libtmfe30.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3476/lib/libchiperencoder_v1_1.so:system/lib/libchiperencoder_v1_1.so
endif

# dinghongzhen 130828 add
ifeq ($(BIRD_APK_HBBD),yes)

	ifeq ($(BIRD_APK_GAODE_HBBD),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/amap_HBBD.apk:data/app/amap_HBBD.apk
	endif
	ifeq ($(BIRD_APK_BAIDU_SEARCH_HBBD),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/BaiduSearch_HBBD.apk:data/app/BaiduSearch_HBBD.apk
	endif
	ifeq ($(BIRD_APK_DOUDIZHU_HBBD),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/Doudizhu_Cooee_HBBD.apk:data/app/Doudizhu_Cooee_HBBD.apk
	endif
	ifeq ($(BIRD_APK_FENGHUANG_NEWS_HBBD),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/FenghuangNews_HBBD.apk:data/app/FenghuangNews_HBBD.apk
	endif
	ifeq ($(BIRD_APK_FENGHUANG_VIDEO_HBBD),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/FenghuangShipin_HBBD.apk:data/app/FenghuangShipin_HBBD.apk
	endif
	ifeq ($(BIRD_APK_IREADER_HBBD),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/iReader_HBBD.apk:data/app/iReader_HBBD.apk
	endif
	ifeq ($(BIRD_APK_QQ_BROWSER_HBBD),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/QQBrowser_HBBD.apk:data/app/QQBrowser_HBBD.apk
	endif
	ifeq ($(BIRD_APK_QQ_MANAGER_HBBD),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/QQManager_HBBD.apk:data/app/QQManager_HBBD.apk
	endif
	ifeq ($(BIRD_APK_YINGYONGHUI_HBBD),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/Yingyonghui_HBBD.apk:data/app/Yingyonghui_HBBD.apk
	endif
	ifeq ($(BIRD_APK_LINGXI_HBBD),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/lingxi_HBBD.apk:data/app/lingxi_HBBD.apk
	endif
	ifeq ($(BIRD_APK_HBBD_QQNEWS),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/QQNews_HBBD.apk:data/app/QQNews_HBBD.apk
	endif
	ifeq ($(BIRD_APK_HBBD_QQSHIPIN),data)
		PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD/QQShipin_HBBD.apk:data/app/QQShipin_HBBD.apk
	endif
	
endif
# dinghongzhen 130828 add end

ifeq ($(BIRD_APK_HBBD_I900),yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/baidu_browser.apk:data/app/baidu_browser.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/baidu_serach.apk:data/app/baidu_serach.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/3Dsaiche.apk:data/app/3Dsaiche.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/baidushoujizhushou.apk:data/app/baidushoujizhushou.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/jinsan_clean.apk:data/app/jinsan_clean.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/shoujiguanjia.apk:data/app/shoujiguanjia.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/yingyonghui.apk:data/app/yingyonghui.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/Baidu_Game_Center.apk:data/app/Baidu_Game_Center.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/iReader.apk:data/app/iReader.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/Sina_weather.apk:data/app/Sina_weather.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/Sina_Weibo.apk:data/app/Sina_Weibo.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/taobao.apk:data/app/taobao.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/tencent_news.apk:data/app/tencent_news.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/tencent_shipin.apk:data/app/tencent_shipin.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/wakudoudizhu.apk:data/app/wakudoudizhu.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I900/.keep_list:data/app/.keep_list
endif

ifeq ($(BIRD_APK_HBBD_I580),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/3Dcar.apk:data/app/3Dcar.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/BaiduAssistant.apk:data/app/BaiduAssistant.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/BaiduBrowser.apk:data/app/BaiduBrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/BaiduGame.apk:data/app/BaiduGame.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/BaiduSearch.apk:data/app/BaiduSearch.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/Doudizhu_Cooee.apk:data/app/Doudizhu_Cooee.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/iReader.apk:data/app/iReader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/KingClean.apk:data/app/KingClean.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/QQManager.apk:data/app/QQManager.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/QQShipin.apk:data/app/QQShipin.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/QQNews.apk:data/app/QQNews.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/QQShipin.apk:data/app/QQShipin.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/SinaWeather.apk:data/app/SinaWeather.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/SinaWeibo.apk:data/app/SinaWeibo.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/Tabao.apk:data/app/Tabao.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/Yingyonghui.apk:data/app/Yingyonghui.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/HBBD_I580/.keep_list:data/app/.keep_list
endif

ifeq ($(BIRD_APK_CTA_QQBrower),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/CTA_QQBrower/qqbrowser.apk:data/app/qqbrowser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/CTA_QQBrower/.keep_list:data/app/.keep_list
endif

ifeq ($(BIRD_502C_ZIF_APK),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/502C_ZIF/angry_birds.apk:data/app/angry_birds.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/502C_ZIF/Fruit.apk:data/app/Fruit.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/502C_ZIF/.keep_list:data/app/.keep_list
endif


ifeq ($(BIRD_APK_LM_BAIDU_SEARCH),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/baidusearch_LM.apk:system/app/baidusearch_LM.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/libAesArithmetic_V1_0.so:system/lib/libAesArithmetic_V1_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/libbase64encoder_v1_3.so:system/lib/libbase64encoder_v1_3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/libBdMoplusMD5_V1.so:system/lib/libBdMoplusMD5_V1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/libbdpush_V1_0.so:system/lib/libbdpush_V1_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/libBDVoiceRecognitionClient_V1.so:system/lib/libBDVoiceRecognitionClient_V1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/libDeflatingDecompressor_V3.so:system/lib/libDeflatingDecompressor_V3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/libLineBreak_V2.so:system/lib/libLineBreak_V2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/liblocSDK3.so:system/lib/liblocSDK3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/libNativeFormats_V3.so:system/lib/libNativeFormats_V3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/librabjni_V2_1_0.so:system/lib/librabjni_V2_1_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/libsapi_so_1.so:system/lib/libsapi_so_1.so
endif

ifeq ($(BIRD_APK_LM_BAIDU_SEARCH),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduSearch/baidusearch_LM.apk:data/app/baidusearch_LM.apk
endif

ifeq ($(BIRD_APK_LM_BAIDU_MAP),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/BaiduMap_LM.apk:system/app/BaiduMap_LM.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/libapp_BaiduMapApplib.so:system/lib/libapp_BaiduMapApplib.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/libapp_BaiduNaviApplib.so:system/lib/libapp_BaiduNaviApplib.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/libAudiofe_V1.so:system/lib/libAudiofe_V1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/libBdMoplusMD5_V1.so:system/lib/libBdMoplusMD5_V1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/libbdpush_V1_0.so:system/lib/libbdpush_V1_0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/libBDVoiceInput_V1.so:system/lib/libBDVoiceInput_V1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/libbspatch.so:system/lib/libbspatch.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/libCNPackage.so:system/lib/libCNPackage.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/libcom_radar.so:system/lib/libcom_radar.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/libejTTS.so:system/lib/libejTTS.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/liblocSDK4b.so:system/lib/liblocSDK4b.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/libQuicProxy.so:system/lib/libQuicProxy.so
endif

ifeq ($(BIRD_APK_LM_BAIDU_MAP),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduMap/BaiduMap_LM.apk:data/app/BaiduMap_LM.apk
endif

ifeq ($(BIRD_APK_LM_IREADER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_iReader/LM_iReader.apk:system/app/LM_iReader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_iReader/libcasdkjni.so:system/lib/libcasdkjni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_iReader/libentryex.so:system/lib/libentryex.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_iReader/libft2_mini_nodep.android.so:system/lib/libft2_mini_nodep.android.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_iReader/libftftottf.android.so:system/lib/libftftottf.android.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_iReader/libidentifyapp.so:system/lib/libidentifyapp.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_iReader/libiReader_common.so:system/lib/libiReader_common.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_iReader/libiReader_icu.so:system/lib/libiReader_icu.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_iReader/liblocSDK4.so:system/lib/liblocSDK4.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_iReader/libUiControl.so:system/lib/libUiControl.so
endif


ifeq ($(BIRD_APK_LM_IREADER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_iReader/LM_iReader.apk:data/app/LM_iReader.apk
endif

ifeq ($(BIRD_APK_LM_QQBROWSER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/LM_iReader.apk:system/app/LM_iReader.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libchirp-jni.so:system/lib/libchirp-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libgif-jni.so:system/lib/libgif-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/liblbs.so:system/lib/liblbs.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libmttwebcore.so:system/lib/libmttwebcore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libNativeRQDMtt.so:system/lib/libNativeRQDMtt.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libqqpatch.so:system/lib/libqqpatch.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libQrcodeDecoderMtt.so:system/lib/libQrcodeDecoderMtt.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libspeex.so:system/lib/libspeex.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libvadLib-v2.so:system/lib/libvadLib-v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libwebp_base.so:system/lib/libwebp_base.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libwebp_base_neon.so:system/lib/libwebp_base_neon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libwonderplayer_init.so:system/lib/libwonderplayer_init.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libwtecdh.so:system/lib/libwtecdh.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/libzipalign.so:system/lib/libzipalign.so
endif


ifeq ($(BIRD_APK_LM_QQBROWSER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQBrowser/LM_iReader.apk:data/app/LM_iReader.apk
endif


ifeq ($(BIRD_APK_LM_QQMANAGER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQManager/LM_QQManager.apk:data/app/LM_QQManager.apk
endif

ifeq ($(BIRD_APK_LM_QQLIVE),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_QQLive/LM_QQLive.apk:data/app/LM_QQLive.apk
endif


ifeq ($(BIRD_APK_LM_TENCENT_NEWS),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_TencentNews/LM_TencentNews.apk:data/app/LM_TencentNews.apk
endif

ifeq ($(BIRD_APK_LM_BAIDU_APP),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduApp/LM_BaiduApp.apk:data/app/LM_BaiduApp.apk
endif

ifeq ($(BIRD_APK_LM_BAIDU_BROWSER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_BaiduBrowser/LM_baiduBrowser.apk:data/app/LM_baiduBrowser.apk
endif

ifeq ($(BIRD_APK_LM_FISHING_JOY),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/LM_APK/LM_FishingJoy/LM_FishingJoy.apk:data/app/LM_FishingJoy.apk
endif


ifeq ($(BIRD_APK_OPERAMINI_HAIWAI),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/OperaMini_Haiwai/Operamini.apk:system/app/Operamini.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/OperaMini_Haiwai/libom.so:system/lib/libom.so

PRODUCT_COPY_FILES += packages/apps/Bird_APK/OperaMini_Haiwai/oem.apk:system/app/oem.apk
endif











#lvhuaiyi-tp add begin
ifeq ($(strip $(BIRD_TP_LOCKSCREEN)), yes)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TPUnlock/Party.ux:system/etc/Party.ux
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TPUnlock/Star_Fetcher.ux:system/etc/Star_Fetcher.ux
PRODUCT_COPY_FILES += packages/apps/Bird_APK/TPUnlock/Wedding.ux:system/etc/Wedding.ux
endif
PRODUCT_COPY_FILES += frameworks/base/policy/libs/armeabi/libux_fm_v1.0.0.so:system/lib/libux_fm_v1.0.0.so
#lvhuaiyi-tp add end

ifeq ($(BIRD_APK_C808),data)

PRODUCT_COPY_FILES += packages/apps/Bird_APK/C808/QQ.apk:data/app/QQ.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/C808/Baidu.apk:system/app/Baidu.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/C808/lib/libkpencore.so:system/lib/libkpencore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/C808/lib/libloginshare.so:system/lib/libloginshare.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/C808/lib/librabjni-1.so:system/lib/librabjni-1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/C808/lib/libshare.so:system/lib/libshare.so


PRODUCT_COPY_FILES += packages/apps/Bird_APK/C808/.keep_list:data/app/.keep_list
PRODUCT_COPY_FILES += packages/apps/Bird_APK/C808/.restore_list:data/app/.restore_list
PRODUCT_COPY_FILES += packages/apps/Bird_APK/C808/tencent.mm.apk:data/app/tencent.mm.apk
endif


#[86582]short press camera key to start camera
ifeq ($(strip $(BIRD_SHORTPRESS_CAMERAKEY_TO_START_CAMERA)), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.camera.shortpress=true 
endif

#  VIBRATE_DURATION for  lockscreen lvhuaiyi
ifdef BIRD_LOCKSCREEN_VIBRATE_DURATION
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.lockscr.vibrate.value=$(BIRD_LOCKSCREEN_VIBRATE_DURATION)
endif

#  VIBRATE_DURATION for  VIRTUAL_KEY lvhuaiyi
ifdef BIRD_VIRTUAL_KEY_VIBRATE_DURATION
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.vikey.vibrate.value=$(BIRD_VIRTUAL_KEY_VIBRATE_DURATION)
endif

#  CUSTOM CONTACTS INFO ADD BY WANGGUODONG 2013/12/11
ifdef BIRD_CUSTOM_CONTACTS_NAME_AND_NUMBER
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.bird.custom.contacts.info=$(BIRD_CUSTOM_CONTACTS_NAME_AND_NUMBER)
endif
#  VIBRATE_DURATION for  LONG_PRESS lvhuaiyi
ifdef BIRD_LONG_PRESS_VIBRATE_DURATION
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.longpress.vibrate.value=$(BIRD_LONG_PRESS_VIBRATE_DURATION)
endif

#  BIRD_WLAN_PORT_NAME lvhuaiyi
ifdef BIRD_WLAN_PORT_NAME
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.wlan.port.name=$(BIRD_WLAN_PORT_NAME)
endif
#  BIRD_SCREEN_BRIGHTNESS lvhuaiyi
ifdef BIRD_SCREEN_BRIGHTNESS
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.bird.brightness=$(BIRD_SCREEN_BRIGHTNESS)
endif

#[84963]ningzhiyu 20100817: birghtness automatic begin
ifeq  (yes,$(strip $(BIRD_BRIGHTNESS_DEFAULT_AUTOMATIC)))
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.brightness.automatic=true
endif

#[84592]ningzhiyu, 20120809
ifeq (yes,$(strip $(BIRD_REMOVE_LOCKSCREEN_OFF)))
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.settings.remove.lockoff=true
endif

#[79045]ningzhiyu 20120406
ifeq ($(strip $(BIRD_TOUCH_HAPTIC_FEEDBACK)), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.bird.touch.hapticfeedback=true
endif

#ningzhiyu 20130410
ifeq ($(strip $(BIRD_TP_KEY_VIBRATE)), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.bird.tp.key.vibrate=true
endif

#BIRD_DOOV_HEADSET_CTRL
ifeq ($(strip $(BIRD_DOOV_HEADSET_CTRL)), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.doov.headset.ctrl=true
endif
#led
ifeq ($(strip $(BIRD_RAINBOW_LED)), yes)
  PRODUCT_PACKAGES += RainbowLed \
                                         libmarquee_led_jni
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.rainbowled.ext.notify=true \
    ro.rainbowled.support=true  
endif

#BIRD_MMI_AUTO_TEST
ifeq ($(strip $(BIRD_MMI_AUTO_TEST)), yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    ro.factorytest.headset.ctrl=true
endif

#-----[81336] ningzhiyu 20120523 :virtural button bar / navigation bar
#  BIRD_VIRTUAL_BUTTON_BAR = yes / no
#  BIRD_VIRTUAL_BUTTON_BAR_HEIGHT   is the height of the bar, in pixels
#  BIRD_VIRTUAL_BUTTON_BAR_HEIGHT = 54   for 854x480
#  BIRD_VIRTUAL_BUTTON_BAR_WIDTH    in pixels.Width of the navigation bar when presented vertically along one side
#  BIRD_VIRTUAL_BUTTON_BAR_WIDTH  = 54   for 854x480
#  follow is added by chengting @20130606
#  ro.bird.style.naviBar        navigation bar is designed like hardware buttons,it has back/home/menu buttons in all,home button can be long clicked and show recents.
#  above is added by chengting @20130606

ifeq (yes,$(strip $(BIRD_VIRTUAL_BUTTON_BAR)))
    PRODUCT_PROPERTY_OVERRIDES += \
    	ro.bird.style.naviBar=true \
        ro.bird.control.naviBar=true \
        persist.sys.naviBar=true
    ifdef BIRD_VIRTUAL_BUTTON_BAR_HEIGHT
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.naviBar.height=$(BIRD_VIRTUAL_BUTTON_BAR_HEIGHT)
    endif
    
    ifdef BIRD_VIRTUAL_BUTTON_BAR_WIDTH
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.naviBar.width=$(BIRD_VIRTUAL_BUTTON_BAR_WIDTH)
    endif
    
    #[96885],chengting,@20130827
    ifeq ($(strip $(BIRD_NAVIBAR_LOCKSCR_SEARCHSUPPORT)), yes)
    PRODUCT_PROPERTY_OVERRIDES += \
    	bird.navi.lockscr.nosearch=false    
    endif
    #[98746],chengting,@20130827
    ifeq ($(strip $(BIRD_NAVIBAR_HOME_SEARCHSUPPORT)), yes)
    PRODUCT_PROPERTY_OVERRIDES += \
    	bird.navi.nosearch=false    
    endif
else
    PRODUCT_PROPERTY_OVERRIDES += \
    	ro.bird.style.naviBar=false \
        ro.bird.control.naviBar=true \
        persist.sys.naviBar=false
endif

#app, launcher, camera guide
ifeq ($(strip $(BIRD_ENABLE_LAUNCHER_GUIDE)), yes)
  PRODUCT_PROPERTY_OVERRIDES +=    \
      ro.enable.launcher.guide=true 
endif

ifeq ($(strip $(BIRD_ENABLE_CAMERA_GUIDE)), yes)
  PRODUCT_PROPERTY_OVERRIDES +=    \
      ro.enable.camera.guide=true
endif

#  apk manager ningzhiyu 20120530
ifeq (yes,$(strip $(BIRD_MORE_APP)))
    PRODUCT_PACKAGES += MoreAppBird
endif

#[84362]ningzhiyu, value should be one of [auto,on,off]
ifdef BIRD_CAMERA_DEFAULT_FLASHMODE
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.camera.default.flashmode=$(strip $(BIRD_CAMERA_DEFAULT_FLASHMODE))
endif
#bug[96344]meifangting, value should be one of [auto,50hz,60hz]
ifdef BIRD_CAMERA_DEFAULT_ANTIBANDING
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.camera.default.antibanding=$(strip $(BIRD_CAMERA_DEFAULT_ANTIBANDING))
endif
#ningzhiyu 20120407 doov font
ifeq ($(strip $(BIRD_DOOV_FONTS)),yes)
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.doovfont.support=true
    
    PRODUCT_PACKAGES += fontchanger
        
    PRODUCT_COPY_FILES += \
    packages/apps/Settings/DoovFont/fonts/default.ttf:system/doovfonts/default.ttf \
    packages/apps/Settings/DoovFont/fonts/junya.ttf:system/doovfonts/junya.ttf \
    packages/apps/Settings/DoovFont/fonts/girl.ttf:system/doovfonts/girl.ttf \
    packages/apps/Settings/DoovFont/fonts/yueyuan.ttf:system/doovfonts/yueyuan.ttf \
    packages/apps/Settings/DoovFont/fonts/font.default:system/doovfonts/font.default
endif

#BIRD_SPECIAL_DATA_RESTORE
ifeq ($(strip $(BIRD_SPECIAL_DATA_RESTORE)),yes)
    PRODUCT_PACKAGES += specialrestore
    PRODUCT_PROPERTY_OVERRIDES += \
		ro.specialrestore=true
        
endif

#chengting add @20121219 for guest mode
ifeq ($(strip $(BIRD_GUESTMODE)),yes)
  PRODUCT_PACKAGES += GuestMode
  PRODUCT_PROPERTY_OVERRIDES += \
				ro.settings.guestmode_support=true

ifeq ($(strip $(BIRD_GUESTMODE_LAUNCHER2_SUPPORT)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
				ro.settings.launcher2_support=true
endif				
endif

#camera default capture size:wangfei
ifdef BIRD_CAMERA_DEFAULT_PICTURE_RATIO
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.camera.picture.ratio=$(BIRD_CAMERA_DEFAULT_PICTURE_RATIO)
endif
ifdef BIRD_CAMERA_MAIN_DEFAULT_CAPTURE_SIZE
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.camera.main.capturesize=$(BIRD_CAMERA_MAIN_DEFAULT_CAPTURE_SIZE)
endif
ifdef BIRD_CAMERA_SUB_DEFAULT_CAPTURE_SIZE
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.camera.sub.capturesize=$(BIRD_CAMERA_SUB_DEFAULT_CAPTURE_SIZE)
endif

ifdef BIRD_CAMERA_MAIN_DEFAULT_CAPTURE_SIZE_16x9
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.camera.main.capturesize.16x9=$(BIRD_CAMERA_MAIN_DEFAULT_CAPTURE_SIZE_16x9)
endif
ifdef BIRD_CAMERA_MAIN_DEFAULT_CAPTURE_SIZE_4x3
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.camera.main.capturesize.4x3=$(BIRD_CAMERA_MAIN_DEFAULT_CAPTURE_SIZE_4x3)
endif
ifdef BIRD_CAMERA_SUB_DEFAULT_CAPTURE_SIZE_16x9
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.camera.sub.capturesize.16x9=$(BIRD_CAMERA_SUB_DEFAULT_CAPTURE_SIZE_16x9)
endif
ifdef BIRD_CAMERA_SUB_DEFAULT_CAPTURE_SIZE_4x3
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.camera.sub.capturesize.4x3=$(BIRD_CAMERA_SUB_DEFAULT_CAPTURE_SIZE_4x3)
endif

#shenyibin [92984] when Battery Level under 15 ,only alert dialog once till power off 
ifeq ($(strip $(BIRD_LOWBATTERY_ALERT_ONCE)),yes)
	PRODUCT_PROPERTY_OVERRIDES += \
		ro.bird.lowbatteryalertonce = true
endif


#bug[93264] add by chengting @20130427
ifeq ($(strip $(BIRD_DEVICE_INFO)),yes)  
	ifdef BIRD_RAM_MEGABYTE_SIZE
		PRODUCT_PROPERTY_OVERRIDES += \
			ro.bird.ram_megabyte_size=$(BIRD_RAM_MEGABYTE_SIZE)
	endif 
endif 

#[94302]shenyibin add @20130523 for launcher2  allapp/workspace no switch animation
ifeq ($(strip $(BIRD_LAU2_WITH_SWITCH_ANIM)),yes)
	PRODUCT_PROPERTY_OVERRIDES += \
			ro.bird.lau2_no_switch_anim= false
endif 

#/**#[95368]shenyibin add @20130701 for launcher2 start**/
ifeq ($(strip $(BIRD_NO_TAB_CHANGE_OVERLAP)),no)
	PRODUCT_PROPERTY_OVERRIDES += \
			ro.bird.no_tab_change_overlap= false 
endif 
#yangbinjie value: kailetong,meifutong,tianshida
ifneq ($(strip $(BIRD_EYUSALES_APK)), )
    PRODUCT_PROPERTY_OVERRIDES += bird_eyusales_apk=$(BIRD_EYUSALES_APK)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/EyuSales/EyuSales-n-n.apk:system/app/EyuSales-n-n.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/EyuSales/Config-KLT/$(BIRD_EYUSALES_APK)/pipe.system.prop:system/etc/pipe.system.prop
endif

#add by yangbinjie 20130805
ifeq ($(strip $(BIRD_CAIVS_T9_APK)), yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs/Caivs.apk:system/app/Caivs.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs/lib/libcaivs-jni.so:system/lib/libcaivs-jni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs/lib/libnv-service-wolftel.so:system/lib/libnv-service-wolftel.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs/bin/callnv-service-wolftel:system/bin/callnv-service-wolftel
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs/etc/ctag.ini:system/etc/ctag.ini
endif

ifeq ($(strip $(BIRD_CAIVS_V9_APK)), yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_V9/Caivs.apk:system/app/Caivs.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_V9/lib/libcaivs-jni.so:system/lib/libcaivs-jni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_V9/lib/libnv-service-wolftel.so:system/lib/libnv-service-wolftel.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_V9/bin/callnv-service-wolftel:system/bin/callnv-service-wolftel
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_V9/etc/ctag.ini:system/etc/ctag.ini
endif

ifeq ($(strip $(BIRD_CAIVS_T3_APK)), yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_T3/Caivs.apk:system/app/Caivs.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_T3/lib/libcaivs-jni.so:system/lib/libcaivs-jni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_T3/lib/libnv-service-wolftel.so:system/lib/libnv-service-wolftel.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_T3/bin/callnv-service-wolftel:system/bin/callnv-service-wolftel
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_T3/etc/ctag.ini:system/etc/ctag.ini
endif

ifeq ($(strip $(BIRD_CAIVS_T3D_APK)), yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_T3D/Caivs.apk:system/app/Caivs.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_T3D/lib/libnv-service-wolftel.so:system/lib/libnv-service-wolftel.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_T3D/bin/callnv-service-wolftel:system/bin/callnv-service-wolftel
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_T3D/etc/ctag.ini:system/etc/ctag.ini
endif

ifeq ($(strip $(BIRD_CAIVS_V5_APK)), yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_V5/Caivs.apk:system/app/Caivs.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_V5/lib/libcaivs-jni.so:system/lib/libcaivs-jni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_V5/lib/libnv-service-wolftel.so:system/lib/libnv-service-wolftel.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_V5/bin/callnv-service-wolftel:system/bin/callnv-service-wolftel
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Caivs_V5/etc/ctag.ini:system/etc/ctag.ini
endif

ifeq ($(BIRD_APK_CUSTOM_KUBAO_S100),yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KUBAO_S100/kubao.apk:system/app/kubao.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KUBAO_S100/ku-2.1.2cms-5.7.apk:system/app/ku-2.1.2cms-5.7.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KUBAO_S100/PuddleDropsT9986.apk:system/app/PuddleDropsT9986.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KUBAO_S100/libpuddle_drops.so:system/lib/libpuddle_drops.so

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KUBAO_S100/leitingzhanji.apk:data/app/leitingzhanji.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KUBAO_S100/Kugou.apk:data/app/Kugou.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KUBAO_S100/QQY_ddz.apk:data/app/QQY_ddz.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KUBAO_S100/UMMarketForAndroid.apk:data/app/UMMarketForAndroid.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/KUBAO_S100/KUBAO_OPERA.apk:data/app/KUBAO_OPERA.apk
endif

#add by yangbinjie 20130902
ifeq ($(strip $(BIRD_WINDOW_ANIMATION_ENABLE)), yes)
    ifdef BIRD_WINDOW_ANIMATION_DURATION_SCALE
        PRODUCT_PROPERTY_OVERRIDES += \
            ro.bird.window.anim.duration=$(BIRD_WINDOW_ANIMATION_DURATION_SCALE)
    endif
endif

#tencent superuser
ifeq ($(strip $(TENCENT_SUPERUSER_SUPPORT)), yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/TencentSuperuser/TencentUser.apk:system/app/TencentUser.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/TencentSuperuser/tu.jar:system/framework/tu.jar
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/TencentSuperuser/tu:system/bin/tu 
endif

#add by shenzhiwang 20130822
ifeq ($(strip $(BIRD_TURN_OFF_SCREEN_BY_TIMER)), yes)
    ifdef BIRD_SCREEN_TIMEOUT
        PRODUCT_PROPERTY_OVERRIDES += \
            ro.bird.screen_timeout=$(BIRD_SCREEN_TIMEOUT)
    endif
    ifdef BIRD_DIM_SCREEN_TIMEOUT
        PRODUCT_PROPERTY_OVERRIDES += \
            ro.bird.dim_screen_timeout=$(BIRD_DIM_SCREEN_TIMEOUT)
    endif
endif

#[102679], add by shenzhiwang, 20140520
ifdef BIRD_ECCLIST
    PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.ecclist=$(BIRD_ECCLIST)
endif

#add by shenzhiwang 20131203 
ifeq ($(strip $(BIRD_PHONE_TOTAL_MEMORY)), yes)
    ifdef BIRD_PHONE_TOTAL_MEMORY_SIZE
        PRODUCT_PROPERTY_OVERRIDES += \
            ro.bird.phone_total_memory=$(BIRD_PHONE_TOTAL_MEMORY_SIZE)
    endif
    ifdef BIRD_PHONE_OTHER_MEMORY_SIZE
        PRODUCT_PROPERTY_OVERRIDES += \
            ro.bird.phone_other_memory=$(BIRD_PHONE_OTHER_MEMORY_SIZE)
    endif
endif

ifeq ($(strip $(BIRD_PHONE_STORAGE_FAKE)), yes)
    ifdef BIRD_PHONE_STORAGE_FAKE_SIZE
        PRODUCT_PROPERTY_OVERRIDES += \
            ro.bird.phone_storage=$(BIRD_PHONE_STORAGE_FAKE_SIZE)
    endif
endif

#[94983] shenyibin add for only change icon ,and would not apply wallpaper when apply themes or switch themes 
#in themeAPkManager and launcherdoov
ifeq ($(strip $(BIRD_THEME_CHANGE_ICON)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.theme_change_icon= true
endif

#[95946] shenyibin add for 13E43 make  fontsize  large start
ifeq ($(strip $(BIRD_QHD_FONTSIZE_LARGE)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.qhd_fontsize_large= true
endif

#shenyibin add for LauncherDoov HAS_HOTSEAT_BG
ifeq ($(strip $(BIRD_HAS_HOTSEAT_BG)),no)
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.has_hotseat_bg= false
endif

#shenyibin add for ThemeApkManager @20130808
ifeq ($(strip $(BIRD_THEME_ONLY_LOCK_PAGE)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.only_lock_page= true
endif

#shenyibin add for LauncherDooc folder_icon @20130916 
ifeq ($(strip $(BIRD_LD_FOLD_GRID)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.ld_fold_icon_grid= true
endif

#yangbinjie
ifneq ($(strip $(BIRD_DISPLAY_SAR_VALUE_FOR_IBALL)),)
  PRODUCT_PROPERTY_OVERRIDES += \
        bird.iball.sarvalue.type= $(strip $(BIRD_DISPLAY_SAR_VALUE_FOR_IBALL))
endif


#shenyibin add  @20130809
ifdef BIRD_THEME_LOCK_TYPE
  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.lock_type= $(BIRD_THEME_LOCK_TYPE)
endif
#meifangting add 20131125
ifdef BIRD_DEFAULT_SCENELOCK
  PRODUCT_PROPERTY_OVERRIDES += \
       ro.bird.default.scenelock= $(BIRD_DEFAULT_SCENELOCK)
endif
#shenyibin add @20121227 for Theme Manager
ifeq ($(strip $(BIRD_THEME_MANAGER)),yes)
  PRODUCT_PACKAGES += ThemeManager
   PRODUCT_PROPERTY_OVERRIDES += \
				ro.bird.theme_manager = true
				
		ifneq ($(strip $(BIRD_THEME_MANAGER_INIT)), )
			PRODUCT_PROPERTY_OVERRIDES += \
             bird_theme_manager_init=$(BIRD_THEME_MANAGER_INIT)

		PRODUCT_COPY_FILES += bird/theme/theme_packages/init_theme/$(BIRD_THEME_MANAGER_INIT)/description.xml:data/theme/description.xml
		
		src_files := $(shell ls bird/theme/theme_packages/init_theme/$(BIRD_THEME_MANAGER_INIT)/drawable)
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
        bird/theme/theme_packages/init_theme/$(BIRD_THEME_MANAGER_INIT)/drawable/$(file):data/theme/drawable/$(file))
		
		
		src_files := $(shell ls bird/theme/theme_packages/init_theme/$(BIRD_THEME_MANAGER_INIT)/launcher_icons)
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
        bird/theme/theme_packages/init_theme/$(BIRD_THEME_MANAGER_INIT)/launcher_icons/$(file):data/theme/launcher_icons/$(file))
		endif
		
		ifeq ($(strip $(BIRD_THEME_MANAGER_CIBAI)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/cibai.bth:system/themes/cibai.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_CUTE)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/cute.bth:system/themes/cute.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_HUGUANG)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/huguang.bth:system/themes/huguang.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_SHUIDI)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/shuidi.bth:system/themes/shuidi.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_SIMPLE)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/simple.bth:system/themes/simple.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_TIANGUANG)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/tianguang.bth:system/themes/tianguang.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_WALLPAPERS)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/wallpapers.wall:system/themes/wallpapers.wall
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_XIAOQINGXIN)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/xiaoqingxin.bth:system/themes/xiaoqingxin.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_YEWAN)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/yewan.bth:system/themes/yewan.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_XINGKONG)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/xingkong.bth:system/themes/xingkong.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_YINXIANGHUALANG)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/yinxianghualang.bth:system/themes/yinxianghualang.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_YOUJING)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/youjing.bth:system/themes/youjing.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_THL_V1)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/thl_v1.bth:system/themes/thl_v1.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_THL_V2)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/thl_v2.bth:system/themes/thl_v2.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_THL_V3)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/thl_v3.bth:system/themes/thl_v3.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_ICS)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/ics.bth:system/themes/ics.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_WALLPAPERS_12G25)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/wallpapers_12G25.wall:system/themes/wallpapers.wall
		endif
		ifeq ($(strip $(BIRD_THEME_TIANGUANG_GETEK_QHD)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/tianguang_GETEK_QHD.bth:system/themes/tianguang.bth
		endif
		ifeq ($(strip $(BIRD_WALLPAPERS_TIANGUANG)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/wallpapers_tianguang.wall:system/themes/wallpapers.wall
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_THL_FHD)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/thl_fhd.bth:system/themes/thl_fhd.bth
		endif
		ifeq ($(strip $(BIRD_THEME_MANAGER_BEAUTIFUL)),yes)
		PRODUCT_COPY_FILES += bird/theme/theme_packages/beautiful.bth:system/themes/beautiful.bth
		endif
endif

#ningzhiyu 20130815, xianxin elder launcher & apks
ifeq ($(strip $(BIRD_DIANXIN_ELDER_LAUNCHER)), yes)
#    PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/DianxinElder/apk/hllservice.apk:system/app/hllservice.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/DianxinElder/apk/1074101111001005.apk:system/app/1074101111001005.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/DianxinElder/apk/1074101111007068.apk:system/app/1074101111007068.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/DianxinElder/apk/elder_launcher2.apk:data/app/elder_launcher2.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/DianxinElder/lib/libfamily_jni.so:system/lib/libfamily_jni.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/DianxinElder/theme/namedValues:/system/etc/jjfamily/preset/namedValues
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/DianxinElder/theme/deskItems:/system/etc/jjfamily/preset/deskItems

  PRODUCT_PROPERTY_OVERRIDES += \
        ro.bird.elderlauncher=true

endif

$(call inherit-product, $(SRC_TARGET_DIR)/product/core.mk)
$(call inherit-product-if-exists, frameworks/base/data/fonts/fonts.mk)
$(call inherit-product-if-exists, external/lohit-fonts/fonts.mk)
$(call inherit-product-if-exists, frameworks/base/data/keyboards/keyboards.mk)
$(call inherit-product-if-exists, mediatek/frameworks-ext/base/data/sounds/AudioMtk.mk)
$(call inherit-product-if-exists, frameworks/base/data/sounds/AllAudio.mk)
$(call inherit-product-if-exists, external/svox/pico/lang/all_pico_languages.mk)
$(call inherit-product-if-exists, mediatek/external/sip/sip.mk)
$(call inherit-product-if-exists, external/naver-fonts/fonts.mk)
$(call inherit-product-if-exists, external/cibu-fonts/fonts.mk)

ifeq ($(strip $(MTK_VOICE_UNLOCK_SUPPORT)),yes)
    $(call inherit-product-if-exists, mediatek/frameworks/base/voicecommand/cfg/voicecommand.mk)
else
        ifeq ($(strip $(MTK_VOICE_UI_SUPPORT)),yes)
            $(call inherit-product-if-exists, mediatek/frameworks/base/voicecommand/cfg/voicecommand.mk)
        endif
endif
#
ifeq ($(strip $(MTK_GAMELOFT_AVENGERS_ULC_CN_APP)), yes)
  PRODUCT_PACKAGES += Avengers_ULC_CN
endif

ifeq ($(strip $(MTK_GAMELOFT_AVENGERS_ULC_WW_APP)), yes)
  PRODUCT_PACKAGES += Avengers_ULC_WW
endif

ifeq ($(strip $(MTK_GAMELOFT_LBC_ULC_CN_APP)), yes)
  PRODUCT_PACKAGES += LittleBigCity_ULC_CN
endif

ifeq ($(strip $(MTK_GAMELOFT_LBC_ULC_WW_APP)), yes)
  PRODUCT_PACKAGES += LittleBigCity_ULC_WW
endif

ifeq ($(strip $(MTK_GAMELOFT_WONDERZOO_ULC_CN_APP)), yes)
  PRODUCT_PACKAGES += WonderZoo_ULC_CN
endif

ifeq ($(strip $(MTK_GAMELOFT_WONDERZOO_ULC_WW_APP)), yes)
  PRODUCT_PACKAGES += WonderZoo_ULC_WW
endif

ifeq ($(strip $(MTK_GAMELOFT_GLL_ULC_CN_APP)), yes)
  PRODUCT_PACKAGES += GLLive_ULC_CN
endif

ifeq ($(strip $(MTK_GAMELOFT_GLL_ULC_WW_APP)), yes)
  PRODUCT_PACKAGES += GLLive_ULC_WW
endif
#

ifeq ($(strip $(MTK_GAMELOFT_GLL_ULC_WW_APP)), yes)
  PRODUCT_PACKAGES += GLLive_ULC_WW
endif
#
ifeq ($(strip $(MTK_DENA_MOBAGE_APP)), yes)
  PRODUCT_PACKAGES += Mobage
  PRODUCT_PACKAGES += libmobage.so
endif

ifeq ($(strip $(MTK_DENA_MINIROSANGUO_APP)), yes)
  PRODUCT_PACKAGES += MiniRoSanguo
  PRODUCT_PACKAGES += libmobage.so
  PRODUCT_PACKAGES += libgameocem.so
  PRODUCT_PACKAGES += libcocosdenshionocem.so
endif

ifeq ($(strip $(MTK_REGIONALPHONE_SUPPORT)),yes)
  PRODUCT_PACKAGES += RegionalPhoneManager
endif
ifeq ($(strip $(MTK_RILD_READ_IMSI)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
  ril.read.imsi=1
endif

ifeq ($(strip $(MTK_MOBILE_MANAGEMENT)), yes)
    PRODUCT_PACKAGES += mac_permissions.xml
endif
ifeq ($(strip $(MTK_PERMISSION_CONTROL)),yes)
  PRODUCT_PACKAGES += PermissionControl
endif

# For BIRD_LIGHTING_LOCKER BIRD Wang Lei 130529 ADD BEGIN
ifeq ($(strip $(BIRD_LIGHTING_LOCKER)), yes)
	PRODUCT_COPY_FILES += bird/BirdLightingLocker/libLighting.so:system/lib/libLighting.so
	PRODUCT_COPY_FILES += bird/BirdLightingLocker/yl_plant_gamecenter_config.xml:system/app/yl_plant_gamecenter_config.xml
	PRODUCT_COPY_FILES += bird/BirdLightingLocker/yl_plant_themecenter_config.xml:system/app/yl_plant_themecenter_config.xml
	PRODUCT_COPY_FILES += bird/BirdLightingLocker/yunlan_lockview_config.xml:system/yunlan_lockview_config.xml
ifeq ($(strip $(BIRD_LIGHTING_LOCKER_BING_GAME)), yes)
	PRODUCT_COPY_FILES += bird/BirdLightingLocker/GAMECENTER.apk:system/app/GAMECENTER.apk
	PRODUCT_COPY_FILES += bird/BirdLightingLocker/LQ_LightingLocker_bingGame.apk:system/app/LQ_LightingLocker.apk
else
	PRODUCT_COPY_FILES += bird/BirdLightingLocker/THEMEMARKET.apk:system/app/THEMEMARKET.apk
	PRODUCT_COPY_FILES += bird/BirdLightingLocker/LQ_LightingLocker.apk:system/app/LQ_LightingLocker.apk
endif
endif
# For BIRD_LIGHTING_LOCKER BIRD Wang Lei 130529 ADD END

# For BIRD_PARTICLE_LOCKER BIRD shenyibin 130715 ADD 
#ningzhiyu 20130819 update tp 1.0.12151 version
ifeq ($(strip $(BIRD_PARTICLE_LOCKER)), yes)
	PRODUCT_COPY_FILES += bird/locks/ParticleLock/Particle_1.0.12151.apk:system/app/Particle_1.0.12151.apk
endif


# For BIRD_3X_SLIDER BIRD Wang Lei 130530 ADD BEGIN
ifeq ($(BIRD_3X_SLIDER), yes)
	PRODUCT_COPY_FILES += packages/apps/3XSlider/slider_app.xml:system/app/slider_app.xml
ifeq ($(BIRD_LIGHTING_LOCKER_BING_GAME), yes)
	PRODUCT_COPY_FILES += packages/apps/3XSlider/3X_SLIDER_S4_bingGame.apk:system/app/3X_SLIDER_S4.apk
else
	PRODUCT_COPY_FILES += packages/apps/3XSlider/3X_SLIDER_S4.apk:system/app/3X_SLIDER_S4.apk
endif
endif
# For BIRD_3X_SLIDER BIRD Wang Lei 130530 ADD END

# For BIRD_3X_LAUNCHER BIRD Wang Lei 130530 ADD BEGIN
ifeq ($(BIRD_3X_LAUNCHER), yes)
	PRODUCT_COPY_FILES += packages/apps/3XLauncher/launcher_config.xml:system/launcher_config.xml
ifeq ($(BIRD_LIGHTING_LOCKER_BING_GAME), yes)
	PRODUCT_COPY_FILES += packages/apps/3XLauncher/3XDESKTOP.LAUNCHER_S4_bingGame.apk:system/app/3XDESKTOP.LAUNCHER_S4.apk
else
	PRODUCT_COPY_FILES += packages/apps/3XLauncher/3XDESKTOP.LAUNCHER_S4.apk:system/app/3XDESKTOP.LAUNCHER_S4.apk
endif
endif
# For BIRD_3X_LAUNCHER BIRD Wang Lei 130530 ADD END

# For BIRD_3X_WIDGET_WEATHER BIRD Wang Lei 131009 ADD BEGIN
ifeq ($(BIRD_3X_WIDGET_WEATHER), yes)
	PRODUCT_COPY_FILES += packages/apps/3XWidgetWeather/3X_S4_WIDGET_WEATHER.apk:system/app/3X_S4_WIDGET_WEATHER.apk
endif
# For BIRD_3X_WIDGET_WEATHER BIRD Wang Lei 131009 ADD END

ifeq ($(strip $(BIRD_3RD_SUPER_THEME)), yes)
    ifeq ($(strip $(BIRD_3RD_SUPER_THEME_NAME)), PaperTown)
        PRODUCT_COPY_FILES += packages/apps/Bird_APK/SuperTheme/PaperTown2.apk:system/app/PaperTown2.apk
    endif
endif
###rom encrypt apks,chengting,@20130712,begin
ifeq ($(BIRD_ROM_ENCRYPT_APKS), yes)
	PRODUCT_PACKAGES += renovateguard
    	PRODUCT_COPY_FILES += bird/renovateguard_apk_list/out/apk_notice.txt:system/apk_notice.txt
#    	PRODUCT_COPY_FILES += bird/renovateguard_apk_list/out/apk_notice.txt:data/apk_notice.txt
    	PRODUCT_COPY_FILES += bird/renovateguard_apk_list/out/apk_compile_info:system/gate/apk_compile_info
endif
###rom encrypt apks,chengting,@20130712,end
ifeq ($(strip $(BIRD_FONTS_CHANGER)), yes)
  PRODUCT_PACKAGES += FontsChanger
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/FontChange/.keep_list:data/app/.keep_list
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/FontChange/fangzhengjianzhi.ttf:data/app/fonts/fangzhengjianzhi.ttf
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/FontChange/fangzhengjinglei.ttf:data/app/fonts/fangzhengjinglei.ttf
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/FontChange/fangzhengmingwu.ttf:data/app/fonts/fangzhengmingwu.ttf
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/FontChange/hanyiyaya.ttf:data/app/fonts/hanyiyaya.ttf
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/FontChange/huakangshaonv.ttf:data/app/fonts/huakangshaonv.ttf
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/FontChange/jianwawa.ttf:data/app/fonts/jianwawa.ttf
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/FontChange/xiongtu.ttf:data/app/fonts/xiongtu.ttf   
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/FontChange/beiweikaiti.ttf:data/app/fonts/beiweikaiti.ttf  
endif

ifeq ($(strip $(BIRD_DIRECT_WITH_PROXIMITY)), yes)
  PRODUCT_PACKAGES += Direct
endif

ifeq ($(strip $(BIRD_V9_SEARCH)), yes)
  PRODUCT_PACKAGES := $(filter-out QuickSearchBox, $(PRODUCT_PACKAGES))
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/V9Search/V9Search_mdd-bodao.apk:system/app/V9Search_mdd-bodao.apk
endif

ifeq ($(BIRD_YUNLAN_NOTE3_LOCK), yes)
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/YuLan_Note3/note3.apk:system/app/note3.apk
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/YuLan_Note3/IqiyooV1.5.0_5_NOTE3.apk:system/app/IqiyooV1.5.0_5_NOTE3.apk
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/YuLan_Note3/libWaterColor.so:system/lib/libWaterColor.so
	PRODUCT_COPY_FILES += packages/apps/Bird_APK/YuLan_Note3/yunlan_lockview_config.xml:system/yunlan_lockview_config.xml
endif

#---------------sunjunwei add yunlan main app  20132219 beign----------------------------
ifeq ($(BIRD_APKTHEME_NEWLOCK),yes)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/yunlan/YunLanUnlock.apk:system/app/YunLanUnlock.apk
endif

ifeq ($(BIRD_USE_IQIYOO),yes)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/yunlan/Iqiyoo.apk:system/app/Iqiyoo.apk
endif
#---------------sunjunwei add yunlan main app  20131119 end----------------------------


#---------------sunjunwei add yunlan jingdian  20131119 begin--------------------------------
ifeq ($(BIRD_LOCKER_FIRE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Fire_Unlock.apk:system/app/Fire_Unlock.apk
endif

ifeq ($(BIRD_LOCKER_FIRE),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Fire_Unlock.apk:data/app/Fire_Unlock.apk
endif

ifeq ($(BIRD_LOCKER_GERIOUSKY),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/GERIOUSKY.apk:system/app/GERIOUSKY.apk
endif

ifeq ($(BIRD_LOCKER_GERIOUSKY),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/GERIOUSKY.apk:data/app/GERIOUSKY.apk
endif

ifeq ($(BIRD_LOCKER_MEIZU),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/MEIZU.apk:system/app/MEIZU.apk
endif

ifeq ($(BIRD_LOCKER_MEIZU),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/MEIZU.apk:data/app/MEIZU.apk
endif

ifeq ($(BIRD_LOCKER_MIUI),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/MiUi.apk:system/app/MiUi.apk
endif

ifeq ($(BIRD_LOCKER_MIUI),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/MiUi.apk:data/app/MiUi.apk
endif

ifeq ($(BIRD_LOCKER_WATER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/WATER.apk:system/app/WATER.apk
endif

ifeq ($(BIRD_LOCKER_WATER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/WATER.apk:data/app/WATER.apk
endif
ifeq ($(BIRD_LOCKER_CHENXIWEILU),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Yl_ChenXiWeiLu.apk:system/app/Yl_ChenXiWeiLu.apk
endif
ifeq ($(BIRD_LOCKER_CHENXIWEILU),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Yl_ChenXiWeiLu.apk:data/app/Yl_ChenXiWeiLu.apk
endif
ifeq ($(BIRD_LOCKER_LUOHUALIUSHUI),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Yl_LuoHuaLiuShui.apk:system/app/Yl_LuoHuaLiuShui.apk
endif
ifeq ($(BIRD_LOCKER_LUOHUALIUSHUI),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Yl_LuoHuaLiuShui.apk:data/app/Yl_LuoHuaLiuShui.apk
endif
ifeq ($(BIRD_LOCKER_XINGCHENZHUANSHI),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Yl_XingChenZhuanShi.apk:system/app/Yl_XingChenZhuanShi.apk
endif
ifeq ($(BIRD_LOCKER_XINGCHENZHUANSHI),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Yl_XingChenZhuanShi.apk:data/app/Yl_XingChenZhuanShi.apk
endif
ifeq ($(BIRD_LOCKER_XINGGUANGSHANSHUO),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Yl_XingGuangShanShuo.apk:system/app/Yl_XingGuangShanShuo.apk
endif
ifeq ($(BIRD_LOCKER_XINGGUANGSHANSHUO),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Yl_XingGuangShanShuo.apk:data/app/Yl_XingGuangShanShuo.apk
endif
ifeq ($(BIRD_LOCKER_YINHEHEIDONG),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Yl_YinHeHeiDong.apk:system/app/Yl_YinHeHeiDong.apk
endif
ifeq ($(BIRD_LOCKER_YINHEHEIDONG),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/jingdian/Yl_YinHeHeiDong.apk:data/app/Yl_YinHeHeiDong.apk
endif
#---------------sunjunwei add yunlan jingdian  20131119 end--------------------------------

#---------------sunjunwei add yunlan changjing 20131119 beign--------------------------------
ifeq ($(BIRD_LOCKER_BEER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/Beer.apk:system/app/Beer.apk
endif

ifeq ($(BIRD_LOCKER_BEER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/Beer.apk:data/app/Beer.apk
endif

ifeq ($(BIRD_LOCKER_BUBBLE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/BUBBLE.apk:system/app/BUBBLE.apk
endif

ifeq ($(BIRD_LOCKER_BUBBLE),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/BUBBLE.apk:data/app/BUBBLE.apk
endif

ifeq ($(BIRD_LOCKER_CARTOON),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/CARTOON.apk:system/app/CARTOON.apk
endif

ifeq ($(BIRD_LOCKER_CARTOON),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/CARTOON.apk:data/app/CARTOON.apk
endif

ifeq ($(BIRD_LOCKER_CHILD),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/CHILD.apk:system/app/CHILD.apk
endif

ifeq ($(BIRD_LOCKER_CHILD),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/CHILD.apk:data/app/CHILD.apk
endif

ifeq ($(BIRD_LOCKER_CROSSRANG),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/CROSSRANG.apk:system/app/CROSSRANG.apk
endif

ifeq ($(BIRD_LOCKER_CROSSRANG),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/CROSSRANG.apk:data/app/CROSSRANG.apk
endif

ifeq ($(BIRD_LOCKER_MECHIES),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/MECHIES.apk:system/app/MECHIES.apk
endif

ifeq ($(BIRD_LOCKER_MECHIES),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/MECHIES.apk:data/app/MECHIES.apk
endif

ifeq ($(BIRD_LOCKER_RAIN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/RAIN.apk:system/app/RAIN.apk
endif

ifeq ($(BIRD_LOCKER_RAIN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/RAIN.apk:data/app/RAIN.apk
endif

ifeq ($(BIRD_LOCKER_SLEEPINGGUITER),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/SLEEPINGGUITER.apk:system/app/SLEEPINGGUITER.apk
endif

ifeq ($(BIRD_LOCKER_SLEEPINGGUITER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/SLEEPINGGUITER.apk:data/app/SLEEPINGGUITER.apk
endif

ifeq ($(BIRD_LOCKER_BLUESKY),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/BlueSky.apk:system/app/BlueSky.apk
endif

ifeq ($(BIRD_LOCKER_BLUESKY),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/BlueSky.apk:data/app/BlueSky.apk
endif

ifeq ($(BIRD_LOCKER_NOTE3),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/Not3.apk:system/app/Not3.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/libWaterColor.so:system/lib/libWaterColor.so
endif

ifeq ($(BIRD_LOCKER_NOTE3),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/Not3.apk:data/app/Not3.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/libWaterColor.so:system/lib/libWaterColor.so
endif

ifeq ($(BIRD_LOCKER_KAPIAN),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/Yl_KaPian.apk:system/app/Yl_KaPian.apk
endif

ifeq ($(BIRD_LOCKER_KAPIAN),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/Yl_KaPian.apk:data/app/Yl_KaPian.apk
endif
ifeq ($(BIRD_LOCKER_ZHEYE),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/Yl_ZheYe.apk:system/app/Yl_ZheYe.apk
endif

ifeq ($(BIRD_LOCKER_ZHEYE),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/yunlan_locker/changjing/Yl_ZheYe.apk:data/app/Yl_ZheYe.apk
endif
#---------------sunjunwei add yunlan changjing 20131119 end--------------------------------
ifeq ($(BIRD_APK_CUSTOM_MOFUT_M3491),yes)
    PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/baiduinput_KLT.apk:system/app/baiduinput_KLT.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/baiduinput_lib/libchiperencoder_v1_1.so:system/lib/libchiperencoder_v1_1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/baiduinput_lib/libkpencore.so:system/lib/libkpencore.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/baiduinput_lib/libshare_v2.so:system/lib/libshare_v2.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/baiduinput_lib/librabjni-1.so:system/lib/librabjni-1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/baiduinput_lib/libtmfe30.so:system/lib/libtmfe30.so
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/iReader_M3491.apk:system/app/iReader_M3491.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/iread_lib/libiReader_common.so:system/lib/libiReader_common.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/iread_lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/iread_lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/iread_lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/iread_lib/libiReader_icu.so:system/lib/libiReader_icu.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/iread_lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/KuwaDouDIZhuv_KLT.apk:system/app/KuwaDouDIZhuv_KLT.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/MFT_market.apk:system/app/MFT_market.apk
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/MFT_game.apk:system/app/MFT_game.apk
    
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/SystemPipe_V020_20131121_SpecForBoDao_sz.apk:system/app/SystemPipe_V020_20131121_SpecForBoDao_sz.apk
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/system/pipe.system.prop:system/etc/pipe.system.prop

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/iBingoLauncher2.apk:system/app/iBingoLauncher2.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/lib/libandroid_animation.so:system/lib/libandroid_animation.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/lib/libbglw_mfd.so:system/lib/libbglw_mfd.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/lib/libCnvtGLPixel.so:system/lib/libCnvtGLPixel.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/lib/libgauss.so:system/lib/libgauss.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/lib/libshader.so:system/lib/libshader.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/lib/libSingleTypeCnvt.so:system/lib/libSingleTypeCnvt.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/com.ibingo.uistore_0.1.70-201312051501.apk:system/app/com.ibingo.uistore_0.1.70-201312051501.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/com_ibingo_security.apk:system/app/com_ibingo_security.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/OppoU2SLockV42.apk:system/app/OppoU2SLockV42.apk
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/lib/libandroidgl20.so:system/lib/libandroidgl20.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/lib/libgdx.so:system/lib/libgdx.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/UI/WaterRipples_V3.20.1022.apk:system/app/WaterRipples_V3.20.1022.apk

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/data/baidubrowser_Android_3-0-7-50_1000789a.apk:data/app/baidubrowser_Android_3-0-7-50_1000789a.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/data/BaiduMap-5_0_lite_xx53829_hdpi_20130828.apk:data/app/BaiduMap-5_0_lite_xx53829_hdpi_20130828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/data/baiduSearchAndroid_1-0-18-93_7300090a.apk:data/app/baiduSearchAndroid_1-0-18-93_7300090a.apk
#   PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/data/cleanmaster-release-380336.apk_m_2010000585.apk:data/app/cleanmaster-release-380336.apk_m_2010000585.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/MOFUT_3491/data/myxj_pinsheng_common_20131213.apk:data/app/myxj_pinsheng_common_20131213.apk
endif


ifeq ($(BIRD_APK_CUSTOM_VM_3492),yes)
    PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/baiduinput_V3493.apk:system/app/baiduinput_V3493.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/baiduinput_lib/libchiperencoder_v1_1.so:system/lib/libchiperencoder_v1_1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/baiduinput_lib/libkpencore.so:system/lib/libkpencore.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/baiduinput_lib/libshare_v2.so:system/lib/libshare_v2.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/baiduinput_lib/librabjni-1.so:system/lib/librabjni-1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/baiduinput_lib/libtmfe30.so:system/lib/libtmfe30.so
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/iReader_K3492.apk:system/app/iReader_K3492.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/iread_lib/libiReader_common.so:system/lib/libiReader_common.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/iread_lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/iread_lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/iread_lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/iread_lib/libiReader_icu.so:system/lib/libiReader_icu.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/iread_lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
    
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/KLT_Game.apk:system/app/KLT_Game.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/KLT_weimeiMarket.apk:system/app/KLT_weimeiMarket.apk
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/SystemPipe_V020_20131121_SpecForBoDao_sz.apk:system/app/SystemPipe_V020_20131121_SpecForBoDao_sz.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/system/pipe.system.prop:system/etc/pipe.system.prop

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/iBingoLauncher2.apk:system/app/iBingoLauncher2.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/lib/libandroid_animation.so:system/lib/libandroid_animation.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/lib/libbglw_mfd.so:system/lib/libbglw_mfd.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/lib/libCnvtGLPixel.so:system/lib/libCnvtGLPixel.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/lib/libgauss.so:system/lib/libgauss.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/lib/libshader.so:system/lib/libshader.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/lib/libSingleTypeCnvt.so:system/lib/libSingleTypeCnvt.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/com.ibingo.uistore_0.1.70-201312051501.apk:system/app/com.ibingo.uistore_0.1.70-201312051501.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/com_ibingo_security.apk:system/app/com_ibingo_security.apk
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/OppoU2SLockV42.apk:system/app/OppoU2SLockV42.apk
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/lib/libandroidgl20.so:system/lib/libandroidgl20.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/lib/libgdx.so:system/lib/libgdx.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/UI/WaterRipplesCofe.apk:system/app/WaterRipplesCofe.apk

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/data/baidubrowser_Android_3-0-7-50_1000789a.apk:data/app/baidubrowser_Android_3-0-7-50_1000789a.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/data/BaiduMap-5_0_lite_xx53829_hdpi_20130828.apk:data/app/BaiduMap-5_0_lite_xx53829_hdpi_20130828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/data/baiduSearchAndroid_1-0-18-93_7300090a.apk:data/app/baiduSearchAndroid_1-0-18-93_7300090a.apk
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/data/cleanmaster-release-380336.apk_m_2010000585.apk:data/app/cleanmaster-release-380336.apk_m_2010000585.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/data/myxj_pinsheng_common_20131213.apk:data/app/myxj_pinsheng_common_20131213.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3492/data/KuwaDouDIZhuv_V3493.apk:data/app/KuwaDouDIZhuv_V3493.apk
endif



#set dialer digits gravity,chengting,@20131223,begin
ifneq ($(strip $(BIRD_DIALER_DIGITS_GRAVITY)),)
   PRODUCT_PROPERTY_OVERRIDES += \
				bird.dialer.digits_gravity=$(BIRD_DIALER_DIGITS_GRAVITY)
endif
#set dialer digits gravity,chengting,@20131223,end

#MeiPaiKe camera,begin
ifeq ($(strip $(BIRD_MEIPAIKE_Camera)),yes) 
  PRODUCT_COPY_FILES += packages/apps/Bird_APK/MeipaiCamera/apk/MeipaiCamera.apk:system/app/MeipaiCamera.apk
  
  	src_file := packages/apps/Bird_APK/MeipaiCamera/extra/2d_scene/095A4A2C-0031E4B3-00000137-E90179E7
	des_file := system/arcsoft/camerahawk/data/2d_scene/095A4A2C-0031E4B3-00000137-E90179E7
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/MeipaiCamera/extra/2d_scene/0B31A832-0031E4B3-00000137-E9BFF24C
	des_file := system/arcsoft/camerahawk/data/2d_scene/0B31A832-0031E4B3-00000137-E9BFF24C
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/MeipaiCamera/extra/2d_scene/21A3A1DC-0031E4B3-00000137-BBAB1C02
	des_file := system/arcsoft/camerahawk/data/2d_scene/21A3A1DC-0031E4B3-00000137-BBAB1C02
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/MeipaiCamera/extra/2d_scene/47665724-0031E4B3-00000137-E97B9094
	des_file := system/arcsoft/camerahawk/data/2d_scene/47665724-0031E4B3-00000137-E97B9094
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/MeipaiCamera/extra/2d_scene/6A5443B0-0031E4B3-00000137-C4FEF1CD
	des_file := system/arcsoft/camerahawk/data/2d_scene/6A5443B0-0031E4B3-00000137-C4FEF1CD
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/MeipaiCamera/extra/2d_scene/8CBABF1C-0031E4B3-00000137-BBBD86A4
	des_file := system/arcsoft/camerahawk/data/2d_scene/8CBABF1C-0031E4B3-00000137-BBBD86A4
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/MeipaiCamera/extra/2d_scene/98C0B868-0031E4B3-00000137-EDFCB86F
	des_file := system/arcsoft/camerahawk/data/2d_scene/98C0B868-0031E4B3-00000137-EDFCB86F
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
	src_file := packages/apps/Bird_APK/MeipaiCamera/extra/2d_scene/F7D1DC5C-0031E4B3-00000137-BBCB38D1
	des_file := system/arcsoft/camerahawk/data/2d_scene/F7D1DC5C-0031E4B3-00000137-BBCB38D1
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
		
	src_file := packages/apps/Bird_APK/MeipaiCamera/so
	des_file := system/arcsoft/camerahawk/libs
		src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
        $(src_file)/$(file):$(des_file)/$(file))
        
  	src_file := packages/apps/Bird_APK/MeipaiCamera/extra/2d_scene/F9BAB1EC-0031E4B3-00000137-EA096658
	des_file := system/arcsoft/camerahawk/data/2d_scene/F9BAB1EC-0031E4B3-00000137-EA096658
	src_files := $(shell ls $(src_file))
		PRODUCT_COPY_FILES += $(foreach file, $(src_files), \
		$(src_file)/$(file):$(des_file)/$(file))
		
endif
#MeiPaiKe camera,end

#BIRD_TP_SMART_WAKE:wangfei 20131227 begin
ifeq ($(strip $(BIRD_TP_SMART_WAKE)),yes)  
  PRODUCT_PACKAGES += MotionRecognition
  
  SMART_WAKE_PIC_PATH_PREFIX := bird/smartwake/
  ifeq ($(strip $(LCM_HEIGHT)),1920) 
      SMART_WAKE_PIC_PATH := $(addprefix $(SMART_WAKE_PIC_PATH_PREFIX),fhd)
  else
      ifeq ($(strip $(LCM_HEIGHT)),1280) 
          SMART_WAKE_PIC_PATH := $(addprefix $(SMART_WAKE_PIC_PATH_PREFIX),hd)
      else
          ifeq ($(strip $(LCM_HEIGHT)),960) 
              SMART_WAKE_PIC_PATH := $(addprefix $(SMART_WAKE_PIC_PATH_PREFIX),qhd)
          else
              SMART_WAKE_PIC_PATH := $(addprefix $(SMART_WAKE_PIC_PATH_PREFIX),hdpi)
          endif
      endif
  endif
  
  define copy-all-png-files
  $(strip \
    $(if $(shell ls $(SMART_WAKE_PIC_PATH)), \
        $(foreach pic, $(shell ls $(SMART_WAKE_PIC_PATH)/$(strip $(1)) | grep -i '.png'), \
            $(SMART_WAKE_PIC_PATH)/$(strip $(1))/$(pic):system/usr/sw/$(strip $(1))/$(pic)) \
        , \
        $(error $(SMART_WAKE_PIC_PATH) doesn't exist) ) \
  )       
  endef
  
  ifeq ($(strip $(BIRD_TP_SMART_WAKE_UNLOCK)),yes)
    PRODUCT_COPY_FILES += $(call copy-all-png-files, unlock)
  endif
  
  ifeq ($(strip $(BIRD_TP_SMART_WAKE_DIALER)),yes)
    PRODUCT_COPY_FILES += $(call copy-all-png-files, dialer)
  endif
  
  ifeq ($(strip $(BIRD_TP_SMART_WAKE_CAMERA)),yes)
    PRODUCT_COPY_FILES += $(call copy-all-png-files, camera)
  endif
  
  ifeq ($(strip $(BIRD_TP_SMART_WAKE_BROWSER)),yes)
    PRODUCT_COPY_FILES += $(call copy-all-png-files, browser)
  endif
  
  ifeq ($(strip $(BIRD_TP_SMART_WAKE_QQ)),yes)
    PRODUCT_COPY_FILES += $(call copy-all-png-files, qq)
  endif
  
  ifeq ($(strip $(BIRD_TP_SMART_WAKE_WEIXIN)),yes)
    PRODUCT_COPY_FILES += $(call copy-all-png-files, weixin)
  endif
  
  ifeq ($(strip $(BIRD_TP_SMART_WAKE_MMS)),yes)
    PRODUCT_COPY_FILES += $(call copy-all-png-files, mms)
  endif
  
  ifeq ($(strip $(BIRD_TP_SMART_WAKE_TORCH)),yes)
    PRODUCT_COPY_FILES += $(call copy-all-png-files, torch)
  endif
  
  ifeq ($(strip $(BIRD_TP_SMART_WAKE_VIDEO)),yes)
    PRODUCT_COPY_FILES += $(call copy-all-png-files, video)
  endif
endif
#BIRD_TP_SMART_WAKE:wangfei 20131227 end

#bird ui contact photo,chengting,@20140111,begin
ifeq ($(strip $(BIRD_CONTACTS_PHOTOS_THEME)),bird_v2_circle)
  PRODUCT_PROPERTY_OVERRIDES += \
    bird.contact_photo.shape=circular
endif
#bird ui contact photo,chengting,@20140111,end

#ningzhiyu ro.tpwake.unlock
ifeq ($(strip $(BIRD_TP_WAKE_UNLOCK)),yes)  
   PRODUCT_PROPERTY_OVERRIDES += \
				ro.tpwake.unlock=true
endif

#performance monitor:wangfei 20140321 begin
ifeq ($(strip $(BIRD_PERFORMANCE_MONITOR)),yes)  
  PRODUCT_PACKAGES += PerformanceMonitor
endif
#performance monitor:wangfei 20140321 end



#BIRD_SIGNAL_NETWORK_STYLE_3G_4G, [100560], sunqi add 140303
ifeq ($(strip $(BIRD_SIGNAL_NETWORK_STYLE_3G_4G)),yes)
  PRODUCT_PROPERTY_OVERRIDES += \
    signal.network.style.3g.4g.def = true
endif

ifeq ($(BIRD_APK_CUSTOM_VM_3493),yes)
    PRODUCT_PACKAGES := $(filter-out Launcher2%, $(PRODUCT_PACKAGES))

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/baiduinput_V3493.apk:system/app/baiduinput_V3493.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/baiduinput_lib/libchiperencoder_v1_1.so:system/lib/libchiperencoder_v1_1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/baiduinput_lib/libkpencore.so:system/lib/libkpencore.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/baiduinput_lib/libshare_v2.so:system/lib/libshare_v2.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/baiduinput_lib/librabjni-1.so:system/lib/librabjni-1.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/baiduinput_lib/libtmfe30.so:system/lib/libtmfe30.so
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/iReader_V3493.apk:system/app/iReader_V3493.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/iread_lib/libiReader_common.so:system/lib/libiReader_common.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/iread_lib/libiReader_ebk3parser.so:system/lib/libiReader_ebk3parser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/iread_lib/libiReader_epubparser.so:system/lib/libiReader_epubparser.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/iread_lib/libiReader_filezip.so:system/lib/libiReader_filezip.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/iread_lib/libiReader_icu.so:system/lib/libiReader_icu.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/iread_lib/libiReader_txtparser.so:system/lib/libiReader_txtparser.so
    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/weimei_V3493.apk:system/app/weimei_V3493.apk
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/weimeiGame_V3493.apk:system/app/weimeiGame_V3493.apk
    
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/SystemPipe_V020_20131121_SpecForBoDao_sz.apk:system/app/SystemPipe_V020_20131121_SpecForBoDao_sz.apk
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/system/pipe.system.prop:system/etc/pipe.system.prop

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/iBingoLauncher2.apk:system/app/iBingoLauncher2.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/lib/libandroid_animation.so:system/lib/libandroid_animation.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/lib/libbglw_mfd.so:system/lib/libbglw_mfd.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/lib/libCnvtGLPixel.so:system/lib/libCnvtGLPixel.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/lib/libgauss.so:system/lib/libgauss.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/lib/libshader.so:system/lib/libshader.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/lib/libSingleTypeCnvt.so:system/lib/libSingleTypeCnvt.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/com.ibingo.uistore_0.1.70-201312051501.apk:system/app/com.ibingo.uistore_0.1.70-201312051501.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/com_ibingo_security.apk:system/app/com_ibingo_security.apk
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/OppoU2SLockV42.apk:system/app/OppoU2SLockV42.apk
    
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/lib/libandroidgl20.so:system/lib/libandroidgl20.so
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/lib/libgdx.so:system/lib/libgdx.so
ifeq ($(BIRD_APK_FRUITLIVEWALLPAPER_3493),yes)    
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/FruitLiveWallpaper.apk:system/app/FruitLiveWallpaper.apk
endif
ifneq ($(BIRD_APK_FRUITLIVEWALLPAPER_3493),yes)
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/UI/BaiHuaQiFang.apk:system/app/BaiHuaQiFang.apk
endif

    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/data/baidubrowser_Android_3-0-7-50_1000789a.apk:data/app/baidubrowser_Android_3-0-7-50_1000789a.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/data/BaiduMap-5_0_lite_xx53829_hdpi_20130828.apk:data/app/BaiduMap-5_0_lite_xx53829_hdpi_20130828.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/data/baiduSearchAndroid_1-0-18-93_7300090a.apk:data/app/baiduSearchAndroid_1-0-18-93_7300090a.apk
#    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/data/cleanmaster-release-380336.apk_m_2010000585.apk:data/app/cleanmaster-release-380336.apk_m_2010000585.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/data/myxj_pinsheng_common_20131213.apk:data/app/myxj_pinsheng_common_20131213.apk
    PRODUCT_COPY_FILES += packages/apps/Bird_APK/Vimoo_3493/data/KuwaDouDIZhuv_V3493.apk:data/app/KuwaDouDIZhuv_V3493.apk
endif

###set QQBroser as default Browser,we'll always block it's notification,chengting,@20140317
ifeq ($(BIRD_APK_QQBROWSER_AS_DEFAULT),yes)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/Browser.apk:system/app/Browser.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libchirp-jni.so:system/lib/libchirp-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libgif-jni.so:system/lib/libgif-jni.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/liblbs.so:system/lib/liblbs.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libmttwebcore.so:system/lib/libmttwebcore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libNativeRQD.so:system/lib/libNativeRQD.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libqqpatch.so:system/lib/libqqpatch.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libQrcodeDecoder.so:system/lib/libQrcodeDecoder.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libspeex.so:system/lib/libspeex.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libvadLib-v2.so:system/lib/libvadLib-v2.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libwebp_base.so:system/lib/libwebp_base.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libwebp_base_neon.so:system/lib/libwebp_base_neon.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libwonderplayer_init.so:system/lib/libwonderplayer_init.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libwtecdh.so:system/lib/libwtecdh.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/QQBrowserSystemDefault/libzipalign.so:system/lib/libzipalign.so

PRODUCT_COPY_FILES += bird/block_notification/notification_policy.xml:system/etc/notification_policy.xml
endif 

ifeq ($(BIRD_APK_MXPLAYER),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/mxplayer/MXVideoPlayer.apk:data/app/MXVideoPlayer.apk
endif
ifeq ($(BIRD_APK_BBM_MAXIS),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/BBM.apk:system/app/BBM.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libbbmcore.so:system/lib/libbbmcore.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libcares.so:system/lib/libcares.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libcurl.so:system/lib/libcurl.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libgnustl_shared.so:system/lib/libgnustl_shared.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libids.so:system/lib/libids.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libimg_codec_bmp.so:system/lib/libimg_codec_bmp.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libimg_codec_gif.so:system/lib/libimg_codec_gif.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libimg_codec_jpg.so:system/lib/libimg_codec_jpg.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libimg_codec_png.so:system/lib/libimg_codec_png.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libimg_codec_tif.so:system/lib/libimg_codec_tif.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libopenssl_crypto.so:system/lib/libopenssl_crypto.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libopenssl_ssl.so:system/lib/libopenssl_ssl.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libsbcrypto.so:system/lib/libsbcrypto.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libsqlite3.so:system/lib/libsqlite3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/lib/libtransport.so:system/lib/libtransport.so

#PRODUCT_COPY_FILES += packages/apps/Bird_APK/bbm/nexcom.apk:system/app/nexcom.apk
endif

ifeq ($(BIRD_APK_WHATSAPP),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Whatsapp/WhatsApp.apk:data/app/WhatsApp.apk
endif


ifeq ($(BIRD_APK_S11M_S999),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S11M_S999/comaptoidepartners.apk:data/app/comaptoidepartners.apk
#PRODUCT_COPY_FILES += packages/apps/Bird_APK/S11M_S999/s4y_mini_jogos.apk:data/app/s4y_mini_jogos.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S11M_S999/Science4youCampos.apk:data/app/Science4youCampos.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S11M_S999/Science4youClube.apk:data/app/Science4youClube.apk
#PRODUCT_COPY_FILES += packages/apps/Bird_APK/S11M_S999/Science4youEbook.apk:data/app/Science4youEbook.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S11M_S999/Science4youFestas.apk:data/app/Science4youFestas.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S11M_S999/Science4youLoja.apk:data/app/Science4youLoja.apk
endif

ifeq ($(BIRD_APK_S11M_CKK),data)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S11M_CKK/FlashlightFreeV1.1.apk:data/app/FlashlightFreeV1.1.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S11M_CKK/NEWS.apk:data/app/NEWS.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S11M_CKK/POPSTAR_1.3.0.apk:data/app/POPSTAR_1.3.0.apk
endif

ifeq ($(BIRD_APK_S11M_GT),system)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/S11M_GT/gadgetdoor.android.keyboard.hardware.apk:system/app/gadgetdoor.android.keyboard.hardware.apk
endif


#load default voice mail
ifeq ($(BIRD_LOAD_CUSTOM_VOICE_MAIL),yes)
  PRODUCT_COPY_FILES += mediatek/frameworks/base/telephony/etc/voicemail-conf.xml:system/etc/voicemail-conf.xml
endif

#BIRD_PHONEMANAGER_MENU lvhuaiyi add for phone manager
ifeq ($(BIRD_PHONEMANAGER_MENU),yes)
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Phone_Manager/Phone_Manager.apk:system/app/PhoneManager.apk
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Phone_Manager/libaddresshelper.so:system/lib/libaddresshelper.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Phone_Manager/libboa-1.0.3.so:system/lib/libboa-1.0.3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Phone_Manager/libdce.so:system/lib/libdce.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Phone_Manager/libdirsync.so:system/lib/libdirsync.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Phone_Manager/libNativeRQD.so:system/lib/libNativeRQD.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Phone_Manager/libspirit-1.0.1.so:system/lib/libspirit-1.0.1.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Phone_Manager/libTms2-Ams-Jni-1.4.0.so:system/lib/libTms2-Ams-Jni-1.4.0.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Phone_Manager/libTmsdk-2.0.3.so:system/lib/libTmsdk-2.0.3.so
PRODUCT_COPY_FILES += packages/apps/Bird_APK/Phone_Manager/libTmsSync.so:system/lib/libTmsSync.so

PRODUCT_COPY_FILES += bird/block_notification/notification_policy.xml:system/etc/notification_policy.xml
endif

#fota start
ifeq ($(strip $(FOTA_UPDATE_SUPPORT)), yes)
    PRODUCT_PACKAGES += SystemFota
    PRODUCT_COPY_FILES += packages/apps/SystemFota/fota-config.xml:system/etc/fota-config.xml
    PRODUCT_COPY_FILES += packages/apps/SystemFota/SystemFota/libluajava.so:system/lib/libluajava.so
endif
#fota end

#[103300],sos,chengting,@20140701,begin
ifeq ($(strip $(BIRD_SOS_CUSTOM)), yes)
    PRODUCT_PACKAGES += BirdSos

endif
#[103300],sos,chengting,@20140701,end
