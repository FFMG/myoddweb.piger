#include "stdafx.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

#include "PluginVirtualMachine.h"
#include "helperapi.h"

// define the virtual machine object.
static PluginVirtualMachine pluginVirtualMachine;

/**
 * Todo
 * @param void
 * @return void
 */
PluginVirtualMachine& GetPluginVirtualMachine()
{
  pluginVirtualMachine.Initialize();
  return (pluginVirtualMachine);
}

/**
 * Todo
 * @param void
 * @return void
 */
PluginVirtualMachine::PluginVirtualMachine(void) : m_amPlugin( NULL )
{
  //  get our own module architecture.
  _moduleArchitecture = myodd::os::GetImageArchitecture( NULL );
}

/**
 * Todo
 * @param void
 * @return void
 */
PluginVirtualMachine::~PluginVirtualMachine(void)
{
  //  destroy all the plugins.
  DestroyPlugins();

  // clean the monitor.
  delete m_amPlugin;
}

/**
 * Todo
 * @param void
 * @return void
 */
void PluginVirtualMachine::Initialize()
{
  //  only do it once
  if( !m_amPlugin )
  {
    m_amPlugin = new amplugin();
    // register our Plugin functions.
    pluginapi::Initialize( );
  }
}

/**
 * Add a function by name that will handle a certain action.
 * @param LPCTSTR the name of the function we are registering
 * @param void* the function itself.
 * @return bool if the function was successfully added or not.
 */
bool PluginVirtualMachine::Register( LPCTSTR what, void* with )
{
  if( !m_amPlugin )
  {
    return false;
  }
  m_amPlugin->Add( what,  with );

  return true;
}

/**
 * Todo
 * @param void
 * @return void
 */
bool PluginVirtualMachine::IsPluginExt( LPCTSTR ext ) const
{
  return ( _tcsicmp( ext, _T("amp") ) == 0 );
}

/**
 * Todo
 * @param void
 * @return void
 */
PluginVirtualMachine::PLUGIN_THREAD* PluginVirtualMachine::Find( const STD_TSTRING& s) const
{
  PLUGIN_CONTAINER::const_iterator iter = m_pluginsContainer.find( s );
  if( iter == m_pluginsContainer.end() )
  {
    return NULL;
  }

  // return what was found.
  return iter->second;
}

/**
 * Same as LoadLibrary( ... ) but we first expand the value.
 * @see ::LoadLibrary( ... )
 * @param LPCTSTR the file we want to load
 * @return HMODULE NULL | or the handle of the lib
 */
HMODULE PluginVirtualMachine::ExpandLoadLibrary( LPCTSTR lpFile )
{
  // try  and expand it first
  LPTSTR lpExpandFile = NULL;
  if( !myodd::files::ExpandEnvironment( lpFile, lpExpandFile ) )
  {
    //  something broke.
    return NULL;
  }
  HMODULE hMod = LoadLibrary( lpExpandFile );
  delete [] lpExpandFile;
  return hMod;
}

/**
 * Todo
 * @param void
 * @return void
 */
int PluginVirtualMachine::LoadFile( LPCTSTR pluginFile )
{
  if( NULL == m_amPlugin )
  {
    // the plugin manager was not created?
    return -1;
  }

  // we need to look for that module
  // if it exists then all we need to do is send another 'doit'
  //
  // otherwise we need to ask it to restart again.
  PLUGIN_THREAD* f = Find( pluginFile );

  if( NULL == f )
  {
    //  init this file.
    return InitFile( pluginFile );
  }

  // assume error 
  AM_RESPONSE result = AM_RESP_NONE;

  // Start the un-threaded main function
  // it is up to the plugins to ensure that they don't block the main thread for too long
  try
  {
    result = f-> fnMsg( AM_MSG_MAIN, 0, (LPARAM)m_amPlugin );
  }
  catch( ... )
  {
    // something bad has happened.
    result = AM_RESP_THROW;
    ErasePlugin( pluginFile );
  }
  return ( result );
}

/**
 * Initialize a plugin for the first time.
 * This is when we load the file.
 * @param LPCTSTR the plugin we are loading.
 * @return
 */
