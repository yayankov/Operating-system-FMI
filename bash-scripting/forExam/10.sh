#!/bin/bash

cat ${1} | grep 'SI' | sort -n | cut -d ':' -f5,6 | sed -rE 's/,.*:/:/g'
