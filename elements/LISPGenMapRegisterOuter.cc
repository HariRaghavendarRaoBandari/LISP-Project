#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/packet.hh>
#include "LISPGenMapRegisterOuter.hh"
#include "LISPMapRegisterOuterHeader.h"

CLICK_DECLS

static const uint32_t headroom = sizeof(click_ip) + sizeof(click_udp) + sizeof(click_ether);

LISPGenMapRegisterOuter::LISPGenMapRegisterOuter() { }

LISPGenMapRegisterOuter::~LISPGenMapRegisterOuter() { }

Packet* LISPGenMapRegisterOuter::pull(int) {
	click_chatter("TODO: test");

	struct LISPMapRegisterOuterHeader data;
	data.Type = LISP_H_TYPE_3;
	data.P = false;
	memset(data.M, 0, sizeof(data.Reserved));
	data.M = false;
	data.Record_Count = (uint8_t) 1;

	WritablePacket *p = Packet::make(headroom, data, 20 * 8, 0);

	if (p == NULL) {
		click_chatter("[-] Packet creation failed!");
		return NULL;
	}

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRegisterOuter)
