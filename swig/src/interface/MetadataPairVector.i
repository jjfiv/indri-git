

//
// MetadataPairVector.i
//
// 24 August 2004 -- tds
//

%typemap(jni) const std::vector<MetadataPair>& "jobjectArray"
%typemap(jtype) const std::vector<MetadataPair>& "Map"
%typemap(jstype) const std::vector<MetadataPair>& "Map"

%typemap(java,in) const std::vector<MetadataPair>& ( std::vector<MetadataPair> mdin, Buffer mdbuf ) {
  // call map.entrySet()
  jclass mapClazz = jenv->GetObjectClass( $input );
  jmethodID mapEntrySetMethod = jenv->GetMethodID( mapClazz, "entrySet", "()Ljava/util/Set;" );
  jobject mapEntrySet = jenv->CallObjectMethod( $input, mapEntrySetMethod );

  // call entrySet.toArray()
  jclass entrySetClazz = jenv->GetObjectClass( mapEntrySet );
  jmethodID entrySetToArrayMethod = jenv->GetMethodID( entrySetClazz, "toArray", "()[Ljava/lang/Object;" );
  jobjectArray entryArray = (jobjectArray) jenv->CallObjectMethod( mapEntrySet, entrySetToArrayMethod );

  // get array length
  jsize arrayLength = jenv->GetArrayLength( entryArray );
  $1 = &mdin;

  jclass stringClazz = jenv->FindClass("edu/java/lang/String");

  for( unsigned int i=0; i<arrayLength; i++ ) {
    jobject mapEntry = jenv->GetObjectArrayElement( entryArray, i );
    jclass mapEntryClazz = jenv->GetObjectClass( mapEntry );
    jmethodID mapEntryGetKeyMethod = jenv->GetMethodID( mapEntryClazz, "getKey", "()Ljava/lang/Object;" );
    jmethodID mapEntryGetValueMethod = jenv->GetMethodID( mapEntryClazz, "getValue", "()Ljava/lang/Object;" );

    jobject key = jenv->CallObjectMethod( mapEntry, mapEntryGetKeyMethod );
    jobject value = jenv->CallObjectMethod( mapEntry, mapEntryGetValueMethod );

    const char* keyChars = jenv->GetStringUTFChars( (jstring) key, 0 );
    jsize keyLength = jenv->GetStringUTFLength( (jstring) key);
    std::string keyString = keyChars;
    char* keyPosition = mdbuf.write( keyLength+1 );
    strncpy( keyPosition, keyChars, keyLength );
    keyPosition[keyLength] = 0;
    char* valuePosition = 0;
    jsize valueLength;

    if( jenv->IsInstanceOf( value, stringClazz ) ) {
      jstring valueString = (jstring) value;
      const char* valueChars = jenv->GetStringUTFChars( valueString, 0);
      valueLength = jenv->GetStringUTFLength( valueString );

      valuePosition = mdbuf.write( valueLength+1 );
      strncpy( valuePosition, valueChars, valueLength );
      valuePosition[valueLength] = 0;
      valueLength++;
      
      jenv->ReleaseStringUTFChars(valueString, valueChars);
    } else {
      // is byte array
      jbyteArray valueArray = (jbyteArray) value;
      jbyte* valueBytes = jenv->GetByteArrayElements( valueArray, 0 );
      valueLength = jenv->GetArrayLength( valueArray );
  
      valuePosition = mdbuf.write( valueLength+1 );
      memcpy( valuePosition, valueBytes, valueLength );
      valuePosition[valueLength] = 0;

      jenv->ReleaseByteArrayElements(valueArray, valueBytes, 0);
    }

    MetadataPair pair;
    pair.key = keyPosition;
    pair.value = valuePosition;
    pair.valueLength = valueLength;
    mdin.push_back(pair);

    jenv->ReleaseStringUTFChars( (jstring)key, keyChars);
  }
}

%typemap(javain) const std::vector<MetadataPair>& "$javainput";

