#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Wrong arguments"
	exit 1
fi

if [ $(id -u) -ne 0 ]; then
	echo "The user is not root"
	exit 2
fi

grep -qE '^[+-]?[0-9]+$' <(echo "${1}")

if [ "$?" -ne 0 ]; then
	echo "Argument 1 is not a number"
	exit 3
fi
 
USERS=$(mktemp)
ps -eo user= | sort | uniq > "${USERS}"

while read USER; do
	total_rss=0
	while read PID RSS; do
		total_rss=$((${total_rss}+${RSS}))
		last_pid=${PID}
	done < <(ps -u ${USER} -o pid=,rss= | sort -n -k2)

	echo "Total ${total_rss} rss for user ${USER}"
	
	if [ ${total_rss} -gt ${1} ]; then
		kill -15 ${last_pid}
		sleep 2
		kill -9 ${last_pid}
	fi
	
done < "${USERS}"
rm -- ${USERS}
