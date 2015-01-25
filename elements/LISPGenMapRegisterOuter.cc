#include <click/config.h>
#include "LISPGenMapRegisterOuter.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPGenMapRegisterOuter::LISPGenMapRegisterOuter() { }

LISPGenMapRegisterOuter::~LISPGenMapRegisterOuter() { }

Packet* LISPGenMapRegisterOuter::pull(int) {

	struct LISPMapRegisterOuterHeader data;
	data.Type = LISP_H_TYPE_3;
	data.P = 0;
	data.Reserved_1 = 0;
	data.Reserved_2 = 0;
	data.Reserved_3 = 0;
	data.M = 0;
	data.Record_Count = 1;

	WritablePacket *p = Packet::make(headroom, &data, sizeof(LISPMapRegister), 0);

	if (p == NULL) {
		click_chatter("[-] Packet creation failed!");
		return NULL;
	}

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRegisterOuter)
