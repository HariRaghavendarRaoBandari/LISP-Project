#include <click/config.h>
#include "LISPClassifier.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPClassifier::LISPClassifier(){
}

LISPClassifier::~LISPClassifier(){
}

inline bool check_request(const void* buf, uint32_t len) {
	const LISPMapRequest* req = (const LISPMapRequest*) buf;
	return len >= sizeof(LISPMapRequest)
	&& req->Type == LISP_H_TYPE_1
	&& req->A == 0
	&& req->M == 0
	&& req->P == 0
	&& req->S == 0
	&& req->p == 0
	&& req->s == 0
	&& req->IRC == 0
	&& req->Record_Count == 1
	&& req->Source_EID_AFI == 0
	&& req->ITR_RLOC_AFI == AFI_IPV4
	&& req->EID_mask_len == 32
	&& req->EID_prefix_AFI == AFI_IPV4
	;
}

inline bool check_register(const void* buf, uint32_t len) {
	const LISPMapRegister* reg = (const LISPMapRegister*) buf;
	return len >= sizeof(LISPMapRegister)
	&& reg->Type == LISP_H_TYPE_3
	&& reg->P == 0
	&& reg->M == 0
	&& reg->Record_Count == 1
	&& reg->Key_Id == KEY_ID_NONE
	&& reg->Authentication_Data_Length == 0
	;
}

void LISPClassifier::push(int, Packet* p) {
	const unsigned char* buf = p->data();
	uint32_t len = p->length();
	
	short outport = LISP_CL_INVALID;

	if (check_request(buf, len))
		outport = LISP_CL_REQUEST;
	else if (check_register(buf, len))
		outport = LISP_CL_REGISTER;

	// DEBUG
	//click_chatter("Function %s, outport: %d", __PRETTY_FUNCTION__, outport);
	output(outport).push(p);
}

EXPORT_ELEMENT(LISPClassifier)
