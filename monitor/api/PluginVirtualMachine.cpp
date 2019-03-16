#include "stdafx.h"

#ifdef ACTIONMONITOR_API_PLUGIN

#include "PluginVirtualMachine.h"
#include "amplugin\ampluginprivate.h"

/**
 * \copydoc
 */
PluginVirtualMachine::PluginVirtualMachine(IApplication& application, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener) :
  IVirtualMachine( application, messagesHandler, iIpcListener ),
  _amPlugin( nullptr ),
  _containerKey( L"Plugins Container")
{
  //  get our own module architecture.
  _moduleArchitecture = myodd::os::GetImageArchitecture( nullptr );
}

/**
 * \brief destructor
 */
PluginVirtualMachine::~PluginVirtualMachine()
{
  VirtualMachineLists<std::thread::id, PluginApi>::Instance().Dispose();
  // remove the plugins list
  delete _amPlugin;
}

/**
 * \brief Remove the current thread API from our list.
 * \param api the api we wish to remove.
 * \return if the item was removed or not.
 */
bool PluginVirtualMachine::DisposeApi(PluginApi* api)
{
  return VirtualMachineLists<std::thread::id, PluginApi>::Instance().Dispose(api);
}

/**
 * \brief Add an api to our current list of plugins.
 *        this must be in thread
 * \param api the api we would like to add.
 */
void PluginVirtualMachine::AddApi(PluginApi* api)
{
  VirtualMachineLists<std::thread::id, PluginApi>::Instance().AddApi(std::this_thread::get_id(), api );
}

/**
 * \brief Called by the plugins to find the currently running action.
 * \return get the plugin api so we can call helper functions.
 */
PluginApi& PluginVirtualMachine::GetApi()
{
  return VirtualMachineLists<std::thread::id, PluginApi>::Instance().GetApi(std::this_thread::get_id());
}

/**
 * \brief Initialize the virtual machine
 * \return bool success or not.
 */
bool PluginVirtualMachine::Initialize()
{
  //  only do it once
  if (_amPlugin != nullptr)
  {
    return true;
  }

  //  get the lock.
  myodd::threads::Lock guard(_pluginKey);

  // double lock...
  if (_amPlugin != nullptr)
  {
    return true;
  }

  try
  {
    //  we can now create it.
    _amPlugin = new AmPluginPrivate();

    // register our Plugin functions.
    InitializeFunctions();
    return true;
  }
  catch( ... )
  {
    return false;
  }
}

/**
* \brief register all the valid actions and the correcponding functions.
*/
void PluginVirtualMachine::InitializeFunctions()
{
  Register(_T("say"), PluginVirtualMachine::Say);
  Register(_T("version"), PluginVirtualMachine::Version);
  Register(_T("getCommand"), PluginVirtualMachine::GetCommand);
  Register(_T("getAction"), PluginVirtualMachine::GetAction);
  Register(_T("getCommandCount"), PluginVirtualMachine::GetCommandCount);
  Register(_T("execute"), PluginVirtualMachine::ExecuteInPlugin);
  Register(_T("getString"), PluginVirtualMachine::GetString);
  Register(_T("getFile"), PluginVirtualMachine::GetFile);
  Register(_T("getFolder"), PluginVirtualMachine::GetFolder);
  Register(_T("getURL"), PluginVirtualMachine::GetUrl);
  Register(_T("addAction"), PluginVirtualMachine::AddAction);
  Register(_T("removeAction"), PluginVirtualMachine::RemoveAction);
  Register(_T("getVersion"), PluginVirtualMachine::GetVersion);
  Register(_T("findAction"), PluginVirtualMachine::FindAction);
  Register(_T("getForegroundWindow"), PluginVirtualMachine::GetForegroundWindow);
  Register(_T("log"), PluginVirtualMachine::Log);
}

/**
 * Add a function by name that will handle a certain action.
 * @param LPCTSTR the name of the function we are registering
 * @param void* the function itself.
 * @return bool if the function was successfully added or not.
 */
bool PluginVirtualMachine::Register( LPCTSTR what, void* with )
{
  Initialize();
  myodd::threads::Lock guard(_pluginKey);
  if( nullptr == _amPlugin )
  {
    return false;
  }
  _amPlugin->Add( what,  with );

  return true;
}

/**
 * Check if a given file extension is used by this API or not.
 * @param const MYODD_STRING& file the file we are checking
 * @return bool true|false if the given extension is LUA or not.
 */
bool PluginVirtualMachine::IsExt(const MYODD_STRING& file )
{
  return myodd::files::IsExtension(file, _T("amp"));
}

/**
 * Todo
 * @param void
 * @return void
 */
