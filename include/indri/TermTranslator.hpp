
//
// TermTranslator
//
// 14 January 2005 -- tds
//

#ifndef INDRI_TERMTRANSLATOR_HPP
#define INDRI_TERMTRANSLATOR_HPP

#include "indri/HashTable.hpp"
#include <vector>
#include "indri/TermBitmap.hpp"

namespace indri {
  namespace index {
    class TermTranslator {
    private:
      TermBitmap* _bitmap;
      int _previousFrequentCount;
      int _currentFrequentCount;
      int _previousTermCount;
      int _currentTermCount;

      std::vector<int>* _frequentMap;
      HashTable<int, int>* _wasInfrequentMap;

    public:
      ~TermTranslator() {
        delete _frequentMap;
      }

      TermTranslator( int previousFrequentCount,
                      int currentFrequentCount,
                      int previousTermCount,
                      int currentTermCount,
                      std::vector<int>* frequentMap,
                      HashTable<int, int>* wasInfrequentMap,
                      TermBitmap* bitmap ) 
        :
        _bitmap(bitmap),
        _frequentMap(frequentMap),
        _wasInfrequentMap(wasInfrequentMap)
      {
        assert( currentFrequentCount >= 0 );
        assert( previousFrequentCount >= 0 );
        assert( previousTermCount >= 0 );
        assert( currentTermCount >= 0 );

        _previousFrequentCount = previousFrequentCount;
        _currentFrequentCount = currentFrequentCount;
        _previousTermCount = previousTermCount;
        _currentTermCount = currentTermCount;
      }

      int operator() ( int termID ) {
        assert( termID >= 0 );
        assert( termID <= _previousTermCount );
        int result = 0;
        int* value;

        if( termID <= _previousFrequentCount ) {
          // common case, termID is a frequent term
          assert( _frequentMap->size() > termID );
          result = (*_frequentMap)[termID];
        } else {
          // term may have become frequent, so check the wasInfrequentMap
          value = (*_wasInfrequentMap).find( termID - _previousFrequentCount - 1 );

          if( value ) {
            result = *value;
          } else {
            // term wasn't frequent and isn't now either, so get it from the bitmap
            result = 1 + _currentFrequentCount + _bitmap->get( termID - _previousFrequentCount - 1 );
          }
        }

        assert( result >= 0 );
        assert( result <= _currentTermCount );
        return result;
      }
    };
  }
}

#endif // INDRI_TERMTRANSLATOR_HPP



