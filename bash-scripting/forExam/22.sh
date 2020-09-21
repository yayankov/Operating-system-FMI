#!/bin/bash

if [ $# -lt 1 -o $# -gt 2 ]; then
	echo "Wrong arugments"
	exit 1
fi

if [ $# -eq 1 ]; then
	CNT=0
	while read Y LINK FILE; do
		if [ ${Y} = "L" -o ${Y} = "N" ]; then
			CNT=$(( ${CNT} + 1 ))
		else
			PATH=$(find ${1} -mindepth 1 -maxdepth 1 -type f | grep ${FILE} )
			echo "${LINK} -> ${PATH}"
		fi
	done < <(find ${1} -mindepth 1 -type l -printf "%Y %f %l\n")
	echo "Broken symlinks: ${CNT}"
fi

touch "${2}"

if [ $# -eq 2 ]; then
    CNT=0
    while read Y LINK FILE; do
        if [ ${Y} = "L" -o ${Y} = "N" ]; then
            CNT=$(( ${CNT} + 1 ))
        else
            PATH=$(find ${1} -mindepth 1 -maxdepth 1 -type f | grep ${FILE} )
            echo "${LINK} -> ${PATH}" >> "${2}"
        fi
    done < <(find ${1} -mindepth 1 -type l -printf "%Y %f %l\n")
    echo "Broken symlinks: ${CNT}" >> "${2}"
fi
