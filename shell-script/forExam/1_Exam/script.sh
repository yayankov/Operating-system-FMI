#!/bin/bash

#if [ "yes" = $(cat "${1}/dir2/file" | grep -o "yes") ]; then
##	echo "Thank GOD"
#fi
temp=$(mktemp)
#cat "${1}/tree/file1" | sort -n > ${temp}
#cat "${temp}" > "${1}/tree/file1"

if [ ${1} -nt ${2} ]; then
	echo ${1}
else 	
	echo ${2}
fi

#find "${1}/dir2" -type f | xargs rm
#echo "YEEEEEEEEEEES" >> "${1}/dir2/file"
#cat "${1}/dir2/file"
