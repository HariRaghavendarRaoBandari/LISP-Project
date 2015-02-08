#include <click/config.h>
#include "LISPGenMapRegisterInner.hh"
#include "LISPStructs.hh"
#include "endian.h"

CLICK_DECLS

LISPGenMapRegisterInner::LISPGenMapRegisterInner() { }

LISPGenMapRegisterInner::~LISPGenMapRegisterInner() { }

Packet* LISPGenMapRegisterInner::simple_action(Packet *p) {

	LISPMapRegister *mr = (LISPMapRegister *) (p->data());
	
	// Modify options below
	mr->headerInner.Key_Id = KEY_ID_NONE;
	mr->headerInner.Authentication_Data_Length = 0x00000004;
	mr->headerInner.Authentication_Data = 0x00000000;
	convert(&(mr->headerInner.Key_Id), 2);
	// End of modification

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRegisterInner)
