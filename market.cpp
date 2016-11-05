// Author: Sean Davis
#include "market.h"
#include "MarketDriver.h"

Market::Market(int numStocks, int offerCount, int IDs)
{
} // Market()

void Market::newOffer(const Offer &offer)
{
} // newOffer()


bool Market::newTransaction(Transaction *transaction)
{
  return false; // means no more transactions, and transaction will be ignored
} // newTransaction()
