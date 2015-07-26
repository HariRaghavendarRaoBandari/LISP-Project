#The project

This is our project for the Master 1. The goal of this was to explore [NFV](https://en.wikipedia.org/wiki/Network_functions_virtualization) by implementing the [LISP Protocol](https://en.wikipedia.org/wiki/Locator/Identifier_Separation_Protocol) using [Click Modular Router](https://github.com/kohler/click) and Xen/[ClickOS](http://cnp.neclab.eu/clickos/).

> This is a PoC so I wouldn't recommend that you use it in production as is.

##State

We did not implement the whole LISP protocol, only the main "features":

- Map Request
- Map Reply
- Map Register
- Encapsulation / Decapsulation

##What's about the language ?

We are french students so we made the commit messages in french but the documentation of Click elements is in english.
If a file is in plain french without any english equivalent, we are working on translating it.

##Usage
Click __must__ be installed in userlevel mode.
See xen/README for Xen/ClickOS usage.

To use the lisp package you have to:

Compile it:
```
$ autoreconf
# make install
```
Add the following line to your click configuration files:
```
require(lisp);
```

##Usefull links

- [LISP Protocol on wikipedia](https://en.wikipedia.org/wiki/Locator/Identifier_Separation_Protocol)
- [LISP RFC](https://tools.ietf.org/html/rfc6830)
- [Cisco LISP](http://lisp.cisco.com/lisp_over.html)
- [Click Modular Router](https://github.com/kohler/click)
- [ClickOS](http://cnp.neclab.eu/clickos/)
