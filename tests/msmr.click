MSMR :: {
	AddressInfo(eth0 10.0.0.2 5c-f9-dd-4e-5f-81);

	FromDevice(enp2s0)
	-> cl :: Classifier(12/0800,		// IP packets
			12/0806 20/0002,	// ARP Replies
			12/0806 20/0001	// ARP Queries
			)			// tout le reste
	-> CheckIPHeader(14)
	-> IPClassifier(udp port 4342,)
	-> Strip(42)
	-> c :: LISPClassifier[1]
	-> LISPGenMapReply
	-> UDPIPEncap(enp2s0, 4342, 10.0.0.3, 1234) // changer 10.0.0.3 pour une annotation a positionner
	-> arpq :: ARPQuerier(eth0) // output 0 : packets IP, output 1 : requêtes ARP
	-> q :: Queue
	-> ToDevice(enp2s0);

	c[0] -> Queue -> LISPExtractEIDAndUpdateDB(IS_REPLY false) -> Discard; // le paquet est un Map Register
	c[2] -> Discard; // other

	cl[1] -> [1]arpq; // ARP Response
	cl[2] -> ar :: ARPResponder(eth0) -> q; // ARP Query

	arpq[1] -> q;
};