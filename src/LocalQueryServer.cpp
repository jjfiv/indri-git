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
// LocalQueryServer
//
// 15 March 2004 -- tds
//

#include "indri/LocalQueryServer.hpp"
#include "indri/QuerySpec.hpp"
#include "lemur/lemur-platform.h"
#include "lemur/lemur-compat.hpp"
#include <vector>

#include "indri/UnnecessaryNodeRemoverCopier.hpp"
#include "indri/ContextSimpleCountCollectorCopier.hpp"
#include "indri/FrequencyListCopier.hpp"
#include "indri/DagCopier.hpp"

#include "indri/InferenceNetworkBuilder.hpp"
#include "indri/InferenceNetwork.hpp"

#include "indri/CompressedCollection.hpp"
#include "indri/delete_range.hpp"
#include "indri/WeightFoldingCopier.hpp"

#include "indri/Appliers.hpp"
#include "indri/ScopedLock.hpp"

//
// Response objects
//

class LocalQueryServerResponse : public QueryServerResponse {
private:
  InferenceNetwork::MAllResults _results;

public:
  LocalQueryServerResponse( const InferenceNetwork::MAllResults& results ) :
    _results(results) {
  }
  
  InferenceNetwork::MAllResults& getResults() {
    return _results;
  }
};

class LocalQueryServerDocumentsResponse : public QueryServerDocumentsResponse {
private:
  std::vector<ParsedDocument*> _documents;

public:
  LocalQueryServerDocumentsResponse( const std::vector<ParsedDocument*>& results )
    :
    _documents(results)
  {
  }

  // caller's responsibility to delete these results
  std::vector<ParsedDocument*>& getResults() {
    return _documents;
  }
};

class LocalQueryServerMetadataResponse : public QueryServerMetadataResponse {
private:
  std::vector<std::string> _metadata;

public:
  LocalQueryServerMetadataResponse( const std::vector<std::string>& metadata ) :
    _metadata(metadata)
  {
  }

  std::vector<std::string>& getResults() {
    return _metadata;
  }
};

class LocalQueryServerVectorsResponse : public QueryServerVectorsResponse {
private:
  std::vector<DocumentVector*> _vectors;

public:
  LocalQueryServerVectorsResponse( int vectorCount ) {
    _vectors.reserve( vectorCount );
  }

  void addVector( DocumentVector* vec ) {
    _vectors.push_back( vec );
  }

  // caller deletes DocumentVector objects
  std::vector<DocumentVector*>& getResults() {
    return _vectors;
  }
};

class LocalQueryServerDocumentIDsResponse : public QueryServerDocumentIDsResponse {
private:
  std::vector<DOCID_T> _documentIDs;

public:
  LocalQueryServerDocumentIDsResponse( const std::vector<DOCID_T>& documents ) : 
    _documentIDs(documents)
  {
  }

  std::vector<DOCID_T>& getResults() {
    return _documentIDs;
  }
};

//
// Class code
//

LocalQueryServer::LocalQueryServer( Repository& repository ) :
  _repository(repository)
{
}

//
// _indexWithDocument
//

indri::index::Index* LocalQueryServer::_indexWithDocument( Repository::index_state& indexes, int documentID ) {
  for( int i=0; i<indexes->size(); i++ ) {
    ScopedLock lock( (*indexes)[i]->statisticsLock() );
    int lowerBound = (*indexes)[i]->documentBase();
    int upperBound = (*indexes)[i]->documentBase() + (*indexes)[i]->documentCount();
    
    if( lowerBound <= documentID && upperBound > documentID ) {
      return (*indexes)[i];
    }
  }
  
  return 0;
}

//
// document
//

ParsedDocument* LocalQueryServer::document( int documentID ) {
  CompressedCollection* collection = _repository.collection();
  ParsedDocument* document = collection->retrieve( documentID );
  return document;
}

std::string LocalQueryServer::documentMetadatum( int documentID, const std::string& attributeName ) {
  CompressedCollection* collection = _repository.collection();
  return collection->retrieveMetadatum( documentID, attributeName );
}

