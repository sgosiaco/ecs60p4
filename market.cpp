// Author: Sean Davis
#include <iostream>
#include <fstream>
#include <cstring>
#include "market.h"
#include "MarketDriver.h"

Stock::Stock()
{
  countB = 0;
  countS = 0;
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
}

Market::Market(int numStocks, int offerCount, int IDs)
{
  count = 0;
  transCount = 0;
  offerC = 0;
  stocks = new Stock[numStocks];
  transactions = new Transaction[offerCount];
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
    stocks[count++].set(20, offer.symbol);
    cout << '>' << offer.symbol << " has been added!" <<  endl;
  }
  Offer offers = offer;


  if(offer.type == 'B') //buyer
  {
    int b;
    //find place to insert the buy offer
    for(b = 0; b < stocks[offerPos].countB && offer.price <= stocks[offerPos].buyers[b].price; b++);
    if(offer.price == stocks[offerPos].buyers[b].price) //handle ties in price
    {
      int i;
      for(i = 0; i < stocks[i].countB && offer.time > stocks[i].buyers[b].time; i++);
      b = i;
    }
    if(b == 0)
    {
      //check for sellers to make a transaction
      if(stocks[offerPos].countS > 0)
      {
        if(offers.price == stocks[offerPos].sellers[0].price) //equal price
        {
          if(offers.shares >= stocks[offerPos].sellers[0].shares) //buyer has more or equal shares than seller
          {
            //remove seller at pos 0 and subtract share amt from buyers
            cout << ">EQUAL and SHARES > " << offerC << endl;
            Transaction *temp = new Transaction(offerC, offers.ID, stocks[offerPos].sellers[0].ID, offers.price, stocks[offerPos].sellers[0].shares, offers.symbol);
            transactions[transCount++] = *temp;
            offers.shares -= stocks[offerPos].sellers[0].shares;
            for(int i = 0; i < stocks[offerPos].countS; i++)
              stocks[offerPos].sellers[i] = stocks[offerPos].sellers[i + 1];
            stocks[offerPos].countS--;
            transCount++;
          }
          else //seller has more shares than buyer
          {
            cout << ">EQUAL and SHARES <" << offerC << endl;
            Transaction *temp = new Transaction(offerC, offers.ID, stocks[offerPos].sellers[0].ID, offers.price, offers.shares, offers.symbol);
            transactions[transCount++] = *temp;
            stocks[offerPos].sellers[0].shares -= offers.shares;
            transCount++;
          }
        }
        else
        {
          if(offers.price > stocks[offerPos].sellers[0].price) //greater than
          {
            int p;
            double price = 0;
            for(p = b; p < stocks[offerPos].countB && stocks[offerPos].buyers[b].price > stocks[offerPos].sellers[0].price; p++);

            if((stocks[offerPos].buyers[b].price > stocks[offerPos].sellers[0].price) && (stocks[offerPos].buyers[b].price < offers.price))
            {
              price = stocks[offerPos].buyers[b].price;
              cout << "tenmp \n";
            }

            else
              price = offers.price;

            if(offers.shares >= stocks[offerPos].sellers[0].shares) //buyer has more or equal shares than seller
            {
              //remove seller at pos 0 and subtract share amt from buyers
              cout << ">GREATER and SHARES >" << offerC << endl;
              Transaction *temp = new Transaction(offerC, offers.ID, stocks[offerPos].sellers[0].ID, price, stocks[offerPos].sellers[0].shares, offers.symbol);
              cout << *temp;
              transactions[transCount++] = *temp;
              offers.shares -= stocks[offerPos].sellers[0].shares;
              for(int i = 0; i < stocks[offerPos].countS; i++)
                stocks[offerPos].sellers[i] = stocks[offerPos].sellers[i + 1];
              stocks[offerPos].countS--;
              transCount++;
            }
            else //seller has more shares than buyer
            {
              cout << ">GREATER and SHARES <" << offerC << endl;
              Transaction *temp = new Transaction(offerC, offers.ID, stocks[offerPos].sellers[0].ID, price, offers.shares, offers.symbol);
              transactions[transCount++] = *temp;
              stocks[offerPos].sellers[0].shares -= offers.shares;
              offers.shares = 0;
              transCount++;
            }
          }
        }
      }
    }
    if(offers.shares != 0) //if after transaction or if b != 0
    {
      //move everything up to make space
      for(int k = stocks[offerPos].countB - 1; k >= b; k--)
        stocks[offerPos].buyers[k + 1] = stocks[offerPos].buyers[k];
      stocks[offerPos].buyers[b] = offers;
      (stocks[offerPos].countB)++;
    }
  }
  else //seller
  {
    int s;
    //find place to insert the buy offer
    for(s = 0; s < stocks[offerPos].countS && offer.price <= stocks[offerPos].sellers[s].price; s++);
    if(offer.price == stocks[offerPos].sellers[s].price) //handle ties in price
    {
      int i;
      for(i = 0; i < stocks[i].countB && offer.time > stocks[i].sellers[s].time; i++);
      s = i;
    }
    if(s == 0)
    {
      //check for buyers to make a transaction
    }
    if(offer.shares != 0) //if after transaction or if b != 0
    {
      //move everything up to make space
      for(int k = stocks[offerPos].countS - 1; k >= s; k--)
        stocks[offerPos].sellers[k + 1] = stocks[offerPos].sellers[k];
      stocks[offerPos].sellers[s] = offer;
      (stocks[offerPos].countS)++;
    }
  }
  offerC++;
} // newOffer()


bool Market::newTransaction(Transaction *transaction)
{
  if(transCount > 0)
  {
    /*
    for(int i = 0; i < transCount; i++)
      cout << "Transaction #" << i+1 << ' ' << transactions[i];
      */
    //transaction = &transactions[0];
    return false;
  }
  return false; // means no more transactions, and transaction will be ignored
} // newTransaction()

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
