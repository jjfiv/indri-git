
//
// ScopedLock
//
// 15 November 2004 -- tds
//

#ifndef INDRI_SCOPEDLOCK_HPP
#define INDRI_SCOPEDLOCK_HPP

class ScopedLock {
private:
  Lockable* _lockable;

public:
  ScopedLock( Lockable& lockable ) {
    _lockable = &lockable;
    _lockable->lock();
  }

  ScopedLock( Lockable* lockable ) {
    _lockable = lockable;

    if( _lockable )
      _lockable->lock();
  }

  ~ScopedLock() {
    if( _lockable )
      _lockable->unlock();
  }

  void unlock() {
    if( _lockable )
      _lockable->unlock();
    _lockable = 0;
  }
};

#endif // INDRI_SCOPEDLOCK_HPP

