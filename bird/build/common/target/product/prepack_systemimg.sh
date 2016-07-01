# !/bin/bash
######## jiali add 20130116 ##########
# script run after build system files ,just before make system.img
# it will be called at build/core/Makefile and makesys
# usage :  prepack_systemimg.sh systemoutpath systemimgpacksize
# example: prepack_systemimg.sh out/target/product/xxx/system 399M
# for nand: only one param

BIRDTOOLPATH=bird/tool
## $1 : $PROJECT_NAME or systemoutpath , both OK
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
chmod 755 $BIRDTOOLPATH/encrypt_for_BirdCustTool $BIRDTOOLPATH/outputBirdFirmwareInfo $BIRDTOOLPATH/aapt $BIRDTOOLPATH/genCRCChecksum

## encrypt for Bird ROM Tool
./$BIRDTOOLPATH/encrypt_for_BirdCustTool $param

## output firmware information to check apks
./$BIRDTOOLPATH/outputBirdFirmwareInfo $param
rm -rf out/target/product/$param/BirdFirmwareInfo.txt

## generate files crc info for Bird ROM Tool , to make sure unpacking-systemimage is safe
./$BIRDTOOLPATH/genCRCChecksum out/target/product/$param/system out/target/product/$param/system

## recode image pack size for Bird ROM Tool
echo $2 >> out/target/product/$param/system/checksum.crc