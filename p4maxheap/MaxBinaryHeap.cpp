#include "MaxBinaryHeap.h"
#include <cstring>
/**
 * Construct the binary heap.
 * capacity is the capacity of the binary heap.
 */

MaxBinaryHeap::MaxBinaryHeap(int size)
: currentSize(0), maxSize(size)
{
  array = new Offer[maxSize];
  secondMax.price = 0.00;
  array[0].price = -1.00;
}

/**
 * Insert item x into the priority queue, maintaining heap order.
 * Duplicates are allowed.
 * Throw Overflow if container is full.
 */

void MaxBinaryHeap::insert( const Offer &offer )
{
  if( isFull( ) )
    throw Overflow( );

  // Percolate up
  int hole = ++currentSize;
  for( ; hole > 1 && offer.price > array[hole/2].price; hole /= 2 )
    array[hole] = array[ hole/2 ];
  
  array[hole] = offer;
  recentOfferIndex = hole;
  storeSecondMax();
}


void MaxBinaryHeap::storeSecondMax()
{
  if(currentSize <= 1)
    secondMax = array[0];
  else if(currentSize == 2)
    secondMax = array[2];
  else
  {
    if(array[2].price > array[3].price)
      secondMax = array[2];
    else if(array[2].price < array[3].price)
      secondMax = array[3];
    else
    {
      if(array[2].time < array[3].time)
        secondMax = array[2];
      else
        secondMax = array[3];
    } // prices are the same
  } //for 3 or more total nodes
} //storeSecondMax()

/**
 * Find the largest item in the priority queue.
 * Return the largest item, or throw Underflow if empty.
 */

Offer& MaxBinaryHeap::findMax()
{
  if(isEmpty())
    return array[0];
  return array[1];
} // findMax()


const Offer MaxBinaryHeap::getSecondMax() const
{
  return secondMax;
} // getSecondMax()



Offer& MaxBinaryHeap::getRecentBidder()
{
  return array[recentOfferIndex];
} // getRecentBidder()


void MaxBinaryHeap::deleteRecentOffer()
{
  if(isEmpty())
    return;
  array[recentOfferIndex] = array[currentSize--];
  percolateDown(recentOfferIndex);
  storeSecondMax();
  recentOfferIndex = 0;
} // remove item at the recentOfferIndex


/**
 * Remove the largest item from the priority queue.
 * Throw Underflow if empty.
 */

void MaxBinaryHeap::deleteMax( )
{
  if( isEmpty( ) )
    return;
  array[1] = array[ currentSize-- ];
  percolateDown( 1 );
  //cout << array[1].price << " " << array[1].ID << " = after deletion\n";
  storeSecondMax();
} 

/**
 * Remove the largest item from the priority queue
 * and place it in maxItem. Throw Underflow if empty.
 */

void MaxBinaryHeap::deleteMax( Offer & maxItem )
{
  if( isEmpty( ) )
    return;
  maxItem = array[ 1 ];
  array[ 1 ] = array[ currentSize-- ];
  percolateDown( 1 );
}

/**
 * Establish heap order property from an arbitrary
 * arrangement of items. Runs in linear time.
 */

void MaxBinaryHeap::buildHeap( )
{
  for( int i = currentSize / 2; i > 0; i-- )
    percolateDown( i );
}

/**
 * Test if the priority queue is logically empty.
 * Return true if empty, false otherwise.
 */

bool MaxBinaryHeap::isEmpty( ) const
{
  return currentSize == 0;
}

/**
 * Test if the priority queue is logically full.
 * Return true if full, false otherwise.
 */

bool MaxBinaryHeap::isFull( ) const
{
  return currentSize == 1000;
}

/**
 * Make the priority queue logically empty.
 */

void MaxBinaryHeap::makeEmpty( )
{
  currentSize = 0;
}

/**
 * Internal method to percolate down in the heap.
 * hole is the index at which the percolate begins.
 */

void MaxBinaryHeap::percolateDown( int hole )
{
  int child;
  Offer tmp = array[hole];

  for( ; hole * 2 <= currentSize; hole = child )
  {
    child = hole * 2;
    if(child != currentSize)
    {
      if(array[child].price == array[child + 1].price)
      {
        if(array[child].time < array[child + 1].time)
          array[hole] = array[child];
        else
          array[hole] = array[++child];
      }
      else if(array[child].price > array[child + 1].price)
        array[hole] = array[child];
       else
        array[hole] = array[++child];
    } // two children
 
    if(array[child].price == tmp.price)
    {
      if(array[child].time < tmp.time)
        array[hole] = array[child];
      else
        break;
    }
    else if(array[child].price > tmp.price)
      array[hole] = array[child];
    else
      break;
  } 
  array[hole] = tmp;
} // percolateDown()
