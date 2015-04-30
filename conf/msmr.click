require(lisp);

MSMR :: {
	define($IFACENAME enp2s0);
	AddressInfo($IFACENAME 10.0.0.2 5c:f9:dd:4e:5f:81);

	FromDevice($IFACENAME)
	-> cl :: Classifier(12/0800,		// IP packets
			12/0806 20/0002,	// ARP Replies
			12/0806 20/0001	// ARP Queries
			)			// tout le reste
	-> CheckIPHeader2(14)
	-> IPClassifier(udp port 4342,)
	-> GetIPAddress(ANNO 0, IP src)
	-> Strip(42)
	-> c :: LISPClassifier[1]
	-> LISPGenMapReply
	-> UDPIPEncap($IFACENAME, 4342, DST_ANNO, 1234)
	-> arpq :: ARPQuerier($IFACENAME) // output 0 : IP packets, output 1 : ARP Requests
	-> q :: Queue
	-> ToDevice($IFACENAME);

	c[0] -> Queue -> LISPExtractEIDAndUpdateDB(IS_REPLY false) -> Discard; // packet is a Map Register
	c[2] -> Discard; // other

	cl[1] -> [1]arpq; // ARP Response
	cl[2] -> ar :: ARPResponder($IFACENAME) -> q; // ARP Query

	arpq[1] -> q;
};
