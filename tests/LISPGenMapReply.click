FromDump(req.cap)
-> LISPGenMapReply()
-> Print()
-> UDPIPEncap(127.0.0.1, 1234, 192.168.1.57, 5678)
-> EnsureEther()
-> ToDump(rep.pcap);
