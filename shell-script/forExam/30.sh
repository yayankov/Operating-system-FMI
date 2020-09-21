#!/bin/bash
		
		
while read TGZ; do
	
	file=$(tar tf ${TGZ} | egrep -w "^meow.txt$")

	NAME=$(echo ${TGZ} | sed "s/_.*//")
	TIMESTAMP=$( echo ${TGZ} | sed "s/${NAME}_report-//" | sed "s/.tgz$//")
	echo ${NAME} ${TIMESTAMP}

    if [ "${file}" = "meow.txt" ]; then
        tar xzvf ${TGZ} meow.txt 
    fi
	mv meow.txt ${NAME}_${TIMESTAMP}.txt
		
done < <(find ${1} -mindepth 1 | egrep -o "^.*\_report\-.*\.tgz" | awk -F '/' '{print $2}')
