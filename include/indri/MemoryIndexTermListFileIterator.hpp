
//
// MemoryIndexTermListFileIterator
//
// 24 November 2004 -- tds
//

#ifndef INDRI_MEMORYINDEXTERMLISTFILEITERATOR_HPP
#define INDRI_MEMORYINDEXTERMLISTFILEITERATOR_HPP

#include <list>
#include <vector>
#include "indri/Buffer.hpp"
#include "indri/indri-platform.h"
#include "indri/DocumentData.hpp"
#include "indri/TermList.hpp"
#include "indri/TermListFileIterator.hpp"

namespace indri {
  namespace index {
    class MemoryIndexTermListFileIterator : public TermListFileIterator {
    private:
      // the buffers contain compressed TermLists, packed together in 1MB memory regions
      // the _buffersIterator points to the last buffer we looked at, while _bufferBase
      // contains the number of bytes in all previous buffers
      std::list<Buffer*>& _buffers;
      std::list<Buffer*>::iterator _buffersIterator;
      UINT64 _bufferBase;
      bool _finished;

      std::vector<DocumentData>& _data;
      TermList _list;
      int _index; // index into _documentData of the current document

    public:
      MemoryIndexTermListFileIterator( std::list<Buffer*>& buffers, std::vector<DocumentData>& data );

      void startIteration();
      bool nextEntry();
      bool finished();
      TermList* currentEntry();
    };
  }
}

#endif // INDRI_MEMORYINDEXTERMLISTFILEITERATOR_HPP

