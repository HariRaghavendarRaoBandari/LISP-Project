#!/bin/bash

# Add a list of EIDs using the MAP handler of LISPPrintDB.
# This probably won't work with multiple LISPPrintDB elements.

if [ $# -eq 0 ]; then
	echo "[-] No EID to add..."
	echo -e "usage\t$0 <eid1> [<eid2> ...]\n"
	exit 1
fi

# Host of the control socket
host="localhost"
# Port number of the control socket
port=1111
# The short element's name (e.g. _not_ flatten by click)
#
# Note:
#   In production you want to modify the LISPGenMapRegister element and it's
#   eid_vector handler.
#
short_name="LISPPrintDB"
# The handler name of the element
handler_name="MAP"
# We wait the following number of seconds before adding the next EID
sleep_time=2

##
## Do not modify below
##

# extract full name from the element list
full_name=$(echo "READ list" | nc -q 1 ${host} ${port} | grep ${short_name})

if [ "x${full_name}" == "x" ]; then
	echo "[-] No element named ${short_name} found!"
	exit 1
fi

# list all handlers and see if we have it
handlers_list=$(echo "READ ${full_name}.handlers" | nc -q 1 ${host} ${port})

echo "$handlers_list" | grep -q ${handler_name}

if [ $? -ne 0 ]; then
	echo "[-] No handler named ${handler_name} for ${full_name}"
	exit 1
fi

# The real deal, send the write order to the handler
for ip in $@; do
	echo -e "WRITEDATA ${full_name}.${handler_name} 12\n${ip}\n" | nc -q 1 ${host} ${port} && echo "[+] EID ${ip} added" || echo "[-] EID ${ip} failed!"
	sleep ${sleep_time}
done

echo "[*] finished"
