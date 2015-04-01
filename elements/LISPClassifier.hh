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

class LISPClassifier: public Element {
public:
	LISPClassifier() CLICK_COLD;
	~LISPClassifier() CLICK_COLD;

	const char *class_name() const { return "LISPClassifier"; }
	const char *port_count() const { return "1/3"; }
	const char *processing() const { return AGNOSTIC; }

	Packet* simple_action(int, Packet*);
};

CLICK_ENDDECLS
#endif
