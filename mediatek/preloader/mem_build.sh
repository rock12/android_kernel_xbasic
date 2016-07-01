#!/bin/bash
# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2011. All rights reserved.
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


##############################################################
# Program:
# Program to create ALPS preloader binary
#

function build_preloader () {

    if [ "$2" != "" ]; then export TARGET_PRODUCT=$2; fi
    if [ "$1" != "" ]; then PRODUCT=$1; fi

    source ../../mediatek/build/shell.sh ../../ preloader
    CUR_DIR=`pwd`

    ##############################################################
    # Variable Initialization
    #
    PL_PATH=bin
    PL_IMAGE=preloader_${MTK_PROJECT}.bin
    PL_ELF_IMAGE=preloader_${MTK_PROJECT}.elf

    ##############################################################
    # Binary Generation
    #

    if [ ${PRODUCT} == "mem_" ]; then
        export BUILDE_MEM_VERSION=TRUE
    else
        export BUILDE_MEM_VERSION=FALSE
    fi

    if [ ${PRODUCT} == "sram_" ]; then
        export BUILDE_SRAM_VERSION=TRUE
    else
        export BUILDE_SRAM_VERSION=FALSE
    fi

    make

    if [ ! -f "${PL_PATH}/${PL_IMAGE}" ]; then echo "1 BUILD FAIL.${PL_PATH}/${PL_IMAGE}"; exit 1; fi

    PL_FUN_MAP=${PRODUCT}function.map

    source ${MTK_PATH_PLATFORM}/check_size.sh

    #for clean variable
    PRODUCT=""
}

function ns_chip () {

    ##############################################################
    # Only Support Non-Secure Chip
    #

    echo ""
    echo "[ Only for Non-Secure Chip ]"
    echo "============================================"

    GFH_PATH=${CHIP_CONFIG_PATH}/ns/

    ##############################################################
    # INITIALIZE GFH
    #

    if [ "${MTK_EMMC_SUPPORT}" == "yes" ]; then
        GFH_INFO=${GFH_PATH}/GFH_INFO_EMMC.txt
    else
        GFH_INFO=${GFH_PATH}/GFH_INFO.txt
    fi
    GFH_HASH=${GFH_PATH}/GFH_HASH.txt

    ##############################################################
    # ATTACH GFH
    #

    echo ""
    echo "[ Attach ${MTK_PLATFORM} GFH ]"
    echo "============================================"
    echo " : GFH_INFO             - ${GFH_INFO}"
    echo " : GFH_HASH             - ${GFH_HASH}"

    chmod u+w ${PL_PATH}/${PL_IMAGE}
    mv -f ${PL_PATH}/${PL_IMAGE} ${PL_PATH}/${PL_IMAGE/%.bin/_NO_GFH.bin}
    cp -f ${GFH_INFO} ${PL_PATH}/${PL_IMAGE}
    chmod u+w ${PL_PATH}/${PL_IMAGE} ${PL_PATH}/${PL_IMAGE/%.bin/_NO_GFH.bin}
    cat ${PL_PATH}/${PL_IMAGE/%.bin/_NO_GFH.bin} >> ${PL_PATH}/${PL_IMAGE}
    cat ${GFH_HASH} >> ${PL_PATH}/${PL_IMAGE}

    ##############################################################
    # PROCESS BOOT LOADER
    #

    chmod 777 ${PBP_TOOL}/PBP

    ${PBP_TOOL}/PBP ${PL_PATH}/${PL_IMAGE}
    if [ $? -eq 0 ] ; then
        echo "${PBP_TOOL}/PBP pass !!!!"
    else
        echo "===BUILD FAIL. ${PBP_TOOL}/PBP return fail==="
        exit 1;
    fi
}

