#include <click/config.h>
#include "LISPGenMapRequest.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPGenMapRequest::LISPGenMapRequest() { }

LISPGenMapRequest::~LISPGenMapRequest() { }

int LISPGenMapRequest::configure(Vector<String> &conf, ErrorHandler *errh) {
	int res = Args(conf, this, errh).read_mp("ITRADDR", _ip).complete();

	if (res < 0)
		return -1;

	return 0;
}

Packet* LISPGenMapRequest::simple_action(Packet *p) {

	// Adjust the length of the packet data buffer
	if (p->length() < proper_size) // "increase"
		p = p->put(proper_size - p->length());
	else if (p->length() > proper_size) // "shrink"
		p->take(p->length() - proper_size);

	LISPMapRequest *mr = (LISPMapRequest *) p->data();

	// DEBUG
	click_chatter("\ndst ip anno: %s\n", p->dst_ip_anno().s().c_str());
	click_chatter("\nITR ip: %s\n", _ip.s().c_str());

	mr->Type = LISP_H_TYPE_1;
	mr->A = 0;
	mr->M = 0;
	mr->P = 0;
	mr->S = 0;
	mr->p = 0;
	mr->s = 0;
	mr->Reserved_1 = 0;
	mr->Reserved_2 = 0;
	mr->IRC = 0;
	mr->Record_Count = 1;
	mr->nonce1 = click_random();
	mr->nonce2 = click_random();
	mr->Source_EID_AFI = 0;
	mr->ITR_RLOC_AFI = AFI_IPV4;
	mr->ITR_RLOC_Address = _ip.addr();
	mr->Reserved = 0;
	mr->EID_mask_len = 32;
	mr->EID_prefix_AFI = AFI_IPV4;
	/*
	 * The destination address is in the dst_ip_anno annotation of the
	 * packet.
	 */
	mr->EID_prefix = p->dst_ip_anno().addr();
	/*
	 * The Map_Reply_Record is never set here because the M bit must be set
	 * to 0.
	 */

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRequest)
