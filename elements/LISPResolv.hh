#ifndef CLICK_LISPRESOLV_HH
#define CLICK_LISPRESOLV_HH

#include <click/element.hh>

CLICK_DECLS

/*
=c

LISPResolv()

=s LISPResolv

Resolv the RLOC associated with the EID in dst_ip_anno.

=d

Resolv the RLOC associated with the EID in dst_ip_anno.
* If a matching RLOC is found, the packet destination is modified to the RLOC
  address, the packet is then forwarded to output 0.
* If no match EID -> RLOC can be found, the packet is forwarded to ouput 1.

If ouput 1 isn't connected to an element, the packet is dropped (p->kill).
The ip checksum isn't computed here, you will need to link SetIPChecksum to do
this.

The dst_ip_anno annotation _must_ be set before calling this element.

LISPDB is used for the EID -> RLOC mapping.

=a LISPDB
*/

class LISPResolv : public Element {
public:
	LISPResolv() CLICK_COLD;
	~LISPResolv() CLICK_COLD;

	const char *class_name() const { return "LISPResolv"; }
	const char *port_count() const { return PORTS_1_1X2; }
	const char *processing() const { return PROCESSING_A_AH; }

	Packet *simple_action(Packet *);
};

CLICK_ENDDECLS
#endif
