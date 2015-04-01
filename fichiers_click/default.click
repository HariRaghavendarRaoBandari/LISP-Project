
MSMR :: {
	FromDevice(eth0)
	-> CheckIPHeader(14)
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
};

/******************************************
 * xTR
 * eth0 addr: 10.0.0.3
 * eth1 addr: 192.168.0.1
 * (internet) < --[ eth0 ]----[ eth1 ]-- > (LAN)
 ******************************************/

EIDRegistration :: {
	LISPGenMapRegister(EID 192.168.0.10, EID 192.168.0.11)
	-> LISPRecordLocator(RLOCIPADDR 10.0.0.3)
	-> Print()
	-> UDPIPEncap(10.0.0.3, 1234, 10.0.0.2, 4342)
	-> EnsureEther()
	-> ToDevice(eth0);
};

/*
 * Décapsulation d’un paquet / Ingress Tunnel Router
 */
ITR :: {
	FromDevice(eth0)
	-> CheckIPHeader(14)
	-> IPClassifier(udp port 4341,)
	-> LISPDecapsulation()
	-> Queue(XXX)
	-> ToDevice(eth1);
};

/*
 * Encapsulation d’un paquet / Egress Tunnel Router
 */

ETR :: {
	tee :: Tee(2);
	
	tee[1] -> LISPPrintDB(RLOC 10.0.0.4) -> Queue(10) -> RequestEIDMapping :: {
		input
		-> LISPGenMapRequest(10.0.0.3)
		-> UDPIPEncap(10.0.0.3, 1234, 10.0.0.2, 4342)
		-> EnsureEther()
		-> ToDump(resolv_map_requested.pcap);
	};

	FromDevice(eth1)
		-> CheckIPHeader(14)
		-> LISPEncapsulation(SRC 192.168.0.10, SPORT 1234)
		-> resolv :: LISPResolv
		-> Queue(10)
		-> SetIPChecksum
		-> EnsureEther
		-> ToDevice(eth0);

	resolv[1] -> tee;
	tee[0] -> failedResolvQueue :: Queue(10) -> TimedUnqueue(INTERVAL 5) -> resolv;
};