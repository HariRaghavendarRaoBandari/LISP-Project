require(lisp);

MSMR :: {
	FromDevice(enp2s0)
	-> CheckIPHeader(14)
	-> IPClassifier(udp port 4342,)
	-> Queue
	-> Strip(42)
	-> Unqueue
	-> c :: LISPClassifier[1]
	-> Queue
	-> LISPGenMapReply
	-> UDPIPEncap(10.0.0.2, 4342, 10.0.0.3, 1234) // changer 10.0.0.3 pour une annotation a positionner
	-> EnsureEther
	-> ToDevice(enp2s0);

	c[0] -> Queue -> LISPExtractEIDAndUpdateDB -> Discard; // le paquet est un Map Register
	c[2] -> Discard; // other
};
