#include "StdAfx.h"

#include "pluginapi.h"
#include "pluginVirtualMachine.h"

/**
 * Todo
 * @param void
 * @return void
 */
pluginapi::pluginapi(const ActiveAction& action) : helperapi(action )
{
}

/**
 * Todo
 * @param void
 * @return void
 */
pluginapi::~pluginapi()
{
}

/**
 * Todo
 * @see __super::version
 * @param void
 * @return void
 */
double pluginapi::Version ( )
{
  // short and sweet
  // all we need is the version number.
  return (ACTIONMONITOR_API_PLUGIN_VERSION );
}

/**
 * Todo
 * @see __super::say
 * @param void
 * @param void
 * @param void
 * @return void
 */
bool pluginapi::Say( LPCWSTR msg, UINT nElapse, UINT nFadeOut)
{
  // display the message
  // and we can now display the message.
  return __super::Say( msg, nElapse, nFadeOut );
}

/**
 * Todo
 * @see __super::getCommand
 * @param UINT idx the command number we want to get.
 * @param DWORD nBufferLength the max buffer length that we want to get. 
 * @param LPWSTR lpBuffer the buffer that will contain the command, if it is NULL only the size will be returned.
 * @return void
 */
size_t pluginapi::GetCommand( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  // first get the command
  STD_TSTRING sValue = _T("");
  if( !__super::GetCommand( idx, sValue ) )
  {
    return 0;
  }

  // then copy the data into the buffer.
  size_t len = sValue.length();
  if ( nBufferLength > 0 && lpBuffer )
  {
    // clear the buffer.
    memset( lpBuffer, 0, nBufferLength * sizeof(TCHAR) );

    // then copy the word.
    _tcsncpy_s( lpBuffer, (nBufferLength > len+1 ? len+1 : nBufferLength), sValue.c_str(), _TRUNCATE );
  }
  return len;
}

/**
 * Todo
 * @see __super::getAction
 * @param void
 * @param void
 * @param void
 * @return void
 */
int pluginapi::GetAction( DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !__super::GetAction( sValue ) )
  {
    return 0;
  }

  size_t len = sValue.length();
  if ( nBufferLength > 0 && lpBuffer )
  {
    memset( lpBuffer, 0, nBufferLength );
    _tcsncpy_s( lpBuffer, nBufferLength, sValue.c_str(), _TRUNCATE );
  }

  return len;
}

/**
 * Get the number of command, (space delimited).
 * @see __super::getCommandCount
 * @return in the number of commanded entered by the user
 */
size_t pluginapi::GetCommandCount()
{
  return __super::GetCommandCount();
}

/**
 * Todo
 * @see __super::execute
 * @param void
 * @param void
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return void
 */
bool pluginapi::Execute( LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged )
{
  return __super::Execute( module, cmdLine, isPrivileged );
}

/**
 * Todo
 * @see __super::getString
 * @param void
 * @param void
 * @return void
 */
int pluginapi::GetString( DWORD nBufferLength, LPWSTR lpBuffer)
{
  STD_TSTRING sValue = _T("");
  if( !__super::GetString ( sValue ) )
  {
    return 0;
  }

  size_t len = sValue.length();
  if ( nBufferLength > 0 && lpBuffer )
  {
    memset( lpBuffer, 0, nBufferLength );
    _tcsncpy_s( lpBuffer, nBufferLength, sValue.c_str(), _TRUNCATE );
  }
  return len;
}

/**
 * Todo
 * @see __super::getFile
 * @param void
 * @param void
 * @param void
 * @return void
 */
int pluginapi::GetFile(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !__super::GetFile(idx, sValue ) )
  {
    return 0;
  }

  size_t len = sValue.length();
  if ( nBufferLength > 0 && lpBuffer )
  {
    memset( lpBuffer, 0, nBufferLength );
    _tcsncpy_s( lpBuffer, nBufferLength, sValue.c_str(), _TRUNCATE );
  }
  return len;
}

/**
 * Todo
 * @see __super::getFolder
 * @param void
 * @param void
 * @param void
 * @return void
 */
int pluginapi::GetFolder (UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !__super::GetFolder (idx, sValue ) )
  {
    return 0;
  }

  size_t len = sValue.length();
  if ( nBufferLength > 0 && lpBuffer )
  {
    memset( lpBuffer, 0, nBufferLength );
    _tcsncpy_s( lpBuffer, nBufferLength, sValue.c_str(), _TRUNCATE );
  }
  return len;
}

/**
 * Todo
 * @see __super::getURL
 * @param void
 * @param void
 * @param void
 * @return void
 */
int pluginapi::GetURL(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !__super::GetURL (idx, sValue ) )
  {
    return 0;
  }

  size_t len = sValue.length();
  if ( nBufferLength > 0 && lpBuffer )
  {
    memset( lpBuffer, 0, nBufferLength );
    _tcsncpy_s( lpBuffer, nBufferLength, sValue.c_str(), _TRUNCATE );
  }
  return len;
}

/**
 * Todo
 * @see __super::addAction
 * @param void
 * @param void
 * @return void
 */
bool pluginapi::AddAction( LPCWSTR szText, LPCWSTR szPath )
{
  // pass it straight to the helper API as it will do all the needed validations
  return __super::AddAction( szText, szPath );
}

/**
 * Todo
 * @see __super::RemoveAction
 * @param void
 * @param void
 * @return void
 */
bool pluginapi::RemoveAction( LPCWSTR szText, LPCWSTR szPath )
{
  // pass it straight to the helper API as it will do all the needed validations
  return __super::RemoveAction( szText, szPath );
}

/**
 * Get the action monitor version string.
 * @param DWORD the max size of the buffer.
 * @param LPWSTR the buffer that will contain the return data, (version).
 * @return bool success or not if there was an error.
 */
bool pluginapi::GetVersion(DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING stdVersion;
  if( !__super::GetVersion( stdVersion ) )
  {
    return false;
  }
  wcsncpy_s( lpBuffer, nBufferLength, stdVersion.c_str(), nBufferLength );
  return true;
}

/**
 * Find an action.
 * @param UINT the action number we are after.
 * @param LPCWSTR the action name we are looking for.
 * @param DWORD the max size of the buffer.
 * @param LPWSTR the buffer that will contain the return data, (version).
 * @return bool if the command exists or not.
 */
bool pluginapi::FindAction
( 
  UINT idx, 
  LPCWSTR lpCommand, 
  DWORD nBufferLength, 
  LPWSTR lpBuffer
)
{
  STD_TSTRING stdActionPath;
  if( !__super::FindAction( idx, lpCommand, stdActionPath ) )
  {
    return false;
  }
  wcsncpy_s( lpBuffer, nBufferLength, stdActionPath.c_str(), nBufferLength );
  return true;
}