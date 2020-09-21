#!/bin/bash

if [ $(id -u) -eq 0 ] || exit 0

while read USER HOMEDIR; do
	if [ ! -d "${HOMEDIR}" ] || sudo -u ${USER} [ ! -w ${HOMEDIR} ]; then
		echo "${USER}"
	fi
done < <(cat /etc/passwd | awk -F: '{print $1,$6}')

