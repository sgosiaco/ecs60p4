// Authors: Sean Davis
#ifndef marketH
#define marketH

#include "MarketDriver.h"

class Market
{
public:
  Market(int numStocks, int offerCount, int IDs);
  void newOffer(const Offer &offer);
  bool newTransaction(Transaction *transaction);
}; // class Market

#endif
