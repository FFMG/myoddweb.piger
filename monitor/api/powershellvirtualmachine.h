#pragma once
#include "IVirtualMachine.h"
#ifdef ACTIONMONITOR_PS_PLUGIN

#include <map>
#include <mutex>
#include <os/ipcmessagehandler.h>
#include "powershellapi.h"

class PowershellVirtualMachine : public myodd::os::IpcMessageHandler, public IVirtualMachine
{
public:
  explicit PowershellVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener);
  virtual ~PowershellVirtualMachine();

  static bool IsExt(const MYODD_STRING& file);

  bool HandleIpcMessage(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) override;

  int Execute(const ActiveAction& action, const std::wstring& pluginFile) override;
  void Destroy() override;
  bool Initialize() override;

private:
  /**
   * \brief if we initialized the virtual machine
   */
  bool _initialized;

  /**
   * \brief the mutex we are using.
   */
  std::mutex _mutex;

  /**
   * \brief the collection of created apis.
   */
  typedef std::map<std::wstring, PowershellApi*> Apis;
  Apis _apis;

  PowershellApi* AddApi(const std::wstring& uuid, const ActiveAction& action );
  PowershellApi* FindApi(const std::wstring& uuid) const;
  void RemoveApi(const std::wstring& uuid );
  void RemoveApis();

  void WaitForApi(const std::wstring& uuid);

  static bool IsPowershell3Installed();
  static bool Powershell3Path(MYODD_STRING& szPath);

protected:
  /**
   * \brief create the full command line argument that will be passed to powershell
   * \param action the action we are working with
   * \param dllFullPath the full path of the dll
   * \param dllInterfaceFullPath the interface full path.
   * \param pluginPath the path to the plugin
   * \param uuid the unique id
   */
  virtual MYODD_STRING GetCommandLineArguments(
    const ActiveAction& action,
    const std::wstring& dllFullPath,
    const std::wstring& dllInterfaceFullPath,
    const std::wstring& pluginPath,
    const std::wstring& uuid
  ) const;
};
#endif /*ACTIONMONITOR_PS_PLUGIN*/