#!/bin/bash

for i; do
	if [ -r "${i}" -a -f "${i}" ]; then
		echo "The file ["${i}"] is readable"
	elif [ -f "${i}" ]; then
		echo "The file ["${i}"] is not readable"
	elif [ -d "${i}" ]; then
		num_files=$(find "${i}" -mindepth 1 -maxdepth 1 -type f | wc -l)
		echo "Num of lines [${num_files}] in [${i}]"
		while read file size; do
			if [ ${size} -lt ${num_files} ]; then
				echo "File [${file}] with size [${size}] from directory [${i}]"
			fi
		done < <(find "${i}" -mindepth 1 -maxdepth 1 -type f -printf "%f %s\n")
	fi
done

