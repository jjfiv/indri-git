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
// FixedPassageNode
//
// 23 February 2005 -- tds
//

#include "indri/FixedPassageNode.hpp"
#include "indri/Annotator.hpp"
#include "lemur/lemur-compat.hpp"

FixedPassageNode::FixedPassageNode( const std::string& name, BeliefNode* child, int windowSize, int increment ) :
  _name(name),
  _child(child),
  _windowSize(windowSize),
  _increment(increment)
{
}

int FixedPassageNode::nextCandidateDocument() {
  return _child->nextCandidateDocument();
}

double FixedPassageNode::maximumBackgroundScore() {
  return INDRI_TINY_SCORE;
}

double FixedPassageNode::maximumScore() {
  return INDRI_HUGE_SCORE;
}

const greedy_vector<ScoredExtentResult>& FixedPassageNode::score( int documentID, int begin, int end, int documentLength ) {
  // we're going to run through the field list, etc.
  _scores.clear();

  // round down to find where the passage starts
  int beginPassage = (begin / _increment) * _increment;

  for( ; beginPassage < end; beginPassage += _increment ) {
    int endPassage = beginPassage + _windowSize;

    int scoreBegin = lemur_compat::max( beginPassage, begin );
    int scoreEnd = lemur_compat::min( endPassage, end );

    const greedy_vector<ScoredExtentResult>& childResults = _child->score( documentID, scoreBegin, scoreEnd, documentLength );

    for( int i=0; i<childResults.size(); i++ ) {
      ScoredExtentResult result( childResults[i].score, documentID, scoreBegin, scoreEnd );
      _scores.push_back( result );
    }
  }

  return _scores;
}

void FixedPassageNode::annotate( Annotator& annotator, int documentID, int begin, int end ) {
  annotator.add(this, documentID, begin, end);

  // round down to find where the passage starts
  int beginPassage = (begin / _increment) * _increment;

  for( ; beginPassage < end; beginPassage += _increment ) {
    int endPassage = beginPassage + _windowSize;

    int scoreBegin = lemur_compat::max( beginPassage, begin );
    int scoreEnd = lemur_compat::min( endPassage, end );

    _child->annotate( annotator, documentID, scoreBegin, scoreEnd );
  }
}

bool FixedPassageNode::hasMatch( int documentID ) {
  return _child->hasMatch( documentID );
}


const std::string& FixedPassageNode::getName() const {
  return _name;
}

void FixedPassageNode::indexChanged( indri::index::Index& index ) {
  // do nothing
}

