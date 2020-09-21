#!/bin/bash

target=$(( (RANDOM%100) + 1 ))
cnt=0
while true; do

	read -p "Guess? " number
    grep -qE '^[+-]?[0-9]+$' <(echo ${number})
	if [ $? -eq 0 ]; then
		cnt=$(( cnt+1 ))	
		if [ ${target} -gt ${number} ]; then
			echo "Bigger"
		elif [ ${target} -lt ${number} ]; then
			echo "Smallest"
		elif [ ${target} -eq ${number} ]; then
			echo "RIGHT! Guessed ${target} in ${cnt} tries!"
			exit 0	
		fi
	else
		echo "Wrong input"
	fi
done


