#pragma once

#include "pluginapi.h"
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
  void ErasePlugin( const STD_TSTRING& plugin);

protected:
  typedef std::map< std::thread::id, pluginapi*> ListOfPlugins;
  ListOfPlugins _apis;
  
  std::mutex _mutex;
  static pluginapi& GetApi();

public:
  void DisposeApi();
  void AddApi(pluginapi* api );

protected:
  amplugin* _amPlugin;
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
  typedef std::map< STD_TSTRING, PLUGIN_THREAD*> PLUGIN_CONTAINER;

  PLUGIN_CONTAINER m_pluginsContainer;

  // find a module
  PLUGIN_THREAD* Find( const STD_TSTRING& );

  HMODULE ExpandLoadLibrary( LPCTSTR lpFile );

public:
  static double version();
  static size_t getCommandCount();

  static bool say(LPCWSTR msg, UINT nElapse, UINT nFadeOut);
  static bool execute(LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged);
  static int getString(DWORD nBufferLength, LPWSTR lpBuffer);
  static size_t getCommand(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer);
  static int getAction(DWORD nBufferLength, LPWSTR lpBuffer);
  static int getFile(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer);
  static int getFolder(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer);
  static int getURL(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer);
  static bool addAction(LPCWSTR szText, LPCWSTR szPath);
  static bool removeAction(LPCWSTR szText, LPCWSTR szPath);
  static bool getVersion(DWORD nBufferLength, LPWSTR lpBuffer);
  static bool findAction(UINT idx, LPCWSTR lpCommand, DWORD nBufferLength, LPWSTR lpBuffer);
};
#endif /*ACTIONMONITOR_API_PLUGIN*/
