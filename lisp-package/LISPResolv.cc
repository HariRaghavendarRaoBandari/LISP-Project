#include <click/config.h>
#include <clicknet/ip.h>
#include <click/glue.hh>
#include <click/ipaddress.hh>
#include <click/args.hh>
#include "LISPResolv.hh"
#include "LISPDB.hh"
#include "LISPStructs.hh" // for RESOLV_COUNTER_OFFSET

CLICK_DECLS

LISPResolv::LISPResolv() : _timer((Element *) this) { }

LISPResolv::~LISPResolv() { }

int LISPResolv::configure(Vector<String> &conf, ErrorHandler *errh) {
	int res = Args(conf, this, errh)
		.read_or_set("MAXTTL", _max_cache_ttl, 5)
		.read_or_set("MAXFAILEDRESOLV", max_failed_resolv, 5)
		.complete();

	if (res < 0)
		return -1;

	return 0;
}

int LISPResolv::initialize(ErrorHandler *) {
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

	click_chatter("@LISPResolv EID cache:\n");

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
		//click_chatter("Resolv failed -> output 1 (if connected)\n");
		p->set_dst_ip_anno(eid);
		uint8_t failed_resolv = p->anno_u8(RESOLV_COUNTER_OFFSET);
		p->set_anno_u8(RESOLV_COUNTER_OFFSET, ++failed_resolv);

		if (noutputs() == 2 && failed_resolv < max_failed_resolv)
			output(1).push(p);
		else {
			click_chatter("\toutput 1 not connected or max_failed_resolv is reached\n");
			p->kill();
		}

		return NULL;
	} else
		click_chatter("Resovl succeeded -> output 0\n");

	p->set_dst_ip_anno(a);
	ip_oh->ip_dst = a;
	_eid_cache[eid] = this->_max_cache_ttl; // reset the TTL for the destination EID

	return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(LISPDB)
EXPORT_ELEMENT(LISPResolv)
