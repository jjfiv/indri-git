
//
// ScoredExtentArray.i
//
// Copies a C++ ScoredExtentResult vector
// java ScoredExtentResult[] array.
//
// 10 August 2004 -- tds
//

%typemap(jni) std::vector<ScoredExtentResult> "jobjectArray"
%typemap(jtype) std::vector<ScoredExtentResult> "ScoredExtentResult[]"
%typemap(jstype) std::vector<ScoredExtentResult> "ScoredExtentResult[]"

%typemap(jni) const std::vector<ScoredExtentResult>& "jobjectArray"
%typemap(jtype) const std::vector<ScoredExtentResult>& "ScoredExtentResult[]"
%typemap(jstype) const std::vector<ScoredExtentResult>& "ScoredExtentResult[]"

%{
jobjectArray java_build_scoredextentresult( JNIEnv* jenv, const std::vector<ScoredExtentResult>& input ) {
  jclass clazz = jenv->FindClass("edu/umass/cs/indri/ScoredExtentResult");
  jmethodID constructor = jenv->GetMethodID(clazz, "<init>", "()V" );
  jobjectArray result;

  result = jenv->NewObjectArray(input.size(), clazz, NULL);
  if (!result) {
    return 0;
  }

  jfieldID scoreField = jenv->GetFieldID(clazz, "score", "D" );
  jfieldID beginField = jenv->GetFieldID(clazz, "begin", "I" );
  jfieldID endField = jenv->GetFieldID(clazz, "end", "I" );
  jfieldID documentField = jenv->GetFieldID(clazz, "document", "I" );

  for( jsize i=0; i<input.size(); i++ ) {
    // make a new scored extent result object
    jobject ser = jenv->NewObject(clazz, constructor);

    // fill in the fields
    jenv->SetDoubleField(ser, scoreField, input[i].score );
    jenv->SetIntField(ser, beginField, input[i].begin );
    jenv->SetIntField(ser, endField, input[i].end );
    jenv->SetIntField(ser, documentField, input[i].document );

    jenv->SetObjectArrayElement(result, i, ser);
  }

  return result;
}
%}

%typemap(java,out) std::vector<ScoredExtentResult>
{
  $result = java_build_scoredextentresult( jenv, $1 );
}

%typemap(java,out) const std::vector<ScoredExtentResult>& {
  $result = java_build_scoredextentresult( jenv, *($1) );
}

%typemap(java,in) const std::vector<ScoredExtentResult>& ( std::vector<ScoredExtentResult> resin )
{
  jsize size = jenv->GetArrayLength($input);

  jclass clazz = jenv->FindClass("edu/umass/cs/indri/ScoredExtentResult");
  jfieldID scoreField = jenv->GetFieldID(clazz, "score", "D" );
  jfieldID beginField = jenv->GetFieldID(clazz, "begin", "I" );
  jfieldID endField = jenv->GetFieldID(clazz, "end", "I" );
  jfieldID documentField = jenv->GetFieldID(clazz, "document", "I" );
  $1 = &resin;

  for( jsize i=0; i<size; i++ ) {
    jobject seobj  = jenv->GetObjectArrayElement($input, i);
    ScoredExtentResult ser;

    ser.begin = jenv->GetIntField(seobj, beginField);
    ser.end = jenv->GetIntField(seobj, endField);
    ser.document = jenv->GetIntField(seobj, documentField);
    ser.score = jenv->GetDoubleField(seobj, scoreField);

    $1->push_back( ser );
  }
}

%typemap(javain) const std::vector<ScoredExtentResult>& "$javainput"
%typemap(javaout) std::vector<ScoredExtentResult> {
  return $jnicall;
}

%typemap(javaout) const std::vector<ScoredExtentResult>& {
  return $jnicall;
}



