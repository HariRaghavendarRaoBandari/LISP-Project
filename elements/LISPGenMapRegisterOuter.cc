#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/packet.hh>
#include <clicknet/ip.h>
#include <clicknet/udp.h>
#include <clicknet/ether.h>
#include "LISPGenMapRegisterOuter.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPGenMapRegisterOuter::LISPGenMapRegisterOuter() { }

LISPGenMapRegisterOuter::~LISPGenMapRegisterOuter() { }

Packet* LISPGenMapRegisterOuter::pull(int) {
	struct LISPMapRegisterOuterHeader data;
	data.Type.value = LISP_H_TYPE_3;
	data.P = false;
	data.Reserved.value = 0;
	data.M = false;
	data.Record_Count = (uint8_t) 1;

	WritablePacket *p = Packet::make(headroom, &data, 20 * 8, 0); // 20 est à modifier en fonction d'"Authentication Data"

	if (p == NULL) {
		click_chatter("[-] Packet creation failed!");
		return NULL;
	}

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRegisterOuter)
