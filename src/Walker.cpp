// 
// Warning: This file is automatically generated
// 

#include "indri/Walker.hpp"
#include "indri/QuerySpec.hpp"

namespace indri { 
  namespace lang { 
    Walker::~Walker() {}

    void Walker::defaultBefore( class Node* n ) {}
    void Walker::defaultAfter( class Node* n ) {}

   void Walker::before( class IndexTerm* n ) { defaultBefore( n ); }
   void Walker::after( class IndexTerm* n ) { defaultAfter( n ); }
   void Walker::before( class Field* n ) { defaultBefore( n ); }
   void Walker::after( class Field* n ) { defaultAfter( n ); }
   void Walker::before( class ExtentInside* n ) { defaultBefore( n ); }
   void Walker::after( class ExtentInside* n ) { defaultAfter( n ); }
   void Walker::before( class ExtentOr* n ) { defaultBefore( n ); }
   void Walker::after( class ExtentOr* n ) { defaultAfter( n ); }
   void Walker::before( class ExtentAnd* n ) { defaultBefore( n ); }
   void Walker::after( class ExtentAnd* n ) { defaultAfter( n ); }
   void Walker::before( class BAndNode* n ) { defaultBefore( n ); }
   void Walker::after( class BAndNode* n ) { defaultAfter( n ); }
   void Walker::before( class UWNode* n ) { defaultBefore( n ); }
   void Walker::after( class UWNode* n ) { defaultAfter( n ); }
   void Walker::before( class ODNode* n ) { defaultBefore( n ); }
   void Walker::after( class ODNode* n ) { defaultAfter( n ); }
   void Walker::before( class FilReqNode* n ) { defaultBefore( n ); }
   void Walker::after( class FilReqNode* n ) { defaultAfter( n ); }
   void Walker::before( class FilRejNode* n ) { defaultBefore( n ); }
   void Walker::after( class FilRejNode* n ) { defaultAfter( n ); }
   void Walker::before( class FieldLessNode* n ) { defaultBefore( n ); }
   void Walker::after( class FieldLessNode* n ) { defaultAfter( n ); }
   void Walker::before( class FieldGreaterNode* n ) { defaultBefore( n ); }
   void Walker::after( class FieldGreaterNode* n ) { defaultAfter( n ); }
   void Walker::before( class FieldBetweenNode* n ) { defaultBefore( n ); }
   void Walker::after( class FieldBetweenNode* n ) { defaultAfter( n ); }
   void Walker::before( class FieldEqualsNode* n ) { defaultBefore( n ); }
   void Walker::after( class FieldEqualsNode* n ) { defaultAfter( n ); }
   void Walker::before( class RawScorerNode* n ) { defaultBefore( n ); }
   void Walker::after( class RawScorerNode* n ) { defaultAfter( n ); }
   void Walker::before( class TermFrequencyScorerNode* n ) { defaultBefore( n ); }
   void Walker::after( class TermFrequencyScorerNode* n ) { defaultAfter( n ); }
   void Walker::before( class CachedFrequencyScorerNode* n ) { defaultBefore( n ); }
   void Walker::after( class CachedFrequencyScorerNode* n ) { defaultAfter( n ); }
   void Walker::before( class PriorNode* n ) { defaultBefore( n ); }
   void Walker::after( class PriorNode* n ) { defaultAfter( n ); }
   void Walker::before( class OrNode* n ) { defaultBefore( n ); }
   void Walker::after( class OrNode* n ) { defaultAfter( n ); }
   void Walker::before( class NotNode* n ) { defaultBefore( n ); }
   void Walker::after( class NotNode* n ) { defaultAfter( n ); }
   void Walker::before( class MaxNode* n ) { defaultBefore( n ); }
   void Walker::after( class MaxNode* n ) { defaultAfter( n ); }
   void Walker::before( class CombineNode* n ) { defaultBefore( n ); }
   void Walker::after( class CombineNode* n ) { defaultAfter( n ); }
   void Walker::before( class WAndNode* n ) { defaultBefore( n ); }
   void Walker::after( class WAndNode* n ) { defaultAfter( n ); }
   void Walker::before( class WSumNode* n ) { defaultBefore( n ); }
   void Walker::after( class WSumNode* n ) { defaultAfter( n ); }
   void Walker::before( class WeightNode* n ) { defaultBefore( n ); }
   void Walker::after( class WeightNode* n ) { defaultAfter( n ); }
   void Walker::before( class ExtentRestriction* n ) { defaultBefore( n ); }
   void Walker::after( class ExtentRestriction* n ) { defaultAfter( n ); }
   void Walker::before( class FixedPassage* n ) { defaultBefore( n ); }
   void Walker::after( class FixedPassage* n ) { defaultAfter( n ); }
   void Walker::before( class FilterNode* n ) { defaultBefore( n ); }
   void Walker::after( class FilterNode* n ) { defaultAfter( n ); }
   void Walker::before( class ContextCounterNode* n ) { defaultBefore( n ); }
   void Walker::after( class ContextCounterNode* n ) { defaultAfter( n ); }
   void Walker::before( class ContextSimpleCounterNode* n ) { defaultBefore( n ); }
   void Walker::after( class ContextSimpleCounterNode* n ) { defaultAfter( n ); }
   void Walker::before( class ScoreAccumulatorNode* n ) { defaultBefore( n ); }
   void Walker::after( class ScoreAccumulatorNode* n ) { defaultAfter( n ); }
   void Walker::before( class AnnotatorNode* n ) { defaultBefore( n ); }
   void Walker::after( class AnnotatorNode* n ) { defaultAfter( n ); }
 };
};

