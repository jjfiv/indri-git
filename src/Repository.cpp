/*==========================================================================
 * Copyright (c) 2004 University of Massachusetts.  All Rights Reserved.
 *
 * Use of the Lemur Toolkit for Language Modeling and Information Retrieval
 * is subject to the terms of the software license set forth in the LICENSE
 * file included with this software, and also available at
 * http://www.lemurproject.org/license.html
 *
 *==========================================================================
*/

//
// Repository
//
// 21 May 2004 -- tds
//

#include "indri/Repository.hpp"
#include "indri/MemoryIndex.hpp"
#include "indri/CompressedCollection.hpp"
#include "indri/Path.hpp"
#include "indri/PorterStemmerTransformation.hpp"
#include "indri/KrovetzStemmerTransformation.hpp"
#include "indri/StopperTransformation.hpp"
#include "indri/NumericFieldAnnotator.hpp"
#include "indri/Parameters.hpp"
#include "indri/StemmerFactory.hpp"
#include "indri/NormalizationTransformation.hpp"
#include "lemur/Exception.hpp"
#include "indri/Thread.hpp"
#include "indri/IndexWriter.hpp"
#include "indri/DiskIndex.hpp"
#include "indri/ScopedLock.hpp"
#include "indri/RepositoryLoadThread.hpp"
#include "indri/RepositoryMaintenanceThread.hpp"
#include "indri/IndriTimer.hpp"
#include <math.h>
#include <string>
#include <algorithm>

const static int defaultMemory = 100*1024*1024;

//
// _fieldsForIndex
//

std::vector<indri::index::Index::FieldDescription> Repository::_fieldsForIndex( std::vector<Repository::Field>& _fields ) {
  std::vector<indri::index::Index::FieldDescription> result;

  for( size_t i=0; i<_fields.size(); i++ ) {
    indri::index::Index::FieldDescription fdesc;
    
    fdesc.name = _fields[i].name;
    fdesc.numeric = _fields[i].numeric;

    result.push_back(fdesc);
  }

  return result;
}

//
// _buildFields
//

void Repository::_buildFields() {
  if( _parameters.exists("field") ) {
    Parameters fields = _parameters["field"];

    for( size_t i=0; i<fields.size(); i++ ) {
      Field field;

      field.name = fields[i].get( "name", "" );
      field.numeric = fields[i].get( "numeric", false ) ? true : false;
      field.parserName = fields[i].get( "parserName", "" );

      _fields.push_back(field);
    }
  }

  _indexFields = _fieldsForIndex( _fields );
}

//
// _buildTransientChain
//

void Repository::_buildTransientChain( Parameters& parameters ) {
  if( parameters.exists("stopper.word") ) {
    Parameters stop = parameters["stopper.word"];
    _transformations.push_back( new StopperTransformation( stop ) );
  }
}

//
// _buildChain
//

void Repository::_buildChain( Parameters& parameters ) {
  bool dontNormalize = parameters.exists( "normalize" ) && ( false == (bool) parameters["normalize"] );

  if( dontNormalize == false ) {
    _transformations.push_back( new NormalizationTransformation() );
  }

  for( size_t i=0; i<_fields.size(); i++ ) {
    if( _fields[i].parserName == "NumericFieldAnnotator" ) {
      _transformations.push_back( new NumericFieldAnnotator( _fields[i].name ) );
    }
  }

  if( _parameters.exists("stopper.word") ) {
    Parameters stop = _parameters["stopper.word"];
    _transformations.push_back( new StopperTransformation( stop ) );
  }

  if( _parameters.exists("stemmer.name") ) {
    std::string stemmerName = std::string(_parameters["stemmer.name"]);
    Parameters stemmerParams = _parameters["stemmer"];
    _transformations.push_back( StemmerFactory::get( stemmerName, stemmerParams ) );
  }
}

//
// _copyParameters
//

void Repository::_copyParameters( Parameters& options ) {
  if( options.exists( "normalize" ) ) {
    _parameters.set( "normalize", (std::string) options["normalize"] );
  }

  if( options.exists("field") ) {
    _parameters.set( "field", "" );
    _parameters["field"] = options["field"];
  }

  if( options.exists("stopper") ) {
    _parameters.set( "stopper", "" );
    _parameters["stopper"] = options["stopper"];
  }

  if( options.exists("stemmer") ) {
    _parameters.set( "stemmer", "" );
    _parameters["stemmer"] = options["stemmer"];
  }
}

