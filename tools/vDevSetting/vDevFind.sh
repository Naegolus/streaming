#!/bin/bash

if [ -z "$1" ]; then
	echo "Usage: vDevFind.sh <usb path>"
	exit 1
fi

devs=$(v4l2-ctl --list-devices | grep video)

for d in $devs ; do

	meta=$(v4l2-ctl -D -d "$d" --all | grep "Format Meta")
	#echo "$meta"

	if [ -n "$meta" ]; then
		continue
	fi

	cDev="$d"
	dName=$(echo "$d" | rev | cut -d "/" -f 1 | rev)
	#echo "$dName"

	con=$(v4l2-ctl -D -d "$d" | grep "Bus info" | uniq | sed "s:  *: :g" | cut -d " " -f 4)
	#echo "$d: $con"

	if [ ! "$1" = "$con" ]; then
		continue
	fi

	echo "$d"

	continue

done

