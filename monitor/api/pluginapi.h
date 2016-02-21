#pragma once

// this is the version number for that particular API
static const double ACTIONMONITOR_API_PLUGIN_VERSION = 0.1f;

// support for PluginAPI
#include "../plugins/amplugin.h"

// add the LUA libs
#ifdef _DEBUG
  #pragma comment(lib, "../plugins/amplugind.lib" )
#else
  #pragma comment(lib, "../plugins/amplugin.lib" )
#endif

class pluginapi
{
protected: // all static
  pluginapi(void);
  ~pluginapi(void);

public:
  static void Initialize();

  static double version ();
  static int getCommandCount();

  static bool say         ( LPCWSTR msg, UINT nElapse, UINT nFadeOut);
  static bool execute     ( LPCWSTR module, LPCWSTR cmdLine);
  static int getString    ( DWORD nBufferLength, LPWSTR lpBuffer);
  static int getCommand   ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  static int getAction    ( DWORD nBufferLength, LPWSTR lpBuffer );
  static int getFile      ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  static int getFolder    ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  static int getURL       ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  static bool addAction   ( LPCWSTR szText, LPCWSTR szPath );
  static bool removeAction( LPCWSTR szText, LPCWSTR szPath );
  static bool getVersion  ( DWORD nBufferLength, LPWSTR lpBuffer);
  static bool findAction  ( UINT idx, LPCWSTR lpCommand, DWORD nBufferLength, LPWSTR lpBuffer);
};