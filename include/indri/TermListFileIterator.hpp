
//
// TermListFileIterator
//
// 23 November 2004 -- tds
//

#ifndef INDRI_TERMLISTFILEITERATOR_HPP
#define INDRI_TERMLISTFILEITERATOR_HPP

#include "indri/TermList.hpp"

namespace indri {
  namespace index {
    class TermListFileIterator {
    public:
      virtual ~TermListFileIterator() {};
      
      virtual void startIteration() = 0;
      virtual bool nextEntry() = 0;
      virtual bool finished() = 0;
      virtual TermList* currentEntry() = 0;
    };
  }
}

#endif // INDRI_TERMLISTFILEITERATOR_HPP

