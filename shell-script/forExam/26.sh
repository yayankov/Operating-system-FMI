#!/bin/bash

if [ $# -ne 2 ]; then
	echo "WRONG ARGUMENTS"
	exit 1
fi

if [ ! -d ${2} ]; then
	echo "Argument 2 is not a directory"
	exit 2
fi

if [ ! -f ${1} ]; then
	echo "Argument 1 is not a file"
	exit 3
fi
touch "${2}/dict_result.txt"


CNT=1
while read NAME; do
	touch "${2}/${CNT}"
	cat ${1} | grep -w "${NAME}" > "${2}/${CNT}" 
	echo "${NAME};${CNT}" >> "${2}/dict_result.txt"
	CNT=$(( ${CNT} + 1 ))
done < <(cat ${1} | cut -d ':' -f1 | sed -E "s/ \(.*\)//g" | sort -n | uniq)
