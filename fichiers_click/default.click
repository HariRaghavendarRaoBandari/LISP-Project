// Coté xTR
// (internet) < --[ eth0 ]----[ eth1 ]-- > (LAN)
RequestEIDMapping :: {
	input -> GetIPAddress(XXX)
	-> ClearPacketData()
	-> LISPGenMapRequestOuter()
	-> LISPNonce(SIZE=64)
	-> LISPGenMapRequest()
	-> UDPIPEncap(eth0.Addr, RANDOM, MSMR.Addr, 4342)
	-> ToDevice(eth0)
	};

// Ajouter un timer pour MapRegister
// MapRegister reçoit un paquet vierge avec l’EID en annotation.
MapRegister :: LISPGenMapRegisterOuter()
	-> LISPNonce(SIZE=24)
	-> LISPGenMapRegisterInner()
	-> LISPGenMSMRCommonBits()
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
	-> LISPGenMSMRCommonBits()
	-> UDPIPEncap(eth0.Addr, 4342, DST_ANNO, PORT)
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
	-> ToDevice(eth0);

failResolvQueue :: Queue(XXX);
resolv[1] -> tee :: Tee -> failResolvQueue;
tee[1] -> Queue(1) -> RequestEIDMapping;
failResolvQueue -> [1]resolv;
