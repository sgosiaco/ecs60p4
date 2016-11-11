#ifndef STOCK_H
#define STOCK_H

#include <iostream>
#include <cstring>
#include "MarketDriver.h"
#include "MaxBinaryHeap.h"

using namespace std;

class Stock
{
private:
  char name[7];
  int currBidders;
  int currSellers;
  int lastSellerInserted;

  MaxBinaryHeap bidders;
  Offer sellers[500];

public:
  Stock();
  ~Stock();
  void addToStock(const Offer &offer);
  bool bidderTransaction(Transaction& t);
  bool sellerTransaction(int sIndex, Transaction& t);
  int findSeller(Offer &offer);
  char* getName();
  int getCurrBidders() { return currBidders; }
  void printSellers();
  void shiftSellerArray(int index);
  void setName(const char *n);
  //bool operator!=(const Stock *rhs);
}; // class Stock


#endif
