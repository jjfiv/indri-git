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
// FieldIteratorNode
//
// 28 July 2004 -- tds
//

#include "indri/FieldIteratorNode.hpp"
#include "indri/FieldListIterator.hpp"
#include "indri/InferenceNetwork.hpp"
#include "indri/Annotator.hpp"

FieldIteratorNode::FieldIteratorNode( const std::string& name, InferenceNetwork& network, int listID ) :
  _name(name),
  _network(network),
  _listID(listID)
{
}

void FieldIteratorNode::indexChanged( indri::index::Index& index ) {
  _list = _network.getFieldIterator( _listID );

  if( _list )
    _list->startIteration();
}

void FieldIteratorNode::prepare( int documentID ) {
  _extents.clear();
  _numbers.clear();

  if( !_list )
    return;

  const indri::index::DocExtentListIterator::DocumentExtentData* info = _list->currentEntry();

  if( info && info->document == documentID ) {
    _extents = info->extents;
    _numbers = info->numbers;
  }
}

/// returns a list of intervals describing positions of children
const greedy_vector<Extent>& FieldIteratorNode::extents() {
  return _extents;
}

const greedy_vector<INT64>& FieldIteratorNode::numbers() {
  return _numbers;
}

int FieldIteratorNode::nextCandidateDocument() {
  if( !_list )
    return MAX_INT32;

  const indri::index::DocExtentListIterator::DocumentExtentData* info = _list->currentEntry();

  if( !info ) {
    return MAX_INT32;
  } else {
    return info->document;
  }
}

const std::string& FieldIteratorNode::getName() const {
  return _name;
}

void FieldIteratorNode::annotate( Annotator& annotator, int documentID, int begin, int end ) {
  annotator.addMatches( _extents, this, documentID, begin, end );
}


