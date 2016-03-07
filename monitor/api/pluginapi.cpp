#include "StdAfx.h"

#include "pluginapi.h"
#include "pluginVirtualMachine.h"

/**
 * Todo
 * @param void
 * @return void
 */
pluginapi::pluginapi() : helperapi()
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
double pluginapi::version ( )
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
bool pluginapi::say( LPCWSTR msg, UINT nElapse, UINT nFadeOut)
{
  // display the message
  // and we can now display the message.
  return __super::say( msg, nElapse, nFadeOut );
}

/**
 * Todo
 * @see __super::getCommand
 * @param UINT idx the command number we want to get.
 * @param DWORD nBufferLength the max buffer length that we want to get. 
 * @param LPWSTR lpBuffer the buffer that will contain the command, if it is NULL only the size will be returned.
 * @return void
 */
size_t pluginapi::getCommand( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  // first get the command
  STD_TSTRING sValue = _T("");
  if( !__super::getCommand( idx, sValue ) )
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
int pluginapi::getAction( DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !__super::getAction( sValue ) )
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
size_t pluginapi::getCommandCount()
{
  return __super::getCommandCount();
}

/**
 * Todo
 * @see __super::execute
 * @param void
 * @param void
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return void
 */
bool pluginapi::execute( LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged )
{
  return __super::execute( module, cmdLine, isPrivileged );
}

/**
 * Todo
 * @see __super::getString
 * @param void
 * @param void
 * @return void
 */
int pluginapi::getString( DWORD nBufferLength, LPWSTR lpBuffer)
{
  STD_TSTRING sValue = _T("");
  if( !__super::getString ( sValue ) )
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
int pluginapi::getFile(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !__super::getFile(idx, sValue ) )
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
int pluginapi::getFolder (UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !__super::getFolder (idx, sValue ) )
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
int pluginapi::getURL(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !__super::getURL (idx, sValue ) )
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
bool pluginapi::addAction( LPCWSTR szText, LPCWSTR szPath )
{
  // pass it straight to the helper API as it will do all the needed validations
  return __super::addAction( szText, szPath );
}

/**
 * Todo
 * @see __super::removeAction
 * @param void
 * @param void
 * @return void
 */
bool pluginapi::removeAction( LPCWSTR szText, LPCWSTR szPath )
{
  // pass it straight to the helper API as it will do all the needed validations
  return __super::removeAction( szText, szPath );
}

/**
 * Get the action monitor version string.
 * @param DWORD the max size of the buffer.
 * @param LPWSTR the buffer that will contain the return data, (version).
 * @return bool success or not if there was an error.
 */
bool pluginapi::getVersion(DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING stdVersion;
  if( !__super::getVersion( stdVersion ) )
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
bool pluginapi::findAction
( 
  UINT idx, 
  LPCWSTR lpCommand, 
  DWORD nBufferLength, 
  LPWSTR lpBuffer
)
{
  STD_TSTRING stdActionPath;
  if( !__super::findAction( idx, lpCommand, stdActionPath ) )
  {
    return false;
  }
  wcsncpy_s( lpBuffer, nBufferLength, stdActionPath.c_str(), nBufferLength );
  return true;
}