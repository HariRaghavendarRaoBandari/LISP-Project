#include <click/config.h>
#include <hashtable.hh>
#include "LISPMsMr.hh"

CLICK_DECLS

/* <eid , rloc> */
HashTable<String, String> bd(0);


bool updateEid(uint32_t eid, uint32_t rloc) {
	IPAddress ipEid(eid);
	IPAddress ipRloc(eid);
	String strEid = ipEid.unparse();
	String strRloc = ipRloc.unparse();
	bd[strEid]=strRloc;
	return bd[strEid]!="";
}

uint32_t getRloc(uint32_t eid) {

	IPAddress ipEid(eid);
	IPAddress ipRloc(bd[ipEid.unparse()]);
	return ipRloc.addr();
}

CLICK_ENDDECLS
ELEMENT_PROVIDES(LISPMSMR) 