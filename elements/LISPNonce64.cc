#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include "LISPNonce64.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPNonce64::LISPNonce64() {
	s10 = s11 = s12 = s20 = s21 = s22 = SEED;
}

LISPNonce64::~LISPNonce64() { }

/*
 * Pseudo random generator.
 * see http://simul.iro.umontreal.ca/rng/MRG32k3a.c
 */
uint32_t LISPNonce64::MRG32k3a() {
	long k;
	double p1, p2, res;

	/* Component 1 */
	p1 = a12 * s11 - a13n * s10;
	k = p1 / m1;
	p1 -= k * m1;

	if (p1 < 0.0)
		p1 += m1;

	s10 = s11;
	s11 = s12;
	s12 = p1;
	
	/* Component 2 */
	p2 = a21 * s22 - a23n * s20;
	k = p2 / m2;
	p2 -= k * m2;

	if (p2 < 0.0)
		p2 += m2;

	s20 = s21;
	s21 = s22;
	s22 = p2;
	
	/* Combination */
	if (p1 <= p2)
		res = (p1 - p2 + m1) * NORM;
	else
		res = (p1 - p2) * NORM;

	return res * 0xFFFFFFFF; // double -> uint32_t without truncation
}

Packet* LISPNonce64::simple_action(Packet *p) {
	LISPMapRegister *mr = (LISPMapRegister *)(p->data());

	mr->nonce1 = MRG32k3a();
	mr->nonce2 = MRG32k3a();

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPNonce64)
