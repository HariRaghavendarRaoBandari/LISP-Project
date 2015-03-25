// Coté xTR
// (internet) < --[ eth0 ]----[ eth1 ]-- > (LAN)
RequestEIDMapping :: {
	input -> GetIPAddress(XXX)
	-> LISPGenMapRequest(ITRADDR) /* ITRADDR: adresse routabledu ITR */
	-> UDPIPEncap(eth0.Addr, RANDOM, MSMR.Addr, 4342)
	-> EnsureEther()
	-> ToDevice(eth0)
	};

// Ajouter un timer pour MapRegister
MapRegister :: LISPGenMapRegister(EID 127.0.0.10, EID 127.0.0.11, EID 127.0.0.12)
	-> LISPRecordLocator(eth0.Addr)
	-> UDPIPEncap(eth0.Addr, RANDOM, MSMR.Addr, 4342)
	-> EnsureEther()
	-> ToDevice(eth0);

// Côté MSMR (une seule interface: eth0)
FromDevice(eth0) -> CheckIPHeader(14)
	-> IPClassifier(udp port 4342,)
	-> Strip(42)
	-> c :: LISPClassifier
	-> Queue(XXX)
	-> LISPGenMapReply()
	-> LISPRecordLocator(eth0.Addr)
	-> UDPIPEncap(eth0.Addr, 4342, DST_ANNO, PORT)
	-> EnsureEther()
	-> ToDevice(eth0);

c[1] -> LISPExtractEIDAndUpdateDB(); // le paquet est un Map Register

c[2] -> Discard; // other

/*
 * (internet) < --[ eth0 ]----[ eth1 ]-- > (LAN)
 */

// Décapsulation d’un paquet
FromDevice(eth0)
	-> CheckIPHeader(14)
	-> IPClassifier(udp port 4341,)
	-> LISPDecap()
	-> Queue(XXX)
	-> ToDevice(eth1);

// Encapsulation d’un paquet
FromDevice(eth1)
	-> CheckIPHeader(14)
	-> LISPUDPIPEncap()
	-> Queue(XXX)
	-> resolv :: LISPResolv
	-> EnsureEther()
	-> ToDevice(eth0);

failResolvQueue :: Queue(XXX);
resolv[1] -> tee :: PullTee;
tee[1] -> failResolvQueue;
tee[0] -> RequestEIDMapping;
failResolvQueue -> resolv;