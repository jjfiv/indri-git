
//
// ReaderLockable
//
// 31 January 2005 -- tds
//
// Lockable adaptor for ReadersWritersLocks
//

#ifndef INDRI_READERLOCKABLE_HPP
#define INDRI_READERLOCKABLE_HPP

#include "indri/ReadersWritersLock.hpp"
#include "indri/Thread.hpp"

class ReaderLockable : public Lockable {
private:
  ReadersWritersLock& _lock;

public:
  ReaderLockable( ReadersWritersLock& lock ) :
    _lock( lock )
  {
  }

  void lock() {
    _lock.lockRead();
  }

  void unlock() {
    _lock.unlockRead();
  }
};

#endif // INDRI_READERLOCKABLE_HPP

