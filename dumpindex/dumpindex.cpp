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

int count_term_in_documents( Repository& r, int termID, std::vector<int>& documents );

void print_field_positions( Repository& r, const std::string& fieldString ) {
  LocalQueryServer local(r);

  UINT64 totalCount = local.termCount();

  std::cout << fieldString << std::endl;

  Repository::index_state state = r.indexes();

  for( size_t i=0; i<state->size(); i++ ) {
    indri::index::Index* index = (*state)[i];
    ScopedLock( index->iteratorLock() );

    indri::index::DocExtentListIterator* iter = index->fieldListIterator( fieldString );
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

void print_term_positions( Repository& r, const std::string& termString ) {
  std::string stem = r.processTerm( termString );
  LocalQueryServer local(r);

  UINT64 totalCount = local.termCount();
  UINT64 termCount = local.termCount( termString );

  std::cout << termString << " "
            << stem << " "
            << termCount << " " 
            << totalCount << " " << std::endl;

  Repository::index_state state = r.indexes();

  for( size_t i=0; i<state->size(); i++ ) {
    indri::index::Index* index = (*state)[i];
    ScopedLock( index->iteratorLock() );

    indri::index::DocListIterator* iter = index->docListIterator( stem );
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

void print_term_counts( Repository& r, const std::string& termString ) {
  std::string stem = r.processTerm( termString );
  LocalQueryServer local(r);

  UINT64 totalCount = local.termCount();
  UINT64 termCount = local.termCount( termString );

  std::cout << termString << " "
            << stem << " "
            << termCount << " " 
            << totalCount << " " << std::endl;

  Repository::index_state state = r.indexes();

  for( size_t i=0; i<state->size(); i++ ) {
    indri::index::Index* index = (*state)[i];
    ScopedLock( index->iteratorLock() );

    indri::index::DocListIterator* iter = index->docListIterator( stem );
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

void print_document_name( Repository& r, const char* number ) {
  CompressedCollection* collection = r.collection();
  std::string documentName = collection->retrieveMetadatum( atoi( number ), "docid" );
  std::cout << documentName << std::endl;
}

void print_document_text( Repository& r, const char* number ) {
  int documentID = atoi( number );
  CompressedCollection* collection = r.collection();
  ParsedDocument* document = collection->retrieve( documentID );

  std::cout << document->text << std::endl;
  delete document;
}

void print_document_data( Repository& r, const char* number ) {
  int documentID = atoi( number );
  CompressedCollection* collection = r.collection();
  ParsedDocument* document = collection->retrieve( documentID );

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

void print_document_vector( Repository& r, const char* number ) {
  LocalQueryServer local(r);
  DOCID_T documentID = atoi( number );

  std::vector<DOCID_T> documentIDs;
  documentIDs.push_back(documentID);

  QueryServerVectorsResponse* response = local.documentVectors( documentIDs );
  
  if( response->getResults().size() ) {
    DocumentVector* docVector = response->getResults()[0];
  
    std::cout << "--- Fields ---" << std::endl;

    for( int i=0; i<docVector->fields().size(); i++ ) {
      const DocumentVector::Field& field = docVector->fields()[i];
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

void print_document_id( Repository& r, const char* an, const char* av ) {
  CompressedCollection* collection = r.collection();
  std::string attributeName = an;
  std::string attributeValue = av;
  std::vector<DOCID_T> documentIDs;

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

int main( int argc, char** argv ) {
  try {
    if( argc < 4 || ( ( argv[2] == "di" || argv[2] == "documentid" ) && argc < 5 ) ) {
      usage();
      return -1;
    }

    Repository r;
    char* repName = argv[1];
    r.openRead( repName );

    std::string command = argv[2];

    if( command == "t" || command == "term" ) {
      std::string term = argv[3];
      print_term_counts( r, term );
    } else if( command == "tp" || command == "termpositions" ) { 
      std::string term = argv[3];
      print_term_positions( r, term );
    } else if( command == "fp" || command == "fieldpositions" ) { 
      std::string field = argv[3];
      print_field_positions( r, field );
    } else if( command == "dn" || command == "documentname" ) {
      print_document_name( r, argv[3] );
    } else if( command == "dt" || command == "documenttext" ) {
      print_document_text( r, argv[3] );
    } else if( command == "dd" || command == "documentdata" ) {
      print_document_data( r, argv[3] );
    } else if( command == "dv" || command == "documentvector" ) {
      print_document_vector( r, argv[3] );
    } else if( command == "di" || command == "documentid" ) {
      print_document_id( r, argv[3], argv[4] );
    } else {
      r.close();
      usage();
      return -1;
    }

    r.close();
    return 0;
  } catch( Exception& e ) {
    LEMUR_ABORT(e);
  }
}


