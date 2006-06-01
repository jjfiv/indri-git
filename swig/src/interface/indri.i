
%module (directors="1") indri

%{
#include "indri/indri-platform.h"
#include "lemur/lemur-compat.hpp"
#include "indri/QueryEnvironment.hpp"
#include "indri/QueryExpander.hpp"
#include "indri/RMExpander.hpp"
#include "indri/PonteExpander.hpp"
#include "indri/ScoredExtentResult.hpp"
#include "indri/ParsedDocument.hpp"
#include "indri/IndexEnvironment.hpp"
#include "indri/Parameters.hpp"
#include "indri/ConflationPattern.hpp"
#include "lemur/Exception.hpp"
%}

%include "std_string.i"
%include "exception.i"

%include "ConflationPattern.i"
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

%include "Specification.i"

typedef long long INT64;
typedef long long UINT64;

%exception {
  try {
    $action
  } catch( lemur::api::Exception& e ) {
    SWIG_exception(SWIG_RuntimeError, e.what().c_str() );
    // control does not leave method when thrown. (fixed in 1.3.25
    // return $null;
  }
}

%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary("indri_jni");
  }
%}

%pragma(java) jniclassimports="import java.util.Map;";
%typemap(javaimports) indri::api::QueryEnvironment "import java.util.Map;"
%typemap(javaimports) indri::api::IndexEnvironment "import java.util.Map;"
namespace indri {
  namespace api {
class QueryAnnotation {
public:
  const indri::api::QueryAnnotationNode* getQueryTree() const;
  const indri::infnet::EvaluatorNode::MResults& getAnnotations() const;
  const std::vector<indri::api::ScoredExtentResult>& getResults() const;
};

class QueryEnvironment {
public:
  void addServer( const std::string& hostname );
  void addIndex( const std::string& pathname );
  void removeServer( const std::string& hostname );
  void removeIndex( const std::string& pathname );
  void close();
  
  void setMemory( UINT64 memory );
  void setScoringRules( const std::vector<std::string>& rules );
  void setStopwords( const std::vector<std::string>& stopwords );

  std::vector<indri::api::ScoredExtentResult> runQuery( const std::string& query, int resultsRequested );
  std::vector<indri::api::ScoredExtentResult> runQuery( const std::string& query, const std::vector<int>& documentSet, int resultsRequested );
  indri::api::QueryAnnotation* runAnnotatedQuery( const std::string& query, int resultsRequested );
  indri::api::QueryAnnotation* runAnnotatedQuery( const std::string& query, const std::vector<int>& documentSet, int resultsRequested );
  
  std::vector<indri::api::ParsedDocument*> documents( const std::vector<int>& documentIDs );
  std::vector<indri::api::ParsedDocument*> documents( const std::vector<indri::api::ScoredExtentResult>& results );

  std::vector<std::string> documentMetadata( const std::vector<int>& documentIDs, const std::string& attributeName );
  std::vector<std::string> documentMetadata( const std::vector<indri::api::ScoredExtentResult>& documentIDs, const std::string& attributeName );

  std::vector<int> documentIDsFromMetadata( const std::string& attributeName, const std::vector<std::string>& attributeValue );
  std::vector<indri::api::ParsedDocument*> documentsFromMetadata( const std::string& attributeName, const std::vector<std::string>& attributeValue );

  INT64 termCount();
  INT64 termCount( const std::string& term );
  INT64 termFieldCount( const std::string& term, const std::string& field );
  std::vector<std::string> fieldList();
  INT64 documentCount();
  INT64 documentCount( const std::string& term );

  std::vector<indri::api::DocumentVector*> documentVectors( const std::vector<int>& documentIDs );
      double expressionCount( const std::string& expression,
                              const std::string &queryType = "indri" );
      std::vector<indri::api::ScoredExtentResult> expressionList( const std::string& expression, 
                                                      const std::string& queryType = "indri" );

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

  void setDocumentRoot( const std::string& documentRoot );  
  void setAnchorTextPath( const std::string& anchorTextRoot );
  void setOffsetMetadataPath( const std::string& offsetMetadataRoot );
  void setOffsetAnnotationsPath( const std::string& offsetAnnotationsRoot );

  void addFileClass( const std::string& name, 
                     const std::string& iterator,
                     const std::string& parser,
                     const std::string& tokenizer,
                     const std::string& startDocTag,
                     const std::string& endDocTag,
                     const std::string& endMetadataTag,
                     const std::vector<std::string>& include,
                     const std::vector<std::string>& exclude,
                     const std::vector<std::string>& index,
                     const std::vector<std::string>& metadata, 
                     const std::map<indri::parse::ConflationPattern *,std::string>& conflations );
  indri::parse::FileClassEnvironmentFactory::Specification *getFileClassSpec( const std::string& name);
  void addFileClass( const indri::parse::FileClassEnvironmentFactory::Specification &spec);
  void deleteDocument( int documentID );
  void setIndexedFields( const std::vector<std::string>& fieldNames );
  void setNumericField( const std::string& fieldName, bool isNumeric, const std::string & parserName="" );
  void setMetadataIndexedFields( const std::vector<std::string>& forward, const std::vector<std::string>& backward );
  void setStopwords( const std::vector<std::string>& stopwords );
  void setStemmer( const std::string& stemmer );
  void setMemory( UINT64 memory );
  void setNormalization( bool normalize );

  void create( const std::string& repositoryPath, indri::api::IndexStatus* callback = 0 );
  void open( const std::string& repositoryPath, indri::api::IndexStatus* callback = 0 );
  void close();

  void addFile( const std::string& fileName );
  void addFile( const std::string& fileName, const std::string& fileClass );

  int addString( const std::string& fileName, const std::string& fileClass, const std::vector<indri::parse::MetadataPair>& metadata );
  int addParsedDocument( indri::api::ParsedDocument* document );
  int documentsIndexed();
  int documentsSeen();
};
}
}
%typemap(javaimports) indri::query::QueryExpander "import java.util.Map;"
%typemap(javaimports) indri::query::RMExpander "import java.util.Map;"
%typemap(javaimports) indri::query::PonteExpander "import java.util.Map;"

namespace indri
{
  namespace query
  {
    
    class QueryExpander {
    public:
      QueryExpander( indri::api::QueryEnvironment * env , indri::api::Parameters& param );
      virtual ~QueryExpander() {};

      // runs original query, expands query based on results ( via expand( .. ) ), then runs expanded query
      std::vector<indri::api::ScoredExtentResult> runExpandedQuery( std::string originalQuery , int resultsRequested , bool verbose = false );
  
      // creates expanded query from an original query and a ranked list of documents
      virtual std::string expand( std::string originalQuery , std::vector<indri::api::ScoredExtentResult>& results ) = 0;
    };

    class RMExpander : public QueryExpander  {
    public:
      RMExpander( indri::api::QueryEnvironment * env , indri::api::Parameters& param );

      virtual std::string expand( std::string originalQuery , std::vector<indri::api::ScoredExtentResult>& results );
    };
    class PonteExpander : public QueryExpander  {
    public:
      PonteExpander( indri::api::QueryEnvironment * env , indri::api::Parameters& param );

      virtual std::string expand( std::string originalQuery, std::vector<indri::api::ScoredExtentResult>& results );
    };

  }
}

