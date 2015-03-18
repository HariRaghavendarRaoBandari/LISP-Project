InfiniteSource(LIMIT 1, LENGTH 28)
-> SetIPAddress(4.3.2.1) /* Sets the IPv4 address in annotations */
-> LISPGenMapRequest(5.6.7.8)
-> Print()
-> UDPIPEncap(127.0.0.1, 1234, 127.0.0.1, 5678)
-> EnsureEther()
-> ToDevice(lo);
