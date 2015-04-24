/******************************************
 * xTR
 * MSMR addr: 10.0.0.2
 * "client" RLOC addr: 10.0.0.3
 * "client" addr: 192.168.0.10
 * "server" addr: 192.168.1.20
 * "server" RLOC addr: 10.0.0.4
 * "Reseau de destination" 192.168.1.1
 ******************************************/

define($RLOCIP 10.0.0.4);
define($MSMRIP 10.0.0.2);
define($CLIENTEID 192.168.0.10);
define($SERVEREID 192.168.0.20);

q1 :: Queue;

EIDRegistration :: {
	LISPGenMapRegister(EID 192.168.1.20)
	-> LISPRecordLocator(RLOCIPADDR $RLOCIP)
	-> UDPIPEncap($RLOCIP, 1234, $MSMRIP, 4342)
	-> EnsureEther
	-> Unqueue
	-> output;
} -> q1;

/*
 * Décapsulation d’un paquet / Ingress Tunnel Router
 */
ITR :: {
	FromDevice(eth0, FORCE_IP true)
	-> CheckIPHeader(14)
	-> IPClassifier(udp port 4341,)
	-> LISPDecapsulation()
	-> EtherEncap(0x0800, 1:1:1:1:1:1, 2:2:2:2:2:2)
	-> output;
} -> q1;

q1
//-> SetUDPChecksum
//-> SetIPChecksum
-> ToDevice(eth0);

/*
 * Encapsulation d’un paquet / Egress Tunnel Router
 */

/*
ETR :: {
	tee :: Tee(2);

	tee[1] -> LISPPrintDB(RLOC 10.0.0.3) -> Queue -> RequestEIDMapping :: {
		input
		-> LISPGenMapRequest(ITRADDR $RLOCIP)
		-> UDPIPEncap($RLOCIP, 1234, $MSMRIP, 4342)
		-> EnsureEther()
		-> ToDevice(eth0);
	};

	InfiniteSource(\<111111111111111111111111111111111111111111111111111111111111>, LIMIT 1)
		-> UDPIPEncap($CLIENTEID, 1234, $SERVEREID, 5678)
		-> IPClassifier(dst net 192.168.0.0/24, - )
		-> CheckIPHeader(14)
		-> LISPEncapsulation(SRC $RLOCIP, SPORT 1234)
		-> resolv :: LISPResolv
		-> Queue
		-> SetIPChecksum
		-> EnsureEther
		-> ToDevice(eth0);

	resolv[1] -> tee;
	tee[0] -> failedResolvQueue :: Queue -> TimedUnqueue(INTERVAL 5) -> resolv;
};
*/
