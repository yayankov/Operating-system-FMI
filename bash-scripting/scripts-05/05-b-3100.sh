#!/bin/bash

read -p "Enter name: " NAME
who | awk '{print $1}' | grep -wF "${NAME}" | wc -l




