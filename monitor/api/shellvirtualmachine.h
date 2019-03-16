//This file is part of Myoddweb.Piger.
//
//    Myoddweb.Piger is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Myoddweb.Piger is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Myoddweb.Piger.  If not, see<https://www.gnu.org/licenses/gpl-3.0.en.html>.
#pragma once
#ifdef ACTIONMONITOR_S_PLUGIN
#include "ExecuteVirtualMachine.h"
#include "ExecuteApi.h"

class ShellVirtualMachine final : public ExecuteVirtualMachine
{
public:
  explicit ShellVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener);
  virtual ~ShellVirtualMachine();

  static bool IsExt(const std::wstring& file);

  bool HandleIpcMessage(ExecuteApi& api, const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) override;
  bool Execute(ExecuteApi& api, const IActiveAction& action, const std::wstring& pluginFile) override;
  ExecuteApi* CreateApi(const std::wstring& uuid, const IActiveAction& action, IActions& actions, IMessagesHandler& messages) override;

protected:
  static bool ShellPath(std::wstring& szPath);

  /**
   * \brief create the full command line argument that will be passed to powershell
   * \param action the action we are working with
   * \param pluginPath the path to the plugin
   * \param uuid the unique id
   */
  std::wstring GetCommandLineArguments(
    const IActiveAction& action,
    const std::wstring& pluginPath,
    const std::wstring& uuid
  ) const;
};
#endif /*ACTIONMONITOR_S_PLUGIN*/