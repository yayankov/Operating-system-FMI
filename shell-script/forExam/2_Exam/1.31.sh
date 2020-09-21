#!/bin/bash

if [ $# -ne 2 ]; then
	echo "wrong arguments"
	exit 1
fi

touch "bar.csv"

if [ ! -d ${2} ]; then
	echo "Argument 2 is not a dir"
	exit 2
fi

if [ ! -r ${2} ]; then
	echo "Directory is not readable"
	exit 3
fi

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" > "bar.csv"

while read FILE; do 
	hostname=$(echo ${FILE} | awk -F '/' '{print $NF}' | sed -E "s/\.log$//")
	phy=$(cat ${FILE} | egrep -w "Physical Interfaces" | awk -F ':' '{print $2}' | awk '{$1=$1}1')
	vlans=$(cat ${FILE} | egrep -w "VLANs" | awk -F ':' '{print $2}' | awk '{$1=$1}1')
	hosts=$(cat ${FILE} | egrep -w "Inside Hosts" | awk -F ':' '{print $2}' | awk '{$1=$1}1')
	failover=$(cat ${FILE} | egrep -w "Failover" | awk -F ':' '{print $2}' | awk '{$1=$1}1')
	VPN=$(cat ${FILE} | egrep -w "VPN-3DES-AES" | awk -F ':' '{print $2}' | awk '{$1=$1}1')
	peers=$(cat ${FILE} | egrep -w "VPN Peers" | awk -F ':' '{print $2}' | awk '{$1=$1}1')
	VLAN=$(cat ${FILE} | egrep -w "VLAN Trunk Ports" | awk -F ':' '{print $2}' | awk '{$1=$1}1')
	license=$(cat ${FILE} | egrep "This platform has" | cut -d ' ' -f5- | sed -E "s/license\.//" | awk '{$1=$1}1')
	SN=$(cat ${FILE} | egrep -w "Serial Number" | awk -F ':' '{print $2}' | awk '{$1=$1}1')
	key=$(cat ${FILE} | egrep -w "Activation Key" | awk -F ':' '{print $2}' | awk '{$1=$1}1')

	echo "${hostname},${phy},${vlans},${hosts},${failover},${VPN},${peers},${VLAN},${license},${SN},${key}" >> "bar.csv"

done < <(find ${2} -mindepth 1 -type f | egrep "\.log$")








