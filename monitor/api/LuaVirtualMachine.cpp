#include "stdafx.h"
#ifdef ACTIONMONITOR_API_LUA
#include "luaapi.h"
#include "luavirtualmachine.h"
#include "ActionMonitor.h"

/**
 * Todo
 * @param void
 * @return void
 */
LuaVirtualMachine::LuaVirtualMachine()
{
}

/**
 * Todo
 * @param void
 * @return void
 */
LuaVirtualMachine::~LuaVirtualMachine(void)
{
  //  dispose all 
  Dispose();
}

/**
 * Try and displose of all the lua states.
 */
void LuaVirtualMachine::Dispose()
{
  // get the locks.
  _mutex.lock();

  // and clear everything.
  for (state_api::iterator it = _lua_Api.begin();
       it != _lua_Api.end();
       ++it 
      )
  {
    // close the lua
    lua_close( it->first );
  }

  // we can now remove everything.
  // as we have cleared the memory.
  _lua_Api.clear();

  // release the lock.
  _mutex.unlock();
}

/**
 * Try and displose of a lua state.
 * @param lua_State* lua the lua we are trying to get rid of.
 */
void LuaVirtualMachine::Dispose(lua_State* lua)
{
  // find the lua...
  _mutex.lock();
  state_api::iterator it = _lua_Api.find(lua);

  // does it exist?
  if (it != _lua_Api.end())
  {
    // close the lua
    lua_close(lua);
    
    //  yes, so we can remove it.
    _lua_Api.erase(it);
  }
  _mutex.unlock();
}

/**
 * Create a new lua state
 * @param ActiveAction* action the current action
 * @return lua_State* a newly created lua state,
 */
lua_State* LuaVirtualMachine::CreateState(LuaApi* api)
{
  // create the new state
  lua_State* lua = luaL_newstate();
  if (NULL == lua)
  {
    return NULL;
  }
  luaL_openlibs( lua ); // provides io.*

  // register our LUA functions.
  lua_register( lua, "am_say", LuaVirtualMachine::Say);
  lua_register( lua, "am_version", LuaVirtualMachine::Version);
  lua_register( lua, "am_getCommand", LuaVirtualMachine::GetCommand);
  lua_register( lua, "am_getAction", LuaVirtualMachine::GetAction);
  lua_register( lua, "am_getCommandCount", LuaVirtualMachine::GetCommandCount);
  lua_register( lua, "am_execute", LuaVirtualMachine::Execute);
  lua_register( lua, "am_getstring", LuaVirtualMachine::Getstring);
  lua_register( lua, "am_getfile", LuaVirtualMachine::Getfile);
  lua_register( lua, "am_getfolder", LuaVirtualMachine::Getfolder);
  lua_register( lua, "am_geturl", LuaVirtualMachine::Geturl);
  lua_register( lua, "am_addAction", LuaVirtualMachine::AddAction);
  lua_register( lua, "am_removeAction", LuaVirtualMachine::RemoveAction);
  lua_register( lua, "am_getVersion", LuaVirtualMachine::GetVersion);
  lua_register( lua, "am_findAction", LuaVirtualMachine::FindAction);
  lua_register( lua, "am_log", LuaVirtualMachine::Log );

  // we can now add it to our list.
  _mutex.lock();
  _lua_Api[lua] = api;
  _mutex.unlock();

  // finally return the lua.
  return lua;
}

/**
 * Todo
 * @param const MYODD_STRING& szFile the file/script we would like to load.
 * @param LuaApi api the created api we will be using.
 * @return int result of the operation.
 */
int LuaVirtualMachine::ExecuteInThread(const MYODD_STRING& szFile, LuaApi* api)
{
  lua_State* lua = CreateState(api);
  if( NULL == lua )
  {
    return -1;
  }

  USES_CONVERSION;

  int resultOfCall = luaL_loadfile( lua , T_T2A(szFile.c_str() ) );
  if(resultOfCall != 0 )
  {
    MYODD_STRING sErr = T_A2T( lua_tostring( lua, -1) );
    myodd::log::LogError( _T("LUA Err: %s"), sErr.c_str() );
  }
  else
  {
    // execute Lua program
    resultOfCall = lua_pcall(lua, 0, LUA_MULTRET, 0);
  }

  Dispose(lua);

  return resultOfCall;
}

/**
 * Check if a given file extension is used by this API or not.
 * @param const MYODD_STRING& file the file we are checking
 * @return bool true|false if the given extension is LUA or not.
 */
bool LuaVirtualMachine::IsExt(const MYODD_STRING& file )
{
  return myodd::files::IsExtension(file, _T("lua"));
}

LuaApi& LuaVirtualMachine::GetApi(lua_State* lua)
{
#ifndef ACTIONMONITOR_API_LUA
  throw - 1;
#else
  // get our current self.
  auto& lvm = App().VirtualMachinesHandler().Get< LuaVirtualMachine>();

  // find the lua...
  lvm._mutex.lock();
  const auto it = lvm._lua_Api.find( lua );

  // does it exist?
  if (it == lvm._lua_Api.end())
  {
    // we could not find this lua!
    // has it been destroyed?
    lvm._mutex.unlock();
    throw - 1;
  }

  //  get the value before we lock it...
  const auto api = it->second;
  lvm._mutex.unlock();

  return *api;
#endif
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::Version(lua_State *lua)
{
  return GetApi(lua).Version( lua );
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::Execute(lua_State *lua)
{
  return GetApi(lua).Execute(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::GetCommandCount(lua_State *lua)
{
  return GetApi(lua).GetCommandCount(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::GetCommand(lua_State *lua)
{
  return GetApi(lua).GetCommand(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::GetAction(lua_State *lua)
{
  return GetApi(lua).GetAction(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::Say(lua_State *lua)
{
  return GetApi(lua).Say(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::Getstring(lua_State *lua)
{
  return GetApi(lua).Getstring(lua);
}

/**
* Get the action monitor version string.
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::GetVersion(lua_State *lua)
{
  return GetApi(lua).GetVersion(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::Getfile(lua_State *lua)
{
  return GetApi(lua).GetFile(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::Getfolder(lua_State *lua)
{
  return GetApi(lua).GetFolder(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::Geturl(lua_State *lua)
{
  return GetApi(lua).GetUrl(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::AddAction(lua_State *lua)
{
  return GetApi(lua).AddAction(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::RemoveAction(lua_State *lua)
{
  return GetApi(lua).RemoveAction(lua);
}

/**
 * Get the action monitor version string.
 * @param lua_State *
 * @return int
 */
int LuaVirtualMachine::FindAction(lua_State *lua)
{
  return GetApi(lua).FindAction(lua);
}

/**
 * Get the action monitor version string.
 * @param lua_State *
 * @return int
 */
int LuaVirtualMachine::Log(lua_State *lua)
{
  return GetApi(lua).Log(lua);
}
#endif /* ACTIONMONITOR_API_LUA */