//
// _remove
//
// In the future, this will remove a directory asynchronously,
// and will be cancellable.
//

void Repository::_remove( const std::string& indexPath ) {
  Path::remove( indexPath );
}

//
// _openIndexes
//

void Repository::_openIndexes( Parameters& params, const std::string& parentPath ) {
  Parameters indexes = params["indexes"]["index"];

  //
  // <!ELEMENT name (#PCDATA)> -- name of index
  // <!ELEMENT prune (#PCDATA)> -- this index is junk and should be pruned
  // <!ELEMENT index (name,deleteable)>
  //

  _active = new index_vector;
  _states.push_back( _active );

  for( int i=0; i<indexes.size(); i++ ) {
    Parameters indexSpec = indexes[i];
    indri::index::DiskIndex* diskIndex = new indri::index::DiskIndex();
    std::string indexName = (std::string) indexSpec;

    if( indexSpec.get("prune", 0) ) {
      _remove( indexName );
    } else {
      diskIndex->open( parentPath, indexName );
      _active->push_back( diskIndex );
    }
  }

  _indexCount = params.get( "indexCount", 0 );
}

//
// countQuery
//
// Counts each document add--useful for load average computation.
//

void Repository::countQuery() {
  indri::atomic::increment( _queryLoad[0] );
}

//
// _countDocumentAdd
//
// Counts each document add--useful for load average computation.
//

void Repository::_countDocumentAdd() {
  indri::atomic::increment( _documentLoad[0] );
}

//
// _incrementLoad
//
// Called four times a minute by a timer thread to update the load average
//

void Repository::_incrementLoad() {
  memmove( (void*) &_documentLoad[1], (void*) &_documentLoad[0], (sizeof _documentLoad[0]) * (LOAD_MINUTES * LOAD_MINUTE_FRACTION - 1) );
  memmove( (void*) &_queryLoad[1], (void*) &_queryLoad[0], (sizeof _queryLoad[0]) * (LOAD_MINUTES * LOAD_MINUTE_FRACTION - 1) );

  _documentLoad[0] = 0;
  _queryLoad[0] = 0;
}

//
// _computeLoad
//

Repository::Load Repository::_computeLoad( indri::atomic::value_type* loadArray ) {
  Load load;

  load.one = load.five = load.fifteen = 0;

  for( int i=0; i<LOAD_MINUTE_FRACTION; i++ ) {
    load.one += loadArray[i];
  }

  for( int i=0; i<5*LOAD_MINUTE_FRACTION; i++ ) { 
    load.five += loadArray[i];
  }
  load.five /= 5.;

  for( int i=0; i<15*LOAD_MINUTE_FRACTION; i++ ) {
    load.fifteen += loadArray[i];
  }
  load.fifteen /= 15.;

  return load;
}

//
// queryLoad
//

Repository::Load Repository::queryLoad() {
  return _computeLoad( _queryLoad );
}

//
// documentLoad
//

Repository::Load Repository::documentLoad() {
  return _computeLoad( _documentLoad );
}

//
// create
//

void Repository::create( const std::string& path, Parameters* options ) {
  _path = path;
  _readOnly = false;

  try {
    if( !Path::exists( path ) ) {
      Path::create( path );
    } else {
      // remove any existing cruft
      Path::remove( path );
      Path::create( path );
    }
    
    _memory = defaultMemory;
    if( options )
      _memory = options->get( "memory", _memory );

    float queryProportion = 0.15f;
    if( options )
      queryProportion = static_cast<float>(options->get( "queryProportion", queryProportion ));

    if( options )
      _copyParameters( *options );

    _buildFields();
    _buildChain( _parameters );

    std::string indexPath = Path::combine( path, "index" );
    std::string collectionPath = Path::combine( path, "collection" );

    if( !Path::exists( indexPath ) )
      Path::create( indexPath );

    std::string indexName = Path::combine( indexPath, "index" );

    _active = new index_vector;
    _states.push_back( _active );
    _active->push_back( new indri::index::MemoryIndex( 1, _indexFields ) );
    _indexCount = 0;

    _collection = new CompressedCollection();

    if( !Path::exists( collectionPath ) )
      Path::create( collectionPath );

    std::vector<std::string> forwardFields;
    std::vector<std::string> backwardFields;

    if( options && options->exists( "collection.forward" ) ) {
      Parameters cfields = options->get( "collection.forward" );

      for( size_t i=0; i<cfields.size(); i++ ) {
        forwardFields.push_back( (std::string) cfields[i] );
      }
    }

    if( options && options->exists( "collection.backward" ) ) {
      Parameters cfields = options->get( "collection.backward" );

      for( size_t i=0; i<cfields.size(); i++ ) {
        backwardFields.push_back( (std::string) cfields[i] );
      }
    }

    _collection->create( collectionPath, forwardFields, backwardFields );

    _startThreads();
  } catch( Exception& e ) {
    LEMUR_RETHROW( e, "Couldn't create a repository at '" + path + "' because:" );
  } catch( ... ) {
    LEMUR_THROW( LEMUR_RUNTIME_ERROR, "Something unexpected happened while trying to create '" + path + "'" );
  }
}

