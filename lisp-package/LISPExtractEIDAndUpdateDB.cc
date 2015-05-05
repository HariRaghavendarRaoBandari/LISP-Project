#include <click/config.h>
#include <click/ipaddress.hh>
#include <click/args.hh>
#include <click/glue.hh>
#include "LISPExtractEIDAndUpdateDB.hh"
#include "LISPStructs.hh"
#include "LISPDB.hh"

CLICK_DECLS

int LISPExtractEIDAndUpdateDB::configure(Vector<String> &conf, ErrorHandler *errh) {

	if (Args(conf, this, errh).read_m("IS_REPLY", _is_reply).complete() < 0)
		return -1;

	return 0;
}

void LISPExtractEIDAndUpdateDB::debug()
{
	HashTable<uint32_t, uint32_t>::iterator it;
	click_chatter("--------------------------------------------------------------------------------\n");
	click_chatter("EID\t\t->\tRLOC\n");
	click_chatter("================================================================================\n");

	for (it = getIterator(); it; it++)
		click_chatter("%s\t->\t%s\n", IPAddress(it.key()).s().c_str(), IPAddress(it.value()).s().c_str());

	click_chatter("--------------------------------------------------------------------------------\n");

}

Packet* LISPExtractEIDAndUpdateDB::simple_action(Packet *inP)
{	
	debug();

	LISPMapRegister *regis = (LISPMapRegister*) inP->data();
	LISPMapReply *reply = NULL;

	uint32_t eid, rloc;
	eid = rloc = 0; // avoid compiler (g++) warnings

	if (regis->Type == LISP_H_TYPE_2 && _is_reply) {
		reply = (LISPMapReply *) inP->data();
		eid = reply->rec.EID_Prefix;
		rloc = reply->loc.Locator;

	} else if (regis->Type == LISP_H_TYPE_3 && !_is_reply) {
		eid = regis->rec.EID_Prefix;

		if (getRLOCFromEID(eid) != 0) {
			click_chatter("Remove mapping.");
			eraseEID(eid); 
		}

		rloc = regis->loc.Locator;
	}

	if (rloc != 0 && eid != 0) {
		// For the pretty print only
		IPAddress rloc_ip(rloc);
		IPAddress eid_ip(eid);
		click_chatter("Add mapping EID -> RLOC: %s -> %s", eid_ip.s().c_str(), rloc_ip.s().c_str());

		// Finally add the mapping
		setEIDToRLOC(eid, rloc);

	} else
		click_chatter("EID or RLOC is 0.0.0.0, we can't add that.");

	inP->kill();
	return NULL;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPExtractEIDAndUpdateDB);
