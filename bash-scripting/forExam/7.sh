#!/bin/bash


cat ${1} | grep "Inf" | grep -E '.*:.* .*a[,:].*' | cut -c 3,4 | uniq -c | head -1
