#pragma once

namespace myodd {
  namespace os {
    class IpcData
    {
    public:
      enum class IpcDataType : unsigned int
      {
        None = 0,
        Int32 = 1,
        String = 2,
        StringAscii = 3,
      };

    public:
      /**
       * The IPC data parser
       */
      IpcData( unsigned char* pData, unsigned int dataSize );
      virtual ~IpcData();

    private:
      unsigned int _numArguments;

      // the number of arguments.
      unsigned int GetNumArguments() const;

      //  read all the data
      void Read(unsigned char* pData, unsigned int dataSize);

      // read the version number only.
      static signed int ReadVersionNumber(unsigned char* pData, int& pointer );

      // read the version number only.
      static IpcDataType ReadDataType(unsigned char* pData, int& pointer);

      // read a number
      static signed int ReadInt32(unsigned char* pData, int& pointer);

      // read a unicode string
      static std::wstring ReadString(unsigned char* pData, int& pointer);

      // read a Ascii string
      static std::string ReadAsciiString(unsigned char* pData, int& pointer);
    };
  }
}