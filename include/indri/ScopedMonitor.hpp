
//
// ScopedMonitor
//
// 15 November 2004 -- tds
//

#ifndef INDRI_SCOPEDMONITOR_HPP
#define INDRI_SCOPEDMONITOR_HPP

#include "indri/Mutex.hpp"
#include "indri/ConditionVariable.hpp"

class ScopedMonitor {
private:
  Mutex& _mutex;
  ConditionVariable& _condition;

public:
  ScopedMonitor( Mutex& mutex, ConditionVariable& condition ) :
    _mutex(mutex),
    _condition(condition)
  {
    _mutex.lock();
  }

  ~ScopedMonitor() {
    _mutex.unlock();
  }

  void wait() {
    _condition.wait( _mutex );
  }

  void notifyOne() {
    _condition.notifyOne();
  }

  void notifyAll() {
    _condition.notifyAll();
  }
};

#endif // INDRI_SCOPEDMONITOR_HPP

