#include "stdafx.h"
#include <os/ipclistener.h>
#include <ActionMonitorDlg.h>
#include <ActionMonitor.h>

#ifdef ACTIONMONITOR_PS_PLUGIN

#include "powershellvirtualmachine.h"

PowershellVirtualMachine::PowershellVirtualMachine() :
  _IpcListener(nullptr)
{
}

PowershellVirtualMachine::~PowershellVirtualMachine()
{
  delete _IpcListener;
  _IpcListener = nullptr;
}

// create the IPC server
void PowershellVirtualMachine::Initialize()
{
  if( nullptr != _IpcListener )
  {
    return;
  }

  // create the listenner.
  auto pThis = static_cast<CActionMonitorDlg*>(App().GetMainWnd());
  auto hWnd = pThis->GetParent()->GetSafeHwnd();
  _IpcListener = new myodd::os::IpcListener(CONF_MUTEXT, hWnd );
}

int PowershellVirtualMachine::ExecuteInThread(LPCTSTR pluginFile)
{
  Initialize();
  return 0;
}

bool PowershellVirtualMachine::IsPluginExt(LPCTSTR ext)
{
  return (_tcsicmp(ext, _T("ps1")) == 0);
}

#endif /*ACTIONMONITOR_PS_PLUGIN*/