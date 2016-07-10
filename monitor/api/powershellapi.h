#pragma once

// add the APLI libs
#ifdef ACTIONMONITOR_PS_PLUGIN
#include "helperapi.h"
#include <string>

// this is the version number for that particular API
static const std::wstring ACTIONMONITOR_PS_PLUGIN_VERSION = L"0.1";

class PowershellApi : public HelperApi
{
public:
  PowershellApi(const ActiveAction& action);
  virtual ~PowershellApi();

public:
  virtual bool Say(const wchar_t* msg, const unsigned int nElapse, const unsigned int nFadeOut) const;

  bool Say(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse);
  bool Version (const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse);
  bool GetCommandCount(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse);
  bool GetCommand(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse);
  bool GetAction(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse);
  bool GetString(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse);
  bool GetFile(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse);
  bool GetFolder(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse);
  bool GetUrl(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse);
  bool GetVersion(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse);
  bool Log(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse);

  bool Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged, HANDLE* hProcess) const override;
  bool AddAction   (const wchar_t* szText, const wchar_t* szPath );
  bool RemoveAction(const wchar_t* szText, const wchar_t* szPath );
  bool FindAction  ( UINT idx, const wchar_t* lpCommand, DWORD nBufferLength, wchar_t* lpBuffer);
  HWND GetForegroundWindow() const;
};

#endif
