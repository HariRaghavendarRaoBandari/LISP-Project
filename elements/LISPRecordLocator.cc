#include <click/config.h>
#include "LISPRecordLocator.hh"
#include "LISPStructs.hh"

CLICK_DECLS

LISPRecordLocator::LISPRecordLocator() { }

LISPRecordLocator::~LISPRecordLocator() { }

int LISPRecordLocator::configure(Vector<String> &conf, ErrorHandler *errh) {
	int res = Args(conf, this, errh)
		.read_mp("RLOCIPADDR", rloc_ip)
		.read_mp("EIDIPADDR", eid_ip).complete();

	if (res < 0)
		return -1;

	return 0;
}

Packet* LISPRecordLocator::simple_action(Packet *p) {

	LISPMapRegister *mr = (LISPMapRegister *) (p->data());

	// Modify options below
	mr->rec.Record_TTL = 0; //mapping gardé en cache 10 mins
	mr->rec.Locator_Count = 1; //un seul locator necessaire ici (voir aucun, a voir)
	mr->rec.EID_Mask_Len = 32; // permet un sous réseau de 1 adresse
	mr->rec.A = 1;
	mr->rec.ACT = 0;
	mr->rec.Reserved1 = 0; //si different de 0 utiliser htons
	mr->rec.Reserved2 = 0;
	mr->rec.Reserved3 = 0;
	mr->rec.Map_Version1 = 0;
	mr->rec.Map_Version2 = 0;
	mr->rec.EID_Prefix_AFI = htons(1); //1 pour ipv4, 2 pour ipv6
	mr->rec.EID_Prefix = htonl(0xC0A8032A); // le map register ne gère que cette adresse, choisie ici arbitrairement 192.168.3.42
	mr->loc.Priority = 0; //priorité des rlocs pour unicast, de 0 à 255 (255 = rloc a ne pas utiliser pour le forwarding unicast)
	mr->loc.Weight = 0; //all weight are set to 0, un algo de hash simple sera utilisé pour distribuer la charge
	mr->loc.M_Priority = 0; //ON FAIT PAS DE MULTICAST
	mr->loc.M_Weight = 0; //ON FAIT PAS DE MULTICAST
	mr->loc.UnusedFlags1 = 0;
	mr->loc.UnusedFlags2 = 0;
	mr->loc.L = 0; //1 pour du reply d'un site avec proxi, 0 sans proxi
	mr->loc.p = 0; //ne sert que dans le map reply
	mr->loc.R = 0; //ne sert que dans le map reply
	mr->loc.Loc_AFI = htons(1); //1 pour ipv4, 2 pour ipv6
	mr->loc.Locator = htonl(0x01000001); 

	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LISPRecordLocator)
