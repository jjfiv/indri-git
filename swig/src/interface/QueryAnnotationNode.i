
//
// QueryAnnotationNode
//
// 10 August 2004 -- tds
//

%typemap(jni) QueryAnnotationNode* "jobject"
%typemap(jtype) QueryAnnotationNode* "QueryAnnotationNode"
%typemap(jstype) QueryAnnotationNode* "QueryAnnotationNode"

%{

jobject java_build_queryannotationnode( QueryAnnotationNode* in,
                                   JNIEnv* jenv,
                                   jclass qanClazz,
                                   jmethodID qanConst ) {
  jobjectArray children = jenv->NewObjectArray(in->children.size(), qanClazz, NULL);

  for( unsigned int i=0; i<in->children.size(); i++ ) {
    jobject child = java_build_queryannotationnode( in->children[i], jenv, qanClazz, qanConst );
    jenv->SetObjectArrayElement(children, i, child);
  }

  jstring name = jenv->NewStringUTF(in->name.c_str());
  jstring type = jenv->NewStringUTF(in->type.c_str());
  jstring queryText = jenv->NewStringUTF(in->queryText.c_str());

  jobject node = jenv->NewObject(qanClazz, qanConst, name, type, queryText, children);

  return node;
}

%}

%typemap(java,out) QueryAnnotationNode* {
  jclass qanClazz = jenv->FindClass("edu/umass/cs/indri/QueryAnnotationNode" );
  const char* signature = "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[Ledu/umass/cs/indri/QueryAnnotationNode;)V";
  jmethodID qanConst = jenv->GetMethodID(qanClazz, "<init>", signature );

  $result = java_build_queryannotationnode( $1,
                                            jenv,
                                            qanClazz,
                                            qanConst );
}

%typemap(javaout) QueryAnnotationNode* {
  return $jnicall;
}

