#!/bin/bash

file=$(mktemp)

input=$(</dev/stdin)
echo "${input}" > ${file}

min_abs=$(cat ${file} | egrep "^[+-]?[0-9]+$" | sort -n | head -n 1 | sed "s/-//g")
max_abs=$(cat ${file} | egrep "^[+-]?[0-9]+$" | sort -n | tail -n 1)

if [ ${min_abs} -gt ${max_abs} ]; then
	echo "-${min_abs}"
elif [ ${min_abs} -lt ${max_abs} ]; then
	echo "${max_abs}"
else
	echo "-${min_abs}"
	echo "${max_abs}"
fi
