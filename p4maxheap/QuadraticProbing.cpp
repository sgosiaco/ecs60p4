#include "QuadraticProbing.h"

        
        QuadraticHashTable::QuadraticHashTable(int size)
          : ITEM_NOT_FOUND(NULL)
        {
            size = 2 * size;
            maxSize = nextPrime(size);
            array = new HashEntry[maxSize];
            makeEmpty( );
        } // construct the hash table


        bool QuadraticHashTable::isPrime( int n ) const
        {
            if( n == 2 || n == 3 )
                return true;

            if( n == 1 || n % 2 == 0 )
                return false;

            for( int i = 3; i * i <= n; i += 2 )
                if( n % i == 0 )
                    return false;

            return true;
        } // method to test if positive number is prime

        
        int QuadraticHashTable::nextPrime( int n ) const
        {
            if( n % 2 == 0 )
                n++;

            for( ; !isPrime( n ); n += 2 )
                ;

            return n;
        } // Internal method to return a prime number at least as large as n.
          // Assumes n > 0.



        void QuadraticHashTable::insert( Stock *x )
        {
                // Insert x as active
            int currentPos = findPos(x);
            if(isActive(currentPos))
            {
                /*if(array[currentPos].element->getName() == x->getName())
                  cout << "HERE BIOTCHES\n";*/
                return;
            }
            array[currentPos] = HashEntry(x, ACTIVE);
            currentSize++;
                // Rehash; see Section 5.5
            //if( ++currentSize > maxSize / 2 )
                //rehash( );
        } // insert into hash table, if stock is already there, then do nothing




/*
        void QuadraticHashTable::rehash( )
        {
            vector<HashEntry> oldArray = array;

                // Create new double-sized, empty table
            array.resize( nextPrime( 2 * oldArray.size( ) ) );
            for( int j = 0; j < maxSize; j++ )
                array[ j ].info = EMPTY;

                // Copy table over
            currentSize = 0;
            for( int i = 0; i < oldArray.size( ); i++ )
                if( oldArray[ i ].info == ACTIVE )
                    insert(oldArray[i].element );
        } // rehash()*/

        
      /*  bool QuadraticHashTable::operator!=(Stock *rhs)
        {
          cout << "in overload\n";
          return true;
          //return (getName() != rhs->getName());
        }*/

     

        void QuadraticHashTable::remove( Stock* x )
        {
            int currentPos = findPos( x );
            if( isActive( currentPos ) )
                array[ currentPos ].info = DELETED;
        } //remove()

        
        Stock* QuadraticHashTable::find(Stock* x) 
        {
            
            int currentPos = findPos(x);

            if(isActive(currentPos))
              return array[currentPos].element;
            
            return ITEM_NOT_FOUND;
        } // find() if not matching, return NULL

        /**
         * Make the hash table logically empty.
         */
       
 
        int QuadraticHashTable::findPos( Stock* x ) 
        {
/* 1*/      int collisionNum = 0;
/* 2*/      int currentPos = hash( x, maxSize );
            
            //cout << "currentPos = " << currentPos << endl;

/* 3*/      while(array[currentPos].info != EMPTY && 
              strcmp(array[currentPos].element->getName(), x->getName())!= 0 )
            {
                //out << array[currentPos].element->getName() << endl;
/* 4*/          currentPos += 2 * ++collisionNum - 1;  // Compute ith probe
/* 5*/          if( currentPos >= maxSize )
/* 6*/              currentPos -= maxSize;
            }
/* 7*/      return currentPos;
        } // Return the position where the search for x terminates.


        
        void QuadraticHashTable::makeEmpty( )
        {
            currentSize = 0;
            for( int i = 0; i < maxSize; i++ )
                array[i].info = EMPTY;
        }

       
 
        const QuadraticHashTable & QuadraticHashTable::
        operator=( const QuadraticHashTable & rhs )
        {
            if( this != &rhs )
            {
                array = rhs.array;
                currentSize = rhs.currentSize;
            }
            return *this;
        } //deep copy


        /**
         * Return true if currentPos exists and is active.
         */
        
        bool QuadraticHashTable::isActive( int currentPos ) const
        {
            return array[ currentPos ].info == ACTIVE;
        }


        
        /**
         * A hash routine for Stock pointer objects.
         */
        
        int QuadraticHashTable::hash(Stock *stock, int tableSize) const
        {
            int hashVal = 0;
            char *n = stock->getName();
//            cout << n << " is the name of the stock\n";

            for( int i = 0; i < (int)strlen(n); i++ )
                hashVal = 37 * hashVal + n[ i ];

            hashVal %= tableSize;
            //cout << hashVal << endl;
            if( hashVal < 0 )
                hashVal += tableSize;

            return hashVal;
        }


        /*
        int QuadraticHashTable::hash( const string & key, int tableSize ) const
        {
            int hashVal = 0;

            for( int i = 0; i < key.length( ); i++ )
                hashVal = 37 * hashVal + key[ i ];

            hashVal %= tableSize;
            if( hashVal < 0 )
                hashVal += tableSize;

            return hashVal;
        }

         
        int QuadraticHashTable::hash( int key, int tableSize ) const
        {
            if( key < 0 ) key = -key;
            return key % tableSize;
        }*/