//
// openRead
//

void Repository::openRead( const std::string& path, Parameters* options ) {
  _path = path;
  _readOnly = true;

  _memory = defaultMemory;
  if( options )
    _memory = options->get( "memory", _memory );

  float queryProportion = 1;
  if( options )
    queryProportion = static_cast<float>(options->get( "queryProportion", queryProportion ));

  _parameters.loadFile( Path::combine( path, "manifest" ) );

  _buildFields();
  _buildChain( _parameters );

  if( options )
    _buildTransientChain( *options );

  std::string indexPath = Path::combine( path, "index" );
  std::string collectionPath = Path::combine( path, "collection" );
  std::string indexName = Path::combine( indexPath, "index" );
  std::string deletedName = Path::combine( path, "deleted" );

  _openIndexes( _parameters, indexPath );

  _collection = new CompressedCollection();
  _collection->openRead( collectionPath );
  _deletedList.read( deletedName );

  _startThreads();
}

//
// open
//

void Repository::open( const std::string& path, Parameters* options ) {
  _path = path;
  _readOnly = false;

  _memory = defaultMemory;
  if( options )
    _memory = options->get( "memory", _memory );

  float queryProportion = 0.75;
  if( options )
    queryProportion = static_cast<float>(options->get( "queryProportion", queryProportion ));

  std::string indexPath = Path::combine( path, "index" );
  std::string collectionPath = Path::combine( path, "collection" );
  std::string indexName = Path::combine( indexPath, "index" );

  _parameters.loadFile( Path::combine( path, "manifest" ) );

  _buildFields();
  _buildChain( _parameters );

  if( options )
    _buildTransientChain( *options );

  // open all indexes, add a memory index
  _openIndexes( _parameters, indexPath );
  _addMemoryIndex();

  // remove that initial state (only disk indexes)
  _states.erase( _states.begin() );

  // open compressed collection
  _collection = new CompressedCollection();
  _collection->open( collectionPath );
  
  // read deleted documents in
  std::string deletedName = Path::combine( path, "deleted" );
  _deletedList.read( deletedName );

  _startThreads();
}

//
// exists
//

bool Repository::exists( const std::string& path ) {
  std::string manifestPath = Path::combine( path, "manifest" );
  return Path::exists( manifestPath );
}

//
// addDocument
//

void Repository::addDocument( ParsedDocument* document ) {
  if( _readOnly )
    LEMUR_THROW( LEMUR_RUNTIME_ERROR, "addDocument: Cannot add documents to a repository that is opened for read-only access." ); 

  while( _thrashing ) {
    Thread::sleep( 100 );
  }

  ScopedLock lock( _addLock );

  for( size_t i=0; i<_transformations.size(); i++ ) {
    document = _transformations[i]->transform( document );
  }

  index_state state;

  { 
    // get a copy of current index state
    ScopedLock stateLock( _stateLock );
    state = _active;
  }

  int documentID = dynamic_cast<indri::index::MemoryIndex*>(state->back())->addDocument( *document );
  _collection->addDocument( documentID, document );

  _countDocumentAdd();
}

//
// deleteDocument
//

void Repository::deleteDocument( int documentID ) {
  _deletedList.markDeleted( documentID );
}

//
// _addMemoryIndex
//
// Add a new MemoryIndex to accept all new updates.  This allows
// the current MemoryIndex to be written to disk.
//

