#include <click/config.h>
#include "LISPGenMapRegister.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPGenMapRegister::LISPGenMapRegister() { }

LISPGenMapRegister::~LISPGenMapRegister() { }

Packet* LISPGenMapRegister::pull(int) {

	struct LISPMapRegister mr;
	mr.Type = LISP_H_TYPE_3;
	mr.P = 0;
	mr.Reserved_1 = 0;
	mr.Reserved_2 = 0;
	mr.Reserved_3 = 0;
	mr.M = 0;
	mr.Record_Count = 1;
	mr.nonce1 = 0;
	mr.nonce2 = 0;
	mr.Key_Id = KEY_ID_HMAC_SHA_1_96 ;
	mr.Authentication_Data_Length = htons(4);
	mr.Authentication_Data = 0; // htons(0) = 0

	WritablePacket *p = Packet::make(headroom, &mr, sizeof(LISPMapRegister), 0);

	if (p == NULL) {
		click_chatter("[-] Packet creation failed!");
		return NULL;
	}

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRegister)
