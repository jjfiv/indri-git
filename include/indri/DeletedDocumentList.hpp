
//
// DeletedDocumentList
//
// 3 February 2005 -- tds
//
// While vector<bool> might have been a nice clean choice for this
// bitmap, there was no platform-independent way to just blit it
// to disk.  I decided to roll my own using a Buffer.
//

#ifndef INDRI_DELETEDDOCUMENTLIST_HPP
#define INDRI_DELETEDDOCUMENTLIST_HPP

#include <vector>
#include "indri/ReadersWritersLock.hpp"
#include "indri/ReaderLockable.hpp"
#include "indri/WriterLockable.hpp"
#include "indri/Buffer.hpp"

class DeletedDocumentList {
private:
  ReadersWritersLock _lock;
  ReaderLockable _readLock;
  WriterLockable _writeLock;

  Buffer _bitmap;
  void _grow( int documentID );

public:
  class read_transaction {
  private:
    ReadersWritersLock& _lock;
    Buffer& _bitmap;

  public:
    read_transaction( DeletedDocumentList& list );
    ~read_transaction();

    int nextCandidateDocument( int documentID );
    bool isDeleted( int documentID );
  };

  DeletedDocumentList();

  void markDeleted( int documentID );
  bool isDeleted( int documentID );
  read_transaction* getReadTransaction();

  void read( const std::string& filename );
  void write( const std::string& filename );
};

#endif // INDRI_DELETEDDOCUMENTLIST_HPP


