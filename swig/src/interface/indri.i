
%module (directors="1") indri

%{
#include "indri/indri-platform.h"
#include "lemur/lemur-compat.hpp"
#include "indri/QueryEnvironment.hpp"
#include "indri/ScoredExtentResult.hpp"
#include "indri/ParsedDocument.hpp"
#include "indri/IndexEnvironment.hpp"
#include "indri/Parameters.hpp"
#include "lemur/Exception.hpp"
%}

%include "std_string.i"
%include "exception.i"

%include "MResults.i"
%include "ParsedDocument.i"
%include "QueryAnnotationNode.i"
%include "ScoredExtentArray.i"
%include "IntVector.i"
%include "StringVector.i"
%include "Parameters.i"
%include "DocumentVector.i"
%include "MetadataPairVector.i"
%include "StringMap.i"

typedef long long INT64;
typedef long long UINT64;

%exception {
  try {
    $action
  } catch( Exception& e ) {
    SWIG_exception( SWIG_RuntimeError, e.what().c_str() );
    // control does not leave method when thrown.
    return $null;
  }
}

%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary("indri");
  }
%}

%pragma(java) jniclassimports="import java.util.Map;";
%typemap(javaimports) QueryEnvironment "import java.util.Map;"
%typemap(javaimports) IndexEnvironment "import java.util.Map;"

class QueryAnnotation {
public:
  const QueryAnnotationNode* getQueryTree() const;
  const EvaluatorNode::MResults& getAnnotations() const;
  const std::vector<ScoredExtentResult>& getResults() const;
};

class QueryEnvironment {
public:
  void addServer( const std::string& hostname );
  void addIndex( const std::string& pathname );
  void close();
  
  void setMemory( UINT64 memory );
  void setScoringRules( const std::vector<std::string>& rules );
  void setStopwords( const std::vector<std::string>& stopwords );

  std::vector<ScoredExtentResult> runQuery( const std::string& query, int resultsRequested );
  std::vector<ScoredExtentResult> runQuery( const std::string& query, const std::vector<int>& documentSet, int resultsRequested );
  QueryAnnotation* runAnnotatedQuery( const std::string& query, int resultsRequested );
  QueryAnnotation* runAnnotatedQuery( const std::string& query, const std::vector<int>& documentSet, int resultsRequested );
  
  std::vector<ParsedDocument*> documents( const std::vector<int>& documentIDs );
  std::vector<ParsedDocument*> documents( const std::vector<ScoredExtentResult>& results );

  std::vector<std::string> documentMetadata( const std::vector<int>& documentIDs, const std::string& attributeName );
  std::vector<std::string> documentMetadata( const std::vector<ScoredExtentResult>& documentIDs, const std::string& attributeName );

  INT64 termCount();
  INT64 termCount( const std::string& term );
  INT64 termFieldCount( const std::string& term, const std::string& field );
  std::vector<std::string> fieldList();
  INT64 documentCount();
  INT64 documentCount( const std::string& term );

  std::vector<DocumentVector*> documentVectors( const std::vector<int>& documentIDs );
};

%feature("director") IndexStatus;
struct IndexStatus {
  enum action_code {
    FileOpen,
    FileSkip,
    FileError,
    FileClose,
    DocumentCount
  };
  
  virtual void status( int code, const std::string& documentPath, const std::string& error, int documentsIndexed, int documentsSeen ) = 0;
};

class IndexEnvironment {
public:
  IndexEnvironment();
  ~IndexEnvironment();
  
  void setAnchorTextPath( const std::string& documentRoot, const std::string& anchorTextRoot );
  void addFileClass( const std::string& name, 
                     const std::string& iterator,
                     const std::string& parser,
                     const std::string& startDocTag,
                     const std::string& endDocTag,
                     const std::string& endMetadataTag,
                     const std::vector<std::string>& include,
                     const std::vector<std::string>& exclude,
                     const std::vector<std::string>& index,
                     const std::vector<std::string>& metadata, 
                     const std::map<std::string,std::string>& conflations );
  void setIndexedFields( const std::vector<std::string>& fieldNames );
  void setNumericField( const std::string& fieldName, bool isNumeric );
  void setMetadataIndexedFields( const std::vector<std::string>& fieldNames );
  void setStopwords( const std::vector<std::string>& stopwords );
  void setStemmer( const std::string& stemmer );
  void setMemory( UINT64 memory );

  void create( const std::string& repositoryPath, IndexStatus* callback = 0 );
  void open( const std::string& repositoryPath, IndexStatus* callback = 0 );
  void close();

  void addFile( const std::string& fileName );
  void addFile( const std::string& fileName, const std::string& fileClass );

  void addString( const std::string& fileName, const std::string& fileClass, const std::vector<MetadataPair>& metadata );
  void addParsedDocument( ParsedDocument* document );
};

