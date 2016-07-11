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
 * Execute a certain application/script.
 * @see __super::execute
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return bool success or not.
 */
bool PowershellApi::Execute(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  const auto ARGUMENT_MODULE = 0;
  const auto ARGUMENT_ARGS = 1;
  const auto ARGUMENT_PRIVILEGED = 2;

  // get the number of arguments.
  // we can only have one or 2 arguments 
  // 
  // the first argument is the name of the module
  // and the second is the arguments.
  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount < 1 || argumentCount > 3)
  {
    auto errorMsg = _T("<b>Error : </b> Missing Module and/or command line.<br>Format is <i>am_execute( module [, commandLine [, privileged])</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsString(ARGUMENT_MODULE) || !ipcRequest.IsString(ARGUMENT_ARGS))
  {
    auto errorMsg = _T("<b>Error : </b> The first and second parameters must be strings.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  bool isPrivileged = false;
  if (argumentCount == 3)
  {
    if (!ipcRequest.IsInt(ARGUMENT_PRIVILEGED))
    {
      auto errorMsg = _T("<b>Error : </b> The third argument, (privileged), can only be true|false");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      return false;
    }
    isPrivileged = (ipcRequest.Get<unsigned int>(ARGUMENT_PRIVILEGED) == 1);
  }

  auto module = ipcRequest.Get<std::wstring>(ARGUMENT_MODULE);
  auto cmdLine = ipcRequest.Get<std::wstring>( ARGUMENT_ARGS);

  // run the query
  bool result = __super::Execute(module.c_str(), cmdLine.c_str(), isPrivileged, nullptr);

  // push the result.
  ipcResponse.Add( result ? 1 : 0);

  // tell the user it did not work
  if (false == result)
  {
    auto errorMsg = _T("<b>Error : </b> There was an error executing the request, please check the parameters.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
  }

  // return the number of results
  return true;
}

/**
 * Overide function to execute a process.
 * @param const wchar_t* module the module we want to execute.
 * @param const wchar_t* cmdLine the arguments to pass to this module.
 * @param bool isPrivileged, HANDLE* hProcess if not null, the handle of the created process.
 * @return bool success or not.
 */
bool PowershellApi::Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged, HANDLE* hProcess) const
{
  return __super::Execute(module, cmdLine, isPrivileged, hProcess);
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

bool PowershellApi::Say(const wchar_t* msg, const unsigned int nElapse, const unsigned int nFadeOut) const
{
  return __super::Say(msg, nElapse, nFadeOut);
}

/**
 * Output a message on the screen.
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return bool success or not.
 */
bool PowershellApi::Say(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  const auto ARGUMENT_TEXT = 0;
  const auto ARGUMENT_ELAPSE = 1;
  const auto ARGUMENT_FADEOUT = 2;

  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount < 2 || argumentCount > 3)
  {
    auto errorMsg = _T("<b>Error : </b> Missing <i>Elapse</i> time.<br>Format is <i>Say( msg, <b>elapse</b>[, fade=0])</i>");
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
    auto errorMsg = _T("<b>Error : </b> Missing <i>Elapse</i> time.<br>Format is <i>Say( msg, <b>elapse</b>[, fade=0])</i>");
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
  auto b = __super::Say(message.c_str(), elapse, fade);
  ipcResponse.Add( b );
  return b;
}

/**
 * Get the powerhsell api version number.
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return bool success or not.
 */
bool PowershellApi::Version(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount > 0 )
  {
    auto errorMsg = _T("<b>Error : </b> The 'Version' function does not take any arguments.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  // short and sweet
  // all we need is the version number.
  ipcResponse.Add(ACTIONMONITOR_PS_PLUGIN_VERSION);
  return true;
}

/**
 * Get the number of command, (space delimited).
 * @see __super::getCommandCount
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us. 
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return bool success or not
 */
bool PowershellApi::GetCommandCount(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount > 0)
  {
    auto errorMsg = _T("<b>Error : </b> The 'GetCommandCount' function does not take any arguments.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  // get it
  size_t nSize = __super::GetCommandCount();

  //  add it to the response
  ipcResponse.Add( nSize);

  // success.
  return true;

}

/**
 * Get a command by index, #0 is the script been run.
 * @see __super::getCommand
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return boolean success or not
 */
bool PowershellApi::GetCommand(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  const auto ARGUMENT_NUMBER = 0;
  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount != 1 || !ipcRequest.IsInt(ARGUMENT_NUMBER))
  {
    auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>GetCommand( <b>index</b> )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  unsigned int idx = ipcRequest.Get<unsigned int>(ARGUMENT_NUMBER);
  MYODD_STRING sValue;
  if (!__super::GetCommand(idx, sValue))
  {
    auto errorMsg = _T("Trying to get an argument past the number of arguments.");
    __super::Log(AM_LOG_WARNING, errorMsg);

    // return that there was an error.
    ipcResponse.Add(0);
    return true;
  }

  //  return the string that we found.
  ipcResponse.Add(sValue);
  return true;
}

/**
 * Get the command that was called, (usefull in case we have multiple registered commands by scripts).
 * @see __super::getAction
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return boolean success or not
 */
bool PowershellApi::GetAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount != 0)
  {
    auto errorMsg = _T("<b>Error : </b>.<br>Format is <i>GetAction( )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  MYODD_STRING sValue;
  if (!__super::GetAction(sValue))
  {
    auto errorMsg = _T("Trying to get the action name/value.");
    __super::Log(AM_LOG_WARNING, errorMsg);

    // return that there was an error.
    ipcResponse.Add(0);
    return true;
  }

  // push the value
  ipcResponse.Add( sValue );

  // success
  return true;
}

/**
 * Get the currently selected string, (if any).
 * @see __super::getString
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return boolean success or not
 */
bool PowershellApi::GetString(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  const auto ARGUMENT_QUOTE = 0;
  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount > 1)
  {
    auto errorMsg = _T("<b>Error : </b> The function 'Getstring' does not take more than one parameter");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  // the quote
  bool bQuote = true;
  if (argumentCount == 1)
  {
    if (!ipcRequest.IsInt( ARGUMENT_QUOTE ))
    {
      auto errorMsg = _T("The first parameter must be a boolean.");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      return false;
    }
    bQuote = (1 == ipcRequest.Get<int>(ARGUMENT_QUOTE));
  }

  MYODD_STRING sValue = _T("");
  if (!__super::GetString(sValue, bQuote))
  {
    __super::Log(AM_LOG_WARNING, _T("Could not get any selected string."));

    // return that there was an error.
    ipcResponse.Add(0);
    return true;
  }

  // otherwise push the string
  ipcResponse.Add( sValue );

  // we have one item
  return true;
}

/**
 * Get a currently selected file, return false if we don't have that index.
 * @see __super::getFile
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return boolean success or not
 */
bool PowershellApi::GetFile(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  //  the arguments.
  const auto ARGUMENT_NUMBER = 0;
  const auto ARGUMENT_QUOTE = 1;

  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount < 1 || argumentCount > 2)
  {
    auto errorMsg = _T("<b>Error : </b> The function 'Getfile' takes 1 or 2 parameters");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsInt(ARGUMENT_NUMBER))
  {
    auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>am_getfile( <b>index</b>[, quote])</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  //  get the nunber
  auto idx = ipcRequest.Get<unsigned int>( ARGUMENT_NUMBER );

  // the quote
  bool bQuote = true;
  if (argumentCount == 2)
  {
    if (!ipcRequest.IsInt( ARGUMENT_QUOTE))
    {
      auto errorMsg = _T("<b>Error : </b> The second parameter must be a boolean true|false");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      return false;
    }
    bQuote = (1 == ipcRequest.Get<signed int>(ARGUMENT_QUOTE));
  }

  MYODD_STRING sValue = _T("");
  if (!__super::GetFile(idx, sValue, bQuote))
  {
    __super::Log(AM_LOG_WARNING, _T("Unable to get the requested file index."));

    ipcResponse.Add(0);
    return true;
  }

  // otherwise push the string
  ipcResponse.Add( sValue );

  // we have one item
  return true;
}

/**
 * Get a currently selected folder, return false if we don't have that index.
 * @see __super::getFolder
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return boolean success or not
 */
bool PowershellApi::GetFolder(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  const auto ARGUMENT_NUMBER = 0;
  const auto ARGUMENT_QUOTE = 1;

  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount < 1 || argumentCount > 2)
  {
    auto errorMsg = _T("<b>Error : </b> The function 'Getfolder' takes 1 or 2 parameters");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsInt(ARGUMENT_NUMBER))
  {
    auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>am_getfolder( <b>index</b>[, quote] )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  //  the number.
  auto idx = ipcRequest.Get<unsigned int>(ARGUMENT_NUMBER);

  // the quote
  auto bQuote = true;
  if (argumentCount >= 2)
  {
    if (!ipcRequest.IsInt(ARGUMENT_QUOTE))
    {
      auto errorMsg = _T("<b>Error : </b> The second parameter must be a boolean true|false");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      return false;
    }
    bQuote = (1 == ipcRequest.Get<unsigned int>(ARGUMENT_QUOTE));
  }

  MYODD_STRING sValue = _T("");
  if (!__super::GetFolder(idx, sValue, bQuote))
  {
    __super::Log(AM_LOG_WARNING, _T("Unable to get the requested folder index."));

    //  just return false.
    ipcResponse.Add(0);
    return true;
  }

  // otherwise push the string
  ipcResponse.Add(sValue);

  // we have one item
  return true;
}

/**
 * Get a currently selected folder, return false if we don't have that index.
 * @see __super::getURL
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return boolean success or not
 */
bool PowershellApi::GetUrl(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  const auto ARGUMENT_NUMBER = 0;
  const auto ARGUMENT_QUOTE = 1;

  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount < 1 || argumentCount > 2)
  {
    auto errorMsg = _T("<b>Error : </b> The function 'Geturl' takes 1 or 2 parameters");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsInt(ARGUMENT_NUMBER))
  {
    auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>am_geturl( <b>index</b> )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  //  get the index.
  auto idx = ipcRequest.Get<unsigned int>( ARGUMENT_NUMBER );

  // the quote
  auto bQuote = true;
  if (argumentCount >= 2)
  {
    if (!ipcRequest.IsInt(ARGUMENT_QUOTE))
    {
      auto errorMsg = _T("<b>Error : </b> The second parameter must be a boolean true|false");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      return false;
    }
    bQuote = (1 == ipcRequest.Get<unsigned int>(ARGUMENT_QUOTE));
  }

  MYODD_STRING sValue = _T("");
  if (!__super::GetURL(idx, sValue, bQuote))
  {
    __super::Log(AM_LOG_WARNING, _T("Unable to get the requested url index."));

    //  just return false.
    ipcResponse.Add(0);
    return true;
  }

  // otherwise push the string
  ipcResponse.Add(sValue);

  // we have one item
  return true;
}

/**
 * Get the action monitor version string.
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return boolean success or not
 */
bool PowershellApi::GetVersion(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount > 0)
  {
    auto errorMsg = _T("<b>Error : </b> The function 'GetVersion' does not take any parameters");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  MYODD_STRING sValue = _T("");
  if (!__super::GetVersion(sValue))
  {
    __super::Log(AM_LOG_ERROR, _T("Unable to get the version number"));

    //could not get it.
    return false;
  }

  // otherwise push the string
  ipcResponse.Add(sValue);

  // we have one item
  return true;
}

/**
 * Log a message.
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return bool success or not.
 */
bool PowershellApi::Log(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  const auto ARGUMENT_LOGTYPE = 0;
  const auto ARGUMENT_MESSAGE = 1;

  //  get the number of arguments
  auto argumentCount = ipcRequest.GetNumArguments();

  // we must have 2 items
  if (argumentCount != 2)
  {
    auto errorMsg = _T("<b>Error : </b> Missing values.<br>Format is <i>am_Log( <b>logType</b>, <b>string</b> )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsInt(ARGUMENT_LOGTYPE))
  {
    auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'logtype' must be a number");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsString(ARGUMENT_MESSAGE))
  {
    auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'message' must be a string");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    return false;
  }

  auto logType = ipcRequest.Get<unsigned int>( ARGUMENT_LOGTYPE );
  auto action = ipcRequest.Get<std::wstring>( ARGUMENT_MESSAGE );

  __super::Log(logType, action.c_str());

  // success.
  ipcResponse.Add(1);
  return true;
}
