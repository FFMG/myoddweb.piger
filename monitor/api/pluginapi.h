#pragma once

#include "helperapi.h"

// this is the version number for that particular API
static const double ACTIONMONITOR_API_PLUGIN_VERSION = 0.1f;

// support for PluginAPI
#include "../plugins/amplugin.h"

// add the LUA libs
#ifdef _DEBUG
# ifdef _WIN64
#   pragma comment(lib, "../plugins/amplugin64d.lib" )
#else
#   pragma comment(lib, "../plugins/amplugind.lib" )
# endif
#else
# ifdef _WIN64
#   pragma comment(lib, "../plugins/amplugin64.lib" )
# else
#   pragma comment(lib, "../plugins/amplugin.lib" )
# endif
#endif

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