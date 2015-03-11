#ifndef CLICK_MSMRFUNC_HH
#define CLICK_MSMRFUNC_HH

#include <click/config.h>

CLICK_DECLS

bool updateEid(uint32_t eid, uint32_t rloc);

uint32_t getRloc(uint32_t eid);

CLICK_ENDDECLS

#endif 