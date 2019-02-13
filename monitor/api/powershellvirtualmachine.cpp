#include "stdafx.h"

#ifdef ACTIONMONITOR_PS_PLUGIN
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <os/ipclistener.h>
#include <ActionMonitorDlg.h>
#include <ActionMonitor.h>

#include "powershellvirtualmachine.h"

PowershellVirtualMachine::PowershellVirtualMachine() : 
  _initialized( false )
{
}

PowershellVirtualMachine::~PowershellVirtualMachine()
{
  if (_initialized)
  {
    const auto pThis = static_cast<CActionMonitorDlg*>(App().GetMainWnd());
    if (pThis)
    {
      pThis->RemoveMessageHandler(*this);
      _initialized = false;
    }
  }

  //  Remove all the apis
  RemoveApis();
}

// create the IPC server
void PowershellVirtualMachine::Initialize()
{
  // make sure that we have ourselves as a listener
  const auto pThis = dynamic_cast<CActionMonitorDlg*>(App().GetMainWnd());
  if( pThis )
  {
    pThis->AddMessageHandler(*this);
    _initialized = true;
  }
}

/**
 * \brief Handle the various messages.
 * \param ipcRequest the request we received.
 * \param ipcResponse if we send a response, this will be the container.
 * \return boolean if we handled the message or not.
 */
bool PowershellVirtualMachine::HandleIpcMessage(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  //  we must have 2 arguments
  // #1 is the uiid
  // #2 is the function name
  const auto argumentCount = ipcRequest.GetNumArguments();
  if ( argumentCount < 2)
  {
    //  this is not for us
    return false;
  }

  //  lock us in to prevent the api from going away
  myodd::threads::Lock lock(_mutex);

  // get the uuid
  const auto uuid = ipcRequest.Get<std::wstring>(0);
  const auto psApi = FindApi(uuid);
  if (nullptr == psApi)
  {
    return false;
  }

  //  get the function name
  const auto functionName = ipcRequest.Get<std::wstring>(1);

  // shift the 2 arguments.
  auto shiftedRequest = ipcRequest;
  shiftedRequest.RemoveArgument(0); // remove the uuid
  shiftedRequest.RemoveArgument(0); // remove the function name

  // Say
  if (functionName == L"Say")
  {
    return psApi->Say(shiftedRequest, ipcResponse);
  }

  // Version number
  if (functionName == L"Version")
  {
    return psApi->Version(shiftedRequest, ipcResponse);
  }

  // get the app version number.
  if (functionName == L"GetVersion")
  {
    return psApi->GetVersion(shiftedRequest, ipcResponse);
  }

  // log a message
  if (functionName == L"Log")
  {
    return psApi->Log(shiftedRequest, ipcResponse);
  }

  // execute a process
  if (functionName == L"Execute")
  {
    return psApi->Execute(shiftedRequest, ipcResponse);
  }

  // get the number of commands
  if (functionName == L"GetCommandCount")
  {
    return psApi->GetCommandCount(shiftedRequest, ipcResponse);
  }

  // get a command by index.
  if (functionName == L"GetCommand")
  {
    return psApi->GetCommand(shiftedRequest, ipcResponse);
  }

  // get the selected string
  if (functionName == L"GetString")
  {
    return psApi->GetString(shiftedRequest, ipcResponse);
  }

  // get a selected file
  if (functionName == L"GetFile")
  {
    return psApi->GetFile(shiftedRequest, ipcResponse);
  }

  // get the selected folder.
  if (functionName == L"GetFolder")
  {
    return psApi->GetFolder(shiftedRequest, ipcResponse);
  }

  // get the selected folder.
  if (functionName == L"GetUrl")
  {
    return psApi->GetUrl(shiftedRequest, ipcResponse);
  }

  // Actions
  if (functionName == L"FindAction")
  {
    return psApi->FindAction(shiftedRequest, ipcResponse);
  }
  if (functionName == L"RemoveAction")
  {
    return psApi->RemoveAction(shiftedRequest, ipcResponse);
  }
  if (functionName == L"AddAction")
  {
    return psApi->AddAction(shiftedRequest, ipcResponse);
  }
  // get this action name
  if (functionName == L"GetAction")
  {
    return psApi->GetAction(shiftedRequest, ipcResponse);
  }

  // get the foreground window
  if (functionName == L"GetForegroundWindow")
  {
    return psApi->GetForegroundWindow(shiftedRequest, ipcResponse);
  }
  //  if we are here then it is an unknown function.
  return false;
}

int PowershellVirtualMachine::ExecuteInThread(LPCTSTR pluginFile, const ActiveAction& action)
{
  Initialize();

  //  create uuid and andd it to our list.
  const auto uuid = boost::lexical_cast<std::wstring>(boost::uuids::random_generator()());
  auto psApi = AddApi(uuid, action);

  //  do we have powerhsell3?
  if (!IsPowershell3Installed())
  {
    auto errorMsg = _T("Powersell 3 is not installed, so we cannot run this script!");
    psApi->Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);
    RemoveApi(uuid);
    return 0;
  }

  MYODD_STRING szPath;
  if( !Powershell3Path(szPath) )
  {
    const auto errorMsg = _T("Powersell 3 could not be found in the installed path, so we cannot run this script!");
    psApi->Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);
    RemoveApi(uuid);
    return 0;
  }

  // get the powershell dll path
  auto dllFullpath = myodd::files::GetAppPath(true);
  myodd::files::Join(dllFullpath, dllFullpath, _T("ActionMonitor.dll"));
  if (!myodd::files::FileExists(dllFullpath))
  {
    const auto errorMsg = _T("I was unable to find the AMPowerShell dll, I cannot execute this script.");
    psApi->Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);

    // did not find the command let
    RemoveApi(uuid);
    return 0;
  }

  //  prepare the arguments.
  auto arguments = GetCommandLineArguments( action, dllFullpath, pluginFile, uuid );
  
  //
  // it is very important that we do not use out locks here!
  // otherwise no other powershell window will be able to run
  // we must let everything run fine.
  //
  //  execute a script now.
  HANDLE hProcess = nullptr;
  if (psApi->Execute(szPath.c_str(), arguments.c_str(), true, &hProcess))
  {
    // it seems to have run, but according to MS it is possible that we do not get a valid process.
    if (hProcess != nullptr)
    {
      {
        //  lock us in
        myodd::threads::Lock lock(_mutex);
        psApi->SetHandle(hProcess);
      }
      
      WaitForApi(uuid);
    }
  }
  else
  {
    myodd::log::LogError(_T("I was unable to start powershell : '%s'"), arguments.c_str());
  }

  // if we are here, it worked.
  return 1;
}

