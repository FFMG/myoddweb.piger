#include "stdafx.h"
#ifdef ACTIONMONITOR_API_LUA
#include "luaapi.h"
#include "luavirtualmachine.h"
#include "ActionMonitor.h"

/**
 * \copydoc
 */
LuaVirtualMachine::LuaVirtualMachine(IApplication& application, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener) :
  IVirtualMachine(application, messagesHandler, iIpcListener )
{
}

/**
 * Todo
 * @param void
 * @return void
 */
LuaVirtualMachine::~LuaVirtualMachine()
{
  //  dispose all 
  Dispose();
}

/**
 * Try and displose of all the lua states.
 */
void LuaVirtualMachine::Dispose()
{
  VirtualMachineLists<lua_State*, LuaApi>::Instance().Dispose([](lua_State* lua, LuaApi* api)
  {
    // close the lua
    lua_close(lua);
  });
}

/**
 * \brief Try and dispose of a lua state.
 * \param, lua the lua we are trying to get rid of.
 */
void LuaVirtualMachine::Dispose(lua_State* lua)
{
  VirtualMachineLists<lua_State*, LuaApi>::Instance().Dispose(lua, [](lua_State* lua, LuaApi* api)
  {
    // close the lua
    lua_close(lua);
  });
}

/**
 * Create a new lua state
 * @param IActiveAction* action the current action
 * @return lua_State* a newly created lua state,
 */
lua_State* LuaVirtualMachine::CreateState(LuaApi* api)
{
  // create the new state
  lua_State* lua = luaL_newstate();
  if (nullptr == lua)
  {
    return nullptr;
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
  VirtualMachineLists<lua_State*, LuaApi>::Instance().AddApi(lua, api);

  // finally return the lua.
  return lua;
}

int LuaVirtualMachine::Execute(const IActiveAction& action, const std::wstring& pluginFile)
{
  const auto api = new LuaApi(action, GetApplication(), GetMessagesHandler() );
  try
  {
    const auto lua = CreateState(api);
    if (nullptr == lua)
    {
      delete api;
      return -1;
    }

    const auto file = myodd::strings::WString2String(pluginFile);
    auto resultOfCall = luaL_loadfile(lua, file.c_str());
    if (resultOfCall != 0)
    {
      const auto sErr = myodd::strings::String2WString(lua_tostring(lua, -1));
      myodd::log::LogError(_T("LUA Err: %s"), sErr.c_str());
    }
    else
    {
      // execute Lua program
      resultOfCall = lua_pcall(lua, 0, LUA_MULTRET, 0);
    }

    // clean lua
    Dispose(lua);

    // finally clean the api itself.
    delete api;

    return resultOfCall;
  }
  catch( ... )
  {
    delete api;
    return -1;
  }
}

/**
 * \brief Check if a given file extension is used by this API or not.
 * \param file the file we are checking
 * \return true|false if the given extension is LUA or not.
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
  return VirtualMachineLists<lua_State*, LuaApi>::Instance().GetApi(lua);
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