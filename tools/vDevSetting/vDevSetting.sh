#!/bin/bash

devs=$(v4l2-ctl --list-devices | grep video)
#echo "$devs"

conCamFace="usb-0000:0a:00.3-1.1.4"
conCamHw="usb-0000:01:00.0-1"
conCapOsc="usb-0000:0a:00.3-1.4.1"

cDev=""

camFaceSet()
{
	echo "Cam Face"
	true
}

camHwSet()
{
	echo "Cam Hardware"
	true
}

capOscSet()
{
	echo "Cap Oscilloscope"
	true
}

for d in $devs ; do

	meta=$(v4l2-ctl -D -d "$d" --all | grep "Format Meta")
	#echo "$meta"

	if [ -n "$meta" ]; then
		continue
	fi

	cDev="$d"
	dName=$(echo "$d" | rev | cut -d "/" -f 1 | rev)
	#echo "$dName"

	udevadm info --name="$d" --attribute-walk > "${dName}_udev.txt"
	v4l2-ctl -D -d "$d" --list-ctrls > "${dName}_v4l2-ctl.txt"

	con=$(v4l2-ctl -D -d "$d" | grep "Bus info" | uniq | sed "s:  *: :g" | cut -d " " -f 4)
	#echo "$d: $con"

	if [ "$con" = "$conCamFace" ]; then
		camFaceSet
		continue
	fi

	if [ "$con" = "$conCamHw" ]; then
		camHwSet
		continue
	fi

	if [ "$con" = "$conCapOsc" ]; then
		capOscSet
		continue
	fi

	echo "$d: $con"

done

