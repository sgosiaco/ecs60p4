#ifndef _MAX_BINARY_HEAP_H_
#define _MAX_BINARY_HEAP_H_
  
#include "dsexceptions.h"
#include "vector.h"
#include "MarketDriver.h"

  // MaxBinaryHeap class
  //
  // CONSTRUCTION: with a negative infinity sentinel and
  //   optional capacity (that defaults to 100)
  //
  // ******************PUBLIC OPERATIONS*********************
  // void insert( x ) --> Insert x
  // deleteMax( maxItem )   --> Remove (and optionally return) largest item
  // Offer findMax()  --> Return largest item
  // bool isEmpty()  --> Return true if empty; else false
  // bool isFull()   --> Return true if full; else false
  // void makeEmpty()--> Remove all items
  // ******************ERRORS********************************
  // Throws Underflow and Overflow as warranted

  
class MaxBinaryHeap
{
  public:
  explicit MaxBinaryHeap(int size = 2000);

  bool isEmpty() const;
  bool isFull() const;
  Offer& findMax();
  const Offer getSecondMax() const; // return the second largest offer
  void storeSecondMax();
  void insert( const Offer &x );
  void deleteMax();
  void deleteMax(Offer &maxItem);
  void deleteRecentOffer();
  void makeEmpty();
  Offer& getRecentBidder();

  private:
  int currentSize;  // Number of elements in heap
  int maxSize;
  int recentOfferIndex;
  Offer secondMax;
  Offer *array;

  void buildHeap();
  void percolateDown(int hole);
};

#endif
