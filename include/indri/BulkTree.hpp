
//
// BulkTree
//
// 4 March 2005 -- tds
//

#ifndef INDRI_BULKTREE_HPP
#define INDRI_BULKTREE_HPP

#include <vector>
#include "indri/File.hpp"
#include "indri/SequentialWriteBuffer.hpp"
#include "indri/HashTable.hpp"

class BulkBlock {
private:
  char* _buffer;

  BulkBlock* _previous;
  BulkBlock* _next;
  UINT32 _id;

  int _remainingCapacity();
  int _dataEnd();
  int _keyEnd( int index );
  int _keyStart( int index );
  int _valueStart( int index );
  int _valueEnd( int index );

  bool _canInsert( int keyLength, int dataLength );
  void _storeKeyValueLength( int insertPoint, int keyLength, int valueLength );
  int _compare( const char* one, int oneLength, const char* two, int twoLength );

  int _find( const char* key, int keyLength, bool& exact );

public:
  BulkBlock( bool leaf = false );
  ~BulkBlock();

  int count();
  bool leaf();

  bool insert( const char* key, int keyLength, const char* data, int dataLength );
  bool insertFirstKey( BulkBlock& block, UINT32 blockID );
  bool getIndex( int index, char* key, int& keyActual, int keyLength, char* value, int& valueActual, int valueLength );
  bool findGreater( const char* key, int keyLength, char* value, int& actualLength, int valueBufferLength );
  bool find( const char* key, int keyLength, char* value, int& actualLength, int valueBufferLength );
  void clear();
  char* data();
  static int dataSize();

  // linked list
  void setID( UINT32 id );
  UINT32 getID();
  void link( BulkBlock* previous, BulkBlock* next );
  void unlink();
  BulkBlock* previous();
  BulkBlock* next();
};


class BulkTreeWriter {
private:
  UINT32 _blockID;
  int _flushLevel;

  void _flush( int blockIndex );
  void _flushAll();

public:
  std::vector<BulkBlock*> _blocks;
  SequentialWriteBuffer _write;
  File _file;

  BulkTreeWriter();
  ~BulkTreeWriter();

  void close();
  void create( const std::string& filename );

  void put( const char* key, const char* value, int valueLength );
  void put( UINT32 key, const char* value, int valueLength );
  void put( const char* key, int keyLength, const char* value, int valueLength );

  // this is a hack for now
  bool get( const char* key, int keyLength, char* value, int& actual, int valueLength );
  bool get( UINT32 key, char* value, int& actual, int valueLength );
  bool get( const char* key, char* value, int& actual, int valueLength );

  void flush();
};

class BulkTreeReader {
private:
  File* _file;
  UINT64 _fileLength;
  bool _ownFile;
  
  BulkBlock* _head;
  BulkBlock* _tail;
  HashTable< UINT32, BulkBlock* > _cache;

  BulkBlock* _fetch( UINT32 id );

public:
  BulkTreeReader::BulkTreeReader();
  BulkTreeReader( File& file );
  BulkTreeReader( File& file, UINT64 length );
  ~BulkTreeReader();
  
  void openRead( const std::string& filename );
  bool get( const char* key, char* value, int& actual, int valueLength );
  bool get( const char* key, int keyLength, char* value, int& actual, int valueLength );
  bool get( UINT32 key, char* value, int& actual, int valueLength );
  void close();
};

#endif // INDRI_BULKTREE_HPP
