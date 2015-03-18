#ifndef CLICK_LISPGENMAPREGISTER_HH
#define CLICK_LISPGENMAPREGISTER_HH

#include <click/element.hh>
#include <clicknet/udp.h>
#include <clicknet/ether.h>

CLICK_DECLS

/*
=c

LISPGenMapRegister()

=s LISPGenMapRegister

Generate a Map-Register without the Locator part.

=d

The "P" bit is fixed to 0.
The "Reserved" field is fixed to 0.
The "M" bit is fixed to 0.
The "record count" field is fixed to 1.
The "key ID" field is fixed to 0 for now.
The "Authentification Data Length" field is fixed to 0 as of now.
The "Authentification Data" field is non-exiting as of now.

=a
https://tools.ietf.org/html/rfc6830#page-38
 */
class LISPGenMapRegister : public Element {
	static const uint32_t headroom = sizeof(click_ip) + sizeof(click_udp) +
		sizeof(click_ether);

	public:
		LISPGenMapRegister() CLICK_COLD;
		~LISPGenMapRegister() CLICK_COLD;

		const char *class_name() const { return "LISPGenMapRegister"; }
		const char *port_count() const { return "0/1"; }
		const char *processing() const { return PULL; }
		int configure(Vector<String>&, ErrorHandler*) { return 0; }

		Packet *pull(int);
};

CLICK_ENDDECLS
#endif
