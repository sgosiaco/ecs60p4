        #ifndef _QUADRATIC_PROBING_H_
        #define _QUADRATIC_PROBING_H_

        #include "mystring.h"
        #include "vector.h"
        #include "stock.h"
        // QuadraticProbing Hash table class
        //
        // CONSTRUCTION: an initialization for ITEM_NOT_FOUND
        //               and an approximate initial size or default of 101
        //
        // ******************PUBLIC OPERATIONS*********************
        // void insert( x )       --> Insert x
        // void remove( x )       --> Remove x
        // Hashable find( x )     --> Return item that matches x
        // void makeEmpty( )      --> Remove all items
        // int hash( String str, int tableSize )
        //                        --> Static method to hash strings

        class QuadraticHashTable
        {
          public:
            explicit QuadraticHashTable(int arraySize = 101);
            /*QuadraticHashTable(const QuadraticHashTable & rhs )
              : array( rhs.array), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
                currentSize( rhs.currentSize ) { }*/

            void makeEmpty( );
            void insert(Stock *x);
            void remove(Stock *x); 
            Stock* find(Stock *x);
            const QuadraticHashTable & operator=(const QuadraticHashTable &rhs);

            bool operator!=(Stock *rhs);
            enum EntryType { ACTIVE, EMPTY, DELETED };

          private:
            struct HashEntry
            {
                Stock *element;
                EntryType info;

                HashEntry(Stock* stock = NULL, EntryType i = EMPTY)
                  : info(i) 
                  {
                    element = stock;
                  }
            };
            HashEntry *array;
            int currentSize;
            int maxSize;
            Stock *ITEM_NOT_FOUND;
            bool isPrime( int n ) const;
            int nextPrime( int n ) const;
            bool isActive( int currentPos ) const;
            int findPos( Stock *x ) ;
            int hash( Stock *stock, int tableSize ) const;
            void rehash( );
        };

        #endif
