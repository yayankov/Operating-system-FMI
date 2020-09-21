#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Wrong arguments"
	exit 1
fi

find ${1} -mindepth 1 -maxdepth 1 -type f -printf "%f\n" | grep -wE "vmlinuz\-[0-9]+\.[0-9]+\.[0-9]+\-${2}" | sort -n -t '-' -k2 | tail -n 1
