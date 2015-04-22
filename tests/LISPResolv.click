failedResolvQueue :: Queue(10);

tee :: Tee(2) -> LISPPrintDB(RLOC 10.0.0.4) -> Queue(10) -> RequestEIDMapping :: {
	input
	-> LISPGenMapRequest(10.0.0.3)
	-> UDPIPEncap(10.0.0.3, 1234, 10.0.0.2, 4342)
	-> EnsureEther()
	-> ToDump(resolv_map_requested.pcap);
};

InfiniteSource(LIMIT 1)
-> UDPIPEncap(192.168.0.10, 1234, 192.168.1.20, 4444)
-> SetIPAddress(192.168.0.10)
-> LISPEncapsulation(SRC 192.168.0.10, SPORT 1234)
-> Paint(COLOR 0, ANNO 5) /* Makes sure that the failed resolv counter is 0 at user annotation offset 5 */
-> resolv :: LISPResolv
-> SetIPChecksum
-> EnsureEther
-> ToDump(resolved.pcap);

resolv[1] -> tee;
tee[1] -> failedResolvQueue -> TimedUnqueue(INTERVAL 5) -> resolv;
