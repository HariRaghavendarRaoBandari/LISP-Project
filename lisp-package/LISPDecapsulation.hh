#ifndef CLICK_LISPDECAPSULATION_HH
#define CLICK_LISPDECAPSULATION_HH
#include <click/element.hh>
CLICK_DECLS
/*
 * LISPDecapsulation
 *
 * Retire les entêtes UDP/IP de l'outerheader ainsi que l'entête LISP
 * S'assure que les champs du header LISP sont toujours à 0
 * Copie du Time-to-live et Type-of-service de l'outerheader vers l'innerheader
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
