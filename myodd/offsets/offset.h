// offset.h: interface for the Offset class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _Offset_h
#define _Offset_h

#pragma once
#include "../common/includes.h"

namespace myodd{ namespace offset{
  template <typename T>
  void write( const T& item, unsigned char*& pData, size_t& ulOffset )
  {
    unsigned int uiSize = sizeof(item);
    if( ulOffset > 0 && pData)
    {
      unsigned char* pTmpData = new unsigned char[ulOffset + uiSize ];
      memcpy( pTmpData, pData, ulOffset );
      delete [] pData;
      pData = pTmpData;
    }
    else
    {
      pData = new unsigned char[ uiSize ];
    }

    memcpy( pData+ulOffset, &item, uiSize );

    ulOffset += uiSize;
  }

  template<>
  void write<MYODD_STRING>( const MYODD_STRING& item, unsigned char*& pData, size_t& ulOffset );

  template <typename T>
  void write( const T*& item, unsigned char*& pData, size_t& ulOffset )
  {
    if( item )
    {
      write<T>( *item, pData, ulOffset );
    }
  }
  template<>
  void write<MYODD_CHAR>( const MYODD_CHAR*& item, unsigned char*& pData, size_t& ulOffset );

  void write( const void* item, size_t unItemSize, unsigned char*& pData, size_t& ulOffset );

  void writeFile( const MYODD_CHAR* fileName, unsigned char*& pData, size_t& ulOffset );

  template <typename T>
  bool read( T& item, const void* pData, size_t& ulOffset, size_t uiMaxSize )
  {
    unsigned int uiSize = sizeof(item);
    if( ulOffset+uiSize > uiMaxSize )
      return false;
    memcpy( &item, (const unsigned char*)pData+ulOffset, uiSize );
    ulOffset += uiSize;
    return true;
  }

  template<>
  bool read<MYODD_STRING>( MYODD_STRING& item, const void* pData, size_t& ulOffset, size_t uiMaxSize );

  template<typename T>
  bool read( T*& item, const void* pData, size_t& ulOffset, size_t uiMaxSize )
  {
    if( !item )
      return true;  //  nothing to do.
    
    return read( *item, pData, ulOffset, uiMaxSize );
  }
  template<>
  bool read<MYODD_CHAR>(MYODD_CHAR*& item, const void* pData, size_t& ulOffset, size_t uiMaxSize );

  bool read( void* item, size_t unItemSize, const void* pData, size_t& ulOffset, size_t uiMaxSize );

  void readFile( const MYODD_CHAR* fileName, const unsigned char* pData, size_t& ulOffset, size_t uiMaxSize  );
} //  offset
} //  myodd
#endif // _Offset_h
