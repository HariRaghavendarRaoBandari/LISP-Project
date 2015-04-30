/******************************************
 * xTR
 * MSMR addr: 10.0.0.2
 * "client" RLOC addr: 10.0.0.3
 * "client" addr: 192.168.0.10
 * "server" addr: 192.168.1.20
 * "server" RLOC addr: 10.0.0.4
 * "Reseau de destination" 192.168.1.1
 ******************************************/

define($RLOCIP 10.0.0.14);
define($MSMRIP 10.0.0.2);
define($CLIENTEID 192.168.0.10);
define($SERVEREID 192.168.1.20);

q0 :: Queue;
q1 :: Queue;
AddressInfo( eth_etr $RLOCIP 00:15:17:15:5d:76);
AddressInfo( eth_itr $SERVEREID 1:1:1:1:1:1 );
aq_etr :: ARPQuerier(eth_etr);
ar :: ARPResponder(eth_etr) -> q0;

/*
 * 0 -> IP
 * 1 -> ARP Queries
 * 2 -> ARP Replies
 * 3 -> Other
 */
c :: Classifier( 12/0800, 12/0806 20/0001, 12/0806 20/0002 );

c[2] -> [1]aq_etr;
aq_etr[1] -> q0;
FromDevice(DEVID 0) -> c;

// incoming ARP Replies...
c[1] -> ar;

EIDRegistration :: {
	LISPGenMapRegister(EID $SERVEREID)
	-> LISPRecordLocator(RLOCIPADDR $RLOCIP)
	-> UDPIPEncap($RLOCIP, 1234, $MSMRIP, 4342)
	-> Unqueue
	-> output

} -> [0]aq_etr[0] -> IPPrint -> q0;

/*
 * Décapsulation d’un paquet / Ingress Tunnel Router
 */
c[0] -> ITR :: {

	input
	-> CheckIPHeader(14)
	-> IPPrint
	-> IPClassifier(dst udp port 4341,)
	-> Print("ICI!")
	-> Strip(14)
	-> LISPDecapsulation()
	-> SetUDPChecksum
	-> SetIPChecksum
	-> EtherEncap(0x0800, 1:1:1:1:1:1, 2:2:2:2:2:2)
	-> Print("HERE!")
	-> output


} -> q1;

q0 -> ToDevice(DEVID 0);
q1 -> ToDevice(DEVID 1);
