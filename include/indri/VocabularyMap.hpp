
//
// VocabularyMap
//
// 29 November 2004 -- tds
//

#ifndef INDRI_VOCABULARYMAP_HPP
#define INDRI_VOCABULARYMAP_HPP

namespace indri {
  namespace index {
    class VocabularyMap {
    public:
      virtual ~VocabularyMap() = 0;

      virtual void store( int oldTermID, int newTermID ) = 0;
      virtual int get( int oldTermID ) = 0;
      virtual int operator[] ( int oldTermID ) = 0;
    };
  }
}

#endif // INDRI_VOCABULARYMAP_HPP


