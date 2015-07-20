#Content

Generic configuration to start a ClickOS instance with the appropriate xTR configuration.

#Start the ClickOS VM as a xTR

```
# ./init.sh xtr.xen xtr.click
# xl console xtr
```

The xTR is ready!

##What's about 'init.sh' ?

It's not a quick and easy task to feed ClickOS the configuration. You need to write the configuration in the Xen meta file system in order:

```
# xenstore-write /local/domain/<DOMID>/clickos/0/config/0 "<conf>"
# xenstore-write /local/domain/<DOMID>/clickos/0/status "Running"
```

What's delicate is the 1024 bytes limit by "entry". In the example above the entry is '0', it's the number at the far right of the first line. Because our click configuration scripts for the xTR are quite long we made a script that takes care to cut the configuration file and feed it to the appropriate entry then launch the click router.

##Troubleshooting

If a ClickOS instance crashes with a dump, one of your elements have some trouble handling memory. In that case just kill the VM and start over, don't try to restart the router...

You are looking for a way to reach the click handlers ? Us too, it seems that it's feasible using the Xen meta file system but we're still looking...
