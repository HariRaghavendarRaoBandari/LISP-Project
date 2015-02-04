#ifndef CLICK_LISPNONCE_HH
#define CLICK_LISPNONCE_HH

#include <click/element.hh>
CLICK_DECLS

/*
=c

LISPNonce()

=s LISPNonce

Add to the end of the packet SIZE bits of Nonce.

=d

Add to the end of the packet SIZE bits of Nonce.
 */
class LISPNonce64 : public Element {
	public:
		LISPNonce64() CLICK_COLD;
		~LISPNonce64() CLICK_COLD;

		const char *class_name() const { return "LISPNonce64"; }
		const char *port_count() const { return PORTS_1_1; }
		const char *processing() const { return AGNOSTIC; }
		int configure(Vector<String>&, ErrorHandler*) { return 0; }
		Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS
#endif
