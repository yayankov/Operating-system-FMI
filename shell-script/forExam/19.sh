#!/bin/bash

if [ $# -lt 1 -o $# -gt 2 ]; then
	echo "Wrong arguments"
	exit 1
fi

if [ ! -d ${1} ]; then
	echo "Argument 1 is not a directory"
	exit 2
fi

if [ ! -r ${1} ]; then
	echo "The directory is not readable"
	exit 3
fi

if [ $# -eq 2 ]; then
	grep -qE "^[+-]?[0-9]+$" <(echo "${2}")
	if [ "$?" -ne 0 ]; then
		echo "Argument 2 is not a number"
		exit 4
	fi

	while read FILE HARDLINKS; do
		if [ ${HARDLINKS} -ge ${2} ]; then
			echo "${FILE}"
		fi
	done < <(find "${1}" -type f -printf "%f %n\n")
fi

if [ $# -eq 1 ]; then
	find ${1} -mindepth 1 -type l -printf "%Y %f\n" | grep -E "^[NL]" | awk '{print $2}'
fi
