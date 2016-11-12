CFLAGS = -ansi -Wall -g 
market.out : market.o MarketDriver.o stock.o QuadraticProbing.o MaxBinaryHeap.o
	g++ $(CFLAGS) -o market.out market.o MarketDriver.o stock.o QuadraticProbing.o MaxBinaryHeap.o

market.o : market.cpp market.h MarketDriver.h
	g++ $(CFLAGS) -c market.cpp

MarketDriver.o : MarketDriver.cpp MarketDriver.h market.h CPUTimer.h 
	g++ $(CFLAGS) -c MarketDriver.cpp

stock.o : stock.cpp stock.h MarketDriver.h market.h
	g++ $(CFLAGS) -c stock.cpp

QuadraticProbing.o : QuadraticProbing.h QuadraticProbing.cpp stock.h
	g++ $(CFLAGS) -c QuadraticProbing.cpp

MaxBinaryHeap.o : MaxBinaryHeap.h MaxBinaryHeap.cpp MarketDriver.h
	g++ $(CFLAGS) -c MaxBinaryHeap.cpp

clean : 
	rm -f market.out market.o MarketDriver.o stock.o QuadraticProbing.o MaxBinaryHeap.o
