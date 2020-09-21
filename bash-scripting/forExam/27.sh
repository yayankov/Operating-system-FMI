#!/bin/bash

touch "${2}"

cat ${1} | sort -n | sort -u -t ',' -k2 -k3 -k4 | sort -n > "${2}"

