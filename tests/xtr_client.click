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

ETR, EIDRegistration -> Queue
	-> SetUDPChecksum
	-> SetIPChecksum
	-> EnsureEther
	-> ToDevice(eth0);

ReplyHandler :: {
		FromDevice(eth0)
		-> IPPrint
		-> Strip(42)
		-> Queue
		-> LISPExtractEIDAndUpdateDB(IS_REPLY true)
		-> Discard;
};
