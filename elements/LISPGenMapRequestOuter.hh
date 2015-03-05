#ifndef CLICK_LISPGENMAPREQUESTOUTER_HH
#define CLICK_LISPGENMAPREQUESROUTER_HH

#include <click/element.hh>
#include <clicknet/udp.h>
#include <clicknet/ether.h>
#include "LISPStructs.hh"

CLICK_DECLS

/*
=c

LISPGenMapRequestOuter()

=s LISPGenMapRequestOuter

Generates the outer common bytes of a MapRequest packet. Adjusts the size of
the data buffer to fit only a LISPMapRequest structure (without the
Map_Reply_Record field).

=d

The outer common bytes for a MapRequest request correspond to the first 4
bytes of the packet.

The "A" bit is fixed to 0.
The "M" bit is fixed to 0.
The "P" bit is fixed to 0.
The "S" bit is fixed to 0.
The "p" bit is fixed to 0.
The "s" bit is fixed to 0.
The "Reserved" field is fixed to 0.
The "IRC" field is fixed to 0.
The "Record Count" field is fixed to 1.

=a
https://tools.ietf.org/html/rfc6830#section-6.1.2
 */
class LISPGenMapRequestOuter : public Element {
	/*
	 * This is the size a Map Request will have (Map_Reply_Record field
	 * exluded).
	 */
	static const uint32_t proper_size = sizeof(LISPMapRequest) - sizeof(uint32_t);

	public:
		LISPGenMapRequestOuter() CLICK_COLD;
		~LISPGenMapRequestOuter() CLICK_COLD;

		const char *class_name() const { return "LISPGenMapRequestOuter"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return PULL; }
		int configure(Vector<String>&, ErrorHandler*) { return 0; }
		Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS
#endif
