
//
// harvestlinks
//
// 3 June 2004 -- tds
//
// Anchor text mining application
//

#include <time.h>
#include "indri/Parameters.hpp"

#include "indri/UnparsedDocument.hpp"
#include "indri/ParsedDocument.hpp"
#include "indri/TaggedDocumentIterator.hpp"
#include "indri/TaggedTextParser.hpp"
#include "indri/HTMLParser.hpp"
#include "indri/AnchorTextWriter.hpp"
#include "indri/FileTreeIterator.hpp"
#include "indri/Path.hpp"
#include "lemur/Exception.hpp"
#include "indri/Combiner.hpp"

static void harvest_anchor_text_file( const std::string& path, const std::string& harvestPath, HTMLParser& parser ) {
  TaggedDocumentIterator iterator;
  iterator.open( path );
  iterator.setTags( 
    "<DOC>",              // startDocTag
    "</DOC>\n",             // endDocTag
    "</DOCHDR>"          // endMetadataTag
  );

  UnparsedDocument* unparsed;
  AnchorTextWriter writer( harvestPath );

  while( (unparsed = iterator.nextDocument()) != 0 ) {
    ParsedDocument* parsed = parser.parse( unparsed );
    writer.handle(parsed);
  }
  
  // close up everything
  iterator.close();
}

static void harvest_anchor_text( const std::string& corpusPath, const std::string& harvestPath ) {
  std::vector<std::string> include;
  include.push_back( "absolute-url" );
  include.push_back( "a" );
  std::vector<std::string> empty;
  std::map<std::string,std::string> mempty;

  HTMLParser parser;
  parser.setTags( empty, empty, include, empty, mempty );

  if( Path::isDirectory( corpusPath ) ) {
    FileTreeIterator files( corpusPath );

    for( ; files != FileTreeIterator::end(); files++ ) {
      std::string filePath = *files;
      std::string relative = Path::relative( corpusPath, filePath );
      std::string anchorText = Path::combine( harvestPath, relative );      
      std::cout << "harvesting " << filePath << std::endl;

      try {
        harvest_anchor_text_file( *files, anchorText, parser );
      } catch( Exception& e ) {
        std::cout << e.what() << std::endl;
      }
    }
  } else {
    std::string anchorText = Path::combine( harvestPath, "data" );
    harvest_anchor_text_file( corpusPath, anchorText, parser );
  }
}

int main(int argc, char * argv[]) {
  try {
    Parameters& parameters = Parameters::instance();
    parameters.loadCommandLine( argc, argv );

    if( parameters.get( "version", 0 ) ) {
      std::cout << INDRI_DISTRIBUTION << std::endl;
    }
    
    std::string corpusPath = parameters["corpus"];
    std::string outputPath = parameters["output"];
    std::string redirectPath = parameters["redirect"];
    int bins = parameters.get( "bins", 10 );

    std::string harvestPath = Path::combine( outputPath, "harvest" );
    std::string bucketPath = Path::combine( outputPath, "buckets" );
    std::string preSortPath = Path::combine( outputPath, "presort" );
    std::string sortedPath = Path::combine( outputPath, "sorted" );

    if( parameters.get( "delete", 1 ) ) {
      if( Path::isDirectory( harvestPath ) )
        Path::remove( harvestPath );
      if( Path::isDirectory( bucketPath ) )
        Path::remove( bucketPath );
      if( Path::isDirectory( preSortPath ) )
        Path::remove( preSortPath );
      if( Path::isDirectory( sortedPath ) )
        Path::remove( sortedPath );

      Path::make( harvestPath ); 
      Path::make( bucketPath ); 
      Path::make( preSortPath ); 
      Path::make( sortedPath ); 
    }

    // step 1: harvest text
    if( parameters.get( "harvest", 1 ) )
      harvest_anchor_text( corpusPath, harvestPath );
   
    Combiner combiner( bins );
    
    // step 2: hash all text into buckets
    if( parameters.get( "buckets", 1 ) )
      combiner.hashToBuckets( bucketPath, harvestPath );

    // step 3: hash all redirects into buckets
    if( parameters.get( "targets", 1 ) )
      combiner.hashRedirectTargets( bucketPath, redirectPath );

    // step 4: combine redirect buckets
    if( parameters.get( "combineredirect", 1 ) )
      combiner.combineRedirectDestinationBuckets( bucketPath );

    // step 5: combine each bin together and write back into corpus files
    if( parameters.get( "combine", 1 ) )
      combiner.combineBuckets( preSortPath, bucketPath );

    // step 6: sort resulting corpus files
    if( parameters.get( "sort", 1 ) )
      combiner.sortCorpusFiles( sortedPath, preSortPath, harvestPath );
  } catch( Exception& e ) {
    LEMUR_ABORT(e);
  }

  return 0;
}


