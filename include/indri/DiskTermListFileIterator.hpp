
//
// DiskTermListFileIterator
//
// 25 January 2005 -- tds
//

#ifndef INDRI_DISKTERMLISTFILEITERATOR_HPP
#define INDRI_DISKTERMLISTFILEITERATOR_HPP

#include "indri/TermListFileIterator.hpp"
#include "indri/SequentialReadBuffer.hpp"
#include "indri/TermList.hpp"

namespace indri {
  namespace index {
    class DiskTermListFileIterator : public TermListFileIterator {
    private:
      TermList _termList;
      File& _termListFile;
      SequentialReadBuffer _buffer;
      UINT64 _fileSize;
      bool _finished;

    public:
      DiskTermListFileIterator( File& termListFile );

      void startIteration();
      TermList* currentEntry();
      bool nextEntry();
      bool finished();
    };
  }
}

#endif // INDRI_DISKTERMLISTFILEITERATOR_HPP

