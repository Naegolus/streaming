#!/bin/bash

# v4l2-ctl -D -d /dev/video0 --all > ./info/video0_v4l2-ctl.txt

fileConvert()
{

	bf=$(echo "$1" | rev | cut -d "." -f "2-" | rev)
	#echo "$bf"

	jf="${bf}.json"

	printf "{\n" > "$jf"
	printf "\t\"ctrls\" :\n" >> "$jf"
	printf "\t{\n" >> "$jf"

	ctlArea=""
	firstCtrl="true"

	while IFS= read -r l; do

		ctlAreaCheck=$(echo "$l" | grep "Controls")

		if [ -n "$ctlAreaCheck" ]; then
			ctlArea="true"
			continue
		fi

		if [ -z "$ctlArea" ]; then
			continue
		fi

		if [ -z "$l" ]; then
			continue
		fi

		eCheck=$(echo "$l" | grep "=")

		if [ -z "$eCheck" ]; then
			continue
		fi

		#echo "$l"
		cl=$(echo "$l" | sed "s:  *: :g" | sed "s/) ://g" | sed "s:[()]::g")
		#echo "$cl"

		name=$(echo "$cl" | cut -d " " -f 2)
		#printf "%-20s %s\n" "name" "$name"

		dtype=$(echo "$cl" | cut -d " " -f 4)
		#printf "%-20s %s\n" "dtype" "$dtype"

		par=$(echo "$cl" | cut -d " " -f "5-")

		if [ -z "$firstCtrl" ]; then
			printf ",\n" >> "$jf"
		fi

		printf "\t\t\"$name\" :\n" >> "$jf"
		printf "\t\t{\n" >> "$jf"
		printf "\t\t\t\"type\" : \"$dtype\",\n" >> "$jf"

		firstPar="true"

		for p in $par; do

			eCheck=$(echo "$p" | grep "=")

			if [ -z "$eCheck" ]; then
				continue
			fi

			if [ -z "$firstPar" ]; then
				printf ",\n" >> "$jf"
			fi

			id=$(echo "$p" | cut -d "=" -f 1)
			val=$(echo "$p" | cut -d "=" -f 2)

			#printf "%-20s %s\n" "$id" "$val"

			printf "\t\t\t\"%s\" : \"%s\"" "$id" "$val" >> "$jf"

			firstPar=""

		done

		printf "\n" >> "$jf"
		printf "\t\t}" >> "$jf"

		#echo

		firstCtrl=""

	done < "$1"

	printf "\n" >> "$jf"
	printf "\t}\n" >> "$jf"
	printf "}\n" >> "$jf"

}

if [ -n "$1" ]; then
	fileConvert "$1"
	exit 0
fi

for f in ./*.txt; do

	vf=$(echo "$f" | grep video | grep ctl)

	if [ -z "$vf" ]; then
		continue
	fi

	#echo "$f"

	fileConvert "$f"

done

