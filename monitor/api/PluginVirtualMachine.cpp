#include "stdafx.h"

#ifdef ACTIONMONITOR_API_PLUGIN

#include "PluginVirtualMachine.h"
#include "helperapi.h"
#include "ActionMonitor.h"
#include "../threads/lock.h"

/**
 * Todo
 * @param void
 * @return void
 */
PluginVirtualMachine::PluginVirtualMachine() : 
  _amPlugin( NULL )
{
  //  get our own module architecture.
  _moduleArchitecture = myodd::os::GetImageArchitecture( NULL );
}

/**
 * Todo
 * @param void
 * @return void
 */
PluginVirtualMachine::~PluginVirtualMachine()
{
  //  destroy all the plugins.
  DestroyPlugins();

  // remove the plugins list
  delete _amPlugin;
}

/**
 * Remove the current thread API from our list.
 */
void PluginVirtualMachine::DisposeApi()
{
  // lock it
  myodd::threads::Lock guard( _mutex );
  
  // find this thread
  ListOfPlugins::const_iterator it = _apis.find(std::this_thread::get_id());
  if (it == _apis.end())
  {
    // not found...
    return;
  }

  // remove it.
  _apis.erase(it);
}

/**
 * Add an api to our current list of plugins.
 * this must be in thread
 * @param PluginApi* api the api we would like to add.
 */
void PluginVirtualMachine::AddApi(PluginApi* api)
{
  // lock it
  myodd::threads::Lock guard(_mutex);

  ListOfPlugins::const_iterator it = _apis.find(std::this_thread::get_id());
  if (it != _apis.end())
  {
    // we are trying to add an api
    // to more than one thread.
    throw - 1;
  }

  // add it
  _apis[std::this_thread::get_id()] = api;
}

PluginApi& PluginVirtualMachine::GetApi()
{
#ifndef ACTIONMONITOR_API_PLUGIN
  throw - 1;
#else
  // get our current self.
  PluginVirtualMachine* pvm = App().GetPluginVirtualMachine();

  // for now, get the first value...
  // @todo, we need to get the actual thread id running.
  ListOfPlugins::const_iterator it = pvm->_apis.find(std::this_thread::get_id());
  if (it == pvm->_apis.end() )
  {
    throw -1;
  }
  return *(it->second);
#endif
}

/**
 * Todo
 * @param void
 * @return void
 */
void PluginVirtualMachine::Initialize()
{
  //  only do it once
  if (_amPlugin == NULL)
  {
    //  get the lock.
    myodd::threads::Lock guard(_mutex);

    // double lock...
    if (_amPlugin == NULL)
    {
      //  we can now create it.
      _amPlugin = new AmPlugin();

      // register our Plugin functions.
      InitializeFunctions();
    }
  }

}

/**
* Todo
* @param void
* @return void
*/
void PluginVirtualMachine::InitializeFunctions()
{
  Register(_T("say"), PluginVirtualMachine::Say);
  Register(_T("version"), PluginVirtualMachine::Version);
  Register(_T("getCommand"), PluginVirtualMachine::GetCommand);
  Register(_T("getAction"), PluginVirtualMachine::GetAction);
  Register(_T("getCommandCount"), PluginVirtualMachine::GetCommandCount);
  Register(_T("execute"), PluginVirtualMachine::Execute);
  Register(_T("getString"), PluginVirtualMachine::GetString);
  Register(_T("getFile"), PluginVirtualMachine::GetFile);
  Register(_T("getFolder"), PluginVirtualMachine::GetFolder);
  Register(_T("getURL"), PluginVirtualMachine::GetURL);
  Register(_T("addAction"), PluginVirtualMachine::AddAction);
  Register(_T("removeAction"), PluginVirtualMachine::RemoveAction);
  Register(_T("getVersion"), PluginVirtualMachine::GetVersion);
  Register(_T("findAction"), PluginVirtualMachine::FindAction);
  Register(_T("getForegroundWindow"), PluginVirtualMachine::GetForegroundWindow);
}

/**
 * Add a function by name that will handle a certain action.
 * @param LPCTSTR the name of the function we are registering
 * @param void* the function itself.
 * @return bool if the function was successfully added or not.
 */
