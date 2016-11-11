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
  //cout << offer.type << ' ' << offer.price << ' ' << offer.shares << ' ' << offer.symbol << endl;
  int offerPos;
  for(offerPos = 0; offerPos < count && (strcmp(offer.symbol, stocks[offerPos].symbol) != 0); offerPos++)
  {
    //cout << "FOR " << offer.symbol << ' ' << stocks[i].symbol << endl;
  }
  if(strcmp(stocks[offerPos].symbol,"-------") == 0)
  {
    stocks[count++].set(divisor, offer.symbol);
    //cout << '>' << offer.symbol << " has been added!" <<  endl;
    stockCount++;
  }
  lastInserted = offerPos;


  if(offer.type == 'B') //buyer
  {

    int i;
    /*
    //find place to insert the buy offer
    for(b = 0; b < stocks[offerPos].countB && offer.price < stocks[offerPos].buyers[b].price; b++);
    cout << offer.price << ' ' << stocks[offerPos].buyers[b].price << endl;
    if(offer.price == stocks[offerPos].buyers[b].price) //handle ties in price
    {
      int i;
      for(i = b; i < stocks[offerPos].countB && offer.price == stocks[offerPos].buyers[i].price && offer.time > stocks[offerPos].buyers[i].time; i++);
      b = i;
    }

    //move everything up to make space
    for(int k = stocks[offerPos].countB - 1; k >= b; k--)
      stocks[offerPos].buyers[k + 1] = stocks[offerPos].buyers[k];
    stocks[offerPos].buyers[b] = offer;
    (stocks[offerPos].countB)++;
    */
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
    //find place to insert the buy offer

    //if we make it >= it would get rid of checking if the price was equal
    //since it would be sorting by time already since the older one is already
    //in the thing; the newer one would go after it.
    /*
    for(s = 0; s < stocks[offerPos].countS && offer.price > stocks[offerPos].sellers[s].price; s++);

    if(offer.price == stocks[offerPos].sellers[s].price) //handle ties in price
    {
      int i;
      for(i = s; i < stocks[offerPos].countB && offer.price == stocks[offerPos].sellers[i].price && offer.time > stocks[offerPos].sellers[i].time; i++);
      s = i;
    }

    //move everything up to make space
    for(int k = stocks[offerPos].countS - 1; k >= s; k--)
      stocks[offerPos].sellers[k + 1] = stocks[offerPos].sellers[k];
    stocks[offerPos].sellers[s] = offer;
    (stocks[offerPos].countS)++;
    */
    for(i = stocks[offerPos].countS; i > 0 && stocks[offerPos].sellers[i - 1].price < offer.price; i--)
        stocks[offerPos].sellers[i] = stocks[offerPos].sellers[i - 1];

    stocks[offerPos].sellers[i] = offer;
    (stocks[offerPos].countS)++;
    stocks[offerPos].lastSeller = i;
    stocks[offerPos].lastOffer = 'S';
  }
  offerC++;
} // newOffer()


bool Market::newTransaction(Transaction *transaction)
{
  if(stocks[lastInserted].countB > 0 && stocks[lastInserted].countS > 0) //transaction possible
  {
    if(stocks[lastInserted].lastOffer == 'B')
    {
      int bIndex = stocks[lastInserted].lastBuyer;
      if(bIndex != -1 && stocks[lastInserted].bidderTransaction(bIndex, *transaction))
        return true;
    }
    else
    {
      int sIndex = stocks[lastInserted].lastSeller;
      if(sIndex != -1 && stocks[lastInserted].sellerTransaction(sIndex, *transaction))
        return true;
    }
  }
  return false; // means no more transactions, and transaction will be ignored
} // newTransaction()

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
