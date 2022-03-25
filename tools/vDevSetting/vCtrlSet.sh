#!/bin/bash

# $1 .. Video device file
# $2 .. Control name
# $3 .. Value

if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ]; then
	echo "Usage: vCtrlSet.sh <dev> <attr name> <value>"
	exit 1
fi

#echo "$1 $2=$3"
v4l2-ctl --device "$1" --set-ctrl "$2"="$3"

