#include <click/config.h>
#include "LISPEncapsulation.hh"
#include "LISPStructs.hh"
#include <clicknet/udp.h>
#include <clicknet/ip.h>
#include <click/args.hh>
#include <click/packet.hh>
#include <click/error.hh>

CLICK_DECLS

LISPEncapsulation::LISPEncapsulation() { }

LISPEncapsulation::~LISPEncapsulation() { }

int
LISPEncapsulation::configure(Vector<String> &conf, ErrorHandler *errh)
{
    IPAddress saddr;
    uint16_t sport;

    if (Args(conf, this, errh)
	.read_mp("SRC", saddr)
	.read_mp("SPORT", IPPortArg(IP_PROTO_UDP), sport)
	.complete() < 0){
		return -1;
    }
    _saddr = saddr;
    _sport = htons(sport);
    return 0;
}

Packet* LISPEncapsulation::simple_action(Packet *p){
	/*
	        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |N|L|E|V|I|flags|            Nonce/Map-Version                  |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                 Instance ID/Locator-Status-Bits               |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/

	// Récupération pointeur sur inner_header

	click_ip* inner_ip = (click_ip*) p->data();

	// Modification taille paquet

	p = p->push(sizeof(struct LISPHeader)); // On agrandit le header de la taille de la structure LISPHeader

	struct LISPHeader *lh = (struct LISPHeader*) p->data(); // Le pointeur lh pointe sur le début de notre structure LISPHeader


	// Initialisation des valeurs pour LISP : Toutes les valeurs sont mise à 0 par defaut
	lh->N = 0;
	lh->L = 0;
	lh->E = 0;
	lh->V = 0;

	lh->I = 0;

	lh->flags = 0;


	// Initialisation des valeurs pour OUTERHEADER UDPIP

	p = p->push(sizeof(click_udp) + sizeof(click_ip)); // On agrandit le header de la taille d'un entête UDP/IP
	click_ip *ip = (click_ip *)(p->data()); // Pointeur click_ip sur l'entête IP de l'outerheader
	click_udp *udp = (click_udp *)(ip + 1); // Pointeur click_udp sur l'entête UDP de l'outerheader

	#if !HAVE_INDIFFERENT_ALIGNMENT
	assert((uintptr_t)ip % 4 == 0);
	#endif

	// set up IP header
	ip->ip_v = 4;
	ip->ip_hl = sizeof(click_ip) >> 2;
	ip->ip_id = inner_ip->ip_id;
	ip->ip_p = 17; // Num Protocole UDP = 17
	ip->ip_src = _saddr;
	ip->ip_len = htons(p->length());
	ip->ip_dst = p->dst_ip_anno(); // Ajout de la destination de l'annotation
	p->set_dst_ip_anno(inner_ip->ip_dst);

	ip->ip_tos = inner_ip->ip_tos; // On récupère la valeur du paquet encapsulé
	ip->ip_off = 0;
	ip->ip_ttl = inner_ip->ip_ttl; // On récupère la valeur du paquet encapsulé
	ip->ip_sum = 0;
	p->set_ip_header(ip, sizeof(click_ip));

	// set up UDP header
	udp->uh_sport = _sport;
	udp->uh_dport = (uint16_t) htons(4341);
	uint16_t len = p->length() - sizeof(click_ip);
	udp->uh_ulen = htons(len);
	udp->uh_sum = 0;

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPEncapsulation)
