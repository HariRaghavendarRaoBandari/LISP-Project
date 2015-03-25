LISPGenMapRegister(EID 127.0.0.10, EID 127.0.0.11, EID 127.0.0.12)
-> LISPRecordLocator(RLOCIPADDR 127.0.0.1)
-> Print()
-> UDPIPEncap(127.0.0.1, 1234, 127.0.0.111, 4342) /* @ MSMR = 127.0.0.111,*/
-> EnsureEther()
-> ToDevice(lo);
