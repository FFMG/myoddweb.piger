#pragma once

// add the LUA libs
#ifdef ACTIONMONITOR_API_LUA

#include "helperapi.h"

// support for LUA
extern "C" 
{
  #include "lua/src/lua.h"
  #include "lua/src/lualib.h"
  #include "lua/src/lauxlib.h"
}

# ifdef _WIN64
#  pragma comment(lib, "lua64.lib" )
# else
#  pragma comment(lib, "lua.lib" )
# endif

class LuaApi final : public HelperApi
{
public:
  LuaApi(const ActiveAction& action);
  virtual ~LuaApi();

public:

  // static functions
  int Version (lua_State *lua) const;
  int Say (lua_State *lua) const;
  int GetAction (lua_State *lua) const;
  int GetCommand (lua_State *lua) const;
  int GetCommandCount (lua_State *lua) const;
  int Execute (lua_State *lua) const;
  int Getstring( lua_State *lua ) const;
  int GetVersion( lua_State *lua ) const;
  int GetFile( lua_State *lua ) const;
  int GetFolder( lua_State *lua ) const;
  int GetUrl( lua_State *lua ) const;
  int AddAction( lua_State *lua ) const;
  int RemoveAction( lua_State *lua ) const;
  int FindAction( lua_State *lua ) const;
  int Log(lua_State *lua) const;
};

#endif /* ACTIONMONITOR_API_LUA */