void Repository::_addMemoryIndex() {
  ScopedLock alock( _addLock );
  ScopedLock slock( _stateLock );

  // build a new memory index
  indri::index::Index* activeIndex = _active->back();
  int documentBase = activeIndex->documentBase() + activeIndex->documentCount();
  indri::index::MemoryIndex* newMemoryIndex = new indri::index::MemoryIndex( documentBase, _indexFields );

  // build a new state vector
  index_state newState = new index_vector;
  newState->assign( _active->begin(), _active->end() );
  newState->push_back( newMemoryIndex );

  // add the new state vector to the active states
  _states.push_back( newState );
  _active = newState;
}

//
// _swapState
//
// Make a new state object, swap in the new index for the old one
//

void Repository::_swapState( std::vector<indri::index::Index*>& oldIndexes, indri::index::Index* newIndex ) {
  ScopedLock lock( _stateLock );

  index_state oldState = _active;
  _active = new index_vector;

  int i;
  // copy all states up to oldIndexes
  for( i=0; i<oldState->size() && (*oldState)[i] != oldIndexes[0]; i++ ) {
    _active->push_back( (*oldState)[i] );
  }

  int firstMatch = i;

  // verify (in debug builds) that all the indexes match up like they should
  for( ; i<oldState->size() && (i-firstMatch) < oldIndexes.size(); i++ ) {
    assert( (*oldState)[i] == oldIndexes[i-firstMatch] );
  }

  // add the new index
  _active->push_back( newIndex );

  // copy all trailing indexes
  for( ; i<oldState->size(); i++ ) {
    _active->push_back( (*oldState)[i] );
  }

  _states.push_back( _active );
}

//
// _removeStates
//
// Remove a certain number of states from the _states vector
//

void Repository::_removeStates( std::vector<index_state>& toRemove ) {
  for( int i=0; i<toRemove.size(); i++ ) {
    std::vector<index_state>::iterator iter;

    for( iter = _states.begin(); iter != _states.end(); iter++ ) {
      if( (*iter) == toRemove[i] ) {
        _states.erase( iter );
        break;
      }
    }
  }
}

//
// _stateContains
//
// Returns true if the state contains any one of the given indexes
//

bool Repository::_stateContains( index_state& state, std::vector<indri::index::Index*>& indexes ) {
  // for every index in this state
  for( int j=0; j<state->size(); j++ ) {
    // does it match one of our indexes?
    for( int k=0; k<indexes.size(); k++ ) {
      if( (*state)[j] == indexes[k] ) {
        return true;
      }
    }
  }

  // no match
  return false;
}

//
// _statesContaining
//
// Find all states that contain any of these indexes
//

std::vector<Repository::index_state> Repository::_statesContaining( std::vector<indri::index::Index*>& indexes ) {
  ScopedLock lock( _stateLock );
  std::vector<index_state> result;

  // for every current state
  for( int i=0; i<_states.size(); i++ ) {
    index_state& state = _states[i];

    if( _stateContains( state, indexes ) )
      result.push_back( state );
  }
  
  return result;
}

//
// _closeIndexes
//

void Repository::_closeIndexes() {
  // we assume we don't need locks, because the one running
  // the repository has stopped all queries and document adds, etc.

  // drops all states except active to reference count 0, so they get deleted
  _states.clear();

  for( int i=0; i<_active->size(); i++ ) {
    (*_active)[i]->close();
    delete (*_active)[i];
  }

  // deletes the active state
  _active = 0;
}

void print_index_state( std::vector<Repository::index_state>& states ) {
  for( int i=0; i<states.size(); i++ ) {
    for( int j=0; j<states[i]->size(); j++ ) {
      std::cout << i << " " << (*states[i])[j] << std::endl;
    }
  }
}

//
// write
//
// Write the most recent memory index to disk, then swap that
// index back in as a disk index
//

void Repository::_write() {
  // this is only legal if we're not readOnly
  if( _readOnly )
    return;

  // grab a copy of the current state
  index_state state = indexes();
  
  // if the current index is empty, don't need to write it
  if( state->size() && state->back()->documentCount() == 0 )
    return;

  // make a new MemoryIndex, cutting off the old one from updates
  _addMemoryIndex();

  // if we just added the first, no need to write the "old" one
  if( state->size() == 0 )
    return;

  // write out the last index
  index_state lastState = new std::vector<indri::index::Index*>;
  lastState->push_back( state->back() );
  state = 0;

  _merge( lastState );
}

//
// _trim
//
// Merge together recent indexes.
//

