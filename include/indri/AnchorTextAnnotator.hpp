/*==========================================================================
 * Copyright (c) 2003-2004 University of Massachusetts.  All Rights Reserved.
 *
 * Use of the Lemur Toolkit for Language Modeling and Information Retrieval
 * is subject to the terms of the software license set forth in the LICENSE
 * file included with this software, and also available at
 * http://www.lemurproject.org/license.html
 *
 *==========================================================================
 */


//
// AnchorTextAnnotator
//
// 25 May 2004 -- tds
//
// Reads anchor text in from files created by the 
// combiner, and adds the text to the end of the
// parsed document
//

#ifndef INDRI_ANCHORTEXTANNOTATOR_HPP
#define INDRI_ANCHORTEXTANNOTATOR_HPP
#include "zlib.h"
#include "indri/Buffer.hpp"
#include "indri/Transformation.hpp"
#include <iostream>
#include "indri/TagExtent.hpp"
#include "indri/ParsedDocument.hpp"
#include "indri/TokenizedDocument.hpp"
#include "indri/TokenizerFactory.hpp"
#include <fstream>
#include <algorithm>

/*! Top level namespace for all indri components. */
namespace indri
{
  /*! \brief File input, parsing, stemming, and stopping classes. */
  namespace parse
  {
    /*! Reads anchor text in from files created by the 
      combiner, and adds the text to the end of the parsed document
    */    
    class AnchorTextAnnotator : public Transformation {
      gzFile _in;
      indri::utility::Buffer _gzbuffer;
      indri::parse::Tokenizer* tokenizer;
      indri::parse::TokenizedDocument* tokenized;
      
      char _docno[256];
      int _count;
      indri::utility::Buffer _buffer;
      ObjectHandler<indri::api::ParsedDocument>* _handler;

      bool _readLine( char* beginLine ) {
        size_t lineLength = 0;
        size_t actual;

        // make a buffer of a reasonable size so we're not always allocating
        if( _gzbuffer.size() < 1024*1024 )
          _gzbuffer.grow( 1024*1024 );
        // if we're running out of room, add 25MB
        if( (_gzbuffer.size() -  _gzbuffer.position()) < 512*1024 ) {
          _gzbuffer.grow( _gzbuffer.size() + 1024*1024*25 );
        }

        size_t readAmount = _gzbuffer.size() - _gzbuffer.position() - 2;

        // fetch next document line
        char* buffer = _gzbuffer.write( readAmount );
        char* result = gzgets( _in, buffer, (int)readAmount );
        
        if(!result) {
          return false;
        }
        // strip the newline
        actual = strlen(buffer);
        buffer[actual - 1] = 0;
        lineLength += actual; 
        _gzbuffer.unwrite( readAmount - actual );
        
        // all finished reading
        *_gzbuffer.write(1) = 0;
        strncpy(beginLine, (_gzbuffer.front() + _gzbuffer.position() - lineLength - 1), lineLength);
        return true;
        
      }
      
      void _readDocumentHeader() {
        char line[65536 * 10];
        bool result;
        _count = 0;

        // DOCNO=
        result = _readLine( _docno );
        if( !result ) {
          return;
        }
        // DOCURL=
        result = _readLine( line );
        if( !result ) {
          return;
        }
        // LINKS=
        result = _readLine( line );
        if( !result ) {
          return;
        }
        _count = atoi( line+6 );
      }

