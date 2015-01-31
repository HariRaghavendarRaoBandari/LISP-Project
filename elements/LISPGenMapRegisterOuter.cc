#include <click/config.h>
#include "LISPGenMapRegisterOuter.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPGenMapRegisterOuter::LISPGenMapRegisterOuter() { }

LISPGenMapRegisterOuter::~LISPGenMapRegisterOuter() { }

Packet* LISPGenMapRegisterOuter::pull(int) {

	struct LISPMapRegisterOuterHeader data;
	data.Type = LISP_H_TYPE_3;

	// Modify options below
	data.P = false;
	data.Reserved = 0x00000000; // max 07 FF FE => 0x00FEFF07
	data.M = false;
	data.Record_Count = 0x01000000;
	// End of modification

	uint32_t header = 0x00000000;
#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	header |= 0x00000030; // Type
	header |= data.P ? 0x00000008 : 0x00000000; // P
	header |= data.Reserved ? data.Reserved : 0x00000000;
	header |= data.M ? 0x00010000 : 0x00000000; // M
	header |= data.Record_Count ? data.Record_Count : 0x00000000; // Record_Count
#else
#error "Only little endian is supported"
#endif

	WritablePacket *p = Packet::make(headroom, &header, sizeof(uint32_t), 20 * 7); // 20 est à modifier en fonction d'"Authentication Data"

	if (p == NULL) {
		click_chatter("[-] Packet creation failed!");
		return NULL;
	}

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRegisterOuter)
