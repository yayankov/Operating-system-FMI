#!/bin/bash

if [ ! -f ${1} ]; then
	echo "Wrong arguments"
	exit 1
fi

if [ ! -r ${1} ]; then
	echo "The file is not readable"
	exit 2
fi

FILE=${1}
sites=$(mktemp)
temp=$(mktemp)

cat ${FILE} | awk -F ' ' '{print $2}' | sort | uniq -c | awk '{$1=$1}1' | sort -rn -k1 -k2 | head -n 3 | awk -F ' ' '{print $2}' > ${sites}

while read site; do
	cat ${FILE} | grep -w "${site}" > ${temp}
	cnt_HTTP2=$(cat ${temp} | grep -w "HTTP/2.0" | wc -l)
	cnt_HTTP=$(cat ${temp} | grep -v -w "HTTP/2.0" | wc -l)
	echo "${site} HTTP/2.0: ${cnt_HTTP2}  non-HTTP/2.0: ${cnt_HTTP}"
done < ${sites}

clients=$(mktemp)

while read line; do
	status=$(echo ${line} | awk -F ' ' '{print $9}')
	if [ ${status} -gt 302 ]; then
		echo ${line} >> ${clients}
	fi
done < ${FILE}

cat ${clients} | sort -n -k1 | uniq -c | awk '{$1=$1}1' | sort -rn -k1 -k2 | awk -F ' ' '{print $1,$2}'

rm -- ${sites}
rm -- ${temp}
rm -- ${clients}
