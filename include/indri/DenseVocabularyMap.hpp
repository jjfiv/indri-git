
//
// DenseVocabularyMap
//
// 29 November 2004 -- tds
//

#ifndef INDRI_DENSEVOCABULARYMAP_HPP
#define INDRI_DENSEVOCABULARYMAP_HPP

class DenseVocabularyMap {
private:
  std::vector<int> _map;

public:
  int store( int oldTermID, int newTermID ) {
    if( _map.size() < oldTermID )
      _map.resize( oldTermID+1 );

    _map[oldTermID] = newTermID;
  }

  int operator[] ( int oldTermID ) {
    return get( oldTermID );
  }

  int get( int oldTermID ) {
    return _map[oldTermID];
  }
};

#endif // INDRI_DENSEVOCABULARYMAP_HPP

