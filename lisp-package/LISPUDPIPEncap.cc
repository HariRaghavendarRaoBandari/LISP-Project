#include <click/config.h>
#include <clicknet/ip.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/standard/alignmentinfo.hh>
#include <click/ipaddress.hh>
#include "LISPUDPIPEncap.hh"
CLICK_DECLS

LISPUDPIPEncap::LISPUDPIPEncap() : _cksum(true) {
	_id = 0;
	_cksum = true;
#if HAVE_FAST_CHECKSUM && FAST_CHECKSUM_ALIGNED
	_checked_aligned = false;
#endif
}

LISPUDPIPEncap::~LISPUDPIPEncap() { }

int LISPUDPIPEncap::configure(Vector<String> &conf, ErrorHandler *errh) {
	IPAddress saddr;
	uint16_t sport;

	if (Args(conf, this, errh)
			.read_mp("SRC", saddr)
			.read_mp("SPORT", IPPortArg(IP_PROTO_UDP), sport)
			.complete() < 0)
		return -1;

	_saddr = saddr;
	_sport = htons(sport);

#if HAVE_FAST_CHECKSUM && FAST_CHECKSUM_ALIGNED
	if (!_checked_aligned) {
		int ans, c, o;

		ans = AlignmentInfo::query(this, 0, c, o);
		_aligned = (ans && c == 4 && o == 0);

		if (!_aligned)
			errh->warning("IP header unaligned, cannot use fast IP checksum");

		if (!ans)
			rrh->message("(Try passing the configuration through %<click-align%>.)");

		_checked_aligned = true;
	}
#endif

	return 0;
}

Packet *LISPUDPIPEncap::simple_action(Packet *p_in) {
	WritablePacket *p = p_in->push(sizeof(click_udp) + sizeof(click_ip));
	click_ip *ip = reinterpret_cast<click_ip *>(p->data());
	click_udp *udp = reinterpret_cast<click_udp *>(ip + 1);

#if !HAVE_INDIFFERENT_ALIGNMENT
	assert((uintptr_t)ip % 4 == 0);
#endif
	/*
	 * Modifications for LISP here.
	 */
	ip->ip_ttl = 250; // TODO modifier en récuppérant de l'annotation
	// We search for the RLOC associated with the destination EID
	IPAddress a = IPAddress(getRLOCFromEID(p->dst_ip_anno()));

	if (a.empty()) {
		click_chatter("@{%s}: failed resolv -> output 1\n", __PRETTY_FUNCTION__);
		a = IPAddress("0.0.0.0"); // set a to an empty address
		p->set_dst_ip_anno(a);
		output(1).push(p);
	} else
		click_chatter("@{%s}: resovl succeeded -> output 0\n", __PRETTY_FUNCTION__);

	p->set_dst_ip_anno(a);
	ip->ip_dst = a;
	udp->uh_dport = htons(4341); // 4341 is the default destination port

	// set up IP header
	ip->ip_v = 4;
	ip->ip_hl = sizeof(click_ip) >> 2;
	ip->ip_len = htons(p->length());
	ip->ip_id = htons(_id.fetch_and_add(1));
	ip->ip_p = IP_PROTO_UDP;
	ip->ip_src = _saddr;
	ip->ip_tos = 0;
	ip->ip_off = 0;
	ip->ip_sum = 0;

#if HAVE_FAST_CHECKSUM && FAST_CHECKSUM_ALIGNED
	if (_aligned)
		ip->ip_sum = ip_fast_csum((unsigned char *)ip, sizeof(click_ip) >> 2);
	else
		ip->ip_sum = click_in_cksum((unsigned char *)ip, sizeof(click_ip));
#elif HAVE_FAST_CHECKSUM
	ip->ip_sum = ip_fast_csum((unsigned char *)ip, sizeof(click_ip) >> 2);
#else
	ip->ip_sum = click_in_cksum((unsigned char *)ip, sizeof(click_ip));
#endif

	p->set_ip_header(ip, sizeof(click_ip));

	// set up UDP header
	udp->uh_sport = _sport;
	uint16_t len = p->length() - sizeof(click_ip);
	udp->uh_ulen = htons(len);
	udp->uh_sum = 0;

	if (_cksum) {
		unsigned csum = click_in_cksum((unsigned char *)udp, len);
		udp->uh_sum = click_in_cksum_pseudohdr(csum, ip, len);
	}

	return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(EIDToRLOC)
EXPORT_ELEMENT(LISPUDPIPEncap)
