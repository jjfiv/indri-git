
//
// RepositoryLoadThread
//
// 31 January 2005 -- tds
//

#ifndef INDRI_REPOSITORYLOADTHREAD_HPP
#define INDRI_REPOSITORYLOADTHREAD_HPP

#include "indri/UtilityThread.hpp"

class RepositoryLoadThread : public UtilityThread {
private:
  class Repository& _repository;
  UINT64 _memory;

public:
  RepositoryLoadThread( class Repository& repository, UINT64 memory );

  UINT64 initialize();
  void deinitialize();
  UINT64 work();
  bool hasWork();
};

#endif // INDRI_REPOSITORYLOADTHREAD_HPP