int PluginVirtualMachine::InitFile( LPCTSTR pluginFile )
{
  HMODULE hModule = ExpandLoadLibrary( pluginFile );
  if( NULL == hModule )
  {
    myodd::os::ARCHITECTURE pe = myodd::os::GetImageArchitecture(pluginFile);
    if (pe == myodd::os::ARCHITECTURE_UNKNOWN)
    {
      helperapi::say(_T("<b>Error : </b> Plugin could not be loaded."), 3000, 5);
    }
    else
    if (pe != _moduleArchitecture )
    {
      helperapi::say(_T("<b>Error : </b> Plugin could not be loaded, the architecture of the plugin does not match our own!"), 3000, 5);
    }
    else
    {
      //  it is not unknown, but somehow we could not load it.
      helperapi::say(_T("<b>Error : </b> Plugin could not be loaded, are some required dlls missing?"), 3000, 5);
    }
    return -1;
  }

  // Do it
  PFUNC_MSG pfMsg = (PFUNC_MSG)GetProcAddress( hModule, "am_Msg");
  if (NULL == pfMsg )
  {
    helperapi::say( _T("<b>Error : </b> Missing Function '<i>am_Msg</i>' )</i>"), 3000, 5 );
    return -1;
  }

  PLUGIN_THREAD* f = new PLUGIN_THREAD;
  f-> hModule         = hModule;
  f-> fnMsg           = pfMsg;

  m_pluginsContainer[ pluginFile ] = f;

  // assume error 
  AM_RESPONSE result = AM_RESP_NONE;

  // as this is the first time we call this plugin
  // we need to initialize it here
  // this tells the plugin that this is the first call.
  try
  {
    // set some of the paths that might be of interest.
    LPCWSTR lp = NULL;
    lp = myodd::config::get( _T("paths\\commands") );
    pfMsg( AM_MSG_PATH_CMD, 0, (LPARAM)lp );
    lp = myodd::config::get( _T("paths\\in"));
    pfMsg( AM_MSG_PATH_IN, 0, (LPARAM)lp );
    lp = myodd::config::get( _T("paths\\out"));
    pfMsg( AM_MSG_PATH_OUT, 0, (LPARAM)lp );
    lp = myodd::config::get( _T("paths\\tmp"));
    pfMsg( AM_MSG_PATH_TMP, 0, (LPARAM)lp );
    lp = myodd::config::get( _T("paths\\plugin"));
    pfMsg( AM_MSG_PATH_PLUGIN, 0, (LPARAM)lp );

    // call the init path
    result = pfMsg( AM_MSG_INIT,     0, (LPARAM)m_amPlugin );
  }
  catch( ... )
  {
    //  something broke, we don't like this plugin.
    ErasePlugin( pluginFile );
    result = AM_RESP_THROW;
  }

  return result;
}

/**
 * Look for a single plugin and remove it from the list.
 * We do not wait for the thread to end, we simply kill it.
 */
void PluginVirtualMachine::ErasePlugin( const STD_TSTRING& plugin)
{
  PLUGIN_CONTAINER::const_iterator iter = m_pluginsContainer.find( plugin );
  if( iter == m_pluginsContainer.end() )
  {
    return;
  }
  
  try
  {
    //  try and destroy it.
    PLUGIN_THREAD* f = iter->second;
 
    // destroy
    f-> fnMsg( AM_MSG_DEINIT, 0, 0 );
  }
  catch( ... )
  {
    //  even that did not work.
  }

  //  remove it from the list.
  m_pluginsContainer.erase( iter );
}

/**
 * Remove/erase all the plugins, wait for them to end 'gracefully'.
 * @param void
 * @return void
 */
void PluginVirtualMachine::DestroyPlugins()
{
  //  we must clear all the thread.
  for( PLUGIN_CONTAINER::const_iterator it = m_pluginsContainer.begin(); 
       it != m_pluginsContainer.end(); 
       ++it 
     )
  {
    PLUGIN_THREAD* f = it->second;

    // destroy
    f-> fnMsg( AM_MSG_DEINIT, 0, 0 );

    // we are done here with this module...
    FreeLibrary( f->hModule );

    // and with the pointer
    delete f;
    f = NULL;
  }// for each modules.

  // and remove everything
  m_pluginsContainer.erase( m_pluginsContainer.begin(), m_pluginsContainer.end() );
}