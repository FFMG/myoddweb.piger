#pragma once
#ifdef ACTIONMONITOR_CS_PLUGIN

#include "csapi.h"

class CsVirtualMachine : public PowershellVirtualMachine
{
public:
  CsVirtualMachine();
  virtual ~CsVirtualMachine();

  static bool IsExt(const MYODD_STRING& file);


protected:
  /**
   * \brief create the full command line argument that will be passed to powershell
   * \param action the action we are working with
   * \param dllFullPath the full path of the dll
   * \param pluginPath the path to the plugin
   * \param uuid the unique id
   */
  MYODD_STRING GetCommandLineArguments(const ActiveAction& action, const std::wstring& dllFullPath, const std::wstring& pluginPath, const std::wstring& uuid) override;
};
#endif /*ACTIONMONITOR_CS_PLUGIN*/