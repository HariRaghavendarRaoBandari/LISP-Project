/******************************************
 * xTR
 * MSMR addr: 10.0.0.2
 * "client" RLOC addr: 10.0.0.3
 * "client" addr: 192.168.0.10
 * "server" addr: 192.168.1.20
 * "server" RLOC addr: 10.0.0.4
 * "Reseau de destination" 192.168.1.1
 ******************************************/

require(lisp);

define($RLOCIP 10.0.0.4);
define($MSMRIP 10.0.0.2);
define($CLIENTEID 192.168.0.10);
define($SERVEREID 192.168.1.20);

q1 :: Queue;
AddressInfo( eth_etr 10.0.0.4 00:23:ae:74:49:8f );
AddressInfo( eth_itr 192.0.1.20 1:1:1:1:1:1 );
aq_etr :: ARPQuerier(eth_etr);
ar :: ARPResponder(eth_etr) -> q1;

/*
 * 0 -> IP
 * 1 -> ARP Queries
 * 2 -> ARP Replies
 * 3 -> Other
 */
c :: Classifier( 12/0800, 12/0806 20/0001, 12/0806 20/0002 );

c[2] -> [1]aq_etr;
aq_etr[1] -> q1;
FromDevice(eth0) -> c;

// incoming ARP Replies...
c[1] -> ar;

EIDRegistration :: {
	LISPGenMapRegister(EID $SERVEREID) 
	-> LISPRecordLocator(RLOCIPADDR $RLOCIP)
	-> UDPIPEncap($RLOCIP, 1234, $MSMRIP, 4342)
	-> Unqueue
	-> output

} -> [0]aq_etr[0] -> IPPrint -> q1;

/*
 * Décapsulation d’un paquet / Ingress Tunnel Router
 */
c[0] -> ITR :: {

	input
	-> CheckIPHeader(14)
	-> IPClassifier(udp port 4341,)
	-> Strip(14)
	-> LISPDecapsulation()
	-> SetUDPChecksum
	-> SetIPChecksum
	-> EtherEncap(0x0800, 1:1:1:1:1:1, 2:2:2:2:2:2)
	-> output


} -> q1;

q1 -> ToDevice(eth0);