QueryServerMetadataResponse* LocalQueryServer::documentMetadata( const std::vector<int>& documentIDs, const std::string& attributeName ) {
  std::vector<std::string> result;

  std::vector<std::pair<int, int> > docSorted;
  for( unsigned int i=0; i<documentIDs.size(); i++ ) {
    docSorted.push_back( std::make_pair( documentIDs[i], i ) );
  }
  std::sort( docSorted.begin(), docSorted.end() );

  for( unsigned int i=0; i<docSorted.size(); i++ ) {
    result.push_back( documentMetadatum(docSorted[i].first, attributeName) );
  }

  std::vector<std::string> actual;
  actual.resize( documentIDs.size() );
  for( unsigned int i=0; i<docSorted.size(); i++ ) {
    actual[docSorted[i].second] = result[i];
  }

  return new LocalQueryServerMetadataResponse( actual );
}

QueryServerDocumentsResponse* LocalQueryServer::documents( const std::vector<int>& documentIDs ) {
  std::vector<ParsedDocument*> result;
  for( unsigned int i=0; i<documentIDs.size(); i++ ) {
    result.push_back( document(documentIDs[i]) );
  }
  return new LocalQueryServerDocumentsResponse( result );
}

QueryServerDocumentsResponse* LocalQueryServer::documentsFromMetadata( const std::string& attributeName, const std::vector<std::string>& attributeValues ) {
  CompressedCollection* collection = _repository.collection();
  std::vector<ParsedDocument*> result;
  
  for( unsigned int i=0; i<attributeValues.size(); i++ ) {
    std::vector<ParsedDocument*> documents = collection->retrieveByMetadatum( attributeName, attributeValues[i] );
    std::copy( documents.begin(), documents.end(), std::back_inserter( documents ) );
  }

  return new LocalQueryServerDocumentsResponse( result );
}

QueryServerDocumentIDsResponse* LocalQueryServer::documentIDsFromMetadata( const std::string& attributeName, const std::vector<std::string>& attributeValues ) {
  CompressedCollection* collection = _repository.collection();
  std::vector<DOCID_T> result;
  
  for( unsigned int i=0; i<attributeValues.size(); i++ ) {
    std::vector<DOCID_T> documents = collection->retrieveIDByMetadatum( attributeName, attributeValues[i] );
    std::copy( documents.begin(), documents.end(), std::back_inserter( documents ) );
  }

  return new LocalQueryServerDocumentIDsResponse( result );
}

INT64 LocalQueryServer::termCount() {
  Repository::index_state indexes = _repository.indexes();
  INT64 total = 0;

  for( int i=0; i<indexes->size(); i++ ) {
    ScopedLock lock( (*indexes)[i]->statisticsLock() );
    total += (*indexes)[i]->termCount();
  }

  return total;
}

INT64 LocalQueryServer::termCount( const std::string& term ) {
  std::string stem = _repository.processTerm( term );
  // stopwords return a string of length 0, causing Keyfile to throw.
  if( stem.length() != 0 ) {
    return stemCount( stem );
  } else {
    return 0;
  }
}

INT64 LocalQueryServer::stemCount( const std::string& stem ) {
  Repository::index_state indexes = _repository.indexes();
  INT64 total = 0;

  for( int i=0; i<indexes->size(); i++ ) {
    ScopedLock lock( (*indexes)[i]->statisticsLock() );
    total += (*indexes)[i]->termCount( stem );
  }

  return total;
}

INT64 LocalQueryServer::termFieldCount( const std::string& term, const std::string& field ) {
  std::string stem = _repository.processTerm( term );

  if( stem.length() != 0 ) {
    return stemFieldCount( stem, field );
  } else {
    return 0;
  }
}

INT64 LocalQueryServer::stemFieldCount( const std::string& stem, const std::string& field ) {
  Repository::index_state indexes = _repository.indexes();
  INT64 total = 0;

  for( int i=0; i<indexes->size(); i++ ) {
    ScopedLock lock( (*indexes)[i]->statisticsLock() );
    total += (*indexes)[i]->fieldTermCount( field, stem );
  }

  return total;
}

std::string LocalQueryServer::termName( int term ) {
  Repository::index_state indexes = _repository.indexes();
  indri::index::Index* index = (*indexes)[0];
  ScopedLock lock( index->statisticsLock() );
  return index->term( term );
}

