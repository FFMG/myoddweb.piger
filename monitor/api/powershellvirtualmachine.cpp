#include "stdafx.h"

#ifdef ACTIONMONITOR_PS_PLUGIN
#include "powershellvirtualmachine.h"

/**
 * \copydoc
 */
PowershellVirtualMachine::PowershellVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener) :
  ExecuteVirtualMachine( actions, messagesHandler, iIpcListener )
{
}

PowershellVirtualMachine::~PowershellVirtualMachine()
{
}

bool PowershellVirtualMachine::HandleIpcMessage(ExecuteApi& api, const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
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

bool PowershellVirtualMachine::Execute(ExecuteApi& api, const ActiveAction& action, const std::wstring& pluginFile)
{
  //  do we have powerhsell3?
  if (!IsPowershell3Installed())
  {
    const auto errorMsg = _T("Powersell 3 is not installed, so we cannot run this script!");
    api.Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);
    return false;
  }

  std::wstring szPath;
  if( !Powershell3Path(szPath) )
  {
    const auto errorMsg = L"Powersell 3 could not be found in the installed path, so we cannot run this script!";
    api.Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);
    return false;
  }

  // get the powershell dll path
  auto dllFullpath = myodd::files::GetAppPath(true);
  myodd::files::Join(dllFullpath, dllFullpath, _T("ActionMonitor.dll"));
  if (!myodd::files::FileExists(dllFullpath))
  {
    const auto errorMsg = _T("I was unable to find the ActionMonitor dll, I cannot execute this script.");
    api.Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);
    return false;
  }

  auto dllInterfacesFullpath = myodd::files::GetAppPath(true);
  myodd::files::Join(dllInterfacesFullpath, dllInterfacesFullpath, _T("ActionMonitor.Interfaces.dll"));
  if (!myodd::files::FileExists(dllInterfacesFullpath))
  {
    const auto errorMsg = _T("I was unable to find the ActionMonitor.Interfaces dll, I cannot execute this script.");
    api.Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);
    return false;
  }

  //  prepare the arguments.
  auto arguments = GetCommandLineArguments( action, dllFullpath, dllInterfacesFullpath, pluginFile, api.GetUniqeId() );
  
  //
  // it is very important that we do not use out locks here!
  // otherwise no other powershell window will be able to run
  // we must let everything run fine.
  //
  //  execute a script now.
  HANDLE hProcess = nullptr;
  if (!api.Execute(szPath.c_str(), arguments.c_str(), true, &hProcess))
  {
    myodd::log::LogError(_T("I was unable to start powershell : '%s'"), arguments.c_str());
    return false;
  }

  // it seems to have run, but according to MS it is possible that we do not get a valid process.
  if (hProcess == nullptr)
  {
    return false;
  }
  api.SetHandle(hProcess);

  // if we are here, it worked.
  return true;
}

/**
 * \brief create the full command line argument that will be passed to powershell
 * \param action the action we are working with
 * \param dllFullPath the full path of the dll
 * \param dllInterfaceFullPath the interface full path.
 * \param pluginPath the path to the plugin
 * \param uuid the unique id
 */
MYODD_STRING PowershellVirtualMachine::GetCommandLineArguments(
  const ActiveAction& action,
  const std::wstring& dllFullPath,
  const std::wstring& dllInterfaceFullPath,
  const std::wstring& pluginPath,
  const std::wstring& uuid
) const
{
  // the object
  auto am = myodd::strings::Format(_T("$am = New-Object ActionMonitor.Core \"%s\""), uuid.c_str());

  //  prepare the arguments.
  return myodd::strings::Format(_T("-command \"&{$policy = Get-ExecutionPolicy; Set-ExecutionPolicy RemoteSigned -Force; Add-Type -Path '%s'; %s; . '%s' ;Set-ExecutionPolicy $policy -Force; }"), 
    dllFullPath.c_str(),
    am.c_str(), 
    pluginPath.c_str()
  );
}

/**
 * \brief Check if a given file extension is used by this API or not.
 * \param file the file we are checking
 * \return bool true|false if the given extension is powershell or not.
 */
bool PowershellVirtualMachine::IsExt(const MYODD_STRING& file)
{
  return myodd::files::IsExtension(file, _T("ps1"));
}

/**
 * \brief Get the path of powershell 3
 * \param szPath the path, if we find it.
 * \return boolean if we managed to locate the exe
 */
bool PowershellVirtualMachine::Powershell3Path(MYODD_STRING& szPath)
{
  // does it even exist?
  if (!IsPowershell3Installed())
  {
    myodd::log::LogWarning(_T("Could not get PS3 path as it is not even installed!"));
    szPath = _T("");
    return false;
  }

  //  the return path.
  MYODD_STRING szRegPath = _T("");
  if (!myodd::reg::LoadStringFullPath(_T("SOFTWARE\\Microsoft\\PowerShell\\3\\PowerShellEngine"), _T("ApplicationBase"), szRegPath, HKEY_LOCAL_MACHINE))
  {
    // this does not make sense.
    myodd::log::LogError(_T("Could not get PS3 path, while it looks like it is installed, the path value could not be found."));

    // the value does not even exist.
    szPath = _T("");
    return false;
  }

  // we now need to add powershell.exe
  myodd::files::Join(szPath, szRegPath, _T("powershell.exe"));
  if (!myodd::files::FileExists(szPath))
  {
    // this does not make sense.
    myodd::log::LogError(_T("Could not get PS3 path, while it looks like it is installed, the path given does not point to a valid exe."));

    // the value does not even exist.
    szPath = _T("");
    return false;
  }

  //  the path is set in the myodd::files::Join( ... ).
  return true;
}

/**
 * Static function to check if powershell is installed.
 * @see https://blogs.msdn.microsoft.com/powershell/2009/06/25/detection-logic-for-powershell-installation/
 * @return boolean if powershell 3 is installed or not.
 */
bool PowershellVirtualMachine::IsPowershell3Installed()
{
  DWORD dwValue;
  if (!myodd::reg::LoadDWORDFullPath(_T("SOFTWARE\\Microsoft\\PowerShell\\3"), _T("Install"), dwValue, HKEY_LOCAL_MACHINE))
  {
    // the value does not even exist.
    return false;
  }

  // if the value is 1 then it is installed.
  return (dwValue == 1 );
}

ExecuteApi* PowershellVirtualMachine::CreateApi(const std::wstring& uuid, const ActiveAction& action, IActions& actions, IMessagesHandler& messages)
{
  return new PowershellApi(uuid, action, actions, messages);
}
#endif /*ACTIONMONITOR_PS_PLUGIN*/