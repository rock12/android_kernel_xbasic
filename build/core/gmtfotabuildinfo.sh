#!/bin/bash

if [ $# -lt 1 ] ; then
echo "Usage: $0 build.prop"
echo "hufeng.mao@generalmobi.com"
exit
fi

echo ""
echo "# begin GMT FOTA properties"

# true|false,用于手动配置brand与model的开关
GMT_MANUAL_SET=false

# 当GMT_MANUAL_SET宏为true时有用
LOCAL_PROJECT_BRAND_VALUE="Fly"
LOCAL_PROJECT_MODEL_VALUE="IQ 4412"

# 根据项目配置一下内容
LOCAL_PROJECT_BRAND="ro.product.manufacturer="
LOCAL_PROJECT_MODEL="ro.product.model="
LOCAL_PROJECT_VERSION="ro.build.display.id="

# DO NOT MOFITY!不要修改这里的值
GMT_FOTA_BRAND="ro.gmt.fota.brand="
GMT_FOTA_MODEL="ro.gmt.fota.model="
GMT_FOTA_VERSION="ro.gmt.fota.version="

if [ "$GMT_MANUAL_SET" == "true" ] ; then
	echo "$GMT_FOTA_BRAND=$LOCAL_PROJECT_BRAND_VALUE"
	echo "$GMT_FOTA_MODEL=$LOCAL_PROJECT_MODEL_VALUE"
else
	echo "$(grep "$LOCAL_PROJECT_BRAND" "$1" | sed "s/$LOCAL_PROJECT_BRAND/$GMT_FOTA_BRAND/" )"
	echo "$(grep "$LOCAL_PROJECT_MODEL" "$1" | sed "s/$LOCAL_PROJECT_MODEL/$GMT_FOTA_MODEL/" )"
fi

echo "$(grep "$LOCAL_PROJECT_VERSION" "$1" | sed "s/$LOCAL_PROJECT_VERSION/$GMT_FOTA_VERSION/" )"
# echo "$grep "ro.build.display.id=" "$1" | sed "s/ro. Radium/3G7334i/" "

echo "# end GMT FOTA properties"
