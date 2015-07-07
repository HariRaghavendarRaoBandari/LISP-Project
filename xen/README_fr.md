#Contenu

Configuration générique pour lancer une instance de ClickOS avec la configuration xTR adéquate.

#Démarrer une machine virtuelle ClickOS en tant qu'xTR

```
# ./init.sh xtr.xen xtr.click
# xl console xtr
```

Le xTR est prêt!

##Pourquoi un script 'init.sh' ?

Donner de la configuration click à ClickOS est assez "sportif". Il faut écrire la configuration dans un méta système de fichiers Xen puis donner l'ordre de démarrage de click:

```
# xenstore-write /local/domain/<DOMID>/clickos/0/config/0 "<conf>"
# xenstore-write /local/domain/<DOMID>/clickos/0/status "Running"
```

L'élément délicat est que ce système impose une limite de 1024 octets par "entrée". Dans l'exemple ci-dessus l'entrée est '0', c'est le chiffre le plus à droite de la première ligne. Comme nos scripts de configuration click pour les xTR dépassent allègrement cette limite nous avons réalisé un script qui écrit des morceaux de configuration click automatiquement à l'entrée adéquate puis lance le router click.

##Troubleshooting

Si une instance de ClickOS crashe avec un dump, un de vos éléments à des soucis de gestion/référencement mémoire. Dans ce cas il est obligatoire de "killer" la vm et de la redémarrer, n'essayez pas d'arrêter le router et de le relancer.

Vous cherchez comment atteindre les handlers ? Nous aussi, il semblerait que ce soit possible par le méta système de fichier mais nous cherchons toujours...
