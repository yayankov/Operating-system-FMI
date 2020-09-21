#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "Wrong number of arguments"
	exit 1
fi

FILE="${1}"
DIR="${2}"

if [ ! -f "${FILE}" ]; then
	echo "Argument 1 is not a file"
	exit 2
fi

if [ ! -r "${FILE}" ]; then
    echo "Argument 1 is not a readable"
    exit 3
fi

if [ ! -d "${DIR}" ]; then
    echo "Argument 2 is not a directory"
    exit 4
fi

CNT=0

while read FILE2; do
	cmp -s "${FILE}" "${FILE2}"
	if [ $? -eq 0 ]; then
		echo "${FILE2}"
		CNT=$((++CNT))
	fi
done < <(find ${DIR} -type f 2>/dev/null)

if [ "${CNT}" -eq 0 ]; then
	echo "No found copies"
	exit 5
fi

