#include <click/config.h>
#include "LISPEncapsulation.hh"
#include "LISPStructs.hh"
#include <clicknet/ip.h>
#include <click/packet.hh>

CLICK_DECLS

LISPEncapsulation::LISPEncapsulation() { }

LISPEncapsulation::~LISPEncapsulation() { }

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

	// Récupération du inner-header

	click_ip* inner_header = (click_ip*) p->data();
	
	uint8_t ip_ttl_ih = inner_header->ip_ttl;
	uint8_t ip_tos_ih = inner_header->ip_tos;
	
	/* 
	 * Annotations
	 * /!\ ATTENTION RISQUE DE PROBLEMES /!\
	 * Offset de 0 et de 1 mais risque d'erreur
	 *
	 * set_anno_u8 (0, ip_ttl_ih);
	 * set_anno_u8 (1, ip_tos_ih);
	 */

	// Modification taille paquet

	p = p->push(sizeof(struct LISPHeader)); // On agrandit le header de la taille de la structure LISPHeader

	struct LISPHeader *lh = (struct LISPHeader*) p->data(); // Notre pointeur inner_header pointe desormais sur le début de notre structure LISPHeader


	// Initialisation des valeurs
	lh->N = 0;
	lh->L = 0;
	lh->E = 0;
	lh->V = 0;

	lh->I = 0;

	lh->flags = 0;

	lh->firstLine_1 = 0;
	lh->firstLine_2 = 0;
	lh->firstLine_3 = 0;
	lh->secondLine = 0;

	return p;
	
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPEncapsulation)
