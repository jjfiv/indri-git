
//
// IntVector.i
//
// 10 August 2004 -- tds
//


%typemap(jni) const std::vector<int>& "jintArray"
%typemap(jtype) const std::vector<int>& "int[]"
%typemap(jstype) const std::vector<int>& "int[]"

%typemap(java,in) const std::vector<int>& ( std::vector<int> typemapin ) {
  jsize arrayLength = jenv->GetArrayLength($input);
  jint* elements = jenv->GetIntArrayElements($input, 0);
  $1 = &typemapin;

  for( unsigned int i=0; i<arrayLength; i++ ) {
    $1->push_back(elements[i]);
  }

  jenv->ReleaseIntArrayElements($input, elements, 0);
}

%typemap(javain) const std::vector<int>& "$javainput";

