#ifndef CLICK_LISPDB_HH
#define CLICK_LISPDB_HH

#include <click/config.h>
#include <click/hashtable.hh>

CLICK_DECLS
static HashTable<uint32_t, uint32_t> rloc2eid, eid2rloc;

void setEIDToRLOC(uint32_t eid, uint32_t rloc);

void setRLOCToEID(uint32_t rloc, uint32_t eid);

uint32_t getRLOCFromEID(uint32_t eid);

uint32_t getEIDFromRLOC(uint32_t rloc);

HashTable<uint32_t, uint32_t>::iterator getIterator(bool isEIDToRLOC);

CLICK_ENDDECLS

#endif
