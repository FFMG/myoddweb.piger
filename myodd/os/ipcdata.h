// ***********************************************************************
// Copyright (c) 2016-2022 Florent Guelfucci
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// @see https://opensource.org/licenses/MIT
// ***********************************************************************
#pragma once
#include <string>
#include <vector>
#include <stdint.h>

namespace myodd {
  namespace os {
    class IpcData
    {
    public:
      enum class IpcDataType : unsigned short int
      {
        None = 0,
        Guid = 1,
        Int32 = 2,
        Int64 = 3,
        String = 4,
        StringAscii = 5,
      };

    public:
      /**
       * The IPC data parser
       */
      IpcData( unsigned char* pData, unsigned int dataSize );
      IpcData(const IpcData& ipcData);
      explicit IpcData( const std::wstring& guid );
      virtual ~IpcData();

      const IpcData& operator=(const IpcData& rhs );

      // the number of arguments.
      unsigned int GetNumArguments() const;

      // the guid
      const std::wstring& GetGuid() const;

      // do we have a valid guid, (we should but still).
      bool HasGuid() const;

      // remove an argument by index.
      void RemoveArgument(unsigned int index);

      // get the pointer.
      unsigned char* GetPtr();

      // get the pointer size.
      size_t GetSize();

      // add a signed int.
      void Add(int32_t dataValue);

      // add a signed long long.
      void Add(int64_t dataValue);

      // add a wide string.
      void Add( const std::wstring& dataValue);

      // add an ascii string.
      void Add(const std::string& dataValue);

      template<typename T>
      T Get(unsigned int index) const;

      // is it a string?
      bool IsString(unsigned int index) const;

      // is it an int?
      bool IsInt(unsigned int index) const;

      bool operator==(const IpcData& rhs) const;
      bool operator!=(const IpcData& rhs) const;

    private:
      // the number of arguments we were passed.
      unsigned int _numArguments;

      // shift an index number by the guid
      unsigned int AdjustIndexNumber(unsigned int givenIndex) const;

      // the guid.
      std::wstring _guid;

      // the current data
      unsigned char* _pData;

      // the current data size.
      size_t _pDataSize;

      //  read all the data
      void Read(unsigned char* pData, unsigned int dataSize);

      // read the version number only.
      static signed int ReadVersionNumber(unsigned char* pData, const size_t dataSize, size_t& pointer );

      // read the version number only.
      static IpcDataType ReadDataType(unsigned char* pData, const size_t dataSize, size_t& pointer);

      // read a number
      static int32_t ReadInt32(unsigned char* pData, const size_t dataSize, size_t& pointer);

      // read a number
      static int64_t ReadInt64(unsigned char* pData, const size_t dataSize, size_t& pointer);

      // read a unicode string
      static std::wstring ReadString(unsigned char* pData, const size_t dataSize, size_t& pointer);

      // read a Ascii string
      static std::string ReadAsciiString(unsigned char* pData, const size_t dataSize, size_t& pointer);

      // read the guid
      static std::wstring ReadGuid(unsigned char* pData, const size_t dataSize, size_t& pointer);

      // make sure we can read the data.
      static void ThrowIfReadingPastSize(const size_t dataSize, const size_t pointer, const size_t needed);
    private:
      // all the arguments.
      struct IpcArgument
      {
        void* pData;
        IpcDataType dataType;
      };

      typedef std::vector<IpcArgument*> IpcArguments;
      IpcArguments _ipcArguments;

      // reset all the data
      void ResetArguments();

      // given all the arguments caclulate the size
      // of the pointer we will need.
      size_t CalculateArgumentsSize() const;

      // reset the point
      void ResetPtr();
    };
  }
}