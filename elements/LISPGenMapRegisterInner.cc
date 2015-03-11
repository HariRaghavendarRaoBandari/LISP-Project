#include <click/config.h>
#include "LISPGenMapRegisterInner.hh"
#include "LISPStructs.hh"

#include <arpa/inet.h>

CLICK_DECLS

LISPGenMapRegisterInner::LISPGenMapRegisterInner() { }

LISPGenMapRegisterInner::~LISPGenMapRegisterInner() { }

Packet* LISPGenMapRegisterInner::simple_action(Packet *p) {

	LISPMapRegister *mr = (LISPMapRegister *) (p->data());
	
	// Modify options below
	mr->headerInner.Key_Id = htons(KEY_ID_HMAC_SHA_1_96) ;
	mr->headerInner.Authentication_Data_Length = htons(4);
	mr->headerInner.Authentication_Data = 0;//htons(0);
	//convert(&(mr->headerInner), 1);
	// End of modification

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRegisterInner)
