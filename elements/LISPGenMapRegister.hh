#ifndef CLICK_LISPGENMAPREGISTER_HH
#define CLICK_LISPGENMAPREGISTER_HH

#include <click/element.hh>
#include <clicknet/udp.h>
#include <clicknet/ether.h>

CLICK_DECLS

/*
=c

LISPGenMapRegisterOuter([EID <ipv4> [, EID <ipv4> ... ] ])

=s LISPGenMapRegister

Generates the outer common bytes of a MapRegister request.
When first started, provided EIDs are set as dst_ip_anno in the outgoing
packet. Then, when a new EID is added using the handler, we repeat the packet
emission for this particular EID.

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
class LISPGenMapRegisterOuter : public Element {
	static const uint32_t headroom = sizeof(click_ip) + sizeof(click_udp) + sizeof(click_ether);
	Vector<int> _ip_vector; // The vector with all EIDs
	static int write_callback(const String &s, Element *e, void *vparam, ErrorHandler *errh);
	static String read_callback(Element *e, void *vparam);

	public:
		LISPGenMapRegister() CLICK_COLD;
		~LISPGenMapRegister() CLICK_COLD;

		const char *class_name() const { return "LISPGenMapRegister"; }
		const char *port_count() const { return "0/1"; }
		const char *processing() const { return PULL; }

		int configure(Vector<String>&, ErrorHandler*);
		Packet *pull(int);
		void add_handlers();
};

CLICK_ENDDECLS
#endif
