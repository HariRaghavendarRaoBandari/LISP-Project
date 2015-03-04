// Coté xTR
// (internet) < --[ eth0 ]----[ eth1 ]-- > (LAN)
RequestEIDMapping :: {
	input -> GetIPAddress(XXX)
	-> LISPGenMapRequest(IPADDR) /* IPADDR: adresse routabledu xTR */
	-> UDPIPEncap(eth0.Addr, RANDOM, MSMR.Addr, 4342)
	-> EnsureEther()
	-> ToDevice(eth0)
	};

// Ajouter un timer pour MapRegister
MapRegister :: LISPGenMapRegister()
	-> LISPRecordLocator(eth0.Addr, EIDADDR) /* EIDADDR a modifier pour avoir une version plus generique */
	-> UDPIPEncap(eth0.Addr, RANDOM, MSMR.Addr, 4342)
	-> EnsureEther()
	-> ToDevice(eth0);

// Côté MSMR (une seule interface: eth0)
FromDevice(eth0) -> CheckIPHeader(14)
	-> IPClassifier(udp port 4342,)
	-> Strip(42)
	-> c :: LISPClassifier
	-> LISPExtractEIDGetRLOC() // le paquet est un Map Request
	-> LISPGenMapReplyOuter()
	-> Queue(XXX)
	-> LISPNonce(SIZE=24)
	-> LISPRecordLocator(eth0.Addr, EIDADDR) /* EIDADDR a modifier pour avoir une version plus generique */
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
	-> Strip(42)
	-> LISPDecap()
	-> Queue(XXX)
	-> ToDevice(eth1);

// Encapsulation d’un paquet
FromDevice(eth1)
	-> CheckIPHeader(14)
	-> LISPEncap()
	-> Queue(XXX)
	-> resolv :: LISPUDPIPEncap(CHECKSUM)
	-> EnsureEther()
	-> ToDevice(eth0);

failResolvQueue :: Queue(XXX);
resolv[1] -> Unqueue() -> tee :: Tee -> failResolvQueue;
tee[1] -> Queue(1) -> RequestEIDMapping;
failResolvQueue -> resolv;
