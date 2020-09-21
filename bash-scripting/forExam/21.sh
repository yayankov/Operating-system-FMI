#!/bin/bash

users=$(mktemp)
ps -eo user= | sort | uniq > "${users}"

while read USER; do
	count=$(ps -eo user= | grep "${USER}" | wc -l)
	total_rss=$(ps -eo user=,rss= | grep "${USER}" | awk '{total+=$2}END{print total}')
	echo "User ${USER} has ${count} processes with total ${total_rss} rss"
done < "${users}"
 
#b
while read USER; do
    average_rss=$(ps -eo user=,rss= | grep "${USER}" | awk '{total+=$2;br++}END{print int(total/br)}')

	rss_process=$(ps -eo rss=,pid= | awk '{$1=$1}1' | sort -n -k1 | head -n 1 | awk '{print $1}')
	pid_process=$(ps -eo rss=,pid= | awk '{$1=$1}1' | sort -n -k1 | head -n 1 | awk '{print $2}')
	if [ ${rss_process} -ge $(( 2 * ${average_rss} )) ]; then
#		echo ${average_rss} ${USER} ${rss_process} ${pid_process}
#		kill -15 ${pid_process}
#		sleep 2
#		kill -9 ${pid_process}
	fi
done < "${users}"

