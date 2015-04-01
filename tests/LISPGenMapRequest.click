InfiniteSource(LIMIT 1, LENGTH 28, STOP 1)
-> SetIPAddress(192.168.1.20)
-> LISPGenMapRequest(10.0.0.3)
-> Print()
-> UDPIPEncap(10.0.0.3, 1234, 10.0.0.2, 4342)
-> EnsureEther()
-> ToDump(map_request.pcap);
