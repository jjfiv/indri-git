
//
// RepositoryMaintenanceThread
//
// 31 January 2005 -- tds
//

#ifndef INDRI_REPOSITORYMAINTENANCETHREAD_HPP
#define INDRI_REPOSITORYMAINTENANCETHREAD_HPP

#include "indri/UtilityThread.hpp"
#include <queue>

class RepositoryMaintenanceThread : public UtilityThread {
private:
  enum { WRITE, MERGE, TRIM };
  class Repository& _repository;

  Mutex _requestLock;
  std::queue<int> _requests;
  UINT64 _memory;

public:
  RepositoryMaintenanceThread( class Repository& repository, UINT64 memory );

  UINT64 initialize();
  UINT64 work();
  bool hasWork();
  void deinitialize();

  void write();
  void merge();
};

#endif // INDRI_REPOSITORYMAINTENANCETHREAD_HPP

