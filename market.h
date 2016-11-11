// Authors: Sean Davis
#ifndef marketH
#define marketH

#include "MarketDriver.h"

class Stock
{
public:
  Offer *buyers;
  Offer *sellers;
  int countB;
  int countS;
  int lastSeller;
  int lastBuyer;
  char lastOffer;
  Offer recentOffer;
  char symbol[7];
  Stock();
  Stock(int size, const char *sym);
  void set(int size, const char *sym);
  bool bidderTransaction(int bIndex, Transaction& t);
  bool sellerTransaction(int sIndex, Transaction& t);
  void shiftBidderArray(int bIndex);
  void shiftSellerArray(int sIndex);
  int findBidder(Offer& offer);
  int findSeller(Offer& offer);
};

class Market
{
public:
  Market(int numStocks, int offerCount, int IDs);
  void newOffer(const Offer &offer);
  bool newTransaction(Transaction *transaction);
  void transaction(int t, int buyer, int seller, double pr, int shar, const char *sym, Transaction *transaction);
  void print();
  int count;
  int transCount;
  int offerC;
  int stockCount;
  int lastInserted;
  int divisor;
  Stock *stocks;
}; // class Market

#endif
