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
#include "helperapi.h"

class ExecuteApi : public HelperApi
{
protected:
  ExecuteApi(const std::wstring& uuid, const IActiveAction& action, IApplication& application, IMessagesHandler& messages);

public:
  ~ExecuteApi();

  ExecuteApi(const ExecuteApi&) = delete;
  void operator=(const ExecuteApi&) = delete;

public:
  void SetHandle(HANDLE process);
  HANDLE GetHandle() const;

  virtual bool Say(const wchar_t* sText, const unsigned int elapseMiliSecondsBeforeFadeOut, const unsigned int totalMilisecondsToShowMessage) const;
  bool Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged, HANDLE* hProcess) const override;

  virtual bool Say(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool Version(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool GetCommandCount(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool GetCommand(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool GetAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool GetString(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool GetFile(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool GetFolder(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool GetUrl(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool GetVersion(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool Log(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;

  virtual bool Execute(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;

  virtual bool FindAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool AddAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;
  virtual bool RemoveAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;

  virtual bool GetForegroundWindow(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const = 0;

  const std::wstring& GetUniqeId() const
  {
    return _uuid;
  }
private:
  const std::wstring _uuid;
  
  HANDLE _hProcess;
};

