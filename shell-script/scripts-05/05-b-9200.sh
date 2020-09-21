#!/bin/bash

export RMLOG=&(mktemp)

for i; do
	grep -qE '^-r$' <(echo "${i}")	
	if [ $? -eq 0 ]; then
		
		if [ -d ${i} ]; then
			elements=$(find ${i} -mindepth 1 -maxdepth 1 | wc -l | awk '{$1=$1}1' )
			if [ ${elements} -eq 0 ]; then
				rmdir ${i}
				echo "[date %H:%m:%M] Removed directory ${i}"
			else
				rm -r ${i}
				echo "[date %H:%m:%M] Removed directory ${i}"
			fi
		elif [ -f ${i} ]; then
			echo "[date %H:%m:%M] Removed directory ${i}"
			rm ${i}
		fi
	else
		
	fi
done

