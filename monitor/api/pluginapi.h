#pragma once

#include "helperapi.h"

// this is the version number for that particular API
static const double ACTIONMONITOR_API_PLUGIN_VERSION = 0.1f;

// add the LUA libs
#ifdef ACTIONMONITOR_API_PLUGIN

// support for PluginAPI
#include "../plugins/amplugin.h"

class pluginapi : public helperapi
{
public:
  pluginapi();
  virtual ~pluginapi();

public:
  double version ();
  size_t getCommandCount();

  bool say         ( LPCWSTR msg, UINT nElapse, UINT nFadeOut);
  bool execute     ( LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged);
  int getString    ( DWORD nBufferLength, LPWSTR lpBuffer);
  size_t getCommand( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  int getAction    ( DWORD nBufferLength, LPWSTR lpBuffer );
  int getFile      ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  int getFolder    ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  int getURL       ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  bool addAction   ( LPCWSTR szText, LPCWSTR szPath );
  bool removeAction( LPCWSTR szText, LPCWSTR szPath );
  bool getVersion  ( DWORD nBufferLength, LPWSTR lpBuffer);
  bool findAction  ( UINT idx, LPCWSTR lpCommand, DWORD nBufferLength, LPWSTR lpBuffer);
};

#endif
