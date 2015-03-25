#ifndef CLICK_LISPRECORDLOCATOR_HH
#define CLICK_LISPRECORDLOCATOR_HH

#include <click/element.hh>
#include <click/args.hh>

CLICK_DECLS

/*
=c

LISPRecordLocator(RLOCIPADDR)

=s LISPRecordLocator

Generates the record/locator bytes of a MapRegister request.

=d

The record/locator bytes for a MapRegister request correspond to the bytes
starting after the authentication_data bytes to the end of the paquet.

"Record TTL" is fixed to 10 minutes.
"Locator Count" is fixed to 1.
"EID mask-len" is fixed to 32.
"ACT" is fixed to 0.
"A" is fixed to true (1).
"Reserved" (all fields) are fixed to 0.
"Map-Version number" is fixed to 0.
"EID-Prefix-AFI" is fixed to AFI_IPV4 (1).
"Priority" and "Weight" (as well as the multicast equivalent) are fixed to 0.
"Unused Flags", "L", "P" and "R" are fixed to 0 ("P" and "R" are only used in
Map-Reply).
"Loc-AFI" is fixed to AFI_IPV4 (1).

RLOCIPADDR must be specified as a valid IPv4 addresse.

=a
https://tools.ietf.org/html/rfc6830#section-6.1.4
 */

class LISPRecordLocator : public Element {
	IPAddress _rloc_ip;

	public:
		LISPRecordLocator();
		~LISPRecordLocator();

		const char *class_name() const { return "LISPRecordLocator"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return PULL; }
		int configure(Vector<String>&, ErrorHandler*);
		Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS
#endif
