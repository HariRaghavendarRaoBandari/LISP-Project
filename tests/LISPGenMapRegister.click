LISPGenMapRegister(EID 127.0.0.10, EID 127.0.0.11, EID 127.0.0.12)
-> LISPRecordLocator(RLOCIPADDR 10.0.0.3)
-> Print()
-> UDPIPEncap(10.0.0.3, 1234, 10.0.0.2, 4342)
-> EnsureEther()
-> ToDump(map_registers.pcap);
