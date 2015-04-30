#ifndef CLICK_LISPENCAPSULATION_HH
#define CLICK_LISPENCAPSULATION_HH

#include <click/element.hh>
#include <clicknet/udp.h>

CLICK_DECLS

/*
=c

LISPEncapsulation(SRC <ip>, SPORT <int>)

=s LISPEncapsulation

Add the outer LISP and UDP/IP headers (in this order).
Set every bit in the LISP header to 0.
Encap to port 4341 of the IP in the dst_ip_anno() of the input packet.
Copy the Time-to-live and Type-of-service IP fields from the inner header to the outer header.
Checksums have to be computed for the output packet.

=d

SRC is the RLOC for the xTR doing the encapsulation (for the outer IP header).
SPORT is the source port (for the outer UDP header).

=a SetIPChecksum, SetUDPChecksum
*/
class LISPEncapsulation : public Element {

	public:
		LISPEncapsulation() CLICK_COLD;
		~LISPEncapsulation() CLICK_COLD;

		const char *class_name() const { return "LISPEncapsulation"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return AGNOSTIC; }

		int configure(Vector<String>&, ErrorHandler*) CLICK_COLD;

		Packet *simple_action(Packet *p);

	private:

    struct in_addr _saddr;
    uint16_t _sport;
};

CLICK_ENDDECLS
#endif
