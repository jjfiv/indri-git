
//
// dumpindex
//
// 13 September 2004 -- tds
//

#include "indri/IndriIndex.hpp"
#include "indri/Repository.hpp"
#include "indri/DocListFrequencyIterator.hpp"
#include "indri/CompressedCollection.hpp"
#include <iostream>

int count_term_in_documents( Repository& r, int termID, std::vector<int>& documents );

void print_term_positions( Repository& r, const std::string& termString ) {
  std::string stem = r.processTerm( termString );

  int termID = r.index()->term( stem.c_str() );

  std::cout << termString << " "
            << termID << " "
            << r.index()->termCount( termID ) << " " 
            << r.index()->termCount() << " " << std::endl;

  indri::index::DocListIterator* iter = (indri::index::DocListIterator*) r.index()->docInfoList( termID );
  iter->startIteration();

  int doc = 0;
  indri::index::DocListInfo* entry;

  while( true ) {
    iter->nextEntry( doc );
    entry = (indri::index::DocListInfo*) iter->currentEntry();

    if(!entry) {
       break;
    }

    std::cout << entry->docID() << " "
              << entry->termCount() << " "
              << r.index()->docLength( entry->docID() ) << " ";

    int count = entry->termCount();

    for( int i=0; i<count; i++ ) {
      std::cout << entry->positions()[i] << " ";
    }

    std::cout << std::endl;
    doc = entry->docID() + 1 ;
  }

  delete iter;
}

void print_term_counts( Repository& r, const std::string& termString ) {
 std::string stem = termString; // r.processTerm( termString );

  int termID = r.index()->term( stem.c_str() );

  std::cout << termString << " "
            << termID << " "
            << r.index()->termCount( termID ) << " " 
            << r.index()->termCount() << " " << std::endl;

  indri::index::DocListFrequencyIterator* iter = r.index()->docFrequencyInfoList( termID );
  iter->startIteration();

  int doc = 0;
  const DocumentCount* entry;

  while( true ) {
    iter->nextEntry( doc );
    entry = iter->currentEntry();

    if(!entry) {
       break;
    }

    std::cout << entry->document << " "
              << entry->count << " "
              << r.index()->docLength( entry->document ) << "\n";
    doc = entry->document + 1 ;
  }
  delete iter;
}

void print_vocabulary( Repository& r ) {
  IndriIndex* index = r.index();
  int unique = index->termCountUnique();

  for( int i=0; i<=unique; i++ ) {
    std::cout << index->term(i)
              << " "
              << index->termCount(i)
              << " "
              << index->docCount(i)
              << std::endl;
  }
}

void print_term_name( Repository& r, const char* number ) {
  int id = atoi( number );
  IndriIndex* index = r.index();

  std::cout << index->term( id ) << std::endl;
}

void print_document_name( Repository& r, const char* number ) {
  int id = atoi( number );
  IndriIndex* index = r.index();

  std::cout << index->document( id ) << std::endl;
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
  int documentID = atoi( number );
  IndriIndex* index = r.index();

  indri::index::TermListBuilder* list = index->termPositionList( documentID );

  if( list ) {
    for( int i=0; i<list->terms().size(); i++ ) {
      std::cout << i << " " << list->terms()[i] << std::endl;
    }
  }

  delete list;
}

int main( int argc, char** argv ) {
  if( argc < 3 ) {
    std::cout << "dumpindex <repository> <command> [ <argument> ]*" << std::endl;
    std::cout << "Valid commands are: " << std::endl;
    std::cout << "    Command              Argument       Description" << std::endl;
    std::cout << "    term (t)             Term text      Print inverted list for a term" << std::endl;
    std::cout << "    termpositions (tp)   Term text      Print inverted list for a term, with positions" << std::endl;
    std::cout << "    vocabulary (v)       None           Print the vocabulary of the index, with counts" << std::endl;
    std::cout << "    termname (tn)        Term ID        Print the text representation of a term ID" << std::endl;
    std::cout << "    documentname (dn)    Document ID    Print the text representation of a document ID" << std::endl;
    std::cout << "    documenttext (dt)    Document ID    Print the text of a document" << std::endl;
    std::cout << "    documenttext (dd)    Document ID    Print the full representation of a document" << std::endl;
    std::cout << "    documentvector (dv)  Document ID    Print the document vector of a document" << std::endl;
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
  } else if( command == "v" || command == "vocabulary" ) {
    print_vocabulary( r );
  } else if( command == "tn" || command == "termname" ) {
    print_term_name( r, argv[3] );
  } else if( command == "dn" || command == "documentname" ) {
    print_document_name( r, argv[3] );
  } else if( command == "dt" || command == "documenttext" ) {
    print_document_text( r, argv[3] );
  } else if( command == "dd" || command == "documentdata" ) {
    print_document_data( r, argv[3] );
  } else if( command == "dv" || command == "documentvector" ) {
    print_document_vector( r, argv[3] );
  }

  r.close();
  return 0;
}


