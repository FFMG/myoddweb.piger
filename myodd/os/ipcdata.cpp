#include "stdafx.h"
#include "ipcdata.h"
#include <afxwin.h>
#include <string>

namespace myodd {
namespace os {

IpcData::IpcData(unsigned char* pData, unsigned int dataSize) : 
  _numArguments( 0 )
{
  //  read the data
  Read(pData, dataSize);
}

IpcData::~IpcData()
{
}

/**
 * Get the number of arguments.
 * @return unsigned int the number of arguments we have.
 */
unsigned int IpcData::GetNumArguments() const
{
  return _numArguments;
}

void IpcData::Read(unsigned char* pData, unsigned int dataSize)
{
  //  the message must be, at the very least the size of the version number.
  // https://www.displayfusion.com/Discussions/View/converting-c-data-types-to-c/?ID=38db6001-45e5-41a3-ab39-8004450204b3
  if (dataSize < sizeof(signed int))
  {
    throw "invalid data size, we need at least the version number.";
  }

  // reset the number of arguments
  _numArguments = 0;

  // the pointer to keep track of where we are.
  size_t pointer = 0;

  //  get the version number
  auto versionNumber = ReadVersionNumber( pData, pointer );

  //  now read all the data
  for (; pointer < dataSize;)
  {
    // get the next item type.
    auto dataType = ReadDataType( pData, pointer );

    switch ( dataType)
    {
    case IpcDataType::Int32:
    {
      auto dataValue = ReadInt32( pData, pointer );
    }
    break;

    case IpcDataType::String://string unicode
    {
      auto sDataValue = ReadString(pData, pointer);
    }
    break;

    case IpcDataType::StringAscii:
    {
      auto sDataValue = ReadAsciiString(pData, pointer);
    }
    break;

    default:
      // no point goint further as we do no know the size of that argument.
      // even if we can move the pointer forward, it would be more luck than anything.
      throw "Unknown argument type, I am unable to read the size/data for it.";
    }

    //  we found a valid argument.
    ++_numArguments;
  }

  // if the data is not exactly correct, then we have a problem.
  if( pointer != dataSize )
  {
    throw "There was a problem reading the given data, the counters do not match exactly as expected.";
  }
}

/**
 * Read the next data type
 * We update the pointer location.
 * @param unsigned char* pData the data container
 * @param size_t& pointer the current pointer location.
 * @return unsigned short int the next data type.
 */
IpcData::IpcDataType IpcData::ReadDataType(unsigned char* pData, size_t& pointer)
{
  // default value, we need to make sure that value is 'signed int' rahter than 'auto'
  unsigned short int dataType = 0;
  try
  {
    // read the value.
    memcpy_s(&dataType, sizeof(dataType), pData + pointer, sizeof(dataType));

    // move the pointer forward.
    pointer += sizeof(dataType);
  }
  catch (...)
  {
    throw "There was a problem reading the version number.";
  }

  // return what we found.
  return static_cast<IpcDataType>(dataType);
}

/**
 * Read a Unicode string.
 * We update the pointer location.
 * @param unsigned char* pData the data container
 * @param size_t& pointer the current pointer location.
 * @return std::wstring the string that we read.
 */
std::wstring IpcData::ReadString(unsigned char* pData, size_t& pointer)
{
  // our return value
  std::wstring sDataValue = L"";

  try
  {
    //  first we get the size, we could be passed a zero size string.
    signed int dataSize = 0;
    memcpy_s(&dataSize, sizeof(dataSize), pData + pointer, sizeof(dataSize));

    // update the pointer location.
    pointer += sizeof(dataSize);

    // if we have anything to read, read it.
    if (0 != dataSize)
    {
      // create a wchar container.
      auto dataValue = new wchar_t[dataSize + 1];

      // reset the data, be sure to add a terminating \0.
      memset(dataValue, L'\0', (dataSize+1) * sizeof(wchar_t));

      // get the data
      auto dataSizeToRead = static_cast<size_t>(dataSize) * sizeof(wchar_t);
      memcpy_s(dataValue, dataSizeToRead, pData + pointer, dataSizeToRead);

      // update the pointer location
      pointer += dataSizeToRead;

      //  copy it to our return string.
      sDataValue = dataValue;

      // clean the temp pointer.
      delete[] dataValue;
    }
  }
  catch (...)
  {
    throw "There was a problem reading the unicode string.";
  }

  // return what we found.
  return sDataValue;
}

/**
 * Read an ascii string.
 * We update the pointer location.
 * @param unsigned char* pData the data container
 * @param size_t& pointer the current pointer location.
 * @return std::string the string that we read.
 */
std::string IpcData::ReadAsciiString(unsigned char* pData, size_t& pointer)
{
  // our return value
  std::string sDataValue = "";

  try
  {
    //  first we get the size, we could be passed a zero size string.
    signed int dataSize = 0;
    memcpy_s(&dataSize, sizeof(dataSize), pData + pointer, sizeof(dataSize));

    // update the pointer location.
    pointer += sizeof(dataSize);

    // if we have anything to read, read it.
    if (0 != dataSize)
    {
      // create a wchar container.
      auto dataValue = new char[dataSize + 1];

      // reset the data, be sure to add a terminating \0.
      memset(dataValue, '\0', dataSize + 1);

      // get the data
      memcpy_s(dataValue, dataSize, pData + pointer, dataSize);

      // update the pointer location
      pointer += dataSize;

      //  copy it to our return string.
      sDataValue = dataValue;

      // clean the temp pointer.
      delete[] dataValue;
    }
  }
  catch (...)
  {
    throw "There was a problem reading the acii string.";
  }

  // return what we found.
  return sDataValue;
}

/**
 * Read the number at our current location.
 * We update the pointer location.
 * @param unsigned char* pData the data container
 * @param size_t& pointer the current pointer location.
 * @return signed int the number.
 */
signed int IpcData::ReadInt32(unsigned char* pData, size_t& pointer)
{
  signed int dataValue = 0;
  try
  {
    // read the value.
    memcpy_s(&dataValue, sizeof(dataValue), pData + pointer, sizeof(dataValue));

    // move the pointer forward.
    pointer += sizeof(dataValue);
  }
  catch (...)
  {
    throw "There was a problem reading the number.";
  }

  // return what we found.
  return dataValue;
}

/**
 * Read the version number at our current location.
 * We update the pointer location.
 * @param unsigned char* pData the data container
 * @param size_t& pointer the current pointer location.
 * @return signed int the version number.
 */
signed int IpcData::ReadVersionNumber(unsigned char* pData, size_t& pointer)
{
  // default value, we need to make sure that value is 'signed int' rahter than 'auto'
  signed int versionNumber = 0;
  try
  {
    // read the value.
    memcpy_s(&versionNumber, sizeof(versionNumber), pData + pointer, sizeof(versionNumber));

    // move the pointer forward.
    pointer += sizeof(versionNumber);
  }
  catch( ... )
  {
    throw "There was a problem reading the version number.";
  }

  // return what we found.
  return versionNumber;
}

}
}
