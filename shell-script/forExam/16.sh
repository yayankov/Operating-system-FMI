#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Wrong arguments"
	exit 1
fi

FILE=${1}
NIZ1=${2}
NIZ2=${3}

old1=$(mktemp)
old2=$(mktemp)
cat ${FILE} | grep "${NIZ1}" | sed -e 's/.*=//g' | tr ' ' '\n' > ${old1}
cat ${FILE} | grep "${NIZ2}" | sed -e 's/.*=//g' | tr ' ' '\n' > ${old2}

old=$(cat ${FILE} | grep "${NIZ2}" | sed -e 's/.*=//g')
bool=0
new=""

while read CHAR2; do
	while read CHAR1; do
		if [ "${CHAR2}" = "${CHAR1}" ]; then
			bool=$((${bool} + 1))		
		fi 
	done < ${old1}
	if [ ${bool} -eq 0 ]; then
		new+="${CHAR2} "	
	fi	
	bool=0
done < ${old2}

echo ${old}
echo ${new}
echo ${NIZ2}

sed -i -e "s/^${NIZ2}=${old}/${NIZ2}=${new}/" ${FILE}
cat ${FILE}
