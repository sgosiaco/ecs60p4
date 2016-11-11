#include "stock.h"

using namespace std;



Stock::Stock():currBidders(0), currSellers(0), lastSellerInserted(0), bidders(400)
{
} // Stock() default constructor


Stock::~Stock()
{
} // Stock() default constructor


void Stock::addToStock(const Offer &offer)
{
  int i;
//  cout << "Here " << offer.price << " " << offer.symbol << endl;
  strcpy(name, offer.symbol);
  if(offer.type == 'B')
  {
    bidders.insert(offer);  //insert into max heap
    currBidders++;
  } // if a incoming bidder
  else
  {
    for(i = currSellers; i > 0 && sellers[i - 1].price < offer.price; i--)
      sellers[i] = sellers[i - 1];
    //cout << " HERE 3\n";
    sellers[i] = offer;
    lastSellerInserted = i;
    currSellers++;
  } // if a incoming seller
} // addToBidderAr()


bool Stock::bidderTransaction(Transaction& t)
{
  //cout << " starting bidder transaction\n";
//bidder comes in and need to check against multiple sellers
  Offer &recentBidder = bidders.getRecentBidder(); //find recentBidder

  if(recentBidder.price == -1)
    return false;

  for(int i = 0; i < currSellers; i++)
  {
    if(sellers[i].price <= recentBidder.price)
    {
      if(sellers[i].time > recentBidder.time)
        t.time = sellers[i].time;
      else
        t.time = recentBidder.time;

      t.buyerID = recentBidder.ID;
      t.sellerID = sellers[i].ID;
      t.price = sellers[i].price;
      strcpy(t.symbol, recentBidder.symbol);

      if(sellers[i].shares == recentBidder.shares)
      {
       // cout << "SELLERS = BUYERS, recentBidder.shares = " << recentBidder.shares << " and name is " << recentBidder.symbol << endl;
        t.shares = recentBidder.shares;
        shiftSellerArray(i);
        bidders.deleteRecentOffer();
        //cout << t;
        return true;
      } //update both arrays
      else if(sellers[i].shares > recentBidder.shares)
      {
        t.shares = recentBidder.shares;
        sellers[i].shares -= recentBidder.shares;
        bidders.deleteRecentOffer();
        //cout << t;
        return true;
      } // upate the stock's bidder list
      else
      {
        t.shares = sellers[i].shares;
        recentBidder.shares -= sellers[i].shares;
        //cout << "shares left = " << recentBidder.shares << endl;
        shiftSellerArray(i);
        //cout << t;
        return true;
      } // upate the stock's seller list
    }
  }
  return false;
} // bidderTransaction() goes through list of sellers for a stock



bool Stock::sellerTransaction(int sIndex, Transaction& t)
{
  Offer &highest = bidders.findMax(), secondHighest = bidders.getSecondMax();
  if(highest.price < sellers[sIndex].price)
    return false;

  if(highest.time > sellers[sIndex].time) // pick the larger time
    t.time = highest.time;
  else
    t.time = sellers[sIndex].time;

  t.buyerID = highest.ID;
  t.sellerID = sellers[sIndex].ID;
  strcpy(t.symbol, sellers[sIndex].symbol);

  if(secondHighest.price >= sellers[sIndex].price) //Felsman's Rule
    t.price = secondHighest.price;
  else
    t.price = sellers[sIndex].price;

  if(highest.shares == sellers[sIndex].shares)
  {
    t.shares = sellers[sIndex].shares;
    shiftSellerArray(sIndex);
    bidders.deleteMax();
    //cout << t;
    return true;
  } // same amount

  if(highest.shares > sellers[sIndex].shares)
  {
    t.shares = sellers[sIndex].shares;
    highest.shares -= sellers[sIndex].shares; // changes actual value in tree
    shiftSellerArray(sIndex);
    //cout << t;
    return true;
  } // bidder asks for more than what seller offers

  else
  {
    t.shares = highest.shares;
    sellers[sIndex].shares -= highest.shares;
    bidders.deleteMax();
    //cout << t;
  } // bidders ask for less than what seller offers
  return true;
} // sellerTransaction()


int Stock::findSeller(Offer& offer)
{
  //for(int i = 0; i < currSellers; i++)
    //if(sellers[i].time == offer.time)
      //return i;
  //return -1;
  if(sellers[lastSellerInserted].time == offer.time)
    return lastSellerInserted;
  else
    return -1;
} //find indiviual bidder in the bidder array


char* Stock::getName()
{
  return name;
}





void Stock::printSellers()
{
  for(int j = 0; j < currSellers; j++)
    cout << sellers[j].time << " " << sellers[j].price << endl;

} //printSellers



inline void Stock::shiftSellerArray(int sIndex)
{
  for(int i = sIndex; i < currSellers - 1; i++)
    sellers[i] = sellers[i + 1];

  currSellers--;
} //shiftSellerArray()


void Stock::setName(const char *n)
{
  strcpy(name, n);
} // setName()


/*bool Stock::operator!=(const Stock *rhs)
{
  cout << "in overload\n";
  return true;
  //return (getName() != rhs->getName());
}*/
