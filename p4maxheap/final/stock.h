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
  int currSellers;
  bool copied;
  MaxBinaryHeap bidders;
  Offer sellers[2000];
 
public:
  Stock();
  ~Stock();
  void addToStock(const Offer &offer);
  bool bidderTransaction(Transaction& t);
  bool sellerTransaction(int sIndex, Transaction& t);
  int findSeller(Offer &offer);
  char* getName() { return name; }
  void shiftSellerArray(int index);
  void setName(const char *n);
}; // class Stock


#endif
