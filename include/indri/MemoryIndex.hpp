
//
// MemoryIndex
//
// 15 November 2004 -- tds
//

#ifndef INDRI_MEMORYINDEX_HPP
#define INDRI_MEMORYINDEX_HPP

#include "indri/Index.hpp"
#include "indri/Mutex.hpp"
#include "HashTable.hpp"
#include "indri/DocumentData.hpp"
#include "Buffer.hpp"
#include <list>
#include <vector>

#include "indri/DocListIterator.hpp"
#include "indri/DocListFileIterator.hpp"
#include "indri/TermList.hpp"
#include "indri/TermListFileIterator.hpp"
#include "indri/VocabularyIterator.hpp"
#include "indri/ParsedDocument.hpp"
#include "indri/DocListMemoryBuilder.hpp"
#include "indri/FieldStatistics.hpp"
#include "indri/CorpusStatistics.hpp"
#include "indri/DocExtentListMemoryBuilder.hpp"
#include "indri/ReadersWritersLock.hpp"
#include "indri/ReaderLockable.hpp"
#include "indri/WriterLockable.hpp"
#include "indri/RegionAllocator.hpp"

namespace indri {
  namespace index {
    class MemoryIndex : public Index {
    public:
      // vocabulary structure
      struct term_entry {
        struct term_less {
          bool operator() ( const term_entry* one, const term_entry* two ) const {
            return strcmp( one->term, two->term ) < 0;
          }
        };

        term_entry( class RegionAllocator* allocator ) :
          list(allocator),
          next(0)
        {
        }

        void clearMark() {
          next = 0;
        }

        bool hasNext() {
          return next != 0 && next != (term_entry*) 1;
        }
          
        void mark() {
          next = (term_entry*) 1;
        }

        bool marked() {
          return next != 0;
        }

        char* term;
        int termID;
        TermData* termData;
        term_entry* next;
        indri::index::DocListMemoryBuilder list;
      };
      
    private:
      RegionAllocator _allocator;

      ReadersWritersLock _lock;
      ReaderLockable _readLock;
      WriterLockable _writeLock;

      CorpusStatistics _corpusStatistics;
      int _baseDocumentID;
      
      // document buffers
      indri::index::TermList _termList;
      greedy_vector<term_entry*> _seenTerms;

      // term lookups
      HashTable<const char*, term_entry*> _stringToTerm;
      std::vector<term_entry*> _idToTerm;

      // field statistics
      HashTable<const char*, int> _fieldLookup;
      std::vector<FieldStatistics> _fieldData;
      std::vector<indri::index::DocExtentListMemoryBuilder*> _fieldLists;
      
      // document statistics
      std::vector<indri::index::DocumentData> _documentData;
      
      // document vector buffers
      std::list<Buffer*> _termLists;
      UINT64 _termListsBaseOffset;
      
      void _addOpenTags( greedy_vector<indri::index::FieldExtent>& indexedTags,
                         greedy_vector<indri::index::FieldExtent>& openTags,
                         const greedy_vector<TagExtent>& extents,
                         unsigned int& extentIndex, 
                         unsigned int position );
      void _removeClosedTags( greedy_vector<indri::index::FieldExtent>& tags, unsigned int position );
      void _writeFieldExtents( int documentID, greedy_vector<indri::index::FieldExtent>& indexedTags );
      void _writeDocumentTermList( UINT64& offset, int& byteLength, int documentID, int documentLength, indri::index::TermList& locatedTerms );
      void _writeDocumentStatistics( UINT64 offset, int byteLength, int indexedLength, int totalLength, int uniqueTerms );
      term_entry* _lookupTerm( const char* term );
      void _destroyTerms();

      int _fieldID( const std::string& fieldName );
      int _fieldID( const char* fieldName );

    public:
      MemoryIndex();
      MemoryIndex( int docBase );
      MemoryIndex( int docBase, const std::vector<Index::FieldDescription>& fields );
      ~MemoryIndex();

      void close();

      int documentBase();
      
      int term( const std::string& t );
      int term( const char* t );
      std::string term( int termID );

      int field( const char* fieldName );
      int field( const std::string& fieldName );
      std::string field( int fieldID );

      int documentLength( int documentID );
      UINT64 documentCount();
      UINT64 documentCount( const std::string& term );
      UINT64 uniqueTermCount();
      
      UINT64 termCount( const std::string& term );
      UINT64 termCount();
      
      UINT64 fieldTermCount( const std::string& field );
      UINT64 fieldTermCount( const std::string& field, const std::string& term );
      
      UINT64 fieldDocumentCount( const std::string& field );
      UINT64 fieldDocumentCount( const std::string& field, const std::string& term );
      
      DocListIterator* docListIterator( int termID );
      DocListIterator* docListIterator( const std::string& term );
      DocListFileIterator* docListFileIterator();
      DocExtentListIterator* fieldListIterator( int fieldID );
      DocExtentListIterator* fieldListIterator( const std::string& field );
      const TermList* termList( int documentID );
      TermListFileIterator* termListFileIterator();

      VocabularyIterator* vocabularyIterator();
      VocabularyIterator* frequentVocabularyIterator();
      VocabularyIterator* infrequentVocabularyIterator();

      DocumentDataIterator* documentDataIterator();
      
      Lockable* iteratorLock();
      Lockable* statisticsLock();

      int addDocument( ParsedDocument& document );
      size_t memorySize();
    };
  }
}

#endif // INDRI_MEMORYINDEX_HPP
