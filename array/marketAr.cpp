// Author: Sean Davis
#include "marketAr.h"

using namespace std;

Stock::Stock():currBidders(0), currSellers(0)
{
  bidders = vector<Offer>(500);
  sellers = vector<Offer>(500);
} // Stock() default constructor

Stock::~Stock()
{
} // Stock() default constructor


Market::Market(int numOfStocks, int numOfOffers, int IDs):
stockCount(numOfStocks), offerCount(numOfOffers), IDCount(IDs), currStock(0)
{
  arrayStocks = new Stock[stockCount];
  lastSeller = 0;
  lastBidder = 0;
  recentStock = 0;
} // Market()

Market::~Market()
{
  delete [] arrayStocks;
} //destructor

inline void Stock::addToBidderAr(const Offer &offer)
{
  int i;
  //cout << " HERE\n";
  for(i = currBidders; i > 0 && bidders[i - 1].price < offer.price; i--)
      bidders[i] = bidders[i - 1];
  //cout << " HERE 2\n";
  bidders[i] = offer;
  lastBidder = i;
  currBidders++;

} // addToBidderAr()



inline void Stock::addToSellerAr(const Offer &offer)
{
  int i;

  for(i = currSellers; i > 0 && sellers[i - 1].price < offer.price; i--)
      sellers[i] = sellers[i - 1];

  sellers[i] = offer; //shallow copy
  lastSeller = i;
  currSellers++;
} // addToSellerAr()

bool Stock::bidderTransaction(int bIndex, Transaction& t)
{
  //pass in the index where the recent offer is
  for(int i = 0; i < currSellers; i++)
  {
    if(sellers[i].price <= bidders[bIndex].price)
    {
      if(sellers[i].time > bidders[bIndex].time)
        t.time = sellers[i].time;
      else
        t.time = bidders[bIndex].time;

      t.buyerID = bidders[bIndex].ID;
      t.sellerID = sellers[i].ID;
      t.price = sellers[i].price;
      strcpy(t.symbol, bidders[bIndex].symbol);

      if(sellers[i].shares == bidders[bIndex].shares)
      {
        //cout << "SELLERS = BUYERS, bidders[bIndex].shares = " << bidders[bIndex].shares << " and name is " << bidders[bIndex].symbol << endl;
        t.shares = bidders[bIndex].shares;
        shiftSellerArray(i);
        shiftBidderArray(bIndex);
        return true;
      } //update both arrays
      if(sellers[i].shares > bidders[bIndex].shares)
      {
        t.shares = bidders[bIndex].shares;
        sellers[i].shares -= bidders[bIndex].shares;
        shiftBidderArray(bIndex);
        return true;
      } // upate the stock's bidder list
      else
      {
        t.shares = sellers[i].shares;
        bidders[bIndex].shares -= sellers[i].shares;
        //cout << "shares left = " << bidders[bIndex].shares << endl;
        shiftSellerArray(i);
        return true;
      } // upate the stock's seller list
    }
  }
  return false;
} // bidderTransaction() goes through list of sellers for a stock


bool Stock::sellerTransaction(int sIndex, Transaction& t)
{
  for(int bIndex = 0; bIndex < currBidders; bIndex++)
  {
      if(bidders[bIndex].price >= sellers[sIndex].price)
      {
        if(bidders[bIndex].time > sellers[sIndex].time)
          t.time = bidders[bIndex].time;
        else
          t.time = sellers[sIndex].time;

        t.buyerID = bidders[bIndex].ID;
        t.sellerID = sellers[sIndex].ID;
        strcpy(t.symbol, sellers[sIndex].symbol);

        if(bIndex < currBidders - 1 && bidders[bIndex + 1].price >= sellers[sIndex].price)
        {
          //sellers[sIndex].price = bidders[bIndex + 1].price;
          t.price = bidders[bIndex + 1].price;
        } // Felsman's Rule
        else
           t.price = sellers[sIndex].price;

        if(bidders[bIndex].shares == sellers[sIndex].shares)
        {
          t.shares = sellers[sIndex].shares;
          shiftSellerArray(sIndex);
          shiftBidderArray(bIndex);
          return true;
        } //update both arrays
        if(bidders[bIndex].shares > sellers[sIndex].shares)
        {
          t.shares = sellers[sIndex].shares;
          bidders[bIndex].shares -= sellers[sIndex].shares;
          shiftSellerArray(sIndex);
          return true;
        } // upate the stock's bidder list
        else
        {
          t.shares = bidders[bIndex].shares;
          sellers[sIndex].shares -= bidders[bIndex].shares;
          shiftBidderArray(bIndex);
          return true;
        } // upate the stock's seller list
      }
  }
  return false;
} // sellerTransaction()


