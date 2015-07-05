# installer la configuration pour les interfaces

	#  cp /etc/network/interfaces /etc/network/interfaces.bak
	#  cp interfaces.xtr_client /etc/network/interfaces
	ou
	#  cp interfaces.xtr_server /etc/network/interfaces
	#  /etc/init.d/networking restart

# précisions pour les interfaces réseau

Les interfaces physiques des machines hôtes ne doivent pas avoir les mêmes adresses IP que les RLOC!

## coté client

RLOC: 10.0.0.13

## coté server

RLOC: 10.0.0.14

# démarrer la machine virtuelle xtr

	# ./init.sh xtr.xen xtr.click
	# xl console xtr

Le xTR est prêt!
