// AppPaths.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AppPaths.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AppPaths* m_appPath = NULL;

PLUGIN_API
AM_RESPONSE am_Msg(AM_MSG msg, AM_UINT wParam, AM_INT lParam)
{
  switch ( msg )
  {
  case AM_MSG_INIT:
    m_appPath = new AppPaths;
    m_appPath->Init( *((AmPlugin*)lParam) );
    return AM_RESP_TRUE;
    break;

  case AM_MSG_DEINIT:
    delete m_appPath;
    m_appPath = NULL;
    return AM_RESP_TRUE;
    break;
  }
  // not handled here.
  return AM_RESP_NOT_SUPPORTED;
}

// ----------------------------------------------------------
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

AppPaths::AppPaths()
{
}

AppPaths::~AppPaths()
{
}

/**
 * Query all the values for a certain given key
 * @param HKEY the key we are querying.
 * @param const std::wstring& full registry path
 * @param REG_VALUES& all the registry values.
 * @return none
 */
void AppPaths::QueryKeyValues
( 
 HKEY hKey, 
 const std::wstring& sFullKey,
 REG_VALUES& values
 )
{ 
  HKEY hOpenKey;
  if( RegOpenKeyEx( hKey,
    sFullKey.c_str(),
    0,
    KEY_READ,
    &hOpenKey) != ERROR_SUCCESS
    )
  {
    // could not get the values.
    return;
  }

  WCHAR    achClass[MAX_PATH] = L"";  // buffer for class name 
  DWORD    cchClassName = MAX_PATH;   // size of class string 
  DWORD    cSubKeys=0;                // number of subkeys 
  DWORD    cbMaxSubKey;               // longest subkey size 
  DWORD    cchMaxClass;               // longest class string 
  DWORD    cchType;                   // the data type.
  DWORD    cchDataSize;               // the data type.
  DWORD    cValues;                   // number of values for key 
  DWORD    cchMaxValue;               // longest value name 
  DWORD    cbMaxValueData;            // longest value data 
  DWORD    cbSecurityDescriptor;      // size of security descriptor 
  FILETIME ftLastWriteTime;           // last write time 

  DWORD i, retCode; 

  WCHAR  achValue[MAX_VALUE_NAME]; 
  DWORD cchValue = MAX_VALUE_NAME; 

  // Get the class name and the value count. 
  retCode = RegQueryInfoKey(
                            hOpenKey,                // key handle 
                            achClass,                // buffer for class name 
                            &cchClassName,           // size of class string 
                            NULL,                    // reserved 
                            &cSubKeys,               // number of subkeys 
                            &cbMaxSubKey,            // longest subkey size 
                            &cchMaxClass,            // longest class string 
                            &cValues,                // number of values for this key 
                            &cchMaxValue,            // longest value name 
                            &cbMaxValueData,         // longest value data 
                            &cbSecurityDescriptor,   // security descriptor 
                            &ftLastWriteTime);       // last write time 

  // Enumerate the key values. 
  if (cValues) 
  {
    for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
    { 
      cchValue = MAX_VALUE_NAME; 
      achValue[0] = '\0'; 
      retCode = RegEnumValue(hOpenKey, 
                             i, 
                             achValue, 
                             &cchValue, 
                             NULL, 
                             &cchType,
                             NULL,
                             &cchDataSize);
      if (retCode != ERROR_SUCCESS ) 
      {
        ASSERT(0);  //  why could we not get they key?
        continue;
      }

      switch( cchType )
      {
      case REG_SZ:
      case REG_MULTI_SZ:
      case REG_EXPAND_SZ:
        {
          auto* sz = new WCHAR[ cchDataSize+1];
          try
          {
            memset(sz, 0, cchDataSize + 1);

            retCode = RegQueryValueEx(hOpenKey,
              achValue,
              NULL,
              NULL,
              (LPBYTE)sz,
              &cchDataSize);

            if (retCode == ERROR_SUCCESS)
            {
              values[lower(achValue)] = lower(sz);
            }
            delete[] sz;
            sz = nullptr;
          }
          catch( ... )
          {
            //  did we throw?
            if (nullptr != sz)
            {
              delete[] sz;
            }
            throw;
          }
        }
        break;

      default:
        break;
      }// switch
    }// for each keys.
  }

  //  we are done we the key
  RegCloseKey( hOpenKey );
}

/**
 * Query all the values for a certain given key
 * @param HKEY the key we are querying.
 * @param const std::wstring& full registry path
 * @param REG_NAMES& all the registry values.
 * @return none
 */
