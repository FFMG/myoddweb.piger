#pragma once
#include <os/ipcmessagehandler.h>

#ifdef ACTIONMONITOR_PS_PLUGIN

class PowershellVirtualMachine : public myodd::os::IpcMessageHandler
{
public:
  PowershellVirtualMachine();
  virtual ~PowershellVirtualMachine();

  int ExecuteInThread(LPCTSTR pluginFile, const ActiveAction& action);
  static bool IsPluginExt(LPCTSTR ext);

  bool HandleIpcMessage(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) override;

protected:
  void Initialize();
};
#endif /*ACTIONMONITOR_PS_PLUGIN*/