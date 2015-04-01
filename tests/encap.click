InfiniteSource(\<111111111111111111111111111111111111111111111111111111111111>, LIMIT 1)
-> UDPIPEncap(127.0.0.1, 1234, 127.0.0.1, 5678)
-> Print("avant")
-> LISPEncapsulation
-> Print("Après")
-> UDPIPEncap(127.0.0.1, 1234, 127.0.0.1, 5678)
-> EnsureEther()
-> ToDevice(lo);