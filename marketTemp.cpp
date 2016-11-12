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
    int b;
    //find place to insert the buy offer
    for(b = 0; b < stocks[offerPos].countB && offer.price <= stocks[offerPos].buyers[b].price; b++);
    /*
    if(offer.price == stocks[offerPos].buyers[b].price) //handle ties in price
    {
      int i;
      for(i = b; i < stocks[offerPos].countB && offer.price == stocks[offerPos].buyers[i].price && offer.time > stocks[offerPos].buyers[i].time; i++);
      b = i;
    }
    */
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
    for(s = 0; s < stocks[offerPos].countS && offer.price >= stocks[offerPos].sellers[s].price; s++);
    /*
    if(offer.price == stocks[offerPos].sellers[s].price) //handle ties in price
    {
      int i;
      for(i = s; i < stocks[offerPos].countB && offer.price == stocks[offerPos].sellers[i].price && offer.time > stocks[offerPos].sellers[i].time; i++);
      s = i;
    }
    */
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
  if(stocks[lastInserted].countB > 0 && stocks[lastInserted].countS > 0) //transaction possible
  {
    if(stocks[lastInserted].buyers[0].price >= stocks[lastInserted].sellers[0].price) //transaction can be made
    {
      //find seller closest to buyer
      int closestSeller;
      for(closestSeller = 0; closestSeller < stocks[lastInserted].countS && stocks[lastInserted].buyers[0].price > stocks[lastInserted].sellers[closestSeller].price; closestSeller++)
      {
        //cout << stocks[i].sellers[k].price << ' ' << stocks[i].buyers[0].price << endl;
	//if(stocks[lastInserted].buyers[0].price == stocks[lastInserted].sellers[closestSeller].price)
	 //break;
      }
      if(closestSeller == 0)
	closestSeller = 1;
      if(stocks[lastInserted].buyers[0].price < stocks[lastInserted].sellers[closestSeller - 1].price)
        return false;
      //if(stocks[lastInserted].buyers[0].price == stocks[lastInserted].sellers[closestSeller - 1].price)
	//cout << "EQUAL "<< stocks[lastInserted].buyers[0].price << ' ' << stocks[lastInserted].sellers[closestSeller - 1].price  << ' ' << stocks[lastInserted].sellers[closestSeller].price<< endl;
      
      //stocks[i].sellers[closestSeller - 1] should be closest
      //cout << stocks[lastInserted].buyers[0].price << ' ' << stocks[lastInserted].sellers[closestSeller - 1].price << endl;
      //check for 2nd buyer
      if(strcmp(stocks[lastInserted].buyers[1].symbol, stocks[lastInserted].buyers[0].symbol) != 0)
      {
        if(stocks[lastInserted].buyers[0].shares >= stocks[lastInserted].sellers[closestSeller - 1].shares) //buyer wants equal or more shares
        {
          //no 2nd buyer. Use seller price
          if(stocks[lastInserted].buyers[0].time > stocks[lastInserted].sellers[closestSeller - 1].time)
            this->transaction(stocks[lastInserted].buyers[0].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].sellers[closestSeller - 1].price, stocks[lastInserted].sellers[closestSeller - 1].shares, stocks[lastInserted].buyers[0].symbol, transaction);
          else
            this->transaction(stocks[lastInserted].sellers[closestSeller - 1].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].sellers[closestSeller - 1].price, stocks[lastInserted].sellers[closestSeller - 1].shares, stocks[lastInserted].buyers[0].symbol, transaction);

          stocks[lastInserted].buyers[0].shares -= stocks[lastInserted].sellers[closestSeller - 1].shares; //remove shares
          if(stocks[lastInserted].buyers[0].shares == 0)
          {
            //remove buyers[0]
            for(int i = 0; i < stocks[lastInserted].countB; i++)
              stocks[lastInserted].buyers[i] = stocks[lastInserted].buyers[i + 1];
            (stocks[lastInserted].countB)--;
          }
          //remove sellers[closestSeller - 1]
          for(int i = closestSeller - 1; i < stocks[lastInserted].countS; i++)
            stocks[lastInserted].sellers[i] = stocks[lastInserted].sellers[i + 1];
          (stocks[lastInserted].countS)--;
          return true;
        }
        else //if buyer has less shares than seller
        {
          //no 2nd buyer. Use seller price
          if(stocks[lastInserted].buyers[0].time > stocks[lastInserted].sellers[closestSeller - 1].time)
            this->transaction(stocks[lastInserted].buyers[0].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].sellers[closestSeller - 1].price, stocks[lastInserted].buyers[0].shares, stocks[lastInserted].buyers[0].symbol, transaction);
          else
            this->transaction(stocks[lastInserted].sellers[closestSeller - 1].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].sellers[closestSeller - 1].price, stocks[lastInserted].buyers[0].shares, stocks[lastInserted].buyers[0].symbol, transaction);

          stocks[lastInserted].sellers[closestSeller - 1].shares -= stocks[lastInserted].buyers[0].shares; //remove shares

          //remove buyers[0]
          for(int i = 0; i < stocks[lastInserted].countB; i++)
            stocks[lastInserted].buyers[i] = stocks[lastInserted].buyers[i + 1];
          (stocks[lastInserted].countB)--;
          return true;
        }
      }
      else //2nd buyer exists
      {
        if(stocks[lastInserted].buyers[1].price >= stocks[lastInserted].sellers[closestSeller - 1].price)
        {
          //use 2nd buyer price
          if(stocks[lastInserted].buyers[0].shares >= stocks[lastInserted].sellers[closestSeller - 1].shares)
          {
            if(stocks[lastInserted].buyers[0].time > stocks[lastInserted].sellers[closestSeller - 1].time)
              this->transaction(stocks[lastInserted].buyers[0].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].buyers[1].price, stocks[lastInserted].sellers[closestSeller - 1].shares, stocks[lastInserted].buyers[0].symbol, transaction);
            else
              this->transaction(stocks[lastInserted].sellers[closestSeller - 1].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].buyers[1].price, stocks[lastInserted].sellers[closestSeller - 1].shares, stocks[lastInserted].buyers[0].symbol, transaction);

            stocks[lastInserted].buyers[0].shares -= stocks[lastInserted].sellers[closestSeller - 1].shares; //remove shares
            if(stocks[lastInserted].buyers[0].shares == 0)
            {
              //remove buyers[0]
              for(int i = 0; i < stocks[lastInserted].countB; i++)
                stocks[lastInserted].buyers[i] = stocks[lastInserted].buyers[i + 1];
              (stocks[lastInserted].countB)--;
            }
            //remove sellers[closestSeller - 1]
            for(int i = closestSeller - 1; i < stocks[lastInserted].countS; i++)
              stocks[lastInserted].sellers[i] = stocks[lastInserted].sellers[i + 1];
            (stocks[lastInserted].countS)--;
            return true;
          }
          else //if buyer has less shares than seller
          {
            if(stocks[lastInserted].buyers[0].time > stocks[lastInserted].sellers[closestSeller - 1].time)
              this->transaction(stocks[lastInserted].buyers[0].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].buyers[1].price, stocks[lastInserted].buyers[0].shares, stocks[lastInserted].buyers[0].symbol, transaction);
            else
              this->transaction(stocks[lastInserted].sellers[closestSeller - 1].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].buyers[1].price, stocks[lastInserted].buyers[0].shares, stocks[lastInserted].buyers[0].symbol, transaction);

            stocks[lastInserted].sellers[closestSeller - 1].shares -= stocks[lastInserted].buyers[0].shares; //remove shares

            //remove buyers[0]
            for(int i = 0; i < stocks[lastInserted].countB; i++)
              stocks[lastInserted].buyers[i] = stocks[lastInserted].buyers[i + 1];
            (stocks[lastInserted].countB)--;
            return true;
          }
        }
        else //use seller price
        {
          if(stocks[lastInserted].buyers[0].shares >= stocks[lastInserted].sellers[closestSeller - 1].shares)
          {
            if(stocks[lastInserted].buyers[0].time > stocks[lastInserted].sellers[closestSeller - 1].time)
              this->transaction(stocks[lastInserted].buyers[0].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].sellers[closestSeller - 1].price, stocks[lastInserted].sellers[closestSeller - 1].shares, stocks[lastInserted].buyers[0].symbol, transaction);
            else
              this->transaction(stocks[lastInserted].sellers[closestSeller - 1].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].sellers[closestSeller - 1].price, stocks[lastInserted].sellers[closestSeller - 1].shares, stocks[lastInserted].buyers[0].symbol, transaction);

            stocks[lastInserted].buyers[0].shares -= stocks[lastInserted].sellers[closestSeller - 1].shares; //remove shares
            if(stocks[lastInserted].buyers[0].shares == 0)
            {
              //remove buyers[0]
              for(int i = 0; i < stocks[lastInserted].countB; i++)
                stocks[lastInserted].buyers[i] = stocks[lastInserted].buyers[i + 1];
              (stocks[lastInserted].countB)--;
            }
            //remove sellers[closestSeller - 1]
            for(int i = closestSeller - 1; i < stocks[lastInserted].countS; i++)
              stocks[lastInserted].sellers[i] = stocks[lastInserted].sellers[i + 1];
            (stocks[lastInserted].countS)--;
            return true;
          }
          else //if buyer has less shares than seller
          {
            if(stocks[lastInserted].buyers[0].time > stocks[lastInserted].sellers[closestSeller - 1].time)
              this->transaction(stocks[lastInserted].buyers[0].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].sellers[closestSeller - 1].price, stocks[lastInserted].buyers[0].shares, stocks[lastInserted].buyers[0].symbol, transaction);
            else
              this->transaction(stocks[lastInserted].sellers[closestSeller - 1].time, stocks[lastInserted].buyers[0].ID, stocks[lastInserted].sellers[closestSeller - 1].ID, stocks[lastInserted].sellers[closestSeller - 1].price, stocks[lastInserted].buyers[0].shares, stocks[lastInserted].buyers[0].symbol, transaction);

            stocks[lastInserted].sellers[closestSeller - 1].shares -= stocks[lastInserted].buyers[0].shares; //remove shares

            //remove buyers[0]
            for(int i = 0; i < stocks[lastInserted].countB; i++)
              stocks[lastInserted].buyers[i] = stocks[lastInserted].buyers[i + 1];
            (stocks[lastInserted].countB)--;
            return true;
          }
        }
      }
    }

  }
  return false; // means no more transactions, and transaction will be ignored
} // newTransaction()

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
