
//
// MemoryIndexVocabularyIterator
//
// 23 November 2004 -- tds
//

#ifndef INDRI_MEMORYINDEXVOCABULARYITERATOR_HPP
#define INDRI_MEMORYINDEXVOCABULARYITERATOR_HPP

#include "indri/Mutex.hpp"
#include "indri/TermData.hpp"
#include "indri/DiskTermData.hpp"

namespace indri {
  namespace index {
    class MemoryIndexVocabularyIterator : public VocabularyIterator {
    private:
      typedef std::vector<MemoryIndex::term_entry*> VTermEntry;
      VTermEntry& _termData;
      VTermEntry::iterator _iterator;
      DiskTermData _diskTermData;
      
    public:
      MemoryIndexVocabularyIterator( VTermEntry& termData ) :
        _termData(termData)
      {
      }
      
      void startIteration() {
        _iterator = _termData.begin();

        _diskTermData.length = 0;
        _diskTermData.startOffset = 0;

        if( _iterator != _termData.end() ) {
          _diskTermData.termData = (*_iterator)->termData;
          _diskTermData.termID = (*_iterator)->termID;
        }
      }
      
      DiskTermData* currentEntry() { 
        if( _iterator == _termData.end() )
          return 0;
        
        return &_diskTermData;
      }
      
      bool nextEntry() {
        if( finished() )
          return false;
        
        _iterator++;

        if( finished() )
          return false;

        _diskTermData.termID++;
        _diskTermData.termData = (*_iterator)->termData;
        return true;
      }
      
      bool finished() {
        return _iterator == _termData.end();
      }
    };
  }
}

#endif // INDRI_MEMORYINDEXVOCABULARYITERATOR_HPP

