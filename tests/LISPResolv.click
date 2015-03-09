InfiniteSource(LIMIT 1, LENGTH 32)
-> Print("1")
-> UDPIPEncap(127.0.0.1, 1234, 127.0.0.1, 5678)
-> LISPEncapsulation()
-> UDPIPEncap(127.0.1.1, 1234, 127.0.1.1, 5678)
-> SetIPAddress(192.168.0.1)
-> resolv :: LISPResolv
-> SetIPChecksum() // Sinon le checksum IP est mauvais
-> Print("output 0")
-> EnsureEther()
-> ToDevice(lo);

resolv[1]
-> Print("output 1")
-> Queue(1) // Necessaire
-> Discard;
