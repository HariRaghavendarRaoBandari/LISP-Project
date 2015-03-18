#ifndef CLICK_LISPGENMAPREQUEST_HH
#define CLICK_LISPGENMAPREQUEST_HH

#include <click/element.hh>
#include <click/args.hh>
#include <click/glue.hh>
#include "LISPStructs.hh"

CLICK_DECLS

/*
=c

LISPGenMapRequest(ITRADDR)

=s LISPGenMapRequest

Generates the outer common bytes of a MapRequest packet. Adjusts the size of
the data buffer to fit only a LISPMapRequest structure (without the
Map_Reply_Record field).

=d

The "A" bit is fixed to 0.
The "M" bit is fixed to 0.
The "P" bit is fixed to 0.
The "S" bit is fixed to 0.
The "p" bit is fixed to 0.
The "s" bit is fixed to 0.
The "Reserved" field is fixed to 0.
The "IRC" field is fixed to 0.
The "Record Count" field is fixed to 1.
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
class LISPGenMapRequest : public Element {
	/*
	 * This is the size a Map Request will have (Map_Reply_Record field
	 * exluded).
	 */
	static const uint32_t proper_size = sizeof(LISPMapRequest) - sizeof(uint32_t);
	IPAddress _ip;

	public:
		LISPGenMapRequest() CLICK_COLD;
		~LISPGenMapRequest() CLICK_COLD;

		const char *class_name() const { return "LISPGenMapRequest"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return PULL; }

		int configure(Vector<String>&, ErrorHandler*) CLICK_COLD;
		Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS
#endif