void AppPaths::QueryKey
( 
  HKEY hKey, 
  const std::wstring& sFullKey,
  REG_NAMES& values
)
{ 
  HKEY hOpenKey;
  if( RegOpenKeyEx( hKey,
                    sFullKey.c_str(),
                    0,
                    KEY_READ,
                    &hOpenKey) != ERROR_SUCCESS
                   )
  {
    // could not get the values.
    return;
  }

  WCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
  DWORD    cbName;                   // size of name string 
  WCHAR    achClass[MAX_PATH] = L"";  // buffer for class name 
  DWORD    cchClassName = MAX_PATH;  // size of class string 
  DWORD    cSubKeys=0;               // number of subkeys 
  DWORD    cbMaxSubKey;              // longest subkey size 
  DWORD    cchMaxClass;              // longest class string 
  DWORD    cValues;              // number of values for key 
  DWORD    cchMaxValue;          // longest value name 
  DWORD    cbMaxValueData;       // longest value data 
  DWORD    cbSecurityDescriptor; // size of security descriptor 
  FILETIME ftLastWriteTime;      // last write time 

  DWORD i, retCode; 
  DWORD cchValue = MAX_VALUE_NAME; 

  // Get the class name and the value count. 
  retCode = RegQueryInfoKey(
                            hOpenKey,                // key handle 
                            achClass,                // buffer for class name 
                            &cchClassName,           // size of class string 
                            NULL,                    // reserved 
                            &cSubKeys,               // number of subkeys 
                            &cbMaxSubKey,            // longest subkey size 
                            &cchMaxClass,            // longest class string 
                            &cValues,                // number of values for this key 
                            &cchMaxValue,            // longest value name 
                            &cbMaxValueData,         // longest value data 
                            &cbSecurityDescriptor,   // security descriptor 
                            &ftLastWriteTime);       // last write time 

  // Enumerate the subkeys, until RegEnumKeyEx fails.

  if (cSubKeys)
  {
    //  go around all the sub keys and add them to the list of actions.
    for (i=0; i<cSubKeys; ++i) 
    { 
      cbName = MAX_KEY_LENGTH;
      retCode = RegEnumKeyEx(hOpenKey, 
                             i,
                             achKey, 
                             &cbName, 
                             NULL, 
                             NULL, 
                             NULL, 
                             &ftLastWriteTime
                             ); 
      if (retCode == ERROR_SUCCESS) 
      {
        // add this action.
        std::wstring sSubKey = sFullKey + L"\\" + achKey;
        QueryKeyValues( hKey, sSubKey, values[ lower(achKey) ] );
      }
    }
  } 

  //  we are done we the key
  RegCloseKey( hOpenKey );
}

/**
 * Go around all the registry keys and add the entries as actions.
 * @param AmPlugin& the AmPlugin that we will use to add actions.
 * @return none
 */
void AppPaths::Init(AmPlugin& am )
{
  //
  // add the registry values.
  QueryRegistry( am );
}

/**
 * Query all the app path registry keys.
 * @param AmPlugin& the common plugin code.
 * @return none.
 */
void AppPaths::QueryRegistry(AmPlugin& am )
{
  //
  // read all the name values
  REG_NAMES values;
  QueryKey( HKEY_LOCAL_MACHINE, 
            L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths",
            values
           );

  //
  // go around all the values we found and add a corresponding action.
  for(REG_NAMES::const_iterator it = values.begin();
      it != values.end();
      ++it )
  {
    // get the name
    const std::wstring& name = it->first;
    const REG_VALUES& value = it->second;

    REG_VALUES::const_iterator itFile = value.find( L"" );
    if( itFile == value.end() || itFile->second.length() == 0 )
    {
      continue;
    }

    // get the extension given in the name
    LPCWSTR pdest = wcsrchr( name.c_str(), '.' );
    std::wstring ext = L"";
    std::wstring action = name;
    if( pdest )
    {
      ext = std::wstring(pdest+1);
      action = action.substr( 0, pdest - name.c_str() );
    }

    //
    // we don't add dlls.
    if( 0 == _wcsicmp( L"dll", ext.c_str() ))
    {
      continue;
    }

    //
    // the path of the file.
    std::wstring filePath = itFile->second;

    //
    // strip the quotes.
    strip_quotes( filePath );

    //
    // make sure that the files does exist.
    if( !file_exists( filePath ) )
    {
      continue;
    }

    //
    // if the action exists we do not want to overwright it.
    WCHAR lpPath[ MAX_PATH ];
    if( !am.FindAction( 0, action.c_str(), MAX_PATH, lpPath ) )
    {
      //
      // it all looks good, we can add this command.
      am.AddAction( action.c_str(), filePath.c_str() );
    }
  }
}

/**
 * Return true if a file exists or not.
 * @param const std::wstring& the file path we are checking for.
 * @return bool true|false if the file exists or not.
 */
bool AppPaths::file_exists(const std::wstring& s ) const
{
  // does the file actually exist
  // if not then we cannot really run it.
  _tfinddata_t fdata;
  // look for the directory, if it exists then we don't need to go further.
  intptr_t ffhandle = _wfindfirst( s.c_str(), &fdata );
  if( ffhandle == -1 )
  {
    // this file does not exist.
    return false;
  }

  // the file exists.
  _findclose( ffhandle );
  return true;
}

/**
 * Strip double quotes from a given string.
 * @param std::wstring& the path we want to clean up
 * @return none
 */
void AppPaths::strip_quotes( std::wstring& s) const
{
  if( s.length() > 0 && s[0] == L'\"' )
  {
    s = s.substr( 1 );
  }
  if( s.length() > 0 && s[s.length() -1 ] == L'\"' )
  {
    s = s.substr( 0, s.length() -1 );
  }
}

/**
 * Convert a string to lowercase.
 * @param const std::wstring& the string we want to lower case.
 * @return std::wstring the lower case version of the given string
 */
std::wstring AppPaths::lower( const std::wstring& s) const
{
  std::wstring ret = s;
  for( std::wstring::iterator it = ret.begin();
       it != ret.end();
       ++it )
  {
    if( *it >= L'A' && *it <= L'Z' )
    {
      *it = (L'a'+(*it-L'A'));
    }
  }
  return ret;
}
