Click __doit__ être installé sur le système (mode userlevel activé)!
Voir le ficher commandes_xen.md pour l'utilisation de Xen/ClickOS.

Pour utiliser le package lisp il est nécessaire d'exécuter dans lisp-package:

Compilation:

	$ autoreconf
	# make install

Ajouter la ligne aux fichiers de configuration .click pour une utilisation en mode userlevel:

	require(lisp);
