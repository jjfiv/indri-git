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
// dumpindex
//
// 13 September 2004 -- tds
//

#include "indri/Repository.hpp"
#include "indri/CompressedCollection.hpp"
#include "indri/LocalQueryServer.hpp"
#include "indri/ScopedLock.hpp"
#include <iostream>

int count_term_in_documents( indri::collection::Repository& r, int termID, std::vector<int>& documents );

void print_invfile( indri::collection::Repository& r ) {
  indri::collection::Repository::index_state state = r.indexes();

  indri::index::Index* index = (*state)[0];
  indri::index::DocListFileIterator* iter = index->docListFileIterator();
  if (iter == NULL) return;
  
  iter->startIteration();

  while( !iter->finished() ) {
    indri::index::DocListFileIterator::DocListData* entry = iter->currentEntry();
    indri::index::TermData* termData = entry->termData;
 
    entry->iterator->startIteration();
    while( !entry->iterator->finished() )
      entry->iterator->nextEntry();

    std::cout << termData->term << std::endl;
    iter->nextEntry();
  }

  delete iter;
}

void print_field_positions( indri::collection::Repository& r, const std::string& fieldString ) {
  indri::server::LocalQueryServer local(r);

  UINT64 totalCount = local.termCount();

  std::cout << fieldString << std::endl;

  indri::collection::Repository::index_state state = r.indexes();

  for( size_t i=0; i<state->size(); i++ ) {
    indri::index::Index* index = (*state)[i];
    indri::thread::ScopedLock( index->iteratorLock() );

    indri::index::DocExtentListIterator* iter = index->fieldListIterator( fieldString );
    if (iter == NULL) continue;
    
    iter->startIteration();

    int doc = 0;
    indri::index::DocExtentListIterator::DocumentExtentData* entry;

    for( iter->startIteration(); iter->finished() == false; iter->nextEntry() ) {
      entry = iter->currentEntry();

      std::cout << entry->document << " "
                << entry->extents.size() << " "
                << index->documentLength( entry->document ) << " ";

      int count = entry->extents.size();

      for( int i=0; i<count; i++ ) {
        std::cout << " ( " << entry->extents[i].begin << ", " << entry->extents[i].end;
        if( entry->numbers.size() ) {
          std::cout << ", " << entry->numbers[i];
        }
        std::cout << " ) ";
      }

      std::cout << std::endl;
    }

    delete iter;
  }
}

void print_term_positions( indri::collection::Repository& r, const std::string& termString ) {
  std::string stem = r.processTerm( termString );
  indri::server::LocalQueryServer local(r);

  UINT64 totalCount = local.termCount();
  UINT64 termCount = local.termCount( termString );

  std::cout << termString << " "
            << stem << " "
            << termCount << " " 
            << totalCount << " " << std::endl;

  indri::collection::Repository::index_state state = r.indexes();

  for( size_t i=0; i<state->size(); i++ ) {
    indri::index::Index* index = (*state)[i];
    indri::thread::ScopedLock( index->iteratorLock() );

    indri::index::DocListIterator* iter = index->docListIterator( stem );
    if (iter == NULL) continue;
    
    iter->startIteration();

    int doc = 0;
    indri::index::DocListIterator::DocumentData* entry;

    for( iter->startIteration(); iter->finished() == false; iter->nextEntry() ) {
      entry = (indri::index::DocListIterator::DocumentData*) iter->currentEntry();

      std::cout << entry->document << " "
                << entry->positions.size() << " "
                << index->documentLength( entry->document ) << " ";

      int count = entry->positions.size();

      for( int i=0; i<count; i++ ) {
        std::cout << entry->positions[i] << " ";
      }

      std::cout << std::endl;
    }

    delete iter;
  }
}

void print_term_counts( indri::collection::Repository& r, const std::string& termString ) {
  std::string stem = r.processTerm( termString );
  indri::server::LocalQueryServer local(r);

  UINT64 totalCount = local.termCount();
  UINT64 termCount = local.termCount( termString );

  std::cout << termString << " "
            << stem << " "
            << termCount << " " 
            << totalCount << " " << std::endl;

  indri::collection::Repository::index_state state = r.indexes();

  for( size_t i=0; i<state->size(); i++ ) {
    indri::index::Index* index = (*state)[i];
    indri::thread::ScopedLock( index->iteratorLock() );

    indri::index::DocListIterator* iter = index->docListIterator( stem );
    if (iter == NULL) continue;

    iter->startIteration();

    int doc = 0;
    indri::index::DocListIterator::DocumentData* entry;

    for( iter->startIteration(); iter->finished() == false; iter->nextEntry() ) {
      entry = iter->currentEntry();

      std::cout << entry->document << " "
                << entry->positions.size() << " "
                << index->documentLength( entry->document ) << std::endl;
    }

    delete iter;
  }
}

void print_document_name( indri::collection::Repository& r, const char* number ) {
  indri::collection::CompressedCollection* collection = r.collection();
  //  std::string documentName = collection->retrieveMetadatum( atoi( number ), "docid" );
  std::string documentName = collection->retrieveMetadatum( atoi( number ), "docno" );
  std::cout << documentName << std::endl;
}

void print_document_text( indri::collection::Repository& r, const char* number ) {
  int documentID = atoi( number );
  indri::collection::CompressedCollection* collection = r.collection();
  indri::api::ParsedDocument* document = collection->retrieve( documentID );

  std::cout << document->text << std::endl;
  delete document;
}

