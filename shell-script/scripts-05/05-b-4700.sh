#!/bin/bash

if [ "$#" -gt 2 -o "$#" -lt 1 ]; then
	echo "Wrong input"
	exit 1
fi

egrep -vq '^[+-]?[0-9]+$' <(echo "${1}")
if [ "$?" -eq 0 ]; then
	echo "Argument 1 is not a number"
	exit 2
fi

if [ "$#" -eq 1 ]; then
    DELIMETER=" "  
	echo "Delimeter is space"
fi

if [ "$#" -eq 2 ]; then
    DELIMETER="${2}"
	echo "Delimiter is ["${DELIMETER}"]"
fi

NUM_REV=$(echo "${1}" | rev)
echo "${NUM_REV}"
echo $(echo "$NUM_REV" | sed "s/.\{3\}/&${DELIMETER}/g" | rev)

