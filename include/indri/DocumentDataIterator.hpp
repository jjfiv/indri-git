
//
// DocumentDataIterator
//
// 21 January 2005 -- tds
//

#ifndef INDRI_DOCUMENTDATAITERATOR_HPP
#define INDRI_DOCUMENTDATAITERATOR_HPP

#include "indri/DocumentData.hpp"

namespace indri {
  namespace index {
    class DocumentDataIterator {
    public:
      virtual ~DocumentDataIterator() {};

      virtual void startIteration() = 0;
      virtual bool nextEntry() = 0;
      virtual const DocumentData* currentEntry() = 0;
      virtual bool finished() = 0;
    };
  }
}

#endif // INDRI_DOCUMENTDATAITERATOR_HPP

