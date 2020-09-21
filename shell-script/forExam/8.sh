#!/bin/bash

find . -printf "%p %n\n" | sort -k2 | tail -5 | awk '{print $1}'
