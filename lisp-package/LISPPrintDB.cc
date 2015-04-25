#include <click/config.h>
#include <click/glue.hh>
#include <click/ipaddress.hh>
#include <click/hashtable.hh>
#include <click/args.hh>
#include <click/error.hh>
#include "LISPPrintDB.hh"
#include "LISPDB.hh"

CLICK_DECLS

int LISPPrintDB::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (Args(conf, this, errh).read("RLOC", IPAddressArg(), _rloc).complete() < 0)
		return -1;

	if (_rloc.empty())
		return errh->error("RLOC is empty, it's not possible");

	return 0;
}

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

enum { H_MAP };

/*
 * Reads a EID IP address from the handler and add the mapping with the
 * configured RLOC in the database.
 */
int LISPPrintDB::write_callback(const String &s, Element *e, void *vparam, ErrorHandler *errh) {
	if ((intptr_t) vparam != H_MAP)
		return 0;

	IPAddress eid(s);
	LISPPrintDB *me = static_cast<LISPPrintDB *>(e);

	if (eid.empty())
		return errh->error("Bad IP");

	click_chatter("[*] Add mapping EID -> RLOC: %s -> %s", eid.s().c_str(), me->_rloc.s().c_str());
	setEIDToRLOC(eid.addr(), me->_rloc.addr());

	return 0;
}

void LISPPrintDB::add_handlers() {
	add_write_handler("MAP", write_callback, H_MAP);
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(LISPDB)
EXPORT_ELEMENT(LISPPrintDB)
