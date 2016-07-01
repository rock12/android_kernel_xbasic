# !/bin/bash
######## jiali add 20130116 ##########
# script run after build userdata files ,just before make userdata.img
# it will be called at build/core/Makefile and makeuser
# usage :  prepack_uerdataimg.sh dataoutpath userdataimgpacksize
# example: prepack_uerdataimg.sh out/target/product/xxx/data 820M
# for nand: only one param

BIRDTOOLPATH=bird/tool
## $1 : $PROJECT_NAME or dataoutpath , both OK
# check parameters
if [[ "$1" == */* ]]; then
	param=${1##*product/}
	param=${param%%/*}
else
	param=$1
fi
echo "PROJECT_NAME is $param"
if [[ -z "$2" ]]; then
	echo "image type is NAND"
elif [[ "$2" == *M ]]; then
	echo "EMMC pack size: $2"
else
	echo "param 2 error, EMMC: xxM, NAND: only one param"
	exit
fi
## set file permission, enable run file
chmod 755 $BIRDTOOLPATH/outputBirdFirmwareInfo $BIRDTOOLPATH/aapt $BIRDTOOLPATH/genCRCChecksum

## output firmware information to check apks
./$BIRDTOOLPATH/outputBirdFirmwareInfo $param
rm -rf out/target/product/$param/BirdFirmwareInfo.txt

#shenzhiwang, zip hide app start
zipPath="out/target/product/$param/data/mediatek/kernel/"
if [ -d "$zipPath" ]; then
    cd $zipPath
    if [ -d "bcopy" ]; then
        if [ -f "bcopy.bpp" ]; then
            rm -r bcopy.bpp
            echo rm -r bcopy.bpp
        else
            echo no bcopy.bpp
        fi
        zip -rq bcopy.bpp bcopy/*
        rm -r bcopy
        echo zip BIRD_HIDE_APP_SUPPORT
    else
        echo no bcopy
    fi
    cd ../../../../../../../
    echo BIRD_HIDE_APP_SUPPORT
else
    echo No BIRD_HIDE_APP_SUPPORT
fi
#shenzhiwang, zip hide app end

## generate files crc info for Bird ROM Tool , to make sure unpacking-userdataimage is safe
./$BIRDTOOLPATH/genCRCChecksum out/target/product/$param/data out/target/product/$param/data

## recode image pack size for Bird ROM Tool
echo $2 >> out/target/product/$param/data/checksum.crc