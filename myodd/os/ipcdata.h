#pragma once

namespace myodd {
  namespace os {
    class IpcData
    {
    public:
      enum class IpcDataType : unsigned int
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
      virtual ~IpcData();

      // the number of arguments.
      unsigned int GetNumArguments() const;

      // the guid
      const std::wstring& GetGuid() const;

      // do we have a valid guid, (we should but still).
      bool HasGuid() const;

    private:
      // the number of arguments we were passed.
      unsigned int _numArguments;

      // the guid.
      std::wstring _guid;

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
    };
  }
}