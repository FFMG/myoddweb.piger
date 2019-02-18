#include "stdafx.h"

#ifdef ACTIONMONITOR_S_PLUGIN
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <os/ipclistener.h>
#include <ActionMonitorDlg.h>
#include <ActionMonitor.h>

#include "ShellVirtualMachine.h"

ShellVirtualMachine::ShellVirtualMachine() : 
  _initialized( false )
{
}

ShellVirtualMachine::~ShellVirtualMachine()
{
  if (_initialized)
  {
    const auto pThis = static_cast<ActionMonitorDlg*>(App().GetMainWnd());
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
void ShellVirtualMachine::Initialize()
{
  // make sure that we have ourselves as a listener
  const auto pThis = dynamic_cast<ActionMonitorDlg*>(App().GetMainWnd());
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
bool ShellVirtualMachine::HandleIpcMessage(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
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

int ShellVirtualMachine::ExecuteInThread(const LPCTSTR pluginFile, const ActiveAction& action)
{
  Initialize();

  //  create uuid and andd it to our list.
  const auto uuid = boost::lexical_cast<std::wstring>(boost::uuids::random_generator()());
  const auto psApi = AddApi(uuid, action);

  // get the path of the exe
  std::wstring szPath;
  if( !ShellPath(szPath) )
  {
    const auto errorMsg = L"Action Monitor Shell could not be found in the installed path, so we cannot run this script!";
    psApi->Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);
    RemoveApi(uuid);
    return 0;
  }

  // get the action monitor dll path
  auto dllFullpath = myodd::files::GetAppPath(true);
  myodd::files::Join(dllFullpath, dllFullpath, L"ActionMonitor.dll");
  if (!myodd::files::FileExists(dllFullpath))
  {
    const auto errorMsg = L"I was unable to find the ActionMonitor dll, I cannot execute this script.";
    psApi->Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);

    // did not find the command let
    RemoveApi(uuid);
    return 0;
  }

  auto dllInterfacesFullpath = myodd::files::GetAppPath(true);
  myodd::files::Join(dllInterfacesFullpath, dllInterfacesFullpath, L"ActionMonitor.Interfaces.dll");
  if (!myodd::files::FileExists(dllInterfacesFullpath))
  {
    const auto errorMsg = L"I was unable to find the ActionMonitor.Interfaces dll, I cannot execute this script.";
    psApi->Say(errorMsg, 3000, 5);
    myodd::log::LogError(errorMsg);

    // did not find the command let
    RemoveApi(uuid);
    return 0;
  }

  //  prepare the arguments.
  auto arguments = GetCommandLineArguments( action, pluginFile, uuid );
  
  //
  // it is very important that we do not use our locks here!
  // otherwise no other Shell window will be able to run
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

      // save the api so we know it is running.
      WaitForApi(uuid);
    }
  }
  else
  {
    myodd::log::LogError(L"I was unable to start Action Monitor with the arguments : '%s'"), arguments.c_str();
  }

  // if we are here, it worked.
  return 1;
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
 * \brief Add an API to our current list, we cannot add duplicates!
 * \param uuid the unique Id we are adding
 * \param action the matching action for this Id.
 * \return the shell API that manages the action
 */
ShellApi* ShellVirtualMachine::AddApi(const std::wstring& uuid, const ActiveAction& action)
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
  const auto psApi = new ShellApi(action);

  // add it to the array
  _apis[uuid] = psApi;

  // return the new psApi
  return psApi;
}

ShellApi* ShellVirtualMachine::FindApi(const std::wstring& uuid) const
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
void ShellVirtualMachine::RemoveApi(const std::wstring& uuid)
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
void ShellVirtualMachine::RemoveApis()
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
      myodd::log::LogError(L"There was a problem deleting a ShellApi : %s", it->first);
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

/**
* \brief Wait for an API to complete, (or be removed).
* \param uuid the uuid that we want to wait for.
*/
void ShellVirtualMachine::WaitForApi(const std::wstring& uuid)
{
  HANDLE hProcess;
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
    const auto singleObject = WaitForSingleObject(hProcess, 1000);

    // if we didn't timeout then something else happened.
    if (WAIT_TIMEOUT != singleObject)
    {
      if (NO_ERROR != singleObject)
      {
        myodd::log::LogWarning( L"There was an error running the Shell application." );
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
void ShellVirtualMachine::DestroyScripts()
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
      myodd::log::LogError(L"There was a problem deleting a powershellApi : %s", it->first);
    }
  }

  //  we can now remove the apis.
  RemoveApis();
}
#endif /*ACTIONMONITOR_S_PLUGIN*/