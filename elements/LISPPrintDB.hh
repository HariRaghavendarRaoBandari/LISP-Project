#ifndef CLICK_LISPPRINTDB_HH
#define CLICK_LISPPRINTDB_HH

#include <click/element.hh>

CLICK_DECLS

/*
=c

LISPPrintDB()

=s LISPPrintDB

Print the Database when a packet cross this element.

=a LISPDB
 */
class LISPPrintDB : public Element {
	public:
		LISPPrintDB() CLICK_COLD;
		~LISPPrintDB() CLICK_COLD;

		const char *class_name() const { return "LISPPrintDB"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return AGNOSTIC; }
		int configure(Vector<String>&, ErrorHandler *) { return 0; }

		Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS
#endif
