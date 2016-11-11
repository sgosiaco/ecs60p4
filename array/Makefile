marketAr.out : marketAr.o MarketDriver.o  
	g++ -ansi -Wall -g -o marketAr.out marketAr.o MarketDriver.o  

marketAr.o : marketAr.cpp marketAr.h MarketDriver.h 
	g++ -ansi -Wall -g -c marketAr.cpp

MarketDriver.o : MarketDriver.cpp MarketDriver.h marketAr.h CPUTimer.h 
	g++ -ansi -Wall -g -c MarketDriver.cpp

clean : 
	rm -f marketAr.out marketAr.o  MarketDriver.o   
