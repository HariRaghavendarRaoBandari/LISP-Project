LISPGenMapRegister()
-> LISPRecordLocator(192.168.0.2, 192.168.100.200) /* RLOC: 192.168.0.2, EID: 192.168.100.200 */
-> Print()
-> UDPIPEncap(127.0.0.1, 1234, 127.0.0.1, 5678)
-> EnsureEther()
-> ToDevice(lo);