int LocalQueryServer::termID( const std::string& term ) {
  Repository::index_state indexes = _repository.indexes();
  indri::index::Index* index = (*indexes)[0];
  std::string processed = _repository.processTerm( term );
  ScopedLock lock( index->statisticsLock() );

  if( processed.length() != 0 ) {
    return index->term( processed.c_str() );
  } else {
    return 0;
  }
}

std::vector<std::string> LocalQueryServer::fieldList() {
  std::vector<std::string> result;
  const std::vector<Repository::Field>& fields = _repository.fields();

  for( int i=0; i<fields.size(); i++ ) {
    result.push_back( fields[i].name );
  }

  return result;
}

int LocalQueryServer::documentLength( int documentID ) {
  Repository::index_state indexes = _repository.indexes();
  indri::index::Index* index = _indexWithDocument( indexes, documentID );

  if( index ) {
    ScopedLock lock( index->statisticsLock() );
    return index->documentLength( documentID );
  }

  return 0;
}

INT64 LocalQueryServer::documentCount() {
  Repository::index_state indexes = _repository.indexes();
  INT64 total = 0;
  
  for( int i=0; i<indexes->size(); i++ ) {
    ScopedLock lock( (*indexes)[i]->statisticsLock() );
    total += (*indexes)[i]->documentCount();
  }
  
  return total;
}

INT64 LocalQueryServer::documentCount( const std::string& term ) {
  Repository::index_state indexes = _repository.indexes();
  INT64 total = 0;
  
  for( int i=0; i<indexes->size(); i++ ) {
    ScopedLock lock( (*indexes)[i]->statisticsLock() );
    total += (*indexes)[i]->documentCount( term );
  }
  
  return total;
}

QueryServerResponse* LocalQueryServer::runQuery( std::vector<indri::lang::Node*>& roots, int resultsRequested, bool optimize ) {
  // use UnnecessaryNodeRemover to get rid of window nodes, ExtentAnd nodes and ExtentOr nodes
  // that only have one child
  ApplyCopiers<UnnecessaryNodeRemoverCopier> unnecessary( roots );

  // run the contextsimplecountcollectorcopier to gather easy stats
  ApplyCopiers<ContextSimpleCountCollectorCopier> contexts( unnecessary.roots() );

  // use frequency-only nodes where appropriate
  ApplyCopiers<FrequencyListCopier> frequency( contexts.roots(), _cache );

  // fold together any nested weight nodes
  ApplyCopiers<WeightFoldingCopier> weight( frequency.roots() );

  // make all this into a dag
  ApplySingleCopier<DagCopier> dag( weight.roots(), _repository );

  std::vector<indri::lang::Node*>& networkRoots = dag.roots();

  if( !optimize ) {
    // we may be asked not to perform optimizations that might
    // drastically change the structure of the tree; for instance,
    // annotation queries may ask for this
    networkRoots = contexts.roots();
  }

  // build an inference network
  InferenceNetworkBuilder builder( _repository, _cache, resultsRequested );
  ApplyWalker<InferenceNetworkBuilder> buildWalker( networkRoots, &builder );

  InferenceNetwork* network = builder.getNetwork();
  InferenceNetwork::MAllResults result;
  result = network->evaluate();

  return new LocalQueryServerResponse( result );
}

QueryServerVectorsResponse* LocalQueryServer::documentVectors( const std::vector<int>& documentIDs ) {
  LocalQueryServerVectorsResponse* response = new LocalQueryServerVectorsResponse( documentIDs.size() );
  Repository::index_state indexes = _repository.indexes();
  std::map<int, std::string> termIDStringMap;

  for( size_t i=0; i<documentIDs.size(); i++ ) {
    indri::index::Index* index = _indexWithDocument( indexes, documentIDs[i] );

    {
      ScopedLock lock( index->statisticsLock() );
  
      const indri::index::TermList* termList = index->termList( documentIDs[i] );
      DocumentVector* result = new DocumentVector( index, termList, termIDStringMap );
      delete termList;
      response->addVector( result );
    }
  }

  return response;
}
