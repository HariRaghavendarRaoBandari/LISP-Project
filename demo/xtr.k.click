define($MSMRIP 10.0.0.2);		// MSMR IP
define($RLOCIP 10.0.0.14);		// Our external IP (RLOC)
define($RLOCMAC 00:15:17:15:5d:76);	// Our external MAC
define($ITRIP 192.168.1.10);		// Our internal IP
define($ITRMAC 00:15:17:15:5d:77);	// Our internal MAC
define($REGISTEREID 192.168.1.50);	// The default EID to register

/*

EIDs <-- internal IP (vifX.1) - [xTR] - external IP (vifX.0) --> RLOCs

vifX.0 is connected to xenbr0.
vifX.1 is connected to xenbr1.

---

We need one classifier, arp querier and responder *per* interface.
EID registration is fired when starting.
*/

/*********************/
/*                   */
/* Do not edit below */
/*                   */
/*********************/

AddressInfo(eth_etr $RLOCIP $RLOCMAC);
AddressInfo(eth_itr $ITRIP $ITRMAC);

ar0 :: ARPResponder(eth_etr);
ar1 :: ARPResponder(eth_itr);
arpq0 :: ARPQuerier(eth_etr);
arpq1 :: ARPQuerier(eth_itr);

cl0 :: Classifier(12/0800, 12/0806 20/0001, 12/0806 20/0002);
cl1 :: Classifier(12/0800, 12/0806 20/0001, 12/0806 20/0002);

cl0[2] -> [1]arpq0;
cl0[1] -> ar0;

cl1[2] -> [1]arpq1;
cl1[1] -> ar1;

arpq0[0], arpq0[1], ar0 -> q0 :: Queue;
arpq1[0], arpq1[1], ar1 -> q1 :: Queue -> ToDevice(DEVID 1);

FromDevice(DEVID 1) -> cl1;

EIDRegistration :: {
	LISPGenMapRegister(EID $REGISTEREID)
		-> LISPRecordLocator(RLOCIPADDR $RLOCIP)
		-> UDPIPEncap($RLOCIP, 1234, $MSMRIP, 4342)
		-> Unqueue
		-> output;
};

cl1 -> ETR :: {
	tee :: Tee(2);

	tee[1] -> Queue -> RequestEIDMapping :: {
		input
			-> LISPGenMapRequest(ITRADDR $RLOCIP)
			-> UDPIPEncap($RLOCIP, 1234, $MSMRIP, 4342)
			-> Unqueue
			-> output;
	} -> output;

	input
		-> Strip(14)
		-> MarkIPHeader
		-> ipc2 :: IPClassifier(src host $REGISTEREID, -)
		-> LISPEncapsulation(SRC $RLOCIP, SPORT 1234)
		-> resolv :: LISPResolv
		-> output;

	ipc2[1] -> IPPrint -> Discard;
	resolv[1] -> tee;
	tee[0] -> failedResolvQueue :: Queue -> DelayUnqueue(DELAY 2) -> resolv;
};

ETR, EIDRegistration -> outETR :: Queue
	-> fixChecksum :: {
		input
			-> SetUDPChecksum
			-> SetIPChecksum
			-> Unqueue
			-> output
	} -> arpq0
		-> q0
		-> ToDevice(DEVID 0);

FromDevice(DEVID 0) -> cl0 ->CheckIPHeader2(14) -> ipc :: IPClassifier(src udp port 4342, dst udp port 4341,) -> ReplyHandler :: {
	input
		-> Strip(42)
		-> Queue
		-> LISPExtractEIDAndUpdateDB(IS_REPLY true)
		-> Discard;
};

ipc[1] -> ITR :: {
	input
		-> Strip(14)
		-> LISPDecapsulation
		-> output
} -> arpq1;
