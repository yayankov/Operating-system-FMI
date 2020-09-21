#!/bin/bash

temp=$(mktemp)

input=$(</dev/stdin)
echo "${input}" | egrep "^[+-]?[0-9]+$" | sort -n | uniq > "${temp}"

min_num=$(cat ${temp} | head -n 1)
max_sum=0

while read NUM; do
	sum=$(echo ${NUM} | sed "s/-//" | egrep -o '.' | awk '{total += $1}END{print total}')	
	if [ ${max_sum} -lt ${sum} ] || [ ${max_sum} -eq ${sum} -a ${min_num} -gt ${NUM} ]; then
		min_num=$(echo ${NUM})
		max_sum=$(echo ${sum})	
	fi

done < ${temp}

echo ${min_num}

rm -- ${temp}
