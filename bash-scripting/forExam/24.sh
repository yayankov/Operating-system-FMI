#!/bin/bash

users=$(mktemp)

while read USER USER_UID HOMEDIR; do
	if [ ! -d ${HOMEDIR} ]; then
		continue;
	elif [ "$(stat -c "%u" "${HOMEDIR}")" != "${USER_UID}" ]; then
		continue;
	elif [ "$(stat -c "%A" "${HOMEDIR}" | cut -c3)" != "w" ]; then
		continue;
	else
		echo ${USER} >> ${users}
	fi
done < <(cat /etc/passwd | grep -wv "root" | awk -F: '{print $1,$3,$6}')

rss_root=$(ps -eo user=,rss= | grep -w "root" | awk '{total+=$2}END{print total}')

while read USER; do
	rss_user=$(ps -eo user=,rss= | grep "${USER}" | awk '{total+=$2}END{print int(total)}')
	if [ ${rss_user} -gt ${rss_root} ]; then
		#kilvame sega no s komentar
		while read PID; do
			#kill -15 ${PID}
			#sleep 2
			#kill -9 ${PID}
		done < <(ps -eo user=,pid= | grep "${USER}" | awk '{print $2}')
	fi
done < ${users}
