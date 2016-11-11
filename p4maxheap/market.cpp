// Author: Sean Davis
#include "market.h"
#include "stock.h"
using namespace std;



Market::Market(int numOfStocks, int numOfOffers, int IDs): 
stockCount(numOfStocks), offerCount(numOfOffers), IDCount(IDs), currStocks(0),
tableStocks(numOfStocks)
{
} // Market()



Market::~Market()
{
} //destructor




void Market::newOffer(const Offer &offer)
{
  Stock incomingStock; 
  incomingStock.setName(offer.symbol);
  
  Stock *ptr = tableStocks.find(&incomingStock);

  if(ptr)
    ptr->addToStock(offer);
  else
  {
    ptr = new Stock;
    ptr->addToStock(offer);
    tableStocks.insert(ptr);
    currStocks++;
  } // a new stock

  recentOffer = offer;
} // newOffer()


bool Market::newTransaction(Transaction *transaction)
{
  Stock *stockPtr, temp;
  temp.setName(recentOffer.symbol);
  stockPtr = tableStocks.find(&temp);

  if(recentOffer.type == 'B')
  {
    if(stockPtr->bidderTransaction(*transaction))
      return true;
  }
  if(recentOffer.type == 'S')
  {
    int sIndex = stockPtr->findSeller(recentOffer);
    //cout << "sIndex = " << sIndex << endl << endl;
    if(sIndex != -1 && stockPtr->sellerTransaction(sIndex, *transaction))
      return true;
  }
  return false;
} // newTransaction()

