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
// ContextCountAccumulator
//
// 24 February 2004 -- tds
//

#include "indri/ContextCountAccumulator.hpp"
#include <assert.h>
#include <vector>
#include "indri/QuerySpec.hpp"
#include "indri/EvaluatorNode.hpp"
#include "indri/DocumentCount.hpp"

ContextCountAccumulator::ContextCountAccumulator( const std::string& name, ListIteratorNode* matches, ListIteratorNode* context ) :
  _name(name),
  _matches(matches),
  _context(context),
  _occurrences(0),
  _contextSize(0)
{
}

ContextCountAccumulator::~ContextCountAccumulator() {
}

const std::string& ContextCountAccumulator::getName() const {
  return _name;
}

UINT64 ContextCountAccumulator::getOccurrences() const {
  return _occurrences;
}

UINT64 ContextCountAccumulator::getContextSize() const {
  return _contextSize;
}

const EvaluatorNode::MResults& ContextCountAccumulator::getResults() {
  // we must be finished, so now is a good time to add our results to the ListCache
  _results.clear();

  _results[ "occurrences" ].push_back( ScoredExtentResult( _occurrences, 0 ) );
  _results[ "contextSize" ].push_back( ScoredExtentResult( _contextSize, 0 ) );

  return _results;
}

const ListIteratorNode* ContextCountAccumulator::getContextNode() const {
  return _context;
}

const ListIteratorNode* ContextCountAccumulator::getMatchesNode() const {
  return _matches;
}

void ContextCountAccumulator::evaluate( int documentID, int documentLength ) {
  UINT64 documentOccurrences; 
  UINT64 documentContextSize;

  if( !_context ) {
    documentOccurrences = _matches->extents().size();
    documentContextSize = documentLength;
  } else {
    documentOccurrences = 0;
    documentContextSize = 0;

    const greedy_vector<Extent>& matches = _matches->extents();
    const greedy_vector<Extent>& extents = _context->extents();
    unsigned int ex = 0;
    
    for( unsigned int i=0; i<matches.size() && ex < extents.size(); i++ ) {
      while( ex < extents.size() && matches[i].begin < extents[ex].begin )
        ex++;

      if( ex < extents.size() &&
        matches[i].begin >= extents[ex].begin &&
        matches[i].end <= extents[ex].end ) {
        documentOccurrences++;
      }
    }

    for( unsigned int i=0; i<extents.size(); i++ ) {
      documentContextSize += extents[i].end - extents[i].begin;
    }
  } 

  _occurrences += documentOccurrences;
  _contextSize += documentContextSize;
}

int ContextCountAccumulator::nextCandidateDocument() {
  int candidate = _matches->nextCandidateDocument();

  if( _context ) {
    candidate = lemur_compat::min( candidate, _context->nextCandidateDocument() );
  }

  return candidate;
}

//
// indexChanged
//

void ContextCountAccumulator::indexChanged( indri::index::Index& index ) {
  // do nothing
}




