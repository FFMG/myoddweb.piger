#include "stdafx.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

#include "pluginapi.h"
#include "helperapi.h"
#include "pluginVirtualMachine.h"

/**
 * Todo
 * @param void
 * @return void
 */
pluginapi::pluginapi(void)
{
}

/**
 * Todo
 * @param void
 * @return void
 */
pluginapi::~pluginapi(void)
{
}

/**
 * Todo
 * @param void
 * @return void
 */
void pluginapi::Initialize ( )
{
  PluginVirtualMachine& vm = GetPluginVirtualMachine();
  vm.Register( _T("say"), pluginapi::say );
  vm.Register( _T("version"), pluginapi::version );
  vm.Register( _T("getCommand"), pluginapi::getCommand );
  vm.Register( _T("getAction"), pluginapi::getAction );
  vm.Register( _T("getCommandCount"),  pluginapi::getCommandCount );
  vm.Register( _T("execute"),  pluginapi::execute );
  vm.Register( _T("getString"),  pluginapi::getString );
  vm.Register( _T("getFile"),  pluginapi::getFile );
  vm.Register( _T("getFolder"),  pluginapi::getFolder );
  vm.Register( _T("getURL"),  pluginapi::getURL );
  vm.Register( _T("addAction"),  pluginapi::addAction );
  vm.Register( _T("removeAction"),  pluginapi::removeAction );
  vm.Register( _T("getVersion"),  pluginapi::getVersion );
  vm.Register( _T("findAction"),  pluginapi::findAction );
}

/**
 * Todo
 * @see helperapi::version
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
 * @see helperapi::say
 * @param void
 * @param void
 * @param void
 * @return void
 */
bool pluginapi::say( LPCWSTR msg, UINT nElapse, UINT nFadeOut)
{
  // display the message
  // and we can now display the message.
  return helperapi::say( msg, nElapse, nFadeOut );
}

/**
 * Todo
 * @see helperapi::getCommand
 * @param void
 * @param void
 * @param void
 * @return void
 */
int pluginapi::getCommand( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !helperapi::getCommand( idx, sValue ) )
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
 * @see helperapi::getAction
 * @param void
 * @param void
 * @param void
 * @return void
 */
int pluginapi::getAction( DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !helperapi::getAction( sValue ) )
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
 * @see helperapi::getCommandCount
 * @return in the number of commanded entered by the user
 */
int pluginapi::getCommandCount()
{
  return helperapi::getCommandCount();
}

/**
 * Todo
 * @see helperapi::execute
 * @param void
 * @param void
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return void
 */
bool pluginapi::execute( LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged )
{
  return helperapi::execute( module, cmdLine, isPrivileged );
}

/**
 * Todo
 * @see helperapi::getString
 * @param void
 * @param void
 * @return void
 */
int pluginapi::getString( DWORD nBufferLength, LPWSTR lpBuffer)
{
  STD_TSTRING sValue = _T("");
  if( !helperapi::getString ( sValue ) )
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
 * @see helperapi::getFile
 * @param void
 * @param void
 * @param void
 * @return void
 */
int pluginapi::getFile(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !helperapi::getFile(idx, sValue ) )
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
 * @see helperapi::getFolder
 * @param void
 * @param void
 * @param void
 * @return void
 */
int pluginapi::getFolder (UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !helperapi::getFolder (idx, sValue ) )
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
 * @see helperapi::getURL
 * @param void
 * @param void
 * @param void
 * @return void
 */
int pluginapi::getURL(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer )
{
  STD_TSTRING sValue = _T("");
  if( !helperapi::getURL (idx, sValue ) )
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
 * @see helperapi::addAction
 * @param void
 * @param void
 * @return void
 */
bool pluginapi::addAction( LPCWSTR szText, LPCWSTR szPath )
{
  // pass it straight to the helper API as it will do all the needed validations
  return helperapi::addAction( szText, szPath );
}

/**
 * Todo
 * @see helperapi::removeAction
 * @param void
 * @param void
 * @return void
 */
bool pluginapi::removeAction( LPCWSTR szText, LPCWSTR szPath )
{
  // pass it straight to the helper API as it will do all the needed validations
  return helperapi::removeAction( szText, szPath );
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
  if( !helperapi::getVersion( stdVersion ) )
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
  if( !helperapi::findAction( idx, lpCommand, stdActionPath ) )
  {
    return false;
  }
  wcsncpy_s( lpBuffer, nBufferLength, stdActionPath.c_str(), nBufferLength );
  return true;
}