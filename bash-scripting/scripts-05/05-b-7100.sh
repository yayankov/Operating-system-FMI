#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "wrong number of arguments"
	exit 1
fi

if [ ! -d "${1}" ]; then
	echo "Argument 1 is not a directory"
	exit 2
elif [ ! -r "${1}" ]; then
	echo "The file is not readable"
	exit 3
fi

egrep -vq '^[+-]?[0-9]+$' <(echo "${2}")
if [ $? -eq 0 ]; then
	echo "Argument 2 is not an integer"
	exit 4
fi

DIR="${1}"
NUM="${2}"

while read SIZE FILE; do

	if [ "${SIZE}" -gt "${NUM}" ]; then
		echo "${FILE}"	
	fi
done < <(find "${DIR}" -type f 2>/dev/null -printf "%s %f\n")






