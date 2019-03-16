#include "StdAfx.h"

#include "PowershellApi.h"
#include "pluginVirtualMachine.h"

/**
 * \copydoc
 */
PowershellApi::PowershellApi(const std::wstring& uuid, const IActiveAction& action, IActions& actions, IMessagesHandler& messagesHandler ) :
  ExecuteApi(uuid, action, actions, messagesHandler )
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
bool PowershellApi::Execute(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
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
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsString(ARGUMENT_MODULE) || !ipcRequest.IsString(ARGUMENT_ARGS))
  {
    auto errorMsg = _T("<b>Error : </b> The first and second parameters must be strings.");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  bool isPrivileged = false;
  if (argumentCount == 3)
  {
    if (!ipcRequest.IsInt(ARGUMENT_PRIVILEGED))
    {
      auto errorMsg = _T("<b>Error : </b> The third argument, (privileged), can only be true|false");
      HelperApi::Log(AM_LOG_ERROR, errorMsg);
      HelperApi::Say(errorMsg, 3000, 5);
      return false;
    }
    isPrivileged = (ipcRequest.Get<unsigned int>(ARGUMENT_PRIVILEGED) == 1);
  }

  auto module = ipcRequest.Get<std::wstring>(ARGUMENT_MODULE);
  auto cmdLine = ipcRequest.Get<std::wstring>( ARGUMENT_ARGS);

  // run the query
  const auto result = HelperApi::Execute(module.c_str(), cmdLine.c_str(), isPrivileged, nullptr);

  // push the result.
  ipcResponse.Add( result ? 1 : 0);

  // tell the user it did not work
  if (false == result)
  {
    auto errorMsg = _T("<b>Error : </b> There was an error executing the request, please check the parameters.");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
  }

  // return the number of results
  return true;
}

/**
 * Todo
 * @see __super::addAction
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return bool success or not.
 */
bool PowershellApi::AddAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto ARGUMENT_ACTION = 0;
  const auto ARGUMENT_PATH = 1;

  //  get the number of arguments
  auto argumentCount = ipcRequest.GetNumArguments();

  // we must have 2 items
  if (argumentCount != 2)
  {
    auto errorMsg = _T("<b>Error : </b> Missing values.<br>Format is <i>am_addAction( <b>action</b>, <b>path</b> )</i>");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsString(ARGUMENT_ACTION))
  {
    auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'action' must be a string");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsString(ARGUMENT_PATH))
  {
    auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'path' must be a string");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  // get the action and path
  auto szAction = ipcRequest.Get<std::wstring>( ARGUMENT_ACTION);
  auto szPath = ipcRequest.Get<std::wstring>( ARGUMENT_PATH);

  // add it, (or try).
  const auto result = HelperApi::AddAction(szAction.c_str(), szPath.c_str());

  // return if it works.
  ipcResponse.Add(result ? 1 : 0);

  // success
  return true;
}

/**
 * Todo
 * @see __super::RemoveAction
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return bool success or not.
 */
bool PowershellApi::RemoveAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto ARGUMENT_ACTION = 0;
  const auto ARGUMENT_PATH = 1;

  //  get the number of arguments
  auto argumentCount = ipcRequest.GetNumArguments();

  // we must have 1 or 2
  if (argumentCount != 2)
  {
    auto errorMsg = _T("<b>Error : </b> Missing values.<br>Format is <i>am_removeAction( <b>action</b>, <b>path</b> )</i>");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsString ( ARGUMENT_ACTION ))
  {
    auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'action' must be a string");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsString( ARGUMENT_PATH))
  {
    auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'path' must be a string");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  // get the action and path
  auto szAction = ipcRequest.Get<std::wstring>( ARGUMENT_ACTION );
  auto szPath = ipcRequest.Get<std::wstring>( ARGUMENT_PATH);

  // remove it, (or try).
  const auto result = HelperApi::RemoveAction(szAction.c_str(), szPath.c_str());

  // return if it works.
  ipcResponse.Add(result ? 1 : 0);

  // success
  return true;
}

/**
 * Find an action.
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return bool success or not.
 */
bool PowershellApi::FindAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto ARGUMENT_INDEX = 0;
  const auto ARGUMENT_ACTION = 1;

  //  get the number of arguments
  auto argumentCount = ipcRequest.GetNumArguments();

  // we must have 2 arguments.
  if (argumentCount != 2)
  {
    auto errorMsg = _T("<b>Error : </b> Missing values.<br>Format is <i>am_findAction( <b>index</b>, <b>action</b> )</i>");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsInt(ARGUMENT_INDEX))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'index' must be a number");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsString(ARGUMENT_ACTION))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'action' must be a string");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  auto idx = ipcRequest.Get<unsigned int>(ARGUMENT_INDEX);
  auto action = ipcRequest.Get<std::wstring>(ARGUMENT_ACTION);

  MYODD_STRING sValue = _T("");
  if (!HelperApi::FindAction(idx, action.c_str(), sValue))
  {
    HelperApi::Log(AM_LOG_WARNING, _T("Could not find action at given index"));

    //  just return false.
    ipcResponse.Add(0);

    // one return variable.
    return false;
  }

  // otherwise push the string
  ipcResponse.Add(sValue);

  // one return variable.
  return true;
}

