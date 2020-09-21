#!/bin/bash










find ${1} -maxdepth 1 -type f -printf "%f (%s bytes)\n"

while read -d $'\n' line; do	

	num=$(find $line | cut -d/ -f2 |sort | uniq -c |sort -r | awk '{print $1}')	    
			
	echo "$(basename ${line}) $num"

done < <( find ${1} -mindepth 1 -maxdepth 1 -type d | sort)

