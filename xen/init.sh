#!/bin/bash -x

if test $# -lt 2
then

	echo "Usage: $0 <xen cfg file> <click cfg file>"
	exit -1

fi

XEN_CONF=$1
if ! test -f $XEN_CONF
then
	echo "$1 not a file"
	exit -2
fi

CLICK_CONF=$2
if ! test -f $2
then
	echo "$2 not a file"
	exit -3
fi

DOMID=-1
VM_NAME=`cat $XEN_CONF | grep "^name" | cut -d "'" -f 2` 

xl create "$XEN_CONF"

DOMID=`xl list | grep "$VM_NAME" | awk -F' ' '{ print $2 }'`

split -l 10 "$CLICK_CONF" "$CLICK_CONF."

i=0
for f in `ls "$CLICK_CONF".*`
do
	xenstore-write "/local/domain/$DOMID/clickos/0/config/$i" "`cat $f`"
	i=$((i+1))

done

xenstore-write "/local/domain/$DOMID/clickos/0/status" "Running"
