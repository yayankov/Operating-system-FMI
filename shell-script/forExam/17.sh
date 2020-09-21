#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Wrong number of arguments"
	exit 1
fi

if [ $(id -u) -eq 0 ]; then
	echo "Not a root"
	exit 2
fi

PR_FOO=$(ps -eo user= |sort | uniq -c | awk '{$1=$1}1'| grep "${1}" | awk '{print $1}')

#a
while read COUNT USER; do

	if [  ${COUNT} -gt ${PR_FOO} ]; then
		echo ${USER}
	fi

done < <(ps -eo user= | sort | uniq -c | awk '{$1=$1}1')

#b
avetime=$(ps -eo etimes= | awk '{total += $1;num++}END{print int(total/num)}')
echo ${avetime}

#c
avetime2=$(( ${avetime} * 2 ))
while read user pid time; do
	if [ ${time} -gt ${avetime2} ];then
		kill -15 ${pid}
		#sleep 2
		#kill -9 ${pid}
	fi
done < <(ps -u ${1} -o user=,pid=,etimes=)
