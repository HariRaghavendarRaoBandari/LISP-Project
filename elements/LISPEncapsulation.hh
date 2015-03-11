#ifndef CLICK_LISPENCAPSULATION_HH
#define CLICK_LISPENCAPSULATION_HH

#include <click/element.hh>

CLICK_DECLS

class LISPEncapsulation : public Element {

	public:
		LISPEncapsulation() CLICK_COLD;
		~LISPEncapsulation() CLICK_COLD;

		const char *class_name() const { return "LISPEncapsulation"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return AGNOSTIC; }
		int configure(Vector<String>&, ErrorHandler*) { return 0; }

		Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS
#endif
