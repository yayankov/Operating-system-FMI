#!/bin/bash

if [ ! -d ${1} ]; then
	echo "Not a directory"
	exit 1
fi

grep -vqE '^[+-]?[0-9]+$' <(echo ${1})
if [ $? -ne 0 ]; then
	echo "Not a number"
	exit 2
fi

sum_dir=0
while read size; do
	if [ ${size} -gt ${2} ]; then
		sum_dir=$(( sum_dir+${size} ))
	fi
done < <(find ${1} -mindepth 1 -maxdepth 1 -type f -printf "%s\n")

echo ${sum_dir}

