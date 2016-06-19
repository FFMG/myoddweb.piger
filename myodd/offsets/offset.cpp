#include "stdafx.h"

#include <assert.h>
#include "offset.h"
#include "../files/files.h"
#include "../string/string.h"

namespace myodd{ namespace offset{
  /**
   * Write a file, (the actual data, not the filename), to the given buffer.
   * @param const TCHAR* the file we want to write to the buffer.
   * @param BYTE*& the data that will contain the file contents.
   * @param unsigned long& the current offset position at the start and end of the operation
   * @return none.
   */
  void writeFile
  ( 
    const TCHAR* fileName, 
    BYTE*& pData, 
    size_t& ulOffset
  )
  {
    long bufferLen = 0;
    if( NULL == fileName )
    {
      // we have no file, so just write that the file size is 0.
      myodd::offset::write( bufferLen, pData, ulOffset );
    }
    else
    {
      MYODD_STRING expanded;
      if( !myodd::files::ExpandEnvironment( fileName, expanded ))
      {
        expanded = fileName;
      }

      FILE* hf;
#ifdef UNICODE
      if( 0 == _wfopen_s( &hf, expanded.c_str(), L"rb") )
#else
      if (0 == fopen_s(&hf, expanded.c_str(), "rb"))
#endif
      {
        // obtain file size.
        fseek (hf , 0 , SEEK_END);
        bufferLen = ftell (hf);
        rewind (hf);

        // the length of the message
        myodd::offset::write( bufferLen, pData, ulOffset );

        // read the file.
        BYTE* bRead = new BYTE[bufferLen];
        memset( bRead, 0, bufferLen );
        if( bRead )
        {
          size_t ulRead = _fread_nolock(bRead, sizeof(BYTE), bufferLen, hf);
          assert( ulRead == (size_t)bufferLen );
          myodd::offset::write( bRead, ulRead, pData, ulOffset );
          delete [] bRead;
        }

        // close the file.
        fclose( hf );
      }// open the file
      else
      {
        // we have no file, (we could not open it), so just write that the file size is 0.
        myodd::offset::write( bufferLen, pData, ulOffset );
      }
    }// if we have a file
  }

  void write
  ( 
    const void* item, 
    size_t unItemSize,
    BYTE*& pData, 
    size_t& ulOffset
  )
  {
    if( unItemSize == 0 )
    {
      // nothing to do here.
      return;
    }

    if( ulOffset > 0 && pData)
    {
      BYTE* pTmpData = new BYTE[ulOffset + unItemSize ];
      memcpy( pTmpData, pData, ulOffset );
      delete [] pData;
      pData = pTmpData;
    }
    else
    {
      pData = new BYTE[ unItemSize ];
    }

    memcpy( (BYTE*)pData+ulOffset, item, unItemSize );

    ulOffset += unItemSize;
  }

  template<>
  void write<TCHAR>( const TCHAR*& item, BYTE*& pData, size_t& ulOffset )
  {
    size_t uiLength = myodd::strings::Length( item ) * sizeof(TCHAR);
    size_t uiSize = uiLength;
    uiSize += sizeof(unsigned int); //  the length
    if( ulOffset > 0 )
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

    memcpy( pData+ulOffset, &uiLength, sizeof(unsigned int) );
    if( uiLength > 0 )
    {
      memcpy( pData+ulOffset+sizeof(unsigned int), item , uiLength );
    }
    ulOffset += uiSize;
  }

  template<>
  void write<MYODD_STRING>( const MYODD_STRING& item, BYTE*& pData, size_t& ulOffset )
  {
    const TCHAR* lp = item.c_str();
    write( lp, pData, ulOffset );
  }

  template<>
  bool read<TCHAR>
  ( 
    TCHAR*& item, 
    const void* pData, 
    size_t& ulOffset,
    size_t uiMaxSize
  )
  {
    if( !pData )
    {
      //  no buffer to read from
      return false;
    }

    //  can we read the size of the string?
    if( ulOffset+sizeof(unsigned int) > uiMaxSize )
      return false;

    unsigned int uiSize = 0;
    memcpy( &uiSize, (const BYTE*)pData+ulOffset, sizeof(unsigned int) );
    ulOffset += sizeof(unsigned int);

    if( ulOffset+uiSize > uiMaxSize )
      return false;

    if( uiSize > 0 )
    {
      item = new TCHAR[(uiSize+ sizeof(TCHAR))];
      memset( item, 0, uiSize+sizeof(TCHAR));
      memcpy( item, (const BYTE*)pData+ulOffset, uiSize );
      ulOffset += uiSize;
    }
    return true;
  }

  template<>
  bool read<MYODD_STRING>
  ( 
    MYODD_STRING& item, 
    const void* pData, 
    size_t& ulOffset,
    size_t uiMaxSize
  )
  {
    TCHAR* pText = NULL;
    if( !read( pText, pData, ulOffset, uiMaxSize ))
      return false;

    if( pText )
    {
      item = pText;
      delete [] pText;
    }
    else
    {
      item = _T("");
    }
    return true;
  }

  bool read
  ( 
    void* item, 
    size_t unItemSize,
    const void* pData, 
    size_t& ulOffset,
    size_t uiMaxSize
  )
  {
    if( ulOffset+unItemSize > uiMaxSize )
      return false;
    memcpy( item, (const BYTE*)pData+ulOffset, unItemSize );
    ulOffset += unItemSize;
    return true;
  }

  /**
   * Read a file content from given data
   * the first 4 bytes is the total size of the 
   * @param const TCHAR* where we are writing the file to.
   * @param const BYTE* the data we are reading from
   * @param unsigned long& the offest start/end position
   * @param unsigned int the maximum size of the buffer
   * @return none
   */
  void readFile
  ( 
    const TCHAR* fileName, 
    const BYTE* pData, 
    size_t& ulOffset,
    size_t uiMaxSize
  )
  {
    long bufferLen =  0;
    myodd::offset::read( bufferLen, pData, ulOffset, uiMaxSize );
    if( bufferLen == 0 )
    {
      // no data to read.
      return;
    }

    FILE* hf = NULL;
#ifdef UNICODE
      if (0 == _wfopen_s(&hf, fileName, L"wb"))
#else
      if (0 == fopen_s(&hf, fileName, "wb"))
#endif
    {
      return;
    }

    // recreate the file.
    // create a buffer that will contain the whole file.
    BYTE* pBuffer = new BYTE[bufferLen];

    // read the data
    myodd::offset::read( pBuffer, bufferLen, pData, ulOffset, uiMaxSize );

    // write the data to the file.
    size_t nWriten = fwrite( pBuffer, sizeof(BYTE), bufferLen, hf );
    assert( nWriten == (size_t)bufferLen );

    // close the file.
    fclose( hf );

    // we are done with the buffer.
    delete [] pBuffer;
  }
} //  offset
} //  myodd