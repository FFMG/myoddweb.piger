#include "stdafx.h"
#include "ipcdata.h"
#include <afxwin.h>
#include <../string/string.h>
#include <inttypes.h>

namespace myodd {
namespace os {

static const signed int VersionNumber = 100;

IpcData::IpcData(unsigned char* pData, unsigned int dataSize) : 
  _numArguments( 0 ),
  _guid( L"" ),
  _pData( nullptr ),
  _pDataSize( 0 )
{
  //  read the data
  Read(pData, dataSize);
}

IpcData::IpcData(const IpcData& ipcData) :
  _numArguments(0),
  _guid(L""),
  _pData(nullptr),
  _pDataSize(0)
{
  *this = ipcData;
}

IpcData::IpcData(const std::wstring& guid) :
  _numArguments(0),
  _guid( guid ),
  _pData(nullptr),
  _pDataSize(0)
{
  //  add it to our list.
  _ipcArguments.push_back(new IpcArgument{
    new std::wstring(_guid),
    IpcDataType::Guid
  });
}

IpcData::~IpcData()
{
  // reset the data.
  ResetArguments();

  // reset the pointer.
  ResetPtr();
}

const IpcData& IpcData::operator=(const IpcData& rhs)
{
  if( this != &rhs )
  {
    //  clear everything
    ResetPtr();
    ResetArguments();

    //  copy some of the data
    _numArguments = rhs._numArguments;
    _guid = rhs._guid;

    for (auto it = rhs._ipcArguments.begin();
      it != rhs._ipcArguments.end();
      ++it)
    {
      // the pointer
      auto ia = (*it);
      switch ( ia->dataType )
      {
      case IpcDataType::Guid:
      {
        // set the guid.
        _ipcArguments.push_back(new IpcArgument{
          new std::wstring(_guid),
          IpcDataType::Guid
        });
      }
      break;

      case IpcDataType::Int32:
      {
        _ipcArguments.push_back(new IpcArgument{
          new (int32_t)( *(static_cast<int32_t*>(ia->pData))),
          IpcDataType::Int32
        });
      }

      case IpcDataType::Int64:
      {
        _ipcArguments.push_back(new IpcArgument{
          new (int64_t)(*(static_cast<int64_t*>(ia->pData))),
          IpcDataType::Int64
        });
      }
      break;

      case IpcDataType::String://string unicode
      {
        _ipcArguments.push_back(new IpcArgument{
          new std::wstring( *(static_cast<std::wstring*>(ia->pData))),
          IpcDataType::String
        });
      }
      break;

      case IpcDataType::StringAscii:
      {
        _ipcArguments.push_back(new IpcArgument{
          new std::string( *(static_cast<std::string*>(ia->pData))),
          IpcDataType::StringAscii
        });
      }
      break;

      default:
        // we cannot copy this item
        throw std::exception( "Unknown argument type, I am unable to copy the original content.");
      }

    }
      
    //  we don't need the pointer and sizes as those are created on the fly.
  }
  return *this;
}

/**
 * Reset the arguments and delete all the values.
 */
void IpcData::ResetArguments()
{
  for(auto it = _ipcArguments.begin();
      it != _ipcArguments.end();
      ++it)
  {
    // the pointer
    auto ia = (*it);

    switch (ia->dataType)
    {
    case IpcDataType::Int32:
      // delete the data.
      delete static_cast<int32_t*>(ia->pData);
      break;

    case IpcDataType::Int64:
      // delete the data.
      delete static_cast<int64_t*>(ia->pData);
      break;

    case IpcDataType::String:
    case IpcDataType::Guid:
      // delete the data.
      delete static_cast<std::wstring*>(ia->pData);
      break;

    case IpcDataType::StringAscii:
      // delete the data.
      delete static_cast<std::string*>(ia->pData);
      break;

    case IpcDataType::None:
    default:
      throw std::exception( "Unnown data type." );
    }

    // delete the data
    delete ia;
  }

  // reset all the data.
  _ipcArguments.clear();
  _numArguments = 0;
}

/**
 * Reset the data pointer.
 */
void IpcData::ResetPtr()
{
  if( nullptr != _pData )
  {
    // clean the array.
    delete[] _pData;
    _pData = nullptr;
  }

  // reset the data size as well.
  _pDataSize = 0;
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
    // the pointer
    auto ia = (*it);

    // the data type, always added.
    totalSize += sizeof(ia->dataType);
    switch( ia->dataType )
    {
    case IpcDataType::Int32:
      totalSize += sizeof(int32_t);
      break;

    case IpcDataType::Int64:
      totalSize += sizeof(int64_t);
      break;

    case IpcDataType::String:
    case IpcDataType::Guid:
      totalSize += sizeof(signed int);  //  the number of characters
      totalSize += static_cast<std::wstring*>(ia->pData)->length() * sizeof(wchar_t);  //  the data itself
      break;

    case IpcDataType::StringAscii:
      totalSize += sizeof(signed int);  //  the number of characters
      totalSize += static_cast<std::string*>(ia->pData)->length() * sizeof(char); //  the data itself
      break;

    case IpcDataType::None:
    default:
      throw std::exception( "Unnown data type." );
    }
  }
  return totalSize;
}

size_t IpcData::GetSize()
{
  //  make sure that the pointer is valid
  // so call the getptr value to get the data size.
  GetPtr();

  // we should now have a valid size.
  return _pDataSize;
}

unsigned char* IpcData::GetPtr()
{
  if ( nullptr != _pData)
  {
    return _pData;
  }

  // re-calculate the size,
  _pDataSize = CalculateArgumentsSize();

  // the size cannot be zero
  if (_pDataSize == 0)
  {
    throw std::exception( "The size cannot be zero, we must have at least the version number!" );
  }

  // create the data 
  _pData = new unsigned char[_pDataSize];
  memset(_pData, 0, _pDataSize);

  // the current pointer.
  size_t pointer = 0;

  //  add the version number.
  memcpy(static_cast<PVOID>(_pData + pointer), &VersionNumber, sizeof(VersionNumber));
  pointer += sizeof(VersionNumber);

  for (IpcArguments::const_iterator it = _ipcArguments.begin();
    it != _ipcArguments.end();
    ++it)
  {
    auto ia = (*it);

    auto dataType = static_cast<unsigned short int>(ia->dataType);
    memcpy(static_cast<PVOID>(_pData + pointer), &dataType, sizeof(dataType));
    pointer += sizeof(dataType);

    switch (ia->dataType)
    {
    case IpcDataType::Int32:
    {
      // get the data.
      auto dataValue = static_cast<int32_t*>(ia->pData);

      // add it to the pointer.
      memcpy(static_cast<PVOID>(_pData + pointer), dataValue, sizeof(*dataValue));
      pointer += sizeof(*dataValue);
    }
    break;

    case IpcDataType::Int64:
    {
      // get the data.
      auto dataValue = static_cast<int64_t*>(ia->pData);

      // add it to the pointer.
      memcpy(static_cast<PVOID>(_pData + pointer), dataValue, sizeof(*dataValue));
      pointer += sizeof(*dataValue);
    }
    break;

    case IpcDataType::String:
    case IpcDataType::Guid:
    {
      // get the data.
      auto dataValue = static_cast<std::wstring*>(ia->pData);

      //  first we need to add the size.
      signed int dataSize = static_cast<signed int>(dataValue->length());
      memcpy(static_cast<PVOID>(_pData + pointer), &dataSize, sizeof(dataSize));
      pointer += sizeof(dataSize);

      //  now add the dataitself.
      //  now add the dataitself.
      if (dataSize > 0)
      {
        memcpy(static_cast<PVOID>(_pData + pointer), dataValue->c_str(), dataSize * sizeof(wchar_t));
        pointer += dataSize * sizeof(wchar_t);
      }
    }
    break;

    case IpcDataType::StringAscii:
    {
      // get the data.
      auto dataValue = static_cast<std::string*>(ia->pData);

      //  first we need to add the size.
      signed int dataSize = static_cast<signed int>(dataValue->length());
      memcpy(static_cast<PVOID>(_pData + pointer), &dataSize, sizeof(dataSize));
      pointer += sizeof(dataSize);

      //  now add the dataitself.
      if (dataSize > 0)
      {
        memcpy(static_cast<PVOID>(_pData + pointer), dataValue->c_str(), dataSize * sizeof(char));
        pointer += dataSize * sizeof(char);
      }
    }
    break;

    case IpcDataType::None:
    default:
      throw std::exception( "Unnown data type." );
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
    throw std::exception( "invalid data size, we need at least the version number." );
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
  if (versionNumber > VersionNumber)
  {
    throw std::exception("Trying to read data with a version number past our version number.");
  }

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
        new (int32_t)(ReadInt32(pData, pointer)),
        IpcDataType::Int32
      });
    }
    break;