PluginVirtualMachine::PLUGIN_THREAD* PluginVirtualMachine::Find( const std::wstring& s)
{
  //  get the lock.
  myodd::threads::Lock guard( _containerKey );

  const auto iter = _pluginsContainer.find( s );
  if( iter == _pluginsContainer.end() )
  {
    return nullptr;
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
    return nullptr;
  }
  auto hMod = LoadLibrary( lpExpandFile );
  delete [] lpExpandFile;
  return hMod;
}

int PluginVirtualMachine::ExecuteInThread(LPCTSTR pluginFile)
{
  // assume error 
  auto result = AM_RESP_NONE;
  try
  {
    // we need to look for that module
    // if it exists then all we need to do is send another 'CreateActiveAction'
    //
    // otherwise we need to ask it to restart again.
    auto f = Find(pluginFile);
    if (nullptr == f)
    {
      //  init this file.
      return Create(pluginFile);
    }

    // Start the un-threaded main function
    // it is up to the plugins to ensure that they don't block the main thread for too long
    try
    {
      result = f->fnMsg(AM_MSG_MAIN, 0, reinterpret_cast<AM_INT>(_amPlugin));
    }
    catch (...)
    {
      // something bad has happened.
      result = AM_RESP_THROW;
      ErasePlugin(pluginFile);
    }
  }
  catch (...)
  {
    // something bad has happened.
    result = AM_RESP_THROW;
    ErasePlugin(pluginFile);
  }
  return result;
}

