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

#include <map>
#include <mutex>
#include <os/ipcmessagehandler.h>
#include "shellapi.h"

class ShellVirtualMachine final : public myodd::os::IpcMessageHandler
{
public:
  ShellVirtualMachine();
  virtual ~ShellVirtualMachine();

  int ExecuteInThread(LPCTSTR pluginFile, const ActiveAction& action);
  static bool IsExt(const std::wstring& file);

  bool HandleIpcMessage(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) override;

  void DestroyScripts();

protected:
  void Initialize();

  bool _initialized;

  std::mutex _mutex;

  typedef std::map<std::wstring, ShellApi*> APIS;
  APIS _apis;

  ShellApi* AddApi(const std::wstring& uuid, const ActiveAction& action);
  ShellApi* FindApi(const std::wstring& uuid) const;
  void RemoveApi(const std::wstring& uuid );
  void RemoveApis();

  void WaitForApi(const std::wstring& uuid);

  static bool ShellPath(std::wstring& szPath);

protected:
  /**
   * \brief create the full command line argument that will be passed to powershell
   * \param action the action we are working with
   * \param pluginPath the path to the plugin
   * \param uuid the unique id
   */
  virtual std::wstring GetCommandLineArguments(
    const ActiveAction& action,
    const std::wstring& pluginPath,
    const std::wstring& uuid
  ) const;
};
#endif /*ACTIONMONITOR_S_PLUGIN*/