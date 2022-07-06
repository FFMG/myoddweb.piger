#include "stdafx.h"

#include <assert.h>
#include "offset.h"
#include "../files/files.h"
#include "../string/string.h"

namespace myodd{ namespace offset{
  /**
   * Write a file, (the actual data, not the filename), to the given buffer.
   * @param const MYODD_CHAR* the file we want to write to the buffer.
   * @param unsigned char*& the data that will contain the file contents.
   * @param unsigned long& the current offset position at the start and end of the operation
   * @return none.
   */
  void writeFile
  ( 
    const MYODD_CHAR* fileName,
    unsigned char*& pData,
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
      std::wstring expanded;
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
        unsigned char* bRead = new unsigned char[bufferLen];
        memset( bRead, 0, bufferLen );
        if( bRead )
        {
          size_t ulRead = _fread_nolock(bRead, sizeof(unsigned char), bufferLen, hf);
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
    unsigned char*& pData,
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
      unsigned char* pTmpData = new unsigned char[ulOffset + unItemSize ];
      memcpy( pTmpData, pData, ulOffset );
      delete [] pData;
      pData = pTmpData;
    }
    else
    {
      pData = new unsigned char[ unItemSize ];
    }

    memcpy( (unsigned char*)pData+ulOffset, item, unItemSize );

    ulOffset += unItemSize;
  }

  template<>
  void write<MYODD_CHAR>( const MYODD_CHAR*& item, unsigned char*& pData, size_t& ulOffset )
  {
    size_t uiLength = myodd::strings::Length( item ) * sizeof(MYODD_CHAR);
    size_t uiSize = uiLength;
    uiSize += sizeof(unsigned int); //  the length
    if( ulOffset > 0 )
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

    memcpy( pData+ulOffset, &uiLength, sizeof(unsigned int) );
    if( uiLength > 0 )
    {
      memcpy( pData+ulOffset+sizeof(unsigned int), item , uiLength );
    }
    ulOffset += uiSize;
  }

  template<>
  void write<std::wstring>( const std::wstring& item, unsigned char*& pData, size_t& ulOffset )
  {
    const MYODD_CHAR* lp = item.c_str();
    write( lp, pData, ulOffset );
  }

  template<>
  bool read<MYODD_CHAR>
  ( 
    MYODD_CHAR*& item,
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
    memcpy( &uiSize, (const unsigned char*)pData+ulOffset, sizeof(unsigned int) );
    ulOffset += sizeof(unsigned int);

    if( ulOffset+uiSize > uiMaxSize )
      return false;

    if( uiSize > 0 )
    {
      item = new MYODD_CHAR[(uiSize+ sizeof(MYODD_CHAR))];
      memset( item, 0, uiSize+sizeof(MYODD_CHAR));
      memcpy( item, (const unsigned char*)pData+ulOffset, uiSize );
      ulOffset += uiSize;
    }
    return true;
  }

  template<>
  bool read<std::wstring>
  ( 
    std::wstring& item, 
    const void* pData, 
    size_t& ulOffset,
    size_t uiMaxSize
  )
  {
    MYODD_CHAR* pText = NULL;
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
    memcpy( item, (const unsigned char*)pData+ulOffset, unItemSize );
    ulOffset += unItemSize;
    return true;
  }

  /**
   * Read a file content from given data
   * the first 4 unsigned chars is the total size of the 
   * @param const MYODD_CHAR* where we are writing the file to.
   * @param const unsigned char* the data we are reading from
   * @param unsigned long& the offest start/end position
   * @param unsigned int the maximum size of the buffer
   * @return none
   */
  void readFile
  ( 
    const MYODD_CHAR* fileName,
    const unsigned char* pData,
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
      unsigned char* pBuffer = new unsigned char[bufferLen];

    // read the data
    myodd::offset::read( pBuffer, bufferLen, pData, ulOffset, uiMaxSize );

    // write the data to the file.
    size_t nWriten = fwrite( pBuffer, sizeof(unsigned char), bufferLen, hf );
    assert( nWriten == (size_t)bufferLen );

    // close the file.
    fclose( hf );

    // we are done with the buffer.
    delete [] pBuffer;
  }
} //  offset
} //  myodd