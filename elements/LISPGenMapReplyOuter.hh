#ifndef CLICK_LISPGENMAPREPLYOUTER_HH
#define CLICK_LISPGENMAPREPLYOUTER_HH

#include <click/element.hh>
#include <clicknet/udp.h>
#include <clicknet/ether.h>

CLICK_DECLS

/*
=c

LISPGenMapReplyOuter()

=s LISPGenMapReplyOuter

Generates the outer common bytes of a MapReply response.

=d

The outer common bytes for a MapReply response correspond to the first 4
bytes of the packet.

The "P" bit is fixed to 0.
The "E" bit is fixed to 0.
The "S" bit is fixed to 0.
The "Reserved" field is fixed to 0.
The "record count" field is fixed to 1.

=a
https://tools.ietf.org/html/rfc6830#page-31
 */
class LISPGenMapReplyOuter : public Element {
	static const uint32_t headroom = sizeof(click_ip) + sizeof(click_udp) +
		sizeof(click_ether);

	public:
		LISPGenMapReplyOuter() CLICK_COLD;
		~LISPGenMapReplyOuter() CLICK_COLD;

		const char *class_name() const { return "LISPGenMapReplyOuter"; }
		const char *port_count() const { return "0/1"; }
		const char *processing() const { return PULL; }
		int configure(Vector<String>&, ErrorHandler*) { return 0; }

		Packet *pull(int);
};

CLICK_ENDDECLS
#endif
