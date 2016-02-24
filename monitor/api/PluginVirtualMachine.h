#pragma once

#include "pluginapi.h"
#include <map>
#include "os/os.h"

class PluginVirtualMachine
{
public:
  PluginVirtualMachine(void);
  virtual ~PluginVirtualMachine(void);

  void Initialize(); 
  int LoadFile( LPCTSTR pluginFile );
  bool IsPluginExt( LPCTSTR ext ) const;

  bool Register( LPCTSTR, void* );

  void DestroyPlugins();
  void ErasePlugin( const STD_TSTRING& plugin);

protected:
  amplugin* m_amPlugin;

protected:
  int InitFile( LPCTSTR pluginFile );

  // our own architecture.
  myodd::os::ARCHITECTURE _moduleArchitecture;

protected:
  // the plugin functions.
  typedef AM_RESPONSE (*PFUNC_MSG)( AM_MSG msg, WPARAM wParam, LPARAM lParam );

  // the threads structures.
  struct PLUGIN_THREAD
  {
    HMODULE           hModule;
    PFUNC_MSG         fnMsg;
  };

  // map of all the functions.
  typedef std::map< STD_TSTRING, PLUGIN_THREAD*> PLUGIN_CONTAINER;

  PLUGIN_CONTAINER m_pluginsContainer;

  // find a module
  PLUGIN_THREAD* Find( const STD_TSTRING& ) const;

  HMODULE ExpandLoadLibrary( LPCTSTR lpFile );
};

// the Lua virtual machine
PluginVirtualMachine& GetPluginVirtualMachine();