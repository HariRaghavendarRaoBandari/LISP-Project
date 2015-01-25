#ifndef CLICK_LISPGENMAPREGISTEROUTER_HH
#define CLICK_LISPGENMAPREGISTEROUTER_HH

#include <click/element.hh>
CLICK_DECLS

/*
=c

LISPGenMapRegisterOuter()

=s LISPGenMapRegisterOuter

Generates the outer common bytes of a MapRegister request.

=d

The outer common bytes for a MapRegister request correspond to the first 4 bytes
of the packet.

The "record count" filed is fixed to 1.
The "M" field/bit is fixed to 0.

=a
https://tools.ietf.org/html/rfc6830#page-38
 */
class LISPGenMapRegisterOuter : public Element {
	public:
		LISPGenMapRegisterOuter() CLICK_COLD;
		~LISPGenMapRegisterOuter() CLICK_COLD;

		const char *class_name() const { return "LISPGenMapRegisterOuter"; }
		const char *port_count() const { return "0/1"; }
		const char *processing() const { return AGNOSTIC; }
		int configure(Vector<String>&, ErrorHandler*) const { return 0; }

		void push(int, Packet *);
		Packet *pull(int);
};

CLICK_ENDDECLS
#endif
