#ifndef CLICK_LISPCLASSIFIER_HH
#define CLICK_LISPCLASSIFIER_HH

#include <click/element.hh>
#include <clicknet/udp.h>
#include <clicknet/ether.h>
#include <click/args.hh>

#define LISP_CL_REGISTER 0
#define LISP_CL_REQUEST	 1
#define LISP_CL_INVALID	 2

CLICK_DECLS

/**

=c

LISPClassifier()

=s LISPClassifier

Classifies lisp (control plan) packet by contents.

=d

output 0 -> LISP Map Register.
output 1 -> LISP Map Request.
output 2 -> Invalid LISP type.

**/
class LISPClassifier: public Element {
public:
	LISPClassifier() CLICK_COLD;
	~LISPClassifier() CLICK_COLD;

	const char *class_name() const { return "LISPClassifier"; }
	const char *port_count() const { return "1/3"; }
	const char *processing() const { return PUSH; }

	void push(int, Packet*);
};

CLICK_ENDDECLS
#endif
