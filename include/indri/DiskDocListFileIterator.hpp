
//
// DiskDocListFileIterator
//
// 13 December 2004 -- tds
//

#ifndef INDRI_DISKDOCLISTFILEITERATOR_HPP
#define INDRI_DISKDOCLISTFILEITERATOR_HPP

#include "indri/DocListFileIterator.hpp"
#include "indri/DiskDocListIterator.hpp"
#include "indri/Buffer.hpp"
#include "lemur/Keyfile.hpp"
#include "indri/SequentialReadBuffer.hpp"
#include "indri/File.hpp"

namespace indri {
  namespace index {
    class DiskDocListFileIterator : public DocListFileIterator {
    private:
      SequentialReadBuffer* _file;
      UINT64 _fileLength;

      int _fieldCount;
      Buffer _header;

      char _term[ Keyfile::MAX_KEY_LENGTH+1 ];
      TermData* _termData;
      DiskDocListIterator _iterator;
      DocListData _docListData;
      bool _finished;

      void _readEntry();

    public:
      DiskDocListFileIterator( File& docListFile, int fieldCount );
      ~DiskDocListFileIterator();
      
      bool finished() const;
      void startIteration();

      bool nextEntry();
      DocListData* currentEntry();
      const DocListData* currentEntry() const;
    };
  }
}

#endif // INDRI_DISKDOCLISTFILEITERATOR_HPP

