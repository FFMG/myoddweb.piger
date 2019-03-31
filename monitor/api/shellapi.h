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

// add the APLI libs
#ifdef ACTIONMONITOR_S_PLUGIN
#include "ExecuteApi.h"
#include <string>

// this is the version number for that particular API
static const std::wstring ACTIONMONITOR_S_PLUGIN_VERSION = L"0.1";

class ShellApi final : public ExecuteApi
{
public:
  explicit ShellApi(const std::wstring& uuid, const IActiveAction& action, IApplication& application, IMessagesHandler& messagesHandler );
  virtual ~ShellApi() = default;

public:
  bool Say(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool Version (const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool GetCommandCount(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool GetCommand(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool GetAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool GetString(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool GetFile(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool GetFolder(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool GetUrl(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool GetVersion(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool Log(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;

  bool Execute(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;

  bool FindAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool AddAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool RemoveAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;

  bool GetForegroundWindow(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
};
#endif
