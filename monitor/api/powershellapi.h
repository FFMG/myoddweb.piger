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
 
  size_t GetCommandCount();

  bool Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged, HANDLE* hProcess) const override;
  int GetString    ( DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote);
  size_t GetCommand( UINT idx, DWORD nBufferLength, wchar_t* lpBuffer );
  int GetAction    ( DWORD nBufferLength, wchar_t* lpBuffer );
  int GetFile      ( UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote);
  int GetFolder    ( UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote);
  int GetURL       ( UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote);
  bool AddAction   (const wchar_t* szText, const wchar_t* szPath );
  bool RemoveAction(const wchar_t* szText, const wchar_t* szPath );
  bool GetVersion  ( DWORD nBufferLength, wchar_t* lpBuffer);
  bool FindAction  ( UINT idx, const wchar_t* lpCommand, DWORD nBufferLength, wchar_t* lpBuffer);
  void Log( unsigned int logType, const wchar_t* lpText );
  HWND GetForegroundWindow() const;
};

#endif
