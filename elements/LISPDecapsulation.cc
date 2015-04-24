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

	struct LISPHeader * lisp = (struct LISPHeader *)(p_in->data() + sizeof(click_ip) + sizeof(click_udp) );

	click_ip * inner_ip = (click_ip *)(p_in->data() + sizeof(click_ip) + sizeof(click_udp) + sizeof(struct LISPHeader));

	assert(lisp->N == 0);
	assert(lisp->L == 0);
	assert(lisp->E == 0);
	assert(lisp->V == 0);
	assert(lisp->I == 0);
	assert(lisp->flags == 0);
	/*
	assert(lisp->firstLine_1 == 0);
	assert(lisp->firstLine_2 == 0);
	assert(lisp->firstLine_3 == 0);
	assert(lisp->secondLine == 0);
	*/

	inner_ip->ip_tos = outer_ip->ip_tos;
	inner_ip->ip_ttl = outer_ip->ip_ttl;

	p_in->pull(sizeof(LISPHeader) + sizeof(click_ip) + sizeof(click_udp));

	p_in->set_dst_ip_anno(inner_ip->ip_dst);
	return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPDecapsulation)
