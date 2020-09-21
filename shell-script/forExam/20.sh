#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Wrong arguments"
	exit 1
fi
if [ ! -d ${1} ]; then
	echo "Argument 1 is not a directory"
	exit 2
fi
if [ ! -d ${1} ]; then
	echo "Argument 2 is not a directory"
	exit 3
fi
DIR1=${1}
DIR2=${2}
NIZ=${3}

while read PATH; do
	echo ${PATH}
	FILE="$(basename ${PATH})"
	echo "${FILE}"
	mkdir -p $(dirname "${PATH}")
	mv -- ${FILE} ${PATH}

#	cp ${FILE} ${DIR2}
done < <(find ${DIR1} -mindepth 1 -type f | grep "${NIZ}" | sed -E "s/${DIR1}/${DIR2}/g")
