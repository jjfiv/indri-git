
//
// DocExtentListIterator
//
// 24 November 2004 -- tds
//

#ifndef INDRI_DOCEXTENTLISTITERATOR_HPP
#define INDRI_DOCEXTENTLISTITERATOR_HPP

#include "Extent.hpp"
#include "indri/indri-platform.h"
#include "greedy_vector"

namespace indri {
  namespace index {
    class DocExtentListIterator {
    public:
      struct DocumentExtentData {
        int document;
        greedy_vector<Extent> extents;
        greedy_vector<INT64> numbers;
      };

      virtual ~DocExtentListIterator() {};
      
      virtual void startIteration() = 0;
      virtual bool finished() const = 0;
      virtual bool nextEntry() = 0;
      virtual bool nextEntry( int documentID ) = 0;
      virtual DocumentExtentData* currentEntry() = 0;
    };
  }
}

#endif // INDRI_DOCEXTENTLISTITERATOR_HPP
