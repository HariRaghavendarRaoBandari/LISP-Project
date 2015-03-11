#ifndef CLICK_LISPDB_HH
#define CLICK_LISPDB_HH

#include <click/config.h>
#include <click/hashtable.hh>

CLICK_DECLS

void setEIDToRLOC(uint32_t eid, uint32_t rloc);

uint32_t getRLOCFromEID(uint32_t eid);

HashTable<uint32_t, uint32_t>::iterator getIterator();

CLICK_ENDDECLS

#endif
