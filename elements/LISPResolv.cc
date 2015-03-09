#include <click/config.h>
#include <clicknet/ip.h>
#include <click/glue.hh>
#include <click/ipaddress.hh>
#include "LISPResolv.hh"
#include "LISPDB.hh"

CLICK_DECLS

LISPResolv::LISPResolv() { }

LISPResolv::~LISPResolv() { }

Packet *LISPResolv::simple_action(Packet *p) {
	click_ip *ip_oh = (click_ip *) p->data();

	// We search for the RLOC associated with the destination EID
	IPAddress a = IPAddress(getRLOCFromEID(p->dst_ip_anno()));

	if (a.empty()) {
		click_chatter("@{%s}: failed resolv -> output 1\n", __PRETTY_FUNCTION__);
		p->set_dst_ip_anno(a.addr());

		if (noutputs() == 2)
			output(1).push(p);
		else
			p->kill();

		return NULL;
	} else
		click_chatter("@{%s}: resovl succeeded -> output 0\n", __PRETTY_FUNCTION__);

	p->set_dst_ip_anno(a);
	ip_oh->ip_dst = a;

	return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(LISPDB)
EXPORT_ELEMENT(LISPResolv)
