#pragma once

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

class pluginapi
{
protected: // all static
  pluginapi(void);
  ~pluginapi(void);

public:
  static void Initialize();

  static double version ();
  static size_t getCommandCount();

  static bool say         ( LPCWSTR msg, UINT nElapse, UINT nFadeOut);
  static bool execute     ( LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged);
  static int getString    ( DWORD nBufferLength, LPWSTR lpBuffer);
  static size_t getCommand( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  static int getAction    ( DWORD nBufferLength, LPWSTR lpBuffer );
  static int getFile      ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  static int getFolder    ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  static int getURL       ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );
  static bool addAction   ( LPCWSTR szText, LPCWSTR szPath );
  static bool removeAction( LPCWSTR szText, LPCWSTR szPath );
  static bool getVersion  ( DWORD nBufferLength, LPWSTR lpBuffer);
  static bool findAction  ( UINT idx, LPCWSTR lpCommand, DWORD nBufferLength, LPWSTR lpBuffer);
};