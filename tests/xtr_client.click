/******************************************
 * xTR
 * MSMR addr: 10.0.0.2
 * "client" RLOC addr: 10.0.0.3
 * "client" addr: 192.168.0.10
 * "server" addr: 192.168.1.20
 * "server" RLOC addr: 10.0.0.4
 * "Reseau de destination" 192.168.1.1
 ******************************************/

define($RLOCIP 10.0.0.3);
define($MSMRIP 10.0.0.2);
define($SERVERRLOCIP 10.0.0.4);
define($CLIENTEID 192.168.0.10);
define($SERVEREID 192.168.1.20);

AddressInfo(eth_etr 10.0.0.3  d8-9d-67-99-55-8e);
AddressInfo(eth_itr 192.168.0.10  2-2-2-2-2-2);

ar :: ARPResponder(eth_etr, eth_itr);

arpq_etr :: ARPQuerier(eth_etr);

cl :: Classifier(12/0800,			// IP
			12/0806 20/0001,	// ARP Query
			12/0806 20/0002	// ARP Response
			);

cl[2] -> [1]arpq_etr;
cl[1] -> ar;
arpq_etr[0], arpq_etr[1], ar -> q :: Queue;

EIDRegistration :: {
	LISPGenMapRegister(EID 192.168.0.10)
	-> LISPRecordLocator(RLOCIPADDR $RLOCIP)
	-> UDPIPEncap($RLOCIP, 1234, $MSMRIP, 4342)
	-> Unqueue
	-> output;
};

/*
 * Encapsulation d’un paquet / Egress Tunnel Router
 */
ETR :: {
	tee :: Tee(2);

	tee[1] -> Queue -> LISPPrintDB(RLOC $SERVERRLOCIP) -> RequestEIDMapping :: {
		input
		-> LISPGenMapRequest(ITRADDR $RLOCIP)
		-> UDPIPEncap($RLOCIP, 1234, $MSMRIP, 4342)
		-> Unqueue
		-> output;
	} -> output;

	InfiniteSource(\<111111111111111111111111111111111111111111111111111111111111>, LIMIT 1)
		-> UDPIPEncap($CLIENTEID, 1234, $SERVEREID, 5678)
		//-> classifier :: IPClassifier(dst net 192.168.1.0/24, -)
		//-> CheckIPHeader(14)
		-> LISPEncapsulation(SRC $RLOCIP, SPORT 1234)
		-> resolv :: LISPResolv
		-> output;

	//classifier[1] -> Discard;
	resolv[1] -> tee;
	tee[0] -> failedResolvQueue :: Queue -> TimedUnqueue(INTERVAL 5) -> resolv;
};

ETR, EIDRegistration -> outIPQueue :: Queue
	-> SetUDPChecksum
	-> SetIPChecksum
	-> Unqueue
	-> arpq_etr
	-> q
	-> ToDevice(eth0);


FromDevice(eth0)
	-> cl
	 -> ReplyHandler :: {
		-> IPPrint
		-> Strip(42)
		-> Queue
		-> LISPExtractEIDAndUpdateDB(IS_REPLY true)
		-> Discard;
};
