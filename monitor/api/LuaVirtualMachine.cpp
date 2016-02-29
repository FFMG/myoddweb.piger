#include "stdafx.h"
#include "luaapi.h"
#include "luavirtualmachine.h"
#include "..\ActionMonitor\ActionMonitor.h"

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

    // delete the api
    delete it->second;
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

    // delete the api
    delete it->second;

    //  yes, so we can remove it.
    _lua_Api.erase(it);
  }
  _mutex.unlock();
}

/**
 * Create a new lua state
 * @return lua_State* a newly created lua state,
 */
lua_State* LuaVirtualMachine::Create()
{
  // create the new state
  lua_State* lua = luaL_newstate();
  if (NULL == lua)
  {
    return NULL;
  }
  luaL_openlibs( lua ); // provides io.*

  // register our LUA functions.
  lua_register( lua, "am_say", LuaVirtualMachine::say);
  lua_register( lua, "am_version", LuaVirtualMachine::version);
  lua_register( lua, "am_getCommand", LuaVirtualMachine::getCommand);
  lua_register( lua, "am_getAction", LuaVirtualMachine::getAction);
  lua_register( lua, "am_getCommandCount", LuaVirtualMachine::getCommandCount);
  lua_register( lua, "am_execute", LuaVirtualMachine::execute);
  lua_register( lua, "am_getstring", LuaVirtualMachine::getstring);
  lua_register( lua, "am_getfile", LuaVirtualMachine::getfile);
  lua_register( lua, "am_getfolder", LuaVirtualMachine::getfolder);
  lua_register( lua, "am_geturl", LuaVirtualMachine::geturl);
  lua_register( lua, "am_addAction", LuaVirtualMachine::addAction);
  lua_register( lua, "am_removeAction", LuaVirtualMachine::removeAction);
  lua_register( lua, "am_getVersion", LuaVirtualMachine::getVersion);
  lua_register( lua, "am_findAction", LuaVirtualMachine::findAction);

  // we can now add it to our list.
  _mutex.lock();
  _lua_Api[lua] = new luaapi();
  _mutex.unlock();

  // finally return the lua.
  return lua;
}

/**
 * Todo
 * @param void
 * @return void
 */
int LuaVirtualMachine::LoadFile( LPCTSTR luaFile )
{
  lua_State* lua = Create();
  if( NULL == lua )
  {
    return -1;
  }

  USES_CONVERSION;

  int resultOfCall = luaL_loadfile( lua , T_T2A(luaFile) );
  if(resultOfCall != 0 )
  {
    STD_TSTRING sErr = T_A2T( lua_tostring( lua, -1) );
    myodd::log::LogError( _T("LUA Err: %s"), sErr.c_str() );
  }
  else
  {
    // execute Lua program
    resultOfCall = lua_pcall(lua, 0, LUA_MULTRET, 0);
  }

  // we can now dispose of the lua
  Dispose(lua);

  return resultOfCall;
}

/**
 * Check if a given file extension is used by this API or not.
 * @param LPCTSTR the extensions.
 * @return bool true|false if the given extension is LUA or not.
 */
bool LuaVirtualMachine::IsLuaExt( LPCTSTR ext )
{
  return ( _tcsicmp( ext, _T("lua") ) == 0 );
}

luaapi& LuaVirtualMachine::GetApi(lua_State* lua)
{
  // get our current self.
  LuaVirtualMachine* lvm = App().GetLuaVirtualMachine();

  // find the lua...
  lvm->_mutex.lock();
  state_api::iterator it = lvm->_lua_Api.find( lua );

  // does it exist?
  if (it == lvm->_lua_Api.end())
  {
    // we could not find this lua!
    // has it been destroyed?
    lvm->_mutex.unlock();
    throw - 1;
  }

  //  get the value before we lock it...
  luaapi* api = it->second;
  lvm->_mutex.unlock();

  return *api;
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::version(lua_State *lua)
{
  return GetApi(lua).version( lua );
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::execute(lua_State *lua)
{
  return GetApi(lua).execute(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::getCommandCount(lua_State *lua)
{
  return GetApi(lua).getCommandCount(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::getCommand(lua_State *lua)
{
  return GetApi(lua).getCommand(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::getAction(lua_State *lua)
{
  return GetApi(lua).getAction(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::say(lua_State *lua)
{
  return GetApi(lua).say(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::getstring(lua_State *lua)
{
  return GetApi(lua).getstring(lua);
}

/**
* Get the action monitor version string.
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::getVersion(lua_State *lua)
{
  return GetApi(lua).getVersion(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::getfile(lua_State *lua)
{
  return GetApi(lua).getfile(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::getfolder(lua_State *lua)
{
  return GetApi(lua).getfolder(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::geturl(lua_State *lua)
{
  return GetApi(lua).geturl(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::addAction(lua_State *lua)
{
  return GetApi(lua).addAction(lua);
}

/**
* Todo
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::removeAction(lua_State *lua)
{
  return GetApi(lua).removeAction(lua);
}

/**
* Get the action monitor version string.
* @param lua_State *
* @return int
*/
int LuaVirtualMachine::findAction(lua_State *lua)
{
  return GetApi(lua).findAction(lua);
}