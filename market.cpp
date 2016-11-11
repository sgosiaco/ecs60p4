// Author: Sean Davis
#include <iostream>
#include <cstring>
#include "market.h"
#include "MarketDriver.h"

Stock::Stock()
{
  countB = 0;
  countS = 0;
  lastSeller = 0;
  lastBuyer = 0;
  lastOffer = '-';
  strcpy(symbol, "-------");
}

Stock::Stock(int size, const char *sym)
{
  strcpy(symbol, sym);
  buyers = new Offer[size];
  sellers = new Offer[size];
  countB = 0;
  countS = 0;
}

void Stock::set(int size, const char *sym)
{
  strcpy(symbol, sym);
  buyers = new Offer[size];
  sellers = new Offer[size];
  countB = 0;
  countS = 0;
  lastSeller = 0;
  lastBuyer = 0;
  lastOffer = '-';
}

Market::Market(int numStocks, int offerCount, int IDs)
{
  count = 0;
  transCount = 0;
  offerC = 0;
  stockCount = 0;
  lastInserted = 0;
  divisor = offerCount/numStocks;
  stocks = new Stock[numStocks];
} // Market()

void Market::newOffer(const Offer &offer)
{
  int i;
  if(!offerC)
  {
    addOffer(offer, 0);
    offerC++;
  } // if currStock = 0
  else
  {
    if(strcmp(offer.symbol, stocks[lastInserted].symbol) == 0)
    {
      addOffer(offer, lastInserted);
      if(lastInserted == offerC)
        offerC++;
    }
    else
    {
      for(i = 0; i < count && (strcmp(offer.symbol, stocks[i].symbol) != 0); i++);
      addOffer(offer, i);
      if(i == offerC)
        offerC++;
    }
  } // currStock > 0
  /*
  //cout << offer.type << ' ' << offer.price << ' ' << offer.shares << ' ' << offer.symbol << endl;
  int offerPos;
  for(offerPos = 0; offerPos < count && (strcmp(offer.symbol, stocks[offerPos].symbol) != 0); offerPos++);

  if(strcmp(stocks[offerPos].symbol,"-------") == 0)
  {
    stocks[count++].set(divisor, offer.symbol);
    //cout << '>' << offer.symbol << " has been added!" <<  endl;
    stockCount++;
  }
  lastInserted = offerPos;
  */
} // newOffer()

void Market::addOffer(const Offer& offer, int offerPos)
{
  if(offer.type == 'B') //buyer
  {

    int i;

    for(i = stocks[offerPos].countB; i > 0 && stocks[offerPos].buyers[i - 1].price < offer.price; i--)
      stocks[offerPos].buyers[i] = stocks[offerPos].buyers[i - 1];

    stocks[offerPos].buyers[i] = offer;
    (stocks[offerPos].countB)++;

    stocks[offerPos].lastBuyer = i;
    stocks[offerPos].lastOffer = 'B';
  }
  else //seller
  {
    int i;

    for(i = stocks[offerPos].countS; i > 0 && stocks[offerPos].sellers[i - 1].price < offer.price; i--)
        stocks[offerPos].sellers[i] = stocks[offerPos].sellers[i - 1];

    stocks[offerPos].sellers[i] = offer;
    (stocks[offerPos].countS)++;

    stocks[offerPos].lastSeller = i;
    stocks[offerPos].lastOffer = 'S';
  }
  recentOffer = offer;
  offerC++;
}

bool Market::newTransaction(Transaction *transaction)
{
    if(stocks[lastInserted].lastOffer == 'B')
    {
      int bIndex = stocks[lastInserted].findBidder(recentOffer);
      if(bIndex != -1 && stocks[lastInserted].bidderTransaction(bIndex, *transaction))
        return true;
    }
    else
    {
      if(stocks[lastInserted].lastOffer == 'S')
      {
        int sIndex = stocks[lastInserted].findSeller(recentOffer);
        if(sIndex != -1 && stocks[lastInserted].sellerTransaction(sIndex, *transaction))
          return true;
      }
    }
  return false; // means no more transactions, and transaction will be ignored
} // newTransaction()

int Stock::findBidder(Offer& offer)
{
  for(int i = 0; i < countB; i++)
    if(buyers[i].time == offer.time)
      return i;
  return -1;
} //find indiviual bidder in the bidder array


int Stock::findSeller(Offer& offer)
{
  for(int i = 0; i < countB; i++)
    if(sellers[i].time == offer.time)
      return i;
  return -1;
} //find indiviual bidder in the bidder array

