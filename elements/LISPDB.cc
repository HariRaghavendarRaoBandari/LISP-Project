#include <click/config.h>
#include <click/hashtable.hh>
#include "LISPDB.hh"

CLICK_DECLS

static HashTable<uint32_t, uint32_t> eid2rloc;

void setEIDToRLOC(uint32_t eid, uint32_t rloc) {
	eid2rloc[eid] = rloc;
}

uint32_t getRLOCFromEID(uint32_t eid) {
	return eid2rloc[eid];
}

HashTable<uint32_t, uint32_t>::iterator getIterator() {
	return eid2rloc.begin();
}

CLICK_ENDDECLS
ELEMENT_PROVIDES(LISPDB)
