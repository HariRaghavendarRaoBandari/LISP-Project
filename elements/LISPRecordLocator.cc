#include <click/config.h>
#include "LISPRecordLocator.hh"
#include "LISPStructs.hh"
#include "endian.hh"

CLICK_DECLS

LISPRecordLocator::LISPRecordLocator() { }

LISPRecordLocator::~LISPRecordLocator() { }

Packet* LISPRecordLocator::simple_action(Packet *p) {

	LISPMapRegister *mr = (LISPMapRegister *) (p->data());
	
	// Modify options below
	mr->recLoc.Record_TTL = 10; //mapping gardé en cache 10 mins
	mr->recLoc.Locator_count = 1; //un seul locator necessaire ici (voir aucun, a voir)
	mr->recLoc.EID_mask_len = 32; // permet un sous réseau de 1 adresse
	mr->recLoc.ACT = 0; 
	mr->recLoc.A = 1;
	mr->recLoc.Reserved = 0;
	mr->recLoc.Map_Version_number = 0;
	mr->recLoc.EID_Prefix_AFI = 1; //1 pour ipv4, 2 pour ipv6
	mr->recLoc.EID_Prefix = 0xC0A8032A; // le map register ne gère que cette adresse, choisie ici arbitrairement 
	mr->recLoc.loc.Priority = 0; //priorité des rlocs pour unicast, de 0 à 255 (255 = rloc a ne pas utiliser pour le forwarding unicast)
	mr->recLoc.loc.Weight = 0; //all weight are set to 0, un algo de hash simple sera utilisé pour distribuer la charge
	mr->recLoc.loc.Priority = 0; //ON FAIT PAS DE MULTICAST
	mr->recLoc.loc.M_Weight = 0; //ON FAIT PAS DE MULTICAST
	mr->recLoc.loc.Unused_Flags = 0;
	mr->recLoc.loc.L = 0; //1 pour du reply d'un site avec proxi, 0 sans proxi
	mr->recLoc.loc.P = 0; //ne sert que dans le map reply
	mr->recLoc.loc.R = 0; //ne sert que dans le map reply
	mr->recLoc.loc.Loc_AFI = 1; //1 pour ipv4, 2 pour ipv6
	mr->recLoc.loc.Locator = 0x01000001; 

	convert(&(mr->recLoc), 7);
	// End of modification

	return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel Convert)
EXPORT_ELEMENT(LISPGenMapRegisterInner)
