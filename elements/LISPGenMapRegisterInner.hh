#ifndef CLICK_LISPGENMAPREGISTERINNER_HH
#define CLICK_LISPGENMAPREGISTERINNER_HH

#include <click/element.hh>
#include <clicknet/udp.h>
#include <clicknet/ether.h>

CLICK_DECLS

/*
=c

LISPGenMapRegisterInner()

=s LISPGenMapRegisterInner

Generates the inner common bytes of a MapRegister request.

=d

The inner common bytes for a MapRegister request correspond to the bytes starting
from the 13th byte to the bytes preceding the "Record-bytes".

The "key ID" field is fixed to 0 for now.
The "Authentification Data Length" field is fixed to 0 as of now.
The "Authentification Data" field is non-exiting as of now.

=a
https://tools.ietf.org/html/rfc6830#page-38
 */

class LISPGenMapRegisterInner : public Element {
	static const uint32_t headroom = sizeof(click_ip) + sizeof(click_udp) + sizeof(click_ether) + 3*4; //(3 * 4 bytes for OuterHeader)

	public:
		LISPGenMapRegisterInner() CLICK_COLD;
		~LISPGenMapRegisterInner() CLICK_COLD;

		const char *class_name() const { return "LISPGenMapRegisterInner"; }
		const char *port_count() const { return "0/1"; }
		const char *processing() const { return PULL; }
		int configure(Vector<String>&, ErrorHandler*) { return 0; }

		Packet *pull(int);
};

CLICK_ENDDECLS
#endif
