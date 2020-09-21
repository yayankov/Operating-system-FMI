#!/bin/bash

if [ ${1} = "-n" ]; then
	N=${2}
	shift 2
else
	N=10
fi

temp=$(mktemp)

for FILE; do
	
	while read line; do
		timestamp=$(echo ${line} | awk -F ' ' '{print $1,$2}')
		IDF=$(echo ${FILE} | sed -E "s/.log$//")
		data=$(echo ${line} | cut -d ' ' -f3-)
		echo "${timestamp} ${IDF} ${data}" >> ${temp}
	done < <(cat ${FILE} | tail -n ${N})
done

cat ${temp} | sort -n

rm -- ${temp}
