#include "stdafx.h"
#include "ShellApi.h"

#ifdef ACTIONMONITOR_S_PLUGIN
#include "ShellVirtualMachine.h"

/**
 * \copydoc
 */
ShellVirtualMachine::ShellVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener) :
  ExecuteVirtualMachine( actions, messagesHandler, iIpcListener)
{
}

ShellVirtualMachine::~ShellVirtualMachine()
{
}

bool ShellVirtualMachine::HandleIpcMessage(ExecuteApi& api, const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  //  get the function name
  const auto functionName = ipcRequest.Get<std::wstring>(1);

  // shift the 2 arguments.
  auto shiftedRequest = ipcRequest;
  shiftedRequest.RemoveArgument(0); // remove the uuid
  shiftedRequest.RemoveArgument(0); // remove the function name

  // Say
  if (functionName == L"Say")
  {
    return api.Say(shiftedRequest, ipcResponse);
  }

  // Version number
  if (functionName == L"Version")
  {
    return api.Version(shiftedRequest, ipcResponse);
  }

  // get the app version number.
  if (functionName == L"GetVersion")
  {
    return api.GetVersion(shiftedRequest, ipcResponse);
  }

  // log a message
  if (functionName == L"Log")
  {
    return api.Log(shiftedRequest, ipcResponse);
  }

  // execute a process
  if (functionName == L"Execute")
  {
    return api.Execute(shiftedRequest, ipcResponse);
  }

  // get the number of commands
  if (functionName == L"GetCommandCount")
  {
    return api.GetCommandCount(shiftedRequest, ipcResponse);
  }

  // get a command by index.
  if (functionName == L"GetCommand")
  {
    return api.GetCommand(shiftedRequest, ipcResponse);
  }

  // get the selected string
  if (functionName == L"GetString")
  {
    return api.GetString(shiftedRequest, ipcResponse);
  }

  // get a selected file
  if (functionName == L"GetFile")
  {
    return api.GetFile(shiftedRequest, ipcResponse);
  }

  // get the selected folder.
  if (functionName == L"GetFolder")
  {
    return api.GetFolder(shiftedRequest, ipcResponse);
  }

  // get the selected folder.
  if (functionName == L"GetUrl")
  {
    return api.GetUrl(shiftedRequest, ipcResponse);
  }

  // Actions
  if (functionName == L"FindAction")
  {
    return api.FindAction(shiftedRequest, ipcResponse);
  }
  if (functionName == L"RemoveAction")
  {
    return api.RemoveAction(shiftedRequest, ipcResponse);
  }
  if (functionName == L"AddAction")
  {
    return api.AddAction(shiftedRequest, ipcResponse);
  }
  // get this action name
  if (functionName == L"GetAction")
  {
    return api.GetAction(shiftedRequest, ipcResponse);
  }

  // get the foreground window
  if (functionName == L"GetForegroundWindow")
  {
    return api.GetForegroundWindow(shiftedRequest, ipcResponse);
  }
  //  if we are here then it is an unknown function.
  return false;
}

bool ShellVirtualMachine::Execute(ExecuteApi& api, const ActiveAction& action, const std::wstring& pluginFile)
{

  // get the path of the exe
  std::wstring szPath;
  if( !ShellPath(szPath) )
  {
    const auto errorMsg = L"Action Monitor Shell could not be found in the installed path, so we cannot run this script!";
    api.Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);
    return false;
  }

  // get the action monitor dll path
  auto dllFullpath = myodd::files::GetAppPath(true);
  myodd::files::Join(dllFullpath, dllFullpath, L"ActionMonitor.dll");
  if (!myodd::files::FileExists(dllFullpath))
  {
    const auto errorMsg = L"I was unable to find the ActionMonitor dll, I cannot execute this script.";
    api.Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);
    return false;
  }

  auto dllInterfacesFullpath = myodd::files::GetAppPath(true);
  myodd::files::Join(dllInterfacesFullpath, dllInterfacesFullpath, L"ActionMonitor.Interfaces.dll");
  if (!myodd::files::FileExists(dllInterfacesFullpath))
  {
    const auto errorMsg = L"I was unable to find the ActionMonitor.Interfaces dll, I cannot execute this script.";
    api.Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);
    return false;
  }

  //  prepare the arguments.
  auto arguments = GetCommandLineArguments( action, pluginFile, api.GetUniqeId() );
  
  //
  // it is very important that we do not use our locks here!
  // otherwise no other Shell window will be able to run
  // we must let everything run fine.
  //
  //  execute a script now.
  HANDLE hProcess = nullptr;
  if (!api.Execute(szPath.c_str(), arguments.c_str(), true, &hProcess))
  {
    myodd::log::LogError(L"I was unable to start Action Monitor with the arguments : '%s'"), arguments.c_str();
    return false;
  }
  api.SetHandle(hProcess);

  // if we are here, it worked.
  return true;
}

/**
 * \brief create the full command line argument that will be passed to ActionMonitor Shell
 * \param action the action we are working with
 * \param pluginPath the path to the plugin
 * \param uuid the unique id
 */
std::wstring ShellVirtualMachine::GetCommandLineArguments(
  const ActiveAction& action,
  const std::wstring& pluginPath,
  const std::wstring& uuid
) const
{
  // the uuid
  const auto uuidCommand = myodd::strings::Format(L"--uuid \"%s\"", uuid.c_str());

  // the path
  const auto pathCommand = myodd::strings::Format(L"--path \"%s\"", pluginPath.c_str());

  //  together ... and this is hidden as well.
  return myodd::strings::Format( L"%s %s --hidden", uuidCommand.c_str(), pathCommand.c_str() );
}

/**
 * \brief Check if a given file extension is used by this API or not.
 * \param file the file we are checking
 * \return bool true|false if the given extension is Action Monitor .NET or not.
 */
bool ShellVirtualMachine::IsExt(const std::wstring& file)
{
  return myodd::files::IsExtension(file, L"amp-net");
}

/**
 * \brief Get the path of powershell 3
 * \param szPath the path, if we find it.
 * \return boolean if we managed to locate the exe
 */
bool ShellVirtualMachine::ShellPath(std::wstring& szPath)
{
  //  the return path.
  std::wstring szRegPath = L"";
  if (!myodd::reg::LoadStringFullPath(_T("SOFTWARE\\Microsoft\\PowerShell\\3\\PowerShellEngine"), _T("ApplicationBase"), szRegPath, HKEY_LOCAL_MACHINE))
  {
    // this does not make sense.
    myodd::log::LogError(_T("Could not get PS3 path, while it looks like it is installed, the path value could not be found."));

    // the value does not even exist.
    szPath = L"";
    return false;
  }

  // get the app path
  const auto appPath = myodd::files::GetAppPath();

  // and then add the exe to it.
  myodd::files::Join(szPath, appPath, L"ActionMonitor.Shell.exe");

  if (!myodd::files::FileExists(szPath))
  {
    // this does not make sense.
    myodd::log::LogError( L"Could not get Shell path, while it looks like it is installed, the path given does not point to a valid exe.");

    // the value does not even exist.
    szPath = L"";
    return false;
  }

  //  the path is set in the myodd::files::Join( ... ).
  return true;
}

ExecuteApi* ShellVirtualMachine::CreateApi(const std::wstring& uuid, const ActiveAction& action, IActions& actions, IMessagesHandler& messages)
{
  return new ShellApi(uuid, action, actions, messages);
}

#endif /*ACTIONMONITOR_S_PLUGIN*/