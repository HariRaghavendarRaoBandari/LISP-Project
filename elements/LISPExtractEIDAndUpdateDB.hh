#ifndef CLICK_LISPEXTRACTUPDATEDB_HH
#define CLICK_LISPEXTRACTUPDATEDB_HH
#include "LISPStructs.hh"
#include <click/element.hh>

CLICK_DECLS

class LISPExtractEIDAndUpdateDB : public Element {

public:

	LISPExtractEIDAndUpdateDB() { }
	~LISPExtractEIDAndUpdateDB() { }

	const char *class_name() const { return "LISPExtractEIDAndUpdateDB"; }
	const char *port_count() const { return "1/1"; }
	const char *processing() const { return PULL; }

	void debug();
	Packet* simple_action(Packet *p);

};

CLICK_ENDDECLS
#endif
