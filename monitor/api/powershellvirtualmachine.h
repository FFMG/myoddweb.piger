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
  static bool IsExt(const MYODD_STRING& file);

  bool HandleIpcMessage(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) override;

  void DestroyScripts();

protected:
  void Initialize();

  bool _initialized;

  std::mutex _mutex;

  typedef std::map<std::wstring, PowershellApi*> APIS;
  APIS _apis;

  PowershellApi* AddApi(const std::wstring& uuid, const ActiveAction& action);
  PowershellApi* FindApi(const std::wstring& uuid) const;
  void RemoveApi(const std::wstring& uuid );
  void RemoveApis();

  void WaitForApi(const std::wstring& uuid);

  static bool IsPowershell3Installed();
  static bool Powershell3Path(MYODD_STRING& szPath);

protected:
  /**
   * \brief create the full command line argument that will be passed to powershell
   * \param dllFullpath the full path of the dll
   * \param pluginPath the path to the plugin
   * \param uuid the unique id
   */
  virtual MYODD_STRING GetCommandLineArguments(const std::wstring& dllFullpath, const std::wstring& pluginPath, const std::wstring& uuid);
};
#endif /*ACTIONMONITOR_PS_PLUGIN*/