void Repository::_trim() {
  // this is only legal if we're not readOnly
  if( _readOnly )
    return;

  // grab a copy of the current state
  index_state state = indexes();

  if( state->size() <= 3 )
    return;

  int count = state->size();
  int position;

  // here's how this works:
  //   we're trying to just 'trim' the indexes so that we merge
  //   together the small indexes, leaving the large ones as they are.
  //   We merge together a minimum of 3 indexes every time.  We may merge
  //   more, however.  We start at the most recent indexes, and search
  //   backward in time.  When we get to an index that is significantly
  //   larger than the previous index, we stop.
  //

  // have to merge at least the last three indexes
  int firstDocumentCount = (*state)[count-1]->documentCount();
  int lastDocumentCount = (*state)[count-3]->documentCount();
  int documentCount = 0;
 
  // move back until we find a really big index--don't merge with that one
  for( position = count-4; position>=0; position-- ) {
    // compute the average number of documents in the indexes we've seen so far
    documentCount = (*state)[position]->documentCount();

    // break if we find an index more than 50% larger than the last one 
    if( documentCount > lastDocumentCount*1.5 && 
        documentCount > firstDocumentCount*4 )
    {
      position++;
      break;
    }

    lastDocumentCount = documentCount;
  }

  // make sure position is greater than or equal to 0
  position = lemur_compat::max<int>( position, 0 );

  // make a new MemoryIndex, cutting off the old one from updates
  _addMemoryIndex();

  // write out the last index
  index_state substate = new std::vector<indri::index::Index*>;
  substate->assign( state->begin() + position, state->end() );
  state = 0;

  _merge( substate );
}

//
// _merge
//
// Merge the specified indexes together.
//

void Repository::_merge( index_state& state ) {
  // this is only legal if we're not readOnly
  if( _readOnly )
    return;

  // make a copy of the indexes in our state
  std::vector<indri::index::Index*> indexes = *(state.get());

  // get an index count
  std::stringstream indexNumber;
  indexNumber << _indexCount;
  _indexCount++;

  // make a path, write the index
  std::string indexPath = Path::combine( _path, "index" );
  std::string newIndexPath = Path::combine( indexPath, indexNumber.str() );
  indri::index::IndexWriter writer;
  writer.write( indexes, _indexFields, newIndexPath );

  // open the index we just wrote
  indri::index::DiskIndex* diskIndex = new indri::index::DiskIndex();
  diskIndex->open( indexPath, indexNumber.str() );

  // make a new state, replacing the old index for the new one
  _swapState( indexes, diskIndex );

  // drop our reference to the old state
  state = 0;

  // need a list of all states that contain the memoryIndex we just wrote
  // we want to wait here until the refcounts all drop to 1
  std::vector<index_state> containing = _statesContaining( indexes );

  while( 1 ) {
    bool referencesExist = false;
    for( int i=0; i<containing.size(); i++ ) {
      // we allow one reference in the _states vector, and one in the containing vector
      referencesExist = referencesExist || containing[i].references() > 2;
    }

    if( !referencesExist )
      break;

    // wait a little bit
    Thread::sleep( 100 );
  }

  // okay, now nobody is using the state, so we can get rid of those states
  // and the index we wrote
  for( int i=0; i<indexes.size(); i++ ) {
    indri::index::DiskIndex* diskIndex = dynamic_cast<indri::index::DiskIndex*>(indexes[i]);
    std::string path;

    // trap the path, if this is a diskIndex
    if( diskIndex ) {
      path = diskIndex->path();
      std::string root = Path::combine( _path, "index" );
      path = Path::combine( root, path );
    }

    // delete the index object
    indexes[i]->close();
    delete indexes[i];

    // if it was a disk index, remove the data
    if( diskIndex ) {
      Path::remove( path );
    }
  }

  // remove all containing states
  _removeStates( containing );
}

//
// merge
//
// Merge all known indexes together
//

void Repository::_merge() {
  // this is only legal if we're not readOnly
  if( _readOnly )
    return;

  // grab a copy of the current state
  index_state state = indexes();
  index_state mergers = state;

  if( state->size() && state->back()->documentCount() == 0 ) {
    // if the current index is empty, don't need to add a new one; write the others
    mergers = new index_vector;
    mergers->assign( state->begin(), state->end() - 1 );
  } else {
    // current index isn't empty, so add a new one and write the old ones
    _addMemoryIndex();
  }

  // no need to merge when there's only one index (or none)
  bool needsWrite = (mergers->size() > 1) ||
                    (mergers->size() == 1 && dynamic_cast<indri::index::MemoryIndex*>((*mergers)[0]));

  if( !needsWrite )
    return;

  state = 0;

  // merge all the indexes together
  _merge( mergers );
}

