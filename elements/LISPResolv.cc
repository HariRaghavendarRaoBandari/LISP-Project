#include <click/config.h>
#include <clicknet/ip.h>
#include <click/glue.hh>
#include <click/ipaddress.hh>
#include <click/args.hh>
#include "LISPResolv.hh"
#include "LISPDB.hh"

CLICK_DECLS

LISPResolv::LISPResolv() : _timer((Element *) this) {
	_max_cache_ttl = 5;
}

LISPResolv::~LISPResolv() { }

int LISPResolv::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (Args(conf, this, errh).read("MAXTTL", _max_cache_ttl).complete() < 0)
		return -1;

	return 0;
}

int LISPResolv::initialize(ErrorHandler *e) {
	_timer.initialize((Element *) this);
	_timer.schedule_now();

	return 0;
}

void LISPResolv::run_timer(Timer *t) {
	assert(&_timer == t);
	// We need to reschedule _before_ checking if _eid_cache is emtpy
	_timer.reschedule_after_sec(1);

	if (_eid_cache.empty())
		return;

	click_chatter("@{%s} EID cache:\n", __PRETTY_FUNCTION__);

	// Print & modify the cache
	for (HashTable<uint32_t, int>::iterator it = _eid_cache.begin(); it ; it++) {
		_eid_cache[it.key()] -= 1;
		click_chatter("\t%s -> %d\n", IPAddress(it.key()).s().c_str(), it.value());

		if (_eid_cache[it.key()] < 0) {
			click_chatter("\t\trecord expired\n");
			eraseEID(it.key());
			it = _eid_cache.erase(it);
		}
	}
}

Packet *LISPResolv::simple_action(Packet *p) {
	click_ip *ip_oh = (click_ip *) p->data();
	// This is the destination EID
	uint32_t eid = p->dst_ip_anno();

	// We search for the RLOC associated with the destination EID
	IPAddress a = IPAddress(getRLOCFromEID(eid));

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
	_eid_cache[eid] = this->_max_cache_ttl; // reset the TTL for the destination EID

	return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(LISPDB)
EXPORT_ELEMENT(LISPResolv)
