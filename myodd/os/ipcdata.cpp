#include "stdafx.h"
#include "ipcdata.h"
#include <afxwin.h>
#include <string>

namespace myodd {
namespace os {

static const signed int VersionNumber = 100;

IpcData::IpcData(unsigned char* pData, unsigned int dataSize) : 
  _numArguments( 0 ),
  _guid( L"" ),
  _pData( nullptr )
{
  //  read the data
  Read(pData, dataSize);
}

IpcData::IpcData(const std::wstring& guid) :
  _numArguments(0),
  _guid( guid ),
  _pData(nullptr)
{
}

IpcData::~IpcData()
{
  // reset the data.
  ResetArguments();

  // reset the pointer.
  ResetPtr();
}

/**
 * Reset the arguments and delete all the values.
 */
void IpcData::ResetArguments()
{
  for(IpcArguments::const_iterator it = _ipcArguments.begin();
      it != _ipcArguments.end();
      ++it)
  {
    auto ia = (*it);
    
    // delete the data.
    delete ia->pData;

    // delete the data
    delete ia;
  }

  // reset all the data.
  _ipcArguments.clear();
}

/**
 * Reset the data pointer.
 */
void IpcData::ResetPtr()
{
  if( _pData )
  {
    delete[] _pData;
    _pData = nullptr;
  }
}

/**
 * Get the number of arguments.
 * @return unsigned int the number of arguments we have.
 */
unsigned int IpcData::GetNumArguments() const
{
  return _numArguments;
}

/**
 * Get the given guid.
 * @return const std::wstring& the guid, if we have one.
 */
const std::wstring& IpcData::GetGuid() const
{
  return _guid;
}

/**
 * Return if we have a guid or not.
 */
bool IpcData::HasGuid() const
{
  return (GetGuid().length() > 0);
}

/**
 * Calculate the total size given all the arguments.
 * @return size_t the total arguments size.
 */
size_t IpcData::CalculateArgumentsSize() const
{
  // at least the size of the version number.
  auto totalSize = sizeof(VersionNumber);

  // now add all the type sizes.
  for (auto it = _ipcArguments.begin();
            it != _ipcArguments.end();
            ++it)
  {
    auto ia = (*it);
    switch( ia->dataType )
    {
    case IpcDataType::Int32:
      totalSize += sizeof(signed int);
      break;

    case IpcDataType::String:
    case IpcDataType::Guid:
      totalSize += static_cast<std::wstring*>(ia->pData)->length() * sizeof(wchar_t*);
      break;

    case IpcDataType::StringAscii:
      totalSize += static_cast<std::string*>(ia->pData)->length() * sizeof( char*);
      break;

    case IpcDataType::None:
    default:
      throw "Unnown data type.";
    }
  }
  return totalSize;
}

unsigned char* IpcData::GetPtr()
{
  if (_pData)
  {
    return _pData;
  }

  auto totalSize = CalculateArgumentsSize();
  _pData = new unsigned char[totalSize];
  memset(_pData, 0, totalSize);

  size_t pointer = 0;

  //  add the version number.
  memcpy(static_cast<PVOID>(_pData + pointer), &VersionNumber, sizeof(VersionNumber));
  pointer += sizeof(VersionNumber);

  for (IpcArguments::const_iterator it = _ipcArguments.begin();
    it != _ipcArguments.end();
    ++it)
  {
    auto ia = (*it);
    switch (ia->dataType)
    {
    case IpcDataType::Int32:
    {
      signed int* dataValue = static_cast<signed int*>(ia->pData);
      unsigned short int dataType = static_cast<unsigned short int>(IpcDataType::Int32);  //  Int32

      // add the data type.
      memcpy(static_cast<PVOID>(_pData + pointer), &dataType, sizeof(dataType));
      pointer += sizeof(dataType);

      // add the data
      memcpy(static_cast<PVOID>(_pData + pointer), dataValue, sizeof(*dataValue));
      pointer += sizeof(*dataValue);
    }
    break;

    case IpcDataType::String:
    case IpcDataType::Guid:
      break;

    case IpcDataType::StringAscii:
      break;

    case IpcDataType::None:
    default:
      throw "Unnown data type.";
    }
  }

  // we can now get the pointer.
  return GetPtr();
}

/**
 * Read data passed to us.
 * @throw if one or more items are invalid.
 * @param unsigned char* pData the data we want to read
 * @param unsigned int dataSize the size of the data we are reading.
 * @return none.
 */
void IpcData::Read(unsigned char* pData, unsigned int dataSize)
{
  //  the message must be, at the very least the size of the version number.
  // https://www.displayfusion.com/Discussions/View/converting-c-data-types-to-c/?ID=38db6001-45e5-41a3-ab39-8004450204b3
  if (dataSize < sizeof(signed int))
  {
    throw "invalid data size, we need at least the version number.";
  }

  // reset the arguments
  ResetArguments();

  // reset the pointer
  ResetPtr();

  // reset the number of arguments
  _numArguments = 0;

  // reset the guid
  _guid = L"";

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
    case IpcDataType::Guid:
    {
      // set the guid.
      _guid = ReadGuid(pData, pointer);
      _ipcArguments.push_back(new IpcArgument{
        new std::wstring(_guid),
        IpcDataType::Guid
      });
    }
    break;
    
    case IpcDataType::Int32:
    {
      _ipcArguments.push_back(new IpcArgument{
        new (signed int)(ReadInt32(pData, pointer)),
        IpcDataType::Int32
      });
    }
    break;

    case IpcDataType::String://string unicode
    {
      _ipcArguments.push_back(new IpcArgument{
        new std::wstring(ReadString(pData, pointer)),
        IpcDataType::String
      });
    }
    break;

    case IpcDataType::StringAscii:
    {
      _ipcArguments.push_back(new IpcArgument{
        new std::string(ReadAsciiString(pData, pointer)),
        IpcDataType::StringAscii
      });
    }
    break;

    default:
      // no point goint further as we do no know the size of that argument.
      // even if we can move the pointer forward, it would be more luck than anything.
      throw "Unknown argument type, I am unable to read the size/data for it.";
    }

    switch (dataType)
    {
    case IpcDataType::Guid:
      //  the guid does not count as an argument.
      break;

    default:
      //  we found a valid argument.
      ++_numArguments;
      break;
    }
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
      //
      // NB: The 'size' is the number of wide characters, not the number of bytes.
      //     This is why we must be carefull when passing data from one function to the other.
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
 * Read a Unicode guid.
 * We update the pointer location.
 * @param unsigned char* pData the data container
 * @param size_t& pointer the current pointer location.
 * @return std::wstring the string that we read.
 */
std::wstring IpcData::ReadGuid(unsigned char* pData, size_t& pointer)
{
  // we just use the string class.
  return ReadString( pData, pointer );
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

void IpcData::Add(signed int dataValue )
{
  _ipcArguments.push_back(new IpcArgument{
    new (signed int)(dataValue),
    IpcDataType::Int32
  });

  // update the argument count
  ++_numArguments;

  // reset the pointer if need be.
  ResetPtr();
}
}
}
