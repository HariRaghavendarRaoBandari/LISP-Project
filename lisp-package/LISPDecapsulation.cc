#include <click/config.h>
#include <clicknet/ip.h>
#include <clicknet/udp.h>
#include <assert.h>
#include "LISPDecapsulation.hh"
#include "LISPStructs.hh"
CLICK_DECLS

LISPDecapsulation::LISPDecapsulation() { }
 
LISPDecapsulation::~LISPDecapsulation() { }

Packet* LISPDecapsulation::simple_action(Packet *p_in)
{
	click_ip* outer_ip = (click_ip *)(p_in->data());
	int tos = outer_ip->ip_tos;
	int ttl = outer_ip->ip_ttl;

	p_in->pull(sizeof(click_ip) + sizeof(click_udp));

	struct LISPHeader *lisp = (struct LISPHeader *) p_in->data();

	assert(lisp->N == 0);
	assert(lisp->L == 0);
	assert(lisp->E == 0);
	assert(lisp->V == 0);
	assert(lisp->I == 0);
	assert(lisp->flags == 0);
	
	p_in->pull(sizeof(LISPHeader));
	click_ip * inner_ip = (click_ip *) p_in->data();

/*	inner_ip->ip_tos = tos;
	inner_ip->ip_ttl = ttl;
	inner_ip->ip_len = p_in->length();
*/
	p_in->set_dst_ip_anno(inner_ip->ip_dst);

	return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPDecapsulation)
