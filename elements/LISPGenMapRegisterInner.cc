#include <click/config.h>
#include "LISPGenMapRegisterInner.hh"
#include "LISPStructs.hh"
#include "endian.hh"

CLICK_DECLS

LISPGenMapRegisterInner::LISPGenMapRegisterInner() { }

LISPGenMapRegisterInner::~LISPGenMapRegisterInner() { }

Packet* LISPGenMapRegisterInner::simple_action(Packet *p) {

	LISPMapRegister *mr = (LISPMapRegister *) (p->data());
	
	// Modify options below
	mr->headerInner.Key_Id = KEY_ID_NONE;
	mr->headerInner.Authentication_Data_Length = 4;
	mr->headerInner.Authentication_Data = 0;
	convert(&(mr->headerInner), 2);
	// End of modification

	return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel Convert)
EXPORT_ELEMENT(LISPGenMapRegisterInner)