function s_chip_support () {

    if [ "$1" != "" ]; then PRODUCT=$1; fi

    ##############################################################
    # Can Support Secure Chip
    #

    echo ""
    echo "[ Enable Secure Chip Support ]"
    echo "============================================"

    GFH_PATH=${CHIP_CONFIG_PATH}/s/gfh
    CONFIG_PATH=${CHIP_CONFIG_PATH}/s/cfg
    KEY_PATH=${CHIP_CONFIG_PATH}/s/key

    ##############################################################
    # INITIALIZE CONFIG and KEY
    #

    CHIP_CONFIG=${CONFIG_PATH}/CHIP_CONFIG.ini
    CHIP_KEY=${KEY_PATH}/CHIP_TEST_KEY.ini

    ##############################################################
    # INITIALIZE GFH
    #

    if [ "${MTK_EMMC_SUPPORT}" == "yes" ]; then
        if [ ${PRODUCT} == "mem_" ]; then
            GFH_INFO=${GFH_PATH}/GFH_INFO_EMMC_MEM.txt
        elif [ ${PRODUCT} == "sram_" ]; then
            GFH_INFO=${GFH_PATH}/GFH_INFO_EMMC_MEM_SRAM.txt
        else
            GFH_INFO=${GFH_PATH}/GFH_INFO_EMMC.txt
        fi
    else
        if [ ${PRODUCT} == "mem_" ]; then
            GFH_INFO=${GFH_PATH}/GFH_INFO_MEM.txt
        elif [ ${PRODUCT} == "sram_" ]; then
            GFH_INFO=${GFH_PATH}/GFH_INFO_MEM_SRAM.txt
        else
            GFH_INFO=${GFH_PATH}/GFH_INFO.txt
        fi
    fi
    GFH_SEC_KEY=${GFH_PATH}/GFH_SEC_KEY.txt
    GFH_ANTI_CLONE=${GFH_PATH}/GFH_ANTI_CLONE.txt
    GFH_HASH_SIGNATURE=${GFH_PATH}/GFH_HASH_AND_SIG.txt
    GFH_PADDING=${GFH_PATH}/GFH_PADDING.txt

    source ${CONFIG_PATH}/SECURE_JTAG_CONFIG.ini
    if [ "${SECURE_JTAG_ENABLE}" == "TRUE" ]; then
        SECURE_JTAG_GFH=${GFH_PATH}/GFH_SEC_CFG_JTAG_ON.txt
        echo " : SECURE_JTAG_ENABLE - TRUE"
    elif [ "${SECURE_JTAG_ENABLE}" == "FALSE" ]; then
        SECURE_JTAG_GFH=${GFH_PATH}/GFH_SEC_CFG_JTAG_OFF.txt
        echo " : SECURE_JTAG_ENABLE - FALSE"
    else
        echo "BUILD FAIL. SECURE_JTAG_ENABLE not defined in ${CONFIG_PATH}/SECURE_JTAG_CONFIG.ini"
        exit 1;
    fi

    ##############################################################
    # ATTACH GFH
    #

    echo ""
    echo "[ Attach ${MTK_PLATFORM} GFH ]"
    echo "============================================"
    echo " : GFH_INFO             - ${GFH_INFO}"
    echo " : GFH_SEC_KEY          - ${GFH_SEC_KEY}"
    echo " : GFH_ANTI_CLONE       - ${GFH_ANTI_CLONE}"
    echo " : GFH_JTAG_CFG         - ${SECURE_JTAG_GFH}"
    echo " : GFH_PADDING          - ${GFH_PADDING}"
    echo " : GFH_HASH_SIGNATURE   - ${GFH_HASH_SIGNATURE}"

    chmod u+w ${PL_PATH}/${PL_IMAGE}
    mv -f ${PL_PATH}/${PL_IMAGE} ${PL_PATH}/${PL_IMAGE/%.bin/_NO_GFH.bin}
    cp -f ${GFH_INFO} ${PL_PATH}/${PL_IMAGE}
    chmod 777 ${PL_PATH}/${PL_IMAGE}
    cat ${GFH_SEC_KEY} >> ${PL_PATH}/${PL_IMAGE}
    cat ${GFH_ANTI_CLONE} >> ${PL_PATH}/${PL_IMAGE}
    cat ${SECURE_JTAG_GFH} >> ${PL_PATH}/${PL_IMAGE}
    cat ${GFH_PADDING} >> ${PL_PATH}/${PL_IMAGE}
    chmod u+w ${PL_PATH}/${PL_IMAGE} ${PL_PATH}/${PL_IMAGE/%.bin/_NO_GFH.bin}
    cat ${PL_PATH}/${PL_IMAGE/%.bin/_NO_GFH.bin} >> ${PL_PATH}/${PL_IMAGE}
    cat ${GFH_HASH_SIGNATURE} >> ${PL_PATH}/${PL_IMAGE}

    echo ""
    echo "[ Load Configuration ]"
    echo "============================================"
    echo " : CONFIG               - ${CHIP_CONFIG}"
    echo " : RSA KEY              - ${CHIP_KEY}"
    echo " : AC_K                 - ${CHIP_KEY}"

    ##############################################################
    # PROCESS BOOT LOADER
    #

    chmod 777 ${PBP_TOOL}/PBP

    ${PBP_TOOL}/PBP -m ${CHIP_CONFIG} -i ${CHIP_KEY} ${PL_PATH}/${PL_IMAGE}
    if [ $? -eq 0 ] ; then
        echo "${PBP_TOOL}/PBP pass !!!!"
    else
        echo "===BUILD FAIL. ${PBP_TOOL}/PBP return fail==="
        exit 1;
    fi

    #for clean variable
    PRODUCT=""
}

function key_encode () {

    ##############################################################
    # Encode Key
    #

    KEY_ENCODE_TOOL=tools/ke/KeyEncode
    chmod 777 ${KEY_ENCODE_TOOL}
    if [ -e ${KEY_ENCODE_TOOL} ]; then

        ./${KEY_ENCODE_TOOL} ${PL_PATH}/${PL_IMAGE} KEY_ENCODED_PL

        if [ $? -eq 0 ] ; then
            echo "${KEY_ENCODE_TOOL} pass !!!!"
        else
            echo "===BUILD FAIL. ${KEY_ENCODE_TOOL} return fail==="
            exit 1;
        fi

        if [ -e KEY_ENCODED_PL ]; then
            rm ${PL_PATH}/${PL_IMAGE}
            mv KEY_ENCODED_PL ${PL_PATH}/${PL_IMAGE}
        fi
    fi
}