bool Stock::bidderTransaction(int bIndex, Transaction& t)
{
  //pass in the index where the recent offer is
  for(int i = 0; i < countS; i++)
  {
    if(sellers[i].price <= buyers[bIndex].price)
    {
      if(sellers[i].time > buyers[bIndex].time)
        t.time = sellers[i].time;
      else
        t.time = buyers[bIndex].time;

      t.buyerID = buyers[bIndex].ID;
      t.sellerID = sellers[i].ID;
      t.price = sellers[i].price;
      strcpy(t.symbol, buyers[bIndex].symbol);

      if(sellers[i].shares == buyers[bIndex].shares)
      {
        //cout << "SELLERS = BUYERS, buyers[bIndex].shares = " << buyers[bIndex].shares << " and name is " << buyers[bIndex].symbol << endl;
        t.shares = buyers[bIndex].shares;
        shiftSellerArray(i);
        shiftBidderArray(bIndex);
        return true;
      } //update both arrays
      if(sellers[i].shares > buyers[bIndex].shares)
      {
        t.shares = buyers[bIndex].shares;
        sellers[i].shares -= buyers[bIndex].shares;
        shiftBidderArray(bIndex);
        return true;
      } // upate the stock's bidder list
      else
      {
        t.shares = sellers[i].shares;
        buyers[bIndex].shares -= sellers[i].shares;
        //cout << "shares left = " << buyers[bIndex].shares << endl;
        shiftSellerArray(i);
        return true;
      } // upate the stock's seller list
    }
  }
  return false;
} // bidderTransaction() goes through list of sellers for a stock

bool Stock::sellerTransaction(int sIndex, Transaction& t)
{
  for(int bIndex = 0; bIndex < countB; bIndex++)
  {
      if(buyers[bIndex].price >= sellers[sIndex].price)
      {
        if(buyers[bIndex].time > sellers[sIndex].time)
          t.time = buyers[bIndex].time;
        else
          t.time = sellers[sIndex].time;

        t.buyerID = buyers[bIndex].ID;
        t.sellerID = sellers[sIndex].ID;
        strcpy(t.symbol, sellers[sIndex].symbol);

        if(bIndex < countB - 1 && buyers[bIndex + 1].price >= sellers[sIndex].price)
        {
          //sellers[sIndex].price = buyers[bIndex + 1].price;
          t.price = buyers[bIndex + 1].price;
        } // Felsman's Rule
        else
           t.price = sellers[sIndex].price;

        if(buyers[bIndex].shares == sellers[sIndex].shares)
        {
          t.shares = sellers[sIndex].shares;
          shiftSellerArray(sIndex);
          shiftBidderArray(bIndex);
          return true;
        } //update both arrays
        if(buyers[bIndex].shares > sellers[sIndex].shares)
        {
          t.shares = sellers[sIndex].shares;
          buyers[bIndex].shares -= sellers[sIndex].shares;
          shiftSellerArray(sIndex);
          return true;
        } // upate the stock's bidder list
        else
        {
          t.shares = buyers[bIndex].shares;
          sellers[sIndex].shares -= buyers[bIndex].shares;
          shiftBidderArray(bIndex);
          return true;
        } // upate the stock's seller list
      }
  }
  return false;
} // sellerTransaction()

inline void Stock::shiftBidderArray(int bIndex)
{
  for(int i = bIndex; i < countB - 1; i++)
    buyers[i] = buyers[i + 1];

  countB--;
} //shiftBidderArray()


inline void Stock::shiftSellerArray(int sIndex)
{
  for(int i = sIndex; i < countS - 1; i++)
    sellers[i] = sellers[i + 1];

  countS--;
} //shiftSellerArray()

void Market::transaction(int t, int buyer, int seller, double pr, int shar, const char *sym, Transaction *transaction)
{
  transaction->time = t;
  transaction->buyerID = buyer;
  transaction->sellerID = seller;
  transaction->price = pr;
  transaction->shares = shar;
  strcpy(transaction->symbol, sym);
}

void Market::print()
{
  for(int i = 0; i < count; i++)
  {
    for(int k = 0; k < stocks[i].countB; k++)
      cout << "Buyer:" << i << ' '<<  stocks[i].buyers[k].price << endl;
    for(int k = 0; k < stocks[i].countS; k++)
      cout << "Seller:" << i << ' '<<  stocks[i].sellers[k].price << endl;
  }
}
