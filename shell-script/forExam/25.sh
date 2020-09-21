#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Wrong arguments"
	exit 1
fi
if [ ! -d "${1}" ]; then
	echo "Argument 1 is not a directory"
	exit 2
fi

friends_msg=$(mktemp)

friends=$(mktemp)
find ${1} -mindepth 3 -maxdepth 3 -type d -printf "%f\n" | sort | uniq > ${friends}

while read FRIEND; do
	total_msg=0
	while read FILE; do
		msg_file=$(cat ${FILE} | wc -l)
		total_msg=$(( ${total_msg} + ${msg_file} ))
	done < <(find ${1} -type f | grep -w "${FRIEND}")
	
	echo "You have ${total_msg} messages with ${FRIEND}" >> ${friends_msg}

done < ${friends}

echo "All friends: $(cat ${friends} | tr '\n' ', ')"

cat ${friends_msg} | sort -n -t ' ' -k3 | tail -n 5

rm -- ${friends_msg}
rm -- ${friends}
