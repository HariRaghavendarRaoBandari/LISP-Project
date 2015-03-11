#ifndef CLICK_LISPENCAPSULATION_HH
#define CLICK_LISPENCAPSULATION_HH

#include <click/element.hh>
#include <clicknet/udp.h>

CLICK_DECLS
/*
 * LISPEncapsulation
 * 
 * Ajoute l'entête LISP et met tout les bits à 0
 * (Bit de statut, flags, Nonce etc...)
 *  
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
