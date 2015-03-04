#include <click/config.h>
#include "LISPGenMapReplyOuter.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPGenMapReplyOuter::LISPGenMapReplyOuter() { }

LISPGenMapReplyOuter::~LISPGenMapReplyOuter() { }

Packet* LISPGenMapReplyOuter::pull(int) {

	struct LISPMapReplyOuterHeader data;
	data.Type = LISP_H_TYPE_2;
	data.P = 0;
	data.E = 0;
	data.S = 0;
	data.Reserved_1 = 0;
	data.Reserved_2 = 0;
	data.Reserved_3 = 0;
	data.Record_Count = 1;

	WritablePacket *p = Packet::make(headroom, &data, sizeof(LISPMapReply), 0);

	if (p == NULL) {
		click_chatter("[-] Packet creation failed!");
		return NULL;
	}

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapReplyOuter)
