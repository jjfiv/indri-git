/*==========================================================================
  Copyright (c) 2004 University of Massachusetts.  All Rights Reserved.

  Use of the Lemur Toolkit for Language Modeling and Information Retrieval
  is subject to the terms of the software license set forth in the LICENSE
  file included with this software, and also available at
  http://www.cs.cmu.edu/~lemur/license.html 
  as well as the conditions below.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

  3. The names "Indri", "Center for Intelligent Information Retrieval", 
  "CIIR", and "University of Massachusetts" must not be used to
  endorse or promote products derived from this software without
  prior written permission. To obtain permission, contact
  indri-info@ciir.cs.umass.edu.

  4. Products derived from this software may not be called "Indri" nor 
  may "Indri" appear in their names without prior written permission of 
  the University of Massachusetts. To obtain permission, contact 
  indri-info@ciir.cs.umass.edu.

  THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY OF MASSACHUSETTS AND OTHER
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
  THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.
  ==========================================================================
*/


//
// FilterRejectNode
//
// 6 July 2004 -- tds
//

#include "lemur/lemur-compat.hpp"
#include "indri/FilterRejectNode.hpp"
#include "indri/ListIteratorNode.hpp"
#include "indri/greedy_vector"
#include "indri/Extent.hpp"
#include "indri/Annotator.hpp"

FilterRejectNode::FilterRejectNode( const std::string& name, 
				    ListIteratorNode* filter, 
				    BeliefNode* disallowed ) {
  _name = name;
  _filter = filter;
  _disallowed = disallowed;
}


int FilterRejectNode::nextCandidateDocument() {
  // it'd be nice to use the information from _filter to 
  // skip documents in the case when _filter->nextCandidate..() == _disallowed->nextCandidate...()
  // but we don't know for sure that _filter will match: we only know that it might match.
  // therefore we have to just go with the next _disallowed document.
  return _disallowed->nextCandidateDocument();
}

double FilterRejectNode::maximumBackgroundScore() {
  // delegate to the query as if the filter were true
  return _disallowed->maximumBackgroundScore();
}

double FilterRejectNode::maximumScore() {
  return _disallowed->maximumScore();
}

bool FilterRejectNode::hasMatch( int documentID ) {
  // delegate to the children.
  return (_filter->extents().size() == 0 &&
	  _disallowed->hasMatch( documentID ));
}

const std::string& FilterRejectNode::getName() const {
  return _name;
}

const greedy_vector<ScoredExtentResult>& FilterRejectNode::score( int documentID, int begin, int end, int documentLength ) {
  _extents.clear();
  // if the filter doesn't apply, return the child score.
  if (_filter->extents().size() == 0 )
    return _disallowed->score( documentID, begin, end, documentLength );
  else
    return _extents;
}

void FilterRejectNode::annotate( Annotator& annotator, int documentID, int begin, int end ) {
  _filter->annotate( annotator, documentID, begin, end );
  if( _filter->extents().size() == 0 ) {
    _disallowed->annotate( annotator, documentID, begin, end );
  }
}


