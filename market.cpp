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
  offerCount = 0;
  stocks = new Stock[numStocks];
  transactions = new Transaction[offerCount];
} // Market()

void Market::newOffer(const Offer &offer)
{
  offerCount++;
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
        if(stocks[offerPos].buyers[b].price == stocks[offerPos].sellers[0].price) //equal price
        {
          if(stocks[offerPos].buyers[b].shares >= stocks[offerPos].sellers[0].shares) //buyer has more or equal shares than seller
          {
            //remove seller at pos 0 and subtract share amt from buyers
            Transaction *temp = new Transaction(offerCount, stocks[offerPos].buyers[b].ID, stocks[offerPos].sellers[0].ID, stocks[offerPos].buyers[b].price, stocks[offerPos].sellers[0].shares, stocks[offerPos].buyers[b].symbol);
            transactions[transCount++] = *temp;
            stocks[offerPos].buyers[b].shares -= stocks[offerPos].sellers[0].shares;
            for(int i = 0; i < stocks[offerPos].countS; i++)
              stocks[offerPos].sellers[i] = stocks[offerPos].sellers[i + 1];
            stocks[offerPos].countS--;
            transCount++;
          }
          else //seller has more shares than buyer
          {
            Transaction *temp = new Transaction(offerCount, stocks[offerPos].buyers[b].ID, stocks[offerPos].sellers[0].ID, stocks[offerPos].buyers[b].price, stocks[offerPos].buyers[b].shares, stocks[offerPos].buyers[b].symbol);
            transactions[transCount++] = *temp;
            stocks[offerPos].sellers[0].shares -= stocks[offerPos].buyers[b].shares;
            transCount++;
          }
        }
        else
        {
          if(stocks[offerPos].buyers[b].price > stocks[offerPos].sellers[0].price) //greater than
          {
            int p;
            for(p = b; p < stocks[offerPos].countB && stocks[offerPos].buyers[p].price > stocks[offerPos].sellers[0].price; p++);
            if(stocks[offerPos].buyers[b].shares >= stocks[offerPos].sellers[0].shares) //buyer has more or equal shares than seller
            {
              //remove seller at pos 0 and subtract share amt from buyers
              Transaction *temp = new Transaction(offerCount, stocks[offerPos].buyers[b].ID, stocks[offerPos].sellers[0].ID, stocks[offerPos].buyers[p].price, stocks[offerPos].sellers[0].shares, stocks[offerPos].buyers[b].symbol);
              transactions[transCount++] = *temp;
              stocks[offerPos].buyers[b].shares -= stocks[offerPos].sellers[0].shares;
              for(int i = 0; i < stocks[offerPos].countS; i++)
                stocks[offerPos].sellers[i] = stocks[offerPos].sellers[i + 1];
              stocks[offerPos].countS--;
              transCount++;
            }
            else //seller has more shares than buyer
            {
              Transaction *temp = new Transaction(offerCount, stocks[offerPos].buyers[b].ID, stocks[offerPos].sellers[0].ID, stocks[offerPos].buyers[p].price, stocks[offerPos].buyers[b].shares, stocks[offerPos].buyers[b].symbol);
              transactions[transCount++] = *temp;
              stocks[offerPos].sellers[0].shares -= stocks[offerPos].buyers[b].shares;
              transCount++;
            }
          }
        }
      }
    }
    if(offer.shares != 0) //if after transaction or if b != 0
    {
      //move everything up to make space
      for(int k = stocks[offerPos].countB - 1; k >= b; k--)
        stocks[offerPos].buyers[k + 1] = stocks[offerPos].buyers[k];
      stocks[offerPos].buyers[b] = offer;
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
} // newOffer()


bool Market::newTransaction(Transaction *transaction)
{
  if(transCount > 0)
  {
    cout << transCount << endl;
    cout << transactions[0] << endl;
    //transaction = &transactions[0];
    return true;
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
