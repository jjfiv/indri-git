
//
// WriterLockable
//
// 31 January 2005 -- tds
//
// Lockable adaptor for ReadersWritersLocks
//

#ifndef INDRI_WRITERLOCKABLE_HPP
#define INDRI_WRITERLOCKABLE_HPP

#include "indri/ReadersWritersLock.hpp"

class WriterLockable : public Lockable {
private:
  ReadersWritersLock& _lock;

public:
  WriterLockable( ReadersWritersLock& lock ) :
    _lock( lock )
  {
  }

  void lock() {
    _lock.lockWrite();
  }

  void unlock() {
    _lock.unlockWrite();
  }
};

#endif // INDRI_WRITERLOCKABLE_HPP



