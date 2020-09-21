#!/bin/bash

if [ "$#" -ne 3 ]; then
	echo "Wrong input"
	exit 10	
fi

target=$1
left=$2
right=$3

egrep -vq '^[+-]?[0-9]+$' <(echo "${1}"; echo "${2}"; echo "${3}")

if [ "$?" -eq 0 ]; then
	echo "The arguments are not integers"
	exit 3
fi

if [ "${right}" -lt "${left}" ]; then
	echo "left and right are replaced"
	exit 2
fi

if [ "${target}" -gt "${right}" -o "${target}" -lt "${left}" ]; then
	echo "Not in interval"
    exit 1
fi

if [ "${target}" -le "${right}" -a "${target}" -ge "${left}" ]; then
	echo "fine"
    exit 0
fi