int Stock::findBidder(Offer& offer)
{
  for(int i = 0; i < currBidders; i++)
    if(bidders[i].time == offer.time)
      return i;
  return -1;
} //find indiviual bidder in the bidder array


int Stock::findSeller(Offer& offer)
{
  for(int i = 0; i < currSellers; i++)
    if(sellers[i].time == offer.time)
      return i;
  return -1;
} //find indiviual bidder in the bidder array



void Stock::printBidders()
{
  for(int j = 0; j < currBidders; j++)
    cout << bidders[j].time << " " << bidders[j].price << endl;
} // printBidders()



void Stock::printSellers()
{
  for(int j = 0; j < currSellers; j++)
    cout << sellers[j].time << " " << sellers[j].price << endl;

} //printSellers

inline void Stock::shiftBidderArray(int bIndex)
{
  for(int i = bIndex; i < currBidders - 1; i++)
    bidders[i] = bidders[i + 1];

  currBidders--;
} //shiftBidderArray()


inline void Stock::shiftSellerArray(int sIndex)
{
  for(int i = sIndex; i < currSellers - 1; i++)
    sellers[i] = sellers[i + 1];

  currSellers--;
} //shiftSellerArray()



void Market::addOffer(const Offer& offer, int index)
{
  if(offer.type == 'B')
  {
    strcpy(arrayStocks[index].name, offer.symbol);
    arrayStocks[index].addToBidderAr(offer);
  }
  else
  {
    strcpy(arrayStocks[index].name, offer.symbol);
    arrayStocks[index].addToSellerAr(offer);
  }
  recentOffer = offer;
  recentStock = index;
} //addOffer()


int Market::getCurrStock()
{
  return currStock;
} //getCurrStock()


void Market::newOffer(const Offer &offer)
{
    int i;
    if(!currStock)
    {
      addOffer(offer, 0);
      currStock++;
    } // if currStock = 0
    else
    {
      if(strcmp(offer.symbol, arrayStocks[recentStock]) == 0)
      {
        addOffer(offer, recentStock);
        if(recentStock == currStock)
          currStock++;
      }
      else
      {
        for(i = 0; i < currStock && (strcmp(offer.symbol, arrayStocks[i].name) != 0); i++);
        addOffer(offer, i);
        if(i == currStock)
          currStock++;
      }
    } // currStock > 0
} // newOffer()


bool Market::newTransaction(Transaction *transaction)
{
  int i;

  for(i = 0; i < currStock &&
      (strcmp(arrayStocks[i].name, recentOffer.symbol) != 0); i++);
 // cout << recentOffer.symbol << " " << recentOffer.type << " "
   // << recentOffer.price << " " << recentOffer.shares << endl;
  if(recentOffer.type == 'B')
  {
    int bIndex = arrayStocks[i].findBidder(recentOffer);
    if(bIndex != -1 &&
      arrayStocks[i].bidderTransaction(bIndex, *transaction))
      return true;
  }
  if(recentOffer.type == 'S')
  {
    int sIndex = arrayStocks[i].findSeller(recentOffer);
    //cout << "sIndex = " << sIndex << endl << endl;
    if(sIndex != -1 && arrayStocks[i].sellerTransaction(sIndex, *transaction))
      return true;
  }
  return false;
} // newTransaction()


void Market::printStocks()
{
  for(int i = 0; i < currStock; i++)
  {
    cout << arrayStocks[i].name << endl; // one stock

    cout << "All Buyers:" << endl;
    arrayStocks[i].printBidders();

    cout << "All Sellers:" << endl;
    arrayStocks[i].printSellers();
  }
} // printStocks()
