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

#include "indri/TermFrequencyBeliefNode.hpp"
#include "indri/InferenceNetwork.hpp"
#include <math.h>

TermFrequencyBeliefNode::TermFrequencyBeliefNode( const std::string& name,
                                                  class InferenceNetwork& network,
                                                  int listID,
                                                  TermScoreFunction& scoreFunction )
  :
  _name(name),
  _network(network),
  _listID(listID),
  _function(scoreFunction)
{
}

TermFrequencyBeliefNode::~TermFrequencyBeliefNode() {
}

const greedy_vector<indri::index::DocListIterator::TopDocument>& TermFrequencyBeliefNode::topdocs() const {
  if( _list )
    return _list->topDocuments();

  return _emptyTopdocs;
}

int TermFrequencyBeliefNode::nextCandidateDocument() {
  if( _list ) {
    const indri::index::DocListIterator::DocumentData* entry = _list->currentEntry();
    
    if( entry ) {
      return entry->document;
    }
  }

  return MAX_INT32;
}

double TermFrequencyBeliefNode::maximumBackgroundScore() {
  return _maximumBackgroundScore;
}

double TermFrequencyBeliefNode::maximumScore() {
  return _maximumScore;
}

const greedy_vector<ScoredExtentResult>& TermFrequencyBeliefNode::score( int documentID, int begin, int end, int documentLength ) {
  assert( begin == 0 && end == documentLength ); // FrequencyListCopier ensures this condition
  _extents.clear();

  if( _list ) {
    const indri::index::DocListIterator::DocumentData* entry = _list->currentEntry();
    int count = ( entry && entry->document == documentID ) ? entry->positions.size() : 0;
    double score = _function.scoreOccurrence( count, documentLength );

    _extents.push_back( ScoredExtentResult( score, documentID, begin, end ) );
  }

  return _extents;
}

bool TermFrequencyBeliefNode::hasMatch( int documentID ) {
  if( _list ) {
    const indri::index::DocListIterator::DocumentData* entry = _list->currentEntry();
    return ( entry && entry->document == documentID );
  }

  return false;
}

const std::string& TermFrequencyBeliefNode::getName() const {
  return _name;
}

void TermFrequencyBeliefNode::indexChanged( indri::index::Index& index ) {
  // fetch the next inverted list
  _list = _network.getDocIterator( _listID );

  if( !_list ) {
    _maximumBackgroundScore = INDRI_HUGE_SCORE;
    _maximumScore = INDRI_HUGE_SCORE;
  } else {
    // maximum fraction
    double maximumFraction = 1;
    
    if( _list->topDocuments().size() ) {
      const indri::index::DocListIterator::TopDocument& document = _list->topDocuments().back();
      maximumFraction = double(document.count) / double(document.length);
    }

    indri::index::TermData* termData = _list->termData();

    UINT64 maxOccurrences = UINT64( ceil( double(termData->maxDocumentLength) * maximumFraction ) );

    _maximumScore = _function.scoreOccurrence( maxOccurrences, termData->maxDocumentLength );
    _maximumBackgroundScore = _function.scoreOccurrence( 0, termData->minDocumentLength );
  }
}

void TermFrequencyBeliefNode::annotate( Annotator& annotator, int documentID, int begin, int end ) {
  // can't annotate -- don't have position info
}
