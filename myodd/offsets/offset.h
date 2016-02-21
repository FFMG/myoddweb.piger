// offset.h: interface for the Offset class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _Offset_h
#define _Offset_h

#pragma once
#include "../common/includes.h"

namespace myodd{ namespace offset{
  template <typename T>
  void write( const T& item, BYTE*& pData, unsigned long& ulOffset )
  {
    unsigned int uiSize = sizeof(item);
    if( ulOffset > 0 && pData)
    {
      BYTE* pTmpData = new BYTE[ulOffset + uiSize ];
      memcpy( pTmpData, pData, ulOffset );
      delete [] pData;
      pData = pTmpData;
    }
    else
    {
      pData = new BYTE[ uiSize ];
    }

    memcpy( pData+ulOffset, &item, uiSize );

    ulOffset += uiSize;
  }

  template<>
  void write<STD_TSTRING>( const STD_TSTRING& item, BYTE*& pData, unsigned long& ulOffset );

  template <typename T>
  void write( const T*& item, BYTE*& pData, unsigned long& ulOffset )
  {
    if( item )
    {
      write<T>( *item, pData, ulOffset );
    }
  }
  template<>
  void write<TCHAR>( const TCHAR*& item, BYTE*& pData, unsigned long& ulOffset );

  void write( const void* item, unsigned long unItemSize, BYTE*& pData, unsigned long& ulOffset );

  void writeFile( const TCHAR* fileName, BYTE*& pData, unsigned long& ulOffset );

  template <typename T>
  bool read( T& item, const void* pData, unsigned long& ulOffset, unsigned int uiMaxSize )
  {
    unsigned int uiSize = sizeof(item);
    if( ulOffset+uiSize > uiMaxSize )
      return false;
    memcpy( &item, (const BYTE*)pData+ulOffset, uiSize );
    ulOffset += uiSize;
    return true;
  }

  template<>
  bool read<STD_TSTRING>( STD_TSTRING& item, const void* pData, unsigned long& ulOffset, unsigned int uiMaxSize );

  template<typename T>
  bool read( T*& item, const void* pData, unsigned long& ulOffset, unsigned int uiMaxSize )
  {
    if( !item )
      return true;  //  nothing to do.
    
    return read( *item, pData, ulOffset, uiMaxSize );
  }
  template<>
  bool read<TCHAR>( TCHAR*& item, const void* pData, unsigned long& ulOffset, unsigned int uiMaxSize );

  bool read( void* item, unsigned long unItemSize, const void* pData, unsigned long& ulOffset, unsigned int uiMaxSize );

  void readFile( const TCHAR* fileName, const BYTE* pData, unsigned long& ulOffset, unsigned int uiMaxSize  );
} //  offset
} //  myodd
#endif // _Offset_h
