#ifndef CLICK_LISPUDPIPENCAP_HH
#define CLICK_LISPUDPIPENCAP_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <click/atomic.hh>
#include <clicknet/udp.h>
#include "EIDToRLOC.hh"
CLICK_DECLS

/*
=c

LISPUDPIPEncap(SRC, SPORT)

=s LISPUDPIPEncap

TODO encapsulates packets in static UDP/IP headers

=d

TODO
Encapsulates each incoming packet in a UDP/IP packet with source address
SRC, source port SPORT, destination address DST, and destination port
DPORT. The UDP checksum is calculated if CHECKSUM? is true; it is true by
default.

As a special case, if DST is "DST_ANNO", then the destination address
is set to the incoming packet's destination address annotation.

The LISPUDPIPEncap element adds both a UDP header and an IP header.

The Strip element can be used by the receiver to get rid of the
encapsulation header.

=e
  LISPUDPIPEncap(1.0.0.1, 1234, 2.0.0.2, 1234)

*/

class LISPUDPIPEncap : public Element {
private:
	struct in_addr _saddr;
	uint16_t _sport;
	bool _cksum;
#if HAVE_FAST_CHECKSUM && FAST_CHECKSUM_ALIGNED
	bool _aligned;
	bool _checked_aligned;
#endif
	atomic_uint32_t _id;

public:
	LISPUDPIPEncap() CLICK_COLD;
	~LISPUDPIPEncap() CLICK_COLD;

	const char *class_name() const { return "LISPUDPIPEncap"; }
	const char *port_count() const { return "1/2"; }
	const char *flags() const { return PROCESSING_A_AH; }

	int configure(Vector<String> &, ErrorHandler *) CLICK_COLD;

	/* TODO
	 * Lorsqu'un EID n'est pas connu, on envoi le paquet vers la sortie 1,
	 * sinon on encapsule (UDP/IP) le paquet et on l'envoi vers la sortie 0.
	 *
	 * Une annotation pour détecter le nombre de tours fait par un paquet
	 * dont la résolution a échouée est souhaitable (on supprime le paquet
	 * quand on atteint le nombre de tours maximal).
	 * Une autre solution est d'utiliser les éléments CycleCountAccum et
	 * SetCycleCount pour remplir cette tache.
	 */
	Packet *simple_action(Packet *);
};

CLICK_ENDDECLS
#endif
