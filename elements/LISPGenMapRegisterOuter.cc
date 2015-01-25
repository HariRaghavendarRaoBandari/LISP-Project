#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include "LISPGenMapRegisterOuter.hh"

CLICK_DECLS
LISPGenMapRegisterOuter::LISPGenMapRegisterOuter() { }

LISPGenMapRegisterOuter::~LISPGenMapRegisterOuter() { }

void LISPGenMapRegisterOuter::push(int, Packet *p) {
	const unsigned char* start = p->data();
	const unsigned char* end = p->end_data();
	click_chatter("TODO");
}

Packet* LISPGenMapRegisterOuter::pull(int) {
	return input(0).pull();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRegisterOuter)
