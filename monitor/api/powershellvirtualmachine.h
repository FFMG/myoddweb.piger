#pragma once
#ifdef ACTIONMONITOR_PS_PLUGIN
#include "executevirtualmachine.h"

class PowershellVirtualMachine : public ExecuteVirtualMachine
{
public:
  explicit PowershellVirtualMachine(IApplication& application, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener);
  virtual ~PowershellVirtualMachine();

  static bool IsExt(const MYODD_STRING& file);

  //  handle the post message.
  bool HandleIpcMessage(ExecuteApi& api, const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) override;
  bool Execute(ExecuteApi& api, const IActiveAction& action, const std::wstring& pluginFile) override;
  ExecuteApi* CreateApi(const std::wstring& uuid, const IActiveAction& action, IApplication& application, IMessagesHandler& messages) override;

private:
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
    const IActiveAction& action,
    const std::wstring& dllFullPath,
    const std::wstring& dllInterfaceFullPath,
    const std::wstring& pluginPath,
    const std::wstring& uuid
  ) const;
};
#endif /*ACTIONMONITOR_PS_PLUGIN*/