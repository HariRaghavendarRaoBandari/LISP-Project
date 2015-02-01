#include <click/config.h>
#include "LISPGenMapRegisterInner.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPGenMapRegisterInner::LISPGenMapRegisterInner() { }

LISPGenMapRegisterInner::~LISPGenMapRegisterInner() { }

Packet* LISPGenMapRegisterInner::pull(int) {

	struct LISPMapRegisterInnerHeader data;
	
	// Modify options below
	data.Key_Id = KEY_ID_NONE;
	data.Authentication_Data_Length = 0x04000000; // (00 00 00 04)
	data.Authentication_Data = 0x00000000;
	// End of modification

	uint32_t header [2] = {0x00000000, 0x00000000}; // mody
#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	header[0] |= data.Key_Id; // key_id
	header[0] |= data.Authentication_Data_Length; // authentication data length
	header[1] |= data.Authentication_Data? data.Authentication_Data : 0x00000000; // authentication data
#else
#error "Only little endian is supported"
#endif

	WritablePacket *p = Packet::make(headroom, &header, 2*sizeof(uint32_t), 28 * 7); // 2 is to be modified depending on "Authentication Data"

	if (p == NULL) {
		click_chatter("[-] Packet creation failed!");
		return NULL;
	}

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRegisterInner)
