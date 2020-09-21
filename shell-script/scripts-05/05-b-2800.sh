#!/bin/bash

#read -p "enter foo: " foo
#echo "${foo}"
#f [[ "$foo" =~ "^[a-z][A-Z][0-9]+" ]]; then
#    echo "The file is normal"
#else
#	echo "unnormal"
#fi
if [[ ! $1 =~ [^a-zA-Z0-9]+ ]]; then
    echo "Your input contains only letters and digits!"
else
    echo "Your input does not contain only letter and digits!"
fi

