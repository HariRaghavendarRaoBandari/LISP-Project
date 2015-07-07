#Contenu

Tous les fichiers de configuration dont nous avons eu besoin pour effectuer la démo.

```
.
├── interfaces.k		# /etc/network/interfaces pour l'hôte Xen de droite
├── interfaces.t
├── interfaces.vm.k		# /etc/network/interfaces pour la vm Debian de droite
├── interfaces.vm.t
├── vm.k.xen			# configuration Xen pour la vm Debian de droite
├── vm.t.xen
├── xtr.k.click			# configuration du xTR pour ClickOS sur la machine de droite
├── xtr.k.xen
├── xtr.t.click
└── xtr.t.xen
```

##Précisions sur les noms des fichiers

Les fichiers comportant 't' et 'k' correspondent respectivement aux machines hôtes Xen de gauche et de droite (voir schéma ci dessous).

#Explication de la démo

La démo est composée de 3 machines dont deux hôtes Xen.

Une machine fait office de MS/MR.

Chaque machine hôte Xen fait tourner :

- une instance de ClickOS avec une configuration xTR
- une instance d'une image debian basique

Le réseau complet est le suivant:

![Demo: Full network topology](./topology_demo.png)

Gros plan sur l'agencement des différentes interfaces réseau sur un hôte Xen:

![Demo: Xen network](./xen_network_demo.png)

Le but de notre démo était de démontrer (1) que nous avions implémenté le protocole LISP avec succès, (2) que cette implémentation fonctionnait dans un réseau entièrement virtualisé.

Pour montrer le fonctionnement correct du protocole nous avons suivit dans wireshark le trajet d'une requête ping. Ce paquet émanait de la machine virtuelle Debian positionnée derrière le routeur de gauche et arrivait à destination sur la machine virtuelle Debian positionnée derrière le routeur de droite.

Pour tester la démo, lancer les vm Debian puis consultez le README du dossier ../xen pour les commandes à exécuter pour démarrer les xTR.

##Environement

Pour cette démo nous avons utilisé des machines Debian "Jessy" avec Xen 4.1. Le MS/MR fonctionnait lui sur un Archlinux.
