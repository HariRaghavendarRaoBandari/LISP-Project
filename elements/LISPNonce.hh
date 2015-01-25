#ifndef CLICK_LISPNONCE_HH
#define CLICK_LISPNONCE_HH

#include <click/element.hh>
CLICK_DECLS

/*
=c

LISPNonce(SIZE=I<integer>)

=s LISPNonce

Add to the end of the packet SIZE bits of Nonce.

=d

Add to the end of the packet SIZE bits of Nonce.
 */
class LISPNonce : public Element {
	public:
		LISPNonce() CLICK_COLD;
		~LISPNonce() CLICK_COLD;

		const char *class_name() const { return "LISPNonce"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return AGNOSTIC; }

		int configure(Vector<String>&, ErrorHandler*);
		Packet *simple_action(Packet *p);
		void push(int, Packet *);
		Packet *pull(int);
};

CLICK_ENDDECLS
#endif
