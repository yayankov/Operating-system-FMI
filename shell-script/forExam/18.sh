#!/bin/bash

temp=$(mktemp)

while IFS=":" read -r USER HOMEDIR; do
	if [ ! -d ${HOMEDIR} -o ! -r ${HOMEDIR} ]; then
		continue
	fi 

	while read FILE TIME; do
		echo "${USER} ${FILE} ${TIME}" >> ${temp}
	done < <(find ${HOMEDIR} -type f -printf "%f %T@\n")
done < <(cat /etc/passwd | cut -d ':' -f1,6)
#cat ${temp} | sort -n -k3
cat ${temp} | sort -n -k3 | tail -n 1 | awk '{print $1,$2}'
