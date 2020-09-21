#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Wrong argument"
	exit 1
fi

if [ ! -d ${1} ]; then
	echo "Argument 1 is not a directory"
	exit 2
fi

if [ ! -d ${2} ]; then
    echo "Argument 2 is not a directory"
    exit 3
fi

if [ ! -r ${1} ]; then
    echo "Directory 1 is not readable"
    exit 4
fi

if [ ! -r ${2} ]; then
	echo "Directory 2 is not readable"
	exit 5
fi

pack_version=$(cat "${2}/version")

tar -C "${2}" -c "tree" | xz > "${1}/packages/archive.tar.xz"

new_hash=$(sha256sum "${1}/packages/archive.tar.xz" | awk '{print $1}')

if [ "${pack_version}" = $(cat "${1}/db" | grep -o "${pack_version}") ]; then
	old_hash=$(cat "${1}/db" | grep "${pack_version}" | awk -F ' ' '{print $2}')
	sed -i "s/"${old_hash}"/"${new_hash}"/" "${1}/db"

	find "${1}/packages" -type f | grep "/${old_hash}.tar.xz" | xargs rm
	mv "${1}/packages/archive.tar.xz" "${1}/packages/${new_hash}.tar.xz"
else
	echo "${2}-${pack_version} ${new_hash}" >> "${1}/db" 
	mv "${1}/packages/archive.tar.xz" "${1}/packages/${new_hash}.tar.xz"
fi

temp=$(mktemp)
cat "${1}/db" | sort > ${temp}
cat ${temp} > "${1}/db"
rm -- ${temp}