/**
 * Get the last foreground window. This is the window that was last on top.
 * It is posible to return NULL if the window is not
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return bool success or not.
 */
bool PowershellApi::GetForegroundWindow(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount > 0)
  {
    const auto errorMsg = _T("<b>Error : </b> The 'GetForegroundWindow' function does not take any arguments.");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  // get the foreground window.
  const auto hwnd = HelperApi::GetForegroundWindow();

  // return it as an int.
  ipcResponse.Add(reinterpret_cast<int64_t>(hwnd));

  //  done 
  return true;
}

/**
 * Output a message on the screen.
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return bool success or not.
 */
bool PowershellApi::Say(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto ARGUMENT_TEXT = 0;
  const auto ARGUMENT_ELAPSE = 1;
  const auto ARGUMENT_FADEOUT = 2;

  const auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount < 2 || argumentCount > 3)
  {
    const auto errorMsg = _T("<b>Error : </b> Missing <i>Elapse</i> time.<br>Format is <i>Say( msg, <b>elapse</b>[, fade=0])</i>");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsString(ARGUMENT_TEXT))
  {
    const auto errorMsg = _T("<b>Error : </b> The first argument must be a string.");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsInt( ARGUMENT_ELAPSE))
  {
    const auto errorMsg = _T("<b>Error : </b> The elapse parameters must be an integer.");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  auto message = ipcRequest.Get<std::wstring>(ARGUMENT_TEXT);
  auto elapse = ipcRequest.Get<unsigned int>(ARGUMENT_ELAPSE);
  if (elapse == 0)
  {
    const auto errorMsg = _T("<b>Error : </b> Missing <i>Elapse</i> time.<br>Format is <i>Say( msg, <b>elapse</b>[, fade=0])</i>");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  auto fade = 0;
  if (argumentCount == 3)
  {
    if (!ipcRequest.IsInt(ARGUMENT_FADEOUT))
    {
      const auto errorMsg = _T("<b>Error : </b> The fadout parameters must be an integer.");
      HelperApi::Log(AM_LOG_ERROR, errorMsg);
      HelperApi::Say(errorMsg, 3000, 5);
      return false;
    }
    fade = ipcRequest.Get<unsigned int>(ARGUMENT_FADEOUT);
  }

  // add the boolean response.
  const auto b = HelperApi::Say(message.c_str(), elapse, fade);
  ipcResponse.Add( b );
  return b;
}

/**
 * Get the powerhsell api version number.
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return bool success or not.
 */
bool PowershellApi::Version(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount > 0 )
  {
    auto errorMsg = _T("<b>Error : </b> The 'Version' function does not take any arguments.");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
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
bool PowershellApi::GetCommandCount(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount > 0)
  {
    const auto errorMsg = _T("<b>Error : </b> The 'GetCommandCount' function does not take any arguments.");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  // get it
  const auto nSize = HelperApi::GetCommandCount();

  //  add it to the response
  ipcResponse.Add( static_cast<int64_t>(nSize) );

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
bool PowershellApi::GetCommand(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto ARGUMENT_NUMBER = 0;
  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount != 1 || !ipcRequest.IsInt(ARGUMENT_NUMBER))
  {
    auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>GetCommand( <b>index</b> )</i>");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  const auto idx = ipcRequest.Get<unsigned int>(ARGUMENT_NUMBER);
  std::wstring sValue;
  if (!HelperApi::GetCommand(idx, sValue))
  {
    const auto errorMsg = _T("Trying to get an argument past the number of arguments.");
    HelperApi::Log(AM_LOG_WARNING, errorMsg);

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
bool PowershellApi::GetAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount != 0)
  {
    const auto errorMsg = _T("<b>Error : </b>.<br>Format is <i>GetAction( )</i>");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  std::wstring sValue;
  if (!HelperApi::GetAction(sValue))
  {
    const auto errorMsg = _T("Trying to get the action name/value.");
    HelperApi::Log(AM_LOG_WARNING, errorMsg);

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
bool PowershellApi::GetString(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto ARGUMENT_QUOTE = 0;
  const auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount > 1)
  {
    const auto errorMsg = _T("<b>Error : </b> The function 'Getstring' does not take more than one parameter");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  // the quote
  auto bQuote = true;
  if (argumentCount == 1)
  {
    if (!ipcRequest.IsInt( ARGUMENT_QUOTE ))
    {
      const auto errorMsg = _T("The first parameter must be a boolean.");
      HelperApi::Log(AM_LOG_ERROR, errorMsg);
      HelperApi::Say(errorMsg, 3000, 5);
      return false;
    }
    bQuote = (1 == ipcRequest.Get<int>(ARGUMENT_QUOTE));
  }

  std::wstring sValue = L"";
  if (!HelperApi::GetString(sValue, bQuote))
  {
    HelperApi::Log(AM_LOG_WARNING, _T("Could not get any selected string."));

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
bool PowershellApi::GetFile(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  //  the arguments.
  const auto ARGUMENT_NUMBER = 0;
  const auto ARGUMENT_QUOTE = 1;

  const auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount < 1 || argumentCount > 2)
  {
    const auto errorMsg = _T("<b>Error : </b> The function 'Getfile' takes 1 or 2 parameters");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsInt(ARGUMENT_NUMBER))
  {
    const auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>am_getfile( <b>index</b>[, quote])</i>");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  //  get the nunber
  auto idx = ipcRequest.Get<unsigned int>( ARGUMENT_NUMBER );

  // the quote
  auto bQuote = true;
  if (argumentCount == 2)
  {
    if (!ipcRequest.IsInt( ARGUMENT_QUOTE))
    {
      const auto errorMsg = _T("<b>Error : </b> The second parameter must be a boolean true|false");
      HelperApi::Log(AM_LOG_ERROR, errorMsg);
      HelperApi::Say(errorMsg, 3000, 5);
      return false;
    }
    bQuote = (1 == ipcRequest.Get<signed int>(ARGUMENT_QUOTE));
  }

  std::wstring sValue = L"";
  if (!HelperApi::GetFile(idx, sValue, bQuote))
  {
    HelperApi::Log(AM_LOG_WARNING, _T("Unable to get the requested file index."));

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
bool PowershellApi::GetFolder(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto ARGUMENT_NUMBER = 0;
  const auto ARGUMENT_QUOTE = 1;

  const auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount < 1 || argumentCount > 2)
  {
    const auto errorMsg = _T("<b>Error : </b> The function 'Getfolder' takes 1 or 2 parameters");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsInt(ARGUMENT_NUMBER))
  {
    const auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>am_getfolder( <b>index</b>[, quote] )</i>");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  //  the number.
  const auto idx = ipcRequest.Get<unsigned int>(ARGUMENT_NUMBER);

  // the quote
  auto bQuote = true;
  if (argumentCount >= 2)
  {
    if (!ipcRequest.IsInt(ARGUMENT_QUOTE))
    {
      const auto errorMsg = _T("<b>Error : </b> The second parameter must be a boolean true|false");
      HelperApi::Log(AM_LOG_ERROR, errorMsg);
      HelperApi::Say(errorMsg, 3000, 5);
      return false;
    }
    bQuote = (1 == ipcRequest.Get<unsigned int>(ARGUMENT_QUOTE));
  }

  std::wstring sValue = L"";
  if (!HelperApi::GetFolder(idx, sValue, bQuote))
  {
    HelperApi::Log(AM_LOG_WARNING, _T("Unable to get the requested folder index."));

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
 * @see __super::getUrl
 * @param const myodd::os::IpcData& ipcRequest the request as was passed to us.
 * @param myodd::os::IpcData& ipcResponse the container that will have the response.
 * @return boolean success or not
 */
bool PowershellApi::GetUrl(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto ARGUMENT_NUMBER = 0;
  const auto ARGUMENT_QUOTE = 1;

  const auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount < 1 || argumentCount > 2)
  {
    const auto errorMsg = _T("<b>Error : </b> The function 'Geturl' takes 1 or 2 parameters");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsInt(ARGUMENT_NUMBER))
  {
    const auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>am_geturl( <b>index</b> )</i>");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
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
      const auto errorMsg = _T("<b>Error : </b> The second parameter must be a boolean true|false");
      HelperApi::Log(AM_LOG_ERROR, errorMsg);
      HelperApi::Say(errorMsg, 3000, 5);
      return false;
    }
    bQuote = (1 == ipcRequest.Get<unsigned int>(ARGUMENT_QUOTE));
  }

  std::wstring sValue = L"";
  if (!HelperApi::GetUrl(idx, sValue, bQuote))
  {
    HelperApi::Log(AM_LOG_WARNING, _T("Unable to get the requested url index."));

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
bool PowershellApi::GetVersion(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto argumentCount = ipcRequest.GetNumArguments();
  if (argumentCount > 0)
  {
    auto errorMsg = _T("<b>Error : </b> The function 'GetVersion' does not take any parameters");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  std::wstring sValue = L"";
  if (!HelperApi::GetVersion(sValue))
  {
    HelperApi::Log(AM_LOG_ERROR, _T("Unable to get the version number"));

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
bool PowershellApi::Log(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const
{
  const auto ARGUMENT_LOGTYPE = 0;
  const auto ARGUMENT_MESSAGE = 1;

  //  get the number of arguments
  auto argumentCount = ipcRequest.GetNumArguments();

  // we must have 2 items
  if (argumentCount != 2)
  {
    auto errorMsg = _T("<b>Error : </b> Missing values.<br>Format is <i>am_Log( <b>logType</b>, <b>string</b> )</i>");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsInt(ARGUMENT_LOGTYPE))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'logtype' must be a number");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  if (!ipcRequest.IsString(ARGUMENT_MESSAGE))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'message' must be a string");
    HelperApi::Log(AM_LOG_ERROR, errorMsg);
    HelperApi::Say(errorMsg, 3000, 5);
    return false;
  }

  auto logType = ipcRequest.Get<unsigned int>( ARGUMENT_LOGTYPE );
  auto action = ipcRequest.Get<std::wstring>( ARGUMENT_MESSAGE );

  HelperApi::Log(logType, action.c_str());

  // success.
  ipcResponse.Add(1);
  return true;
}
