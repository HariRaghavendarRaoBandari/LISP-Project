#include <click/config.h>
#include "LISPGenMapReply.hh"
#include "LISPStructs.hh"
#include "LISPDB.hh"

CLICK_DECLS

LISPGenMapReply::LISPGenMapReply() { }

LISPGenMapReply::~LISPGenMapReply() { }


void LISPGenMapReply::setOuterHeader(LISPMapReply* reply ) {
	reply->oh.Type = LISP_H_TYPE_2;
	reply->oh.P = 0;
	reply->oh.E = 0;
	reply->oh.S = 0;
	reply->oh.Reserved_1 = 0;
	reply->oh.Reserved_2 = 0;
	reply->oh.Reserved_3 = 0;
	reply->oh.Record_Count = 1; // Fixed to 1
}

void LISPGenMapReply::setInnerHeader(LISPMapReply* reply) {
	reply->rec.Record_TTL = 0xFFFFFFFF; // recipient decide locally how long to store the mapping
	reply->rec.EID_Mask_Len = 32; // fixed to 32bit mask len
	reply->rec.ACT = 0;
	reply->rec.A = 0;
	reply->rec.Reserved1 = 0;
	reply->rec.Reserved2 = 0;
	reply->rec.Reserved3 = 0;
	reply->rec.Map_Version1 = 0;
	reply->rec.Map_Version2 = 0;
	reply->rec.EID_Prefix_AFI = AFI_IPV4; // fixed to IPv4
}

void LISPGenMapReply::setLocator(LISPMapReply* reply ) {
	reply->loc.Priority = 0;
	reply->loc.UnusedFlags2 = 0;
	reply->loc.Weight = 0;
	reply->loc.M_Priority = 0;
	reply->loc.M_Weight = 0;
	reply->loc.L = 0;
	reply->loc.p = 0;
	reply->loc.R = 0;
	reply->loc.Loc_AFI = AFI_IPV4;
}

Packet* LISPGenMapReply::simple_action(Packet* inP) {

	// extract EID and resolv RLOC
	/*
	 * Free inP
	 * Make outP
	 * Generate MapReply
	 */

	// build reply packet
	WritablePacket *outP = Packet::make(sizeof(LISPMapReply));

	LISPMapReply *reply = (LISPMapReply*) outP->data();
	LISPMapRequest *request = (LISPMapRequest*) inP->data();

	click_chatter("EID_Prefix = %X\n", request->EID_prefix);

	reply->rec.EID_Prefix = request->EID_prefix;
	// TODO call prefix-check

	unsigned int locator = getRLOCFromEID(request->EID_prefix);
	reply->rec.Locator_Count = (locator == 0) ? 0 : 1;
	reply->loc.Locator = locator;

	setOuterHeader(reply);

	// set Nonce
	reply->nonce1 = request->nonce1;
	reply->nonce2 = request->nonce2;

	setInnerHeader(reply);

	setLocator(reply);

	// destroy request packet.
	inP->kill();

	return outP;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapReply)
