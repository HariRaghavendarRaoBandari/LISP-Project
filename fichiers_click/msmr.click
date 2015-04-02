MSMR :: {
	FromDevice(eth0)
	-> CheckIPHeader(14)
	-> IPClassifier(udp port 4342,)
	-> Queue(XXX)
	-> Strip(42)
	-> c :: LISPClassifier
	-> LISPGenMapReply()
	-> LISPRecordLocator(RLOCIPADDR <ip>)
	-> UDPIPEncap(eth0.Addr, 4342, DST_ANNO, PORT)
	-> EnsureEther()
	-> ToDevice(eth0);

	c[1] -> LISPExtractEIDAndUpdateDB -> Discard; // le paquet est un Map Register
	c[2] -> Discard; // other
};