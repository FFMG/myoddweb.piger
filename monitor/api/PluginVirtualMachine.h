#pragma once

#include "PluginApi.h"
#include <map>
#include <thread>
#include <mutex>
#include "os/os.h"

#ifdef ACTIONMONITOR_API_PLUGIN
class PluginVirtualMachine
{
public:
  PluginVirtualMachine();
  virtual ~PluginVirtualMachine();

  int ExecuteInThread( LPCTSTR pluginFile );
  static bool IsPluginExt( LPCTSTR ext );

  bool Register( LPCTSTR, void* );

  void DestroyPlugins();
  void ErasePlugin( const MYODD_STRING& plugin);

protected:
  typedef std::map< std::thread::id, PluginApi*> ListOfPlugins;
  ListOfPlugins _apis;
  
  std::mutex _mutex;
  static PluginApi& GetApi();

public:
  void DisposeApi();
  void AddApi(PluginApi* api );

protected:
  AmPlugin* _amPlugin;
  void Initialize();
  void InitializeFunctions();

protected:
  int Create( LPCTSTR pluginFile );

  // our own architecture.
  myodd::os::ARCHITECTURE _moduleArchitecture;

protected:
  // the plugin functions.
  typedef AM_RESPONSE (*PFUNC_MSG)(AM_MSG msg, AM_UINT wParam, AM_INT lParam);

  // the threads structures.
  struct PLUGIN_THREAD
  {
    HMODULE    hModule;
    PFUNC_MSG  fnMsg;
  };

  // map of all the functions.
  typedef std::map< MYODD_STRING, PLUGIN_THREAD*> PLUGIN_CONTAINER;

  PLUGIN_CONTAINER m_pluginsContainer;

  // find a module
  PLUGIN_THREAD* Find( const MYODD_STRING& );

  HMODULE ExpandLoadLibrary( LPCTSTR lpFile );

public:
  static double Version();
  static size_t GetCommandCount();

  static bool Say(LPCWSTR msg, UINT nElapse, UINT nFadeOut);
  static bool Execute(LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged);
  static int GetString(DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote);
  static size_t GetCommand(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer);
  static int GetAction(DWORD nBufferLength, LPWSTR lpBuffer);
  static int GetFile(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote);
  static int GetFolder(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote);
  static int GetURL(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote);
  static bool AddAction(LPCWSTR szText, LPCWSTR szPath);
  static bool RemoveAction(LPCWSTR szText, LPCWSTR szPath);
  static bool GetVersion(DWORD nBufferLength, LPWSTR lpBuffer);
  static bool FindAction(UINT idx, LPCWSTR lpCommand, DWORD nBufferLength, LPWSTR lpBuffer);
  static HWND GetForegroundWindow();
};
#endif /*ACTIONMONITOR_API_PLUGIN*/
