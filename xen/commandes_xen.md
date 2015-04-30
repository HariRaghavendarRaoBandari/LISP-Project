# installer la configuration pour les interfaces

	# cp /etc/network/interfaces /etc/network/interfaces.bak
	# cp interfaces.xtr_client /etc/network/interfaces
	ou
	# cp interfaces.xtr_server /etc/network/interfaces
	# /etc/init.d/networking restart

# précisions pour les interfaces réseau

Les interfaces physiques des machines h^otes ne doivent pas avoir les m^emes adresses IP que les RLOC!

## coté client

RLOC: 10.0.0.13

## coté server

RLOC: 10.0.0.14

# démarrer la machine virtuelle xtr

	# xl create xtr_client.xen
	ou
	# xl create xtr_server.xen

Il es possible d'optenir une console sur la machine virtuelle en tapant:

	# xl console xtr

Charger la configuration:

	# ./init.sh xtr_client.click
	ou
	# ./init.sh xtr_server.click

Le xTR est pr^et!
