LISPGenMapRegister()
-> LISPRecordLocator(10.0.0.3, 192.168.0.10) /* RLOC: 10.0.0.3, EID: 192.168.0.10 */
-> Print()
-> UDPIPEncap(10.0.0.3, 1234, 10.0.0.2, 4342)
-> EnsureEther()
-> ToDump(map_registers.pcap);
