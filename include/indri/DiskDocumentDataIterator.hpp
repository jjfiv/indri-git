
//
// DiskDocumentDataIterator
//
// 21 January 2005 -- tds
//

#ifndef INDRI_DISKDOCUMENTDATAITERATOR_HPP
#define INDRI_DISKDOCUMENTDATAITERATOR_HPP

#include "indri/File.hpp"
#include "indri/SequentialReadBuffer.hpp"
#include "indri/DocumentDataIterator.hpp"
#include "indri/DocumentData.hpp"

namespace indri {
  namespace index {
    class DiskDocumentDataIterator : public DocumentDataIterator {
    private:
      SequentialReadBuffer* _readBuffer;
      UINT64 _fileSize;

      DocumentData _documentData;
      File& _documentDataFile;
      bool _finished;

    public:
      DiskDocumentDataIterator( File& documentDataFile );
      ~DiskDocumentDataIterator();

      void startIteration();
      bool nextEntry();
      const DocumentData* currentEntry();
      bool finished();
    };
  }
}

#endif // INDRI_DISKDOCUMENTDATAITERATOR_HPP

