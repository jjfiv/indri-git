
//
// DiskFrequentVocabularyIterator
//
// 19 January 2005 -- tds
//

#ifndef INDRI_DISKFREQUENTVOCABULARYITERATOR_HPP
#define INDRI_DISKFREQUENTVOCABULARYITERATOR_HPP

#include "indri/File.hpp"
#include "indri/Buffer.hpp"
#include "indri/TermData.hpp"
#include "indri/VocabularyIterator.hpp"
#include "indri/RVLDecompressStream.hpp"

namespace indri {
  namespace index {
    class DiskFrequentVocabularyIterator : public VocabularyIterator {
    private:
      File& _file;
      RVLDecompressStream _stream;
      Buffer _buffer;
      const char* _current;
      int _fieldCount;
      bool _finished;

      DiskTermData* _data;
      char _dataBuffer[16*1024];

    public:
      DiskFrequentVocabularyIterator( File& frequentTermsData, int fieldCount ); 
      ~DiskFrequentVocabularyIterator() {};
      
      void startIteration();
      bool finished();
      bool nextEntry();
      DiskTermData* currentEntry();
    };
  } 
}

#endif // INDRI_DISKFREQUENTVOCABULARYITERATOR_HPP

