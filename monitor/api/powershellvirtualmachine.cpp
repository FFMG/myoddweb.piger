#include "stdafx.h"
#include <os/ipclistener.h>
#include <ActionMonitorDlg.h>
#include <ActionMonitor.h>

#ifdef ACTIONMONITOR_PS_PLUGIN

#include "powershellvirtualmachine.h"

PowershellVirtualMachine::PowershellVirtualMachine()
{
}

PowershellVirtualMachine::~PowershellVirtualMachine()
{
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
  return 0;
}

bool PowershellVirtualMachine::IsPluginExt(LPCTSTR ext)
{
  return (_tcsicmp(ext, _T("ps1")) == 0);
}
#endif /*ACTIONMONITOR_PS_PLUGIN*/