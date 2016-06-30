#include "stdafx.h"

#ifdef ACTIONMONITOR_PS_PLUGIN
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <os/ipclistener.h>
#include <ActionMonitorDlg.h>
#include <ActionMonitor.h>

#include "powershellvirtualmachine.h"

PowershellVirtualMachine::PowershellVirtualMachine()
{
}

PowershellVirtualMachine::~PowershellVirtualMachine()
{
  //  Remove all the apis
  RemoveApis();
}

// create the IPC server
void PowershellVirtualMachine::Initialize()
{
  // make sure that we have ourselves as a listener
  auto pThis = static_cast<CActionMonitorDlg*>(App().GetMainWnd());
  if( pThis )
  {
    pThis->AddMessageHandler(*this);
  }
}

/**
 * Handle the various messages.
 * @param const myodd::os::IpcData& ipcRequest the request we received.
 * @param myodd::os::IpcData& ipcResponse if we send a response, this will be the container.
 * @return boolean if we handled the message or not.
 */
bool PowershellVirtualMachine::HandleIpcMessage(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  ipcResponse.Add(37);
  ipcResponse.Add(L"Hello");
  ipcResponse.Add(L"");  // empty L string
  ipcResponse.Add("Hello");
  ipcResponse.Add("");  // empty string

  return true;
}

int PowershellVirtualMachine::ExecuteInThread(LPCTSTR pluginFile, const ActiveAction& action)
{
  Initialize();

  //  create uuid and andd it to our list.
  std::wstring uuid = boost::lexical_cast<std::wstring>(boost::uuids::random_generator()());
  auto psApi = AddApi(uuid, action);

  //  execute a script now.
  MYODD_STRING arguments = myodd::strings::Format(_T("-command \"&{$policy = Get-ExecutionPolicy; Set-ExecutionPolicy RemoteSigned -Force; . '%s' ;Set-ExecutionPolicy $policy -Force; }"), pluginFile);

  //
  // it is very important that we do not use out locks here!
  // otherwise no other powershell window will be able to run
  // we must let everything run fine.
  HANDLE hProcess = nullptr;
  if (psApi->Execute(_T("powershell.exe"), arguments.c_str(), true, &hProcess))
  {
    // it seems to have run, but according to MS it is possible that we do not get a valid process.
    if (hProcess != nullptr)
    {
      for (;;)
      {
        // wait for it to finish... a little.
        auto singleObject = WaitForSingleObject(hProcess, 1000);

        // if we didn't timeout then something else happened.
        if (WAIT_TIMEOUT != singleObject)
        {
          if (NO_ERROR != singleObject)
          {
            myodd::log::LogWarning(_T("There was an error running powershell : '%s'"), arguments.c_str());
          }
          break;
        }
      }
      CloseHandle(hProcess);
    }
  }
  else
  {
    myodd::log::LogError(_T("I was unable to start powershell : '%s'"), arguments.c_str());
  }
  return 0;
}

bool PowershellVirtualMachine::IsPluginExt(LPCTSTR ext)
{
  return (_tcsicmp(ext, _T("ps1")) == 0);
}

PowershellApi* PowershellVirtualMachine::AddApi(const std::wstring& uuid, const ActiveAction& action)
{
  //  lock us in
  myodd::threads::Lock lock(_mutex);

  //  does it exist already
  auto it = _apis.find(uuid);
  if (it != _apis.end())
  {
    //  yes it does, so just return what we have.
    return it->second;
  }

  //  create the powershell api.
  auto psApi = new PowershellApi(action);

  // add it to the array
  _apis[uuid] = psApi;

  // return the new psApi
  return psApi;
}

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
#endif /*ACTIONMONITOR_PS_PLUGIN*/