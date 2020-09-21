#!/bin/bash

read -p 'Input a string: ' string

for i; do

	if [ ! -f "${i}" ]; then
		echo "Argument [${i}] is not a file"
	elif [ ! -r "${i}" ]; then
        echo "Argument [${i}] is not readable"
	else
	    lines=$(cat "${i}" | grep -E "${string}" | wc -l)
	    echo "File [${i}] has the string: ${string} in  ${lines} lines" 
	fi
 
done

