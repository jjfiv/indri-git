
//
// DiskKeyfileVocabularyIterator
//
// 20 January 2005 -- tds
//

#ifndef INDRI_DISKKEYFILEVOCABULARYITERATOR_HPP
#define INDRI_DISKKEYFILEVOCABULARYITERATOR_HPP

#include "indri/DiskTermData.hpp"
#include "lemur/Keyfile.hpp"
#include "indri/VocabularyIterator.hpp"
#include "indri/Mutex.hpp"
#include "indri/BulkTree.hpp"

namespace indri {
  namespace index {
    class DiskKeyfileVocabularyIterator : public VocabularyIterator {
    private:
      DiskTermData* _diskTermData;
      int _baseID;
      BulkTreeReader& _keyfile;
      char _data[16*1024];
      int _fieldCount;

      Buffer _compressedData;
      Buffer _decompressedData;

      Mutex& _mutex;
      bool _holdingLock;
      bool _finished;

      void _acquire();
      void _release();

    public:
      DiskKeyfileVocabularyIterator( int baseID, BulkTreeReader& keyfile, Mutex& mutex, int fieldCount );
      
      void startIteration();
      bool nextEntry();
      indri::index::DiskTermData* currentEntry();
      bool finished();
    };
  }
};

#endif // INDRI_DISKKEYFILEVOCABULARYITERATOR_HPP

