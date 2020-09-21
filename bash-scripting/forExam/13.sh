#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Wrong number of arguments"
	exit 1
fi

if [ ! -d ${1} ]; then
	echo "Argument 1 must be a directory"
	exit 2
fi

DIR=${1}
find ${DIR} -type l -printf "%Y %f\n" | grep '^[NL]' | awk '{print $2}'
