#include <click/config.h>
#include "LISPGenMapRequestOuter.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPGenMapRequestOuter::LISPGenMapRequestOuter() { }

LISPGenMapRequestOuter::~LISPGenMapRequestOuter() { }

Packet* LISPGenMapRequestOuter::simple_action(Packet *p) {

	// Adjust the length of the packet data buffer
	if (p->length() < proper_size) // "increase"
		p = p->put(proper_size - p->length());
	else if (p->length() > proper_size) // "shrink"
		p->take(p->length() - proper_size);

	LISPMapRequest *mr = (LISPMapRequest *) p->data();

	mr->header.Type = LISP_H_TYPE_1;
	mr->header.A = 0;
	mr->header.M = 0;
	mr->header.P = 0;
	mr->header.S = 0;
	mr->header.p = 0;
	mr->header.s = 0;
	mr->header.Reserved_1 = 0;
	mr->header.Reserved_2 = 0;
	mr->header.IRC = 0;
	mr->header.Record_Count = 1;

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRequestOuter)
