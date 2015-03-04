#include <click/config.h>
#include "LISPRecordLocator.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPRecordLocator::LISPRecordLocator() { }

LISPRecordLocator::~LISPRecordLocator() { }

int LISPRecordLocator::configure(Vector<String> &conf, ErrorHandler *errh) {
	int res = Args(conf, this, errh)
		.read_mp("RLOCIPADDR", rloc_ip)
		.read_mp("EIDIPADDR", eid_ip).complete();

	if (res < 0)
		return -1;

	return 0;
}

Packet* LISPRecordLocator::simple_action(Packet *p) {

	LISPMapRegister *mr = (LISPMapRegister *) (p->data());

	mr->recLoc.Record_TTL = htonl(10);
	mr->recLoc.Locator_count = 1;
	mr->recLoc.EID_mask_len = 32;
	mr->recLoc.ACT = 0;
	mr->recLoc.A = 1;
	mr->recLoc.Reserved = 0;
	mr->recLoc.Rsvd1 = 0;
	mr->recLoc.Rsvd2 = 0;
	mr->recLoc.Map_Version_number = 0;
	mr->recLoc.EID_Prefix_AFI = AFI_IPV4;
	mr->recLoc.EID_Prefix = eid_ip.addr();
	mr->recLoc.loc.Priority = 0;
	mr->recLoc.loc.Weight = 0;
	mr->recLoc.loc.Priority = 0;
	mr->recLoc.loc.M_Weight = 0;
	mr->recLoc.loc.Unused_Flags = 0;
	mr->recLoc.loc.L = 0;
	mr->recLoc.loc.P = 0; // only used in Map-Reply
	mr->recLoc.loc.R = 0; // only used in Map-Reply
	mr->recLoc.loc.Loc_AFI = AFI_IPV4;
	mr->recLoc.loc.Locator = rloc_ip.addr();

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPRecordLocator)