void print_document_data( indri::collection::Repository& r, const char* number ) {
  int documentID = atoi( number );
  indri::collection::CompressedCollection* collection = r.collection();
  indri::api::ParsedDocument* document = collection->retrieve( documentID );

  std::cout << std::endl << "--- Metadata ---" << std::endl << std::endl;

  for( int i=0; i<document->metadata.size(); i++ ) {
    if( document->metadata[i].key[0] == '#' )
      continue;

    std::cout << document->metadata[i].key << ": "
              << (const char*) document->metadata[i].value
              << std::endl;
  }

  std::cout << std::endl << "--- Positions ---" << std::endl << std::endl;

  for( int i=0; i<document->positions.size(); i++ ) {
    std::cout << i << " "
              << document->positions[i].begin << " "
              << document->positions[i].end << std::endl;

  }

  std::cout << std::endl << "--- Tags ---" << std::endl << std::endl;

  for( int i=0; i<document->tags.size(); i++ ) {
    std::cout << i << " "
              << document->tags[i].name << " " 
              << document->tags[i].begin << " "
              << document->tags[i].end << " " 
              << document->tags[i].number << std::endl;
  }

  std::cout << std::endl << "--- Text ---" << std::endl << std::endl;
  std::cout << document->text << std::endl;
  delete document;
}

void print_document_vector( indri::collection::Repository& r, const char* number ) {
  indri::server::LocalQueryServer local(r);
  lemur::api::DOCID_T documentID = atoi( number );

  std::vector<lemur::api::DOCID_T> documentIDs;
  documentIDs.push_back(documentID);

  indri::server::QueryServerVectorsResponse* response = local.documentVectors( documentIDs );
  
  if( response->getResults().size() ) {
    indri::api::DocumentVector* docVector = response->getResults()[0];
  
    std::cout << "--- Fields ---" << std::endl;

    for( int i=0; i<docVector->fields().size(); i++ ) {
      const indri::api::DocumentVector::Field& field = docVector->fields()[i];
      std::cout << field.name << " " << field.begin << " " << field.end << " " << field.number << std::endl;
    }

    std::cout << "--- Terms ---" << std::endl;

    for( int i=0; i<docVector->positions().size(); i++ ) {
      int position = docVector->positions()[i];
      const std::string& stem = docVector->stems()[position];

      std::cout << i << " " << position << " " << stem << std::endl;
    }

    delete docVector;
  }

  delete response;
}

void print_document_id( indri::collection::Repository& r, const char* an, const char* av ) {
  indri::collection::CompressedCollection* collection = r.collection();
  std::string attributeName = an;
  std::string attributeValue = av;
  std::vector<lemur::api::DOCID_T> documentIDs;

  documentIDs = collection->retrieveIDByMetadatum( attributeName, attributeValue );

  for( size_t i=0; i<documentIDs.size(); i++ ) {
    std::cout << documentIDs[i] << std::endl;
  }
}

void usage() {
  std::cout << "dumpindex <repository> <command> [ <argument> ]*" << std::endl;
  std::cout << "Valid commands are: " << std::endl;
  std::cout << "    Command              Argument       Description" << std::endl;
  std::cout << "    term (t)             Term text      Print inverted list for a term" << std::endl;
  std::cout << "    termpositions (tp)   Term text      Print inverted list for a term, with positions" << std::endl;
  std::cout << "    fieldpositions (fp)  Field name     Print inverted list for a field, with positions" << std::endl;
  std::cout << "    documentid (di)      Field, Value   Print the document IDs of documents having a metadata field"
            << "                                              matching this value" << std::endl;
  std::cout << "    documentname (dn)    Document ID    Print the text representation of a document ID" << std::endl;
  std::cout << "    documenttext (dt)    Document ID    Print the text of a document" << std::endl;
  std::cout << "    documenttext (dd)    Document ID    Print the full representation of a document" << std::endl;
  std::cout << "    documentvector (dv)  Document ID    Print the document vector of a document" << std::endl;
}

#define REQUIRE_ARGS(n) { if( argc < n ) { usage(); return -1; } }

int main( int argc, char** argv ) {
  try {
    REQUIRE_ARGS(3);

    indri::collection::Repository r;
    char* repName = argv[1];
    r.openRead( repName );

    std::string command = argv[2];

    if( command == "t" || command == "term" ) {
      REQUIRE_ARGS(4);
      std::string term = argv[3];
      print_term_counts( r, term );
    } else if( command == "tp" || command == "termpositions" ) { 
      REQUIRE_ARGS(4);
      std::string term = argv[3];
      print_term_positions( r, term );
    } else if( command == "fp" || command == "fieldpositions" ) { 
      REQUIRE_ARGS(4);
      std::string field = argv[3];
      print_field_positions( r, field );
    } else if( command == "dn" || command == "documentname" ) {
      REQUIRE_ARGS(4);
      print_document_name( r, argv[3] );
    } else if( command == "dt" || command == "documenttext" ) {
      REQUIRE_ARGS(4);
      print_document_text( r, argv[3] );
    } else if( command == "dd" || command == "documentdata" ) {
      REQUIRE_ARGS(4);
      print_document_data( r, argv[3] );
    } else if( command == "dv" || command == "documentvector" ) {
      REQUIRE_ARGS(4);
      print_document_vector( r, argv[3] );
    } else if( command == "di" || command == "documentid" ) {
      REQUIRE_ARGS(5);
      print_document_id( r, argv[3], argv[4] );
    } else if( command == "il" || command == "invlist" ) {
      REQUIRE_ARGS(3);
      print_invfile( r );
    } else {
      r.close();
      usage();
      return -1;
    }

    r.close();
    return 0;
  } catch( lemur::api::Exception& e ) {
    LEMUR_ABORT(e);
  }
}