//
// fields
//

const std::vector<Repository::Field>& Repository::fields() const {
  return _fields;
}

//
// tags
//

std::vector<std::string> Repository::tags() const {
  std::vector<std::string> t;

  for( size_t i=0; i<_fields.size(); i++ ) {
    t.push_back(_fields[i].name);
  }

  return t;
}

//
// processTerm
//

std::string Repository::processTerm( const std::string& term ) {
  ParsedDocument original;
  ParsedDocument* document;
  std::string result;
  char termBuffer[256];
  assert( term.length() < sizeof termBuffer );
  strcpy( termBuffer, term.c_str() );

  original.text = termBuffer;
  original.textLength = strlen(termBuffer)+1;

  original.terms.push_back( termBuffer );
  document = &original;
  
  for( size_t i=0; i<_transformations.size(); i++ ) {
    document = _transformations[i]->transform( document );    
  }
  
  if( document->terms[0] )
    result = document->terms[0];

  return result;
}

//
// collection
//

CompressedCollection* Repository::collection() {
  return _collection;
}

//
// deletedList
//

DeletedDocumentList& Repository::deletedList() {
  return _deletedList;
}

//
// _writeParameters
//

void Repository::_writeParameters( const std::string& path ) {
  // have to make a list of all the indexes to load
  _parameters.set( "indexes", "" );
  ScopedLock lock( _stateLock );

  Parameters indexes = _parameters["indexes"];
  indexes.clear();

  for( int i=0; i<_active->size(); i++ ) {
    indri::index::DiskIndex* index = dynamic_cast<indri::index::DiskIndex*>((*_active)[i]);

    if( index ) {
      indexes.append( "index" ).set( index->path() );
    }
  }

  _parameters.set( "indexCount", _indexCount );
  _parameters.writeFile( path );
}

//
// close
//

void Repository::close() {
  if( _collection ) {
    // TODO: make sure all the indexes get deleted
    std::string manifest = "manifest";
    std::string paramPath = Path::combine( _path, manifest );
    std::string deletedPath = Path::combine( _path, "deleted" );

    if( !_readOnly ) {
      write();
      merge();
    }

    // have to stop threads after the write request,
    // so the indexes actually get written
    _stopThreads();

    if( !_readOnly ) {
      if( Path::exists( deletedPath ) )
        lemur_compat::remove( deletedPath.c_str() );
      _deletedList.write( deletedPath );
      _writeParameters( paramPath );
    }

    _closeIndexes();
    _collection->close();

    delete _collection;
    _collection = 0;

    _fields.clear();
    delete_vector_contents( _transformations );
  }
}

//
// indexes
//

Repository::index_state Repository::indexes() {
  // calling this method implies that some query-related operation
  // is about to happen
  return _active;
}

//
// write
//
// Send a write request to the maintenance thread.
//

void Repository::write() {
  if( _maintenanceThread )
    _maintenanceThread->write();
}

//
// merge
//
// Send a merge request to the maintenance thread.
//

void Repository::merge() {
  if( _maintenanceThread )
    _maintenanceThread->merge();
}

//
// _startThreads
//

void Repository::_startThreads() {
  if( !_readOnly ) {
    _maintenanceThread = new RepositoryMaintenanceThread( *this, _memory );
    _maintenanceThread->start();
  } else {
    _maintenanceThread = 0;
  }

  _loadThread = new RepositoryLoadThread( *this, _memory );
  _loadThread->start();
}

//
// _stopThreads
//

void Repository::_stopThreads() {
  if( !_loadThread && !_maintenanceThread )
    return;

  if( _maintenanceThread )
    _maintenanceThread->signal();
  if( _loadThread )
    _loadThread->signal();

  if( _loadThread ) {
    _loadThread->join();
    delete _loadThread;
    _loadThread = 0;
  }

  if( _maintenanceThread ) {
    _maintenanceThread->join();
    delete _maintenanceThread;
    _maintenanceThread = 0;
  }
}

//
// _setThrashing
//

void Repository::_setThrashing( bool flag ) {
  _thrashing = flag;
  
  if( _thrashing ) {
    _lastThrashTime = IndriTimer::currentTime();
  }
}

//
// _timeSinceThrashing
//

UINT64 Repository::_timeSinceThrashing() {
  return IndriTimer::currentTime() - _lastThrashTime;
}



