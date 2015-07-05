/******************************************
 * xTR
 * eth0 addr: 10.0.0.3
 * eth1 addr: 192.168.0.1
 * (internet) < --[ eth0 ]----[ eth1 ]-- > (LAN)
 * "Reseau de destination" 192.168.1.1
 ******************************************/

require(lisp);

define($RLOCIP 10.0.0.3);
define($MSMRIP 10.0.0.2);

EIDRegistration :: {
	LISPGenMapRegister(EID 192.168.0.10, EID 192.168.0.11)
	-> LISPRecordLocator(RLOCIPADDR $RLOCIP)
	-> UDPIPEncap($RLOCIP, 1234, $MSMRIP, 4342)
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
		-> LISPGenMapRequest(ITRADDR $RLOCIP)
		-> UDPIPEncap($RLOCIP, 1234, $MSMRIP, 4342)
		-> EnsureEther()
		-> ToDevice(eth0);
	};

	FromDevice(eth1)
		-> IPClassifier(dst net 192.168.1.0/24, - )
		-> CheckIPHeader(14)
		-> LISPEncapsulation(SRC $RLOCIP, SPORT 1234)
		-> resolv :: LISPResolv
		-> Queue
		-> SetIPChecksum
		-> EnsureEther
		-> ToDevice(eth0);

	resolv[1] -> tee;
	tee[0] -> failedResolvQueue :: Queue(10) -> TimedUnqueue(INTERVAL 5) -> resolv;
};
