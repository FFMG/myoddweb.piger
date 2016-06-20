#pragma once

// add the APLI libs
#ifdef ACTIONMONITOR_API_PLUGIN
#include "helperapi.h"

// this is the version number for that particular API
static const double ACTIONMONITOR_API_PLUGIN_VERSION = 0.1f;

class PluginApi : public HelperApi
{
public:
  PluginApi(const ActiveAction& action);
  virtual ~PluginApi();

public:
  double Version ();
  size_t GetCommandCount();

  bool Say         (const wchar_t* msg, UINT nElapse, UINT nFadeOut);
  bool Execute     (const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged);
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
