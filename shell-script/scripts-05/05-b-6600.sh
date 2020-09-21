#!/bin/bash

DIR="${1}"


while read -d $'\n' line; do
	find "${DIR}" -maxdepth 1 -type f -exec md5sum {} \; | awk -v FIRST="${line}" '$1 == FIRST {print $2}' | sort | tail -n +2 | xargs -I{} rm {}
done < <(find "${DIR}" -maxdepth 1 -type f -exec md5sum {} \; | awk '{print $1}' | sort | uniq -d )

