market.out : market.o MarketDriver.o stock.o QuadraticProbing.o MaxBinaryHeap.o
	g++ -ansi -Wall -g -o market.out market.o MarketDriver.o stock.o QuadraticProbing.o MaxBinaryHeap.o

market.o : market.cpp market.h MarketDriver.h
	g++ -ansi -Wall -g -c market.cpp

MarketDriver.o : MarketDriver.cpp MarketDriver.h market.h CPUTimer.h 
	g++ -ansi -Wall -g -c MarketDriver.cpp

stock.o : stock.cpp stock.h MarketDriver.h market.h
	g++ -ansi -Wall -g -c stock.cpp

QuadraticProbing.o : QuadraticProbing.h QuadraticProbing.cpp stock.h
	g++ -ansi -Wall -g -c QuadraticProbing.cpp

MaxBinaryHeap.o : MaxBinaryHeap.h MaxBinaryHeap.cpp MarketDriver.h
	g++ -ansi -Wall -g -c MaxBinaryHeap.cpp

clean : 
	rm -f market.out market.o MarketDriver.o stock.o QuadraticProbing.o MaxBinaryHeap.o
