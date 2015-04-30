#ifndef CLICK_LISPDECAPSULATION_HH
#define CLICK_LISPDECAPSULATION_HH
#include <click/element.hh>
CLICK_DECLS

/*
=c

LISPDecapsulation()

=s LISPDecapsulation

Decapsulate a LISP Data packet, the Time-to-live and Type-of-service fields of
the IP outer header are copied in the inner IP header. The output packet will
be stripped to his inner IP header.
*/
class LISPDecapsulation : public Element {

	public:
		 LISPDecapsulation();
		~LISPDecapsulation();
		const char *class_name() const { return "LISPDecapsulation"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return AGNOSTIC; }
		int configure(Vector<String>&, ErrorHandler*) { return 0; }

		Packet *simple_action(Packet *p_in);
};

CLICK_ENDDECLS
#endif
