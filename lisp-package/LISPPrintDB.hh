#ifndef CLICK_LISPPRINTDB_HH
#define CLICK_LISPPRINTDB_HH

#include <click/element.hh>

CLICK_DECLS

/*
=c

LISPPrintDB(RLOC)

=s LISPPrintDB

Print the Database when a packet cross this element.

=d

RLOC is the RLOC IP address, only of use if you want to add mappings in the
database using handlers, see write_callback() for more details.

This is used for testing purposes.
You can add a mapping by providing a EID to the "MAP" handler, only one EID at a time.

=a LISPDB
 */
class LISPPrintDB : public Element {
	static int write_callback(const String &s, Element *e, void *vparam, ErrorHandler *errh);
	IPAddress _rloc;

	public:
		LISPPrintDB() CLICK_COLD;
		~LISPPrintDB() CLICK_COLD;

		const char *class_name() const { return "LISPPrintDB"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return AGNOSTIC; }

		int configure(Vector<String>&, ErrorHandler *);
		Packet *simple_action(Packet *p);
		void add_handlers();
};

CLICK_ENDDECLS
#endif
