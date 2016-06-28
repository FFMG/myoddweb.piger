#pragma once
#include <os/ipclistener.h>

#ifdef ACTIONMONITOR_PS_PLUGIN

class PowershellVirtualMachine
{
public:
  PowershellVirtualMachine();
  virtual ~PowershellVirtualMachine();

  int ExecuteInThread(LPCTSTR pluginFile);
  static bool IsPluginExt(LPCTSTR ext);

protected:
  void Initialize();
  myodd::os::IpcListener* _IpcListener;
};
#endif /*ACTIONMONITOR_PS_PLUGIN*/