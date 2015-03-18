#include <click/config.h>
#include <click/hashtable.hh>
#include "EIDToRLOC.hh"

CLICK_DECLS

static HashTable<uint32_t, uint32_t> rloc2eid, eid2rloc;

void setEIDToRLOC(uint32_t eid, uint32_t rloc) {
	eid2rloc[eid] = rloc;
}

void setRLOCToEID(uint32_t rloc, uint32_t eid) {
	rloc2eid[rloc] = eid;
}

uint32_t getRLOCFromEID(uint32_t eid) {
	return eid2rloc[eid];
}

uint32_t getEIDFromRLOC(uint32_t rloc) {
	return rloc2eid[eid];
}

HashTable<uint32_t, uint32_t>::iterator getIterator(bool isEIDToRLOC) {
	return isEIDToRLOC ? eid2rloc.begin() : rloc2eid.begin();
}

CLICK_ENDDECLS
ELEMENT_PROVIDES(LISPDB)
