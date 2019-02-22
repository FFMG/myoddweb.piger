#include "StdAfx.h"

#include "PluginApi.h"
#include "pluginVirtualMachine.h"

/**
 * Todo
 * @param void
 * @return void
 */
PluginApi::PluginApi(const ActiveAction& action, IMessages& messages) : 
  HelperApi(action, messages )
{
}

/**
 * Todo
 * @param void
 * @return void
 */
PluginApi::~PluginApi()
{
}

/**
 * Todo
 * @see __super::version
 * @param void
 * @return void
 */
double PluginApi::Version ( )
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
bool PluginApi::Say(const wchar_t* msg, const unsigned int nElapse, const unsigned int nFadeOut) const
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
 * @param wchar_t* lpBuffer the buffer that will contain the command, if it is NULL only the size will be returned.
 * @return void
 */
size_t PluginApi::GetCommand(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer ) const
{
  // first get the command
  MYODD_STRING sValue = _T("");
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
int PluginApi::GetAction(unsigned int nBufferLength, wchar_t* lpBuffer ) const
{
  MYODD_STRING sValue = _T("");
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

  return static_cast<int>(len);
}

/**
 * Get the number of command, (space delimited).
 * @see __super::getCommandCount
 * @return in the number of commanded entered by the user
 */
size_t PluginApi::GetCommandCount()
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
bool PluginApi::Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged, HANDLE* hProcess) const
{
  if (!__super::Execute(module, cmdLine, isPrivileged, hProcess ))
  {
    // tell the user it did not work
    __super::Say(_T("<b>Error : </b> There was an error executing the request, please check the parameters."), 3000, 5);
    return false;
  }
  return true;
}

/**
 * Todo
 * @see __super::getString
 * @param void
 * @param void
 * @return void
 */
int PluginApi::GetString(unsigned int nBufferLength, wchar_t* lpBuffer, bool bQuote ) const
{
  MYODD_STRING sValue = _T("");
  if( !__super::GetString ( sValue, bQuote ) )
  {
    return 0;
  }

  size_t len = sValue.length();
  if ( nBufferLength > 0 && lpBuffer )
  {
    memset( lpBuffer, 0, nBufferLength );
    _tcsncpy_s( lpBuffer, nBufferLength, sValue.c_str(), _TRUNCATE );
  }
  return static_cast<int>(len);
}

/**
 * Todo
 * @see __super::getFile
 * @param void
 * @param void
 * @param void
 * @return void
 */
int PluginApi::GetFile(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer, bool bQuote) const
{
  MYODD_STRING sValue = _T("");
  if( !__super::GetFile(idx, sValue, bQuote ) )
  {
    return 0;
  }

  size_t len = sValue.length();
  if ( nBufferLength > 0 && lpBuffer )
  {
    memset( lpBuffer, 0, nBufferLength );
    _tcsncpy_s( lpBuffer, nBufferLength, sValue.c_str(), _TRUNCATE );
  }
  return static_cast<int>(len);
}

/**
 * Todo
 * @see __super::getFolder
 * @param void
 * @param void
 * @param void
 * @return void
 */
int PluginApi::GetFolder (unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer, bool bQuote) const
{
  MYODD_STRING sValue = _T("");
  if( !__super::GetFolder (idx, sValue, bQuote ) )
  {
    return 0;
  }

  size_t len = sValue.length();
  if ( nBufferLength > 0 && lpBuffer )
  {
    memset( lpBuffer, 0, nBufferLength );
    _tcsncpy_s( lpBuffer, nBufferLength, sValue.c_str(), _TRUNCATE );
  }
  return static_cast<int>(len);
}

/**
 * Todo
 * @see __super::getUrl
 * @param void
 * @param void
 * @param void
 * @return void
 */
int PluginApi::GetUrl(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer, bool bQuote) const
{
  MYODD_STRING sValue = _T("");
  if( !__super::GetUrl (idx, sValue, bQuote ) )
  {
    return 0;
  }

  const auto len = sValue.length();
  if ( nBufferLength > 0 && lpBuffer )
  {
    memset( lpBuffer, 0, nBufferLength );
    _tcsncpy_s( lpBuffer, nBufferLength, sValue.c_str(), _TRUNCATE );
  }
  return static_cast<int>(len);
}

/**
 * Todo
 * @see __super::addAction
 * @param void
 * @param void
 * @return void
 */
bool PluginApi::AddAction(const wchar_t* szText, const wchar_t* szPath ) const
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
bool PluginApi::RemoveAction(const wchar_t* szText, const wchar_t* szPath ) const
{
  // pass it straight to the helper API as it will do all the needed validations
  return __super::RemoveAction( szText, szPath );
}

/**
 * Get the action monitor version string.
 * @param DWORD the max size of the buffer.
 * @param wchar_t* the buffer that will contain the return data, (version).
 * @return bool success or not if there was an error.
 */
bool PluginApi::GetVersion(unsigned int nBufferLength, wchar_t* lpBuffer ) const
{
  MYODD_STRING stdVersion;
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
 * @param const wchar_t* the action name we are looking for.
 * @param DWORD the max size of the buffer.
 * @param wchar_t* the buffer that will contain the return data, (version).
 * @return bool if the command exists or not.
 */
bool PluginApi::FindAction
( 
  unsigned int idx,
  const wchar_t* lpCommand,
  unsigned int nBufferLength,
  wchar_t* lpBuffer
) const
{
  MYODD_STRING stdActionPath;
  if( !__super::FindAction( idx, lpCommand, stdActionPath ) )
  {
    return false;
  }
  wcsncpy_s( lpBuffer, nBufferLength, stdActionPath.c_str(), nBufferLength );
  return true;
}

/**
 * Get the last foreground window. This is the window that was last on top.
 * It is posible to return NULL if the window is not
 * @return HWND the last top window.
 */
HWND PluginApi::GetForegroundWindow() const
{
  return __super::GetForegroundWindow();
}

/**
 * Log a message.
 * @param unsigned int logType the message type we are logging.
 * @param const wchar_t* lpText the text we wish to log.
 * @return none.
 */
void PluginApi::Log(unsigned int logType, const wchar_t* lpText) const
{
  __super::Log(logType, lpText);
}