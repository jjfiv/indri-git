
//
// File
//
// 15 November 2004 -- tds
//

#include <string>
#include "indri/indri-platform.h"
#include "indri/File.hpp"

#ifndef WIN32
#include <fcntl.h>
#include <sys/stat.h>
#endif

#include "lemur/Exception.hpp"
#include "indri/ScopedLock.hpp"

//
// File constructor
//

File::File() :
#ifdef WIN32
  _handle(INVALID_HANDLE_VALUE)
#else
  _handle(-1)
#endif
{
}

//
// File destructor
//

File::~File() {
  close();
}

//
// create
// 

bool File::create( const std::string& filename ) {
#ifdef WIN32
  _handle = ::CreateFile( filename.c_str(),
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          CREATE_ALWAYS,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL );

  if( _handle == INVALID_HANDLE_VALUE )
    return false;

  return true;
#else 
  _handle = creat( filename.c_str(), 0600 );

  if( _handle < 0 )
    return false;

  return true;
#endif
}

bool File::open( const std::string& filename ) {
#ifdef WIN32
  _handle = ::CreateFile( filename.c_str(),
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL );
  
  if( _handle == INVALID_HANDLE_VALUE )
    return false;
  
  return true;
#else 
#ifdef __APPLE__
  _handle = ::open( filename.c_str(), O_RDWR );
#else
  _handle = ::open( filename.c_str(), O_LARGEFILE | O_RDWR );
#endif
  
  if( _handle < 0 )
    return false;

  return true;
#endif
}

bool File::openRead( const std::string& filename ) {
#ifdef WIN32
  _handle = ::CreateFile( filename.c_str(),
                          GENERIC_READ,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL );
  
  if( _handle == INVALID_HANDLE_VALUE )
    return false;

  return true;
#else 
#ifdef __APPLE__
  _handle = ::open( filename.c_str(), O_RDONLY );
#else
  _handle = ::open( filename.c_str(), O_LARGEFILE | O_RDONLY );
#endif
  
  if( _handle < 0 )
    return false;
  
  return true;
#endif
}

size_t File::read( void* buffer, UINT64 position, size_t length ) {
#ifdef WIN32
  assert( _handle != INVALID_HANDLE_VALUE );

  ScopedLock sl( _mutex );
  LARGE_INTEGER actual;
  BOOL result;
  LARGE_INTEGER largePosition;
  largePosition.QuadPart = position;

  result = SetFilePointerEx( _handle, largePosition, &actual, FILE_BEGIN ); 
  assert( largePosition.QuadPart == actual.QuadPart );

  if( !result )
    LEMUR_THROW( LEMUR_IO_ERROR, "Failed to seek to some file position" );

  DWORD actualBytes;
  result = ::ReadFile( _handle, buffer, length, &actualBytes, NULL );

  if( !result )
    LEMUR_THROW( LEMUR_IO_ERROR, "Error when reading file" );

  return actualBytes;
#else // POSIX
  assert( _handle > 0 );
  ssize_t result = ::pread( _handle, buffer, length, position ); 

  if( result < 0 )
    LEMUR_THROW( LEMUR_IO_ERROR, "Error when reading file" );

  return size_t(result);
#endif
}

size_t File::write( const void* buffer, UINT64 position, size_t length ) {
  if( length == 0 )
    return 0;

#ifdef WIN32
  assert( _handle != INVALID_HANDLE_VALUE );

  ScopedLock sl( _mutex );
  LARGE_INTEGER actual;
  BOOL result;
  LARGE_INTEGER largePosition;
  largePosition.QuadPart = position;

  result = SetFilePointerEx( _handle, largePosition, &actual, FILE_BEGIN ); 
  assert( largePosition.QuadPart == actual.QuadPart );

  if( !result )
    LEMUR_THROW( LEMUR_IO_ERROR, "Failed to seek to some file position" );

  DWORD actualBytes;
  result = ::WriteFile( _handle, buffer, length, &actualBytes, NULL );

  if( !result )
    LEMUR_THROW( LEMUR_IO_ERROR, "Error when writing file" );

  return actualBytes;
#else // POSIX
  assert( _handle > 0 );

  ssize_t result = ::pwrite( _handle, buffer, length, position );

  if( result < 0 )
    LEMUR_THROW( LEMUR_IO_ERROR, "Error when writing file" );

  return size_t(result);
#endif
}

void File::close() {
#ifdef WIN32
  if( _handle != INVALID_HANDLE_VALUE ) {
    ::CloseHandle( _handle );
    _handle = INVALID_HANDLE_VALUE;
  }
#else
  if( _handle >= 0 ) {
    ::close( _handle );
    _handle = -1;
  }
#endif
}

UINT64 File::size() {
#ifdef WIN32
  LARGE_INTEGER length;
  BOOL result = ::GetFileSizeEx( _handle, &length );

  if( !result )
    LEMUR_THROW( LEMUR_IO_ERROR, "Got an error while trying to retrieve file size" );

  return length.QuadPart;
#else // POSIX
  struct stat stats;
  fstat( _handle, &stats );
  return stats.st_size;
#endif
}

