#include <click/config.h>
#include "LISPRecordLocator.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPRecordLocator::LISPRecordLocator() { }

LISPRecordLocator::~LISPRecordLocator() { }

int LISPRecordLocator::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (Args(conf, this, errh).read_m("RLOCIPADDR", IPAddressArg(), _rloc_ip).complete() < 0 || _rloc_ip.empty())
		return -1;

	return 0;
}

Packet* LISPRecordLocator::simple_action(Packet *p) {

	LISPMapRegister *mr = (LISPMapRegister *) (p->data());

	// Modify options below
	mr->rec.Record_TTL = htonl(1);
	mr->rec.Locator_Count = 1; // Only 1 locator
	mr->rec.EID_Mask_Len = 32; // Allow a sub network of one address
	mr->rec.A = 1;
	mr->rec.ACT = 0;
	mr->rec.Reserved1 = 0;
	mr->rec.Reserved2 = 0;
	mr->rec.Reserved3 = 0;
	mr->rec.Map_Version1 = 0;
	mr->rec.Map_Version2 = 0;
	mr->rec.EID_Prefix_AFI = AFI_IPV4;
	mr->rec.EID_Prefix = p->anno_u32(USER_ANNO_EID);

	mr->loc.Priority = 0; // RLOC priorities for unicast [0, 255] (255 = do not use RLOC to forward multicast)
	mr->loc.Weight = 0; // all weight are set to 0, a simple hash algorithm will be used to balance the load
	mr->loc.M_Priority = 0; // We _don't_ do multicast
	mr->loc.M_Weight = 0; // We _don't_ do multicast
	mr->loc.UnusedFlags1 = 0;
	mr->loc.UnusedFlags2 = 0;
	mr->loc.L = 0; // 0 -> no proxy, 1 -> use of proxy for a reply
	mr->loc.p = 0; // Only used for Map-Reply
	mr->loc.R = 0; // Only used for Map-Reply
	mr->loc.Loc_AFI = AFI_IPV4;
	mr->loc.Locator = _rloc_ip;

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPRecordLocator)
