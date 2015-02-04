#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include "LISPNonce64.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPNonce64::LISPNonce64() { }
LISPNonce64::~LISPNonce64() { }

Packet* LISPNonce64::simple_action(Packet *p) {
	LISPMapRegister *mr = (LISPMapRegister *)(p->data());
	mr->nonce = 0xFFFFFFFFFFFFFFFF;

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPNonce64)
