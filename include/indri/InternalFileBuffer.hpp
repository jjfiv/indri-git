
//
// InternalFileBuffer
//
// 2004 November 24 -- tds
//

#ifndef INDRI_INTERNALFILEBUFFER_HPP
#define INDRI_INTERNALFILEBUFFER_HPP

#include "indri/Buffer.hpp"

struct InternalFileBuffer {
  InternalFileBuffer( size_t length ) {
    buffer.grow(length);
    filePosition = 0;
  }

  Buffer buffer;
  UINT64 filePosition;
};

#endif // INDRI_INTERNALFILEBUFFER_HPP
