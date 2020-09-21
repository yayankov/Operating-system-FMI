#!/bin/bash

inode=$(find ~ -type f -printf "%T@ %i %p\n" | sort -n | tail -n 1 | awk '{print $2}')
find ~ -inum ${inode} | egrep -o '.' | fgrep '/' | wc -l
