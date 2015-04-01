FromDump(map_request.pcap)
-> LISPGenMapReply()
-> Print()
-> UDPIPEncap(10.0.0.2, 4342, 10.0.0.3, 4342)
-> EnsureEther()
-> ToDump(map_reply.pcap);