/**
 * \brief create the full command line argument that will be passed to powershell
 * \param action the action we are working with
 * \param dllFullPath the full path of the dll
 * \param pluginPath the path to the plugin
 * \param uuid the unique id
 */
MYODD_STRING PowershellVirtualMachine::GetCommandLineArguments(const ActiveAction& action, const std::wstring& dllFullPath, const std::wstring& pluginPath, const std::wstring& uuid )
{
  // the object
  auto am = myodd::strings::Format(_T("$am = New-Object Am.Core \"%s\""), uuid.c_str());

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
 * \brief Add an API to our current list, we cannot add duplicates!
 * \param uuid the unique Id we are adding
 * \param action the matching action for this Id.
 * \return the powershell API that manages the action
 */
PowershellApi* PowershellVirtualMachine::AddApi(const std::wstring& uuid, const ActiveAction& action)
{
  //  lock us in
  myodd::threads::Lock lock(_mutex);

  //  does it exist already
  const auto it = _apis.find(uuid);
  if (it != _apis.end())
  {
    //  yes it does, so just return what we have.
    return it->second;
  }

  //  create the powershell api.
  const auto psApi = new PowershellApi(action);

  // add it to the array
  _apis[uuid] = psApi;

  // return the new psApi
  return psApi;
}

PowershellApi* PowershellVirtualMachine::FindApi(const std::wstring& uuid) const
{
  //  look for it
  const auto it = _apis.find(uuid);
  if (it == _apis.end())
  {
    //  does not seem to exist.
    return nullptr;
  }

  //  return the api
  return it->second;
}

/**
 * \brief Remove a single api from our list of apis.
 * \param uuid the uuid we want to remove.
 */
void PowershellVirtualMachine::RemoveApi(const std::wstring& uuid)
{
  //  lock us in
  myodd::threads::Lock lock(_mutex);

  //  look for it
  auto it = _apis.find(uuid);
  if (it == _apis.end())
  {
    //  does not seem to exist.
    return;
  }

  // delete the api
  delete it->second;

  // remove it from the map
  _apis.erase(it);
}

/**
 * Remove all the created apis.
 */
void PowershellVirtualMachine::RemoveApis()
{
  //  lock us in
  myodd::threads::Lock lock(_mutex);

  //  delete all the pointers
  for (auto it = _apis.begin(); it != _apis.end(); ++it)
  {
    try
    {
      //  delete it.
      delete it->second;
    }
    catch (const std::exception&)
    {
      //  there was a problem...
      //  but we have to carry on to delete the others.
      myodd::log::LogError(_T("There was a problem deleting a powershellApi : %s"), it->first);
    }
  }

  //  reset the api map
  _apis.clear();
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

/**
* \brief Wait for an API to complete, (or be removed).
* \param uuid the uuid that we want to wait for.
*/
void PowershellVirtualMachine::WaitForApi(const std::wstring& uuid)
{
  HANDLE hProcess = nullptr;
  for (;;)
  {
    {
      //  lock us in
      myodd::threads::Lock lock(_mutex);
      const auto api = FindApi(uuid);

      // do we have that api?
      if( api == nullptr )
      {
        // no
        return;
      }

      // get the handle.
      hProcess = api->GetHandle();

      // do we have a handle?
      if (hProcess == nullptr)
      {
        // no
        return;
      }
    }

    // wait for it to finish... a little.
    auto singleObject = WaitForSingleObject(hProcess, 1000);

    // if we didn't timeout then something else happened.
    if (WAIT_TIMEOUT != singleObject)
    {
      if (NO_ERROR != singleObject)
      {
        myodd::log::LogWarning(_T("There was an error running powershell.") );
      }
      break;
    }
  }
  CloseHandle(hProcess);

  // we can now remove this api
  RemoveApi(uuid);
}

/**
 * Destroy all the currently running scripts.
 */
void PowershellVirtualMachine::DestroyScripts()
{
  //  are we initialised?
  if( !_initialized )
  {
    return;
  }

  //  lock us in
  myodd::threads::Lock lock(_mutex);

  //  close all the handles.
  for (auto it = _apis.begin(); it != _apis.end(); ++it)
  {
    try
    {
      const auto hProcess = it->second->GetHandle();
      if( hProcess )
      {
        CloseHandle(hProcess);
        it->second->SetHandle(nullptr);
      }
    }
    catch (const std::exception&)
    {
      //  there was a problem...
      //  but we have to carry on to delete the others.
      myodd::log::LogError(_T("There was a problem deleting a powershellApi : %s"), it->first);
    }
  }

  //  we can now remove the apis.
  RemoveApis();
}
#endif /*ACTIONMONITOR_PS_PLUGIN*/