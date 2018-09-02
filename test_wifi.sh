#!/bin/sh

kill -9  `ps -A |grep "wpa_supplicant"| grep -v "grep"  | awk '{print $1}' `

#insmod bcmdhd.ko "firmware_path=/lib/firmware/brcm/fw_bcm43438a1.bin nvram_path=/lib/firmware/brcm/nvram_ap6212a.txt" 


insmod bcmdhd.ko "firmware_path=/lib/firmware/brcm/fw_bcm43438a1_apsta.bin nvram_path=/lib/firmware/brcm/nvram_ap6212a.txt" 

sleep 3 

result_name="/sys/class/rfkill"
if [  -d $result_name ];then
	temp=`cat /sys/class/rfkill/rfkill0/name`
	if [ "$temp" = "phy0" ];then
	  echo 1 > /sys/class/rfkill/rfkill0/state
	else
	  exit 
	fi

	sleep 2
	kill -9  `ps -A |grep "wpa_supplicant"| grep -v "grep"  | awk '{print $1}' `
	#wpa_supplicant -D nl80211 -i wlan0 -c wifi.conf
	wpa_supplicant -D wext -B -i wlan0 -c wifi.conf
fi


