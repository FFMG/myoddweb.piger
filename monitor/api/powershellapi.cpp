#include "StdAfx.h"

#include "PowershellApi.h"
#include "pluginVirtualMachine.h"

/**
 * Todo
 * @param void
 * @return void
 */
PowershellApi::PowershellApi(const ActiveAction& action) : HelperApi(action )
{
}

/**
 * Todo
 * @param void
 * @return void
 */
PowershellApi::~PowershellApi()
{
}

/**
 * Todo
 * @see __super::version
 * @param void
 * @return void
 */
double PowershellApi::Version ( )
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
bool PowershellApi::Say(const wchar_t* msg, const unsigned int nElapse, const unsigned int nFadeOut) const
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
size_t PowershellApi::GetCommand( UINT idx, DWORD nBufferLength, wchar_t* lpBuffer )
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
int PowershellApi::GetAction( DWORD nBufferLength, wchar_t* lpBuffer )
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
size_t PowershellApi::GetCommandCount()
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
bool PowershellApi::Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged, HANDLE* hProcess) const
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
int PowershellApi::GetString( DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote )
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
int PowershellApi::GetFile(UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote)
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
int PowershellApi::GetFolder (UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote)
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
 * @see __super::getURL
 * @param void
 * @param void
 * @param void
 * @return void
 */
int PowershellApi::GetURL(UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote)
{
  MYODD_STRING sValue = _T("");
  if( !__super::GetURL (idx, sValue, bQuote ) )
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
 * @see __super::addAction
 * @param void
 * @param void
 * @return void
 */
bool PowershellApi::AddAction(const wchar_t* szText, const wchar_t* szPath )
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
bool PowershellApi::RemoveAction(const wchar_t* szText, const wchar_t* szPath )
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
bool PowershellApi::GetVersion(DWORD nBufferLength, wchar_t* lpBuffer )
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
bool PowershellApi::FindAction
( 
  UINT idx, 
  const wchar_t* lpCommand,
  DWORD nBufferLength, 
  wchar_t* lpBuffer
)
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
HWND PowershellApi::GetForegroundWindow() const
{
  return __super::GetForegroundWindow();
}

/**
 * Log a message.
 * @param unsigned int logType the message type we are logging.
 * @param const wchar_t* lpText the text we wish to log.
 * @return none.
 */
void PowershellApi::Log(unsigned int logType, const wchar_t* lpText)
{
  __super::Log(logType, lpText);
}

bool PowershellApi::Say(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  const int ARGUMENT_TEXT = 0;
  const int ARGUMENT_ELAPSE = 1;
  const int ARGUMENT_FADEOUT = 2;

  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount < 2 || argumentCount > 3)
  {
    auto errorMsg = _T("<b>Error : </b> Missing <i>Elapse</i> time.<br>Format is <i>am_say( msg, <b>elapse</b>[, fade=0])</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsString(ARGUMENT_TEXT))
  {
    auto errorMsg = _T("<b>Error : </b> The first argument must be a string.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsInt( ARGUMENT_ELAPSE))
  {
    auto errorMsg = _T("<b>Error : </b> The elapse parameters must be an integer.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  auto message = ipcRequest.Get<std::wstring>(ARGUMENT_TEXT);
  auto elapse = ipcRequest.Get<unsigned int>(ARGUMENT_ELAPSE);
  if (elapse == 0)
  {
    auto errorMsg = _T("<b>Error : </b> Missing <i>Elapse</i> time.<br>Format is <i>am_say( msg, <b>elapse</b>[, fade=0])</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  auto fade = 0;
  if (argumentCount == 3)
  {
    if (!ipcRequest.IsInt(ARGUMENT_FADEOUT))
    {
      auto errorMsg = _T("<b>Error : </b> The fadout parameters must be an integer.");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      return false;
    }
    fade = ipcRequest.Get<unsigned int>(ARGUMENT_FADEOUT);
  }

  // add the boolean response.
  auto b = Say(message.c_str(), elapse, fade);
  ipcResponse.Add( b );
  return b;
}