int PluginVirtualMachine::Execute(const IActiveAction& action, const std::wstring& pluginFile)
{
  Initialize();
  if (nullptr == _amPlugin)
  {
    // the plugin manager was not created?
    // or could not be created properly
    return AM_RESP_NONE;
  }

  // add the api to the list.
  auto api = new PluginApi(action, GetApplication(), GetMessagesHandler() );
  AddApi( api );

  // execute this plugin
  const auto result = ExecuteInThread(pluginFile.c_str() );

  // dispose of the api
  if( false == DisposeApi( api ) )
  {
    myodd::log::LogError(_T("I was unable to remove the plugin API."));
  }

  //  now delete the api
  delete api;

  // return what we found.
  return result;
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
      api.Say(_T("<b>Error : </b> Plugin could not be loaded."), 500, 3000);
    }
    else
    if (pe != _moduleArchitecture )
    {
      api.Say(_T("<b>Error : </b> Plugin could not be loaded, the architecture of the plugin does not match our own!"), 500, 3000);
    }
    else
    {
      //  it is not unknown, but somehow we could not load it.
      api.Say(_T("<b>Error : </b> Plugin could not be loaded, are some required dlls missing?"), 500, 3000);
    }
    return -1;
  }

  // Do it
  auto pfMsg = (PFUNC_MSG)GetProcAddress( hModule, "am_Msg");
  if (NULL == pfMsg )
  {
    api.Say( _T("<b>Error : </b> Missing Function '<i>am_Msg</i>' )</i>"), 500, 3000);
    return -1;
  }

  auto pluginThread = new PLUGIN_THREAD;
  pluginThread-> hModule = hModule;
  pluginThread-> fnMsg   = pfMsg;

  //  get the lock.
  myodd::threads::Lock guard( _containerKey );
  _pluginsContainer[ pluginFile ] = pluginThread;

  // assume error 
  auto result = AM_RESP_NONE;

  // as this is the first time we call this plugin
  // we need to initialize it here
  // this tells the plugin that this is the first call.
  try
  {
    // set some of the paths that might be of interest.
    std::wstring lp;
    lp = static_cast<const wchar_t*>(::myodd::config::Get( L"paths\\commands", L"" ));
    pfMsg( AM_MSG_PATH_CMD, 0, reinterpret_cast<AM_INT>(lp.c_str()) );
    lp = static_cast<const wchar_t*>(::myodd::config::Get( L"paths\\in", L""));
    pfMsg( AM_MSG_PATH_IN, 0, reinterpret_cast<AM_INT>(lp.c_str()) );
    lp = static_cast<const wchar_t*>(::myodd::config::Get( L"paths\\out", L""));
    pfMsg( AM_MSG_PATH_OUT, 0, reinterpret_cast<AM_INT>(lp.c_str()) );
    lp = static_cast<const wchar_t*>(::myodd::config::Get( L"paths\\tmp", L""));
    pfMsg( AM_MSG_PATH_TMP, 0, reinterpret_cast<AM_INT>(lp.c_str()) );
    lp = static_cast<const wchar_t*>(::myodd::config::Get( L"paths\\plugin", L""));
    pfMsg( AM_MSG_PATH_PLUGIN, 0, reinterpret_cast<AM_INT>(lp.c_str()));

    // call the init path
    result = pfMsg( AM_MSG_INIT,     0, reinterpret_cast<AM_INT>(_amPlugin) );
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
  myodd::threads::Lock guard(_containerKey );

  const auto iter = _pluginsContainer.find( plugin );
  if( iter == _pluginsContainer.end() )
  {
    return;
  }
  
  try
  {
    //  try and destroy it.
    const auto f = iter->second;
 
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
  _pluginsContainer.erase( iter );
}

/**
 * Remove/erase all the plugins, wait for them to end 'gracefully'.
 * @param void
 * @return void
 */
void PluginVirtualMachine::Destroy()
{
  //  get the lock.
  myodd::threads::Lock guardContainer( _containerKey);

  //  we must clear all the thread.
  for( auto it = _pluginsContainer.begin(); 
       it != _pluginsContainer.end(); 
       ++it 
     )
  {
    auto f = it->second;

    // destroy
    f-> fnMsg( AM_MSG_DEINIT, 0, 0 );

    // we are done here with this module...
    FreeLibrary( f->hModule );

    // and with the pointer
    delete f;
    f = nullptr;
  }// for each modules.

  // and remove everything
  _pluginsContainer.erase( _pluginsContainer.begin(), _pluginsContainer.end() );

  myodd::threads::Lock guardPlugin(_pluginKey);
  delete _amPlugin;
  _amPlugin = nullptr;
}

/**
* \brief get the version number of application monitor
* \return the version number.
*/
double PluginVirtualMachine::Version()
{
  return GetApi().Version();
}

/**
* \brief output a fading message
* \param sText the message to output
* \param elapseMiliSecondsBeforeFadeOut how long we want to show the message before fade out.
* \param totalMilisecondsToShowMessage how long we want to display the message, including fade out.
*/
bool PluginVirtualMachine::Say(const wchar_t* sText, const unsigned int elapseMiliSecondsBeforeFadeOut, const unsigned int totalMilisecondsToShowMessage)
{
  return GetApi().Say(sText, elapseMiliSecondsBeforeFadeOut, totalMilisecondsToShowMessage);
}

/**
 * \brief get a command and pass it to the buffer.
 * \param idx the index number of the command we are after.
 * \param nBufferLength the max length of the buffer.
 * \param lpBuffer the data container
 * \return the actual size of the command.
 */
size_t PluginVirtualMachine::GetCommand(const UINT idx, const DWORD nBufferLength, wchar_t* lpBuffer)
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
int PluginVirtualMachine::GetAction(DWORD nBufferLength, wchar_t* lpBuffer)
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
bool PluginVirtualMachine::ExecuteInPlugin(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged )
{
  return GetApi().Execute(module, cmdLine, isPrivileged, nullptr );
}

/**
* Todo
* @see __super::getString
* @param void
* @param void
* @return void
*/
int PluginVirtualMachine::GetString(DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote)
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
int PluginVirtualMachine::GetFile(UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote)
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
int PluginVirtualMachine::GetFolder(UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote)
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
int PluginVirtualMachine::GetUrl(UINT idx, DWORD nBufferLength, wchar_t* lpBuffer, bool bQuote)
{
  return GetApi().GetUrl( idx,  nBufferLength,  lpBuffer, bQuote );
}

/**
* Todo
* @see __super::addAction
* @param void
* @param void
* @return void
*/
bool PluginVirtualMachine::AddAction(const wchar_t* szText, const wchar_t* szPath)
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
bool PluginVirtualMachine::RemoveAction(const wchar_t* szText, const wchar_t* szPath)
{
  return GetApi().RemoveAction( szText, szPath);
}

/**
* Get the action monitor version string.
* @param DWORD the max size of the buffer.
* @param wchar_t* the buffer that will contain the return data, (version).
* @return bool success or not if there was an error.
*/
bool PluginVirtualMachine::GetVersion(DWORD nBufferLength, wchar_t* lpBuffer)
{
  return GetApi().GetVersion( nBufferLength, lpBuffer);
}

/**
* Find an action.
* @param UINT the action number we are after.
* @param const wchar_t* the action name we are looking for.
* @param DWORD the max size of the buffer.
* @param wchar_t* the buffer that will contain the return data, (version).
* @return bool if the command exists or not.
*/
bool PluginVirtualMachine::FindAction
(
  UINT idx,
  const wchar_t* lpCommand,
  DWORD nBufferLength,
  wchar_t* lpBuffer
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

/** 
 * Log a message.
 * @param unsigned int logType the message type we are logging.
 * @param const wchar_t* lpText the text we wish to log.
 * @return none.
 */
void PluginVirtualMachine::Log(unsigned int logType, const wchar_t* lpText)
{
  GetApi().Log(logType, lpText);
}
#endif /* ACTIONMONITOR_API_PLUGIN*/
