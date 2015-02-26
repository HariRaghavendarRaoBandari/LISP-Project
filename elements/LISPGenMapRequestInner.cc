#include <click/config.h>
#include "LISPGenMapRequestInner.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPGenMapRequestInner::LISPGenMapRequestInner() { }

LISPGenMapRequestInner::~LISPGenMapRequestInner() { }

int LISPGenMapRequestInner::configure(Vector<String> &conf, ErrorHandler *errh) {
	int res = Args(conf, this, errh).read_mp("ITRADDR", _ip).complete();

	if (res < 0)
		return -1;

	return 0;
}

Packet* LISPGenMapRequestInner::simple_action(Packet *p) {
	LISPMapRequest *mr = (LISPMapRequest *) p->data();

	// DEBUG
	click_chatter("\ndst ip anno: %s\n", p->dst_ip_anno().s().c_str());
	click_chatter("\nITR ip: %s\n", _ip.s().c_str());

	mr->inner.Source_EID_AFI = 0;
	mr->inner.ITR_RLOC_AFI = AFI_IPV4;
	mr->inner.ITR_RLOC_Address = _ip.addr();
	mr->inner.Reserved = 0;
	mr->inner.EID_mask_len = 32;
	mr->inner.EID_prefix_AFI = AFI_IPV4;
	/*
	 * The destination address is in the dst_ip_anno annotation of the
	 * packet.
	 */
	mr->inner.EID_prefix = p->dst_ip_anno().addr();
	/*
	 * The Map_Reply_Record is never set here because the M bit must be set
	 * to 0.
	 */

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPGenMapRequestInner)
