define($RLOCIP 10.0.0.13);
define($MSMRIP 10.0.0.2);
define($SERVERRLOCIP 10.0.0.14);
define($CLIENTEID 192.168.0.10);
define($SERVEREID 192.168.1.20);

AddressInfo(eth_etr 10.0.0.13  00:15:17:15:5d:74);
AddressInfo(eth_itr 192.168.0.10  00:15:17:15:5d:75);

ar :: ARPResponder(eth_etr, eth_itr);
arpq_etr :: ARPQuerier(eth_etr);

cl :: Classifier(12/0800, 12/0806 20/0001, 12/0806 20/0002);

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
		-> LISPEncapsulation(SRC $RLOCIP, SPORT 1234)
		-> resolv :: LISPResolv
		-> output;

	resolv[1] -> tee;
	tee[0] -> failedResolvQueue :: Queue -> DelayUnqueue(DELAY 2) -> resolv;
};

ETR, EIDRegistration -> outIPQueue :: Queue -> SetUDPChecksum -> SetIPChecksum -> Unqueue -> arpq_etr -> q -> ToDevice(DEVID 0);

FromDevice(DEVID 0) -> cl ->CheckIPHeader2(14) -> IPClassifier(src udp port 4342,) -> ReplyHandler :: {
	input
		-> Strip(42)
		-> Queue
		-> LISPExtractEIDAndUpdateDB(IS_REPLY true)
		-> Discard;
};