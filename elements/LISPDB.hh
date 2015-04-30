#ifndef CLICK_LISPDB_HH
#define CLICK_LISPDB_HH

#include <click/config.h>
#include <click/hashtable.hh>

CLICK_DECLS

/*
 * This database is in fact a hashtable, it holds eid to RLOC mappings.
 */

/*
 * Insert the mapping eid -> rloc in the hashtable.
 */
void setEIDToRLOC(uint32_t eid, uint32_t rloc);

/*
 * Get the associated RLOC from an EID, if no mapping could be foud, this
 * returns the address 0.0.0.0 (in uint32_t obviously).
 */
uint32_t getRLOCFromEID(uint32_t eid);

/*
 * Erase a mapping.
 */
void eraseEID(uint32_t eid);

/*
 * Returns an iterator on the hashtable.
 */
HashTable<uint32_t, uint32_t>::iterator getIterator();

CLICK_ENDDECLS

#endif
