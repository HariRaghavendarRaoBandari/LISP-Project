InfiniteSource(LIMIT 1, LENGTH 32)
->LISPGenMapRequestOuter()
-> Print()
-> UDPIPEncap(127.0.0.1, 1234, 127.0.0.1, 5678)
-> EnsureEther()
-> ToDevice(lo);
