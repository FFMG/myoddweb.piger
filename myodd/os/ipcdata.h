#pragma once
#include <vector>

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
        String = 3,
        StringAscii = 4,
      };

    public:
      /**
       * The IPC data parser
       */
      IpcData( unsigned char* pData, unsigned int dataSize );
      IpcData( const std::wstring& guid );
      virtual ~IpcData();

      // the number of arguments.
      unsigned int GetNumArguments() const;

      // the guid
      const std::wstring& GetGuid() const;

      // do we have a valid guid, (we should but still).
      bool HasGuid() const;

      // get the pointer.
      unsigned char* GetPtr();

      // get the pointer size.
      size_t GetSize();

      // add a signed int.
      void Add(signed int dataValue);

      // add a wide string.
      void Add( const std::wstring& dataValue);

    private:
      // the number of arguments we were passed.
      unsigned int _numArguments;

      // the guid.
      std::wstring _guid;

      // the current data
      unsigned char* _pData;

      // the current data size.
      size_t _pDataSize;

      //  read all the data
      void Read(unsigned char* pData, unsigned int dataSize);

      // read the version number only.
      static signed int ReadVersionNumber(unsigned char* pData, size_t& pointer );

      // read the version number only.
      static IpcDataType ReadDataType(unsigned char* pData, size_t& pointer);

      // read a number
      static signed int ReadInt32(unsigned char* pData, size_t& pointer);

      // read a unicode string
      static std::wstring ReadString(unsigned char* pData, size_t& pointer);

      // read a Ascii string
      static std::string ReadAsciiString(unsigned char* pData, size_t& pointer);

      // read the guid
      static std::wstring ReadGuid(unsigned char* pData, size_t& pointer);

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