
//
// ContextSimpleCountAccumulator
//
// 14 December 2004 -- tds
//
// Unlike the ContextCountAccumulator, which counts occurrences of
// terms in a very general way, this node uses knowledge about the 
// query tree to extract counts directly from the index.
//
// This node is placed into the query tree by the
// ContextSimpleCountCollectorCopier.
//

#include "indri/ContextSimpleCountAccumulator.hpp"
#include "indri/ScoredExtentResult.hpp"

void ContextSimpleCountAccumulator::_computeCounts( indri::index::Index& index ) {
  assert( _terms.size() );
  assert( _context.size() == 0 || _field.size() == 0 );

  if( _context.size() ) {
    _size += index.fieldTermCount( _context );
  } else {
    _size += index.termCount();
  }

  for( unsigned int i=0; i<_terms.size(); i++ ) {
    if( _terms[i].length() != 0 ) {
      if( _field.size() ) {
        _occurrences += index.fieldTermCount( _field, _terms[i] );
      } else if( _context.size() ) {
        _occurrences += index.fieldTermCount( _context, _terms[i] );
      } else {
        _occurrences += index.termCount( _terms[i] );
      }
    }
  }
}

ContextSimpleCountAccumulator::ContextSimpleCountAccumulator( const std::string& nodeName,
                                                              const std::vector<std::string>& terms,
                                                              const std::string& field,
                                                              const std::string& context ) :
  _name(nodeName),
  _terms(terms),
  _field(field),
  _context(context),
  _occurrences(0),
  _size(0)
{
}

const std::string& ContextSimpleCountAccumulator::getName() const {
  return _name;
}

const EvaluatorNode::MResults& ContextSimpleCountAccumulator::getResults() {
  _results.clear();

  _results[ "occurrences" ].push_back( ScoredExtentResult( _occurrences, 0 ) );
  _results[ "contextSize" ].push_back( ScoredExtentResult( _size, 0 ) );

  return _results;
}

void ContextSimpleCountAccumulator::indexChanged( indri::index::Index& index ) {
  _computeCounts( index );
  _maximumDocument = index.documentCount() + index.documentBase();
}

void ContextSimpleCountAccumulator::evaluate( int documentID, int documentLength ) {
  // do nothing
}

int ContextSimpleCountAccumulator::nextCandidateDocument() {
  return MAX_INT32;
}
