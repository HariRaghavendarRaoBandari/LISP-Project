#ifndef CLICK_LISPRECORDLOCATOR_HH
#define CLICK_LISPRECORDLOCATOR_HH
#include <click/element.hh>

CLICK_DECLS

/*
=c

LISPRecordLocator()

=s LISPRecordLocator

Generates the record/locator bytes of a MapRegister request.

=d

The record/locator bytes for a MapRegister request correspond to the bytes starting
after the authentication_data bytes to the end of the paquet.

=a
https://tools.ietf.org/html/rfc6830#section-6.1.4
 */

class LISPRecordLocator : public Element {

	public:
		LISPRecordLocator();
		~LISPRecordLocator();

		const char *class_name() const { return "LISPRecordLocator"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return PULL; }
		int configure(Vector<String>&, ErrorHandler*) { return 0; }
		Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS
#endif
