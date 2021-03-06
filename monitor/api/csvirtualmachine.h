#pragma once
#ifdef ACTIONMONITOR_CS_PLUGIN

#include "csapi.h"

class CsVirtualMachine final : public PowershellVirtualMachine
{
public:
  explicit CsVirtualMachine(IApplication& application, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener);
  virtual ~CsVirtualMachine();

  static bool IsExt(const MYODD_STRING& file);

  ExecuteApi* CreateApi(const std::wstring& uuid, const IActiveAction& action, IApplication& application, IMessagesHandler& messages) override;
  
protected:
  /**
   * \brief create the full command line argument that will be passed to powershell
   * \param action the action we are working with
   * \param dllFullPath the full path of the dll
   * \param dllInterfaceFullPath the interface full path.
   * \param pluginPath the path to the plugin
   * \param uuid the unique id
   */
  MYODD_STRING GetCommandLineArguments(
    const IActiveAction& action,
    const std::wstring& dllFullPath,
    const std::wstring& dllInterfaceFullPath,
    const std::wstring& pluginPath, 
    const std::wstring& uuid) const override;
};
#endif /*ACTIONMONITOR_CS_PLUGIN*/