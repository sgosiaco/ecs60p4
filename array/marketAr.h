// Authors: Sean Davis
#ifndef marketARH
#define marketARH

#include <cstring>
#include "vector.h"
#include "MarketDriver.h"

using namespace std;
class Stock;

class Market
{
  int stockCount;
  int offerCount;
  int IDCount;
  int currStock;
  int recentStock;
  int lastBidder;
  int lastSeller;
  Offer recentOffer;
  Stock *arrayStocks;

public:
  Market(int numStocks, int offerCount, int IDs);
  ~Market();
  void addOffer(const Offer& offer, int index);
  int getCurrStock();
  int getStockCount() { return stockCount; }
  void printStocks();
  void newOffer(const Offer &offer);
  bool newTransaction(Transaction *transaction);

}; // class Market

class Stock
{
private:
  char name[7];
  int currBidders;
  int currSellers;

  vector<Offer> bidders;
  vector<Offer> sellers;

  //Offer bidders[500];
  //Offer sellers[500];


public:
  //Stock(char *n, QueueLL<Offer> buyer, QueueLL<Offer> seller);
  Stock();
  ~Stock();
  void addToBidderAr(const Offer &offer);
  void addToSellerAr(const Offer &offer);
  bool bidderTransaction(int bIndex, Transaction& t);
  bool sellerTransaction(int sIndex, Transaction& t);
  int findBidder(Offer &offer);
  int findSeller(Offer &offer);
  void printBidders();
  void printSellers();
  void shiftBidderArray(int index);
  void shiftSellerArray(int index);
  friend class Market;
}; // class Stock

#endif
