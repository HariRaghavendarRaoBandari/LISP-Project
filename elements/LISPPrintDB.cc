#include <click/config.h>
#include <click/glue.hh>
#include <click/ipaddress.hh>
#include <click/hashtable.hh>
#include "LISPPrintDB.hh"
#include "LISPDB.hh"

CLICK_DECLS

LISPPrintDB::LISPPrintDB() { };
LISPPrintDB::~LISPPrintDB() { };

Packet *LISPPrintDB::simple_action(Packet *p) {
	HashTable<uint32_t, uint32_t>::iterator it;
	click_chatter("--------------------------------------------------------------------------------\n");
	click_chatter("EID\t\t->\tRLOC\n");
	click_chatter("================================================================================\n");

	for (it = getIterator(); it; it++)
		click_chatter("%s\t->\t%s\n", IPAddress(it.key()).s().c_str(), IPAddress(it.value()).s().c_str());

	click_chatter("--------------------------------------------------------------------------------\n");

	return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(LISPDB)
EXPORT_ELEMENT(LISPPrintDB)