bool PluginVirtualMachine::Register( LPCTSTR what, void* with )
{
  if( !_amPlugin )
  {
    return false;
  }
  _amPlugin->Add( what,  with );

  return true;
}

/**
 * Todo
 * @param void
 * @return void
 */
bool PluginVirtualMachine::IsPluginExt( LPCTSTR ext )
{
  return ( _tcsicmp( ext, _T("amp") ) == 0 );
}

/**
 * Todo
 * @param void
 * @return void
 */
PluginVirtualMachine::PLUGIN_THREAD* PluginVirtualMachine::Find( const MYODD_STRING& s)
{
  //  get the lock.
  myodd::threads::Lock guard( _mutex );

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
int PluginVirtualMachine::ExecuteInThread( LPCTSTR pluginFile )
{
  Initialize();
  if( NULL == _amPlugin )
  {
    // the plugin manager was not created?
    // or could not be created properly
    return -1;
  }

  // we need to look for that module
  // if it exists then all we need to do is send another 'CreateActiveAction'
  //
  // otherwise we need to ask it to restart again.
  PLUGIN_THREAD* f = Find( pluginFile );
  if( NULL == f )
  {
    //  init this file.
    return Create( pluginFile );
  }

  // assume error 
  AM_RESPONSE result = AM_RESP_NONE;

  // Start the un-threaded main function
  // it is up to the plugins to ensure that they don't block the main thread for too long
  try
  {
    result = f-> fnMsg( AM_MSG_MAIN, 0, (AM_INT)_amPlugin );
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
int PluginVirtualMachine::Create( LPCTSTR pluginFile )
{
  // we must have an api by now...
  PluginApi& api = GetApi();

  HMODULE hModule = ExpandLoadLibrary( pluginFile );
  if( NULL == hModule )
  {
    myodd::os::ARCHITECTURE pe = myodd::os::GetImageArchitecture(pluginFile);
    if (pe == myodd::os::ARCHITECTURE_UNKNOWN)
    {
      api.Say(_T("<b>Error : </b> Plugin could not be loaded."), 3000, 5);
    }
    else
    if (pe != _moduleArchitecture )
    {
      api.Say(_T("<b>Error : </b> Plugin could not be loaded, the architecture of the plugin does not match our own!"), 3000, 5);
    }
    else
    {
      //  it is not unknown, but somehow we could not load it.
      api.Say(_T("<b>Error : </b> Plugin could not be loaded, are some required dlls missing?"), 3000, 5);
    }
    return -1;
  }

  // Do it
  PFUNC_MSG pfMsg = (PFUNC_MSG)GetProcAddress( hModule, "am_Msg");
  if (NULL == pfMsg )
  {
    api.Say( _T("<b>Error : </b> Missing Function '<i>am_Msg</i>' )</i>"), 3000, 5 );
    return -1;
  }

  PLUGIN_THREAD* f = new PLUGIN_THREAD;
  f-> hModule = hModule;
  f-> fnMsg   = pfMsg;

  //  get the lock.
  myodd::threads::Lock guard(_mutex);
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
    pfMsg( AM_MSG_PATH_CMD, 0, (AM_INT)lp );
    lp = myodd::config::get( _T("paths\\in"));
    pfMsg( AM_MSG_PATH_IN, 0, (AM_INT)lp );
    lp = myodd::config::get( _T("paths\\out"));
    pfMsg( AM_MSG_PATH_OUT, 0, (AM_INT)lp );
    lp = myodd::config::get( _T("paths\\tmp"));
    pfMsg( AM_MSG_PATH_TMP, 0, (AM_INT)lp );
    lp = myodd::config::get( _T("paths\\plugin"));
    pfMsg( AM_MSG_PATH_PLUGIN, 0, (AM_INT)lp );

    // call the init path
    result = pfMsg( AM_MSG_INIT,     0, (AM_INT)_amPlugin );
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
void PluginVirtualMachine::ErasePlugin( const MYODD_STRING& plugin)
{
  //  get the lock.
  myodd::threads::Lock guard(_mutex);

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

    //  free the memory
    delete f;
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
  //  get the lock.
  myodd::threads::Lock guard(_mutex);

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

/**
* Todo
* @see __super::version
* @param void
* @return void
*/
double PluginVirtualMachine::Version()
{
  return GetApi().Version();
}

/**
* Todo
* @see __super::say
* @param void
* @param void
* @param void
* @return void
*/
bool PluginVirtualMachine::Say(LPCWSTR msg, UINT nElapse, UINT nFadeOut)
{
  return GetApi().Say(msg, nElapse, nFadeOut);
}

/**
* Todo
* @see __super::getCommand
* @param void
* @param void
* @param void
* @return void
*/
size_t PluginVirtualMachine::GetCommand(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer)
{
  return GetApi().GetCommand( idx, nBufferLength, lpBuffer);
}

/**
* Todo
* @see __super::getAction
* @param void
* @param void
* @param void
* @return void
*/
int PluginVirtualMachine::GetAction(DWORD nBufferLength, LPWSTR lpBuffer)
{
  return GetApi().GetAction( nBufferLength, lpBuffer);
}

/**
* Get the number of command, (space delimited).
* @see __super::getCommandCount
* @return in the number of commanded entered by the user
*/
size_t PluginVirtualMachine::GetCommandCount()
{
  return GetApi().GetCommandCount();
}

/**
* Todo
* @see __super::execute
* @param void
* @param void
* @param bool isPrivileged if we need administrator privilege to run this.
* @return void
*/
bool PluginVirtualMachine::Execute(LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged)
{
  return GetApi().Execute(module, cmdLine, isPrivileged);
}

/**
* Todo
* @see __super::getString
* @param void
* @param void
* @return void
*/
int PluginVirtualMachine::GetString(DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote)
{
  return GetApi().GetString(nBufferLength, lpBuffer, bQuote );
}

/**
* Todo
* @see __super::getFile
* @param void
* @param void
* @param void
* @return void
*/
int PluginVirtualMachine::GetFile(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote)
{
  return GetApi().GetFile( idx, nBufferLength, lpBuffer, bQuote );
}

/**
* Todo
* @see __super::getFolder
* @param void
* @param void
* @param void
* @return void
*/
int PluginVirtualMachine::GetFolder(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote)
{
  return GetApi().GetFolder( idx, nBufferLength, lpBuffer, bQuote );
}

/**
* Todo
* @see __super::getURL
* @param void
* @param void
* @param void
* @return void
*/
int PluginVirtualMachine::GetURL(UINT idx, DWORD nBufferLength, LPWSTR lpBuffer, bool bQuote)
{
  return GetApi().GetURL( idx,  nBufferLength,  lpBuffer, bQuote );
}

/**
* Todo
* @see __super::addAction
* @param void
* @param void
* @return void
*/
bool PluginVirtualMachine::AddAction(LPCWSTR szText, LPCWSTR szPath)
{
  return GetApi().AddAction(szText, szPath);
}

/**
* Todo
* @see __super::RemoveAction
* @param void
* @param void
* @return void
*/
bool PluginVirtualMachine::RemoveAction(LPCWSTR szText, LPCWSTR szPath)
{
  return GetApi().RemoveAction( szText, szPath);
}

/**
* Get the action monitor version string.
* @param DWORD the max size of the buffer.
* @param LPWSTR the buffer that will contain the return data, (version).
* @return bool success or not if there was an error.
*/
bool PluginVirtualMachine::GetVersion(DWORD nBufferLength, LPWSTR lpBuffer)
{
  return GetApi().GetVersion( nBufferLength, lpBuffer);
}

/**
* Find an action.
* @param UINT the action number we are after.
* @param LPCWSTR the action name we are looking for.
* @param DWORD the max size of the buffer.
* @param LPWSTR the buffer that will contain the return data, (version).
* @return bool if the command exists or not.
*/
bool PluginVirtualMachine::FindAction
(
  UINT idx,
  LPCWSTR lpCommand,
  DWORD nBufferLength,
  LPWSTR lpBuffer
)
{
  return GetApi().FindAction
    (
      idx,
      lpCommand,
      nBufferLength,
      lpBuffer
    );
}

/**
 * Get the last foreground window.
 * @return HWND the last foregorund window.
 */
HWND PluginVirtualMachine::GetForegroundWindow()
{
  return GetApi().GetForegroundWindow();
}

#endif /* ACTIONMONITOR_API_PLUGIN*/