      void _fetchText( indri::utility::greedy_vector<TagExtent *>& tags, 
                       indri::utility::greedy_vector<char*>& terms ) {
        // now, fetch the additional terms
        char line[65536 * 10];
        bool result;
        _buffer.clear();
        for( int i=0; i<_count; i++ ) {
          // LINK
          result = _readLine( line );
          if( !result ) {
            return;
          }

          // LINKDOCNO 
          result = _readLine( line );
          if( !result ) {
            return;
          }
          
          // TEXT=
          result = _readLine( line );
          if( !result ) {
            return;
          }

          if (!line[0]) continue;
          char *textStart = line+6;
          size_t textLen = strlen(textStart);
          // now insert the terms with a separate inlink for each
          // line.
          indri::parse::UnparsedDocument document;
          document.text = textStart;
          document.textLength = textLen + 1; // for the null
          document.content = document.text;
          document.contentLength = document.textLength - 1 ;

          tokenized = tokenizer->tokenize( &document );
          if (tokenized->terms.size() == 0) continue;
          int beginIndex = (int)terms.size();
          indri::utility::greedy_vector<char*>::iterator j  = 
            tokenized->terms.begin();
          // copy the terms into a persistent buffer
          while (j != tokenized->terms.end()) {
            size_t len = strlen (*j);            
            char *term = _buffer.write(len + 1);
            strncpy(term, *j, len);
            term[len] = '\0';
            terms.push_back( term );
            j++;
          }
          TagExtent * extent = new TagExtent;
          extent->name = "inlink";
          extent->begin = beginIndex;
          extent->end = (int)terms.size();
          extent->number = 0;
          extent->parent = 0;
          tags.push_back(extent);
        }
      }
      
      bool _matchingDocno( indri::api::ParsedDocument* document ) {
        // find DOCNO attribute in document
        for( size_t i=0; i<document->metadata.size(); i++ ) {
          const char* attributeName = document->metadata[i].key;
          const char* attributeValue = (const char*) document->metadata[i].value;

          if( !strcmp( attributeName, "docno" ) ) {
            if( !strcmp( attributeValue, _docno+6 ) ) {
              return true;
            } else {
              return false;
            }
          }
        }
 
        return false;
      }

  public:
    AnchorTextAnnotator() {
      tokenizer = indri::parse::TokenizerFactory::get("Word");
      _handler = 0;
    }

    ~AnchorTextAnnotator() {
      if( _in )
        gzclose( _in );
      _in = 0;
      delete tokenizer;
    }

    void open( const std::string& anchorFile ) {
      if( _in )
        gzclose( _in );
      _in = gzopen( anchorFile.c_str(), "rb" );
      if( !_in )
        LEMUR_THROW( LEMUR_IO_ERROR, "Couldn't open file " + anchorFile + "." );
      _buffer.clear();
      _gzbuffer.clear();
      _readDocumentHeader();
    }

    indri::api::ParsedDocument* transform( indri::api::ParsedDocument* document ) {
      _buffer.clear();
      _buffer.grow(2*1024*1024);
        
      _gzbuffer.clear();
      _gzbuffer.grow( 1024*1024 );
      // surround current text with a mainbody tag
      TagExtent * mainbody = new TagExtent;
      mainbody->begin = 0;
      mainbody->end = (int)document->terms.size();
      mainbody->name = "mainbody";
      mainbody->number = 0;
      mainbody->parent = 0;
      // order no longer matters - the indexer takes care of any sorting needed
      document->tags.push_back( mainbody );

      // set old tags' parent to mainbody
      indri::utility::greedy_vector<TagExtent *>::iterator oldTag = document->tags.begin();
      indri::utility::greedy_vector<TagExtent *>::iterator oldTagsEnd = document->tags.end();
      while ( oldTag != oldTagsEnd ) {
        if ((*oldTag)->parent == 0) {
          (*oldTag)->parent = mainbody;
        }
        oldTag++;
      }

      if( _matchingDocno( document ) ) {
        _fetchText( document->tags, document->terms );
        _readDocumentHeader();
      }
      std::sort( document->tags.begin(), document->tags.end(), indri::parse::LessTagExtent() );
      return document;
    }

    void setHandler( ObjectHandler<indri::api::ParsedDocument>& handler ) {
      _handler = &handler;
    }

    void handle( indri::api::ParsedDocument* document ) {
      _handler->handle( transform( document ) );
    }
  };
}
}

#endif // INDRI_ANCHORTEXTANNOTATOR_HPP

