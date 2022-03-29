#!/bin/bash

devs=$(vDevSetting.py --list)

for d in $devs; do
	vDevSetting.py --dev "$d"
	vDevSetting.py --dev "$d"
done

