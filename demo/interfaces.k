# This file describes the network interfaces available on your system
# and how to activate them. For more information, see interfaces(5).

# The loopback network interface
auto lo
iface lo inet loopback

iface eth0 inet manual

auto xenbr0
iface xenbr0 inet static
	bridge_ports eth0
	address 10.0.0.4
	broadcast 10.0.0.255
	netmask 255.255.255.0

auto xenbr1
iface xenbr1 inet static
	address 192.168.1.1
	broadcast 192.168.1.255
	netmask 255.255.255.0
#	pre-up brctl addbr $IFACE
#	up ip link set $IFACE up
#	post-down brctl delbr $IFACE
#0.	down ip link set $IFACE down

