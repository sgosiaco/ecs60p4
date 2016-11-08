// Author: Sean Davis
#include <iostream>
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
  stockCount = 0;
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
    //cout << '>' << offer.symbol << " has been added!" <<  endl;
    stockCount++;
  }


  if(offer.type == 'B') //buyer
  {
    int b;
    //find place to insert the buy offer
    for(b = 0; b < stocks[offerPos].countB && offer.price <= stocks[offerPos].buyers[b].price; b++);
    if(offer.price == stocks[offerPos].buyers[b].price) //handle ties in price
    {
      int i;
      for(i = b; i < stocks[offerPos].countB && offer.time > stocks[offerPos].buyers[i].time; i++);
        b = i;
    }
    //move everything up to make space
    for(int k = stocks[offerPos].countB - 1; k >= b; k--)
      stocks[offerPos].buyers[k + 1] = stocks[offerPos].buyers[k];
    stocks[offerPos].buyers[b] = offer;
    (stocks[offerPos].countB)++;
  }
  else //seller
  {
    int s;
    //find place to insert the buy offer

    //if we make it >= it would get rid of checking if the price was equal
    //since it would be sorting by time already since the older one is already
    //in the thing; the newer one would go after it.
    for(s = 0; s < stocks[offerPos].countS && offer.price > stocks[offerPos].sellers[s].price; s++);
    if(offer.price == stocks[offerPos].sellers[s].price) //handle ties in price
    {
      int i;
      for(i = s; i < stocks[offerPos].countB && offer.time > stocks[offerPos].sellers[i].time; i++);
        s = i;
    }
    //move everything up to make space
    for(int k = stocks[offerPos].countS - 1; k >= s; k--)
      stocks[offerPos].sellers[k + 1] = stocks[offerPos].sellers[k];
    stocks[offerPos].sellers[s] = offer;
    (stocks[offerPos].countS)++;
  }
  offerC++;
} // newOffer()


