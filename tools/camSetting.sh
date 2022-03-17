#!/bin/bash

devs=$(v4l2-ctl --list-devices | grep video)
#echo "$devs"

serCam1="E2A83F8E"
serCam2="665A8DAE"

cam1Set()
{
	true
}

cam2Set()
{
	true
}

for d in $devs ; do

	meta=$(v4l2-ctl -D -d "$d" --all | grep "Format Meta")
	#echo "$meta"

	if [ -n "$meta" ]; then
		continue
	fi

	ser=$(v4l2-ctl -D -d "$d" | grep Serial | cut -d " " -f 13)
	#echo "$d: $ser"

	if [ "$ser" = "$serCam1" ]; then
		echo "Cam 1"
		cam1Set
	fi

	if [ "$ser" = "$serCam2" ]; then
		echo "Cam 2"
		cam2Set
	fi

done

