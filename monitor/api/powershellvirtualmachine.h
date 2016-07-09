#pragma once
#ifdef ACTIONMONITOR_PS_PLUGIN

#include <map>
#include <mutex>
#include <os/ipcmessagehandler.h>
#include "powershellapi.h"

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

  std::mutex _mutex;

  typedef std::map<std::wstring, PowershellApi*> APIS;
  APIS _apis;

  PowershellApi* AddApi(const std::wstring& uuid, const ActiveAction& action);
  void RemoveApi(const std::wstring& uuid );
  void RemoveApis();

  static bool IsPowershell3Installed();
  static bool Powershell3Path(MYODD_STRING& szPath);
};
#endif /*ACTIONMONITOR_PS_PLUGIN*/