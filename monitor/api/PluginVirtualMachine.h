#pragma once
#include "IVirtualMachine.h"

#ifdef ACTIONMONITOR_API_PLUGIN
#include "PluginApi.h"
#include <map>
#include "../threads/lock.h"
#include "os/os.h"
#include "amplugin/ampluginprivate.h"

class PluginVirtualMachine final : public IVirtualMachine
{
public:
  PluginVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener);
  virtual ~PluginVirtualMachine();

  int Execute(const IActiveAction& action, const std::wstring& pluginFile) override;
  void Destroy() override;
  bool Initialize() override;

  static bool IsExt(const MYODD_STRING& file );

  bool Register( LPCTSTR, void* );

  void ErasePlugin( const MYODD_STRING& plugin);

protected:
  bool DisposeApi(PluginApi* api);
  void AddApi( PluginApi* api );

  int ExecuteInThread( LPCTSTR pluginFile );
protected:
  AmPluginPrivate* _amPlugin;
  void InitializeFunctions();

  static PluginApi& GetApi();

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

  myodd::threads::Key _containerKey;
  myodd::threads::Key _pluginKey;

  // map of all the functions.
  typedef std::map< MYODD_STRING, PLUGIN_THREAD*> PLUGIN_CONTAINER;
  PLUGIN_CONTAINER _pluginsContainer;

  // find a module
  PLUGIN_THREAD* Find( const std::wstring& );

  HMODULE ExpandLoadLibrary( LPCTSTR lpFile );

public:
  static double Version();
  static size_t GetCommandCount();

  static bool Say(const wchar_t* sText, unsigned int elapseMiliSecondsBeforeFadeOut, unsigned int totalMilisecondsToShowMessage);
  static bool ExecuteInPlugin(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged);
  static int GetString(DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote);
  static size_t GetCommand(UINT idx, DWORD nBufferLength, wchar_t* lpBuffer);
  static int GetAction(DWORD nBufferLength, wchar_t* lpBuffer);
  static int GetFile(UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote);
  static int GetFolder(UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote);
  static int GetUrl(UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote);
  static bool AddAction(const wchar_t* szText, const wchar_t* szPath);
  static bool RemoveAction(const wchar_t* szText, const wchar_t* szPath);
  static bool GetVersion(DWORD nBufferLength, wchar_t* lpBuffer);
  static bool FindAction(UINT idx, const wchar_t* lpCommand, DWORD nBufferLength, wchar_t* lpBuffer);
  static HWND GetForegroundWindow();
  static void Log(unsigned int logType, const wchar_t* lpText);
};
#endif /*ACTIONMONITOR_API_PLUGIN*/
