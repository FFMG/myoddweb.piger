#pragma once

#include "helperapi.h"

// this is the version number for that particular API
static const double ACTIONMONITOR_API_PLUGIN_VERSION = 0.1f;

// add the LUA libs
#ifdef ACTIONMONITOR_API_PLUGIN

// support for PluginApi
#include "../plugins/amplugin.h"

class PluginApi : public helperapi
{
public:
  PluginApi(const ActiveAction& action);
  virtual ~PluginApi();

public:
  double Version ();
  size_t GetCommandCount();

  bool Say         ( LPCWSTR msg, UINT nElapse, UINT nFadeOut);
  bool Execute     ( LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged);
  int GetString    ( DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote);
  size_t GetCommand( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  int GetAction    ( DWORD nBufferLength, LPWSTR lpBuffer );
  int GetFile      ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote);
  int GetFolder    ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote);
  int GetURL       ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote);
  bool AddAction   ( LPCWSTR szText, LPCWSTR szPath );
  bool RemoveAction( LPCWSTR szText, LPCWSTR szPath );
  bool GetVersion  ( DWORD nBufferLength, LPWSTR lpBuffer);
  bool FindAction  ( UINT idx, LPCWSTR lpCommand, DWORD nBufferLength, LPWSTR lpBuffer);
  HWND GetForegroundWindow() const;
};

#endif
