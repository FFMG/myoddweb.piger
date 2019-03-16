#pragma once

// add the APLI libs
#ifdef ACTIONMONITOR_PS_PLUGIN
#include "ExecuteApi.h"
#include <string>

// this is the version number for that particular API
static const std::wstring ACTIONMONITOR_PS_PLUGIN_VERSION = L"0.1";

class PowershellApi : public ExecuteApi
{
public:
  PowershellApi(const std::wstring& uuid, const IActiveAction& action, IActions& actions, IMessagesHandler& messagesHandler);
  virtual ~PowershellApi();

public:
  bool Say(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) const override;
  bool Version (const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)  const override;
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
