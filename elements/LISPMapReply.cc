#include <click/config.h>
#include "LISPMapReply.hh"
#include "LISPStructs.hh"
#include "LISPMsMr.hh"

CLICK_DECLS

LISPGenMapReply::LISPGenMapReply() { }

LISPGenMapReply::~LISPGenMapReply() { }

Packet* LISPGenMapReply::simple_action(int i, Packet* inP) {

	// extract EID and resolv RLOC
	/*
	 * Free inP
	 * Make outP
	 * Generate MapReply
	 */

	// build reply packet
	WritablePacket *outP = Packet::make(sizeof(LISPMapReply));

	LISPMapReply *reply = (LISPMapReply*) outP->data();
	LISPMapRequest *request = (LISPMapRequest*) inP->uniqueify()->data();

	reply->ih.EID_Prefix = request->inner.EID_prefix;

	// set header fileds.
	reply->oh.Type = LISP_H_TYPE_2;
	reply->oh.P = 0;
	reply->oh.E = 0;
	reply->oh.S = 0;
	reply->oh.Reserved1 = 0;
	reply->oh.Reserved2 = 0;
	reply->oh.Record_Count = 1; // Fixed to 1

	// set Nonce ( copy request nonce )
	reply->nonce1 = request->nonce1;
	reply->nonce2 = request->nonce2;
	
	set_anno_u32(USER_ANNO_EID, getRloc(request->inner.EID_prefix));

	// destroy request packet.
	inP->kill();

	return outP;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapReply)
