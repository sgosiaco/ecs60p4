// Authors: Sean Davis
#ifndef marketH
#define marketH

#include <cstring>
#include "MarketDriver.h"
#include "vector.h"
#include "stock.h"
#include "QuadraticProbing.h"

using namespace std;

class Market;

class Market
{
  int stockCount;
  int offerCount;
  int IDCount;
  int currStocks;
  Offer recentOffer; 
  QuadraticHashTable tableStocks;

public:
  Market(int numStocks, int offerCount, int IDs);
  ~Market();
  void newOffer(const Offer &offer);
  bool newTransaction(Transaction *transaction);
}; // class Market


#endif