bool Market::newTransaction(Transaction *transaction)
{
  for(int i = 0; i < stockCount; i++)
  {

    if(stocks[i].countB > 0 && stocks[i].countS > 0) //transaction possible
    {
      if(stocks[i].buyers[0].price > stocks[i].sellers[0].price) //transaction can be made
      {
        if(stocks[i].buyers[0].shares >= stocks[i].sellers[0].shares) //buyer wants >= shares
        {
          //iterate through buyers to find lowest "highest" price
          int k;
          for(k = 0; k < stocks[i].countB && stocks[i].buyers[k].price > stocks[i].sellers[0].price; k++)
          {
            //cout << stocks[i].buyers[k].price << ' ' << stocks[i].sellers[0].price << endl;
          }
          //if lowest "highest" price is the same
          if(stocks[i].buyers[0].price == stocks[i].buyers[k - 1].price)
          {
            //use seller price
            //make transaction with seller price and sellers share amt and symbol.
            ////transaction = new Transaction(offerC - 1, stocks[i].buyers[0].ID, stocks[i].sellers[0].ID, stocks[i].sellers[0].price, stocks[i].sellers[0].shares, stocks[i].sellers[0].symbol);
            transaction->time = offerC - 1;
            transaction->buyerID = stocks[i].buyers[0].ID;
            transaction->sellerID = stocks[i].sellers[0].ID;
            transaction->price = stocks[i].sellers[0].price;
            transaction->shares = stocks[i].sellers[0].shares;
            strcpy(transaction->symbol, stocks[i].sellers[0].symbol);
            //cout << *transaction;
          }
          else
          {
            if(stocks[i].buyers[k-1].price >= stocks[i].sellers[0].price)
            {
              //make transaction with price found above and sellers share amt and symbol.
              //transaction = new Transaction(offerC - 1, stocks[i].buyers[0].ID, stocks[i].sellers[0].ID, stocks[i].buyers[k - 1].price, stocks[i].sellers[0].shares, stocks[i].sellers[0].symbol);
              transaction->time = offerC - 1;
              transaction->buyerID = stocks[i].buyers[0].ID;
              transaction->sellerID = stocks[i].sellers[0].ID;
              transaction->price = stocks[i].buyers[k - 1].price;
              transaction->shares = stocks[i].sellers[0].shares;
              strcpy(transaction->symbol, stocks[i].sellers[0].symbol);
              //cout << *transaction;
            }
          }

          //adjust buyer and seller objects
          if(stocks[i].buyers[0].shares == stocks[i].sellers[0].shares)
          {
            //move the buyers up by one if shares are equal
            for(int j = 0; j < stocks[i].countB; j++)
              stocks[i].buyers[j] = stocks[i].buyers[j + 1];
            (stocks[i].countB)--;
          }
          else
          {
            //subtract share amount
            stocks[i].buyers[0].shares -= stocks[i].sellers[0].shares;
          }

          //move everything in the sellers array up one after the transaction is made
          for(int j = 0; j < stocks[i].countS; j++)
            stocks[i].sellers[j] = stocks[i].sellers[j + 1];
          (stocks[i].countS)--;
          return true;
        }
        else //buyer wants < shares
        {
          //move everything in the sellers array up one after the transaction is made
          //
          //iterate through buyers to find lowest "highest" price
          int k;
          for(k = 0; k < stocks[i].countB && stocks[i].buyers[k].price > stocks[i].sellers[0].price; k++)
          {
            //cout << stocks[i].buyers[k].price << ' ' << stocks[i].sellers[0].price << endl;
          }
          if(stocks[i].buyers[0].price == stocks[i].buyers[k - 1].price)
          {
            //use seller price
            //make transaction with seller price and buyers share amt and symbol.
            //transaction = new Transaction(offerC - 1, stocks[i].buyers[0].ID, stocks[i].sellers[0].ID, stocks[i].sellers[0].price, stocks[i].buyers[0].shares, stocks[i].sellers[0].symbol);
            transaction->time = offerC - 1;
            transaction->buyerID = stocks[i].buyers[0].ID;
            transaction->sellerID = stocks[i].sellers[0].ID;
            transaction->price = stocks[i].sellers[0].price;
            transaction->shares = stocks[i].buyers[0].shares;
            strcpy(transaction->symbol, stocks[i].sellers[0].symbol);
            //cout << *transaction;
          }
          else
          {
            if(stocks[i].buyers[k-1].price >= stocks[i].sellers[0].price)
            {
              //make transaction with price found above and buyers share amt and symbol.
              //transaction = new Transaction(offerC - 1, stocks[i].buyers[0].ID, stocks[i].sellers[0].ID, stocks[i].buyers[k - 1].price, stocks[i].buyers[0].shares, stocks[i].sellers[0].symbol);
              transaction->time = offerC - 1;
              transaction->buyerID = stocks[i].buyers[0].ID;
              transaction->sellerID = stocks[i].sellers[0].ID;
              transaction->price = stocks[i].buyers[k - 1].price;
              transaction->shares = stocks[i].buyers[0].shares;
              strcpy(transaction->symbol, stocks[i].sellers[0].symbol);
              //cout << *transaction;
            }
          }

          stocks[i].sellers[0].shares -= stocks[i].buyers[0].shares;

          //move the buyers up by one if shares are equal
          for(int j = 0; j < stocks[i].countB; j++)
            stocks[i].buyers[j] = stocks[i].buyers[j + 1];
          (stocks[i].countB)--;
          return true;
        }
      }
    }

    if(false)
    {
      for(int k = 0; k < stocks[i].countB; k++)
        cout << 'B' << stocks[i].buyers[k].price << endl;
      for(int k = 0; k < stocks[i].countS; k++)
        cout << 'S' << stocks[i].sellers[k].price << endl;
      cout << endl;
    }

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
