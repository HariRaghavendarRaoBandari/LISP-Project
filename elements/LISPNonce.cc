#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include "LISPNonce.hh"

CLICK_DECLS
LISPNonce::LISPNonce() { }

LISPNonce::~LISPNonce() { }

void LISPNonce::push(int, Packet *p) {
	click_chatter("TODO");
}

Packet* LISPNonce::pull(int) {
	return input(0).pull();
}

Packet* LISPNonce::simple_action(Packet *p) {
	// On ajoute le nonce ici
	click_chatter("TODO");
}

int configure(Vector<String> &str, ErrorHandler *eh) {
	// TODO: Gérer SIZE
	return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPNonce)
