
//
// RepositoryMaintenanceThread
//
// 31 January 2005 -- tds
//

#include "indri/RepositoryMaintenanceThread.hpp"
#include "indri/Repository.hpp"
#include "indri/ScopedLock.hpp"
#include <iostream>

const UINT64 TIME_DELAY = 15*1000*1000;
const UINT64 THRASHING_MERGE_DELAY = 300*1000*1000;
const int MAXIMUM_INDEX_COUNT = 50;


//
// maintenance_smoothed_load
//

static float maintenance_smoothed_load( Repository::Load& load ) {
  const float fifteenWeight = 0.2f;
  const float fiveWeight = 0.2f;
  const float oneWeight = 0.6f;

  return load.fifteen * fifteenWeight + load.five * fiveWeight + load.one * oneWeight;
}

//
// maintenance_should_merge
//

static bool maintenance_should_merge( Repository::index_state& state, Repository::Load& documentLoad, Repository::Load& queryLoad, UINT64 lastThrashing ) {
  float smoothedQueryLoad = maintenance_smoothed_load( queryLoad ) + 1;
  float smoothedDocumentLoad = maintenance_smoothed_load( documentLoad );
  float addRatio = smoothedDocumentLoad / (smoothedQueryLoad+1); 

  bool hasntThrashedRecently = lastThrashing > THRASHING_MERGE_DELAY;
  bool couldUseMerge = state->size() >= 2;
  bool significantQueryLoad = smoothedQueryLoad > 2;
  bool insignificantDocumentLoad = smoothedDocumentLoad < 1;
  int indexesToMerge = state->size(); 
  
  // extremely heuristic choice for when indexes should be merged:
  //   we merge if there are a lot of incoming queries relative to
  //   the amount of documents added; and this
  //   is all weighted by the number of indexes we have to merge.

  return couldUseMerge &&
         hasntThrashedRecently &&
         ((addRatio/50) < indexesToMerge && (significantQueryLoad || insignificantDocumentLoad));
}

//
// maintenance_should_trim
//

static bool maintenance_should_trim( Repository::index_state& state, Repository::Load& documentLoad, Repository::Load& queryLoad, UINT64 lastThrashing ) {
  return state->size() > MAXIMUM_INDEX_COUNT;
}

//
// constructor
//

RepositoryMaintenanceThread::RepositoryMaintenanceThread( Repository& repository, UINT64 memory ) :
  UtilityThread(),
  _repository( repository ),
  _memory( memory )
{
}

//
// initialize
//

UINT64 RepositoryMaintenanceThread::initialize() {
  return TIME_DELAY;
}

//
// deinitialize
//

void RepositoryMaintenanceThread::deinitialize() {
  // do nothing
}

//
// work
//

UINT64 RepositoryMaintenanceThread::work() {
  // fetch current index state
  bool write = false;
  bool merge = false;
  bool trim = false;

  {
    // lock the request queue
    ScopedLock l( _requestLock );

    // if nobody has any requests, check to see if we should be working
    if( ! _requests.size() ) {
      // get the memory size of the active memory index
      Repository::index_state state = _repository.indexes();
      indri::index::MemoryIndex* index = dynamic_cast<indri::index::MemoryIndex*>(state->back());

      if( index ) {
        // if the index is too big, we'd better get to work
        if( _memory < index->memorySize() ) {
          _requests.push( WRITE );
        }

        Repository::Load documentLoad = _repository.documentLoad();
        Repository::Load queryLoad = _repository.queryLoad();
        UINT64 lastThrashing = _repository._timeSinceThrashing();

        if( maintenance_should_merge( state, documentLoad, queryLoad, lastThrashing ) ) {
          _requests.push( MERGE );
        } else if( maintenance_should_trim( state, documentLoad, queryLoad, lastThrashing ) ) {
          _requests.push( TRIM );
        }
      }
    }

    // now, figure out what needs to be done
    while( _requests.size() ) {
      switch( _requests.front() ) {
      case MERGE:
        merge = true;
        break;

      case TRIM:
        trim = true;
        break;

      case WRITE:
        write = true;
        break;
      }

      _requests.pop();
    }
  
    // unlock request queue
  }

  _repository._setThrashing( false );

  if( merge ) {
    _repository._merge();
  } else if( trim ) {
    _repository._trim();
  } else if( write ) {
    _repository._write();
  }

  return TIME_DELAY;
}

//
// hasWork
//

bool RepositoryMaintenanceThread::hasWork() {
  ScopedLock l( _requestLock );
  return _requests.size() > 0;
}

//
// write
//

void RepositoryMaintenanceThread::write() {
  ScopedLock l( _requestLock );
  _requests.push( WRITE );
}

//
// merge
//

void RepositoryMaintenanceThread::merge() {
  ScopedLock l( _requestLock );
  _requests.push( MERGE );
}

