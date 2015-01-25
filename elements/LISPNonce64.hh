#ifndef CLICK_LISPNONCE64_HH
#define CLICK_LISPNONCE64_HH

#include <click/element.hh>

// MRG32k3a defines
#define NORM 2.328306549295728e-10
#define m1   4294967087.0
#define m2   4294944443.0
#define a12     1403580.0
#define a13n     810728.0
#define a21      527612.0
#define a23n    1370589.0
#define SEED        12345

CLICK_DECLS

/*
=c

LISPNonce64()

=s LISPNonce64

Add 64 bits of Nonce.
The Nonce is created using MRG32k3a pseudo-random algorithm.

=d

Add 64 bits of Nonce.
 */
class LISPNonce64 : public Element {
	double s10, s11, s12, s20, s21, s22;
	uint32_t MRG32k3a();

	public:
		LISPNonce64() CLICK_COLD;
		~LISPNonce64() CLICK_COLD;

		const char *class_name() const { return "LISPNonce64"; }
		const char *port_count() const { return PORTS_1_1; }
		const char *processing() const { return AGNOSTIC; }
		int configure(Vector<String>&, ErrorHandler*) { return 0; }
		Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS
#endif