    case IpcDataType::Int64:
    {
      _ipcArguments.push_back(new IpcArgument{
        new (int64_t)(ReadInt64(pData, pointer)),
        IpcDataType::Int64
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
      throw std::exception( "Unknown argument type, I am unable to read the size/data for it.");
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
    throw std::exception( "There was a problem reading the given data, the counters do not match exactly as expected.");
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
    throw std::exception( "There was a problem reading the version number.");
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
    throw std::exception( "There was a problem reading the unicode string." );
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
    throw std::exception( "There was a problem reading the acii string.");
  }

  // return what we found.
  return sDataValue;
}

/**
 * Read the number at our current location.
 * We update the pointer location.
 * @param unsigned char* pData the data container
 * @param size_t& pointer the current pointer location.
 * @return int32_t the number.
 */
int32_t IpcData::ReadInt32(unsigned char* pData, size_t& pointer)
{
  int32_t dataValue = 0;
  try
  {
    // read the value.
    memcpy_s(&dataValue, sizeof(dataValue), pData + pointer, sizeof(dataValue));

    // move the pointer forward.
    pointer += sizeof(dataValue);
  }
  catch (...)
  {
    throw std::exception( "There was a problem reading the number.");
  }

  // return what we found.
  return dataValue;
}

/**
 * Read the number at our current location.
 * We update the pointer location.
 * @param unsigned char* pData the data container
 * @param size_t& pointer the current pointer location.
 * @return int32_t the number.
 */
int64_t IpcData::ReadInt64(unsigned char* pData, size_t& pointer)
{
  int64_t dataValue = 0;
  try
  {
    // read the value.
    memcpy_s(&dataValue, sizeof(dataValue), pData + pointer, sizeof(dataValue));

    // move the pointer forward.
    pointer += sizeof(dataValue);
  }
  catch (...)
  {
    throw std::exception( "There was a problem reading the number.");
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
    throw std::exception( "There was a problem reading the version number.");
  }

  // return what we found.
  return versionNumber;
}

/**
 * Add a signed integer to our list of arguments.
 * @param signed int dataValue the string we want to add.
 */
void IpcData::Add(int32_t dataValue )
{
  _ipcArguments.push_back(new IpcArgument{
    new (int32_t)(dataValue),
    IpcDataType::Int32
  });

  // update the argument count
  ++_numArguments;

  // reset the pointer if need be.
  ResetPtr();
}

/**
* Add a signed integer to our list of arguments.
* @param signed int dataValue the string we want to add.
*/
void IpcData::Add(int64_t dataValue)
{
  _ipcArguments.push_back(new IpcArgument{
    new (int64_t)(dataValue),
    IpcDataType::Int64
  });

  // update the argument count
  ++_numArguments;

  // reset the pointer if need be.
  ResetPtr();
}

/**
 * Add a wide string to our list of arguments.
 * @param const std::wstring& dataValue the string we want to add.
 */
void IpcData::Add(const std::wstring& dataValue)
{
  _ipcArguments.push_back(new IpcArgument{
    new std::wstring(dataValue),
    IpcDataType::String
  });

  // update the argument count
  ++_numArguments;

  // reset the pointer if need be.
  ResetPtr();
}

/**
 * Add an asccii string to our list of arguments.
 * @param const std::string& dataValue the string we want to add.
 */
void IpcData::Add(const std::string& dataValue)
{
  _ipcArguments.push_back(new IpcArgument{
    new std::string(dataValue),
    IpcDataType::StringAscii
  });

  // update the argument count
  ++_numArguments;

  // reset the pointer if need be.
  ResetPtr();
}

/**
 * Check if the argument at the index is a string or a number
 * @param unsigned int index we are checking
 * @return bool if the index is a string or not.
 */
bool IpcData::IsString(unsigned int index) const
{
  // adjust the index, we will check for out of range values.
  index = AdjustIndexNumber(index);

  auto ipc = _ipcArguments[index];
  switch (ipc->dataType)
  {
  case IpcDataType::Int32:
  case IpcDataType::Int64:
    return false;

  case IpcDataType::String:
  case IpcDataType::Guid:
  case IpcDataType::StringAscii:
    return true;

  case IpcDataType::None:
  default:
    throw std::exception("Unnown data type.");
  }

  // never reached
  return false;
}

/**
 * Check if the argument at the index is a number or a string
 * @param unsigned int index we are checking
 * @return bool if the index is a number or not.
 */
bool IpcData::IsInt(unsigned int index) const
{
  // adjust the index, we will check for out of range values.
  index = AdjustIndexNumber(index);

  auto ipc = _ipcArguments[index];
  switch (ipc->dataType)
  {
  case IpcDataType::Int32:
  case IpcDataType::Int64:
    return true;

  case IpcDataType::String:
  case IpcDataType::Guid:
    return myodd::strings::IsNumeric( *(static_cast<std::wstring*>(ipc->pData)) );

  case IpcDataType::StringAscii:
    return myodd::strings::IsNumeric( myodd::strings::String2WString(*(static_cast<std::string*>(ipc->pData))) );

  case IpcDataType::None:
  default:
    throw std::exception( "Unnown data type." );
  }

  // never reached
  return false;
}

template<>
std::wstring IpcData::Get<std::wstring>(unsigned int index) const
{
  // adjust the index, we will check for out of range values.
  index = AdjustIndexNumber(index);

  auto ipc = _ipcArguments[index];
  switch (ipc->dataType)
  {
  case IpcDataType::Int32:
    return myodd::strings::Format(_T("%d"), *static_cast<int32_t*>(ipc->pData));

  case IpcDataType::Int64:
    return myodd::strings::Format(_T("%" PRId64), *static_cast<int64_t*>(ipc->pData));

  case IpcDataType::String:
  case IpcDataType::Guid:
    return *(static_cast<std::wstring*>(ipc->pData));

  case IpcDataType::StringAscii:
    return myodd::strings::String2WString(*(static_cast<std::string*>(ipc->pData)));

  case IpcDataType::None:
  default:
    throw std::exception( "Unnown data type." );
  }

  // never reached
  return L"";
}

template<>
std::string IpcData::Get<std::string>(unsigned int index) const
{
  auto ws2Convert = this->Get<std::wstring>(index);
  return strings::WString2String( ws2Convert );
}

template<>
uint32_t IpcData::Get<uint32_t>(unsigned int index) const
{
  // adjust the index, we will check for out of range values.
  index = AdjustIndexNumber(index);

  auto ipc = _ipcArguments[index];
  switch (ipc->dataType)
  {
  case IpcDataType::Int32:
    return (uint32_t)(*(static_cast<int32_t*>(ipc->pData)));

  case IpcDataType::Int64:
    return (uint32_t)(*(static_cast<int64_t*>(ipc->pData)));

  case IpcDataType::String:
  case IpcDataType::Guid:
    return (uint32_t)std::stoll(*(static_cast<std::wstring*>(ipc->pData)));

  case IpcDataType::StringAscii:
    return (uint32_t)std::stoll(myodd::strings::String2WString(*(static_cast<std::string*>(ipc->pData))));

  case IpcDataType::None:
  default:
    throw std::exception( "Unnown data type." );
  }

  // never reached
  return 0;
}

template<>
int32_t IpcData::Get<int32_t>(unsigned int index) const
{
  // adjust the index, we will check for out of range values.
  index = AdjustIndexNumber(index);

  auto ipc = _ipcArguments[index];
  switch (ipc->dataType)
  {
  case IpcDataType::Int32:
    return (int32_t)(*(static_cast<int32_t*>(ipc->pData)));

  case IpcDataType::Int64:
    return (int32_t)(*(static_cast<int64_t*>(ipc->pData)));

  case IpcDataType::String:
  case IpcDataType::Guid:
    return (int32_t)std::stoll(*(static_cast<std::wstring*>(ipc->pData)));

  case IpcDataType::StringAscii:
    return (int32_t)std::stoll(myodd::strings::String2WString(*(static_cast<std::string*>(ipc->pData))));

  case IpcDataType::None:
  default:
    throw std::exception( "Unnown data type." );
  }

  // never reached
  return 0;
}

template<>
uint64_t IpcData::Get<uint64_t>(unsigned int index) const
{
  // adjust the index, we will check for out of range values.
  index = AdjustIndexNumber(index);

  auto ipc = _ipcArguments[index];
  switch (ipc->dataType)
  {
  case IpcDataType::Int32:
    return (uint64_t)(*(static_cast<int32_t*>(ipc->pData)));

  case IpcDataType::Int64:
    return (uint64_t)(*(static_cast<int64_t*>(ipc->pData)));

  case IpcDataType::String:
  case IpcDataType::Guid:
    return (uint64_t)std::stoll(*(static_cast<std::wstring*>(ipc->pData)));

  case IpcDataType::StringAscii:
    return (uint64_t)std::stoll(myodd::strings::String2WString(*(static_cast<std::string*>(ipc->pData))));

  case IpcDataType::None:
  default:
    throw std::exception( "Unnown data type." );
  }

  // never reached
  return 0;
}

template<>
int64_t IpcData::Get<int64_t>(unsigned int index) const
{
  // adjust the index, we will check for out of range values.
  index = AdjustIndexNumber(index);

  auto ipc = _ipcArguments[index];
  switch (ipc->dataType)
  {
  case IpcDataType::Int32:
    return (int64_t)(*(static_cast<int32_t*>(ipc->pData)));

  case IpcDataType::Int64:
    return (int64_t)(*(static_cast<int64_t*>(ipc->pData)));

  case IpcDataType::String:
  case IpcDataType::Guid:
    return (int64_t)std::stoll( *(static_cast<std::wstring*>(ipc->pData)));

  case IpcDataType::StringAscii:
    return (int64_t)std::stoll( myodd::strings::String2WString(*(static_cast<std::string*>(ipc->pData))));

  case IpcDataType::None:
  default:
    throw std::exception( "Unnown data type.");
  }

  // never reached
  return 0;
}

/**
 * Given an index we adjust the number in relation to the guid(s)
 * @param unsigned int givenIndex the given index we want to get.
 * @return unsigned int the adjusted index number.
 */
unsigned int IpcData::AdjustIndexNumber(unsigned int givenIndex) const
{
  // sanity check, (number or arguments does not include the GUIDs).
  if ( givenIndex >= GetNumArguments())
  {
    throw std::exception( "The argument index is out of range.");
  }

  auto offset = 0;
  for (unsigned int i = 0; i <= givenIndex; ++i)
  {
    auto ipc = _ipcArguments[i];
    switch (ipc->dataType)
    {
    case IpcDataType::Guid:
      ++offset;
      break;
    }
  }
  return (givenIndex + offset);
}

/**
 * Remove an argument at a given index.
 * @param unsigned int index the argument index we want to remove.
 * @retrun none
 */
void IpcData::RemoveArgument(unsigned int index)
{
  // sanity check, (number or arguments does not include the GUIDs).
  if (index >= GetNumArguments())
  {
    throw std::exception( "The argument index is out of range." );
  }

  // reset the pointer
  ResetPtr();

  // adjust the index
  index = AdjustIndexNumber(index);

  //  we removed an index.
  --_numArguments;

  auto it = _ipcArguments.begin() + index;

  // the pointer
  auto ia = (*it);

  switch (ia->dataType)
  {
  case IpcDataType::Int32:
    // delete the data.
    delete static_cast<signed int*>(ia->pData);
    break;

  case IpcDataType::String:
  case IpcDataType::Guid:
    // delete the data.
    delete static_cast<std::wstring*>(ia->pData);
    break;

  case IpcDataType::StringAscii:
    // delete the data.
    delete static_cast<std::string*>(ia->pData);
    break;

  case IpcDataType::None:
  default:
    throw std::exception( "Unnown data type.");
  }

  // delete the data
  delete ia;

  // remove it
  _ipcArguments.erase(it);
}
//
} // os
} // myodd