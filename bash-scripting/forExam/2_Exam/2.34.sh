#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Wrong arguments"
	exit 1
fi

if [ ! -d ${1} ]; then
	echo "Argument 1 is not a directory"
	exit 2
fi
	
if [ ! -r ${1} ]; then
	echo "Directory 1 is not readable"
	exit 3
fi

while read FILE; do

	name=$(echo "${FILE}" | awk -F '/' '{print $NF}')
	title=$(echo "${name}" | sed -E "s/\([^()]*\)//g" | sed -E "s/\.jpg$//" | tr -s ' ' | awk '{$1=$1}1')
	album=$(echo "${FILE}" | rev | awk -F ')' '{print $2}' | awk -F '(' '{print $1}' | rev  | awk '{$1=$1}1')
	if [ -z "${album}" ]; then
		album=$(echo "misc")
	fi
	data=$(find "${1}" -type f -printf "%TY-%Tm-%Td %f\n" | grep "${name}" | awk -F ' ' '{print $1}' )
	hesh=$(sha256sum "${FILE}" | awk -F ' ' '{print $1}' | cut -c1-16)
	
	echo "----------------------------------"
	echo "${FILE}" 
	echo "${name}"
	echo "${title}"
	echo "${album}"
	echo "${data}"
	echo "${hesh}"
	echo "-----------------------------------"

	title=$(echo "${title}.jpg")
	echo "${title}"

	mkdir -p ""${2}"/images"
	touch ""${2}"/images/"${hesh}".jpg"
	cp "${FILE}" "${2}/images/"${hesh}".jpg"

	mkdir -p ""${2}"/by-date/"${data}"/by-album/'${album}'/by-title/"
	ln -s ""${2}"/images/"${hesh}".jpg" ""${2}"/by-date/"${data}"/by-album/'${album}'/by-title/'${title}'"

	mkdir -p ""${2}"/by-date/"${data}"/by-title/"
	ln -s ""${2}"/images/"${hesh}".jpg" ""${2}"/by-date/"${data}"/by-title/'${title}'"
	
	mkdir -p ""${2}"/by-album/'${album}'/by-date/"${data}"/by-title/"
	ln -s ""${2}"/images/"${hesh}".jpg" ""${2}"/by-album/'${album}'/by-date/"${data}"/by-title/'${title}'"

	mkdir -p ""${2}"/by-album/'${album}'/by-title/"
	ln -s "${2}/images/${hesh}.jpg" "${2}/by-album/'${album}'/by-title/'${title}'"

	mkdir -p "${2}/by-title/"
	ln -s "${2}/images/${hesh}.jpg" "${2}/by-title/'${title}'"

done < <(find ${1} -mindepth 1 -type f | egrep "\.jpg$")



















