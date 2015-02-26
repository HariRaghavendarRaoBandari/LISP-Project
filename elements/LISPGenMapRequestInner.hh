#ifndef CLICK_LISPGENMAPREQUESTINNER_HH
#define CLICK_LISPGENMAPREQUESRINNER_HH

#include <click/element.hh>
#include <clicknet/udp.h>
#include <clicknet/ether.h>
#include <click/args.hh>

CLICK_DECLS

/*
=c

LISPGenMapRequestInner(ITRADDR)

=s LISPGenMapRequestInner

Generates the inner common bytes of a MapRequest packet (after the Nonce).

=d

The inner bytes for a MapRequest request correspond to the bytes after the
Nonce.

The "Source EID AFI" is fixed to 0 (thus "Source EID Address" isn't present).
The other "*-AFI" fields are fixed to AFI_IPV4.
The "Reserved" field is fixed to 0.
The "EID_mask_len" is fixed to 32.

ITDADDR must be specified as a valid IPv4 address. It will be used to fill the
"ITR-RLOC Address" in the packet.

The EID to query is in the packet annotations (offset: dst_ip_anno) and _must_
be a valid IPv4 address.

=a
https://tools.ietf.org/html/rfc6830#section-6.1.2
 */
class LISPGenMapRequestInner : public Element {
	IPAddress _ip;

	public:
		LISPGenMapRequestInner() CLICK_COLD;
		~LISPGenMapRequestInner() CLICK_COLD;

		const char *class_name() const { return "LISPGenMapRequestInner"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return PULL; }

		int configure(Vector<String>&, ErrorHandler*) CLICK_COLD;
		Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS
#endif
