#!/bin/bash

while read FILE SIZE; do
	if [ ${SIZE} -eq 0 ]; then
		rm ${FILE}
	fi
done < <(find home1 -type f -printf "%p %s\n")

#while read FILE SIZE USER; do
#    rm ${FILE}
#done < <(find DIR1 -type f -printf "%p %s %u\n" | grep "^$(whoami)$" | sort -k2 | tail -5)
find DIR1 -type f -printf "%p %s %u\n" | grep "^$(whoami)$" | sort -k2 | tail -5 | xargs rm
