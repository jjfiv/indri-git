
//
// Thread
//
// 15 November 2004 -- tds
//

#ifndef INDRI_THREAD_HPP
#define INDRI_THREAD_HPP

#include "indri/indri-platform.h"

#ifndef WIN32
#include <pthread.h>
#endif

class Thread {
private:
#ifdef WIN32
  unsigned int _id;
  uintptr_t _thread;
#else
  pthread_t _thread;
#endif
  void (*_function)( void* data );
  void* _data;

public:
  Thread( void (*function)(void*), void* data );
  void execute();
  void join();

  static int id();
  static void sleep( int milliseconds );
  static void yield();
};

#endif // INDRI_THREAD_HPP