function post_process () {

    ##############################################################
    # Binary Secure Postprocessing
    #

    if [ "$1" != "" ]; then PRODUCT=$1; fi
    PBP_TOOL=tools/pbp
    CUSTOM_PATH=${MTK_ROOT_CUSTOM}/${MTK_PROJECT}/security
    CHIP_CONFIG_PATH=${CUSTOM_PATH}/chip_config
    if [ -e ${PBP_TOOL}/PBP ]; then

        echo ""
        echo "[ Pre-loader Post Processing ]"
        echo "============================================"

        ##############################################################
        # ENCODE KEY FIRST
        #
        if [ ${PRODUCT} == "sram_" ]; then
            echo "============================================"
            echo "$======= ${PRODUCT}, No key_encode ======="
            echo "============================================"
        else
            key_encode;
        fi

        ##############################################################
        # CHECK CHIP TYPE
        #

        if [ -e ${CHIP_CONFIG_PATH}/s/gfh/GFH_INFO.txt ] || [ -e ${CHIP_CONFIG_PATH}/s/gfh/GFH_INFO_EMMC.txt ]; then

            echo ""
            echo "[ Load Chip Config. ]"
            echo "============================================"
            echo " : MTK_SEC_CHIP_SUPPORT - ${MTK_SEC_CHIP_SUPPORT}"

            if [ "${MTK_SEC_CHIP_SUPPORT}" == "no" ]; then

                ##############################################################
                # ONLY SUPPORT NON-SECURE CHIP
                #

                CHIP_CONFIG_PATH=${MTK_PATH_PLATFORM}/gfh/default
                ns_chip;

            elif [ "${MTK_SEC_CHIP_SUPPORT}" == "yes" ]; then

                ##############################################################
                # CAN SUPPORT SECURE CHIP
                #

                CHIP_CONFIG_PATH=${CUSTOM_PATH}/chip_config
                s_chip_support $1;

            else

                echo "BUILD FAIL. MTK_SEC_CHIP_SUPPORT not defined in ProjectConfig.mk"
                exit 1;
            fi

        else

            ##############################################################
            # NO CONFIGURATION IS FOUND. APPLY DEFAULT SETTING
            #

            echo "${CHIP_CONFIG_PATH}/s/gfh/GFH_INFO.txt not found."
            echo "Suppose it is non-secure chip and apply default config."
            CHIP_CONFIG_PATH=${MTK_PATH_PLATFORM}/gfh/default
            ns_chip;
        fi
    fi
}

function dump_build_info () {

    ##############################################################
    # Dump Message
    #

    if [ "$1" != "" ]; then PRODUCT=$1; fi
    echo ""
    echo "============================================"
    echo "${MTK_PROJECT} preloader load"
    echo "${PL_PATH}/${PL_IMAGE} built at"
    echo "time : $(date)"
    echo "img size : $(stat -c%s "${PL_PATH}/${PL_IMAGE}")" byte
    echo "bss size : 0x$(readelf -SW "${PL_PATH}/${PL_ELF_IMAGE}" | grep "bss" | awk '{if (NF==11) print $6; else print $7;}')" byte
    echo "============================================"

    PL_PATH=bin
    PL_ELF_IMAGE=preloader_${MTK_PROJECT}.elf

    chmod a+w ${PL_PATH}/${PL_IMAGE}

    # copy bin/elf from preloader/bin/xxx to preloader/xxx
    cp -f ${PL_PATH}/${PL_ELF_IMAGE} ${PRODUCT}${PL_ELF_IMAGE}
    cp -f ${PL_PATH}/${PL_IMAGE} ${PL_PATH}/${PRODUCT}${PL_IMAGE}
    cp -f ${PL_PATH}/${PRODUCT}${PL_IMAGE} .

    #for clean variable
    PRODUCT=""
}

function copy_binary () {

    ##############################################################
    # Copy Binary to Output Direcory
    #

    if [ "$1" != "" ]; then PRODUCT=$1; fi

    copy_to_legacy_download_flash_folder   ${PL_PATH}/${PRODUCT}${PL_IMAGE}

    #for clean variable
    PRODUCT=""
}


##############################################################
# Main Flow
#
if [ ${MTK_MEM_PRESERVED_MODE_ENABLE} == "yes" ]; then
build_preloader sram_;
post_process sram_;
dump_build_info sram_;
copy_binary sram_;

build_preloader mem_;
post_process mem_;
dump_build_info mem_;
copy_binary mem_;
fi

build_preloader;
post_process;
dump_build_info;
copy_binary;


