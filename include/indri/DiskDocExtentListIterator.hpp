
//
// DiskDocExtentListIterator
//
// 13 December 2004 -- tds
//

#ifndef INDRI_DISKDOCEXTENTLISTITERATOR_HPP
#define INDRI_DISKDOCEXTENTLISTITERATOR_HPP

#include "indri/SequentialReadBuffer.hpp"
#include "indri/DocExtentListIterator.hpp"

namespace indri {
  namespace index {
    class DiskDocExtentListIterator : public DocExtentListIterator {
    private:
      const char* _list;
      const char* _listEnd;
      int _skipDocument;

      SequentialReadBuffer* _file;
      UINT64 _startOffset;
      bool _numeric;

      DocumentExtentData _data;
      bool _finished;

      void _readEntry();
      void _readSkip();

    public:
      DiskDocExtentListIterator( SequentialReadBuffer* buffer, UINT64 startOffset );
      ~DiskDocExtentListIterator();
      
      void setStartOffset( UINT64 startOffset );

      bool finished() const;
      void startIteration();
      bool nextEntry();
      bool nextEntry( int documentID );
      DocumentExtentData* currentEntry();
    };
  }
}

#endif // INDRI_DISKDOCEXTENTLISTITERATOR_HPP

