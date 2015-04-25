require(lisp);

InfiniteSource
-> TimedUnqueue(INTERVAL 5)
-> LISPPrintDB(RLOC 10.0.0.3)
-> Discard;
